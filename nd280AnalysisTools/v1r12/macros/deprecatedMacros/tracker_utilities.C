/// Helper methods for Tracker CCQE analysis


/// Fiure out if position is inside volume defined by minpos and maxpos.
/// Assumes we are dealing with a box that is perfectly aligned with 
/// the coordinate system.
bool IsInvolume(TVector3 pos, TVector3 minpos, TVector3 maxpos){

  if(pos[0] >= minpos[0] && 
     pos[1] >= minpos[1] &&
     pos[2] >= minpos[2] &&
     pos[0] <= maxpos[0] && 
     pos[1] <= maxpos[1] && 
     pos[2] <= maxpos[2])
    return true;

  return false;
    
}

// Is position in Fiducial FGD1?
bool IsInFiducialFGD1(TVector3 pos, TVector3 fgd_trim){
  
  TVector3 fFGD1ActiveMin = TVector3(-931.6, -856.6, 193.558);
  TVector3 fFGD1ActiveMax = TVector3(931.6, 1006.6, 513.758);

  TVector3 fFGD1FiducialMin = fFGD1ActiveMin + fgd_trim;
  TVector3 fFGD1FiducialMax = fFGD1ActiveMax - fgd_trim;;

  return IsInvolume(pos,fFGD1FiducialMin,fFGD1FiducialMax);
}

bool IsInFiducialFGD1(TLorentzVector pos, TVector3 fgd_trim){
  
  TVector3 tmp_pos(pos.X(),pos.Y(),pos.Z());
  return IsInFiducialFGD1(tmp_pos,fgd_trim);
}


// Is position in Fiducial FGD2?
bool IsInFiducialFGD2(TVector3 pos, TVector3 fgd_trim){

  TVector3 fFGD2ActiveMin = TVector3(-931.6, -856.6, 1542.26);
  TVector3 fFGD2ActiveMax = TVector3(931.6, 1006.6, 1863.86);
  TVector3 fFGD2FiducialMin = fFGD2ActiveMin + fgd_trim;
  TVector3 fFGD2FiducialMax = fFGD2ActiveMax - fgd_trim;;

  return IsInvolume(pos,fFGD2FiducialMin,fFGD2FiducialMax);
}

bool IsInFiducialFGD2(TLorentzVector pos, TVector3 fgd_trim){
  TVector3 tmp_pos(pos.X(),pos.Y(),pos.Z());
  return IsInFiducialFGD2(tmp_pos,fgd_trim);
}


/// ________________________________________________________________________
// Truth helper routines


// Get reaction code for this true vertex.  Works only for NEUT!!! 
int GetReactionCode(ND::TTruthVerticesModule::TTruthVertex *vertex){

  TString path(vertex->ReactionCode);
  Ssiz_t start = path.Index("NEUT ") + 5;
  TString *lll;
  lll = new TString(path(start,2));

  return lll->Atoi();

}


// ________________________________________________________________________
// Track helper routines

// Return the TPC measured sign of charge for this track.
// Measurement assumes forward going track.
int getTrackTpcCharge(ND::TTrackerReconModule::TTrackerTrack *track){

  // Return the first charge measurement we find.  
  // In future, should check whether charge measurements agree.
  for(int i = 0; i < 3; i++)
    if(track->tpc_charge[i] != 0.0)
      return (int)track->tpc_charge[i];
    
  return 0;
}

// Return the TPC momentum.   Just use first TPC momentum measurement we find.
double getTrackMomentum(ND::TTrackerReconModule::TTrackerTrack *track){
    
    if(!track)
        return 0;


  // In future, should check whether momentum measurements agree.
  for(int i = 0; i < 3; i++)
    if(track->tpc_momentum[i] != 0.0)
      return track->tpc_momentum[i];
    
  return 0.0;
}


// Return the track angle
double getTrackCosAngle(ND::TTrackerReconModule::TTrackerTrack *track){

  double cosangle = 0.0;

  if(track->isForwards){  // Forward going.
    TVector3 dir = track->FrontDirection;
    cosangle = TMath::Cos(dir.Theta());
  }else{
    TVector3 dir = track->BackDirection;
    dir[2] = -1.0;
    cosangle = TMath::Cos(dir.Theta()); 
  }

  return cosangle;
}

