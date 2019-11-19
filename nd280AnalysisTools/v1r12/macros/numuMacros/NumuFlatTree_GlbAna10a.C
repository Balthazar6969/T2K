//*******************************************
// What's in here:
//
//  Mandatory methods for all modules: 
//  1) DefineTree                       
//  2) ResetSpillVariables     
//  3) ResetBunchVariables     
//
//  4) FillTPCTracksCounters 
//  5) FillBunchVariablesFromPIDs 
//
//  Other methods
//  6) ApplyPIDCorrections
//  7) ComputePIDProbabilities  
//  8) PrepareTPCTracksCounters
//  9) FillTwoTracksVariables
//********************************************

#include "NumuFlatTree_GlbAna10a.h"


NumuFlatTree_GlbAna10a::NumuFlatTree_GlbAna10a(NumuFlatTree_Standard *std_mod1){
  cout <<"Defining Global Analysis 2010a class (passing Standard class)" << endl;
  std_mod=std_mod1;
}


void NumuFlatTree_GlbAna10a::DefineTree(){

  // ---------------Define Spill variables (g10a) -------------------
  
  // ---------------Define Bunch variables (g10aB) -------------------
  
  // Corrctions on PID and compute prob out of PID
  p_numutree->Branch("g10aB_trCTCorr",  _g10aB_trCTCorr, "g10aB_ctc[stdB_NTracksWithTPC][3]/D");
  p_numutree->Branch("g10aB_trPullCorrmu",  _g10aB_trPullCorr[0], "g10aB_trPullcmu[stdB_NTracksWithTPC][3]/D");
  p_numutree->Branch("g10aB_trPullCorrele",  _g10aB_trPullCorr[1], "g10aB_trPullcele[stdB_NTracksWithTPC][3]/D");
  p_numutree->Branch("g10aB_trPullCorrp",  _g10aB_trPullCorr[2], "g10aB_trPullcp[stdB_NTracksWithTPC][3]/D");
  p_numutree->Branch("g10aB_trPullCorrpi",  _g10aB_trPullCorr[3], "g10aB_trPullcpi[stdB_NTracksWithTPC][3]/D");
  p_numutree->Branch("g10aB_trPullCorrk",  _g10aB_trPullCorr[4], "g10aB_trPullck[stdB_NTracksWithTPC][3]/D");
  p_numutree->Branch("g10aB_trProbmu",  _g10aB_trProb[0], "g10a_trProbmu[stdB_NTracksWithTPC]/D");
  p_numutree->Branch("g10aB_trProbele",  _g10aB_trProb[1], "g10a_trProbele[stdB_NTracksWithTPC]/D");
  p_numutree->Branch("g10aB_trProbp",  _g10aB_trProb[2], "g10a_trProbp[stdB_NTracksWithTPC]/D");
  p_numutree->Branch("g10aB_trProbpi",  _g10aB_trProb[3], "g10a_trProbpi[stdB_NTracksWithTPC]/D");
  p_numutree->Branch("g10aB_trProbk",  _g10aB_trProb[4], "g10a_trProbk[stdB_NTracksWithTPC]/D");
  p_numutree->Branch("g10aB_trCTexpCorrmu",  _g10aB_trCTexpCorr[0], "g10aB_ctexpCorrmu[stdB_NTracksWithTPC][3]/D");
  p_numutree->Branch("g10aB_trCTexpCorrele",  _g10aB_trCTexpCorr[1], "g10aB_ctexpCorrele[stdB_NTracksWithTPC][3]/D");
  p_numutree->Branch("g10aB_trCTexpCorrp",  _g10aB_trCTexpCorr[2], "g10aB_ctexpCorrp[stdB_NTracksWithTPC][3]/D");
  p_numutree->Branch("g10aB_trCTexpCorrpi",  _g10aB_trCTexpCorr[3], "g10aB_ctexpCorrpi[stdB_NTracksWithTPC][3]/D");
  p_numutree->Branch("g10aB_trCTexpCorrk",  _g10aB_trCTexpCorr[4], "g10aB_ctexpCorrk[stdB_NTracksWithTPC][3]/D");
  
  p_numutree->Branch("g10aB_NTPCTracksCut", _g10aB_NTPCTracksCut, "g10aB_NTPCTracksCut[3]/I");
  p_numutree->Branch("g10aB_NTPCTracksPosCut", _g10aB_NTPCTracksPosCut, "g10aB_NTPCTracksPosCut[3]/I");
  p_numutree->Branch("g10aB_NTPCTracksNegCut", _g10aB_NTPCTracksNegCut, "g10aB_NTPCTracksNegCut[3]/I");

  // Variable Kinematics:
  // Old way to store kinematics variable (should be removed once new way is validated)                                                        
  p_numutree->Branch("g10aB_ENuRec",     _g10aB_ENuRec,          "g10aB_ENuRec[2]/D");
  p_numutree->Branch("g10aB_EMuRec",        _g10aB_EMuRec,     "g10aB_EMuRec[2]/D");
  p_numutree->Branch("g10aB_PQ2",         _g10aB_PQ2,      "g10aB_PQ2[2]/D");
  p_numutree->Branch("g10aB_MuQ2",        _g10aB_MuQ2,      "g10aB_MuQ2[2]/D");
  p_numutree->Branch("g10aB_DeltaThetaP",_g10aB_DeltaThetaP,     "g10aB_DeltaThetaP[2]/D");
  p_numutree->Branch("g10aB_EProton",         &_g10aB_EProton,      "g10aB_EProton/D");
  p_numutree->Branch("g10aB_FinalCosTheta",  &_g10aB_FinalCosTheta,     "g10aB_FinalCosTheta/D");
  p_numutree->Branch("g10aB_FinalMass",         &_g10aB_FinalMass,      "g10aB_FinalMass/D");
  p_numutree->Branch("g10aB_FinalEnergy",         &_g10aB_FinalEnergy,      "g10aB_FinalEnergy/D");
  p_numutree->Branch("g10aB_FinalMom",         &_g10aB_FinalMom,      "g10aB_FinalMom/D");  

}

