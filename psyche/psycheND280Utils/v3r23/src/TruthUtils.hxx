#ifndef TruthUtils_h
#define TruthUtils_h

#include "AnalysisUtils.hxx"

/// This namespace contains useful functions for analyses. Functions include
/// those related to fiducial volumes, accessing tracks with specific
/// characteristics, and more.
namespace anaUtils{

    /// Get the number of true michel electrons
    Int_t GetNMichelElectrons(const AnaTrueVertexB& trueVertex, SubDetId::SubDetEnum det = SubDetId::kFGD1);

    /// Get the number of true michel electrons, alternative version. This one looks for electrons with muon parents where the electron time is greater than or equal to the muon end time. At most one Michel per parent is allowed.
    Int_t GetNMichelElectronsAlt(const AnaTrueVertexB& trueVertex, SubDetId::SubDetEnum det = SubDetId::kFGD1);

    /// Return the true linear length traversed in the TPC
    Float_t GetTrueLinearLengthInTPC(const AnaTrueParticleB& trueTrack, Float_t& distz);

    /// Get all the true traj. in the bunch that are charged and cross the FGD and ECal (Barrel) and no TPC
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllChargedTrajsInFgdECalInBunch(const AnaEventB& event, AnaTrueParticleB* outTraj[], SubDetId::SubDetEnum det);
   
    /// Get all true tajs that cross ECal(s) (Barrel)
    /// Returns the number of entries in the input array, the number of tracks found
    int GetAllTrajsInECalInBunch(const AnaEventB& event, AnaTrueParticleB* outTraj[]);
   
     
    /// Whether a true track crosses a SMRD so to be able to deposit charge in at least one layer
    /// For the moment consider the minimum separation in the direction perpendicular to the beam as:
    /// 48 mm (iron) mm (we are intersted for tracks that come from inside,  so should cross the first iron block)
    /// as it comes from oaAnalysis only one point is saved for SMRD,  as I understand the code it should be the exit point
    /// so make a cut in X/Y distance between the point and particular surface inner border  
    /// Note: this assumes track going from inside
    bool TrueParticleCrossesSMRD(const AnaTrueParticleB*  track, Float_t min_sep = 48.);
    
    /// Whether a true track crosses a TECal so to be able to reconstruct an object
    /// For the moment consider the minimum separation as four layers in the direction perpendicular
    /// to the beam (dealing with BarrelECal here)
    /// Note: this assumes track going from inside
    bool TrueParticleCrossesTECal(const AnaTrueParticleB* track, Float_t min_sep = 50.);
    
    /// Whether a true track crosses a FGD so to be able to deposit charge in at least two layers
    bool TrueParticleCrossesFGD(const AnaTrueParticleB*   track, SubDetId::SubDetEnum det = SubDetId::kFGD);
    
    /// Whether a true track crosses a TPC so to be able to reconstruct an object 
    bool TrueParticleCrossesTPC(const AnaTrueParticleB*   track, SubDetId::SubDetEnum det = SubDetId::kTPC);

    /// Whether a true track enters a given sub-detector 
    bool TrueParticleEntersDet(const AnaTrueParticleB*   track, SubDetId::SubDetEnum det);
    
    /// SMRD detectors crossed
    int GetSMRDDetCrossed(const AnaTrueParticleB*      track, SubDetId::SubDetEnum det[]);

    /// ECal detectors crossed: split based on geom info PECal and TECal sub-detectors, this may not be very accurate 
    int GetECalDetCrossed(const AnaTrueParticleB*     track, SubDetId::SubDetEnum det[]);

    /// FGD detectors crossed
    int GetFGDDetCrossed(const AnaTrueParticleB*       track, SubDetId::SubDetEnum det[]);   
       
    /// Check whether an AnaDetCrossing uses a particular sub-detector 
    /// Consider special treatment for SMRD, PECal and BarrelECal since the
    /// original detector bits from oaAnalysis do not split between the
    /// sub-modules
    bool GetDetectorUsed(const AnaDetCrossingB& cross, SubDetId::SubDetEnum det, bool check_active);
    
    /// Get an AnaDetCrossing corresponding to a given detector
    /// the first one found will be returned
    AnaDetCrossingB* GetAnaDetCrossing(const AnaTrueParticleB* track, SubDetId::SubDetEnum det, bool check_active = true);
 
    /// Get the momentum value from AnaDetCrossing
    Float_t GetEntranceMomentum(const AnaDetCrossingB& cross);
    
    /// Get the momentum value from AnaDetCrossing
    Float_t GetExitMomentum(const AnaDetCrossingB& cross);
     
    /// Check whether a true object has a relation to the reco one
    /// Not all the generations are considered
    /// Only: object istelf, parent, grant parent or primary for the truth
    /// object the corresponds to a given reco: this is what "Simple" stays for
    /// Proper treatment (full) may involve lots of looping
    bool CheckTrueCausesRecoSimple(const AnaTrueParticleB& trueTrack, const AnaParticleB& track);

    /// Check whether a true object has a relation to the reco one, 
    /// The corresponding generation (0 is direct contributor) will be returned
    /// as well
    bool CheckTrueCausesRecoFull(const AnaEventB& event, const AnaTrueParticleB& trueTrack, const AnaParticleB& track, int& gen);
}
#endif
