#include "nueCCSelection.hxx"

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
nueCCSelection::nueCCSelection(bool forceBreak): SelectionBase(forceBreak, EventBoxId::kEventBoxTracker) {
//********************************************************************

  _Ethreshold              = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.PID.EThreshold");

  // Initialize systematic tuning parameters
  systTuning::Initialize();
}

//********************************************************************
void nueCCSelection::DefineDetectorFV(){
//********************************************************************

  // The detector in which the selection is applied
  SetDetectorFV(SubDetId::kFGD1);
}

//********************************************************************
void nueCCSelection::DefineSteps(){
//********************************************************************

  // last "true" means the step sequence is broken if cut is not passed (default is "false")
  
  AddStep(StepBase::kCut,       "event quality",                        new EventQualityCut(), true); // 0
  AddStep(StepBase::kCut,       "> 0 tracks ",                          new MultiplicityCut(), true); // 1
  AddStep(StepBase::kAction,	"find leading tracks",			new FillLeadingTracksAction());     
  AddStep(StepBase::kAction,	"Fill vertex info",			new FillVertex());
  AddStep(StepBase::kAction,	"fill summary",				new FillSummaryAction_nueCC());  
  AddStep(StepBase::kCut,	"TPC Quality",				new TrackQualityCut(), true); // 2
  
  // e- PID cuts.	
  AddStep(StepBase::kCut,	"TPC Electron Pull",			new TPCElectronPullCut()); // 3
  AddStep(StepBase::kCut,	"TPC Muon Pull",			new TPCMuonPullCut()); // 4
  AddStep(StepBase::kCut,	"TPC Pion Pull",			new TPCPionPullCut()); // 5
  AddStep(StepBase::kCut,	"Ecal EMEnergy",			new EcalEMEnergyCut()); // 6
  AddStep(StepBase::kCut,	"Ecal MIPEM PID",			new EcalMIPEMPIDCut()); // 7
  AddStep(StepBase::kCut,	"TPC2 PID",				new SecondTPCPIDCut()); // 8

  // Minimum momentum
  AddStep(StepBase::kCut,	"Momentum",				new HighestMomentumCut()); // 9
  
  // Upstream vetos. Reduce OOFV background.
  AddStep(StepBase::kAction,	"find TPC Veto track",			new FillTPCVetoTrackAction());
  AddStep(StepBase::kCut,	"TPC Veto",				new TPCVetoCut()); // 10
  AddStep(StepBase::kCut,	"Gamma Iso Veto",			new GammaIsoVetoCut()); // 11
  AddStep(StepBase::kCut,       "External FGD1",                        new ExternalFGD1layersCut()); // 12

  AddStep(StepBase::kAction,	"find best e+e- pair",			new FindPairsAction());
  AddStep(StepBase::kCut,	"Pair Veto",				new PairCut()); // 13  

  AddStep(StepBase::kAction,	"find P0D veto track",			new FindP0DVetoTrackAction());
  AddStep(StepBase::kCut,	"P0D Veto",				new P0DVetoCut()); // 14
  
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
bool nueCCSelection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
//********************************************************************

  AnaEventB& eventB = static_cast<AnaEventB&>(event);
  AnaEventSummaryB* eveSumB = static_cast<AnaEventSummaryB*>(event.Summary);

  if(allCutsPassed[0]){
    if( !anaUtils::IsRHC(eventB.EventInfo.Run) )
      eveSumB->EventSample = SampleId::kFGD1NuECC;
    else
      eveSumB->EventSample = SampleId::kFGD1NuEBkgInAntiNuModeCC;
  }
  
  return (eveSumB->EventSample != SampleId::kUnassigned);

}


