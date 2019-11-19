/*
   converter for oaAnalysis input format

   A. Cervera September 2012
*/


#ifndef oaAnalysisTreeConverter_h
#define oaAnalysisTreeConverter_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TClonesArray.h>
#include <TRefArray.h>
#include <TRef.h>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <set>
#include "InputConverter.hxx"
#include "DataClasses.hxx"
#include "TTruthUtils.hxx"
#include "oaAnalysisReaderProjectHeaders.h"
#include "TECALPidLikelihood.hxx"
#include "GeometryManager.hxx"
#include "ND280AnalysisUtils.hxx"
#include <ND280BeamBunching.hxx>

// ---- TEST HIGHLAND2-P0D ----
#include <P0DDataClasses.hxx>

const UInt_t NMAXP0DALGORES=50;
// ----------------------------

const double _trueBunchShift = 2690.;

class oaAnalysisTreeConverter: public InputConverter{

 public:

  oaAnalysisTreeConverter();
  virtual ~oaAnalysisTreeConverter();

  virtual bool Initialize();
  virtual void SetCosmicMode(const bool cosmic) {cosmic_mode = cosmic;}
  virtual Int_t GetSpill(Long64_t& entry, AnaSpillC*& spill);
  Int_t GetEvent(Long64_t& entry, AnaEventC*& event){(void)entry;(void)event; return 0;}

  /// Record the POT for the current spill, based on information in the AnaBeam
  /// member of the current AnaSpill.
  void IncrementPOTBySpill();

  virtual Int_t ReadEntries(Long64_t& entry);
  virtual bool AddFileToTChain(const std::string& inputString);

  //---------------------------

  //----------------
  virtual AnaSpillB* MakeSpill() { return new AnaSpill(); }
  virtual AnaBunchB* MakeBunch() { return new AnaBunch(); }
  virtual AnaBeamB* MakeBeam() { return new AnaBeam(); }
  virtual AnaDataQualityB* MakeDataQuality() { return new AnaDataQuality(); }
  virtual AnaEventInfoB* MakeEventInfo() { return new AnaEventInfo(); }
  virtual AnaTrigger* MakeTrigger() { return new AnaTrigger(); }
  virtual AnaVertexB* MakeVertex() { return new AnaVertex(); }
  virtual AnaTrueParticleB* MakeTrueParticle() { return new AnaTrueParticle(); }
  virtual AnaTrueVertexB* MakeTrueVertex() { return new AnaTrueVertex(); }
  virtual AnaTrackB* MakeTrack() { return new AnaTrack(); }
  virtual AnaTPCParticleB* MakeTpcTrack() { return new AnaTPCParticle(); }
  virtual AnaTrackerTrackB* MakeTrackerTrack() { return new AnaTrackerTrack(); }
  virtual AnaFGDParticleB* MakeFgdTrack() { return new AnaFGDParticle(); }
  virtual AnaP0DParticleB* MakeP0dTrack() { return new AnaP0DParticle(); }
  virtual AnaECALParticleB* MakeEcalTrack() { return new AnaECALParticle(); }
  virtual AnaSMRDParticleB* MakeSmrdTrack() { return new AnaSMRDParticle(); }
  virtual AnaFgdTimeBinB* MakeFgdTimeBin() { return new AnaFgdTimeBin(); }
  virtual AnaTECALReconObject* MakeTECALReconObject() { return new AnaTECALReconObject(); }
  virtual AnaTECALUnmatchedObject* MakeTECALUnmatchedObject() { return new AnaTECALUnmatchedObject(); }
  virtual AnaP0DReconVertex* MakeP0DReconVertex() { return new AnaP0DReconVertex(); }
  virtual AnaP0DReconParticle* MakeP0DReconParticle() { return new AnaP0DReconParticle(); }
  virtual AnaP0DReconCluster* MakeP0DReconCluster() { return new AnaP0DReconCluster(); }
  virtual AnaBunch* MakeLocalReconBunch() { return new AnaLocalReconBunch(); }


  // ---- TEST HIGHLAND2-P0D ----
  virtual void GetBunchLocalP0DObjects();
  virtual void GetBunchLocalP0DObjectsInAlgoResult(Int_t iAlgoRes);

