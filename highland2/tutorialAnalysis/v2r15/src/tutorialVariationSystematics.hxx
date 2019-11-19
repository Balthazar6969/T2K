#ifndef tutorialVariationSystematics_h
#define tutorialVariationSystematics_h

#include "EventVariationBase.hxx"
#include "BinnedParams.hxx"

/// This systematic shifts the momentum of all tracks with TPC information.
///
/// For each virtual analysis, all tracks are shifted by the same amount.
/// The shift for each analysis is chosen from a Gaussian distribution
/// specified by the user.
class tutorialVariationSystematics: public EventVariationBase, public BinnedParams {
public:
  
  /// Instantiate the momentum scale systematic. nbins bins for the PDF. scale and scaleError describe
  /// the Gaussian distribution from which the shift in momentum is chosen.
  tutorialVariationSystematics();
  
  virtual ~tutorialVariationSystematics() {}
  
  /// Apply the systematic
  virtual void Apply(const ToyExperiment& toy, AnaEventC& event);
  
  /// Undo  the systematic variations done by ApplyVariation. This is faster tha reseting the full Spill
  virtual bool UndoSystematic(AnaEventC& event);

protected:

  /// Get the TrackGroup IDs array for this systematic
  Int_t GetRelevantRecObjectGroups(const SelectionBase& sel, Int_t* IDs) const;
  
protected:
  /// Mean of the scale distribution.
  Float_t _scale;
  
  /// Width of the scale distribution.
  Float_t _scaleError;
};

#endif
