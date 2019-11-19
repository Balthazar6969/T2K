#ifndef DataClassesIO_hxx
#define DataClassesIO_hxx

#include "DataClasses.hxx"
#include "P0DDataClasses.hxx"
#include "OutputManager.hxx"
#include "TreeConverterUtils.hxx"

extern Int_t prefix_index;
extern Int_t counter_index;

const UInt_t NSubDet = 6;


/// A base method for all subdetector-level constituents of global tracks.
/// Provides filling methods for the variables common for all sub-detectors
namespace AnaSubTrackIO{
  void Define(OutputManager& man, const std::string& counter, const Int_t size2, const std::string& prefix);
  void Write(OutputManager& man,const AnaParticleB& seg,  const std::string& prefix, Int_t indx1=-1, Int_t indx2=0);
}

/// Representation of a detector crossing info for a trajectory/track.
class AnaDetCrossingIO: public AnaDetCrossingB{
 public :

  /// 
  AnaDetCrossingIO(){}
  virtual ~AnaDetCrossingIO(){}
  
  /// A constructor from the corresponding base class
  AnaDetCrossingIO(const AnaDetCrossingB& cross): AnaDetCrossingB(cross){}

  virtual void Define(OutputManager& man, const std::string& counter, const std::string& prefix="") const;
  virtual void Write(OutputManager& man, const std::string& prefix="", Int_t indx1=-1, Int_t indx2=0) const;
};

/// Representation of a true Monte Carlo vertex.
class AnaTrueVertexIO: public AnaTrueVertex{
 public :

  AnaTrueVertexIO(){}
  virtual ~AnaTrueVertexIO(){}

  /// A constructor from the corresponding base class
  // Consider copying only the relevant (flat-tree) info (i.e. accum_level not need, nor the particle arrays), 
  // although it should be minor,  and the code is smaller the current way
  AnaTrueVertexIO(const AnaTrueVertex& vertex): AnaTrueVertex(vertex){
    
    // The vectors of "NReconTracks",  "NReconVertices" should be filled with NULL to at least keep the size
    // this is stored in the flat-trees
    ReconParticles = std::vector<AnaParticleB*>(vertex.ReconParticles.size(), NULL);
    ReconVertices  = std::vector<AnaVertexB*>(vertex.ReconVertices.size(),    NULL);
  }   
  virtual void Define(OutputManager& man, const std::string& counter, const std::string& prefix="") const;
  virtual void Write(OutputManager& man, const std::string& prefix="") const;
};

/// Representation of a true Monte Carlo trajectory/particle.
class AnaTrueParticleIO: public AnaTrueParticle{
 public :

  AnaTrueParticleIO(){}
  virtual ~AnaTrueParticleIO(){}
  
  /// A constructor from the corresponding base class
  AnaTrueParticleIO(const AnaTrueParticle&);


  virtual void Define(OutputManager& man, const std::string& counter, const std::string& prefix="") const;
  virtual void Write(OutputManager& man, const Int_t trueVertexIndex, const std::string& prefix="") const;
  
};

/// Representation of a TPC segment of a global particle.
class AnaTPCParticleIO:public AnaTPCParticle{
 public :

  AnaTPCParticleIO(){}
  virtual ~AnaTPCParticleIO(){}

  /// A constructor from the corresponding base class
  AnaTPCParticleIO(const AnaTPCParticle& particle): AnaTPCParticle(particle){}

  virtual void Define(OutputManager& man, const std::string& counter, const std::string& prefix="") const;
  virtual void Write(OutputManager& man, const std::string& prefix="", Int_t indx1=-1, Int_t indx2=0) const;
};

/// Representation of a FGD segment of a global track.
class AnaFGDParticleIO:public AnaFGDParticle{
 public :

  AnaFGDParticleIO(){}
  virtual ~AnaFGDParticleIO(){}

  /// A constructor from the corresponding base class
  AnaFGDParticleIO(const AnaFGDParticle& particle): AnaFGDParticle(particle){}
  
