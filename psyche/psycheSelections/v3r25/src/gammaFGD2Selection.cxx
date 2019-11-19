#include "gammaFGD2Selection.hxx"
#include "gammaSelection.hxx"
#include "nueCCFGD2Selection.hxx"

#include "baseSelection.hxx"
#include "CutUtils.hxx"
#include "EventBoxUtils.hxx"
#include "VersioningUtils.hxx"
#include "SystematicUtils.hxx"
#include "trackerSelectionUtils.hxx"
#include "SystematicTuning.hxx"

#include "nueCCUtils.hxx"
#include "nueCutUtils.hxx"

//********************************************************************
gammaFGD2Selection::gammaFGD2Selection(bool forceBreak): SelectionBase(forceBreak, EventBoxId::kEventBoxTracker) {
//********************************************************************

  _useVetoes = (bool)(ND::params().GetParameterI("psycheSelections.gammaAnalysis.UseVetoes"));

  _gammaSelection.Initialize();

}

//********************************************************************
void gammaFGD2Selection::DefineSteps(){
//********************************************************************

  // last "true" means the step sequence is broken if cut is not passed (default is "false")
  AddStep(StepBase::kCut,    "Event quality",                       new EventQualityCut(), true); // 0
  AddStep(StepBase::kCut,    "> 0 tracks ",                         new MultiplicityCut(), true); // 1
  AddStep(StepBase::kAction, "Find leading tracks",                 new FindLeadingTracksAction_gamma() );
  AddStep(StepBase::kAction, "Find vertex",                         new FillVertex() );
  AddStep(StepBase::kAction, "Fill summary",                        new FillSummaryActionGamma_FGD2() );

  AddStep(StepBase::kCut,    "# TPC nodes 1ary track",              new PrimaryTrackQualityCut_gamma() ); // 2

  AddStep(StepBase::kCut,    "Electron PID",                        new PIDCut_gamma() ); // 3
  //AddStep(StepBase::kCut,    "Highest Momentum Track Momentum cut", new HighestMomentumCut() );
  AddStep(StepBase::kAction, "Find secondary tracks",               new FindPairsAction() );
  AddStep(StepBase::kCut,    "Secondary Track",                     new PairTrackCut_gamma() ); // 4
  AddStep(StepBase::kCut,    "Invariant Mass Cut",                  new MinvCut_gamma() ); // 5

  if(_useVetoes){
    // Upstream vetos. Reduce OOFV background.

    // Do we actually need the next two? FGD1 veto is applied later on...
    //AddStep(StepBase::kAction, "find best FGD1 e+e- pair",            new FindFGD1PairsAction_gamma() );
    //AddStep(StepBase::kCut,    "Pair Veto",                           new FGD1PairCut_gamma() ); 

    AddStep(StepBase::kAction,	"find TPC Veto tracks",			new FillTPCVetoTrackAction());
    AddStep(StepBase::kCut,	"TPC Veto",				new TPCVetoCut_FGD2()); // 6
 
    AddStep(StepBase::kCut,     "External FGD2",                        new ExternalFGD2layersCut()); // 7

    AddStep(StepBase::kAction,	"find P0D-FGD1 veto track",		new FindP0DVetoTrackAction());
    AddStep(StepBase::kCut,	"P0D-FGD1 Veto",			new P0DFGD1VetoCut()); // 8

    AddStep(StepBase::kAction,	"find ecal veto track",			new FindECalVetoTrackAction());
    AddStep(StepBase::kCut,	"ECal Veto",				new ECalVetoCut()); // 9
  }
  
  SetBranchAlias(0, "Gamma Selection");

  _FindLeadingTracksStepIndex = GetStepNumber("Find leading tracks");
  _TotalMultiplicityCutIndex  = GetCutNumber("> 0 tracks ");

  _ElecPIDCutIndex            = GetCutNumber("Electron PID");
  _ElecPIDStepIndex           = GetStepNumber("Electron PID");

  // By default the preselection correspond to cuts 0-3
  // It means that if any of the four first cuts (0,1,2,3) is not passed
  // the loop over toys will be broken ===> A single toy will be run
  SetPreSelectionAccumLevel(3);

}

