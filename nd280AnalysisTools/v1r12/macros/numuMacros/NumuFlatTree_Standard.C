
// This class contains the following methods:

// Mandatory methods for all modules:
// 1) DefineTree
// 2) ResetSpillVariables
// 3) ResetBunchVariables

// Other useful methods related to standard module:

// 4) FillOAInfo // Copy variables straight from oaAnlaysis (very basic right now)
// 5) InitializeInternalVariables
// 6) FillVertexMCInfo 
// 7) PrepareVariables
// 8) FillBunchVariables
// 9) FillBunchVariablesFromPIDs

// Other methods:
// 10) PrepareBunchWithMaxMomInSpill
// 11) FillNTracksSpillInfo
// 12) FillKinematicsCCQE
// 13) FillKinematicsFor1Muon
// 14) GetEnuReweight 
// 15) FillDelayedClusters
// 16) FillTrueQ2
// 17) FillDetectorsUsed
#include "NumuFlatTree_Standard.h"

NumuFlatTree_Standard::NumuFlatTree_Standard(NumuFlatTree_GeneralUtils *utils1){
  utils=utils1;
  cout << "Defining class NumuFlatTree_Standard " << endl;
}
  
//Branch definitions for the std variables in the numutree
//*************************************************************
void NumuFlatTree_Standard::DefineTree(){
  
  //----------- filled for each spill (syntax _std_)  -----------
  p_numutree->Branch("std_TrueSpillInfo",   &_std_TrueSpillInfo,   "std_TrueSpillInfo/I");
  p_numutree->Branch("std_Evt",        &_std_Evt,      "std_Evt/I");
  p_numutree->Branch("std_Run",        &_std_Run,      "std_Run/I");
  p_numutree->Branch("std_FileRun",        &_std_FileRun,      "std_FileRun/I");
  p_numutree->Branch("std_FileSubrun",        &_std_FileSubrun,      "std_FileSubrun/I");
  p_numutree->Branch("std_Entry",      &_std_Entry,    "std_Entry/I");
  p_numutree->Branch("std_NBunches",   &_std_NBunches, "std_NBunches/I");
  p_numutree->Branch("std_NValidBunches", &_std_NValidBunches, "std_NValidBunches/I");
  p_numutree->Branch("std_BunchNbr",   &_std_BunchNbr, "std_BunchNbr/I");
  p_numutree->Branch("std_SpillWithTPC1Track",   &_std_SpillWithTPC1Track,   "std_SpillWithTPC1Track/I");
  p_numutree->Branch("std_NTrueVertices",     &_std_NTrueVertices,  "std_NTrueVertices/I");
  p_numutree->Branch("std_ReacCode",       _std_ReacCode,       "std_ReacCode[std_NTrueVertices]/I");
  p_numutree->Branch("std_TrueVertexPos",  _std_TrueVertexPos,  "std_TrueVertexPos[std_NTrueVertices][4]/D");
  p_numutree->Branch("std_TrueNuEnergy",   _std_TrueNuEnergy,   "std_TrueNuEnergy[std_NTrueVertices]/D");
  p_numutree->Branch("std_TrueNuDir",      _std_TrueNuDir,      "std_TrueNuDir[std_NTrueVertices][3]/D");
  p_numutree->Branch("std_TrueNuPDG",      _std_TrueNuPDG,      "std_TrueNuPDG[std_NTrueVertices]/I");
  p_numutree->Branch("std_TrueEnuWeight",  _std_TrueEnuWeight,  "std_TrueEnuWeight[std_NTrueVertices]/D");
  p_numutree->Branch("std_TrueVertexBunch",      _std_TrueVertexBunch,"std_TrueVertexBunch[std_NTrueVertices]/I");
  p_numutree->Branch("std_TrueLeptonMom",      _std_TrueLeptonMom,     "std_TrueLeptonMom[std_NTrueVertices]/D");
  p_numutree->Branch("std_TrueProtonMom",      _std_TrueProtonMom,     "std_TrueProtonMom[std_NTrueVertices]/D");
  p_numutree->Branch("std_TrueTargetMom",      _std_TrueTargetMom,     "std_TrueTargetMom[std_NTrueVertices]/D");
  p_numutree->Branch("std_TrueLeptonDir",      _std_TrueLeptonDir,     "std_TrueLeptonDir[std_NTrueVertices][3]/D");
  p_numutree->Branch("std_TrueProtonDir",      _std_TrueProtonDir,     "std_TrueProtonDir[std_NTrueVertices][3]/D");
  p_numutree->Branch("std_TrueTargetDir",      _std_TrueTargetDir,     "std_TrueTargetDir[std_NTrueVertices][3]/D");
  p_numutree->Branch("std_TrueTargetPDG",      _std_TrueTargetPDG,     "std_TrueTargetPDG[std_NTrueVertices]/D");
  p_numutree->Branch("std_TrueBaryonSize",     _std_TrueBaryonSize,    "std_TrueBaryonSize[std_NTrueVertices]/I");
  p_numutree->Branch("std_TrueQ2",             _std_TrueQ2,            "std_TrueQ2[std_NTrueVertices]/D");
  p_numutree->Branch("std_TrueVertexDet",      _std_TrueVertexDet,      "std_TrueVertexDet[std_NTrueVertices]/I");
  p_numutree->Branch("std_NECALTracks",      &_std_NECALTracks,  "std_NECALTracks/I");
  p_numutree->Branch("std_NP0DTracks",       &_std_NP0DTracks,   "std_NP0DTracks/I");
  p_numutree->Branch("std_NSMRDTracks",      &_std_NSMRDTracks,  "std_NSMRDTracks/I");
  p_numutree->Branch("std_NFGDTracks",       &_std_NFGDTracks,   "std_NFGDTracks/I");
  p_numutree->Branch("std_NTPCTracks",       &_std_NTPCTracks,   "std_NTPCTracks/I");
  p_numutree->Branch("std_NFGDUnusedHits", _std_NFGDUnusedHits,    "std_NFgdUnusedHits[2]/I");
  p_numutree->Branch("std_NTracksTotal",    &_std_NTracksTotal,     "std_NTracksTotal/I");
  p_numutree->Branch("std_NTpc1TracksTotal",    &_std_NTpcTracksTotal[0],     "std_NTpc1TracksTotal/I");
  p_numutree->Branch("std_NTpc2TracksTotal",    &_std_NTpcTracksTotal[1],     "std_NTpc2TracksTotal/I");
  p_numutree->Branch("std_NTpc3TracksTotal",    &_std_NTpcTracksTotal[2],     "std_NTpc3TracksTotal/I");
  
  //----------- filled for each bunch (syntax _stdB_) -----------
  p_numutree->Branch("stdB_BunchWithMaxMomInSpill",   &_stdB_BunchWithMaxMomInSpill,   "stdB_BunchWithMaxMomInSpill/I");
  p_numutree->Branch("stdB_TBunch",     &_stdB_TBunch,    "std_TBunch/D");
  p_numutree->Branch("stdB_Bunch",      &_stdB_Bunch,    "stdB_Bunch/I");
  p_numutree->Branch("stdB_NTracksWithTPC",      &_stdB_NTracksWithTPC,    "stdB_NTracksWithTPC/I");
  p_numutree->Branch("stdB_Vertex",     _stdB_Vertex,      "stdB_Vertex[4][4]/D");
  p_numutree->Branch("stdB_nTrueVerticesPerBunch",   &_stdB_nTrueVerticesPerBunch,   "stdB_nTrueVerticesPerBunch/I");
  p_numutree->Branch("stdB_NTracksGlobal", &_stdB_NTracksGlobal, "stdB_NTracksGlobal/I");
  p_numutree->Branch("stdB_NPosTracksWithTPC",       &_stdB_NPosTracksWithTPC,  "stdB_NPosTracksWithTPC/I");
  p_numutree->Branch("stdB_NNegTracksWithTPC",       &_stdB_NNegTracksWithTPC,  "stdB_NNegTracksWithTPC/I");
  p_numutree->Branch("stdB_NTpc1Tracks",    &_stdB_NTPCTracks[0],     "stdB_NTpc1Tracks/I");
  p_numutree->Branch("stdB_NTpc2Tracks",    &_stdB_NTPCTracks[1],     "stdB_NTpc2Tracks/I");
  p_numutree->Branch("stdB_NTpc3Tracks",    &_stdB_NTPCTracks[2],     "stdB_NTpc3Tracks/I");
  
  p_numutree->Branch("stdB_trDetectors",  _stdB_trDetectors,   "stdB_trDetectors[stdB_NTracksWithTPC]/I");
  p_numutree->Branch("stdB_trDetUsed",    _stdB_trDetUsed,  "stdB_trDetUsed[stdB_NTracksWithTPC][9]/I");
  p_numutree->Branch("stdB_trNHits",      _stdB_trNHits, "stdB_trNHits[stdB_NTracksWithTPC]/I");
  p_numutree->Branch("stdB_trChi2", _stdB_trChi2,  "stdB_trChi2[stdB_NTracksWithTPC]/D");
  p_numutree->Branch("stdB_trPositionStart",   _stdB_trPositionStart,   "stdB_trPositionStart[stdB_NTracksWithTPC][4]/D");
  p_numutree->Branch("stdB_trPositionEnd",   _stdB_trPositionEnd,   "stdB_trPositionEnd[stdB_NTracksWithTPC][4]/D");
  p_numutree->Branch("stdB_trDir",  _stdB_trDir,   "stdB_trDir[stdB_NTracksWithTPC][3]/D");
  p_numutree->Branch("stdB_trMom",   _stdB_trMom,   "stdB_trMom[stdB_NTracksWithTPC]/D");
  p_numutree->Branch("stdB_trMomError",  _stdB_trMomError,   "stdB_trMomErr[stdB_NTracksWithTPC]/D");
  p_numutree->Branch("stdB_trMomRelErr",  _stdB_trMomRelError,  "stdB_trMomRelErr[stdB_NTracksWithTPC]/D");
  p_numutree->Branch("stdB_trCharge",     _stdB_trCharge,   "stdB_trCharge[stdB_NTracksWithTPC]/I");
  p_numutree->Branch("stdB_trCosTheta",   _stdB_trCosTheta, "stdB_trCosTheta[stdB_NTracksWithTPC]/D");
  p_numutree->Branch("stdB_trECALTrShVal",    _stdB_trECALTrShVal,  "stdB_trECALTrShVal[stdB_NTracksWithTPC]/D");
  p_numutree->Branch("stdB_trEnuWeight",  _stdB_trEnuWeight,  "stdB_trEnuWeight[stdB_NTracksWithTPC]/D");
  p_numutree->Branch("stdB_trNFgds",      _stdB_trNFgds,    "stdB_trNFgds[stdB_NTracksWithTPC]/I");
  p_numutree->Branch("stdB_trTpcDet",     _stdB_trTpcDet,    "stdB_trTpcDet[stdB_NTracksWithTPC][3]/I");
  
  p_numutree->Branch("stdB_trCT",      _stdB_trCT,     "stdB_trCT[stdB_NTracksWithTPC][3]/D");
  p_numutree->Branch("stdB_trPullmu",  _stdB_trPull[0], "stdB_trPullmu[stdB_NTracksWithTPC][3]/D");
  p_numutree->Branch("stdB_trPullele",  _stdB_trPull[1], "stdB_trPullele[stdB_NTracksWithTPC][3]/D");
  p_numutree->Branch("stdB_trPullp",  _stdB_trPull[2], "stdB_trPullp[stdB_NTracksWithTPC][3]/D");
  p_numutree->Branch("stdB_trPullpi",  _stdB_trPull[3], "stdB_trPullpi[stdB_NTracksWithTPC][3]/D");
  p_numutree->Branch("stdB_trPullk",  _stdB_trPull[4], "stdB_trPullk[stdB_NTracksWithTPC][3]/D");
  p_numutree->Branch("stdB_trCTexpmu",  _stdB_trCTexp[0], "stdB_ctexpmu[stdB_NTracksWithTPC][3]/D");
  p_numutree->Branch("stdB_trCTexpele",  _stdB_trCTexp[1], "stdB_ctexpele[stdB_NTracksWithTPC][3]/D");
  p_numutree->Branch("stdB_trCTexpp",  _stdB_trCTexp[2], "stdB_ctexpp[stdB_NTracksWithTPC][3]/D");
  p_numutree->Branch("stdB_trCTexppi",  _stdB_trCTexp[3], "stdB_ctexppi[stdB_NTracksWithTPC][3]/D");
  p_numutree->Branch("stdB_trCTexpk",  _stdB_trCTexp[4], "stdB_ctexpk[stdB_NTracksWithTPC][3]/D");    
  p_numutree->Branch("stdB_trCTerrmu",  _stdB_trCTerr[0], "stdB_cterrmu[stdB_NTracksWithTPC][3]/D");
  p_numutree->Branch("stdB_trCTerrele",  _stdB_trCTerr[1], "stdB_cterrele[stdB_NTracksWithTPC][3]/D");
  p_numutree->Branch("stdB_trCTerrp",  _stdB_trCTerr[2], "stdB_cterrp[stdB_NTracksWithTPC][3]/D");
  p_numutree->Branch("stdB_trCTerrpi",  _stdB_trCTerr[3], "stdB_cterrpi[stdB_NTracksWithTPC][3]/D");
  p_numutree->Branch("stdB_trCTerrk",  _stdB_trCTerr[4], "stdB_cterrk[stdB_NTracksWithTPC][3]/D");
  // Note for Valencia people: Corrected variable have been moved to NumuFlatTree_GlbAna10a.C 
  // And a new variable for corrected trCTexp was created there as well.
  
  p_numutree->Branch("stdB_trTpcCharge",     _stdB_trTpcCharge, "stdB_trTpcCharge[stdB_NTracksWithTPC][3]/D");
  p_numutree->Branch("stdB_trTpcMom",        _stdB_trTpcMom,         "stdB_trTpcMom[stdB_NTracksWithTPC][3]/D");
  p_numutree->Branch("stdB_trTpcMomRelErr",  _stdB_trTpcMomRelError, "stdB_trTpcMomRelErr[stdB_NTracksWithTPC][3]/D");
  p_numutree->Branch("stdB_trTpcNHits",      _stdB_trTpcNHits,       "stdB_trTpcNHits[stdB_NTracksWithTPC][3]/I");
  
  p_numutree->Branch("stdB_trTruePDG",        _stdB_trTruePDG,        "stdB_trTruePDG[stdB_NTracksWithTPC]/I");
  p_numutree->Branch("stdB_trTrueParentPDG",  _stdB_trTrueParentPDG,  "stdB_trTrueParentPdg[stdB_NTracksWithTPC]/I");
  p_numutree->Branch("stdB_trTrueGParentPDG", _stdB_trTrueGParentPDG, "stdB_trTrueGParentPdg[stdB_NTracksWithTPC]/I");
  p_numutree->Branch("stdB_trTruePur",        _stdB_trTruePur,        "stdB_trTruePur[stdB_NTracksWithTPC]/D");
  p_numutree->Branch("stdB_trTruePosition",   _stdB_trTruePos,        "stdB_trTruePos[stdB_NTracksWithTPC][4]/D");
  p_numutree->Branch("stdB_trTrueDirection",  _stdB_trTrueDir,        "stdB_trTrueDir[stdB_NTracksWithTPC][3]/D");
  p_numutree->Branch("stdB_trTrueMomentum",   _stdB_trTrueMom,        "stdB_trTrueMom[stdB_NTracksWithTPC]/D");
  p_numutree->Branch("stdB_trTrueCharge",     _stdB_trTrueCharge,     "stdB_trTrueCharge[stdB_NTracksWithTPC]/I");
  p_numutree->Branch("stdB_trTrueVertexIndex",_stdB_trTrueVertexIndex,"stdB_trTrueVertexIndex[stdB_NTracksWithTPC]/I");
  
  
  p_numutree->Branch("stdB_NFgdTracksBunch",    &_stdB_NFgdTracksBunch, "stdB_NFgdTracksBunch/I");    
  p_numutree->Branch("stdB_fgdtr_Det",  _stdB_fgdtr_Det,   "stdB_fgdtr_Det[stdB_NFgdTracksBunch]/I");
  p_numutree->Branch("stdB_fgdtr_NHits",      _stdB_fgdtr_NHits, "stdB_fgdtr_NHits[stdB_NFgdTracksBunch]/I");
  p_numutree->Branch("stdB_fgdtr_Chi2", _stdB_fgdtr_Chi2,  "stdB_fgdtr_Chi2[stdB_NFgdTracksBunch]/D");
  p_numutree->Branch("stdB_fgdtr_PositionStart",   _stdB_fgdtr_PositionStart,   "stdB_fgdtr_PositionStart[stdB_NFgdTracksBunch][4]/D");
  p_numutree->Branch("stdB_fgdtr_PositionEnd",   _stdB_fgdtr_PositionEnd,   "stdB_fgdtr_PositionEnd[stdB_NFgdTracksBunch][4]/D");
  p_numutree->Branch("stdB_fgdtr_Dir",  _stdB_fgdtr_Dir,   "stdB_fgdtr_Dir[stdB_NFgdTracksBunch][3]/D");
  
  // Kinematics variables (for 2010a analysis see specific module)
  p_numutree->Branch("stdB_trENuRec_1Muon",_stdB_trENuRec_1Muon, "stdB_trENuRec_1Muon[stdB_NTracksWithTPC]/D");
  p_numutree->Branch("stdB_trEMuRec_1Muon",_stdB_trEMuRec_1Muon, "stdB_trEMuRec_1Muon[stdB_NTracksWithTPC]/D");
  p_numutree->Branch("stdB_trQ2_1Muon",_stdB_trQ2_1Muon, "stdB_trQ2_1Muon[stdB_NTracksWithTPC]/D");
  p_numutree->Branch("stdB_ENuRec_CCQE",  &_stdB_ENuRec_CCQE, "stdB_ENuRec_CCQE/D") ;
  p_numutree->Branch("stdB_Q2_CCQE",  &_stdB_Q2_CCQE, "stdB_Q2_CCQE/D") ;
  p_numutree->Branch("stdB_PQ2_CCQE",  &_stdB_PQ2_CCQE, "stdB_PQ2_CCQE/D") ;
  p_numutree->Branch("stdB_NuInvMass_CCQE",  &_stdB_NuInvMass_CCQE, "stdB_NuInvMass_CCQE/D") ;
  p_numutree->Branch("stdB_EProton_CCQE",         &_stdB_EProton_CCQE,      "stdB_EProton_CCQE/D");
  p_numutree->Branch("stdB_FinalCosTheta_CCQE",  &_stdB_FinalCosTheta_CCQE,     "stdB_FinalCosTheta_CCQE/D");
  p_numutree->Branch("stdB_FinalMass_CCQE",         &_stdB_FinalMass_CCQE,      "stdB_FinalMass_CCQE/D");
  p_numutree->Branch("stdB_FinalEnergy_CCQE",         &_stdB_FinalEnergy_CCQE,      "stdB_FinalEnergy_CCQE/D");
  p_numutree->Branch("stdB_FinalMom_CCQE",         &_stdB_FinalMom_CCQE,      "stdB_FinalMom_CCQE/D");
  
  p_numutree->Branch("stdB_EarliestTrackTime",  &_stdB_EarliestTrackTime,         "stdB_EarliestTrackTime/D");
  p_numutree->Branch("stdB_NClusters",          &_stdB_NClusters,    "stdB_NClusters/I");
  p_numutree->Branch("stdB_NClusterscut",       _stdB_NClusterscut,    "stdB_NClusterscut[3][5]/I");
  p_numutree->Branch("stdB_ClusterPos", _stdB_ClusterPos,       "stdB_ClusterPos[stdB_NClusters][4]/D");
  p_numutree->Branch("stdB_ClusterNHits", _stdB_ClusterNHits,   "stdB_ClusterNHits[stdB_NClusters]/I");
  p_numutree->Branch("stdB_ClusterRMS",   _stdB_ClusterRMS,     "stdB_ClusterRMS[stdB_NClusters][4]/D"); 
  p_numutree->Branch("stdB_ClusterCharge", _stdB_ClusterCharge, "stdB_ClusterCharge[stdB_NClusters]/D");
  p_numutree->Branch("stdB_ClusterDistVertex", _stdB_ClusterDistVertex,"dtdB_ClusterDistVertex[stdB_NClusters][3]/D");
  p_numutree->Branch("stdB_ClusterTimeDistVertex", _stdB_ClusterTimeDistVertex, "stdB_ClusterTimeDistVertex[stdB_NClusters][3]/D");
  
}


