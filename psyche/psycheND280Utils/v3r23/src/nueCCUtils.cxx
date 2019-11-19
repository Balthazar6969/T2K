#include "nueCCUtils.hxx"
#include "CutUtils.hxx"
#include "EventBoxUtils.hxx"

//**************************************************
TLorentzVector nueCCUtils::GetMomLorentzVector(const AnaTrackB& track, Float_t mass) {
//**************************************************

  TVector3 dir_start = anaUtils::ArrayToTVector3(track.DirectionStart);
  Float_t mom_start = track.Momentum;
  TVector3 momentum = dir_start * (mom_start / dir_start.Mag());
  Float_t energy = sqrt(mom_start * mom_start + mass * mass);
  TLorentzVector lv(momentum, energy);
  return lv;
}    

//**************************************************
bool nueCCUtils::IsEcalContained(AnaECALParticleB* ecal) {
//**************************************************

  Float_t showerPosX = ecal->ShowerPosition[0];
  Float_t showerPosY = ecal->ShowerPosition[1];
  Float_t showerPosZ = ecal->ShowerPosition[2];

  if (SubDetId::GetDetectorUsed(ecal->Detector, SubDetId::kDSECAL)) {
    return (fabs(showerPosX) < 900 && fabs(showerPosY) < 900);
  } 
  else if (SubDetId::GetDetectorUsed(ecal->Detector, SubDetId::kLeftTECAL) || SubDetId::GetDetectorUsed(ecal->Detector, SubDetId::kRightTECAL)) {
    return (fabs(showerPosZ) < 2900 && fabs(showerPosY) < 900);
  } 
  else {
    return (fabs(showerPosZ) < 2900 && fabs(showerPosX) < 900);
  }
} 

//**************************************************
bool nueCCUtils::UseEcal(AnaTrackB* track){
//**************************************************
  
  if(track->nECALSegments <= 0) return false;

  AnaECALParticleB* EcalSegment = track->ECALSegments[0];
  if(!EcalSegment) return false;

  // Protection against tracks that go in the ecal, but no ecal info is available
  if(EcalSegment->EMEnergy < 0.0) return false;

  // Ecal recon information must be available, turn on < highlandIO.FlatTree.UseReconDirTrackerECal = 1 >
  // Check if indeed this is turned on by checking if the Ecal variables are filled
  Float_t showerPosX = EcalSegment->ShowerPosition[0];
  Float_t showerPosY = EcalSegment->ShowerPosition[1];
  if(showerPosX == -999 && showerPosY == -999)
    std::cout << "WARNING::nueCCUtils::UseEcal - Shower position not availale. Check if the local Ecal Recon Directory is accesible! Turn on < highlandIO.FlatTree.UseReconDirTrackerECal >." << std::endl;

  Int_t firstlayer = EcalSegment->MostUpStreamLayerHit;
  
  if(firstlayer >= 4) return false;

  return true;
}

//**************************************************
bool nueCCUtils::UseEcalEMEnergyPID(AnaECALParticleB* EcalSegment, Float_t EMEnergy_thr, Float_t TPCBackMom){
//**************************************************
  return (TPCBackMom > EMEnergy_thr && nueCCUtils::IsEcalContained(EcalSegment));
}

