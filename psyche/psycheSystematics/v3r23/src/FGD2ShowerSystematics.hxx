#ifndef FGD2ShowerSystematics_h
#define FGD2ShowerSystematics_h

#include "EventWeightBase.hxx"
#include "BinnedParams.hxx"

class FGD2ShowerSystematics: public EventWeightBase, public BinnedParams {
public:

  FGD2ShowerSystematics(bool computecounters = false);  
  
  
  virtual ~FGD2ShowerSystematics() {}

  /// Apply the systematic
  Weight_h ComputeWeight(const ToyExperiment&, const AnaEventC&, const ToyBoxB&){return 1;}
  
  Weight_h ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box, const SelectionBase& sel);

protected:

  /// Is this track relevant for this systematic ?
  bool IsRelevantRecObject(const AnaEventC& event, const AnaRecObjectC& recObj) const;

  bool  _computecounters;
};

#endif
