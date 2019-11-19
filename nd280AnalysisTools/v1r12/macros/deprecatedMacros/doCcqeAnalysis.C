#include "iostream"
#include "vector"


//______________________________________________________________________________________
// 
// This routine allows one to do a rudimentary CCQE selection using Tracker 
// reconstruction information stored in oaAnalysis ROOT tree.
// 
// In order to run this script, you must do the following.
//
// 1) You must have a file (or files) containing the oaAnalysis summary ROOT tree.  
//    This must be generated using OAAnalysis.exe, if you don't already 
//    have it.
// 2) Build the oaAnalysis library in the normal way.
// 3) Go to the macros directory.
// 4) Edit the file called 'ccqe_input_files.txt': this should be a list of all
//    the summary files that you want to process.
// 5) Start root and run this macro by doing:
//    .x doCcqeAnalysis.C()
// 
//    The macro will loop over all the events in your selected files and 
//    output some CCQE selection efficiency and purity distributions.
//
//    A description of the selection cuts is given here:
//
//    http://www.nd280.org/meetings/nd280200801/talks/software/tracker_ccqe_event_selection_v2.pdf
//
// T. Lindner - Jan 28, 2008
//
void doCcqeAnalysis() {

// Load some important libraries.

  // gSystem->AddIncludePath("-I$OAEVENTROOT/src");
  gSystem->AddIncludePath("-I$OAANALYSISROOT/src");
  gSystem->AddIncludePath("-I.");
  // gROOT->ProcessLine(".x rootlogon.C");
  
  //TEMPORARY SOLUTION
  gSystem->Load("libPhysics");
  //TEMPORARY SOLUTION
  //Changed all TTrackerAnalysisUtils::IsInVolume  to  IsInvolume   
  //Changed all TTrackerAnalysisUtils::GetReactionCode  to  GetReactionCode
  //Changed all TTrackerAnalysisUtils::GetTrackTpcCharge  to  getTrackTpcCharge
  //Changed all TTrackerAnalysisUtils::GetTrackMomentum   to   getTrackMomentum


  gSystem->Load("libGeom");
  gSystem->Load("libTree");
  gSystem->Load("libMatrix");
  gSystem->Load("liboaEvent.so");
  gSystem->Load("liboaAnalysis.so");
  gROOT->ProcessLine(".L tracker_utilities.C");



  TChain mcTruthVertices = TChain("TruthDir/Vertices");
  TChain mcTruthTrajectories = TChain("TruthDir/Trajectories");
  TChain reconTracker = TChain("ReconDir/Tracker");

  //______________________________________________________________________________________
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

  

  // -------------------------------------------------------------------------------------
  // Create histograms.

  TH1F *nu_energy = new TH1F("nu_energy","Neutrino Energy Spectrum",100,0,3000);
  nu_energy->SetXTitle("Energy (MeV)");

  TH1F *nu_ccqe_energy = new TH1F("nu_energy_ccqe","Neutrino CCQE Energy Spectrum",100,0,3000);
  nu_ccqe_energy->SetXTitle("Energy (MeV)");  
  
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
  TString fileName("ccqe_input_files.txt");
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

  std::cout << "ent: " << entries << std::endl;
  int ntotal = 0, nfid = 0;

  int total_ccqe_fiducial = 0;
  int total_selected[4] = {0,0,0,0};
  int total_selected_ccqe_fiducial[4] = {0,0,0,0};

  TStopwatch timer;
  timer.Start();

  bool passedVertexCut = false;
  bool passedNegTpcCut = false;
  bool passedFgdPionCut = false;


  for(int i = 0; i < entries; i++){
    
    mcTruthVertices.GetEntry(i);
    reconTracker.GetEntry(i);
    if(i%500 == 0)
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
      std::cerr << "There should be at least one vertex in one FGD.  Skipping event."<<std::endl;
      continue;
    }
            
    // Get initial neutrino momentum.
    TLorentzVector neutrino_momentum = true_fgd_vertex->MomNu;


    TLorentzVector VV = true_fgd_vertex->Vertex;  
    TVector3 pos(VV.X(),VV.Y(),VV.Z());
    bool fiducial = false;
    bool ccqe_fiducial = false;
    if(IsInvolume(pos,fFGD1FiducialMin,fFGD1FiducialMax) ||
       IsInvolume(pos,fFGD2FiducialMin,fFGD2FiducialMax)){
      fiducial = true;
      if(GetReactionCode(true_fgd_vertex) == 1)
	ccqe_fiducial = true;
    }
    
    // Find the true muon track.
    TLorentzVector muon_momentum;
    if(ccqe_fiducial)
      for(int ll = 0; ll < true_fgd_vertex->LeptonIDs.size(); ll++)
	if(true_fgd_vertex->LeptonPDGs[ll] == 13)
	  muon_momentum = true_fgd_vertex->LeptonMoms[ll];


    // -------------------------------------------------------------------------
    // Apply event selection criterion.
    
    // reconstructed initial vertex 
    TLorentzVector recon_vertex_pos;
    // reconstructed muon track
    ND::TTrackerReconModule::TTrackerTrack *muonTrack = 0;
    ND::TTrackerReconModule::TTrackerTrack *second_track = 0;

    // Reset variables
    passedVertexCut = false; passedNegTpcCut = false;
    passedFgdPionCut = false;

    int total_tracks = NTracks; // Total number of tracks.
    int total_tpc = 0;    // Total number of TPC tracks.
    for(unsigned int j = 0; j < NTracks; j++){
      ND::TTrackerReconModule::TTrackerTrack *track 
	= (ND::TTrackerReconModule::TTrackerTrack*)(*Tracks)[j];
      bool hasTPC = false;
      for(int tt = 0; tt < 3; tt++){
	if(track->tpc_charge[tt] != 0.0)            
	  hasTPC = true;
      }

      if(hasTPC)
	total_tpc++;
    }

    if(total_tracks == 1 && total_tpc == 1 ){ // One track analysis
      
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
	passedVertexCut = true;
		
	// ______________________________________________________________
	// Step 2 -> Check that we have exactly one negative track.
	
	// Check the charge of only track.
	int charge = getTrackTpcCharge(track);

	// If negative, pass cut and we've found muon track.
	if(charge == -1.0){
	  passedNegTpcCut = true;
	  muonTrack = track;
	} // End step 2, one track analysis.
      } // End step 1, one track analysis.
    }
    
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
	  passedVertexCut = true;

	  // ______________________________________________________________
	  // Step 2 -> Check that we have exactly one negative track.
	  
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
	      muonTrack = track;
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

    if(passedVertexCut && passedNegTpcCut){

      // check if there are any delayed  FGD clustered 
      // near the vertex point.  If so, then assume from 
      // pi+ -> mu+ -> e+ and hence reject.
      bool foundMichelCluster = false;
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
	  foundMichelCluster = true;
	
      }
      
      if(!foundMichelCluster)
	passedFgdPionCut = true;

    } // End step 3.
    

    // -------------------------------------------------------------------------
    // Update counters
    if(ccqe_fiducial)
      total_ccqe_fiducial++;

    for(int cc = 0; cc < 4; cc++){
    
      if(cc==0 
	 || (cc==1 && passedVertexCut)
	 || (cc==2 && passedNegTpcCut)
	 || (cc==3 && passedFgdPionCut)){
	total_selected[cc]++;      
	if(ccqe_fiducial)
	  total_selected_ccqe_fiducial[cc]++;
      }
    }


    //______________________________________________________________________________________
    // Fill histograms.
    
    double nu_mom = sqrt(neutrino_momentum.X()**2 + neutrino_momentum.Y()**2 + neutrino_momentum.Z()**2);

    if(fiducial)
      nu_energy->Fill(nu_mom);

    if(ccqe_fiducial){
           
      getTrackMomentum(muonTrack);

      nu_ccqe_energy->Fill(nu_mom);
      TVector3 dir(muon_momentum.X(),muon_momentum.Y(),muon_momentum.Z());
      muon_ccqe_angle->Fill(dir.Theta()*180.0/3.1416);

      if(passedFgdPionCut){
    	ccqe_eff_vs_nu_energy->Fill(nu_mom,1.0);
	ccqe_eff_vs_muon_angle->Fill(dir.Theta()*180.0/3.1416, 1.0);
      }else{
	ccqe_eff_vs_nu_energy->Fill(nu_mom,0.00001);
	ccqe_eff_vs_muon_angle->Fill(dir.Theta()*180.0/3.1416, 0.00001);
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
  nu_ccqe_energy->SetLineColor(2);
  nu_ccqe_energy->Draw("SAME");
  TLegend *leg2 = new TLegend(0.6,0.68,0.89,0.85);
  leg2->SetFillColor(10);
  leg2->AddEntry(nu_energy,"All");
  leg2->AddEntry(nu_ccqe_energy,"CCQE");
  leg2->Draw("SAME");

  c2->cd(2);
  ccqe_eff_vs_nu_energy->Draw();

  c2->cd(3);
  muon_ccqe_angle->Draw();

  c2->cd(4);
  ccqe_eff_vs_muon_angle->Draw();


  return; 



}
