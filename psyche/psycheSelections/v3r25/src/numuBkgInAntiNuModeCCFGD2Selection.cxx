#include "baseSelection.hxx"
#include "numuBkgInAntiNuModeCCFGD2Selection.hxx"
#include "CutUtils.hxx"
#include "SubDetId.hxx"
#include "EventBoxId.hxx"

//********************************************************************
numuBkgInAntiNuModeCCFGD2Selection::numuBkgInAntiNuModeCCFGD2Selection(bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxTracker) {
//********************************************************************

    // initialize selections being used
    _numuCCFGD2Selection.Initialize();
    _numuBkgInAntiNuModeCCSelection.Initialize();
}

//********************************************************************
void numuBkgInAntiNuModeCCFGD2Selection::DefineSteps(){
//********************************************************************

    // Cuts must be added in the right order
    // last "true" means the step sequence is broken if cut is not passed (default is "false")
    AddStep(StepBase::kCut,    "event quality",      new EventQualityCut(),           true);
    AddStep(StepBase::kCut,    "> 0 tracks ",        new TotalMultiplicityCut(),      true);  
    AddStep(StepBase::kAction, "find leading tracks",new FindLeadingTracksAction());  
    AddStep(StepBase::kAction, "find vertex",        new FindVertexAction());  
    AddStep(StepBase::kAction, "fill_summary",       new FillSummaryAction_numuBkgFGD2());
    AddStep(StepBase::kCut,    "quality+fiducial",   new TrackQualityFiducialCut(),   true);  
    AddStep(StepBase::kCut,    "neg_mult",           new NegativeMultiplicityCut());  
    AddStep(StepBase::kAction, "find veto track",    new FindVetoTrackAction());
    AddStep(StepBase::kCut,    "veto",               new ExternalFGD2VetoCut());
    AddStep(StepBase::kAction, "find oofv track",    new FindOOFVTrackAction());
    AddStep(StepBase::kCut,    "External FGD2",      new ExternalFGD2LastLayersCut());
    AddStep(StepBase::kCut,    "Muon PID",           new NumuBkgMuonPIDCut());
    
    SetBranchAlias(0,"trunk");

    // By default the preselection correspond to cuts 0-2
    SetPreSelectionAccumLevel(2);
}

//********************************************************************
void numuBkgInAntiNuModeCCFGD2Selection::DefineDetectorFV(){
//********************************************************************

    // The detector in which the selection is applied
    SetDetectorFV(SubDetId::kFGD2);

    _numuCCFGD2Selection.SetDetectorFV(SubDetId::kFGD2);
    _numuBkgInAntiNuModeCCSelection.SetDetectorFV(SubDetId::kFGD2);
}

//********************************************************************
bool numuBkgInAntiNuModeCCFGD2Selection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
//********************************************************************

    if(allCutsPassed[0]){
        static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD2NuMuBkgInAntiNuModeCC;
    }
    return (static_cast<AnaEventSummaryB*>(event.Summary)->EventSample != SampleId::kUnassigned);
}

//********************************************************************
bool FillSummaryAction_numuBkgFGD2::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//********************************************************************

    // Cast the ToyBox to the appropriate type
    ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

    if(!box.HMNtrack) return 1;

    static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD2NuMuBkgInAntiNuModeCC] = box.HMNtrack;
    for(int i = 0; i < 4; ++i){
        static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD2NuMuBkgInAntiNuModeCC][i] = box.HMNtrack->PositionStart[i];
    }
    if(box.HMNtrack->GetTrueParticle()) static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD2NuMuBkgInAntiNuModeCC] = box.HMNtrack->GetTrueParticle()->TrueVertex;

    return true;
}

//**************************************************
bool numuBkgInAntiNuModeCCFGD2Selection::IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* track, SystId_h systId, Int_t branch) const{
//**************************************************

  return _numuBkgInAntiNuModeCCSelection.IsRelevantRecObjectForSystematic(event,track,systId,branch);

}

//**************************************************
bool numuBkgInAntiNuModeCCFGD2Selection::IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueTrack, SystId_h systId, Int_t branch) const{
//**************************************************

  return _numuBkgInAntiNuModeCCSelection.IsRelevantTrueObjectForSystematic(event,trueTrack,systId,branch);

}

//**************************************************
bool numuBkgInAntiNuModeCCFGD2Selection::IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const{
//**************************************************

  // Same as for numuBkgInAntiNuModeCCSelection
  return _numuBkgInAntiNuModeCCSelection.IsRelevantSystematic(event, box, systId, branch);
}

//**************************************************
bool numuBkgInAntiNuModeCCFGD2Selection::IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& box, AnaRecObjectC* recObj,  SystId_h systId, Int_t branch) const{
//**************************************************

  return _numuBkgInAntiNuModeCCSelection.IsRelevantRecObjectForSystematicInToy(event, box, recObj, systId, branch);
}

//**************************************************
bool numuBkgInAntiNuModeCCFGD2Selection::IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& box, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
//**************************************************
 
  return _numuBkgInAntiNuModeCCSelection.IsRelevantTrueObjectForSystematicInToy(event, box, trueObj, systId, branch);
}

//**************************************************
Int_t numuBkgInAntiNuModeCCFGD2Selection::GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  return _numuBkgInAntiNuModeCCSelection.GetRelevantRecObjectGroupsForSystematic(systId, IDs, branch);
}

//**************************************************
Int_t numuBkgInAntiNuModeCCFGD2Selection::GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  return _numuBkgInAntiNuModeCCSelection.GetRelevantTrueObjectGroupsForSystematic(systId, IDs, branch);
}


//**************************************************
void numuBkgInAntiNuModeCCFGD2Selection::InitializeEvent(AnaEventC& event){
//**************************************************
  
  _numuBkgInAntiNuModeCCSelection.InitializeEvent(event);
}

//********************************************************************
bool numuBkgInAntiNuModeCCFGD2Selection::CheckRedoSelection(const AnaEventC& event, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep){
//********************************************************************

  return _numuBkgInAntiNuModeCCSelection.CheckRedoSelection(event, PreviousToyBox, redoFromStep);
}
