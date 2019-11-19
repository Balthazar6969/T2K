#include "SelectionManager.hxx"
#include "numuCCSelection.hxx"
#include "InputManager.hxx"
#include "RedoTreeConverter.hxx"

#include <unistd.h>


int main(int argc, char *argv[]){

  std::string programName = argv[0];
  std::string paramFile = "";
  int nmax = 0;
  std::string inputFileName = "";
  std::string inputFileType = "";

  for (;;) {
    int c = getopt(argc, argv, "n:");
    if (c < 0)
      break;
    switch (c) {
      case 'n': {
        std::istringstream tmp(optarg);
        tmp >> nmax;
        break;
      }
      default: {
      }
    }
  }

  // get the input file name
  inputFileName = argv[optind++];

  // Create the SelectionManager
  SelectionManager _sel;

  // Add the numuCC selection
  _sel.AddSelection("numuCC","inclusive numu CC selection", new numuCCSelection());

  // Create the InputManager
  InputManager _input;

  // add the RedTree converter (to read the "FlatTree")
  _input.AddConverter("FlatTree",       new RedoTreeConverter());

  // Initialize the InputManager by specifying the input type and the input file
  if (!_input.Initialize(inputFileName,inputFileType, false)) return false;

  // Get the number of entries in the tree
  Long64_t nentries = _input.GetEntries();  
  std::cout << "AnalysisBase::Loop(). input tree has " << nentries << " entries" << std::endl;
  std::cout << "AnalysisBase::Loop(). loop over " << nmax << std::endl;

  // Define the steps for the numuCC selection and print them
  _sel.GetSelection("numuCC")->DumpSteps();

  int npassed=0;

  //--------- Loop over entries in the tree ----------------------------------
  Long64_t entry=0;
  while (entry<nmax) {

    // Fill the spill structure for the current spill
    if (!_input.LoadEvent(entry,false)) continue;
    
    AnaEventC* event = _input.GetSuperEvent().Event;
    
    bool redo;
    
    // Apply the cuts
    int passed = (int)_sel.ApplySelection("numuCC", *event, redo);
    
    // How many events passed all cuts
    npassed += passed;        

  }
  std::cout << "N events passing all cuts: " << npassed << std::endl;
}
