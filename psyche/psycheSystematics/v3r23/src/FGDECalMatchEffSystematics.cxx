#include "FGDECalMatchEffSystematics.hxx"
#include "CutUtils.hxx"
#include "SystematicUtils.hxx"
#include "SystId.hxx"

//#define DEBUG


//********************************************************************
FGDECalMatchEffSystematics::FGDECalMatchEffSystematics(bool comp):BinnedParams("FGDECalMatchEff",k3D_EFF_ASSYMMETRIC){
  //********************************************************************

  _computecounters=comp;
  SetNParameters(2*GetNBins());
  if(_computecounters)
    InitializeEfficiencyCounter();
}

//********************************************************************
FGDECalMatchEffSystematics::FGDECalMatchEffSystematics(bool computecounters, const std::string& name, BinnedParams::TypeEnum type):
  BinnedParams(name, type){
  //********************************************************************
  _computecounters = computecounters;
  SetNParameters(2*GetNBins());
  if(_computecounters)
    InitializeEfficiencyCounter();
}

//********************************************************************
Weight_h FGDECalMatchEffSystematics::ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box, const SelectionBase& sel){
  //********************************************************************


  if(_computecounters)
    InitializeEfficiencyCounter();


  // Get the SystBox for this event, and the appropriate selection and branch
  SystBoxB* SystBox = GetSystBox(event, box.SelectionEnabledIndex, box.SuccessfulBranch);

  Weight_h eventWeight = 1.;

#ifdef DEBUG
  std::cout << "FGDECalMatchEffSystematics::Apply(): " << SystBox->nRelevantTrueObjects<< std::endl; 
#endif

  // Loop over relevant true tracks
  for (Int_t itrk = 0; itrk < SystBox->nRelevantTrueObjects; itrk++){
    
    AnaTrueParticleB* truePart  = static_cast<AnaTrueParticleB*>(SystBox->RelevantTrueObjects[itrk]);

    AnaTrackB*        recoTrack = static_cast<AnaTrackB*>(SystBox->RelevantTrueObjectsReco[itrk]);

    if (!truePart || !recoTrack) continue;
    
    
    // Do fine-tuning of the track relevance via the selection
    if (!sel.IsRelevantTrueObjectForSystematicInToy(event, box, truePart, SystId::kFgdECalMatchEff, box.SuccessfulBranch)) continue;
    

    SubDetId::SubDetEnum fgd_det[5];

    int nfgd_det = anaUtils::GetFGDDetCrossed(truePart, fgd_det); 

    if(nfgd_det==0) continue;

    // Get ecal segment (wont work if call for TECAL)
    AnaParticleB* ecal = anaUtils::GetSegmentWithMostNodesInDet(*(recoTrack), SubDetId::kECAL);  

    if(!ecal)
      continue; 

    Float_t* dir = anaUtils::GetSLineDir(ecal->PositionStart, ecal->PositionEnd);

    // Check the direction of the reco track 
    dir[2] *= ((recoTrack->DirectionStart[2]*dir[2] >= 0) ? 1. : -1.);

    BinnedParamsParams params;
    int index;

    if (!GetBinValues(fgd_det[0], SubDetId::GetSubdetectorEnum(ecal->Detector), dir[2], params, index)) continue;

    // Found the correspondence,  now check detector bits
    bool found = SubDetId::GetDetectorUsed(recoTrack->Detector, fgd_det[0]);

    eventWeight *= systUtils::ComputeEffLikeWeight(found, toy, GetIndex(), index, params);

#ifdef DEBUG
    std::cout<<"fgd-ecal found "<< found<<" eventWeight "<<eventWeight<<std::endl; 
#endif

    if(_computecounters)
      UpdateEfficiencyCounter(index,found);    

  }

  return eventWeight;
}

//********************************************************************
bool FGDECalMatchEffSystematics::IsRelevantTrueObject(const AnaEventC& event, const AnaTrueObjectC& track) const{
  //********************************************************************
  
  (void)event;
  
  //should cross at least two layers in FGD
  if (anaUtils::TrueParticleCrossesFGD(static_cast<const AnaTrueParticleB*>(&track)))
    return true;

  return false;

}

//**************************************************
bool FGDECalMatchEffSystematics::IsRelevantRecObject(const AnaEventC& event, const AnaRecObjectC& track) const{
  //**************************************************

  (void)event;
  //should use Barrel ECal since ECal iso-tracking efficiency is treated elsewhere

  if (SubDetId::GetDetectorUsed(  track.Detector,   SubDetId::kTECAL)   && 
      !SubDetId::GetDetectorUsed( track.Detector,   SubDetId::kDSECAL)  && 
      !SubDetId::GetDetectorUsed( track.Detector,   SubDetId::kPECAL)   && 
      !SubDetId::GetDetectorUsed( track.Detector,   SubDetId::kTPC))
    return true;   

  return false; 
}
