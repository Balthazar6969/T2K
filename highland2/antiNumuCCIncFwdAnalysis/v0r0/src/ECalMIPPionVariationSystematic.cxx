#include "ECalMIPPionVariationSystematic.hxx"
#include "EventBoxTracker.hxx"
#include "ND280AnalysisUtils.hxx"
#include "Parameters.hxx"
#include "DataClasses.hxx"


//********************************************************************
ECalMIPPionVariationSystematic::ECalMIPPionVariationSystematic():EventVariationBase(1){
//********************************************************************

  // Read various parameters
  _ECal_MipPion_Sigma = (Float_t)ND::params().GetParameterD("antiNumuControlSamples.Variations.ECal_MipPion_Sigma");
}

//********************************************************************
void ECalMIPPionVariationSystematic::Apply(const ToyExperiment& toy, AnaEventC& event){
//********************************************************************

  // Get the SystBox for this event
  SystBoxB* box = GetSystBox(event);

  AnaTrackB* ecalTracks[100];
  Int_t ntracks = anaUtils::GetAllTracksUsingDet(*static_cast<AnaEventB*>(&event), SubDetId::kECAL, ecalTracks);

  for (Int_t itrk=0;itrk<ntracks;itrk++){

    AnaTrackB* track = static_cast<AnaTrackB*>(ecalTracks[itrk]);

    for (Int_t subdet = 0; subdet<9; subdet++) {
      if (!SubDetId::GetDetectorUsed(track->Detector, static_cast<SubDetId::SubDetEnum >(subdet+6))) continue;

      AnaECALParticle* ECALSegment = static_cast<AnaECALParticle*>(anaUtils::GetSegmentInDet(*track, static_cast<SubDetId::SubDetEnum >(subdet+6)));

      if (!ECALSegment) continue;
      if (ECALSegment->PIDMipPion==-9999) continue;

      ECALSegment->PIDMipPion += _ECal_MipPion_Sigma * toy.GetToyVariations(_index)->Variations[0];
    }
  }
}

//********************************************************************
bool ECalMIPPionVariationSystematic::UndoSystematic(AnaEventC& event){
//********************************************************************

  // Get the SystBox for this event
  SystBoxB* box = GetSystBox(event);

  AnaTrackB* ecalTracks[100];
  Int_t ntracks = anaUtils::GetAllTracksUsingDet(*static_cast<AnaEventB*>(&event), SubDetId::kECAL, ecalTracks);

  for (Int_t itrk=0;itrk<ntracks;itrk++){

    AnaTrackB* track = static_cast<AnaTrackB*>(ecalTracks[itrk]);

    for (Int_t subdet = 0; subdet<9; subdet++) {
      if (!SubDetId::GetDetectorUsed(track->Detector, static_cast<SubDetId::SubDetEnum >(subdet+6))) continue;

      AnaECALParticle* ECALSegment = static_cast<AnaECALParticle*>(anaUtils::GetSegmentInDet(*track, static_cast<SubDetId::SubDetEnum >(subdet+6)));

      if (!ECALSegment) continue;
      if (ECALSegment->PIDMipPion==-9999) continue;

      ECALSegment->PIDMipPion = static_cast<AnaECALParticle*>(anaUtils::GetSegmentInDet(*track->GetOriginalTrack(), static_cast<SubDetId::SubDetEnum >(subdet+6)))->PIDMipPion;
    }
  }

  // Don't reset the spill to corrected
  return false;
}

