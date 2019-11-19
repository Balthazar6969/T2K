/*
   converter for new RedoTree input format (direct dump of analysis data classes)

   A. Cervera September 2012
*/


#ifndef RedoTreeConverter_h
#define RedoTreeConverter_h

#include "InputConverter.hxx"
#include "BaseDataClasses.hxx"
#include <TClonesArray.h>

class RedoTreeConverter: public InputConverter{

 public:

  RedoTreeConverter();
  virtual ~RedoTreeConverter();

  virtual bool Initialize();

  virtual void Reset() {
    InputConverter::Reset();
    _firstFile = true;
  }

  /// Define the branches to be read from the input tree
  virtual void DefineBranches();


  Int_t GetSpill(Long64_t& entry, AnaSpillC*& spill);
  Int_t GetEvent(Long64_t& entry, AnaEventC*& event);

  /// Record the POT for the current spill, based on information in the AnaBeam
  /// member of the current AnaSpillB.
  void IncrementPOTBySpill();

  virtual bool AddFileToTChain(const std::string& inputString);
  Long64_t GetEntries();

  virtual AnaSpillB* MakeSpill() { return new AnaSpillB(); }
  virtual AnaBunchB* MakeBunch() { return new AnaBunchB(); }
  virtual AnaBeamB* MakeBeam() { return new AnaBeamB(); }
  virtual AnaDataQualityB* MakeDataQuality() { return new AnaDataQualityB(); }
  virtual AnaEventInfoB* MakeEventInfo() { return new AnaEventInfoB(); }
  //virtual   AnaTrigger* MakeTrigger() { return new AnaTrigger(); }
  virtual AnaVertexB* MakeVertex() { return new AnaVertexB(); }
  virtual AnaTrueParticleB* MakeTrueParticle() { return new AnaTrueParticleB(); }
  virtual AnaTrueVertexB* MakeTrueVertex() { return new AnaTrueVertexB(); }
  virtual AnaTrackB* MakeTrack() { return new AnaTrackB(); }
  virtual AnaTPCParticleB* MakeTpcTrack() { return new AnaTPCParticleB(); }
  virtual AnaTrackerTrackB* MakeTrackerTrack() { return new AnaTrackerTrackB(); }
  virtual AnaFGDParticleB* MakeFgdTrack() { return new AnaFGDParticleB(); }
  virtual AnaP0DParticleB* MakeP0dTrack() { return new AnaP0DParticleB(); }
  virtual AnaECALParticleB* MakeEcalTrack() { return new AnaECALParticleB(); }
  virtual AnaSMRDParticleB* MakeSmrdTrack() { return new AnaSMRDParticleB(); }
  virtual AnaFgdTimeBinB* MakeFgdTimeBin() { return new AnaFgdTimeBinB(); }


  void FillEventInfo(AnaEventB* event);
  void FillEventSpillInfo(AnaEventB* event);

  virtual void FillInfo(AnaSpillB* spill);
  virtual void FillBunchInfo(std::vector<AnaTrueParticleB*>& trueParticles, AnaBunchB* bunch);
  
  virtual void FillTrueParticleInfo(AnaEventB* event, int itrk, AnaTrueParticleB* track);
  
  virtual void FillTrueParticleInfo(int ntrueVertices, AnaTrueVertexB** trueVertices, int itrk, AnaTrueParticleB* track);
  
  virtual void FillTrackInfo(AnaEventB* event, int itrk, AnaTrackB* track);
  virtual void FillTrackInfo(std::vector<AnaTrueParticleB*>& trueParticles, int itrk, AnaTrackB* track);
  
  virtual void FillTpcInfo(std::vector<AnaTrueParticleB*>& trueParticles, int itrk,  int tpc, AnaTPCParticleB* track);
  virtual void FillTpcInfo(AnaEventB* event, int itrk,  int tpc, AnaTPCParticleB* track);
  
  virtual void FillFgdTimeBinInfo(AnaEventB* event);
  virtual void FillFgdTimeBinInfo(std::vector<AnaFgdTimeBinB*>& AnaFgdTimeBins);
  
  virtual void FillFgdInfo(int itrk,  int fgd, AnaFGDParticleB* track);
  virtual void FillEcalInfo(int itrk, int ecal, AnaECALParticleB* track);
  virtual void FillSmrdInfo(int itrk, int smrd, AnaSMRDParticleB* track);
  virtual void FillP0dInfo(int itrk,  int p0d, AnaP0DParticleB* track);

  void FillSubdetectorInfo(AnaEventB* event, int itrk, AnaTrackB* track);
  virtual void FillSubdetectorInfo(std::vector<AnaTrueParticleB*>& trueParticles, int itrk, AnaTrackB* track);

  virtual void FillTrueVertexInfo(int ivtx, AnaTrueVertexB* vertex);
  virtual bool FillTrueVertexRooInfo(AnaTrueVertexB*){return true;}

