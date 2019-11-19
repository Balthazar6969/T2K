#include "numuBkgInAntiNuModeCCSelection.hxx"
#include "baseSelection.hxx"
#include "numuCCSelection.hxx"
#include "CutUtils.hxx"
#include "SubDetId.hxx"
#include "VersioningUtils.hxx"
#include "trackerSelectionUtils.hxx"
#include "EventBoxId.hxx"
#include "SystId.hxx"

//********************************************************************
numuBkgInAntiNuModeCCSelection::numuBkgInAntiNuModeCCSelection(bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxTracker) {
//********************************************************************

    // Initialize the numuCCSelection, which is used
    _numuCCSelection.Initialize();
}

//********************************************************************
void numuBkgInAntiNuModeCCSelection::DefineSteps(){
//********************************************************************

    // Cuts must be added in the right order
    // last "true" means the step sequence is broken if cut is not passed (default is "false")
    AddStep(StepBase::kCut,    "event quality",      new EventQualityCut(),           true);
    AddStep(StepBase::kCut,    "> 0 tracks ",        new TotalMultiplicityCut(),      true);  
    AddStep(StepBase::kAction, "find leading tracks",new FindLeadingTracksAction());  
    AddStep(StepBase::kAction, "find vertex",        new FindVertexAction());  
    AddStep(StepBase::kAction, "fill summary",       new FillSummaryAction_numuBkg());  
    AddStep(StepBase::kCut,    "quality+fiducial",   new TrackQualityFiducialCut(),   true);
    AddStep(StepBase::kCut,    "neg_mult",           new NegativeMultiplicityCut());  
    AddStep(StepBase::kAction, "find veto track",    new FindVetoTrackAction());
    AddStep(StepBase::kCut,    "veto",               new ExternalVetoCut());
    AddStep(StepBase::kAction, "find oofv track",    new FindOOFVTrackAction());
    AddStep(StepBase::kCut,    "External FGD1",      new ExternalFGD1lastlayersCut());
    AddStep(StepBase::kCut,    "Muon PID",           new NumuBkgMuonPIDCut());

    SetBranchAlias(0,"trunk");

    // By default the preselection correspond to cuts 0-2
    SetPreSelectionAccumLevel(2);

    // Step and Cut numbers needed by CheckRedoSelection
    _MuonPIDCutIndex            = GetCutNumber("Muon PID");
    _MuonPIDStepIndex           = GetStepNumber("Muon PID");
    _FindLeadingTracksStepIndex = GetStepNumber("find leading tracks");
    _TotalMultiplicityCutIndex  = GetCutNumber("> 0 tracks ");
}

//********************************************************************
void numuBkgInAntiNuModeCCSelection::DefineDetectorFV(){
//********************************************************************

    // The detector in which the selection is applied
    SetDetectorFV(SubDetId::kFGD1);

    // Set the detector field into the selection beaing used
    _numuCCSelection.SetDetectorFV(SubDetId::kFGD1);
}

//********************************************************************
bool numuBkgInAntiNuModeCCSelection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
//********************************************************************

    // The event sample corresponding to this selection
    if(allCutsPassed[0]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD1NuMuBkgInAntiNuModeCC;

    return (static_cast<AnaEventSummaryB*>(event.Summary)->EventSample != SampleId::kUnassigned);
}

//********************************************************************
bool FillSummaryAction_numuBkg::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//********************************************************************

    // Cast the ToyBox to the appropriate type
    ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

    if(!box.HMNtrack) return true;

    static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD1NuMuBkgInAntiNuModeCC] = box.HMNtrack;
    for(int i = 0; i < 4; ++i){
        static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD1NuMuBkgInAntiNuModeCC][i] = box.HMNtrack->PositionStart[i];
    }
    if(box.HMNtrack->GetTrueParticle()) static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD1NuMuBkgInAntiNuModeCC] = box.HMNtrack->GetTrueParticle()->TrueVertex;
    return true;
}

//**************************************************
bool NegativeMultiplicityCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

    (void)event;

    // Cast the ToyBox to the appropriate type
    ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

    return (box.HMNtrack==box.HMtrack);
}

