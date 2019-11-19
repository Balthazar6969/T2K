#ifndef __P0DNUMUUTILS_H_
#define __P0DNUMUUTILS_H_

#include <TF1.h>
#include <TGraph.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TVector3.h>

#include "libReadoaAnalysis/libReadoaAnalysisProjectHeaders.h"

using namespace std;

class TFile;
class TGraph;
class TF1;

class P0DELossCorr {

public:
  P0DELossCorr(bool waterInFlag = true);
  ~P0DELossCorr();

  double Calc(double p0dTrackLength, double initMom, TVector3 p0dFrontDir,
              bool reverse, double p0dTrackZEnd);
  double Eval(double p0dTrackLength, double tpcMom, TVector3 p0dFrontDir,
              bool reverse = false);
  double Eval(ND::TGlobalReconModule::TGlobalPID *globalTrack);

  void Initialize(bool);
  void UnInitialize();

  bool waterStatus;

private:
  TGraph *fEcalRangeGraph;
  TGraph *fEcalRangeInverseGraph;
  TGraph *fTargetRangeGraph;
  TGraph *fTargetRangeInverseGraph;

  TF1 *fEcalFCN;
  TF1 *fTargetFCN;

  double fZCentralECAL; // mm from materials sheet
  double fWaterTargetZ;
  double fEcalGap1;
  double fWaterTargetGap;
  double fEcalGap2;

  double fP0DEnd_Z;
  double fP0DCEStart_Z;
  double fP0DUSEEnd_Z;
};
#endif
