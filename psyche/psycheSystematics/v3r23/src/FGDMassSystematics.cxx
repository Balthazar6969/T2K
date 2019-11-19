#include "FGDMassSystematics.hxx"
#include "ND280AnalysisUtils.hxx"
#include "BasicUtils.hxx"
#include "ToyBoxTracker.hxx"

//********************************************************************
FGDMassSystematics::FGDMassSystematics():EventWeightBase(2){
  //********************************************************************

  Int_t baseindex=0;
  _scintmass = new BinnedParams("FGD1Mass",       BinnedParams::k1D_SYMMETRIC, versionUtils::Extension());
  baseindex +=  _scintmass->GetNBins();
  _watermass = new BinnedParams("FGD2MassWater",  BinnedParams::k1D_SYMMETRIC);
  baseindex +=_watermass->GetNBins();
  SetNParameters(baseindex);
 

  _scintmass->GetParametersForBin(0, _scintmass_corr, _scintmass_err);
  _watermass->GetParametersForBin(0, _watermass_corr, _watermass_err);
}

//********************************************************************
Weight_h FGDMassSystematics::ComputeWeight(const ToyExperiment& toy, const AnaEventC& eventBB, const ToyBoxB& boxB){
//********************************************************************

  const AnaEventB& event = *static_cast<const AnaEventB*>(&eventBB); 

  // Cast the ToyBox to the appropriate type
  const ToyBoxTracker& box = *static_cast<const ToyBoxTracker*>(&boxB); 

  Weight_h eventWeight=1;

  // True vertex associated to the recon vertex should exist
  if (!box.Vertex->TrueVertex) return eventWeight;         
  
  // Apply FGD mass systematic when the true vertex is in FGD

  // Depending on FGD1 or FGD2  
  if( box.DetectorFV==SubDetId::kFGD1){   // Event selected in FGD1 FV
    if (anaUtils::InDetVolume(SubDetId::kFGD1, box.Vertex->TrueVertex->Position)){  // TrueVertex in FGD1 FV
      eventWeight.Systematic = 1 + _scintmass_corr + _scintmass_err * toy.GetToyVariations(_index)->Variations[0];
      eventWeight.Correction = 1 + _scintmass_corr;
    }
  }
  else if( box.DetectorFV==SubDetId::kFGD2){  // Event selected in FGD2 FV
    if (anaUtils::InDetVolume(SubDetId::kFGD2, box.Vertex->TrueVertex->Position)){  // TrueVertex in FGD2 FV
      // Check water of scintilator
      if (anaUtils::GetFgdModuleTypeNoFV(event.GetIsMC(), box.Vertex->TrueVertex->Position,true) == 2){
        eventWeight.Systematic = 1 + _watermass_corr + _watermass_err * toy.GetToyVariations(_index)->Variations[1];
        eventWeight.Correction = 1 + _watermass_corr;
      }
      else{
        eventWeight.Systematic = 1 + _scintmass_corr + _scintmass_err * toy.GetToyVariations(_index)->Variations[0];
        eventWeight.Correction = 1 + _scintmass_corr;
      }
    }
  }
  return eventWeight;
}

