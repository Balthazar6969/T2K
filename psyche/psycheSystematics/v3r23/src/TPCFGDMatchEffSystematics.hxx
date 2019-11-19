#ifndef TPCFGDMatchEffSystematics_h
#define TPCFGDMatchEffSystematics_h

#include "EventWeightBase.hxx"
#include "BinnedParams.hxx"

class TPCFGDMatchEffSystematics: public EventWeightBase, public BinnedParams {
public:
  
  TPCFGDMatchEffSystematics(bool computecounters=false);  
  virtual ~TPCFGDMatchEffSystematics() {}
  
  /// Apply the systematic
  Weight_h ComputeWeight(const ToyExperiment&, const AnaEventC&, const ToyBoxB&){return 1;}

  Weight_h ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box, const SelectionBase& sel);


protected:

  /// Is this track relevant for this systematic ?
  bool IsRelevantRecObject(const AnaEventC& event, const AnaRecObjectC& track) const;

  /// Check the true-reco association
  bool CheckTrueRecoAssociation(const AnaTrueObjectC& trueTrack, const AnaRecObjectC& track) const;
  
  /// Utility to utilize for several FGD objects if needed
  /// Get the weight given an FGD segment: prod-6
  Weight_h GetWeight(const AnaFGDParticleB* FGDSegment, const ToyExperiment& toy);

protected:

  bool  _computecounters;
  bool  _apply_both_FGD1_FGD2;
  Int_t _prod6_nnodes_cut;
};

#endif
