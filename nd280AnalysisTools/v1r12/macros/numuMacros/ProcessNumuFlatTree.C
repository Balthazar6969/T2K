#include "iostream"
#include "vector"
#include "stdio.h"
#include <utility>
#include <fstream>
#include <memory>
#include <map> 

//#include <TND280Event.hxx>
//#include <TGlobalReconModule.hxx>
//#include <TTruthVerticesModule.hxx>
//#include <TBeamSummaryDataModule.hxx>

#include "ProcessNumuFlatTree.h" 
#include "NumuFlatTree_GeneralUtils.C"   
#include "NumuFlatTree_Standard.C"
#include "NumuFlatTree_GlbAna10a.C" 
// Add yours:




//ClassImp(ND::TGlobalReconModule);
//ClassImp(ND::TTruthVerticesModule);
//ClassImp(ND::TBeamSummaryDataModule);
//

using namespace std;

//========================================================================
//
//   ProcessNumuFlatTree.C
//   ROOT macro to fill a flat TTree
//   using input from oaAnalysis TTree
//
//   Run command: 
//   root -l 'ProcessNumuFlatTree.C("tmpin.root","tmpout.root",evLow, evHigh, mode, mc, silent)'
//
//
//   ** evLow: is the first event to run (default 0)
//
//   ** evHigh: the last (default 1e6)
//
//   ** mode: Which module are run is defined by mode (default 0 ie, standard module only)
//       See "GetModeDefinition" in NumuFlatTree_GeneralUtils.C to understand
//       which mode to use if you want to run a given set of modules (other than standard).
//
//   ** mc: is 0 for data (default 1 meaning MC)
//
//   ** silent: is 0 for verbose output (default 1 meaning silent)
//
//   If you want to create a new module:
//     1) Look at NumuFlatTree_standard.C and NumuFlatTree_standard.h
//        and follow th syntax especially about name variables
//     2) Associate a Analysis number to it, and 3 or 4 letters to start your variables
//     3) Add call to your class with and if statement on the mode
//        See examples already in place by searching for "mode".
//
//
//   Current state (Feb 28th 2011): 
//      - Not fully teste with RDP3 and MCP2
//      - Need to try to make this code "compileable"
//      - Move constants to separate files (bunching, PID correction, energy reweighting)
//========================================================================


