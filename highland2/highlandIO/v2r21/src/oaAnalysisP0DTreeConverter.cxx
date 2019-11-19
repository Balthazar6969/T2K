#include "oaAnalysisTreeConverter.hxx"
#include "InputManager.hxx"
#include "Versioning.hxx"
#include "BasicUtils.hxx"
#include "HighlandAnalysisUtils.hxx"
#include "TreeConverterUtils.hxx"
#include "Parameters.hxx"
#include "P0DAnalysisUtils.hxx"
#include "P0DGeometryManager.hxx"

// ---------- TEST HIGHLAND2-P0D -------------

//*****************************************************************************
void oaAnalysisTreeConverter::GetBunchLocalP0DObjects(){
//*****************************************************************************

  // Clear the vectors used to create links between the local p0d recon objects:  indices --> pointers
  _AnaP0DVertices.clear();
  _AnaP0DParticles.clear();
  _AnaP0DClusters.clear();
  _AnaP0DTracks.clear();
  _AnaP0DShowers.clear();

  _AnaP0DVertices.resize(NP0DReconVertices);
  _AnaP0DParticles.resize(NP0DReconParticles);
  _AnaP0DClusters.resize(NP0DReconClusters);
  _AnaP0DTracks.resize(NP0DReconTracks);
  _AnaP0DShowers.resize(NP0DReconShowers);

  // Get index of the Top level TP0DAlgoRes  as indicated in the parameters file
  _topP0DAlgoRes=-1;
  for (int iP0D=0; iP0D<NP0DAlgoResults; ++iP0D){
    ND::TP0DReconModule::TP0DAlgoRes *p0dAlgoRes = (ND::TP0DReconModule::TP0DAlgoRes*) (*P0DAlgoResults)[iP0D];
    //    std::cout << p0dAlgoRes->AlgorithmName << " " << p0dAlgoRes->FullName << std::endl;
    if (p0dAlgoRes->AlgorithmName == _p0dAlgoResName){
      _topP0DAlgoRes = iP0D;
      break;
    }
  }

  GetBunchLocalP0DObjectsInAlgoResult(_topP0DAlgoRes);
}

