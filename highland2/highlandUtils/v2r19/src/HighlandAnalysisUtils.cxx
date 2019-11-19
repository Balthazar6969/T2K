#include "HighlandAnalysisUtils.hxx"
#include <TMath.h>
#include <math.h>

//**************************************************
int anaUtils::GetAllChargedTrajOnlyDsECal(const AnaEventB& event, AnaTrueParticleB* DsEcalChargedTracks[] ) {///*****mirar esto!! luego añadir varibales q puso sasha y mirar si pinta micro tree, añadir edeposit y posicion a ver q pinta y asociar edeposit con emenergy || mip energy
//**************************************************
    int nDsECalChargedTracks = 0;
    AnaTrueParticleB* trajInBunch[NMAXPARTICLES];
    int nChargedTrajInBunch = GetAllChargedTrajInBunch(event,trajInBunch);
    for(int i=0;i<nChargedTrajInBunch;i++){    
        for(int idet=0;idet<trajInBunch[i]->nDetCrossings;idet++){
            if((SubDetId::GetDetectorUsed(trajInBunch[i]->DetCrossings[idet]->Detector, SubDetId::kFGD1)) || 
                (SubDetId::GetDetectorUsed(trajInBunch[i]->DetCrossings[idet]->Detector, SubDetId::kFGD2)) || 
                (SubDetId::GetDetectorUsed(trajInBunch[i]->DetCrossings[idet]->Detector, SubDetId::kTPC1)) || 
                (SubDetId::GetDetectorUsed(trajInBunch[i]->DetCrossings[idet]->Detector, SubDetId::kTPC2)) ||
                (SubDetId::GetDetectorUsed(trajInBunch[i]->DetCrossings[idet]->Detector, SubDetId::kTPC3)) || 
                (SubDetId::GetDetectorUsed(trajInBunch[i]->DetCrossings[idet]->Detector, SubDetId::kP0D))  ||
                (SubDetId::GetDetectorUsed(trajInBunch[i]->DetCrossings[idet]->Detector, SubDetId::kPECAL)) ||
                (SubDetId::GetDetectorUsed(trajInBunch[i]->DetCrossings[idet]->Detector, SubDetId::kSMRD))) {continue;}

            //i.e crossing ONLY the active part of the DsECal, no POD ECal or any other sub-detector 
            if (SubDetId::GetDetectorUsed(trajInBunch[i]->DetCrossings[idet]->Detector, SubDetId::kDSECAL) && trajInBunch[i]->DetCrossings[idet]->InActive){
                // In Ds not needed cut on lenght
                /* double dist = sqrt(pow(trajInBunch[i]->DetCrossings[idet]->ExitPosition[0]-trajInBunch[i]->DetCrossings[idet]->EntrancePosition[0] ,2) +
                                   pow(trajInBunch[i]->DetCrossings[idet]->ExitPosition[1]-trajInBunch[i]->DetCrossings[idet]->EntrancePosition[1] ,2) + 
                                   pow(trajInBunch[i]->DetCrossings[idet]->ExitPosition[2]-trajInBunch[i]->DetCrossings[idet]->EntrancePosition[2] ,2) + 
                                   pow(trajInBunch[i]->DetCrossings[idet]->ExitPosition[3]-trajInBunch[i]->DetCrossings[idet]->EntrancePosition[3] ,2)); */
                //    Value of ECal threshold to set    
                //    if ((trajInBunch[i]->Momentum<80) ) continue;   
                DsEcalChargedTracks[nDsECalChargedTracks] = trajInBunch[i];
                nDsECalChargedTracks++;
            }
        }
    }
    return nDsECalChargedTracks;
}

