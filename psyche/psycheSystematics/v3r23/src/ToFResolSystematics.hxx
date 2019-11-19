#ifndef tofSystematics_h
#define tofSystematics_h

#include "EventVariationBase.hxx"
#include "BinnedParams.hxx"
#include "ToFSenseCorrector.hxx"

class ToFResolSystematics: public EventVariationBase, public BinnedParams {
public:

  ToFResolSystematics();

  virtual ~ToFResolSystematics() {}

  /// Apply the systematic
  virtual void Apply(const ToyExperiment& toy, AnaEventC& event);

  /// Undo  the systematic variations done by ApplyVariation. This is faster tha reseting the full Spill
  virtual bool UndoSystematic(AnaEventC& event);


protected:
  /// Only sigma values since mean is supposed to be already "fixed" by a dedicated (highland2) correction
  Float_t _sigma_FGD1FGD2; 
  Float_t _sigma_FGD1P0D_Track; 
  Float_t _sigma_FGD1ECAL_LAStartFgd_Track; 
  Float_t _sigma_FGD1ECAL_LAStartFgd_Shower; 
  Float_t _sigma_FGD1ECAL_LAEndFgd_Track; 
  Float_t _sigma_FGD1ECAL_LAEndFgd_Shower; 
  Float_t _sigma_FGD1ECAL_HAStartFgd_Track; 
  Float_t _sigma_FGD1ECAL_HAStartFgd_Shower; 
  Float_t _sigma_FGD1ECAL_HAEndFgd_Track; 
  Float_t _sigma_FGD1ECAL_HAEndFgd_Shower; 


  /// ToF sense corrector
  ToFSenseCorrector _tofCorrector;
};

#endif