void NumuFlatTree_GlbAna10a::SetBranchAddress(TTree *p_numutree){

  // ---------------Define Spill variables (g10a) -------------------
  
  // ---------------Define Bunch variables (g10aB) -------------------
  
  // Corrctions on PID and compute prob out of PID
  p_numutree->SetBranchAddress("g10aB_trCTCorr",  _g10aB_trCTCorr);
  p_numutree->SetBranchAddress("g10aB_trPullCorrmu",  _g10aB_trPullCorr[0]);
  p_numutree->SetBranchAddress("g10aB_trPullCorrele",  _g10aB_trPullCorr[1]);
  p_numutree->SetBranchAddress("g10aB_trPullCorrp",  _g10aB_trPullCorr[2]); 
  p_numutree->SetBranchAddress("g10aB_trPullCorrpi",  _g10aB_trPullCorr[3]); 
  p_numutree->SetBranchAddress("g10aB_trPullCorrk",  _g10aB_trPullCorr[4]); 
  p_numutree->SetBranchAddress("g10aB_trProbmu",  _g10aB_trProb[0]); 
  p_numutree->SetBranchAddress("g10aB_trProbele",  _g10aB_trProb[1]); 
  p_numutree->SetBranchAddress("g10aB_trProbp",  _g10aB_trProb[2]); 
  p_numutree->SetBranchAddress("g10aB_trProbpi",  _g10aB_trProb[3]);
  p_numutree->SetBranchAddress("g10aB_trProbk",  _g10aB_trProb[4]); 
  p_numutree->SetBranchAddress("g10aB_trCTexpCorrmu",  _g10aB_trCTexpCorr[0]);
  p_numutree->SetBranchAddress("g10aB_trCTexpCorrele",  _g10aB_trCTexpCorr[1]); 
  p_numutree->SetBranchAddress("g10aB_trCTexpCorrp",  _g10aB_trCTexpCorr[2]); 
  p_numutree->SetBranchAddress("g10aB_trCTexpCorrpi",  _g10aB_trCTexpCorr[3]);
  p_numutree->SetBranchAddress("g10aB_trCTexpCorrk",  _g10aB_trCTexpCorr[4]); 
  
  p_numutree->SetBranchAddress("g10aB_NTPCTracksCut", _g10aB_NTPCTracksCut); 
  p_numutree->SetBranchAddress("g10aB_NTPCTracksPosCut", _g10aB_NTPCTracksPosCut); 
  p_numutree->SetBranchAddress("g10aB_NTPCTracksNegCut", _g10aB_NTPCTracksNegCut); 

  // Variable Kinematics:
  // Old way to store kinematics variable (should be removed once new way is validated)                                                        
  p_numutree->SetBranchAddress("g10aB_ENuRec",     _g10aB_ENuRec);        
  p_numutree->SetBranchAddress("g10aB_EMuRec",        _g10aB_EMuRec);     
  p_numutree->SetBranchAddress("g10aB_PQ2",         _g10aB_PQ2);     
  p_numutree->SetBranchAddress("g10aB_MuQ2",        _g10aB_MuQ2);    
  p_numutree->SetBranchAddress("g10aB_DeltaThetaP",_g10aB_DeltaThetaP); 
  p_numutree->SetBranchAddress("g10aB_EProton",         &_g10aB_EProton); 
  p_numutree->SetBranchAddress("g10aB_FinalCosTheta",  &_g10aB_FinalCosTheta); 
  p_numutree->SetBranchAddress("g10aB_FinalMass",         &_g10aB_FinalMass);  
  p_numutree->SetBranchAddress("g10aB_FinalEnergy",         &_g10aB_FinalEnergy); 
  p_numutree->SetBranchAddress("g10aB_FinalMom",         &_g10aB_FinalMom);    

}



