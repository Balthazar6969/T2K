#ifndef BoxUtils_h
#define BoxUtils_h

#include "ND280AnalysisUtils.hxx"
#include "BaseDataClasses.hxx"
#include "EventBoxId.hxx"

namespace boxUtils{
  
  /// Fill in the EventBox several arrays of tracks with TPC
  void FillTracksWithTPC(AnaEventB& event, SubDetId::SubDetEnum det = SubDetId::kFGD);

  /// Fill in the EventBox the array of tracks using FGD
  void FillTracksWithFGD(AnaEventB& event, SubDetId::SubDetEnum det = SubDetId::kFGD);

  /// Fill in the EventBox the array of tracks using ECals
  void FillTracksWithECal(AnaEventB& event);

  /// Fill in the EventBox the array of tracks using the P0D
  void FillTracksWithP0D(AnaEventB& event);

  /// Fill in the EventBox the array of true tracks passing through TPC
  void FillTrajsChargedInTPC(AnaEventB& event);

  /// Fill in the EventBox the array of true tracks passing through FGD and no TPC
  void FillTrajsChargedInFGDAndNoTPC(AnaEventB& event, SubDetId::SubDetEnum det = SubDetId::kFGD);

  /// Fill in the EventBox the array of true tracks passing through FGD or TPC
  void FillTrajsChargedInTPCorFGD(AnaEventB& event, SubDetId::SubDetEnum det = SubDetId::kFGD);

  /// Fill in the EventBox the arrays of true tracks relevant for high-angle business: FGD-ECal and FGD-ECal-SMRD
  void FillTrajsChargedHATracker(AnaEventB& event, SubDetId::SubDetEnum det = SubDetId::kFGD);

  /// Fill in the EventBox the arrays of true tracks relevant for ECal systematics
  void FillTrajsInECal(AnaEventB& event);

  /// Fill in the EventBox the arrays of true tracks relevant for P0D systematics
  void FillTrajsChargedInP0D(AnaEventB& event);
    
  /// Fill in the EventBox Michel Electron candidates
  void FillFGDMichelElectrons(AnaEventB& event, SubDetId::SubDetEnum det = SubDetId::kFGD, bool prod5Cut = false);


}

#endif