  virtual void Define(OutputManager& man, const std::string& counter, const std::string& prefix="") const;
  virtual void Write(OutputManager& man, const std::string& prefix="", Int_t indx1=-1, Int_t indx2=0) const;
};

/// Representation of an ECAL segment of a global track.
class AnaECALParticleIO:public AnaECALParticle{
 public :

  AnaECALParticleIO(){}
  virtual ~AnaECALParticleIO(){}

  /// A constructor from the corresponding base class
  AnaECALParticleIO(const AnaECALParticle& particle): AnaECALParticle(particle){}
  
  virtual void Define(OutputManager& man, const std::string& counter, const std::string& prefix="") const;
  virtual void Write(OutputManager& man, const std::string& prefix="", Int_t indx1=-1, Int_t indx2=0) const;
};

/// Representation of an SMRD segment of a global track.
class AnaSMRDParticleIO:public AnaSMRDParticle{
 public :

  AnaSMRDParticleIO(){}
  virtual ~AnaSMRDParticleIO(){}

  /// A constructor from the corresponding base class
  AnaSMRDParticleIO(const AnaSMRDParticle& particle): AnaSMRDParticle(particle){}
  
  virtual void Define(OutputManager& man, const std::string& counter, const std::string& prefix="") const;
  virtual void Write(OutputManager& man, const std::string& prefix="", Int_t indx1=-1, Int_t indx2=0) const;
};

/// Representation of a P0D segment of a global track.
class AnaP0DParticleIO:public AnaP0DParticle{
 public :

  AnaP0DParticleIO(){}
  virtual ~AnaP0DParticleIO(){}

  /// A constructor from the corresponding base class
  AnaP0DParticleIO(const AnaP0DParticle& particle): AnaP0DParticle(particle){}
  
  virtual void Define(OutputManager& man, const std::string& counter, const std::string& prefix="") const;
  virtual void Write(OutputManager& man, const std::string& prefix="", Int_t indx1=-1, Int_t indx2=0) const;
};

/// Representation of a Tracker segment of a global track.
class AnaTrackerTrackIO:public AnaTrackerTrack{
 public :

  AnaTrackerTrackIO(){}
  virtual ~AnaTrackerTrackIO(){}

  /// A constructor from the corresponding base class
  AnaTrackerTrackIO(const AnaTrackerTrack& track): AnaTrackerTrack(track){}
  
  virtual void Define(OutputManager& man, const std::string& counter, const std::string& prefix="") const;
  virtual void Write(OutputManager& man, const std::string& prefix="", Int_t indx1=-1, Int_t indx2=0) const;
};


/// Representation of a global track.
class AnaTrackIO: public AnaTrack{
 public :

  AnaTrackIO(){}
  virtual ~AnaTrackIO(){}
  
  /// A constructor from the corresponding base clasAnaTrackIO(const AnaTrack& track): AnaTrack(track){}
  AnaTrackIO(const AnaTrack& track);
  
  virtual void Define(OutputManager& man, const std::string& counter, const std::string& prefix="") const;
  virtual void Write(OutputManager& man, const std::string& prefix="") const;
};

/// Representation of a global vertex.
class AnaVertexIO: public AnaVertex{
 public :

  AnaVertexIO(){}
  virtual ~AnaVertexIO(){}

  /// A constructor from the corresponding base class
  AnaVertexIO(const AnaVertex& vertex): AnaVertex(vertex){}
  
  virtual void Define(OutputManager& man, const std::string& counter, const std::string& prefix="") const;
  virtual void Write(OutputManager& man, const std::vector<int> trueVertexIndices, const std::string& prefix="") const;
};

/// Representation of the beam information, including POT and quality.
class AnaBeamIO: public AnaBeam{
 public :

  AnaBeamIO(){}
  virtual ~AnaBeamIO(){}
  
  /// A constructor from the corresponding base class
  AnaBeamIO(const AnaBeam& beam): AnaBeam(beam){}

  virtual void Define(OutputManager& man,const std::string& prefix="") const;
  virtual void Write(OutputManager& man,const std::string& prefix="") const;
};

