#include "TMath.h"
#include "TGraphAsymmErrors.h"
#include "TH1F.h"

//TString mcfile="/atlas/users/mravonel/t2k/nd280/highland2/numuCCAnalysis/v0r26/amd64_linux26/outfiles_new/CCnom_allw5_5Fb.root";
//TString mcfile="/atlas/users/mravonel/t2k/nd280/highland/numuCCAnalysis/v0r26/amd64_linux26/outfiles_new/CCnom_fgdtrw8_5Fb.root";

//TString mcfile="/atlas/users/mravonel/t2k/nd280/highland2/numuCCMultiPiAnalysis/v0r2/amd64_linux26/work/outfiles_new/CCnPIh2_corr_syst_ft_6Brun2w_p5tables_0_0.root";
//TString mcfile="/atlas/users/mravonel/t2k/nd280/highland2/numuCCMultiPiAnalysis/v0r2/amd64_linux26/work/outfiles_new/CCnPIh2_corr_accum12_fth2_1_6Brun2w_6Btables.root";
//TString mcfile="/atlas/users/mravonel/t2k/nd280/highland2/numuCCMultiPiAnalysis/v0r2/amd64_linux26/work/outfiles_new/CCnPIh2_corr_hyb_fth2_100_6Brun2w_6Btables.root";
TString mcfile="/atlas/users/mravonel/t2k/nd280/highland2/numuCCMultiPiAnalysis/v0r6/amd64_linux26/work/outfiles_new/CCnPIh2_corr_syst_fth2_100_6Brun2w_6Btables.root";



void compute_Eff(double *cc_nwrong,double *cc_ncorr,TH1F **cc_wrong,TH1F **cc_corr,TH1F *hcc_tot,std::string name){
  int nbins=hcc_tot->GetNbinsX();
  std::cout<<"\n ======== "<<name<<": "<<nbins<<" ========= "<<std::endl;
   TH1F *hcc_corr=new TH1F("hcc_corr","",nbins,0,nbins);
   for(int ip=0;ip<nbins;ip++){
     for(int i=0;i<10;i++){
       //cout<<cc_wrong[ip]->GetBinContent(i+1)<<" "<<cc_corr[ip]->GetBinContent(i+1)<<std::endl;
       cc_nwrong[ip]+=cc_wrong[ip]->GetBinContent(i+1)*(i);
       cc_ncorr[ip]+=cc_corr[ip]->GetBinContent(i+1)*(i);
     }
     double tot=cc_ncorr[ip]+cc_nwrong[ip];
     hcc_tot->SetBinContent(ip+1,tot);
     hcc_corr->SetBinContent(ip+1,cc_ncorr[ip]);
     if(tot!=0)
       std::cout<<"ip "<<ip<<" eff "<<cc_ncorr[ip]/tot<<" \t tot "<<tot<<" \t ncorr "<<cc_ncorr[ip]<<std::endl;
   }
   TGraphAsymmErrors *gcc=new TGraphAsymmErrors();
   gcc->BayesDivide(hcc_corr,hcc_tot);
   for(int ip=0;ip<nbins;ip++){
     double nx,ny,erry,erryh;
     gcc->GetPoint(ip,nx,ny);
     erry=gcc->GetErrorYlow(ip);
     erryh=gcc->GetErrorYhigh(ip);
     // std::cout<<nx<<" \t "<<ny<<" \t "<<erry<<" \t "<<erryh<<std::endl;
     
   }
   delete hcc_corr;
}

