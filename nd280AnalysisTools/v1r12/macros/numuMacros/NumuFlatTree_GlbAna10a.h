#ifndef NumuFlatTree_GlbAna10a_h
#define NumuFlatTree_GlbAna10a_h
#include "NumuFlatTree_GeneralUtils.h"
#include "NumuFlatTree_Standard.h"

class NumuFlatTree_GlbAna10a{

 private :

  // Using General utils class:   
  NumuFlatTree_Standard *std_mod; 
  NumuFlatTree_GeneralUtils *utils;
  
  // Internal variables
  
  int g10aB_NTPCTracksCut[p_NBUNCH][3];
  int g10aB_NTPCTracksPosCut[p_NBUNCH][3];
  int g10aB_NTPCTracksNegCut[p_NBUNCH][3];
  
  double  g10a_trMom[p_NMAXTRACKS];
  int     g10a_trCharge[p_NMAXTRACKS];
  TLorentzVector  g10a_trPosStart;
  TLorentzVector  g10a_trPosEnd;
  TVector3 g10a_trDir[p_NMAXTRACKS];
  double g10a_trCosTheta[p_NMAXTRACKS];

  //Containers for momentum sorting                                    
  vector<int> g10a_global_index_usort[p_NBUNCH];
  vector<double> g10a_momentum_usort[p_NBUNCH]; //unsorted momentum         

 public :
  
  // _numutree variables:
  
  //Bunch (g10aB)
  double _g10aB_trCTCorr[p_NMAXTRACKS][3];
  double _g10aB_trProb[p_NHYPO][p_NMAXTRACKS];
  double _g10aB_trPullCorr[p_NHYPO][p_NMAXTRACKS][3];
  double _g10aB_trCTexpCorr[p_NHYPO][p_NMAXTRACKS][3];
  
  int _g10aB_NTPCTracksCut[p_NBUNCH][3];
  int _g10aB_NTPCTracksPosCut[p_NBUNCH][3];
  int _g10aB_NTPCTracksNegCut[p_NBUNCH][3];
 
  // Kinematics variable specific to 2010a analysis
  double _g10aB_ENuRec[p_NOPTIONS];
  double _g10aB_EMuRec[p_NOPTIONS];
  double _g10aB_PQ2[p_NOPTIONS];
  double _g10aB_MuQ2[p_NOPTIONS];
  double _g10aB_DeltaThetaP[p_NOPTIONS];
  double _g10aB_NuInvMass;
  double _g10aB_EProton;
  double _g10aB_FinalCosTheta;
  double _g10aB_FinalMass;
  double _g10aB_FinalEnergy;
  double _g10aB_FinalMom;

 

  // Start Defining Methods
  
  NumuFlatTree_GlbAna10a(NumuFlatTree_Standard *std_mod1);
  
  // Mandatory methods for all modules: 
  void DefineTree();                       
  void SetBranchAddress(TTree *numutree);                       
  void ResetSpillVariables();
  void ResetBunchVariables();     

  void FillTPCTracksCounters();
  void FillBunchVariablesFromPIDs(int ibunch); 

  // Other methods
  void ApplyPIDCorrections(int run);
  void ComputePIDProbabilities();
  void PrepareTPCTracksCounters(int ibunch, int ipid);
  void FillTwoTracksVariables(bool track1, bool track2);

};
#endif
