#define SystematicsAnalysis_cxx


#include "SystematicsAnalysis.h"
#include <stdio.h>
#include <iostream>


//********************************************************************
SystematicsAnalysis::SystematicsAnalysis(const std::string& file){
//********************************************************************

  _flatTreeTools = new NumuFlatTreeTools(file);
  //  _oaAnalysisTreeTools = new oaAnalysisTreeTools(file);

  Build();

}

//********************************************************************
void SystematicsAnalysis::Build(){
//********************************************************************


  // Number of analyses we want to run for systematic error calculation. I by default


  fRandomGenerator = TRandom3(0);


  int NANA = 50;

  // no correction nor systematic
  AddConfiguration("default");
  SetNAnalyses("default",1);

  //-------- corrections -----------
  
  // ALL corrections and  no systematics
  AddConfiguration("all_corr");
  SetNAnalyses("all_corr",1);
  SetEnabled("all_corr","charge_misid_corr", true);
  SetEnabled("all_corr","CT_corr", true);

  // expected CT correction only and  no systematics
  AddConfiguration("CTexp_corr");
  SetNAnalyses("CTexp_corr",1);
  SetEnabled("CTexp_corr","CTexp_corr", true);


  // CT correction only and  no systematics
  AddConfiguration("CT_corr");
  SetNAnalyses("CT_corr",1);
  SetEnabled("CT_corr","CT_corr", true);

  // Charge mis-id correction only and  no systematics
  AddConfiguration("charge_misid_corr");
  SetNAnalyses("charge_misid_corr",1);
  SetEnabled("charge_misid_corr","charge_misid_corr", true);


  //-------- systematics -----------

  // Systematic on momentum scale (impact on mometum only)
  AddConfiguration("mom_scale_syst");
  SetNAnalyses("mom_scale_syst",NANA);
  SetEnabled("mom_scale_syst","mom_scale", true);
  //  SetEnabled("mom_scale_syst","charge_misid_corr", true);
  //  SetEnabled("mom_scale_syst","CT_corr", true);

  // Systematic on momentum scale (impact on TPC PID pull only)
  AddConfiguration("mom_scale_pull_syst");
  SetNAnalyses("mom_scale_pull_syst",NANA);
  SetEnabled("mom_scale_pull_syst","mom_scale_pull", true);
  SetEnabled("mom_scale_pull_syst","charge_misid_corr", true);
  SetEnabled("mom_scale_pull_syst","CT_corr", true);

  // Systematic on momentum scale (impact on both TPC PID pull and momentum)
  AddConfiguration("mom_scale_all_syst");
  SetNAnalyses("mom_scale_all_syst",NANA);
  SetEnabled("mom_scale_all_syst","mom_scale_pull", true);
  SetEnabled("mom_scale_all_syst","mom_scale", true);
  SetEnabled("mom_scale_all_syst","charge_misid_corr", true);
  SetEnabled("mom_scale_all_syst","CT_corr", true);

  // Systematic on momentum resolution
  AddConfiguration("mom_resol_syst");
  SetNAnalyses("mom_resol_syst",NANA);
  SetEnabled("mom_resol_syst","mom_resol", true);
  SetEnabled("mom_resol_syst","charge_misid_corr", true);
  SetEnabled("mom_resol_syst","CT_corr", true);

  // Systematic on TPC PID correction
  AddConfiguration("tpcpid_syst");
  SetNAnalyses("tpcpid_syst",NANA);
  SetEnabled("tpcpid_syst","charge_misid_corr", true);
  SetEnabled("tpcpid_syst","CT_corr", true);
  SetEnabled("tpcpid_syst","CT_corr_syst", true);

  // Systematic on charge mis-id
  AddConfiguration("charge_misid_syst");
  SetNAnalyses("charge_misid_syst",NANA);
  SetEnabled("charge_misid_syst","charge_misid", true);
  SetEnabled("charge_misid_syst","charge_misid_corr", true);
  SetEnabled("charge_misid_syst","CT_corr", true);

  // All systematics
  AddConfiguration("all_syst");
  SetNAnalyses("all_syst",NANA);
  SetEnabled("all_syst","mom_scale", true);
  SetEnabled("all_syst","mom_scale_pull", true);
  //  SetEnabled("all_syst","mom_resol", true);
  SetEnabled("all_syst","charge_misid", true);
  SetEnabled("all_syst","CT_corr_syst", true);

  SetEnabled("all_syst","charge_misid_corr", true);
  SetEnabled("all_syst","CT_corr", true);


  // systematics disabled by default
  DisableSystematics();
  DisableAllConfigurations();
  EnableConfiguration("default");


  _pull_computed=false;

  _CT_correction_file="POTCTcorrection.dat";

}

