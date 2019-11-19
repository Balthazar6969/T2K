#ifndef TNEUTAnalysisUtils_h_seen
#define TNEUTAnalysisUtils_h_seen
#include "TVNeutrinoGeneratorAnalysisUtils.hxx"
#include "libReadoaAnalysis/libReadoaAnalysisProjectHeaders.h"
#include <TObject.h>

/// oaAnalysis read-out utility in macros/AnalysisTools,
/// which helps understand NEUT neutrino generator output
///
/// This can be queried to ask if particular interactions were CC or
/// not, for example.
class TNEUTAnalysisUtils : public TVNeutrinoGeneratorAnalysisUtils {

public:
  TNEUTAnalysisUtils(TClonesArray *vertices, Int_t *n_vertices)
      : fRooVtx(vertices), fNRooVtx(n_vertices){};

  TNEUTAnalysisUtils() : fRooVtx(0), fNRooVtx(0){};

  /// Change the location of the TClonesArray of vertices
  void SetVerticesArray(TClonesArray *vertices) { fRooVtx = vertices; }

  /// Change the location of the TClonesArray of vertices
  void SetVerticesNumber(Int_t *n_vertices) { fNRooVtx = n_vertices; }

  ND::NRooTrackerVtx *GetVertexById(Int_t id) const;

  virtual ~TNEUTAnalysisUtils(){};

  Int_t GetReactionCode(ND::TTruthVerticesModule::TTruthVertex *vertex) const;

  /// Is it a CC event?
  virtual Bool_t IsCC(ND::TTruthVerticesModule::TTruthVertex *vertex) const;

  /// Is it a NC event?
  virtual Bool_t IsNC(ND::TTruthVerticesModule::TTruthVertex *vertex) const;

  /// Is it a QEL CC event?Note: Excludes QEL charm production -
  virtual Bool_t IsQELCC(ND::TTruthVerticesModule::TTruthVertex *vertex) const;

  /// Is it an Elastic NC event?
  virtual Bool_t IsELNC(ND::TTruthVerticesModule::TTruthVertex *vertex) const;

  /// Is it a Resonance (CC+NC) event?
  virtual Bool_t IsRES(ND::TTruthVerticesModule::TTruthVertex *vertex) const;

  /// Is it a Resonance CC event?
  virtual Bool_t IsRESCC(ND::TTruthVerticesModule::TTruthVertex *vertex) const;

  /// Is it a Resonance NC event?
  virtual Bool_t IsRESNC(ND::TTruthVerticesModule::TTruthVertex *vertex) const;

  /// Is it a DIS (CC+NC) event?Note: Excludes DIS charm production -
  virtual Bool_t IsDIS(ND::TTruthVerticesModule::TTruthVertex *vertex) const;

  /// Is it a DIS CC event?Note: Excludes DIS charm production -
  virtual Bool_t IsDISCC(ND::TTruthVerticesModule::TTruthVertex *vertex) const;

  /// Is it a DIS NC event?Note: Excludes DIS charm production -
  virtual Bool_t IsDISNC(ND::TTruthVerticesModule::TTruthVertex *vertex) const;

  /// Is it a COH (CC+NC, elastic + pi prod + ...)  event?
  virtual Bool_t IsCOH(ND::TTruthVerticesModule::TTruthVertex *vertex) const;

  /// Is it a COH CC pion production event?
  virtual Bool_t
  IsCOHPiCC(ND::TTruthVerticesModule::TTruthVertex *vertex) const;

  /// Is it a COH NC pion production event?
  virtual Bool_t
  IsCOHPiNC(ND::TTruthVerticesModule::TTruthVertex *vertex) const;

  /// Is it a COH elastic event?
  virtual Bool_t IsCOHEl(ND::TTruthVerticesModule::TTruthVertex *vertex) const;

  /// Is it an anomaly-mediated nu-gamma interaction?
  virtual Bool_t
  IsAMNuGamma(ND::TTruthVerticesModule::TTruthVertex *vertex) const;

  /// Is it a (anti)neutrino - electron elastic event?
  virtual Bool_t IsNuEEL(ND::TTruthVerticesModule::TTruthVertex *vertex) const;

  /// Is it an inverse muon decay event?
  virtual Bool_t IsIMD(ND::TTruthVerticesModule::TTruthVertex *vertex) const;

  /// Is it a charm production event?
  virtual Bool_t
  IsCharmProd(ND::TTruthVerticesModule::TTruthVertex *vertex) const;

  /// Is it a QEL charm production event?
  virtual Bool_t
  IsQELCharm(ND::TTruthVerticesModule::TTruthVertex *vertex) const;

  /// Is it a DIS charm production event?
  virtual Bool_t
  IsDISCharm(ND::TTruthVerticesModule::TTruthVertex *vertex) const;