  virtual void FillP0DBunch(    AnaBunchB* bunch, int ibunch,  Int_t iAlgoRes);
  virtual void FillP0DVertices(            const std::vector<Int_t>& indices, AnaP0DBunch* bunch);
  virtual void FillP0DParticles(           const std::vector<Int_t>& indices, AnaP0DBunch* bunch);
  virtual void FillP0DParticlesFromTracks( const std::vector<Int_t>& indices, AnaP0DBunch* bunch);
  virtual void FillP0DParticlesFromShowers(const std::vector<Int_t>& indices, AnaP0DBunch* bunch);
  virtual void FillP0DClusters(            const std::vector<Int_t>& indices, AnaP0DBunch* bunch);

  ND::TP0DReconModule::TP0DParticle* GetP0DReconParticleWithUniqueID(UInt_t uniqueID);
  void AddReconVertexToReconParticle(AnaVertexB* vertex, AnaTrack* part);
  void FillDelayedClustersInfo(AnaSpill& spill);

  AnaP0DVertex*   GetAnaP0DVertex(Int_t index);
  AnaP0DCluster*  GetAnaP0DCluster(Int_t index);
  AnaP0DParticle* GetAnaP0DParticle(Int_t index);
  AnaP0DParticle* GetAnaP0DParticleFromTrack(Int_t index);
  AnaP0DParticle* GetAnaP0DParticleFromShower(Int_t index);

  virtual AnaP0DVertex*   MakeP0DVertex()   { return new AnaP0DVertex();   }
  virtual AnaP0DParticle* MakeP0DParticle() { return new AnaP0DParticle(); }
  virtual AnaP0DCluster*  MakeP0DCluster()  { return new AnaP0DCluster();  }
  virtual AnaP0DHit*      MakeP0DHit()      { return new AnaP0DHit();      }
  virtual AnaP0DBunch*    MakeP0DBunch() { return new AnaP0DBunch(); }
  // ----------------------------

  virtual void FillInfo(AnaSpill* spill);
  virtual void FillBunchInfo(AnaSpill* spill);
  virtual void FillPIDs(AnaBunch* bunch, int ibunch);
  virtual void FillTECALReconObjects(AnaBunch* bunch, int ibunch);
  virtual void FillTECALReconUnmatchedObjects(AnaBunch* bunch, int ibunch);
  virtual void FillP0DReconVertices(AnaBunch* bunch, int ibunch);
  virtual void FillP0DReconParticles(AnaBunch* bunch, int ibunch);
  virtual void FillP0DReconClusters(AnaBunch* bunch, int ibunch);
  virtual void FillGlobalVertices(AnaSpill* spill);
  virtual void DeleteBadObjects(AnaSpill* spill);
  virtual void DeleteTrueParticle(AnaSpill* spill, AnaTrueParticleB* trackToDelete);
  virtual void DeleteRecoParticle(AnaSpill* spill, AnaParticleB* particleToDelete);
  virtual void DeleteRecoVertex(AnaSpill* spill, AnaVertexB* vertexToDelete);
  virtual void FillTrackInfo(ND::TGlobalReconModule::TGlobalPID& PID, AnaTrack* track);
  virtual void FillVertexInfo(ND::TGlobalReconModule::TGlobalVertex& global_vertex, AnaVertex* vertex, AnaBunch* bunch, AnaSpill* spill);
  virtual AnaTrack* FindTrack(const int UniqueID, AnaBunch* bunch, AnaSpill* spill);
  virtual void FillTrueVerticesMatchInfo(ND::TTrueVertex* matchedTrueVertex, AnaVertex* vertex, AnaBunch* bunch);
  virtual AnaRecTrueMatch* FillCompletenessCleanliness(AnaVertex* vertex, AnaBunch* bunch, AnaTrueVertexB* truevertex);
  virtual void FillTrueParticleRecoInfo(ND::TTrueParticle& trueParticle, AnaTrueObjectC*& trueObj, bool setpurity);
  virtual void FillTrueParticleInfo(ND::TTruthTrajectoriesModule::TTruthTrajectory* truthTraj, AnaTrueParticle* truePart, AnaSpill* spill);

  virtual void FillTrackHits(ND::TGlobalReconModule::TGlobalPID& PID, AnaTrack* track);

