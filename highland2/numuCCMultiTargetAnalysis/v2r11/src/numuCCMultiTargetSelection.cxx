#include "numuCCMultiTargetSelection.hxx"
#include "ND280AnalysisUtils.hxx"
#include "EventBoxUtils.hxx"
#include "EventBoxTracker.hxx"

bool _doMultiPiSelection = false;

//*******************************************************************************
numuCCMultiTargetSelection::numuCCMultiTargetSelection(bool doMultiPi, bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxTracker){
//*******************************************************************************

    _doMultiPiSelection = doMultiPi;
}

//*******************************************************************************
void numuCCMultiTargetSelection::DefineSteps(){
//*******************************************************************************

    AddStep(StepBase::kCut, "event quality", new EventQualityCut(), true);

    // Add a split to the trunk with 2 branches
    AddSplit(2);

    // ********* FGD1 ********* //
    AddStep(0,StepBase::kAction, "FGD1 FV", new SetDetectorFVAction(SubDetId::kFGD1));

    // Copy from numuCCSelection, branch 0 (trunk), steps from 1(second) to 10, into branch 0
    numuCCSelection _numuCCSelection;
    _numuCCSelection.Initialize();
    CopySteps(_numuCCSelection,0,1,10,0);

    if (_doMultiPiSelection){
      //Additional actions for the multi-pi selection in FGD1 (must to this before spliting)
      AddStep(0, StepBase::kAction, "fill_summary_FGD1", new FillSummaryAction_numuCCMultiPi());
      AddStep(0, StepBase::kAction, "find_pions_FGD1",   new FindPionsAction());
    }

    // Set the branch aliases
    SetBranchAlias(0,"numuCC_FGD1",      0);

    // ********* FGD2 ********* //
    // this is needed here to avoid that the box is replaced with the candidate in FGD2
    AddStep(1,StepBase::kCut, "> 0 tracks & no FGD1 tracks", new TotalMultiplicityFGD2Cut(), true);

    AddStep(1,StepBase::kAction, "FGD2 FV", new SetDetectorFVAction(SubDetId::kFGD2));

    // Copy from numuCCFGD2Selection, branch 0 (trunk), steps from 2(third) to 10, into branch 1
    numuCCFGD2Selection _numuCCFGD2Selection;
    _numuCCFGD2Selection.Initialize();
    CopySteps(_numuCCFGD2Selection,0,2,10,1);

    if (_doMultiPiSelection){
      // Additional actions for the multi-pi selection in FGD2
      AddStep(1, StepBase::kAction, "fill_summary_FGD2", new FillSummaryAction_numuCCMultiPiFGD2());
      AddStep(1, StepBase::kAction, "find_pions_FGD2",   new FindPionsAction());
    }

    // split in FGD2 into water and scint
    AddSplit(2,1);
    AddStep(1,0, StepBase::kCut, "numu CC in water FGD2", new WaterCut());
    AddStep(1,1, StepBase::kCut, "numu CC in scint FGD2", new ScintillatorCut());

    // Set the branch aliases
    SetBranchAlias(1,"numuCC_FGD2",      1);
    SetBranchAlias(2,"numuCC_FGD2-water",1,0);
    SetBranchAlias(3,"numuCC_FGD2-scint",1,1);

    if ( ! _doMultiPiSelection) return;


    // ******* MultiPi selection  ****** //

    // ********* FGD1 ********* //

    // multi-pi split for FGD1 (3 branches for branch 0,1 which correspond to branchID 2)
    AddSplit(3,0);
    AddStep(0,0, StepBase::kCut, "CC-0pi",   new NoPionCut());
    AddStep(0,1, StepBase::kCut, "CC-1pi",   new OnePionCut(true));
    AddStep(0,2, StepBase::kCut, "CC-Other", new OthersCut());

    // Set the branch aliases
    SetBranchAlias(4,"numuCC_FGD1_CC-0pi",  0,0);
    SetBranchAlias(5,"numuCC_FGD1_CC-1pi",  0,1);
    SetBranchAlias(6,"numuCC_FGD1_CC-Other",0,2);


    // ********* FGD2 ********* //

    // multi-pi split for FGD2 water (3 branches for branch 1,0 which correspond to branchID 4)
    AddSplit(3,1,0);
    AddStep(1,0,0, StepBase::kCut, "CC-0pi",   new NoPionCut());
    AddStep(1,0,1, StepBase::kCut, "CC-1pi",   new OnePionCut(true));
    AddStep(1,0,2, StepBase::kCut, "CC-Other", new OthersCut());

    // Set the branch aliases
    SetBranchAlias(7, "numuCC_FGD2-water_CC-0pi",  1,0,0);
    SetBranchAlias(8, "numuCC_FGD2-water_CC-1pi", 1,0,1);
    SetBranchAlias(9, "numuCC_FGD2-water_CC-Other",1,0,2);

    // multi-pi split for FGD2 scint (3 branches for branch 1,1 which correspond to branchID 5)
    AddSplit(3,1,1);
    AddStep(1,1,0, StepBase::kCut, "CC-0pi",   new NoPionCut());
    AddStep(1,1,1, StepBase::kCut, "CC-1pi",   new OnePionCut(true));
    AddStep(1,1,2, StepBase::kCut, "CC-Other", new OthersCut());

    // Set the branch aliases
    SetBranchAlias(10,"numuCC_FGD2-scint_CC-0pi",  1,1,0);
    SetBranchAlias(11,"numuCC_FGD2-scint_CC-1pi",  1,1,1);
    SetBranchAlias(12,"numuCC_FGD2-scint_CC-Other",1,1,2);


    // By default the preselection correspond to cuts 0-2
    SetPreSelectionAccumLevel(2);
}


