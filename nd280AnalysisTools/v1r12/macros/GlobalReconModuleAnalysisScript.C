/*
This is an example .C analysis macro to demonstrate how to loop over the
GlobalReconModule in the oaAnalysis output file and access the TClonesArrays
of analysis summary objects and fill histograms which are saved in a file.
It also shows how to call an analysis utility from the AnalysisUtils directory.

To run the macro you should not need to compile oaAnalysis
and should only require an input analysis file and the version of ROOT you
installed when installing the nd280 software (this is because you need a rooti
for which the python module was enables).

To setup and compile the analysis tools libraries to interpret analysis files:
'cd ../cmt; source setup.sh; make analysisTools'

There is are 2 options as to how to run the macro:

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

The selection flag can be used to only fill selected objects for PID,
PIDalternates or ECalObjects histograms:
selection=1, PID electrons
selection=2, PID muons
selection=3, PIDalternate electrons
selection=4, PIDalternate muons
selection=5, PIDalternate protons
selection=6, ECalObjects tracks
selection=7, ECalobjects showers
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
[root] .L GlobalReconModuleAnalysisScript.C+
[root] GlobalReconModuleAnalysisScript("path/2/analysisfile.root", selection, \
nevents);
... make any changes to GlobalReconModuleAnalysisScript.C ...
[root] .L GlobalReconModuleAnalysisScript.C+
[root] GlobalReconModuleAnalysisScript("path/2/analysisfile.root", selection, \
nevents);

*/
#include <iostream>

#include "TROOT.h"
#include "TFile.h"
#include "TChain.h"
#include "TClonesArray.h"
#include "TH1F.h"
#include <AnalysisTools/libReadoaAnalysis/libReadoaAnalysisProjectHeaders.h>

// To use AnalysisTools utils in ACLiC mode then need to add the relevant header
// here and inside the ifdef so it is not seen by CINT.
//#if !defined(__CINT__) || defined(__MAKECINT__)
//#include <AnalysisTools/TCheatingAnalysisUtils.hxx>
//#endif

using std::cout;
using std::endl;

