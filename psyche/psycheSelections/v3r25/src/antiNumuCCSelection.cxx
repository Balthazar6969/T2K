#include "antiNumuCCSelection.hxx"
#include "baseSelection.hxx"
#include "SystematicTuning.hxx"
#include "CutUtils.hxx"
#include "EventBoxUtils.hxx"
#include "SubDetId.hxx"
#include "SystId.hxx"
#include "VersioningUtils.hxx"
#include "SystematicUtils.hxx"
#include "trackerSelectionUtils.hxx"

//********************************************************************
antiNumuCCSelection::antiNumuCCSelection(bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxTracker) {
//********************************************************************

  // Initialize the numuCCSelection, which is used
  _numuCCSelection.Initialize();

  // Initialize systematic tuning parameters
  systTuning::Initialize();
}

//********************************************************************
void antiNumuCCSelection::DefineSteps(){
//********************************************************************

    // Cuts must be added in the right order
    // last "true" means the step sequence is broken if cut is not passed (default is "false")
    AddStep(StepBase::kCut,    "event quality",      new EventQualityCut(),           true);
    AddStep(StepBase::kCut,    "> 0 tracks ",        new TotalMultiplicityCut(),      true);  
    AddStep(StepBase::kAction, "find leading tracks",new FindLeadingTracksAction_antinu());  
    AddStep(StepBase::kAction, "find vertex",        new FindVertexAction());  
    AddStep(StepBase::kAction, "fill summary",       new FillSummaryAction_antinu());  
    AddStep(StepBase::kCut,    "quality+fiducial",   new TrackQualityFiducialCut(),   true);
    AddStep(StepBase::kCut,    "pos_mult",           new PositiveMultiplicityCut());  
    AddStep(StepBase::kAction, "find veto track",    new FindVetoTrackAction());
    AddStep(StepBase::kCut,    "veto",               new ExternalVetoCut());
    AddStep(StepBase::kAction, "find oofv track",    new FindOOFVTrackAction());
    AddStep(StepBase::kCut,    "External FGD1",      new ExternalFGD1lastlayersCut());
    AddStep(StepBase::kCut,    "Anti muon PID",      new AntiMuonPIDCut());

    SetBranchAlias(0,"trunk");

    // By default the preselection correspond to cuts 0-2
    SetPreSelectionAccumLevel(2);

    // Step and Cut numbers needed by CheckRedoSelection
    _MuonPIDCutIndex            = GetCutNumber("Anti muon PID");
    _MuonPIDStepIndex           = GetStepNumber("Anti muon PID");
    _FindLeadingTracksStepIndex = GetStepNumber("find leading tracks");
    _TotalMultiplicityCutIndex  = GetCutNumber("> 0 tracks ");
}

//********************************************************************
void antiNumuCCSelection::DefineDetectorFV(){
//********************************************************************

    // The detector in which the selection is applied
    SetDetectorFV(SubDetId::kFGD1);
    _numuCCSelection.SetDetectorFV(SubDetId::kFGD1);
}

//********************************************************************
bool antiNumuCCSelection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
//********************************************************************

    // The event sample corresponding to this selection
    if(allCutsPassed[0]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD1AntiNuMuCC;

    return (static_cast<AnaEventSummaryB*>(event.Summary)->EventSample != SampleId::kUnassigned);
}

//********************************************************************
bool FillSummaryAction_antinu::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//********************************************************************

    // Cast the ToyBox to the appropriate type
    ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

    if(!box.HMPtrack) return 1;

    static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD1AntiNuMuCC] = box.HMPtrack;
    for(int i = 0; i < 4; ++i){
        static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD1AntiNuMuCC][i] = box.HMPtrack->PositionStart[i];
    }
    if(box.HMPtrack->GetTrueParticle()) static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD1AntiNuMuCC] = box.HMPtrack->GetTrueParticle()->TrueVertex;
    return 1;
}

//**************************************************
bool FindLeadingTracksAction_antinu::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************
        
    // Cast the ToyBox to the appropriate type
    ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

    // Find leading tracks with good quality and only in FGD FV
    trackerSelUtils::FindLeadingTracks(event, box);

    // For this selection the main track is the HMP track
    box.MainTrack = box.HMPtrack;
    return true;
}

//**************************************************
bool PositiveMultiplicityCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

  (void)event;

  // Cast the ToyBox to the appropriate type
  ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

  return (box.HMPtrack==box.HMtrack);
}

//**************************************************
bool AntiMuonPIDCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

    (void)event;
    // Cast the ToyBox to the appropriate type
    ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

    if (!box.HMPtrack) return false;
    if(box.HMPtrack->Momentum < 0.) return false;
    return cutUtils::AntiMuonPIDCut(*(box.HMPtrack));
}

