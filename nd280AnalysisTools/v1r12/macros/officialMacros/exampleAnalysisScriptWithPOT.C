/*
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
[root] ..x ${ND280ANALYSISTOOLSROOT}/AnalysisTools/oaAnalysisReadInitFile-\
${ND280ANALYSISTOOLSCONFIG}.C
[root] .L exampleAnalysisScript.C+;
[root] exampleAnalysisScript("path/2/MC/analysisfile.root", nevents);

For a rapid development cycle you can stay in the same root session and do not
have to repeat the first step when recompiling the analysis script to reflect
any changes you have made. This works for CINT or ACLiC:
[root].x ${ND280ANALYSISTOOLSROOT}/AnalysisTools/oaAnalysisReadInitFile-\
${ND280ANALYSISTOOLSCONFIG}.C
[root].L exampleAnalysisScriptWithPOT.C+;
[root]exampleAnalysisScriptWithPOT("path/2/analysisfile.root", nevents);
... make any changes to exampleAnalysisScriptWithPOT.C ...
[root].L exampleAnalysisScriptWithPOT.C+;
[root]exampleAnalysisScriptWithPOT("path/2/analysisfile.root", nevents);

*/
#include <iostream>

#include "TROOT.h"
#include "TFile.h"
#include "TChain.h"
#include "TClonesArray.h"

// To use AnalysisTools utils in ACLiC mode then need to add the relevant header
// here and inside the ifdef so it is not seen by CINT.
#if !defined(__CINT__) || defined(__MAKECINT__)
//Here include the header of your preferred AnalysisTools module
#endif

using std::cout;
using std::endl;

