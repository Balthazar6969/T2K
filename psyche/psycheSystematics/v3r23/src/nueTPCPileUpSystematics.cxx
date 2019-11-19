#include "nueTPCPileUpSystematics.hxx"
#include "ND280AnalysisUtils.hxx"
#include "SystematicUtils.hxx"

//********************************************************************
nueTPCPileUpSystematics::nueTPCPileUpSystematics():EventWeightBase(1){
//********************************************************************

  _fgd1 = new BinnedParams("nueFGD1_TPC_PileUp",BinnedParams::k1D_SYMMETRIC);
  _fgd2 = new BinnedParams("nueFGD2_TPC_PileUp",BinnedParams::k1D_SYMMETRIC);
  SetNParameters(_fgd1->GetNBins()+_fgd2->GetNBins());
}

//********************************************************************
void nueTPCPileUpSystematics::Initialize(){
//********************************************************************
  
  // Register the BinnedParams to have proper offsets for each of them
  systUtils::AddBinnedParamsOffsetToSystematic(*this, *_fgd1, _fgd1->GetNBins());
  systUtils::AddBinnedParamsOffsetToSystematic(*this, *_fgd2, _fgd2->GetNBins());
  
}

//********************************************************************
Weight_h nueTPCPileUpSystematics::ComputeWeight(const ToyExperiment& toy, const AnaEventC& eventBB, const ToyBoxB& box){
//********************************************************************

  Weight_h eventWeight= 1;

  const AnaEventB& event = *static_cast<const AnaEventB*>(&eventBB); 

  if( box.DetectorFV != SubDetId::kFGD1 && box.DetectorFV != SubDetId::kFGD2) return eventWeight;

  // Get the run period (from 0 to 8)
  Int_t runPeriod = anaUtils::GetRunPeriod(event.EventInfo.Run);
  
  // MainTrack should be in FV. This condition is already in box.DetectorFV
  //  if  (!anaUtils::InFiducialVolume(box.DetectorFV, box.MainTrack->PositionStart))  return eventWeight;
  
  // Get the pileup values for this run period
  Float_t pileup;
  Float_t pileup_error;
  Int_t index;
  if     (box.DetectorFV==SubDetId::kFGD1){
    if(!_fgd1->GetBinValues(runPeriod, pileup, pileup_error, index)) return eventWeight;
    index += systUtils::GetBinnedParamsOffsetForSystematic(*this, *_fgd1);
  }
  else if(box.DetectorFV==SubDetId::kFGD2){
    if(!_fgd2->GetBinValues(runPeriod, pileup, pileup_error, index)) return eventWeight;
    index += systUtils::GetBinnedParamsOffsetForSystematic(*this, *_fgd2);
  }
  else return eventWeight;
  
  // compute the weight
  
  eventWeight.Systematic = 1 - pileup - pileup_error * toy.GetToyVariations(_index)->Variations[index];
  eventWeight.Correction = 1 - pileup;
  
  return eventWeight;
}