//Sets all class global variables to default values
//*************************************************************
void NumuFlatTree_Standard::ResetSpillVariables(){
  //*************************************************************
  
  _std_TrueSpillInfo = 0;
  _std_Evt = -1;
  _std_Run = -1;
  _std_FileRun = -1;
  _std_FileSubrun = -1;
  _std_Entry = -1;
  _std_NBunches = -1;
  _std_NValidBunches = -1;
  _std_BunchNbr = -1;
  _std_SpillWithTPC1Track = -1; 
  _std_NTrueVertices = 0;
  _std_NECALTracks = -1;
  _std_NP0DTracks = -1;
  _std_NSMRDTracks = -1;
  _std_NFGDTracks = -1;
  _std_NTPCTracks = -1;
  
  for(int j = 0; j < p_NMAXTRUEVERTICES; j ++) {
    _std_ReacCode[j] = 0;
    for(int k = 0; k < 4; k ++) _std_TrueVertexPos[j][k] = 0;
    _std_TrueNuEnergy[j] = 0;
    for(int k = 0; k < 3; k ++){
      _std_TrueNuDir[j][k] = 0;
      _std_TrueLeptonDir[j][k] = 0;
      _std_TrueProtonDir[j][k] = 0;
      _std_TrueTargetDir[j][k] = 0;
    }
    _std_TrueNuPDG[j] = 0;
    _std_TrueEnuWeight[j] = 0;
    _std_TrueVertexBunch[j] = 0;
    _std_TrueLeptonMom[j] = 0;
    _std_TrueProtonMom[j] = 0;
    _std_TrueTargetMom[j] = 0;
    _std_TrueTargetPDG[j] = 0;
    _std_TrueBaryonSize[j] = 0;
    _std_TrueQ2[j] = 0;  
    _std_TrueVertexDet[j] = 0;

    for(int t = 0; t < p_NMAXTRAJ; t ++) std_truePrimaryID[j][t] = 0;    

    std_trueLeptonEnergy[j] = 0;
  }

  _std_NFGDUnusedHits[0] = -1;
  _std_NFGDUnusedHits[1] = -1;

  _std_NTracksTotal = -1;
  for(int k = 0; k < 3; k ++) _std_NTpcTracksTotal[k] = -1;
  

  std_nPrimTraj = 0;
}


