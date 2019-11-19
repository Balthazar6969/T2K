#include "BFieldDistortionSystematics.hxx"
#include "ND280AnalysisUtils.hxx"
#include "EventBoxTracker.hxx"
#include "VersioningUtils.hxx"
#include <math.h>

//#define DEBUG

//********************************************************************
BFieldDistortionSystematics::BFieldDistortionSystematics():EventVariationBase(1),BinnedParams("BFieldDistortion",k1D_SYMMETRIC_NOMEAN){
//********************************************************************

  // Get the systematic source values 
  GetSigmaValueForBin(0, _mean_error);

  // This systematic uses a Uniform random throw
  _PDF = kUniform;
}

//********************************************************************
void BFieldDistortionSystematics::Apply(const ToyExperiment& toy, AnaEventC& event){
//********************************************************************

  // Get the SystBox for this event
  SystBoxB* box = GetSystBox(event);

#ifdef DEBUG 
  std::cout << "BFieldDistortionSystematics::Apply(): " << box->nRelevantRecObjects << std::endl;  
#endif

  // Loop over the relevant tracks for this systematic
  for (Int_t itrk = 0; itrk < box->nRelevantRecObjects; itrk++){
#ifdef DEBUG 
    std::cout << "itrk = " << itrk << std::endl;
#endif
    AnaTrackB* track = static_cast<AnaTrackB*>(box->RelevantRecObjects[itrk]); 
    
    if (!track) continue;

    // Consider the case for the momentum (start of the track) and momentum flip
    // (i.e. end of the track)
    
    // Start of the track
    // Use the TPC segment with more nodes in closest TPC to the start position of the track
    AnaTPCParticleB* tpcTrack = static_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInClosestTpc(*track, track->PositionStart));
    
    VaryMomentum(track->Momentum, tpcTrack, toy);
   
    tpcTrack = static_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInClosestTpc(*track, track->PositionEnd));
    
    VaryMomentum(track->MomentumFlip, tpcTrack, toy); 
    
  }
  
}
 
//********************************************************************
void BFieldDistortionSystematics::VaryMomentum(Float_t& Momentum, const AnaTPCParticleB* tpcTrack, const ToyExperiment& toy) const{
//********************************************************************

  if (!tpcTrack) return;
  
  Float_t mom_global = Momentum;
  Float_t mom_tpc    = tpcTrack->Momentum;
      
#ifdef DEBUG
      std::cout << " BFieldDistortionSystematics::VaryMomentum: momentum global " << mom_global << std::endl;
      std::cout << " BFieldDistortionSystematics::VaryMomentum: momentum in TPC " << mom_tpc    << std::endl;
#endif
      
  // Make sure we have a valid momentum
  if (mom_global < 0 || !TMath::Finite(mom_global)) return;
  
  Float_t mom_tpc_refit;
      
  // The sign represents the charge in the prod6 files!
  if (versionUtils::prod6_systematics){
    mom_tpc_refit = fabs(tpcTrack->RefitMomentum);
    if (mom_tpc_refit == 99999 || !TMath::Finite(mom_tpc_refit)) return;  
  }
  else{
    mom_tpc_refit = tpcTrack->RefitMomentum;
    if (mom_tpc_refit < 0 || !TMath::Finite(mom_tpc_refit)) return;
  }
      
#ifdef DEBUG    
  std::cout << " BFieldDistortionSystematics::VaryMomentum: momentum in TPC (refit) " << mom_tpc_refit << std::endl;
#endif
      
  // The TPC momentum should always be smaller
  // than the global momentum ( mom_global = mom_tpc + fgd_eloss)
           
  if (mom_tpc > mom_global || mom_tpc_refit > mom_global) return; // hmm?
    
  // Apply the variation    
  Momentum += (mom_tpc_refit - mom_tpc) * _mean_error * toy.GetToyVariations(_index)->Variations[0];;
    
#ifdef DEBUG   
  std::cout << "BFieldDistortionSystematics::VaryMomentum: after variation momentum global (after variation) = " << Momentum << " var " << toy.Variations[0] << std::endl;
#endif

  return;

}

//********************************************************************
bool BFieldDistortionSystematics::UndoSystematic(AnaEventC& event){
//********************************************************************

  // Get the SystBox for this event
  SystBoxB* box = GetSystBox(event);

  // Get the relevant tracks for this systematic
  AnaRecObjectC** tracks = box->RelevantRecObjects;

  for (Int_t itrk=0;itrk<box->nRelevantRecObjects;itrk++){
    AnaTrackB* track = static_cast<AnaTrackB*>(tracks[itrk]);
    // Go back to the original (prior to variation) momentum
    track->Momentum     = track->GetOriginalTrack()->Momentum;
    
    // Go back to the original momentum for the flipped one
    track->MomentumFlip = track->GetOriginalTrack()->MomentumFlip;
  }

  // Don't reset the spill to corrected
  return false;
}

