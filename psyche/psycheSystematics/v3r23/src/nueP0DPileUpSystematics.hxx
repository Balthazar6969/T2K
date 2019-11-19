#ifndef nueP0DPileUpSystematics_h
#define nueP0DPileUpSystematics_h

#include "EventWeightBase.hxx"
#include "BinnedParams.hxx"

class nueP0DPileUpSystematics: public EventWeightBase {
public:
  
  nueP0DPileUpSystematics();
  
  virtual ~nueP0DPileUpSystematics() {
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
