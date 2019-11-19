#include "TGRooTrackerUtils.hxx"

#include <string>

#include "TGRooTrackerDefs.h"
#include <TLorentzVector.h>
#include <TMath.h>
#include <TObjString.h>

using std::cout;
using std::endl;

// using namespace GENIEDEFS;
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsCC(const ND::GRooTrackerVtx &evt) {
  // Is it a CC event?
  //
  string code = evt.EvtCode->GetString().Data();
  return (code.find("Weak[CC]") != string::npos);
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsNC(const ND::GRooTrackerVtx &evt) {
  // Is it a NC event?
  //
  string code = evt.EvtCode->GetString().Data();
  return (code.find("Weak[NC]") != string::npos);
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsQELCC(const ND::GRooTrackerVtx &evt) {
  // Is it a QEL CC event?
  // Note: Excludes QEL charm production -
  //
  string code = evt.EvtCode->GetString().Data();
  return (!TGRooTrackerUtils::IsCharmProd(evt) &&
          code.find("Weak[CC],QES") != string::npos);
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsELNC(const ND::GRooTrackerVtx &evt) {
  // Is it an Elastic NC event?
  //
  string code = evt.EvtCode->GetString().Data();
  return (code.find("Weak[NC],QES") != string::npos);
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsRES(const ND::GRooTrackerVtx &evt) {
  // Is it a Resonance (CC+NC) event?
  //
  string code = evt.EvtCode->GetString().Data();
  return (code.find("RES") != string::npos);
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsRESCC(const ND::GRooTrackerVtx &evt) {
  // Is it a Resonance CC event?
  //
  string code = evt.EvtCode->GetString().Data();
  return (code.find("Weak[CC],RES") != string::npos);
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsRESNC(const ND::GRooTrackerVtx &evt) {
  // Is it a Resonance NC event?
  //
  string code = evt.EvtCode->GetString().Data();
  return (code.find("Weak[NC],RES") != string::npos);
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsDIS(const ND::GRooTrackerVtx &evt) {
  // Is it a DIS (CC+NC) event?
  // Note: Excludes DIS charm production -
  //
  string code = evt.EvtCode->GetString().Data();
  return (!TGRooTrackerUtils::IsCharmProd(evt) &&
          code.find("DIS") != string::npos);
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsDISCC(const ND::GRooTrackerVtx &evt) {
  // Is it a DIS CC event?
  // Note: Excludes DIS charm production -
  //
  string code = evt.EvtCode->GetString().Data();
  return (!TGRooTrackerUtils::IsCharmProd(evt) &&
          code.find("Weak[CC],DIS") != string::npos);
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsDISNC(const ND::GRooTrackerVtx &evt) {
  // Is it a DIS NC event?
  // Note: Excludes DIS charm production -
  //
  string code = evt.EvtCode->GetString().Data();
  return (!TGRooTrackerUtils::IsCharmProd(evt) &&
          code.find("Weak[NC],DIS") != string::npos);
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsCOH(const ND::GRooTrackerVtx &evt) {
  // Is it a COH (CC+NC, elastic + pi prod + ...)  event?
  //
  string code = evt.EvtCode->GetString().Data();
  return (code.find("COH") != string::npos);
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsCOHPi(const ND::GRooTrackerVtx &evt) {
  // Is it a COH pion production event (CC+NC)?
  //
  bool is_coh = TGRooTrackerUtils::IsCOH(evt);
  bool has_1pi = true;

  return (is_coh && has_1pi);
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsCOHPiCC(const ND::GRooTrackerVtx &evt) {
  // Is it a COH CC pion production event?
  //
  bool is_cohpi = TGRooTrackerUtils::IsCOHPi(evt);
  bool is_cc = TGRooTrackerUtils::IsCC(evt);

  return (is_cohpi && is_cc);
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsCOHPiNC(const ND::GRooTrackerVtx &evt) {
  // Is it a COH NC pion production event?
  //
  bool is_cohpi = TGRooTrackerUtils::IsCOHPi(evt);
  bool is_nc = TGRooTrackerUtils::IsNC(evt);

  return (is_cohpi && is_nc);
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsAMNuGamma(const ND::GRooTrackerVtx &evt) {
  // Is it an anomaly-mediated nu-gamma interaction?
  //
  string code = evt.EvtCode->GetString().Data();
  return (code.find("AMNuGamma") != string::npos);
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsNuEEL(const ND::GRooTrackerVtx &evt) {
  // Is it a (anti)neutrino - electron elastic event?
  //
  string code = evt.EvtCode->GetString().Data();
  return (code.find("NuEEL") != string::npos);
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsIMD(const ND::GRooTrackerVtx &evt) {
  // Is it an inverse muon decay event?
  //
  string code = evt.EvtCode->GetString().Data();
  return (code.find("IMD") != string::npos);
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsCharmProd(const ND::GRooTrackerVtx &evt) {
  // Is it a charm production event?
  //
  string code = evt.EvtCode->GetString().Data();
  return (code.find("charm") != string::npos);
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsQELCharm(const ND::GRooTrackerVtx &evt) {
  // Is it a QEL charm production event?
  //
  string code = evt.EvtCode->GetString().Data();
  return (code.find("QES;charm") != string::npos);
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsDISCharm(const ND::GRooTrackerVtx &evt) {
  // Is it a DIS charm production event?
  //
  string code = evt.EvtCode->GetString().Data();
  return (code.find("DIS;charm") != string::npos);
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::NucIsHit(const ND::GRooTrackerVtx &evt) {
  string code = evt.EvtCode->GetString().Data();
  return (code.find("N:2112") != string::npos ||
          code.find("N:2212") != string::npos);
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::QuarkIsHit(const ND::GRooTrackerVtx &evt) {
  string code = evt.EvtCode->GetString().Data();
  return (code.find("N:2112;q:") != string::npos ||
          code.find("N:2212;q:") != string::npos);
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::SeaQuarkIsHit(const ND::GRooTrackerVtx &evt) {
  string code = evt.EvtCode->GetString().Data();
  return (code.find("(s)") != string::npos);
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::ValQuarkIsHit(const ND::GRooTrackerVtx &evt) {
  string code = evt.EvtCode->GetString().Data();
  return (code.find("(v)") != string::npos);
}
//_______________________________________________________________________________________
double TGRooTrackerUtils::KineXbj(const ND::GRooTrackerVtx &evt) {
  // Bjorken scaling var, x
  //
  if (TGRooTrackerUtils::IdxHitNuc(evt) < 0)
    return 0;

  double Q2 = TGRooTrackerUtils::KineQ2(evt);
  double v = TGRooTrackerUtils::KineNu(evt);

  if (v <= 0)
    return 0;

  double M = kPcNucleonMass;
  double xbj = 0.5 * Q2 / (M * v);

  return xbj;
}
//_______________________________________________________________________________________
double TGRooTrackerUtils::KineNu(const ND::GRooTrackerVtx &evt) {
  // Energy transfer, v (GeV)
  //
  int iv = TGRooTrackerUtils::IdxNu(evt);
  int il = TGRooTrackerUtils::IdxFsl(evt);

  if (iv == -1 || il == -1)
    return 0;

  // get the neutrino, final state prim lepton & hit nucleon 4-momenta
  TLorentzVector p4v(
      evt.StdHepP4[iv][kGStdHepIdxPx], evt.StdHepP4[iv][kGStdHepIdxPy],
      evt.StdHepP4[iv][kGStdHepIdxPz], evt.StdHepP4[iv][kGStdHepIdxE]);
  TLorentzVector p4l(
      evt.StdHepP4[il][kGStdHepIdxPx], evt.StdHepP4[il][kGStdHepIdxPy],
      evt.StdHepP4[il][kGStdHepIdxPz], evt.StdHepP4[il][kGStdHepIdxE]);

  TLorentzVector q = p4v - p4l;

  return q.Energy();
}
//_______________________________________________________________________________________
double TGRooTrackerUtils::KineY(const ND::GRooTrackerVtx &evt) {
  // Inelasticity, y
  //
  if (TGRooTrackerUtils::IdxHitNuc(evt) < 0)
    return 0;

  int iv = TGRooTrackerUtils::IdxNu(evt);

  double Ev = evt.StdHepP4[iv][kGStdHepIdxE];
  if (Ev <= 0)
    return 0;

  double v = TGRooTrackerUtils::KineNu(evt);
  double y = v / Ev;
  return y;
}
//_______________________________________________________________________________________
double TGRooTrackerUtils::KineTau(const ND::GRooTrackerVtx & /*evt*/) {
  // t, for COH events only
  //
  return 0;
}
//_______________________________________________________________________________________
double TGRooTrackerUtils::KineW(const ND::GRooTrackerVtx &evt) {
  // Hadronic invariant mass, W (GeV)
  //
  int iv = TGRooTrackerUtils::IdxNu(evt);
  int il = TGRooTrackerUtils::IdxFsl(evt);
  int in = TGRooTrackerUtils::IdxHitNuc(evt);

  if (iv == -1 || il == -1 || in == -1)
    return 0;

  TLorentzVector p4v(
      evt.StdHepP4[iv][kGStdHepIdxPx], evt.StdHepP4[iv][kGStdHepIdxPy],
      evt.StdHepP4[iv][kGStdHepIdxPz], evt.StdHepP4[iv][kGStdHepIdxE]);
  TLorentzVector p4l(
      evt.StdHepP4[il][kGStdHepIdxPx], evt.StdHepP4[il][kGStdHepIdxPy],
      evt.StdHepP4[il][kGStdHepIdxPz], evt.StdHepP4[il][kGStdHepIdxE]);
  TLorentzVector p4n(
      evt.StdHepP4[in][kGStdHepIdxPx], evt.StdHepP4[in][kGStdHepIdxPy],
      evt.StdHepP4[in][kGStdHepIdxPz], evt.StdHepP4[in][kGStdHepIdxE]);

  TLorentzVector q = p4v - p4l;
  TLorentzVector w = p4n + q;
  ;

  return w.Mag();
}
//_______________________________________________________________________________________
double TGRooTrackerUtils::KineQ2(const ND::GRooTrackerVtx &evt) {
  // Momentum transfer, Q2
  //
  int iv = TGRooTrackerUtils::IdxNu(evt);
  int il = TGRooTrackerUtils::IdxFsl(evt);

  if (iv == -1 || il == -1)
    return 0;

  // get the neutrino, final state prim lepton & hit nucleon 4-momenta
  TLorentzVector p4v(
      evt.StdHepP4[iv][kGStdHepIdxPx], evt.StdHepP4[iv][kGStdHepIdxPy],
      evt.StdHepP4[iv][kGStdHepIdxPz], evt.StdHepP4[iv][kGStdHepIdxE]);
  TLorentzVector p4l(
      evt.StdHepP4[il][kGStdHepIdxPx], evt.StdHepP4[il][kGStdHepIdxPy],
      evt.StdHepP4[il][kGStdHepIdxPz], evt.StdHepP4[il][kGStdHepIdxE]);

  TLorentzVector q = p4v - p4l;

  return (-1. * q.M2());
}
//_______________________________________________________________________________________
int TGRooTrackerUtils::ResId(const ND::GRooTrackerVtx &evt) {
  string code = evt.EvtCode->GetString().Data();

  // do not change order of below if statements as
  // they only work in current order, otherwise a 'code.find("RES;res:1")'
  // will return true when it should be 'code.find("RES;res:1?")'

  if (code.find("RES;res:0") != string::npos)
    return 0; // kP33_1232
  if (code.find("RES;res:10") != string::npos)
    return 10; // kP13_1720
  if (code.find("RES;res:11") != string::npos)
    return 11; // kF15_1680
  if (code.find("RES;res:12") != string::npos)
    return 12; // kP31_1910
  if (code.find("RES;res:13") != string::npos)
    return 13; // kP33_1920
  if (code.find("RES;res:14") != string::npos)
    return 14; // kF35_1905
  if (code.find("RES;res:15") != string::npos)
    return 15; // kF37_1950
  if (code.find("RES;res:16") != string::npos)
    return 16; // kP11_1710
  if (code.find("RES;res:17") != string::npos)
    return 17; // kF17_1970
  if (code.find("RES;res:1") != string::npos)
    return 1; // kS11_1535
  if (code.find("RES;res:2") != string::npos)
    return 2; // kD13_1520
  if (code.find("RES;res:3") != string::npos)
    return 3; // kS11_1650
  if (code.find("RES;res:4") != string::npos)
    return 4; // kD13_1700
  if (code.find("RES;res:5") != string::npos)
    return 5; // kD15_1675
  if (code.find("RES;res:6") != string::npos)
    return 6; // kS31_1620
  if (code.find("RES;res:7") != string::npos)
    return 7; // kD33_1700
  if (code.find("RES;res:8") != string::npos)
    return 8; // kP11_1440
  if (code.find("RES;res:9") != string::npos)
    return 9; // kP33_1600

  return -1;
}
//_______________________________________________________________________________________
int TGRooTrackerUtils::PdgNu(const ND::GRooTrackerVtx &evt) {
  // Return the neutrino PDG code
  //
  int iv = TGRooTrackerUtils::IdxNu(evt);
  if (iv < 0)
    return 0;

  return evt.StdHepPdg[iv];
}
//_______________________________________________________________________________________
int TGRooTrackerUtils::PdgFsl(const ND::GRooTrackerVtx &evt) {
  // Return the final state primarly lepton PDG code
  //
  int il = TGRooTrackerUtils::IdxFsl(evt);
  if (il < 0)
    return 0;

  return evt.StdHepPdg[il];
}
//_______________________________________________________________________________________
int TGRooTrackerUtils::PdgTgt(const ND::GRooTrackerVtx &evt) {
  // Return the hit target pdg code
  //
  int it = TGRooTrackerUtils::IdxTgt(evt);
  if (it < 0)
    return 0;

  return evt.StdHepPdg[it];
}
//_______________________________________________________________________________________
int TGRooTrackerUtils::PdgHitNuc(const ND::GRooTrackerVtx &evt) {
  // Return the hit nucleon pdg code (if a nucleon was hit)
  //
  int idx = TGRooTrackerUtils::IdxHitNuc(evt);
  if (idx < 0)
    return 0;

  return evt.StdHepPdg[idx];
}
//_______________________________________________________________________________________
int TGRooTrackerUtils::TgtZ(const ND::GRooTrackerVtx &evt) {
  // Return the hit target Z
  //
  int tgtpdg = TGRooTrackerUtils::PdgTgt(evt);

  if (tgtpdg == kPdgProton)
    return 1;
  if (tgtpdg == kPdgNeutron)
    return 0;

  // nuclear pdg codes of the form: 10LZZZAAAI
  // extract ZZZ
  if (tgtpdg > 1000000000) {
    int Z = (tgtpdg / 10000) -
            1000 * (tgtpdg / 10000000); // don't factor out - bit shifts!
    return Z;
  }

  return 0;
}
//_______________________________________________________________________________________
int TGRooTrackerUtils::TgtA(const ND::GRooTrackerVtx &evt) {
  // Return the hit target A
  //
  int tgtpdg = TGRooTrackerUtils::PdgTgt(evt);

  if (tgtpdg == kPdgProton)
    return 1;
  if (tgtpdg == kPdgNeutron)
    return 1;

  // nuclear pdg codes of the form: 10LZZZAAAI
  // extract AAA
  if (tgtpdg > 1000000000) {
    int A = (tgtpdg / 10) -
            1000 * (tgtpdg / 10000); // don't factor out - bit shifts!
    return A;
  }

  return 0;
}
//_______________________________________________________________________________________
int TGRooTrackerUtils::NParticles(const ND::GRooTrackerVtx &evt) {
  int N = 0;
  while (evt.StdHepPdg[N] != 0) {
    N++;
  }
  return N;
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsPrimProton(const ND::GRooTrackerVtx &evt, int i) {
  //  Is entry i a primary state proton?
  //
  int Np = TGRooTrackerUtils::NParticles(evt);
  if (i >= Np || i < 0) {
    return false;
  }

  if (evt.StdHepStatus[i] == kIStHadronInTheNucleus &&
      evt.StdHepPdg[i] == kPdgProton) {
    return true;
  }

  return false;
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsPrimNeutron(const ND::GRooTrackerVtx &evt, int i) {
  // Is entry i a primary state neutron?
  //
  int Np = TGRooTrackerUtils::NParticles(evt);
  if (i >= Np || i < 0) {
    return false;
  }

  if (evt.StdHepStatus[i] == kIStHadronInTheNucleus &&
      evt.StdHepPdg[i] == kPdgNeutron) {
    return true;
  }

  return false;
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsPrimPi0(const ND::GRooTrackerVtx &evt, int i) {
  // Is entry i a primary state pi0?
  //
  bool is_coh = TGRooTrackerUtils::IsCOH(evt);
  bool is_pi0_fs = TGRooTrackerUtils::IsFinStPi0(evt, i);

  int Np = TGRooTrackerUtils::NParticles(evt);
  if (i >= Np || i < 0) {
    return false;
  }

  // if is coherent event then set primary to final state
  if (is_coh && is_pi0_fs) {
    return true;
  }

  // else check is is a hadron created within the nucleus
  if (evt.StdHepStatus[i] == kIStHadronInTheNucleus &&
      evt.StdHepPdg[i] == kPdgPi0) {
    return true;
  }

  return false;
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsPrimPip(const ND::GRooTrackerVtx &evt, int i) {
  // Is entry i a primary state pi+?
  //
  bool is_coh = TGRooTrackerUtils::IsCOH(evt);
  bool is_pip_fs = TGRooTrackerUtils::IsFinStPip(evt, i);

  int Np = TGRooTrackerUtils::NParticles(evt);
  if (i >= Np || i < 0) {
    return false;
  }

  // if is coherent event then set primary to final state
  if (is_coh && is_pip_fs)
    return true;

  // else check is is a hadron created within the nucleus
  if (evt.StdHepStatus[i] == kIStHadronInTheNucleus &&
      evt.StdHepPdg[i] == kPdgPiP) {
    return true;
  }

  return false;
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsPrimPim(const ND::GRooTrackerVtx &evt, int i) {
  // Is entry i a primary state pi-
  //

  bool is_coh = TGRooTrackerUtils::IsCOH(evt);
  bool is_pim_fs = TGRooTrackerUtils::IsFinStPim(evt, i);

  int Np = TGRooTrackerUtils::NParticles(evt);
  if (i >= Np || i < 0) {
    return false;
  }

  // if is coherent event then set primary to final state
  if (is_coh && is_pim_fs) {
    return true;
  }

  // else check is is a hadron created within the nucleus
  if (evt.StdHepStatus[i] == kIStHadronInTheNucleus &&
      evt.StdHepPdg[i] == kPdgPiM) {
    return true;
  }

  return false;
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsPrimK0(const ND::GRooTrackerVtx &evt, int i) {
  // Is entry i a primary state K0+K0bar
  //
  int Np = TGRooTrackerUtils::NParticles(evt);
  if (i >= Np || i < 0) {
    return false;
  }

  if (evt.StdHepStatus[i] == kIStHadronInTheNucleus &&
      (evt.StdHepPdg[i] == kPdgK0 || evt.StdHepPdg[i] == kPdgAntiK0)) {
    return true;
  }

  return false;
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsPrimKp(const ND::GRooTrackerVtx &evt, int i) {
  // Is entry i a primary state K+
  //
  int Np = TGRooTrackerUtils::NParticles(evt);
  if (i >= Np || i < 0) {
    return false;
  }

  if (evt.StdHepStatus[i] == kIStHadronInTheNucleus &&
      evt.StdHepPdg[i] == kPdgKP) {
    return true;
  }

  return false;
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsPrimKm(const ND::GRooTrackerVtx &evt, int i) {
  // Is entry i a primary state K-
  //
  int Np = TGRooTrackerUtils::NParticles(evt);
  if (i >= Np || i < 0) {
    return false;
  }

  if (evt.StdHepStatus[i] == kIStHadronInTheNucleus &&
      evt.StdHepPdg[i] == kPdgKM) {
    return true;
  }

  return false;
}
//_______________________________________________________________________________________
int TGRooTrackerUtils::NPrimProton(const ND::GRooTrackerVtx &evt) {
  // Number of protons in primary hadronic system (before intranuclear hadron
  // transport)
  //
  if (TGRooTrackerUtils::TgtA(evt) > 1) {
    // is a nuclear target: primary hadronic system != final hadronic system
    int N = 0;
    int Np = TGRooTrackerUtils::NParticles(evt);
    for (int i = 0; i < Np; i++) {
      if (TGRooTrackerUtils::IsPrimProton(evt, i)) {
        N++;
      }
    }
    return N;
  } else {
    return TGRooTrackerUtils::NFinStProton(evt);
  }
}
//_______________________________________________________________________________________
int TGRooTrackerUtils::NPrimNeutron(const ND::GRooTrackerVtx &evt) {
  // Number of neutrons in primary hadronic system (before intranuclear hadron
  // transport)
  //
  if (TGRooTrackerUtils::TgtA(evt) > 1) {
    // is a nuclear target: primary hadronic system != final hadronic system
    int N = 0;
    int Np = TGRooTrackerUtils::NParticles(evt);
    for (int i = 0; i < Np; i++) {
      if (TGRooTrackerUtils::IsPrimNeutron(evt, i)) {
        N++;
      }
    }
    return N;
  }
  return TGRooTrackerUtils::NFinStNeutron(evt);
}
//_______________________________________________________________________________________
int TGRooTrackerUtils::NPrimPi0(const ND::GRooTrackerVtx &evt) {
  // Number of pi0 in primary hadronic system (before intranuclear hadron
  // transport)
  //
  if (TGRooTrackerUtils::TgtA(evt) > 1) {
    // is a nuclear target: primary hadronic system != final hadronic system
    int N = 0;
    int Np = TGRooTrackerUtils::NParticles(evt);
    for (int i = 0; i < Np; i++) {
      if (TGRooTrackerUtils::IsPrimPi0(evt, i)) {
        N++;
      }
    }
    return N;
  }
  return TGRooTrackerUtils::NFinStPi0(evt);
}
//_______________________________________________________________________________________
int TGRooTrackerUtils::NPrimPip(const ND::GRooTrackerVtx &evt) {
  // Number of pi+ in primary hadronic system (before intranuclear hadron
  // transport)
  //
  if (TGRooTrackerUtils::TgtA(evt) > 1) {
    // is a nuclear target: primary hadronic system != final hadronic system
    int N = 0;
    int Np = TGRooTrackerUtils::NParticles(evt);
    for (int i = 0; i < Np; i++) {
      if (TGRooTrackerUtils::IsPrimPip(evt, i)) {
        N++;
      }
    }
    return N;
  }
  return TGRooTrackerUtils::NFinStPip(evt);
}
//_______________________________________________________________________________________
int TGRooTrackerUtils::NPrimPim(const ND::GRooTrackerVtx &evt) {
  // Number of pi- in primary hadronic system (before intranuclear hadron
  // transport)
  //
  if (TGRooTrackerUtils::TgtA(evt) > 1) {
    // is a nuclear target: primary hadronic system != final hadronic system
    int N = 0;
    int Np = TGRooTrackerUtils::NParticles(evt);
    for (int i = 0; i < Np; i++) {
      if (TGRooTrackerUtils::IsPrimPim(evt, i)) {
        N++;
      }
    }
    return N;
  }
  return TGRooTrackerUtils::NFinStPim(evt);
}
//_______________________________________________________________________________________
int TGRooTrackerUtils::NPrimK0(const ND::GRooTrackerVtx &evt) {
  // Number of K0+KObar in primary hadronic system (before intranuclear hadron
  // transport)
  //
  if (TGRooTrackerUtils::TgtA(evt) > 1) {
    // is a nuclear target: primary hadronic system != final hadronic system
    int N = 0;
    int Np = TGRooTrackerUtils::NParticles(evt);
    for (int i = 0; i < Np; i++) {
      if (TGRooTrackerUtils::IsPrimK0(evt, i)) {
        N++;
      }
    }
    return N;
  }
  return TGRooTrackerUtils::NFinStK0(evt);
}
//_______________________________________________________________________________________
int TGRooTrackerUtils::NPrimKp(const ND::GRooTrackerVtx &evt) {
  // Number of K+ in primary hadronic system (before intranuclear hadron
  // transport)
  //
  if (TGRooTrackerUtils::TgtA(evt) > 1) {
    // is a nuclear target: primary hadronic system != final hadronic system
    int N = 0;
    int Np = TGRooTrackerUtils::NParticles(evt);
    for (int i = 0; i < Np; i++) {
      if (TGRooTrackerUtils::IsPrimKp(evt, i)) {
        N++;
      }
    }
    return N;
  }
  return TGRooTrackerUtils::NFinStKp(evt);
}
//_______________________________________________________________________________________
int TGRooTrackerUtils::NPrimKm(const ND::GRooTrackerVtx &evt) {
  // Number of K- in primary hadronic system (before intranuclear hadron
  // transport)
  //
  if (TGRooTrackerUtils::TgtA(evt) > 1) {
    int N = 0;
    int Np = TGRooTrackerUtils::NParticles(evt);
    for (int i = 0; i < Np; i++) {
      if (TGRooTrackerUtils::IsPrimKm(evt, i)) {
        N++;
      }
    }
    return N;
  }
  return TGRooTrackerUtils::NFinStKm(evt);
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsFinStProton(const ND::GRooTrackerVtx &evt, int i) {
  //  Is entry i a final state proton
  //
  int Np = TGRooTrackerUtils::NParticles(evt);
  if (i >= Np || i < 0) {
    return false;
  }

  if (evt.StdHepStatus[i] == kIStStableFinalState &&
      evt.StdHepPdg[i] == kPdgProton) {
    return true;
  }

  return false;
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsFinStNeutron(const ND::GRooTrackerVtx &evt, int i) {
  // Is entry i a final state neutron?
  //
  int Np = TGRooTrackerUtils::NParticles(evt);
  if (i >= Np || i < 0) {
    return false;
  }

  if (evt.StdHepStatus[i] == kIStStableFinalState &&
      evt.StdHepPdg[i] == kPdgNeutron) {
    return true;
  }

  return false;
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsFinStPi0(const ND::GRooTrackerVtx &evt, int i) {
  // Is entry i a final state pi0?
  // (note: pi0's are decayed - so count pi0's marked as decayed with gamma's in
  // the
  // daughter list so as not to count absorbed/etc pi0's)

  int Np = TGRooTrackerUtils::NParticles(evt);
  if (i >= Np || i < 0) {
    return false;
  }
  // count pi0's that are in final state
  else if (evt.StdHepStatus[i] == kIStStableFinalState &&
           evt.StdHepPdg[i] == kPdgPi0) {
    return true;
  }
  // count pi0's that have been decayed
  else if (evt.StdHepStatus[i] == kIStDecayedState &&
           evt.StdHepPdg[i] == kPdgPi0) {
    if (evt.StdHepFd[i] > 0 &&
        (evt.StdHepPdg[evt.StdHepFd[i]] == kPdgGamma ||
         evt.StdHepPdg[evt.StdHepFd[i]] == kPdgElectron ||
         evt.StdHepPdg[evt.StdHepFd[i]] == kPdgPositron) &&
        (evt.StdHepPdg[evt.StdHepLd[i]] == kPdgGamma ||
         evt.StdHepPdg[evt.StdHepLd[i]] == kPdgElectron ||
         evt.StdHepPdg[evt.StdHepLd[i]] == kPdgPositron)) {
      return true;
    }
  }
  return false;
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsFinStPip(const ND::GRooTrackerVtx &evt, int i) {
  // Is entry i a final state pi+ ?
  //
  int Np = TGRooTrackerUtils::NParticles(evt);
  if (i >= Np || i < 0) {
    return false;
  }

  if (evt.StdHepStatus[i] == kIStStableFinalState &&
      evt.StdHepPdg[i] == kPdgPiP) {
    return true;
  }

  return false;
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsFinStPim(const ND::GRooTrackerVtx &evt, int i) {
  // Is entry i a final state pi- ?
  //
  int Np = TGRooTrackerUtils::NParticles(evt);
  if (i >= Np || i < 0) {
    return false;
  }

  if (evt.StdHepStatus[i] == kIStStableFinalState &&
      evt.StdHepPdg[i] == kPdgPiM) {
    return true;
  }

  return false;
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsFinStK0(const ND::GRooTrackerVtx &evt, int i) {
  // Is entry i a final state K0+K0bar?
  //
  int Np = TGRooTrackerUtils::NParticles(evt);
  if (i >= Np || i < 0) {
    return false;
  }

  if (evt.StdHepStatus[i] == kIStStableFinalState &&
      (evt.StdHepPdg[i] == kPdgK0 || evt.StdHepPdg[i] == kPdgAntiK0)) {
    return true;
  }

  return false;
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsFinStKp(const ND::GRooTrackerVtx &evt, int i) {
  // Is entry i a final state K+?
  //
  int Np = TGRooTrackerUtils::NParticles(evt);
  if (i >= Np || i < 0) {
    return false;
  }

  if (evt.StdHepStatus[i] == kIStStableFinalState &&
      evt.StdHepPdg[i] == kPdgKP) {
    return true;
  }

  return false;
}
//_______________________________________________________________________________________
bool TGRooTrackerUtils::IsFinStKm(const ND::GRooTrackerVtx &evt, int i) {
  // Is entry i a final state K-?
  //
  int Np = TGRooTrackerUtils::NParticles(evt);
  if (i >= Np || i < 0) {
    return false;
  }

  if (evt.StdHepStatus[i] == kIStStableFinalState &&
      evt.StdHepPdg[i] == kPdgKM) {
    return true;
  }

  return false;
}
//_______________________________________________________________________________________
int TGRooTrackerUtils::NFinStProton(const ND::GRooTrackerVtx &evt) {
  // Number of protons in final state hadronic system
  //
  int N = 0;
  int Np = TGRooTrackerUtils::NParticles(evt);
  for (int i = 0; i < Np; i++) {
    if (TGRooTrackerUtils::IsFinStProton(evt, i)) {
      N++;
    }
  }
  return N;
}
//_______________________________________________________________________________________
int TGRooTrackerUtils::NFinStNeutron(const ND::GRooTrackerVtx &evt) {
  // Number of neutrons in final state hadronic system
  //
  //  return 0;
  int N = 0;
  int Np = TGRooTrackerUtils::NParticles(evt);
  for (int i = 0; i < Np; i++) {
    if (TGRooTrackerUtils::IsFinStNeutron(evt, i)) {
      N++;
    }
  }
  return N;
}
//_______________________________________________________________________________________
int TGRooTrackerUtils::NFinStPi0(const ND::GRooTrackerVtx &evt) {
  // Number of pi0 in final state hadronic system
  // (note: pi0's are decayed - so count pi0's marked as decayed with gamma's in
  // the
  // daughter list so as not to count absorbed/etc pi0's)

  int N = 0;
  int Np = TGRooTrackerUtils::NParticles(evt);
  for (int i = 0; i < Np; i++) {
    if (TGRooTrackerUtils::IsFinStPi0(evt, i)) {
      N++;
    }
  }
  return N;
}
//_______________________________________________________________________________________
int TGRooTrackerUtils::NFinStPip(const ND::GRooTrackerVtx &evt) {
  // Number of pi+ in final state hadronic system
  //
  int N = 0;
  int Np = TGRooTrackerUtils::NParticles(evt);
  for (int i = 0; i < Np; i++) {
    if (TGRooTrackerUtils::IsFinStPip(evt, i)) {
      N++;
    }
  }
  return N;
}
//_______________________________________________________________________________________
int TGRooTrackerUtils::NFinStPim(const ND::GRooTrackerVtx &evt) {
  // Number of pi- in final state hadronic system
  //
  int N = 0;
  int Np = TGRooTrackerUtils::NParticles(evt);
  for (int i = 0; i < Np; i++) {
    if (TGRooTrackerUtils::IsFinStPim(evt, i)) {
      N++;
    }
  }
  return N;
}
//_______________________________________________________________________________________
int TGRooTrackerUtils::NFinStK0(const ND::GRooTrackerVtx &evt) {
  // Number of K0+K0bar in final state hadronic system
  //
  int N = 0;
  int Np = TGRooTrackerUtils::NParticles(evt);
  for (int i = 0; i < Np; i++) {
    if (evt.StdHepStatus[i] != kIStStableFinalState)
      continue;
    if (evt.StdHepPdg[i] == kPdgK0 || evt.StdHepPdg[i] == kPdgAntiK0)
      N++;
  }
  return N;
}
//_______________________________________________________________________________________
int TGRooTrackerUtils::NFinStKp(const ND::GRooTrackerVtx &evt) {
  // Number of K+ in final state hadronic system
  //
  int N = 0;
  int Np = TGRooTrackerUtils::NParticles(evt);
  for (int i = 0; i < Np; i++) {
    if (TGRooTrackerUtils::IsFinStKp(evt, i)) {
      N++;
    }
  }
  return N;
}
//_______________________________________________________________________________________
int TGRooTrackerUtils::NFinStKm(const ND::GRooTrackerVtx &evt) {
  // Number of K- in final state hadronic system
  //
  int N = 0;
  int Np = TGRooTrackerUtils::NParticles(evt);
  for (int i = 0; i < Np; i++) {
    if (TGRooTrackerUtils::IsFinStKm(evt, i)) {
      N++;
    }
  }
  return N;
}
//_______________________________________________________________________________________
double TGRooTrackerUtils::ENu(const ND::GRooTrackerVtx &evt) {
  // Return the neutrino energy
  //
  int iv = TGRooTrackerUtils::IdxNu(evt);
  if (iv < 0)
    return 0;

  return evt.StdHepP4[iv][kGStdHepIdxE];
}
//_______________________________________________________________________________________
double TGRooTrackerUtils::ENu_nrf(const ND::GRooTrackerVtx &evt) {
  // Return the neutrino energy at the hit nucleon rest frame
  //
  int iN = TGRooTrackerUtils::IdxHitNuc(evt);
  if (iN < 0)
    return TGRooTrackerUtils::ENu(evt); // no hit nuc - same as Ev at LAB

  int iv = TGRooTrackerUtils::IdxNu(evt);

  double bx = evt.StdHepP4[iN][kGStdHepIdxPx] / evt.StdHepP4[iN][kGStdHepIdxE];
  double by = evt.StdHepP4[iN][kGStdHepIdxPy] / evt.StdHepP4[iN][kGStdHepIdxE];
  double bz = evt.StdHepP4[iN][kGStdHepIdxPz] / evt.StdHepP4[iN][kGStdHepIdxE];

  TLorentzVector p4v(
      evt.StdHepP4[iv][kGStdHepIdxPx], evt.StdHepP4[iv][kGStdHepIdxPy],
      evt.StdHepP4[iv][kGStdHepIdxPz], evt.StdHepP4[iv][kGStdHepIdxE]);

  p4v.Boost(-bx, -by, -bz);

  return p4v.Energy();
}
//_______________________________________________________________________________________
double TGRooTrackerUtils::ENu_reco(const ND::GRooTrackerVtx &evt) {
  // Return the reconstructed-like neutrino energy for QELCC
  // Ev_reco = (M*El - ml^2/2) / (M-El+pl*costheta)
  //
  int iv = TGRooTrackerUtils::IdxNu(evt);
  if (iv < 0)
    return 0;
  int il = TGRooTrackerUtils::IdxFsl(evt);
  if (il < 0)
    return 0;

  double El = TGRooTrackerUtils::E(evt, il);
  double pl = TGRooTrackerUtils::Momentum(evt, il);
  double costhl = TGRooTrackerUtils::CosTheta(evt, il);
  double M = kPcNucleonMass;
  double ml2 = TMath::Power(kPcMuonMass, 2.);

  double Ev_reco = (M * El - 0.5 * ml2) / (M - El + pl * costhl);
  return Ev_reco;
}
//_______________________________________________________________________________________
double TGRooTrackerUtils::EFsl(const ND::GRooTrackerVtx &evt) {
  // Return the final state primary lepton energy
  //
  int il = TGRooTrackerUtils::IdxFsl(evt);
  return TGRooTrackerUtils::E(evt, il);
}
//_______________________________________________________________________________________
double TGRooTrackerUtils::MomentumFsl(const ND::GRooTrackerVtx &evt) {
  // Return the final state primary lepton momentum
  //
  int il = TGRooTrackerUtils::IdxFsl(evt);
  return TGRooTrackerUtils::Momentum(evt, il);
}
//_______________________________________________________________________________________
double TGRooTrackerUtils::Momentum(const ND::GRooTrackerVtx &evt, int i) {
  // Return the final state ith stdhep entry momentum

  int Np = TGRooTrackerUtils::NParticles(evt);
  if (i >= Np || i < 0) {
    return 0.0;
  }

  return TMath::Sqrt(TMath::Power(evt.StdHepP4[i][kGStdHepIdxPx], 2) +
                     TMath::Power(evt.StdHepP4[i][kGStdHepIdxPy], 2) +
                     TMath::Power(evt.StdHepP4[i][kGStdHepIdxPz], 2));
}
//_______________________________________________________________________________________
double TGRooTrackerUtils::E(const ND::GRooTrackerVtx &evt, int i) {
  // Return the final state ith stdhep entry momentum

  int Np = TGRooTrackerUtils::NParticles(evt);
  if (i >= Np || i < 0) {
    return 0.0;
  }

  return evt.StdHepP4[i][kGStdHepIdxE];
}
//_______________________________________________________________________________________
double TGRooTrackerUtils::KE(const ND::GRooTrackerVtx &evt, int i) {
  double E_local = TGRooTrackerUtils::E(evt, i);
  double P_local = TGRooTrackerUtils::Momentum(evt, i);
  double KE_local =
      E_local -
      TMath::Sqrt(TMath::Max(0., E_local * E_local - P_local * P_local));

  return KE_local;
}
//_______________________________________________________________________________________
double TGRooTrackerUtils::CosTheta(const ND::GRooTrackerVtx &evt, int i) {
  // Return the i^th particle cos(theta) with respect to the neutrino direction
  //
  int Np = TGRooTrackerUtils::NParticles(evt);
  if (i >= Np || i < 0) {
    return 0.0;
  }

  int iv = TGRooTrackerUtils::IdxNu(evt);
  if (iv < 0)
    return 0;

  double costh =
      (evt.StdHepP4[iv][kGStdHepIdxPx] * evt.StdHepP4[i][kGStdHepIdxPx] +
       evt.StdHepP4[iv][kGStdHepIdxPy] * evt.StdHepP4[i][kGStdHepIdxPy] +
       evt.StdHepP4[iv][kGStdHepIdxPz] * evt.StdHepP4[i][kGStdHepIdxPz]) /
      (evt.StdHepP4[iv][kGStdHepIdxE] * evt.StdHepP4[i][kGStdHepIdxE]);
  return costh;
}
//_______________________________________________________________________________________
double TGRooTrackerUtils::FermiMomentum(const ND::GRooTrackerVtx &evt) {
  // Return the hit nucleon Fermi momentum
  //
  int in = TGRooTrackerUtils::IdxHitNuc(evt);
  if (in < 0)
    return 0;

  return TMath::Sqrt(TMath::Power(evt.StdHepP4[in][kGStdHepIdxPx], 2) +
                     TMath::Power(evt.StdHepP4[in][kGStdHepIdxPy], 2) +
                     TMath::Power(evt.StdHepP4[in][kGStdHepIdxPz], 2));
}
//_______________________________________________________________________________________
int TGRooTrackerUtils::IdxNu(const ND::GRooTrackerVtx & /*evt*/) {
  // Return the input neutrino index in the STDHEP array
  // Always at 0
  //
  return 0;
}
//_______________________________________________________________________________________
int TGRooTrackerUtils::IdxTgt(const ND::GRooTrackerVtx & /*evt*/) {
  // Return the input neutrino index in the STDHEP array
  // Always at 1
  //
  return 1;
}
//_______________________________________________________________________________________
int TGRooTrackerUtils::IdxFsl(const ND::GRooTrackerVtx &evt) {
  // Return the final state prim lepton index in the STDHEP array
  // Look for the input neutrino's daughter
  //
  return evt.StdHepFd[0];
}
//_______________________________________________________________________________________
int TGRooTrackerUtils::IdxPi0(const ND::GRooTrackerVtx &evt) {
  // Return the final state prim lepton index in the STDHEP array
  // Look for the input neutrino's daughter
  //
  int id = -1;
  double E_local = -1.;

  int Np = TGRooTrackerUtils::NParticles(evt);
  for (int i = 0; i < Np; i++) {
    if (evt.StdHepPdg[i] != kPdgPi0)
      continue;
    if (evt.StdHepStatus[i] == kIStDecayedState ||
        evt.StdHepStatus[i] == kIStStableFinalState) {
      double e = evt.StdHepP4[i][kGStdHepIdxE];
      if (e > E_local) {
        E_local = e;
        id = i;
      }
    }
  }

  return id;
}
//_______________________________________________________________________________________
int TGRooTrackerUtils::IdxPi0_prim(const ND::GRooTrackerVtx &evt) {
  // Return the final state prim lepton index in the STDHEP array
  // Look for the input neutrino's daughter
  //
  int id = TGRooTrackerUtils::IdxPi0(evt);

  while (id != -1 && evt.StdHepPdg[evt.StdHepFm[id]] == kPdgPi0) {
    id = evt.StdHepFm[id];
  }

  return id;
}
//_______________________________________________________________________________________
int TGRooTrackerUtils::IdxHitNuc(const ND::GRooTrackerVtx &evt) {
  // Return the hit nucleon index in the STDHEP array
  // Look for particle at idx 1 for interactions off free nucleon targets
  // Look at the daughters ofparticle at idx 1 for interactions off nuclear
  // targets

  int pdg = evt.StdHepPdg[1];
  if (pdg == kPdgProton || pdg == kPdgNeutron)
    return 1;

  for (int i = evt.StdHepFd[1]; i <= evt.StdHepLd[1]; i++) {
    if (i <= 0)
      continue;
    if (evt.StdHepStatus[i] == kIStNucleonTarget)
      return i;
  }

  return -1;
}
//_______________________________________________________________________________________
#ifdef __T2KRW_GENIE_ENABLED__

INukeFateHA_t
TGRooTrackerUtils::DetermineHadronFate(const ND::GRooTrackerVtx &evt, int i) {
  //
  // Determine the fate of the i^th particle in the particle array during
  // intranuclear
  // hadron transport phase
  // Returns one of:
  //   genie::kIHAFtUndefined  -> undefined (no re-interaction)
  //   genie::kIHAFtCEx        -> charge exchange
  //   genie::kIHAFtElas       -> elastic
  //   genie::kIHAFtInelas     -> inelastic
  //   genie::kIHAFtAbsNP      -> absorption followed by emission of np
  //   genie::kIHAFtAbsPP      -> absorption followed by emission of pp
  //   genie::IHAFtAbsNPP      -> absorption followed by emission of npp
  //   genie::kIHAFtAbsNNP     -> absorption followed by emission of nnp
  //   genie::kIHAFtAbs2N2P    -> absorption followed by emission of 2n2p
  //   genie::kIHAFtAbs2N3P    -> absorption followed by emission of 2n3p
  //   genie::kIHAFtNPip       -> pi production : n pi+
  //   genie::kIHAFtNPipPi0    -> pi production : n pi+ pi0
  //

  INukeFateHA_t hadron_fate = kIHAFtUndefined;

  // Get global parameter list to retrieve the binding energy for given nuclear
  // target
  AlgConfigPool *fGenieConfigPool = AlgConfigPool::Instance();
  Registry *fUserPhysicsConfig = fGenieConfigPool->GlobalParameterList();

  double binding_energy = 0.0;

  // get target pdgc code
  int target_index = TGRooTrackerUtils::IdxTgt(evt);
  int target_pdgc = evt.StdHepPdg[target_index];

  // make string to pass to fUserPhysicsConfig->Get(...)
  string get_string = "RFG-NucRemovalE@Pdg=";
  char buffer[20];
  sprintf(buffer, "%i", target_pdgc);
  get_string.append(buffer);

  if (fUserPhysicsConfig->Exists(get_string)) {
    binding_energy = fUserPhysicsConfig->GetDouble(get_string);
    //      std::cout << "Target pdgc code " << target_pdgc << " has binding
    //      energy  " << binding_energy << " taken from genie
    //      UserPhysicsConfig.xml"<< std::endl;
  } else {
    Target *target_nucl = new Target(target_pdgc);
    binding_energy = BindEnergy(*target_nucl);
    //  std::cout << "Target pdgc code " << target_pdgc << " has binding energy
    //  " << binding_energy << " taken from
    //  genie::nuclear::BindEnergy(*target_nucl)"<< std::endl;
  }

  // tolerances have been set so as to catch events with zero energy change
  // and those that have had binding energy subtracted but to be narrow
  // enough that almost no other events will be included due to machine
  // floating point precision differences
  double epsilon_zero_energy_max = 2.0E-7;
  //   double epsilon_zero_energy_min = -1.0R-7;
  double epsilon_binding_energy_min = binding_energy - 0.000001; // 3.5999E-2;
  double epsilon_binding_energy_max = binding_energy + 0.000001; // 3.6001E-2;
  double epsilon_delta_P_squared = 1.0E-12;

  int ist = evt.StdHepStatus[i];
  if (ist != kIStHadronInTheNucleus) {
    return hadron_fate;
  }

  int fd = evt.StdHepFd[i];
  int ld = evt.StdHepLd[i];
  int nd = ld - fd + 1;
  if (nd == 1) {
    int j = evt.StdHepFd[i];
    if (evt.StdHepStatus[j] == 3) {
      bool is_pi0_gg = (evt.StdHepPdg[j] == kPdgPi0) &&
                       (evt.StdHepPdg[evt.StdHepFd[j]] == kPdgGamma ||
                        evt.StdHepPdg[evt.StdHepFd[j]] == kPdgElectron ||
                        evt.StdHepPdg[evt.StdHepFd[j]] == kPdgPositron) &&
                       (evt.StdHepPdg[evt.StdHepLd[j]] == kPdgGamma ||
                        evt.StdHepPdg[evt.StdHepLd[j]] == kPdgElectron ||
                        evt.StdHepPdg[evt.StdHepLd[j]] == kPdgPositron);

      if (!is_pi0_gg) {
        fd = evt.StdHepFd[j];
        ld = evt.StdHepLd[j];
        nd = ld - fd + 1;
      }
    }
  }
  //  cout << "Has " << nd << " daughters at [" << fd << ", " << ld << "]" <<
  //  endl;

  int np = 0;
  int nn = 0;
  int npi0 = 0;
  int npip = 0;
  int npim = 0;
  for (int id = fd; id <= ld; id++) {
    if (evt.StdHepPdg[id] == kPdgProton)
      np++;
    if (evt.StdHepPdg[id] == kPdgNeutron)
      nn++;
    if (evt.StdHepPdg[id] == kPdgPi0)
      npi0++;
    if (evt.StdHepPdg[id] == kPdgPiP)
      npip++;
    if (evt.StdHepPdg[id] == kPdgPiM)
      npim++;
  }
  //   cout << "p: " << np << ", n: " << nn << ", pi+: " << npip << ", pi-" <<
  //   npim << ", pi0: " << npi0 << endl;

  //
  // check whether it is absorption followed by nuclear breakup
  // hadron dissapears & many nucleons (>=2) appear in the final state
  // possible modes: pi + A -> A' + np/pp/npp/nnp/nnpp
  //                 N  + A -> A' + np/pp/npp/nnp/nnppp
  //
  if (npip + npim + npi0 == 0 && nn == 1 && np == 1)
    hadron_fate = kIHAFtAbsNP;
  else if (npip + npim + npi0 == 0 && nn == 0 && np == 2)
    hadron_fate = kIHAFtAbsPP;
  else if (npip + npim + npi0 == 0 && nn == 1 && np == 2)
    hadron_fate = kIHAFtAbsNPP;
  else if (npip + npim + npi0 == 0 && nn == 2 && np == 1)
    hadron_fate = kIHAFtAbsNNP;
  else if (npip + npim + npi0 == 0 && nn == 2 && np == 2)
    hadron_fate = kIHAFtAbs2N2P;
  else if (npip + npim + npi0 == 0 && nn == 2 && np == 3)
    hadron_fate = kIHAFtAbs2N3P;

  //
  // check whether it is pion production
  // possible modes: pi + A -> A' + n + pi+ + pi0
  //                 N  + A -> A' + n + pi+
  //                 N  + A -> A' + n + pi+ + pi0
  //
  // JIMFIX changed npi0==0 -> npi0==1 on line below.
  else if (nn == 1 && np == 0 && npi0 == 0 && npip == 1 && npim == 0)
    hadron_fate = kIHAFtNPip;
  else if (nn == 1 && np == 0 && npi0 == 1 && npip == 1 && npim == 0)
    hadron_fate = kIHAFtNPipPi0;

  //
  // check whether it is charge exchange
  // possible modes: p   + A -> A' + n
  //                 n   + A -> A' + p
  //                 pi- + A -> A' + pi0
  //                 pi+ + A -> A' + pi0
  //                 pi0 + A -> A' + pi+
  //                 pi0 + A -> A' + pi-
  //
  else if ((nd == 1) && ((evt.StdHepPdg[i] == kPdgProton && nn == 1) ||
                         (evt.StdHepPdg[i] == kPdgNeutron && np == 1) ||
                         (evt.StdHepPdg[i] == kPdgPiM && npi0 == 1) ||
                         (evt.StdHepPdg[i] == kPdgPiP && npi0 == 1) ||
                         (evt.StdHepPdg[i] == kPdgPi0 && npip == 1) ||
                         (evt.StdHepPdg[i] == kPdgPi0 && npim == 1))) {
    hadron_fate = kIHAFtCEx;
  }

  //
  // check if the event had zero energy loss
  //
  //
  else if ((nd == 1) && (evt.StdHepPdg[i] == evt.StdHepPdg[fd]) &&
           TMath::Abs(evt.StdHepP4[i][kGStdHepIdxE] -
                      evt.StdHepP4[fd][kGStdHepIdxE]) <
               epsilon_zero_energy_max) {
    // at this point we know that there was no energy change
    // check if there was any change in momentum
    double delta_P_squared = TMath::Power(evt.StdHepP4[i][kGStdHepIdxPx] -
                                              evt.StdHepP4[fd][kGStdHepIdxPx],
                                          2.) +
                             TMath::Power(evt.StdHepP4[i][kGStdHepIdxPy] -
                                              evt.StdHepP4[fd][kGStdHepIdxPy],
                                          2.) +
                             TMath::Power(evt.StdHepP4[i][kGStdHepIdxPz] -
                                              evt.StdHepP4[fd][kGStdHepIdxPz],
                                          2.);

    // either it must be elastic
    if (TMath::Abs(delta_P_squared) > epsilon_delta_P_squared) {
      hadron_fate = kIHAFtElas;
    }

    // else it must have not interacted
    else {
      hadron_fate = kIHAFtUndefined;
    }

  }

  //
  // check if the event had an energy loss consistent with
  // only the binding energy (0.036GeV) being subtracted
  //
  else if ((nd == 1) && (evt.StdHepPdg[i] == evt.StdHepPdg[fd]) &&
           TMath::Abs(evt.StdHepP4[i][kGStdHepIdxE] -
                      evt.StdHepP4[fd][kGStdHepIdxE]) >
               epsilon_binding_energy_min &&
           TMath::Abs(evt.StdHepP4[i][kGStdHepIdxE] -
                      evt.StdHepP4[fd][kGStdHepIdxE]) <
               epsilon_binding_energy_max) {
    // at this point know that only enery difference is nuclear binding
    // energy (except in the unlikely case of inelastic scattering with
    // coincidental energy loss == nucl binding energy of target. This means
    // only two

    // if only had a nuclear binding energy subtraction and no elastic
    // scattering
    // then all scale_px == scale_py == scale_pz
    double scale_px = -1.0; // initial momentum in x / final momentum in x
    double scale_py = -2.0; //     "    "         y        "   "        y
    double scale_pz = -3.0; //     "    "         y        "   "        y

    scale_px = (double)(evt.StdHepP4[i][kGStdHepIdxPx] /
                        evt.StdHepP4[fd][kGStdHepIdxPx]);
    scale_py = (double)(evt.StdHepP4[i][kGStdHepIdxPy] /
                        evt.StdHepP4[fd][kGStdHepIdxPy]);
    scale_pz = (double)(evt.StdHepP4[i][kGStdHepIdxPz] /
                        evt.StdHepP4[fd][kGStdHepIdxPz]);

    double epsilon_parrallel = 1E-6;

    if ((TMath::Abs(scale_px - scale_py) < epsilon_parrallel) &&
        (TMath::Abs(scale_py - scale_pz) < epsilon_parrallel) &&
        (TMath::Abs(scale_pz - scale_px) < epsilon_parrallel)) {

      hadron_fate = kIHAFtUndefined;

    }

    else {
      hadron_fate = kIHAFtElas;
    }

    // at this point we know that there was no energy change except for the
    // binding energy

  }
  // otherwise its inelastic
  else {
    hadron_fate = kIHAFtInelas;
  }

  return hadron_fate;
}
#endif
//_______________________________________________________________________________________
void TGRooTrackerUtils::Print(const ND::GRooTrackerVtx &evt) {
  cout << "\nGRooTrackerMCTruth: " << endl;
  cout << "  --> GeneratorName = " << evt.GeneratorName->GetString() << endl;
  cout << "  --> EvtCode       = " << evt.EvtCode->GetString() << endl;
  cout << "  --> GeomPath      = " << evt.GeomPath->GetString() << endl;
  cout << "  --> EvtNum        = " << evt.EvtNum << endl;
  cout << "  --> EvtXSec       = " << evt.EvtXSec << endl;
  cout << "  --> EvtDXSec      = " << evt.EvtDXSec << endl;
  cout << "  --> EvtWght       = " << evt.EvtWght << endl;
  cout << "  --> EvtProb       = " << evt.EvtProb << endl;

  cout << "GRooTrackerStdHep:" << endl;

  for (int i = 0; i < evt.StdHepN; i++) {
    cout << setfill(' ') << setw(4) << i << " -> pdgc: " << setfill(' ')
         << setw(20) << evt.StdHepPdg[i] << " / rescat: " << setfill(' ')
         << setw(3) << evt.StdHepRescat[i] << " / ist: " << setfill(' ')
         << setw(3) << evt.StdHepStatus[i] << " / mom: " << setfill(' ')
         << setw(3) << evt.StdHepFm[i] << " / daughters: (" << setfill(' ')
         << setw(3) << evt.StdHepFd[i] << ", " << setfill(' ') << setw(3)
         << evt.StdHepLd[i] << ") / E = " << setfill(' ') << setw(10)
         << evt.StdHepP4[i][kGStdHepIdxE] << ", px = " << setfill(' ')
         << setw(10) << evt.StdHepP4[i][kGStdHepIdxPx]
         << ", py = " << setfill(' ') << setw(10)
         << evt.StdHepP4[i][kGStdHepIdxPy] << ", pz = " << setfill(' ')
         << setw(10) << evt.StdHepP4[i][kGStdHepIdxPz] << endl;
  }

  cout << "Jnu-beam pass through info:" << endl;
  cout << "  -->  NuParentPdg      = " << evt.NuParentPdg << endl;
  cout << "  -->  NuParentDecMode  = " << evt.NuParentDecMode << endl;
  //  cout << "  -->  NuParentProNVtx  = " << evt.NuParentProNVtx << endl;
  //  cout << "  -->  NuParentProNVtx  = " << evt.NuParentProNVtx << endl;
  cout << "  -->  NuParentDecP4[4] = " << evt.NuParentDecP4[0] << ", "
       << evt.NuParentDecP4[1] << ", " << evt.NuParentDecP4[2] << ", "
       << evt.NuParentDecP4[3] << endl;
  cout << "  -->  NuParentDecX4[4] = " << evt.NuParentDecX4[0] << ", "
       << evt.NuParentDecX4[1] << ", " << evt.NuParentDecX4[2] << ", "
       << evt.NuParentDecX4[3] << endl;
  cout << "  -->  NuParentProP4[4] = " << evt.NuParentProP4[0] << ", "
       << evt.NuParentProP4[1] << ", " << evt.NuParentProP4[2] << ", "
       << evt.NuParentProP4[3] << endl;
  cout << "  -->  NuParentProX4[4] = " << evt.NuParentProX4[0] << ", "
       << evt.NuParentProX4[1] << ", " << evt.NuParentProX4[2] << ", "
       << evt.NuParentProX4[3] << endl;
  cout << "  -->  NuFluxEntry      = " << evt.NuFluxEntry << endl;
  cout << "  -->  NuIdfd           = " << evt.NuIdfd << endl;
  cout << "  -->  NuCospibm        = " << evt.NuCospibm << endl;
  cout << "  -->  NuCospi0bm       = " << evt.NuCospi0bm << endl;
  cout << "  -->  NuGipart         = " << evt.NuGipart << endl;
  cout << "  -->  NuGamom0         = " << evt.NuGamom0 << endl;
  cout << "  -->  NuGpos0[3] = " << evt.NuGpos0[0] << ", " << evt.NuGpos0[1]
       << ", " << evt.NuGpos0[2] << endl;
  cout << "  -->  NuGvec0[3] = " << evt.NuGvec0[0] << ", " << evt.NuGvec0[1]
       << ", " << evt.NuGvec0[2] << endl;

  cout << "Pass-through file info:" << endl;
  cout << "  --> OrigFileName       = " << evt.OrigFileName->GetString()
       << endl;
  cout << "  --> OrigTreeName       = " << evt.OrigTreeName->GetString()
       << endl;
  cout << "  --> OrigEvtNum         = " << evt.OrigEvtNum << endl;
  cout << "  --> TimeInSpill         = " << evt.TimeInSpill << endl;
  cout << "  --> OrigTreeEntries    = " << evt.OrigTreeEntries << endl;
  cout << "  --> OrigTreePOT        = " << evt.OrigTreePOT << endl;
}
//_______________________________________________________________________________________
