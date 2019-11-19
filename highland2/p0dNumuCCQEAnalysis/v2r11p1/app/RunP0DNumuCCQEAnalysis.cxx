#include "p0dNumuCCQEAnalysis.hxx"
#include "AnalysisLoop.hxx"

int main(int argc, char *argv[]){
  p0dNumuCCQEAnalysis* ana = new p0dNumuCCQEAnalysis();
  AnalysisLoop loop(ana, argc, argv); 
  loop.Execute();
}
