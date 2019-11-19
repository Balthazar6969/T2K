#include "TFile.h"
#include "TTree.h"
#include "TList.h"
#include "TObject.h"
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <OutputManager.hxx>
#include <Header.hxx>

int main(int argc, char *argv[]){
  if (argc < 4) {
    std::cerr << "Using hadd to add a header tree to a existing highland output file" << std::endl;
    std::cerr << "This is needed for the POT counting and the software version" << std::endl;
    std::cerr << std::endl;
    std::cerr << "Usage: " << argv[0] << " file  #POT  software_version" << std::endl;
    exit(1);
  }

  // Open the file in update mode
  TFile* output = new TFile(argv[1], "UPDATE");
  std::cout << "Output file: " << argv[1] << std::endl;
  std::cout << "Adding header tree with " << argv[2] << " POT and nd280 software version " << argv[3] << std::endl;

  // Set the values into the output manager
  OutputManager outputman;
  Header header;
  header.SetPOTGoodBeamGoodND280(atof(argv[2]));
  //  header.SetSoftwareVersion((std::string)argv[3]);

  // Create the header tree and write it
  outputman.AddTreeWithName(OutputManager::header,"header");  
  Header* headerp = &header;
  outputman.GetTree(OutputManager::header)->Branch("POTInfo","Header",&headerp);  
  outputman.GetTree(OutputManager::header)->Fill();

  outputman.GetTree("header")->Write();
  output->Close();
}
