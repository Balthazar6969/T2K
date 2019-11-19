#include "ToFSenseCorrector.hxx"
#include "Parameters.hxx"
#include "BasicUtils.hxx"
#include "DetectorDefinition.hxx"
#include "ND280AnalysisUtils.hxx"
#include "CutUtils.hxx"
//#define DEBUG


//********************************************************************
ToFSenseCorrector::ToFSenseCorrector(){
  //********************************************************************

  // Read various parameters
  ToFCutValues[ToFSenseCorrector::kToFTopo_FGD1FGD2]                   = (Float_t)ND::params().GetParameterD("psycheND280Utils.ToF.FGD1FGD2"                   );
  ToFCutValues[ToFSenseCorrector::kToFTopo_FGD1P0D_Track]              = (Float_t)ND::params().GetParameterD("psycheND280Utils.ToF.FGD1P0D_Track"              );
  ToFCutValues[ToFSenseCorrector::kToFTopo_FGD1ECAL_LAStartFgd_Track]  = (Float_t)ND::params().GetParameterD("psycheND280Utils.ToF.FGD1ECAL_LAStartFgd_Track"  );
  ToFCutValues[ToFSenseCorrector::kToFTopo_FGD1ECAL_LAStartFgd_Shower] = (Float_t)ND::params().GetParameterD("psycheND280Utils.ToF.FGD1ECAL_LAStartFgd_Shower" );
  ToFCutValues[ToFSenseCorrector::kToFTopo_FGD1ECAL_LAEndFgd_Track]    = (Float_t)ND::params().GetParameterD("psycheND280Utils.ToF.FGD1ECAL_LAEndFgd_Track"    );
  ToFCutValues[ToFSenseCorrector::kToFTopo_FGD1ECAL_LAEndFgd_Shower]   = (Float_t)ND::params().GetParameterD("psycheND280Utils.ToF.FGD1ECAL_LAEndFgd_Shower"   );
  ToFCutValues[ToFSenseCorrector::kToFTopo_FGD1ECAL_HAStartFgd_Track]  = (Float_t)ND::params().GetParameterD("psycheND280Utils.ToF.FGD1ECAL_HAStartFgd_Track"  );
  ToFCutValues[ToFSenseCorrector::kToFTopo_FGD1ECAL_HAStartFgd_Shower] = (Float_t)ND::params().GetParameterD("psycheND280Utils.ToF.FGD1ECAL_HAStartFgd_Shower" );
  ToFCutValues[ToFSenseCorrector::kToFTopo_FGD1ECAL_HAEndFgd_Track]    = (Float_t)ND::params().GetParameterD("psycheND280Utils.ToF.FGD1ECAL_HAEndFgd_Track"    );
  ToFCutValues[ToFSenseCorrector::kToFTopo_FGD1ECAL_HAEndFgd_Shower]   = (Float_t)ND::params().GetParameterD("psycheND280Utils.ToF.FGD1ECAL_HAEndFgd_Shower"   );

}

//********************************************************************
void ToFSenseCorrector::FlipTrack(AnaTrackB& track){
  //********************************************************************

  //Flip the charge
  track.Charge = -1.*track.Charge;

  // Position
  Float_t pos_tmp[4];
  anaUtils::CopyArray(track.PositionStart, pos_tmp,             4);
  anaUtils::CopyArray(track.PositionEnd,   track.PositionStart, 4);
  anaUtils::CopyArray(pos_tmp,             track.PositionEnd,   4);

  // DirectionEnd is proved to provide more stable results than reverse refit: (acceptably) worse resolution but
  // not fit failures
  Float_t dir_tmp_start[3] = {-1.*track.DirectionStart[0],  -1.*track.DirectionStart[1], -1.*track.DirectionStart[2]}; 
  Float_t dir_tmp_end[3]   = {-1.*track.DirectionEnd[0],    -1.*track.DirectionEnd[1],   -1.*track.DirectionEnd[2]  }; 
  anaUtils::CopyArray(dir_tmp_end,    track.DirectionStart,  3); 
  anaUtils::CopyArray(dir_tmp_start,  track.DirectionEnd,    3); 

  // Check whether a track uses TPC
  if (SubDetId::GetDetectorUsed(track.Detector, SubDetId::kTPC))
    //Momentum
    std::swap(track.Momentum, track.MomentumFlip);

}

//********************************************************************
bool ToFSenseCorrector::IsForward(const AnaTrackB& track){
  //********************************************************************

  // check if a track is forward (in Z) going
  // need something fancier?

  return track.PositionStart[2] < track.PositionEnd[2];  
}


//********************************************************************
bool ToFSenseCorrector::IsForward(const AnaTrueParticleB& track){
  //********************************************************************

  // check if a track is forward (in Z) going
  // need something fancier?

  return track.Position[2] < track.PositionEnd[2];  
}



//********************************************************************
void ToFSenseCorrector::ForceForward(AnaTrackB& track){
  //********************************************************************

  if (!IsForward(track)) FlipTrack(track);  
}


