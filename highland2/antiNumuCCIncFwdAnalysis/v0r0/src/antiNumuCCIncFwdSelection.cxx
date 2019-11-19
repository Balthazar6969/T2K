#include "antiNumuCCIncFwdSelection.hxx"
#include "antiNumuCCIncFwdAnalysis.hxx"
#include "baseSelection.hxx"
#include "SystematicTuning.hxx"
#include "CutUtils.hxx"
#include "EventBoxUtils.hxx"
#include "SubDetId.hxx"
#include "SystId.hxx"
#include "VersioningUtils.hxx"
#include "SystematicUtils.hxx"
#include "trackerSelectionUtils.hxx"

#include "DataClasses.hxx"


//********************************************************************
antiNumuCCIncFwdSelection::antiNumuCCIncFwdSelection(bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxTracker) {
//********************************************************************

  // Initialize the numuCCSelection, which is used
  _antiNumuCCSelection.Initialize();

  // Initialize systematic tuning parameters
  systTuning::Initialize();
}

//********************************************************************
void antiNumuCCIncFwdSelection::DefineSteps(){
//********************************************************************

    CopySteps(_antiNumuCCSelection,0,0,10,0);

    AddStep(StepBase::kCut,		"OOFV timing: ECal main track", new OOFVTECalFGD1Cut());
    AddStep(StepBase::kCut,		"OOFV timing: FGD2 main track", new OOFVFGD2FGD1Cut());
    AddStep(StepBase::kCut,		"OOFV timing: other tracks",    new OOFVOtherCut());
    AddStep(StepBase::kCut,		"TPC antimuon PID",             new TPCAntiMuonPIDCut());
    AddStep(StepBase::kCut,		"ECal contained PID",           new EcalContainedPIDCut());
    AddStep(StepBase::kCut,		"ECal other PID",               new EcalOtherPIDCut());
    AddStep(StepBase::kCut,	 	"FGD2 veto",                    new FGD2VetoCut());

    SetBranchAlias(0,"trunk");

    // By default the preselection correspond to cuts 0-2
    SetPreSelectionAccumLevel(2);

    // Step and Cut numbers needed by CheckRedoSelection
    _AntiMuonPIDCutIndex            = GetCutNumber("TPC antimuon PID");
    _AntiMuonPIDStepIndex           = GetStepNumber("TPC antimuon PID");
    _FindLeadingTracksStepIndex     = GetStepNumber("find leading tracks");
    _TotalMultiplicityCutIndex      = GetCutNumber("> 0 tracks ");
}

//********************************************************************
void antiNumuCCIncFwdSelection::DefineDetectorFV(){
//********************************************************************

    // The detector in which the selection is applied
    SetDetectorFV(SubDetId::kFGD1);
}

//********************************************************************
bool antiNumuCCIncFwdSelection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
//********************************************************************

    // The event sample corresponding to this selection
    if(allCutsPassed[0]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD1AntiNuMuCC;

    return (static_cast<AnaEventSummaryB*>(event.Summary)->EventSample != SampleId::kUnassigned);
}

//**************************************************
bool OOFVTECalFGD1Cut::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************

  (void) event;

  // cast the box to the appropriate type
  ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB);

  // get the main track
  AnaTrackB* track = box.MainTrack;
  if(!track) return false;
/*
  // pass tracks not going into the ecal
  if(track->nECALSegments <= 0) return true;

  // get the ecal segment
  AnaECALParticle* ecal_segment = static_cast<AnaECALParticle*>(track->ECALSegments[0]);
  if(!ecal_segment) return false;
*/
  if(!track->ToF.Flag_ECal_FGD1) return true;
  
  float ToF = track->ToF.ECal_FGD1;

  return (ToF > _time_main_ecal_min);
}

//**************************************************
bool OOFVFGD2FGD1Cut::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************

  (void) event;

  // cast the box to the appropriate type
  ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB);

  // get the main track
  AnaTrackB* track = box.MainTrack;
  if(!track) return false;

  if(!track->ToF.Flag_FGD1_FGD2) return true;
  
  float ToF = track->ToF.FGD1_FGD2;

  return (ToF > _time_main_fgd2_min);
}

