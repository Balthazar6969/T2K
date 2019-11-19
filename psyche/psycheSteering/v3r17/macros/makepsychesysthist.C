void convert_fname_systname(TString fsystname,TString &systname2,TString &opt){

  if(fsystname=="bfield")             {opt="SSYS"; systname2="bfield_syst";} 
  else if(fsystname=="momresol")      {opt="SSYS"; systname2="momresol_syst";}
  else if(fsystname=="momscale")      {opt="SSYS"; systname2="momscale_syst";}
  else if(fsystname=="tpcpid")        {opt="SSYS"; systname2="tpcpid_syst";}
  else if(fsystname=="fgdpid")        {opt="SSYS"; systname2="fgdpid_syst";}
  else if(fsystname=="chargeideff")   {opt="WSYS"; systname2="chargeideff_syst";}
  else if(fsystname=="tpcclustereff") {opt="WSYS"; systname2="tpcclustereff_syst";}
  else if(fsystname=="tpctrackeff")   {opt="WSYS"; systname2="tpctrackeff_syst";}
  else if(fsystname=="fgdtrackeff")   {opt="WSYS"; systname2="fgdtrackeff_syst";}
  else if(fsystname=="fgdhybtrackeff"){opt="WSYS"; systname2="fgdhybridtrackeff_syst";}
  else if(fsystname=="tpcfgdmatcheff"){opt="WSYS"; systname2="tpcfgdmatcheff_syst";}
  else if(fsystname=="meeff")         {opt="WSYS"; systname2="michel_syst";}
  else if(fsystname=="micheleleeff")  {opt="WSYS"; systname2="michel_syst";}
  else if(fsystname=="pileup")        {opt="WSYS"; systname2="pileup_syst";}
  else if(fsystname=="fgdmass")       {opt="WSYS"; systname2="fgdmass_syst";}
  else if(fsystname=="oofv")          {opt="WSYS"; systname2="oofv_syst";}
  else if(fsystname=="sipion")        {opt="WSYS"; systname2="sipion_syst";}
  else if(fsystname=="sandmu")        {opt="WSYS"; systname2="sandmu_syst";}
  else if(fsystname=="all")           {opt="SYS"; systname2="all_syst";}
  
  std::cout<<" name "<<systname2<<std::endl;
  
  return;
}



