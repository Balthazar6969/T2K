#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>

#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TStopwatch.h"

#include "ND__NRooTrackerVtx.h"

//#define __DEBUG__

bool checkDefaultTree = false;
bool checkTruthTree = false;
bool checkSystTree = false;
std::string microTreeFileName;

int count_skipped_events = 0;   // counter for skipped (= fake inserted) events

void checkTree(TFile* microTreeFile, TTree* microTree);
void usage();


int main(int argc, char* argv[]) {
  TStopwatch* totalStopwatch = new TStopwatch();
  totalStopwatch->Start(kTRUE);

  std::cout.precision(5);
  std::cout.setf(std::ios_base::scientific, std::ios_base::floatfield);
  std::cout.setf(std::ios_base::showpoint);

  std::string programName(argv[0]);
  int c;
  opterr = 0;
  
  // Pass arguments
  while ((c = getopt(argc, argv, "hdtsl:m:")) != - 1) {
    if (c == 'h'){
	  usage();
	  return 0;
    }
    if (c == 'd')
      checkDefaultTree = true;
    else if (c == 'm')
      microTreeFileName.assign(optarg);
    else if (c == 't')
      checkTruthTree = true;
    else if (c == 's')
      checkSystTree = true;
    else if (c == '?') {
      if (optopt == 'm') {
        std::cerr << "ERROR: Option \"-" << ((char) optopt) << "\" requires an argument!" << std::endl;
        usage();
        exit(1);
      }
      else if (std::isprint(optopt)) {
        std::cerr << "ERROR: Unknown option \"-" << ((char) optopt) << "\"!" << std::endl;
        usage();
        exit(1);
      }
      else {
        std::cerr << "ERROR: Invalid option string!" << std::endl;
        usage();
        exit(1);
      }
    }
  }

  if (microTreeFileName.empty()) {
    std::cerr << "ERROR: Please specify microTree file!" << std::endl;
    usage();
    exit(1);
  }

//
  std::cout << "Starting to check " << microTreeFileName << std::endl;
  std::cout << std::endl;
  
  TFile* microTreeFile = new TFile(microTreeFileName.c_str());
  if (!microTreeFile) {
    std::cerr << "ERROR: Cannot open microTree file " << microTreeFileName << '!' << std::endl;
    usage();
    exit(1);
  }
  TTree* microDefaultTree = NULL;
  if (checkDefaultTree) {
    microDefaultTree = (TTree*) microTreeFile->Get("default");
    if (!microDefaultTree) {
      std::cerr << "ERROR: Cannot find default tree in " << microTreeFileName << '!' << std::endl;
      usage();
      exit(1);
    }
  }
  TTree* microSystTree = NULL;
  if (checkSystTree) {
    microSystTree = (TTree*) microTreeFile->Get("all_syst");
    if (!microSystTree) {
      std::cerr << "ERROR: Cannot find all_syst tree in " << microTreeFileName << '!' << std::endl;
      usage();
      exit(1);
    }
  }
  TTree* microTruthTree = NULL;
  if (checkTruthTree) {
    microTruthTree = (TTree*) microTreeFile->Get("truth");
    if (!microTruthTree) {
      std::cerr << "ERROR: Cannot find truth tree in " << microTreeFileName << '!' << std::endl;
      usage();
      exit(1);
    }
  }

// start timer
  std::cout << "Done.\tElapsed time:\tCPU: " << totalStopwatch->CpuTime() << " s\treal: "
            << totalStopwatch->RealTime() << " s\tRatio: "
            << totalStopwatch->CpuTime() / totalStopwatch->RealTime() << std::endl;
  totalStopwatch->Continue();
  std::cout << std::endl;


  if (checkDefaultTree) {
    std::cout << "Checking link between NRooTrackerVtx and default tree with  " << microDefaultTree->GetEntries() << " entries ..." << std::endl;
    checkTree(microTreeFile, microDefaultTree);
  }
  if (checkSystTree) {
    std::cout << "Checking link between NRooTrackerVtx and all_syst tree with  " << microSystTree->GetEntries() << " entries ..." << std::endl;
    checkTree(microTreeFile, microSystTree);
  }

  if (checkTruthTree) {
    std::cout << "Checking link between NRooTrackerVtx and truth tree with  " << microTruthTree->GetEntries() << " entries ..." << std::endl;
    checkTree(microTreeFile, microTruthTree);
  }


  std::cout << "Total elapsed time:\tCPU: " << totalStopwatch->CpuTime() << " s\treal: "
            << totalStopwatch->RealTime() << " s\tRatio: "
            << totalStopwatch->CpuTime() / totalStopwatch->RealTime() << std::endl;
  std::cout << std::endl;


  return 0;
}


