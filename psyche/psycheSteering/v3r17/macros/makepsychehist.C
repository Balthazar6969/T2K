

void makepsychehist(TString stFGD,TString infile,TString outfile,TString update="UPDATE",int _nbins=50)
{
  int nbins=_nbins;
  int low=0;
  int high=5000;
  TFile *inpf=new TFile( infile.Data());
  TFile *outf =new TFile(outfile.Data(),update);

  TString path="numuCC"+stFGD;
  outf->mkdir(path.Data(),path.Data());
  TH1F           *hh[4];
  TString         nn[4]={"CC","CC0pi","CC1pi","CCOth"};  
  TString scutpsyToy[4]={"is"+stFGD+"CCToy","is"+stFGD+"CC0PiToy","is"+stFGD+"CC1PiToy","is"+stFGD+"CCOtherToy"};
  TString scutpsyNom[4]={"is"+stFGD+"CCNom","is"+stFGD+"CC0PiNom","is"+stFGD+"CC1PiNom","is"+stFGD+"CCOtherNom"};

  TString treename="bfield_syst";
  if( !inpf->Get(treename.Data()) ){
    std::cout<<" tree "<<treename<<" does not exist! "<<std::endl;
    continue;
  }
  TTree *tree=(TTree*) inpf->Get(treename.Data());
  std::cout<<" taking info from "<<treename<<std::endl;
  for(int ih=0;ih<4;ih++){
    std::cout<<" ih "<<ih<<" nn "<<nn[ih]<<std::endl;
    hh[ih]=new TH1F(Form("hh%s",nn[ih].Data()),"",nbins,low,high);
    tree->Project(hh[ih]->GetName(),"selmu_momNom",scutpsyNom[ih]);
    // tree->Scan("selmu_momNom",scutpsyNom[ih]);
    hh[ih]->SetTitle(Form("%s",nn[ih].Data()));
    hh[ih]->SetName(Form("mom_psy_%s",nn[ih].Data()));
    hh[ih]->GetYaxis()->SetTitle("entries");
    hh[ih]->GetXaxis()->SetTitle("Reconstructed momentum (MeV/c)");
    outf->cd();     
    TString nnn=nn[ih];
    if(_nbins==20){
      nnn+="_reg20";
    }
    TString path3=path+"/"+nnn;

    outf->mkdir(path3.Data(),path3.Data());
    outf->cd(path3.Data());
    hh[ih]->Write();
  
  } 
  outf->Close();
}
