#include "p0dExampleAnalysis.hxx"
#include "FiducialVolumeDefinition.hxx"
#include "p0dExampleSelection.hxx"
#include "Parameters.hxx"
#include "CategoriesUtils.hxx"
#include "BasicUtils.hxx"
#include "P0DAnalysisUtils.hxx"
#include "P0DGeometryManager.hxx"

#include <iostream>
#include <iterator>

//***************************************************************************************
p0dExampleAnalysis::p0dExampleAnalysis(AnalysisAlgorithm* ana) : baseP0DAnalysis(ana) {
//***************************************************************************************

  // Add the package version (to be stored in the "config" tree). Nothing in this case because we are inside baseP0DAnalysis package
  //  ND::versioning().AddPackage("p0dExampleAnalysis", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("P0DEXAMPLENALYSISROOT")));
}

//***********************************************************
bool p0dExampleAnalysis::Initialize(){
//***********************************************************

  // Initialize the baseP0DAnalysis
  if(!baseP0DAnalysis::Initialize()) return false;

  // Minimum accum level to save event into the output Micro-trees
  SetMinAccumCutLevelToSave(ND::params().GetParameterI("baseP0DAnalysis.Example.MinAccumLevelToSave"));

  // for whether or not P0D Recon directory variables are read
  _isUsingReconDirP0D = ND::params().GetParameterI("highlandIO.FlatTree.UseReconDirP0D");

  // for whether or not P0D Recon directory variables are read
  _isUsingReconDirP0DNew = ND::params().GetParameterI("highlandIO.P0DDataClasses.UseReconDirP0DNew");

  // parameter to control output
  _debug = ND::params().GetParameterI("baseP0DAnalysis.Example.DebugAnalysis");

  // Add the standard categories
  anaUtils::AddStandardCategories();

  return true;
}

//***********************************************************
void p0dExampleAnalysis::DefineSelections(){
//***********************************************************

  //Add a simple Selection
  sel().AddSelection("p0dExampleSelection",       "Simple selection in the P0D" ,         new p0dExampleSelection(false));
}

//**********************************************************
void p0dExampleAnalysis::DefineCorrections(){
//**********************************************************

  baseP0DAnalysis::DefineCorrections();
}

//***********************************************************
void p0dExampleAnalysis::DefineSystematics(){
//***********************************************************

  baseP0DAnalysis::DefineSystematics();
}

//***********************************************************
void p0dExampleAnalysis::DefineConfigurations(){
//***********************************************************

  baseP0DAnalysis::DefineConfigurations();
}