/// Representation of the ND280 data quality flags.
class AnaDataQualityIO: public AnaDataQualityB {
  public:

  AnaDataQualityIO(){}
    virtual ~AnaDataQualityIO() {}
  
  /// A constructor from the corresponding base class
  AnaDataQualityIO(const AnaDataQualityB& dq): AnaDataQualityB(dq){}
  
  virtual void Define(OutputManager& man,const std::string& prefix="") const;
  virtual void Write(OutputManager& man,const std::string& prefix="") const;
};



/// An AnaBunch contains all the reconstructed objects in a given time window.
/// The normal bunches are indexed from 0-7.
///
/// The bunch windows for each run period are defined in the text file
/// $HIGHLEVELANALYSISIOROOT/data/BunchPosition.dat, where the run period of -1
/// means Monte Carlo.
///
/// Each bunch has a weighting associated with it, to allow for flux
/// re-weighting.
class AnaBunchIO: public AnaBunch{
 public :

  AnaBunchIO(){}
  virtual ~AnaBunchIO(){}
  
  /// A constructor from the corresponding base class
  AnaBunchIO(const AnaBunch& bunch);
  
  virtual void Define(OutputManager& man,const std::string& prefix="") const;
  virtual void Write(OutputManager& man,const std::string& prefix="") const;
};

class AnaFgdTimeBinIO: public AnaFgdTimeBin{
 public :

  AnaFgdTimeBinIO(){}
  virtual ~AnaFgdTimeBinIO(){}
  
  /// A constructor from the corresponding base class
  AnaFgdTimeBinIO(const AnaFgdTimeBin& bin) : AnaFgdTimeBin(bin){}
  
  virtual void Define(OutputManager& man, const std::string& counter, const std::string& prefix="") const;
  virtual void Write(OutputManager& man,const std::string& prefix="") const;
};

class AnaEventInfoIO: public AnaEventInfo{
 public :

  AnaEventInfoIO(){}
  virtual ~AnaEventInfoIO(){}

  /// A constructor from the corresponding base class
  AnaEventInfoIO(const AnaEventInfo& info): AnaEventInfo(info){}
  
  virtual void Define(OutputManager& man,const std::string& prefix="") const;
  virtual void Write(OutputManager& man,const std::string& prefix="") const;
};


/// Representation of the ND280 trigger bits
class AnaTriggerIO: public AnaTrigger {
public:

  AnaTriggerIO(){}
  virtual ~AnaTriggerIO() {}

  /// A constructor from the corresponding base class
  AnaTriggerIO(const AnaTrigger& trigger): AnaTrigger(trigger){}
 
  /// Add the variables for representing an AnaTriggerIO object to the OutputManager.
  virtual void Define(OutputManager& man, const std::string& prefix="") const;

  /// Fill the current state of the AnaTriggerIO object into the OutputManager.
  virtual void Write(OutputManager& man, const std::string& prefix="") const;
};


/// The top-level object for representing information from the input files.
/// A single AnaSpillB contains information from an entire ND280 event.
/// The reconstructed objects are separated into bunches based on their time
/// information.
///
/// There are multiple copies of each spill available through the InputManager
/// [ND::input()], showing the raw state of the spill after reading it in from
/// the input files; after applying corrections; and after applying corrections
/// and systematics.
class AnaSpillIO: public AnaSpill{
 public :

  AnaSpillIO(){
    TriggerIO = NULL;
  }
  virtual ~AnaSpillIO(){
    if (TriggerIO)
      delete TriggerIO;
    
    TriggerIO = NULL;
  }
  
  /// A constructor from the corresponding base class
  AnaSpillIO(const AnaSpill& spill);

  virtual void Define(OutputManager& man,const std::string& prefix="") const;
  virtual void Write(OutputManager& man,const std::string& prefix="") const;
  
  AnaTriggerIO* TriggerIO;
  
};

class AnaEventIO: public AnaEvent{
 public :

  AnaEventIO(){}
  virtual ~AnaEventIO(){}

