#ifndef P0DOOFVSystematics_h
#define P0DOOFVSystematics_h

#include "EventWeightBase.hxx"
#include "BinnedParams.hxx"

/*
Class to change the weights of out of fiducial volume events appearing in the P0D

Note: 2016/07/15 - an identical class also exists in p0dNumuCCQEAnalysis. Do not 
enable both at once.

Should be merged with the psycheSystematics OOFVSystematics class.


 */

class P0dTrackerOOFVSystematics: public EventWeightBase {
public:
  
  using EventWeightBase::ComputeWeight;
  P0dTrackerOOFVSystematics();
  
  virtual ~P0dTrackerOOFVSystematics() {
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
