#include "ConstituentsUtils.hxx"
#include <TVector3.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <typeinfo>
#include "BaseDataClasses.hxx"
#include "CutUtils.hxx"
#include "SubDetId.hxx"

#include <bitset>

namespace anaUtils {}


//**************************************************
Int_t anaUtils::GetOneSegmentPerTPC(AnaTPCParticleB* in[], Int_t nseg_in, AnaTPCParticleB* out[]) {
//**************************************************

  // This method takes as input all TPC segments in a track and returns an array with only one segment per TPC, the one with more nodes

    Int_t nnodes_max[3]={0,0,0};
    Int_t itrack_nnodes_max[3]={-1,-1,-1};
    for(Int_t iseg=0; iseg< nseg_in; iseg++){
      Int_t tpc = SubDetId::GetTPC(in[iseg]->Detector)-1;
      if (in[iseg]->NNodes > nnodes_max[tpc]){
        nnodes_max[tpc] = in[iseg]->NNodes;
        itrack_nnodes_max[tpc]=iseg;
      }      
    }

    int nseg_out = 0;    
    for(Int_t i=0;i< 3;i++){
      if (itrack_nnodes_max[i]!=-1)
        out[nseg_out++]=in[itrack_nnodes_max[i]];
    }
    
    return nseg_out;
}

//**************************************************
SubDetId::SubDetEnum anaUtils::GetClosestTPC(const AnaTrackB& track){
//**************************************************

    // returns the TPC closest to the track start point
    // simply use the linear distance for now 

  /*
    SubDetId::SubDetEnum tpc_closest = SubDetId::kInvalid;

    Float_t dist = 9999999.;

    for(int i = 0; i < track.nTPCSegments; ++i){
        AnaTPCParticleB* tpc_track = track.TPCSegments[i];
        Float_t dist_tmp = GetSeparationSquared(track.PositionStart, tpc_track->PositionStart);
        if(dist_tmp < dist){
            dist = dist_tmp;
            tpc_closest = SubDetId::GetSubdetectorEnum(tpc_track->Detector);
        }
    }
    return tpc_closest;  
  */

  AnaParticleB* TPCSegment = anaUtils::GetSegmentWithMostNodesInClosestTpc(track);
  
  if (TPCSegment) return SubDetId::GetSubdetectorEnum(TPCSegment->Detector);
  else return SubDetId::kInvalid;
}

//**************************************************
int anaUtils::GetSegmentsInDet(const AnaTrackB& track, SubDetId::SubDetEnum det, AnaParticleB* segments[]){
//**************************************************
    if (det == SubDetId::kInvalid) {
        return 0;
    }
    if(!SubDetId::GetDetectorUsed(track.Detector, det)){
        return 0;
    }

    int count = 0;

    // Return segments for complete detector subsystems (all TPC etc.) first
    switch(det){
        case SubDetId::kTPC :
            std::copy(&track.TPCSegments[0], &track.TPCSegments[track.nTPCSegments], segments);
            return track.nTPCSegments;
            break;
        case SubDetId::kFGD :
            std::copy(&track.FGDSegments[0], &track.FGDSegments[track.nFGDSegments], segments);
            return track.nFGDSegments;
            break;

        case SubDetId::kECAL :
            std::copy(&track.ECALSegments[0], &track.ECALSegments[track.nECALSegments], segments);
            return track.nECALSegments;
            break;

        case SubDetId::kSMRD :
            std::copy(&track.SMRDSegments[0], &track.SMRDSegments[track.nSMRDSegments], segments);
            return track.nSMRDSegments;
            break;

        case SubDetId::kP0D :
            std::copy(&track.P0DSegments[0], &track.P0DSegments[track.nP0DSegments], segments);
            return track.nP0DSegments;
            break;
        default:

            if(SubDetId::IsTPCDetector(det)){
                for(int i = 0; i < track.nTPCSegments; ++i){
                    AnaTPCParticleB* tpc_track = track.TPCSegments[i];
                    if(SubDetId::GetDetectorUsed(tpc_track->Detector, det)){
                        segments[count] = tpc_track;
                        ++count;
                    }
                }
            }
            else if(SubDetId::IsFGDDetector(det)){
                for(int i = 0; i < track.nFGDSegments; ++i){
                    AnaFGDParticleB* fgd_track = track.FGDSegments[i];
                    if(SubDetId::GetDetectorUsed(fgd_track->Detector, det)){
                        segments[count] = fgd_track;
                        ++count;
                    }
                }
            }
            else if(SubDetId::IsECALDetector(det)){
                for(int i = 0; i < track.nECALSegments; ++i){
                    AnaECALParticleB* ecal_track = track.ECALSegments[i];
                    if(SubDetId::GetDetectorUsed(ecal_track->Detector, det)){
                        segments[count] = ecal_track;
                        ++count;
                    }
                }
            }
            else if(SubDetId::IsSMRDDetector(det)){
                for(int i = 0; i < track.nSMRDSegments; ++i){
                    AnaSMRDParticleB* smrd_track = track.SMRDSegments[i];
                    if(SubDetId::GetDetectorUsed(smrd_track->Detector, det)){
                        segments[count] = smrd_track;
                        ++count;
                    }
                }
            }
            return count;
    }
    return count;
}

//**************************************************
AnaParticleB* anaUtils::GetSegmentWithMostNodesInClosestTpc(const AnaTrackB& track){
//**************************************************

  return GetSegmentWithMostNodesInClosestTpc(track, track.PositionStart);
}

//**************************************************
AnaParticleB* anaUtils::GetSegmentWithMostNodesInClosestTpc(const AnaTrackB& track, const Float_t* pos, bool quality_cut){
//**************************************************

    int tpc_closest = SubDetId::kInvalid;
    int tpc = SubDetId::kInvalid;

    AnaParticleB* subtrack[3] = {NULL, NULL, NULL};

    Float_t dist = 9999999.;
    int nNodes[3] = {0,0,0};

    for(int i = 0; i < track.nTPCSegments; ++i){
        AnaTPCParticleB* tpc_track = track.TPCSegments[i];
        
        if (!tpc_track) continue;
        
        if (quality_cut && !cutUtils::TPCTrackQualityCut(*tpc_track))
          continue;
        
        Float_t dist_tmp = std::min(
            GetSeparationSquared(pos, tpc_track->PositionStart), 
            GetSeparationSquared(pos, tpc_track->PositionEnd)
            );
        tpc = SubDetId::GetTPC(tpc_track->Detector);
       
        if(dist_tmp < dist){
            dist = dist_tmp;
            tpc_closest = tpc;
        }
        // TPC number is not zero ordered
        if(tpc_track->NNodes > nNodes[tpc-1]){
            nNodes[tpc-1] = tpc_track->NNodes;
            subtrack[tpc-1] = tpc_track;
        }
    }

    if(tpc_closest != (int)SubDetId::kInvalid) return subtrack[tpc_closest-1];

    return NULL;
}