//
//
// Example to run over an oaAnalysis root file, select events assessed
// good from the DQ and beam points of view and compute the POT.
// The beam info is extracted from the file as described in
// app/BSDChecker.cxx. For the DQ info, the flag ND280OffFlag is requested
// to be 0 - that means the data quality assessment is OK.
//
// To run this example use eg
// root -b -q 'exampleAnalysisScriptWithPOT.C("oa_nd_spl_00004174-0002_i6flzubxmpvo_anal_000_v8r5p9-bsdv01.root")'
//
//
void exampleAnalysisScriptWithPOT(char * inputfilename, int max_entries = -1){
  gROOT->SetBatch(1);
// See commented code below for loading libraries automatically.
//#if !defined(__MAKECINT__)
// This should not be seen when using ACLiC.
//  gROOT->ProcessLine(".x ${ND280ANALYSISTOOLSROOT}/AnalysisTools/\\
//  oaAnalysisReadInitFile-${ND280ANALYSISTOOLSCONFIG}.C");
//#endif
  // _________________________________________________________________________
  // Prepare to read trees and then read trees.
  //
  std::cout << "Adding file(s) matching string: " << inputfilename << std::endl;

  //
  // BasicHeader
  //

  TChain BasicHeader = TChain("HeaderDir/BasicHeader");
  //std::cout << "Adding file(s) matching string: " << inputfilename << std::endl;
  //std::cout << "Added "<< BasicHeader.AddFile(inputfilename) << " file(s)"<< std::endl;
  BasicHeader.AddFile(inputfilename);
  Int_t EventID;
  Int_t RunID;
  UInt_t EventTime;
  Int_t EventNumber;
  BasicHeader.SetBranchAddress("EventTime", &EventTime);
  BasicHeader.SetBranchAddress("EventNumber", &EventNumber);
  BasicHeader.SetBranchAddress("EventID", &EventID);
  BasicHeader.SetBranchAddress("RunID", &RunID);

  //
  // BasicDataQuality
  //
  TChain BasicDataQuality = TChain("HeaderDir/BasicDataQuality");
  //std::cout << "Adding file(s) matching string: " << inputfilename << std::endl;
  //std::cout << "Added "<< BasicDataQuality.AddFile(inputfilename) << " file(s)"<< std::endl;
  BasicDataQuality.AddFile(inputfilename);
  Int_t DQEventID;
  Int_t DQRunID;
  Int_t ND280OffFlag;
  BasicDataQuality.SetBranchAddress("EventID", &DQEventID);
  BasicDataQuality.SetBranchAddress("RunID", &DQRunID);
  BasicDataQuality.SetBranchAddress("ND280OffFlag", &ND280OffFlag);

  //
  // BeamSummaryData
  //
  bool health = true;
  // _____________________________________________________________________________________
  // Prepare to read trees.

  TChain BeamSummaryData("HeaderDir/BeamSummaryData");
  //std::cout << "Adding file(s) matching string: " << inputfilename << std::endl;
  //std::cout << "Added "<< BeamSummaryData.AddFile(inputfilename) << " file(s)"<< std::endl;
  BeamSummaryData.AddFile(inputfilename);
  //Int_t SpillNumber;
  Int_t ND280Spill;
  Int_t BRunID;
  Int_t BEventID;
  Int_t BeamSummaryDataStatus;

  BeamSummaryData.SetBranchAddress("RunID",&BRunID);
  BeamSummaryData.SetBranchAddress("EventID",&BEventID);
  BeamSummaryData.SetBranchAddress("BeamSummaryDataStatus",&BeamSummaryDataStatus);
  BeamSummaryData.SetBranchAddress("ND280Spill", &ND280Spill);

  TClonesArray *TClonesBSD = new TClonesArray("ND::TBeamSummaryDataModule::TBeamSummaryData");
  BeamSummaryData.SetBranchAddress("BeamSummaryData",&TClonesBSD);
  ND::TBeamSummaryDataModule::TBeamSummaryData *bsd=(ND::TBeamSummaryDataModule::TBeamSummaryData*)(*TClonesBSD)[0];

  // _________________________________________________________________________
  // Now loop over the entries in the input tree. Each entry corresponds to an
  // ND280Event and it is safe to assume a correspondence between entries on
  // different trees in the same analysis output file.
  // However a check is performed anyway.

  // Loop over all events.
  BasicHeader.GetEntry(0);
  BasicDataQuality.GetEntry(0);
  BeamSummaryData.GetEntry(0);
  int entries = BasicHeader.GetEntries();
  if(max_entries > 0 && max_entries<entries) entries = max_entries;

  Double_t POTs(0);
  Int_t eveTot(0), eveBeam(0), eveDQ(0);
  for(int i = 0; i < entries; i++){

    BasicHeader.GetEntry(i);
    BasicDataQuality.GetEntry(i);
    BeamSummaryData.GetEntry(i);
    if (RunID != BRunID || EventID != BEventID || RunID != DQRunID || EventID != DQEventID ) {
      std::cout << "Runs and Event # mismatch." << std::endl;
      return -1;
     }

    // Only data quality assessed data (ND280OffFlag = 0 ), Beam Data OK (BeamSummaryDataStatus == 1), and good spill ((Int_t (bsd->GoodSpillFlag)) == 1)
    if ( ND280OffFlag == 0 && BeamSummaryDataStatus == 1 && (Int_t (bsd->GoodSpillFlag)) == 1 )
      POTs += bsd->CT5ProtonsPerSpill;
    if ( ND280OffFlag == 0 && BeamSummaryDataStatus == 1 && (Int_t (bsd->GoodSpillFlag)) == 1 )
      eveTot += 1;
    if ( ND280OffFlag == 0 && BeamSummaryDataStatus == 1 && (Int_t (bsd->GoodSpillFlag)) == 1 )
      eveBeam += 1;
    if ( ND280OffFlag == 0 && BeamSummaryDataStatus == 1 && (Int_t (bsd->GoodSpillFlag)) == 1 )
      eveDQ += 1;

  }///end loop entries


  std::cout << "------------------------------------------------------" << std::endl;
  std::cout << ">>>> entries            : " << entries << std::endl;
  std::cout << ">>>> Starting RunID     : " << RunID << std::endl;
  std::cout << ">>>> Starting EventTime : " << EventTime << std::endl;
  std::cout << ">>>> Total POTs         : " << POTs << std::endl;
  std::cout << "------------------------------------------------------" << std::endl;

  return;
}

