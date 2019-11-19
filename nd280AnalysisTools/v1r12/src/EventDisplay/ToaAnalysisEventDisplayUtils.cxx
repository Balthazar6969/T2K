#include "ToaAnalysisEventDisplayUtils.hxx"
#include "TTruthUtils.hxx"

#if !defined(__CINT__)
ClassImp(ToaAnalysisEventDisplayUtils)
#endif

size_t const kMaxPoints = 200;

//*************************************************
ToaAnalysisEventDisplayUtils::ToaAnalysisEventDisplayUtils() {
  //*************************************************

  fDrawSMRD = true;
  fProportionalHits = false;
  fDebug = false;

  for (int i = 0; i < 3; i++) tline[i] = NULL;
  for (int i = 0; i < 5; i++) tmarker[i] = NULL;
  for (int i = 0; i < 8; i++) text[i] = NULL;

  YZHisto = NULL;
  XZHisto = NULL;
  XYHisto = NULL;

  geomInitialized = false;
}

//**************************************************************
void ToaAnalysisEventDisplayUtils::runEvents(std::string infile_name,
                                             std::string geom_file) {
  //**************************************************************

  p_Vertices = new TClonesArray("ND::TGlobalReconModule::TGlobalVertex", 20);
  p_PIDs = new TClonesArray("ND::TGlobalReconModule::TGlobalPID", 20);
  p_FgdTimeBins = new TClonesArray("ND::TGlobalReconModule::TFgdTimeBin", 20);
  p_DelayedClusters =
      new TClonesArray("ND::TGlobalReconModule::TFgdCluster", 10);
  p_P0DUnused = new TClonesArray("ND::TGlobalReconModule::TGlobalHit", 100);

  p_FGD1Active = new ND::TGeometrySummaryModule::TDetectorBoundingBox;
  p_FGD2Active = new ND::TGeometrySummaryModule::TDetectorBoundingBox;
  p_P0DActive = new ND::TGeometrySummaryModule::TDetectorBoundingBox;
  p_DSECALExtent = new ND::TGeometrySummaryModule::TDetectorBoundingBox;
  p_TECAL1Extent = new ND::TGeometrySummaryModule::TDetectorBoundingBox;
  p_TECAL2Extent = new ND::TGeometrySummaryModule::TDetectorBoundingBox;
  p_TECAL3Extent = new ND::TGeometrySummaryModule::TDetectorBoundingBox;
  p_TECAL4Extent = new ND::TGeometrySummaryModule::TDetectorBoundingBox;
  p_TECAL5Extent = new ND::TGeometrySummaryModule::TDetectorBoundingBox;
  p_TECAL6Extent = new ND::TGeometrySummaryModule::TDetectorBoundingBox;
  p_PECAL1Extent = new ND::TGeometrySummaryModule::TDetectorBoundingBox;
  p_PECAL2Extent = new ND::TGeometrySummaryModule::TDetectorBoundingBox;
  p_PECAL3Extent = new ND::TGeometrySummaryModule::TDetectorBoundingBox;
  p_PECAL4Extent = new ND::TGeometrySummaryModule::TDetectorBoundingBox;
  p_PECAL5Extent = new ND::TGeometrySummaryModule::TDetectorBoundingBox;
  p_PECAL6Extent = new ND::TGeometrySummaryModule::TDetectorBoundingBox;
  p_SMRD1Extent = new ND::TGeometrySummaryModule::TDetectorBoundingBox;
  p_SMRD2Extent = new ND::TGeometrySummaryModule::TDetectorBoundingBox;
  p_SMRD3Extent = new ND::TGeometrySummaryModule::TDetectorBoundingBox;
  p_SMRD4Extent = new ND::TGeometrySummaryModule::TDetectorBoundingBox;
  p_TPC1Extent = new ND::TGeometrySummaryModule::TDetectorBoundingBox;
  p_TPC2Extent = new ND::TGeometrySummaryModule::TDetectorBoundingBox;
  p_TPC3Extent = new ND::TGeometrySummaryModule::TDetectorBoundingBox;

  p_MM = new ND::TGeometrySummaryModule::TTPCMicroMegasVolumes;

  geo = new TGeoManager();

  TString geometryInputFile(geom_file);

  ifstream inp;
  inp.open(geom_file.c_str(), ifstream::in);

  if (inp.fail()) {
    using_geometry = false;
  } else {
    using_geometry = true;
  }

  geo->Import(geometryInputFile);

  ToaAnalysisEventDisplayUtils::Initialized();
  ToaAnalysisEventDisplayUtils::GeomInit();

  TString infileName(infile_name);

  // Create Tree objects
  TChain mcTruthVertices("TruthDir/Vertices");
  TChain mcTruthTrajectories("TruthDir/Trajectories");
  TChain reconGlobal("ReconDir/Global");
  TChain geometrySummary("HeaderDir/GeometrySummary");

  // _____________________________________________________________________________________
  // Load data files

  ifstream inputFile(infileName.Data(), ios::in);

  if (!inputFile) {
    cerr << "Cannot open input file '" << infileName.Data() << "'. Exit!"
         << endl;
    return;
  }

  // Loop over the input files
  std::string inputString = infile_name;
  cout << "Adding file: " << inputString << endl;
  if (inputString.find("mdp") != std::string::npos)
    p_isMC = true;
  else
    p_isMC = false;

  if (p_isMC) {
    mcTruthVertices.Add(inputString.c_str());
    mcTruthTrajectories.Add(inputString.c_str());
  }
  reconGlobal.Add(inputString.c_str());
  geometrySummary.Add(inputString.c_str());
  // _____________________________________________________________________________________
  // Prepare to read trees.

  // For MC fill true info (need to decide if variables are defined in standard
  // or general)
  if (p_isMC) {
    cout << " set branches adresses " << endl;
    for (int ii = 0; ii < p_NVTXTYPES; ii++) {
      p_TruthVertices[ii] =
          new TClonesArray("ND::TTruthVerticesModule::TTruthVertex", 50);
      mcTruthVertices.SetBranchAddress(p_vtxnames[ii].c_str(),
                                       &(p_TruthVertices[ii]));
      mcTruthVertices.SetBranchAddress(("N" + p_vtxnames[ii]).c_str(),
                                       &(p_NTruthVertices[ii]));
    }

    for (int ii = 0; ii < p_NTRAJTYPES; ii++) {
      p_TruthTrajs[ii] = new TClonesArray(
          "ND::TTruthTrajectoriesModule::TTruthTrajectory", 200);
      mcTruthTrajectories.SetBranchAddress(p_trajnames[ii].c_str(),
                                           &(p_TruthTrajs[ii]));
      mcTruthTrajectories.SetBranchAddress(p_ntrajnames[ii].c_str(),
                                           &(p_NTruthTrajs[ii]));
    }
  }

  // Set branches from global reconstructions:

  reconGlobal.SetBranchAddress("NVertices", &p_NVertices);
  reconGlobal.SetBranchAddress("Vertices", &p_Vertices);
  reconGlobal.SetBranchAddress("NPIDs", &p_NPIDs);
  reconGlobal.SetBranchAddress("PIDs", &p_PIDs);
  reconGlobal.SetBranchAddress("NTPC", &p_NTPC);
  reconGlobal.SetBranchAddress("NTPCIso", &p_NTPCIso);
  reconGlobal.SetBranchAddress("NECALIso", &p_NECALIso);
  reconGlobal.SetBranchAddress("NP0DIso", &p_NP0DIso);
  reconGlobal.SetBranchAddress("NSMRDIso", &p_NSMRDIso);
  reconGlobal.SetBranchAddress("NFGDIso", &p_NFGDIso);
  reconGlobal.SetBranchAddress("NP0DUnused", &p_NP0DUnused);
  reconGlobal.SetBranchAddress("P0DUnused", &p_P0DUnused);
  //  reconGlobal.SetBranchAddress("P0DOutermostHits",
  //  &p_P0DOutermostHits,&test);
  reconGlobal.SetBranchAddress("NFGD1Unused", &p_NFGD1Unused);
  reconGlobal.SetBranchAddress("NFGD2Unused", &p_NFGD2Unused);
  reconGlobal.SetBranchAddress("NFgdTimeBins", &p_NFgdTimeBins);
  reconGlobal.SetBranchAddress("FgdTimeBins", &p_FgdTimeBins);
  reconGlobal.SetBranchAddress("NDelayedClusters", &p_NDelayedClusters);
  reconGlobal.SetBranchAddress("DelayedClusters", &p_DelayedClusters);
  //  reconGlobal.SetBranchAddress("EarliestTrackTime", &p_EarliestTrackTime);
  reconGlobal.SetBranchAddress("EventID", &p_EventID);
  reconGlobal.SetBranchAddress("RunID", &p_RunID);

  geometrySummary.SetBranchAddress("FGD1Active", &p_FGD1Active);
  geometrySummary.SetBranchAddress("FGD2Active", &p_FGD2Active);
  geometrySummary.SetBranchAddress("TPC1Extent", &p_TPC1Extent);
  geometrySummary.SetBranchAddress("TPC2Extent", &p_TPC2Extent);
  geometrySummary.SetBranchAddress("TPC3Extent", &p_TPC3Extent);
  geometrySummary.SetBranchAddress("DSECALExtent", &p_DSECALExtent);
  geometrySummary.SetBranchAddress("P0DActive", &p_P0DActive);
  geometrySummary.SetBranchAddress("TPCMMvolumes", &p_MM);
  geometrySummary.SetBranchAddress("TECAL1Extent", &p_TECAL1Extent);
  geometrySummary.SetBranchAddress("TECAL2Extent", &p_TECAL2Extent);
  geometrySummary.SetBranchAddress("TECAL3Extent", &p_TECAL3Extent);
  geometrySummary.SetBranchAddress("TECAL4Extent", &p_TECAL4Extent);
  geometrySummary.SetBranchAddress("TECAL5Extent", &p_TECAL5Extent);
  geometrySummary.SetBranchAddress("TECAL6Extent", &p_TECAL6Extent);
  geometrySummary.SetBranchAddress("PECAL1Extent", &p_PECAL1Extent);
  geometrySummary.SetBranchAddress("PECAL2Extent", &p_PECAL2Extent);
  geometrySummary.SetBranchAddress("PECAL3Extent", &p_PECAL3Extent);
  geometrySummary.SetBranchAddress("PECAL4Extent", &p_PECAL4Extent);
  geometrySummary.SetBranchAddress("PECAL5Extent", &p_PECAL5Extent);
  geometrySummary.SetBranchAddress("PECAL6Extent", &p_PECAL6Extent);
  geometrySummary.SetBranchAddress("SMRD1Extent", &p_SMRD1Extent);
  geometrySummary.SetBranchAddress("SMRD2Extent", &p_SMRD2Extent);
  geometrySummary.SetBranchAddress("SMRD3Extent", &p_SMRD3Extent);
  geometrySummary.SetBranchAddress("SMRD4Extent", &p_SMRD4Extent);

  // __________________________________________________________________________
  // Loop over all events up to evHigh.
  fNumberEntries = reconGlobal.GetEntries();
  cout << "Number entries: " << fNumberEntries << endl;

  mf->SetMaxEntry(fNumberEntries - 1);

  //-------------------------------------------------------------
  // Start to loop over events (spill)
  //-------------------------------------------------------------

  mf->SetCurrentEntry(0);

  while (true) {
    fCurrentEntry = mf->GetCurrentEntry();
    fProportionalHits = mf->GetProportionalHits();
    fDebug = mf->GetDebug();

    if (mf->GetRequestedEvent() > 0)
      FindEvent(reconGlobal, mf->GetRequestedEvent());

    cout << "--------> begin event: Entry = " << fCurrentEntry << endl;

    if (fDrawSMRD != mf->GetDrawSMRD()) {
      fDrawSMRD = mf->GetDrawSMRD();
      InitExplanation();
    }

    geometrySummary.GetEvent(fCurrentEntry);

    DrawDetector();

    DrawExplanation();

    // Get event entry:
    //-----------------
    if (p_isMC) {
      mcTruthVertices.GetEntry(fCurrentEntry);
      mcTruthTrajectories.GetEntry(fCurrentEntry);
    }

    reconGlobal.GetEntry(fCurrentEntry);

    if (fCurrentEntry % 100 == 0)
      cout << "Events Processed " << fCurrentEntry << endl;

    DrawEventInfo(p_RunID, p_EventID);

    //----------- draw MC-true primary vertices ------------------
    if (p_isMC) DrawMCVertices(fTrueVertexColor);

    if (p_isMC) DrawMCTrajectories(fTruthColor);

    //----------------------------------------------------------------------
    // Loop over Vertices in event
    //---------------------------------------------------------------------

    std::cout << "# vertices: " << p_NVertices << std::endl;

    for (int j = 0; j < p_NVertices; j++) {
      ND::TGlobalReconModule::TGlobalVertex *globalVertex =
          (ND::TGlobalReconModule::TGlobalVertex *)(*p_Vertices)[j];

      if (!globalVertex) continue;

      DrawVertex(*globalVertex, 1);
    }

    //----------------------------------------------------------------------
    // Loop over PIDs in event
    // Done to store tracks counter per bunch and sort momentum per bunch
    //---------------------------------------------------------------------

    std::cout << "# global tracks: " << p_NPIDs << std::endl;

    for (int j = 0; j < p_NPIDs; j++) {
      // This loop over the pids, is used to set tracks counters,
      // and to sort tracks according to their momentum,
      // all for a given bunch.

      ND::TGlobalReconModule::TGlobalPID *globalTrack =
          (ND::TGlobalReconModule::TGlobalPID *)(*p_PIDs)[j];

      if (!globalTrack)
        continue;  // if no global track is found, go to the next PID

      // --- Get the bunch number -----
      // This is based on the begining of the global track found
      // First check if track is forward or backward.

      double tTrack = 0;
      // Forward going track
      TLorentzVector frontPos = globalTrack->FrontPosition;
      TLorentzVector backPos = globalTrack->BackPosition;
      double frontzi = frontPos.Z();
      double backzi = backPos.Z();

      // Forwards going track
      if ((globalTrack->isForward && (frontzi < backzi)) ||
          (!globalTrack->isForward && (frontzi > backzi))) {
        tTrack = frontPos.T();
      }
      // Backwards going track
      else {
        tTrack = backPos.T();
      }

      // Draw the global track
      if (fDebug)
        DrawTrackDebug(*globalTrack);
      else
        DrawTrack(*globalTrack);

      // Draw the most upstream and downstream hits in every track
      DrawHits(globalTrack->HitsSaved, globalTrack->NHitsSaved,
               fObjectHitsColor);

      for (int i = 0; i < p_NFgdTimeBins; i++) {
        ND::TGlobalReconModule::TFgdTimeBin *tbin =
            (ND::TGlobalReconModule::TFgdTimeBin *)(*p_FgdTimeBins)[i];
        // Draw unused hits in FGD
        DrawHits(tbin->FGD1Unused, tbin->NFGD1Unused, fUnusedHitsColor);
        DrawHits(tbin->FGD2Unused, tbin->NFGD2Unused, fUnusedHitsColor);
        // Draw outermost hits in FGD
        DrawOutermostHits(tbin->FGD1OutermostHits, fOutermostHitsColor);
        DrawOutermostHits(tbin->FGD2OutermostHits, fOutermostHitsColor);
      }

      // Draw unused hits in P0D
      DrawHits(p_P0DUnused, p_NP0DUnused, fUnusedHitsColor);

      //      DrawOutermostHits(p_P0DOutermostHits, fOutermostHitsColor);

      std::cout << " - " << j << ": " << globalTrack->Detectors << std::endl;

    }  // End loop over PIDs in event

    //_________________________________________________________________
    //__________________________________________________________________

    // Loop over bunches in event and fill bunch variables:
    //
    // Now: Bunch 0-->time exactly zero;1-6 real bunches but no sigma,
    // 7 huge times, 8 everything else(e.g. "very" negative times for MC)

    //_________________________________________________________________
    //__________________________________________________________________

    //    if (!p_silent) cout << "Enter Bunch loop " << endl;

    YZ->Update();
    XZ->Update();
    XY->Update();

    theApp->Run(kTRUE);

    // ------------------------------------------
    for (size_t i = 0; i < fBoxList.size(); i++) {
      delete fBoxList[i];
    }
    fBoxList.clear();

    for (size_t i = 0; i < fGraphList.size(); i++) {
      delete fGraphList[i];
    }
    fGraphList.clear();

    DrawDetector();
    DrawExplanation();
  }

  cout << "--------Done--------" << endl;
}

