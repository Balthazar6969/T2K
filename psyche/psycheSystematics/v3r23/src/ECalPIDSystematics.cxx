#include "ECalPIDSystematics.hxx"
#include "CutUtils.hxx"
#include "SystematicUtils.hxx"
#include "SystId.hxx"
#include "EventBoxId.hxx"


//#define DEBUG


//********************************************************************
ECalPIDSystematics::ECalPIDSystematics(bool comp):BinnedParams("ECalPID", k3D_EFF_ASSYMMETRIC){
  //********************************************************************

  _computecounters = comp;
  SetNParameters(2*GetNBins());
  if(_computecounters)
    InitializeEfficiencyCounter();
}

//********************************************************************
Weight_h ECalPIDSystematics::ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box, const SelectionBase& sel){
  //********************************************************************


  if(_computecounters)
    InitializeEfficiencyCounter();

  (void)event;

  // Get the SystBox for this event, and the appropriate selection and branch
  SystBoxB* SystBox = GetSystBox(event, box.SelectionEnabledIndex, box.SuccessfulBranch);

  Weight_h eventWeight = 1.;

#ifdef DEBUG
  std::cout << " \n ECalPIDSystematics::Apply(): " << SystBox->nRelevantRecObjects<< std::endl; 
#endif

  AnaECALParticleB* ecalTracks[NMAXPARTICLES];

  // Loop over relevant tracks
  for (Int_t itrk = 0; itrk < SystBox->nRelevantRecObjects; itrk++){

    AnaTrackB* track = static_cast<AnaTrackB*>(SystBox->RelevantRecObjects[itrk]);

    if (!track) continue;

    if (!sel.IsRelevantRecObjectForSystematicInToy(event, box, track, SystId::kECalPID, box.SuccessfulBranch)) continue;

    // Should have a true track
    AnaTrueParticleB* trueTrack = track->GetTrueParticle();
    if (!trueTrack) continue;

    // Get the ECal segment
    int count = anaUtils::GetTrackerDsEcals(track, ecalTracks);

    if (count < 1) continue;

    AnaECALParticleB* ecalTrack = ecalTracks[0];
    if (!ecalTrack) continue;

    // Protection against tracks with no physical values
    if(ecalTrack->PIDMipEm <= -999) continue;

    Float_t PIDMipEm = ecalTrack->PIDMipEm;

    // Get the PDG
    int pdg = abs(trueTrack->PDG);   

    BinnedParamsParams params;
    int index;

    // Do not have a pion control sample, same as muons for now - need to revise
    if (pdg == 211) pdg = 13;

    if (!GetBinValues(pdg, SubDetId::GetSubdetectorEnum(ecalTrack->Detector), trueTrack->Momentum, params, index)) continue;

    // The systematics is estimated in the following way: given a certain PID,  what is the efficiency to reconstruct it as
    // a shower like object (i.e. PIDMipEm>0),  these very numbers are stored in the data file, 
    // hence relatively large numbers for electrons and small ones for muons,  we then base the systematic
    // propagation on the pure eff-like case,  depending on the parameter sign we use the proper efficiency number:
    // eff or 1-eff
   
    // No inefficiency based on what said above
    bool found = true;

    // Use "proper" eff/1-eff value depending on the sign of PIDMipEm, once the uncertainties are gaussian,  should be fine!
    if (PIDMipEm < 0 ){ // track case
      params.meanDATA = 1 - params.meanDATA;
      params.meanMC   = 1 - params.meanMC;
    }
     
    // Found the correspondence,  now get the weight
    Weight_h eventWeight_tmp = 1.;
#if useNewWeights 
    eventWeight_tmp = systUtils::ComputeEffLikeWeight(found, toy.GetToyVariations(_index)->Variations[index], params); // New way including data-mc diff
#else
    eventWeight_tmp = systUtils::ComputeEffLikeWeight(found, toy.GetToyVariations(_index)->Variations[2*index],
        toy.GetToyVariations(_index)->Variations[2*index+1], params);
#endif

#ifdef DEBUG
    std::cout<<"ecal track PIDMipEm  "<< found << " eventWeight local " << eventWeight_tmp << std::endl; 
#endif

    // Update the total weight
    eventWeight *= eventWeight_tmp;

    if(_computecounters)
      UpdateEfficiencyCounter(index,found);    

  }

#ifdef DEBUG
  std::cout << " eventWeight total " << eventWeight << std::endl; 
#endif

  return eventWeight;
}


//********************************************************************
bool ECalPIDSystematics::IsRelevantRecObject(const AnaEventC& event, const AnaRecObjectC& recObj) const{
  //********************************************************************

  (void)event;

  const AnaTrackB& track = *static_cast<const AnaTrackB*>(&recObj);

  // Should use exactly one Tracker/DsECal detector --> optimized for tracker analysis at the moment 
  int count = (int)(anaUtils::TrackUsesDet(track, SubDetId::kTopTECAL)) + 
    (int)(anaUtils::TrackUsesDet(track, SubDetId::kBottomTECAL))      + 
    (int)(anaUtils::TrackUsesDet(track, SubDetId::kLeftTECAL))        + 
    (int)(anaUtils::TrackUsesDet(track, SubDetId::kRightTECAL))       + 
    (int)(anaUtils::TrackUsesDet(track, SubDetId::kDSECAL));

  if (count != 1) return false;

  return true;

}