//********************************************************************
bool OOFVOtherCut::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//********************************************************************

  (void) event;

  // cast the event to the appropriate type
  AnaEventB& eventB = *static_cast<AnaEventB*>(&event);

  // cast the box to the appropriate type
  ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB);

  float ToF_ECal = false;
  float ToF_min_ECal = false;

  AnaTrackB* primary = box.MainTrack;
  if(!primary) return false;

  AnaTrackB* ecalTracks[100];
  Int_t ntracks = anaUtils::GetAllTracksUsingDet(*static_cast<AnaEventB*>(&eventB), SubDetId::kECAL, ecalTracks);

  if (ntracks > 0) {

    for (int i = 0; i < ntracks; ++i) {

      AnaTrackB* thisTrack = ecalTracks[i];

      if (thisTrack == primary) continue;

      if (primary->Time.Flag_FGD1) {
        if (thisTrack->Time.Flag_DSECal) ToF_ECal = thisTrack->Time.DSECal-primary->Time.FGD1;
        else if (thisTrack->Time.Flag_ECal) ToF_ECal = thisTrack->Time.ECal-primary->Time.FGD1;

        if (!ToF_min_ECal) ToF_min_ECal = ToF_ECal;
        else if (ToF_ECal < ToF_min_ECal) ToF_min_ECal = ToF_ECal;
      }
    }

    if (!ToF_min_ECal) return true;
    return (ToF_min_ECal > _time_other_min);
  }

  return true;
}

//**************************************************
bool TPCAntiMuonPIDCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

  (void)event;

  // Cast the ToyBox to the appropriate type
  ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB);

  if (!box.HMPtrack) return false;
  if(box.HMPtrack->Momentum < 0.) return false;

  AnaTrackB& track = *static_cast<AnaTrackB*>(box.HMPtrack);

  Float_t cut1 = _likemip_min;
  Float_t cut2 = _likemu_min;
  Float_t cut3 = _likemu_max;
  Float_t pcut = 500;

  Float_t PIDLikelihood[4];
  anaUtils::GetPIDLikelihood(track, PIDLikelihood);

  if (((PIDLikelihood[0]+PIDLikelihood[3])/(1-PIDLikelihood[2]) > cut1 || track.Momentum > pcut ) && (PIDLikelihood[0]>cut2 && PIDLikelihood[0]<cut3)){
    return true;
  }

  return false;
}

//**************************************************
bool EcalContainedPIDCut::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************

  (void) event;

  // cast the box to the appropriate type
  ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB);

  // get the main track
  AnaTrackB* track = box.MainTrack;
  if(!track) return false;

  // pass tracks not going into the ecal
  if(track->nECALSegments <= 0) return true;

  // get the ecal segment
  AnaECALParticle* ecal_segment = static_cast<AnaECALParticle*>(track->ECALSegments[0]);
  if(!ecal_segment) return false;

  // cast the ecal segment to the appropriate type
  AnaECALParticle& segment = *static_cast<AnaECALParticle*>(ecal_segment);

  // pass tracks not contained in the ecal
  if(segment.Containment!=1) return true;

  // cast the track to the appropriate type
  AnaTrackB& ttrack = *static_cast<AnaTrackB*>(track);

  // cut tracks with high momentum & ecal PID
  return (ttrack.Momentum < _cont_mom_max && ecal_segment->PIDMipPion < _cont_mippion_max);
}

//**************************************************
bool EcalOtherPIDCut::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************

  (void) event;

  // cast the box to the appropriate type
  ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB);

  // get the main track
  AnaTrackB* track = box.MainTrack;
  if(!track) return false;

  // pass tracks not going into the ecal
  if(track->nECALSegments <= 0) return true;

  // get the ecal segment
  AnaECALParticle* ecal_segment = static_cast<AnaECALParticle*>(track->ECALSegments[0]);
  if(!ecal_segment) return false;

  // cast the ecal segment to the appropriate type
  AnaECALParticle& segment = *static_cast<AnaECALParticle*>(ecal_segment);

  // pass tracks contained in the ecal
  if(segment.Containment==1) return true;

  // ecal PID
  return (ecal_segment->PIDMipPion < _other_mippion_max);
}

