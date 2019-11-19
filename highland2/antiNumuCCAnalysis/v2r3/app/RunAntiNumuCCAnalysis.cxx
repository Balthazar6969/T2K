#include "antiNumuCCAnalysis.hxx"
#include "AnalysisLoop.hxx"

int main(int argc, char *argv[]){
  antiNumuCCAnalysis* ana = new antiNumuCCAnalysis();
  AnalysisLoop loop(ana, argc, argv); 
  loop.Execute();
}
