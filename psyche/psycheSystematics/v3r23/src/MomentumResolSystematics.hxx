#ifndef MomentumResolSystematics_h
#define MomentumResolSystematics_h

#include "EventVariationBase.hxx"
#include "MomentumResolVariation.hxx"

/// This systematic smears the momentum of tracks with TPC information, by a
/// random amount from a Gaussian distribution.
///
/// The amount each track is changed by uses two Gaussian distributions.
/// The first distribution uses the input parameters to select a resolution.
/// This is called once for each virtual analysis. The second distribution
/// then uses the chosen resolution as its width, and the amount to vary each
/// track's momentum is chosen from this distribution. This is called for each
/// track individually.
class MomentumResolSystematics: public EventVariationBase, public MomentumResolVariation {
public:
  
  /// Instantiate the momentum resolution systematic. nbins is the number of
  /// bins in the PDF. addResol and addResolError describe
  /// the Gaussian distribution from which the resolution of each virtual
  /// analysis is selected from.
  MomentumResolSystematics();
  
  virtual ~MomentumResolSystematics() {}
  
  /// Apply the systematic
  virtual void Apply(const ToyExperiment& toy, AnaEventC& event);

  /// Get the variation given a track
  bool GetVariation(AnaTrackB* track, Float_t& variation, const ToyExperiment& exp);
 
  /// Get the variation given a track
  bool GetVariationTPC(AnaTPCParticleB* track, Float_t& variation, const ToyExperiment& exp);
  
  
  /// Undo  the systematic variations done by ApplyVariation. This is faster tha reseting the full Spill
  virtual bool UndoSystematic(AnaEventC& event);  


protected:

  /// Value of psycheSystematics.Tracker.FullCorrelations parameter
  bool _full_correlations;
  
  /// Whether to do the smearing based on the local TPC mom
  bool _tpc_based_var;

  bool _useP0DFV;

};

#endif
