#ifndef nueTPCPileUpSystematics_h
#define nueTPCPileUpSystematics_h

#include "EventWeightBase.hxx"
#include "BinnedParams.hxx"

class nueTPCPileUpSystematics: public EventWeightBase {
public:
  
  nueTPCPileUpSystematics();
  
  virtual ~nueTPCPileUpSystematics() {
    if (_fgd1) delete _fgd1; _fgd1 = NULL; 
    if (_fgd2) delete _fgd2; _fgd2 = NULL; 
  }
  
  /// Apply this systematic
  using EventWeightBase::ComputeWeight;
  Weight_h ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box);

  void Initialize();
  
  BinnedParams* _fgd1;
  BinnedParams* _fgd2;

};

#endif
