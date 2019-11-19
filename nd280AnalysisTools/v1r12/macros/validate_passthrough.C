/*
This macro takes an MC oaAnalysis file as input, and checks that the pass through
information is correctly filled, by comparing it to the corressponding numc file.

Need to use string "oa_gn_" or "oa_nt_" when choosing input oaAnalysis file
(i.e. these can't be replaced with a wildcard *). This is a second check on what
the input file is (also need to set GENERATOR to >0 (GENIE) or <0 (NEUT).

"beamcondition" is to check that the BasicHeader/Intensity variable is filled correctly
(expected values are hard coded, specify which hard coded value with "beamcondition"
Current support values are:
"prod5Abeama" for RunI magnet files
"prod5Abeamb" for RunII magnet files

Always run the macros from the directory in which you ran runAnalysis.py with
the -c option (if you have not done this then please read the EXAMPLES_README
and the analysis turorial again).

Run the macro in the following way (compiled mode is much quicker!):
[root].x ${ND280ANALYSISTOOLSROOT}/AnalysisTools/oaAnalysisReadInitFile-\
${ND280ANALYSISTOOLSCONFIG}.C
[root].L validate_passthrough.C+
[root]validate_passthrough("path/to/analysisfile.root", "beamcondition", \
"/path/to/dir/containing/numcfiles/", firstevent, nevents);

*/

#define GENERATOR 1 //>0 GENIE, <0 NEUT
//#define __DEBUG__

#include <iostream>
#include <string>

#include "TROOT.h"
#include "TFile.h"
#include "TChain.h"
#include "TClonesArray.h"

// To use AnalysisTools utils in ACLiC mode then need to add the relevant header
// here and inside the ifdef so it is not seen by CINT.

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <AnalysisTools/TCheatingAnalysisUtils.hxx>
#include <AnalysisTools/TGRooTrackerUtils.hxx>
#endif

using std::cout;
using std::endl;

// A couple of functions for comparing arrays of int's and floats
double fASmallNum = 0.000001;
bool Compare(double a, double b);
bool Compare(float a, float b);
bool Compare(int a, int b);
bool Compare(long a, long b);
bool Compare(float a[], float b[], int ni);
bool Compare(int a[], int b[], int ni);
bool Compare(double a[], double b[], int ni);

