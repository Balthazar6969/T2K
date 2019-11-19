#include "antiNueCCSelection.hxx"
#include "nueCCFGD2Selection.hxx"
#include "antiNueCCFGD2Selection.hxx"

#include "baseSelection.hxx"
#include "CutUtils.hxx"
#include "EventBoxUtils.hxx"
#include "VersioningUtils.hxx"
#include "SystematicUtils.hxx"
#include "trackerSelectionUtils.hxx"

#include "nueCCUtils.hxx"
#include "nueCutUtils.hxx"
#include "SystematicTuning.hxx"

//********************************************************************
antiNueCCFGD2Selection::antiNueCCFGD2Selection(bool forceBreak): SelectionBase(forceBreak, EventBoxId::kEventBoxTracker) {
//********************************************************************
  
  // Initialize selections being used
  _nueCCFGD2Selection.Initialize();

  // Variables needed from datacard
  _protonregion_low        = (Float_t)ND::params().GetParameterD("psycheSelections.antiNueCCAnalysis.Analysis.ProtonMomentumRegion.Low");

  // Initialize systematic tuning parameters
  systTuning::Initialize();
}

//********************************************************************
void antiNueCCFGD2Selection::DefineDetectorFV(){
//********************************************************************

  // The detector in which the selection is applied
  SetDetectorFV(SubDetId::kFGD2);
  // Set the detector field into the selection being used
  _nueCCFGD2Selection.SetDetectorFV(SubDetId::kFGD2);
}

//********************************************************************
void antiNueCCFGD2Selection::DefineSteps(){
//********************************************************************

  // last "true" means the step sequence is broken if cut is not passed (default is "false")
  
  AddStep(StepBase::kCut,       "event quality",                        new EventQualityCut(), true); // 0
  AddStep(StepBase::kCut,       "> 0 tracks ",                          new MultiplicityCut(), true); // 1
  AddStep(StepBase::kAction,	"find leading tracks",			new FillLeadingTracksAction_antiNueCC());
  AddStep(StepBase::kAction,	"Fill vertex info",			new FillVertex());
  AddStep(StepBase::kAction,	"fill summary",				new FillSummaryAction_antiNueCCFGD2Selection());
  
  AddStep(StepBase::kCut,	"TPC Quality",				new TrackQualityCut(), true); // 2
  
  // e- PID cuts.	
  AddStep(StepBase::kCut,	"TPC Electron Pull",			new TPCElectronPullCut()); // 3
  AddStep(StepBase::kCut,	"TPC Muon Pull",			new TPCMuonPullCut()); // 4
  AddStep(StepBase::kCut,	"TPC Pion Pull",			new TPCPionPullCut()); // 5
  AddStep(StepBase::kCut,	"Ecal EMEnergy",			new EcalEMEnergyCut_antiNueCC()); // 6
  AddStep(StepBase::kCut,	"Ecal MIPEM PID",			new EcalMIPEMPIDCut()); // 7

  // Minimum momentum
  AddStep(StepBase::kCut,	"Momentum",				new HighestMomentumCut()); // 8

  // Here the most energetic track must be positive and it has to be the most energetic compared to all TPC tracks
  AddStep(StepBase::kAction,	"find second most energetic track",	new FindSecondMostEnergeticTPCTrack_antiNueCC());
  AddStep(StepBase::kCut,	"Most energetic positive track",	new MostEnergeticTrackCut_antiNueCC()); // 9

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
  AddStep(StepBase::kCut,	"Ecal Veto",				new ECalVetoCut()); // 15
  
  // Proton rejection in nuebar selection
  AddStep(StepBase::kCut,       "E over P",                             new EoverPCut_antiNueCC()); // 16
  AddStep(StepBase::kCut,       "Ecal EMHIP PID",                       new EcalEMHIPPIDCut_antiNueCC()); // 17
  AddStep(StepBase::kCut,	"FGD Shower",				new FGDShowerCut_antiNueCC()); // 18
  
  // ToF for antinue selection - applied only to fhc selection
  AddStep(StepBase::kCut,	"ToF",				        new ToFCut_antiNueCC()); // 19

  // Momentum quality cut
  AddStep(StepBase::kCut,	"Momentum quality",			new MomentumQualityCut()); // 20
  
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
bool antiNueCCFGD2Selection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
//********************************************************************

  AnaEventB& eventB = static_cast<AnaEventB&>(event);

  bool isRHC = anaUtils::IsRHC(eventB.EventInfo.Run);
  AnaEventSummaryB* eveSumB = static_cast<AnaEventSummaryB*>(event.Summary);

  if(allCutsPassed[0]){
    if(!isRHC)
      eveSumB->EventSample = SampleId::kFGD2AntiNuEBkgInNuModeCC; 
    else
      eveSumB->EventSample = SampleId::kFGD2AntiNuECC;
  }
  
  return (eveSumB->EventSample != SampleId::kUnassigned);
  
}