//***********************************************************
void p0dExampleAnalysis::DefineMicroTrees(bool addBase){
//***********************************************************

  if (addBase) baseP0DAnalysis::DefineMicroTrees(addBase);

  AddVarVI(output(), P0DOnlyTracksInGlobalID,  "ID for P0D only tracks in global",                  nP0DOnlyTracksInGlobal);
  AddVarVI(output(), P0DTracksInGlobalID,      "ID of P0D segment in global tracks containing P0D", nP0DTracksInGlobal);

  AddVarI(output(),   nShowers,             "number of showers");
  AddVarF(output(),   Shower1EDeposit,      "EDeposit of first shower");
  AddVarF(output(),   Shower2EDeposit,      "EDeposit of second shower");
  AddVar3VF(output(), Shower1Direction,     "Direction of first shower");
  AddVar3VF(output(), Shower2Direction,     "Direction of second shower");

  AddVar4VF(output(), truevertex_pos,       "position of the selected true vertex");

  AddVarVI(output(), nClustersInVertex,        "number of P0DRecon clusters associated to each vertex",  nVertices);
  AddVarVI(output(), nHitsInVertex,            "number of P0DRecon hits associated to each vertex",      nVertices);
  AddVarMI(output(), ClusterInVertexTruePDG,   "PDG of TrueParticle associated to cluster in vertex",    nVertices,-200,100);
  output().Add3DMatrixVar(ClusterInVertexPosition,  "ClusterInVertexPosition", "F", "Position of clusters in vertex", nVertices,  
                          "nVertices", -200, 100, 4);

  output().Add3DMatrixVar(HitInVertexPosition,  "HitInVertexPosition", "F", "Position of hits in vertex", nVertices,  
                          "nVertices", -200, 100, 4);



  AddVarVI(output(), nTracksInParticle,    "number of P0DRecon tracks associated to each particle",    nParticles);
  AddVarVI(output(), nShowersInParticle,   "number of P0DRecon showers associated to each particle",   nParticles);
  AddVarVI(output(), nNodesInParticle,     "number of P0DRecon nodes associated to each particle",     nParticles);
  AddVarVI(output(), nHitsInParticle,      "number of P0DRecon hits associated to each particle",      nParticles);
  AddVar4MF(output(),ParticlePosition,     "number of P0DRecon tracks associated to each particle",    nParticles);

  AddVarVI(output(), nClustersInParticleInVertex,  "number of P0DRecon clusters associated to each particle",  nParticlesInVertex);
  AddVarVI(output(), nTracksInParticleInVertex,    "number of P0DRecon tracks associated to each particle",    nParticlesInVertex);
  AddVarVI(output(), nShowersInParticleInVertex,   "number of P0DRecon showers associated to each particle",   nParticlesInVertex);
  AddVarVI(output(), nNodesInParticleInVertex,     "number of P0DRecon nodes associated to each particle",     nParticlesInVertex);
  AddVarVI(output(), nHitsInParticleInVertex,      "number of P0DRecon hits associated to each particle",      nParticlesInVertex);
  AddVar4MF(output(),ParticleInVertexPosition,     "Position of  particle",                    nParticlesInVertex);
  AddVarVF(output(), ParticleInVertexMomentum,     "Momentum of  particle (only for showers)", nParticlesInVertex);
  AddVarVF(output(), ParticleInVertexEDeposit,     "Energy deposited (only for showers)",      nParticlesInVertex);
  AddVarVF(output(), ParticleInVertexLength,       "Length of particle (only for tracks)",     nParticlesInVertex);
  AddVarVI(output(), ParticleInVertexID,           "Unique ID of particle",                    nParticlesInVertex);
  AddVarVF(output(), ParticleInVertexTrueMomentum, "TrueMomentum of particle ",                nParticlesInVertex);
  AddVarVI(output(), ParticleInVertexTruePDG,      "TruePDG of particle",                      nParticlesInVertex);
  AddVarVI(output(), ParticleInVertexLink,         "Has this P0D particle a link in global ?", nParticlesInVertex);
  AddVarVI(output(), ParticleInVertexGlobal,       "Is this particle a global track with a P0D segment ?", nParticlesInVertex);

  AddVarMI(output(), ClusterInParticleInVertexTruePDG,   "PDG of TrueParticle associated to cluster in a particle in a vertex",    nParticlesInVertex, -200,100);

  output().Add3DMatrixVar(ClusterInParticleInVertexPosition,  "ClusterInParticleInVertexPosition", "F", "Position of clusters in particle in vertex", nParticlesInVertex,  
                          "nParticlesInVertex", -100, 100, 4);


  output().Add3DMatrixVar(HitInParticleInVertexPosition,  "HitInParticleInVertexPosition", "F", "Position of hits in particle in vertex", nParticlesInVertex,  
                          "nParticlesInVertex", -100, 200, 4);

  AddVarMF(output(),HitInParticleInVertexCharge,  "Charge of hits in particle in vertex",          nParticlesInVertex,-100, 200);
  AddVarMI(output(),HitInParticleInVertexType,    "Type (X=0, Y=1) of hits in particle in vertex", nParticlesInVertex,-100, 200);



  output().AddMatrixVar(ClusterPosition,  "ClusterPosition", "F", "Position of clusters", nClusters,  
                          "nClusters", -200, 4);


  AddVarMaxSizeVI(output(), ClusterTruePDG,   "PDG of TrueParticle associated to a cluster in a particle", nClusters,200);
  AddVarMaxSizeVI(output(), nHitsInCluster,   "number of P0DRecon hits associated to each cluster",        nClusters,200); // extend the size of the vector to 200, default is 100

  AddVarVF(output(),   vertexFiducial,    "Positive values imply within Fiducial Volume", nVertices); 
  AddVarVI(output(),   vertexCycle,       " Vertex Cycle",                                nVertices); 
  AddVarVI(output(),   vertexValidDim,    "Vertex Valid Dimensions",                      nVertices); 
  AddVar4MF(output(),  vertexPosition,    "Vertex position",                              nVertices); 

  AddVarI(output(),   nMichel,     "number of p0dreconclusters with AlgorithmName==TP0DTagMuonDecay in out-of-time bunch");
}