  virtual void FillTpcInfo(ND::TGlobalReconModule::TGlobalPID& PID, AnaTrack* track);
  virtual void FillTrackerInfo(ND::TGlobalReconModule::TGlobalPID& PID, AnaTrack* track);
  virtual void FillFgdInfo(ND::TGlobalReconModule::TGlobalPID& PID, AnaTrack* track);
  virtual void FillEcalInfo(ND::TGlobalReconModule::TGlobalPID& PID, AnaTrack* track);
  virtual void FillSmrdInfo(ND::TGlobalReconModule::TGlobalPID& PID, AnaTrack* track);
  virtual void FillP0dInfo(ND::TGlobalReconModule::TGlobalPID& PID, AnaTrack* track);

  virtual void FillTpcTrackInfo( ND::TGlobalReconModule::TTPCObject&  tpcTrack,  AnaTPCParticle* seg);
  virtual void FillTrackerTrackInfo( ND::TGlobalReconModule::TTrackerObject&  trTrack,  AnaTrackerTrack* seg);
  virtual void FillFgdTrackInfo( ND::TGlobalReconModule::TFGDObject&  fgdTrack,  AnaFGDParticle* seg);
  virtual void FillEcalTrackInfo(ND::TGlobalReconModule::TECALObject& ecalTrack, AnaECALParticle* seg);
  virtual void FillSmrdTrackInfo(ND::TGlobalReconModule::TSMRDObject& smrdTrack, AnaSMRDParticle* seg);
  virtual void FillP0dTrackInfo( ND::TGlobalReconModule::TP0DObject&  p0dTrack,  AnaP0DParticle* seg);
  virtual void FillSubdetectorTrackInfo(ND::TSubBaseObject& subTrack, AnaParticleB* seg);

  virtual void FillTrueInfo(AnaSpill* spill);
  virtual bool FillTrueVertexInfo(ND::TTruthVerticesModule::TTruthVertex* true_vertex, AnaTrueVertex* vertex, int v, AnaSpill* spill);
  virtual void FillFgdTimeBinInfo(std::vector<AnaFgdTimeBinB*>* AnaFgdTimeBins);

  virtual void FillBeamInfo(bool isMC, AnaBeam* beam);
  virtual void FillTriggerInfo(AnaTrigger* trigger);
  virtual void FillDQInfo(AnaDataQuality* dq);

  virtual void FillTECALReconObjectInfo(ND::TTrackerECALReconModule::TECALReconObject& tecalReconObject, AnaTECALReconObject* anaTECALReconObject, AnaLocalReconBunch* anaLocalBunch);
  virtual void FillTECALUnmatchedObjectInfo(ND::TTrackerECALReconModule::TECALReconUnmatchedObject& tecalUnmatchedObject, AnaTECALUnmatchedObject* anaTECALUnmatchedObject, AnaLocalReconBunch* anaLocalBunch);

  // --- Methods to create local P0DRecon objects -------
  virtual void FillP0DReconVertexInfo(ND::TP0DReconModule::TP0DVertex& p0dVertex, AnaP0DReconVertex* anaP0DReconVertex, AnaLocalReconBunch* anaLocalBunch);
  virtual void FillP0DReconParticleInfo(ND::TP0DReconModule::TP0DParticle& p0dParticle, AnaP0DReconParticle* anaP0DReconParticle, AnaLocalReconBunch* anaLocalBunch);
  virtual void FillP0DReconClusterInfo(ND::TP0DReconModule::TP0DCluster& p0dCluster, AnaP0DReconCluster* anaP0DReconCluster, AnaLocalReconBunch* anaLocalBunch);

  virtual void FillP0DReconVertexLinks(  ND::TP0DReconModule::TP0DVertex&   p0dVertex,   AnaP0DReconVertex*   anaP0DReconVertex);
  virtual void FillP0DReconParticleLinks(ND::TP0DReconModule::TP0DParticle& p0dParticle, AnaP0DReconParticle* anaP0DReconParticle);
  virtual void FillP0DReconClusterLinks( ND::TP0DReconModule::TP0DCluster&  p0dCluster,  AnaP0DReconCluster*  anaP0DReconCluster);