//**************************************************
void nueCCUtils::FindP0DVetoTracks(const AnaEventB& event, const AnaTrackB& mtrack, Int_t nTracks[3]){
//**************************************************

  Int_t ntracks = event.nParticles;
  nTracks[0] = 0;
  nTracks[1] = 0;
  nTracks[2] = 0;

  for(Int_t i = 0; i < ntracks; i++){
    AnaTrackB* track = static_cast<AnaTrackB*>(event.Particles[i]);

    // Not the selected main track
    if (&mtrack == track) continue;

    // Protection against sand muons entering the detector
    if(track->PositionStart[2] < -3290.0) continue;

    if(anaUtils::TrackUsesDet(*track, SubDetId::kP0D))
      nTracks[0]++;

    if(anaUtils::TrackUsesDet(*track, SubDetId::kPECAL))
      nTracks[1]++;
    
    // Since this is only a track finding search, don't need to cut on the fgd iso direction 
    //if(cutUtils::FiducialCut(*track, SubDetId::kFGD1))
    if(anaUtils::InDetVolume(SubDetId::kFGD1, track->PositionStart) || anaUtils::TrackUsesOnlyDet(*track, SubDetId::kFGD1) )
      nTracks[2]++;
    //if (event.Particles[i]->nTPCSegments > 0 || anaUtils::ArrayToTLorentzVector(event.Particles[i]->DirectionStart).CosTheta() > isoctmin) {
  }
}

//**************************************************
AnaTrackB* nueCCUtils::FindPairTrack(const AnaEventB& event, const AnaTrackB& track, Float_t delta_pos_max, Float_t pullel_min, Float_t pullel_max, bool same_charge){
//**************************************************
  AnaTrackB* PairTrack = NULL;

  Float_t minv_postcuts = 1e6;
  Float_t mass = 0.511;
  
  // The start 4 momentum and position of the primary particle.
  TLorentzVector primary_PE = nueCCUtils::GetMomLorentzVector(track, mass);
  TVector3 primary_pos = anaUtils::ArrayToTVector3(track.PositionStart);

  // Get all TPC tracks
  EventBoxTracker::RecObjectGroupEnum groupID;
  groupID = EventBoxTracker::kTracksWithTPC;
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];

  // Loop over all TPC tracks that isn't the primary one.
  for(Int_t i = 0; i < EventBox->nRecObjectsInGroup[groupID]; i++ ) {
    AnaTrackB *secondary = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[groupID][i]);

    // Not the selected main track
    if (&track == secondary) continue;

    // The start 4 momentum and position of the secondry track
    TLorentzVector secondary_PE = nueCCUtils::GetMomLorentzVector(*secondary, mass);
    TVector3 secondary_pos = anaUtils::ArrayToTVector3(secondary->PositionStart);

    Float_t minv = 2. * mass * mass + 2. * (secondary_PE.Dot(primary_PE));
    if(minv > 0)
      minv = sqrt(minv);
    else
      continue;
    
    // Tracks start within a certain distance of each other.
    if((primary_pos - secondary_pos).Mag() > delta_pos_max) continue;

    // Option to select two tracks with the same charge
    if(same_charge){
      if(track.Charge * secondary->Charge < 0) continue;
    }
    else{ // Tracks have opposite charge.
      if(track.Charge * secondary->Charge > 0) continue;
    }

    // Is electron like cut. Perform PID on the longest TPC Segment in the closest TPC
    if(secondary->nTPCSegments < 1) continue;
    
    AnaTPCParticleB* longCloseTPC = static_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInClosestTpc(*secondary));
    if(!longCloseTPC) continue;

    // At least 18 TPC hits
    if(!cutUtils::TPCTrackQualityCut(*longCloseTPC)) continue;

    Float_t pullelec = (longCloseTPC->dEdxMeas-longCloseTPC->dEdxexpEle)/longCloseTPC->dEdxSigmaEle;

    if(pullelec < pullel_min || pullelec > pullel_max) continue;

    if(minv < minv_postcuts){
      PairTrack = secondary;
      minv_postcuts = minv;
    }
  }

  return PairTrack;
}

//**************************************************
AnaTrackB* nueCCUtils::FindTPCVetoTrack(const AnaEventB& event, const AnaTrackB& track){
//**************************************************

  AnaTrackB* ret = NULL; 

  // Get all TPC tracks
  EventBoxTracker::RecObjectGroupEnum groupID;
  groupID = EventBoxTracker::kTracksWithTPC;
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];
  
  Float_t maxmom = -1000;
  for(Int_t i = 0; i < EventBox->nRecObjectsInGroup[groupID]; i++ ) {
    AnaTrackB *veto = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[groupID][i]);
 
    if(veto == &track) continue;

    // Protection against sand muons entering the detector
    if(veto->PositionStart[2] < -3290.0) continue;

    if(veto->Momentum > maxmom){
      maxmom = veto->Momentum;
      ret = veto; 
    }
  } 

  return ret;
}

