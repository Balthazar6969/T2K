#include "numuCC4piSelection.hxx"
#include "baseSelection.hxx"
#include "SystematicTuning.hxx"
#include "CutUtils.hxx"
#include "EventBoxUtils.hxx"
#include "Parameters.hxx"
#include "SubDetId.hxx"
#include "SystId.hxx"
#include "SystematicUtils.hxx"


//********************************************************************
numuCC4piSelection::numuCC4piSelection(bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxTracker) {
  //********************************************************************

  // Initialize systematic tuning parameters
  systTuning::Initialize();
}

//********************************************************************
void numuCC4piSelection::DefineSteps(){
  //********************************************************************

  AddStep(StepBase::kCut,    "event quality",       new EventQualityCut(),      true); //if passed accum_level=1
  AddStep(StepBase::kCut,    "> 0 tracks ",         new numuCC4pi::TotalMultiplicityCut(), true); //if passed accum_level=2
  AddStep(StepBase::kAction, "Sort TPC tracks",     new numuCC4pi::SortTracksAction());
  AddStep(StepBase::kCut,    "quality+fiducial",    new numuCC4pi::TrackGQandFVCut(),      true); //if passed accum_level=3
  AddStep(StepBase::kAction, "veto Action",         new numuCC4pi::VetoAction());
  AddStep(StepBase::kAction, "muon PID Action",     new numuCC4pi::PIDAction());
  AddStep(StepBase::kAction, "find vertex",         new numuCC4pi::FindVertexAction());
  AddStep(StepBase::kAction, "fill summary",        new FillSummaryAction_numuCC4pi());

  AddSplit(4);

  AddStep(0, StepBase::kCut, "Fwd Quality Cut",     new numuCC4pi::Fwd_Quality());         //if passed accum_level=4
  AddStep(0, StepBase::kCut, "Fwd Veto Cut",        new numuCC4pi::Fwd_Veto());            //if passed accum_level=5    
  AddStep(0, StepBase::kCut, "Fwd PID Cut",         new numuCC4pi::Fwd_PID());                //if passed accum_level=6
  AddStep(0, StepBase::kCut, "Fwd 4pi Cut",         new numuCC4pi::Fwd());             		 //if passed accum_level=7

  AddStep(1, StepBase::kCut, "Bwd Quality Cut",     new numuCC4pi::Bwd_Quality());
  AddStep(1, StepBase::kCut, "Bwd Veto Cut",        new numuCC4pi::Bwd_Veto());
  AddStep(1, StepBase::kCut, "Bwd PID Cut",         new numuCC4pi::Bwd_PID());
  AddStep(1, StepBase::kCut, "Bwd 4pi Cut",         new numuCC4pi::Bwd());

  AddStep(2, StepBase::kCut, "HAFwd Quality Cut",   new numuCC4pi::HAFwd_Quality());
  AddStep(2, StepBase::kCut, "HAFwd Veto Cut",      new numuCC4pi::HAFwd_Veto());
  AddStep(2, StepBase::kCut, "HAFwd PID Cut",       new numuCC4pi::HAFwd_PID());
  AddStep(2, StepBase::kCut, "HAFwd 4pi Cut",       new numuCC4pi::HAFwd());

  AddStep(3, StepBase::kCut, "HABwd Quality Cut",   new numuCC4pi::HABwd_Quality());
  AddStep(3, StepBase::kCut, "HABwd Veto Cut",      new numuCC4pi::HABwd_Veto());
  AddStep(3, StepBase::kCut, "HABwd PID Cut",       new numuCC4pi::HABwd_PID());
  AddStep(3, StepBase::kCut, "HABwd 4pi Cut",       new numuCC4pi::HABwd());

  SetBranchAlias(0, "Fwd",    0);
  SetBranchAlias(1, "Bwd",    1);
  SetBranchAlias(2, "HAFwd",  2);
  SetBranchAlias(3, "HABwd",  3);

  SetPreSelectionAccumLevel(2);

}

//********************************************************************
void numuCC4piSelection::DefineDetectorFV(){
  //********************************************************************

  // Change FV definition to take all thickness
  // Note! this will affect all downstream stuff
  FVDef::FVdefminFGD1[2] = 0;
  FVDef::FVdefmaxFGD1[2] = 0;

  // The detector in which the selection is applied
  SetDetectorFV(SubDetId::kFGD1);

}