  // --- New Methods to create native psyche P0D objects -------
  // ---- TEST HIGHLAND2-P0D ----
  virtual void FillP0DVertexInfo(  ND::TP0DReconModule::TP0DVertex&   p0dVertex,   AnaP0DVertex*   anaP0DVertex, AnaBunchB* bunch);
  virtual void FillP0DParticleInfo(ND::TP0DReconModule::TP0DParticle& p0dParticle, AnaP0DParticle* anaP0DParticle);
  virtual void FillP0DParticleInfo(ND::TP0DReconModule::TP0DTrack&    p0dTrack,    AnaP0DParticle* anaP0DParticle);
  virtual void FillP0DParticleInfo(ND::TP0DReconModule::TP0DShower&   p0dShower,   AnaP0DParticle* anaP0DParticle);
  virtual void FillP0DClusterInfo( ND::TP0DReconModule::TP0DCluster&  p0dCluster,  AnaP0DCluster*  anaP0DCluster);
  virtual void FillP0DClusterInfo( ND::TP0DReconModule::TP0DNode&     p0dNode,  Short_t Cycle,    AnaP0DCluster*  anaP0DCluster);
  virtual void FillP0DClusterInfo( ND::TP0DReconModule::TP0DShower&   p0dShower,   AnaP0DCluster*  anaP0DCluster);
  virtual void FillP0DHitInfo(     ND::TP0DReconModule::TP0DHit&      p0dHit,      AnaP0DHit*      anaP0DHit);
  // ----------------------------

 //----------------

  virtual void GetBunchPIDs();
  virtual void GetBunchVertices();
  virtual void GetBunchLocalObjects();
  virtual double GetVertexTime(ND::TGlobalReconModule::TGlobalPID& globalTrack);

  virtual int GetGenieReactionCode(const std::string& reactionCode);

  Double_t GetPOTFromRooTrackerVtx();

  bool HasNonContainedReconParticles(AnaTrueParticleB* trueTrack);
  bool IsFullyContained(const AnaTrueVertexB& trueVertex);

  Int_t           GetNVtx(){if (NVtx) return NNVtx; else if (GVtx) return NGVtx; else return 0;}
  TClonesArray *  GetVtx() {if (NVtx) return NVtx;  else if (GVtx) return GVtx;  else return NULL;}

  Int_t GetRooVtxEntryInCurrentInputFile(){ return _RooVtxEntryInCurrentInputFile; }

protected:

  std::vector<ND::TGlobalReconModule::TGlobalPID*> _bunchPIDs[NBUNCHES+1];
  std::vector<ND::TGlobalReconModule::TGlobalVertex*> _bunchVertices[NBUNCHES+1];


  std::vector<ND::TP0DReconModule::TP0DVertex*>   _bunchP0DVertices[NBUNCHES+1];
  std::vector<ND::TP0DReconModule::TP0DParticle*> _bunchP0DParticles[NBUNCHES+1];
  std::vector<ND::TP0DReconModule::TP0DCluster*>  _bunchP0DClusters[NBUNCHES+1];
  std::vector<ND::TTrackerECALReconModule::TECALReconObject*> _bunchTECALObjects[NBUNCHES+1];
  std::vector<ND::TTrackerECALReconModule::TECALReconUnmatchedObject*> _bunchTECALUnmatchedObjects[NBUNCHES+1];

  //---- These objects are used to keep the proper links between P0DRecon objects, converting the indices in pointers
  std::map<ND::TP0DReconModule::TP0DVertex*,   AnaP0DReconVertex*>   _P0DReconVerticesMap;
  std::map<ND::TP0DReconModule::TP0DParticle*, AnaP0DReconParticle*> _P0DReconParticlesMap;
  std::map<ND::TP0DReconModule::TP0DCluster*,  AnaP0DReconCluster*>  _P0DReconClustersMap;


  // ---- TEST HIGHLAND2-P0D ----
  std::vector<Int_t> _bunchP0DVerticesInAlgoRes [NBUNCHES+1][NMAXP0DALGORES];
  std::vector<Int_t> _bunchP0DClustersInAlgoRes [NBUNCHES+1][NMAXP0DALGORES];
  std::vector<Int_t> _bunchP0DParticlesInAlgoRes[NBUNCHES+1][NMAXP0DALGORES];
  std::vector<Int_t> _bunchP0DTracksInAlgoRes   [NBUNCHES+1][NMAXP0DALGORES];
  std::vector<Int_t> _bunchP0DShowersInAlgoRes  [NBUNCHES+1][NMAXP0DALGORES];

