#ifndef ToFDSECalVariationSystematic_h
#define ToFDSECalVariationSystematic_h

#include "EventVariationBase.hxx"
#include "BinnedParams.hxx"

/// This systematic shifts the momentum of all tracks with TPC information.
///
/// For each virtual analysis, all tracks are shifted by the same amount.
/// The shift for each analysis is chosen from a Gaussian distribution
/// specified by the user.
class ToFDSECalVariationSystematic: public EventVariationBase, public BinnedParams {
public:
  
  /// Instantiate the momentum scale systematic. nbins bins for the PDF. scale and scaleError describe
  /// the Gaussian distribution from which the shift in momentum is chosen.
  ToFDSECalVariationSystematic();
  
  virtual ~ToFDSECalVariationSystematic() {}
  
  /// Apply the systematic
  virtual void Apply(const ToyExperiment& toy, AnaEventC& event);
  
  /// Undo  the systematic variations done by ApplyVariation. This is faster tha reseting the full Spill
  virtual bool UndoSystematic(AnaEventC& event);

protected:

  /// RD run number
  Int_t _RD_Run_Number;
  Int_t _Is_Sand_MC;

  /// Only sigma values since mean is supposed to be already "fixed" by a dedicated (highland2) correction
  Float_t _FGD1DSECal_MC_Sigma_run5;
  Float_t _FGD1DSECal_MC_Sigma_run6;
  Float_t _FGD1DSECal_Sand_Sigma_run5;
  Float_t _FGD1DSECal_Sand_Sigma_run6;

};

#endif