//*****************************************************************************
void ToaAnalysisEventDisplayUtils::GeomInit() {
  //*****************************************************************************

  YZ = new TCanvas("YZ", "  YZ  ", 100, 0, 600, 400);
  XZ = new TCanvas("XZ", "  XZ  ", 720, 0, 600, 400);
  XY = new TCanvas("XY", "  XY  ", 100, 420, 400, 400);
}

//**************************************************************
void ToaAnalysisEventDisplayUtils::Initialized() {
  //**************************************************************

  int argc2 = 0;
  char **argv2 = NULL;

  theApp = new TApplication("App", &argc2, argv2);

  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);

  mf = new MyMainFrame(gClient->GetRoot(), 100, 200);

  mf->SetWMPosition(10, 10);

  fOutermostHitsColor = 3;
  fUnusedHitsColor = 2;
  fOtherColor = 6;
  fObjectHitsColor = 4;
  fTrackColor = 4;
  fTruthColor = 41;
  fTrueVertexColor = 41;
  fRecVertexColor = 1;

  fHitMarker = kFullSquare;
  fTrueVertexMarker = kOpenCircle;
  fRecVertexMarker = kOpenCircle;

  EvtLabel = NULL;

  InitExplanation();
}

//**************************************************************
void ToaAnalysisEventDisplayUtils::DrawDetector() {
  //**************************************************************

  if (YZHisto) delete YZHisto;
  if (XZHisto) delete XZHisto;
  if (XYHisto) delete XYHisto;

  if (fDrawSMRD) {
    YZHisto = new TH2F("YZHisto", " YZ Projection ", 100, -4000., 4000., 100,
                       -3200., 3200.);
    XZHisto = new TH2F("XZHisto", " XZ Projection ", 100, -4000., 4000., 100,
                       -3000., 3000.);
    XYHisto = new TH2F("XYHisto", " XY Projection ", 100, -3000., 3000., 100,
                       -3200., 3200.);
  } else {
    YZHisto = new TH2F("YZHisto", " YZ Projection ", 100, -3500., 3500., 100,
                       -1800., 1800.);
    XZHisto = new TH2F("XZHisto", " XZ Projection ", 100, -3500., 3500., 100,
                       -1500., 1500.);
    XYHisto = new TH2F("XYHisto", " XY Projection ", 100, -1200., 1200., 100,
                       -1200., 1200.);
  }

  // -------------- Build the TPC MM ------------------
  if (using_geometry) {
    if (MMvector.size() == 0) {
      int saved = geo->GetNodeId();

      for (std::map<int, std::pair<int, const TGeoNode *> >::iterator ib =
               p_MM[0].MMvolumes.begin();
           ib != p_MM[0].MMvolumes.end(); ib++) {
        const TGeoNode *mm = (ib->second).second;
        const TGeoVolume *mvol = mm->GetVolume();
        TGeoShape *shape = mvol->GetShape();
        TGeoBBox *box = dynamic_cast<TGeoBBox *>(shape);
        double x = box->GetDX();
        double y = box->GetDY();
        double z = box->GetDZ();
        Double_t local[3], master[3];

        local[0] = local[1] = local[2] = 0.0;

        geo->CdNode((ib->second).first);

        geo->LocalToMaster(local, master);

        TBox *nn;
        if (master[0] < 0) {
          nn = new TBox(master[2] - x, master[1] - y, master[2] + x,
                        master[1] + y);

          MMvector.push_back(nn);
          nn->SetFillStyle(0);
          nn->SetLineColor(1);
        }

        nn = new TBox(master[2] - x, master[0] - z, master[2] + x,
                      master[0] + z);

        MMvectorXZ.push_back(nn);
        nn->SetFillStyle(0);
        nn->SetLineColor(1);

        nn = new TBox(master[0] - z, master[1] - y, master[0] + z,
                      master[1] + y);

        MMvectorXY.push_back(nn);
        nn->SetFillStyle(0);
        nn->SetLineColor(1);
      }

      geo->CdNode(saved);
    }
  }

  else {
    const TVector3 tpc1min = p_TPC1Extent->Minimum;
    const TVector3 tpc1max = p_TPC1Extent->Maximum;
    const TVector3 tpc2min = p_TPC2Extent->Minimum;
    const TVector3 tpc2max = p_TPC2Extent->Maximum;
    const TVector3 tpc3min = p_TPC3Extent->Minimum;
    const TVector3 tpc3max = p_TPC3Extent->Maximum;

    boxtpc1_XZ = new TBox(tpc1min.Z(), tpc1min.X(), tpc1max.Z(), tpc1max.X());
    boxtpc2_XZ = new TBox(tpc2min.Z(), tpc2min.X(), tpc2max.Z(), tpc2max.X());
    boxtpc3_XZ = new TBox(tpc3min.Z(), tpc3min.X(), tpc3max.Z(), tpc3max.X());
    boxtpc1_YZ = new TBox(tpc1min.Z(), tpc1min.Y(), tpc1max.Z(), tpc1max.Y());
    boxtpc2_YZ = new TBox(tpc2min.Z(), tpc2min.Y(), tpc2max.Z(), tpc2max.Y());
    boxtpc3_YZ = new TBox(tpc3min.Z(), tpc3min.Y(), tpc3max.Z(), tpc3max.Y());
    boxtpc_XY = new TBox(tpc1min.Y(), tpc1min.X(), tpc1max.Y(), tpc1max.X());

    boxtpc1_XZ->SetFillStyle(0);
    boxtpc2_XZ->SetFillStyle(0);
    boxtpc3_XZ->SetFillStyle(0);
    boxtpc1_YZ->SetLineColor(kMagenta);
    boxtpc2_YZ->SetLineColor(kMagenta);
    boxtpc3_YZ->SetLineColor(kMagenta);
    boxtpc1_XZ->SetLineColor(kMagenta);
    boxtpc2_XZ->SetLineColor(kMagenta);
    boxtpc3_XZ->SetLineColor(kMagenta);
    boxtpc1_YZ->SetFillStyle(0);
    boxtpc2_YZ->SetFillStyle(0);
    boxtpc3_YZ->SetFillStyle(0);
    boxtpc_XY->SetLineColor(kMagenta);
    boxtpc_XY->SetFillStyle(0);
  }
  // -------------- Build FGDs ------------------

  const TVector3 f1min = p_FGD1Active->Minimum;
  const TVector3 f1max = p_FGD1Active->Maximum;
  const TVector3 f2min = p_FGD2Active->Minimum;
  const TVector3 f2max = p_FGD2Active->Maximum;

  boxfgd1_XZ = new TBox(f1min.Z(), f1min.X(), f1max.Z(), f1max.X());
  boxfgd2_XZ = new TBox(f2min.Z(), f2min.X(), f2max.Z(), f2max.X());
  boxfgd1_YZ = new TBox(f1min.Z(), f1min.Y(), f1max.Z(), f1max.Y());
  boxfgd2_YZ = new TBox(f2min.Z(), f2min.Y(), f2max.Z(), f2max.Y());

  boxfgd1_XZ->SetFillStyle(0);
  boxfgd2_XZ->SetFillStyle(0);
  boxfgd1_XZ->SetLineColor(kTeal + 2);
  boxfgd2_XZ->SetLineColor(kTeal + 2);
  boxfgd1_YZ->SetFillStyle(0);
  boxfgd2_YZ->SetFillStyle(0);
  boxfgd1_YZ->SetLineColor(kTeal + 2);
  boxfgd2_YZ->SetLineColor(kTeal + 2);

  // -------------- Build P0D ------------------

  const TVector3 &pmin = p_P0DActive->Minimum;
  const TVector3 &pmax = p_P0DActive->Maximum;

  boxP0D_XZ = new TBox(pmin.Z(), pmin.X(), pmax.Z(), pmax.X());
  boxP0D_YZ = new TBox(pmin.Z(), pmin.Y(), pmax.Z(), pmax.Y());

  boxP0D_XZ->SetFillStyle(0);
  boxP0D_XZ->SetLineColor(kBlue);
  boxP0D_YZ->SetFillStyle(0);
  boxP0D_YZ->SetLineColor(kBlue);

  // -------------- Build TECals ------------------

  std::vector<TVector3> tmin;
  std::vector<TVector3> tmax;

  tmin.push_back(p_TECAL1Extent->Minimum);
  tmax.push_back(p_TECAL1Extent->Maximum);
  tmin.push_back(p_TECAL2Extent->Minimum);
  tmax.push_back(p_TECAL2Extent->Maximum);
  tmin.push_back(p_TECAL3Extent->Minimum);
  tmax.push_back(p_TECAL3Extent->Maximum);
  tmin.push_back(p_TECAL4Extent->Minimum);
  tmax.push_back(p_TECAL4Extent->Maximum);
  tmin.push_back(p_TECAL5Extent->Minimum);
  tmax.push_back(p_TECAL5Extent->Maximum);
  tmin.push_back(p_TECAL6Extent->Minimum);
  tmax.push_back(p_TECAL6Extent->Maximum);

  std::vector<TVector3>::iterator tecalmax = tmax.begin();

  for (std::vector<TVector3>::iterator tecal = tmin.begin();
       tecal != tmin.end(); ++tecal) {
    TBox boxXZ(tecal->Z(), tecal->X(), tecalmax->Z(), tecalmax->X());
    TBox boxYZ(tecal->Z(), tecal->Y(), tecalmax->Z(), tecalmax->Y());

    if (fabs(tecalmax->X() - tecal->X()) < 1400) {
      boxTECAL_XZ.push_back(boxXZ);
    }
    if (fabs(tecalmax->Y() - tecal->Y()) < 1400) {
      boxTECAL_YZ.push_back(boxYZ);
    }

    tecalmax++;
  }

  for (std::vector<TBox>::iterator tecalbox = boxTECAL_XZ.begin();
       tecalbox != boxTECAL_XZ.end(); ++tecalbox) {
    tecalbox->SetFillStyle(0);
    tecalbox->SetLineColor(kRed);
    tecalbox->SetFillStyle(0);
    tecalbox->SetLineColor(kRed);
  }

  for (std::vector<TBox>::iterator tecalbox = boxTECAL_YZ.begin();
       tecalbox != boxTECAL_YZ.end(); ++tecalbox) {
    tecalbox->SetFillStyle(0);
    tecalbox->SetLineColor(kRed);
    tecalbox->SetFillStyle(0);
    tecalbox->SetLineColor(kRed);
  }

  // -------------- Build PECals ------------------

  std::vector<TVector3> pecalmin;
  std::vector<TVector3> pecalmax;

  pecalmin.push_back(p_PECAL1Extent->Minimum);
  pecalmax.push_back(p_PECAL1Extent->Maximum);
  pecalmin.push_back(p_PECAL2Extent->Minimum);
  pecalmax.push_back(p_PECAL2Extent->Maximum);
  pecalmin.push_back(p_PECAL3Extent->Minimum);
  pecalmax.push_back(p_PECAL3Extent->Maximum);
  pecalmin.push_back(p_PECAL4Extent->Minimum);
  pecalmax.push_back(p_PECAL4Extent->Maximum);
  pecalmin.push_back(p_PECAL5Extent->Minimum);
  pecalmax.push_back(p_PECAL5Extent->Maximum);
  pecalmin.push_back(p_PECAL6Extent->Minimum);
  pecalmax.push_back(p_PECAL6Extent->Maximum);

  std::vector<TVector3>::iterator pecalmaxit = pecalmax.begin();

  for (std::vector<TVector3>::iterator pecal = pecalmin.begin();
       pecal != pecalmin.end(); ++pecal) {
    TBox boxXZ(pecal->Z(), pecal->X(), pecalmaxit->Z(), pecalmaxit->X());
    TBox boxYZ(pecal->Z(), pecal->Y(), pecalmaxit->Z(), pecalmaxit->Y());
    if (fabs(pecalmaxit->X() - pecal->X()) < 300) {
      boxPECAL_XZ.push_back(boxXZ);
    }
    if (fabs(pecalmaxit->Y() - pecal->Y()) < 300) {
      boxPECAL_YZ.push_back(boxYZ);
    }
    pecalmaxit++;
  }

  for (std::vector<TBox>::iterator pecalbox = boxPECAL_XZ.begin();
       pecalbox != boxPECAL_XZ.end(); ++pecalbox) {
    pecalbox->SetFillStyle(0);
    pecalbox->SetLineColor(kGreen + 2);
    pecalbox->SetFillStyle(0);
    pecalbox->SetLineColor(kGreen + 2);
  }

  for (std::vector<TBox>::iterator pecalbox = boxPECAL_YZ.begin();
       pecalbox != boxPECAL_YZ.end(); ++pecalbox) {
    pecalbox->SetFillStyle(0);
    pecalbox->SetLineColor(kGreen + 2);
    pecalbox->SetFillStyle(0);
    pecalbox->SetLineColor(kGreen + 2);
  }

  // -------------- Build Ds ECAL ------------------
  const TVector3 &smin = p_DSECALExtent->Minimum;
  const TVector3 &smax = p_DSECALExtent->Maximum;

  boxDsECAL_XZ = new TBox(smin.Z(), smin.X(), smax.Z(), smax.X());
  boxDsECAL_YZ = new TBox(smin.Z(), smin.Y(), smax.Z(), smax.Y());

  boxDsECAL_XZ->SetFillStyle(0);
  boxDsECAL_XZ->SetLineColor(kAzure);
  boxDsECAL_YZ->SetFillStyle(0);
  boxDsECAL_YZ->SetLineColor(kAzure);

  // -------------- Build SMRD ------------------

  if (fDrawSMRD) {
    std::vector<TVector3> smrdmin;
    std::vector<TVector3> smrdmax;
    smrdmin.push_back(p_SMRD1Extent->Minimum);
    smrdmax.push_back(p_SMRD1Extent->Maximum);
    smrdmin.push_back(p_SMRD2Extent->Minimum);
    smrdmax.push_back(p_SMRD2Extent->Maximum);
    smrdmin.push_back(p_SMRD3Extent->Minimum);
    smrdmax.push_back(p_SMRD3Extent->Maximum);
    smrdmin.push_back(p_SMRD4Extent->Minimum);
    smrdmax.push_back(p_SMRD4Extent->Maximum);

    for (int s = 0; s < 4; s++) {
      boxSMRD_XZ[s] = new TBox(smrdmin[s].Z(), smrdmin[s].X(), smrdmax[s].Z(),
                               smrdmax[s].X());
      boxSMRD_YZ[s] = new TBox(smrdmin[s].Z(), smrdmin[s].Y(), smrdmax[s].Z(),
                               smrdmax[s].Y());
      boxSMRD_XY[s] = new TBox(smrdmin[s].X(), smrdmin[s].Y(), smrdmax[s].X(),
                               smrdmax[s].Y());

      boxSMRD_XZ[s]->SetFillStyle(0);
      boxSMRD_XZ[s]->SetLineColor(1);
      boxSMRD_YZ[s]->SetFillStyle(0);
      boxSMRD_YZ[s]->SetLineColor(1);
      boxSMRD_XY[s]->SetFillStyle(0);
      boxSMRD_XY[s]->SetLineColor(1);
    }
  }

  //---------------------------------------

  YZHisto->GetXaxis()->UnZoom();
  YZHisto->GetYaxis()->UnZoom();
  XYHisto->GetYaxis()->UnZoom();
  XYHisto->GetXaxis()->UnZoom();
  XZHisto->GetXaxis()->UnZoom();
  XZHisto->GetYaxis()->UnZoom();

  //------ Draw the YZ view --------
  YZ->cd();
  YZHisto->Draw();

  if (using_geometry) {
    for (size_t i = 0; i < MMvector.size(); i++) MMvector[i]->Draw("same");
  }

  else {
    boxtpc1_YZ->Draw("same");
    boxtpc2_YZ->Draw("same");
    boxtpc3_YZ->Draw("same");
  }

  boxfgd1_YZ->Draw("same");
  boxfgd2_YZ->Draw("same");

  boxP0D_YZ->Draw("same");

  boxDsECAL_YZ->Draw("same");

  for (std::vector<TBox>::iterator ecals = boxTECAL_YZ.begin();
       ecals != boxTECAL_YZ.end(); ++ecals) {
    ecals->Draw("same");
  }

  for (std::vector<TBox>::iterator ecals = boxPECAL_YZ.begin();
       ecals != boxPECAL_YZ.end(); ++ecals) {
    ecals->Draw("same");
  }

  if (fDrawSMRD)
    for (int s = 2; s < 4; s++) boxSMRD_YZ[s]->Draw("same");

  YZ->Update();

  //------ Draw the XZ view --------
  XZ->cd();
  XZHisto->Draw();

  if (using_geometry) {
    for (size_t i = 0; i < MMvectorXZ.size(); i++) MMvectorXZ[i]->Draw("same");
  }

  else {
    boxtpc1_XZ->Draw("same");
    boxtpc2_XZ->Draw("same");
    boxtpc3_XZ->Draw("same");
  }

  boxfgd1_XZ->Draw("same");
  boxfgd2_XZ->Draw("same");

  boxP0D_XZ->Draw("same");

  boxDsECAL_XZ->Draw("same");

  for (std::vector<TBox>::iterator ecals = boxTECAL_XZ.begin();
       ecals != boxTECAL_XZ.end(); ++ecals) {
    ecals->Draw("same");
  }

  for (std::vector<TBox>::iterator ecals = boxPECAL_XZ.begin();
       ecals != boxPECAL_XZ.end(); ++ecals) {
    ecals->Draw("same");
  }

  if (fDrawSMRD)
    for (int s = 0; s < 2; s++) boxSMRD_XZ[s]->Draw("same");

  XZ->Update();

  //------ Draw the XY view --------
  XY->cd();
  XYHisto->Draw();

  if (using_geometry) {
    for (size_t i = 0; i < MMvectorXY.size(); i++) MMvectorXY[i]->Draw("same");
  }

  else {
    boxtpc_XY->Draw("same");
  }

  if (fDrawSMRD)
    for (int s = 0; s < 4; s++) boxSMRD_XY[s]->Draw("same");

  XY->Update();
}

