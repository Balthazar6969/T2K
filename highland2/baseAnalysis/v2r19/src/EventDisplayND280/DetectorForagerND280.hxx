#ifndef DetectorForagerND280_h
#define DetectorForagerND280_h

#include "EventDisplay/DetectorForager.hxx"

namespace det{

  /// Enumeration for detectors
  /// Use different from SubDetId since want to be more
  /// specific e.g. with SMRD side volumes which have three boxes
  enum ModuleDetEnum {
    kFGD1 = 0,
    kFGD2,
    kTPC1,
    kTPC2,
    kTPC3,
    kP0D,
    kDSECAL,
    kTopLeftTECAL, 
    kTopRightTECAL, 
    kBottomLeftTECAL,
    kBottomRightTECAL,
    kLeftTECAL,
    kRightTECAL,
    kTopLeftPECAL,
    kTopRightPECAL,
    kBottomLeftPECAL,
    kBottomRightPECAL,
    kLeftPECAL,
    kRightPECAL,
    kTopLeftSMRD,
    kTopRightSMRD,
    kBottomLeftSMRD,
    kBottomRightSMRD,
    kLeft15SMRD,
    kLeft6SMRD,
    kLeft78SMRD,
    kRight15SMRD, 
    kRight6SMRD,
    kRight78SMRD
  };


  class DetectorForagerND280: public det::DetectorForager{
  public:
    DetectorForagerND280(){}
    virtual ~DetectorForagerND280(){}

    /// Fill the detector information
    bool FillDetectorInfo();

  protected:

//    bool VisitNode(int, 
   //     const std::string& name, 
     //   const TGeoNode* node);
  };


};


#endif
