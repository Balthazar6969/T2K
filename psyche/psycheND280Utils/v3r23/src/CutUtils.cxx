#include "CutUtils.hxx"
#include "FiducialVolumeDefinition.hxx"
#include "EventBoxId.hxx"
#include <TVector3.h>
#include <stdio.h>
#include <iostream>
#include <math.h>

//**************************************************
bool cutUtils::TrackQualityCut(AnaTrackB& track){
  //**************************************************

  if(track.TPCQualityCut > -1){
    return track.TPCQualityCut;
  }
  // Gets all segments in the closest TPC
  AnaParticleB* TPCSegment = anaUtils::GetSegmentWithMostNodesInClosestTpc(track);

  if (TPCSegment){
    bool passed = TPCTrackQualityCut(*TPCSegment);
    track.TPCQualityCut = passed;
    return passed;
  }
  else{
    track.TPCQualityCut = 0;
    return false;
  }
}


//**************************************************
bool cutUtils::TrackQualityCut(const AnaTrackB& track){
  //**************************************************

  // Gets all segments in the closest TPC
  AnaParticleB* TPCSegment = anaUtils::GetSegmentWithMostNodesInClosestTpc(track);

  if (TPCSegment){
    bool passed = TPCTrackQualityCut(*TPCSegment);
    return passed;
  }
  else{
    return false;
  }
}

//**************************************************
bool cutUtils::TPCTrackQualityCut(const AnaParticleB& tpcSeg){
  //**************************************************

  Float_t cut = 18;  
  if (tpcSeg.NNodes > cut) 
    return true;
  else
    return false;
}

//**************************************************
bool cutUtils::MuonPIDCut(const AnaTrackB& track, bool prod5Cut){
  //**************************************************

  Float_t cut1 = 0.8;
  Float_t cut2 = 0.05;
  Float_t pcut = 500;

  Float_t PIDLikelihood[4];
  anaUtils::GetPIDLikelihood(track, PIDLikelihood, prod5Cut);

  if (((PIDLikelihood[0]+PIDLikelihood[3])/(1-PIDLikelihood[2]) > cut1 || track.Momentum > pcut ) && (PIDLikelihood[0]>cut2)){
    return true; 
  }

  return false;
}

//**************************************************
bool cutUtils::AntiMuonPIDCut(const AnaTrackB& track){
  //**************************************************

  Float_t cut1 = 0.9;
  Float_t cut2 = 0.1;
  Float_t pcut = 500;

  Float_t PIDLikelihood[4];
  anaUtils::GetPIDLikelihood(track, PIDLikelihood);

  // 18.04.2017
  // Removed the cut on the upper bound,  prod-6
  // Previousely used (PIDLikelihood[0]>cut2 && PIDLikelihood[0]<cut3) with cut3
  // = 0.7
  // For motivation see: http://www.t2k.org/nd280/physics/nu-mu/meetings/2017/0418/antiNumuCCinclSelection_optimization_suvorov 
  
  
  if (((PIDLikelihood[0]+PIDLikelihood[3])/(1-PIDLikelihood[2]) > cut1 || track.Momentum > pcut ) && (PIDLikelihood[0]>cut2)){
    return true; 
  }

  return false;
}

//**************************************************
bool cutUtils::PionPIDCut(const AnaTrackB& track){
  //**************************************************

  Float_t PIDLikelihood[4];
  anaUtils::GetPIDLikelihood(track, PIDLikelihood);

  if (PIDLikelihood[3] < 0.3) return false; 

  double cut1 = (PIDLikelihood[0]+PIDLikelihood[3])/(1.-PIDLikelihood[2]); 

  if (track.Momentum < 500.  && cut1 < 0.8) return false;

  return true;
}

//**************************************************
bool cutUtils::ElectronPIDCut(const AnaTrackB& track){
  //**************************************************

  if (track.nTPCSegments == 0) return false; // There is no TPC segment

  bool seltrack = false; 

  if (track.Momentum < 50.) return seltrack; 

  Float_t pulls[4];

  for (int i = 0; i < track.nTPCSegments ; i++){

    AnaTPCParticleB *tpcTrack = track.TPCSegments[i];

    if (!tpcTrack ) continue; // Extra protection.

    // Pulls are: Muon, Electron, Proton, Pion
    anaUtils::ComputeTPCPull(*tpcTrack, pulls);

    if (TMath::Abs(pulls[0]) > 1.e+6 // muon pull
        || TMath::Abs(pulls[1]) > 1.e+6 // electron pull
        || TMath::Abs(pulls[2]) > 1.e+6 // proton pull
        || TMath::Abs(pulls[3]) > 1.e+6) continue; // pion pull

    if (pulls[1] < -2.0 || pulls[1] > 2.0) break;

    if (track.Charge > 0. &&  (pulls[2] > -4.0 && pulls[2] < 8.0)) break;

    seltrack = true; 

  }
  return seltrack;

}

