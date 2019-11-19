
#include "stdio.h"
#include "iostream"
#include "vector"
#include <utility>
#include <fstream>

#include <TSystem.h>
#include <TROOT.h>
#include <TApplication.h>
#include <memory>
#include <string>
#include <TCanvas.h>
#include <TStyle.h>
#include <TF1.h>
#include <TH1.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TMultiGraph.h>
#include <TBox.h>
#include <TNtuple.h>
#include <TFile.h>
#include <TChain.h>
#include <TProfile.h>
#include <TRefArray.h>
#include <TClonesArray.h>
#include <TLegend.h>
#include <TGeoManager.h>
#include <TTree.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TStopwatch.h>
#include <TLatex.h>
#include <TString.h>
#include <TRandom.h>
#include <time.h>





//______________________________________________________________________________________


void GlobalTemplate(char *filename) {

  // file name should be a text file containing one or several root files (one per line)

  // Load some important libraries.
  
  gROOT->SetBatch(1);

  gSystem->AddIncludePath("-I$OAANALYSISROOT/src");
  gSystem->AddIncludePath("-I."); 
  
  gSystem->Load("libPhysics");
  gSystem->Load("libGeom");
  gSystem->Load("libTree");
  gSystem->Load("libMatrix");
  gSystem->Load("liboaEvent.so");
  gSystem->Load("librecpack.so");
  gSystem->Load("liboaRecPack.so");
  gSystem->Load("liboaAnalysis.so");
  
  TChain reconGlobal = TChain("ReconDir/Global");

  // _____________________________________________________________________________________
  // Load data files   
  TString fileName(filename);
   
  ifstream inputFile(fileName.Data(), ios::in);
  if (!inputFile) {
    cerr << "Cannot open input file '" << fileName.Data() << "'. Exit!" << endl;
    return;
  }
  
  string inputString;
  while (inputFile >> inputString) {
    std::cout << "Adding file: " << inputString << std::endl;
    reconGlobal.AddFile(inputString.c_str());
  }

  // _____________________________________________________________________________________
  // Prepare to read trees.

  Int_t NPIDs;
  Int_t EventID;
  Int_t RunID;
  TClonesArray *PIDs = new TClonesArray("ND::TGlobalReconModule::TGlobalPID");
  reconGlobal.SetBranchAddress("NPIDs", &NPIDs);
  reconGlobal.SetBranchAddress("PIDs", &PIDs);
  reconGlobal.SetBranchAddress("EventID", &EventID);
  reconGlobal.SetBranchAddress("RunID", &RunID);
  reconGlobal.Print("toponly");
  
  // __________________________________________________________________________
  // Loop over all events.  
  int entries = reconGlobal.GetEntries();
  std::cout << ">>>> entries : " << entries << std::endl;

  TStopwatch timer;
  timer.Start();
  
 
  //  entries=100;
  
  for(int i = 0; i < entries; i++){
    
    int Cut=0;
    
    reconGlobal.GetEntry(i);
    
    if((i)%(entries/10) == 0)
      std::cout <<  "Events Processed " << (100*i)/entries << " % " << std::endl;
    
    std::cout << "------------------------------------------------------" << std::endl;

    std::cout << "Number of tracks in the event:  " << NPIDs << std::endl;

    //global tracks            
    for( int j=0; j<NPIDs; j++) {
      
      ND::TGlobalReconModule::TGlobalPID *globalTrack = (ND::TGlobalReconModule::TGlobalPID*)(*PIDs)[j];      
      
      if( globalTrack == NULL ) continue;

      std::cout << "  -------------- " << std::endl;
      std::cout << "  Number of constituents in each detector: " << std::endl;
      std::cout << "    NTPCs:  " << globalTrack->NTPCs << std::endl;
      std::cout << "    NFGDs:  " << globalTrack->NFGDs << std::endl;
      std::cout << "    NECALs: " << globalTrack->NECALs << std::endl;
      std::cout << "    NPODs:  " << globalTrack->NP0Ds << std::endl;
      std::cout << "    NSMRDs: " << globalTrack->NSMRDs << std::endl;
      std::cout << "  Global quantities: " << std::endl;
      std::cout << "    Detectors:         " << globalTrack->Detectors << std::endl;
      std::cout << "    Momentum:          " << globalTrack->Momentum << std::endl;
      std::cout << "    Charge:            " << globalTrack->Charge << std::endl;
      std::cout << "    MomentumError:     " << globalTrack->MomentumError << std::endl;

      // TPC constituents
      std::cout << "  TPC constituents: " << std::endl;
      for( int jj=0; jj<globalTrack->NTPCs; jj++) {
	ND::TGlobalReconModule::TTPCObject *tpcTrack=(ND::TGlobalReconModule::TTPCObject*)(*(globalTrack->TPC))[jj];

	if (!tpcTrack) continue;

	std::cout << "    TPC number:         " << tpcTrack->Detector << std::endl;
	std::cout << "    TPC ndof:           " << tpcTrack->Ndof << std::endl;
	std::cout << "    TPC Momentum:       " << tpcTrack->Momentum << std::endl;
	std::cout << "    TPC Momentum error: " << tpcTrack->MomentumError << std::endl;
	std::cout << "    TPC CT:             " << tpcTrack->Ccorr << std::endl;
	std::cout << "    TPC CT expected mu: " << tpcTrack->dEdxexpMuon << std::endl;
	std::cout << "    TPC pull mu:        " << tpcTrack->PullMuon << std::endl;

      }



      // FGD constituents
      std::cout << "  FGD constituents: " << std::endl;
      for( int jj=0; jj<globalTrack->NFGDs; jj++) {
	ND::TGlobalReconModule::TFGDObject *fgdTrack=(ND::TGlobalReconModule::TFGDObject*)(*(globalTrack->FGD))[jj];

	if (!fgdTrack) continue;

	std::cout << "    FGD number:         " << fgdTrack->Detector << std::endl;
	std::cout << "    FGD ndof:           " << fgdTrack->Ndof << std::endl;

      }


    }


    
  }///end loop entries
  
  std::cout<<" End of Run "<<std::endl;


  //  TFile *f = new TFile(outputfile,"RECREATE");


  //  f->Close();
  

  return true;
}

