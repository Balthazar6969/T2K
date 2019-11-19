

void PlotRelErr(TString fname,TString sname,int nbins,int low, int high)
{

  TFile *fsyst=new TFile(fname.Data());
  TTree *tree=(TTree*) fsyst->Get(sname);
  TH1F *relerr=new TH1F("relerr","",nbins,low,high);

  TH1F *hp_n=new TH1F("hp_n","",nbins,low,high);
  TH1F *hp_ntest=new TH1F("hp_ntest","",nbins,low,high);
  TH1F *hp_s[1000];
  for(int i=0;i<1000;i++)
    hp_s[i]=new TH1F(Form("hp_s%d",i),"",nbins,low,high);
  
  //  tree->Project("hp_n","selmu_momNom","passedCCNom==1");
  //for(int i=0;i<1000;i++){
  //tree->Project(Form("hp_s%d",i),Form("selmu_momToy[%d]",i),Form("(passedCCToy[%d]==1)*weightsToy[%d]",i,i));
  //}
  // TH2F *hp2_s=new TH2F("hp2_s","",1000,0,1000,nbins,low,high);
  // tree->Project("hp2_s","selmu_momToy[]:anaToy[]","(passedCCToy[]==1)*weightsToy");
  

  Float_t selmu_momNom;
  Float_t selmu_momToy[1000];
  Float_t weightsToy[1000];
  Int_t   passedCCToy[1000];
  Int_t   passedCCNom;
  Int_t   anaToy[1000];
  tree->SetBranchAddress("passedCCNom", &passedCCNom);
  tree->SetBranchAddress("selmu_momNom",&selmu_momNom);
  tree->SetBranchAddress("selmu_momToy",selmu_momToy);
  tree->SetBranchAddress("weightsToy",  weightsToy);
  tree->SetBranchAddress("passedCCToy", passedCCToy);
  tree->SetBranchAddress("anaToy",  anaToy);
  int nentries=tree->GetEntries();
  for(int ie=0;ie<nentries;ie++){
    tree->LoadTree(ie);
    tree->GetEntry(ie);
    if(ie%10000==0) std::cout<<" ie "<<ie<<" over "<<nentries<<" => "<<(double)ie/(double)nentries<<std::endl;
    if(passedCCNom){
      hp_n->Fill(selmu_momNom);
    }
    for(int itoy=0;itoy<1000;itoy++){
      //why is there a difference between passedCCToy[itoy] and passedCCNom?
      if(passedCCToy[itoy]){
     	hp_s[itoy]->Fill(selmu_momToy[itoy],weightsToy[itoy]);
      }
    }
  }

  for(int ib=0;ib<nbins;ib++){
    double val=0;
    for(int itoy=0;itoy<1000;itoy++){
      double ni=hp_n->GetBinContent(ib+1);
      double ns=hp_s[itoy]->GetBinContent(ib+1);
      if(ni!=0)
	val+=(ni-ns)*(ni-ns)/(ni*ni*1000.);
    }
    relerr->SetBinContent(ib+1,sqrt(val));
  }
  TCanvas *c1=new TCanvas("c1","",1);
  relerr->Draw();
  c1->Print(Form("%s.eps",sname.Data()));
}
