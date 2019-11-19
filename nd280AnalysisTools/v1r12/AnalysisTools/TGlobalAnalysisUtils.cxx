#include <TClonesArray.h>
#include <TMath.h>
#include <TRefArray.h>
#include <math.h>
#include <functional>
#include <list>
#include <numeric>
#include <vector>
#include "TDirectory.h"
#include "TFile.h"
#include "TROOT.h"
//#include <algorithm>
#include <iostream>
#include "TGlobalAnalysisUtils.hxx"

//*************************************************
TGlobalAnalysisUtils::TGlobalAnalysisUtils() {
  //*************************************************

  fMmu = 105.658367;  // MeV (muon mass)
  fMp = 938.27203;    // MeV (proton mass)
  fMn = 939.56536;    // MeV (neutron mass)
  fV = 25.;           // MeV (nuclear potential)
}

//*************************************************
double TGlobalAnalysisUtils::GetDeltaThetaP(
    const ND::TGlobalReconModule::TGlobalPID &posTrack,
    const ND::TGlobalReconModule::TGlobalPID &negTrack) {
  //*************************************************

  TVector3 pCand_momDir;
  TVector3 muCand_momDir;
  TVector3 muCand_mom;
  double muCand_momMag = negTrack.FrontMomentum;
  double pCand_momMag = posTrack.FrontMomentum;

  double Emu = GetEnergy(fMmu, muCand_momMag);  // MeV
  double cosThetaMu = GetTrackCosTheta(negTrack);
  double ENuRec = GetNeutrinoEnergy(Emu, muCand_momMag, cosThetaMu);

  // get proton and muon candidates' momentum versor (unitary vector)
  if (posTrack.isForward) pCand_momDir = posTrack.BackDirection;
  if (negTrack.isForward) muCand_momDir = negTrack.BackDirection;
  if (!posTrack.isForward) pCand_momDir = posTrack.FrontDirection;
  if (!negTrack.isForward) muCand_momDir = negTrack.FrontDirection;

  if (muCand_momMag != 0 && pCand_momMag != 0) {
    double npz = pCand_momDir.Z();
    // build the muon candidate momentum vector
    TVector3 muCandMom = muCand_momMag * muCand_momDir;

    double cosDeltaThetaP =
        (npz * ENuRec - pCand_momDir * muCandMom) /
        (sqrt(muCand_momMag * muCand_momMag + ENuRec * ENuRec -
              2 * muCandMom.Z() * ENuRec));
    double deltaThetaP = acos(cosDeltaThetaP);
    return deltaThetaP;
  } else
    return 999.;
}

//*************************************************
double TGlobalAnalysisUtils::GetNeutrinoEnergy(double Emu, double muMom,
                                               double cosThetaMu) {
  //*************************************************

  double ENuRec = (((pow(fMp, 2) - pow(fMmu, 2)) + 2 * Emu * (fMn - fV) -
                    pow(fMn - fV, 2)) /
                   (-Emu + (fMn - fV) + (muMom * cosThetaMu))) /
                  2;
  /*
  cout << "--- Emu = " << Emu<< endl;
  cout << "--- muMom = " << muMom << endl;
  cout << "--- cosThetaMu = " <<cosThetaMu << endl;
  */
  return ENuRec;
}

//*************************************************
double TGlobalAnalysisUtils::GetNeutrinoEnergy(
    const ND::TGlobalReconModule::TGlobalPID &track) {
  //*************************************************

  double muCandMom = track.FrontMomentum;
  double Emu = GetEnergy(fMmu, muCandMom);  // MeV
  double cosThetaMu = GetTrackCosTheta(track);
  double ENuRec = GetNeutrinoEnergy(Emu, muCandMom, cosThetaMu);
  /*
  cout << "--- Emu = " << Emu<< endl;
  cout << "--- muMom = " << muMom << endl;
  cout << "--- cosThetaMu = " <<cosThetaMu << endl;
  */
  return ENuRec;
}

//*************************************************
double TGlobalAnalysisUtils::GetNeutrinoEnergy2Tracks(
    const ND::TGlobalReconModule::TGlobalPID &posTrack,
    const ND::TGlobalReconModule::TGlobalPID &negTrack) {
  //*************************************************

  // muon candidate kinematics
  TVector3 muonMom = GetTrackMomentumVector(negTrack);
  double muCandMom = negTrack.FrontMomentum;
  double Emu = GetEnergy(fMmu, muCandMom);  // MeV

  // proton candidate kinematics
  TVector3 protonMom = GetTrackMomentumVector(posTrack);
  double pCandMom = posTrack.FrontMomentum;
  // double pCandMomZ = protonMom.Z();
  double Ep = GetEnergy(fMp, pCandMom);  // MeV

  // final state kinematics
  double Mf2 =
      (pow(Emu, 2) - pow(muCandMom, 2)) + (pow(Ep, 2) - pow(pCandMom, 2));
  double Ef = Emu + Ep;
  TVector3 finalStateMom = muonMom + protonMom;
  double fMom = finalStateMom.Mag();
  double fMomZ = finalStateMom.Z();
  // double Ef_2 = GetEnergy(sqrt(Mf2),fMom);
  // we assume the neutrino moving along the z axis
  double cosThetaF = fMomZ / fMom;
  // double cosThetaF_2 = pCandMomZ/pCandMom;

  // double ENuRec_2 = ( (Mf2 - fMn*fMn + 2*fV*Ef + fV*fV) / (fV + Ef -
  // fMom*cosThetaF_2) )/2;
  double ENuRec = ((Mf2 - fMn * fMn + 2 * fV * Ef + fV * fV) /
                   (fV + Ef - fMom * cosThetaF)) /
                  2;
  /*
  cout << "ENuRec (2tracks) = " << ENuRec << "     ENuRec_2 (2tracks) = " <<
  ENuRec_2 << endl;
  cout << "Ef = " << Ef << "     Ef_2 = " << Ef_2 << endl;
  cout << "cosThetaF = " << cosThetaF << "     cosThetaF_2 = " << cosThetaF_2 <<
  endl;
  */
  return ENuRec;
}

// Return the angle between the outcoming track and the incoming neutrinos
//*************************************************
double TGlobalAnalysisUtils::GetTrackCosTheta(
    const ND::TGlobalReconModule::TGlobalPID &track) {
  //*************************************************
  double cosTheta = 0.0;
  if (track.isForward) {  // Forward going.
    TVector3 dir = track.FrontDirection;
    cosTheta = TMath::Cos(dir.Theta());
  } else {
    TVector3 dir = track.BackDirection;
    dir[2] = -1.0;
    cosTheta = TMath::Cos(dir.Theta());
  }
  return cosTheta;
}

//*************************************************
double TGlobalAnalysisUtils::GetTrackMomentum(
    const ND::TGlobalReconModule::TGlobalPID &track) {
  //*************************************************

  return track.FrontMomentum;
}

// Return the TPC momentum.
//*************************************************
double TGlobalAnalysisUtils::GetTrackTpcMomentum(
    ND::TGlobalReconModule::TGlobalPID *globalTrack) {
  //*************************************************
  // return the momentum with smallest mom_err/mom<momcut
  // if this is not found return the first momentum!=0
  double relerr = 999999;
  double mom = 0;
  double errmin = 999999;
  for (int jj = 0; jj < globalTrack->NTPCs; jj++) {
    ND::TGlobalReconModule::TTPCObject *tpcTrack =
        (ND::TGlobalReconModule::TTPCObject *)(*globalTrack->TPC)[jj];
    if (tpcTrack->FrontMomentum != 0)
      relerr = tpcTrack->FrontMomentumError / tpcTrack->FrontMomentum;
    if (relerr < errmin) {
      errmin = relerr;
      mom = tpcTrack->FrontMomentum;
    }
  }
  return mom;
}

// Return the TPC momentum error.
//*************************************************
double TGlobalAnalysisUtils::GetTrackTpcMomentumError(
    ND::TGlobalReconModule::TGlobalPID *globalTrack) {
  //*************************************************
  // return the momentum with smallest mom_err/mom<momcut
  // if this is not found return the first momentum!=0
  double relerr = 999999;
  double errmom = 0;
  double errmin = 999999;
  for (int jj = 0; jj < globalTrack->NTPCs; jj++) {
    ND::TGlobalReconModule::TTPCObject *tpcTrack =
        (ND::TGlobalReconModule::TTPCObject *)(*globalTrack->TPC)[jj];
    if (tpcTrack->FrontMomentum != 0)
      relerr = tpcTrack->FrontMomentumError / tpcTrack->FrontMomentum;
    if (relerr < errmin) {
      errmin = relerr;
      errmom = tpcTrack->FrontMomentumError;
    }
  }
  return errmom;
}

// Return the TPC charge.
//*************************************************
double TGlobalAnalysisUtils::GetTrackTpcCharge(
    ND::TGlobalReconModule::TGlobalPID *globalTrack) {
  //*************************************************
  // return the momentum with smallest mom_err/mom<momcut
  // if this is not found return the first momentum!=0
  double relerr = 999999;
  double charge = 0;
  double errmin = 999999;
  for (int jj = 0; jj < globalTrack->NTPCs; jj++) {
    ND::TGlobalReconModule::TTPCObject *tpcTrack =
        (ND::TGlobalReconModule::TTPCObject *)(*globalTrack->TPC)[jj];
    if (tpcTrack->FrontMomentum != 0)
      relerr = tpcTrack->FrontMomentumError / tpcTrack->FrontMomentum;
    if (relerr < errmin) {
      errmin = relerr;
      charge = tpcTrack->Charge;
    }
  }
  return charge;
}

