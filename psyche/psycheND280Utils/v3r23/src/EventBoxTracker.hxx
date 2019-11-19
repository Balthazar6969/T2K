#ifndef EventBoxTracker_hxx
#define EventBoxTracker_hxx

#include "BaseDataClasses.hxx"


class EventBoxTracker:public EventBoxB{
 public :

  enum RecObjectGroupEnum{
    kTracksUnassigned=0,
    kTracksWithTPC,
    kTracksWithTPCInFGD1FV,
    kTracksWithTPCInFGD2FV,
    kTracksWithTPCWithStartOrEndInFGD1FV,
    kTracksWithTPCWithStartOrEndInFGD2FV,
    kTracksWithGoodQualityTPCInFGD1FV,
    kTracksWithGoodQualityTPCInFGD2FV,
    kTracksWithGoodQualityTPCWithStartOrEndInFGD1FV,
    kTracksWithGoodQualityTPCWithStartOrEndInFGD2FV,
    kTracksWithTPCAndFGD1,
    kTracksWithTPCAndFGD2,
    kTracksWithFGD1AndNoTPC,
    kTracksWithFGD2AndNoTPC,
    kTracksWithFGD1,
    kTracksWithFGD2,
    kTracksWithFGD1InFGD1FV,
    kTracksWithFGD2InFGD2FV,
    kTracksWithFGD1WithStartOrEndInFGD1FV,
    kTracksWithFGD2WithStartOrEndInFGD2FV,
    kTracksWithFGD1AndTECALInFGD1FV,
    kTracksWithFGD2AndTECALInFGD2FV,
    kTracksWithFGD1AndTECALWithStartOrEndInFGD1FV,
    kTracksWithFGD2AndTECALWithStartOrEndInFGD2FV,
    kTracksWithTPCorFGD1,
    kTracksWithTPCorFGD2, 
    //tmp
    kTracksWithECal,
    kIsoTracksInECal, 
    kTracksWithP0D,
    kTracksWithTPCInP0DFV,
    kTracksWithGoodQualityTPCInP0DFV,
    kTracksWithP0DAndNoTPC
  };
  
  enum TrueObjectGroupEnum{
    kTrueParticlesUnassigned=0,
    kTrueParticlesChargedInTPCInBunch,
    kTrueParticlesChargedInFGD1AndNoTPCInBunch,
    kTrueParticlesChargedInFGD2AndNoTPCInBunch,
    kTrueParticlesChargedInTPCorFGDInBunch, 
    ///High-angle samples
    kTrueParticlesChargedInFGD1ECalHAInBunch, 
    kTrueParticlesChargedInFGD2ECalHAInBunch, 
    ///True particles crossing ECal
    kTrueParticlesInECalInBunch,
    //True particles crossing P0D
    kTrueParticlesChargedInP0DInBunch,
    kTrueParticlesChargedInP0DAndTPCInBunch,
    kTrueParticlesChargedInP0DAndNoTPCInBunch   
  };

  EventBoxTracker();
  virtual ~EventBoxTracker();
  
  ///-------------  Michel Electron candidates ----------------------------------
  AnaFgdTimeBinB** FGDMichelElectrons[2];
  Int_t nFGDMichelElectrons[2];
};

#endif
