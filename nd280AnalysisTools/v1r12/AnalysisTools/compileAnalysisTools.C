#include <iostream>
#include <fstream>
#include <string>
#include "TFile.h"

void compileAnalysisTools() {
  if(! gSystem->Exec("python checkNeedCompile.py 1 ../")) {
    // There's no need for us to re-compile
    gApplication->Terminate();
    exit;
  }

  TFile *inputFile;

  char *path=NULL;
  path=getenv("PWD");
  strcat (path, "/input-file.list");

  ifstream in(path);
  if (!in || in.bad()) return;

  std::string file;
  while(!in.eof()){
    in >> file;
    if(file.find("break") != std::string::npos){
      std::cout << "Hit 'break' without finding a valid file" << std::endl;
      return;
    }
    std::cout << "Trying: "<< file << std::endl;
    inputFile =  TFile::Open(file.c_str());
    if(inputFile) break;
  }
  in.close();

  if(!inputFile) std::cout << "File open failed" << std::endl;

  std::cout << "Opened" << std::endl;

  inputFile->MakeProject("libReadoaAnalysis", "*", "recreate++");
  inputFile->Close();

  gSystem->Load("libReadoaAnalysis.so");

  ifstream load("analysisToolsOrder.list");
  if (!load || load.bad()) return;

  std::string load_file;

  while(!load.eof()){

    std::string command(".L ");
    load>>load_file;
    if(load_file[0] == '#'){
      std::cout << "Skipping commented AnalysisTool: "
        << load_file << std::endl;
      continue;
    }
    if (load.eof()) break;
    command.append(load_file);
    command.append("+");
    std::cout<<command.c_str()<<std::endl;
    char* command_char = command.c_str();
    gROOT->ProcessLine(command_char);
  }
  load.close();

  gApplication->Terminate();
}
