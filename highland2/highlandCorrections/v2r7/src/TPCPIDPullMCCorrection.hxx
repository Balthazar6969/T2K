#ifndef TPCPIDPullMCCorrection_h
#define TPCPIDPullMCCorrection_h

#include "CorrectionBase.hxx"
#include "TPCPIDVariation.hxx"
#include "ToyExperiment.hxx"


/// This class defines a correction that affects the reconstructed TPC Momentum  of an 

class TPCPIDPullMCCorrection: public TPCPIDVariation, public CorrectionBase {

public:
  
  TPCPIDPullMCCorrection();

  virtual ~TPCPIDPullMCCorrection() {}

  /// Apply the correction to all the relevant objects: TPC constituent of an AnaTrack 
  void Apply(AnaSpillC& spill);

  /// Get the variation for a given TPC object
  bool GetVariation(const AnaTPCParticleB& tpcTrack, 
      Float_t& mean_var, Float_t& sigma_var, 
      const AnaTrackB& track);
  
  /// Get the variation for a given TPC object
  bool GetVariation(const AnaTPCParticleB& tpcTrack, 
      Float_t& mean_var, Float_t& sigma_var, 
      const AnaTrackB& track, const ToyExperiment& exp){
    (void)exp;
    return GetVariation(tpcTrack, mean_var, sigma_var, track); 
  }
  

protected:
  
  /// Check whether a track is relevant to apply the correction: e.g. momentum resolution was studied for tracker objects only 
  /// Probably should be moved to a base class
  bool IsRelevantObject(const AnaRecObjectC* object) {(void)object; return true;} // all considered for the moment
  
  
  /// Some default toy experiment,  not used at the moment
  ToyExperiment _exp;
  
};

#endif
