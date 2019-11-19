#include "oaAnalysisTreeConverter.hxx"
#include "BasicUtils.hxx"
#include "HighlandAnalysisUtils.hxx"
#include "HighlandTreeConverterUtils.hxx"
#include "InputManager.hxx"
#include "Parameters.hxx"
#include "TreeConverterUtils.hxx"
#include "Versioning.hxx"

// FACTOR FOR SAND MC POT CALCULATION
const Float_t SANDFACTOR = 2.5e17;

// single gamma and 1pi Pauli-blocked vertices must be discarded from the truth
// tree
// but kept in the NRooTrackerVtx (see bugzilla 1011)
bool _discardPauliBlocked = true;
// unphysical coherent interaction on hydrogen are present in both prod 5 and 6
// because of a bug in neut (see bugzilla 1056)
bool _discardCohOnH = true;

// #define DEBUG

//****************************************************************
bool CompareNNodes(ND::TSubBaseObject* obj1, ND::TSubBaseObject* obj2) {
  //****************************************************************
  // decreasing nnodes order

  if (obj1 && obj2)
    return (obj1->NNodes > obj2->NNodes);
  else
    return false;
}

//********************************************************************
oaAnalysisTreeConverter::oaAnalysisTreeConverter()
    : InputConverter("ReconDir/Global") {
  //********************************************************************

  _firstFile = true;
  _firstEntry = true;
  _currentFileIndex = -1;

  _RooVtxEntryInCurrentInputFile = 0;

  TrECalObjects = NULL;
  TrECalUnmatched = NULL;
  P0DReconVertices = NULL;
  P0DReconParticles = NULL;
  P0DReconClusters = NULL;
  P0DReconTracks = NULL;
  P0DReconShowers = NULL;
  P0DReconNodes = NULL;
  P0DReconHits = NULL;
  PIDs = NULL;
  Vertices = NULL;
  DelayedClusters = NULL;
  TPCOthers = NULL;
  FgdTimeBins = NULL;
  TruthVertices = NULL;
  TruthTrajs = NULL;
  GVtx = NULL;
  NVtx = NULL;
  XZTracks_Radon2 = NULL;
  YZTracks_Radon2 = NULL;
  XYZTracks_Radon2 = NULL;
  XZTracksAllFGD_Radon2 = NULL;
  YZTracksAllFGD_Radon2 = NULL;
  XYZTracksAllFGD_Radon2 = NULL;
  VtxP0DECal = NULL;
  beamSummary = NULL;
  beamSummaryDataStatus = -1;
  _spill = NULL;

  AccPOTflg = false;

  cosmic_mode = false;
  NTruthTrajs = 0;
  NTruthVertices = 0;

  Genie = false;
  Neut = false;
  NeutFirstFile = false;
  GenieFirstFile = false;

  // Initialise DQ flags, which won't get read in for MC
  fND280OffFlag = 0;
  fTPCFlag = 0;
  fTPC1Flag = 0;
  fTPC2Flag = 0;
  fTPC3Flag = 0;
  fFGDFlag = 0;
  fFGD1Flag = 0;
  fFGD2Flag = 0;
  fECALFlag = 0;
  fDSECALFlag = 0;
  fBarECALFlag = 0;
  fP0DECALFlag = 0;
  fP0DFlag = 0;
  fSMRDFlag = 0;
  fMAGNETFlag = 0;
  fINGRIDFlag = 0;

  fCurrent = -999;
  Entries = -999;
  Counter = -999;
  EventID = -999;
  RunID = -999;
  SubrunID = -999;
  IsMC = false;
  Preselected = -999;
  EventTime = -999;
  TriggerWord = 0;
  FGDCosmicEvent = false;
  TripTCosmicEvent = false;
  for (int i = 0; i < 3; i++) CTMTriggerPattern[i] = 0;

  P0DWaterStatus = false;
  // SoftwareVersion[50];
  NTrECalObjects = -999;
  NP0DReconVertices = -999;
  NP0DReconParticles = -999;
  NP0DReconClusters = -999;
  NP0DReconTracks = -999;
  NP0DReconShowers = -999;
  NP0DReconNodes = -999;
  NP0DReconHits = -999;
  NPIDs = -999;
  NVertices = -999;
  NFGD1Unused = -999;
  NFGD2Unused = -999;
  NP0DUnused = -999;
  NTPCUnused = -999;
  NDelayedClusters = -999;
  NTPCOthers = -999;
  NNVtx = -999;
  NGVtx = -999;
  NFgdTimeBins = -999;
  NTruthVertices = -999;
  NVtxFGD1 = -999;
  NVtxFGD2 = -999;
  NTruthTrajs = -999;
  NXZTracks_Radon2 = -999;
  NYZTracks_Radon2 = -999;
  NXYZTracks_Radon2 = -999;
  NXZTracksAllFGD_Radon2 = -999;
  NYZTracksAllFGD_Radon2 = -999;
  NXYZTracksAllFGD_Radon2 = -999;
  ND280Spill = -999;
  fBarECALFlag = -999;
  fDSECALFlag = -999;
  fECALFlag = -999;
  fFGD1Flag = -999;
  fFGD2Flag = -999;
  fFGDFlag = -999;
  fINGRIDFlag = -999;
  fMAGNETFlag = -999;
  fND280OffFlag = -999;
  fP0DECALFlag = -999;
  fP0DFlag = -999;
  fSMRDFlag = -999;
  fTPC1Flag = -999;
  fTPC2Flag = -999;
  fTPC3Flag = -999;
  fTPCFlag = -999;

  for (int i = 0; i < 7; i++) {
    TruthTrajsName[i] = "";
  }
  for (int i = 0; i < 9; i++) {
    TruthVerticesName[i] = "";
  }

  _previousFile = "";
  _previousRunID = -1;
  _previousSubrunID = -1;
  _previousRefEventID = -1;

#if !VERSION_HAS_ECAL_LLR
  _ecalPidCalc = new ecalPid::TECALPidLikelihood();
  _ecalPidCalc->SetupAlgorithms();
#endif
}

//********************************************************************
bool oaAnalysisTreeConverter::Initialize() {
  //********************************************************************

  _fillFgdSingleHitsInfo = (bool)ND::params().GetParameterI(
      "highlandIO.oaAnalysis.FillFgdSingleHitsInfo");

  /// Use ReconDir/TrackerECal information?
  _isUsingReconDirFGDOnly =
      ND::params().GetParameterI("highlandIO.FlatTree.UseReconDirFGDOnly");
  _isUsingReconDirP0D =
      ND::params().GetParameterI("highlandIO.FlatTree.UseReconDirP0D");
  _isUsingReconDirPECAL =
      ND::params().GetParameterI("highlandIO.FlatTree.UseReconDirP0DECal");
  _isUsingReconDirTECAL =
      ND::params().GetParameterI("highlandIO.FlatTree.UseReconDirTrackerECal");

  // ---- TEST HIGHLAND2-P0D ----
  _isUsingReconDirP0DNew =
      ND::params().GetParameterI("highlandIO.P0DDataClasses.UseReconDirP0DNew");
  _p0dAlgoResName = ND::params().GetParameterS(
      "highlandIO.P0DDataClasses.UseReconDirP0DAlgoResult");
  _addGlobalTracksToP0DVertices = (bool)ND::params().GetParameterI(
      "highlandIO.P0DDataClasses.AddGlobalTracksToP0DVertices");

  if (_isUsingReconDirP0D && _isUsingReconDirP0DNew) {
    std::cerr << "ERROR. oaAnalysisTreeConverter::Initialize(). "
                 "'highlandIO.FlatTree.UseReconDirP0D' and "
                 "'highlandIO.P0DRecon.UseReconDirP0DNew' are both set to 1 "
              << "highlandIO.parameters.dat.  Please switch off one of two !!!!"
              << std::endl;
    exit(1);
  }
  // ----------------------------

  /// Ignore TrueVertices when all their true tracks are fully contained
  _ignoreSMRDContainedTrueObjects = ND::params().GetParameterI(
      "highlandIO.oaAnalysis.IgnoreSMRDContainedTrueObjects");
  _ignoreP0DECalContainedTrueObjects = ND::params().GetParameterI(
      "highlandIO.oaAnalysis.IgnoreP0DECalContainedTrueObjects");
  _ignoreDsECalContainedTrueObjects = ND::params().GetParameterI(
      "highlandIO.oaAnalysis.IgnoreDsECalContainedTrueObjects");
  _ignoreBrECalContainedTrueObjects = ND::params().GetParameterI(
      "highlandIO.oaAnalysis.IgnoreBrECalContainedTrueObjects");
  _ignoreINGRIDContainedTrueObjects = ND::params().GetParameterI(
      "highlandIO.oaAnalysis.IgnoreINGRIDContainedTrueObjects");

  _ForceIgnoreRunNumberClash = ND::params().GetParameterI(
      "highlandIO.oaAnalysis.ForceIgnoreRunNumberClash");
  _FixECalGlobalVerticesTime = ND::params().GetParameterI(
      "highlandIO.oaAnalysis.FixECalGlobalVerticesTime");

  // Remove uninteresting vertices
  _removeTrueVerticesWithNoTrueParticles = ND::params().GetParameterI(
      "highlandIO.oaAnalysis.RemoveTrueVerticesWithNoTrueParticles");

  AddChain("ReconDir/Global");
  AddChain("TruthDir/Vertices");
  AddChain("TruthDir/Trajectories");
  AddChain("HeaderDir/BeamSummaryData");
  AddChain("HeaderDir/BasicDataQuality");
  AddChain("HeaderDir/BasicHeader");

  if (_isUsingReconDirFGDOnly) AddChain("ReconDir/FGDOnly");
  if (_isUsingReconDirTECAL) AddChain("ReconDir/TrackerECal");
  if (_isUsingReconDirP0D || _isUsingReconDirP0DNew) AddChain("ReconDir/P0D");

  reconGlobal = GetChain("ReconDir/Global");
  mcTruthVertices = GetChain("TruthDir/Vertices");
  mcTruthTrajectories = GetChain("TruthDir/Trajectories");
  beamInfo = GetChain("HeaderDir/BeamSummaryData");
  DQInfo = GetChain("HeaderDir/BasicDataQuality");
  BasicHeader = GetChain("HeaderDir/BasicHeader");
  accPOTInfo = NULL;  // GetChain("HeaderDir/AccumulatedPOT");
  GRooTrackerVTX = NULL;
  NRooTrackerVTX = NULL;

  if (_isUsingReconDirFGDOnly)
    fgdOnly = GetChain("ReconDir/FGDOnly");
  else
    fgdOnly = NULL;
  if (_isUsingReconDirTECAL)
    trackerECAL = GetChain("ReconDir/TrackerECal");
  else
    trackerECAL = NULL;
  if (_isUsingReconDirP0D || _isUsingReconDirP0DNew)
    p0d = GetChain("ReconDir/P0D");
  else
    p0d = NULL;

  fChain = reconGlobal;

  // Set branch addresses and branch pointers

  if (!fChain) return false;
  fCurrent = -1;

  // ------------- TGlobalPIDs --------------------
  PIDs = new TClonesArray("ND::TGlobalReconModule::TGlobalPID", 20);
  reconGlobal->SetBranchAddress("NPIDs", &NPIDs);
  reconGlobal->SetBranchAddress("PIDs", &PIDs);

  FgdTimeBins = new TClonesArray("ND::TGlobalReconModule::TFgdTimeBin");
  reconGlobal->SetBranchAddress("NFgdTimeBins", &NFgdTimeBins);
  reconGlobal->SetBranchAddress("FgdTimeBins", &FgdTimeBins);

  Vertices = new TClonesArray("ND::TGlobalReconModule::TGlobalVertex", 20);
  reconGlobal->SetBranchAddress("NVertices", &NVertices);
  reconGlobal->SetBranchAddress("Vertices", &Vertices);

  /*
  DelayedClusters = new TClonesArray("ND::TGlobalReconModule::TFgdCluster",200);
  reconGlobal->SetBranchAddress("DelayedClusters",&DelayedClusters);
  reconGlobal->SetBranchAddress("NDelayedClusters",&NDelayedClusters);

  reconGlobal->SetBranchAddress("NFGD1Unused",&NFGD1Unused);
  reconGlobal->SetBranchAddress("NFGD2Unused",&NFGD2Unused);
  reconGlobal->SetBranchAddress("NP0DUnused",&NP0DUnused);
  reconGlobal->SetBranchAddress("NTPCUnused",&NTPCUnused);

  TPCOthers = new TClonesArray("ND::TGlobalReconModule::TTPCOtherObject",20);
  reconGlobal->SetBranchAddress("TPCOthers",&TPCOthers);
  reconGlobal->SetBranchAddress("NTPCOthers",&NTPCOthers);
  */

  if (trackerECAL) {
    TrECalObjects =
        new TClonesArray("ND::TTrackerECALReconModule::TECALReconObject", 20);
    trackerECAL->SetBranchAddress("NReconObject", &NTrECalObjects);
    trackerECAL->SetBranchAddress("ReconObject", &TrECalObjects);

    TrECalUnmatched = new TClonesArray(
        "ND::TTrackerECALReconModule::TECALReconUnmatchedObject", 20);
    trackerECAL->SetBranchAddress("NUnmatchedObject", &NTrECalUnmatched);
    trackerECAL->SetBranchAddress("UnmatchedObject", &TrECalUnmatched);
  }

  if (p0d) {
    // ---- TEST HIGHLAND2-P0D ----
    P0DAlgoResults = new TClonesArray("ND::TP0DReconModule::TP0DAlgoRes", 20);
    p0d->SetBranchAddress("NAlgoResults", &NP0DAlgoResults);
    p0d->SetBranchAddress("AlgoResults", &P0DAlgoResults);

    P0DReconVertices = new TClonesArray("ND::TP0DReconModule::TP0DVertex", 100);
    p0d->SetBranchAddress("NVertices", &NP0DReconVertices);
    p0d->SetBranchAddress("Vertices", &P0DReconVertices);

    P0DReconParticles =
        new TClonesArray("ND::TP0DReconModule::TP0DParticle", 100);
    p0d->SetBranchAddress("NParticles", &NP0DReconParticles);
    p0d->SetBranchAddress("Particles", &P0DReconParticles);

    P0DReconTracks = new TClonesArray("ND::TP0DReconModule::TP0DTrack", 100);
    p0d->SetBranchAddress("NTracks", &NP0DReconTracks);
    p0d->SetBranchAddress("Tracks", &P0DReconTracks);

    P0DReconShowers = new TClonesArray("ND::TP0DReconModule::TP0DShower", 100);
    p0d->SetBranchAddress("NShowers", &NP0DReconShowers);
    p0d->SetBranchAddress("Showers", &P0DReconShowers);

    P0DReconClusters =
        new TClonesArray("ND::TP0DReconModule::TP0DCluster", 100);
    p0d->SetBranchAddress("NClusters", &NP0DReconClusters);
    p0d->SetBranchAddress("Clusters", &P0DReconClusters);

    P0DReconNodes = new TClonesArray("ND::TP0DReconModule::TP0DNode", 100);
    p0d->SetBranchAddress("NNodes", &NP0DReconNodes);
    p0d->SetBranchAddress("Nodes", &P0DReconNodes);

    P0DReconHits = new TClonesArray("ND::TP0DReconModule::TP0DHit", 100);
    p0d->SetBranchAddress("NHits", &NP0DReconHits);
    p0d->SetBranchAddress("Hits", &P0DReconHits);
    // ------------------------------
  }

  //--------------- True vertices -------------------
  if (mcTruthVertices) {
    TruthVertices =
        new TClonesArray("ND::TTruthVerticesModule::TTruthVertex", 100);
    mcTruthVertices->SetBranchAddress("NVtx", &NTruthVertices);
    mcTruthVertices->SetBranchAddress("NVtxFGD1", &NVtxFGD1);
    mcTruthVertices->SetBranchAddress("NVtxFGD2", &NVtxFGD2);
    mcTruthVertices->SetBranchAddress("Vertices", &TruthVertices);
  }

  //--------------- True Trajectories --------------
  if (mcTruthTrajectories) {
    TruthTrajs =
        new TClonesArray("ND::TTruthTrajectoriesModule::TTruthTrajectory", 100);
    mcTruthTrajectories->SetBranchAddress("NTraj", &NTruthTrajs);
    mcTruthTrajectories->SetBranchAddress("Trajectories", &TruthTrajs);
  }

  //------------- FGD information -----------------------
  if (fgdOnly) {
    XZTracks_Radon2 =
        new TClonesArray("ND::TFgdOnlyModule::TFgd2DIsoTrack", 200);
    fgdOnly->SetBranchAddress("NXZTracks_Radon2", &NXZTracks_Radon2);
    fgdOnly->SetBranchAddress("XZTracks_Radon2", &XZTracks_Radon2);

    YZTracks_Radon2 =
        new TClonesArray("ND::TFgdOnlyModule::TFgd2DIsoTrack", 200);
    fgdOnly->SetBranchAddress("NYZTracks_Radon2", &NYZTracks_Radon2);
    fgdOnly->SetBranchAddress("YZTracks_Radon2", &YZTracks_Radon2);

    XYZTracks_Radon2 =
        new TClonesArray("ND::TFgdOnlyModule::TFgd3DIsoTrack", 200);
    fgdOnly->SetBranchAddress("NXYZTracks_Radon2", &NXYZTracks_Radon2);
    fgdOnly->SetBranchAddress("XYZTracks_Radon2", &XYZTracks_Radon2);

    XZTracksAllFGD_Radon2 =
        new TClonesArray("ND::TFgdOnlyModule::TFgd2DIsoTrack", 200);
    fgdOnly->SetBranchAddress("NXZTracksAllFGD_Radon2",
                              &NXZTracksAllFGD_Radon2);
    fgdOnly->SetBranchAddress("XZTracksAllFGD_Radon2", &XZTracksAllFGD_Radon2);

    YZTracksAllFGD_Radon2 =
        new TClonesArray("ND::TFgdOnlyModule::TFgd2DIsoTrack", 200);
    fgdOnly->SetBranchAddress("NYZTracksAllFGD_Radon2",
                              &NYZTracksAllFGD_Radon2);
    fgdOnly->SetBranchAddress("YZTracksAllFGD_Radon2", &YZTracksAllFGD_Radon2);

    XYZTracksAllFGD_Radon2 =
        new TClonesArray("ND::TFgdOnlyModule::TFgd3DIsoTrack", 200);
    fgdOnly->SetBranchAddress("NXYZTracksAllFGD_Radon2",
                              &NXYZTracksAllFGD_Radon2);
    fgdOnly->SetBranchAddress("XYZTracksAllFGD_Radon2",
                              &XYZTracksAllFGD_Radon2);
  }

  //-------------- Beam summary --------------------
  if (beamInfo) {
    beamSummary =
        new TClonesArray("ND::TBeamSummaryDataModule::TBeamSummaryData", 200);
    beamInfo->SetBranchAddress("BeamSummaryData", &beamSummary);
    beamInfo->SetBranchAddress("BeamSummaryDataStatus", &beamSummaryDataStatus);
    beamInfo->SetBranchAddress("ND280Spill", &ND280Spill);
  }

  //-------------- Accumulated POT  -----------------------
  //  if( AccPOTflg && accPOTInfo) {
  //    accPOTInfo->SetBranchAddress("AccPOT", &skimaccpotbtwEvt);
  //  }

  //-------------- Header ---------------------------------
  if (BasicHeader) {
    BasicHeader->SetBranchAddress("EventID", &EventID);
    BasicHeader->SetBranchAddress("RunID", &RunID);
    BasicHeader->SetBranchAddress("SubrunID", &SubrunID);
    BasicHeader->SetBranchAddress("IsMC", &IsMC);
    BasicHeader->SetBranchAddress("EventTime", &EventTime);
    BasicHeader->SetBranchAddress("Preselected", &Preselected);
    BasicHeader->SetBranchAddress("TriggerWord", &TriggerWord);
    BasicHeader->SetBranchAddress("CTMTriggerPattern", CTMTriggerPattern);
    BasicHeader->SetBranchAddress("FGDCosmicEvent", &FGDCosmicEvent);
    BasicHeader->SetBranchAddress("TripTCosmicEvent", &TripTCosmicEvent);
    BasicHeader->SetBranchAddress("SoftwareVersion", &SoftwareVersion);
    BasicHeader->SetBranchAddress("P0DWaterStatus", &P0DWaterStatus);
#if VERSION_HAS_OFFICIAL_POT
    BasicHeader->SetBranchAddress("POTPerSpill", &POTPerSpill);
#endif
  }

  //------------ DataQuality summary ----------------------
  if (DQInfo) {
    DQInfo->SetBranchAddress("ND280OffFlag", &fND280OffFlag);
    DQInfo->SetBranchAddress("TPCFlag", &fTPCFlag);
    DQInfo->SetBranchAddress("TPC1Flag", &fTPC1Flag);
    DQInfo->SetBranchAddress("TPC2Flag", &fTPC2Flag);
    DQInfo->SetBranchAddress("TPC3Flag", &fTPC3Flag);
    DQInfo->SetBranchAddress("FGDFlag", &fFGDFlag);
    DQInfo->SetBranchAddress("FGD1Flag", &fFGD1Flag);
    DQInfo->SetBranchAddress("FGD2Flag", &fFGD2Flag);
    DQInfo->SetBranchAddress("ECALFlag", &fECALFlag);
    DQInfo->SetBranchAddress("DSECALFlag", &fDSECALFlag);
    DQInfo->SetBranchAddress("BarECALFlag", &fBarECALFlag);
    DQInfo->SetBranchAddress("P0DECALFlag", &fP0DECALFlag);
    DQInfo->SetBranchAddress("P0DFlag", &fP0DFlag);
    DQInfo->SetBranchAddress("SMRDFlag", &fSMRDFlag);
    DQInfo->SetBranchAddress("MAGNETFlag", &fMAGNETFlag);
    DQInfo->SetBranchAddress("INGRIDFlag", &fINGRIDFlag);
  }

  // Setup the TruthUtils
  _truthUtils.SetTrajectoriesArray(TruthTrajs);
  _truthUtils.SetVerticesArray(TruthVertices);
  _truthUtils.SetTrajectoriesNumber(&NTruthTrajs);
  _truthUtils.SetVerticesNumber(&NTruthVertices);

  return true;
}