//********************************************************************
bool FillSummaryAction_nueCC::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//********************************************************************

  ToyBoxNueCC& nuebox = *static_cast<ToyBoxNueCC*>(&boxB);

  if(!nuebox.MainTrack) return false;
  
  static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD1NuECC               ] = nuebox.MainTrack;
  static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD1NuEBkgInAntiNuModeCC] = nuebox.MainTrack;
  for(int i = 0; i < 4; ++i){
    static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD1NuECC               ][i] = nuebox.MainTrack->PositionStart[i];
    static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD1NuEBkgInAntiNuModeCC][i] = nuebox.MainTrack->PositionStart[i];
  }
  if(nuebox.MainTrack->GetTrueParticle()){
    static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD1NuECC               ] = nuebox.MainTrack->GetTrueParticle()->TrueVertex;
    static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD1NuEBkgInAntiNuModeCC] = nuebox.MainTrack->GetTrueParticle()->TrueVertex;
  }
  
  return true;
}

//*********************************************************************
bool nueCCSelection::IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const{
//*********************************************************************

  (void)event;
  (void)branch;

  AnaTrackB* track = static_cast<AnaTrackB*>(recObj);

  // True track should always exist
  if(!track->TrueObject) return false;

  if(systId == SystId::kTpcClusterEff){
    if(track->nTPCSegments == 0) return false;
    AnaTPCParticleB* tpcTrack = static_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInClosestTpc(*track));      
    if(!tpcTrack) return false;

    // Different number of TPC nodes if the main track goes in the ECal
    if(nueCCUtils::UseEcal(track)  && (tpcTrack->NNodes == 17 || tpcTrack->NNodes == 18) ) return true;
    if(!nueCCUtils::UseEcal(track) && (tpcTrack->NNodes == 35 || tpcTrack->NNodes == 36) ) return true;

    return false;
  }

  return true;
}

//*********************************************************************
bool nueCCSelection::IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
//*********************************************************************

  (void)event;
  (void)branch;

  AnaTrueParticleB* truePart=static_cast<AnaTrueParticleB*>(trueObj);

  // TPC track eff
  if(systId == SystId::kTpcTrackEff){
    if(!anaUtils::InFiducialVolume(static_cast<SubDetId::SubDetEnum>(GetDetectorFV()), truePart->Position)) return false;
  }
  else if (systId == SystId::kFgdTrackEff){
    if (!anaUtils::InFiducialVolume(static_cast<SubDetId::SubDetEnum>(GetDetectorFV()), truePart->Position)) return false;
  }  
  else if (systId == SystId::kFgdHybridTrackEff){    
    if (!anaUtils::InFiducialVolume(static_cast<SubDetId::SubDetEnum>(GetDetectorFV()), truePart->Position)) return false;
  }

  return true;
}

//**************************************************
bool nueCCSelection::IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const{
//**************************************************

  (void)event;
  (void)branch;

  AnaTrackB* track = static_cast<AnaTrackB*>(recObj);

  // cast the box to the appropriate type
  const ToyBoxNueCC& box = *static_cast<const ToyBoxNueCC*>(&boxB);

  if (!box.MainTrack) return false;

  if(systId == SystId::kChargeIDEff){
    if(!systTuning::APPLY_SYST_FINE_TUNING) return true;

    // Inclusive selection only depends on the charge ID of the electron candidate 
    if (track == box.MainTrack) return true;

    // For now: also applied to another track if it is the same charge as the HM track and failed the invariant mass cut
    if(box.PairTrack_ForSyst && track == box.PairTrack_ForSyst) return true;

    return false;
  }
  else if(systId == SystId::kTpcFgdMatchEff){
    if(!systTuning::APPLY_SYST_FINE_TUNING) return true;

    // Special case for the GammaIsoCut
    // Assuming the likelihood to loose two or more tracks is negligible
    if(box.TPCVetoNearTracks < 2 && box.TPCVetoFarTracks == 2) return true;

    // Appied to main track
    if (track == box.MainTrack) return true;

    return false;
  }
  else if(systId == SystId::kTpcClusterEff){
    if(!systTuning::APPLY_SYST_FINE_TUNING) return true;

    // Applied to the main track
    if (track == box.MainTrack) return true;

    if (track->Charge == box.MainTrack->Charge && 
	cutUtils::FiducialCut(track->PositionStart, static_cast<SubDetId::SubDetEnum>(GetDetectorFV())) && 
	track->Momentum > box.MainTrack->Momentum)
        return true;

    return false;
  }
  else if(systId == SystId::kECalPID){
    // Only applied to main track
    if(track != box.MainTrack) return false;

    // Main track has to go in Ecal
    if(!nueCCUtils::UseEcal(track)) return false;

    // Ecal track must exist
    if(!track->ECALSegments[0]) return false;

    // Only applied to tracks with p < 1000 MeV/c or to tracks not fully contained in the Ecal
    if(!nueCCUtils::IsEcalContained(track->ECALSegments[0])) return true;

    // Select the TPC segment of good quality (true) closer to ECal
    // Require track quality to trust the momentum
    AnaTPCParticleB* tpcTrackBack = 
      static_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInClosestTpc(*track, track->PositionEnd, true));

    // TPC track must exist
    if(!tpcTrackBack) return false;
  
    // Minimum momentum requirement
    if(tpcTrackBack->Momentum > _Ethreshold)  return false;

    return true;
  }
 
  return true;
}

