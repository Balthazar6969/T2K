#include "numuCCSelection.hxx"
#include "baseSelection.hxx"
#include "SystematicTuning.hxx"
#include "CutUtils.hxx"
#include "EventBoxUtils.hxx"
#include "VersioningUtils.hxx"
#include "SystematicUtils.hxx"
#include "trackerSelectionUtils.hxx"


/*
    \\! [numuCCSelection_info]

    <table>
    <tr><th> #cut  <th> cut class                        <th> defined in file     <th> Cut description
    <tr><td> 1     <td> EventQualityCut::Apply           <td> baseSelection.cxx   <td> Event quality: good detector and beam quality flags 
    <tr><td> 2     <td> TotalMultiplicityCut::Apply      <td> numuCCSelection.cxx <td> Total multiplicity cut: at least one track with TPC segments 
    <tr><td> 3     <td> FindLeadingTracksAction::Apply,   
                        FindVertexAction::Apply,                                                       
                        TrackQualityFiducialCut::Apply   <td> numuCCSelection.cxx <td> Select muon candidate as the highest momentum negative (HMN) 
                                                                                       track with more than 18 clusters in the TPC and initial position in the FGD FV 
    <tr><td> 4     <td> ExternalVetoCut::Apply           <td> numuCCSelection.cxx <td> External veto cut 
    <tr><td> 5     <td> ExternalFGD1lastlayersCut::Apply <td> numuCCSelection.cxx <td> External FGD1 last layers cut
    <tr><td> 6     <td> MuonPIDCut::Apply                <td> numuCCSelection.cxx <td> Muon PID cut
    </table>

    \\! [numuCCSelection_info]
*/



//********************************************************************
numuCCSelection::numuCCSelection(bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxTracker) {
//********************************************************************

  // Initialize systematic tuning parameters
  systTuning::Initialize();
}

///
/// This is the method where the actual steps are added to the selection
///
//********************************************************************
void numuCCSelection::DefineSteps(){
//********************************************************************
  
    // Cuts must be added in the right order
    // last "true" means the step sequence is broken if cut is not passed (default is "false")
    AddStep(StepBase::kCut,    "event quality",      new EventQualityCut(),           true);
    AddStep(StepBase::kCut,    "> 0 tracks ",        new TotalMultiplicityCut(),      true);  
    AddStep(StepBase::kAction, "find leading tracks",new FindLeadingTracksAction());  
    AddStep(StepBase::kAction, "find vertex",        new FindVertexAction());  
    AddStep(StepBase::kAction, "fill_summary",       new FillSummaryAction_numuCC());
    AddStep(StepBase::kCut,    "quality+fiducial",   new TrackQualityFiducialCut(),   true);  
    AddStep(StepBase::kAction, "find veto track",    new FindVetoTrackAction());
    AddStep(StepBase::kCut,    "veto",               new ExternalVetoCut());
    AddStep(StepBase::kAction, "find oofv track",    new FindOOFVTrackAction());
    AddStep(StepBase::kCut,    "External FGD1",      new ExternalFGD1lastlayersCut());
    AddStep(StepBase::kCut,    "muon PID",           new MuonPIDCut());

    SetBranchAlias(0,"trunk");

    // By default the preselection correspond to cuts 0-2. 
    // It means that if any of the three first cuts (0,1,2) is not passed 
    // the loop over toys will be broken ===> A single toy will be run
    SetPreSelectionAccumLevel(2);

    // Step and Cut numbers needed by CheckRedoSelection
    _MuonPIDCutIndex            = GetCutNumber("muon PID");
    _MuonPIDStepIndex           = GetStepNumber("muon PID");
    _FindLeadingTracksStepIndex = GetStepNumber("find leading tracks");
    _TotalMultiplicityCutIndex  = GetCutNumber("> 0 tracks ");
}

//********************************************************************
void numuCCSelection::DefineDetectorFV(){
//********************************************************************

    /// The detector in which the selection is applied
    SetDetectorFV(SubDetId::kFGD1);
}

//********************************************************************
bool numuCCSelection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
//********************************************************************

    if(allCutsPassed[0]){
        static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD1NuMuCC;
    }
    return (static_cast<AnaEventSummaryB*>(event.Summary)->EventSample != SampleId::kUnassigned);
}

///
/// Total multiplicity cut
///
//**************************************************
bool TotalMultiplicityCut::Apply(AnaEventC& event, ToyBoxB& box) const{
//**************************************************

  (void)box;
  // Check we have at least one reconstructed track in the TPC
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];
  return (EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithTPC]>0);
}