  virtual void FillDQInfo(AnaDataQualityB* dq);
  virtual void FillBeamInfo(AnaBeamB* beam);
  //  void FillTriggerInfo(AnaTrigger* trigger);

  void FindTrueParticle(AnaEventB* event, int ID, AnaTrueObjectC*& trueParticle);
  void FindTrueParticle(std::vector<AnaTrueParticleB*>& trueParticles, int ID, AnaTrueObjectC*& trueParticle);
//  int  FindTrueVertex(int ID, AnaTrueVertexB*& trueVertex); // not used anymore
  virtual void FillVertexInfo(int ivtx, AnaVertexB* vertex, AnaBunchB* bunch);
  void FillVertexInfo(int ivtx, AnaVertexB* vertex, AnaEventB* event);
  virtual AnaParticleB* FindParticle(int ID, AnaBunchB* bunch);
  AnaParticleB* FindParticle(int ID, AnaEventB* event);

protected:

  bool _firstFile;  
  Int_t _entry_roo;  
  TString _currentfilename;
 
  AnaSpillB* _spill;

  AnaEventB* _prevEvent;

  TChain *flattree;
  TChain *GRooTrackerVTX;
  TChain *NRooTrackerVTX;

  bool fGenie;
  bool fNeut;

 protected:

   // Declaration of leaf types
   Int_t           sRun;
   Int_t           sSubrun;
   Int_t           sEvt;
   Int_t           sEventTime;
   Int_t           sNTotalTrueParticles;
   Int_t           sNTotalTrueVertices;
   Int_t           sBeamSpill;
   Int_t           sBeamSpillNumber;
   Int_t           sBeamGoodSpill;
   Int_t           sBeamBeamRunNumber;
   Int_t           sDQGoodDaq;
   Int_t           sDQND280Flag;
   Int_t           sDQDetFlag[7];

