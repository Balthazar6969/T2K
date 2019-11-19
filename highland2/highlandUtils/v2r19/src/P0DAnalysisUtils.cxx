#include "P0DAnalysisUtils.hxx"
#include "EventBoxId.hxx"
#include <TMath.h>
#include <math.h>

//********************************************************************
AnaP0DParticle* p0dUtils::GetP0DParticle(AnaParticleB* part){
  //********************************************************************

  // TODO. Is the dynamic_cast OK here ?
  AnaP0DParticle* p0dParticle = NULL;
  AnaTrack* track = dynamic_cast<AnaTrack*>(part);
  if (track){
    if (track->nP0DSegments>0)
      p0dParticle = static_cast<AnaP0DParticle*>(track->P0DSegments[0]);
  }
  else
    p0dParticle = static_cast<AnaP0DParticle*>(part);

  return p0dParticle;
}

//**************************************************
Int_t p0dUtils::GetAllP0DParticles(const AnaEventB& event, AnaP0DParticle* selParts[]) {
  //**************************************************

  int count = 0;
  for (int it = 0; it < event.nParticles; it++) {
    AnaParticleB* part = event.Particles[it];
    AnaP0DParticle* p0dParticle = GetP0DParticle(part);
    if (!p0dParticle) continue;
    selParts[count] = p0dParticle;
    count++;
  }
  return count;
}

//**************************************************
Int_t p0dUtils::GetAllP0DVertices(const AnaEventB& event, AnaP0DVertex* selverts[]) {
  //**************************************************

  // TODO. Use dynamic_cast temporarily. To be substituted by a type enum probably

  int count = 0;
  for (int iv = 0; iv < event.nVertices; iv++) {
    AnaP0DVertex* p0dVertex = dynamic_cast<AnaP0DVertex*>(event.Vertices[iv]);
    if (!p0dVertex) continue;
    selverts[count] = p0dVertex;
    count++;
  }
  return count;
}

//********************************************************************
Int_t p0dUtils::GetMichelElectrons(const AnaSpillB& spill, AnaDelayedClustersB**& delayedClusters, Int_t firstBunch){
  //********************************************************************
  //cout<<"in GetMichel"<<endl;

  std::vector<AnaBunchC*> bunches = spill.Bunches;
  if (spill.OutOfBunch) bunches.push_back(spill.OutOfBunch);

  Int_t nmichels=0;

  anaUtils::CreateArray(delayedClusters,100); 

  std::vector<int> clustersUsed;
  for (std::vector<AnaBunchC*>::iterator it = bunches.begin(); it != bunches.end(); ++it) {

    AnaP0DBunch* p0dBunch = dynamic_cast<AnaP0DBunch*> (*it);
    if (!p0dBunch) return nmichels;

    // Skip bunches before firstBunch (except OutOfBunch, with bunch nbr -1)
    if (p0dBunch->Bunch < firstBunch && p0dBunch->Bunch!=-1) continue;


    for (Int_t c = 0 ; c<p0dBunch->nClusters; c++){
      //cout<<"in cluster loop"<<endl;
      AnaP0DCluster* cluster = p0dBunch->Clusters[c];
      if (cluster->AlgorithmName.compare("TP0DTagMuonDecay") == 0 && find(clustersUsed.begin(),clustersUsed.end(),cluster->UniqueID) == clustersUsed.end() ){

        //cout<<"in if"<<endl;

        delayedClusters[nmichels] = new AnaDelayedClustersB();
        Float_t maxTime=0;
        Float_t minTime=1e10;
        for (Int_t h = 0 ; h<cluster->nHits; h++){
          AnaP0DHit* hit = cluster->Hits[h];                
          /*
             if (hit->Position[3]>maxTime) maxTime=hit->Position[3];
             if (hit->Position[3]<minTime) minTime=hit->Position[3];
             */
          if (hit->Time>maxTime) maxTime=hit->Time;
          if (hit->Time<minTime) minTime=hit->Time;
          delayedClusters[nmichels]->NHits++;        
          delayedClusters[nmichels]->RawChargeSum += hit->Charge;
        }
        delayedClusters[nmichels]->MinTime = minTime;
        delayedClusters[nmichels]->MaxTime = maxTime;

        nmichels++;
        clustersUsed.push_back(cluster->UniqueID);
      }
    }
  }

  anaUtils::ResizeArray(delayedClusters,nmichels, 100); 

  return nmichels;
}

//********************************************************************
Int_t p0dUtils::GetNMichelElectrons(const AnaSpillB& spill, Int_t firstBunch){
  //********************************************************************

  AnaDelayedClustersB** clusters;
  return GetMichelElectrons(spill,clusters,firstBunch);
}


//********************************************************************
Int_t p0dUtils::GetMichelElectrons(AnaP0DVertex* anaP0DVertex, AnaDelayedClustersB**& delayedClusters){
  //********************************************************************

  Int_t nmichels=0;

  anaUtils::CreateArray(delayedClusters,100); 

  if (anaP0DVertex->nClusters == 0) return nmichels;

  std::vector<int> clustersUsed;  
  for (Int_t c = 0 ; c < anaP0DVertex->nClusters; c++){

    AnaP0DCluster* cluster = anaP0DVertex->Clusters[c];
    if (cluster->AlgorithmName.compare("TP0DTagMuonDecay") == 0 && find(clustersUsed.begin(),clustersUsed.end(),cluster->UniqueID) == clustersUsed.end() ){

      delayedClusters[nmichels] = new AnaDelayedClustersB();
      Float_t maxTime=0;
      Float_t minTime=1e10;
      for (Int_t h = 0 ; h<cluster->nHits; h++){
        AnaP0DHit* hit = cluster->Hits[h];                
        /*
           if (hit->Position[3]>maxTime) maxTime=hit->Position[3];
           if (hit->Position[3]<minTime) minTime=hit->Position[3];
           */
        if (hit->Time>maxTime) maxTime=hit->Time;
        if (hit->Time<minTime) minTime=hit->Time;
        delayedClusters[nmichels]->NHits++;        
        delayedClusters[nmichels]->RawChargeSum += hit->Charge;
      }
      delayedClusters[nmichels]->MinTime = minTime;
      delayedClusters[nmichels]->MaxTime = maxTime;
      
      nmichels++;
      clustersUsed.push_back(cluster->UniqueID);
    }
  }


  anaUtils::ResizeArray(delayedClusters,nmichels, 100); 

  return nmichels;
}