//**************************************************
bool NumuBkgMuonPIDCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

    (void)event;

    // Cast the ToyBox to the appropriate type
    ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

    if (!box.HMNtrack) return false;
    if(box.HMNtrack->Momentum < 0.) return false;
    return cutUtils::NumuBkgMuonPIDCut(*(box.HMNtrack));
}

//**************************************************
bool numuBkgInAntiNuModeCCSelection::IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* track, SystId_h systId, Int_t branch) const{
//**************************************************

  return _numuCCSelection.IsRelevantRecObjectForSystematic(event,track,systId,branch);
}

//**************************************************
bool numuBkgInAntiNuModeCCSelection::IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueTrack, SystId_h systId, Int_t branch) const{
//**************************************************

  return _numuCCSelection.IsRelevantTrueObjectForSystematic(event,trueTrack,systId,branch);
}

//**************************************************
bool numuBkgInAntiNuModeCCSelection::IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const{
//**************************************************

  // Same as for numuCC
  return _numuCCSelection.IsRelevantSystematic(event, box, systId, branch);
}

//**************************************************
bool numuBkgInAntiNuModeCCSelection::IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& box, AnaRecObjectC* recObj,  SystId_h systId, Int_t branch) const{
//**************************************************

  // Same as for numuCC
  return _numuCCSelection.IsRelevantRecObjectForSystematicInToy(event, box, recObj, systId, branch);
}

//**************************************************
bool numuBkgInAntiNuModeCCSelection::IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& box, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
//**************************************************
 
  // Same as for numuCC
  return _numuCCSelection.IsRelevantTrueObjectForSystematicInToy(event, box, trueObj, systId, branch);
}

//**************************************************
Int_t numuBkgInAntiNuModeCCSelection::GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  return _numuCCSelection.GetRelevantRecObjectGroupsForSystematic(systId, IDs, branch);
}

//**************************************************
Int_t numuBkgInAntiNuModeCCSelection::GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  return _numuCCSelection.GetRelevantTrueObjectGroupsForSystematic(systId, IDs, branch);
}


//**************************************************
void numuBkgInAntiNuModeCCSelection::InitializeEvent(AnaEventC& event){
//**************************************************

  _numuCCSelection.InitializeEvent(event);
}

//********************************************************************
bool numuBkgInAntiNuModeCCSelection::CheckRedoSelection(const AnaEventC& event, const ToyBoxB& PreviousToyBoxB, Int_t& redoFromStep){
//********************************************************************

  (void)event;
  //  redoFromStep=0;
  //  return true;
  // Relevant properties of the previous toy are saved in the PreviousToyBox that is passed as parameter
  // - i.e  PreviousToyBox->HMNtrack is the HMNtrack of the previous toy
  // - i.e  PreviousToyBox->AccumLevel[i] is the accum level of the previous toy 
  //         for branch i in this selection

  // Only redo the selection when the HMN track changes identity or the PID cut has a different effect:

  // Cast the ToyBox to the appropriate type
  const ToyBoxTracker& PreviousToyBox = *static_cast<const ToyBoxTracker*>(&PreviousToyBoxB); 

  // nothing to do if there is no HMN track
  if (!PreviousToyBox.HMNtrack) return false;

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

  // When the leading tracks do not change identity, Redo the selection if the effect of the PID cut is different.
  // We have previously saved in the EventBox the AccumLevel of the previous toy for each branch. 
  // PreviousToyBox->AccumLevel[0]>_MuonPIDCutIndex means that the PID cut was passed, so we check whether the cut 
  // was passed in the previous toy and not in the current one, or the opposit, it was not passed before 
  // and it is passed now. 

  if (PreviousToyBox.MaxAccumLevel >= _MuonPIDCutIndex){
    bool pidCut = cutUtils::NumuBkgMuonPIDCut(*(PreviousToyBox.HMNtrack));
    if  (( pidCut && (PreviousToyBox.AccumLevel[0] == _MuonPIDCutIndex)) ||
         (!pidCut && (PreviousToyBox.AccumLevel[0]  > _MuonPIDCutIndex))){
      // We should redo the selection starting from the MuonPIDCut step
      redoFromStep = _MuonPIDStepIndex;
      return true;
    }
  }
  
  return false;
}



