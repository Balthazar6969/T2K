#include "FGDHybridTrackEffSystematics.hxx"
#include "ND280AnalysisUtils.hxx"
#include "EventBoxTracker.hxx"
#include "SystematicUtils.hxx"
#include "ToyBoxTracker.hxx"
#include "SystId.hxx"
#include "EventBoxId.hxx"


//********************************************************************
FGDHybridTrackEffSystematics::FGDHybridTrackEffSystematics(bool comp):EventWeightBase(1){
//********************************************************************

  _computecounters=comp;
  _fgd1 = new BinnedParams("FGD1HybridTrackEff_p6B",BinnedParams::k2D_EFF_ASSYMMETRIC);
  _fgd2 = new BinnedParams("FGD2HybridTrackEff_p6B",BinnedParams::k2D_EFF_ASSYMMETRIC);
  
  SetNParameters(systUtils::GetNumberSystParams(*_fgd1, true) + 
      systUtils::GetNumberSystParams(*_fgd2, true));
    
  if(_computecounters){
    _fgd1->InitializeEfficiencyCounter();
    _fgd2->InitializeEfficiencyCounter();
  }
}

//********************************************************************
void FGDHybridTrackEffSystematics::Initialize(){
//********************************************************************
 
  //Register BinnedParams to get proper offsets
  systUtils::AddBinnedParamsOffsetToSystematic(*this, *_fgd1, systUtils::GetNumberSystParams(*_fgd1, true));
  systUtils::AddBinnedParamsOffsetToSystematic(*this, *_fgd2, systUtils::GetNumberSystParams(*_fgd2, true));
  
}


//********************************************************************
Weight_h FGDHybridTrackEffSystematics::ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& boxB){
//********************************************************************

  Weight_h eventWeight=1; 
  BinnedParamsParams params;

  // Cast the ToyBox to the appropriate type
  const ToyBoxTracker& box = *static_cast<const ToyBoxTracker*>(&boxB); 

  // Get the SystBox for this event, and the appropriate selection and branch
  SystBoxB* SystBox = GetSystBox(event,box.SelectionEnabledIndex,box.SuccessfulBranch);

  if(_computecounters){
    _fgd1->InitializeEfficiencyCounter();
    _fgd2->InitializeEfficiencyCounter();
  }
  // Get the direction of the main track
  AnaTrackB *maintrack = box.MainTrack;	
  if(!maintrack)  return eventWeight;

  for (Int_t itrue=0;itrue< SystBox->nRelevantTrueObjects; itrue++){      
    AnaTrueParticleB* trueTrack = static_cast<AnaTrueParticleB*>(SystBox->RelevantTrueObjects[itrue]);            

    // TODO by now the reco track associated to the true is the first one found, we should use instead a criteria to decide.
    // by now since it's very rare it's like that.
    // retrieve the reconstructed track associated
    AnaTrackB* recoTrack = static_cast<AnaTrackB*>(SystBox->RelevantTrueObjectsReco[itrue]);

    // Get the cos (theta), where theta is the angle between the main track and the FGD track
    Float_t costheta = 0;
    if (recoTrack)
      costheta = anaUtils::ScalarProduct(maintrack->DirectionStart, recoTrack->DirectionStart);
    else
      costheta = anaUtils::ScalarProduct(maintrack->DirectionStart, trueTrack->Direction);

    Float_t pdg=(Float_t)trueTrack->PDG;
    bool found = (recoTrack);
    
    // Get the FGD tracking efficiency
    Int_t index;
    if(box.DetectorFV==SubDetId::kFGD1){
      if(!_fgd1->GetBinValues(fabs(pdg),costheta, params, index)) continue;
      // save the index of the varied parameters into the systematics box
      if(_computecounters)
        _fgd1->UpdateEfficiencyCounter(index,found); 
      
      // Add proper offset to the index
      index += systUtils::GetBinnedParamsOffsetForSystematic(*this, *_fgd1);
      
    }
    else if(box.DetectorFV==SubDetId::kFGD2){
      
      if(!_fgd2->GetBinValues(fabs(pdg),costheta, params, index)) continue;
      if(_computecounters)
        _fgd2->UpdateEfficiencyCounter(index,found);    
      
      // Add proper offset to the index
      index += systUtils::GetBinnedParamsOffsetForSystematic(*this, *_fgd2);
    }
    else return eventWeight;
    
    eventWeight *= systUtils::ComputeEffLikeWeight(true, toy, GetIndex(), index, params);
  }

  return eventWeight;

}

//********************************************************************
bool FGDHybridTrackEffSystematics::IsRelevantTrueObject(const AnaEventC& event, const AnaTrueObjectC& trueTrack) const{
//********************************************************************

  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];

  //we should not take into account the tracks that are matched to the tpc,
  //this is done in tpc-fgd matching efficiency  
  for (Int_t i=0;i<EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithTPC];i++){
    AnaRecObjectC* track = EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithTPC][i];
    if (CheckTrueRecoAssociation(trueTrack, *track))
      return false;
  }

  return true;
}
