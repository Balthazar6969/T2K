#ifndef NumuFlatTree_Standard_h
#define NumuFlatTree_Standard_h

//***********************************************************************
//Include file for NumuFlatTree_Standard.C
//
// Syntax info:
// Variables defined in this class must have "std" attached to them
//
// Definition of Tree variables:
// must start with _std_  for spill variables    
// must start with _stdB_ for bunch variables    
//
// Global variables that are not copied to the numutree start with std_
//
// Internal variables to the class can be named freely
//***********************************************************************
#include "ProcessNumuFlatTree.h"
#include "NumuFlatTree_GeneralUtils.h"

class NumuFlatTree_Standard{
  
 private:

  // Using General utils class:
    NumuFlatTree_GeneralUtils *utils;

  
  // If you are using one of this variables outside of the standard module
  // please change its name to std_name and move it to public section
  //___________________________________________________________________

  int haveTPC1track ;
  bool savespillonce ;
  double max_mom ;
  int max_mom_bunch ;  

  int NMaxNegReconTrack;
  int NMaxPosReconTrack;
  int NMaxRecTrack;
  int NMaxFgdTrack;
  int NNegReconTrack[p_NBUNCH];
  int NPosReconTrack[p_NBUNCH];
  int NGlobalTracks_total[p_NBUNCH];
  int NTpc1RecTrack[p_NBUNCH];
  int NTpc2RecTrack[p_NBUNCH];
  int NTpc3RecTrack[p_NBUNCH];
  int NTpc1RecTrack_total;
  int NTpc2RecTrack_total;
  int NTpc3RecTrack_total;
  int nvtxPerBunch[p_NBUNCH];

  //Containers for momentum sorting                                                                                        
  vector<int> global_index_usort[p_NBUNCH];                                                                                  
  vector<double> momentum_usort[p_NBUNCH]; //unsorted momentum                                     
  vector<int> fgd_index_usort[p_NBUNCH];                                                                                          
  vector<double> fgd_nhits_usort[p_NBUNCH]; //unsorted nhits        


 public :

  // Variable mainly linked to the Standard Module, but not tree variables

  int std_nPrimTraj;
  int std_truePrimaryID[p_NMAXTRUEVERTICES][p_NMAXTRAJ];
  
  
  int std_NFgdTrack[p_NBUNCH];
  int std_NRecTrack[p_NBUNCH];
  int std_NRecTrack_total;

  double std_trueLeptonEnergy[p_NMAXTRUEVERTICES];

  // Definition of Tree variables: 
  // must start with _std_  for spill variables
  // must start with _stdB_ for bunch variables
  //_______________________________________________________________________
  
  //Filled for each spill
  int _std_TrueSpillInfo;
  int _std_Evt;
  int _std_Run;
  int _std_FileRun;
  int _std_FileSubrun;
  int _std_Entry;
  int _std_NBunches;
  int _std_NValidBunches;
  int _std_BunchNbr;
  int _std_SpillWithTPC1Track;
  int _std_NTrueVertices; 
  int _std_ReacCode[p_NMAXTRUEVERTICES];
  double _std_TrueVertexPos[p_NMAXTRUEVERTICES][4];
  double _std_TrueNuEnergy[p_NMAXTRUEVERTICES];
  double _std_TrueNuDir[p_NMAXTRUEVERTICES][3];
  int    _std_TrueNuPDG[p_NMAXTRUEVERTICES];
  double _std_TrueEnuWeight[p_NMAXTRUEVERTICES];
  int _std_TrueVertexBunch[p_NMAXTRUEVERTICES];
  double _std_TrueLeptonMom[p_NMAXTRUEVERTICES];
  double _std_TrueProtonMom[p_NMAXTRUEVERTICES];
  double _std_TrueTargetMom[p_NMAXTRUEVERTICES];
  double _std_TrueLeptonDir[p_NMAXTRUEVERTICES][3];
  double _std_TrueProtonDir[p_NMAXTRUEVERTICES][3];
  double _std_TrueTargetDir[p_NMAXTRUEVERTICES][3];
  double _std_TrueTargetPDG[p_NMAXTRUEVERTICES];
  int _std_TrueBaryonSize[p_NMAXTRUEVERTICES];
  double _std_TrueQ2[p_NMAXTRUEVERTICES];
  int _std_TrueVertexDet[p_NMAXTRUEVERTICES];
  int _std_NECALTracks;
  int _std_NP0DTracks;
  int _std_NSMRDTracks;
  int _std_NFGDTracks;
  int _std_NTPCTracks;
  int _std_NFGDUnusedHits[2];
  int _std_NTracksTotal;
  int _std_NTpcTracksTotal[3];
  
