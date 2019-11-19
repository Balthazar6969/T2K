#ifndef ECalTrackEffSystematics_h
#define ECalTrackEffSystematics_h

#include "EventWeightBase.hxx"
#include "BinnedParams.hxx"

class ECalTrackEffSystematics: public EventWeightBase, public BinnedParams {
public:

  ECalTrackEffSystematics(bool computecounters=false);  
  
  
  virtual ~ECalTrackEffSystematics() {}

  /// Apply the systematic
  Weight_h ComputeWeight(const ToyExperiment&, const AnaEventC&, const ToyBoxB&){return 1;}
  
  Weight_h ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box, const SelectionBase& sel);

protected:

  /// Is this track relevant for this systematic ?
  bool IsRelevantTrueObject(const AnaEventC& event, const AnaTrueObjectC& track) const;

  bool  _computecounters;
};

#endif