//**************************************************
bool TrackQualityFiducialCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

  (void)event;

  // Cast the ToyBox to the appropriate type
  ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

  return (box.Vertex);
}

//**************************************************
bool MuonPIDCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

    (void)event;

    // Cast the ToyBox to the appropriate type
    ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

    if (!box.MainTrack) return false;
    if (box.MainTrack->Momentum < 0.) return false;
    return cutUtils::MuonPIDCut(*(box.MainTrack), _prod5Cut);
}

//********************************************************************
bool ExternalVetoCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//********************************************************************

    (void)event;
    // Cast the ToyBox to the appropriate type
    ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

    if (!box.MainTrack) return false;
    return cutUtils::ExternalVetoCut(*(box.MainTrack),box.VetoTrack);
}

//********************************************************************
bool ExternalFGD1lastlayersCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//********************************************************************

    (void)event;
    // Cast the ToyBox to the appropriate type
    ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

    /// Reject external background from the last 2 layers of FGD1
    if (!box.MainTrack) return false;  
    if (!box.OOFVtrack) return true;
    return (box.MainTrack->PositionStart[2]<425.);
}

//**************************************************
bool FindVertexAction::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

    (void)event;

    // Cast the ToyBox to the appropriate type
    ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

    // reset the vertex 
    if (box.Vertex) delete box.Vertex;
    box.Vertex = NULL;

    // also the true vertex
    box.TrueVertex = NULL;

    if (!box.MainTrack) return false;

    box.Vertex = new AnaVertexB();
    anaUtils::CreateArray(box.Vertex->Particles, 1);

    box.Vertex->nParticles = 0;
    box.Vertex->Particles[box.Vertex->nParticles++] = box.MainTrack;

    for(int i = 0; i < 4; ++i){
        box.Vertex->Position[i] = box.MainTrack->PositionStart[i];
    }
    if(  box.MainTrack->GetTrueParticle() ){ 
      box.TrueVertex = box.Vertex->TrueVertex = box.MainTrack->GetTrueParticle()->TrueVertex;
    }
    return true;
}

//********************************************************************
bool FillSummaryAction_numuCC::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//********************************************************************

    // Cast the ToyBox to the appropriate type
    ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

    if(!box.HMNtrack) return 1;

    static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD1NuMuCC] = box.HMNtrack;
    for(int i = 0; i < 4; ++i){
        static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD1NuMuCC][i] = box.HMNtrack->PositionStart[i];
    }
    if(box.HMNtrack->GetTrueParticle()) static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD1NuMuCC] = box.HMNtrack->GetTrueParticle()->TrueVertex;

    return true;
}

//**************************************************
bool FindLeadingTracksAction::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

    // Cast the ToyBox to the appropriate type
    ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 


    // Find leading tracks with good quality and only in FGD FV
    trackerSelUtils::FindLeadingTracks(event,box);

    // For this selection the main track is the HMN track
    box.MainTrack = box.HMNtrack;
    return true;
}

//**************************************************
bool FindVetoTrackAction::Apply(AnaEventC& eventBB, ToyBoxB& boxB) const{
//**************************************************

    AnaEventB& event = *static_cast<AnaEventB*>(&eventBB); 

    // Cast the ToyBox to the appropriate type
    ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

    box.VetoTrack = cutUtils::FindVetoTrack(event, *(box.MainTrack));  
    return true;
}

//**************************************************
bool FindOOFVTrackAction::Apply(AnaEventC& eventBB, ToyBoxB& boxB) const{
//**************************************************

    AnaEventB& event = *static_cast<AnaEventB*>(&eventBB); 

    // Cast the ToyBox to the appropriate type
    ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

    box.OOFVtrack = cutUtils::FindFGDOOFVtrack(event, *(box.MainTrack), static_cast<SubDetId::SubDetEnum>(box.DetectorFV));
    return true;
}

//**************************************************
bool numuCCSelection::IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const{
//**************************************************
  (void)event;
  (void)branch;

  AnaTrackB* track = static_cast<AnaTrackB*>(recObj);
 
  // True track should always exist
  if (!track->TrueObject) return false;

  if (systId == SystId::kFgdPid){
    if (!anaUtils::TrackUsesOnlyDet(*track,static_cast<SubDetId::SubDetEnum>(GetDetectorFV()))) return false;
  }
  else if(systId == SystId::kTpcClusterEff){
    if (track->nTPCSegments == 0) return false;
    //    AnaTPCParticleB* tpcTrack = static_cast<AnaTPCParticleB*>(track->TPCSegments[0]);
    AnaTPCParticleB* tpcTrack = static_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInClosestTpc(*track));      
    if (!tpcTrack) return false;
    // use 17 and 18 for numu analysis, 35 and 36 for nue analyses
    if (tpcTrack->NNodes == 17 || tpcTrack->NNodes == 18)
      return true;  
    return false;
  } 
  return true;
}