//***********************************************************
void p0dExampleAnalysis::DefineTruthTree(){
//***********************************************************

  baseP0DAnalysis::DefineTruthTree();
}

//***********************************************************
void p0dExampleAnalysis::FillMicroTrees(bool addBase){
//***********************************************************

  if (addBase) baseP0DAnalysis::FillMicroTrees(addBase);

  //-----------------Add variables to the anlysis tree -------------
  
  // Count the number of P0D tracks
  AnaTrackB* p0dTracks[NMAXPARTICLES];    
  int nP0D         = anaUtils::GetAllTracksUsingDet(GetBunch(), SubDetId::kP0D,  p0dTracks);

  for (Int_t i=0;i<nP0D;i++){
    if (static_cast<AnaTrack*>(p0dTracks[i])->nP0DSegments>0)
      output().FillVectorVar(P0DTracksInGlobalID, static_cast<AnaTrack*>(p0dTracks[i])->P0DSegments[0]->UniqueID);
    else
      output().FillVectorVar(P0DTracksInGlobalID, 0);
    output().IncrementCounter(nP0DTracksInGlobal);
  }

  AnaTrackB* p0dOnlyTracks[NMAXPARTICLES];
  int nP0DOnly     = anaUtils::GetAllTracksUsingOnlyDet(GetBunch(), SubDetId::kP0D,  p0dOnlyTracks);

  if (_debug) std::cout << " nP0Dtracks using _event : " << nP0D << std::endl;
  
  for (Int_t i=0;i<nP0DOnly;i++){
    if (static_cast<AnaTrack*>(p0dOnlyTracks[i])->nP0DSegments>2)
      output().FillVectorVar(P0DOnlyTracksInGlobalID, static_cast<AnaTrack*>(p0dOnlyTracks[i])->P0DSegments[0]->UniqueID);
    else
      output().FillVectorVar(P0DOnlyTracksInGlobalID, 0);
    output().IncrementCounter(nP0DOnlyTracksInGlobal);
  }

  // Use the old AnaLocalReconBunch stuff or the new strategy with fully integrated native objects
  if (_isUsingReconDirP0DNew)
    FillMicroTreesNative();
  else if (_isUsingReconDirP0D)
    FillMicroTreesLocal();

}

