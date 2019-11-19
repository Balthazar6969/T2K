#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "TApplication.h"
#include "TEveManager.h"
#include "TEventDisplay.hxx"

using std::cout;
using std::endl;
using std::cerr;
using std::string;
/// Run event display.
void Display(std::vector<std::string> filelist, std::string geomfile) {
  TApplication* rootApplication =
      new TApplication("oaAnalysisEventDisplay", 0, 0);
  TEveManager::Create(kTRUE, "IV");
  TEventDisplay* mainWin = new TEventDisplay(filelist, geomfile);
  cout << " Starting EventDisplay " << endl;
  mainWin->StartDisplay();
  rootApplication->Run(kFALSE);
  return;
}
/// Process options for event display then run it.
int main(int argc, char** argv) {
  char* infile;
  std::string geomfile = "";
  int c;
  int err = 0;

  // Process the options
  while ((c = getopt(argc, argv, ":g:")) != -1) {
    switch (c) {
      case 'g': {
        geomfile = optarg;
        break;
      }
      case ':': {
        if (err == 0)
          cerr << endl
               << "Error - No argument specified for option" << endl
               << endl;
        err++;
      }
      case '?': {
        if (err == 0) cerr << endl << "Error - Unknown option" << endl << endl;
        err++;
      }
      default: { err++; }
    }
  }
  if (argc <= optind) {
    if (err == 0)
      cerr << endl << "Error - No input file specified" << endl << endl;
    err++;
    ;
  }
  if (err > 0) {
    cerr << "Usage: EventDisplay.exe [OPTION] OAANALYSISFILE" << endl
         << endl
         << "Possible options are:" << endl
         << "     - g = Path to a ROOT file containing the appropriate ND280 "
            "geometry.  If this is not specified a hard coded geometry will be "
            "used."
         << endl
         << endl;
    return 0;
  }

  infile = argv[optind];

  /* we can either have a single input file - in which case it has  a .root
  extension,
  or a file containing a list of files to process
  */

  std::vector<std::string> filelist;
  std::string Infile(infile);
  if (Infile.find(".root") != string::npos)
    filelist.push_back(Infile);
  else {
    ifstream files(infile);
    if (files.is_open()) {
      string filename;
      while (files.good()) {
        getline(files, filename);
        if (filename.find(".root") != string::npos)
          filelist.push_back(filename);
      }
    }
  }

  for (std::vector<std::string>::iterator file = filelist.begin();
       file != filelist.end(); ++file) {
    ifstream openfile((*file).c_str());
    if (openfile.fail()) {
      cerr << endl
           << "Unable to open an input file called:" << (*file) << ":" << endl
           << endl;
      return 0;
    }
  }

  Display(filelist, geomfile);

  return 0;
}
