#ifndef ECalPIDSystematics_h
#define ECalPIDSystematics_h

#include "EventWeightBase.hxx"
#include "BinnedParams.hxx"

class ECalPIDSystematics: public EventWeightBase, public BinnedParams {
public:

  ECalPIDSystematics(bool computecounters=false);  
  
  
  virtual ~ECalPIDSystematics() {}

  /// Apply the systematic
  Weight_h ComputeWeight(const ToyExperiment&, const AnaEventC&, const ToyBoxB&){
    return 1.;  
  }

  /// Apply the systematic
  Weight_h ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box, const SelectionBase& sel);


protected:

  /// Is this track relevant for this systematic ?
  bool IsRelevantRecObject(const AnaEventC& event, const AnaRecObjectC& recObj) const;
   
  bool  _computecounters;
};

#endif
