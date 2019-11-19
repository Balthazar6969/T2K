/*This macro allows to compare the results of the systematic propagation obtained with different systematic table and to compute di total detector systematic error.*/
TString key[2]={"h2","h2"};
TString key2[2]={"_reg20","_reg20"};
TString in_h2_p6t="hist_fth2_"+key[0]+"_100toys_6Brun5c_6Btables"+key2[0]+".root";//highland2 6B files 6B tables
TString in_h2_p5t="hist_fth2_"+key[0]+"_100toys_6Brun5c_5Ftables"+key2[0]+".root";//highland2 6B files 5F tables
TString inh2sand="hist_fth2_"+key[0]+"_100toys_6Brun5cSAND_6Btables"+key2[0]+".root";//sand


float sanderror[3]={0};
TH1F *hhsand[3][2]={NULL};
TH2F *hcovsand[3][2]={NULL};


float computetotalerror(TH2F *hcov, TH1F *hnom){
  int nbins=hnom->GetXaxis()->GetNbins();
  int nbins2=hcov->GetXaxis()->GetNbins();
  if(nbins!=nbins2){ 
    std::cout<<" ERROR: nbins are not the same"<<std::endl;// bins of cov matrix and nominal plot must be the same
    return 0;
  }
  float abserr=0;
  float relerr=0;
  float ntot=0;
  for(int ib=0;ib<nbins;ib++){
    for(int jb=0;jb<nbins;jb++){
      float cov=hcov->GetBinContent(ib+1,jb+1);
      float ni=hnom->GetBinContent(ib+1);
      float nj=hnom->GetBinContent(jb+1);
      abserr+=cov*ni*nj;
    }
    ntot+=hnom->GetBinContent(ib+1);
  }
  relerr=sqrt(abserr)/ntot;
  return relerr;
}

void makesandmuonsysthist(){
gStyle->SetOptStat(1110);
  TFile *infsand[2]={NULL};
  TFile *infnom[2]={NULL};
  for(int iff=0;iff<2;iff++){
    infsand[iff]= new TFile(inh2sand.Data(),"READ");//sand MC
    infnom[iff] = new TFile(in_h2_p6t.Data(),"READ");//magnet MC
  }
  TString systname="sandmu";
  TString systname_dir="sandmu_syst";
  TString systlatex="Sand muon background";

  TString nn[3]={"CC","CC1Track","CCNTracks"};
  TString path="numuBkg";

  float POTnorm=2.0495/1.17975;//nom/sand
  
  TCanvas *c1[4];
  TString path2=path+"/"+systname_dir;
  TH1F *hh[3][2];
  TH1F *hnom[3][2];
  TH1F *hnomsand[3][2];
  for(int ih=0;ih<3;ih++){
    c1[ih]=new TCanvas(Form("c1%s%d",systname.Data(),ih),Form("c1%s%d",systname.Data(),ih),1);
    TString path3=path2+"/"+nn[ih];//example: /numuBkg/tpcpid_syst/CC/
    for(int iff=0;iff<2;iff++){
      infsand[iff]->cd(path3.Data());
      TString pathnom=path+"/"+nn[ih];//example: numuBkg/CC_reg20      
      if(infsand[iff] && infnom[iff]){
	hhsand[ih][iff]=   (TH1F*) infsand[iff]->Get(Form("%s/relerr_%s_%s_%s",path3.Data(),key[iff].Data(),systname.Data(),nn[ih].Data()));//relative error vs momentum
	hcovsand[ih][iff]= (TH2F*) infsand[iff]->Get(Form("%s/relcov_%s_%s_%s",path3.Data(),key[iff].Data(),systname.Data(),nn[ih].Data()));//relative covariance matrix
	hnomsand[ih][iff]= (TH1F*) infsand[iff]->Get(Form("%s/mom_%s_%s",pathnom.Data(),key[iff].Data(),nn[ih].Data()));//momentum (sand MC)
	hnom[ih][iff]= (TH1F*) infnom[iff]->Get(Form("%s/mom_%s_%s",pathnom.Data(),key[iff].Data(),nn[ih].Data()));//momentum (magnet MC)
	
	int nbins=hnom[ih][iff]->GetXaxis()->GetNbins();
	int nbins2=hnomsand[ih][iff]->GetXaxis()->GetNbins();
	int nbins3=hcovsand[ih][iff]->GetXaxis()->GetNbins();
	if(nbins!=nbins2 || nbins!=nbins3 || nbins2!=nbins3) 
	  std::cout<<" ERROR: don't have the same binning! "<<std::endl;
	
	for(int ib=0;ib<nbins;ib++){
	  float val=hhsand[ih][iff]->GetBinContent(ib+1);
	  float ni =hnom[ih][iff]->GetBinContent(ib+1);
	  float nisand =hnomsand[ih][iff]->GetBinContent(ib+1)*POTnorm;
	  hhsand[ih][iff]->SetBinContent(ib+1,val*nisand/ni);
	  for(int jb=0;jb<nbins;jb++){
	    float njsand =hnomsand[ih][iff]->GetBinContent(jb+1)*POTnorm;
	    float nj   =hnom[ih][iff]->GetBinContent(jb+1);
	    float valij=hcovsand[ih][iff]->GetBinContent(ib+1,jb+1);
	    hcovsand[ih][iff]->SetBinContent(ib+1,jb+1,valij*nisand*njsand/(ni*nj));
	  }
	}
	
	if(hhsand[ih][iff])std::cout<<" ih "<<ih<<" iff "<<iff<<" exists! "<<std::endl;
      }
    }
  }
}

