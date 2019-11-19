#ifndef MomResolMCCorrection_h
#define MomResolMCCorrection_h

#include "CorrectionBase.hxx"
#include "BaseDataClasses.hxx"
#include "MomentumResolVariation.hxx"
#include "ToyExperiment.hxx"


/// This class defines a correction that affects the reconstructed TPC Momentum  of an 
/// AnaTpcSegment as well as the global (AnaTrack) track Momentum. It basically applies an additional smearing.
/// This correction is applied to MC only

class MomResolMCCorrection: public MomentumResolVariation, public CorrectionBase {

public:
  
  MomResolMCCorrection();

  virtual ~MomResolMCCorrection() {}

  /// Apply the momentum correction to all the relevant objects: AnaTrack and corresponding 
  void Apply(AnaSpillC& spill);
 
  /// Get the variation given a track
  bool GetVariation(AnaTrackB* track, Float_t& variation);
 
  /// Get the variation given a track
  bool GetVariationTPC(AnaTPCParticleB* track, Float_t& variation); 
  
  /// Get the variation given a track
  bool GetVariation(AnaTrackB* track, Float_t& variation, const ToyExperiment& exp){
    (void)exp;
    return GetVariation(track, variation);
  }
 
  /// Get the variation given a track
  bool GetVariationTPC(AnaTPCParticleB* track, Float_t& variation, const ToyExperiment& exp){
    (void)exp;
    return GetVariationTPC(track, variation);
  }
 

protected:
  
  /// Check whether a track is relevant to apply the correction: e.g. momentum resolution was studied for tracker objects only 
  /// Probably should be moved to a base class
  bool IsRelevantObject(const AnaRecObjectC* object);
 
  /// Some default toy experiment,  not used at the moment
  ToyExperiment _exp;
  
private:
  bool _tpc_based_var;
};

#endif
