#define NumuFlatTreeTools_cxx
#include "NumuFlatTreeTools.h"

//********************************************************************
NumuFlatTreeTools::NumuFlatTreeTools(const std::string& infile_name){
//********************************************************************

  TFile outf("dummy", "RECREATE");   

  TChain* c = new TChain("numutree");
  Init(c);
  ReadFile(infile_name);


}

//****************************************************************************
void NumuFlatTreeTools::AddFileToTChain(const std::string& inputString){
//****************************************************************************
    
  std::cout << "Adding file: " << inputString << std::endl;
  (dynamic_cast<TChain*>(fChain))->AddFile(inputString.c_str());
  
}



