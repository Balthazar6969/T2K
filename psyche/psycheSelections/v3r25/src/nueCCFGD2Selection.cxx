#include "nueCCFGD2Selection.hxx"

#include "baseSelection.hxx"
#include "CutUtils.hxx"
#include "EventBoxUtils.hxx"
#include "VersioningUtils.hxx"
#include "SystematicUtils.hxx"
#include "trackerSelectionUtils.hxx"

#include "nueCCUtils.hxx"
#include "nueCutUtils.hxx"

//********************************************************************
nueCCFGD2Selection::nueCCFGD2Selection(bool forceBreak): SelectionBase(forceBreak, EventBoxId::kEventBoxTracker) {
//********************************************************************
  
  // Initialize the nueCCSelection, which is used
  _nueCCSelection.Initialize();
}

//********************************************************************
void nueCCFGD2Selection::DefineDetectorFV(){
//********************************************************************

  // The detector in which the selection is applied
  SetDetectorFV(SubDetId::kFGD2);
  _nueCCSelection.SetDetectorFV(SubDetId::kFGD2);
}

//********************************************************************
void nueCCFGD2Selection::DefineSteps(){
//********************************************************************

  // last "true" means the step sequence is broken if cut is not passed (default is "false")
  
  AddStep(StepBase::kCut,       "event quality",                        new EventQualityCut(), true); // 0
  AddStep(StepBase::kCut,       "> 0 tracks ",                          new MultiplicityCut(), true); // 1
  AddStep(StepBase::kAction,	"find leading tracks",			new FillLeadingTracksAction());     
 
  AddStep(StepBase::kAction,	"Fill vertex info",			new FillVertex());
  AddStep(StepBase::kAction,	"fill summary",				new FillSummaryAction_nueCCFGD2Selection());
  
  AddStep(StepBase::kCut,	"TPC Quality",				new TrackQualityCut(), true); // 2
  
  // e- PID cuts.	
  AddStep(StepBase::kCut,	"TPC Electron Pull",			new TPCElectronPullCut()); // 3
  AddStep(StepBase::kCut,	"TPC Muon Pull",			new TPCMuonPullCut()); // 4
  AddStep(StepBase::kCut,	"TPC Pion Pull",			new TPCPionPullCut()); // 5
  AddStep(StepBase::kCut,	"Ecal EMEnergy",			new EcalEMEnergyCut()); // 6
  AddStep(StepBase::kCut,	"Ecal MIPEM PID",			new EcalMIPEMPIDCut()); // 7
  AddStep(StepBase::kCut,	"EoP",		                	new EOverPCut()); // 8

  // Minimum momentum
  AddStep(StepBase::kCut,	"Momentum",				new HighestMomentumCut()); // 9

  // Upstream vetos. Reduce OOFV background.
  AddStep(StepBase::kAction,	"find TPC Veto tracks",			new FillTPCVetoTrackAction());
  AddStep(StepBase::kCut,	"TPC Veto",				new TPCVetoCut_FGD2()); // 10
  AddStep(StepBase::kCut,	"Gamma Iso Veto",			new GammaIsoVetoCut()); // 11
  AddStep(StepBase::kCut,       "External FGD2",                        new ExternalFGD2layersCut()); // 12
  
  AddStep(StepBase::kAction,	"find best e+e- pair",			new FindPairsAction());
  AddStep(StepBase::kCut,	"Pair Veto",				new PairCut()); // 13  

  AddStep(StepBase::kAction,	"find P0D-FGD1 veto track",		new FindP0DVetoTrackAction());
  AddStep(StepBase::kCut,	"P0D-FGD1 Veto",			new P0DFGD1VetoCut()); // 14  

  AddStep(StepBase::kAction,	"find ecal veto track",			new FindECalVetoTrackAction());
  AddStep(StepBase::kCut,	"ECal Veto",				new ECalVetoCut()); // 15

  // ToF
  AddStep(StepBase::kCut,	"ToF",				        new ToFCut()); // 16

  // Momentum quality cut
  AddStep(StepBase::kCut,	"Momentum quality",			new MomentumQualityCut()); // 17
 
  SetBranchAlias(0,"trunk");

  // By default the preselection correspond to cuts 0-3
  // It means that if any of the four first cuts (0,1,2,3) is not passed
  // the loop over toys will be broken ===> A single toy will be run
  SetPreSelectionAccumLevel(3);
  _FindLeadingTracksStepIndex = GetStepNumber("find leading tracks");
  _TotalMultiplicityCutIndex  = GetCutNumber("> 0 tracks ");
  
  _ElecPIDCutIndex            = GetCutNumber("TPC Electron Pull");
  _ElecPIDStepIndex           = GetStepNumber("TPC Electron Pull");
}

