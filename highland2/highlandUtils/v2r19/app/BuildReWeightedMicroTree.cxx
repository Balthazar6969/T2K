#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <cstdlib>


#include "TFile.h"
#include "TTree.h"
#include "TArrayF.h"
#include "TClonesArray.h"
#include "TStopwatch.h"

// This application clones a micro-tree file adding an external weight from a root file created by xsTool
// The new weights are stored into the vector of floats ext_weight, with size NEXTWEIGHTS. If you run the application
// over a file that already contains external weights, the new weight will be added to the vector

// The user should provide a reweight file ...

void PrintUsage(const std::string& programName);

void CopyWeights(TTree* weights_tree, TTree* old_tree, TTree* new_tree);

//*************************************************************
int main(int argc, char* argv[]) {
//*************************************************************

  std::string programName = argv[0];
  std::string paramFile = "";

  std::string _outputFileName = "";
  std::string _inputFileName = "";
  std::string _inputWeightFileName = "";
  std::string _inputTruthWeightFileName = "";

  Int_t _entry_nmax;
  Int_t _entry_imin;
  
  for (;;) {
    Int_t c = getopt(argc, argv, "d:t:n:s:o:");
    if (c < 0)
      break;
    switch (c) {
      case 'n': {
        std::istringstream tmp(optarg);
        tmp >> _entry_nmax;
        break;
      }
      case 'o': {
        _outputFileName = optarg;
        break;
      }
      case 'd': {
        _inputWeightFileName = optarg;
        break;
      }
      case 't': {
        _inputTruthWeightFileName = optarg;
        break;
      }
      case 's': {
        std::istringstream tmp(optarg);
        tmp >> _entry_imin;
        break;
      }
      default: {
        PrintUsage(programName);
      }
    }
  }

  // Check that an output file was specified
  if (_outputFileName == "") {
    std::cerr << "ERROR: No output file" << std::endl << std::endl;
    PrintUsage(programName);
  }

  // Check that there is a default weight file
  if (_inputWeightFileName == "") {
    std::cerr << "ERROR: No truth weight file" << std::endl << std::endl;
    PrintUsage(programName);
  }

    // Check that there is a truth weight file
  if (_inputTruthWeightFileName == "") {
    std::cerr << "ERROR: No default weight file" << std::endl << std::endl;
    PrintUsage(programName);
  }

  
  // Check that there is an input file.
  if (argc <= optind) {
    std::cerr << "ERROR: No input file" << std::endl << std::endl;
    PrintUsage(programName);
  } else if (argc != optind + 1) {
    std::cerr << "ERROR: Only one input file may be specified. To process multiple ROOT files, " << std::endl
              << "       list them in a text file, and specify that text file as the input." << std::endl << std::endl;
    PrintUsage(programName);
  }

  _inputFileName = argv[optind++];


  const char* old_MT_name = _inputFileName.c_str();
  const char* rw_file_default_name = _inputWeightFileName.c_str();
  const char* rw_file_truth_name = _inputTruthWeightFileName.c_str();
  const char* new_MT_name = _outputFileName.c_str();


  bool debug = true;
  //Get old default and truth trees and weights for each of them.
  TFile *old_MT = new TFile(old_MT_name,"READ");
  if (!old_MT) std::cout<<"ERROR: Can't find the microTree file !"<<std::endl;
  TTree *olddefault = (TTree*)old_MT->Get("default");
  if (!olddefault) std::cout<<"ERROR: No default tree in the microTree !"<<std::endl;
  TTree *oldtruth = (TTree*)old_MT->Get("truth");
  if (!oldtruth) std::cout<<"ERROR: No truth tree in the microTree !"<<std::endl;

  TFile *weights_default_file = new TFile(rw_file_default_name,"READ");
  if (!weights_default_file) std::cout<<"ERROR: Can't find the default weight file !"<<std::endl;
  TTree *weights_default = (TTree*)weights_default_file->Get("weights_file");
  if (!weights_default) std::cout<<"ERROR: No weight_file tree in the default weight file!"<<std::endl;

  // Check nbr of entry matches
  UInt_t nbr_default_evts = olddefault->GetEntries();
  if(debug) std::cout<<"N events in default = "<< nbr_default_evts <<std::endl;
  UInt_t nbr_default_weights = weights_default->GetEntries();
  if(debug) std::cout<<"N events in default weight = "<< nbr_default_weights <<std::endl;
  if(nbr_default_evts != nbr_default_weights) std::cout<<"ERROR: Nbr of entries in default doesn't match the nbr of weigths"<<std::endl;

  TFile *weights_truth_file = new TFile(rw_file_truth_name,"READ");
  if (!weights_truth_file) std::cout<<"ERROR: Can't find the truth weight file !"<<std::endl;
  TTree *weights_truth = (TTree*)weights_truth_file->Get("weights_file");
  if (!weights_truth) std::cout<<"ERROR: No weight_file tree in the truth weight file!"<<std::endl;

  UInt_t nbr_truth_evts = oldtruth->GetEntries();
  if(debug) std::cout<<"N events in truth = "<< nbr_truth_evts <<std::endl;
  UInt_t nbr_truth_weights = weights_truth->GetEntries();
  if(debug) std::cout<<"N events in truth weight = "<< nbr_truth_weights <<std::endl;
  if(nbr_truth_evts != nbr_truth_weights) std::cout<<"ERROR: Nbr of entries in truth doesn't match the nbr of weigths"<<std::endl;

  // -------------- Get the other trees -----------------------
  TTree *oldNRoo = (TTree*)old_MT->Get("NRooTrackerVtx");
  TTree *oldconf = (TTree*)old_MT->Get("config");
  TTree *oldhead = (TTree*)old_MT->Get("header");

  // -------- clone all the branches but the weights of all trees
  olddefault->SetBranchStatus("*",1);
  olddefault->SetBranchStatus("ext_weight",0);
  olddefault->SetBranchStatus("NEXTWEIGHTS",0);

  oldtruth->SetBranchStatus("*",1);
  oldtruth->SetBranchStatus("ext_weight",0);
  oldtruth->SetBranchStatus("NEXTWEIGHTS",0);

  oldNRoo->SetBranchStatus("*",1);
  oldconf->SetBranchStatus("*",1);
  oldhead->SetBranchStatus("*",1);

  // define new Microtree
  TFile *new_MT = new TFile(new_MT_name,"RECREATE");

  // fill the new trees without the weights
  std::cout<<"Copy default Tree"<<std::endl;
  TTree *newdefault = olddefault->CloneTree(0);
  newdefault->CopyEntries(olddefault);

  std::cout<<"Copy truth Tree"<<std::endl;
  TTree *newtruth = oldtruth->CloneTree(0);
  newtruth->CopyEntries(oldtruth);

  std::cout<<"Copy NRoo Tree"<<std::endl;
  TTree *newNRoo = oldNRoo->CloneTree(0);
  newNRoo->CopyEntries(oldNRoo);

  std::cout<<"Copy conf Tree"<<std::endl;
  TTree *newconf = oldconf->CloneTree(0);
  newconf->CopyEntries(oldconf);

  std::cout<<"Copy head Tree"<<std::endl;
  TTree *newhead = oldhead->CloneTree(0);
  newhead->CopyEntries(oldhead);

  //------------------ copy weights -------------------------
  
  CopyWeights(weights_default, olddefault, newdefault);
  CopyWeights(weights_truth,   oldtruth,   newtruth);


  //---------------------------------------------
  
  newdefault->Write();
  newtruth->Write();
  //newmass->Write();
  newNRoo->Write();
  newconf->Write();
  newhead->Write();

  delete old_MT;
  delete new_MT;
}

