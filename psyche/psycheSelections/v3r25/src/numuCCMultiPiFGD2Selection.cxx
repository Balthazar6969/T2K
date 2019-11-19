#include "numuCCMultiPiFGD2Selection.hxx"
#include "numuCCFGD2Selection.hxx"
#include "baseSelection.hxx"
#include "CutUtils.hxx"
#include "EventBoxUtils.hxx"
#include "Parameters.hxx"
#include "SubDetId.hxx"
#include "SystId.hxx"
#include "VersioningUtils.hxx"
#include "SystematicUtils.hxx"
#include "SystematicTuning.hxx"


//*******************************************************************************
numuCCMultiPiFGD2Selection::numuCCMultiPiFGD2Selection(bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxTracker){
//*******************************************************************************

  // Initialize systematic tuning parameters
  systTuning::Initialize();
  _numuCCMultiPiSelection.Initialize();
  
}

//*******************************************************************************
void numuCCMultiPiFGD2Selection::DefineSteps(){
//*******************************************************************************

    // Copy all steps from the numuCCFGD2Selection
    CopySteps(_numuCCFGD2Selection);

    //Additional actions for the multi-pi selection.
    AddStep(StepBase::kAction, "fill_summary_multiPiFGD2", new FillSummaryAction_numuCCMultiPiFGD2());
    AddStep(StepBase::kAction, "find_pions",               new FindPionsAction());
    
    //Add a split to the trunk with 3 branches.
    AddSplit(3);

    //First branch is for CC-0pi
    AddStep(0, StepBase::kCut, "CC-0pi",        new NoPionCut());
    AddStep(0, StepBase::kCut, "ECal Pi0 veto", new EcalPi0VetoCut());
    
    //Second branch is for CC-1pi
    AddStep(1, StepBase::kCut, "CC-1pi",        new OnePionCut(true));
    AddStep(1, StepBase::kCut, "ECal Pi0 veto", new EcalPi0VetoCut());
    
    //Third branch is for CC-Other
    AddStep(2, StepBase::kCut, "CC-Other", new OthersCut());

    // Set the branch aliases to the three branches
    SetBranchAlias(0, "CC-0pi",   0);
    SetBranchAlias(1, "CC-1pi",   1);
    SetBranchAlias(2, "CC-Other", 2);

    // By default the preselection correspond to cuts 0-2
    SetPreSelectionAccumLevel(2);
}


//*******************************************************************************
void numuCCMultiPiFGD2Selection::DefineDetectorFV(){
//*******************************************************************************

    // The detector in which the selection is applied
    SetDetectorFV(SubDetId::kFGD2);

    _numuCCMultiPiSelection.SetDetectorFV(SubDetId::kFGD2);
    _numuCCFGD2Selection.SetDetectorFV(SubDetId::kFGD2);

}

//********************************************************************
bool numuCCMultiPiFGD2Selection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
//********************************************************************

    //CC0pi
    if(allCutsPassed[0]){
        static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD2NuMuCC0Pi;
    }  
    //CC1pi
    else if (allCutsPassed[1]){
        static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD2NuMuCC1Pi;
    }
    //CCOther
    else if (allCutsPassed[2]){
        static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD2NuMuCCOther;
    }

    // otherwise kUnassigned is used from the EventSummary constructor
    return (static_cast<AnaEventSummaryB*>(event.Summary)->EventSample != SampleId::kUnassigned);
}


//*********************************************************************
bool FillSummaryAction_numuCCMultiPiFGD2::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//*********************************************************************

    // Cast the ToyBox to the appropriate type
    ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

    if(!box.HMNtrack) return 1;

    static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD2NuMuCC0Pi] = box.HMNtrack;
    static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD2NuMuCC1Pi] = box.HMNtrack;
    static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD2NuMuCCOther] = box.HMNtrack;

    for(int i = 0; i < 4; ++i){
        static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD2NuMuCC0Pi][i] = box.HMNtrack->PositionStart[i];
        static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD2NuMuCC1Pi][i] = box.HMNtrack->PositionStart[i];
        static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD2NuMuCCOther][i] = box.HMNtrack->PositionStart[i];
    }
    if(box.HMNtrack->GetTrueParticle()){
        static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD2NuMuCC0Pi] = box.HMNtrack->GetTrueParticle()->TrueVertex;
        static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD2NuMuCC1Pi] = box.HMNtrack->GetTrueParticle()->TrueVertex;
        static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD2NuMuCCOther] = box.HMNtrack->GetTrueParticle()->TrueVertex;
    }
    return 1;
}

//**************************************************
bool numuCCMultiPiFGD2Selection::IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* track, SystId_h systId, Int_t branch) const{
//**************************************************

  return _numuCCFGD2Selection.IsRelevantRecObjectForSystematic(event,track,systId,branch);
}

//**************************************************
bool numuCCMultiPiFGD2Selection::IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueTrack, SystId_h systId, Int_t branch) const{
//**************************************************

  return _numuCCFGD2Selection.IsRelevantTrueObjectForSystematic(event,trueTrack,systId,branch);
}

//**************************************************
bool numuCCMultiPiFGD2Selection::IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const{
//**************************************************

  return _numuCCMultiPiSelection.IsRelevantSystematic(event,box,systId,branch);
}

//**************************************************
Int_t numuCCMultiPiFGD2Selection::GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  return _numuCCMultiPiSelection.GetRelevantRecObjectGroupsForSystematic(systId, IDs, branch);
}

//**************************************************
Int_t numuCCMultiPiFGD2Selection::GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  return _numuCCMultiPiSelection.GetRelevantTrueObjectGroupsForSystematic(systId, IDs, branch);
}

//**************************************************
bool numuCCMultiPiFGD2Selection::IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& box, AnaRecObjectC* track, SystId_h systId, Int_t branch) const{
//**************************************************
  return _numuCCMultiPiSelection.IsRelevantRecObjectForSystematicInToy(event, box, track, systId, branch);
}

//**************************************************
bool numuCCMultiPiFGD2Selection::IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& box, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
//**************************************************

  return _numuCCMultiPiSelection.IsRelevantTrueObjectForSystematicInToy(event, box, trueObj, systId, branch);
}


//**************************************************
void numuCCMultiPiFGD2Selection::InitializeEvent(AnaEventC& eventBB){
//**************************************************
  
  return _numuCCMultiPiSelection.InitializeEvent(eventBB);
}

//********************************************************************
bool numuCCMultiPiFGD2Selection::CheckRedoSelection(const AnaEventC& event, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep){
//********************************************************************
  
  return _numuCCMultiPiSelection.CheckRedoSelection(event, PreviousToyBox, redoFromStep);							    
}