//********************************************************************
bool nueCCFGD2Selection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
//********************************************************************

  AnaEventB& eventB = static_cast<AnaEventB&>(event);
  bool isRHC = anaUtils::IsRHC(eventB.EventInfo.Run);
  AnaEventSummaryB* eveSumB = static_cast<AnaEventSummaryB*>(event.Summary);

  if(allCutsPassed[0]){
    if(!isRHC)
      eveSumB->EventSample = SampleId::kFGD2NuECC;
    else
      eveSumB->EventSample = SampleId::kFGD2NuEBkgInAntiNuModeCC;
  }
  
  return (eveSumB->EventSample != SampleId::kUnassigned); 
} 

//********************************************************************
bool FillSummaryAction_nueCCFGD2Selection::Apply(AnaEventC& event, ToyBoxB& box) const{
//********************************************************************

  //ToyBoxTracker& nuebox = *static_cast<ToyBoxTracker*>(&box);
  ToyBoxNueCC&  nuebox = *static_cast<ToyBoxNueCC*>(&box);
  
  if(!nuebox.MainTrack) return false;

  static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD2NuECC               ] = nuebox.MainTrack;
  static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD2NuEBkgInAntiNuModeCC] = nuebox.MainTrack;
  for(int i = 0; i < 4; ++i){
    static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD2NuECC               ][i] = nuebox.MainTrack->PositionStart[i];
    static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD2NuEBkgInAntiNuModeCC][i] = nuebox.MainTrack->PositionStart[i];
  }
  if(nuebox.MainTrack->GetTrueParticle()){
    static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD2NuECC               ] = nuebox.MainTrack->GetTrueParticle()->TrueVertex;
    static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD2NuEBkgInAntiNuModeCC] = nuebox.MainTrack->GetTrueParticle()->TrueVertex;
  }

  return true;
}

//*********************************************************************
bool nueCCFGD2Selection::IsRelevantRecObjectForSystematic(const AnaEventC& eventC, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const{
//*********************************************************************

  return _nueCCSelection.IsRelevantRecObjectForSystematic(eventC, recObj, systId, branch);
 
}

//*********************************************************************
bool nueCCFGD2Selection::IsRelevantTrueObjectForSystematic(const AnaEventC& eventC, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
//*********************************************************************

  return _nueCCSelection.IsRelevantTrueObjectForSystematic(eventC, trueObj, systId, branch);
}

//**************************************************
bool nueCCFGD2Selection::IsRelevantRecObjectForSystematicInToy(const AnaEventC& eventC, const ToyBoxB& boxB, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const{
//**************************************************

  return _nueCCSelection.IsRelevantRecObjectForSystematicInToy(eventC, boxB, recObj, systId, branch);
}

//**************************************************
bool nueCCFGD2Selection::IsRelevantTrueObjectForSystematicInToy(const AnaEventC& eventC, const ToyBoxB& boxB, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
//**************************************************

  return _nueCCSelection.IsRelevantTrueObjectForSystematicInToy(eventC, boxB, trueObj, systId, branch);
}

//*********************************************************************
bool nueCCFGD2Selection::IsRelevantSystematic(const AnaEventC& eventC, const ToyBoxB& boxB, SystId_h systId, Int_t branch) const{
//*********************************************************************

  return _nueCCSelection.IsRelevantSystematic(eventC, boxB, systId, branch);
}

//**************************************************
Int_t nueCCFGD2Selection::GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  return _nueCCSelection.GetRelevantRecObjectGroupsForSystematic(systId, IDs, branch);
}

//**************************************************
Int_t nueCCFGD2Selection::GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  return _nueCCSelection.GetRelevantTrueObjectGroupsForSystematic(systId, IDs, branch);
}


