
#include "numuCC4piFGD2Selection.hxx"
#include "baseSelection.hxx"
#include "SystematicTuning.hxx"
#include "CutUtils.hxx"
#include "EventBoxUtils.hxx"
#include "Parameters.hxx"
#include "SubDetId.hxx"
#include "SystId.hxx"
#include "SystematicUtils.hxx"


//********************************************************************
numuCC4piFGD2Selection::numuCC4piFGD2Selection(bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxTracker) {
    //********************************************************************
    
    // Initialize systematic tuning parameters
    systTuning::Initialize();
}

//********************************************************************
void numuCC4piFGD2Selection::DefineSteps(){
    //********************************************************************
    
    AddStep(StepBase::kCut,    "event quality",       new EventQualityCut(),                    true); //if passed accum_level=1
    AddStep(StepBase::kCut,    "> 0 tracks ",         new numuCC4piFGD2::TotalMultiplicityCut(), true); //if passed accum_level=2
    AddStep(StepBase::kAction, "Sort TPC tracks",     new numuCC4piFGD2::SortTracksAction());
    AddStep(StepBase::kCut,    "quality+fiducial",    new numuCC4piFGD2::TrackGQandFVCut(),      true); //if passed accum_level=3
    AddStep(StepBase::kAction, "veto Action",         new numuCC4piFGD2::VetoAction());
    AddStep(StepBase::kAction, "muon PID Action",     new numuCC4piFGD2::PIDAction());
    AddStep(StepBase::kAction, "find vertex",         new numuCC4pi::FindVertexAction());
    AddStep(StepBase::kAction, "fill summary",        new FillSummaryAction_numuCC4piFGD2());
    
    AddSplit(4);
    
    AddStep(0, StepBase::kCut, "Fwd Quality Cut",     new numuCC4pi::Fwd_Quality());         //if passed accum_level=4
    AddStep(0, StepBase::kCut, "Fwd Veto Cut",        new numuCC4pi::Fwd_Veto());            //if passed accum_level=5
    AddStep(0, StepBase::kCut, "Fwd PID Cut",         new numuCC4pi::Fwd_PID());             //if passed accum_level=6
    AddStep(0, StepBase::kCut, "Fwd 4pi Cut",         new numuCC4pi::Fwd());                 //if passed accum_level=7
    
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
void numuCC4piFGD2Selection::DefineDetectorFV(){
    //********************************************************************
    
    // Change FV definition to take all thickness
    // Note! this will affect all downstream stuff
    FVDef::FVdefminFGD2[2] = 0;
    FVDef::FVdefmaxFGD2[2] = 0;
    
    // The detector in which the selection is applied
    SetDetectorFV(SubDetId::kFGD2);
    
}

//**************************************************
void numuCC4piFGD2Selection::InitializeEvent(AnaEventC& eventBB){
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
bool numuCC4piFGD2Selection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
    //********************************************************************
    
    if(allCutsPassed[0]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD2NuMuCC;
    if(allCutsPassed[1]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD2NuMuCC;
    if(allCutsPassed[2]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD2NuMuCC;
    if(allCutsPassed[3]) static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD2NuMuCC;
    
    return (static_cast<AnaEventSummaryB*>(event.Summary)->EventSample != SampleId::kFGD2NuMuCC);
}

//**************************************************
bool numuCC4piFGD2::TotalMultiplicityCut::Apply(AnaEventC& event, ToyBoxB& box) const{
    //**************************************************
    
    (void)box;
    
    // Check we have at least one reconstructed track in the FGD
    EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];
    return (EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithFGD2]>0);
    
}

//**************************************************
bool numuCC4piFGD2::SortTracksAction::Apply(AnaEventC& event, ToyBoxB& box) const{
    //**************************************************
    
    ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
    
    // Retrieve the EventBoxTracker
    EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];
    
    //Find TPCGoodQuality tracks in Fwd and Bwd
    int nTPC=EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithGoodQualityTPCInFGD2FV];
    for (Int_t i=0;i<nTPC; ++i){
        AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithGoodQualityTPCInFGD2FV][i]);
        if ( track->Charge!=-1 ) continue;
        cc4pibox->LowAngle.push_back(track);
    }
    // Sort TPCGoodQuality using Momentum based method
    std::sort(cc4pibox->LowAngle.begin(), cc4pibox->LowAngle.end(), AnaTrackB::CompareMomentum);
    
    int nECALTracks=EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithECal];
    for(int i=0;i<nECALTracks;i++){
        AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithECal][i]);
        if ( anaUtils::InFiducialVolume(SubDetId::kFGD2, track->PositionStart) ) {
			if (track->PositionStart[2]-track->PositionEnd[2] > 0) std::cout<<track->PositionStart[2]-track->PositionEnd[2]<<std::endl;
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
bool numuCC4piFGD2::TrackGQandFVCut::Apply(AnaEventC& event, ToyBoxB& box) const{
    //**************************************************

    (void)event;
    
    ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
    
    for(UInt_t i=0;i<cc4pibox->LowAngle.size();i++ ){
        if ( anaUtils::InFiducialVolume(SubDetId::kFGD2, cc4pibox->LowAngle[i]->PositionStart, numuCC4piFGD2_utils::LAFVmin, numuCC4piFGD2_utils::LAFVmax) ){
            if ( ToFSenseCorrector::IsForward(*cc4pibox->LowAngle[i]) ) cc4pibox->FwdTracks.push_back(cc4pibox->LowAngle[i]);
            else cc4pibox->BwdTracks.push_back(cc4pibox->LowAngle[i]);
            break;
        }
    }
    
    for(UInt_t i=0;i<cc4pibox->HighAngle.size();i++){
        if ( anaUtils::InFiducialVolume(SubDetId::kFGD2, cc4pibox->HighAngle[i]->PositionStart, numuCC4piFGD2_utils::HAFVmin, numuCC4piFGD2_utils::HAFVmax) ) {
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
bool numuCC4piFGD2::VetoAction::Apply(AnaEventC& event, ToyBoxB& box) const{
    //**************************************************
    
    AnaEventB& eventB = *static_cast<AnaEventB*>(&event);
    
    ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
    
    for (UInt_t i=0;i<cc4pibox->FwdTracks.size();i++){
        if ( numuCC4pi_utils::VetoCut(0,eventB,*(cc4pibox->FwdTracks[i])) ) {
            if ( numuCC4piFGD2_utils::ExternalCut(0,event,*(cc4pibox->FwdTracks[i])) ) cc4pibox->FwdTracks_Veto.push_back(cc4pibox->FwdTracks[i]);
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
bool numuCC4piFGD2::PIDAction::Apply(AnaEventC& event, ToyBoxB& box) const{
    //**************************************************
    
    (void)event;
    
    ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&box);
    
    for (UInt_t i=0;i<cc4pibox->FwdTracks_Veto.size();i++){
        if ( numuCC4piFGD2_utils::PIDCut(0,*(cc4pibox->FwdTracks_Veto[i]))==1 ) cc4pibox->FwdTracks_PID.push_back(cc4pibox->FwdTracks_Veto[i]);
    }
    
    for (UInt_t i=0;i<cc4pibox->BwdTracks_Veto.size();i++){
        if ( numuCC4piFGD2_utils::PIDCut(1,*(cc4pibox->BwdTracks_Veto[i]))==1 ) cc4pibox->BwdTracks_PID.push_back(cc4pibox->BwdTracks_Veto[i]);
    }
    
    for (UInt_t i=0;i<cc4pibox->HAFwdTracks_Veto.size();i++){
        if ( numuCC4piFGD2_utils::PIDCut(2,*(cc4pibox->HAFwdTracks_Veto[i]))==1 ) cc4pibox->HAFwdTracks_PID.push_back(cc4pibox->HAFwdTracks_Veto[i]);
    }
    
    for (UInt_t i=0;i<cc4pibox->HABwdTracks_Veto.size();i++){
        if ( numuCC4piFGD2_utils::PIDCut(2,*(cc4pibox->HABwdTracks_Veto[i]))==1 ) cc4pibox->HABwdTracks_PID.push_back(cc4pibox->HABwdTracks_Veto[i]);
    }

    for (UInt_t i=0;i<cc4pibox->FwdTracks_Veto.size();i++){
        if ( numuCC4piFGD2_utils::PIDCut(0,*(cc4pibox->FwdTracks_Veto[i]))==3 ) cc4pibox->CSECALTracks_PID.push_back(cc4pibox->FwdTracks_Veto[i]);
    }
    
    if      ( cc4pibox->FwdTracks_PID.size()>0    ) cc4pibox->MainTrack = cc4pibox->FwdTracks_PID[0];
    else if ( cc4pibox->BwdTracks_PID.size()>0    ) cc4pibox->MainTrack = cc4pibox->BwdTracks_PID[0];
    else if ( cc4pibox->HAFwdTracks_PID.size()>0  ) cc4pibox->MainTrack = cc4pibox->HAFwdTracks_PID[0];
    else if ( cc4pibox->HABwdTracks_PID.size()>0  ) cc4pibox->MainTrack = cc4pibox->HABwdTracks_PID[0];
    
    return true;
    
}

//********************************************************************
bool FillSummaryAction_numuCC4piFGD2::Apply(AnaEventC& event, ToyBoxB& boxB) const{
    //********************************************************************
    
    // Cast the ToyBox to the appropriate type
    ToyBoxCC4pi* cc4pibox = static_cast<ToyBoxCC4pi*>(&boxB);
    
    if(!cc4pibox->MainTrack) return 1;
    
    static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD2NuMuCC] = cc4pibox->MainTrack;
    
    anaUtils::CopyArray( cc4pibox->MainTrack->PositionStart, static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD2NuMuCC], 4);
    
    if(cc4pibox->MainTrack->GetTrueParticle()) static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD2NuMuCC] = cc4pibox->MainTrack->GetTrueParticle()->TrueVertex;
    
    return true;
}

//********************************************************************
bool numuCC4piFGD2_utils::ExternalCut(int topo, const AnaEventC& event, AnaTrackB& candidate){
    //********************************************************************
    if ( topo==0 ) {
        if ( numuCC4pi_utils::GetFgdLayer(candidate.PositionStart) > 27 ) {
            EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];
            int nFgdTracks=EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithFGD2AndNoTPC];
            for (Int_t i=0;i<nFgdTracks;i++){
                AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithFGD2AndNoTPC][i]);
                AnaFGDParticleB *fgdTrack = track->FGDSegments[0];
                if( !fgdTrack ) continue;
                if( SubDetId::GetDetectorUsed(fgdTrack->Detector,SubDetId::kFGD2) ) return false;
            }
        }
        return true;
    }
    return true;
    
}

