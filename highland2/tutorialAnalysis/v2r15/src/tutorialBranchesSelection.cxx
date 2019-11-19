#include "tutorialBranchesSelection.hxx"
#include "tutorialSelection.hxx"
#include "baseSelection.hxx"
#include "CutUtils.hxx"
#include "Parameters.hxx"
#include "SubDetId.hxx"
#include "EventBoxUtils.hxx"

//*******************************************************************************
tutorialBranchesSelection::tutorialBranchesSelection(bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxTracker){
//*******************************************************************************
  // Fill smth if needed
    
}

//*******************************************************************************
void tutorialBranchesSelection::DefineDetectorFV(){
//*******************************************************************************

    // Set the detector Fiducial Volume in which the selection is applied. This is now a mandatory method
    // If different branches use different FV you can specify the branch with SetDetectorFV(SubDetId::kFGD1, branch).
    // Have a look at numuCCMultiTargetAnalysis for an example
    SetDetectorFV(SubDetId::kFGD1);
}

//*******************************************************************************
void tutorialBranchesSelection::DefineSteps(){
//*******************************************************************************

    // Copy all steps from the tutorialSelection
    tutorialSelection tut;
    CopySteps(tut);

    // Add an action to the trunk
    AddStep(StepBase::kAction, "find long tpc tracks", new  FindLongTPCTracks());

    //Add a split to the trunk with 4 branches.
    AddSplit(4);

    //First branch is for 1-track events. We add now a cut to branch 0
    AddStep(0, StepBase::kCut, "1-track", new  OneTPCTrackCut());

    //Second branch is for 2-track events. We add now a cut to branch 1
    AddStep(1, StepBase::kCut, "2-tracks", new TwoTPCTracksCut());

    //Third branch is for >2 track events. We add now a cut to branch 2
    AddStep(2, StepBase::kCut, ">2-tracks", new MoreThanTwoTPCTracksCut());

    //Forth branch is for >2 track events in which the two tracks have more than 65 nodes. We add now a cut to branch 3
    AddStep(3, StepBase::kCut, ">2 long-tracks", new MoreThanTwoLongTPCTracksCut());


    // Set the branch aliases to the three branches (this is mandatory)
    // First integer is the branch number at each split. Since we have only one explit there is a single number (0, 1, 2)
    // Second integer is the alias number (0, 1, 2) to be used in the DrawingTools when refering to a specific branch   
    SetBranchAlias(0,"1-track",       0);
    SetBranchAlias(1,"2-tracks",      1);
    SetBranchAlias(2,">2-tracks",     2);
    SetBranchAlias(3,">2 long-tracks",3);

    // By default the preselection correspond to cuts 0-2. 
    // It means that if any of the three first cuts (0,1,2) is not passed 
    // the loop over toys will be broken ===> A single toy will be run
    SetPreSelectionAccumLevel(2);
}

//********************************************************************
bool tutorialBranchesSelection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
//********************************************************************

    //CC0pi
    if(allCutsPassed[0]){
        static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD1NuMuCC0Pi;
    }  
    //CC1pi
    else if (allCutsPassed[1]){
        static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD1NuMuCC1Pi;
    }
    //CCOther
    else if (allCutsPassed[2]){
        static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD1NuMuCCOther;
    }

    // otherwise kUnassigned is used from the EventSummary constructor
    return (static_cast<AnaEventSummaryB*>(event.Summary)->EventSample != SampleId::kUnassigned);
}


//*********************************************************************
bool FillSummaryAction_tutorialBranches::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//*********************************************************************

    // cast the box to the appropriate type
    ToyBoxTutorial& box = *static_cast<ToyBoxTutorial*>(&boxB);

    if(!box.HMNtrack) return 1;

    static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD1NuMuCC0Pi] = box.HMNtrack;
    static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD1NuMuCC1Pi] = box.HMNtrack;
    static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD1NuMuCCOther] = box.HMNtrack;

    for(int i = 0; i < 4; ++i){
        static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD1NuMuCC0Pi][i] = box.HMNtrack->PositionStart[i];
        static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD1NuMuCC1Pi][i] = box.HMNtrack->PositionStart[i];
        static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD1NuMuCCOther][i] = box.HMNtrack->PositionStart[i];
    }
    if(box.HMNtrack->GetTrueParticle()){
        static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD1NuMuCC0Pi] = box.HMNtrack->GetTrueParticle()->TrueVertex;
        static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD1NuMuCC1Pi] = box.HMNtrack->GetTrueParticle()->TrueVertex;
        static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD1NuMuCCOther] = box.HMNtrack->GetTrueParticle()->TrueVertex;
    }

    return 1;
}