void GlobalReconModuleAnalysisScript(char * inputfilename, int selection = 0, int max_entries = -1){
  gROOT->SetBatch(1);
// See commented code below for loading libraries automatically.
// --Removed 2014-7-2 as it is no longer valid, the ReadInitFile is now named
// oaAnalysisReadInitFile-$ND280ANALYSISTOOLSCONFIG.C
// #if !defined(__MAKECINT__)
//   // This should not be seen when using ACLiC.
//  gROOT->ProcessLine(".x ${ND280ANALYSISTOOLSROOT}/AnalysisTools/\\
// oaAnalysisReadInitFile-${ND280ANALYSISTOOLSCONFIG}.C");
// #endif
  // _________________________________________________________________________
  // Prepare to read trees.
  //
  // Make a chain for the Global tree stored in the ReconDir. This needs
  // to be done for each analysis tree you want to get information from.

  TChain globTreeChain = TChain("ReconDir/Global");

  TH1F *h_runid = new TH1F("h_runid", "RunID", 5, 0., 5.);
  TH1F *h_eventid = new TH1F("h_eventid", "EventID", 1000, 700000., 701000.);
  TH1F *h_preselected = new TH1F("h_preselected", "Preselected", 5, 0., 5.);
  TH1F *h_subrunid = new TH1F("h_subrunid", "SubrunID", 10, 0., 10.);

  TH1F *h_nvertices = new TH1F("h_nvertices", "NVertices", 5, 0., 5.);

  TH1F *h_vert_primaryindex = new TH1F("h_vert_primaryindex", "PrimaryIndex", 10, -5., 5.);
  TH1F *h_vert_status = new TH1F("h_vert_status", "Status", 100, 0., 1000000000.);
  TH1F *h_vert_quality = new TH1F("h_vert_quality", "Quality", 700, 0., 70000000.);
  TH1F *h_vert_ndof = new TH1F("h_vert_ndof", "NDOF", 5, 0., 5.);

  TH1F *h_vert_ntruevertices = new TH1F("h_vert_ntruevertices", "NTrueVertices", 5, 0., 5.);
  TH1F *h_vert_nconstituents = new TH1F("h_vert_nconstituents", "NConstituents", 5, 0., 5.);

  TH1F *h_vert_pos_x = new TH1F("h_vert_pos_x", "X()", 140, -1200., 1200.);
  TH1F *h_vert_pos_y = new TH1F("h_vert_pos_y", "Y()", 140, -1200., 1200.);
  TH1F *h_vert_pos_z = new TH1F("h_vert_pos_z", "Z()", 150, -800., 2200.);
  TH1F *h_vert_pos_t = new TH1F("h_vert_pos_t", "T()", 50, 2000., 7000.);
  TH1F *h_vert_pos_px = new TH1F("h_vert_pos_px", "Px()", 140, -1200., 1200.);
  TH1F *h_vert_pos_py = new TH1F("h_vert_pos_py", "Py()", 140, -1200., 1200.);
  TH1F *h_vert_pos_pz = new TH1F("h_vert_pos_pz", "Pz()", 50, 2000., 7000.);
  TH1F *h_vert_pos_energy = new TH1F("h_vert_pos_energy", "Energy()", 100, 0., 10000.);
  TH1F *h_vert_pos_theta = new TH1F("h_vert_pos_theta", "Theta()", 140, -1200., 1200.);
  TH1F *h_vert_pos_costheta = new TH1F("h_vert_pos_costheta", "CosTheta()", 140, -1200., 1200.);
  TH1F *h_vert_pos_phi = new TH1F("h_vert_pos_phi", "Phi()", 80, -4., 4.);
  TH1F *h_vert_pos_rho = new TH1F("h_vert_pos_rho", "Rho()", 100, 0., 10000.);
  TH1F *h_vert_pos_perp2 = new TH1F("h_vert_pos_perp2", "Perp2()", 120, 0., 12000.);
  TH1F *h_vert_pos_pt = new TH1F("h_vert_pos_pt", "Pt()", 200, 0., 2000.);
  TH1F *h_vert_pos_perp = new TH1F("h_vert_pos_perp", "Perp()", 100, 0., 5000.);
  TH1F *h_vert_pos_et2 = new TH1F("h_vert_pos_et2", "Et2()", 100, 0., 100000.);

	//P0DOutermostHits

  TH1F *h_p0doutermosthits_xmax_charge   = new TH1F("h_p0doutermosthits_xmax_charge", "P0D Outermost Hits Max X: Charge", 55, 0, 1100);
  TH1F *h_p0doutermosthits_xmax_time     = new TH1F("h_p0doutermosthits_xmax_time",   "P0D Outermost Hits Max X: Time", 140, 0, 14000);
  TH1F *h_p0doutermosthits_xmax_pos_x    = new TH1F("h_p0doutermosthits_xmax_pos_x",  "P0D Outermost Hits Max X: X", 60, -1200, 1200);
  TH1F *h_p0doutermosthits_xmax_pos_y    = new TH1F("h_p0doutermosthits_xmax_pos_y",  "P0D Outermost Hits Max X: Y", 200, -2, -1);
  TH1F *h_p0doutermosthits_xmax_pos_z    = new TH1F("h_p0doutermosthits_xmax_pos_z",  "P0D Outermost Hits Max X: Z", 45, -3500, -500);
  TH1F *h_p0doutermosthits_xmax_dpos_x   = new TH1F("h_p0doutermosthits_xmax_dpos_x", "P0D Outermost Hits Max X: X Error", 50, 0, 5);
  TH1F *h_p0doutermosthits_xmax_dpos_y   = new TH1F("h_p0doutermosthits_xmax_dpos_y", "P0D Outermost Hits Max X: Y Error", 100, 0, 300);
  TH1F *h_p0doutermosthits_xmax_dpos_z   = new TH1F("h_p0doutermosthits_xmax_dpos_z", "P0D Outermost Hits Max X: Z Error", 100, 0, 2);
  TH1F *h_p0doutermosthits_xmax_type     = new TH1F("h_p0doutermosthits_xmax_type",   "P0D Outermost Hits Max X: Type", 40, 95, 115);

  TH1F *h_p0doutermosthits_xmin_charge   = new TH1F("h_p0doutermosthits_xmin_charge", "P0D Outermost Hits Min X: Charge", 55, 0, 1100);
  TH1F *h_p0doutermosthits_xmin_time     = new TH1F("h_p0doutermosthits_xmin_time",   "P0D Outermost Hits Min X: Time", 140, 0, 14000);
  TH1F *h_p0doutermosthits_xmin_pos_x    = new TH1F("h_p0doutermosthits_xmin_pos_x",  "P0D Outermost Hits Min X: X", 60, -1200, 1200);
  TH1F *h_p0doutermosthits_xmin_pos_y    = new TH1F("h_p0doutermosthits_xmin_pos_y",  "P0D Outermost Hits Min X: Y", 200, -2, -1);
  TH1F *h_p0doutermosthits_xmin_pos_z    = new TH1F("h_p0doutermosthits_xmin_pos_z",  "P0D Outermost Hits Min X: Z", 45, -3500, -500);
  TH1F *h_p0doutermosthits_xmin_dpos_x   = new TH1F("h_p0doutermosthits_xmin_dpos_x", "P0D Outermost Hits Min X: X Error", 50, 0, 5);
  TH1F *h_p0doutermosthits_xmin_dpos_y   = new TH1F("h_p0doutermosthits_xmin_dpos_y", "P0D Outermost Hits Min X: Y Error", 100, 0, 300);
  TH1F *h_p0doutermosthits_xmin_dpos_z   = new TH1F("h_p0doutermosthits_xmin_dpos_z", "P0D Outermost Hits Min X: Z Error", 100, 0, 2);
  TH1F *h_p0doutermosthits_xmin_type     = new TH1F("h_p0doutermosthits_xmin_type",   "P0D Outermost Hits Min X: Type", 40, 95, 115);

  TH1F *h_p0doutermosthits_ymax_charge   = new TH1F("h_p0doutermosthits_ymax_charge", "P0D Outermost Hits Max Y: Charge", 55, 0, 1100);
  TH1F *h_p0doutermosthits_ymax_time     = new TH1F("h_p0doutermosthits_ymax_time",   "P0D Outermost Hits Max Y: Time", 140, 0, 14000);
  TH1F *h_p0doutermosthits_ymax_pos_x    = new TH1F("h_p0doutermosthits_ymax_pos_x",  "P0D Outermost Hits Max Y: X", 50, -40, -30);
  TH1F *h_p0doutermosthits_ymax_pos_y    = new TH1F("h_p0doutermosthits_ymax_pos_y",  "P0D Outermost Hits Max Y: Y", 60, -1500, 1500);
  TH1F *h_p0doutermosthits_ymax_pos_z    = new TH1F("h_p0doutermosthits_ymax_pos_z",  "P0D Outermost Hits Max Y: Z", 45, -3500, -500);
  TH1F *h_p0doutermosthits_ymax_dpos_x   = new TH1F("h_p0doutermosthits_ymax_dpos_x", "P0D Outermost Hits Max Y: X Error", 50, 0, 250);
  TH1F *h_p0doutermosthits_ymax_dpos_y   = new TH1F("h_p0doutermosthits_ymax_dpos_y", "P0D Outermost Hits Max Y: Y Error", 50, 0, 5);
  TH1F *h_p0doutermosthits_ymax_dpos_z   = new TH1F("h_p0doutermosthits_ymax_dpos_z", "P0D Outermost Hits Max Y: Z Error", 100, 0, 2);
  TH1F *h_p0doutermosthits_ymax_type     = new TH1F("h_p0doutermosthits_ymax_type",   "P0D Outermost Hits Max Y: Type", 40, 95, 115);

  TH1F *h_p0doutermosthits_ymin_charge   = new TH1F("h_p0doutermosthits_ymin_charge", "P0D Outermost Hits Min Y: Charge", 55, 0, 1100);
  TH1F *h_p0doutermosthits_ymin_time     = new TH1F("h_p0doutermosthits_ymin_time",   "P0D Outermost Hits Min Y: Time", 140, 0, 14000);
  TH1F *h_p0doutermosthits_ymin_pos_x    = new TH1F("h_p0doutermosthits_ymin_pos_x",  "P0D Outermost Hits Min Y: X", 50, -40, -30);
  TH1F *h_p0doutermosthits_ymin_pos_y    = new TH1F("h_p0doutermosthits_ymin_pos_y",  "P0D Outermost Hits Min Y: Y", 60, -1500, 1500);
  TH1F *h_p0doutermosthits_ymin_pos_z    = new TH1F("h_p0doutermosthits_ymin_pos_z",  "P0D Outermost Hits Min Y: Z", 45, -3500, -500);
  TH1F *h_p0doutermosthits_ymin_dpos_x   = new TH1F("h_p0doutermosthits_ymin_dpos_x", "P0D Outermost Hits Min Y: X Error", 50, 0, 250);
  TH1F *h_p0doutermosthits_ymin_dpos_y   = new TH1F("h_p0doutermosthits_ymin_dpos_y", "P0D Outermost Hits Min Y: Y Error", 50, 0, 5);
  TH1F *h_p0doutermosthits_ymin_dpos_z   = new TH1F("h_p0doutermosthits_ymin_dpos_z", "P0D Outermost Hits Min Y: Z Error", 100, 0, 2);
  TH1F *h_p0doutermosthits_ymin_type     = new TH1F("h_p0doutermosthits_ymin_type",   "P0D Outermost Hits Min Y: Type", 40, 95, 115);

  TH1F *h_p0doutermosthits_zmax_charge   = new TH1F("h_p0doutermosthits_zmax_charge", "P0D Outermost Hits Max Z: Charge", 55, 0, 1100);
  TH1F *h_p0doutermosthits_zmax_time     = new TH1F("h_p0doutermosthits_zmax_time",   "P0D Outermost Hits Max Z: Time", 140, 0, 14000);
  TH1F *h_p0doutermosthits_zmax_pos_x    = new TH1F("h_p0doutermosthits_zmax_pos_x",  "P0D Outermost Hits Max Z: X", 60, -1500, 1500);
  TH1F *h_p0doutermosthits_zmax_pos_y    = new TH1F("h_p0doutermosthits_zmax_pos_y",  "P0D Outermost Hits Max Z: Y", 60, -1500, 1500);
  TH1F *h_p0doutermosthits_zmax_pos_z    = new TH1F("h_p0doutermosthits_zmax_pos_z",  "P0D Outermost Hits Max Z: Z", 45, -3500, -500);
  TH1F *h_p0doutermosthits_zmax_dpos_x   = new TH1F("h_p0doutermosthits_zmax_dpos_x", "P0D Outermost Hits Max Z: X Error", 60, -10, 280);
  TH1F *h_p0doutermosthits_zmax_dpos_y   = new TH1F("h_p0doutermosthits_zmax_dpos_y", "P0D Outermost Hits Max Z: Y Error", 60, -10, 280);
  TH1F *h_p0doutermosthits_zmax_dpos_z   = new TH1F("h_p0doutermosthits_zmax_dpos_z", "P0D Outermost Hits Max Z: Z Error", 100, 0, 2);
  TH1F *h_p0doutermosthits_zmax_type     = new TH1F("h_p0doutermosthits_zmax_type",   "P0D Outermost Hits Max Z: Type", 40, 95, 115);

  TH1F *h_p0doutermosthits_zmin_charge   = new TH1F("h_p0doutermosthits_zmin_charge", "P0D Outermost Hits Min Z: Charge", 55, 0, 1100);
  TH1F *h_p0doutermosthits_zmin_time     = new TH1F("h_p0doutermosthits_zmin_time",   "P0D Outermost Hits Min Z: Time", 140, 0, 14000);
  TH1F *h_p0doutermosthits_zmin_pos_x    = new TH1F("h_p0doutermosthits_zmin_pos_x",  "P0D Outermost Hits Min Z: X", 60, -1500, 1500);
  TH1F *h_p0doutermosthits_zmin_pos_y    = new TH1F("h_p0doutermosthits_zmin_pos_y",  "P0D Outermost Hits Min Z: Y", 60, -1500, 1500);
  TH1F *h_p0doutermosthits_zmin_pos_z    = new TH1F("h_p0doutermosthits_zmin_pos_z",  "P0D Outermost Hits Min Z: Z", 45, -3500, -500);
  TH1F *h_p0doutermosthits_zmin_dpos_x   = new TH1F("h_p0doutermosthits_zmin_dpos_x", "P0D Outermost Hits Min Z: X Error", 60, -10, 280);
  TH1F *h_p0doutermosthits_zmin_dpos_y   = new TH1F("h_p0doutermosthits_zmin_dpos_y", "P0D Outermost Hits Min Z: Y Error", 60, -10, 280);
  TH1F *h_p0doutermosthits_zmin_dpos_z   = new TH1F("h_p0doutermosthits_zmin_dpos_z", "P0D Outermost Hits Min Z: Z Error", 100, 0, 2);
  TH1F *h_p0doutermosthits_zmin_type     = new TH1F("h_p0doutermosthits_zmin_type",   "P0D Outermost Hits Min Z: Type", 40, 95, 115);


  //TGlobalPID

  TH1F *h_npids = new TH1F("h_npids", "NPIDs", 35, 0., 35.);

  TH1F *h_pid_uid = new TH1F("h_pid_uid", "UniqueID", 100, 0., 1600000.);
  TH1F *h_pid_buid = new TH1F("h_pid_buid", "BrokenUniqueIDs", 100, 0., 1600000.);
  TH1F *h_pid_det = new TH1F("h_pid_det", "Detectors", 2501, -0.5, 2500.5);
  TH1F *h_pid_detu = new TH1F("h_pid_detu", "DetectorUsed", 100, -0.5, 1.5);
  TH1F *h_pid_status = new TH1F("h_pid_status", "Status", 100, -0.5, 1.5);
  TH1F *h_pid_ndof = new TH1F("h_pid_ndof", "NDOF", 100, 0., 600.);
  TH1F *h_pid_chi2= new TH1F("h_pid_chi2","Chi2", 100, 0., 100.);
  TH1F *h_pid_nnodes = new TH1F("h_pid_nnodes", "NNodes", 100, 0., 100.);
  TH1F *h_pid_nhits = new TH1F("h_pid_nhits", "NHits", 100, 0., 150.);
  TH1F *h_pid_nconstituents = new TH1F("h_pid_nconstituents", "NConstituents", 10, -0.5, 9.5);
  TH1F *h_pid_isf = new TH1F("h_pid_isf", "isForward", 100, -0.5, 1.5);
  TH1F *h_pid_sok = new TH1F("h_pid_sok", "SenseOK", 100, -0.5, 1.5);
  TH1F *h_pid_charge = new TH1F("h_pid_charge", "Charge", 100, -1.5, 1.5);
  TH1F *h_pid_edep = new TH1F("h_pid_edep", "EDeposit", 100, 0., 100.);

  TH1F *h_pid_conex = new TH1F("h_pid_conex", "Cone.x", 100, 0., 100.);
  TH1F *h_pid_coney = new TH1F("h_pid_coney", "Cone.y", 100, 0., 100.);
  TH1F *h_pid_conez = new TH1F("h_pid_conez", "Cone.z", 100, 0., 100.);

  TH1F *h_pid_width = new TH1F("h_pid_width", "Width", 100, 0., 100.);
  TH1F *h_pid_length = new TH1F("h_pid_length", "Length", 100, 0., 4000.);
  TH1F *h_pid_partid= new TH1F("h_pid_partid","ParticleId", 30, -0.5, 29.5);
  TH1F *h_pid_pidw= new TH1F("h_pid_pidw","PIDWeight", 100, 0., 1.);

  TH1F *h_pid_fpx = new TH1F("h_pid_fpx", "FrontPosition.X", 100, -1000., 1000.);
  TH1F *h_pid_fpy = new TH1F("h_pid_fpy", "FrontPosition.Y", 100, -1000., 1000.);
  TH1F *h_pid_fpz = new TH1F("h_pid_fpz", "FrontPosition.Z", 100, -800., 2200.);
  TH1F *h_pid_fpt = new TH1F("h_pid_fpt", "FrontPosition.T", 100, 2000., 7000.);

  TH1F *h_pid_bpx = new TH1F("h_pid_bpx", "BackPosition.X", 100, -1000., 1000.);
  TH1F *h_pid_bpy = new TH1F("h_pid_bpy", "BackPosition.Y", 100, -1000., 1000.);
  TH1F *h_pid_bpz = new TH1F("h_pid_bpz", "BackPosition.Z", 100, -800., 2200.);
  TH1F *h_pid_bpt = new TH1F("h_pid_bpt", "BackPosition.T", 100, 2000., 7000.);

  TH1F *h_pid_fpvx = new TH1F("h_pid_fpvx", "FrontPositionVar.X", 100, -2000., 2000.);
  TH1F *h_pid_fpvy = new TH1F("h_pid_fpvy", "FrontPositionVar.Y", 100, -2000., 2000.);
  TH1F *h_pid_fpvz = new TH1F("h_pid_fpvz", "FrontPositionVar.Z", 100, -20., 20.);
  TH1F *h_pid_fpvt = new TH1F("h_pid_fpvt", "FrontPositionVar.T", 100, -10000., 10000.);

  TH1F *h_pid_bpvx = new TH1F("h_pid_bpvx", "BackPositionVar.X", 100, -2000., 2000.);
  TH1F *h_pid_bpvy = new TH1F("h_pid_bpvy", "BackPositionVar.Y", 100, -2000., 2000.);
  TH1F *h_pid_bpvz = new TH1F("h_pid_bpvz", "BackPositionVar.Z", 100, -2000., 2000.);
  TH1F *h_pid_bpvt = new TH1F("h_pid_bpvt", "BackPositionVar.T", 100, -10000., 10000.);

  TH1F *h_pid_fdx = new TH1F("h_pid_fdx", "FrontDirection.x", 100, -1., 1.);
  TH1F *h_pid_fdy = new TH1F("h_pid_fdy", "FrontDirection.y", 100, -1., 1.);
  TH1F *h_pid_fdz = new TH1F("h_pid_fdz", "FrontDirection.z", 100, -1., 1.);

  TH1F *h_pid_bdx = new TH1F("h_pid_bdx", "BackDirection.x", 100, -1., 1.);
  TH1F *h_pid_bdy = new TH1F("h_pid_bdy", "BackDirection.y", 100, -1., 1.);
  TH1F *h_pid_bdz = new TH1F("h_pid_bdz", "BackDirection.z", 100, -1., 1.);

  TH1F *h_pid_fdvx = new TH1F("h_pid_fdvx", "FrontDirectionVar.x", 100, -2000., 2000.);
  TH1F *h_pid_fdvy = new TH1F("h_pid_fdvy", "FrontDirectionVar.y", 100, -2000., 2000.);
  TH1F *h_pid_fdvz = new TH1F("h_pid_fdvz", "FrontDirectionVar.z", 100, -2000., 2000.);

  TH1F *h_pid_bdvx = new TH1F("h_pid_bdvx", "BackDirectionVar.x", 100, -2000., 2000.);
  TH1F *h_pid_bdvy = new TH1F("h_pid_bdvy", "BackDirectionVar.y", 100, -2000., 2000.);
  TH1F *h_pid_bdvz = new TH1F("h_pid_bdvz", "BackDirectionVar.z", 100, -2000., 2000.);

  TH1F *h_pid_fm= new TH1F("h_pid_fm","FrontMomentum", 100, 0., 2000.);
  TH1F *h_pid_bm= new TH1F("h_pid_bm","BackMomentum", 100, 0., 2000.);
  TH1F *h_pid_fme= new TH1F("h_pid_fme","FrontMomentumError", 100, 0., 100.);
  TH1F *h_pid_bme= new TH1F("h_pid_bme","BackMomentumError", 100, 0., 100.);

  TH1F *h_pid_patvx = new TH1F("h_pid_patvx", "PositionAtTrueVertex.X", 100, -1000., 1000.);
  TH1F *h_pid_patvy = new TH1F("h_pid_patvy", "PositionAtTrueVertex.Y", 100, -1000., 1000.);
  TH1F *h_pid_patvz = new TH1F("h_pid_patvz", "PositionAtTrueVertex.Z", 100, -800., 2200.);
  TH1F *h_pid_patvt = new TH1F("h_pid_patvt", "PositionAtTrueVertex.T", 100, 2000., 7000.);

  TH1F *h_pid_pvatvx = new TH1F("h_pid_pvatvx", "PositionVarAtTrueVertex.X", 100, -2000., 2000.);
  TH1F *h_pid_pvatvy = new TH1F("h_pid_pvatvy", "PositionVarAtTrueVertex.Y", 100, -2000., 2000.);
  TH1F *h_pid_pvatvz = new TH1F("h_pid_pvatvz", "PositionVarAtTrueVertex.Z", 100, -2000., 2000.);
  TH1F *h_pid_pvatvt = new TH1F("h_pid_pvatvt", "PositionVarAtTrueVertex.T", 100, -10., 10.);

  TH1F *h_pid_datvx = new TH1F("h_pid_datvx", "DirectionAtTrueVertex.x", 100, -1., 1.);
  TH1F *h_pid_datvy = new TH1F("h_pid_datvy", "DirectionAtTrueVertex.y", 100, -1., 1.);
  TH1F *h_pid_datvz = new TH1F("h_pid_datvz", "DirectionAtTrueVertex.z", 100, -1., 1.);

  TH1F *h_pid_dvatvx = new TH1F("h_pid_dvatvx", "DirectionVarAtTrueVertex.x", 100, -2000., 2000.);
  TH1F *h_pid_dvatvy = new TH1F("h_pid_dvatvy", "DirectionVarAtTrueVertex.y", 100, -2000., 2000.);
  TH1F *h_pid_dvatvz = new TH1F("h_pid_dvatvz", "DirectionVarAtTrueVertex.z", 100, -2000., 2000.);

  TH1F *h_pid_matv= new TH1F("h_pid_matv","MomentumAtTrueVertex", 100, 0., 2000.);
  TH1F *h_pid_meatv= new TH1F("h_pid_meatv","MomentumErrorAtTrueVertx", 100, 0.,100.);

  TH1F *h_pid_enok = new TH1F("h_pid_enok", "EntranceOK", 100, -0.5, 1.5);
  TH1F *h_pid_exok = new TH1F("h_pid_exok", "ExitOK", 100, -0.5, 1.5);

  TH1F *h_pid_enpx = new TH1F("h_pid_enpx", "EntrancePosition.X", 100, -1000., 1000.);
  TH1F *h_pid_enpy = new TH1F("h_pid_enpy", "EntrancePosition.Y", 100, -1000., 1000.);
  TH1F *h_pid_enpz = new TH1F("h_pid_enpz", "EntrancePosition.Z", 100, -800., 2200.);
  TH1F *h_pid_enpt = new TH1F("h_pid_enpt", "EntrancePosition.T", 100, -10., 10.);

  TH1F *h_pid_expx = new TH1F("h_pid_expx", "ExitPosition.X", 100, -1000., 1000.);
  TH1F *h_pid_expy = new TH1F("h_pid_expy", "ExitPosition.Y", 100, -1000., 1000.);
  TH1F *h_pid_expz = new TH1F("h_pid_expz", "ExitPosition.Z", 100, -800., 2200.);
  TH1F *h_pid_expt = new TH1F("h_pid_expt", "ExitPosition.T", 100, -10000., 10000.);

  TH1F *h_pid_endx = new TH1F("h_pid_endx", "EntranceDirection.x", 100, -1., 1.);
  TH1F *h_pid_endy = new TH1F("h_pid_endy", "EntranceDirection.y", 100, -1., 1.);
  TH1F *h_pid_endz = new TH1F("h_pid_endz", "EntranceDirection.z", 100, -1., 1.);

  TH1F *h_pid_exdx = new TH1F("h_pid_exdx", "ExitDirection.x", 100, -1., 1.);
  TH1F *h_pid_exdy = new TH1F("h_pid_exdy", "ExitDirection.y", 100, -1., 1.);
  TH1F *h_pid_exdz = new TH1F("h_pid_exdz", "ExitDirection.z", 100, -1., 1.);

  TH1F *h_pid_enmom= new TH1F("h_pid_enmom","EntranceMomentum", 100, 0., 2000.);
  TH1F *h_pid_exmom= new TH1F("h_pid_exmom","ExitMomentum", 100, 0., 2000.);

  TH1F *h_pid_enpex = new TH1F("h_pid_enpex", "EntrancePositionErr.X", 100, -1000., 1000.);
  TH1F *h_pid_enpey = new TH1F("h_pid_enpey", "EntrancePositionErr.Y", 100, -1000., 1000.);
  TH1F *h_pid_enpez = new TH1F("h_pid_enpez", "EntrancePositionErr.Z", 100, -800., 2200.);
  TH1F *h_pid_enpet = new TH1F("h_pid_enpet", "EntrancePositionErr.T", 100, -10000., 10000.);

  TH1F *h_pid_expex = new TH1F("h_pid_expex", "ExitPositionError.X", 100, -1000., 1000.);
  TH1F *h_pid_expey = new TH1F("h_pid_expey", "ExitPositionError.Y", 100, -1000., 1000.);
  TH1F *h_pid_expez = new TH1F("h_pid_expez", "ExitPositionErr.Z", 100, -800., 2200.);
  TH1F *h_pid_expet = new TH1F("h_pid_expet", "ExitPositionErr.T", 100, -10000., 10000.);

  TH1F *h_pid_endex = new TH1F("h_pid_endex", "EntranceDirectionErr.x", 100, -1., 1.);
  TH1F *h_pid_endey = new TH1F("h_pid_endey", "EntranceDirectionErr.y", 100, -1., 1.);
  TH1F *h_pid_endez = new TH1F("h_pid_endez", "EntranceDirectionErr.z", 100, -1., 1.);

  TH1F *h_pid_exdex = new TH1F("h_pid_exdex", "ExitDirectionErr.x", 100, -1., 1.);
  TH1F *h_pid_exdey = new TH1F("h_pid_exdey", "ExitDirectionErr.y", 100, -1., 1.);
  TH1F *h_pid_exdez = new TH1F("h_pid_exdez", "ExitDirectionErr.z", 100, -1., 1.);

  TH1F *h_pid_enmome= new TH1F("h_pid_enmome","EntranceMomentumErr", 100, 0., 2000.);
  TH1F *h_pid_exmome= new TH1F("h_pid_exmome","ExitMomentumErr", 100, 0., 2000.);

  TH1F *h_pid_nhitss = new TH1F("h_pid_nhitss", "NHitsSaved", 10, -0.5, 9.5);

  TH1F *h_pid_hs_ch = new TH1F("h_pid_hs_ch", "HitsSaved.Charge", 100, 0., 1000.);
  TH1F *h_pid_hs_t = new TH1F("h_pid_hs_t", "HitsSaved.Time", 100, 2000., 7000.);
  TH1F *h_pid_hs_px = new TH1F("h_pid_hs_px", "HitsSaved.Position.x", 100, -1000., 1000.);
  TH1F *h_pid_hs_py = new TH1F("h_pid_hs_py", "HitsSaved.Position.y", 100, -1000., 1000.);
  TH1F *h_pid_hs_pz = new TH1F("h_pid_hs_pz", "HitsSaved.Position.z", 100, -800., 2200.);
  TH1F *h_pid_hs_pex = new TH1F("h_pid_hs_pex", "HitsSaved.PositionError.x", 100, -500., 500.);
  TH1F *h_pid_hs_pey = new TH1F("h_pid_hs_pey", "HitsSaved.PositionError.y", 100, -500., 500.);
  TH1F *h_pid_hs_pez = new TH1F("h_pid_hs_pez", "HitsSaved.PositionError.z", 100, -500., 500.);
  TH1F *h_pid_hs_type = new TH1F("h_pid_hs_type", "HitsSaved.Type", 100, 0., 200.);

  TH1F *h_pid_tp_id = new TH1F("h_pid_tp_id", "TrueParticle.ID", 51, -.5, 50.5);
  TH1F *h_pid_tp_pur = new TH1F("h_pid_tp_pur", "TrueParticle.Pur", 100, .0, 1.);
  TH1F *h_pid_tp_eff = new TH1F("h_pid_tp_eff", "TrueParticle.Eff", 100, .0, 1.);

  TH1F *h_pid_tp_vx_pur = new TH1F("h_pid_tp_vx_pur", "TrueParticle.Vertex.Pur", 100, .0, 1.);
  TH1F *h_pid_tp_vx_eff = new TH1F("h_pid_tp_vx_eff", "TrueParticle.Vertex.Eff", 100, .0, 1.);


  TH1F *h_pid_nalt = new TH1F("h_pid_nalt", "NAlternates", 10, -0.5, 9.5);

  TH1F *h_pidalt_det = new TH1F("h_pidalt_det", "Detectors", 5001, -0.5, 5000.5);
  TH1F *h_pidalt_detu = new TH1F("h_pidalt_detu", "DetectorUsed", 100, -0.5, 1.5);
  TH1F *h_pidalt_status = new TH1F("h_pidalt_status", "Status", 100, -0.5, 1.5);
  TH1F *h_pidalt_ndof = new TH1F("h_pidalt_ndof", "NDOF", 100, 0., 600.);
  TH1F *h_pidalt_chi2= new TH1F("h_pidalt_chi2","Chi2", 100, 0., 200.);
  TH1F *h_pidalt_charge = new TH1F("h_pidalt_charge", "Charge", 100, -1.5, 1.5);
  TH1F *h_pidalt_length = new TH1F("h_pidalt_length", "Length", 100, 0., 8000.);
  TH1F *h_pidalt_partid= new TH1F("h_pidalt_partid","ParticleId", 30, -0.5, 29.5);
  TH1F *h_pidalt_pidw= new TH1F("h_pidalt_pidw","PIDWeight", 100, 0., 1.5);
  TH1F *h_pidalt_fpx = new TH1F("h_pidalt_fpx", "FrontPosition.X", 100, -3000., 3000.);
  TH1F *h_pidalt_fpy = new TH1F("h_pidalt_fpy", "FrontPosition.Y", 100, -3000., 3000.);
  TH1F *h_pidalt_fpz = new TH1F("h_pidalt_fpz", "FrontPosition.Z", 100, -3000., 3000.);
  TH1F *h_pidalt_fpt = new TH1F("h_pidalt_fpt", "FrontPosition.T", 100, 0., 8000.);

  TH1F *h_pidalt_bpx = new TH1F("h_pidalt_bpx", "BackPosition.X", 100, -3000., 3000.);
  TH1F *h_pidalt_bpy = new TH1F("h_pidalt_bpy", "BackPosition.Y", 100, -3000., 3000.);
  TH1F *h_pidalt_bpz = new TH1F("h_pidalt_bpz", "BackPosition.Z", 100, -3000., 3000.);
  TH1F *h_pidalt_bpt = new TH1F("h_pidalt_bpt", "BackPosition.T", 100, 0., 8000.);

  TH1F *h_pidalt_fpvx = new TH1F("h_pidalt_fpvx", "FrontPositionVar.X", 100, -5000., 5000.);
  TH1F *h_pidalt_fpvy = new TH1F("h_pidalt_fpvy", "FrontPositionVar.Y", 100, -5000., 5000.);
  TH1F *h_pidalt_fpvz = new TH1F("h_pidalt_fpvz", "FrontPositionVar.Z", 100, -5000., 5000.);
  TH1F *h_pidalt_fpvt = new TH1F("h_pidalt_fpvt", "FrontPositionVar.T", 100, -100000., 100000.);

  TH1F *h_pidalt_bpvx = new TH1F("h_pidalt_bpvx", "BackPositionVar.X", 100, -5000., 5000.);
  TH1F *h_pidalt_bpvy = new TH1F("h_pidalt_bpvy", "BackPositionVar.Y", 100, -5000., 5000.);
  TH1F *h_pidalt_bpvz = new TH1F("h_pidalt_bpvz", "BackPositionVar.Z", 100, -5000., 5000.);
  TH1F *h_pidalt_bpvt = new TH1F("h_pidalt_bpvt", "BackPositionVar.T", 100, -100000., 100000.);

  TH1F *h_pidalt_fdx = new TH1F("h_pidalt_fdx", "FrontDirection.x", 100, -1., 1.);
  TH1F *h_pidalt_fdy = new TH1F("h_pidalt_fdy", "FrontDirection.y", 100, -1., 1.);
  TH1F *h_pidalt_fdz = new TH1F("h_pidalt_fdz", "FrontDirection.z", 100, -1., 1.);

  TH1F *h_pidalt_bdx = new TH1F("h_pidalt_bdx", "BackDirection.x", 100, -1., 1.);
  TH1F *h_pidalt_bdy = new TH1F("h_pidalt_bdy", "BackDirection.y", 100, -1., 1.);
  TH1F *h_pidalt_bdz = new TH1F("h_pidalt_bdz", "BackDirection.z", 100, -1., 1.);

  TH1F *h_pidalt_fdvx = new TH1F("h_pidalt_fdvx", "FrontDirectionVar.x", 100, -2000., 2000.);
  TH1F *h_pidalt_fdvy = new TH1F("h_pidalt_fdvy", "FrontDirectionVar.y", 100, -2000., 2000.);
  TH1F *h_pidalt_fdvz = new TH1F("h_pidalt_fdvz", "FrontDirectionVar.z", 100, -2000., 2000.);

  TH1F *h_pidalt_bdvx = new TH1F("h_pidalt_bdvx", "BackDirectionVar.x", 100, -2000., 2000.);
  TH1F *h_pidalt_bdvy = new TH1F("h_pidalt_bdvy", "BackDirectionVar.y", 100, -2000., 2000.);
  TH1F *h_pidalt_bdvz = new TH1F("h_pidalt_bdvz", "BackDirectionVar.z", 100, -2000., 2000.);

  TH1F *h_pidalt_fm= new TH1F("h_pidalt_fm","FrontMomentum", 100, 0., 4000.);
  TH1F *h_pidalt_bm= new TH1F("h_pidalt_bm","BackMomentum", 100, 0., 4000.);
  TH1F *h_pidalt_fme= new TH1F("h_pidalt_fme","FrontMomentumError", 100, 0., 400.);
  TH1F *h_pidalt_bme= new TH1F("h_pidalt_bme","BackMomentumError", 100, 0., 400.);

  TH1F *h_pidalt_patvx = new TH1F("h_pidalt_patvx", "PositionAtTrueVertex.X", 100, -3000., 3000.);
  TH1F *h_pidalt_patvy = new TH1F("h_pidalt_patvy", "PositionAtTrueVertex.Y", 100, -3000., 3000.);
  TH1F *h_pidalt_patvz = new TH1F("h_pidalt_patvz", "PositionAtTrueVertex.Z", 100, -3000., 3000.);
  TH1F *h_pidalt_patvt = new TH1F("h_pidalt_patvt", "PositionAtTrueVertex.T", 100, 0., 8000.);

  TH1F *h_pidalt_pvatvx = new TH1F("h_pidalt_pvatvx", "PositionVarAtTrueVertex.X", 100, -5000., 5000.);
  TH1F *h_pidalt_pvatvy = new TH1F("h_pidalt_pvatvy", "PositionVarAtTrueVertex.Y", 100, -5000., 5000.);
  TH1F *h_pidalt_pvatvz = new TH1F("h_pidalt_pvatvz", "PositionVarAtTrueVertex.Z", 100, -5000., 5000.);
  TH1F *h_pidalt_pvatvt = new TH1F("h_pidalt_pvatvt", "PositionVarAtTrueVertex.T", 100, -10000., 10000.);

  TH1F *h_pidalt_datvx = new TH1F("h_pidalt_datvx", "DirectionAtTrueVertex.x", 100, -1., 1.);
  TH1F *h_pidalt_datvy = new TH1F("h_pidalt_datvy", "DirectionAtTrueVertex.y", 100, -1., 1.);
  TH1F *h_pidalt_datvz = new TH1F("h_pidalt_datvz", "DirectionAtTrueVertex.z", 100, -1., 1.);

  TH1F *h_pidalt_dvatvx = new TH1F("h_pidalt_dvatvx", "DirectionVarAtTrueVertex.x", 100, -2000., 2000.);
  TH1F *h_pidalt_dvatvy = new TH1F("h_pidalt_dvatvy", "DirectionVarAtTrueVertex.y", 100, -2000., 2000.);
  TH1F *h_pidalt_dvatvz = new TH1F("h_pidalt_dvatvz", "DirectionVarAtTrueVertex.z", 100, -2000., 2000.);

  TH1F *h_pidalt_matv= new TH1F("h_pidalt_matv","MomentumAtTrueVertex", 100, 0., 4000.);
  TH1F *h_pidalt_meatv= new TH1F("h_pidalt_meatv","MomentumErrorAtTrueVertx", 100, 0.,400.);


  //histograms to match Recon validation tree histos
  TH1F *h_event= new TH1F("h_event","Evt", 100, -0.5, 1000.5);
  TH1F *h_reconame= new TH1F("h_reconame","RecoName", 10, 0., 10.);
  TH1F *h_nconst= new TH1F("h_nconst","NConstituents", 10, -0.5, 9.5);
  TH1F *h_detectors= new TH1F("h_detectors","Detectors", 2501, -0.5, 2500.5);
  TH1F *h_purity= new TH1F("h_purity","Purity", 101, -0.005, 1.005);
  TH1F *h_eff= new TH1F("h_eff","Efficiency", 101, -0.005, 1.005);
  TH1F *h_fitchi= new TH1F("h_fitchi","FitChiInfo", 100, 0., 50.);
  TH1F *h_fitndof= new TH1F("h_fitndof","Fitndof", 701, 0.5, 700.5);
  TH1F *h_fitstatus= new TH1F("h_fitstatus","FitStatus", 100, 0., 10.);
  TH1F *h_nhits= new TH1F("h_nhits","NHits", 150, 0., 150.);
  TH1F *h_type= new TH1F("h_type","Type", 11, -0.5, 10.5);
  TH1F *h_partid= new TH1F("h_partid","PartId", 15, -0.5, 14.5);

  TH1F *h_chomom = new TH1F("h_chomom", "TrkChrgOverMom", 100, -1.0, 1.0);
  TH1F *h_chopt = new TH1F("h_chopt", "TrkChrgOverTransMom", 100, -1.0, 1.0);
  TH1F *h_sense = new TH1F("h_sense", "TrkSense", 5, -2.5, 2.5);
  TH1F *h_pos1 = new TH1F("h_pos1", "TrkPosition", 100, -1200.0, 1200.0);
  TH1F *h_pos2 = new TH1F("h_pos2", "TrkPosition", 100, -1200.0, 1200.0);
  TH1F *h_pos3 = new TH1F("h_pos3", "TrkPosition", 100, 0.0, 2000.0);
  TH1F *h_posvar1 = new TH1F("h_posvar1", "TrkPositionCov", 100, 0.0, 100.0);
  TH1F *h_posvar2 = new TH1F("h_posvar2", "TrkPositionCov", 100, 0.0, 100.0);
  TH1F *h_posvar3 = new TH1F("h_posvar3", "TrkPositionCov", 100, 0.0, 5.0);
  TH1F *h_dir1 = new TH1F("h_dir1", "TrkSlopes", 100, -1.0, 1.0);
  TH1F *h_dir2 = new TH1F("h_dir2", "TrkSlopes", 100, -1.0, 1.0);
  TH1F *h_dirvar1 = new TH1F("h_dirvar1", "TrkSlopesCov", 100, -1.0, 1.0);
  TH1F *h_dirvar2 = new TH1F("h_dirvar2", "TrkSlopesCov", 100, -1.0, 1.0);

  TH1F *h_tpartid = new TH1F("h_tpartid", "TrueParticleID", 100, .0, 100.0);
  TH1F *h_tpartpdg = new TH1F("h_tpartpdg", "TrueParticlePDGenc", 100, .0, 100.0);
  TH1F *h_tdetectors= new TH1F("h_tdetectors","TrueDetectors", 2501, -0.5, 2500.5);
  TH1F *h_tchomom = new TH1F("h_tchomom", "TrueChrgOverMom", 100, -1.0, 1.0);
  TH1F *h_tchopt = new TH1F("h_tchopt", "TrueChrgOverTransMom", 100, -1.0, 1.0);
  TH1F *h_tsense = new TH1F("h_tsense", "TrueSense", 5, -2.5, 2.5);
  TH1F *h_tpos1 = new TH1F("h_tpos1", "TruePosition", 100, -1200.0, 1200.0);
  TH1F *h_tpos2 = new TH1F("h_tpos2", "TruePosition", 100, -1200.0, 1200.0);
  TH1F *h_tpos3 = new TH1F("h_tpos3", "TruePosition", 100, 0.0, 2000.0);


  //FGD CLUSTER
  /* Removed for production 6
  //TH1F *h_nfgdclusters = new TH1F("h_nfgdclusters","h_fgdclusters",10,0,10);
  TH1F *h_fgdcluster_TotalCharge  = new TH1F("h_fgdcluster_TotalCharge","h_fgdcluster_TotalCharge",100,0,700);
  TH1F *h_fgdcluster_TimeDisp     = new TH1F("h_fgdcluster_TimeDisp","h_fgdcluster_TimeDisp",100,0,100);
  TH1F *h_fgdcluster_TimeDisp_small = new TH1F("h_fgdcluster_TimeDisp_small","h_fgdcluster_TimeDisp_small",100,0,20);
  TH1F *h_fgdcluster_TimeDisp_large = new TH1F("h_fgdcluster_TimeDisp_large","h_fgdcluster_TimeDisp_large",1000,1675,1690);
  TH1F *h_fgdcluster_SpatialDisp  = new TH1F("h_fgdcluster_SpatialDisp","h_fgdcluster_SpatialDisp",100,0,140);
  TH1F *h_fgdcluster_NHits        = new TH1F("h_fgdcluster_NHits","h_fgdcluster_NHits",100,2,10);
  TH1F *h_fgdcluster_PosRMSx       = new TH1F("h_fgdcluster_PosRMSx","h_fgdcluster_PosRMSx",100,-5,35);
  TH1F *h_fgdcluster_PosRMSy       = new TH1F("h_fgdcluster_PosRMSy","h_fgdcluster_PosRMSy",100,-10,50);
  TH1F *h_fgdcluster_PosRMSz       = new TH1F("h_fgdcluster_PosRMSz","h_fgdcluster_PosRMSz",100,-10,50);
  TH1F *h_fgdcluster_PosRMSt       = new TH1F("h_fgdcluster_PosRMSt","h_fgdcluster_PosRMSt",100,-1,10);
  TH1F *h_fgdcluster_Positionx     = new TH1F("h_fgdcluster_Positionx","h_fgdcluster_Positionx",100,-1000,800);
  TH1F *h_fgdcluster_Positiony     = new TH1F("h_fgdcluster_Positiony","h_fgdcluster_Positiony",100,-1000,1000);
  TH1F *h_fgdcluster_Positionz     = new TH1F("h_fgdcluster_Positionz","h_fgdcluster_Positionz",100,0,2000);
  TH1F *h_fgdcluster_Positiont     = new TH1F("h_fgdcluster_Positiont","h_fgdcluster_Positiont",100,2000,12000);
  TH1F *h_fgdcluster_Variancex     = new TH1F("h_fgdcluster_Variancex","h_fgdcluster_Variancex",100,0,100);
  TH1F *h_fgdcluster_Variancey     = new TH1F("h_fgdcluster_Variancey","h_fgdcluster_Variancey",100,0,100);
  TH1F *h_fgdcluster_Variancez     = new TH1F("h_fgdcluster_Variancez","h_fgdcluster_Variancez",100,-10,10);
  TH1F *h_fgdcluster_Variancet    = new TH1F("h_fgdcluster_Variancet","h_fgdcluster_Variancet",100,-10,10);
  */

  //FGD OBJECT

  TH1F *h_nfgds = new TH1F("h_nfgds","h_fgds",10,0,10);
  TH1F *h_fgd_avgtime                         = new TH1F("h_fgd_avgtime","h_fgd_avgtime",100,0,100);
  TH1F *h_fgd_chargePerLayer                  = new TH1F("h_fgd_chargePerLayer","h_fgd_chargePerLayer",100,0,200);
  TH1F *h_fgd_chargePerLayerAttenCorr         = new TH1F("h_fgd_chargePerLayerAttenCorr","h_fgd_chargePerLayerAttenCorr",100,0,200);
  TH1F *h_fgd_fgdContainment                  = new TH1F("h_fgd_fgdContainment","h_fgd_fgdContainment",100,-3,3);
  TH1F *h_fgd_E                               = new TH1F("h_fgd_E","h_fgd_E",100,-10,140);
  TH1F *h_fgd_x                               = new TH1F("h_fgd_x","h_fgd_x",100,0,1300);
  TH1F *h_fgd_E_exp_muon                      = new TH1F("h_fgd_E_exp_muon","h_fgd_E_exp_muon",100,-10,250);
  TH1F *h_fgd_E_exp_pion                      = new TH1F("h_fgd_E_exp_pion","h_fgd_E_exp_pion",100,-10,250);
  TH1F *h_fgd_E_exp_proton                    = new TH1F("h_fgd_E_exp_proton","h_fgd_E_exp_proton",100,-10,400);
  TH1F *h_fgd_sigmaE_muon                     = new TH1F("h_fgd_sigmaE_muon","h_fgd_sigmaE_muon",100,-1,15);
  TH1F *h_fgd_sigmaE_pion                     = new TH1F("h_fgd_sigmaE_pion","h_fgd_sigmaE_pion",100,-2,10);
  TH1F *h_fgd_sigmaE_proton                   = new TH1F("h_fgd_sigmaE_proton","h_fgd_sigmaE_proton",100,-2,23);
  TH1F *h_fgd_PullMuon                        = new TH1F("h_fgd_PullMuon","h_fgd_PullMuon",100,-11000000000.,-9000000000.);
  TH1F *h_fgd_PullMuon_small                  = new TH1F("h_fgd_PullMuon_small","h_fgd_PullMuon_small",100,-15,15);
  TH1F *h_fgd_PullPion                        = new TH1F("h_fgd_PullPion","h_fgd_PullPion",100,-11000000000.,-9000000000.);
  TH1F *h_fgd_PullPion_small                  = new TH1F("h_fgd_PullPion_small","h_fgd_PullPion_small",100,-15,15);
  TH1F *h_fgd_PullProton                      = new TH1F("h_fgd_PullProton","h_fgd_PullProton",100,-11000000000.,-9000000000.);
  TH1F *h_fgd_PullProton_small                = new TH1F("h_fgd_PullProton_small","h_fgd_PullProton_small",100,-15,15);
  TH1F *h_fgd_PullNotSet                    = new TH1F("h_fgd_PullNotSet","h_fgd_PullNotSet",100,-11000000000.,-9000000000.);
  TH1F *h_fgd_PullNotSet_small                = new TH1F("h_fgd_PullNotSet_small","h_fgd_PullNotSet_small",100,0,2);
  TH1F *h_fgd_isFgdVA                         = new TH1F("h_fgd_isFgdVA","h_fgd_isFgdVA",100,-1,2);
  TH1F *h_fgd_isFgdVA_flag                    = new TH1F("h_fgd_isFgdVA_flag","h_fgd_isFgdVA_flag",100,-1,1);
  TH1F *h_fgd_fgdVA_upMinZ                    = new TH1F("h_fgd_fgdVA_upMinZ","h_fgd_fgdVA_upMinZ",100,-1200000,100000);
  TH1F *h_fgd_fgdVA_downMaxZ                  = new TH1F("h_fgd_fgdVA_downMaxZ","h_fgd_fgdVA_downMaxZ",100,-1200000,100000);
  TH1F *h_fgd_fgdVA_upCosTheta                = new TH1F("h_fgd_fgdVA_upCosTheta","h_fgd_fgdVA_upCosTheta",100,-2,2);
  TH1F *h_fgd_fgdVA_downCosTheta              = new TH1F("h_fgd_fgdVA_downCosTheta","h_fgd_fgdVA_downCosTheta",100,-1,2);
  TH1F *h_fgd_fgdVA_otherUpQ                  = new TH1F("h_fgd_fgdVA_otherUpQ","h_fgd_fgdVA_otherUpQ",100,-10,1000);
  TH1F *h_fgd_fgdVA_otherDownQ                = new TH1F("h_fgd_fgdVA_otherDownQ","h_fgd_fgdVA_otherDownQ",100,-10,500);
  TH1F *h_fgd_fgdVA_verQ                      = new TH1F("h_fgd_fgdVA_verQ","h_fgd_fgdVA_verQ",100,-10,300);
  TH1F *h_fgd_fgdVA_verLayQ                   = new TH1F("h_fgd_fgdVA_verLayQ","h_fgd_fgdVA_verLayQ",100,-10,1000);
  TH1F *h_fgd_fgdVA_verNearQ                  = new TH1F("h_fgd_fgdVA_verNearQ","h_fgd_fgdVA_verNearQ",100,-10,500);
  TH1F *h_fgd_fgdVA_verNextNearQ              = new TH1F("h_fgd_fgdVA_verNextNearQ","h_fgd_fgdVA_verNextNearQ",100,-10,1300);
  TH1F *h_fgd_fgdVA_verNextNextNearQ          = new TH1F("h_fgd_fgdVA_verNextNextNearQ","h_fgd_fgdVA_verNextNextNearQ",100,-10,1300);
  TH1F *h_fgd_fgdVA_totalQ                    = new TH1F("h_fgd_fgdVA_totalQ","h_fgd_fgdVA_totalQ",100,-10,5500);
  TH1F *h_fgd_fgdVA_totalCorrE                = new TH1F("h_fgd_fgdVA_totalCorrE","h_fgd_fgdVA_totalCorrE",100,-10,250);
  // New Vertex Activity (2x3,2x5,2x7)
  TH1F *h_fgd_fgdVA_verNearQ_rect             = new TH1F("h_fgd_fgdVA_verNearQ_rect","h_fgd_fgdVA_verNearQ_rect",100,-10,500);
  TH1F *h_fgd_fgdVA_verNextNearQ_rect         = new TH1F("h_fgd_fgdVA_verNextNearQ_rect","h_fgd_fgdVA_verNextNearQ_rect",100,-10,1300);
  TH1F *h_fgd_fgdVA_verNextNextNearQ_rect     = new TH1F("h_fgd_fgdVA_verNextNextNearQ_rect","h_fgd_fgdVA_verNextNextNearQ_rect",100,-10,1300);
  // New End activities (2x3,2x5,2x7)
  TH1F *h_fgd_fgdEA_verNearQ _rect                 = new TH1F("h_fgd_fgdVA_verNearQ_rect","h_fgd_fgdEA_verNearQ_rect",100,-10,500);
  TH1F *h_fgd_fgdEA_verNextNearQ_rect              = new TH1F("h_fgd_fgdVA_verNextNearQ_rect","h_fgd_fgdEA_verNextNearQ_rect",100,-10,1300);
  TH1F *h_fgd_fgdEA_verNextNextNearQ_rect          = new TH1F("h_fgd_fgdVA_verNextNextNearQ_rect","h_fgd_fgdEA_verNextNextNearQ_rect",100,-10,1300);
  //TPC object
  TH1F *h_tpc_t0source  = new TH1F("h_tpc_t0source", "T0Source", 200, -100, 100);
  TH1F *h_tpc_nhorrows  = new TH1F("h_tpc_nhorrows", "NHorRows", 200, -100, 100);
  TH1F *h_tpc_nverrows  = new TH1F("h_tpc_nverrows", "NVerRows", 200, -100, 100);
  TH1F *h_tpc_charge  = new TH1F("h_tpc_charge", "Charge", 30, -1.5, 1.5);
  TH1F *h_tpc_ntrun  = new TH1F("h_tpc_ntrun", "NTrun", 200, -100, 100);
  TH1F *h_tpc_ccorr  = new TH1F("h_tpc_ccorr", "Ccorr", 200, -100, 100);
  TH1F *h_tpc_pullele  = new TH1F("h_tpc_pullele", "PullEle", 200, -100, 100);
  TH1F *h_tpc_pullmuon  = new TH1F("h_tpc_pullmuon", "PullMuon", 200, -100, 100);
  TH1F *h_tpc_pullpion  = new TH1F("h_tpc_pullpion", "PullPion", 200, -100, 100);
  TH1F *h_tpc_pullkaon  = new TH1F("h_tpc_pullkaon", "PullKaon", 200, -100, 100);
  TH1F *h_tpc_pullproton  = new TH1F("h_tpc_pullproton", "PullProton", 200, -100, 100);
  TH1F *h_tpc_dedxexpele  = new TH1F("h_tpc_dedxexpele", "dEdxexpEle", 200, -100, 100);
  TH1F *h_tpc_dedxexpmuon  = new TH1F("h_tpc_dedxexpmuon", "dEdxexpMuon", 200, -100, 100);
  TH1F *h_tpc_dedxexppion  = new TH1F("h_tpc_dedxexppion", "dEdxexpPion", 200, -100, 100);
  TH1F *h_tpc_dedxexpkaon  = new TH1F("h_tpc_dedxexpkaon", "dEdxexpKaon", 200, -100, 100);
  TH1F *h_tpc_dedxexpproton  = new TH1F("h_tpc_dedxexpproton", "dEdxexpProton", 200, -100, 100);
  TH1F *h_tpc_sigmaele  = new TH1F("h_tpc_sigmaele", "SigmaEle", 200, -100, 100);
  TH1F *h_tpc_sigmamuon  = new TH1F("h_tpc_sigmamuon", "SigmaMuon", 200, -100, 100);
  TH1F *h_tpc_sigmapion  = new TH1F("h_tpc_sigmapion", "SigmaPion", 200, -100, 100);
  TH1F *h_tpc_sigmakaon  = new TH1F("h_tpc_sigmakaon", "SigmaKaon", 200, -100, 100);
  TH1F *h_tpc_sigmaproton  = new TH1F("h_tpc_sigmaproton", "SigmaProton", 200, -100, 100);
  TH1F *h_tpc_nnodes  = new TH1F("h_tpc_nnodes", "NNodes", 200, -100, 100);
  TH1F *h_tpc_kalmanfittype  = new TH1F("h_tpc_kalmanfittype", "KalmanFitType", 200, -100, 100);
  TH1F *h_tpc_tpclength  = new TH1F("h_tpc_tpclength", "TPCLength", 200, -100, 100);
  TH1F *h_tpc_pefield  = new TH1F("h_tpc_pefield", "PEField", 200, -100, 100);
  TH1F *h_tpc_pdist  = new TH1F("h_tpc_pdist", "PDist", 200, -100, 100);

  TH1F *h_tpc_nhits = new TH1F("h_tpc_nhits", "No. of Hits", 100, 0, 300);
  TH1F *h_tpc_widthx = new TH1F("h_tpc_widthx", "Width X",100,-1,2);
  TH1F *h_tpc_widthy = new TH1F("h_tpc_widthy", "Width y",100,-1,2);
  TH1F *h_tpc_widthz = new TH1F("h_tpc_widthz", "Width z",100,-1,2);
  TH1F *h_tpc_fpost = new TH1F("h_tpc_fpost", "Front Position t",100,2000,15000);
  TH1F *h_tpc_fposx = new TH1F("h_tpc_fposx", "Front Position x",100,-5000,10000);
  TH1F *h_tpc_fposy = new TH1F("h_tpc_fposy", "Front Position y",100,-5000,40000);
  TH1F *h_tpc_fposz = new TH1F("h_tpc_fposz", "Front Position z",100,-40000,20000);
  TH1F *h_tpc_fpos_vart = new TH1F("h_tpc_fpos_vart", "Front Position Var t",100,-5,55);
  TH1F *h_tpc_fpos_varx = new TH1F("h_tpc_fpos_varx", "Front Position Var x",100,-1000,10000);
  TH1F *h_tpc_fpos_vary = new TH1F("h_tpc_fpos_vary", "Front Position Var y",100,-7000,100000);
  TH1F *h_tpc_fpos_varz = new TH1F("h_tpc_fpos_varz", "Front Position Var z",100,-4000,200000);
  TH1F *h_tpc_bpost = new TH1F("h_tpc_bpost", "Back Position t",100,2000,15000);
  TH1F *h_tpc_bposx = new TH1F("h_tpc_bposx", "Back Position x",100,-8000,10000);
  TH1F *h_tpc_bposy = new TH1F("h_tpc_bposy", "Back Position y",100,-40000,20000);
  TH1F *h_tpc_bposz = new TH1F("h_tpc_bposz", "Back Position z",100,-5000,12000);
  TH1F *h_tpc_bpos_vart = new TH1F("h_tpc_bpos_vart", "Back Position Var t",100,-5,55);
  TH1F *h_tpc_bpos_varx = new TH1F("h_tpc_bpos_varx", "Back Position Var x",100,-4000,200000);
  TH1F *h_tpc_bpos_vary = new TH1F("h_tpc_bpos_vary", "Back Position Var y",100,-8000,200000);
  TH1F *h_tpc_bpos_varz = new TH1F("h_tpc_bpos_varz", "Back Position Var z",100,-2000,200000);
  TH1F *h_tpc_conex= new TH1F("h_tpc_conex", "Cone X", 100,-0.5,1);
  TH1F *h_tpc_coney= new TH1F("h_tpc_coney", "Cone y", 100,-0.5,1);
  TH1F *h_tpc_conez= new TH1F("h_tpc_conez", "Cone z", 100,-0.5,1);
  TH1F *h_tpc_fdirx = new TH1F("h_tpc_fdirx", "Front Direction x",100,-2,2);
  TH1F *h_tpc_fdiry = new TH1F("h_tpc_fdiry", "Front Direction y",100,-2,2);
  TH1F *h_tpc_fdirz = new TH1F("h_tpc_fdirz", "Front Direction z",100,-2,2);
  TH1F *h_tpc_fdir_varx = new TH1F("h_tpc_fdir_varx", "Front Direction Var x",100,-500,400);
  TH1F *h_tpc_fdir_vary = new TH1F("h_tpc_fdir_vary", "Front Direction Var y",100,-600,400);
  TH1F *h_tpc_fdir_varz = new TH1F("h_tpc_fdir_varz", "Front Direction Var z",100,-1000,400);
  TH1F *h_tpc_bdirx = new TH1F("h_tpc_bdirx", "Back Direction x",100,-2,2);
  TH1F *h_tpc_bdiry = new TH1F("h_tpc_bdiry", "Back Direction y",100,-2,2);
  TH1F *h_tpc_bdirz = new TH1F("h_tpc_bdirz", "Back Direction z",100,-2,2);
  TH1F *h_tpc_bdir_varx = new TH1F("h_tpc_bdir_varx", "Back Direction Var x",100,-400,400);
  TH1F *h_tpc_bdir_varx_full = new TH1F("h_tpc_bdir_varx_full", "Back Direction Var x (Full)",100,-400,400);
  TH1F *h_tpc_bdir_vary = new TH1F("h_tpc_bdir_vary", "Back Direction Var y",100,-400,800);
  TH1F *h_tpc_bdir_varz = new TH1F("h_tpc_bdir_varz", "Back Direction Var z",100,-500,400);
  TH1F *h_tpc_UniqueID                   = new TH1F("h_tpc_UniqueID", "h_tpc_UniqueID",100,0,3000000);
  TH1F *h_tpc_Status                     = new TH1F("h_tpc_Status", "h_tpc_Status", 100,0,4);
  TH1F *h_tpc_Detector                   = new TH1F("h_tpc_Detector", "h_tpc_Detector",100,0,12);
  TH1F *h_tpc_NNodes               = new TH1F("h_tpc_NNodes", "h_tpc_NNodes",100,0,40);
  TH1F *h_tpc_NDOF                       = new TH1F("h_tpc_NDOF", "h_tpc_NDOF",100,-10,40);
  TH1F *h_tpc_Chi2                       = new TH1F("h_tpc_Chi2", "h_tpc_Chi2",100,0,50000);
  TH1F *h_tpc_EDeposit                   = new TH1F("h_tpc_EDeposit","h_tpc_EDeposit",100,0,900);
  TH1F *h_tpc_NConstituents              = new TH1F("h_tpc_NConstituents", "h_tpc_NConstituents",100,0,4);
  TH1F *h_tpc_Length                     =new TH1F("h_tpc_Length", "h_tpc_Length", 100,-10000,110000);
  TH1F *h_tpc_FrontMomentum              = new TH1F("h_tpc_FrontMomentum", "h_tpc_FrontMomentum", 100,-100000,400000);
  TH1F *h_tpc_BackMomentum              = new TH1F("h_tpc_BackMomentum", "h_tpc_BackMomentum", 100,-200000,700000);
  TH1F *h_tpc_FrontMomentumError         = new TH1F("h_tpc_FrontMomentumError", "h_tpc_FrontMomentumError", 100,-5000000,30000000);
  TH1F *h_tpc_BackMomentumError         = new TH1F("h_tpc_BackMomentumError", "h_tpc_BackMomentumError", 100,-20000000,30000000);
  TH1F *h_tpc_TrueParticleID         = new TH1F("h_tpc_TrueParticleID ","h_tpc_TrueParticleID ",100,0,60000);
  TH1F *h_tpc_TrueParticlePur        = new TH1F("h_tpc_TrueParticlePur ","h_tpc_TrueParticlePur ",100,0,4);
  TH1F *h_tpc_TrueParticleEff        = new TH1F("h_tpc_TrueParticleEff ","h_tpc_TrueParticleEff ",100,0,4);
  TH1F *h_tpc_TrueParticleVertexPositionX     = new TH1F("h_tpc_TrueParticleVertexPositionX ","h_tpc_TrueParticleVertexPositionX ",100,-3200,3200);
  TH1F *h_tpc_TrueParticleVertexPositionY     = new TH1F("h_tpc_TrueParticleVertexPositionY ","h_tpc_TrueParticleVertexPositionY ",100,-4000,4000);
  TH1F *h_tpc_TrueParticleVertexPositionZ     = new TH1F("h_tpc_TrueParticleVertexPositionZ ","h_tpc_TrueParticleVertexPositionZ ",100,-5000,5000);
  TH1F *h_tpc_TrueParticleVertexPositionT     = new TH1F("h_tpc_TrueParticleVertexPositionT ","h_tpc_TrueParticleVertexPositionT ",100,0,5000);
  TH1F *h_tpc_TrueParticleVertexID         = new TH1F("h_tpc_TrueParticleVertexID ","h_tpc_TrueParticleVertexID ",100,0,41000);


  //ECAL OBJECT

  TH1F *h_necals = new TH1F("h_necals", "No. of ECal objects", 10,0,10);
  TH1F *h_ecals_nhits = new TH1F("h_ecals_nhits", "No. of ECal Hits", 100, 0, 300);
  TH1F *h_ecals_trshval = new TH1F("h_ecals_trshval", "TrkShowValue",100,-2,2);
  TH1F *h_ecals_emhadval = new TH1F("h_ecals_emhadval", "EMHadValue",100,-2,2);
  TH1F *h_ecals_widthx = new TH1F("h_ecals_widthx", "Width X",100,-1,2);
  TH1F *h_ecals_widthy = new TH1F("h_ecals_widthy", "Width y",100,-1,2);
  TH1F *h_ecals_widthz = new TH1F("h_ecals_widthz", "Width z",100,-1,2);
  TH1F *h_ecals_fpost = new TH1F("h_ecals_fpost", "Front Position t",100,2000,15000);
  TH1F *h_ecals_fposx = new TH1F("h_ecals_fposx", "Front Position x",100,-5000,10000);
  TH1F *h_ecals_fposy = new TH1F("h_ecals_fposy", "Front Position y",100,-5000,40000);
  TH1F *h_ecals_fposz = new TH1F("h_ecals_fposz", "Front Position z",100,-40000,20000);
  TH1F *h_ecals_fpos_vart = new TH1F("h_ecals_fpos_vart", "Front Position Var t",100,-5,55);
  TH1F *h_ecals_fpos_varx = new TH1F("h_ecals_fpos_varx", "Front Position Var x",100,-1000,10000);
  TH1F *h_ecals_fpos_vary = new TH1F("h_ecals_fpos_vary", "Front Position Var y",100,-7000,100000);
  TH1F *h_ecals_fpos_varz = new TH1F("h_ecals_fpos_varz", "Front Position Var z",100,-4000,200000);
  TH1F *h_ecals_bpost = new TH1F("h_ecals_bpost", "Back Position t",100,2000,15000);
  TH1F *h_ecals_bposx = new TH1F("h_ecals_bposx", "Back Position x",100,-8000,10000);
  TH1F *h_ecals_bposy = new TH1F("h_ecals_bposy", "Back Position y",100,-40000,20000);
  TH1F *h_ecals_bposz = new TH1F("h_ecals_bposz", "Back Position z",100,-5000,12000);
  TH1F *h_ecals_bpos_vart = new TH1F("h_ecals_bpos_vart", "Back Position Var t",100,-5,55);
  TH1F *h_ecals_bpos_varx = new TH1F("h_ecals_bpos_varx", "Back Position Var x",100,-4000,200000);
  TH1F *h_ecals_bpos_vary = new TH1F("h_ecals_bpos_vary", "Back Position Var y",100,-8000,200000);
  TH1F *h_ecals_bpos_varz = new TH1F("h_ecals_bpos_varz", "Back Position Var z",100,-2000,200000);
  TH1F *h_ecals_emenergy= new TH1F("h_ecals_emenergy", "EM Energy",100,-100,10000);
  TH1F *h_ecals_emenergyerror= new TH1F("h_ecals_emenergyerror", "EM Energy Error",100,-50,2000);
  TH1F *h_ecals_averagehittime= new TH1F("h_ecals_averagehittime", "Average Hit Time",100,-1000,40000);
  TH1F *h_ecals_averagehittime_full= new TH1F("h_ecals_averagehittime_full", "Average Hit Time (Full)",100,-1000,40000);
  TH1F *h_ecals_kfparameter= new TH1F("h_ecals_kfparameter", "KF Parameter",100,-1.5,0.5);
  TH1F *h_ecals_kfnnodes= new TH1F("h_ecals_kfnnodes", "KF Number of Nodes",100,-5,100);
  TH1F *h_ecals_kfquality= new TH1F("h_ecals_kfquality", "KF Quality",100,-10,50000);
  TH1F *h_ecals_kfparameternodes= new TH1F("h_ecals_kfparameternodes", "KF Parameter Nodes",100,-2,2);
  TH1F *h_ecals_kfndof= new TH1F("h_ecals_kfndof", "KFNDOF ",100,-5,100);
  TH1F *h_ecals_kfhitquality= new TH1F("h_ecals_kfhitquality", "KF Hit Quality",100,-5,60);
  TH1F *h_ecals_melectrontag_ndelayedcluster= new TH1F("h_ecals_melectrontag_ndelayedcluster", "M Electon Tag Delayed Cluster",100,-1,15);
  TH1F *h_ecals_melectrontag_ndelayedhits= new TH1F("h_ecals_melectrontag_ndelayedhits", "M Electron Tag Delayed Hits ",100,0,10);
  TH1F *h_ecals_melectrontag_deltax= new TH1F("h_ecals_melectrontag_deltax", "M Electron Tag Delta X",100,-120,500);
  TH1F *h_ecals_melectrontag_deltay= new TH1F("h_ecals_melectrontag_deltay", "M Electron Tag Delta Y",100,-120,500);
  TH1F *h_ecals_melectrontag_deltaz= new TH1F("h_ecals_melectrontag_deltaz", "M Electron Tag Delta Z",100,-120,500);
  TH1F *h_ecals_melectrontag_deltat= new TH1F("h_ecals_melectrontag_deltat", "M Electron Tag Delta T",100,-100,11000);
  TH1F *h_ecals_melectrontag_edep= new TH1F("h_ecals_melectrontag_edep", "M Electron Tag EDep",100,0,30);
  TH1F *h_ecals_melectrontag_trackend= new TH1F("h_ecals_melectrontag_trackend", "M Electron Tag Track End",100,-1.5,1.5);
  TH1F *h_ecals_emenergy_likelihood_energygrad= new TH1F("h_ecals_emenergy_likelihood_energygrad", "EM Energy Likelihood Energygrad",100,-10,100);
  TH1F *h_ecals_emenergy_likelihood_energy_qsumgrad= new TH1F("h_ecals_emenergy_likelihood_energy_qsumgrad", "EM Energy Likelihood Energy qsum grad",100,-5,100);
  TH1F *h_ecals_emenergy_likelihood_qsum_like= new TH1F("h_ecals_emenergy_likelihood_qsum_like", "EM Energy Likelihood qsum like",100,-2,20);
  TH1F *h_ecals_emenergy_para_qsum= new TH1F("h_ecals_emenergy_para_qsum", "EM Energy param QSum ",100,-100,2000);
  TH1F *h_ecals_emenergy_para_qmean= new TH1F("h_ecals_emenergy_para_qmean", "EM Energy param QMean ",100,-20,100);
  TH1F *h_ecals_emenergy_para_qrms= new TH1F("h_ecals_emenergy_para_qrms", "EM Energy param QRMS ",100,-1,20);
  TH1F *h_ecals_emenergy_para_qskew= new TH1F("h_ecals_emenergy_para_qskew", "EM Energy param QSkew ",100,-2,20);
  TH1F *h_ecals_emenergy_para_qmax= new TH1F("h_ecals_emenergy_para_qmax", "EM Energy param QMax ",100,-10,200);
  TH1F *h_ecals_isshowerlike= new TH1F("h_ecals_isshowerlike", "Is Shower Like",100,-0.2,1.2);
  TH1F *h_ecals_istracklike= new TH1F("h_ecals_istracklike", "Is Track Like",100,-0.2,1.2);
  TH1F *h_ecals_pid_amr= new TH1F("h_ecals_pid_amr", "PID AMR",100,-10,310);
  TH1F *h_ecals_pid_angle= new TH1F("h_ecals_pid_angle", "PID Angle",100,-10,100);
  TH1F *h_ecals_pid_max_ratio= new TH1F("h_ecals_pid_max_ratio", "PID Max Ratio",100,-10,400);
  TH1F *h_ecals_pid_showerangle= new TH1F("h_ecals_pid_showerangle", "PID Shower Angle",100,-0.5,1);
  TH1F *h_ecals_pid_asymmetry= new TH1F("h_ecals_pid_asymmetry", "PID Asymmetry", 100,-0.5,600);
  TH1F *h_ecals_pid_meanposition= new TH1F("h_ecals_pid_meanposition", "PID Mean Position", 100,-200,3000);
  TH1F *h_ecals_pid_showerwidth= new TH1F("h_ecals_pid_showerwidth", "PID Shower Width", 100,-0.5,1);
  TH1F *h_ecals_pid_mostupstreamlayerhit= new TH1F("h_ecals_pid_mostupstreamlayerhit", "PID Most Upstream Layer hit", 100,-2,35);
  TH1F *h_ecals_pid_mostdownstreamlayerhit= new TH1F("h_ecals_pid_mostdownstreamlayerhit", "PID Most Downstream Layer hit", 100,-2,35);
  TH1F *h_ecals_conex= new TH1F("h_ecals_conex", "Cone X", 100,-0.5,1);
  TH1F *h_ecals_coney= new TH1F("h_ecals_coney", "Cone y", 100,-0.5,1);
  TH1F *h_ecals_conez= new TH1F("h_ecals_conez", "Cone z", 100,-0.5,1);
  TH1F *h_ecals_fdirx = new TH1F("h_ecals_fdirx", "Front Direction x",100,-2,2);
  TH1F *h_ecals_fdiry = new TH1F("h_ecals_fdiry", "Front Direction y",100,-2,2);
  TH1F *h_ecals_fdirz = new TH1F("h_ecals_fdirz", "Front Direction z",100,-2,2);
  TH1F *h_ecals_fdir_varx = new TH1F("h_ecals_fdir_varx", "Front Direction Var x",100,-500,400);
  TH1F *h_ecals_fdir_vary = new TH1F("h_ecals_fdir_vary", "Front Direction Var y",100,-600,400);
  TH1F *h_ecals_fdir_varz = new TH1F("h_ecals_fdir_varz", "Front Direction Var z",100,-1000,400);
  TH1F *h_ecals_bdirx = new TH1F("h_ecals_bdirx", "Back Direction x",100,-2,2);
  TH1F *h_ecals_bdiry = new TH1F("h_ecals_bdiry", "Back Direction y",100,-2,2);
  TH1F *h_ecals_bdirz = new TH1F("h_ecals_bdirz", "Back Direction z",100,-2,2);
  TH1F *h_ecals_bdir_varx = new TH1F("h_ecals_bdir_varx", "Back Direction Var x",100,-400,400);
  TH1F *h_ecals_bdir_varx_full = new TH1F("h_ecals_bdir_varx_full", "Back Direction Var x (Full)",100,-400,400);
  TH1F *h_ecals_bdir_vary = new TH1F("h_ecals_bdir_vary", "Back Direction Var y",100,-400,800);
  TH1F *h_ecals_bdir_varz = new TH1F("h_ecals_bdir_varz", "Back Direction Var z",100,-500,400);
  TH1F *h_ecals_UniqueID                   = new TH1F("h_ecals_UniqueID", "h_ecals_UniqueID",100,0,3000000);
  TH1F *h_ecals_Status                     = new TH1F("h_ecals_Status", "h_ecals_Status", 100,0,4);
  TH1F *h_ecals_Detector                   = new TH1F("h_ecals_Detector", "h_ecals_Detector",100,0,12);
  TH1F *h_ecals_NNodes               = new TH1F("h_ecals_NNodes", "h_ecals_NNodes",100,0,40);
  TH1F *h_ecals_NDOF                       = new TH1F("h_ecals_NDOF", "h_ecals_NDOF",100,-10,40);
  TH1F *h_ecals_Chi2                       = new TH1F("h_ecals_Chi2", "h_ecals_Chi2",100,0,50000);
  TH1F *h_ecals_EDeposit                   = new TH1F("h_ecals_EDeposit","h_ecals_EDeposit",100,0,900);
  TH1F *h_ecals_NConstituents              = new TH1F("h_ecals_NConstituents", "h_ecals_NConstituents",100,0,4);
  TH1F *h_ecals_Length                     =new TH1F("h_ecals_Length", "h_ecals_Length", 100,-10000,110000);
  TH1F *h_ecals_FrontMomentum              = new TH1F("h_ecals_FrontMomentum", "h_ecals_FrontMomentum", 100,-100000,400000);
  TH1F *h_ecals_BackMomentum              = new TH1F("h_ecals_BackMomentum", "h_ecals_BackMomentum", 100,-200000,700000);
  TH1F *h_ecals_FrontMomentumError         = new TH1F("h_ecals_FrontMomentumError", "h_ecals_FrontMomentumError", 100,-5000000,30000000);
  TH1F *h_ecals_BackMomentumError         = new TH1F("h_ecals_BackMomentumError", "h_ecals_BackMomentumError", 100,-20000000,30000000);
  TH1F *h_ecals_TrueParticleID         = new TH1F("h_ecals_TrueParticleID ","h_ecals_TrueParticleID ",100,0,60000);
  TH1F *h_ecals_TrueParticlePur        = new TH1F("h_ecals_TrueParticlePur ","h_ecals_TrueParticlePur ",100,0,4);
  TH1F *h_ecals_TrueParticleEff        = new TH1F("h_ecals_TrueParticleEff ","h_ecals_TrueParticleEff ",100,0,4);
  TH1F *h_ecals_TrueParticleVertexPositionX     = new TH1F("h_ecals_TrueParticleVertexPositionX ","h_ecals_TrueParticleVertexPositionX ",100,-3200,3200);
  TH1F *h_ecals_TrueParticleVertexPositionY     = new TH1F("h_ecals_TrueParticleVertexPositionY ","h_ecals_TrueParticleVertexPositionY ",100,-4000,4000);
  TH1F *h_ecals_TrueParticleVertexPositionZ     = new TH1F("h_ecals_TrueParticleVertexPositionZ ","h_ecals_TrueParticleVertexPositionZ ",100,-5000,5000);
  TH1F *h_ecals_TrueParticleVertexPositionT     = new TH1F("h_ecals_TrueParticleVertexPositionT ","h_ecals_TrueParticleVertexPositionT ",100,0,5000);
  TH1F *h_ecals_TrueParticleVertexID         = new TH1F("h_ecals_TrueParticleVertexID ","h_ecals_TrueParticleVertexID ",100,0,41000);
  TH1F *h_ecals_TrueParticleVertexPur         = new TH1F("h_ecals_TrueParticleVertexPur ","h_ecals_TrueParticleVertexPur ",100,-10,10);
  TH1F *h_ecals_TrueParticleVertexPur_large     = new TH1F("h_ecals_TrueParticleVertexPur_large ","h_ecals_TrueParticleVertexPur_large ",100,0,100);
  TH1F *h_ecals_TrueParticleVertexEff           = new TH1F("h_ecals_TrueParticleVertexEff ","h_ecals_TrueParticleVertexEff ",100,-10,10);
  TH1F *h_ecals_TrueParticleVertexEff_large     = new TH1F("h_ecals_TrueParticleVertexEff_large ","h_ecals_TrueParticleVertexEff_large ",100,0,100);
  TH1F *h_ecals_pid_asymmetry_small= new TH1F("h_ecals_pid_asymmetry_small", "PID Asymmetry_small", 100,-0.5,5);

  //SMRD OBJECT

  TH1F *h_smrd_UniqueID          = new TH1F("h_smrd_UniqueID          ","h_smrd_UniqueID          ",100,0,3000e3);
  TH1F *h_smrd_Status            = new TH1F("h_smrd_Status            ","h_smrd_Status            ",100,0,4);
  TH1F *h_smrd_Detector          = new TH1F("h_smrd_Detector          ","h_smrd_Detector          ",100,0,8);
  TH1F *h_smrd_NHits             = new TH1F("h_smrd_NHits             ","h_smrd_NHits             ",100,0,16);
  TH1F *h_smrd_NNodes            = new TH1F("h_smrd_NNodes            ","h_smrd_NNodes            ",100,0,16);
  TH1F *h_smrd_NDOF              = new TH1F("h_smrd_NDOF              ","h_smrd_NDOF              ",100,-10,30);
  TH1F *h_smrd_Chi2              = new TH1F("h_smrd_Chi2              ","h_smrd_Chi2              ",100,0,6300);
  TH1F *h_smrd_EDeposit          = new TH1F("h_smrd_EDeposit          ","h_smrd_EDeposit          ",100,-5000,10000);
  TH1F *h_smrd_NConstituents     = new TH1F("h_smrd_NConstituents     ","h_smrd_NConstituents     ",100,-2,2);
  TH1F *h_smrd_Length            = new TH1F("h_smrd_Length            ","h_smrd_Length            ",100,-10000,50000);
  TH1F *h_smrd_FrontPositionT    = new TH1F("h_smrd_FrontPositionT    ","h_smrd_FrontPositionT    ",100,2000,13500);
  TH1F *h_smrd_FrontPositionX    = new TH1F("h_smrd_FrontPositionX    ","h_smrd_FrontPositionX    ",100,-10000,120000);
  TH1F *h_smrd_FrontPositionY    = new TH1F("h_smrd_FrontPositionY    ","h_smrd_FrontPositionY    ",100,-15000,90000);
  TH1F *h_smrd_FrontPositionZ    = new TH1F("h_smrd_FrontPositionZ    ","h_smrd_FrontPositionZ    ",100,-30000,55000);
  TH1F *h_smrd_BackPositionT     = new TH1F("h_smrd_BackPositionT     ","h_smrd_BackPositionT     ",100,2000,13500);
  TH1F *h_smrd_BackPositionX     = new TH1F("h_smrd_BackPositionX     ","h_smrd_BackPositionX     ",100,-10000,120000);
  TH1F *h_smrd_BackPositionY     = new TH1F("h_smrd_BackPositionY     ","h_smrd_BackPositionY     ",100,-15000,90000);
  TH1F *h_smrd_BackPositionZ     = new TH1F("h_smrd_BackPositionZ     ","h_smrd_BackPositionZ     ",100,-10000,50000);
  TH1F *h_smrd_FrontPositionVarT = new TH1F("h_smrd_FrontPositionVarT ","h_smrd_FrontPositionVarT ",100,-100,100);
  TH1F *h_smrd_FrontPositionVarX = new TH1F("h_smrd_FrontPositionVarX ","h_smrd_FrontPositionVarX ",100,-500e3,500e3);
  TH1F *h_smrd_FrontPositionVarY = new TH1F("h_smrd_FrontPositionVarY ","h_smrd_FrontPositionVarY ",100,-500e3,500e3);
  TH1F *h_smrd_FrontPositionVarZ = new TH1F("h_smrd_FrontPositionVarZ ","h_smrd_FrontPositionVarZ ",100,-500e3,500e3);
  TH1F *h_smrd_BackPositionVarT  = new TH1F("h_smrd_BackPositionVarT  ","h_smrd_BackPositionVarT  ",100,-100,100);
  TH1F *h_smrd_BackPositionVarX  = new TH1F("h_smrd_BackPositionVarX  ","h_smrd_BackPositionVarX  ",100,-2000e3,2000e3);
  TH1F *h_smrd_BackPositionVarY  = new TH1F("h_smrd_BackPositionVarY  ","h_smrd_BackPositionVarY  ",100,-2000e3,2000e3);
  TH1F *h_smrd_BackPositionVarZ  = new TH1F("h_smrd_BackPositionVarZ  ","h_smrd_BackPositionVarZ  ",100,-2000e3,2000e3);
  TH1F *h_smrd_FrontDirectionX   = new TH1F("h_smrd_FrontDirectionX   ","h_smrd_FrontDirectionX   ",100,-5,5);
  TH1F *h_smrd_FrontDirectionY   = new TH1F("h_smrd_FrontDirectionY   ","h_smrd_FrontDirectionY   ",100,-5,5);
  TH1F *h_smrd_FrontDirectionZ   = new TH1F("h_smrd_FrontDirectionZ   ","h_smrd_FrontDirectionZ   ",100,-5,5);
  TH1F *h_smrd_BackDirectionX    = new TH1F("h_smrd_BackDirectionX    ","h_smrd_BackDirectionX    ",100,-5,5);
  TH1F *h_smrd_BackDirectionY    = new TH1F("h_smrd_BackDirectionY    ","h_smrd_BackDirectionY    ",100,-5,5);
  TH1F *h_smrd_BackDirectionZ    = new TH1F("h_smrd_BackDirectionZ    ","h_smrd_BackDirectionZ    ",100,-5,5);
  TH1F *h_smrd_FrontDirectionVarX= new TH1F("h_smrd_FrontDirectionVarX","h_smrd_FrontDirectionVarX",100,-200,1800);
  TH1F *h_smrd_FrontDirectionVarY= new TH1F("h_smrd_FrontDirectionVarY","h_smrd_FrontDirectionVarY",100,-100,500);
  TH1F *h_smrd_FrontDirectionVarZ= new TH1F("h_smrd_FrontDirectionVarZ","h_smrd_FrontDirectionVarZ",100,-100,700);
  TH1F *h_smrd_BackDirectionVarX = new TH1F("h_smrd_BackDirectionVarX ","h_smrd_BackDirectionVarX ",100,-10000,50000);
  TH1F *h_smrd_BackDirectionVarY = new TH1F("h_smrd_BackDirectionVarY ","h_smrd_BackDirectionVarY ",100,-10000,55000);
  TH1F *h_smrd_BackDirectionVarZ = new TH1F("h_smrd_BackDirectionVarZ ","h_smrd_BackDirectionVarZ ",100,-20000,60000);
  TH1F *h_smrd_FrontMomentum     = new TH1F("h_smrd_FrontMomentum     ","h_smrd_FrontMomentum     ",100,-25600,140e3);
  TH1F *h_smrd_BackMomentum      = new TH1F("h_smrd_BackMomentum      ","h_smrd_BackMomentum      ",100,-51200,260e3);
  TH1F *h_smrd_FrontMomentumError= new TH1F("h_smrd_FrontMomentumError","h_smrd_FrontMomentumError",100,-7e6,36e6);
  TH1F *h_smrd_BackMomentumError = new TH1F("h_smrd_BackMomentumError ","h_smrd_BackMomentumError ",100,-1.5e7,80e6);
  TH1F *h_smrd_TrueParticleID      = new TH1F("h_smrd_TrueParticleID","h_smrd_TrueParticleID",100,-5000,50000);
  TH1F *h_smrd_TrueParticlePur                = new TH1F("h_smrd_TrueParticlePur","h_smrd_TrueParticlePur",100,-1,2);
  TH1F *h_smrd_TrueParticleEff     = new TH1F("h_smrd_TrueParticleEff","h_smrd_TrueParticleEff",100,-1,2);
  TH1F *h_smrd_TrueParticleVertexPositionX      = new TH1F("h_smrd_TrueParticleVertexPositionX","h_smrd_TrueParticleVertexPositionX",100,-10000,10000);
  TH1F *h_smrd_TrueParticleVertexPositionY    = new TH1F("h_smrd_TrueParticleVertexPositionY","h_smrd_TrueParticleVertexPositionY",100,-10000,10000);
  TH1F *h_smrd_TrueParticleVertexPositionZ      = new TH1F("h_smrd_TrueParticleVertexPositionZ","h_smrd_TrueParticleVertexPositionZ",100,-10000,10000);
  TH1F *h_smrd_TrueParticleVertexPositionT    = new TH1F("h_smrd_TrueParticleVertexPositionT","h_smrd_TrueParticleVertexPositionT",100,-10000,10000);
  TH1F *h_smrd_TrueParticleVertexID             = new TH1F("h_smrd_TrueParticleVertexID","h_smrd_TrueParticleVertexID",100,-2000,42000);
  TH1F *h_smrd_TrueParticleVertexPur          = new TH1F("h_smrd_TrueParticleVertexPur","h_smrd_TrueParticleVertexPur",100,0,100);
  TH1F *h_smrd_TrueParticleVertexEff            = new TH1F("h_smrd_TrueParticleVertexEff","h_smrd_TrueParticleVertexEff",100,0,100);
  TH1F *h_smrd_TrueParticleVertexPur_zoom       = new TH1F("h_smrd_TrueParticleVertexPur_zoom","h_smrd_TrueParticleVertexPur_zoom",100,0,100);
  TH1F *h_smrd_TrueParticleVertexEff_zoom       = new TH1F("h_smrd_TrueParticleVertexEff_zoom","h_smrd_TrueParticleVertexEff_zoom",100,0,100);

  //SMRD HITS

  TH1F *h_smrdhit_Wall               = new TH1F("h_smrdhit_Wall               ","h_smrdhit_Wall               ",100,-1,4);
  TH1F *h_smrdhit_Yoke               = new TH1F("h_smrdhit_Yoke               ","h_smrdhit_Yoke               ",100,0,10);
  TH1F *h_smrdhit_Layer              = new TH1F("h_smrdhit_Layer              ","h_smrdhit_Layer              ",100,0,8);
  TH1F *h_smrdhit_Tower              = new TH1F("h_smrdhit_Tower              ","h_smrdhit_Tower              ",100,0,20);
  TH1F *h_smrdhit_Counter            = new TH1F("h_smrdhit_Counter            ","h_smrdhit_Counter            ",100,-5,25);
  TH1F *h_smrdhit_Charge             = new TH1F("h_smrdhit_Charge             ","h_smrdhit_Charge             ",100,0,3400);
  TH1F *h_smrdhit_Time               = new TH1F("h_smrdhit_Time               ","h_smrdhit_Time               ",100,2000,15000);
  TH1F *h_smrdhit_PositionX          = new TH1F("h_smrdhit_PositionX          ","h_smrdhit_PositionX          ",100,-2400,2400);
  TH1F *h_smrdhit_PositionY          = new TH1F("h_smrdhit_PositionY          ","h_smrdhit_PositionY          ",100,-2400,2400);
  TH1F *h_smrdhit_PositionZ          = new TH1F("h_smrdhit_PositionZ          ","h_smrdhit_PositionZ          ",100,-4200,4200);
  TH1F *h_smrdhit_PositionErrorX     = new TH1F("h_smrdhit_PositionErrorX     ","h_smrdhit_PositionErrorX     ",100,0,100);
  TH1F *h_smrdhit_PositionErrorY     = new TH1F("h_smrdhit_PositionErrorY     ","h_smrdhit_PositionErrorY     ",100,0,100);
  TH1F *h_smrdhit_PositionErrorZ     = new TH1F("h_smrdhit_PositionErrorZ     ","h_smrdhit_PositionErrorZ     ",100,0,260);
  TH1F *h_smrdhit_Type               = new TH1F("h_smrdhit_Type               ","h_smrdhit_Type               ",100,9,13);

  //TPC OTHER OBJECTS

  TH1F *h_tpcoth_charge  = new TH1F("h_tpcoth_charge", "Charge", 30, -1.5, 1.5);
  TH1F *h_tpcoth_detec  = new TH1F("h_tpcoth_detec", "Detector", 20, 0, 4);
  TH1F *h_tpcoth_nhits  = new TH1F("h_tpcoth_nhits", "No. of hits", 50, 0, 50);
  TH1F *h_tpcoth_chi2  = new TH1F("h_tpcoth_chi2", "Chi2", 250, 0, 20000000000.);
  TH1F *h_tpcoth_edep = new TH1F("h_tpcoth_edep", "Energy Deposit", 75, 0, 15000);

  TH1F *h_tpcoth_frontpos_x  = new TH1F("h_tpcoth_frontpos_x",  "Front Position: X",   60, -3000, 3000);
  TH1F *h_tpcoth_frontpos_y  = new TH1F("h_tpcoth_frontpos_y",  "Front Position: Y",   60, -3000, 3000);
  TH1F *h_tpcoth_frontpos_z  = new TH1F("h_tpcoth_frontpos_z",  "Front Position: Z",   80, -2000, 4000);
  TH1F *h_tpcoth_frontpos_t  = new TH1F("h_tpcoth_frontdir_t",  "Front Position: T", 75, 1500, 13500);

  TH1F *h_tpcoth_backpos_x   = new TH1F("h_tpcoth_backpos_x",  "Back Position: X",        60, -3000, 3000);
  TH1F *h_tpcoth_backpos_y   = new TH1F("h_tpcoth_backpos_y",  "Back Position: Y",        60, -3000, 3000);
  TH1F *h_tpcoth_backpos_z   = new TH1F("h_tpcoth_backpos_z",  "Back Position: Z",        80, -2000, 4000);
  TH1F *h_tpcoth_backpos_t   = new TH1F("h_tpcoth_backpos_t",  "Back Position: T",75, 1500, 13500);

  TH1F *h_tpcoth_frontdir_x  = new TH1F("h_tpcoth_frontdir_x",  "Front Direction: X", 60, -1.5, 1.5);
  TH1F *h_tpcoth_frontdir_y  = new TH1F("h_tpcoth_frontdir_y",  "Front Direction: Y", 60, -1.5, 1.5);
  TH1F *h_tpcoth_frontdir_z  = new TH1F("h_tpcoth_frontdir_z",  "Front Direction: Z", 40, -0.5, 1.5);

  TH1F *h_tpcoth_backdir_x  = new TH1F("h_tpcoth_backdir_x",  "Back Direction: X", 60, -1.5, 1.5);
  TH1F *h_tpcoth_backdir_y  = new TH1F("h_tpcoth_backdir_y",  "Back Direction: Y", 60, -1.5, 1.5);
  TH1F *h_tpcoth_backdir_z  = new TH1F("h_tpcoth_backdir_z",  "Back Direction: Z", 40, -0.5, 1.5);

  TH1F *h_tpcoth_mom= new TH1F("h_tpcoth_mom", "Momentum", 105, -1, 699);

  TH1F *h_tpcoth_tpid = new TH1F("h_tpcoth_tpid", "True Particle ID", 62, 0, 3100);
  TH1F *h_tpcoth_tppur  = new TH1F("h_tpcoth_tppur", "True Particle Purity", 25, 0.3, 1.2);
  TH1F *h_tpcoth_tpeff  = new TH1F("h_tpcoth_tpeff", "True Particle Efficiency", 25, 0, 1.2);
  TH1F *h_tpcoth_tpvertpos_x     = new TH1F("h_tpcoth_tpvertpos_x", "True Particle Vertex Position: X", 40, -3000, 3000);
  TH1F *h_tpcoth_tpvertpos_y     = new TH1F("h_tpcoth_tpvertpos_y", "True Particle Vertex Position: Y", 40, -3000, 3000);
  TH1F *h_tpcoth_tpvertpos_z     = new TH1F("h_tpcoth_tpvertpos_z", "True Particle Vertex Position: Z", 60, -4500, 4500);
  TH1F *h_tpcoth_tpvertid      = new TH1F("h_tpcoth_tpvertid", "True Particle Vertex ID", 40, 0, 40000);
  TH1F *h_tpcoth_tpvertpur     = new TH1F("h_tpcoth_tpvertpur", "True Particle Vertex Purity", 240, 0, 2400);
  TH1F *h_tpcoth_tpverteff = new TH1F("h_tpcoth_tpverteff", "True Particle Vertex Efficiency", 200, -180000000000000000., 20000000000000000.);

  //FGD TIME BINS

  TH1F *h_tfgtb_maxtime             = new TH1F("h_tfgtb_maxtime", "Max Time", 1000, 2500, 12700);
  TH1F *h_tfgtb_mintime = new TH1F("h_tfgtb_mintime", "Min Time", 1000, 2500, 12700);
  TH1F *h_tfgtb_nhits = new TH1F("h_tfgtb_nhits", "No. of Hits", 200, 0, 200);
  TH1F *h_tfgtb_rawchargesum = new TH1F("h_tfgtb_rawchargesum", "Raw Charge Sum", 125, 0, 4200);
  TH1F *h_tfgtb_chargeweightedposx = new TH1F("h_tfgtb_chargeweightedposx", "Charge Weighted Position: X", 100, -1000, 1000);
  TH1F *h_tfgtb_chargeweightedposy = new TH1F("h_tfgtb_chargeweightedposy", "Charge Weighted Position: Y", 100, -1000, 1000);
  TH1F *h_tfgtb_chargeweightedposz = new TH1F("h_tfgtb_chargeweightedposz", "Charge Weighted Position: Z", 250, 0, 2000);

  //FGD TIME BINS: FGD1 Outermost Hits

  TH1F *h_tfgtb_fgd1outermosthits_xmax_charge = new TH1F("h_tfgtb_fgd1outermosthits_xmax_charge", "FGD1 Outermost Hits Max X: Charge", 125, 0, 500);
  TH1F *h_tfgtb_fgd1outermosthits_xmax_time   = new TH1F("h_tfgtb_fgd1outermosthits_xmax_time",   "FGD1 Outermost Hits Max X: Time", 250, 1000, 14000);
  TH1F *h_tfgtb_fgd1outermosthits_xmax_pos_x   = new TH1F("h_tfgtb_fgd1outermosthits_xmax_pos_x",  "FGD1 Outermost Hits Max X: X", 100, -1200, 1200);
  TH1F *h_tfgtb_fgd1outermosthits_xmax_pos_y    = new TH1F("h_tfgtb_fgd1outermosthits_xmax_pos_y",  "FGD1 Outermost Hits Max X: Y", 1100, -105000, 5000);
  TH1F *h_tfgtb_fgd1outermosthits_xmax_pos_z    = new TH1F("h_tfgtb_fgd1outermosthits_xmax_pos_z",  "FGD1 Outermost Hits Max X: Z", 30, -100, 600);
  TH1F *h_tfgtb_fgd1outermosthits_xmax_dpos_x   = new TH1F("h_tfgtb_fgd1outermosthits_xmax_dpos_x", "FGD1 Outermost Hits Max X: X Error", 100, 0, 2);
  TH1F *h_tfgtb_fgd1outermosthits_xmax_dpos_y   = new TH1F("h_tfgtb_fgd1outermosthits_xmax_dpos_y", "FGD1 Outermost Hits Max X: Y Error", 100, 0, 300);
  TH1F *h_tfgtb_fgd1outermosthits_xmax_dpos_z   = new TH1F("h_tfgtb_fgd1outermosthits_xmax_dpos_z", "FGD1 Outermost Hits Max X: Z Error", 100, 0, 2);
  TH1F *h_tfgtb_fgd1outermosthits_xmax_type     = new TH1F("h_tfgtb_fgd1outermosthits_xmax_type",   "FGD1 Outermost Hits Max X: Type", 40, 95, 115);

  TH1F *h_tfgtb_fgd1outermosthits_xmin_charge   = new TH1F("h_tfgtb_fgd1outermosthits_xmin_charge", "FGD1 Outermost Hits Min X: Charge", 125, 0, 500);
  TH1F *h_tfgtb_fgd1outermosthits_xmin_time     = new TH1F("h_tfgtb_fgd1outermosthits_xmin_time",   "FGD1 Outermost Hits Min X: Time", 250, 1000, 14000);
  TH1F *h_tfgtb_fgd1outermosthits_xmin_pos_x    = new TH1F("h_tfgtb_fgd1outermosthits_xmin_pos_x",  "FGD1 Outermost Hits Min X: X", 100, -1200, 1200);
  TH1F *h_tfgtb_fgd1outermosthits_xmin_pos_y    = new TH1F("h_tfgtb_fgd1outermosthits_xmin_pos_y",  "FGD1 Outermost Hits Min X: Y", 1100, -105000, 5000);
  TH1F *h_tfgtb_fgd1outermosthits_xmin_pos_z    = new TH1F("h_tfgtb_fgd1outermosthits_xmin_pos_z",  "FGD1 Outermost Hits Min X: Z", 30, -100, 600);
  TH1F *h_tfgtb_fgd1outermosthits_xmin_dpos_x   = new TH1F("h_tfgtb_fgd1outermosthits_xmin_dpos_x", "FGD1 Outermost Hits Min X: X Error", 100, 0, 2);
  TH1F *h_tfgtb_fgd1outermosthits_xmin_dpos_y   = new TH1F("h_tfgtb_fgd1outermosthits_xmin_dpos_y", "FGD1 Outermost Hits Min X: Y Error", 100, 0, 300);
  TH1F *h_tfgtb_fgd1outermosthits_xmin_dpos_z   = new TH1F("h_tfgtb_fgd1outermosthits_xmin_dpos_z", "FGD1 Outermost Hits Min X: Z Error", 100, 0, 2);
  TH1F *h_tfgtb_fgd1outermosthits_xmin_type     = new TH1F("h_tfgtb_fgd1outermosthits_xmin_type",   "FGD1 Outermost Hits Min X: Type", 40, 95, 115);

  TH1F *h_tfgtb_fgd1outermosthits_ymax_charge   = new TH1F("h_tfgtb_fgd1outermosthits_ymax_charge", "FGD1 Outermost Hits Max Y: Charge", 125, 0, 500);
  TH1F *h_tfgtb_fgd1outermosthits_ymax_time     = new TH1F("h_tfgtb_fgd1outermosthits_ymax_time",   "FGD1 Outermost Hits Max Y: Time", 250, 1000, 14000);
  TH1F *h_tfgtb_fgd1outermosthits_ymax_pos_x    = new TH1F("h_tfgtb_fgd1outermosthits_ymax_pos_x",  "FGD1 Outermost Hits Max Y: X", 1100, -105000, 5000);
  TH1F *h_tfgtb_fgd1outermosthits_ymax_pos_y    = new TH1F("h_tfgtb_fgd1outermosthits_ymax_pos_y",  "FGD1 Outermost Hits Max Y: Y", 100, -1200, 1200);
  TH1F *h_tfgtb_fgd1outermosthits_ymax_pos_z    = new TH1F("h_tfgtb_fgd1outermosthits_ymax_pos_z",  "FGD1 Outermost Hits Max Y: Z", 30, -100, 600);
  TH1F *h_tfgtb_fgd1outermosthits_ymax_dpos_x   = new TH1F("h_tfgtb_fgd1outermosthits_ymax_dpos_x", "FGD1 Outermost Hits Max Y: X Error", 100, 0, 300);
  TH1F *h_tfgtb_fgd1outermosthits_ymax_dpos_y   = new TH1F("h_tfgtb_fgd1outermosthits_ymax_dpos_y", "FGD1 Outermost Hits Max Y: Y Error", 100, -0.5, 2.5);
  TH1F *h_tfgtb_fgd1outermosthits_ymax_dpos_z   = new TH1F("h_tfgtb_fgd1outermosthits_ymax_dpos_z", "FGD1 Outermost Hits Max Y: Z Error", 100, 0, 2);
  TH1F *h_tfgtb_fgd1outermosthits_ymax_type     = new TH1F("h_tfgtb_fgd1outermosthits_ymax_type",   "FGD1 Outermost Hits Max Y: Type", 40, 95, 115);

  TH1F *h_tfgtb_fgd1outermosthits_ymin_charge   = new TH1F("h_tfgtb_fgd1outermosthits_ymin_charge", "FGD1 Outermost Hits Min Y: Charge", 150, 0, 600);
  TH1F *h_tfgtb_fgd1outermosthits_ymin_time     = new TH1F("h_tfgtb_fgd1outermosthits_ymin_time",   "FGD1 Outermost Hits Min Y: Time", 250, 1000, 14000);
  TH1F *h_tfgtb_fgd1outermosthits_ymin_pos_x    = new TH1F("h_tfgtb_fgd1outermosthits_ymin_pos_x",  "FGD1 Outermost Hits Min Y: X", 1100, -105000, 5000);
  TH1F *h_tfgtb_fgd1outermosthits_ymin_pos_y    = new TH1F("h_tfgtb_fgd1outermosthits_ymin_pos_y",  "FGD1 Outermost Hits Min Y: Y", 100, -1200, 1200);
  TH1F *h_tfgtb_fgd1outermosthits_ymin_pos_z    = new TH1F("h_tfgtb_fgd1outermosthits_ymin_pos_z",  "FGD1 Outermost Hits Min Y: Z", 30, -100, 600);
  TH1F *h_tfgtb_fgd1outermosthits_ymin_dpos_x   = new TH1F("h_tfgtb_fgd1outermosthits_ymin_dpos_x", "FGD1 Outermost Hits Min Y: X Error", 100, 0, 300);
  TH1F *h_tfgtb_fgd1outermosthits_ymin_dpos_y   = new TH1F("h_tfgtb_fgd1outermosthits_ymin_dpos_y", "FGD1 Outermost Hits Min Y: Y Error", 100, -0.5, 2.5);
  TH1F *h_tfgtb_fgd1outermosthits_ymin_dpos_z   = new TH1F("h_tfgtb_fgd1outermosthits_ymin_dpos_z", "FGD1 Outermost Hits Min Y: Z Error", 100, 0, 2);
  TH1F *h_tfgtb_fgd1outermosthits_ymin_type     = new TH1F("h_tfgtb_fgd1outermosthits_ymin_type",   "FGD1 Outermost Hits Min Y: Type", 40, 95, 115);

  TH1F *h_tfgtb_fgd1outermosthits_zmax_charge   = new TH1F("h_tfgtb_fgd1outermosthits_zmax_charge", "FGD1 Outermost Hits Max Z: Charge", 125, 0, 500);
  TH1F *h_tfgtb_fgd1outermosthits_zmax_time     = new TH1F("h_tfgtb_fgd1outermosthits_zmax_time",   "FGD1 Outermost Hits Max Z: Time", 250, 1000, 14000);
  TH1F *h_tfgtb_fgd1outermosthits_zmax_pos_x    = new TH1F("h_tfgtb_fgd1outermosthits_zmax_pos_x",  "FGD1 Outermost Hits Max Z: X", 100, -1200, 1200);
  TH1F *h_tfgtb_fgd1outermosthits_zmax_pos_y    = new TH1F("h_tfgtb_fgd1outermosthits_zmax_pos_y",  "FGD1 Outermost Hits Max Z: Y", 100, -1200, 1200);
  TH1F *h_tfgtb_fgd1outermosthits_zmax_pos_z    = new TH1F("h_tfgtb_fgd1outermosthits_zmax_pos_z",  "FGD1 Outermost Hits Max Z: Z", 30, -100, 600);
  TH1F *h_tfgtb_fgd1outermosthits_zmax_dpos_x   = new TH1F("h_tfgtb_fgd1outermosthits_zmax_dpos_x", "FGD1 Outermost Hits Max Z: X Error", 150, -200, 400);
  TH1F *h_tfgtb_fgd1outermosthits_zmax_dpos_y   = new TH1F("h_tfgtb_fgd1outermosthits_zmax_dpos_y", "FGD1 Outermost Hits Max Z: Y Error", 150, -200, 400);
  TH1F *h_tfgtb_fgd1outermosthits_zmax_dpos_z   = new TH1F("h_tfgtb_fgd1outermosthits_zmax_dpos_z", "FGD1 Outermost Hits Max Z: Z Error", 100, -1, 2);
  TH1F *h_tfgtb_fgd1outermosthits_zmax_type     = new TH1F("h_tfgtb_fgd1outermosthits_zmax_type",   "FGD1 Outermost Hits Max Z: Type", 40, 95, 115);

  TH1F *h_tfgtb_fgd1outermosthits_zmin_charge   = new TH1F("h_tfgtb_fgd1outermosthits_zmin_charge", "FGD1 Outermost Hits Min Z: Charge", 125, 0, 500);
  TH1F *h_tfgtb_fgd1outermosthits_zmin_time     = new TH1F("h_tfgtb_fgd1outermosthits_zmin_time",   "FGD1 Outermost Hits Min Z: Time", 250, 1000, 14000);
  TH1F *h_tfgtb_fgd1outermosthits_zmin_pos_x    = new TH1F("h_tfgtb_fgd1outermosthits_zmin_pos_x",  "FGD1 Outermost Hits Min Z: X", 100, -1200, 1200);
  TH1F *h_tfgtb_fgd1outermosthits_zmin_pos_y    = new TH1F("h_tfgtb_fgd1outermosthits_zmin_pos_y",  "FGD1 Outermost Hits Min Z: Y", 100, -1200, 1200);
  TH1F *h_tfgtb_fgd1outermosthits_zmin_pos_z    = new TH1F("h_tfgtb_fgd1outermosthits_zmin_pos_z",  "FGD1 Outermost Hits Min Z: Z", 15, 1250, 2050);
  TH1F *h_tfgtb_fgd1outermosthits_zmin_dpos_x   = new TH1F("h_tfgtb_fgd1outermosthits_zmin_dpos_x", "FGD1 Outermost Hits Min Z: X Error", 150, -200, 400);
  TH1F *h_tfgtb_fgd1outermosthits_zmin_dpos_y   = new TH1F("h_tfgtb_fgd1outermosthits_zmin_dpos_y", "FGD1 Outermost Hits Min Z: Y Error", 150, -200, 400);
  TH1F *h_tfgtb_fgd1outermosthits_zmin_dpos_z   = new TH1F("h_tfgtb_fgd1outermosthits_zmin_dpos_z", "FGD1 Outermost Hits Min Z: Z Error", 100, -1, 2);
  TH1F *h_tfgtb_fgd1outermosthits_zmin_type     = new TH1F("h_tfgtb_fgd1outermosthits_zmin_type",   "FGD1 Outermost Hits Min Z: Type", 40, 95, 115);


    //FGD TIME BINS: FGD2 Outermost Hits

  TH1F *h_tfgtb_fgd2outermosthits_xmax_charge   = new TH1F("h_tfgtb_fgd2outermosthits_xmax_charge", "FGD2 Outermost Hits Max X: Charge", 125, 0, 500);
  TH1F *h_tfgtb_fgd2outermosthits_xmax_time     = new TH1F("h_tfgtb_fgd2outermosthits_xmax_time",   "FGD2 Outermost Hits Max X: Time", 250, 1000, 14000);
  TH1F *h_tfgtb_fgd2outermosthits_xmax_pos_x    = new TH1F("h_tfgtb_fgd2outermosthits_xmax_pos_x",  "FGD2 Outermost Hits Max X: X", 100, -1200, 1200);
  TH1F *h_tfgtb_fgd2outermosthits_xmax_pos_y    = new TH1F("h_tfgtb_fgd2outermosthits_xmax_pos_y",  "FGD2 Outermost Hits Max X: Y", 1100, -105000, 5000);
  TH1F *h_tfgtb_fgd2outermosthits_xmax_pos_z    = new TH1F("h_tfgtb_fgd2outermosthits_xmax_pos_z",  "FGD2 Outermost Hits Max X: Z", 15, 1250, 2050);
  TH1F *h_tfgtb_fgd2outermosthits_xmax_dpos_x   = new TH1F("h_tfgtb_fgd2outermosthits_xmax_dpos_x", "FGD2 Outermost Hits Max X: X Error", 100, 0, 2);
  TH1F *h_tfgtb_fgd2outermosthits_xmax_dpos_y   = new TH1F("h_tfgtb_fgd2outermosthits_xmax_dpos_y", "FGD2 Outermost Hits Max X: Y Error", 100, 0, 300);
  TH1F *h_tfgtb_fgd2outermosthits_xmax_dpos_z   = new TH1F("h_tfgtb_fgd2outermosthits_xmax_dpos_z", "FGD2 Outermost Hits Max X: Z Error", 100, 0, 2);
  TH1F *h_tfgtb_fgd2outermosthits_xmax_type     = new TH1F("h_tfgtb_fgd2outermosthits_xmax_type",   "FGD2 Outermost Hits Max X: Type", 40, 95, 115);

  TH1F *h_tfgtb_fgd2outermosthits_xmin_charge   = new TH1F("h_tfgtb_fgd2outermosthits_xmin_charge", "FGD2 Outermost Hits Min X: Charge", 125, 0, 500);
  TH1F *h_tfgtb_fgd2outermosthits_xmin_time     = new TH1F("h_tfgtb_fgd2outermosthits_xmin_time",   "FGD2 Outermost Hits Min X: Time", 250, 1000, 14000);
  TH1F *h_tfgtb_fgd2outermosthits_xmin_pos_x    = new TH1F("h_tfgtb_fgd2outermosthits_xmin_pos_x",  "FGD2 Outermost Hits Min X: X", 100, -1200, 1200);
  TH1F *h_tfgtb_fgd2outermosthits_xmin_pos_y    = new TH1F("h_tfgtb_fgd2outermosthits_xmin_pos_y",  "FGD2 Outermost Hits Min X: Y", 1100, -105000, 5000);
  TH1F *h_tfgtb_fgd2outermosthits_xmin_pos_z    = new TH1F("h_tfgtb_fgd2outermosthits_xmin_pos_z",  "FGD2 Outermost Hits Min X: Z", 15, 1250, 2050);
  TH1F *h_tfgtb_fgd2outermosthits_xmin_dpos_x   = new TH1F("h_tfgtb_fgd2outermosthits_xmin_dpos_x", "FGD2 Outermost Hits Min X: X Error", 100, 0, 2);
  TH1F *h_tfgtb_fgd2outermosthits_xmin_dpos_y   = new TH1F("h_tfgtb_fgd2outermosthits_xmin_dpos_y", "FGD2 Outermost Hits Min X: Y Error", 100, 0, 300);
  TH1F *h_tfgtb_fgd2outermosthits_xmin_dpos_z   = new TH1F("h_tfgtb_fgd2outermosthits_xmin_dpos_z", "FGD2 Outermost Hits Min X: Z Error", 100, 0, 2);
  TH1F *h_tfgtb_fgd2outermosthits_xmin_type     = new TH1F("h_tfgtb_fgd2outermosthits_xmin_type",   "FGD2 Outermost Hits Min X: Type", 40, 95, 115);

  TH1F *h_tfgtb_fgd2outermosthits_ymax_charge   = new TH1F("h_tfgtb_fgd2outermosthits_ymax_charge", "FGD2 Outermost Hits Max Y: Charge", 175, 0, 700);
  TH1F *h_tfgtb_fgd2outermosthits_ymax_time     = new TH1F("h_tfgtb_fgd2outermosthits_ymax_time",   "FGD2 Outermost Hits Max Y: Time", 250, 1000, 14000);
  TH1F *h_tfgtb_fgd2outermosthits_ymax_pos_x    = new TH1F("h_tfgtb_fgd2outermosthits_ymax_pos_x",  "FGD2 Outermost Hits Max Y: X", 1100, -105000, 5000);
  TH1F *h_tfgtb_fgd2outermosthits_ymax_pos_y    = new TH1F("h_tfgtb_fgd2outermosthits_ymax_pos_y",  "FGD2 Outermost Hits Max Y: Y", 100, -1200, 1200);
  TH1F *h_tfgtb_fgd2outermosthits_ymax_pos_z    = new TH1F("h_tfgtb_fgd2outermosthits_ymax_pos_z",  "FGD2 Outermost Hits Max Y: Z", 15, 1250, 2050);
  TH1F *h_tfgtb_fgd2outermosthits_ymax_dpos_x   = new TH1F("h_tfgtb_fgd2outermosthits_ymax_dpos_x", "FGD2 Outermost Hits Max Y: X Error", 100, 0, 300);
  TH1F *h_tfgtb_fgd2outermosthits_ymax_dpos_y   = new TH1F("h_tfgtb_fgd2outermosthits_ymax_dpos_y", "FGD2 Outermost Hits Max Y: Y Error", 100, -0.5, 2.5);
  TH1F *h_tfgtb_fgd2outermosthits_ymax_dpos_z   = new TH1F("h_tfgtb_fgd2outermosthits_ymax_dpos_z", "FGD2 Outermost Hits Max Y: Z Error", 100, 0, 2);
  TH1F *h_tfgtb_fgd2outermosthits_ymax_type     = new TH1F("h_tfgtb_fgd2outermosthits_ymax_type",   "FGD2 Outermost Hits Max Y: Type", 40, 95, 115);

  TH1F *h_tfgtb_fgd2outermosthits_ymin_charge   = new TH1F("h_tfgtb_fgd2outermosthits_ymin_charge", "FGD2 Outermost Hits Min Y: Charge", 150, 0, 600);
  TH1F *h_tfgtb_fgd2outermosthits_ymin_time     = new TH1F("h_tfgtb_fgd2outermosthits_ymin_time",   "FGD2 Outermost Hits Min Y: Time", 250, 1000, 14000);
  TH1F *h_tfgtb_fgd2outermosthits_ymin_pos_x    = new TH1F("h_tfgtb_fgd2outermosthits_ymin_pos_x",  "FGD2 Outermost Hits Min Y: X", 1100, -105000, 5000);
  TH1F *h_tfgtb_fgd2outermosthits_ymin_pos_y    = new TH1F("h_tfgtb_fgd2outermosthits_ymin_pos_y",  "FGD2 Outermost Hits Min Y: Y", 100, -1200, 1200);
  TH1F *h_tfgtb_fgd2outermosthits_ymin_pos_z    = new TH1F("h_tfgtb_fgd2outermosthits_ymin_pos_z",  "FGD2 Outermost Hits Min Y: Z", 15, 1250, 2050);
  TH1F *h_tfgtb_fgd2outermosthits_ymin_dpos_x   = new TH1F("h_tfgtb_fgd2outermosthits_ymin_dpos_x", "FGD2 Outermost Hits Min Y: X Error", 100, 0, 300);
  TH1F *h_tfgtb_fgd2outermosthits_ymin_dpos_y   = new TH1F("h_tfgtb_fgd2outermosthits_ymin_dpos_y", "FGD2 Outermost Hits Min Y: Y Error", 100, -0.5, 2.5);
  TH1F *h_tfgtb_fgd2outermosthits_ymin_dpos_z   = new TH1F("h_tfgtb_fgd2outermosthits_ymin_dpos_z", "FGD2 Outermost Hits Min Y: Z Error", 100, 0, 2);
  TH1F *h_tfgtb_fgd2outermosthits_ymin_type     = new TH1F("h_tfgtb_fgd2outermosthits_ymin_type",   "FGD2 Outermost Hits Min Y: Type", 40, 95, 115);

  TH1F *h_tfgtb_fgd2outermosthits_zmax_charge   = new TH1F("h_tfgtb_fgd2outermosthits_zmax_charge", "FGD2 Outermost Hits Max Z: Charge", 125, 0, 500);
  TH1F *h_tfgtb_fgd2outermosthits_zmax_time     = new TH1F("h_tfgtb_fgd2outermosthits_zmax_time",   "FGD2 Outermost Hits Max Z: Time", 250, 1000, 14000);
  TH1F *h_tfgtb_fgd2outermosthits_zmax_pos_x    = new TH1F("h_tfgtb_fgd2outermosthits_zmax_pos_x",  "FGD2 Outermost Hits Max Z: X", 100, -1200, 1200);
  TH1F *h_tfgtb_fgd2outermosthits_zmax_pos_y    = new TH1F("h_tfgtb_fgd2outermosthits_zmax_pos_y",  "FGD2 Outermost Hits Max Z: Y", 100, -1200, 1200);
  TH1F *h_tfgtb_fgd2outermosthits_zmax_pos_z    = new TH1F("h_tfgtb_fgd2outermosthits_zmax_pos_z",  "FGD2 Outermost Hits Max Z: Z", 15, 1250, 2050);
  TH1F *h_tfgtb_fgd2outermosthits_zmax_dpos_x   = new TH1F("h_tfgtb_fgd2outermosthits_zmax_dpos_x", "FGD2 Outermost Hits Max Z: X Error", 150, -200, 400);
  TH1F *h_tfgtb_fgd2outermosthits_zmax_dpos_y   = new TH1F("h_tfgtb_fgd2outermosthits_zmax_dpos_y", "FGD2 Outermost Hits Max Z: Y Error", 150, -200, 400);
  TH1F *h_tfgtb_fgd2outermosthits_zmax_dpos_z   = new TH1F("h_tfgtb_fgd2outermosthits_zmax_dpos_z", "FGD2 Outermost Hits Max Z: Z Error", 100, -1, 2);
  TH1F *h_tfgtb_fgd2outermosthits_zmax_type     = new TH1F("h_tfgtb_fgd2outermosthits_zmax_type",   "FGD2 Outermost Hits Max Z: Type", 40, 95, 115);

  TH1F *h_tfgtb_fgd2outermosthits_zmin_charge   = new TH1F("h_tfgtb_fgd2outermosthits_zmin_charge", "FGD2 Outermost Hits Min Z: Charge", 175, 0, 700);
  TH1F *h_tfgtb_fgd2outermosthits_zmin_time     = new TH1F("h_tfgtb_fgd2outermosthits_zmin_time",   "FGD2 Outermost Hits Min Z: Time", 250, 1000, 14000);
  TH1F *h_tfgtb_fgd2outermosthits_zmin_pos_x    = new TH1F("h_tfgtb_fgd2outermosthits_zmin_pos_x",  "FGD2 Outermost Hits Min Z: X", 100, -1200, 1200);
  TH1F *h_tfgtb_fgd2outermosthits_zmin_pos_y    = new TH1F("h_tfgtb_fgd2outermosthits_zmin_pos_y",  "FGD2 Outermost Hits Min Z: Y", 100, -1200, 1200);
  TH1F *h_tfgtb_fgd2outermosthits_zmin_pos_z    = new TH1F("h_tfgtb_fgd2outermosthits_zmin_pos_z",  "FGD2 Outermost Hits Min Z: Z", 15, 1250, 2050);
  TH1F *h_tfgtb_fgd2outermosthits_zmin_dpos_x   = new TH1F("h_tfgtb_fgd2outermosthits_zmin_dpos_x", "FGD2 Outermost Hits Min Z: X Error", 150, -200, 400);
  TH1F *h_tfgtb_fgd2outermosthits_zmin_dpos_y   = new TH1F("h_tfgtb_fgd2outermosthits_zmin_dpos_y", "FGD2 Outermost Hits Min Z: Y Error", 150, -200, 400);
  TH1F *h_tfgtb_fgd2outermosthits_zmin_dpos_z   = new TH1F("h_tfgtb_fgd2outermosthits_zmin_dpos_z", "FGD2 Outermost Hits Min Z: Z Error", 100, -1, 2);
  TH1F *h_tfgtb_fgd2outermosthits_zmin_type     = new TH1F("h_tfgtb_fgd2outermosthits_zmin_type",   "FGD2 Outermost Hits Min Z: Type", 40, 95, 115);

  //FGD TIME BINS: not outermost hits

  TH1F *h_tfgtb_nfgd1unused = new TH1F("h_tfgtb_nfgd1unused", "No. FGD1 Unused", 50, 0, 100);
  //  TH1F *h_tfgtb_fgd1unused = new TH1F("h_tfgtb_fgd1unused", "FGD1 Unused", 1000, 0, 100);
  TH1F *h_tfgtb_nfgd2unused = new TH1F("h_tfgtb_nfgd2unused", "No. FGD2 Unused", 50, 0, 100);
  //  TH1F *h_tfgtb_fgd2unused = new TH1F("h_tfgtb_fgd2unused", "FGD1 Unused", 1000, 0, 100);
  TH1F *h_tfgtb_g4id = new TH1F("h_tfgtb_g4id", "G4ID", 100, 0, 50000);

  TH1F *h_tfgtb_hackfgdunused_charge = new TH1F("h_tfgtb_hackfgdunused_charge", "FGD Unused: Charge", 100, 0, 0.1);
  TH1F *h_tfgtb_hackfgdunused_time   = new TH1F("h_tfgtb_hackfgdunused_time", "FGD Unused: Time", 100, 0, 0.1);
  TH1F *h_tfgtb_hackfgdunused_pos_x = new TH1F("h_tfgtb_hackfgdunused_pos_x", "FGD Unused: Position X", 100, 0, 0.1);
  TH1F *h_tfgtb_hackfgdunused_pos_y = new TH1F("h_tfgtb_hackfgdunused_pos_y", "FGD Unused: Position Y", 100, 0, 0.1);
  TH1F *h_tfgtb_hackfgdunused_pos_z = new TH1F("h_tfgtb_hackfgdunused_pos_z", "FGD Unused: Position Z", 100, 0, 0.1);
  TH1F *h_tfgtb_hackfgdunused_dpos_x = new TH1F("h_tfgtb_hackfgdunused_dpos_x", "FGD Unused: X Error", 100, 0, 0.1);
  TH1F *h_tfgtb_hackfgdunused_dpos_y = new TH1F("h_tfgtb_hackfgdunused_dpos_y", "FGD Unused: Y Error", 100, 0, 0.1);
  TH1F *h_tfgtb_hackfgdunused_dpos_z = new TH1F("h_tfgtb_hackfgdunused_dpos_z", "FGD Unused: Z Error", 100, 0, 0.1);
  TH1F *h_tfgtb_hackfgdunused_type = new TH1F("h_tfgtb_hackfgdunused_type", "FGD Unused: Type", 500, -50000000, 900000000);

  //General Node information

  TH1F *h_annodes = new TH1F("h_annodes", "AllNNodes", 100, 0.0, 100.0);
  TH1F *h_nnodes = new TH1F("h_nnodes", "NNodes", 100, 0.0, 100.0);

  std::cout << "Adding file(s) matching string: " << inputfilename << std::endl;
  std::cout << "Added "<< globTreeChain.AddFile(inputfilename) << " file(s)"<< std::endl;

  Int_t NVertices, RunID, EventID, Preselected, SubrunID;
  Int_t NDelayedClusters, NFgdTimeBins, NPIDs, NTPCOthers, NFGD1Unused, NFGD2Unused, NP0DUnused, NSMRDUnused, NSMRDTopUnused, NSMRDBottomUnused, NSMRDLeftUnused, NSMRDRightUnused, NTPCUnused, EarliestTrackTime, NTPC, NFGD, NP0D, NSMRD, NTPCIso, NFGDIso, NP0DIso, NSMRDIso;

  TClonesArray *Vertices =  new TClonesArray("ND::TGlobalReconModule::TGlobalVertex", 20);
  //TClonesArray *DelayedClusters =  new TClonesArray("ND::TGlobalReconModule::TFgdCluster", 10);
  TClonesArray *FgdTimeBins =  new TClonesArray("ND::TGlobalReconModule::TFgdTimeBin", 20);
  TClonesArray *PIDs =  new TClonesArray("ND::TGlobalReconModule::TGlobalPID", 20);
  TClonesArray *TPCOthers =  new TClonesArray("ND::TGlobalReconModule::TTPCOtherObject", 20);
  TClonesArray *P0DUnused =   new TClonesArray("ND::TGlobalReconModule::TGlobalHit", 100);
  TClonesArray *SMRDUnused = new TClonesArray("ND::TGlobalReconModule::TSMRDHit", 100);
  /*
  TClonesArray *ECAL = new TClonesArray("ND:TGlobalReconModule::TECalObject", 1);
  TClonesArray *FGD = new TClonesArray("ND:TGlobalReconModule::TFGDObject", 1);

  TClonesArray *HitsSaved = new TClonesArray("ND::TGlobalReconModule::TGlobalHit", 4);
  */
  // Set the branches for the TClonesArray of oaanalysis objects (as defined in
  // the relevant module) as well as the integer that stores the number of
  // these stored in the TClonesArray.

  globTreeChain.SetBranchAddress("RunID",&RunID);
  globTreeChain.SetBranchAddress("EventID",&EventID);
  globTreeChain.SetBranchAddress("Preselected",&Preselected);
  globTreeChain.SetBranchAddress("SubrunID",&SubrunID);

  //P0D Outermost Hits
  ND::TGlobalReconModule::TOutermostHits* P0DOutermostHits = new ND::TGlobalReconModule::TOutermostHits();
  globTreeChain.SetBranchAddress("P0DOutermostHits",&P0DOutermostHits);

  //Vertices
  globTreeChain.SetBranchAddress("NVertices",&NVertices);
  globTreeChain.SetBranchAddress("Vertices",&Vertices);

  //Delayed Clusters
  //globTreeChain.SetBranchAddress("NDelayedClusters",&NDelayedClusters);
  //globTreeChain.SetBranchAddress("DelayedClusters",&DelayedClusters);

  //FGD Time Bins
  globTreeChain.SetBranchAddress("NFgdTimeBins",&NFgdTimeBins);
  globTreeChain.SetBranchAddress("FgdTimeBins",&FgdTimeBins);

  //PIDs
  globTreeChain.SetBranchAddress("NPIDs",&NPIDs);
  globTreeChain.SetBranchAddress("PIDs",&PIDs);

  //TPCOthers
  globTreeChain.SetBranchAddress("NTPCOthers",&NTPCOthers);
  globTreeChain.SetBranchAddress("TPCOthers",&TPCOthers);

  //FGD1Unused
  globTreeChain.SetBranchAddress("NFGD1Unused",&NFGD1Unused);
  globTreeChain.SetBranchAddress("NFGD2Unused",&NFGD2Unused);

  //P0DUnused
  globTreeChain.SetBranchAddress("NP0DUnused",&NP0DUnused);
  globTreeChain.SetBranchAddress("P0DUnused",&P0DUnused);

  //SMRDUnused
  globTreeChain.SetBranchAddress("NSMRDUnused",&NSMRDUnused);
  globTreeChain.SetBranchAddress("SMRDUnused",&SMRDUnused);
  globTreeChain.SetBranchAddress("NSMRDTopUnused",&NSMRDTopUnused);
  globTreeChain.SetBranchAddress("NSMRDBottomUnused",&NSMRDBottomUnused);
  globTreeChain.SetBranchAddress("NSMRDLeftUnused",&NSMRDLeftUnused);
  globTreeChain.SetBranchAddress("NSMRDRightUnused",&NSMRDRightUnused);

  //TPCUnused
  globTreeChain.SetBranchAddress("NTPCUnused",&NTPCUnused);


  //Earliest Track time
  globTreeChain.SetBranchAddress("EarliestTrackTime",&EarliestTrackTime);

  //N detector
  globTreeChain.SetBranchAddress("NTPC",&NTPC);
  globTreeChain.SetBranchAddress("NFGD",&NFGD);
  globTreeChain.SetBranchAddress("NP0D",&NP0D);
  globTreeChain.SetBranchAddress("NSMRD",&NSMRD);

  //N detector Iso
  globTreeChain.SetBranchAddress("NTPCIso",&NTPCIso);
  globTreeChain.SetBranchAddress("NFGDIso",&NFGDIso);
  globTreeChain.SetBranchAddress("NP0DIso",&NP0DIso);
  globTreeChain.SetBranchAddress("NSMRDIso",&NSMRDIso);

  // Get a utilities class for use with the oaanalysis objects. In this case
  // use the cheating utils as it contains methods for querying truth
  // trajectories.
  //  TCheatingAnalysisUtils cheatUtils;

  // _________________________________________________________________________
  // Now loop over the entries in the input tree. Each entry corresponds to an
  // ND280Event and it is safe to assume a correspondence between entries on
  // different trees in the same analysis output file.
  int entries = globTreeChain.GetEntries();
  if(max_entries > 0 && max_entries<entries) entries = max_entries;
  std::cout << "Looping over " << entries<< " events " << std::endl;
  ND::TGlobalReconModule::TGlobalVertex * curr_vert;
  ND::TGlobalReconModule::TGlobalPID * curr_pid;

  int fill=0;
  int fill_ecal=0;
  int fill_pidalt=0;

  for(int ient = 0; ient < entries; ient++)
  {
    //      cout << "\n======== Getting event/entry "<< ient <<" ==========" << endl;
      globTreeChain.GetEntry(ient);
      //   cout << "     -> the TClonesArray contains "<< NPIDs
      //                                                << " PIDs" << endl;

      h_runid->Fill(RunID);
      h_eventid->Fill(EventID);
      h_preselected->Fill(Preselected);
      h_subrunid->Fill(SubrunID);
      h_nvertices->Fill(NVertices);
      h_npids->Fill(NPIDs);


//P0D Outermost Hits
//------------------

  h_p0doutermosthits_xmax_charge   ->Fill(P0DOutermostHits->hitMaxX.Charge);
  h_p0doutermosthits_xmax_time     ->Fill(P0DOutermostHits->hitMaxX.Time);
  h_p0doutermosthits_xmax_pos_x    ->Fill(P0DOutermostHits->hitMaxX.Position.X());
  h_p0doutermosthits_xmax_pos_y    ->Fill(P0DOutermostHits->hitMaxX.Position.Y());
  h_p0doutermosthits_xmax_pos_z    ->Fill(P0DOutermostHits->hitMaxX.Position.Z());
  h_p0doutermosthits_xmax_dpos_x   ->Fill(P0DOutermostHits->hitMaxX.PositionError.X());
  h_p0doutermosthits_xmax_dpos_y   ->Fill(P0DOutermostHits->hitMaxX.PositionError.Y());
  h_p0doutermosthits_xmax_dpos_z   ->Fill(P0DOutermostHits->hitMaxX.PositionError.Z());
  h_p0doutermosthits_xmax_type     ->Fill(P0DOutermostHits->hitMaxX.Type);

  h_p0doutermosthits_xmin_charge   ->Fill(P0DOutermostHits->hitMinX.Charge);
  h_p0doutermosthits_xmin_time     ->Fill(P0DOutermostHits->hitMinX.Time);
  h_p0doutermosthits_xmin_pos_x    ->Fill(P0DOutermostHits->hitMinX.Position.X());
  h_p0doutermosthits_xmin_pos_y    ->Fill(P0DOutermostHits->hitMinX.Position.Y());
  h_p0doutermosthits_xmin_pos_z    ->Fill(P0DOutermostHits->hitMinX.Position.Z());
  h_p0doutermosthits_xmin_dpos_x   ->Fill(P0DOutermostHits->hitMinX.PositionError.X());
  h_p0doutermosthits_xmin_dpos_y   ->Fill(P0DOutermostHits->hitMinX.PositionError.Y());
  h_p0doutermosthits_xmin_dpos_z   ->Fill(P0DOutermostHits->hitMinX.PositionError.Z());
  h_p0doutermosthits_xmin_type     ->Fill(P0DOutermostHits->hitMinX.Type);

  h_p0doutermosthits_ymax_charge   ->Fill(P0DOutermostHits->hitMaxY.Charge);
  h_p0doutermosthits_ymax_time     ->Fill(P0DOutermostHits->hitMaxY.Time);
  h_p0doutermosthits_ymax_pos_x    ->Fill(P0DOutermostHits->hitMaxY.Position.X());
  h_p0doutermosthits_ymax_pos_y    ->Fill(P0DOutermostHits->hitMaxY.Position.Y());
  h_p0doutermosthits_ymax_pos_z    ->Fill(P0DOutermostHits->hitMaxY.Position.Z());
  h_p0doutermosthits_ymax_dpos_x   ->Fill(P0DOutermostHits->hitMaxY.PositionError.X());
  h_p0doutermosthits_ymax_dpos_y   ->Fill(P0DOutermostHits->hitMaxY.PositionError.Y());
  h_p0doutermosthits_ymax_dpos_z   ->Fill(P0DOutermostHits->hitMaxY.PositionError.Z());
  h_p0doutermosthits_ymax_type     ->Fill(P0DOutermostHits->hitMaxY.Type);

  h_p0doutermosthits_ymin_charge   ->Fill(P0DOutermostHits->hitMinY.Charge);
  h_p0doutermosthits_ymin_time     ->Fill(P0DOutermostHits->hitMinY.Time);
  h_p0doutermosthits_ymin_pos_x    ->Fill(P0DOutermostHits->hitMinY.Position.X());
  h_p0doutermosthits_ymin_pos_y    ->Fill(P0DOutermostHits->hitMinY.Position.Y());
  h_p0doutermosthits_ymin_pos_z    ->Fill(P0DOutermostHits->hitMinY.Position.Z());
  h_p0doutermosthits_ymin_dpos_x   ->Fill(P0DOutermostHits->hitMinY.PositionError.X());
  h_p0doutermosthits_ymin_dpos_y   ->Fill(P0DOutermostHits->hitMinY.PositionError.Y());
  h_p0doutermosthits_ymin_dpos_z   ->Fill(P0DOutermostHits->hitMinY.PositionError.Z());
  h_p0doutermosthits_ymin_type     ->Fill(P0DOutermostHits->hitMinY.Type);

  h_p0doutermosthits_zmax_charge   ->Fill(P0DOutermostHits->hitMaxZ.Charge);
  h_p0doutermosthits_zmax_time     ->Fill(P0DOutermostHits->hitMaxZ.Time);
  h_p0doutermosthits_zmax_pos_x    ->Fill(P0DOutermostHits->hitMaxZ.Position.X());
  h_p0doutermosthits_zmax_pos_y    ->Fill(P0DOutermostHits->hitMaxZ.Position.Y());
  h_p0doutermosthits_zmax_pos_z    ->Fill(P0DOutermostHits->hitMaxZ.Position.Z());
  h_p0doutermosthits_zmax_dpos_x   ->Fill(P0DOutermostHits->hitMaxZ.PositionError.X());
  h_p0doutermosthits_zmax_dpos_y   ->Fill(P0DOutermostHits->hitMaxZ.PositionError.Y());
  h_p0doutermosthits_zmax_dpos_z   ->Fill(P0DOutermostHits->hitMaxZ.PositionError.Z());
  h_p0doutermosthits_zmax_type     ->Fill(P0DOutermostHits->hitMaxZ.Type);

  h_p0doutermosthits_zmin_charge   ->Fill(P0DOutermostHits->hitMinZ.Charge);
  h_p0doutermosthits_zmin_time     ->Fill(P0DOutermostHits->hitMinZ.Time);
  h_p0doutermosthits_zmin_pos_x    ->Fill(P0DOutermostHits->hitMinZ.Position.X());
  h_p0doutermosthits_zmin_pos_y    ->Fill(P0DOutermostHits->hitMinZ.Position.Y());
  h_p0doutermosthits_zmin_pos_z    ->Fill(P0DOutermostHits->hitMinZ.Position.Z());
  h_p0doutermosthits_zmin_dpos_x   ->Fill(P0DOutermostHits->hitMinZ.PositionError.X());
  h_p0doutermosthits_zmin_dpos_y   ->Fill(P0DOutermostHits->hitMinZ.PositionError.Y());
  h_p0doutermosthits_zmin_dpos_z   ->Fill(P0DOutermostHits->hitMinZ.PositionError.Z());
  h_p0doutermosthits_zmin_type     ->Fill(P0DOutermostHits->hitMinZ.Type);

	h_p0doutermosthits_xmax_pos_y    ->SetBit(TH1::kCanRebin);
	h_p0doutermosthits_xmin_pos_y    ->SetBit(TH1::kCanRebin);

//---------------

       for(int i=0;i <NVertices; i++){
            curr_vert = (ND::TGlobalReconModule::TGlobalVertex *) Vertices->At(i);
            h_vert_primaryindex->Fill(curr_vert->PrimaryIndex);
	    h_vert_status->Fill(curr_vert->Status);
	    h_vert_quality->Fill(curr_vert->Quality);
	    h_vert_ndof->Fill(curr_vert->NDOF);
            h_vert_ntruevertices->Fill(curr_vert->NTrueVertices);
	    h_vert_nconstituents->Fill(curr_vert->NConstituents);
            h_vert_pos_x->Fill(curr_vert->Position.X());
            h_vert_pos_y->Fill(curr_vert->Position.Y());
            h_vert_pos_z->Fill(curr_vert->Position.Z());
            h_vert_pos_t->Fill(curr_vert->Position.T());
            h_vert_pos_px->Fill(curr_vert->Position.Px());
            h_vert_pos_py->Fill(curr_vert->Position.Py());
            h_vert_pos_pz->Fill(curr_vert->Position.Pz());
            h_vert_pos_energy->Fill(curr_vert->Position.Energy());
            h_vert_pos_theta->Fill(curr_vert->Position.Theta());
            h_vert_pos_costheta->Fill(curr_vert->Position.CosTheta());
            h_vert_pos_phi->Fill(curr_vert->Position.Phi());
            h_vert_pos_rho->Fill(curr_vert->Position.Rho());
            h_vert_pos_perp2->Fill(curr_vert->Position.Perp2());
            h_vert_pos_pt->Fill(curr_vert->Position.Pt());
            h_vert_pos_perp->Fill(curr_vert->Position.Perp());
            h_vert_pos_et2->Fill(curr_vert->Position.Et2());
     }

     for(int i=0;i<NPIDs;i++){
         curr_pid = (ND::TGlobalReconModule::TGlobalPID *) PIDs->At(i);
         fill=0;
         if (selection == 1) {//electrons
	   if (curr_pid->ParticleIds.size() == 1 && curr_pid->ParticleIds[0] == 11) fill=1;
	 }
         else if (selection == 2) {//muons
	   if (curr_pid->ParticleIds.size() == 1 && curr_pid->ParticleIds[0] == 13) fill=1;
	 }
         else{
           fill=1;
	 }
	 if (fill==1){
	    h_pid_uid->Fill(curr_pid->UniqueID);
	    //	    cout << curr_pid->AlgorithmName << endl;
	    // cout << curr_pid->ParticleIds.size() << endl;
	    for (unsigned int jj=0; jj < curr_pid->BrokenUniqueIDs.size(); jj++ ){
	        h_pid_buid->Fill(curr_pid->BrokenUniqueIDs[jj]);
            }
            h_pid_det->Fill(curr_pid->Detectors);
	    for (int j=0; j < 19; j++ ){
	      h_pid_detu->Fill(curr_pid->DetectorUsed[j]);
	      //              if (curr_pid->DetectorUsed[j] > 2) cout << curr_pid->DetectorUsed[j] << endl;
	    }
	    h_pid_status->Fill(curr_pid->Status);
	    h_pid_ndof->Fill(curr_pid->NDOF);
            h_pid_chi2->Fill(curr_pid->Chi2);
	    h_pid_nnodes->Fill(curr_pid->NNodes);
	    h_pid_nhits->Fill(curr_pid->NHits);
	    h_pid_nconstituents->Fill(curr_pid->NConstituents);
            h_pid_isf->Fill(curr_pid->isForward);
            h_pid_sok->Fill(curr_pid->SenseOK);
            h_pid_charge->Fill(curr_pid->Charge);

            h_pid_edep->Fill(curr_pid->EDeposit);

	    h_pid_conex->Fill(curr_pid->Cone.x());
            h_pid_coney->Fill(curr_pid->Cone.y());
            h_pid_conez->Fill(curr_pid->Cone.z());

            h_pid_width->Fill(curr_pid->Width);
            h_pid_length->Fill(curr_pid->Length);
            for (unsigned int jj=0; jj < curr_pid->ParticleIds.size(); jj++ ){
                h_pid_partid->Fill(curr_pid->ParticleIds[jj]);
	    }
	    for (unsigned int jj=0; jj < curr_pid->PIDWeights.size(); jj++ ){
                h_pid_pidw->Fill(curr_pid->PIDWeights[jj]);
	    }
            h_pid_fpx->Fill(curr_pid->FrontPosition.X());
            h_pid_fpy->Fill(curr_pid->FrontPosition.Y());
            h_pid_fpz->Fill(curr_pid->FrontPosition.Z());
            h_pid_fpt->Fill(curr_pid->FrontPosition.T());

            h_pid_bpx->Fill(curr_pid->BackPosition.X());
            h_pid_bpy->Fill(curr_pid->BackPosition.Y());
            h_pid_bpz->Fill(curr_pid->BackPosition.Z());
            h_pid_bpt->Fill(curr_pid->BackPosition.T());

            h_pid_fpvx->Fill(curr_pid->FrontPositionVar.X());
            h_pid_fpvy->Fill(curr_pid->FrontPositionVar.Y());
            h_pid_fpvz->Fill(curr_pid->FrontPositionVar.Z());
            h_pid_fpvt->Fill(curr_pid->FrontPositionVar.T());

            h_pid_bpvx->Fill(curr_pid->BackPositionVar.X());
            h_pid_bpvy->Fill(curr_pid->BackPositionVar.Y());
            h_pid_bpvz->Fill(curr_pid->BackPositionVar.Z());
            h_pid_bpvt->Fill(curr_pid->BackPositionVar.T());

            h_pid_fdx->Fill(curr_pid->FrontDirection.X());
            h_pid_fdy->Fill(curr_pid->FrontDirection.Y());
            h_pid_fdz->Fill(curr_pid->FrontDirection.Z());

            h_pid_bdx->Fill(curr_pid->BackDirection.X());
            h_pid_bdy->Fill(curr_pid->BackDirection.Y());
            h_pid_bdz->Fill(curr_pid->BackDirection.Z());

            h_pid_fdvx->Fill(curr_pid->FrontDirectionVar.X());
            h_pid_fdvy->Fill(curr_pid->FrontDirectionVar.Y());
            h_pid_fdvz->Fill(curr_pid->FrontDirectionVar.Z());

            h_pid_bdvx->Fill(curr_pid->BackDirectionVar.X());
            h_pid_bdvy->Fill(curr_pid->BackDirectionVar.Y());
            h_pid_bdvz->Fill(curr_pid->BackDirectionVar.Z());

            h_pid_fm->Fill(curr_pid->FrontMomentum);
            h_pid_bm->Fill(curr_pid->BackMomentum);
            h_pid_fme->Fill(curr_pid->FrontMomentumError);
            h_pid_bme->Fill(curr_pid->BackMomentumError);

            h_pid_patvx->Fill(curr_pid->PositionAtTrueVertex.X());
            h_pid_patvy->Fill(curr_pid->PositionAtTrueVertex.Y());
            h_pid_patvz->Fill(curr_pid->PositionAtTrueVertex.Z());
            h_pid_patvt->Fill(curr_pid->PositionAtTrueVertex.T());

            h_pid_pvatvx->Fill(curr_pid->PositionVarAtTrueVertex.X());
            h_pid_pvatvy->Fill(curr_pid->PositionVarAtTrueVertex.Y());
            h_pid_pvatvz->Fill(curr_pid->PositionVarAtTrueVertex.Z());
            h_pid_pvatvt->Fill(curr_pid->PositionVarAtTrueVertex.T());

            h_pid_datvx->Fill(curr_pid->DirectionAtTrueVertex.x());
            h_pid_datvy->Fill(curr_pid->DirectionAtTrueVertex.y());
            h_pid_datvz->Fill(curr_pid->DirectionAtTrueVertex.z());

            h_pid_dvatvx->Fill(curr_pid->DirectionVarAtTrueVertex.x());
            h_pid_dvatvy->Fill(curr_pid->DirectionVarAtTrueVertex.y());
            h_pid_dvatvz->Fill(curr_pid->DirectionVarAtTrueVertex.z());

            h_pid_matv->Fill(curr_pid->MomentumAtTrueVertex);
            h_pid_meatv->Fill(curr_pid->MomentumErrorAtTrueVertex);

            for (int j=0; j < 25; j++ ){
               h_pid_enok->Fill(curr_pid->EntranceOK[j]);
               h_pid_exok->Fill(curr_pid->ExitOK[j]);
               h_pid_enpx->Fill(curr_pid->EntrancePosition[j].X());
               h_pid_enpy->Fill(curr_pid->EntrancePosition[j].Y());
               h_pid_enpz->Fill(curr_pid->EntrancePosition[j].Z());
               h_pid_enpt->Fill(curr_pid->EntrancePosition[j].T());
               h_pid_expx->Fill(curr_pid->ExitPosition[j].X());
               h_pid_expy->Fill(curr_pid->ExitPosition[j].Y());
               h_pid_expz->Fill(curr_pid->ExitPosition[j].Z());
               h_pid_expt->Fill(curr_pid->ExitPosition[j].T());
               h_pid_endx->Fill(curr_pid->EntranceDirection[j].x());
               h_pid_endy->Fill(curr_pid->EntranceDirection[j].y());
               h_pid_endz->Fill(curr_pid->EntranceDirection[j].z());
               h_pid_exdx->Fill(curr_pid->ExitDirection[j].x());
               h_pid_exdy->Fill(curr_pid->ExitDirection[j].y());
               h_pid_exdz->Fill(curr_pid->ExitDirection[j].z());
               h_pid_enmom->Fill(curr_pid->EntranceMomentum[j]);
               h_pid_exmom->Fill(curr_pid->ExitMomentum[j]);
               h_pid_enpex->Fill(curr_pid->EntrancePositionErr[j].X());
               h_pid_enpey->Fill(curr_pid->EntrancePositionErr[j].Y());
               h_pid_enpez->Fill(curr_pid->EntrancePositionErr[j].Z());
               h_pid_enpet->Fill(curr_pid->EntrancePositionErr[j].T());
               h_pid_expex->Fill(curr_pid->ExitPositionErr[j].X());
               h_pid_expey->Fill(curr_pid->ExitPositionErr[j].Y());
               h_pid_expez->Fill(curr_pid->ExitPositionErr[j].Z());
               h_pid_expet->Fill(curr_pid->ExitPositionErr[j].T());
               h_pid_endex->Fill(curr_pid->EntranceDirectionErr[j].x());
               h_pid_endey->Fill(curr_pid->EntranceDirectionErr[j].y());
               h_pid_endez->Fill(curr_pid->EntranceDirectionErr[j].z());
               h_pid_exdex->Fill(curr_pid->ExitDirectionErr[j].x());
               h_pid_exdey->Fill(curr_pid->ExitDirectionErr[j].y());
               h_pid_exdez->Fill(curr_pid->ExitDirectionErr[j].z());
               h_pid_enmome->Fill(curr_pid->EntranceMomentumErr[j]);
               h_pid_exmome->Fill(curr_pid->ExitMomentumErr[j]);
	    }
	    //HitsSaved
            h_pid_nhitss->Fill(curr_pid->NHitsSaved);
            for (int j= 0; j < curr_pid->NHitsSaved; j++){
	       ND::TGlobalReconModule::TGlobalHit* glbhit_obj = (ND::TGlobalReconModule::TGlobalHit*) curr_pid->HitsSaved->At(j);
               h_pid_hs_ch->Fill(glbhit_obj->Charge);
               h_pid_hs_t->Fill(glbhit_obj->Time);
               h_pid_hs_px->Fill(glbhit_obj->Position.x());
               h_pid_hs_py->Fill(glbhit_obj->Position.y());
               h_pid_hs_pz->Fill(glbhit_obj->Position.z());
               h_pid_hs_pex->Fill(glbhit_obj->PositionError.x());
               h_pid_hs_pey->Fill(glbhit_obj->PositionError.y());
               h_pid_hs_pez->Fill(glbhit_obj->PositionError.z());
               h_pid_hs_type->Fill(glbhit_obj->Type);
	    }
	    //TrueParticle
            h_pid_tp_id->Fill(curr_pid->TrueParticle.ID);
            h_pid_tp_eff->Fill(curr_pid->TrueParticle.Eff);
            h_pid_tp_pur->Fill(curr_pid->TrueParticle.Pur);
            h_pid_tp_vx_eff->Fill(curr_pid->TrueParticle.Vertex.Eff);
            h_pid_tp_vx_pur->Fill(curr_pid->TrueParticle.Vertex.Pur);
	 }

	    //Alternates
            h_pid_nalt->Fill(curr_pid->NAlternates);
 	    for (int j = 0; j < curr_pid->NAlternates; ++j){
	      ND::TGlobalReconModule::TGlobalPIDAlternate* alt_obj = (ND::TGlobalReconModule::TGlobalPIDAlternate*) curr_pid->Alternates->At(j);
              fill_pidalt=0;
              if (selection == 3) {
	         if (alt_obj->ParticleId == 11) fill_pidalt=1;
	      }
              else if (selection == 4) {
	         if (alt_obj->ParticleId == 13) fill_pidalt=1;
	      }
              else if (selection == 5){
                 if (alt_obj->ParticleId == 2212) fill_pidalt=1;
	      }
              else fill_pidalt=1;
              if (fill_pidalt==1){
               h_pidalt_det->Fill(alt_obj->Detectors);
	       for (int k=0; k < 19; k++ ){
	         h_pidalt_detu->Fill(alt_obj->DetectorUsed[k]);
		 // if (alt_obj->DetectorUsed[k] > 2) cout << alt_obj->DetectorUsed[k] << endl;
	       }
	       h_pidalt_status->Fill(alt_obj->Status);
	       h_pidalt_ndof->Fill(alt_obj->NDOF);
               h_pidalt_chi2->Fill(alt_obj->Chi2);

               h_pidalt_charge->Fill(alt_obj->Charge);

               h_pidalt_length->Fill(alt_obj->Length);
               //cout << alt_obj->ParticleId << endl;
               h_pidalt_partid->Fill(alt_obj->ParticleId);
               h_pidalt_pidw->Fill(alt_obj->PIDWeight);

	       h_pidalt_fpx->Fill(alt_obj->FrontPosition.X());
               h_pidalt_fpy->Fill(alt_obj->FrontPosition.Y());
               h_pidalt_fpz->Fill(alt_obj->FrontPosition.Z());
               h_pidalt_fpt->Fill(alt_obj->FrontPosition.T());

               h_pidalt_bpx->Fill(alt_obj->BackPosition.X());
               h_pidalt_bpy->Fill(alt_obj->BackPosition.Y());
               h_pidalt_bpz->Fill(alt_obj->BackPosition.Z());
               h_pidalt_bpt->Fill(alt_obj->BackPosition.T());

               h_pidalt_fpvx->Fill(alt_obj->FrontPositionVar.X());
               h_pidalt_fpvy->Fill(alt_obj->FrontPositionVar.Y());
               h_pidalt_fpvz->Fill(alt_obj->FrontPositionVar.Z());
               h_pidalt_fpvt->Fill(alt_obj->FrontPositionVar.T());
               //cout << alt_obj->PositionVarAtTrueVertex.T() << endl;
               h_pidalt_bpvx->Fill(alt_obj->BackPositionVar.X());
               h_pidalt_bpvy->Fill(alt_obj->BackPositionVar.Y());
               h_pidalt_bpvz->Fill(alt_obj->BackPositionVar.Z());
               h_pidalt_bpvt->Fill(alt_obj->BackPositionVar.T());

               h_pidalt_fdx->Fill(alt_obj->FrontDirection.X());
               h_pidalt_fdy->Fill(alt_obj->FrontDirection.Y());
               h_pidalt_fdz->Fill(alt_obj->FrontDirection.Z());

               h_pidalt_bdx->Fill(alt_obj->BackDirection.X());
               h_pidalt_bdy->Fill(alt_obj->BackDirection.Y());
               h_pidalt_bdz->Fill(alt_obj->BackDirection.Z());

               h_pidalt_fdvx->Fill(alt_obj->FrontDirectionVar.X());
               h_pidalt_fdvy->Fill(alt_obj->FrontDirectionVar.Y());
               h_pidalt_fdvz->Fill(alt_obj->FrontDirectionVar.Z());

               h_pidalt_bdvx->Fill(alt_obj->BackDirectionVar.X());
               h_pidalt_bdvy->Fill(alt_obj->BackDirectionVar.Y());
               h_pidalt_bdvz->Fill(alt_obj->BackDirectionVar.Z());

               h_pidalt_fm->Fill(alt_obj->FrontMomentum);
               h_pidalt_bm->Fill(alt_obj->BackMomentum);
               h_pidalt_fme->Fill(alt_obj->FrontMomentumError);
               h_pidalt_bme->Fill(alt_obj->BackMomentumError);

               h_pidalt_patvx->Fill(alt_obj->PositionAtTrueVertex.X());
               h_pidalt_patvy->Fill(alt_obj->PositionAtTrueVertex.Y());
               h_pidalt_patvz->Fill(alt_obj->PositionAtTrueVertex.Z());
               h_pidalt_patvt->Fill(alt_obj->PositionAtTrueVertex.T());

               h_pidalt_pvatvx->Fill(alt_obj->PositionVarAtTrueVertex.X());
               h_pidalt_pvatvy->Fill(alt_obj->PositionVarAtTrueVertex.Y());
               h_pidalt_pvatvz->Fill(alt_obj->PositionVarAtTrueVertex.Z());
               h_pidalt_pvatvt->Fill(alt_obj->PositionVarAtTrueVertex.T());

               h_pidalt_datvx->Fill(alt_obj->DirectionAtTrueVertex.x());
               h_pidalt_datvy->Fill(alt_obj->DirectionAtTrueVertex.y());
               h_pidalt_datvz->Fill(alt_obj->DirectionAtTrueVertex.z());

               h_pidalt_dvatvx->Fill(alt_obj->DirectionVarAtTrueVertex.x());
               h_pidalt_dvatvy->Fill(alt_obj->DirectionVarAtTrueVertex.y());
               h_pidalt_dvatvz->Fill(alt_obj->DirectionVarAtTrueVertex.z());

               h_pidalt_matv->Fill(alt_obj->MomentumAtTrueVertex);
               h_pidalt_meatv->Fill(alt_obj->MomentumErrorAtTrueVertex);
	      }
	    }

	 h_event->Fill(EventID);
         h_reconame->Fill(0.);
	 h_nconst->Fill(curr_pid->NConstituents);
         h_detectors->Fill(curr_pid->Detectors);
         h_purity->Fill(curr_pid->TrueParticle.Pur);
         h_eff->Fill(curr_pid->TrueParticle.Eff);
         h_fitchi->Fill(curr_pid->Chi2);
         h_fitndof->Fill(curr_pid->NDOF);
	 h_fitstatus->Fill(curr_pid->Status);
         h_nhits->Fill(curr_pid->NHits);
         h_type->Fill(curr_pid->Status);
         h_partid->Fill(curr_pid->TrueParticle.ID);

	 //tracks
         h_chomom->Fill(curr_pid->Charge/curr_pid->FrontMomentum);
         h_chopt->Fill(curr_pid->Charge/curr_pid->DirectionAtTrueVertex.Pt());
         h_sense->Fill(curr_pid->isForward);
         h_pos1->Fill(curr_pid->FrontPosition.X());
         h_pos2->Fill(curr_pid->FrontPosition.Y());
         h_pos3->Fill(curr_pid->FrontPosition.Z());
         h_posvar1->Fill(curr_pid->FrontPositionVar.X());
	 h_posvar2->Fill(curr_pid->FrontPositionVar.Y());
         h_posvar3->Fill(curr_pid->FrontPositionVar.Z());
         h_dir1->Fill(curr_pid->FrontDirection.x());
         h_dir2->Fill(curr_pid->FrontDirection.y());
         h_dirvar1->Fill(curr_pid->FrontDirectionVar.x());
         h_dirvar2->Fill(curr_pid->FrontDirectionVar.y());

	 //truth tracks
         h_tpartid->Fill(curr_pid->TrueParticle.ID);
         h_tpartpdg->Fill(curr_pid->TrueParticle.ID);
         h_tdetectors->Fill(curr_pid->Detectors);
         h_tchomom->Fill(curr_pid->Charge);
         h_tchopt->Fill(curr_pid->Charge);
         if (curr_pid->TrueParticle.Vertex.Position.Z() > 0){
            h_tsense->Fill(1.);
         }
         else h_tsense->Fill(-1.);
         h_tpos1->Fill(curr_pid->TrueParticle.Vertex.Position.X());
         h_tpos2->Fill(curr_pid->TrueParticle.Vertex.Position.Y());
         h_tpos3->Fill(curr_pid->TrueParticle.Vertex.Position.Z());

         //General Node information

         h_annodes->Fill(curr_pid->NNodes);
         h_nnodes->Fill(curr_pid->NNodes);

         //TPC OBJECT
         for (int j = 0; j < curr_pid->NTPCs; ++j)
           {
	     ND::TGlobalReconModule::TTPCObject* tpc_obj = (ND::TGlobalReconModule::TTPCObject*) curr_pid->TPC->At(j);
             for (int jj=0;jj<tpc_obj->T0Source.size(); jj++){
               h_tpc_t0source->Fill(tpc_obj->T0Source[jj]);
             }
             for (int jj=0;jj<tpc_obj->NHorRows.size(); jj++){
               h_tpc_nhorrows->Fill(tpc_obj->NHorRows[jj]);
             }
             for (int jj=0;jj<tpc_obj->NVerRows.size(); jj++){
               h_tpc_nverrows->Fill(tpc_obj->NVerRows[jj]);
             }
             h_tpc_nnodes->Fill(tpc_obj->NNodes);
             h_tpc_tpclength->Fill(tpc_obj->TPCLength);
             h_tpc_kalmanfittype->Fill(tpc_obj->KalmanFitType);
             h_tpc_pdist->Fill(tpc_obj->PDist);
             h_tpc_pefield->Fill(tpc_obj->PEField);
	     h_tpc_charge->Fill(tpc_obj->Charge);
	     h_tpc_ntrun->Fill(tpc_obj->NTrun);
	     h_tpc_ccorr->Fill(tpc_obj->Ccorr);
	     h_tpc_pullele->Fill(tpc_obj->PullEle);
	     h_tpc_pullmuon->Fill(tpc_obj->PullMuon);
	     h_tpc_pullpion->Fill(tpc_obj->PullPion);
	     h_tpc_pullkaon->Fill(tpc_obj->PullKaon);
	     h_tpc_pullproton->Fill(tpc_obj->PullProton);
	     h_tpc_dedxexpele->Fill(tpc_obj->dEdxexpEle);
	     h_tpc_dedxexpmuon->Fill(tpc_obj->dEdxexpMuon);
	     h_tpc_dedxexppion->Fill(tpc_obj->dEdxexpPion);
	     h_tpc_dedxexpkaon->Fill(tpc_obj->dEdxexpKaon);
	     h_tpc_dedxexpproton->Fill(tpc_obj->dEdxexpProton);
	     h_tpc_sigmaele->Fill(tpc_obj->SigmaEle);
	     h_tpc_sigmamuon->Fill(tpc_obj->SigmaMuon);
	     h_tpc_sigmapion->Fill(tpc_obj->SigmaPion);
	     h_tpc_sigmakaon->Fill(tpc_obj->SigmaKaon);
	     h_tpc_sigmaproton->Fill(tpc_obj->SigmaProton);
	     h_tpc_nhits->Fill(tpc_obj->NHits);
	     h_tpc_fpost->Fill(tpc_obj->FrontPosition.T());
	     h_tpc_fposx->Fill(tpc_obj->FrontPosition.X());
	     h_tpc_fposy->Fill(tpc_obj->FrontPosition.Y());
	     h_tpc_fposz->Fill(tpc_obj->FrontPosition.Z());
	     h_tpc_fpos_vart->Fill(tpc_obj->FrontPositionVar.T());
	     h_tpc_fpos_varx->Fill(tpc_obj->FrontPositionVar.X());
	     h_tpc_fpos_vary->Fill(tpc_obj->FrontPositionVar.Y());
	     h_tpc_fpos_varz->Fill(tpc_obj->FrontPositionVar.Z());
	     h_tpc_bpost->Fill(tpc_obj->BackPosition.T());
	     h_tpc_bposx->Fill(tpc_obj->BackPosition.X());
	     h_tpc_bposy->Fill(tpc_obj->BackPosition.Y());
	     h_tpc_bposz->Fill(tpc_obj->BackPosition.Z());
	     h_tpc_bpos_vart->Fill(tpc_obj->BackPositionVar.T());
	     h_tpc_bpos_varx->Fill(tpc_obj->BackPositionVar.X());
	     h_tpc_bpos_vary->Fill(tpc_obj->BackPositionVar.Y());
	     h_tpc_bpos_varz->Fill(tpc_obj->BackPositionVar.Z());
	     //              h_tpc_conex->Fill(tpc_obj->Cone.X());
	     //h_tpc_coney->Fill(tpc_obj->Cone.Y());
	     //h_tpc_conez->Fill(tpc_obj->Cone.Z());
	     h_tpc_fdirx->Fill(tpc_obj->FrontDirection.X());
	     h_tpc_fdiry->Fill(tpc_obj->FrontDirection.Y());
	     h_tpc_fdirz->Fill(tpc_obj->FrontDirection.Z());
	     h_tpc_fdir_varx->Fill(tpc_obj->FrontDirectionVar.X());
	     h_tpc_fdir_vary->Fill(tpc_obj->FrontDirectionVar.Y());
	     h_tpc_fdir_varz->Fill(tpc_obj->FrontDirectionVar.Z());
	     h_tpc_bdirx->Fill(tpc_obj->BackDirection.X());
	     h_tpc_bdiry->Fill(tpc_obj->BackDirection.Y());
	     h_tpc_bdirz->Fill(tpc_obj->BackDirection.Z());
	     h_tpc_bdir_varx->Fill(tpc_obj->BackDirectionVar.X());
	     h_tpc_bdir_varx_full->Fill(tpc_obj->BackDirectionVar.X());
	     h_tpc_bdir_vary->Fill(tpc_obj->BackDirectionVar.Y());
	     h_tpc_bdir_varz->Fill(tpc_obj->BackDirectionVar.Z());
	     h_tpc_UniqueID                   ->Fill(tpc_obj->UniqueID          );
	     h_tpc_Status                     ->Fill(tpc_obj->Status            );
	     h_tpc_Detector                   ->Fill(tpc_obj->Detector          );
	     h_tpc_NNodes                     ->Fill(tpc_obj->NNodes            );
	     h_tpc_NDOF                       ->Fill(tpc_obj->NDOF              );
	     h_tpc_Chi2                       ->Fill(tpc_obj->Chi2              );
	     h_tpc_EDeposit                   ->Fill(tpc_obj->EDeposit          );
	     h_tpc_NConstituents              ->Fill(tpc_obj->NConstituents     );
	     h_tpc_Length                     ->Fill(tpc_obj->Length            );
	     h_tpc_FrontMomentum              ->Fill(tpc_obj->FrontMomentum     );
	     h_tpc_BackMomentum               ->Fill(tpc_obj->BackMomentum      );
	     h_tpc_FrontMomentumError         ->Fill(tpc_obj->FrontMomentumError);
	     h_tpc_BackMomentumError         ->Fill(tpc_obj->BackMomentumError);

	     h_tpc_TrueParticleID         ->Fill(tpc_obj->TrueParticle.ID);
	     h_tpc_TrueParticlePur         ->Fill(tpc_obj->TrueParticle.Pur);
	     h_tpc_TrueParticleEff         ->Fill(tpc_obj->TrueParticle.Eff);

	     h_tpc_TrueParticleVertexPositionX ->Fill(tpc_obj->TrueParticle.Vertex.Position.X());
	     h_tpc_TrueParticleVertexPositionY ->Fill(tpc_obj->TrueParticle.Vertex.Position.Y());
	     h_tpc_TrueParticleVertexPositionZ ->Fill(tpc_obj->TrueParticle.Vertex.Position.Z());
	     h_tpc_TrueParticleVertexPositionT ->Fill(tpc_obj->TrueParticle.Vertex.Position.T());
	     h_tpc_TrueParticleVertexID   ->Fill(tpc_obj->TrueParticle.Vertex.ID);
           }

	 //FGD OBJECT

	 h_nfgds->Fill(curr_pid->NFGDs);
	 for (int j = 0; j < curr_pid->NFGDs;++j)
	   {
	     ND::TGlobalReconModule::TFGDObject* fgd_obj = (ND::TGlobalReconModule::TFGDObject*) curr_pid->FGD->At(j);

	     h_fgd_avgtime->Fill(fgd_obj->                            avgtime);
	     for (int k=0;k<30;k++){
	       h_fgd_chargePerLayer->Fill(fgd_obj->                     chargePerLayer[k]);
	       h_fgd_chargePerLayerAttenCorr->Fill(fgd_obj->            chargePerLayerAttenCorr[k]);
	     }

	     h_fgd_fgdContainment->Fill(fgd_obj->        fgdContainment);
	     h_fgd_E->Fill(fgd_obj->                     E);
	     h_fgd_x->Fill(fgd_obj->                     x);
	     h_fgd_E_exp_muon->Fill(fgd_obj->            E_exp_muon);
	     h_fgd_E_exp_pion->Fill(fgd_obj->            E_exp_pion);
	     h_fgd_E_exp_proton->Fill(fgd_obj->          E_exp_proton);
	     h_fgd_sigmaE_muon->Fill(fgd_obj->           sigmaE_muon);
	     h_fgd_sigmaE_pion->Fill(fgd_obj->           sigmaE_pion);
	     h_fgd_sigmaE_proton->Fill(fgd_obj->         sigmaE_proton);
	     h_fgd_PullMuon->Fill(fgd_obj->              PullMuon);
	     h_fgd_PullMuon_small->Fill(fgd_obj->              PullMuon);
	     h_fgd_PullPion->Fill(fgd_obj->              PullPion);
	     h_fgd_PullPion_small->Fill(fgd_obj->              PullPion);
	     h_fgd_PullProton->Fill(fgd_obj->            PullProton);
	     h_fgd_PullProton_small->Fill(fgd_obj->            PullProton);
	     h_fgd_PullNotSet->Fill(fgd_obj->            PullNotSet);
	     h_fgd_PullNotSet_small->Fill(fgd_obj->            PullNotSet);
	     h_fgd_isFgdVA->Fill(fgd_obj->               isFgdVA);
	     h_fgd_isFgdVA_flag->Fill(fgd_obj->          isFgdVA_flag);
	     h_fgd_fgdVA_upMinZ->Fill(fgd_obj->          fgdVA_upMinZ);
	     h_fgd_fgdVA_downMaxZ->Fill(fgd_obj->        fgdVA_downMaxZ);
	     h_fgd_fgdVA_upCosTheta->Fill(fgd_obj->      fgdVA_upCosTheta);
	     h_fgd_fgdVA_downCosTheta->Fill(fgd_obj->    fgdVA_downCosTheta);
	     h_fgd_fgdVA_otherUpQ->Fill(fgd_obj->        fgdVA_otherUpQ);
	     h_fgd_fgdVA_otherDownQ->Fill(fgd_obj->      fgdVA_otherDownQ);
	     h_fgd_fgdVA_verQ->Fill(fgd_obj->            fgdVA_verQ);
	     h_fgd_fgdVA_verLayQ->Fill(fgd_obj->         fgdVA_verLayQ);
	     h_fgd_fgdVA_verNearQ->Fill(fgd_obj->        fgdVA_verNearQ);
	     h_fgd_fgdVA_verNextNearQ->Fill(fgd_obj->    fgdVA_verNextNearQ);
	     h_fgd_fgdVA_verNextNextNearQ->Fill(fgd_obj->fgdVA_verNextNextNearQ);
	     h_fgd_fgdVA_totalQ->Fill(fgd_obj->          fgdVA_totalQ);
	     h_fgd_fgdVA_totalCorrE->Fill(fgd_obj->      fgdVA_totalCorrE);
       
       
       h_fgd_fgdVA_verNearQ_rect->Fill(fgd_obj->        fgdVA_verNearQ_rect);
       h_fgd_fgdVA_verNextNearQ_rect->Fill(fgd_obj->    fgdVA_verNextNearQ_rect);
       h_fgd_fgdVA_verNextNextNearQ_rect->Fill(fgd_obj->fgdVA_verNextNextNearQ_rect);
       
	     h_fgd_fgdEA_verNearQ_rect->Fill(fgd_obj->        fgdEA_verNearQ_rect);         
       h_fgd_fgdEA_verNextNearQ_rect->Fill(fgd_obj->    fgdEA_verNextNearQ_rect);     
       h_fgd_fgdEA_verNextNextNearQ_rect->Fill(fgd_obj->fgdEA_verNextNextNearQ_rect);
       
       //FGD OBJECT HISTOGRAMS THAT HAVE AXES AUTOMATICALLY SCALED
	     h_fgd_avgtime                ->SetBit(TH1::kCanRebin);
	     h_fgd_PullMuon               ->SetBit(TH1::kCanRebin);
	     h_fgd_PullPion               ->SetBit(TH1::kCanRebin);
	     h_fgd_PullProton             ->SetBit(TH1::kCanRebin);
	     h_fgd_PullNotSet             ->SetBit(TH1::kCanRebin);
	     h_fgd_fgdVA_upMinZ           ->SetBit(TH1::kCanRebin);
	     h_fgd_fgdVA_downMaxZ         ->SetBit(TH1::kCanRebin);


	   }

	 //ECAL OBJECT

	 h_necals->Fill(curr_pid->NECALs);
	 for (int j = 0; j < curr_pid->NECALs; ++j)
	   {
	     ND::TGlobalReconModule::TECALObject* ecal_obj = (ND::TGlobalReconModule::TECALObject*) curr_pid->ECAL->At(j);
             fill_ecal=0;
	     if (selection==6){
	       if(ecal_obj->IsTrackLike==1) fill_ecal=1;
	     }
             else if (selection==7){
               if(ecal_obj->IsShowerLike==1) fill_ecal=1;
	     }
             else fill_ecal=1;
             if (fill_ecal==1){
	       h_ecals_nhits->Fill(ecal_obj->NHits);
	       h_ecals_trshval->Fill(ecal_obj->TrShVal);
	       h_ecals_emhadval->Fill(ecal_obj->EMHadVal);
	       h_ecals_widthx->Fill(ecal_obj->Width.X());
	       h_ecals_widthy->Fill(ecal_obj->Width.Y());
	       h_ecals_widthz->Fill(ecal_obj->Width.Z());
	       h_ecals_fpost->Fill(ecal_obj->FrontPosition.T());
	       h_ecals_fposx->Fill(ecal_obj->FrontPosition.X());
	       h_ecals_fposy->Fill(ecal_obj->FrontPosition.Y());
	       h_ecals_fposz->Fill(ecal_obj->FrontPosition.Z());
	       h_ecals_fpos_vart->Fill(ecal_obj->FrontPositionVar.T());
	       h_ecals_fpos_varx->Fill(ecal_obj->FrontPositionVar.X());
	       h_ecals_fpos_vary->Fill(ecal_obj->FrontPositionVar.Y());
	       h_ecals_fpos_varz->Fill(ecal_obj->FrontPositionVar.Z());
	       h_ecals_bpost->Fill(ecal_obj->BackPosition.T());
	       h_ecals_bposx->Fill(ecal_obj->BackPosition.X());
	       h_ecals_bposy->Fill(ecal_obj->BackPosition.Y());
	       h_ecals_bposz->Fill(ecal_obj->BackPosition.Z());
	       h_ecals_bpos_vart->Fill(ecal_obj->BackPositionVar.T());
	       h_ecals_bpos_varx->Fill(ecal_obj->BackPositionVar.X());
	       h_ecals_bpos_vary->Fill(ecal_obj->BackPositionVar.Y());
	       h_ecals_bpos_varz->Fill(ecal_obj->BackPositionVar.Z());
	       h_ecals_emenergy->Fill(ecal_obj->EMEnergy);
	       h_ecals_emenergyerror->Fill(ecal_obj->EMEnergyError);
	       h_ecals_averagehittime->Fill(ecal_obj->AverageHitTime);
	       h_ecals_averagehittime_full->Fill(ecal_obj->AverageHitTime);
	       h_ecals_kfparameter->Fill(ecal_obj->KFParameter);
	       h_ecals_kfnnodes->Fill(ecal_obj->KFNNodes);
	       h_ecals_kfparameternodes->Fill(ecal_obj->KFParameterNodes);
	       h_ecals_kfquality->Fill(ecal_obj->KFQuality);
	       h_ecals_kfndof->Fill(ecal_obj->KFNDOF);
	       h_ecals_kfhitquality->Fill(ecal_obj->KFHitQuality);
	       h_ecals_melectrontag_ndelayedcluster->Fill(ecal_obj->MElectronTag_NDelayedCluster);

	       for (unsigned int jj=0;jj<ecal_obj->MElectronTag_NDelayedHits.size(); jj++){
		 h_ecals_melectrontag_ndelayedhits->Fill(ecal_obj->MElectronTag_NDelayedHits[jj]);
	       }
	       for (unsigned int jj=0;jj<ecal_obj->MElectronTag_DeltaX.size(); jj++){
		 h_ecals_melectrontag_deltax->Fill(ecal_obj->MElectronTag_DeltaX[jj]);
	       }
	       for (unsigned int jj=0;jj<ecal_obj->MElectronTag_DeltaY.size(); jj++){
		 h_ecals_melectrontag_deltay->Fill(ecal_obj->MElectronTag_DeltaY[jj]);
	       }
	       for (unsigned int jj=0;jj<ecal_obj->MElectronTag_DeltaZ.size(); jj++){
		 h_ecals_melectrontag_deltaz->Fill(ecal_obj->MElectronTag_DeltaZ[jj]);
	       }
	       for (unsigned int jj=0;jj<ecal_obj->MElectronTag_DeltaT.size(); jj++){
		 h_ecals_melectrontag_deltat->Fill(ecal_obj->MElectronTag_DeltaT[jj]);
	       }
	       for (unsigned int jj=0;jj<ecal_obj->MElectronTag_EDep.size(); jj++){
		 h_ecals_melectrontag_edep->Fill(ecal_obj->MElectronTag_EDep[jj]);
	       }
	       for (unsigned int jj=0;jj<ecal_obj->MElectronTag_TrackEnd.size(); jj++){
		 h_ecals_melectrontag_trackend->Fill(ecal_obj->MElectronTag_TrackEnd[jj]);
	       }

	       h_ecals_emenergy_likelihood_energygrad->Fill(ecal_obj->EMEnergy_Likelihood_energyGrad);
	       h_ecals_emenergy_likelihood_energy_qsumgrad->Fill(ecal_obj->EMEnergy_Likelihood_energy_qsumGrad);
	       h_ecals_emenergy_likelihood_qsum_like->Fill(ecal_obj->EMEnergy_Likelihood_qsum_like);
	       h_ecals_emenergy_para_qsum->Fill(ecal_obj->EMEnergy_Para_QSum);
	       h_ecals_emenergy_para_qmean->Fill(ecal_obj->EMEnergy_Para_QMean);
	       h_ecals_emenergy_para_qrms->Fill(ecal_obj->EMEnergy_Para_QRMS);
	       h_ecals_emenergy_para_qskew->Fill(ecal_obj->EMEnergy_Para_QSkew);
	       h_ecals_emenergy_para_qmax->Fill(ecal_obj->EMEnergy_Para_QMax);
	       h_ecals_isshowerlike->Fill(ecal_obj->IsShowerLike);
	       h_ecals_istracklike->Fill(ecal_obj->IsTrackLike);
	       h_ecals_pid_amr->Fill(ecal_obj->PID_AMR);
	       h_ecals_pid_angle->Fill(ecal_obj->PID_Angle);
	       h_ecals_pid_max_ratio->Fill(ecal_obj->PID_Max_Ratio);
	       h_ecals_pid_showerangle->Fill(ecal_obj->PID_ShowerAngle);
	       h_ecals_pid_asymmetry->Fill(ecal_obj->PID_Asymmetry);
	       h_ecals_pid_meanposition->Fill(ecal_obj->PID_MeanPosition);
	       h_ecals_pid_showerwidth->Fill(ecal_obj->PID_ShowerWidth);
	       h_ecals_pid_mostupstreamlayerhit->Fill(ecal_obj->mostUpStreamLayerHit);
	       h_ecals_pid_mostdownstreamlayerhit->Fill(ecal_obj->mostDownStreamLayerHit);
	       h_ecals_conex->Fill(ecal_obj->Cone.X());
	       h_ecals_coney->Fill(ecal_obj->Cone.Y());
	       h_ecals_conez->Fill(ecal_obj->Cone.Z());
	       h_ecals_fdirx->Fill(ecal_obj->FrontDirection.X());
	       h_ecals_fdiry->Fill(ecal_obj->FrontDirection.Y());
	       h_ecals_fdirz->Fill(ecal_obj->FrontDirection.Z());
	       h_ecals_fdir_varx->Fill(ecal_obj->FrontDirectionVar.X());
	       h_ecals_fdir_vary->Fill(ecal_obj->FrontDirectionVar.Y());
	       h_ecals_fdir_varz->Fill(ecal_obj->FrontDirectionVar.Z());
	       h_ecals_bdirx->Fill(ecal_obj->BackDirection.X());
	       h_ecals_bdiry->Fill(ecal_obj->BackDirection.Y());
	       h_ecals_bdirz->Fill(ecal_obj->BackDirection.Z());
	       h_ecals_bdir_varx->Fill(ecal_obj->BackDirectionVar.X());
	       h_ecals_bdir_varx_full->Fill(ecal_obj->BackDirectionVar.X());
	       h_ecals_bdir_vary->Fill(ecal_obj->BackDirectionVar.Y());
	       h_ecals_bdir_varz->Fill(ecal_obj->BackDirectionVar.Z());
	       h_ecals_UniqueID                   ->Fill(ecal_obj-> UniqueID          );
	       h_ecals_Status                     ->Fill(ecal_obj-> Status            );
	       h_ecals_Detector                   ->Fill(ecal_obj-> Detector          );
	       h_ecals_NNodes                     ->Fill(ecal_obj-> NNodes            );
	       h_ecals_NDOF                       ->Fill(ecal_obj-> NDOF              );
	       h_ecals_Chi2                       ->Fill(ecal_obj-> Chi2              );
	       h_ecals_EDeposit                   ->Fill(ecal_obj-> EDeposit          );
	       h_ecals_NConstituents              ->Fill(ecal_obj-> NConstituents     );
	       h_ecals_Length                     ->Fill(ecal_obj-> Length            );
	       h_ecals_FrontMomentum              ->Fill(ecal_obj-> FrontMomentum     );
	       h_ecals_BackMomentum               ->Fill(ecal_obj-> BackMomentum      );
	       h_ecals_FrontMomentumError         ->Fill(ecal_obj-> FrontMomentumError);
	       h_ecals_BackMomentumError         ->Fill(ecal_obj-> BackMomentumError);
	       h_ecals_TrueParticleID         ->Fill(ecal_obj->      TrueParticle.ID);
	       h_ecals_TrueParticlePur         ->Fill(ecal_obj->     TrueParticle.Pur);
	       h_ecals_TrueParticleEff         ->Fill(ecal_obj->     TrueParticle.Eff);
	       h_ecals_TrueParticleVertexPositionX ->Fill(ecal_obj-> TrueParticle.Vertex.Position.X());
	       h_ecals_TrueParticleVertexPositionY ->Fill(ecal_obj-> TrueParticle.Vertex.Position.Y());
	       h_ecals_TrueParticleVertexPositionZ ->Fill(ecal_obj-> TrueParticle.Vertex.Position.Z());
	       h_ecals_TrueParticleVertexPositionT ->Fill(ecal_obj-> TrueParticle.Vertex.Position.T());
	       h_ecals_TrueParticleVertexID   ->Fill(ecal_obj->      TrueParticle.Vertex.ID);
	       h_ecals_TrueParticleVertexPur  ->Fill(ecal_obj->      TrueParticle.Vertex.Pur);
	       h_ecals_TrueParticleVertexEff  ->Fill(ecal_obj->      TrueParticle.Vertex.Eff);
	       h_ecals_TrueParticleVertexPur_large  ->Fill(ecal_obj-> TrueParticle.Vertex.Pur);
	       h_ecals_TrueParticleVertexEff_large  ->Fill(ecal_obj-> TrueParticle.Vertex.Eff);
	       h_ecals_pid_asymmetry_small->Fill(ecal_obj->PID_Asymmetry);


	     }

	     //Autoscaling

	     h_ecals_averagehittime_full->SetBit(TH1::kCanRebin);
	     h_ecals_bdir_varx_full->SetBit(TH1::kCanRebin);
	     h_ecals_TrueParticleVertexPur_large  ->SetBit(TH1::kCanRebin);
	     h_ecals_TrueParticleVertexEff_large  ->SetBit(TH1::kCanRebin);

	   }

	 //SMRD OBJECT

	 for (int j=0;j<curr_pid->NSMRDs;j++){
	   ND::TGlobalReconModule::TSMRDObject* msrd_obj = (ND::TGlobalReconModule::TSMRDObject*) curr_pid->SMRD->At(j);

	   h_smrd_UniqueID          ->Fill(msrd_obj->UniqueID          );
	   h_smrd_Status            ->Fill(msrd_obj->Status            );
	   h_smrd_Detector          ->Fill(msrd_obj->Detector          );
	   h_smrd_NHits             ->Fill(msrd_obj->NHits             );
	   h_smrd_NNodes            ->Fill(msrd_obj->NNodes            );
	   h_smrd_NDOF              ->Fill(msrd_obj->NDOF              );
	   h_smrd_Chi2              ->Fill(msrd_obj->Chi2              );
	   h_smrd_EDeposit          ->Fill(msrd_obj->EDeposit          );
	   h_smrd_NConstituents     ->Fill(msrd_obj->NConstituents     );
	   h_smrd_Length            ->Fill(msrd_obj->Length            );
	   h_smrd_FrontPositionT    ->Fill(msrd_obj->FrontPosition.T()    );
	   h_smrd_FrontPositionX    ->Fill(msrd_obj->FrontPosition.X()    );
	   h_smrd_FrontPositionY    ->Fill(msrd_obj->FrontPosition.Y()    );
	   h_smrd_FrontPositionZ    ->Fill(msrd_obj->FrontPosition.Z()    );
	   h_smrd_BackPositionT     ->Fill(msrd_obj->BackPosition.T()     );
	   h_smrd_BackPositionX     ->Fill(msrd_obj->BackPosition.X()     );
	   h_smrd_BackPositionY     ->Fill(msrd_obj->BackPosition.Y()     );
	   h_smrd_BackPositionZ     ->Fill(msrd_obj->BackPosition.Z()     );
	   h_smrd_FrontPositionVarT ->Fill(msrd_obj->FrontPositionVar.T() );
	   h_smrd_FrontPositionVarX ->Fill(msrd_obj->FrontPositionVar.X() );
	   h_smrd_FrontPositionVarY ->Fill(msrd_obj->FrontPositionVar.Y() );
	   h_smrd_FrontPositionVarZ ->Fill(msrd_obj->FrontPositionVar.Z() );
	   h_smrd_BackPositionVarT  ->Fill(msrd_obj->BackPositionVar.T()  );
	   h_smrd_BackPositionVarX  ->Fill(msrd_obj->BackPositionVar.X()  );
	   h_smrd_BackPositionVarY  ->Fill(msrd_obj->BackPositionVar.Y()  );
	   h_smrd_BackPositionVarZ  ->Fill(msrd_obj->BackPositionVar.Z()  );
	   h_smrd_FrontDirectionX   ->Fill(msrd_obj->FrontDirection.X()   );
	   h_smrd_FrontDirectionY   ->Fill(msrd_obj->FrontDirection.Y()   );
	   h_smrd_FrontDirectionZ   ->Fill(msrd_obj->FrontDirection.Z()   );
	   h_smrd_BackDirectionX    ->Fill(msrd_obj->BackDirection.X()    );
	   h_smrd_BackDirectionY    ->Fill(msrd_obj->BackDirection.Y()    );
	   h_smrd_BackDirectionZ    ->Fill(msrd_obj->BackDirection.Z()    );
	   h_smrd_FrontDirectionVarX->Fill(msrd_obj->FrontDirectionVar.X());
	   h_smrd_FrontDirectionVarY->Fill(msrd_obj->FrontDirectionVar.Y());
	   h_smrd_FrontDirectionVarZ->Fill(msrd_obj->FrontDirectionVar.Z());
	   h_smrd_BackDirectionVarX ->Fill(msrd_obj->BackDirectionVar.X() );
	   h_smrd_BackDirectionVarY ->Fill(msrd_obj->BackDirectionVar.Y() );
	   h_smrd_BackDirectionVarZ ->Fill(msrd_obj->BackDirectionVar.Z() );
	   h_smrd_FrontMomentum     ->Fill(msrd_obj->FrontMomentum     );
	   h_smrd_BackMomentum      ->Fill(msrd_obj->BackMomentum      );
	   h_smrd_FrontMomentumError->Fill(msrd_obj->FrontMomentumError);
	   h_smrd_BackMomentumError ->Fill(msrd_obj->BackMomentumError );
	   h_smrd_TrueParticleID   ->Fill(msrd_obj->TrueParticle.ID);
	   h_smrd_TrueParticlePur          ->Fill(msrd_obj->TrueParticle.Pur);
	   h_smrd_TrueParticleEff        ->Fill(msrd_obj->TrueParticle.Eff);
	   h_smrd_TrueParticleVertexPositionX      ->Fill(msrd_obj->TrueParticle.Vertex.Position.X());
	   h_smrd_TrueParticleVertexPositionY      ->Fill(msrd_obj->TrueParticle.Vertex.Position.Y());
	   h_smrd_TrueParticleVertexPositionZ      ->Fill(msrd_obj->TrueParticle.Vertex.Position.Z());
	   h_smrd_TrueParticleVertexPositionT      ->Fill(msrd_obj->TrueParticle.Vertex.Position.T());
	   h_smrd_TrueParticleVertexID             ->Fill(msrd_obj->TrueParticle.Vertex.ID);
	   h_smrd_TrueParticleVertexPur            ->Fill(msrd_obj->TrueParticle.Vertex.Pur);
	   h_smrd_TrueParticleVertexEff            ->Fill(msrd_obj->TrueParticle.Vertex.Eff);
	   h_smrd_TrueParticleVertexPur_zoom            ->Fill(msrd_obj->TrueParticle.Vertex.Pur);
	   h_smrd_TrueParticleVertexEff_zoom            ->Fill(msrd_obj->TrueParticle.Vertex.Eff);

	   //Autoscaling

	   h_smrd_TrueParticleVertexPur                              ->SetBit(TH1::kCanRebin);
	   h_smrd_TrueParticleVertexEff                              ->SetBit(TH1::kCanRebin);

	 }

     }
     //end GlobalPIDs
     //TPC OTHER OBJECTS

     for(int i = 0; i < NTPCOthers; ++i){
       ND::TGlobalReconModule::TTPCOtherObject* tpcothobj = (ND::TGlobalReconModule::TTPCOtherObject*) TPCOthers->At(i);

       h_tpcoth_charge      ->Fill(tpcothobj->Charge);
       h_tpcoth_detec       ->Fill(tpcothobj->Detector);
       h_tpcoth_nhits       ->Fill(tpcothobj->NHits);
       h_tpcoth_chi2        ->Fill(tpcothobj->Chi2);
       h_tpcoth_edep        ->Fill(tpcothobj->EDeposit);

       h_tpcoth_frontpos_x  ->Fill(tpcothobj->FrontPosition.X());
       h_tpcoth_frontpos_y     ->Fill(tpcothobj->FrontPosition.Y());
       h_tpcoth_frontpos_z     ->Fill(tpcothobj->FrontPosition.Z());
       h_tpcoth_frontpos_t     ->Fill(tpcothobj->FrontPosition.T());

       h_tpcoth_backpos_x      ->Fill(tpcothobj->BackPosition.X());
       h_tpcoth_backpos_y      ->Fill(tpcothobj->BackPosition.Y());
       h_tpcoth_backpos_z      ->Fill(tpcothobj->BackPosition.Z());
       h_tpcoth_backpos_t      ->Fill(tpcothobj->BackPosition.T());

       h_tpcoth_frontdir_x     ->Fill(tpcothobj->FrontDirection.X());
       h_tpcoth_frontdir_y     ->Fill(tpcothobj->FrontDirection.Y());
       h_tpcoth_frontdir_z     ->Fill(tpcothobj->FrontDirection.Z());

       h_tpcoth_backdir_x ->Fill(tpcothobj->BackDirection.X());
       h_tpcoth_backdir_y      ->Fill(tpcothobj->BackDirection.Y());
       h_tpcoth_backdir_z      ->Fill(tpcothobj->BackDirection.Z());

       h_tpcoth_mom       ->Fill(tpcothobj->Momentum);

       h_tpcoth_tpid        ->Fill(tpcothobj->TrueParticle.ID);
       h_tpcoth_tppur       ->Fill(tpcothobj->TrueParticle.Pur);
       h_tpcoth_tpeff       ->Fill(tpcothobj->TrueParticle.Eff);
       h_tpcoth_tpvertpos_x ->Fill(tpcothobj->TrueParticle.Vertex.Position.X());
       h_tpcoth_tpvertpos_y   ->Fill(tpcothobj->TrueParticle.Vertex.Position.Y());
       h_tpcoth_tpvertpos_z    ->Fill(tpcothobj->TrueParticle.Vertex.Position.Z());
       h_tpcoth_tpvertid    ->Fill(tpcothobj->TrueParticle.Vertex.ID);
       h_tpcoth_tpvertpur   ->Fill(tpcothobj->TrueParticle.Vertex.Pur);
       h_tpcoth_tpverteff   ->Fill(tpcothobj->TrueParticle.Vertex.Eff);

     }

     //FGD TIME BINS
     for (int j = 0; j < NFgdTimeBins; ++j)
       {
	 ND::TGlobalReconModule::TFgdTimeBin* fgd_tbin = (ND::TGlobalReconModule::TFgdTimeBin*) FgdTimeBins->At(j);

	 h_tfgtb_maxtime             ->Fill(fgd_tbin->maxTime);
	 h_tfgtb_mintime             ->Fill(fgd_tbin->minTime);

	 for (int k = 0; k < 2; ++k)
	   {
	     h_tfgtb_nhits              ->Fill(fgd_tbin->nHits[k]);
	     h_tfgtb_rawchargesum ->Fill(fgd_tbin->rawChargeSum[k]);
	     h_tfgtb_chargeweightedposx ->Fill(fgd_tbin->chargeWeightedPos[k].X());
	     h_tfgtb_chargeweightedposy ->Fill(fgd_tbin->chargeWeightedPos[k].Y());
	     h_tfgtb_chargeweightedposz ->Fill(fgd_tbin->chargeWeightedPos[k].Z());
	   }

	 //FGD1 OUTERMOST HITS

	 h_tfgtb_fgd1outermosthits_xmax_charge  ->Fill(fgd_tbin->FGD1OutermostHits.hitMaxX.Charge);
	 h_tfgtb_fgd1outermosthits_xmax_time    ->Fill(fgd_tbin->FGD1OutermostHits.hitMaxX.Time);
	 h_tfgtb_fgd1outermosthits_xmax_pos_x   ->Fill(fgd_tbin->FGD1OutermostHits.hitMaxX.Position.X());
	 h_tfgtb_fgd1outermosthits_xmax_pos_y   ->Fill(fgd_tbin->FGD1OutermostHits.hitMaxX.Position.Y());
	 h_tfgtb_fgd1outermosthits_xmax_pos_z   ->Fill(fgd_tbin->FGD1OutermostHits.hitMaxX.Position.Z());
	 h_tfgtb_fgd1outermosthits_xmax_dpos_x  ->Fill(fgd_tbin->FGD1OutermostHits.hitMaxX.PositionError.X());
	 h_tfgtb_fgd1outermosthits_xmax_dpos_y  ->Fill(fgd_tbin->FGD1OutermostHits.hitMaxX.PositionError.Y());
	 h_tfgtb_fgd1outermosthits_xmax_dpos_z  ->Fill(fgd_tbin->FGD1OutermostHits.hitMaxX.PositionError.Z());
	 h_tfgtb_fgd1outermosthits_xmax_type    ->Fill(fgd_tbin->FGD1OutermostHits.hitMaxX.Type);

	 h_tfgtb_fgd1outermosthits_xmin_charge  ->Fill(fgd_tbin->FGD1OutermostHits.hitMinX.Charge);
	 h_tfgtb_fgd1outermosthits_xmin_time    ->Fill(fgd_tbin->FGD1OutermostHits.hitMinX.Time);
	 h_tfgtb_fgd1outermosthits_xmin_pos_x   ->Fill(fgd_tbin->FGD1OutermostHits.hitMinX.Position.X());
	 h_tfgtb_fgd1outermosthits_xmin_pos_y   ->Fill(fgd_tbin->FGD1OutermostHits.hitMinX.Position.Y());
	 h_tfgtb_fgd1outermosthits_xmin_pos_z   ->Fill(fgd_tbin->FGD1OutermostHits.hitMinX.Position.Z());
	 h_tfgtb_fgd1outermosthits_xmin_dpos_x  ->Fill(fgd_tbin->FGD1OutermostHits.hitMinX.PositionError.X());
	 h_tfgtb_fgd1outermosthits_xmin_dpos_y  ->Fill(fgd_tbin->FGD1OutermostHits.hitMinX.PositionError.Y());
	 h_tfgtb_fgd1outermosthits_xmin_dpos_z  ->Fill(fgd_tbin->FGD1OutermostHits.hitMinX.PositionError.Z());
	 h_tfgtb_fgd1outermosthits_xmin_type    ->Fill(fgd_tbin->FGD1OutermostHits.hitMinX.Type);

	 h_tfgtb_fgd1outermosthits_ymax_charge  ->Fill(fgd_tbin->FGD1OutermostHits.hitMaxY.Charge);
	 h_tfgtb_fgd1outermosthits_ymax_time    ->Fill(fgd_tbin->FGD1OutermostHits.hitMaxY.Time);
	 h_tfgtb_fgd1outermosthits_ymax_pos_x   ->Fill(fgd_tbin->FGD1OutermostHits.hitMaxY.Position.X());
	 h_tfgtb_fgd1outermosthits_ymax_pos_y   ->Fill(fgd_tbin->FGD1OutermostHits.hitMaxY.Position.Y());
	 h_tfgtb_fgd1outermosthits_ymax_pos_z   ->Fill(fgd_tbin->FGD1OutermostHits.hitMaxY.Position.Z());
	 h_tfgtb_fgd1outermosthits_ymax_dpos_x  ->Fill(fgd_tbin->FGD1OutermostHits.hitMaxY.PositionError.X());
	 h_tfgtb_fgd1outermosthits_ymax_dpos_y  ->Fill(fgd_tbin->FGD1OutermostHits.hitMaxY.PositionError.Y());
	 h_tfgtb_fgd1outermosthits_ymax_dpos_z  ->Fill(fgd_tbin->FGD1OutermostHits.hitMaxY.PositionError.Z());
	 h_tfgtb_fgd1outermosthits_ymax_type    ->Fill(fgd_tbin->FGD1OutermostHits.hitMaxY.Type);

	 h_tfgtb_fgd1outermosthits_ymin_charge  ->Fill(fgd_tbin->FGD1OutermostHits.hitMinY.Charge);
	 h_tfgtb_fgd1outermosthits_ymin_time    ->Fill(fgd_tbin->FGD1OutermostHits.hitMinY.Time);
	 h_tfgtb_fgd1outermosthits_ymin_pos_x   ->Fill(fgd_tbin->FGD1OutermostHits.hitMinY.Position.X());
	 h_tfgtb_fgd1outermosthits_ymin_pos_y   ->Fill(fgd_tbin->FGD1OutermostHits.hitMinY.Position.Y());
	 h_tfgtb_fgd1outermosthits_ymin_pos_z   ->Fill(fgd_tbin->FGD1OutermostHits.hitMinY.Position.Z());
	 h_tfgtb_fgd1outermosthits_ymin_dpos_x  ->Fill(fgd_tbin->FGD1OutermostHits.hitMinY.PositionError.X());
	 h_tfgtb_fgd1outermosthits_ymin_dpos_y  ->Fill(fgd_tbin->FGD1OutermostHits.hitMinY.PositionError.Y());
	 h_tfgtb_fgd1outermosthits_ymin_dpos_z  ->Fill(fgd_tbin->FGD1OutermostHits.hitMinY.PositionError.Z());
	 h_tfgtb_fgd1outermosthits_ymin_type    ->Fill(fgd_tbin->FGD1OutermostHits.hitMinY.Type);

	 h_tfgtb_fgd1outermosthits_zmax_charge  ->Fill(fgd_tbin->FGD1OutermostHits.hitMaxZ.Charge);
	 h_tfgtb_fgd1outermosthits_zmax_time    ->Fill(fgd_tbin->FGD1OutermostHits.hitMaxZ.Time);
	 h_tfgtb_fgd1outermosthits_zmax_pos_x   ->Fill(fgd_tbin->FGD1OutermostHits.hitMaxZ.Position.X());
	 h_tfgtb_fgd1outermosthits_zmax_pos_y   ->Fill(fgd_tbin->FGD1OutermostHits.hitMaxZ.Position.Y());
	 h_tfgtb_fgd1outermosthits_zmax_pos_z   ->Fill(fgd_tbin->FGD1OutermostHits.hitMaxZ.Position.Z());
	 h_tfgtb_fgd1outermosthits_zmax_dpos_x  ->Fill(fgd_tbin->FGD1OutermostHits.hitMaxZ.PositionError.X());
	 h_tfgtb_fgd1outermosthits_zmax_dpos_y  ->Fill(fgd_tbin->FGD1OutermostHits.hitMaxZ.PositionError.Y());
	 h_tfgtb_fgd1outermosthits_zmax_dpos_z  ->Fill(fgd_tbin->FGD1OutermostHits.hitMaxZ.PositionError.Z());
	 h_tfgtb_fgd1outermosthits_zmax_type    ->Fill(fgd_tbin->FGD1OutermostHits.hitMaxZ.Type);

	 h_tfgtb_fgd1outermosthits_zmin_charge  ->Fill(fgd_tbin->FGD1OutermostHits.hitMinZ.Charge);
	 h_tfgtb_fgd1outermosthits_zmin_time    ->Fill(fgd_tbin->FGD1OutermostHits.hitMinZ.Time);
	 h_tfgtb_fgd1outermosthits_zmin_pos_x   ->Fill(fgd_tbin->FGD1OutermostHits.hitMinZ.Position.X());
	 h_tfgtb_fgd1outermosthits_zmin_pos_y   ->Fill(fgd_tbin->FGD1OutermostHits.hitMinZ.Position.Y());
	 h_tfgtb_fgd1outermosthits_zmin_pos_z   ->Fill(fgd_tbin->FGD1OutermostHits.hitMinZ.Position.Z());
	 h_tfgtb_fgd1outermosthits_zmin_dpos_x  ->Fill(fgd_tbin->FGD1OutermostHits.hitMinZ.PositionError.X());
	 h_tfgtb_fgd1outermosthits_zmin_dpos_y  ->Fill(fgd_tbin->FGD1OutermostHits.hitMinZ.PositionError.Y());
	 h_tfgtb_fgd1outermosthits_zmin_dpos_z  ->Fill(fgd_tbin->FGD1OutermostHits.hitMinZ.PositionError.Z());
	 h_tfgtb_fgd1outermosthits_zmin_type    ->Fill(fgd_tbin->FGD1OutermostHits.hitMinZ.Type);

	 //FGD2 OUTERMOST HITS

	 h_tfgtb_fgd2outermosthits_xmax_charge  ->Fill(fgd_tbin->FGD2OutermostHits.hitMaxX.Charge);
	 h_tfgtb_fgd2outermosthits_xmax_time    ->Fill(fgd_tbin->FGD2OutermostHits.hitMaxX.Time);
	 h_tfgtb_fgd2outermosthits_xmax_pos_x   ->Fill(fgd_tbin->FGD2OutermostHits.hitMaxX.Position.X());
	 h_tfgtb_fgd2outermosthits_xmax_pos_y   ->Fill(fgd_tbin->FGD2OutermostHits.hitMaxX.Position.Y());
	 h_tfgtb_fgd2outermosthits_xmax_pos_z   ->Fill(fgd_tbin->FGD2OutermostHits.hitMaxX.Position.Z());
	 h_tfgtb_fgd2outermosthits_xmax_dpos_x  ->Fill(fgd_tbin->FGD2OutermostHits.hitMaxX.PositionError.X());
	 h_tfgtb_fgd2outermosthits_xmax_dpos_y  ->Fill(fgd_tbin->FGD2OutermostHits.hitMaxX.PositionError.Y());
	 h_tfgtb_fgd2outermosthits_xmax_dpos_z  ->Fill(fgd_tbin->FGD2OutermostHits.hitMaxX.PositionError.Z());
	 h_tfgtb_fgd2outermosthits_xmax_type    ->Fill(fgd_tbin->FGD2OutermostHits.hitMaxX.Type);

	 h_tfgtb_fgd2outermosthits_xmin_charge  ->Fill(fgd_tbin->FGD2OutermostHits.hitMinX.Charge);
	 h_tfgtb_fgd2outermosthits_xmin_time    ->Fill(fgd_tbin->FGD2OutermostHits.hitMinX.Time);
	 h_tfgtb_fgd2outermosthits_xmin_pos_x   ->Fill(fgd_tbin->FGD2OutermostHits.hitMinX.Position.X());
	 h_tfgtb_fgd2outermosthits_xmin_pos_y   ->Fill(fgd_tbin->FGD2OutermostHits.hitMinX.Position.Y());
	 h_tfgtb_fgd2outermosthits_xmin_pos_z   ->Fill(fgd_tbin->FGD2OutermostHits.hitMinX.Position.Z());
	 h_tfgtb_fgd2outermosthits_xmin_dpos_x  ->Fill(fgd_tbin->FGD2OutermostHits.hitMinX.PositionError.X());
	 h_tfgtb_fgd2outermosthits_xmin_dpos_y  ->Fill(fgd_tbin->FGD2OutermostHits.hitMinX.PositionError.Y());
	 h_tfgtb_fgd2outermosthits_xmin_dpos_z  ->Fill(fgd_tbin->FGD2OutermostHits.hitMinX.PositionError.Z());
	 h_tfgtb_fgd2outermosthits_xmin_type    ->Fill(fgd_tbin->FGD2OutermostHits.hitMinX.Type);

	 h_tfgtb_fgd2outermosthits_ymax_charge  ->Fill(fgd_tbin->FGD2OutermostHits.hitMaxY.Charge);
	 h_tfgtb_fgd2outermosthits_ymax_time    ->Fill(fgd_tbin->FGD2OutermostHits.hitMaxY.Time);
	 h_tfgtb_fgd2outermosthits_ymax_pos_x   ->Fill(fgd_tbin->FGD2OutermostHits.hitMaxY.Position.X());
	 h_tfgtb_fgd2outermosthits_ymax_pos_y   ->Fill(fgd_tbin->FGD2OutermostHits.hitMaxY.Position.Y());
	 h_tfgtb_fgd2outermosthits_ymax_pos_z   ->Fill(fgd_tbin->FGD2OutermostHits.hitMaxY.Position.Z());
	 h_tfgtb_fgd2outermosthits_ymax_dpos_x  ->Fill(fgd_tbin->FGD2OutermostHits.hitMaxY.PositionError.X());
	 h_tfgtb_fgd2outermosthits_ymax_dpos_y  ->Fill(fgd_tbin->FGD2OutermostHits.hitMaxY.PositionError.Y());
	 h_tfgtb_fgd2outermosthits_ymax_dpos_z  ->Fill(fgd_tbin->FGD2OutermostHits.hitMaxY.PositionError.Z());
	 h_tfgtb_fgd2outermosthits_ymax_type    ->Fill(fgd_tbin->FGD2OutermostHits.hitMaxY.Type);

	 h_tfgtb_fgd2outermosthits_ymin_charge  ->Fill(fgd_tbin->FGD2OutermostHits.hitMinY.Charge);
	 h_tfgtb_fgd2outermosthits_ymin_time    ->Fill(fgd_tbin->FGD2OutermostHits.hitMinY.Time);
	 h_tfgtb_fgd2outermosthits_ymin_pos_x   ->Fill(fgd_tbin->FGD2OutermostHits.hitMinY.Position.X());
	 h_tfgtb_fgd2outermosthits_ymin_pos_y   ->Fill(fgd_tbin->FGD2OutermostHits.hitMinY.Position.Y());
	 h_tfgtb_fgd2outermosthits_ymin_pos_z   ->Fill(fgd_tbin->FGD2OutermostHits.hitMinY.Position.Z());
	 h_tfgtb_fgd2outermosthits_ymin_dpos_x  ->Fill(fgd_tbin->FGD2OutermostHits.hitMinY.PositionError.X());
	 h_tfgtb_fgd2outermosthits_ymin_dpos_y  ->Fill(fgd_tbin->FGD2OutermostHits.hitMinY.PositionError.Y());
	 h_tfgtb_fgd2outermosthits_ymin_dpos_z  ->Fill(fgd_tbin->FGD2OutermostHits.hitMinY.PositionError.Z());
	 h_tfgtb_fgd2outermosthits_ymin_type    ->Fill(fgd_tbin->FGD2OutermostHits.hitMinY.Type);

	 h_tfgtb_fgd2outermosthits_zmax_charge  ->Fill(fgd_tbin->FGD2OutermostHits.hitMaxZ.Charge);
	 h_tfgtb_fgd2outermosthits_zmax_time    ->Fill(fgd_tbin->FGD2OutermostHits.hitMaxZ.Time);
	 h_tfgtb_fgd2outermosthits_zmax_pos_x   ->Fill(fgd_tbin->FGD2OutermostHits.hitMaxZ.Position.X());
	 h_tfgtb_fgd2outermosthits_zmax_pos_y   ->Fill(fgd_tbin->FGD2OutermostHits.hitMaxZ.Position.Y());
	 h_tfgtb_fgd2outermosthits_zmax_pos_z   ->Fill(fgd_tbin->FGD2OutermostHits.hitMaxZ.Position.Z());
	 h_tfgtb_fgd2outermosthits_zmax_dpos_x  ->Fill(fgd_tbin->FGD2OutermostHits.hitMaxZ.PositionError.X());
	 h_tfgtb_fgd2outermosthits_zmax_dpos_y  ->Fill(fgd_tbin->FGD2OutermostHits.hitMaxZ.PositionError.Y());
	 h_tfgtb_fgd2outermosthits_zmax_dpos_z  ->Fill(fgd_tbin->FGD2OutermostHits.hitMaxZ.PositionError.Z());
	 h_tfgtb_fgd2outermosthits_zmax_type    ->Fill(fgd_tbin->FGD2OutermostHits.hitMaxZ.Type);

	 h_tfgtb_fgd2outermosthits_zmin_charge  ->Fill(fgd_tbin->FGD2OutermostHits.hitMinZ.Charge);
	 h_tfgtb_fgd2outermosthits_zmin_time    ->Fill(fgd_tbin->FGD2OutermostHits.hitMinZ.Time);
	 h_tfgtb_fgd2outermosthits_zmin_pos_x   ->Fill(fgd_tbin->FGD2OutermostHits.hitMinZ.Position.X());
	 h_tfgtb_fgd2outermosthits_zmin_pos_y   ->Fill(fgd_tbin->FGD2OutermostHits.hitMinZ.Position.Y());
	 h_tfgtb_fgd2outermosthits_zmin_pos_z   ->Fill(fgd_tbin->FGD2OutermostHits.hitMinZ.Position.Z());
	 h_tfgtb_fgd2outermosthits_zmin_dpos_x  ->Fill(fgd_tbin->FGD2OutermostHits.hitMinZ.PositionError.X());
	 h_tfgtb_fgd2outermosthits_zmin_dpos_y  ->Fill(fgd_tbin->FGD2OutermostHits.hitMinZ.PositionError.Y());
	 h_tfgtb_fgd2outermosthits_zmin_dpos_z  ->Fill(fgd_tbin->FGD2OutermostHits.hitMinZ.PositionError.Z());
	 h_tfgtb_fgd2outermosthits_zmin_type    ->Fill(fgd_tbin->FGD2OutermostHits.hitMinZ.Type);


	 h_tfgtb_nfgd1unused -> Fill(fgd_tbin->NFGD1Unused);
	 //h_tfgtb_fgd1unused  //this is a whole TClonesArray
	 h_tfgtb_nfgd2unused -> Fill(fgd_tbin->NFGD2Unused);
	 //h_tfgtb_fgd2unused          //this is a whole TClonesArray
	 h_tfgtb_g4id        -> Fill(fgd_tbin->g4ID);

	 h_tfgtb_hackfgdunused_charge ->Fill(fgd_tbin->hackFGDUnused.Charge);
	 h_tfgtb_hackfgdunused_time   ->Fill(fgd_tbin->hackFGDUnused.Time);
	 h_tfgtb_hackfgdunused_pos_x  ->Fill(fgd_tbin->hackFGDUnused.Position.X());
	 h_tfgtb_hackfgdunused_pos_y  ->Fill(fgd_tbin->hackFGDUnused.Position.Y());
	 h_tfgtb_hackfgdunused_pos_z  ->Fill(fgd_tbin->hackFGDUnused.Position.Z());
	 h_tfgtb_hackfgdunused_dpos_x ->Fill(fgd_tbin->hackFGDUnused.PositionError.X());
	 h_tfgtb_hackfgdunused_dpos_y ->Fill(fgd_tbin->hackFGDUnused.PositionError.Y());
	 h_tfgtb_hackfgdunused_dpos_z ->Fill(fgd_tbin->hackFGDUnused.PositionError.Z());
	 h_tfgtb_hackfgdunused_type   ->Fill(fgd_tbin->hackFGDUnused.Type);

       }


     //SMRD HIT FILLING SECTION

     for(int j=0;j<NSMRDUnused;j++){
       ND::TGlobalReconModule::TSMRDHit* smrdhit_obj = (ND::TGlobalReconModule::TSMRDHit*) SMRDUnused->At(j);

       h_smrdhit_Wall          ->Fill(smrdhit_obj->Wall);
       h_smrdhit_Yoke          ->Fill(smrdhit_obj->Yoke);
       h_smrdhit_Layer         ->Fill(smrdhit_obj->Layer);
       h_smrdhit_Tower         ->Fill(smrdhit_obj->Tower);
       h_smrdhit_Counter       ->Fill(smrdhit_obj->Counter);
       h_smrdhit_Charge        ->Fill(smrdhit_obj->Charge);
       h_smrdhit_Time          ->Fill(smrdhit_obj->Time);
       h_smrdhit_PositionX     ->Fill(smrdhit_obj->Position.X());
       h_smrdhit_PositionY     ->Fill(smrdhit_obj->Position.Y());
       h_smrdhit_PositionZ     ->Fill(smrdhit_obj->Position.Z());
       h_smrdhit_PositionErrorX->Fill(smrdhit_obj->PositionError.X());
       h_smrdhit_PositionErrorY->Fill(smrdhit_obj->PositionError.Y());
       h_smrdhit_PositionErrorZ->Fill(smrdhit_obj->PositionError.Z());
       h_smrdhit_Type          ->Fill(smrdhit_obj->Type);

     }

     //FGD CLUSTER FILLING SECTION
     /*
     for(int j=0;j<NDelayedClusters;j++){
       ND::TGlobalReconModule::TFgdCluster* fgdclust_obj = (ND::TGlobalReconModule::TFgdCluster*) DelayedClusters->At(j);

       h_fgdcluster_TotalCharge     ->Fill(fgdclust_obj->                               TotalCharge);
       h_fgdcluster_TimeDisp        ->Fill(fgdclust_obj->                               TimeDisp);
       h_fgdcluster_TimeDisp_small        ->Fill(fgdclust_obj->                               TimeDisp);
       h_fgdcluster_TimeDisp_large        ->Fill(fgdclust_obj->                               TimeDisp);
       h_fgdcluster_SpatialDisp     ->Fill(fgdclust_obj->                               SpatialDisp);
       h_fgdcluster_NHits           ->Fill(fgdclust_obj->                               NHits);
       h_fgdcluster_PosRMSx          ->Fill(fgdclust_obj->                               PosRMS.X());
       h_fgdcluster_PosRMSy          ->Fill(fgdclust_obj->                               PosRMS.Y());
       h_fgdcluster_PosRMSz          ->Fill(fgdclust_obj->                               PosRMS.Z());
       h_fgdcluster_PosRMSt          ->Fill(fgdclust_obj->                               PosRMS.T());
       h_fgdcluster_Positionx        ->Fill(fgdclust_obj->                               Position.X());
       h_fgdcluster_Positiony        ->Fill(fgdclust_obj->                               Position.Y());
       h_fgdcluster_Positionz        ->Fill(fgdclust_obj->                               Position.Z());
       h_fgdcluster_Positiont        ->Fill(fgdclust_obj->                               Position.T());
       h_fgdcluster_Variancex        ->Fill(fgdclust_obj->                               Variance.X());
       h_fgdcluster_Variancey        ->Fill(fgdclust_obj->                               Variance.Y());
       h_fgdcluster_Variancez        ->Fill(fgdclust_obj->                               Variance.Z());
       h_fgdcluster_Variancet        ->Fill(fgdclust_obj->                               Variance.T());

       //Autoscaling

       h_fgdcluster_TimeDisp       ->SetBit(TH1::kCanRebin);
       h_fgdcluster_Variancex      ->SetBit(TH1::kCanRebin);
       h_fgdcluster_Variancey      ->SetBit(TH1::kCanRebin);
     }
     */

 } // eventloop
  //  h_event->Draw();

  TDirectory *dir = gDirectory;
  TFile *file1 = new TFile("TGlobalReconModule_histograms.root","recreate");
  dir->GetList()->Write();
  delete file1;

  return;
}
