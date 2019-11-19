#ifndef FGDMassSystematics_h
#define FGDMassSystematics_h

#include "EventWeightBase.hxx"
#include "BinnedParams.hxx"

/// This is a normalization systematic. It takes into account the uncertainty on the FGD mass introduced in the MC

class FGDMassSystematics: public EventWeightBase {
public:
  
  FGDMassSystematics();

  virtual ~FGDMassSystematics() {
    if (_scintmass) delete _scintmass; _scintmass = NULL;
    if (_watermass) delete _watermass; _watermass = NULL;
  }
  
  /// Apply this systematic
  using EventWeightBase::ComputeWeight;
  Weight_h ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box);
  
protected:
  
  /// FGD mass correction and error
  Float_t _scintmass_corr;
  Float_t _scintmass_err;
  Float_t _watermass_corr;
  Float_t _watermass_err;

  BinnedParams* _scintmass;
  BinnedParams* _watermass;

};

#endif
