#include "antiNueCCSelection.hxx"

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
antiNueCCSelection::antiNueCCSelection(bool forceBreak): SelectionBase(forceBreak, EventBoxId::kEventBoxTracker) {
//********************************************************************

  // Initialize the nueCCSelection, which is used
  _nueCCSelection.Initialize();

  _protonregion_low        = (Float_t)ND::params().GetParameterD("psycheSelections.antiNueCCAnalysis.Analysis.ProtonMomentumRegion.Low");

  // Initialize systematic tuning parameters
  systTuning::Initialize();
}

//********************************************************************
void antiNueCCSelection::DefineDetectorFV(){
//********************************************************************

  // The detector in which the selection is applied
  SetDetectorFV(SubDetId::kFGD1);

  // Set the detector field into the selection being used
  _nueCCSelection.SetDetectorFV(SubDetId::kFGD1);
}

//********************************************************************
void antiNueCCSelection::DefineSteps(){
//********************************************************************

  // Last "true" means the step sequence is broken if cut is not passed (default is "false")
  AddStep(StepBase::kCut,       "event quality",                        new EventQualityCut(), true); // 0
  AddStep(StepBase::kCut,       "> 0 tracks ",                          new MultiplicityCut(), true); // 1
  AddStep(StepBase::kAction,	"find leading tracks",			new FillLeadingTracksAction_antiNueCC());     
  AddStep(StepBase::kAction,	"Fill vertex info",			new FillVertex());
  AddStep(StepBase::kAction,	"fill summary",				new FillSummaryAction_antiNueCC());
  
  AddStep(StepBase::kCut,	"TPC Quality",				new TrackQualityCut(), true); // 2
  
  // e- PID cuts.	
  AddStep(StepBase::kCut,	"TPC Electron Pull",			new TPCElectronPullCut()); // 3
  AddStep(StepBase::kCut,	"TPC Muon Pull",			new TPCMuonPullCut()); // 4
  AddStep(StepBase::kCut,	"TPC Pion Pull",			new TPCPionPullCut()); // 5 
  AddStep(StepBase::kCut,	"Ecal EMEnergy",			new EcalEMEnergyCut_antiNueCC()); // 6
  AddStep(StepBase::kCut,	"Ecal MIPEM PID",			new EcalMIPEMPIDCut()); // 7
  AddStep(StepBase::kCut,	"TPC2 PID",				new SecondTPCPIDCut_antiNueCC()); // 8

  // Minimum momentum
  AddStep(StepBase::kCut,	"Momentum",				new HighestMomentumCut()); // 9

  // Here the most energetic track must be positive and it has to be the most energetic compared to all TPC tracks
  AddStep(StepBase::kAction,	"find second most energetic track",	new FindSecondMostEnergeticTPCTrack_antiNueCC());
  AddStep(StepBase::kCut,	"Most energetic positive track",	new MostEnergeticTrackCut_antiNueCC()); // 10
  
  // Upstream vetos. Reduce OOFV background.
  AddStep(StepBase::kAction,	"find TPC Veto track",			new FillTPCVetoTrackAction());
  AddStep(StepBase::kCut,	"TPC Veto",				new TPCVetoCut()); // 11
  AddStep(StepBase::kCut,	"Gamma Iso Veto",			new GammaIsoVetoCut()); // 12
  AddStep(StepBase::kCut,       "External FGD1",                        new ExternalFGD1layersCut()); // 13
  
  AddStep(StepBase::kAction,	"find best e+e- pair",			new FindPairsAction());
  AddStep(StepBase::kCut,	"Pair Veto",				new PairCut()); // 14  

  AddStep(StepBase::kAction,	"find P0D veto track",			new FindP0DVetoTrackAction());
  AddStep(StepBase::kCut,	"P0D Veto",				new P0DVetoCut()); // 15
  
  AddStep(StepBase::kAction,	"find ecal veto track",			new FindECalVetoTrackAction());
  AddStep(StepBase::kCut,	"Ecal Veto",				new ECalVetoCut()); // 16

  // Proton rejection in nuebar selection
  AddStep(StepBase::kCut,       "E over P",                             new EoverPCut_antiNueCC()); // 17
  AddStep(StepBase::kCut,       "Ecal EMHIP PID",                       new EcalEMHIPPIDCut_antiNueCC()); // 18
  AddStep(StepBase::kCut,	"FGD2 Shower",				new FGDShowerCut_antiNueCC()); // 19

  AddStep(StepBase::kCut,	"FGD2 PID",				new FGD2ProtonPID_antiNueCC()); // 20
  
  // ToF for antinue selection
  AddStep(StepBase::kCut,	"ToF",				        new ToFCut_antiNueCC()); // 21

  // Momentum quality cut
  AddStep(StepBase::kCut,	"Momentum quality",			new MomentumQualityCut()); // 22

  SetBranchAlias(0,"trunk");


  // By default the preselection correspond to cuts 0-3
  // It means that if any of the four first cuts (0,1,2,3) is not passed
  // the loop over toys will be broken ===> A single toy will be run
  SetPreSelectionAccumLevel(3); //

  _FindLeadingTracksStepIndex = GetStepNumber("find leading tracks");
  _TotalMultiplicityCutIndex  = GetCutNumber("> 0 tracks ");
  
  _ElecPIDCutIndex            = GetCutNumber("TPC Electron Pull");
  _ElecPIDStepIndex           = GetStepNumber("TPC Electron Pull");
}