//**************************************************
bool cutUtils::ProtonPIDCut(const AnaTrackB& track){
  //**************************************************

  if (anaUtils::GetPIDLikelihood(track,2)>0.9)
    return true;
  else    
    return false;
}

//**************************************************
bool cutUtils::FiducialCut(const AnaTrackB& track, const SubDetId::SubDetEnum det){
  //**************************************************
  if (!SubDetId::GetDetectorUsed(track.Detector, det)) return false;
  return FiducialCut(track.PositionStart, det);
}

//**************************************************
bool cutUtils::FiducialCut(const Float_t* pos, const SubDetId::SubDetEnum det){
  //**************************************************

  return anaUtils::InFiducialVolume(det,pos);
}

//********************************************************************
bool cutUtils::DeltaZCut(const AnaTrackB& track){
  //********************************************************************

  Float_t startposz = track.PositionStart[2];
  Float_t endposz   = track.PositionEnd[2];
  TVector3 vect(track.PositionStart[0], track.PositionStart[1], track.PositionStart[2]);
  if (anaUtils::InFiducialVolume(SubDetId::kFGD1, track.PositionStart) && endposz < startposz){
    return false;
  }
  else{
    return true;
  }
}

//********************************************************************
AnaTrackB* cutUtils::FindVetoTrack(const AnaEventB& event, const AnaTrackB& track){
  //********************************************************************


  Float_t Hmom = -9999; 
  AnaTrackB* vetoTrack = NULL;

  // Retrieve the EventBox
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];

  AnaRecObjectC** selTracks = EventBox->RecObjectsInGroup[EventBoxTracker::kTracksWithTPC];
  int nTPC=EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithTPC];


  //loop over tpc tracks
  for (Int_t i=0;i<nTPC; ++i){
    AnaTrackB* track_temp = static_cast<AnaTrackB*>(selTracks[i]);
    Float_t mom = track_temp->Momentum;

    if (&track == track_temp) continue;  // same particle as muon candidate 
    if( mom < 0 ) continue;   // protection for bad momentum
    if( mom > Hmom ) {        // Search for highest momentum track in the bunch that is not he muon. 
      vetoTrack =  track_temp;
      Hmom = mom;
    }
  }

  return vetoTrack;
}

//********************************************************************
bool cutUtils::ExternalVetoCut(const AnaTrackB& candidate, AnaTrackB* vetoTrack){
  //********************************************************************

  Float_t TPCVetoDistance = -150.;

  if(vetoTrack) {
    if(vetoTrack->Original){
      if(vetoTrack->Original->PositionStart[2] - candidate.PositionStart[2] < TPCVetoDistance)  return false;
    }
  }

  return true;
}

//**************************************************
AnaTrackB* cutUtils::FindFGDOOFVtrack(const AnaEventB& event, const AnaTrackB& mainTrack, SubDetId::SubDetEnum det){
  //**************************************************
  /// Get the OOFV FGD track

  /* TODO: we need to now whether the OOFVtracl was already search for in this event
     if (ToyBox->PreviousToyBox->MainTrack && ToyBox->PreviousToyBox->MainTrack == &mainTrack){
     ToyBox->OOFVtrack = ToyBox->PreviousToyBox->OOFVtrack; 
     return;
     }
     */

  EventBoxTracker::RecObjectGroupEnum groupID; 
  if   (det == SubDetId::kFGD1)
    groupID = EventBoxTracker::kTracksWithFGD1AndNoTPC;
  else if (det == SubDetId::kFGD2)
    groupID = EventBoxTracker::kTracksWithFGD2AndNoTPC;
  else 
    return NULL;

  // Retrieve the EventBoxTracker
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];

  //loop over isoFGD tracks
  for (Int_t i=0;i<EventBox->nRecObjectsInGroup[groupID];i++){
    AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[groupID][i]);

    AnaParticleB *fgdTrack = anaUtils::GetSegmentWithMostNodesInDet(*track, det);

    if( !fgdTrack ) continue;

    Float_t* StartPos;

    Float_t dist1 = anaUtils::GetSeparationSquared(fgdTrack->PositionStart, mainTrack.PositionStart);
    Float_t dist2 = anaUtils::GetSeparationSquared(fgdTrack->PositionEnd,   mainTrack.PositionStart);

    Float_t distance = TMath::Min(dist1,dist2); 
    if( distance == dist1 ) StartPos = track->PositionEnd;
    else StartPos = track->PositionStart;/// check that the start position of the isoFGD is well defined
    if(SubDetId::GetDetectorUsed(fgdTrack->Detector,det) && !cutUtils::FiducialCut(StartPos,det) ){
      return track;
    }
  }

  return NULL;
}

