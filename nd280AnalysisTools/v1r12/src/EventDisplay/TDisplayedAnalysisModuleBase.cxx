#include "TDisplayedAnalysisModuleBase.hxx"
#include "TChain.h"
#include "TEveManager.h"
#include "TEveTrack.h"
#include "TEveVSDStructs.h"
#include "TGLViewer.h"
#include "TND280MagField.hxx"
#include "TOstreamOverloads.hxx"
unsigned int TDisplayedAnalysisModuleBase::indent = 0;
bool TDisplayedAnalysisModuleBase::fIsMC = kFALSE;
TDisplayedAnalysisModuleBase::TDisplayedAnalysisModuleBase(TString ChainName,
                                                           TString TrackName) {
  fChain = new TChain(ChainName);
  fChain->SetBranchAddress("RunID", &fRunID);

  fVertexChain = NULL;
  fKchainOK = kTRUE;
  fTracks = new TClonesArray(TrackName, 1000);
  fVertices = NULL;
  fShowers = NULL;
  fParticles = NULL;
}
bool TDisplayedAnalysisModuleBase::InSelectedBunch(float tTrack, int bunch) {
  if (bunch == -2) return true;
  double sigma = 15.;
  double NSigmas = 4.0;
  double time = GetTimeOnBunch(bunch, fRunID, fIsMC);
  return (fabs(tTrack - time) < sigma * NSigmas);
}

TDisplayedAnalysisModuleBase::TDisplayedAnalysisModuleBase(
    TString ChainName, TString TrackName, TString VertexName,
    TString ShowerName, TString ParticleName) {
  fChain = new TChain(ChainName);
  fChain->SetBranchAddress("RunID", &fRunID);
  fVertexChain = NULL;
  fKchainOK = kTRUE;
  fTracks = new TClonesArray(TrackName, 1000);
  if (VertexName.Contains("Vertex")) {
    fVertices = new TClonesArray(VertexName, 1000);
  } else{
    std::cout << "[WARNING]: Set Vertices TClonesArray (ClassName=\""
      << VertexName << "\") to NULL for chain: " << ChainName << std::endl;
    fVertices = NULL;
  }
  if (ShowerName.Contains("Shower")){
    fShowers = new TClonesArray(ShowerName, 1000);
  }
  else{
    std::cout << "[WARNING]: Set Showers TClonesArray (ClassName=\""
      << ShowerName << "\") to NULL for chain: " << ChainName << std::endl;
    fShowers = NULL;
  }
  if (ParticleName.Contains("Particle")){
    fParticles = new TClonesArray(ParticleName, 1000);
  }
  else{
    std::cout << "[WARNING]: Set Particles TClonesArray (ClassName=\""
      << ParticleName << "\") to NULL for chain: " << ChainName << std::endl;
    fParticles = NULL;
  }
}
TDisplayedAnalysisModuleBase::TDisplayedAnalysisModuleBase(
    TString TrackChainName, TString VertexChainName, TString TrackName,
    TString VertexName) {
  fChain = new TChain(TrackChainName);
  fChain->SetBranchAddress("RunID", &fRunID);
  fKchainOK = kTRUE;
  fTracks = new TClonesArray(TrackName, 1000);
  fVertexChain = new TChain(VertexChainName);
  if (VertexName != TString("unknown")) {
    fVertices = new TClonesArray(VertexName, 1000);
  } else {
    fVertices = NULL;
  }
}
TND280TrackList *TDisplayedAnalysisModuleBase::PrepareToDraw(int entry,
                                                             TString EveName) {
  if (fChain->GetEntry(entry) == 0) {
    cout << " failed to get entry " << entry << " in " << EveName << endl;
    return NULL;
  };
  if (fVertexChain != NULL) fVertexChain->GetEntry(entry);
  return defaultTrackList(EveName);
}
TND280TrackList *TDisplayedAnalysisModuleBase::defaultTrackList(
    TString EveName) {
  TND280TrackList *gTrackList = new TND280TrackList(EveName);
  gTrackList->SetMainColor(6);
  gTrackList->SetMarkerColor(kYellow);
  gTrackList->SetMarkerStyle(4);
  gTrackList->SetMarkerSize(0.5);
  gTrackList->SetLineWidth(2);
  TND280TrackPropagator *trkProp =
      new TND280TrackPropagator("TND280TrackPropagator", "", NULL, true);
  gTrackList->SetPropagator(trkProp);
  trkProp->SetFitDaughters(kFALSE);
  trkProp->SetMaxZ(4000);
  trkProp->SetMaxR(4000);
  trkProp->SetStepper(TND280TrackPropagator::kHelix);
  if (fRunID == 3195 || fRunID == 3196 || fRunID == 4178 || fRunID == 4180 ||
      fRunID == 4184 || (fRunID >= 4195 && fRunID <= 4197) || fRunID == 4863 ||
      fRunID == 4902 || fRunID == 4903 || fRunID == 4929 || fRunID == 7714 ||
      (fRunID >= 8037 && fRunID <= 8039) || fRunID == 8055 || fRunID == 8183 ||
      (fRunID >= 8234 && fRunID <= 8303) ||
      (fRunID >= 8764 && fRunID <= 8771)) {
    trkProp->SetMagField(0);
  } else {
    TND280MagField *M = new TND280MagField();
    trkProp->SetMagFieldObj(M);
  }
  return gTrackList;
}

