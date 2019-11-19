#ifndef ConstituentsUtils_h
#define ConstituentsUtils_h

#include "DetectorDefinition.hxx"
#include "FiducialVolumeDefinition.hxx"
#include "AnalysisUtils.hxx"

/// This namespace contains useful functions for analyses. Functions include
/// those related to fiducial volumes, accessing tracks with specific
/// characteristics, and more.
namespace anaUtils{

    //----- Utility functions -------------

    /// Whether the specified position is in the volume of the given detector.
    /// Accepted detectors are kFGD1, kFGD2, kP0D, kDSECal, k(Top, Bottom, Left, Right)T(P)ECal (or SMRD)
    bool InDetVolume(SubDetId::SubDetEnum det, const Float_t* pos);

    /// Whether the specified position is in the fiducial volume of the given detector.
    /// Accepted detectors are kFGD1, kFGD2, kP0D, kDSECal, k(Top, Bottom, Left, Right)T(P)ECal (or SMRD)
    /// The fiducial volume is specified using the FVdefmin and FVdefmax vectors. These
    /// vectors are the amount of space to *exclude*, compared to the nominal side of
    /// the detector.
    bool InFiducialVolume(SubDetId::SubDetEnum det, const Float_t* pos, const Float_t* FVdefmin, const Float_t* FVdefmax);
    bool InFiducialVolume(SubDetId::SubDetEnum det, const Float_t* pos);

    /// Return the module type in which the position is
    /// water module, 1st half XY layer, 2nd half XY layer, gaps should be empty
    // to do: do it also for FGD1
    Int_t GetFgdModuleType(bool IsMC, const Float_t* pos, SubDetId::SubDetEnum det, bool includeGlueSkin = true);
    Int_t GetFgdModuleTypeNoFV(bool IsMC, const Float_t* pos, bool includeGlueSkin = true);
    Int_t GetFgdMaterial(bool IsMC, const Float_t* pos, SubDetId::SubDetEnum det);
    Int_t GetFgdMaterialNoFV(bool IsMC, const Float_t* pos);

    /// Return the detector in which the position is.
    SubDetId::SubDetEnum GetDetector(const Float_t* pos);


    Int_t GetOneSegmentPerTPC(AnaTPCParticleB* in[], Int_t nseg, AnaTPCParticleB* out[]);

    /// For tracks that start in the FGD, get the closest TPC in the direction of the track.
    SubDetId::SubDetEnum GetClosestTPC(const AnaTrackB& track);

    /// Get the vector of AnaParticle segment that uses the specified detector.
    /// See SubDetId::SubDetEnum for the detector numbering convention.
    /// Return the number of entries in the input array, the number of tracks found
    int GetSegmentsInDet(const AnaTrackB& track, SubDetId::SubDetEnum det, AnaParticleB* selTracks[]);

    /// Method to get the subtrack with most nodes in a given detector
    AnaParticleB* GetSegmentWithMostNodesInDet(const AnaTrackB& track, SubDetId::SubDetEnum det);

    /// Combined function to address NuMu selection needs as efficiently as possible - gets the TPC segment with the most nodes in the TPC closest to the start of the global track
    AnaParticleB* GetSegmentWithMostNodesInClosestTpc(const AnaTrackB& track);

    /// Same as above but closest to a given point
    AnaParticleB* GetSegmentWithMostNodesInClosestTpc(const AnaTrackB& track, const Float_t* pos, bool quality_cut = false);
    
    /// Get the AnaParticle segment that uses the specified detector.
    /// See SubDetId::SubDetEnum for the detector numbering convention.
    AnaParticleB* GetSegmentInDet(const AnaTrackB& track, SubDetId::SubDetEnum det);

    bool HasTrackUsingTPC(const AnaEventB& event);

    /// Get all the true traj. in the bunch that are charged and crosses the TPC 
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllChargedTrajInTPCInBunch(const AnaEventB& event, AnaTrueParticleB* traj[]);
    /// Get all the true traj. in the bunch that are charged and crosses the FGD
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllChargedTrajInFGDInBunch(const AnaEventB& event, AnaTrueParticleB* traj[],SubDetId::SubDetEnum det);
    /// Get all the true traj. in the bunch that are charged and crosses the TPC with a length > 1/4 of the TPC 
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllBigEnoughChargedTrajInTPCInBunch(const AnaEventB& event, AnaTrueParticleB* chargedtrajInBunch[]);
    /// Get all the true traj. in the bunch that are charged and crosses the TPC and the FGD (TPC1-FGD1, FGD1-TPC2, TPC2-FGD2, FGD2-TPC3)
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllChargedTrajInTPCFGDInBunch(const AnaEventB& event, AnaTrueParticleB* chargedtrajInBunch[]);
    /// Get all the true traj. in the bunch that are charged and crosses the the FGD but not the TPC
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllChargedTrajInFGDAndNoTPCInBunch(const AnaEventB& event, AnaTrueParticleB* chargedtrajInBunch[],SubDetId::SubDetEnum det);