//**************************************************************
void ToaAnalysisEventDisplayUtils::DrawEventInfo(int run, int evt) {
  //**************************************************************

  char label[256];

  sprintf(label, " Run %d, Evt %d", run, evt);

  if (EvtLabel) delete EvtLabel;

  if (fDrawSMRD)
    EvtLabel = new TText(-2500., 3500., label);
  else
    EvtLabel = new TText(-2100., 1930., label);

  YZ->cd();
  EvtLabel->Draw();
  YZ->Update();
}

//**************************************************************
void ToaAnalysisEventDisplayUtils::DrawTrack(
    const ND::TGlobalReconModule::TGlobalPID &globalTrack) {
  //**************************************************************

  int det[23] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11,
                 11, 12, 12, 13, 14, 15, 15, 16, 16, 17, 18};

  double zmin;
  double zmax;
  if (globalTrack.FrontPosition.Z() < globalTrack.BackPosition.Z()) {
    zmin = globalTrack.FrontPosition.Z();
    zmax = globalTrack.BackPosition.Z();
  } else {
    zmin = globalTrack.BackPosition.Z();
    zmax = globalTrack.FrontPosition.Z();
  }

  std::vector<TVector3> positions;

  TVector3 pos = globalTrack.FrontPosition.Vect();
  positions.push_back(pos);

  for (int j = 0; j < NDETSEXTRAP; j++) {
    if (!globalTrack.DetectorUsed[det[j]]) continue;

    if (globalTrack.EntranceOK[j] &&
        fabs(globalTrack.EntrancePosition[j].Z()) > 0.0001) {
      pos = globalTrack.EntrancePosition[j].Vect();
      positions.push_back(pos);
    }
    if (globalTrack.ExitOK[j] &&
        fabs(globalTrack.ExitPosition[j].Z()) > 0.0001) {
      pos = globalTrack.ExitPosition[j].Vect();
      positions.push_back(pos);
    }
  }

  TVector3 pos2 = globalTrack.BackPosition.Vect();
  positions.push_back(pos2);

  std::sort(positions.begin(), positions.end(),
            ToaAnalysisEventDisplayUtils::SortInZ);

  int npoints = 0;

  for (size_t j = 0; j < positions.size(); j++) {
    if (positions[j].Z() < zmin || positions[j].Z() > zmax) continue;
    if (j > 0)
      if (fabs(positions[j].Z() - positions[j - 1].Z()) < 0.01) continue;

    npoints++;
  }

  if (npoints > kMaxPoints) {
    std::cout << "[WARN]" << __FILE__ << ":" << __LINE__ << " attempting to"
                                                            " plot "
              << npoints << " (> " << kMaxPoints << ")." << std::endl;
  }

  double xf[kMaxPoints];
  double yf[kMaxPoints];
  double zf[kMaxPoints];

  int n = 0;
  for (size_t j = 0; j < std::min(kMaxPoints, positions.size()); j++) {
    if (positions[j].Z() < zmin || positions[j].Z() > zmax) {
      continue;
    }
    if (j > 0) {
      if (fabs(positions[j].Z() - positions[j - 1].Z()) < 0.01) {
        continue;
      }
    }

    xf[n] = positions[j].X();
    yf[n] = positions[j].Y();
    zf[n] = positions[j].Z();

    n++;
  }

  DrawPoints(npoints, xf, yf, zf, fTrackColor);
}