//Sets all class global bunch variables to default values
//*************************************************************
void NumuFlatTree_Standard::ResetBunchVariables(){
  //*************************************************************
  _stdB_BunchWithMaxMomInSpill = -1;
  _stdB_TBunch = -1;
  _stdB_Bunch = -1;
  _stdB_NTracksWithTPC = 0;
  _stdB_nTrueVerticesPerBunch = 0;
  _stdB_NTracksGlobal = 0;
  _stdB_EarliestTrackTime = 0;
  _stdB_NClusters = 0;  

  for(int i = 0; i < p_NMAXTRACKS; i ++){
    _stdB_trENuRec_1Muon[i] = -1;
    _stdB_trEMuRec_1Muon[i] = -1;
    _stdB_trQ2_1Muon[i] = -1;
  }
  _stdB_ENuRec_CCQE = -1;
  _stdB_Q2_CCQE = -1;
  _stdB_PQ2_CCQE = -1;
  _stdB_DeltaThetaP_CCQE = -2;
  _stdB_NuInvMass_CCQE = -1;
  _stdB_EProton_CCQE = 0;
  _stdB_FinalCosTheta_CCQE = -2;
  _stdB_FinalMass_CCQE = 0;
  _stdB_FinalEnergy_CCQE = 0;
  _stdB_FinalMom_CCQE = 0;
  

  for(int i = 0; i < 4; i ++){
    for(int k = 0; k < 4; k ++){
      _stdB_Vertex[i][k] = 0;
    }
  }

  for(int i = 0; i < p_NMAXTRACKS; i ++){
    for(int k = 0; k < 9; k ++){
    _stdB_trDetUsed[i][k] = 0;
    }
    for(int k = 0; k < 4; k ++){
      _stdB_trPositionStart[i][k] = 0;
      _stdB_trPositionEnd[i][k] = 0;
      _stdB_fgdtr_PositionStart[i][k] = 0;
      _stdB_fgdtr_PositionEnd[i][k] = 0;
    }
    for(int k = 0; k < 3; k ++){
      _stdB_trDir[i][k] = 0;
      _stdB_trTpcDet[i][k] = 0;
      _stdB_trCT[i][k] = 0;
      _stdB_trTpcMom[i][k] = 0;
      _stdB_trTpcMomRelError[i][k] = 0;
      _stdB_trTpcNHits[i][k] = 0;
      _stdB_fgdtr_Dir[i][k] = 0;

      for(int j = 0; j < p_NHYPO; j ++){
	_stdB_trPull[j][i][k] = 0;
	_stdB_trCTexp[j][i][k] = 0;
	_stdB_trCTerr[j][i][k] = 0;
      }
    }
    
    _stdB_trDetectors[i] = 0;
    _stdB_trNHits[i] = 0;
    _stdB_trChi2[i] = 0;
    _stdB_trMom[i] = 0;
    _stdB_trMomError[i] = 0;
    _stdB_trCharge[i] = 0;
    _stdB_trCosTheta[i] = -2;
    _stdB_trECALTrShVal[i] = 0;
    _stdB_trEnuWeight[i] = 0;
    _stdB_trNFgds[i] = 0;
    _stdB_fgdtr_Det[i] = 0;
    _stdB_fgdtr_NHits[i] = 0;
    _stdB_fgdtr_Chi2[i] = 0;
    

    _stdB_trTruePDG[i] = 0;
    _stdB_trTrueParentPDG[i] = 0;
    _stdB_trTrueGParentPDG[i] = 0;
    _stdB_trTruePur[i] = 0;
    for(int k = 0; k < 4; k ++) _stdB_trTruePos[i][k] = 0;
    for(int k = 0; k < 3; k ++) _stdB_trTrueDir[i][k] = 0;
    _stdB_trTrueMom[i] = 0;
    _stdB_trTrueCharge[i] = 0;
    _stdB_trTrueVertexIndex[i] = -1;
  }

  for(int i = 0; i < 5; i ++){ 
    for(int k = 0; k < 3; k ++){
      _stdB_NClusterscut[k][i] = 0;
    }
  }
  for(int k = 0; k < 3; k ++){
    _stdB_NTPCTracks[k] = 0;
  }
  
  for(int i = 0; i < p_NMAXCLUSTERS; i ++){
    for(int k = 0; k < 4; k ++){
      _stdB_ClusterPos[i][k] = 0;
      _stdB_ClusterRMS[i][k] = 0;
    }
    for(int k = 0; k < 3; k ++){
      _stdB_ClusterDistVertex[i][k] = 0;
      _stdB_ClusterTimeDistVertex[i][k] = 0;
    }
    _stdB_ClusterNHits[i] = 0;
    _stdB_ClusterCharge[i] = 0;
  }
}
  //**********************************************************
