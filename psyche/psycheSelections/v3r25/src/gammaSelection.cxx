#include "gammaSelection.hxx"

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
gammaSelection::gammaSelection(bool forceBreak): SelectionBase(forceBreak, EventBoxId::kEventBoxTracker) {
//********************************************************************
  
  _useVetoes = (bool)(ND::params().GetParameterI("psycheSelections.gammaAnalysis.UseVetoes"));

  _nueCCSelection.Initialize();

  // Initialize systematic tuning parameters
  systTuning::Initialize();
}

//********************************************************************
void gammaSelection::DefineSteps(){
//********************************************************************

  // last "true" means the step sequence is broken if cut is not passed (default is "false")
  AddStep(StepBase::kCut,    "Event quality",                       new EventQualityCut(), true); // 0
  AddStep(StepBase::kCut,    "> 0 tracks ",                         new MultiplicityCut(), true); // 1
  AddStep(StepBase::kAction, "Find leading tracks",                 new FindLeadingTracksAction_gamma() );
  AddStep(StepBase::kAction, "Find vertex",                         new FillVertex() );
  AddStep(StepBase::kAction, "Fill summary",                        new FillSummaryActionGamma() );

  AddStep(StepBase::kCut,    "# TPC nodes 1ary track",              new PrimaryTrackQualityCut_gamma() ); // 2

  AddStep(StepBase::kCut,    "Electron PID",                        new PIDCut_gamma() ); // 3
  //AddStep(StepBase::kCut,    "Highest Momentum Track Momentum cut", new HighestMomentumCut() );
  AddStep(StepBase::kAction, "Find secondary tracks",               new FindPairsAction() );
  AddStep(StepBase::kCut,    "Secondary Track",                     new PairTrackCut_gamma() ); // 4
  AddStep(StepBase::kCut,    "Invariant Mass Cut",                  new MinvCut_gamma() ); // 5

  if(_useVetoes){
    // Upstream vetos. Reduce OOFV background.
    AddStep(StepBase::kAction,	"find TPC Veto track",			new FillTPCVetoTrackAction());
    AddStep(StepBase::kCut,	"TPC Veto",				new TPCVetoCut()); // 6

    AddStep(StepBase::kCut,     "External FGD1",                        new ExternalFGD1layersCut()); // 7
    
    AddStep(StepBase::kAction,	"find P0D veto track",			new FindP0DVetoTrackAction());
    AddStep(StepBase::kCut,	"P0D Veto",				new P0DVetoCut()); // 8
     
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

//********************************************************************
void gammaSelection::DefineDetectorFV(){
//********************************************************************
  
  // The detector in which the selection is applied
  SetDetectorFV(SubDetId::kFGD1);

  // Set the detector field into the selection being used
  _nueCCSelection.SetDetectorFV(SubDetId::kFGD1);

}

//********************************************************************
bool FillSummaryActionGamma::Apply(AnaEventC& eventC, ToyBoxB& boxB) const{
//********************************************************************

  ToyBoxNueCC& box = *static_cast<ToyBoxNueCC*>(&boxB);

  if(!box.MainTrack) return false;

  static_cast<AnaEventSummaryB*>(eventC.Summary)->LeptonCandidate[SampleId::kFGD1Gamma            ] = box.MainTrack;
  static_cast<AnaEventSummaryB*>(eventC.Summary)->LeptonCandidate[SampleId::kFGD1GammaInAntiNuMode] = box.MainTrack;

  for(int i = 0; i < 4; ++i){
    static_cast<AnaEventSummaryB*>(eventC.Summary)->VertexPosition[SampleId::kFGD1Gamma             ][i] = box.MainTrack->PositionStart[i];
    static_cast<AnaEventSummaryB*>(eventC.Summary)->VertexPosition[SampleId::kFGD1GammaInAntiNuMode][i] = box.MainTrack->PositionStart[i];
  }

  if(box.MainTrack->GetTrueParticle()){
    static_cast<AnaEventSummaryB*>(eventC.Summary)->TrueVertex[SampleId::kFGD1Gamma            ] = box.MainTrack->GetTrueParticle()->TrueVertex;
    static_cast<AnaEventSummaryB*>(eventC.Summary)->TrueVertex[SampleId::kFGD1GammaInAntiNuMode] = box.MainTrack->GetTrueParticle()->TrueVertex;
  }
  return true;

}

//********************************************************************
bool gammaSelection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
//********************************************************************

  AnaEventB& eventB = static_cast<AnaEventB&>(event);

  bool isRHC = anaUtils::IsRHC(eventB.EventInfo.Run);
  AnaEventSummaryB* eveSumB = static_cast<AnaEventSummaryB*>(event.Summary);

  if(allCutsPassed[0]){
    if(!isRHC)
      eveSumB->EventSample = SampleId::kFGD1Gamma;
    else
      eveSumB->EventSample = SampleId::kFGD1GammaInAntiNuMode;
  }
  
  return (eveSumB->EventSample != SampleId::kUnassigned);
}

//*********************************************************************
bool FindLeadingTracksAction_gamma::Apply(AnaEventC& eventC, ToyBoxB& boxB) const{
//*********************************************************************

  ToyBoxNueCC& box = *static_cast<ToyBoxNueCC*>(&boxB);

  trackerSelUtils::FindLeadingTracks(eventC, boxB);

  if(box.HMtrack) box.MainTrack = box.HMtrack;
  if(!box.MainTrack) return false;
  
  return true;
  
}

//*********************************************************************
bool PairTrackCut_gamma::Apply(AnaEventC& eventC, ToyBoxB& boxB) const{
//*********************************************************************

  (void) eventC;

  ToyBoxNueCC& nuebox   = *static_cast<ToyBoxNueCC*>(&boxB);
  bool accept = (nuebox.PairTrack);

  return accept;

}

//*********************************************************************
bool PrimaryTrackQualityCut_gamma::Apply(AnaEventC& eventC, ToyBoxB& boxB) const{
//*********************************************************************

  (void) eventC;

  ToyBoxNueCC& box = *static_cast<ToyBoxNueCC*>(&boxB);

  // No vertex - reject
  if(!box.Vertex) return false;

  AnaTrackB* track = box.MainTrack;
  if(!track) return false;

  return nueCutUtils::TrackQuality(track, 18, 18);
}

//*********************************************************************
bool MinvCut_gamma::Apply(AnaEventC& eventC, ToyBoxB& boxB) const{
//*********************************************************************

  (void) eventC;

  ToyBoxNueCC& nuebox   = *static_cast<ToyBoxNueCC*>(&boxB);

  if (!nuebox.MainTrack)
    return false;

  if (!nuebox.PairTrack)
    return false;

  return (nueCCUtils::GetInvMass(*nuebox.MainTrack, *nuebox.PairTrack) < _inv_mass_min);
  
}

//*********************************************************************
bool PIDCut_gamma::Apply(AnaEventC& eventC, ToyBoxB& boxB) const{
//*********************************************************************

  (void) eventC;
  ToyBoxNueCC& box = *static_cast<ToyBoxNueCC*>(&boxB);

  AnaTrackB* track_pri = box.MainTrack;
  //  AnaTrackB* track_sec = box.PairTrack;

  bool accept = nueCutUtils::TPCElectronPull(track_pri, _elepullpri_min, _elepullpri_max,
					     _elepullpri_min, _elepullpri_max, 0.0);
    
  return accept;

}



//*********************************************************************
void gammaSelection::InitializeEvent(AnaEventC& eventC){
//*********************************************************************
  
  _nueCCSelection.InitializeEvent(eventC);
}

//*********************************************************************
bool gammaSelection::CheckRedoSelection(const AnaEventC& eventC, const ToyBoxB& PreviousToyBoxB, Int_t& redoFromStep){
//*********************************************************************

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


//*********************************************************************
bool gammaSelection::IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* trackRecC, SystId_h systId, Int_t branch) const{
//*********************************************************************
  
  (void)event;
  (void)branch;

  AnaTrackB* track = static_cast<AnaTrackB*>(trackRecC);

  // True track should always exist
  if(!track->TrueObject) return false;

  if(systId == SystId::kTpcClusterEff){
    if(track->nTPCSegments == 0) return false;
    //    AnaTPCParticleB* tpcTrack = static_cast<AnaTPCParticleB*>(track->TPCSegments[0]);
    AnaTPCParticleB* tpcTrack = static_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInClosestTpc(*track));      
    if(!tpcTrack) return false;

    if(tpcTrack->NNodes == 17 || tpcTrack->NNodes == 18)
      return true;

    return false;
  }

  return true;
  
}