//**************************************************
AnaTrackB* nueCCUtils::FindFGDTPCVetoTrack(const AnaEventB& event, const AnaTrackB& track, SubDetId::SubDetEnum det){
//**************************************************

  AnaTrackB* ret = NULL; 

  // Get all FGD-TPC tracks
  EventBoxTracker::RecObjectGroupEnum groupID;
  if(det == SubDetId::kFGD1)
    groupID = EventBoxTracker::kTracksWithTPCAndFGD1;
  else if (det == SubDetId::kFGD2)
    groupID = EventBoxTracker::kTracksWithTPCAndFGD2;
  else 
    return NULL;

  // Retrieve the EventBoxTracker
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];
  
  Float_t maxmom = -1000;
  for(Int_t i = 0; i < EventBox->nRecObjectsInGroup[groupID]; i++ ) {
    AnaTrackB *veto = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[groupID][i]);
 
    if(veto == &track) continue;

    // Protection against sand muons entering the detector
    if(veto->PositionStart[2] < -3290.0) continue;

    if(veto->Momentum > maxmom){
      maxmom = veto->Momentum;
      ret = veto; 
    }
  } 

  return ret;
}

//**************************************************
Int_t nueCCUtils::FindOOFVFGDTPCTracks(const AnaEventB& event, const AnaTrackB& track, SubDetId::SubDetEnum det){
//**************************************************
  int noofv = 0;

  // Get all FGD-TPC tracks
  EventBoxTracker::RecObjectGroupEnum groupID;
  if(det == SubDetId::kFGD1)
    groupID = EventBoxTracker::kTracksWithTPCAndFGD1;
  else if (det == SubDetId::kFGD2)
    groupID = EventBoxTracker::kTracksWithTPCAndFGD2;
  else 
    return noofv;

  // Retrieve the EventBoxTracker
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];

  for(Int_t i = 0; i < EventBox->nRecObjectsInGroup[groupID]; i++ ) {
    AnaTrackB *veto = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[groupID][i]);
 
    if(veto == &track) continue;

    //if(!anaUtils::InFiducialVolume(det, veto->PositionStart))
    // Only look in the first two layers
    if(det == SubDetId::kFGD1 && veto->PositionStart[2] > 110 && veto->PositionStart[2] < 140)
      noofv++;
    else if(det == SubDetId::kFGD2 && veto->PositionStart[2] > 1455 && veto->PositionStart[2] < 1485)
      noofv++;
  }

  return noofv;
      
}

//**************************************************
void nueCCUtils::FindGammaIsoTracks(const AnaEventB& event, const AnaTrackB& track, SubDetId::SubDetEnum det, Float_t vertex_activity, Int_t nTracksx[3]){
//**************************************************

  nTracksx[0] = 0;
  nTracksx[1] = 0;
  nTracksx[2] = 0;

  // Get all FGD-TPC tracks
  EventBoxTracker::RecObjectGroupEnum groupID;
  if(det == SubDetId::kFGD1)
    groupID = EventBoxTracker::kTracksWithTPCAndFGD1;
  else if(det == SubDetId::kFGD2)
    groupID = EventBoxTracker::kTracksWithTPCAndFGD2;
  else return;
  
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];
  
  for(Int_t i = 0; i < EventBox->nRecObjectsInGroup[groupID]; i++ ) {
    AnaTrackB *fgdtpctracks = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[groupID][i]);

    if(fgdtpctracks == &track) continue;

    Float_t diff = (anaUtils::ArrayToTVector3(fgdtpctracks->PositionStart) -
		    anaUtils::ArrayToTVector3(track.PositionStart)).Mag();

    if(diff > vertex_activity)
      nTracksx[0]++;
    else{
      nTracksx[1]++;
      if(anaUtils::TrackUsesDet(*fgdtpctracks, SubDetId::kP0D))
        nTracksx[2]++;
    }
  }
}

