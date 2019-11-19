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
and should only require an input MC analysis file and the version of ROOT you
installed when installing the nd280 software (this is because you need a rooti
for which the python module was enables).

Always run the macros from the directory in which you ran runAnalysis.py with
the -c option (if you have not done this then please read the EXAMPLES_README
and the analysis turorial again). There are two options as to how to run
the macro:

----------
1) It can be run in CINT interpreter mode from a root session running anywhere
using:

[root] .x ${ND280ANALYSISTOOLSROOT}/AnalysisTools/oaAnalysisReadInitFile-\
${ND280ANALYSISTOOLSCONFIG}.C
[root] .L exampleAnalysisScript.C;
[root] exampleAnalysisScript("path/to/MC/analysisfile.root", nevents);


You could avoid having to run execute
oaAnalysisReadInitFile-${ND280ANALYSISTOOLSCONFIG}.C each time if
running in CINT mode by adding the line,
gROOT->ProcessLine(".x ${ND280ANALYSISTOOLSROOT}/AnalysisTools/\
oaAnalysisReadInitFile-${ND280ANALYSISTOOLSCONFIG}.C"); at the
beginning of this analysis script - if you do this you need to ensure it is
not run when using ACLiC. See commented out code at beginning of function as
example.

----------
2) Or as compiled code (using ACLiC) by appending a '+' sign to the .L
command:
[root] .x ${ND280ANALYSISTOOLSROOT}/AnalysisTools/oaAnalysisReadInitFile-\
${ND280ANALYSISTOOLSCONFIG}.C
[root] .L exampleAnalysisScript.C+;
[root] exampleAnalysisScript("path/2/MC/analysisfile.root", nevents);

For a rapid development cycle you can stay in the same root session and do not
have to repeat the first step when recompiling the analysis script to reflect
any changes you have made. This works for CINT or ACLiC:
[root] .x ${ND280ANALYSISTOOLSROOT}/AnalysisTools/oaAnalysisReadInitFile-\
${ND280ANALYSISTOOLSCONFIG}.C
[root] .L exampleAnalysisScript.C+;
[root] exampleAnalysisScript("path/2/MC/analysisfile.root", nevents);
... make any changes to exampleAnalysisScript.C ...
[root] .L exampleAnalysisScript.C+;
[root] exampleAnalysisScript("path/2/MC/analysisfile.root", nevents);

*/
#include <iostream>

#include "TROOT.h"
#include "TFile.h"
#include "TChain.h"
#include "TClonesArray.h"

// To use AnalysisTools utils in ACLiC mode then need to add the relevant header
// here and inside the ifdef so it is not seen by CINT.
#if !defined(__CINT__) || defined(__MAKECINT__)
#include <AnalysisTools/TCheatingAnalysisUtils.hxx>
#endif

using std::cout;
using std::endl;

void exampleAnalysisScript(char * inputfilename, int max_entries = -1){
  gROOT->SetBatch(1);
// See commented code below for loading libraries automatically.
//#if !defined(__MAKECINT__)
//  // This should not be seen when using ACLiC.
//  gROOT->ProcessLine(".x ${ND280ANALYSISTOOLSROOT}/AnalysisTools/\\
//  oaAnalysisReadInitFile-${ND280ANALYSISTOOLSCONFIG}.C");
//#endif
  // _________________________________________________________________________
  // Prepare to read trees.
  //
  // Make a chain for the Trajectories tree stored in the TruthDir. This needs
  // to be done for each analysis tree you want to get information from.
  TChain trajTreeChain = TChain("TruthDir/Trajectories");
  std::cout << "Adding file(s) matching string: " << inputfilename << std::endl;
  std::cout << "Added "<< trajTreeChain.AddFile(inputfilename) << " file(s)"<< std::endl;
  Int_t NTraj;
  TClonesArray *Trajectories =
       new TClonesArray("ND::TTruthTrajectoriesModule::TTruthTrajectory", 100);
  // Set the branches for the TClonesArray of oaanalysis objects (as defined in
  // the relevant module) as well as the integer that stores the number of
  // these stored in the TClonesArray.
  trajTreeChain.SetBranchAddress("NTraj", &NTraj);
  trajTreeChain.SetBranchAddress("Trajectories", &Trajectories);

  // Get a utilities class for use with the oaanalysis objects. In this case
  // use the cheating utils as it contains methods for querying truth
  // trajectories.
  TCheatingAnalysisUtils cheatUtils;

  // _________________________________________________________________________
  // Now loop over the entries in the input tree. Each entry corresponds to an
  // ND280Event and it is safe to assume a correspondence between entries on
  // different trees in the same analysis output file.
  int entries = trajTreeChain.GetEntries();
  if(max_entries > 0 && max_entries<entries) entries = max_entries;
  std::cout << "Looping over " << entries<< " events " << std::endl;
  for(int ient = 0; ient < entries; ient++)
  {
      trajTreeChain.GetEntry(ient);
      cout << "\n======== Getting event/entry "<< ient <<" ==========" << endl;
      cout << "     -> the TClonesArray contains "<< NTraj
                                                 << " trajectories" << endl;
      // Now loop over the lepton trajectories we have found.
      ND::TTruthTrajectoriesModule::TTruthTrajectory * curr_traj;
      for(int i=0;i<NTraj;i++){
        // Example of accessing a data member of a TruthTrajectory.
        curr_traj = (ND::TTruthTrajectoriesModule::TTruthTrajectory *) Trajectories->At(i);
        cout << "              ---> trajectory "<< i << " is for particle with pdg = "<< curr_traj->PDG << endl;
        // Example of calling an AnalysisUtils method.
        bool useful = cheatUtils.IsAUsefulTPCTrack((*curr_traj));
        cout << "                   is it a usefull TPC track bool = "<< useful << endl;
      }

  } // eventloop

  return;
}

