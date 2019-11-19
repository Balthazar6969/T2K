#include "iostream"
#include "vector"
#include <map>

//#include <AnalysisTools/TCCPlotGlobalAnalysisUtils.hxx>

//______________________________________________________________________________________
// 
// In order to run this script, you must do the following.
//
// 1) You must have a file (or files) containing the oaAnalysis summary ROOT tree.  
//    This must be generated using OAAnalysis.exe, if you don't already 
//    have it.
// 2) Build the oaAnalysis library in the normal way.
// 3) Go to the macros directory.
// 4) Edit the file called 'ccGlobalAnalysis_input_files.txt': this should be a list of all
//    the summary files that you want to process.
// 5) Start root and run this macro by doing:
//    .x ccGlobalAnalysis.C()



using namespace std;


//define histos
TH1F *trueCCQEcand_deltaThetaP;
TH1F *trueDIScand_deltaThetaP;
TH1F *trueNCcand_deltaThetaP;
TH1F *trueCC1PIcand_deltaThetaP;

TH1F *trueCCQEcand_deltaThetaP_afterNuInvMassCut;
TH1F *trueDIScand_deltaThetaP_afterNuInvMassCut;
TH1F *trueNCcand_deltaThetaP_afterNuInvMassCut;
TH1F *trueCC1PIcand_deltaThetaP_afterNuInvMassCut;

TH1F *trueCCQEcand_nuInvariantMass;
TH1F *trueDIScand_nuInvariantMass;
TH1F *trueNCcand_nuInvariantMass;
TH1F *trueCC1PIcand_nuInvariantMass;

TH1F *michelClusterTime;
TH1F *michelClusterXPos;
TH1F *michelClusterYPos;
TH1F *michelClusterZPos;
TH1F *vertexTime;
TH1F *vertexXPos;
TH1F *vertexYPos;
TH1F *vertexZPos;
TH1F *timeDiff;
TH1F *distance;

TH1F *timeDiff_FGD1;
TH1F *distance_FGD1;

TH1F *timeDiff_FGD2;
TH1F *distance_FGD2;

TH1F *distance_CCQE;
TH1F *distance_notCCQE;
TH1F *distance_FGD1_CCQE;
TH1F *distance_FGD1_notCCQE;
TH1F *distance_FGD2;
TH1F *distance_FGD2_CCQE;
TH1F *distance_FGD2_notCCQE;


TH1F *trackVtx;
TH1F *vtxTime;
TH1F *timeDiff_1;
TH1F *trackVtx_notZeroTrackTime;
TH1F *vtxTime_notZeroTrackTime;
TH1F *timeDiff_notZeroTrackTime;
TH1F *timeDiff_notZeroTrackTime_ZOOM;
TH1F *unusedHitsInFGD1;
TH1F *unusedHitsInFGD2;
TH1F *clusterT;
TH1F *trackVtx_cluster;
TH1F *trackVtx_cluster_notZeroTrackTime;
TH1F *timeDiff_cluster_notZeroTrackTime;
TH1F *timeDiff_cluster;
TH1F *timeDispersion;
TH1F *spatialDispersion;
TH2F *dispersion;
TH1F *NbrOfHitsPerCluster;


void InitializeHistos();
void FillDeltaThetaPHistos(double deltaThetaP, int reacCode);
void FillDeltaThetaPHistos_afterNuInvMassCut(double deltaThetaP, int reacCode);
void FillNuInvariantMassHistos(double nuInvMass, int reacCode);

void FillMichelElectronHistos(double clusterTime, double clusterXPos, double clusterYPos, double clusterZPos, double vtxTime, double vtxXPos, double vtxYPos, double vtxZPos, double time, double dist, int NVtxFGD1, int NVtxFGD2);
void FillMichelEDistanceHistos(double dist, int NVtxFGD1, int NVtxFGD2, int reacCode);
void FillVertexTimeHistos(double trackTime, double vertexTime, double tDiff);
void FillUnusedFGDHits(int unusedFGD1, int unusedFGD2);
void FillClusterTimeHistos(double clstrTime, double trackTime, double tDiff);
void FillDispersionHistos(double sDisp, double tDisp);
void FillNbrOfHitsHisto(double nbrHits);


void DrawHistos();

//----------------------------------------------