//*********************************************************************
bool gammaSelection::IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueTrack, SystId_h systId, Int_t branch) const{
//*********************************************************************

  return _nueCCSelection.IsRelevantTrueObjectForSystematic(event, trueTrack, systId, branch);

}


//*********************************************************************
bool gammaSelection::IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const{
//*********************************************************************

  (void)event;
  (void)branch;
  
  // cast the box to the appropriate type
  const ToyBoxNueCC &nuebox = *static_cast<const ToyBoxNueCC*>(&boxB);

  AnaTrackB* track = static_cast<AnaTrackB*>(recObj);

  if(systId == SystId::kChargeIDEff){
    if(!systTuning::APPLY_SYST_FINE_TUNING) return true;
    if (track != nuebox.MainTrack && track != nuebox.PairTrack) return false;
  }
  else if(systId == SystId::kTpcClusterEff){
    if(!systTuning::APPLY_SYST_FINE_TUNING) return true;
    if (track == nuebox.MainTrack) return true;
    if (track == nuebox.PairTrack) return true;

    return false;
  }

  return true;

}


//*********************************************************************
bool gammaSelection::IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
//*********************************************************************

  (void)event;
  (void)branch;

  // cast the box to the appropriate type
  const ToyBoxNueCC &box = *static_cast<const ToyBoxNueCC*>(&boxB);

  AnaTrueParticleB* truePart = static_cast<AnaTrueParticleB*>(trueObj);

  if(systId == SystId::kTpcTrackEff){
    if(!systTuning::APPLY_SYST_FINE_TUNING) return true;

    // Main track
    if(box.MainTrack->GetTrueParticle()){
      // At first order the inclusive selection only depends on the tracking efficiency of the electron candidate.
      if(truePart->ID  == box.MainTrack->GetTrueParticle()->ID) return true; 
      // Consider also the case in which the electron candidate is not a true electron but this track it is
      if(abs(truePart->PDG) == 11 && abs(box.MainTrack->GetTrueParticle()->PDG) != 11) return true; 
    }
    
    // Pair track
    if(box.PairTrack && box.PairTrack->GetTrueParticle()){
      if(truePart->ID  == box.PairTrack->GetTrueParticle()->ID) return true; 
      // Consider also the case in which the electron candidate is not a true electron but this track it is
      if(abs(truePart->PDG) == 11 && abs(box.PairTrack->GetTrueParticle()->PDG) != 11) return true; 
    }

    return false;
  } 
  else if(systId == SystId::kSIPion){
    if(!systTuning::APPLY_SYST_FINE_TUNING) return true;
    if(box.MainTrack->GetTrueParticle() && truePart->ID == box.MainTrack->GetTrueParticle()->ID){
      if(abs(box.MainTrack->GetTrueParticle()->PDG)        == 211) return true;
      if(abs(box.MainTrack->GetTrueParticle()->ParentPDG)  == 211) return true;
      if(abs(box.MainTrack->GetTrueParticle()->GParentPDG) == 211) return true;
    }
    // If this truePart is NOT associated to the MainTrack, consider the posibility of this truePart to become the MainTrack and be identified as electron
    // For the moment assume a negative pion may become the MainTrack if its momentum its above 90% of the current MainTrack momentum. 
    // Ideally we should check that this true pion is not associated to any reconstructed track, but this is not possible now without looping.
    // Multiply by the charge of the MainTrack such that it can be use for antiNumu selection
    //if (truePart->PDG == 211*((Int_t)box.MainTrack->Charge) && truePart->Momentum > 0.9*box.MainTrack->Momentum) return true;
 
    // Now check the paired track - it doesn't have to be the most energetic track
    if(box.PairTrack && box.PairTrack->GetTrueParticle() && truePart->ID == box.PairTrack->GetTrueParticle()->ID){
      if(abs(box.PairTrack->GetTrueParticle()->PDG)        == 211) return true;
      if(abs(box.PairTrack->GetTrueParticle()->ParentPDG)  == 211) return true;
      if(abs(box.PairTrack->GetTrueParticle()->GParentPDG) == 211) return true;
    }

    return false;
  }
  else if(systId == SystId::kSIProton){
    if(!systTuning::APPLY_SYST_FINE_TUNING) return true;
    // If this truePart is associated to the MainTrack
    if(box.MainTrack->GetTrueParticle() && truePart->ID == box.MainTrack->GetTrueParticle()->ID){
      if(box.MainTrack->GetTrueParticle()->PDG        == 2212) return true;
      if(box.MainTrack->GetTrueParticle()->ParentPDG  == 2212) return true;
      if(box.MainTrack->GetTrueParticle()->GParentPDG == 2212) return true;
    }
    // If this truePart is NOT associated to the MainTrack, consider the posibility of this truePart to become the MainTrack
    //if(truePart->PDG == 2212 && truePart->Momentum > 0.9*box.MainTrack->Momentum) return true;
 
    // Now check the paired track - it doesn't have to be the most energetic track
    if(box.PairTrack && box.PairTrack->GetTrueParticle() && truePart->ID == box.PairTrack->GetTrueParticle()->ID){
      if(box.PairTrack->GetTrueParticle()->PDG        == 2212) return true;
      if(box.PairTrack->GetTrueParticle()->ParentPDG  == 2212) return true;
      if(box.PairTrack->GetTrueParticle()->GParentPDG == 2212) return true;
    }
    
    return false;
  }

  return true;

}


