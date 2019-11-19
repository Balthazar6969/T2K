#ifndef TCheatingAnalysisUtils_h_seen
#define TCheatingAnalysisUtils_h_seen
#include <TObject.h>

#include "libReadoaAnalysis/libReadoaAnalysisProjectHeaders.h"

/// oaAnalysis read-out utilities in macros/AnalysisTools,
/// which are useful when analysing Cheating (FGD + TPC) information.
/// These utilities were derived from the original oaCheatingAnalysis

class TCheatingAnalysisUtils{

public:
  // Copied from ToaAnalysisUtils. Should be removed as soon as
  // ToaAnalysisUtils is accesible from /macros!!!!!
  enum ESubdetector {
    kFGD1 = 0,
    kFGD2,
    kP0D,
    kDsECal,
    kBrlECal,
    kP0DECal,
    kTPC1,
    kTPC2,
    kTPC3,
    kMRD,
    kOffAxis, /// the rest of the off-axis detector
    kINGRID,
    kCavern,
    kNSubdetectors
  };

  TCheatingAnalysisUtils();
  virtual ~TCheatingAnalysisUtils(){};

  bool
  IsAUsefulTPCTrack(const ND::TTruthTrajectoriesModule::TTruthTrajectory &traj);
  bool IsMichelElectronCandidate(
      const ND::TTruthTrajectoriesModule::TTruthTrajectory &traj,
      const TLorentzVector &vertex);

  // Straight line approximation
  double GetLongestTPCLength(
      const ND::TTruthTrajectoriesModule::TTruthTrajectory &traj);
  // Straight line approximation
  double GetLongestTPCZLength(
      const ND::TTruthTrajectoriesModule::TTruthTrajectory &traj);

  // Straight line approximation
  double
  GetTPC1Length(const ND::TTruthTrajectoriesModule::TTruthTrajectory &traj);
  // Straight line approximation
  double
  GetTPC2Length(const ND::TTruthTrajectoriesModule::TTruthTrajectory &traj);
  // Straight line approximation
  double
  GetTPC3Length(const ND::TTruthTrajectoriesModule::TTruthTrajectory &traj);

  // Straight line approximation
  double
  GetTPC1ZLength(const ND::TTruthTrajectoriesModule::TTruthTrajectory &traj);
  // Straight line approximation
  double
  GetTPC2ZLength(const ND::TTruthTrajectoriesModule::TTruthTrajectory &traj);
  // Straight line approximation
  double
  GetTPC3ZLength(const ND::TTruthTrajectoriesModule::TTruthTrajectory &traj);

  TVector3 GetTPCEntranceMomentum(
      const ND::TTruthTrajectoriesModule::TTruthTrajectory &traj);
private:
  double
  GetTraversedLength(const ND::TTruthTrajectoriesModule::TTruthTrajectory &traj,
                     ESubdetector detector); // Straight line approximation
  double GetTraversedZLength(
      const ND::TTruthTrajectoriesModule::TTruthTrajectory &traj,
      ESubdetector detector); // Straight line approximation
  TLorentzVector
  GetTraversedVector(const ND::TTruthTrajectoriesModule::TTruthTrajectory &traj,
                     ESubdetector detector);
};
#endif