void  ccGlobalAnalysis() {

  bool debug = false;

  double momRelErrorCut = 0.2;
  double michelCut = 800;

  // Load some important libraries.
 
  // gSystem->AddIncludePath("-I$OAEVENTROOT/src");
  gROOT->SetBatch(1);

  gSystem->AddIncludePath("-I$OAANALYSISROOT/src");
  gSystem->AddIncludePath("-I.");
  //  gROOT->ProcessLine(".x rootlogon.C");
  
  //TEMPORARY SOLUTION
  gSystem->Load("libPhysics");
  gSystem->Load("libGeom");
  gSystem->Load("libTree");
  gSystem->Load("libMatrix");
  gSystem->Load("liboaEvent.so");
  gSystem->Load("librecpack.so");
  gSystem->Load("liboaRecPack.so");
  gSystem->Load("liboaAnalysis.so");

  gROOT->ProcessLine(".L AnalysisTools/TNEUTAnalysisUtils.cxx");
  gROOT->ProcessLine(".L AnalysisTools/TTrackerAnalysisUtils.cxx");
  //gROOT->ProcessLine(".L AnalysisTools/TCCreconAnalysisUtils.cxx");
  gROOT->ProcessLine(".L AnalysisTools/TCCtruthAnalysisUtils.cxx");
  gROOT->ProcessLine(".L AnalysisTools/TGlobalAnalysisUtils.cxx");

  //gROOT->ProcessLine(".L AnalysisTools/TCCPlotGlobalAnalysisUtils.cxx");

  TChain mcTruthVertices = TChain("TruthDir/Vertices");
  TChain mcTruthTrajectories = TChain("TruthDir/Trajectories");
  TChain reconTracker = TChain("ReconDir/Tracker");
  TChain reconGlobal = TChain("ReconDir/Global");
  
  TNEUTAnalysisUtils *NeutUtils = new TNEUTAnalysisUtils();
  TTrackerAnalysisUtils *AnTUtils = new TTrackerAnalysisUtils();  
  //TCCreconAnalysisUtils *recAn = new TCCreconAnalysisUtils();
  TCCtruthAnalysisUtils *trueAn = new TCCtruthAnalysisUtils();
  TGlobalAnalysisUtils *globAn = new TGlobalAnalysisUtils();

  //TCCPlotGlobalAnalysisUtils *plotAn = new TCCPlotGlobalAnalysisUtils();
  
  //______________________________________________________________________________________
  // Set constants for analysis
  
  double muMass = 105.658367; //MeV (muon mass)
  double pMass = 938.27203; //MeV (proton mass)
  double nMass = 939.56536; //MeV (neutron mass)
  double V = 25.; //MeV (nuclear potential)

  //initialize histograms
  //plotAn->InitializeHistos;
  InitializeHistos();

  // _____________________________________________________________________________________
  // Load data files   

  TString fileName("ccGlobalAnalysis_input_rms.txt");
  //TString fileName("ccGlobalAnalysis_input_files.txt");
  //TString fileName("ccGlobalAnalysis_input_files_new.txt");
  //TString fileName("ccGlobalAnalysis_input_files_100ns.txt");
  //TString fileName("ccGlobalAnalysis_eloss_input_files.txt");  
  

  ifstream inputFile(fileName.Data(), ios::in);
  
  if (!inputFile) {
    cerr << "Cannot open input file '" << fileName.Data() << "'. Exit!" << endl;
    return;
  }
  
  string inputString;   
  while (inputFile >> inputString) {
    std::cout << "Adding file: " << inputString << std::endl;
    mcTruthVertices.AddFile(inputString.c_str());
    mcTruthTrajectories.AddFile(inputString.c_str());
    reconTracker.AddFile(inputString.c_str());

    reconGlobal.AddFile(inputString.c_str());
  }    
  
  // _____________________________________________________________________________________
  // Prepare to read trees.
  
  // Set the branches for the truth information.
  Int_t NVtxFGD1; 
  Int_t NVtxFGD2;
  TClonesArray *VtxFGD1 = new TClonesArray("ND::TTruthVerticesModule::TTruthVertex", 20);
  TClonesArray *VtxFGD2 = new TClonesArray("ND::TTruthVerticesModule::TTruthVertex", 20);
  
  cout<<" set branches adresses "<<endl;
  mcTruthVertices.SetBranchAddress("NVtxFGD1", &NVtxFGD1);
  mcTruthVertices.SetBranchAddress("NVtxFGD2", &NVtxFGD2);
  mcTruthVertices.SetBranchAddress("VtxFGD1", &VtxFGD1);
  mcTruthVertices.SetBranchAddress("VtxFGD2", &VtxFGD2);   
  mcTruthVertices.Print("toponly");
  
  
  TRefArray *AllTruthTrajectories = new TRefArray();
  mcTruthTrajectories.SetBranchAddress("AllTrajectories",AllTruthTrajectories);    
  mcTruthTrajectories.Print("toponly");
  
  //AllTrajectories(new TRefArray()       
  // Set the branches for the tracker recon information.

  Int_t NVertices;
  TClonesArray *Vertices = new TClonesArray("ND::TTrackerReconModule::TTrackerVertex", 20);
  reconTracker.SetBranchAddress("NVertices", &NVertices);
  reconTracker.SetBranchAddress("Vertices", &Vertices);

  Int_t NTracks;
  TClonesArray *Tracks = new TClonesArray("ND::TTrackerReconModule::TTrackerTrack", 20);
  reconTracker.SetBranchAddress("NTracks", &NTracks);
  reconTracker.SetBranchAddress("Tracks", &Tracks);

  /*
  Int_t NDelayedClusters;
  TClonesArray *DelayedClusters = new TClonesArray("ND::TTrackerReconModule::TFgdCluster", 20);
  reconTracker.SetBranchAddress("NDelayedClusters", &NDelayedClusters);
  reconTracker.SetBranchAddress("DelayedClusters", &DelayedClusters);    
  */

  reconTracker.Print("toponly");

  Int_t NPIDs;
  TClonesArray *PIDs = new TClonesArray("ND::TGlobalReconModule::TGlobalPID", 20);
  reconGlobal.SetBranchAddress("NPIDs", &NPIDs);
  reconGlobal.SetBranchAddress("PIDs", &PIDs);

  Int_t NDelayedClusters;                           
  TClonesArray *DelayedClusters = new TClonesArray("ND::TGlobalReconModule::TFgdCluster", 10);
  reconGlobal.SetBranchAddress("NDelayedClusters", &NDelayedClusters);
  reconGlobal.SetBranchAddress("DelayedClusters", &DelayedClusters);    

  double EarliestTrackTime;
  reconGlobal.SetBranchAddress("EarliestTrackTime", &EarliestTrackTime);

  int NFGD1Unused;
  reconGlobal.SetBranchAddress("NFGD1Unused", &NFGD1Unused);

  int NFGD2Unused;
  reconGlobal.SetBranchAddress("NFGD2Unused", &NFGD2Unused);


  //  Int_t NGlobalVertices;
  //  TClonesArray *GlobalVertices = new TClonesArray("ND::TGlobalReconModule::TGlobalVertex", 20);
  //  reconGlobal.SetBranchAddress("NVertices", &NGlobalVertices);
  //  reconGlobal.SetBranchAddress("Vertices", &GlobalVertices);



  reconGlobal.Print("toponly");
  
  
  //__________________
  // counters and cuts
  
  int total_ccqe_fiducial = 0;
  int total_selected[9] = {0,0,0,0,0,0,0,0,0};
  int total_selected_ccqe_fiducial[9] = {0,0,0,0,0,0,0,0,0};
  
  int total_ccqe_fiducial_2tracks = 0;
  int total_selected_2tracks[8] = {0,0,0,0,0,0,0,0};
  int total_selected_ccqe_fiducial_2tracks[8] = {0,0,0,0,0,0,0,0};
  
  bool passedVertexCut = false;
  bool passedRecVertexCut = false;
  bool passedTracksNbrCut = false;
  bool passedCurvatureCut = false;
  bool passedNegTpcCut = false;    
  bool passedShowerCut = false;
  bool passedPosTpcCut = false;
  bool passedDeltaThetaPCut = false;
  bool passedMichelElectronCut = false;
  bool passedNuInvariantMassCut = false;

  bool enableTwoTrackAnalysis = true;
  bool isCCQE = false;
  bool isInFiducialVolume = false;
  
  bool oneAn = false;

  // __________________________________________________________________________
  // Loop over all events.  

  //int numberEntries = mcTruthVertices.GetEntries();
  int numberEntries = 100;
  std::cout << "Number entries: " << numberEntries << std::endl;
  
  // time before event loop
  TStopwatch timer;
  timer.Start();
  
  for(int ient = 0; ient < numberEntries; ient++){  //start event loop
    //for(int ient = 0; ient < 42; ient++){  //start event loop
    
    cout << "--------> begin event " << ient << endl;
    
    mcTruthVertices.GetEntry(ient);
    //mcTruthTrajectories.GetEntry(ient);
    reconTracker.GetEntry(ient);
    reconGlobal.GetEntry(ient);
    
    if(ient%500 == 0) std::cout <<  "Events Processed " << ient << std::endl;
    
    
    // -----------------------------------------------------------------
    // Check vertex truth information. (fiducial and CCQE reaction code)
    
    // Get the true FGD vertex
    ND::TTruthVerticesModule::TTruthVertex *true_fgd_vertex = NULL;
    if(NVtxFGD1 >0) true_fgd_vertex = (ND::TTruthVerticesModule::TTruthVertex*)VtxFGD1->UncheckedAt(0);
    else if (NVtxFGD2 >0) true_fgd_vertex = (ND::TTruthVerticesModule::TTruthVertex*)VtxFGD2->UncheckedAt(0);
    else {
      std::cerr << "There should be at least one vertex in one FGD.  Skipping event."<<std::endl;
      continue;
    }
    
    // Get true primary vertex
    TLorentzVector truthVertex = true_fgd_vertex->Vertex;  
    
    // Check that the true vertex is inside the fiducial volume  
    TVector3 fgd_fiducial_trim = (200.0,200.0,10.0);
    AnTUtils->DefineFiducialVolume(fgd_fiducial_trim);
    
    isInFiducialVolume = false;
    isInFiducialVolume = AnTUtils->IsInFiducialVolume(truthVertex.Vect());
    //if (AnTUtils->IsInFiducialFGD1(truthVertex.Vect()) || AnTUtils->IsInFiducialFGD2(truthVertex.Vect())) isInFiducialVolume = true;
    
    // check CCQE interaction
    isCCQE = NeutUtils->IsQELCC(true_fgd_vertex);    
    
    int reacCode =  NeutUtils->GetReactionCode(true_fgd_vertex);
    //cout << "Reaction code: " << NeutUtils->GetReactionCode(true_fgd_vertex) << endl;
    

    //-----------------------------------
    //loop over recon trajectories in TPC
    //-----------------------------------
    
    int kTotalTPC = 0;
    
    int NNegReconTrack = 0;
    int NMaxNegReconTrack = 20;
    int NPosReconTrack = 0;
    int NMaxPosReconTrack = 20;
    int NRecTrack = 0;
    int NMaxRecTrack = 20;

    int NFgdTrack = 0;
    int NNoTPCTrack = 0;
    int NEcalTrack = 0;

    ND::TGlobalReconModule::TGlobalPID recTrack[NMaxRecTrack];
    ND::TGlobalReconModule::TGlobalPID recNegTrack[NMaxNegReconTrack];
    ND::TGlobalReconModule::TGlobalPID recPosTrack[NMaxPosReconTrack];

    
    //global tracks    
    for( int j=0; j<NPIDs; j++) {
      ND::TGlobalReconModule::TGlobalPID *globalTrack = (ND::TGlobalReconModule::TGlobalPID*)(*PIDs)[j];      

      //the track is in one of the TPCs:
      //if (!(globAn->IsInTPC(globalTrack))) continue;
      
      //if (globalTrack->TrueParticle.InitMom.Z() <=  0) continue;

      //the track is in one of the TPCs:
      if (!(globAn->IsInTPC(globalTrack))){	
	NNoTPCTrack++;	
	
	if(globalTrack->Detectors == 4 || globalTrack->Detectors ==5) {
	  NFgdTrack++;
	}
	if(globalTrack->Detectors == 6 || globalTrack->Detectors ==7) {
	  NEcalTrack++;
	}
	continue;
      }
      
      if (NRecTrack < NMaxRecTrack) 
	recTrack[NRecTrack] = *globalTrack;	              
      NRecTrack++;
      if (globalTrack->Charge > 0) {
	//cout << "pos charge = " << globalTrack->Charge << endl;
	if (NPosReconTrack < NMaxPosReconTrack) 
	  recPosTrack[NPosReconTrack] = *globalTrack;	
	NPosReconTrack++;
      }
      
      if (globalTrack->Charge < 0) {
	//cout << "neg charge = " << globalTrack->Charge << endl;
	if (NNegReconTrack < NMaxNegReconTrack) 
	  recNegTrack[NNegReconTrack] = *globalTrack;
	NNegReconTrack++;
      }
    }


     
    //global vertex
    int NVertex = 0;
    int NMaxVertex = 20;
    ND::TTrackerReconModule::TTrackerVertex vertex[NMaxVertex];
    for(int j=0; j<NVertices; j++){      
      ND::TTrackerReconModule::TTrackerVertex *vtx = (ND::TTrackerReconModule::TTrackerVertex*)(*Vertices)[j];
      if (NVertex < NMaxVertex) 
	vertex[NVertex] = *vtx;
      NVertex++;
      cout << "NVertex = "<< NVertex << endl;
    }
    

    /*
    //clusters
    int NCluster = 0;
    int NMaxCluster = 20;
    ND::TTrackerReconModule::TFgdCluster cluster[NMaxCluster];
    for(int j=0; j<NDelayedClusters; j++){      
      ND::TTrackerReconModule::TFgdCluster *clstr = (ND::TTrackerReconModule::TFgdCluster*)(*DelayedClusters)[j];
      if (NCluster < NMaxCluster) 
	cluster[NCluster] = *clstr;	
      NCluster++;
    }
    */


    //clusters
    int NCluster = 0;
    int NMaxCluster = 10;    
    ND::TGlobalReconModule::TFgdCluster cluster[NMaxCluster];

    //Michel electron candidates
    int NMichelElectron = 0;
    int NMaxMichelElectron = 10;    
    ND::TGlobalReconModule::TFgdCluster michelElectron[NMaxMichelElectron];
    
    for(int j=0; j<NDelayedClusters; j++) {      

      ND::TGlobalReconModule::TFgdCluster *clstr   = (ND::TGlobalReconModule::TFgdCluster*)(*DelayedClusters)[j];
      ND::TGlobalReconModule::TFgdCluster *michelElec   = (ND::TGlobalReconModule::TFgdCluster*)(*DelayedClusters)[j];
      
      if (NCluster < NMaxCluster)
	cluster[NCluster] = *clstr;	      
      NCluster++;

      if (globAn->IsAMichelElectronCandidate(*clstr, truthVertex, 800)) {
	
	if (NMichelElectron < NMaxMichelElectron) {
	  michelElectron[NMichelElectron] = *michelElec;
	}
	NMichelElectron++;
      }
    }


    //Check track with both TPC and FGD constituents
    int NTrackInTPCAndInFGD = 0;
    int NMaxTrackInTPCAndInFGD = 20;
    ND::TGlobalReconModule::TGlobalPID trackInTPCAndInFGD[NMaxTrackInTPCAndInFGD];    
    //global tracks with at least one FGD constituent   
    for( int j=0; j<NPIDs; j++) {
      ND::TGlobalReconModule::TGlobalPID *globalTrack = (ND::TGlobalReconModule::TGlobalPID*)(*PIDs)[j];      
      if (globAn->IsInTPCAndInFGD(globalTrack)) {
	NTrackInTPCAndInFGD++;
	if (NTrackInTPCAndInFGD < NMaxTrackInTPCAndInFGD) 
	  trackInTPCAndInFGD[NTrackInTPCAndInFGD] = *globalTrack;
      }
    }    
    
    
    //check tracks with FGD constituents (no requirements on TPC constituents)
    int NTrackInFGD = 0;
    int NMaxTrackInFGD = 20;
    ND::TGlobalReconModule::TGlobalPID trackInFGD[NMaxTrackInFGD];    
    //global tracks with at least one FGD constituent   
    for( int j=0; j<NPIDs; j++) {
      ND::TGlobalReconModule::TGlobalPID *globalTrack = (ND::TGlobalReconModule::TGlobalPID*)(*PIDs)[j];      
      if (globAn->IsInFGD(globalTrack)) {
	NTrackInFGD++;
	if (NTrackInFGD < NMaxTrackInFGD) 
	  trackInFGD[NTrackInFGD] = *globalTrack;
      }
    }    
    double trackVertexTime = EarliestTrackTime; 
    double trueVertexTime = truthVertex.T();
    double diff = trackVertexTime - trueVertexTime;
    FillVertexTimeHistos(trackVertexTime, trueVertexTime, diff);
    
    FillUnusedFGDHits(NFGD1Unused, NFGD2Unused);

    
    /*
    cout << "# rec tracks = " << NRecTrack << endl;
    cout << "# pos tracks = " << NPosReconTrack << endl;
    cout << "# neg tracks = " << NNegReconTrack << endl;
    cout << "# FGD tracks = " << NFgdTrack << endl;
    cout << "# ECAL tracks = " << NEcalTrack << endl;
    cout << "# no tpc tracks = " << NNoTPCTrack << endl;
    cout << "# clusters = " << NCluster << endl;
    */



    //________________________________
    // Apply event selection criterion.
    
    // Reset variables
    passedVertexCut = false;
    passedRecVertexCut = false;
    passedTracksNbrCut = false;
    passedCurvatureCut = false;
    passedNegTpcCut = false;
    passedShowerCut = false;
    passedPosTpcCut = false;
    passedDeltaThetaPCut = false;
    passedMichelElectronCut = false;
    passedNuInvariantMassCut = false;



    
    //___________
    // Apply cuts      
    
    //double ENuRec = 0;
    double nuInvariantMass = 0;
    double deltaThetaP = 0;


    // 0. true vertex in fiducial volume
    if (isInFiducialVolume) {     
      passedVertexCut = true;
      if (debug) cout << " 0. trueVtx in fiducial " << endl;      

      // 1. reconstructed vertex in fiducial volume
      //if (recVtxIsInFiducialVolume) {
      if (true) {
	passedRecVertexCut = true;
	if (debug) cout << " 1. recVtx in fiducial " << endl;      



	//---------  One track Analysis-----------------
	
	// 2. only one track
	if (NRecTrack == 1) {
	  passedTracksNbrCut = true;
	  if (debug) cout << " 2. one track " << endl;

	  // 3. useful TPC tracks
	  double momRelError1 = 100;
	  if (recTrack[0].Momentum != 0){
	    momRelError1 = fabs(recTrack[0].MomentumError/recTrack[0].Momentum);
	  }
	  if (momRelError1 < momRelErrorCut){ 
	    passedCurvatureCut = true;
	    if (debug) cout << " 3. one useful TPC track " << endl;
	    
	    // 4. negative track
	    if (NNegReconTrack == 1) {
	      passedNegTpcCut = true;
	      if (debug) cout << " 4. one negative track " << endl;
	     	      
	      // 5. not a shower in the ECal
	      if (globAn->IsNotAShower(recNegTrack[0])) {
		passedShowerCut = true;
		if (debug) cout << " 5. not a shower " << endl;
		
		// 6. positive track 
		passedPosTpcCut = true;
		
		// 7. deltaThetaP cut 
		passedDeltaThetaPCut = true;


		//TEMP
		if (NMichelElectron>0) {
		  for (int i=0; i<NMichelElectron; i++) {
		    //double clusterTime = cluster[i].Time;
		    double clusterTime = cluster[i].Position.T();
		    double diff_1 = clusterTime - trackVertexTime;
		    FillClusterTimeHistos(clusterTime, trackVertexTime, diff_1);

		    double timeDisp = cluster[i].TimeDisp;
		    double spatialDisp = cluster[i].SpatialDisp;
		    double nbrOfHits = cluster[i].NHits;
		    FillDispersionHistos(spatialDisp,timeDisp);
		    FillNbrOfHitsHisto(nbrOfHits);

		    double vertexTime = truthVertex.T();
		    double vertexZPos = truthVertex.Z();
		    double vertexYPos = truthVertex.Y();
		    double vertexXPos = truthVertex.X();
		    double timeSep = globAn->GetClusterTimeSeparationFromVertex(cluster[i], truthVertex);
		    double clusterTime = cluster[i].Position.T();
		    double clusterZPos = cluster[i].Position.Z();
		    double clusterYPos = cluster[i].Position.Y();
		    double clusterXPos = cluster[i].Position.X();
		    double dist = globAn->GetClusterDistanceFromTheVertex(cluster[i], truthVertex);
		    FillMichelElectronHistos(clusterTime, clusterXPos, clusterYPos, clusterZPos, vertexTime, vertexXPos, vertexYPos, vertexZPos, timeSep, dist, NVtxFGD1, NVtxFGD2);
		    FillMichelEDistanceHistos(dist, NVtxFGD1, NVtxFGD2, reacCode);

		  }
		}
					  
		// 8. michel electron cut 
		if (NMichelElectron == 0) {
		  passedMichelElectronCut = true;
		  if (debug) cout << " 8. passed Michel Electron cut " << endl;	
	  		
		  // 9. nuInvariantMassCut cut 
		  passedNuInvariantMassCut = true;
		  
		  //get neutrino energy
		  //ENuRec = globAn->GetNeutrinoEnergy(recNegTrack[0]);
		  
		}
	      }
	    }
	  }
	}
	
	
	//---------  Two tracks Analysis-----------------
	
	// 2. only two tracks
	else if (enableTwoTrackAnalysis && NRecTrack == 2) {
	  if (debug) cout << " 2. two tracks " << endl;
	  passedTracksNbrCut = true;
	  
	  double momRelError1 = 100;
	  double momRelError2 = 100;
	  if (recTrack[0].Momentum != 0){
	    momRelError1 = fabs(recTrack[0].MomentumError/recTrack[0].Momentum);
	  }
	  if (recTrack[1].Momentum != 0){
	    momRelError2 = fabs(recTrack[1].MomentumError/recTrack[1].Momentum);
	  }
	  if (momRelError1 < momRelErrorCut && momRelError2<momRelErrorCut){
	    passedCurvatureCut = true;
	    if (debug) cout << " 3. two useful TPC tracks " << endl;	      
	    
	    // 4. negative track
	    if (NNegReconTrack == 1) {
	      passedNegTpcCut = true;
	      if (debug) cout << " 4. one negative track " << endl;	    
	      
	      // 5. not a shower in the ECal
	      if (globAn->IsNotAShower(recNegTrack[0])) {
		passedShowerCut = true;
		if (debug) cout << " 5. not a shower " << endl;
		
		// 6. one positive track 
		if (NPosReconTrack == 1) {
		  passedPosTpcCut = true;
		  if (debug) cout << " 6. one positive track (we are in 2 tracks case !!!) " << endl;		  
		  
		  //get deltaThetaP
		  deltaThetaP = globAn->GetDeltaThetaP(recPosTrack[0], recNegTrack[0]);
		  
		  //get nu invariant mass		
		  nuInvariantMass = globAn->GetNuInvariantMass(recPosTrack[0], recNegTrack[0]);

		  //get neutrino energy
		  //ENuRec = globAn->GetNeutrinoEnergy2Tracks(recPosTrack[0],recNegTrack[0]);

		  // 7. deltaThetaP cut 
		  if (deltaThetaP < 0.6) {
		    passedDeltaThetaPCut = true;
		    if (debug) cout << " 7. passed DeltaThetaP cut " << endl;		  
		    

		    if (NMichelElectron>0) {
		      for (int i=0; i<NMichelElectron; i++) {
			double clusterTime = cluster[i].Position.T();
			double diff_1 = clusterTime - trackVertexTime;
			FillClusterTimeHistos(clusterTime, trackVertexTime, diff_1);
			
			double timeDisp = cluster[i].TimeDisp;
			double spatialDisp = cluster[i].SpatialDisp;
			double nbrOfHits = cluster[i].NHits;
			FillDispersionHistos(spatialDisp,timeDisp);
			FillNbrOfHitsHisto(nbrOfHits);
			
			double vertexTime = truthVertex.T();
			double vertexZPos = truthVertex.Z();
			double vertexYPos = truthVertex.Y();
			double vertexXPos = truthVertex.X();
			double timeSep = globAn->GetClusterTimeSeparationFromVertex(cluster[i], truthVertex);
			double clusterTime = cluster[i].Position.T();
			double clusterZPos = cluster[i].Position.Z();
			double clusterYPos = cluster[i].Position.Y();
			double clusterXPos = cluster[i].Position.X();
			double dist = globAn->GetClusterDistanceFromTheVertex(cluster[i], truthVertex);
			FillMichelElectronHistos(clusterTime, clusterXPos, clusterYPos, clusterZPos, vertexTime, vertexXPos, vertexYPos, vertexZPos, timeSep, dist, NVtxFGD1, NVtxFGD2);
			FillMichelEDistanceHistos(dist, NVtxFGD1, NVtxFGD2, reacCode);
			
		      }
		    }
		    		    		    		    
		    
		    // 8. michel electron cut 
		    if (NMichelElectron == 0) {
		      passedMichelElectronCut = true;
		      if (debug) cout << " 8. passed Michel Electron cut " << endl;		  		      	        
		      
		      // 9. nuInvariantMassCut cut 
		      if (nuInvariantMass > -0.4) {
			passedNuInvariantMassCut = true;	      
			if (debug) cout << " 9. passed nuInvMass cut " << endl;		  
			
			//get neutrino energy
			//ENuRec = globAn->GetNeutrinoEnergy2Tracks(recPosTrack[0],recNegTrack[0]);
			
		      }      
		    }
		  }
		}
	      }
	    }
	  }
	}
      }
    }
 
   
    
    //________________________________
    //Fill histos

    //plotAn->FillDeltaThetaPHistos(deltaThetaP);
    if (NPosReconTrack == 1 && passedPosTpcCut) {
      FillDeltaThetaPHistos(deltaThetaP, reacCode);
    }
    if (NPosReconTrack == 1 && passedNuInvariantMassCut) {
      FillDeltaThetaPHistos_afterNuInvMassCut(deltaThetaP, reacCode);
    }
    if (NPosReconTrack == 1 && passedDeltaThetaPCut) {
      FillNuInvariantMassHistos(nuInvariantMass, reacCode);
    }

    

    // ---------------
    // Update counters
    
    if (isInFiducialVolume && isCCQE) total_ccqe_fiducial++;   //true ccqe events
    
    // loop over selection cuts
    for (int cc=0; cc<9; cc++) {
      
      if ( (cc==0 && passedVertexCut) ||
	   (cc==1 && passedTracksNbrCut) ||
	   (cc==2 && passedCurvatureCut) ||
	   (cc==3 && passedNegTpcCut) ||
	   (cc==4 && passedShowerCut) ||
	   (cc==5 && passedPosTpcCut) ||
	   (cc==6 && passedDeltaThetaPCut) ||
	   (cc==7 && passedMichelElectronCut) ||
	   (cc==8 && passedNuInvariantMassCut)) {
		
	total_selected[cc]++;                            //ccqe candidates
	if (isCCQE) total_selected_ccqe_fiducial[cc]++;  //true ccqe candidates
      }
    }
    
    
    //________________________________________
    // Update counters for two tracks analysis
    
    if (isInFiducialVolume && isCCQE && NRecTrack == 2) total_ccqe_fiducial_2tracks++;  //true ccqe events
    
    // loop over selection cuts
    for (int nn=0; nn<8; nn++){
      
      //two tracks analysis
      if (NRecTrack == 2) {  
	
	if( (nn==0 && passedVertexCut) ||
	    //(nn==1 && passedTracksNbrCut && passedCurvatureCut) ||
	    (nn==1 && passedCurvatureCut) ||
	    (nn==2 && passedNegTpcCut) ||
	    (nn==3 && passedShowerCut) ||
	    (nn==4 && passedPosTpcCut) ||
	    (nn==5 && passedDeltaThetaPCut) ||
	    (nn==6 && passedMichelElectronCut) ||
	    (nn==7 && passedNuInvariantMassCut)) {
	  
	  total_selected_2tracks[nn]++;                  //ccqe candidates
	  if(isCCQE) total_selected_ccqe_fiducial_2tracks[nn]++;  //true ccqe candidates
	}
      }
    }
    
    
  }  // end of event loop
  
  
  // final time
  timer.Stop();
  
  std::cout << "Processing time: " << timer.CpuTime() << " " << timer.RealTime() << std::endl;       
  
  
  
  //____________________________
  // Plot histograms and graphs.
  
  //create a root file where histograms and plots can be saved
  TFile *myfile = new TFile("effPurHistos.root", "RECREATE");
  

  TGraph *eff = new TGraph(); 
  TGraph *pur = new TGraph();
  
  for (int i=0; i<9; i++) {
    std::cout << "Summary: " << total_selected[i] << " selected, " << total_ccqe_fiducial
	      << " CCQE in fiducial volume, " << total_selected_ccqe_fiducial[i] << " selected in fiducial" << std::endl;
    if(total_selected[i]> 0){
      std::cout << "Purity:     "  << ((double)total_selected_ccqe_fiducial[i])/((double)total_selected[i])
		<<std::endl;
      pur->SetPoint(i,i+1,((double)total_selected_ccqe_fiducial[i])/((double)total_selected[i]) *100.0);
    }
    if(total_ccqe_fiducial > 0){
      
      std::cout << i << " --> " << total_selected_ccqe_fiducial[i] << std::endl;
      std::cout << "Efficiency: " << ((double)total_selected_ccqe_fiducial[i])/((double)total_ccqe_fiducial)
		<<std::endl;
      eff->SetPoint(i,i+1,((double)total_selected_ccqe_fiducial[i])/((double)total_ccqe_fiducial)*100.0);
    }
  }
  
  TCanvas *c = new TCanvas("C");
  c->SetFillColor(10);
  
  eff->Draw("APL*");
  eff->GetXaxis()->SetLabelSize(0);
  eff->SetMarkerStyle(22);
  eff->SetMarkerColor(4);
  eff->SetLineColor(4);
  eff->SetMarkerSize(eff->GetMarkerSize()*1.8);
  eff->GetYaxis()->SetTitle("Percentage");
  eff->SetTitle("CCQE Efficiency and Purity");
  eff->SetMaximum(110);
  eff->SetMinimum(0);
  
  eff->GetXaxis()->SetNdivisions(9);

  pur->Draw("*L");
  pur->SetMarkerStyle(21);
  pur->SetMarkerColor(2);
  pur->SetLineColor(2);
  pur->SetMarkerSize(pur->GetMarkerSize()*1.8);
  
  TLatex t;
  t.SetTextSize(t.GetTextSize()*0.4);
  t.DrawText(1.0,-7,"Vertex");
  t.DrawText(1.0,-9.5,"in FV");

  t.DrawText(2.0,-7,"1 or 2");
  t.DrawText(2.0,-9.5," TPC");
  t.DrawText(2.0,-11.5,"tracks");

  t.DrawLatex(3.0,-7,"#Delta p/p<0.2");

  t.DrawText(4.0,-7,"1 Neg");
  t.DrawText(4.0,-9.5,"  TPC");
  t.DrawText(4.0,-11.5,"Track");

  t.DrawText(5.0,-7," not a");
  t.DrawText(5.0,-9.5,"shower");

  t.DrawText(6.0,-7," 1 Pos");
  t.DrawText(6.0,-9.5,"  TPC");
  t.DrawText(6.0,-11.5," Track");

  t.DrawLatex(7.0,-7,"#Delta#theta_{p}<0.6");

  t.DrawText(8.0,-7,"   NO");
  t.DrawText(8.0,-9.5," Michel");
  t.DrawText(8.0,-11.5,"electrons");

  t.DrawLatex(9.0,-7,"m_{#nu}^{2}>-0.4");
  //t.DrawLatex(6.0,-9.5,"   GeV^{2}");


  TLegend *leg = new TLegend(0.3,0.25,0.12,0.12);
  leg->SetFillColor(10);
  leg->AddEntry(eff,"Efficiency (%)","p");
  leg->AddEntry(pur,"Purity (%)","p");
  leg->Draw("SAME");
  c->Print("eff-pur.png");

  c->Update();
  
  //set width and position of the stats box
  gStyle->SetStatX(.997);
  gStyle->SetStatW(0.10);
  

  //-----------------------------------------------------------------

  //only two tracks analysis
  TGraph *eff_2tracks = new TGraph(); 
  TGraph *pur_2tracks = new TGraph();

  
  for (int i=0; i<8; i++) {
    std::cout << "Summary_2tracks: " << total_selected_2tracks[i] << " selected, " << total_ccqe_fiducial_2tracks
	      << " CCQE in fiducial volume, " << total_selected_ccqe_fiducial_2tracks[i] << " selected in fiducial" << std::endl;
    if(total_selected_2tracks[i]> 0){
      std::cout << "Purity:     "  << ((double)total_selected_ccqe_fiducial_2tracks[i])/((double)total_selected_2tracks[i])
		<<std::endl;
      pur_2tracks->SetPoint(i,i+1,((double)total_selected_ccqe_fiducial_2tracks[i])/((double)total_selected_2tracks[i]) *100.0);
    }
    if(total_ccqe_fiducial_2tracks > 0){
      
      std::cout << i << " --> " << total_selected_ccqe_fiducial_2tracks[i] << std::endl;
      std::cout << "Efficiency: " << ((double)total_selected_ccqe_fiducial_2tracks[i])/((double)total_ccqe_fiducial_2tracks)
		<<std::endl;
      eff_2tracks->SetPoint(i,i+1,((double)total_selected_ccqe_fiducial_2tracks[i])/((double)total_ccqe_fiducial_2tracks)*100.0);
    }
  }
  
  TCanvas *c_2tracks = new TCanvas("C_2tracks");
  c_2tracks->SetFillColor(10);
  
  eff_2tracks->Draw("APL*");
  eff_2tracks->GetXaxis()->SetLabelSize(0);
  eff_2tracks->SetMarkerStyle(22);
  eff_2tracks->SetMarkerColor(4);
  eff_2tracks->SetLineColor(4);
  eff_2tracks->SetMarkerSize(eff_2tracks->GetMarkerSize()*1.8);
  eff_2tracks->GetYaxis()->SetTitle("Percentage");
  eff_2tracks->SetTitle("CCQE Efficiency and Purity");
  eff_2tracks->SetMaximum(110);
  eff_2tracks->SetMinimum(0);
  
  eff_2tracks->GetXaxis()->SetNdivisions(8);

  pur_2tracks->Draw("*L");
  pur_2tracks->SetMarkerStyle(21);
  pur_2tracks->SetMarkerColor(2);
  pur_2tracks->SetLineColor(2);
  pur_2tracks->SetMarkerSize(pur_2tracks->GetMarkerSize()*1.8);

  TLatex t_2tracks;
  t_2tracks.SetTextSize(t_2tracks.GetTextSize()*0.4);

  t_2tracks.DrawText(1.0,-7,"Vertex");
  t_2tracks.DrawText(1.0,-9.5,"in FV");

  t_2tracks.DrawLatex(2.0,-7,"#Delta p/p < 0.2");

  t_2tracks.DrawText(3.0,-7,"1 Neg");
  t_2tracks.DrawText(3.0,-9.5,"  TPC");
  t_2tracks.DrawText(3.0,-11.5,"Track");

  t_2tracks.DrawText(4.0,-7," not a");
  t_2tracks.DrawText(4.0,-9.5,"shower");

  t_2tracks.DrawText(5.0,-7," 1 Pos");
  t_2tracks.DrawText(5.0,-9.5,"  TPC");
  t_2tracks.DrawText(5.0,-11.5," Track");

  t_2tracks.DrawLatex(6.0,-7,"#Delta#theta_{p}<0.6");

  t_2tracks.DrawText(7.0,-7,"   NO");
  t_2tracks.DrawText(7.0,-9.5," Michel");
  t_2tracks.DrawText(7.0,-11.5,"electrons");

  t_2tracks.DrawLatex(8.0,-7,"m_{#nu}^{2}>-0.4");
  //t_2tracks.DrawLatex(5.0,-9.5,"   GeV^{2}");


  TLegend *leg_2tracks = new TLegend(0.3,0.25,0.12,0.12);
  leg_2tracks->SetFillColor(10);
  leg_2tracks->AddEntry(eff_2tracks,"Efficiency (%)","p");
  leg_2tracks->AddEntry(pur_2tracks,"Purity (%)","p");
  leg_2tracks->Draw("SAME");
  c_2tracks->Print("eff-pur_2tracks.png");
  c_2tracks->Update();


  DrawHistos();



  //close root file where histograms and plots have been saved
  myfile->Close();    


  return; 

}





