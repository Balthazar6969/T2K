// To compile this on the command line do:
// 
//  g++ -o compileCommandLine.exe compileCommandLine.cxx `root-config --cflags --libs` -I $PSYCHECOREROOT/src -I$ND280ANALYSISTOOLSROOT/$ND280ANALYSISTOOLSCONFIG -I$PSYCHEUTILSROOT/src -I$HIGHLANDUTILSROOT/src -I$HIGHLANDTOOLSROOT/src -L$PSYCHECOREROOT/$ND280ANALYSISTOOLSCONFIG -L$PSYCHEUTILSROOT/$ND280ANALYSISTOOLSCONFIG -L$HIGHLANDTOOLSROOT/$ND280ANALYSISTOOLSCONFIG -lEG -lpsycheCore -lpsycheUtils -lhighlandTools -lGeom
// 
// To run the executable do:
// 
// ./compileCommandLine.exe -m filename.root  

#include "TCanvas.h"

#include "DrawingTools.hxx"
#include "DataSample.hxx"

void ParseArgs(int argc, char **argv);
std::string filename = "test.root";

int main(int argc, char** argv){

  // Parse the command line arguments
  ParseArgs(argc,argv);

  // Create an instance of the DrawingTools and load the configuration specified in the input file
  DrawingTools draw(filename);

  // Create a DataSample with the input file
  DataSample sample(filename);

  // Make a very simple plot and save it
  TCanvas* c1 = new TCanvas();
  draw.SetTitleX("muon candidate momentum (MeV/c)");
  draw.Draw(sample,"selmu_mom",10,0,5000,"reaction","accum_level>5");
  c1->SaveAs("tutorial_selmu_mom.pdf");

  return 0;
  
}

// Function to pass command line arguments to the int main()
void ParseArgs(int argc, char **argv){
  int nargs = 1;
  if(argc<(nargs*2+1)){exit(1);}
  for(int i = 1; i < argc; i+=2){
    if(std::string(argv[i]) == "-m"){filename = argv[i+1];}
    else{
      std::cout<<"ERROR: invalid argument:"<<argv[i]<<" "<<argv[i+1]<<std::endl;
      exit(1);
    }
  }
  return;
}
