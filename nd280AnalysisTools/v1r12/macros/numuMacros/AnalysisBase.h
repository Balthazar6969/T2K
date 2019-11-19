#ifndef AnalysisBase_h
#define AnalysisBase_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <map>
#include <vector>
#include "TrackCategory.h"
#include "DrawingTools.h"
#include "NumuFlatTreeTools.h"
#include "SystematicsAnalysis.h"

/* This class is a base class for analysis. 

   It contains: 
   - the event loop 
   - the loop over configurations and analyses for 
   systematic or cut optimisation studies.  
   
   - The definition of the stundard cuts
   - The definition of the PID likelihood
   
   - Some utility functions

*/

const int NMAXCUTS=20;

class AnalysisBase: public SystematicsAnalysis {
 public :
  
  AnalysisBase(){}
  AnalysisBase(const std::string& file);
  virtual ~AnalysisBase(){}

  // ---- Initialize functions -------------
  virtual void Initialize();
  virtual void InitializeEvent();
  virtual void InitializeConfiguration();
  virtual void InitializeAnalysis();

  // ---- Finalize functons -------------
  virtual void Finalize();
  virtual void FinalizeEvent(){}
  virtual void FinalizeConfiguration();
  virtual void FinalizeAnalysis();

  // ---- Fuction called for each event
  virtual bool Process(){return false;}

  // ---- Main event loop --------------
  void Loop(int nmax=0,int imin=0);

  
  //---- cut definitions ------
  
  bool FiducialCut(std::string det, double *pos);
  bool InDetVolume(std::string det, double *pos);

  bool MuonPIDCutPull(int itrk);
  bool MuonPIDCutLike(int itrk);
  bool ProtonPIDCutPull(int itrk);
  bool ProtonPIDCutLike(int itrk);
  virtual bool TrackQualityCut(int itrk);
  bool CommonVertexCut(int itrk1, int itrk2);
  //bool ExternalVetoCut(int itrk,bool qualityCut=true);
  bool ExternalVetoCut(int itrk);
  bool DeltaZCut(int itrk);

  //---- PID likelihood ----------

  double GetPIDLikelihood(int itrk, int hypo);
  double GetPIDPrior(int itrk, int hypo);

  //----- Utility functions -------------

  double GetTrueCosTheta(int itrk);
  double GetRecCosTheta(int itrk);
  double GetDeltaCosTheta(int itrk);
  int GetNSegmentsInDet(int itrk, const std::string& det);
  int GetReaction(int itrk);
  int GetClosestTPC(int itrk);
  bool FindLeadingTracks(int& HM_track, int& HMN_track, int& HMP_track, bool qualityCut=true);

  int GetNHitsInClosestTpc(int itrk);
  double GetMomentumInClosestTpc(int itrk);
  double GetMomentumRelErrorInClosestTpc(int itrk);

  double GetPtLocal(int itrk);
  double GetSigmaPtLocal(int itrk, int tpc);
  double GetSigmaInvPtLocal(int itrk, int tpc);
  double GetTruePt(int itrk);

  void CutPassed(int icut);
  bool GetCutPassed(int icut){return _cut_passed[icut];}
  int GetAccumCutLevel(){return _accum_cut_level;}

  // Fill the track categories for color drawing
  void FillCategories(int itrk, TrackCategory& track);


 protected:

  std::vector<bool> _like_computed;
  std::vector<std::vector<double> > _likelihood;

  // index of highes momentum tracks (inclusive, negative and positive)
  int _HM_track;
  int _HMN_track;
  int _HMP_track;

  // track category (for color drawing) of highest momentum tracks
  TrackCategory _HM_TrackCategory;
  TrackCategory _HMN_TrackCategory;
  TrackCategory _HMP_TrackCategory;

  bool _cut_passed[NMAXCUTS];
  int _accum_cut_level;

};

#endif