// Return the TPC quality.
//*************************************************
double TGlobalAnalysisUtils::GetTrackTpcQuality(
    ND::TGlobalReconModule::TGlobalPID *globalTrack) {
  //*************************************************
  // return the momentum with smallest mom_err/mom<momcut
  // if this is not found return the first momentum!=0
  double relerr = 999999;
  double chi2 = 0;
  double errmin = 999999;
  for (int jj = 0; jj < globalTrack->NTPCs; jj++) {
    ND::TGlobalReconModule::TTPCObject *tpcTrack =
        (ND::TGlobalReconModule::TTPCObject *)(*globalTrack->TPC)[jj];
    if (tpcTrack->FrontMomentum != 0)
      relerr = tpcTrack->FrontMomentumError / tpcTrack->FrontMomentum;
    if (relerr < errmin) {
      errmin = relerr;
      chi2 = tpcTrack->Chi2;
    }
  }
  return chi2;
}
// Return the TPC momentum.
//*************************************************
double TGlobalAnalysisUtils::GetTrackTpcMomentum(
    ND::TGlobalReconModule::TGlobalPID *globalTrack, int itpc) {
  //*************************************************
  double mom = 0;
  for (int jj = 0; jj < globalTrack->NTPCs; jj++) {
    ND::TGlobalReconModule::TTPCObject *tpcTrack =
        (ND::TGlobalReconModule::TTPCObject *)(*globalTrack->TPC)[jj];
    int det = tpcTrack->Detector;
    if (det == itpc) mom = tpcTrack->FrontMomentum;
  }
  return mom;
}
// Return the TPC momentum error
//*************************************************
double TGlobalAnalysisUtils::GetTrackTpcMomentumError(
    ND::TGlobalReconModule::TGlobalPID *globalTrack, int itpc) {
  //*************************************************
  double errmom = 0;
  for (int jj = 0; jj < globalTrack->NTPCs; jj++) {
    ND::TGlobalReconModule::TTPCObject *tpcTrack =
        (ND::TGlobalReconModule::TTPCObject *)(*globalTrack->TPC)[jj];
    int det = tpcTrack->Detector;
    if (det == itpc) errmom = tpcTrack->FrontMomentumError;
  }
  return errmom;
}
double TGlobalAnalysisUtils::GetTrackTpcCharge(
    ND::TGlobalReconModule::TGlobalPID *globalTrack, int itpc) {
  //*************************************************
  double charge = 0;
  for (int jj = 0; jj < globalTrack->NTPCs; jj++) {
    ND::TGlobalReconModule::TTPCObject *tpcTrack =
        (ND::TGlobalReconModule::TTPCObject *)(*globalTrack->TPC)[jj];
    int det = tpcTrack->Detector;
    if (det == itpc) charge = tpcTrack->Charge;
  }
  return charge;
}
//*************************************************
double TGlobalAnalysisUtils::GetTrackTpcQuality(
    ND::TGlobalReconModule::TGlobalPID *globalTrack, int itpc) {
  //*************************************************
  double chi2 = 0;
  for (int jj = 0; jj < globalTrack->NTPCs; jj++) {
    ND::TGlobalReconModule::TTPCObject *tpcTrack =
        (ND::TGlobalReconModule::TTPCObject *)(*globalTrack->TPC)[jj];
    int det = tpcTrack->Detector;
    if (det == itpc) chi2 = tpcTrack->Chi2;
  }
  return chi2;
}

//*************************************************
double TGlobalAnalysisUtils::GetTrackTpcPidCcorr(
    ND::TGlobalReconModule::TGlobalPID *globalTrack) {
  //*************************************************
  // return the momentum with smallest mom_err/mom<momcut
  // if this is not found return the first momentum!=0
  double relerr = 999999;
  double ccorr = 999999;
  double errmin = 999999;
  for (int jj = 0; jj < globalTrack->NTPCs; jj++) {
    ND::TGlobalReconModule::TTPCObject *tpcTrack =
        (ND::TGlobalReconModule::TTPCObject *)(*globalTrack->TPC)[jj];
    if (tpcTrack->FrontMomentum != 0)
      relerr = tpcTrack->FrontMomentumError / tpcTrack->FrontMomentum;
    if (relerr < errmin) {
      errmin = relerr;
      ccorr = tpcTrack->Ccorr;
    }
  }
  return ccorr;
}
//*************************************************
double TGlobalAnalysisUtils::GetTrackTpcPidCcorr(
    ND::TGlobalReconModule::TGlobalPID *globalTrack, int itpc) {
  //*************************************************
  double ccorr = 999999;
  for (int jj = 0; jj < globalTrack->NTPCs; jj++) {
    ND::TGlobalReconModule::TTPCObject *tpcTrack =
        (ND::TGlobalReconModule::TTPCObject *)(*globalTrack->TPC)[jj];
    int det = tpcTrack->Detector;
    if (det == itpc) ccorr = tpcTrack->Ccorr;
  }
  return ccorr;
}

//*************************************************
double TGlobalAnalysisUtils::GetTrackTpcPidNTrun(
    ND::TGlobalReconModule::TGlobalPID *globalTrack) {
  //*************************************************
  // return the momentum with smallest mom_err/mom<momcut
  // if this is not found return the first momentum!=0
  double relerr = 999999;
  double ntrun = 999999;
  double errmin = 999999;
  for (int jj = 0; jj < globalTrack->NTPCs; jj++) {
    ND::TGlobalReconModule::TTPCObject *tpcTrack =
        (ND::TGlobalReconModule::TTPCObject *)(*globalTrack->TPC)[jj];
    if (tpcTrack->FrontMomentum != 0)
      relerr = tpcTrack->FrontMomentumError / tpcTrack->FrontMomentum;
    if (relerr < errmin) {
      errmin = relerr;
      ntrun = tpcTrack->NTrun;
    }
  }
  return ntrun;
}
//*************************************************
double TGlobalAnalysisUtils::GetTrackTpcPidNTrun(
    ND::TGlobalReconModule::TGlobalPID *globalTrack, int itpc) {
  //*************************************************
  double ntrun = 999999;
  for (int jj = 0; jj < globalTrack->NTPCs; jj++) {
    ND::TGlobalReconModule::TTPCObject *tpcTrack =
        (ND::TGlobalReconModule::TTPCObject *)(*globalTrack->TPC)[jj];
    int det = tpcTrack->Detector;
    if (det == itpc) ntrun = tpcTrack->NTrun;
  }
  return ntrun;
}

//*************************************************
double TGlobalAnalysisUtils::GetTrackTpcPidPull(
    ND::TGlobalReconModule::TGlobalPID *globalTrack, std::string parName) {
  //*************************************************
  // give the pid corresponding to the tpc for which the momentum is maximal,
  // and the error on the curvature is minimal
  // if this doesn't exist, just take the first information we have.
  double pullel = 999999;
  double pullmu = 999999;
  double pullp = 999999;
  double pullpi = 999999;
  double pullk = 999999;
  double relerr = 999999;
  double errmin = 999999;
  for (int jj = 0; jj < globalTrack->NTPCs; jj++) {
    ND::TGlobalReconModule::TTPCObject *tpcTrack =
        (ND::TGlobalReconModule::TTPCObject *)(*globalTrack->TPC)[jj];
    if (tpcTrack->FrontMomentum != 0) {
      relerr = tpcTrack->FrontMomentumError / tpcTrack->FrontMomentum;
      if (relerr < errmin) {
        errmin = relerr;
        pullel = tpcTrack->PullEle;
        pullmu = tpcTrack->PullMuon;
        pullpi = tpcTrack->PullPion;
        pullp = tpcTrack->PullProton;
        pullk = tpcTrack->PullKaon;
      }
    }
  }

  if (relerr != 999999) {
    if (parName.compare("Electron") == 0)
      if (pullel != 999999.0 && fabs(pullel) > 0.00001) return pullel;
    if (parName.compare("Muon") == 0)
      if (pullmu != 999999.0 && fabs(pullmu) > 0.00001) return pullmu;
    if (parName.compare("Proton") == 0)
      if (pullp != 999999.0 && fabs(pullp) > 0.00001) return pullp;
    if (parName.compare("Pion") == 0)
      if (pullpi != 999999.0 && fabs(pullpi) > 0.00001) return pullpi;
    if (parName.compare("Kaon") == 0)
      if (pullk != 999999.0 && fabs(pullk) > 0.00001) return pullk;
  }

  return 999999;
}
//*************************************************
double TGlobalAnalysisUtils::GetTrackTpcPidPull(
    ND::TGlobalReconModule::TGlobalPID *globalTrack, std::string parName,
    int itpc) {
  //*************************************************
  double pullel = 999999;
  double pullmu = 999999;
  double pullp = 999999;
  double pullpi = 999999;
  double pullk = 999999;
  for (int jj = 0; jj < globalTrack->NTPCs; jj++) {
    ND::TGlobalReconModule::TTPCObject *tpcTrack =
        (ND::TGlobalReconModule::TTPCObject *)(*globalTrack->TPC)[jj];
    if (tpcTrack->FrontMomentum != 0) {
      int det = tpcTrack->Detector;
      if (det == itpc) {
        pullel = tpcTrack->PullEle;
        pullmu = tpcTrack->PullMuon;
        pullpi = tpcTrack->PullPion;
        pullp = tpcTrack->PullProton;
        pullk = tpcTrack->PullKaon;
      }
    }
  }

  if (parName.compare("Electron") == 0)
    if (pullel != 999999.0 && fabs(pullel) > 0.00001) return pullel;
  if (parName.compare("Muon") == 0)
    if (pullmu != 999999.0 && fabs(pullmu) > 0.00001) return pullmu;
  if (parName.compare("Proton") == 0)
    if (pullp != 999999.0 && fabs(pullp) > 0.00001) return pullp;
  if (parName.compare("Pion") == 0)
    if (pullpi != 999999.0 && fabs(pullpi) > 0.00001) return pullpi;
  if (parName.compare("Kaon") == 0)
    if (pullk != 999999.0 && fabs(pullk) > 0.00001) return pullk;

  return 999999;
}