void  NumuFlatTree_Standard::SetBranchAddress(TTree *p_numutree){
    //**********************************************************
         //----------- filled for each spill (syntax _std_)  -----------
  p_numutree->SetBranchAddress("std_TrueSpillInfo",   &_std_TrueSpillInfo);  
  p_numutree->SetBranchAddress("std_Evt",        &_std_Evt);     
  p_numutree->SetBranchAddress("std_Run",        &_std_Run);    
  p_numutree->SetBranchAddress("std_FileRun",        &_std_FileRun); 
  p_numutree->SetBranchAddress("std_FileSubrun",        &_std_FileSubrun);   
  p_numutree->SetBranchAddress("std_Entry",      &_std_Entry);  
  p_numutree->SetBranchAddress("std_NBunches",   &_std_NBunches);
  p_numutree->SetBranchAddress("std_NValidBunches", &_std_NValidBunches);
  p_numutree->SetBranchAddress("std_BunchNbr",   &_std_BunchNbr); 
  p_numutree->SetBranchAddress("std_SpillWithTPC1Track",   &_std_SpillWithTPC1Track); 
  p_numutree->SetBranchAddress("std_NTrueVertices",     &_std_NTrueVertices);
  p_numutree->SetBranchAddress("std_ReacCode",       _std_ReacCode);       
  p_numutree->SetBranchAddress("std_TrueVertexPos",  _std_TrueVertexPos);  
  p_numutree->SetBranchAddress("std_TrueNuEnergy",   _std_TrueNuEnergy);   
  p_numutree->SetBranchAddress("std_TrueNuDir",      _std_TrueNuDir);      
  p_numutree->SetBranchAddress("std_TrueNuPDG",      _std_TrueNuPDG);      
  p_numutree->SetBranchAddress("std_TrueEnuWeight",  _std_TrueEnuWeight);  
  p_numutree->SetBranchAddress("std_TrueVertexBunch",      _std_TrueVertexBunch);
  p_numutree->SetBranchAddress("std_TrueLeptonMom",      _std_TrueLeptonMom);    
  p_numutree->SetBranchAddress("std_TrueProtonMom",      _std_TrueProtonMom);    
  p_numutree->SetBranchAddress("std_TrueTargetMom",      _std_TrueTargetMom);    
  p_numutree->SetBranchAddress("std_TrueLeptonDir",      _std_TrueLeptonDir);    
  p_numutree->SetBranchAddress("std_TrueProtonDir",      _std_TrueProtonDir);    
  p_numutree->SetBranchAddress("std_TrueTargetDir",      _std_TrueTargetDir);    
  p_numutree->SetBranchAddress("std_TrueTargetPDG",      _std_TrueTargetPDG);    
  p_numutree->SetBranchAddress("std_TrueBaryonSize",     _std_TrueBaryonSize);   
  p_numutree->SetBranchAddress("std_TrueQ2",             _std_TrueQ2);           
  p_numutree->SetBranchAddress("std_TrueVertexDet",      _std_TrueVertexDet);    
  p_numutree->SetBranchAddress("std_NECALTracks",      &_std_NECALTracks); 
  p_numutree->SetBranchAddress("std_NP0DTracks",       &_std_NP0DTracks);  
  p_numutree->SetBranchAddress("std_NSMRDTracks",      &_std_NSMRDTracks);
  p_numutree->SetBranchAddress("std_NFGDTracks",       &_std_NFGDTracks); 
  p_numutree->SetBranchAddress("std_NTPCTracks",       &_std_NTPCTracks); 
  p_numutree->SetBranchAddress("std_NFGDUnusedHits", _std_NFGDUnusedHits); 
  p_numutree->SetBranchAddress("std_NTracksTotal",    &_std_NTracksTotal); 
  p_numutree->SetBranchAddress("std_NTpc1TracksTotal",    &_std_NTpcTracksTotal[0]); 
  p_numutree->SetBranchAddress("std_NTpc2TracksTotal",    &_std_NTpcTracksTotal[1]); 
  p_numutree->SetBranchAddress("std_NTpc3TracksTotal",    &_std_NTpcTracksTotal[2]); 
  
  //----------- filled for each bunch (syntax _stdB_) -----------
  p_numutree->SetBranchAddress("stdB_BunchWithMaxMomInSpill",   &_stdB_BunchWithMaxMomInSpill); 
  p_numutree->SetBranchAddress("stdB_TBunch",     &_stdB_TBunch); 
  p_numutree->SetBranchAddress("stdB_Bunch",      &_stdB_Bunch);  
  p_numutree->SetBranchAddress("stdB_NTracksWithTPC",      &_stdB_NTracksWithTPC);  
  p_numutree->SetBranchAddress("stdB_Vertex",     _stdB_Vertex);   
  p_numutree->SetBranchAddress("stdB_nTrueVerticesPerBunch",   &_stdB_nTrueVerticesPerBunch); 
  p_numutree->SetBranchAddress("stdB_NTracksGlobal", &_stdB_NTracksGlobal);
  p_numutree->SetBranchAddress("stdB_NPosTracksWithTPC",       &_stdB_NPosTracksWithTPC);
  p_numutree->SetBranchAddress("stdB_NNegTracksWithTPC",       &_stdB_NNegTracksWithTPC);
  p_numutree->SetBranchAddress("stdB_NTpc1Tracks",    &_stdB_NTPCTracks[0]);    
  p_numutree->SetBranchAddress("stdB_NTpc2Tracks",    &_stdB_NTPCTracks[1]);    
  p_numutree->SetBranchAddress("stdB_NTpc3Tracks",    &_stdB_NTPCTracks[2]);    
  
  p_numutree->SetBranchAddress("stdB_trDetectors",  _stdB_trDetectors);   
  p_numutree->SetBranchAddress("stdB_trDetUsed",    _stdB_trDetUsed); 
  p_numutree->SetBranchAddress("stdB_trNHits",      _stdB_trNHits); 
  p_numutree->SetBranchAddress("stdB_trChi2", _stdB_trChi2);  
  p_numutree->SetBranchAddress("stdB_trPositionStart",   _stdB_trPositionStart); 
  p_numutree->SetBranchAddress("stdB_trPositionEnd",   _stdB_trPositionEnd);  
  p_numutree->SetBranchAddress("stdB_trDir",  _stdB_trDir);  
  p_numutree->SetBranchAddress("stdB_trMom",   _stdB_trMom); 
  p_numutree->SetBranchAddress("stdB_trMomError",  _stdB_trMomError);   
  p_numutree->SetBranchAddress("stdB_trMomRelErr",  _stdB_trMomRelError);  
  p_numutree->SetBranchAddress("stdB_trCharge",     _stdB_trCharge); 
  p_numutree->SetBranchAddress("stdB_trCosTheta",   _stdB_trCosTheta); 
  p_numutree->SetBranchAddress("stdB_trECALTrShVal",    _stdB_trECALTrShVal);  
  p_numutree->SetBranchAddress("stdB_trEnuWeight",  _stdB_trEnuWeight); 
  p_numutree->SetBranchAddress("stdB_trNFgds",      _stdB_trNFgds);   
  p_numutree->SetBranchAddress("stdB_trTpcDet",     _stdB_trTpcDet);  
  
  p_numutree->SetBranchAddress("stdB_trCT",      _stdB_trCT);     
  p_numutree->SetBranchAddress("stdB_trPullmu",  _stdB_trPull[0]); 
  p_numutree->SetBranchAddress("stdB_trPullele",  _stdB_trPull[1]);
  p_numutree->SetBranchAddress("stdB_trPullp",  _stdB_trPull[2]); 
  p_numutree->SetBranchAddress("stdB_trPullpi",  _stdB_trPull[3]);
  p_numutree->SetBranchAddress("stdB_trPullk",  _stdB_trPull[4]); 
  p_numutree->SetBranchAddress("stdB_trCTexpmu",  _stdB_trCTexp[0]); 
  p_numutree->SetBranchAddress("stdB_trCTexpele",  _stdB_trCTexp[1]);
  p_numutree->SetBranchAddress("stdB_trCTexpp",  _stdB_trCTexp[2]); 
  p_numutree->SetBranchAddress("stdB_trCTexppi",  _stdB_trCTexp[3]); 
  p_numutree->SetBranchAddress("stdB_trCTexpk",  _stdB_trCTexp[4]); 
  p_numutree->SetBranchAddress("stdB_trCTerrmu",  _stdB_trCTerr[0]);
  p_numutree->SetBranchAddress("stdB_trCTerrele",  _stdB_trCTerr[1]);
  p_numutree->SetBranchAddress("stdB_trCTerrp",  _stdB_trCTerr[2]);
  p_numutree->SetBranchAddress("stdB_trCTerrpi",  _stdB_trCTerr[3]);
  p_numutree->SetBranchAddress("stdB_trCTerrk",  _stdB_trCTerr[4]); 
  // Note for Valencia people: Corrected variable have been moved to NumuFlatTree_GlbAna10a.C 
  // And a new variable for corrected trCTexp was created there as well.
  
  p_numutree->SetBranchAddress("stdB_trTpcCharge",     _stdB_trTpcCharge);
  p_numutree->SetBranchAddress("stdB_trTpcMom",        _stdB_trTpcMom);   
  p_numutree->SetBranchAddress("stdB_trTpcMomRelErr",  _stdB_trTpcMomRelError);
  p_numutree->SetBranchAddress("stdB_trTpcNHits",      _stdB_trTpcNHits);      
  
  p_numutree->SetBranchAddress("stdB_trTruePDG",        _stdB_trTruePDG);      
  p_numutree->SetBranchAddress("stdB_trTrueParentPDG",  _stdB_trTrueParentPDG); 
  p_numutree->SetBranchAddress("stdB_trTrueGParentPDG", _stdB_trTrueGParentPDG);
  p_numutree->SetBranchAddress("stdB_trTruePur",        _stdB_trTruePur);       
  p_numutree->SetBranchAddress("stdB_trTruePosition",   _stdB_trTruePos);       
  p_numutree->SetBranchAddress("stdB_trTrueDirection",  _stdB_trTrueDir);       
  p_numutree->SetBranchAddress("stdB_trTrueMomentum",   _stdB_trTrueMom);       
  p_numutree->SetBranchAddress("stdB_trTrueCharge",     _stdB_trTrueCharge);    
  p_numutree->SetBranchAddress("stdB_trTrueVertexIndex",_stdB_trTrueVertexIndex);
  
  
  p_numutree->SetBranchAddress("stdB_NFgdTracksBunch",    &_stdB_NFgdTracksBunch);
  p_numutree->SetBranchAddress("stdB_fgdtr_Det",  _stdB_fgdtr_Det);   
  p_numutree->SetBranchAddress("stdB_fgdtr_NHits",      _stdB_fgdtr_NHits);
  p_numutree->SetBranchAddress("stdB_fgdtr_Chi2", _stdB_fgdtr_Chi2);  
  p_numutree->SetBranchAddress("stdB_fgdtr_PositionStart",   _stdB_fgdtr_PositionStart);  
  p_numutree->SetBranchAddress("stdB_fgdtr_PositionEnd",   _stdB_fgdtr_PositionEnd);  
  p_numutree->SetBranchAddress("stdB_fgdtr_Dir",  _stdB_fgdtr_Dir);   
  
  // Kinematics variables (for 2010a analysis see specific module)
  p_numutree->SetBranchAddress("stdB_trENuRec_1Muon",_stdB_trENuRec_1Muon); 
  p_numutree->SetBranchAddress("stdB_trEMuRec_1Muon",_stdB_trEMuRec_1Muon); 
  p_numutree->SetBranchAddress("stdB_trQ2_1Muon",_stdB_trQ2_1Muon); 
  p_numutree->SetBranchAddress("stdB_ENuRec_CCQE",  &_stdB_ENuRec_CCQE); 
  p_numutree->SetBranchAddress("stdB_Q2_CCQE",  &_stdB_Q2_CCQE); 
  p_numutree->SetBranchAddress("stdB_PQ2_CCQE",  &_stdB_PQ2_CCQE); 
  p_numutree->SetBranchAddress("stdB_NuInvMass_CCQE",  &_stdB_NuInvMass_CCQE); 
  p_numutree->SetBranchAddress("stdB_EProton_CCQE",         &_stdB_EProton_CCQE);   
  p_numutree->SetBranchAddress("stdB_FinalCosTheta_CCQE",  &_stdB_FinalCosTheta_CCQE); 
  p_numutree->SetBranchAddress("stdB_FinalMass_CCQE",         &_stdB_FinalMass_CCQE);  
  p_numutree->SetBranchAddress("stdB_FinalEnergy_CCQE",         &_stdB_FinalEnergy_CCQE); 
  p_numutree->SetBranchAddress("stdB_FinalMom_CCQE",         &_stdB_FinalMom_CCQE);     
  
  p_numutree->SetBranchAddress("stdB_EarliestTrackTime",  &_stdB_EarliestTrackTime);    
  p_numutree->SetBranchAddress("stdB_NClusters",          &_stdB_NClusters);   
  p_numutree->SetBranchAddress("stdB_NClusterscut",       _stdB_NClusterscut); 
  p_numutree->SetBranchAddress("stdB_ClusterPos", _stdB_ClusterPos);      
  p_numutree->SetBranchAddress("stdB_ClusterNHits", _stdB_ClusterNHits);  
  p_numutree->SetBranchAddress("stdB_ClusterRMS",   _stdB_ClusterRMS);    
  p_numutree->SetBranchAddress("stdB_ClusterCharge", _stdB_ClusterCharge);
  p_numutree->SetBranchAddress("stdB_ClusterDistVertex", _stdB_ClusterDistVertex);
  p_numutree->SetBranchAddress("stdB_ClusterTimeDistVertex", _stdB_ClusterTimeDistVertex); 
}

//**********************************************************
void NumuFlatTree_Standard::FillOAInfo(){
  //**********************************************************
  // Fill very basic variables from oaAnalysis 
  
  _std_Evt = p_EventID;
  
  if(p_isMC) _std_Run = 0;
  else _std_Run = p_RunID;
  
}

//*********************************************************
void NumuFlatTree_Standard::InitializeInternalVariables(){
  //********************************************************
  
  // Initialize some internal parameters                                                                                                                   
  NMaxNegReconTrack = p_NMAXTRACKS;
  NMaxPosReconTrack = p_NMAXTRACKS;
  NMaxRecTrack = p_NMAXTRACKS;
  NMaxFgdTrack = p_NMAXTRACKS;
  for(int j = 0; j < p_NBUNCH; j ++){
    std_NFgdTrack[j]= 0;
    NNegReconTrack[j]= 0;
    NPosReconTrack[j]= 0;
    std_NRecTrack[j]= 0;
    NGlobalTracks_total[j]= 0;
    NTpc1RecTrack[j]= 0;
    NTpc2RecTrack[j]= 0;
    NTpc3RecTrack[j]= 0;
    nvtxPerBunch[j] = 0;
  }
  std_NRecTrack_total = 0;
  NTpc1RecTrack_total = 0;
  NTpc2RecTrack_total = 0;
  NTpc3RecTrack_total = 0;
  
  // Set momentum sort to zero:
  for (int jj= 0; jj < p_NBUNCH; jj ++){ 
    global_index_usort[jj].clear();
    momentum_usort[jj].clear();
    fgd_index_usort[jj].clear();
    fgd_nhits_usort[jj].clear();
  }

  haveTPC1track = 0;
  savespillonce = false;
  
}