//**************************************************
bool cutUtils::CommonVertexCut(const AnaTrackB& track1, const AnaTrackB& track2, int cutx, int cuty, int cutz){
  //**************************************************

  if (fabs(track1.PositionStart[0]-track2.PositionStart[0]) > cutx ) return false; 
  if (fabs(track1.PositionStart[1]-track2.PositionStart[1]) > cuty ) return false; 
  if (fabs(track1.PositionStart[2]-track2.PositionStart[2]) > cutz ) return false;   

  return true;
}

//**************************************************
bool cutUtils::NumuBkgMuonPIDCut(const AnaTrackB& track){
  //**************************************************

  Float_t cut1 = 0.7;
  Float_t cut2 = 0.1;
  Float_t cut3 = 0.8;
  Float_t pcut = 500;

  Float_t PIDLikelihood[4];
  anaUtils::GetPIDLikelihood(track, PIDLikelihood);

  if (((PIDLikelihood[0]+PIDLikelihood[3])/(1-PIDLikelihood[2]) > cut1
        || track.Momentum > pcut ) && (PIDLikelihood[0]>cut2 &&
          PIDLikelihood[0]<cut3)){
    return true;
  }

  return false;
}

//**************************************************
bool cutUtils::MuonECALPIDCut(const AnaTrackB& track){
  //**************************************************

  Float_t cut1 = 0;
  Float_t cut2 = 0.8;

  // if a track enters SMRD then there is no need to apply a cuts since SMRD presence is already a good criteria to select muons
  if ( SubDetId::GetDetectorUsed(track.Detector, SubDetId::kSMRD) ) return true;

  if( track.nECALSegments > 0 ){
    AnaECALParticleB* ECalSeg = static_cast<AnaECALParticleB*>( track.ECALSegments[0] );
    if ( ECalSeg->PIDMipEm>-100 && ECalSeg->Length>0 && ECalSeg->EMEnergy>0 ) {
      if ( ECalSeg->PIDMipEm < cut1 && ECalSeg->Length/ECalSeg->EMEnergy > cut2 ) return true;
    }
  }

  return false;

}

//**************************************************
int cutUtils::StoppingBrECALorSMRDCut(const Float_t* pos) {
  //**************************************************

  if( anaUtils::InFiducialVolume( SubDetId::kTopTECAL,    pos, FVDef::FVdefminTECALTop,    FVDef::FVdefmaxTECALTop)    ) return 0;
  if( anaUtils::InFiducialVolume( SubDetId::kBottomTECAL, pos, FVDef::FVdefminTECALBottom, FVDef::FVdefmaxTECALBottom) ) return 0;
  if( anaUtils::InFiducialVolume( SubDetId::kLeftTECAL,   pos, FVDef::FVdefminTECALLeft,   FVDef::FVdefmaxTECALLeft)   ) return 0;
  if( anaUtils::InFiducialVolume( SubDetId::kRightTECAL,  pos, FVDef::FVdefminTECALRight,  FVDef::FVdefmaxTECALRight)  ) return 0;

  if( anaUtils::InFiducialVolume( SubDetId::kTopSMRD,     pos, FVDef::FVdefminSMRDTop,    FVDef::FVdefmaxSMRDTop)    ) return 1;
  if( anaUtils::InFiducialVolume( SubDetId::kBottomSMRD,  pos, FVDef::FVdefminSMRDBottom, FVDef::FVdefmaxSMRDBottom) ) return 1;
  if( anaUtils::InFiducialVolume( SubDetId::kLeftSMRD,    pos, FVDef::FVdefminSMRDLeft,   FVDef::FVdefmaxSMRDLeft)   ) return 1;
  if( anaUtils::InFiducialVolume( SubDetId::kRightSMRD,   pos, FVDef::FVdefminSMRDRight,  FVDef::FVdefmaxSMRDRight)  ) return 1;

  return -1;

}