//**************************************************************
void ToaAnalysisEventDisplayUtils::DrawTrackDebug(
    const ND::TGlobalReconModule::TGlobalPID &globalTrack) {
  //**************************************************************

  int det[NDETSEXTRAP] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11,
                          11, 12, 12, 13, 14, 15, 15, 16, 16, 17, 18};

  double xf[50];
  double yf[50];
  double zf[50];

  int n = 0;

  TVector3 minZpoint;
  TVector3 maxZpoint;
  if (globalTrack.FrontPosition.Z() < globalTrack.BackPosition.Z()) {
    minZpoint = globalTrack.FrontPosition.Vect();
    maxZpoint = globalTrack.BackPosition.Vect();
  } else {
    minZpoint = globalTrack.BackPosition.Vect();
    maxZpoint = globalTrack.FrontPosition.Vect();
  }

  xf[n] = minZpoint.X();
  yf[n] = minZpoint.Y();
  zf[n] = minZpoint.Z();
  n++;

  for (int j = 0; j < NDETSEXTRAP; j++) {
    if (!globalTrack.DetectorUsed[det[j]]) continue;

    if (globalTrack.FrontPosition.Z() < globalTrack.BackPosition.Z()) {
      if (globalTrack.EntranceOK[j] &&
          fabs(globalTrack.EntrancePosition[j].Z()) > 0.0001) {
        if (globalTrack.EntrancePosition[j].Z() > zf[n] &&
            globalTrack.EntrancePosition[j].Z() < maxZpoint.Z()) {
          xf[n] = globalTrack.EntrancePosition[j].X();
          yf[n] = globalTrack.EntrancePosition[j].Y();
          zf[n] = globalTrack.EntrancePosition[j].Z();
          n++;
        }
      }

      if (globalTrack.ExitOK[j] &&
          fabs(globalTrack.ExitPosition[j].Z()) > 0.0001) {
        if (globalTrack.ExitPosition[j].Z() > zf[n] &&
            globalTrack.ExitPosition[j].Z() < maxZpoint.Z()) {
          xf[n] = globalTrack.ExitPosition[j].X();
          yf[n] = globalTrack.ExitPosition[j].Y();
          zf[n] = globalTrack.ExitPosition[j].Z();

          n++;
        }
      }
    } else {
      if (globalTrack.ExitOK[j] &&
          fabs(globalTrack.ExitPosition[j].Z()) > 0.0001) {
        if (globalTrack.ExitPosition[j].Z() > zf[n] &&
            globalTrack.ExitPosition[j].Z() < maxZpoint.Z()) {
          xf[n] = globalTrack.ExitPosition[j].X();
          yf[n] = globalTrack.ExitPosition[j].Y();
          zf[n] = globalTrack.ExitPosition[j].Z();

          n++;
        }
      }

      if (globalTrack.EntranceOK[j] &&
          fabs(globalTrack.EntrancePosition[j].Z()) > 0.0001) {
        if (globalTrack.EntrancePosition[j].Z() > zf[n] &&
            globalTrack.EntrancePosition[j].Z() < maxZpoint.Z()) {
          xf[n] = globalTrack.EntrancePosition[j].X();
          yf[n] = globalTrack.EntrancePosition[j].Y();
          zf[n] = globalTrack.EntrancePosition[j].Z();
          n++;
        }
      }
    }
  }

  xf[n] = maxZpoint.X();
  yf[n] = maxZpoint.Y();
  zf[n] = maxZpoint.Z();
  n++;

  const int npoints = n;

  if (npoints > kMaxPoints) {
    std::cout << "[WARN]" << __FILE__ << ":" << __LINE__ << " attempting to"
                                                            " plot "
              << npoints << " (> " << kMaxPoints << ")." << std::endl;
  }

  double xf2[kMaxPoints];
  double yf2[kMaxPoints];
  double zf2[kMaxPoints];

  for (int j = 0; j < std::min(kMaxPoints, size_t(npoints)); j++) {
    xf2[j] = xf[j];
    yf2[j] = yf[j];
    zf2[j] = zf[j];
  }

  DrawPoints(npoints, xf2, yf2, zf2, fTrackColor);
}

