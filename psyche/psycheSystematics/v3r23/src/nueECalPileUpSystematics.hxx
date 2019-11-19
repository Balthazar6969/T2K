#ifndef nueECalPileUpSystematics_h
#define nueECalPileUpSystematics_h

#include "EventWeightBase.hxx"
#include "BinnedParams.hxx"

class nueECalPileUpSystematics: public EventWeightBase {
public:
  
  nueECalPileUpSystematics();
  
  virtual ~nueECalPileUpSystematics() {
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