//**************************************************
AnaTrackB* nueCCUtils::FindEcalVetoTrack(const AnaEventB& event, const AnaTrackB& track, bool nc){
//**************************************************

  AnaTrackB* ret = NULL; 

  Float_t min_z = 1e6;

  // Get all Ecal objects
  EventBoxTracker::RecObjectGroupEnum groupID;
  groupID = EventBoxTracker::kTracksWithECal;
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];
  
  // Loop over all tracks that use the ECal
  for(int i = 0; i < EventBox->nRecObjectsInGroup[groupID]; i++ ) {

    AnaTrackB *ecalTrack = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[groupID][i]);
    if(!(ecalTrack->nECALSegments)) continue;
    if(ecalTrack == &track) continue;

    // If this is enabled, only take ecal neutral clusters (clusters only in Ecal or clusters in Ecal-SMRD) 
    if(nc){
      if( anaUtils::TrackUsesDet(*ecalTrack, SubDetId::kP0D) || anaUtils::TrackUsesDet(*ecalTrack, SubDetId::kFGD) || anaUtils::TrackUsesDet(*ecalTrack, SubDetId::kTPC) ) continue;   
    }

    // Check track uses track or DsECal
    if (!anaUtils::UsesTrackerDsEcal(ecalTrack) ) continue;
    
    // Find most upstream ECal seg of the track
    AnaECALParticleB* ecal_seg = anaUtils::GetMostUpstreamECalSegment(ecalTrack);
    if(!ecal_seg) continue;
    
    // Select the most upstream point. This could be the start or end of the Ecal segment
    Float_t upstream_z = (ecal_seg->PositionStart[2]<ecal_seg->PositionEnd[2]) ?
      ecal_seg->PositionStart[2]: ecal_seg->PositionEnd[2];
    
    // If this is the most upstream Ecal track/segment save it        
    if(upstream_z < min_z) {
      ret = ecalTrack;
      min_z = upstream_z;
    }   
  }

  return ret;
}

//**************************************************
void nueCCUtils::FindFGD2ShowerTracks(const AnaEventB& event, const AnaTrackB& track, Int_t nTracksx[2]){
//**************************************************
  nTracksx[0] = 0;
  nTracksx[1] = 0;

  // Get all TPC tracks
  EventBoxTracker::RecObjectGroupEnum groupID;
  groupID = EventBoxTracker::kTracksWithTPC;
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];

  for(Int_t i = 0; i < EventBox->nRecObjectsInGroup[groupID]; i++ ) {
    AnaTrackB *fgdtpctracks = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[groupID][i]);
    if(fgdtpctracks == &track){continue;}

    if(SubDetId::GetDetectorUsed(fgdtpctracks->Detector, SubDetId::kFGD1)){
      if(SubDetId::GetDetectorUsed(fgdtpctracks->Detector, SubDetId::kTPC2) && !SubDetId::GetDetectorUsed(fgdtpctracks->Detector, SubDetId::kTPC1))
	nTracksx[0]++;
    }
    else if(SubDetId::GetDetectorUsed(fgdtpctracks->Detector, SubDetId::kFGD2)){
      if(SubDetId::GetDetectorUsed(fgdtpctracks->Detector, SubDetId::kTPC3) && !SubDetId::GetDetectorUsed(fgdtpctracks->Detector, SubDetId::kTPC2) && !SubDetId::GetDetectorUsed(fgdtpctracks->Detector, SubDetId::kFGD1))
	nTracksx[1]++;
    }
  }

}

