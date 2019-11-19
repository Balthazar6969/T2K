#ifndef SandMuonsSystematics_h
#define SandMuonsSystematics_h

#include "EventWeightBase.hxx"
#include "BinnedParams.hxx"

// This systematic should then be applied as a simple weighting factor to all events in the analysis:

/// TN-152 for a longer explanaition

class SandMuonsSystematics: public EventWeightBase, public BinnedParams {
public:
  
  SandMuonsSystematics();
  
  virtual ~SandMuonsSystematics() {}
  
  /// Apply this systematic
  using EventWeightBase::ComputeWeight;
  Weight_h ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box);


};

#endif
