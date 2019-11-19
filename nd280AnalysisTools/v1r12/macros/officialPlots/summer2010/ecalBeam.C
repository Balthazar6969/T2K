void ecalBeam() {}


/*



Root macros to make the plots

In general, you only have to run ecalBeam("ecalBeam.root")
where ecalBeam.root is the output of the runAnalyses.py stage

Author: Pawel Guzowski <p.guzowski07@imperial.ac.uk>


*/



void ecalBeam(const char* fname) {

  TFile *f = new TFile(fname);
  TTree *tall = (TTree*)f->Get("event_tree");
  TTree *tclus = (TTree*)f->Get("cluster_tree");

  DrawDailyRates(tall);
  PlotSpill(tclus, tall);

  /*

  Plot cumulative clusters vs cumulative POT

  */
  
  TCanvas *c6 = new TCanvas("ccum","ccum",0,0,800,600);
  tall->Draw("cumclus:cumpotdaqd", "gs == 1");
  TGraph *gr3 = new TGraph(*(TGraph*)gPad->GetPrimitive("Graph"));
  gr3->SetLineWidth(2);
  gr3->SetMarkerStyle(6);
  TGaxis::SetMaxDigits(2);
  gr3->GetXaxis()->SetTitle("Cumulative CT5 proton count");
  gr3->GetYaxis()->SetTitle("Cumulative DsECal clusters");
  gr3->GetYaxis()->SetNoExponent();
  gr3->SetTitle("Beam runs 31 & 32");
  gr3->Draw("ap");
  


}







////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*

Makes the plots of #dsecal clusters and POT vs day

*/


