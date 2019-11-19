#ifndef ECalEMEnergySystematicsBase_h
#define ECalEMEnergySystematicsBase_h

#include "EventVariationBase.hxx"
#include "BinnedParams.hxx"

/// This systematic smears the ECal EM energy by a
/// random amount from a Gaussian distribution.

class ECalEMEnergySystematicsBase: public EventVariationBase, public BinnedParams {
public:
   
  ECalEMEnergySystematicsBase(UInt_t npar, const std::string& name, BinnedParams::TypeEnum type);
  
  virtual ~ECalEMEnergySystematicsBase(){}
  
  /// Undo  the systematic variations done by ApplyVariation. This is faster tha reseting the full Spill
  virtual bool UndoSystematic(AnaEventC& event); 
  
protected:
  
  /// Check whether a RecObject is relevant for this systematic or not
  virtual bool IsRelevantRecObject(const AnaEventC&, const AnaRecObjectC&) const;

private:
  ECalEMEnergySystematicsBase(){}
  

};

#endif