//********************************************************************
oaAnalysisTreeConverter::~oaAnalysisTreeConverter() {
  //********************************************************************

  if (!fChain) return;

  if (reconGlobal) delete reconGlobal->GetCurrentFile();
  if (trackerECAL) delete trackerECAL->GetCurrentFile();
  if (p0d) delete p0d->GetCurrentFile();
  if (fgdOnly) delete fgdOnly->GetCurrentFile();
  if (mcTruthVertices) delete mcTruthVertices->GetCurrentFile();
  if (beamInfo) delete beamInfo->GetCurrentFile();
  if (DQInfo) delete DQInfo->GetCurrentFile();
  if (BasicHeader) delete BasicHeader->GetCurrentFile();
  if (mcTruthTrajectories) delete mcTruthTrajectories->GetCurrentFile();
  if (GRooTrackerVTX) delete GRooTrackerVTX->GetCurrentFile();
  if (NRooTrackerVTX) delete NRooTrackerVTX->GetCurrentFile();
  if (accPOTInfo) delete accPOTInfo->GetCurrentFile();

  delete PIDs;
  delete Vertices;
  delete DelayedClusters;
  delete TPCOthers;
  delete FgdTimeBins;
  delete TruthVertices;
  delete TruthTrajs;
  delete GVtx;
  delete NVtx;
  delete TrECalObjects;
  delete TrECalUnmatched;
  delete P0DReconVertices;
  delete P0DReconParticles;
  delete P0DReconTracks;
  delete P0DReconShowers;
  delete P0DReconClusters;
  delete P0DReconNodes;
  delete P0DReconHits;
  delete XZTracks_Radon2;
  delete YZTracks_Radon2;
  delete XYZTracks_Radon2;
  delete XZTracksAllFGD_Radon2;
  delete YZTracksAllFGD_Radon2;
  delete XYZTracksAllFGD_Radon2;
  delete VtxP0DECal;
  delete beamSummary;
}

//****************************************************************************
bool oaAnalysisTreeConverter::AddFileToTChain(const std::string& inputString) {
  //****************************************************************************

  std::cout << "oaAnalysisTreeConverter::AddFileToTChain(). Adding file: "
            << inputString << std::endl;

  // SKIP FILE WHEN BASIC HEADER HAS 0 ENTRIES
  TChain BasicHeaderForFile("HeaderDir/BasicHeader");
  BasicHeaderForFile.AddFile(inputString.c_str());
  if (BasicHeaderForFile.GetEntries() == 0) {
    std::cout
        << "      ----> This file does not contain any entries. IGNORED !!!!"
        << std::endl;
    return true;
  }

  // Read one entry from the BasicHeaderForFile such that IsMC is available
  BasicHeaderForFile.SetBranchAddress("IsMC", &IsMC);
  BasicHeaderForFile.GetEntry(0);

  AccPOTflg = false;
  Genie = Neut = false;
  if (IsMC) {
    TFile* f = TFile::Open(inputString.c_str());
    f->cd();

    gDirectory->cd("/TruthDir");
    if (gDirectory->FindObjectAny("NRooTrackerVtx"))
      Neut = true;
    else if (gDirectory->FindObjectAny("GRooTrackerVtx"))
      Genie = true;

    if (_firstFile) {
      _firstFile = false;

      NeutFirstFile = Neut;
      GenieFirstFile = Genie;

      //    gDirectory->cd("/HeaderDir");
      //    if( gDirectory->FindObjectAny("AccumulatedPOT") ) AccPOTflg  = true;

      //------------- Genie and Neut TRacker Vertex  ---------------------
      // These can only be initialised once we know the file type.
      if (Neut) {
        AddChain("NRooTrackerVtx", "TruthDir/NRooTrackerVtx");
        NRooTrackerVTX = GetChain("NRooTrackerVtx");
        NVtx = new TClonesArray("ND::NRooTrackerVtx", 100);
        NRooTrackerVTX->SetBranchAddress("NVtx", &NNVtx);
        NRooTrackerVTX->SetBranchAddress("Vtx", &NVtx);
      } else if (Genie) {
        AddChain("GRooTrackerVtx", "TruthDir/GRooTrackerVtx");
        GRooTrackerVTX = GetChain("GRooTrackerVtx");
        GVtx = new TClonesArray("ND::GRooTrackerVtx", 100);
        GRooTrackerVTX->SetBranchAddress("NVtx", &NGVtx);
        GRooTrackerVTX->SetBranchAddress("Vtx", &GVtx);
      }

      // if( !Neut ) delete NRooTrackerVTX;
      // if( !Genie ) delete GRooTrackerVTX;

      if (Neut)
        std::cout << " Neut Branch " << std::endl;
      else if (Genie)
        std::cout << " Genie Branch " << std::endl;
    } else {
      if ((!Genie && !Neut) && (GenieFirstFile || NeutFirstFile)) {
        std::cout << "ERROR (see "
                     "https://bugzilla.nd280.org/show_bug.cgi?id=1372): This "
                     "file does not contain RooTrackerVtx Tree. IGNORED !!!!"
                  << std::endl;
        return true;
      }

      // STOP WHEN THE FIRST FILE DOES NOT CONTAIN RooTrackerVtx BUT SOME OTHER
      // DOES
      if ((Genie || Neut) && (!GenieFirstFile && !NeutFirstFile)) {
        std::cout << "ERROR (see "
                     "https://bugzilla.nd280.org/show_bug.cgi?id=1372): The "
                     "first file didn't contain RooTrackerVtx tree, while this "
                     "does. Please remove first file from list !!!!"
                  << std::endl;
        return false;
      }

      // STOP WHEN Neut and Genie FILES ARE MIXED ON THE LIST
      if ((Genie && NeutFirstFile)) {
        std::cout << "ERROR (see "
                     "https://bugzilla.nd280.org/show_bug.cgi?id=1372): This "
                     "is a Genie file. Neut and Genie files cannot be mixed "
                     "!!!!"
                  << std::endl;
        return false;
      }

      if ((Neut && GenieFirstFile)) {
        std::cout << "ERROR (see "
                     "https://bugzilla.nd280.org/show_bug.cgi?id=1372): This "
                     "is a Neut file. Neut and Genie files cannot be mixed !!!!"
                  << std::endl;
        return false;
      }
    }

    f->Close();
  }

  // Chain only the directories we are interested in

  if (reconGlobal) reconGlobal->AddFile(inputString.c_str());
  if (trackerECAL) trackerECAL->AddFile(inputString.c_str());
  if (fgdOnly) fgdOnly->AddFile(inputString.c_str());
  if (p0d) p0d->AddFile(inputString.c_str());
  if (mcTruthVertices) mcTruthVertices->AddFile(inputString.c_str());
  if (mcTruthTrajectories) mcTruthTrajectories->AddFile(inputString.c_str());
  if (beamInfo) beamInfo->AddFile(inputString.c_str());
  if (DQInfo) DQInfo->AddFile(inputString.c_str());
  if (BasicHeader) BasicHeader->AddFile(inputString.c_str());
  if (AccPOTflg && accPOTInfo) accPOTInfo->AddFile(inputString.c_str());
  if (Neut && NRooTrackerVTX) NRooTrackerVTX->AddFile(inputString.c_str());
  if (Genie && GRooTrackerVTX) GRooTrackerVTX->AddFile(inputString.c_str());

  // Read one entry from the BasicHeader tree such that RunID and SubrunID are
  // available
  BasicHeader->GetEntry(BasicHeader->GetEntries() - 1);

  // Make sure the current file has not the same run and subrun number as the
  // previous
  if (!_ForceIgnoreRunNumberClash &&
      (_previousRunID == RunID && _previousSubrunID == SubrunID &&
       _previousRefEventID >= EventID)) {
    std::cout << "-------------------------------------------------------------"
                 "------------------------------------------"
              << std::endl;
    std::cout << "oaAnalysisTreeConverter::AddFileToTChain(). Current file has "
                 "the same run and subrun as the previous"
              << std::endl;
    std::cout << "                                           and no higher "
                 "event number !!!"
              << std::endl;
    std::cout << "   - this file:     " << inputString << std::endl;
    std::cout << "   - previous file: " << _previousFile << std::endl;
    std::cout << "Please verify the input file list !!!" << std::endl;
    std::cout << "-------------------------------------------------------------"
                 "------------------------------------------"
              << std::endl;
    exit(1);
  }

  // The previous attributes
  _previousFile = inputString;
  _previousRunID = RunID;
  _previousSubrunID = SubrunID;
  _previousRefEventID = EventID;

  // IS SAND MC?
  bool issand = false;
  if (IsMC) {  // IsMC is also is the BasicHeader tree
    std::stringstream sRun;
    sRun << RunID;
    issand = ((sRun.str())[4] == '7');
  }

  if (IsMC) {
    Double_t NPOT = GetPOTFromRooTrackerVtx();
    bool bySpillInMC = false;
#if VERSION_HAS_OFFICIAL_POT
    bySpillInMC = true;
#endif

    if (issand)
      header().IncrementPOTByFile(NPOT * SANDFACTOR, bySpillInMC);
    else  // here already multiplied by 5e17
      header().IncrementPOTByFile(NPOT, bySpillInMC);
  }

  // Set the data/MC mode and return false when mixing data and MC files
  if (!header().SetIsMC(IsMC)) return false;

  // Sets the software version for this file
  return header().SetSoftwareVersion(SoftwareVersion);
}

//*****************************************************************************
Double_t oaAnalysisTreeConverter::GetPOTFromRooTrackerVtx() {
  //*****************************************************************************

  // ---- POT counting ------------------------------------
  Double_t NPOT = 0;

  Int_t ientry, ilastentry;
  if (GRooTrackerVTX) {
    // Get the POT for this file from the second last entry in the chain
    ilastentry = GRooTrackerVTX->GetEntries() - 2;
    ientry = ilastentry;
    do {
      GRooTrackerVTX->GetEntry(ientry--);
      ND::GRooTrackerVtx* gvtx = (ND::GRooTrackerVtx*)(*GVtx)[0];
      NPOT = gvtx->OrigTreePOT;
    } while (NPOT == 0 && ientry >= 0);
  } else if (NRooTrackerVTX) {
    // Get the POT for this file from the last entry in the chain
    ilastentry = NRooTrackerVTX->GetEntries() - 2;
    ientry = ilastentry;
    do {
      NRooTrackerVTX->GetEntry(ientry--);
      ND::NRooTrackerVtx* gvtx = (ND::NRooTrackerVtx*)(*NVtx)[0];
      NPOT = gvtx->OrigTreePOT;
    } while (NPOT == 0 && ientry >= 0);
  } else
    return NPOT;

  if (IsMC) {
    if (NPOT == 0) {
      std::cout << "WARNING: This file has 0 POT but it has some entries !!!!"
                << std::endl;
    } else if (ientry + 1 != ilastentry) {
      std::cout << "INFO: OrigTreePOT is 0 in the usual second last entry of "
                   "RooTrackerVtx ("
                << ilastentry << "), POT taken from entry " << ientry + 1
                << std::endl;
    }
  }
  return NPOT;
}

//*****************************************************************************
Int_t oaAnalysisTreeConverter::ReadEntries(Long64_t& entry) {
  //*****************************************************************************

  Int_t entry_temp = reconGlobal->GetEntry(entry);

  if (entry_temp > 0) {
    if (trackerECAL) trackerECAL->GetEntry(entry);
    if (p0d) p0d->GetEntry(entry);
    if (fgdOnly) fgdOnly->GetEntry(entry);
    if (BasicHeader) BasicHeader->GetEntry(entry);
    if (IsMC) {
      if (mcTruthVertices) mcTruthVertices->GetEntry(entry);
      if (mcTruthTrajectories) mcTruthTrajectories->GetEntry(entry);

      if (Genie && GRooTrackerVTX) GRooTrackerVTX->GetEntry(entry);

      if (Neut && NRooTrackerVTX) NRooTrackerVTX->GetEntry(entry);
    } else {
      if (beamInfo) beamInfo->GetEntry(entry);
      if (DQInfo) DQInfo->GetEntry(entry);
      if (AccPOTflg && accPOTInfo) accPOTInfo->GetEntry(entry);
    }
  }

  //-------- Increase the cache size to speed up reading Branches ----------
  if (_firstEntry) {
    if (reconGlobal) {
      reconGlobal->SetCacheSize(200000000);
      reconGlobal->AddBranchToCache("*", kTRUE);
    }
    if (mcTruthTrajectories && IsMC) {
      mcTruthTrajectories->SetCacheSize(200000000);
      mcTruthTrajectories->AddBranchToCache("*", kTRUE);
    }
    if (mcTruthVertices && IsMC) {
      mcTruthVertices->SetCacheSize(200000000);
      mcTruthVertices->AddBranchToCache("*", kTRUE);
    }
    if (Neut && NRooTrackerVTX) {
      NRooTrackerVTX->SetCacheSize(200000000);
      NRooTrackerVTX->AddBranchToCache("*", kTRUE);
    }
    if (Genie && GRooTrackerVTX) {
      GRooTrackerVTX->SetCacheSize(200000000);
      GRooTrackerVTX->AddBranchToCache("*", kTRUE);
    }
    _firstEntry = false;
  }

  return entry_temp;
}

//*****************************************************************************
Int_t oaAnalysisTreeConverter::GetSpill(Long64_t& entry, AnaSpillC*& spill) {
  //*****************************************************************************

  // Read contents of entry (which correspond to one Spill)
  if (!fChain) return 0;

  std::string filename(reconGlobal->GetFile()->GetName());

  if (filename != _currentFileName) {
    _currentFileIndex++;
    _RooVtxEntryInCurrentInputFile = 0;
    std::cout << " Running on file (" << _currentFileIndex << "): " << filename
              << std::endl;
    _currentFileName = filename;
    _alreadyWarned = false;

    // load geometry
    ND::hgman().LoadGeometry(filename);
  }

  Int_t entry_temp = ReadEntries(entry);

  // If this is not one of the events to skim just go to the next entry (and
  // don't process that event --> return <=0)
  if (!anaUtils::CheckSkimmedEvent(RunID, SubrunID, EventID)) {
    entry++;
    return 0;
  }

  // Protection against empty spills. For those spills do not create a AnaSpill
  // in the InputManager
  if (((Neut && NNVtx == 0) || (Genie && NGVtx == 0) || EventID < 0) &&
      !FGDCosmicEvent && !TripTCosmicEvent) {
    entry_temp = 0;
    // too many of these warnings for antinu, see bugzilla 1133
    // TODO: count them and dump the final number at the end
    //    std::cout << "INFO: NVtx=0 in RooTrackerVtx (no vertices, i.e. empty
    //    spill) for event " << RunID << "-" << EventID << " --> Skip it" <<
    //    std::endl;
  } else {
    if (entry_temp > 0) {
      // Create an instance of the Spill
      spill = MakeSpill();

      // Cast it to AnaSpill
      _spill = static_cast<AnaSpill*>(spill);

      FillInfo(_spill);
    } else {
      std::cout << "Failed in reading entry " << entry << std::endl;
    }
  }

  entry++;
  _RooVtxEntryInCurrentInputFile++;

  return entry_temp;
}