//********************************************************************
bool antiNueCCSelection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
//********************************************************************

  AnaEventB& eventB = static_cast<AnaEventB&>(event);

  bool isRHC = anaUtils::IsRHC(eventB.EventInfo.Run);
  AnaEventSummaryB* eveSumB = static_cast<AnaEventSummaryB*>(event.Summary);

  if(allCutsPassed[0]){
    if(!isRHC)
      eveSumB->EventSample = SampleId::kFGD1AntiNuEBkgInNuModeCC;
    else
      eveSumB->EventSample = SampleId::kFGD1AntiNuECC;
  }
  return (eveSumB->EventSample != SampleId::kUnassigned);

}

//********************************************************************
bool FillSummaryAction_antiNueCC::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//********************************************************************

  //ToyBoxTracker& nuebox = *static_cast<ToyBoxTracker*>(&boxB);
  ToyBoxNueCC&  nuebox = *static_cast<ToyBoxNueCC*>(&boxB);

  if(!nuebox.MainTrack) return false;
  
  static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD1AntiNuECC           ] = nuebox.MainTrack;
  static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD1AntiNuEBkgInNuModeCC] = nuebox.MainTrack;
  for(Int_t i = 0; i < 4; ++i){
    static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD1AntiNuECC           ][i] = nuebox.MainTrack->PositionStart[i];
    static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD1AntiNuEBkgInNuModeCC][i] = nuebox.MainTrack->PositionStart[i];

  }
  if(nuebox.MainTrack->GetTrueParticle()){
    static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD1AntiNuECC           ] = nuebox.MainTrack->GetTrueParticle()->TrueVertex;
    static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD1AntiNuEBkgInNuModeCC] = nuebox.MainTrack->GetTrueParticle()->TrueVertex;
  }
  
  return true;
}

//*********************************************************************
bool antiNueCCSelection::IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const{
//*********************************************************************

  return _nueCCSelection.IsRelevantRecObjectForSystematic(event, recObj, systId, branch);
}

//*********************************************************************
bool antiNueCCSelection::IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
//*********************************************************************

  return _nueCCSelection.IsRelevantTrueObjectForSystematic(event, trueObj, systId, branch);
}

//**************************************************
bool antiNueCCSelection::IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const{
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
  else if(systId == SystId::kFGD2Shower){
    // Only applied to main track
    if (track != box.MainTrack) return false;

    // Only applied for tracks with p > _protonregion_low
    if(track->Momentum < _protonregion_low) return false;

    // Conditions to apply this systematic
    if(nueCCUtils::UseEcal(track)){
      if(nueCutUtils::FGD2ShowerActivityEcal(track, _protonregion_low, box.FGD2ShowerNFGD1TPC2Tracks, box.FGD2ShowerNFGD2TPC3Tracks))
	return true;

      return false;
    }

    return true;
  }
 
  return _nueCCSelection.IsRelevantRecObjectForSystematicInToy(event, boxB, recObj, systId, branch);
}

//**************************************************
bool antiNueCCSelection::IsRelevantTrueObjectForSystematicInToy(const AnaEventC&event, const ToyBoxB& boxB, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
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

  return _nueCCSelection.IsRelevantTrueObjectForSystematicInToy(event, boxB, trueObj, systId, branch);
}