//Sets all class GlbAna10a spill variables to default values             
//*************************************************************     
void NumuFlatTree_GlbAna10a::ResetSpillVariables(){
  //*************************************************************       
  
  // Set momentum sort to zero:                                         
  for (int jj= 0; jj < p_NBUNCH; jj ++){
    g10a_global_index_usort[jj].clear();
    g10a_momentum_usort[jj].clear(); //unsorted momentum                                                                                     
  }

}

//Sets all class GlbAna10a bunch variables to default values             
//*************************************************************     
void NumuFlatTree_GlbAna10a::ResetBunchVariables(){
  //*************************************************************       
  
  
  for(int i = 0; i < p_NMAXTRACKS; i ++){
    for(int k = 0; k < 3; k ++){
      _g10aB_trCTCorr[i][k] = 0;
      
      for(int j = 0; j < p_NHYPO; j ++){
	_g10aB_trPullCorr[j][i][k] = 0;
	_g10aB_trCTexpCorr[j][i][k] = 0;
      }
    }
    for(int j = 0; j < p_NHYPO; j ++) _g10aB_trProb[j][i] = 0;   

  }
  
  for(int c = 0; c < 4; c ++){
    g10a_trPosStart[c] = 0;
    g10a_trPosEnd[c] = 0;
  }
  
  for(int i = 0; i < p_NOPTIONS; i ++){   
    _g10aB_ENuRec[i] = 0;                 
    _g10aB_DeltaThetaP[i] = 0;            
    _g10aB_EMuRec[i] = 0;                 
    _g10aB_PQ2[i] = 0;                    
    _g10aB_MuQ2[i] = 0;                   
  }          
  
  _g10aB_EProton = 0;
  _g10aB_FinalCosTheta = -2;
  _g10aB_FinalMass = 0;
  _g10aB_FinalEnergy = 0;
  _g10aB_FinalMom = 0;
}
  // Fill spill variables
//******************************

void NumuFlatTree_GlbAna10a::FillTPCTracksCounters(){
  
    // nothing yet

  }