//**************************************************************
void ToaAnalysisEventDisplayUtils::DrawHits(TClonesArray *hits, int nhits,
                                            int hitColor) {
  //**************************************************************

  for (int i = 0; i < nhits; i++) {
    ND::TGlobalReconModule::TGlobalHit *hit =
        (ND::TGlobalReconModule::TGlobalHit *)(*hits)[i];
    DrawHit(*hit, hitColor);
  }
}

//**************************************************************
void ToaAnalysisEventDisplayUtils::DrawOutermostHits(
    const ND::TGlobalReconModule::TOutermostHits &hits, int hitColor) {
  //**************************************************************

  DrawHit(hits.hitMinX, hitColor);
  DrawHit(hits.hitMaxX, hitColor);
  DrawHit(hits.hitMinY, hitColor);
  DrawHit(hits.hitMaxY, hitColor);
  DrawHit(hits.hitMinZ, hitColor);
  DrawHit(hits.hitMaxZ, hitColor);
}

//**************************************************************
void ToaAnalysisEventDisplayUtils::DrawHit(
    const ND::TGlobalReconModule::TGlobalHit &hit, int hitColor) {
  //**************************************************************

  //------------ Draw reconstructed 2D hits ------------------------------

  const TVector3 pos = hit.Position;
  const TVector3 epos = hit.PositionError;

  TVector3 size(15, 15, 15);

  if (fProportionalHits) size = TVector3(epos.X(), epos.Y(), epos.Z());

  if (hit.Type == 010 || hit.Type == 110 || hit.Type == 011 ||
      hit.Type == 111) {
    YZ->cd();
    TBox *tb = new TBox(pos.Z() - size.Z(), pos.Y() - size.Y(),
                        pos.Z() + size.Z(), pos.Y() + size.Y());
    fBoxList.push_back(tb);
    tb->SetFillStyle(0);
    tb->SetLineColor(hitColor);
    tb->SetLineWidth(1);
    tb->SetLineStyle(1);
    tb->SetFillColor(hitColor);
    tb->Draw("same");
  }
  if (hit.Type == 100 || hit.Type == 110 || hit.Type == 101 ||
      hit.Type == 111) {
    XZ->cd();
    TBox *tb = new TBox(pos.Z() - size.Z(), pos.X() - size.X(),
                        pos.Z() + size.Z(), pos.X() + size.X());
    fBoxList.push_back(tb);
    tb->SetFillStyle(0);
    tb->SetLineColor(hitColor);
    tb->SetLineWidth(1);
    tb->SetLineStyle(1);
    tb->SetFillColor(hitColor);
    tb->Draw("same");
  }
  if (hit.Type == 110 || hit.Type == 111) {
    XY->cd();
    TBox *tb = new TBox(pos.X() - size.X(), pos.Y() - size.Y(),
                        pos.X() + size.X(), pos.Y() + size.Y());
    fBoxList.push_back(tb);
    tb->SetFillStyle(0);
    tb->SetLineColor(hitColor);
    tb->SetLineWidth(1);
    tb->SetLineStyle(1);
    tb->SetFillColor(hitColor);
    tb->Draw("same");
  }
}