Color_t TDisplayedAnalysisModuleBase::ColorBasedOnPDGValue(int PDG) {
  switch (abs(PDG)) {
    case 22:
      return 9;
    case 13:
      return 3;
    case 11:
      return 50;
    case 12:
      return 46;
    case 14:
      return 8;
    case 111:
      return 42;
    case 211:
      return 43;
    case 2112:
      return 6;
    case 2212:
      return 4;
  }
  return 10;
}
TEveVectorD TDisplayedAnalysisModuleBase::Dir3MagToEve(double magnitude,
                                                       TVector3 Direction) {
  double x = magnitude * Direction.X();
  double y = magnitude * Direction.Y();
  double z = magnitude * Direction.Z();
  return TEveVectorD(x, y, z);
}

TString TDisplayedAnalysisModuleBase::DetectorName(int i) {
  switch (i) {
    case 0:
      return TString("FGD1");
    case 1:
      return TString("FGD2");
    case 2:
      return TString("P0D");
    case 3:
      return TString("DsECal");
    case 4:
      return TString("BrlECal");
    case 5:
      return TString("P0DEcal");
    case 6:
      return TString("TPC1");
    case 7:
      return TString("TPC2");
    case 8:
      return TString("TPC3");
    case 9:
      return TString("MRD");
    case 10:
      return TString("OffAxis");
    case 11:
      return TString("INGRID");
    case 12:
      return TString("Cavern");
  }
  return TString("(unknown)");
}
void TDisplayedAnalysisModuleBase::Centre(ND::TSubBaseObject *RO) {
  float x = RO->FrontPosition.X();
  float y = RO->FrontPosition.Y();
  float z = RO->FrontPosition.Z();
  gEve->GetDefaultGLViewer()->CurrentCamera().SetCenterVec(x, y, z);
}
void TDisplayedAnalysisModuleBase::Centre(TLorentzVector c) {
  float x = c.X();
  float y = c.Y();
  float z = c.Z();
  gEve->GetDefaultGLViewer()->CurrentCamera().SetCenterVec(x, y, z);
}
void TDisplayedAnalysisModuleBase::Centre(TLorentzVector f, TLorentzVector b) {
  if (inND280(f)) {
    if (inND280(b)) {
      float x = (f.X() + b.X()) / 2.0;
      float y = (f.Y() + b.Y()) / 2.0;
      float z = (f.Z() + b.Z()) / 2.0;
      gEve->GetDefaultGLViewer()->CurrentCamera().SetCenterVec(x, y, z);
    } else
      Centre(f);
  }
}
void TDisplayedAnalysisModuleBase::Centre(TVector3 c) {
  float x = c.X();
  float y = c.Y();
  float z = c.Z();
  gEve->GetDefaultGLViewer()->CurrentCamera().SetCenterVec(x, y, z);
}
bool TDisplayedAnalysisModuleBase::inND280(TVector3 c) {
  if (fabs(c.Z()) < 4000 && fabs(c.Y()) < 3000 && fabs(c.X()) < 3000)
    return true;
  return false;
}
bool TDisplayedAnalysisModuleBase::inND280(TLorentzVector c) {
  return inND280(c.Vect());
}
TString TDisplayedAnalysisModuleBase::DetectorUsedName(int i) {
  switch (i) {
    case 0:
      return TString("TPC1");
    case 1:
      return TString("TPC2");
    case 2:
      return TString("TPC3");
    case 3:
      return TString("FGD1");
    case 4:
      return TString("FGD2");
    case 5:
      return TString("P0D");
    case 6:
      return TString("Downstream ECal");
    case 7:
      return TString("Top SMRD");
    case 8:
      return TString("Bottom SMRD");
    case 9:
      return TString("Left SMRD");
    case 10:
      return TString("Right SMRD");
    case 11:
      return TString("Top P0D ECal");
    case 12:
      return TString("Bottom P0D ECal");
    case 13:
      return TString("Left P0D ECal");
    case 14:
      return TString("Right P0D ECal");
    case 15:
      return TString("Top Tracker  ECal");
    case 16:
      return TString("Bottom Tracker ECal");
    case 17:
      return TString("Left Tracker ECal");
    case 18:
      return TString("Right Tracker ECal");
  }
  return TString("(unknown)");
}

