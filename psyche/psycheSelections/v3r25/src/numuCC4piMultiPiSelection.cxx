#include "numuCC4piMultiPiSelection.hxx"
#include "baseSelection.hxx"
#include "SystematicTuning.hxx"
#include "CutUtils.hxx"
#include "EventBoxUtils.hxx"
#include "Parameters.hxx"
#include "SubDetId.hxx"
#include "SystId.hxx"
#include "SystematicUtils.hxx"

//********************************************************************
numuCC4piMultiPiSelection::numuCC4piMultiPiSelection(bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxTracker) {
//********************************************************************

  // Initialize systematic tuning parameters
  systTuning::Initialize();
}

//********************************************************************
void numuCC4piMultiPiSelection::DefineSteps(){
//********************************************************************

  AddStep(StepBase::kCut,       "event quality",          new EventQualityCut(),      true); //if passed accum_level=1
  AddStep(StepBase::kCut,       "> 0 tracks ",            new numuCC4pi::TotalMultiplicityCut(), true); //if passed accum_level=2
  AddStep(StepBase::kAction,    "Sort TPC tracks",        new numuCC4pi::SortTracksAction());
  AddStep(StepBase::kCut,       "quality+fiducial",       new numuCC4pi::TrackGQandFVCut(),      true); //if passed accum_level=3
  AddStep(StepBase::kAction,    "veto Action",            new numuCC4pi::VetoAction());
  AddStep(StepBase::kAction,    "muon PID Action",        new numuCC4pi::PIDAction());
  AddStep(StepBase::kAction,    "find vertex",            new numuCC4pi::FindVertexAction());
  AddStep(StepBase::kAction,    "fill summary",           new FillSummaryAction_numuCC4piMultiPi());
  AddStep(StepBase::kAction,    "find_pions",             new numuCC4piMultiPi::FindPionsAction());

  AddSplit(4);

  AddStep(0, StepBase::kCut,    "Fwd Quality Cut",        new numuCC4pi::Fwd_Quality());         //if passed accum_level=4
  AddStep(0, StepBase::kCut,    "Fwd Veto Cut",           new numuCC4pi::Fwd_Veto());            //if passed accum_level=5       
  AddStep(0, StepBase::kCut,    "Fwd PID Cut",            new numuCC4pi::Fwd_PID());             //if passed accum_level=6
  AddStep(0, StepBase::kCut,    "Fwd 4piMultiPi Cut",     new numuCC4pi::Fwd());             //if passed accum_level=7	

  AddSplit(3,0);
  AddStep(0,0, StepBase::kCut,  "Fwd CC-0pi",             new numuCC4piMultiPi::NoPionCut());
  AddStep(0,0, StepBase::kCut,  "Fwd ECal Pi0 veto",      new numuCC4piMultiPi::EcalPi0VetoCut());

  AddStep(0,1, StepBase::kCut,  "Fwd CC-1pi",             new numuCC4piMultiPi::OnePionCut(true));
  AddStep(0,1, StepBase::kCut,  "Fwd ECal Pi0 veto",      new numuCC4piMultiPi::EcalPi0VetoCut());

  AddStep(0,2, StepBase::kCut,  "Fwd CC-Other",           new numuCC4piMultiPi::OthersCut());

  AddStep(1, StepBase::kCut,    "Bwd Quality Cut",        new numuCC4pi::Bwd_Quality());
  AddStep(1, StepBase::kCut,    "Bwd Veto Cut",           new numuCC4pi::Bwd_Veto());
  AddStep(1, StepBase::kCut,    "Bwd PID Cut",            new numuCC4pi::Bwd_PID());
  AddStep(1, StepBase::kCut,    "Bwd 4piMultiPi Cut",     new numuCC4pi::Bwd());

  AddSplit(3,1);
  AddStep(1,0, StepBase::kCut,  "Bwd CC-0pi",             new numuCC4piMultiPi::NoPionCut());
  AddStep(1,0, StepBase::kCut,  "Bwd ECal Pi0 veto",      new numuCC4piMultiPi::EcalPi0VetoCut());

  AddStep(1,1, StepBase::kCut,  "Bwd CC-1pi",             new numuCC4piMultiPi::OnePionCut(true));
  AddStep(1,1, StepBase::kCut,  "Bwd ECal Pi0 veto",      new numuCC4piMultiPi::EcalPi0VetoCut());

  AddStep(1,2, StepBase::kCut,  "Bwd CC-Other",           new numuCC4piMultiPi::OthersCut());

  AddStep(2, StepBase::kCut,    "HAFwd Quality Cut",      new numuCC4pi::HAFwd_Quality());
  AddStep(2, StepBase::kCut,    "HAFwd Veto Cut",         new numuCC4pi::HAFwd_Veto());
  AddStep(2, StepBase::kCut,    "HAFwd PID Cut",          new numuCC4pi::HAFwd_PID());
  AddStep(2, StepBase::kCut,    "HAFwd 4piMultiPi Cut",   new numuCC4pi::HAFwd());

  AddSplit(3,2);
  AddStep(2,0, StepBase::kCut,  "HAFwd CC-0pi",           new numuCC4piMultiPi::NoPionCut());
  AddStep(2,0, StepBase::kCut,  "HAFwd ECal Pi0 veto",    new numuCC4piMultiPi::EcalPi0VetoCut());

  AddStep(2,1, StepBase::kCut,  "HAFwd CC-1pi",           new numuCC4piMultiPi::OnePionCut(true));
  AddStep(2,1, StepBase::kCut,  "HAFwd ECal Pi0 veto",    new numuCC4piMultiPi::EcalPi0VetoCut());

  AddStep(2,2, StepBase::kCut,  "HAFwd CC-Other",         new numuCC4piMultiPi::OthersCut());

  AddStep(3, StepBase::kCut,    "HABwd Quality Cut",      new numuCC4pi::HABwd_Quality());
  AddStep(3, StepBase::kCut,    "HABwd Veto Cut",         new numuCC4pi::HABwd_Veto());
  AddStep(3, StepBase::kCut,    "HABwd PID Cut",          new numuCC4pi::HABwd_PID());
  AddStep(3, StepBase::kCut,    "HABwd 4piMultiPi Cut",   new numuCC4pi::HABwd());

  AddSplit(3,3);
  AddStep(3,0, StepBase::kCut,  "HABwd CC-0pi",           new numuCC4piMultiPi::NoPionCut());
  AddStep(3,0, StepBase::kCut,  "HABwd ECal Pi0 veto",    new numuCC4piMultiPi::EcalPi0VetoCut());

  AddStep(3,1, StepBase::kCut,  "HABwd CC-1pi",           new numuCC4piMultiPi::OnePionCut(true));
  AddStep(3,1, StepBase::kCut,  "HABwd ECal Pi0 veto",    new numuCC4piMultiPi::EcalPi0VetoCut());

  AddStep(3,2, StepBase::kCut, "HABwd CC-Other",          new numuCC4piMultiPi::OthersCut());

  SetBranchAlias(0, "Fwd CC0pi",    0, 0);
  SetBranchAlias(1, "Fwd CC1pi",    0, 1);
  SetBranchAlias(2, "Fwd CCOther",  0, 2);

  SetBranchAlias(3, "Bwd CC0pi",     1,0);
  SetBranchAlias(4, "Bwd CC1pi",     1,1);
  SetBranchAlias(5, "Bwd CCOther",   1,2);

  SetBranchAlias(6, "HAFwd CC0pi",   2,0);
  SetBranchAlias(7, "HAFwd CC1pi",   2,1);
  SetBranchAlias(8, "HAFwd CCOther", 2,2);

  SetBranchAlias(9,  "HABwd CC0pi",  3,0);
  SetBranchAlias(10, "HABwd CC1pi",  3,1);
  SetBranchAlias(11, "HABwd CCOther",3,2);

  SetPreSelectionAccumLevel(2);

}