//**************************************************
void numuCC4piSelection::InitializeEvent(AnaEventC& eventBB){
  //**************************************************

  AnaEventB& event = *static_cast<AnaEventB*>(&eventBB);

  // Create the appropriate EventBox if it does not exist yet
  if (!event.EventBoxes[EventBoxId::kEventBoxTracker]) event.EventBoxes[EventBoxId::kEventBoxTracker] = new EventBoxTracker();

  boxUtils::FillTracksWithTPC(event,  static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
  boxUtils::FillTracksWithFGD(event,  static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
  boxUtils::FillTracksWithECal(event);

  boxUtils::FillTrajsChargedInTPC(event);
  boxUtils::FillTrajsChargedInFGDAndNoTPC(event,  static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
  boxUtils::FillTrajsChargedHATracker(event,  static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
}

//********************************************************************
bool numuCC4piSelection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
  //********************************************************************
  if(allCutsPassed[0]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD1NuMuCC;
  if(allCutsPassed[1]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD1NuMuCC;
  if(allCutsPassed[2]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD1NuMuCC;
  if(allCutsPassed[3]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD1NuMuCC;

  return (static_cast<AnaEventSummaryB*>(event.Summary)->EventSample != SampleId::kFGD1NuMuCC);
}

//**************************************************
bool numuCC4pi::TotalMultiplicityCut::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************

  (void)box;

  // Check we have at least one reconstructed track in the FGD
  EventBoxB* eventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];
 
  
  SubDetId_h det = box.DetectorFV;
  
  if (det == SubDetId::kFGD1){
    return eventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithFGD1] > 0;
  }
  else if (det == SubDetId::kFGD2){
    return eventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithFGD2] > 0;
  }
  else if (det == SubDetId::kFGD){
    return (eventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithFGD1] > 0 || 
        eventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithFGD2] > 0);
  }
 
  return false;
 
}

//**************************************************
bool numuCC4pi::SortTracksAction::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************

  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);

  // Retrieve the EventBoxTracker
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];

  //Find TPCGoodQuality tracks in Fwd and Bwd
  int nTPC = EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithGoodQualityTPCInFGD1FV];
  
  for (Int_t i=0;i<nTPC; ++i){
    AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithGoodQualityTPCInFGD1FV][i]);
    if ( track->Charge!=-1 ) continue;
    cc4pibox->LowAngle.push_back(track);
  }
  // Sort TPCGoodQuality using Momentum method
  std::sort(cc4pibox->LowAngle.begin(), cc4pibox->LowAngle.end(), AnaTrackB::CompareMomentum);

  int nECALTracks=EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithECal];
  for(int i=0;i<nECALTracks;i++){
    AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithECal][i]);
    if ( anaUtils::InFiducialVolume(SubDetId::kFGD1, track->PositionStart) ) {
      if ( cutUtils::TrackQualityCut(*track) ) continue;
      if ( cutUtils::StoppingBrECALorSMRDCut(track->PositionEnd)==-1 ) continue;
      cc4pibox->HighAngle.push_back(track);
    }
  }
  // Sort HighAngle using RangeMomentumMuon method
  std::sort(cc4pibox->HighAngle.begin(), cc4pibox->HighAngle.end(), AnaTrackB::CompareMuonRangeMomentum);

  return true;

}

//**************************************************
bool numuCC4pi::TrackGQandFVCut::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************

  (void)event; 

  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);

  for(UInt_t i=0;i<cc4pibox->LowAngle.size();i++ ){
    if ( anaUtils::InFiducialVolume(SubDetId::kFGD1, cc4pibox->LowAngle[i]->PositionStart, numuCC4pi_utils::LAFVmin, numuCC4pi_utils::LAFVmax) ){
      if ( ToFSenseCorrector::IsForward(*cc4pibox->LowAngle[i]) ) cc4pibox->FwdTracks.push_back(cc4pibox->LowAngle[i]);
      else cc4pibox->BwdTracks.push_back(cc4pibox->LowAngle[i]);
      break;
    }
  }

  for(UInt_t i=0;i<cc4pibox->HighAngle.size();i++){
    if ( anaUtils::InFiducialVolume(SubDetId::kFGD1, cc4pibox->HighAngle[i]->PositionStart, numuCC4pi_utils::HAFVmin, numuCC4pi_utils::HAFVmax) ) {
      if ( ToFSenseCorrector::IsForward(*cc4pibox->HighAngle[i]) ) cc4pibox->HAFwdTracks.push_back(cc4pibox->HighAngle[i]);
      else cc4pibox->HABwdTracks.push_back(cc4pibox->HighAngle[i]);
    }
  }

  if      ( cc4pibox->FwdTracks.size()>0   ) { cc4pibox->MainTrack = cc4pibox->FwdTracks[0];   return true; }
  else if ( cc4pibox->BwdTracks.size()>0   ) { cc4pibox->MainTrack = cc4pibox->BwdTracks[0];   return true; }
  else if ( cc4pibox->HAFwdTracks.size()>0 ) { cc4pibox->MainTrack = cc4pibox->HAFwdTracks[0]; return true; }
  else if ( cc4pibox->HABwdTracks.size()>0 ) { cc4pibox->MainTrack = cc4pibox->HABwdTracks[0]; return true; }

  return false;

}

