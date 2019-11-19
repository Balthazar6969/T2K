#ifndef nueCCAnalysis_h
#define nueCCAnalysis_h

#include "nueCCSelection.hxx"

#include "ND280AnalysisUtils.hxx"
#include "baseTrackerAnalysis.hxx"

class nueCCAnalysis: public baseTrackerAnalysis {
 public:

  nueCCAnalysis(AnalysisAlgorithm* ana=NULL);
  virtual ~nueCCAnalysis(){}

  //---- These are mandatory functions
  void DefineSelections();
  void DefineCorrections();
  void DefineMicroTrees(bool addBase=true);
  void DefineTruthTree();

  void FillMicroTrees(bool addBase=true);
  void FillTruthVars();
  void FillToyVarsInMicroTrees(bool addBase=true);

  bool CheckFillTruthTree(const AnaTrueVertex& vtx);

  using baseAnalysis::FillTruthTree;
  void FillTruthTree(const AnaTrueVertex& vtx);
  using baseTrackerAnalysis::FillTruthTreeBase;
  void FillTruthTreeBase(const AnaTrueVertex& vtx, bool IsAntinu = false);
  //------------------------------------

  void FillConfigTree();
  void FillCategories();
  bool Initialize();

  void FillTrackVars();
  void FillFirstTPCVars();
  void FillSecondTPCVars();
  void FillECalVars();
  void FillTPCVetoVars();
  void FillPairFinderVars();
  void FillP0DVetoVars();
  void FillECalVetoVars();
  void FillEcalNeutralClusterVars();
  void FillAllTracksVars();
  void FillFGDVars();
  void FillToFVars();

  void SetSaveSecondaries(bool save) {_savesecondaries = save;}
  void SetSaveGammaInitiator(bool save) {_savegammainitiator = save;}

  const ToyBoxNueCC& nuebox(){return *static_cast<const ToyBoxNueCC*>(&box());}

  AnaTrueParticleB* GoBackToPrimaryParticle(const AnaTrueParticleB* Gamma,
					    const AnaTrueVertex *vtx);
  
private:
  Int_t _whichFGD; // 1 for FGD1 analysis, 2 for FGD2, 3 for both

  bool _savesecondaries; // Save secondary tracks
  bool _addmoretoyvariables; // Extend the list of toy variables

  bool _savegammainitiator;

 public:

  enum enumStandardMicroTrees_nueCCAnalysis{

    selelec_mom = enumStandardMicroTreesLast_baseTrackerAnalysis+1,
    selelec_costheta,
    selelec_costheta_z, 
    selelec_costheta_offaxis,
    selelec_charge,
    selelec_phi,
    selelec_pt,
    selelec_startpos, 
    selelec_startdir,
    selelec_momErr,
    selelec_momEleFit, 
    selelec_momMuonFit,
    selelec_ElemomECalEntrance,
    selelec_tpcNNodes,
    selelec_FGD,
    selelec_ENuRec,
    selelec_Q2Rec,
    selelec_ElemomTPCBack,
    selelec_tpcBackPos,
    selelec_tpcBackDir,
    selelec_inp0d,
    
    selelec_nfgds,
    selelec_fgd_det,
    selelec_fgd_x,
    selelec_fgd_E,
    selelec_fgd_V11,
    selelec_fgd_V33,
    selelec_fgd_V55,
    selelec_fgd_V77,
    selelec_fgd_VLayer,
    selelec_fgd_pullmu,
    selelec_fgd_pullpi,
    selelec_fgd_pullprot,
    selelec_fgd_pullno,
    selelec_fgd_containment,

    selelec_tpcdedx,
    selelec_pullmuon,
    selelec_pullelec,
    selelec_pullpion,
    selelec_pullprot,
    selelec_pullmuon2nd,
    selelec_pullelec2nd,

    selelec_nECALs,
    selelec_ecaldetector,
    selelec_ecalnhits,
    selelec_ecalMostUpStreamLayerHit,
    selelec_ecalmipem,
    selelec_ecalmippion,
    selelec_ecalemhip,
    selelec_ecalemene,
    selelec_ecalcontained,
    selelec_ecalshowerpos,
    selelec_ecallength,

    selelec_ToF_P0D_FGD1,
    selelec_ToF_ECal_FGD1,
    selelec_ToF_FGD1_FGD2,
    selelec_ToF_ECal_FGD2,
    selelec_ToF_DsECal_FGD1,
    selelec_ToF_DsECal_FGD2,
	
    selelec_true_pdg,    
    selelec_true_det,
    selelec_true_costheta_z,  
    selelec_true_costheta_offaxis,
    selelec_true_costheta_nuDir,  
    selelec_true_mom, 
    selelec_true_pos,
    selelec_true_endpos,
    selelec_true_dir,

    truelepton_pdg,
    truelepton_costheta_z,
    truelepton_costheta_offaxis,
    truelepton_costheta_nuDir,
    truelepton_pos, 
    truelepton_dir,
    truelepton_det,
    truelepton_nuErecQE,
   