//*********************************************************************
bool OneTPCTrackCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//*********************************************************************

    (void)event;

    // cast the box to the appropriate type
    ToyBoxTutorial& box = *static_cast<ToyBoxTutorial*>(&boxB);


    if (box.nNegativeTPCtracks+box.nPositiveTPCtracks==1)
      return true;
    else
      return false; 
}


//*********************************************************************
bool TwoTPCTracksCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//*********************************************************************

    (void)event;

    // cast the box to the appropriate type
    ToyBoxTutorial& box = *static_cast<ToyBoxTutorial*>(&boxB);


    if (box.nNegativeTPCtracks+box.nPositiveTPCtracks==2)
      return true;
    else
      return false; 
}


//*********************************************************************
bool MoreThanTwoTPCTracksCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//*********************************************************************

    (void)event;

    // cast the box to the appropriate type
    ToyBoxTutorial& box = *static_cast<ToyBoxTutorial*>(&boxB);


    if (box.nNegativeTPCtracks+box.nPositiveTPCtracks>2)
      return true;
    else
      return false; 
}

//*********************************************************************
bool MoreThanTwoLongTPCTracksCut::Apply(AnaEventC& event, ToyBoxB& box) const{
//*********************************************************************

    (void)event;

    // Cast the box such that we can access nLongTPCTracks, not present in ToyBoxB
    const ToyBoxTutorial* tutBox = static_cast<const ToyBoxTutorial*>(&box);

    if (tutBox->nLongTPCTracks>2)
      return true;
    else
      return false; 
}

//*********************************************************************
bool FindLongTPCTracks::Apply(AnaEventC& eventC, ToyBoxB& box) const{
//*********************************************************************
  
    AnaEventB& event = *static_cast<AnaEventB*>(&eventC); 
    
    // Get all tracks using the TPC
    EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];
    AnaRecObjectC** selTracks = EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithTPC];
    int nTPC=EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithTPC];
    
    // Cast the box such that we can access nLongTPCTracks, not present in ToyBoxB
    ToyBoxTutorial* tutBox = static_cast<ToyBoxTutorial*>(&box);
    
    // reset to 0 
    tutBox->nLongTPCTracks=0;
    
    //loop over tpc tracks
    for (Int_t i=0;i<nTPC; ++i){
      AnaTrackB* track = static_cast<AnaTrackB*>(selTracks[i]);
      
      // Apply the fiducial cut
      if (!cutUtils::FiducialCut(*track, SubDetId::kFGD1)){
        continue;
      }
      
      // Gets the longest segment in the closest TPC
      AnaParticleB* TPCSegment = anaUtils::GetSegmentWithMostNodesInClosestTpc(*track);
      
      // Apply the nodes cut
      if (TPCSegment->NNodes>65) tutBox->nLongTPCTracks++;
    }  
    
    return true;
}

//**************************************************
void tutorialBranchesSelection::InitializeEvent(AnaEventC& eventC){
//**************************************************

    AnaEventB& event = *static_cast<AnaEventB*>(&eventC); 

    // Create the appropriate EventBox if it does not exist yet
    if (!event.EventBoxes[EventBoxId::kEventBoxTracker])
      event.EventBoxes[EventBoxId::kEventBoxTracker] = new EventBoxTracker();
    
    boxUtils::FillTracksWithTPC(event,             static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
    boxUtils::FillTracksWithFGD(event,             static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
    boxUtils::FillTrajsChargedInTPC(event);
    boxUtils::FillTrajsChargedInFGDAndNoTPC(event, static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
}


