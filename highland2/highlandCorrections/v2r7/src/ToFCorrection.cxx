#include "ToFCorrection.hxx"
#include "VersioningUtils.hxx"
#include "ND280AnalysisUtils.hxx"
#include "TRandom3.h"
#include "Parameters.hxx"
//#define DEBUG


//********************************************************************
ToFCorrection::ToFCorrection() : CorrectionBase(){
  //********************************************************************
  _RandomGenerator = NULL;

  InitializeRandomGenerator();

  // Read various parameters
  _P0D_FGD1_ToF_MC_TrueFwd_Track_Mean   = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.P0D_FGD1_MC_TrueFwd_Track_Mean");
  _P0D_FGD1_ToF_MC_TrueFwd_Track_Sigma  = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.P0D_FGD1_MC_TrueFwd_Track_Sigma");
  _P0D_FGD1_ToF_MC_TrueBwd_Track_Mean   = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.P0D_FGD1_MC_TrueBwd_Track_Mean");
  _P0D_FGD1_ToF_MC_TrueBwd_Track_Sigma  = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.P0D_FGD1_MC_TrueBwd_Track_Sigma");
  _P0D_FGD1_ToF_Sand_Track_Mean         = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.P0D_FGD1_Sand_Track_Mean");
  _P0D_FGD1_ToF_Sand_Track_Sigma        = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.P0D_FGD1_Sand_Track_Sigma");

  _FGD1_FGD2_ToF_MC_TrueFwd_Mean  = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.FGD1_FGD2_MC_TrueFwd_Mean");
  _FGD1_FGD2_ToF_MC_TrueFwd_Sigma = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.FGD1_FGD2_MC_TrueFwd_Sigma");
  _FGD1_FGD2_ToF_MC_TrueBwd_Mean  = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.FGD1_FGD2_MC_TrueBwd_Mean");
  _FGD1_FGD2_ToF_MC_TrueBwd_Sigma = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.FGD1_FGD2_MC_TrueBwd_Sigma");
  _FGD1_FGD2_ToF_Sand_Mean        = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.FGD1_FGD2_Sand_Mean");
  _FGD1_FGD2_ToF_Sand_Sigma       = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.FGD1_FGD2_Sand_Sigma");

  _ECal_FGD1_ToF_MC_LAStartFgd_TrueFwd_Shower_Mean  = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_MC_LAStartFgd_TrueFwd_Shower_Mean");
  _ECal_FGD1_ToF_MC_LAStartFgd_TrueFwd_Shower_Sigma = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_MC_LAStartFgd_TrueFwd_Shower_Sigma");
  _ECal_FGD1_ToF_MC_LAStartFgd_TrueBwd_Shower_Mean  = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_MC_LAStartFgd_TrueBwd_Shower_Mean");
  _ECal_FGD1_ToF_MC_LAStartFgd_TrueBwd_Shower_Sigma = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_MC_LAStartFgd_TrueBwd_Shower_Sigma");
  _ECal_FGD1_ToF_MC_LAStartFgd_TrueFwd_Track_Mean   = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_MC_LAStartFgd_TrueFwd_Track_Mean");
  _ECal_FGD1_ToF_MC_LAStartFgd_TrueFwd_Track_Sigma  = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_MC_LAStartFgd_TrueFwd_Track_Sigma");
  _ECal_FGD1_ToF_MC_LAStartFgd_TrueBwd_Track_Mean   = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_MC_LAStartFgd_TrueBwd_Track_Mean");
  _ECal_FGD1_ToF_MC_LAStartFgd_TrueBwd_Track_Sigma  = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_MC_LAStartFgd_TrueBwd_Track_Sigma");
  _ECal_FGD1_ToF_Sand_LAStartFgd_Shower_Mean        = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_Sand_LAStartFgd_Shower_Mean");
  _ECal_FGD1_ToF_Sand_LAStartFgd_Shower_Sigma       = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_Sand_LAStartFgd_Shower_Sigma");
  _ECal_FGD1_ToF_Sand_LAStartFgd_Track_Mean         = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_Sand_LAStartFgd_Track_Mean");
  _ECal_FGD1_ToF_Sand_LAStartFgd_Track_Sigma        = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_Sand_LAStartFgd_Track_Sigma");

  _ECal_FGD1_ToF_MC_LAEndFgd_TrueFwd_Shower_Mean    = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_MC_LAEndFgd_TrueFwd_Shower_Mean");
  _ECal_FGD1_ToF_MC_LAEndFgd_TrueFwd_Shower_Sigma   = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_MC_LAEndFgd_TrueFwd_Shower_Sigma");
  _ECal_FGD1_ToF_MC_LAEndFgd_TrueBwd_Shower_Mean    = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_MC_LAEndFgd_TrueBwd_Shower_Mean");
  _ECal_FGD1_ToF_MC_LAEndFgd_TrueBwd_Shower_Sigma   = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_MC_LAEndFgd_TrueBwd_Shower_Sigma");
  _ECal_FGD1_ToF_MC_LAEndFgd_TrueFwd_Track_Mean     = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_MC_LAEndFgd_TrueFwd_Track_Mean");
  _ECal_FGD1_ToF_MC_LAEndFgd_TrueFwd_Track_Sigma    = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_MC_LAEndFgd_TrueFwd_Track_Sigma");
  _ECal_FGD1_ToF_MC_LAEndFgd_TrueBwd_Track_Mean     = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_MC_LAEndFgd_TrueBwd_Track_Mean");
  _ECal_FGD1_ToF_MC_LAEndFgd_TrueBwd_Track_Sigma    = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_MC_LAEndFgd_TrueBwd_Track_Sigma");

  _ECal_FGD1_ToF_MC_HAStartFgd_TrueFwd_Shower_Mean  = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_MC_HAStartFgd_TrueFwd_Shower_Mean");
  _ECal_FGD1_ToF_MC_HAStartFgd_TrueFwd_Shower_Sigma = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_MC_HAStartFgd_TrueFwd_Shower_Sigma");
  _ECal_FGD1_ToF_MC_HAStartFgd_TrueBwd_Shower_Mean  = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_MC_HAStartFgd_TrueBwd_Shower_Mean");
  _ECal_FGD1_ToF_MC_HAStartFgd_TrueBwd_Shower_Sigma = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_MC_HAStartFgd_TrueBwd_Shower_Sigma");
  _ECal_FGD1_ToF_MC_HAStartFgd_TrueFwd_Track_Mean   = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_MC_HAStartFgd_TrueFwd_Track_Mean");
  _ECal_FGD1_ToF_MC_HAStartFgd_TrueFwd_Track_Sigma  = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_MC_HAStartFgd_TrueFwd_Track_Sigma");
  _ECal_FGD1_ToF_MC_HAStartFgd_TrueBwd_Track_Mean   = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_MC_HAStartFgd_TrueBwd_Track_Mean");
  _ECal_FGD1_ToF_MC_HAStartFgd_TrueBwd_Track_Sigma  = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_MC_HAStartFgd_TrueBwd_Track_Sigma");
  _ECal_FGD1_ToF_Sand_HAStartFgd_Shower_Mean        = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_Sand_HAStartFgd_Shower_Mean");
  _ECal_FGD1_ToF_Sand_HAStartFgd_Shower_Sigma       = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_Sand_HAStartFgd_Shower_Sigma");
  _ECal_FGD1_ToF_Sand_HAStartFgd_Track_Mean         = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_Sand_HAStartFgd_Track_Mean");
  _ECal_FGD1_ToF_Sand_HAStartFgd_Track_Sigma        = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_Sand_HAStartFgd_Track_Sigma");

  _ECal_FGD1_ToF_MC_HAEndFgd_TrueFwd_Shower_Mean    = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_MC_HAEndFgd_TrueFwd_Shower_Mean");
  _ECal_FGD1_ToF_MC_HAEndFgd_TrueFwd_Shower_Sigma   = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_MC_HAEndFgd_TrueFwd_Shower_Sigma");
  _ECal_FGD1_ToF_MC_HAEndFgd_TrueBwd_Shower_Mean    = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_MC_HAEndFgd_TrueBwd_Shower_Mean");
  _ECal_FGD1_ToF_MC_HAEndFgd_TrueBwd_Shower_Sigma   = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_MC_HAEndFgd_TrueBwd_Shower_Sigma");
  _ECal_FGD1_ToF_MC_HAEndFgd_TrueFwd_Track_Mean     = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_MC_HAEndFgd_TrueFwd_Track_Mean");
  _ECal_FGD1_ToF_MC_HAEndFgd_TrueFwd_Track_Sigma    = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_MC_HAEndFgd_TrueFwd_Track_Sigma");
  _ECal_FGD1_ToF_MC_HAEndFgd_TrueBwd_Track_Mean     = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_MC_HAEndFgd_TrueBwd_Track_Mean");
  _ECal_FGD1_ToF_MC_HAEndFgd_TrueBwd_Track_Sigma    = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_MC_HAEndFgd_TrueBwd_Track_Sigma");
  _ECal_FGD1_ToF_Sand_HAEndFgd_Shower_Mean          = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_Sand_HAEndFgd_Shower_Mean");
  _ECal_FGD1_ToF_Sand_HAEndFgd_Shower_Sigma         = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_Sand_HAEndFgd_Shower_Sigma");
  _ECal_FGD1_ToF_Sand_HAEndFgd_Track_Mean           = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_Sand_HAEndFgd_Track_Mean");
  _ECal_FGD1_ToF_Sand_HAEndFgd_Track_Sigma          = (Float_t)ND::params().GetParameterD("highlandCorrections.ToF.ECal_FGD1_Sand_HAEndFgd_Track_Sigma");

  _apply_ToF_based_Flip = (bool)ND::params().GetParameterI("psycheSystematics.ToF.ApplyToFFlip"); 

}

