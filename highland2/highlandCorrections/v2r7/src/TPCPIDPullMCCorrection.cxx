#include <stdio.h>
#include "TPCPIDPullMCCorrection.hxx"
#include "HighlandAnalysisUtils.hxx"
#include "VersioningUtils.hxx"

//#define DEBUG


//********************************************************************
TPCPIDPullMCCorrection::TPCPIDPullMCCorrection(){
  //********************************************************************


  // Read the systematic source parameters from the data files: a special case of sharing the data file with systematics
  _sigma[TPCPIDVariation::kMuon]     = 
    new BinnedParams(std::string(getenv("PSYCHESYSTEMATICSROOT")) + "/data", "TPCPIDMuonSigRatio",   BinnedParams::k2D_SYMMETRIC, versionUtils::Extension());
  _sigma[TPCPIDVariation::kElectron] = 
    new BinnedParams(std::string(getenv("PSYCHESYSTEMATICSROOT")) + "/data", "TPCPIDEleSigRatio",    BinnedParams::k2D_SYMMETRIC, versionUtils::Extension());
  _sigma[TPCPIDVariation::kProton]   = 
    new BinnedParams(std::string(getenv("PSYCHESYSTEMATICSROOT")) + "/data", "TPCPIDProtonSigRatio", BinnedParams::k2D_SYMMETRIC, versionUtils::Extension());
  _mean[TPCPIDVariation::kMuon]      = 
    new BinnedParams(std::string(getenv("PSYCHESYSTEMATICSROOT")) + "/data", "TPCPIDMuonMeanDiff",   BinnedParams::k2D_SYMMETRIC, versionUtils::Extension());
  _mean[TPCPIDVariation::kElectron]  = 
    new BinnedParams(std::string(getenv("PSYCHESYSTEMATICSROOT")) + "/data", "TPCPIDEleMeanDiff",    BinnedParams::k2D_SYMMETRIC, versionUtils::Extension());
  _mean[TPCPIDVariation::kProton]    = 
    new BinnedParams(std::string(getenv("PSYCHESYSTEMATICSROOT")) + "/data", "TPCPIDProtonMeanDiff", BinnedParams::k2D_SYMMETRIC, versionUtils::Extension());
}

//********************************************************************
void TPCPIDPullMCCorrection::Apply(AnaSpillC& spillBB){
  //********************************************************************

  AnaSpill& spill = *static_cast<AnaSpill*>(&spillBB);

#ifdef DEBUG 
  std::cout << " TPCPIDPullMCCorrection::Apply(): IsMC " << spill.GetIsMC() << std::endl;  
#endif


  // No correction for data
  if (!spill.GetIsMC())
    return;

  // Loop over all bunches
  for (unsigned int i = 0; i < spill.Bunches.size(); i++) {

    AnaBunch* bunch = static_cast<AnaBunch*>(spill.Bunches[i]);
    AnaTrackB* allTpcTracks[NMAXPARTICLES];

    // Get all the tracks in the TPC
    int nTPC = anaUtils::GetAllTracksUsingDet(*bunch, SubDetId::kTPC, allTpcTracks);
    for (Int_t j = 0; j < nTPC; j++)
      ApplyVariation(allTpcTracks[j], _exp);
  }


#ifdef DEBUG 
  std::cout << "TPCPIDPullMCCorrection::Apply() end " << std::endl;  
#endif

}


//********************************************************************
bool TPCPIDPullMCCorrection::GetVariation(const AnaTPCParticleB& tpcTrack, 
    Float_t& mean_var, Float_t& sigma_var, 
    const AnaTrackB& track){
  //******************************************************************** 

  // Get the TPC 
  int tpc = SubDetId::GetTPC(tpcTrack.Detector);
  
  // Need true particle
  if (!track.GetTrueParticle()) return false; 

  // Get the expected dEdx and error on the dEdx depending on the true particle of the 
  // (global) track
  
  Int_t PDG = abs(track.GetTrueParticle()->PDG);
  
  TPCPIDVariation::HypEnum part; 
  
  switch (PDG){
    case 13: // Muon
    case 211:
      part = TPCPIDVariation::kMuon;
      break;    
    case 11: // Electron
      part = TPCPIDVariation::kElectron;
      break;
    case 2212: // Proton
      part = TPCPIDVariation::kProton;
      break;
    default:
      return false;
      break;
  }
  
  if (!_mean[part] || !_sigma[part]) return false;
  
  // Need the correction part from the systematics
  Float_t mean_err, sigma_err;
  
  // Note that the momentum changes if the mom resoltion, scale and bfield are also anabled.
	if (!_mean[part]->GetBinValues(track.Momentum, (Float_t) tpc, mean_var, mean_err))  return false;
	if (!_sigma[part]->GetBinValues(track.Momentum,(Float_t) tpc, sigma_var, sigma_err)) return false;

  return true;
}