//**************************************************
bool numuCC4pi::VetoAction::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************

  AnaEventB& eventB = *static_cast<AnaEventB*>(&event);

  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);

  for (UInt_t i=0;i<cc4pibox->FwdTracks.size();i++){
    if ( numuCC4pi_utils::VetoCut(0,eventB,*(cc4pibox->FwdTracks[i])) ) {
      if ( numuCC4pi_utils::ExternalCut(0,event,*(cc4pibox->FwdTracks[i])) ) cc4pibox->FwdTracks_Veto.push_back(cc4pibox->FwdTracks[i]);
    }
  }

  for (UInt_t i=0;i<cc4pibox->BwdTracks.size();i++){
    cc4pibox->BwdTracks_Veto.push_back(cc4pibox->BwdTracks[i]);
  }

  for (UInt_t i=0;i<cc4pibox->HAFwdTracks.size();i++){
    if ( numuCC4pi_utils::VetoCut(2,eventB,*(cc4pibox->HAFwdTracks[i])) ) cc4pibox->HAFwdTracks_Veto.push_back(cc4pibox->HAFwdTracks[i]);
  }

  for (UInt_t i=0;i<cc4pibox->HABwdTracks.size();i++){
    if ( numuCC4pi_utils::VetoCut(3,eventB,*(cc4pibox->HABwdTracks[i])) ) cc4pibox->HABwdTracks_Veto.push_back(cc4pibox->HABwdTracks[i]);
  }

  if      ( cc4pibox->FwdTracks_Veto.size()>0   ) cc4pibox->MainTrack = cc4pibox->FwdTracks_Veto[0];
  else if ( cc4pibox->BwdTracks_Veto.size()>0   ) cc4pibox->MainTrack = cc4pibox->BwdTracks_Veto[0];
  else if ( cc4pibox->HAFwdTracks_Veto.size()>0 ) cc4pibox->MainTrack = cc4pibox->HAFwdTracks_Veto[0];
  else if ( cc4pibox->HABwdTracks_Veto.size()>0 ) cc4pibox->MainTrack = cc4pibox->HABwdTracks_Veto[0];

  return true;

}

//**************************************************
bool numuCC4pi::PIDAction::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************

  (void)event;

  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);

  for (UInt_t i=0;i<cc4pibox->FwdTracks_Veto.size();i++){
    if ( numuCC4pi_utils::PIDCut(0,*(cc4pibox->FwdTracks_Veto[i]))==1 ) cc4pibox->FwdTracks_PID.push_back(cc4pibox->FwdTracks_Veto[i]);
  }

  for (UInt_t i=0;i<cc4pibox->BwdTracks_Veto.size();i++){
    if ( numuCC4pi_utils::PIDCut(1,*(cc4pibox->BwdTracks_Veto[i]))==1 ) cc4pibox->BwdTracks_PID.push_back(cc4pibox->BwdTracks_Veto[i]);
  }

  for (UInt_t i=0;i<cc4pibox->HAFwdTracks_Veto.size();i++){
    if ( numuCC4pi_utils::PIDCut(2,*(cc4pibox->HAFwdTracks_Veto[i]))==1 ) cc4pibox->HAFwdTracks_PID.push_back(cc4pibox->HAFwdTracks_Veto[i]);
  }

  for (UInt_t i=0;i<cc4pibox->HABwdTracks_Veto.size();i++){
    if ( numuCC4pi_utils::PIDCut(2,*(cc4pibox->HABwdTracks_Veto[i]))==1 ) cc4pibox->HABwdTracks_PID.push_back(cc4pibox->HABwdTracks_Veto[i]);
  }

  if      ( cc4pibox->FwdTracks_PID.size()>0    ) cc4pibox->MainTrack = cc4pibox->FwdTracks_PID[0];
  else if ( cc4pibox->BwdTracks_PID.size()>0    ) cc4pibox->MainTrack = cc4pibox->BwdTracks_PID[0];
  else if ( cc4pibox->HAFwdTracks_PID.size()>0  ) cc4pibox->MainTrack = cc4pibox->HAFwdTracks_PID[0];
  else if ( cc4pibox->HABwdTracks_PID.size()>0  ) cc4pibox->MainTrack = cc4pibox->HABwdTracks_PID[0];

  return true;

}

