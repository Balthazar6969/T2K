#include "antiNumuCCIncFwdAnalysis.hxx"
#include "AnalysisLoop.hxx"

int main(int argc, char *argv[]){
  antiNumuCCIncFwdAnalysis* ana = new antiNumuCCIncFwdAnalysis();
  AnalysisLoop loop(ana, argc, argv); 
  loop.Execute();
}