void compute_AnaSampleEff_p6B(TString mcfile1=mcfile){
 
  DrawingTools *draw=new DrawingTools(mcfile1.Data(),false);
  DataSample *mc1=new DataSample(mcfile1.Data());
  TTree *tree=mc1->GetTree("default");
  
  TH1F *chargeid_corr[15];
  TH1F *chargeid_wrong[15];
  TH1F *chargeid_tot[15];
  
  TH1F *tpcfgd_corr[3];
  TH1F *tpcfgd_wrong[3];
  TH1F *tpcfgd_tot[3];
  
  TH1F *tpctr_corr[3];
  TH1F *tpctr_wrong[3];
  TH1F *tpctr_tot[3];

  TH1F *fgdtr_corr[7];
  TH1F *fgdtr_wrong[7];
  TH1F *fgdtr_tot[7];

  TH1F *fgdhybtr_corr[20];
  TH1F *fgdhybtr_wrong[20];
  TH1F *fgdhybtr_tot[20];
  
  
  TH1F *meeff_corr[1];
  TH1F *meeff_wrong[1];
  TH1F *meeff_tot[1];

  TH1F *mepur_corr[4];
  TH1F *mepur_wrong[4];
  TH1F *mepur_tot[4];
  

  if(mc1->GetTree("chargeideff_syst")){
    tree=mc1->GetTree("chargeideff_syst");

   for(int ip=0;ip<15;ip++){
     chargeid_corr[ip]=new TH1F(Form("chargeid_corr_pbin%d",ip),"",10,-0.5,10.5);
     chargeid_wrong[ip]=new TH1F(Form("chargeid_wrong_pbin%d",ip),"",10,-0.5,10.5);
     tree->Project(Form("chargeid_corr_pbin%d",ip),Form("chargeid_ncorrect[%d]",ip),"accum_level[0][0]>1");
     tree->Project(Form("chargeid_wrong_pbin%d",ip),Form("chargeid_nwrong[%d]",ip),"accum_level[0][0]>1");
   }
  }
  if(mc1->GetTree("tpcfgdmatcheff_syst")){
   tree=mc1->GetTree("tpcfgdmatcheff_syst");
   for(int ip=0;ip<3;ip++){
     tpcfgd_corr[ip]=new TH1F(Form("tpcfgdmatch_corr_pbin%d",ip),"",10,-0.5,10.5);
     tpcfgd_wrong[ip]=new TH1F(Form("tpcfgdmatch_wrong_pbin%d",ip),"",10,-0.5,10.5);
     tree->Project(Form("tpcfgdmatch_corr_pbin%d",ip),Form("tpcfgdmatch_ncorrect[%d]",ip),"accum_level[0][0]>1");
     tree->Project(Form("tpcfgdmatch_wrong_pbin%d",ip),Form("tpcfgdmatch_nwrong[%d]",ip),"accum_level[0][0]>1");
   }
  }
  if(mc1->GetTree("tpctrackeff_syst")){
   tree=mc1->GetTree("tpctrackeff_syst");
   for(int ip=0;ip<3;ip++){
     tpctr_corr[ip]=new TH1F(Form("tpctr_corr_pbin%d",ip),"",10,-0.5,10.5);
     tpctr_wrong[ip]=new TH1F(Form("tpctr_wrong_pbin%d",ip),"",10,-0.5,10.5);
     tree->Project(Form("tpctr_corr_pbin%d",ip),Form("tpctr_ncorrect[%d]",ip),"accum_level[0][0]>1");
     tree->Project(Form("tpctr_wrong_pbin%d",ip),Form("tpctr_nwrong[%d]",ip),"accum_level[0][0]>1");
   }
  }
  if(mc1->GetTree("fgdtrackeff_syst")){
   tree=mc1->GetTree("fgdtrackeff_syst");
   for(int ip=0;ip<7;ip++){
     fgdtr_corr[ip]=new TH1F(Form("fgdtr_corr_pbin%d",ip),"",10,-0.5,10.5);
     fgdtr_wrong[ip]=new TH1F(Form("fgdtr_wrong_pbin%d",ip),"",10,-0.5,10.5);
     tree->Project(Form("fgdtr_corr_pbin%d",ip),Form("fgdtr_ncorrect[%d]",ip),"accum_level[0][0]>1");
     tree->Project(Form("fgdtr_wrong_pbin%d",ip),Form("fgdtr_nwrong[%d]",ip),"accum_level[0][0]>1");
   }
  }
  if(mc1->GetTree("fgdhybridtrackeff_syst")){
   tree=mc1->GetTree("fgdhybridtrackeff_syst");
   for(int ip=0;ip<20;ip++){
     fgdhybtr_corr[ip]=new TH1F(Form("fgdhybtr_corr_pbin%d",ip),"",10,-0.5,10.5);
     fgdhybtr_wrong[ip]=new TH1F(Form("fgdhybtr_wrong_pbin%d",ip),"",10,-0.5,10.5);
     tree->Project(Form("fgdhybtr_corr_pbin%d",ip),Form("fgdhybtr_ncorrect[%d]",ip),"accum_level[0][0]>1");
     tree->Project(Form("fgdhybtr_wrong_pbin%d",ip),Form("fgdhybtr_nwrong[%d]",ip),"accum_level[0][0]>1");
   }
  }
  if(mc1->GetTree("michel_syst")){
   tree=mc1->GetTree("michel_syst");
   for(int ip=0;ip<1;ip++){
     meeff_corr[ip]=new TH1F(Form("meeff_corr_pbin%d",ip),"",10,-0.5,10.5);
     meeff_wrong[ip]=new TH1F(Form("meeff_wrong_pbin%d",ip),"",10,-0.5,10.5);
     tree->Project(Form("meeff_corr_pbin%d",ip),Form("meeff_ncorrect[%d]",ip),"accum_level[0][0]>1");
     tree->Project(Form("meeff_wrong_pbin%d",ip),Form("meeff_nwrong[%d]",ip),"accum_level[0][0]>1");
   }
  }
   if(mc1->GetTree("michel_syst")){
   tree=mc1->GetTree("michel_syst");
   for(int ip=0;ip<4;ip++){
     mepur_corr[ip]=new TH1F(Form("mepur_corr_pbin%d",ip),"",10,-0.5,10.5);
     mepur_wrong[ip]=new TH1F(Form("mepur_wrong_pbin%d",ip),"",10,-0.5,10.5);
     tree->Project(Form("mepur_corr_pbin%d",ip),Form("mepur_ncorrect[%d]",ip),"accum_level[0][0]>1");
     tree->Project(Form("mepur_wrong_pbin%d",ip),Form("mepur_nwrong[%d]",ip),"accum_level[0][0]>1");
   }
  }
  
   double chargeid_ncorr[15]={0};
   double chargeid_nwrong[15]={0};
   double tpcfgd_ncorr[3]={0};
   double tpcfgd_nwrong[3]={0};
   double tpctr_ncorr[3]={0};
   double tpctr_nwrong[3]={0};
   double fgdtr_ncorr[7]={0};
   double fgdtr_nwrong[7]={0};
   double fgdhybtr_ncorr[12]={0};
   double fgdhybtr_nwrong[12]={0};
   double meeff_ncorr[1]={0};
   double meeff_nwrong[1]={0};
   double mepur_ncorr[4]={0};
   double mepur_nwrong[4]={0};

  
   TH1F *hcc_tot=new TH1F("hcc_tot","",15,0,15);
   TH1F *htpcfgd_tot=new TH1F("htpcfgd_tot","",3,0,3);  
   TH1F *htpctr_tot=new TH1F("htpctr_tot","",3,0,3);  
   TH1F *hfgdtr_tot=new TH1F("hfgdtr_tot","",7,0,7);  
   TH1F *hfgdhybtr_tot=new TH1F("hfgdhybtr_tot","",12,0,12);  
   TH1F *hmeeff_tot=new TH1F("hmeeff_tot","",1,0,1);  
   TH1F *hmepur_tot=new TH1F("hmepur_tot","",4,0,4);  
   if(mc1->GetTree("chargeconf_syst"))
     compute_Eff(chargeid_nwrong,chargeid_ncorr,chargeid_wrong,chargeid_corr,hcc_tot,"Q ID EFF");
   if(mc1->GetTree("tpcfgdmatcheff_syst"))
     compute_Eff(tpcfgd_nwrong,tpcfgd_ncorr,tpcfgd_wrong,tpcfgd_corr,htpcfgd_tot,"TPCFGD-MATCH EFF");
   if(mc1->GetTree("tpctrackeff_syst"))
     compute_Eff(tpctr_nwrong,tpctr_ncorr,tpctr_wrong,tpctr_corr,htpctr_tot,"TPC TRACK EFF");
   if(mc1->GetTree("fgdtrackeff_syst"))
     compute_Eff(fgdtr_nwrong,fgdtr_ncorr,fgdtr_wrong,fgdtr_corr,hfgdtr_tot,"FGD TRACK EFF");
   if(mc1->GetTree("fgdhybridtrackeff_syst"))
     compute_Eff(fgdhybtr_nwrong,fgdhybtr_ncorr,fgdhybtr_wrong,fgdhybtr_corr,hfgdhybtr_tot,"FGD HYBRID TRACK EFF");
   if(mc1->GetTree("michel_syst"))
     compute_Eff(meeff_nwrong,meeff_ncorr,meeff_wrong,meeff_corr,hmeeff_tot,"ME EFF");
   if(mc1->GetTree("michel_syst"))
     compute_Eff(mepur_nwrong,mepur_ncorr,mepur_wrong,mepur_corr,hmepur_tot,"ME PUR");
  
  
}