   Int_t           sNTrueVertices;
   Int_t           sTrueVertexID[NMAXTRUEVERTICES];   //[sNTrueVertices]
   Int_t           sTrueVertexRooVtxIndex[NMAXTRUEVERTICES];   //[sNTrueVertices]
   Int_t           sTrueVertexRooVtxEntry[NMAXTRUEVERTICES];   //[sNTrueVertices]
   Int_t           sTrueVertexReacCode[NMAXTRUEVERTICES];   //[sNTrueVertices]
   Int_t           sTrueVertexNTrueParticles[NMAXTRUEVERTICES];   //[sNTrueVertices]
   Int_t           sTrueVertexNReconTracks[NMAXTRUEVERTICES];   //[sNTrueVertices]
   Int_t           sTrueVertexNReconVertices[NMAXTRUEVERTICES];   //[sNTrueVertices]
   Int_t           sTrueVertexNBaryons[NMAXTRUEVERTICES];   //[sNTrueVertices]
   Int_t           sTrueVertexBunch[NMAXTRUEVERTICES];   //[sNTrueVertices]
   Int_t           sTrueVertexDetector[NMAXTRUEVERTICES];   //[sNTrueVertices]
   Int_t           sTrueVertexNuPDG[NMAXTRUEVERTICES];   //[sNTrueVertices]
   Int_t           sTrueVertexTargetPDG[NMAXTRUEVERTICES];   //[sNTrueVertices]
   Int_t           sNTrueParticles;
   Int_t           sTrueVertexNuParentPDG[NMAXTRUEVERTICES];   //[sNTrueVertices]
   Int_t           sTrueParticleID[NMAXTRUEPARTICLES];   //[sNTrueParticles]
   Int_t           sTrueParticleBunch[NMAXTRUEPARTICLES];   //[sNTrueParticles]
   Int_t           sTrueParticlePDG[NMAXTRUEPARTICLES];   //[sNTrueParticles]
   Int_t           sTrueParticleParentPDG[NMAXTRUEPARTICLES];   //[sNTrueParticles]
   Int_t           sTrueParticleGParentPDG[NMAXTRUEPARTICLES];   //[sNTrueParticles]
   Int_t           sTrueParticlePrimaryID[NMAXTRUEPARTICLES];   //[sNTrueParticles]
   Int_t           sTrueParticleParentID[NMAXTRUEPARTICLES];   //[sNTrueParticles]
   Int_t           sTrueParticleGParentID[NMAXTRUEPARTICLES];   //[sNTrueParticles]
   Int_t           sTrueParticleVertexIndex[NMAXTRUEPARTICLES];   //[sNTrueParticles]
   Int_t           sTrueParticleTruthVertexID[NMAXTRUEPARTICLES];   //[sNTrueParticles]
   Int_t           sTrueParticleNReconTracks[NMAXTRUEPARTICLES];   //[sNTrueParticles]
   Int_t           sTrueParticleNDetCrossings[NMAXTRUEPARTICLES];   //[NTrueParticles]
   Int_t           sTrueParticleDetector[NMAXTRUEPARTICLES][NMAXCROSSEDDET];   //[NTrueParticles]
   Int_t           sTrueParticleInActive[NMAXTRUEPARTICLES][NMAXCROSSEDDET];   //[NTrueParticles]
   Int_t           sTrueParticleIsTruePrimaryPi0DecayPhoton[NMAXTRUEPARTICLES]; //[sNTrueParticles]
   Int_t           sTrueParticleIsTruePrimaryPi0DecayPhotonChild[NMAXTRUEPARTICLES]; //[sNTrueParticles]
   Int_t           sNFgdTimeBins;
   Int_t           sFgdTimeBinNHits1[NMAXFGDTIMEBINS];   //[sNFgdTimeBins]
   Int_t           sFgdTimeBinNHits2[NMAXFGDTIMEBINS];   //[sNFgdTimeBins]
   Int_t           sFgdTimeBinG4ID[NMAXFGDTIMEBINS];   //[sNFgdTimeBins]
   Int_t           Bunch;
   Int_t           NVertices;
   Int_t           vPrimaryIndex[NMAXVERTICES];  //[NVertices]
   Int_t           vBunch[NMAXVERTICES];  //[NVertices]
   Int_t           vNDOF[NMAXVERTICES];  //[NVertices]
   Int_t           vNReconParticles[NMAXVERTICES];  //[NVertices]
   Int_t           vNTrueVerticesMatch[NMAXVERTICES];  //[NVertices]
   Int_t           vParticlesUniqueID[NMAXVERTICES][NMAXPARTICLESINVERTEX];   //[NVertices]
   Int_t           vTrueVertexIndex[NMAXVERTICES][NMAXTRUEVERTICESMATCH];   //[NVertices]
   Int_t           NParticles;
   Int_t           trUniqueID[NMAXPARTICLES];   //[NParticles]
   Int_t           trStatus[NMAXPARTICLES];   //[NParticles]
   Int_t           trBunch[NMAXPARTICLES];   //[NParticles]
   Int_t           trNReconVertices[NMAXPARTICLES];  //[NParticles]
   Int_t           trDetectors[NMAXPARTICLES];   //[NParticles]
   Int_t           trDetUsed[NMAXPARTICLES][NDETECTORS];   //[NParticles]
   Int_t           trNTPCs[NMAXPARTICLES];   //[NParticles]
   Int_t           trNFGDs[NMAXPARTICLES];   //[NParticles]
   Int_t           trNECALs[NMAXPARTICLES];   //[NParticles]
   Int_t           trNSMRDs[NMAXPARTICLES];   //[NParticles]
   Int_t           trNP0Ds[NMAXPARTICLES];   //[NParticles]
   Int_t           trNHits[NMAXPARTICLES];   //[NParticles]
   Int_t           trNNodes[NMAXPARTICLES];   //[NParticles]
   Int_t           trNDOF[NMAXPARTICLES];   //[NParticles]
   Int_t           trTpcDetector[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
   Int_t           trTpcNHits[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
   Int_t           trTpcNNodes[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
   
   Int_t           trFgdDetector[NMAXPARTICLES][NMAXFGDS];   //[NParticles]
   Int_t           trFgdNHits[NMAXPARTICLES][NMAXFGDS];   //[NParticles]
   Int_t           trFgdNNodes[NMAXPARTICLES][NMAXFGDS];   //[NParticles]
   Int_t           trFgdContainment[NMAXPARTICLES][NMAXFGDS];   //[NParticles]
   
   Int_t           trECALDetector[NMAXPARTICLES][NECALSUBDETS];   //[NParticles]
   Int_t           trECALNHits[NMAXPARTICLES][NECALSUBDETS];   //[NParticles]
   Int_t           trECALNNodes[NMAXPARTICLES][NECALSUBDETS];   //[NParticles]
   Int_t           trECALMostUpStreamLayerHit[NMAXPARTICLES][NECALSUBDETS]; //[NParticles]
   Int_t           trECALIsShowerLike[NMAXPARTICLES][NECALSUBDETS];   //[NParticles]
   
   Int_t           trSMRDDetector[NMAXPARTICLES][NSMRDSUBDETS];   //[NParticles]
   Int_t           trSMRDNHits[NMAXPARTICLES][NSMRDSUBDETS];   //[NParticles]
   Int_t           trSMRDNNodes[NMAXPARTICLES][NSMRDSUBDETS];   //[NParticles]
  
   Int_t           trP0DDetector[NMAXPARTICLES][1];   //[NParticles]
   Int_t           trP0DNHits[NMAXPARTICLES][1];   //[NParticles]
   Int_t           trP0DNNodes[NMAXPARTICLES][1];   //[NParticles]
   
   Int_t           trTrueParticleID[NMAXPARTICLES];   //[NParticles]
   Int_t           trTpcTrueParticleID[NMAXPARTICLES][NMAXTPCS];   //[NParticles]

   Float_t         Weight;
   Float_t         sTrueVertexNuEnergy[NMAXTRUEVERTICES];   //[sNTrueVertices]
   Float_t         sTrueVertexQ2[NMAXTRUEVERTICES];   //[sNTrueVertices]
   Float_t         sTrueVertexPosition[NMAXTRUEVERTICES][4];   //[sNTrueVertices]
   Float_t         sBeamPOT;
   Float_t         sBeamPOTCT4;
   Float_t         sPOTSincePreviousSpill;
   Float_t         sTrueVertexNuParentDecPoint[NMAXTRUEVERTICES][4];   //[sNTrueVertices]
   Float_t         sTrueVertexNuDir[NMAXTRUEVERTICES][3];   //[sNTrueVertices]
   Float_t         sTrueParticlePurity[NMAXTRUEPARTICLES];   //[sNTrueParticles]
   Float_t         sTrueParticleMomentum[NMAXTRUEPARTICLES];   //[sNTrueParticles]
   Float_t         sTrueParticleCharge[NMAXTRUEPARTICLES];   //[sNTrueParticles]
   Float_t         sTrueParticlePosition[NMAXTRUEPARTICLES][4];   //[sNTrueParticles]
   Float_t         sTrueParticlePositionEnd[NMAXTRUEPARTICLES][4];   //[sNTrueParticles]
   Float_t         sTrueParticleDirection[NMAXTRUEPARTICLES][3];   //[sNTrueParticles]
   Float_t         sTrueParticleEntrancePosition[NMAXTRUEPARTICLES][NMAXCROSSEDDET][4];   //[NTrueParticles]
   Float_t         sTrueParticleExitPosition[NMAXTRUEPARTICLES][NMAXCROSSEDDET][4];   //[NTrueParticles]
   Float_t         sTrueParticleEntranceMomentum[NMAXTRUEPARTICLES][NMAXCROSSEDDET][3];   //[NTrueParticles]
   Float_t         sTrueParticleExitMomentum[NMAXTRUEPARTICLES][NMAXCROSSEDDET][3];   //[NTrueParticles]
   Float_t         sFgdTimeBinMinTime[NMAXFGDTIMEBINS];   //[sNFgdTimeBins]
   Float_t         sFgdTimeBinMaxTime[NMAXFGDTIMEBINS];   //[sNFgdTimeBins]
   Float_t         sFgdTimeBinRawChargeSum1[NMAXFGDTIMEBINS];   //[sNFgdTimeBins]
   Float_t         sFgdTimeBinRawChargeSum2[NMAXFGDTIMEBINS];   //[sNFgdTimeBins]
   Float_t         vPosition[NMAXVERTICES][4];   //[NVertices]
   Float_t         vVariance[NMAXVERTICES][4];   //[NVertices]
   Float_t         vChi2[NMAXVERTICES];  //[NVertices]
   Float_t         vTrueVerticesClean[NMAXVERTICES][NMAXTRUEVERTICESMATCH];   //[NVertices]
   Float_t         vTrueVerticesCompl[NMAXVERTICES][NMAXTRUEVERTICESMATCH];   //[NVertices]
   Float_t         trChi2[NMAXPARTICLES];   //[NParticles]
   Float_t         trCharge[NMAXPARTICLES];   //[NParticles]
   Float_t         trToFFGD1_FGD2[NMAXPARTICLES]; //!
   Float_t         trToFP0D_FGD1[NMAXPARTICLES]; //!
   Float_t         trToFDSECal_FGD1[NMAXPARTICLES]; //!
   Float_t         trToFECal_FGD1[NMAXPARTICLES]; //!
   Float_t         trToFDSECal_FGD2[NMAXPARTICLES]; //!
   Float_t         trToFECal_FGD2[NMAXPARTICLES]; //!
   Float_t         trToFFlag_FGD1_FGD2[NMAXPARTICLES]; //!
   Float_t         trToFFlag_P0D_FGD1[NMAXPARTICLES]; //!
   Float_t         trToFFlag_DSECal_FGD1[NMAXPARTICLES]; //!
   Float_t         trToFFlag_ECal_FGD1[NMAXPARTICLES]; //!
   Float_t         trToFFlag_DSECal_FGD2[NMAXPARTICLES]; //!
   Float_t         trToFFlag_ECal_FGD2[NMAXPARTICLES]; //!
   Float_t         trMomentum[NMAXPARTICLES];   //[NParticles]
   Float_t         trMomentumError[NMAXPARTICLES];   //[NParticles]
   Float_t         trMomentumMuon[NMAXPARTICLES];   //[NParticles]
   Float_t         trMomentumErrorMuon[NMAXPARTICLES];   //[NParticles]
   Float_t         trMomentumProton[NMAXPARTICLES];   //[NParticles]
   Float_t         trMomentumErrorProton[NMAXPARTICLES];   //[NParticles]
   Float_t         trMomentumEle[NMAXPARTICLES];   //[NParticles]
   Float_t         trMomentumErrorEle[NMAXPARTICLES];   //[NParticles]
   Float_t         trRangeMomentumEle[NMAXPARTICLES];   //[NParticles]
   Float_t         trRangeMomentumMuon[NMAXPARTICLES];   //[NParticles]
   Float_t         trRangeMomentumProton[NMAXPARTICLES];   //[NParticles]
   Float_t         trDirectionStart[NMAXPARTICLES][3];   //[NParticles]
   Float_t         trDirectionEnd[NMAXPARTICLES][3];   //[NParticles]
   Float_t         trPositionStart[NMAXPARTICLES][4];   //[NParticles]
   Float_t         trPositionEnd[NMAXPARTICLES][4];   //[NParticles]
   Float_t         trMomentumFlip[NMAXPARTICLES];   //[NParticles]
   
   Float_t         trTpcLength[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
   Float_t         trTpcDirectionStart[NMAXPARTICLES][NMAXTPCS][3];   //[NParticles]
   Float_t         trTpcDirectionEnd[NMAXPARTICLES][NMAXTPCS][3];   //[NParticles]
   Float_t         trTpcPositionStart[NMAXPARTICLES][NMAXTPCS][4];   //[NParticles]
   Float_t         trTpcPositionEnd[NMAXPARTICLES][NMAXTPCS][4];   //[NParticles]
   Float_t         trTpcCharge[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
   Float_t         trTpcMomentum[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
   Float_t         trTpcMomentumError[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
   Float_t         trTpcBackMomentum[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
   Float_t         trTpcRefitCharge[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
   Float_t         trTpcRefitMomentum[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
   Float_t         trTpcRefitPosition[NMAXPARTICLES][NMAXTPCS][4];   //[NParticles]
   Float_t         trTpcRefitDirection[NMAXPARTICLES][NMAXTPCS][3];   //[NParticles]
   Float_t         trTpcEFieldRefitMomentum[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
   Float_t         trTpcdEdxMeas[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
   Float_t         trTpcdEdxExpMu[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
   Float_t         trTpcdEdxExpEle[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
   Float_t         trTpcdEdxExpP[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
   Float_t         trTpcdEdxExpPi[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
   Float_t         trTpcdEdxExpK[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
   Float_t         trTpcdEdxSigmaMu[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
   Float_t         trTpcdEdxSigmaEle[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
   Float_t         trTpcdEdxSigmaP[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
   Float_t         trTpcdEdxSigmaPi[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
   Float_t         trTpcdEdxSigmaK[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
   Float_t         trTpcPurity[NMAXPARTICLES][NMAXTPCS];   //[NParticles]
   
   Float_t         trFgdLength[NMAXPARTICLES][NMAXFGDS];   //[NParticles]
   Float_t         trFgdDirectionStart[NMAXPARTICLES][NMAXFGDS][3];   //[NParticles]
   Float_t         trFgdDirectionEnd[NMAXPARTICLES][NMAXFGDS][3];   //[NParticles]
   Float_t         trFgdPositionStart[NMAXPARTICLES][NMAXFGDS][4];   //[NParticles]
   Float_t         trFgdPositionEnd[NMAXPARTICLES][NMAXFGDS][4];   //[NParticles]
   Float_t         trFgdX[NMAXPARTICLES][NMAXFGDS];   //[NParticles]
   Float_t         trFgdE[NMAXPARTICLES][NMAXFGDS];   //[NParticles]
   Float_t         trFgdPullmu[NMAXPARTICLES][NMAXFGDS];   //[NParticles]
   Float_t         trFgdPullp[NMAXPARTICLES][NMAXFGDS];   //[NParticles]
   Float_t         trFgdPullpi[NMAXPARTICLES][NMAXFGDS];   //[NParticles]
   Float_t         trFgdPullno[NMAXPARTICLES][NMAXFGDS];   //[NParticles]
   Float_t         trFgdAvgTime[NMAXPARTICLES][NMAXFGDS];   //[NParticles]
   Float_t         trFgdVertex1x1[NMAXPARTICLES][NMAXFGDS];   //[NParticles]
   Float_t         trFgdVertex3x3[NMAXPARTICLES][NMAXFGDS];   //[NParticles]
   Float_t         trFgdVertex5x5[NMAXPARTICLES][NMAXFGDS];   //[NParticles]
   Float_t         trFgdVertex7x7[NMAXPARTICLES][NMAXFGDS];   //[NParticles]
   Float_t         trFgdVertexLayer[NMAXPARTICLES][NMAXFGDS];   //[NParticles]
   
   Float_t         trECALLength[NMAXPARTICLES][NECALSUBDETS];   //[NParticles]
   Float_t         trECALDirectionStart[NMAXPARTICLES][NECALSUBDETS][3];   //[NParticles]
   Float_t         trECALDirectionEnd[NMAXPARTICLES][NECALSUBDETS][3];   //[NParticles]
   Float_t         trECALPositionStart[NMAXPARTICLES][NECALSUBDETS][4];   //[NParticles]
   Float_t         trECALPositionEnd[NMAXPARTICLES][NECALSUBDETS][4];   //[NParticles]
   Float_t         trECALEMEnergy[NMAXPARTICLES][NECALSUBDETS];   //[NParticles]
   Float_t         trECALEDeposit[NMAXPARTICLES][NECALSUBDETS];   //[NParticles]
   Float_t         trECALAvgTime[NMAXPARTICLES][NECALSUBDETS];   //[NParticles]
   Float_t         trECALPIDMipEm[NMAXPARTICLES][NECALSUBDETS];  //[NParticles]
   Float_t         trECALPIDEmHip[NMAXPARTICLES][NECALSUBDETS];    //[NParticles]
   Float_t         trECALShowerPosition[NMAXPARTICLES][NECALSUBDETS][3];   //[NParticles]
  
  
   Float_t         trSMRDLength[NMAXPARTICLES][NSMRDSUBDETS];   //[NParticles]
   Float_t         trSMRDDirectionStart[NMAXPARTICLES][NSMRDSUBDETS][3];   //[NParticles]
   Float_t         trSMRDDirectionEnd[NMAXPARTICLES][NSMRDSUBDETS][3];   //[NParticles]
   Float_t         trSMRDPositionStart[NMAXPARTICLES][NSMRDSUBDETS][4];   //[NParticles]
   Float_t         trSMRDPositionEnd[NMAXPARTICLES][NSMRDSUBDETS][4];   //[NParticles]
   Float_t         trSMRDAvgTime[NMAXPARTICLES][NSMRDSUBDETS];   //[NParticles]
   
   Float_t         trP0DLength[NMAXPARTICLES][1];   //[NParticles]
   Float_t         trP0DELoss[NMAXPARTICLES][1];   //[NParticles]
   Float_t         trP0DDirectionStart[NMAXPARTICLES][1][3];   //[NParticles]
   Float_t         trP0DDirectionEnd[NMAXPARTICLES][1][3];   //[NParticles]
   Float_t         trP0DPositionStart[NMAXPARTICLES][1][4];   //[NParticles]
   Float_t         trP0DPositionEnd[NMAXPARTICLES][1][4];   //[NParticles]
  
   // List of branches
   TBranch        *b_sPOTSincePreviousSpill;   //!
   TBranch        *b_sRun;   //!
   TBranch        *b_sSubrun;   //!
   TBranch        *b_sEvt;   //!
   TBranch        *b_sEventTime;   //!
   TBranch        *b_sNTotalTrueParticles;   //!
   TBranch        *b_sNTotalTrueVertices;   //!
   TBranch        *b_sBeamGoodSpill;   //!
   TBranch        *b_sDQGoodDaq;   //!

   TBranch        *b_sNTrueVertices;   //!
   TBranch        *b_sTrueVertexID;   //!
   TBranch        *b_sTrueVertexRooVtxIndex;   //!
   TBranch        *b_sTrueVertexRooVtxEntry;   //!
   TBranch        *b_sTrueVertexReacCode;   //!
   TBranch        *b_sTrueVertexNTrueParticles;   //!
   TBranch        *b_sTrueVertexNReconTracks;   //!
   TBranch        *b_sTrueVertexNReconVertices;   //!
   TBranch        *b_sTrueVertexNBaryons;   //!
   TBranch        *b_sTrueVertexBunch;   //!
   TBranch        *b_sTrueVertexDetector;   //!
   TBranch        *b_sTrueVertexNuPDG;   //!
   TBranch        *b_sTrueVertexTargetPDG;   //!
   TBranch        *b_sTrueVertexNuEnergy;   //!
   TBranch        *b_sTrueVertexQ2;   //!
   TBranch        *b_sTrueVertexPosition;   //!
   TBranch        *b_sTrueVertexNuParentPDG;   //!
   TBranch        *b_sTrueVertexNuParentDecPoint;   //!
   TBranch        *b_sTrueVertexNuDir;   //!
   TBranch        *b_sNTrueParticles;   //!
   TBranch        *b_sTrueParticleID;   //!
   TBranch        *b_sTrueParticleBunch;   //!
   TBranch        *b_sTrueParticlePDG;   //!
   TBranch        *b_sTrueParticleParentPDG;   //!
   TBranch        *b_sTrueParticleGParentPDG;   //!
   TBranch        *b_sTrueParticlePrimaryID;   //!
   TBranch        *b_sTrueParticleParentID;   //!
   TBranch        *b_sTrueParticleGParentID;   //!
   TBranch        *b_sTrueParticlePurity;   //!
   TBranch        *b_sTrueParticleMomentum;   //!
   TBranch        *b_sTrueParticleCharge;   //!
   TBranch        *b_sTrueParticleVertexIndex;   //!
   TBranch        *b_sTrueParticleTruthVertexID;   //!
   TBranch        *b_sTrueParticleNReconTracks;   //!
   TBranch        *b_sTrueParticlePosition;   //!
   TBranch        *b_sTrueParticlePositionEnd;   //!
   TBranch        *b_sTrueParticleDirection;   //!
   TBranch        *b_sTrueParticleNDetCrossings;
   TBranch        *b_sTrueParticleEntrancePosition;
   TBranch        *b_sTrueParticleExitPosition;
   TBranch        *b_sTrueParticleEntranceMomentum;
   TBranch        *b_sTrueParticleExitMomentum;
   TBranch        *b_sTrueParticleDetector;
   TBranch        *b_sTrueParticleInActive;
   TBranch        *b_sTrueParticleIsTruePrimaryPi0DecayPhoton; //!
   TBranch        *b_sTrueParticleIsTruePrimaryPi0DecayPhotonChild; //!
   TBranch        *b_sNFgdTimeBins;   //!
   TBranch        *b_sFgdTimeBinMinTime;   //!
   TBranch        *b_sFgdTimeBinMaxTime;   //!
   TBranch        *b_sFgdTimeBinNHits1;   //!
   TBranch        *b_sFgdTimeBinNHits2;   //!
   TBranch        *b_sFgdTimeBinRawChargeSum1;   //!
   TBranch        *b_sFgdTimeBinRawChargeSum2;   //!
   TBranch        *b_sFgdTimeBinG4ID;   //!
   TBranch        *b_Bunch;   //!
   TBranch        *b_Weight;   //!
   TBranch        *b_NVertices;   //!
   TBranch        *b_vPrimaryIndex;   //!
   TBranch        *b_vBunch;   //!
   TBranch        *b_vPosition;   //!
   TBranch        *b_vVariance;   //!
   TBranch        *b_vChi2;   //!
   TBranch        *b_vNDOF;   //!
   TBranch        *b_vNReconParticles;   //!
   TBranch        *b_vNTrueVerticesMatch;   //!
   TBranch        *b_vParticlesUniqueID;   //!
   TBranch        *b_vTrueVertexIndex;   //!
   TBranch        *b_vTrueVerticesClean;   //!
   TBranch        *b_vTrueVerticesCompl;   //!
   TBranch        *b_NParticles;   //!
   TBranch        *b_trUniqueID;   //!
   TBranch        *b_trStatus;   //!
   TBranch        *b_trBunch;   //!
   TBranch        *b_trNReconVertices;   //!
   TBranch        *b_trDetectors;   //!
   TBranch        *b_trDetUsed;   //!
   TBranch        *b_trNTPCs;   //!
   TBranch        *b_trNFGDs;   //!
   TBranch        *b_trNECALs;   //!
   TBranch        *b_trNSMRDs;   //!
   TBranch        *b_trNP0Ds;   //!
   TBranch        *b_trNHits;   //!
   TBranch        *b_trNNodes;   //!
   TBranch        *b_trNDOF;   //!
   TBranch        *b_trChi2;   //!
   TBranch        *b_trCharge;   //!
   TBranch        *b_trMomentum;   //!
   TBranch        *b_trToFFGD1_FGD2; //!
   TBranch        *b_trToFP0D_FGD1; //!
   TBranch        *b_trToFDSECal_FGD1; //!
   TBranch        *b_trToFECal_FGD1; //!
   TBranch        *b_trToFDSECal_FGD2; //!
   TBranch        *b_trToFECal_FGD2; //!
   TBranch        *b_trToFFlag_FGD1_FGD2; //!
   TBranch        *b_trToFFlag_P0D_FGD1; //!
   TBranch        *b_trToFFlag_DSECal_FGD1; //!
   TBranch        *b_trToFFlag_ECal_FGD1; //!
   TBranch        *b_trToFFlag_DSECal_FGD2; //!
   TBranch        *b_trToFFlag_ECal_FGD2; //!
   TBranch        *b_trMomentumError;   //!
   TBranch        *b_trMomentumMuon;   //!
   TBranch        *b_trMomentumErrorMuon;   //!
   TBranch        *b_trMomentumProton;   //!
   TBranch        *b_trMomentumErrorProton;   //!
   TBranch        *b_trMomentumEle;   //!
   TBranch        *b_trMomentumErrorEle;   //!
   TBranch        *b_trRangeMomentumEle;   //!
   TBranch        *b_trRangeMomentumMuon;   //!
   TBranch        *b_trRangeMomentumProton;   //!
   TBranch        *b_trDirectionStart;   //!
   TBranch        *b_trDirectionEnd;   //!
   TBranch        *b_trPositionStart;   //!
   TBranch        *b_trPositionEnd;   //!
   TBranch        *b_trMomentumFlip;   //!
   
   TBranch        *b_trTpcDetector;   //!
   TBranch        *b_trTpcNHits;   //!
   TBranch        *b_trTpcNNodes;   //!
   TBranch        *b_trTpcLength;   //!
   TBranch        *b_trTpcDirectionStart;   //!
   TBranch        *b_trTpcDirectionEnd;   //!
   TBranch        *b_trTpcPositionStart;   //!
   TBranch        *b_trTpcPositionEnd;   //!
   TBranch        *b_trTpcCharge;   //!
   TBranch        *b_trTpcMomentum;   //!
   TBranch        *b_trTpcMomentumError;   //!
   TBranch        *b_trTpcBackMomentum;   //!
   TBranch        *b_trTpcRefitCharge;
   TBranch        *b_trTpcRefitMomentum;
   TBranch        *b_trTpcRefitPosition;
   TBranch        *b_trTpcRefitDirection;
   TBranch        *b_trTpcEFieldRefitMomentum;
   TBranch        *b_trTpcdEdxMeas;   //!
   TBranch        *b_trTpcdEdxExpMu;   //!
   TBranch        *b_trTpcdEdxExpEle;   //!
   TBranch        *b_trTpcdEdxExpP;   //!
   TBranch        *b_trTpcdEdxExpPi;   //!
   TBranch        *b_trTpcdEdxExpK;   //!
   TBranch        *b_trTpcdEdxSigmaMu;   //!
   TBranch        *b_trTpcdEdxSigmaEle;   //!
   TBranch        *b_trTpcdEdxSigmaP;   //!
   TBranch        *b_trTpcdEdxSigmaPi;   //!
   TBranch        *b_trTpcdEdxSigmaK;   //!
   TBranch        *b_trTpcPurity;   //!
   TBranch        *b_trFgdDetector;   //!
   TBranch        *b_trFgdNHits;   //!
   TBranch        *b_trFgdNNodes;   //!
   TBranch        *b_trFgdLength;   //!
   TBranch        *b_trFgdDirectionStart;   //!
   TBranch        *b_trFgdDirectionEnd;   //!
   TBranch        *b_trFgdPositionStart;   //!
   TBranch        *b_trFgdPositionEnd;   //!
   TBranch        *b_trFgdX;   //!
   TBranch        *b_trFgdE;   //!
   TBranch        *b_trFgdPullmu;   //!
   TBranch        *b_trFgdPullp;   //!
   TBranch        *b_trFgdPullpi;   //!
   TBranch        *b_trFgdPullno;   //!
   TBranch        *b_trFgdContainment;   //!
   TBranch        *b_trFgdAvgTime;   //!
   TBranch        *b_trFgdVertex1x1;   //!
   TBranch        *b_trFgdVertex3x3;   //!
   TBranch        *b_trFgdVertex5x5;   //!
   TBranch        *b_trFgdVertex7x7;   //!
   TBranch        *b_trFgdVertexLayer;   //!
   
   TBranch        *b_trECALDetector;   //!
   TBranch        *b_trECALNHits;   //!
   TBranch        *b_trECALNNodes;   //!
   TBranch        *b_trECALLength;   //!
   TBranch        *b_trECALDirectionStart;   //!
   TBranch        *b_trECALDirectionEnd;   //!
   TBranch        *b_trECALPositionStart;   //!
   TBranch        *b_trECALPositionEnd;   //!
   TBranch        *b_trECALEMEnergy;   //!
   TBranch        *b_trECALEDeposit;   //!
   TBranch        *b_trECALIsShowerLike;   //!
   TBranch        *b_trECALAvgTime;   //!
   TBranch        *b_trECALPIDMipEm; //!
   TBranch        *b_trECALPIDEmHip; //!
   TBranch        *b_trECALMostUpStreamLayerHit; //!
   TBranch        *b_trECALShowerPosition; //!
   
   TBranch        *b_trSMRDDetector;   //!
   TBranch        *b_trSMRDNHits;   //!
   TBranch        *b_trSMRDNNodes;   //!
   TBranch        *b_trSMRDLength;   //!
   TBranch        *b_trSMRDDirectionStart;   //!
   TBranch        *b_trSMRDDirectionEnd;   //!
   TBranch        *b_trSMRDPositionStart;   //!
   TBranch        *b_trSMRDPositionEnd;   //!
   TBranch        *b_trSMRDAvgTime;   //!
   
   TBranch        *b_trP0DDetector;   //!
   TBranch        *b_trP0DNHits;   //!
   TBranch        *b_trP0DNNodes;   //!
   TBranch        *b_trP0DLength;   //!
   TBranch        *b_trP0DELoss;   //!
   TBranch        *b_trP0DDirectionStart;   //!
   TBranch        *b_trP0DDirectionEnd;   //!
   TBranch        *b_trP0DPositionStart;   //!
   TBranch        *b_trP0DPositionEnd;   //!
   TBranch        *b_trTrueParticleID; //!
   TBranch        *b_trTpcTrueParticleID; //!
};

#endif