  /// Return the input neutrino pdg code
  virtual Int_t
  NeutrinoPdg(ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const;

  /// Return the hit target pdg codeNote: Using PDG-2006 ion code convention
  // 10LZZZAAAI (example: Fe56 -> 1000260560)      Free nucleon targets
  // 1000010010(proton), 1000000010(neutron) are automatically converted to
  // the more familiar codes (2212, 2112 respectivelly)
  virtual Int_t
  TargetPdg(ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const;

  /// Return the hit target atomic number
  virtual Int_t
  TargetZ(ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const;

  /// Return the hit target mass number
  virtual Int_t
  TargetA(ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const;

  /// Is there a hit nucleon? (may not be set, eg for COH or ve- events)
  virtual Bool_t
  HitNucleon(ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const;

  /// Return the hit nucleon pdg code? (if the hit nucleon is set)
  virtual Int_t
  HitNucleonPdg(ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const;

  /// Is there a hit quark? (a hit quark can only be set in DIS events)
  virtual Bool_t
  HitQuark(ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const;

  /// Return the hit quark pdg code (if the hit quark is set)
  virtual Int_t
  HitQuarkPdg(ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const;

  /// Is the hit quark from the sea? (if it is set at all)
  virtual Bool_t
  HitSeaQuark(ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const;

  /// Is the hit quark a valence one? (if it is set at all)
  virtual Bool_t
  HitValenceQuark(ND::TTruthVerticesModule::TTruthVertex *vertex) const;

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
  virtual Int_t
  ResonanceId(ND::TTruthVerticesModule::TTruthVertex *vertex) const;
  virtual std::string
  ResonanceName(ND::TTruthVerticesModule::TTruthVertex *vertex) const;

  /// Number of protons in primary hadronic shower (before intranuclear hadron
  /// transport)
  virtual Int_t
  NProton_primary(ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const;

  /// Number of neutrons in primary hadronic shower (before intranuclear hadron
  /// transport)
  virtual Int_t
  NNeutron_primary(ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const;

  /// Number of pi0 in primary hadronic shower (before intranuclear hadron
  /// transport)
  virtual Int_t
  NPi0_primary(ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const;

  /// Number of pi+ in primary hadronic shower (before intranuclear hadron
  /// transport)
  virtual Int_t
  NPip_primary(ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const;

  /// Number of pi- in primary hadronic shower (before intranuclear hadron
  /// transport)
  virtual Int_t
  NPim_primary(ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const;

  /// Number of K0+\bar{K0} in primary hadronic shower (before intranuclear
  /// hadron transport)
  virtual Int_t
  NK0_primary(ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const;

  /// Number of K+ in primary hadronic shower (before intranuclear hadron
  /// transport)
  virtual Int_t
  NKp_primary(ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const;

  /// Number of K- in primary hadronic shower (before intranuclear hadron
  /// transport)
  virtual Int_t
  NKm_primary(ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const;

  /// Number of protons in final state (after intranuclear hadron transport)
  virtual Int_t
  NProton_fs(ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const;

  /// Number of neutrons in final state (after intranuclear hadron transport)
  virtual Int_t
  NNeutron_fs(ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const;

  /// Number of pi0 in final state (after intranuclear hadron transport)
  virtual Int_t
  NPi0_fs(ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const;

  /// Number of pi+ in final state (after intranuclear hadron transport)
  virtual Int_t
  NPip_fs(ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const;

  /// Number of pi- in final state (after intranuclear hadron transport)
  virtual Int_t
  NPim_fs(ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const;

  /// Number of K0+\bar{K0} in final state (after intranuclear hadron transport)
  virtual Int_t
  NK0_fs(ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const;

  /// Number of K+ in final state (after intranuclear hadron transport)
  virtual Int_t
  NKp_fs(ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const;

  /// Number of K- in final state (after intranuclear hadron transport)
  virtual Int_t
  NKm_fs(ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const;

  /// Return the Bjorken x kinematical variablefor the current event
  virtual Double_t
  Kinematics_x(ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const;

  /// Return the energy transfer to the target for the current event (GeV)
  virtual Double_t
  Kinematics_v(ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const;

  /// Return the inelasticity y for the current event
  virtual Double_t
  Kinematics_y(ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const;

  /// Return the energy transfer to the nucleus for COHPi events
  virtual Double_t
  Kinematics_t(ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const;

  /// Return the hadronic invariant mass W for the current event (in GeV)
  virtual Double_t
  Kinematics_W(ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const;

  /// Return the momentum transfer Q2 (>0) for the current event (in GeV^2)
  virtual Double_t
  Kinematics_Q2(ND::TTruthVerticesModule::TTruthVertex * /* vertex */) const;

  /// Return the momentum transfer q2 (<0) for the current event (in GeV^2)
  virtual Double_t
  Kinematics_q2(ND::TTruthVerticesModule::TTruthVertex *vertex) const;

private:
  TClonesArray *fRooVtx;
  Int_t *fNRooVtx;
};
#endif
