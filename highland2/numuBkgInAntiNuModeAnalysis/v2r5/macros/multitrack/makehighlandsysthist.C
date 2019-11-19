void convert_fname_systname(TString fsystname,TString &systname2,TString &opt){


  if(fsystname=="bfield")             {opt="SSYS"; systname2="bfield_syst";} 
  else if(fsystname=="momresol")      {opt="SSYS"; systname2="momresol_syst";}
  else if(fsystname=="momres")        {opt="SSYS"; systname2="momresol_syst";}
  else if(fsystname=="momscale")      {opt="SSYS"; systname2="momscale_syst";}
  else if(fsystname=="tpcpid")        {opt="SSYS"; systname2="tpcpid_syst";}
  else if(fsystname=="chargeideff")   {opt="WSYS"; systname2="chargeideff_syst";}
  else if(fsystname=="tpcclustereff") {opt="WSYS"; systname2="tpcclustereff_syst";}
  else if(fsystname=="tpctrackeff")   {opt="WSYS"; systname2="tpctrackeff_syst";}
  else if(fsystname=="tpcfgdmatcheff"){opt="WSYS"; systname2="tpcfgdmatcheff_syst";}
  else if(fsystname=="pileup")        {opt="WSYS"; systname2="pileup_syst";}
  else if(fsystname=="fgdmass")       {opt="WSYS"; systname2="fgdmass_syst";}
  else if(fsystname=="oofv")          {opt="WSYS"; systname2="oofv_syst";}
  else if(fsystname=="sipion")        {opt="WSYS"; systname2="sipion_syst";}
  else if(fsystname=="sandmu")        {opt="WSYS"; systname2="sandmu_syst";}
  else if(fsystname=="all")           {opt="SYS"; systname2 ="all_syst";}
  
  std::cout<<" name "<<systname2<<std::endl;
  
  return;
}

