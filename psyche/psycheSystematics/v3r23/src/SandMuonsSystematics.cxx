#include "SandMuonsSystematics.hxx"
#include "ND280AnalysisUtils.hxx"
//********************************************************************
SandMuonsSystematics::SandMuonsSystematics():BinnedParams("SandMuons",k1D_SYMMETRIC,versionUtils::Extension()){
//********************************************************************
  SetNParameters(GetNBins());
}

//********************************************************************
Weight_h SandMuonsSystematics::ComputeWeight(const ToyExperiment& toy, const AnaEventC& eventBB, const ToyBoxB& box){
//********************************************************************

  (void)box;

  Weight_h eventWeight=1;

  const AnaEventB& event = *static_cast<const AnaEventB*>(&eventBB); 

  if(!event.GetIsSandMC() ) return eventWeight; 

  // Get the sand MC mode (from 0 to 2)
  int sandMode = anaUtils::GetSandMode(event.EventInfo.Run);
  Float_t sand_corr,sand_corr_error;
  Int_t index;
  // Get the sand muons correction values for this run period 
  if (!GetBinValues(sandMode, sand_corr, sand_corr_error,index)) return eventWeight;

  // compute the weight
  eventWeight.Systematic = 1 + sand_corr + sand_corr_error * toy.GetToyVariations(_index)->Variations[index];
  eventWeight.Correction = 1 + sand_corr;
  // std::cout<<sandMode<<" "<<eventWeight<<std::endl;
  return eventWeight;
}