//----------------------------------------------

void InitializeHistos() {
  trueCCQEcand_deltaThetaP = new TH1F("trueCCQEcand_deltaThetaP","deltaThetaP",20,-1,5);
  trueDIScand_deltaThetaP = new TH1F("trueDIScand_deltaThetaP","deltaThetaP",20,-1,5);
  trueNCcand_deltaThetaP = new TH1F("trueNCcand_deltaThetaP","deltaThetaP",20,-1,5);
  trueCC1PIcand_deltaThetaP = new TH1F("trueCC1PIcand_deltaThetaP","deltaThetaP",20,-1,5);

  trueCCQEcand_nuInvariantMass = new TH1F("trueCCQEcand_nuInvariantMass","nuInvariantMass",20,-1,0.5);
  trueDIScand_nuInvariantMass = new TH1F("trueDIScand_nuInvariantMass","nuInvariantMass",20,-1,0.5);
  trueNCcand_nuInvariantMass = new TH1F("trueNCcand_nuInvariantMass","nuInvariantMass",20,-1,0.5);
  trueCC1PIcand_nuInvariantMass = new TH1F("trueCC1PIcand_nuInvariantMass","nuInvariantMass",20,-1,0.5);

  trueCCQEcand_deltaThetaP_afterNuInvMassCut = new TH1F("trueCCQEcand_deltaThetaP_afterNuInvMassCut","deltaThetaP",20,-0.5,1.5);
  trueDIScand_deltaThetaP_afterNuInvMassCut = new TH1F("trueDIScand_deltaThetaP_afterNuInvMassCut","deltaThetaP",20,-0.5,1.5);
  trueNCcand_deltaThetaP_afterNuInvMassCut = new TH1F("trueNCcand_deltaThetaP_afterNuInvMassCut","deltaThetaP",20,-0.5,1.5);
  trueCC1PIcand_deltaThetaP_afterNuInvMassCut = new TH1F("trueCC1PIcand_deltaThetaP_afterNuInvMassCut","deltaThetaP",20,-0.5,1.5);

  michelClusterTime = new TH1F("michelClusterTime","time",50,0,2000);
  //michelClusterTime = new TH1F("michelClusterTime","time",2000,0,2000);
  michelClusterXPos = new TH1F("michelClusterXPos","x pos",50,-1000,1000);
  michelClusterYPos = new TH1F("michelClusterYPos","y pos",50,-1000,1000);
  michelClusterZPos = new TH1F("michelClusterZPos","z pos",50,0,2000);
  //vertexTime = new TH1F("vertexTime","time",50,0,2000);
  vertexTime = new TH1F("vertexTime","time",20,0,150);
  vertexXPos = new TH1F("vertexXPos","x pos",50,-1000,1000);
  vertexYPos = new TH1F("vertexYPos","y pos",50,-1000,1000);
  vertexZPos = new TH1F("vertexZPos","z pos",50,0,2000);
  timeDiff = new TH1F("timeDiff","time diff",30,0,150);
  distance = new TH1F("distance","distance",50,0,2000);

  timeDiff_FGD1 = new TH1F("timeDiff_FGD1","time diff",50,0,2000);
  distance_FGD1 = new TH1F("distance_FGD1","distance",50,0,2000);
  timeDiff_FGD2 = new TH1F("timeDiff_FGD2","time diff",50,0,2000);
  distance_FGD2 = new TH1F("distance_FGD2","distance",50,0,2000);


  distance_CCQE = new TH1F("distance_CCQE","distance",50,0,2000);
  distance_FGD1_CCQE = new TH1F("distance_FGD1_CCQE","distance",50,0,2000);
  distance_FGD2_CCQE = new TH1F("distance_FGD2_CCQE","distance",50,0,2000);

  distance_notCCQE = new TH1F("distance_notCCQE","distance",50,0,2000);
  distance_FGD1_notCCQE = new TH1F("distance_FGD1_notCCQE","distance",50,0,2000);
  distance_FGD2_notCCQE = new TH1F("distance_FGD2_notCCQE","distance",50,0,2000);


  trackVtx = new TH1F("trackVtx","time",50,0,160);
  vtxTime = new TH1F("vtxTime","time",50,0,160);
  timeDiff_1 = new TH1F("timeDiff_1","time",80,-130,30);
  trackVtx_notZeroTrackTime = new TH1F("trackVtx_notZeroTrackTime","time",50,20,140);
  vtxTime_notZeroTrackTime = new TH1F("vtxTime_notZeroTrackTime","time",50,0,160);
  timeDiff_notZeroTrackTime = new TH1F("timeDiff_notZeroTrackTime","time",50,-130,4500);
  timeDiff_notZeroTrackTime_ZOOM = new TH1F("timeDiff_notZeroTrackTime","time",50,-50,50);

  unusedHitsInFGD1 = new TH1F("unusedHitsInFGD1","nbr if hits",50,0,200);
  unusedHitsInFGD2 = new TH1F("unusedHitsInFGD2","nbr if hits",50,0,200);
  clusterT = new TH1F("clusterT","time",50,0,13000);
  trackVtx_cluster = new TH1F("trackVtx_cluster","time",50,0,250);
  trackVtx_cluster_notZeroTrackTime = new TH1F("trackVtx_cluster_notZeroTrackTime","time",50,0,250);
  timeDiff_cluster = new TH1F("timeDiff_cluster","time",50,-4500,5000);
  timeDiff_cluster_notZeroTrackTime = new TH1F("timeDiff_cluster_notZeroTrackTime","time",50,-4500,5000);
  timeDispersion = new TH1F("timeDispersion","time",40,0,200);
  spatialDispersion = new TH1F("spatialDispersion","length",30,0,400);
  dispersion = new TH2F("dispersion","length VS time",20,0,400,10,0,200);
  NbrOfHitsPerCluster = new TH1F("NbrOfHitsPerCluster","nbr if hits",20,0,30);
  
}


