#include "TMath.h"
#include "TNEUTAnalysisUtils.hxx"

//__________________________________________________________________________________________________
ND::NRooTrackerVtx *TNEUTAnalysisUtils::GetVertexById(Int_t id) const {
  if (fRooVtx == 0 || fNRooVtx == 0) {
    std::cerr << "[ERROR][TGRooTrackerUtils] RooVertices clones array / number "
                 "not available"
              << std::endl;
    return 0;
  }

  // Check for common 'not set' type values
  if (id < 0)
    return 0;

  ND::NRooTrackerVtx *vtx;

  for (Int_t i = 0; i < *fNRooVtx; ++i) {
    vtx = static_cast<ND::NRooTrackerVtx *>(fRooVtx->At(i));
    if (id == vtx->TruthVertexID)
      return vtx;
  }

  return 0;
}
//____________________________________________________________________________________________________
Int_t TNEUTAnalysisUtils::GetReactionCode(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  return std::atoi(vertex->ReactionCode.substr(strlen("NEUT ")).c_str());
}

/*
NEUT Interation modes for reference

        Mode    : Comment
                :
                : ***   Neutrino   ***
                : ****  Charged Current  ****
                :
                : -- quasi-elastic scattering --
          1     : NEU,N --> LEPTON-,P
                :
                : -- single pi from resonances --
         11     : NEU,P --> LEPTON-,P,PI+
         12     : NEU,N --> LEPTON-,P,PI0
         13     : NEU,N --> LEPTON-,N,PI+
                :
                : -- coherent pi production --
         16     : NEU,O(16) --> LEPTON-,O(16),PI+
                :
                : -- multi pi production (DIS), (1.3 < W < 2.0 GeV) --
         21     : NEU,(N OR P) --> LEPTON-,(N OR P),MULTI PI
                :
                : -- single eta from resonances --
         22     : NEU,N --> LEPTON-,P,ETA0
                :
                : -- single k from resonances --
         23     : NEU,N --> LEPTON-,LAMBDA,K+
                :
                : -- DIS (2.0 GeV < W , JETset) --
         26     : NEU,(N OR P) --> LEPTON-,(N OR P),MESONS
                :
                :
                : **** Neutal Current ****
                :
                : -- single pi from resonances --
         31     : NEU,N --> NEU,N,PI0
         32     : NEU,P --> NEU,P,PI0
         33     : NEU,N --> NEU,P,PI-
         34     : NEU,P --> NEU,N,PI+
                :
                : -- coherent pi production --
         36     : NEU,O(16) --> NEU,O(16),PI0
                :
                : -- multi pi production (DIS), (1.3 < W < 2.0 GeV) --
         41     : NEU,(N OR P) --> NEU,(N OR P),MULTI PI
                :
                : -- single eta from resonances --
         42     : NEU,N --> NEU,N,ETA0
         43     : NEU,P --> NEU,P,ETA0
                :
                : -- single  k  from resonances --
         44     : NEU,N --> NEU,LAMBDA,K0
         45     : NEU,P --> NEU,LAMBDA,K+
                :
                : -- DIS (2.0 GeV < W , JETset) --
         46     : NEU,(N OR P) --> NEU,(N OR P),MESONS
                :
                : -- elastic scattering --
         51     : NEU,P --> NEU,P
         52     : NEU,N --> NEU,N
                :
                :
                :***  Anti neutrino  ***
                :**** Charged Current ****
                :
                : -- quasi-elastic scattering --
         -1     : NEUBAR,P --> LEPTON+,N
                :
                : -- single pi from resonances --
        -11     : NEUBAR,N --> LEPTON+,N,PI-
        -12     : NEUBAR,P --> LEPTON+,N,PI0
        -13     : NEUBAR,P --> LEPTON+,P,PI-
                :
                : -- coherent pi production --
        -16     : NEUBAR,O(16) --> LEPTON+,O(16),PI-
                :
                : -- multi pi production (DIS), (1.3 < W < 2.0 GeV) --
        -21     : NEUBAR,(N OR P) --> LEPTON+,(N OR P),MULTI PI
                :
                : -- single eta from resonances --
        -22     : NEUBAR,P --> LEPTON+,N,ETA0
                :
                : -- single k from resonances --
        -23     : NEUBAR,P --> LEPTON+,LAMBDA,K0
                :
                : -- DIS (2.0 GeV < W , JETset) --
        -26     : NEUBAR,(N OR P) --> LEPTON+,(N OR P),MESONS
                :
                :
                :***  Neutal  Current ***
                :
                : -- single pi from resonances --
        -31     : NEUBAR,N --> NEUBAR,N,PI0
        -32     : NEUBAR,P --> NEUBAR,P,PI0
        -33     : NEUBAR,N --> NEUBAR,P,PI-
        -34     : NEUBAR,P --> NEUBAR,N,PI+
                :
                : -- coherent pi production --
        -36     : NEUBAR,O(16) --> NEUBAR,O(16),PI0
                :
                : -- multi pi production (DIS), (1.3 < W < 2.0 GeV) --
        -41     : NEUBAR,(N OR P) --> NEUBAR,(N OR P),MULTI PI
                :
                : -- single eta from resonances --
        -42     : NEUBAR,N --> NEUBAR,N,ETA0
        -43     : NEUBAR,P --> NEUBAR,P,ETA0
                :
                : -- single  k  from resonances --
        -44     : NEUBAR,N --> NEUBAR,LAMBDA,K0
        -45     : NEUBAR,P --> NEUBAR,LAMBDA,K+
                :
                : -- DIS (2.0 GeV < W , JETset) --
        -46     : NEUBAR,(N OR P) --> NEUBAR,(N OR P),MESONS
                :
                : -- elastic scattering --
        -51     : NEUBAR,P --> NEUBAR,P
        -52     : NEUBAR,N --> NEUBAR,N
*/

