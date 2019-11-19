#ifndef numuCCMultiPiAnalysis_h
#define numuCCMultiPiAnalysis_h

#include "numuCCAnalysis.hxx"
#include "numuCCMultiPiSelection.hxx"
#include "baseTrackerAnalysis.hxx"
#include "PionInteractionSystematic.hxx"

class numuCCMultiPiAnalysis: public baseTrackerAnalysis {
 public:

  numuCCMultiPiAnalysis(AnalysisAlgorithm* ana=NULL);
  virtual ~numuCCMultiPiAnalysis(){}

  //---- These are mandatory functions

  void DefineSelections();
  void DefineCorrections(){_numuCCAnalysis->DefineCorrections();}
  void DefineMicroTrees(bool addBase=false);
  void DefineTruthTree();

  void FillCategories(){_numuCCAnalysis->FillCategories();}

  void FillMicroTrees(bool addBase=false);
  void FillToyVarsInMicroTrees(bool addBase=false);

  bool CheckFillTruthTree(const AnaTrueVertex& vtx);

  using baseTrackerAnalysis::FillTruthTree;
  void FillTruthTree(const AnaTrueVertex& vtx);

  //--------------------

  bool Initialize();

  const ToyBoxCCMultiPi& mybox(){return *static_cast<const ToyBoxCCMultiPi*>(&box());}

  Int_t FindTPCTracks(const ToyBoxB& box, SubDetId::SubDetEnum det, AnaTrackB** TPCtracks);

private:
  Int_t _whichFGD; // 1 for FGD1 analysis, 2 for FGD2, 3 for both

  SubDetId::SubDetEnum _fgdID;

  bool _addPionSIinfo;
  
  /// Pion SI manager
  PionSIManager _pionSIManager;

protected:

  numuCCAnalysis* _numuCCAnalysis;

public:

  enum enumStandardMicroTrees_numuMultiPiAnalysis{
    HM_pos= numuCCAnalysis::enumStandardMicroTreesLast_numuCCAnalysis + 1,
    MuonVertexId,
    MuonIndex,
    NNegPion,
    NegPionMom,
    NegPionPidLik,
    NegPionElPidLik,
    NegPionTId,
    NegPionIndex,
    NegPionParentTId,
    NegPionGParentTId,
    NegPionVId,
    NegPionDir,
    NegPionPosStart,
    NegPionPosEnd,
    NPosPion,
    PosPionMom,
    PosPionTheta,
    PosPionPidLik,
    PosPionElPidLik,
    PosPionTId,
    PosPionIndex,
    PosPionParentTId,
    PosPionGParentTId,
    PosPionVId,
    PosPionDir,
    PosPionPosStart,
    PosPionPosEnd,
    NPi0El,
    Pi0ElMom,
    Pi0ElPull,
    Pi0ElTId,
    Pi0ElIndex,
    Pi0ElParentTId,
    Pi0ElGParentTId,
    Pi0ElVId,
    Pi0ElDir,
    Pi0ElPosStart,
    Pi0ElPosEnd,
    NPi0Pos,
    Pi0PosMom,
    Pi0PosPull,
    Pi0PosTId,
    Pi0PosIndex,
    Pi0PosParentTId,
    Pi0PosGParentTId,
    Pi0PosVId,
    Pi0PosDir,
    Pi0PosPosStart,
    Pi0PosPosEnd,
    NTPCSec,
    TPCSecMom,
    TPCSecPiPidLik,
    TPCSecMuPidLik,
    TPCSecPrPidLik,
    TPCSecElPidLik,
    TPCSecDetectors,
    TPCSecQ,
    TPCSecTId,
    TPCSecIndex,
    TPCSecParentTId,
    TPCSecGParentTId,
    TPCSecDir,
    TPCSecPosStart,
    TPCSecPosEnd,
    NFGDPi,
    FGDPiLength,
    FGDPiPiPull,
    FGDPiAvgTime,
    FGDPiDistance,
    FGDPiMuonangle,
    FGDPiCosTheta,
    FGDPiTId,
    FGDPiIndex,
    FGDPiParentTId,
    FGDPiGParentTId,
    FGDPiVId,
    FGDPiDir,
    FGDPiPosStart,
    FGDPiPosEnd,
    FGDPiTimeIni,
    FGDPiTimeEnd,
    FGDPiDeltaTimeIniSelMu,
    FGDPiDeltaTimeEndSelMu,
    NFGDEl,
    FGDElLength,
    FGDElPiPull,
    FGDElAvgTime,
    FGDElDistance,
    FGDElMuonangle,
    FGDElCosTheta,
    FGDElTId,
    FGDElIndex,
    FGDElParentTId,
    FGDElGParentTId,
    FGDElVId,
    FGDElDir,
    FGDElPosStart,
    FGDElPosEnd,
    FGDElTimeIni,
    FGDElTimeEnd,
    FGDElDeltaTimeIniSelMu,
    FGDElDeltaTimeEndSelMu,
    NFGDSec,
    FGDSecLength,
    FGDSecPiPull,
    FGDSecContained,
    FGDSecAvgTime,
    FGDSecCosTheta,
    FGDSecTId,
    FGDSecIndex,
    FGDSecParentTId,
    FGDSecGParentTId,
    FGDSecDir,
    FGDSecPosStart,
    FGDSecPosEnd,
    FGDSecTimeIni,
    FGDSecTimeEnd,
    FGDSecDeltaTimeIniSelMu,
    FGDSecDeltaTimeEndSelMu,
    NME,
    MENHits,
    MERawCharge,
    MEMinTime,
    MEMaxTime,
    NTPC1Track,
    TPC1TrackMom,
    TPC1TrackCosTheta,
    TPC1TrackPhi,
    TPC1TrackVId,
    TPC1TrackTId,
    TPC1TrackParentTId,
    TPC1TrackGParentTId,
    TPC1TrackPosStart,
    TPC1TrackPosEnd,
    IntType,
    NInts,
    PionType,
    PionMom,
    NPions,

    truepi_mom,
    truepi_dir,
    truepi_costheta,
    
    enumStandardMicroTreesLast_numuCCMultiPiAnalysis
  };
};

#endif