  /// A constructor from the corresponding base class
  AnaEventIO(const AnaEvent& event): AnaEvent(event){}
  
  virtual void Define(OutputManager& man,const std::string& prefix="") const;
  virtual void Write(OutputManager& man,const std::string& prefix="") const;
};


/// Representation of detector time info
class AnaTimeNodeIO:public AnaTimeNode{
public :

  AnaTimeNodeIO(){}
  virtual ~AnaTimeNodeIO(){}
  
  /// A constructor from the corresponding base class
  AnaTimeNodeIO(const AnaTimeNode& node): AnaTimeNode(node){}

  /// Add the variables for representing an AnaDetTimeInfo to the OutputManager.
  virtual void Define(OutputManager& man, const std::string& counter, const std::string& prefix="") const;

  /// Fill the current state of the AnaDetTimeInfo into the OutputManager.
  virtual void Write(OutputManager& man, const std::string& prefix="", Int_t indx1=-1, Int_t indx2=0) const;
};


class AnaTECALReconObjectIO: public AnaTECALReconObject{

public :

  AnaTECALReconObjectIO(){}
  virtual ~AnaTECALReconObjectIO(){}
  
  /// A constructor from the corresponding base class
  AnaTECALReconObjectIO(const AnaTECALReconObject& object): AnaTECALReconObject(object){}

  virtual void Define(OutputManager& man, const std::string& counter, const std::string& prefix) const;
  virtual void Write (OutputManager& man, const std::string& prefix                            ) const;

};


class AnaTECALUnmatchedObjectIO: public AnaTECALUnmatchedObject{

public :

  AnaTECALUnmatchedObjectIO(){}
  virtual ~AnaTECALUnmatchedObjectIO(){}

  /// A constructor from the corresponding base class
  AnaTECALUnmatchedObjectIO(const AnaTECALUnmatchedObject& object): AnaTECALUnmatchedObject(object){}
  
  virtual void Define(OutputManager& man, const std::string& counter, const std::string& prefix) const;
  virtual void Write (OutputManager& man, const std::string& prefix                            ) const;
};

class AnaP0DReconObjectIO: public AnaP0DReconObject{

public :

  AnaP0DReconObjectIO():AnaP0DReconObject(){}
  virtual ~AnaP0DReconObjectIO(){}

  /// A constructor from the corresponding base class
  AnaP0DReconObjectIO(const AnaP0DReconObject& object): AnaP0DReconObject(object){}

  virtual void Define(OutputManager& man, const std::string& counter, const std::string& prefix) const;
  virtual void Write (OutputManager& man, const std::string& prefix                            ) const;

};



class AnaP0DReconVertexIO: public AnaP0DReconVertex{

public :

  AnaP0DReconVertexIO(){}
  virtual ~AnaP0DReconVertexIO(){}
  
  /// A constructor from the corresponding base class
  AnaP0DReconVertexIO(const AnaP0DReconVertex& vertex): AnaP0DReconVertex(vertex){}
  
  virtual void Define(OutputManager& man, const std::string& counter, const std::string& prefix) const;
  virtual void Write (OutputManager& man, const std::string& prefix                            ) const;

};

class AnaP0DReconParticleIO: public AnaP0DReconParticle{

public :

  AnaP0DReconParticleIO(){}
  virtual ~AnaP0DReconParticleIO(){}

  /// A constructor from the corresponding base class
  AnaP0DReconParticleIO(const AnaP0DReconParticle& particle): AnaP0DReconParticle(particle){}

  virtual void Define(OutputManager& man, const std::string& counter, const std::string& prefix) const;
  virtual void Write (OutputManager& man, const std::string& prefix                            ) const;

};

class AnaP0DReconClusterIO: public AnaP0DReconCluster{

public :

  AnaP0DReconClusterIO(){}
  virtual ~AnaP0DReconClusterIO(){}

  /// A constructor from the corresponding base class
  AnaP0DReconClusterIO(const AnaP0DReconCluster& cluster): AnaP0DReconCluster(cluster){}