void DrawDailyRates(TTree *t) {


  Int_t oldopt = gStyle->GetOptStat();
  Int_t oldfit = gStyle->GetOptFit();
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1);
  gROOT->ForceStyle();

  if (!t) {
    TFile *f = new TFile("ecalBeam.root");
    if(!f || f->IsZombie()) return;
    TTree *t = (TTree*) f->Get("event_tree");
  }

  Int_t N = t->GetEntries();

  Int_t nclus[7], time, good_spill, run;
  Float_t pot[7];

  const Int_t unixday = 24*60*60;

  t->SetBranchAddress("instpot", pot);
  t->SetBranchAddress("time", &time);
  t->SetBranchAddress("gs", &good_spill);
  t->SetBranchAddress("instclus", nclus);
  t->SetBranchAddress("run", &run);

  Int_t mintime = 0, maxtime;

  if(N > 0) {
    t->GetEntry(0);
    mintime = time;
  }

  for(Int_t i = 0; i < N; ++i) {
    t->GetEntry(i);
    if(time > maxtime) maxtime = time;
    if(time < mintime) mintime = time;
  }

  TDatime mindate(mintime);
  TDatime maxdate(maxtime + unixday);
  TDatime date;

  mindate.Set(mindate.GetYear(), mindate.GetMonth(), mindate.GetDay(), 0, 0, 0);
  maxdate.Set(maxdate.GetYear(), maxdate.GetMonth(), maxdate.GetDay(), 0, 0, 0);

  mintime = mindate.Convert();
  maxtime = maxdate.Convert();
  const Int_t ndays = (maxtime - mintime) / unixday;
  const char* datelabel = "#splitline{%02d}{%02d}";

  TH1F* hpot = new TH1F("hpot", "Title;axis", ndays, mintime, maxtime);
  TH1F* hclus = new TH1F("hclus", "Title;axis", ndays, mintime, maxtime);
  TH1F* hrun = new TH1F("hrun", "Title;axis", 600, 4000, 4600);

  for(Int_t i = 0; i < N; ++i) {
    t->GetEntry(i);
    if(good_spill == 0) continue;
    hpot->Fill(time, pot[0]);
    hclus->Fill(time, nclus[0]);
    hrun->Fill(run, nclus[0]);
  }


  Float_t scale = hpot->Integral() / hclus->Integral();
  Float_t max_pot_axis = 800e15;
  Float_t max_clus_axis = max_pot_axis / scale;


  Int_t nfulldays = 0;
  for(Int_t i = 1; i <= ndays; ++i) {
    Float_t potx = hpot->GetBinContent(i);
    if(potx > 1.) nfulldays++;
  }


  TH1F* hpot_full = new TH1F("hpot_full", "Daily DsECal cluster rates;Date;protons @ CT5 / day", nfulldays, 0, nfulldays);
  hpot_full->SetFillColor(kGray+1);
  hpot_full->SetLineColor(kGray+1);
  hpot_full->GetXaxis()->SetTitleOffset(1.2);
  hpot_full->GetXaxis()->CenterTitle();

  TH1F* hclus_full = new TH1F("hclus_full", "Title;Date;DsECal clusters", nfulldays, 0, nfulldays);

  TH1F* hclus_full_scaled = new TH1F("hclus_full_scaled", "DsECal clusters", nfulldays, 0, nfulldays);
  hclus_full_scaled->SetLineWidth(2);

  TH1F* hratio_full = new TH1F("hratio_full", "Normalised DsECal cluster rate; Date; DsECal clusters / 10^{15} protons @ CT5", nfulldays, 0, nfulldays);
  hratio_full->SetLineWidth(2);
  hratio_full->GetXaxis()->SetTitleOffset(1.2);
  hratio_full->GetXaxis()->CenterTitle();

  Int_t nskip = 0;
  Int_t firstskip = 0;
  Float_t fclus, error;
  for(Int_t i = 1; i <= ndays; ++i) {
    Float_t potx = hpot->GetBinContent(i);
    if(potx < 1.) {
      if(firstskip == 0) firstskip = i-1;
      nskip++;
      continue;
    }
    fclus = hclus->GetBinContent(i);
    error = TMath::Sqrt(fclus);
    hpot_full->SetBinContent(i - nskip, potx);
    hclus_full->SetBinContent(i - nskip, fclus);
    hratio_full->SetBinContent(i - nskip, fclus / (potx / 1e15));
    hratio_full->SetBinError(i - nskip, error / (potx / 1e15));
    hclus_full_scaled->SetBinContent(i - nskip, fclus * scale);
    hclus_full_scaled->SetBinError(i - nskip, error * scale);
    date.Set(hpot->GetXaxis()->GetBinCenter(i));
    hpot_full->GetXaxis()->SetBinLabel(i - nskip, Form(datelabel, date.GetMonth(), date.GetDay()));
    hclus_full->GetXaxis()->SetBinLabel(i - nskip, Form(datelabel, date.GetMonth(), date.GetDay()));
    hratio_full->GetXaxis()->SetBinLabel(i - nskip, Form(datelabel, date.GetMonth(), date.GetDay()));
  }


  TCanvas *c5 = new TCanvas("c5","c5",0,0,800,600);
  hpot_full->Draw("ah");
  hpot_full->GetYaxis()->SetRangeUser(0., max_pot_axis);
  hclus_full_scaled->Draw("same e0");
  hpot_full->Draw("axis same");

  TLegend *leg = c5->BuildLegend(0.1350575,0.7224576,0.3764368,0.8813559);
  leg->SetFillStyle(0);
  leg->SetLineColor(kWhite);
  TLegendEntry* en = (TLegendEntry*)leg->GetListOfPrimitives()->Last();
  leg->GetListOfPrimitives()->Remove(en);
  TIter next(leg->GetListOfPrimitives());
  while((en = (TLegendEntry*) next())) {
    if(en->GetObject() == hpot_full) {
      en->SetLabel("Protons @ CT5");
      en->SetOption("f");
    }
  }
  
  TGaxis* axis = new TGaxis(nfulldays, 0, nfulldays, max_pot_axis, 0, max_clus_axis, 210,"+L");
  axis->SetTitle("DsECal clusters / day");
  axis->SetTitleOffset(1.2);
  axis->Draw("same");

  new TCanvas("c6","c6",0,0,800,600);
  hratio_full->Draw("e0");
  hratio_full->GetYaxis()->SetRangeUser(0, 2.5);
  hratio_full->Fit("pol0");
  TF1* pol0 = hratio_full->GetFunction("pol0");
  if(pol0) pol0->SetLineStyle(3);

  if(firstskip > 0) {
    TLine *l1 = new TLine(firstskip, 0, firstskip, 2.5);
    l1->Draw();
    TPaveText *text = new TPaveText(0.9, 0.8, firstskip-0.9, 1.);
    text->InsertText("Run 31");
    text->SetLineColor(kWhite);
    text->SetFillStyle(0);
    text->SetShadowColor(kWhite);
    text->Draw();
    text = new TPaveText(firstskip + 0.9, 0.8, nfulldays-0.9, 1.);
    text->InsertText("Run 32");
    text->SetLineColor(kWhite);
    text->SetFillStyle(0);
    text->SetShadowColor(kWhite);
    text->Draw();
  }
  
  // List the rates for each run
  
  cout << "Ratio: " << 1./(scale/1e15) << " clusters per 10^15 protons" << endl;

  int totnc = 0;
  for(int i = 1; i<= hrun->GetNbinsX(); ++i) {
    int nc = hrun->GetBinContent(i);
    int r = hrun->GetBinCenter(i);
    if(nc > 0) {
      cout << "run " << r << "\t ev: " << nc << endl;
      totnc += nc;
    }
  }
  cout << "totnc: " << totnc << endl;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/*

Makes plot of cluster times

Also makes plot of clusters/POT vs spill number, but this doesn't work with the beam summary text files provided by ND280-beam group.

*/

