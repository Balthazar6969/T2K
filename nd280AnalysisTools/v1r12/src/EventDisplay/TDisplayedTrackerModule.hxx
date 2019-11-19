#ifndef TDISPLAYEDTrackerMODULE_HXX
#define TDISPLAYEDTrackerMODULE_HXX
#include "TChain.h"
#include "TClonesArray.h"
#include "TDisplayedAnalysisModuleBase.hxx"
#include "TEveBoxSet.h"
#include "TND280Track.hxx"
#include "TND280TrackList.hxx"
#include "TND280TrackPropagator.hxx"
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TTrackerAnalysisUtils.hxx"
#endif
/// Display the results from the tracker module.
/// \author Alex Finch (A.Finch@lancaster.ac.uk)
class TDisplayedTrackerModule : public TDisplayedAnalysisModuleBase {

public:
  TDisplayedTrackerModule();
  /// Draw entry , actually load it into Eve's lists.
  void Draw(int entry, int bunch = -2);
  /// Describe a TTrackerResult on standard output.
  void Describe(ND::TTrackerReconModule::TTrackerResult *RO);
  /// Describe a TTrackerConstituent on standard output.
  void Describe(ND::TTrackerReconModule::TTrackerConstituent *RO,
                TClonesArray *Constituents=NULL);
  /// Describe an array of objectTypes on standard output.
  template <class objectType>
  void DescribeArray(TClonesArray *Constituents, Int_t NConstituents);
  template <class objectType>
  TND280TrackList *DrawArray(TClonesArray *Constituents, Int_t NConstituents);
  /// Describe a tpc track on standard output.
  void Describe(ND::TTrackerReconModule::TTPCTrack *RO);
  /// Describe a fgd track on standard output.
  void Describe(ND::TTrackerReconModule::TFGDTrack *RO);
  /// Return an eve element representing a TTrackerConstituent
  TND280Track *Draw(ND::TTrackerReconModule::TTrackerConstituent *RO, int index,
                    TClonesArray *Constituents);
  /// Return an eve element representing a TTPCTrack
  TND280Track *Draw(ND::TTrackerReconModule::TTPCTrack *RO, int index);
  /// Return an eve element representing a TFGDTrack
  TND280Track *Draw(ND::TTrackerReconModule::TFGDTrack *RO, int index);

private:
  /// Helper function creating TND280Track from T2K Tracker result.
  TND280Track *MakeTrack(TND280TrackPropagator *trkProp, Int_t index,
                         ND::TTrackerReconModule::TTrackerResult *result);
  /// Helper function creating TND280Track from T2K Tracker constituent.
  TND280Track *MakeTrack(TND280TrackPropagator *trkProp, Int_t index,
                         ND::TTrackerReconModule::TTrackerConstituent *result);
  TND280Track *MakeTrack(TND280TrackPropagator *trkProp, Int_t index,
                         ND::TTrackerReconModule::TTPCTrack *track);
  TND280Track *MakeTrack(TND280TrackPropagator *trkProp, Int_t index,
                         ND::TTrackerReconModule::TFGDTrack *track);
  TEveTrack *make_track(TND280TrackPropagator *prop, Int_t sign);
  TND280TrackPropagator *fTrkProp;
};
#endif
