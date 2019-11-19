#include "antiNumuCCFGD2Selection.hxx"
#include "CutUtils.hxx"
#include "SubDetId.hxx"
#include "SystId.hxx"
#include "EventBoxUtils.hxx"
#include "baseSelection.hxx"

//********************************************************************
antiNumuCCFGD2Selection::antiNumuCCFGD2Selection(bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxTracker) {
//********************************************************************

    // Initialize selections being used
    _antiNumuCCSelection.Initialize();
}

//********************************************************************
void antiNumuCCFGD2Selection::DefineSteps(){
//********************************************************************

    // Cuts must be added in the right order
    // last "true" means the step sequence is broken if cut is not passed (default is "false")
    AddStep(StepBase::kCut,    "event quality",               new EventQualityCut(),                   true); // cut 0
    AddStep(StepBase::kCut,    "> 0 tracks ",                 new TotalMultiplicityCut(),              true); // cut 1    
    AddStep(StepBase::kAction, "find leading tracks",         new FindLeadingTracksAction_antinu()         );
    AddStep(StepBase::kAction, "find vertex",                 new FindVertexAction()                       );
    AddStep(StepBase::kAction, "summary",                     new FillSummaryAction_antiNumuCCFGD2()       );
    AddStep(StepBase::kCut,    "quality+fiducial",            new TrackQualityFiducialCut(),           true); // cut 2
    AddStep(StepBase::kCut,    "pos_mult",                    new PositiveMultiplicityCut(),           true); // cut 3
    //AddStep(StepBase::kCut,    "> 0 positive tracks",         new PositiveMultiplicityCut_antiNumuCCFGD2(),    true); // cut 3
    AddStep(StepBase::kAction, "find veto track",             new FindVetoTrackAction()                    );
    AddStep(StepBase::kCut,    "veto",                        new ExternalFGD2VetoCut(),               true); // cut 4
    AddStep(StepBase::kAction, "find oofv track",             new FindOOFVTrackAction()                    );
    AddStep(StepBase::kCut,    "external FGD2",               new ExternalFGD2LastLayersCut(),         true); // cut 5
    AddStep(StepBase::kCut,    "pid",                         new AntiMuonPIDCut()                         ); // cut 6
    //AddStep(StepBase::kCut,    "HMPtrack == HMtrack",         new AntiNuMuFGD2HighestMomentumTrack()            ); // cut 7
    //AddStep(StepBase::kCut,    "HMNtrack is not mu-",         new AntiNuMuFGD2NegativeMuonVeto()                ); // cut 8
    //AddStep(StepBase::kCut,    "Vertex in layer after water", new AntiNuMuFGD2FGDLayer()                        ); // cut 9
    //AddStep(StepBase::kCut,    "Mu+ momentum > 200 MeV",      new AntiNuMuFGD2MuPlusMomentum()                  ); // cut 10

    SetBranchAlias(0,"trunk");

    // By default the preselection correspond to cuts 0-2
    SetPreSelectionAccumLevel(2);
}

//********************************************************************
void antiNumuCCFGD2Selection::DefineDetectorFV(){
//********************************************************************

    // The detector in which the selection is applied
    SetDetectorFV(SubDetId::kFGD2);

    // Set the detector field into the selection beaing used
    _antiNumuCCSelection.SetDetectorFV(SubDetId::kFGD2);

}

//**************************************************
bool antiNumuCCFGD2Selection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
 
  // The event sample corresponding to this selection                                                                                                                             
  if(allCutsPassed[0]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD2AntiNuMuCC;

  return (static_cast<AnaEventSummaryB*>(event.Summary)->EventSample != SampleId::kUnassigned);
}

//**************************************************
bool FillSummaryAction_antiNumuCCFGD2::Apply(AnaEventC& event, ToyBoxB& boxB) const{

  // Cast the ToyBox to the appropriate type
  ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

  if(!box.HMPtrack) return 1;

  static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD2AntiNuMuCC] = box.HMPtrack;

  for(int i = 0; i < 4; i++){
    static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD2AntiNuMuCC][i] = box.HMPtrack->PositionStart[i];
  }

  if(box.HMPtrack->GetTrueParticle()){
    static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD2AntiNuMuCC] = box.HMPtrack->GetTrueParticle()->TrueVertex;
  }
  return 1;
}

// //********************************************************************
// bool ExternalFGD2VetoCut_antiNumuCCFGD2::Apply(AnaEventC& event, ToyBoxB& box) const{
// //********************************************************************

//     (void)event;

