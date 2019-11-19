#include "numuCCMultiTargetAnalysis.hxx"
#include "AnalysisLoop.hxx"

int main(int argc, char *argv[]){
  numuCCMultiTargetAnalysis* ana = new numuCCMultiTargetAnalysis();
  AnalysisLoop loop(ana, argc, argv); 
  loop.Execute();
}