//********************************************************************
void ToFSenseCorrector::ForceForward(AnaTrackB* Tracks[], int nTracks){
  //********************************************************************
  for (int i=0; i<nTracks; i++){
    if (!Tracks[i]) continue;
    ForceForward(*Tracks[i]);
  }    
}


//********************************************************************
bool ToFSenseCorrector::CheckNeedApplyToFBasedFlip(const AnaTrackB& track, ToFSenseCorrector::ToFTopology topo) const{
  //********************************************************************

  /* The ToF cuts were studied to select Bwd going tracks in case of FGD-FGD/P0D ToF(1) and
   * those that start from FGD for FGD-ECal (ToF2) cases. So here the following checks are done:
   * track should be reverese in case:
   * isForward but ToF1 > cut
   * isBackward but ToF1 < cut
   * if not valid ToF --> do not reverse
   */

  bool isForward = IsForward(track);

  //FGD1-FGD2
  if (topo == kToFTopo_FGD1FGD2) {
    if ((track.ToF.FGD1_FGD2 <= ToFCutValues[ToFSenseCorrector::kToFTopo_FGD1FGD2] && !isForward) ||
        (track.ToF.FGD1_FGD2 >  ToFCutValues[ToFSenseCorrector::kToFTopo_FGD1FGD2] &&  isForward)) return true;
  }

  // P0D-FGD1 -> We apply the flip only for track like cases
  if (topo == ToFSenseCorrector::kToFTopo_FGD1P0D_Track) {
    if ((track.ToF.P0D_FGD1 <= ToFCutValues[ToFSenseCorrector::kToFTopo_FGD1P0D_Track] && !isForward) ||
        (track.ToF.P0D_FGD1 >  ToFCutValues[ToFSenseCorrector::kToFTopo_FGD1P0D_Track] &&  isForward)) return true;
  }

  // ECal-FGD1 -> We apply the flip for both track&shower like cases
  if (topo == ToFSenseCorrector::kToFTopo_FGD1ECAL_LAStartFgd_Track) {
    if ((track.ToF.ECal_FGD1 >  ToFCutValues[ToFSenseCorrector::kToFTopo_FGD1ECAL_LAStartFgd_Track] && !isForward) ||
        (track.ToF.ECal_FGD1 <= ToFCutValues[ToFSenseCorrector::kToFTopo_FGD1ECAL_LAStartFgd_Track] &&  isForward)) return true;
  }
  if (topo == ToFSenseCorrector::kToFTopo_FGD1ECAL_LAStartFgd_Shower) {
    if ((track.ToF.ECal_FGD1 >  ToFCutValues[ToFSenseCorrector::kToFTopo_FGD1ECAL_LAStartFgd_Shower] && !isForward) ||
        (track.ToF.ECal_FGD1 <= ToFCutValues[ToFSenseCorrector::kToFTopo_FGD1ECAL_LAStartFgd_Shower] &&  isForward)) return true;
  }

  if (topo == ToFSenseCorrector::kToFTopo_FGD1ECAL_LAEndFgd_Track) {
    if ((track.ToF.ECal_FGD1 <= ToFCutValues[ToFSenseCorrector::kToFTopo_FGD1ECAL_LAEndFgd_Track] && !isForward) ||
        (track.ToF.ECal_FGD1 >  ToFCutValues[ToFSenseCorrector::kToFTopo_FGD1ECAL_LAEndFgd_Track] &&  isForward)) return true;
  }
  if (topo == ToFSenseCorrector::kToFTopo_FGD1ECAL_LAEndFgd_Shower) {
    if ((track.ToF.ECal_FGD1 <= ToFCutValues[ToFSenseCorrector::kToFTopo_FGD1ECAL_LAEndFgd_Shower] && !isForward) ||
        (track.ToF.ECal_FGD1 >  ToFCutValues[ToFSenseCorrector::kToFTopo_FGD1ECAL_LAEndFgd_Shower] &&  isForward)) return true;
  } 

  if (topo == ToFSenseCorrector::kToFTopo_FGD1ECAL_HAStartFgd_Track) {
    if ((track.ToF.ECal_FGD1 >  ToFCutValues[ToFSenseCorrector::kToFTopo_FGD1ECAL_HAStartFgd_Track] && !isForward) ||
        (track.ToF.ECal_FGD1 <= ToFCutValues[ToFSenseCorrector::kToFTopo_FGD1ECAL_HAStartFgd_Track] &&  isForward)) return true;
  }  
  if (topo == ToFSenseCorrector::kToFTopo_FGD1ECAL_HAStartFgd_Shower) {
    if ((track.ToF.ECal_FGD1 >  ToFCutValues[ToFSenseCorrector::kToFTopo_FGD1ECAL_HAStartFgd_Shower] && !isForward) ||
        (track.ToF.ECal_FGD1 <= ToFCutValues[ToFSenseCorrector::kToFTopo_FGD1ECAL_HAStartFgd_Shower] &&  isForward)) return true;
  }  

  if (topo == ToFSenseCorrector::kToFTopo_FGD1ECAL_HAEndFgd_Track) {
    if ((track.ToF.ECal_FGD1 <= ToFCutValues[ToFSenseCorrector::kToFTopo_FGD1ECAL_HAEndFgd_Track] && !isForward) ||
        (track.ToF.ECal_FGD1 >  ToFCutValues[ToFSenseCorrector::kToFTopo_FGD1ECAL_HAEndFgd_Track] &&  isForward)) return true;
  }    
  if (topo == ToFSenseCorrector::kToFTopo_FGD1ECAL_HAEndFgd_Shower) {
    if ((track.ToF.ECal_FGD1 <= ToFCutValues[ToFSenseCorrector::kToFTopo_FGD1ECAL_HAEndFgd_Shower] && !isForward) ||
        (track.ToF.ECal_FGD1 >  ToFCutValues[ToFSenseCorrector::kToFTopo_FGD1ECAL_HAEndFgd_Shower] &&  isForward)) return true;
  }  

  // Everything is fine or cannot say that any flipping is needed
  return false;

}

