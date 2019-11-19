#include <stdio.h>
#include "MomResolMCCorrection.hxx"
#include "HighlandAnalysisUtils.hxx"
#include "VersioningUtils.hxx"
#include "Parameters.hxx"
//#define DEBUG


//********************************************************************
MomResolMCCorrection::MomResolMCCorrection(){
  //********************************************************************
  _params = new BinnedParams(
      std::string(getenv("PSYCHESYSTEMATICSROOT")) + "/data", 
      "MomentumResol",
      BinnedParams::k1D_SYMMETRIC, 
      versionUtils::Extension()
      );
  
  // This is to be consistent between the correction and systematic
  _tpc_based_var = (bool)ND::params().GetParameterI("psycheSystematics.MomResol.UseTPCBased");
}

//********************************************************************
void MomResolMCCorrection::Apply(AnaSpillC& spillBB){
  //********************************************************************
#ifdef DEBUG 
  std::cout << " MomentumResolMCCorrection::Apply(): IsMC " << spill.GetIsMC() << std::endl;  
#endif

  AnaSpill& spill = *static_cast<AnaSpill*>(&spillBB);

  // No correction for data
  if (!spill.GetIsMC())
    return;

  // Loop over all bunches
  for (unsigned int i = 0; i < spill.Bunches.size(); i++) {
    AnaBunch* bunch = static_cast<AnaBunch*>(spill.Bunches[i]);

    AnaTrackB* allTpcTracks[NMAXPARTICLES];

    // Get all the tracks in the TPC
    int nTPC = anaUtils::GetAllTracksUsingDet(*bunch, SubDetId::kTPC, allTpcTracks);
    for (Int_t j = 0; j < nTPC; j++){
      if (_tpc_based_var)
        ApplyVariationTPCBased(allTpcTracks[j], _exp);
      else
        ApplyVariation(allTpcTracks[j], _exp);
    }
  }


#ifdef DEBUG 
  std::cout << "MomentumResolMCCorrection::Apply() end " << std::endl;  
#endif

}


//********************************************************************
bool MomResolMCCorrection::GetVariation(AnaTrackB* track, Float_t& variation){
  //******************************************************************** 
  if (!track)
    return false;

  if (!_params)
    return false;
  
  variation = 0.;

  Float_t value1, value2;
  Int_t index1, index2;
    
  if (versionUtils::prod6_systematics){

    // Complicated method for prod6
    if (!GetXBinnedValues(track, value1, value2, index1, index2, MomentumResolVariation::kCorr)) return false;  
  }
  else {
    Float_t val1_tmp;
    // P dependence for prod5
    if (!_params->GetBinValues(track->Momentum, value1, val1_tmp)) return false;

    value2 = value1;
  }  

  variation = 0.5 * (value1 + value2);
  return true;
}

//********************************************************************
bool MomResolMCCorrection::GetVariationTPC(AnaTPCParticleB* track, Float_t& variation){
  //******************************************************************** 
  if (!track)
    return false;

  if (!_params)
    return false;
  
  variation = 0.;

  Float_t value1, value2;
  Int_t index1, index2;
    
  if (versionUtils::prod6_systematics){

    // Complicated method for prod6
    if (!GetXBinnedValues(track, value1, value2, index1, index2, MomentumResolVariation::kCorr)) return false;  
  }
  else {
    Float_t val1_tmp;
    // P dependence for prod5
    if (!_params->GetBinValues(track->Momentum, value1, val1_tmp)) return false;

    value2 = value1;
  }  

  variation = 0.5 * (value1 + value2);
  return true;
}

//********************************************************************
bool MomResolMCCorrection::IsRelevantObject(const AnaRecObjectC* object){
 //******************************************************************** 

  if (!object) return false;
  
 
  // Assuming the particles ToF reversing is already done can just run for the tracks that start in the FGDs
  // ToDo
  // 
  return true;

}