//*************************************************
double TGlobalAnalysisUtils::GetTrackTpcPidSigma(
    ND::TGlobalReconModule::TGlobalPID *globalTrack, std::string parName) {
  //*************************************************
  // give the pid corresponding to the tpc for which the momentum is maximal,
  // and the error on the curvature is minimal
  // if this doesn't exist, just take the first information we have.
  double sigmael = 999999;
  double sigmamu = 999999;
  double sigmap = 999999;
  double sigmapi = 999999;
  double sigmak = 999999;
  double relerr = 999999;
  double errmin = 999999;
  for (int jj = 0; jj < globalTrack->NTPCs; jj++) {
    ND::TGlobalReconModule::TTPCObject *tpcTrack =
        (ND::TGlobalReconModule::TTPCObject *)(*globalTrack->TPC)[jj];
    if (tpcTrack->FrontMomentum != 0) {
      relerr = tpcTrack->FrontMomentumError / tpcTrack->FrontMomentum;
      if (relerr < errmin) {
        errmin = relerr;
        sigmael = tpcTrack->SigmaEle;
        sigmamu = tpcTrack->SigmaMuon;
        sigmapi = tpcTrack->SigmaPion;
        sigmap = tpcTrack->SigmaProton;
        sigmak = tpcTrack->SigmaKaon;
      }
    }
  }

  if (relerr != 999999) {
    if (parName.compare("Electron") == 0)
      if (sigmael != 999999.0 && fabs(sigmael) > 0.00001) return sigmael;
    if (parName.compare("Muon") == 0)
      if (sigmamu != 999999.0 && fabs(sigmamu) > 0.00001) return sigmamu;
    if (parName.compare("Proton") == 0)
      if (sigmap != 999999.0 && fabs(sigmap) > 0.00001) return sigmap;
    if (parName.compare("Pion") == 0)
      if (sigmapi != 999999.0 && fabs(sigmapi) > 0.00001) return sigmapi;
    if (parName.compare("Kaon") == 0)
      if (sigmak != 999999.0 && fabs(sigmak) > 0.00001) return sigmak;
  }

  return 999999;
}
//*************************************************
double TGlobalAnalysisUtils::GetTrackTpcPidSigma(
    ND::TGlobalReconModule::TGlobalPID *globalTrack, std::string parName,
    int itpc) {
  //*************************************************
  double sigmael = 999999;
  double sigmamu = 999999;
  double sigmap = 999999;
  double sigmapi = 999999;
  double sigmak = 999999;
  for (int jj = 0; jj < globalTrack->NTPCs; jj++) {
    ND::TGlobalReconModule::TTPCObject *tpcTrack =
        (ND::TGlobalReconModule::TTPCObject *)(*globalTrack->TPC)[jj];
    if (tpcTrack->FrontMomentum != 0) {
      int det = tpcTrack->Detector;
      if (det == itpc) {
        sigmael = tpcTrack->SigmaEle;
        sigmamu = tpcTrack->SigmaMuon;
        sigmapi = tpcTrack->SigmaPion;
        sigmap = tpcTrack->SigmaProton;
        sigmak = tpcTrack->SigmaKaon;
      }
    }
  }

  if (parName.compare("Electron") == 0)
    if (sigmael != 999999.0 && fabs(sigmael) > 0.00001) return sigmael;
  if (parName.compare("Muon") == 0)
    if (sigmamu != 999999.0 && fabs(sigmamu) > 0.00001) return sigmamu;
  if (parName.compare("Proton") == 0)
    if (sigmap != 999999.0 && fabs(sigmap) > 0.00001) return sigmap;
  if (parName.compare("Pion") == 0)
    if (sigmapi != 999999.0 && fabs(sigmapi) > 0.00001) return sigmapi;
  if (parName.compare("Kaon") == 0)
    if (sigmak != 999999.0 && fabs(sigmak) > 0.00001) return sigmak;

  return 999999;
}

//*************************************************
double TGlobalAnalysisUtils::GetTrackTpcPiddEdxexp(
    ND::TGlobalReconModule::TGlobalPID *globalTrack, std::string parName) {
  //*************************************************
  // give the pid corresponding to the tpc for which the momentum is maximal,
  // and the error on the curvature is minimal
  // if this doesn't exist, just take the first information we have.
  double dEdxexpel = 999999;
  double dEdxexpmu = 999999;
  double dEdxexpp = 999999;
  double dEdxexppi = 999999;
  double dEdxexpk = 999999;
  double relerr = 999999;
  double errmin = 999999;
  for (int jj = 0; jj < globalTrack->NTPCs; jj++) {
    ND::TGlobalReconModule::TTPCObject *tpcTrack =
        (ND::TGlobalReconModule::TTPCObject *)(*globalTrack->TPC)[jj];
    if (tpcTrack->FrontMomentum != 0) {
      relerr = tpcTrack->FrontMomentumError / tpcTrack->FrontMomentum;
      if (relerr < errmin) {
        errmin = relerr;
        dEdxexpel = tpcTrack->dEdxexpEle;
        dEdxexpmu = tpcTrack->dEdxexpMuon;
        dEdxexppi = tpcTrack->dEdxexpPion;
        dEdxexpp = tpcTrack->dEdxexpProton;
        dEdxexpk = tpcTrack->dEdxexpKaon;
      }
    }
  }

  if (relerr != 999999) {
    if (parName.compare("Electron") == 0)
      if (dEdxexpel != 999999.0 && fabs(dEdxexpel) > 0.00001) return dEdxexpel;
    if (parName.compare("Muon") == 0)
      if (dEdxexpmu != 999999.0 && fabs(dEdxexpmu) > 0.00001) return dEdxexpmu;
    if (parName.compare("Proton") == 0)
      if (dEdxexpp != 999999.0 && fabs(dEdxexpp) > 0.00001) return dEdxexpp;
    if (parName.compare("Pion") == 0)
      if (dEdxexppi != 999999.0 && fabs(dEdxexppi) > 0.00001) return dEdxexppi;
    if (parName.compare("Kaon") == 0)
      if (dEdxexpk != 999999.0 && fabs(dEdxexpk) > 0.00001) return dEdxexpk;
  }

  return 999999;
}
//*************************************************
double TGlobalAnalysisUtils::GetTrackTpcPiddEdxexp(
    ND::TGlobalReconModule::TGlobalPID *globalTrack, std::string parName,
    int itpc) {
  //*************************************************
  double dEdxexpel = 999999;
  double dEdxexpmu = 999999;
  double dEdxexpp = 999999;
  double dEdxexppi = 999999;
  double dEdxexpk = 999999;
  for (int jj = 0; jj < globalTrack->NTPCs; jj++) {
    ND::TGlobalReconModule::TTPCObject *tpcTrack =
        (ND::TGlobalReconModule::TTPCObject *)(*globalTrack->TPC)[jj];
    if (tpcTrack->FrontMomentum != 0) {
      int det = tpcTrack->Detector;
      if (det == itpc) {
        dEdxexpel = tpcTrack->dEdxexpEle;
        dEdxexpmu = tpcTrack->dEdxexpMuon;
        dEdxexppi = tpcTrack->dEdxexpPion;
        dEdxexpp = tpcTrack->dEdxexpProton;
        dEdxexpk = tpcTrack->dEdxexpKaon;
      }
    }
  }

  if (parName.compare("Electron") == 0)
    if (dEdxexpel != 999999.0 && fabs(dEdxexpel) > 0.00001) return dEdxexpel;
  if (parName.compare("Muon") == 0)
    if (dEdxexpmu != 999999.0 && fabs(dEdxexpmu) > 0.00001) return dEdxexpmu;
  if (parName.compare("Proton") == 0)
    if (dEdxexpp != 999999.0 && fabs(dEdxexpp) > 0.00001) return dEdxexpp;
  if (parName.compare("Pion") == 0)
    if (dEdxexppi != 999999.0 && fabs(dEdxexppi) > 0.00001) return dEdxexppi;
  if (parName.compare("Kaon") == 0)
    if (dEdxexpk != 999999.0 && fabs(dEdxexpk) > 0.00001) return dEdxexpk;

  return 999999;
}

