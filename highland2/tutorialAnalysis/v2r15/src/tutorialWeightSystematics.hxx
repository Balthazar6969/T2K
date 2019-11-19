#ifndef tutorialWeightSystematics_h
#define tutorialWeightSystematics_h

#include "EventWeightBase.hxx"
#include "BinnedParams.hxx"

/// This is a normalization systematic. It takes into account the uncertainty on the FGD mass introduced in the MC

class tutorialWeightSystematics: public EventWeightBase {
public:

  /// Default constructor
  tutorialWeightSystematics();
  
  virtual ~tutorialWeightSystematics() {}
  
  /// Apply this systematic
  using EventWeightBase::ComputeWeight;
  Weight_h ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box);
  
protected:
  
  /// FGD mass correction and error
  Float_t _scintmass_corr;
  Float_t _scintmass_err;
  Float_t _watermass_corr;
  Float_t _watermass_err;

  BinnedParams *_scintmass;
  BinnedParams *_watermass;

};

#endif