//**************************************************
Float_t nueCCUtils::GetInvMass(const AnaTrackB &primary, const AnaTrackB &secondary){
//**************************************************

  Float_t mass = 0.511;
  TLorentzVector primary_PE   = nueCCUtils::GetMomLorentzVector(primary,   mass);
  TLorentzVector secondary_PE = nueCCUtils::GetMomLorentzVector(secondary, mass);
  Float_t minv = 2. * mass * mass + 2. * (secondary_PE.Dot(primary_PE));
  if(minv > 0)
    return sqrt(minv);
  else 
    return -999.;

}

//**************************************************
AnaTrackB* nueCCUtils::FindSecondTPCTrack(const AnaEventB& event, const AnaTrackB& primary, SubDetId::SubDetEnum det){
//**************************************************

  AnaTrackB* HMextraFgdTpc = NULL;

  // Get all TPC tracks
  EventBoxTracker::RecObjectGroupEnum groupID;
  groupID = EventBoxTracker::kTracksWithTPC;
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];

  for(Int_t i = 0; i < EventBox->nRecObjectsInGroup[groupID]; i++ ) {
    AnaTrackB *track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[groupID][i]);
    
    if(!track) continue;
    if(!track->Momentum) continue;
    if(track == &primary) continue;
    if(!SubDetId::GetDetectorUsed(track->Detector, det) ) continue;

    if(!HMextraFgdTpc)
      HMextraFgdTpc = track;
    else{
      if(track->Momentum > HMextraFgdTpc->Momentum) 
	HMextraFgdTpc = track;
    }
  }

  return HMextraFgdTpc;
}

//********************************************************************
AnaECALParticleB* nueCCUtils::GetMostUpstreamECalNCSegment(AnaTrackB* track) {
//********************************************************************
  AnaECALParticleB* upstream = NULL;

  Float_t pos_z = 1e4;
  
  if (track) {
    // Only looking for ecal neutral clusters
    if( anaUtils::TrackUsesDet(*track, SubDetId::kP0D) || anaUtils::TrackUsesDet(*track, SubDetId::kFGD) || anaUtils::TrackUsesDet(*track, SubDetId::kTPC) ) return upstream;
    
    for (Int_t i = 0; i < track->nECALSegments; i++) {
      AnaECALParticleB* ecal = track->ECALSegments[i];
      if(!ecal) continue;
      if (ecal->PositionStart[2] < pos_z) {
	upstream = ecal;
	pos_z = ecal->PositionStart[2]; 
      }
      if (ecal->PositionEnd[2] < pos_z) {
	upstream = ecal;
	pos_z = ecal->PositionEnd[2];
      }
    }
  }

  return upstream;
}
//**************************************************
AnaTrackB* nueCCUtils::FindLeadingTrackInDet(const AnaEventB& event, SubDetId::SubDetEnum det){
//**************************************************

  EventBoxTracker::RecObjectGroupEnum groupID; 

  if     (det == SubDetId::kFGD1)
    groupID = EventBoxTracker::kTracksWithGoodQualityTPCInFGD1FV;
  else if(det == SubDetId::kFGD2)
    groupID = EventBoxTracker::kTracksWithGoodQualityTPCInFGD2FV;
  else{
    std::cout << "FindLeadingTrackInDet valid only with FGD1 or 2" << std::endl;
    return NULL;
  }
  
  // Retrieve the EventBoxTracker
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];

  AnaRecObjectC** selTracks = EventBox->RecObjectsInGroup[groupID];
  int nTPC=EventBox->nRecObjectsInGroup[groupID];

  // Reset to NULL all pointers
 
  AnaTrackB* LeadingTrack = NULL;
  
  Float_t hm_mom = 0;

  for (Int_t i=0;i<nTPC; ++i){
    AnaTrackB* track = static_cast<AnaTrackB*>(selTracks[i]);
      
    // Find the HM tracks
    if(track->Momentum > hm_mom){
      LeadingTrack = track;
      hm_mom= track->Momentum;
    }
  }      

  return LeadingTrack;
}
