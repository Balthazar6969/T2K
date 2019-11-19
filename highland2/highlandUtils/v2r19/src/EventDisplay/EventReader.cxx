#include "EventReader.hxx"

#include <string>
#include <iomanip>
#include <iostream> 
#include <sstream>
#include <vector> 
#include <utility>
#include <cmath>
#include <algorithm>
#include <unistd.h>
#include <memory>

#include <stdexcept>


#include "Parameters.hxx"

//********************************************************************
evrd::EventReader::EventReader(int argc, char *argv[]){
  //********************************************************************
  std::string programName = argv[0];
  std::string paramFile = "";
 
  _inputFileName = "";
  _cosmicMode    = false;
  _versionCheck  = true;
  
  _event         = NULL;
  _spill         = NULL;
  _entry         = -1;
  _index_bunch   = -1;
  

  for (;;) {
    int c = getopt(argc, argv, "cvp:");
    if (c < 0)
      break;
    switch (c) {
      case 'c': {
                  _cosmicMode = true;
                  break;
                }
      case 'v': {
                  // Not used for the moment
                  _versionCheck = false;
                  break;
                }
  
      case 'p': {
                  paramFile = optarg;
                  break;
                }
 
      default: {
                 PrintUsage(programName);
               }
    }
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

  // Set up the parameter override file if it was specified
  if (paramFile != "") {
    ND::params().ReadParamOverrideFile(paramFile);
  }

  // Add Your converters here


  // Make sure no parameters have been accessed yet
  ND::params().SetReadParamOverrideFilePointPassed();

}

//********************************************************************
bool evrd::EventReader::Initialize(){
  //********************************************************************

  // Initialize the InputManager by specifying the input type and the input file
  if (!input().Initialize(_inputFileName,_inputFileType,_cosmicMode)) return false;
  
  // Read first event
  return GetFwdSpill();
}


//********************************************************************
AnaSpillB* evrd::EventReader::GetGivenSpill(Int_t eventID,  Long64_t& entry){
  //********************************************************************
  AnaSpillB* spill = NULL; 
  
  for (Long64_t i = 0; i < input().GetEntries(); i++){
    spill = static_cast<AnaSpillB*>(ReadSpill(i));
    if (!spill) continue;
    
    if (spill->EventInfo->Event == eventID){
      entry = i;
      return spill;
    }
  }
  return NULL;
}


//********************************************************************
bool evrd::EventReader::GetSpill(Int_t eventID){
  //********************************************************************
   
  AnaSpillB* spill = GetGivenSpill(eventID, _entry);

  if (!spill) return false;
  
  if (_spill) delete _spill;
  _spill = spill->Clone();
  MakeEvent();
  return true;
  
}


//********************************************************************
AnaSpillB* evrd::EventReader::GetFwdSpill(Int_t eventID,  Long64_t entry_start,  Long64_t& entry){
  //********************************************************************
  AnaSpillB* spill = NULL;
  
  if (!CheckValidEntry(entry_start))
    return spill;
  
  for (Long64_t i = entry_start; i < input().GetEntries(); i++){
    spill = static_cast<AnaSpillB*>(ReadSpill(i));
    if (!spill) continue;
    
    if (spill->EventInfo->Event != eventID){
      entry = i;
      return spill;    
    }
  }
  return NULL;
}

//********************************************************************
bool evrd::EventReader::GetFwdSpill(){
  //********************************************************************

  Int_t eventID = -1;
  if (_spill) eventID = _spill->EventInfo->Event;
  AnaSpillB* spill = GetFwdSpill(eventID, _entry + 1, _entry);
  
  if (!spill) return false;
  
  if (_spill) delete _spill;
  _spill = spill->Clone();
  MakeEvent();
  return true;

}



//********************************************************************
AnaSpillB* evrd::EventReader::GetBwdSpill(Int_t eventID, Long64_t entry_start,  Long64_t& entry){
  //********************************************************************
  AnaSpillB* spill = NULL; 
  
  if (!CheckValidEntry(entry_start))
    return spill;
  
  Int_t eventID_tmp = -1;
  bool ok = false;
  Long64_t entry_tmp1 = -1;
  Long64_t entry_tmp2 = -1;
  
  for (Long64_t i = entry_start; i >= 0; i--){
    spill = static_cast<AnaSpillB*>(ReadSpill(i));
    if (!spill) continue;
   
     ok = spill->EventInfo->Event != eventID;
     
     if (ok){
       entry_tmp1 = i;
       eventID_tmp = spill->EventInfo->Event;
       break;
     }  
  }
  
  if (!ok) return NULL;

  ok = false;
  
  // Another loop until the event changes again
  for (Long64_t i = entry_tmp1 + 1; i >= 0; i--){
    spill = static_cast<AnaSpillB*>(ReadSpill(i));
    if (!spill) continue;
   
    ok = spill->EventInfo->Event != eventID_tmp;
     
    if (ok){
      entry_tmp2 = i - 1;
      break; 
    }  
  }
  
  if (!ok) return NULL;
 
  entry = entry_tmp2;
  spill = ReadSpill(entry);
  return spill;  
} 

//********************************************************************
bool evrd::EventReader::GetBwdSpill(){
  //********************************************************************

  if (!_spill) return false;
  Int_t eventID = _spill->EventInfo->Event;
  AnaSpillB* spill = GetBwdSpill(eventID, _entry - 1, _entry);
  
  if (!spill) return false;
  
  if (_spill) delete _spill;
  _spill = spill->Clone();
  MakeEvent();
  return true;

}

//********************************************************************
AnaSpillB* evrd::EventReader::ReadSpill(Long64_t entry){
  //********************************************************************

  // Read a valid spill with information available
  
  AnaSpillB* spill = NULL;
 
  if (!CheckValidEntry(entry)) return spill;
   
  // Fill the spill structure for the current spill from the input tree
  // The previous succesfully read spill is deleted internally if the current spill is OK
  bool spillOK = input().LoadSpill(entry);
  
  if (!spillOK) return spill;
  
  spill = static_cast<AnaSpillB*>(&input().GetCorrectedSpill());
  
  if (spill->Bunches.size() == 0 && !spill->OutOfBunch) return NULL;
   
  if (spill->Bunches.size() > 0)
    _index_bunch = 0;
  else 
    _index_bunch = OutBunchIndex;
  
  return spill;
  
}

//********************************************************************
bool evrd::EventReader::ChangeBunch(bool forward){
  //********************************************************************
  if (!_spill) return false;
  
  Int_t index = _index_bunch;
  
  index += forward ? 1 : -1;
  
  if (_index_bunch == OutBunchIndex)
    index = forward ? 0 : _spill->Bunches.size() - 1;

  // Try to read
  try {
    _spill->Bunches.at(index);      // vector::at throws an out-of-range
  }
  catch (const std::out_of_range& oor) {
    if (_spill->OutOfBunch)
      index = OutBunchIndex;
    else
      return false;
  }
   _index_bunch = index;
  MakeEvent();
  return true;
}

//********************************************************************
void evrd::EventReader::MakeEvent(){
  //********************************************************************
  if (!_spill) return;
 
  if (_event) delete _event;
  _event = NULL;

  if (_index_bunch == OutBunchIndex)
    _event = new AnaEventB(*_spill, *(static_cast<AnaBunchB*>(_spill->OutOfBunch)));
  else if (_index_bunch > -1 && _index_bunch < (Int_t)_spill->Bunches.size())
    _event = new AnaEventB(*_spill, *(static_cast<AnaBunchB*>(_spill->Bunches.at(_index_bunch))));
}

//********************************************************************
void evrd::EventReader::PrintUsage(const std::string& programName){
  //********************************************************************
  std::cout << "usage: " << programName << " [options] [input-file]" << std::endl;
  std::cout << std::endl;
  std::cout << "Options:" << std::endl;
  std::cout << "    -c                Run in cosmics mode, putting all tracks in one 'bunch'" << std::endl;
  std::cout << "    -p <file>         Set the name of a parameter override file" << std::endl;
  std::cout << "    -v                Don't Check version compatibility between oaAnalysisReader and oaAnalysis file (not used for the moment)" << std::endl;
  std::exit(1);
}