//********************************************************************
AnaTrackB* cutUtils::FindP0DVetoTrack(const AnaEventC& event){
  //********************************************************************
  AnaTrackB* vetoTrack = NULL;

  EventBoxB* evtBox = event.EventBoxes[EventBoxId::kEventBoxTracker] ;
  AnaRecObjectC** selTracks = evtBox->RecObjectsInGroup[EventBoxTracker::kTracksWithP0D];
  int np0d = evtBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithP0D];

  //Loop over tracks
  for (Int_t i = 0 ; i < np0d; i++)
  {
    AnaTrackB* track_temp= static_cast<AnaTrackB*>(selTracks[i]);
    bool veto = IsP0DVetoTrack(*track_temp);

    if (veto) {
      vetoTrack = track_temp;
      break;
    }
  }//Loop over P0D tracks

  return vetoTrack;

}

// AI: I`d prefer this to go into detector definitions!

const double p0dVetoMinLimits[3] = {-988,-1020,-3139};
const double p0dVetoMaxLimits[3] = {910,1010,-900};
//"Corridor-" removes 3 most downstream layers
const double p0dVetoEndMaxZ = -992.156;

//***********************************************************************
bool cutUtils::IsOutsideP0DVetoCorridor(const AnaTrackB& track)
  //***********************************************************************
{
  Float_t mom = track.Momentum;
  if (mom < 0 ) return false;

  if (!track.nP0DSegments) return false;

  AnaP0DParticleB* p0dSegment = track.P0DSegments[0];

  float* start = p0dSegment->PositionStart;

  //Find a track starting outside a volume slightly larger than the P0D WT FV
  if (start[0] < p0dVetoMinLimits[0] || start[0] > p0dVetoMaxLimits[0] || start[1] < p0dVetoMinLimits[1] || start[1] > p0dVetoMaxLimits[1] || start[2] < p0dVetoMinLimits[2] || start[2] > p0dVetoMaxLimits[2])
  {
    return true;
  }
  return false;
}

//***********************************************************************
bool cutUtils::IsP0DVetoTrack(const AnaTrackB& track)
  //***********************************************************************
{
  if (!IsOutsideP0DVetoCorridor(track)) return false;
  AnaP0DParticleB* p0dSegment = track.P0DSegments[0];

  float* end = p0dSegment->PositionEnd;
  if (end[0] >= p0dVetoMinLimits[0] && end[0] <= p0dVetoMaxLimits[0] && end[1] >= p0dVetoMinLimits[1] && end[1] <= p0dVetoMaxLimits[1] && end[2] >= p0dVetoMinLimits[2] && end[2] <= p0dVetoEndMaxZ)
    return true;

  return false;
}

//***********************************************************************
bool cutUtils::FGDContainedPionCut(const AnaTrackB& track, SubDetId::SubDetEnum det){
  //***********************************************************************
  if( track.nFGDSegments != 1 ) return false; // There is no FGD segment or it has more than one FGD 

  AnaFGDParticleB *fgdTrack = track.FGDSegments[0];

  if( !fgdTrack  ) return false; // Extra protection. 

  if( TMath::Abs(fgdTrack->Pullp) > 1.e+6 || TMath::Abs(fgdTrack->Pullmu) > 1.e+6 || TMath::Abs(fgdTrack->Pullpi) > 1.e+6  ) return false; 

  // Check the pullnotset variable. 
  // This tells us if the pull was calculated by the algorithm. 
  // If it was not calculated this variable will be 1, so for calculated pulls fgdpulls!=1. 
  if(  fgdTrack->Pullno == 1 ) return false;

  // This tell us something about the geometry. 
  // If the algorithm says the particle started and stopped in 1st/ 2nd fgd this is equal 1 or 2. 
  //  If the particle stopped in 1st/2nd fgd, this is equal -1/-2. Other cases: 0.
  if( fgdTrack->Containment == 0 ) return false;
  if     ( det == SubDetId::kFGD1 && fgdTrack->Containment != 1 ) return false;
  else if( det == SubDetId::kFGD2 && fgdTrack->Containment != 2 ) return false; 
  else if( det == SubDetId::kFGD  && fgdTrack->Containment != 1 && fgdTrack->Containment != 2) return false; 

  double cosFGDpion = fgdTrack->DirectionStart[2];

  if(cosFGDpion > -0.3 && cosFGDpion < 0.3) return false;/// only for systematics issues. Must be remove in the future!!

  if( fgdTrack->Pullpi < 2.5 && fgdTrack->Pullpi > -2.0 ) return true; 

  return false;

}