//********************************************************************
bool FillSummaryAction_antiNueCCFGD2Selection::Apply(AnaEventC& event, ToyBoxB& box) const{
//********************************************************************

  //ToyBoxTracker& nuebox = *static_cast<ToyBoxTracker*>(&box);
  ToyBoxNueCC&  nuebox = *static_cast<ToyBoxNueCC*>(&box);
  
  if(!nuebox.MainTrack) return false;
  
  static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD2AntiNuECC           ] = nuebox.MainTrack;
  static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD2AntiNuEBkgInNuModeCC] = nuebox.MainTrack;
  for(Int_t i = 0; i < 4; ++i){
    static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD2AntiNuECC           ][i] = nuebox.MainTrack->PositionStart[i];
    static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD2AntiNuEBkgInNuModeCC][i] = nuebox.MainTrack->PositionStart[i];

  }
  if(nuebox.MainTrack->GetTrueParticle()){
    static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD2AntiNuECC           ] = nuebox.MainTrack->GetTrueParticle()->TrueVertex;
    static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD2AntiNuEBkgInNuModeCC] = nuebox.MainTrack->GetTrueParticle()->TrueVertex;
  }
  
  return true;
}

//*********************************************************************
bool antiNueCCFGD2Selection::IsRelevantRecObjectForSystematic(const AnaEventC& eventC, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const{
//*********************************************************************

  return _nueCCFGD2Selection.IsRelevantRecObjectForSystematic(eventC, recObj, systId, branch);
}

//*********************************************************************
bool antiNueCCFGD2Selection::IsRelevantTrueObjectForSystematic(const AnaEventC& eventC, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
//*********************************************************************

  return _nueCCFGD2Selection.IsRelevantTrueObjectForSystematic(eventC, trueObj, systId, branch);
}

//**************************************************
bool antiNueCCFGD2Selection::IsRelevantRecObjectForSystematicInToy(const AnaEventC& eventC, const ToyBoxB& boxB, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const{
//**************************************************

  AnaTrackB* track = static_cast<AnaTrackB*>(recObj);

  // cast the box to the appropriate type
  const ToyBoxNueCC& box = *static_cast<const ToyBoxNueCC*>(&boxB);

  // Special nuebar systematics
  if(systId == SystId::kECalEmHipPID){
    // Only applied to main track
    if (track != box.MainTrack) return false;

    if(!nueCCUtils::UseEcal(track)) return false;
    // Only applied for tracks with p > 600 MeV/c
    if(track->Momentum < _protonregion_low) return false;

    return true;
  }
 
  return _nueCCFGD2Selection.IsRelevantRecObjectForSystematicInToy(eventC, boxB, recObj, systId, branch);
}

//**************************************************
bool antiNueCCFGD2Selection::IsRelevantTrueObjectForSystematicInToy(const AnaEventC& eventC, const ToyBoxB& boxB, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
//**************************************************

  if(systId == SystId::kSIProton){
    const ToyBoxTracker& box = *static_cast<const ToyBoxTracker*>(&boxB); 
  
    AnaTrueParticleB* trueTrack = static_cast<AnaTrueParticleB*>(trueObj);
  
    // If this trueTrack is NOT associated to the MainTrack, consider the posibility of this trueTrack to become the MainTrack
    // This is to be tuned: e.g. may consider explicitely those not reconstructed in TPC
    // also the ones that have the momentum ~closer to the Bethe-Bloch crossing point, 
    // i.e. when proton background starts to become non-negligible
    if (trueTrack->PDG == 2212 && trueTrack->Momentum > box.MainTrack->Momentum
	&& trueTrack->Momentum > _protonregion_low) return true;
  }

  return _nueCCFGD2Selection.IsRelevantTrueObjectForSystematicInToy(eventC, boxB, trueObj, systId, branch);
}

//*********************************************************************
bool antiNueCCFGD2Selection::IsRelevantSystematic(const AnaEventC& eventC, const ToyBoxB& boxB, SystId_h systId, Int_t branch) const{
//*********************************************************************

  // Anti-nue FGD2 related
  if(systId == SystId::kECalEmHipPID) return true;

  // Rest are the same as in the nue analysis
  return _nueCCFGD2Selection.IsRelevantSystematic(eventC, boxB, systId, branch);
}

//**************************************************
Int_t antiNueCCFGD2Selection::GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  Int_t ngroups = _nueCCFGD2Selection.GetRelevantRecObjectGroupsForSystematic(systId, IDs, branch);
  if (ngroups) return ngroups;
  
  // Consider the systematics specific for the analysis
  std::vector<std::vector<Int_t> > groups(SystId::SystEnumLast_SystId, std::vector<Int_t>(1, -1));   
  
  // Explicitely set the groups for various systeamtics that need them, 
  // this allows to have a transparent set/control of the systematics by the
  // selection  

  using namespace SystId;
  
  // --- Systematic         Detectors                                   FGD2                                   
  groups[kECalEmHipPID] =  anaUtils::CreateVectorI(1, EventBoxTracker::kTracksWithECal);
   
  // The systematics not mentioned above will get no groups
  // Check the systematic index is ok
  try {
    groups.at(systId);
  }
  catch (const std::out_of_range& oor) {
    std::cerr << this->Name() << " GetRelevantTrueObjectGroupsForSystematic: syst index beyond limits "<< systId << " Error " << oor.what() << '\n';
    exit(1);
  }
  
  if (groups[systId][0] >= 0) IDs[ngroups++] = groups[systId][0];
  return ngroups;
}

//**************************************************
Int_t antiNueCCFGD2Selection::GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  return _nueCCFGD2Selection.GetRelevantTrueObjectGroupsForSystematic(systId, IDs, branch);
}


