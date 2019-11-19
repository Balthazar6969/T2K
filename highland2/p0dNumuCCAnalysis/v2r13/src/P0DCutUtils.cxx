#include "P0DCutUtils.hxx"
#include "BaseDataClasses.hxx"
#include "DataClasses.hxx"
#include "EventBoxId.hxx"
#include "AnalysisUtils.hxx"
#include "ToyBoxTracker.hxx"
#include "EventBoxTracker.hxx"
#include "CutUtils.hxx"


//Remove first 3 scintillator layers
const double sandMaxZ = -3223.03;


bool
cutUtils::IsStoppingP0DSandCandidate(AnaTrackB* track)
{
  if (!IsP0DVetoTrack(*track)) return false;
  AnaP0DParticleB* p0dSegment = track->P0DSegments[0];
  float* start = p0dSegment->PositionStart;
  if (start[2] < sandMaxZ) return true;
  return false;
}

bool
cutUtils::IsP0DSandCandidate(AnaTrackB* track)
{
  if (!IsOutsideP0DVetoCorridor(*track)) return false;
  AnaP0DParticleB* p0dSegment = track->P0DSegments[0];
  float* start = p0dSegment->PositionStart;
  if (start[2] < sandMaxZ) return true;
  return false;
}

int
cutUtils::CountTracksInP0DVetoCorridor(const AnaEventC& event)
{
  EventBoxTracker* evtBox =  static_cast<EventBoxTracker*> (event.EventBoxes[EventBoxId::kEventBoxTracker]) ;
  AnaRecObjectC** selTracks = evtBox->RecObjectsInGroup[EventBoxTracker::kTracksWithP0D];
  int np0d = evtBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithP0D];
  int n_in_corridor = 0;
  for (Int_t i = 0 ; i < np0d; i++)
  {
    AnaTrackB* track_temp= static_cast<AnaTrackB*>(selTracks[i]);

    if (!IsOutsideP0DVetoCorridor(*track_temp)) n_in_corridor++; 
  }

  return n_in_corridor;
}

int
cutUtils::CountP0DTracks(const AnaEventC& event)
{
  EventBoxTracker::RecObjectGroupEnum groupID = EventBoxTracker::kTracksWithP0D;

    // Retrieve the EventBox
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];

  int nP0D=EventBox->nRecObjectsInGroup[groupID];
  return nP0D;

}

//********************************************************************
bool cutUtils::ChooseP0DVetoTracks(const AnaEventC& event, ToyBoxB& boxB){
//********************************************************************
    // Get the highest momentum track (HM),
    // from all tracks identified as P0D veto tracks

    // this function cannot be in anaUtils because it needs the cuts

  // Cast the ToyBox to the appropriate type
  ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

  SubDetId::SubDetEnum det = static_cast<SubDetId::SubDetEnum>(box.DetectorFV);

  EventBoxTracker::RecObjectGroupEnum groupID; 
  if   (det == SubDetId::kP0D)
    groupID = EventBoxTracker::kTracksWithP0D;
  else 
    return false;

  // Retrieve the EventBoxTracker
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];

  AnaRecObjectC** selTracks = EventBox->RecObjectsInGroup[groupID];
  int nP0D=EventBox->nRecObjectsInGroup[groupID];


  // Reset to NULL all pointers
  box.HMNtrack = box.HMPtrack = box.SHMNtrack = box.SHMPtrack = box.SHMtrack = box.HMtrack = NULL;

  Float_t hm_mom=0;
  Float_t shm_mom=0;

  for (Int_t i=0;i<nP0D; ++i){
    AnaTrackB* track = static_cast<AnaTrackB*>(selTracks[i]);
    if (!IsP0DVetoTrack(*track)) continue;
    // Find the HM and SHM tracks
    if (track->Momentum > hm_mom){
      if (box.HMtrack){
        box.SHMtrack = box.HMtrack;
        shm_mom= hm_mom;
      }
      box.HMtrack = track;
      hm_mom= track->Momentum;
    }
    else if (track->Momentum >shm_mom || !box.SHMtrack){
      box.SHMtrack = track;
      shm_mom= track->Momentum;
    }
  }  
  return (box.HMtrack);  

}

/*
int
cutUtils::CountP0DMichelClusters(const AnaEventC& event, const AnaTrackB& track)
{

}

int
cutUtils::CountTrueP0DMichelClusters(const AnaEventC& event, const AnaTruTrackB& track)
{


}

*/