//____________________________________________________________________________________________________
Bool_t
TNEUTAnalysisUtils::IsCC(ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a CC event?
  Int_t number = this->GetReactionCode(vertex);
  if (TMath::Abs(number) <= 27) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t
TNEUTAnalysisUtils::IsNC(ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a NC event?
  Int_t number = this->GetReactionCode(vertex);
  if (TMath::Abs(number) >= 31) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNEUTAnalysisUtils::IsQELCC(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a QEL CC event?
  Int_t number = this->GetReactionCode(vertex);
  if (TMath::Abs(number) == 1) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNEUTAnalysisUtils::IsELNC(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it an Elastic NC event?
  Int_t number = this->GetReactionCode(vertex);
  if (TMath::Abs(number) == 51 || TMath::Abs(number) == 52) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNEUTAnalysisUtils::IsRES(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a Resonance (CC+NC) event?
  Int_t number = this->GetReactionCode(vertex);
  if ((TMath::Abs(number) >= 11 && TMath::Abs(number) <= 13) ||
      (TMath::Abs(number) >= 22 && TMath::Abs(number) <= 23) ||
      (TMath::Abs(number) >= 31 && TMath::Abs(number) <= 34) ||
      (TMath::Abs(number) >= 42 && TMath::Abs(number) <= 45)) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNEUTAnalysisUtils::IsRESCC(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a Resonance CC event?
  Int_t number = this->GetReactionCode(vertex);
  if ((TMath::Abs(number) >= 11 && TMath::Abs(number) <= 13) ||
      (TMath::Abs(number) >= 22 && TMath::Abs(number) <= 23)) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNEUTAnalysisUtils::IsRESNC(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a Resonance NC event?
  Int_t number = this->GetReactionCode(vertex);
  if ((TMath::Abs(number) >= 31 && TMath::Abs(number) <= 34) ||
      (TMath::Abs(number) >= 42 && TMath::Abs(number) <= 45)) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNEUTAnalysisUtils::IsDIS(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a DIS (CC+NC) event?
  /// Note: Excludes DIS charm production -
  Int_t number = this->GetReactionCode(vertex);
  if (TMath::Abs(number) == 21 || TMath::Abs(number) == 26 ||
      TMath::Abs(number) == 41 || TMath::Abs(number) == 46) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNEUTAnalysisUtils::IsDISCC(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a DIS CC event?
  /// Note: Excludes DIS charm production -
  Int_t number = this->GetReactionCode(vertex);
  if (TMath::Abs(number) == 21 || TMath::Abs(number) == 42) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNEUTAnalysisUtils::IsDISNC(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a DIS NC event?
  /// Note: Excludes DIS charm production -
  Int_t number = this->GetReactionCode(vertex);
  if (TMath::Abs(number) == 41 || TMath::Abs(number) == 46) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNEUTAnalysisUtils::IsCOH(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a COH (CC+NC, elastic + pi prod + ...)  event?
  Int_t number = this->GetReactionCode(vertex);
  if (TMath::Abs(number) == 16 || TMath::Abs(number) == 36) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNEUTAnalysisUtils::IsCOHPiCC(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a COH CC pion production event?
  Int_t number = this->GetReactionCode(vertex);
  if (TMath::Abs(number) == 16) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNEUTAnalysisUtils::IsCOHPiNC(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a COH NC pion production event?
  Int_t number = this->GetReactionCode(vertex);
  if (TMath::Abs(number) == 36) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNEUTAnalysisUtils::IsCOHEl(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a COH elastic event?
  Int_t number = this->GetReactionCode(vertex);
  if (TMath::Abs(number) == -9999) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNEUTAnalysisUtils::IsAMNuGamma(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it an anomaly-mediated nu-gamma interaction?
  Int_t number = this->GetReactionCode(vertex);
  if (TMath::Abs(number) == -9999) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNEUTAnalysisUtils::IsNuEEL(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a (anti)neutrino - electron elastic event?
  Int_t number = this->GetReactionCode(vertex);
  if (TMath::Abs(number) == -9999) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNEUTAnalysisUtils::IsIMD(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it an inverse muon decay event?
  Int_t number = this->GetReactionCode(vertex);
  if (TMath::Abs(number) == -9999) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNEUTAnalysisUtils::IsCharmProd(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a charm production event?
  Int_t number = this->GetReactionCode(vertex);
  if (TMath::Abs(number) == -9999) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNEUTAnalysisUtils::IsQELCharm(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a QEL charm production event?
  Int_t number = this->GetReactionCode(vertex);
  if (TMath::Abs(number) == -9999) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Bool_t TNEUTAnalysisUtils::IsDISCharm(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a DIS charm production event?
  Int_t number = this->GetReactionCode(vertex);
  if (TMath::Abs(number) == -9999) {
    return kTRUE;
  } else {
    return kFALSE;
  }
}

//____________________________________________________________________________________________________
Int_t TNEUTAnalysisUtils::NeutrinoPdg(
    ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const {
  /// Return the input neutrino pdg code
  return -1;
}

//____________________________________________________________________________________________________
Int_t TNEUTAnalysisUtils::TargetPdg(
    ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const {
  /// Return the hit target pdg code
  /// Note: Using PDG-2006 ion code convention 10LZZZAAAI (example: Fe56 ->
  /// 1000260560)
  ///       Free nucleon targets 1000010010(proton), 1000000010(neutron) are
  ///       automatically converted to
  ///        the more familiar codes (2212, 2112 respectivelly)

  return -1;
}

//____________________________________________________________________________________________________
Int_t TNEUTAnalysisUtils::TargetZ(
    ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const {
  /// Return the hit target atomic number
  return -1;
}

//____________________________________________________________________________________________________
Int_t TNEUTAnalysisUtils::TargetA(
    ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const {
  /// Return the hit target mass number
  return -1;
}

//____________________________________________________________________________________________________
Bool_t TNEUTAnalysisUtils::HitNucleon(
    ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const {
  /// Is there a hit nucleon? (may not be set, eg for COH or ve- events)
  return false;
}

//____________________________________________________________________________________________________
Int_t TNEUTAnalysisUtils::HitNucleonPdg(
    ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const {
  /// Return the hit nucleon pdg code? (if the hit nucleon is set)
  return -1;
}

//____________________________________________________________________________________________________
Bool_t TNEUTAnalysisUtils::HitQuark(
    ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const {
  /// Is there a hit quark? (a hit quark can only be set in DIS events)
  return false;
}

//____________________________________________________________________________________________________
Int_t TNEUTAnalysisUtils::HitQuarkPdg(
    ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const {
  /// Return the hit quark pdg code (if the hit quark is set)
  return -1;
}

//____________________________________________________________________________________________________
Bool_t TNEUTAnalysisUtils::HitSeaQuark(
    ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const {
  /// Is the hit quark from the sea? (if it is set at all)
  return false;
}

//____________________________________________________________________________________________________
Bool_t TNEUTAnalysisUtils::HitValenceQuark(
    ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const {
  /// Is the hit quark a valence one? (if it is set at all)
  return false;
}

//____________________________________________________________________________________________________
Int_t TNEUTAnalysisUtils::ResonanceId(
    ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const {
  /// Return produced baryon resonance id
  ///   no resonance => -1
  ///   P33_1232     =>  0
  ///   S11_1535     =>  1
  ///   D13_1520     =>  2
  ///   S11_1650     =>  3
  ///   D13_1700     =>  4
  ///   D15_1675     =>  5
  ///   S31_1620     =>  6
  ///   D33_1700     =>  7
  ///   P11_1440     =>  8
  ///   P33_1600     =>  9
  ///   P13_1720     => 10
  ///   F15_1680     => 11
  ///   P31_1910     => 12
  ///   P33_1920     => 13
  ///   F35_1905     => 14
  ///   F37_1950     => 15
  ///   P11_1710     => 16
  ///   F17_1970     => 17

  return -1;
}

//____________________________________________________________________________________________________
std::string TNEUTAnalysisUtils::ResonanceName(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  Int_t resid = this->ResonanceId(vertex);

  switch (resid) {
  case (-1):
    return "no_resonance";
    break;
  case (0):
    return "P33_1232";
    break;
  case (1):
    return "S11_1535";
    break;
  case (2):
    return "D13_1520";
    break;
  case (3):
    return "S11_1650";
    break;
  case (4):
    return "D13_1700";
    break;
  case (5):
    return "D15_1675";
    break;
  case (6):
    return "S31_1620";
    break;
  case (7):
    return "D33_1700";
    break;
  case (8):
    return "P11_1440";
    break;
  case (9):
    return "P33_1600";
    break;
  case (10):
    return "P13_1720";
    break;
  case (11):
    return "F15_1680";
    break;
  case (12):
    return "P31_1910";
    break;
  case (13):
    return "P33_1920";
    break;
  case (14):
    return "F35_1905";
    break;
  case (15):
    return "F37_1950";
    break;
  case (16):
    return "P11_1710";
    break;
  case (17):
    return "F17_1970";
    break;
  default:
    return "no_resonance";
    break;
  }
  return "??";
}

//____________________________________________________________________________________________________
Int_t TNEUTAnalysisUtils::NProton_primary(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of protons in primary hadronic shower (before intranuclear hadron
  /// transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TNEUTAnalysisUtils::NNeutron_primary(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of neutrons in primary hadronic shower (before intranuclear hadron
  /// transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TNEUTAnalysisUtils::NPi0_primary(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of pi0 in primary hadronic shower (before intranuclear hadron
  /// transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TNEUTAnalysisUtils::NPip_primary(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of pi+ in primary hadronic shower (before intranuclear hadron
  /// transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TNEUTAnalysisUtils::NPim_primary(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of pi- in primary hadronic shower (before intranuclear hadron
  /// transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TNEUTAnalysisUtils::NK0_primary(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of K0+\bar{K0} in primary hadronic shower (before intranuclear
  /// hadron transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TNEUTAnalysisUtils::NKp_primary(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of K+ in primary hadronic shower (before intranuclear hadron
  /// transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TNEUTAnalysisUtils::NKm_primary(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of K- in primary hadronic shower (before intranuclear hadron
  /// transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TNEUTAnalysisUtils::NProton_fs(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of protons in final state (after intranuclear hadron transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TNEUTAnalysisUtils::NNeutron_fs(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of neutrons in final state (after intranuclear hadron transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TNEUTAnalysisUtils::NPi0_fs(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of pi0 in final state (after intranuclear hadron transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TNEUTAnalysisUtils::NPip_fs(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of pi+ in final state (after intranuclear hadron transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TNEUTAnalysisUtils::NPim_fs(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of pi- in final state (after intranuclear hadron transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TNEUTAnalysisUtils::NK0_fs(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of K0+\bar{K0} in final state (after intranuclear hadron transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TNEUTAnalysisUtils::NKp_fs(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of K+ in final state (after intranuclear hadron transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TNEUTAnalysisUtils::NKm_fs(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of K- in final state (after intranuclear hadron transport)
  return -1;
}

//____________________________________________________________________________________________________
Double_t TNEUTAnalysisUtils::Kinematics_x(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Return the Bjorken x kinematical variablefor the current event
  return 0.;
}

//____________________________________________________________________________________________________
Double_t TNEUTAnalysisUtils::Kinematics_v(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Return the energy transfer to the target for the current event (GeV)
  return 0.;
}

//____________________________________________________________________________________________________
Double_t TNEUTAnalysisUtils::Kinematics_y(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Return the inelasticity y for the current event
  return 0.;
}

//____________________________________________________________________________________________________
Double_t TNEUTAnalysisUtils::Kinematics_t(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Return the energy transfer to the nucleus for COHPi events
  return 0.;
}

//____________________________________________________________________________________________________
Double_t TNEUTAnalysisUtils::Kinematics_W(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Return the hadronic invariant mass W for the current event (in GeV)
  return 0.;
}

//____________________________________________________________________________________________________
Double_t TNEUTAnalysisUtils::Kinematics_Q2(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Return the momentum transfer Q2 (>0) for the current event (in GeV^2)
  return 0.;
}

//____________________________________________________________________________________________________
Double_t TNEUTAnalysisUtils::Kinematics_q2(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Return the momentum transfer q2 (<0) for the current event (in GeV^2)
  return -1 * this->Kinematics_Q2(vertex);
}