void  DrawRelativeErrors(TH2F *hcov,TH1F* hh,TTree *tree,TString var,int nbins,TH1F &hn_nom, TH2F &hn_a, TH2F &hn_va,TString cut_nom,TString cut_toy,TString opt){
  
  TStopwatch *watch=new TStopwatch();
  watch->Start();
  
  TH1F h0("v","v",1,0,1);
  watch->Stop();std::cout<<" 0time "<<watch->RealTime()<<std::endl;
  watch->Reset(); watch->Start();
  tree->Project("v","0.5","nToys","",1);
  watch->Stop();std::cout<<" 1time "<<watch->RealTime()<<std::endl;
  watch->Reset(); watch->Start();
  int ntoys=h0.GetBinContent(1);
  tree->Project("hn_nom",Form("%sNom",var.Data()),Form("(%s)*pileupweight*fluxweight",cut_nom.Data()));
  tree->Project("hn_a",Form("%sToy:anaToy",var.Data()),Form("(%s && weightsToy<100)*pileupweight*fluxweight*weightsToy",cut_toy.Data()));
  // tree->Project("hn_nom",Form("%sNom",var.Data()),Form("(%s)*pileupweight",cut_nom.Data()));
  // tree->Project("hn_a",Form("%sToy:anaToy",var.Data()),Form("(%s && weightsToy<100)*pileupweight*weightsToy",cut_toy.Data()));
  // tree->Project("hn_nom",Form("%sNom",var.Data()),Form("(%s)*fluxweight",cut_nom.Data()));
  // tree->Project("hn_a",Form("%sToy:anaToy",var.Data()),Form("(%s && weightsToy<100)*fluxweight*weightsToy",cut_toy.Data()));

  
  watch->Stop();std::cout<<" 2time "<<watch->RealTime()<<std::endl;
  watch->Reset(); watch->Start();
  
  tree->Project("hn_va",Form("%sToy:anaToy",var.Data()),Form("(%s)*pileupweight*fluxweight",cut_toy.Data()));
  //  tree->Project("hn_va",Form("%sToy:anaToy",var.Data()),Form("(%s)*pileupweight",cut_toy.Data()));
  //  tree->Project("hn_va",Form("%sToy:anaToy",var.Data()),Form("(%s)*fluxweight",cut_toy.Data()));
  watch->Stop();std::cout<<" 3time "<<watch->RealTime()<<std::endl;
  watch->Reset(); watch->Start();

  
  float NA_av=0;
  float na_av[1000]={0};
  for(int i=0;i<nbins;i++){
    na_av[i]=0;
    for(int ia=0;ia<ntoys;ia++){
      float nai=hn_va.GetBinContent(ia+1,i+1);
      na_av[i]+=nai;
      NA_av+=nai;
    }
    na_av[i]=na_av[i]/(float)ntoys;
  }

  watch->Stop();std::cout<<"4 time "<<watch->RealTime()<<std::endl;
  watch->Reset(); watch->Start();

  float cov[1000][1000]={{0}};
  for(int i=0;i<nbins;i++){
    for(int j=0;j<nbins;j++){
      cov[i][j]=0;
      float nnomi=hn_nom.GetBinContent(i+1);
      for(int ia=0;ia<ntoys;ia++){
	float nai,naj;
	nai=hn_a.GetBinContent(ia+1,i+1);
	naj=hn_a.GetBinContent(ia+1,j+1);
	//not necessary
	// if(opt=="SSYS"){
	//   nai=hn_va.GetBinContent(ia+1,i+1);
	//   naj=hn_va.GetBinContent(ia+1,j+1);
	// }
	if(na_av[i]!=0 && na_av[j]!=0)
	  cov[i][j]+=(na_av[i]-nai)*(na_av[j]-naj)/(na_av[i]*na_av[j]*ntoys);
	
	//	if(i==j) std::cout<<i<<" rms "<<sqrt(cov[i][j])<<" it "<<ia<<" ntoys "<<ntoys<<" nai "<<nai<<" nav "<<na_av[i]<<" nnom "<<nnomi<<std::endl;

      if(nai!=nai || naj!=naj || na_av[i]!=na_av[i] || na_av[j]!=na_av[j])
	std::cout<<" NAN !!!! ni"<<nai<<" or av:"<<na_av[i]<<std::endl;
      
      }
      hcov->SetBinContent(i+1,j+1,cov[i][j]);
    }
    hh->SetBinContent(i+1,sqrt(cov[i][i]));
  }
  watch->Stop();std::cout<<" 5time "<<watch->RealTime()<<std::endl;
  watch->Reset(); watch->Start();
  
  NA_av=NA_av/(float)ntoys;
  float RMS=0;
  for(int ia=0;ia<ntoys;ia++){
    float NAi=0;
    for(int i=0;i<nbins;i++){
      NAi+=hn_a.GetBinContent(ia+1,i+1);
    }
    if(NA_av!=0)
      RMS+=(NA_av-NAi)*(NA_av-NAi)/(NA_av*NA_av*ntoys);
  }
  std::cout<<tree->GetName()<<" total error : "<<sqrt(RMS)<<std::endl;

  
}

void  DrawRelativeErrors(TH2F *hcov,TH1F* hh,TTree *tree,TString var,int nbins,float low,float high,TString cut_nom,TString cut_toy,TString opt){
   TH1F hn_nom("hn_nom","",nbins,low,high);
   TH2F hn_a("hn_a","",1000,0,1000,nbins,low,high);
   TH2F hn_va("hn_va","",1000,0,1000,nbins,low,high);
   DrawRelativeErrors(hcov,hh,tree,var,nbins,hn_nom,hn_a,hn_va,cut_nom,cut_toy,opt);
  
}
void  DrawRelativeErrors(TH2F *hcov,TH1F* hh,TTree *tree,TString var,int nbins,double* bins,TString cut_nom,TString cut_toy,TString opt){
  TH1F hn_nom("hn_nom","",nbins,bins);
  TH2F hn_a("hn_a","",1000,0,1000,nbins,bins);
  TH2F hn_va("hn_va","",1000,0,1000,nbins,bins);
  DrawRelativeErrors(hcov,hh,tree,var,nbins,hn_nom,hn_a,hn_va,cut_nom,cut_toy,opt);
}




