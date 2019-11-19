#include "TDirectory.h"
#include "TFile.h"
#include "TROOT.h"
#include "TTrackerAnalysisUtils.hxx"

TTrackerAnalysisUtils::TTrackerAnalysisUtils() {
  ftpcindex = 9;
  // These are the dimensions of the FGD1 active volume.
  // fFGD1ActiveMin.SetXYZ(-931.6, -906.6, 170.3);
  // fFGD1ActiveMax.SetXYZ(931.6, 956.6, 490.5);
  fFGD1ActiveMin.SetXYZ(-931.6, -906.6, 119.2);
  fFGD1ActiveMax.SetXYZ(931.6, 956.6, 479.6);

  // These are the dimensions of the FGD2 active volume.
  //    fFGD2ActiveMin.SetXYZ(-931.6, -906.6, 1528.6);
  // fFGD2ActiveMax.SetXYZ(931.6, 956.6, 1850.2);
  fFGD2ActiveMin.SetXYZ(-931.6, -906.6, 1439.7);
  fFGD2ActiveMax.SetXYZ(931.6, 956.6, 1835.6);

  //    fgd_fiducial_trim.SetXYZ(200.0,200.0,10.0);
  //    DefineFiducialVolume(fgd_fiducial_trim);
}

/// Figure out if position is inside volume defined by minpos and maxpos.
/// Assumes we are dealing with a box that is perfectly aligned with
/// the coordinate system.
bool TTrackerAnalysisUtils::IsInVolume(TVector3 pos, TVector3 minpos,
                                       TVector3 maxpos) {

  if (pos[0] >= minpos[0] && pos[1] >= minpos[1] && pos[2] >= minpos[2] &&
      pos[0] <= maxpos[0] && pos[1] <= maxpos[1] && pos[2] <= maxpos[2]) {
    return true;
  }
  return false;
}
void TTrackerAnalysisUtils::DefineFiducialVolume(TVector3 dist) {
  fFGD1FiducialMin.SetXYZ(0, 0, 0);
  fFGD1FiducialMax.SetXYZ(0, 0, 0);
  for (int jj = 0; jj < 3; jj++) {
    fFGD1FiducialMin[jj] = fFGD1ActiveMin[jj] + dist[jj];
    fFGD1FiducialMax[jj] = fFGD1ActiveMax[jj] - dist[jj];

    fFGD2FiducialMin[jj] = fFGD2ActiveMin[jj] + dist[jj];
    fFGD2FiducialMax[jj] = fFGD2ActiveMax[jj] - dist[jj];
  }
}
void TTrackerAnalysisUtils::DefineFiducialVolume(double dx, double dy,
                                                 double dz) {
  TVector3 kfgd_fiducial_trim(dx, dy, dz);
  fFGD1FiducialMin = fFGD1ActiveMin + kfgd_fiducial_trim;
  fFGD1FiducialMax = fFGD1ActiveMax - kfgd_fiducial_trim;
  fFGD2FiducialMin = fFGD2ActiveMin + kfgd_fiducial_trim;
  fFGD2FiducialMax = fFGD2ActiveMax - kfgd_fiducial_trim;
}
void TTrackerAnalysisUtils::DefineFiducialVolume(double xmin, double xmax,
                                                 double ymin, double ymax,
                                                 double zmin, double zmax) {
  TVector3 kfgd_fiducial_trim_min(xmin, ymin, zmin);
  TVector3 kfgd_fiducial_trim_max(xmax, ymax, zmax);
  for (int jj = 0; jj < 3; jj++) {
    fFGD1FiducialMin[jj] = fFGD1ActiveMin[jj] + kfgd_fiducial_trim_min[jj];
    fFGD1FiducialMax[jj] = fFGD1ActiveMax[jj] - kfgd_fiducial_trim_max[jj];

    fFGD2FiducialMin[jj] = fFGD2ActiveMin[jj] + kfgd_fiducial_trim_min[jj];
    fFGD2FiducialMax[jj] = fFGD2ActiveMax[jj] - kfgd_fiducial_trim_max[jj];
  }
}
bool TTrackerAnalysisUtils::IsInFiducialVolume(TVector3 pos) {
  bool fiducial = false;
  bool isfgd1 = IsInVolume(pos, fFGD1FiducialMin, fFGD1FiducialMax);
  bool isfgd2 = IsInVolume(pos, fFGD2FiducialMin, fFGD2FiducialMax);
  if (isfgd1 || isfgd2) {
    fiducial = true;
  }
  return fiducial;
}

