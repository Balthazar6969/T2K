#include "TCheatingAnalysisUtils.hxx"
#include "TDirectory.h"
#include "TFile.h"
#include "TROOT.h"

#include <map>
#include <vector>


TCheatingAnalysisUtils::TCheatingAnalysisUtils() {}

bool TCheatingAnalysisUtils::IsAUsefulTPCTrack(
    const ND::TTruthTrajectoriesModule::TTruthTrajectory &traj) {
  // In oaCheatingAnalysis we first checked that there was a 3 sigma seperation
  // for curvature/deltaCurvature (equations from the pdg). But that was to use
  // too much truth info so we changed it to simply checking that the trajectory
  // had a long enough z length (straight line approximation) for possible being
  // able to detect the charge
  // double MMZSize = 36 * unit::cm;  // get from a DB???

  // TEMP
  double MMZSize = 360.0;

  return (GetLongestTPCZLength(traj) >= MMZSize);
}

double TCheatingAnalysisUtils::GetLongestTPCLength(
    const ND::TTruthTrajectoriesModule::TTruthTrajectory &traj) {
  double l1 = GetTPC1Length(traj);
  double l2 = GetTPC2Length(traj);
  double l3 = GetTPC3Length(traj);
  if ((l1 >= l2) && (l1 >= l3))
    return l1;
  if ((l2 >= l1) && (l2 >= l3))
    return l2;
  if ((l3 >= l1) && (l3 >= l2))
    return l3;
  cout << "ERROR in TCheatingAnalysisUtils::GetLongestTPCLength" << endl;
  return -1;
}

double TCheatingAnalysisUtils::GetLongestTPCZLength(
    const ND::TTruthTrajectoriesModule::TTruthTrajectory &traj) {
  double l1 = GetTPC1ZLength(traj);
  double l2 = GetTPC2ZLength(traj);
  double l3 = GetTPC3ZLength(traj);
  if ((l1 >= l2) && (l1 >= l3))
    return l1;
  if ((l2 >= l1) && (l2 >= l3))
    return l2;
  if ((l3 >= l1) && (l3 >= l2))
    return l3;
  cout << "ERROR in TCheatingAnalysisUtils::GetLongestTPCZLength" << endl;
  return -1;
}

double TCheatingAnalysisUtils::GetTPC1Length(
    const ND::TTruthTrajectoriesModule::TTruthTrajectory &traj) {
  enum ESubdetector det = kTPC1;
  return GetTraversedLength(traj, det);
}

double TCheatingAnalysisUtils::GetTPC2Length(
    const ND::TTruthTrajectoriesModule::TTruthTrajectory &traj) {
  enum ESubdetector det = kTPC2;
  return GetTraversedLength(traj, det);
}

double TCheatingAnalysisUtils::GetTPC3Length(
    const ND::TTruthTrajectoriesModule::TTruthTrajectory &traj) {
  enum ESubdetector det = kTPC3;
  return GetTraversedLength(traj, det);
}

double TCheatingAnalysisUtils::GetTraversedLength(
    const ND::TTruthTrajectoriesModule::TTruthTrajectory &traj,
    ESubdetector detector) {
  return fabs(GetTraversedVector(traj, detector).Mag());
}

double TCheatingAnalysisUtils::GetTPC1ZLength(
    const ND::TTruthTrajectoriesModule::TTruthTrajectory &traj) {
  enum ESubdetector det = kTPC1;
  return GetTraversedZLength(traj, det);
}

double TCheatingAnalysisUtils::GetTPC2ZLength(
    const ND::TTruthTrajectoriesModule::TTruthTrajectory &traj) {
  enum ESubdetector det = kTPC2;
  return GetTraversedZLength(traj, det);
}

double TCheatingAnalysisUtils::GetTPC3ZLength(
    const ND::TTruthTrajectoriesModule::TTruthTrajectory &traj) {
  enum ESubdetector det = kTPC3;
  return GetTraversedZLength(traj, det);
}

