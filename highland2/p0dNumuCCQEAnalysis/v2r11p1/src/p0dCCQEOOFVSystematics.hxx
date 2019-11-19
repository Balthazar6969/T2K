#ifndef P0DCCQEOOFVSystematics_h
#define P0DCCQEOOFVSystematics_h

#include "EventWeightBase.hxx"
#include "BinnedParams.hxx"

class p0dCCQEOOFVSystematics: public EventWeightBase {
public:
  
  using EventWeightBase::ComputeWeight;
  p0dCCQEOOFVSystematics();
  
  virtual ~p0dCCQEOOFVSystematics() {
    delete _p0d;
    delete _rate;
  }
  
  Weight_h ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box);
  
protected:

  Int_t GetDetNumber(SubDetId::SubDetEnum det);  
  Int_t GetBeamNumber(Int_t runperiod,AnaTrackB *maintrack);

protected:  

  /// Mean of the rate correction
  Float_t _rate_corr;  

  /// Uncertainty on the rate correction
  Float_t _rate_error;  

  /// Mean of the reco eff  correction
  Float_t _reco_corr[9];  

  /// Uncertainty on the reco eff  correction
  Float_t _reco_error[9];  

  //Index for rate corrections
  Int_t _rate_index;
      
  //Index for reco corrections
  Int_t _reco_index[9];
  
  BinnedParams* _p0d;
  BinnedParams* _rate;

};

#endif
