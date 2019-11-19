#include "baseSelection.hxx"
#include "p0dNumuCCQESelection.hxx"
#include "p0dNumuCCSelection.hxx"
#include "CutUtils.hxx"
#include "EventBoxUtils.hxx"

//********************************************************************
p0dNumuCCQESelection::p0dNumuCCQESelection(bool forceBreak): SelectionBase(forceBreak, EventBoxId::kEventBoxTracker) {
//********************************************************************

}

//********************************************************************
void p0dNumuCCQESelection::DefineSteps(){
//********************************************************************
  p0dNumuCCSelection p0dnumucc;
  CopySteps(p0dnumucc);
  
  //Add a split to the trunk with 2 branches. One for single p0d
  //selection and the other for multiple. This is necessary to get the
  //weight systs as ApplyWeightSystematic is only called if a branch
  //passes.
  AddSplit(2);

  // QE selection
  AddStep(0, StepBase::kCut, "single p0d pid", new SingleP0DPIDCut());
  // Other selection. Need this additional "cut" or else the selection crashes
  AddStep(1, StepBase::kCut, "multiple p0d pid", new MultipleP0DPIDCut());

  SetBranchAlias(0,"Single-P0D", 0);
  SetBranchAlias(1,"Multiple-P0D", 1);
}

//********************************************************************
void p0dNumuCCQESelection::DefineDetectorFV(){
//********************************************************************

  SetDetectorFV(SubDetId::kP0D);
}

//********************************************************************
bool p0dNumuCCQESelection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
//********************************************************************

    if(allCutsPassed[0]){
        static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kP0DNuMuCC;
    }
    return (static_cast<AnaEventSummaryB*>(event.Summary)->EventSample != SampleId::kUnassigned);
}

//**************************************************
void p0dNumuCCQESelection::InitializeEvent(AnaEventC& eventC){
//**************************************************

  AnaEventB& event = *static_cast<AnaEventB*>(&eventC); 

  // Create the appropriate EventBox if it does not exist yet
  if (!event.EventBoxes[EventBoxId::kEventBoxTracker])
    event.EventBoxes[EventBoxId::kEventBoxTracker] = new EventBoxTracker();

  boxUtils::FillTracksWithTPC(event,             static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
  boxUtils::FillTracksWithP0D(event);
  boxUtils::FillTrajsChargedInTPC(event);
  boxUtils::FillTrajsChargedInP0D(event);


}

//**************************************************
bool SingleP0DPIDCut::Apply(AnaEventC& eventC, ToyBoxB& box) const{
//**************************************************

  (void)box;

  AnaEventB& event = *static_cast<AnaEventB*>(&eventC); 

  // Get all tracks using P0D
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];
  int nP0D=EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithP0D];

  return (nP0D == 1);
}

//**************************************************
bool MultipleP0DPIDCut::Apply(AnaEventC& eventC, ToyBoxB& box) const{
//**************************************************

  (void)box;

  AnaEventB& event = *static_cast<AnaEventB*>(&eventC); 

  // Get all tracks using P0D
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];
  int nP0D=EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithP0D];

  return (nP0D > 1);
}
