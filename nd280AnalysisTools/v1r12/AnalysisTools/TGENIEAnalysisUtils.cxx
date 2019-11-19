#include "TGENIEAnalysisUtils.hxx"

//__________________________________________________________________________________________________
ND::GRooTrackerVtx *TGENIEAnalysisUtils::GetVertexById(Int_t id) const {
  if (fRooVtx == 0 || fNRooVtx == 0) {
    std::cerr << "[ERROR][TGRooTrackerUtils] RooVertices clones array / number "
                 "not available"
              << std::endl;
    return 0;
  }

  // Check for common 'not set' type values
  if (id < 0)
    return 0;

  ND::GRooTrackerVtx *vtx;

  for (Int_t i = 0; i < *fNRooVtx; ++i) {
    vtx = static_cast<ND::GRooTrackerVtx *>(fRooVtx->At(i));
    if (id == vtx->TruthVertexID)
      return vtx;
  }

  return 0;
}
//____________________________________________________________________________________________________
Bool_t TGENIEAnalysisUtils::IsCC(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a CC event?
  std::string reactionCode(vertex->ReactionCode);
  return (reactionCode.find("Weak[CC]") != std::string::npos);
}

//____________________________________________________________________________________________________
Bool_t TGENIEAnalysisUtils::IsNC(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a NC event?
  std::string reactionCode(vertex->ReactionCode);
  return (reactionCode.find("Weak[NC]") != std::string::npos);
}

//____________________________________________________________________________________________________
Bool_t TGENIEAnalysisUtils::IsQELCC(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a QEL CC event?
  /// Note: Excludes QEL charm production -
  std::string reactionCode(vertex->ReactionCode);
  return (!this->IsCharmProd(vertex) &&
          reactionCode.find("Weak[CC],QES") != std::string::npos);
}

//____________________________________________________________________________________________________
Bool_t TGENIEAnalysisUtils::IsELNC(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it an Elastic NC event?
  std::string reactionCode(vertex->ReactionCode);
  return (reactionCode.find("Weak[NC],QES") != std::string::npos);
}

//____________________________________________________________________________________________________
Bool_t TGENIEAnalysisUtils::IsRES(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a Resonance (CC+NC) event?
  std::string reactionCode(vertex->ReactionCode);
  return (reactionCode.find("RES") != std::string::npos);
}

//____________________________________________________________________________________________________
Bool_t TGENIEAnalysisUtils::IsRESCC(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a Resonance CC event?
  std::string reactionCode(vertex->ReactionCode);
  return (reactionCode.find("Weak[CC],RES") != std::string::npos);
}

//____________________________________________________________________________________________________
Bool_t TGENIEAnalysisUtils::IsRESNC(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a Resonance NC event?
  std::string reactionCode(vertex->ReactionCode);
  return (reactionCode.find("Weak[NC],RES") != std::string::npos);
}

//____________________________________________________________________________________________________
Bool_t TGENIEAnalysisUtils::IsDIS(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a DIS (CC+NC) event?
  /// Note: Excludes DIS charm production -
  std::string reactionCode(vertex->ReactionCode);
  return (!this->IsCharmProd(vertex) &&
          reactionCode.find("DIS") != std::string::npos);
}

//____________________________________________________________________________________________________
Bool_t TGENIEAnalysisUtils::IsDISCC(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a DIS CC event?
  /// Note: Excludes DIS charm production -
  std::string reactionCode(vertex->ReactionCode);
  return (!this->IsCharmProd(vertex) &&
          reactionCode.find("Weak[CC],DIS") != std::string::npos);
}

//____________________________________________________________________________________________________
Bool_t TGENIEAnalysisUtils::IsDISNC(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a DIS NC event?
  /// Note: Excludes DIS charm production -
  std::string reactionCode(vertex->ReactionCode);
  return (!this->IsCharmProd(vertex) &&
          reactionCode.find("Weak[NC],DIS") != std::string::npos);
}

