#ifndef P0dVetoSystematics_h
#define P0dVetoSystematics_h

#include "EventWeightBase.hxx"
#include "BinnedParams.hxx"

//P0D veto correction & systemtic based on cut defined in TN-080
//Note: 2016/07/15 - Do not apply to p0dNumuCCQEAnalysis since the 
//current pileup correction covers this already. This should be updated soon

class P0DVetoSystematics: public EventWeightBase {
public:
  
  P0DVetoSystematics();
  
  virtual ~P0DVetoSystematics() {
    if (_p0d) delete _p0d; _p0d = NULL; 

  }
  
  /// Apply this systematic
  using EventWeightBase::ComputeWeight;
  Weight_h ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box);

  BinnedParams* _p0d;
  

};

#endif