void FillDeltaThetaPHistos(double deltaThetaP, int reacCode) {  
  if (reacCode == 1) trueCCQEcand_deltaThetaP->Fill(deltaThetaP);                 //CCQE
  if (reacCode == 21 ||  26) trueDIScand_deltaThetaP->Fill(deltaThetaP);          //DIS
  if (reacCode >= 31) trueNCcand_deltaThetaP->Fill(deltaThetaP);                  //NC
  if (reacCode == 11 || 12 || 13) trueCC1PIcand_deltaThetaP->Fill(deltaThetaP);   //CC1pi
}

void FillDeltaThetaPHistos_afterNuInvMassCut(double deltaThetaP, int reacCode) {
  if (reacCode == 1) trueCCQEcand_deltaThetaP_afterNuInvMassCut->Fill(deltaThetaP);                 //CCQE
  if (reacCode == 21 ||  26) trueDIScand_deltaThetaP_afterNuInvMassCut->Fill(deltaThetaP);          //DIS
  if (reacCode >= 31) trueNCcand_deltaThetaP_afterNuInvMassCut->Fill(deltaThetaP);                  //NC
  if (reacCode == 11 || 12 || 13) trueCC1PIcand_deltaThetaP_afterNuInvMassCut->Fill(deltaThetaP);   //CC1pi
}

