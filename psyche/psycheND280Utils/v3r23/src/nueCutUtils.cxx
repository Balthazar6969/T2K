#include "nueCutUtils.hxx"
#include "nueCCUtils.hxx"
#include "CutUtils.hxx"

//**************************************************
bool nueCutUtils::TPCElectronPull(AnaTrackB* track, Float_t pullel_accept_min, Float_t pullel_accept_max, Float_t pullel_accept_tight_min, Float_t pullel_accept_tight_max, Float_t minMom_ecal){
//**************************************************
  if(!track || track->nTPCSegments == 0) return false;

  AnaTPCParticleB* longCloseTPC = static_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInClosestTpc(*track));
  if(!longCloseTPC) return false;

  Float_t pullelec = (longCloseTPC->dEdxMeas-longCloseTPC->dEdxexpEle)/longCloseTPC->dEdxSigmaEle;

  if(pullelec < pullel_accept_min || pullelec > pullel_accept_max) return false;

  // Tight electron pull applied only to the events not going in the Ecal or have momentum below the ecal pid threshold
  if(nueCCUtils::UseEcal(track) && track->Momentum > minMom_ecal) return true;

  if(pullelec < pullel_accept_tight_min || pullelec > pullel_accept_tight_max) return false;

  return true;
}

//**************************************************
bool nueCutUtils::TPCMuonPull(AnaTrackB* track, Float_t pullmu_reject_min, Float_t pullmu_reject_max, Float_t minMom_ecal){
//**************************************************
  if(!track) return false;

  AnaTPCParticleB* longCloseTPC = static_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInClosestTpc(*track));
  if(!longCloseTPC) return false;

  // Applied to events not going in the Ecal and have momentum below the ecal pid threshold
  if(nueCCUtils::UseEcal(track) && track->Momentum > minMom_ecal) return true;

  Float_t pullmuon = (longCloseTPC->dEdxMeas-longCloseTPC->dEdxexpMuon)/longCloseTPC->dEdxSigmaMuon;

  if(pullmuon > pullmu_reject_min && pullmuon < pullmu_reject_max) return false;

  return true;
}

//**************************************************
bool nueCutUtils::TPCPionPull(AnaTrackB* track, Float_t pullpi_reject_min, Float_t pullpi_reject_max, Int_t num_tpc_nodes, Float_t minMom_ecal){
//**************************************************
  if(!track) return false;

  AnaTPCParticleB* longCloseTPC = static_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInClosestTpc(*track));
  if(!longCloseTPC) return false;

  // Not applied to events going in the Ecal with more than the minimum number of TPC hits and have momentum above the ecal pid threshold
  if(nueCCUtils::UseEcal(track) && longCloseTPC->NNodes > num_tpc_nodes && track->Momentum > minMom_ecal) return true;

  Float_t pullpion = (longCloseTPC->dEdxMeas-longCloseTPC->dEdxexpPion)/longCloseTPC->dEdxSigmaPion;
 
  if(pullpion > pullpi_reject_min && pullpion < pullpi_reject_max) return false;

  return true;
}

//**************************************************
bool nueCutUtils::EcalEMEnergy(AnaTrackB* track, Float_t Ethreshold, Float_t Emin_ecal, Float_t eoverp, bool AntiNu){
//**************************************************
  if(!track) return false;

  // Ecal PID applied only if the track goes in the Ecal
  if(!nueCCUtils::UseEcal(track)) return true;
  
  // Get the Ecal segment
  AnaECALParticleB* EcalSegment = track->ECALSegments[0];
  if(!EcalSegment) return true;

  // Select the TPC segment of good quality (true) closet to ECal,
  // Require track quality to trust the momentum
  AnaTPCParticleB* tpcTrackBackB = 
    static_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInClosestTpc(*track, EcalSegment->PositionStart, true));
   
  if(!tpcTrackBackB) return true;

  // Ecal EMEnergy PID is only applied for high energy tracks
  if(!nueCCUtils::UseEcalEMEnergyPID(EcalSegment, Ethreshold, tpcTrackBackB->Momentum)) return true;

  // Applied only to fully ecal contained clusters
  if(!nueCCUtils::IsEcalContained(EcalSegment)) return true;
  
  if(AntiNu){
    if(EcalSegment->EMEnergy < Emin_ecal) return false;
  }
  else{
    if(EcalSegment->EMEnergy < Emin_ecal && EcalSegment->EMEnergy/tpcTrackBackB->Momentum < eoverp) return false;
  }

  return true;
}