  virtual void Define(OutputManager& man, const std::string& counter, const std::string& prefix) const;
  virtual void Write (OutputManager& man, const std::string& prefix                            ) const;
};


class AnaLocalReconBunchIO: public AnaLocalReconBunch{
public :

  AnaLocalReconBunchIO():AnaLocalReconBunch(){}
  virtual ~AnaLocalReconBunchIO(){}

  /// A constructor from the corresponding base class
  AnaLocalReconBunchIO(const AnaLocalReconBunch& bunch): AnaLocalReconBunch(bunch){}

  /// Add the variables for representing an AnaBunch to the OutputManager.
  virtual void Define(OutputManager& man,const std::string& prefix="") const;

  /// Fill the current state of the AnaBunch into the OutputManager.
  virtual void Write(OutputManager& man, const std::string& prefix="") const;

};



// Note! should keep unique indexes!
class TreeVars{
  
public:
   
  enum enumVar{
    // sub-detector related variables
    // have in total five sub-detectors: TPC, FGD,  ECal,  SMRD and P0D 
    // so to have unique indexes then define offsets
    subDetector=0, 
    subNNodes,
    subDirectionStart,
    subPositionStart,
    subPositionEnd, 
    subDirectionEnd, 
    subLength, 
    subNHits,
    subUniqueID, 
   
    // TPC
    tpcCharge = (subUniqueID + 1) * NSubDet,    
    tpcMomentum,  
    tpcMomentumError,
    tpcRefitMomentum,
    tpcEFieldRefitMomentum,
    tpcdEdxMeas,
    tpcdEdxExpMu,
    tpcdEdxExpEle,
    tpcdEdxExpP,  
    tpcdEdxExpPi, 
    tpcRawdEdxMeas,
    tpcRawdEdxExpMu,
    tpcRawdEdxExpEle,
    tpcRawdEdxExpP,  
    tpcRawdEdxExpPi, 
    tpcdEdxSigmaMu,  
    tpcdEdxSigmaEle, 
    tpcdEdxSigmaP,
    tpcdEdxSigmaPi,  
    tpcTrueTrackID,    
    // FGD
    fgdX, 
    fgdPullmu,        
    fgdPullp,         
    fgdPullpi,        
    fgdPullno,        
    fgdContainment,   
    
    // ECal
    ecalTrShVal, 
    ecalPIDMipEm,
    ecalEMEnergy,     
    ecalEDeposit,     
    ecalIsShowerLike, 
    ecalAvgTime,
    ecalShowerPosition,      
    
    // SMRD
    smrdAvgTime,
    smrdEDeposit, 
    
    // P0D
    // TRACKER
    trackerCharge, 
    trackerMomentum, 
    trackerMomentumEnd, 
    
    // globalPID (AnaTrackB)
    globalUniqueID,       
    globalDetUsed,        
    globalNTPCs,          
    globalNFGDs,          
    globalNECALs,         
    globalNSMRDs,         
    globalNP0Ds,          
    globalNHits,
    globalCharge, 
    globalMomentum,
    globalDirectionStart, 
    globalPositionStart, 
    globalPositionEnd, 
    globalDirectionEnd, 
    globalTrueTrackID,
    globalRangeMomentumMuon,
    globalMomentumFlip,
    globalDirectionStartFlip,
    globalToFFGD1_FGD2,
    globalToFP0D_FGD1,
    globalToFDSECal_FGD1,
    globalToFECal_FGD1,
    globalToFDSECal_FGD2, 
    globalToFECal_FGD2,
    globalToFFlag_FGD1_FGD2,
    globalToFFlag_P0D_FGD1,
    globalToFFlag_DSECal_FGD1,
    globalToFFlag_ECal_FGD1,
    globalToFFlag_DSECal_FGD2,
    globalToFFlag_ECal_FGD2,
    
    // AnaDetCrossingB 
    detCrossInActive,         
    detCrossDetector,       
    detCrossEntrancePosition, 
    detCrossExitPosition,
    detCrossEntranceMomentum, 
    detCrossExitMomentum, 
    