void  DrawRelativeErrors(TH2F *hcov,TH1F* hh,TTree *deftree,TTree *tree,TString var,int nbins,TH1F &hn_nom, TH2F &hn_a, TH2F &hn_va,TString cut_nom,TString opt){

  TH1F h0("v","v",1,0,1);
  tree->Project("v","0.5","NTOYS","",1);
  int ntoys=h0.GetBinContent(1);
  deftree->Project("hn_nom",Form("%s",var.Data()),Form("(%s)*weight[0]*weight[1]",cut_nom.Data()));
  tree->Project("hn_a",Form("%s:toy_index",var.Data()),Form("(%s && weight_syst_total<100)*weight[0]*weight[1]*weight_syst_total",cut_nom.Data()));
  tree->Project("hn_va",Form("%s:toy_index",var.Data()),Form("(%s)*weight[0]*weight[1]",cut_nom.Data()));
  
  float NA_av=0;
  float na_av[100]={0};
  for(int i=0;i<nbins;i++){
    na_av[i]=0;
    for(int ia=0;ia<ntoys;ia++){
      float nai=hn_va.GetBinContent(ia+1,i+1);
      na_av[i]+=nai;
      NA_av+=nai;
    }
    na_av[i]=na_av[i]/(float)ntoys;
  }
  float cov[100][100]={{0}};
  for(int i=0;i<nbins;i++){
    for(int j=0;j<nbins;j++){
      cov[i][j]=0;
      float nnomi=hn_nom.GetBinContent(i+1);
      for(int ia=0;ia<ntoys;ia++){
	float nai,naj;
	nai=hn_a.GetBinContent(ia+1,i+1);
	naj=hn_a.GetBinContent(ia+1,j+1);
	// not needed
	// if(opt=="SSYS"){
	//   nai=hn_va.GetBinContent(ia+1,i+1);
	//   naj=hn_va.GetBinContent(ia+1,j+1);
	// }
	if(na_av[i]!=0 && na_av[j]!=0)
	  cov[i][j]+=(na_av[i]-nai)*(na_av[j]-naj)/(na_av[i]*na_av[j]*ntoys);
	if(i==j) std::cout<<i<<" rms "<<sqrt(cov[i][j])<<" it "<<ia<<" ntoys "<<ntoys<<" nai "<<nai<<" nav "<<na_av[i]<<" nnom "<<nnomi<<std::endl;
	 
	if(nai!=nai || naj!=naj || na_av[i]!=na_av[i] || na_av[j]!=na_av[j])
	  std::cout<<" NAN !!!! ni"<<nai<<" or av:"<<na_av[i]<<std::endl;
      
      }
      hcov->SetBinContent(i+1,j+1,cov[i][j]);
    }
    hh->SetBinContent(i+1,sqrt(cov[i][i]));
  }

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

void  DrawRelativeErrors(TH2F* hcov,TH1F* hh,TTree *deftree,TTree *tree,TString var,int nbins,float low,float high,TString cut_nom,TString opt){
   TH1F hn_nom("hn_nom","",nbins,low,high);
   TH2F hn_a("hn_a","",1000,0,1000,nbins,low,high);
   TH2F hn_va("hn_va","",1000,0,1000,nbins,low,high);
   DrawRelativeErrors(hcov,hh,deftree,tree,var,nbins,hn_nom,hn_a,hn_va,cut_nom,opt);
  
}
void  DrawRelativeErrors(TH2F* hcov,TH1F* hh,TTree *deftree,TTree *tree,TString var,int nbins,double* bins,TString cut_nom,TString opt){
  TH1F hn_nom("hn_nom","",nbins,bins);
  TH2F hn_a("hn_a","",1000,0,1000,nbins,bins);
  TH2F hn_va("hn_va","",1000,0,1000,nbins,bins);
  DrawRelativeErrors(hcov,hh,deftree,tree,var,nbins,hn_nom,hn_a,hn_va,cut_nom,opt);
}

void makehighlandsysthist(TString infile,TString outfile,TString update="UPDATE",bool regular=false,int _nbins=50)
{
  
  int low=0;
  int high=5000;
  //Asher's binning
  int nbins[3]={5,5,5};
  double bins[3][6]={0,500,900,1200,2000,30000,
		      0,500,900,1200,2000,30000,
		      0,600,1000,1500,2200,30000};

  
  TString mcfile    =infile;
  TString name      =outfile;
  TFile *outf=new TFile(name.Data(),update);
  DrawingTools *draw=new DrawingTools(mcfile.Data(),false);
  DataSample   *mc  =new DataSample(mcfile.Data()); 
  TString systname[14]={"bfield","momresol","momscale","tpcpid","chargeideff","tpcclustereff","tpctrackeff","tpcfgdmatcheff","pileup","fgdmass","oofv","sipion","sandmu","all"};
  TString path="numuBkg";
  outf->mkdir(path.Data(),path.Data());

  TString treename[14];
  TH1F *hh[3];
  TH2F *hcov[3];
  TString scuth2[3]={"accum_level[][0]>6","accum_level[][0]>7","accum_level[][1]>7"};
  TString nn[3]={"CC","CC1Track","CCNTracks"};  
  TTree *deftree=(TTree*)mc->GetTree("default");
  for(int isyst=0;isyst<14;isyst++){   
    TString systname2,opt; 
    convert_fname_systname(systname[isyst],systname2,opt);
    std::cout<<" name2 "<<systname2<<std::endl;
    treename[isyst]=systname2;
   
    if(!mc->GetTree(treename[isyst].Data()) ){
      std::cout<<" tree "<<treename[isyst]<<" does not exist! "<<std::endl;
      continue;
    }
    if(treename[isyst]!="tpcpid_syst" )continue;
    TString path2=path+"/"+systname2;
    outf->mkdir(path2.Data(),path2.Data());
    for(int ih=0;ih<3;ih++){
     
      std::cout<<" ih "<<ih<<" nn "<<nn[ih]<<std::endl;
      if(regular){
	hcov[ih]=new TH2F(Form("hcov%s%s",nn[ih].Data(),systname[isyst].Data()),"",_nbins,low,high,_nbins,low,high);
	hh[ih]=new TH1F(Form("hh%s%s",nn[ih].Data(),systname[isyst].Data()),"",_nbins,low,high);

      }else{
	hcov[ih]=new TH2F(Form("hcov%s%s",nn[ih].Data(),systname[isyst].Data()),"",nbins[ih],bins[ih],nbins[ih],bins[ih]);
	hh[ih]=new TH1F(Form("hh%s%s",nn[ih].Data(),systname[isyst].Data()),"",nbins[ih],bins[ih]);
      }
      TString scut=scuth2[ih];

          std::cout<<treename[isyst].Data()<<std::endl;
      TTree *tree=(TTree*) mc->GetTree(treename[isyst].Data());      
      if(regular)
      	DrawRelativeErrors(hcov[ih],hh[ih],deftree,tree,"selmu_mom",_nbins,low,high,scut.Data(),opt);
      else
      	DrawRelativeErrors(hcov[ih],hh[ih],deftree,tree,"selmu_mom",nbins[ih],bins[ih],scut.Data(),opt);

      hh[ih]->SetTitle(Form("%s for %s",systname[isyst].Data(),nn[ih].Data()));
      hh[ih]->GetYaxis()->SetTitle("Relative error");
      hh[ih]->GetXaxis()->SetTitle("Reconstructed momentum (MeV/c)");
      outf->cd();     
      TString path3=path2+"/"+nn[ih];
      outf->mkdir(path3.Data(),path3.Data());
      outf->cd(path3.Data());
      hh[ih]->SetName(Form("relerr_h2_%s_%s",systname[isyst].Data(),nn[ih].Data()));
      hh[ih]->Write();
      hcov[ih]->SetName(Form("relcov_h2_%s_%s",systname[isyst].Data(),nn[ih].Data()));
      hcov[ih]->Write();
    }
  }
  outf->Close();
}
