#include "TPCP0DMatchEffSystematics.hxx"
#include "CutUtils.hxx"
#include "SystematicUtils.hxx"
#include "SystId.hxx"
#include <cmath>

//#define DEBUG

//********************************************************************
TPCP0DMatchEffSystematics::TPCP0DMatchEffSystematics(bool comp):BinnedParams("TPCP0DMatchEff",k2D_EFF_ASSYMMETRIC){
  //********************************************************************

  _computecounters=comp;
  SetNParameters(2*GetNBins());
  if(_computecounters)
    InitializeEfficiencyCounter();

  //Fill graphs to interpolate
  
}

//********************************************************************
Weight_h TPCP0DMatchEffSystematics::ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box, const SelectionBase& sel){
  //********************************************************************


  if(_computecounters)
    InitializeEfficiencyCounter();

  (void)event;

  // Get the SystBox for this event, and the appropriate selection and branch
  SystBoxB* SystBox = GetSystBox(event, box.SelectionEnabledIndex, box.SuccessfulBranch);


  Weight_h eventWeight = 1.;

#ifdef DEBUG
  std::cout << "TPCP0DMatchEffSystematics::Apply(): " << SystBox->nRelevantTrueObjects<< std::endl; 
#endif

  // Loop over relevant true tracks
  for (Int_t itrk=0;itrk<SystBox->nRelevantTrueObjects;itrk++){

    AnaTrueParticleB* truePart  = static_cast<AnaTrueParticleB*>(SystBox->RelevantTrueObjects[itrk]);
    
    AnaTrackB*        recoTrack = static_cast<AnaTrackB*>(SystBox->RelevantTrueObjectsReco[itrk]);

    if (!truePart || !recoTrack) continue;
    
    // For example in numuCC inclusive selection, only the TrueTrack associated to the muon candidate, and other true muon tracks should be considered
    if (!sel.IsRelevantTrueObjectForSystematicInToy(event, box, truePart, SystId::kTpcP0dMatchEff, box.SuccessfulBranch)) continue;
    
    AnaTPCParticleB* tpcTrack = static_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInDet(*recoTrack, SubDetId::kTPC1));
  
    if(!tpcTrack) continue;
   
    if (!tpcTrack) continue;
    
    BinnedParamsParams params;
    int index;
    
    if (!GetBinValues(13, tpcTrack->Momentum, params, index)) continue;

    // Now check detector bits
    bool found = SubDetId::GetDetectorUsed(recoTrack->Detector, SubDetId::kP0D);

    eventWeight *= systUtils::ComputeEffLikeWeight(found, toy, GetIndex(), index, params);

#ifdef DEBUG
    std::cout<<"tpc1-p0d found "<< found<<" eventWeight "<<eventWeight<<std::endl; 
#endif

    if(_computecounters)
      UpdateEfficiencyCounter(index,found);    

  }

  return eventWeight;
}

//********************************************************************
bool TPCP0DMatchEffSystematics::IsRelevantTrueObject(const AnaEventC& event, const AnaTrueObjectC& track) const{
  //********************************************************************

  (void)event;

  //for the moment just require a track to enter P0D 
  if (!anaUtils::TrueParticleEntersDet(static_cast<const AnaTrueParticleB*>(&track), SubDetId::kP0D)) return false;
  
  return true;

}

//**************************************************
bool TPCP0DMatchEffSystematics::IsRelevantRecObject(const AnaEventC& event, const AnaRecObjectC& track) const{
  //**************************************************

  (void)event;

  //should use TPC1: probably good quality as well?
  AnaParticleB* tpcTrack = anaUtils::GetSegmentWithMostNodesInDet(*static_cast<const AnaTrackB*>(&track), SubDetId::kTPC1);
  
  if(!tpcTrack) return false;
  
  if (!cutUtils::TPCTrackQualityCut(*tpcTrack)) return false;
    
  return true; 
}