//***********************************************************
void p0dExampleAnalysis::FillMicroTreesLocal(){
//***********************************************************

  // Count the number of P0D tracks
  AnaTrackB* p0dTracks[NMAXPARTICLES];    
  int nP0D         = anaUtils::GetAllTracksUsingDet(GetBunch(), SubDetId::kP0D,  p0dTracks);

  // Cast this event to the local variety  
  AnaLocalReconEvent* localEvent = NULL;
  localEvent = static_cast<AnaLocalReconEvent*>(_event);
  
  // Pointer will be NULL if dynamic_cast fails
  if(!localEvent){
    std::cerr<<"Invalid local event, did you enable UseReconDirP0D = 1 in highlandIO.parameters.dat?\n";
    return;
  }
  
  if (_debug) std::cout << "For the iBunch value:" << GetEvent().Bunch << "the # of P0D Recon vertices" << localEvent->P0DReconVertices.size() << std::endl;   
  
  for(std::vector<AnaP0DReconVertex*>::iterator it =localEvent->P0DReconVertices.begin(); it!= localEvent->P0DReconVertices.end(); ++it ){
    if (_debug) std::cout << "P0DReconVertices:AlgorithmName : " << (*it)->AlgorithmName << " Cycle: " << (*it)->Cycle << " Bunch: " << (*it)->Bunch << std::endl;
    
    output().FillVectorVar(nClustersInVertex, (Int_t)(*it)->ClustersP.size());
    output().FillVectorVar(nHitsInVertex,     (Int_t)(*it)->Hits.size());
    
    output().FillVectorVar(vertexCycle,      (Int_t)(*it)->Cycle);
    output().FillVectorVar(vertexFiducial,          (*it)->Fiducial);
    output().FillVectorVar(vertexValidDim,   (Int_t)(*it)->ValidDimensions);
    output().FillMatrixVarFromArray(vertexPosition, (*it)->Position, 4 );
    
    output().IncrementCounter(nVertices);
    
    for(std::vector<AnaP0DReconParticle*>::iterator it2 =(*it)->ParticlesP.begin(); it2!= (*it)->ParticlesP.end(); ++it2 ){
      if (_debug) std::cout << "P0DReconParticles:AlgorithmName : " << (*it2)->AlgorithmName << " Cycle: " << (*it2)->Cycle << " Bunch: " << (*it2)->Bunch << std::endl;
      
      if (!(*it2)) continue;
      output().FillVectorVar(nClustersInParticleInVertex, (Int_t)(*it2)->Clusters.size());
      output().FillVectorVar(nTracksInParticleInVertex,   (Int_t)(*it2)->Tracks.size());
      output().FillVectorVar(nShowersInParticleInVertex,  (Int_t)(*it2)->Showers.size());
      output().FillVectorVar(nNodesInParticleInVertex,    (Int_t)(*it2)->Nodes.size());
      output().FillVectorVar(nHitsInParticleInVertex,     (Int_t)(*it2)->Hits.size());

      output().FillMatrixVarFromArray(ParticleInVertexPosition, (*it2)->Position, 4 );            
      output().FillVectorVar(ParticleInVertexMomentum, (*it2)->Momentum);
      output().FillVectorVar(ParticleInVertexEDeposit, (*it2)->EDeposit);
      output().FillVectorVar(ParticleInVertexLength,   (*it2)->Length);
      output().FillVectorVar(ParticleInVertexID,       (Int_t)(*it2)->UniqueID);
      if ((*it2)->TrueParticle){
        output().FillVectorVar(ParticleInVertexTrueMomentum,   (*it2)->TrueParticle->Momentum);
        output().FillVectorVar(ParticleInVertexTruePDG,        (*it2)->TrueParticle->PDG);
      }

      bool found=false;
      for (Int_t i=0;i<nP0D;i++){
        AnaP0DParticle* part = p0dUtils::GetP0DParticle(p0dTracks[i]);
        if (part->UniqueID ==  (Int_t)(*it2)->UniqueID){
          found=true;
          break;
        }
      }
      
      output().FillVectorVar(ParticleInVertexLink, (Int_t)found);
      
      output().IncrementCounter(nParticlesInVertex);      
    }
  }
    
  for(std::vector<AnaP0DReconParticle*>::iterator it =localEvent->P0DReconParticles.begin(); it!= localEvent->P0DReconParticles.end(); ++it ){
    if (_debug) std::cout << "P0DReconParticles:AlgorithmName : " << (*it)->AlgorithmName << " Cycle: " << (*it)->Cycle << " Bunch: " << (*it)->Bunch << std::endl;
    
    output().FillVectorVar(nTracksInParticle,   (Int_t)(*it)->Tracks.size());
    output().FillVectorVar(nShowersInParticle,  (Int_t)(*it)->Showers.size());
    output().FillVectorVar(nNodesInParticle,    (Int_t)(*it)->Nodes.size());
    output().FillVectorVar(nHitsInParticle,     (Int_t)(*it)->Hits.size());
    output().IncrementCounter(nParticles);
  }
  
  for(std::vector<AnaP0DReconCluster*>::iterator it =localEvent->P0DReconClusters.begin(); it!= localEvent->P0DReconClusters.end(); ++it ){
    if (_debug) std::cout << "P0DReconClusters:AlgorithmName : " << (*it)->AlgorithmName << " Cycle: " << (*it)->Cycle << "Bunch: " << (*it)->Bunch << std::endl;
    
    output().FillVectorVar(nHitsInCluster,     (Int_t)(*it)->Hits.size());
    output().IncrementCounter(nClusters);
  }    
  
  // Count the number of P0D michel candidates  
  int nmichels = 0;
  
  // Check clusters in bunches equal to or after bunch of HMNtrack
  for (std::vector<AnaBunchC*>::iterator it = GetSpill().Bunches.begin();
       it != GetSpill().Bunches.end(); ++it) {
    AnaLocalReconBunch* p0dRecoBunch = dynamic_cast<AnaLocalReconBunch*> (*it);
    
    // Skip bunches before HMNtrack
    //      if (p0dRecoBunch->Bunch < ((AnaTrack*) box().HMNtrack)->Bunch) continue;
    
    for (std::vector<AnaP0DReconCluster*>::iterator it = p0dRecoBunch->P0DReconClusters.begin();
         it != p0dRecoBunch->P0DReconClusters.end(); ++it) {
      if (_debug) std::cout << "P0DReconCluster (*it)->AlgorithmName: " << (*it)->AlgorithmName << std::endl;
      if (_debug) std::cout << "P0DReconCluster (*it)->Position: " << (*it)->Position << std::endl;
      if ((*it)->AlgorithmName.compare("TP0DTagMuonDecay") == 0) nmichels += 1;
    }
  }

  // !!!!!!!!!!!!!!!!!! The box was not filled by the Selection. HMN will be empty
  // Check clusters in out of bunch with time > HMNtrack->time
  /* TODO HMNTrack not available now in the box
  AnaLocalReconBunch* p0dRecoOutOfBunch = dynamic_cast<AnaLocalReconBunch*> (GetSpill().OutOfBunch);
  if (p0dRecoOutOfBunch) {
    for (std::vector<AnaP0DReconCluster*>::iterator it = p0dRecoOutOfBunch->P0DReconClusters.begin();
         it != p0dRecoOutOfBunch->P0DReconClusters.end(); ++it) {
      if (box().HMNtrack && (*it)->Position[3] > box().HMNtrack->PositionStart[3] &&
          (*it)->AlgorithmName.compare("TP0DTagMuonDecay") == 0) nmichels += 1;
    }
  }
  */
  output().FillVar(nMichel, nmichels);
}

