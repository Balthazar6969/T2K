#ifndef MomRangeResolSystematics_h
#define MomRangeResolSystematics_h

#include "EventVariationBase.hxx"
#include "BinnedParams.hxx"

/// This systematic smears the momentum-by-range by a
/// random amount from a Gaussian distribution.
///
class MomRangeResolSystematics: public EventVariationBase, public BinnedParams {
public:
  
  /// Instantiate the momentum resolution systematic
  MomRangeResolSystematics();
  
  virtual ~MomRangeResolSystematics() {}
  
  /// Apply the systematic
  virtual void Apply(const ToyExperiment& toy, AnaEventC& event);
  
  /// Undo  the systematic variations done by ApplyVariation. This is faster tha reseting the full Spill
  virtual bool UndoSystematic(AnaEventC& event);  
  
protected:
  
  /// Check whether a RecObject is relevant for this systematic or not
  virtual bool IsRelevantRecObject(const AnaEventC&, const AnaRecObjectC&) const;
  
  /// Get SMRD detector assuming only one can be present
  SubDetId::SubDetEnum GetSMRDDet(unsigned long Detector);
  
  /// Get TECal detector assuming only one can be present
  SubDetId::SubDetEnum GetTECalDet(unsigned long Detector);

  /// Get FGD detector assuming only one can be present
  SubDetId::SubDetEnum GetFGDDet(unsigned long Detector);

};

#endif