// Is position in Fiducial FGD1?
bool TTrackerAnalysisUtils::IsInFiducialFGD1(TVector3 pos, TVector3 fgd_trim) {
  TVector3 FGD1FiducialMin = fFGD1ActiveMin + fgd_trim;
  TVector3 FGD1FiducialMax = fFGD1ActiveMax - fgd_trim;

  bool retval = IsInVolume(pos, FGD1FiducialMin, FGD1FiducialMax);
  if (retval) {
    if (fabs(pos[0]) < 0.0001 && fabs(pos[1]) < 0.0001 &&
        fabs(pos[2]) < 0.001) {
      std::cout << "IsInFiducialFGD1 0,0,0 is inside of min=("
                << FGD1FiducialMin[0] << "," << FGD1FiducialMin[1] << ","
                << FGD1FiducialMin[2] << ") max=(" << FGD1FiducialMax[0] << ","
                << FGD1FiducialMax[1] << "," << FGD1FiducialMax[2] << ")"
                << std::endl;
    }
  }

  return retval;
}

bool TTrackerAnalysisUtils::IsInFiducialFGD1(TLorentzVector pos,
                                             TVector3 fgd_trim) {
  TVector3 tmp_pos(pos.X(), pos.Y(), pos.Z());
  return IsInFiducialFGD1(tmp_pos, fgd_trim);
}

bool TTrackerAnalysisUtils::IsInFiducialFGD1(TLorentzVector pos) {
  TVector3 tmp_pos(pos.X(), pos.Y(), pos.Z());
  return IsInFiducialFGD1(tmp_pos);
}

bool TTrackerAnalysisUtils::IsInFiducialFGD1(TVector3 pos) {
  return IsInVolume(pos, fFGD1FiducialMin, fFGD1FiducialMax);
}

bool TTrackerAnalysisUtils::IsInFGD1(TLorentzVector pos) {
  TVector3 tmp_pos(pos.X(), pos.Y(), pos.Z());
  return IsInVolume(tmp_pos, fFGD1ActiveMin, fFGD1ActiveMax);
}
bool TTrackerAnalysisUtils::IsInFGD2(TLorentzVector pos) {
  TVector3 tmp_pos(pos.X(), pos.Y(), pos.Z());
  return IsInVolume(tmp_pos, fFGD2ActiveMin, fFGD2ActiveMax);
}

bool TTrackerAnalysisUtils::IsInFGD1(TVector3 pos) {
  return IsInVolume(pos, fFGD1ActiveMin, fFGD1ActiveMax);
}
bool TTrackerAnalysisUtils::IsInFGD2(TVector3 pos) {
  return IsInVolume(pos, fFGD2ActiveMin, fFGD2ActiveMax);
}

// Is position in Fiducial FGD2?
bool TTrackerAnalysisUtils::IsInFiducialFGD2(TVector3 pos, TVector3 fgd_trim) {
  TVector3 FGD2FiducialMin = fFGD2ActiveMin + fgd_trim;
  TVector3 FGD2FiducialMax = fFGD2ActiveMax - fgd_trim;

  bool retval = IsInVolume(pos, FGD2FiducialMin, FGD2FiducialMax);
  if (retval) {
    if (fabs(pos[0]) < 0.0001 && fabs(pos[1]) < 0.0001 &&
        fabs(pos[2]) < 0.001) {
      std::cout << "IsInFiducialFGD2 0,0,0 is inside of min=("
                << FGD2FiducialMin[0] << "," << FGD2FiducialMin[1] << ","
                << FGD2FiducialMin[2] << ") max=(" << FGD2FiducialMax[0] << ","
                << FGD2FiducialMax[1] << "," << FGD2FiducialMax[2] << ")"
                << std::endl;
    }
  }

  return retval;
}