//**************************************************
bool nueCCSelection::IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
//**************************************************

  (void)event;
  (void)branch;

  // cast the box to the appropriate type
  const ToyBoxNueCC& box = *static_cast<const ToyBoxNueCC*>(&boxB);

  AnaTrueParticleB* truePart=static_cast<AnaTrueParticleB*>(trueObj);

  if (!box.MainTrack) return false;

  if(systId == SystId::kTpcTrackEff){
    if(!systTuning::APPLY_SYST_FINE_TUNING) return true;

    // Special case for the GammaIsoCut
    // Assuming the likelihood to loose two or more tracks is negligible
    if(box.TPCVetoNearTracks < 2 && box.TPCVetoFarTracks == 2) return true;

    // Main track
    if(box.MainTrack->GetTrueParticle()){
      // At first order the inclusive selection only depends on the tracking efficiency of the electron candidate.
      if(truePart->ID  == box.MainTrack->GetTrueParticle()->ID) return true; 
      // Consider also the case in which the electron candidate is not a true electron but this track it is
      if(abs(truePart->PDG) == 11 && abs(box.MainTrack->GetTrueParticle()->PDG) != 11) return true;

      // Check if should be applied to paired track (if any)
      // Only applied to electron track coming from the same gamma as the HM track
      if(abs(truePart->PDG) == 11 && truePart->ParentPDG == 22 && truePart->ParentID == box.MainTrack->GetTrueParticle()->ParentID) return true;
    }
    // Apply for all tracks starting in the same FGD as the main track
    else{
      if(anaUtils::InDetVolume(static_cast<SubDetId::SubDetEnum>(GetDetectorFV()), truePart->Position)) return true;
      //if (abs(truePart->PDG) == 11)  return true;
    }

    return false;
  } 
  else if(systId == SystId::kSIPion){
    if(!systTuning::APPLY_SYST_FINE_TUNING) return true;
    // If this truePart is associated to the MainTrack
    if(box.MainTrack->GetTrueParticle() && truePart->ID == box.MainTrack->GetTrueParticle()->ID){
      if(abs(box.MainTrack->GetTrueParticle()->PDG)        == 211) return true;
      if(abs(box.MainTrack->GetTrueParticle()->ParentPDG)  == 211) return true;
      if(abs(box.MainTrack->GetTrueParticle()->GParentPDG) == 211) return true;
      return false;
    }
    // If this truePart is NOT associated to the MainTrack, consider the posibility of this truePart to become the MainTrack and be identified as electron
    // For the moment assume a negative pion may become the MainTrack if its momentum its above 90% of the current MainTrack momentum. 
    // Ideally we should check that this true pion is not associated to any reconstructed track, but this is not possible now without looping.
    // Multiply by the charge of the MainTrack such that it can be use for antiNumu selection
    if (truePart->PDG == 211*((Int_t)box.MainTrack->Charge) && truePart->Momentum > 0.9*box.MainTrack->Momentum) return true;
    return false;
  }
  else if(systId == SystId::kSIProton){
    if(!systTuning::APPLY_SYST_FINE_TUNING) return true;
    // If this truePart is associated to the MainTrack
    if(box.MainTrack->GetTrueParticle() && truePart->ID == box.MainTrack->GetTrueParticle()->ID){
      if(box.MainTrack->GetTrueParticle()->PDG        == 2212) return true;
      if(box.MainTrack->GetTrueParticle()->ParentPDG  == 2212) return true;
      if(box.MainTrack->GetTrueParticle()->GParentPDG == 2212) return true;
      return false;
    }

    // If this truePart is NOT associated to the MainTrack, consider the posibility of this truePart to become the MainTrack
    // May be relevant for anti-neutrino case but much less here
    //if(truePart->PDG == 2212 && truePart->Momentum > 0.9*box.MainTrack->Momentum) return true;
    return false;
  }
  else if(systId == SystId::kTpcECalMatchEff){
    //if(!nueCCUtils::UseEcal(box.MainTrack)) return false;
    if(box.MainTrack->GetTrueParticle() && truePart->ID  == box.MainTrack->GetTrueParticle()->ID) return true; 

    return false;
  }

  return true;
}