//**************************************************
bool nueCutUtils::EcalMIPEMPID(AnaTrackB* track, Float_t Ethreshold, Float_t ecal_mipem_min){
//**************************************************
  if(!track) return false;

  // Ecal PID applied only if the track goes in the Ecal
  if(!nueCCUtils::UseEcal(track)) return true;
  
  // Get the Ecal segment
  AnaECALParticleB* EcalSegment = track->ECALSegments[0];
  if(!EcalSegment) return true;
 
  // Select the TPC segment of good quality (true) closet to ECal,
  // Require track quality to trust the momentum
  AnaTPCParticleB* tpcTrackBackB = 
    static_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInClosestTpc(*track, EcalSegment->PositionStart, true));

  // Ecal PID is applied to these tracks that the EMEnergy PID is not applied
  if(tpcTrackBackB && nueCCUtils::UseEcalEMEnergyPID(EcalSegment, Ethreshold, tpcTrackBackB->Momentum) && nueCCUtils::IsEcalContained(EcalSegment) ) return true;

  if(EcalSegment->PIDMipEm < ecal_mipem_min) return false;

  return true;
}

//**************************************************
bool nueCutUtils::EOverPNuE(AnaTrackB* track, Float_t eop_threshold, Float_t eop_minmom){
//**************************************************
  if(!track) return false;

  // Applied only if the track goes in the Ecal
  if(!nueCCUtils::UseEcal(track)) return true;

  // Applied ony above  MeV/c
  if(track->Momentum < eop_minmom) return true;
  
  // Get the Ecal segment
  AnaECALParticleB* EcalSegment = track->ECALSegments[0];
  if(!EcalSegment) return true;

  // Applied only if the track is Ecal contained
  //if(!nueCCUtils::IsEcalContained(EcalSegment)) return true;

  // Select the TPC segment of good quality (true) closet to ECal,
  // Require track quality to trust the momentum
  AnaTPCParticleB* tpcTrackBackB = 
    static_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInClosestTpc(*track, EcalSegment->PositionStart, true));

  if(!tpcTrackBackB) return true;
  
  // Fully contained in the Ecal
  if(nueCCUtils::IsEcalContained(EcalSegment) && EcalSegment->EMEnergy/tpcTrackBackB->Momentum < eop_threshold) return false;

  // Above p > 2 GeV/c no containment condition
  if(track->Momentum > 2000 && EcalSegment->EMEnergy/tpcTrackBackB->Momentum < eop_threshold) return false;

  return true;
}

//**************************************************
bool nueCutUtils::EcalEMHIPPID(AnaTrackB* track, Float_t protonregion_low, Float_t emhip){
//**************************************************
  if(!track) return false;

   // Applied only if the track goes in the Ecal
  if(!nueCCUtils::UseEcal(track)) return true;

  // Only applied in the proton momentum region
  if(track->Momentum < protonregion_low) return true;

  // Get the Ecal segment
  AnaECALParticleB* EcalSegment = track->ECALSegments[0];
  if(!EcalSegment) return true;

  if (EcalSegment->PIDEmHip > emhip) return false;

  return true;
}

//**************************************************
bool nueCutUtils::EOverP(AnaTrackB* track, Float_t protonregion_low, Float_t eoverp){
//**************************************************
  if(!track) return false;

   // Applied only if the track goes in the Ecal
  if(!nueCCUtils::UseEcal(track)) return true;

  // Only applied in the proton momentum region
  if(track->Momentum < protonregion_low) return true;

  // Get the Ecal segment
  AnaECALParticleB* EcalSegment = track->ECALSegments[0];
  if(!EcalSegment) return true;

  // Select the TPC segment of good quality (true) closet to ECal,
  // Require track quality to trust the momentum
  AnaTPCParticleB* tpcTrackBackB = 
    static_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInClosestTpc(*track, EcalSegment->PositionStart, true));

  if(!tpcTrackBackB) return true;
  
  if(EcalSegment->EMEnergy/tpcTrackBackB->Momentum < eoverp) return false;

  return true;
}

//**************************************************
bool nueCutUtils::SecondTPCPID(AnaTrackB* track, Float_t pullmu_reject_min, Float_t pullmu_reject_max){
//**************************************************
  // Only applied to FGD1 events
  if(anaUtils::InDetVolume(SubDetId::kFGD2, track->PositionStart)) return true;

  AnaTPCParticleB* longestTPC3 = static_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInDet(*track, SubDetId::kTPC3));
  if(!longestTPC3) return true;
  
  /// Minimum of 18 TPC hits
  if(!cutUtils::TPCTrackQualityCut(*longestTPC3)) return true;

  Float_t pullmuon = (longestTPC3->dEdxMeas-longestTPC3->dEdxexpMuon)/longestTPC3->dEdxSigmaMuon;
 
  if (pullmuon > pullmu_reject_min && pullmuon < pullmu_reject_max) return false;

  return true;
}

