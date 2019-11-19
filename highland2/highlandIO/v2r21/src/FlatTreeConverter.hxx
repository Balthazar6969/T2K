/*
   converter for new FlatTree input format (direct dump of analysis data classes)

   A. Cervera September 2012
*/


#ifndef FlatTreeConverter_h
#define FlatTreeConverter_h

#include "RedoTreeConverter.hxx"
#include "DataClassesIO.hxx"
#include "P0DDataClasses.hxx"
#include "InputConverter.hxx"
#include <TClonesArray.h>
#include "oaAnalysisReaderProjectHeaders.h"


class FlatTreeConverter: public RedoTreeConverter{

 public:

  FlatTreeConverter(bool readRooTrackerVtx=false);
  virtual ~FlatTreeConverter();

  virtual bool Initialize();

  /// Define the branches to be read from the input tree
  virtual void DefineBranches();

  using RedoTreeConverter::GetSpill;
  Int_t GetSpill(Long64_t& entry, AnaSpillC*& spill);

  virtual bool AddFileToTChain(const std::string& inputString);

  virtual AnaSpillB*         MakeSpill        () { return new AnaSpill        (); }
  virtual AnaBunchB*         MakeBunch        () { return new AnaBunch        (); }
  virtual AnaBeamB*          MakeBeam         () { return new AnaBeam         (); }
  virtual AnaDataQualityB*   MakeDataQuality  () { return new AnaDataQuality  (); }
  virtual AnaEventInfoB*     MakeEventInfo    () { return new AnaEventInfo    (); }
  virtual AnaTrigger*        MakeTrigger      () { return new AnaTrigger      (); }
  virtual AnaVertexB*        MakeVertex       () { return new AnaVertex       (); }
  virtual AnaTrueParticleB*  MakeTrueParticle () { return new AnaTrueParticle (); }
  virtual AnaTrueVertexB*    MakeTrueVertex   () { return new AnaTrueVertex   (); }
  virtual AnaTrackB*         MakeTrack        () { return new AnaTrack        (); }
  virtual AnaTPCParticleB*   MakeTpcTrack     () { return new AnaTPCParticle  (); }
  virtual AnaTrackerTrackB*  MakeTrackerTrack () { return new AnaTrackerTrack (); }
  virtual AnaFGDParticleB*   MakeFgdTrack     () { return new AnaFGDParticle  (); }
  virtual AnaP0DParticleB*   MakeP0dTrack     () { return new AnaP0DParticle  (); }
  virtual AnaECALParticleB*  MakeEcalTrack    () { return new AnaECALParticle (); }
  virtual AnaSMRDParticleB*  MakeSmrdTrack    () { return new AnaSMRDParticle (); }
  virtual AnaFgdTimeBinB*    MakeFgdTimeBin   () { return new AnaFgdTimeBin   (); }

  // Optional information from the local reconstruction...
  virtual AnaBunchB*               MakeLocalReconBunch     () { return new AnaLocalReconBunch     (); }
  virtual AnaTECALReconObject*     MakeTECALReconObject    () { return new AnaTECALReconObject    (); }
  virtual AnaTECALUnmatchedObject* MakeTECALUnmatchedObject() { return new AnaTECALUnmatchedObject(); }
  virtual AnaPECALReconObject*     MakePECALReconObject    () { return new AnaPECALReconObject    (); }
  virtual AnaPECALUnmatchedObject* MakePECALUnmatchedObject() { return new AnaPECALUnmatchedObject(); }
  virtual AnaP0DReconObject*       MakeP0DReconVertex      () { return new AnaP0DReconVertex      (); }
  virtual AnaFGDOnlyReconObject*   MakeFGDOnlyReconObject  () { return new AnaFGDOnlyReconObject  (); }

  virtual void FillTECALReconObjectInfo(int iObj, AnaTECALReconObject* anaTECAL, AnaBunch* bunch);

