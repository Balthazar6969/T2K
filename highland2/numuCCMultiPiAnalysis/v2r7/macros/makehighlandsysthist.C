void convert_fname_systname(TString fsystname,TString &systname2,TString &opt){


  if(fsystname=="bfield")             {opt="SSYS"; systname2="bfield_syst";} 
  else if(fsystname=="momresol")      {opt="SSYS"; systname2="momresol_syst";}
  else if(fsystname=="momres")        {opt="SSYS"; systname2="momresol_syst";}
  else if(fsystname=="momscale")      {opt="SSYS"; systname2="momscale_syst";}
  else if(fsystname=="tpcpid")        {opt="SSYS"; systname2="tpcpid_syst";}
  else if(fsystname=="tpcpid2")       {opt="SSYS"; systname2="tpcpid_syst";}
  else if(fsystname=="fgdpid")        {opt="SSYS"; systname2="fgdpid_syst";}
  else if(fsystname=="chargeideff")   {opt="WSYS"; systname2="chargeideff_syst";}
  else if(fsystname=="qideff")        {opt="WSYS"; systname2="chargeideff_syst";}
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
  else if(fsystname=="all")           {opt="SYS"; systname2 ="all_syst";}
  
  std::cout<<" name "<<systname2<<std::endl;
  
  return;
}



void  DrawRelativeErrors(TH2F *hcov,TH1F* hh,TTree *deftree,TTree *tree,TString var,int nbins,TH1F &hn_nom, TH2F &hn_a, TH2F &hn_va,TString cut_nom,TString opt,double &totrelerr){

  TH1F h0("v","v",1,0,1);
  tree->Project("v","0.5","NTOYS","",1);
  int ntoys=h0.GetBinContent(1);

  deftree->Project("hn_nom",Form("%s",var.Data()),Form("(%s)*weight[0]*weight[1]",cut_nom.Data()));
  tree->Project("hn_a",Form("%s:toy_index",var.Data()),Form("(%s && weight_syst_total<100)*weight[0]*weight[1]*weight_syst_total",cut_nom.Data()));
  //tree->Project("hn_va",Form("%s:toy_index",var.Data()),Form("(%s)*weight[0]*weight[1]",cut_nom.Data()));
  //deftree->Project("hn_nom",Form("%s",var.Data()),Form("(%s)*weight[0]",cut_nom.Data()));
  //tree->Project("hn_a",Form("%s:toy_index",var.Data()),Form("(%s && weight_syst_total<100)*weight[0]*weight_syst_total",cut_nom.Data()));
  //tree->Project("hn_va",Form("%s:toy_index",var.Data()),Form("(%s)*weight[0]",cut_nom.Data()));


  float NA_av=0;
  float na_av[1000]={0};
  for(int i=0;i<nbins;i++){
    na_av[i]=0;
    for(int ia=0;ia<ntoys;ia++){
      float nai=hn_a.GetBinContent(ia+1,i+1);
      na_av[i]+=nai;
      NA_av+=nai;
    }
    na_av[i]=na_av[i]/(float)ntoys;
  }
  
  float cov[1000][1000]={{0}};
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
	//	if(i==j) std::cout<<i<<" rms "<<sqrt(cov[i][j])<<" it "<<ia<<" ntoys "<<ntoys<<" nai "<<nai<<" nav "<<na_av[i]<<" nnom "<<nnomi<<std::endl;
	 
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
  totrelerr=sqrt(RMS);

  /*
  double xmax=hh->GetXaxis()->GetXmax();
  double xmin=hh->GetXaxis()->GetXmin();
  TAxis *ax=(TAxis*) hh->GetXaxis();
  //TH2F *varvstoy=new TH2F("varvstoy","",ntoys,0,ntoys,10000,xmin,xmax);
  //tree->Project("varvstoy",Form("%s:toy_index",var.Data()),Form("(%s && weight_syst_total<100)*weight[0]*weight[1]*weight_syst_total",cut_nom.Data()));

  //  TAxis *ax2=(TAxis*) varvstoy->GetXaxis();
  TH1F *var_dispertion_perbin[1000];  
  TH1F *nentries_dispertion_perbin[1000];  
  float na_rms[1000]={0};
  TF1 *gg[1000];

  for(int i=0;i<nbins;i++){
    na_rms[i]=sqrt(cov[i][i])*na_av[i];
    // var_dispertion_perbin[i]=new TH1F(Form("vardispbin%d",i),"",100,ax->GetBinLowEdge(i+1),ax->GetBinUpEdge(i+1));
    nentries_dispertion_perbin[i]=new TH1F(Form("nentriesdispbin%d",i),"",25,na_av[i]-3*na_rms[i],na_av[i]+3*na_rms[i]);
  }

  int jmin=0;
  for(int i=0;i<nbins;i++){   
    for(int ia=0;ia<ntoys;ia++){
      nai=hn_a.GetBinContent(ia+1,i+1);
      nentries_dispertion_perbin[i]->Fill(nai);
    }
    nentries_dispertion_perbin[i]->Fit("gaus","R","",na_av[i]-3*na_rms[i],na_av[i]+3*na_rms[i]);
    nentries_dispertion_perbin[i]->Draw();
    //normalisation of a gaussian is: 1/sig*sqrt(2pi) *sizehist/nbins*nentries
    gg[i]=new TF1(Form("gg%i",i),"[0]*exp(-0.5*((x-[1])/([2]))**2)",na_av[i]-3*na_rms[i],na_av[i]+3*na_rms[i]);
    
    gg[i]->SetParameter(0,ntoys*6./(25.*sqrt(2*TMath::Pi()))); //6 because we go from -3sig +3sig=6/25 bins
    gg[i]->SetParameter(1,na_av[i]);
    gg[i]->SetParameter(2,na_rms[i]);
    gg[i]->SetLineColor(7);
    gg[i]->Draw("same");
    gg[i]->Write();
    nentries_dispertion_perbin[i]->Write();
  }
  */
  

}

