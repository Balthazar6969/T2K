#ifndef p0dExampleAnalysis_h
#define p0dExampleAnalysis_h

#include "baseP0DAnalysis.hxx"
#include "P0DDataClasses.hxx"
#include "ND280AnalysisUtils.hxx"
#include "p0dExampleSelection.hxx"

class p0dExampleAnalysis: public baseP0DAnalysis {
public:
  p0dExampleAnalysis(AnalysisAlgorithm* ana=NULL );
  virtual ~p0dExampleAnalysis(){}

  // Mandatory functions

  bool Initialize();

  void DefineSelections();
  void DefineCorrections();
  void DefineSystematics();
  void DefineConfigurations();
  void DefineMicroTrees(bool addBase=true);
  void DefineTruthTree();

  void FillMicroTrees(bool addBase=true);
  void FillToyVarsInMicroTrees(bool addBase=true);  

  bool CheckFillTruthTree(const AnaTrueVertex& vtx);

  void FillTruthTree(const AnaTrueVertex& vtx);

  //  ---------

  /// Create the appropriate event type from an Spill and a Bunch in that spill
  virtual AnaEventC* MakeEvent(){
    if( _isUsingReconDirP0D){
      // Create special event AnaLocalReconEvent
      return new AnaLocalReconEvent(GetSpill(),*static_cast<const AnaLocalReconBunch*>(&input().GetBunch()));
    }
    else if(_isUsingReconDirP0DNew){
      // Create special AnaP0DEvent
      return new AnaP0DEvent(GetSpill(),GetBunch());
    }
    else{
      // Create an standard event
      return baseP0DAnalysis::MakeEvent();
    }

  }

  /// Returns the ToyBoxP0D
  virtual const ToyBoxP0D& box(Int_t isel=-1) const {return *static_cast<const ToyBoxP0D*>(&boxB(isel));}

  /// The ToyBoxP0D does not have a Vertex method
  virtual AnaVertexB* GetVertex() const{return NULL;}

  /// returns the true vertex from the ToyBoxP0D
  virtual AnaTrueVertexB* GetTrueVertex() const {return box().TrueVertex;}

  void FillMicroTreesNative();
  void FillMicroTreesLocal();
  void FillCategories();

public:

  enum enumStandardMicroTrees_p0dExampleAnalysis{
    vertexFiducial = enumStandardMicroTreesLast_baseP0DAnalysis+1,
    vertexCycle,
    vertexPosition,
    vertexValidDim,
    ClusterInVertexTruePDG,
    ClusterInVertexPosition,    

    HitInVertexPosition,    

    nP0DTracksInGlobal,
    nP0DOnlyTracksInGlobal,
    P0DTracksInGlobalID,
    P0DOnlyTracksInGlobalID,

    nMichel,

    nShowers, 
    Shower1EDeposit, 
    Shower2EDeposit, 
    Shower1Direction, 
    Shower2Direction, 
    
    truevertex_pos,

    nVertices, 
    nParticles,
    nClusters, 

    nParticlesInVertex,  
    nClustersInVertex,   
    nHitsInVertex,   
    
    nTracksInParticle, 
    nShowersInParticle,
    nHitsInParticle,   
    nNodesInParticle,
    ParticlePosition,

    nClustersInParticleInVertex, 
    nTracksInParticleInVertex, 
    nShowersInParticleInVertex, 
    nHitsInParticleInVertex,   
    nNodesInParticleInVertex,    
    nClustersInParticleInVertex2, 

    ParticleInVertexPosition, 
    ParticleInVertexMomentum, 
    ParticleInVertexEDeposit, 
    ParticleInVertexLength, 
    ParticleInVertexID, 
    ParticleInVertexLink,
    ParticleInVertexGlobal,
    ParticleInVertexTrueMomentum, 
    ParticleInVertexTruePDG, 

    ClusterInParticleInVertexTruePDG,
    ClusterInParticleInVertexPosition,    

    HitInParticleInVertexPosition,    
    HitInParticleInVertexCharge,    
    HitInParticleInVertexType,    

    ClusterPosition,
    ClusterTruePDG,
    nHitsInCluster,      

    enumStandardMicroTreesLast_p0dExampleAnalysis
  };

protected:

  bool _isUsingReconDirP0D;  
  bool _isUsingReconDirP0DNew;  
  bool _debug;
};

#endif
