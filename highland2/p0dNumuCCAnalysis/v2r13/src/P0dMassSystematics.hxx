#ifndef P0DMassSystematics_h
#define P0DMassSystematics_h

#include "EventWeightBase.hxx"
#include "BinnedParams.hxx"

/// This is a normalization systematic. It takes into account the uncertainty on the P0D mass introduced in the MC
// Numbers are taken from the prod. 5 update of TN-073.

class P0dMassSystematics: public EventWeightBase, public BinnedParams {
public:
  
  P0dMassSystematics();
  
  virtual ~P0dMassSystematics() {}
  
  /// Apply this systematic
  using EventWeightBase::ComputeWeight;
  Weight_h ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box);
  
protected:
  
  /// P0D mass correction and error
  Float_t leadMass_corr;
  Float_t brassMass_corr;
  Float_t waterMass_corr;
  Float_t otherMass_corr;
  Float_t leadMass_err;//Lead layer
  Float_t brassMass_err;//Brass layer
  Float_t waterMass_err;//Water layer
  Float_t otherMass_err;//Scintillator, dead material, WT cover, etc.

};

#endif