//*********************************************************************
bool nueCCSelection::IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const{
//*********************************************************************

  (void)event;
  (void)branch;
  (void)box;

  // Enable systematics relevant to this analysis
  if(systId == SystId::kBFieldDist)      return true;
  if(systId == SystId::kMomScale)        return true;
  if(systId == SystId::kMomResol)        return true;
  if(systId == SystId::kTpcPid)          return true;
  if(systId == SystId::kChargeIDEff)     return true;
  if(systId == SystId::kTpcClusterEff)   return true;
  if(systId == SystId::kTpcTrackEff)     return true;
  if(systId == SystId::kTpcFgdMatchEff)  return true;
  if(systId == SystId::kTpcECalMatchEff) return true;
  if(systId == SystId::kFgdMass)         return true;
  if(systId == SystId::kSIPion)          return true;
  if(systId == SystId::kSIProton)        return true;
  if(systId == SystId::kSandMu)          return true;
  if(systId == SystId::kECalEMResol)     return true;
  if(systId == SystId::kECalEMScale)     return true;
  if(systId == SystId::kECalPID)         return true;
  if(systId == SystId::kToFResol)        return true;
  if(systId == SystId::kNuETPCPileUp)    return true;
  if(systId == SystId::kNuEP0DPileUp)    return true;
  if(systId == SystId::kNuEECalPileUp)   return true;
  if(systId == SystId::kNuEOOFV)         return true;
  if(systId == SystId::kFluxWeight)      return true;
  return false;
}

//**************************************************
Int_t nueCCSelection::GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
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
  groups[   kToFResol         ] = CreateVectorI( 2, EventBoxTracker::kTracksWithTPCInFGD1FV,  EventBoxTracker::kTracksWithTPCInFGD2FV );
  
  if (versionUtils::prod6_systematics)
    groups[ kTpcFgdMatchEff   ] = CreateVectorI( 2, EventBoxTracker::kTracksWithTPCInFGD1FV,  EventBoxTracker::kTracksWithTPCInFGD2FV );
  else
    groups[ kTpcFgdMatchEff   ] = CreateVectorI( 2, EventBoxTracker::kTracksWithTPCorFGD1,    EventBoxTracker::kTracksWithTPCorFGD2   );
  
  groups[   kTpcECalMatchEff  ] = CreateVectorI( 2, EventBoxTracker::kTracksWithTPCAndFGD1,   EventBoxTracker::kTracksWithTPCAndFGD2  );
  groups[   kECalEMResol      ] = CreateVectorI( 2, EventBoxTracker::kTracksWithECal,         EventBoxTracker::kTracksWithECal        );
  groups[   kECalEMScale      ] = CreateVectorI( 2, EventBoxTracker::kTracksWithECal,         EventBoxTracker::kTracksWithECal        );
  groups[   kECalPID          ] = CreateVectorI( 2, EventBoxTracker::kTracksWithECal,         EventBoxTracker::kTracksWithECal        );
 
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
Int_t nueCCSelection::GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
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
  groups[ kTpcECalMatchEff  ] =  CreateVectorI(1, EventBoxTracker::kTrueParticlesChargedInTPCInBunch);
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