void compareprodsysthist(int opt){
  makesandmuonsysthist();
  TFile *inf[2];
  inf[0]=new TFile(in_h2_p6t.Data(),"READ");//prod6B files prod6B tables
  inf[1]=new TFile(in_h2_p5t.Data(),"READ");//prod6B files prod5F tables 

  std::ofstream txtfile;
  if(opt==0)
    txtfile.open("latex_6B-6B.txt");
  else if(opt==1)
    txtfile.open("latex_6B-5F.txt");
  else{ std::cout<<"error!!! option non correct. exit"<<std::endl; return 0;}

  char ttt[500];

  //systematic errors
  const int NSYST=14;
  TString systname[NSYST]={"bfield","momresol","momscale","tpcpid","chargeideff","tpcclustereff","tpctrackeff","tpcfgdmatcheff","oofv","pileup","fgdmass","sipion","all","sandmu"};
  TString systname_dir[NSYST]={"bfield_syst","momresol_syst","momscale_syst","tpcpid_syst","chargeideff_syst","tpcclustereff_syst","tpctrackeff_syst","tpcfgdmatcheff_syst","oofv_syst","pileup_syst","fgdmass_syst","sipion_syst","all_syst","sandmu_syst"};
  TString systlatex[NSYST]={"B field distortions","Momentum resolution","Momentum scale","TPC PID","Charge ID efficiency","TPC cluster efficiency","TPC track efficiency","TPC-FGD matching efficiency","OOFV background","Pile-up","FGD mass","Pion secondary interactions","All magnet","Sand muon background"};

  //samples
  TString nn[3]={"CC","CC1Track","CCNTracks"};
  //analysis
  TString path="numuBkg";
  
  TCanvas *c1[3];
  float totalerror[3]={0};
  for(int isyst=0;isyst<NSYST;isyst++){
    if(isyst==0){
      sprintf(ttt," \\hline \n \\multicolumn{4}{l}{\\textbf{Observable-variation systematics}} \\\\ \n \\hline \n");txtfile<<ttt;
    }else if(isyst==4){
      sprintf(ttt," \\hline \n \\multicolumn{4}{l}{\\textbf{Efficiency-like systematics}} \\\\ \n \\hline \n");txtfile<<ttt;
    }else if(isyst==8){
      sprintf(ttt," \\hline \n \\multicolumn{4}{l}{\\textbf{Normalization systematics}} \\\\ \n \\hline \n");txtfile<<ttt;
    }else if(isyst==NSYST-2){
      sprintf(ttt," \\hline \n \\multicolumn{4}{l}{\\textbf{ALL}} \\\\ \n \\hline \n");txtfile<<ttt;
    }
    
    sprintf(ttt,"%s",systlatex[isyst].Data());
    txtfile<<ttt;

    TH1F *hh[3][2]={NULL};
    TH2F *hcov[3][2]={NULL};
    TH1F *hnom[3][2]={NULL};
    for(int ih=0;ih<3;ih++){//loop on samples
      c1[ih]=new TCanvas(Form("c1%s%d",systname[isyst].Data(),ih),Form("c1%s%d",systname[isyst].Data(),ih),1);
      for(int iff=0;iff<2;iff++){
	TString pathnom=path+"/"+nn[ih];//table 5F or 6B pathnom?
	TString path3  =path+"/"+systname_dir[isyst]+"/"+nn[ih];
	TString name   = systname[isyst];
	std::cout<<" iff "<<iff<<" "<<systname[isyst]<<std::endl;
	if(inf[iff]->cd(path3.Data()) || name=="sandmu"){
	  hh[ih][iff]= (TH1F*) inf[iff]->Get(Form("%s/relerr_%s_%s_%s",path3.Data(),key[iff].Data(),name.Data(),nn[ih].Data()));
	  hcov[ih][iff]= (TH2F*) inf[iff]->Get(Form("%s/relcov_%s_%s_%s",path3.Data(),key[iff].Data(),name.Data(),nn[ih].Data()));
	  hnom[ih][iff]= (TH1F*) inf[iff]->Get(Form("%s/mom_%s_%s",pathnom.Data(),key[iff].Data(),nn[ih].Data()));
	  if(name=="sandmu"){
	    hh[ih][iff]=(TH1F*) hhsand[ih][iff];
	    hcov[ih][iff]=(TH2F*) hcovsand[ih][iff];
	  }
	    
	  if(hh[ih][iff] && opt==iff){
	    float error=computetotalerror(hcov[ih][iff],hnom[ih][iff]);
	    std::cout<<systname[isyst]<<" ih "<<ih<<" iff "<<iff<<" exists! "<<error<<std::endl;
	    if(isyst>=NSYST-2)
	      totalerror[ih]+=pow(error,2);
	    if(opt==iff){
	      sprintf(ttt," & %.4f",error*100);
	      txtfile<<ttt;
	    }
	  }
	}
      }

      gStyle->SetOptStat(1110);
      TLegend *leg = new TLegend(0.7,0.5,0.88,0.7);
      leg->SetFillColor(0);
      leg->SetLineColor(0);
      if(hh[ih][0]){
	hh[ih][0]->SetTitle(Form("%s for %s",systlatex[isyst].Data(),nn[ih].Data()));
	hh[ih][0]->SetLineColor(kBlue);
	leg->AddEntry(hh[ih][0],"6B files, 6B tables","f");
      }
      if(hh[ih][1]){
	hh[ih][1]->SetMarkerColor(2);
	hh[ih][1]->SetMarkerStyle(21);
	leg->AddEntry(hh[ih][1],"6B files, 5F tables","p");
      }
      
      float vmax0 =-9999., vmax1=-9999., vmax2=-9999., vmax3=-9999.,vmax=1.;
      if(hh[ih][0])vmax0 = TMath::Max(-999.,hh[ih][0]->GetMaximum());
      if(hh[ih][1])vmax1 = TMath::Max(vmax0,hh[ih][1]->GetMaximum());
      vmax=TMath::Max(vmax0,vmax1);
      //if(hh[ih][2])vmax3 = TMath::Max(vmax2,hh[ih][2]->GetMaximum());
      //vmax =TMath::Max(vmax2,vmax3);

      for(int iff=0;iff<2;iff++)
	if(hh[ih][iff])hh[ih][iff]->GetYaxis()->SetRangeUser(0.,vmax*1.1);
      if(hh[ih][0])      hh[ih][0]->Draw();
      else if(hh[ih][1]) hh[ih][1]->Draw();
      //else if(hh[ih][2]) hh[ih][2]->Draw();
      
      if(hh[ih][1]) hh[ih][1]->Draw("p same");    
      //if(hh[ih][2]) hh[ih][2]->Draw("same");

      leg->Draw("same");
      c1[ih]->Print(Form("numuBkgValidation/%s_%s.png",name.Data(),nn[ih].Data()));
    }
    txtfile<<"\\\\ \n";
  }
  txtfile<<"TOTAL";
  for(int ih=0;ih<3;ih++){
    sprintf(ttt," & %.4f",sqrt(totalerror[ih])*100);
    txtfile<<ttt;
  }
  txtfile<<"\\\\ \n \\hline";
  txtfile.close();
}


