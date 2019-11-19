/*
This is an example .C analysis macro to demonstrate how to loop over an 
oaAnalysis output file and access the TClonesArrays of analysis summary 
objects stored in the various output trees. It also shows how to call
an analysis utility from the AnalysisUtils directory. It has the same
functionality as its python equivalent except that no output tree is saved. 

It is important to note that neither this script or its python equivalent
link to any of the nd280 softwarei. This includes anything in the 
oaAnalysis src directory (with the possible exception of oaAnalysisUtilsModule).
This is important as to run the macros you should not need to compile oaAnalysis
and should only require an input analysis file and the version of ROOT you 
installed when installing the nd280 software (this is because you need a rooti
for which the python module was enables). 

Always run the macros from the directory in which you ran runAnalysis.py with 
the -c option (if you have not done this then please read the EXAMPLES_README
and the analysis turorial again). There are two options as to how to run
the macro: 

----------
1) It can be run in CINT interpreter mode from a root session running in the
 macros directory:

[root].x oaAnalysisReadInitFile.C;
[root].L exampleAnalysisScript.C;
[root]exampleAnalysisScript("path/to/analysisfile.root", nevents);

You could avoid having to run .x oaAnalysisReadInitFile.C each time if
running in CINT mode by adding the line,
gROOT->ProcessLine(".x oaAnalysisReadInitFile.C;"); at the begging of this
analysis script - if you do this you need to ensure it is not run when using 
ACLiC. See commented out code at begginging of function as example.

----------
2) Or as compiled code (using ACLiC) by appending a '+' sign to the .L  
command:
[root].x oaAnalysisReadInitFile.C;
[root].L exampleAnalysisScript.C+;
[root]exampleAnalysisScript("path/2/analysisfile.root", nevents);

For a rapid development cycle you can stay in the same root session and do not
have to repeat the first step when recompiling the analysis script to reflect
any changes you have made. This works for CINT or ACLiC:
[root].x oaAnalysisReadInitFile.C;
[root].L exampleAnalysisScript.C+;
[root]exampleAnalysisScript("path/2/analysisfile.root", nevents);
... make any changes to exampleAnalysisScript.C ...
[root].L exampleAnalysisScript.C+;
[root]exampleAnalysisScript("path/2/analysisfile.root", nevents);

*/
#include <iostream>

#include "TROOT.h"
#include "TFile.h"
#include "TChain.h"
#include "TClonesArray.h"

using std::cout;
using std::endl;

