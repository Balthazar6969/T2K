#ifndef PileUpSystematics_h
#define PileUpSystematics_h

#include "EventWeightBase.hxx"
#include "BinnedParams.hxx"

/// There are a number of categories of possible pile up, but only the effect of sand muons is 	
/// significant for this analysis. Events are rejected if there is activity in TPC1 (cut *) since in most 
/// cases, the TPC1 activity is due to tracks from interactions downstream of the detector 
/// (sand muons) or outside the tracker fiducial volume. A quick study of MC events rejected due to 
/// the TPC1 veto indicates that the majority are not true CC interactions. Since sand muons are 
/// not included in the standard NEUT simulation, the Monte Carlo does not include the effect of 
/// events that are rejected due to coincidence with a Sand Muon and a correction must be made ...

// This systematic should then be applied as a simple weighting factor to all events in the analysis:

/// TN-152 for a longer explanaition

class PileUpSystematics: public EventWeightBase {
public:
  
  PileUpSystematics();
  
  virtual ~PileUpSystematics() {
    if (_fgd1) delete _fgd1; _fgd1 = NULL; 
    if (_fgd2) delete _fgd2; _fgd2 = NULL; 
  }
  
  /// Apply this systematic
  using EventWeightBase::ComputeWeight;
  Weight_h ComputeWeight(const ToyExperiment&, const AnaEventC&, const ToyBoxB&){return 1;}
  Weight_h ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box, const SelectionBase& sel);
  
  void Initialize();
  
  BinnedParams* _fgd1;
  BinnedParams* _fgd2;

};

#endif