//**************************************************
bool antiNumuCCSelection::IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* track, SystId_h systId, Int_t branch) const{
//**************************************************

  return _numuCCSelection.IsRelevantRecObjectForSystematic(event,track,systId,branch);
}

//**************************************************
bool antiNumuCCSelection::IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueTrack, SystId_h systId, Int_t branch) const{
//**************************************************

  return _numuCCSelection.IsRelevantTrueObjectForSystematic(event,trueTrack,systId,branch);
}

//**************************************************
bool antiNumuCCSelection::IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& box, AnaRecObjectC* track, SystId_h systId, Int_t branch) const{
//**************************************************

  return _numuCCSelection.IsRelevantRecObjectForSystematicInToy(event,box,track,systId,branch);
}

//**************************************************
bool antiNumuCCSelection::IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
//**************************************************

  // Apply to all objects,  not fine-tuning
  if (!systTuning::APPLY_SYST_FINE_TUNING) return true;
 
  // Cast the ToyBox to the appropriate type
  const ToyBoxTracker& box = *static_cast<const ToyBoxTracker*>(&boxB); 


  AnaTrueParticleB* trueTrack = static_cast<AnaTrueParticleB*>(trueObj);
  
  // Main track "mode",  will only consider certain true tracks of interest

  if(systId == SystId::kTpcTrackEff){
    if (box.MainTrack->GetTrueParticle() && trueTrack->ID  == box.MainTrack->GetTrueParticle()->ID){ 
      return true; 
    }
    
    // Apply for anti-muon 
    if (trueTrack->PDG == -13){  
      return true;
    }
    
    return false;
  }
  
  else if (systId == SystId::kSIProton){ 
  
    // If this trueTrack is NOT associated to the MainTrack, consider the posibility of this trueTrack to become the MainTrack
    // This is to be tuned: e.g. may consider explicitely those not reconstructed in TPC
    // also the ones that have the momentum ~closer to the Bethe-Bloch crossing point, 
    // i.e. when proton background starts to become non-negligible
    if (trueTrack->PDG == 2212 && trueTrack->Momentum > box.MainTrack->Momentum
        && trueTrack->Momentum > 1000.) return true; 
  }
  
 
  // The rest taken from numuCC analysis
  return _numuCCSelection.IsRelevantTrueObjectForSystematicInToy(event,boxB,trueObj,systId,branch);
  
    
}

//**************************************************
bool antiNumuCCSelection::IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const{
//**************************************************
  
  // For the rest trust numu
  return _numuCCSelection.IsRelevantSystematic(event, box, systId, branch);
   
}

//**************************************************
Int_t antiNumuCCSelection::GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  return _numuCCSelection.GetRelevantRecObjectGroupsForSystematic(systId, IDs, branch);
}

//**************************************************
Int_t antiNumuCCSelection::GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  return _numuCCSelection.GetRelevantTrueObjectGroupsForSystematic(systId, IDs, branch);
}

//**************************************************
void antiNumuCCSelection::InitializeEvent(AnaEventC& event){
//**************************************************

  _numuCCSelection.InitializeEvent(event);
}


//********************************************************************
bool antiNumuCCSelection::CheckRedoSelection(const AnaEventC& event, const ToyBoxB& PreviousToyBoxB, Int_t& redoFromStep){
//********************************************************************

  (void)event;

  // Cast the ToyBox to the appropriate type
  const ToyBoxTracker& PreviousToyBox = *static_cast<const ToyBoxTracker*>(&PreviousToyBoxB); 

  // Relevant properties of the previous toy are saved in the PreviousToyBox that is passed as parameter
  // - i.e  PreviousToyBox->HMPtrack is the HMPtrack of the previous toy
  // - i.e  PreviousToyBox->AccumLevel[i] is the accum level of the previous toy 
  //         for branch i in this selection

  // Only redo the selection when the HMN track changes identity or the PID cut has a different effect:

  // nothing to do if there is no HMP track
  if (!PreviousToyBox.HMPtrack) return false;

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
    bool pidCut = cutUtils::AntiMuonPIDCut(*(PreviousToyBox.HMPtrack));
    if  (( pidCut && (PreviousToyBox.AccumLevel[0] == _MuonPIDCutIndex)) ||
         (!pidCut && (PreviousToyBox.AccumLevel[0]  > _MuonPIDCutIndex))){
      redoFromStep = _MuonPIDStepIndex;
      return true;
    }
  }
  
  return false;
}