//**************************************************
bool FindFGD1PairsAction_gamma::Apply(AnaEventC& eventC, ToyBoxB& boxB) const {
//**************************************************
  
  AnaEventB& event = *static_cast<AnaEventB*>(&eventC);
  ToyBoxNueCC& nuebox = *static_cast<ToyBoxNueCC*>(&boxB);

  AnaTrackB* MainTrackFGD1 = nueCCUtils::FindLeadingTrackInDet(event, SubDetId::kFGD1);

  // Initialise
  nuebox.VetoTrack = NULL;

  if(!MainTrackFGD1)
    return true;

  nuebox.VetoTrack = nueCCUtils::FindPairTrack(event, *MainTrackFGD1, _delta_pos_max, _pullel_min,  _pullel_max);

  return true;

}

//**************************************************
bool FGD1PairCut_gamma::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************

  (void) event;
  
  ToyBoxNueCC& nuebox = *static_cast<ToyBoxNueCC*>(&boxB);
  
  if(nuebox.VetoTrack) return false;

  return true;

}

//********************************************************************
void gammaFGD2Selection::DefineDetectorFV(){
//********************************************************************
  
  // The detector in which the selection is applied
  SetDetectorFV(SubDetId::kFGD2);

  // Set the detector field into the selection being used
  _gammaSelection.SetDetectorFV(SubDetId::kFGD2);
}

//********************************************************************
bool FillSummaryActionGamma_FGD2::Apply(AnaEventC& eventC, ToyBoxB& boxB) const{
//********************************************************************

  ToyBoxNueCC& box = *static_cast<ToyBoxNueCC*>(&boxB);

  if(!box.MainTrack) return false;
  
  static_cast<AnaEventSummaryB*>(eventC.Summary)->LeptonCandidate[SampleId::kFGD2Gamma            ] = box.MainTrack;
  static_cast<AnaEventSummaryB*>(eventC.Summary)->LeptonCandidate[SampleId::kFGD2GammaInAntiNuMode] = box.MainTrack;

  for(int i = 0; i < 4; ++i){
    static_cast<AnaEventSummaryB*>(eventC.Summary)->VertexPosition[SampleId::kFGD2Gamma             ][i] = box.MainTrack->PositionStart[i];
    static_cast<AnaEventSummaryB*>(eventC.Summary)->VertexPosition[SampleId::kFGD2GammaInAntiNuMode][i] = box.MainTrack->PositionStart[i];
  }

  if(box.MainTrack->GetTrueParticle()){
    static_cast<AnaEventSummaryB*>(eventC.Summary)->TrueVertex[SampleId::kFGD2Gamma            ] = box.MainTrack->GetTrueParticle()->TrueVertex;
    static_cast<AnaEventSummaryB*>(eventC.Summary)->TrueVertex[SampleId::kFGD2GammaInAntiNuMode] = box.MainTrack->GetTrueParticle()->TrueVertex;
  }
  
  return true;

}

//********************************************************************
bool gammaFGD2Selection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
//********************************************************************

  AnaEventB& eventB = static_cast<AnaEventB&>(event);
  bool isRHC = anaUtils::IsRHC(eventB.EventInfo.Run);
  AnaEventSummaryB* eveSumB = static_cast<AnaEventSummaryB*>(event.Summary);

  if(allCutsPassed[0]){
    if(!isRHC)
      eveSumB->EventSample = SampleId::kFGD2Gamma;
    else
      eveSumB->EventSample = SampleId::kFGD2GammaInAntiNuMode;
  }
  
  return (eveSumB->EventSample != SampleId::kUnassigned);

}

