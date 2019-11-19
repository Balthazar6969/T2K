#ifndef TGlobalAnalysisUtils_h_seen
#define TGlobalAnalysisUtils_h_seen
#include <TClonesArray.h>
#include <TLorentzVector.h>
#include <TObject.h>
#include <TVector3.h>

/// oaAnalysis read-out utilities in macros/AnalysisTools,
#include "libReadoaAnalysis/libReadoaAnalysisProjectHeaders.h"

class TGlobalAnalysisUtils {

public:
  TGlobalAnalysisUtils();
  virtual ~TGlobalAnalysisUtils() {}

  double GetDeltaThetaP(const ND::TGlobalReconModule::TGlobalPID &posTrack,
                        const ND::TGlobalReconModule::TGlobalPID &negTrack);

  double GetTrackCosTheta(const ND::TGlobalReconModule::TGlobalPID &track);

  double GetTrackMomentum(const ND::TGlobalReconModule::TGlobalPID &track);

  double GetTrackTpcMomentum(ND::TGlobalReconModule::TGlobalPID *globalTrack);
  double
  GetTrackTpcMomentumError(ND::TGlobalReconModule::TGlobalPID *globalTrack);
  double GetTrackTpcCharge(ND::TGlobalReconModule::TGlobalPID *globalTrack);
  double GetTrackTpcQuality(ND::TGlobalReconModule::TGlobalPID *globalTrack);
  double GetTrackTpcMomentum(ND::TGlobalReconModule::TGlobalPID *globalTrack,
                             int itpc);
  double
  GetTrackTpcMomentumError(ND::TGlobalReconModule::TGlobalPID *globalTrack,
                           int itpc);
  double GetTrackTpcCharge(ND::TGlobalReconModule::TGlobalPID *globalTrack,
                           int itpc);
  double GetTrackTpcQuality(ND::TGlobalReconModule::TGlobalPID *globalTrack,
                            int itpc);
  // JIMFIX - Originally these were not defined in .cxx file. Instead
  // GetTrackTpcCcorr,
  // GetTrackTpcCcorr, GetTrackTpcNTrun and GetTrackTpcNTrun were defined. I
  // presume this
  // was a mistake but needs to be checked. For now to get to compile have
  // changed the
  // names in the src file to match.
  double GetTrackTpcPidCcorr(ND::TGlobalReconModule::TGlobalPID *globalTrack);
  double GetTrackTpcPidCcorr(ND::TGlobalReconModule::TGlobalPID *globalTrack,
                             int itpc);
  double GetTrackTpcPidNTrun(ND::TGlobalReconModule::TGlobalPID *globalTrack);
  double GetTrackTpcPidNTrun(ND::TGlobalReconModule::TGlobalPID *globalTrack,
                             int itpc);
  double GetTrackTpcPidPull(ND::TGlobalReconModule::TGlobalPID *globalTrack,
                            std::string parName);
  double GetTrackTpcPidPull(ND::TGlobalReconModule::TGlobalPID *globalTrack,
                            std::string parName, int itpc);
  double GetTrackTpcPidSigma(ND::TGlobalReconModule::TGlobalPID *globalTrack,
                             std::string parName);
  double GetTrackTpcPidSigma(ND::TGlobalReconModule::TGlobalPID *globalTrack,
                             std::string parName, int itpc);
  double GetTrackTpcPiddEdxexp(ND::TGlobalReconModule::TGlobalPID *globalTrack,
                               std::string parName);
  double GetTrackTpcPiddEdxexp(ND::TGlobalReconModule::TGlobalPID *globalTrack,
                               std::string parName, int itpc);

  double GetNeutrinoEnergy(double Emu, double muMom, double cosThetaMu);
  double GetNeutrinoEnergy(const ND::TGlobalReconModule::TGlobalPID &track);

  double
  GetNeutrinoEnergy2Tracks(const ND::TGlobalReconModule::TGlobalPID &posTrack,
                           const ND::TGlobalReconModule::TGlobalPID &negTrack);

  double GetEnergy(double m, double p);

  TVector3
  GetTrackMomentumVector(const ND::TGlobalReconModule::TGlobalPID &track);

  // bool IsInTPC(const ND::TGlobalReconModule::TGlobalPID& track);
  bool IsInTPC(const ND::TGlobalReconModule::TGlobalPID *track);

  double GetNuInvariantMass(const ND::TGlobalReconModule::TGlobalPID &posTrack,
                            const ND::TGlobalReconModule::TGlobalPID &negTrack);

  // double GetClusterDistanceFromTheVertex(const
  // ND::TTrackerReconModule::TFgdCluster& cluster, const
  // ND::TTrackerReconModule::TTrackerVertex& vertex);
  // bool IsAMichelElectronCandidate(const ND::TTrackerReconModule::TFgdCluster&
  // cluster, const ND::TTrackerReconModule::TTrackerVertex& vertex, double
  // michelCut);

  //  double GetClusterDistanceFromTheVertex(const
  //  ND::TGlobalReconModule::TFgdCluster& cluster, TLorentzVector vertex);
  // bool IsAMichelElectronCandidate(const ND::TGlobalReconModule::TFgdCluster&
  // cluster, TLorentzVector vertex, double michelCut);
  // double GetClusterTimeSeparationFromVertex(const
  // ND::TGlobalReconModule::TFgdCluster& cluster, TLorentzVector vertex);

  bool IsNotAShower(const ND::TGlobalReconModule::TGlobalPID &track);

  bool IsInFGD(const ND::TGlobalReconModule::TGlobalPID *track);
  bool IsInTPCAndInFGD(const ND::TGlobalReconModule::TGlobalPID *track);
  TLorentzVector ComputeRecVertex(TClonesArray *PIDs, vector<int> &charge_mod,
                                  double &quality, int version, int bunch,
                                  bool isMC);
  int GetBunch(const ND::TGlobalReconModule::TGlobalPID *track, bool isMC);

private:
  // masses
  double fMmu;
  double fMp;
  double fMn;
  // nuclear potential
  double fV;
};

#endif