//*****************************************************************************
void oaAnalysisTreeConverter::GetBunchLocalP0DObjectsInAlgoResult(Int_t iAlgoRes){
//*****************************************************************************

  if (iAlgoRes==-1) return;
  ND::TP0DReconModule::TP0DAlgoRes *p0dAlgoRes = (ND::TP0DReconModule::TP0DAlgoRes*) (*P0DAlgoResults)[iAlgoRes];
  if (!p0dAlgoRes) return;

  for (unsigned int i=0;i<NBUNCHES+1;i++){
    _bunchP0DVerticesInAlgoRes [i][iAlgoRes].clear();
    _bunchP0DClustersInAlgoRes [i][iAlgoRes].clear();
    _bunchP0DParticlesInAlgoRes[i][iAlgoRes].clear();
    _bunchP0DTracksInAlgoRes   [i][iAlgoRes].clear();
    _bunchP0DShowersInAlgoRes  [i][iAlgoRes].clear();
  }

  // Loop over all the P0D vertices in the selected AlgorithmResult
  for (UInt_t iP0D=0; iP0D<p0dAlgoRes->Vertices.size(); ++iP0D){

    ND::TP0DReconModule::TP0DVertex *p0dVertex = (ND::TP0DReconModule::TP0DVertex*) (*P0DReconVertices)[p0dAlgoRes->Vertices[iP0D]];
    
    Int_t ibunch = _bunching.GetBunch(p0dVertex->Position.T(), RunID, IsMC, cosmic_mode);
    if (ibunch==-1) ibunch = NBUNCHES;

    _bunchP0DVerticesInAlgoRes[ibunch][iAlgoRes].push_back(p0dAlgoRes->Vertices[iP0D]);    
  }
  
  // Loop over all the P0D clusters in the AlgoRes
  for (UInt_t iP0D=0; iP0D<p0dAlgoRes->Clusters.size(); ++iP0D){
    ND::TP0DReconModule::TP0DCluster *p0dCluster = (ND::TP0DReconModule::TP0DCluster*) (*P0DReconClusters)[p0dAlgoRes->Clusters[iP0D]];
    
    Int_t ibunch = _bunching.GetBunch(p0dCluster->Position.T(), RunID, IsMC, cosmic_mode);      
    if (ibunch==-1) ibunch = NBUNCHES;
    
    _bunchP0DClustersInAlgoRes[ibunch][iAlgoRes].push_back(p0dAlgoRes->Clusters[iP0D]);
  }
  
  // Loop over all the P0D particles in the AlgoRes
  for (UInt_t iP0D=0; iP0D<p0dAlgoRes->Particles.size(); ++iP0D){
    ND::TP0DReconModule::TP0DParticle *p0dParticle = (ND::TP0DReconModule::TP0DParticle*) (*P0DReconParticles)[p0dAlgoRes->Particles[iP0D]];
    
    Int_t ibunch = _bunching.GetBunch(p0dParticle->Position.T(), RunID, IsMC, cosmic_mode);      
    if (ibunch==-1) ibunch = NBUNCHES;
    
    _bunchP0DParticlesInAlgoRes[ibunch][iAlgoRes].push_back(p0dAlgoRes->Particles[iP0D]);
  }
  
  // Loop over all the P0D tracks in the AlgoRes
  for (UInt_t iP0D=0; iP0D<p0dAlgoRes->Tracks.size(); ++iP0D){
    ND::TP0DReconModule::TP0DTrack *p0dTrack = (ND::TP0DReconModule::TP0DTrack*) (*P0DReconTracks)[p0dAlgoRes->Tracks[iP0D]];
    
    Int_t ibunch = _bunching.GetBunch(p0dTrack->Position.T(), RunID, IsMC, cosmic_mode);      
    if (ibunch==-1) ibunch = NBUNCHES;
    
    _bunchP0DTracksInAlgoRes[ibunch][iAlgoRes].push_back(p0dAlgoRes->Tracks[iP0D]);
  }
  
  // Loop over all the P0D showers in the AlgoRes
  for (UInt_t iP0D=0; iP0D<p0dAlgoRes->Showers.size(); ++iP0D){
    ND::TP0DReconModule::TP0DShower *p0dShower = (ND::TP0DReconModule::TP0DShower*) (*P0DReconShowers)[p0dAlgoRes->Showers[iP0D]];
    
    Int_t ibunch = _bunching.GetBunch(p0dShower->Position.T(), RunID, IsMC, cosmic_mode);      
    if (ibunch==-1) ibunch = NBUNCHES;
    
    _bunchP0DShowersInAlgoRes[ibunch][iAlgoRes].push_back(p0dAlgoRes->Showers[iP0D]);
  }

  // Loop over all the P0D AlgoResults in the AlgoRes
  for (UInt_t iP0D=0; iP0D<p0dAlgoRes->AlgoResults.size(); ++iP0D){
    GetBunchLocalP0DObjectsInAlgoResult(p0dAlgoRes->AlgoResults[iP0D]);
  }
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillP0DBunch(AnaBunchB* bunch, Int_t ibunch, Int_t iAlgoRes){
//*****************************************************************************

  if(!_isUsingReconDirP0DNew) return;

  // Cast to the P0D type
  AnaP0DBunch* p0dBunch = static_cast<AnaP0DBunch*>(bunch);

  // Get The TP0DAlgoRes with the required index
  if (iAlgoRes==-1) return;
  ND::TP0DReconModule::TP0DAlgoRes *p0dAlgoRes = (ND::TP0DReconModule::TP0DAlgoRes*) (*P0DAlgoResults)[iAlgoRes];

  // Fill the P0D Water Status 
  p0dBunch->P0DWaterStatus = P0DWaterStatus;

  // Fill the FullName 
  p0dBunch->FullName = p0dAlgoRes->FullName;

  // Fill P0D vertices
  FillP0DVertices(_bunchP0DVerticesInAlgoRes[ibunch][iAlgoRes], p0dBunch);
  
  // Fill P0D particles
  FillP0DParticles(           _bunchP0DParticlesInAlgoRes[ibunch][iAlgoRes], p0dBunch);
  FillP0DParticlesFromTracks( _bunchP0DTracksInAlgoRes   [ibunch][iAlgoRes], p0dBunch);
  FillP0DParticlesFromShowers(_bunchP0DShowersInAlgoRes  [ibunch][iAlgoRes], p0dBunch);
  
  // Fill P0D clusters
  FillP0DClusters(_bunchP0DClustersInAlgoRes[ibunch][iAlgoRes], p0dBunch);

  // Fill Alternates  
  for (UInt_t iAlt=0;iAlt<p0dAlgoRes->AlgoResults.size();iAlt++){
    AnaP0DBunch* alt = MakeP0DBunch();
    p0dBunch->AlternateBunches.push_back(alt);
    FillP0DBunch(alt, ibunch, p0dAlgoRes->AlgoResults[iAlt]);
  }
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillP0DVertices(const std::vector<Int_t>& indices, AnaP0DBunch* bunch){
//*****************************************************************************

  // Loop over all the P0D vertices in the event
  for (UInt_t iP0D=0; iP0D<indices.size(); ++iP0D){
    ND::TP0DReconModule::TP0DVertex *p0dVertex = (ND::TP0DReconModule::TP0DVertex*) (*P0DReconVertices)[indices[iP0D]];

    // get the highland vertex 
    AnaP0DVertex *anaP0D = GetAnaP0DVertex(indices[iP0D]);
    
    // Fill the AnaP0DVertex with the TP0DVertex information
    FillP0DVertexInfo(*p0dVertex,anaP0D, bunch);
    // If this object had a time outside the bunches, the anaP0D pointer is null
    // so don't add it to the bunch or the flat tree.
    if(anaP0D)  bunch->Vertices.push_back(anaP0D);
  }
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillP0DClusters(const std::vector<Int_t>& indices, AnaP0DBunch* bunch){
//*****************************************************************************

  // Create the array of clusters
  bunch->nClusters = 0;
  anaUtils::CreateArray(bunch->Clusters, (Int_t)indices.size());

  // Loop over all the P0D clusters in the AlgoRes
  for (UInt_t iP0D=0; iP0D<indices.size(); ++iP0D){
    ND::TP0DReconModule::TP0DCluster *p0dCluster = (ND::TP0DReconModule::TP0DCluster*) (*P0DReconClusters)[indices[iP0D]];

    // get the highland cluster
    AnaP0DCluster *anaP0D = GetAnaP0DCluster(indices[iP0D]);

    // Fill the AnaP0DCluster with the TP0DCluster information
    FillP0DClusterInfo(*p0dCluster,anaP0D);

    // If this object had a time outside the bunches, the anaP0D pointer is null
    // so don't add it to the bunch or the flat tree.
    if(anaP0D) bunch->Clusters[bunch->nClusters++] = anaP0D;
  }
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillP0DParticles(const std::vector<Int_t>& indices, AnaP0DBunch* bunch){
//*****************************************************************************

  // Loop over all the P0D particles in the AlgoRes
  for (UInt_t iP0D=0; iP0D<indices.size(); ++iP0D){
    ND::TP0DReconModule::TP0DParticle *p0dParticle = (ND::TP0DReconModule::TP0DParticle*) (*P0DReconParticles)[indices[iP0D]];

    // get the highland particle
    AnaP0DParticle *anaP0D = GetAnaP0DParticle(indices[iP0D]);

    // Fill the AnaP0DParticle with the TP0DParticle information
    FillP0DParticleInfo(*p0dParticle,anaP0D);

    // If this object had a time outside the bunches, the anaP0D pointer is null
    // so don't add it to the bunch or the flat tree.
    if(anaP0D) bunch->Particles.push_back(anaP0D);      
  } 
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillP0DParticlesFromTracks(const std::vector<Int_t>& indices, AnaP0DBunch* bunch){
//*****************************************************************************

  // Loop over all the P0D tracks in the AlgoRes
  for (UInt_t iP0D=0; iP0D<indices.size(); ++iP0D){
    ND::TP0DReconModule::TP0DTrack *p0dTrack = (ND::TP0DReconModule::TP0DTrack*) (*P0DReconTracks)[indices[iP0D]];

    // get the highland particle
    AnaP0DParticle *anaP0D = GetAnaP0DParticleFromTrack(indices[iP0D]);

    // Fill the AnaP0DParticle with the TP0DTrack information
    FillP0DParticleInfo(*p0dTrack,anaP0D);

    // If this object had a time outside the bunches, the anaP0D pointer is null
    // so don't add it to the bunch or the flat tree.
    if(anaP0D) bunch->Particles.push_back(anaP0D);      
  } 
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillP0DParticlesFromShowers(const std::vector<Int_t>& indices, AnaP0DBunch* bunch){
//*****************************************************************************

  // Loop over all the P0D showers in the AlgoRes
  for (UInt_t iP0D=0; iP0D<indices.size(); ++iP0D){
    ND::TP0DReconModule::TP0DShower *p0dShower = (ND::TP0DReconModule::TP0DShower*) (*P0DReconShowers)[indices[iP0D]];

    // get the highland particle
    AnaP0DParticle *anaP0D = GetAnaP0DParticleFromShower(indices[iP0D]);

    // Fill the AnaP0DParticle with the TP0DShower information
    FillP0DParticleInfo(*p0dShower,anaP0D);

    // If this object had a time outside the bunches, the anaP0D pointer is null
    // so don't add it to the bunch or the flat tree.
    if(anaP0D) bunch->Particles.push_back(anaP0D);      
  } 
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillP0DVertexInfo(ND::TP0DReconModule::TP0DVertex& p0dVertex, AnaP0DVertex* anaP0DVertex, AnaBunchB* bunch){
//*****************************************************************************

  // --- Add general info ----

  anaP0DVertex->AlgorithmName         = p0dVertex.AlgorithmName;
  anaP0DVertex->NHits                 = p0dVertex.NHits;
  //  anaP0DVertex->UniqueID              = p0dVertex.UniqueID;
  anaP0DVertex->Status                = p0dVertex.Status;
  anaP0DVertex->ValidDimensions       = p0dVertex.ValidDimensions;
  anaP0DVertex->Fiducial              = p0dVertex.Fiducial;

  anaUtils::VectorToArray(p0dVertex.Position,    anaP0DVertex->Position   );
  anaUtils::VectorToArray(p0dVertex.PosVariance, anaP0DVertex->PosVariance);


  Float_t max_charge=0;
  Int_t  max_ID=-1;
  
  // --- Add particles to the vertex ----

  anaP0DVertex->nParticles = 0;
  anaUtils::CreateArray(anaP0DVertex->Particles, p0dVertex.Particles.size());
  for (UInt_t i=0;i<p0dVertex.Particles.size();i++){
    ND::TP0DReconModule::TP0DParticle *p0dReconParticle = (ND::TP0DReconModule::TP0DParticle*) (*P0DReconParticles)[p0dVertex.Particles[i]];
    AnaP0DParticle* p0dParticle = GetAnaP0DParticle(p0dVertex.Particles[i]);

    // Check whether this particle was already added to the bunch->Particles vector by Global (oaAnalysisTreeConverter::FillPIDs)
    bool found=false;
    for (UInt_t j=0;j<bunch->Particles.size();j++){
      // The AnaParticleB can be either a global track (AnaTrack) or a AnaP0DParticle. In both cases it could have been added by global 
      // either as a global track with more constituents or as a p0d-only AnaP0DParticle
      AnaTrack* globalTrack = dynamic_cast<AnaTrack*>(bunch->Particles[j]);      
      if (globalTrack){
        // When it is a global track it should have a P0D segment with the same ID as the AnaP0DParticle
        if (globalTrack->nP0DSegments!=0 && globalTrack->P0DSegments[0]->UniqueID==(Int_t)p0dReconParticle->UniqueID){
          found=true;
          
          // Now we have the choice to add the global track to the vertex or just the local particle (below)
          if (_addGlobalTracksToP0DVertices){
            // Add the global track to the array of particles in the vertex
            anaP0DVertex->Particles[anaP0DVertex->nParticles++] = globalTrack;
            
            // Stablish the link between the particle and the vertices it is associated to
            AddReconVertexToReconParticle(anaP0DVertex, globalTrack);
          }          
        }
      }
      // When it is not a global track, the ID of the AnaP0DParticle created by global should be the same as the AnaP0DParticle in p0dRecon module
      else if ((UInt_t)bunch->Particles[j]->UniqueID==p0dReconParticle->UniqueID){
        found=true;
        if (_addGlobalTracksToP0DVertices){
          // Add the AnaP0DParticle created by global to the array of particles in the vertex
          anaP0DVertex->Particles[anaP0DVertex->nParticles++] = bunch->Particles[j];
          // We cannot Stablish the link between the particle and the vertices it is associated to because 
          // AnaP0DParticle does not have a vector of ReconVertices (it does not inherit from AnaParticleE). TODO
        }
      }      
      if (found) break;  // In either case the AnaP0DParticle was already found in global
    }


    // When the object found by global is added to the P0D vertex we don't need to add the one found in p0dRecon as well. 
    // In either case the AnaP0DParticle must be filled 
    if (!found || !_addGlobalTracksToP0DVertices){ 

      // Add the particle to the array of particles in the vertex
      anaP0DVertex->Particles[anaP0DVertex->nParticles++] = p0dParticle;
      
      // Fill the particle info
      FillP0DParticleInfo(*p0dReconParticle,p0dParticle);

      // Stablish the link between the particle and the vertices it is associated to. TODO
      //      AddReconVertexToReconParticle(anaP0DVertex, p0dParticle);
    }
   
    // If this local P0DRecon object is not yet in the list of Particles (by Global) we should add it
    if (!found){
      // Add the particle to the vector in the bunch
      bunch->Particles.push_back(p0dParticle);      
    }

    // Compute the true particle that contributes the most to this recon vertex
    for(UInt_t j = 0; j < p0dReconParticle->Truth_TrajIDs.size(); j++ ){
      if (p0dReconParticle->Truth_ChargeShare[j] > max_charge){
        max_charge = p0dReconParticle->Truth_ChargeShare[j];
        max_ID = p0dReconParticle->Truth_TrajIDs[j];
      }        
    }
  }

  // --- Add clusters to the vertex ----
  anaP0DVertex->nClusters = 0;
  anaUtils::CreateArray(anaP0DVertex->Clusters, p0dVertex.Clusters.size());  
  for (UInt_t i=0;i<p0dVertex.Clusters.size();i++){
    ND::TP0DReconModule::TP0DCluster  *p0dCluster2 = (ND::TP0DReconModule::TP0DCluster*) (*P0DReconClusters)[p0dVertex.Clusters[i]];
    AnaP0DCluster* p0dCluster3 = GetAnaP0DCluster(p0dVertex.Clusters[i]);
    
    // Add the cluster to the array of clusters in the vertex
    anaP0DVertex->Clusters[anaP0DVertex->nClusters++] = p0dCluster3;
    
    // Fill the cluster info
    FillP0DClusterInfo(*p0dCluster2,p0dCluster3);
  }

  // --- Associate the AnaTrueVertexB linked to the AnaTrueParticleB that contributes the most to this recon vertex ---
  anaP0DVertex->TrueVertex=NULL;
  std::vector<AnaTrueParticleB*>::iterator it;
  for (it = _spill->TrueParticles.begin(); it!=_spill->TrueParticles.end();it++){
    AnaTrueParticleB* trueTrack = *it;
    if (max_ID == trueTrack->ID){
      anaP0DVertex->TrueVertex = trueTrack->TrueVertex;
      static_cast<AnaTrueVertex*>(anaP0DVertex->TrueVertex)->ReconVertices.push_back(anaP0DVertex);
      break; // Stop when association is found.
    }
  }

}

//*****************************************************************************
void oaAnalysisTreeConverter::FillP0DParticleInfo(ND::TP0DReconModule::TP0DParticle& p0dParticle, AnaP0DParticle* anaP0DParticle){
//*****************************************************************************
  // --- Add general info ----

  // This is a P0D-only object 
  SubDetId::SetDetectorUsed(anaP0DParticle->Detector, SubDetId::kP0D);

  anaUtils::VectorToArray(p0dParticle.Position,    anaP0DParticle->PositionStart );
  anaUtils::VectorToArray(p0dParticle.Direction,   anaP0DParticle->DirectionStart);       

  //  anaP0DParticle->Type                  = AnaP0dParticle::kShower;
  anaP0DParticle->AlgorithmName         = p0dParticle.AlgorithmName;
  anaP0DParticle->NHits                 = p0dParticle.NHits;
  anaP0DParticle->UniqueID              = p0dParticle.UniqueID;
  anaP0DParticle->Status                = p0dParticle.Status;
  anaP0DParticle->SideDeposit           = p0dParticle.SideDeposit;
  anaP0DParticle->EndDeposit            = p0dParticle.EndDeposit;
  anaP0DParticle->ValidDimensions       = p0dParticle.ValidDimensions;
  anaP0DParticle->Momentum              = p0dParticle.Momentum;        
  anaP0DParticle->Charge                = p0dParticle.Charge;          
  anaP0DParticle->realPIDNames          = p0dParticle.realPIDNames;    
  anaP0DParticle->realPIDValues         = p0dParticle.realPIDValues;   
  anaP0DParticle->integerPIDNames       = p0dParticle.integerPIDNames; 
  anaP0DParticle->integerPIDValues      = p0dParticle.integerPIDValues;
  anaP0DParticle->PID                   = p0dParticle.PID;             
  anaP0DParticle->PID_weight            = p0dParticle.PID_weight;      

  // --- Add Information only available for tracks ----
  if (p0dParticle.Tracks.size()==1){
    anaP0DParticle->Type = AnaP0DParticle::kTrack;

    
    ND::TP0DReconModule::TP0DTrack *p0dTrack = (ND::TP0DReconModule::TP0DTrack*) (*P0DReconTracks)[p0dParticle.Tracks[0]];
    anaP0DParticle->Length           = p0dTrack->Length;

    // Add the clusters to the particle
    anaP0DParticle->nClusters = 0;
    anaUtils::CreateArray(anaP0DParticle->Clusters, p0dTrack->Nodes.size());
    for (UInt_t i=0;i<p0dTrack->Nodes.size();i++){
      ND::TP0DReconModule::TP0DNode *p0dNode = (ND::TP0DReconModule::TP0DNode*) (*P0DReconNodes)[p0dTrack->Nodes[i]];

      // Must create an ad-hoc cluster since the P0DTrack does not contain clusters
      AnaP0DCluster* p0dCluster = MakeP0DCluster();

      // Add the cluster to the array of clusters in the particle
      anaP0DParticle->Clusters[anaP0DParticle->nClusters++] = p0dCluster;

      // Fill the cluster info from a P0DNode
      FillP0DClusterInfo(*p0dNode, p0dTrack->Cycle, p0dCluster);
    }
  }
  else   if (p0dParticle.Tracks.size()>1){
    std::cout << "more than 1 track in particle" << std::endl;
  }

  // --- Add Information only available for showers ----
  if (p0dParticle.Showers.size()==1){
    anaP0DParticle->Type = AnaP0DParticle::kShower;

    
    ND::TP0DReconModule::TP0DShower *p0dShower = (ND::TP0DReconModule::TP0DShower*) (*P0DReconShowers)[p0dParticle.Showers[0]];
    anaP0DParticle->EDeposit         = p0dShower->EDeposit;

    //Check if the particle is 3D
    if (p0dParticle.ValidDimensions == 7) {     
      // Add the clusters to the particle
      anaP0DParticle->nClusters = 0;
      anaUtils::CreateArray(anaP0DParticle->Clusters, p0dShower->Clusters.size());
      for (UInt_t i=0;i<p0dShower->Clusters.size();i++){
	ND::TP0DReconModule::TP0DCluster *p0dCluster2 = (ND::TP0DReconModule::TP0DCluster*) (*P0DReconClusters)[p0dShower->Clusters[i]];
	AnaP0DCluster* p0dCluster3 = GetAnaP0DCluster(p0dShower->Clusters[i]);
	
	// Add the cluster to the array of clusters in the particle
	anaP0DParticle->Clusters[anaP0DParticle->nClusters++] = p0dCluster3;
	
	// Fill the cluster info
	FillP0DClusterInfo(*p0dCluster2,p0dCluster3);
	
      }
    }
    else {
      // Shower->Cluster = 0 in p0dRecon for NOT 3D Particles.
      //However to save the hits information, we make 1 cluster and add it to the p0dParticle for Highland2.
      
      anaP0DParticle->nClusters = 0;
      anaUtils::CreateArray(anaP0DParticle->Clusters, 1);
      
      AnaP0DCluster* p0dCluster3 = MakeP0DCluster();
      /*
      // Add the clusters to the particle. Build them from clusters
      anaP0DParticle->nClusters = 0;
      anaUtils::CreateArray(anaP0DParticle->Clusters, p0dShower->Clusters.size());
      for (UInt_t i=0;i<p0dShower->Clusters.size();i++){
      ND::TP0DReconModule::TP0DCluster *p0dCluster2 = (ND::TP0DReconModule::TP0DCluster*) (*P0DReconClusters)[p0dShower->Clusters[i]];
      AnaP0DCluster* p0dCluster3 = GetAnaP0DCluster(p0dShower->Clusters[i]);
       */

      //Add the cluster to the array of clusters in the particle
      anaP0DParticle->Clusters[anaP0DParticle->nClusters++] = p0dCluster3;
      
      //Fill the cluster info
      FillP0DClusterInfo(*p0dShower, p0dCluster3);
      
    }

    /*

    // Add the clusters to the particle. Build them from nodes (Clark's advice)
    anaP0DParticle->nClusters = 0;
    anaUtils::CreateArray(anaP0DParticle->Clusters, p0dShower->Nodes.size());
    for (UInt_t i=0;i<p0dShower->Nodes.size();i++){
      ND::TP0DReconModule::TP0DNode *p0dNode = (ND::TP0DReconModule::TP0DNode*) (*P0DReconNodes)[p0dShower->Nodes[i]];

      // Must create an ad-hoc cluster since the P0DTrack does not contain clusters
      AnaP0DCluster* p0dCluster = MakeP0DCluster();

      // Add the cluster to the array of clusters in the particle
      anaP0DParticle->Clusters[anaP0DParticle->nClusters++] = p0dCluster;

      // Fill the cluster info from a P0DNode
      FillP0DClusterInfo(*p0dNode,p0dCluster);
    }
    */
  }
  else   if (p0dParticle.Showers.size()>1){
    std::cout << "more than 1 shower in particle" << std::endl;
  }


  //--- Compute the true particle that contributes the most to this recon particle  ---
  Float_t max_charge=0;
  Int_t  max_index=-1;
  Float_t total_charge=0;
  for (UInt_t i = 0; i<p0dParticle.Truth_PrimaryTrajIDs.size();i++){
    if (p0dParticle.Truth_ChargeShare[i] > max_charge){
      max_charge = p0dParticle.Truth_ChargeShare[i];
      max_index = i; 
    }
    total_charge += p0dParticle.Truth_ChargeShare[i];
    //    std::cout << p0dParticle.Truth_PrimaryTrajIDs[i] << " " << p0dParticle.Truth_TrajIDs[i] << " " << p0dParticle.Truth_HitCount[i] << " " << p0dParticle.Truth_ChargeShare[i] <<std::endl;
  }


  // --- Associate the AnaTrueParticleB that contributes the most to this recon particle ---
  anaP0DParticle->TrueObject=NULL;
  if (max_index>-1){
    std::vector<AnaTrueParticleB*>::iterator it;
    for (it = _spill->TrueParticles.begin(); it!=_spill->TrueParticles.end();it++){
      AnaTrueParticleB* trueTrack = *it;
      
      if (p0dParticle.Truth_TrajIDs[max_index] == trueTrack->ID){
        anaP0DParticle->TrueObject = trueTrack;
        if (p0dParticle.Truth_ChargeShare[max_index]/total_charge!=0)
          static_cast<AnaTrueParticle*>(trueTrack)->Purity = p0dParticle.Truth_ChargeShare[max_index]/total_charge;
        
        break; // Stop when association is found.
      }
    }
  }

  // --- Save all recon tracks associated to this true track ---
  if (anaP0DParticle->TrueObject){
    static_cast<AnaTrueParticle*>(anaP0DParticle->TrueObject)->ReconParticles.push_back(anaP0DParticle);

    if (anaP0DParticle->GetTrueParticle()->TrueVertex){
      // Save all recon tracks associated to this true  vertex (track without truth association might be missing here)
      static_cast<AnaTrueVertex*>(anaP0DParticle->GetTrueParticle()->TrueVertex)->ReconParticles.push_back(anaP0DParticle);
    }
  }


  /*
  for (UInt_t i = 0; i<p0dParticle.realPIDNames.size();i++){
    std::cout << i << " (realPID): "  
              << p0dParticle.realPIDNames[i] << " --> "; 
    for (UInt_t j = 0; j<p0dParticle.realPIDValues[i].size();j++)
      std::cout << p0dParticle.realPIDValues[i][j] << " ";
    std::cout << std::endl;
  }
  for (UInt_t i = 0; i<p0dParticle.integerPIDNames.size();i++){
    std::cout << i << " (IntPID): "  
              << p0dParticle.integerPIDNames[i] << " --> "; 
      for (UInt_t j = 0; j<p0dParticle.integerPIDValues[i].size();j++)
        std::cout << p0dParticle.integerPIDValues[i][j] << " ";
    std::cout << std::endl;
  }

  for (UInt_t i = 0; i<p0dParticle.PID.size();i++)
    std::cout << i << " (PID) : "  
              << p0dParticle.PID[i] << " " 
              << p0dParticle.PID_weight[i] << std::endl;
  */

}

//*****************************************************************************
void oaAnalysisTreeConverter::FillP0DParticleInfo(ND::TP0DReconModule::TP0DTrack& p0dTrack, AnaP0DParticle* anaP0DParticle){
//*****************************************************************************
  // --- Add general info ----

  // This is a P0D-only object 
  SubDetId::SetDetectorUsed(anaP0DParticle->Detector, SubDetId::kP0D);

  anaUtils::VectorToArray(p0dTrack.Position,    anaP0DParticle->PositionStart );
  anaUtils::VectorToArray(p0dTrack.Direction,   anaP0DParticle->DirectionStart);       

  anaP0DParticle->Type                  = AnaP0DParticle::kTrack;
  anaP0DParticle->AlgorithmName         = p0dTrack.AlgorithmName;
  anaP0DParticle->NHits                 = p0dTrack.NHits;
  anaP0DParticle->UniqueID              = p0dTrack.UniqueID;
  anaP0DParticle->Status                = p0dTrack.Status;
  anaP0DParticle->SideDeposit           = p0dTrack.SideDeposit;
  anaP0DParticle->EndDeposit            = p0dTrack.EndDeposit;
  anaP0DParticle->ValidDimensions       = p0dTrack.ValidDimensions;
  /*
  anaP0DParticle->Momentum              = p0dTrack.Momentum;        
  anaP0DParticle->Charge                = p0dTrack.Charge;          
  anaP0DParticle->realPIDNames          = p0dTrack.realPIDNames;    
  anaP0DParticle->realPIDValues         = p0dTrack.realPIDValues;   
  anaP0DParticle->integerPIDNames       = p0dTrack.integerPIDNames; 
  anaP0DParticle->integerPIDValues      = p0dTrack.integerPIDValues;
  anaP0DParticle->PID                   = p0dTrack.PID;             
  anaP0DParticle->PID_weight            = p0dTrack.PID_weight;      
  */
  anaP0DParticle->Length           = p0dTrack.Length;
  
  // Add the clusters to the particle
  anaP0DParticle->nClusters = 0;
  anaUtils::CreateArray(anaP0DParticle->Clusters, p0dTrack.Nodes.size());
  for (UInt_t i=0;i<p0dTrack.Nodes.size();i++){
    ND::TP0DReconModule::TP0DNode *p0dNode = (ND::TP0DReconModule::TP0DNode*) (*P0DReconNodes)[p0dTrack.Nodes[i]];
    
    // Must create an ad-hoc cluster since the P0DTrack does not contain clusters
    AnaP0DCluster* p0dCluster = MakeP0DCluster();
    
    // Add the cluster to the array of clusters in the particle
    anaP0DParticle->Clusters[anaP0DParticle->nClusters++] = p0dCluster;

    //    p0dCluster->Cycle                  = p0dTrack.Cycle;
    // Fill the cluster info from a P0DNode
    FillP0DClusterInfo(*p0dNode, p0dTrack.Cycle, p0dCluster);
  }

  //--- Compute the true particle that contributes the most to this recon particle  ---
  Float_t max_charge=0;
  Int_t  max_index=-1;
  Float_t total_charge=0;
  for (UInt_t i = 0; i<p0dTrack.Truth_PrimaryTrajIDs.size();i++){
    if (p0dTrack.Truth_ChargeShare[i] > max_charge){
      max_charge = p0dTrack.Truth_ChargeShare[i];
      max_index = i; 
    }
    total_charge += p0dTrack.Truth_ChargeShare[i];
    //    std::cout << p0dTrack.Truth_PrimaryTrajIDs[i] << " " << p0dTrack.Truth_TrajIDs[i] << " " << p0dTrack.Truth_HitCount[i] << " " << p0dTrack.Truth_ChargeShare[i] <<std::endl;
  }


  // --- Associate the AnaTrueParticleB that contributes the most to this recon particle ---
  anaP0DParticle->TrueObject=NULL;
  if (max_index>-1){
    std::vector<AnaTrueParticleB*>::iterator it;
    for (it = _spill->TrueParticles.begin(); it!=_spill->TrueParticles.end();it++){
      AnaTrueParticleB* trueTrack = *it;
      
      if (p0dTrack.Truth_TrajIDs[max_index] == trueTrack->ID){
        anaP0DParticle->TrueObject = trueTrack;
        if (p0dTrack.Truth_ChargeShare[max_index]/total_charge!=0)
          static_cast<AnaTrueParticle*>(trueTrack)->Purity = p0dTrack.Truth_ChargeShare[max_index]/total_charge;
        
        break; // Stop when association is found.
      }
    }
  }

  // --- Save all recon tracks associated to this true track ---
  if (anaP0DParticle->TrueObject){
    static_cast<AnaTrueParticle*>(anaP0DParticle->TrueObject)->ReconParticles.push_back(anaP0DParticle);

    if (anaP0DParticle->GetTrueParticle()->TrueVertex){
      // Save all recon tracks associated to this true  vertex (track without truth association might be missing here)
      static_cast<AnaTrueVertex*>(anaP0DParticle->GetTrueParticle()->TrueVertex)->ReconParticles.push_back(anaP0DParticle);
    }
  }


}

//*****************************************************************************
void oaAnalysisTreeConverter::FillP0DParticleInfo(ND::TP0DReconModule::TP0DShower& p0dShower, AnaP0DParticle* anaP0DParticle){
//*****************************************************************************
  // --- Add general info ----
  
  // This is a P0D-only object 
  SubDetId::SetDetectorUsed(anaP0DParticle->Detector, SubDetId::kP0D);

  anaUtils::VectorToArray(p0dShower.Position,    anaP0DParticle->PositionStart );
  anaUtils::VectorToArray(p0dShower.Direction,   anaP0DParticle->DirectionStart);       

  anaP0DParticle->Type                  = AnaP0DParticle::kShower;
  anaP0DParticle->AlgorithmName         = p0dShower.AlgorithmName;
  anaP0DParticle->NHits                 = p0dShower.NHits;
  anaP0DParticle->UniqueID              = p0dShower.UniqueID;
  anaP0DParticle->Status                = p0dShower.Status;
  anaP0DParticle->SideDeposit           = p0dShower.SideDeposit;
  anaP0DParticle->EndDeposit            = p0dShower.EndDeposit;
  anaP0DParticle->ValidDimensions       = p0dShower.ValidDimensions;

  /*
  anaP0DParticle->Momentum              = p0dShower.Momentum;        
  anaP0DParticle->Charge                = p0dShower.Charge;          
  anaP0DParticle->realPIDNames          = p0dShower.realPIDNames;    
  anaP0DParticle->realPIDValues         = p0dShower.realPIDValues;   
  anaP0DParticle->integerPIDNames       = p0dShower.integerPIDNames; 
  anaP0DParticle->integerPIDValues      = p0dShower.integerPIDValues;
  anaP0DParticle->PID                   = p0dShower.PID;             
  anaP0DParticle->PID_weight            = p0dShower.PID_weight;      
  */
  anaP0DParticle->Length           = p0dShower.Length;
  
  // Add the clusters to the particle (As it is a shower -- build it from Clusters)

  if (p0dShower.ValidDimensions == 7){
    anaP0DParticle->nClusters = 0;
    anaUtils::CreateArray(anaP0DParticle->Clusters, p0dShower.Clusters.size());
    for (UInt_t i=0;i<p0dShower.Clusters.size();i++){
      ND::TP0DReconModule::TP0DCluster *p0dCluster2 = (ND::TP0DReconModule::TP0DCluster*) (*P0DReconClusters)[p0dShower.Clusters[i]];
      
      AnaP0DCluster* p0dCluster3 = GetAnaP0DCluster(p0dShower.Clusters[i]);
      
      // Add the cluster to the array of clusters in the particle
      anaP0DParticle->Clusters[anaP0DParticle->nClusters++] = p0dCluster3;
      
      // Fill the cluster info from a P0DNode
      FillP0DClusterInfo(*p0dCluster2,p0dCluster3);
    
    /*
    // Add the clusters to the particle
    anaP0DParticle->nClusters = 0;
    anaUtils::CreateArray(anaP0DParticle->Clusters, p0dShower.Nodes.size());
    for (UInt_t i=0;i<p0dShower.Nodes.size();i++){
    ND::TP0DReconModule::TP0DNode *p0dNode = (ND::TP0DReconModule::TP0DNode*) (*P0DReconNodes)[p0dShower.Nodes[i]];
    
    // Must create an ad-hoc cluster since the P0DShower does not contain clusters
    AnaP0DCluster* p0dCluster = MakeP0DCluster();
    
    // Add the cluster to the array of clusters in the particle
    anaP0DParticle->Clusters[anaP0DParticle->nClusters++] = p0dCluster;
    
    // Fill the cluster info from a P0DNode
    FillP0DClusterInfo(*p0dNode,p0dCluster);
    */    
    }
  }
  else {
	  // Shower->Cluster = 0 in p0dRecon for NOT 3D Particles.
	  //However to save the hits information, we make 1 cluster and add it to the p0dParticle for Highland2.
	  
	  anaP0DParticle->nClusters = 0;
	  anaUtils::CreateArray(anaP0DParticle->Clusters, 1);
	  
	  AnaP0DCluster* p0dCluster3 = MakeP0DCluster();
	  
	  //Add the cluster to the array of clusters in the particle
	  anaP0DParticle->Clusters[anaP0DParticle->nClusters++] = p0dCluster3;
	  
	  //Fill the cluster info
	  FillP0DClusterInfo(p0dShower, p0dCluster3);

  }
  //--- Compute the true particle that contributes the most to this recon particle  ---
  Float_t max_charge=0;
  Int_t  max_index=-1;
  Float_t total_charge=0;
  for (UInt_t i = 0; i<p0dShower.Truth_PrimaryTrajIDs.size();i++){
    if (p0dShower.Truth_ChargeShare[i] > max_charge){
      max_charge = p0dShower.Truth_ChargeShare[i];
      max_index = i; 
    }
    total_charge += p0dShower.Truth_ChargeShare[i];
    //    std::cout << p0dShower.Truth_PrimaryTrajIDs[i] << " " << p0dShower.Truth_TrajIDs[i] << " " << p0dShower.Truth_HitCount[i] << " " << p0dShower.Truth_ChargeShare[i] <<std::endl;
  }


  // --- Associate the AnaTrueParticleB that contributes the most to this recon particle ---
  anaP0DParticle->TrueObject=NULL;
  if (max_index>-1){
    std::vector<AnaTrueParticleB*>::iterator it;
    for (it = _spill->TrueParticles.begin(); it!=_spill->TrueParticles.end();it++){
      AnaTrueParticleB* trueTrack = *it;
      
      if (p0dShower.Truth_TrajIDs[max_index] == trueTrack->ID){
        anaP0DParticle->TrueObject = trueTrack;
        if (p0dShower.Truth_ChargeShare[max_index]/total_charge!=0)
          static_cast<AnaTrueParticle*>(trueTrack)->Purity = p0dShower.Truth_ChargeShare[max_index]/total_charge;
        
        break; // Stop when association is found.
      }
    }
  }

  // --- Save all recon tracks associated to this true track ---
  if (anaP0DParticle->TrueObject){
    static_cast<AnaTrueParticle*>(anaP0DParticle->TrueObject)->ReconParticles.push_back(anaP0DParticle);

    if (anaP0DParticle->GetTrueParticle()->TrueVertex){
      // Save all recon tracks associated to this true  vertex (track without truth association might be missing here)
      static_cast<AnaTrueVertex*>(anaP0DParticle->GetTrueParticle()->TrueVertex)->ReconParticles.push_back(anaP0DParticle);
    }
  }


}


//*****************************************************************************
void oaAnalysisTreeConverter::FillP0DClusterInfo(ND::TP0DReconModule::TP0DCluster& p0dCluster, AnaP0DCluster* anaP0DCluster){
//*****************************************************************************

  anaP0DCluster->AlgorithmName         = p0dCluster.AlgorithmName;
  anaP0DCluster->UniqueID              = p0dCluster.UniqueID;
  anaP0DCluster->ValidDimensions       = p0dCluster.ValidDimensions;
  //  anaP0DCluster->NFiducialHits         = p0dCluster.NFiducialHits;        
  anaP0DCluster->Cycle                 = p0dCluster.Cycle;
  anaP0DCluster->EDeposit              = p0dCluster.EDeposit;          

  anaUtils::CopyArray(p0dCluster.Moments,    anaP0DCluster->Moments, 9);

  anaUtils::VectorToArray(p0dCluster.Position,    anaP0DCluster->Position   );
  anaUtils::VectorToArray(p0dCluster.PosVariance, anaP0DCluster->PosVariance);

  // Add the hits to the cluster
  anaP0DCluster->nHits = 0;
  anaUtils::CreateArray(anaP0DCluster->Hits, p0dCluster.Hits.size());
  for (UInt_t i=0;i<p0dCluster.Hits.size();i++){
    ND::TP0DReconModule::TP0DHit *p0dHit = (ND::TP0DReconModule::TP0DHit*) (*P0DReconHits)[p0dCluster.Hits[i]];
    
    // Must create an ad-hoc hit since the P0DTrack does not contain hits
    AnaP0DHit* p0dHit2 = MakeP0DHit();
    
    // Add the hit to the array of hits in the cluster
    anaP0DCluster->Hits[anaP0DCluster->nHits++] = p0dHit2;
    
    // Fill the hit info from a P0DNode
    FillP0DHitInfo(*p0dHit,p0dHit2);
  }

  //--- Compute the true particle that contributes the most to this recon cluster  ---
  Float_t max_charge=0;
  Int_t  max_index=-1;
  for (UInt_t i = 0; i<p0dCluster.Truth_PrimaryTrajIDs.size();i++){
    if (p0dCluster.Truth_ChargeShare[i] > max_charge){
      max_charge = p0dCluster.Truth_ChargeShare[i];
      max_index = i; 
    }
  }

  // --- Associate the AnaTrueParticleB that contributes the most to this recon particle ---
  anaP0DCluster->TrueParticle=NULL;
  if (max_index>-1){
    std::vector<AnaTrueParticleB*>::iterator it;
    for (it = _spill->TrueParticles.begin(); it!=_spill->TrueParticles.end();it++){
      AnaTrueParticleB* trueTrack = *it;
      if (p0dCluster.Truth_TrajIDs[max_index] == trueTrack->ID){
        anaP0DCluster->TrueParticle = static_cast<AnaTrueParticleB*>(trueTrack);
        break; // Stop when association is found.
      }
    }
  }

}

//*****************************************************************************
void oaAnalysisTreeConverter::FillP0DClusterInfo(ND::TP0DReconModule::TP0DNode& p0dNode, Short_t Cycle,  AnaP0DCluster* anaP0DCluster){
//*****************************************************************************

  //  anaP0DCluster->AlgorithmName         = p0dNode.AlgorithmName;
  //  anaP0DCluster->UniqueID              = p0dNode.UniqueID;
  anaP0DCluster->ValidDimensions       = p0dNode.ValidDimensions;
  //  anaP0DCluster->NFiducialHits         = p0dNode.NFiducialHits;        
  anaP0DCluster->EDeposit              = p0dNode.EDeposit;

  anaP0DCluster->Cycle                 = Cycle;

  anaUtils::VectorToArray(p0dNode.Position,    anaP0DCluster->Position   );
  anaUtils::VectorToArray(p0dNode.PosVariance, anaP0DCluster->PosVariance);

  // Add the hits to the cluster
  anaP0DCluster->nHits = 0;
  anaUtils::CreateArray(anaP0DCluster->Hits, p0dNode.Hits.size());
  for (UInt_t i=0;i<p0dNode.Hits.size();i++){
    ND::TP0DReconModule::TP0DHit *p0dHit = (ND::TP0DReconModule::TP0DHit*) (*P0DReconHits)[p0dNode.Hits[i]];
    
    // Must create an ad-hoc hit since the P0DTrack does not contain hits
    AnaP0DHit* p0dHit2 = MakeP0DHit();
    
    // Add the hit to the array of hits in the cluster
    anaP0DCluster->Hits[anaP0DCluster->nHits++] = p0dHit2;
    
    // Fill the hit info from a P0DNode
    FillP0DHitInfo(*p0dHit,p0dHit2);
  }


  //--- Compute the true particle that contributes the most to this recon cluster  ---
  Float_t max_charge=0;
  Int_t  max_index=-1;
  for (UInt_t i = 0; i<p0dNode.Truth_PrimaryTrajIDs.size();i++){
    if (p0dNode.Truth_ChargeShare[i] > max_charge){
      max_charge = p0dNode.Truth_ChargeShare[i];
      max_index = i; 
    }
  }

  // --- Associate the AnaTrueParticleB that contributes the most to this recon particle ---
  anaP0DCluster->TrueParticle=NULL;
  if (max_index>-1){
    std::vector<AnaTrueParticleB*>::iterator it;
    for (it = _spill->TrueParticles.begin(); it!=_spill->TrueParticles.end();it++){
      AnaTrueParticleB* trueTrack = *it;
      if (p0dNode.Truth_TrajIDs[max_index] == trueTrack->ID){
        anaP0DCluster->TrueParticle = static_cast<AnaTrueParticleB*>(trueTrack);
        break; // Stop when association is found.
      }
    }
  }

}
//Function to fill 2D clusters with hit information from showers
//*****************************************************************************
void oaAnalysisTreeConverter::FillP0DClusterInfo(ND::TP0DReconModule::TP0DShower& p0dShower, AnaP0DCluster* anaP0DCluster){
//*****************************************************************************

  anaP0DCluster->AlgorithmName         = p0dShower.AlgorithmName;
  
  anaP0DCluster->ValidDimensions       = p0dShower.ValidDimensions;
  
  anaP0DCluster->Cycle                 = p0dShower.Cycle;

  // Add the hits to the cluster
  anaP0DCluster->nHits = 0;
  anaUtils::CreateArray(anaP0DCluster->Hits, p0dShower.Hits.size());
  for (UInt_t i=0;i<p0dShower.Hits.size();i++){
    ND::TP0DReconModule::TP0DHit *p0dHit = (ND::TP0DReconModule::TP0DHit*) (*P0DReconHits)[p0dShower.Hits[i]];
    
    // Must create an ad-hoc hit since the P0DTrack does not contain hits
    AnaP0DHit* p0dHit2 = MakeP0DHit();
    
    // Add the hit to the array of hits in the cluster
    anaP0DCluster->Hits[anaP0DCluster->nHits++] = p0dHit2;
    
    // Fill the hit info from a P0DNode
    FillP0DHitInfo(*p0dHit,p0dHit2);
  }

 anaP0DCluster-> TrueParticle = NULL;
  /*
  //--- Compute the true particle that contributes the most to this recon cluster  ---
  Float_t max_charge=0;
  Int_t  max_index=-1;
  for (UInt_t i = 0; i<p0dCluster.Truth_PrimaryTrajIDs.size();i++){
    if (p0dCluster.Truth_ChargeShare[i] > max_charge){
      max_charge = p0dCluster.Truth_ChargeShare[i];
      max_index = i; 
    }
  }

  // --- Associate the AnaTrueParticleB that contributes the most to this recon particle ---
  anaP0DCluster->TrueParticle=NULL;
  if (max_index>-1){
    std::vector<AnaTrueParticleB*>::iterator it;
    for (it = _spill->TrueParticles.begin(); it!=_spill->TrueParticles.end();it++){
      AnaTrueParticleB* trueTrack = *it;
      if (p0dCluster.Truth_TrajIDs[max_index] == trueTrack->ID){
        anaP0DCluster->TrueParticle = static_cast<AnaTrueParticleB*>(trueTrack);
        break; // Stop when association is found.
      }
    }
  }
  */
}



//*****************************************************************************
void oaAnalysisTreeConverter::FillP0DHitInfo(ND::TP0DReconModule::TP0DHit& p0dHit, AnaP0DHit* anaP0DHit){
//*****************************************************************************

/*
  // Get the 3D position from the GeomID
  static P0DGeometryManager geom;
  TVector3 pos = geom.GeomIdPosition(p0dHit.GeomID);  
  for (Int_t i=0;i<3;i++)
    anaP0DHit->Position[i] = pos[i];

  // Set the time
  anaP0DHit->Position[3] = p0dHit.Time;


  // Set the hit type
  Int_t type = ND::GeomId::P0D::GetBarLayer(p0dHit.GeomID);

  if      (type==0) anaP0DHit->Type = AnaP0DHit::kXHit;
  else if (type==1) anaP0DHit->Type = AnaP0DHit::kYHit;
*/
  
  // Set the hit charge
  anaP0DHit->Charge = p0dHit.Charge;

  anaP0DHit->GeomID = p0dHit.GeomID;
  //  anaP0DHit->ChanID = p0dHit.ChanID;
  anaP0DHit->Time   = p0dHit.Time;  
}

//*****************************************************************************
ND::TP0DReconModule::TP0DParticle* oaAnalysisTreeConverter::GetP0DReconParticleWithUniqueID(UInt_t uniqueID){
//*****************************************************************************

  ND::TP0DReconModule::TP0DParticle *p0dParticle = NULL;
  for (Int_t i=0;i<NP0DReconParticles;i++){
    ND::TP0DReconModule::TP0DParticle *p0dReconParticle = (ND::TP0DReconModule::TP0DParticle*) (*P0DReconParticles)[i];
    if (p0dReconParticle->UniqueID == uniqueID){
      p0dParticle = p0dReconParticle;
      break;
    }
  }

  return p0dParticle;
}


//*****************************************************************************
void oaAnalysisTreeConverter::AddReconVertexToReconParticle(AnaVertexB* vertex, AnaTrack* part){
//*****************************************************************************

  part->ReconVertices.push_back(vertex);
  // choose the vertex more primary
  if ( ! part->ReconVertex || part->ReconVertex->PrimaryIndex > vertex->PrimaryIndex) {
    // also check it is in the same bunch, so it will be the same in the flattree
    if ( vertex->Bunch == part->Bunch) {
      part->ReconVertex = vertex;
        //          part->MomentumAtVertex  = vConst->Momentum.Mag();
        //          anaUtils::VectorToArray((1 / part->MomentumAtVertex) * vConst->Momentum, part->DirectionAtVertex);
    }
  }
}
  
//*****************************************************************************
void oaAnalysisTreeConverter::FillDelayedClustersInfo(AnaSpill& spill){
//*****************************************************************************

  for (std::vector<AnaBunchC*>::iterator it = spill.Bunches.begin(); it != spill.Bunches.end(); ++it) {
    AnaBunch* bunch = static_cast<AnaBunch*> (*it);

    bunch->nDelayedClusters = p0dUtils::GetMichelElectrons(spill, bunch->DelayedClusters, bunch->Bunch);
  }
}

//*****************************************************************************
AnaP0DVertex* oaAnalysisTreeConverter::GetAnaP0DVertex(Int_t index){
//*****************************************************************************

  if (_AnaP0DVertices[index]) return _AnaP0DVertices[index];

  _AnaP0DVertices[index] = MakeP0DVertex();
  return _AnaP0DVertices[index];
}

//*****************************************************************************
AnaP0DCluster* oaAnalysisTreeConverter::GetAnaP0DCluster(Int_t index){
//*****************************************************************************

  if (_AnaP0DClusters[index]) return _AnaP0DClusters[index];

  _AnaP0DClusters[index] = MakeP0DCluster();
  return _AnaP0DClusters[index];
}

//*****************************************************************************
AnaP0DParticle* oaAnalysisTreeConverter::GetAnaP0DParticle(Int_t index){
//*****************************************************************************

  if (_AnaP0DParticles[index]) return _AnaP0DParticles[index];

  _AnaP0DParticles[index] = MakeP0DParticle();
  return _AnaP0DParticles[index];
}

//*****************************************************************************
AnaP0DParticle* oaAnalysisTreeConverter::GetAnaP0DParticleFromTrack(Int_t index){
//*****************************************************************************

  if (_AnaP0DTracks[index]) return _AnaP0DTracks[index];

  _AnaP0DTracks[index] = MakeP0DParticle();
  return _AnaP0DTracks[index];
}

//*****************************************************************************
AnaP0DParticle* oaAnalysisTreeConverter::GetAnaP0DParticleFromShower(Int_t index){
//*****************************************************************************

  if (_AnaP0DShowers[index]) return _AnaP0DShowers[index];

  _AnaP0DShowers[index] = MakeP0DParticle();
  return _AnaP0DShowers[index];
}
