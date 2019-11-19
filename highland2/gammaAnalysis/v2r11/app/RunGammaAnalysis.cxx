#include "gammaAnalysis.hxx"
#include "AnalysisLoop.hxx"

int main(int argc, char *argv[]){
  gammaAnalysis* ana = new gammaAnalysis();
  AnalysisLoop loop(ana, argc, argv); 
  loop.Execute();
}