//____________________________________________________________________________________________________
Bool_t TGENIEAnalysisUtils::IsCOH(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a COH (CC+NC, elastic + pi prod + ...)  event?
  std::string reactionCode(vertex->ReactionCode);
  return (reactionCode.find("COH") != std::string::npos);
}

//____________________________________________________________________________________________________
Bool_t TGENIEAnalysisUtils::IsCOHPiCC(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a COH CC pion production event?
  std::string reactionCode(vertex->ReactionCode);
  return (reactionCode.find("Weak[CC],COHPi") != std::string::npos);
}

//____________________________________________________________________________________________________
Bool_t TGENIEAnalysisUtils::IsCOHPiNC(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a COH NC pion production event?
  std::string reactionCode(vertex->ReactionCode);
  return (reactionCode.find("Weak[NC],COHPi") != std::string::npos);
}

//____________________________________________________________________________________________________
Bool_t TGENIEAnalysisUtils::IsCOHEl(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a COH elastic event?
  std::string reactionCode(vertex->ReactionCode);
  return (reactionCode.find("COHEl") != std::string::npos);
}

//____________________________________________________________________________________________________
Bool_t TGENIEAnalysisUtils::IsAMNuGamma(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it an anomaly-mediated nu-gamma interaction?
  std::string reactionCode(vertex->ReactionCode);
  return (reactionCode.find("AMNuGamma") != std::string::npos);
}

//____________________________________________________________________________________________________
Bool_t TGENIEAnalysisUtils::IsNuEEL(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a (anti)neutrino - electron elastic event?
  std::string reactionCode(vertex->ReactionCode);
  return (reactionCode.find("NuEEL") != std::string::npos);
}

//____________________________________________________________________________________________________
Bool_t TGENIEAnalysisUtils::IsIMD(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it an inverse muon decay event?
  std::string reactionCode(vertex->ReactionCode);
  return (reactionCode.find("IMD") != std::string::npos);
}

//____________________________________________________________________________________________________
Bool_t TGENIEAnalysisUtils::IsCharmProd(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a charm production event?
  std::string reactionCode(vertex->ReactionCode);
  return (reactionCode.find("charm") != std::string::npos);
}

//____________________________________________________________________________________________________
Bool_t TGENIEAnalysisUtils::IsQELCharm(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a QEL charm production event?
  std::string reactionCode(vertex->ReactionCode);
  return (reactionCode.find("QES;charm") != std::string::npos);
}

//____________________________________________________________________________________________________
Bool_t TGENIEAnalysisUtils::IsDISCharm(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Is it a DIS charm production event?
  std::string reactionCode(vertex->ReactionCode);
  return (reactionCode.find("DIS;charm") != std::string::npos);
}