//********************************************************************
void SystematicsAnalysis::Initialize(){
//********************************************************************

  DrawingTools::Initialize();

  if (IsEnabled("CT_corr"))
    ReadCTCorrections(_CT_correction_file);

  // initialize parameters for systematics error study
  InitializeAnalysisParameters();
  
}

//********************************************************************
void SystematicsAnalysis::InitializeAnalysisParameters(){
//********************************************************************


  // Here we initialise the random numbers for the N analyses

  //------ charge misid (in %) for different bins of NDOF -----------
  const int NNDOFBINS=10;
  //                                   0-7    7-14  14-21  21-28 28-35 35-42 42-49  49-56 56-70   70-
  double cmisid_data[NNDOFBINS]     = {24.3,  25.1,  11.6,  9.7,  1.8,  4.2,  18.3,  3.4,  4.6,  3.6};
  double cmisid_mc[NNDOFBINS]       = {30.1,  11.8,   8.9,  6.5,  5.8,  5.0,   6.1,  5.2,  2.4,  2.5};
  double cmisid_data_err[NNDOFBINS] = {16.5,  11.6,   6.5,  5.8,  2.1,  3.6,   8.6,  4.1,  2.5,  0.9};

  // ---- Single parameters ---------
  AddParameter("delta_presol");
  AddParameter("delta_tpc_presol");
  AddParameter("mom_scale");
  // ---- A vector of  parameters ---------
  const unsigned int NRUNS = _CT_correction[0].size();
  AddParameterVector("CT_corr_tpc1",NRUNS);
  AddParameterVector("CT_corr_tpc2",NRUNS);
  AddParameterVector("CT_corr_tpc3",NRUNS);
  AddParameterVector("delta_charge_misid",NNDOFBINS);


  //----- Loop over virtual analyses --------------
  for (int i=1;i<GetNMaxAnalyses();i++){

    // momentum resolution systematic 
    SetParameter("delta_presol", i, 0.05 );
    SetParameter("delta_tpc_presol", i, 0.05 );

    // momentum scale systematic 
    SetParameter("mom_scale", i, 1. + fRandomGenerator.Gaus(0, 0.05) );


    // CT correction systematic 
    for (int tpc=0;tpc<3;tpc++){
      std::map<std::pair<int,int>,std::pair<double,double> >::iterator it;
      int j=0;
      for (it=_CT_correction[tpc].begin();it!=_CT_correction[tpc].end();it++,j++){
	double deltaCT_err = it->second.second;    
	if (tpc==0)
	  SetParameter("CT_corr_tpc1",i,j, fRandomGenerator.Gaus(0., deltaCT_err)  );	
	else if (tpc==1)
	  SetParameter("CT_corr_tpc2",i,j, fRandomGenerator.Gaus(0., deltaCT_err)  );	
	else if (tpc==2)
	  SetParameter("CT_corr_tpc3",i,j, fRandomGenerator.Gaus(0., deltaCT_err)  );	
      }
    }

    // charge mis-id systematic
    for (int j=0;j<NNDOFBINS;j++){
      double delta_cmisid_mean = (cmisid_data[j]-cmisid_mc[j])/100.;
      double delta_cmisid = fRandomGenerator.Gaus(delta_cmisid_mean, cmisid_data_err[j]/100.);
      SetParameter("delta_charge_misid",i,j, delta_cmisid);
      //      std::cout << i << " " << j << " " << cmisid_data_err[j]/100. << " " << delta_cmisid_mean << " " << delta_cmisid << std::endl;
    }
  }

  //--------- Random number for reference analysis ---------------

  SetParameter("mom_scale",        0, 1.);
  SetParameter("delta_presol",     0, 0 );
  SetParameter("delta_tpc_presol", 0, 0 );


  for (int j=0;j<NNDOFBINS;j++){    
    double delta_cmisid = (cmisid_data[j]-cmisid_mc[j])/100.;
    SetParameter("delta_charge_misid",0,j,delta_cmisid);
  }


  for (unsigned int j=0;j<NRUNS;j++){
    SetParameter("CT_corr_tpc1",0,j, 0);
    SetParameter("CT_corr_tpc2",0,j, 0);
    SetParameter("CT_corr_tpc3",0,j, 0);
  }


}