//**************************************************************
void ToaAnalysisEventDisplayUtils::DrawVertex(
    const ND::TGlobalReconModule::TGlobalVertex &vertex, int vertexColor) {
  //**************************************************************

  const TVector3 pos = vertex.Position.Vect();
  const TVector3 vpos = vertex.Variance.Vect();
  TVector3 size(sqrt(vpos.X()), sqrt(vpos.Y()), sqrt(vpos.Z()));

  // Draw a box at the vertex with size equal to the vertex error

  YZ->cd();
  TBox *tb1 = new TBox(pos.Z() - size.Z(), pos.Y() - size.Y(),
                       pos.Z() + size.Z(), pos.Y() + size.Y());
  fBoxList.push_back(tb1);
  tb1->SetFillStyle(0);
  tb1->SetLineColor(vertexColor);
  tb1->Draw("same");

  XZ->cd();
  TBox *tb2 = new TBox(pos.Z() - size.Z(), pos.X() - size.X(),
                       pos.Z() + size.Z(), pos.X() + size.Z());
  fBoxList.push_back(tb2);
  tb2->SetFillStyle(0);
  tb2->SetLineColor(vertexColor);
  tb2->Draw("same");

  XY->cd();
  TBox *tb3 = new TBox(pos.X() - size.X(), pos.Y() - size.Y(),
                       pos.X() + size.X(), pos.Y() + size.Y());
  fBoxList.push_back(tb3);
  tb3->SetFillStyle(0);
  tb3->SetLineColor(vertexColor);
  tb3->Draw("same");

  // draw a bigger circle at the vertex

  double x[1], y[1], z[1];
  x[0] = pos.X();
  y[0] = pos.Y();
  z[0] = pos.Z();

  TGraph *txy = new TGraph(1, x, y);
  TGraph *txz = new TGraph(1, z, x);
  TGraph *tyz = new TGraph(1, z, y);

  txz->SetMarkerStyle(kOpenCircle);
  tyz->SetMarkerStyle(kOpenCircle);
  txy->SetMarkerStyle(kOpenCircle);

  txz->SetMarkerSize(1);
  tyz->SetMarkerSize(1);
  txy->SetMarkerSize(1);

  txz->SetLineWidth(2);
  tyz->SetLineWidth(2);
  txy->SetLineWidth(2);

  txz->SetMarkerColor(vertexColor);
  tyz->SetMarkerColor(vertexColor);
  txy->SetMarkerColor(vertexColor);

  fGraphList.push_back(txz);
  fGraphList.push_back(tyz);
  fGraphList.push_back(txy);

  XZ->cd();
  txz->Draw("P");
  YZ->cd();
  tyz->Draw("P");
  XY->cd();
  txy->Draw("P");
}

