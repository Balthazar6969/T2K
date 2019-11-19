#include "trackerSelectionUtils.hxx"

//********************************************************************
bool trackerSelUtils::FindLeadingTracksOld(AnaEventC& eventBB, ToyBoxB& boxB, bool qualityCut, SubDetId::SubDetEnum det, bool fiducialCut){
  //********************************************************************

  // Get the highest momentum track (HM),
  // the highest momentum negative track (HMN) and the 
  // highest momentum positive track (HMP)
  // from all tracks passing the quality and fiducial cut. 

  // this function cannot be in anaUtils because it needs the cuts

  AnaEventB& event = *static_cast<AnaEventB*>(&eventBB); 

  // Cast the ToyBox to the appropriate type
  ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 


  AnaTrackB* HM  = NULL;
  AnaTrackB* SHM  = NULL;
  AnaTrackB* HMN = NULL;
  AnaTrackB* SHMN = NULL;
  AnaTrackB* HMP = NULL;
  AnaTrackB* SHMP = NULL;

  AnaTrackB* negativeTracks[NMAXPARTICLES];
  AnaTrackB* positiveTracks[NMAXPARTICLES];
  int nNegativeTracks = 0;
  int nPositiveTracks = 0;

  AnaTrackB* selTracks[NMAXPARTICLES];
  int nTPC = anaUtils::GetAllTracksUsingTPC(event, selTracks);

  box.nNegativeTPCtracks=0;
  anaUtils::CreateArray(box.NegativeTPCtracks, nTPC);

  box.nPositiveTPCtracks=0;
  anaUtils::CreateArray(box.PositiveTPCtracks, nTPC);


  //loop over tpc tracks
  for (Int_t i=0;i<nTPC; ++i){
    AnaTrackB* track = selTracks[i];

    if (qualityCut){
      //apply the quality cut
      if (!cutUtils::TrackQualityCut(*track)){
        continue;
      }
    }

    // Apply the fiducial cut
    if (fiducialCut == true && ! cutUtils::FiducialCut(*track, det)){
      continue;
    }

    if (track->Charge == -1){
      negativeTracks[nNegativeTracks] = track;
      nNegativeTracks++;
    }
    else{
      positiveTracks[nPositiveTracks] = track;
      nPositiveTracks++;
    }

    //tracks are sorted by momentum, so the first track is the most energetic one
    if (!HM) HM = track;
    else if (HM && !SHM) SHM = track;
    //check the charge of the most energetic track
    if (track->Charge == -1 && !HMN) {

      HMN = track;
    }

    else if (track->Charge == -1 && HMN && !SHMN) SHMN = track;

    if (track->Charge == +1 && !HMP) HMP = track;
    else if (track->Charge == +1 && HMP && !SHMP) SHMP = track;


  }

  // Fill the box
  box.HMtrack   = HM;
  box.HMNtrack  = HMN;
  box.SHMtrack  = SHM;
  box.SHMNtrack = SHMN;
  box.HMPtrack  = HMP;
  box.SHMPtrack = SHMP;

  for(int k = 0; k < nNegativeTracks; ++k){
    box.NegativeTPCtracks[k] = negativeTracks[k];
  }
  box.nNegativeTPCtracks = nNegativeTracks;
  for(int k = 0; k < nPositiveTracks; ++k){
    box.PositiveTPCtracks[k] = positiveTracks[k];
  }
  box.nPositiveTPCtracks = nPositiveTracks;

  return (HM);  
}