//Fill bunch variables:
//******************************************
void NumuFlatTree_GlbAna10a::FillBunchVariablesFromPIDs(int ibunch){

  // move that to a method
  for(int k = 0; k < 3; k ++){
    _g10aB_NTPCTracksCut[ibunch][k]=g10aB_NTPCTracksCut[ibunch][k];
    _g10aB_NTPCTracksPosCut[ibunch][k]=g10aB_NTPCTracksPosCut[ibunch][k];
    _g10aB_NTPCTracksNegCut[ibunch][k]=g10aB_NTPCTracksNegCut[ibunch][k];
  }  


  // Get info from oaAnalysis global recon:
  ND::TGlobalReconModule::TGlobalPID tracks_bunch[p_NMAXTRACKS];
  bool tracks_bunch_exist[p_NMAXTRACKS];
  
  for(int i = 0; i < p_NMAXTRACKS; i ++){
    tracks_bunch_exist[i] = false;
  }

  //sort all the tracks by momentum 
  //--------------------------------------------------------------------
  vector<double> momentum; //momentum in falling order
  vector<int> index; //index of momentum order
  //Copy vector and sort in momentum
  for(int i = 0; i < std_mod->std_NRecTrack[ibunch]; i ++){
    momentum.push_back(g10a_momentum_usort[ibunch][i]);
  }
  //Sort according to increasing momentum
  sort(momentum.begin(),momentum.end());
  //Mapping of index for decreasing momentum
  for(int i = 0; i < std_mod->std_NRecTrack[ibunch]; i ++){
    for(int ii = 0; ii < std_mod->std_NRecTrack[ibunch]; ii ++){
      if(momentum[std_mod->std_NRecTrack[ibunch] - 1 - i] == g10a_momentum_usort[ibunch][ii]){
	index.push_back(ii);
      }
    }
  }      
  
  
  for(int j = 0; j < std_mod->std_NRecTrack[ibunch]; j ++){
    if(j == p_NMAXTRACKS) break;
    int it = index[j];
    int index_pos = g10a_global_index_usort[ibunch][it];
    ND::TGlobalReconModule::TGlobalPID *gTrack = (ND::TGlobalReconModule::TGlobalPID*)(*p_PIDs)[index_pos];
    tracks_bunch[j] = *gTrack;
    tracks_bunch_exist[j] = true;
  }
  
  // end momentum sorting 
  //-------------------------------------------------------------------


  //Store variables I'll need for kinematics
  for(int itrk = 0; itrk < std_mod->std_NRecTrack[ibunch]; itrk ++){
    if(itrk == p_NMAXTRACKS) break;

    g10a_trMom[itrk] = tracks_bunch[itrk].Momentum;
    g10a_trCharge[itrk] = tracks_bunch[itrk].Charge;
    
    

    utils->FillPosDirAtVertex(itrk,tracks_bunch[itrk].BackPosition,
			      tracks_bunch[itrk].FrontPosition,
			      tracks_bunch[itrk].BackDirection,
			      tracks_bunch[itrk].FrontDirection,
			      tracks_bunch[itrk].isForward,
			      g10a_trPosStart,
			      g10a_trPosEnd,
			      g10a_trDir[itrk],
			      g10a_trCosTheta[itrk]);
    
  }
  FillTwoTracksVariables(tracks_bunch_exist[0], tracks_bunch_exist[1]);
  
  // apply corrections to get centered pulls                                                                                             
  ApplyPIDCorrections(p_RunID);                                                                                                      
  
  // convert pulls into probabilities                                                                                                    
  ComputePIDProbabilities();                                                                                                       
  
}