void FillNuInvariantMassHistos(double nuInvMass, int reacCode) {  
  if (reacCode == 1) trueCCQEcand_nuInvariantMass->Fill(nuInvMass);                 //CCQE
  if (reacCode == 21 ||  26) trueDIScand_nuInvariantMass->Fill(nuInvMass);          //DIS
  if (reacCode >= 31) trueNCcand_nuInvariantMass->Fill(nuInvMass);                  //NC
  if (reacCode == 11 || 12 || 13) trueCC1PIcand_nuInvariantMass->Fill(nuInvMass);   //CC1pi  
}

void FillMichelElectronHistos(double clusterTime, double clusterXPos, double clusterYPos, double clusterZPos, double vtxTime, double vtxXPos, double vtxYPos, double vtxZPos, double time, double dist, int NVtxFGD1, int NVtxFGD2) {
  michelClusterTime->Fill(clusterTime);
  if (clusterXPos!=0) michelClusterXPos->Fill(clusterXPos);
  if (clusterYPos!=0) michelClusterYPos->Fill(clusterYPos);
  if (clusterZPos!=0) michelClusterZPos->Fill(clusterZPos);
  vertexTime->Fill(vtxTime);
  if (vtxXPos!=0) vertexXPos->Fill(vtxXPos);
  if (vtxYPos!=0) vertexYPos->Fill(vtxYPos);
  if (vtxZPos!=0) vertexZPos->Fill(vtxZPos);
  timeDiff->Fill(time);
  distance->Fill(dist);
  if (NVtxFGD1>0) {
    timeDiff_FGD1->Fill(time);
    distance_FGD1->Fill(dist);
  }
  if (NVtxFGD2>0) {
    timeDiff_FGD2->Fill(time);
    distance_FGD2->Fill(dist);
  }
}