//*********************************************************************
bool cutUtils::FGDContainedElectronNonMECut(const AnaEventC& event, const AnaTrackB& track, SubDetId::SubDetEnum det){
  //*********************************************************************

  // Exit for the moment since assume detector should be FGD1 or FGD2
  if (!SubDetId::IsFGDDetector(det)){
    std::cout << " cutUtils::FGDContainedElectronNonMECut(): provided detecor " << det << "is not FGD1 or FGD2" << std::endl;
    exit(1);
  } 


  if( track.nFGDSegments != 1 ) return false; // There is no FGD segment or it has more than one FGD 

  //    ToyBoxCCMultiPi * ccmultipibox = static_cast<ToyBoxCCMultiPi*>(&box);

  AnaFGDParticleB *fgdTrack = track.FGDSegments[0];

  if( !fgdTrack  ) return false; // Extra protection. 

  if( TMath::Abs(fgdTrack->Pullp) > 1.e+6 || TMath::Abs(fgdTrack->Pullmu) > 1.e+6 || TMath::Abs(fgdTrack->Pullpi) > 1.e+6  ) return false; 

  // Check the pullnotset variable. 
  // This tells us if the pull was calculated by the algorithm. 
  // If it was not calculated this variable will be 1, so for calculated pulls fgdpulls!=1. 
  if(  fgdTrack->Pullno == 1 ) return false;

  // This tell us something about the geometry. 
  // If the algorithm says the particle started and stopped in 1st/ 2nd fgd this is equal 1 or 2. 
  //  If the particle stopped in 1st/2nd fgd, this is equal -1/-2. Other cases: 0.
  if( fgdTrack->Containment == 0 ) return false; 
  if     ( det == SubDetId::kFGD1 && fgdTrack->Containment != 1 ) return false; 
  else if( det == SubDetId::kFGD2 && fgdTrack->Containment != 2 ) return false; 
  else if( det == SubDetId::kFGD  && fgdTrack->Containment != 1 && fgdTrack->Containment != 2) return false; 

  EventBoxTracker* EventBox = static_cast<EventBoxTracker*>(event.EventBoxes[EventBoxId::kEventBoxTracker]);

  if(EventBox->nFGDMichelElectrons[det] > 0  && fgdTrack->Pullpi < -3.0 ) return true;
  if(EventBox->nFGDMichelElectrons[det] == 0 && fgdTrack->Pullpi < -1.5 ) return true;


  return false;
}

//***********************************************************************
bool cutUtils::IsECalShower(const AnaECALParticleB& particle, int MostUpstreamLayerHitCut, 
    Float_t EMEnergyCut, Float_t PIDMipEmCut){
  //***********************************************************************

  bool shower = false;

  if (particle.EMEnergy > EMEnergyCut && particle.PIDMipEm > PIDMipEmCut) shower = true;

  // This is the cut on MostUpstreamLayerHit variable.
  // The cut threshold has to be provided
  // For more information on this variable, see slide 10: http://www.t2k.org/nd280/physics/convener/2015/November2/ECal_pi0_veto
  if (particle.MostUpStreamLayerHit > MostUpstreamLayerHitCut ) shower = false;

  return shower;
}

//***********************************************************************
bool cutUtils::IsECalShower(const AnaTrackB& track, int MostUpstreamLayerHitCut, 
    Float_t EMEnergyCut, Float_t PIDMipEmCut){
  //***********************************************************************

  if (track.nECALSegments < 1) return false;

  if (!track.ECALSegments[0]) return false;

  return IsECalShower(*(static_cast<AnaECALParticleB*>(track.ECALSegments[0])), 
      MostUpstreamLayerHitCut, EMEnergyCut, PIDMipEmCut);
}