//Define vertex
//**************************************************
bool numuCC4pi::FindVertexAction::Apply(AnaEventC& event, ToyBoxB& boxB) const{
  //**************************************************

  (void)event;

  ToyBoxTracker& box    = *static_cast<ToyBoxTracker*>(&boxB);
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&boxB);

  // reset the vertex
  box.Vertex = NULL;

  if ( !cc4pibox->MainTrack ) return false;

  box.Vertex = new AnaVertexB();
  anaUtils::CreateArray(box.Vertex->Particles, 1);

  box.Vertex->nParticles = 0;
  box.Vertex->Particles[box.Vertex->nParticles++] = cc4pibox->MainTrack;

  anaUtils::CopyArray( cc4pibox->MainTrack->PositionStart, box.Vertex->Position, 4);

  if ( cc4pibox->MainTrack->TrueObject ) box.TrueVertex = box.Vertex->TrueVertex = cc4pibox->MainTrack->GetTrueParticle()->TrueVertex;

  return true;
}

//********************************************************************
bool FillSummaryAction_numuCC4pi::Apply(AnaEventC& event, ToyBoxB& boxB) const{
  //********************************************************************

  // Cast the ToyBox to the appropriate type
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&boxB);

  if(!cc4pibox->MainTrack) return 1;

  static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD1NuMuCC] = cc4pibox->MainTrack;

  anaUtils::CopyArray( cc4pibox->MainTrack->PositionStart, static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD1NuMuCC], 4);

  if(cc4pibox->MainTrack->GetTrueParticle()) static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD1NuMuCC] = cc4pibox->MainTrack->GetTrueParticle()->TrueVertex;

  return true;
}

//**************************************************
bool numuCC4pi::Fwd_Quality::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  if (cc4pibox->FwdTracks.size()>0) return true;
  return false;
}
//**************************************************
bool numuCC4pi::Fwd_Veto::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  if (cc4pibox->FwdTracks_Veto.size()>0) return true;
  return false;
}
//**************************************************
bool numuCC4pi::Fwd_PID::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  if (cc4pibox->FwdTracks_PID.size()>0) return true;
  return false;
}
//**************************************************
bool numuCC4pi::Fwd::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  if (cc4pibox->FwdTracks_PID.size()>0) return true;
  return false;
}

//**************************************************
bool numuCC4pi::Bwd_Quality::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  if (cc4pibox->BwdTracks.size()>0) return true;
  return false;
}
//**************************************************
bool numuCC4pi::Bwd_Veto::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  if (cc4pibox->BwdTracks_Veto.size()>0) return true;
  return false;
}
//**************************************************
bool numuCC4pi::Bwd_PID::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  if (cc4pibox->BwdTracks_PID.size()>0) return true;
  return false;
}
//**************************************************
bool numuCC4pi::Bwd::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  if (cc4pibox->BwdTracks_PID.size()>0 && cc4pibox->FwdTracks_PID.size()==0) return true;
  return false;
}

//**************************************************
bool numuCC4pi::HAFwd_Quality::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  if (cc4pibox->HAFwdTracks.size()>0) return true;
  return false;
}
//**************************************************
bool numuCC4pi::HAFwd_Veto::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  if (cc4pibox->HAFwdTracks_Veto.size()>0) return true;
  return false;
}
//**************************************************
bool numuCC4pi::HAFwd_PID::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  if (cc4pibox->HAFwdTracks_PID.size()>0) return true;
  return false;
}
//**************************************************
bool numuCC4pi::HAFwd::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  if (cc4pibox->HAFwdTracks_PID.size()>0 && cc4pibox->FwdTracks_PID.size()==0 && cc4pibox->BwdTracks_PID.size()==0) return true;
  return false;
}

