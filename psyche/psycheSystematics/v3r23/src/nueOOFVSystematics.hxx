#ifndef nueOOFVSystematics_h
#define nueOOFVSystematics_h

#include "EventWeightBase.hxx"
#include "BinnedParams.hxx"

/// This systematic evaluates the oofv systematic for nue analysis. At the moment a 30% rate unceratinty is applied to out of fgd events

class nueOOFVSystematics: public EventWeightBase {
public:
  
  using EventWeightBase::ComputeWeight;
  nueOOFVSystematics();
  
  virtual ~nueOOFVSystematics() {
    if (_fgd1) delete _fgd1; _fgd1 = NULL; 
    if (_fgd2) delete _fgd2; _fgd2 = NULL;
    if (_rate) delete _rate; _rate = NULL;
  }
  
  /// Apply the systematic
  Weight_h ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box);
  
  void Initialize();
  
  
protected:

  Int_t GetDetNumber(SubDetId::SubDetEnum det);  

protected:  

  /// Mean of the rate correction
  Float_t _rate_corr[2][9];  

  /// Uncertainty on the rate correction
  Float_t _rate_error[2][9];  

  /// Mean of the reco eff  correction
  Float_t _reco_corr[2][9];  

  /// Uncertainty on the reco eff  correction
  Float_t _reco_error[2][9];  

  //Index for rate corrections
  Int_t _rate_index[2][9];
      
  //Index for reco corrections
  Int_t _reco_index[2][9];
  
  BinnedParams* _fgd1;
  BinnedParams* _fgd2;
  BinnedParams* _rate;

};

#endif