//***********************************************************
void p0dExampleAnalysis::FillMicroTreesNative(){
//***********************************************************

  /*
    This method only uses the AnaEventB. All information is fully integrated using psyche philosophy:
    - No derived buch type
    - No use of spill to get the michel electrons
  */

  static P0DGeometryManager geom;

  // Cast this event to the local variety  
  AnaP0DEvent* p0dEvent = static_cast<AnaP0DEvent*>(_event);

  if (_debug){
    std::cout << "-------------------------- AlternateEvents structure for event " << p0dEvent->EventInfo.Event 
              << " and bunch " << p0dEvent->Bunch << " --------------------------" << std::endl;
    std::cout << " - " << p0dEvent->FullName << std::endl;
    for (UInt_t i=0;i<p0dEvent->AlternateEvents.size();i++){
      std::cout << "   - " << p0dEvent->AlternateEvents[i]->FullName << std::endl;
      for (UInt_t j=0;j<p0dEvent->AlternateEvents[i]->AlternateEvents.size();j++){
        std::cout << "     - " << p0dEvent->AlternateEvents[i]->AlternateEvents[j]->FullName << std::endl;
        for (UInt_t k=0;k<p0dEvent->AlternateEvents[i]->AlternateEvents[j]->AlternateEvents.size();k++){
          std::cout << "       - " << p0dEvent->AlternateEvents[i]->AlternateEvents[j]->AlternateEvents[k]->FullName << std::endl;
          for (UInt_t l=0;l<p0dEvent->AlternateEvents[i]->AlternateEvents[j]->AlternateEvents[k]->AlternateEvents.size();l++){
            std::cout << "         - " << p0dEvent->AlternateEvents[i]->AlternateEvents[j]->AlternateEvents[k]->AlternateEvents[l]->FullName << std::endl;
          }
        }
      }
    }
    std::cout << "-------------------------------------------------------------------------------------------------------------" << std::endl;
  }

  // This is just to verify that all tracks including in vertices are also in the array of tracks in the event
  Int_t nPartsInVertices=0;

  // Get all objects containing P0D segment (global tracks and p0d only tracks)
  AnaTrackB* p0dTracks[NMAXPARTICLES];    
  int nP0D         = anaUtils::GetAllTracksUsingDet(GetEvent(), SubDetId::kP0D,  p0dTracks);

  if (_debug) std::cout << "For the iBunch value:" << GetEvent().Bunch << "the # of P0D Recon vertices is " << GetEvent().nVertices << std::endl;   

  // ------- Loop over vertices in the event and select the ones of type AnaP0DVertex ------------------

  // Get all vertices of type AnaP0DVertex
  AnaP0DVertex* p0dVertices[NMAXVERTICES];
  Int_t nVerts = p0dUtils::GetAllP0DVertices(GetEvent(),p0dVertices);
  
  for(Int_t iv=0;iv<nVerts; iv++ ){
    AnaP0DVertex* p0dVertex = p0dVertices[iv];

    if (_debug) std::cout << "P0DReconVertices:AlgorithmName : " << p0dVertex->AlgorithmName << " Bunch: " << p0dVertex->Bunch << std::endl;
    
    // ---- Fill Vertex variables -----------
    output().FillVectorVar(nClustersInVertex, (Int_t)p0dVertex->nClusters);    
    output().FillVectorVar(vertexFiducial,           p0dVertex->Fiducial);
    output().FillVectorVar(vertexValidDim,    (Int_t)p0dVertex->ValidDimensions);
    output().FillMatrixVarFromArray(vertexPosition,  p0dVertex->Position, 4 );

    // ------ Loop over clusters in the vertex ----------      
    Int_t nhits = 0;
    for(Int_t ic=0;ic<p0dVertex->nClusters; ic++ ){
      AnaP0DCluster* p0dCluster = static_cast<AnaP0DCluster*>(p0dVertex->Clusters[ic]);
      
      // ---- Fill cluster variables -----------
      output().Fill3DMatrixVarFromArray(ClusterInVertexPosition,  p0dCluster->Position, -1, ic, 4);
      if (p0dCluster->TrueParticle)
        output().FillMatrixVar(ClusterInVertexTruePDG,            p0dCluster->TrueParticle->PDG, -1, ic);
      
      // ---- Fill the Hit position ----------
      for(Int_t ih=0;ih<p0dCluster->nHits; ih++ ){
        if (nhits>=200) break;
        AnaP0DHit* p0dHit = static_cast<AnaP0DHit*>(p0dCluster->Hits[ih]);
        // Get the 3D position from the GeomID
        TVector3 pos = geom.GeomIdPosition(p0dHit->GeomID);          
        Float_t posArray[4]={pos[0],pos[1],pos[2],p0dHit->Time};
        //        output().Fill3DMatrixVarFromArray(HitInVertexPosition,  p0dHit->Position, -1, nhits, 4);
        output().Fill3DMatrixVarFromArray(HitInVertexPosition,  posArray, -1, nhits, 4);
        nhits++;
      }
    }
    output().FillVectorVar(nHitsInVertex, nhits);    


    // ------ Loop over particles in the vertex ----------
    for(Int_t ip=0;ip<p0dVertex->nParticles; ip++ ){
      // Get the P0DParticle from the track in the vertex. That means either the P0Donly track or the segment inside a global track
      AnaP0DParticle* p0dParticle = p0dUtils::GetP0DParticle(p0dVertex->Particles[ip]);
      if (!p0dParticle) continue;

      if (_debug) std::cout << "P0DReconParticles:AlgorithmName : " << p0dParticle->AlgorithmName << std::endl;
      
      // ---- Fill Particle variables -----------
      output().FillVectorVar(nClustersInParticleInVertex, (Int_t)p0dParticle->nClusters);

      output().FillMatrixVarFromArray(ParticleInVertexPosition, p0dParticle->PositionStart, 4 );      
      output().FillVectorVar(ParticleInVertexMomentum,  p0dParticle->Momentum);
      output().FillVectorVar(ParticleInVertexEDeposit,  p0dParticle->EDeposit);
      output().FillVectorVar(ParticleInVertexLength,    p0dParticle->Length);
      output().FillVectorVar(ParticleInVertexID, (Int_t)p0dParticle->UniqueID);

      if (p0dParticle->TrueObject){
        output().FillVectorVar(ParticleInVertexTrueMomentum,   p0dParticle->GetTrueParticle()->Momentum);
        output().FillVectorVar(ParticleInVertexTruePDG,        p0dParticle->GetTrueParticle()->PDG);
      }

      // ------ Loop over clusters in the particle ----------      
      nhits=0;
      for(Int_t ic=0;ic<p0dParticle->nClusters; ic++ ){
        AnaP0DCluster* p0dCluster = static_cast<AnaP0DCluster*>(p0dParticle->Clusters[ic]);

        // ---- Fill cluster variables -----------
        output().Fill3DMatrixVarFromArray(ClusterInParticleInVertexPosition,  p0dCluster->Position, -1, ic, 4);
        if (p0dCluster->TrueParticle)
          output().FillMatrixVar(ClusterInParticleInVertexTruePDG,            p0dCluster->TrueParticle->PDG, -1, ic);


        // ---- Fill the Hit variables ----------
        for(Int_t ih=0;ih<p0dCluster->nHits; ih++ ){
          if (nhits>=200) break;
          AnaP0DHit* p0dHit = static_cast<AnaP0DHit*>(p0dCluster->Hits[ih]);
          // Get the 3D position from the GeomID
          TVector3 pos = geom.GeomIdPosition(p0dHit->GeomID);          
          Float_t posArray[4]={pos[0],pos[1],pos[2],p0dHit->Time};

          // Get the hit type
          Int_t type = ND::GeomId::P0D::GetBarLayer(p0dHit->GeomID);          

          output().Fill3DMatrixVarFromArray(HitInParticleInVertexPosition,  posArray,         -1, nhits, 4);
          output().FillMatrixVar(           HitInParticleInVertexCharge,    p0dHit->Charge,   -1, nhits);            
          output().FillMatrixVar(           HitInParticleInVertexType,      type,             -1, nhits);

          nhits++;
        }
      }

      output().FillVectorVar(nHitsInParticleInVertex, nhits);    

      // ------ Is there any segment in a global track with the same ID  ----------            
      bool found=false;
      for (Int_t i=0;i<nP0D;i++){
        AnaP0DParticle* part = p0dUtils::GetP0DParticle(p0dTracks[i]);
        if (part->UniqueID ==  (Int_t)p0dParticle->UniqueID){
          found=true;
          break;
        }
      }

      //      output().FillVectorVar(ParticleInVertexGlobal, (Int_t)(static_cast<AnaTrack*>(p0dParticle)->nP0DSegments!=0));   TODO
      output().FillVectorVar(ParticleInVertexLink, (Int_t)found);

      nPartsInVertices++;
      output().IncrementCounter(nParticlesInVertex);
    }

    output().IncrementCounter(nVertices);    
  }

  // ------ Loop over particles in the event  ----------

  // This is just to verify that all tracks including in vertices are also in the array of tracks in the event
  AnaP0DParticle* p0dParticles[NMAXPARTICLES];
  Int_t nParts = p0dUtils::GetAllP0DParticles(GetEvent(), p0dParticles);
  if (_debug && nParts != nPartsInVertices) std::cout << "warning: #Particles in vertices and #Particles in event differ !!!!" << std::endl;

  for(Int_t ip=0;ip<nParts; ip++ ){
    AnaP0DParticle* p0dParticle = p0dParticles[ip];

    if (_debug) std::cout << "P0DReconParticles:AlgorithmName : " << p0dParticle->AlgorithmName << std::endl;

    output().FillMatrixVarFromArray(ParticlePosition, p0dParticle->PositionStart, 4 );      
    output().IncrementCounter(nParticles);
  }



  // ---- Count the number of P0D michel candidates. This is precomputed inside oaAnalysisTreeConverter using the method p0dUtils::GetMichelElectrons  
  Int_t nmichels = static_cast<AnaEvent*>(_event)->nDelayedClusters;  
  output().FillVar(nMichel, nmichels);

  // ------ Loop over clusters in the event  ----------
  for(Int_t i = 0; i< p0dEvent->nClusters;i++ ){
    //    if (_debug) std::cout << "P0DReconClusters:AlgorithmName : " << (*it)->AlgorithmName << " Cycle: " << (*it)->Cycle << "Bunch: " << (*it)->Bunch << std::endl;
    AnaP0DCluster* cluster = p0dEvent->Clusters[i];

    output().FillVectorVar(nHitsInCluster, cluster->nHits);
    output().FillMatrixVarFromArray(ClusterPosition, cluster->Position,4);
    
    if (cluster->TrueParticle)
      output().FillVectorVar(ClusterTruePDG,  cluster->TrueParticle->PDG);
    
    output().IncrementCounter(nClusters);
  }    

  // ---- Fill variables needing the box (computed in the selection) -----------

  // downcast the box to the appropriate type
  const ToyBoxP0D* p0dBox = static_cast<const ToyBoxP0D*>(&boxB());

  output().FillVar(nShowers, p0dBox->nShowers);  

  if (p0dBox->Shower1){
    output().FillVar(Shower1EDeposit,  p0dBox->Shower1->EDeposit);  
    output().FillVectorVarFromArray(Shower1Direction, p0dBox->Shower1->DirectionStart,3);  
  }
  if (p0dBox->Shower2){
    output().FillVar(Shower2EDeposit,  p0dBox->Shower2->EDeposit);  
    output().FillVectorVarFromArray(Shower2Direction, p0dBox->Shower2->DirectionStart,3);  
  }

  if (p0dBox->TrueVertex){
    output().FillVectorVarFromArray(truevertex_pos, p0dBox->TrueVertex->Position,4);  
  }

}

