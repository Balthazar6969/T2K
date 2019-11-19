#include "baseSelection.hxx"
#include "p0dNumuCCSelection.hxx"
#include "CutUtils.hxx"
#include "trackerSelectionUtils.hxx"
#include "EventBoxUtils.hxx"


//********************************************************************
p0dNumuCCSelection::p0dNumuCCSelection(bool forceBreak): SelectionBase(forceBreak, EventBoxId::kEventBoxTracker) {
//********************************************************************

}

//********************************************************************
void p0dNumuCCSelection::DefineSteps(){
//********************************************************************

    // Cuts must be added in the right order
    // last "true" means the step sequence is broken if cut is not passed (default is "false")
    AddStep(StepBase::kCut,    "event quality",      new EventQualityCut(),           true);
    AddStep(StepBase::kCut,    "> 0 tracks ",        new TotalMultiplicityCut(),      true);  
    AddStep(StepBase::kAction, "find leading tracks",new FindP0DLeadingTracksAction());  
    AddStep(StepBase::kAction, "find vertex",        new FindVertexAction());  
    AddStep(StepBase::kCut,    "quality+fiducial",   new TrackQualityFiducialCut(),   true);  
    AddStep(StepBase::kAction, "find veto track", new FindP0DVetoAction());
    AddStep(StepBase::kCut,    "veto", new P0DSelectionVetoCut(),true);
    SetBranchAlias(0,"trunk");
}

//**************************************************
void p0dNumuCCSelection::InitializeEvent(AnaEventC& eventC){
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

//********************************************************************
void p0dNumuCCSelection::DefineDetectorFV(){
//********************************************************************

  SetDetectorFV(SubDetId::kP0D);
}

//********************************************************************
bool p0dNumuCCSelection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
//********************************************************************

    if(allCutsPassed[0]){
        static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kP0DNuMuCC;
    }
    return (static_cast<AnaEventSummaryB*>(event.Summary)->EventSample != SampleId::kUnassigned);
}

//**************************************************
Int_t p0dNumuCCSelection::GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  (void)branch;
  
  std::vector<std::vector<Int_t> > groups(SystId::SystEnumLast_SystId, std::vector<Int_t>(1, -1));
  groups.resize(SystId::SystEnumLast_SystId);
   
  
  // Explicitely set the groups for various systeamtics that need them, 
  // this allows to have a transparent set/control of the systematics by the
  // selection  

  using namespace SystId;
  using namespace anaUtils;
  // --- Systematic         Detectors                                   P0D                                 
  groups[   kBFieldDist     ] = CreateVectorI( 1, EventBoxTracker::kTracksWithTPCInP0DFV );
  groups[   kMomScale       ] = CreateVectorI( 1, EventBoxTracker::kTracksWithTPCInP0DFV );
  groups[   kMomResol       ] = CreateVectorI( 1, EventBoxTracker::kTracksWithTPCInP0DFV );
  groups[   kTpcPid         ] = CreateVectorI( 1, EventBoxTracker::kTracksWithTPCInP0DFV );
  groups[   kChargeIDEff    ] = CreateVectorI( 1, EventBoxTracker::kTracksWithTPCInP0DFV );
  groups[   kTpcClusterEff  ] = CreateVectorI( 1, EventBoxTracker::kTracksWithTPCInP0DFV );
  groups[   kP0DELossScale  ] = CreateVectorI( 1, EventBoxTracker::kTracksWithTPCInP0DFV );
  groups[   kP0DELossResol  ] = CreateVectorI( 1, EventBoxTracker::kTracksWithTPCInP0DFV );
  groups[   kTpcTrackEff    ] = CreateVectorI( 1, EventBoxTracker::kTracksWithTPC        );
  groups[   kTpcP0dMatchEff ] = CreateVectorI( 1, EventBoxTracker::kTracksWithTPC        );
  
  // The systematics not mentioned above will get no groups
  Int_t ngroups = 0;
  // Check the systematic index is ok
  try {
    groups.at(systId);
  }
  catch (const std::out_of_range& oor) {
    std::cerr << this->Name() << " GetRelevantRecObjectGroupsForSystematic: syst index beyond limits "<< systId << " Error " << oor.what() << '\n';
    return ngroups;
  }
  
  if (groups[systId][0] >= 0) IDs[ngroups++] = groups[systId][0];
  
  return ngroups;
}

//**************************************************
Int_t p0dNumuCCSelection::GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  (void)branch;
  
  std::vector<std::vector<Int_t> > groups(SystId::SystEnumLast_SystId, std::vector<Int_t>(1, -1));
 
  // Explicitely set the groups for various systeamtics that need them, 
  // this allows to have a transparent set/control of the systematics by the
  // selection  

  using namespace SystId;
  using namespace anaUtils;
  // --- Systematic         Detectors                                   P0D                               
  groups[ kTpcTrackEff    ] =  CreateVectorI(1, EventBoxTracker::kTrueParticlesChargedInTPCInBunch);
  groups[ kTpcP0dMatchEff ] =  CreateVectorI(1, EventBoxTracker::kTrueParticlesChargedInTPCInBunch);
  
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
bool FindP0DLeadingTracksAction::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

  // Cast the ToyBox to the appropriate type
  ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

  trackerSelUtils::FindLeadingTracks(event, box);

  // For this selection the main track is the HMN track
  box.MainTrack = box.HMNtrack;
  return true;
}

//************************************************** 
bool FindP0DVetoAction::Apply(AnaEventC& event, ToyBoxB& boxB) const{ 
//************************************************** 
  ToyBoxTracker* box = static_cast<ToyBoxTracker*> (&boxB); 
  box->VetoTrack = cutUtils::FindP0DVetoTrack(event); 

  return true; 
} 

//************************************************** 
bool P0DSelectionVetoCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{ 
//************************************************** 

  (void)event;
  
  ToyBoxTracker* box = static_cast<ToyBoxTracker*> (&boxB); 
  if (box->VetoTrack)
    return false; 

  return true;

}



