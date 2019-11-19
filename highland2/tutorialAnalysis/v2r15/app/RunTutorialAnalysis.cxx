#include "tutorialAnalysis.hxx"
#include "AnalysisLoop.hxx"

int main(int argc, char *argv[]){
  tutorialAnalysis* ana = new tutorialAnalysis();
  AnalysisLoop loop(ana, argc, argv); 
  loop.Execute();
}