//*************************************************
double TGlobalAnalysisUtils::GetEnergy(double m, double p) {
  //*************************************************

  double E = sqrt(pow(m, 2) + pow(p, 2));
  return E;
}

//*************************************************
TVector3 TGlobalAnalysisUtils::GetTrackMomentumVector(
    const ND::TGlobalReconModule::TGlobalPID &track) {
  //*************************************************

  TVector3 mom = track.BackDirection;
  // get momentum from tree
  double p = track.FrontMomentum;
  // build momentum vector
  mom *= p;
  return mom;
}

// bool TGlobalAnalysisUtils::IsInTPC(const ND::TGlobalReconModule::TGlobalPID&
// track) {
//*************************************************
bool TGlobalAnalysisUtils::IsInTPC(
    const ND::TGlobalReconModule::TGlobalPID *track) {
  //*************************************************
  int det = track->Detectors;
  char charDet[50];
  sprintf(charDet, "%d", det);
  string detectors;
  detectors.assign(charDet);
  // cout << "detectors = " << detectors << endl;
  string tpc1("1");
  string tpc2("2");
  string tpc3("3");
  size_t len;
  char charDet_i[20];

  for (int i = 0; i < (int)detectors.length(); i++) {
    len = detectors.copy(charDet_i, 1, i);
    charDet_i[len] = '\0';
    string detI;
    detI.assign(charDet_i);
    if (detI.compare(tpc1) == 0 || detI.compare(tpc2) == 0 ||
        detI.compare(tpc3) == 0) {
      // cout << "IN TPC!!!" << endl;
      return true;
    } else {
      // cout << "NOT IN TPC!!!" << endl;
      return false;
    }
  }
  return false;
}

//*************************************************
double TGlobalAnalysisUtils::GetNuInvariantMass(
    const ND::TGlobalReconModule::TGlobalPID &posTrack,
    const ND::TGlobalReconModule::TGlobalPID &negTrack) {
  //*************************************************

  double nuInvariantMass = -999;
  TLorentzVector proton4vector;
  TLorentzVector muon4vector;
  TLorentzVector neutron4vector;

  TVector3 protonMom = GetTrackMomentumVector(posTrack);
  double Ep = GetEnergy(fMp, protonMom.Mag());
  proton4vector.SetPxPyPzE(protonMom.X(), protonMom.Y(), protonMom.Z(), Ep);

  TVector3 muonMom = GetTrackMomentumVector(negTrack);
  double Emu = GetEnergy(fMmu, muonMom.Mag());
  muon4vector.SetPxPyPzE(muonMom.X(), muonMom.Y(), muonMom.Z(), Emu);

  neutron4vector.SetPxPyPzE(0., 0., 0., fMn);

  TLorentzVector sum = proton4vector + muon4vector - neutron4vector;
  nuInvariantMass = (sum * sum) * 1.e-6;  // GeV^2

  return nuInvariantMass;
}

//*************************************************
// double TGlobalAnalysisUtils::GetClusterDistanceFromTheVertex(const
// ND::TTrackerReconModule::TFgdCluster& cluster, const
// ND::TTrackerReconModule::TTrackerVertex& vertex) {
//*************************************************
//*************************************************
// double TGlobalAnalysisUtils::GetClusterDistanceFromTheVertex(const
// ND::TGlobalReconModule::TFgdCluster& cluster, TLorentzVector vertex) {
//*************************************************
/*
  //TLorentzVector dp = cluster.Position - vertex.Position;
  TLorentzVector dp = cluster.Position - vertex;

  // find distance between vertex position and hit position.
  double dist = 9999.0;
  TLorentzVector var = cluster.Variance;
  if(var.X() == 0.0)
    dist = sqrt(dp.X()*dp.X() + dp.Z()*dp.Z());
  else
    dist = sqrt(dp.Y()*dp.Y() + dp.Z()*dp.Z());

  return dist;
}
*/

//*************************************************
// bool TGlobalAnalysisUtils::IsAMichelElectronCandidate(const
// ND::TTrackerReconModule::TFgdCluster& cluster, const
// ND::TTrackerReconModule::TTrackerVertex& vertex, double michelCut) {
//*************************************************
//*************************************************
// bool TGlobalAnalysisUtils::IsAMichelElectronCandidate(const
// ND::TGlobalReconModule::TFgdCluster& cluster, TLorentzVector vertex, double
// michelCut) {
//*************************************************
/*
  bool foundMichelCluster = false;
  double dist = GetClusterDistanceFromTheVertex(cluster, vertex);
*/
/*
cout << "==============================================" << endl;
cout << "Z cluster position =" << cluster.Position.Z() << endl;
cout << "Z vertex position = " << vertex.Z() << endl;
cout << "dist = "<< dist << endl;
cout << "==============================================" << endl;
*/
/*
  if (dist < michelCut) {
    foundMichelCluster = true;
  }
  return foundMichelCluster;
}
*/

//*************************************************
// double TGlobalAnalysisUtils::GetClusterTimeSeparationFromVertex(const
// ND::TGlobalReconModule::TFgdCluster& cluster, TLorentzVector vertex) {
//*************************************************
/*
  TLorentzVector dp = cluster.Position - vertex;

  //cout << "cluster time = " << cluster.Position.T() << endl;
  //cout << "vertex time = " << vertex.T() << endl;

  // find time separation between vertex position and hit position.
  double timeSep = -9999.;
  timeSep = dp.T();

  //cout << "timeSep = "<< timeSep << endl;

  return timeSep;
}
*/

//*************************************************
bool TGlobalAnalysisUtils::IsNotAShower(
    const ND::TGlobalReconModule::TGlobalPID &track) {
  //*************************************************

  bool isNotAShower = false;
  double MIP_EM = -9999;
  double MIP_Pion = -9999;

  TClonesArray *ECAL = track.ECAL;
  if (track.NECALs > 0) {
    for (int j = 0; j < track.NECALs; j++) {
      ND::TGlobalReconModule::TECALObject *ecal =
          (ND::TGlobalReconModule::TECALObject *)(*ECAL)[j];
      MIP_EM = ecal->LLR_MIP_EM;
      MIP_Pion = ecal->LLR_MIP_Pion;
      // Assume track if not very sure that it is showering
      return ((!(MIP_EM > 0)) && (!(MIP_Pion > 0)));
    }
  }
  return true;
}

// Check if the recon track has FGD constituent
//*************************************************
bool TGlobalAnalysisUtils::IsInFGD(
    const ND::TGlobalReconModule::TGlobalPID *track) {
  //*************************************************
  int det = track->Detectors;
  char charDet[50];
  sprintf(charDet, "%d", det);
  string detectors;
  detectors.assign(charDet);

  // cout << "detectors = " << detectors << endl;

  string fgd1("4");
  string fgd2("5");

  size_t found1;
  size_t found2;

  found1 = detectors.find(fgd1);
  found2 = detectors.find(fgd2);

  if ((found1 != string::npos) || (found2 != string::npos)) {
    // cout << "IN FGD!!!" << endl;
    return true;
  }

  if ((found1 == string::npos) && (found2 == string::npos)) {
    // cout << "NOT IN FGD!!!" << endl;
    return false;
  }
  return false;
}

// Check if the recon track has both TPC and FGD constituent
//*************************************************
bool TGlobalAnalysisUtils::IsInTPCAndInFGD(
    const ND::TGlobalReconModule::TGlobalPID *track) {
  //*************************************************
  int det = track->Detectors;
  char charDet[50];
  sprintf(charDet, "%d", det);
  string detectors;
  detectors.assign(charDet);

  // cout << "detectors = " << detectors << endl;

  string tpc1("1");
  string tpc2("2");
  string tpc3("3");
  string fgd1("4");
  string fgd2("5");

  size_t found1 = detectors.find(tpc1);
  size_t found2 = detectors.find(tpc2);
  size_t found3 = detectors.find(tpc3);
  size_t found4 = detectors.find(fgd1);
  size_t found5 = detectors.find(fgd2);

  if (((found1 != string::npos) && (found4 != string::npos)) ||
      ((found1 != string::npos) && (found5 != string::npos)) ||
      ((found2 != string::npos) && (found4 != string::npos)) ||
      ((found2 != string::npos) && (found5 != string::npos)) ||
      ((found3 != string::npos) && (found4 != string::npos)) ||
      ((found3 != string::npos) && (found5 != string::npos))) {
    // cout << "detectors = " << detectors << " ----> YES " << endl;
    return true;
  } else {
    // cout << "detectors = " << detectors << " ----> NO " << endl;
    return false;
  }
}

