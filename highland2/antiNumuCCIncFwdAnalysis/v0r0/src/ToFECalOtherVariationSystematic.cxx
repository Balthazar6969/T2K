#include "ToFECalOtherVariationSystematic.hxx"
#include "EventBoxTracker.hxx"
#include "ND280AnalysisUtils.hxx"
#include "Parameters.hxx"
#include "CutUtils.hxx"


//********************************************************************
ToFECalOtherVariationSystematic::ToFECalOtherVariationSystematic():EventVariationBase(2){
//********************************************************************

  // Read various parameters
  _RD_Run_Number = (Int_t)ND::params().GetParameterI("antiNumuControlSamples.RD_Run_Number");
  _Is_Sand_MC = (Int_t)ND::params().GetParameterI("antiNumuControlSamples.Is_Sand_MC");

  // Read various parameters
  _FGD1ECalOther_MC_Sigma_run5 = (Float_t)ND::params().GetParameterD("antiNumuControlSamples.Variations.Other.ToF_ECal_Other_MC_Sigma_run5");
  _FGD1ECalOther_MC_Sigma_run6 = (Float_t)ND::params().GetParameterD("antiNumuControlSamples.Variations.Other.ToF_ECal_Other_MC_Sigma_run6");
  _FGD1ECalOther_Sand_Sigma_run5 = (Float_t)ND::params().GetParameterD("antiNumuControlSamples.Variations.Other.ToF_ECal_Other_Sand_Sigma_run5");
  _FGD1ECalOther_Sand_Sigma_run6 = (Float_t)ND::params().GetParameterD("antiNumuControlSamples.Variations.Other.ToF_ECal_Other_Sand_Sigma_run6");

}

//********************************************************************
void ToFECalOtherVariationSystematic::Apply(const ToyExperiment& toy, AnaEventC& event){
//********************************************************************

  // Get the SystBox for this event
  SystBoxB* box = GetSystBox(event);

  AnaTrackB* ecalTracks[100];
  Int_t ntracks = anaUtils::GetAllTracksUsingDet(*static_cast<AnaEventB*>(&event), SubDetId::kECAL, ecalTracks);

  for (Int_t itrk=0;itrk<ntracks;itrk++){

    AnaTrackB* track = static_cast<AnaTrackB*>(ecalTracks[itrk]);

	if (track->Time.Flag_ECal) {

      if(!_Is_Sand_MC) {
        if(_RD_Run_Number==5) track->Time.ECal += (_FGD1ECalOther_MC_Sigma_run5) * toy.GetToyVariations(_index)->Variations[0];
        else if(_RD_Run_Number==6) track->Time.ECal += (_FGD1ECalOther_MC_Sigma_run6) * toy.GetToyVariations(_index)->Variations[0];
      }
      else{
        if(_RD_Run_Number==5) track->Time.ECal += (_FGD1ECalOther_Sand_Sigma_run5) * toy.GetToyVariations(_index)->Variations[0];
        else if(_RD_Run_Number==6) track->Time.ECal += (_FGD1ECalOther_Sand_Sigma_run6) * toy.GetToyVariations(_index)->Variations[0];
      }
    }
    if (track->Time.Flag_DSECal) {

      if(!_Is_Sand_MC) {
        if(_RD_Run_Number==5) track->Time.DSECal += (_FGD1ECalOther_MC_Sigma_run5) * toy.GetToyVariations(_index)->Variations[0];
        else if(_RD_Run_Number==6) track->Time.DSECal += (_FGD1ECalOther_MC_Sigma_run6) * toy.GetToyVariations(_index)->Variations[0];
      }
      else{
        if(_RD_Run_Number==5) track->Time.DSECal += (_FGD1ECalOther_Sand_Sigma_run5) * toy.GetToyVariations(_index)->Variations[0];
        else if(_RD_Run_Number==6) track->Time.DSECal += (_FGD1ECalOther_Sand_Sigma_run6) * toy.GetToyVariations(_index)->Variations[0];
      }
    }
  }
}

//********************************************************************
bool ToFECalOtherVariationSystematic::UndoSystematic(AnaEventC& event){
//********************************************************************

  // Get the SystBox for this event
  SystBoxB* box = GetSystBox(event);

  AnaTrackB* ecalTracks[100];
  Int_t ntracks = anaUtils::GetAllTracksUsingDet(*static_cast<AnaEventB*>(&event), SubDetId::kECAL, ecalTracks);

  for (Int_t itrk=0;itrk<ntracks;itrk++){

    AnaTrackB* track = static_cast<AnaTrackB*>(ecalTracks[itrk]);
    
    // Go back to the corrected momentum
    if(track->Time.Flag_ECal)
      track->Time.ECal = track->GetOriginalTrack()->Time.ECal;

    if(track->Time.Flag_DSECal)
      track->Time.DSECal = track->GetOriginalTrack()->Time.DSECal;
  }

  // Don't reset the spill to corrected
  return false;
}

