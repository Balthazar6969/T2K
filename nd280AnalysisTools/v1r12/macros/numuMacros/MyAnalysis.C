#define MyAnalysis_cxx
#include "MyAnalysis.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TVector3.h>
#include <TLegend.h>
#include <stdio.h>
#include <iostream>



//********************************************************************
MyAnalysis::MyAnalysis(const std::string& file):AnalysisBase(file){
//********************************************************************
  
}


//********************************************************************
void MyAnalysis::Initialize(){
//********************************************************************

  AnalysisBase::Initialize();


  // -------- Add variables to the analysis tree ----------------------

  //--- event variables -------
  AddVar("evt",    "I");
  AddVar("run",    "I");
  AddVar("subrun", "I");
  AddVar("ntracks","I");

  // --- muon candidate kinematics -----
  AddAnalysisVar("mu_mom",      "D");
  AddVar("mu_costheta", "D");
  AddVar("mu_mom_tpc",  "D");
  AddVar("mu_sigmap_p", "D");

  // --- muon candidate topology ---------
  AddVar("mu_vfgd",             "I");  
  AddVar("mu_ntpcs",            "I");
  AddVar("mu_nfgds",            "I");
  AddVar("mu_nhitstpc",         "I");

  //--- cut flags ----------------------

  // accumulated cut level
  AddAnalysisVar("accum_level",       "I");

  // cut level that each event passes
  AddAnalysisVar("cut1",            "I");
  AddAnalysisVar("cut2",            "I");
  AddAnalysisVar("cut3",            "I");
  AddAnalysisVar("cut4",            "I");
  AddAnalysisVar("cut5",            "I");
  AddAnalysisVar("cut6",            "I");
  AddAnalysisVar("cut7",            "I");
 
}

//********************************************************************
void MyAnalysis::Finalize(){
//********************************************************************

  AnalysisBase::Finalize();

}

//********************************************************************
void MyAnalysis::InitializeEvent(){
//********************************************************************

  AnalysisBase::InitializeEvent();

  InitializeAnalysisVar("accum_level",0);
  InitializeAnalysisVar("cut1",0);
  InitializeAnalysisVar("cut2",0);
  InitializeAnalysisVar("cut3",0);
  InitializeAnalysisVar("cut4",0);
  InitializeAnalysisVar("cut5",0);
  InitializeAnalysisVar("cut6",0);
  InitializeAnalysisVar("cut7",0);

}

//********************************************************************
void MyAnalysis::FinalizeEvent(){
//********************************************************************

}

//********************************************************************
void MyAnalysis::FinalizeConfiguration(){
//********************************************************************

  // Fill all tree variables that are common to all analyses in a given configuration


  // Only fill the tree for tracks that pass the fiducial cut
  if (GetAccumCutLevel()<4){
    AnalysisBase::FinalizeEvent();
    return;
  }

  FillVar("run",    GetTreeTools().GetRun());
  FillVar("subrun", GetTreeTools().GetSubrun());
  FillVar("evt",    GetTreeTools().GetEvent());
  FillVar("ntracks",GetTreeTools().GetNTracksWithTPC());

  if (_HMN_track!=-1){
    FillVar("mu_costheta",    GetTreeTools().GetDirection(_HMN_track)[2]);
    FillVar("mu_ntpcs",       GetNSegmentsInDet(_HMN_track,"TPC"));
    FillVar("mu_nfgds",       GetNSegmentsInDet(_HMN_track,"FGD"));
       
    int tpc= GetClosestTPC(_HMN_track);
    if (tpc>-1){      
      FillVar("mu_mom_tpc",    GetTreeTools().GetTpcMomentum(_HMN_track,tpc));      
      FillVar("mu_nhitstpc",   GetTreeTools().GetTpcNHits(_HMN_track,tpc));
      FillVar("mu_sigmap_p",   GetTreeTools().GetMomentumRelErrorLocal(_HMN_track,tpc));
    }
    
  }
  if (FiducialCut("FGD1",GetTreeTools().GetPositionStart(_HM_track)))
    FillVar("mu_vfgd", 1);
  else if (FiducialCut("FGD2",GetTreeTools().GetPositionStart(_HM_track)))
    FillVar("mu_vfgd", 2);
  else 
    FillVar("mu_vfgd", 0);



  // Fill the tree
  FillTree(_HMN_TrackCategory);


  AnalysisBase::FinalizeConfiguration();

}

//********************************************************************
void MyAnalysis::FinalizeAnalysis(){
//********************************************************************

  // Fill all tree variables that are analysis dependent

  FillAnalysisVar("accum_level", GetAccumCutLevel());
  FillAnalysisVar("cut1",  GetCutPassed(0));
  FillAnalysisVar("cut2",  GetCutPassed(1));
  FillAnalysisVar("cut3",  GetCutPassed(2));
  FillAnalysisVar("cut4",  GetCutPassed(3));
  FillAnalysisVar("cut5",  GetCutPassed(4));
  FillAnalysisVar("cut6",  GetCutPassed(5));
  FillAnalysisVar("cut7",  GetCutPassed(6));

  if (_HMN_track!=-1)
    FillAnalysisVar("mu_mom",      GetMomentum(_HMN_track));


  AnalysisBase::FinalizeAnalysis();

}

//********************************************************************
bool MyAnalysis::Process(){
//********************************************************************

  //------ This function is called for each event  ----------

  SystematicsAnalysis::Process();
   
  ///////////////////////////////////////////////////////////////////////
  // Here is the list of selection cuts
  ///////////////////////////////////////////////////////////////////////


  // ----- Inclusive CC -----------

  // 0. Good spill
  if ((GetTreeTools().GetIsGoodSpill() && GetTreeTools().GetHasGoodDQ() ) || GetTreeTools().GetIsMC()){
    CutPassed(0);
  }  
  else
    return true;

  // 1. Multiplicity
  if (GetTreeTools().GetNTracksWithTPC()>0){
    CutPassed(1);
  }
  else
    return true;

  // 2. Find at least one good quality track  
  if(FindLeadingTracks(_HM_track,_HMN_track,_HMP_track)){
    CutPassed(2);
  }
  else 
    return true;

  // Fill the track categories for color drawing
  FillCategories(_HM_track,  _HM_TrackCategory);
  FillCategories(_HMN_track, _HMN_TrackCategory);
  FillCategories(_HMP_track, _HMP_TrackCategory);
  
  // 3. Fiducial cut
  if (_HM_track!=-1){
    if (FiducialCut("FGD",GetTreeTools().GetPositionStart(_HM_track))){
      CutPassed(3);
    }  
    else
      return true;
  }  

  // 4. Find at least one negative track with good quality
  if (_HMN_track!=-1){
    CutPassed(4);

    // 5. Melody's external veto cut
    if (ExternalVetoCut(_HMN_track)){
      CutPassed(5);
    }

    // 6. Muon PID cut
    if (MuonPIDCutLike(_HMN_track)){
      CutPassed(6);
    }

  }

  return true;      
}


