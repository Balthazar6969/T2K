#if !defined(__CINT__) || defined(__MAKECINT__)
#include <iostream>
#include <iomanip>
#include <fstream>
#include <strstream>
#include "TString.h"
#include "TFile.h"
#include "TChain.h"
#include "TROOT.h"
#include "TObjString.h"
#include "TSortedList.h"
#include "TObjArray.h"
#include "TOrdCollection.h"
#include "THashTable.h"
#include "TBtree.h"
#include "TStopwatch.h"
#include <stdio.h>
#include <stdlib.h>
#include "TMatrixD.h"
#include "TMatrixDSym.h"
//#include <TMatrixDBase.h>
#include <TStyle.h>
#include <ccAnalysis_tree_process.h>
#include <AnalysisTools/TCCweightMA.hxx>
using namespace std;

extern void InitGui();
VoidFuncPtr_t initfuncs[] = { InitGui, 0 };
TROOT root("Rint","The ROOT Interactive Interface", initfuncs);

#endif

//*******************************************************************
//
// This program reads the root file produced by ccAnalysis.C
// and produces output files then read by the fitting program
// The output format is related to the current fitting setup.
//
// Francesca Di Lodovico (Oct 2009)
//
//*******************************************************************


//
TChain *gimeChain(){
//*******************************************************************
//
//-- Input root file as created from ccAnalysis. 
//
//*******************************************************************
  cout<<"Starting TChain"<<endl;
  ostrstream name;
  name<<"tree_process"<<ends;
  string stringtree=name.str();  
  TChain *c = new TChain(stringtree.c_str()); 
  const char *fn[] = { "ccAnalysis_output_Curv3.00-Cut6-MA1.1.root", 0};
  for (unsigned i=0; fn[i]!=0; ++i) {
    cout << "appending " <<  fn[i] << endl;
    c->Add(fn[i]);
  }
  return c;
}

//
void run_ccAnalysis_fit(bool dotree=true, bool dofit=true, int which=0){
//*******************************************************************
//
// dotree = running over the input file and creating the needed files for the fit
// dofit = performing the fit on the (Pmu,costthetamu) plane to extract the Enu spectrum
// which = is the index referring to the MA value
//
//*******************************************************************

//-- change the ROOT setup to make nice plots
  gROOT->LoadMacro("setup.C"); 
  setup();

//-- creating input files for the fit program
  if(dotree){

    gROOT->LoadMacro("AnalysisTools/TCCweightMA.cxx"); //-- this is a temporary solution for MA to allow to perform tests
    printf( "loading TCCWeightMA \n"); 
    
    gROOT->LoadMacro("ccAnalysis_tree_process.C"); //-- this processes the tree in the input root file
    printf( "loading ccAnalysis_tree_process\n");   
    
    ostrstream name; 
    name<<"output/CCtree.root"<<ends; //output root file with histograms
    string str=name.str();
    TFile out(str.c_str(),"recreate");
    cout<<"creating histo: "<<str.c_str()<<endl;
    
    //-- output files for fit
    ofstream myfileCC, myfileCCQE, myfileCCnQE;
    ostrstream baseCC; baseCC<<"output/Enu_"<<which<<"_tree.txt"<<ends; string nameCC; nameCC=baseCC.str(); myfileCC.open(nameCC.c_str());
    ostrstream baseCCQE; baseCCQE<<"output/EnuCCQE_"<<which<<"_tree.txt"<<ends; string nameCCQE; nameCCQE=baseCCQE.str(); myfileCCQE.open(nameCCQE.c_str());
    ostrstream baseCCnQE; baseCCnQE<<"output/EnuCCnQE_"<<which<<"_tree.txt"<<ends; string nameCCnQE; nameCCnQE=baseCCnQE.str(); myfileCCnQE.open(nameCCnQE.c_str());
    
    //-- running the code 
    TChain *treesel = gimeChain();
    ccAnalysis_tree_process t;
    t.Init(treesel);
    t.Loop(which, &out,&myfileCC,&myfileCCQE,&myfileCCnQE);
    out.Write();
    myfileCC.close();
    myfileCCQE.close();
    myfileCCnQE.close();

  }

//-- perform the fit using the input files created above

  if(dofit){ 
    
    int i(0);

    gROOT->LoadMacro("ccAnalysis_neutrinoData.C++"); //file that takes as input the data distribution
    printf( "loading ccAnalysis_neutrinoData \n"); 
    
    gROOT->LoadMacro("ccAnalysis_fit.C++"); //file that takes as input the MC distribution (ie input files produced above) and performs the fit
    printf( "loading ccAnalysis_fit \n");
    
    //-- initialize the output files
    //-- results
    ostrstream nameres;
    nameres<<"output/results_"<<i<<".txt"<<ends;
    string strres=nameres.str();
    ofstream outfileres(strres.c_str());
    cout<<strres.c_str()<<endl;
    
    //-- correlation matrix
    ostrstream namecorr;
    namecorr<<"output/corrmatrix_"<<i<<".txt"<<ends;
    string strcorr=namecorr.str();
    ofstream outfilecorr(strcorr.c_str());
    cout<<strcorr.c_str()<<endl;

    //-- plots
    ostrstream nameeps;
    ostrstream namegif;
    nameeps<<"output/fullspectrumfit_"<<i<<".eps"<<ends;
    namegif<<"output/fullspectrumfit_"<<i<<".gif"<<ends;
    string stringeps=nameeps.str();
    string stringgif=namegif.str();
    
    //-- perform the fit
    ccAnalysis_fit a;
    a.Loop(&outfileres, &outfilecorr, stringeps.c_str(), stringgif.c_str());
  }

}
