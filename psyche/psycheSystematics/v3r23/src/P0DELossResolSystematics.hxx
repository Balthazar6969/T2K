#ifndef p0dMomResolSystematics_h
#define p0dMomResolSystematics_h

#include "EventVariationBase.hxx"
#include "BinnedParams.hxx"
#include <string>

/// This systematic smears momentum of all tracks in the P0D FV with TPC information.
/// The smearing is applied to the P0D momentum loss, which is defined as the difference between the global track momentum and the first subdetector entrance momentum (i.e. the momentum at the upstream end of TPC1 for the vast majority of tracks in the selection)
/// For each virtual analysis, all tracks are shifted by the same amount.
/// The shift for each analysis is chosen from a Gaussian distribution
/// specified by the user.
class P0DELossResolSystematics : public EventVariationBase, public BinnedParams {
  public:
  
   /// Instantiate the momentum scale systematic. nbins bins for the PDF. scale and scaleError describe
   /// the Gaussian distribution from which the shift in momentum is chosen.
   P0DELossResolSystematics();
  
   virtual ~P0DELossResolSystematics() {}
  
   /// Apply the systematic
   virtual void Apply(const ToyExperiment& toy, AnaEventC& event);
  
   /// Undo  the systematic variations done by ApplyVariation. This is faster than resetting the full Spill
   virtual bool UndoSystematic(AnaEventC& event);
};

#endif