//**************************************************
AnaParticleB* anaUtils::GetSegmentWithMostNodesInDet(const AnaTrackB& track, SubDetId::SubDetEnum det){
//**************************************************

    if (det == SubDetId::kInvalid) {
        return NULL;
    }
    if(!SubDetId::GetDetectorUsed(track.Detector, det)){
        return NULL;
    }

    int nNodes = 0;
    AnaParticleB* subtrack = NULL;

    switch(det){
        case SubDetId::kTPC :
            for(int i = 0; i < track.nTPCSegments; ++i){
                AnaTPCParticleB* tpc_track = track.TPCSegments[i];
                if(tpc_track->NNodes > nNodes){
                    nNodes = tpc_track->NNodes;
                    subtrack = tpc_track;
                }
            }
            return subtrack;
            break;
        case SubDetId::kFGD :
            for(int i = 0; i < track.nFGDSegments; ++i){
                AnaFGDParticleB* fgd_track = track.FGDSegments[i];
                if(fgd_track->NNodes > nNodes){
                    nNodes = fgd_track->NNodes;
                    subtrack = fgd_track;
                }
            }
            return subtrack;
            break;
        case SubDetId::kECAL :
            for(int i = 0; i < track.nECALSegments; ++i){
                AnaECALParticleB* tpc_track = track.ECALSegments[i];
                if(tpc_track->NNodes > nNodes){
                    nNodes = tpc_track->NNodes;
                    subtrack = tpc_track;
                }
            }
            return subtrack;
            break;
        case SubDetId::kSMRD :
            for(int i = 0; i < track.nSMRDSegments; ++i){
                AnaSMRDParticleB* smrd_track = track.SMRDSegments[i];
                if(smrd_track->NNodes > nNodes){
                    nNodes = smrd_track->NNodes;
                    subtrack = smrd_track;
                }
            }
            return subtrack;
            break;
        case SubDetId::kP0D :
            for(int i = 0; i < track.nP0DSegments; ++i){
                AnaP0DParticleB* p0d_track = track.P0DSegments[i];
                if(p0d_track->NNodes > nNodes){
                    nNodes = p0d_track->NNodes;
                    subtrack = p0d_track;
                }
            }
            return subtrack;
            break;
        default:
            if(SubDetId::IsTPCDetector(det)){
                for(int i = 0; i < track.nTPCSegments; ++i){
                    AnaTPCParticleB* tpc_track = track.TPCSegments[i];
                    if(SubDetId::GetDetectorUsed(tpc_track->Detector, det)){
                        if(tpc_track->NNodes > nNodes){
                            nNodes = tpc_track->NNodes;
                            subtrack = tpc_track;
                        }
                    }
                }
                return subtrack;
            }
            else if(SubDetId::IsFGDDetector(det)){
                for(int i = 0; i < track.nFGDSegments; ++i){
                    AnaFGDParticleB* fgd_track = track.FGDSegments[i];
                    if(SubDetId::GetDetectorUsed(fgd_track->Detector, det)){
                        if(fgd_track->NNodes > nNodes){
                            nNodes = fgd_track->NNodes;
                            subtrack = fgd_track;
                        }
                    }
                }
                return subtrack;
            }
            
            else if(SubDetId::IsP0DDetector(det)){
                for(int i = 0; i < track.nP0DSegments; ++i){
                    AnaP0DParticleB* p0d_track = track.P0DSegments[i];
                    if(SubDetId::GetDetectorUsed(p0d_track->Detector, det)){
                        if(p0d_track->NNodes > nNodes){
                            nNodes = p0d_track->NNodes;
                            subtrack = p0d_track;
                        }
                    }
                }
                return subtrack;
            }
            else if(SubDetId::IsECALDetector(det)){
                for(int i = 0; i < track.nECALSegments; ++i){
                    AnaECALParticleB* ecal_track = track.ECALSegments[i];
                    if(SubDetId::GetDetectorUsed(ecal_track->Detector, det)){
                        if(ecal_track->NNodes > nNodes){
                            nNodes = ecal_track->NNodes;
                            subtrack = ecal_track;
                        }
                    }
                }
                return subtrack;
            }
            else if(SubDetId::IsSMRDDetector(det)){
                for(int i = 0; i < track.nSMRDSegments; ++i){
                    AnaSMRDParticleB* smrd_track = track.SMRDSegments[i];
                    if(SubDetId::GetDetectorUsed(smrd_track->Detector, det)){
                        if(smrd_track->NNodes > nNodes){
                            nNodes = smrd_track->NNodes;
                            subtrack = smrd_track;
                        }
                    }
                }
                return subtrack;
            }
            return NULL;
    }
}

//**************************************************
AnaParticleB* anaUtils::GetSegmentInDet(const AnaTrackB& track, SubDetId::SubDetEnum det){
//**************************************************

    if(SubDetId::IsTPCDetector(det)){
        for(int i = 0; i < track.nTPCSegments; ++i){
            AnaTPCParticleB* tpc_track = track.TPCSegments[i];
            if(SubDetId::GetDetectorUsed(tpc_track->Detector, det)){
                return tpc_track;
            }
        }
    }
   
    if(SubDetId::IsFGDDetector(det)){
        for(int i = 0; i < track.nFGDSegments; ++i){
            AnaFGDParticleB* fgd_track = track.FGDSegments[i];
            if(SubDetId::GetDetectorUsed(fgd_track->Detector, det)){
                return fgd_track;
            }
        }
    }
    
    if(SubDetId::IsP0DDetector(det)){
        for(int i = 0; i < track.nP0DSegments; ++i){
            AnaP0DParticleB* p0d_track = track.P0DSegments[i];
            if(SubDetId::GetDetectorUsed(p0d_track->Detector, det)){
                return p0d_track;
            }
        }
    }
    
    if(SubDetId::IsECALDetector(det)){
        for(int i = 0; i < track.nECALSegments; ++i){
            AnaECALParticleB* ecal_track = track.ECALSegments[i];
            if(SubDetId::GetDetectorUsed(ecal_track->Detector, det)){
                return ecal_track;
            }
        }
    }
    
    if(SubDetId::IsSMRDDetector(det)){
        for(int i = 0; i < track.nSMRDSegments; ++i){
            AnaSMRDParticleB* smrd_track = track.SMRDSegments[i];
            if(SubDetId::GetDetectorUsed(smrd_track->Detector, det)){
                return smrd_track;
            }
        }
    }
    return NULL;
}

//**************************************************
SubDetId::SubDetEnum anaUtils::GetDetector(const Float_t* pos){
//**************************************************  
    for(Int_t it = 0; it != static_cast<Int_t>(SubDetId::kInvalidSubdetector); it++) {
        SubDetId::SubDetEnum det = static_cast<SubDetId::SubDetEnum>(it);
        if (anaUtils::InDetVolume(det, pos))
            return det;
    }
    return SubDetId::kInvalid;
}

//**************************************************
bool anaUtils::InDetVolume(SubDetId::SubDetEnum det, const Float_t* pos){
//**************************************************

    Float_t null[3] = {0.,0.,0.};
 
    //account for a case when a "general" volume is provided
    switch(det){
        case SubDetId::kFGD:
            return (InFiducialVolume(SubDetId::kFGD1, pos, null, null) || InFiducialVolume(SubDetId::kFGD2, pos, null, null));
            break;
        case SubDetId::kFGD1:
            return anaUtils::InFiducialVolume(SubDetId::kFGD1, pos, null, null);
            break;
        case SubDetId::kFGD2:
            return anaUtils::InFiducialVolume(SubDetId::kFGD2, pos, null, null);
            break;
        case SubDetId::kTPC:
            return (
                InFiducialVolume(SubDetId::kTPC1, pos, null, null) ||
                InFiducialVolume(SubDetId::kTPC2, pos, null, null) ||
                InFiducialVolume(SubDetId::kTPC3, pos, null, null) 
                );
            break;
        case SubDetId::kTPC1:
            return anaUtils::InFiducialVolume(SubDetId::kTPC1, pos, null, null);
            break;
        case SubDetId::kTPC2:
            return anaUtils::InFiducialVolume(SubDetId::kTPC2, pos, null, null);
            break;
        case SubDetId::kTPC3:
            return anaUtils::InFiducialVolume(SubDetId::kTPC3, pos, null, null);
            break;
        case SubDetId::kP0D:
            return anaUtils::InFiducialVolume(SubDetId::kP0D, pos, null, null);
            break;
        case SubDetId::kECAL:
            return (
                    anaUtils::InFiducialVolume(SubDetId::kTopTECAL, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kBottomTECAL, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kLeftTECAL, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kRightTECAL, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kTopPECAL, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kBottomPECAL, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kLeftPECAL, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kRightPECAL, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kDSECAL, pos, null, null)
                   );
            break;
        case SubDetId::kTECAL:
            return (
                    anaUtils::InFiducialVolume(SubDetId::kTopTECAL, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kBottomTECAL, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kLeftTECAL, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kRightTECAL, pos, null, null)
                   );
            break;
        case SubDetId::kTopTECAL:
            return anaUtils::InFiducialVolume(SubDetId::kTopTECAL, pos, null, null);
            break;
        case SubDetId::kBottomTECAL:
            return anaUtils::InFiducialVolume(SubDetId::kBottomTECAL, pos, null, null);
            break;
        case SubDetId::kLeftTECAL:
            return anaUtils::InFiducialVolume(SubDetId::kLeftTECAL, pos, null, null);
            break;
        case SubDetId::kRightTECAL:
            return anaUtils::InFiducialVolume(SubDetId::kRightTECAL, pos, null, null);
            break;
        case SubDetId::kPECAL:
            return (
                    anaUtils::InFiducialVolume(SubDetId::kTopPECAL, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kBottomPECAL, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kLeftPECAL, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kRightPECAL, pos, null, null)
                   );
            break;
        case SubDetId::kTopPECAL:
            return anaUtils::InFiducialVolume(SubDetId::kTopPECAL, pos, null, null);
            break;
        case SubDetId::kBottomPECAL:
            return anaUtils::InFiducialVolume(SubDetId::kBottomPECAL, pos, null, null);
            break;
        case SubDetId::kLeftPECAL:
            return anaUtils::InFiducialVolume(SubDetId::kLeftPECAL, pos, null, null);
            break;
        case SubDetId::kRightPECAL:
            return anaUtils::InFiducialVolume(SubDetId::kRightPECAL, pos, null, null);
            break;
        case SubDetId::kSMRD:
            return (
                    anaUtils::InFiducialVolume(SubDetId::kTopSMRD, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kBottomSMRD, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kLeftSMRD, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kRightSMRD, pos, null, null)
                   );
            break;
        case SubDetId::kTopSMRD:
            return anaUtils::InFiducialVolume(SubDetId::kTopSMRD, pos, null, null);
            break;
        case SubDetId::kBottomSMRD:
            return anaUtils::InFiducialVolume(SubDetId::kBottomSMRD, pos, null, null);
            break;
        case SubDetId::kLeftSMRD:
            return anaUtils::InFiducialVolume(SubDetId::kLeftSMRD, pos, null, null);
            break;
        case SubDetId::kRightSMRD:
            return anaUtils::InFiducialVolume(SubDetId::kRightSMRD, pos, null, null);
            break;
        case SubDetId::kDSECAL:
            return anaUtils::InFiducialVolume(SubDetId::kDSECAL, pos, null, null);
            break;
        default:
            std::cout << "Warning: anaUtils::InDetVolume() No Volume set for " << det << std::endl;
            return false;
            break;
    }

}

