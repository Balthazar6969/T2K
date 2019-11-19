#include <cmath>
#include <fstream>
#include <iostream>

#include <TF1.h>
#include <TFile.h>
#include <TGraph.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TVector3.h>

#include "P0DNuMuUtils.hxx"

using namespace std;

P0DELossCorr::P0DELossCorr(bool waterInFlag)
    : fEcalRangeGraph(0), fEcalRangeInverseGraph(0), fTargetRangeGraph(0),
      fTargetRangeInverseGraph(0), fEcalFCN(0), fTargetFCN(0) {
  Initialize(waterInFlag);
}

P0DELossCorr::~P0DELossCorr() { UnInitialize(); }

void P0DELossCorr::UnInitialize() {
  if (fEcalRangeGraph)
    delete fEcalRangeGraph;
  if (fEcalRangeInverseGraph)
    delete fEcalRangeInverseGraph;
  if (fTargetRangeGraph)
    delete fTargetRangeGraph;
  if (fTargetRangeInverseGraph)
    delete fTargetRangeInverseGraph;
  if (fEcalFCN)
    delete fEcalFCN;
  if (fTargetFCN)
    delete fTargetFCN;
}

void P0DELossCorr::Initialize(bool waterInFlag) {
  UnInitialize();
  fZCentralECAL = 303.71;
  fWaterTargetZ = 1741;
  fEcalGap1 = 20;
  fWaterTargetGap = 40;
  fEcalGap2 = 18;

  fP0DEnd_Z = -939.;
  fP0DCEStart_Z = fP0DEnd_Z - fZCentralECAL;
  fP0DUSEEnd_Z = fP0DEnd_Z - fWaterTargetZ - fWaterTargetGap - fEcalGap2 -
                 fZCentralECAL - fEcalGap1;

  waterStatus = waterInFlag;

  TDirectory *curDir = gDirectory;
  TFile *fitFile =
      new TFile(Form("%s/AnalysisTools/p0dNumuInputs/muRangeFits.root",
                     getenv("ND280ANALYSISTOOLSROOT")));
  curDir->cd();
  fEcalRangeGraph =
      (TGraph *)fitFile->Get("ecal_Range")->Clone("fEcalRangeGraph");
  fEcalRangeInverseGraph = (TGraph *)fitFile->Get("ecal_Range_inverse")
                               ->Clone("fEcalRangeInverseGraph");
  fEcalFCN = (TF1 *)fitFile->Get("ecalFn")->Clone("fEcalFCN");
  // Decide whether we are processing water in or water out data
  if (!waterInFlag) {
    cout << "Initializing P0D Energy Loss Correction for water out..." << endl;
    fTargetRangeGraph =
        (TGraph *)fitFile->Get("air_Range")->Clone("fTargetRangeGraph");
    fTargetRangeInverseGraph = (TGraph *)fitFile->Get("air_Range_inverse")
                                   ->Clone("fTargetRangeInverseGraph");
    fTargetFCN = (TF1 *)fitFile->Get("airFn")->Clone("fTargetFCN");
  } else {
    cout << "Initializing P0D Energy Loss Correction for water in..." << endl;
    fTargetRangeGraph =
        (TGraph *)fitFile->Get("water_Range")->Clone("fTargetRangeGraph");
    fTargetRangeInverseGraph = (TGraph *)fitFile->Get("water_Range_inverse")
                                   ->Clone("fTargetRangeInverseGraph");
    fTargetFCN = (TF1 *)fitFile->Get("waterFn")->Clone("fTargetFCN");
  }
  fitFile->Close();
  delete fitFile;
}