//********************************************************************
void numuCC4piMultiPiSelection::DefineDetectorFV(){
  //********************************************************************

  // Change FV definition to take all thickness
  // Note! this will affect all downstream stuff
  FVDef::FVdefminFGD1[2] = 0;
  FVDef::FVdefmaxFGD1[2] = 0;

  // The detector in which the selection is applied
  SetDetectorFV(SubDetId::kFGD1);

}


//**************************************************
void numuCC4piMultiPiSelection::InitializeEvent(AnaEventC& eventBB){
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
  boxUtils::FillTrajsInECal(event);
  
  boxUtils::FillFGDMichelElectrons(event, (SubDetId::SubDetEnum)GetDetectorFV(),
      (bool)ND::params().GetParameterI("psycheSelections.numuCC4piMultiPi.Prod5Cuts"));

}

//********************************************************************
bool numuCC4piMultiPiSelection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
  //********************************************************************

  if(allCutsPassed[0])  static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD1NuMuCC0PiFwd;
  if(allCutsPassed[1])  static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD1NuMuCC1PiFwd;
  if(allCutsPassed[2])  static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD1NuMuCCOtherFwd;
  
  if(allCutsPassed[3])  static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD1NuMuCC0PiBwd;
  if(allCutsPassed[4])  static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD1NuMuCC1PiBwd;
  if(allCutsPassed[5])  static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD1NuMuCCOtherBwd;
  
  if(allCutsPassed[6])  static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD1NuMuCC0PiHAFwd;
  if(allCutsPassed[7])  static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD1NuMuCC1PiHAFwd;
  if(allCutsPassed[8])  static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD1NuMuCCOtherHAFwd;
  
  if(allCutsPassed[9])  static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD1NuMuCC0PiHABwd;
  if(allCutsPassed[10]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD1NuMuCC1PiHABwd;
  if(allCutsPassed[11]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD1NuMuCCOtherHABwd;

  return (static_cast<AnaEventSummaryB*>(event.Summary)->EventSample != SampleId::kUnassigned);
}

//********************************************************************
bool FillSummaryAction_numuCC4piMultiPi::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//********************************************************************

  // Cast the ToyBox to the appropriate type
  ToyBoxCC4piMultiPi* cc4piMultiPibox = static_cast<ToyBoxCC4piMultiPi*>(&boxB);

  if(!cc4piMultiPibox->MainTrack) return 1;
  AnaEventSummaryB* evesum = static_cast<AnaEventSummaryB*>(event.Summary);
  
  evesum->LeptonCandidate[SampleId::kFGD1NuMuCC0PiFwd    ] = cc4piMultiPibox->MainTrack;
  evesum->LeptonCandidate[SampleId::kFGD1NuMuCC1PiFwd    ] = cc4piMultiPibox->MainTrack;
  evesum->LeptonCandidate[SampleId::kFGD1NuMuCCOtherFwd  ] = cc4piMultiPibox->MainTrack;
  
  evesum->LeptonCandidate[SampleId::kFGD1NuMuCC0PiBwd    ] = cc4piMultiPibox->MainTrack;
  evesum->LeptonCandidate[SampleId::kFGD1NuMuCC1PiBwd    ] = cc4piMultiPibox->MainTrack;
  evesum->LeptonCandidate[SampleId::kFGD1NuMuCCOtherBwd  ] = cc4piMultiPibox->MainTrack;
  
  evesum->LeptonCandidate[SampleId::kFGD1NuMuCC0PiHAFwd  ] = cc4piMultiPibox->MainTrack;
  evesum->LeptonCandidate[SampleId::kFGD1NuMuCC1PiHAFwd  ] = cc4piMultiPibox->MainTrack;
  evesum->LeptonCandidate[SampleId::kFGD1NuMuCCOtherHAFwd] = cc4piMultiPibox->MainTrack;
  
  evesum->LeptonCandidate[SampleId::kFGD1NuMuCC0PiHABwd  ] = cc4piMultiPibox->MainTrack;
  evesum->LeptonCandidate[SampleId::kFGD1NuMuCC1PiHABwd  ] = cc4piMultiPibox->MainTrack;
  evesum->LeptonCandidate[SampleId::kFGD1NuMuCCOtherHABwd] = cc4piMultiPibox->MainTrack;
  
  anaUtils::CopyArray(cc4piMultiPibox->MainTrack->PositionStart, evesum->VertexPosition[SampleId::kFGD1NuMuCC0PiFwd    ], 4);
  anaUtils::CopyArray(cc4piMultiPibox->MainTrack->PositionStart, evesum->VertexPosition[SampleId::kFGD1NuMuCC1PiFwd    ], 4);
  anaUtils::CopyArray(cc4piMultiPibox->MainTrack->PositionStart, evesum->VertexPosition[SampleId::kFGD1NuMuCCOtherFwd  ], 4);
  
  anaUtils::CopyArray(cc4piMultiPibox->MainTrack->PositionStart, evesum->VertexPosition[SampleId::kFGD1NuMuCC0PiBwd    ], 4);
  anaUtils::CopyArray(cc4piMultiPibox->MainTrack->PositionStart, evesum->VertexPosition[SampleId::kFGD1NuMuCC1PiBwd    ], 4);
  anaUtils::CopyArray(cc4piMultiPibox->MainTrack->PositionStart, evesum->VertexPosition[SampleId::kFGD1NuMuCCOtherBwd  ], 4);
  
  anaUtils::CopyArray(cc4piMultiPibox->MainTrack->PositionStart, evesum->VertexPosition[SampleId::kFGD1NuMuCC0PiHAFwd  ], 4);
  anaUtils::CopyArray(cc4piMultiPibox->MainTrack->PositionStart, evesum->VertexPosition[SampleId::kFGD1NuMuCC1PiHAFwd  ], 4);
  anaUtils::CopyArray(cc4piMultiPibox->MainTrack->PositionStart, evesum->VertexPosition[SampleId::kFGD1NuMuCCOtherHAFwd], 4);
  
  anaUtils::CopyArray(cc4piMultiPibox->MainTrack->PositionStart, evesum->VertexPosition[SampleId::kFGD1NuMuCC0PiHABwd  ], 4);
  anaUtils::CopyArray(cc4piMultiPibox->MainTrack->PositionStart, evesum->VertexPosition[SampleId::kFGD1NuMuCC1PiHABwd  ], 4);
  anaUtils::CopyArray(cc4piMultiPibox->MainTrack->PositionStart, evesum->VertexPosition[SampleId::kFGD1NuMuCCOtherHABwd], 4);
  
  if(cc4piMultiPibox->MainTrack->GetTrueParticle()){
    evesum->TrueVertex[SampleId::kFGD1NuMuCC0PiFwd    ] = cc4piMultiPibox->MainTrack->GetTrueParticle()->TrueVertex;
    evesum->TrueVertex[SampleId::kFGD1NuMuCC1PiFwd    ] = cc4piMultiPibox->MainTrack->GetTrueParticle()->TrueVertex;
    evesum->TrueVertex[SampleId::kFGD1NuMuCCOtherFwd  ] = cc4piMultiPibox->MainTrack->GetTrueParticle()->TrueVertex;
    
    evesum->TrueVertex[SampleId::kFGD1NuMuCC0PiBwd    ] = cc4piMultiPibox->MainTrack->GetTrueParticle()->TrueVertex;
    evesum->TrueVertex[SampleId::kFGD1NuMuCC1PiBwd    ] = cc4piMultiPibox->MainTrack->GetTrueParticle()->TrueVertex;
    evesum->TrueVertex[SampleId::kFGD1NuMuCCOtherBwd  ] = cc4piMultiPibox->MainTrack->GetTrueParticle()->TrueVertex;
    
    evesum->TrueVertex[SampleId::kFGD1NuMuCC0PiHAFwd  ] = cc4piMultiPibox->MainTrack->GetTrueParticle()->TrueVertex;
    evesum->TrueVertex[SampleId::kFGD1NuMuCC1PiHAFwd  ] = cc4piMultiPibox->MainTrack->GetTrueParticle()->TrueVertex;
    evesum->TrueVertex[SampleId::kFGD1NuMuCCOtherHAFwd] = cc4piMultiPibox->MainTrack->GetTrueParticle()->TrueVertex;
    
    evesum->TrueVertex[SampleId::kFGD1NuMuCC0PiHABwd  ] = cc4piMultiPibox->MainTrack->GetTrueParticle()->TrueVertex;
    evesum->TrueVertex[SampleId::kFGD1NuMuCC1PiHABwd  ] = cc4piMultiPibox->MainTrack->GetTrueParticle()->TrueVertex;
    evesum->TrueVertex[SampleId::kFGD1NuMuCCOtherHABwd] = cc4piMultiPibox->MainTrack->GetTrueParticle()->TrueVertex;
  }

  return true;

}