//**************************************************
bool anaUtils::InFiducialVolume(SubDetId::SubDetEnum det, const Float_t* pos){
//**************************************************


    


    Float_t null[3] = {0.,0.,0.};
    switch(det){
        case SubDetId::kFGD:
            return (InFiducialVolume(SubDetId::kFGD1,pos) || InFiducialVolume(SubDetId::kFGD2,pos));
            break;
        case SubDetId::kFGD1:
            return anaUtils::InFiducialVolume(SubDetId::kFGD1, pos, FVDef::FVdefminFGD1, FVDef::FVdefmaxFGD1);
            break;
        case SubDetId::kFGD2:
            return anaUtils::InFiducialVolume(SubDetId::kFGD2, pos, FVDef::FVdefminFGD2, FVDef::FVdefmaxFGD2);
            break;
        case SubDetId::kTPC:
            return (
                InFiducialVolume(SubDetId::kTPC1, pos, null, null) ||
                InFiducialVolume(SubDetId::kTPC2, pos, null, null) ||
                InFiducialVolume(SubDetId::kTPC3, pos, null, null) 
                );
            break;
        case SubDetId::kTPC1:
            return anaUtils::InFiducialVolume(SubDetId::kTPC1, pos, null, null);
            break;
        case SubDetId::kTPC2:
            return anaUtils::InFiducialVolume(SubDetId::kTPC2, pos, null, null);
            break;
        case SubDetId::kTPC3:
            return anaUtils::InFiducialVolume(SubDetId::kTPC3, pos, null, null);
            break;
        case SubDetId::kP0D:
            return anaUtils::InFiducialVolume(SubDetId::kP0D, pos, FVDef::FVdefminP0D, FVDef::FVdefmaxP0D);
            break;
        case SubDetId::kECAL:
            return (
                    anaUtils::InFiducialVolume(SubDetId::kTopTECAL, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kBottomTECAL, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kLeftTECAL, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kRightTECAL, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kTopPECAL, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kBottomPECAL, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kLeftPECAL, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kRightPECAL, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kDSECAL, pos, null, null)
                   );
            break;
        case SubDetId::kTECAL:
            return (
                    anaUtils::InFiducialVolume(SubDetId::kTopTECAL, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kBottomTECAL, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kLeftTECAL, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kRightTECAL, pos, null, null)
                   );
            break;
        case SubDetId::kTopTECAL:
            return anaUtils::InFiducialVolume(SubDetId::kTopTECAL, pos, null, null);
            break;
        case SubDetId::kBottomTECAL:
            return anaUtils::InFiducialVolume(SubDetId::kBottomTECAL, pos, null, null);
            break;
        case SubDetId::kLeftTECAL:
            return anaUtils::InFiducialVolume(SubDetId::kLeftTECAL, pos, null, null);
            break;
        case SubDetId::kRightTECAL:
            return anaUtils::InFiducialVolume(SubDetId::kRightTECAL, pos, null, null);
            break;
        case SubDetId::kPECAL:
            return (
                    anaUtils::InFiducialVolume(SubDetId::kTopPECAL, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kBottomPECAL, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kLeftPECAL, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kRightPECAL, pos, null, null)
                   );
            break;
        case SubDetId::kTopPECAL:
            return anaUtils::InFiducialVolume(SubDetId::kTopPECAL, pos, null, null);
            break;
        case SubDetId::kBottomPECAL:
            return anaUtils::InFiducialVolume(SubDetId::kBottomPECAL, pos, null, null);
            break;
        case SubDetId::kLeftPECAL:
            return anaUtils::InFiducialVolume(SubDetId::kLeftPECAL, pos, null, null);
            break;
        case SubDetId::kRightPECAL:
            return anaUtils::InFiducialVolume(SubDetId::kRightPECAL, pos, null, null);
            break;
        case SubDetId::kSMRD:
            return (
                    anaUtils::InFiducialVolume(SubDetId::kTopSMRD, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kBottomSMRD, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kLeftSMRD, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kRightSMRD, pos, null, null)
                   );
            break;
        case SubDetId::kTopSMRD:
            return anaUtils::InFiducialVolume(SubDetId::kTopSMRD, pos, null, null);
            break;
        case SubDetId::kBottomSMRD:
            return anaUtils::InFiducialVolume(SubDetId::kBottomSMRD, pos, null, null);
            break;
        case SubDetId::kLeftSMRD:
            return anaUtils::InFiducialVolume(SubDetId::kLeftSMRD, pos, null, null);
            break;
        case SubDetId::kRightSMRD:
            return anaUtils::InFiducialVolume(SubDetId::kRightSMRD, pos, null, null);
            break;
        case SubDetId::kDSECAL:
            return anaUtils::InFiducialVolume(SubDetId::kDSECAL, pos, null, null);
            break;
        default:
            std::cout << "Warning: anaUtils::InFiducialVolume() No Fiducial Volume set for " << det << std::endl;
            return false;
            break;
    }
}

