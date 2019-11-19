#ifndef TreeToolsBase_h
#define TreeToolsBase_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include "TRandom3.h"
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <iostream>


class TreeToolsBase{
 public :
  
  TreeToolsBase(){}

  virtual void AddFileToTChain(const std::string& inputString) = 0;

  void ReadFile(const std::string& infile_name){
    // _____________________________________________________________________________________
    // Load data files     
    TString infileName(infile_name);  
    ifstream inputFile(infileName.Data(), ios::in);
    
    if(!inputFile){
      cerr << "Cannot open input file '" << infileName.Data() << "'. Exit!" << endl;
      return;
    }
    
    //Loop over the input files
    std::string inputString=infile_name;   
    
    TFile* isROOTFile = TFile::Open(inputString.c_str());
    if (!isROOTFile){ 
      std::cout << "This is not a root file, try as a list of files !!!" << std::endl;    
      while (inputFile >> inputString) {
	AddFileToTChain(inputString);
      }    
    }
    else{
      AddFileToTChain(inputString);
    }
  }


};

#endif


