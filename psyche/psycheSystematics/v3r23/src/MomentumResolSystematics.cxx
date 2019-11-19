#include "MomentumResolSystematics.hxx"
#include "ND280AnalysisUtils.hxx"
#include "EventBoxTracker.hxx"
#include "VersioningUtils.hxx"
#include "Parameters.hxx"

//const bool debug = true;
//#define DEBUG true


/*
\\! [MomentumResolSystematics_info]
\htmlonly
This systematic is treated in detail in T2K-TN-222 \cite{TN-222}. The study has been performed using tracks that cross multiple TPCs, 
whose redundancy allows building a fully reconstructed observable (no truth info needed) sensitive to the intrinsic TPC resolution. 
The ultimate goal of this analysis is to compare the TPC and global momentum resolutions of data and MC, and in the case they differ, to find 
the smearing factor that makes them similar. This smearing factor would be the systematic parameter to be propagated in any event selection. 

The use of tracks crossing at least two TPCs allows to compute the difference between the momentum reconstructed using the two TPC segments of the same global track. 
Using the inverse of the transverse momentum to the magnetic field, \(1/p_t\), the distribution of its difference corrected by energy loss in the intermediate FGD \(\Delta 1/p_t\) 
is approximately Gaussian and centered at zero, with a standard deviation having as main contribuitor the intrinsic resolution of the TPCs involved. 
The resulting \(\Delta(1/p_t)\) distribution can be fitted to a Gaussian function in order to obtain the standard deviation, \(\sigma_{\Delta 1/p_t}\), 
for different kinematic ranges (\(p_t\), angle, position, etc). \(\sigma_{\Delta 1/p_t}\) contains multiple contributions,  
among which the one related to the momentum resolution should be extracted. 


\endhtmlonly
\\! [MomentumResolSystematics_info]
*/


//********************************************************************
MomentumResolSystematics::MomentumResolSystematics():EventVariationBase(1){
  //********************************************************************

  _params = new BinnedParams("MomentumResol", 
      BinnedParams::k1D_SYMMETRIC,
      versionUtils::Extension());

  // Whether to use local TPC based smearing
  _tpc_based_var = (bool)ND::params().GetParameterI("psycheSystematics.MomResol.UseTPCBased");
  _useP0DFV = (bool) ND::params().GetParameterI("psycheSystematics.Tracker.UseP0DFV");

  SetNParameters(_params->GetNBins());

  // Wether assume full correlation between syst bins
  _full_correlations = ND::params().GetParameterI("psycheSystematics.Tracker.FullCorrelations");
}

//********************************************************************
void MomentumResolSystematics::Apply(const ToyExperiment& toy, AnaEventC& event){
  //********************************************************************

  // Get the SystBox for this event
  SystBoxB* box = GetSystBox(event);

#ifdef DEBUG
  std::cout << "MomentumResolSystematics::ApplyVariation(): " << box->nRelevantRecObjects << std::endl;  
#endif

  // Loop over relevant tracks for this systematic
  for (Int_t itrk=0;itrk<box->nRelevantRecObjects;itrk++){
#ifdef DEBUG
    std::cout << itrk << std::endl;
#endif
    AnaTrackB* track = static_cast<AnaTrackB*>(box->RelevantRecObjects[itrk]);

    if (_tpc_based_var || _useP0DFV)
      ApplyVariationTPCBased(track, toy);
    else
      ApplyVariation(track, toy);
  }
}

//********************************************************************
bool MomentumResolSystematics::UndoSystematic(AnaEventC& event){
  //********************************************************************

  // Get the SystBox for this event
  SystBoxB* box = GetSystBox(event);

  for (Int_t itrk=0;itrk<box->nRelevantRecObjects;itrk++){
    AnaTrackB* track = static_cast<AnaTrackB*>(box->RelevantRecObjects[itrk]);    

    if (!track) continue;

    // Go back to the corrected momentum
    if (!track->GetOriginalTrack()) continue; 

    track->Momentum     = track->GetOriginalTrack()->Momentum;

    track->MomentumFlip = track->GetOriginalTrack()->MomentumFlip;

    for (int iseg = 0; iseg < track->nTPCSegments; iseg++){
      AnaTPCParticleB* tpcTrackorig = track->GetOriginalTrack()->TPCSegments[iseg];
      AnaTPCParticleB* tpcTrack     = track->TPCSegments[iseg];

      if (!tpcTrack || !tpcTrackorig) continue;

      tpcTrack->MomentumError = tpcTrackorig->MomentumError;
      tpcTrack->Momentum      = tpcTrackorig->Momentum;
    }

    // Go back to the corrected charge
    //    static_cast<AnaTrackB*>(tracks[itrk])->Charge = track->GetOriginalTrack()->Charge;
  }
  // Don't reset the spill to corrected
  return false;
}

//********************************************************************
bool MomentumResolSystematics::GetVariation(AnaTrackB* track, Float_t& variation, const ToyExperiment& exp){
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
    if (!GetXBinnedValues(track, value1, value2, index1, index2, MomentumResolVariation::kSyst)) return false;  
  }
  else {
    Float_t val1_tmp;
    // P dependence for prod5
    if (!_params->GetBinValues(track->Momentum, val1_tmp, value1, index1)) return false;

    value2 = value1;
    index2 = index1;
  }  

  // Full correlation if requested
  if (_full_correlations)
    index1 = index2 = 0;

  variation = 0.5 * (value1 * exp.GetToyVariations(_index)->Variations[index1] + 
      value2 * exp.GetToyVariations(_index)->Variations[index2]);

  return true;
}

//********************************************************************
bool MomentumResolSystematics::GetVariationTPC(AnaTPCParticleB* track, Float_t& variation, const ToyExperiment& exp){
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
    if (!GetXBinnedValues(track, value1, value2, index1, index2, MomentumResolVariation::kSyst)) return false;  
  }
  else {
    Float_t val1_tmp;
    // P dependence for prod5
    if (!_params->GetBinValues(track->Momentum, val1_tmp, value1, index1)) return false;

    value2 = value1;
    index2 = index1;
  }  

  // Full correlation if requested
  if (_full_correlations)
    index1 = index2 = 0;

  variation = 0.5 * (value1 * exp.GetToyVariations(_index)->Variations[index1] + 
      value2 * exp.GetToyVariations(_index)->Variations[index2]);

  return true;


}