double P0DELossCorr::Calc(double p0dTrackLength, double initMom,
                          TVector3 p0dFrontDir, bool reverse,
                          double p0dTrackZEnd) {
  // This does not apply any cuts itself.  It is assumed that cuts on P0D and
  // TPC1 used are
  // already made by the user.
  //
  // p0dTrackZEnd: Default set at -939.  Useful for stepping through nodes since
  // the node doesn't necessarily end
  // at the last layer of the P0D.

  // variables storing track length various parts of P0D
  double zTot = abs(p0dTrackLength * p0dFrontDir.Z());
  double lengthCE = 0.;
  double lengthWT = 0.;
  double lengthUSE = 0.;

  double p0dTrackZStart = p0dTrackZEnd - zTot;
  // Case 1:
  // If the p0dtrack starts upstream of the P0DWT
  if (p0dTrackZStart < fP0DUSEEnd_Z) {
    // Uses TGraphs
    // All lengths in [mm]
    // Case 1.1:
    // If the P0Dtrack ends downstream of the CE
    if (p0dTrackZEnd > fP0DCEStart_Z) {
      lengthCE = p0dTrackLength * (p0dTrackZEnd - fP0DCEStart_Z) / zTot;
      lengthWT = p0dTrackLength * (fP0DCEStart_Z - fP0DUSEEnd_Z) / zTot;
      lengthUSE = p0dTrackLength * (fP0DUSEEnd_Z - p0dTrackZStart) / zTot;
      if (reverse) {
        return fEcalRangeInverseGraph->Eval(
            fEcalRangeGraph->Eval(fTargetRangeInverseGraph->Eval(
                fTargetRangeGraph->Eval(fEcalRangeInverseGraph->Eval(
                    fEcalRangeGraph->Eval(initMom) + lengthUSE * 0.1)) +
                lengthWT * 0.1)) +
            lengthCE * 0.1);
      } else {
        return fEcalRangeInverseGraph->Eval(
            fEcalRangeGraph->Eval(fTargetRangeInverseGraph->Eval(
                fTargetRangeGraph->Eval(fEcalRangeInverseGraph->Eval(
                    fEcalRangeGraph->Eval(initMom) + lengthCE * 0.1)) +
                lengthWT * 0.1)) +
            lengthUSE * 0.1);
      }
    }
    // Case 1.2:
    // If the track ends between the end of USE and the start of CE
    else if (p0dTrackZEnd > fP0DUSEEnd_Z) {
      lengthWT = p0dTrackLength * (p0dTrackZEnd - fP0DUSEEnd_Z) / zTot;
      lengthUSE = p0dTrackLength * (fP0DUSEEnd_Z - p0dTrackZStart) / zTot;
      if (reverse) {
        return fTargetRangeInverseGraph->Eval(
            fTargetRangeGraph->Eval(fEcalRangeInverseGraph->Eval(
                fEcalRangeGraph->Eval(initMom) + lengthUSE * 0.1)) +
            lengthWT * 0.1);
      } else {
        return fEcalRangeInverseGraph->Eval(
            fEcalRangeGraph->Eval(fTargetRangeInverseGraph->Eval(
                fTargetRangeGraph->Eval(initMom) + lengthWT * 0.1)) +
            lengthUSE * 0.1);
      }
    }
    // Case 1.3: Track entirely within USE
    else {
      lengthUSE = p0dTrackLength;
      return fEcalRangeInverseGraph->Eval(fEcalRangeGraph->Eval(initMom) +
                                          lengthUSE * 0.1);
    }
  }
  // Case 2:
  // Otherwise, if the p0d track starts between the USE and CE
  else if (p0dTrackZStart < fP0DCEStart_Z) {
    // subcase 2.1: p0d track starts in WT and ends upstream CE
    if (p0dTrackZEnd > fP0DCEStart_Z) {
      lengthCE = p0dTrackLength * (p0dTrackZEnd - fP0DCEStart_Z) / zTot;
      lengthWT = p0dTrackLength * (fP0DCEStart_Z - p0dTrackZStart) / zTot;
      if (reverse) {
        return fEcalRangeInverseGraph->Eval(
            fEcalRangeGraph->Eval(fTargetRangeInverseGraph->Eval(
                fTargetRangeGraph->Eval(initMom) + lengthWT * 0.1)) +
            lengthCE * 0.1);
      } else {
        return fTargetRangeInverseGraph->Eval(
            fTargetRangeGraph->Eval(fEcalRangeInverseGraph->Eval(
                fEcalRangeGraph->Eval(initMom) + lengthCE * 0.1)) +
            lengthWT * 0.1);
      }
    }
    // subcase 2.2: p0d track starts in WT and ends in WT
    else {
      lengthWT = p0dTrackLength;
      return fTargetRangeInverseGraph->Eval(fTargetRangeGraph->Eval(initMom) +
                                            lengthWT * 0.1);
    }
  }
  // Case 3: Track entirely in CE
  else {
    lengthCE = p0dTrackLength;
    return fEcalRangeInverseGraph->Eval(fEcalRangeGraph->Eval(initMom) +
                                        lengthCE * 0.1);
  }
}

// Wrapper for calc when we want to set the end position to be default fP0DEnd_Z
double P0DELossCorr::Eval(double p0dTrackLength, double tpcMom,
                          TVector3 p0dFrontDir, bool reverse) {
  return P0DELossCorr::Calc(p0dTrackLength, tpcMom, p0dFrontDir, reverse,
                            fP0DEnd_Z);
}

double P0DELossCorr::Eval(ND::TGlobalReconModule::TGlobalPID *globalTrack) {
  // Checks that TPC1 and P0D detectors are used then calculates P0DELoss
  if (globalTrack->DetectorUsed[0] && globalTrack->DetectorUsed[5] &&
      globalTrack->TPC->GetEntriesFast() > 0 &&
      globalTrack->P0D->GetEntriesFast() > 0) {
    ND::TGlobalReconModule::TTPCObject *tpc1Track =
        (ND::TGlobalReconModule::TTPCObject *)globalTrack->TPC->At(0);
    ND::TGlobalReconModule::TP0DObject *p0dTrack =
        (ND::TGlobalReconModule::TP0DObject *)globalTrack->P0D->At(0);

    return P0DELossCorr::Eval(p0dTrack->Length, tpc1Track->FrontMomentum,
                              p0dTrack->FrontDirection);
  } else {
    // If TPC1 or P0D not used, return front momentum from global recon.
    return globalTrack->FrontMomentum;
  }
}