//**************************************************************
void ToaAnalysisEventDisplayUtils::DrawMCTrajectories(int truthColor) {
  //**************************************************************

  int det[5] = {6, 0, 7, 1, 8};

  for (int ii = 0; ii < p_NTRAJTYPES; ii++) {
    for (int jj = 0; jj < p_NTruthTrajs[ii]; jj++) {
      ND::TTruthTrajectoriesModule::TTruthTrajectory *traj =
          (ND::TTruthTrajectoriesModule::TTruthTrajectory
               *)(*p_TruthTrajs[ii])[jj];

      double x[20];
      double y[20];
      double z[20];

      int n = 0;
      x[n] = traj->InitPosition.X();
      y[n] = traj->InitPosition.Y();
      z[n] = traj->InitPosition.Z();
      n++;

      for (int kk = 0; kk < 5; kk++) {
        TLorentzVector entrance = TTruthUtils::EntrancePosition(traj,
          oaAnalysisEnums::IntToSubdetector(det[kk]));
        TLorentzVector exit = TTruthUtils::ExitPosition(traj,
          oaAnalysisEnums::IntToSubdetector(det[kk]));
        if (entrance.Z() != 0) {
          x[n] = entrance.X();
          y[n] = entrance.Y();
          z[n] = entrance.Z();
          n++;
        }
        if (exit.Z() != 0) {
          x[n] = exit.X();
          y[n] = exit.Y();
          z[n] = exit.Z();
          n++;
        }
      }

      x[n] = traj->FinalPosition.X();
      y[n] = traj->FinalPosition.Y();
      z[n] = traj->FinalPosition.Z();
      n++;

      int npoints = n;

      if (npoints > kMaxPoints) {
        std::cout << "[WARN]" << __FILE__ << ":" << __LINE__ << " attempting to"
                                                                " plot "
                  << npoints << " (> " << kMaxPoints << ")." << std::endl;
      }

      double x2[kMaxPoints];
      double y2[kMaxPoints];
      double z2[kMaxPoints];

      for (int j = 0; j < std::min(kMaxPoints, size_t(npoints)); j++) {
        x2[j] = x[j];
        y2[j] = y[j];
        z2[j] = z[j];
      }

      DrawPoints(npoints, x2, y2, z2, truthColor);
    }
  }
}

//**************************************************************
void ToaAnalysisEventDisplayUtils::DrawMCVertices(int vertexColor) {
  //**************************************************************

  int npoints = 0;

  for (int ii = 0; ii < p_NVTXTYPES; ii++) {
    npoints += p_NTruthVertices[ii];
  }

  if(npoints > kMaxPoints){
    std::cout << "[WARN]" << __FILE__ << ":" << __LINE__ << " attempting to"
      " plot " << npoints << " (> " << kMaxPoints << ")." << std::endl;
  }

  double x[kMaxPoints];
  double y[kMaxPoints];
  double z[kMaxPoints];

  int n = 0;
  for (int ii = 0; ii < p_NVTXTYPES; ii++) {
    for (int jj = 0; jj < p_NTruthVertices[ii]; jj++) {
      ND::TTruthVerticesModule::TTruthVertex *vertex =
          (ND::TTruthVerticesModule::TTruthVertex *)(*p_TruthVertices[ii])[jj];
      x[n] = vertex->Position.X();
      y[n] = vertex->Position.Y();
      z[n] = vertex->Position.Z();
      n = std::min(kMaxPoints,size_t(n+1));
    }
  }

  if (npoints == 0) return;

  DrawVertices(npoints, x, y, z, vertexColor, kOpenCircle);
}

//**************************************************************
void ToaAnalysisEventDisplayUtils::DrawPoints(int npoints, double *x, double *y,
                                              double *z, int color) {
  //**************************************************************

  if (npoints == 0) return;

  TGraph *txy = new TGraph(npoints, x, y);
  TGraph *txz = new TGraph(npoints, z, x);
  TGraph *tyz = new TGraph(npoints, z, y);

  txz->SetLineColor(color);
  tyz->SetLineColor(color);
  txy->SetLineColor(color);

  fGraphList.push_back(txz);
  fGraphList.push_back(tyz);
  fGraphList.push_back(txy);

  XZ->cd();
  txz->Draw("same C");
  YZ->cd();
  tyz->Draw("same C");
  XY->cd();
  txy->Draw("same C");
}

//**************************************************************
void ToaAnalysisEventDisplayUtils::DrawVertices(int npoints, double *x,
                                                double *y, double *z,
                                                int vertexColor, int marker) {
  //**************************************************************

  if (npoints == 0) return;

  TGraph *txy = new TGraph(npoints, x, y);
  TGraph *txz = new TGraph(npoints, z, x);
  TGraph *tyz = new TGraph(npoints, z, y);

  txz->SetMarkerStyle(marker);
  tyz->SetMarkerStyle(marker);
  txy->SetMarkerStyle(marker);

  txz->SetMarkerSize(2);
  tyz->SetMarkerSize(2);
  txy->SetMarkerSize(2);

  txz->SetLineWidth(2);
  tyz->SetLineWidth(2);
  txy->SetLineWidth(2);

  txz->SetMarkerColor(vertexColor);
  tyz->SetMarkerColor(vertexColor);
  txy->SetMarkerColor(vertexColor);

  fGraphList.push_back(txz);
  fGraphList.push_back(tyz);
  fGraphList.push_back(txy);

  XZ->cd();
  txz->Draw("P");
  YZ->cd();
  tyz->Draw("P");
  XY->cd();
  txy->Draw("P");
}