//********************************************************************
ToFCorrection::~ToFCorrection() {
  //********************************************************************
  if (_RandomGenerator)
    delete _RandomGenerator;
}

//********************************************************************
void ToFCorrection::InitializeRandomGenerator(){
  //********************************************************************
  if (!_RandomGenerator){
    _RandomGenerator = new TRandom3();
    SetRandomSeed(ND::params().GetParameterI("highlandCorrections.ToF.RandomSeed"));

  }
}

//********************************************************************
UInt_t ToFCorrection::GetRandomSeed() const{
  //********************************************************************
  if (_RandomGenerator)
    return _RandomGenerator->GetSeed();
  return 0XDEADBEEF;
}

//******************************************************************** 
void ToFCorrection::SetRandomSeed(UInt_t seed){
  //********************************************************************
  if (_RandomGenerator)
    _RandomGenerator->SetSeed(seed); 
}


//********************************************************************
void ToFCorrection::Apply(AnaSpillC& spillBB){
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

      ToFSenseCorrector::ToFTopology topo = ToFSenseCorrector::GetToFTopology(*track);

      if (topo == ToFSenseCorrector::kToFInvalid) continue;

      ApplyToFSmear(*track, topo, isSandMC);

      if (!_apply_ToF_based_Flip) continue;

      if (_tofCorrector.CheckNeedApplyToFBasedFlip(*track, topo)) ToFSenseCorrector::FlipTrack(*track);

    }

  }

}