//*********************************************************************
bool antiNueCCSelection::IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& boxB, SystId_h systId, Int_t branch) const{
//*********************************************************************

  // Anti-nue FGD1 related
  if(systId == SystId::kECalEmHipPID) return true;
  if(systId == SystId::kFGD2Shower) return true;
  if(systId == SystId::kFgdPid) return true;

  // Rest are the same as in the nue analysis
  return _nueCCSelection.IsRelevantSystematic(event, boxB, systId, branch);
}

//**************************************************
Int_t antiNueCCSelection::GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  Int_t ngroups = _nueCCSelection.GetRelevantRecObjectGroupsForSystematic(systId, IDs, branch);
  if (ngroups) return ngroups;
  
  // Consider the systematics specific for the analysis
  std::vector<std::vector<Int_t> > groups(SystId::SystEnumLast_SystId, std::vector<Int_t>(1, -1));   
  
  // Explicitely set the groups for various systeamtics that need them, 
  // this allows to have a transparent set/control of the systematics by the
  // selection  

  using namespace SystId;
  
  // --- Systematic         Detectors                                   FGD1                    
  groups[kFgdPid        ] =  anaUtils::CreateVectorI(1, EventBoxTracker::kTracksWithTPCInFGD1FV);
  groups[kFGD2Shower    ] =  anaUtils::CreateVectorI(1, EventBoxTracker::kTracksWithTPC);
  groups[kECalEmHipPID  ] =  anaUtils::CreateVectorI(1, EventBoxTracker::kTracksWithECal);
   
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
Int_t antiNueCCSelection::GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  return _nueCCSelection.GetRelevantTrueObjectGroupsForSystematic(systId, IDs, branch);
}

//*********************************************************************
bool antiNueCCSelection::CheckRedoSelection(const AnaEventC& eventC, const ToyBoxB& PreviousToyBoxB, Int_t& redoFromStep){
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
void antiNueCCSelection::InitializeEvent(AnaEventC& eventC){
//*********************************************************************

  _nueCCSelection.InitializeEvent(eventC);
}

//*********************************************************************
bool FillLeadingTracksAction_antiNueCC::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//*********************************************************************

  ToyBoxNueCC& nuebox = *static_cast<ToyBoxNueCC*>(&boxB);
  
  //trackerSelUtils::FindLeadingTracksOld(event, box, false, det_FGD, true);
  trackerSelUtils::FindLeadingTracks(event,boxB);

  if(nuebox.HMPtrack) nuebox.MainTrack = nuebox.HMPtrack;
  if(!nuebox.MainTrack) return false;

  return true;
}

//*********************************************************************
bool FindSecondMostEnergeticTPCTrack_antiNueCC::Apply(AnaEventC& eventC, ToyBoxB& boxB) const{
//*********************************************************************
  
  AnaEventB& event = *static_cast<AnaEventB*>(&eventC); 
  
  // cast the box to the appropriate type
  ToyBoxNueCC& nuebox = *static_cast<ToyBoxNueCC*>(&boxB);

  nuebox.SecondMostEnergeticTPCTrack = nueCCUtils::FindSecondTPCTrack(event, *nuebox.MainTrack, static_cast<SubDetId::SubDetEnum>(nuebox.DetectorFV));

  return true;
}

//*********************************************************************
bool MostEnergeticTrackCut_antiNueCC::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//*********************************************************************

  (void) event;

  // cast the box to the appropriate type
  ToyBoxNueCC& nuebox = *static_cast<ToyBoxNueCC*>(&boxB);

  AnaTrackB* track = nuebox.MainTrack;  
  if(!track) return false;

  // Most energetic track must be positive
  if(track != nuebox.HMtrack) return false;

  if(!nuebox.SecondMostEnergeticTPCTrack) return true;

  if(nuebox.SecondMostEnergeticTPCTrack->Momentum > track->Momentum) return false;

  return true;
}

//**************************************************
bool SecondTPCPIDCut_antiNueCC::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************
  
  (void) event;

  // cast the box to the appropriate type
  ToyBoxNueCC& nuebox = *static_cast<ToyBoxNueCC*>(&boxB);

  AnaTrackB* track = nuebox.MainTrack;  
  if(!track) return false;

  return nueCutUtils::AntiNuSecondTPCPID(track, _protonregion_low, _protonregion_high, _pullelec_reject_min, _pullelec_reject_max);
}