//*********************************************************************
bool numuCC4piMultiPi::FindPionsAction::Apply(AnaEventC& event, ToyBoxB& box) const{
//*********************************************************************
  ToyBoxCC4piMultiPi* ccmultipibox = static_cast<ToyBoxCC4piMultiPi*>(&box);

  pionSelParams.refTrack = ccmultipibox->MainTrack;
  
  ccmultipibox->pionBox.Detector = (SubDetId::SubDetEnum)box.DetectorFV;
  
  // Fill the info
  cutUtils::FillPionInfo(event, ccmultipibox->pionBox, pionSelParams);

  int nnegpions        = ccmultipibox->pionBox.nNegativePionTPCtracks;
  int npospions        = ccmultipibox->pionBox.nPositivePionTPCtracks;
  int nisofgdpions     = ccmultipibox->pionBox.nIsoFGDPiontracks;
  int nmichelelectrons = ccmultipibox->pionBox.nMichelElectrons;
  int npi0             = ccmultipibox->pionBox.nPosPi0TPCtracks + ccmultipibox->pionBox.nElPi0TPCtracks;

  int pionFGD = nmichelelectrons;
  if (!nmichelelectrons && nisofgdpions > 0) pionFGD = 1;

  ccmultipibox->pionBox.nPosPions   = npospions + pionFGD;
  ccmultipibox->pionBox.nOtherPions = nnegpions + npi0;
  return true;

}