bool TTrackerAnalysisUtils::IsInFiducialFGD2(TLorentzVector pos,
                                             TVector3 fgd_trim) {
  TVector3 tmp_pos(pos.X(), pos.Y(), pos.Z());
  return IsInFiducialFGD2(tmp_pos, fgd_trim);
}

bool TTrackerAnalysisUtils::IsInFiducialFGD2(TVector3 pos) {
  return IsInVolume(pos, fFGD2FiducialMin, fFGD2FiducialMax);
}

bool TTrackerAnalysisUtils::IsInFiducialFGD2(TLorentzVector pos) {
  TVector3 tmp_pos(pos.X(), pos.Y(), pos.Z());
  return IsInFiducialFGD2(tmp_pos);
}

// ________________________________________________________________________
// Track helper routines
int TTrackerAnalysisUtils::GetTrackTpcCharge(
    ND::TTrackerReconModule::TTrackerResult *track) {
  return int(track->Charge);
}

double TTrackerAnalysisUtils::GetTrackTpcQuality(
    ND::TTrackerReconModule::TTrackerResult *track) {
  return track->Quality;
}

// Return the TPC momentum.   Just use first TPC momentum measurement we find.
double TTrackerAnalysisUtils::GetTrackMomentum(
    ND::TTrackerReconModule::TTrackerResult *track) {
  // return the momentum with smallest mom_err/mom<momcut
  return track->Momentum;
}

// Return the TPC momentum. Return the value for which the quality is minimum
double TTrackerAnalysisUtils::GetTrackMomentumError(
    ND::TTrackerReconModule::TTrackerResult *track) {
  return track->MomentumError;
}

double TTrackerAnalysisUtils::GetTrackTpcPid(
    ND::TTrackerReconModule::TTrackerResult *track, std::string parName) {
  // look for TPC track with lowest relative momentum error, and return the Pid
  // weight for parName for that track
  double momerr = 99999.0;
  double chi2 = 99999.0;
  double errmin = 999999.0;
  ftpcindex = -1;

  TClonesArray *TPC = track->TPC;
  for (int itpc = 0; itpc < track->NTPCs; itpc++) {
    ND::TTrackerReconModule::TTPCTrack *tpctrack =
        (ND::TTrackerReconModule::TTPCTrack *)(*TPC)[itpc];
    // if (tpctrack->Detector != tpcindex ) continue;

    if (tpctrack->Momentum != 0) {
      chi2 = tpctrack->Chi2;
      momerr = tpctrack->MomentumError / tpctrack->Momentum;
    }
    if (momerr < errmin) {
      errmin = momerr;
      ftpcindex = itpc;
    }
  }

  if (ftpcindex != -1) {
    ND::TTrackerReconModule::TTPCTrack *tpctrack =
        (ND::TTrackerReconModule::TTPCTrack *)(*TPC)[ftpcindex];
    if (parName.compare("Electron") == 0)
      if (tpctrack->PullEle != 999999.0)
        return tpctrack->PullEle;
    if (parName.compare("Muon") == 0)
      if (tpctrack->PullMuon != 999999.0)
        return tpctrack->PullMuon;
    if (parName.compare("Proton") == 0)
      if (tpctrack->PullProton != 999999.0)
        return tpctrack->PullProton;
    if (parName.compare("Pion") == 0)
      if (tpctrack->PullPion != 999999.0)
        return tpctrack->PullPion;
    if (parName.compare("Kaon") == 0)
      if (tpctrack->PullKaon != 999999.0)
        return tpctrack->PullKaon;
  }
  return 999999;
}

