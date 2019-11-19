#ifndef TrackerSelectionUtils_h
#define TrackerSelectionUtils_h

#include "CutUtils.hxx"
#include "ToyBoxTracker.hxx"
#include "EventBoxId.hxx"

namespace trackerSelUtils{

  /// Fill the analysis box. Only tracks with TPC information are
  /// considered. If qualityCut is true, then tracks must pass the
  /// TrackQualityCut function. Tracks must also start in the specified volume -
  /// FGD, FGD1, FGD2, and P0D are currently supported.
  ///
  /// The following members of the box are filled:
  /// * HMtrack............. The highest-momentum track.
  /// * HMNtrack............ The highest-momentum negative track.
  /// * HMPtrack............ The highest-momentum positive track.
  /// * NegativeTPCtracks... All the negative tracks, sorted by momentum.
  /// * PositiveTPCtracks... All the positive tracks, sorted by momentum.
  ///
  /// Again, only tracks with TPC information are considered.
  DEPRECATED(bool FindLeadingTracksOld(AnaEventC& event, ToyBoxB& ToyBox, bool qualityCut = true, SubDetId::SubDetEnum det = SubDetId::kFGD, bool applyFiducialCut = true));
  bool FindLeadingTracks(const AnaEventC& event, ToyBoxB& ToyBox);
  
  /// Find leading tracks given a collection of objects, the variables same as
  /// above are filled 
  bool FindLeadingTracks(AnaRecObjectC** objects, size_t nObjects, ToyBoxB& ToyBox);
}

#endif