//*********************************************************************
bool nueCCFGD2Selection::CheckRedoSelection(const AnaEventC& eventC, const ToyBoxB& PreviousToyBoxB, Int_t& redoFromStep){
//*********************************************************************
  
  const AnaEventB& event = *static_cast<const AnaEventB*>(&eventC); 
  
  // cast the box to the appropriate type
  const ToyBoxNueCC& PreviousToyBox = *static_cast<const ToyBoxNueCC*>(&PreviousToyBoxB);

  /// Nothing to do if there is no HMN track
  if(!PreviousToyBox.HMNtrack) return false;

  // TODO: The code below implies calling cutUtils::FindLeadingTracks(event, box) twice. Can we avoid that ?
  if(PreviousToyBox.MaxAccumLevel > _TotalMultiplicityCutIndex){
    ToyBoxNueCC box;
    trackerSelUtils::FindLeadingTracks(event, box);

    // Redo the selection if any of the leading tracks changes identity
    if(PreviousToyBox.SHMNtrack!=box.SHMNtrack || 
       PreviousToyBox.SHMPtrack!=box.SHMPtrack || 
       PreviousToyBox.HMNtrack !=box.HMNtrack  || 
       PreviousToyBox.HMPtrack !=box.HMPtrack  ||
       PreviousToyBox.SHMtrack !=box.SHMtrack  ||
       PreviousToyBox.HMtrack  !=box.HMtrack){
      
      redoFromStep = _FindLeadingTracksStepIndex;
      return true;
    }
  }

  // When the HMN track does not change identity, Redo the selection if the effect of the PID cut is different.
  // We have previously saved in the EventBox the AccumLevel of the previous toy for each branch.
  // PreviousToyBox->AccumLevel[0]>_ElecPIDCutIndex means that the PID cut was passed, so we check whether the cut
  // was passed in the previous toy and not in the current one, or the opposit, it was not passed before
  // and it is passed now.
  
  if(PreviousToyBox.MaxAccumLevel >= _ElecPIDCutIndex){
    //bool pidCut = nueCutUtils::TPCElectronPull(PreviousToyBox.HMNtrack, _pullel_accept_min, _pullel_accept_max, _pullel_accept_tight_min, _pullel_accept_tight_max, _minMom_ecal);
    //if(( pidCut && (PreviousToyBox.AccumLevel[0] == _ElecPIDCutIndex)) ||
    // (!pidCut && (PreviousToyBox.AccumLevel[0]  > _ElecPIDCutIndex))){
    // We should redo the selection starting from the PIDCut step
    redoFromStep = _ElecPIDStepIndex;
    return true;
    //}
  }
  
  return false;
}

//*********************************************************************
void nueCCFGD2Selection::InitializeEvent(AnaEventC& eventC){
//*********************************************************************

  _nueCCSelection.InitializeEvent(eventC);
}

//**************************************************
bool EOverPCut::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************
  
  (void) event;

  // cast the box to the appropriate type
  ToyBoxNueCC& nuebox = *static_cast<ToyBoxNueCC*>(&boxB);
 
  AnaTrackB* track = nuebox.MainTrack;
  if(!track) return false;

  return nueCutUtils::EOverPNuE(track, _eoverp_threshold, _eoverp_minmom);
}

//**************************************************
bool P0DFGD1VetoCut::Apply(AnaEventC& eventC, ToyBoxB& boxB) const {
//**************************************************
  
  (void) eventC;

  // Cast the ToyBox to the appropriate type
  ToyBoxNueCC&  nuebox = *static_cast<ToyBoxNueCC*>(&boxB);

  if(nuebox.P0DVetoP0DTracks > 0 || nuebox.P0DVetoP0DEcalTracks > 0 || nuebox.P0DVetoFGD1Tracks > 0) return false;
  
  return true;
}

//**************************************************
bool TPCVetoCut_FGD2::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************
  
  (void) event;

  // cast the box to the appropriate type
  ToyBoxNueCC&  nuebox = *static_cast<ToyBoxNueCC*>(&boxB);
  
  AnaTrackB* track = nuebox.MainTrack;
  if(!track) return false;
  
  AnaTrackB* vetoTrack = nuebox.VetoTrack;
  if(!vetoTrack) return true;

  return (nueCutUtils::TPCVeto(*track, vetoTrack, _delta_z_min));
}

//********************************************************************
bool ExternalFGD2layersCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//********************************************************************

    (void)event;
    // Cast the ToyBox to the appropriate type
    ToyBoxNueCC&  nuebox = *static_cast<ToyBoxNueCC*>(&boxB);

    if(!nuebox.MainTrack) return false;

    // Reject external background from the first layer of FGD2
    if(nuebox.NOOFVTracks > 0) return false;

    // Reject external background from the last 2 layers of FGD2
    if(!nuebox.OOFVtrack) return true;

    if(nuebox.MainTrack->PositionStart[2] > 1770.) return false;

    return true;
}