//**************************************************
bool numuCC4pi::HABwd_Quality::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  if (cc4pibox->HABwdTracks.size()>0) return true;
  return false;
}
//**************************************************
bool numuCC4pi::HABwd_Veto::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  if (cc4pibox->HABwdTracks_Veto.size()>0) return true;
  return false;
}
//**************************************************
bool numuCC4pi::HABwd_PID::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  if (cc4pibox->HABwdTracks_PID.size()>0) return true;
  return false;
}
//**************************************************
bool numuCC4pi::HABwd::Apply(AnaEventC& event, ToyBoxB& box) const{
  //**************************************************
  (void)event;
  ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
  if (cc4pibox->HABwdTracks_PID.size()>0 && cc4pibox->FwdTracks_PID.size()==0 && cc4pibox->BwdTracks_PID.size()==0 && cc4pibox->HAFwdTracks_PID.size()==0) return true;
  return false;
}

//********************************************************************
int numuCC4pi_utils::GetFgdLayer(Float_t* pos){
  //********************************************************************

  Float_t initZ=0;
  Float_t finalZ=0;
  Float_t barCenter[30];

  for (Int_t i=0; i<30; i++) {
    barCenter[i] = DetDef::fgd1BarCenter[i];
    if (i==0) initZ = barCenter[0]-(DetDef::fgdXYBarWidth/2)-DetDef::fgdXYGlueWidth-DetDef::fgdXYSkinWidth-DetDef::fgdXYAirWidth;
    else initZ = finalZ;
    if      (i%2==0) finalZ = barCenter[i]+(DetDef::fgdXYBarWidth/2)+(DetDef::fgdXYMiddleGlueWidth/2);
    else if (i%2==1) finalZ = barCenter[i]+(DetDef::fgdXYBarWidth/2)+DetDef::fgdXYGlueWidth+DetDef::fgdXYSkinWidth+(DetDef::fgdXYAirWidth/2);
    if (i==29) finalZ += (DetDef::fgdXYAirWidth/2);
    if (pos[2]>initZ && pos[2]<finalZ) return i;
  }

  return -1;

}

//********************************************************************
bool numuCC4pi_utils::VetoCut(int topo, const AnaEventB& event, AnaTrackB& candidate){
  //********************************************************************

  if (topo==0) {
    AnaTrackB* VetoTrack = cutUtils::FindVetoTrack(event, candidate);
    if (VetoTrack) {
      if (VetoTrack->PositionStart[2]-candidate.PositionStart[2]<-100. && VetoTrack->Momentum/candidate.Momentum>0.8) return false;
    }
    return true;
  }
  if (topo==2) {
    AnaTrackB* VetoTrack = cutUtils::FindVetoTrack(event, candidate);
    if (VetoTrack) {
      if (VetoTrack->PositionStart[2]-candidate.PositionStart[2]<-150. && VetoTrack->Momentum/candidate.RangeMomentumMuon>0.9) return false;
    }
    return true;
  }
  if (topo==3) {
    AnaTrackB* VetoTrack = cutUtils::FindVetoTrack(event, candidate);
    if (VetoTrack) {
      if (VetoTrack->PositionStart[2]-candidate.PositionStart[2]<-400. && VetoTrack->Momentum/candidate.RangeMomentumMuon>0.9) return false;
    }
    return true;
  }
  return true;

}

//********************************************************************
bool numuCC4pi_utils::ExternalCut(int topo, const AnaEventC& event, AnaTrackB& candidate){
  //********************************************************************
  if ( topo==0 ) {
    if ( GetFgdLayer(candidate.PositionStart) > 27 ) {
      EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];
      int nFgdTracks=EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithFGD1AndNoTPC];
      for (Int_t i=0;i<nFgdTracks;i++){
        AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithFGD1AndNoTPC][i]);
        AnaFGDParticleB *fgdTrack = track->FGDSegments[0];
        if( !fgdTrack ) continue;
        if( SubDetId::GetDetectorUsed(fgdTrack->Detector,SubDetId::kFGD1) ) return false;
      }
    }
    return true;
  }
  return true;

}

