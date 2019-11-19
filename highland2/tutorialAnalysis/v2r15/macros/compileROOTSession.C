// To compile this (very basic) macro in a ROOT session do:
// 
//  root -l
//   gSystem->Load("libEG.so");
//   gSystem->Load("libpsycheCore.so");
//   gSystem->Load("libReadoaAnalysis.so");
//   gSystem->Load("libpsycheUtils.so");
//   gSystem->Load("libhighlandTools.so");
//   gSystem->AddIncludePath("-I$PSYCHECOREROOT/src");
//   gSystem->AddIncludePath("-I$ND280ANALYSISTOOLSROOT/$ND280ANALYSISTOOLSCONFIG");
//   gSystem->AddIncludePath("-I$PSYCHEUTILSROOT/src");
//   gSystem->AddIncludePath("-I$HIGHLANDUTILSROOT/src");
//   gSystem->AddIncludePath("-I$HIGHLANDTOOLSROOT/src"); 
//   gSystem->CompileMacro("compileROOTSession.C");
// 
// To run the macro (after you've done the above) do:
//   makeplots("filename.root");


#include "DrawingTools.hxx"
#include "DataSample.hxx"

void makeplots(std::string filename = "test.root"){

  // Create an instance of the DrawingTools and load the configuration specified in the input file
  DrawingTools draw(filename);

  // Create a DataSample with the input file
  DataSample sample(filename);

  // Make a very simple plot
  draw.SetTitleX("muon candidate momentum (MeV/c)");
  draw.Draw(sample,"selmu_mom",10,0,5000,"reaction","accum_level>5");
  
  return;
  
}