void checkTree(TFile* microTreeFile, TTree* microTree) {
  TStopwatch* checkTreeStopwatch = new TStopwatch();
  checkTreeStopwatch->Start(kTRUE);

#ifdef __DEBUG__
  const int nDbgStopwatches = 3;
  TStopwatch* dbgStopwatch[nDbgStopwatches];
  for (int i = 0; i < nDbgStopwatches; i++) {
    dbgStopwatch[i] = new TStopwatch();
    dbgStopwatch[i]->Reset();
  }
#endif

  int microTreeRunID;
  int microTreeSubrunID;
  int microTreeEventID;
  int microTreeToyIndex[1000];
  int microTreeTruthVertexID[1000];
  int microTreeRooVtxIndex[1000];
  int microTreeRooVtxEntry[1000];
  int MCRunID;
  int MCSubrunID;
  int MCEventID;
  int MCNVtx;
  int progress = 0;
  int NTOYS=1;

  const int nMicroTreeEvents = microTree->GetEntries();
  std::string currentMCFileName;
  TFile *MCFile = NULL;
  TTree *MCTree = NULL; // MicroTree file NRooTrackerVtx tree
  TClonesArray *MCNRooVtxs = new TClonesArray("ND::NRooTrackerVtx", 100);
  MCTree = (TTree*) microTreeFile->Get("NRooTrackerVtx");


  microTree->SetBranchStatus("*", 0);
  microTree->SetBranchStatus("run", 1);
  microTree->SetBranchStatus("subrun", 1);
  microTree->SetBranchStatus("evt", 1);
  microTree->SetBranchAddress("run", &microTreeRunID);
  microTree->SetBranchAddress("subrun", &microTreeSubrunID);
  microTree->SetBranchAddress("evt", &microTreeEventID);

  if (strcmp(microTree->GetName(), "truth")){
    microTree->SetBranchStatus("toy_index", 1);
    microTree->SetBranchStatus("NTOYS", 1);
    microTree->SetBranchAddress("toy_index", microTreeToyIndex);
    microTree->SetBranchAddress("NTOYS", &NTOYS);
  }

  microTree->SetBranchStatus("TruthVertexID", 1);
  microTree->SetBranchStatus("RooVtxIndex", 1);
  microTree->SetBranchStatus("RooVtxEntry", 1);
  microTree->SetBranchAddress("TruthVertexID", microTreeTruthVertexID);
  microTree->SetBranchAddress("RooVtxIndex", microTreeRooVtxIndex);
  microTree->SetBranchAddress("RooVtxEntry", microTreeRooVtxEntry);
	  
  MCTree->SetBranchStatus("Preselected", 0);
  MCTree->SetBranchAddress("RunID", &MCRunID);
  MCTree->SetBranchAddress("SubrunID", &MCSubrunID);
  MCTree->SetBranchAddress("EventID", &MCEventID);
  MCTree->SetBranchAddress("NVtx", &MCNVtx);
  MCTree->SetBranchAddress("Vtx", &MCNRooVtxs);
  
  for (int ientry = 0; ientry < nMicroTreeEvents; ientry++) {
    microTree->GetEntry(ientry);

    // print progress
    std::cout << ientry << '/' << nMicroTreeEvents << '\r' << std::flush;
    if (((10 * ientry) / nMicroTreeEvents) > progress) {
        progress = (10 * ientry) / nMicroTreeEvents;
#ifdef __DEBUG__
        std::cout << std::endl;
        std::cout << (10 * progress) << '%'
                  << "\tElapsed time:\tCPU: " << checkTreeStopwatch->CpuTime() << " s\treal: "
                  << checkTreeStopwatch->RealTime() << " s\tRatio: "
                  << checkTreeStopwatch->CpuTime() / checkTreeStopwatch->RealTime() << std::endl;
#endif
        checkTreeStopwatch->Continue();

#ifdef __DEBUG__
        std::cout << "Elapsed time to open MC files:\tCPU: " << dbgStopwatch[0]->CpuTime() << " s\treal: "
                  << dbgStopwatch[0]->RealTime() << " s\tRatio: "
                  << dbgStopwatch[0]->CpuTime() / dbgStopwatch[0]->RealTime() << std::endl;
        std::cout << "Elapsed time to get events:   \tCPU: " << dbgStopwatch[1]->CpuTime() << " s\treal: "
                  << dbgStopwatch[1]->RealTime() << " s\tRatio: "
                  << dbgStopwatch[1]->CpuTime() / dbgStopwatch[1]->RealTime() << std::endl;
        std::cout << "Elapsed time to save events:  \tCPU: " << dbgStopwatch[2]->CpuTime() << " s\treal: "
                  << dbgStopwatch[2]->RealTime() << " s\tRatio: "
                  << dbgStopwatch[2]->CpuTime() / dbgStopwatch[2]->RealTime() << std::endl;
#endif
    }

#ifdef __DEBUG__
    dbgStopwatch[1]->Start(kFALSE);
#endif
 
      // truth match found, loop through RooTracker entries from the last matched event until the
      // right event is found
      for (Int_t itoy=0;itoy<NTOYS;itoy++){
        // This toy is not filled becouse it does to reach the min accum level to save
        if (microTreeRooVtxEntry[itoy]==-999 && microTreeToyIndex[itoy]==-999) continue;

        // This toy is not filled becouse it did not reach the accum level necessary to have a true vertex asociated
        if (microTreeRooVtxEntry[itoy]==-999 && microTreeTruthVertexID[itoy]==-999) continue;

        // The toy is filled but does not have a link
        if (microTreeRooVtxEntry[itoy]==-999 && microTreeTruthVertexID[itoy]!=-999 && microTreeToyIndex[itoy]!=-999){
          std::cout << "Event not truth matched (RooVtxEntry == -999) for entry " << ientry << " and toy " << itoy << std::endl;	 
          std::cout << "RunID " << microTreeRunID << ", SubrunID " << microTreeSubrunID << ", EventID " << microTreeEventID << std::endl;	 
          count_skipped_events++;     // increase counter

          continue;
        }
        
        MCTree->GetEntry(microTreeRooVtxEntry[itoy]);
        
        // There is a link but to the wrong entry
        if(!(microTreeEventID == MCEventID && microTreeRunID == MCRunID && microTreeSubrunID == MCSubrunID)) {
          std::cout << "*** Error: Incorrect matching for entry " << ientry << " and toy " << itoy << std::endl;
          std::cout << "    microTree:     RunID " << microTreeRunID << ", SubrunID " << microTreeSubrunID << ", EventID " << microTreeEventID << std::endl;
          std::cout << "    RooTrackerVtx: RunID " << MCRunID        << ", SubrunID " << MCSubrunID        << ", EventID " << MCEventID        << std::endl;
          count_skipped_events++;     // increase counter
        }else if (microTreeRooVtxIndex[itoy]!=-999){
          ND::NRooTrackerVtx *vertex = (ND::NRooTrackerVtx*) (*MCNRooVtxs)[microTreeRooVtxIndex[itoy]];
          if (vertex->TruthVertexID != microTreeTruthVertexID[itoy]){
            std::cout << "*** Error: Incorrect matching for entry " << ientry << " and toy " << itoy << std::endl;
            std::cout << "    microTree:     RunID " << microTreeRunID << ", SubrunID " << microTreeSubrunID << ", EventID " << microTreeEventID << " " << vertex->TruthVertexID << std::endl;
            std::cout << "    RooTrackerVtx: RunID " << MCRunID        << ", SubrunID " << MCSubrunID        << ", EventID " << MCEventID        << " " <<  microTreeTruthVertexID[itoy] << std::endl;
          }                  
        }


      }


#ifdef __DEBUG__
    dbgStopwatch[2]->Stop();
#endif
  }


  if(MCTree != NULL) delete MCTree;
  if(MCFile != NULL) {
    MCFile->Close();
    delete MCFile;
  }
  delete MCNRooVtxs;

#ifdef __DEBUG__
  for(int i = 0; i < nDbgStopwatches; i++) delete dbgStopwatch[i];

  std::cout << std::endl;
  std::cout << "Done.\tElapsed time:\tCPU: " << checkTreeStopwatch->CpuTime() << " s\treal: "
            << checkTreeStopwatch->RealTime() << " s\tRatio: "
            << checkTreeStopwatch->CpuTime() / checkTreeStopwatch->RealTime() << std::endl;
  std::cout << std::endl;
#endif

  delete checkTreeStopwatch;
  
  // print number of generated fake entries
  if(count_skipped_events > 0) {
    std::cout << "*******************************************************************" << std::endl;
    std::cout << "**  WARNING: Some events were skipped due to missing RooTracker" << std::endl;
    std::cout << "**           information in the input micro tree. A few skipped" << std::endl;
    std::cout << "**           are ok, as they just will get a weight of 1." << std::endl;
    double p = (double)count_skipped_events/(double)nMicroTreeEvents * 100.;
    std::cout << "**  Number of skipped events: " << count_skipped_events << " (" << TString::Format("%5.2f",p) << "%)" << std::endl;
    if(p > 0.1) std::cout << "**  THIS IS EXCEEDING THE THRESHOLD (0.5%), GENERATING AN ERROR" << std::endl;
    std::cout << "*******************************************************************" << std::endl;
    if(p > 0.1) exit(1);
  }
  count_skipped_events = 0; // reset to 0 as we may process another tree

}

void usage() {
  std::cout << "This program copies checks the link between micro-trees and RooTrackerVtx tree" << std::endl;
  std::cout << "Usage:" << std::endl;
  std::cout << "\tMandatory arguments:" << std::endl;
  std::cout << "\t-m /path/to/microTree" << std::endl;
  std::cout << "\tOptional arguments:" << std::endl;
  std::cout << "\t-d: process default tree in microTree file" << std::endl;
  std::cout << "\t-t: process truth tree in microTree file" << std::endl;
  std::cout << "\t-s: process all_syst tree in microTree file" << std::endl;
  std::cout << "\t-h: print this information and exit" << std::endl;

}