//*********************************************************************
void gammaFGD2Selection::InitializeEvent(AnaEventC& eventC){
//*********************************************************************

  _gammaSelection.InitializeEvent(eventC);

}

//*********************************************************************
bool gammaFGD2Selection::IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const{
//*********************************************************************

  return _gammaSelection.IsRelevantRecObjectForSystematic(event, recObj, systId, branch);

}

//*********************************************************************
bool gammaFGD2Selection::IsRelevantTrueObjectForSystematic(const AnaEventC& eventC, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
//*********************************************************************

  return _gammaSelection.IsRelevantTrueObjectForSystematic(eventC, trueObj, systId, branch);

}

//**************************************************
bool gammaFGD2Selection::IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const{
//**************************************************

  return _gammaSelection.IsRelevantRecObjectForSystematicInToy(event, boxB, recObj, systId, branch);
  
}

//**************************************************
bool gammaFGD2Selection::IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
//**************************************************

  return _gammaSelection.IsRelevantTrueObjectForSystematicInToy(event, boxB, trueObj, systId, branch);

}

//*********************************************************************
bool gammaFGD2Selection::IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& boxB, SystId_h systId, Int_t branch) const{
  //*********************************************************************

  return _gammaSelection.IsRelevantSystematic(event, boxB, systId, branch);

}

//**************************************************
Int_t gammaFGD2Selection::GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  return _gammaSelection.GetRelevantRecObjectGroupsForSystematic(systId, IDs, branch);
}

//**************************************************
Int_t gammaFGD2Selection::GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  return _gammaSelection.GetRelevantTrueObjectGroupsForSystematic(systId, IDs, branch);
}

//*********************************************************************
bool gammaFGD2Selection::CheckRedoSelection(const AnaEventC& eventC, const ToyBoxB& PreviousToyBoxB, Int_t& redoFromStep){
//*********************************************************************

  // For this one we don't call the _gammaSelection since the cuts could actually be in a different order
  
  const AnaEventB& event = *static_cast<const AnaEventB*>(&eventC); 

  // cast the box to the appropriate type
  const ToyBoxNueCC& PreviousToyBox = *static_cast<const ToyBoxNueCC*>(&PreviousToyBoxB);

  if(!PreviousToyBox.HMtrack) return false;

  // TODO: The code below implies calling cutUtils::FindLeadingTracks(event, box) twice. Can we avoid that ?
  if(PreviousToyBox.MaxAccumLevel > _TotalMultiplicityCutIndex){
    ToyBoxNueCC box;
    trackerSelUtils::FindLeadingTracks(event, box);
    
    // Redo the selection if any of the leading tracks changes identity
    if(PreviousToyBox.SHMNtrack != box.SHMNtrack  || 
       PreviousToyBox.SHMPtrack != box.SHMPtrack  ||
       PreviousToyBox.HMNtrack  != box.HMNtrack   ||
       PreviousToyBox.HMPtrack  != box.HMPtrack   ||
       PreviousToyBox.SHMtrack  != box.SHMtrack   ||
       PreviousToyBox.HMtrack   != box.HMtrack){
      
      redoFromStep = _FindLeadingTracksStepIndex;
      return true;
    }
  }

  if(PreviousToyBox.MaxAccumLevel >= _ElecPIDCutIndex){
    //bool pidCut = nueCutUtils::TPCElectronPull(PreviousToyBox.HMtrack, _elepullpri_min, _elepullpri_max, _elepullpri_min, _elepullpri_max, 0.0);
    //if(( pidCut && (PreviousToyBox.AccumLevel[0] == _ElecPIDCutIndex)) ||
    // (!pidCut && (PreviousToyBox.AccumLevel[0]  > _ElecPIDCutIndex))){
    // We should redo the selection starting from the PIDCut step
    redoFromStep = _ElecPIDStepIndex;
    return true;
    //}
  }

  return false;
}