//********************************************************************
bool SystematicsAnalysis::Process(){
//********************************************************************

  //------ This function is called for each event  ----------

  return true;
      
}


//**************************************************
double SystematicsAnalysis::GetCharge(int itrk){
//**************************************************

  if (!IsEnabled("charge_misid_corr"))
    return GetTreeTools().GetCharge(itrk);

  double q0 = GetTreeTools().GetCharge(itrk);
  double q=q0;

  double delta_misid = GetDeltaChargeMisId(itrk);

  double random = fRandomGenerator.Uniform();

  if (random < delta_misid)
    q =-q0;

  return q;
}

//********************************************************************
double SystematicsAnalysis::GetMomentum(int itrk){
//********************************************************************

  double p0 = GetTreeTools().GetMomentum(itrk);
  double p=p0;

  if (IsEnabled("mom_scale"))
    p = p0*GetParameter("mom_scale",GetAnalysisIndex());

  if (IsEnabled("mom_resol"))
    p *= (1 + fRandomGenerator.Gaus(0., GetParameter("delta_presol",GetAnalysisIndex())));
  
  return p;

}

//********************************************************************
double SystematicsAnalysis::GetTpcMomentum(int itrk, int tpc){
//********************************************************************

  double p0 = GetTreeTools().GetTpcMomentum(itrk,tpc);
  double p=p0;

  if (IsEnabled("mom_scale"))
    p = p0*GetParameter("mom_scale",GetAnalysisIndex());

  if (IsEnabled("mom_resol"))
    p *= (1 + fRandomGenerator.Gaus(0., GetParameter("delta_tpc_presol",GetAnalysisIndex())));

  return p;

}


//********************************************************************
double SystematicsAnalysis::GetTpcMomentumForPull(int itrk, int tpc){
//********************************************************************

  double p0 = GetTreeTools().GetTpcMomentum(itrk,tpc);
  double p=p0;

  if (IsEnabled("CTexp_corr"))
    p = p0*0.959;

  if (IsEnabled("mom_scale_pull"))
    p = p0*GetParameter("mom_scale",GetAnalysisIndex());

  if (IsEnabled("mom_resol_pull"))
    p *= (1 + fRandomGenerator.Gaus(0., GetParameter("delta_tpc_presol",GetAnalysisIndex())));

  return p;

}