void validate_passthrough(TString inputfilename, string beamcondition, TString numcfiledirectory="./", int first_entry = 0, int max_entries = -1)
{
  gROOT->SetBatch(1);
  // See commented code below for loading libraries automatically.
  /*
  #if !defined(__MAKECINT__)
    // This should not be seen when using ACLiC.
  //  gROOT->ProcessLine(".x ${ND280ANALYSISTOOLSROOT}/AnalysisTools/\\
  //  oaAnalysisReadInitFile-${ND280ANALYSISTOOLSCONFIG}.C");
  #endif
  */

  // _________________________________________________________________________
  //**************************************************************************
  // Prepare to read trees.
  //
  //determine whether it's a genie or a neut file

  std::cout << "Adding file(s) matching string: " << inputfilename << std::endl;
  #if GENERATOR>0
  if(inputfilename.Contains("oa_nt_")) {
    cout << "This is a neut file. You need to redefine GENERATOR to be less than 0. Exiting..." << endl;
    return;
  }
  else if(inputfilename.Contains("oa_gn_"))
    cout << "This is a genie file" << endl;
  #elif GENERATOR<0
  if(inputfilename.Contains("oa_nt_"))
    cout << "This is a neut file" << endl;
  else if(inputfilename.Contains("oa_gn_")) {
    cout << "This is a genie file. You need to redefine GENERATOR to be more than 0. Exiting..." << endl;
    return;
  }
  #elif GENERATOR==0
  std::cout << "Please specify GENERATOR variable in macro file. >0 GENIE. <0 NEUT." << std::endl;
  #endif
  else {
    std::cout << "Not a genie or neut file. Please specify input filename with oa_nt_ or oa_gn_. Exiting" << std::endl;
    return;
  }

  //setup the TChain
  string chainname;
  #if GENERATOR>0
  chainname = "TruthDir/GRooTrackerVtx";
  #elif GENERATOR<0
  chainname = "TruthDir/NRooTrackerVtx";
  #endif
  TChain rooTrackerOAAnalysisChain = TChain(chainname.c_str());
  TClonesArray *Vtx;
  #if GENERATOR>0
  Vtx = new TClonesArray("ND::GRooTrackerVtx", 100);
  #elif GENERATOR<0
  Vtx = new TClonesArray("ND::NRooTrackerVtx", 100);
  #endif
  std::cout << "Added " << rooTrackerOAAnalysisChain.AddFile(inputfilename) << " file(s)"<< std::endl;
  int NVtx;
  rooTrackerOAAnalysisChain.SetBranchAddress("NVtx",&NVtx);
  rooTrackerOAAnalysisChain.SetBranchAddress("Vtx",&Vtx);

  if(rooTrackerOAAnalysisChain.GetEntries() <= 0) {
    cout << inputfilename << " gives a TChain with no entries" << endl;
    return;
  }

  //Setup the specific RooTrackerVtx and get the first entry
  rooTrackerOAAnalysisChain.GetEntry(0);
  #if GENERATOR>0
  ND::GRooTrackerVtx *curr_flux = (ND::GRooTrackerVtx*) Vtx->At(0);
  cout << "generator 1: GENIE" << endl;
  #elif GENERATOR<0
  ND::NRooTrackerVtx *curr_flux = (ND::NRooTrackerVtx*) Vtx->At(0);
  cout << "generator -1: NEUT" << endl;
  #else
  return;
  #endif

  // Get original numc file information
  TObjString* OrigFileName = curr_flux->OrigFileName;
  TObjString* OrigTreeName = curr_flux->OrigTreeName;
  TString numcfilename = OrigFileName->GetString();
  TString numcorigfilename(numcfilename);
  TString numctreename = OrigTreeName->GetString();
  numcfilename.Prepend(numcfiledirectory);
  cout << "\nFound tree: "<< rooTrackerOAAnalysisChain.GetName() << endl;

  // Load original numc tree
  cout << "Checking against tree: "<< numctreename<<" in numc file: "<< numcfilename <<"\n"<< endl;
  TChain *rooTrackerNumcChain = new TChain(numctreename);
  rooTrackerNumcChain->Add(numcfilename);
  assert(rooTrackerNumcChain->GetEntries() > 0);
  long numctreeentries = rooTrackerNumcChain->GetEntries();
  if (rooTrackerNumcChain->GetEntries() != curr_flux->OrigTreeEntries) {
    cout<<"Numc tree has "<<rooTrackerNumcChain->GetEntries()<<"entries; expect "
	<<curr_flux->OrigTreeEntries<<" entries"<<endl;
    return;
  }
  double numctreepot = rooTrackerNumcChain->GetWeight();
  if (rooTrackerNumcChain->GetWeight() != curr_flux->OrigTreePOT) {
    cout<<"Numc tree has "<<rooTrackerNumcChain->GetWeight()<<"POT; expect "
	<<curr_flux->OrigTreePOT<<" POT"<<endl;
    return;
  }
  TString generatorname = curr_flux->GeneratorName->GetString();

  //__________________________________________________
  //Setup numc tree branches
  //
  // Setup generator independant numc tree branches - flux info
  Long64_t    NuFluxEntry; rooTrackerNumcChain->SetBranchAddress("NuFluxEntry", &NuFluxEntry);
  TObjString *NuFileName=0; rooTrackerNumcChain->SetBranchAddress("NuFileName", &NuFileName);
  int         NuParentPdg; rooTrackerNumcChain->SetBranchAddress("NuParentPdg", &NuParentPdg);
  int         NuParentDecMode; rooTrackerNumcChain->SetBranchAddress("NuParentDecMode", &NuParentDecMode);
  double      NuParentDecP4 [4]; rooTrackerNumcChain->SetBranchAddress("NuParentDecP4", NuParentDecP4);
  double      NuParentDecX4 [4]; rooTrackerNumcChain->SetBranchAddress("NuParentDecX4", NuParentDecX4);
  float       NuCospibm; rooTrackerNumcChain->SetBranchAddress("NuCospibm", &NuCospibm);
  float       NuNorm; rooTrackerNumcChain->SetBranchAddress("NuNorm", &NuNorm);
  double      NuParentProP4[4]; rooTrackerNumcChain->SetBranchAddress("NuParentProP4", NuParentProP4);
  double      NuParentProX4[4]; rooTrackerNumcChain->SetBranchAddress("NuParentProX4", NuParentProX4);
  float       NuCospi0bm; rooTrackerNumcChain->SetBranchAddress("NuCospi0bm", &NuCospi0bm);
  //  int         NuParentProNVtx; rooTrackerNumcChain->SetBranchAddress("NuParentProNVtx", &NuParentProNVtx);
  //  NuParentProNVtx is not passed through, since it contains redundant/unused information
  float       NuRnu; rooTrackerNumcChain->SetBranchAddress("NuRnu", &NuRnu);
  float       NuXnu [2]; rooTrackerNumcChain->SetBranchAddress("NuXnu", NuXnu);
  int         NuIdfd; rooTrackerNumcChain->SetBranchAddress("NuIdfd", &NuIdfd);
  int         NuGipart; rooTrackerNumcChain->SetBranchAddress("NuGipart", &NuGipart);
  float       NuGpos0[3]; rooTrackerNumcChain->SetBranchAddress("NuGpos0", NuGpos0);
  float       NuGvec0[3]; rooTrackerNumcChain->SetBranchAddress("NuGvec0", NuGvec0);
  float       NuGamom0; rooTrackerNumcChain->SetBranchAddress("NuGamom0", &NuGamom0);
  int         NuNg; rooTrackerNumcChain->SetBranchAddress("NuNg", &NuNg);
  float       NuGp[12][3]; rooTrackerNumcChain->SetBranchAddress("NuGp", NuGp);
  float       NuGcosbm[12]; rooTrackerNumcChain->SetBranchAddress("NuGcosbm", NuGcosbm);
  float       NuGv[12][3]; rooTrackerNumcChain->SetBranchAddress("NuGv", NuGv);
  int         NuGpid[12]; rooTrackerNumcChain->SetBranchAddress("NuGpid", NuGpid);
  int         NuGmec[12]; rooTrackerNumcChain->SetBranchAddress("NuGmec", NuGmec);
  float       NuEnusk; rooTrackerNumcChain->SetBranchAddress("NuEnusk", &NuEnusk);
  float       NuNormsk; rooTrackerNumcChain->SetBranchAddress("NuNormsk", &NuNormsk);
  float       NuAnorm; rooTrackerNumcChain->SetBranchAddress("NuAnorm", &NuAnorm);
  int         NuGmat[12]; rooTrackerNumcChain->SetBranchAddress("NuGmat", NuGmat);
  float       NuGdistc[12]; rooTrackerNumcChain->SetBranchAddress("NuGdistc", NuGdistc);
  float       NuGdistal[12]; rooTrackerNumcChain->SetBranchAddress("NuGdistal", NuGdistal);
  float       NuGdistti[12]; rooTrackerNumcChain->SetBranchAddress("NuGdistti", NuGdistti);
  float       NuGdistfe[12]; rooTrackerNumcChain->SetBranchAddress("NuGdistfe", NuGdistfe);
  float       NuVersion; rooTrackerNumcChain->SetBranchAddress("NuVersion", &NuVersion);
  int         NuTuneid; rooTrackerNumcChain->SetBranchAddress("NuTuneid", &NuTuneid);
  int         NuNtrig; rooTrackerNumcChain->SetBranchAddress("NuNtrig", &NuNtrig);
  int         NuPint; rooTrackerNumcChain->SetBranchAddress("NuPint", &NuPint);
  float       NuBpos[2]; rooTrackerNumcChain->SetBranchAddress("NuBpos", NuBpos);
  float       NuBtilt[2]; rooTrackerNumcChain->SetBranchAddress("NuBtilt", NuBtilt);
  float       NuBrms[2]; rooTrackerNumcChain->SetBranchAddress("NuBrms", NuBrms);
  float       NuEmit[2]; rooTrackerNumcChain->SetBranchAddress("NuEmit", NuEmit);
  float       NuAlpha[2]; rooTrackerNumcChain->SetBranchAddress("NuAlpha", NuAlpha);
  float       NuHcur[3]; rooTrackerNumcChain->SetBranchAddress("NuHcur", NuHcur);
  int         NuRand; rooTrackerNumcChain->SetBranchAddress("NuRand", &NuRand);

  //Setup generator independent numc branches - generator info (defined separately in TGRooTrackerVtx.hxx and TNRooTrackerVtx.hxx so need to be careful if there are changes)
  TObjString *EvtCode=0; rooTrackerNumcChain->SetBranchAddress("EvtCode", &EvtCode);
  int    EvtNum; rooTrackerNumcChain->SetBranchAddress("EvtNum",&EvtNum);
  double EvtXSec; rooTrackerNumcChain->SetBranchAddress("EvtXSec",&EvtXSec);
  double EvtDXSec; rooTrackerNumcChain->SetBranchAddress("EvtDXSec",&EvtDXSec);
  double EvtWght; rooTrackerNumcChain->SetBranchAddress("EvtWght",&EvtWght);
  double EvtProb; rooTrackerNumcChain->SetBranchAddress("EvtProb",&EvtProb);
  double EvtVtx[4]; rooTrackerNumcChain->SetBranchAddress("EvtVtx",EvtVtx);
  int    StdHepN; rooTrackerNumcChain->SetBranchAddress("StdHepN",&StdHepN);
  int    StdHepPdg[350]; rooTrackerNumcChain->SetBranchAddress("StdHepPdg",StdHepPdg);
  int    StdHepStatus[350]; rooTrackerNumcChain->SetBranchAddress("StdHepStatus",StdHepStatus);
  double StdHepX4[350][4]; rooTrackerNumcChain->SetBranchAddress("StdHepX4",StdHepX4);
  double StdHepP4[350][4]; rooTrackerNumcChain->SetBranchAddress("StdHepP4",StdHepP4);
  double StdHepPolz[350][3]; rooTrackerNumcChain->SetBranchAddress("StdHepPolz",StdHepPolz);
  int    StdHepFd[350]; rooTrackerNumcChain->SetBranchAddress("StdHepFd",StdHepFd);
  int    StdHepLd[350]; rooTrackerNumcChain->SetBranchAddress("StdHepLd",StdHepLd);
  int    StdHepFm[350]; rooTrackerNumcChain->SetBranchAddress("StdHepFm",StdHepFm);
  int    StdHepLm[350]; rooTrackerNumcChain->SetBranchAddress("StdHepLm",StdHepLm);

  //Setup genie specific numc tree branches
  #if GENERATOR>0
  int    StdHepRescat[350]; rooTrackerNumcChain->SetBranchAddress("StdHepRescat",StdHepRescat);
  int    G2NeutEvtCode; rooTrackerNumcChain->SetBranchAddress("G2NeutEvtCode",&G2NeutEvtCode);
  //Setup neut specific numc tree branches
  #elif GENERATOR<0
  //    int    NEneutmode; rooTrackerNumcChain->SetBranchAddress("NEneutmode",&NEneutmode);
  //    NEneutmode is not passed through, since it contains redundant/unused information
  int    NEnvc; rooTrackerNumcChain->SetBranchAddress("NEnvc",&NEnvc);
  int    NEipvc[350]; rooTrackerNumcChain->SetBranchAddress("NEipvc",NEipvc);
  float  NEpvc[350][3]; rooTrackerNumcChain->SetBranchAddress("NEpvc",NEpvc);
  int    NEiorgvc[350]; rooTrackerNumcChain->SetBranchAddress("NEiorgvc",NEiorgvc);
  int    NEiflgvc[350]; rooTrackerNumcChain->SetBranchAddress("NEiflgvc",NEiflgvc);
  int    NEicrnvc[350]; rooTrackerNumcChain->SetBranchAddress("NEicrnvc",NEicrnvc);
  int    NEnvert; rooTrackerNumcChain->SetBranchAddress("NEnvert",&NEnvert);
  float  NEposvert[350][3]; rooTrackerNumcChain->SetBranchAddress("NEposvert",NEposvert);
  int    NEiflgvert[350]; rooTrackerNumcChain->SetBranchAddress("NEiflgvert",NEiflgvert);
  int    NEnvcvert; rooTrackerNumcChain->SetBranchAddress("NEnvcvert",&NEnvcvert);
  float  NEdirvert[300][3]; rooTrackerNumcChain->SetBranchAddress("NEdirvert",NEdirvert);
  float  NEabspvert[300]; rooTrackerNumcChain->SetBranchAddress("NEabspvert",NEabspvert);
  float  NEabstpvert[300]; rooTrackerNumcChain->SetBranchAddress("NEabstpvert",NEabstpvert);
  int    NEipvert[300]; rooTrackerNumcChain->SetBranchAddress("NEipvert",NEipvert);
  int    NEiverti[300]; rooTrackerNumcChain->SetBranchAddress("NEiverti",NEiverti);
  int    NEivertf[300]; rooTrackerNumcChain->SetBranchAddress("NEivertf",NEivertf);
  float  NEcrsx;    rooTrackerNumcChain->SetBranchAddress("NEcrsx",&NEcrsx);
  float  NEcrsy;    rooTrackerNumcChain->SetBranchAddress("NEcrsy",&NEcrsy);
  float  NEcrsz;    rooTrackerNumcChain->SetBranchAddress("NEcrsz",&NEcrsz);
  float  NEcrsphi;  rooTrackerNumcChain->SetBranchAddress("NEcrsphi",&NEcrsphi);
  #endif

  //___________________________________________________________________
  //**************************************************************************
  //Loop over the oaAnalysis file and do the comparisons
  //
  int vtxentries = rooTrackerOAAnalysisChain.GetEntries();
  if(max_entries > 0 && (max_entries+first_entry)<vtxentries) vtxentries = max_entries+first_entry;
  if(first_entry < 0) first_entry=0;
  std::cout << "Looping over " << vtxentries-first_entry << " events " << std::endl;
  int failedTotal = 0;
  for(int jent = first_entry; jent < vtxentries; jent++) {
    int failedEvent = 0;
    rooTrackerOAAnalysisChain.GetEntry(jent);
    if(jent%3000==0) {
      cout << "\n======== Getting event/entry "<< jent <<" ==========" << endl;
      cout << "     -> the TClonesArray contains "<< NVtx << " entries (vertices)" << endl;
    }
    for(int jvtx=1; jvtx<NVtx; jvtx++){
      #if GENERATOR>0
      curr_flux = (ND::GRooTrackerVtx*) Vtx->At(jvtx);
      #elif GENERATOR<0
      curr_flux = (ND::NRooTrackerVtx*) Vtx->At(jvtx);
      #endif
      rooTrackerNumcChain->GetEntry(curr_flux->OrigEvtNum);
      int failedVtx = 0;

      //**************************************************************************
      //the actual comparisons...
      //______________________________
      //generator independent (flux)
      if( !Compare(NuFluxEntry, curr_flux->NuFluxEntry) )
	{ failedVtx++; cout << "Failed check for: NuFluxEntry!"<< endl; }
      if( NuFileName->GetString().CompareTo(curr_flux->NuFileName->GetString()) != 0 ) {
	failedVtx++;
	cout<<"Compared: "<<NuFileName->GetString()<<" to "<<curr_flux->NuFileName->GetString()<<endl;
	cout<<"Failed check for: NuFileName!"<< endl;
      }
      if( !Compare(NuParentPdg, curr_flux->NuParentPdg) )
	{ failedVtx++; cout << "Failed check for: NuParentPdg!"<< endl; }
      if( !Compare(NuParentDecMode, curr_flux->NuParentDecMode) )
	{ failedVtx++; cout << "Failed check for: NuParentDecMode!"<< endl; }
      if( !Compare(NuParentDecP4, curr_flux->NuParentDecP4,4) )
	{ failedVtx++; cout << "Failed check for: NuParentDecP4[]!"<< endl; }
      if( !Compare(NuParentDecX4, curr_flux->NuParentDecX4, 4) )
	  { failedVtx++; cout << "Failed check for: NuParentDecX4[]!"<< endl; }
      if( !Compare(NuCospibm, curr_flux->NuCospibm) )
	{ failedVtx++; cout << "Failed check for: NuCospibm!"<< endl; }
      if( !Compare(NuNorm, curr_flux->NuNorm) )
	{ failedVtx++; cout << "Failed check for: NuNorm!"<< endl; }
      if( !Compare(NuParentProP4, curr_flux->NuParentProP4, 4) )
	{ failedVtx++; cout << "Failed check for: NuParentProP4[]!"<< endl; }
      if( !Compare(NuParentProP4, curr_flux->NuParentProP4, 4) )
	{ failedVtx++; cout << "Failed check for: NuParentProP4[]!"<< endl; }
      if( !Compare(NuCospi0bm, curr_flux->NuCospi0bm) )
	{ failedVtx++; cout << "Failed check for: NuCospi0bm!"<< endl; }
      //NuParentProNVtx is not passed through, because its redundant/unused
      //if( !Compare(NuParentProNVtx, curr_flux->NuParentProNVtx) )
      //	{ failedVtx++; cout << "Failed check for: NuParentProNVtx!"<< endl; }
      if( !Compare(NuRnu, curr_flux->NuRnu) )
	{ failedVtx++; cout << "Failed check for: NuRnu!"<< endl; }
      if( !Compare(NuXnu[0], curr_flux->NuXnu[0]) )
	{ failedVtx++; cout << "Failed check for: NuXnu[0]!"<< endl; }
      if( !Compare(NuXnu[1], curr_flux->NuXnu[1]) )
	{ failedVtx++; cout << "Failed check for: NuXnu[1]!"<< endl; }
      if( !Compare(NuIdfd, curr_flux->NuIdfd) )
	{ failedVtx++; cout << "Failed check for: NuIdfd!"<< endl; }
      if( !Compare(NuGipart, curr_flux->NuGipart) )
	{ failedVtx++; cout << "Failed check for: NuGipart!"<< endl; }
      if( !Compare(NuGpos0, curr_flux->NuGpos0, 3) )
	{ failedVtx++; cout << "Failed check for: NuGpos0[]!"<< endl; }
      if( !Compare(NuGvec0, curr_flux->NuGvec0, 3) )
	{ failedVtx++; cout << "Failed check for: NuGvec0[]!"<< endl; }
      if( !Compare(NuGamom0, curr_flux->NuGamom0) )
	{ failedVtx++; cout << "Failed check for: NuGamom0!"<< endl; }
      if( !Compare(NuNg, curr_flux->NuNg) )
	{ failedVtx++; cout << "Failed check for: NuNg!"<< endl; }
      for(int k=0;k<NuNg;k++){
	if( !Compare(NuGv[k][0], curr_flux->NuGv[k][0]) )
	  { failedVtx++; cout<< "Failed check for: NuGv["<<k<<"][0]!"<< endl; }
	if( !Compare(NuGv[k][1], curr_flux->NuGv[k][1]) )
	  { failedVtx++; cout<< "Failed check for: NuGv["<<k<<"][1]!"<< endl; }
	if( !Compare(NuGv[k][2], curr_flux->NuGv[k][2]) )
	  { failedVtx++; cout<< "Failed check for: NuGv["<<k<<"][2]!"<< endl; }
       	if( !Compare(NuGp[k][0], curr_flux->NuGp[k][0]) )
	  { failedVtx++; cout<< "Failed check for: NuGp["<<k<<"][0]!"<< endl; }
	if( !Compare(NuGp[k][1], curr_flux->NuGp[k][1]) )
	  { failedVtx++; cout<< "Failed check for: NuGp["<<k<<"][1]!"<< endl; }
	if( !Compare(NuGp[k][2], curr_flux->NuGp[k][2]) )
	  { failedVtx++; cout<< "Failed check for: NuGp["<<k<<"][2]!"<< endl; }
      }
      if( !Compare(NuGcosbm, curr_flux->NuGcosbm, NuNg) )
	{ failedVtx++; cout << "Failed check for: NuGcosbm[]!"<< endl; }
      if( !Compare(NuGpid, curr_flux->NuGpid, NuNg) )
	{ failedVtx++; cout << "Failed check for: NuGpid[]!"<< endl; }
      if( !Compare(NuGmec, curr_flux->NuGmec, NuNg) )
	{ failedVtx++; cout << "Failed check for: NuGmec[]!"<< endl; }
      if( !Compare(NuEnusk, curr_flux->NuEnusk) )
	{ failedVtx++; cout << "Failed check for: NuEnusk!"<< endl; }
      if( !Compare(NuAnorm, curr_flux->NuAnorm) )
	{ failedVtx++; cout << "Failed check for: NuAnorm!"<< endl; }
      if( !Compare(NuNormsk, curr_flux->NuNormsk) )
	{ failedVtx++; cout << "Failed check for: NuNormsk!"<< endl; }
      if( !Compare(NuGmat, curr_flux->NuGmat, NuNg) )
	{ failedVtx++; cout << "Failed check for: NuGmat[]!"<< endl; }
      if( !Compare(NuGdistc, curr_flux->NuGdistc, NuNg) )
	{ failedVtx++; cout << "Failed check for: NuGdistc[]!"<< endl; }
      if( !Compare(NuGdistal, curr_flux->NuGdistal, NuNg) )
	{ failedVtx++; cout << "Failed check for: NuGdistal[]!"<< endl; }
      if( !Compare(NuGdistti, curr_flux->NuGdistti, NuNg) )
	{ failedVtx++; cout << "Failed check for: NuGdistti[]!"<< endl; }
      if( !Compare(NuGdistfe, curr_flux->NuGdistfe, NuNg) )
	{ failedVtx++; cout << "Failed check for: NuGdistfe[]!"<< endl; }
      if( !Compare(NuVersion, curr_flux->NuVersion) )
	{ failedVtx++; cout << "Failed check for: NuVersion!"<< endl; }
      if( !Compare(NuTuneid, curr_flux->NuTuneid) )
	{ failedVtx++; cout << "Failed check for: NuTuneid!"<< endl; }
      if( !Compare(NuNtrig, curr_flux->NuNtrig) )
	{ failedVtx++; cout << "Failed check for: NuNtrig!"<< endl; }
      if( !Compare(NuPint, curr_flux->NuPint) )
	{ failedVtx++; cout << "Failed check for: NuPint!"<< endl; }
      if( !Compare(NuBpos, curr_flux->NuBpos, 2) )
	{ failedVtx++; cout << "Failed check for: NuBpos[]!"<< endl; }
      if( !Compare(NuBtilt, curr_flux->NuBtilt, 2) )
	{ failedVtx++; cout << "Failed check for: NuBtilt[]!"<< endl; }
      if( !Compare(NuBrms, curr_flux->NuBrms, 2) )
	{ failedVtx++; cout << "Failed check for: NuBrms[]!"<< endl; }
      if( !Compare(NuEmit, curr_flux->NuEmit, 2) )
	{ failedVtx++; cout << "Failed check for: NuEmit[]!"<< endl; }
      if( !Compare(NuAlpha, curr_flux->NuAlpha, 2) )
	{ failedVtx++; cout << "Failed check for: NuAlpha[]!"<< endl; }
      if( !Compare(NuHcur, curr_flux->NuHcur, 3) )
	{ failedVtx++; cout << "Failed check for: NuHcur[]!"<< endl; }
      if( !Compare(NuRand, curr_flux->NuRand) )
	{ failedVtx++; cout << "Failed check for: NuRand!"<< endl; }

      //______________________________
      //generator independent (generator - careful!)
      if( EvtCode->GetString().CompareTo(curr_flux->EvtCode->GetString()) != 0 ) {
	failedVtx++;
	cout<<"Compared: "<<EvtCode->GetString()<<" to "<<curr_flux->EvtCode->GetString()<<endl;
	cout<<"Failed check for: EvtCode!"<< endl;
      }
      if( !Compare(EvtNum, curr_flux->EvtNum) )
	{ failedVtx++; cout << "Failed check for: EvtNum!"<< endl; }
      if( !Compare(EvtXSec, curr_flux->EvtXSec) )
	{ failedVtx++; cout << "Failed check for: EvtXSec!"<< endl; }
      if( !Compare(EvtDXSec, curr_flux->EvtDXSec) )
	{ failedVtx++; cout << "Failed check for: EvtDXSec!"<< endl; }
      if( !Compare(EvtWght, curr_flux->EvtWght) )
	{ failedVtx++; cout << "Failed check for: EvtWght!"<< endl; }
      if( !Compare(EvtProb, curr_flux->EvtProb) )
	{ failedVtx++; cout << "Failed check for: EvtProb!"<< endl; }
      for(int k=0; k<3; k++) {
	if( !Compare(EvtVtx[k]*1000, curr_flux->EvtVtx[k]) )
	  //*1000 to compare same spatial units, conversion occurs in ND::T(G/N)RooTrackerVtxModule::FillVtx
	  { failedVtx++; cout << "Failed check for: EvtVtx["<<k<<"]!"<< endl; }
      }
      if( !Compare(EvtVtx[3], curr_flux->EvtVtx[3]) )
	{ failedVtx++; cout << "Failed check for: EvtVtx[3]!"<< endl; }
      if( !Compare(StdHepN, curr_flux->StdHepN) )
	{ failedVtx++; cout << "Failed check for: StdHepN!"<< endl; }
      if( !Compare(StdHepPdg, curr_flux->StdHepPdg, StdHepN) )
	{ failedVtx++; cout << "Failed check for: StdHepPdg[]!"<< endl; }
      if( !Compare(StdHepStatus, curr_flux->StdHepStatus, StdHepN) )
	{ failedVtx++; cout << "Failed check for: StdHepStatus[]!"<< endl; }
      for(int k=0;k<StdHepN;k++){
	if( !Compare(StdHepX4[k][0], curr_flux->StdHepX4[k][0]) )
	  { failedVtx++; cout<< "Failed check for: StdHepX4["<<k<<"][0]!"<< endl; }
	if( !Compare(StdHepX4[k][1], curr_flux->StdHepX4[k][1]) )
	  { failedVtx++; cout<< "Failed check for: StdHepX4["<<k<<"][1]!"<< endl; }
	if( !Compare(StdHepX4[k][2], curr_flux->StdHepX4[k][2]) )
	  { failedVtx++; cout<< "Failed check for: StdHepX4["<<k<<"][2]!"<< endl; }
	if( !Compare(StdHepX4[k][3], curr_flux->StdHepX4[k][3]) )
	  { failedVtx++; cout<< "Failed check for: StdHepX4["<<k<<"][3]!"<< endl; }
	if( !Compare(StdHepP4[k][0], curr_flux->StdHepP4[k][0]) )
	  { failedVtx++; cout<< "Failed check for: StdHepP4["<<k<<"][0]!"<< endl; }
	if( !Compare(StdHepP4[k][1], curr_flux->StdHepP4[k][1]) )
	  { failedVtx++; cout<< "Failed check for: StdHepP4["<<k<<"][1]!"<< endl; }
	if( !Compare(StdHepP4[k][2], curr_flux->StdHepP4[k][2]) )
	  { failedVtx++; cout<< "Failed check for: StdHepP4["<<k<<"][2]!"<< endl; }
	if( !Compare(StdHepP4[k][3], curr_flux->StdHepP4[k][3]) )
	  { failedVtx++; cout<< "Failed check for: StdHepP4["<<k<<"][3]!"<< endl; }
	if( !Compare(StdHepPolz[k][0], curr_flux->StdHepPolz[k][0]) )
	  { failedVtx++; cout<< "Failed check for: StdHepPolz["<<k<<"][0]!"<< endl; }
	if( !Compare(StdHepPolz[k][1], curr_flux->StdHepPolz[k][1]) )
	  { failedVtx++; cout<< "Failed check for: StdHepPolz["<<k<<"][1]!"<< endl; }
	if( !Compare(StdHepPolz[k][2], curr_flux->StdHepPolz[k][2]) )
	  { failedVtx++; cout<< "Failed check for: StdHepPolz["<<k<<"][2]!"<< endl; }
      }
      if( !Compare(StdHepFd, curr_flux->StdHepFd, StdHepN) )
	{ failedVtx++; cout << "Failed check for: StdHepFd[]!"<< endl; }
      if( !Compare(StdHepLd, curr_flux->StdHepLd, StdHepN) )
	{ failedVtx++; cout << "Failed check for: StdHepLd[]!"<< endl; }
      if( !Compare(StdHepFm, curr_flux->StdHepFm, StdHepN) )
	{ failedVtx++; cout << "Failed check for: StdHepFm[]!"<< endl; }
      if( !Compare(StdHepLm, curr_flux->StdHepLm, StdHepN) )
	{ failedVtx++; cout << "Failed check for: StdHepLm[]!"<< endl; }

      //______________________________
      #if GENERATOR>0
      //genie specification comparisons
      if( !Compare(StdHepRescat, curr_flux->StdHepRescat, StdHepN) )
	{ failedVtx++; cout << "Failed check for: StdHepRescat[]!"<< endl; }
      if( !Compare(G2NeutEvtCode, curr_flux->G2NeutEvtCode) )
	{ failedVtx++; cout << "Failed check for: G2NeutEvtCode!"<< endl; }
      #elif GENERATOR<0
      //neut specific comparisons
      //NEneutmode is not passed through, because its redundant/unused
      //if( !Compare(NEneutmode, curr_flux->NEneutmode) )
      //  { failedVtx++; cout << "Failed check for: NEneutmode!"<< endl; }
      if( !Compare(NEnvc, curr_flux->NEnvc) )
	{ failedVtx++; cout << "Failed check for: NEnvc!"<< endl; }
      if( !Compare(NEipvc, curr_flux->NEipvc, NEnvc) )
	{ failedVtx++; cout << "Failed check for: NEipvc[]!"<< endl; }
      for(int k=0;k<NEnvc;k++){
	if( !Compare(NEpvc[k][0], curr_flux->NEpvc[k][0]) )
	  { failedVtx++; cout << "Failed check for: NEpvc["<<k<<"][0]!"<< endl; }
	if( !Compare(NEpvc[k][1], curr_flux->NEpvc[k][1]) )
	  { failedVtx++; cout << "Failed check for: NEpvc["<<k<<"][1]!"<< endl; }
	if( !Compare(NEpvc[k][2], curr_flux->NEpvc[k][2]) )
	  { failedVtx++; cout << "Failed check for: NEpvc["<<k<<"][2]!"<< endl; }
      }
      if( !Compare(NEiorgvc, curr_flux->NEiorgvc, NEnvc) )
	{ failedVtx++; cout << "Failed check for: NEiorgvc[]!"<< endl; }
      if( !Compare(NEiflgvc, curr_flux->NEiflgvc, NEnvc) )
	{ failedVtx++; cout << "Failed check for: NEiflgvc[]!"<< endl; }
      if( !Compare(NEicrnvc, curr_flux->NEicrnvc, NEnvc) )
	{ failedVtx++; cout << "Failed check for: NEicrnvc[]!"<< endl; }
      if( !Compare(NEnvert, curr_flux->NEnvert) )
	{ failedVtx++; cout << "Failed check for: NEnvert!"<< endl; }
      if( NEnvert >100 ) {
	cout << NEnvert << " NEnvert should be less than 100!" << endl;
	NEnvert=100;
	failedVtx++;
      }
      for(int k=0;k<NEnvert;k++){
	if( !Compare(NEposvert[k][0], curr_flux->NEposvert[k][0]) )
	  { failedVtx++; cout << "Failed check for: NEposvert["<<k<<"][0]!"<< endl; }
	if( !Compare(NEposvert[k][1], curr_flux->NEposvert[k][1]) )
	  { failedVtx++; cout << "Failed check for: NEposvert["<<k<<"][1]!"<< endl; }
	if( !Compare(NEposvert[k][2], curr_flux->NEposvert[k][2]) )
	  { failedVtx++; cout << "Failed check for: NEposvert["<<k<<"][2]!"<< endl; }
      }
      if( !Compare(NEiflgvert, curr_flux->NEiflgvert, NEnvert) )
	{ failedVtx++; cout << "Failed check for: NEiflgvert[]!"<< endl; }
      if( !Compare(NEnvcvert, curr_flux->NEnvcvert) )
	{ failedVtx++; cout << "Failed check for: NEnvcvert!"<< endl; }
      for(int k=0;k<NEnvcvert;k++){
	if( !Compare(NEdirvert[k][0], curr_flux->NEdirvert[k][0]) )
	  { failedVtx++; cout << "Failed check for: NEdirvert["<<k<<"][0]!"<< endl; }
	if( !Compare(NEdirvert[k][1], curr_flux->NEdirvert[k][1]) )
	  { failedVtx++; cout << "Failed check for: NEdirvert["<<k<<"][1]!"<< endl; }
	if( !Compare(NEdirvert[k][2], curr_flux->NEdirvert[k][2]) )
	  { failedVtx++; cout << "Failed check for: NEdirvert["<<k<<"][2]!"<< endl; }
      }
      if( !Compare(NEabspvert, curr_flux->NEabspvert, NEnvcvert) )
	{ failedVtx++; cout << "Failed check for: NEabspvert[]!"<< endl; }
      if( !Compare(NEabstpvert, curr_flux->NEabstpvert, NEnvcvert) )
	{ failedVtx++; cout << "Failed check for: NEabstpvert[]!"<< endl; }
      if( !Compare(NEipvert, curr_flux->NEipvert, NEnvcvert) )
	{ failedVtx++; cout << "Failed check for: NEipvert[]!"<< endl; }
      if( !Compare(NEiverti, curr_flux->NEiverti, NEnvcvert) )
	{ failedVtx++; cout << "Failed check for: NEiverti[]!"<< endl; }
      if( !Compare(NEivertf, curr_flux->NEivertf, NEnvcvert) )
	{ failedVtx++; cout << "Failed check for: NEivertf[]!"<< endl; }

      if( !Compare(NEcrsx, curr_flux->NEcrsx) )
	{ failedVtx++; cout << "Failed check for: NEcrsx!"<< endl; }
      if( !Compare(NEcrsy, curr_flux->NEcrsy) )
	{ failedVtx++; cout << "Failed check for: NEcrsy!"<< endl; }
      if( !Compare(NEcrsz, curr_flux->NEcrsz) )
	{ failedVtx++; cout << "Failed check for: NEcrsz!"<< endl; }
      if( !Compare(NEcrsphi, curr_flux->NEcrsphi) )
	{ failedVtx++; cout << "Failed check for: NEcrsphi!"<< endl; }
      #endif

      //some checks to make sure "Orig" variables don't change part way through the file
      if( numcorigfilename.CompareTo(curr_flux->OrigFileName->GetString()) )
	{ failedVtx++; cout << "Failed check for: OrigFileName!" << endl
			    << "\tCompared "<< numcorigfilename << " to " << curr_flux->OrigFileName->GetString() << endl; }
      if( !Compare((int)numctreeentries, curr_flux->OrigTreeEntries) )
	{ failedVtx++; cout << "Failed check for: OrigTreeEntries!" << endl; }
      if( numctreename.CompareTo(curr_flux->OrigTreeName->GetString()) )
	{ failedVtx++; cout << "Failed check for: OrigTreeName!" << endl
			    << "\tCompared " << numctreename << " to " << curr_flux->OrigTreeName->GetString() << endl; }
      if( !Compare(numctreepot, curr_flux->OrigTreePOT) )
	{ failedVtx++; cout << "Failed check for: OrigTreePOT!" << endl; }
      if( generatorname.CompareTo(curr_flux->GeneratorName->GetString()) )
	{ failedVtx++; cout << "Failed check for: GeneratorName!" << endl
			    << "\tCompared "<< generatorname << " to " << curr_flux->GeneratorName->GetString() << endl; }

      //bookeeping to output number of failures
      if(failedVtx>0) {cout << failedVtx << " failures in vtx " << jvtx << " of " << NVtx << endl; failedEvent+=failedVtx;}
    }//loop over vertex
    if(failedEvent>0) {cout << failedEvent << " failures in event " << jent << endl; failedTotal+=failedEvent;}
  }//loop over event

  //extra check in basic header module for the simulated beam POTPerSpill
  TChain basicHeaderChain = TChain("HeaderDir/BasicHeader");
  basicHeaderChain.AddFile(inputfilename);
  float POTPerSpill; basicHeaderChain.SetBranchAddress("POTPerSpill", &POTPerSpill);
  bool IsMC; basicHeaderChain.SetBranchAddress("IsMC", &IsMC);
  float expectedPOTPerSpill = 0;
  int failedHeader = 0;
  bool skip_POTPerSpill = false;
  if(beamcondition == "prod5Abeama")
    expectedPOTPerSpill = 3.66169E+13;
  else if(beamcondition == "prod5Abeamb")
    expectedPOTPerSpill = 7.98913E+13;
  else if(beamcondition == "prod6beamb")
    expectedPOTPerSpill = 7.9891E+13;
  else if(beamcondition == "prod6beamc")
    expectedPOTPerSpill = 9.462526E+13;
  else if(beamcondition == "basket")
    expectedPOTPerSpill = 0;
  else
    {cout << "Unknown beamcondition. Skipping POTPerSpill check..."; skip_POTPerSpill = true;}
  for(int i = 0; i < basicHeaderChain.GetEntries(); i++) {
    basicHeaderChain.GetEntry(i);
    if (!skip_POTPerSpill && !Compare(expectedPOTPerSpill, POTPerSpill))
      {failedHeader++; cout << "Failed check for: POTPerSpill!" << endl;}
    if (!Compare(1, IsMC))
      {failedHeader++; cout << "Failed check for: IsMC!" << endl;}
  }
  if(failedHeader>0) {cout << failedHeader << " failures in header " << endl; failedTotal+=failedHeader;}

  //Finishing off
  cout << "\n******** " << failedTotal << " failures in total!" << endl;
  cout << "Analysed file was " << inputfilename << endl;

  delete Vtx;
  delete rooTrackerNumcChain;
  return;
}


