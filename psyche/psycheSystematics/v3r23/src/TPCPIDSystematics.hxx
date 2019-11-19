#ifndef TPCPIDSystematics_h
#define TPCPIDSystematics_h

#include "EventVariationBase.hxx"
#include "TPCPIDVariation.hxx"

/// This systematic smears the CT of each TPC track segment
class TPCPIDSystematics: public TPCPIDVariation, public EventVariationBase{
public:
  
  /// Instantiate the PID systematic. nbins is the number of
  /// bins in the PDF
  TPCPIDSystematics();
  
  virtual ~TPCPIDSystematics(){} 
  
  /// Apply the systematic
  virtual void Apply(const ToyExperiment& toy, AnaEventC& event);
  
  /// Undo  the systematic variations done by ApplyVariation. This is faster tha reseting the full Spill
  bool UndoSystematic(AnaEventC& event);
  
  /// Get the variation for a given TPC object
  bool GetVariation(const AnaTPCParticleB& tpcTrack, 
      Float_t& mean_var, Float_t& sigma_var, 
      const AnaTrackB& track, const ToyExperiment& toy);

protected:

  /// Is this track relevant for this systematic ?
  bool IsRelevantRecObject(const AnaEventC& event, const AnaRecObjectC& track) const;

  Int_t _offset;

  /// value of psycheSystematics.Tracker.FullCorrelations parameter
  bool _full_correlations;

};

#endif