  std::vector<AnaP0DVertex*>   _AnaP0DVertices;
  std::vector<AnaP0DParticle*> _AnaP0DParticles;
  std::vector<AnaP0DParticle*> _AnaP0DTracks;
  std::vector<AnaP0DParticle*> _AnaP0DShowers;
  std::vector<AnaP0DCluster*>  _AnaP0DClusters;
  // ----------------------------

  //------------------------------------------------------------------------------------------------------------------

  bool _firstFile;
  bool _firstEntry;

  Int_t _RooVtxEntryInCurrentInputFile;

  TTruthUtils _truthUtils;

  AnaSpill* _spill;

  ND280BeamBunching _bunching;

  std::string _previousFile;
  Int_t _previousRunID;
  Int_t _previousSubrunID;
  Int_t _previousRefEventID;

 protected:

  // TChains
  TChain *reconGlobal;
  TChain *trackerECAL;
  TChain *p0d;
  TChain *fgdOnly;
  TChain *mcTruthVertices;
  TChain *mcTruthTrajectories;
  TChain *GRooTrackerVTX;
  TChain *NRooTrackerVTX;
  TChain *beamInfo;
  TChain *accPOTInfo;
  TChain *BasicHeader;
  TChain *DQInfo;

  Int_t Entries;
  Int_t Counter;

  // MC information
  bool Genie;
  bool Neut;
  bool GenieFirstFile;
  bool NeutFirstFile;

  // Skimmed information
  bool AccPOTflg;

  // Header's
  Int_t EventID;
  Int_t RunID;
  Int_t SubrunID;
  Bool_t IsMC;
  Int_t Preselected;
  Int_t EventTime;

  ULong64_t TriggerWord;
  ULong64_t CTMTriggerPattern[3];
  Bool_t FGDCosmicEvent;
  Bool_t TripTCosmicEvent;
  Bool_t P0DWaterStatus;
  Char_t SoftwareVersion[50];
  Float_t POTPerSpill;

  // Tracker ECAL
  Int_t NTrECalObjects;
  TClonesArray *TrECalObjects;
  Int_t NTrECalUnmatched;
  TClonesArray *TrECalUnmatched;

  // P0D
  // ---- TEST HIGHLAND2-P0D ----
  Short_t NP0DAlgoResults;
  TClonesArray *P0DAlgoResults;
  Short_t NP0DReconVertices;
  TClonesArray *P0DReconVertices;
  Short_t NP0DReconParticles;
  TClonesArray *P0DReconParticles;
  Short_t NP0DReconTracks;
  TClonesArray *P0DReconTracks;
  Short_t NP0DReconShowers;
  TClonesArray *P0DReconShowers;
  Short_t NP0DReconClusters;
  TClonesArray *P0DReconClusters;
  Short_t NP0DReconNodes;
  TClonesArray *P0DReconNodes;
  Short_t NP0DReconHits;
  TClonesArray *P0DReconHits;

  // ----------------------------

  // PID's
  Int_t NPIDs;
  TClonesArray *PIDs;
  Int_t NVertices;
  TClonesArray *Vertices;
  Int_t NFGD1Unused,NFGD2Unused,NP0DUnused,NTPCUnused;
  Int_t NDelayedClusters;
  TClonesArray *DelayedClusters;
  Int_t NTPCOthers;
  TClonesArray *TPCOthers;

  Int_t NFgdTimeBins;
  TClonesArray *FgdTimeBins;

   // Set the branches for the truth information.
  Int_t NTruthVertices;
  Int_t NVtxFGD1;
  Int_t NVtxFGD2;

  TClonesArray *TruthVertices;
  std::string TruthVerticesName[9];

  Int_t NTruthTrajs;
  TClonesArray *TruthTrajs;
  std::string TruthTrajsName[7];


  // Genie and Neut Tracker Vertex
  Int_t NGVtx;
  TClonesArray *GVtx;
  Int_t NNVtx;
  TClonesArray *NVtx;