//**********************************************************
//Main method to be called from outside
//**********************************************************
void ProcessNumuFlatTree(string infile_name,string outfile_name,int evLow = 0, int evHigh = 1e6,int mode = 0, int mc = 1, int silent = 1){

  cout << "----------------------------------------------" << endl;
  cout << "  Your inputs: " << endl;
  cout << "    mode = " << mode << " See later for details."<<endl;
  cout << "    mc = " << mc << " (MC=1, Data=0)" << endl;
  cout << "    silent = " << silent <<" (1 silent, 0 verbose)" << endl;
  cout << "----------------------------------------------" << endl;

  // Call external classes

//  // Mandatory class:
  NumuFlatTree_GeneralUtils *utils=new NumuFlatTree_GeneralUtils(); // call class that have basic utils functions
//
// Call class that deal with all standard variables
// It needs the utils class.
  NumuFlatTree_Standard *std_mod=new NumuFlatTree_Standard(utils);
//
//  //Optional classes:
// This one needs the standard class
  NumuFlatTree_GlbAna10a *g10a_mod=new NumuFlatTree_GlbAna10a(std_mod);
  
  cout << "----------------------------------------------" << endl;
  utils->GetModeDefinition(mode);
  cout << "----------------------------------------------" << endl;
  
  
  p_fRandomGenerator = TRandom3(0);

  //------------------------------
  // Load some important libraries.
  //------------------------------
  
  gROOT->SetBatch(1);
  gSystem->AddIncludePath("-I$OAANALYSISROOT/src");
  gSystem->AddIncludePath("-I.");
  

  // If running with v8r5p13 then:            
  gSystem->Load("libPhysics");
  gSystem->Load("libGeom");
  gSystem->Load("libTree");
  gSystem->Load("libEG");
  gSystem->Load("libMinuit");
  gSystem->Load("libCLHEP.so");
  gSystem->Load("librecpack.so");
  gSystem->Load("liboaEvent.so");
  gSystem->Load("liboaRuntimeParameters.so");
  gSystem->Load("libMinosDBI.so");
  gSystem->Load("liboaOfflineDatabase.so");
  gSystem->Load("liboaUtility.so");
  gSystem->Load("libsmrdRecon.so");
  gSystem->Load("libBeamData.so");
  gSystem->Load("liboaBeamData.so");
  gSystem->Load("liboaRecPack.so");
  gSystem->Load("liboaAnalysis.so");

  //If Running with v7r21p1:                  
  //  gSystem->Load("libPhysics");            
  //  gSystem->Load("libGeom");               
  //  gSystem->Load("libTree");               
  //  gSystem->Load("libMatrix");             
  //  gSystem->Load("liboaEvent.so");         
  //  gSystem->Load("librecpack.so");         
  //  gSystem->Load("liboaRecPack.so");       
  //  gSystem->Load("liboaAnalysis.so");      




  //Set input parameters
  p_isMC = (bool)mc;

  TString infileName(infile_name);  
  p_silent = (bool)silent;
  
  //Create Tree objects
  TChain mcTruthVertices = TChain("TruthDir/Vertices");
  TChain mcTruthTrajectories = TChain("TruthDir/Trajectories");
  TChain reconGlobal = TChain("ReconDir/Global");
  
  //Define the analysis tree
  p_numutree = new TTree("numutree", "numutree analysis tree");

  std_mod->DefineTree(); // This is the standard module, always filled.
    // Define tha variables of your class;
    // yrs_mod.DefineTree();
  if (mode!=0) {
    if((mode & p_Module_g10a)==p_Module_g10a) { //  Global Analysis 2010 a
      g10a_mod->DefineTree();
    }
    if((mode & p_Module_bbb)==p_Module_bbb) { 
    }
    if((mode & p_Module_ccc)==p_Module_ccc) { 
    }
    if((mode & p_Module_ddd)==p_Module_ddd) { 
    }
  }


  // _____________________________________________________________________________________
  // Load data files     

  ifstream inputFile(infileName.Data(), ios::in);
  
  if(!inputFile){
    cerr << "Cannot open input file '" << fileName.Data() << "'. Exit!" << endl;
    return;
  }
  
  //Loop over the input files
  string inputString=infileName;   
  cout << "Adding file: " << inputString << endl;
  if(p_isMC){
    mcTruthVertices.AddFile(inputString.c_str());
    mcTruthTrajectories.AddFile(inputString.c_str());
  }
  reconGlobal.AddFile(inputString.c_str());
  
  
  TString outfileName(outfile_name);   
  TFile *outputFile = new TFile(outfileName, "RECREATE");
    
  // _____________________________________________________________________________________
  // Prepare to read trees.


  //For MC fill true info (need to decide if variables are defined in standard or general)
  if(p_isMC){  
    cout<<" set branches adresses "<<endl;
    for(int ii = 0; ii < p_NVTXTYPES; ii ++){
      p_TruthVertices[ii] = new TClonesArray("ND::TTruthVerticesModule::TTruthVertex", 50);
      mcTruthVertices.SetBranchAddress(p_vtxnames[ii].c_str(), &(p_TruthVertices[ii]));
      mcTruthVertices.SetBranchAddress(("N"+p_vtxnames[ii]).c_str(), &(p_NTruthVertices[ii]));
    }
    mcTruthVertices.Print("toponly");
    
    for(int ii = 0; ii < p_NTRAJTYPES; ii ++){
      p_TruthTrajs[ii] = new TClonesArray("ND::TTruthTrajectoriesModule::TTruthTrajectory", 200);
      mcTruthTrajectories.SetBranchAddress(p_trajnames[ii].c_str(), &(p_TruthTrajs[ii]));
      mcTruthTrajectories.SetBranchAddress(p_ntrajnames[ii].c_str(), &(p_NTruthTrajs[ii]));
    }
    mcTruthTrajectories.Print("toponly");
  }  

  //Set branches from global reconstructions:
    
  p_PIDs = new TClonesArray("ND::TGlobalReconModule::TGlobalPID", 20);
  p_DelayedClusters = new TClonesArray("ND::TGlobalReconModule::TFgdCluster", 10);

  reconGlobal.SetBranchAddress("NPIDs", &p_NPIDs);
  reconGlobal.SetBranchAddress("PIDs", &p_PIDs);
  reconGlobal.SetBranchAddress("NTPC", &p_NTPC);
  reconGlobal.SetBranchAddress("NTPCIso", &p_NTPCIso);
  reconGlobal.SetBranchAddress("NDsECALIso", &p_NDsECALIso);
  reconGlobal.SetBranchAddress("NTrECALIso", &p_NTrECALIso);
  reconGlobal.SetBranchAddress("NP0DIso", &p_NP0DIso);
  reconGlobal.SetBranchAddress("NSMRDIso", &p_NSMRDIso);
  reconGlobal.SetBranchAddress("NFGDIso", &p_NFGDIso);  
  reconGlobal.SetBranchAddress("NFGD1Unused", &p_NFGD1Unused);
  reconGlobal.SetBranchAddress("NFGD2Unused", &p_NFGD2Unused);
  reconGlobal.SetBranchAddress("NDelayedClusters", &p_NDelayedClusters);
  reconGlobal.SetBranchAddress("DelayedClusters", &p_DelayedClusters);    
  reconGlobal.SetBranchAddress("EarliestTrackTime", &p_EarliestTrackTime);
  reconGlobal.SetBranchAddress("EventID", &p_EventID);
  reconGlobal.SetBranchAddress("RunID", &p_RunID);

  reconGlobal.Print("toponly");


  // __________________________________________________________________________
  // Loop over all events up to evHigh.  
  int totalNbrOfEntries = reconGlobal.GetEntries();
  
  int numberEntries = reconGlobal.GetEntries();
  cout << "Number entries: " << numberEntries << endl;
  if(evHigh > numberEntries) evHigh = numberEntries;
  
  // time before event loop
  TStopwatch timer;
  timer.Start();


  //-------------------------------------------------------------
  //Start to loop over events (spill) 
  //-------------------------------------------------------------
  for(int ient = evLow; ient < evHigh; ient ++){

    if (!p_silent)    cout << "--------> begin event " << ient << endl;

    //Reset tree variables
    //---------------------
    std_mod->ResetSpillVariables(); // Always do it for standard variables
    if (mode!=0) {
      // Reset spill variables of your class;                                                        
      if((mode & p_Module_g10a)==p_Module_g10a) { 
	g10a_mod->ResetSpillVariables(); 
      }
    }
    

    // Get event entry:
    //-----------------
    if(p_isMC){
      mcTruthVertices.GetEntry(ient);    
      mcTruthTrajectories.GetEntry(ient);    
    }

    reconGlobal.GetEntry(ient);

    // Hack to get run and subrun number from file name, not optimal!!!
    string filenametoParse;    
    string ParsedRun;    
    string ParsedSubRun;    
    stringstream filenameString;
    filenameString << reconGlobal.GetFile()->GetName();
    filenametoParse = filenameString.str();
    int arxi = filenametoParse.find("oa_", 0);

    if(p_isMC){
      ParsedRun = filenametoParse.substr(arxi + 11, 8);
      ParsedSubRun = filenametoParse.substr(arxi + 20, 4);
    }
    else{
      ParsedRun = filenametoParse.substr(arxi + 10, 8);
      ParsedSubRun = filenametoParse.substr(arxi + 19, 4);
    }


    if(ient%100 == 0 )
        cout <<  "Events Processed " << ient << endl;

    // Initialize internal variables:
    //-------------------------------------
    // From standard module:
    std_mod->InitializeInternalVariables();

    // The initialize your the variables from your own modules:
    if (mode !=0 ) {
      
      if((mode & p_Module_g10a)==p_Module_g10a) { 
	 g10a_mod->InitializeInternalVariables();
      }
    }


    // Fill basic standard variables:
    //---------------------------------

    std_mod->_std_Entry = ient;
    std_mod->_std_FileRun = atoi(ParsedRun.c_str());
    std_mod->_std_FileSubrun = atoi(ParsedSubRun.c_str());

    std_mod->FillOAInfo();

    // If you need to fill some of your own variables to it now:
    if (mode !=0 ) {
      if((mode & p_Module_g10a)==p_Module_g10a) { 
      }
    }

    
    //Fill Vertex MC info for event (standard module)
    //---------------------------------------------------------

    if(p_isMC){
      std_mod->FillVertexMCInfo();
    }

    //----------------------------------------------------------------------
    // Loop over PIDs in event 
    // Done to store tracks counter per bunch and sort momentum per bunch
    //---------------------------------------------------------------------
 

    for(int j = 0; j < p_NPIDs; j ++){
      // This loop over the pids, is used to set tracks counters,
      // and to sort tracks according to their momentum,
      // all for a given bunch.
 
      ND::TGlobalReconModule::TGlobalPID *globalTrack = (ND::TGlobalReconModule::TGlobalPID*)(*p_PIDs)[j];      

      if(!globalTrack) continue; // if no global track is found, go to the next PID
  
      // --- Get the bunch number -----
      // This is based on the begining of the global track found
      // First check if track is forward or backward.

      double tTrack = 0;	
      // Forward going track      
      TLorentzVector frontPos = globalTrack->FrontPosition;
      TLorentzVector backPos = globalTrack->BackPosition;
      double frontzi = frontPos.Z();
      double backzi = backPos.Z();

      //Forwards going track
      if((globalTrack->isForward && (frontzi < backzi)) || (!globalTrack->isForward && (frontzi > backzi))){
	tTrack = frontPos.T();
      }
      // Backwards going track
      else{
	tTrack = backPos.T();
      }
      
      int ibunch = utils->GetBunch(tTrack);

      //--------- End get bunch number ------------------------

      // Make sure the momentum is not an NaN
      double mom=globalTrack->Momentum;
      if(mom != mom) mom = 0;//nan check
      if(mom == 0) continue;


      // Set some counters for tracks with TPC segment, after some set of cuts
      if((mode & p_Module_g10a)==p_Module_g10a) {
	g10a_mod->PrepareTPCTracksCounters(ibunch,j);
      }

      // Prepare the general tracks counter and sort tracks according to momentum or FGD hits
      if (std_mod->PrepareVariables(ibunch,j)) break;  // inside, if return 1, then break,
                                                      // if return 0 then eqv continue
      // Maybe should be split in two methods

    }//End loop over PIDs in event

    
    //---------------------------------------------------------------------
    //Check whether we have a true interaction in
    //the interesting detectors (FGD1, FGD2, P0D)

    bool true_in_det = false;
    for(int i = 0; i < std_mod->_std_NTrueVertices; i ++){
      int det = std_mod->_std_TrueVertexDet[i];
      if(det == 4 || det == 5 || det == 8) true_in_det = true;
    }

    //If no tracks remaining at this cut, nothing to do  
    // ---- consider also events with FGD iso tracks ------------
    if(!std_mod->std_NRecTrack_total && !p_NFGDIso && !true_in_det) continue;
      
      
    // Store ntracks info from what was done in previous PIDs loop
    std_mod->FillNTracksSpillInfo();
    
    // Prepare a few variables needed later
    std_mod->PrepareBunchWithMaxMomInSpill();
                  
    //_________________________________________________________________
    //__________________________________________________________________


    //Loop over bunches in event and fill bunch variables:
    //
    // Now: Bunch 0-->time exactly zero;1-6 real bunches but no sigma,
    // 7 huge times, 8 everything else(e.g. "very" negative times for MC)

    //_________________________________________________________________
    //__________________________________________________________________

    if (!p_silent) cout << "Enter Bunch loop " << endl;


    for(int ibunch = 0; ibunch < p_NBUNCH; ibunch ++){
  
      if(std_mod->std_NRecTrack[ibunch] > 0){ 
	std_mod->_std_NBunches ++;
	if(ibunch != 0 && ibunch < 7) std_mod->_std_NValidBunches++;
      }    

      //If no good tracks, nothing to do
      if(std_mod->std_NRecTrack[ibunch]==0 && std_mod->std_NFgdTrack[ibunch]==0 && !true_in_det) continue;


      std_mod->_std_BunchNbr ++; //same as _bunches?


      //Clear containers (but not the spill ones!)
      std_mod->ResetBunchVariables();
      // Fill standard bunch variables (Warning the method is rather messy right now)
      std_mod->FillBunchVariablesFromPIDs(ibunch);  
	  
      // Then do your own module:
      if (mode!=0) {
	if((mode & p_Module_g10a)==p_Module_g10a) { 
	  g10a_mod->ResetBunchVariables();
	  g10a_mod->FillBunchVariablesFromPIDs(ibunch);
	}
      }
	  	  
      int fillcatch = p_numutree->Fill();
	  
    }  // End loop over bunches     
  }  // End of event loop
  //------------------------------------------------------------------------  
  
  // Give processing time:
  timer.Stop();
  cout << "Processing time: " << timer.CpuTime() << " " << timer.RealTime() << endl;       
  
  //Write tree to file
  //---------------------------------------------------------------------------
  cout<<"Writing file: "<<outfileName<<endl;

  outputFile->cd();
  
  p_numutree->Write();
  
  outputFile->Close();
  
  cout<<"--------Done--------"<<endl;
}
//=======================================================================================