//**************************************************
bool anaUtils::InFiducialVolume(SubDetId::SubDetEnum det, const Float_t* pos, const Float_t* FVdefmin, const Float_t* FVdefmax){
//**************************************************

    switch(det){
        case SubDetId::kFGD1:
            if (pos[0] > DetDef::fgd1min[0]+FVdefmin[0] &&
                    pos[0] < DetDef::fgd1max[0]-FVdefmax[0] &&
                    pos[1] > DetDef::fgd1min[1]+FVdefmin[1] &&
                    pos[1] < DetDef::fgd1max[1]-FVdefmax[1] &&
                    pos[2] > DetDef::fgd1min[2]+FVdefmin[2] &&
                    pos[2] < DetDef::fgd1max[2]-FVdefmax[2])
                return true;
            break;
        case SubDetId::kFGD2:
            if (pos[0] > DetDef::fgd2min[0]+FVdefmin[0] &&
                    pos[0] < DetDef::fgd2max[0]-FVdefmax[0] &&
                    pos[1] > DetDef::fgd2min[1]+FVdefmin[1] &&
                    pos[1] < DetDef::fgd2max[1]-FVdefmax[1] &&
                    pos[2] > DetDef::fgd2min[2]+FVdefmin[2] &&
                    pos[2] < DetDef::fgd2max[2]-FVdefmax[2])
                return true;
            break;
        case SubDetId::kTPC1:
            if (pos[0] > DetDef::tpc1min[0]+FVdefmin[0] &&
                    pos[0] < DetDef::tpc1max[0]-FVdefmax[0] &&
                    pos[1] > DetDef::tpc1min[1]+FVdefmin[1] &&
                    pos[1] < DetDef::tpc1max[1]-FVdefmax[1] &&
                    pos[2] > DetDef::tpc1min[2]+FVdefmin[2] &&
                    pos[2] < DetDef::tpc1max[2]-FVdefmax[2])
                return true;
            break;
        case SubDetId::kTPC2:
            if (pos[0] > DetDef::tpc2min[0]+FVdefmin[0] &&
                    pos[0] < DetDef::tpc2max[0]-FVdefmax[0] &&
                    pos[1] > DetDef::tpc2min[1]+FVdefmin[1] &&
                    pos[1] < DetDef::tpc2max[1]-FVdefmax[1] &&
                    pos[2] > DetDef::tpc2min[2]+FVdefmin[2] &&
                    pos[2] < DetDef::tpc2max[2]-FVdefmax[2])
                return true;
            break;
        case SubDetId::kTPC3:
            if (pos[0] > DetDef::tpc3min[0]+FVdefmin[0] &&
                    pos[0] < DetDef::tpc3max[0]-FVdefmax[0] &&
                    pos[1] > DetDef::tpc3min[1]+FVdefmin[1] &&
                    pos[1] < DetDef::tpc3max[1]-FVdefmax[1] &&
                    pos[2] > DetDef::tpc3min[2]+FVdefmin[2] &&
                    pos[2] < DetDef::tpc3max[2]-FVdefmax[2])
                return true;
            break;
        case SubDetId::kP0D:
            if (pos[0] > DetDef::p0dmin[0]+FVdefmin[0] &&
                    pos[0] < DetDef::p0dmax[0]-FVdefmax[0] &&
                    pos[1] > DetDef::p0dmin[1]+FVdefmin[1] &&
                    pos[1] < DetDef::p0dmax[1]-FVdefmax[1] &&
                    pos[2] > DetDef::p0dmin[2]+FVdefmin[2] &&
                    pos[2] < DetDef::p0dmax[2]-FVdefmax[2])
                return true;
            break;
            //DsECal (Ecal)
        case SubDetId::kDSECAL:
            if (pos[0] > DetDef::dsecalmin[0]+FVdefmin[0] &&
                    pos[0] < DetDef::dsecalmax[0]-FVdefmax[0] &&
                    pos[1] > DetDef::dsecalmin[1]+FVdefmin[1] &&
                    pos[1] < DetDef::dsecalmax[1]-FVdefmax[1] &&
                    pos[2] > DetDef::dsecalmin[2]+FVdefmin[2] &&
                    pos[2] < DetDef::dsecalmax[2]-FVdefmax[2])
                return true;
            break;
            //BarrelECal (TEcal)
        case SubDetId::kLeftTECAL:
            if (pos[0] > DetDef::tecalLmin[0]+FVdefmin[0] &&
                    pos[0] < DetDef::tecalLmax[0]-FVdefmax[0] &&
                    pos[1] > DetDef::tecalLmin[1]+FVdefmin[1] &&
                    pos[1] < DetDef::tecalLmax[1]-FVdefmax[1] &&
                    pos[2] > DetDef::tecalLmin[2]+FVdefmin[2] &&
                    pos[2] < DetDef::tecalLmax[2]-FVdefmax[2])
                return true;
            break;
        case SubDetId::kRightTECAL:
            if (pos[0] > DetDef::tecalRmin[0]+FVdefmin[0] &&
                    pos[0] < DetDef::tecalRmax[0]-FVdefmax[0] &&
                    pos[1] > DetDef::tecalRmin[1]+FVdefmin[1] &&
                    pos[1] < DetDef::tecalRmax[1]-FVdefmax[1] &&
                    pos[2] > DetDef::tecalRmin[2]+FVdefmin[2] &&
                    pos[2] < DetDef::tecalRmax[2]-FVdefmax[2])
                return true;
            break;
        case SubDetId::kTopTECAL:
            if (pos[0] > DetDef::tecalTLmin[0]+FVdefmin[0] &&
                    pos[0] < DetDef::tecalTLmax[0]-FVdefmax[0] &&
                    pos[1] > DetDef::tecalTLmin[1]+FVdefmin[1] &&
                    pos[1] < DetDef::tecalTLmax[1]-FVdefmax[1] &&
                    pos[2] > DetDef::tecalTLmin[2]+FVdefmin[2] &&
                    pos[2] < DetDef::tecalTLmax[2]-FVdefmax[2])
                return true;
            //two parts symmetric w.r.t to Z axis
            if (pos[0] > DetDef::tecalTRmin[0]+FVdefmax[0] &&
                    pos[0] < DetDef::tecalTRmax[0]-FVdefmin[0] &&
                    pos[1] > DetDef::tecalTRmin[1]+FVdefmin[1] &&
                    pos[1] < DetDef::tecalTRmax[1]-FVdefmax[1] &&
                    pos[2] > DetDef::tecalTRmin[2]+FVdefmin[2] &&
                    pos[2] < DetDef::tecalTRmax[2]-FVdefmax[2])
                return true;
            break;
        case SubDetId::kBottomTECAL:
            if (pos[0] > DetDef::tecalBLmin[0]+FVdefmin[0] &&
                    pos[0] < DetDef::tecalBLmax[0]-FVdefmax[0] &&
                    pos[1] > DetDef::tecalBLmin[1]+FVdefmin[1] &&
                    pos[1] < DetDef::tecalBLmax[1]-FVdefmax[1] &&
                    pos[2] > DetDef::tecalBLmin[2]+FVdefmin[2] &&
                    pos[2] < DetDef::tecalBLmax[2]-FVdefmax[2])
                return true;
            //two parts symmetric w.r.t to Z axis
            if (pos[0] > DetDef::tecalBRmin[0]+FVdefmax[0] &&
                    pos[0] < DetDef::tecalBRmax[0]-FVdefmin[0] &&
                    pos[1] > DetDef::tecalBRmin[1]+FVdefmin[1] &&
                    pos[1] < DetDef::tecalBRmax[1]-FVdefmax[1] &&
                    pos[2] > DetDef::tecalBRmin[2]+FVdefmin[2] &&
                    pos[2] < DetDef::tecalBRmax[2]-FVdefmax[2])
                return true;
            break;
            //P0DECal (PEcal)
        case SubDetId::kLeftPECAL:
            if (pos[0] > DetDef::pecalLmin[0]+FVdefmin[0] &&
                    pos[0] < DetDef::pecalLmax[0]-FVdefmax[0] &&
                    pos[1] > DetDef::pecalLmin[1]+FVdefmin[1] &&
                    pos[1] < DetDef::pecalLmax[1]-FVdefmax[1] &&
                    pos[2] > DetDef::pecalLmin[2]+FVdefmin[2] &&
                    pos[2] < DetDef::pecalLmax[2]-FVdefmax[2])
                return true;
            break;
        case SubDetId::kRightPECAL:
            if (pos[0] > DetDef::pecalRmin[0]+FVdefmin[0] &&
                    pos[0] < DetDef::pecalRmax[0]-FVdefmax[0] &&
                    pos[1] > DetDef::pecalRmin[1]+FVdefmin[1] &&
                    pos[1] < DetDef::pecalRmax[1]-FVdefmax[1] &&
                    pos[2] > DetDef::pecalRmin[2]+FVdefmin[2] &&
                    pos[2] < DetDef::pecalRmax[2]-FVdefmax[2])
                return true;
            break;
        case SubDetId::kTopPECAL:
             if (pos[0] > DetDef::pecalTLmin[0]+FVdefmin[0] &&
                    pos[0] < DetDef::pecalTLmax[0]-FVdefmax[0] &&
                    pos[1] > DetDef::pecalTLmin[1]+FVdefmin[1] &&
                    pos[1] < DetDef::pecalTLmax[1]-FVdefmax[1] &&
                    pos[2] > DetDef::pecalTLmin[2]+FVdefmin[2] &&
                    pos[2] < DetDef::pecalTLmax[2]-FVdefmax[2])
                return true;
            //two parts symmetric w.r.t to Z axis
            if (pos[0] > DetDef::pecalTRmin[0]+FVdefmax[0] &&
                    pos[0] < DetDef::pecalTRmax[0]-FVdefmin[0] &&
                    pos[1] > DetDef::pecalTRmin[1]+FVdefmin[1] &&
                    pos[1] < DetDef::pecalTRmax[1]-FVdefmax[1] &&
                    pos[2] > DetDef::pecalTRmin[2]+FVdefmin[2] &&
                    pos[2] < DetDef::pecalTRmax[2]-FVdefmax[2])
                return true;
            break;    

        case SubDetId::kBottomPECAL:
           if (pos[0] > DetDef::pecalBLmin[0]+FVdefmin[0] &&
                    pos[0] < DetDef::pecalBLmax[0]-FVdefmax[0] &&
                    pos[1] > DetDef::pecalBLmin[1]+FVdefmin[1] &&
                    pos[1] < DetDef::pecalBLmax[1]-FVdefmax[1] &&
                    pos[2] > DetDef::pecalBLmin[2]+FVdefmin[2] &&
                    pos[2] < DetDef::pecalBLmax[2]-FVdefmax[2])
                return true;
            //two parts symmetric w.r.t to Z axis
            if (pos[0] > DetDef::pecalBRmin[0]+FVdefmax[0] &&
                    pos[0] < DetDef::pecalBRmax[0]-FVdefmin[0] &&
                    pos[1] > DetDef::pecalBRmin[1]+FVdefmin[1] &&
                    pos[1] < DetDef::pecalBRmax[1]-FVdefmax[1] &&
                    pos[2] > DetDef::pecalBRmin[2]+FVdefmin[2] &&
                    pos[2] < DetDef::pecalBRmax[2]-FVdefmax[2])
                return true;
            break;        
        //SMRD
        case SubDetId::kLeftSMRD:
            if (pos[1] > DetDef::smrd15Lmin[1]+FVdefmin[1] &&
                    pos[1] < DetDef::smrd15Lmax[1]-FVdefmax[1] &&
                    pos[2] > DetDef::smrd15Lmin[2]+FVdefmin[2] &&
                    pos[2] < DetDef::smrd78Lmax[2]-FVdefmax[2]){

                if (pos[2] > DetDef::smrd15Lmin[2] &&
                        pos[2] < DetDef::smrd6Lmin[2] &&
                        pos[0] > DetDef::smrd15Lmin[0]+FVdefmin[0] &&
                        pos[0] < DetDef::smrd15Lmax[0]-FVdefmax[0])
                    return true;

                if (pos[2] > DetDef::smrd6Lmin[2] &&
                        pos[2] < DetDef::smrd78Lmin[2] &&
                        pos[0] > DetDef::smrd6Lmin[0]+FVdefmin[0] &&
                        pos[0] < DetDef::smrd6Lmax[0]-FVdefmax[0])
                    return true;

                if (pos[2] > DetDef::smrd78Lmin[2] &&
                        pos[2] < DetDef::smrd78Lmax[2] &&
                        pos[0] > DetDef::smrd78Lmin[0]+FVdefmin[0] &&
                        pos[0] < DetDef::smrd78Lmax[0]-FVdefmax[0])
                    return true;
            }
            break;

        case SubDetId::kRightSMRD:
            if (pos[1] > DetDef::smrd15Rmin[1]+FVdefmin[1] &&
                    pos[1] < DetDef::smrd15Rmax[1]-FVdefmax[1] &&
                    pos[2] > DetDef::smrd15Rmin[2]+FVdefmin[2] &&
                    pos[2] < DetDef::smrd78Rmax[2]-FVdefmax[2]){

                if (pos[2] > DetDef::smrd15Rmin[2] &&
                        pos[2] < DetDef::smrd6Rmin[2] &&
                        pos[0] > DetDef::smrd15Rmin[0]+FVdefmin[0] &&
                        pos[0] < DetDef::smrd15Rmax[0]-FVdefmax[0])
                    return true;

                if (pos[2] > DetDef::smrd6Rmin[2] &&
                        pos[2] < DetDef::smrd78Rmin[2] &&
                        pos[0] > DetDef::smrd6Rmin[0]+FVdefmin[0] &&
                        pos[0] < DetDef::smrd6Rmax[0]-FVdefmax[0])
                    return true;

                if (pos[2] > DetDef::smrd78Rmin[2] &&
                        pos[2] < DetDef::smrd78Rmax[2] &&
                        pos[0] > DetDef::smrd78Rmin[0]+FVdefmin[0] &&
                        pos[0] < DetDef::smrd78Rmax[0]-FVdefmax[0])
                    return true;
            }
            break;
        case SubDetId::kTopSMRD:
            if (pos[0] > DetDef::smrdTLmin[0]+FVdefmin[0] &&
                    pos[0] < DetDef::smrdTLmax[0]-FVdefmax[0] &&
                    pos[1] > DetDef::smrdTLmin[1]+FVdefmin[1] &&
                    pos[1] < DetDef::smrdTLmax[1]-FVdefmax[1] &&
                    pos[2] > DetDef::smrdTLmin[2]+FVdefmin[2] &&
                    pos[2] < DetDef::smrdTLmax[2]-FVdefmax[2])
                return true;
            //two parts symmetric w.r.t to Z axis
            if (pos[0] > DetDef::smrdTRmin[0]+FVdefmax[0] &&
                    pos[0] < DetDef::smrdTRmax[0]-FVdefmin[0] &&
                    pos[1] > DetDef::smrdTRmin[1]+FVdefmin[1] &&
                    pos[1] < DetDef::smrdTRmax[1]-FVdefmax[1] &&
                    pos[2] > DetDef::smrdTRmin[2]+FVdefmin[2] &&
                    pos[2] < DetDef::smrdTRmax[2]-FVdefmax[2])
                return true;
            break;
        case SubDetId::kBottomSMRD:
            if (pos[0] > DetDef::smrdBLmin[0]+FVdefmin[0] &&
                    pos[0] < DetDef::smrdBLmax[0]-FVdefmax[0] &&
                    pos[1] > DetDef::smrdBLmin[1]+FVdefmin[1] &&
                    pos[1] < DetDef::smrdBLmax[1]-FVdefmax[1] &&
                    pos[2] > DetDef::smrdBLmin[2]+FVdefmin[2] &&
                    pos[2] < DetDef::smrdBLmax[2]-FVdefmax[2])
                return true;
            //two parts symmetric w.r.t to Z axis
            if (pos[0] > DetDef::smrdBRmin[0]+FVdefmax[0] &&
                    pos[0] < DetDef::smrdBRmax[0]-FVdefmin[0] &&
                    pos[1] > DetDef::smrdBRmin[1]+FVdefmin[1] &&
                    pos[1] < DetDef::smrdBRmax[1]-FVdefmax[1] &&
                    pos[2] > DetDef::smrdBRmin[2]+FVdefmin[2] &&
                    pos[2] < DetDef::smrdBRmax[2]-FVdefmax[2])
                return true;
            break;
        default:
            std::cout << "Error:  anaUtils::InFiducialVolume() given an unknown subdetector enumeration: " << det << std::endl;

    }

    return false;

}


