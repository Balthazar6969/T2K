#include "ToFDSECalVariationSystematic.hxx"
#include "ToFDSECalCorrection.hxx"
#include "EventBoxTracker.hxx"
#include "ND280AnalysisUtils.hxx"
#include "Parameters.hxx"


//********************************************************************
ToFDSECalVariationSystematic::ToFDSECalVariationSystematic():EventVariationBase(1){
//********************************************************************

  // Read various parameters
  _RD_Run_Number = (Int_t)ND::params().GetParameterI("antiNumuControlSamples.RD_Run_Number");
  _Is_Sand_MC = (Int_t)ND::params().GetParameterI("antiNumuControlSamples.Is_Sand_MC");

  // Read various parameters
  _FGD1DSECal_MC_Sigma_run5 = (Float_t)ND::params().GetParameterD("antiNumuControlSamples.Variations.ToF_DSECal_FGD1_MC_Sigma_run5");
  _FGD1DSECal_MC_Sigma_run6 = (Float_t)ND::params().GetParameterD("antiNumuControlSamples.Variations.ToF_DSECal_FGD1_MC_Sigma_run6");
  _FGD1DSECal_Sand_Sigma_run5 = (Float_t)ND::params().GetParameterD("antiNumuControlSamples.Variations.ToF_DSECal_FGD1_Sand_Sigma_run5");
  _FGD1DSECal_Sand_Sigma_run6 = (Float_t)ND::params().GetParameterD("antiNumuControlSamples.Variations.ToF_DSECal_FGD1_Sand_Sigma_run6");
}

//********************************************************************
void ToFDSECalVariationSystematic::Apply(const ToyExperiment& toy, AnaEventC& event){
//********************************************************************

  // Get the SystBox for this event
  SystBoxB* box = GetSystBox(event);

  AnaTrackB* ecalTracks[100];
  Int_t ntracks = anaUtils::GetAllTracksUsingDet(*static_cast<AnaEventB*>(&event), SubDetId::kECAL, ecalTracks);

  for (Int_t itrk=0;itrk<ntracks;itrk++){

    AnaTrackB* track = static_cast<AnaTrackB*>(ecalTracks[itrk]);

	if (track->ToF.Flag_DSECal_FGD1) {

      if(!_Is_Sand_MC) {
        if(_RD_Run_Number==5) track->ToF.DSECal_FGD1 += (_FGD1DSECal_MC_Sigma_run5) * toy.GetToyVariations(_index)->Variations[0];
        else if(_RD_Run_Number==6) track->ToF.DSECal_FGD1 += (_FGD1DSECal_MC_Sigma_run6) * toy.GetToyVariations(_index)->Variations[0];
      }
      else{
        if(_RD_Run_Number==5) track->ToF.DSECal_FGD1 += (_FGD1DSECal_Sand_Sigma_run5) * toy.GetToyVariations(_index)->Variations[0];
        else if(_RD_Run_Number==6) track->ToF.DSECal_FGD1 += (_FGD1DSECal_Sand_Sigma_run6) * toy.GetToyVariations(_index)->Variations[0];
      }
    }
  }
}

//********************************************************************
bool ToFDSECalVariationSystematic::UndoSystematic(AnaEventC& event){
//********************************************************************

  // Get the SystBox for this event
  SystBoxB* box = GetSystBox(event);

  AnaTrackB* ecalTracks[100];
  Int_t ntracks = anaUtils::GetAllTracksUsingDet(*static_cast<AnaEventB*>(&event), SubDetId::kECAL, ecalTracks);

  for (Int_t itrk=0;itrk<ntracks;itrk++){

    AnaTrackB* track = static_cast<AnaTrackB*>(ecalTracks[itrk]);

    // Go back to the corrected momentum
	if (track->ToF.Flag_DSECal_FGD1) track->ToF.DSECal_FGD1 = track->GetOriginalTrack()->ToF.DSECal_FGD1;
  }

  // Don't reset the spill to corrected
  return false;
}