    // AnaTrueParticleB
    trueTrackID,         
    trueTrackPDG,        
    trueTrackParentPDG,  
    trueTrackGParentPDG, 
    trueTrackParentID,  
    trueTrackGParentID,   
    trueTrackMomentum, 
    trueTrackCharge, 
    trueTrackPosition,
    trueTrackPositionEnd,
    trueTrackDirection, 
    trueTrackTruthVertexID,
    trueTrackNDetCrossings,
    
    // AnaTrueVertexB
    trueVertexID,   
    trueVertexNuPDG,      
    trueVertexNuEnergy,
    trueVertexBunch, 
    trueVertexPosition, 
    trueVertexNTrueTracks,
    trueVertexDetector,
    
    // AnaBunchB
    bunchBunch,     
    bunchWeight,     
    
    // AnaFgdTimeBin
    fgdBinMinTime,       
    fgdBinNHits1,        
    fgdBinNHits2,        
    fgdBinRawChargeSum1, 
    fgdBinRawChargeSum2,
    
    // AnaSpillB
    spillNBunches,  
    spillNTotalTrueVertices, 
    spillNTotalTrueTracks, 

    spillInputFileIndex,
    spillRooVtxEntry,
    
    // AnaEventInfoB
    eventInfoIsMC, 
    eventInfoIsSand, 
    eventInfoRun, 
    eventInfoSubRun, 
    eventInfoEvt,       
    
    // AnaDataQuality
    dqGoodDaq,       
    
    // AnaBeam 
    beamGoodSpill,    
    
    // AnaVertexB
    vertexPosition, 
    vertexPrimaryIndex,      
    vertexNReconTracks,      
    vertexNTrueVerticesMatch,
    vertexTracksUniqueID,    
    vertexTrueVertexIndex,   
    vertexTrueVerticesClean, 
    vertexTrueVerticesCompl, 
    
    // AnaEvent
    eventBunch,
    
    // All sub segments

    
    // TPC
    tpcBackMomentum,
    tpcRefitCharge,
    tpcRefitPosition,
    tpcRefitDirection,
    tpcPullmu,
    tpcPullele,
    tpcPullp,
    tpcPullpi,
    tpcPullk,
    tpcdEdxExpK,
    tpcdEdxSigmaK,
    tpcRawdEdxExpK,
    tpcPurity,
    
    // FGD
    fgdE,
    fgdAvgTime,
#if VERSION_HAS_FGD_VERTEX_AND_END_ACTIVITY
    fgdHasFgdVA,
    fgdHasFgdVA_fmode,
    fgdTypeVA,
#endif
    fgdVertex1x1,
    fgdVertex3x3,
    fgdVertex5x5,
    fgdVertex7x7,
    fgdVertexLayer,
#if VERSION_HAS_FGD_VERTEX_AND_END_ACTIVITY
    fgdHasFgdEA,
    fgdHasFgdEA_fmode,
    fgdTypeEA,
    fgdEnd0x0,
    fgdEnd1x1,
    fgdEnd3x3,
    fgdEnd5x5,
    fgdEnd7x7,
#endif
    
    // ECAL
    ecalPIDMipPion,
    ecalPIDEmHip,
    ecalContainment,
    ecalMostUpStreamLayerHit,

    //SMRD
    
    // P0D
    p0dAvgTime,
    
    // Tracker
    
    // globalPID (AnaTrack)
    globalStatus,
    globalDetectors,
    globalNNodes,
    globalMomentumError,
    globalNDOF,
    globalChi2,
    globalReconPDG, 
    globalBunch,
    globalMomentumMuon,
    globalMomentumErrorMuon,
    globalMomentumProton,
    globalMomentumErrorProton,
    globalMomentumEle,
    globalMomentumErrorEle,
#if VERSION_HAS_REVERSED_REFITS
    globalPositionStartFlip,
    globalDirectionEndFlip,
#endif

#if VERSION_HAS_PRANGE_ESTIMATES
    globalRangeMomentumEle,
    globalRangeMomentumProton,
    globalRangeMomentumMuonFlip,
    globalRangeMomentumProtonFlip,
#endif
    globalNReconVertices,
    globalMomentumAtVertex,
    globalDirectionAtVertex,
    globalLength, 
    globalNTRACKERs,
    