//**************************************************************
void ToaAnalysisEventDisplayUtils::InitExplanation() {
  //**************************************************************

  double size = 0.02;

  std::string s[8] = {"True trajectory",     "Global track",
                      "non-fitted object",   "First/last hits in track",
                      "Unused hits",         "Outermost hits",
                      "True primary vertex", "Rec vertex"};
  int color[8];
  color[0] = fTruthColor;
  color[1] = fTrackColor;
  color[2] = fOtherColor;
  color[3] = fObjectHitsColor;
  color[4] = fUnusedHitsColor;
  color[5] = fOutermostHitsColor;
  color[6] = fTrueVertexColor;
  color[7] = fRecVertexColor;

  int marker[5];

  marker[0] = fHitMarker;
  marker[1] = fHitMarker;
  marker[2] = fHitMarker;
  marker[3] = fTrueVertexMarker;
  marker[4] = fRecVertexMarker;

  double deltay_text = -50;
  double sizex = 50;
  double sizey = 55;
  double posy_ini = 3800.;
  double posx_ini = 1000;
  if (!fDrawSMRD) {
    deltay_text = -30;
    sizex = 35;
    sizey = 30;
    posy_ini = 2150.;
    posx_ini = 1200.;
  }

  // draw the text
  double posy = posy_ini;
  double posx = posx_ini;
  for (int i = 0; i < 8; i++) {
    if (i == 4) posy = posy_ini;
    if (i >= 4) posx = posx_ini + 1500;

    if (text[i]) delete text[i];
    text[i] = new TText(posx, posy + deltay_text, s[i].c_str());
    text[i]->SetTextSize(size);
    text[i]->SetTextColor(color[i]);

    if (i < 3) {
      if (tline[i]) delete tline[i];
      tline[i] = new TLine(posx - 200, posy, posx - 50, posy);
      tline[i]->SetLineColor(color[i]);
    } else {
      double x;
      double y;

      x = posx - 70;
      y = posy;

      if (tmarker[i - 3]) delete tmarker[i - 3];

      TBox *tb;
      TEllipse *te;

      if (i < 6) {
        tb = new TBox(x - sizex, y - sizey, x + sizex, y + sizey);
        tb->SetFillStyle(1);
        tb->SetFillColor(color[i]);
        tb->SetLineColor(color[i]);
        tmarker[i - 3] = tb;
      } else {
        te = new TEllipse(x, y, sizex, sizey);
        te->SetFillStyle(1);
        te->SetFillColor(color[i]);
        te->SetLineColor(color[i]);
        tmarker[i - 3] = te;
      }
    }

    if (fDrawSMRD)
      posy -= 160;
    else
      posy -= 90;
  }
}

//**************************************************************
void ToaAnalysisEventDisplayUtils::DrawExplanation() {
  //**************************************************************

  YZ->cd();

  for (int i = 0; i < 8; i++) {
    text[i]->Draw("same");

    if (i < 3)
      tline[i]->Draw("same");
    else
      tmarker[i - 3]->Draw("P");
  }
}

//**************************************************************
bool ToaAnalysisEventDisplayUtils::FindEvent(TChain &reconGlobal,
                                             int requestedEvent) {
  //**************************************************************

  mf->SetRequestedEvent(-1);

  for (int i = 0; i < fNumberEntries; i++) {
    reconGlobal.GetEntry(i);
    if (p_EventID == requestedEvent) {
      fCurrentEntry = i;
      mf->SetCurrentEntry(i);
      return true;
    }
  }

  return false;
}

//**************************************************************
ToaAnalysisEventDisplayUtils::MyMainFrame::MyMainFrame(const TGWindow *p,
                                                       UInt_t w, UInt_t h)
    : TGMainFrame(p, w, h) {
  //**************************************************************

  // Create a main frame with a number of different buttons.
  fButton1 = new TGTextButton(this, "&Next", 1);
  fButton2 = new TGTextButton(this, "&Previous", 2);
  fButton3 = new TGTextButton(this, "&SMRD", 3);
  fButton4 = new TGTextButton(this, "&Hits size", 4);
  fButton5 = new TGTextButton(this, "&Debug", 5);
  fNumber = new TGNumberEntry(this, 0, 8, -1, TGNumberFormat::kNESInteger,
                              TGNumberFormat::kNEANonNegative,
                              TGNumberFormat::kNELLimitMinMax, 0, 99999999);

  fLayout = new TGLayoutHints(kLHintsCenterX | kLHintsCenterY | kLHintsExpandX |
                              kLHintsExpandY);
  AddFrame(fButton1, fLayout);
  AddFrame(fButton2, fLayout);
  AddFrame(fButton3, fLayout);
  AddFrame(fButton4, fLayout);
  AddFrame(fButton5, fLayout);
  AddFrame(fNumber, fLayout);
  MapSubwindows();
  Layout();
  SetWindowName("Button Control");
  SetIconName("Button Control ");
  MapWindow();

  fDrawSMRD = true;
  fProportionalHits = false;
  fRequestedEvent = 0;
}

//**************************************************************
ToaAnalysisEventDisplayUtils::MyMainFrame::~MyMainFrame(void) {
  //**************************************************************

  std::cout << " Entering delete Main Frame " << std::endl;
  delete fButton1;
  delete fButton2;
  delete fButton3;
  delete fButton4;
  delete fButton5;
  delete fNumber;

  delete fLayout;
}

//**************************************************************
Bool_t ToaAnalysisEventDisplayUtils::MyMainFrame::ProcessMessage(Long_t msg,
                                                                 Long_t parm1,
                                                                 Long_t) {
  //**************************************************************

  // Process events generated by the buttons in the frame.
  switch (GET_MSG(msg)) {
    case kC_COMMAND:
      switch (GET_SUBMSG(msg)) {
        case kCM_BUTTON:
          printf("text button id %ld pressed\n", parm1);
          if (parm1 == 1) {
            if (fCurrentEntry < fMaxEntry) {
              fCurrentEntry++;
              std::cout << " Next Event " << std::endl;
            } else
              std::cout << " Already in last event " << std::endl;
            gApplication->Terminate(true);
          }
          if (parm1 == 2) {
            if (fCurrentEntry > 0) {
              fCurrentEntry--;
              std::cout << " Previous Event " << std::endl;
            } else
              std::cout << " Already in first event " << std::endl;
            gApplication->Terminate(true);
          }
          if (parm1 == 3) {
            if (fDrawSMRD)
              fDrawSMRD = false;
            else
              fDrawSMRD = true;
            std::cout << " SMRD: " << fDrawSMRD << std::endl;
            gApplication->Terminate(true);
          }
          if (parm1 == 4) {
            if (fProportionalHits)
              fProportionalHits = false;
            else
              fProportionalHits = true;
            std::cout << " Proportional hits: " << fProportionalHits
                      << std::endl;
            gApplication->Terminate(true);
          }

          if (parm1 == 5) {
            if (fDebug)
              fDebug = false;
            else
              fDebug = true;
            std::cout << " Debug: " << fDebug << std::endl;
            gApplication->Terminate(true);
          }

        default:
          break;
      }
    case kC_TEXTENTRY:
      switch (GET_SUBMSG(msg)) {
        case kTE_ENTER:
          fRequestedEvent = fNumber->GetIntNumber();
          std::cout << "requestedEvent: " << fRequestedEvent << std::endl;
          gApplication->Terminate(true);
      }
    default:
      break;
  }

  return kTRUE;
}

//**************************************************************
void ToaAnalysisEventDisplayUtils::MyMainFrame::GoToEventNumber() {
  //**************************************************************

  Int_t requestedEvent = fNumber->GetIntNumber();
  std::cout << "requestedEvent: " << requestedEvent << std::endl;
}

//**************************************************************
void ToaAnalysisEventDisplayUtils::TBoxPad::ExecuteEvent(Int_t event) {
  //**************************************************************
  if (!gPad) return;

  TVirtualPad *savedpad = gPad;

  if (event != kMouseEnter) return;

  HideToolTip(event);

  savedpad->cd();
}

//**************************************************************
void ToaAnalysisEventDisplayUtils::TBoxPad::Add(double t, double a) {
  //**************************************************************

  if (t < timemin) timemin = t;
  if (t > timemax) timemax = t;

  time.push_back(t);
  adc.push_back(a);
}

//**************************************************************
TH1F *ToaAnalysisEventDisplayUtils::TBoxPad::histogram() {
  //**************************************************************

  if (timemin > timemax) return NULL;

  if (!histo) {
    char name[256];
    char title[256];
    sprintf(name, "WF_%d_%d_%d_%d", tpc, half, mm, pad);
    sprintf(title, "TPC %d HALF %d MM %d PAD %d", tpc, half, mm, pad);

    histo =
        new TH1F(name, title, (int)(timemax - timemin + 1), timemin, timemax);
    for (size_t i = 0; i < time.size(); i++) {
      double a = adc[i];
      double t = time[i];
      histo->Fill(t, a);
    }
  }

  return histo;
}