//**************************************************
bool numuCC4piMultiPiSelection::IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const{
  //**************************************************

  (void)event;
  (void)branch;
  (void)box;
  (void)systId;

  return true;

}

//**************************************************
bool numuCC4piMultiPiSelection::IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const{
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
bool numuCC4piMultiPiSelection::IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
  //**************************************************

  (void)event;
  (void)branch;
  (void)systId;

  AnaTrueParticleB* trueTrack = static_cast<AnaTrueParticleB*>(trueObj);

  if (!trueTrack) return false;

  return true;

}

//**************************************************
bool numuCC4piMultiPiSelection::IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const{
  //**************************************************

  (void)event;
  (void)branch;

  const ToyBoxCC4piMultiPi& cc4piMultiPibox = *static_cast<const ToyBoxCC4piMultiPi*>(&boxB); 

  AnaTrackB* track = static_cast<AnaTrackB*>(recObj);

  if(systId == SystId::kTpcClusterEff){
    if (track->nTPCSegments > 0) {
      AnaTPCParticleB* tpcTrack = static_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInClosestTpc(*track));
      if (tpcTrack) {
        if (tpcTrack->NNodes > 16 && tpcTrack->NNodes < 19) return true;
      }
    }
    return false;
  }
  if(systId == SystId::kChargeIDEff){
    if (track == cc4piMultiPibox.MainTrack) return true;
    return false;
  }
  if(systId == SystId::kTpcFgdMatchEff){
    if (track == cc4piMultiPibox.MainTrack) return true;
    return false;
  }
  if(systId == SystId::kECalPID){
    if (track == cc4piMultiPibox.MainTrack) {
      if (branch>6 && branch<12) return true;
      else if (branch==0 || branch==1 || branch==2) {
        if ( anaUtils::TrackUsesDet(*track,SubDetId::kTECAL) ) return true;
        if ( anaUtils::InFiducialVolume(SubDetId::kDSECAL, track->PositionEnd, numuCC4pi_utils::_FVdefminDsECal, numuCC4pi_utils::_FVdefmaxDsECal) ) return true;
      }
    }
    return false;
  }

  return true;

}

