#ifndef MomentumScaleSystematics_h
#define MomentumScaleSystematics_h

#include "EventVariationBase.hxx"
#include "BinnedParams.hxx"

/// This systematic shifts the momentum of all tracks with TPC information.
///
/// For each virtual analysis, all tracks are shifted by the same amount.
/// The shift for each analysis is chosen from a Gaussian distribution
/// specified by the user.
class MomentumScaleSystematics: public EventVariationBase, public BinnedParams {
public:
  
  /// Instantiate the momentum scale systematic. nbins bins for the PDF. scale and scaleError describe
  /// the Gaussian distribution from which the shift in momentum is chosen.
  MomentumScaleSystematics();
  
  virtual ~MomentumScaleSystematics() {}
  
  /// Apply the systematic
  void Apply(const ToyExperiment& toy, AnaEventC& event);

  ///
  virtual void ApplyP0DTPC(const ToyExperiment& toy, AnaEventC& event);
  
  /// Undo  the systematic variations done by ApplyVariation. This is faster tha reseting the full Spill
  bool UndoSystematic(AnaEventC& event);

protected:

  /// Width of the scale distribution.
  Float_t _scaleError;

  ///
  bool _useP0DFV;
};

#endif