void PlotSpill(TTree* tclus, TTree* tall) {
  TFile *f = 0;
  if (!tall) {
    f = new TFile("ecalBeam.root");
    tall = (TTree*) f->Get("event_tree");
  }
  if(!tclus) {
    if(!f) f = new TFile("ecalBeam.root");
    tclus = (TTree*) f->Get("cluster_tree");
  }

  TCanvas *c7 = new TCanvas("c7","c7",0,0,800,600);

  int nev = tclus->Draw("time/1000.>>h(1400,0.000,14.000)", "bunch > 0");
  if(nev == 0) return;
  tclus->Draw("time/1000.>>h2(1400,0.000,14.000)", "bunch == 0");
  
  h->SetTitle("DsECal cluster times#; Runs 31 & 32");
  h->GetXaxis()->SetTitle("Time relative to trigger (#mus)");
  h->SetLineColor(kBlue);
  h->SetFillColor(kBlue);
  //h->GetXaxis()->SetTickLength(0.);
  h->GetXaxis()->SetNdivisions(14);
  h->Draw("ah");

  h2->SetTitle("Out of beam time");
  h2->SetLineColor(kRed);
  h2->SetFillColor(kRed);
  h2->Draw("same");

  h->Draw("axis same");
  gPad->SetLogy();
  gPad->SetGridx();


  TLegend *leg = gPad->BuildLegend();
  leg->SetFillStyle(0);
  leg->SetLineColor(kWhite);
  TLegendEntry* en = (TLegendEntry*)leg->GetListOfPrimitives()->Last();
  leg->GetListOfPrimitives()->Remove(en);
  TIter next(leg->GetListOfPrimitives());
  while((en = (TLegendEntry*) next())) {
    if(en->GetObject() == h) en->SetLabel("In beam time");
    en->SetOption("f");
  }

  //TCanvas *c8 = new TCanvas("c8","c8",0,0,800,600);

  TH1F *h8clus = new TH1F("h8clus", "clus", 6,1,7);
  TH1F *h8clus_scaled = new TH1F("h8clus_scaled", "DsECal clusters", 6,1,7);
  h8clus_scaled->SetLineWidth(2);
  TH1F *h8pot = new TH1F("h8pot", "Integrated rates per bunch in runs 31 & 32;Bunch;Protons @ CT5 / bunch", 6,1,7);
  h8pot->SetFillColor(kGray+1);
  h8pot->SetLineColor(kGray+1);
  h8pot->GetYaxis()->SetTitleOffset(1.2);
  h8pot->GetXaxis()->CenterTitle();
  h8pot->GetXaxis()->CenterLabels();
  h8pot->GetXaxis()->SetNdivisions(10);

  Int_t nclus[7], good_spill;
  Float_t pot[7];

  tall->SetBranchAddress("instpot", pot);
  tall->SetBranchAddress("gs", &good_spill);
  tall->SetBranchAddress("instclus", nclus);


  Int_t N = tall->GetEntries();

  for(Int_t i = 0; i < N; ++i) {
    tall->GetEntry(i);
    if(good_spill == 0) continue;
    for(int b = 1; b < 7; ++b) {
      h8pot->Fill(b, pot[b]);
      h8clus->Fill(b, nclus[b]);
    }
  }


  Float_t scale = h8pot->Integral() / h8clus->Integral();
  Float_t max_pot_axis = 2000e15;
  Float_t max_clus_axis = max_pot_axis / scale;

  Float_t fclus, error;
  for(Int_t i = 1; i <= 6; ++i) {
    Float_t potx = h8pot->GetBinContent(i);
    fclus = h8clus->GetBinContent(i);
    error = TMath::Sqrt(fclus);
    h8clus_scaled->SetBinContent(i, fclus * scale);
    h8clus_scaled->SetBinError(i, error * scale);
  }


  //h8pot->Draw();
  //  TCanvas *c9 = new TCanvas("c9","c9",0,0,800,600);
  //h8clus->Draw();
  TCanvas *c10 = new TCanvas("c10","c10",0,0,800,600);
  h8pot->Draw("ah");
  h8pot->GetYaxis()->SetRangeUser(0., max_pot_axis);
  h8clus_scaled->Draw("same e0");
  h8pot->Draw("axis same");

  TLegend *leg = c10->BuildLegend(0.1344221,0.8164336,0.8592965,0.8828671);
  leg->SetNColumns(2);
  leg->SetFillStyle(0);
  leg->SetLineColor(kWhite);
  TLegendEntry* en = (TLegendEntry*)leg->GetListOfPrimitives()->Last();
  leg->GetListOfPrimitives()->Remove(en);
  TIter next(leg->GetListOfPrimitives());
  while((en = (TLegendEntry*) next())) {
    if(en->GetObject() == h8pot) {
      en->SetLabel("Protons @ CT5");
      en->SetOption("f");
    }
  }
  
  TGaxis* axis = new TGaxis(7, 0, 7, max_pot_axis, 0, max_clus_axis, 210,"+L");
  axis->SetTitle("DsECal clusters / bunch");
  axis->SetTitleOffset(1.3);
  axis->Draw("same");
}