//*********************************************************************
bool nueCCSelection::CheckRedoSelection(const AnaEventC& eventC, const ToyBoxB& PreviousToyBoxB, Int_t& redoFromStep){
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
void nueCCSelection::InitializeEvent(AnaEventC& eventC){
//*********************************************************************

  AnaEventB& event = *static_cast<AnaEventB*>(&eventC);  

  // Create the appropriate EventBox if it does not exist yet
  if(!event.EventBoxes[EventBoxId::kEventBoxTracker])
    event.EventBoxes[EventBoxId::kEventBoxTracker] = new EventBoxTracker();

  boxUtils::FillTracksWithTPC(event,             static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
  boxUtils::FillTracksWithFGD(event,             static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
  boxUtils::FillTracksWithECal(event);
  boxUtils::FillTrajsChargedInTPC(event);
  boxUtils::FillTrajsChargedInFGDAndNoTPC(event, static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
  boxUtils::FillTrajsInECal(event);
}

//**************************************************
bool MultiplicityCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

  (void)boxB;
  // Check we have at least one reconstructed track in the TPC
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];
  return (EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithTPC]>0);
}

//*********************************************************************
bool FillLeadingTracksAction::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//*********************************************************************
  
  (void)event;
  
  // cast the box to the appropriate type
  ToyBoxNueCC& nuebox = *static_cast<ToyBoxNueCC*>(&boxB);
  
  //trackerSelUtils::FindLeadingTracksOld(event, box, false, det_FGD, true);
  trackerSelUtils::FindLeadingTracks(event,boxB);

  if(nuebox.HMNtrack) nuebox.MainTrack = nuebox.HMNtrack;  
  if(!nuebox.MainTrack) return false;
 
  return true;
}

//**************************************************
bool FillVertex::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************
  
  (void)event;
  
  // cast the box to the appropriate type
  ToyBoxNueCC& box = *static_cast<ToyBoxNueCC*>(&boxB);

  // reset the vertex
  if(box.Vertex) delete box.Vertex;
  box.Vertex = NULL;

  // also the true vertex
  box.TrueVertex = NULL;

  if(!box.MainTrack) return false;
  
  box.Vertex = new AnaVertexB();
  anaUtils::CreateArray(box.Vertex->Particles, 1);
  
  box.Vertex->nParticles = 0;
  box.Vertex->Particles[box.Vertex->nParticles++] = box.MainTrack;
  
  for(Int_t i = 0; i < 4; ++i){
    box.Vertex->Position[i] = box.MainTrack->PositionStart[i];
  }

  if(box.MainTrack->GetTrueParticle())
    box.TrueVertex = box.Vertex->TrueVertex = box.MainTrack->GetTrueParticle()->TrueVertex;
  
  return true;
}

//*********************************************************************
bool FindP0DVetoTrackAction::Apply(AnaEventC& eventC, ToyBoxB& boxB) const{
//*********************************************************************

  AnaEventB& event = *static_cast<AnaEventB*>(&eventC); 

  // cast the box to the appropriate type
  ToyBoxNueCC& nuebox = *static_cast<ToyBoxNueCC*>(&boxB);

  int nTrack[3];
  nueCCUtils::FindP0DVetoTracks(event, *nuebox.MainTrack, nTrack);
  
  nuebox.P0DVetoP0DTracks = nTrack[0];
  nuebox.P0DVetoP0DEcalTracks = nTrack[1];
  nuebox.P0DVetoFGD1Tracks = nTrack[2];

  return true;
}

