#include "useTutorialAnalysis.hxx"
#include "AnalysisLoop.hxx"

int main(int argc, char *argv[]){
  useTutorialAnalysis* ana = new useTutorialAnalysis();
  AnalysisLoop loop(ana, argc, argv); 
  loop.Execute();
}
