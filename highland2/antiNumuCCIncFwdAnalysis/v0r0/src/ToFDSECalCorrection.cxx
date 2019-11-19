#include "ToFDSECalCorrection.hxx"
#include "VersioningUtils.hxx"
#include "ND280AnalysisUtils.hxx"
#include "TRandom3.h"
#include "Parameters.hxx"
#include "CutUtils.hxx"
//#define DEBUG


//********************************************************************
ToFDSECalCorrection::ToFDSECalCorrection() : CorrectionBase(){
//********************************************************************
  _RandomGenerator = NULL;

  InitializeRandomGenerator();

  // Read various parameters
  _RD_Run_Number                      = (Int_t)ND::params().GetParameterI("antiNumuControlSamples.RD_Run_Number");

  _DSECal_FGD1_ToF_MC_Mean_run5       = (Float_t)ND::params().GetParameterD("antiNumuControlSamples.Corrections.ToF_DSECal_FGD1_MC_Mean_run5");
  _DSECal_FGD1_ToF_MC_Sigma_run5      = (Float_t)ND::params().GetParameterD("antiNumuControlSamples.Corrections.ToF_DSECal_FGD1_MC_Sigma_run5");
  _DSECal_FGD1_ToF_Sand_Mean_run5     = (Float_t)ND::params().GetParameterD("antiNumuControlSamples.Corrections.ToF_DSECal_FGD1_Sand_Mean_run5");
  _DSECal_FGD1_ToF_Sand_Sigma_run5    = (Float_t)ND::params().GetParameterD("antiNumuControlSamples.Corrections.ToF_DSECal_FGD1_Sand_Sigma_run5");

  _DSECal_FGD1_ToF_MC_Mean_run6       = (Float_t)ND::params().GetParameterD("antiNumuControlSamples.Corrections.ToF_DSECal_FGD1_MC_Mean_run6");
  _DSECal_FGD1_ToF_MC_Sigma_run6      = (Float_t)ND::params().GetParameterD("antiNumuControlSamples.Corrections.ToF_DSECal_FGD1_MC_Sigma_run6");
  _DSECal_FGD1_ToF_Sand_Mean_run6     = (Float_t)ND::params().GetParameterD("antiNumuControlSamples.Corrections.ToF_DSECal_FGD1_Sand_Mean_run6");
  _DSECal_FGD1_ToF_Sand_Sigma_run6    = (Float_t)ND::params().GetParameterD("antiNumuControlSamples.Corrections.ToF_DSECal_FGD1_Sand_Sigma_run6");

}

//********************************************************************
ToFDSECalCorrection::~ToFDSECalCorrection() {
//********************************************************************
  if (_RandomGenerator)
    delete _RandomGenerator;
}

//********************************************************************
void ToFDSECalCorrection::InitializeRandomGenerator(){
//********************************************************************
  if (!_RandomGenerator){
    _RandomGenerator = new TRandom3();
    SetRandomSeed(ND::params().GetParameterI("highlandCorrections.ToF.RandomSeed"));

  }
}

//********************************************************************
UInt_t ToFDSECalCorrection::GetRandomSeed() const{
//********************************************************************
  if (_RandomGenerator)
    return _RandomGenerator->GetSeed();
  return 0XDEADBEEF;
}

//******************************************************************** 
void ToFDSECalCorrection::SetRandomSeed(UInt_t seed){
//********************************************************************
  if (_RandomGenerator)
    _RandomGenerator->SetSeed(seed); 
}


//********************************************************************
void ToFDSECalCorrection::Apply(AnaSpillC& spillBB){
//********************************************************************

  AnaSpill& spill = *static_cast<AnaSpill*>(&spillBB);

  // Data or MC
  bool isMC     = spill.GetIsMC();
  bool isSandMC = spill.GetIsSandMC();

  if (!isMC && !isSandMC) return;

  for (UInt_t i = 0; i < spill.Bunches.size(); i++) {
    AnaBunch* bunch = static_cast<AnaBunch*>(spill.Bunches[i]);
    for (UInt_t j = 0; j < bunch->Particles.size(); j++) {
      AnaTrack* track = static_cast<AnaTrack*>(bunch->Particles[j]);
      if (!track) continue;

      ApplyToFSmear(*track, isSandMC);
    }
  }
}

//********************************************************************
void ToFDSECalCorrection::ApplyToFSmear(AnaTrack& track, bool IsSandMC) const {
//******************************************************************** 

  // should have a corresponding true track
  AnaTrueParticleB* trueTrack = track.GetTrueParticle();
  if(!trueTrack) return;

  // apply the smearing
  if (!IsSandMC){
    //DSECal-FGD1
    if(_RD_Run_Number==5) {
      if(track.ToF.Flag_DSECal_FGD1) track.ToF.DSECal_FGD1 += _RandomGenerator->Gaus(_DSECal_FGD1_ToF_MC_Mean_run5, TMath::Sqrt(_DSECal_FGD1_ToF_MC_Sigma_run5));
      else if(track.Time.Flag_DSECal) track.Time.DSECal += _RandomGenerator->Gaus(_DSECal_FGD1_ToF_MC_Mean_run5, TMath::Sqrt(_DSECal_FGD1_ToF_MC_Sigma_run5));
    }
    else if(_RD_Run_Number==6) {
      if(track.ToF.Flag_DSECal_FGD1) track.ToF.DSECal_FGD1 += _RandomGenerator->Gaus(_DSECal_FGD1_ToF_MC_Mean_run6, TMath::Sqrt(_DSECal_FGD1_ToF_MC_Sigma_run6));
      else if(track.Time.Flag_DSECal) track.Time.DSECal += _RandomGenerator->Gaus(_DSECal_FGD1_ToF_MC_Mean_run6, TMath::Sqrt(_DSECal_FGD1_ToF_MC_Sigma_run6));
    }
  }
  else {
    //DSECal-FGD1
    if (_RD_Run_Number==5) {
      if(track.ToF.Flag_DSECal_FGD1) track.ToF.DSECal_FGD1 += _RandomGenerator->Gaus(_DSECal_FGD1_ToF_Sand_Mean_run5, TMath::Sqrt(_DSECal_FGD1_ToF_Sand_Sigma_run5));
      else if(track.Time.Flag_DSECal) track.Time.DSECal += _RandomGenerator->Gaus(_DSECal_FGD1_ToF_Sand_Mean_run5, TMath::Sqrt(_DSECal_FGD1_ToF_Sand_Sigma_run5));
    }
    else if (_RD_Run_Number==6) {
      if(track.ToF.Flag_DSECal_FGD1) track.ToF.DSECal_FGD1 += _RandomGenerator->Gaus(_DSECal_FGD1_ToF_Sand_Mean_run6, TMath::Sqrt(_DSECal_FGD1_ToF_Sand_Sigma_run6));
      else if(track.Time.Flag_DSECal) track.Time.DSECal += _RandomGenerator->Gaus(_DSECal_FGD1_ToF_Sand_Mean_run6, TMath::Sqrt(_DSECal_FGD1_ToF_Sand_Sigma_run6));
    }
  }
}