void FillMichelEDistanceHistos(double dist, int NVtxFGD1, int NVtxFGD2, int reacCode) {
  if (reacCode==1) distance_CCQE->Fill(dist);
  else if (reacCode!=1) distance_notCCQE->Fill(dist);
  if (NVtxFGD1>0) {
    if (reacCode==1) distance_FGD1_CCQE->Fill(dist);
    else if (reacCode!=1) distance_FGD1_notCCQE->Fill(dist);
  }
  if (NVtxFGD2>0) {
    if (reacCode==1) distance_FGD2_CCQE->Fill(dist);
    else if (reacCode!=1) distance_FGD2_notCCQE->Fill(dist);
  }
}


void FillVertexTimeHistos(double trackTime, double vertexTime, double tDiff) {
  trackVtx->Fill(trackTime);
  vtxTime->Fill(vertexTime);
  timeDiff_1->Fill(tDiff);
  if (trackTime!=0 && trackTime!=999) {
    trackVtx_notZeroTrackTime->Fill(trackTime);
    vtxTime_notZeroTrackTime->Fill(vertexTime);
    timeDiff_notZeroTrackTime->Fill(tDiff);
  }
  if (trackTime!=0 && trackTime!=999 && tDiff<50) {
    timeDiff_notZeroTrackTime_ZOOM->Fill(tDiff);
  }
}


void FillUnusedFGDHits(int unusedFGD1, int unusedFGD2) {
  if (unusedFGD1!=0) unusedHitsInFGD1->Fill(unusedFGD1);
  if (unusedFGD2!=0) unusedHitsInFGD2->Fill(unusedFGD2);
}

void FillClusterTimeHistos(double clstrTime, double trackTime, double tDiff) {
  clusterT->Fill(clstrTime);
  trackVtx_cluster->Fill(trackTime);
  timeDiff_cluster->Fill(tDiff);
  if (trackTime!=0 && trackTime!=999) {
    trackVtx_cluster_notZeroTrackTime->Fill(trackTime);
    timeDiff_cluster_notZeroTrackTime->Fill(tDiff);
  }
}

void FillDispersionHistos(double sDisp, double tDisp) {
  timeDispersion->Fill(tDisp);
  spatialDispersion->Fill(sDisp);
  dispersion->Fill(sDisp,tDisp);
}

void FillNbrOfHitsHisto(double nbrHits) {
  NbrOfHitsPerCluster->Fill(nbrHits);
}