//***********************************************************************
AnaTrackB* cutUtils::GetMostEnergeticIsoTrackInECal(const AnaEventC& event){
  //***********************************************************************

  AnaTrackB* output = NULL;

  EventBoxB* evtBox = event.EventBoxes[EventBoxId::kEventBoxTracker];
  AnaRecObjectC** selTracks = evtBox->RecObjectsInGroup[EventBoxTracker::kIsoTracksInECal];
  if (!selTracks) return output;
  int count = evtBox->nRecObjectsInGroup[EventBoxTracker::kIsoTracksInECal];


  Float_t EMEnergy = 0;

  // Loop over tracks
  for (Int_t i = 0 ; i < count; i++){
    AnaTrackB* track = static_cast<AnaTrackB*>(selTracks[i]);
    if (!track) continue;

    if (track->nECALSegments < 1) continue;

    if (!track->ECALSegments[0]) continue;

    if (track->ECALSegments[0]->EMEnergy > EMEnergy){ 
      output   = track;
      EMEnergy = track->ECALSegments[0]->EMEnergy;
    }
  }

  return output;

}


//*********************************************************************
void cutUtils::FindGoodQualityTPCPionInfoInFGDFV(const AnaEventC& event, const AnaTrackB* reftrack, multipi::MultiPiBox& pionBox, 
    bool useOldSecondaryPID){
  //*********************************************************************

  pionBox.nPositivePionTPCtracks = 0;
  pionBox.nPosPi0TPCtracks       = 0;
  pionBox.nNegativePionTPCtracks = 0;
  pionBox.nElPi0TPCtracks        = 0;

  EventBoxTracker::RecObjectGroupEnum groupID;
  if      (pionBox.Detector == SubDetId::kFGD1) groupID = EventBoxTracker::kTracksWithGoodQualityTPCInFGD1FV;
  else if (pionBox.Detector == SubDetId::kFGD2) groupID = EventBoxTracker::kTracksWithGoodQualityTPCInFGD2FV;
  else return;

  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];

  // Look for pions in positive tracks 
  for(int i = 0; i < EventBox->nRecObjectsInGroup[groupID]; i++ ) {


    AnaTrackB *ptrack = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[groupID][i]);

    if (!ptrack) continue;

    if (reftrack == ptrack ) continue; // Same as the reference track.


    if (ptrack->Charge>0){
      if (useOldSecondaryPID){
        if (cutUtils::PionPIDCut(*ptrack) ) {
          pionBox.PositivePionTPCtracks[pionBox.nPositivePionTPCtracks++] = ptrack;
        }
        else if (cutUtils::ElectronPIDCut(*ptrack)) {
          pionBox.PosPi0TPCtracks[pionBox.nPosPi0TPCtracks++] = ptrack; 
        }
      }
      else {
        Float_t PIDLikelihood[4];
        anaUtils::GetPIDLikelihood(*ptrack, PIDLikelihood);

        // For Positive tracks we distinguish pions, electrons and protons.
        double ElLklh = PIDLikelihood[1];  
        double ProtonLklh = PIDLikelihood[2];  
        double PionLklh = PIDLikelihood[3];  
        double norm = ElLklh+ProtonLklh+PionLklh;
        ProtonLklh /= norm; 
        ElLklh /= norm; 
        PionLklh /= norm; 

        if( ProtonLklh > ElLklh && ProtonLklh > PionLklh ) continue; // If the highest probability is a proton continue. 

        // Id associated to the largest of the two probabilities.
        if (PionLklh > ElLklh){
          pionBox.PositivePionTPCtracks[pionBox.nPositivePionTPCtracks++] = ptrack;
        }
        else {
          if (ptrack->Momentum > 900.) continue; // This is mainly protons.
          pionBox.PosPi0TPCtracks[pionBox.nPosPi0TPCtracks++] = ptrack; 
        }
      }
    }
    else{
      if(useOldSecondaryPID) {
        if (cutUtils::PionPIDCut(*ptrack)) {
          pionBox.NegativePionTPCtracks[pionBox.nNegativePionTPCtracks++] = ptrack;
        } 
        else if (cutUtils::ElectronPIDCut(*ptrack)) {
          pionBox.ElPi0TPCtracks[pionBox.nElPi0TPCtracks++] = ptrack; 
        }
      }
      else {
        // For Negative tracks we distinguish pions and electrons
        Float_t PIDLikelihood[4];
        anaUtils::GetPIDLikelihood(*ptrack, PIDLikelihood);

        double ElLklh = PIDLikelihood[1];  
        double PionLklh = PIDLikelihood[3];  
        double norm = ElLklh+PionLklh;
        ElLklh /= norm; 
        PionLklh /= norm;

        if( PionLklh > 0.8 ){ // Id associated to the largest of the two probabilities.
          pionBox.NegativePionTPCtracks[pionBox.nNegativePionTPCtracks++] = ptrack;
        }
        else{ 
          pionBox.ElPi0TPCtracks[pionBox.nElPi0TPCtracks++] = ptrack; 
        }
      }
    }
  }
}