// Fill general MC info about one event
//*************************************************************                
void NumuFlatTree_Standard::FillVertexMCInfo(){
  //*************************************************************            
  
  ND::TTruthVerticesModule::TTruthVertex *true_vertex = NULL;
  //Loop over vertex types
  for(int vv = 0; vv < p_NVTXTYPES; vv ++){
    
    //Loop over true vertices
    for(int v = 0; v < p_NTruthVertices[vv]; v ++){
      
      true_vertex = (ND::TTruthVerticesModule::TTruthVertex*) p_TruthVertices[vv]->UncheckedAt(v);
      
      if(!true_vertex) continue;
      if(_std_NTrueVertices == p_NMAXTRUEVERTICES) break;
      
      _std_TrueVertexDet[_std_NTrueVertices] = p_TruthVertexDet[vv];
      
      if((true_vertex->ReactionCode).find(":") != string::npos){
	_std_ReacCode[_std_NTrueVertices] = utils->GetGenieReactionCode(true_vertex->ReactionCode);
      }	  
      else{
	_std_ReacCode[_std_NTrueVertices] = atoi((true_vertex->ReactionCode).c_str());
      }	     
      for(int k = 0; k < 4; k ++){
	_std_TrueVertexPos[_std_NTrueVertices][k] = true_vertex->Vertex[k];
      }
      
      _std_TrueVertexBunch[_std_NTrueVertices] = utils->GetBunch(_std_TrueVertexPos[_std_NTrueVertices][3]);
      
      // get the number of true vertices per bunch
      nvtxPerBunch[ _std_TrueVertexBunch[_std_NTrueVertices] ] ++;
      
      _std_TrueNuEnergy[_std_NTrueVertices] = true_vertex->MomNu.Vect().Mag();
      
      _std_TrueEnuWeight[_std_NTrueVertices] = GetEnuReweight(_std_TrueNuEnergy[_std_NTrueVertices]);
      
      _std_TrueBaryonSize[_std_NTrueVertices] = true_vertex->BaryonMoms.size();
      
      if(true_vertex->LeptonMoms.size() > 0){
	std_trueLeptonEnergy[_std_NTrueVertices] = true_vertex->LeptonMoms[0].E();
      }
      
      for(int ii = 0; ii < 3; ii ++){
	if(true_vertex->MomNu.Vect().Mag() > 0){
	  _std_TrueNuDir[_std_NTrueVertices][ii] = true_vertex->MomNu.Vect()[ii]/true_vertex->MomNu.Vect().Mag();
	}
	
	if(true_vertex->MomTarget.Vect().Mag() > 0){
	  _std_TrueTargetDir[_std_NTrueVertices][ii] = true_vertex->MomTarget.Vect()[ii]/true_vertex->MomTarget.Vect().Mag();
	}
	
	if(true_vertex->LeptonMoms.size() > 0){
	  if(true_vertex->LeptonMoms[0].Vect().Mag() > 0){
	    _std_TrueLeptonDir[_std_NTrueVertices][ii] = true_vertex->LeptonMoms[0].Vect()[ii]/true_vertex->LeptonMoms[0].Vect().Mag();
	  }
	}	       
	
	if( true_vertex->BaryonMoms.size() > 0){
	  if(true_vertex->BaryonMoms[0].Vect().Mag() > 0){
	    _std_TrueProtonDir[_std_NTrueVertices][ii] = true_vertex->BaryonMoms[0].Vect()[ii]/true_vertex->BaryonMoms[0].Vect().Mag();
	  }
	}
      }
      
      _std_TrueNuPDG[_std_NTrueVertices] = true_vertex->PDGnu;
      
      if(true_vertex->LeptonMoms.size() > 0){
	_std_TrueLeptonMom[_std_NTrueVertices] = true_vertex->LeptonMoms[0].Vect().Mag();
      }
      
      if(true_vertex->BaryonMoms.size() > 0){
	_std_TrueProtonMom[_std_NTrueVertices] = true_vertex->BaryonMoms[0].Vect().Mag();
      }
      
      _std_TrueTargetMom[_std_NTrueVertices] = true_vertex->MomTarget.Vect().Mag();
      _std_TrueTargetPDG[_std_NTrueVertices] = true_vertex->PDGtarget;
      
      if(true_vertex->LeptonMoms.size() > 0) FillTrueQ2();
      else _std_TrueQ2[_std_NTrueVertices] = 0;
      
      //Fill primary trajectory info for each type of vertices
      //----------------------------------------------------------
      for(unsigned int ll = 0; ll < true_vertex->LeptonIDs.size(); ll ++){
	std_truePrimaryID[_std_NTrueVertices][std_nPrimTraj] = true_vertex->LeptonIDs[ll];
	std_nPrimTraj ++;
      }
      for(unsigned int ll = 0; ll < true_vertex->BaryonIDs.size(); ll ++){
	std_truePrimaryID[_std_NTrueVertices][std_nPrimTraj] = true_vertex->BaryonIDs[ll];
	std_nPrimTraj ++;
      }
      for(unsigned int ll = 0; ll < true_vertex->OtherNeutralIDs.size(); ll ++){
	std_truePrimaryID[_std_NTrueVertices][std_nPrimTraj] = true_vertex->OtherNeutralIDs[ll];
	std_nPrimTraj ++;
      }
      for(unsigned int ll = 0; ll < true_vertex->MesonIDs.size(); ll ++){
	std_truePrimaryID[_std_NTrueVertices][std_nPrimTraj] = true_vertex->MesonIDs[ll];
	std_nPrimTraj ++;
      }
      for(unsigned int ll = 0; ll < true_vertex->OtherChargedIDs.size(); ll ++){
	std_truePrimaryID[_std_NTrueVertices][std_nPrimTraj] = true_vertex->OtherChargedIDs[ll];
	std_nPrimTraj ++;
      }
      for(unsigned int ll = 0; ll < true_vertex->PhotonIDs.size(); ll ++){
	std_truePrimaryID[_std_NTrueVertices][std_nPrimTraj] = true_vertex->PhotonIDs[ll];
	std_nPrimTraj ++;
      }
      
      _std_NTrueVertices ++;
      
    }//End loop over vertices
  }//End loop over vertex types
}

// Prepare variables before the loop on the bunch
//***********************************
int NumuFlatTree_Standard::PrepareVariables(int ibunch,int ipid){
  
  // Prepare the tracks counter and sort tracks according to momentum or FGD hits  
  
  // Copy the current pid to gTrack
  ND::TGlobalReconModule::TGlobalPID *gTrack = (ND::TGlobalReconModule::TGlobalPID*)(*p_PIDs)[ipid];
  
  NGlobalTracks_total[ibunch]++;  // Count total number of global track in bunch                                                
  
  //--- Check for Fgd isolated  tracks -----------------                                                                      
  if(gTrack->Detectors == 4 || gTrack->Detectors == 5){
    
    if(std_NFgdTrack[ibunch] == p_NMAXTRACKS) return 1;
    
    std_NFgdTrack[ibunch] ++;
    double fgdhits = gTrack->NHits;
    // Do sorting according to FGD hits                                                                                   
    fgd_index_usort[ibunch].push_back(ipid);
    fgd_nhits_usort[ibunch].push_back(fgdhits);
  }
  // --------End check FGD for isolated tracks ------------ 
  
  //----------- For TPC tracks --------------------              
    if(!gTrack->NTPCs) return 0; // be default if global track does not                                                                       
    // have a TPC segment (and is not a FGD isolated object) , go to next PID.   
    
    if(std_NRecTrack[ibunch] == p_NMAXTRACKS) return 1;//reached all the possible array space

    //Do sorting according to momentum                                                                                           
    global_index_usort[ibunch].push_back(ipid);
    momentum_usort[ibunch].push_back(gTrack->Momentum);  
    
    // Count numbers of tracks with TPC segments 
    std_NRecTrack[ibunch] ++; 
    std_NRecTrack_total ++;   
    
    // ***** check with detectors are used  and keep counters**********                                                        
    int detec = gTrack->Detectors;                                                                                 
    if(utils->CheckTpcUsed(1, detec)){ 
      NTpc1RecTrack[ibunch] ++;       
      NTpc1RecTrack_total ++;        
    }
    
    if(utils->CheckTpcUsed(2, detec)){
      NTpc2RecTrack[ibunch] ++; 
      NTpc2RecTrack_total ++; 
    }                                                   
    
    if(utils->CheckTpcUsed(3, detec)){ 
      NTpc3RecTrack[ibunch] ++; 
      NTpc3RecTrack_total ++;   
    }                                                            
    if(gTrack->Charge > 0){   
      NPosReconTrack[ibunch] ++; 
    }                           
    if(gTrack->Charge < 0){                                                                                                                   
      NNegReconTrack[ibunch] ++;
    }
    
    return 0;
}
// Fill Bunch variables
//*************************************************************                                         
void NumuFlatTree_Standard::FillBunchVariables(int ibunch){

  // Easy to copy variables:
  
  _stdB_NTracksWithTPC = std_NRecTrack[ibunch];
  _stdB_NFgdTracksBunch = std_NFgdTrack[ibunch];
  
  
  _stdB_NPosTracksWithTPC = NPosReconTrack[ibunch];                       
  _stdB_NNegTracksWithTPC = NNegReconTrack[ibunch];                                                                                       
  _stdB_NTPCTracks[0] = NTpc1RecTrack[ibunch];                                                                                         
  _stdB_NTPCTracks[1] = NTpc2RecTrack[ibunch];                                                                                         
  _stdB_NTPCTracks[2] = NTpc3RecTrack[ibunch];
  
  _stdB_Bunch = ibunch;
  _stdB_TBunch = utils->GetBunchTime(ibunch);
  _stdB_NTracksGlobal = NGlobalTracks_total[ibunch];
  _std_TrueSpillInfo = 0;
  
  //Save _std_TrueSpillInfo 
  if(!savespillonce){
    savespillonce = true;
    _std_TrueSpillInfo = 1;
  }
  
  //Fill _BunchWithMaxMomInSpill                                                                                                        
  if(ibunch == max_mom_bunch) _stdB_BunchWithMaxMomInSpill = 1;
  else _stdB_BunchWithMaxMomInSpill = 0;
  
  //get the number of true vertices per recbunches                                                                                      
  if(p_isMC) _stdB_nTrueVerticesPerBunch = nvtxPerBunch[ibunch];
  
}


//********************************************************************
// ** This method is still quite messy.... need to be cleaned up


