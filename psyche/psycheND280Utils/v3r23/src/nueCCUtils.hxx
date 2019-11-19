#ifndef nueCCUtils_h
#define nueCCUtils_h

#include "ND280AnalysisUtils.hxx"
#include "BaseDataClasses.hxx"
#include "EventBoxTracker.hxx"

namespace nueCCUtils{

  TLorentzVector GetMomLorentzVector(const AnaTrackB& track, Float_t mass);

  /// Check if Ecal cluster is contained
  bool IsEcalContained(AnaECALParticleB* ecal);

  /// Check whatever the Ecal should be used for analysis
  bool UseEcal(AnaTrackB* track);

  /// Use either the Ecal EMEnergy PID or the MIPEM
  bool UseEcalEMEnergyPID(AnaECALParticleB* EcalSegment, Float_t EMEnergy_thr, Float_t TPCBackMom);

  /// Find the best e-e+ pair
  AnaTrackB* FindPairTrack(const AnaEventB& event, const AnaTrackB& track, Float_t delta_pos_max, Float_t pullel_min, Float_t pullel_max, bool same_charge=false);

  /// P0D-P0DEcal-FGD1 veto tracks
  void FindP0DVetoTracks(const AnaEventB& event, const AnaTrackB& track, Int_t nTracksx[3]);

  /// Find TPC veto track
  AnaTrackB* FindTPCVetoTrack(const AnaEventB& event, const AnaTrackB& track);

  /// Find gamma isolation tracks
  void FindGammaIsoTracks(const AnaEventB& event, const AnaTrackB& track, SubDetId::SubDetEnum det, Float_t delta_z_min, Int_t nTracksx[3]);

  /// Find Ecal veto track. Last optional argument is to search for ecal neutral clusters only
  AnaTrackB* FindEcalVetoTrack(const AnaEventB& event, const AnaTrackB& track, bool nc = false);

  /// Find number of FGD1-TPC2 and FGD2-TPC3 tracks
  void FindFGD2ShowerTracks(const AnaEventB& event, const AnaTrackB& track, Int_t nTracksx[2]);

  /// Gamma Invariant mass calculation 
  Float_t GetInvMass(const AnaTrackB & primary,  const AnaTrackB &secondary);

  /// Find second most energetic track in all TPCs
  AnaTrackB* FindSecondTPCTrack(const AnaEventB& event, const AnaTrackB& primary, SubDetId::SubDetEnum det);

  /// Find second most energetic FGD-TPC track in the same FGD as the main track
  AnaTrackB* FindFGDTPCVetoTrack(const AnaEventB& event, const AnaTrackB& track, SubDetId::SubDetEnum det);

  /// Find the most upstream Ecal neutral cluster
  AnaECALParticleB* GetMostUpstreamECalNCSegment(AnaTrackB* track);

  /// Find Leading track in FGD
  AnaTrackB* FindLeadingTrackInDet(const AnaEventB& event, SubDetId::SubDetEnum det);

  /// Find OOFV tracks
  Int_t FindOOFVFGDTPCTracks(const AnaEventB& event, const AnaTrackB& track, SubDetId::SubDetEnum det);

}


#endif
