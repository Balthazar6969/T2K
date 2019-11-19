#ifndef TTrackerAnalysisUtils_h_seen
#define TTrackerAnalysisUtils_h_seen
#include "libReadoaAnalysis/libReadoaAnalysisProjectHeaders.h"
#include <TObject.h>

/// oaAnalysis read-out utility in macros/AnalysisTools,
/// which which are useful when analysing
/// Tracker (FGD + TPC) information.
///
/// These were derived from the
/// codeset for the original Tracker CCcE Analysis from 2007.
///
/// Updated to handle newer TTrackerReconModule which was rewritten
/// near the end of June 2010.
///
class TTrackerAnalysisUtils{
public:
  /// Default constructor, which looks in all the currently open files for
  /// something
  /// called //ReconDir/Modules/TrackerMod, and then keeps a pointer to this
  /// object
  /// called fTrackerModuleInFile, which it can refer to for variables saved
  /// inside,
  /// for example FGD1ActiveMin etc
  TTrackerAnalysisUtils();

  virtual ~TTrackerAnalysisUtils(){};

  /// Figure out if position is inside volume defined by minpos and maxpos.
  /// Assumes we are dealing with a box that is perfectly aligned with
  /// the coordinate system.
  bool IsInVolume(TVector3 pos, TVector3 minpos, TVector3 maxpos);
  bool IsInFiducialVolume(TVector3 pos);
  void DefineFiducialVolume(TVector3 dist);
  void DefineFiducialVolume(double dx, double dy, double dz);
  void DefineFiducialVolume(double xmin, double xmax, double ymin, double ymax,
                            double zmin, double zmax);
  /// Is position in Fiducial FGD1?
  bool IsInFiducialFGD1(TVector3 pos, TVector3 fgd_trim);
  bool IsInFiducialFGD1(TVector3 pos);
  bool IsInFiducialFGD1(TLorentzVector pos, TVector3 fgd_trim);
  bool IsInFiducialFGD1(TLorentzVector pos);
  bool IsInFGD1(TLorentzVector pos);
  /// Is position in Fiducial FGD2?
  bool IsInFiducialFGD2(TVector3 pos, TVector3 fgd_trim);
  bool IsInFiducialFGD2(TVector3 pos);
  bool IsInFiducialFGD2(TLorentzVector pos, TVector3 fgd_trim);
  bool IsInFiducialFGD2(TLorentzVector pos);
  bool IsInFGD2(TLorentzVector pos);
  bool IsInFGD2(TVector3 pos);
  bool IsInFGD1(TVector3 pos);

  // ________________________________________________________________________
  // Track helper routines

  /// Return the TPC measured sign of charge for this track.
  /// Measurement assumes forward going track.
  int GetTrackTpcCharge(ND::TTrackerReconModule::TTrackerResult *track);
  double GetTrackTpcQuality(ND::TTrackerReconModule::TTrackerResult *track);
  // Return the TPC momentum.   Just use first TPC momentum measurement we find.
  double GetTrackMomentum(ND::TTrackerReconModule::TTrackerResult *track);
  double GetTrackMomentumError(ND::TTrackerReconModule::TTrackerResult *track);
  // Return the track angle
  double GetTrackCosAngle(ND::TTrackerReconModule::TTrackerResult *track);
  double GetTrackCosAngle(ND::TTrackerReconModule::TTrackerResult *track,
                          bool backwards);
  // Return the pull
  double GetTrackTpcPid(ND::TTrackerReconModule::TTrackerResult *track,
                        std::string parName);
  double GetTrackTpcPidSigma(ND::TTrackerReconModule::TTrackerResult *track,
                             std::string parName);
  double GetTrackTpcPiddEdxexp(ND::TTrackerReconModule::TTrackerResult *track,
                               std::string parName);
  double GetTrackTpcPidNTrun(ND::TTrackerReconModule::TTrackerResult *track);
  double GetTrackTpcPidCcorr(ND::TTrackerReconModule::TTrackerResult *track);

  // Return the pull for a given tpc
  double GetTrackTpcPid(ND::TTrackerReconModule::TTrackerResult *track,
                        std::string parName, int tpcindex);
  double GetTrackTpcPidSigma(ND::TTrackerReconModule::TTrackerResult *track,
                             std::string parName, int tpcindex);
  double GetTrackTpcPiddEdxexp(ND::TTrackerReconModule::TTrackerResult *track,
                               std::string parName, int tpcindex);
  double GetTrackTpcPidNTrun(ND::TTrackerReconModule::TTrackerResult *track,
                             int tpcindex);
  double GetTrackTpcPidCcorr(ND::TTrackerReconModule::TTrackerResult *track,
                             int tpcindex);

  int ftpcindex;
  TVector3 fgd_fiducial_trim;

private:
  /// Quantities read in from the module
  TVector3 fFGD1ActiveMin;
  TVector3 fFGD1ActiveMax;
  TVector3 fFGD2ActiveMin;
  TVector3 fFGD2ActiveMax;
  TVector3 fFGD1FiducialMin;
  TVector3 fFGD1FiducialMax;
  TVector3 fFGD2FiducialMin;
  TVector3 fFGD2FiducialMax;
};
#endif
