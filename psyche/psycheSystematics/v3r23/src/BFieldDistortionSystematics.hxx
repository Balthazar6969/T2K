#ifndef BFieldDistortionSystematics_h
#define BFieldDistortionSystematics_h

#include "EventVariationBase.hxx"
#include "ToyExperiment.hxx"
#include "BinnedParams.hxx"

/// This is the BField distortions systematic
///

class BFieldDistortionSystematics: public EventVariationBase, public BinnedParams {
public:
  
  /// Instantiate the momentum resolution systematic. nbins is the number of
  /// bins in the PDF. addResol and addResolError describe
  /// the Gaussian distribution from which the resolution of each virtual
  /// analysis is selected from.
  BFieldDistortionSystematics();
  
  virtual ~BFieldDistortionSystematics() {}
  
  /// Apply the systematic
  virtual void Apply(const ToyExperiment& toy, AnaEventC& event);
  
  /// Undo  the systematic variations done by ApplyVariation. This is faster tha reseting the full Spill
  virtual bool UndoSystematic(AnaEventC& event);
  
  
protected:

  /// Update momentum given the momentum to change and the corresponding TPC
  /// track
  void VaryMomentum(Float_t& Momentum, const AnaTPCParticleB* tpcTrack, const ToyExperiment& toy) const;

protected:
  
  /// The mean error of the systematic source 
  Float_t _mean_error;
};

#endif