double TTrackerAnalysisUtils::GetTrackTpcPid(
    ND::TTrackerReconModule::TTrackerResult *track, std::string parName,
    int tpcindex) {
  // look for TPC track with lowest relative momentum error, and return the Pid
  // weight for parName for that track
  // give the pid corresponding to the tpc tpcindex

  double momerr = 99999.0;
  double chi2 = 99999.0;
  double errmin = 999999.0;
  ftpcindex = -1;

  TClonesArray *TPC = track->TPC;
  for (int itpc = 0; itpc < track->NTPCs; itpc++) {
    ND::TTrackerReconModule::TTPCTrack *tpctrack =
        (ND::TTrackerReconModule::TTPCTrack *)(*TPC)[itpc];
    if (tpctrack->Detector != tpcindex)
      continue;

    if (tpctrack->Momentum != 0) {
      chi2 = tpctrack->Chi2;
      momerr = tpctrack->MomentumError / tpctrack->Momentum;
    }
    if (momerr < errmin) {
      errmin = momerr;
      ftpcindex = itpc;
    }
  }

  if (ftpcindex != -1) {
    ND::TTrackerReconModule::TTPCTrack *tpctrack =
        (ND::TTrackerReconModule::TTPCTrack *)(*TPC)[ftpcindex];
    if (parName.compare("Electron") == 0)
      if (tpctrack->PullEle != 999999.0)
        return tpctrack->PullEle;
    if (parName.compare("Muon") == 0)
      if (tpctrack->PullMuon != 999999.0)
        return tpctrack->PullMuon;
    if (parName.compare("Proton") == 0)
      if (tpctrack->PullProton != 999999.0)
        return tpctrack->PullProton;
    if (parName.compare("Pion") == 0)
      if (tpctrack->PullPion != 999999.0)
        return tpctrack->PullPion;
    if (parName.compare("Kaon") == 0)
      if (tpctrack->PullKaon != 999999.0)
        return tpctrack->PullKaon;
  }
  return 999999;
}

double TTrackerAnalysisUtils::GetTrackTpcPidSigma(
    ND::TTrackerReconModule::TTrackerResult *track, std::string parName) {
  // look for TPC track with lowest relative momentum error, and return the Pid
  // sigma for parName for that track

  double momerr = 99999.0;
  double chi2 = 99999.0;
  double errmin = 999999.0;
  ftpcindex = -1;

  TClonesArray *TPC = track->TPC;
  for (int itpc = 0; itpc < track->NTPCs; itpc++) {
    ND::TTrackerReconModule::TTPCTrack *tpctrack =
        (ND::TTrackerReconModule::TTPCTrack *)(*TPC)[itpc];

    if (tpctrack->Momentum != 0) {
      chi2 = tpctrack->Chi2;
      momerr = tpctrack->MomentumError / tpctrack->Momentum;
    }
    if (momerr < errmin) {
      errmin = momerr;
      ftpcindex = itpc;
    }
  }

  if (ftpcindex != -1) {
    ND::TTrackerReconModule::TTPCTrack *tpctrack =
        (ND::TTrackerReconModule::TTPCTrack *)(*TPC)[ftpcindex];
    if (parName.compare("Electron") == 0)
      if (tpctrack->SigmaEle != 999999.0)
        return tpctrack->SigmaEle;
    if (parName.compare("Muon") == 0)
      if (tpctrack->SigmaMuon != 999999.0)
        return tpctrack->SigmaMuon;
    if (parName.compare("Proton") == 0)
      if (tpctrack->SigmaProton != 999999.0)
        return tpctrack->SigmaProton;
    if (parName.compare("Pion") == 0)
      if (tpctrack->SigmaPion != 999999.0)
        return tpctrack->SigmaPion;
    if (parName.compare("Kaon") == 0)
      if (tpctrack->SigmaKaon != 999999.0)
        return tpctrack->SigmaKaon;
  }
  return 0.0;
}

