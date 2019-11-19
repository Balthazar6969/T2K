#include "p0dExampleAnalysis.hxx"
#include "AnalysisLoop.hxx"

int main(int argc, char *argv[]){
  p0dExampleAnalysis* ana = new p0dExampleAnalysis();
  AnalysisLoop loop(ana, argc, argv); 
  loop.Execute();
}