//*********************************************************************
void cutUtils::FindIsoFGDPionInfo(const AnaEventC& event, multipi::MultiPiBox& pionBox){
  //*********************************************************************
  pionBox.nIsoFGDPiontracks = 0;
  pionBox.nIsoFGDElPi0tracks = 0;

  EventBoxTracker::RecObjectGroupEnum groupID;
  if      (pionBox.Detector == SubDetId::kFGD1) groupID = EventBoxTracker::kTracksWithFGD1AndNoTPC;
  else if (pionBox.Detector == SubDetId::kFGD2) groupID = EventBoxTracker::kTracksWithFGD2AndNoTPC;
  else return;

  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];

  //loop over tracks
  for (int i=0; i < EventBox->nRecObjectsInGroup[groupID]; i++){
    AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[groupID][i]);

    if (!track) continue;

    if (cutUtils::FGDContainedPionCut(*track, pionBox.Detector)){ 
      pionBox.IsoFGDPiontracks[pionBox.nIsoFGDPiontracks++] = track;
    }
    else if (cutUtils::FGDContainedElectronNonMECut(event, *track, pionBox.Detector)){
      pionBox.IsoFGDElPi0tracks[pionBox.nIsoFGDElPi0tracks++] = track;
    }
  }

}

//*********************************************************************
void cutUtils::FillPionInfo(const AnaEventC& event, multipi::MultiPiBox& pionBox, const multipi::PionSelectionParams& params){
  //*********************************************************************

  EventBoxTracker* EventBox = static_cast<EventBoxTracker*>(event.EventBoxes[EventBoxId::kEventBoxTracker]);
  if (!EventBox){
    std::cout << " cutUtils::FillPionInfo(): EventBoxTracker not available " << std::endl;  
    exit(1);
  }

  if (!SubDetId::IsFGDDetector(pionBox.Detector)){
    std::cout << " cutUtils::FillPionInfo(): provided detecor " << pionBox.Detector << "is not FGD1 or FGD2" << std::endl;
    exit(1);
  } 

  if (params.useTPCPions) cutUtils::FindGoodQualityTPCPionInfoInFGDFV(event, params.refTrack, 
      pionBox, params.useOldSecondaryPID); 

  if (params.useFGDPions) cutUtils::FindIsoFGDPionInfo(event, pionBox);

  if (params.useME)  pionBox.nMichelElectrons =  EventBox->nFGDMichelElectrons[pionBox.Detector];

  // ECal 
  if (params.useECalPiZeroInfo){

    // Get the most energetic ECal iso object from the event 
    AnaTrackB* track = cutUtils::GetMostEnergeticIsoTrackInECal(event);
    if (track && cutUtils::IsECalShower(*track, 
          params.ECalMostUpstreamLayerHitCut, 
          params.ECalEMEnergyCut, 
          params.ECalPIDMipEmCut)){
      pionBox.ECalPi0Photon  = track; 
    }

  }
  return;

}

//*********************************************************************
bool cutUtils::ECalPi0VetoGeneralCutBase::Apply( AnaEventC& event, ToyBoxB& box) const{
  //*********************************************************************

  (void)box;
  // Get the most energetic ECal iso object from the event 
  AnaTrackB* track = cutUtils::GetMostEnergeticIsoTrackInECal(event);
  if (!track) return true;

  return (!cutUtils::IsECalShower(*track, _MostUpstreamLayerHitCut));
}


