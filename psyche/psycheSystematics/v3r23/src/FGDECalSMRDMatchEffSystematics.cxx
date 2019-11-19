#include "FGDECalSMRDMatchEffSystematics.hxx"
#include "CutUtils.hxx"
#include "SystematicUtils.hxx"
#include "SystId.hxx"

//#define DEBUG


//********************************************************************
FGDECalSMRDMatchEffSystematics::FGDECalSMRDMatchEffSystematics(bool computecounters):
  FGDECalMatchEffSystematics(computecounters, "FGDECalSMRDMatchEff", k2D_EFF_ASSYMMETRIC){
  //********************************************************************

}

//********************************************************************
Weight_h FGDECalSMRDMatchEffSystematics::ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box, const SelectionBase& sel){
  //********************************************************************


  if(_computecounters)
    InitializeEfficiencyCounter();

  (void)event;

  // Get the SystBox for this event, and the appropriate selection and branch
  SystBoxB* SystBox = GetSystBox(event, box.SelectionEnabledIndex, box.SuccessfulBranch);

  Weight_h eventWeight = 1.;

#ifdef DEBUG
  std::cout << "FGDECalSMRDMatchEffSystematics::Apply(): " << SystBox->nRelevantTrueObjects<< std::endl; 
#endif

  // Loop over relevant true tracks
  for (Int_t itrk=0;itrk<SystBox->nRelevantTrueObjects;itrk++){

    AnaTrueParticleB* truePart  = static_cast<AnaTrueParticleB*>(SystBox->RelevantTrueObjects[itrk]);

    AnaTrackB*        recoTrack = static_cast<AnaTrackB*>(SystBox->RelevantTrueObjectsReco[itrk]);

    if (!truePart || !recoTrack) continue;
    
    // Do fine-tuning of the track relevance via the selection
    if (!sel.IsRelevantTrueObjectForSystematicInToy(event, box, truePart, SystId::kFgdECalSmrdMatchEff, box.SuccessfulBranch)) continue;
    
    SubDetId::SubDetEnum smrd_det[5];

    int nsmrd_det = anaUtils::GetSMRDDetCrossed(truePart, smrd_det); 

    if(nsmrd_det==0) continue;

    // Get ecal segment (wont work if call for TECAL)
    AnaParticleB* ecal = anaUtils::GetSegmentWithMostNodesInDet(*(recoTrack), SubDetId::kECAL);  

    if(!ecal)
      continue; 

    Float_t* dir = anaUtils::GetSLineDir(ecal->PositionStart, ecal->PositionEnd);

    // Check the direction of the reco track 
    dir[2] *= ((recoTrack->DirectionStart[2]*dir[2] >= 0) ? 1. : -1.);

    BinnedParamsParams params;
    int index;

    if (!GetBinValues(smrd_det[0], dir[2], params, index)) continue;

    // Found the correspondence,  now check detector bits
    bool found = SubDetId::GetDetectorUsed(recoTrack->Detector, smrd_det[0]);
    
    eventWeight *= systUtils::ComputeEffLikeWeight(found, toy, GetIndex(), index, params);

#ifdef DEBUG
    std::cout<<"fgd-ecal-smrd found "<< found<<" eventWeight "<<eventWeight<<std::endl; 
#endif

    if(_computecounters)
      UpdateEfficiencyCounter(index,found);    

  }

  return eventWeight;
}

//********************************************************************
bool FGDECalSMRDMatchEffSystematics::IsRelevantTrueObject(const AnaEventC& event, const AnaTrueObjectC& track) const{
  //********************************************************************
  
  (void)event;
  
  //should cross SMRD so to be potentially reconstructable
  if (anaUtils::TrueParticleCrossesSMRD(static_cast<const AnaTrueParticleB*>(&track)))
    return true;

  return false;

}

//**************************************************
bool FGDECalSMRDMatchEffSystematics::IsRelevantRecObject(const AnaEventC& event, const AnaRecObjectC& track) const{
  //**************************************************

  (void)event;
  //should use Barrel ECal since ECal iso-tracking efficiency is treated elsewhere

  if (SubDetId::GetDetectorUsed(  track.Detector,   SubDetId::kFGD)     &&
      SubDetId::GetDetectorUsed(  track.Detector,   SubDetId::kTECAL)   &&
      !SubDetId::GetDetectorUsed( track.Detector,   SubDetId::kDSECAL)  && 
      !SubDetId::GetDetectorUsed( track.Detector,   SubDetId::kPECAL)   && 
      !SubDetId::GetDetectorUsed( track.Detector,   SubDetId::kTPC))
    return true;   

  return false; 
}