    int GetAllChargedTrajInP0DInBunch(const AnaEventB& event, AnaTrueParticleB* chargedTrajInBunch[]);
    int GetAllChargedTrajInP0DAndTPCInBunch(const AnaEventB& event, AnaTrueParticleB* chargedTrajInBunch[]);
    int GetAllChargedTrajInP0DAndNoTPCInBunch(const AnaEventB& event, AnaTrueParticleB* chargedTrajInBunch[]);

    /// Access function to get all the tracks in the bunch that use the P0D, sorted by decreasing momentum.
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllTracksUsingP0D(const AnaEventB& event, AnaTrackB* selTracks[]);

    /// Access function to get all the tracks in the bunch that use the FGD, sorted by decreasing momentum.
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllTracksUsingFGD(const AnaEventB& event, AnaTrackB* selTracks[]);

    /// Access function to get all the tracks in the bunch that use the TPC, sorted by decreasing momentum.
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllTracksUsingTPC(const AnaEventB& event, AnaTrackB* selTracks[]);

    /// Access function to get all the tracks in the bunch that use the TPC or the FGD, sorted by decreasing number of hits
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllTracksUsingFGDorTPC(const AnaEventB& event, AnaTrackB* selTracks[]);

    /// Access function to get all the tracks in the bunch that use the ECAL, sorted by decreasing momentum.
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllTracksUsingECAL(const AnaEventB& event, AnaTrackB* selTracks[]);
    
    /// Access function to get all the tracks in the bunch that use the ECAL, sorted by decreasing momentum.
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllIsoTracksInECAL(const AnaEventB& event, AnaTrackB* selTracks[]);

    /// Access function to get all the tracks in the bunch that use the SMRD, sorted by decreasing momentum.
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllTracksUsingSMRD(const AnaEventB& event, AnaTrackB* selTracks[]);

    /// Access function to get all the tracks in the bunch that use the FGD and no TPC, sorted by decreasing NHits.
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllTracksUsingFGDAndNoTPC(const AnaEventB& event, AnaTrackB* selTracks[],SubDetId::SubDetEnum fgddet);

    /// Returns the number of tracks using both the TPC and the subdetector 'det'.
    int GetNTracksUsingTPCAndDet(const AnaEventB& event, SubDetId::SubDetEnum det);


  /// Whether this track has an ecal segment in the TrackerECal or DsECal.
  bool UsesTrackerDsEcal(AnaTrackB* track);

  /// Whether this ecal segment is in the TrackerECal or DsECal (i.e. not in the P0DECal).
  bool UsesTrackerDsEcal(AnaECALParticleB* ecal);
     
  /// Get the Tracker ECal and DsECal segments from the given track.
  /// Returns the number of entries in the input array, the number of tracks found
  int GetTrackerDsEcals(AnaTrackB* track, AnaECALParticleB* selTracks[]);

  /// Get the most upstream ECal component of a track, based on either the start or
  /// end position. Fill the pos variable with the position used. Return NULL if not found.
  AnaECALParticleB* GetMostUpstreamECalSegment(AnaTrackB* track);

  /// Whether the ECal object is "contained" within the ECal volume. This is
  /// intended for deciding whether to trust the ECal EM energy or not.
    /// At the moment this is based on the start position of the shower. It will
    /// be updated to a more rigorous definition when production 5F files have been
    /// studied more closely.
    //  bool IsEcalContained(AnaEcalTrackEcalPID* EcalSegment);

    /// Return an integer corresponding to the array index of the track in the old local detector enumeration
    int GetLocalDetEnum(SubDetId::SubDetEnum det, int i);


    // enum used for GetMassComponent and GetNTargets etc
    enum massComponentEnum { kFGD1=1, kFGD2=2, kFGDs=3, kFGD2xymodules=4, kFGD2watermodules=5, kP0Dwater=6, kP0Dair=7, kFGD2waterlike=8, kFGD2xylike=9, kInvalid=-1 };

    /// Get the detector component in which the position is
    massComponentEnum GetMassComponent(bool IsMC, const Float_t* pos);


}
#endif

//  LocalWords:  ifndef
