#include "antiNumuCCMultiTrackFGD2Selection.hxx"
#include "antiNumuCCMultiTrackSelection.hxx"
#include "antiNumuCCFGD2Selection.hxx"
#include "numuCCMultiPiFGD2Selection.hxx"
#include "baseSelection.hxx"
#include "CutUtils.hxx"
#include "Parameters.hxx"
#include "SubDetId.hxx"
#include "SystId.hxx"
#include "EventBoxId.hxx"

//********************************************************************
antiNumuCCMultiTrackFGD2Selection::antiNumuCCMultiTrackFGD2Selection(bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxTracker) {
//********************************************************************

    // Initialize selections being used
    _antiNumuCCFGD2Selection.Initialize();
    _antiNumuCCMultiTrackSelection.Initialize();
}

//********************************************************************
void antiNumuCCMultiTrackFGD2Selection::DefineSteps(){
//********************************************************************

    // Copy all steps from the antiNumuCCFGD2Selection
    CopySteps(_antiNumuCCFGD2Selection);

    //Additional actions for the multi-pi selection.
    AddStep(StepBase::kAction,  "fill summary AntiNuMuCCMultiTrackFGD2",   new AntiNuMuCCMultiTrackFGD2_FillSummaryAction());  

    //Add a split to the trunk with 2 branches.
    AddSplit(2);

    //First branch is for CC 1track
    AddStep(0, StepBase::kCut, "CC1Track", new AntiNuMuCCMultiTrackFGD2_CC1TrackCut());

    //Second branch is for CC nTracks
    AddStep(1, StepBase::kCut, "CCNTracks", new AntiNuMuCCMultiTrackFGD2_CCNTracksCut());

    // Set the branch aliases to the three branches
    SetBranchAlias(0,"CC1Track",0);
    SetBranchAlias(1,"CCNTracks",1);

    // By default the preselection correspond to cuts 0-2
    SetPreSelectionAccumLevel(2);
}

//********************************************************************
void antiNumuCCMultiTrackFGD2Selection::DefineDetectorFV(){
//********************************************************************

    // The detector in which the selection is applied
    SetDetectorFV(SubDetId::kFGD2);

    _antiNumuCCFGD2Selection.SetDetectorFV(SubDetId::kFGD2);
    _antiNumuCCMultiTrackSelection.SetDetectorFV(SubDetId::kFGD2);
}

//********************************************************************
bool antiNumuCCMultiTrackFGD2Selection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
//********************************************************************

    //CC1Track
    if(allCutsPassed[0]){
        static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD2AntiNuMuCC1Track;
    }  
    //CCNTracks
    else if (allCutsPassed[1]){
        static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD2AntiNuMuCCNTracks;
    }

    // otherwise kUnassigned is used from the EventSummary constructor
    return (static_cast<AnaEventSummaryB*>(event.Summary)->EventSample != SampleId::kUnassigned);
}

//*********************************************************************
bool AntiNuMuCCMultiTrackFGD2_FillSummaryAction::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//*********************************************************************

    // Cast the ToyBox to the appropriate type
    ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

    if(!box.HMPtrack) return 1;

    static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD2AntiNuMuCC1Track] = box.HMPtrack;
    static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD2AntiNuMuCCNTracks] = box.HMPtrack;

    for(int i = 0; i < 4; ++i){
        static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD2AntiNuMuCC1Track][i] = box.HMPtrack->PositionStart[i];
        static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD2AntiNuMuCCNTracks][i] = box.HMPtrack->PositionStart[i];
    }
    if(box.HMPtrack->GetTrueParticle()){
        static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD2AntiNuMuCC1Track] = box.HMPtrack->GetTrueParticle()->TrueVertex;
        static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD2AntiNuMuCCNTracks] = box.HMPtrack->GetTrueParticle()->TrueVertex;
    }
    return 1;
}

//**************************************************
bool AntiNuMuCCMultiTrackFGD2_CC1TrackCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

    // Cast the ToyBox to the appropriate type
    ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

    if (!box.MainTrack)return false;
    EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];
    return (EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithTPCAndFGD2] == 1);
}

//**************************************************
bool AntiNuMuCCMultiTrackFGD2_CCNTracksCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

    // Cast the ToyBox to the appropriate type
    ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

    if (!box.MainTrack)return false;
    EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];
    return (EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithTPCAndFGD2] > 1);
}


//**************************************************
bool antiNumuCCMultiTrackFGD2Selection::IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* track, SystId_h systId, Int_t branch) const{
//**************************************************

  return _antiNumuCCFGD2Selection.IsRelevantRecObjectForSystematic(event,track,systId,branch);
}

//**************************************************
bool antiNumuCCMultiTrackFGD2Selection::IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueTrack, SystId_h systId, Int_t branch) const{
//**************************************************

    return _antiNumuCCFGD2Selection.IsRelevantTrueObjectForSystematic(event,trueTrack,systId,branch);
}

//**************************************************
bool antiNumuCCMultiTrackFGD2Selection::IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const{
//**************************************************
  
  return _antiNumuCCMultiTrackSelection.IsRelevantSystematic(event, box, systId, branch);
}

//**************************************************
Int_t antiNumuCCMultiTrackFGD2Selection::GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  return _antiNumuCCMultiTrackSelection.GetRelevantRecObjectGroupsForSystematic(systId, IDs, branch);
}

//**************************************************
Int_t antiNumuCCMultiTrackFGD2Selection::GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  return _antiNumuCCMultiTrackSelection.GetRelevantTrueObjectGroupsForSystematic(systId, IDs, branch);
}

//**************************************************
void antiNumuCCMultiTrackFGD2Selection::InitializeEvent(AnaEventC& event){
//**************************************************
  
  return _antiNumuCCFGD2Selection.InitializeEvent(event);
}

//********************************************************************
bool antiNumuCCMultiTrackFGD2Selection::CheckRedoSelection(const AnaEventC& event, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep){
//********************************************************************

  return _antiNumuCCFGD2Selection.CheckRedoSelection(event, PreviousToyBox, redoFromStep);
}