//**************************************************
bool FGD2VetoCut::Apply(AnaEventC& event, ToyBoxB& boxB) const {
//**************************************************

  (void) event;

  // cast the box to the appropriate type
  ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB);

  // get the main track
  AnaTrackB* track = box.MainTrack;
  if(!track) return false;

  // cut tracks stopping in FGD2
  if(track->nECALSegments > 0) return true;
  if(anaUtils::TrackUsesDet(*static_cast<AnaTrackB*>(track), SubDetId::kFGD2) && !anaUtils::TrackUsesDet(*static_cast<AnaTrackB*>(track), SubDetId::kTPC3)) return false;

  return true;
}


//**************************************************
bool antiNumuCCIncFwdSelection::IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* track, SystId_h systId, Int_t branch) const{
//**************************************************

  return _antiNumuCCSelection.IsRelevantRecObjectForSystematic(event,track,systId,branch);
}

//**************************************************
bool antiNumuCCIncFwdSelection::IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueTrack, SystId_h systId, Int_t branch) const{
//**************************************************

  return _antiNumuCCSelection.IsRelevantTrueObjectForSystematic(event,trueTrack,systId,branch);
}

//**************************************************
bool antiNumuCCIncFwdSelection::IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& box, AnaRecObjectC* track, SystId_h systId, Int_t branch) const{
//**************************************************

  return _antiNumuCCSelection.IsRelevantRecObjectForSystematicInToy(event,box,track,systId,branch);
}

//**************************************************
bool antiNumuCCIncFwdSelection::IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
//**************************************************

  return _antiNumuCCSelection.IsRelevantTrueObjectForSystematicInToy(event,boxB,trueObj,systId,branch);
}

//**************************************************
bool antiNumuCCIncFwdSelection::IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const{
//**************************************************
  
  return _antiNumuCCSelection.IsRelevantSystematic(event, box, systId, branch);
}

//**************************************************
Int_t antiNumuCCIncFwdSelection::GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  if(systId == antiNumuCCIncFwdAnalysis::kToFDSECalFGD1) return true;
  if(systId == antiNumuCCIncFwdAnalysis::kToFECalOther) return true;
  if(systId == antiNumuCCIncFwdAnalysis::kECalMIPPion) return true;

  return _antiNumuCCSelection.GetRelevantRecObjectGroupsForSystematic(systId, IDs, branch);
}

//**************************************************
Int_t antiNumuCCIncFwdSelection::GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  if(systId == antiNumuCCIncFwdAnalysis::kToFDSECalFGD1) return true;
  if(systId == antiNumuCCIncFwdAnalysis::kToFECalOther) return true;
  if(systId == antiNumuCCIncFwdAnalysis::kECalMIPPion) return true;

  return _antiNumuCCSelection.GetRelevantTrueObjectGroupsForSystematic(systId, IDs, branch);
}


//**************************************************
void antiNumuCCIncFwdSelection::InitializeEvent(AnaEventC& event){
//**************************************************

  _antiNumuCCSelection.InitializeEvent(event);
}


//********************************************************************
bool antiNumuCCIncFwdSelection::CheckRedoSelection(const AnaEventC& event, const ToyBoxB& PreviousToyBoxB, Int_t& redoFromStep){
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


  if (PreviousToyBox.MaxAccumLevel >= _AntiMuonPIDCutIndex){
    bool pidCut = cutUtils::AntiMuonPIDCut(*(PreviousToyBox.HMPtrack));
    if  (( pidCut && (PreviousToyBox.AccumLevel[0] == _AntiMuonPIDCutIndex)) ||
         (!pidCut && (PreviousToyBox.AccumLevel[0]  > _AntiMuonPIDCutIndex))){
      redoFromStep = _AntiMuonPIDStepIndex;
      return true;
    }
  }
  
  return false;
}