  using RedoTreeConverter::FillInfo;
  virtual void FillInfo(AnaSpillB* spill);
  using RedoTreeConverter::FillBunchInfo;
  virtual void FillBunchInfo(std::vector<AnaTrueParticleB*>& trueParticles, AnaBunchB* bunch);
  virtual void FillNotInBunchObjWithNullPointers(AnaSpill* spill);
  virtual void CheckNotInBunchObjWithNullPointers(AnaSpill* spill);
  virtual void FillNotInBunchObjWithNullPointers(AnaBunch* bunch);
  using RedoTreeConverter::FillFgdTimeBinInfo;
  virtual void FillFgdTimeBinInfo(std::vector<AnaFgdTimeBinB*>& AnaFgdTimeBins);
  using RedoTreeConverter::FillTrackInfo;
  virtual void FillTrackInfo(std::vector<AnaTrueParticleB*>& trueParticles, int itrk, AnaTrackB* track);
  using RedoTreeConverter::FillTrueParticleInfo;
  virtual void FillTrueParticleInfo(int ntrueVertices, AnaTrueVertexB** trueVertices, int itrk, AnaTrueParticleB* part);
  using RedoTreeConverter::FillTpcInfo;
  virtual void FillTpcInfo(std::vector<AnaTrueParticleB*>& trueParticles, int itrk,  int tpc, AnaTPCParticleB* track);
  using RedoTreeConverter::FillFgdInfo;
  virtual void FillFgdInfo(int itrk,  int fgd, AnaFGDParticleB* track);
  using RedoTreeConverter::FillEcalInfo;
  virtual void FillEcalInfo(int itrk, int ecal, AnaECALParticleB* track);
  using RedoTreeConverter::FillSmrdInfo;
  virtual void FillSmrdInfo(int itrk, int smrd, AnaSMRDParticleB* track);
  using RedoTreeConverter::FillP0dInfo;
  virtual void FillP0dInfo(int itrk,  int p0d, AnaP0DParticleB* track);
  virtual void FillTrackerInfo(int itrk, int tracker, AnaTrackerTrack* seg);

  using RedoTreeConverter::FillSubdetectorInfo;
  virtual void FillSubdetectorInfo(std::vector<AnaTrueParticleB*>& trueParticles, int itrk, AnaTrackB* trackB);

  using RedoTreeConverter::FillTrueVertexInfo;
  virtual void FillTrueVertexInfo(int ivtx, AnaTrueVertexB* vertex);
  using RedoTreeConverter::FillTrueVertexRooInfo;
  virtual bool FillTrueVertexRooInfo(AnaTrueVertexB* vertex);
  virtual void FillNPrimaryParticles(AnaTrueVertex* vertex);

  using RedoTreeConverter::FillDQInfo;
  virtual void FillDQInfo(AnaDataQualityB* dq);
  using RedoTreeConverter::FillBeamInfo;
  virtual void FillBeamInfo(AnaBeamB* beam);
  virtual void FillTriggerInfo(AnaTrigger* trigger);

  using RedoTreeConverter::FillVertexInfo;
  virtual void FillVertexInfo(int ivtx, AnaVertexB* vertex, AnaBunchB* bunch);

//  virtual int  FindTrueVertex(int ID, AnaTrueVertex*& trueVertex); // not used anymore
  virtual AnaParticleB* FindParticle(int ID, AnaBunchB* bunch);

  /// Get old local detector enumeration to find array index of Flat tree
  int GetLocalDetEnum(SubDetId::SubDetEnum det, SubDetId::SubDetEnum i);

protected:

  bool _useCorrectedValues;
  bool _readRooTrackerVtx;

  bool _firstFile;
  bool _firstEntry;

  Int_t _RooVtxEntryInCurrentInputFile;
  
  // the current RooTrackerVtx entry
  Long64_t _entry_roo;

  /// Store whether AnaLocalReconBunch information is enabled in the parameters file
  bool _isUsingReconDirFGDOnly;
  bool _isUsingReconDirP0D;
  bool _isUsingReconDirPECAL;
  bool _isUsingReconDirTECAL;

 protected:

  // Genie and Neut Tracker Vertex
  Int_t RunID;
  Int_t SubrunID;
  Int_t EventID;
  Int_t NNVtx;
  TClonesArray *NVtx;
  Int_t NGVtx;
  TClonesArray *GVtx;

  Int_t           sIsMC;
  
  // AnaTrigger related
  Int_t           sTriggerFGDCosmic;
  Int_t           sTriggerTripTCosmic;
  // This is to have enough precision for 32-bit patterns
  // Double_t gives ~16 decimal digits over ~6 in Float_t (need 10)
  Double_t        sTriggerCTMTriggerPattern[6];
  