//**************************************************
int anaUtils::GetAllChargedTrajOnlyBrECal(const AnaEventB& event, AnaTrueParticleB* BrECalChargedTracks[]) {
//**************************************************
    int nBrECalChargedTracks = 0;
    AnaTrueParticleB* trajInBunch[NMAXPARTICLES];
    int nChargedTrajInBunch = GetAllChargedTrajInBunch(event,trajInBunch);
    for(int i=0;i<nChargedTrajInBunch;i++){  
        for(int idet=0;idet<trajInBunch[i]->nDetCrossings;idet++){
            if((SubDetId::GetDetectorUsed(trajInBunch[i]->DetCrossings[idet]->Detector, SubDetId::kFGD1)) ||
                (SubDetId::GetDetectorUsed(trajInBunch[i]->DetCrossings[idet]->Detector, SubDetId::kFGD2)) ||
                (SubDetId::GetDetectorUsed(trajInBunch[i]->DetCrossings[idet]->Detector, SubDetId::kTPC1)) ||
                (SubDetId::GetDetectorUsed(trajInBunch[i]->DetCrossings[idet]->Detector, SubDetId::kTPC2)) ||
                (SubDetId::GetDetectorUsed(trajInBunch[i]->DetCrossings[idet]->Detector, SubDetId::kTPC3)) ||
                (SubDetId::GetDetectorUsed(trajInBunch[i]->DetCrossings[idet]->Detector, SubDetId::kP0D))  ||
                (SubDetId::GetDetectorUsed(trajInBunch[i]->DetCrossings[idet]->Detector, SubDetId::kPECAL)) ||
                (SubDetId::GetDetectorUsed(trajInBunch[i]->DetCrossings[idet]->Detector, SubDetId::kSMRD))) {continue;}

            //i.e crossing ONLY the active part of the BrECal 
            if (SubDetId::GetDetectorUsed(trajInBunch[i]->DetCrossings[idet]->Detector, SubDetId::kTECAL) && trajInBunch[i]->DetCrossings[idet]->InActive ){
                //double dist = sqrt(pow(trajInBunch[i]->DetCrossings[idet]->ExitPosition[0]-trajInBunch[i]->DetCrossings[idet]->EntrancePosition[0] ,2) +
                   //                pow(trajInBunch[i]->DetCrossings[idet]->ExitPosition[1]-trajInBunch[i]->DetCrossings[idet]->EntrancePosition[1] ,2) +
                     //              pow(trajInBunch[i]->DetCrossings[idet]->ExitPosition[2]-trajInBunch[i]->DetCrossings[idet]->EntrancePosition[2] ,2) +
                       //            pow(trajInBunch[i]->DetCrossings[idet]->ExitPosition[3]-trajInBunch[i]->DetCrossings[idet]->EntrancePosition[3] ,2));
            //    Value of ECal threshold to set    
            //    if (TMath::Abs(dist) < 35 || (trajInBunch[i]->Momentum<30) ) continue;
                BrECalChargedTracks[nBrECalChargedTracks] = trajInBunch[i];
                nBrECalChargedTracks++;
            }
        }
    }
    return nBrECalChargedTracks;
}

//*********************************************************
AnaVertexB* anaUtils::GetGlobalVertex(const AnaEventB& event, AnaTrackB** Tracks, int nTracks){
//*********************************************************
  AnaVertexB* vertex = NULL;
  
  // Loop over vertices available in the event
  for (int i = 0; i < event.nVertices; i++){
    
    if (!event.Vertices[i]) continue;
    
    int nMatch_tmp  = 0;
    int nTracks_tmp = 0;
    bool tracks_loop = false;
    
    // Loop through vertex constituents
    for (int j = 0; j < event.Vertices[i]->nParticles; j++){
      
      if (!event.Vertices[i]->Particles[j]) continue;
      
      // Loop through input tracks
      for (int k = 0; k < nTracks; k++){
        if (!Tracks[k]) continue;
        
        // Count valid tracks
        if (!tracks_loop) nTracks_tmp++; 
        
        // Check whether have the same objects, check UniqueID just in case some copying took place 
        // (probably redundunt)
        if (event.Vertices[i]->Particles[j]->UniqueID == Tracks[k]->UniqueID)
          nMatch_tmp++;
      }
      
      tracks_loop = true; 
    }
    
    // Check whether all valid tracks provided belong to the vertex
    if (nTracks_tmp == nMatch_tmp){ 
      vertex = event.Vertices[i];
      break;
    }
  }
  
  return vertex;

}