//     if (!box.MainTrack) return false;
//     if( !cutUtils::ExternalVetoCut(*(box.MainTrack),box.VetoTrack)) return false;

//     EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];

//     // Veto also whenever there's a tracker track (or just any track? to be decided) starting in FGD1
//     // this guarantees to veto events with a muon candidate in FGD1  
//     AnaTrackB** selTracks = EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithTPC];
//     int nTPC=EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithTPC];

//     //loop over tpc tracks
//     for (Int_t i=0;i<nTPC; ++i){
//         AnaTrackB* thistrack = selTracks[i];

//         if (thistrack->Momentum < 0 ) continue;   // protection for bad momentum

//         if (anaUtils::InFiducialVolume(SubDetId::kFGD1, thistrack->PositionStart )) return false;
//     }

//     return true;
// }


//**************************************************
//bool PositiveMultiplicityCut_antiNumuCCFGD2::Apply(AnaEventC& event, ToyBoxB& box) const{
//**************************************************

//(void)event;

//if (box.HMPtrack)
//  return true;
//else
//  return false;
//}

//**************************************************
//bool AntiNuMuFGD2HighestMomentumTrack::Apply(AnaEventC& event, ToyBoxB& box) const{

//(void)event;

//if(box.HMPtrack != box.HMtrack){return false;}
//return true;
//}

//**************************************************
//bool AntiNuMuFGD2NegativeMuonVeto::Apply(AnaEventC& event, ToyBoxB& box) const{

//(void)event;

//if(!box.HMNtrack){return true;} // No muon candidate, so pass cut
//if(cutUtils::MuonPIDCut(*(box.HMNtrack))){return false;} // Have a muon, so fail cut
//else{return true;}
//}

//**************************************************
//bool AntiNuMuFGD2FGDLayer::Apply(AnaEventC& event, ToyBoxB& box) const{

//float pos[3] = {box.Vertex->Position[0], box.Vertex->Position[1], box.Vertex->Position[2]};

  // 0=vertical layer, 1=horizontal layer, 2=water, 3=gap: glue, air, G10, polypropilene
  //int module = anaUtils::GetFgdModuleType(event, pos, SubDetId::kFGD2);

  //if(module == 0){return true;}
  //return false;
//}

//**************************************************
//bool AntiNuMuFGD2MuPlusMomentum::Apply(AnaEventC& event, ToyBoxB& box) const{

//(void)event;

//double mom = box.HMPtrack->Momentum;
//if(mom < 200){return false;}
//return true;
//}


//**************************************************
bool antiNumuCCFGD2Selection::IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* track, SystId_h systId, Int_t branch) const{
//**************************************************

  return _antiNumuCCSelection.IsRelevantRecObjectForSystematic(event,track,systId,branch);
}

//**************************************************
bool antiNumuCCFGD2Selection::IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueTrack, SystId_h systId, Int_t branch) const{
//**************************************************

  return _antiNumuCCSelection.IsRelevantTrueObjectForSystematic(event,trueTrack,systId,branch);
}

//**************************************************
bool antiNumuCCFGD2Selection::IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const{
//**************************************************
  
  return _antiNumuCCSelection.IsRelevantSystematic(event, box, systId, branch);
  
}

//**************************************************
bool antiNumuCCFGD2Selection::IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& box, AnaRecObjectC* recObj,  SystId_h systId, Int_t branch) const{
//**************************************************

  return _antiNumuCCSelection.IsRelevantRecObjectForSystematicInToy(event, box, recObj, systId, branch);
}

//**************************************************
bool antiNumuCCFGD2Selection::IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& box, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
//**************************************************
 
  return _antiNumuCCSelection.IsRelevantTrueObjectForSystematicInToy(event, box, trueObj, systId, branch);
}


//**************************************************
Int_t antiNumuCCFGD2Selection::GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  return _antiNumuCCSelection.GetRelevantRecObjectGroupsForSystematic(systId, IDs, branch);
}

//**************************************************
Int_t antiNumuCCFGD2Selection::GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  return _antiNumuCCSelection.GetRelevantTrueObjectGroupsForSystematic(systId, IDs, branch);
}


//**************************************************
void antiNumuCCFGD2Selection::InitializeEvent(AnaEventC& event){
//**************************************************

  _antiNumuCCSelection.InitializeEvent(event);
}

//********************************************************************
bool antiNumuCCFGD2Selection::CheckRedoSelection(const AnaEventC& event, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep){
//********************************************************************

  return _antiNumuCCSelection.CheckRedoSelection(event,PreviousToyBox,redoFromStep);
}



