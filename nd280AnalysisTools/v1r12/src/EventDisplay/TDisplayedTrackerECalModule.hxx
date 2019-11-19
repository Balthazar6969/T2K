#ifndef TDisplayedTrackerECalMODULE_HXX
#define TDisplayedTrackerECalMODULE_HXX
#include "TChain.h"
#include "TEveBox.h"
#include "TEveBoxSet.h"

#include "TND280Track.hxx"
#include "TND280TrackPropagator.hxx"

#include "TDisplayedAnalysisModuleBase.hxx"
#include "TGlobalAnalysisUtils.hxx"

#ifdef TTRACKERECALRECONMODULE_USE_EXP
#include "TTrackerECALReconModuleTools.hxx"
#endif

/// Class to display information from the Tracker Ecal module in event display.
/// \author    Alex Finch (A.Finch@lancaster.ac.uk)
class TDisplayedTrackerECalModule : public TDisplayedAnalysisModuleBase {
 public:
  TDisplayedTrackerECalModule();
  /// Draw P0D Ecal Module information in event `entry', and bunch 'bunch'.
  void Draw(int entry, int bunch = -2);
  /// Describe track number index on standard output.
  void DescribeTrack(int index);
  /// Describe shower number index on standard output.
  void DescribeShower(TString Name);
  /// Describe Track on standard output.
  void Describe(ND::TTrackerECALReconModule::TECALReconTrack *Track);
  /// Describe Shower on standard output.
  void Describe(ND::TTrackerECALReconModule::TECALReconShower *Shower);

  /// Threshold used discrimintate between MIP and EM shower when drawing.
  double MIP_EM_Thresh;
  /// Threshold used discrimintate between MIP and showering pion when drawing.
  double MIP_Pion_Thresh;
  ///\brief Threshold used discrimintate between EM shower and highly
  /// ionising hadrons when drawing.
  double EM_HIP_Thresh;

 private:
  /// Construct a TND280Track to represent a TECALReconTrack.
  /// index is used to name it and trkProp is the propogator used to draw it.
  TND280Track *MakeTrack(
      TND280TrackPropagator *trkProp, Int_t index,
      ND::TTrackerECALReconModule::TECALReconTrack *reconObject);
#ifdef TTRACKERECALRECONMODULE_USE_EXP
  TEveBox *MakeVertex(
      ND::TTrackerECALReconModule::TECALReconVertexCandidate const &VC,
      int i = 0);
  TND280Track *MakeTrack(
      TND280TrackPropagator *trkProp, Int_t index,
      ND::TTrackerECALReconModule::TECALReconVertexTrack const &VT);
#endif
  /// Describe a TTrackerECALReconModule::TECALReconObject on standard output.
  void Describe(ND::TTrackerECALReconModule::TECALReconObject *reconObject);

#ifdef TTRACKERECALRECONMODULE_USE_EXP
  TTrackerECALReconModuleTools ECALUtil;

  Int_t fNUsedHits;
  Int_t fNVertexCandidates;
  Int_t fNVertexCandidateTracks;
  TClonesArray *fUsedHits;
  TClonesArray *fVertexCandidates;
  TClonesArray *fVertexCandidateTracks;
#endif
};
#endif
