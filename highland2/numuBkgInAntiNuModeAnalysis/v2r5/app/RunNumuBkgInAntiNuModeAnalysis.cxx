#include "numuBkgInAntiNuModeAnalysis.hxx"
#include "AnalysisLoop.hxx"

int main(int argc, char *argv[]){
  numuBkgInAntiNuModeAnalysis* ana = new numuBkgInAntiNuModeAnalysis();
  AnalysisLoop loop(ana, argc, argv); 
  loop.Execute();
}
