#ifndef ECalEMEnergyScaleSystematics_h
#define ECalEMEnergyScaleSystematics_h

#include "ECalEMEnergySystematicsBase.hxx"

/// This systematic shifts the momentum of all tracks with TPC information.
///
/// For each virtual analysis, all tracks are shifted by the same amount.
/// The shift for each analysis is chosen from a Gaussian distribution
/// specified by the user.
class ECalEMEnergyScaleSystematics: public ECalEMEnergySystematicsBase {
public:
  
  /// Instantiate the momentum scale systematic. nbins bins for the PDF. scale and scaleError describe
  /// the Gaussian distribution from which the shift in momentum is chosen.
  ECalEMEnergyScaleSystematics():ECalEMEnergySystematicsBase(1, "ECalEMEnergyScale", k1D_SYMMETRIC_NOMEAN){}
  
  virtual ~ECalEMEnergyScaleSystematics() {}
  
  /// Apply the systematic
  void Apply(const ToyExperiment& toy, AnaEventC& event);
  
  
 
};

#endif