//**************************************************
int numuCC4pi_utils::PIDCut(int topo, const AnaTrackB& candidate) {
  //**************************************************

  if ( topo==0 ) {
    if (cutUtils::MuonPIDCut(candidate, false)){
      if ( anaUtils::InFiducialVolume(SubDetId::kFGD2, candidate.PositionEnd, _FVdefminFGD2, _FVdefmaxFGD2) ) {
        if ( candidate.Momentum>280. ) return 2;
      }
      if ( candidate.nECALSegments>0 ) {
        if ( candidate.ECALSegments[0]->PIDMipEm>15 ) {
          if ( anaUtils::TrackUsesDet(candidate,SubDetId::kTECAL) ) return 3;
          if ( anaUtils::InFiducialVolume(SubDetId::kDSECAL, candidate.PositionEnd, numuCC4pi_utils::_FVdefminDsECal, numuCC4pi_utils::_FVdefmaxDsECal) ) return 3;
        }
      }
      return 1;
    }
    return 0;
  }
  else if ( topo==1 ) {
    if ( anaUtils::GetPIDLikelihood(candidate, 0, false) > 0.05 ) return 1;
    return 0;
  }
  else if ( topo==2 ) {
    if ( cutUtils::MuonECALPIDCut(candidate) ) return 1;
    return 0;
  }
  return 1;

}

//**************************************************
bool numuCC4piSelection::IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const{
  //**************************************************

  (void)event;
  (void)box;
  (void)systId;

  // Pile-up deals with an activity in the upstream TPC, the TPC-based BWD selection branch does not veto the upstream activity
  if (systId == SystId::kPileUp){
    if (branch == 1) return false; // TPC-based BWD case
  }


  return true;

}

//**************************************************
bool numuCC4piSelection::IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const{
  //**************************************************

  (void)event;
  (void)branch;
  (void)systId;

  AnaTrackB* track = static_cast<AnaTrackB*>(recObj);

  if (!track) return false;
  if (!track->TrueObject) return false;

  return true;

}

//**************************************************
bool numuCC4piSelection::IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
  //**************************************************

  (void)event;
  (void)branch;
  (void)systId;

  AnaTrueParticleB* trueTrack = static_cast<AnaTrueParticleB*>(trueObj);

  if (!trueTrack) return false;

  return true;

}

//**************************************************
bool numuCC4piSelection::IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const{
  //**************************************************

  (void)event;
  (void)branch;

  const ToyBoxCC4pi& cc4pibox = *static_cast<const ToyBoxCC4pi*>(&boxB);

  AnaTrackB* track = static_cast<AnaTrackB*>(recObj);

  if(systId == SystId::kTpcClusterEff){
    if (track->nTPCSegments == 0) return false;
    AnaTPCParticleB* tpcTrack = static_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInClosestTpc(*track));
    if (!tpcTrack) return false;
    if (tpcTrack->NNodes != 17 && tpcTrack->NNodes != 18) return false;
  }
  if(systId == SystId::kChargeIDEff){
    if (track != cc4pibox.MainTrack) return false;
  }
  if(systId == SystId::kTpcFgdMatchEff){
    if (track != cc4pibox.MainTrack) return false;
  }
  if(systId == SystId::kECalPID){
    if (branch==1) return false;
    else { if (track != cc4pibox.MainTrack) return false; }
  }

  return true;

}

//**************************************************
bool numuCC4piSelection::IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
  //**************************************************

  (void)event;
  (void)branch;

  const ToyBoxCC4pi& cc4pibox = *static_cast<const ToyBoxCC4pi*>(&boxB);

  AnaTrueParticleB* trueTrack = static_cast<AnaTrueParticleB*>(trueObj);

  if(systId == SystId::kTpcTrackEff){
    if (cc4pibox.MainTrack->GetTrueParticle()){
      if (trueTrack->PDG == 13 && cc4pibox.MainTrack->GetTrueParticle()->PDG!=13) return true;
      if (branch>1 && branch<4) return false;
      else { if (trueTrack->ID  == cc4pibox.MainTrack->GetTrueParticle()->ID) return true; }
    }
  }
  if(systId == SystId::kECalTrackEff){
    if (cc4pibox.MainTrack->GetTrueParticle()){
      if (trueTrack->PDG == 13 && cc4pibox.MainTrack->GetTrueParticle()->PDG!=13) return true;
      if (branch==2 || branch==3) { if (trueTrack->ID  == cc4pibox.MainTrack->GetTrueParticle()->ID) return true; }
      else return false;
    }
  }
  if(systId == SystId::kTpcP0dMatchEff){
    if (cc4pibox.MainTrack->GetTrueParticle()){
      if (branch==1) { if (trueTrack->ID  == cc4pibox.MainTrack->GetTrueParticle()->ID) return true; }
      else return false;
    }
  }
  if(systId == SystId::kTpcECalMatchEff){
    if (cc4pibox.MainTrack->GetTrueParticle()){
      if (branch>-1 && branch<6) { if (trueTrack->ID  == cc4pibox.MainTrack->GetTrueParticle()->ID) return true; }
      return false;
    }
  }
  if(systId == SystId::kFgdECalMatchEff){
    if (cc4pibox.MainTrack->GetTrueParticle()){
      if (branch==2 || branch==3) { if (trueTrack->ID  == cc4pibox.MainTrack->GetTrueParticle()->ID) return true; }
      else return false;
    }
  }

  return true;

}