//**************************************************
int numuCC4piFGD2_utils::PIDCut(int topo, const AnaTrackB& candidate) {
    //**************************************************
    
    if ( topo==0 ) {
        if (cutUtils::MuonPIDCut(candidate, false)){
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
bool numuCC4piFGD2Selection::IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const{
    //**************************************************
    
    (void)event;
    (void)branch;
    (void)box;
    (void)systId;
    
    return true;
    
}

//**************************************************
bool numuCC4piFGD2Selection::IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const{
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
bool numuCC4piFGD2Selection::IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
    //**************************************************
    
    (void)event;
    (void)branch;
    (void)systId;
    
    AnaTrueParticleB* trueTrack = static_cast<AnaTrueParticleB*>(trueObj);
    
    if (!trueTrack) return false;
    
    return true;
    
}

//**************************************************
bool numuCC4piFGD2Selection::IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const{
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
bool numuCC4piFGD2Selection::IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
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

//********************************************************************
bool numuCC4piFGD2Selection::CheckRedoSelection(const AnaEventC& eventC, const ToyBoxB& PreviousToyBoxB, Int_t& redoFromStep){
    //********************************************************************
    
    (void)eventC;
    (void)PreviousToyBoxB;
    (void)redoFromStep;
    
    return true;
    
}