  //Filled for each bunch syntax: _stdB_
  int _stdB_BunchWithMaxMomInSpill;
  double _stdB_TBunch;
  int _stdB_Bunch;
  int _stdB_NTracksWithTPC;
  double _stdB_Vertex[4][4];
  int _stdB_nTrueVerticesPerBunch;
  int _stdB_NTracksGlobal;
  int _stdB_NPosTracksWithTPC;
  int _stdB_NNegTracksWithTPC;
  int _stdB_NTPCTracks[3];
  int _stdB_trDetectors[p_NMAXTRACKS];
  int _stdB_trDetUsed[p_NMAXTRACKS][9];
  int _stdB_trNHits[p_NMAXTRACKS];
  double _stdB_trChi2[p_NMAXTRACKS];
  double _stdB_trPositionStart[p_NMAXTRACKS][4];
  double _stdB_trPositionEnd[p_NMAXTRACKS][4];
  double _stdB_trDir[p_NMAXTRACKS][3];
  double _stdB_trMom[p_NMAXTRACKS];
  double _stdB_trMomError[p_NMAXTRACKS];
  double _stdB_trMomRelError[p_NMAXTRACKS];
  int    _stdB_trCharge[p_NMAXTRACKS];
  double _stdB_trCosTheta[p_NMAXTRACKS];
  double _stdB_trECALTrShVal[p_NMAXTRACKS];
  double _stdB_trEnuWeight[p_NMAXTRACKS];
  int _stdB_trNFgds[p_NMAXTRACKS];
  int _stdB_trTpcDet[p_NMAXTRACKS][3];
  double _stdB_trCT[p_NMAXTRACKS][3];
  double _stdB_trPull[p_NHYPO][p_NMAXTRACKS][3];
  double _stdB_trCTexp[p_NHYPO][p_NMAXTRACKS][3];
  double _stdB_trCTerr[p_NHYPO][p_NMAXTRACKS][3];
  double _stdB_trTpcCharge[p_NMAXTRACKS][3];
  double _stdB_trTpcMom[p_NMAXTRACKS][3];
  double _stdB_trTpcMomRelError[p_NMAXTRACKS][3];
  int _stdB_trTpcNHits[p_NMAXTRACKS][3];
  
  int _stdB_NFgdTracksBunch;
  int _stdB_fgdtr_Det[p_NMAXTRACKS];
  int _stdB_fgdtr_NHits[p_NMAXTRACKS];
  double _stdB_fgdtr_Chi2[p_NMAXTRACKS];
  double _stdB_fgdtr_PositionStart[p_NMAXTRACKS][4];
  double _stdB_fgdtr_PositionEnd[p_NMAXTRACKS][4];
  double _stdB_fgdtr_Dir[p_NMAXTRACKS][3];
  
  
  double _stdB_trENuRec_1Muon[p_NMAXTRACKS];
  double _stdB_trEMuRec_1Muon[p_NMAXTRACKS];
  double _stdB_trQ2_1Muon[p_NMAXTRACKS];
  double _stdB_ENuRec_CCQE;
  double _stdB_Q2_CCQE;
  double _stdB_PQ2_CCQE  ;
  double _stdB_DeltaThetaP_CCQE;
  double _stdB_NuInvMass_CCQE;
  double _stdB_EProton_CCQE;
  double _stdB_FinalCosTheta_CCQE;
  double _stdB_FinalMass_CCQE;
  double _stdB_FinalEnergy_CCQE;
  double _stdB_FinalMom_CCQE;
  
  double _stdB_EarliestTrackTime;
  int _stdB_NClusters;
  int _stdB_NClusterscut[3][5];
  double _stdB_ClusterPos[p_NMAXCLUSTERS][4];
  double _stdB_ClusterRMS[p_NMAXCLUSTERS][4];
  int    _stdB_ClusterNHits[p_NMAXCLUSTERS];
  double _stdB_ClusterCharge[p_NMAXCLUSTERS];
  double _stdB_ClusterDistVertex[p_NMAXCLUSTERS][3];
  double _stdB_ClusterTimeDistVertex[p_NMAXCLUSTERS][3];
  
  int _stdB_trTruePDG[p_NMAXTRACKS];
  int _stdB_trTrueParentPDG[p_NMAXTRACKS];
  int _stdB_trTrueGParentPDG[p_NMAXTRACKS];
  double _stdB_trTruePur[p_NMAXTRACKS];
  double _stdB_trTruePos[p_NMAXTRACKS][4];
  double _stdB_trTrueDir[p_NMAXTRACKS][3];
  double _stdB_trTrueMom[p_NMAXTRACKS];
  int _stdB_trTrueCharge[p_NMAXTRACKS];
  int _stdB_trTrueVertexIndex[p_NMAXTRACKS];
    
  
  
//_________________________________________________________________
// Define methods used in Standard class

 NumuFlatTree_Standard(NumuFlatTree_GeneralUtils *utils);

 // Mandatory methods for all modules:                                                                                                          
 void DefineTree();
 void ResetSpillVariables();
 void ResetBunchVariables();
 void SetBranchAddress(TTree *tree);
 

 // Other useful methods related to standard module:                                   
 void FillOAInfo(); // Copy variables straight from oaAnlaysis (very basic right now)  
 void InitializeInternalVariables();
 void FillVertexMCInfo();
 int PrepareVariables(int ibunch, int ipid);
 void FillBunchVariables(int ibunch);
 void FillBunchVariablesFromPIDs(int ibunch);
 
 // Other methods:
 void PrepareBunchWithMaxMomInSpill();
 void FillNTracksSpillInfo();
 void FillKinematicsCCQE();
 void FillKinematicsFor1Muon(int trCharge, double trMom, double trCosTheta,int itrk);
 double GetEnuReweight(double enu);         
 void FillDelayedClusters(TClonesArray *DelayedClusters, int NDelayedClusters, 
			   double EarliestTrackTime, TLorentzVector vtx1, 
			   TLorentzVector vtx2);
 
  void FillTrueQ2();
  void FillDetectorsUsed(int itrk, int det); 
};
#endif
