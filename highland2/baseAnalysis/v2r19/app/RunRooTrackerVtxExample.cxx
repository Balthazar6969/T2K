#include <stdio.h>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include "TVector3.h"
#include "TLorentzVector.h"
#include "oaAnalysisReaderProjectHeaders.h"

using std::cout;
using std::cerr;
using std::endl;

int rNEvts = -1; // Rootree
int tNEvts = -1; // truthtree
int sNEvts = -1; // seltree
char * fNDFileName;
char * fSKFileName;
double  t_nu_true_e;
int  t_TruthVertexID;
int  t_TruthVertexNB;
int  t_RooVtxIndex;
double  s_nu_true_e;
int  s_TruthVertexID;
int  s_TruthVertexNB;
int  s_RooVtxIndex;

void Usage();
void ParseArgs(int argc, char **argv);

int main(int argc, char *argv[])
{
  // This example only works when compiled against oaAnalysis
  ParseArgs(argc, argv);
  //Set up output tree
  Double_t Enu;
  TFile fcc("cc_summary.root","RECREATE");
  TTree* out_tree = new TTree("sample_sum","CC Summary" );
  out_tree->Branch("Enu",           &Enu,            "Enu/D");
  // Load the oaAnalysis TNRooTrackerVtx tree containing
  TFile * infile = new TFile(fNDFileName, "OPEN");
  TTree * Roo_tree = (TTree*) infile->Get("NRooTrackerVtx");
  
  int NVtx=0; 
  TClonesArray * nRooVtxs = new TClonesArray("ND::NRooTrackerVtx", 100);
  Roo_tree->SetBranchAddress("Vtx", &nRooVtxs);
  Roo_tree->SetBranchAddress("NVtx", &NVtx);
  TTree * truth_tree = (TTree*) infile->Get("truth");
  truth_tree->SetBranchAddress("nu_true_e",&t_nu_true_e);
  truth_tree->SetBranchAddress("TruthVertexID",&t_TruthVertexID);
  truth_tree->SetBranchAddress("TruthVertexNB",&t_TruthVertexNB);
  truth_tree->SetBranchAddress("RooVtxIndex",&t_RooVtxIndex);

  TTree * sel_tree = (TTree*) infile->Get("default");
  sel_tree->SetBranchAddress("nu_true_e",&s_nu_true_e);
  sel_tree->SetBranchAddress("TruthVertexID",&s_TruthVertexID);
  sel_tree->SetBranchAddress("TruthVertexNB",&s_TruthVertexNB);
  sel_tree->SetBranchAddress("RooVtxIndex",&s_RooVtxIndex);


  tNEvts = truth_tree->GetEntries(); 
  sNEvts = sel_tree->GetEntries(); 
  rNEvts = Roo_tree->GetEntries();
  cout<<" truth_tree "<<tNEvts<<" sNEvtx "<<sNEvts<<" rNEvts "<<rNEvts<<endl;
  for(int i = 0; i < sNEvts; i++){
    sel_tree->LoadTree(i);
    sel_tree->GetEntry(i);
    if (s_RooVtxIndex == -999 ) continue;
    Roo_tree->LoadTree(s_RooVtxIndex);
    Roo_tree->GetEntry(s_RooVtxIndex);
    ND::NRooTrackerVtx * vtx = (ND::NRooTrackerVtx*) nRooVtxs->At(s_TruthVertexNB);
    cout<<i<<" i "<<s_RooVtxIndex<<endl;
    if(s_TruthVertexID!=vtx->TruthVertexID)
      std::cout<<" truthvertex sel "<<s_TruthVertexID<<" vtx "<<vtx->TruthVertexID<<std::endl;
  }
  int prev=-999;
  for(int i = 0; i < tNEvts; i++){
    truth_tree->LoadTree(i);
    truth_tree->GetEntry(i);

    if (t_RooVtxIndex == -999 || t_RooVtxIndex == -1 ) continue;
    Roo_tree->LoadTree(t_RooVtxIndex);
    Roo_tree->GetEntry(t_RooVtxIndex);

    cout<<i<<" "<<tNEvts<<" itrue "<<t_RooVtxIndex<<" nb "<<t_TruthVertexNB<<std::endl;
    ND::NRooTrackerVtx * vtx = (ND::NRooTrackerVtx*) nRooVtxs->At(t_TruthVertexNB);
    if(t_TruthVertexID!=vtx->TruthVertexID){
      std::cout<<" truthvertex truth "<<t_TruthVertexID<<" vtx "<<vtx->TruthVertexID<<std::endl;
    }
    prev=t_RooVtxIndex;
  }
  //  cout<<" sel "<<isel<<" true "<<itrue<<" nseltrue "<<nseltrue<<endl;
  fcc.cd();
  out_tree->Write("sample_sum");
  fcc.Close();

  return 0;
}

// Print the cmd line syntax
void Usage(){
  cout << "Cmd line syntax should be:" << endl;
  cout << "out_ana.exe -n nd_inputfile  [-e nevents]" << endl;
}

// Messy way to process cmd line arguments.
void ParseArgs(int argc, char **argv){
  int nargs = 1; 
  if(argc<(nargs*2+1)){ Usage(); exit(1); }
  for(int i = 1; i < argc; i+=2){
    if(string(argv[i]) == "-n") fNDFileName = argv[i+1];
    else if(string(argv[i]) == "-e") rNEvts = std::atoi(argv[i+1]);
    else {  
        cout << "Invalid argument:" << argv[i] << " "<< argv[i+1] << endl;
        Usage();
        exit(1);
    }
  } 
}