//*****************************************************
void PrintUsage(const std::string& programName) {
//*****************************************************

  std::cout << "usage: " << programName << " [options] [input-file]" << std::endl;
  std::cout << std::endl;
  std::cout << "Options:" << std::endl;
  std::cout << "    -o <file>         Set the name of an output file (mandatory)" << std::endl;
  std::cout << "    -n <cnt>          Only read <cnt> events" << std::endl;
  std::cout << "    -s <cnt>          Skip <cnt> events" << std::endl;
  std::cout << "    -w <file>         Set the name of file containing the weights" << std::endl;
  std::exit(1);
}


//*****************************************************
void CopyWeights(TTree* weights_tree, TTree* old_tree, TTree* new_tree) {
//*****************************************************


  //------------------ Truth weights -------------------------

  // get the weights in the weight file
  TArrayF * farrayt = NULL;
  weights_tree->SetBranchAddress("weights",&farrayt);

  //define the new weights
  Int_t NEXTWEIGHTS=1;
  TBranch *br_NEXTWEIGHTS = new_tree->Branch("NEXTWEIGHTS", &NEXTWEIGHTS, "NEXTWEIGHTS/I");
  Float_t ext_weight[10];
  TBranch *br_ext_weight  = new_tree->Branch("ext_weight",  &ext_weight, "ext_weight[NEXTWEIGHTS]/F");

  old_tree->SetBranchStatus("NEXTWEIGHTS",1);
  Int_t old_NEXTWEIGHTS;
  old_tree->SetBranchAddress("NEXTWEIGHTS",&old_NEXTWEIGHTS);
  
  old_tree->SetBranchStatus("ext_weight",1);
  Float_t old_ext_weight[10];
  old_tree->SetBranchAddress("ext_weight",&old_ext_weight);

  UInt_t nbr_events = weights_tree->GetEntries();
 
  //Get new weights 
  for(UInt_t i=0;i<nbr_events;i++){
    old_tree->GetEntry(i);
    weights_tree->GetEntry(i);
    new_tree->GetEntry(i);

    for (Int_t j=0;j<old_NEXTWEIGHTS;j++)
      ext_weight[j]=old_ext_weight[j];

    NEXTWEIGHTS = old_NEXTWEIGHTS+1;
    ext_weight[old_NEXTWEIGHTS]=farrayt->At(0);

    br_NEXTWEIGHTS->Fill();
    br_ext_weight->Fill();
  }
}