//***********************************************************
void p0dExampleAnalysis::FillToyVarsInMicroTrees(bool addBase){
//***********************************************************

  if (addBase) baseP0DAnalysis::FillToyVarsInMicroTreesBase(addBase);
}

//*********************************************************************
bool p0dExampleAnalysis::CheckFillTruthTree(const AnaTrueVertex& vtx){
//*********************************************************************

  /* To avoid unecessary events in the "truth" tree in this method we define the condition to include or not a given                                          
     true vertex in the tree.                                                                                                                                 
  */
  // In this case we only save neutral currents  (|ReacCode|>=30) interactions in the P0D FV
  bool NC=(abs(vtx.ReacCode)>=30);
  return (anaUtils::InFiducialVolume(SubDetId::kP0D, vtx.Position, FVDef::FVdefminP0D,FVDef::FVdefmaxP0D) && NC);
}

//*********************************************************************
void p0dExampleAnalysis::FillTruthTree(const AnaTrueVertex& vtx){
//*********************************************************************

  baseP0DAnalysis::FillTruthTreeBase(vtx);
}

//********************************************************************
void p0dExampleAnalysis::FillCategories(){
//********************************************************************

  // For the muon candidate  
  anaUtils::FillCategories(box().TrueVertex,"", SubDetId::kP0D, GetEvent().GetIsSandMC());
}
