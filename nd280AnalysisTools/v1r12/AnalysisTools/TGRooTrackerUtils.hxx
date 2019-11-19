#ifndef TGRooTrackerUtils_h_seen
#define TGRooTrackerUtils_h_seen
//
/// oaAnalysis read-out utility in macros/AnalysisTools,
/// which helps understand the GRooTrackerVtx pass-through info. This code
/// is mainly a copy of the GenieUtils in the T2KReWeight package.
///
/// This can be queried to get details such as the numbers of pi+ before
/// and after intranuclear reinteractions.
///

#include <string>

#include "libReadoaAnalysis/libReadoaAnalysisProjectHeaders.h"

class TGRooTrackerUtils {

public:
  TGRooTrackerUtils(){};
  virtual ~TGRooTrackerUtils(){};

  bool IsCC(const ND::GRooTrackerVtx &evt); ///< is it a CC event?
  bool IsNC(const ND::GRooTrackerVtx &evt); ///< is it a NC event?
  bool IsQELCC(const ND::GRooTrackerVtx
                   &evt); ///< is it a v - (free or bound) nucleon QEL CC event
  bool IsELNC(const ND::GRooTrackerVtx &evt); ///< is it a v - (free or bound)
                                              ///nucleon NC elastic event
  bool IsRES(const ND::GRooTrackerVtx &evt);  ///< is it a v - (free or bound)
                                              ///nucleon RES (CC+NC) event?
  bool IsRESCC(const ND::GRooTrackerVtx
                   &evt); ///< is it a v - (free or bound) nucleon RES CC event?
  bool IsRESNC(const ND::GRooTrackerVtx
                   &evt); ///< is it a v - (free or bound) nucleon RES NC event?
  bool IsDIS(const ND::GRooTrackerVtx
                 &evt); ///< is it a v - (free or bound) nucleon DIS event?
  bool IsDISCC(const ND::GRooTrackerVtx
                   &evt); ///< is it a v - (free or bound) nucleon DIS CC event?
  bool IsDISNC(const ND::GRooTrackerVtx &evt); ///< is it a v - (free or bound)
                                               ///nucleon(free or bound) DIS NC
                                               ///event?
  bool IsCOH(const ND::GRooTrackerVtx &evt); ///< is it a v - nucleus COH event
                                             ///(CC+NC, COH elastic + 1pi)?
  bool IsCOHPi(const ND::GRooTrackerVtx
                   &evt); ///< is it a v - nucleus COH pi event (CC+NC)?
  bool IsCOHPiCC(
      const ND::GRooTrackerVtx &evt); ///< is it a v - nucleus COH CC pi event?
  bool IsCOHPiNC(
      const ND::GRooTrackerVtx &evt); ///< is it a v - nucleus COH NC pi event?
  bool IsAMNuGamma(const ND::GRooTrackerVtx &evt);   ///<
  bool IsNuEEL(const ND::GRooTrackerVtx &evt);       ///<
  bool IsIMD(const ND::GRooTrackerVtx &evt);         ///<
  bool IsCharmProd(const ND::GRooTrackerVtx &evt);   ///<
  bool IsQELCharm(const ND::GRooTrackerVtx &evt);    ///<
  bool IsDISCharm(const ND::GRooTrackerVtx &evt);    ///<
  bool NucIsHit(const ND::GRooTrackerVtx &evt);      ///<
  bool QuarkIsHit(const ND::GRooTrackerVtx &evt);    ///<
  bool SeaQuarkIsHit(const ND::GRooTrackerVtx &evt); ///<
  bool ValQuarkIsHit(const ND::GRooTrackerVtx &evt); ///<
  double KineXbj(const ND::GRooTrackerVtx &evt);     ///< bjorken x
  double KineNu(const ND::GRooTrackerVtx
                    &evt); ///< energy transfer to the nucleus v (GeV)
  double KineY(const ND::GRooTrackerVtx &evt);   ///< inelasticity y
  double KineTau(const ND::GRooTrackerVtx &evt); ///< t, for COH events only
  double
  KineW(const ND::GRooTrackerVtx &evt); ///< hadronic invariant mass (GeV)
  double KineQ2(const ND::GRooTrackerVtx &evt); ///< momentum transfer (GeV^2)
  int PdgNu(const ND::GRooTrackerVtx &evt); ///< initial state neutrino pdg code
  int PdgFsl(
      const ND::GRooTrackerVtx &evt); ///< final state primary lepton pdg code
  int PdgTgt(const ND::GRooTrackerVtx &evt);    ///< (nuclear) target pdg code
  int PdgHitNuc(const ND::GRooTrackerVtx &evt); ///< hit nucleon pdg code
  int TgtZ(const ND::GRooTrackerVtx &evt);      ///< target atomic number Z
  int TgtA(const ND::GRooTrackerVtx &evt);      ///< target mass number A
  int ResId(const ND::GRooTrackerVtx &evt);     ///< resonance id
  int NParticles(const ND::GRooTrackerVtx
                     &evt); ///< number of particles at the event record
  bool IsFinStProton(const ND::GRooTrackerVtx &evt,
                     int i); ///< Is entry i a final state proton
  bool IsFinStNeutron(const ND::GRooTrackerVtx &evt,
                      int i); ///< Is entry i a final state neutron
  bool IsFinStPi0(const ND::GRooTrackerVtx &evt,
                  int i); ///< Is entry i a final state pi0
  bool IsFinStPip(const ND::GRooTrackerVtx &evt,
                  int i); ///< Is entry i a final state pi+
  bool IsFinStPim(const ND::GRooTrackerVtx &evt,
                  int i); ///< Is entry i a final state pi-
  bool IsFinStK0(const ND::GRooTrackerVtx &evt,
                 int i); ///< Is entry i a final state K0+K0bar
  bool IsFinStKp(const ND::GRooTrackerVtx &evt,
                 int i); ///< Is entry i a final state K+
  bool IsFinStKm(const ND::GRooTrackerVtx &evt,
                 int i); ///< Is entry i a final state K-
  bool IsPrimProton(const ND::GRooTrackerVtx &evt,
                    int i); ///< Is entry i a primary state proton?
  bool IsPrimNeutron(const ND::GRooTrackerVtx &evt,
                     int i); ///< Is entry i a primary state neutron?
  bool IsPrimPi0(const ND::GRooTrackerVtx &evt,
                 int i); ///< Is entry i a primary state pi0?
  bool IsPrimPip(const ND::GRooTrackerVtx &evt,
                 int i); ///< Is entry i a primary state pi+?
  bool IsPrimPim(const ND::GRooTrackerVtx &evt,
                 int i); ///< Is entry i a primary state pi-?
  bool IsPrimK0(const ND::GRooTrackerVtx &evt,
                int i); ///< Is entry i a primary state K0+K0bar?
  bool IsPrimKp(const ND::GRooTrackerVtx &evt,
                int i); ///< Is entry i a primary state K+?
  bool IsPrimKm(const ND::GRooTrackerVtx &evt,
                int i); ///< Is entry i a primary state K-?
  int NPrimProton(const ND::GRooTrackerVtx &evt); ///< n of protons  - in
                                                  ///primary hadronic system
                                                  ///(before intranuclear hadron
                                                  ///transport)
  int NPrimNeutron(const ND::GRooTrackerVtx &evt); ///< n of neutrons - in
                                                   ///primary hadronic system
                                                   ///(before intranuclear
                                                   ///hadron transport)
  int NPrimPi0(const ND::GRooTrackerVtx &evt); ///< n of pi0      - in primary
                                               ///hadronic system (before
                                               ///intranuclear hadron transport)
  int NPrimPip(const ND::GRooTrackerVtx &evt); ///< n of pi+      - in primary
                                               ///hadronic system (before
                                               ///intranuclear hadron transport)
  int NPrimPim(const ND::GRooTrackerVtx &evt); ///< n of pi-      - in primary
                                               ///hadronic system (before
                                               ///intranuclear hadron transport)
  int NPrimK0(const ND::GRooTrackerVtx &evt);  ///< n of K0+K0bar - in primary
                                               ///hadronic system (before
                                               ///intranuclear hadron transport)
  int NPrimKp(const ND::GRooTrackerVtx &evt);  ///< n of K+       - in primary
                                               ///hadronic system (before
                                               ///intranuclear hadron transport)
  int NPrimKm(const ND::GRooTrackerVtx &evt);  ///< n of K-       - in primary
                                               ///hadronic system (before
                                               ///intranuclear hadron transport)
  int NFinStProton(const ND::GRooTrackerVtx &evt); ///< n of protons  - in final
                                                   ///state (after intranuclear
                                                   ///hadron transport)
  int NFinStNeutron(const ND::GRooTrackerVtx &evt); ///< n of neutrons - in
                                                    ///final state (after
                                                    ///intranuclear hadron
                                                    ///transport)
  int NFinStPi0(const ND::GRooTrackerVtx &evt); ///< n of pi0      - in final
                                                ///state (after intranuclear
                                                ///hadron transport)
  int NFinStPip(const ND::GRooTrackerVtx &evt); ///< n of pi+      - in final
                                                ///state (after intranuclear
                                                ///hadron transport)
  int NFinStPim(const ND::GRooTrackerVtx &evt); ///< n of pi-      - in final
                                                ///state (after intranuclear
                                                ///hadron transport)
  int NFinStK0(const ND::GRooTrackerVtx &evt);  ///< n of K0+K0bar - in final
                                                ///state (after intranuclear
                                                ///hadron transport)
  int NFinStKp(const ND::GRooTrackerVtx &evt);  ///< n of K+       - in final
                                                ///state (after intranuclear
                                                ///hadron transport)
  int NFinStKm(const ND::GRooTrackerVtx &evt);  ///< n of K-       - in final
                                                ///state (after intranuclear
                                                ///hadron transport)
  double ENu(const ND::GRooTrackerVtx
                 &evt); ///< incoming neutrino energy (GeV, at the LAB)
  double ENu_nrf(const ND::GRooTrackerVtx &evt);  ///< incoming neutrino energy
                                                  ///(GeV, at the nucleon rest
                                                  ///frame)
  double ENu_reco(const ND::GRooTrackerVtx &evt); ///< incoming neutrino energy
                                                  ///(GeV, 'reconstructed-like'
                                                  ///energy from the ~QEL
                                                  ///formula)
  double EFsl(const ND::GRooTrackerVtx &
                  evt); ///< final state primary lepton energy (GeV, at the LAB)
  double MomentumFsl(const ND::GRooTrackerVtx &evt); ///< final state primary
                                                     ///lepton |momentum| (GeV,
                                                     ///at the LAB)
  double Momentum(const ND::GRooTrackerVtx &evt,
                  int i); ///< ith stdhep entry |momentum| (GeV, at the LAB)
  double E(const ND::GRooTrackerVtx &evt,
           int i); ///< ith stdhep entry kinetic energy (GeV)
  double KE(const ND::GRooTrackerVtx &evt,
            int i); ///< ith stdhep entry kinetic energy (GeV)
  double CosTheta(const ND::GRooTrackerVtx &evt, int i); ///< ith stdhep entry
                                                         ///cos(scattering angle
                                                         ///in respect to the
                                                         ///neutrino direction)
  double FermiMomentum(const ND::GRooTrackerVtx &evt); ///< hit nucleon (fermi)
                                                       ///|momentum| (GeV, at
                                                       ///the LAB)
  int IdxNu(const ND::GRooTrackerVtx
                &evt); ///< incoming neutrino index in particle array
  int IdxTgt(const ND::GRooTrackerVtx
                 &evt); ///< (nuclear) target index in particle array
  int IdxFsl(const ND::GRooTrackerVtx
                 &evt); ///< final state primary lepton index in particle array
  int IdxHitNuc(
      const ND::GRooTrackerVtx &evt); ///< hit nucleon index in particle array
  int IdxPi0(const ND::GRooTrackerVtx &evt);      ///<
  int IdxPi0_prim(const ND::GRooTrackerVtx &evt); ///<
  void Print(const ND::GRooTrackerVtx &evt);      ///<

};

#endif