    TPCVetoDeltaz,
    TPCVetoMomentum,
    TPCVetoCharge,
    TPCVetoNNearP0DTracks,
    TPCVetoNNearTracks,
    TPCVetoNFarTracks,
    NP0DVetoP0DTracks,
    NP0DVetoFGD1Tracks,
    NP0DVetoP0DEcalTracks,
    ECALMinZ,
    ECALNCMinZ,

    NFGDTracksReqTPC,
    NFGD1TracksReqTPC1,
    NFGD1TracksReqTPC2,
    NFGD2TracksReqTPC2,
    NFGD2TracksReqTPC3,
    NFGD1TracksReqEcal,
    NFGD2TracksReqEcal,
    FGDDistanceReqTpc,
    FGDMaxDistanceReqTpc,
    FGDDistanceNoReqTpc,
    FGDMaxDistanceNoReqTpc,
    FGD2XYMaxTrackDist,
    NTPCTracksReqEcal,
    FGD2ShowerNFGD1TPC2Tracks,
    FGD2ShowerNFGD2TPC3Tracks,
    FGDOOFVActivity,
    NFGDOOFVTracksReqTPC,
    SecMomFGDDeltaZ,

    PairTrack_mom,
    PairTrack_costheta,
    PairTrack_pdg,
    PairTrack_InvMass,
    PairTrack_charge,
    PairTrack_DistToMainTrack,
    PairTrack_startdir,
    PairTrack_startpos,
    PairTrack_true_startdir,
    PairTrack_true_startpos,
    PairTrack_tpcNNodes,
    PairTrack_pullmuon,
    PairTrack_pullelec,
    PairTrack_pullpion,
    PairTrack_pullprot,
    PairTrack_gamma_dir,
    PairTrack_gamma_true_dir,

    TruePairTrack_true_mom,
    TruePairTrack_pdg,
    selelec_parent_true_mom,
    selelec_parent_pdg,
    selelec_parent_id,
    selelec_gparent_true_mom,
    selelec_gparent_pdg,
    selelec_gparent_id,
    selelec_true_nbrem,
    selelec_true_brem_mom,
    selelec_true_brem_detector,
 
    NisoFgds,
    fgdiso_det,
    fgdiso_x,
    fgdiso_E,
    fgdiso_pullmu,
    fgdiso_pullpi,
    fgdiso_pullprot,
    fgdiso_containment,
    fgdiso_position,
    fgdiso_costheta,
    fgdiso_pdg,
    
    NECalTracks,
    NDsEcalNeutralClusters,
    NBrEcalNeutralClusters,
    MostEnergeticEcalNeutralCluster,
    EcalNeutralClusterEnergySum,
    EcalClusterMostEnergeticThanHMT,
    BremNeutralClusters,
    BremNeutralClustersEnergy,
    BremNeutralClustersMinTheta,
    BremNeutralClustersMinPhi,
    FragmentsNeutralClusters,
    FragmentsDistanceToShower,
    TPCECALdist,
    ECALPosECALSegment,
    ECALPosGlobalTrack,
    ECALTrueVertexPos,
    
    NumomTrue,
    Q2_true,
    NuParentPDGRaw,
    TargetPDG,
    NuParentDecPoint,
    NTruePions,
    NTruePi0,
    NTrueKaonRhoEta,

    InitiatorPDG,      
    InitiatorParentPDG,
    InitiatorMom,
    InitiatorDir,
    
    Nfgdtpc,
    FGDTPCTrack_det,
    FGDTPCTrack_mom,
    FGDTPCTrack_costheta,
    FGDTPCTrack_charge,
    FGDTPCTrack_startpos,
    FGDTPCTrack_startdir,
    FGDTPCTrack_pullmuon,
    FGDTPCTrack_pullelec,
    FGDTPCTrack_pullpion,
    FGDTPCTrack_pullprot,
    FGDTPCTrack_tpcNNodes,
    FGDTPCTrack_pdg,
    FGDTPCTrack_MomECalEntrance,
    FGDTPCTrack_EMEnergy,
    FGDTPCTrack_MipEm,
    FGDTPCTrack_MipPion,
    FGDTPCTrack_EmHip,
    FGDTPCTrack_ShowerPosition,
    FGDTPCTrack_ECALdet,
    FGDTPCTrack_NHits,
    FGDTPCTrack_MostUpStreamLayerHit,
    FGDTPCTrack_IsEcalContained,

    // (for xsTool)
    truevtx_mass_component,
    nue_signal,
    nue_ccqe,
    nue_ccnonqe,
    nue_cc0pi,
    nue_ccother,
  
    enumStandardMicroTreesLast_nueCCAnalysis
  };

  enum enumConfigTree_nueCCAnalysis{
    nNucleonsFGD1 = enumConfigTreeLast_AnalysisAlgorithm+1,
    nNucleonsFGD2scint,
    nNucleonsFGD2water,
    enumConfigTreeLast_nueCCAnalysis
  };

};



#endif