//********************************************************************
void oaAnalysisTreeConverter::IncrementPOTBySpill() {
  //********************************************************************

  bool bySpillInMC = false;
#if VERSION_HAS_OFFICIAL_POT
  bySpillInMC = true;
#endif

  if (!IsMC || bySpillInMC)
    //    header().IncrementPOTBySpill(*_spill);
    anaUtils::IncrementPOTBySpill(*_spill, header());
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillInfo(AnaSpill* spill) {
  //*****************************************************************************
  spill->EventInfo = MakeEventInfo();
  AnaEventInfo& info = *static_cast<AnaEventInfo*>(spill->EventInfo);

  info.Run = RunID;
  info.SubRun = SubrunID;
  info.Event = EventID;
  info.IsMC = IsMC;
  info.EventTime = EventTime;
  info.SetIsSandMC();

  spill->InputFileIndex = _currentFileIndex;
  spill->RooVtxEntry = _RooVtxEntryInCurrentInputFile;

  spill->DataQuality = MakeDataQuality();
  spill->Beam = MakeBeam();

  spill->GeomID = (UInt_t)ND::hgman().GetCurrentGeomID();

  // beam related information
  FillBeamInfo(IsMC, static_cast<AnaBeam*>(spill->Beam));

  // data quality info
  FillDQInfo(static_cast<AnaDataQuality*>(spill->DataQuality));
  // trigger information
  FillTriggerInfo(&(spill->Trigger));

  // True vertex information
  FillTrueInfo(spill);

  // All information about each bunch
  FillBunchInfo(spill);

  // After filling both true and reco objects, delete those related to bad true
  // vertices
  if (!_isUsingReconDirP0DNew)  // TODO. Temporarily disabled for P0D native
                                // objects since it is giving problems
    DeleteBadObjects(spill);

  // Save the number of true tracks and vertices after deleting bad objects
  spill->NTotalTrueVertices = spill->TrueVertices.size();
  spill->NTotalTrueParticles = spill->TrueParticles.size();

  // FGD time bin info
  FillFgdTimeBinInfo(&(spill->FgdTimeBins));

  // Delayed clusters info
  FillDelayedClustersInfo(*spill);

  // Sort ReconParticles by momentum (to do after filling both spill and
  // bunches)
  /*  TODO
  std::vector<AnaTrueParticleB*>::iterator itTruePart;
  for (itTruePart = _spill->TrueParticles.begin();
  itTruePart!=_spill->TrueParticles.end(); itTruePart++) {
    AnaTrueParticle* TruePart = static_cast<AnaTrueParticle*>(*itTruePart);
    //    std::sort(TruePart->ReconParticles.begin(),
  TruePart->ReconParticles.end(), AnaTrack::CompareMomentum);
  }
  */

  // Sort ReconVertices by PrimaryIndex (to do after filling both spill and
  // bunches)
  std::vector<AnaTrueVertexB*>::iterator itvtx;
  for (itvtx = _spill->TrueVertices.begin();
       itvtx != _spill->TrueVertices.end(); itvtx++) {
    AnaTrueVertex* vertex = static_cast<AnaTrueVertex*>(*itvtx);
    std::sort(vertex->ReconVertices.begin(), vertex->ReconVertices.end(),
              AnaVertex::ComparePrimaryIndex);
  }
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillBeamInfo(bool isMC, AnaBeam* beam) {
  //*****************************************************************************
  beam->POT = 0;
#if !VERSION_HAS_OFFICIAL_POT
  beam->POTCT4 = 0;
#endif
  beam->Spill = ND280Spill;
  beam->GoodSpill = 1;
  beam->SpillNumber = 0;
  beam->BeamRunNumber = 0;

  if (!isMC) {
    beam->GoodSpill = 0;
    if (beamSummaryDataStatus == 1) {
      ND::TBeamSummaryDataModule::TBeamSummaryData* beamS =
          (ND::TBeamSummaryDataModule::TBeamSummaryData*)(*beamSummary)[0];
      if (beamS) {
        beam->GoodSpill = beamS->GoodSpillFlag;
#if VERSION_HAS_OFFICIAL_POT
        beam->POT = beamS->OfficialProtonsPerSpill;
#else
        beam->POT = beamS->CT5ProtonsPerSpill;
        beam->POTCT4 = beamS->OtherData.ProtonsPerSpill[3];
#endif
        beam->SpillNumber = beamS->SpillNumber;
        beam->BeamRunNumber = beamS->BeamRunNumber;
      } else {
        std::cout << "WARNING: run " << RunID << ", subrun " << SubrunID
                  << ", event " << EventID
                  << " claims to have beam summary data, but it isn't there! "
                     "Event will be ignored."
                  << std::endl;
      }
    } else if (EventID != -559038737 && !cosmic_mode && !_alreadyWarned) {
      // Warn the user, unless this is an ODB dump, which appears as the
      // first/last "event"
      // in RDP oaAnalysis files. Event -559038737 is 0xDEADBEEF converted to
      // int.
      //      std::cout << "INFO: run " << RunID << ", subrun " << SubrunID <<
      //      ", event " << EventID
      _alreadyWarned = true;
      std::cout
          << "INFO: at least one event in this file has no beam summary data."
          << " These events will be ignored if you are using the event_quality "
             "cut."
          << std::endl;
    }
  }
#if VERSION_HAS_OFFICIAL_POT
  else {
    beam->POT = POTPerSpill;
  }
#endif

  // In all cases. Keep track of the POT and Number of spills, corresponding to
  // good spills
  if (beam->GoodSpill) {
    beam->POTSincePreviousSavedSpill = beam->POT;
    beam->SpillsSincePreviousSavedSpill = 1;
  } else {
    beam->POTSincePreviousSavedSpill = 0;
    beam->SpillsSincePreviousSavedSpill = 0;
  }
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillTriggerInfo(AnaTrigger* tr) {
  //*****************************************************************************

  tr->FGDCosmic = FGDCosmicEvent;
  tr->TripTCosmic = TripTCosmicEvent;

  // Decode the CTM trigger pattern
  // To split between sub-detector logical patterns
  for (unsigned int i = 0; i < 3; i++) {
    if (CTMTriggerPattern[i] == 0xDEADBEEF) continue;

    tr->CTMTriggerPattern[2 * i] =
        (CTMTriggerPattern[i] & 0x00000000FFFFFFFFull);
    tr->CTMTriggerPattern[2 * i + 1] =
        ((CTMTriggerPattern[i] & 0xFFFFFFFF00000000ull) >> 32);
  }
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillBunchInfo(AnaSpill* spill) {
  //*****************************************************************************

  if (_isUsingReconDirTECAL || _isUsingReconDirPECAL ||
      _isUsingReconDirFGDOnly || _isUsingReconDirP0D)
    spill->OutOfBunch = MakeLocalReconBunch();
  else if (_isUsingReconDirP0DNew)
    spill->OutOfBunch = MakeP0DBunch();
  else
    spill->OutOfBunch = MakeBunch();

  spill->OutOfBunch->Bunch = -1;

  // Distribute all global tracks in bunches
  GetBunchPIDs();

  // Distribute all global vertices in bunches
  GetBunchVertices();

  // Distribute all local objects in bunches
  GetBunchLocalObjects();

  // loop over bunches
  for (unsigned int ibunch = 0; ibunch < NBUNCHES + 1; ibunch++) {
    if (_bunchPIDs[ibunch].size() == 0 && _bunchVertices[ibunch].size() == 0 &&
        _bunchTECALObjects[ibunch].size() == 0 &&
        _bunchTECALUnmatchedObjects[ibunch].size() == 0 &&
        _bunchP0DVertices[ibunch].size() == 0 &&
        _bunchP0DParticles[ibunch].size() == 0 &&
        _bunchP0DClusters[ibunch].size() == 0)
      continue;

    // warnings
    if ((unsigned int)_bunchPIDs[ibunch].size() > NMAXPARTICLES) {
      std::cout << "INFO: event " << EventID << " has "
                << (unsigned int)_bunchPIDs[ibunch].size()
                << " recon tracks (too many), "
                << "only the first " << NMAXPARTICLES
                << " will be stored (=> some warnings might appear)"
                << std::endl;
    }
    if ((unsigned int)_bunchVertices[ibunch].size() > NMAXVERTICES) {
      std::cout << "INFO: event " << EventID << " has "
                << (unsigned int)_bunchVertices[ibunch].size()
                << " recon vertices (too many), "
                << "only the first " << NMAXVERTICES
                << " will be stored (=> some warnings might appear)"
                << std::endl;
    }

    // create the bunch
    AnaBunch* bunch = NULL;
    if (ibunch < NBUNCHES) {
      // This is unilateral - what if the saving of p0dRecon output is desired?
      // An AnaLocalReconBunch is needed, which encapsulates vectors of all
      // types of locally reconstructed objects
      // but only fills the required ones....
      if (_isUsingReconDirTECAL || _isUsingReconDirPECAL ||
          _isUsingReconDirFGDOnly || _isUsingReconDirP0D)
        bunch = static_cast<AnaLocalReconBunch*>(MakeLocalReconBunch());
      else if (_isUsingReconDirP0DNew)
        bunch = static_cast<AnaP0DBunch*>(
            MakeP0DBunch());  // ---- TEST HIGHLAND2-P0D ----
      else
        bunch = static_cast<AnaBunch*>(MakeBunch());

      spill->Bunches.push_back(bunch);
      bunch->Bunch = ibunch;
    } else {
      if (_isUsingReconDirTECAL || _isUsingReconDirPECAL ||
          _isUsingReconDirFGDOnly || _isUsingReconDirP0D)
        bunch = static_cast<AnaLocalReconBunch*>(spill->OutOfBunch);
      else if (_isUsingReconDirP0DNew)
        bunch = static_cast<AnaP0DBunch*>(
            spill->OutOfBunch);  // ---- TEST HIGHLAND2-P0D ----
      else
        bunch = static_cast<AnaBunch*>(spill->OutOfBunch);
    }

    // Fill PIDs information
    if (_bunchPIDs[ibunch].size() > 0) FillPIDs(bunch, ibunch);

    // Fill TECALReconObjects information
    // What does a -ve Bunch mean?
    if (_isUsingReconDirTECAL && _bunchTECALObjects[ibunch].size() > 0)
      FillTECALReconObjects(bunch, ibunch);

    // Fill TECALReconUnmatchedObjects information
    // What does a -ve Bunch mean?
    if (_isUsingReconDirTECAL && _bunchTECALUnmatchedObjects[ibunch].size() > 0)
      FillTECALReconUnmatchedObjects(bunch, ibunch);

    // Fill P0DRecon vertices
    if (_isUsingReconDirP0D && _bunchP0DVertices[ibunch].size() > 0)
      FillP0DReconVertices(bunch, ibunch);

    // Fill P0DRecon particles
    if (_isUsingReconDirP0D && _bunchP0DParticles[ibunch].size() > 0)
      FillP0DReconParticles(bunch, ibunch);

    // Fill P0DRecon clusters
    if (_isUsingReconDirP0D && _bunchP0DClusters[ibunch].size() > 0)
      FillP0DReconClusters(bunch, ibunch);

    // ---- TEST HIGHLAND2-P0D ----
    // Fill P0D Bunch recursively, first the top level AlgoResult
    if (_isUsingReconDirP0DNew) FillP0DBunch(bunch, ibunch, _topP0DAlgoRes);
    // ----------------------------

  }  // End of loop over bunches

  // need to loop again over bunches to fill global vertices
  // (allowing the global vertices to point to PIDs in other bunches)
  FillGlobalVertices(spill);
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillPIDs(AnaBunch* bunch, int ibunch) {
  //*****************************************************************************

  // loop over global tracks in bunch
  unsigned int nTracks =
      std::min((unsigned int)_bunchPIDs[ibunch].size(), NMAXPARTICLES);
  for (unsigned int j = 0; j < nTracks; j++) {
    ND::TGlobalReconModule::TGlobalPID* globalTrack = _bunchPIDs[ibunch][j];

    // ---- TEST HIGHLAND2-P0D ----
    // When the global tracks is a p0d only track, create directly a
    // AnaP0DParticle (which does not contain segments)
    // We will most-likely follow a similar strategy for other detectors
    AnaParticleB* part = NULL;
    if (_isUsingReconDirP0DNew && globalTrack->Detectors == 6) {
      part = MakeP0DParticle();
      // Get the p0d segment in the global PID
      ND::TGlobalReconModule::TP0DObject* p0dObjectInGlobal =
          (ND::TGlobalReconModule::TP0DObject*)(*(globalTrack->P0D))[0];
      // Find the P0DReconParticle (from TP0DReconModule with the same Unique
      // ID)
      ND::TP0DReconModule::TP0DParticle* p0dParticle =
          GetP0DReconParticleWithUniqueID(p0dObjectInGlobal->UniqueID);
      // Fill info into the P0DParticle
      FillP0DParticleInfo(*p0dParticle, static_cast<AnaP0DParticle*>(part));
    }
    //-----------------------------
    else {
      part = MakeTrack();
      FillTrackInfo(*globalTrack, static_cast<AnaTrack*>(part));
      static_cast<AnaTrackB*>(part)->Index = bunch->Particles.size();
    }

    //    part->Bunch = ibunch;  TODO

    bunch->Particles.push_back(part);
  }

  // Sort tracks by momentum
  //  std::sort(bunch->Particles.begin(), bunch->Particles.end(),
  //  AnaTrack::CompareMomentum);
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillTECALReconObjects(AnaBunch* bunch,
                                                    int ibunch) {
  //*****************************************************************************

  // Cast a pointer to AnaLocalReconBunch
  AnaLocalReconBunch* localBunch = static_cast<AnaLocalReconBunch*>(bunch);

  // Clear the bunch vectors of AnaTECALReconObjects
  localBunch->TECALReconObjects.clear();

  // Loop over all the TECALReconObjects in the event
  for (UInt_t iTECAL = 0; iTECAL < _bunchTECALObjects[ibunch].size();
       ++iTECAL) {
    ND::TTrackerECALReconModule::TECALReconObject* tecalObj =
        _bunchTECALObjects[ibunch][iTECAL];

    // Make a pointer to a new AnaTECALReconObject, ready for filling
    AnaTECALReconObject* anaTECAL =
        static_cast<AnaTECALReconObject*>(MakeTECALReconObject());

    // Fill the AnaTECALReconObject with the TECALReconObject information
    FillTECALReconObjectInfo(*tecalObj, anaTECAL, localBunch);

    // If this object had an AverageHitTime outside the bunches, the anaTECAL
    // pointer is null
    // so don't add it to the bunch or the flat tree.
    if (anaTECAL) localBunch->TECALReconObjects.push_back(anaTECAL);

  }  // End of loop over TECALReconObjects
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillTECALReconUnmatchedObjects(AnaBunch* bunch,
                                                             int ibunch) {
  //*****************************************************************************

  // Cast a pointer to AnaLocalReconBunch
  AnaLocalReconBunch* localBunch = static_cast<AnaLocalReconBunch*>(bunch);

  // Clear the bunch vectors of AnaTECALUnmatchecObjects
  localBunch->TECALUnmatchedObjects.clear();

  // Loop over all the TECALReconUnmatchedObjects in the event
  for (UInt_t iTECAL = 0; iTECAL < _bunchTECALUnmatchedObjects[ibunch].size();
       ++iTECAL) {
    ND::TTrackerECALReconModule::TECALReconUnmatchedObject* tecalObj =
        _bunchTECALUnmatchedObjects[ibunch][iTECAL];

    // Make a pointer to a new AnaTECALUnmatchedObject, ready for filling
    AnaTECALUnmatchedObject* anaTECAL =
        static_cast<AnaTECALUnmatchedObject*>(MakeTECALUnmatchedObject());

    // Fill the AnaTECALUnmatchedObject with the TECALReconUnmatchedObject
    // information
    FillTECALUnmatchedObjectInfo(*tecalObj, anaTECAL, localBunch);

    // If this object had an AverageHitTime outside the bunches, the anaTECAL
    // pointer is null
    // so don't add it to the bunch or the flat tree.
    if (anaTECAL) localBunch->TECALUnmatchedObjects.push_back(anaTECAL);

  }  // End of loop over TECALReconUnmatchedObjects
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillP0DReconVertices(AnaBunch* bunch,
                                                   int ibunch) {
  //*****************************************************************************

  // Cast a pointer to AnaLocalReconBunch
  AnaLocalReconBunch* localBunch = static_cast<AnaLocalReconBunch*>(bunch);

  // Clear the bunch vectors of AnaP0DReconVertices
  localBunch->P0DReconVertices.clear();

  // Loop over all the P0D vertices in the event
  for (UInt_t iP0D = 0; iP0D < _bunchP0DVertices[ibunch].size(); ++iP0D) {
    ND::TP0DReconModule::TP0DVertex* p0dVertex =
        _bunchP0DVertices[ibunch][iP0D];

    // get the highland veranatex from the map
    AnaP0DReconVertex* anaP0DRecon = _P0DReconVerticesMap[p0dVertex];

    // Fill the AnaP0DReconVertex with the TP0DVertex information
    FillP0DReconVertexInfo(*p0dVertex, anaP0DRecon, localBunch);

    // If this object had a time outside the bunches, the anaP0DRecon pointer is
    // null
    // so don't add it to the bunch or the flat tree.
    if (anaP0DRecon) localBunch->P0DReconVertices.push_back(anaP0DRecon);

  }  // End of loop over P0DReconVertices
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillP0DReconParticles(AnaBunch* bunch,
                                                    int ibunch) {
  //*****************************************************************************

  // Cast a pointer to AnaLocalReconBunch
  AnaLocalReconBunch* localBunch = static_cast<AnaLocalReconBunch*>(bunch);

  // Clear the bunch vectors of AnaP0DReconParticles
  localBunch->P0DReconParticles.clear();

  // Loop over all the P0D particles in the event
  for (UInt_t iP0D = 0; iP0D < _bunchP0DParticles[ibunch].size(); ++iP0D) {
    ND::TP0DReconModule::TP0DParticle* p0dParticle =
        _bunchP0DParticles[ibunch][iP0D];

    // get the highland particle from the map
    AnaP0DReconParticle* anaP0DRecon = _P0DReconParticlesMap[p0dParticle];
    // Fill the AnaP0DReconParticle with the TP0DParticle information
    FillP0DReconParticleInfo(*p0dParticle, anaP0DRecon, localBunch);

    // If this object had a time outside the bunches, the anaP0DRecon pointer is
    // null
    // so don't add it to the bunch or the flat tree.
    if (anaP0DRecon) localBunch->P0DReconParticles.push_back(anaP0DRecon);

  }  // End of loop over P0DReconParticles
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillP0DReconClusters(AnaBunch* bunch,
                                                   int ibunch) {
  //*****************************************************************************

  // Cast a pointer to AnaLocalReconBunch
  AnaLocalReconBunch* localBunch = static_cast<AnaLocalReconBunch*>(bunch);

  // Clear the bunch vectors of AnaP0DReconClusters
  localBunch->P0DReconClusters.clear();

  // Loop over all the P0D clusters in the event
  for (UInt_t iP0D = 0; iP0D < _bunchP0DClusters[ibunch].size(); ++iP0D) {
    ND::TP0DReconModule::TP0DCluster* p0dCluster =
        _bunchP0DClusters[ibunch][iP0D];

    // get the highland cluster from the map
    AnaP0DReconCluster* anaP0D = _P0DReconClustersMap[p0dCluster];

    // Fill the AnaP0DReconCluster with the TP0DCluster information
    FillP0DReconClusterInfo(*p0dCluster, anaP0D, localBunch);

    // If this object had a time outside the bunches, the anaP0D pointer is null
    // so don't add it to the bunch or the flat tree.
    if (anaP0D) localBunch->P0DReconClusters.push_back(anaP0D);

  }  // End of loop over P0DReconClusters
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillP0DReconVertexInfo(
    ND::TP0DReconModule::TP0DVertex& p0dVertex,
    AnaP0DReconVertex* anaP0DReconVertex, AnaLocalReconBunch* anaLocalBunch) {
  //*****************************************************************************

  FillP0DReconVertexLinks(p0dVertex, anaP0DReconVertex);

  anaP0DReconVertex->Vertices = p0dVertex.Vertices;
  anaP0DReconVertex->Particles = p0dVertex.Particles;
  anaP0DReconVertex->Tracks = p0dVertex.Tracks;
  anaP0DReconVertex->Showers = p0dVertex.Showers;
  anaP0DReconVertex->Clusters = p0dVertex.Clusters;
  anaP0DReconVertex->Nodes = p0dVertex.Nodes;
  anaP0DReconVertex->Hits = p0dVertex.Hits;
  anaP0DReconVertex->AlgorithmName = p0dVertex.AlgorithmName;
  anaP0DReconVertex->Cycle = p0dVertex.Cycle;
  anaP0DReconVertex->NHits = p0dVertex.NHits;
  anaP0DReconVertex->UniqueID = p0dVertex.UniqueID;
  anaP0DReconVertex->Status = p0dVertex.Status;
  anaP0DReconVertex->Quality = p0dVertex.Quality;
  anaP0DReconVertex->NDOF = p0dVertex.NDOF;
  anaP0DReconVertex->Truth_PrimaryTrajIDs = p0dVertex.Truth_PrimaryTrajIDs;
  anaP0DReconVertex->Truth_TrajIDs = p0dVertex.Truth_TrajIDs;
  anaP0DReconVertex->Truth_HitCount = p0dVertex.Truth_HitCount;
  anaP0DReconVertex->Truth_ChargeShare = p0dVertex.Truth_ChargeShare;
  anaUtils::VectorToArray(p0dVertex.Position, anaP0DReconVertex->Position);
  anaUtils::VectorToArray(p0dVertex.PosVariance,
                          anaP0DReconVertex->PosVariance);
  anaP0DReconVertex->ValidDimensions = p0dVertex.ValidDimensions;
  anaP0DReconVertex->Fiducial = p0dVertex.Fiducial;

  anaP0DReconVertex->Bunch = anaLocalBunch->Bunch;
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillP0DReconParticleInfo(
    ND::TP0DReconModule::TP0DParticle& p0dParticle,
    AnaP0DReconParticle* anaP0DReconParticle,
    AnaLocalReconBunch* anaLocalBunch) {
  //*****************************************************************************

  FillP0DReconParticleLinks(p0dParticle, anaP0DReconParticle);

  anaP0DReconParticle->Vertices = p0dParticle.Vertices;
  anaP0DReconParticle->Particles = p0dParticle.Particles;
  anaP0DReconParticle->Tracks = p0dParticle.Tracks;
  anaP0DReconParticle->Showers = p0dParticle.Showers;
  anaP0DReconParticle->Clusters = p0dParticle.Clusters;
  anaP0DReconParticle->Nodes = p0dParticle.Nodes;
  anaP0DReconParticle->Hits = p0dParticle.Hits;
  anaP0DReconParticle->AlgorithmName = p0dParticle.AlgorithmName;
  anaP0DReconParticle->Cycle = p0dParticle.Cycle;
  anaP0DReconParticle->NHits = p0dParticle.NHits;
  anaP0DReconParticle->UniqueID = p0dParticle.UniqueID;
  anaP0DReconParticle->Status = p0dParticle.Status;
  anaP0DReconParticle->Quality = p0dParticle.Quality;
  anaP0DReconParticle->NDOF = p0dParticle.NDOF;
  anaP0DReconParticle->SideDeposit = p0dParticle.SideDeposit;
  anaP0DReconParticle->EndDeposit = p0dParticle.EndDeposit;
  anaP0DReconParticle->Truth_PrimaryTrajIDs = p0dParticle.Truth_PrimaryTrajIDs;
  anaP0DReconParticle->Truth_TrajIDs = p0dParticle.Truth_TrajIDs;
  anaP0DReconParticle->Truth_HitCount = p0dParticle.Truth_HitCount;
  anaP0DReconParticle->Truth_ChargeShare = p0dParticle.Truth_ChargeShare;
  anaUtils::VectorToArray(p0dParticle.Position, anaP0DReconParticle->Position);
  anaUtils::VectorToArray(p0dParticle.PosVariance,
                          anaP0DReconParticle->PosVariance);
  anaP0DReconParticle->ValidDimensions = p0dParticle.ValidDimensions;
  anaUtils::VectorToArray(p0dParticle.Direction,
                          anaP0DReconParticle->Direction);
  anaUtils::VectorToArray(p0dParticle.DirVariance,
                          anaP0DReconParticle->DirVariance);
  anaP0DReconParticle->Momentum = p0dParticle.Momentum;
  anaP0DReconParticle->Charge = p0dParticle.Charge;
  anaP0DReconParticle->realPIDNames = p0dParticle.realPIDNames;
  anaP0DReconParticle->realPIDValues = p0dParticle.realPIDValues;
  anaP0DReconParticle->integerPIDNames = p0dParticle.integerPIDNames;
  anaP0DReconParticle->integerPIDValues = p0dParticle.integerPIDValues;
  anaP0DReconParticle->PID = p0dParticle.PID;
  anaP0DReconParticle->PID_weight = p0dParticle.PID_weight;

  anaP0DReconParticle->Bunch = anaLocalBunch->Bunch;

  if (p0dParticle.Tracks.size() == 1) {
    ND::TP0DReconModule::TP0DTrack* p0dTrack =
        (ND::TP0DReconModule::TP0DTrack*)(*P0DReconTracks)[p0dParticle
                                                               .Tracks[0]];
    anaP0DReconParticle->Length = p0dTrack->Length;
    anaP0DReconParticle->Clusters = p0dTrack->Clusters;
    //    anaP0DReconParticle->Nodes         = p0dTrack->Nodes;
  } else if (p0dParticle.Tracks.size() > 1) {
    std::cout << "more than 1 track in particle" << std::endl;
  }
  if (p0dParticle.Showers.size() == 1) {
    ND::TP0DReconModule::TP0DShower* p0dShower =
        (ND::TP0DReconModule::TP0DShower*)(*P0DReconShowers)[p0dParticle
                                                                 .Showers[0]];
    anaP0DReconParticle->EDeposit = p0dShower->EDeposit;
    anaP0DReconParticle->Clusters = p0dShower->Clusters;
    //    std::cout << p0dParticle.Clusters.size() << " " <<
    //    p0dParticle.Hits.size() << " " << p0dParticle.Nodes.size() << " " <<
    //    p0dShower->Clusters.size() << " " << p0dShower->Hits.size() << " " <<
    //    p0dShower->Nodes.size() << std::endl;
  } else if (p0dParticle.Showers.size() > 1) {
    std::cout << "more than 1 shower in particle" << std::endl;
  }

  Float_t max_charge = 0;
  Int_t max_index = -1;
  Float_t total_charge = 0;
  for (UInt_t i = 0; i < p0dParticle.Truth_PrimaryTrajIDs.size(); i++) {
    if (p0dParticle.Truth_ChargeShare[i] > max_charge) {
      max_charge = p0dParticle.Truth_ChargeShare[i];
      max_index = i;
    }
    total_charge += p0dParticle.Truth_ChargeShare[i];
    /*    std::cout << p0dParticle.Truth_PrimaryTrajIDs[i] << " "
              << p0dParticle.Truth_TrajIDs[i] << " "
              << p0dParticle.Truth_HitCount[i] << " "
              << p0dParticle.Truth_ChargeShare[i] <<std::endl;
    */
  }

  anaP0DReconParticle->TrueParticle = NULL;

  /* Comment out temporarily since it is giving problems if DeleteBadObjects()
  is called
  if (max_index!=-1){
    for (std::vector<AnaTrueParticleB*>::iterator it =
  _spill->TrueParticles.begin(); it!=_spill->TrueParticles.end();it++){
      AnaTrueParticleB* trueTrack = *it;

      if (p0dParticle.Truth_TrajIDs[max_index] == trueTrack->ID){
        anaP0DReconParticle->TrueParticle = trueTrack;
        if (p0dParticle.Truth_ChargeShare[max_index]/total_charge!=0)
          static_cast<AnaTrueParticle*>(trueTrack)->Purity =
  p0dParticle.Truth_ChargeShare[max_index]/total_charge;

        break; // Stop when association is found.
      }
    }
  }
  */
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
void oaAnalysisTreeConverter::FillP0DReconClusterInfo(
    ND::TP0DReconModule::TP0DCluster& p0dCluster,
    AnaP0DReconCluster* anaP0DReconCluster, AnaLocalReconBunch* anaLocalBunch) {
  //*****************************************************************************

  FillP0DReconClusterLinks(p0dCluster, anaP0DReconCluster);

  anaP0DReconCluster->Vertices = p0dCluster.Vertices;
  anaP0DReconCluster->Particles = p0dCluster.Particles;
  anaP0DReconCluster->Tracks = p0dCluster.Tracks;
  anaP0DReconCluster->Showers = p0dCluster.Showers;
  anaP0DReconCluster->Clusters = p0dCluster.Clusters;
  anaP0DReconCluster->Nodes = p0dCluster.Nodes;
  anaP0DReconCluster->Hits = p0dCluster.Hits;
  anaP0DReconCluster->AlgorithmName = p0dCluster.AlgorithmName;
  anaP0DReconCluster->Cycle = p0dCluster.Cycle;
  anaP0DReconCluster->NHits = p0dCluster.NHits;
  anaP0DReconCluster->UniqueID = p0dCluster.UniqueID;
  anaP0DReconCluster->Truth_PrimaryTrajIDs = p0dCluster.Truth_PrimaryTrajIDs;
  anaP0DReconCluster->Truth_TrajIDs = p0dCluster.Truth_TrajIDs;
  anaP0DReconCluster->Truth_HitCount = p0dCluster.Truth_HitCount;
  anaP0DReconCluster->Truth_ChargeShare = p0dCluster.Truth_ChargeShare;
  anaUtils::VectorToArray(p0dCluster.Position, anaP0DReconCluster->Position);
  anaUtils::VectorToArray(p0dCluster.PosVariance,
                          anaP0DReconCluster->PosVariance);
  anaP0DReconCluster->ValidDimensions = p0dCluster.ValidDimensions;
  anaP0DReconCluster->NFiducialHits = p0dCluster.NFiducialHits;
  anaP0DReconCluster->EDeposit = p0dCluster.EDeposit;

  anaP0DReconCluster->Bunch = anaLocalBunch->Bunch;
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillP0DReconVertexLinks(
    ND::TP0DReconModule::TP0DVertex& p0dVertex,
    AnaP0DReconVertex* anaP0DReconVertex) {
  //*****************************************************************************

  for (UInt_t i = 0; i < p0dVertex.Vertices.size(); i++) {
    ND::TP0DReconModule::TP0DVertex* p0dVertex2 =
        (ND::TP0DReconModule::TP0DVertex*)(*P0DReconVertices)[p0dVertex
                                                                  .Vertices[i]];
    anaP0DReconVertex->VerticesP.push_back(_P0DReconVerticesMap[p0dVertex2]);
  }

  for (UInt_t i = 0; i < p0dVertex.Particles.size(); i++) {
    ND::TP0DReconModule::TP0DParticle* p0dParticle2 =
        (ND::TP0DReconModule::TP0DParticle*)(*P0DReconParticles)
            [p0dVertex.Particles[i]];
    anaP0DReconVertex->ParticlesP.push_back(
        _P0DReconParticlesMap[p0dParticle2]);
  }

  for (UInt_t i = 0; i < p0dVertex.Clusters.size(); i++) {
    ND::TP0DReconModule::TP0DCluster* p0dCluster2 =
        (ND::TP0DReconModule::TP0DCluster*)(*P0DReconClusters)
            [p0dVertex.Clusters[i]];
    anaP0DReconVertex->ClustersP.push_back(_P0DReconClustersMap[p0dCluster2]);
  }
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillP0DReconParticleLinks(
    ND::TP0DReconModule::TP0DParticle& p0dParticle,
    AnaP0DReconParticle* anaP0DReconParticle) {
  //*****************************************************************************

  for (UInt_t i = 0; i < p0dParticle.Vertices.size(); i++) {
    ND::TP0DReconModule::TP0DVertex* p0dVertex2 =
        (ND::TP0DReconModule::TP0DVertex*)(*P0DReconVertices)[p0dParticle
                                                                  .Vertices[i]];
    anaP0DReconParticle->VerticesP.push_back(_P0DReconVerticesMap[p0dVertex2]);
  }

  for (UInt_t i = 0; i < p0dParticle.Particles.size(); i++) {
    ND::TP0DReconModule::TP0DParticle* p0dParticle2 =
        (ND::TP0DReconModule::TP0DParticle*)(*P0DReconParticles)
            [p0dParticle.Particles[i]];
    anaP0DReconParticle->ParticlesP.push_back(
        _P0DReconParticlesMap[p0dParticle2]);
  }

  for (UInt_t i = 0; i < p0dParticle.Clusters.size(); i++) {
    ND::TP0DReconModule::TP0DCluster* p0dCluster2 =
        (ND::TP0DReconModule::TP0DCluster*)(*P0DReconClusters)
            [p0dParticle.Clusters[i]];
    anaP0DReconParticle->ClustersP.push_back(_P0DReconClustersMap[p0dCluster2]);
  }
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillP0DReconClusterLinks(
    ND::TP0DReconModule::TP0DCluster& p0dCluster,
    AnaP0DReconCluster* anaP0DReconCluster) {
  //*****************************************************************************

  for (UInt_t i = 0; i < p0dCluster.Vertices.size(); i++) {
    ND::TP0DReconModule::TP0DVertex* p0dVertex2 =
        (ND::TP0DReconModule::TP0DVertex*)(*P0DReconVertices)[p0dCluster
                                                                  .Vertices[i]];
    anaP0DReconCluster->VerticesP.push_back(_P0DReconVerticesMap[p0dVertex2]);
  }

  for (UInt_t i = 0; i < p0dCluster.Particles.size(); i++) {
    ND::TP0DReconModule::TP0DParticle* p0dParticle2 =
        (ND::TP0DReconModule::TP0DParticle*)(*P0DReconParticles)
            [p0dCluster.Particles[i]];
    anaP0DReconCluster->ParticlesP.push_back(
        _P0DReconParticlesMap[p0dParticle2]);
  }

  for (UInt_t i = 0; i < p0dCluster.Clusters.size(); i++) {
    ND::TP0DReconModule::TP0DCluster* p0dCluster2 =
        (ND::TP0DReconModule::TP0DCluster*)(*P0DReconClusters)
            [p0dCluster.Clusters[i]];
    anaP0DReconCluster->ClustersP.push_back(_P0DReconClustersMap[p0dCluster2]);
  }
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillGlobalVertices(AnaSpill* spill) {
  //*****************************************************************************

  // need to loop again over bunches to fill global vertices
  // (allowing the global vertices to point to PIDs in other bunches)
  for (unsigned int ibunch = 0; ibunch < NBUNCHES + 1; ibunch++) {
    if (_bunchVertices[ibunch].size() == 0) continue;

    // Find the pointer to the right bunch
    AnaBunch* bunch = 0;
    if (ibunch == NBUNCHES)
      bunch = static_cast<AnaBunch*>(spill->OutOfBunch);
    else {
      std::vector<AnaBunchC*>::iterator itbunch;
      for (itbunch = spill->Bunches.begin(); itbunch != spill->Bunches.end();
           itbunch++) {
        if (ibunch == (unsigned int)(*itbunch)->Bunch) {
          bunch = static_cast<AnaBunch*>(*itbunch);
          break;
        }
      }
    }
    if (!bunch)
      std::cout << "ERROR 1102 in oaAnalysisConverter: looking for a bunch not "
                   "created!"
                << std::endl;

    // loop over global vertex in bunch
    unsigned int nVertices =
        std::min((unsigned int)_bunchVertices[ibunch].size(), NMAXVERTICES);
    for (unsigned int j = 0; j < nVertices; j++) {
      ND::TGlobalReconModule::TGlobalVertex* globalVertex =
          _bunchVertices[ibunch][j];
      AnaVertex* vertex = static_cast<AnaVertex*>(MakeVertex());
      FillVertexInfo(*globalVertex, vertex, bunch, spill);
      bunch->Vertices.push_back(vertex);
    }

    // Sort vertices by PrimaryIndex
    std::sort(bunch->Vertices.begin(), bunch->Vertices.end(),
              AnaVertex::ComparePrimaryIndex);

  }  // End of loop over bunches
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillFgdTimeBinInfo(
    std::vector<AnaFgdTimeBinB*>* AnaFgdTimeBins) {
  //*****************************************************************************

  // loop over bunches
  for (Int_t ibin = 0; ibin < NFgdTimeBins; ibin++) {
    ND::TGlobalReconModule::TFgdTimeBin* bin =
        (ND::TGlobalReconModule::TFgdTimeBin*)(*(FgdTimeBins))[ibin];
    if (!bin) continue;

    if (AnaFgdTimeBins->size() >= NMAXFGDTIMEBINS) break;

    AnaFgdTimeBin* abin = static_cast<AnaFgdTimeBin*>(MakeFgdTimeBin());
    AnaFgdTimeBins->push_back(abin);

    abin->MinTime = bin->minTime;
    abin->MaxTime = bin->maxTime;
    abin->G4ID = bin->g4ID;

    for (int i = 0; i < 2; i++) {
      abin->NHits[i] = bin->nHits[i];
      abin->RawChargeSum[i] = bin->rawChargeSum[i];
    }
    // Fill the hits info if requested
    if (!_fillFgdSingleHitsInfo) continue;

#if VERSION_HAS_FGD_HIT_INFO

    anaUtils::CreateArray(abin->FgdHits,
                          bin->FGD1HitLayer.size() + bin->FGD2HitLayer.size());

    // FGD1
    for (size_t i = 0; i < bin->FGD1HitLayer.size(); i++) {
      AnaHit* hit = new AnaHit();
      // Has only X/Y pos stored
      // X layers are even (from zero) and Y ones are odd
      hit->Type = (bin->FGD1HitLayer[i] % 2) ? AnaHit::kYZ : AnaHit::kXZ;
      SubDetId::SetDetectorUsed(hit->Detector, SubDetId::kFGD1);
      SubDetId::SetDetectorSystemFields(hit->Detector);

      hit->Layer = bin->FGD1HitLayer[i];

      size_t index = (bin->FGD1HitLayer[i] % 2) ? 1 : 0;
      hit->Position[index] = bin->FGD1HitPosition[i];
      hit->Position[3] = bin->FGD1HitT[i];
      hit->Charge = bin->FGD1HitQ[i];
#if VERSION_HAS_FGD_HIT_TRUE_TRAJ_ID
      hit->TrueG4IDs.push_back(bin->FGD1HitTrajID[i]);
#endif
      abin->FgdHits[abin->nFgdHits++] = hit;
    }

    // FGD2
    for (size_t i = 0; i < bin->FGD2HitLayer.size(); i++) {
      AnaHit* hit = new AnaHit();
      // Has only X/Y pos stored
      // X layers are even (from zero) and Y ones are odd
      hit->Type = (bin->FGD2HitLayer[i] % 2) ? AnaHit::kYZ : AnaHit::kXZ;
      SubDetId::SetDetectorUsed(hit->Detector, SubDetId::kFGD2);
      SubDetId::SetDetectorSystemFields(hit->Detector);

      hit->Layer = bin->FGD2HitLayer[i];

      size_t index = (bin->FGD2HitLayer[i] % 2) ? 1 : 0;
      hit->Position[index] = bin->FGD2HitPosition[i];
      hit->Position[3] = bin->FGD2HitT[i];
      hit->Charge = bin->FGD2HitQ[i];

#if VERSION_HAS_FGD_HIT_TRUE_TRAJ_ID
      hit->TrueG4IDs.push_back(bin->FGD2HitTrajID[i]);
#endif
      abin->FgdHits[abin->nFgdHits++] = hit;
    }
#endif
  }
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillTECALReconObjectInfo(
    ND::TTrackerECALReconModule::TECALReconObject& tecalReconObject,
    AnaTECALReconObject* anaTECALReconObject,
    AnaLocalReconBunch* anaLocalBunch) {
  //*****************************************************************************

  anaTECALReconObject->AverageHitTime = tecalReconObject.AverageHitTime;
  anaTECALReconObject->AverageZPos = tecalReconObject.AverageZPosition;
  anaTECALReconObject->Containment = tecalReconObject.Containment;
  anaTECALReconObject->EFitResult = tecalReconObject.EMEnergyFit_Result;
  anaTECALReconObject->EFitUncertainty =
      tecalReconObject.EMEnergyFit_Uncertainty;

#if !VERSION_HAS_REMOVED_OBSOLETE_ECAL_PID_VARS
  anaTECALReconObject->FirstLayer = tecalReconObject.FirstLayer;
  anaTECALReconObject->IsShowerLike = tecalReconObject.IsShowerLike;
  anaTECALReconObject->IsTrackLike = tecalReconObject.IsTrackLike;
  anaTECALReconObject->LastLayer = tecalReconObject.LastLayer;
  anaTECALReconObject->MichelTagNDelayedCluster =
      tecalReconObject.MElectronTag_NDelayedCluster;
  anaTECALReconObject->PIDAMR = tecalReconObject.PID_AMR;
  anaTECALReconObject->PIDMaxRatio = tecalReconObject.PID_Max_Ratio;
  anaTECALReconObject->PIDMeanPos = tecalReconObject.PID_MeanPosition;
  anaTECALReconObject->PIDShowerWidth = tecalReconObject.PID_ShowerWidth;
  anaTECALReconObject->TimeBunch = tecalReconObject.TimeBunch;

#endif

#if VERSION_HAS_ECAL_LLR
  anaTECALReconObject->LikeEMHIP = tecalReconObject.PID_LLR_EM_HIP;
  anaTECALReconObject->LikeMIPEM = tecalReconObject.PID_LLR_MIP_EM;
  anaTECALReconObject->LikeMIPEMLow =
      tecalReconObject.PID_LLR_MIP_EM_LowMomentum;
  anaTECALReconObject->LikeMIPPion = tecalReconObject.PID_LLR_MIP_Pion;
#endif

  anaTECALReconObject->MatchingLike = tecalReconObject.MatchingLikelihood;
  anaTECALReconObject->Module = tecalReconObject.Module;

#if !VERSION_PROD7_DEVEL
  anaTECALReconObject->MostDownStreamLayerHit =
      tecalReconObject.mostDownStreamLayerHit;
  anaTECALReconObject->MostUpStreamLayerHit =
      tecalReconObject.mostUpStreamLayerHit;
#else
  anaTECALReconObject->MostDownStreamLayerHit =
      tecalReconObject.MostDownStreamLayerHit;
  anaTECALReconObject->MostUpStreamLayerHit =
      tecalReconObject.MostUpStreamLayerHit;
#endif

  anaTECALReconObject->NHits = tecalReconObject.NHits;
  anaTECALReconObject->NLayersHit = tecalReconObject.NLayersHit;
  anaTECALReconObject->ObjectLength = tecalReconObject.ObjectLength;
  anaTECALReconObject->PIDAngle = tecalReconObject.PID_Angle;
  anaTECALReconObject->PIDAsymmetry = tecalReconObject.PID_Asymmetry;
  anaTECALReconObject->PIDCircularity = tecalReconObject.PID_Circularity;
  anaTECALReconObject->PIDFBR = tecalReconObject.PID_FrontBackRatio;
  anaTECALReconObject->PIDShowerAngle = tecalReconObject.PID_ShowerAngle;
  anaTECALReconObject->PIDTransverseChargeRatio =
      tecalReconObject.PID_TransverseChargeRatio;
  anaTECALReconObject->PIDTruncatedMaxRatio =
      tecalReconObject.PID_TruncatedMaxRatio;
  anaUtils::VectorToArray(tecalReconObject.Pointing,
                          anaTECALReconObject->Pointing);
  anaTECALReconObject->Thrust = tecalReconObject.Thrust;
  anaUtils::VectorToArray(tecalReconObject.ThrustAxis,
                          anaTECALReconObject->ThrustAxis);
  anaUtils::VectorToArray(tecalReconObject.ThrustOrigin,
                          anaTECALReconObject->ThrustOrigin);
  anaTECALReconObject->TotalHitCharge = tecalReconObject.TotalHitCharge;
  anaTECALReconObject->TrueID = tecalReconObject.G4ID;
  anaTECALReconObject->TrueIDPrimary = tecalReconObject.G4ID_Primary;
  anaTECALReconObject->TrueIDRecursive = tecalReconObject.G4ID_Recursive;
  anaTECALReconObject->TrueIDSingle = tecalReconObject.G4ID_Single;
  anaTECALReconObject->UniqueID = tecalReconObject.UniqueID;

  anaTECALReconObject->Bunch = anaLocalBunch->Bunch;
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillTECALUnmatchedObjectInfo(
    ND::TTrackerECALReconModule::TECALReconUnmatchedObject&
        tecalUnmatchedObject,
    AnaTECALUnmatchedObject* anaTECALUnmatchedObject,
    AnaLocalReconBunch* anaLocalBunch) {
  //*****************************************************************************

  anaTECALUnmatchedObject->AverageHitTime = tecalUnmatchedObject.AverageHitTime;
  anaUtils::VectorToArray(tecalUnmatchedObject.BackPosition,
                          anaTECALUnmatchedObject->BackPos);
  anaUtils::VectorToArray(tecalUnmatchedObject.FrontPosition,
                          anaTECALUnmatchedObject->FrontPos);
#if !VERSION_PROD7_DEVEL
  anaTECALUnmatchedObject->MostDownStreamLayerHit =
      tecalUnmatchedObject.mostDownStreamLayerHit;
  anaTECALUnmatchedObject->MostUpStreamLayerHit =
      tecalUnmatchedObject.mostUpStreamLayerHit;
#else
  anaTECALUnmatchedObject->MostDownStreamLayerHit =
      tecalUnmatchedObject.MostDownStreamLayerHit;
  anaTECALUnmatchedObject->MostUpStreamLayerHit =
      tecalUnmatchedObject.MostUpStreamLayerHit;
#endif

  anaTECALUnmatchedObject->NHits = tecalUnmatchedObject.NHits;
  anaTECALUnmatchedObject->TotalHitCharge = tecalUnmatchedObject.TotalHitCharge;
  anaTECALUnmatchedObject->TrueID = tecalUnmatchedObject.G4ID;
  anaTECALUnmatchedObject->TrueIDPrimary = tecalUnmatchedObject.G4ID_Primary;
  anaTECALUnmatchedObject->TrueIDRecursive =
      tecalUnmatchedObject.G4ID_Recursive;
  anaTECALUnmatchedObject->TrueIDSingle = tecalUnmatchedObject.G4ID_Single;
  anaTECALUnmatchedObject->View = tecalUnmatchedObject.View;

  anaTECALUnmatchedObject->Bunch = anaLocalBunch->Bunch;
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillDQInfo(AnaDataQuality* dq) {
  //*****************************************************************************

  if (fND280OffFlag)
    dq->GoodDaq = false;
  else
    dq->GoodDaq = true;

  dq->ND280Flag = fND280OffFlag;

  dq->DetFlag[0] = fTPCFlag;
  dq->DetFlag[1] = fFGDFlag;
  dq->DetFlag[2] = fECALFlag;
  dq->DetFlag[3] = fDSECALFlag;
  dq->DetFlag[4] = fP0DFlag;
  dq->DetFlag[5] = fSMRDFlag;
  dq->DetFlag[6] = fMAGNETFlag;
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillTrackInfo(
    ND::TGlobalReconModule::TGlobalPID& globalTrack, AnaTrack* track) {
  //*****************************************************************************

  //-------------- Global info
  track->UniqueID = globalTrack.UniqueID;
  track->Status = globalTrack.Status;
  track->NNodes = globalTrack.NNodes;
  track->NHits = globalTrack.NHits;
  track->NNodes = globalTrack.NNodes;
  track->NDOF = globalTrack.NDOF;
  track->Chi2 = globalTrack.Chi2;
  track->Detectors = globalTrack.Detectors;
  track->Length = globalTrack.Length;
  track->Momentum = globalTrack.FrontMomentum;
  track->MomentumError = globalTrack.FrontMomentumError;
  track->Charge = globalTrack.Charge;
  anaUtils::VectorToArray(globalTrack.FrontDirection, track->DirectionStart);
  anaUtils::VectorToArray(globalTrack.BackDirection, track->DirectionEnd);
  anaUtils::VectorToArray(globalTrack.FrontPosition, track->PositionStart);
  anaUtils::VectorToArray(globalTrack.BackPosition, track->PositionEnd);

  FillTrackHits(globalTrack, track);

  track->MomentumEle = -10000;
  track->MomentumMuon = -10000;
  track->MomentumProton = -10000;
  track->MomentumErrorEle = -10000;
  track->MomentumErrorMuon = -10000;
  track->MomentumErrorProton = -10000;

#if VERSION_HAS_PRANGE_ESTIMATES
  track->RangeMomentumEle = -10000;
  track->RangeMomentumMuon = -10000;
  track->RangeMomentumProton = -10000;

  track->RangeMomentumMuonFlip = -10000;
  track->RangeMomentumProtonFlip = -10000;
#endif

  // get the PID of the main track
  std::map<double, int, std::greater<double> > map_pid;

  for (int i = 0; i < (int)globalTrack.PIDWeights.size(); i++)
    map_pid[globalTrack.PIDWeights[i]] = globalTrack.ParticleIds[i];

  if (map_pid.size() > 0) track->ReconPDG = map_pid.begin()->second;

  //----------------- Alternate fits assuming a given particle hypothesis
  for (int jj = 0; jj < globalTrack.NAlternates; jj++) {
    ND::TGlobalReconModule::TGlobalPIDAlternate* alt =
        (ND::TGlobalReconModule::TGlobalPIDAlternate*)(*(
            globalTrack.Alternates))[jj];
    if (!alt) continue;

    bool samedir = true;

#if VERSION_HAS_REVERSED_REFITS
    // P6 files have refits in the backwards direction too. For now,
    // we just save the info about the fit in the same direction as
    // the default fit. In future, we may want to save the reversed
    // info too.
    samedir = ((alt->FrontDirection[2] < 0 && track->DirectionStart[2] < 0) ||
               (alt->FrontDirection[2] >= 0 && track->DirectionStart[2] >= 0));

    if (!samedir && map_pid.size() > 0) {
      // Fill the momentum and direction for the main PID hypothesis and
      // reversed sense
      if (alt->ParticleId == map_pid.begin()->second) {
        track->MomentumFlip = alt->FrontMomentum;
        anaUtils::VectorToArray(alt->FrontDirection, track->DirectionStartFlip);
        anaUtils::VectorToArray(alt->FrontDirection, track->DirectionEndFlip);
      }
    }

#endif

    if (alt->ParticleId == 11 && samedir) {
      track->MomentumEle = alt->FrontMomentum;
      track->MomentumErrorEle = alt->FrontMomentumError;
    } else if (alt->ParticleId == 13 && samedir) {
      track->MomentumMuon = alt->FrontMomentum;
      track->MomentumErrorMuon = alt->FrontMomentumError;
    } else if (alt->ParticleId == 2212 && samedir) {
      track->MomentumProton = alt->FrontMomentum;
      track->MomentumErrorProton = alt->FrontMomentumError;
    }
  }

#if VERSION_HAS_PRANGE_ESTIMATES
  // P6 files have momentum from range estimates for
  // three PID hypothesis and in the backwards direction too (six in total). For
  // now,
  // we save the three values that correspond to the "original" reconstructed
  // direction of the track
  track->RangeMomentumEle = globalTrack.RangeMomentumElectron;
  track->RangeMomentumMuon = globalTrack.RangeMomentumMuon;
  track->RangeMomentumProton = globalTrack.RangeMomentumProton;

  track->RangeMomentumMuonFlip = globalTrack.RangeMomentumMuonFlip;
  track->RangeMomentumProtonFlip = globalTrack.RangeMomentumProtonFlip;
#endif

#if VERSION_HAS_TIME_FITS
  for (unsigned int i = 0; i < globalTrack.NodeTimes.size(); i++) {
    AnaTimeNode* node = new AnaTimeNode();

    node->Detector = globalTrack.NodeTimes[i].first;
    node->TimeStart = globalTrack.NodeTimes[i].second.X();
    node->TimeEnd = globalTrack.NodeTimes[i].second.Y();

    track->TimeNodes.push_back(node);
  }

  track->ToF.Flag_FGD1_FGD2   = false;
  track->ToF.Flag_P0D_FGD1    = false;
  track->ToF.Flag_DSECal_FGD1 = false;
  track->ToF.Flag_ECal_FGD1   = false;
  track->ToF.Flag_ECal_FGD2   = false;
  track->ToF.Flag_DSECal_FGD2 = false;

  track->Time.Flag_FGD1       = false;
  track->Time.Flag_FGD2       = false;
  track->Time.Flag_P0D        = false;
  track->Time.Flag_ECal       = false;
  track->Time.Flag_DSECal     = false;

  bool fgd1 = false, fgd2 = false, p0d = false, ecal = false, dsecal = false;
  float tfgd1 = 0, tfgd2 = 0, tp0d = 0, tecal = 0, tdsecal = 0;

  for (unsigned int i = 0; i < globalTrack.NodeTimes.size(); i++) {
    double t = (globalTrack.NodeTimes[i].second.X() +
                globalTrack.NodeTimes[i].second.Y()) /
               2.0;
    switch (globalTrack.NodeTimes[i].first) {
      case 4:
        if (!fgd1) {
          tfgd1 = t;
        }
        fgd1 = true;
        break;
      case 5:
        if (!fgd2) {
          tfgd2 = t;
        }
        fgd2 = true;
        break;
      case 6:
        if (!p0d) {
          tp0d = t;
        }
        p0d = true;
        break;
      case 7:
        if (!dsecal) {
          tdsecal = t;
        }
        dsecal = true;
        break;
      case 9:
        if (!ecal) {
          tecal = t;
        }  // take the first ecal, there can be two (example : cosmics)
        ecal = true;
        break;
    }
  }

  if (fgd1 && fgd2) {
    track->ToF.Flag_FGD1_FGD2 = true;
    track->ToF.FGD1_FGD2 = tfgd1 - tfgd2;
  } else {
    track->ToF.Flag_FGD1_FGD2 = false;
  }
  if (fgd1 && p0d) {
    track->ToF.Flag_P0D_FGD1 = true;
    track->ToF.P0D_FGD1 = tp0d - tfgd1;
  } else {
    track->ToF.Flag_P0D_FGD1 = false;
  }
  if (fgd1 && dsecal) {
    track->ToF.Flag_DSECal_FGD1 = true;
    track->ToF.DSECal_FGD1 = tdsecal - tfgd1;
  } else {
    track->ToF.Flag_DSECal_FGD1 = false;
  }
  if (fgd1 && ecal) {
    track->ToF.Flag_ECal_FGD1 = true;
    track->ToF.ECal_FGD1 = tecal - tfgd1;
  } else {
    track->ToF.Flag_ECal_FGD1 = false;
  }
  if (fgd2 && dsecal) {
    track->ToF.Flag_DSECal_FGD2 = true;
    track->ToF.DSECal_FGD2 = tdsecal - tfgd2;
  } else {
    track->ToF.Flag_DSECal_FGD2 = false;
  }
  if (fgd2 && ecal) {
    track->ToF.Flag_ECal_FGD2 = true;
    track->ToF.ECal_FGD2 = tecal - tfgd2;
  } else {
    track->ToF.Flag_ECal_FGD2 = false;
  }


  if (fgd1) {
    track->Time.Flag_FGD1 = true;
    track->Time.FGD1 = tfgd1;
  } else {
    track->Time.Flag_FGD1 = false;
  }

  if (fgd2) {
    track->Time.Flag_FGD2 = true;
    track->Time.FGD2 = tfgd2;
  } else {
    track->Time.Flag_FGD2 = false;
  }

  if (p0d) {
    track->Time.Flag_P0D = true;
    track->Time.P0D = tp0d;
  } else {
    track->Time.Flag_P0D = false;
  }

  if (ecal) {
    track->Time.Flag_ECal = true;
    track->Time.ECal = tecal;
  } else {
    track->Time.Flag_ECal = false;
  }

  if (dsecal) {
    track->Time.Flag_DSECal = true;
    track->Time.DSECal = tdsecal;
  } else {
    track->Time.Flag_DSECal = false;
  }

#endif

  convUtils::ConvertTrackDetEnumToBitField(track->Detector,
                                           globalTrack.DetectorUsed);

  //-------------- Subdetector Constituents

  FillTpcInfo(globalTrack, track);
  FillFgdInfo(globalTrack, track);
  FillEcalInfo(globalTrack, track);
  FillSmrdInfo(globalTrack, track);
  FillP0dInfo(globalTrack, track);
  FillTrackerInfo(globalTrack, track);

  // Make truth-reco association
  FillTrueParticleRecoInfo(globalTrack.TrueParticle, track->TrueObject, true);

  // Save all recon tracks associated to this true track
  if (track->TrueObject) {
    AnaTrueParticle* trueParticle =
        static_cast<AnaTrueParticle*>(track->TrueObject);
    trueParticle->ReconParticles.push_back(track);

#ifdef DEBUG
    if (trueParticle->IsTruePrimaryPi0DecayPhoton)
      std::cout << "oaAnalysisTreeConverter::FillTrackInfo() found a track "
                   "from globalPID with UniqueID:"
                << globalTrack.UniqueID << " and appended it to ReconParticles "
                                           "for true pi0 decay photon with ID:"
                << trueParticle->ID << std::endl;
#endif

    if (track->GetTrueParticle()->TrueVertex) {
      // Save all recon tracks associated to this true vertex (track without
      // truth association might be missing here)
      static_cast<AnaTrueVertex*>(track->GetTrueParticle()->TrueVertex)
          ->ReconParticles.push_back(track);
    }
  }
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillTrackHits(
    ND::TGlobalReconModule::TGlobalPID& globalTrack, AnaTrack* track) {
  //*****************************************************************************

  // Fill the first 2 more upstream and the first 2 more downstream hits

  if (globalTrack.NHitsSaved == 0) return;

  int ihit = 0;
  int prevtype, thistype;
  ND::TGlobalReconModule::TGlobalHit* hit;
  // Upstream hit(s)
  hit = (ND::TGlobalReconModule::TGlobalHit*)(*(globalTrack.HitsSaved))[ihit];
  track->UpstreamHits_Position[0] = hit->Position;
  track->UpstreamHits_Charge[0] = hit->Charge;
  prevtype = hit->Type % 100;  // ##1 has X info, #1# has Y info, 1## has Z info
  if (globalTrack.NHitsSaved == 4 && prevtype == 11)
    std::cout
        << "Minor error in oaAnalysisConverter::FillTrackHits (ref. 4833)\n";

  // if X or Y info are missing, another upstream hit with the missing info is
  // stored (if found)
  // if 3 HitsSaved, the 2nd is the second upstream or the first downstream?
  // Check if it is more downstream than the 3rd (and last) hit
  bool anotherupstream = false;
  if (globalTrack.NHitsSaved == 3 && prevtype != 11) {
    ND::TGlobalReconModule::TGlobalHit* hit1 =
        (ND::TGlobalReconModule::TGlobalHit*)(*(globalTrack.HitsSaved))[1];
    ND::TGlobalReconModule::TGlobalHit* hit2 =
        (ND::TGlobalReconModule::TGlobalHit*)(*(globalTrack.HitsSaved))[2];
    if (hit1->Position.Z() < hit2->Position.Z()) anotherupstream = true;
    // if same Z and the first has both X and Y info, it has to be another
    // upstream
    if (hit1->Position.Z() == hit2->Position.Z() && (hit1->Type % 100) == 11)
      anotherupstream = true;
    // otherwise, if same Z, we don't know (in TGlobalReconModule hits are only
    // sorted by Z)
    // and sometimes it happens (for very short tracks, so that also the first
    // upstream is very near)
    //    if (hit1->Position.Z() == hit2->Position.Z() && (hit1->Type % 100) !=
    //    11)
    //          std::cout << "Minor error in oaAnalysisConverter (ref. 4834)
    //          with deltaZ = " << hit2->Position.Z() - hit1->Position.Z() <<
    //          "\n";
  }

  if (globalTrack.NHitsSaved == 4 || anotherupstream) {
    hit =
        (ND::TGlobalReconModule::TGlobalHit*)(*(globalTrack.HitsSaved))[++ihit];
    track->UpstreamHits_Position[1] = hit->Position;
    track->UpstreamHits_Charge[1] = hit->Charge;
    thistype = hit->Type % 100;
    if (prevtype == thistype)
      std::cout
          << "Minor error in oaAnalysisConverter::FillTrackHits (ref. 4835) "
          << prevtype << " vs " << thistype << "\n";
  }

  // Downstream hit(s)
  hit = (ND::TGlobalReconModule::TGlobalHit*)(*(globalTrack.HitsSaved))[++ihit];
  track->DownstreamHits_Position[0] = hit->Position;
  track->DownstreamHits_Charge[0] = hit->Charge;
  prevtype = hit->Type % 100;

  // if X or Y info are missing, another downstream hit is stored (if found)
  if (ihit == globalTrack.NHitsSaved - 1) return;
  if (prevtype == 11)
    std::cout
        << "Minor error in oaAnalysisConverter::FillTrackHits (ref. 4836) "
        << ihit << " vs " << globalTrack.NHitsSaved << " and "
        << anotherupstream << "\n";
  hit = (ND::TGlobalReconModule::TGlobalHit*)(*(globalTrack.HitsSaved))[++ihit];
  track->DownstreamHits_Position[1] = hit->Position;
  track->DownstreamHits_Charge[1] = hit->Charge;
  thistype = hit->Type % 100;
  if (prevtype == thistype)
    std::cout
        << "Minor error in oaAnalysisConverter::FillTrackHits (ref. 4837) \n";

  if (ihit != globalTrack.NHitsSaved - 1)
    std::cout
        << "Minor error in oaAnalysisConverter::FillTrackHits (ref. 4838) \n";
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillTpcInfo(
    ND::TGlobalReconModule::TGlobalPID& globalTrack, AnaTrack* track) {
  //*****************************************************************************
  track->nTPCSegments = 0;

  std::vector<ND::TGlobalReconModule::TTPCObject*> tpcObjects;

  tpcObjects.reserve((unsigned int)globalTrack.NTPCs);

  for (int jj = 0; jj < globalTrack.NTPCs; jj++) {
    ND::TGlobalReconModule::TTPCObject* tpcTrack =
        (ND::TGlobalReconModule::TTPCObject*)(*(globalTrack.TPC))[jj];
    if (!tpcTrack) continue;
    tpcObjects.push_back(tpcTrack);
  }

  // sort in decreasing NNodes order
  std::sort(tpcObjects.begin(), tpcObjects.end(), CompareNNodes);

  int nObjects = std::min((unsigned int)tpcObjects.size(), NMAXTPCS);
  for (int jj = 0; jj < nObjects; jj++) {
    ND::TGlobalReconModule::TTPCObject* tpcTrack = tpcObjects[jj];

    AnaTPCParticle* seg = static_cast<AnaTPCParticle*>(MakeTpcTrack());
    FillTpcTrackInfo(*tpcTrack, seg);
    track->TPCSegments[track->nTPCSegments++] = seg;
  }
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillFgdInfo(
    ND::TGlobalReconModule::TGlobalPID& globalTrack, AnaTrack* track) {
  //*****************************************************************************
  track->nFGDSegments = 0;
  for (int jj = 0; jj < globalTrack.NFGDs; jj++) {
    if ((unsigned int)track->nFGDSegments == NMAXFGDS) break;

    ND::TGlobalReconModule::TFGDObject* fgdTrack =
        (ND::TGlobalReconModule::TFGDObject*)(*(globalTrack.FGD))[jj];
    if (!fgdTrack) continue;

    AnaFGDParticle* seg = static_cast<AnaFGDParticle*>(MakeFgdTrack());
    FillFgdTrackInfo(*fgdTrack, seg);
    track->FGDSegments[track->nFGDSegments++] = seg;
  }
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillEcalInfo(
    ND::TGlobalReconModule::TGlobalPID& globalTrack, AnaTrack* track) {
  //*****************************************************************************
  track->nECALSegments = 0;

  // Fill Tracker ECAL and DsEcal
  for (int jj = 0; jj < globalTrack.NECALs; jj++) {
    if ((unsigned int)track->nECALSegments == NMAXECALS) break;

    ND::TGlobalReconModule::TECALObject* ecalTrack =
        (ND::TGlobalReconModule::TECALObject*)(*(globalTrack.ECAL))[jj];
    if (!ecalTrack) continue;

    AnaECALParticle* seg = static_cast<AnaECALParticle*>(MakeEcalTrack());
    FillEcalTrackInfo(*ecalTrack, seg);
    track->ECALSegments[track->nECALSegments++] = seg;
  }

  // Fill P0DEcal
  for (int jj = 0; jj < globalTrack.NP0DECALs; jj++) {
    if ((unsigned int)track->nECALSegments == NMAXECALS) break;

    ND::TGlobalReconModule::TECALObject* ecalTrack =
        (ND::TGlobalReconModule::TECALObject*)(*(globalTrack.P0DECAL))[jj];
    if (!ecalTrack) continue;

    AnaECALParticle* seg = static_cast<AnaECALParticle*>(MakeEcalTrack());
    FillEcalTrackInfo(*ecalTrack, seg);
    track->ECALSegments[track->nECALSegments++] = seg;
  }
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillSmrdInfo(
    ND::TGlobalReconModule::TGlobalPID& globalTrack, AnaTrack* track) {
  //*****************************************************************************
  track->nSMRDSegments = 0;
  for (int jj = 0; jj < globalTrack.NSMRDs; jj++) {
    if ((unsigned int)track->nSMRDSegments == NMAXSMRDS) break;

    ND::TGlobalReconModule::TSMRDObject* smrdTrack =
        (ND::TGlobalReconModule::TSMRDObject*)(*(globalTrack.SMRD))[jj];
    if (!smrdTrack) continue;

    AnaSMRDParticle* seg = static_cast<AnaSMRDParticle*>(MakeSmrdTrack());
    FillSmrdTrackInfo(*smrdTrack, seg);
    track->SMRDSegments[track->nSMRDSegments++] = seg;
  }
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillP0dInfo(
    ND::TGlobalReconModule::TGlobalPID& globalTrack, AnaTrack* track) {
  //*****************************************************************************
  track->nP0DSegments = 0;
  for (int jj = 0; jj < globalTrack.NP0Ds; jj++) {
    if ((unsigned int)track->nP0DSegments == NMAXP0DS) break;

    ND::TGlobalReconModule::TP0DObject* p0dTrack =
        (ND::TGlobalReconModule::TP0DObject*)(*(globalTrack.P0D))[jj];
    if (!p0dTrack) continue;

    AnaP0DParticle* seg = static_cast<AnaP0DParticle*>(MakeP0dTrack());

    FillP0dTrackInfo(*p0dTrack, seg);
    // Add P0D energy loss
    Float_t entrance[4] = {float(globalTrack.EntrancePosition[0].X()),
                           float(globalTrack.EntrancePosition[0].Y()),
                           float(globalTrack.EntrancePosition[0].Z()),
                           float(globalTrack.EntrancePosition[0].T())};

    if (anaUtils::InDetVolume(SubDetId::kTPC1, entrance))
      seg->ELoss = track->Momentum - globalTrack.EntranceMomentum[0];
    else
      seg->ELoss = -99999;

    track->P0DSegments[track->nP0DSegments++] = seg;

    // ---- TEST HIGHLAND2-P0D ----
    // Put the object with info from TP0DReconModule tree as a constituent of
    // the Global track. For the moment use the P0DSegments array to avoid
    // modifiying psyche
    // Once this is validated it will be moved into the P0DSegments array in
    // AnaTrackB
    if (_isUsingReconDirP0DNew) {
      track->nP0DSegments = 0;
      ND::TP0DReconModule::TP0DParticle* p0dParticle =
          GetP0DReconParticleWithUniqueID(p0dTrack->UniqueID);
      if (p0dParticle) {
        AnaP0DParticle* part = MakeP0DParticle();
        track->P0DSegments[track->nP0DSegments++] = part;
        // Fill info into the P0DParticle
        FillP0DParticleInfo(*p0dParticle, part);
      }
    }
    // ----------------------------
  }
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillTrackerInfo(
    ND::TGlobalReconModule::TGlobalPID& globalTrack, AnaTrack* track) {
  //*****************************************************************************

  for (int jj = 0; jj < globalTrack.NTRACKERs; jj++) {
    if (track->TRACKERSegments.size() == NMAXTRACKERS) break;

    ND::TGlobalReconModule::TTrackerObject* trTrack =
        (ND::TGlobalReconModule::TTrackerObject*)(*(globalTrack.TRACKER))[jj];
    if (!trTrack) continue;

    AnaTrackerTrack* seg = static_cast<AnaTrackerTrack*>(MakeTrackerTrack());
    FillTrackerTrackInfo(*trTrack, seg);
    track->TRACKERSegments.push_back(seg);
  }
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillSubdetectorTrackInfo(
    ND::TSubBaseObject& subTrack, AnaParticleB* seg) {
  //*****************************************************************************

  seg->NHits = subTrack.NHits;

  AnaSubTrack* seg2 = dynamic_cast<AnaSubTrack*>(seg);
  if (seg2) seg2->Length = subTrack.Length;

  seg->NNodes = subTrack.NNodes;
  anaUtils::VectorToArray(subTrack.FrontDirection, seg->DirectionStart);
  anaUtils::VectorToArray(subTrack.BackDirection, seg->DirectionEnd);
  anaUtils::VectorToArray(subTrack.FrontPosition, seg->PositionStart);
  anaUtils::VectorToArray(subTrack.BackPosition, seg->PositionEnd);
  seg->UniqueID = subTrack.UniqueID;

  FillTrueParticleRecoInfo(subTrack.TrueParticle, seg->TrueObject, false);
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillTpcTrackInfo(
    ND::TGlobalReconModule::TTPCObject& tpcTrack, AnaTPCParticle* seg) {
  //*****************************************************************************

  FillSubdetectorTrackInfo(tpcTrack, seg);

  convUtils::ConvertLocalDetEnumToBitField(seg->Detector, tpcTrack.Detector - 1,
                                           SubDetId::kTPC);

  seg->Charge = tpcTrack.Charge;
  seg->Momentum = tpcTrack.FrontMomentum;
  seg->MomentumError = tpcTrack.FrontMomentumError;
  seg->MomentumEnd = tpcTrack.BackMomentum;

#if VERSION_HAS_BFIELD_REFIT_FULL
  seg->RefitCharge = tpcTrack.RefitCharge;
  seg->RefitMomentum = tpcTrack.RefitMomentum;
  anaUtils::VectorToArray(tpcTrack.RefitDirection, seg->RefitDirection);
  anaUtils::VectorToArray(tpcTrack.RefitPosition, seg->RefitPosition);
#endif

#if VERSION_HAS_BFIELD_REFIT_BASIC
  seg->RefitMomentum = tpcTrack.PDist;
#endif

#if VERSION_HAS_EFIELD_REFIT
  seg->EFieldRefitMomentum = tpcTrack.PEField;
#endif

  seg->Pullmu = tpcTrack.PullMuon;
  seg->Pullele = tpcTrack.PullEle;
  seg->Pullp = tpcTrack.PullProton;
  seg->Pullpi = tpcTrack.PullPion;
  seg->Pullk = tpcTrack.PullKaon;

  seg->dEdxMeas = tpcTrack.Ccorr;

  seg->dEdxexpMuon = tpcTrack.dEdxexpMuon;
  seg->dEdxexpEle = tpcTrack.dEdxexpEle;
  seg->dEdxexpPion = tpcTrack.dEdxexpPion;
  seg->dEdxexpProton = tpcTrack.dEdxexpProton;
  seg->dEdxexpKaon = tpcTrack.dEdxexpKaon;

  seg->dEdxSigmaMuon = tpcTrack.SigmaMuon;
  seg->dEdxSigmaEle = tpcTrack.SigmaEle;
  seg->dEdxSigmaPion = tpcTrack.SigmaPion;
  seg->dEdxSigmaProton = tpcTrack.SigmaProton;
  seg->dEdxSigmaKaon = tpcTrack.SigmaKaon;

  // Make truth-reco association, but don't set the purity of the track, as
  // that is reserved for the global purity. Instead we record the TPC-specific
  // purity in seg->Purity.
  FillTrueParticleRecoInfo(tpcTrack.TrueParticle, seg->TrueObject, false);

  seg->Purity = tpcTrack.TrueParticle.Pur;

// For production 7 iteration substitute the NNodes (later treated as a figure
// of merit for track quality)
// with the actual number of horizontal and vertical clusters used in the fit
// (as recommended by TREx group and can be indeed
// considered as a proper characteristic of track quality)
#if VERSION_HAS_NUMBER_OF_FITTED_CLUSTERS
  seg->NNodes =
      tpcTrack.NbFittedVerticalClusters + tpcTrack.NbFittedHorizontalClusters;
#endif
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillFgdTrackInfo(
    ND::TGlobalReconModule::TFGDObject& fgdTrack, AnaFGDParticle* seg) {
  //*****************************************************************************

  FillSubdetectorTrackInfo(fgdTrack, seg);
  convUtils::ConvertLocalDetEnumToBitField(seg->Detector, fgdTrack.Detector - 1,
                                           SubDetId::kFGD);

  seg->E = fgdTrack.E;
  seg->X = fgdTrack.x;
  seg->Containment = fgdTrack.fgdContainment;
  seg->AvgTime = fgdTrack.avgtime;

  seg->Pullmu = fgdTrack.PullMuon;
  seg->Pullp = fgdTrack.PullProton;
  seg->Pullpi = fgdTrack.PullPion;
  seg->Pullno = fgdTrack.PullNotSet;

  seg->Vertex1x1 = fgdTrack.fgdVA_verQ;
  seg->Vertex3x3 = fgdTrack.fgdVA_verNearQ;
  seg->Vertex5x5 = fgdTrack.fgdVA_verNextNearQ;
  seg->Vertex7x7 = fgdTrack.fgdVA_verNextNextNearQ;

  // New FGD2 Vertex Activity Variables
  /*
  seg->Vertex2x3 = fgdTrack.fgdVA_verNearQ_rect;
  seg->Vertex2x5 = fgdTrack.fgdVA_verNextNearQ_rect;
  seg->Vertex2x7 = fgdTrack.fgdVA_verNextNextNearQ_rect;
  */
  seg->VertexLayer = fgdTrack.fgdVA_verLayQ;
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillTrackerTrackInfo(
    ND::TGlobalReconModule::TTrackerObject& trackerTrack,
    AnaTrackerTrack* seg) {
  //*****************************************************************************

  FillSubdetectorTrackInfo(trackerTrack, seg);

  // seg->Detector = trackerTrack.Detector;

  convUtils::ConvertTrackerDetFieldToBitField(seg->Detector,
                                              trackerTrack.Detector);

  seg->Charge = trackerTrack.Charge;
  seg->Momentum = trackerTrack.FrontMomentum;
  seg->MomentumEnd = trackerTrack.BackMomentum;
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillEcalTrackInfo(
    ND::TGlobalReconModule::TECALObject& ecalTrack, AnaECALParticle* seg) {
  //*****************************************************************************

  FillSubdetectorTrackInfo(ecalTrack, seg);

  convUtils::ConvertLocalDetEnumToBitField(
      seg->Detector, ecalTrack.Detector - 1, SubDetId::kECAL);

#if !VERSION_PROD7_DEVEL
  seg->EMEnergy = ecalTrack.EMEnergy;
  seg->EDeposit = ecalTrack.EDeposit;
  seg->IsShowerLike = ecalTrack.IsShowerLike;
#else
  seg->EMEnergy = ecalTrack.EMEnergyFit_Result;
  seg->EDeposit = ecalTrack.EDeposit;
  // This is NOT a PID, just a hint as to what ecalRecon thought the deposite
  // might be most like. Please use LLR-style PID variables at the analysis
  // level
  seg->IsShowerLike = (ecalTrack.FilledAsType == 1);
#endif

  seg->AvgTime = ecalTrack.AverageHitTime;
  seg->Length = ecalTrack.Length;
#if VERSION_HAS_ECAL_LLR
  // ecal pid variables
  seg->PIDMipEm = ecalTrack.LLR_MIP_EM;
  seg->PIDMipPion = ecalTrack.LLR_MIP_Pion;
  seg->PIDEmHip = ecalTrack.LLR_EM_HIP;
#endif

#if VERSION_PROD7_DEVEL
  seg->Containment = ecalTrack.Containment;
  seg->MostUpStreamLayerHit = ecalTrack.InnerMostLayerHit;

  seg->PIDMIPEM_LowMomentum = ecalTrack.LLR_MIP_EM_LowMomentum;
  seg->AverageZPosition = ecalTrack.AverageZPosition;
  seg->EMEnergyFit_Uncertainty = ecalTrack.EMEnergyFit_Uncertainty;
  seg->FilledAsType = ecalTrack.FilledAsType;
  seg->PID_Circularity = ecalTrack.PID_Circularity;
  seg->PID_Angle = ecalTrack.PID_Angle;
  seg->PID_ShowerAngle = ecalTrack.PID_ShowerAngle;
  seg->PID_Asymmetry = ecalTrack.PID_Asymmetry;
  seg->PID_TruncatedMaxRatio = ecalTrack.PID_TruncatedMaxRatio;
  seg->PID_TransverseChargeRatio = ecalTrack.PID_TransverseChargeRatio;
  seg->PID_FrontBackRatio = ecalTrack.PID_FrontBackRatio;
  seg->PIDMipHip_VACut = ecalTrack.LLR_MIP_HIP_VACut;
  seg->Thrust = ecalTrack.Thrust;
  anaUtils::VectorToArray(ecalTrack.ThrustOrigin, seg->ThrustOrigin);
  anaUtils::VectorToArray(ecalTrack.ThrustAxis, seg->ThrustAxis);
  seg->InnerMostLayerHit = ecalTrack.InnerMostLayerHit;
  seg->OuterMostLayerHit = ecalTrack.OuterMostLayerHit;
  anaUtils::VectorToArray(ecalTrack.Width, seg->Width);
#endif

  for (int i = 0; i < NTrECalObjects; i++) {
    ND::TTrackerECALReconModule::TECALReconObject* ecalObject =
        (ND::TTrackerECALReconModule::TECALReconObject*)(*TrECalObjects)[i];

    if (ecalTrack.UniqueID == ecalObject->UniqueID) {
#if !VERSION_HAS_ECAL_LLR
      // In this case, we need to find the LLR values ourselves
      _ecalPidCalc->CalculateLikelihoodValues(*ecalObject);
      seg->PIDMipEm = _ecalPidCalc->GetLikelihoodValue("MipEm");
      seg->PIDMipPion = _ecalPidCalc->GetLikelihoodValue("MipPion");
      seg->PIDEmHip = _ecalPidCalc->GetLikelihoodValue("EmHip");
#endif
#if !VERSION_PROD7_DEVEL
      seg->Containment = ecalObject->Containment;
      seg->MostUpStreamLayerHit = ecalObject->mostUpStreamLayerHit;
#endif

#if !VERSION_HAS_REMOVED_OBSOLETE_ECAL_PID_VARS
      seg->TrShVal = ecalObject->PID_TrShval;
#endif

      anaUtils::VectorToArray(ecalObject->Shower.Position.Vect(),
                              seg->ShowerPosition);
    }
  }
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillSmrdTrackInfo(
    ND::TGlobalReconModule::TSMRDObject& smrdTrack, AnaSMRDParticle* seg) {
  //*****************************************************************************

  FillSubdetectorTrackInfo(smrdTrack, seg);

  convUtils::ConvertLocalDetEnumToBitField(
      seg->Detector, smrdTrack.Detector - 1, SubDetId::kSMRD);

  seg->AvgTime = smrdTrack.avgtime;
  seg->EDeposit = smrdTrack.EDeposit;
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillP0dTrackInfo(
    ND::TGlobalReconModule::TP0DObject& p0dTrack, AnaP0DParticle* seg) {
  //*****************************************************************************

  FillSubdetectorTrackInfo(p0dTrack, seg);
  seg->Length = p0dTrack.Length;
  convUtils::ConvertLocalDetEnumToBitField(seg->Detector, p0dTrack.Detector - 1,
                                           SubDetId::kP0D);
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillTrueParticleRecoInfo(
    ND::TTrueParticle& trueParticle, AnaTrueObjectC*& trueObj, bool setpurity) {
  //*****************************************************************************

  // Compare the IDs to find the AnaTrueParticle associated with this
  // trueParticle.
  // Set the Purity, which is a truth-reco quantity

  if (trueParticle.ID == -1)
    return;  // particle without true particle associated

  std::vector<AnaTrueParticleB*>::iterator it;
  for (it = _spill->TrueParticles.begin(); it != _spill->TrueParticles.end();
       it++) {
    AnaTrueParticleB* truePart = *it;

    if (trueParticle.ID == truePart->ID) {
      trueObj = truePart;
      if (setpurity) {
        static_cast<AnaTrueParticle*>(trueObj)->Purity = trueParticle.Pur;
      }
      break;  // Stop when association is found.
    }
  }
  // if not found means that the correspondent Trajectories was not saved in the
  // oaAnalysis file
  // this is the case when there are no G4Points
  // it often happens in SMRD (Detectors=8), sometimes in DSECal / ECal
  // (Detectors=7/9), might be also P0D (Detectors=6) and TPCs (Detectors=1,2,3)
  // it seems it never happens in FGDs (Detectors=4,5)
  return;
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillTrueParticleInfo(
    ND::TTruthTrajectoriesModule::TTruthTrajectory* truthTraj,
    AnaTrueParticle* truePart, AnaSpill* spill) {
  //*****************************************************************************

  // This is not known at this level We need the reco-truth association
  truePart->Purity = 0;

  if (truthTraj == NULL) {
    truePart->ID        = 0;
    truePart->PDG       = 0;
    truePart->PrimaryID = 0;
    truePart->ParentID  = 0;
    truePart->GParentID = 0;
    anaUtils::VectorToArray(TLorentzVector(0, 0, 0, 0), truePart->Position);
    truePart->Momentum = 0;
    anaUtils::VectorToArray(TVector3(0, 0, 0), truePart->Direction);
    truePart->Charge = 0;
    truePart->Bunch = -1;
    truePart->VertexID = -1;

    return;
  }

  truePart->ID = truthTraj->ID;
  truePart->PDG = truthTraj->PDG;
  truePart->PrimaryID = truthTraj->PrimaryID;
  truePart->ParentID = truthTraj->ParentID;
  anaUtils::VectorToArray(truthTraj->InitPosition, truePart->Position);
  anaUtils::VectorToArray(truthTraj->FinalPosition, truePart->PositionEnd);
  truePart->Momentum = truthTraj->InitMomentum.Vect().Mag();
  anaUtils::VectorToArray(
      (1 / truePart->Momentum) * truthTraj->InitMomentum.Vect(),
      truePart->Direction);
  truePart->Charge = truthTraj->Charge;
  truePart->VertexID = -1;
  truePart->VertexIndex = -1;

  truePart->Bunch =
      _bunching.GetBunch(truthTraj->InitPosition.T() + _trueBunchShift,
                         spill->EventInfo->Run, IsMC, cosmic_mode);

  // 0=FGD1,1=FGD2,2=P0D,3=DsEcal,4=BrlEcal,5=P0DEcal,6=TPC1,7=TPC2,8=TPC3,9=MRD,10=Ingrid,11=cavern

  unsigned int nCrossers = std::min(
      (unsigned int)truthTraj->TraceSubdetectors.size(), NMAXCROSSEDDET);

  truePart->nDetCrossings = 0;
  anaUtils::CreateArray(truePart->DetCrossings, nCrossers);
  for (unsigned int i = 0; i < nCrossers; i++) {
    AnaDetCrossingB* cross = new AnaDetCrossingB();

    convUtils::ConvertTrueParticleDetEnumToBitField(
        cross->Detector, truthTraj->TraceSubdetectors[i]);
    cross->InActive = truthTraj->TraceInActive[i];
    anaUtils::VectorToArray(truthTraj->TraceEntrancePosition[i],
                            cross->EntrancePosition);
    anaUtils::VectorToArray(truthTraj->TraceExitPosition[i],
                            cross->ExitPosition);
    anaUtils::VectorToArray(truthTraj->TraceEntranceMomentum[i],
                            cross->EntranceMomentum);
    anaUtils::VectorToArray(truthTraj->TraceExitMomentum[i],
                            cross->ExitMomentum);

    truePart->DetCrossings[truePart->nDetCrossings] = cross;
    truePart->nDetCrossings++;
  }

  // TEMPORARILY set the VertexIndex to the ID of the true vertex. This will
  // be overwritten in FillTrueVertexInfo() to point to the actual index in the
  // spill.TrueVertices vector that the vertex is saved at.

  ND::TTruthVerticesModule::TTruthVertex* vtx =
      _truthUtils.GetVertexOfTrajectory(truthTraj);
  if (vtx) {
    truePart->VertexIndex = vtx->ID;
    truePart->VertexID = vtx->ID;
  }

  ND::TTruthTrajectoriesModule::TTruthTrajectory* parentTraj =
      _truthUtils.GetParentTrajectory(truthTraj->ID);
  ND::TTruthTrajectoriesModule::TTruthTrajectory* grandparentTraj = NULL;

  if (parentTraj != NULL) {
    truePart->ParentPDG = parentTraj->PDG;

    grandparentTraj = _truthUtils.GetParentTrajectory(parentTraj->ID);

    if (grandparentTraj != NULL) {
      truePart->GParentPDG  = grandparentTraj->PDG;
      truePart->GParentID   = grandparentTraj->ID;
    }
  }

  // is this track a decay photon from an FGD or P0D pi0?
  if (parentTraj == NULL) return;  // give up if parentTraj is undefined
  if (truePart->PDG == ParticleId::kPhotonPDG and     // this is a photon
      truePart->ParentPDG == ParticleId::kPi0PDG and  // its parent is a pi0
      truePart->PrimaryID ==
          truePart->ParentID and     // its primary is its parent pi0
      parentTraj->ParentID == 0 and  // the parent pi0 was produced at the
                                     // vertex (and hence has no parent of its
                                     // own)
      parentTraj->TraceSubdetectors[0] <
          3) {  // the parent pi0 was produced in FGD1, FGD2 or P0D

    truePart->IsTruePrimaryPi0DecayPhoton = true;

#ifdef DEBUG
    std::cout
        << "oaAnalysisTreeConverter::FillTrueParticleInfo() truePart with ID:"
        << setw(6) << truePart->ID << " and ParentID:" << setw(6)
        << parentTraj->ID
        << " is a true subdet:" << parentTraj->TraceSubdetectors[0]
        << " pi0 decay photon" << std::endl;
#endif

  } else {
    // is this track the child of an existing pi0 decay photon?
    // loop over TrueParticles
    for (std::vector<AnaTrueParticleB*>::iterator it =
             spill->TrueParticles.begin();
         it != spill->TrueParticles.end(); it++) {
      // give up if TrueParticle is not a pi0 decay photon
      if (not static_cast<AnaTrueParticle*>(*it)->IsTruePrimaryPi0DecayPhoton)
        continue;

      // if true particle is a pi0 decay photon does the ParentID of PrimaryID
      // of this track match it?
      if (truePart->ParentID == (*it)->ID or truePart->PrimaryID == (*it)->ID) {
        truePart->IsTruePrimaryPi0DecayPhotonChild = true;

#ifdef DEBUG
        std::cout << "oaAnalysisTreeConverter::FillTrueParticleInfo() truePart "
                     "with ID:"
                  << setw(6) << truePart->ID << " and ParentID:" << setw(6)
                  << parentTraj->ID
                  << " is a true subdet:" << parentTraj->TraceSubdetectors[0]
                  << " pi0 decay photon child" << std::endl;
#endif
      }
    }
  }

  return;
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillTrueInfo(AnaSpill* spill) {
  //*****************************************************************************

  _containedTrueParticles.clear();

  if (!spill->GetIsMC()) return;

  // First create all the true trajectories
  for (int i = 0; i < NTruthTrajs; i++) {
    if (spill->TrueParticles.size() == NMAXTRUEPARTICLES) {
      std::cout << "INFO: event " << EventID << " has " << NTruthTrajs
                << " true trajectories (too many), ";
      std::cout << "only the first " << NMAXTRUEPARTICLES
                << " will be stored (=> some warnings might appear)"
                << std::endl;
      break;
    }

    ND::TTruthTrajectoriesModule::TTruthTrajectory* traj =
        (ND::TTruthTrajectoriesModule::TTruthTrajectory*)(*TruthTrajs)[i];

    if (!traj) continue;

    // Create an AnaTrueParticle for each TruthTrajectory
    AnaTrueParticle* trueParticle =
        static_cast<AnaTrueParticle*>(MakeTrueParticle());
    FillTrueParticleInfo(traj, trueParticle, spill);
    if (trueParticle->PDG == 0) {
      delete trueParticle;
    } else {
      // Ignore fully contained trajectories in some subdetectors. Numbering
      // follows ToaAnalysisUtils convention
      int InitSubdetector = (traj->TraceSubdetectors.empty())
                                ? -1
                                : traj->TraceSubdetectors.front();
      if (_ignoreDsECalContainedTrueObjects && InitSubdetector == 3 &&
          traj->TraceSubdetectors.size() == 1)
        _containedTrueParticles.push_back(trueParticle);
      if (_ignoreBrECalContainedTrueObjects && InitSubdetector == 4 &&
          traj->TraceSubdetectors.size() == 1)
        _containedTrueParticles.push_back(trueParticle);
      if (_ignoreP0DECalContainedTrueObjects && InitSubdetector == 5 &&
          traj->TraceSubdetectors.size() == 1)
        _containedTrueParticles.push_back(trueParticle);
      if (_ignoreSMRDContainedTrueObjects && InitSubdetector == 9 &&
          traj->TraceSubdetectors.size() == 1)
        _containedTrueParticles.push_back(trueParticle);
      if (_ignoreINGRIDContainedTrueObjects && InitSubdetector == 10 &&
          traj->TraceSubdetectors.size() == 1)
        _containedTrueParticles.push_back(trueParticle);
      spill->TrueParticles.push_back(trueParticle);
    }
  }

  // VERTICES
  if (Neut && NNVtx != NTruthVertices)
    std::cout << "ERROR: NNVtx (in TTruthVerticesModule) = " << NNVtx
              << " vs NTruthVertices (in rooTrackerVtx) = " << NTruthVertices
              << " in EventID " << EventID << std::endl;
  if (Genie && NGVtx != NTruthVertices)
    std::cout << "ERROR: NGVtx (in TTruthVerticesModule) = " << NGVtx
              << " vs NTruthVertices (in rooTrackerVtx) " << NTruthVertices
              << " in EventID " << EventID << std::endl;

  // Loop over true vertices
  _foundPauliBlocked = false;
  _foundCohOnH = false;
  std::vector<AnaTrueVertexB*>& vertices = spill->TrueVertices;
  ND::TTruthVerticesModule::TTruthVertex* true_vertex = NULL;
  for (int v = 0; v < NTruthVertices; v++) {
    if (vertices.size() == NMAXTRUEVERTICES) {
      std::cout << "INFO: event " << EventID << " has " << NTruthVertices
                << " true vertices (too many), ";
      std::cout << "only the first " << NMAXTRUEVERTICES
                << " will be stored (=> some warnings might appear)"
                << std::endl;
      break;
    }

    true_vertex =
        (ND::TTruthVerticesModule::TTruthVertex*)TruthVertices->UncheckedAt(v);

    if (!true_vertex) continue;

    AnaTrueVertex* vertex = static_cast<AnaTrueVertex*>(MakeTrueVertex());
    // currently this returns always true, bad vertices will be deleted
    // afterward in DeleteBadObjects
    // in order to remove first also the associated reconstructed particles and
    // vertices
    bool goodVtx = FillTrueVertexInfo(true_vertex, vertex, v, spill);
    if (!goodVtx) {
      delete vertex;
      continue;
    } else {
      vertices.push_back(vertex);
    }

  }  // End loop over vertices
}

//*****************************************************************************
bool oaAnalysisTreeConverter::FillTrueVertexInfo(
    ND::TTruthVerticesModule::TTruthVertex* true_vertex, AnaTrueVertex* vertex,
    int v, AnaSpill* spill) {
  //*****************************************************************************

  vertex->RooVtxEntry = _RooVtxEntryInCurrentInputFile;

  if ((true_vertex->ReactionCode).find(":") != std::string::npos) {
    // local conversion from Genie to Neut convention. This value will be
    // overwritten in FillTrueVertexRooInfo if the GRooTrackerVertex exists
    vertex->ReacCode = GetGenieReactionCode(true_vertex->ReactionCode);
  } else {
    // Neut convention
    vertex->ReacCode = atoi((true_vertex->ReactionCode).c_str());
  }

  vertex->ID =
      true_vertex->ID;  // this needs to be before calling FillTrueVertexRooInfo

  // The detector info provided by oaAnalysis should be converted into psyche
  // definitions
  vertex->Detector = 0;
  convUtils::ConvertTrueParticleDetEnumToBitField(vertex->Detector,
                                                  true_vertex->Subdetector);

  vertex->NuPDG =
      true_vertex->NeutrinoPDG;  // also this, but just for some cout
  vertex->NuEnergy = true_vertex->NeutrinoMomentum.E();  // will be replaced in
  // FillTrueVertexRooInfo if
  // rooTrackerVtx exists

  // currently this returns always true, bad vertices will be deleted afterward
  // in DeleteBadObjects
  // in order to remove first also the associated reconstructed tracks and
  // vertices
  //
  vertex->RooVtxIndex = -1;
  bool goodRooVtx = anaUtils::FillTrueVertexRooInfo(
      vertex, RunID, Neut, Genie, NNVtx, NVtx, NGVtx, GVtx, _foundCohOnH,
      _foundPauliBlocked);
  if (!goodRooVtx) return false;

  vertex->TargetMom = true_vertex->TargetMomentum.Vect().Mag();
  vertex->TargetPDG = true_vertex->TargetPDG;
  anaUtils::VectorToArray(true_vertex->Position, vertex->Position);
  vertex->Bunch =
      _bunching.GetBunch(true_vertex->Position.T() + _trueBunchShift,
                         spill->EventInfo->Run, IsMC, cosmic_mode);

  // get the number of true vertices per bunch
  // nvtxPerBunch[ _std_TrueVertexBunch[_std_NTrueVertices] ] ++;

  if (true_vertex->NeutrinoMomentum.Vect().Mag() > 0)
    anaUtils::VectorToArray((1 / true_vertex->NeutrinoMomentum.Vect().Mag()) *
                                true_vertex->NeutrinoMomentum.Vect(),
                            vertex->NuDir);

  if (true_vertex->TargetMomentum.Vect().Mag() > 0)
    anaUtils::VectorToArray((1 / true_vertex->TargetMomentum.Vect().Mag()) *
                                true_vertex->TargetMomentum.Vect(),
                            vertex->TargetDir);

  // check (temporarily) that the neutrino energy corresponds in RooTrackerVtx
  // and in TruthTrajectoryModule
  // (even right after the assignement vertex-> they are not exactly equal)
  // (even in the assignement of vertex->NuEnergy itself, some differences are
  // introduced)
  float Ediff =
      std::max(vertex->NuEnergy, float(true_vertex->NeutrinoMomentum[3])) -
      std::min(vertex->NuEnergy, float(true_vertex->NeutrinoMomentum[3]));
  if ((vertex->RooVtxIndex > -1) &&
      (Ediff > std::numeric_limits<float>::epsilon())) {
    std::cout << "Error: the neutrino energy differs between "
                 "TruthTrajectoryModule and RooTrackerVtx!!! ID "
              << true_vertex->ID << std::endl;
  }

  std::stringstream ssRun;
  ssRun << RunID;
  bool issand = ((ssRun.str())[4] == '7');

  // Loop over TruthTrajectories
  bool found = false;
  for (int i = 0; i < true_vertex->NPrimaryTraj; i++) {
    ND::TTruthTrajectoriesModule::TTruthTrajectory* truthTraj =
        _truthUtils.GetTrajectoryById(true_vertex->PrimaryTrajIDs[i]);
    if (truthTraj == NULL) continue;

    // Sometimes FSI produces a lepton pair (listed secondly)
    if ((vertex->NuPDG == 14 && truthTraj->PDG == 13) ||
        (vertex->NuPDG == 12 && truthTraj->PDG == 11) ||
        (vertex->NuPDG == -14 && truthTraj->PDG == -13) ||
        (vertex->NuPDG == -12 && truthTraj->PDG == -11)) {
      // if already filled do not overwrite (the first lepton listed is the one
      // coming from the neutrino)
      if (!found) {
        found = true;

        // If no rooTrackerVtx, fill lepton vars in the old way, using the
        // TruthTrajectoryModule
        if (!Neut && !Genie) {
          vertex->LeptonPDG = truthTraj->PDG;
          vertex->Q2 =
              -(truthTraj->InitMomentum - true_vertex->NeutrinoMomentum).Mag2();
          vertex->LeptonMom = truthTraj->InitMomentum.Vect().Mag();
          if (vertex->LeptonMom > 0)
            anaUtils::VectorToArray(
                (1 / vertex->LeptonMom) * truthTraj->InitMomentum.Vect(),
                vertex->LeptonDir);
        }

        // check that LeptonMom corresponds in RooTrackerVtx and in
        // TruthTrajectoryModule
        // (even in the assignement of vertex->LeptonMom itself some differences
        // are introduced)
        else if (truthTraj->InitMomentum.Vect().Mag() >
                 0) {     // if lepton found (might not be saved if without G4
                          // points)
          if (!issand) {  // for sand muons, the momentum at nd280 is always
                          // different from the one at the vertex
            if (fabs(vertex->LeptonMom - truthTraj->InitMomentum.Vect().Mag()) >
                0.005) {  // seen differences up to 0.002 (in Genie)
              if (vertex->NPrimaryParticles[ParticleId::kLeptons] > 1) {
                std::cout << "INFO: LeptonMom differs between "
                             "TruthTrajectoryModule and RooTrackerVtx: there "
                             "is a lepton pair produced by FSI and probably "
                             "the lepton coming from the neutrino is not saved "
                             "in TruthTrajectoryModule because it didn't have "
                             "G4 points (we use RooTrackerVtx info anyway): "
                             "Nmuon "
                          << vertex->NPrimaryParticles[ParticleId::kMuon]
                          << ", Nantimuon "
                          << vertex->NPrimaryParticles[ParticleId::kAntiMuon]
                          << ", NuPDG " << vertex->NuPDG << std::endl;

              } else if (vertex->LeptonMom ==
                         -999) {  // happens in Genie... to be debugged
                std::cout << "ERROR: LeptonMom is -999 (something wrong in "
                             "RooTrackerVtx ?), setting it from "
                             "TruthTrajectoryModule ("
                          << truthTraj->InitMomentum.Vect().Mag()
                          << "), for vertex ID " << true_vertex->ID
                          << std::endl;
                vertex->LeptonMom = truthTraj->InitMomentum.Vect().Mag();
                vertex->LeptonPDG = truthTraj->PDG;
                vertex->Q2 =
                    -(truthTraj->InitMomentum - true_vertex->NeutrinoMomentum)
                         .Mag2();
                vertex->LeptonMom = truthTraj->InitMomentum.Vect().Mag();
                if (vertex->LeptonMom > 0)
                  anaUtils::VectorToArray(
                      (1 / vertex->LeptonMom) * truthTraj->InitMomentum.Vect(),
                      vertex->LeptonDir);

              } else {
                std::cout << "ERROR: LeptonMom differs between RooTrackerVtx "
                             "and TruthTrajectoryModule! "
                          << vertex->LeptonMom << " - "
                          << truthTraj->InitMomentum.Vect().Mag() << " = "
                          << vertex->LeptonMom -
                                 truthTraj->InitMomentum.Vect().Mag()
                          << ", for vertex ID " << true_vertex->ID << std::endl;
              }
              /*
                          } else if (fabs(vertex->Q2 - ( -
                 (truthTraj->InitMomentum -
                 true_vertex->NeutrinoMomentum).Mag2())) > 20) {
                          // they differ even more than 10 (Q2 differs from its
                 formula right after the assignement)
                            std::cout << "Error: NeutrinoMomentum differs
                 between TruthTrajectoryModule and RooTrackerVtx!!! " <<
                 vertex->Q2 - ( - (truthTraj->InitMomentum -
                 true_vertex->NeutrinoMomentum).Mag2()) << std::endl;
              */
            }
          }
        }

        break;  // break when the lepton is found
      }

    }  // end if lepton

    // If no rooTrackerVtx, fill true proton vars, using the
    // TruthTrajectoryModule (old way)
    else if (!Neut && !Genie && truthTraj->PDG == 2212 &&
             truthTraj->ParentID == 0) {
      // if many, take the first one, that should be the first primary one (not
      // from FSI)
      if (vertex->ProtonMom < 0) {
        vertex->ProtonMom = truthTraj->InitMomentum.Vect().Mag();
        if (vertex->ProtonMom > 0)
          anaUtils::VectorToArray(
              (1. / vertex->ProtonMom) * truthTraj->InitMomentum.Vect(),
              vertex->ProtonDir);
      }
    }  // end if proton

    // If no rooTrackerVtx, fill true pion vars, using the TruthTrajectoryModule
    // (old way)
    else if (!Neut && !Genie && truthTraj->PDG == 211 &&
             truthTraj->ParentID == 0) {
      // if many, take the highest momentum one, that should be most likely to
      // be reconstructed
      if (vertex->PionMom < truthTraj->InitMomentum.Vect().Mag()) {
        vertex->PionMom = truthTraj->InitMomentum.Vect().Mag();
        if (vertex->PionMom > 0)
          anaUtils::VectorToArray(
              (1. / vertex->PionMom) * truthTraj->InitMomentum.Vect(),
              vertex->PionDir);
      }
    }  // end if pion

  }  // end loop over TruthTrajectories

  // Notify that the lepton was not found in TruthTrajectoryModule, but only for
  // CC in FGDs (too many otherwise)
  // (it happens when it doesn't have G4 points, see bugzilla 1051)
  if (!found && !vertex->IsPauliBlocked) {
    if (anaUtils::InFiducialVolume(SubDetId::kFGD, vertex->Position) &&
        abs(vertex->ReacCode) < 30) {
      std::cout << "INFO: true lepton not found in TruthTrajectoryModule "
                   "(probably no G4 points, but we use RooTrackerVtx info), ";
      std::cout << "ReacCode " << vertex->ReacCode << ", in FV FGD"
                << true_vertex->Subdetector + 1 << ", NuPDG " << vertex->NuPDG
                << "" << std::endl;
    }
  }

  // Loop over all the true particles to find the ones that belong to this
  // vertex,
  // and set up particle<->vertex links.
  vertex->nTrueParticles = 0;
  // allocate a big array. Once it is filled we will resize it
  anaUtils::CreateArray(vertex->TrueParticles, NMAXTRUEPARTICLES);
  for (unsigned int jj = 0; jj < spill->TrueParticles.size(); jj++) {
    if ((unsigned int)vertex->nTrueParticles == NMAXTRUEPARTICLES) break;

    AnaTrueParticle* particle =
        static_cast<AnaTrueParticle*>(spill->TrueParticles[jj]);
    if (particle->VertexID == true_vertex->ID) {
      particle->TrueVertex = vertex;
      particle->VertexIndex = v;
      particle->VertexID = true_vertex->ID;
      vertex->TrueParticles[vertex->nTrueParticles++] = particle;
    }
  }

  // Give the proper size to the array
  anaUtils::ResizeArray(vertex->TrueParticles, vertex->nTrueParticles,
                        NMAXTRUEPARTICLES);

  // Fill primary trajectory info for each type of vertices
  //----------------------------------------------------------
  //    for(int ll = 0; ll < true_vertex->NPrimaryTraj; ll ++){
  //      std_truePrimaryID[_std_NTrueVertices][std_nPrimTraj] =
  //      true_vertex->PrimaryTrajIDs[ll];
  //      std_nPrimTraj ++;
  //    }

  // remove vertices with no TrueParticles. This happens when some uninteresting
  // particles are removed
  if (_removeTrueVerticesWithNoTrueParticles && vertex->nTrueParticles == 0) {
    return false;
  }

  return true;
}

//*****************************************************************************
double oaAnalysisTreeConverter::GetVertexTime(
    ND::TGlobalReconModule::TGlobalPID& globalTrack) {
  //*****************************************************************************

  return globalTrack.FrontPosition.T();
}

//*****************************************************************************
void oaAnalysisTreeConverter::GetBunchPIDs() {
  //*****************************************************************************

  for (unsigned int i = 0; i < NBUNCHES + 1; i++) _bunchPIDs[i].clear();

  for (int j = 0; j < NPIDs; j++) {
    ND::TGlobalReconModule::TGlobalPID* globalTrack =
        (ND::TGlobalReconModule::TGlobalPID*)PIDs->UncheckedAt(j);
    if (!globalTrack)
      std::cout << "debug error 1100 in oaAnalysisConverter"
                << std::endl;  // shouldn't happen!
    if (!globalTrack)
      continue;  // if no global track is found, go to the next PID

    // check that the track is valid (the momentum is not NaN nor 0, etc)
    //    if (!IsValidTrack(globalTrack)) continue;

    // --- Get the bunch number -----
    double tTrack = GetVertexTime(*globalTrack);
    int ibunch = _bunching.GetBunch(tTrack, RunID, IsMC, cosmic_mode);

    if (ibunch == -1) {
      ibunch = NBUNCHES;
    }

    _bunchPIDs[ibunch].push_back(globalTrack);
  }
}

//*****************************************************************************
void oaAnalysisTreeConverter::GetBunchLocalObjects() {
  //*****************************************************************************

  for (unsigned int i = 0; i < NBUNCHES + 1; i++) {
    _bunchTECALObjects[i].clear();
    _bunchTECALUnmatchedObjects[i].clear();
    _bunchP0DVertices[i].clear();
    _bunchP0DParticles[i].clear();
    _bunchP0DClusters[i].clear();
  }

  if (_isUsingReconDirTECAL) {
    // Loop over all the TECALReconUnmatchedObjects in the event
    for (int iTECAL = 0; iTECAL < NTrECalUnmatched; ++iTECAL) {
      ND::TTrackerECALReconModule::TECALReconUnmatchedObject* tecalObj =
          (ND::TTrackerECALReconModule::
               TECALReconUnmatchedObject*)(*TrECalUnmatched)[iTECAL];

      // Is this object in this bunch?
      Int_t ibunch = _bunching.GetBunch(tecalObj->AverageHitTime, RunID, IsMC,
                                        cosmic_mode);

      if (ibunch == -1) {
        ibunch = NBUNCHES;
      }

      _bunchTECALUnmatchedObjects[ibunch].push_back(tecalObj);
    }

    // Loop over all the TECALReconObjects in the event
    for (int iTECAL = 0; iTECAL < NTrECalObjects; ++iTECAL) {
      ND::TTrackerECALReconModule::TECALReconObject* tecalObj =
          (ND::TTrackerECALReconModule::TECALReconObject*)(*TrECalObjects)
              [iTECAL];

      Int_t ibunch = _bunching.GetBunch(tecalObj->AverageHitTime, RunID, IsMC,
                                        cosmic_mode);

      if (ibunch == -1) {
        ibunch = NBUNCHES;
      }

      _bunchTECALObjects[ibunch].push_back(tecalObj);
    }
  }

  if (_isUsingReconDirP0D) {
    // Clear the maps used to create links between the local p0d recon objects:
    // indices --> pointers
    _P0DReconVerticesMap.clear();
    _P0DReconParticlesMap.clear();
    _P0DReconClustersMap.clear();

    // Loop over all the P0D vertices in the selected AlgorithmResult
    for (int iP0D = 0; iP0D < NP0DReconVertices; ++iP0D) {
      ND::TP0DReconModule::TP0DVertex* p0dVertex =
          (ND::TP0DReconModule::TP0DVertex*)(*P0DReconVertices)[iP0D];

      Int_t ibunch =
          _bunching.GetBunch(p0dVertex->Position.T(), RunID, IsMC, cosmic_mode);
      if (ibunch == -1) {
        ibunch = NBUNCHES;
      }

      _bunchP0DVertices[ibunch].push_back(p0dVertex);

      // Make a pointer to a new AnaP0DReconVertex, ready for filling
      AnaP0DReconVertex* anaP0DRecon =
          static_cast<AnaP0DReconVertex*>(MakeP0DReconVertex());
      _P0DReconVerticesMap[p0dVertex] = anaP0DRecon;
    }

    // Loop over all the P0D particles in the event
    // (not in a given AlgorithmResult since the index in the vector of
    // particles in each vertex follows that convention)
    for (Int_t iP0D = 0; iP0D < NP0DReconParticles; ++iP0D) {
      ND::TP0DReconModule::TP0DParticle* p0dParticle =
          (ND::TP0DReconModule::TP0DParticle*)(*P0DReconParticles)[iP0D];

      // Is this object in this bunch?
      Int_t ibunch = _bunching.GetBunch(p0dParticle->Position.T(), RunID, IsMC,
                                        cosmic_mode);

      if (ibunch == -1) {
        ibunch = NBUNCHES;
      }

      _bunchP0DParticles[ibunch].push_back(p0dParticle);

      // Make a pointer to a new AnaP0DReconParticle, ready for filling
      AnaP0DReconParticle* anaP0DRecon =
          static_cast<AnaP0DReconParticle*>(MakeP0DReconParticle());
      _P0DReconParticlesMap[p0dParticle] = anaP0DRecon;
    }

    // Loop over all the P0D clusters in the event
    // (not in a given AlgorithmResult since the index in the vector of clusters
    // in each track/shower follows that convention)
    for (Int_t iP0D = 0; iP0D < NP0DReconClusters; ++iP0D) {
      ND::TP0DReconModule::TP0DCluster* p0dCluster =
          (ND::TP0DReconModule::TP0DCluster*)(*P0DReconClusters)[iP0D];

      // Is this object in this bunch?
      Int_t ibunch = _bunching.GetBunch(p0dCluster->Position.T(), RunID, IsMC,
                                        cosmic_mode);

      if (ibunch == -1) {
        ibunch = NBUNCHES;
      }

      _bunchP0DClusters[ibunch].push_back(p0dCluster);

      // Make a pointer to a new AnaP0DReconCluster, ready for filling
      AnaP0DReconCluster* anaP0DRecon =
          static_cast<AnaP0DReconCluster*>(MakeP0DReconCluster());
      _P0DReconClustersMap[p0dCluster] = anaP0DRecon;
    }
  }

  //---- TEST HIGHLAND2-P0D ----
  if (_isUsingReconDirP0DNew) {
    GetBunchLocalP0DObjects();
  }
  //------------------------
}

//*****************************************************************************
int oaAnalysisTreeConverter::GetGenieReactionCode(
    const std::string& reactionCode) {
  //*****************************************************************************

  int code = 0;

  // if(!p_silent) cout << "3: " << reactionCode << endl;
  if (reactionCode.find("charm") == string::npos &&
      reactionCode.find("Weak[CC],QES") != string::npos) {
    code = 1;
  } else if (reactionCode.find("Weak[CC],RES") != string::npos) {
    code = 11;
  } else if (reactionCode.find("charm") == string::npos &&
             reactionCode.find("Weak[CC],DIS") != string::npos) {
    code = 21;
  } else if (reactionCode.find("Weak[CC],COH") != string::npos) {
    code = 16;
  } else if (reactionCode.find("Weak[NC],RES") != string::npos) {
    code = 31;
  } else if (reactionCode.find("Weak[NC],QES") != string::npos) {
    code = 51;
  } else if (reactionCode.find("Weak[NC],COH") != string::npos) {
    code = 36;
  } else if (reactionCode.find("Weak[NC],DIS") != string::npos) {
    code = 41;
  }

  return code;
}

//*****************************************************************************
void oaAnalysisTreeConverter::GetBunchVertices() {
  //*****************************************************************************

  for (unsigned int i = 0; i < NBUNCHES + 1; i++) _bunchVertices[i].clear();

  for (int j = 0; j < NVertices; j++) {
    ND::TGlobalReconModule::TGlobalVertex* globalVertex =
        (ND::TGlobalReconModule::TGlobalVertex*)Vertices->UncheckedAt(j);
    if (!globalVertex)
      std::cout << "debug error 1101 in oaAnalysisConverter"
                << std::endl;  // shouldn't happen!
    if (!globalVertex)
      continue;  // if no global vertex is found, go to the next vertex

    // check that the track is valid (the momentum is not NaN nor 0, etc)
    //    if (!IsValidTrack(globalTrack)) continue;

    // --- Get the bunch number -----

    // global ECal vertices have a bug in the timing information
    // optionally fix from associated tracks
    if (_FixECalGlobalVerticesTime &&
        (globalVertex->AlgorithmName == "ECalHoughVtx") &&
        (globalVertex->NConstituents > 1)) {
      // std::cout << "[VERBOSE]: Global vertex (T:" << globalVertex->Position.T()
      //           << ") with " << globalVertex->NConstituents << " constituents."
      //           << std::endl;
      double SumTrackTime = 0;
      for (int i = 0; i < globalVertex->NConstituents; i++) {
        ND::TGlobalReconModule::TVertexConstituent* vConst =
            (ND::TGlobalReconModule::TVertexConstituent*)(*(
                globalVertex->Constituents))[i];
        ND::TGlobalReconModule::TGlobalPID* globalTrack =
            (ND::TGlobalReconModule::TGlobalPID*)(*PIDs)[vConst->PID];
        if (!globalTrack) {
          // std::cout << "[WARN]: Failed to find global vertex constituent."
          //           << std::endl;
        } else {
          // std::cout
          //     << "[VERBOSE]: Found global vertex constituent track with time: "
          //     << globalTrack->FrontPosition[3] << std::endl;
          SumTrackTime += globalTrack->FrontPosition[3];
        }
      }
      globalVertex->Position[3] = (SumTrackTime / globalVertex->NConstituents);
      // std::cout << "[VERBOSE]: Global vertex corrected T = "
      //           << globalVertex->Position.T() << "." << std::endl;
    }

    double tVertex = (*globalVertex).Position.T();
    int ibunch = _bunching.GetBunch(tVertex, RunID, IsMC, cosmic_mode);

    if (ibunch == -1) {
      ibunch = NBUNCHES;
    }

    _bunchVertices[ibunch].push_back(globalVertex);
  }
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillVertexInfo(
    ND::TGlobalReconModule::TGlobalVertex& globalVertex, AnaVertex* vertex,
    AnaBunch* bunch, AnaSpill* spill) {
  //*****************************************************************************

  vertex->PrimaryIndex = globalVertex.PrimaryIndex;
  anaUtils::VectorToArray(globalVertex.Position, vertex->Position);
  vertex->Bunch = bunch->Bunch;
  anaUtils::VectorToArray(globalVertex.Variance, vertex->Variance);
  vertex->Chi2 = globalVertex.Quality;
  vertex->NDOF = globalVertex.NDOF;

  // Save all recon particles associated to this vertex.
  // Loop over the constituent particles of this global vertex.
  vertex->nParticles = 0;
  anaUtils::CreateArray(vertex->Particles, globalVertex.NConstituents);
  for (int i = 0; i < globalVertex.NConstituents; i++) {
    ND::TGlobalReconModule::TVertexConstituent* vConst =
        (ND::TGlobalReconModule::TVertexConstituent*)(*(
            globalVertex.Constituents))[i];

    // this should never happen (except when masking FGD layers)
    if (vConst->PID < 0)  // it happens a lot in NuWro... why?
      std::cout << "WARNING: a reconstructed global vertex (position "
                << globalVertex.Position.X() << "," << globalVertex.Position.Y()
                << "," << globalVertex.Position.Z() << ")"
                << " is supposed to have an associated particle (PID = "
                << vConst->PID << ") which cannot be found (see bug 1161)"
                << std::endl;
    if (vConst->PID < 0) continue;

    ND::TGlobalReconModule::TGlobalPID* globalTrack =
        (ND::TGlobalReconModule::TGlobalPID*)(*PIDs)[vConst->PID];

    // Find AnaTrack associated with this global vertex
    AnaTrack* track = FindTrack(globalTrack->UniqueID, bunch, spill);

    // Fill even with a NULL pointer, to know that somewhere there is this
    // consituent
    vertex->Particles[vertex->nParticles++] = track;

    // Save vertices info in AnaTrack
    if (track) {
      track->ReconVertices.push_back(vertex);
      // choose the vertex more primary
      if (!track->ReconVertex ||
          track->ReconVertex->PrimaryIndex > vertex->PrimaryIndex) {
        // also check it is in the same bunch, so it will be the same in the
        // flattree
        if (vertex->Bunch == track->Bunch) {
          track->ReconVertex = vertex;
          track->MomentumAtVertex = vConst->Momentum.Mag();
          anaUtils::VectorToArray(
              (1 / track->MomentumAtVertex) * vConst->Momentum,
              track->DirectionAtVertex);
        }
      }
    }
  }
  // Sort tracks by momentum
  //  std::sort(vertex->Particles, vertex->Particles + vertex->nParticles,
  //  AnaTrack::CompareMomentum);

  // Make truth-reco association
  // Loop over true vertices matched with this global vertex.
  for (int i = 0; i < globalVertex.NTrueVertices; i++) {
    ND::TTrueVertex* matchedTrueVertex =
        (ND::TTrueVertex*)((*(globalVertex.TrueVertices))[i]);
    FillTrueVerticesMatchInfo(matchedTrueVertex, vertex, bunch);
  }
  // Sort true vertices by cleanliness
  std::sort(vertex->TrueVerticesMatch.begin(), vertex->TrueVerticesMatch.end(),
            CompareCleanliness(vertex));

  // Let's keep also vertex->TrueVertex by now (the user could just called
  // GetMainTrueVertex instead)
  vertex->TrueVertex =
      vertex->GetMainTrueVertex(false);  // argument false to suppress warnings
}

//*****************************************************************************
AnaTrack* oaAnalysisTreeConverter::FindTrack(int ID, AnaBunch* bunch,
                                             AnaSpill* spill) {
  //*****************************************************************************

  // Compare the UniqueID to find the corresponding AnaTrack
  // AnaTrack already exists because it is saved for first

  std::vector<AnaParticleB*>::iterator ittrack;

  // Look first in the same bunch of the vertex
  for (ittrack = bunch->Particles.begin(); ittrack != bunch->Particles.end();
       ittrack++) {
    if (ID == (*ittrack)->UniqueID) return static_cast<AnaTrack*>(*ittrack);
  }

  // If not found, look into OutOfBunch
  for (ittrack = spill->OutOfBunch->Particles.begin();
       ittrack != spill->OutOfBunch->Particles.end(); ittrack++) {
    if (ID == (*ittrack)->UniqueID) return static_cast<AnaTrack*>(*ittrack);
  }

// If not found, look into all bunches
// in prod 5 this could happens only when the vertex is OutOfBunch
#if !VERSION_HAS_TIME_FITS
  if (bunch->Bunch != -1) {
    if ((int)NPIDs > (int)NMAXPARTICLES)
      std::cout << "minor error: track to be associated to a global vertex not "
                   "found (UniqueID "
                << ID << "), likely because this event has too many tracks ("
                << NPIDs << ") and highland saves maximum " << NMAXPARTICLES
                << " tracks." << std::endl;
    else
      std::cout << "minor error in oaAnalysisConverter (ref 6056)" << std::endl;
    return NULL;
  }
#endif

  // Look into all bunches
  std::vector<AnaBunchC*>::iterator itbunch;
  for (itbunch = spill->Bunches.begin(); itbunch != spill->Bunches.end();
       itbunch++) {
    AnaBunchB* bunch0 = static_cast<AnaBunchB*>(*itbunch);
    for (ittrack = bunch0->Particles.begin();
         ittrack != bunch0->Particles.end(); ittrack++) {
      if (ID == (*ittrack)->UniqueID) {
        if (bunch->Bunch != -1) {  // often a global vertex out of bunch has
                                   // constituent tracks in the near bunch
          int bunchStart = _bunching.GetBunch((*ittrack)->PositionStart[3],
                                              RunID, IsMC, cosmic_mode);
          int bunchEnd = _bunching.GetBunch((*ittrack)->PositionEnd[3], RunID,
                                            IsMC, cosmic_mode);
          std::cout << "INFO: this global vertex (in bunch " << bunch->Bunch
                    << ") has an associated track stored in another bunch, "
                       "which starts in bunch "
                    << bunchStart << " and ends in bunch " << bunchEnd
                    << std::endl;
        }
        return static_cast<AnaTrack*>(*ittrack);
      }
    }
  }

  if ((int)NPIDs > (int)NMAXPARTICLES)
    std::cout << "minor error: track to be associated to a global vertex not "
                 "found (UniqueID "
              << ID << "), likely because this event has too many tracks ("
              << NPIDs << ") and highland saves maximum " << NMAXPARTICLES
              << " tracks." << std::endl;
  else
    std::cout << "minor error: track to be associated to a global vertex not "
                 "found in bunch "
              << bunch->Bunch << ": in prod6 it should be a flipped track with "
                                 "start position in a bunch not yet loaded!"
              << std::endl;

  return NULL;
}

//*****************************************************************************
void oaAnalysisTreeConverter::FillTrueVerticesMatchInfo(
    ND::TTrueVertex* matchedTrueVertex, AnaVertex* vertex, AnaBunch* bunch) {
  //*****************************************************************************

  // Compare the ID to find the corresponding AnaTrueVertex.
  // Set Cleanliness and Completeness that are truth-reco quantities.

  // Loop over all AnaTrueVertex
  std::vector<AnaTrueVertexB*>::iterator it;
  for (it = _spill->TrueVertices.begin(); it != _spill->TrueVertices.end();
       it++) {
    if (matchedTrueVertex->ID != (*it)->ID) continue;

    // Cleanliness and Completenss were wrong in TGlobalReconModule, fixed from
    // oaAnalysis v5r30
    // anyway it is better to re-evaluate them here where objects are "bunched"
    //      AnaRecTrueMatch* thisRecTrueMatch = new AnaRecTrueMatch;
    //      thisRecTrueMatch->Cleanliness = matchedTrueVertex->Pur;
    //      thisRecTrueMatch->Completeness = matchedTrueVertex->Eff;
    AnaRecTrueMatch* thisRecTrueMatch =
        FillCompletenessCleanliness(vertex, bunch, *it);

    AnaTrueVertex* tvertex = static_cast<AnaTrueVertex*>(*it);
    pair<AnaTrueVertexB*, AnaRecTrueMatch> myPair(tvertex, *thisRecTrueMatch);
    vertex->TrueVerticesMatch.push_back(myPair);
    tvertex->ReconVertices.push_back(vertex);
    return;  // Stop when association is found.
  }

  std::cout << "minor error: matchedTrueVertex not found, ID:"
            << matchedTrueVertex->ID << std::endl;
}

//*************************************************************
AnaRecTrueMatch* oaAnalysisTreeConverter::FillCompletenessCleanliness(
    AnaVertex* vertex, AnaBunch* bunch, AnaTrueVertexB* truevertex) {
  //*************************************************************

  // Cleanliness and Completenss were wrong in TGlobalReconModule, fixed from
  // oaAnalysis v5r30
  // anyway it is better to re-evaluate them here where objects are "bunched"
  // Let's evalutate completeness and cleanliness only with in-bunch tracks, so
  // it will be the same for flat trees

  // trueV constituent track: loop over all PIDs, and store those that come from
  // this true vertex
  std::vector<AnaTrackB*> trueVconst;
  std::set<AnaTrueObjectC*> trueVconst_true;
  AnaTrackB* tracks[NMAXPARTICLES];
  // Only tracks in FGD OR TPC are used by the global vertex algorithm
  int ntracks = anaUtils::GetAllTracksUsingFGDorTPC(*bunch, tracks);
  for (int i = 0; i < ntracks; i++) {
    AnaTrack* track = static_cast<AnaTrack*>(tracks[i]);

    if (!track->TrueObject) continue;  // track without true
    if (track->GetTrueParticle()->ParentID != 0)
      continue;  // true track not primary
    if (track->NNodes > 0)
      ;
    else
      continue;                        // track not used in vertexing algorithm
    if (track->Status != 1) continue;  // track not used in vertexing algorithm

    // double counting broken tracks
    if (track->GetTrueParticle()->TrueVertex == truevertex)
      trueVconst.push_back(track);
    // NOT double counting broken tracks
    if (track->GetTrueParticle()->TrueVertex == truevertex)
      trueVconst_true.insert(track->TrueObject);
  }

  int nTrueVconst = (int)trueVconst.size();
  int nTrueVconst_true = (int)trueVconst_true.size();
  int nRecoVconst = 0;  // count only those in this bunch, as for the trueV
  int nCommonConst = 0;
  int nRecoVconst_true = 0;  // count only those in this bunch, as for the trueV
  int nCommonConst_true = 0;

  // Loop over vertex constituent tracks
  std::set<AnaTrueObjectC*> recoVconst_true;
  std::vector<AnaTrackB*>::iterator itrecoVconst;
  for (int i = 0; i < vertex->nParticles; i++) {
    AnaTrack* itrecoVconst = static_cast<AnaTrack*>(vertex->Particles[i]);
    if (!itrecoVconst) continue;

    if (itrecoVconst->Bunch != bunch->Bunch) continue;

    nRecoVconst++;

    // Check if they match true vertex constituent tracks
    std::vector<AnaTrackB*>::iterator ittrueVconst;
    for (ittrueVconst = trueVconst.begin(); ittrueVconst != trueVconst.end();
         ittrueVconst++) {
      if (itrecoVconst->UniqueID == (*ittrueVconst)->UniqueID) {
        nCommonConst++;
        break;
      }
    }

    // Check if they match true vertex constituent tracks NOT BROKEN
    if (!itrecoVconst->TrueObject) {
      nRecoVconst_true++;
      continue;
    }

    double checkBroken = recoVconst_true.size();
    recoVconst_true.insert(itrecoVconst->TrueObject);
    if (checkBroken == recoVconst_true.size()) continue;

    std::set<AnaTrueObjectC*>::iterator ittrueVconst_true;
    for (ittrueVconst_true = trueVconst_true.begin();
         ittrueVconst_true != trueVconst_true.end(); ittrueVconst_true++) {
      if (itrecoVconst->TrueObject->ID == (*ittrueVconst_true)->ID) {
        nCommonConst_true++;
        break;
      }
    }
  }
  nRecoVconst_true += (int)recoVconst_true.size();

  AnaRecTrueMatch* thisRecTrueMatch = new AnaRecTrueMatch;
  if (nRecoVconst == 0) thisRecTrueMatch->Cleanliness = 0;
  //  else                thisRecTrueMatch->Cleanliness  =
  //  (double)nCommonConst/nRecoVconst; // double counting broken tracks
  else
    thisRecTrueMatch->Cleanliness =
        (double)nCommonConst_true /
        nRecoVconst_true;  // NOT double counting broken tracks
  if (nTrueVconst == 0) thisRecTrueMatch->Completeness = 0;
  //  else                thisRecTrueMatch->Completeness =
  //  (double)nCommonConst/nTrueVconst; // double counting broken tracks
  else
    thisRecTrueMatch->Completeness =
        (double)nCommonConst_true /
        nTrueVconst_true;  // NOT double counting broken tracks

  return thisRecTrueMatch;
}

//*****************************************************************************
void oaAnalysisTreeConverter::DeleteBadObjects(AnaSpill* spill) {
  //*****************************************************************************

  // Delete all the objects associated with a Pauli blocked vertices
  // (single gamma and 1pi Pauli-blocked vertices must be discarded from the
  // truth tree
  // but kept in the NRooTrackerVtx, see bugzilla 1011)
  // or with a coherent interaction on hydrogen, wrongly produced because of a
  // bug in neut,
  // both in prod 5 and 6, see bugzilla 1056:

  // Also Delete true vertices with all true tracks (and their associated recon
  // tracks)
  // fully contained in the specified subdetectors. Also deletes the true tracks
  // and the recon tracks in that case

  if (!_foundCohOnH && !_foundPauliBlocked &&
      _containedTrueParticles.size() == 0)
    return;

  if (!_discardPauliBlocked && _foundPauliBlocked)
    std::cout << "There is a Pauli blocked vertices but their deletion is not "
                 "enabled, why?!? (see bugzilla 1011)"
              << std::endl;
  if (!_discardCohOnH && _foundCohOnH)
    std::cout << "There is a coherent interaction on hydrogen but their "
                 "deletion is not enabled, why?!? (see bugzilla 1056)"
              << std::endl;
  if (!_discardPauliBlocked && !_discardCohOnH &&
      _containedTrueParticles.size() == 0)
    return;

  // loop over true vertices (without deleting them, by now)
  std::vector<AnaTrueVertexB*>::iterator truevertexB;
  for (truevertexB = spill->TrueVertices.begin();
       truevertexB != spill->TrueVertices.end();) {
    AnaTrueVertex* truevertex = static_cast<AnaTrueVertex*>(*truevertexB);

    // if not fully contained and not bad vertex, continue
    if (!IsFullyContained(*truevertex)) {
      if ((!truevertex->IsCohOnH && !truevertex->IsPauliBlocked) ||
          (truevertex->IsPauliBlocked && !_discardPauliBlocked) ||
          (truevertex->IsCohOnH && !_discardCohOnH)) {
        truevertexB++;
        continue;
      }
    }

    //    if (truevertex->nTrueParticles > 0) std::cout << "OK, let's remove ID
    //    " << truevertex->ID << " , " << truevertex->IsPauliBlocked << " , " <<
    //    truevertex->IsCohOnH << "\n";

    // loop over global vertices of this truevertex (and delete them at the end)
    std::vector<AnaVertexB*>::iterator vertexB;
    for (vertexB = truevertex->ReconVertices.begin();
         vertexB != truevertex->ReconVertices.end(); vertexB++) {
      // loop over ReconParticles of this global vertex (they might be
      // associated to a different true vertex);
      for (int i = 0; i < (*vertexB)->nParticles;
           i++) {  // (this is a **, cannot be done with the iterator)
        AnaTrack* track = dynamic_cast<AnaTrack*>((*vertexB)->Particles[i]);
        if (!track) continue;
        if (!track)
          std::cout << std::endl
                    << "ERROR: this track has to exist otherwise something is "
                       "wrong somewhere else!"
                    << " The process will now crash (see bug 1172)."
                    << std::endl
                    << std::endl;

        // delete the pointer between this track and this global vertex (which
        // will be deleted)
        int size_orig = (int)track->ReconVertices.size();
        (void)size_orig;
        std::vector<AnaVertexB*>::iterator vertexB2;
        for (vertexB2 = track->ReconVertices.begin();
             vertexB2 != track->ReconVertices.end(); vertexB2++) {
          if (*vertexB2 == *vertexB) {
            //            std::cout << "INFO: yes it happens! ref. 9885" <<
            //            std::endl; // yes it happens!
            vertexB2 = track->ReconVertices.erase(vertexB2);
            break;
          }
        }
        //        std::cout << size_orig << " ReconVertices vs " <<
        //        track->ReconVertices.size() << std::endl;
        // If a global vertex (associated to a bad true vertex) has a track
        // without truth association,
        // better not deleting that track, since it has a loose relation with
        // the true vertex:
        // we cannot assume the global vertex is correct in the association in
        // that case (see bug 1056).
        // (And also, even though it's unlikely, the TrueParticle might be
        // missing only because was not saved, for not having G4 points)
        if (!track->TrueObject) continue;

        // do not delete the tracks here to allow the next loop
        // truevertex->ReconParticles
        if (track->GetTrueParticle()->TrueVertex == (*truevertexB)) continue;

        // if this track is associated to another truevertex,
        // delete the pointer between this global vertex and that other true
        // vertex
        AnaTrueVertex* anothertrueV =
            static_cast<AnaTrueVertex*>(track->GetTrueParticle()->TrueVertex);
        size_orig = (int)anothertrueV->ReconVertices.size();
        (void)size_orig;
        for (vertexB2 = anothertrueV->ReconVertices.begin();
             vertexB2 != anothertrueV->ReconVertices.end(); vertexB2++) {
          if (*vertexB2 == *vertexB) {
            //            std::cout << "INFO: yes it happens! ref. 9886" <<
            //            std::endl; // yes it happens!
            vertexB2 = anothertrueV->ReconVertices.erase(vertexB2);
            break;
          }
        }
        //        std::cout << size_orig << " ReconVertices vs " <<
        //        anothertrueV->ReconVertices.size() << std::endl;
      }

      // now delete this global vertex
      DeleteRecoVertex(spill, *vertexB);
    }

    // loop over ReconParticles of this truevertex and delete them
    // (they might not have had a global vertex associated)
    std::vector<AnaParticleB*>::iterator part;
    for (part = truevertex->ReconParticles.begin();
         part != truevertex->ReconParticles.end(); part++) {
      DeleteRecoParticle(spill, *part);
    }

    // loop over TrueParticles of this true vertex and delete them
    // (this is a **, cannot be done with the iterator)
    for (int i = 0; i < truevertex->nTrueParticles; i++) {
      AnaTrueParticleB* truePartB = truevertex->TrueParticles[i];
      DeleteTrueParticle(spill, truePartB);
    }

    // delete the bunch of this true vertex if it is empty (not if OutOfBunch)
    if ((*truevertexB)->Bunch != -1) {
      std::vector<AnaBunchC*>::iterator bunchC;
      for (bunchC = spill->Bunches.begin(); bunchC != spill->Bunches.end();) {
        AnaBunch* bunch = static_cast<AnaBunch*>(*bunchC);

        if ((*bunchC)->Bunch != (*truevertexB)->Bunch) {
          bunchC++;
          continue;
        }

        // if emptied, delete this bunch
        if (bunch->IsEmpty()) {
          int size_orig = (int)spill->Bunches.size();
          (void)size_orig;
          delete *bunchC;
          bunchC = spill->Bunches.erase(bunchC);
          //          std::cout << size_orig << " bunches vs " <<
          //          spill->Bunches.size() << std::endl;
        }

        // break when bunch is found
        break;
      }  // end loop over bunches
    }

    // Delete this bad true vertex (maybe it would be better to put
    // a bad quality cut in baseAnalysis to show and remind what's going on)
    delete *truevertexB;
    truevertexB = spill->TrueVertices.erase(truevertexB);
    //    truevertexB++; // if not deleted, go to next iterator

  }  // endl loop over true vertices
}

//*****************************************************************************
void oaAnalysisTreeConverter::DeleteTrueParticle(
    AnaSpill* spill, AnaTrueParticleB* trackToDelete) {
  //*****************************************************************************

  // first set to null TPCsegment->trackToDelete
  // since they might belong to a globaltrack associated to another good true
  // track (very rare)

  //------ create outofbunch+bunches --------------
  std::vector<AnaBunchC*> outplusbunches;
  for (int b = 0; b < (int)spill->Bunches.size(); b++) {
    outplusbunches.push_back(spill->Bunches[b]);
  }
  // loop also in OutOfBunch, for last since it is unlikely
  outplusbunches.push_back(spill->OutOfBunch);

  //------ Loop over outofbunch+bunches --------------
  for (int b = 0; b < (int)outplusbunches.size(); b++) {
    AnaBunchB* bunch = static_cast<AnaBunchB*>(outplusbunches[b]);

    int size_orig = (int)bunch->Particles.size();
    (void)size_orig;

    // loop over Tracks
    std::vector<AnaParticleB*>::iterator it;
    for (it = bunch->Particles.begin(); it != bunch->Particles.end(); it++) {
      AnaTrack* track = dynamic_cast<AnaTrack*>(*it);
      if (!track) continue;

      // loop over TPCSegments
      for (int i = 0; i < track->nTPCSegments; i++) {
        if (track->TPCSegments[i]->TrueObject == trackToDelete &&
            track->TrueObject != trackToDelete) {
          std::cout << "INFO: found a TPCsegment associated to a trueTrack to "
                       "be deleted (see bug 1011 and 1056) and to a reconTrack "
                       "not to be deleted (which is associated to a different "
                       "trueTrack)! Keep the segment but set to NULL the link "
                       "to its trueTrack."
                    << std::endl;
          track->TPCSegments[i]->TrueObject = NULL;
        }
      }

      // loop over FGDSegments
      for (int i = 0; i < track->nFGDSegments; i++) {
        if (track->FGDSegments[i]->TrueObject == trackToDelete &&
            track->TrueObject != trackToDelete) {
          std::cout << "INFO: found a FGDsegment associated to a trueTrack to "
                       "be deleted (see bug 1011 and 1056) and to a reconTrack "
                       "not to be deleted (which is associated to a different "
                       "trueTrack)! Keep the segment but set to NULL the link "
                       "to its trueTrack."
                    << std::endl;
          track->FGDSegments[i]->TrueObject = NULL;
        }
      }

      // loop over SMRDSegments
      for (int i = 0; i < track->nSMRDSegments; i++) {
        if (track->SMRDSegments[i]->TrueObject == trackToDelete &&
            track->TrueObject != trackToDelete) {
          std::cout << "INFO: found a SMRDsegment associated to a trueTrack to "
                       "be deleted (see bug 1011 and 1056) and to a reconTrack "
                       "not to be deleted (which is associated to a different "
                       "trueTrack)! Keep the segment but set to NULL the link "
                       "to its trueTrack."
                    << std::endl;
          track->SMRDSegments[i]->TrueObject = NULL;
        }
      }

      // loop over ECALSegments
      for (int i = 0; i < track->nECALSegments; i++) {
        if (track->ECALSegments[i]->TrueObject == trackToDelete &&
            track->TrueObject != trackToDelete) {
          std::cout << "INFO: found a ECALsegment associated to a trueTrack to "
                       "be deleted (see bug 1011 and 1056) and to a reconTrack "
                       "not to be deleted (which is associated to a different "
                       "trueTrack)! Keep the segment but set to NULL the link "
                       "to its trueTrack."
                    << std::endl;
          track->ECALSegments[i]->TrueObject = NULL;
        }
      }

      // loop over P0DSegments
      for (int i = 0; i < track->nP0DSegments; i++) {
        if (track->P0DSegments[i]->TrueObject == trackToDelete &&
            track->TrueObject != trackToDelete) {
          std::cout << "INFO: found a P0Dsegment associated to a trueTrack to "
                       "be deleted (see bug 1011 and 1056) and to a reconTrack "
                       "not to be deleted (which is associated to a different "
                       "trueTrack)! Keep the segment but set to NULL the link "
                       "to its trueTrack."
                    << std::endl;
          track->P0DSegments[i]->TrueObject = NULL;
        }
      }

    }  // end of loop over Tracks

  }  // end of loop over bunches

  // now find the true track in the spill and delete it there
  int size_orig = (int)spill->TrueParticles.size();
  (void)size_orig;
  bool found = false;
  // loop over true tracks
  std::vector<AnaTrueParticleB*>::iterator part;
  for (part = spill->TrueParticles.begin();
       part != spill->TrueParticles.end();) {
    if (*part != trackToDelete) {
      part++;
      continue;
    }

    // perform deleting
    delete *part;
    part = spill->TrueParticles.erase(part);
    found = true;
    break;
  }

  //  std::cout << size_orig << " truetracks vs " << spill->TrueParticles.size()
  //  << std::endl;
  if (!found)
    std::cout << "error in oaAnalysisTreeConverter::DeleteTrueParticle: track "
                 "to delete not found!!!"
              << std::endl;
}

//*****************************************************************************
void oaAnalysisTreeConverter::DeleteRecoParticle(
    AnaSpill* spill, AnaParticleB* particleToDelete) {
  //*****************************************************************************

  // All global vertices should be also linked to the true vertices, and should
  // have been already deleted
  AnaTrack* track = dynamic_cast<AnaTrack*>(particleToDelete);
  if (!track) return;
  if ((int)track->ReconVertices.size() > 0) {
    std::cout << "ERROR in DeleteRecoParticle (ref. 9884): "
                 "particleToDelete->ReconVertices.size = "
              << track->ReconVertices.size() << std::endl;
  }

  //------ create outofbunch+bunches --------------
  std::vector<AnaBunchC*> outplusbunches;
  for (int b = 0; b < (int)spill->Bunches.size(); b++) {
    outplusbunches.push_back(spill->Bunches[b]);
  }
  // loop also in OutOfBunch, for last since it is unlikely
  outplusbunches.push_back(spill->OutOfBunch);

  //------ Loop over outofbunch+bunches --------------
  bool found = false;
  for (int b = 0; b < (int)outplusbunches.size(); b++) {
    AnaBunchB* bunch = static_cast<AnaBunchB*>(outplusbunches[b]);

    int size_orig = (int)bunch->Particles.size();
    (void)size_orig;

    // loop over Particles
    std::vector<AnaParticleB*>::iterator part;
    for (part = bunch->Particles.begin(); part != bunch->Particles.end();) {
      if (*part != particleToDelete) {
        part++;
        continue;
      }

      // perform deleting
      delete *part;
      part = bunch->Particles.erase(part);

      found = true;
      break;

    }  // end of loop over Particles

    //    if (found) std::cout << size_orig << " reco particles vs " <<
    //    bunch->Particles.size() << std::endl;

    if (found) break;

  }  // end of loop over outofbunch+bunches

  if (!found)
    std::cout << "error in oaAnalysisTreeConverter::DeleteRecoParticle: "
                 "particle to delete not found!!!"
              << std::endl;
}

//*****************************************************************************
void oaAnalysisTreeConverter::DeleteRecoVertex(AnaSpill* spill,
                                               AnaVertexB* vertexToDelete) {
  //*****************************************************************************

  //------ create outofbunch+bunches --------------
  std::vector<AnaBunchC*> outplusbunches;
  for (int b = 0; b < (int)spill->Bunches.size(); b++) {
    outplusbunches.push_back(spill->Bunches[b]);
  }
  // loop also in OutOfBunch, for last since it is unlikely
  outplusbunches.push_back(spill->OutOfBunch);

  //------ Loop over outofbunch+bunches --------------
  bool found = false;
  for (int b = 0; b < (int)outplusbunches.size(); b++) {
    AnaBunchB* bunch = static_cast<AnaBunchB*>(outplusbunches[b]);

    int size_orig = (int)bunch->Vertices.size();
    (void)size_orig;

    // loop over Vertices
    std::vector<AnaVertexB*>::iterator vertexB;
    for (vertexB = bunch->Vertices.begin(); vertexB != bunch->Vertices.end();) {
      if (*vertexB != vertexToDelete) {
        vertexB++;
        continue;
      }

      // perform deleting
      delete *vertexB;
      vertexB = bunch->Vertices.erase(vertexB);

      found = true;
      break;

    }  // end of loop over Vertices

    //    if (found) std::cout << size_orig << " reco vertices vs " <<
    //    bunch->Vertices.size() << std::endl;

    if (found) break;

  }  // end of loop over outofbunch+bunches

  if (!found)
    std::cout << "error in oaAnalysisTreeConverter::DeleteRecoVertex: vertex "
                 "to delete not found!!!"
              << std::endl;
}

//*****************************************************************************
bool oaAnalysisTreeConverter::HasNonContainedReconParticles(
    AnaTrueParticleB* trueParticleB) {
  //*****************************************************************************

  AnaTrueParticle* trueParticle = static_cast<AnaTrueParticle*>(trueParticleB);
  SubDetId::SubDetEnum det;

  // find in which det the true particle is fully contained
  for (Int_t idet = 0; idet < trueParticle->nDetCrossings; idet++) {
    if (SubDetId::GetDetectorUsed(trueParticle->DetCrossings[idet]->Detector,
                                  SubDetId::kDSECAL))
      det = SubDetId::kDSECAL;
    else if (SubDetId::GetDetectorUsed(
                 trueParticle->DetCrossings[idet]->Detector, SubDetId::kTECAL))
      det = SubDetId::kTECAL;
    else if (SubDetId::GetDetectorUsed(
                 trueParticle->DetCrossings[idet]->Detector, SubDetId::kPECAL))
      det = SubDetId::kPECAL;
    else if (SubDetId::GetDetectorUsed(
                 trueParticle->DetCrossings[idet]->Detector, SubDetId::kSMRD))
      det = SubDetId::kSMRD;
  }

  // loop over trueParticle->ReconParticles
  for (std::vector<AnaParticleB*>::iterator it =
           trueParticle->ReconParticles.begin();
       it != trueParticle->ReconParticles.end(); it++) {
    // if this recon particle is not fully contained in the same det, return
    // true
    if (!anaUtils::TrackUsesOnlyDet(**it, det)) return true;
  }

  // should we check also global vertices? see bug 1174
  // loop over global vertices of AnaTrueParticle::AnaTrueVertex
  AnaTrueVertex* trueVertex =
      static_cast<AnaTrueVertex*>(trueParticleB->TrueVertex);
  for (std::vector<AnaVertexB*>::iterator it =
           trueVertex->ReconVertices.begin();
       it != trueVertex->ReconVertices.end(); it++) {
    // if this global vertex is not in the same det, return true
    if (!anaUtils::InDetVolume(det, (*it)->Position)) return true;
  }

  return false;
}

//*****************************************************************************
bool oaAnalysisTreeConverter::IsFullyContained(
    const AnaTrueVertexB& trueVertex) {
  //*****************************************************************************

  // Check that all true particles (and the recon particles associated) in the
  // vertex are fully contained in a detector

  bool contained = true;
  // loop over true particles in the true trueVertex
  for (Int_t j1 = 0; j1 < trueVertex.nTrueParticles; j1++) {
    bool found = false;
    // and over fully contained true particles
    for (size_t j2 = 0; j2 < _containedTrueParticles.size(); j2++) {
      if (trueVertex.TrueParticles[j1] == _containedTrueParticles[j2] &&
          !HasNonContainedReconParticles(_containedTrueParticles[j2])) {
        found = true;
        break;
      }
    }
    //  if the trueVertex contains not fully contained particles, it is not
    //  fully contained
    if (!found) {
      contained = false;
      break;
    }
  }

  return contained;
}

