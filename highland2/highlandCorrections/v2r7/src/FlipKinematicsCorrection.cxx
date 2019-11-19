#include "FlipKinematicsCorrection.hxx"
#include "ToFSenseCorrector.hxx"
#include "CutUtils.hxx"

//#define UseRecPack
#ifdef  UseRecPack
#include "AnaRecPackManager.hxx"
#endif



//********************************************************************
FlipKinematicsCorrection::FlipKinematicsCorrection(SubDetId::SubDetEnum det) :  CorrectionBase(){
  //********************************************************************
  _det = det; 
}

//********************************************************************
void FlipKinematicsCorrection::Apply(AnaSpillC& spillC) {
//********************************************************************


  AnaSpill& spill = *static_cast<AnaSpill*>(&spillC);

  for (UInt_t i = 0; i < spill.Bunches.size(); i++) {
    AnaBunch* bunch = static_cast<AnaBunch*>(spill.Bunches[i]);
    for (UInt_t j = 0; j < bunch->Particles.size(); j++) {

      AnaTrack* track = static_cast<AnaTrack*>(bunch->Particles[j]);

      if (!track) continue;

      // Check whether a track is relevant to apply the correction
      if (!IsRelevantTrack(*track)) continue;

      AnaTPCParticle* tpcTrack = static_cast<AnaTPCParticle*>(anaUtils::GetSegmentWithMostNodesInClosestTpc(*track, track->PositionEnd));

      if (!tpcTrack) continue;

      if (!cutUtils::TPCTrackQualityCut(*tpcTrack)) continue;

#ifdef  UseRecPack
      track->MomentumFlip = -999.;
      double momentum_tmp = tpcTrack->MomentumEnd; 
      if (ND::tman().CorrectEnergyLossLinear(tpcTrack->PositionEnd, track->PositionEnd, momentum_tmp, 
            ParticleId::GetParticle(track->ReconPDG), AnaRecPackManager::kBwd)) // from closest TPC endpoint and to track end backwards
        track->MomentumFlip = momentum_tmp;
    

#else
      std::cout << " WARNING! FlipKinematicsCorrection is run w/o highlandRecPack usage. \n"
        " It is highly recommended to use RecPack to restore possible fit failures of the \n"
        " global recon re-fits with opposite track sense hypotheis assumes (end to start) " << std::endl;

#endif

    } 
  }	  
}

//********************************************************************
bool FlipKinematicsCorrection::IsRelevantTrack(const AnaTrackB& track) const{
  //********************************************************************

  if (!SubDetId::GetDetectorUsed(track.Detector, _det)) return false;

  if (!SubDetId::GetDetectorUsed(track.Detector, SubDetId::kTPC)) return false;

      
  if (ToFSenseCorrector::GetToFTopology(track) == ToFSenseCorrector::kToFInvalid) return false;
	
  return true;

}