//____________________________________________________________________________________________________
Int_t TGENIEAnalysisUtils::NeutrinoPdg(
    ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const {
  /// Return the input neutrino pdg code
  return -1;
}

//____________________________________________________________________________________________________
Int_t TGENIEAnalysisUtils::TargetPdg(
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
Int_t TGENIEAnalysisUtils::TargetZ(
    ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const {
  /// Return the hit target atomic number
  return -1;
}

//____________________________________________________________________________________________________
Int_t TGENIEAnalysisUtils::TargetA(
    ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const {
  /// Return the hit target mass number
  return -1;
}

//____________________________________________________________________________________________________
Bool_t TGENIEAnalysisUtils::HitNucleon(
    ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const {
  /// Is there a hit nucleon? (may not be set, eg for COH or ve- events)
  return false;
}

//____________________________________________________________________________________________________
Int_t TGENIEAnalysisUtils::HitNucleonPdg(
    ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const {
  /// Return the hit nucleon pdg code? (if the hit nucleon is set)
  return -1;
}

//____________________________________________________________________________________________________
Bool_t TGENIEAnalysisUtils::HitQuark(
    ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const {
  /// Is there a hit quark? (a hit quark can only be set in DIS events)
  return false;
}

//____________________________________________________________________________________________________
Int_t TGENIEAnalysisUtils::HitQuarkPdg(
    ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const {
  /// Return the hit quark pdg code (if the hit quark is set)
  return -1;
}

//____________________________________________________________________________________________________
Bool_t TGENIEAnalysisUtils::HitSeaQuark(
    ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const {
  /// Is the hit quark from the sea? (if it is set at all)
  return false;
}

//____________________________________________________________________________________________________
Bool_t TGENIEAnalysisUtils::HitValenceQuark(
    ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const {
  /// Is the hit quark a valence one? (if it is set at all)
  return false;
}

//____________________________________________________________________________________________________
Int_t TGENIEAnalysisUtils::ResonanceId(
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
std::string TGENIEAnalysisUtils::ResonanceName(
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
Int_t TGENIEAnalysisUtils::NProton_primary(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of protons in primary hadronic shower (before intranuclear hadron
  /// transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TGENIEAnalysisUtils::NNeutron_primary(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of neutrons in primary hadronic shower (before intranuclear hadron
  /// transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TGENIEAnalysisUtils::NPi0_primary(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of pi0 in primary hadronic shower (before intranuclear hadron
  /// transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TGENIEAnalysisUtils::NPip_primary(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of pi+ in primary hadronic shower (before intranuclear hadron
  /// transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TGENIEAnalysisUtils::NPim_primary(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of pi- in primary hadronic shower (before intranuclear hadron
  /// transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TGENIEAnalysisUtils::NK0_primary(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of K0+\bar{K0} in primary hadronic shower (before intranuclear
  /// hadron transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TGENIEAnalysisUtils::NKp_primary(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of K+ in primary hadronic shower (before intranuclear hadron
  /// transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TGENIEAnalysisUtils::NKm_primary(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of K- in primary hadronic shower (before intranuclear hadron
  /// transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TGENIEAnalysisUtils::NProton_fs(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of protons in final state (after intranuclear hadron transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TGENIEAnalysisUtils::NNeutron_fs(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of neutrons in final state (after intranuclear hadron transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TGENIEAnalysisUtils::NPi0_fs(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of pi0 in final state (after intranuclear hadron transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TGENIEAnalysisUtils::NPip_fs(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of pi+ in final state (after intranuclear hadron transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TGENIEAnalysisUtils::NPim_fs(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of pi- in final state (after intranuclear hadron transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TGENIEAnalysisUtils::NK0_fs(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of K0+\bar{K0} in final state (after intranuclear hadron transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TGENIEAnalysisUtils::NKp_fs(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of K+ in final state (after intranuclear hadron transport)
  return -1;
}

//____________________________________________________________________________________________________
Int_t TGENIEAnalysisUtils::NKm_fs(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Number of K- in final state (after intranuclear hadron transport)
  return -1;
}

//____________________________________________________________________________________________________
Double_t TGENIEAnalysisUtils::Kinematics_x(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Return the Bjorken x kinematical variablefor the current event
  return 0.;
}

//____________________________________________________________________________________________________
Double_t TGENIEAnalysisUtils::Kinematics_v(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Return the energy transfer to the target for the current event (GeV)
  return 0.;
}

//____________________________________________________________________________________________________
Double_t TGENIEAnalysisUtils::Kinematics_y(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Return the inelasticity y for the current event
  return 0.;
}

//____________________________________________________________________________________________________
Double_t TGENIEAnalysisUtils::Kinematics_t(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Return the energy transfer to the nucleus for COHPi events
  return 0.;
}

//____________________________________________________________________________________________________
Double_t TGENIEAnalysisUtils::Kinematics_W(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Return the hadronic invariant mass W for the current event (in GeV)
  return 0.;
}

//____________________________________________________________________________________________________
Double_t TGENIEAnalysisUtils::Kinematics_Q2(
    ND::TTruthVerticesModule::TTruthVertex * /*vertex*/) const {
  /// Return the momentum transfer Q2 (>0) for the current event (in GeV^2)
  return 0.;
}

//____________________________________________________________________________________________________
Double_t TGENIEAnalysisUtils::Kinematics_q2(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  /// Return the momentum transfer q2 (<0) for the current event (in GeV^2)
  return -1 * this->Kinematics_Q2(vertex);
}

//____________________________________________________________________________________________________