double TTrackerAnalysisUtils::GetTrackTpcPidSigma(
    ND::TTrackerReconModule::TTrackerResult *track, std::string parName,
    int tpcindex) {
  // look for TPC track with lowest relative momentum error, and return the Pid
  // sigma for parName for that track
  // only look at TPC tracs in tpc tpcindex

  double momerr = 99999.0;
  double chi2 = 99999.0;
  double errmin = 999999.0;
  ftpcindex = -1;

  TClonesArray *TPC = track->TPC;
  for (int itpc = 0; itpc < track->NTPCs; itpc++) {
    ND::TTrackerReconModule::TTPCTrack *tpctrack =
        (ND::TTrackerReconModule::TTPCTrack *)(*TPC)[itpc];
    if (tpctrack->Detector != tpcindex)
      continue;

    if (tpctrack->Momentum != 0) {
      chi2 = tpctrack->Chi2;
      momerr = tpctrack->MomentumError / tpctrack->Momentum;
    }
    if (momerr < errmin) {
      errmin = momerr;
      ftpcindex = itpc;
    }
  }

  if (ftpcindex != -1) {
    ND::TTrackerReconModule::TTPCTrack *tpctrack =
        (ND::TTrackerReconModule::TTPCTrack *)(*TPC)[ftpcindex];
    if (parName.compare("Electron") == 0)
      if (tpctrack->SigmaEle != 999999.0)
        return tpctrack->SigmaEle;
    if (parName.compare("Muon") == 0)
      if (tpctrack->SigmaMuon != 999999.0)
        return tpctrack->SigmaMuon;
    if (parName.compare("Proton") == 0)
      if (tpctrack->SigmaProton != 999999.0)
        return tpctrack->SigmaProton;
    if (parName.compare("Pion") == 0)
      if (tpctrack->SigmaPion != 999999.0)
        return tpctrack->SigmaPion;
    if (parName.compare("Kaon") == 0)
      if (tpctrack->SigmaKaon != 999999.0)
        return tpctrack->SigmaKaon;
  }

  return 0.0;
}

double TTrackerAnalysisUtils::GetTrackTpcPiddEdxexp(
    ND::TTrackerReconModule::TTrackerResult *track, std::string parName) {
  // look for TPC track with lowest relative momentum error, and return the Pid
  // dEdx for parName for that track
  double momerr = 99999.0;
  double chi2 = 99999.0;
  double errmin = 999999.0;
  ftpcindex = -1;

  TClonesArray *TPC = track->TPC;
  for (int itpc = 0; itpc < track->NTPCs; itpc++) {
    ND::TTrackerReconModule::TTPCTrack *tpctrack =
        (ND::TTrackerReconModule::TTPCTrack *)(*TPC)[itpc];
    // tpctrack->SetClass("ND::TTrackerReconModule::TTPCTrack",1);

    if (tpctrack->Momentum != 0) {
      chi2 = tpctrack->Chi2;
      momerr = tpctrack->MomentumError / tpctrack->Momentum;
    }
    if (momerr < errmin) {
      errmin = momerr;
      ftpcindex = itpc;
    }
  }

  if (ftpcindex != -1) {
    ND::TTrackerReconModule::TTPCTrack *tpctrack =
        (ND::TTrackerReconModule::TTPCTrack *)(*TPC)[ftpcindex];
    // tpctrack->SetClass("ND::TTrackerReconModule::TTPCTrack",1);

    if (parName.compare("Electron") == 0)
      if (tpctrack->dEdxexpEle != 999999.0)
        return tpctrack->dEdxexpEle;
    if (parName.compare("Muon") == 0)
      if (tpctrack->dEdxexpMuon != 999999.0)
        return tpctrack->dEdxexpMuon;
    if (parName.compare("Proton") == 0)
      if (tpctrack->dEdxexpProton != 999999.0)
        return tpctrack->dEdxexpProton;
    if (parName.compare("Pion") == 0)
      if (tpctrack->dEdxexpPion != 999999.0)
        return tpctrack->dEdxexpPion;
    if (parName.compare("Kaon") == 0)
      if (tpctrack->dEdxexpKaon != 999999.0)
        return tpctrack->dEdxexpKaon;
  }

  return 0.0;
}