//*************************************************
// MC information is needed by GetBunch(track,mc) but not by vertex algorithm
TLorentzVector TGlobalAnalysisUtils::ComputeRecVertex(TClonesArray *PIDs,
                                                      vector<int> &charge_mod,
                                                      double &quality,
                                                      int version, int bunch,
                                                      bool isMC) {
  int NTracks = PIDs->GetEntries();

  quality = 0;

  // This is the vertex position
  TLorentzVector RecVertex;

  // No PIDs, nothing to do
  if (NTracks == 0) return RecVertex;

  // Containers for the cluster positions
  vector<vector<int> > cluster_list;
  vector<double> cluster_x;
  vector<double> cluster_y;
  vector<double> cluster_z;
  vector<double> cluster_time;
  vector<double> cluster_rank;
  vector<int> cluster_hm;
  int nclusters = 0;

  // Container for the bunch mask
  vector<int> PID_bunch;

  // Settings (these should maybe move somewhere else?)
  double max_dist = 500;      // mm
  double low_cl_max = 200;    // mm
  double link_xdist = 50;     // mm
  double link_ydist = 100;    // mm
  double max_fit_dist = 200;  // mm
  double max_dot = 0.995;
  double max_dot_ab = 0.8;
  double unc = 20;    // mm
  double z_step = 5;  // mm

  // Number of tracks in this bunch
  int NTracks_bunch = 0;

  // Look for the global hm track
  vector<double> moms;
  TLorentzVector RecVertex_hm, RecVertex_hm_other, lowz_pos;
  TVector3 VDir_hm, VDir_hm_other, lowz_dir;
  int hm_PID = -1;
  double hm_mom = 0;
  int lowz_PID = -1;
  double lowz = 1e6;
  double lowz_mom = 0;
  for (int j = 0; j < NTracks; j++) {
    TLorentzVector this_pos, this_pos_other;
    TVector3 this_dir, this_dir_other;
    ND::TGlobalReconModule::TGlobalPID *track =
        (ND::TGlobalReconModule::TGlobalPID *)(*PIDs)[j];

    // Find the bunch and save bunch mask
    int this_bunch = GetBunch(track, isMC);
    if (this_bunch == bunch)
      PID_bunch.push_back(1);
    else
      PID_bunch.push_back(0);

    // Save the charge for later
    charge_mod.push_back((int)track->Charge);

    // Get momentum
    double mom = fabs(track->FrontMomentum);
    if (mom != mom) mom = 0;  // nan check

    // Is the track passing a TPC?
    bool isTPC = IsInTPC(track);

    // Downgrading of momentum
    if (mom > 9000)
      mom = 1e-4;  // Downgrade the FGD only tracks
    else if (!isTPC)
      mom = 1e-3;  // Downgrade tracks without TPC

    // Save the momentum for later
    moms.push_back(mom);

    // Check if we're in the right bunch
    if (!PID_bunch[j]) continue;

    // Get position and direction
    double front_z = track->FrontPosition.Z();
    double back_z = track->BackPosition.Z();
    if ((track->isForward && (front_z <= back_z)) ||
        (!track->isForward && (front_z > back_z))) {
      this_pos = track->FrontPosition;
      this_dir = track->FrontDirection;
      this_pos_other = track->BackPosition;
      this_dir_other = track->BackDirection;
    } else {
      this_pos = track->BackPosition;
      this_dir = track->BackDirection;
      this_pos_other = track->FrontPosition;
      this_dir_other = track->FrontDirection;
    }

    // If there's something wrong with the direction
    // the track can't be used
    if (this_dir.Z() != this_dir.Z() ||
        this_dir_other.Z() != this_dir_other.Z())
      continue;
    if (fabs(this_dir.Z()) < 1e-10 || fabs(this_dir_other.Z()) < 1e-10)
      continue;

    // Increment tracks in bunch
    NTracks_bunch++;

    // Save the lowest starting forward track with TPC
    if (isTPC && this_pos.Z() < lowz && this_dir.Z() > 0) {
      lowz = this_pos.Z();
      lowz_mom = mom;
      lowz_PID = j;
      lowz_pos = this_pos;
      lowz_dir = this_dir;
    }

    if (mom > hm_mom) {
      hm_mom = mom;
      hm_PID = j;
      RecVertex_hm = this_pos;
      VDir_hm = this_dir;
      RecVertex_hm_other = this_pos_other;
      VDir_hm_other = this_dir_other;
    }
  }
  // If no tracks in this bunch
  if (NTracks_bunch == 0) return RecVertex;

  quality = 1;  // this is for the one track case

  // Start with assuming that the start of hm track is primary vertex
  // This also will cover the case nPID = 1
  RecVertex = RecVertex_hm;

  // Version 1 ends here
  if (version == 1) return RecVertex;

  // Version 2 starts here
  //---------------------------------------------------------
  if (NTracks_bunch == 1) return RecVertex;

  // Do we have the right hm track?
  if (lowz_PID > -1 && lowz_PID != hm_PID &&
      fabs(lowz_pos.Z() - RecVertex_hm.Z()) > unc) {
    TLorentzVector hm_p;
    TVector3 hm_v;
    if (fabs(RecVertex_hm.Z() - lowz_pos.Z()) <
        fabs(RecVertex_hm_other.Z() - lowz_pos.Z())) {
      hm_p = RecVertex_hm;
      hm_v = VDir_hm;
    } else {
      hm_p = RecVertex_hm_other;
      hm_v = VDir_hm_other;
    }

    // New position
    TVector3 pluspos = hm_p.Vect();
    double hm_p_t = (lowz_pos.Z() - hm_p.Z()) / hm_v.Z();
    pluspos += hm_p_t * hm_v;

    // If diff below condition, take upstream track as hm
    if (fabs(lowz_pos.X() - pluspos.X()) < low_cl_max) {
      hm_PID = lowz_PID;
      hm_mom = lowz_mom;
      RecVertex_hm = lowz_pos;
      VDir_hm = lowz_dir;
      RecVertex = RecVertex_hm;
    }
  }

  // Loop over PID positions
  // search for clusters
  //------------------------------------------------------------------
  for (int j = 0; j < NTracks; j++) {
    // Check if we're in the right bunch
    if (!PID_bunch[j]) continue;

    TLorentzVector thisalpos, thisahpos;
    TVector3 thisaldir, thisahdir;
    ND::TGlobalReconModule::TGlobalPID *track_a =
        (ND::TGlobalReconModule::TGlobalPID *)(*PIDs)[j];
    if (track_a->FrontPosition.Z() <= track_a->BackPosition.Z()) {
      thisalpos = track_a->FrontPosition;
      thisahpos = track_a->BackPosition;
      thisaldir = track_a->FrontDirection;
      thisahdir = track_a->BackDirection;
    } else {
      thisalpos = track_a->BackPosition;
      thisahpos = track_a->FrontPosition;
      thisaldir = track_a->BackDirection;
      thisahdir = track_a->FrontDirection;
    }

    if (track_a->Detectors > 5 && track_a->Detectors < 10)
      continue;  // Skip iso tracks if not in TPC,FGD
    if (thisaldir.Z() != thisaldir.Z() || thisahdir.Z() != thisahdir.Z())
      continue;
    if (fabs(thisaldir.Z()) < 1e-10 || fabs(thisahdir.Z()) < 1e-10) continue;

    // If the ends are very different in direction we might have a kink on the
    // track
    TVector3 thisaldir_noy = thisaldir;
    thisaldir_noy.SetY(0);
    double thisal_noy_mag = thisaldir_noy.Mag();
    thisaldir_noy.SetMag(1.0);
    TVector3 thisahdir_noy = thisahdir;
    thisahdir_noy.SetY(0);
    double thisah_noy_mag = thisahdir_noy.Mag();
    thisahdir_noy.SetMag(1.0);
    double a_dot_p = thisaldir_noy.Dot(thisahdir_noy);

    // Second loop over PID tracks b
    for (int jj = j + 1; jj < NTracks; jj++) {
      // Check if we're in the right bunch
      if (!PID_bunch[jj]) continue;

      TLorentzVector thisblpos, thisbhpos;
      TVector3 thisbldir, thisbhdir;
      ND::TGlobalReconModule::TGlobalPID *track_b =
          (ND::TGlobalReconModule::TGlobalPID *)(*PIDs)[jj];
      if (track_b->FrontPosition.Z() <= track_b->BackPosition.Z()) {
        thisblpos = track_b->FrontPosition;
        thisbhpos = track_b->BackPosition;
        thisbldir = track_b->FrontDirection;
        thisbhdir = track_b->BackDirection;
      } else {
        thisblpos = track_b->BackPosition;
        thisbhpos = track_b->FrontPosition;
        thisbldir = track_b->BackDirection;
        thisbhdir = track_b->FrontDirection;
      }

      if (track_b->Detectors > 5 && track_b->Detectors < 10)
        continue;  // Skip iso tracks if not in TPC,FGD
      if (thisbldir.Z() != thisbldir.Z() || thisbhdir.Z() != thisbhdir.Z())
        continue;
      if (fabs(thisbldir.Z()) < 1e-10 || fabs(thisbhdir.Z()) < 1e-10) continue;

      // If the ends are very different in direction we might have a kink on the
      // track
      TVector3 thisbldir_noy = thisbldir;
      thisbldir_noy.SetY(0);
      double thisbl_noy_mag = thisbldir_noy.Mag();
      thisbldir_noy.SetMag(1.0);
      TVector3 thisbhdir_noy = thisbhdir;
      thisbhdir_noy.SetY(0);
      double thisbh_noy_mag = thisbhdir_noy.Mag();
      thisbhdir_noy.SetMag(1.0);
      double b_dot_p = thisbldir_noy.Dot(thisbhdir_noy);

      // Only two combinations should be propagated, find which
      // all ends should be used!
      // of these (max three) one should be best

      // Find the track combination case
      bool ahbl = 0;
      bool albh = 0;
      double dot_p = 0;
      if (thisalpos.Z() <= thisblpos.Z()) {
        if (thisahpos.Z() - unc <= thisblpos.Z()) {
          dot_p = thisahdir_noy.Dot(thisbldir_noy);
          if (thisahpos == thisalpos) {
            double dot_p2 = thisaldir_noy.Dot(thisbldir_noy);
            if (fabs(dot_p2) > max_dot) dot_p = dot_p2;
          }
          if (thisblpos == thisbhpos) {
            double dot_p2 = thisahdir_noy.Dot(thisbhdir_noy);
            if (fabs(dot_p2) > max_dot) dot_p = dot_p2;
          }
          // Not a broken track
          if (fabs(dot_p) < max_dot) ahbl = 1;
        }
        // overlayered in z
        else {
          // doubled track
          dot_p = thisahdir_noy.Dot(thisbhdir_noy);
          double dot_p2 = thisaldir_noy.Dot(thisbldir_noy);
          if (fabs(dot_p2) > max_dot) dot_p = dot_p2;

          // A kink track
          if (fabs(a_dot_p) < max_dot_ab || fabs(b_dot_p) < max_dot_ab)
            ahbl = 1;
        }
      }
      // b lower
      else {
        if (thisalpos.Z() + unc >= thisbhpos.Z()) {
          dot_p = thisaldir_noy.Dot(thisbhdir_noy);
          if (thisahpos == thisalpos) {
            double dot_p2 = thisahdir_noy.Dot(thisbhdir_noy);
            if (fabs(dot_p2) > max_dot) dot_p = dot_p2;
          }
          if (thisblpos == thisbhpos) {
            double dot_p2 = thisaldir_noy.Dot(thisbldir_noy);
            if (fabs(dot_p2) > max_dot) dot_p = dot_p2;
          }
          // Not a broken track
          if (fabs(dot_p) < max_dot) albh = 1;
        }
        // overlayered in z
        else {
          // doubled track
          dot_p = thisahdir_noy.Dot(thisbhdir_noy);
          double dot_p2 = thisaldir_noy.Dot(thisbldir_noy);
          if (fabs(dot_p2) > max_dot) dot_p = dot_p2;

          // A kink track
          if (fabs(a_dot_p) < max_dot_ab || fabs(b_dot_p) < max_dot_ab)
            albh = 1;
        }
      }

      // If broken track, take upstream end if this is the only track
      if (fabs(dot_p) > max_dot) {
        if (NTracks_bunch == 2) {
          if (ahbl) {
            if (thisaldir.Z() > 0)
              RecVertex = thisalpos;
            else
              RecVertex = thisbhpos;
          } else if (albh) {
            if (thisaldir.Z() > 0)
              RecVertex = thisblpos;
            else
              RecVertex = thisahpos;
          }
          break;
        }
        continue;  // or at least skip this cluster
      }

      // Closest approach calculation
      // al-bl always
      TVector3 p1 = thisalpos.Vect();
      p1 += -thisblpos.Vect();
      double adir_bdir1 = thisaldir_noy.Dot(thisbldir_noy);
      double t_a1 = -p1.Dot(thisaldir_noy) + adir_bdir1 * p1.Dot(thisbldir_noy);
      if (fabs(adir_bdir1) < 1)
        t_a1 /= (1.0 - adir_bdir1 * adir_bdir1);
      else
        t_a1 = 0;
      TVector3 closest_a1 = thisalpos.Vect();
      closest_a1 += (t_a1 / thisal_noy_mag) * thisaldir;
      TVector3 closest_b1 = thisblpos.Vect();
      double t_b1 =
          (closest_a1.Z() - thisblpos.Z()) / (thisbldir.Z() / thisbl_noy_mag);
      closest_b1 += (t_b1 / thisbl_noy_mag) * thisbldir;
      double clt1 = thisalpos.T();

      // ah-bh always
      TVector3 p2 = thisahpos.Vect();
      p2 += -thisbhpos.Vect();
      double adir_bdir2 = thisahdir_noy.Dot(thisbhdir_noy);
      double t_a2 = -p2.Dot(thisahdir_noy) + adir_bdir2 * p2.Dot(thisbhdir_noy);
      if (fabs(adir_bdir2) < 1)
        t_a2 /= (1.0 - adir_bdir2 * adir_bdir2);
      else
        t_a2 = 0;
      TVector3 closest_a2 = thisahpos.Vect();
      closest_a2 += (t_a2 / thisah_noy_mag) * thisahdir;
      double t_b2 =
          (closest_a2.Z() - thisbhpos.Z()) / (thisbhdir.Z() / thisbh_noy_mag);
      TVector3 closest_b2 = thisbhpos.Vect();
      closest_b2 += (t_b2 / thisbh_noy_mag) * thisbhdir;
      double clt2 = thisahpos.T();

      TVector3 closest_a3(0, 0, 0);
      TVector3 closest_b3(0, 0, 0);
      double clt3 = 0;
      double dza3 = 0;
      double dzb3 = 0;
      // ah-bl if applicable
      if (ahbl) {
        TVector3 p3 = thisahpos.Vect();
        p3 += -thisblpos.Vect();
        double adir_bdir3 = thisahdir_noy.Dot(thisbldir_noy);
        double t_a3 =
            -p3.Dot(thisahdir_noy) + adir_bdir3 * p3.Dot(thisbldir_noy);
        if (fabs(adir_bdir3) < 1)
          t_a3 /= (1.0 - adir_bdir3 * adir_bdir3);
        else
          t_a3 = 0;
        closest_a3 += thisahpos.Vect();
        closest_a3 += (t_a3 / thisah_noy_mag) * thisahdir;
        double t_b3 =
            (closest_a3.Z() - thisblpos.Z()) / (thisbldir.Z() / thisbl_noy_mag);
        closest_b3 += thisblpos.Vect();
        closest_b3 += (t_b3 / thisbl_noy_mag) * thisbldir;
        clt3 = thisahpos.T();
        dza3 = closest_a3.Z() - thisahpos.Z();
        dzb3 = closest_b3.Z() - thisblpos.Z();
      }
      // al-bh if applicable
      else if (albh) {
        TVector3 p3 = thisalpos.Vect();
        p3 += -thisbhpos.Vect();
        double adir_bdir3 = thisaldir_noy.Dot(thisbhdir_noy);
        double t_a3 =
            -p3.Dot(thisaldir_noy) + adir_bdir3 * p3.Dot(thisbhdir_noy);
        if (fabs(adir_bdir3) < 1)
          t_a3 /= (1.0 - adir_bdir3 * adir_bdir3);
        else
          t_a3 = 0;
        closest_a3 += thisalpos.Vect();
        closest_a3 += (t_a3 / thisal_noy_mag) * thisaldir;
        double t_b3 =
            (closest_a3.Z() - thisbhpos.Z()) / (thisbhdir.Z() / thisbh_noy_mag);
        closest_b3 += thisbhpos.Vect();
        closest_b3 += (t_b3 / thisbh_noy_mag) * thisbhdir;
        clt3 = thisalpos.T();
        dza3 = closest_a3.Z() - thisalpos.Z();
        dzb3 = closest_b3.Z() - thisbhpos.Z();
      }

      // Border conditions
      double dza1 = closest_a1.Z() - thisalpos.Z();
      double dzb1 = closest_b1.Z() - thisblpos.Z();
      double dza2 = closest_a2.Z() - thisahpos.Z();
      double dzb2 = closest_b2.Z() - thisbhpos.Z();

      // No interpolation for al-bl or ah-bh
      if (dza1 - unc > 0 || dzb1 - unc > 0) closest_a1.SetXYZ(0, 0, 0);
      if (dza2 + unc < 0 || dzb2 + unc < 0) closest_a2.SetXYZ(0, 0, 0);
      if (closest_a3.Mag() > 0) {
        // or for ah-bl
        if (ahbl && thisahpos.Z() - unc <= thisblpos.Z()) {
          if ((closest_a3.Z() - thisahpos.Z() + unc < 0) ||
              (closest_a3.Z() - thisblpos.Z() - unc > 0))
            closest_a3.SetXYZ(0, 0, 0);
        }
        // or for al-bh
        else if (albh && thisalpos.Z() + unc >= thisbhpos.Z()) {
          if ((closest_a3.Z() - thisbhpos.Z() + unc < 0) ||
              (closest_a3.Z() - thisalpos.Z() - unc > 0))
            closest_a3.SetXYZ(0, 0, 0);
        }
        // but require interpolation on a or b for the kink tracks
        else {
          if (fabs(a_dot_p) < max_dot_ab &&
              ((closest_a3.Z() - thisalpos.Z() + unc < 0) ||
               (closest_a3.Z() - thisahpos.Z() - unc > 0)))
            closest_a3.SetXYZ(0, 0, 0);
          else if (fabs(b_dot_p) < max_dot_ab &&
                   ((closest_a3.Z() + thisblpos.Z() + unc < 0) ||
                    (closest_a3.Z() - thisbhpos.Z() - unc > 0)))
            closest_a3.SetXYZ(0, 0, 0);
        }
      }

      // Extrapolation conditions, don't go too far
      if (fabs(dza1) > max_dist || fabs(dzb1) > max_dist)
        closest_a1.SetXYZ(0, 0, 0);
      if (fabs(dza2) > max_dist || fabs(dzb2) > max_dist)
        closest_a2.SetXYZ(0, 0, 0);
      if (fabs(dza3) > max_dist || fabs(dzb3) > max_dist)
        closest_a3.SetXYZ(0, 0, 0);

      // track length in z
      double tlz_a = thisahpos.Z() - thisalpos.Z();
      double tlz_b = thisbhpos.Z() - thisblpos.Z();
      if (fabs(dza1) > tlz_a || fabs(dzb1) > tlz_b) closest_a1.SetXYZ(0, 0, 0);
      if (fabs(dza2) > tlz_a || fabs(dzb2) > tlz_b) closest_a2.SetXYZ(0, 0, 0);
      if (fabs(dza3) > tlz_a || fabs(dzb3) > tlz_b) closest_a3.SetXYZ(0, 0, 0);

      // Select the best combination by looking at y diff
      TVector3 plusapos(0, 0, 0);
      TVector3 plusbpos(0, 0, 0);
      double dy = 1e6;  // diff in y
      double clt = 0;   // Cluster time
      if (closest_a1.Mag() > 0) {
        double this_dy = fabs(closest_a1.Y() - closest_b1.Y());
        dy = this_dy;
        plusapos = closest_a1;
        plusbpos = closest_b1;
        clt = clt1;
      }
      if (closest_a2.Mag() > 0) {
        double this_dy = fabs(closest_a2.Y() - closest_b2.Y());
        if (this_dy < dy) {
          plusapos = closest_a2;
          plusbpos = closest_b2;
          dy = this_dy;
          clt = clt2;
        }
      }
      if (closest_a3.Mag() > 0) {
        double this_dy = fabs(closest_a3.Y() - closest_b3.Y());
        if (this_dy < dy) {
          plusapos = closest_a3;
          plusbpos = closest_b3;
          dy = this_dy;
          clt = clt3;
        }
      }
      // No link found
      if (plusapos.Mag() == 0 && plusbpos.Mag() == 0) {
        continue;
      }

      // If close enough in x and y, form cluster
      if (fabs(plusapos.X() - plusbpos.X()) < link_xdist &&
          fabs(plusapos.Y() - plusbpos.Y()) < link_ydist) {
        // Add cluster to list, if not already there
        int vi = -1;
        for (unsigned int i = 0; i < cluster_z.size(); i++) {
          if (fabs(cluster_z[i] - plusapos.Z()) < unc) {
            vi = i;
            break;
          }
        }
        // A closeby cluster exists
        if (vi >= 0) {
          bool jbool = false;
          bool jjbool = false;
          for (unsigned int i = 0; i < cluster_list[vi].size(); i++) {
            if (cluster_list[vi][i] == j) jbool = true;
            if (cluster_list[vi][i] == jj) jjbool = true;
          }
          // Neither j nor jj in list
          // Add new cluster if distance to cluster mean too big
          if (!jbool && !jjbool) {
            if (fabs(plusapos.X() - cluster_x[vi] / cluster_list[vi].size()) >
                    link_xdist &&
                fabs(plusbpos.X() - cluster_x[vi] / cluster_list[vi].size()) >
                    link_xdist)
              vi = -1;
          }
          // Adding j to list
          if (!jbool && vi >= 0) {
            cluster_list[vi].push_back(j);
            cluster_x[vi] += plusapos.X();
            cluster_y[vi] += plusapos.Y();
            if (moms[j] > cluster_rank[vi]) {
              cluster_rank[vi] = moms[j];
              cluster_hm[vi] = j;
            }
          }
          // Adding jj to list
          if (!jjbool && vi >= 0) {
            cluster_list[vi].push_back(jj);
            cluster_x[vi] += plusbpos.X();
            cluster_y[vi] += plusbpos.Y();
            if (moms[jj] > cluster_rank[vi]) {
              cluster_rank[vi] = moms[jj];
              cluster_hm[vi] = jj;
            }
          }
        }

        // Add new cluster
        if (vi < 0) {
          vector<int> vpids;
          cluster_list.push_back(vpids);
          cluster_x.push_back(plusapos.X() + plusbpos.X());
          cluster_y.push_back(plusapos.Y() + plusbpos.Y());
          cluster_z.push_back(plusapos.Z());
          cluster_time.push_back(clt);
          cluster_list[nclusters].push_back(j);
          cluster_list[nclusters].push_back(jj);
          if (moms[j] >= moms[jj]) {
            cluster_rank.push_back(moms[j]);
            cluster_hm.push_back(j);
          } else {
            cluster_rank.push_back(moms[jj]);
            cluster_hm.push_back(jj);
          }
          nclusters++;
        }
      }  // End separation check
    }    // end second PID loop
  }      // end first PID loop

  // If we have clusters, examine them
  //------------------------------------------------------------
  if (cluster_z.size() > 0) {
    // Index of primary vertex
    int prim_cl_i = -1;

    // Find lowest highest rank cluster
    // first add hm track as a cluster
    vector<int> vpids;
    cluster_list.push_back(vpids);
    cluster_list[nclusters].push_back(hm_PID);
    cluster_z.push_back(RecVertex_hm.Z());
    cluster_rank.push_back(hm_mom);

    double max_rank = 0;
    double max_rank_lowz = 1e6;
    for (unsigned int i = 0; i < cluster_z.size(); i++) {
      if (cluster_list[i].size() == 0) continue;
      if (cluster_rank[i] > max_rank) {
        max_rank = cluster_rank[i];
        max_rank_lowz = cluster_z[i];
        prim_cl_i = i;
      } else if (cluster_rank[i] == max_rank) {
        if (cluster_z[i] < max_rank_lowz) {
          max_rank_lowz = cluster_z[i];
          prim_cl_i = i;
        }
      }
    }

    /*
    //OLD METHOD:
    //Get the average x, y, t of clusters
    //and cluster with max and min z
    double cl_low = 1e6;
    double cl_high = -1e6;
    int cl_low_i = 0;
    int cl_high_i = 0;
    for(int i = 0; i < cluster_z.size(); i ++){
      if(cluster_list[i].size() == 0) continue;
      cluster_x[i] *= 1.0/(cluster_list[i].size());
      cluster_y[i] *= 1.0/(cluster_list[i].size());
      if(cluster_z[i] < cl_low){
        cl_low = cluster_z[i];
        cl_low_i = i;
      }
      if(cluster_z[i] > cl_high){
        cl_high = cluster_z[i];
        cl_high_i = i;
      }
    }

    //Pick a primary cluster
    double hm_dist = 1e6;
    for(int i = 0; i < cluster_z.size(); i ++){
      if(cluster_list[i].size() == 0) continue;
      //Global hm track and right end if several connections
      if((thisPID == hm_PID) && ((fabs(RecVertex_hm.Z() - cluster_z[i]) <
    hm_dist))){
        prim_cl_i = i;
        hm_dist = fabs(RecVertex_hm.Z()-cluster_z[i]);
      }
    }

    if(prim_cl_i >= 0){
     //If cluster is on the middle of the hm track
      if((cluster_z[prim_cl_i]-unc > RecVertex_hm.Z() && RecVertex_hm_other.Z()
    > RecVertex_hm.Z()) ||
         (cluster_z[prim_cl_i]+unc < RecVertex_hm.Z() && RecVertex_hm_other.Z()
    < RecVertex_hm.Z())){

        //If not a kink on hm track, set hm track start as vertex
        TVector3 thisdira_noy = VDir_hm;
        thisdira_noy.SetY(0);
        thisdira_noy.SetMag(1.0);
        TVector3 thisdirb_noy = VDir_hm_other;
        thisdirb_noy.SetY(0);
        thisdirb_noy.SetMag(1.0);
        double a_dot_b = thisdira_noy.Dot(thisdirb_noy);
        if(fabs(a_dot_b) > max_dot_ab){
          prim_cl_i = -1;
        }
      }
      //The "wrong" end of the hm track is connected to the cluster
      //hm track sense used here
      else if(fabs(cluster_z[prim_cl_i]-RecVertex_hm_other.Z()) <
    fabs(cluster_z[prim_cl_i]-RecVertex_hm.Z())){
        //temporary solution: set cluster as secondary
        prim_cl_i = -1;
      }
    }

    //hm track not in clusters
    //or clusters upstream from hm cluster?
    //hm track sense used again
    if(prim_cl_i < 0 || prim_cl_i != cl_low_i){

      //See if hm track is moving in right direction
      //could it be created upstream?
      //Only allow higher cluster if hm not connected
      int high_low = 0;
      if(VDir_hm.Z() > 0 && RecVertex_hm.Z() > cl_low) high_low = -1;
      else if(VDir_hm.Z() < 0 && RecVertex_hm.Z() < cl_high && prim_cl_i < 0)
    high_low = 1;

      //If hm track forward but not lowest in z
      //or backward and not highest in z
      //take end cluster
      TVector3 end_pos;
      if(VDir_hm.Z() > 0){
        //Position of lowest cluster in z
        end_pos.SetXYZ(cluster_x[cl_low_i],cluster_y[cl_low_i],cluster_z[cl_low_i]);
      }
      else{
        //Position of highest cluster in z
        end_pos.SetXYZ(cluster_x[cl_high_i],cluster_y[cl_high_i],cluster_z[cl_high_i]);
      }

      ND::TGlobalReconModule::TGlobalPID *hm_track =
    (ND::TGlobalReconModule::TGlobalPID*)(*PIDs)[hm_PID];
      TVector3 hm_v;
      TLorentzVector hm_p;
      //Extrapolate from closest end
      if(fabs(hm_track->FrontPosition.Z() - end_pos.Z()) <
    fabs(hm_track->BackPosition.Z() - end_pos.Z())){
        hm_p = hm_track->FrontPosition;
        hm_v = hm_track->FrontDirection;
      }
      else{
        hm_p = hm_track->BackPosition;
        hm_v = hm_track->BackDirection;
      }

      //New position
      TVector3 pluspos = hm_p.Vect();
      double hm_p_t = (end_pos.Z() - hm_p.Z())/hm_v.Z();
      pluspos += hm_p_t*hm_v;

      //If diff below condition, take upstream cluster
      if(fabs(end_pos.X() - pluspos.X()) < low_cl_max){
        if(high_low < 0) prim_cl_i = cl_low_i;
        if(high_low > 0) prim_cl_i = cl_high_i;
      }
    }
    */

    // Primary cluster not found
    if (prim_cl_i < 0) return RecVertex;

    // Look at the associated tracks
    int nAss = cluster_list[prim_cl_i].size();
    if (nAss > 1) {
      // Fit the primary vertex position to the tracks in cluster
      // Extrapolate from cluster hm track
      // Simple least squares method in steps of z_step mm
      double all_sq_sum = 1e18;

      int cl_hm_PID = cluster_hm[prim_cl_i];
      TLorentzVector cl_hm_p;
      TVector3 cl_hm_v;
      ND::TGlobalReconModule::TGlobalPID *cl_hm_track =
          (ND::TGlobalReconModule::TGlobalPID *)(*PIDs)[cl_hm_PID];
      // Extrapolate from closest end
      if (fabs(cl_hm_track->FrontPosition.Z() - cluster_z[prim_cl_i]) <
          fabs(cl_hm_track->BackPosition.Z() - cluster_z[prim_cl_i])) {
        cl_hm_p = cl_hm_track->FrontPosition;
        cl_hm_v = cl_hm_track->FrontDirection;
      } else {
        cl_hm_p = cl_hm_track->BackPosition;
        cl_hm_v = cl_hm_track->BackDirection;
      }

      // Start position
      TVector3 cl_hm_pluspos = cl_hm_p.Vect();
      double cl_hm_t = (cluster_z[prim_cl_i] - cl_hm_p.Z()) / cl_hm_v.Z();
      cl_hm_pluspos += cl_hm_t * cl_hm_v;
      RecVertex.SetVect(cl_hm_pluspos);
      RecVertex.SetT(cl_hm_p.T());

      // Minimization loop
      int nstep = int(max_fit_dist / z_step);
      for (int i = 0; i <= nstep; i++) {
        double this_z = cluster_z[prim_cl_i] + (i - nstep / 2) * z_step;
        double sq_sum = 0;
        TVector3 allpos(0, 0, 0);

        // extrapolate hm track to this_z
        cl_hm_pluspos = cl_hm_p.Vect();
        cl_hm_t = (this_z - cl_hm_p.Z()) / cl_hm_v.Z();
        cl_hm_pluspos += cl_hm_t * cl_hm_v;

        bool interpolate = false;
        // Loop over PIDs in cluster, extrapolate
        for (int j = 0; j < nAss; j++) {
          int this_PID = cluster_list[prim_cl_i][j];
          if (this_PID == cl_hm_PID) continue;

          // Find the point to extrapolate
          ND::TGlobalReconModule::TGlobalPID *track =
              (ND::TGlobalReconModule::TGlobalPID *)(*PIDs)[this_PID];
          TLorentzVector thisp, thisp_other;
          TVector3 thisv, thisv_other;
          if (fabs(track->FrontPosition.Z() - cluster_z[prim_cl_i]) <
              fabs(track->BackPosition.Z() - cluster_z[prim_cl_i])) {
            thisp = track->FrontPosition;
            thisv = track->FrontDirection;
            thisp_other = track->BackPosition;
            thisv_other = track->BackDirection;
          } else {
            thisp = track->BackPosition;
            thisv = track->BackDirection;
            thisp_other = track->FrontPosition;
            thisv_other = track->FrontDirection;
          }

          // Check if this_z lies between the ends of the track
          // or if there's a kink
          if ((this_z - thisp.Z() + unc < 0 &&
               this_z - thisp_other.Z() - unc > 0) ||
              (this_z - thisp.Z() - unc > 0 &&
               this_z - thisp_other.Z() + unc > 0)) {
            TVector3 thisv_noy = thisv;
            thisv_noy.SetY(0);
            thisv_noy.SetMag(1.0);
            TVector3 thisv_other_noy = thisv_other;
            thisv_other_noy.SetY(0);
            thisv_other_noy.SetMag(1.0);
            double a_dot_b = thisv_noy.Dot(thisv_other_noy);
            if (fabs(a_dot_b) > max_dot_ab) {
              interpolate = true;
              break;
            }
          }

          // Extrapolate to this_z
          TVector3 pluspos = thisp.Vect();
          double pluspos_t = (this_z - thisp.Z()) / thisv.Z();
          pluspos += pluspos_t * thisv;
          TVector3 pdiffv = cl_hm_pluspos;
          pdiffv += -pluspos;
          sq_sum += fabs(pdiffv.Mag2());
        }

        // Do not interpolate on the tracks
        if (interpolate) continue;

        // Look for the minimum
        if (sq_sum < all_sq_sum) {
          all_sq_sum = sq_sum;
          RecVertex.SetVect(cl_hm_pluspos);
        }
      }
      cluster_z[prim_cl_i] = RecVertex.Z();  // for book keeping

      // set the quality parameter to something chi2-like
      quality = sqrt(all_sq_sum) / nAss;
    }  // end if nAss > 1

    // See if some tracks at primary vertex need to change direction
    for (unsigned int j = 0; j < cluster_list[prim_cl_i].size(); j++) {
      int this_PID = cluster_list[prim_cl_i][j];

      // Check if we're in the right bunch
      if (!PID_bunch[this_PID]) continue;

      // Look at point farthest away from vertex
      TLorentzVector thisp;
      TVector3 thisv;
      ND::TGlobalReconModule::TGlobalPID *track =
          (ND::TGlobalReconModule::TGlobalPID *)(*PIDs)[this_PID];
      if (fabs(track->FrontPosition.Z() - RecVertex.Z()) <
          fabs(track->BackPosition.Z() - RecVertex.Z())) {
        thisp = track->BackPosition;
        thisv = track->BackDirection;
      } else {
        thisp = track->FrontPosition;
        thisv = track->FrontDirection;
      }

      // Is it going the wrong way?
      // Change in PIDs_modified container
      if (thisv.Z() * (RecVertex.Z() - thisp.Z()) > 0) {
        charge_mod[this_PID] *= -1;
      }
    }  // End loop over associated tracks
  }    // End check if any links found

  return RecVertex;
}

