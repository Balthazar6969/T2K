#define oaAnalysisTreeTools_cxx
#include "oaAnalysisTreeTools.h"

//********************************************************************
oaAnalysisTreeTools::oaAnalysisTreeTools(const std::string& infile_name):oaAnalysisTreeDefinition(infile_name){
//********************************************************************
  
  TFile outf("dummy", "RECREATE"); 

  ReadFile(infile_name);

}


//****************************************************************************
void oaAnalysisTreeTools::AddFileToTChain(const std::string& inputString){
//****************************************************************************
    
  std::cout << "Adding file: " << inputString << std::endl;

  _reconGlobal->AddFile(inputString.c_str());
  _beamSummary->AddFile(inputString.c_str());
  _DQInfo->AddFile(inputString.c_str());

  if (!_isMC_initialized){
    _reconGlobal->SetBranchAddress("RunID", &_RunID);
    _reconGlobal->GetEntry(0);
    if (_RunID == 0 || _RunID>1000000)
      _isMC = true;
    _isMC_initialized=true;
  }
  
      
  if(_isMC){
    _mcTruthVertices->AddFile(inputString.c_str());
    _mcTruthTrajectories->AddFile(inputString.c_str());
  }
  
}