    // AnaTimeNode
    globalNTimeNodes,
    globalTimeNodeDetector,
    globalTimeNodeTimeStart,
    globalTimeNodeTimeEnd,
    
    // AnaBunch
    // AnaFgdTimeBin
    fgdBinMaxTime,
    fgdBinG4ID,
    
    // AnaSpill
    // AnaEventInfo
    eventInfoEventTime,
    
    // AnaBeam
    beamPOT,
    beamPOTCT4,
    beamSpill,
    beamSpillNumber,
    beamBeamRunNumber,
    
    // AnaDataQuality
    dqND280Flag,
    dqDetFlag,
    
    // AnaTrigger
    triggerFGDCosmic,
    triggerTripTCosmic,
    triggerCTMTriggerPattern, 
    
    // AnaTrueVertex
    trueVertexRooVtxIndex,
    trueVertexRooVtxEntry,
    trueVertexReacCode,
    trueVertexTargetPDG,
    trueVertexNuDir,
    trueVertexNuParentPDG,
    trueVertexNuParentDecPoint,
    trueVertexNBaryons,
    trueVertexLeptonPDG,
    trueVertexQ2,
    trueVertexTargetDir,
    trueVertexLeptonDir,
    trueVertexProtonDir,
    trueVertexPionDir,
    trueVertexTargetMom,
    trueVertexLeptonMom,
    trueVertexProtonMom,
    trueVertexPionMom,
    trueVertexNReconTracks,
    trueVertexNReconVertices,
    trueVertexIsPauliBlocked,
    trueVertexIsCohOnH,
    trueVertexNPrimaryParticles,    

    // AnaTrueParticle
    trueTrackPrimaryID,
    trueTrackPurity,
    trueTrackBunch,
    trueTrackNReconTracks,
    trueTrackVertexIndex,
    trueTrackIsTruePrimaryPi0DecayPhoton,
    trueTrackIsTruePrimaryPi0DecayPhotonChild,
    
    
    // AnaVertex
    vertexVariance,
    vertexBunch,
    vertexChi2,
    vertexNDOF,

    // ReconDir/TrackerECal (AnaTCEALReconObject)
    tecalReconAverageHitTime,
    tecalReconAverageZPos,
    tecalReconContainment,
    tecalReconEFitResult,
    tecalReconEFitUncertainty,
    tecalReconFirstLayer,
    tecalReconIsShowerLike,
    tecalReconIsTrackLike,
    tecalReconLastLayer,
    tecalReconLikeMIPEM,
    tecalReconLikeMIPPion,
    tecalReconLikeEMHIP,
    tecalReconLikeMIPEMLow,
    tecalReconMatchingLike,
    tecalReconMichelTagNDelayedCluster,
    tecalReconModule,
    tecalReconMostDownStreamLayerHit,
    tecalReconMostUpStreamLayerHit,
    tecalReconNHits,
    tecalReconNLayersHit,
    tecalReconObjectLength,
    tecalReconPIDAMR,
    tecalReconPIDAngle,
    tecalReconPIDAsymmetry,
    tecalReconPIDCircularity,
    tecalReconPIDFBR,
    tecalReconPIDMaxRatio,
    tecalReconPIDMeanPos,
    tecalReconPIDShowerAngle,
    tecalReconPIDShowerWidth,
    tecalReconPIDTransverseChargeRatio,
    tecalReconPIDTruncatedMaxRatio,
    tecalReconPointing,
    tecalReconThrust,
    tecalReconThrustAxis,
    tecalReconThrustOrigin,
    tecalReconTimeBunch,
    tecalReconTotalHitCharge,
    tecalReconTrueID,
    tecalReconTrueIDPrimary,
    tecalReconTrueIDRecursive,
    tecalReconTrueIDSingle,
    tecalReconUniqueID,
    tecalReconBunch,

