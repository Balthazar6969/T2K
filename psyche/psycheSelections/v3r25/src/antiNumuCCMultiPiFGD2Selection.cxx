#include "antiNumuCCMultiPiFGD2Selection.hxx"
#include "antiNumuCCMultiPiSelection.hxx"
#include "numuCCFGD2Selection.hxx"
#include "baseSelection.hxx"
#include "CutUtils.hxx"
#include "EventBoxUtils.hxx"
#include "Parameters.hxx"
#include "SubDetId.hxx"
#include "SystId.hxx"
#include "VersioningUtils.hxx"
#include "SystematicUtils.hxx"


//*******************************************************************************
antiNumuCCMultiPiFGD2Selection::antiNumuCCMultiPiFGD2Selection(bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxTracker){
  //*******************************************************************************
  // Initialize selections being used
  _antiNumuCCFGD2Selection.Initialize();
  _antiNumuCCMultiPiSelection.Initialize();
}

//*******************************************************************************
void antiNumuCCMultiPiFGD2Selection::DefineSteps(){
  //*******************************************************************************

  // Copy all steps from the numuCCFGD2Selection
  CopySteps(_antiNumuCCFGD2Selection);

  //Additional actions for the multi-pi selection.
  AddStep(StepBase::kAction, "fill_summary_multiPiFGD2", new FillSummaryAction_antiNumuCCMultiPiFGD2());
  AddStep(StepBase::kAction, "find_pions",               new FindPionsAction_antinuCCMultiPi());

  //Add a split to the trunk with 3 branches.
  AddSplit(3);

  //First branch is for CC-0pi
  AddStep(0, StepBase::kCut, "CC-0pi",        new NoPionCut());
  AddStep(0, StepBase::kCut, "ECal Pi0 veto", new EcalPi0VetoCut());

  //Second branch is for CC-1pi
  AddStep(1, StepBase::kCut, "CC-1pi",        new OnePionCut(false));
  AddStep(1, StepBase::kCut, "ECal Pi0 veto", new EcalPi0VetoCut());

  //Third branch is for CC-Other
  AddStep(2, StepBase::kCut, "CC-Other",      new OthersCut());

  // Set the branch aliases to the three branches
  SetBranchAlias(0,"CC-0pi",0);
  SetBranchAlias(1,"CC-1pi",1);
  SetBranchAlias(2,"CC-Other",2);

  // By default the preselection correspond to cuts 0-2
  SetPreSelectionAccumLevel(2);
}


//*******************************************************************************
void antiNumuCCMultiPiFGD2Selection::DefineDetectorFV(){
  //*******************************************************************************

  // The detector in which the selection is applied
  SetDetectorFV(SubDetId::kFGD2);

  _antiNumuCCMultiPiSelection.SetDetectorFV(SubDetId::kFGD2);
  _antiNumuCCFGD2Selection.SetDetectorFV(SubDetId::kFGD2);

}

//********************************************************************
bool antiNumuCCMultiPiFGD2Selection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
  //********************************************************************

  //CC0pi
  if(allCutsPassed[0]){
    static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD2AntiNuMuCC0Pi;
  }  
  //CC1pi
  else if (allCutsPassed[1]){
    static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD2AntiNuMuCC1Pi;
  }
  //CCOther
  else if (allCutsPassed[2]){
    static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD2AntiNuMuCCOther;
  }

  // otherwise kUnassigned is used from the EventSummary constructor
  return (static_cast<AnaEventSummaryB*>(event.Summary)->EventSample != SampleId::kUnassigned);
}


//*********************************************************************
bool FillSummaryAction_antiNumuCCMultiPiFGD2::Apply(AnaEventC& event, ToyBoxB& boxB) const{
  //*********************************************************************

  // Cast the ToyBox to the appropriate type
  ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

  if(!box.HMPtrack) return 1;

  static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD2AntiNuMuCC0Pi] = box.HMPtrack;
  static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD2AntiNuMuCC1Pi] = box.HMPtrack;
  static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD2AntiNuMuCCOther] = box.HMPtrack;

  for(int i = 0; i < 4; ++i){
    static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD2AntiNuMuCC0Pi][i] = box.HMPtrack->PositionStart[i];
    static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD2AntiNuMuCC1Pi][i] = box.HMPtrack->PositionStart[i];
    static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD2AntiNuMuCCOther][i] = box.HMPtrack->PositionStart[i];
  }
  if(box.HMPtrack->GetTrueParticle()){
    static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD2AntiNuMuCC0Pi] = box.HMPtrack->GetTrueParticle()->TrueVertex;
    static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD2AntiNuMuCC1Pi] = box.HMPtrack->GetTrueParticle()->TrueVertex;
    static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD2AntiNuMuCCOther] = box.HMPtrack->GetTrueParticle()->TrueVertex;
  }
  return 1;
}

//**************************************************
bool antiNumuCCMultiPiFGD2Selection::IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* track, SystId_h systId, Int_t branch) const{
  //**************************************************

  return _antiNumuCCFGD2Selection.IsRelevantRecObjectForSystematic(event,track,systId,branch);
}

//**************************************************
bool antiNumuCCMultiPiFGD2Selection::IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueTrack, SystId_h systId, Int_t branch) const{
  //**************************************************

  return _antiNumuCCFGD2Selection.IsRelevantTrueObjectForSystematic(event,trueTrack,systId,branch);
}

//**************************************************
bool antiNumuCCMultiPiFGD2Selection::IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& box, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const{
  //**************************************************
  return _antiNumuCCMultiPiSelection.IsRelevantRecObjectForSystematicInToy(event, box, recObj, systId, branch);
}

//**************************************************
bool antiNumuCCMultiPiFGD2Selection::IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& box, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
  //**************************************************
  return _antiNumuCCMultiPiSelection.IsRelevantTrueObjectForSystematicInToy(event, box, trueObj, systId, branch);
}


//**************************************************
bool antiNumuCCMultiPiFGD2Selection::IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const{
  //**************************************************

  return _antiNumuCCMultiPiSelection.IsRelevantSystematic(event,box,systId,branch);
}

//**************************************************
Int_t antiNumuCCMultiPiFGD2Selection::GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  return _antiNumuCCMultiPiSelection.GetRelevantRecObjectGroupsForSystematic(systId, IDs, branch);
}

//**************************************************
Int_t antiNumuCCMultiPiFGD2Selection::GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  return _antiNumuCCMultiPiSelection.GetRelevantTrueObjectGroupsForSystematic(systId, IDs, branch);
}

//**************************************************
void antiNumuCCMultiPiFGD2Selection::InitializeEvent(AnaEventC& eventBB){
  //**************************************************

  AnaEventB& event = *static_cast<AnaEventB*>(&eventBB); 

  _antiNumuCCFGD2Selection.InitializeEvent(event);	
  boxUtils::FillTracksWithECal(event);
  boxUtils::FillFGDMichelElectrons(event, (SubDetId::SubDetEnum)GetDetectorFV(),
      (bool)ND::params().GetParameterI("psycheSelections.antinumuCCMultiPi.Prod5Cuts"));

  boxUtils::FillTrajsInECal(event);

}

//********************************************************************
bool antiNumuCCMultiPiFGD2Selection::CheckRedoSelection(const AnaEventC& event, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep){
  //********************************************************************

  return _antiNumuCCMultiPiSelection.CheckRedoSelection(event, PreviousToyBox, redoFromStep);							    
}