//**************************************************
bool HighestMomentumCut::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************

  (void) event;

  // cast the box to the appropriate type
  ToyBoxNueCC& box = *static_cast<ToyBoxNueCC*>(&boxB);

  AnaTrackB* track = box.MainTrack;
  if(!track) return false;

  // Criteria to remove OOFGD electrons
  
  // Only at low momenta
  if(track->Momentum > _min_mom) return true;

  if(static_cast<SubDetId::SubDetEnum>(box.DetectorFV) == SubDetId::kFGD1){
    // Track must go in ECal or FGD2 to get the ToF
    if(!track->ToF.Flag_FGD1_FGD2 && !track->ToF.Flag_ECal_FGD1) return false;
    
    // FGD1-FGD2 ToF. FGD-ECal will be applied later
    if(track->ToF.FGD1_FGD2 > 0.0) return false;
  }
  else if(static_cast<SubDetId::SubDetEnum>(box.DetectorFV) == SubDetId::kFGD2){
    // Track must go in ECal to get the ToF
    if(!track->ToF.Flag_ECal_FGD2 && !track->ToF.Flag_DSECal_FGD2) return false;
  }

  return true;
 
}

//**************************************************
bool TrackQualityCut::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************
  
  (void) event;

  // cast the box to the appropriate type
  ToyBoxNueCC& box = *static_cast<ToyBoxNueCC*>(&boxB);

  // No vertex - reject
  if(!box.Vertex) return false;

  AnaTrackB* track = box.MainTrack;
  if(!track) return false;

  return nueCutUtils::TrackQuality(track, 36, 18);
  //                                 no Ecal, Ecal 
}

//**************************************************
bool TPCElectronPullCut::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************
  
  (void) event;

  // cast the box to the appropriate type
  ToyBoxNueCC& box = *static_cast<ToyBoxNueCC*>(&boxB);
 
  AnaTrackB* track = box.MainTrack;
  if(!track || track->nTPCSegments == 0) return false;

  return nueCutUtils::TPCElectronPull(track, _pullel_accept_min, _pullel_accept_max, _pullel_accept_tight_min, _pullel_accept_tight_max, _minMom_ecal);
}

//**************************************************
bool TPCMuonPullCut::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************
  
  (void) event;

  // cast the box to the appropriate type
  ToyBoxNueCC& box = *static_cast<ToyBoxNueCC*>(&boxB);

  AnaTrackB* track = box.MainTrack; 
  if(!track) return false;

  return nueCutUtils::TPCMuonPull(track, _pullmu_reject_min, _pullmu_reject_max, _minMom_ecal);
}	

//**************************************************
bool TPCPionPullCut::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************
  
  (void) event;

  // cast the box to the appropriate type
  ToyBoxNueCC& box = *static_cast<ToyBoxNueCC*>(&boxB);

  AnaTrackB* track = box.MainTrack;
  if(!track) return false;

  return nueCutUtils::TPCPionPull(track, _pullpi_reject_min, _pullpi_reject_max, 36, _minMom_ecal);
  // 36 -> TPC Nodes
}

//**************************************************
bool EcalEMEnergyCut::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************
  
  (void) event;

  // cast the box to the appropriate type
  ToyBoxNueCC& nuebox = *static_cast<ToyBoxNueCC*>(&boxB);
  
  AnaTrackB* track = nuebox.MainTrack;
  if(!track) return false;

  return nueCutUtils::EcalEMEnergy(track, _Ethreshold, _Emin_ecal, _EoverP, false);
}

//**************************************************
bool EcalMIPEMPIDCut::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************
  
  (void) event;

  // cast the box to the appropriate type
  ToyBoxNueCC& nuebox = *static_cast<ToyBoxNueCC*>(&boxB);
 
  AnaTrackB* track = nuebox.MainTrack; 
  if(!track) return false;

  return nueCutUtils::EcalMIPEMPID(track, _Ethreshold, _ecal_mipem_min);
}

//**************************************************
bool SecondTPCPIDCut::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************
  
  (void) event;

  // cast the box to the appropriate type
  ToyBoxNueCC& nuebox = *static_cast<ToyBoxNueCC*>(&boxB);

  AnaTrackB* track = nuebox.MainTrack;  
  if(!track) return false;

  return nueCutUtils::SecondTPCPID(track, _pullmu_reject_min, _pullmu_reject_max);
}