//Method that applies PID corrections
//**********************************************************
void NumuFlatTree_GlbAna10a::ApplyPIDCorrections(int run){

  double CTexp_corr_data = 0.9060;
  double CT_corr_MC = 1.0025;
  double smearing_sigma = 20.55;
  
  double CT_corr, CTexp_corr;
  
  double mean_data_all       = -0.040;//TpcNHits>40&&CTc>0&&TpcMom<2000&&TpcMom>200, negative, fit in [-2,2]
  double sigma_data_all      = 1.105;
  double mean_data_all_elec  = -0.562;//TpcNHits>20&&CTc<900&&TpcMom<80, all charges, fit in [-1.5,0.5]-not used
  double sigma_data_all_elec = 1.054;//TpcNHits>20&&CTc<900&&TpcMom<80, all charges, fit in [-1.5,0.5]-not used
  
  double mean_MC_all       = 0.00;//as it should be - no good reason to have this
  double sigma_MC_all      = 1.08;//this is still contested - will leave it in
  double mean_MC_all_elec  = -0.214;//no need to have this - it's small and very uncertain
  double sigma_MC_all_elec = 0.98;//we inflate/smear them, anyway!
  double ElePull_correction_MC_muons = 1.525;//how to move the true mu in MC to match data
  
  //Symmetric fits
  double mean_data[p_NRUNS][3] = {{ 0.346, 0.067, 0.264},
				    { 0.285, 0.108, 0.256},
				    { 0.363,-0.318,-0.026},
				    { 0.036,-0.569,-0.296}};
  
  double sigma_data[p_NRUNS][3] = {{ 1.024, 1.029, 1.030},
				     { 1.084, 1.035, 1.047},
				     { 1.095, 1.062, 1.038},
				     { 1.041, 1.030, 1.037}};
  
  double mean  = mean_MC_all;
  double sigma = sigma_data_all;
  
  int irun = 0; 
  
  if(!p_isMC) irun = utils->GetBeamRun(run);
  
  if(p_isMC){
    CT_corr = CT_corr_MC;
    CTexp_corr = 1;
  } 
  else{
    CT_corr = 1;
    CTexp_corr = CTexp_corr_data;
  }
  
  //Loop over the tracks

  for(int itr = 0; itr < std_mod->_stdB_NTracksWithTPC; itr ++){
    if(itr == p_NMAXTRACKS) break;
    
    //Loop over the TPCs    
    for(int itpc = 0; itpc < 3; itpc ++){
      
      if(std_mod->_stdB_trCT[itr][itpc] > 0 && std_mod->_stdB_trCT[itr][itpc] < 5e4){
	_g10aB_trCTCorr[itr][itpc] = std_mod->_stdB_trCT[itr][itpc]*CT_corr;
	if(p_isMC) _g10aB_trCTCorr[itr][itpc] += p_fRandomGenerator.Gaus(0, smearing_sigma);
      }
      else _g10aB_trCTCorr[itr][itpc] = -999.0;

      //Loop over hypotheses
      for(int h = 0; h < p_NHYPO; h ++){

	//MC
	if(p_isMC){
	  if(h != 1){
	    mean = mean_MC_all;
	    sigma = sigma_MC_all;
	  }
	  // for electrons
	  else{
	    if(abs(std_mod->_stdB_trTruePDG[0]) == 13)
	      mean = mean_MC_all_elec - ElePull_correction_MC_muons;
	    else{
	      mean = mean_MC_all_elec;
	      sigma = sigma_MC_all_elec;
	    }
	  }
	}

	//Data
	else{
	  mean = mean_data[irun][itpc];
	  sigma = sigma_data[irun][itpc];
	  if(h != 1) CTexp_corr = CTexp_corr_data;
	  else CTexp_corr = CTexp_corr_data*0.959;
	}	

	if(_g10aB_trCTCorr[itr][itpc] > 0 && _g10aB_trCTCorr[itr][itpc] < 5e4){
	  _g10aB_trCTexpCorr[h][itr][itpc] = std_mod->_stdB_trCTexp[h][itr][itpc]*CTexp_corr;
	  _g10aB_trPullCorr[h][itr][itpc] = ((_g10aB_trCTCorr[itr][itpc] - _g10aB_trCTexpCorr[h][itr][itpc])/(std_mod->_stdB_trCTerr[h][itr][itpc]) - mean)/sigma;
	}
	else{
	  _g10aB_trPullCorr[h][itr][itpc] = -999.0;
	  _g10aB_trCTexpCorr[h][itr][itpc] = std_mod->_stdB_trCTexp[h][itr][itpc]; // Make sure it's ok
	}
      }//End loop over hypotheses
    }//End loop over TPCs
  }//End loop over tracks
}



//**********************************************************
//Computes PID probabilities
//**********************************************************
void NumuFlatTree_GlbAna10a::ComputePIDProbabilities(){

  double mean  = 0;
  double sigma = 1;

  vector<double> sum;
  
  //Loop over the tracks
  for(int itr = 0; itr < std_mod->_stdB_NTracksWithTPC; itr ++){
    
    sum.push_back(0);
    
    if(itr == p_NMAXTRACKS) break;
    
    //Loop over hypotheses
    for(int h = 0; h < p_NHYPO; h ++){
      _g10aB_trProb[h][itr] = 1;
      if(std_mod->_stdB_NTracksWithTPC > 1) _g10aB_trProb[h][itr] = 1;
      for(int itpc = 0; itpc < 3; itpc ++){
	if(_g10aB_trPullCorr[h][itr][itpc] != 0 && abs(_g10aB_trPullCorr[h][itr][itpc]) < 100){
	  _g10aB_trProb[h][itr] *= exp(-0.5*pow((_g10aB_trPullCorr[h][itr][itpc] - mean)/sigma, 2));
	}
      }      
      sum[itr] += _g10aB_trProb[h][itr];
    }//End loop over hypotheses
  }//End loop over tracks  

  //Normalize  
  for(int itr = 0; itr < std_mod->_stdB_NTracksWithTPC; itr ++){
      if(itr == p_NMAXTRACKS) break;
      for(int h = 0; h < p_NHYPO; h ++){
          if(sum[itr] != 0) _g10aB_trProb[h][itr] /= sum[itr];
      }    
  }
}

