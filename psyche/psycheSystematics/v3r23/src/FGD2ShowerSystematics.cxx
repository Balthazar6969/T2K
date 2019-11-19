#include "FGD2ShowerSystematics.hxx"
#include "CutUtils.hxx"
#include "SystematicUtils.hxx"
#include "SystId.hxx"
#include "EventBoxId.hxx"

//#define DEBUG

//********************************************************************
FGD2ShowerSystematics::FGD2ShowerSystematics(bool comp):BinnedParams("FGD2Shower", k2D_EFF_ASSYMMETRIC){
//********************************************************************

  _computecounters = comp;
  SetNParameters(2*GetNBins());
  if(_computecounters)
    InitializeEfficiencyCounter();
}

//********************************************************************
Weight_h FGD2ShowerSystematics::ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& boxB, const SelectionBase& sel){
//********************************************************************


  if(_computecounters)
    InitializeEfficiencyCounter();

  (void)event;

  // Get the SystBox for this event, and the appropriate selection and branch
  SystBoxB* SystBox = GetSystBox(event, boxB.SelectionEnabledIndex, boxB.SuccessfulBranch);

  Weight_h eventWeight = 1.;

#ifdef DEBUG
  std::cout << " \n FGD2ShowerSystematics::Apply(): " << SystBox->nRelevantRecObjects<< std::endl; 
#endif

  if(boxB.DetectorFV != SubDetId::kFGD1) return eventWeight;

  // Loop over relevant tracks
  for (Int_t itrk = 0; itrk < SystBox->nRelevantRecObjects; itrk++){

    AnaTrackB* track = static_cast<AnaTrackB*>(SystBox->RelevantRecObjects[itrk]);
    if (!track) continue;

    if (!sel.IsRelevantRecObjectForSystematicInToy(event, boxB, track, SystId::kFGD2Shower, boxB.SuccessfulBranch)) continue;

    // Should have a true track
    AnaTrueParticleB* truePart = track->GetTrueParticle();
    if (!truePart) continue;

    int pdg = abs(truePart->PDG);
    
    BinnedParamsParams params;
    int index;

    //if (!GetBinValues(pdg, truePart->Momentum, params, index)) continue;
    if (!GetBinValues(pdg, track->Momentum, params, index)) continue;

    bool found = true;

    // Found the correspondence,  now get the weight
    eventWeight *= systUtils::ComputeEffLikeWeight(found, toy, GetIndex(), index, params);


#ifdef DEBUG
    std::cout<<"FGD2Shower  " << found << " eventWeight local " << eventWeight << std::endl; 
#endif

    if(_computecounters)
      UpdateEfficiencyCounter(index,true);    

  }

  return eventWeight;
}


//********************************************************************
bool FGD2ShowerSystematics::IsRelevantRecObject(const AnaEventC& event, const AnaRecObjectC& recObj) const{
  //********************************************************************

  (void)event;

  const AnaTrackB& track = *static_cast<const AnaTrackB*>(&recObj);

  if(!track.TrueObject) return false;
  if(track.nFGDSegments==0) return false;
  //if(anaUtils::InFiducialVolume(SubDetId::kFGD2, track.PositionStart)) return false;

  return true;

}