//**************************************************
Int_t  numuCC4piSelection::GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  std::vector<std::vector<Int_t> > groups(SystId::SystEnumLast_SystId, std::vector<Int_t>(2, -1));
  groups.resize(SystId::SystEnumLast_SystId);
   
  
  // Explicitely set the groups for various systeamtics that need them, 
  // this allows to have a transparent set/control of the systematics by the
  // selection  

  using namespace SystId;
  using namespace anaUtils;
  // --- Systematic         Detectors                                   FGD1                                    FGD2 
  // General numuCC
  groups[   kBFieldDist                 ] = CreateVectorI( 2, EventBoxTracker::kTracksWithTPCWithStartOrEndInFGD1FV,          EventBoxTracker::kTracksWithTPCWithStartOrEndInFGD2FV           );
  groups[   kMomScale                   ] = CreateVectorI( 2, EventBoxTracker::kTracksWithTPCWithStartOrEndInFGD1FV,          EventBoxTracker::kTracksWithTPCWithStartOrEndInFGD2FV           );
  groups[   kMomResol                   ] = CreateVectorI( 2, EventBoxTracker::kTracksWithTPCWithStartOrEndInFGD1FV,          EventBoxTracker::kTracksWithTPCWithStartOrEndInFGD2FV           );
  groups[   kTpcPid                     ] = CreateVectorI( 2, EventBoxTracker::kTracksWithTPCWithStartOrEndInFGD1FV,          EventBoxTracker::kTracksWithTPCWithStartOrEndInFGD2FV           );
  groups[   kChargeIDEff                ] = CreateVectorI( 2, EventBoxTracker::kTracksWithTPCWithStartOrEndInFGD1FV,          EventBoxTracker::kTracksWithTPCWithStartOrEndInFGD2FV           );
  groups[   kTpcClusterEff              ] = CreateVectorI( 2, EventBoxTracker::kTracksWithTPCWithStartOrEndInFGD1FV,          EventBoxTracker::kTracksWithTPCWithStartOrEndInFGD2FV           ); 
  groups[   kTpcTrackEff                ] = CreateVectorI( 2, EventBoxTracker::kTracksWithTPC,                                EventBoxTracker::kTracksWithTPC                                 );
  if (versionUtils::prod6_systematics)
    groups[ kTpcFgdMatchEff             ] = CreateVectorI( 2, EventBoxTracker::kTracksWithTPCWithStartOrEndInFGD1FV,          EventBoxTracker::kTracksWithTPCWithStartOrEndInFGD2FV           );
  else
    groups[ kTpcFgdMatchEff             ] = CreateVectorI( 2, EventBoxTracker::kTracksWithTPCorFGD1,                          EventBoxTracker::kTracksWithTPCorFGD2                           );
  
  // 4pi specific
  groups[   kMomRangeResol              ] = CreateVectorI( 2, EventBoxTracker::kTracksWithFGD1AndTECALWithStartOrEndInFGD1FV, EventBoxTracker::kTracksWithFGD2AndTECALWithStartOrEndInFGD2FV  );
  groups[   kMomBiasFromVertexMigration ] = CreateVectorI( 2, EventBoxTracker::kTracksWithTPCWithStartOrEndInFGD1FV,          EventBoxTracker::kTracksWithTPCWithStartOrEndInFGD2FV  );
  groups[   kToFResol                   ] = CreateVectorI( 2, EventBoxTracker::kTracksWithFGD1WithStartOrEndInFGD1FV,         EventBoxTracker::kTracksWithFGD2WithStartOrEndInFGD2FV          );
  groups[   kECalEMResol                ] = CreateVectorI( 2, EventBoxTracker::kTracksWithECal,                               EventBoxTracker::kTracksWithECal                                );
  groups[   kECalEMScale                ] = CreateVectorI( 2, EventBoxTracker::kTracksWithECal,                               EventBoxTracker::kTracksWithECal                                );
  groups[   kECalTrackEff               ] = CreateVectorI( 2, EventBoxTracker::kTracksWithECal,                               EventBoxTracker::kTracksWithECal                                );
  groups[   kECalPID                    ] = CreateVectorI( 2, EventBoxTracker::kTracksWithECal,                               EventBoxTracker::kTracksWithECal                                );
  groups[   kTpcECalMatchEff            ] = CreateVectorI( 2, EventBoxTracker::kTracksWithTPCAndFGD1,                         EventBoxTracker::kTracksWithTPCAndFGD2                          );
  groups[   kTpcP0dMatchEff             ] = CreateVectorI( 2, EventBoxTracker::kTracksWithTPC,                                EventBoxTracker::kTracksWithTPC                                 );
  groups[   kFgdECalMatchEff            ] = CreateVectorI( 2, EventBoxTracker::kTracksWithFGD1AndTECALWithStartOrEndInFGD1FV, EventBoxTracker::kTracksWithFGD2AndTECALWithStartOrEndInFGD2FV  );
  groups[   kFgdECalSmrdMatchEff        ] = CreateVectorI( 2, EventBoxTracker::kTracksWithFGD1AndTECALWithStartOrEndInFGD1FV, EventBoxTracker::kTracksWithFGD2AndTECALWithStartOrEndInFGD2FV  );
  
  
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
  
  SubDetId_h det = GetDetectorFV(branch);
  
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
Int_t numuCC4piSelection::GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  // Consider the systematics specific for the analysis
  std::vector<std::vector<Int_t> > groups(SystId::SystEnumLast_SystId, std::vector<Int_t>(2, -1));
   
  
  // Explicitely set the groups for various systeamtics that need them, 
  // this allows to have a transparent set/control of the systematics by the
  // selection  

  using namespace SystId;
  using namespace anaUtils;
  
  // --- Systematic         Detectors                                   FGD1                                                    FGD2 
  groups[ kTpcTrackEff          ] = CreateVectorI( 2, EventBoxTracker::kTrueParticlesChargedInTPCorFGDInBunch,    EventBoxTracker::kTrueParticlesChargedInTPCorFGDInBunch   );
  groups[ kTpcFgdMatchEff       ] = CreateVectorI( 2, EventBoxTracker::kTrueParticlesChargedInTPCorFGDInBunch,    EventBoxTracker::kTrueParticlesChargedInTPCorFGDInBunch   );
  groups[ kTpcECalMatchEff      ] = CreateVectorI( 2, EventBoxTracker::kTrueParticlesChargedInTPCInBunch,         EventBoxTracker::kTrueParticlesChargedInTPCInBunch        );
  groups[ kTpcP0dMatchEff       ] = CreateVectorI( 2, EventBoxTracker::kTrueParticlesChargedInTPCInBunch,         EventBoxTracker::kTrueParticlesChargedInTPCInBunch        );
  groups[ kECalTrackEff         ] = CreateVectorI( 2, EventBoxTracker::kTrueParticlesInECalInBunch,               EventBoxTracker::kTrueParticlesInECalInBunch              );
  groups[ kFgdECalMatchEff      ] = CreateVectorI( 2, EventBoxTracker::kTrueParticlesChargedInFGD1ECalHAInBunch,  EventBoxTracker::kTrueParticlesChargedInFGD2ECalHAInBunch );
  groups[ kFgdECalSmrdMatchEff  ] = CreateVectorI( 2, EventBoxTracker::kTrueParticlesChargedInFGD1ECalHAInBunch,  EventBoxTracker::kTrueParticlesChargedInFGD2ECalHAInBunch );
  

  // The systematics not mentioned above will get no groups
  // Check the systematic index is ok
  try {
    groups.at(systId);
  }
  catch (const std::out_of_range& oor) {
    std::cerr << this->Name() << " GetRelevantTrueObjectGroupsForSystematic: syst index beyond limits "<< systId << " Error " << oor.what() << '\n';
    exit(1);
  }
  (void)branch;
  SubDetId_h det = GetDetectorFV();
  size_t ngroups = 0;
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

//********************************************************************
bool numuCC4piSelection::CheckRedoSelection(const AnaEventC& eventC, const ToyBoxB& PreviousToyBoxB, Int_t& redoFromStep){
  //********************************************************************

  (void)eventC;
  (void)PreviousToyBoxB;
  (void)redoFromStep;

  return true;

}