// Initialize internal variables:
//********************************************
void NumuFlatTree_GlbAna10a::InitializeInternalVariables(){
  
  for(int j = 0; j < p_NBUNCH; j ++){
    for(int k = 0; k < 3; k ++){
      
      g10aB_NTPCTracksCut[j][k]=0;
      g10aB_NTPCTracksPosCut[j][k]=0;
      g10aB_NTPCTracksNegCut[j][k]=0;
    }
  }
  
}

//*************************************************************
// Keep counter of how many tracks with a TPC segment pass the following cuts:
// 1) no cuts
// 2) Sum of hits in all TPC be greater than 18.
// 3) Cuts on pulls (not implemented yet)
// For all tracks and negative and positive tracks
// Note this still need to be checked... 

void NumuFlatTree_GlbAna10a::PrepareTPCTracksCounters(int ibunch,int ipid){
  
  // Copy the current pid to hTrack                                        
  ND::TGlobalReconModule::TGlobalPID *hTrack = (ND::TGlobalReconModule::TGlobalPID*)(*p_PIDs)[ipid];   
  if(!hTrack->NTPCs) return;
  
  //Do sorting according to momentum                                                                                                         
  g10a_global_index_usort[ibunch].push_back(ipid);
  g10a_momentum_usort[ibunch].push_back(hTrack->Momentum);

  int nhits[3] = {0};
  int nhitstotal=0;
  for(int jj = 0; jj < hTrack->NTPCs; jj ++){
    ND::TGlobalReconModule::TTPCObject *tpcTrack=(ND::TGlobalReconModule::TTPCObject*)(*(hTrack->TPC))[jj];
    if(!tpcTrack) return;
    
    // Check the number of hits in TPC tracks and sum them 
    nhits[jj] = tpcTrack->NHits;
    nhitstotal += tpcTrack->NHits;
    
    // Check the quality of the cut: hard if I want to apply it to corrected pull...
    // Note trivial right now!!! Need to reorganize... see p.107 logbook
  }
  
  // I should initialize the counters somewhere.....
  
  // First fill for all tracks:
  g10aB_NTPCTracksCut[ibunch][0] ++;
  if (nhitstotal > 18) {
    g10aB_NTPCTracksCut[ibunch][1] ++;
  }
  
  // Then fill for positive and negative separatly:    
  
  if(hTrack->Charge > 0){
    g10aB_NTPCTracksPosCut[ibunch][0] ++;
    if (nhitstotal > 18) {
      g10aB_NTPCTracksPosCut[ibunch][1] ++;
    }
  }
  else if(hTrack->Charge < 0){
    g10aB_NTPCTracksNegCut[ibunch][0] ++;      
    if (nhitstotal > 18) {
      g10aB_NTPCTracksNegCut[ibunch][1] ++;
    }
  }   
}