//********************************************************************
double SystematicsAnalysis::GetDeltaChargeMisId(int itrk){
//********************************************************************

  int ndof_bin = 0;
  int ndof = GetTreeTools().GetTpcNHits(itrk,0)+GetTreeTools().GetTpcNHits(itrk,1)+GetTreeTools().GetTpcNHits(itrk,2)-8;
  if      (ndof < 7)              ndof_bin=0;
  else if (ndof >= 7 && ndof<14)  ndof_bin=1;
  else if (ndof >= 14 && ndof<21) ndof_bin=2;
  else if (ndof >= 21 && ndof<28) ndof_bin=3;
  else if (ndof >= 28 && ndof<35) ndof_bin=4;
  else if (ndof >= 35 && ndof<42) ndof_bin=5;
  else if (ndof >= 42 && ndof<49) ndof_bin=6;
  else if (ndof >= 49 && ndof<56) ndof_bin=7;
  else if (ndof >= 56 && ndof<70) ndof_bin=8;
  else if (ndof >= 70)            ndof_bin=9;

  //  std::cout << ndof << " " << ndof_bin << " " << GetParameter("delta_charge_misid",GetAnalysisIndex(),ndof_bin) << std::endl;

  if (IsEnabled("charge_misid"))
    return GetParameter("delta_charge_misid",GetAnalysisIndex(),ndof_bin);
  else
    return GetParameter("delta_charge_misid",GetRefAnalysisIndex(),ndof_bin);

}

//**************************************************
double SystematicsAnalysis::GetTpcPullmu(int itrk, int tpc){
//**************************************************

  double pull = 999999.;
  if (GetTpcCTerrmu(itrk,tpc)!=0 && GetTreeTools().GetTpcPullmu(itrk,tpc)<1e5 && !isnan(GetTreeTools().GetTpcPullmu(itrk,tpc)))
    pull = (GetTpcCT(itrk, tpc)-GetTpcCTexpmu(itrk,tpc))/GetTpcCTerrmu(itrk,tpc);

  return pull;
  
}

//**************************************************
double SystematicsAnalysis::GetTpcPullpi(int itrk, int tpc){
//**************************************************

  double pull = 999999.;
  if (GetTreeTools().GetTpcCTerrpi(itrk,tpc)!=0 && GetTreeTools().GetTpcPullpi(itrk,tpc)<1e5 && !isnan(GetTreeTools().GetTpcPullpi(itrk,tpc)))
    pull = (GetTpcCT(itrk, tpc)-GetTpcCTexppi(itrk,tpc))/GetTreeTools().GetTpcCTerrpi(itrk,tpc);

  return pull;
  
}


//**************************************************
double SystematicsAnalysis::GetTpcPullele(int itrk, int tpc){
//**************************************************

  double pull = 999999.;
  if (GetTreeTools().GetTpcCTerrele(itrk,tpc)!=0 && GetTreeTools().GetTpcPullele(itrk,tpc)<1e5 && !isnan(GetTreeTools().GetTpcPullele(itrk,tpc)))
    pull = (GetTpcCT(itrk, tpc)-GetTpcCTexpele(itrk,tpc))/GetTreeTools().GetTpcCTerrele(itrk,tpc);

  return pull;
  
}


//**************************************************
double SystematicsAnalysis::GetTpcPullp(int itrk, int tpc){
//**************************************************

  double pull = 999999.;
  if (GetTreeTools().GetTpcCTerrp(itrk,tpc)!=0 && GetTreeTools().GetTpcPullp(itrk,tpc)<1e5 && !isnan(GetTreeTools().GetTpcPullp(itrk,tpc)))
    pull = (GetTpcCT(itrk, tpc)-GetTpcCTexpp(itrk,tpc))/GetTreeTools().GetTpcCTerrp(itrk,tpc);


  return pull;
  
}


