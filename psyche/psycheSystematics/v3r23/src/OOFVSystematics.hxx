#ifndef OOFVSystematics_h
#define OOFVSystematics_h

#include "EventWeightBase.hxx"
#include "BinnedParams.hxx"

class OOFVSystematics: public EventWeightBase {
public:
  
  OOFVSystematics();
  
  virtual ~OOFVSystematics() {
    if (_fgd1) delete _fgd1; _fgd1 = NULL; 
    if (_fgd2) delete _fgd2; _fgd2 = NULL;
    if (_rate) delete _rate; _rate = NULL;
  }
  
  /// Apply this systematic
  using EventWeightBase::ComputeWeight;
  Weight_h ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box){(void)toy;(void)event;(void)box;return 1;};
  Weight_h ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box, const SelectionBase& sel);
  
  void Initialize();
  
  
protected:

  Int_t GetDetNumber(SubDetId::SubDetEnum det);  
  Int_t GetBeamNumber(Int_t runperiod,AnaTrackB *maintrack);

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