//Fills information for two track events (Old method for Kinematics)
//**********************************************************
void NumuFlatTree_GlbAna10a::FillTwoTracksVariables(bool track1, bool track2){ 
  
  double posMom = 0;
  double negMom = 0;
  double Emu = 0;
  double Ep = 0;
  
  //For two tracks
  if(track1 && track2){
    //The 2 most energetic tracks are one positive and one negative 
    if((g10a_trCharge[0] ==  1 && g10a_trCharge[1] == -1) || (g10a_trCharge[0] == -1 && g10a_trCharge[1] == 1)){
      
      TVector3 negDir;
      TVector3 posDir;
      TVector3 posMomVect;
      TVector3 negMomVect;
      double Mf2 = 0;
      double Ef = 0;
      TVector3 finalStateMom(0,0,0); 
      double fMom = 0;
      double fMomZ = 0;
      double cosThetaF = 0;
      int posIndex, negIndex;
	
      if(g10a_trCharge[0] == 1){    
	posIndex = 0;
	negIndex = 1;
      }
      else{
	posIndex = 1;
	negIndex = 0;
      }

      posDir = g10a_trDir[posIndex];
      posMom = g10a_trMom[posIndex];
      posMomVect = posMom * posDir;
      negDir = g10a_trDir[negIndex];
      negMom = g10a_trMom[negIndex];
      negMomVect = negMom * negDir;
	
      Emu = sqrt(pow(p_muMass,2) + pow(g10a_trMom[negIndex],2));
      Ep = sqrt(pow(p_pMass, 2) + pow(g10a_trMom[posIndex],2));
	
      //final state kinematics
      Ef = Emu + Ep;
      finalStateMom = negMomVect + posMomVect;
      fMom = finalStateMom.Mag();
      fMomZ = finalStateMom.Z();
      Mf2 = pow(Ef,2)- pow(fMom,2);

       //we assume the neutrino moving along the z axis
      cosThetaF = fMomZ/fMom;

      _g10aB_EMuRec[0] = Emu;
      _g10aB_EProton = Ep;
      _g10aB_FinalCosTheta = cosThetaF;
      _g10aB_FinalMass = sqrt(Mf2);
      _g10aB_FinalEnergy = Ef;
      _g10aB_FinalMom = fMom;
	
      //neutrino energy
      //---------------
      _g10aB_ENuRec[0] = ( ( (pow(p_pMass,2) - pow(p_muMass,2)) + 2*Emu*(p_nMass-p_NucPot) - pow(p_nMass-p_NucPot,2) )/ (-Emu + (p_nMass-p_NucPot) + (g10a_trMom[negIndex] * g10a_trCosTheta[negIndex])) )/2;
      _g10aB_ENuRec[1] = (Mf2 - p_nMass*p_nMass + 2*p_NucPot*Ef + p_NucPot*p_NucPot) / (2*(p_NucPot + Ef - fMom*cosThetaF));

       _g10aB_MuQ2[0] = (2*_g10aB_ENuRec[0]*(_g10aB_EMuRec[0] - negMomVect.Mag()*g10a_trCosTheta[negIndex]) - pow(p_muMass,2))/1e6; //GeV
      _g10aB_MuQ2[1] = (2*_g10aB_ENuRec[1]*(_g10aB_EMuRec[0] - negMomVect.Mag()*g10a_trCosTheta[negIndex]) - pow(p_muMass,2))/1e6; //GeV

      _g10aB_PQ2[0] = (_g10aB_ENuRec[0]*_g10aB_ENuRec[0] - _g10aB_EProton*_g10aB_EProton + (finalStateMom - posMomVect)*(finalStateMom - posMomVect) + pow(p_nMass-p_NucPot,2)*((2*_g10aB_EProton)/(p_nMass-p_NucPot) -1) + 2*_g10aB_ENuRec[0]*(_g10aB_EProton*g10a_trCosTheta[posIndex] - _g10aB_FinalMom*_g10aB_FinalCosTheta))/1e6;

      _g10aB_PQ2[1] = (_g10aB_ENuRec[1]*_g10aB_ENuRec[1] - _g10aB_EProton*_g10aB_EProton + (finalStateMom - posMomVect)*(finalStateMom - posMomVect) + pow(p_nMass-p_NucPot,2)*((2*_g10aB_EProton)/(p_nMass-p_NucPot) -1) + 2*_g10aB_ENuRec[1]*(_g10aB_EProton*g10a_trCosTheta[posIndex] - _g10aB_FinalMom*_g10aB_FinalCosTheta))/1e6;

      // deltaThetaP
      //------------
      double cosDeltaThetaP1 = (posDir.Z()*_g10aB_ENuRec[0] - posDir*negMomVect) / (sqrt(negMom*negMom + _g10aB_ENuRec[0]*_g10aB_ENuRec[0] - 2*negMomVect.Z()*_g10aB_ENuRec[0]));
      double cosDeltaThetaP2 = (posDir.Z()*_g10aB_ENuRec[1] - posDir*negMomVect) / (sqrt(negMom*negMom + _g10aB_ENuRec[1]*_g10aB_ENuRec[1] - 2*negMomVect.Z()*_g10aB_ENuRec[1]));
      _g10aB_DeltaThetaP[0] = acos(cosDeltaThetaP1);
      _g10aB_DeltaThetaP[1] = acos(cosDeltaThetaP2);
	
      // neutrino invariant mass
      //------------------------
      TLorentzVector proton4vector(posMomVect, Ep);
      TLorentzVector muon4vector(negMomVect, Emu);
      TLorentzVector neutron4vector(0, 0, 0, p_nMass);
	
      TLorentzVector sum = proton4vector + muon4vector - neutron4vector;
      _g10aB_NuInvMass = (sum*sum)*1e-6; //GeV^2
    }
    //End (+,-) or (-,+) case

    //The 2 most energetic tracks are both negative (ie assuming 2 muons?)
    else if(g10a_trCharge[0] == -1 && g10a_trCharge[1] == -1){
      double Emu_0 = sqrt(p_muMass*p_muMass + g10a_trMom[0]*g10a_trMom[0]);
      double Emu_1 = sqrt(p_muMass*p_muMass + g10a_trMom[1]*g10a_trMom[1]);

      _g10aB_EMuRec[0] = Emu_0;
      _g10aB_EMuRec[1] = Emu_1;
      
      _g10aB_ENuRec[0] = ((p_pMass*p_pMass - p_muMass*p_muMass + 2*Emu_0*(p_nMass-p_NucPot) - pow(p_nMass-p_NucPot,2))/(-Emu_0 + (p_nMass-p_NucPot) + (g10a_trMom[0]*g10a_trCosTheta[0])))/2;	
      _g10aB_ENuRec[1] = ((p_pMass*p_pMass - p_muMass*p_muMass + 2*Emu_1*(p_nMass-p_NucPot) - pow(p_nMass-p_NucPot,2))/(-Emu_1 + (p_nMass-p_NucPot) + (g10a_trMom[1]*g10a_trCosTheta[1])))/2;
      
      _g10aB_MuQ2[0] = (2*_g10aB_ENuRec[0]*(_g10aB_EMuRec[0] - g10a_trMom[0]*g10a_trCosTheta[0]) - p_muMass*p_muMass)/1e6; //GeV
      _g10aB_MuQ2[1] = (2*_g10aB_ENuRec[1]*(_g10aB_EMuRec[1] - g10a_trMom[1]*g10a_trCosTheta[1]) - p_muMass*p_muMass)/1e6; //GeV
      
      _g10aB_PQ2[0] = 0;
      _g10aB_PQ2[1] = 0;
    } 
    
    // the 2 most energetic tracks are both positive 
    else{
      _g10aB_ENuRec[0] = 0;
      _g10aB_ENuRec[1] = 0;
      _g10aB_MuQ2[0] = 0;
      _g10aB_MuQ2[1] = 0;
      _g10aB_PQ2[0] = 0;
      _g10aB_PQ2[1] = 0;
    }     
  }//End if two tracks
  
  //If there is only one track 
  else if(track1 && !track2){
    
    //If it's negative
    if(g10a_trCharge[0] == -1){
      
      Emu = sqrt(p_muMass*p_muMass + g10a_trMom[0]*g10a_trMom[0]);
      _g10aB_EMuRec[0] = Emu;
      _g10aB_ENuRec[0] = ((p_pMass*p_pMass - p_muMass*p_muMass + 2*Emu*(p_nMass-p_NucPot) - pow(p_nMass-p_NucPot,2))/(-Emu + (p_nMass-p_NucPot) + (g10a_trMom[0]*g10a_trCosTheta[0])))/2;	
      _g10aB_ENuRec[1] = 0;      
      _g10aB_MuQ2[0] = (2 * _g10aB_ENuRec[0] * (_g10aB_EMuRec[0]- g10a_trMom[0] * g10a_trCosTheta[0]) - p_muMass*p_muMass)/1e6; //GeV
    }
  }
  else if(track2 && !track1){

    //If it's negative
    if(g10a_trCharge[1] == -1){

      Emu = sqrt(p_muMass*p_muMass + g10a_trMom[1]*g10a_trMom[1]);
      _g10aB_EMuRec[0] = Emu;
      _g10aB_ENuRec[0] = ((p_pMass*p_pMass - p_muMass*p_muMass + 2*Emu*(p_nMass-p_NucPot) - pow(p_nMass-p_NucPot,2))/(-Emu + (p_nMass-p_NucPot) + (g10a_trMom[1]*g10a_trCosTheta[1])))/2;	
      _g10aB_ENuRec[1] = 0;
      _g10aB_MuQ2[0] = (2*_g10aB_ENuRec[0]*(_g10aB_EMuRec[0] - g10a_trMom[1]*g10a_trCosTheta[1]) - pow(p_muMass,2))/1e6; //GeV
    }
  }
  //No tracks
  else{
    _g10aB_ENuRec[0] = 0;
    _g10aB_ENuRec[1] = 0;
  }
}