void DrawHistos() {
  //create a root file where histograms and plots can be saved
  TFile *myfile = new TFile("globalHistos.root", "RECREATE");

  //__________________________
 
  //draw histograms
  TCanvas *c1 = new TCanvas("C1","C1",400,0,1000,800);

  trueCC1PIcand_deltaThetaP->SetLineWidth(2);
  trueCC1PIcand_deltaThetaP->SetLineColor(6);
  trueCC1PIcand_deltaThetaP->Draw();

  trueCCQEcand_deltaThetaP->SetLineWidth(2);
  trueCCQEcand_deltaThetaP->SetLineColor(2);
  trueCCQEcand_deltaThetaP->Draw("SAME");

  trueDIScand_deltaThetaP->SetLineWidth(2);
  trueDIScand_deltaThetaP->SetLineColor(3);
  trueDIScand_deltaThetaP->Draw("SAME");

  trueNCcand_deltaThetaP->SetLineWidth(2);
  trueNCcand_deltaThetaP->SetLineColor(4);
  trueNCcand_deltaThetaP->Draw("SAME");
  TLegend *leg1 = new TLegend(0.6,0.68,0.89,0.78);
  leg1->SetFillColor(10);
  leg1->AddEntry(trueCCQEcand_deltaThetaP, "true CCQE cand");
  leg1->AddEntry(trueDIScand_deltaThetaP, "true DIS cand");
  leg1->AddEntry(trueNCcand_deltaThetaP, "true NC cand");
  leg1->AddEntry(trueCC1PIcand_deltaThetaP, "true CC1pi cand");
  leg1->Draw("SAME");

  //print histogram into file
  c1->Print("deltaThetaP.png");


  TCanvas *c2 = new TCanvas("C2","C2",400,0,1000,800);

  trueCC1PIcand_nuInvariantMass->SetLineWidth(2);
  trueCC1PIcand_nuInvariantMass->SetLineColor(6);
  trueCC1PIcand_nuInvariantMass->Draw();

  trueCCQEcand_nuInvariantMass->SetLineWidth(2);
  trueCCQEcand_nuInvariantMass->SetLineColor(2);
  trueCCQEcand_nuInvariantMass->Draw("SAME");

  trueDIScand_nuInvariantMass->SetLineWidth(2);
  trueDIScand_nuInvariantMass->SetLineColor(3);
  trueDIScand_nuInvariantMass->Draw("SAME");

  trueNCcand_nuInvariantMass->SetLineWidth(2);
  trueNCcand_nuInvariantMass->SetLineColor(4);
  trueNCcand_nuInvariantMass->Draw("SAME");

  TLegend *leg2 = new TLegend(0.6,0.68,0.89,0.78);
  leg2->SetFillColor(10);
  leg2->AddEntry(trueCCQEcand_nuInvariantMass, "true CCQE cand");
  leg2->AddEntry(trueDIScand_nuInvariantMass, "true DIS cand");
  leg2->AddEntry(trueNCcand_nuInvariantMass, "true NC cand");
  leg2->AddEntry(trueCC1PIcand_nuInvariantMass, "true CC1pi cand");
  leg2->Draw("SAME");

  //print histogram into file
  c2->Print("nuInvariantMass.png");


  TCanvas *c3 = new TCanvas("C3","C3",400,0,1000,800);

  trueCC1PIcand_deltaThetaP_afterNuInvMassCut->SetLineWidth(2);
  trueCC1PIcand_deltaThetaP_afterNuInvMassCut->SetLineColor(6);
  trueCC1PIcand_deltaThetaP_afterNuInvMassCut->Draw();

  trueCCQEcand_deltaThetaP_afterNuInvMassCut->SetLineWidth(2);
  trueCCQEcand_deltaThetaP_afterNuInvMassCut->SetLineColor(2);
  trueCCQEcand_deltaThetaP_afterNuInvMassCut->Draw("SAME");

  trueDIScand_deltaThetaP_afterNuInvMassCut->SetLineWidth(2);
  trueDIScand_deltaThetaP_afterNuInvMassCut->SetLineColor(3);
  trueDIScand_deltaThetaP_afterNuInvMassCut->Draw("SAME");

  trueNCcand_deltaThetaP_afterNuInvMassCut->SetLineWidth(2);
  trueNCcand_deltaThetaP_afterNuInvMassCut->SetLineColor(4);
  trueNCcand_deltaThetaP_afterNuInvMassCut->Draw("SAME");

  TLegend *leg3 = new TLegend(0.6,0.68,0.89,0.78);
  leg3->SetFillColor(10);
  leg3->AddEntry(trueCCQEcand_deltaThetaP_afterNuInvMassCut, "true CCQE cand");
  leg3->AddEntry(trueDIScand_deltaThetaP_afterNuInvMassCut, "true DIS cand");
  leg3->AddEntry(trueNCcand_deltaThetaP_afterNuInvMassCut, "true NC cand");
  leg3->AddEntry(trueCC1PIcand_deltaThetaP_afterNuInvMassCut, "true CC1pi cand");
  leg3->Draw("SAME");

  //print histogram into file
  c3->Print("deltaThetaP_afterNuInvMassCut.png");


  TCanvas *c4 = new TCanvas("C4","C4",400,0,1000,800);
  michelClusterTime->SetLineWidth(2);
  michelClusterTime->Draw();
  //print histogram into file
  c4->Print("michelClusterTime.png");

  TCanvas *c5 = new TCanvas("C5","C5",400,0,1000,800);
  michelClusterXPos->SetLineWidth(2);
  michelClusterXPos->Draw();
  //print histogram into file
  c5->Print("michelClusterXPos.png");

  TCanvas *c6 = new TCanvas("C6","C6",400,0,1000,800);
  michelClusterYPos->SetLineWidth(2);
  michelClusterYPos->Draw();
  //print histogram into file
  c6->Print("michelClusterYPos.png");

  TCanvas *c7 = new TCanvas("C7","C7",400,0,1000,800);
  michelClusterZPos->SetLineWidth(2);
  michelClusterZPos->Draw();
  //print histogram into file
  c7->Print("michelClusterZPos.png");

  TCanvas *c8 = new TCanvas("C8","C8",400,0,1000,800);
  vertexTime->SetLineWidth(2);
  vertexTime->Draw();
  //print histogram into file
  c8->Print("vertexTime.png");

  TCanvas *c9 = new TCanvas("C9","C9",400,0,1000,800);
  vertexXPos->SetLineWidth(2);
  vertexXPos->Draw();
  //print histogram into file
  c9->Print("vertexXPos.png");

  TCanvas *c10 = new TCanvas("C10","C10",400,0,1000,800);
  vertexYPos->SetLineWidth(2);
  vertexYPos->Draw();
  //print histogram into file
  c10->Print("vertexYPos.png");

  TCanvas *c11 = new TCanvas("C11","C11",400,0,1000,800);
  vertexZPos->SetLineWidth(2);
  vertexZPos->Draw();
  //print histogram into file
  c11->Print("vertexZPos.png");

  TCanvas *c12 = new TCanvas("C12","C12",400,0,1000,800);
  timeDiff->SetLineWidth(2);
  timeDiff->Draw();
  //print histogram into file
  c12->Print("timeDiff.png");

  TCanvas *c13 = new TCanvas("C13","C13",400,0,1000,800);
  distance->SetLineWidth(2);
  distance->Draw();
  //print histogram into file
  c13->Print("distance.png");

  TCanvas *c14 = new TCanvas("C14","C14",400,0,1000,800);
  timeDiff_FGD1->SetLineWidth(2);
  timeDiff_FGD1->Draw();
  //print histogram into file
  c14->Print("timeDiff_FGD1.png");

  TCanvas *c15 = new TCanvas("C15","C15",400,0,1000,800);
  distance_FGD1->SetLineWidth(2);
  distance_FGD1->Draw();
  //print histogram into file
  c15->Print("distance_FGD1.png");

  TCanvas *c16 = new TCanvas("C16","C16",400,0,1000,800);
  timeDiff_FGD2->SetLineWidth(2);
  timeDiff_FGD2->Draw();
  //print histogram into file
  c16->Print("timeDiff_FGD2.png");

  TCanvas *c17 = new TCanvas("C17","C17",400,0,1000,800);
  distance_FGD2->SetLineWidth(2);
  distance_FGD2->Draw();
  //print histogram into file
  c17->Print("distance_FGD2.png");



  TCanvas *c18 = new TCanvas("C18","C18",400,0,1000,800);
  //distance_CCQE->SetLineWidth(2);
  //distance_CCQE->Draw();
  distance_notCCQE->SetLineWidth(2);
  distance_notCCQE->SetLineColor(2);
  distance_notCCQE->Draw();
  //distance_notCCQE->Draw("SAME");
  distance_CCQE->SetLineWidth(2);
  distance_CCQE->Draw("SAME");
  TLegend *leg18 = new TLegend(0.6,0.68,0.89,0.78);
  leg18->SetFillColor(10);
  leg18->AddEntry(distance_CCQE, "true CCQE cand");
  leg18->AddEntry(distance_notCCQE, "true not CCQE cand");
  leg18->Draw("SAME");
  c18->Print("distance_CCQE_notCCQE.png");

  TCanvas *c18_1 = new TCanvas("C18_1","C18_1",400,0,1000,800);
  distance_CCQE->SetLineWidth(2);
  distance_CCQE->Draw();
  c18_1->Print("distance_CCQE.png");

  TCanvas *c18_2 = new TCanvas("C18_2","C18_2",400,0,1000,800);
  distance_notCCQE->SetLineWidth(2);
  distance_notCCQE->SetLineColor(2);
  distance_notCCQE->Draw();
  c18_2->Print("distance_notCCQE.png");


  TCanvas *c19 = new TCanvas("C19","C19",400,0,1000,800);
  //distance_FGD1_CCQE->SetLineWidth(2);
  //distance_FGD1_CCQE->Draw();
  distance_FGD1_notCCQE->SetLineWidth(2);
  distance_FGD1_notCCQE->SetLineColor(2);
  distance_FGD1_notCCQE->Draw();
  //distance_FGD1_notCCQE->Draw("SAME");
  distance_FGD1_CCQE->SetLineWidth(2);
  distance_FGD1_CCQE->Draw("SAME");
  TLegend *leg19 = new TLegend(0.6,0.68,0.89,0.78);
  leg19->SetFillColor(10);
  leg19->AddEntry(distance_FGD1_CCQE, "true CCQE cand");
  leg19->AddEntry(distance_FGD1_notCCQE, "true not CCQE cand");
  leg19->Draw("SAME");
  c19->Print("distanceFGD1_CCQE_notCCQE.png");

  TCanvas *c19_1 = new TCanvas("C19_1","C19_1",400,0,1000,800);
  distance_FGD1_CCQE->SetLineWidth(2);
  distance_FGD1_CCQE->Draw();
  c19_1->Print("distanceFGD1_CCQE.png");

  TCanvas *c19_2 = new TCanvas("C19_2","C19_2",400,0,1000,800);
  distance_FGD1_notCCQE->SetLineWidth(2);
  distance_FGD1_notCCQE->SetLineColor(2);
  distance_FGD1_notCCQE->Draw();
  c19_2->Print("distanceFGD1_notCCQE.png");


  TCanvas *c20 = new TCanvas("C20","C20",400,0,1000,800);
  //distance_FGD2_CCQE->SetLineWidth(2);
  //distance_FGD2_CCQE->Draw();
  distance_FGD2_notCCQE->SetLineWidth(2);
  distance_FGD2_notCCQE->SetLineColor(2);
  distance_FGD2_notCCQE->Draw();
  //distance_FGD2_notCCQE->Draw("SAME");
  distance_FGD2_CCQE->SetLineWidth(2);
  distance_FGD2_CCQE->Draw("SAME");
  TLegend *leg20 = new TLegend(0.6,0.68,0.89,0.78);
  leg20->SetFillColor(10);
  leg20->AddEntry(distance_FGD2_CCQE, "true CCQE cand");
  leg20->AddEntry(distance_FGD2_notCCQE, "true not CCQE cand");
  leg20->Draw("SAME");
  c20->Print("distanceFGD2_CCQE_notCCQE.png");

  TCanvas *c20_1 = new TCanvas("C20_1","C20_1",400,0,1000,800);
  distance_FGD2_CCQE->SetLineWidth(2);
  distance_FGD2_CCQE->Draw();
  c20_1->Print("distanceFGD2_CCQE.png");

  TCanvas *c20_2 = new TCanvas("C20_2","C20_2",400,0,1000,800);
  distance_FGD2_notCCQE->SetLineWidth(2);
  distance_FGD2_notCCQE->SetLineColor(2);
  distance_FGD2_notCCQE->Draw();
  c20_2->Print("distanceFGD2_notCCQE.png");





  TCanvas *c21 = new TCanvas("C21","C21",400,0,1000,800);
  trackVtx->SetLineWidth(2);
  trackVtx->Draw();
  c21->Print("trackVtx.png");

  TCanvas *c22 = new TCanvas("C22","C22",400,0,1000,800);
  vtxTime->SetLineWidth(2);
  vtxTime->Draw();
  c22->Print("vtxTime.png");

  TCanvas *c23 = new TCanvas("C23","C23",400,0,1000,800);
  timeDiff_1->SetLineWidth(2);
  timeDiff_1->Draw();
  c23->Print("timeDiff_1.png");

  TCanvas *c24 = new TCanvas("C24","C24",400,0,1000,800);
  trackVtx_notZeroTrackTime->SetLineWidth(2);
  trackVtx_notZeroTrackTime->Draw();
  c24->Print("trackVtx_notZeroTrackTime.png");

  TCanvas *c25 = new TCanvas("C25","C25",400,0,1000,800);
  vtxTime_notZeroTrackTime->SetLineWidth(2);
  vtxTime_notZeroTrackTime->Draw();
  c25->Print("vtxTime_notZeroTrackTime.png");

  TCanvas *c26 = new TCanvas("C26","C26",400,0,1000,800);
  timeDiff_notZeroTrackTime->SetLineWidth(2);
  timeDiff_notZeroTrackTime->Draw();
  c26->Print("timeDiff_notZeroTrackTime.png");

  TCanvas *c26_1 = new TCanvas("C26_1","C26_1",400,0,1000,800);
  timeDiff_notZeroTrackTime_ZOOM->SetLineWidth(2);
  timeDiff_notZeroTrackTime_ZOOM->Draw();
  c26_1->Print("timeDiff_notZeroTrackTime_ZOOM.png");

  TCanvas *c27 = new TCanvas("C27","C27",400,0,1000,800);
  unusedHitsInFGD1->SetLineWidth(2);
  unusedHitsInFGD1->Draw();
  c27->Print("unusedHitsInFGD1.png");

  TCanvas *c28 = new TCanvas("C28","C28",400,0,1000,800);
  unusedHitsInFGD2->SetLineWidth(2);
  unusedHitsInFGD2->Draw();
  c28->Print("unusedHitsInFGD2.png");

  TCanvas *c29 = new TCanvas("C29","C29",400,0,1000,800);
  clusterT->SetLineWidth(2);
  clusterT->Draw();
  c29->Print("clusterT.png");

  TCanvas *c30 = new TCanvas("C30","C30",400,0,1000,800);
  trackVtx_cluster->SetLineWidth(2);
  trackVtx_cluster->Draw();
  c30->Print("trackVtx_cluster.png");

  TCanvas *c30_1 = new TCanvas("C30_1","C30_1",400,0,1000,800);
  trackVtx_cluster_notZeroTrackTime->SetLineWidth(2);
  trackVtx_cluster_notZeroTrackTime->Draw();
  c30_1->Print("trackVtx_cluster_notZeroTrackTime.png");

  TCanvas *c31 = new TCanvas("C31","C31",400,0,1000,800);
  timeDiff_cluster->SetLineWidth(2);
  timeDiff_cluster->Draw();
  c31->Print("timeDiff_cluster.png");

  TCanvas *c31_1 = new TCanvas("C31_1","C31_1",400,0,1000,800);
  timeDiff_cluster_notZeroTrackTime->SetLineWidth(2);
  timeDiff_cluster_notZeroTrackTime->Draw();
  c31_1->Print("timeDiff_cluster_notZeroTrackTime.png");

  TCanvas *c32 = new TCanvas("C32","C32",400,0,1000,800);
  timeDispersion->SetLineWidth(2);
  timeDispersion->Draw();
  c32->Print("timeDispersion.png");

  TCanvas *c33 = new TCanvas("C33","C33",400,0,1000,800);
  spatialDispersion->SetLineWidth(2);
  spatialDispersion->Draw();
  c33->Print("spatialDispersion.png");

  TCanvas *c34 = new TCanvas("C34","C34",400,0,1000,800);
  dispersion->SetLineWidth(2);
  dispersion->Draw();
  c34->Print("dispersion.png");

  TCanvas *c35 = new TCanvas("C35","C35",400,0,1000,800);
  NbrOfHitsPerCluster->SetLineWidth(2);
  NbrOfHitsPerCluster->Draw();
  c35->Print("NbrOfHitsPerCluster.png");






  //__________________________
  
  //write histos into root file
  trueCCQEcand_deltaThetaP->Write();
  trueDIScand_deltaThetaP->Write();
  trueNCcand_deltaThetaP->Write();
  trueCC1PIcand_deltaThetaP->Write();

  trueCCQEcand_nuInvariantMass->Write();
  trueDIScand_nuInvariantMass->Write();
  trueNCcand_nuInvariantMass->Write();
  trueCC1PIcand_nuInvariantMass->Write();

  trueCCQEcand_deltaThetaP_afterNuInvMassCut->Write();
  trueDIScand_deltaThetaP_afterNuInvMassCut->Write();
  trueNCcand_deltaThetaP_afterNuInvMassCut->Write();
  trueCC1PIcand_deltaThetaP_afterNuInvMassCut->Write();

  michelClusterTime->Write();
  michelClusterXPos->Write();
  michelClusterYPos->Write();
  michelClusterZPos->Write();
  vertexTime->Write();
  vertexXPos->Write();
  vertexYPos->Write();
  vertexZPos->Write();
  timeDiff->Write();
  distance->Write();
  distance_FGD1->Write();
  distance_FGD2->Write();

  distance_CCQE->Write();
  distance_notCCQE->Write();
  distance_FGD1_CCQE->Write();
  distance_FGD1_notCCQE->Write();
  distance_FGD2_CCQE->Write();
  distance_FGD2_notCCQE->Write();
  
  trackVtx->Write();
  vtxTime->Write();
  timeDiff_1->Write();
  trackVtx_notZeroTrackTime->Write();
  vtxTime_notZeroTrackTime->Write();
  timeDiff_notZeroTrackTime->Write();
  timeDiff_notZeroTrackTime_ZOOM->Write();
  unusedHitsInFGD1->Write();
  unusedHitsInFGD2->Write();
  clusterT->Write();
  trackVtx_cluster->Write();
  trackVtx_cluster_notZeroTrackTime->Write();
  timeDiff_cluster_notZeroTrackTime->Write();
  timeDiff_cluster->Write();
  timeDispersion->Write();
  spatialDispersion->Write();
  dispersion->Write();
  NbrOfHitsPerCluster->Write();





  //close root file
  myfile->Close();

}
