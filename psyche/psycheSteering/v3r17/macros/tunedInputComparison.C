TString _psyname="../amd64_linux26/BANFF_SelectedEvent_Histograms.root";
TString _h2name="../amd64_linux26/work/outfiles_new/CCnPIh2_corr_syst_fth2_100_6Brun2w_6Btables.root";
void tunedInputComparison(TString psyname=_psyname,TString h2name=_h2name,bool numu=1){
  TFile* inputfile=new TFile(psyname,"","READ");
  if(!inputfile->Get("tree"))return;
  TTree *tree=(TTree*)inputfile->Get("tree");
  DrawingTools *draw=new DrawingTools(h2name.Data(),false);
  DataSample   *mc  =new DataSample(h2name.Data()); 
  TTree *deftree=(TTree*)mc->GetTree("all_syst");

  TH1F *hmom_nom[4];
  TH1F *hmom_tun[4];
#if numu==1
  TString scuth2[4]={"accum_level[][0]>5","accum_level[][0]>6","accum_level[][1]>6","accum_level[][2]>6"};
  TString type[]={"CC","CC0Pi","CC1Pi","CCOther"};
  TString sel[]={"isFGD1NuMuCC","isFGD1NuMuCC0Pi","isFGD1NuMuCC1Pi","isFGD1NuMuCCOther"};
  int ntypes=4;
#else
  TString type[]={"CC","CCQE","CCnQE"};
  TString sel[]={"isFGD1AntiNuMuCC","isFGD1AntiNuMuCCQE","isFGD1AntiNuMuCCnQE"};
  int ntypes=3;
#endif
  
 
  for(int ih=0;ih<ntypes;ih++){
    TCanvas c1(Form("canvas_%s",type[ih].Data()),Form("canvas_%s",type[ih].Data()),1);
    c1.cd();
    inputfile->cd();
    hmom_tun[ih]=new TH1F(Form("hmom_tun%s",type[ih].Data()),"",50,0,5000);
    std::cout<< tree->Project(Form("hmom_tun%s",type[ih].Data()),"selmu_mom",Form("(%s)*potW*pileupW*fluxW*detW*niwgW",sel[ih].Data()))<<std::endl;
    hmom_nom[ih]=new TH1F(Form("hmom_nom%s",type[ih].Data()),"",50,0,5000);
    draw->SetTitleY("Entries");
    draw->SetTitleX("Reconstructed momentum (MeV/c)");
    draw->Draw(deftree,"selmu_mom",50,0,5000,"reaction",scuth2[ih].Data());

    hmom_nom[ih]=(TH1F*) draw->GetLastStackTotal();
    tree->Project(Form("hmom_nom%s",type[ih].Data()),"selmu_mom",Form("(%s)*potW*pileupW*fluxW*detW",sel[ih].Data()));
    
    hmom_nom[ih]->SetLineColor(1);
    hmom_nom[ih]->SetLineWidth(2);
     
    hmom_tun[ih]->SetLineColor(2);
    hmom_tun[ih]->SetLineWidth(2);

   
    TLegend *leg = new TLegend(0.6,0.39,0.89,0.49);
    leg->SetFillColor(0);
    leg->SetLineColor(0);
    leg->AddEntry(hmom_nom[ih],Form("Nominal - %s",type[ih].Data(),"f"));
    leg->AddEntry(hmom_tun[ih],Form("Tuned   - %s",type[ih].Data(),"f"));

    hmom_tun[ih]->Draw("same");
    hmom_nom[ih]->Draw("same");
    leg->Draw("same");
    c1.Update();
    c1.Print(Form("tuned%s.C",type[ih].Data()));
  }
  
}