//*********************************************************************
bool FillTPCVetoTrackAction::Apply(AnaEventC& eventC, ToyBoxB& boxB) const{
//*********************************************************************
  
  AnaEventB& event = *static_cast<AnaEventB*>(&eventC);

  // cast the box to the appropriate type
  ToyBoxNueCC&  nuebox = *static_cast<ToyBoxNueCC*>(&boxB);

  AnaTrackB* track = nuebox.MainTrack;
  if(!track) return false;
  
  nuebox.VetoTrack = nueCCUtils::FindTPCVetoTrack(event, *track);
  nuebox.SecondMostEnergeticFGDTPCTrack = nueCCUtils::FindFGDTPCVetoTrack(event, *track, static_cast<SubDetId::SubDetEnum>(nuebox.DetectorFV) );
 
  /// Gamma isolation veto
  Int_t nTracks[3];
  nueCCUtils::FindGammaIsoTracks(event, *track, static_cast<SubDetId::SubDetEnum>(nuebox.DetectorFV), _vertex_activity, nTracks);
  // Find activity near and far from the vertex
  nuebox.TPCVetoNearTracks = nTracks[1];
  nuebox.TPCVetoFarTracks  = nTracks[0];
  // Tracks at vertex going in the P0D
  nuebox.TPCVetoP0DNearTracks = nTracks[2];

  // Tracks starting in the first two layers for FGD1 or in the first layer for FGD2 
  nuebox.NOOFVTracks = nueCCUtils::FindOOFVFGDTPCTracks(event, *track, static_cast<SubDetId::SubDetEnum>(nuebox.DetectorFV) );

  // FGD OOFV track
  nuebox.OOFVtrack = cutUtils::FindFGDOOFVtrack(event, *track, static_cast<SubDetId::SubDetEnum>(nuebox.DetectorFV));

  // FGD2 Shower Tracks
  Int_t nSTracks[2];
  nueCCUtils::FindFGD2ShowerTracks(event, *track, nSTracks);

  nuebox.FGD2ShowerNFGD1TPC2Tracks = nSTracks[0];
  nuebox.FGD2ShowerNFGD2TPC3Tracks = nSTracks[1];

  return true;
}

//**************************************************
bool TPCVetoCut::Apply(AnaEventC& event, ToyBoxB& boxB) const {
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

//**************************************************
bool GammaIsoVetoCut::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************
  
  (void) event;

  // cast the box to the appropriate type
  ToyBoxNueCC&  nuebox = *static_cast<ToyBoxNueCC*>(&boxB);

  // Remove single track events near the vertex, accompanied by a number of tracks far from the vertex
  if(nuebox.TPCVetoNearTracks < 1 && nuebox.TPCVetoFarTracks > 1)
    return false;

  return true;
}

//**************************************************
bool FindPairsAction::Apply(AnaEventC& eventC, ToyBoxB& boxB) const {
//**************************************************

  AnaEventB& event = *static_cast<AnaEventB*>(&eventC); 
  
  // Cast the ToyBox to the appropriate type
  ToyBoxNueCC&  nuebox = *static_cast<ToyBoxNueCC*>(&boxB);

  AnaTrackB* track = nuebox.MainTrack;
  if(!track) return false;
  
  // Find the best pair track candidate
  nuebox.PairTrack = nueCCUtils::FindPairTrack(event, *track, _delta_pos_max, _pullel_min, _pullel_max);

  // Pair track with the same charge as the HM track; only used for systematics
  AnaTrackB* temp_track = nueCCUtils::FindPairTrack(event, *track, _delta_pos_max, _pullel_min, _pullel_max, true);
  if(temp_track){
    if(nueCutUtils::PairCut(*track, temp_track, 110.0))
      nuebox.PairTrack_ForSyst = temp_track;
  }
  
  return true;
}