//********************************************************************
void boxP0DUtils::FillVerticesFinal(AnaEventB& event){
  //********************************************************************

  EventBoxP0D2* EventBox = static_cast<EventBoxP0D2*>(event.EventBoxes[EventBoxId::kEventBoxP0D]);

  // Don't fill it when already filled by other selection
  if (EventBox->VerticesInGroup[EventBoxP0D2::kVerticesInP0DFinal]) return;

  AnaP0DVertex* vertices[NMAXVERTICES];
  Int_t nP0D = p0dUtils::GetAllP0DVertices(event,vertices);
  //  if(NMAXTRUEPARTICLES<(UInt_t)nTPC) nTPC=NMAXTRUEPARTICLES;
  anaUtils::CreateArray(EventBox->VerticesInGroup[EventBoxP0D2::kVerticesInP0DFinal],nP0D);
  anaUtils::CopyArray(vertices, EventBox->VerticesInGroup[EventBoxP0D2::kVerticesInP0DFinal], nP0D);
  EventBox->nVerticesInGroup[EventBoxP0D2::kVerticesInP0DFinal]= nP0D;
}


//********************************************************************
void boxP0DUtils::FillP0DParticles(AnaEventB& event){
  //********************************************************************

  EventBoxP0D2* EventBox = static_cast<EventBoxP0D2*>(event.EventBoxes[EventBoxId::kEventBoxP0D]);

  // Don't fill it when already filled by other selection
  if (EventBox->RecObjectsInGroup[EventBoxP0D2::kAllP0DParticles]) return;

  // Get all P0D particles
  AnaP0DParticle* p0dParticles[NMAXPARTICLES];
  Int_t nP0D = p0dUtils::GetAllP0DParticles(event,p0dParticles);
  Int_t nShowers=0;
  Int_t nTracks=0;

  // Create the variable size arrays
  anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBoxP0D2::kAllP0DParticles],nP0D);
  anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBoxP0D2::kP0DTracks],      nP0D);
  anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBoxP0D2::kP0DShowers],     nP0D);

  //  Fill the arrays
  for(int i = 0; i < nP0D; ++i){
    EventBox->RecObjectsInGroup[EventBoxP0D2::kAllP0DParticles][i] = p0dParticles[i];

    // Check whether it is a shower
    if (p0dParticles[i]->AlgorithmName.find("Shower")!=std::string::npos)
      EventBox->RecObjectsInGroup[EventBoxP0D2::kP0DShowers][nShowers++] = p0dParticles[i];
    else
      EventBox->RecObjectsInGroup[EventBoxP0D2::kP0DTracks][nTracks++] = p0dParticles[i];
  }

  // Resize the arrays
  anaUtils::ResizeArray(EventBox->RecObjectsInGroup[EventBoxP0D2::kP0DTracks],  nTracks, nP0D);
  anaUtils::ResizeArray(EventBox->RecObjectsInGroup[EventBoxP0D2::kP0DShowers], nShowers, nP0D);

  // set the size
  EventBox->nRecObjectsInGroup[EventBoxP0D2::kAllP0DParticles] = nP0D;
  EventBox->nRecObjectsInGroup[EventBoxP0D2::kP0DShowers]      = nShowers;
  EventBox->nRecObjectsInGroup[EventBoxP0D2::kP0DTracks]       = nTracks;

}

//Geometry Utilities to get SP0Dule, P0Dule, Layer and Bar from GeomID
//*************Geometry Utils*************************************


static const int kSP0DuleMSB = 20;
static const int kSP0DuleLSB = 18;
static const int kLayerMSB = 17;
static const int kLayerLSB = 17;
static const int kP0DuleMSB = 16;
static const int kP0DuleLSB = 11;
static const int kBarMSB = 10;
static const int kBarLSB = 0;



//********************************************************************
int p0dUtils::GetField(unsigned int geomid, int lsb, int msb){
  //********************************************************************

  int field = geomid >> lsb;
  int mask = (1<<(msb-lsb+1))-1;

  return (field & mask);

}


//********************************************************************
int p0dUtils::GetSP0DuleFromGeomId(unsigned int geomid){
  //********************************************************************

  return GetField(geomid, kSP0DuleLSB, kSP0DuleMSB);
}

//********************************************************************                                                                                       
int p0dUtils::GetP0DuleFromGeomId(unsigned int geomid){
//********************************************************************                                                                                       
  return GetField(geomid, kP0DuleLSB, kP0DuleMSB);
}

//********************************************************************                                                                                       
int p0dUtils::GetLayerFromGeomId(unsigned int geomid){
//********************************************************************                                                                                       
  return GetField(geomid, kLayerLSB, kLayerMSB);
}


//********************************************************************                                                                                       
int p0dUtils::GetBarFromGeomId(unsigned int geomid){
//********************************************************************                                                                                      
  return GetField(geomid, kBarLSB, kBarMSB);
}