void NumuFlatTree_Standard::FillBunchVariablesFromPIDs(int ibunch){
  
  // Get info from oaAnalysis global recon:
  ND::TGlobalReconModule::TGlobalPID fgdtracks_bunch[p_NMAXTRACKS];
  bool fgdtracks_bunch_exist[p_NMAXTRACKS];
  
  ND::TGlobalReconModule::TGlobalPID tracks_bunch[p_NMAXTRACKS];
  bool tracks_bunch_exist[p_NMAXTRACKS];
  
  for(int i = 0; i < p_NMAXTRACKS; i ++){
    fgdtracks_bunch_exist[i] = false;
    tracks_bunch_exist[i] = false;
  }
  

  //sort all the tracks by momentum and FGD iso objects by number of hits
  //--------------------------------------------------------------------
  vector<double> momentum; //momentum in falling order
  vector<int> index; //index of momentum order
  //Copy vector and sort in momentum
  for(int i = 0; i < std_NRecTrack[ibunch]; i ++){
    momentum.push_back(momentum_usort[ibunch][i]);
  }
  
  //Sort according to increasing momentum
  sort(momentum.begin(),momentum.end());
  //Mapping of index for decreasing momentum
  for(int i = 0; i < std_NRecTrack[ibunch]; i ++){
    for(int ii = 0; ii < std_NRecTrack[ibunch]; ii ++){
      if(momentum[std_NRecTrack[ibunch] - 1 - i] == momentum_usort[ibunch][ii]){
	index.push_back(ii);
      }
    }
  }      
  
  
  for(int j = 0; j < std_NRecTrack[ibunch]; j ++){
    if(j == p_NMAXTRACKS) break;
    int it = index[j];
    cout<< it<<" stand "<<endl;
    int index_pos = global_index_usort[ibunch][it];
    ND::TGlobalReconModule::TGlobalPID *gTrack = (ND::TGlobalReconModule::TGlobalPID*)(*p_PIDs)[index_pos];
    tracks_bunch[j] = *gTrack;
    tracks_bunch_exist[j] = true;
  }
  
  
  // Get the FGD iso tracks with higher number of hits
  // get the track with highest momentum:sort tracks       
  vector<double> fgd_nhits; //nhits in falling order
  vector<int> fgd_index_hits; //index of nhits order
  
  //Copy vector and sort in nhits
  for(int i = 0; i < std_NFgdTrack[ibunch]; i ++) fgd_nhits.push_back(fgd_nhits_usort[ibunch][i]);
  
  //Sort according to increasing nhits
  sort(fgd_nhits.begin(),fgd_nhits.end());
  
  //Mapping of index for decreasing nhits
  for(int i = 0; i < std_NFgdTrack[ibunch]; i ++){      
    for(int ii = 0; ii < p_NPIDs; ii ++){
      if(fgd_nhits[std_NFgdTrack[ibunch] - 1 - i] == fgd_nhits_usort[ibunch][ii]) fgd_index_hits.push_back(ii);
    }
  }
  
  for(int j = 0; j < std_NFgdTrack[ibunch]; j ++){
    
    if(j == p_NMAXTRACKS) break;
    
    int it = fgd_index_hits[j];
    int index_pos = fgd_index_usort[ibunch][it];
    ND::TGlobalReconModule::TGlobalPID *gTrack = (ND::TGlobalReconModule::TGlobalPID*)(*p_PIDs)[index_pos];
    fgdtracks_bunch[j] = *gTrack;
    fgdtracks_bunch_exist[j] = true;
  }	 
  
  // end momentum sorting and fgd hits sorting
  //-------------------------------------------------------------------
  
  
  // Start to fill bunch variables of standard module
  FillBunchVariables(ibunch);
  
  
  //Fill the tree for this bunch:
  //------------------------------------------------------------
  
  // a) Fill the vertex information 
  //*************************************
  
  // 1. start point of the track with highest momentum)    
  TLorentzVector thisPos = utils->GetStartOfTrack(tracks_bunch[0].BackPosition,tracks_bunch[0].FrontPosition, tracks_bunch[0].isForward);
  
  for(int i = 0; i < 4; i ++) _stdB_Vertex[0][i] = thisPos[i];  
  TLorentzVector vtx1(_stdB_Vertex[0][0], _stdB_Vertex[0][1], _stdB_Vertex[0][2], _stdB_Vertex[0][3]); 
  
  // 2. start point of the negative track with highest momentum    
  if(tracks_bunch[0].Charge == -1){
    for(int i = 0; i < 4; i ++){
      _stdB_Vertex[1][i] = thisPos[i];
    }
  }
  else if(tracks_bunch[1].Charge == -1){
    thisPos = utils->GetStartOfTrack(tracks_bunch[1].BackPosition,tracks_bunch[1].FrontPosition, tracks_bunch[1].isForward);
    for(int i = 0; i < 4; i ++){
      _stdB_Vertex[1][i] = thisPos[i];
    }
  }	  
  TLorentzVector vtx2(_stdB_Vertex[1][0], _stdB_Vertex[1][1], _stdB_Vertex[1][2], _stdB_Vertex[1][3]);       
  
  // b) Fill the delayed clusters for FGD
  //*************************************
  
  FillDelayedClusters(p_DelayedClusters, p_NDelayedClusters, p_EarliestTrackTime, vtx1, vtx2); 
  
  // c) Fill FGD iso tracks 	  
  //*************************************
  
  //Loop over the FGD tracks in bunch
  //----------------------------------------------------------------
  for(int itrk = 0; itrk < _stdB_NFgdTracksBunch; itrk ++){
    
    if(itrk == p_NMAXTRACKS) break;
    
    //If FGD track fill FGD info
    if(!fgdtracks_bunch_exist[itrk]) continue;
    
    _stdB_fgdtr_NHits[itrk] = fgdtracks_bunch[itrk].NHits;
    _stdB_fgdtr_Chi2[itrk] = fgdtracks_bunch[itrk].Quality;
    _stdB_fgdtr_Det[itrk] = fgdtracks_bunch[itrk].Detectors;
    
//    // Position and Direction at vertex
    double dummy=0;
    TLorentzVector fgd_posstart;
    TLorentzVector fgd_posend;
    TVector3 fgd_dir;
    if (_stdB_fgdtr_Det[itrk] == 4 || _stdB_fgdtr_Det[itrk]==5) {
      utils->FillPosDirAtVertex(itrk,fgdtracks_bunch[itrk].BackPosition,
				fgdtracks_bunch[itrk].FrontPosition,
				fgdtracks_bunch[itrk].BackDirection,
				fgdtracks_bunch[itrk].FrontDirection,
				fgdtracks_bunch[itrk].isForward,
				fgd_posstart,
				fgd_posend,
				fgd_dir,
				dummy); // Theta angle isn't useful for FGD iso tracks
    }

    // Copy variables to tree variables
    for(int c = 0; c < 4; c ++){
      _stdB_fgdtr_PositionStart[itrk][c]=fgd_posstart[c];
      _stdB_fgdtr_PositionEnd[itrk][c]=fgd_posend[c];
    }
    for(int c = 0; c < 3; c ++){
      _stdB_fgdtr_Dir[itrk][c]=fgd_dir[c];
    }


  }//End loop over FGD tracks
  
  // d) Fill global tracks with TPC constituents
  //*************************************
  
  //Loop over the tracks in bunch
  //----------------------------------------------------------------
  for(int itrk = 0; itrk < _stdB_NTracksWithTPC; itrk ++){
    
    if(itrk == p_NMAXTRACKS) break;
    
    //If TPC track
    if(!tracks_bunch_exist[itrk]) continue;
    

    FillDetectorsUsed(itrk, tracks_bunch[itrk].Detectors);

    if(_stdB_trDetUsed[itrk][0] == 1){
      haveTPC1track = 1;
    }
    
    //For MC  (Describe goal of what is done.... can it be moved to a method?)
    //-----------------------------------------------------------
    if(p_isMC){
      if(tracks_bunch[itrk].TrueParticle.PDG != 0){
	
	_stdB_trTruePur[itrk] = tracks_bunch[itrk].TrueParticle.Pur;
	_stdB_trTrueMom[itrk] = tracks_bunch[itrk].TrueParticle.InitMom.Vect().Mag();
	
	for(int c = 0; c < 4; c ++){
	  _stdB_trTruePos[itrk][c] = tracks_bunch[itrk].TrueParticle.InitPos[c];
	}
	if(_stdB_trTrueMom[itrk] != 0){
	  for(int c = 0; c < 3; c ++){
	    _stdB_trTrueDir[itrk][c] = tracks_bunch[itrk].TrueParticle.InitMom.Vect()[c]/_stdB_trTrueMom[itrk];
	  }
	}
	
	_stdB_trTrueCharge[itrk] = tracks_bunch[itrk].TrueParticle.Charge;
	_stdB_trTruePDG[itrk] = tracks_bunch[itrk].TrueParticle.PDG;
      }//End if PDG != 0
      
      _stdB_trTrueParentPDG[itrk] = tracks_bunch[itrk].TrueParticle.ParentPDG;
      _stdB_trTrueGParentPDG[itrk] = tracks_bunch[itrk].TrueParticle.GrandParentPDG;
      
      bool trajfound = false;
      
      //look for the true vertex 
      //*****************************************
      int primID;
      //Loop over trajectory types
      for(int tt = 0; tt < p_NTRAJTYPES; tt ++){
	
	//Loop over trajectories
	for(int ii = 0; ii< p_NTruthTrajs[tt]; ii++){                
	  ND::TTruthTrajectoriesModule::TTruthTrajectory* traj = (ND::TTruthTrajectoriesModule::TTruthTrajectory*)p_TruthTrajs[tt]->UncheckedAt(ii);
	  
	  if(tracks_bunch[itrk].TrueParticle.ID != traj->TrajID) continue;
	  
	  trajfound = true;
	  primID = traj->PrimaryID;
	  
	  if(primID == 0){
	    primID = traj->TrajID;
	  }
	  
	  //Loop over true vertices
	  for(int v = 0; v < _std_NTrueVertices; v ++){
	    //Loop over primaries
	    for(int prim = 0; prim < std_nPrimTraj; prim ++){
	      if( std_truePrimaryID[v][prim] == primID){
		_stdB_trTrueVertexIndex[itrk] = v;
	      }
	    }//End loop over primaries
	  }//End loop over true vertices
	  
	  break; //Only find one match per trajectory
	  
	}//End loop over trajectories
      }//End loop over trajectory types
      
      //this can be solved if in the globalreconmodule we add the info of the primID directly
      if(!trajfound){
	//if(!p_silent) cout<<" it has not been found "<<endl;	      	      	    	      
      }
      
      //If no true vertex
      if(_stdB_trTrueVertexIndex[itrk] == -1){
	
	//Find true vertex
	int tvertex = -1;
	double dist_min = 1e5;
	double dist;
	double dist2;
	double dist3;
	
	//Loop over true vertices
	for(int v = 0; v < _std_NTrueVertices; v ++){
	  dist = sqrt(pow(tracks_bunch[itrk].TrueParticle.Vertex.Position[0] - _std_TrueVertexPos[v][0], 2) + 
		      pow(tracks_bunch[itrk].TrueParticle.Vertex.Position[1] - _std_TrueVertexPos[v][1], 2) + 
		      pow(tracks_bunch[itrk].TrueParticle.Vertex.Position[2] - _std_TrueVertexPos[v][2], 2));
	  
	  dist2 = sqrt(pow(tracks_bunch[itrk].TrueParticle.ParentInitPos[0] - _std_TrueVertexPos[v][0], 2) + 
		       pow(tracks_bunch[itrk].TrueParticle.ParentInitPos[1] - _std_TrueVertexPos[v][1], 2) + 
		       pow(tracks_bunch[itrk].TrueParticle.ParentInitPos[2] - _std_TrueVertexPos[v][2], 2));
	  
	  dist3 = sqrt(pow(tracks_bunch[itrk].TrueParticle.GrandParentInitPos[0] - _std_TrueVertexPos[v][0], 2) + 
		       pow(tracks_bunch[itrk].TrueParticle.GrandParentInitPos[1] - _std_TrueVertexPos[v][1], 2) + 
		       pow(tracks_bunch[itrk].TrueParticle.GrandParentInitPos[2] - _std_TrueVertexPos[v][2], 2));
	  
	  if(dist == 0 || dist2 == 0 || dist3 == 0){
	    _stdB_trTrueVertexIndex[itrk] = v;
	  }
	}//End loop over true vertices
      }//End if no true vertex

      if(_stdB_trTrueVertexIndex[itrk] == -1){
	cerr<<" problem no vertex associated. ibunch ="<<ibunch<<endl;
      }	     
      else{
	_stdB_trEnuWeight[itrk] = _std_TrueEnuWeight[ _stdB_trTrueVertexIndex[itrk]];
      }
      
      //Loop over ECALs
      for(int jj = 0; jj < tracks_bunch[itrk].NECALs; jj ++){
	
	//Using globaltrack here sounds like a bug!!!!
	//	    ND::TGlobalReconModule::TECALObject *ecalTrack = (ND::TGlobalReconModule::TECALObject*)(*globalTrack->ECAL)[jj];
	
	ND::TGlobalReconModule::TECALObject *ecalTrack=(ND::TGlobalReconModule::TECALObject*)(*tracks_bunch[itrk].ECAL)[jj];	    
	
	
	
	if(ecalTrack){
	  _stdB_trECALTrShVal[itrk] = ecalTrack->TrShVal;
	}
      }//End loop over ECALs
      
    }//End if MC
    //----------------------------------------------------------------------
    
    _stdB_trNHits[itrk] = tracks_bunch[itrk].NHits;
    _stdB_trChi2[itrk] = tracks_bunch[itrk].Quality;
    _stdB_trDetectors[itrk] = tracks_bunch[itrk].Detectors;
    _stdB_trMom[itrk] = tracks_bunch[itrk].FrontMomentum;
    _stdB_trMomError[itrk] = tracks_bunch[itrk].frontMomentumError;
    _stdB_trCharge[itrk] = tracks_bunch[itrk].Charge;
    _stdB_trNFgds[itrk] = tracks_bunch[itrk].NFGDs;
    
    if(tracks_bunch[itrk].FrontMomentum != 0){
      _stdB_trMomRelError[itrk] = tracks_bunch[itrk].FrontMomentumError/tracks_bunch[itrk].FrontMomentum;
    }
    
    // Position and Direction at vertex
    double tpc_costheta=0;
    TLorentzVector tpc_posstart;
    TLorentzVector tpc_posend;
    TVector3 tpc_dir;
    utils->FillPosDirAtVertex(itrk,tracks_bunch[itrk].BackPosition,
				tracks_bunch[itrk].FrontPosition,
				tracks_bunch[itrk].BackDirection,
				tracks_bunch[itrk].FrontDirection,
				tracks_bunch[itrk].isForward,
				tpc_posstart,
				tpc_posend,
				tpc_dir,
				tpc_costheta); 
    

    // Copy variables to tree variables
    for(int c = 0; c < 4; c ++){
      _stdB_trPositionStart[itrk][c]=tpc_posstart[c];
      _stdB_trPositionEnd[itrk][c]=tpc_posend[c];
    }
    for(int c = 0; c < 3; c ++){
      _stdB_trDir[itrk][c]=tpc_dir[c];
    }
    _stdB_trCosTheta[itrk]=tpc_costheta;

    
    //---------- PID Pulls and TPC and ECAL variables -----------------------------
    //Loop over TPC track segment for this track
    for(int jj = 0; jj < tracks_bunch[itrk].NTPCs; jj ++){
      ND::TGlobalReconModule::TTPCObject *tpcTrack=(ND::TGlobalReconModule::TTPCObject*)(*(tracks_bunch[itrk].TPC))[jj];
      if(!tpcTrack) continue;

      _stdB_trTpcDet[itrk][tpcTrack->Detector-1] = tpcTrack->Detector;
      
      _stdB_trPull[0][itrk][tpcTrack->Detector-1] = tpcTrack->PullMuon;
      _stdB_trPull[1][itrk][tpcTrack->Detector-1] = tpcTrack->PullEle;
      _stdB_trPull[2][itrk][tpcTrack->Detector-1] = tpcTrack->PullProton;
      _stdB_trPull[3][itrk][tpcTrack->Detector-1] = tpcTrack->PullPion;
      _stdB_trPull[4][itrk][tpcTrack->Detector-1] = tpcTrack->PullKaon;	
      
      _stdB_trCT[itrk][tpcTrack->Detector-1] = tpcTrack->Ccorr;
      
      _stdB_trCTexp[0][itrk][tpcTrack->Detector-1] = tpcTrack->dEdxexpMuon;
      _stdB_trCTexp[1][itrk][tpcTrack->Detector-1] = tpcTrack->dEdxexpEle;
      _stdB_trCTexp[2][itrk][tpcTrack->Detector-1] = tpcTrack->dEdxexpProton;
      _stdB_trCTexp[3][itrk][tpcTrack->Detector-1] = tpcTrack->dEdxexpPion;
      _stdB_trCTexp[4][itrk][tpcTrack->Detector-1] = tpcTrack->dEdxexpKaon;
      
      _stdB_trCTerr[0][itrk][tpcTrack->Detector-1] = tpcTrack->SigmaMuon;
      _stdB_trCTerr[1][itrk][tpcTrack->Detector-1] = tpcTrack->SigmaEle;
      _stdB_trCTerr[2][itrk][tpcTrack->Detector-1] = tpcTrack->SigmaProton;
      _stdB_trCTerr[3][itrk][tpcTrack->Detector-1] = tpcTrack->SigmaPion;
      _stdB_trCTerr[4][itrk][tpcTrack->Detector-1] = tpcTrack->SigmaKaon;
      
      _stdB_trTpcNHits[itrk][tpcTrack->Detector-1]  = tpcTrack->NHits;	    
      
      _stdB_trTpcCharge[itrk][tpcTrack->Detector-1] = tpcTrack->Charge;
      _stdB_trTpcMom[itrk][tpcTrack->Detector-1] = tpcTrack->FrontMomentum;
      _stdB_trTpcMomRelError[itrk][tpcTrack->Detector-1] = tpcTrack->FrontMomentumError/tpcTrack->FrontMomentum;
      
    }//End loop over TPCs

    // Fill Kinematics using info of only 1 track, assuming it is a muon
    // For positive track, these variable are set to zero.    
    FillKinematicsFor1Muon(_stdB_trCharge[itrk], _stdB_trMom[itrk], _stdB_trCosTheta[itrk],itrk);

  }// end loop over the tracks with TPC segment in this bunch
  //------------------------------------------------------------------
  //Fill _std_SpillWithTPC1Track                                                
  _std_SpillWithTPC1Track = haveTPC1track; 
  
  
  

  // Fill kinematics for the two highest momentum tracks, assuming a CCQE interaction

  if (tracks_bunch_exist[0]  &&  tracks_bunch_exist[1]){
    FillKinematicsCCQE(); 
  }
  
}