//**************************************************
int anaUtils::GetAllChargedTrajInTPCInBunch(const AnaEventB& event, AnaTrueParticleB* chargedtrajInBunch[]) {
//**************************************************
    int count = 0;
    for(Int_t i=0;i< event.nTrueParticles;i++){
      if(!event.TrueParticles[i]->TrueVertex) continue;
      if(event.TrueParticles[i]->TrueVertex->Bunch!=event.Bunch) continue;
      if(event.TrueParticles[i]->Charge==0)continue;
      Float_t dist=-9999999;
        for(Int_t idet=0;idet<event.TrueParticles[i]->nDetCrossings;idet++){
            //i.e crossing the active part of the tpc
            if(SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet]->Detector, SubDetId::kTPC) && event.TrueParticles[i]->DetCrossings[idet]->InActive) {
                Float_t sep = GetSeparationSquared(event.TrueParticles[i]->DetCrossings[idet]->EntrancePosition, event.TrueParticles[i]->DetCrossings[idet]->ExitPosition);
                if(sep>dist) dist=sep;
            }
        }
        // 30* 30 originally
        if((dist)>900 && event.TrueParticles[i]->Momentum>5){//bigger than 3 TPC hits (30*30 is faster that sqrt(dist)), and momentum > 5 MeV 
	  chargedtrajInBunch[count] = event.TrueParticles[i];
	  ++count;
        }
    }
    return count;
}