  // FGD's
  Int_t NXZTracks_Radon2,NYZTracks_Radon2,NXYZTracks_Radon2,NXZTracksAllFGD_Radon2,NYZTracksAllFGD_Radon2,NXYZTracksAllFGD_Radon2;
  TClonesArray *XZTracks_Radon2;
  TClonesArray *YZTracks_Radon2;
  TClonesArray *XYZTracks_Radon2;
  TClonesArray *XZTracksAllFGD_Radon2;
  TClonesArray *YZTracksAllFGD_Radon2;
  TClonesArray *XYZTracksAllFGD_Radon2;

  // beam information
  TClonesArray *VtxP0DECal;
  TClonesArray *beamSummary;
  int beamSummaryDataStatus;
  Int_t ND280Spill;

  // DQ information
  Int_t fBarECALFlag,fDSECALFlag,fECALFlag,fFGD1Flag,fFGD2Flag,fFGDFlag,fINGRIDFlag,fMAGNETFlag,fND280OffFlag;
  Int_t fP0DECALFlag,fP0DFlag,fSMRDFlag,fTPC1Flag,fTPC2Flag,fTPC3Flag,fTPCFlag;

  /// Whether we're running in "cosmic" mode, where there is only one bunch
  bool cosmic_mode;

  bool _ForceIgnoreRunNumberClash;
  bool _FixECalGlobalVerticesTime;

  /// To calculate the ECal PID likelihood variables
  ecalPid::TECALPidLikelihood* _ecalPidCalc;

  /// Use local reconstruction information?
  bool _isUsingReconDirFGDOnly;
  bool _isUsingReconDirP0D;
  bool _isUsingReconDirPECAL;
  bool _isUsingReconDirTECAL;

  // ---- TEST HIGHLAND2-P0D ----
  bool _isUsingReconDirP0DNew;
  std::string _p0dAlgoResName;
  Int_t _topP0DAlgoRes;
  bool _addGlobalTracksToP0DVertices;
  // ----------------------------

  /// ignore some true objects when there are fully contained in the specified subdetectors
  bool _ignoreSMRDContainedTrueObjects;
  bool _ignoreP0DECalContainedTrueObjects;
  bool _ignoreBrECalContainedTrueObjects;
  bool _ignoreDsECalContainedTrueObjects;
  bool _ignoreINGRIDContainedTrueObjects;

  /// Remove uninteresting vertices
  bool _removeTrueVerticesWithNoTrueParticles;

  // Vector of Fully contained TrueParticles
  std::vector<AnaTrueParticleB*> _containedTrueParticles;

  /// Whether to fill FGD single hits info into FGD time bins
  bool _fillFgdSingleHitsInfo;

protected:
  bool _foundPauliBlocked;
  bool _foundCohOnH;
  bool _alreadyWarned;
};



// function to sort tracks in decreasing Cleanliness order
// class needed for passing the global vertex position as argument
class CompareCleanliness{

  AnaVertex* vertex;

public:

  CompareCleanliness(AnaVertex* param) : vertex(param) {}

  bool operator()(std::pair<AnaTrueVertexB*, AnaRecTrueMatchB> t1, std::pair<AnaTrueVertexB*, AnaRecTrueMatchB> t2) {

    // set null pointer to be -1 so they are sorted last.
    double m1 = -1;
    double m2 = -1;
    if (t1.first != NULL) m1 = t1.second.Cleanliness;
    if (t2.first != NULL) m2 = t2.second.Cleanliness;

    // Set nan to be -1 so they are sorted last.
    if (m1 != m1) m1 = -1;
    if (m2 != m2) m2 = -1;

    if (m1 != m2) return m1 > m2;

    // if same cleanliness, take the nearest one (
    // (in general the nearest could not have any common tracks,
    // then better sort by Cleanliness first)

    // sort first those in-bunch
    if (vertex->Bunch == t1.first->Bunch && vertex->Bunch != t2.first->Bunch) return true;
    if (vertex->Bunch == t2.first->Bunch && vertex->Bunch != t1.first->Bunch) return false;

    double t1dist = anaUtils::GetSeparationSquared(t1.first->Position, vertex->Position);
    double t2dist = anaUtils::GetSeparationSquared(t2.first->Position, vertex->Position);
    return t1dist < t2dist;
  }
};



#endif