//**************************************************
bool numuCC4piMultiPiSelection::IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
  //**************************************************

  (void)event;
  (void)branch;

  const ToyBoxCC4piMultiPi& cc4piMultiPibox = *static_cast<const ToyBoxCC4piMultiPi*>(&boxB);

  AnaTrueParticleB* trueTrack = static_cast<AnaTrueParticleB*>(trueObj);

  if ( !cc4piMultiPibox.MainTrack->GetTrueParticle() ) return false;

  if(systId == SystId::kTpcTrackEff){
    if (trueTrack->PDG == 13 && cc4piMultiPibox.MainTrack->GetTrueParticle()->PDG!=13) return true;
    if (branch<6 && branch>-1) { if (trueTrack->ID  == cc4piMultiPibox.MainTrack->GetTrueParticle()->ID) return true; }
    return false;
  }
  if(systId == SystId::kECalTrackEff){
    if (trueTrack->PDG == 13 && cc4piMultiPibox.MainTrack->GetTrueParticle()->PDG!=13) return true;
    if (branch>6 && branch<12) { if (trueTrack->ID  == cc4piMultiPibox.MainTrack->GetTrueParticle()->ID) return true; }
    return false;
  }
  if(systId == SystId::kTpcP0dMatchEff){
    if (branch==3 || branch==4 || branch==5) { if (trueTrack->ID  == cc4piMultiPibox.MainTrack->GetTrueParticle()->ID) return true; }
    return false;
  }
  if(systId == SystId::kTpcECalMatchEff){
    if (branch>-1 && branch<12) { if (trueTrack->ID  == cc4piMultiPibox.MainTrack->GetTrueParticle()->ID) return true; }
    return false;
  }
  if(systId == SystId::kFgdECalMatchEff){
    if (branch>6 && branch<12) { if (trueTrack->ID  == cc4piMultiPibox.MainTrack->GetTrueParticle()->ID) return true; }
    return false;
  }
  if(systId == SystId::kFgdECalSmrdMatchEff){
    if (branch>6 && branch<12) { if (trueTrack->ID  == cc4piMultiPibox.MainTrack->GetTrueParticle()->ID) return true; }
    return false;
  }
  if(systId == SystId::kSIPion){
    // If this trueTrack is associated to the MainTrack
    if (trueTrack->ID == cc4piMultiPibox.MainTrack->GetTrueParticle()->ID){
      if (abs(cc4piMultiPibox.MainTrack->GetTrueParticle()->PDG)        == 211) return true;
      if (abs(cc4piMultiPibox.MainTrack->GetTrueParticle()->ParentPDG)  == 211) return true;
      if (abs(cc4piMultiPibox.MainTrack->GetTrueParticle()->GParentPDG) == 211) return true;
      return false;
    }
    // If this trueTrack is NOT associated to the MainTrack, consider the posibility of this trueTrack to become the MainTrack and be identified as muon
    // For the moment assume a negative pion may become the MainTrack if its momentum its above 90% of the current MainTrack momentum. 
    // Ideally we should check that this true pion is not associated to any reconstructed track, but this is not possible now without looping.
    // Multiply by the charge of the MainTrack such that it can be use for antiNumu selection
    if (trueTrack->PDG == 211*((Int_t)cc4piMultiPibox.MainTrack->Charge) && trueTrack->Momentum > 0.9*cc4piMultiPibox.MainTrack->Momentum) return true;
    return false;
  }
  if(systId == SystId::kSIProton){
    // If this trueTrack is associated to the MainTrack
    if (trueTrack->ID == cc4piMultiPibox.MainTrack->GetTrueParticle()->ID){
      if (cc4piMultiPibox.MainTrack->GetTrueParticle()->PDG        == 2212) return true;
      if (cc4piMultiPibox.MainTrack->GetTrueParticle()->ParentPDG  == 2212) return true;
      if (cc4piMultiPibox.MainTrack->GetTrueParticle()->GParentPDG == 2212) return true;
      return false;
    }
    // If this trueTrack is NOT associated to the MainTrack, consider the posibility of this trueTrack to become the MainTrack
    if (trueTrack->PDG == 2212 && trueTrack->Momentum > 0.9*cc4piMultiPibox.MainTrack->Momentum) return true;
    return false;
  }

  return true;

}

//********************************************************************
bool numuCC4piMultiPiSelection::CheckRedoSelection(const AnaEventC& eventC, const ToyBoxB& PreviousToyBoxB, Int_t& redoFromStep){
  //********************************************************************

  (void)eventC;
  (void)PreviousToyBoxB;
  redoFromStep = 1;

  return true;

}
//********************************************************************
const multipi::MultiPiBox& numuCC4piMultiPi::GetPionBox(const ToyBoxB& box){
//********************************************************************
  const ToyBoxCC4piMultiPi& ccmultipibox = static_cast<const ToyBoxCC4piMultiPi&>(box);
  return ccmultipibox.pionBox;  
}