//**************************************************
bool EcalEMEnergyCut_antiNueCC::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************
  
  (void) event;

  // cast the box to the appropriate type
  ToyBoxNueCC& nuebox = *static_cast<ToyBoxNueCC*>(&boxB);
  
  AnaTrackB* track = nuebox.MainTrack;
  if(!track) return false;

  return nueCutUtils::EcalEMEnergy(track, _Ethreshold, _Emin_ecal, _EoverP, true);
}


//**************************************************
bool EoverPCut_antiNueCC::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************

  AnaEventB& eventB = *static_cast<AnaEventB*>(&event);

  // cast the box to the appropriate type
  ToyBoxNueCC& box = *static_cast<ToyBoxNueCC*>(&boxB);

  AnaTrackB* track = box.MainTrack;
  if(!track) return false;

  // For FGD1 events only: If produced FGD2 shower activity then keep the event 
  if(nueCutUtils::FGD2ShowerActivityEcal(track, _protonregion_low, box.FGD2ShowerNFGD1TPC2Tracks, box.FGD2ShowerNFGD2TPC3Tracks))
    return true;
  
  Int_t realrun    = eventB.EventInfo.Run;
  Int_t realsubrun = eventB.EventInfo.SubRun;
  
  Int_t run = anaUtils::GetRunPeriod(realrun,realsubrun);

  if(run < 8 || run == 13)
    return nueCutUtils::EOverP(track, _protonregion_low, _eoverpfhc);
  else
    return nueCutUtils::EOverP(track, _protonregion_low, _eoverp);
}

//**************************************************
bool EcalEMHIPPIDCut_antiNueCC::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************

  AnaEventB& eventB = *static_cast<AnaEventB*>(&event);

  // cast the box to the appropriate type
  ToyBoxNueCC& box = *static_cast<ToyBoxNueCC*>(&boxB);

  AnaTrackB* track = box.MainTrack;
  if(!track) return false;

  // For FGD1 events only: If produced FGD2 shower activity then keep the event 
  if(nueCutUtils::FGD2ShowerActivityEcal(track, _protonregion_low, box.FGD2ShowerNFGD1TPC2Tracks, box.FGD2ShowerNFGD2TPC3Tracks))
    return true;

  Int_t realrun    = eventB.EventInfo.Run;
  Int_t realsubrun = eventB.EventInfo.SubRun;

  Int_t run        = anaUtils::GetRunPeriod(realrun,realsubrun);

  if(run < 8 || run == 13)
    return (nueCutUtils::EcalEMHIPPID(track, _protonregion_low, _emhipfhc));
  else
    return (nueCutUtils::EcalEMHIPPID(track, _protonregion_low, _emhip));
}

//**************************************************
bool FGDShowerCut_antiNueCC::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************

  (void) event;

  // cast the box to the appropriate type
  ToyBoxNueCC& box = *static_cast<ToyBoxNueCC*>(&boxB);

  AnaTrackB* track = box.MainTrack;
  if(!track) return false;

  return (nueCutUtils::FGD2Shower(track, _protonregion_low, _protonregion_high, box.FGD2ShowerNFGD1TPC2Tracks, box.FGD2ShowerNFGD2TPC3Tracks));
}

//**************************************************
bool ToFCut_antiNueCC::Apply(AnaEventC& eventC, ToyBoxB& boxB) const {
//**************************************************

  AnaEventB& eventB = *static_cast<AnaEventB*>(&eventC);

  // Cast the ToyBox to the appropriate type
  ToyBoxNueCC&  nuebox = *static_cast<ToyBoxNueCC*>(&boxB);

  AnaTrackB* track = nuebox.MainTrack;
  if(!track) return false;

  Int_t realrun    = eventB.EventInfo.Run;
  Int_t realsubrun = eventB.EventInfo.SubRun;

  Int_t run        = anaUtils::GetRunPeriod(realrun,realsubrun);

  return (nueCutUtils::AntiNuToF(*track, _fgdecaltof, _fgd1fgd2tof, _fgd1fgd2tof_relaxed, _protonregion_low, run));
}

//**************************************************
bool FGD2ProtonPID_antiNueCC::Apply(AnaEventC& eventC, ToyBoxB& boxB) const {
//**************************************************

  (void) eventC;

  // Cast the ToyBox to the appropriate type
  ToyBoxNueCC&  nuebox = *static_cast<ToyBoxNueCC*>(&boxB);

  AnaTrackB* track = nuebox.MainTrack;
  if(!track) return false;

  return (nueCutUtils::FGDProtonPull(track,  static_cast<SubDetId::SubDetEnum>(nuebox.DetectorFV), _fgd2protonpull));
}