//**************************************************
bool PairCut::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************
  
  (void) event;
  
  // Cast the ToyBox to the appropriate type
  ToyBoxNueCC&  nuebox = *static_cast<ToyBoxNueCC*>(&boxB);

  AnaTrackB* track = nuebox.MainTrack;
  if(!track) return false;

  AnaTrackB* pair = nuebox.PairTrack;
  if(!pair) return true;

  return (nueCutUtils::PairCut(*track, pair, _inv_mass_min));
}

//**************************************************
bool P0DVetoCut::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************
  
  (void) event;
  
  // Cast the ToyBox to the appropriate type
  ToyBoxNueCC&  nuebox = *static_cast<ToyBoxNueCC*>(&boxB);

  if(nuebox.P0DVetoP0DTracks > 0 || nuebox.P0DVetoP0DEcalTracks > 0) return false;
  
  return true;
}

//*********************************************************************
bool FindECalVetoTrackAction::Apply(AnaEventC& eventC, ToyBoxB& boxB) const{
//*********************************************************************

  AnaEventB& event = *static_cast<AnaEventB*>(&eventC); 

  // Cast the ToyBox to the appropriate type
  ToyBoxNueCC&  nuebox = *static_cast<ToyBoxNueCC*>(&boxB);

  nuebox.ECalVetoTrack = nueCCUtils::FindEcalVetoTrack(event, *(nuebox.MainTrack));
  nuebox.ECalNCVetoTrack = nueCCUtils::FindEcalVetoTrack(event, *(nuebox.MainTrack), true);
  return true;
}

//**************************************************
bool ECalVetoCut::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************
  
  (void) event;
  
  // Cast the ToyBox to the appropriate type
  ToyBoxNueCC&  nuebox = *static_cast<ToyBoxNueCC*>(&boxB);

  AnaTrackB* track = nuebox.MainTrack;
  if(!track) return false;

  AnaTrackB* veto = nuebox.ECalVetoTrack;
  if(!veto) return true;

  return (nueCutUtils::EcalVeto(*track, veto, _delta_z_min));
  
}

//**************************************************
bool ToFCut::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************

  AnaEventB& eventB = *static_cast<AnaEventB*>(&event);

  // Cast the ToyBox to the appropriate type
  ToyBoxNueCC&  nuebox = *static_cast<ToyBoxNueCC*>(&boxB);

  AnaTrackB* track = nuebox.MainTrack;
  if(!track) return false;

  Int_t realrun    = eventB.EventInfo.Run;
  Int_t realsubrun = eventB.EventInfo.SubRun;

  Int_t run        = anaUtils::GetRunPeriod(realrun,realsubrun);

  return(nueCutUtils::ToF(*track, _fgdecaltof, run));
}

//********************************************************************
bool ExternalFGD1layersCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//********************************************************************

    (void)event;
    // Cast the ToyBox to the appropriate type
    ToyBoxNueCC&  nuebox = *static_cast<ToyBoxNueCC*>(&boxB);

    if(!nuebox.MainTrack) return false;

    // Reject external background from the first 2 layers of FGD1
    if(nuebox.NOOFVTracks > 0) return false;
    
    // Reject external background from the last 2 layers of FGD1
    if(!nuebox.OOFVtrack) return true;

    if(nuebox.MainTrack->PositionStart[2] > 425.) return false;

    return true;
}

//**************************************************
bool MomentumQualityCut::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************

  (void) event;
  
  // cast the box to the appropriate type
  ToyBoxNueCC& nuebox = *static_cast<ToyBoxNueCC*>(&boxB);

  AnaTrackB* track = nuebox.MainTrack;
  if(!track) return false;

  // Only applied to specific momentum range
  if(track->Momentum < 200.0) return true;

  AnaTPCParticleB* longCloseTPC = static_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInClosestTpc(*track));
  if(!longCloseTPC) return false;

  Float_t pullmuon = (longCloseTPC->dEdxMeas-longCloseTPC->dEdxexpMuon)/longCloseTPC->dEdxSigmaMuon;
  Float_t pullpion = (longCloseTPC->dEdxMeas-longCloseTPC->dEdxexpPion)/longCloseTPC->dEdxSigmaPion;

  if(pullmuon < 0.0 || pullpion < 0.0) return false;

  return true;
}