void makepsychesysthist(TString stFGD,TString infile,TString outfile,TString update="UPDATE",bool regular=false,int _nbins=50)
{
  //TString stFGD="FGD2";
  int low=0;
  int high=5000;
  int nbins[4]={14,14,13,14};
  double bins[4][15]={0,300,400,500,600,700,800,900,1000,1250,1500,2000,3000,5000,30000,
		     0,300,400,500,600,700,800,900,1000,1250,1500,2000,3000,5000,30000,
		     0,300,400,500,600,700,800,900,1000,1250,1500,2000,3000,30000,30000,
		     0,300,400,500,600,700,800,900,1000,1250,1500,2000,3000,5000,30000};


  TFile *inpf=new TFile( infile.Data());
  TFile *outf =new TFile(outfile.Data(),update);

  TString systname[18]={"bfield","momresol","momscale","tpcpid","fgdpid","chargeideff","tpcclustereff","tpctrackeff","fgdtrackeff","fgdhybtrackeff","tpcfgdmatcheff","meeff","pileup","fgdmass","oofv","sipion","sandmu","all"};
  TString path="numuCC"+stFGD;
  outf->mkdir(path.Data(),path.Data());
  TString treename[18];
  TH1F           *hh[4];
  TH2F           *hcov[4];
  TString         nn[4]={"CC","CC0pi","CC1pi","CCOth"};  
  TString scutpsyToy[4]={"is"+stFGD+"CCToy","is"+stFGD+"CC0PiToy","is"+stFGD+"CC1PiToy","is"+stFGD+"CCOtherToy"};
  TString scutpsyNom[4]={"is"+stFGD+"CCNom","is"+stFGD+"CC0PiNom","is"+stFGD+"CC1PiNom","is"+stFGD+"CCOtherNom"};

  for(int isyst=0;isyst<18;isyst++){   
    TString systname2,systtype2,opt; 
     convert_fname_systname(systname[isyst],systname2,opt);
    treename[isyst]=systname2;
    if( !inpf->Get(treename[isyst].Data()) ){
      std::cout<<" tree "<<treename[isyst]<<" does not exist! "<<std::endl;
      continue;
    }
    //if( treename[isyst]!="tpcpid_syst" )continue;

    TString path2=path+"/"+systname2;
    outf->mkdir(path2.Data(),path2.Data());
    TTree *tree=(TTree*) inpf->Get(treename[isyst].Data());
    std::cout<<" taking info from "<<treename[isyst]<<std::endl;
    for(int ih=0;ih<4;ih++){
      std::cout<<" ih "<<ih<<" nn "<<nn[ih]<<std::endl;
      if(regular){
	hh[ih]=new TH1F(Form("hh%s%s",nn[ih].Data(),systname[isyst].Data()),"",_nbins,low,high);
	hcov[ih]=new TH2F(Form("hcov%s%s",nn[ih].Data(),systname[isyst].Data()),"",_nbins,low,high,_nbins,low,high);
      }else{
	hh[ih]=new TH1F(Form("hh%s%s",nn[ih].Data(),systname[isyst].Data()),"",nbins[ih],bins[ih]);
	hcov[ih]=new TH2F(Form("hcov%s%s",nn[ih].Data(),systname[isyst].Data()),"",nbins[ih],bins[ih],nbins[ih],bins[ih]);
      }
      
      if(regular)
	DrawRelativeErrors(hcov[ih],hh[ih],tree,"selmu_mom",_nbins,low,high,scutpsyNom[ih],scutpsyToy[ih],opt);
      else
	DrawRelativeErrors(hcov[ih],hh[ih],tree,"selmu_mom",nbins[ih],bins[ih],scutpsyNom[ih],scutpsyToy[ih],opt);
  
      hh[ih]->SetTitle(Form("%s for %s",systname[isyst].Data(),nn[ih].Data()));
      hh[ih]->SetName(Form("relerr_psy_%s_%s",systname[isyst].Data(),nn[ih].Data()));
      hh[ih]->GetYaxis()->SetTitle("Relative error");
      hh[ih]->GetXaxis()->SetTitle("Reconstructed momentum (MeV/c)");

      hcov[ih]->SetTitle(Form("%s for %s",systname[isyst].Data(),nn[ih].Data()));
      hcov[ih]->SetName(Form("relcov_psy_%s_%s",systname[isyst].Data(),nn[ih].Data()));
      hcov[ih]->GetYaxis()->SetTitle("Reconstructed momentum (MeV/c)");
      hcov[ih]->GetXaxis()->SetTitle("Reconstructed momentum (MeV/c)");
      outf->cd();     
      TString path3=path2+"/"+nn[ih];
      outf->mkdir(path3.Data(),path3.Data());
      outf->cd(path3.Data());
      hh[ih]->Write();
      hcov[ih]->Write();
    }
  } 
  outf->Close();
}
