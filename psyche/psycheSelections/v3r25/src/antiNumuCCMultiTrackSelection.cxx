#include "antiNumuCCMultiTrackSelection.hxx"
#include "antiNumuCCSelection.hxx"
#include "baseSelection.hxx"
#include "CutUtils.hxx"
#include "EventBoxUtils.hxx"
#include "SubDetId.hxx"
#include "SystId.hxx"
#include "VersioningUtils.hxx"
#include "SystematicUtils.hxx"


//********************************************************************
antiNumuCCMultiTrackSelection::antiNumuCCMultiTrackSelection(bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxTracker) {
//********************************************************************
  _antiNumuCCSelection.Initialize();
}

//********************************************************************
void antiNumuCCMultiTrackSelection::DefineSteps(){
//********************************************************************

    // Copy all steps from the antiNumuCCSelection
    CopySteps(_antiNumuCCSelection);

    //Additional actions for the multi-pi selection.
    AddStep(StepBase::kAction, "fill summary antinu_CCMultiTrack",       new FillSummaryAction_antinu_CCMultiTrack());  

    //Add a split to the trunk with 2 branches.
    AddSplit(2);

    //First branch is for CCQE enhanced sample (CC-0-pi)
    AddStep(0, StepBase::kCut, "CCQE", new CCQEcut_antinu());

    //Second branch is for CCnQE enhanced sample (CC-N-pi)
    AddStep(1, StepBase::kCut, "CCnQE", new CCnQEcut_antinu());

    // Set the branch aliases to the three branches
    SetBranchAlias(0,"CCQE",0);
    SetBranchAlias(1,"CCnQE",1);

    // By default the preselection correspond to cuts 0-2
    SetPreSelectionAccumLevel(2);
}

//********************************************************************
void antiNumuCCMultiTrackSelection::DefineDetectorFV(){
//********************************************************************

    // The detector in which the selection is applied
    SetDetectorFV(SubDetId::kFGD1);
    _antiNumuCCSelection.SetDetectorFV(SubDetId::kFGD1);
}

//********************************************************************
bool antiNumuCCMultiTrackSelection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
//********************************************************************

    //CCQE enhanced (CC-0-pi)
    if(allCutsPassed[0]){
        static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD1AntiNuMuCC1Track;
    }  
    //CCnQE enhanced (CC-N-pi)
    else if (allCutsPassed[1]){
        static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD1AntiNuMuCCNTracks;
    }

    // otherwise kUnassigned is used from the EventSummary constructor
    return (static_cast<AnaEventSummaryB*>(event.Summary)->EventSample != SampleId::kUnassigned);
}

//*********************************************************************
bool FillSummaryAction_antinu_CCMultiTrack::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//*********************************************************************

    // Cast the ToyBox to the appropriate type
    ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 


    if(!box.HMPtrack) return 1;

    static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD1AntiNuMuCC1Track] = box.HMPtrack;
    static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD1AntiNuMuCCNTracks] = box.HMPtrack;

    for(int i = 0; i < 4; ++i){
        static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD1AntiNuMuCC1Track][i] = box.HMPtrack->PositionStart[i];
        static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD1AntiNuMuCCNTracks][i] = box.HMPtrack->PositionStart[i];
    }
    if(box.HMPtrack->GetTrueParticle()){
        static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD1AntiNuMuCC1Track] = box.HMPtrack->GetTrueParticle()->TrueVertex;
        static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD1AntiNuMuCCNTracks] = box.HMPtrack->GetTrueParticle()->TrueVertex;
    }
    return 1;
}

//**************************************************
bool CCQEcut_antinu::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

    // Cast the ToyBox to the appropriate type
    ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

    if (!box.MainTrack)return false;
    EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];
    return (EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithTPCAndFGD1] == 1);
}

//**************************************************
bool CCnQEcut_antinu::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

    // Cast the ToyBox to the appropriate type
    ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

    if (!box.MainTrack)return false;
    EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];
    return (EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithTPCAndFGD1] > 1);
}

//**************************************************
bool antiNumuCCMultiTrackSelection::IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* track, SystId_h systId, Int_t branch) const{
//**************************************************

  return _antiNumuCCSelection.IsRelevantRecObjectForSystematic(event,track,systId,branch);
}

//**************************************************
bool antiNumuCCMultiTrackSelection::IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueTrack, SystId_h systId, Int_t branch) const{
//**************************************************

  return _antiNumuCCSelection.IsRelevantTrueObjectForSystematic(event,trueTrack,systId,branch);
}

//**************************************************
bool antiNumuCCMultiTrackSelection::IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const{
//**************************************************

  (void)event;
  (void)branch;
  (void)box;
  
  // Start with antinumu CC
  if (_antiNumuCCSelection.IsRelevantSystematic(event, box, systId, branch))
    return true;
  
  // No additional systematics for the moment
  return false;
}

//**************************************************
Int_t antiNumuCCMultiTrackSelection::GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  return _antiNumuCCSelection.GetRelevantRecObjectGroupsForSystematic(systId, IDs, branch);
}

//**************************************************
Int_t antiNumuCCMultiTrackSelection::GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  return _antiNumuCCSelection.GetRelevantTrueObjectGroupsForSystematic(systId, IDs, branch);
}



//**************************************************
void antiNumuCCMultiTrackSelection::InitializeEvent(AnaEventC& event){
//**************************************************

  _antiNumuCCSelection.InitializeEvent(event);   
}

//********************************************************************
bool antiNumuCCMultiTrackSelection::CheckRedoSelection(const AnaEventC& event, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep){
//********************************************************************
  
  // Must redo selection if antiNumuCCSelection decides so
  if( _antiNumuCCSelection.CheckRedoSelection(event,PreviousToyBox,redoFromStep)) return true;

  // Otherwise selection should not be redone since the number of tracks with TPC and FGD will not be changed by systematics
  return false;
}
