#include "ECalEMEnergyScaleSystematics.hxx"

//#define DEBUG

  
//********************************************************************
void ECalEMEnergyScaleSystematics::Apply(const ToyExperiment& toy, AnaEventC& event){
//********************************************************************

  // Get the SystBox for this event
  SystBoxB* box = GetSystBox(event);
 
#ifdef DEBUG
    std::cout << "ECalEMEnergyScaleSystematics::ApplyVariation(): " << box->nRelevantRecObjects << std::endl;  
#endif

  // Loop over the relevant tracks for this systematic
  for (Int_t itrk=0;itrk<box->nRelevantRecObjects;itrk++){
    AnaTrackB* track = static_cast<AnaTrackB*>(box->RelevantRecObjects[itrk]);
    
    if(!track) continue;
    
    // Get ECal segment
    AnaECALParticleB* ecalTrack = track->ECALSegments[0];    
    if(!ecalTrack) continue;

    // Protection against tracks with no physical values
    if(ecalTrack->EMEnergy <= 0) continue;

#ifdef DEBUG
      std::cout << itrk << " EM Energy before the tweak --> em_0 = " << ecalTrack->EMEnergy << std::endl;
#endif
    
    Float_t scaleError;
    Int_t index;
    
    // Get the systematic source values 
    if (!GetBinSigmaValue(ecalTrack->EMEnergy, scaleError, index)) continue;
    
#ifdef DEBUG
    std::cout << " bin values: scaleError " << scaleError << " index " << index << std::endl;
#endif

    // Apply the momentum scale factor
    ecalTrack->EMEnergy *=(1 + scaleError*toy.GetToyVariations(_index)->Variations[index]);

#ifdef DEBUG
    std::cout << "EMEnergy after the tweak em = " << ecalTrack->EMEnergy << std::endl;
#endif
  }
}


