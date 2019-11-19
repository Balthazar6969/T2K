#ifndef TPCP0DMatchEffSystematics_h
#define TPCP0DMatchEffSystematics_h

#include "EventWeightBase.hxx"
#include "BinnedParams.hxx"


class TPCP0DMatchEffSystematics: public EventWeightBase, public BinnedParams {
public:

  TPCP0DMatchEffSystematics(bool computecounters=false);  
  
  virtual ~TPCP0DMatchEffSystematics() {}

  /// Apply the systematic
  Weight_h ComputeWeight(const ToyExperiment&, const AnaEventC&, const ToyBoxB&){return 1;}
  
  Weight_h ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box, const SelectionBase& sel);
  
protected:
  
  /// Is this track relevant for this systematic ?
  bool IsRelevantRecObject(const AnaEventC& event, const AnaRecObjectC& track) const;

  /// Is this track relevant for this systematic ?
  bool IsRelevantTrueObject(const AnaEventC& event, const AnaTrueObjectC& track) const;

  bool  _computecounters;
  
  /// Graphs relevant for interpolation business
  /// assume that is the last bin we interpolate through
  
};

#endif
