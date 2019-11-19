#ifndef FGDECalMatchEffSystematics_h
#define FGDECalMatchEffSystematics_h

#include "EventWeightBase.hxx"
#include "BinnedParams.hxx"

class FGDECalMatchEffSystematics: public EventWeightBase, public BinnedParams {
public:

  FGDECalMatchEffSystematics(bool computecounters=false);  
  FGDECalMatchEffSystematics(bool computecounters, const std::string& name, BinnedParams::TypeEnum type);
  
  
  virtual ~FGDECalMatchEffSystematics() {}

  /// Apply the systematic
  Weight_h ComputeWeight(const ToyExperiment&, const AnaEventC&, const ToyBoxB&){return 1;}
  
  Weight_h ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box, const SelectionBase& sel);


protected:

  /// Is this track relevant for this systematic ?
  bool IsRelevantRecObject(const AnaEventC& event, const AnaRecObjectC& track) const;

  /// Is this track relevant for this systematic ?
  bool IsRelevantTrueObject(const AnaEventC& event, const AnaTrueObjectC& track) const;
   
  bool  _computecounters;
};

#endif
