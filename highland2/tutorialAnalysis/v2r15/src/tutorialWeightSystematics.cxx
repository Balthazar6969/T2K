#include "tutorialWeightSystematics.hxx"
#include "ND280AnalysisUtils.hxx"
#include "BasicUtils.hxx"
#include "ToyBoxTracker.hxx"

//********************************************************************
/// Default constructor
tutorialWeightSystematics::tutorialWeightSystematics():EventWeightBase(2){
  //********************************************************************
  Int_t baseindex=0;

  // With this instruction the BinnedParams class will read the file FGD1Mass_p5F or FGD1Mass_p6B (hence the versionUtils::Extension()), which has
  // a format BinnedParams::k1D_SYMMETRIC (tells BinnedParams to interprete the parameters in those files)
  _scintmass= new BinnedParams("FGD1Mass",      BinnedParams::k1D_SYMMETRIC,versionUtils::Extension());

  // Get the number of parameters read from the file
  baseindex+=_scintmass->GetNBins();

  // In the same way for the file FGD2MassWater.dat, with no production extension
  _watermass = new BinnedParams("FGD2MassWater",BinnedParams::k1D_SYMMETRIC);

  // Increment the number of parameters
  baseindex+=_watermass->GetNBins();

  // Set the total number of parameters 
  SetNParameters(baseindex);
 
  // For this particular example there is a single parameter in each of the files, get the numbers thers
  // and save them in four simple float variables that we can use later
  _scintmass->GetParametersForBin(0, _scintmass_corr, _scintmass_err);
  _watermass->GetParametersForBin(0, _watermass_corr, _watermass_err);
}

//********************************************************************
Weight_h tutorialWeightSystematics::ComputeWeight(const ToyExperiment& toy, const AnaEventC& eventC, const ToyBoxB& boxB){
//********************************************************************

  const AnaEventB& event = *static_cast<const AnaEventB*>(&eventC); 

  // Cast the ToyBox to the appropriate type
  const ToyBoxTracker& box = *static_cast<const ToyBoxTracker*>(&boxB); 

  // This is a class (WeightBase, in psycheCore) that holds to floats, a correction and a systematic (see below)
  Weight_h eventWeight=1;

  // True vertex associated to the recon vertex should exist
  if (!box.Vertex->TrueVertex) return eventWeight;         
  
  // ----- Apply FGD mass systematic only when the true vertex is in FGD --------

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
        // Use the numbers for water
        eventWeight.Systematic = 1 + _watermass_corr + _watermass_err * toy.GetToyVariations(_index)->Variations[1];
        eventWeight.Correction = 1 + _watermass_corr;
      }
      else{
        // Use the numbers for scintillator
        eventWeight.Systematic = 1 + _scintmass_corr + _scintmass_err * toy.GetToyVariations(_index)->Variations[0];
        eventWeight.Correction = 1 + _scintmass_corr;
      }
    }
  }
  return eventWeight;
}
