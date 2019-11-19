#ifndef ECalEmHipPIDSystematics_h
#define ECalEmHipPIDSystematics_h

#include "EventWeightBase.hxx"
#include "BinnedParams.hxx"

class ECalEmHipPIDSystematics: public EventWeightBase, public BinnedParams {
public:

  ECalEmHipPIDSystematics(bool computecounters=false);  
  
  
  virtual ~ECalEmHipPIDSystematics() {}

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