//*********************************************************************
bool antiNueCCFGD2Selection::CheckRedoSelection(const AnaEventC& eventC, const ToyBoxB& PreviousToyBoxB, Int_t& redoFromStep){
//*********************************************************************

   const AnaEventB& event = *static_cast<const AnaEventB*>(&eventC); 
  
  // cast the box to the appropriate type
  const ToyBoxNueCC& PreviousToyBox = *static_cast<const ToyBoxNueCC*>(&PreviousToyBoxB);

  /// Nothing to do if there is no HM or HMP track
  if(!PreviousToyBox.HMtrack || !PreviousToyBox.HMPtrack) return false;

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

  // When the HMP track does not change identity, Redo the selection if the effect of the PID cut is different.
  // We have previously saved in the EventBox the AccumLevel of the previous toy for each branch.
  // PreviousToyBox->AccumLevel[0]>_ElecPIDCutIndex means that the PID cut was passed, so we check whether the cut
  // was passed in the previous toy and not in the current one, or the opposit, it was not passed before
  // and it is passed now.
  
  if(PreviousToyBox.MaxAccumLevel >= _ElecPIDCutIndex){
    //bool pidCut = nueCutUtils::TPCElectronPull(PreviousToyBox.HMPtrack, _pullel_accept_min, _pullel_accept_max, _pullel_accept_tight_min, _pullel_accept_tight_max, _minMom_ecal);
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
void antiNueCCFGD2Selection::InitializeEvent(AnaEventC& eventC){
//*********************************************************************

  _nueCCFGD2Selection.InitializeEvent(eventC);
}