//**************************************************
double SystematicsAnalysis::GetTpcCT(int itrk, int tpc){
//**************************************************

  double CT = GetTreeTools().GetTpcCT(itrk,tpc);

  if (!IsEnabled("CT_corr"))
    return CT;
 
  double deltaCT=0;
  double deltaCT_err=0;
  
  int index = GetCTCorrection(GetTreeTools().GetRun(),GetTreeTools().GetRun(),tpc,deltaCT, deltaCT_err);

  if (index==-1) return CT;
 
  CT /= deltaCT;

  if (IsEnabled("tpcpid_syst")){
    if      (tpc==0)
      CT /= GetParameter("CT_corr_tpc1", index, GetAnalysisIndex());
    else if (tpc==1)
      CT /= GetParameter("CT_corr_tpc2", index, GetAnalysisIndex());
    else if (tpc==2)
      CT /= GetParameter("CT_corr_tpc3", index, GetAnalysisIndex());
  }

  return CT;

}


//**************************************************
double SystematicsAnalysis::GetTpcCTexppi(int itrk, int tpc){
//**************************************************

  if (!IsEnabled("CTexp_corr") ) return GetTreeTools().GetTpcCTexppi(itrk,tpc);

  if (!_pull_computed) RecalculatePullQuantities(itrk,tpc);

  return _CTexp[2]*1.02;


}

//**************************************************
double SystematicsAnalysis::GetTpcCTexpp(int itrk, int tpc){
//**************************************************

  if (!IsEnabled("CTexp_corr") ) return GetTreeTools().GetTpcCTexpp(itrk,tpc);

  if (!_pull_computed) RecalculatePullQuantities(itrk,tpc);

  return _CTexp[4]*1.02;

}

//**************************************************
double SystematicsAnalysis::GetTpcCTexpele(int itrk, int tpc){
//**************************************************

  if (!IsEnabled("CTexp_corr") ) return GetTreeTools().GetTpcCTexpele(itrk,tpc);

  if (!_pull_computed) RecalculatePullQuantities(itrk,tpc);

  return _CTexp[0]*1.02;

}



//**************************************************
double SystematicsAnalysis::GetTpcCTexpmu(int itrk, int tpc){
//**************************************************

  if (!IsEnabled("CTexp_corr") && !IsEnabled("mom_scale_pull")) return GetTreeTools().GetTpcCTexpmu(itrk,tpc);

  if (!_pull_computed) RecalculatePullQuantities(itrk,tpc);

  if (IsEnabled("CTexp_corr"))
    return _CTexp[1]*1.02;
  else
    return _CTexp[1];

}

//**************************************************
double SystematicsAnalysis::GetTpcCTerrmu(int itrk, int tpc){
//**************************************************

  if (!IsEnabled("mom_scale_pull")) return GetTreeTools().GetTpcCTerrmu(itrk,tpc);

  if (!_pull_computed) RecalculatePullQuantities(itrk,tpc);

  return _CTerr[1];

}

//************************************************************
double SystematicsAnalysis::ExpecteddEdx(double bg){
//************************************************************

    
  double beta=sqrt((bg*bg)/(1.+bg*bg));

  double func=6.047-pow(beta,2.308)-log(0.00064+1./pow(bg,1.359));

  func=func*785./pow(beta,2.308);
  return func;
}

//************************************************************
double SystematicsAnalysis::TrackMomError(double momentum, double momerr, double Mparticle){
//************************************************************

   double bgless, bgmore, dedxless, dedxmore;
   double epsilon = 2;

  if(momentum<6)bgless=1;///protect bgless from being negative
  else bgless = (momentum-epsilon)/Mparticle;

  bgmore = (momentum+epsilon)/Mparticle;
  dedxless = ExpecteddEdx(bgless)*0.082*30./40.;
  dedxmore = ExpecteddEdx(bgmore)*0.082*30./40.;

  
  return (dedxless-dedxmore)/(bgless-bgmore)*momerr/Mparticle;

}