//********************************************************************
bool trackerSelUtils::FindLeadingTracks(const AnaEventC& event, ToyBoxB& boxB){
  //********************************************************************

  // Get the highest momentum track (HM),
  // the highest momentum negative track (HMN) and the 
  // highest momentum positive track (HMP)
  // from all tracks passing the quality and fiducial cut. 

  // this function cannot be in anaUtils because it needs the cuts

  // Cast the ToyBox to the appropriate type
  ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

  SubDetId::SubDetEnum det = static_cast<SubDetId::SubDetEnum>(box.DetectorFV);

  EventBoxTracker::RecObjectGroupEnum groupID; 
  if   (det == SubDetId::kFGD1)
    groupID = EventBoxTracker::kTracksWithGoodQualityTPCInFGD1FV;
  else if (det == SubDetId::kFGD2)
    groupID = EventBoxTracker::kTracksWithGoodQualityTPCInFGD2FV;
  else if (det == SubDetId::kP0D)
    groupID = EventBoxTracker::kTracksWithGoodQualityTPCInP0DFV;
  else
    return false;

  // Retrieve the EventBoxTracker
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];

  AnaRecObjectC** selTracks = EventBox->RecObjectsInGroup[groupID];
  int nTPC=EventBox->nRecObjectsInGroup[groupID];

  box.nNegativeTPCtracks=0;
  if (box.NegativeTPCtracks) delete [] box.NegativeTPCtracks;
  anaUtils::CreateArray(box.NegativeTPCtracks, nTPC);

  box.nPositiveTPCtracks=0;
  if (box.PositiveTPCtracks) delete [] box.PositiveTPCtracks;
  anaUtils::CreateArray(box.PositiveTPCtracks, nTPC);

  // Reset to NULL all pointers
  box.HMNtrack = box.HMPtrack = box.SHMNtrack = box.SHMPtrack = box.SHMtrack = box.HMtrack = NULL;

  Float_t hm_mom=0;
  Float_t shm_mom=0;
  Float_t hmn_mom=0;
  Float_t shmn_mom=0;
  Float_t hmp_mom=0;
  Float_t shmp_mom=0;

  for (Int_t i=0;i<nTPC; ++i){
    AnaTrackB* track = static_cast<AnaTrackB*>(selTracks[i]);

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

    // Find the HMN and SHMN tracks
    if (track->Charge == -1){
      box.NegativeTPCtracks[box.nNegativeTPCtracks++] = track;

      if (track->Momentum > hmn_mom){
        if (box.HMNtrack){
          box.SHMNtrack = box.HMNtrack;
          shmn_mom= hmn_mom;
        }
        box.HMNtrack = track;
        hmn_mom= track->Momentum;

      }
      else if (track->Momentum >shmn_mom){
        box.SHMNtrack = track;
        shmn_mom= track->Momentum;
      }
    }
    // Find the HMP and SHMP tracks
    else if (track->Charge == +1){
      box.PositiveTPCtracks[box.nPositiveTPCtracks++] = track;

      if (track->Momentum > hmp_mom){
        if (box.HMPtrack){
          box.SHMPtrack = box.HMPtrack;
          shmp_mom= hmp_mom;
        }
        box.HMPtrack = track;
        hmp_mom= track->Momentum;
      }
      else if (track->Momentum >shmp_mom || !box.SHMPtrack){
        box.SHMPtrack = track;
        shmp_mom= track->Momentum;
      }
    }
  }

  // Resize the arrays
  anaUtils::ResizeArray(box.NegativeTPCtracks, box.nNegativeTPCtracks, nTPC);
  anaUtils::ResizeArray(box.PositiveTPCtracks, box.nPositiveTPCtracks, nTPC);

  return (box.HMtrack);  
}


//********************************************************************
bool trackerSelUtils::FindLeadingTracks(AnaRecObjectC** objects, size_t nObjects, ToyBoxB& boxB){
  //********************************************************************

  // Get the highest momentum track (HM),
  // the highest momentum negative track (HMN) and the 
  // highest momentum positive track (HMP)
  // from all tracks passing the quality and fiducial cut. 

  SubDetId::SubDetEnum det = static_cast<SubDetId::SubDetEnum>(boxB.DetectorFV);

  // Cast the ToyBox to the appropriate type
  ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

  box.nNegativeTPCtracks=0;
  anaUtils::CreateArray(box.NegativeTPCtracks, nObjects);

  box.nPositiveTPCtracks=0;
  anaUtils::CreateArray(box.PositiveTPCtracks, nObjects);

  // Reset to NULL all pointers
  box.HMNtrack = box.HMPtrack = box.SHMNtrack = box.SHMPtrack = box.SHMtrack = box.HMtrack = NULL;

  Float_t hm_mom    = 0;
  Float_t shm_mom   = 0;
  Float_t hmn_mom   = 0;
  Float_t shmn_mom  = 0;
  Float_t hmp_mom   = 0;
  Float_t shmp_mom  = 0;

  // Loop over tracks
  for (size_t i = 0; i < nObjects; i++){
    AnaTrackB* track = static_cast<AnaTrackB*>(objects[i]);

    // Apply the quality cut
    if (!cutUtils::TrackQualityCut(*track)){
      continue;
    }

    // Apply the fiducial cut
    if (!cutUtils::FiducialCut(*track, det)){
      continue;
    }

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

    // Find the HMN and SHMN tracks
    if (track->Charge == -1){
      box.NegativeTPCtracks[box.nNegativeTPCtracks++] = track;

      if (track->Momentum > hmn_mom){
        if (box.HMNtrack){
          box.SHMNtrack = box.HMNtrack;
          shmn_mom= hmn_mom;
        }
        box.HMNtrack = track;
        hmn_mom= track->Momentum;

      }
      else if (track->Momentum >shmn_mom){
        box.SHMNtrack = track;
        shmn_mom= track->Momentum;
      }
    }
    // Find the HMP and SHMP tracks
    else if (track->Charge == +1){
      box.PositiveTPCtracks[box.nPositiveTPCtracks++] = track;

      if (track->Momentum > hmp_mom){
        if (box.HMPtrack){
          box.SHMPtrack = box.HMPtrack;
          shmp_mom= hmp_mom;
        }
        box.HMPtrack = track;
        hmp_mom= track->Momentum;
      }
      else if (track->Momentum >shmp_mom || !box.SHMPtrack){
        box.SHMPtrack = track;
        shmp_mom= track->Momentum;
      }
    }

  }

  // Resize the arrays
  anaUtils::ResizeArray(box.NegativeTPCtracks, box.nNegativeTPCtracks, nObjects);
  anaUtils::ResizeArray(box.PositiveTPCtracks, box.nPositiveTPCtracks, nObjects);

  return (box.HMtrack);  
}

