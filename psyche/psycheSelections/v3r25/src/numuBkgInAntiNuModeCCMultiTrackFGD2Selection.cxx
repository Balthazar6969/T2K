#include "numuBkgInAntiNuModeCCMultiTrackFGD2Selection.hxx"
#include "antiNumuCCMultiTrackFGD2Selection.hxx"
#include "baseSelection.hxx"
#include "CutUtils.hxx"
#include "EventBoxUtils.hxx"
#include "Parameters.hxx"
#include "SubDetId.hxx"
#include "SystId.hxx"
#include "VersioningUtils.hxx"
#include "SystematicUtils.hxx"


//*******************************************************************************
numuBkgInAntiNuModeCCMultiTrackFGD2Selection::numuBkgInAntiNuModeCCMultiTrackFGD2Selection(bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxTracker){
//*******************************************************************************

    // initialize selections being used
    _numuCCMultiPiFGD2Selection.Initialize();
    _numuBkgInAntiNuModeCCFGD2Selection.Initialize();
    _numuBkgInAntiNuModeCCMultiTrackSelection.Initialize();
}

//*******************************************************************************
void numuBkgInAntiNuModeCCMultiTrackFGD2Selection::DefineSteps(){
//*******************************************************************************

    // Copy all steps from the numuBkgInAntiNuModeCCFGD2Selection
    CopySteps(_numuBkgInAntiNuModeCCFGD2Selection);

    //Additional actions for the multi-pi selection.
    AddStep(StepBase::kAction, "fill_summary numuBkgMultiTrackFGD2", new FillSummaryAction_numuBkgMultiTrackFGD2());

    //Add a split to the trunk with 2 branches.
    AddSplit(2);
    
    //First branch is for CC1Track enhanced sample
    AddStep(0, StepBase::kCut, "CC1Track", new AntiNuMuCCMultiTrackFGD2_CC1TrackCut());

    //Second branch is for CCNTracks enhanced sample 
    AddStep(1, StepBase::kCut, "CCNTracks", new AntiNuMuCCMultiTrackFGD2_CCNTracksCut());

    // Set the branch aliases to the three branches
    SetBranchAlias(0,"CC1Track",0);
    SetBranchAlias(1,"CCNTracks",1);

    // By default the preselection correspond to cuts 0-2
    SetPreSelectionAccumLevel(2);    
}

//*******************************************************************************
void numuBkgInAntiNuModeCCMultiTrackFGD2Selection::DefineDetectorFV(){
//*******************************************************************************

    // The detector in which the selection is applied
    SetDetectorFV(SubDetId::kFGD2);

    _numuCCMultiPiFGD2Selection.SetDetectorFV(SubDetId::kFGD2);
    _numuBkgInAntiNuModeCCFGD2Selection.SetDetectorFV(SubDetId::kFGD2);
    _numuBkgInAntiNuModeCCMultiTrackSelection.SetDetectorFV(SubDetId::kFGD2);
}

//********************************************************************
bool numuBkgInAntiNuModeCCMultiTrackFGD2Selection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
//********************************************************************

    //CC1Track
    if(allCutsPassed[0]){
        static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD2NuMuBkgInAntiNuModeCC1Track;
    }  
    //CCNTracks
    else if (allCutsPassed[1]){
        static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD2NuMuBkgInAntiNuModeCCNTracks;
    }
    // otherwise kUnassigned is used from the EventSummary constructor
    return (static_cast<AnaEventSummaryB*>(event.Summary)->EventSample != SampleId::kUnassigned);
}


//*********************************************************************
bool FillSummaryAction_numuBkgMultiTrackFGD2::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//*********************************************************************

    // Cast the ToyBox to the appropriate type
    ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

    if(!box.HMNtrack) return 1;

    static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD2NuMuBkgInAntiNuModeCC1Track] = box.HMNtrack;
    static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD2NuMuBkgInAntiNuModeCCNTracks] = box.HMNtrack;

    for(int i = 0; i < 4; ++i){
        static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD2NuMuBkgInAntiNuModeCC1Track][i] = box.HMNtrack->PositionStart[i];
        static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD2NuMuBkgInAntiNuModeCCNTracks][i] = box.HMNtrack->PositionStart[i];
    }
    if(box.HMNtrack->GetTrueParticle()){
        static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD2NuMuBkgInAntiNuModeCC1Track] = box.HMNtrack->GetTrueParticle()->TrueVertex;
        static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD2NuMuBkgInAntiNuModeCCNTracks] = box.HMNtrack->GetTrueParticle()->TrueVertex;
    }
    return 1;

}

//**************************************************
bool numuBkgInAntiNuModeCCMultiTrackFGD2Selection::IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* track, SystId_h systId, Int_t branch) const{
//**************************************************

  return _numuCCMultiPiFGD2Selection.IsRelevantRecObjectForSystematic(event,track,systId,branch);
}

//**************************************************
bool numuBkgInAntiNuModeCCMultiTrackFGD2Selection::IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueTrack, SystId_h systId, Int_t branch) const{
//**************************************************

  return _numuCCMultiPiFGD2Selection.IsRelevantTrueObjectForSystematic(event,trueTrack,systId,branch);
}

//**************************************************
bool numuBkgInAntiNuModeCCMultiTrackFGD2Selection::IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const{
//**************************************************

  return _numuBkgInAntiNuModeCCMultiTrackSelection.IsRelevantSystematic(event,box,systId,branch);
}

//**************************************************
Int_t numuBkgInAntiNuModeCCMultiTrackFGD2Selection::GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  return _numuBkgInAntiNuModeCCMultiTrackSelection.GetRelevantRecObjectGroupsForSystematic(systId, IDs, branch);
}

//**************************************************
Int_t numuBkgInAntiNuModeCCMultiTrackFGD2Selection::GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  return _numuBkgInAntiNuModeCCMultiTrackSelection.GetRelevantTrueObjectGroupsForSystematic(systId, IDs, branch);
}
//**************************************************
void numuBkgInAntiNuModeCCMultiTrackFGD2Selection::InitializeEvent(AnaEventC& event){
//**************************************************

  _numuCCMultiPiFGD2Selection.InitializeEvent(event);							    
}

//********************************************************************
bool numuBkgInAntiNuModeCCMultiTrackFGD2Selection::CheckRedoSelection(const AnaEventC& event, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep){
//********************************************************************
  
  return _numuCCMultiPiFGD2Selection.CheckRedoSelection(event, PreviousToyBox, redoFromStep);							    
}