///////////////////////////////////////////////////////////////////////////////////////
//************************************
void NumuFlatTree_Standard::PrepareBunchWithMaxMomInSpill(){
  //--------- Find the bunch which contains the highest momentum track of the spill  
  max_mom = -999.;
  max_mom_bunch = -1;
  
    //Loop over bunch containers                                                                                                      
  for(int j = 0; j < p_NBUNCH; j ++){
    //Loop over the selected tracks                                                                                  
    for(int ii = 0; ii < std_NRecTrack[j]; ii ++){
      if(momentum_usort[j][ii] > max_mom){
	max_mom = momentum_usort[j][ii];
	max_mom_bunch = j;
      }
    }
  }
}

//***************************************
void NumuFlatTree_Standard::FillNTracksSpillInfo(){
  
  // Store ntracks info from what was done in previous PIDs loop                                                     
  
  _std_NTracksTotal = std_NRecTrack_total;
  _std_NTpcTracksTotal[0] = NTpc1RecTrack_total;
  _std_NTpcTracksTotal[1] = NTpc2RecTrack_total;
  _std_NTpcTracksTotal[2] = NTpc3RecTrack_total;
  
  _std_NFGDTracks  = p_NFGDIso;
  _std_NDsECALTracks = p_NDsECALIso;
  _std_NTrECALTracks = p_NTrECALIso;
  _std_NP0DTracks  = p_NP0DIso;
  _std_NSMRDTracks = p_NSMRDIso;
  _std_NTPCTracks =  p_NTPCIso;
  
  _stdB_EarliestTrackTime = p_EarliestTrackTime;
  
  _std_NFGDUnusedHits[0] = p_NFGD1Unused;
  _std_NFGDUnusedHits[1] = p_NFGD2Unused;
  
  
}


