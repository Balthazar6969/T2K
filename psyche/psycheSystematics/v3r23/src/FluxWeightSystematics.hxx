#ifndef FluxWeightSystematics_h
#define FluxWeightSystematics_h

#include "EventWeightBase.hxx"
#include "BinnedParams.hxx"
#include "FluxWeighting.hxx"

/// This is a normalization systematic. It takes into account the uncertainty on the neutrino flux

class FluxWeightSystematics: public EventWeightBase, public BinnedParams {
  public:

    FluxWeightSystematics();  
    virtual ~FluxWeightSystematics() {}

    /// Apply this systematic
    using EventWeightBase::ComputeWeight;
    Weight_h ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box);
    
protected:
  /// Access to the flux weighting.
  FluxWeighting* _flux;

};


#endif
