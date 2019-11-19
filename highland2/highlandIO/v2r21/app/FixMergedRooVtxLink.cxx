#include "TFile.h"
#include "TTree.h"
#include "TList.h"
#include "TObject.h"
#include <vector>
#include <iostream>
#include <stdlib.h>

/// Using hadd to merge highland files breaks the link between the default/truth trees and
/// the NRooTrackerVtx tree. This script fixes that. As merging ROOT files is incredibly
/// tricky, you should use hadd to do the actual merging. Then use this script to fix the
/// links.
int main(int argc, char *argv[]){
  if (argc < 4) {
    std::cerr << "Using hadd to merge highland files breaks the link between the default/truth trees and" << std::endl;
    std::cerr << "the NRooTrackerVtx tree. This script fixes that. As merging ROOT files is incredibly" << std::endl;
    std::cerr << "tricky, you should use hadd to do the actual merging. Then use this script to fix the" << std::endl;
    std::cerr << "links." << std::endl;
    std::cerr << std::endl;
    std::cerr << "Usage: " << argv[0] << " output_file merged_input_file input_file1 [input_file2] [...] " << std::endl;
    exit(1);
  }
  ////////////////////////////////////////////////////////////////////////
  /////       Set which SYSTEMATIC TREES here                     ////////
  // (would be nicer pass this as argument)
  std::string syst = "all_syst";
  ////////////////////////////////////////////////////////////////////////
 
  TFile* output = new TFile(argv[1], "RECREATE");
  std::cout << "Output file: " << argv[1] << std::endl;
  TFile* merged = new TFile(argv[2], "READ");
  std::cout << "Merged input file: " << argv[2] << std::endl;

  TTree* oldsyst = (TTree*)merged->Get(syst.c_str());
  TTree* olddflt = (TTree*)merged->Get("default");
  TTree* oldtruth = (TTree*)merged->Get("truth");
  TTree* oldroo = (TTree*)merged->Get("NRooTrackerVtx");
  if(!oldroo) oldroo = (TTree*)merged->Get("GRooTrackerVtx");
  TTree* oldcnfg = (TTree*)merged->Get("config");
  TTree* oldhdr = (TTree*)merged->Get("header");

  std::cout << "Cloning header, config and NRooTrackerVtx trees" << std::endl;
  output->cd();
  TTree* newsyst = oldsyst->CloneTree(0);
  TTree* newdflt = olddflt->CloneTree(0);
  TTree* newtruth = oldtruth->CloneTree(0);
  TTree* newroo = oldroo->CloneTree();
  TTree* newcnfg = oldcnfg->CloneTree();
  TTree* newhdr = oldhdr->CloneTree();
  (void)newroo;
  (void)newcnfg;
  (void)newhdr;

  int oldrooidxdflt = 0;
  int newrooidxdflt = 0;
  int oldrooidxtruth = 0;
  int newrooidxtruth = 0;

  olddflt->SetBranchAddress("RooVtxEntry", &oldrooidxdflt);
  newdflt->SetBranchAddress("RooVtxEntry", &newrooidxdflt);
  oldtruth->SetBranchAddress("RooVtxEntry", &oldrooidxtruth);
  newtruth->SetBranchAddress("RooVtxEntry", &newrooidxtruth);

  int offset = 0;
  int dflttot = 0;
  int truthtot = 0;

 // Need to know the number of toys (NTOYS variable in the microtrees)
  const int nbr_toys = 500;
  int oldrooidxsyst[nbr_toys];
  int newrooidxsyst[nbr_toys];
  oldsyst->SetBranchAddress("RooVtxEntry", &oldrooidxsyst);
  newsyst->SetBranchAddress("RooVtxEntry", &newrooidxsyst);
  int systtot = 0;

  std::cout << "Fixing links in default and truth trees" << std::endl;

  for (int i = 3; i < argc; i++) {
    TFile* input = new TFile(argv[i], "READ");

    TTree* thisdflt = (TTree*)input->Get("default");
    TTree* thistruth = (TTree*)input->Get("truth");
    TTree* thissyst = (TTree*)input->Get(syst.c_str());
    TTree* thisroo = (TTree*)input->Get("NRooTrackerVtx");
    if(!thisroo) thisroo = (TTree*)input->Get("GRooTrackerVtx");

    std::cout << "Unmerged input file: " << argv[i] << std::endl;

    output->cd();

    for (int i = 0; i < thisdflt->GetEntries(); i++, dflttot++) {
      olddflt->GetEntry(dflttot);
      if (oldrooidxdflt >= 0) {
        newrooidxdflt = oldrooidxdflt + offset;
      }
      newdflt->Fill();
    }

    for (int i = 0; i < thistruth->GetEntries(); i++, truthtot++) {
      oldtruth->GetEntry(truthtot);
      if (oldrooidxtruth >= 0) {
        newrooidxtruth = oldrooidxtruth + offset;
      }
      newtruth->Fill();
    }

    for (int i = 0; i < thissyst->GetEntries(); i++, systtot++) {
      oldsyst->GetEntry(systtot);
      if (oldrooidxsyst[0] >= 0) { // doesn't matter here, there is a unique RooVtxIndex for all the toys
        for(int j=0; j<nbr_toys;j++){
          newrooidxsyst[j] = oldrooidxsyst[j] + offset;
        }
      }
      newsyst->Fill();
    }

    offset += thisroo->GetEntries();

    input->Close();
    delete input;
  }

  std::cout << "Writing new trees to: " << argv[1] << std::endl;
  output->Write(0, TObject::kWriteDelete);
  output->Close();
}
