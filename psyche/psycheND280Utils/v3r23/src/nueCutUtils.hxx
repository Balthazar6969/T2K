#ifndef nueCutUtils_h
#define nueCutUtils_h

#include "ND280AnalysisUtils.hxx"
#include "BaseDataClasses.hxx"
#include "EventBoxTracker.hxx"

namespace nueCutUtils{

  /// Minimum number of TPC hits
  bool TrackQuality(AnaTrackB* track, int _num_tpc_nodes, int _num_tpc_nodes_ecal);

  /// TPC Electron Pull
  bool TPCElectronPull(AnaTrackB* track, Float_t pullel_accept_min, Float_t pullel_accept_max, Float_t pullel_accept_tight_min, Float_t pullel_accept_tight_max, Float_t minMom_ecal);

  /// TPC Muon Pull
  bool TPCMuonPull(AnaTrackB* track, Float_t pullmu_reject_min, Float_t pullmu_reject_max, Float_t minMom_ecal);

  /// TPC Pion Pull
  bool TPCPionPull(AnaTrackB* track, Float_t pullpi_reject_min, Float_t pullpi_reject_max, Int_t num_tpc_nodes, Float_t minMom_ecal);

  /// Ecal EM Energy PID
  bool EcalEMEnergy(AnaTrackB* track, Float_t Ethreshold, Float_t Emin_ecal, Float_t eoverp, bool AntiNu);

  /// Ecal mip-shower discriminator
  bool EcalMIPEMPID(AnaTrackB* track, Float_t Ethreshold, Float_t ecal_mipem_min);

  /// Ecal em-had shower discriminator
  bool EcalEMHIPPID(AnaTrackB* track, Float_t protonregion_low, Float_t emhip);
  
  /// E/P cut for nue analysis
  bool EOverPNuE(AnaTrackB* track, Float_t eop_threshold, Float_t eop_minmom);

  /// E/P cut for antinue analysis
  bool EOverP(AnaTrackB* track, Float_t protonregion_low, Float_t eoverp);

  /// Second TPC PID
  bool SecondTPCPID(AnaTrackB* track, Float_t pullmu_reject_min, Float_t pullmu_reject_max);

  bool AntiNuSecondTPCPID(AnaTrackB* track, Float_t protonregion_low, Float_t protonregion_high, Float_t pullel_reject_min, Float_t pullel_reject_max);

  /// e-e+ pair cut
  bool PairCut(const AnaTrackB& track, AnaTrackB* pair, Float_t inv_mass_min);

  /// TPC Veto cut
  bool TPCVeto(const AnaTrackB& track, AnaTrackB* veto, Float_t delta_z_min);
  
  /// Ecal Veto cut
  bool EcalVeto(const AnaTrackB& track, AnaTrackB* veto, Float_t delta_z_min);

  /// ToF cut
  bool ToF(const AnaTrackB& track, Float_t fgdecaltof, Int_t RunNum);

  bool AntiNuToF(const AnaTrackB& track, Float_t fgdecaltof, Float_t fgd1fgd2tof, Float_t fgd1fgd2tof_relaxed, Float_t protonregion_low, Int_t RunNum);

  /// FGD2 shower cut for events NOT going in the Ecal
  bool FGD2Shower(AnaTrackB* track, Float_t protonregion_low, Float_t protonregion_high, Int_t nFGD1TPC2, Int_t nFGD2TPC3);

  /// FGD2 shower cut for events going in the Ecal
  bool FGD2ShowerActivityEcal(AnaTrackB* track, Float_t protonregion_low, Int_t nFGD1TPC2, Int_t nFGD2TPC3);
  
  /// FGD proton PID cut for anue
  bool FGDProtonPull(AnaTrackB* track, SubDetId::SubDetEnum det, Float_t protonpull);
}

#endif