//**************************************************
bool nueCutUtils::AntiNuSecondTPCPID(AnaTrackB* track, Float_t protonregion_low, Float_t protonregion_high, Float_t pullel_reject_min, Float_t pullel_reject_max){
//**************************************************
  // Only applied to FGD1 events
  if(anaUtils::InDetVolume(SubDetId::kFGD2, track->PositionStart)) return true;

  AnaTPCParticleB* longestTPC3 = static_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInDet(*track, SubDetId::kTPC3));
  if(!longestTPC3) return true;
  
  // Minimum of 18 TPC hits
  if(!cutUtils::TPCTrackQualityCut(*longestTPC3)) return true;

  // Only apply to the events in the proton momentum region
  if(track->Momentum < protonregion_low || track->Momentum > protonregion_high) return true;
  // Only applied to tracks not going in the ecal
  if(nueCCUtils::UseEcal(track)) return true;
  
  Float_t pullelec = (longestTPC3->dEdxMeas-longestTPC3->dEdxexpEle)/longestTPC3->dEdxSigmaEle;
  if (pullelec < pullel_reject_min || pullelec > pullel_reject_max) return false;

  return true;
}

//**************************************************
bool nueCutUtils::TrackQuality(AnaTrackB* track, int _num_tpc_nodes, int _num_tpc_nodes_ecal){
  //**************************************************

  if(!track) return false;
 
  AnaParticleB* TpcSegment = anaUtils::GetSegmentWithMostNodesInClosestTpc(*track);
  if(!TpcSegment) return false;

  if(!nueCCUtils::UseEcal(track))
    return (TpcSegment->NNodes > _num_tpc_nodes);
  else
    return (TpcSegment->NNodes > _num_tpc_nodes_ecal);
}


//**************************************************
bool nueCutUtils::PairCut(const AnaTrackB& track, AnaTrackB* pair, Float_t inv_mass_min){
//**************************************************
  if(!pair) return true;

  Float_t mass = 0.511;

  // The minimum invariant mass required to pass the cut.
  TLorentzVector secondary_PE = nueCCUtils::GetMomLorentzVector(*pair, mass);
  TLorentzVector primary_PE   = nueCCUtils::GetMomLorentzVector(track, mass);
  Float_t minv = 2. * mass * mass + 2. * (secondary_PE.Dot(primary_PE));

  if(minv < 0) return true; // backward track?

  minv = sqrt(minv);
  if(minv < inv_mass_min) return false;

  return true;
}

//**************************************************
bool nueCutUtils::TPCVeto(const AnaTrackB& track, AnaTrackB* veto, Float_t delta_z_min){
//**************************************************
  if(!veto) return true;
  if(!veto->Original) return true;

  // The minimum Z distance there must be between the veto and candidate tracks
  Float_t hmnZstart = track.PositionStart[2];
  Float_t delta_z = veto->Original->PositionStart[2] - hmnZstart;
  
  if(delta_z < delta_z_min) return false;

  return true;
}

//**************************************************
bool nueCutUtils::EcalVeto(const AnaTrackB& track, AnaTrackB* veto, Float_t delta_z_min){
//**************************************************
  if(!veto) return true;

  Float_t found_delta_z = 1e6;
  Float_t start_z = track.PositionStart[2];
  
  AnaECALParticleB* ecal_seg = anaUtils::GetMostUpstreamECalSegment(veto);
  if(!ecal_seg) return true;

  Float_t upstream_z = (ecal_seg->PositionStart[2]<ecal_seg->PositionEnd[2]) ?
    ecal_seg->PositionStart[2]: ecal_seg->PositionEnd[2];

  found_delta_z = upstream_z - start_z;	

  return (found_delta_z > delta_z_min);
}

//**************************************************
bool nueCutUtils::ToF(const AnaTrackB& track, Float_t fgdecaltof, Int_t RunNum){
//**************************************************

  if(RunNum < 14 && RunNum >= 0){
    // FGD1-BrEcal
    if(track.ToF.Flag_ECal_FGD1 && track.ToF.ECal_FGD1 < fgdecaltof) return false;
    // FGD2-BrEcal
    if(track.ToF.Flag_ECal_FGD2 && track.ToF.ECal_FGD2 < fgdecaltof) return false;
  }

  return true;
}

//**************************************************
bool nueCutUtils::AntiNuToF(const AnaTrackB& track, Float_t fgdecaltof, Float_t fgd1fgd2tof, Float_t fgd1fgd2tof_relaxed, Float_t protonregion_low, Int_t RunNum){
//**************************************************
 
  // FGD-BrEcal check
  if(!nueCutUtils::ToF(track, fgdecaltof, RunNum)) return false;

  // Relaxed cut applied to both FHC and RHC
  if(track.ToF.Flag_FGD1_FGD2 && track.ToF.FGD1_FGD2 < fgd1fgd2tof_relaxed) return false;
  
  // Cut harder on the FHC
  if(track.ToF.Flag_FGD1_FGD2 && RunNum < 8 && RunNum >= 0){
    // Only applied in the proton momentum region
    if(track.Momentum < protonregion_low) return true;

    if(track.ToF.FGD1_FGD2 < fgd1fgd2tof) return false;
  }

  return true;
}