//**********************************************************
int TGlobalAnalysisUtils::GetBunch(
    const ND::TGlobalReconModule::TGlobalPID *globalTrack, bool isMC) {
  //**********************************************************

  double shift = 0;
  int det = globalTrack->Detectors;

  // det: 1,2,3 == tpc1, tpc2, tpc3; 4,5 == fgd1, fgd2; 6 == DSECal, 7==TECal or
  // PECal;8 == P0D;9 == SMRD
  if (isMC) {
    if (det == 8) shift = 2899;
    if (det == 6 || det == 7) shift = 2778;
    if (det == 9) shift = 2793;
  }

  if (!isMC) {
    shift = 0;
    if (det == 4 || det == 5) shift = -582;
  }

  double tTrack = 0;

  // Forward going track
  TLorentzVector frontPos = globalTrack->FrontPosition;
  TLorentzVector backPos = globalTrack->BackPosition;

  double frontzi = frontPos.Z();
  double backzi = backPos.Z();
  if ((globalTrack->isForward && (frontzi < backzi)) ||
      (!globalTrack->isForward && (frontzi > backzi))) {
    tTrack = frontPos.T();
  }
  // Backward going track
  else {
    tTrack = backPos.T();
  }

  int bunch = 0;
  if (!isMC) {
    double time[6] = {3361.3, 3946.3, 4529.2, 5109.4, 5693.4, 6274.9};  // for
                                                                        // fgd
    double sigma = 20.0;
    for (int i = 0; i < 6; i++) {
      if (fabs(tTrack - shift - time[i]) < sigma * 4) {
        bunch = i + 1;
        break;
      }
    }
  } else {
    double time0 = 93.0;
    double sigma = 20.0;
    double time[6] = {0, 0, 0, 0, 0, 0};
    for (int i = 0; i < 6; i++) {
      time[i] = time0 + 582 * i;
      if (fabs(tTrack - shift - time[i]) < sigma * 4) {
        bunch = i + 1;
        break;
      }
    }
  }

  return bunch;
}