size_t GetIndexOfFirstEntrance(
    const ND::TTruthTrajectoriesModule::TTruthTrajectory &traj,
    TCheatingAnalysisUtils::ESubdetector det) {
  for (size_t i = 0; i < traj.TraceSubdetectors.size(); ++i) {
    if (traj.TraceSubdetectors[i] == det) {
      return i;
    }
  }
  return -1;
}
size_t
GetIndexOfLastExit(const ND::TTruthTrajectoriesModule::TTruthTrajectory &traj,
                   TCheatingAnalysisUtils::ESubdetector det) {
  for (size_t i = traj.TraceSubdetectors.size(); i > 0; --i) {
    if (traj.TraceSubdetectors[(i - 1)] == det) {
      return (i - 1);
    }
  }
  return -1;
}

TLorentzVector TCheatingAnalysisUtils::GetTraversedVector(
    const ND::TTruthTrajectoriesModule::TTruthTrajectory &traj,
    ESubdetector detector) {

  size_t DetEntranceIndex = GetIndexOfFirstEntrance(traj, detector);
  size_t DetExitIndex = GetIndexOfLastExit(traj, detector);
  TLorentzVector entrance = traj.TraceEntrancePosition[DetEntranceIndex];
  TLorentzVector exit = traj.TraceExitPosition[DetExitIndex];

  // If this trajectory has both entrance and exit position for this subdetector
  if (entrance.Z() != 0 && exit.Z() != 0) {
    return (entrance - exit);
    // If this trajectory enters but does not exit this subdetector
  }
  if (entrance.Z() != 0 && exit.Z() == 0) {
    if (traj.TraceSubdetectors.back() != detector) {
      cout << "ERROR in TCheatingAnalysisUtils::GetTraversedZLength; should "
              "stop in this detector!"
           << endl;
      return TLorentzVector(0, 0, 0, 0);
    }
    return (entrance - traj.TraceExitPosition.back());
    // If this trajectory does not enter but exits this subdetector
  }
  if (entrance.Z() == 0 && exit.Z() != 0) {
    if (traj.TraceSubdetectors.front() != detector) {
      cout << "ERROR in TCheatingAnalysisUtils::GetTraversedLength; should "
              "start in this detector!"
           << endl;
      return TLorentzVector(0, 0, 0, 0);
    }
    return (traj.TraceEntrancePosition.front() - exit);
  }

  // This trajectory is not in this subdetector at all
  return TLorentzVector(0, 0, 0, 0);
}

double TCheatingAnalysisUtils::GetTraversedZLength(
    const ND::TTruthTrajectoriesModule::TTruthTrajectory &traj,
    ESubdetector detector) {
  return fabs(GetTraversedVector(traj, detector).Z());
}

bool TCheatingAnalysisUtils::IsMichelElectronCandidate(
    const ND::TTruthTrajectoriesModule::TTruthTrajectory &traj,
    const TLorentzVector &vertex) {

  if (abs(traj.PDG) == 11) {
    if (traj.TraceEntrancePosition.front().T() >
        vertex.T() + 50.0 /* unit::nanosecond*/) {
      if ((traj.TraceEntrancePosition.front().Vect() - vertex.Vect()).Mag() <
          500 /* unit::mm*/) {
        return true;
      }
    }
  }
  return false;
}

TVector3 TCheatingAnalysisUtils::GetTPCEntranceMomentum(
    const ND::TTruthTrajectoriesModule::TTruthTrajectory &traj) {

  TVector3 vector(0, 0, 0);
  size_t DetEntranceIndex = GetIndexOfFirstEntrance(traj, kTPC1);
  if (GetTPC1Length(traj) > 0) {
    return traj.TraceEntranceMomentum[DetEntranceIndex];
  }
  DetEntranceIndex = GetIndexOfFirstEntrance(traj, kTPC2);
  if (GetTPC2Length(traj) > 0) {
    return traj.TraceEntranceMomentum[DetEntranceIndex];
  }
  DetEntranceIndex = GetIndexOfFirstEntrance(traj, kTPC3);
  if (GetTPC3Length(traj) > 0) {
    return traj.TraceEntranceMomentum[DetEntranceIndex];
  }

  return vector;
}
