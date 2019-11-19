#include "iostream"
#include "vector"



//______________________________________________________________________________________
// 
// This routine allows one to do a rudimentary CCQE selection using Cheating
// information stored in oaAnalysis ROOT tree.
// 
// In order to run this script, you must do the following.
//
// 1) You must have a file (or files) containing the oaAnalysis summary ROOT tree.  
//    This must be generated using OAAnalysis.exe, if you don't already 
//    have it.
// 2) Build the oaAnalysis library in the normal way.
// 3) Go to the macros directory.
// 4) Edit the file called 'cheating_ccqe_input_files.txt': this should be a list of all
//    the summary files that you want to process.
// 5) Start root and run this macro by doing:
//    .x doCheatingCcqeAnalysis.C()
// 
//    The macro will loop over all the events in your selected files and 
//    output some cheating CCQE selection efficiency and purity distributions.
//
//    A description of the selection cuts is given here:
//
//    http://ific.uv.es/~hansen/TALKS/cheatingInOaAnalysis.pdf
//
// C. Hansen   (based on coCcqeAnalysis.C by  T. Lindner)
//
void doCheatingCcqeAnalysis() {

// Load some important libraries.

  gSystem->AddIncludePath("-I$OAEVENTROOT/src");
  gSystem->AddIncludePath("-I$OAANALYSISROOT/src");
  gSystem->AddIncludePath("-I.");
  // gROOT->ProcessLine(".x rootlogon.C");
  
  //TEMPORARY SOLUTION
  gSystem->Load("libPhysics");
 
 
  gSystem->Load("libGeom");
  gSystem->Load("libTree");
  gSystem->Load("libMatrix");
  gSystem->Load("liboaEvent.so");
  gSystem->Load("liboaAnalysis.so");
  gROOT->ProcessLine(".L AnalysisTools/TTrackerAnalysisUtils.cxx");
  gROOT->ProcessLine(".L AnalysisTools/TNEUTAnalysisUtils.cxx");
  gROOT->ProcessLine(".L AnalysisTools/TCheatingAnalysisUtils.cxx");
  gROOT->ProcessLine(".L $OAEVENTROOT/src/HEPUnits.hxx ");
  gROOT->ProcessLine(".L $OAEVENTROOT/src/HEPConstants.hxx ");


  TChain mcTruthVertices = TChain("TruthDir/Vertices");
  TChain mcTruthTrajectories = TChain("TruthDir/Trajectories");

  TTrackerAnalysisUtils* tracker_utils = new TTrackerAnalysisUtils();
  TNEUTAnalysisUtils* neut_utils = new TNEUTAnalysisUtils();
  TCheatingAnalysisUtils* cheating_utils = new TCheatingAnalysisUtils();

  //______________________________________________________________________________________
  // Set constants for analysis

  // Parameters defines how many mm to trim off the active 
  // FGD volume in order to define the fiducial FGD volume.
  TVector3 fgdFiducialTrim = TVector3(200.0,200.0,10.0);

  // -------------------------------------------------------------------------------------
  // Create histograms.

  TH1F *nu_energy = new TH1F("nu_energy","Neutrino Energy Spectrum",100,0,3000);
  nu_energy->SetXTitle("Energy (MeV)");

  TH1F *nu_ccqe_energy = new TH1F("nu_energy_ccqe","Neutrino CCQE Energy Spectrum",100,0,3000);
  nu_ccqe_energy->SetXTitle("Energy (MeV)");  

  TH1F *nu_ccqe_candidate_energy = new TH1F("nu_energy_ccqe_can","Neutrino CCQE candidate Energy Spectrum",100,0,3000);
  nu_ccqe_candidate_energy->SetXTitle("Energy (MeV)");  

  TH1F *nu_true_ccqe_candidate_energy = new TH1F("nu_energy_true_ccqe_can","Neutrinotrue  CCQE candidate Energy Spectrum",100,0,3000);
  nu_true_ccqe_candidate_energy->SetXTitle("Energy (MeV)");  

  TH1F *nu_false_ccqe_candidate_energy = new TH1F("nu_energy_false_ccqe_can","Neutrinofalse  CCQE candidate Energy Spectrum",100,0,3000);
  nu_false_ccqe_candidate_energy->SetXTitle("Energy (MeV)");  

  TH1F *rec_nu_energy = new TH1F("rec_nu_energy","Reconstructed Neutrino Energy Spectrum",100,0,3000);
  rec_nu_energy->SetXTitle("Reconstructed energy (MeV)");

  
  TProfile *ccqe_eff_vs_nu_energy 
    = new TProfile("eff_vs_nu_energy","CCQE Efficiency vs Neutrino Energy",100,0,3000);
  ccqe_eff_vs_nu_energy->SetXTitle("Energy (MeV)");
  ccqe_eff_vs_nu_energy->SetYTitle("CCQE Efficiency");

  TH1F *muon_ccqe_angle = new TH1F("muon_ccqe_angle","Muon CCQE Angle",60,0,180);
  muon_ccqe_angle->SetXTitle("Muon Angle (degrees)");  
  
  TProfile *ccqe_eff_vs_muon_angle
    = new TProfile("eff_vs_muon_angle","CCQE Efficiency vs Muon Angle",60,0,180);
  ccqe_eff_vs_muon_angle->SetXTitle("Muon Angle (degrees)");
  ccqe_eff_vs_muon_angle->SetYTitle("CCQE Efficiency");


  // _____________________________________________________________________________________
  // Load data files   
  TString fileName("cheating_ccqe_input_files.txt");
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
  }

  // _____________________________________________________________________________________
  // Prepare to read trees.

  // Set the branches for the truth information.


  // vertex information
  Int_t NVtxFGD1; 
  Int_t NVtxFGD2;
  TClonesArray *VtxFGD1 = new TClonesArray("ND::TTruthVerticesModule::TTruthVertex", 20);
  TClonesArray *VtxFGD2 = new TClonesArray("ND::TTruthVerticesModule::TTruthVertex", 20);

  mcTruthVertices.SetBranchAddress("NVtxFGD1", &NVtxFGD1);
  mcTruthVertices.SetBranchAddress("NVtxFGD2", &NVtxFGD2);
  mcTruthVertices.SetBranchAddress("VtxFGD1", &VtxFGD1);
  mcTruthVertices.SetBranchAddress("VtxFGD2", &VtxFGD2);

  mcTruthVertices.Print("toponly");

  //  TRefArray *AllTruthTrajectories = new TRefArray();
  //  mcTruthTrajectories.SetBranchAddress("AllTrajecto",AllTruthTrajectories);

  // trajectories information
  const int NTRAJTYPES=7;
  std::string names[NTRAJTYPES] = {"TrajsLepton","TrajsBaryon","TrajsMeson","TrajsPhoton","TrajsOtherCharged",
			  "TrajsOtherNeutral","TrajsOther"}
  TClonesArray *TruthTrajs[NTRAJTYPES];
  for (int ii=0;ii<NTRAJTYPES;ii++){
    TruthTrajs[ii] = new TClonesArray("ND::TTruthTrajectoriesModule::TTruthTrajectory", 30);
    mcTruthTrajectories.SetBranchAddress(names[ii].c_str(),&(TruthTrajs[ii]));
  }

  mcTruthTrajectories.Print("toponly");


  //__________________________________
  // counters and cuts

  int total_ccqe_fiducial = 0;
  int total_selected[4] = {0,0,0,0};
  int total_selected_ccqe_fiducial[4] = {0,0,0,0};

  bool passedVertexCut = false;
  bool passedNegTpcCut = false;
  bool passedFgdPionCut = false;
  bool foundMichelCluster = false;
  bool isCCQE = false;
  bool isCCQECandidate = false;
  bool isInFiducialVolume = false;
  bool foundTrueMuonMom = false;

  // __________________________________________________________________________
  // Loop over all events.  
  int numberEntries = mcTruthVertices.GetEntries();
  std::cout << "Number entries: " << numberEntries << std::endl;

  // time before event loop
  TStopwatch timer;
  timer.Start();

  int nEventsLoop = numberEntries;

  //  nEventsLoop=100;
  for(int i = 0; i < nEventsLoop; i++){
    
    mcTruthVertices.GetEntry(i);
    mcTruthTrajectories.GetEntry(i);

    if(i%500 == 0) std::cout <<  "Events Processed " << i << std::endl;
    
    // ------------------------------------------------------------------------
    // Check vertex truth information. (fiducial and CCQE reaction code)


    // Get the true FGD vertex
    ND::TTruthVerticesModule::TTruthVertex *true_fgd_vertex = NULL;
    if(NVtxFGD1 >0){
      true_fgd_vertex = (ND::TTruthVerticesModule::TTruthVertex*)VtxFGD1->UncheckedAt(0);
    }else if (NVtxFGD2 >0){
      true_fgd_vertex = (ND::TTruthVerticesModule::TTruthVertex*)VtxFGD2->UncheckedAt(0);
    }else{
      std::cerr << "There should be at least one vertex in one FGD.  Skipping event."<<std::endl;
      continue;
    }
            
    // Get initial neutrino momentum.
    TLorentzVector truthNeutrinoMom = true_fgd_vertex->MomNu;

    // Get true primary vertex
    TLorentzVector truthVertex = true_fgd_vertex->Vertex;  

    // Check that the vertex is inside the fiducial volume  
    isInFiducialVolume = false;
    if (tracker_utils->IsInFiducialFGD1(truthVertex.Vect(), fgdFiducialTrim) || 
	tracker_utils->IsInFiducialFGD2(truthVertex.Vect(), fgdFiducialTrim)){
      isInFiducialVolume = true;
    }
    
    // check CCQE interaction
    isCCQE = neut_utils->IsQELCC(true_fgd_vertex);    

    cout << "Reaction code: " << neut_utils->GetReactionCode(true_fgd_vertex) << endl;

    // -------------------------------------------------------------------------
    // Get true trajectory information
  
    TLorentzVector muon_momentum; 
    TVector3 muon_momentum_tpc; 
    foundTrueMuonMom = false;
    foundMichelCluster = false;

    int nUsefulTPCTrajs=0;
    int nNegativeTPCTrajs=0;
    int nMichelElecTrajs=0;
    TClonesArray *usefulTPCTrajs = new TClonesArray("ND::TTruthTrajectoriesModule::TTruthTrajectory", 10);
    TClonesArray *negativeTPCTrajs = new TClonesArray("ND::TTruthTrajectoriesModule::TTruthTrajectory", 5);
    TClonesArray *michelElecTrajs = new TClonesArray("ND::TTruthTrajectoriesModule::TTruthTrajectory", 5);

    for (int tt=0;tt<NTRAJTYPES;tt++){
      for (int ii=0; ii< TruthTrajs[tt]->GetEntries(); ii++){
	ND::TTruthTrajectoriesModule::TTruthTrajectory* traj = 
	  (ND::TTruthTrajectoriesModule::TTruthTrajectory*)TruthTrajs[tt]->UncheckedAt(ii);

		
	// Find the true muon momentum
	if (fabs(traj->TrajPDG) == 13 && !foundTrueMuonMom){
	  muon_momentum = traj->InitMom;
	  foundTrueMuonMom = true;		

	  muon_momentum_tpc = cheating_utils->GetTPCEntranceMomentum(*traj);        

	}
		
	if (traj->TrajID >= true_fgd_vertex->MinTrackID && traj->TrajID <= true_fgd_vertex->MaxTrackID){
	  if (!traj->CheatingInfoIsSet) cout << "cheating info not set !!!" << endl;	
	  
	  // Useful TPC tracks
	  if (cheating_utils->IsAUsefulTPCTrack(*traj)){
	    (*usefulTPCTrajs)[nUsefulTPCTrajs++] = traj;

	    // Useful negative tracks in TPC
	    if (traj->Charge == -3)
	      (*negativeTPCTrajs)[nNegativeTPCTrajs++] = traj;
	  }

	  // Get Michel's electron candidates
	  if (cheating_utils->IsMichelElectronCandidate(*traj, truthVertex))
	    (*michelElecTrajs)[nMichelElecTrajs++] = traj;
	  
	}
      }
    }

    cout << " # useful TPC trajs = " << nUsefulTPCTrajs << endl;
    cout << " # useful negative TPC trajs = " << nNegativeTPCTrajs << endl;
    cout << " # michel electron candidates = " << nMichelElecTrajs << endl;


    // -------------------------------------------------------------------------
    // Apply event selection criterion.
    
    // Reset variables
    isCCQECandidate = false;
    passedVertexCut = false; 
    passedNegTpcCut = false;
    passedFgdPionCut = false;

    //---------  One track  Analysis-----------------
    
    // 1. only one track
    if (nUsefulTPCTrajs == 1){

      cout << " 1. one track " << endl;

      ND::TTruthTrajectoriesModule::TTruthTrajectory* traj2 = 
	(ND::TTruthTrajectoriesModule::TTruthTrajectory*)usefulTPCTrajs->UncheckedAt(0);

      // 2. vertex in fiducial volume
      if (tracker_utils->IsInFiducialFGD1(truthVertex.Vect(), fgdFiducialTrim) || 
	  tracker_utils->IsInFiducialFGD2(truthVertex.Vect(), fgdFiducialTrim)){
	passedVertexCut = true;

	cout << " 2. in fiducial " << endl;

	// 3. negative track
	//	if (usefulTPCTrajs[0]->Charge == -3){
	if (nNegativeTPCTrajs == 1){
	  passedNegTpcCut = true;

	  cout << " 3. one negative track " << endl;

	  // 4. No michel electron 
	  if (nMichelElecTrajs == 0){
	    passedFgdPionCut = true;

	    cout << " 4. no michel electron " << endl;
	  }
	}
      }
    } 


    //---------  Two tracks  Analysis-----------------
    
    // 1. only two tracks
    if (nUsefulTPCTrajs == 2){

      // 2. Need to be exactly one measured vertex - for cheating analysis check if
      // the two "useful TPC trajectories" starts in same point   --- would be good to use unit

    
      ND::TTruthTrajectoriesModule::TTruthTrajectory* traj0 = 
	(ND::TTruthTrajectoriesModule::TTruthTrajectory*)usefulTPCTrajs->UncheckedAt(0);
      ND::TTruthTrajectoriesModule::TTruthTrajectory* traj1 = 
	(ND::TTruthTrajectoriesModule::TTruthTrajectory*)usefulTPCTrajs->UncheckedAt(1);

      TLorentzVector pos0 = traj0->InitPos;
      TLorentzVector pos1 = traj1->InitPos;

      if ((truthVertex - pos0).Mag() < 0.1 &&
	  (truthVertex - pos1).Mag() < 0.1) {      

	cout << " 2. tracks from same vertex " << endl;

	// 3. vertex in fiducial volume
	if (tracker_utils->IsInFiducialFGD1(truthVertex.Vect(), fgdFiducialTrim) || 
	    tracker_utils->IsInFiducialFGD2(truthVertex.Vect(), fgdFiducialTrim)){
	  passedVertexCut = true;

	  cout << " 3. in fiducial " << endl;

	  // 4. only one negative track
	  if (nNegativeTPCTrajs == 1){
	    passedNegTpcCut = true;
	  
	    cout << " 4. one negative track " << endl;

	    // 5. No michel electron 
	    if (nMichelElecTrajs == 0){
	      passedFgdPionCut = true;
	      
	      cout << " 5. no michel electron " << endl;

	    }
	  }
	}

      } 

    }


    // -------------------------------------------------------------------------
    // Update counters

    
    if(isInFiducialVolume && isCCQE) total_ccqe_fiducial++;

    // loop over selection cuts
    for(int cc = 0; cc < 4; cc++){
    
      if( cc==0 || 
	 (cc==1 && passedVertexCut) || 
	 (cc==2 && passedNegTpcCut) || 
	 (cc==3 && passedFgdPionCut)){
	
	total_selected[cc]++;      

	if(isInFiducialVolume && isCCQE)
	  total_selected_ccqe_fiducial[cc]++;

      }
    }

    if (passedVertexCut && 
	passedNegTpcCut &&
	passedFgdPionCut)
      isCCQECandidate = true;


    //______________________________________________________________________________________
    // Fill histograms.
    
    double neutrino_energy = sqrt(truthNeutrinoMom.X()**2 + truthNeutrinoMom.Y()**2 + truthNeutrinoMom.Z()**2);

    double pmu = muon_momentum_tpc.Mag();
    double V = 25*unit::MeV;
    double mp = unit::proton_mass_c2;
    double mn = unit::neutron_mass_c2;
    double mmu = 105.658367*unit::MeV;
    double emu = sqrt(pmu*pmu+mmu*mmu);
    double cos_theta_mu = muon_momentum.CosTheta();

    double rec_neutrino_energy = 0.5*((mp*mp-mmu*mmu)+2*emu*(mn-V) - pow(mn-V,2))/
      (-emu+(mn-V)+pmu*cos_theta_mu);


    if(isInFiducialVolume){
      nu_energy->Fill(neutrino_energy);
      rec_nu_energy->Fill(rec_neutrino_energy);
    }

    if(isInFiducialVolume && isCCQE){

      nu_ccqe_energy->Fill(neutrino_energy);
      //      TVector3 dir(muon_momentum.X(),muon_momentum.Y(),muon_momentum.Z());
      //      muon_ccqe_angle->Fill(dir.Theta()*180.0/3.1416);
      muon_ccqe_angle->Fill(muon_momentum.Theta()*180.0/3.1416);

      if(passedFgdPionCut){
    	ccqe_eff_vs_nu_energy->Fill(neutrino_energy,1.0);
	//	ccqe_eff_vs_muon_angle->Fill(dir.Theta()*180.0/3.1416, 1.0);
	ccqe_eff_vs_muon_angle->Fill(muon_momentum.Theta()*180.0/3.1416, 1.0);
      }else{
	ccqe_eff_vs_nu_energy->Fill(neutrino_energy,0.00001);
	//	ccqe_eff_vs_muon_angle->Fill(dir.Theta()*180.0/3.1416, 0.00001);
	ccqe_eff_vs_muon_angle->Fill(muon_momentum.Theta()*180.0/3.1416, 0.00001);
      }
    }

    if (isCCQECandidate){
      nu_ccqe_candidate_energy->Fill(neutrino_energy);
      if (isCCQE)
	nu_true_ccqe_candidate_energy->Fill(neutrino_energy);
      else
	nu_false_ccqe_candidate_energy->Fill(neutrino_energy);
    }

  }  // end of event loop


  // final time
  timer.Stop();

  std::cout << "Processing time: " << timer.CpuTime() << " " << timer.RealTime() << td::endl;


  //_________________________________________________________________________________________________ 
  // Plot histograms and graphs.

  TGraph *eff = new TGraph(); 
  TGraph *pur = new TGraph();

  for(int i = 0 ; i < 4; i++){
    std::cout << "Summary: " << total_selected[i] << " selected, " << total_ccqe_fiducial
	      << " CCQE in fiducial volume, " << total_selected_ccqe_fiducial[i] << " selected in fiducial" << std::endl;
    if(total_selected[i]> 0){
      std::cout << "Purity:     "  << ((double)total_selected_ccqe_fiducial[i])/((double)total_selected[i])
            <<std::endl;
      pur->SetPoint(i,i+1,((double)total_selected_ccqe_fiducial[i])/((double)total_selected[i]) *100.0);
    }
    if(total_ccqe_fiducial > 0){
      std::cout << "Efficiency: " << ((double)total_selected_ccqe_fiducial[i])/((double)total_ccqe_fiducial)
            <<std::endl;
      eff->SetPoint(i,i+1,((double)total_selected_ccqe_fiducial[i])/((double)total_ccqe_fiducial)*100.0);
    }

  }


  //-- efficiency plot --

  TCanvas *c = new TCanvas("C");
  c->SetFillColor(10);

  eff->Draw("APL*");
  eff->GetXaxis()->SetLabelSize(0);
  eff->SetMarkerStyle(22);
  eff->SetMarkerColor(4);
  eff->SetLineColor(4);
  eff->SetMarkerSize(eff->GetMarkerSize()*1.8);
  eff->GetYaxis()->SetTitle("Percentage");
  eff->SetTitle("CCQE Efficiency and Purity - Proper Reconstruction");
  eff->SetMaximum(110);
  eff->SetMinimum(0);
  
  eff->GetXaxis()->SetNdivisions(4);

  pur->Draw("*L");
  pur->SetMarkerStyle(21);
  pur->SetMarkerColor(2);
  pur->SetLineColor(2);
  pur->SetMarkerSize(pur->GetMarkerSize()*1.8);
 

  TLatex t;
  t.SetTextSize(t.GetTextSize()*0.7);
  t.DrawText(0.8,-7,"No Cut");
  t.DrawText(1.8,-7,"1 or 2 Track/");
  t.DrawText(1.7,-12,"Fiducial Vertex Cut");
  t.DrawText(2.8,-7,"1 Neg TPC");
  t.DrawText(2.8,-12,"Track Cut");
  t.DrawLatex(3.8,-7,"FGD #pi^{+} Cut");

  TLegend *leg = new TLegend(0.6,0.68,0.89,0.85);
  leg->SetFillColor(10);
  leg->AddEntry(eff,"Efficiency","p");
  leg->AddEntry(pur,"Purity","p");
  leg->Draw("SAME");
  c->Update();


  //-- neutrino energy --

  TCanvas *c2 = new TCanvas("C2","C2",400,0,1000,800);
  c2->SetFillColor(10);
  c2->Divide(2,2);
  c2->cd(1);
  nu_energy->Draw();
  nu_ccqe_energy->SetLineColor(2);
  nu_ccqe_energy->Draw("SAME");

  nu_ccqe_candidate_energy->SetLineColor(3);
  nu_ccqe_candidate_energy->Draw("SAME");

  nu_true_ccqe_candidate_energy->SetLineColor(4);
  nu_true_ccqe_candidate_energy->Draw("SAME");

  nu_false_ccqe_candidate_energy->SetLineColor(5);
  nu_false_ccqe_candidate_energy->Draw("SAME");

  TLegend *leg2 = new TLegend(0.6,0.68,0.89,0.85);
  leg2->SetFillColor(10);
  leg2->AddEntry(nu_energy,"All");
  leg2->AddEntry(nu_ccqe_energy,"CCQE");
  leg2->AddEntry(nu_ccqe_candidate_energy,"CCQE candidates");
  leg2->AddEntry(nu_true_ccqe_candidate_energy,"CCQE true candidates");
  leg2->AddEntry(nu_false_ccqe_candidate_energy,"CCQE false candidates");
  leg2->Draw("SAME");

  //-- efficiency versus energy --

  c2->cd(2);
  ccqe_eff_vs_nu_energy->Draw();

  //-- muon angle --

  c2->cd(3);
  muon_ccqe_angle->Draw();

  //-- efficiency versus muon angle --

  c2->cd(4);
  ccqe_eff_vs_muon_angle->Draw();


  return; 



}