//**************************************************
bool numuCCSelection::IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
//**************************************************

  (void)event;
  (void)branch;

  AnaTrueParticleB* trueTrack = static_cast<AnaTrueParticleB*>(trueObj);

  // TPC track eff
  if (systId == SystId::kTpcTrackEff){
    if (!anaUtils::InFiducialVolume(static_cast<SubDetId::SubDetEnum>(GetDetectorFV()), trueTrack->Position)) return false;
  }  
  else if (systId == SystId::kFgdTrackEff){
    if (!anaUtils::InFiducialVolume(static_cast<SubDetId::SubDetEnum>(GetDetectorFV()), trueTrack->Position)) return false;
  }  
  else if (systId == SystId::kFgdHybridTrackEff){    
    if (!anaUtils::InFiducialVolume(static_cast<SubDetId::SubDetEnum>(GetDetectorFV()), trueTrack->Position)) return false;
  }

  return true;
}

//**************************************************
bool numuCCSelection::IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaRecObjectC* track, SystId_h systId, Int_t branch) const{
//**************************************************

  (void)event;
  (void)branch;
  

  // Cast the ToyBox to the appropriate type
  const ToyBoxTracker& box = *static_cast<const ToyBoxTracker*>(&boxB); 
  AnaTrackB* trackb = static_cast<AnaTrackB*>(track);

  // All are relevant if no main track concept applied  
  if (!systTuning::APPLY_SYST_FINE_TUNING) return true;
 
  if (!box.MainTrack) return false;
  
  switch (systId){
    // Fall through the relevant ones
    // TPC reco
    case SystId::kChargeIDEff:
    // TPC matching
    case SystId::kTpcFgdMatchEff:
      if (track == box.MainTrack) return true;
      return false;
      break;
    // TPC cluster,  consider  a potential HMN track,  can affect the selection
    case SystId::kTpcClusterEff:
      // This cannot happen, so for consistency
      if (track == box.MainTrack) return true;
      if (trackb->Charge == box.MainTrack->Charge && 
          cutUtils::FiducialCut(trackb->PositionStart, static_cast<SubDetId::SubDetEnum>(GetDetectorFV())) && 
          trackb->Momentum > box.MainTrack->Momentum)
        return true;
      return false;
     break;
    
    // So to use for other selection when e.g. the systematic is not "known" for numu CC
    default:
     return true;
     break;
  }

  return true;
}

//**************************************************
bool numuCCSelection::IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
//**************************************************

  (void)event;
  (void)branch;

  // Cast the ToyBox to the appropriate type
  const ToyBoxTracker& box = *static_cast<const ToyBoxTracker*>(&boxB); 


  AnaTrueParticleB* trueTrack = static_cast<AnaTrueParticleB*>(trueObj);
  
  if (!trueTrack)
    return false;
   
  // Apply to all objects,  not fine-tuning
  if (!systTuning::APPLY_SYST_FINE_TUNING) return true;
 
  // Main track "mode",  will only consider certain true tracks of interest

  if(systId == SystId::kTpcTrackEff){
    
    if (box.MainTrack->GetTrueParticle() && trueTrack->ID  == box.MainTrack->GetTrueParticle()->ID){
      return true; 
    }
    // Consider true muon
    if (trueTrack->PDG == 13){  
      return true;
    }
    
    return false;
  } 
  else if(systId == SystId::kSIPion){
    
    // If this trueTrack is associated to the MainTrack or its one of its
    // ancestors
    if (anaUtils::CheckTrueCausesRecoSimple(*trueTrack, *box.MainTrack)){
      return true;
    }
    
    // If this trueTrack is NOT associated to the MainTrack, consider the posibility of this trueTrack to become the MainTrack and be identified as muon
    // For the moment assume a negative pion may become the MainTrack if its momentum its above 90% of the current MainTrack momentum. 
    // Ideally we should check that this true pion is not associated to any reconstructed track, but this is not possible now without looping.
    // Multiply by the charge of the MainTrack such that it can be use for antiNumu selection
    if (trueTrack->PDG == 211*((Int_t)box.MainTrack->Charge) && trueTrack->Momentum > 0.9*box.MainTrack->Momentum) return true;
    
    return false;
  }
  else if(systId == SystId::kSIProton){
    
    // If this trueTrack is associated to the MainTrack or its one of its
    // ancestors
    if (anaUtils::CheckTrueCausesRecoSimple(*trueTrack, *box.MainTrack)){
      return true;    
    }
        
    // If this trueTrack is NOT associated to the MainTrack, consider the posibility of this trueTrack to become the MainTrack
    
    // May be relevant for anti-neutrino case but much less here
    // if (trueTrack->PDG == 2212 && trueTrack->Momentum > 0.9*box.MainTrack->Momentum) return true;
    return false;
  }

  return true;
}

