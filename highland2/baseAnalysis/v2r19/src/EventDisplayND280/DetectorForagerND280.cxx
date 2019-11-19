#include "DetectorForagerND280.hxx"
#include "GeometryManager.hxx"
#include "DetectorDefinition.hxx"

//********************************************************************
bool det::DetectorForagerND280::FillDetectorInfo(){
  //******************************************************************** 
  // Use volumes defined in Detector Definition
  
  // Fill them
  ND::hgman().InitializeDetectorDefinitionFromROOTGeometry();
 
  // Fill the map
  _detBBoxes[kFGD1] = det::DetBBox(anaUtils::ArrayToTVector3(DetDef::fgd1min), 
      anaUtils::ArrayToTVector3(DetDef::fgd1max));
      
      _detBBoxes[kFGD2] = det::DetBBox(anaUtils::ArrayToTVector3(DetDef::fgd2min), 
      anaUtils::ArrayToTVector3(DetDef::fgd2max));


    _detBBoxes[kTPC1] =  det::DetBBox(anaUtils::ArrayToTVector3(DetDef::tpc1min), 
      anaUtils::ArrayToTVector3(DetDef::tpc1max));

    _detBBoxes[kTPC2] = det::DetBBox(anaUtils::ArrayToTVector3(DetDef::tpc2min), 
      anaUtils::ArrayToTVector3(DetDef::tpc2max));

    _detBBoxes[kTPC3] = det::DetBBox(anaUtils::ArrayToTVector3(DetDef::tpc3min), 
      anaUtils::ArrayToTVector3(DetDef::tpc3max));

    _detBBoxes[kP0D] =  det::DetBBox(anaUtils::ArrayToTVector3(DetDef::p0dmin), 
      anaUtils::ArrayToTVector3(DetDef::p0dmax));

    _detBBoxes[kDSECAL] =  det::DetBBox(anaUtils::ArrayToTVector3(DetDef::dsecalmin), 
      anaUtils::ArrayToTVector3(DetDef::dsecalmax)); 
    
    _detBBoxes[kTopLeftTECAL] =  det::DetBBox(anaUtils::ArrayToTVector3(DetDef::tecalTLmin), 
      anaUtils::ArrayToTVector3(DetDef::tecalTLmax));
 
    _detBBoxes[kTopRightTECAL] = det::DetBBox(anaUtils::ArrayToTVector3(DetDef::tecalTRmin), 
      anaUtils::ArrayToTVector3(DetDef::tecalTRmax));
    
    _detBBoxes[kBottomLeftTECAL] = det::DetBBox(anaUtils::ArrayToTVector3(DetDef::tecalBLmin), 
      anaUtils::ArrayToTVector3(DetDef::tecalBLmax));
    
    _detBBoxes[kBottomRightTECAL] = det::DetBBox(anaUtils::ArrayToTVector3(DetDef::tecalBRmin), 
      anaUtils::ArrayToTVector3(DetDef::tecalBRmax));

    _detBBoxes[kLeftTECAL] = det::DetBBox(anaUtils::ArrayToTVector3(DetDef::tecalLmin), 
      anaUtils::ArrayToTVector3(DetDef::tecalLmax));
    
    _detBBoxes[kRightTECAL] = det::DetBBox(anaUtils::ArrayToTVector3(DetDef::tecalRmin), 
      anaUtils::ArrayToTVector3(DetDef::tecalRmax));

    _detBBoxes[kTopLeftPECAL] = det::DetBBox(anaUtils::ArrayToTVector3(DetDef::pecalTLmin), 
      anaUtils::ArrayToTVector3(DetDef::pecalTLmax));

    _detBBoxes[kTopRightPECAL] = det::DetBBox(anaUtils::ArrayToTVector3(DetDef::pecalTRmin), 
      anaUtils::ArrayToTVector3(DetDef::pecalTRmax));

    _detBBoxes[kBottomLeftPECAL] = det::DetBBox(anaUtils::ArrayToTVector3(DetDef::pecalBLmin), 
      anaUtils::ArrayToTVector3(DetDef::pecalBLmax));
    
    _detBBoxes[kBottomRightPECAL] = det::DetBBox(anaUtils::ArrayToTVector3(DetDef::pecalBRmin), 
      anaUtils::ArrayToTVector3(DetDef::pecalBRmax));

    _detBBoxes[kLeftPECAL] = det::DetBBox(anaUtils::ArrayToTVector3(DetDef::pecalLmin), 
      anaUtils::ArrayToTVector3(DetDef::pecalLmax));

    _detBBoxes[kRightPECAL] = det::DetBBox(anaUtils::ArrayToTVector3(DetDef::pecalRmin), 
      anaUtils::ArrayToTVector3(DetDef::pecalRmax));

    
    _detBBoxes[kTopLeftSMRD] = det::DetBBox(anaUtils::ArrayToTVector3(DetDef::smrdTLmin), 
      anaUtils::ArrayToTVector3(DetDef::smrdTLmax));

    _detBBoxes[kTopRightSMRD] = det::DetBBox(anaUtils::ArrayToTVector3(DetDef::smrdTRmin), 
      anaUtils::ArrayToTVector3(DetDef::smrdTRmax));

    _detBBoxes[kBottomLeftSMRD] = det::DetBBox(anaUtils::ArrayToTVector3(DetDef::smrdBLmin), 
      anaUtils::ArrayToTVector3(DetDef::smrdBLmax));

    _detBBoxes[kBottomRightSMRD] = det::DetBBox(anaUtils::ArrayToTVector3(DetDef::smrdBRmin), 
      anaUtils::ArrayToTVector3(DetDef::smrdBRmax));

    _detBBoxes[kLeft15SMRD] = det::DetBBox(anaUtils::ArrayToTVector3(DetDef::smrd15Lmin), 
      anaUtils::ArrayToTVector3(DetDef::smrd15Lmax));

    _detBBoxes[kLeft6SMRD] = det::DetBBox(anaUtils::ArrayToTVector3(DetDef::smrd6Lmin), 
      anaUtils::ArrayToTVector3(DetDef::smrd6Lmax));

    _detBBoxes[kLeft78SMRD] = det::DetBBox(anaUtils::ArrayToTVector3(DetDef::smrd78Lmin), 
      anaUtils::ArrayToTVector3(DetDef::smrd78Lmax));

    _detBBoxes[kRight15SMRD] = det::DetBBox(anaUtils::ArrayToTVector3(DetDef::smrd15Rmin), 
      anaUtils::ArrayToTVector3(DetDef::smrd15Rmax));
 
    _detBBoxes[kRight6SMRD] = det::DetBBox(anaUtils::ArrayToTVector3(DetDef::smrd6Rmin), 
      anaUtils::ArrayToTVector3(DetDef::smrd6Rmax));

    _detBBoxes[kRight78SMRD] = det::DetBBox(anaUtils::ArrayToTVector3(DetDef::smrd78Rmin), 
      anaUtils::ArrayToTVector3(DetDef::smrd78Rmax));
 
  return true;
}

