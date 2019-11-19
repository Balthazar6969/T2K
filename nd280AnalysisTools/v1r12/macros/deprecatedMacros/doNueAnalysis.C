#include "iostream"
#include "vector"
//______________________________________________________________________________________
// 
// This routine allows one to do a rudimentary Nu e selection using Tracker 
// reconstruction information stored in oaAnalysis ROOT tree.
// 
// In order to run this script, you must do the following.
//
// 1) You must have a file (or files) containing the oaAnalysis summary ROOT tree.  
//    This must be generated using OAAnalysis.exe, if you don't already 
//    have it.
// 2) Build the oaAnalysis library by running ./runAnalyses.py -c inputFile.
// 3) Go to the macros directory.
// 4) Edit the file called 'Nue_input_files.txt': this should be a list of all
//    the summary files that you want to process.
// 5) Start root and run this macro by doing:
//    .x doNueAnalysis.C("generatorName") Generator name should be genie or neut
// 
//    The macro will loop over all the events in your selected files and 
//    output some Nue selection efficiency and purity distributions.
//
//
// M. George - Nov 10, 2008
//
void doNueAnalysis(char* generator) {

  // Load some important libraries.

  gSystem->AddIncludePath("-I$OAANALYSISROOT/src");
  gSystem->AddIncludePath("-I.");
  //gROOT->ProcessLine(".x rootlogon.C");
  
  gSystem->Load("libPhysics");
  gSystem->Load("libGeom");
  gSystem->Load("libTree");
  gSystem->Load("libMatrix");
  gSystem->Load("liboaEvent.so");
  gSystem->Load("liboaAnalysis.so");
  gROOT->ProcessLine(".L tracker_utilities.C");

  TChain mcTruthVertices = TChain("TruthDir/Vertices");
  TChain mcTruthTrajectories = TChain("TruthDir/Trajectories");
  TChain reconTracker = TChain("ReconDir/Tracker");
  //__________________________________________________________________________________
  // Set constants for analysis

  // Parameters defines how many mm to trim off the active 
  // FGD volume in order to define the fiducial FGD volume.
  TVector3 fgd_fiducial_trim = TVector3(200.0,200.0,10.0);

  // These are the dimensions of the FGD1 active volume.
  TVector3 fFGD1ActiveMin = TVector3(-931.6, -856.6, 193.558);
  TVector3 fFGD1ActiveMax = TVector3(931.6, 1006.6, 513.758);

  // These are the dimensions of the FGD1 fiducial volume.
  TVector3 fFGD1FiducialMin = fFGD1ActiveMin + fgd_fiducial_trim;
  TVector3 fFGD1FiducialMax = fFGD1ActiveMax - fgd_fiducial_trim;;

  // These are the dimensions of the FGD2 active volume.
  TVector3 fFGD2ActiveMin = TVector3(-931.6, -856.6, 1542.26);
  TVector3 fFGD2ActiveMax = TVector3(931.6, 1006.6, 1863.86);

  // These are the dimensions of the FGD2 fiducial volume.
  TVector3 fFGD2FiducialMin = fFGD2ActiveMin + fgd_fiducial_trim;
  TVector3 fFGD2FiducialMax = fFGD2ActiveMax - fgd_fiducial_trim;;

  // These are the dimensions of the ECAL volume.
  TVector3 fECALMin = TVector3(-1000.0, -950.0, 2920.00);
  TVector3 fECALMax = TVector3(1000.0, 1050.0, 3332.00);  
  // -------------------------------------------------------------------------------------
  // Create histograms.

  TH1F *nu_energy = new TH1F("nu_energy","Neutrino Energy Spectrum",100,0,3000);
  nu_energy->SetXTitle("Energy (MeV)");

  TH1F *nu_electron_energy = new TH1F("nu_electron_energy","Electron Neutrino Energy Spectrum",100,0,3000);
  nu_electron_energy->SetXTitle("Energy (MeV)");  
  
  TProfile *Nue_eff_vs_nu_energy 
    = new TProfile("eff_vs_nu_energy","NuE Efficiency vs Neutrino Energy",100,0,3000);
  Nue_eff_vs_nu_energy->SetXTitle("Energy (MeV)");
  Nue_eff_vs_nu_energy->SetYTitle("NuE Efficiency");

  TH1F *electron_Nue_angle = new TH1F("electron_Nue_angle","Electron NuE Angle",60,0,180);
  electron_Nue_angle->SetXTitle("Electron Angle (degrees)");  
  
  TProfile *Nue_eff_vs_electron_angle
    = new TProfile("eff_vs_electron_angle","NuE Efficiency vs electron Angle",60,0,180);
  Nue_eff_vs_electron_angle->SetXTitle("Electron Angle (degrees)");
  Nue_eff_vs_electron_angle->SetYTitle("NuE Efficiency");
  // _____________________________________________________________________________________
  // Load data files   
  TString fileName("Nue_input_files.txt");
  
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
  }
  // _____________________________________________________________________________________
  // Prepare to read trees.

  // Set the branches for the truth information.
  Int_t NVtxFGD1; 
  Int_t NVtxFGD2;
  TClonesArray *VtxFGD1 = new TClonesArray("ND::TTruthVerticesModule::TTruthVertex", 20);
  TClonesArray *VtxFGD2 = new TClonesArray("ND::TTruthVerticesModule::TTruthVertex", 20);

  mcTruthVertices.SetBranchAddress("NVtxFGD1", &NVtxFGD1);
  mcTruthVertices.SetBranchAddress("NVtxFGD2", &NVtxFGD2);
  mcTruthVertices.SetBranchAddress("VtxFGD1", &VtxFGD1);
  mcTruthVertices.SetBranchAddress("VtxFGD2", &VtxFGD2);

  mcTruthVertices.Print("toponly");

  TRefArray *AllTruthTrajectories = new TRefArray();
  mcTruthTrajectories.SetBranchAddress("AllTrajectories",AllTruthTrajectories);
  //In case the above doesn't work for trajectories you may need to access them like this
  //for example for lepton trajectories 
  //Int_t NTrajsLepton;
  //TClonesArray *TrajsLepton = new TClonesArray("ND::TTruthTrajectoriesModule::TTruthTrajectory", 100);
  //mcTruthTrajectories->SetBranchAddress("NTrajsLepton",  &NTrajsLepton); 
  mcTruthTrajectories.Print("toponly");

  //AllTrajectories(new TRefArray()

  // Set the branches for the tracker recon information.
  Int_t NVertices;
  Int_t NTracks;
  Int_t NDelayedClusters;
  TClonesArray *Vertices = new TClonesArray("ND::TTrackerReconModule::TTrackerVertex", 20);
  TClonesArray *Tracks = new TClonesArray("ND::TTrackerReconModule::TTrackerTrack", 20);
  TClonesArray *DelayedClusters = new TClonesArray("ND::TTrackerReconModule::TFgdCluster", 20);

  reconTracker.SetBranchAddress("NVertices", &NVertices);
  reconTracker.SetBranchAddress("NTracks", &NTracks);
  reconTracker.SetBranchAddress("NDelayedClusters", &NDelayedClusters);
  reconTracker.SetBranchAddress("Vertices", &Vertices);
  reconTracker.SetBranchAddress("Tracks", &Tracks);
  reconTracker.SetBranchAddress("DelayedClusters", &DelayedClusters);

  reconTracker.Print("toponly");
  // __________________________________________________________________________
  // Loop over all events.  
 
  int entries = mcTruthVertices.GetEntries();

  std::cout << "entries: " << entries << std::endl;
  
  int ntotal = 0, nfid = 0;
  int total_Nue_fiducial = 0;
  int total_selected[4] = {0,0,0,0};
  int total_selected_Nue_fiducial[4] = {0,0,0,0};

  TStopwatch timer;
  timer.Start();

  bool passedFgdVertexCut = false;
  bool passedNegTpcCut = false;
  bool passedFgdPionCut = false;

  bool passedEcalStopsCut = false;
  bool passedPi0Cut = false;

  for(int i = 0; i < entries; i++){
    
    mcTruthVertices.GetEntry(i);
    reconTracker.GetEntry(i);
    if(i%5000 == 0)
      std::cout <<  "Events Processed " << i << std::endl;

    
    // ------------------------------------------------------------------------
    // Check vertex truth information.

    ND::TTruthVerticesModule::TTruthVertex *true_fgd_vertex;

    bool trueFiducialFGD1 = false, trueFiducialFGD2 = false;

    if(NVtxFGD1 >0){
      true_fgd_vertex = (ND::TTruthVerticesModule::TTruthVertex*)VtxFGD1->UncheckedAt(0);
      trueFiducialFGD1 = true;

    }else if (NVtxFGD2 >0){
      true_fgd_vertex = (ND::TTruthVerticesModule::TTruthVertex*)VtxFGD2->UncheckedAt(0);
      trueFiducialFGD2 = true;
    }else{
      //std::cerr << "There should be at least one vertex in one FGD.  Skipping event."<<std::endl;
      continue;
    }

    //GENIE
    if(generator == "genie"){
      //std::cout << "GENIE is choosen " << std::endl;    
      // Get initial neutrino momentum.
      TLorentzVector neutrino_momentum = true_fgd_vertex->MomNu;
      int neutrino_pdg = true_fgd_vertex->PDGnu;
      TLorentzVector VV = true_fgd_vertex->Vertex;  
      TVector3 pos(VV.X(),VV.Y(),VV.Z());
      bool fiducial = false;
      bool Nue_fiducial = false;

      std::string reaction =  true_fgd_vertex->ReactionCode;
      std::string neutrino_type, target_pdg, process;
      neutrino_type = reaction.substr (0,6);
      target_pdg = reaction.substr (6,15);
      size_t poss;
      poss = reaction.find("proc:");
      process = reaction.substr (poss);
      if( neutrino_type == "nu:14;" && (IsInvolume(pos,fFGD1FiducialMin,fFGD1FiducialMax) ||
					IsInvolume(pos,fFGD2FiducialMin,fFGD2FiducialMax))){
	fiducial = true;

	if(process == "proc:Weak[CC],QES;")
	  Nue_fiducial = true;
      }
    }
    //NEUT
    else if(generator == "neut"){
	// Get initial neutrino momentum.
	TLorentzVector neutrino_momentum = true_fgd_vertex->MomNu;
	int neutrino_pdg = true_fgd_vertex->PDGnu;
	TLorentzVector VV = true_fgd_vertex->Vertex;
	TVector3 pos(VV.X(),VV.Y(),VV.Z());
	bool fiducial = false;
	bool Nue_fiducial = false;
	if(neutrino_pdg == 14 && (IsInvolume(pos,fFGD1FiducialMin,fFGD1FiducialMax) ||
				  IsInvolume(pos,fFGD2FiducialMin,fFGD2FiducialMax))){
	  fiducial = true;

	  if(GetReactionCode(true_fgd_vertex) == 1)
	    Nue_fiducial = true;
      }
    }
     else{
	cerr << "You have to specify one generator name either genie or neut " << endl;
	return;
      }
     
    // Find the true electron track.
    TLorentzVector electron_momentum;
    if(Nue_fiducial)
      for(int ll = 0; ll < true_fgd_vertex->LeptonIDs.size(); ll++)
	if(true_fgd_vertex->LeptonPDGs[ll] == 11)
	  electron_momentum = true_fgd_vertex->LeptonMoms[ll];
  
    // -------------------------------------------------------------------------
    // Apply event selection criterion.
    /* NOTE: The way this macro was initial written was supposed to take as input
       only a "monochromatic" NuE beam. For this case only the reconstruction as 
       described below is still valid. In the case for example of MDC0, where the neutrino
       flux is mostly NuMu, and only a small fraction is NuE, a proper PID selection must be performed.
     */    
    // reconstructed initial vertex 
    TLorentzVector recon_vertex_pos;
    TLorentzVector recon_ECAL_back_pos;

    // reconstructed electron track
    ND::TTrackerReconModule::TTrackerTrack *electron_track = 0;
    ND::TTrackerReconModule::TTrackerTrack *second_track = 0;

    // Reset variables
    //passedFgdVertexCut = false;
    //passedEcalStopsCut = false;
    //passedPi0Cut = false;

    int total_tracks = NTracks; // Total number of tracks.
    int total_tpc = 0;    // Total number of TPC tracks.
    for(unsigned int j = 0; j < NTracks; j++){
      ND::TTrackerReconModule::TTrackerTrack *track 
	= (ND::TTrackerReconModule::TTrackerTrack*)(*Tracks)[j];
      bool hasTPC = false;
      for(int tt = 0; tt < 3; tt++)
	if(track->tpc_charge[tt] != 0.0)
	  hasTPC = true;
      if(hasTPC)
	total_tpc++;
    }

    if(total_tracks == 1 && total_tpc == 1 ){ // One track analysis - ie ignore 
                                              // multi track events
      // ______________________________________________________________
      // Step 1 -> Check vertex is in fiducial

      // Get the one track.  
      ND::TTrackerReconModule::TTrackerTrack *track 
	= (ND::TTrackerReconModule::TTrackerTrack*)(*Tracks)[0];
      
      // Assume that upstream end of track is initial vertex.
      recon_vertex_pos  = track->FrontPosition;
      TVector3 tmp_pos(recon_vertex_pos.X(),recon_vertex_pos.Y(),recon_vertex_pos.Z());

      // check that vertex position is inside one of the two FGDs.
      if(IsInvolume(tmp_pos,fFGD1FiducialMin,fFGD1FiducialMax) ||
	 IsInvolume(tmp_pos,fFGD2FiducialMin,fFGD2FiducialMax)){
	passedFgdVertexCut = true;
		
	// ______________________________________________________________
	// Step 2 -> Check that we have exactly one negative track.
	
	// Check the charge of only track.
	int charge = getTrackTpcCharge(track);
	
	// If negative, pass cut and we've found ccqe track.
	if(charge == -1.0){
	  passedNegTpcCut = true;
	  ccqeTrack = track;
	} // End step 2, one NEGATIVE track analysis.
      } // End step 1, one track analysis.
    }
	// ______________________________________________________________
	// Step 1 -> Check vertex is in fiducial for 2 tpc tracks.
        
    if(total_tracks == 2 && total_tpc > 0 ){ // Two track analysis
      
      // Need to be exactly one measured vertex.
      if(NVertices == 1){
    
	ND::TTrackerReconModule::TTrackerVertex *vertex 
	  = (ND::TTrackerReconModule::TTrackerVertex*)(*Vertices)[0];

	recon_vertex_pos  = vertex->Position;
	TVector3 tmp_pos(recon_vertex_pos.X(),recon_vertex_pos.Y(),recon_vertex_pos.Z());

	// check that vertex position is inside one of the two FGDs.
	if(IsInvolume(tmp_pos,fFGD1FiducialMin,fFGD1FiducialMax) ||
	   IsInvolume(tmp_pos,fFGD2FiducialMin,fFGD2FiducialMax)){
	  passedFgdVertexCut = true;
	  // ______________________________________________________________
	  // Step 2 -> Check that we have exactly one negative track for 2 tpc tracks.
	  
	  int number_negative_tpc_tracks=0;
	  for(unsigned int j = 0; j < NTracks; j++){
	    
	    ND::TTrackerReconModule::TTrackerTrack *track 
	      = (ND::TTrackerReconModule::TTrackerTrack*)(*Tracks)[j];

	    // Get reconstructed charge (which assumes forward going track)
	    int charge = getTrackTpcCharge(track);
	    
	    // Check if track was actually backward-going.  If so, flip charge.
	    double delta_z_front = recon_vertex_pos.Z() - track->FrontPosition.Z();
	    
	    if(delta_z_front > 100.0)
	      charge = -1*charge;
	    	    
	    // Now add up number of negative tracks.
	    if(charge < 0){
	      number_negative_tpc_tracks++;
	      ccqeTrack = track;
	    }
	    else if( charge > 0 ) {
	      protonTrack = track;
	    }
	  }
          
          if(number_negative_tpc_tracks == 1)
            passedNegTpcCut = true;

        } // End step 2, two track analysis.	  	
      } // End step 1, two track analysis.        
    }
   // _______________________________________________________________________________
   // Step 3 -> Check that we have no delayed fgd clusters ie remove pi+ -> mu+ -> e+.

    if(passedFgdVertexCut && passedNegTpcCut){

      // check if there are any delayed  FGD clustered 
      // near the vertex point.  If so, then assume from 
      // pi+ -> mu+ -> e+ and hence reject.
      bool foundFgdMichelCluster = false;
      for(unsigned int j = 0; j < NDelayedClusters; j++){

	ND::TTrackerReconModule::TFgdCluster *cluster
	  = (ND::TTrackerReconModule::TFgdCluster*)(*DelayedClusters)[j];
	
	TLorentzVector dp = cluster->Position - recon_vertex_pos;
	// find distance between vertex position and hit position.
	double dist = 9999.0;
	if(cluster->PositionVariance.X() = 0.0)
	  dist = sqrt(dp.X()*dp.X() + dp.Z()*dp.Z());
	else
	  dist = sqrt(dp.Y()*dp.Y() + dp.Z()*dp.Z());
	
	if(dist < 500.0)
	  foundFgdMichelCluster = true;
	
      }
      
      if(!foundFgdMichelCluster)
	passedFgdPionCut = true;

    } // End step 3.
   // _______________________________________________________________________________
   // Step 4 -> Check that the track stops in the ECAL.
    //Needs to be fixed soon
    /*
    if(passedFgdVertexCut && passedNegTpcCut){

      // check that the track ends in the ECAL 
      // if not could be a muon and hence reject.

      int total_tracks = NTracks; // Total number of tracks.
      int total_ECAL = 0;    // Total number of ECAL tracks.
      for(unsigned int k = 0; k < NTracks; k++){
        // NB currently does tracks change for clusters
	ND::TTrackerECALReconModule::TECALReconTrack *ecal_track 
	  = (ND::TTrackerECALReconModule::TECALReconTrack*)(*ReconTracks)[k];

      // Assume that downstream end of track is final vertex.
      recon_ECAL_back_pos  = ecal_track->BackPosition;
      TVector3 tmp_pos(recon_ECAL_back_pos.X(),recon_ECAL_back_pos.Y(),recon_ECAL_back_pos.Z());
      
        if(IsInvolume(tmp_pos,fECALMin,fECALMax)){ 

          passedEcalStopsCut = true;
        }
      }
    }  // End step 4.
    */
    /*
   // _______________________________________________________________________________
   // Step 5 -> Check that we have no in time ECAL clusters ie remove pi0.
   // Work in progress.

    if(passedFgdVertexCut && passedNegTpcCut && passedEcalStopsCut){

      // Check if there are any in time (same bucket as track) clusters (TReconPID's) 
      //  in ECAL.  If so, then assume pi0 and hence reject. 

      bool foundECalMichelCluster = false;
      for(unsigned int j = 0; j < NInTimeClusters; j++){

	ND::TNueReconModule::TECALInTimeClusters *inTimeCluster
	  = (ND::TNueReconModule::TECALInTimeClusters*)(*InTimeClusters)[j];
	
	TLorentzVector it = inTimeCluster->Bucket - timeBucket;
	}
      if(!foundECalMichelCluster)
	passedPi0Cut = true;

    } // End step 5.
*/
    // -------------------------------------------------------------------------
    // Update counters
    if(Nue_fiducial)
      total_Nue_fiducial++;

    for(int cc = 0; cc < 5; cc++){
    
      if(cc==0 
	 || (cc==1 && passedFgdVertexCut)
	 || (cc==2 && passedNegTpcCut)
	 || (cc==3 && passedFgdPionCut)
	 || (cc==4 && passedEcalStopsCut)
	 //|| (cc==5 && passedPi0Cut)
	 ){
	total_selected[cc]++;      
	if(Nue_fiducial)
	  total_selected_Nue_fiducial[cc]++;
      }
    }
    //______________________________________________________________________________________
    // Fill histograms.
    
    double nu_mom = sqrt(neutrino_momentum.X()**2 + neutrino_momentum.Y()**2 + neutrino_momentum.Z()**2);

    if(fiducial)
      nu_energy->Fill(nu_mom);

    if(Nue_fiducial){

      getTrackMomentum(ccqeTrack);

      nu_electron_energy->Fill(nu_mom);
      TVector3 dir(electron_momentum.X(),electron_momentum.Y(),electron_momentum.Z());
      electron_Nue_angle->Fill(dir.Theta()*180.0/3.1416);

      if(passedFgdPionCut){
    	Nue_eff_vs_nu_energy->Fill(nu_mom,1.0);
	Nue_eff_vs_electron_angle->Fill(dir.Theta()*180.0/3.1416, 1.0);
      }else{
	Nue_eff_vs_nu_energy->Fill(nu_mom,0.00001);
	Nue_eff_vs_electron_angle->Fill(dir.Theta()*180.0/3.1416, 0.00001);
      }
    }
  }

  timer.Stop();

  std::cout << "Processing time: " << timer.CpuTime() << " " << timer.RealTime() << td::endl;
  // -----------------------------------------------------------------------------------------
  // Plot histograms and graphs.

  TGraph *eff = new TGraph(); 
  TGraph *pur = new TGraph();

  for(int i = 0 ; i < 4; i++){
    std::cout << "Summary: " << total_selected[i] << " selected, " << total_Nue_fiducial
	      << " CCQE in fiducial volume, " << total_selected_Nue_fiducial[i] << " selected in fiducial" << std::endl;
    if(total_selected[i]> 0){
      std::cout << "Purity:     "  << ((double)total_selected_Nue_fiducial[i])/((double)total_selected[i])
            <<std::endl;
      pur->SetPoint(i,i+1,((double)total_selected_Nue_fiducial[i])/((double)total_selected[i]) *100.0);
    }
    if(total_Nue_fiducial > 0){
      std::cout << "Efficiency: " << ((double)total_selected_Nue_fiducial[i])/((double)total_Nue_fiducial)
            <<std::endl;
      eff->SetPoint(i,i+1,((double)total_selected_Nue_fiducial[i])/((double)total_Nue_fiducial)*100.0);
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


  TCanvas *c2 = new TCanvas("C2","C2",400,0,1000,800);
  c2->SetFillColor(10);
  c2->Divide(2,2);
  c2->cd(1);
  nu_energy->Draw();
  nu_electron_energy->SetLineColor(2);
  nu_electron_energy->Draw("SAME");
  TLegend *leg2 = new TLegend(0.6,0.68,0.89,0.85);
  leg2->SetFillColor(10);
  leg2->AddEntry(nu_energy,"All");
  leg2->AddEntry(nu_electron_energy,"CCQE");
  leg2->Draw("SAME");

  c2->cd(2);
  Nue_eff_vs_nu_energy->Draw();

  c2->cd(3);
  electron_Nue_angle->Draw();

  c2->cd(4);
  Nue_eff_vs_electron_angle->Draw();


  return; 



}