double TTrackerAnalysisUtils::GetTrackTpcPiddEdxexp(
    ND::TTrackerReconModule::TTrackerResult *track, std::string parName,
    int tpcindex) {
  // look for TPC track with lowest relative momentum error, and return the Pid
  // dEdx for parName for that track
  // only look at tracks in tpc tpcindex
  double momerr = 99999.0;
  double chi2 = 99999.0;
  double errmin = 999999.0;
  ftpcindex = -1;

  TClonesArray *TPC = track->TPC;
  for (int itpc = 0; itpc < track->NTPCs; itpc++) {
    ND::TTrackerReconModule::TTPCTrack *tpctrack =
        (ND::TTrackerReconModule::TTPCTrack *)(*TPC)[itpc];
    if (tpctrack->Detector != tpcindex)
      continue;

    if (tpctrack->Momentum != 0) {
      chi2 = tpctrack->Chi2;
      momerr = tpctrack->MomentumError / tpctrack->Momentum;
    }
    if (momerr < errmin) {
      errmin = momerr;
      ftpcindex = itpc;
    }
  }

  if (ftpcindex != -1) {
    ND::TTrackerReconModule::TTPCTrack *tpctrack =
        (ND::TTrackerReconModule::TTPCTrack *)(*TPC)[ftpcindex];

    if (parName.compare("Electron") == 0)
      if (tpctrack->dEdxexpEle != 999999.0)
        return tpctrack->dEdxexpEle;
    if (parName.compare("Muon") == 0)
      if (tpctrack->dEdxexpMuon != 999999.0)
        return tpctrack->dEdxexpMuon;
    if (parName.compare("Proton") == 0)
      if (tpctrack->dEdxexpProton != 999999.0)
        return tpctrack->dEdxexpProton;
    if (parName.compare("Pion") == 0)
      if (tpctrack->dEdxexpPion != 999999.0)
        return tpctrack->dEdxexpPion;
    if (parName.compare("Kaon") == 0)
      if (tpctrack->dEdxexpKaon != 999999.0)
        return tpctrack->dEdxexpKaon;
  }

  return 0.0;
}

double TTrackerAnalysisUtils::GetTrackTpcPidNTrun(
    ND::TTrackerReconModule::TTrackerResult *track) {
  // look for TPC track with lowest relative momentum error, and return the Pid
  // NTrun
  double momerr = 99999.0;
  double chi2 = 99999.0;
  double errmin = 999999.0;
  ftpcindex = -1;

  TClonesArray *TPC = track->TPC;
  for (int itpc = 0; itpc < track->NTPCs; itpc++) {
    ND::TTrackerReconModule::TTPCTrack *tpctrack =
        (ND::TTrackerReconModule::TTPCTrack *)(*TPC)[itpc];
    if (tpctrack->Momentum != 0) {
      chi2 = tpctrack->Chi2;
      momerr = tpctrack->MomentumError / tpctrack->Momentum;
    }
    if (momerr < errmin) {
      errmin = momerr;
      ftpcindex = itpc;
    }
  }
  if (ftpcindex != -1) {
    ND::TTrackerReconModule::TTPCTrack *tpctrack =
        (ND::TTrackerReconModule::TTPCTrack *)(*TPC)[ftpcindex];
    if (tpctrack->NTrun != 999999.0)
      return tpctrack->NTrun;
  }
  return 0.0;
}

