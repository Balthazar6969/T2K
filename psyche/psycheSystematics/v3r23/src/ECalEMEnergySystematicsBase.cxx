#include "ECalEMEnergySystematicsBase.hxx"
#include "EventBoxTracker.hxx"

//********************************************************************
ECalEMEnergySystematicsBase::ECalEMEnergySystematicsBase(UInt_t npar, const std::string& name, BinnedParams::TypeEnum type):EventVariationBase(npar),BinnedParams(name, type){
  //********************************************************************

  SetNParameters(GetNBins());
}

//********************************************************************
bool ECalEMEnergySystematicsBase::UndoSystematic(AnaEventC& event){
  //********************************************************************

  // Get the SystBox for this event
  SystBoxB* box = GetSystBox(event);

  // Get the relevant tracks for this systematic
  AnaRecObjectC** tracks = box->RelevantRecObjects;

  for (Int_t itrk=0; itrk<box->nRelevantRecObjects; itrk++){
    // For each track get ecal segment
    for (Int_t iecal=0; iecal<static_cast<AnaTrackB*>(tracks[itrk])->nECALSegments; iecal++){

      AnaECALParticleB* ecal = static_cast<AnaECALParticleB*>(static_cast<AnaTrackB*>(tracks[itrk])->ECALSegments[iecal]);
 
      if(!ecal) continue;
      if (!ecal->Original) continue;  
      
      // Go back to the corrected EMEnergy
      ecal->EMEnergy = static_cast<const AnaECALParticleB*>(ecal->Original)->EMEnergy;
    }
  }
  // Don't reset the spill to corrected
  return false;
}

//********************************************************************
bool ECalEMEnergySystematicsBase::IsRelevantRecObject(const AnaEventC& event, const AnaRecObjectC& recObj) const{
//********************************************************************
  (void)(event);
 
  const AnaTrackB* track = static_cast<const AnaTrackB*>(&recObj);

  //track should use Tracker or DsECal
  if (!SubDetId::GetDetectorUsed(track->Detector, SubDetId::kTECAL) && 
      !SubDetId::GetDetectorUsed(track->Detector, SubDetId::kDSECAL))
    return false;
  
  // track should have one ECal segment
  if (track->nECALSegments!=1) return false;
  
  if (!track->ECALSegments[0]) return false; 
  
  return true;
}