    // ReconDir/TrackerECal (AnaTECALUnmatchedObject)
    tecalUnmatchedAverageHitTime,
    tecalUnmatchedBackPos,
    tecalUnmatchedFrontPos,
    tecalUnmatchedMostDownStreamLayerHit,
    tecalUnmatchedMostUpStreamLayerHit,
    tecalUnmatchedNHits,
    tecalUnmatchedTotalHitCharge,
    tecalUnmatchedTrueID,
    tecalUnmatchedTrueIDPrimary,
    tecalUnmatchedTrueIDRecursive,
    tecalUnmatchedTrueIDSingle,
    tecalUnmatchedView,
    tecalUnmatchedBunch,

    // ReconDir/P0D (AnaP0DReconVertex)
    p0dVertexAlgorithmName,
    p0dVertexCycle,
    p0dVertexNHits,
    p0dVertexUniqueID,
    p0dVertexBunch,
    p0dVertexVertices,
    p0dVertexParticles,
    p0dVertexTracks,
    p0dVertexShowers,
    p0dVertexClusters,
    p0dVertexNodes,
    p0dVertexHits,
    p0dVertexStatus,               
    p0dVertexQuality,              
    p0dVertexNDOF,                 
    p0dVertexTruth_PrimaryTrajIDs, 
    p0dVertexTruth_TrajIDs,        
    p0dVertexTruth_HitCount,       
    p0dVertexTruth_ChargeShare,    
    p0dVertexPosition,             
    p0dVertexPosVariance,          
    p0dVertexValidDimensions,      
    p0dVertexFiducial,             

    // ReconDir/P0D (AnaP0DReconVertex)
    p0dParticleAlgorithmName,
    p0dParticleCycle,
    p0dParticleNHits,
    p0dParticleUniqueID,
    p0dParticleBunch,
    p0dParticleVertices,
    p0dParticleParticles,
    p0dParticleTracks,
    p0dParticleShowers,
    p0dParticleClusters,
    p0dParticleNodes,
    p0dParticleHits,
    p0dParticleStatus,
    p0dParticleQuality,
    p0dParticleNDOF,
    p0dParticleTruth_PrimaryTrajIDs,
    p0dParticleTruth_TrajIDs,
    p0dParticleTruth_HitCount,
    p0dParticleTruth_ChargeShare,
    p0dParticleEDeposit,  // Only in showers
    p0dParticleSideDeposit,
    p0dParticleEndDeposit,
    p0dParticlePosition,
    p0dParticlePosVariance,
    p0dParticleValidDimensions,
    p0dParticleDirection,
    p0dParticleDirVariance,
    p0dParticleMomentum,
    p0dParticleCharge,
    p0dParticleLength,      // Only in Tracks

    // ReconDir/P0D (AnaP0DReconCluster)
    p0dClusterAlgorithmName,
    p0dClusterCycle,
    p0dClusterNHits,
    p0dClusterUniqueID,
    p0dClusterBunch,
    p0dClusterVertices,
    p0dClusterParticles,
    p0dClusterTracks,
    p0dClusterShowers,
    p0dClusterClusters,
    p0dClusterNodes,
    p0dClusterHits,
    p0dClusterTruth_PrimaryTrajIDs,
    p0dClusterTruth_TrajIDs,
    p0dClusterTruth_HitCount,
    p0dClusterTruth_ChargeShare,
    p0dClusterNFiducialHits,
    p0dClusterEDeposit,
    p0dClusterPosition,
    p0dClusterPosVariance,
    p0dClusterValidDimensions,
    //p0dClusterMoments,  
    
    
    TreeVarsLast
  };

  
  //offsets
  static unsigned int offsetTPC;
  static unsigned int offsetFGD;
  static unsigned int offsetECAL;
  static unsigned int offsetSMRD;
  static unsigned int offsetP0D;
  static unsigned int offsetTRACKER;

};

#endif