//**************************************************
int anaUtils::GetAllChargedTrajInFGDInBunch(const AnaEventB& event, AnaTrueParticleB* chargedtrajInBunch[],SubDetId::SubDetEnum det){
//**************************************************
    /* 
     * we need here to select in-FGD tracks that potentially should have been reconstruced
     * by FGD iso recon (the function name is confusing);
     * this involves putting some min requirements for the true tracks:
     * since FGD iso recon requires a track to extend for at least 4 Z layers (i.e. having hits in five consequitive layers)
     * in order to be reconstruced this requirement should be applied for the true tracks as well.
     * In principle one can use the geometry info to retrieve layers that true entrance and exit point correspond to
     * but it can be time taking,  so we use an approximation: a true trajectory should have a length in Z at least of the one of 4 FGD layers:
     * so 4 cm

     */

  int count = 0;
  for (Int_t i = 0; i < event.nTrueParticles; i++) {
    if(!event.TrueParticles[i]->TrueVertex) continue;
    if(event.TrueParticles[i]->TrueVertex->Bunch!=event.Bunch) continue;
    if(event.TrueParticles[i]->Charge==0)continue;
    Float_t dist = -9999999;
    for (Int_t idet = 0; idet < event.TrueParticles[i]->nDetCrossings; idet++) {
      // i.e crossing the active part of the FGD
      if (SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet]->Detector, SubDetId::kFGD)){
        if (SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet]->Detector, det) && event.TrueParticles[i]->DetCrossings[idet]->InActive) {
          //the separation should be done using the z position, since the fgd is separated by layer in z,
          //making the z position the reconstructed quantity to be cut on
          Float_t sep = fabs(event.TrueParticles[i]->DetCrossings[idet]->EntrancePosition[2] - event.TrueParticles[i]->DetCrossings[idet]->ExitPosition[2]);
          if(sep>dist) dist=sep;
        }
      }
    }
    // apply the cut (this cut is only valid for FGD!)
    if (dist > 40){
      chargedtrajInBunch[count] = event.TrueParticles[i];
      ++count;
    }
  }

  return count;
}

//**************************************************
int anaUtils::GetAllChargedTrajInFGDAndNoTPCInBunch(const AnaEventB& event, AnaTrueParticleB* chargedtrajInBunch[],SubDetId::SubDetEnum det){
//**************************************************  
    AnaTrueParticleB* trajInBunchInFgdx[NMAXTRUEPARTICLES];
    Int_t ntrajInBunchInFgdx = anaUtils::GetAllChargedTrajInFGDInBunch(event, trajInBunchInFgdx,det);

    Int_t count = 0;
    for (Int_t i = 0; i < ntrajInBunchInFgdx; i++) {
      Float_t dist=-999999.;
      for(Int_t idet=0;idet<trajInBunchInFgdx[i]->nDetCrossings;idet++){
        //i.e crossing the active part of the tpc
        if(SubDetId::GetDetectorUsed(trajInBunchInFgdx[i]->DetCrossings[idet]->Detector, SubDetId::kTPC) && trajInBunchInFgdx[i]->DetCrossings[idet]->InActive) {
          Float_t sep = GetSeparationSquared(trajInBunchInFgdx[i]->DetCrossings[idet]->EntrancePosition, trajInBunchInFgdx[i]->DetCrossings[idet]->ExitPosition);
          
          if(sep>dist) dist=sep;
        }
      }
      
      bool cross_tpc = false;
      // 250*250 originally
      if(dist>62500)//bigger than the ~1/4 of the width of the TPC
        cross_tpc = true;
      
      if (!cross_tpc){
        chargedtrajInBunch[count] = trajInBunchInFgdx[i];
        ++count;
      }
    }
    
    return count;
}

//**************************************************
int anaUtils::GetAllChargedTrajInTPCFGDInBunch(const AnaEventB& event, AnaTrueParticleB* chargedtrajInBunch[]){
//**************************************************

    int count = 0;

    for(Int_t i=0;i<event.nTrueParticles;i++){
      if(!event.TrueParticles[i]->TrueVertex) continue;
      if(event.TrueParticles[i]->TrueVertex->Bunch!=event.Bunch) continue;
      if(event.TrueParticles[i]->Charge==0)continue;

        Float_t dist=-9999999;
        for(Int_t idet=0;idet<event.TrueParticles[i]->nDetCrossings;idet++){
            //i.e crossing the active part of one of the FGDs
          if(SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet]->Detector, SubDetId::kFGD)){
            for(Int_t idet1=0;idet1<event.TrueParticles[i]->nDetCrossings;idet1++){
              //look for TPC1-FGD1, FGD1-TPC2, TPC2-FGD2, FGD2-TPC3 trajectories
              if((SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet]->Detector, SubDetId::kFGD1) && 
                  (SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet1]->Detector, SubDetId::kTPC1) || SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet1]->Detector, SubDetId::kTPC2))) || 
                 (SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet]->Detector, SubDetId::kFGD2) && 
                  (SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet1]->Detector, SubDetId::kTPC2) || SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet1]->Detector, SubDetId::kTPC3)))) 
                {
                  Float_t sep = GetSeparationSquared(event.TrueParticles[i]->DetCrossings[idet1]->EntrancePosition, event.TrueParticles[i]->DetCrossings[idet1]->ExitPosition);
                  if(sep>dist) dist=sep;
                }
            }
          }
        }
        
        // 10*10 originally, now 100
        if(dist>100){
            chargedtrajInBunch[count] = event.TrueParticles[i];
            ++count;
        }
    }

    return count;

}

//**************************************************
int anaUtils::GetAllBigEnoughChargedTrajInTPCInBunch(const AnaEventB& event, AnaTrueParticleB* chargedtrajInBunch[]){
//**************************************************

    int count = 0;

    for(Int_t i=0;i< event.nTrueParticles;i++){
      if(!event.TrueParticles[i]->TrueVertex) continue;
      if(event.TrueParticles[i]->TrueVertex->Bunch!=event.Bunch) continue;
      if(event.TrueParticles[i]->Charge==0)continue;

      Float_t dist=0;
        for(Int_t idet=0;idet<event.TrueParticles[i]->nDetCrossings;idet++){
            //i.e crossing the active part of the tpc
            if(SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet]->Detector, SubDetId::kTPC) && event.TrueParticles[i]->DetCrossings[idet]->InActive) {
                Float_t sep = GetSeparationSquared(event.TrueParticles[i]->DetCrossings[idet]->EntrancePosition, event.TrueParticles[i]->DetCrossings[idet]->ExitPosition);
                if(sep>dist) dist=sep;
            }
        }
        //250*250 originally 
        if(dist>62500){//bigger than the ~1/4 of the width of the TPC
            chargedtrajInBunch[count] = event.TrueParticles[i];
            ++count;
        }

    }
    return count;
}

int anaUtils::GetAllChargedTrajInP0DInBunch(const AnaEventB& event, AnaTrueParticleB* chargedTrajInBunch[])
{
  int count = 0;
  for (Int_t i = 0; i<event.nTrueParticles; i++)
  {
    if (!event.TrueParticles[i]->TrueVertex) continue;
    if (event.TrueParticles[i]->TrueVertex->Bunch!=event.Bunch) continue;
    if (event.TrueParticles[i]->Charge==0) continue;
    Float_t dist = -9999999;
    for (Int_t idet = 0; idet<event.TrueParticles[i]->nDetCrossings;idet++)
    {
      if (SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet]->Detector,SubDetId::kP0D) && event.TrueParticles[i]->DetCrossings[idet]->InActive){
        Float_t sep = anaUtils::GetSeparationSquared(event.TrueParticles[i]->DetCrossings[idet]->EntrancePosition,event.TrueParticles[i]->DetCrossings[idet]->ExitPosition);
        if (sep>dist) dist = sep;
      }
    }
    if ( (dist)>900 && event.TrueParticles[i]->Momentum>5){
      chargedTrajInBunch[count] = event.TrueParticles[i];
      count++;
    }
    if (count >= (int) NMAXTRUEPARTICLES) break;
  }

  return count;

}