const double TDisplayedAnalysisModuleBase::time_mc[NBUNCH] = {
    2750.2, 3332.0, 3914.7, 4497.0, 5078.4, 5659.7, 6243.4, 6824.2};

const double TDisplayedAnalysisModuleBase::time_data_run1[NBUNCH] = {
    2839.7, 3423.5, 4005.4, 4588.6, 5172.2, 5754.6, -1000., -1000.};

const double TDisplayedAnalysisModuleBase::time_data_run2[NBUNCH] = {
    2853.95, 3444.15, 4030.41, 4620.34, 5180.28, 5770.12, 6343.77, 6924.67};

const double TDisplayedAnalysisModuleBase::time_data_run3[NBUNCH] = {
    3019.11, 3597.74, 4180.73, 4763.93, 5346.49, 5927.83, 6508.5, 7093.56};

const double TDisplayedAnalysisModuleBase::time_data_run4[NBUNCH] = {
    3039.11, 3617.74, 4200.73, 4783.93, 5366.49, 5957.83, 6528.5, 7113.56};

int TDisplayedAnalysisModuleBase::GetBunchMCTrue(double tTime) {
  int bunch = -1;
  const double sigma = 15.;
  // ---- For MC truth -----------
  for (int i = 0; i < NBUNCH; i++) {
    double time = 60. + 582. * (double)i;
    if (fabs(tTime - time) < sigma * 5.) {
      bunch = i;
      break;
    }
  }

  return bunch;
}

int TDisplayedAnalysisModuleBase::GetBunch(double tTrack, int run, bool isMC,
                                           double sigma, double NSigmas) {
  //**********************************************************
  int bunch = -1;
  //  double sigma;
  double time;

  for (int i = 0; i < NBUNCH; i++) {
    if (isMC) {
      time = time_mc[i];
      //      sigma = 15.;
    } else {
      if (run < 6000)
        time = time_data_run1[i];
      else if (run < 7000)
        time = time_data_run2[i];
      else if (run < 8000)
        time = time_data_run3[i];
      else
        time = time_data_run4[i];
      //      sigma = 15.;
    }
    if (time < 0) return bunch;

    if (fabs(tTrack - time) < sigma * NSigmas) {
      bunch = i;
      break;
    }
  }
  return bunch;
}

double TDisplayedAnalysisModuleBase::GetTimeOnBunch(int ibunch, int run,
                                                    bool isMC) {
  if (isMC) {
    return time_mc[ibunch];
  } else {
    if (run < 6000)
      return time_data_run1[ibunch];
    else if (run < 7000)
      return time_data_run2[ibunch];
    else if (run < 7000)
      return time_data_run3[ibunch];
    else
      return time_data_run4[ibunch];
  }
}