//**************************************************
bool nueCutUtils::FGD2Shower(AnaTrackB* track, Float_t protonregion_low, Float_t protonregion_high, Int_t nFGD1TPC2, Int_t nFGD2TPC3){
//**************************************************

  // Only applied in the proton momentum region
  if(track->Momentum < protonregion_low) return true;

  // Only applied to events not going in the Ecal
  if(nueCCUtils::UseEcal(track)) return true;

  // Reject for FGD2
  if(anaUtils::InDetVolume(SubDetId::kFGD2, track->PositionStart)) return false;

  // Reject if the fgd1 track checked doesn't go in fgd2
  if(!anaUtils::TrackUsesDet(*track, SubDetId::kFGD2)) return false;

  // Check if the track goes in the TPC
  bool intpc3 = anaUtils::TrackUsesDet(*track, SubDetId::kTPC3);
  AnaTPCParticleB* longestTPC3 = static_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInDet(*track,SubDetId::kTPC3));
  if(!longestTPC3) intpc3 = false;
  //if(longestTPC3)
  //if(!cutUtils::TPCTrackQualityCut(*longestTPC3)) intpc3 = false;

  if(track->Momentum < protonregion_high){
    if(intpc3){ // in TPC3
      // Apply this as many of the low energy protons won't make it to TPC3
      if(track->Momentum < 950.0) return true;

      // Minimum number of tracks
      if(nFGD2TPC3 < 1) return false;
      // Extra for multi-track events
      if(nFGD2TPC3 == 1 && nFGD1TPC2 > 0) return false;
    }
    else{ // tracks stopped in FGD2
      // Minimum number of FGD2-TPC3 tracks
      if(nFGD2TPC3 < 2) return false;
      // Only keep single track events for now
      if(nFGD1TPC2 > 0) return false;
    }
  }
  else{
    if(nFGD2TPC3 < 1) return false;
  }

  return true;
}

//**************************************************
bool nueCutUtils::FGD2ShowerActivityEcal(AnaTrackB* track, Float_t protonregion_low, Int_t nFGD1TPC2, Int_t nFGD2TPC3){
//**************************************************
  
  // Will return true only when a candidate is found

  // Only applied in the proton momentum region
  if(track->Momentum < protonregion_low) return false;
  
  // Only applied to events going in the Ecal
  if(!nueCCUtils::UseEcal(track)) return false;

  // Only for FGD1 events
  if(anaUtils::InDetVolume(SubDetId::kFGD2, track->PositionStart)) return false;

  // Main track from FGD1 has to go in FGD2
  if(!anaUtils::TrackUsesDet(*track, SubDetId::kFGD2)) return false;

  // Applied only to single track events
  if(nFGD1TPC2 > 0 || nFGD2TPC3 < 2) return false;

  return true;
}

//**************************************************
bool nueCutUtils::FGDProtonPull(AnaTrackB* track, SubDetId::SubDetEnum det, Float_t protonpull){
  //**************************************************

  if( track->nFGDSegments != 2 ) return true;

  // Second FGD segment
  AnaFGDParticleB *fgdTrack = track->FGDSegments[1];

  if( !fgdTrack  ) return true; // Protection.

  if( TMath::Abs(fgdTrack->Pullp) >= 1.e+6 || TMath::Abs(fgdTrack->Pullmu) >= 1.e+6 || TMath::Abs(fgdTrack->Pullpi) >= 1.e+6  ) return true;

  // Check the pullnotset variable.
  // This tells us if the pull was calculated by the algorithm.
  // If it was not calculated this variable will be 1, so for calculated pulls fgdpulls!=1.
  if(  fgdTrack->Pullno == 1 ) return true;

  // This tell us something about the geometry.
  // If the algorithm says the particle started and stopped in 1st/ 2nd fgd this is equal 1 or 2.
  // If the particle stopped in 1st/2nd fgd, this is equal -1/-2. Other cases: 0.
  if( fgdTrack->Containment == 0 ) return true;
  if     ( det == SubDetId::kFGD1 && fgdTrack->Containment != -2 ) return true;
  else if( det == SubDetId::kFGD2 && fgdTrack->Containment != -1 ) return true;
  //else if( det == SubDetId::kFGD  && fgdTrack->Containment != -1 && fgdTrack->Containment != -2) return false;

  double cosFGD = fgdTrack->DirectionStart[2];

  if(cosFGD > -0.3 && cosFGD < 0.3) return true;/// only for systematics issues. Must be remove in the future!!

  if( TMath::Abs(fgdTrack->Pullp) < protonpull) return false;

  return true;
}