//********************************************************************
void ToFCorrection::ApplyToFSmear(AnaTrack& track, ToFSenseCorrector::ToFTopology topo, bool IsSandMC) const {
  //******************************************************************** 

  // should have a corresponding true track
  AnaTrueParticleB* trueTrack = track.GetTrueParticle();
  if(!trueTrack) return;

  // apply the smearing
  if (!IsSandMC){

    //  fwd or bwd track
    bool TrueIsFwd = ToFSenseCorrector::IsForward(*trueTrack);

    //FGD1-FGD2
    if(topo == ToFSenseCorrector::kToFTopo_FGD1FGD2)
      track.ToF.FGD1_FGD2 += TrueIsFwd ? _RandomGenerator->Gaus(_FGD1_FGD2_ToF_MC_TrueFwd_Mean, 
          TMath::Sqrt(_FGD1_FGD2_ToF_MC_TrueFwd_Sigma)): 
        _RandomGenerator->Gaus(_FGD1_FGD2_ToF_MC_TrueBwd_Mean, TMath::Sqrt(_FGD1_FGD2_ToF_MC_TrueBwd_Sigma));

    // P0D-FGD1 -> only apply smearing to trak like cases
    if(topo == ToFSenseCorrector::kToFTopo_FGD1P0D_Track)
      track.ToF.P0D_FGD1 += TrueIsFwd ? _RandomGenerator->Gaus(_P0D_FGD1_ToF_MC_TrueFwd_Track_Mean, 
          TMath::Sqrt(_P0D_FGD1_ToF_MC_TrueFwd_Track_Sigma) ): 
        _RandomGenerator->Gaus(_P0D_FGD1_ToF_MC_TrueBwd_Track_Mean, TMath::Sqrt(_P0D_FGD1_ToF_MC_TrueBwd_Track_Sigma));            

    // ECal-FGD1
    if(topo == ToFSenseCorrector::kToFTopo_FGD1ECAL_LAStartFgd_Track)  
      track.ToF.ECal_FGD1 += TrueIsFwd ? _RandomGenerator->Gaus(_ECal_FGD1_ToF_MC_LAStartFgd_TrueFwd_Track_Mean, 
          TMath::Sqrt(_ECal_FGD1_ToF_MC_LAStartFgd_TrueFwd_Track_Sigma)): 
        _RandomGenerator->Gaus(_ECal_FGD1_ToF_MC_LAStartFgd_TrueBwd_Track_Mean, 
            TMath::Sqrt(_ECal_FGD1_ToF_MC_LAStartFgd_TrueBwd_Track_Sigma));
    if(topo == ToFSenseCorrector::kToFTopo_FGD1ECAL_LAStartFgd_Shower) 
      track.ToF.ECal_FGD1 += TrueIsFwd ? _RandomGenerator->Gaus(_ECal_FGD1_ToF_MC_LAStartFgd_TrueFwd_Shower_Mean, 
          TMath::Sqrt(_ECal_FGD1_ToF_MC_LAStartFgd_TrueFwd_Shower_Sigma)): 
        _RandomGenerator->Gaus(_ECal_FGD1_ToF_MC_LAStartFgd_TrueBwd_Shower_Mean, 
            TMath::Sqrt(_ECal_FGD1_ToF_MC_LAStartFgd_TrueBwd_Shower_Sigma));
    if(topo == ToFSenseCorrector::kToFTopo_FGD1ECAL_LAEndFgd_Track)  
      track.ToF.ECal_FGD1 += TrueIsFwd ? _RandomGenerator->Gaus(_ECal_FGD1_ToF_MC_LAEndFgd_TrueFwd_Track_Mean, 
          TMath::Sqrt(_ECal_FGD1_ToF_MC_LAEndFgd_TrueFwd_Track_Sigma)) : 
        _RandomGenerator->Gaus(_ECal_FGD1_ToF_MC_LAEndFgd_TrueBwd_Track_Mean , 
            TMath::Sqrt(_ECal_FGD1_ToF_MC_LAEndFgd_TrueBwd_Track_Sigma));
    if(topo == ToFSenseCorrector::kToFTopo_FGD1ECAL_LAEndFgd_Shower) 
      track.ToF.ECal_FGD1 += TrueIsFwd ? _RandomGenerator->Gaus(_ECal_FGD1_ToF_MC_LAEndFgd_TrueFwd_Shower_Mean, TMath::Sqrt(_ECal_FGD1_ToF_MC_LAEndFgd_TrueFwd_Shower_Sigma)) : _RandomGenerator->Gaus(_ECal_FGD1_ToF_MC_LAEndFgd_TrueBwd_Shower_Mean , TMath::Sqrt(_ECal_FGD1_ToF_MC_LAEndFgd_TrueBwd_Shower_Sigma));
    if(topo == ToFSenseCorrector::kToFTopo_FGD1ECAL_HAStartFgd_Track)  
      track.ToF.ECal_FGD1 += TrueIsFwd ? _RandomGenerator->Gaus(_ECal_FGD1_ToF_MC_HAStartFgd_TrueFwd_Track_Mean, TMath::Sqrt(_ECal_FGD1_ToF_MC_HAStartFgd_TrueFwd_Track_Sigma)) : _RandomGenerator->Gaus(_ECal_FGD1_ToF_MC_HAStartFgd_TrueBwd_Track_Mean , TMath::Sqrt(_ECal_FGD1_ToF_MC_HAStartFgd_TrueBwd_Track_Sigma));
    if(topo == ToFSenseCorrector::kToFTopo_FGD1ECAL_HAStartFgd_Shower) 
      track.ToF.ECal_FGD1 += TrueIsFwd ? _RandomGenerator->Gaus(_ECal_FGD1_ToF_MC_HAStartFgd_TrueFwd_Shower_Mean, TMath::Sqrt(_ECal_FGD1_ToF_MC_HAStartFgd_TrueFwd_Shower_Sigma)) : _RandomGenerator->Gaus(_ECal_FGD1_ToF_MC_HAStartFgd_TrueBwd_Shower_Mean , TMath::Sqrt(_ECal_FGD1_ToF_MC_HAStartFgd_TrueBwd_Shower_Sigma));
    if(topo == ToFSenseCorrector::kToFTopo_FGD1ECAL_HAEndFgd_Track)  
      track.ToF.ECal_FGD1 += TrueIsFwd ? _RandomGenerator->Gaus(_ECal_FGD1_ToF_MC_HAEndFgd_TrueFwd_Track_Mean, TMath::Sqrt(_ECal_FGD1_ToF_MC_HAEndFgd_TrueFwd_Track_Sigma)) : _RandomGenerator->Gaus(_ECal_FGD1_ToF_MC_HAEndFgd_TrueBwd_Track_Mean , TMath::Sqrt(_ECal_FGD1_ToF_MC_HAEndFgd_TrueBwd_Track_Sigma));
    if(topo == ToFSenseCorrector::kToFTopo_FGD1ECAL_HAEndFgd_Shower) 
      track.ToF.ECal_FGD1 += TrueIsFwd ? _RandomGenerator->Gaus(_ECal_FGD1_ToF_MC_HAEndFgd_TrueFwd_Shower_Mean, TMath::Sqrt(_ECal_FGD1_ToF_MC_HAEndFgd_TrueFwd_Shower_Sigma)) : _RandomGenerator->Gaus(_ECal_FGD1_ToF_MC_HAEndFgd_TrueBwd_Shower_Mean , TMath::Sqrt(_ECal_FGD1_ToF_MC_HAEndFgd_TrueBwd_Shower_Sigma));

  }
  else {

    //FGD1-FGD2
    if(topo == ToFSenseCorrector::kToFTopo_FGD1FGD2)
      track.ToF.FGD1_FGD2 += _RandomGenerator->Gaus(_FGD1_FGD2_ToF_Sand_Mean , TMath::Sqrt(_FGD1_FGD2_ToF_Sand_Sigma));

    // P0D-FGD1 -> only apply smearing to trak like cases
    if(topo == ToFSenseCorrector::kToFTopo_FGD1P0D_Track)
      track.ToF.P0D_FGD1 += _RandomGenerator->Gaus(_P0D_FGD1_ToF_Sand_Track_Mean  , TMath::Sqrt(_P0D_FGD1_ToF_Sand_Track_Sigma) );

    // ECal-FGD1 -> only apply smearing for cases in which we have enough statistic to compute the correction factors
    if(topo == ToFSenseCorrector::kToFTopo_FGD1ECAL_LAStartFgd_Track)  
      track.ToF.ECal_FGD1 += _RandomGenerator->Gaus(_ECal_FGD1_ToF_Sand_LAStartFgd_Track_Mean , TMath::Sqrt(_ECal_FGD1_ToF_Sand_LAStartFgd_Track_Sigma));
    if(topo == ToFSenseCorrector::kToFTopo_FGD1ECAL_LAStartFgd_Shower) 
      track.ToF.ECal_FGD1 += _RandomGenerator->Gaus(_ECal_FGD1_ToF_Sand_LAStartFgd_Shower_Mean , TMath::Sqrt(_ECal_FGD1_ToF_Sand_LAStartFgd_Shower_Sigma));
    if(topo == ToFSenseCorrector::kToFTopo_FGD1ECAL_HAStartFgd_Track)  
      track.ToF.ECal_FGD1 += _RandomGenerator->Gaus(_ECal_FGD1_ToF_Sand_HAStartFgd_Track_Mean , TMath::Sqrt(_ECal_FGD1_ToF_Sand_HAStartFgd_Track_Sigma));
    if(topo == ToFSenseCorrector::kToFTopo_FGD1ECAL_HAStartFgd_Shower) 
      track.ToF.ECal_FGD1 += _RandomGenerator->Gaus(_ECal_FGD1_ToF_Sand_HAStartFgd_Shower_Mean , TMath::Sqrt(_ECal_FGD1_ToF_Sand_HAStartFgd_Shower_Sigma));
    if(topo == ToFSenseCorrector::kToFTopo_FGD1ECAL_HAEndFgd_Track)  
      track.ToF.ECal_FGD1 += _RandomGenerator->Gaus(_ECal_FGD1_ToF_Sand_HAEndFgd_Track_Mean , TMath::Sqrt(_ECal_FGD1_ToF_Sand_HAEndFgd_Track_Sigma));
    if(topo == ToFSenseCorrector::kToFTopo_FGD1ECAL_HAEndFgd_Shower)  
      track.ToF.ECal_FGD1 += _RandomGenerator->Gaus(_ECal_FGD1_ToF_Sand_HAEndFgd_Shower_Mean , TMath::Sqrt(_ECal_FGD1_ToF_Sand_HAEndFgd_Shower_Sigma));

  }


}
