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
  int n_bins = 25;
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
  NEUT->SetBranchAddress("selmu_costheta",&selmu_truecostheta); // in " ": as defined in the .root file
  int truelepton_starts_in_FGD1FV;
  NEUT->SetBranchAddress("truelepton_starts_in_FGD1FV",&truelepton_starts_in_FGD1FV);
  int reactionCC;
  NEUT->SetBranchAddress("reactionCC",&reactionCC); // in " ": as defined in the .root file
  int particle;
  NEUT->SetBranchAddress("selmu_PDG",&particle);


  // Define histograms
  TH1D *NEUT_Fwd_selected_all    = new TH1D("NEUT_Fwd_all",  "NEUT Fwd all",  n_bins,x_min,x_max);
  TH1D *NEUT_Fwd_selected_true   = new TH1D("NEUT_Fwd_true", "NEUT Fwd true", n_bins,x_min,x_max);
  TH1D *NEUT_Bwd_selected_all    = new TH1D("NEUT_Bwd_all",  "NEUT Bwd all",  n_bins,x_min,x_max);
  TH1D *NEUT_Bwd_selected_true   = new TH1D("NEUT_Bwd_true", "NEUT Bwd true", n_bins,x_min,x_max);
  TH1D *NEUT_HA_selected_all     = new TH1D("NEUT_HA_all",   "NEUT HA all",   n_bins,x_min,x_max);
  TH1D *NEUT_HA_selected_true    = new TH1D("NEUT_HA_true",  "NEUT HA true",  n_bins,x_min,x_max);


  // Define error propagation
  NEUT_Fwd_selected_all ->Sumw2();
  NEUT_Fwd_selected_true->Sumw2();
  NEUT_Bwd_selected_all ->Sumw2();
  NEUT_Bwd_selected_true->Sumw2();
  NEUT_HA_selected_all  ->Sumw2();
  NEUT_HA_selected_true ->Sumw2();


  // Loop over all events in Fwd branch
  for (int i=0; i<NEUT->GetEntries(); i++) {
    NEUT->GetEntry(i);
    int cut_arg = 0;
    for (int j=0; j<(Fwd_acc_lvl+1); j++) {
      if (Cut[j][Fwd_branch]) cut_arg++;
    }
    if (cut_arg < (Fwd_acc_lvl+1)) continue; // Only take into account the events which passed the specified accum_level
    else {
      NEUT_Fwd_selected_all -> Fill(selmu_truecostheta);
      if ( truelepton_starts_in_FGD1FV == 1 && reactionCC == 1 && particle == -13 ) {
        NEUT_Fwd_selected_true -> Fill(selmu_truecostheta);
      }
    }
  }

  // Loop over all events in Bwd branch
  for (int i=0; i<NEUT->GetEntries(); i++) {
    NEUT->GetEntry(i);
    int cut_arg = 0;
    for (int j=0; j<(Bwd_acc_lvl+1); j++) {
      if (Cut[j][Bwd_branch]) cut_arg++;
    }
    if (cut_arg < (Bwd_acc_lvl+1)) continue; // Only take into account the events which passed the specified accum_level
    else {
      NEUT_Bwd_selected_all -> Fill(selmu_truecostheta);
      if ( (truelepton_starts_in_FGD1FV == 1) && (reactionCC == 1) && (particle == -13) ) {
        NEUT_Bwd_selected_true -> Fill(selmu_truecostheta);
      }
    }
  }

  // Loop over all events in HA branch
  for (int i=0; i<NEUT->GetEntries(); i++) {
    NEUT->GetEntry(i);
    int cut_arg = 0;
    for (int j=0; j<(HA_acc_lvl+1); j++) {
      if (Cut[j][HA_branch]) cut_arg++;
    }
    if (cut_arg < (HA_acc_lvl+1)) continue; // Only take into account the events which passed the specified accum_level
    else {
      NEUT_HA_selected_all -> Fill(selmu_truecostheta);
      if ( truelepton_starts_in_FGD1FV == 1 && reactionCC == 1 && particle == -13 ) {
        NEUT_HA_selected_true -> Fill(selmu_truecostheta);
      }
    }
  }


  // Define histograms for the efficiencies
  TH1D *NEUT_Fwd_pur   = new TH1D("NEUT_Fwd_pur",   "NEUT Fwd pur",   n_bins,x_min,x_max);
  TH1D *NEUT_Bwd_pur   = new TH1D("NEUT_Bwd_pur",   "NEUT Bwd pur",   n_bins,x_min,x_max);
  TH1D *NEUT_HA_pur    = new TH1D("NEUT_HA_pur",    "NEUT HA pur",    n_bins,x_min,x_max);
  TH1D *NEUT_Total_selected_all  = (TH1D*) NEUT_Fwd_selected_all ->Clone();
  TH1D *NEUT_Total_selected_true = (TH1D*) NEUT_Fwd_selected_true->Clone();
  TH1D *NEUT_Total_pur = new TH1D("NEUT_Total_pur", "NEUT Total pur", n_bins,x_min,x_max);

  // Define error propagation
  NEUT_Fwd_pur  ->Sumw2();
  NEUT_Bwd_pur  ->Sumw2();
  NEUT_HA_pur   ->Sumw2();
  NEUT_Total_selected_all ->Sumw2();
  NEUT_Total_selected_true->Sumw2();
  NEUT_Total_pur->Sumw2();

  // Calculate the total number of selected events
  NEUT_Total_selected_all ->Add(NEUT_Bwd_selected_all, 1);
  NEUT_Total_selected_all ->Add(NEUT_HA_selected_all,  1);
  NEUT_Total_selected_true->Add(NEUT_Bwd_selected_true,1);
  NEUT_Total_selected_true->Add(NEUT_HA_selected_true, 1);

  // Divide histograms to get the efficiencies
  NEUT_Fwd_pur  ->Divide(NEUT_Fwd_selected_true,   NEUT_Fwd_selected_all,   1.0, 1.0, "B");
  NEUT_Bwd_pur  ->Divide(NEUT_Bwd_selected_true,   NEUT_Bwd_selected_all,   1.0, 1.0, "B");
  NEUT_HA_pur   ->Divide(NEUT_HA_selected_true,    NEUT_HA_selected_all,    1.0, 1.0, "B");
  NEUT_Total_pur->Divide(NEUT_Total_selected_true, NEUT_Total_selected_all, 1.0, 1.0, "B");




  // Plot: NEUT_all
  // --------------------------------------------------------------------------
  // Define the Canvas
  TCanvas *c0 = new TCanvas("c0","canvas", 1000, 800);
  c0->Divide(1,2,0,0);

  // First pad:
  c0->cd(1);
  //c0->GetPad(1)->SetMin(1);
  c0->GetPad(1)->SetLogy(1);
  //NEUT_Total_selected_true->SetTitle("");
  //NEUT_Total_selected_true->GetXaxis()->SetTitle("reconstructed costheta [-]");
  NEUT_Total_selected_true->GetYaxis()->SetTitle("#events");
  NEUT_Total_selected_true->GetXaxis()->SetLabelSize(0.1);
  NEUT_Total_selected_true->GetYaxis()->SetLabelSize(0.2);
  //NEUT_Total_selected_true->GetXaxis()->SetTitleSize(0.1);
  NEUT_Total_selected_true->GetYaxis()->SetTitleSize(0.1);
  //NEUT_Total_selected_true->GetXaxis()->SetTitleOffset(1.02);
  NEUT_Total_selected_true->GetYaxis()->SetTitleOffset(0.9);
  //NEUT_Total_selected_true->GetXaxis()->SetLabelSize(0.09);
  //NEUT_Total_selected_true->GetYaxis()->SetLabelSize(0.09);

  //NEUT_Total_selected_true->SetMarkerStyle(kFullCircle);
  NEUT_Total_selected_true->SetMarkerColor(kBlack);
  NEUT_Total_selected_true->SetLineColor(kBlack);
  NEUT_Total_selected_true->SetLineWidth(3);

  NEUT_Fwd_selected_all->SetMarkerStyle(kFullCircle);
  NEUT_Fwd_selected_all->SetMarkerColor(kGreen);
  NEUT_Fwd_selected_all->SetLineColor(kGreen);

  NEUT_Bwd_selected_all->SetMarkerStyle(kFullCircle);
  NEUT_Bwd_selected_all->SetMarkerColor(kBlue);
  NEUT_Bwd_selected_all->SetLineColor(kBlue);

  NEUT_HA_selected_all->SetMarkerStyle(kFullCircle);
  NEUT_HA_selected_all->SetMarkerColor(kRed);
  NEUT_HA_selected_all->SetLineColor(kRed);

  NEUT_Total_selected_all->SetMarkerStyle(kFullCircle);
  NEUT_Total_selected_all->SetMarkerColor(kMagenta+1);
  NEUT_Total_selected_all->SetLineColor(kMagenta+1);

  //NEUT_all->SetMinimum(1);
  NEUT_Total_selected_true ->Draw("hist");
  NEUT_Fwd_selected_all  ->Draw("E1");
  NEUT_Bwd_selected_all  ->Draw("E1 same");
  NEUT_HA_selected_all   ->Draw("E1 same");
  NEUT_Total_selected_all->Draw("E1 same");
  NEUT_Total_selected_true ->Draw("hist same");

  // Plot legend
  leg = new TLegend(0.20, 0.5, 0.40, 0.95);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.09);
  leg->SetFillStyle(0);
  leg->AddEntry(NEUT_Total_selected_true,"selected true CC in FGD1 FV","l"); // "t", "f", "true", "false", "l"
  leg->AddEntry(NEUT_Fwd_selected_all,   "selected Fwd","lep"); // "t", "f", "true", "false", "l"
  leg->AddEntry(NEUT_Bwd_selected_all,   "selected Bwd","lep"); // "t", "f", "true", "false", "l"
  leg->AddEntry(NEUT_HA_selected_all,    "selected HA", "lep"); // "t", "f", "true", "false", "l"
  leg->AddEntry(NEUT_Total_selected_all, "selected Fwd+Bwd+HA","lep"); // "t", "f", "true", "false", "l"
  leg->Draw();


  // Second pad:
  c0->cd(2);
  NEUT_Fwd_pur->SetMinimum(-0.05);
  NEUT_Fwd_pur->SetMaximum(1.05);

  c0->GetPad(2)->SetLogy(0);

  NEUT_Fwd_pur->SetTitle("");
  NEUT_Fwd_pur->GetXaxis()->SetTitle("reconstructed costheta [-]");
  NEUT_Fwd_pur->GetYaxis()->SetTitle("selection purity");
  NEUT_Fwd_pur->GetXaxis()->SetLabelSize(0.1);
  NEUT_Fwd_pur->GetYaxis()->SetLabelSize(0.1);
  NEUT_Fwd_pur->GetXaxis()->SetTitleSize(0.1);
  NEUT_Fwd_pur->GetYaxis()->SetTitleSize(0.1);
  NEUT_Fwd_pur->GetYaxis()->SetTitleOffset(0.84);

  NEUT_Fwd_pur->SetMarkerStyle(kFullSquare);
  NEUT_Fwd_pur->SetMarkerColor(kGreen);
  NEUT_Fwd_pur->SetLineColor(kGreen);

  NEUT_Bwd_pur->SetMarkerStyle(kFullSquare);
  NEUT_Bwd_pur->SetMarkerColor(kBlue);
  NEUT_Bwd_pur->SetLineColor(kBlue);

  NEUT_HA_pur->SetMarkerStyle(kFullCircle);
  NEUT_HA_pur->SetMarkerColor(kRed);
  NEUT_HA_pur->SetLineColor(kRed);

  //NEUT_Total_pur->SetMarkerStyle(kFullCircle);
  NEUT_Total_pur->SetMarkerColor(kMagenta+1);
  NEUT_Total_pur->SetLineColor(kMagenta+1);
  NEUT_Total_pur->SetLineWidth(3);


  NEUT_Fwd_pur  ->Draw("E1");
  NEUT_Bwd_pur  ->Draw("E1 same");
  NEUT_HA_pur   ->Draw("E1 same");
  NEUT_Total_pur->Draw("hist same");


  //c0->Update();
  c0->SaveAs("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/Plots/PNGs/SelectionPurity.png");
}