//***********************************************************************
bool cutUtils::IsECALPi0Photon(AnaEventC& event, AnaTrackB *HO, int MostUpstreamLayerHitCut, SubDetId::SubDetEnum det){
  //***********************************************************************

  (void)event;
  (void)det;

  // If not, then the ecal object might comes from a pi0 (if the object is shower like)

  bool Ecalpi0 = false;

  Float_t EMENERGY    = -9999.;
  Float_t MIPEM       = -9999.;
  Float_t EM_UPSTREAM = -9999;
  AnaECALParticleB* EcalSegment = HO->ECALSegments[0];
  if (!EcalSegment) return Ecalpi0; 

  // Check that the Highest energetic ecal obj is actually shower-like (MIPEM>0)
  // Cut on EMEnergy because of big spike around 26 MeV (feature of the reconstruction)
  MIPEM    = EcalSegment->PIDMipEm;
  EMENERGY = EcalSegment->EMEnergy;
  if (EMENERGY > 30. && MIPEM > 0.) Ecalpi0 = true;

  // This is the cut on MostUpstreamLayerHit variable.
  // The cut threshold has to be provided
  // For more information on this variable, see slide 10: http://www.t2k.org/nd280/physics/convener/2015/November2/ECal_pi0_veto
  EM_UPSTREAM = EcalSegment->MostUpStreamLayerHit;
  if (EM_UPSTREAM > MostUpstreamLayerHitCut ) Ecalpi0 = false;

  return Ecalpi0;
}

    
//*********************************************************************
bool cutUtils::numuCCTrkMultiPi::CheckTrueCausesBoxSimple(const AnaTrueParticleB& trueTrack, const multipi::MultiPiBox& box){
//*********************************************************************

  // Check all possible objects
  for (int i = 0; i < box.nNegativePionTPCtracks; i++){
    if (box.NegativePionTPCtracks[i]) continue;
    if (anaUtils::CheckTrueCausesRecoSimple(trueTrack, *box.NegativePionTPCtracks[i]))
      return true;
  }
  
  for (int i = 0; i < box.nPositivePionTPCtracks; i++){
    if (box.PositivePionTPCtracks[i]) continue;
    if (anaUtils::CheckTrueCausesRecoSimple(trueTrack, *box.PositivePionTPCtracks[i]))
      return true;
  }
  
  for (int i = 0; i < box.nIsoFGDPiontracks; i++){
    if (box.IsoFGDPiontracks[i]) continue;
    if (anaUtils::CheckTrueCausesRecoSimple(trueTrack, *box.IsoFGDPiontracks[i]))
      return true;
  }
  
  for (int i = 0; i < box.nPosPi0TPCtracks; i++){
    if (box.PosPi0TPCtracks[i]) continue;
    if (anaUtils::CheckTrueCausesRecoSimple(trueTrack, *box.PosPi0TPCtracks[i]))
      return true;
  }
  
  for (int i = 0; i < box.nElPi0TPCtracks; i++){
    if (box.ElPi0TPCtracks[i]) continue;
    if (anaUtils::CheckTrueCausesRecoSimple(trueTrack, *box.ElPi0TPCtracks[i]))
      return true;
  }
  return false;
}

//*********************************************************************
bool cutUtils::numuCCTrkMultiPi::NoPionCutBase::Apply(AnaEventC& event, ToyBoxB& box) const{
  //*********************************************************************

  (void)event;

  const multipi::MultiPiBox& pionBox = GetPionBox(box);

  if (pionBox.nPosPions + pionBox.nOtherPions + pionBox.nNegPions == 0) return true; 
  
  return false; 
}


//*********************************************************************
bool cutUtils::numuCCTrkMultiPi::OnePionCutBase::Apply(AnaEventC& event, ToyBoxB& box) const{
  //*********************************************************************

  (void)event;

  const multipi::MultiPiBox& pionBox = GetPionBox(box);
  
  if (pionBox.nOtherPions != 0) return false; 
  
  bool ok = _posPionMode ? (pionBox.nPosPions == 1) : (pionBox.nNegPions == 1);
  
  return ok;
}


//*********************************************************************
bool cutUtils::numuCCTrkMultiPi::OthersCutBase::Apply(AnaEventC& event, ToyBoxB& box) const{
  //*********************************************************************

  (void)event;

  const multipi::MultiPiBox& pionBox = GetPionBox(box);

  // More than one pion of interest or more than zero other pions or event doesn't pass the pi0 veto
  if (pionBox.nOtherPions != 0) return true; 
 
  // Both cases give CC-other 
  if (pionBox.nPosPions > 1 || pionBox.nNegPions > 1) return true;

  if (pionBox.ECalPi0Photon) return true;
  
  return false; 
}

//*********************************************************************
bool cutUtils::numuCCTrkMultiPi::ECalPi0VetoCutBase::Apply(AnaEventC& event, ToyBoxB& box) const{
  //*********************************************************************

  (void)event;
  const multipi::MultiPiBox& pionBox = GetPionBox(box);
  
  if (pionBox.ECalPi0Photon) return false;
  
  return true;
}


