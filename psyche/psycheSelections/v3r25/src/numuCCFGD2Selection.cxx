#include "numuCCFGD2Selection.hxx"
#include "baseSelection.hxx"
#include "CutUtils.hxx"
#include "EventBoxUtils.hxx"
#include "SubDetId.hxx"
#include "SystId.hxx"
#include "VersioningUtils.hxx"
#include "SystematicUtils.hxx"

//********************************************************************
numuCCFGD2Selection::numuCCFGD2Selection(bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxTracker) {
//********************************************************************

  // Initialize the numuCCSelection, which is used
  _numuCCSelection.Initialize();
}

//********************************************************************
void numuCCFGD2Selection::DefineSteps(){
//********************************************************************

    // Cuts must be added in the right order
    // last "true" means the step sequence is broken if cut is not passed (default is "false")
    AddStep(StepBase::kCut,    "event quality",      new EventQualityCut(),           true);
    AddStep(StepBase::kCut,    "> 0 tracks ",        new TotalMultiplicityCut(),      true);  
    AddStep(StepBase::kAction, "find leading tracks",new FindLeadingTracksAction());  
    AddStep(StepBase::kAction, "find vertex",        new FindVertexAction());  
    AddStep(StepBase::kAction, "fill_summary",       new FillSummaryAction_numuCCFGD2());
    AddStep(StepBase::kCut,    "quality+fiducial",   new TrackQualityFiducialCut(),   true);  
    AddStep(StepBase::kAction, "find veto track",    new FindVetoTrackAction());
    AddStep(StepBase::kCut,    "veto",               new ExternalFGD2VetoCut());
    AddStep(StepBase::kAction, "find oofv track",    new FindOOFVTrackAction());
    AddStep(StepBase::kCut,    "External FGD2",      new ExternalFGD2LastLayersCut());
    AddStep(StepBase::kCut,    "muon PID",           new MuonPIDCut());

    SetBranchAlias(0,"trunk");

    // By default the preselection correspond to cuts 0-2
    SetPreSelectionAccumLevel(2);
}

//********************************************************************
void numuCCFGD2Selection::DefineDetectorFV(){
//********************************************************************

    // The detector in which the selection is applied
    SetDetectorFV(SubDetId::kFGD2);

    // If we want to use the numuCCSelection we should tell it to use FGD2
    _numuCCSelection.SetDetectorFV(SubDetId::kFGD2);
}

//********************************************************************
bool numuCCFGD2Selection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
//********************************************************************

    if(allCutsPassed[0]){
        static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD2NuMuCC;
    }
    return (static_cast<AnaEventSummaryB*>(event.Summary)->EventSample != SampleId::kUnassigned);
}

//********************************************************************
bool ExternalFGD2VetoCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//********************************************************************

    // Cast the ToyBox to the appropriate type
    ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

    if (!box.MainTrack) return false;
    if( !cutUtils::ExternalVetoCut(*(box.MainTrack),box.VetoTrack)) return false;

    // Veto also whenever there's a tracker track (or just any track? to be decided) starting in FGD1
    // this guarantees to veto events with a muon candidate in FGD1  
    EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];
    AnaRecObjectC** selTracks = EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithTPC];
    int nTPC=EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithTPC];

    //loop over tpc tracks
    for (Int_t i=0;i<nTPC; ++i){
        AnaTrackB* thistrack = static_cast<AnaTrackB*>(selTracks[i]);

        if (thistrack->Momentum < 0 ) continue;   // protection for bad momentum

        if (anaUtils::InFiducialVolume(SubDetId::kFGD1, thistrack->PositionStart )) return false;
    }

    return true;
}

//********************************************************************
bool ExternalFGD2LastLayersCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//********************************************************************

    (void)event;

    // Cast the ToyBox to the appropriate type
    ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

    /// Reject external background from the last 2 layers of FGD2
    if (!box.MainTrack)  return false;  
    if (!box.OOFVtrack) return true;
    if (box.MainTrack->PositionStart[2]>1770.) //1795 for 1 layer, +0.15 % eff, -0.18 purity
      return false;

    return true; 
}

//********************************************************************
bool FillSummaryAction_numuCCFGD2::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//********************************************************************

    // Cast the ToyBox to the appropriate type
    ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

    if(!box.HMNtrack) return 1;

    static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD2NuMuCC] = box.HMNtrack;
    for(int i = 0; i < 4; ++i){
        static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD2NuMuCC][i] = box.HMNtrack->PositionStart[i];
    }
    if(box.HMNtrack->GetTrueParticle()) static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD2NuMuCC] = box.HMNtrack->GetTrueParticle()->TrueVertex;

    return true;
}

//**************************************************
bool numuCCFGD2Selection::IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const{
//**************************************************

  return _numuCCSelection.IsRelevantRecObjectForSystematic(event,recObj,systId,branch);
}

//**************************************************
bool numuCCFGD2Selection::IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
//**************************************************

  return _numuCCSelection.IsRelevantTrueObjectForSystematic(event,trueObj,systId,branch);
}

//**************************************************
bool numuCCFGD2Selection::IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& box, AnaRecObjectC* recObj,  SystId_h systId, Int_t branch) const{
//**************************************************

  return _numuCCSelection.IsRelevantRecObjectForSystematicInToy(event, box, recObj, systId, branch);
}

//**************************************************
bool numuCCFGD2Selection::IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& box, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
//**************************************************
 
  return _numuCCSelection.IsRelevantTrueObjectForSystematicInToy(event, box, trueObj, systId, branch);
}

//**************************************************
bool numuCCFGD2Selection::IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const{
//**************************************************

  return _numuCCSelection.IsRelevantSystematic(event, box, systId, branch);
}

//**************************************************
Int_t numuCCFGD2Selection::GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  return _numuCCSelection.GetRelevantRecObjectGroupsForSystematic(systId, IDs, branch);
}

//**************************************************
Int_t numuCCFGD2Selection::GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  return _numuCCSelection.GetRelevantTrueObjectGroupsForSystematic(systId, IDs, branch);
}

//**************************************************
void numuCCFGD2Selection::InitializeEvent(AnaEventC& event){
//**************************************************
  
  _numuCCSelection.InitializeEvent(event);
}

//********************************************************************
bool numuCCFGD2Selection::CheckRedoSelection(const AnaEventC& event, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep){
//********************************************************************

  return _numuCCSelection.CheckRedoSelection(event, PreviousToyBox, redoFromStep);
}
