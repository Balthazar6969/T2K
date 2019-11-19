#include "antiNueCCAnalysis.hxx"
#include "AnalysisLoop.hxx"

int main(int argc, char *argv[]){
    antiNueCCAnalysis* ana = new antiNueCCAnalysis();
    AnalysisLoop loop(ana, argc, argv);
    loop.Execute();
}