//************************************************************
bool SystematicsAnalysis::RecalculatePullQuantities(int itrk, int tpc){
//************************************************************
  ////compute and apply corrections to the sigma

  int nhits         = GetTreeTools().GetTpcNHits(itrk, tpc);
  double mom        = GetTpcMomentumForPull(itrk,tpc);
  double mom_err    = GetTreeTools().GetTpcMomentumError(itrk,tpc);

  
  //SLmean ~0.8
  
  double Csigmasamplelength=1.173+0.8*(-0.1513);
  double Csigmarow=pow((double)nhits/72,(double)(-0.5718));

  double sigmade=370*Csigmarow*Csigmasamplelength*0.082;

  double MIPmuon = ExpecteddEdx(4.5)*0.082*30./40.;
  double Mparticle=0;
  
  for(int i=0;i<5;i++){
    if(i==0)
      Mparticle=0.511;
    else if(i==1)
      Mparticle=105.66;
    else if(i==2)
      Mparticle=139.57;
    else if(i==3)
      Mparticle=493.67;
    else if(i==4)
      Mparticle=938.27;
    
    double bg=mom/Mparticle;
    
    //expected energy loss and sigma for different particles
    _CTexp[i]= ExpecteddEdx(bg);
    
    _CTexp[i]= _CTexp[i]*0.082*30./40.;//The PID parameterization was done assuming 30 ns of sampling time 
    
    ///small correction to the dE/dx of the electrons as in the data it's different from the expected Geant4
    //    if(i==0)
    //      _CTexp[i]=_CTexp[i]*0.943; ///Tuned with Cosmics
    
    double sigmamom=TrackMomError(mom,mom_err,Mparticle);
    
    double sigmadedx=sigmade*sqrt(_CTexp[i]/MIPmuon);
    if(_CTexp[i]<0)sigmadedx=sigmade;
    
    _CTerr[i]=sqrt(pow(sigmadedx,2)+pow(sigmamom,2));
    if(sigmamom!=sigmamom) _CTerr[i]=sigmadedx;
    
  }
  
  return true;

}


//************************************************************
bool SystematicsAnalysis::ReadCTCorrections(const std::string& filename){
//************************************************************

  std::cout << " CT correction data " << filename << std::endl; 

  FILE *pFile = fopen (filename.c_str(),"r");

  if( pFile == NULL ) {
    printf("Cannot open file.\n");
    exit(1);
  }

  int run; 
  int subrun; 
  float c[3];   // CT_expected -CT_measured
  float ec[3];  // error on CT_expected - CT_measured
  
  while ( fscanf(pFile,"%d%d%f%f%f%f%f%f",&run, &subrun, &c[0], &ec[0], &c[1], &ec[1], &c[2], &ec[2]) == 8 ){
  std::pair<int,int> comb;
  comb = make_pair(run,subrun);

    if (_CT_correction[0].find(comb)!=_CT_correction[0].end()) continue;
    for (int tpc=0;tpc<3;tpc++)
      _CT_correction[tpc][comb] = std::pair<double,double>(c[tpc],ec[tpc]); 
  }

  fclose(pFile); 

  return true;
}

//************************************************************
int SystematicsAnalysis::GetCTCorrection(int run, int subrun, int tpc, double& deltaCT, double& deltaCT_error) {
//************************************************************

  if (run==0) return false;

  int run0=0;
  int run0_prev=0;
  int subrun0=0;
  int subrun0_prev=0;
  int index = -1;

  std::map<std::pair<int,int>,std::pair<double,double> >::iterator it;
  for (it=_CT_correction[tpc].begin();it!=_CT_correction[tpc].end();it++,index++){
     float check = (float)(run+subrun/200.);
     float checknow = (float)(it->first.first+it->first.second/200.);
    if (check < checknow){
      run0 = run0_prev;
      subrun0 = subrun0_prev;
      break;
    }
    else {
      run0=7752;
      subrun0=6;
    }
    subrun0_prev = it->first.second;
    run0_prev = it->first.first;
  }

  std::pair<int,int> comb0;
  comb0 = make_pair(run0,subrun0);
  deltaCT = _CT_correction[tpc][comb0].first;
  deltaCT_error = _CT_correction[tpc][comb0].second;

  return index;
}


