#include "FluxWeightSystematics.hxx"
#include "ND280AnalysisUtils.hxx"
#include "ToyBoxND280.hxx"


//#define DEBUG

//********************************************************************
FluxWeightSystematics::FluxWeightSystematics():BinnedParams("FluxWeight",k3D_SYMMETRIC_NOMEAN){
//********************************************************************
   SetNParameters(GetNBins());

   std::cout << "Create FluxWeight in FluxWeightSystematics: " << std::endl;
   _flux = new FluxWeighting();   
}

//********************************************************************
Weight_h FluxWeightSystematics::ComputeWeight(const ToyExperiment& toy, const AnaEventC& eventBB, const ToyBoxB& boxB){
//********************************************************************

  const AnaEventB& event = *static_cast<const AnaEventB*>(&eventBB); 

  // Cast the ToyBox to the appropriate type
  const ToyBoxND280& box = *static_cast<const ToyBoxND280*>(&boxB); 

  Weight_h eventWeight = 1;

  if (event.GetIsSandMC()) return eventWeight; // No weight for sand MC
  if (!box.TrueVertex)     return eventWeight; // True vertex associated to the recon vertex should exist

  // Get the true neutrino energy
  Float_t enu  = box.TrueVertex->NuEnergy;
  int     type = box.TrueVertex->NuPDG;

  // Get the flux error values for this energy
  Float_t sigma;
  Int_t index;
  if(!GetBinSigmaValue(anaUtils::GetRunPeriod(event.EventInfo.Run), enu, type, sigma, index)) return eventWeight;
 
  // Actual mean from FluxWeighting class
  Float_t mean =  _flux->GetWeight(box.TrueVertex, anaUtils::GetRunPeriod(event.EventInfo.Run));

  // Compute the weight
  eventWeight.Systematic = 1 + (mean - 1) + sigma * toy.GetToyVariations(_index)->Variations[index];
  eventWeight.Correction = 1 + (mean - 1);

#ifdef DEBUG
  std::cout << "Run: " << event.EventInfo.Run << " Enu: " << enu << " type: " << type << " mean " << mean 
    << " sigma " << sigma << " index " << index << " variation " << toy.GetToyVariations(_index)->Variations[index] 
    << " weight " << eventWeight << std::endl;
#endif

  return eventWeight;

}