int anaUtils::GetAllChargedTrajInP0DAndTPCInBunch(const AnaEventB& event, AnaTrueParticleB* chargedTrajInBunch[])
{
  int count = 0;
  for (Int_t i = 0; i<event.nTrueParticles; i++)
  {
    if (!event.TrueParticles[i]->TrueVertex) continue;
    if (event.TrueParticles[i]->TrueVertex->Bunch!=event.Bunch) continue;
    if (event.TrueParticles[i]->Charge==0) continue;
    Float_t dist = -9999999;
    for (Int_t idet = 0; idet<event.TrueParticles[i]->nDetCrossings;idet++)
    {
      if (SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet]->Detector,SubDetId::kP0D) && event.TrueParticles[i]->DetCrossings[idet]->InActive){
        for (Int_t idet1=0; idet1<event.TrueParticles[i]->nDetCrossings; idet1++)
        {
          if (SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet1]->Detector,SubDetId::kTPC1) && event.TrueParticles[i]->DetCrossings[idet1]->InActive){
            Float_t sep = anaUtils::GetSeparationSquared(event.TrueParticles[i]->DetCrossings[idet]->EntrancePosition,event.TrueParticles[i]->DetCrossings[idet]->ExitPosition);
            if (sep>dist) dist = sep;
          }//in TPC1
        }//Loop over detector crossings
      }//in P0D
    }//Loop over detector crossings
    if ( (dist)>100 && event.TrueParticles[i]->Momentum>5){
      chargedTrajInBunch[count] = event.TrueParticles[i];
      count++;
    }
    if (count >= (int) NMAXTRUEPARTICLES) break;
  }

  return count;


}

int anaUtils::GetAllChargedTrajInP0DAndNoTPCInBunch(const AnaEventB& event, AnaTrueParticleB* chargedTrajInBunch[])
{
  AnaTrueParticleB* trajInP0D[NMAXTRUEPARTICLES];
  Int_t ntrajInP0D = GetAllChargedTrajInP0DInBunch(event,trajInP0D);
  Int_t count = 0;
  for (Int_t i = 0; i < ntrajInP0D; i++){
    Float_t dist=-999999.;
    for (Int_t idet=0; idet<trajInP0D[i]->nDetCrossings; idet++)
    {
      if (SubDetId::GetDetectorUsed(trajInP0D[i]->DetCrossings[idet]->Detector,SubDetId::kTPC1) && trajInP0D[i]->DetCrossings[idet]->InActive){
        Float_t sep = anaUtils::GetSeparationSquared(trajInP0D[i]->DetCrossings[idet]->EntrancePosition,trajInP0D[i]->DetCrossings[idet]->ExitPosition);
        if (sep>dist) dist=sep;


      }
    }
    bool cross_tpc = false;
    if (dist>900)//originally 62500=250**2: 1/4 width of TPC
      cross_tpc = true;
    if (!cross_tpc){
      chargedTrajInBunch[count] = trajInP0D[i];
      count++;
      if (count>=(int)NMAXTRUEPARTICLES) break;
    }

  }
  return count;


}


//**************************************************
int anaUtils::GetAllTracksUsingFGDAndNoTPC(const AnaEventB& event, AnaTrackB* selTracks[],SubDetId::SubDetEnum fgddet) {
//**************************************************

    int count = 0;
    for (int it = 0; it < event.nParticles; it++) {
        AnaTrackB* track = static_cast<AnaTrackB*>(event.Particles[it]);
        if (!SubDetId::GetDetectorUsed(track->Detector, SubDetId::kTPC) && SubDetId::GetDetectorUsed(track->Detector, fgddet)) {
            selTracks[count] = track;
            ++count;
        }
    }

    // Sort by decreasing number of hits
    std::sort(&selTracks[0], &selTracks[count], AnaParticleB::CompareNHits);

    return count;
}


//**************************************************
int anaUtils::GetAllTracksUsingFGDorTPC(const AnaEventB& event, AnaTrackB* selTracks[]) {
//**************************************************

    int count = 0;
    for (int it = 0; it < event.nParticles; it++) {
        AnaTrackB* track = static_cast<AnaTrackB*>(event.Particles[it]);
        if (SubDetId::GetDetectorUsed(track->Detector, SubDetId::kTPC) || SubDetId::GetDetectorUsed(track->Detector, SubDetId::kFGD)) {
            selTracks[count] = track;
            ++count;
        }
    }

    // Sort by decreasing number of hits
    std::sort(&selTracks[0], &selTracks[count], AnaParticleB::CompareNHits);

    return count;
}

//**************************************************
bool anaUtils::HasTrackUsingTPC(const AnaEventB& event) {
//**************************************************
  return anaUtils::HasTrackUsingDet(event, SubDetId::kTPC);
}

//**************************************************
int anaUtils::GetAllTracksUsingP0D(const AnaEventB& event, AnaTrackB* selTracks[]) {
//**************************************************
    return GetAllTracksUsingDet(event, SubDetId::kP0D, selTracks);
}

//**************************************************
int anaUtils::GetAllTracksUsingFGD(const AnaEventB& event, AnaTrackB* selTracks[]) {
//**************************************************
    return GetAllTracksUsingDet(event, SubDetId::kFGD, selTracks);
}

//**************************************************
int anaUtils::GetAllTracksUsingTPC(const AnaEventB& event, AnaTrackB* selTracks[]) {
//**************************************************
    return GetAllTracksUsingDet(event, SubDetId::kTPC, selTracks);
}

//**************************************************
int anaUtils::GetAllTracksUsingECAL(const AnaEventB& event, AnaTrackB* selTracks[]) {
//**************************************************
    return GetAllTracksUsingDet(event, SubDetId::kECAL, selTracks);
}

//**************************************************
int anaUtils::GetAllIsoTracksInECAL(const AnaEventB& event, AnaTrackB* selTracks[]) {
//**************************************************
   SubDetId::SubDetEnum det[5];
   // Do it explicitely
   det[0] = SubDetId::kDSECAL; 
   det[1] = SubDetId::kTopTECAL; 
   det[2] = SubDetId::kBottomTECAL; 
   det[3] = SubDetId::kLeftTECAL;
   det[4] = SubDetId::kRightTECAL;
   
   return anaUtils::GetAllTracksUsingOnlyDetFromList(event, selTracks, det, 5);
    
}


//**************************************************
int anaUtils::GetAllTracksUsingSMRD(const AnaEventB& event, AnaTrackB* selTracks[]) {
//**************************************************
    return GetAllTracksUsingDet(event, SubDetId::kSMRD, selTracks);
}

//**************************************************
int anaUtils::GetNTracksUsingTPCAndDet(const AnaEventB& event, SubDetId::SubDetEnum det) {
//**************************************************

    int count = 0;

    SubDetId::SubDetEnum dets[2];
    dets[0] = SubDetId::kTPC;
    dets[1] = det;

    // Loop over all tracks
    for (int it = 0; it < event.nParticles; it++) {
        AnaTrackB* track = static_cast<AnaTrackB*>(event.Particles[it]);
        if (anaUtils::TrackUsesDets(*track, dets, 2)){
            count ++;
        }
    }

    return count;
}

//********************************************************************
bool anaUtils::UsesTrackerDsEcal(AnaTrackB* track) {
//********************************************************************
    return (SubDetId::GetDetectorUsed(track->Detector, SubDetId::kTECAL) || SubDetId::GetDetectorUsed(track->Detector, SubDetId::kDSECAL));
}

//********************************************************************
int anaUtils::GetTrackerDsEcals(AnaTrackB* track, AnaECALParticleB* ecals[]) {
//********************************************************************

    int count = 0;
    for (Int_t i = 0; i < track->nECALSegments; i++) {
        AnaECALParticleB* ecal = track->ECALSegments[i];
        if (SubDetId::GetDetectorUsed(ecal->Detector, SubDetId::kTECAL) || SubDetId::GetDetectorUsed(ecal->Detector, SubDetId::kDSECAL)) {
            ecals[count] = ecal;
            count ++;
        }
    }

    return count;
}