void exampleRooTrackScript(char * inputfilename, int max_entries = -1){
  gROOT->SetBatch(1);
// See commented code below for loading libraries automatically.
//#if !defined(__MAKECINT__)
//  // This should not be seen when using ACLiC.
//  gROOT->ProcessLine(".x exampleRooTrackScript.C");
//#endif
  // _________________________________________________________________________
  // Prepare to read trees.
  //
  // Make a chain for the Trajectories tree stored in the TruthDir. This needs
  // to be done for each analysis tree you want to get information from.
  TChain trajTreeChain = TChain("TruthDir/Trajectories");
  TChain nRooTracTreeChain = TChain("TruthDir/NRooTrackerVtx");
  //std::cout << "Adding file(s) matching string: " << inputfilename << std::endl;
   trajTreeChain.AddFile(inputfilename);// << " file(s)"<< std::endl;
  nRooTracTreeChain.AddFile(inputfilename);
  Int_t NTrajsLepton;
  Int_t NRooTrackerVtx;
  TClonesArray *TrajsLepton = 
       new TClonesArray("ND::TTruthTrajectoriesModule::TTruthTrajectory", 100);
  TClonesArray *nrootrac = 
       new TClonesArray("ND::NRooTrackerVtx", 100);
  // Set the branches for the TClonesArray of oaanalysis objects (as defined in
  // the relevant module) as well as the integer that stores the number of 
  // these stored in the TClonesArray. 
  trajTreeChain.SetBranchAddress("NTrajsLepton", &NTrajsLepton);
  trajTreeChain.SetBranchAddress("TrajsLepton", &TrajsLepton);

  nRooTracTreeChain.SetBranchAddress("NVtx", &NRooTrackerVtx);
  nRooTracTreeChain.SetBranchAddress("Vtx", &nrootrac);

  // _________________________________________________________________________
  // Now loop over the entries in the input tree. Each entry corresponds to an
  // ND280Event and it is safe to assume a correspondence between entries on 
  // different trees in the same analysis output file.  
  int entries = nRooTracTreeChain.GetEntries();
  if(max_entries > 0 && max_entries<entries) entries = max_entries;
  std::cout << "Looping over " << entries<< " events " << std::endl;
  for(int ient = 0; ient < entries; ient++)
  {
      nRooTracTreeChain.GetEntry(ient);
      cout << "\n======== Getting event/entry "<< ient <<" ==========" << endl;
      cout << "     -> the TClonesArray contains "<< NTrajsLepton 
                                                 << " lepton trajectories" << endl;
      // Now loop over the lepton trajectories we have found.
      ND::NRooTrackerVtx * curr_vtx;
      for(int i=0;i<NRooTrackerVtx;i++){
        // Example of accessing a data member of a TruthTrajectory.
        curr_vtx = (ND::NRooTrackerVtx *) nrootrac->At(i);

	
        cout << "              ---> vertex "<< i << endl;

	//cout << " Unique ID  = " << curr_vtx->fUniqueID << endl;
	//cout << " Bits  = " << curr_vtx->fBits << endl;
	//cout << " Evt Flags  = " << curr_vtx->EvtFlags << endl;
	cout << " Neut code  = " << curr_vtx->EvtCode->GetString() << endl;
	cout << " Evt Num  = " << curr_vtx->EvtNum << endl;
	cout << " Evt XSec  = " << curr_vtx->EvtXSec << endl;
	cout << " Evt DXSec  = " << curr_vtx->EvtDXSec << endl;
	cout << " Evt Wght  = " << curr_vtx->EvtWght << endl;
	cout << " Evt Prob  = " << curr_vtx->EvtProb << endl;

	cout << " Evt Vtx  = ";
	for (int i=0; i<4; i++)
	  cout << curr_vtx->EvtVtx[i] << " ";
	cout << endl;

	int Npvc = curr_vtx->StdHepN;

	cout << " N  = " << Npvc << endl;

	cout << endl;
	for (int ipvc=0; ipvc<Npvc; ipvc++) {
	  cout << "Particle #: " << ipvc << endl;


	  cout << " Ipvc =  " <<  curr_vtx->StdHepPdg[ipvc] << endl;
	  cout << " Ichvc =  " << curr_vtx->StdHepStatus[ipvc] << endl;     // generator-specific status code
	  
	  cout << " Vertex in Nucl =  ";
	  for (int i=0; i<4; i++)
	    cout << curr_vtx->StdHepX4[ipvc][i] << " ";
	  cout << endl;

	  cout << " pmom =  ";
	  for (int i=0; i<4; i++)
	    cout << curr_vtx->StdHepP4[ipvc][i] << " ";
	  cout << endl;

	  cout << " polariz. =  ";
	  for (int i=0; i<3; i++)
	    cout << curr_vtx->StdHepPolz[ipvc][i] << " ";  // polarization vector
	  cout << endl;

	  cout << " 1st daughter =  " << 	  curr_vtx->StdHepFd[ipvc] << endl;     // first daughter
	  cout << " last daughter =  " << 	  curr_vtx->StdHepLd[ipvc] << endl;     // last  daughter 
	  cout << " 1st mother =  " << 	  curr_vtx->StdHepFm[ipvc] << endl;     // first mother
	  cout << " last mother =  " << 	  curr_vtx->StdHepLm[ipvc] << endl;     // last  mother

	  cout << endl;
	}

	cout << endl << "Beam ntuple data:" << endl;
	cout <<         "-----------------" << endl;

	cout << " Neutrino Parent ID = " << curr_vtx->NuParentPdg << endl;
	cout << " Parent Decay mode = " << curr_vtx->NuParentDecMode << endl;
	cout << " Parent momentum at vertex = ";
	for (int i=0; i<4; i++)
	  cout << curr_vtx->NuParentDecP4[i] << " ";
	cout << endl;

	cout << " Parent decay vertex = ";
	for (int i=0; i<4; i++)
	  cout << curr_vtx->NuParentDecX4[i] << " ";
	cout << endl;

	cout << " Parent initial momentum = ";
	for (int i=0; i<4; i++)
	  cout << curr_vtx->NuParentProP4[i] << " ";
	cout << endl;

	cout << " Parent initial vertex = ";
	for (int i=0; i<4; i++)
	  cout << curr_vtx->NuParentProX4[i] << " ";
	cout << endl;

	cout << " Vertex ID = " << curr_vtx->NuParentProNVtx << endl;

	
	cout << " Primary Particle ID = " << curr_vtx->NuGipart << endl;
	
	cout << " Primary Particle Pos = ";
	for (int i=0; i<3; i++)
	  cout << curr_vtx->NuGpos0[i] << " ";
	cout << endl;

	cout << " Primary Particle Dir = ";
	for (int i=0; i<3; i++)
	  cout << curr_vtx->NuGvec0[i] << " ";
	cout << endl;

	cout << " Primary Particle Momentum = " << curr_vtx->NuGamom0 << endl;
	
	/**/
	


	cout << endl;

      }

  } // eventloop

  return;
}

