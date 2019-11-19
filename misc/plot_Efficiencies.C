{
  gROOT.ProcessLine(".x /home/rberner/nd280work/.rootlogin.C");
  gStyle->SetPadLeftMargin(0.15); //0.18
  gStyle->SetPadBottomMargin(0.20); //0.16
  gStyle->SetPadTopMargin(0.08); //0.16 //1.05,"Y"
  gStyle->SetPadRightMargin(0.05);
  //gStyle->SetTextSize(0.1);
  //gStyle->SetLabelSize(0.1);
  //gStyle->SetTitleSize(0.1);
  //gStyle->SetTitleFontSize(0.1);
  TGaxis::SetMaxDigits(5);
  //gROOT->Reset();

  TH1::SetDefaultSumw2();

  double pi = 3.141592653589793238463;


  // Create TChain
  TChain *NEUT = new TChain("default");


  // Read data files
  NEUT->Add("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/NEUT_Run3_20180117/water_01.root");
  NEUT->Add("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/NEUT_Run3_20180117/water_02.root");
  NEUT->Add("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/NEUT_Run3_20180117/air_01.root");
  NEUT->Add("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/NEUT_Run3_20180117/air_02.root");
  NEUT->Add("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/NEUT_Run3_20180117/air_03.root");
  NEUT->Add("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/NEUT_Run3_20180117/air_04.root");
  NEUT->Add("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/NEUT_Run3_20180117/sand_01.root");
  NEUT->Add("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/NEUT_Run3_20180117/sand_02.root");
/*    DataSample* mc_magnet_run5_01_GENIE   = new DataSample("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/GENIE_Run3_20180117/water_GENIE_01.root");
    DataSample* mc_magnet_run5_02_GENIE   = new DataSample("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/GENIE_Run3_20180117/water_GENIE_02.root");
    DataSample* mc_magnet_run6_01_GENIE   = new DataSample("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/GENIE_Run3_20180117/air_GENIE_01.root");
    DataSample* mc_magnet_run6_02_GENIE   = new DataSample("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/GENIE_Run3_20180117/air_GENIE_02.root");
    DataSample* mc_magnet_run6_03_GENIE   = new DataSample("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/GENIE_Run3_20180117/air_GENIE_03.root");
    DataSample* mc_magnet_run6_04_GENIE   = new DataSample("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/GENIE_Run3_20180117/air_GENIE_04.root");*/


  // Select the branch called "default"
  NEUT->Branch("default");


  // Number of branches in tree
  int n_branch = 3;


  // Number of toys
  int n_toy = 0;


  // Set branch ID:
  int all_branch = 0;
  int Fwd_branch = 0;
  int Bwd_branch = 1;
  int HA_branch = 2;


  // Set accumulation level
  int all_acc_lvl = 1;
  int Fwd_acc_lvl = 11;
  int Bwd_acc_lvl = 12;
  int HA_acc_lvl = 6;


  // Set title of plot
  char plottitle[40] = "TiTlE";


  // Set number of bins and range
  int n_bins = 20;
  float x_min = -1;
  float x_max = 1;


  //Define variables and link in tree
  int Cut[Bwd_acc_lvl+1][n_branch]; // select maximum acc_lvl
  char buf[8];
  for (int i=0; i<(Bwd_acc_lvl+1); i++) {
    snprintf(buf, 8, "cut%d", i);
    NEUT->SetBranchAddress(buf,&Cut[i][0]);
  }


  // Define variables and link in tree
  float selmu_truecostheta;
  NEUT->SetBranchAddress("selmu_truecostheta",&selmu_truecostheta); // in " ": as defined in the .root file
  int truelepton_starts_in_FGD1FV;
  NEUT->SetBranchAddress("truelepton_starts_in_FGD1FV",&truelepton_starts_in_FGD1FV);
  int reactionCC;
  NEUT->SetBranchAddress("reactionCC",&reactionCC); // in " ": as defined in the .root file
  int particle;
  NEUT->SetBranchAddress("selmu_PDG",&particle);




  // Define histograms
  TH1D *NEUT_all   = new TH1D("NEUT_all", "NEUT all", n_bins,x_min,x_max);
  TH1D *NEUT_Fwd   = new TH1D("NEUT_Fwd", "NEUT Fwd", n_bins,x_min,x_max);
  TH1D *NEUT_Bwd   = new TH1D("NEUT_Bwd", "NEUT Bwd", n_bins,x_min,x_max);
  TH1D *NEUT_HA    = new TH1D("NEUT_HA",  "NEUT HA",  n_bins,x_min,x_max);


  // Define error propagation
  NEUT_all->Sumw2();
  NEUT_Fwd->Sumw2();
  NEUT_Bwd->Sumw2();
  NEUT_HA ->Sumw2();


  // Loop over all events in all branch
  for (int i=0; i<NEUT->GetEntries(); i++) {
    NEUT->GetEntry(i);
    int cut_arg = 0;
    for (int j=0; j<(all_acc_lvl+1); j++) {
      if (Cut[j][all_branch]) cut_arg++;
    }
    if (cut_arg < (all_acc_lvl+1) || truelepton_starts_in_FGD1FV != 1 || reactionCC != 1 || particle != -13) continue; // Only take into account the events which passed the specified accum_level
    else {
      NEUT_all -> Fill(selmu_truecostheta);
    }
  }

  // Loop over all events in Fwd branch
  for (int i=0; i<NEUT->GetEntries(); i++) {
    NEUT->GetEntry(i);
    int cut_arg = 0;
    for (int j=0; j<(Fwd_acc_lvl+1); j++) {
      if (Cut[j][Fwd_branch]) cut_arg++;
    }
    if (cut_arg < (Fwd_acc_lvl+1) || truelepton_starts_in_FGD1FV != 1 || reactionCC != 1 || particle != -13) continue; // Only take into account the events which passed the specified accum_level
    else {
      NEUT_Fwd -> Fill(selmu_truecostheta);
    }
  }

  // Loop over all events in Bwd branch
  for (int i=0; i<NEUT->GetEntries(); i++) {
    NEUT->GetEntry(i);
    int cut_arg = 0;
    for (int j=0; j<(Bwd_acc_lvl+1); j++) {
      if (Cut[j][Bwd_branch]) cut_arg++;
    }
    if (cut_arg < (Bwd_acc_lvl+1) || truelepton_starts_in_FGD1FV != 1 || reactionCC != 1 || particle != -13) continue; // Only take into account the events which passed the specified accum_level
    else {
      NEUT_Bwd -> Fill(selmu_truecostheta);
    }
  }

  // Loop over all events in HA branch
  for (int i=0; i<NEUT->GetEntries(); i++) {
    NEUT->GetEntry(i);
    int cut_arg = 0;
    for (int j=0; j<(HA_acc_lvl+1); j++) {
      if (Cut[j][HA_branch]) cut_arg++;
    }
    if (cut_arg < (HA_acc_lvl+1) || truelepton_starts_in_FGD1FV != 1 || reactionCC != 1 || particle != -13) continue; // Only take into account the events which passed the specified accum_level
    else {
      NEUT_HA -> Fill(selmu_truecostheta);
    }
  }


  // Define histograms for the efficiencies
  TH1D *NEUT_Fwd_eff   = new TH1D("NEUT_Fwd_eff",   "NEUT Fwd eff",   n_bins,x_min,x_max);
  TH1D *NEUT_Bwd_eff   = new TH1D("NEUT_Bwd_eff",   "NEUT Bwd eff",   n_bins,x_min,x_max);
  TH1D *NEUT_HA_eff    = new TH1D("NEUT_HA_eff",    "NEUT HA eff",    n_bins,x_min,x_max);
  TH1D *NEUT_Total = (TH1D*) NEUT_Fwd->Clone();
  TH1D *NEUT_Total_eff = new TH1D("NEUT_Total_eff", "NEUT Total eff", n_bins,x_min,x_max);

  // Define error propagation
  NEUT_Fwd_eff  ->Sumw2();
  NEUT_Bwd_eff  ->Sumw2();
  NEUT_HA_eff   ->Sumw2();
  NEUT_Total    ->Sumw2();
  NEUT_Total_eff->Sumw2();

  // Calculate the total number of selected events
  NEUT_Total->Add(NEUT_Bwd,1);
  NEUT_Total->Add(NEUT_HA, 1);


  // Divide histograms to get the efficiencies
  NEUT_Fwd_eff  ->Divide(NEUT_Fwd,     NEUT_all, 1.0, 1.0, "B");
  NEUT_Bwd_eff  ->Divide(NEUT_Bwd,     NEUT_all, 1.0, 1.0, "B");
  NEUT_HA_eff   ->Divide(NEUT_HA,      NEUT_all, 1.0, 1.0, "B");
  NEUT_Total_eff->Divide(NEUT_Total,   NEUT_all, 1.0, 1.0, "B");




  // Plot: NEUT_all
  // --------------------------------------------------------------------------
  // Define the Canvas
  TCanvas *c0 = new TCanvas("c0","canvas", 1000, 800);
  c0->Divide(1,2,0,0);

  // First pad:
  c0->cd(1);
  //c0->GetPad(1)->SetMin(1);
  c0->GetPad(1)->SetLogy(1);
  NEUT_all->SetTitle("");
  //NEUT_all->GetXaxis()->SetTitle("true costheta [-]");
  NEUT_all->GetYaxis()->SetTitle("# signal events");
  NEUT_all->GetXaxis()->SetLabelSize(0.1);
  NEUT_all->GetYaxis()->SetLabelSize(0.1);
  //NEUT_all->GetXaxis()->SetTitleSize(0.1);
  NEUT_all->GetYaxis()->SetTitleSize(0.12);
  //NEUT_all->GetXaxis()->SetTitleOffset(1.02);
  NEUT_all->GetYaxis()->SetTitleOffset(0.7);
  //NEUT_all->GetXaxis()->SetLabelSize(0.09);
  //NEUT_all->GetYaxis()->SetLabelSize(0.09);

  //NEUT_all->SetMarkerStyle(kFullCircle);
  NEUT_all->SetMarkerColor(kBlack);
  NEUT_all->SetLineColor(kBlack);
  NEUT_all->SetLineWidth(3);

  NEUT_Fwd->SetMarkerStyle(kFullCircle);
  NEUT_Fwd->SetMarkerColor(kGreen);
  NEUT_Fwd->SetLineColor(kGreen);

  NEUT_Bwd->SetMarkerStyle(kFullCircle);
  NEUT_Bwd->SetMarkerColor(kBlue);
  NEUT_Bwd->SetLineColor(kBlue);

  NEUT_HA->SetMarkerStyle(kFullCircle);
  NEUT_HA->SetMarkerColor(kRed);
  NEUT_HA->SetLineColor(kRed);

  NEUT_Total->SetMarkerStyle(kFullCircle);
  NEUT_Total->SetMarkerColor(kMagenta+1);
  NEUT_Total->SetLineColor(kMagenta+1);

  //NEUT_all->SetMinimum(1);
  NEUT_all  ->Draw("hist");
  NEUT_Fwd  ->Draw("E1 same");
  NEUT_Bwd  ->Draw("E1 same");
  NEUT_HA   ->Draw("E1 same");
  NEUT_Total->Draw("E1 same");
  NEUT_all  ->Draw("hist same");

  // Plot legend
  leg = new TLegend(0.2, 0.47, 0.4, 0.92);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.09);
  leg->SetFillStyle(0);
  leg->AddEntry(NEUT_all,   "all true CC in FGD1 FV","l"); // "t", "f", "true", "false", "l"
  leg->AddEntry(NEUT_Fwd,   "selected true Fwd","lep"); // "t", "f", "true", "false", "l"
  leg->AddEntry(NEUT_Bwd,   "selected true Bwd","lep"); // "t", "f", "true", "false", "l"
  leg->AddEntry(NEUT_HA,    "selected true HA","lep"); // "t", "f", "true", "false", "l"
  leg->AddEntry(NEUT_Total, "selected true Fwd+Bwd+HA","lep"); // "t", "f", "true", "false", "l"
  leg->Draw();


  // Second pad:
  c0->cd(2);
  NEUT_Fwd_eff->SetMinimum(-0.05);
  NEUT_Fwd_eff->SetMaximum(1.05);

  c0->GetPad(2)->SetLogy(0);

  NEUT_Fwd_eff->SetTitle("");
  NEUT_Fwd_eff->GetXaxis()->SetTitle("true costheta [-]");
  NEUT_Fwd_eff->GetYaxis()->SetTitle("selection efficiency");
  NEUT_Fwd_eff->GetXaxis()->SetLabelSize(0.1);
  NEUT_Fwd_eff->GetYaxis()->SetLabelSize(0.1);
  NEUT_Fwd_eff->GetXaxis()->SetTitleSize(0.1);
  NEUT_Fwd_eff->GetYaxis()->SetTitleSize(0.1);
  NEUT_Fwd_eff->GetYaxis()->SetTitleOffset(0.84);

  NEUT_Fwd_eff->SetMarkerStyle(kFullCircle);
  NEUT_Fwd_eff->SetMarkerColor(kGreen);
  NEUT_Fwd_eff->SetLineColor(kGreen);

  NEUT_Bwd_eff->SetMarkerStyle(kFullCircle);
  NEUT_Bwd_eff->SetMarkerColor(kBlue);
  NEUT_Bwd_eff->SetLineColor(kBlue);

  NEUT_HA_eff->SetMarkerStyle(kFullCircle);
  NEUT_HA_eff->SetMarkerColor(kRed);
  NEUT_HA_eff->SetLineColor(kRed);

  //NEUT_Total_eff->SetMarkerStyle(kFullCircle);
  NEUT_Total_eff->SetMarkerColor(kMagenta+1);
  NEUT_Total_eff->SetLineColor(kMagenta+1);
  NEUT_Total_eff->SetLineWidth(3);

  NEUT_Fwd_eff  ->Draw("E1");
  NEUT_Bwd_eff  ->Draw("E1 same");
  NEUT_HA_eff   ->Draw("E1 same");
  NEUT_Total_eff->Draw("hist same");


  //c0->Update();
  c0->SaveAs("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/Plots/PNGs/SelectionEfficiency.png");
}
