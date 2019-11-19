#ifndef FGDECalSMRDMatchEffSystematics_h
#define FGDECalSMRDMatchEffSystematics_h

#include "FGDECalMatchEffSystematics.hxx"

class FGDECalSMRDMatchEffSystematics: public FGDECalMatchEffSystematics {
public:

  FGDECalSMRDMatchEffSystematics(bool computecounters=false);  
  virtual ~FGDECalSMRDMatchEffSystematics() {}

  /// Apply the systematic
  Weight_h ComputeWeight(const ToyExperiment&, const AnaEventC&, const ToyBoxB&){return 1;}
  
  Weight_h ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box, const SelectionBase& sel);

protected:

  /// Is this track relevant for this systematic ?
  bool IsRelevantRecObject(const AnaEventC& event, const AnaRecObjectC& track) const;

  /// Is this track relevant for this systematic ?
  bool IsRelevantTrueObject(const AnaEventC& event, const AnaTrueObjectC& track) const;

};

#endif
