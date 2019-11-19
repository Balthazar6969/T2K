#ifndef TPCTrackEffSystematics_h
#define TPCTrackEffSystematics_h

#include "EventWeightBase.hxx"
#include "ToyExperiment.hxx"
#include "BinnedParams.hxx"

/// This systematic smears the momentum of tracks with TPC information, by a
/// random amount from a Gaussian distribution.
///
/// The amount each track is changed by uses two Gaussian distributions.
/// The first distribution uses the input parameters to select a resolution.
/// This is called once for each virtual analysis. The second distribution
/// then uses the chosen resolution as its width, and the amount to vary each
/// track's momentum is chosen from this distribution. This is called for each
/// track individually.
class TPCTrackEffSystematics: public EventWeightBase, public BinnedParams {
public:

  /// this is only 1 toy experiment, as we only store the weight for a change of one sigma.
  /// The mean and sigma for each momentum bin is defined in a data file.
    /// the Gaussian distribution from which the resolution of each virtual
    /// analysis is selected from.
  TPCTrackEffSystematics(bool computecounters=false);

  virtual ~TPCTrackEffSystematics() {}

  Weight_h ComputeWeight(const ToyExperiment&, const AnaEventC&, const ToyBoxB&){return 1;}
  Weight_h ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box, const SelectionBase& sel);

protected:

  /// Is this true track relevant for this systematic ?
  bool IsRelevantTrueObject(const AnaEventC& event, const AnaTrueObjectC& trueTrack) const;

  /// Check the true-reco association
  bool CheckTrueRecoAssociation(const AnaTrueObjectC& trueTrack, const AnaRecObjectC& track) const;

protected:

  /// The systematic source parameters
  bool  _computecounters;

  /// value of psycheSystematics.Tracker.FullCorrelations parameter
  bool _full_correlations;

};

#endif
