#include "FGDTrackEffSystematics.hxx"
#include "ND280AnalysisUtils.hxx"
#include "EventBoxTracker.hxx"
#include "SystematicUtils.hxx"
#include "SystId.hxx"
#include "EventBoxId.hxx"

//********************************************************************
FGDTrackEffSystematics::FGDTrackEffSystematics(bool comp):EventWeightBase(1){
//********************************************************************
  
  _fgd1 = NULL;
  _fgd2 = NULL;
  
  _computecounters = comp;
  _fgd1 = new BinnedParams("FGD1TrackEff_p5F",BinnedParams::k1D_EFF_SYMMETRIC);
  //_fgd2 = new BinnedParams("FGD2TrackEff_p5F",BinnedParams::k1D_EFF_SYMMETRIC);
  if( _computecounters) {
   _fgd1->InitializeEfficiencyCounter();
   //_fgd2->InitializeEfficiencyCounter();
  }
  //SetNParameters(systUtils::GetNumberSystParams(*_fgd1, true) + 
  //    systUtils::GetNumberSystParams(*_fgd2, true));

  SetNParameters(systUtils::GetNumberSystParams(*_fgd1, true));
}
//********************************************************************
void FGDTrackEffSystematics::Initialize(){
//********************************************************************

  //Register BinnedParams to get proper offsets
  systUtils::AddBinnedParamsOffsetToSystematic(*this, *_fgd1, systUtils::GetNumberSystParams(*_fgd1, true));
//  systUtils::AddBinnedParamsOffsetToSystematic(*this, *_fgd2, systUtils::GetNumberSystParams(*_fgd2, true));
   
}


//********************************************************************
Weight_h FGDTrackEffSystematics::ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box){
//********************************************************************

  // Get the SystBox for this event, and the appropriate selection and branch
  SystBoxB* SystBox = GetSystBox(event,box.SelectionEnabledIndex,box.SuccessfulBranch);

  if(_computecounters){ 
      _fgd1->InitializeEfficiencyCounter();
      //    _fgd2->InitializeEfficiencyCounter();
  }
    Weight_h eventWeight=1;

    BinnedParamsParams params;


    for (Int_t itrue=0;itrue< SystBox->nRelevantTrueObjects; itrue++){      
      AnaTrueParticleB* trueTrack = static_cast<AnaTrueParticleB*>(SystBox->RelevantTrueObjects[itrue]);            
      // retrieve the reconstructed track associated
      AnaTrackB* recoTrack = static_cast<AnaTrackB*>(SystBox->RelevantTrueObjectsReco[itrue]);
      
      // Retrieve the cos (theta) value
      Float_t costheta = trueTrack->Direction[2];
      if (recoTrack) costheta=recoTrack->DirectionStart[2];
      
      bool found = (recoTrack);
      
      // Get the FGD tracking efficiency
      Int_t index;
      if(box.DetectorFV == SubDetId::kFGD1 || box.DetectorFV == SubDetId::kFGD ){
        if( !_fgd1->GetBinValues(fabs(costheta), params, index)) continue;
        if(_computecounters)
          _fgd1->UpdateEfficiencyCounter(index,found);   
        // Add proper offset to the index
        index += systUtils::GetBinnedParamsOffsetForSystematic(*this, *_fgd1);
      }
      else return eventWeight;
      //    if(box.DetectorFV == SubDetId::kFGD2 || box.DetectorFV == SubDetId::kFGD ){
      //  if(!_fgd2->GetBinValues(fabs(costheta), params, index)) continue;
      // if(_computecounters)
      //   _fgd2->UpdateEfficiencyCounter(index,found);
      // Add proper offset to the index
      // index += systUtils::GetBinnedParamsOffsetForSystematic(*this, *_fgd2);
      //}
      eventWeight *= systUtils::ComputeEffLikeWeight(found, toy, GetIndex(), index, params);
    }

    return eventWeight;
}


//********************************************************************
bool FGDTrackEffSystematics::IsRelevantTrueObject(const AnaEventC& event, const AnaTrueObjectC& trueTrack) const{
//********************************************************************

  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];

  //we should not take into account the tracks that are matched to the tpc,
  //this is done in tpc-fgd matching efficiency  
  for (Int_t i=0;i<EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithTPC];i++){
    AnaRecObjectC* track = EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithTPC][i];
    if (CheckTrueRecoAssociation(trueTrack, *track))
      return false;
  }

  // costheta larger than 0.3
  if(fabs(static_cast<const AnaTrueParticleB*>(&trueTrack)->Direction[2]) < 0.3 ) return false;

  return true;
}