//Fills information for two HW tracks assuming it is a CCQE interaction
//**********************************************************
void NumuFlatTree_Standard::FillKinematicsCCQE(){ 

  double posMom = 0;
  double negMom = 0;
  double Emu = 0;
  double Ep = 0;
  
  //For two tracks
  //The 2 most energetic tracks are one positive and one negative 
  if((_stdB_trCharge[0] ==  1 && _stdB_trCharge[1] == -1) || (_stdB_trCharge[0] == -1 && _stdB_trCharge[1] == 1)){
    
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
    
    // Check which track is negative:	
    if(_stdB_trCharge[0] == 1){    
      posIndex = 0;
      negIndex = 1;
    }
    else{
      posIndex = 1;
      negIndex = 0;
    }
    
    posDir = _stdB_trDir[posIndex];
    posMom = _stdB_trMom[posIndex];
    posMomVect = posMom * posDir;
    negDir = _stdB_trDir[negIndex];
    negMom = _stdB_trMom[negIndex];
    negMomVect = negMom * negDir;
    
    Emu = sqrt(pow(p_muMass,2) + pow(_stdB_trMom[negIndex],2));
    Ep = sqrt(pow(p_pMass, 2) + pow(_stdB_trMom[posIndex],2));
    
    //final state kinematics
    Ef = Emu + Ep;
    finalStateMom = negMomVect + posMomVect;
    fMom = finalStateMom.Mag();
    fMomZ = finalStateMom.Z();
    Mf2 = pow(Ef,2)- pow(fMom,2);
    
    //we assume the neutrino moving along the z axis
    cosThetaF = fMomZ/fMom;
    
    _stdB_EProton_CCQE = Ep;
    _stdB_FinalCosTheta_CCQE = cosThetaF;
    _stdB_FinalMass_CCQE = sqrt(Mf2);
    _stdB_FinalEnergy_CCQE  = Ef;
    _stdB_FinalMom_CCQE = fMom;
    //	
    //neutrino energy assuming CCQE interaction
    //---------------
    _stdB_ENuRec_CCQE = (Mf2 - p_nMass*p_nMass + 2*p_NucPot*Ef + p_NucPot*p_NucPot) / (2*(p_NucPot + Ef - fMom*cosThetaF));
    
    _stdB_Q2_CCQE = (2*_stdB_ENuRec_CCQE*(Emu - negMomVect.Mag()*_stdB_trCosTheta[negIndex]) - pow(p_muMass,2))/1e6; //GeV
    
    
    _stdB_PQ2_CCQE = (_stdB_ENuRec_CCQE*_stdB_ENuRec_CCQE - _stdB_EProton_CCQE*_stdB_EProton_CCQE + (finalStateMom - posMomVect)*(finalStateMom - posMomVect) + pow(p_nMass-p_NucPot,2)*((2*_stdB_EProton_CCQE)/(p_nMass-p_NucPot) -1) + 2*_stdB_ENuRec_CCQE*(_stdB_EProton_CCQE *_stdB_trCosTheta[posIndex] - _stdB_FinalMom_CCQE*_stdB_FinalCosTheta_CCQE))/1e6;
    
    double cosDeltaThetaP2 = (posDir.Z()*_stdB_ENuRec_CCQE - posDir*negMomVect) / (sqrt(negMom*negMom + _stdB_ENuRec_CCQE*_stdB_ENuRec_CCQE - 2*negMomVect.Z()*_stdB_ENuRec_CCQE));
    //      _stdB_DeltaThetaP[0] = acos(cosDeltaThetaP1);
    _stdB_DeltaThetaP_CCQE = acos(cosDeltaThetaP2);
    
    // neutrino invariant mass
    //------------------------
    TLorentzVector proton4vector(posMomVect, Ep);
    TLorentzVector muon4vector(negMomVect, Emu);
    TLorentzVector neutron4vector(0, 0, 0, p_nMass);
    
    TLorentzVector sum = proton4vector + muon4vector - neutron4vector;
    _stdB_NuInvMass_CCQE = (sum*sum)*1e-6; //GeV^2
  }
  //End (+,-) or (-,+) case
  
  //The 2 most energetic tracks are both negative
  else if(_stdB_trCharge[0] == -1 && _stdB_trCharge[1] == -1){
    _stdB_ENuRec_CCQE = -1;
    _stdB_Q2_CCQE = -1;
    _stdB_PQ2_CCQE = -1;
    _stdB_DeltaThetaP_CCQE = -2;
    _stdB_NuInvMass_CCQE = -1;
  } 
  
  // the 2 most energetic tracks are both positive 
  else{
    _stdB_ENuRec_CCQE = -1;
    _stdB_Q2_CCQE = -1;
    _stdB_PQ2_CCQE = -1;
    _stdB_DeltaThetaP_CCQE = -2;
    _stdB_NuInvMass_CCQE = -1;
  }     
  
}

    
//Fills kinematic information for 1 track, assuming it is a muon
//**********************************************************
void NumuFlatTree_Standard::FillKinematicsFor1Muon(int trCharge, double trMom, double trCosTheta,int itrk){ 
  
  double Emu = 0;
  
  // Check it is negative:
  if(trCharge == -1){
    
    Emu = sqrt(p_muMass*p_muMass + trMom*trMom);
    
    _stdB_trEMuRec_1Muon[itrk] = Emu;
    _stdB_trENuRec_1Muon[itrk] = ((p_pMass*p_pMass - p_muMass*p_muMass + 2*Emu*(p_nMass-p_NucPot) - pow(p_nMass-p_NucPot,2))/(-Emu + (p_nMass-p_NucPot) + (trMom*trCosTheta)))/2;	
    _stdB_trQ2_1Muon[itrk] = (2 * _stdB_trENuRec_1Muon[itrk] * (_stdB_trEMuRec_1Muon[itrk]- trMom * trCosTheta) - p_muMass*p_muMass)/1e6; //GeV
  }
  
  else{
    _stdB_trENuRec_1Muon[itrk] = -1;
    _stdB_trEMuRec_1Muon[itrk] = -1;
    _stdB_trQ2_1Muon[itrk] = -1;
  }
}    

//*************************************************************
//**********************************************************                           
double NumuFlatTree_Standard::GetEnuReweight(double enu){
  
  double weight[20] = {
    1.02943,
    0.979603,
    1.00123,
    1.03182,
    1.03621,
    1.03053,
    1.01495,
    0.997691,
    0.974313,
    0.949989,
    0.906395,
    0.90201,
    0.928674,
    0.959829,
    0.917201,
    0.940934,
    0.92232,
    0.961158,
    0.995986,
    0.982822
  };
  
  int bin = (int)(enu/100);
  
  if(bin < 20 && bin >= 0) return weight[bin];
  
  return 1;
}
  

//Fills information about delayed (Michel) FGD clusters
//**********************************************************
void NumuFlatTree_Standard::FillDelayedClusters(TClonesArray *p_DelayedClusters, int p_NDelayedClusters, double p_EarliestTrackTime, TLorentzVector vtx1, TLorentzVector vtx2){
  
  //Loop over delayed clusters
  for(int j = 0; j < p_NDelayedClusters; j ++){            
    
    ND::TGlobalReconModule::TFgdCluster *cluster = (ND::TGlobalReconModule::TFgdCluster*)(*p_DelayedClusters)[j];

    if(!cluster) continue;
    if(_stdB_NClusters == p_NMAXCLUSTERS) break;
    
    //Take only the final clusters with unused hits
    _stdB_ClusterNHits[_stdB_NClusters] = cluster->NHits;
    
    //Fill 4-position
    for(int i = 0; i < 4; i ++){
      _stdB_ClusterRMS[_stdB_NClusters][i] = cluster->PosRMS[i];
      _stdB_ClusterPos[_stdB_NClusters][i] = cluster->Position[i];
    }
    
    _stdB_ClusterCharge[_stdB_NClusters] = cluster->TotalCharge;
    _stdB_ClusterTimeDistVertex[2][_stdB_NClusters] = cluster->Position.T() - p_EarliestTrackTime;
    _stdB_ClusterTimeDistVertex[0][_stdB_NClusters] = cluster->Position.T() - vtx1.T();
    _stdB_ClusterTimeDistVertex[1][_stdB_NClusters] = cluster->Position.T() - vtx2.T();
    
    TLorentzVector dp1 = cluster->Position - vtx1;
    TLorentzVector dp2 = cluster->Position - vtx2;
    
    //Get the distance between the cluster and the vertex
    if(cluster->Position.X() == 0){
      _stdB_ClusterDistVertex[0][_stdB_NClusters] = sqrt(dp1.Y()*dp1.Y() + dp1.Z()*dp1.Z());
      _stdB_ClusterDistVertex[1][_stdB_NClusters] = sqrt(dp2.Y()*dp2.Y() + dp2.Z()*dp2.Z());
    }
    else if(cluster->Position.Y() == 0){
      _stdB_ClusterDistVertex[0][_stdB_NClusters] = sqrt(dp1.X()*dp1.X() + dp1.Z()*dp1.Z());
      _stdB_ClusterDistVertex[1][_stdB_NClusters] = sqrt(dp2.X()*dp2.X() + dp2.Z()*dp2.Z());
    }
    else{ 
      _stdB_ClusterDistVertex[0][_stdB_NClusters] = sqrt(dp1.X()*dp1.X() + dp1.Y()*dp1.Y() + dp1.Z()*dp1.Z());
      _stdB_ClusterDistVertex[1][_stdB_NClusters] = sqrt(dp2.X()*dp2.X() + dp2.Y()*dp2.Y() + dp2.Z()*dp2.Z());
    }
    
    
    for(int imcut1 = 0; imcut1 < 5; imcut1 ++){
      if(_stdB_ClusterDistVertex[0][_stdB_NClusters] < p_michelCut[imcut1]){
	_stdB_NClusterscut[0][imcut1] ++;
      }
    }
    
    for(int imcut2 = 0; imcut2 < 5; imcut2 ++){
      if(_stdB_ClusterDistVertex[1][_stdB_NClusters] < p_michelCut[imcut2]){
	_stdB_NClusterscut[1][imcut2] ++;
      }
    }      
    
    _stdB_NClusters++;
  }//End loop over clusters
}


//
//**********************************************************
void NumuFlatTree_Standard::FillTrueQ2(){
  double leptonMass = 0;
  double trueCosTheta = 0;
  
  TVector3 nuDir(_std_TrueNuDir[_std_NTrueVertices][0], _std_TrueNuDir[_std_NTrueVertices][1], _std_TrueNuDir[_std_NTrueVertices][2]);
  TVector3 dir(_std_TrueLeptonDir[_std_NTrueVertices][0], _std_TrueLeptonDir[_std_NTrueVertices][1], _std_TrueLeptonDir[_std_NTrueVertices][2]);
  trueCosTheta = (dir*nuDir);  

  //Muon
  if(abs(_std_TrueNuPDG[_std_NTrueVertices]) == 14) leptonMass = 105.6583668;
  //Electron
  else if(abs(_std_TrueNuPDG[_std_NTrueVertices]) == 12) leptonMass = 0.510998910;

  _std_TrueQ2[_std_NTrueVertices] = (2*_std_TrueNuEnergy[_std_NTrueVertices]*(std_trueLeptonEnergy[_std_NTrueVertices] - _std_TrueLeptonMom[_std_NTrueVertices] * trueCosTheta) - pow(leptonMass,2))/1e6; //GeV

}

//
//**********************************************************
void NumuFlatTree_Standard::FillDetectorsUsed(int itrk, int det){ 

  char cdet[50];
  sprintf(cdet,"%d",det);
  string idets[9]={"1", "2", "3", "4", "5", "6", "7", "8", "9"}

  string sdet;
  sdet.assign(cdet);

  for(int d = 0; d < 9; d ++){
    if(sdet.find(idets[d]) != string::npos) _stdB_trDetUsed[itrk][d] = 1;
  }
}

//**********************************************************
