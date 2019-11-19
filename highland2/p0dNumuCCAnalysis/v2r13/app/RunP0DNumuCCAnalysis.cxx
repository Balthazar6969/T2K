#include "p0dNumuCCAnalysis.hxx"
#include "AnalysisLoop.hxx"

int main(int argc, char *argv[]){
  p0dNumuCCAnalysis* ana = new p0dNumuCCAnalysis();
  AnalysisLoop loop(ana, argc, argv); 
  loop.Execute();
}