//*******************************************************************************
void numuCCMultiTargetSelection::DefineDetectorFV(){
//*******************************************************************************

  SetDetectorFV(SubDetId::kFGD2);
  SetDetectorFV(SubDetId::kFGD1, 0);

  if (_doMultiPiSelection){
    SetDetectorFV(SubDetId::kFGD1, 4);
    SetDetectorFV(SubDetId::kFGD1, 5);
    SetDetectorFV(SubDetId::kFGD1, 6);
  }
}

//**************************************************
bool TotalMultiplicityFGD2Cut::Apply(AnaEventC& event, ToyBoxB& box) const{
//**************************************************

    (void)box;

    EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];

    // Check we have at least one reconstructed track in the TPC
    int nTPC=EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithTPC];
    if ( nTPC == 0) return false;

    // Cut if there are track starting in FGD1 FV
    AnaRecObjectC** selObjects = EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithTPC];
    for (Int_t i=0; i<nTPC; ++i) {
      AnaTrackB* track = static_cast<AnaTrackB*>(selObjects[i]);
      if ( ! track) continue;
      if (anaUtils::InFiducialVolume(SubDetId::kFGD1, track->PositionStart)) return false;
    }

    return true;
}


//*********************************************************************
bool WaterCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//*********************************************************************

    // cast the box to the appropriate type
    ToyBoxCCMultiPi& box = *static_cast<ToyBoxCCMultiPi*>(&boxB);

    // return false for the fake FGD1 cut
    if (anaUtils::InFiducialVolume(SubDetId::kFGD1,box.MainTrack->PositionStart)) return false;

    // GetFgdModuleType: 0 vert bars, 1 horiz bars, 2 water, 3 gaps (air, glue, G10, polypropilene)
    if (anaUtils::GetFgdModuleType(event.GetIsMC(), box.MainTrack->PositionStart, SubDetId::kFGD2)==0) return true;

    return false;
}


//*********************************************************************
bool ScintillatorCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//*********************************************************************

    // cast the box to the appropriate type
    ToyBoxCCMultiPi& box = *static_cast<ToyBoxCCMultiPi*>(&boxB);

    // return true for the fake FGD1 cut
    if (anaUtils::InFiducialVolume(SubDetId::kFGD1,box.MainTrack->PositionStart)) return true;

    // GetFgdModuleType: 0 vert bars, 1 horiz bars, 2 water, 3 gaps (air, glue, G10, polypropilene)
    if (anaUtils::GetFgdModuleType(event.GetIsMC(), box.MainTrack->PositionStart, SubDetId::kFGD2)==1) return true;

    return false;
}


//**************************************************
void numuCCMultiTargetSelection::InitializeEvent(AnaEventC& eventC){
//**************************************************

    AnaEventB& event = *static_cast<AnaEventB*>(&eventC); 

    // Create the appropriate EventBox if it does not exist yet
    if (!event.EventBoxes[EventBoxId::kEventBoxTracker])
        event.EventBoxes[EventBoxId::kEventBoxTracker] = new EventBoxTracker();

    boxUtils::FillTracksWithTPC(event,             SubDetId::kFGD1);
    boxUtils::FillTracksWithFGD(event,             SubDetId::kFGD1);
    boxUtils::FillTrajsChargedInFGDAndNoTPC(event, SubDetId::kFGD1);

    boxUtils::FillTracksWithTPC(event,             SubDetId::kFGD2);
    boxUtils::FillTracksWithFGD(event,             SubDetId::kFGD2);
    boxUtils::FillTrajsChargedInFGDAndNoTPC(event, SubDetId::kFGD2);

    boxUtils::FillTrajsChargedInTPC(event);
}


//*********************************************************************
bool SetDetectorFVAction::Apply(AnaEventC& event, ToyBoxB& box) const{
//*********************************************************************

    (void)event;

    box.DetectorFV = _detectorFV;
    return true;
}