//**************************************************
bool numuCCSelection::IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const{
//**************************************************

  (void)event;
  (void)branch;
  (void)box;

  
  // Direct list of (Efficiency/Weight-like) systematics that a relevant instead of tagging the ones which are not, 
  // the latter may require updating the file with more systematics added which is not desirable
  // (although indeed the list of systematics can be controlled externally when adding to manager,
  // it is better to have a consistent treatment already here)
  // the selection is mature and the list of systematics is "well-defined" 
  switch (systId){
    // Fall through the relevant ones
    // TPC reco
    case SystId::kChargeIDEff:
    case SystId::kTpcClusterEff:
    case SystId::kTpcTrackEff:
    // TPC matching
    case SystId::kTpcFgdMatchEff:
    // Normalization
    case SystId::kPileUp:
    case SystId::kFgdMass:
    case SystId::kOOFV:
    case SystId::kSandMu:
    // Flux
    case SystId::kFluxWeight:
    // SI
    case SystId::kSIPion:
    case SystId::kSIProton:
      return true;
      break;
    default:
     return false;
     break;
  }
  
  return false;
}


//**************************************************
Int_t numuCCSelection::GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  std::vector<std::vector<Int_t> > groups(SystId::SystEnumLast_SystId, std::vector<Int_t>(2, -1));
  groups.resize(SystId::SystEnumLast_SystId);
   
  
  // Explicitely set the groups for various systeamtics that need them, 
  // this allows to have a transparent set/control of the systematics by the
  // selection  

  using namespace SystId;
  using namespace anaUtils;
  // --- Systematic         Detectors                                   FGD1                                    FGD2 
  groups[   kBFieldDist    ] = CreateVectorI( 2, EventBoxTracker::kTracksWithTPCInFGD1FV,  EventBoxTracker::kTracksWithTPCInFGD2FV );
  groups[   kMomScale      ] = CreateVectorI( 2, EventBoxTracker::kTracksWithTPCInFGD1FV,  EventBoxTracker::kTracksWithTPCInFGD2FV );
  groups[   kMomResol      ] = CreateVectorI( 2, EventBoxTracker::kTracksWithTPCInFGD1FV,  EventBoxTracker::kTracksWithTPCInFGD2FV );
  groups[   kTpcPid        ] = CreateVectorI( 2, EventBoxTracker::kTracksWithTPCInFGD1FV,  EventBoxTracker::kTracksWithTPCInFGD2FV );
  groups[   kChargeIDEff   ] = CreateVectorI( 2, EventBoxTracker::kTracksWithTPCInFGD1FV,  EventBoxTracker::kTracksWithTPCInFGD2FV );
  groups[   kTpcClusterEff ] = CreateVectorI( 2, EventBoxTracker::kTracksWithTPCInFGD1FV,  EventBoxTracker::kTracksWithTPCInFGD2FV ); 
  groups[   kTpcTrackEff   ] = CreateVectorI( 2, EventBoxTracker::kTracksWithTPC,          EventBoxTracker::kTracksWithTPC         );
  
  if (versionUtils::prod6_systematics)
    groups[ kTpcFgdMatchEff] = CreateVectorI( 2, EventBoxTracker::kTracksWithTPCInFGD1FV,  EventBoxTracker::kTracksWithTPCInFGD2FV );
  else
    groups[ kTpcFgdMatchEff] = CreateVectorI( 2, EventBoxTracker::kTracksWithTPCorFGD1,    EventBoxTracker::kTracksWithTPCorFGD2   );
  
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
  // No branches
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
Int_t numuCCSelection::GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  (void)branch;
  
  std::vector<std::vector<Int_t> > groups(SystId::SystEnumLast_SystId, std::vector<Int_t>(1, -1));
 
  // Explicitely set the groups for various systeamtics that need them, 
  // this allows to have a transparent set/control of the systematics by the
  // selection  

  using namespace SystId;
  using namespace anaUtils;
  // --- Systematic         Detectors                                   FGD                                
  groups[ kTpcFgdMatchEff ] =  CreateVectorI(1, EventBoxTracker::kTrueParticlesChargedInTPCorFGDInBunch);
  groups[ kTpcTrackEff    ] =  CreateVectorI(1, EventBoxTracker::kTrueParticlesChargedInTPCInBunch);
   
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

