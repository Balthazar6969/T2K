#ifndef ToFCorrection_h
#define ToFCorrection_h

#include "CorrectionBase.hxx"
#include "DataClasses.hxx"
#include "ToFSenseCorrector.hxx"

/// This correction smears and shifts the MC ToF to match data
/// There is also an option to flip track according to ToF
class TRandom;

class ToFCorrection: public CorrectionBase {
public:
  
  ToFCorrection();

  virtual ~ToFCorrection();

  /// Apply the correction to all relevant tracks 
  void Apply(AnaSpillC& spill);
  
  /// The only thing we allow for the generator is to get the seed
 	UInt_t GetRandomSeed() const; 
 	  
 	/// And change the seed
 	void SetRandomSeed(UInt_t seed);
 	  
private:
  /// Apply MC ToF smearing
  void ApplyToFSmear(AnaTrack& track, ToFSenseCorrector::ToFTopology topo, bool IsSandMC) const;
  
 	/// Initialze random generator
  void InitializeRandomGenerator();
  
  /// Values to apply the smearing (MC)
  Float_t _P0D_FGD1_ToF_MC_TrueFwd_Track_Mean;
  Float_t _P0D_FGD1_ToF_MC_TrueFwd_Track_Sigma;
  Float_t _P0D_FGD1_ToF_MC_TrueBwd_Track_Mean;
  Float_t _P0D_FGD1_ToF_MC_TrueBwd_Track_Sigma;
  Float_t _P0D_FGD1_ToF_Sand_Track_Mean;
  Float_t _P0D_FGD1_ToF_Sand_Track_Sigma;

  Float_t _FGD1_FGD2_ToF_MC_TrueFwd_Mean;
  Float_t _FGD1_FGD2_ToF_MC_TrueFwd_Sigma;
  Float_t _FGD1_FGD2_ToF_MC_TrueBwd_Mean;
  Float_t _FGD1_FGD2_ToF_MC_TrueBwd_Sigma;
  Float_t _FGD1_FGD2_ToF_Sand_Mean;
  Float_t _FGD1_FGD2_ToF_Sand_Sigma;

  Float_t _ECal_FGD1_ToF_MC_LAStartFgd_TrueFwd_Shower_Mean;
  Float_t _ECal_FGD1_ToF_MC_LAStartFgd_TrueFwd_Shower_Sigma;
  Float_t _ECal_FGD1_ToF_MC_LAStartFgd_TrueBwd_Shower_Mean;
  Float_t _ECal_FGD1_ToF_MC_LAStartFgd_TrueBwd_Shower_Sigma;
  Float_t _ECal_FGD1_ToF_MC_LAStartFgd_TrueFwd_Track_Mean;
  Float_t _ECal_FGD1_ToF_MC_LAStartFgd_TrueFwd_Track_Sigma;
  Float_t _ECal_FGD1_ToF_MC_LAStartFgd_TrueBwd_Track_Mean;
  Float_t _ECal_FGD1_ToF_MC_LAStartFgd_TrueBwd_Track_Sigma;
  Float_t _ECal_FGD1_ToF_Sand_LAStartFgd_Shower_Mean;
  Float_t _ECal_FGD1_ToF_Sand_LAStartFgd_Shower_Sigma;
  Float_t _ECal_FGD1_ToF_Sand_LAStartFgd_Track_Mean;
  Float_t _ECal_FGD1_ToF_Sand_LAStartFgd_Track_Sigma;

  Float_t _ECal_FGD1_ToF_MC_LAEndFgd_TrueFwd_Shower_Mean;
  Float_t _ECal_FGD1_ToF_MC_LAEndFgd_TrueFwd_Shower_Sigma;
  Float_t _ECal_FGD1_ToF_MC_LAEndFgd_TrueBwd_Shower_Mean;
  Float_t _ECal_FGD1_ToF_MC_LAEndFgd_TrueBwd_Shower_Sigma;
  Float_t _ECal_FGD1_ToF_MC_LAEndFgd_TrueFwd_Track_Mean;
  Float_t _ECal_FGD1_ToF_MC_LAEndFgd_TrueFwd_Track_Sigma;
  Float_t _ECal_FGD1_ToF_MC_LAEndFgd_TrueBwd_Track_Mean;
  Float_t _ECal_FGD1_ToF_MC_LAEndFgd_TrueBwd_Track_Sigma;

  Float_t _ECal_FGD1_ToF_MC_HAStartFgd_TrueFwd_Shower_Mean;
  Float_t _ECal_FGD1_ToF_MC_HAStartFgd_TrueFwd_Shower_Sigma;
  Float_t _ECal_FGD1_ToF_MC_HAStartFgd_TrueBwd_Shower_Mean;
  Float_t _ECal_FGD1_ToF_MC_HAStartFgd_TrueBwd_Shower_Sigma;
  Float_t _ECal_FGD1_ToF_MC_HAStartFgd_TrueFwd_Track_Mean;
  Float_t _ECal_FGD1_ToF_MC_HAStartFgd_TrueFwd_Track_Sigma;
  Float_t _ECal_FGD1_ToF_MC_HAStartFgd_TrueBwd_Track_Mean;
  Float_t _ECal_FGD1_ToF_MC_HAStartFgd_TrueBwd_Track_Sigma;
  Float_t _ECal_FGD1_ToF_Sand_HAStartFgd_Shower_Mean;
  Float_t _ECal_FGD1_ToF_Sand_HAStartFgd_Shower_Sigma;
  Float_t _ECal_FGD1_ToF_Sand_HAStartFgd_Track_Mean;
  Float_t _ECal_FGD1_ToF_Sand_HAStartFgd_Track_Sigma;

  Float_t _ECal_FGD1_ToF_MC_HAEndFgd_TrueFwd_Shower_Mean;
  Float_t _ECal_FGD1_ToF_MC_HAEndFgd_TrueFwd_Shower_Sigma;
  Float_t _ECal_FGD1_ToF_MC_HAEndFgd_TrueBwd_Shower_Mean;
  Float_t _ECal_FGD1_ToF_MC_HAEndFgd_TrueBwd_Shower_Sigma;
  Float_t _ECal_FGD1_ToF_MC_HAEndFgd_TrueFwd_Track_Mean;
  Float_t _ECal_FGD1_ToF_MC_HAEndFgd_TrueFwd_Track_Sigma;
  Float_t _ECal_FGD1_ToF_MC_HAEndFgd_TrueBwd_Track_Mean;
  Float_t _ECal_FGD1_ToF_MC_HAEndFgd_TrueBwd_Track_Sigma;
  Float_t _ECal_FGD1_ToF_Sand_HAEndFgd_Shower_Mean;
  Float_t _ECal_FGD1_ToF_Sand_HAEndFgd_Shower_Sigma;
  Float_t _ECal_FGD1_ToF_Sand_HAEndFgd_Track_Mean;
  Float_t _ECal_FGD1_ToF_Sand_HAEndFgd_Track_Sigma;
  
  /// Whether to flip the track based on ToF
  bool _apply_ToF_based_Flip; 
  
  /// A random generator that can be used to generate throws.
  TRandom* _RandomGenerator;
  
  /// A corrector to deal with various ToF business
  ToFSenseCorrector _tofCorrector;
};

#endif