//********************************************************************
ToFSenseCorrector::ToFTopology ToFSenseCorrector::GetToFTopology(const AnaTrackB& track){
  //********************************************************************

  // This function returns which type of ToF check/cut (there may be several involved since a 
  // track can cross several sub-detectors and the check/cut can also depend on the type of track, 
  // e.g. track/shower for the same topology)


  // The current values (Sept 2016,  details in TN-) used were retrieved using the track being forced to be forward going in Z 
  // hence the relevant type should be checked based on the position of the upstream/downstream (in Z) ends,  no matter there correspondence 
  // to an actual start/end point

  const Float_t* pos_upstream;
  const Float_t* pos_downstream;
  bool forward = IsForward(track);
  pos_upstream   = forward ? track.PositionStart : track.PositionEnd;
  pos_downstream = forward ? track.PositionEnd   : track.PositionStart;

  // Four different topologies -> Fwd, HAFwd, Bwd, HABwd
  if (anaUtils::InDetVolume(SubDetId::kFGD1, pos_upstream)) {
    if (cutUtils::TrackQualityCut(track)) {
      if (track.ToF.Flag_FGD1_FGD2) return kToFTopo_FGD1FGD2;
      else if (track.ToF.Flag_ECal_FGD1) {
        if (ToFSenseCorrector::IsBarrelECAL(track)) {
          if (track.ECALSegments[0]->NNodes == 1) return kToFTopo_FGD1ECAL_LAStartFgd_Shower;
          if (track.ECALSegments[0]->NNodes >  1) return kToFTopo_FGD1ECAL_LAStartFgd_Track;
        }
      }
    }
    else {
      if (cutUtils::StoppingBrECALorSMRDCut(pos_downstream)!=-1) {
        if (track.ToF.Flag_ECal_FGD1) {
          if (ToFSenseCorrector::IsBarrelECAL(track)) {
            if (track.ECALSegments[0]->NNodes == 1) return kToFTopo_FGD1ECAL_HAStartFgd_Shower;
            if (track.ECALSegments[0]->NNodes >  1) return kToFTopo_FGD1ECAL_HAStartFgd_Track;
          }
        }
      }
    }
  }
  else if (anaUtils::InDetVolume(SubDetId::kFGD1, pos_downstream)) {
    if (cutUtils::TrackQualityCut(track)) {
      if (track.ToF.Flag_P0D_FGD1) {
        if (track.nP0DSegments > 0) {
          if (track.P0DSegments[0]) {
            if (track.P0DSegments[0]->NNodes >  1) return kToFTopo_FGD1P0D_Track;  
          }
        }
      }
      else if (track.ToF.Flag_ECal_FGD1) {
        if (ToFSenseCorrector::IsBarrelECAL(track)) {
          if (track.ECALSegments[0]->NNodes == 1) return kToFTopo_FGD1ECAL_LAEndFgd_Shower;
          if (track.ECALSegments[0]->NNodes >  1) return kToFTopo_FGD1ECAL_LAEndFgd_Track;
        }
      }
    }
    else {
      if (cutUtils::StoppingBrECALorSMRDCut(pos_upstream)!=-1) {
        if (track.ToF.Flag_ECal_FGD1) {
          if (ToFSenseCorrector::IsBarrelECAL(track)) {
            if (track.ECALSegments[0]->NNodes == 1) return kToFTopo_FGD1ECAL_HAEndFgd_Shower;
            if (track.ECALSegments[0]->NNodes >  1) return kToFTopo_FGD1ECAL_HAEndFgd_Track;
          }
        }
      }
    }
  }

  return kToFInvalid;

}

//********************************************************************
bool ToFSenseCorrector::IsBarrelECAL(const AnaTrackB& track){
  //********************************************************************

  if (track.nECALSegments < 1) return false; 

  int count = (int)(anaUtils::TrackUsesDet(track, SubDetId::kTopTECAL)) + 
    (int)(anaUtils::TrackUsesDet(track, SubDetId::kBottomTECAL))      + 
    (int)(anaUtils::TrackUsesDet(track, SubDetId::kLeftTECAL))        + 
    (int)(anaUtils::TrackUsesDet(track, SubDetId::kRightTECAL));
  if (count != 1) return false;

  return track.ECALSegments[0];

}
