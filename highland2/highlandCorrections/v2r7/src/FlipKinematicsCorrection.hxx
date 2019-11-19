#ifndef FlipKinematicsCorrection_h
#define FlipKinematicsCorrection_h

#include "CorrectionBase.hxx"
#include "DataClasses.hxx"
#include "SubDetId.hxx"

/*! 
 * Each track that has a TPC component present together with a segment in other
 * detector should have an "alternate" re-fit with a reversed charge and direction
 * Sometimes it happens that this reverse fit fails on oaRecon level, hence the info
 * is not available, this correction "fixes" the problem by taking the closest
 * (to the end point) TPC momentum and applying a "reversed" dEdX correction up to the 
 * end point
 */ 

class FlipKinematicsCorrection: public CorrectionBase {
public:

  FlipKinematicsCorrection(SubDetId::SubDetEnum det = SubDetId::kFGD);
  virtual ~FlipKinematicsCorrection(){}

  void Apply(AnaSpillC& spill); 

protected:

  /// A correction is supposed to be relevant for the tracks that 
  /// have a potential to be reversed (should have a TOF) and 
  /// for which B field + eloss momentum estimation is crucial: TPC should be present 
  bool IsRelevantTrack(const AnaTrackB& track) const;


  /// A detector bit that defines which ToFs should be present to consider
  /// the object for correction
  SubDetId::SubDetEnum _det;

};

#endif
