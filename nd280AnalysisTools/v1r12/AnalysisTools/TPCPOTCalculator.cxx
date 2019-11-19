#include "TPCPOTCalculator.hxx"

using std::cout;
using std::endl;

void TPCPOTCalculator(char *inputfilename, int max_entries) {
  gROOT->SetBatch(1);
  // See commented code below for loading libraries automatically.
  //#if !defined(__MAKECINT__)
  // This should not be seen when using ACLiC.
  //  gROOT->ProcessLine(".x oaAnalysisReadInitFile.C");
  //#endif
  // _________________________________________________________________________
  // Prepare to read trees and then read trees.
  //
  std::cout << "Adding file(s) matching string: " << inputfilename << std::endl;

  //
  // BasicHeader
  //
  TChain BasicHeader("HeaderDir/BasicHeader");
  BasicHeader.Add(inputfilename);
  Int_t EventID;
  Int_t RunID;
  UInt_t EventTime;
  Bool_t IsMC;
  Float_t POTPerSpill;
  BasicHeader.SetBranchAddress("EventTime", &EventTime);
  BasicHeader.SetBranchAddress("EventID", &EventID);
  BasicHeader.SetBranchAddress("RunID", &RunID);
  BasicHeader.SetBranchAddress("IsMC", &IsMC);
  BasicHeader.SetBranchAddress("POTPerSpill", &POTPerSpill);

  //
  // BasicDataQuality
  //
  TChain BasicDataQuality("HeaderDir/BasicDataQuality");
  BasicDataQuality.Add(inputfilename);
  Int_t DQEventID;
  Int_t DQRunID;
  Int_t ND280OffFlag;
  Int_t TPC1Flag;
  Int_t TPC2Flag;
  Int_t TPC3Flag;
  BasicDataQuality.SetBranchAddress("EventID", &DQEventID);
  BasicDataQuality.SetBranchAddress("RunID", &DQRunID);
  BasicDataQuality.SetBranchAddress("ND280OffFlag", &ND280OffFlag);
  BasicDataQuality.SetBranchAddress("TPC1Flag", &TPC1Flag);
  BasicDataQuality.SetBranchAddress("TPC2Flag", &TPC2Flag);
  BasicDataQuality.SetBranchAddress("TPC3Flag", &TPC3Flag);

  //
  // BeamSummaryData
  //
  TChain BeamSummaryData("HeaderDir/BeamSummaryData");
  BeamSummaryData.Add(inputfilename);
  Int_t ND280Spill;
  Int_t BRunID;
  Int_t BEventID;
  Int_t BeamSummaryDataStatus;

  BeamSummaryData.SetBranchAddress("RunID", &BRunID);
  BeamSummaryData.SetBranchAddress("EventID", &BEventID);
  BeamSummaryData.SetBranchAddress("BeamSummaryDataStatus",
                                   &BeamSummaryDataStatus);
  BeamSummaryData.SetBranchAddress("ND280Spill", &ND280Spill);

  TClonesArray *TClonesBSD =
      new TClonesArray("ND::TBeamSummaryDataModule::TBeamSummaryData");
  BeamSummaryData.SetBranchAddress("BeamSummaryData", &TClonesBSD);
  ND::TBeamSummaryDataModule::TBeamSummaryData *bsd =
      (ND::TBeamSummaryDataModule::TBeamSummaryData *)(*TClonesBSD)[0];

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
  if (max_entries > 0 && max_entries < entries)
    entries = max_entries;

  double TPC_POT[3] = {0, 0, 0};
  double TPC_POT_GoodSpill[3] = {0, 0, 0};

  int eveBeam = 0;
  int eveDQ = 0;
  int eveTot = 0;

  for (int i = 0; i < entries; i++) {

    BasicHeader.GetEntry(i);
    BasicDataQuality.GetEntry(i);
    BeamSummaryData.GetEntry(i);
    if (RunID != BRunID || EventID != BEventID || RunID != DQRunID ||
        EventID != DQEventID) {
      std::cout << "Runs and Event # mismatch." << std::endl;
      return;
    }

    // Count POT in each TPC
    if (IsMC) {
      for (int j = 0; j < 3; j++) {
        TPC_POT[j] += POTPerSpill;
        TPC_POT_GoodSpill[j] += POTPerSpill;
      }
    } else {
      if (ND280OffFlag == 0 && BeamSummaryDataStatus == 1) {
        if (TPC1Flag == 0) {
          TPC_POT[0] += bsd->CT5ProtonsPerSpill;
          if ((Int_t(bsd->GoodSpillFlag)) == 1)
            TPC_POT_GoodSpill[0] += bsd->CT5ProtonsPerSpill;
        }
        if (TPC2Flag == 0) {
          TPC_POT[1] += bsd->CT5ProtonsPerSpill;
          if ((Int_t(bsd->GoodSpillFlag)) == 1)
            TPC_POT_GoodSpill[1] += bsd->CT5ProtonsPerSpill;
        }
        if (TPC3Flag == 0) {
          TPC_POT[2] += bsd->CT5ProtonsPerSpill;
          if ((Int_t(bsd->GoodSpillFlag)) == 1)
            TPC_POT_GoodSpill[2] += bsd->CT5ProtonsPerSpill;
        }
      }
    }

    if (ND280OffFlag == 0 && BeamSummaryDataStatus == 1 &&
        (Int_t(bsd->GoodSpillFlag)) == 1)
      eveTot += 1;
    if (ND280OffFlag == 0 && BeamSummaryDataStatus == 1 &&
        (Int_t(bsd->GoodSpillFlag)) == 1)
      eveBeam += 1;
    if (ND280OffFlag == 0 && BeamSummaryDataStatus == 1 &&
        (Int_t(bsd->GoodSpillFlag)) == 1)
      eveDQ += 1;

  } /// end loop entries

  std::cout << "                           ************TOTAL ACCEPTED "
               "POT************** "
            << std::endl;
  for (int i = 0; i < 3; i++) {
    std::cout << "                           ---------------> IN TPC" << i + 1
              << ": " << TPC_POT[i] << std::endl;
    if (!IsMC)
      std::cout << "                           -> with good spill flags: "
                << TPC_POT_GoodSpill[i] << std::endl;
  }
  std::cout << "                           "
               "*******************************************"
            << std::endl;

  return;
}