//*********************************************************************
bool gammaSelection::IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const{
//*********************************************************************

  if     (systId == SystId::kTpcECalMatchEff)  // No ecal related syst
    return false;
  else if(systId == SystId::kECalPID)  // No ecal related syst
    return false;
  else if(systId == SystId::kECalEMResol)  // No ecal related syst
    return false;
  else if(systId == SystId::kECalEMScale)  // No ecal related syst
    return false;
  
  if(!_useVetoes){
    if     (systId == SystId::kNuETPCPileUp)  // No pile-up syst 
      return false;
    else if(systId == SystId::kNuEP0DPileUp)  // No pile-up syst 
      return false;
    else if(systId == SystId::kNuEECalPileUp)  // No pile-up syst
      return false;
  }
  // The rest are disabled here
  return _nueCCSelection.IsRelevantSystematic(event, box, systId, branch);
}

//**************************************************
Int_t gammaSelection::GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  std::vector<std::vector<Int_t> > groups(SystId::SystEnumLast_SystId, std::vector<Int_t>(2, -1));
  groups.resize(SystId::SystEnumLast_SystId);
   
  
  // Explicitely set the groups for various systeamtics that need them, 
  // this allows to have a transparent set/control of the systematics by the
  // selection  

  using namespace SystId;
  using namespace anaUtils;
  // --- Systematic         Detectors                                   FGD1                                    FGD2 
  groups[   kBFieldDist       ] = CreateVectorI( 2, EventBoxTracker::kTracksWithTPCInFGD1FV,  EventBoxTracker::kTracksWithTPCInFGD2FV );
  groups[   kMomScale         ] = CreateVectorI( 2, EventBoxTracker::kTracksWithTPCInFGD1FV,  EventBoxTracker::kTracksWithTPCInFGD2FV );
  groups[   kMomResol         ] = CreateVectorI( 2, EventBoxTracker::kTracksWithTPCInFGD1FV,  EventBoxTracker::kTracksWithTPCInFGD2FV );
  groups[   kTpcPid           ] = CreateVectorI( 2, EventBoxTracker::kTracksWithTPCInFGD1FV,  EventBoxTracker::kTracksWithTPCInFGD2FV );
  groups[   kChargeIDEff      ] = CreateVectorI( 2, EventBoxTracker::kTracksWithTPCInFGD1FV,  EventBoxTracker::kTracksWithTPCInFGD2FV );
  groups[   kTpcClusterEff    ] = CreateVectorI( 2, EventBoxTracker::kTracksWithTPCInFGD1FV,  EventBoxTracker::kTracksWithTPCInFGD2FV ); 
  groups[   kTpcTrackEff      ] = CreateVectorI( 2, EventBoxTracker::kTracksWithTPC,          EventBoxTracker::kTracksWithTPC         );
  
  if (versionUtils::prod6_systematics)
    groups[ kTpcFgdMatchEff   ] = CreateVectorI( 2, EventBoxTracker::kTracksWithTPCInFGD1FV,  EventBoxTracker::kTracksWithTPCInFGD2FV );
  else
    groups[ kTpcFgdMatchEff   ] = CreateVectorI( 2, EventBoxTracker::kTracksWithTPCorFGD1,    EventBoxTracker::kTracksWithTPCorFGD2   );
  
    // The systematics not mentioned above will get no groups
  Int_t ngroups = 0;
  // Check the systematic index is ok
  try {
    groups.at(systId);
  }
  catch (const std::out_of_range& oor) {
    std::cerr << this->Name() << " GetRelevantRecObjectGroupsForSystematic: syst index beyond limits "<< systId << " Error " << oor.what() << '\n';
    exit(1);
  }
  // One branch
  (void)branch;
  SubDetId_h det = GetDetectorFV();
  
  if (det == SubDetId::kFGD1){
    if (groups[systId][0] >= 0) IDs[ngroups++] = groups[systId][0];
  }
  else   if (det == SubDetId::kFGD2){
    if (groups[systId][1] >= 0) IDs[ngroups++] = groups[systId][1];
  }
  else if (det == SubDetId::kFGD){
    if (groups[systId][0] >= 0) IDs[ngroups++] = groups[systId][0];
    if (groups[systId][0] != groups[systId][1]){
      IDs[ngroups++] = groups[systId][1];
    }   
  }
  
  return ngroups;
}

//**************************************************
Int_t gammaSelection::GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  (void)branch;
  
  std::vector<std::vector<Int_t> > groups(SystId::SystEnumLast_SystId, std::vector<Int_t>(1, -1));
 
  // Explicitely set the groups for various systeamtics that need them, 
  // this allows to have a transparent set/control of the systematics by the
  // selection  

  using namespace SystId;
  using namespace anaUtils;
  // --- Systematic         Detectors                                   FGD                                
  groups[ kTpcFgdMatchEff   ] =  CreateVectorI(1, EventBoxTracker::kTrueParticlesChargedInTPCorFGDInBunch);
  groups[ kTpcTrackEff      ] =  CreateVectorI(1, EventBoxTracker::kTrueParticlesChargedInTPCInBunch);
  // The systematics not mentioned above will get no groups
  Int_t ngroups = 0;
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