//_____________________________________________________________________________
//**************************************************************************
//helper functions
//_____________________________________________________________________________
bool Compare(double a, double b){
  bool arethesame = TMath::Abs(a-b) < fASmallNum;
#ifdef __DEBUG__
  arethesame = false;
#endif
  if(!arethesame){ cout << "Compared: "<< a <<" to " << b << " --> diff = "<< a-b << endl; }
  return arethesame;
}

//_____________________________________________________________________________
bool Compare(float a, float b){
  bool arethesame = TMath::Abs(a-b) < fASmallNum;
#ifdef __DEBUG__
  arethesame = false;
#endif
  if(!arethesame){ cout << "Compared: "<< a <<" to " << b << " --> diff = "<< a-b << endl; }
  return arethesame;
}

//_____________________________________________________________________________
bool Compare(int a, int b){
  bool arethesame = TMath::Abs(a-b) < fASmallNum;
#ifdef __DEBUG__
  arethesame = false;
#endif
  if(!arethesame){ cout << "Compared: "<< a <<" to " << b << " --> diff = "<< a-b << endl; }
  return arethesame;
}

//_____________________________________________________________________________
bool Compare(long a, long b){
  bool arethesame = TMath::Abs(a-b) < fASmallNum;
#ifdef __DEBUG__
  arethesame = false;
#endif
  if(!arethesame){ cout << "Compared: "<< a <<" to " << b << " --> diff = "<< a-b << endl; }
  return arethesame;
}

//_____________________________________________________________________________
bool Compare(float a[], float b[], int ni){
  for(int i=0; i<ni; i++){ if(Compare(a[i],b[i]) == false){ return false;} }
  return true;
}

//_____________________________________________________________________________
bool Compare(double a[], double b[], int ni){
  for(int i=0; i<ni; i++){ if(Compare(a[i],b[i]) == false){ return false;} }
  return true;
}

//_____________________________________________________________________________
bool Compare(int a[], int b[], int ni){
  for(int i=0; i<ni; i++){ if(Compare(a[i],b[i]) == false){ return false;} }
  return true;
}

