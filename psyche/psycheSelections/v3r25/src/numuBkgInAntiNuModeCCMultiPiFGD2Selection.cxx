#include "numuBkgInAntiNuModeCCMultiPiFGD2Selection.hxx"
#include "antiNumuCCMultiPiFGD2Selection.hxx"
#include "baseSelection.hxx"
#include "CutUtils.hxx"
#include "EventBoxUtils.hxx"
#include "Parameters.hxx"
#include "SubDetId.hxx"
#include "SystId.hxx"
#include "VersioningUtils.hxx"
#include "SystematicUtils.hxx"


//*******************************************************************************
numuBkgInAntiNuModeCCMultiPiFGD2Selection::numuBkgInAntiNuModeCCMultiPiFGD2Selection(bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxTracker){
//*******************************************************************************

    // initialize selections being used
    _numuCCMultiPiFGD2Selection.Initialize();
    _numuBkgInAntiNuModeCCFGD2Selection.Initialize();
    _numuBkgInAntiNuModeCCMultiPiSelection.Initialize();
}

//*******************************************************************************
void numuBkgInAntiNuModeCCMultiPiFGD2Selection::DefineSteps(){
//*******************************************************************************

    // Copy all steps from the numuBkgInAntiNuModeCCFGD2Selection
    CopySteps(_numuBkgInAntiNuModeCCFGD2Selection);

    //Additional actions for the multi-pi selection.
    AddStep(StepBase::kAction, "fill_summary numuBkgMultiPiFGD2", new FillSummaryAction_numuBkgMultiPiFGD2());
    AddStep(StepBase::kAction, "find_pions",                      new FindPionsAction());

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
    SetBranchAlias(0,"CC-0pi",0);
    SetBranchAlias(1,"CC-1pi",1);
    SetBranchAlias(2,"CC-Other",2);
    
    // By default the preselection correspond to cuts 0-2
    SetPreSelectionAccumLevel(2);    
}

//*******************************************************************************
void numuBkgInAntiNuModeCCMultiPiFGD2Selection::DefineDetectorFV(){
//*******************************************************************************

    // The detector in which the selection is applied
    SetDetectorFV(SubDetId::kFGD2);

    _numuCCMultiPiFGD2Selection.SetDetectorFV(SubDetId::kFGD2);
    _numuBkgInAntiNuModeCCFGD2Selection.SetDetectorFV(SubDetId::kFGD2);
    _numuBkgInAntiNuModeCCMultiPiSelection.SetDetectorFV(SubDetId::kFGD2);
}

//********************************************************************
bool numuBkgInAntiNuModeCCMultiPiFGD2Selection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
//********************************************************************

    //CC0pi
    if(allCutsPassed[0]){
        static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD2NuMuBkgInAntiNuModeCC0Pi;
    }  
    //CC1pi
    else if (allCutsPassed[1]){
        static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD2NuMuBkgInAntiNuModeCC1Pi;
    }
    //CCOther
    else if (allCutsPassed[2]){
        static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD2NuMuBkgInAntiNuModeCCOther;
    }

    // otherwise kUnassigned is used from the EventSummary constructor
    return (static_cast<AnaEventSummaryB*>(event.Summary)->EventSample != SampleId::kUnassigned);
}


//*********************************************************************
bool FillSummaryAction_numuBkgMultiPiFGD2::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//*********************************************************************

    // Cast the ToyBox to the appropriate type
    ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

    if(!box.HMNtrack) return 1;

    static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD2NuMuBkgInAntiNuModeCC0Pi] = box.HMNtrack;
    static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD2NuMuBkgInAntiNuModeCC1Pi] = box.HMNtrack;
    static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD2NuMuBkgInAntiNuModeCCOther] = box.HMNtrack;

    for(int i = 0; i < 4; ++i){
        static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD2NuMuBkgInAntiNuModeCC0Pi][i] = box.HMNtrack->PositionStart[i];
        static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD2NuMuBkgInAntiNuModeCC1Pi][i] = box.HMNtrack->PositionStart[i];
        static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD2NuMuBkgInAntiNuModeCCOther][i] = box.HMNtrack->PositionStart[i];
    }
    if(box.HMNtrack->GetTrueParticle()){
        static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD2NuMuBkgInAntiNuModeCC0Pi] = box.HMNtrack->GetTrueParticle()->TrueVertex;
        static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD2NuMuBkgInAntiNuModeCC1Pi] = box.HMNtrack->GetTrueParticle()->TrueVertex;
        static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD2NuMuBkgInAntiNuModeCCOther] = box.HMNtrack->GetTrueParticle()->TrueVertex;
    }
    return 1;
}

//**************************************************
bool numuBkgInAntiNuModeCCMultiPiFGD2Selection::IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* track, SystId_h systId, Int_t branch) const{
//**************************************************

  return _numuCCMultiPiFGD2Selection.IsRelevantRecObjectForSystematic(event,track,systId,branch);
}

//**************************************************
bool numuBkgInAntiNuModeCCMultiPiFGD2Selection::IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueTrack, SystId_h systId, Int_t branch) const{
//**************************************************

  return _numuCCMultiPiFGD2Selection.IsRelevantTrueObjectForSystematic(event,trueTrack,systId,branch);
}

//**************************************************
bool numuBkgInAntiNuModeCCMultiPiFGD2Selection::IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const{
//**************************************************

  return _numuBkgInAntiNuModeCCMultiPiSelection.IsRelevantSystematic(event,box,systId,branch);
}

//**************************************************
bool numuBkgInAntiNuModeCCMultiPiFGD2Selection::IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& box, AnaRecObjectC* track, SystId_h systId, Int_t branch) const{
//**************************************************
  return _numuBkgInAntiNuModeCCMultiPiSelection.IsRelevantRecObjectForSystematicInToy(event, box, track, systId, branch);
}

//**************************************************
bool numuBkgInAntiNuModeCCMultiPiFGD2Selection::IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& box, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
//**************************************************

  return _numuBkgInAntiNuModeCCMultiPiSelection.IsRelevantTrueObjectForSystematicInToy(event, box, trueObj, systId, branch);
}


//**************************************************
Int_t numuBkgInAntiNuModeCCMultiPiFGD2Selection::GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  return _numuCCMultiPiFGD2Selection.GetRelevantRecObjectGroupsForSystematic(systId, IDs, branch);
}

//**************************************************
Int_t numuBkgInAntiNuModeCCMultiPiFGD2Selection::GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  return _numuCCMultiPiFGD2Selection.GetRelevantTrueObjectGroupsForSystematic(systId, IDs, branch);
}


//**************************************************
void numuBkgInAntiNuModeCCMultiPiFGD2Selection::InitializeEvent(AnaEventC& event){
//**************************************************

  _numuCCMultiPiFGD2Selection.InitializeEvent(event);							    
}

//********************************************************************
bool numuBkgInAntiNuModeCCMultiPiFGD2Selection::CheckRedoSelection(const AnaEventC& event, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep){
//********************************************************************
  
  return _numuCCMultiPiFGD2Selection.CheckRedoSelection(event, PreviousToyBox, redoFromStep);							    
}