void  DrawRelativeErrors(TH2F* hcov,TH1F* hh,TTree *deftree,TTree *tree,TString var,int nbins,float low,float high,TString cut_nom,TString opt, double &totrelerr){
   TH1F hn_nom("hn_nom","",nbins,low,high);
   TH2F hn_a("hn_a","",1000,0,1000,nbins,low,high);
   TH2F hn_va("hn_va","",1000,0,1000,nbins,low,high);
   DrawRelativeErrors(hcov,hh,deftree,tree,var,nbins,hn_nom,hn_a,hn_va,cut_nom,opt,totrelerr);
  
}
void  DrawRelativeErrors(TH2F* hcov,TH1F* hh,TTree *deftree,TTree *tree,TString var,int nbins,double* bins,TString cut_nom,TString opt,double totrelerr){
  TH1F hn_nom("hn_nom","",nbins,bins);
  TH2F hn_a("hn_a","",1000,0,1000,nbins,bins);
  TH2F hn_va("hn_va","",1000,0,1000,nbins,bins);
  DrawRelativeErrors(hcov,hh,deftree,tree,var,nbins,hn_nom,hn_a,hn_va,cut_nom,opt,totrelerr);
}


void makehighlandsysthist(TString stFGD,TString infile,TString outfile,int ishighland1,TString update="UPDATE",bool regular=false,int _nbins=50)
{
  
  int low=0;
  int high=5000;
  int nbins[4]={14,14,13,14};
  double bins[4][15]={0,300,400,500,600,700,800,900,1000,1250,1500,2000,3000,5000,30000,
		     0,300,400,500,600,700,800,900,1000,1250,1500,2000,3000,5000,30000,
		     0,300,400,500,600,700,800,900,1000,1250,1500,2000,3000,30000,30000,
		     0,300,400,500,600,700,800,900,1000,1250,1500,2000,3000,5000,30000};

  
  TString mcfile    =infile;
  TString name      =outfile;
  TFile *outf=new TFile(name.Data(),update);
#if ishighland1
  DrawingTools    *draw =new DrawingTools(mcfile.Data(),false);
  MultiAnalysisTools *mc=new MultiAnalysisTools(mcfile.Data());
  TString systname[18]={"bfield","momresol","momscale","tpcpid","fgdpid","chargeideff","tpcclustereff","tpctrackeff","fgdtrackeff","tpcfgdmatcheff","meeff","pileup","fgdmass","oofv","sipion","sandmu","",""};
#else  
  DrawingTools *draw=new DrawingTools(mcfile.Data(),false);
  DataSample   *mc  =new DataSample(mcfile.Data()); 
  TString systname[18]={"bfield","momresol","momscale","tpcpid","fgdpid","chargeideff","tpcclustereff","tpctrackeff","fgdhybtrackeff","fgdtrackeff","tpcfgdmatcheff","meeff","pileup","fgdmass","oofv","sipion","sandmu","all"};
#endif
  TString path="numuCC"+stFGD;
  outf->mkdir(path.Data(),path.Data());

  TString treename[18];
  TH1F *hh[4];
  TH2F *hcov[4];
  TString scuth2[4]={"accum_level[][0]>5","accum_level[][0]>6","accum_level[][1]>6","accum_level[][2]>6"};
  TString scuth1[4]={"accum_level>5","accum_level>5 && cut6==1","accum_level>5 && cut7==1","accum_level>5 && cut8==1"};
  TString nn[4]={"CC","CC0pi","CC1pi","CCOth"};  
  TTree *deftree=(TTree*)mc->GetTree("default");
  std::ofstream txtfile;
  txtfile.open("totalerror.txt");
  char ttt[500];

  for(int isyst=0;isyst<18;isyst++){   
    TString systname2,opt; 
    convert_fname_systname(systname[isyst],systname2,opt);
    std::cout<<" name2 "<<systname2<<std::endl;
    treename[isyst]=systname2;
    sprintf(ttt,"%s",treename[isyst].Data());txtfile<<ttt;
    if(!mc->GetTree(treename[isyst].Data()) ){
      std::cout<<" tree "<<treename[isyst]<<" does not exist! "<<std::endl;
      continue;
    }
    // if(treename[isyst]!="tpcpid_syst" )continue;
    TString path2=path+"/"+systname2;
    outf->mkdir(path2.Data(),path2.Data());
    for(int ih=0;ih<4;ih++){

      std::cout<<" ih "<<ih<<" nn "<<nn[ih]<<std::endl;
      if(regular){
	hcov[ih]=new TH2F(Form("hcov%s%s",nn[ih].Data(),systname[isyst].Data()),"",_nbins,low,high,_nbins,low,high);
	hh[ih]=new TH1F(Form("hh%s%s",nn[ih].Data(),systname[isyst].Data()),"",_nbins,low,high);

      }else{
	hcov[ih]=new TH2F(Form("hcov%s%s",nn[ih].Data(),systname[isyst].Data()),"",nbins[ih],bins[ih],nbins[ih],bins[ih]);
	hh[ih]=new TH1F(Form("hh%s%s",nn[ih].Data(),systname[isyst].Data()),"",nbins[ih],bins[ih]);
      }
#if ishighland1
      TString scut=scuth1[ih];
#else
      TString scut=scuth2[ih];
#endif
    
      std::cout<<treename[isyst].Data()<<std::endl;
      TTree *tree=(TTree*) mc->GetTree(treename[isyst].Data());
      outf->cd();     

      TString path3=path2+"/"+nn[ih];
      outf->mkdir(path3.Data(),path3.Data());
      outf->cd(path3.Data());
      double totrelerr=0;

#if  ishighland1
       if(regular)
       	draw->DrawRelativeErrors(tree,"selmu_mom",_nbins,low,high,scut.Data(),"",opt.Data());
       else
       	draw->DrawRelativeErrors(tree,"selmu_mom",nbins[ih],bins[ih],scut.Data(),"",opt.Data());
       hh[ih]->Reset();
       hh[ih] =(TH1F*) draw->GetLastHisto();
#else

      if(regular)
      	DrawRelativeErrors(hcov[ih],hh[ih],deftree,tree,"selmu_mom",_nbins,low,high,scut.Data(),opt,totrelerr);
      else
      	DrawRelativeErrors(hcov[ih],hh[ih],deftree,tree,"selmu_mom",nbins[ih],bins[ih],scut.Data(),opt,totrelerr);
      
      std::cout<<" !!!!!!!!!!! "<<totrelerr<<std::endl;
      

      // if(ih!=3)
      // 	sprintf(ttt,"\t %.4f \\% ",totrelerr*100);txtfile<<ttt;
      // else
      // 	sprintf(ttt,"\t %.4f \\% \n",totrelerr*100);txtfile<<ttt;
      if(ih!=3)
	txtfile<<" \t "<<totrelerr*100;
      else
	txtfile<<" \t "<<totrelerr*100<<std::endl;
#endif     
      hh[ih]->SetTitle(Form("%s for %s",systname[isyst].Data(),nn[ih].Data()));
      hh[ih]->GetYaxis()->SetTitle("Relative error");
      hh[ih]->GetXaxis()->SetTitle("Reconstructed momentum (MeV/c)");
#if ishighland1
      hh[ih]->SetName(Form("relerr_h1_%s_%s",systname[isyst].Data(),nn[ih].Data()));
      hh[ih]->Write();
#else
      hh[ih]->SetName(Form("relerr_h2_%s_%s",systname[isyst].Data(),nn[ih].Data()));
      hh[ih]->Write();
      hcov[ih]->SetName(Form("relcov_h2_%s_%s",systname[isyst].Data(),nn[ih].Data()));
      hcov[ih]->Write();
#endif
    }
  }
  outf->Close();
  txtfile.close();
}