//********************************************************************
AnaECALParticleB* anaUtils::GetMostUpstreamECalSegment(AnaTrackB* track) {
//********************************************************************
    AnaECALParticleB* upstream = NULL;

    Float_t pos_z = 1e4;

    if (track) {
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

//********************************************************************
//bool anaUtils::IsEcalContained(AnaEcalTrackEcalPID* ecal) {
//********************************************************************
/*
   TVector3 pos = ecal->ShowerPosition;

   if (ecal->Detector == subDetId::kDSECAL) {
   return (fabs(pos.X()) < 900 && fabs(pos.Y()) < 900);
   } else if (ecal->Detector == subDetId::kLeftTECAL || ecal->Detector == subDetId::kRightTECAL) {
   return (fabs(pos.Z()) < 2900 && fabs(pos.Y()) < 900);
   } else {
   return (fabs(pos.Z()) < 2900 && fabs(pos.X()) < 900);
   }
   }
*/


//********************************************************************
Int_t anaUtils::GetFgdModuleType(bool IsMC, const Float_t* pos, SubDetId::SubDetEnum det, bool includeGlueSkin){
//********************************************************************

  if (det == SubDetId::kFGD1) std::cout << "FGD1 not supported" << std::endl;
  if ( ! anaUtils::InFiducialVolume(det,pos)) return 7; // here CATOUTFV is not known
  else return GetFgdModuleTypeNoFV(IsMC, pos, includeGlueSkin);
}


//********************************************************************
Int_t anaUtils::GetFgdModuleTypeNoFV(bool IsMC, const Float_t* pos, bool includeGlueSkin){
//********************************************************************

  Int_t material = GetFgdMaterialNoFV(IsMC, pos);

  if (includeGlueSkin) {
    if (material >= 10 && material <= 14) return 0; // 1st half XY module
    if (material >= 15 && material <= 19) return 1; // 2nd half XY module
    if (material >= 20 && material <= 26) return 2; // water module
  }
  else {
    // OLD WAY, until nd280Highland2 v1r3
    if (material == 10) return 0; // X layer
    if (material == 15) return 1; // Y layer
    if (material == 20) return 2; // water/PC(polycarbonate) panel
  }

  return 3; // gaps, should be empty if includeGlueSkin=true (default)
}


//********************************************************************
Int_t anaUtils::GetFgdMaterial(bool IsMC, const Float_t* pos, SubDetId::SubDetEnum det){
//********************************************************************

  if (det == SubDetId::kFGD1) std::cout << "FGD1 not supported" << std::endl;
  if ( ! anaUtils::InFiducialVolume(det,pos)) return 7; // here CATOUTFV is not known
  else return GetFgdMaterialNoFV(IsMC, pos);
}


//********************************************************************
Int_t anaUtils::GetFgdMaterialNoFV(bool IsMC, const Float_t* pos){
//********************************************************************

    // to do: do it also for FGD1
    Float_t barCenter[14];
    Float_t waterCenter[6];
    if (IsMC) {
      for (Int_t i=0; i<14; i++) barCenter[i] = DetDef::fgd2BarCenterMC[i];
      for (Int_t i=0; i<6; i++) waterCenter[i] = DetDef::fgd2WaterCenterMC[i];
    } else {
      for (Int_t i=0; i<14; i++) barCenter[i] = DetDef::fgd2BarCenterDATA[i];
      for (Int_t i=0; i<6; i++) waterCenter[i] = DetDef::fgd2WaterCenterDATA[i];
    }

    // this has to be done like this because Float_t appoximation leaves gaps
    Float_t midpos_preX[7];
    Float_t midpos_postY[7];
    Float_t midpos_XY[7];
    Int_t ixymod=0;

    // look in scintillator layers
    Float_t initZ, finalZ;
    for (Int_t ibar=0; ibar<14; ibar++) {

      initZ = barCenter[ibar] - (DetDef::fgdXYBarWidth/2);
      finalZ = barCenter[ibar] + (DetDef::fgdXYBarWidth/2);

      if (ibar%2 == 0) {
        if (pos[2] >= initZ && pos[2] <= finalZ) return 10; // X layer
        initZ = initZ - DetDef::fgdXYGlueWidth;
        if (pos[2] >= initZ && pos[2] <= finalZ) return 11; // glue before X layer
        initZ = initZ - DetDef::fgdXYSkinWidth;
        if (pos[2] >= initZ && pos[2] <= finalZ) return 12; // G10 skin before X layer
        initZ = initZ - (DetDef::fgdWaterAirWidth/2);
        if (pos[2] >= initZ && pos[2] <= finalZ) return 13; // half of air before X layer
        finalZ = finalZ + (DetDef::fgdXYMiddleGlueWidth/2);
        if (pos[2] >= initZ && pos[2] <= finalZ) return 14; // half of glue between X and Y

        midpos_preX[ixymod] = initZ;
        midpos_XY[ixymod] = finalZ;

      } else if (ibar%2 == 1) {
        if (pos[2] >= initZ && pos[2] <= finalZ) return 15; // Y layer
        finalZ = finalZ + DetDef::fgdXYGlueWidth;
        if (pos[2] >= initZ && pos[2] <= finalZ) return 16; // glue after Y layer
        finalZ = finalZ + DetDef::fgdXYSkinWidth;
        if (pos[2] >= initZ && pos[2] <= finalZ) return 17; // G10 skin after Y layer
        finalZ = finalZ + (DetDef::fgdXYAirWidth/2);
        if (pos[2] >= initZ && pos[2] <= finalZ) return 18; // half of air after Y layer
//        initZ = initZ - (DetDef::fgdXYMiddleGlueWidth/2);
//        if (pos[2] >= initZ && pos[2] <= finalZ) return 19; // half of glue between X and Y

        if (pos[2] >= midpos_XY[ixymod] && pos[2] <= finalZ) return 19; // half of glue between X and Y

        midpos_postY[ixymod] = finalZ;
        ixymod++;
      }
    }

    // look in water modules
    for (Int_t ibar=0; ibar<6; ibar++) {
      initZ = waterCenter[ibar] - (DetDef::fgdWaterPCWidth/2);
      finalZ = waterCenter[ibar] + (DetDef::fgdWaterPCWidth/2);
      if (pos[2] >= initZ && pos[2] <= finalZ) return 20; // water/PC(polycarbonate) panel
      initZ = initZ - DetDef::fgdWaterGlueWidth;
      if (pos[2] >= initZ && pos[2] <= finalZ) return 21; // glue before water/PC
      initZ = initZ - DetDef::fgdWaterSkinWidth;
      if (pos[2] >= initZ && pos[2] <= finalZ) return 22; // polypropylene skin before water/PC
//      initZ = initZ - (DetDef::fgdXYAirWidth/2);
//      if (pos[2] >= initZ && pos[2] <= finalZ) return 23; // half of air before water/PC

      if (pos[2] >= midpos_postY[ibar] && pos[2] <= finalZ) return 23; // half of air before water/PC

      finalZ = finalZ + DetDef::fgdWaterGlueWidth;
      if (pos[2] >= initZ && pos[2] <= finalZ) return 24; // glue after water/PC
      finalZ = finalZ + DetDef::fgdWaterSkinWidth;
      if (pos[2] >= initZ && pos[2] <= finalZ) return 25; // polypropilene skin after water/PC
//      finalZ = finalZ + (DetDef::fgdWaterAirWidth/2);
//      if (pos[2] >= initZ && pos[2] <= finalZ) return 26; // half of air after water/PC

      if (pos[2] >= initZ && pos[2] <= midpos_preX[ibar+1]) return 26; // half of air after water/PC
    }

    std::cout << "Error in GetFgdMaterialNoFV: material not found for this position." << std::endl;
    return -1;
}


//********************************************************************
anaUtils::massComponentEnum anaUtils::GetMassComponent(bool IsMC, const Float_t* pos){
//********************************************************************

  if (anaUtils::InDetVolume(SubDetId::kFGD1,pos))
    return anaUtils::kFGD1;
  else if (anaUtils::InDetVolume(SubDetId::kFGD2, pos)) {
    if (anaUtils::GetFgdModuleTypeNoFV(IsMC,pos,true) == 2)
      return anaUtils::kFGD2watermodules;
    else
      return anaUtils::kFGD2xymodules;
  }
  /*
  else if (anaUtils::InDetVolume(SubDetId::kP0D, pos)){
    if (anaUtils::GetP0DModuleTypeNoFV(event,pos,true) == 2)
      return anaUtils::kP0DWater;
    else
      return anaUtils::kP0DAir;
  }
  */
  else return kInvalid;
}