double TTrackerAnalysisUtils::GetTrackTpcPidNTrun(
    ND::TTrackerReconModule::TTrackerResult *track, int tpcindex) {
  // look for TPC track with lowest relative momentum error, and return the Pid
  // NTrun
  // only look at tracks in tpc tpcindex
  double momerr = 99999.0;
  double chi2 = 99999.0;
  double errmin = 999999.0;
  ftpcindex = -1;
  TClonesArray *TPC = track->TPC;
  for (int itpc = 0; itpc < track->NTPCs; itpc++) {
    ND::TTrackerReconModule::TTPCTrack *tpctrack =
        (ND::TTrackerReconModule::TTPCTrack *)(*TPC)[itpc];
    if (tpctrack->Detector != tpcindex)
      continue;
    if (tpctrack->Momentum != 0) {
      chi2 = tpctrack->Chi2;
      momerr = tpctrack->MomentumError / tpctrack->Momentum;
    }
    if (momerr < errmin) {
      errmin = momerr;
      ftpcindex = itpc;
    }
  }
  if (ftpcindex != -1) {
    ND::TTrackerReconModule::TTPCTrack *tpctrack =
        (ND::TTrackerReconModule::TTPCTrack *)(*TPC)[ftpcindex];
    if (tpctrack->NTrun != 999999.0)
      return tpctrack->NTrun;
  }

  return 0.0;
}
double TTrackerAnalysisUtils::GetTrackTpcPidCcorr(
    ND::TTrackerReconModule::TTrackerResult *track) {
  // look for TPC track with lowest relative momentum error, and return the Pid
  // Ccorr
  double momerr = 99999.0;
  double chi2 = 99999.0;
  double errmin = 999999.0;
  ftpcindex = -1;
  TClonesArray *TPC = track->TPC;
  for (int itpc = 0; itpc < track->NTPCs; itpc++) {
    ND::TTrackerReconModule::TTPCTrack *tpctrack =
        (ND::TTrackerReconModule::TTPCTrack *)(*TPC)[itpc];
    if (tpctrack->Momentum != 0) {
      chi2 = tpctrack->Chi2;
      momerr = tpctrack->MomentumError / tpctrack->Momentum;
    }
    if (momerr < errmin) {
      errmin = momerr;
      ftpcindex = itpc;
    }
  }
  if (ftpcindex != -1) {
    ND::TTrackerReconModule::TTPCTrack *tpctrack =
        (ND::TTrackerReconModule::TTPCTrack *)(*TPC)[ftpcindex];
    if (tpctrack->Ccorr != 999999.0)
      return tpctrack->Ccorr;
  }
  return 0.0;
}

double TTrackerAnalysisUtils::GetTrackTpcPidCcorr(
    ND::TTrackerReconModule::TTrackerResult *track, int tpcindex) {
  // look for TPC track with lowest relative momentum error, and return the Pid
  // Ccorr
  // only look at tracks in tpc tpcindex
  double momerr = 99999.0;
  double chi2 = 99999.0;
  double errmin = 999999.0;
  ftpcindex = -1;
  TClonesArray *TPC = track->TPC;
  for (int itpc = 0; itpc < track->NTPCs; itpc++) {
    ND::TTrackerReconModule::TTPCTrack *tpctrack =
        (ND::TTrackerReconModule::TTPCTrack *)(*TPC)[itpc];
    // tpctrack->SetClass("ND::TTrackerReconModule::TTPCTrack",1);
    if (tpctrack->Detector != tpcindex)
      continue;
    if (tpctrack->Momentum != 0) {
      chi2 = tpctrack->Chi2;
      momerr = tpctrack->MomentumError / tpctrack->Momentum;
    }
    if (momerr < errmin) {
      errmin = momerr;
      ftpcindex = itpc;
    }
  }
  if (ftpcindex != -1) {
    ND::TTrackerReconModule::TTPCTrack *tpctrack =
        (ND::TTrackerReconModule::TTPCTrack *)(*TPC)[ftpcindex];
    // tpctrack->SetClass("ND::TTrackerReconModule::TTPCTrack",1);
    if (tpctrack->Ccorr != 999999.0)
      return tpctrack->Ccorr;
  }
  return 0.0;
}

// Return the track angle
double TTrackerAnalysisUtils::GetTrackCosAngle(
    ND::TTrackerReconModule::TTrackerResult *track) {
  return TMath::Cos(track->Direction.Theta());
}

// Return the track angle (I don't understand this one)
double TTrackerAnalysisUtils::GetTrackCosAngle(
    ND::TTrackerReconModule::TTrackerResult *track, bool backwards) {

  double cosangle = 0.0;

  if (track->NNodes > 0) {
    TClonesArray *nodes = track->Nodes;
    if (!backwards) { // Forward going.
      ND::TTrackerReconModule::TTrackerNode *frontnode =
          (ND::TTrackerReconModule::TTrackerNode *)(*nodes)[0];
      TVector3 dir = frontnode->Direction;
      cosangle = TMath::Cos(dir.Theta());
    } else {
      ND::TTrackerReconModule::TTrackerNode *backnode =
          (ND::TTrackerReconModule::TTrackerNode
               *)(*nodes)[(track->NNodes) - 1];
      TVector3 dir = backnode->Direction;
      dir[2] = -1.0;
      cosangle = TMath::Cos(dir.Theta());
    }
  }
  return cosangle;
}