  Int_t           sRooVtxEntry;
  Int_t           sInputFileIndex;
  Int_t           sTrueVertexLeptonPDG[NMAXTRUEVERTICES];   //[sNTrueVertices]
  Int_t           sTrueVertexIsPauliBlocked[NMAXTRUEVERTICES];   //[sNTrueVertices]
  Int_t           sTrueVertexIsCohOnH[NMAXTRUEVERTICES];   //[sNTrueVertices]
  Int_t           sTrueVertexNPrimaryParticles[NMAXTRUEVERTICES][Int_t(ParticleId::kLast)+1];   //[sNTrueVertices]
  Int_t           trReconPDG[NMAXPARTICLES];   //[NParticles]
  Int_t           trNTimeNodes[NMAXPARTICLES]; //[NParticles]
  Int_t           trTimeNodeDetector[NMAXPARTICLES][NMAXTIMENODES]; //[NParticles]
  Int_t           trTpcUniqueID[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
  Int_t           trFgdUniqueID[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
  Int_t           trECALUniqueID[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
  Int_t           trSMRDUniqueID[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
  Int_t           trP0DUniqueID[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
  Int_t           trNTRACKERs[NMAXPARTICLES];   //[NParticles]
  Int_t           trTRACKERDetector[NMAXPARTICLES][NMAXTRACKERS];   //[NParticles]
  Int_t           trTRACKERNHits[NMAXPARTICLES][NMAXTRACKERS];   //[NParticles]
  Int_t           trTRACKERNNodes[NMAXPARTICLES][NMAXTRACKERS];   //[NParticles]
  Int_t           trTRACKERUniqueID[NMAXPARTICLES][NMAXTRACKERS];   //[NParticles]
  Int_t           trFgdHasFgdVA[NMAXPARTICLES][NMAXFGDS];   //[NParticles]
  Int_t           trFgdHasFgdVA_fmode[NMAXPARTICLES][NMAXFGDS];   //[NParticles]
  Int_t           trFgdTypeVA[NMAXPARTICLES][NMAXFGDS];   //[NParticles]
  Int_t           trFgdHasFgdEA[NMAXPARTICLES][NMAXFGDS];   //[NParticles]
  Int_t           trFgdHasFgdEA_fmode[NMAXPARTICLES][NMAXFGDS];   //[NParticles]
  Int_t           trFgdTypeEA[NMAXPARTICLES][NMAXFGDS];   //[NParticles]

  
  
  Float_t         sTrueVertexTargetMom[NMAXTRUEVERTICES];   //[sNTrueVertices]
  Float_t         sTrueVertexTargetDir[NMAXTRUEVERTICES][3];   //[sNTrueVertices]
  Float_t         trTimeNodeTimeStart[NMAXPARTICLES][NMAXTIMENODES]; //[NParticles]
  Float_t         trTimeNodeTimeEnd[NMAXPARTICLES][NMAXTIMENODES]; //[NParticles]
  Float_t         sTrueVertexLeptonDir[NMAXTRUEVERTICES][3];   //[sNTrueVertices]
  Float_t         sTrueVertexProtonDir[NMAXTRUEVERTICES][3];   //[sNTrueVertices]
  Float_t         sTrueVertexPionDir[NMAXTRUEVERTICES][3];   //[sNTrueVertices]
  Float_t         sTrueVertexLeptonMom[NMAXTRUEVERTICES];   //[sNTrueVertices]
  Float_t         sTrueVertexProtonMom[NMAXTRUEVERTICES];   //[sNTrueVertices]
  Float_t         sTrueVertexPionMom[NMAXTRUEVERTICES];   //[sNTrueVertices]
  Float_t         trTpcPullmu[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
  Float_t         trTpcPullele[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
  Float_t         trTpcPullp[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
  Float_t         trTpcPullpi[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
  Float_t         trTpcPullk[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
  Float_t         trTpcRawdEdxMeas[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
  Float_t         trTpcRawdEdxExpMu[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
  Float_t         trTpcRawdEdxExpEle[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
  Float_t         trTpcRawdEdxExpP[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
  Float_t         trTpcRawdEdxExpPi[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
  Float_t         trTpcRawdEdxExpK[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
  Float_t         trFgdEnd0x0[NMAXPARTICLES][NMAXFGDS];   //[NParticles]
  Float_t         trFgdEnd1x1[NMAXPARTICLES][NMAXFGDS];   //[NParticles]
  Float_t         trFgdEnd3x3[NMAXPARTICLES][NMAXFGDS];   //[NParticles]
  Float_t         trFgdEnd5x5[NMAXPARTICLES][NMAXFGDS];   //[NParticles]
  Float_t         trFgdEnd7x7[NMAXPARTICLES][NMAXFGDS];   //[NParticles]
  
  Float_t         trECALPIDMipPion[NMAXPARTICLES][NECALSUBDETS];  //[NParticles]
  Float_t         trECALContainment[NMAXPARTICLES][NECALSUBDETS]; //[NParticles]
  
  // SMRD
  Float_t         trSMRDEDeposit[NMAXPARTICLES][NSMRDSUBDETS]; //[NParticles]
  
  
  Float_t         trP0DAvgTime[NMAXPARTICLES][NMAXP0DS];   //[NParticles]
  Float_t         trTRACKERLength[NMAXPARTICLES][NMAXTRACKERS];   //[NParticles]
  Float_t         trTRACKERDirectionStart[NMAXPARTICLES][NMAXTRACKERS][3];   //[NParticles]
  Float_t         trTRACKERDirectionEnd[NMAXPARTICLES][NMAXTRACKERS][3];   //[NParticles]
  Float_t         trTRACKERPositionStart[NMAXPARTICLES][NMAXTRACKERS][4];   //[NParticles]
  Float_t         trTRACKERPositionEnd[NMAXPARTICLES][NMAXTRACKERS][4];   //[NParticles]
  Float_t         trTRACKERCharge[NMAXPARTICLES][NMAXTRACKERS];   //[NParticles]
  Float_t         trTRACKERMomentum[NMAXPARTICLES][NMAXTRACKERS];   //[NParticles]
  Float_t         trTRACKERMomentumEnd[NMAXPARTICLES][NMAXTRACKERS];   //[NParticles]
  Float_t         trDirectionStartFlip[NMAXPARTICLES][3];   //[NParticles]
  Float_t         trDirectionEndFlip[NMAXPARTICLES][3];   //[NParticles]
  Float_t         trRangeMomentumMuonFlip[NMAXPARTICLES];   //[NParticles]
  Float_t         trRangeMomentumProtonFlip[NMAXPARTICLES];   //[NParticles]
  Float_t         trMomentumAtVertex[NMAXPARTICLES];   //[NParticles]
  Float_t         trDirectionAtVertex[NMAXPARTICLES][3];   //[NParticles]
  Float_t         trLength[NMAXPARTICLES];   //[NParticles]

  
  /// AnaLocalReconBunch information
  Int_t           tecalReconCounter;
  Float_t         tecalReconAverageHitTime          [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Float_t         tecalReconAverageZPosition        [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Float_t         tecalReconContainment             [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Float_t         tecalReconEFitResult              [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Float_t         tecalReconEFitUncertainty         [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Int_t           tecalReconFirstLayer              [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Int_t           tecalReconIsShowerLike            [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Int_t           tecalReconIsTrackLike             [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Int_t           tecalReconLastLayer               [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Float_t         tecalReconLikeMIPEM               [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Float_t         tecalReconLikeMIPPion             [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Float_t         tecalReconLikeEMHIP               [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Float_t         tecalReconLikeMIPEMLow            [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Float_t         tecalReconMatchingLike            [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Int_t           tecalReconMichelTagNDelayedCluster[NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Char_t          tecalReconModule                  [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Int_t           tecalReconMostDownStreamLayerHit  [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Int_t           tecalReconMostUpStreamLayerHit    [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Int_t           tecalReconNHits                   [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Int_t           tecalReconNLayersHit              [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Float_t         tecalReconObjectLength            [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Float_t         tecalReconPIDAMR                  [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Float_t         tecalReconPIDAngle                [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Float_t         tecalReconPIDAsymmetry            [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Float_t         tecalReconPIDCircularity          [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Float_t         tecalReconPIDFBR                  [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Float_t         tecalReconPIDMaxRatio             [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Float_t         tecalReconPIDMeanPos              [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Float_t         tecalReconPIDShowerAngle          [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Float_t         tecalReconPIDShowerWidth          [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Float_t         tecalReconPIDTransverseChargeRatio[NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Float_t         tecalReconPIDTruncatedMaxRatio    [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Float_t         tecalReconPointing                [NMAXTECALRECONOBJECTS][3]; //[tecalReconCounter]
  Float_t         tecalReconThrust                  [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Float_t         tecalReconThrustAxis              [NMAXTECALRECONOBJECTS][3]; //[tecalReconCounter]
  Float_t         tecalReconThrustOrigin            [NMAXTECALRECONOBJECTS][3]; //[tecalReconCounter]
  Int_t           tecalReconTimeBunch               [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Float_t         tecalReconTotalHitCharge          [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Int_t           tecalReconTrueID                  [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Int_t           tecalReconTrueIDPrimary           [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Int_t           tecalReconTrueIDRecursive         [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Int_t           tecalReconTrueIDSingle            [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Int_t           tecalReconUniqueID                [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]
  Int_t           tecalReconBunch                   [NMAXTECALRECONOBJECTS];    //[tecalReconCounter]


  // List of branches
  TBranch        *b_sIsMC;   //!
  TBranch        *b_sInputFileIndex;   //!
  TBranch        *b_sRooVtxEntry; //!
  TBranch        *b_sBeamPOT;   //!
  TBranch        *b_sBeamPOTCT4;   //!
  TBranch        *b_sBeamSpill;   //!
  TBranch        *b_sBeamSpillNumber;   //!
  TBranch        *b_sBeamBeamRunNumber;   //!
  TBranch        *b_sDQND280Flag;   //!
  TBranch        *b_sDQDetFlag;   //!
 
  // AnaTrigger related
  TBranch        *b_sTriggerFGDCosmic;//!
  TBranch        *b_sTriggerTripTCosmic;//!
  TBranch        *b_sTriggerCTMTriggerPattern;//!
     
  TBranch        *b_sTrueVertexLeptonPDG;   //!
  TBranch        *b_sTrueVertexLeptonDir;   //!
  TBranch        *b_sTrueVertexProtonDir;   //!
  TBranch        *b_sTrueVertexPionDir;   //!
  TBranch        *b_sTrueVertexLeptonMom;   //!
  TBranch        *b_sTrueVertexProtonMom;   //!
  TBranch        *b_sTrueVertexPionMom;   //!
  TBranch        *b_sTrueVertexTargetDir;   //!
  TBranch        *b_sTrueVertexTargetMom;   //!
  TBranch        *b_sTrueVertexIsPauliBlocked;   //!
  TBranch        *b_sTrueVertexIsCohOnH;   //!  
  TBranch        *b_sTrueVertexNPrimaryParticles;   //!  

  TBranch        *b_trReconPDG; //!
  TBranch        *b_trDirectionAtVertex;   //!
  
  TBranch        *b_trNTRACKERs;   //!
  
  TBranch        *b_trTpcPullmu;   //!
  TBranch        *b_trTpcPullele;   //!
  TBranch        *b_trTpcPullp;   //!
  TBranch        *b_trTpcPullpi;   //!
  TBranch        *b_trTpcPullk;   //!
  TBranch        *b_trNTimeNodes; //!
  TBranch        *b_trTimeNodeDetector; //!
  TBranch        *b_trTimeNodeTimeStart; //!
  TBranch        *b_trTimeNodeTimeEnd; //!
  
  TBranch        *b_trTpcUniqueID;   //!
  TBranch        *b_trFgdUniqueID;   //!
  TBranch        *b_trECALUniqueID;   //!
  TBranch        *b_trSMRDUniqueID;   //!
  TBranch        *b_trP0DUniqueID;   //!
  
  TBranch        *b_trTpcRawdEdxMeas;   //!
  TBranch        *b_trTpcRawdEdxExpMu;   //!
  TBranch        *b_trTpcRawdEdxExpEle;   //!
  TBranch        *b_trTpcRawdEdxExpP;   //!
  TBranch        *b_trTpcRawdEdxExpPi;   //!
  TBranch        *b_trTpcRawdEdxExpK;   //!
  
  TBranch        *b_trFgdHasFgdVA;   //!
  TBranch        *b_trFgdHasFgdVA_fmode;   //!
  TBranch        *b_trFgdTypeVA;   //!
  TBranch        *b_trFgdHasFgdEA;   //!
  TBranch        *b_trFgdHasFgdEA_fmode;   //!
  TBranch        *b_trFgdTypeEA;   //!
  TBranch        *b_trFgdEnd0x0;   //!
  TBranch        *b_trFgdEnd1x1;   //!
  TBranch        *b_trFgdEnd3x3;   //!
  TBranch        *b_trFgdEnd5x5;   //!
  TBranch        *b_trFgdEnd7x7;   //!
  
  TBranch        *b_trP0DAvgTime;   //!
  
  TBranch        *b_trTRACKERDetector;   //!
  TBranch        *b_trTRACKERNHits;   //!
  TBranch        *b_trTRACKERNNodes;   //!
  TBranch        *b_trTRACKERUniqueID;   //!
  TBranch        *b_trTRACKERLength;   //!
  TBranch        *b_trTRACKERDirectionStart;   //!
  TBranch        *b_trTRACKERDirectionEnd;   //!
  TBranch        *b_trTRACKERPositionStart;   //!
  TBranch        *b_trTRACKERPositionEnd;   //!
  TBranch        *b_trTRACKERCharge;   //!
  TBranch        *b_trTRACKERMomentum;   //!
  TBranch        *b_trTRACKERMomentumEnd;   //!
  
  TBranch        *b_trDirectionStartFlip;   //! 
  TBranch        *b_trDirectionEndFlip;   //!
  TBranch        *b_trRangeMomentumMuonFlip;   //!
  TBranch        *b_trRangeMomentumProtonFlip;   //!
  TBranch        *b_trMomentumAtVertex;   //!
    
  TBranch        *b_trECALPIDMipPion; //!
  TBranch        *b_trECALContainment; //!
  TBranch        *b_trLength;   //!
  
  TBranch        *b_trSMRDEDeposit;   //!
  
  
  TBranch        *b_tecalReconCounter;   //!
  TBranch        *b_tecalReconAverageHitTime;   //!
  TBranch        *b_tecalReconAverageZPosition;   //!
  TBranch        *b_tecalReconContainment;   //!
  TBranch        *b_tecalReconEFitResult;   //!
  TBranch        *b_tecalReconEFitUncertainty;   //!
  TBranch        *b_tecalReconFirstLayer;   //!
  TBranch        *b_tecalReconIsShowerLike;   //!
  TBranch        *b_tecalReconIsTrackLike;   //!
  TBranch        *b_tecalReconLastLayer;   //!
  TBranch        *b_tecalReconLikeMIPEM;   //!
  TBranch        *b_tecalReconLikeMIPPion;   //!
  TBranch        *b_tecalReconLikeEMHIP;   //!
  TBranch        *b_tecalReconLikeMIPEMLow;   //!
  TBranch        *b_tecalReconMatchingLike;   //!
  TBranch        *b_tecalReconMichelTagNDelayedCluster;   //!
  TBranch        *b_tecalReconModule;   //!
  TBranch        *b_tecalReconMostDownStreamLayerHit;   //!
  TBranch        *b_tecalReconMostUpStreamLayerHit;   //!
  TBranch        *b_tecalReconNHits;   //!
  TBranch        *b_tecalReconNLayersHit;   //!
  TBranch        *b_tecalReconObjectLength;   //!
  TBranch        *b_tecalReconPIDAMR;   //!
  TBranch        *b_tecalReconPIDAngle;   //!
  TBranch        *b_tecalReconPIDAsymmetry;   //!
  TBranch        *b_tecalReconPIDCircularity;   //!
  TBranch        *b_tecalReconPIDFBR;   //!
  TBranch        *b_tecalReconPIDMaxRatio;   //!
  TBranch        *b_tecalReconPIDMeanPos;   //!
  TBranch        *b_tecalReconPIDShowerAngle;   //!
  TBranch        *b_tecalReconPIDShowerWidth;   //!
  TBranch        *b_tecalReconPIDTransverseChargeRatio;   //!
  TBranch        *b_tecalReconPIDTruncatedMaxRatio;   //!
  TBranch        *b_tecalReconPointing;   //!
  TBranch        *b_tecalReconThrust;   //!
  TBranch        *b_tecalReconThrustAxis;   //!
  TBranch        *b_tecalReconThrustOrigin;   //!
  TBranch        *b_tecalReconTimeBunch;   //!
  TBranch        *b_tecalReconTotalHitCharge;   //!
  TBranch        *b_tecalReconTrueID;   //!
  TBranch        *b_tecalReconTrueIDPrimary;   //!
  TBranch        *b_tecalReconTrueIDRecursive;   //!
  TBranch        *b_tecalReconTrueIDSingle;   //!
  TBranch        *b_tecalReconUniqueID;   //!
  TBranch        *b_tecalReconBunch;   //!                                        
};

#endif