//**************************************************
void numuCCSelection::InitializeEvent(AnaEventC& eventBB){
//**************************************************

  AnaEventB& event = *static_cast<AnaEventB*>(&eventBB); 

  // Create the appropriate EventBox if it does not exist yet
  if (!event.EventBoxes[EventBoxId::kEventBoxTracker])
    event.EventBoxes[EventBoxId::kEventBoxTracker] = new EventBoxTracker();

  boxUtils::FillTracksWithTPC(event,             static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
  boxUtils::FillTracksWithFGD(event,             static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
  boxUtils::FillTrajsChargedInTPC(event);
  boxUtils::FillTrajsChargedInFGDAndNoTPC(event, static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
}

//********************************************************************
bool numuCCSelection::CheckRedoSelection(const AnaEventC& eventBB, const ToyBoxB& PreviousToyBoxB, Int_t& redoFromStep){
//********************************************************************

  const AnaEventB& event = *static_cast<const AnaEventB*>(&eventBB); 

  // Cast the ToyBox to the appropriate type
  const ToyBoxTracker& PreviousToyBox = *static_cast<const ToyBoxTracker*>(&PreviousToyBoxB); 

  // allways return true by now. TODO
  //  redoFromStep =0;
  //  return true;
  // Relevant properties of the previous toy are saved in the PreviousToyBox that is passed as parameter
  // - i.e  PreviousToyBox->HMNtrack is the HMNtrack of the previous toy
  // - i.e  PreviousToyBox->AccumLevel[i] is the accum level of the previous toy 
  //         for branch i in this selection

  // Only redo the selection when the HMN track changes identity or the PID cut has a different effect:

  // nothing to do if there is no HMN track
  if (!PreviousToyBox.HMNtrack) return false;

  /* TODO: This is not enough since the third HM track could become the HM track. Substituted by the code below
  // The momentum of SHMN track is larger than the one of HMN track
  // We have not yet called FindLeadingTracks, so HMN and SHMN are from previous toy, however we have already modified the 
  // momentum of those tracks because this method is called after the variation systematics
  if (PreviousToyBox.MaxAccumLevel > _TotalMultiplicityCutIndex){
    if (PreviousToyBox.SHMNtrack){
      if (PreviousToyBox.SHMNtrack->Momentum > PreviousToyBox.HMNtrack->Momentum){
        // We should redo the selection starting from the FindLeadingTracks step
        redoFromStep = _FindLeadingTracksStepIndex;
        return true;
      }
    }
  }
  */

  // TODO: The code below implies calling cutUtils::FindLeadingTracks(event, box) twice. Can we avoid that ?
  if (PreviousToyBox.MaxAccumLevel > _TotalMultiplicityCutIndex){
    ToyBoxTracker box;
    trackerSelUtils::FindLeadingTracks(event, box);

    // Redo the selection if any of the leading tracks changes identity
    if (PreviousToyBox.SHMNtrack!=box.SHMNtrack || 
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
  // PreviousToyBox->AccumLevel[0]>_MuonPIDCutIndex means that the PID cut was passed, so we check whether the cut 
  // was passed in the previous toy and not in the current one, or the opposit, it was not passed before 
  // and it is passed now. 

  if (PreviousToyBox.MaxAccumLevel >= _MuonPIDCutIndex){
    bool pidCut = cutUtils::MuonPIDCut(*(PreviousToyBox.HMNtrack), !versionUtils::prod6_systematics);
    if  (( pidCut && (PreviousToyBox.AccumLevel[0] == _MuonPIDCutIndex)) ||
         (!pidCut && (PreviousToyBox.AccumLevel[0]  > _MuonPIDCutIndex))){
      // We should redo the selection starting from the MuonPIDCut step
      redoFromStep = _MuonPIDStepIndex;
      return true;
    }
  }
  
  return false;
}

