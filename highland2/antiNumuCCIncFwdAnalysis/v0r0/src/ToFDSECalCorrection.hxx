#ifndef ToFDSECalCorrection_h
#define ToFDSECalCorrection_h

#include "CorrectionBase.hxx"
#include "DataClasses.hxx"
#include "ToFSenseCorrector.hxx"

/// This correction smears and shifts the MC ToF to match data
/// There is also an option to flip track according to ToF
class TRandom;

class ToFDSECalCorrection: public CorrectionBase {
public:

  /// Various bits to check which ToF relevant topology has to be checked/present etc
  /// ToF was studied with specific samples and this id why it matters,  more details will
  /// be provided in the dedicated TNs
  enum ToFTopology{
    kToFTopo_FGD1DSECAL = 0,
    kToFInvalid,  // will use it for array counter...
  };
  
  ToFDSECalCorrection();

  virtual ~ToFDSECalCorrection();

  /// Apply the correction to all relevant tracks 
  void Apply(AnaSpillC& spill);
  
  /// The only thing we allow for the generator is to get the seed
  UInt_t GetRandomSeed() const; 
 	  
  /// And change the seed
  void SetRandomSeed(UInt_t seed);
 	  
private:
  /// Apply MC ToF smearing
  void ApplyToFSmear(AnaTrack& track, bool IsSandMC) const;
  
  /// Initialze random generator
  void InitializeRandomGenerator();

  /// RD run number
  Int_t _RD_Run_Number;
  
  /// Values to apply the smearing (MC)
  Float_t _DSECal_FGD1_ToF_MC_Mean_run5;
  Float_t _DSECal_FGD1_ToF_MC_Sigma_run5;
  Float_t _DSECal_FGD1_ToF_Sand_Mean_run5;
  Float_t _DSECal_FGD1_ToF_Sand_Sigma_run5;

  Float_t _DSECal_FGD1_ToF_MC_Mean_run6;
  Float_t _DSECal_FGD1_ToF_MC_Sigma_run6;
  Float_t _DSECal_FGD1_ToF_Sand_Mean_run6;
  Float_t _DSECal_FGD1_ToF_Sand_Sigma_run6;
  
  /// A random generator that can be used to generate throws.
  TRandom* _RandomGenerator;

  /// A corrector to deal with various ToF business
  ToFSenseCorrector _tofCorrector;
};

#endif

