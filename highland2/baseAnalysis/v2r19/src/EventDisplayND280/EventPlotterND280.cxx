#include "EventPlotterND280.hxx"
#include "DetectorForagerND280.hxx"

//********************************************************************
datt::HLAttLine plot::EventPlotterND280::GetLineAttributesBox(Int_t det) const{
  //********************************************************************
  datt::HLAttLine att(15, 1, 3);

  Color_t col = att.Color;
  switch (det){
    case det::kFGD1:
    case det::kFGD2:
      col = 2;
      break;

    case det::kTPC1:         
    case det::kTPC2:
    case det::kTPC3:
      col = 1;
      break;

    case det::kP0D:
      col = 6;
      break;

    case det::kDSECAL:
      col = 3;
      break;

    case det::kTopLeftTECAL: 
    case det::kTopRightTECAL:  
    case det::kBottomLeftTECAL:  
    case det::kBottomRightTECAL:
    case det::kLeftTECAL:    
    case det::kRightTECAL:   
      col = 8;
      break;

    case det::kTopLeftPECAL:  
    case det::kTopRightPECAL:  
    case det::kBottomLeftPECAL:  
    case det::kBottomRightPECAL:  
    case det::kLeftPECAL:    
    case det::kRightPECAL:
      col = 4;
      break;

    case det::kTopLeftSMRD:  
    case det::kTopRightSMRD:  
    case det::kBottomLeftSMRD:  
    case det::kBottomRightSMRD:  
    case det::kLeft15SMRD:   
    case det::kLeft6SMRD:    
    case det::kLeft78SMRD:   
    case det::kRight15SMRD:  
    case det::kRight6SMRD:   
    case det::kRight78SMRD:
      col = 7;
      break;
    default:
      break;
  }
  att.Color = col;

  return att; 
}


//********************************************************************
bool plot::EventPlotterND280::CheckAddForProjection(butils::PrjType proj, Int_t det) const{
  //********************************************************************

  switch (proj){

    case butils::kXZ:{
                       switch(det){

                         case det::kTopLeftTECAL: 
                         case det::kTopRightTECAL:  
                         case det::kBottomLeftTECAL:  
                         case det::kBottomRightTECAL:
                         case det::kTopLeftPECAL:  
                         case det::kTopRightPECAL:  
                         case det::kBottomLeftPECAL:  
                         case det::kBottomRightPECAL:  
                         case det::kTopLeftSMRD:  
                         case det::kTopRightSMRD:  
                         case det::kBottomLeftSMRD:  
                         case det::kBottomRightSMRD:  

                           return false;
                           break;
                         default:
                           return true;
                           break;

                       }
                       break;
                     }
    case butils::kYZ:

                     switch(det){
                       case det::kLeftTECAL:    
                       case det::kRightTECAL:   
                       case det::kLeftPECAL:    
                       case det::kRightPECAL:
                       case det::kLeft15SMRD:   
                       case det::kLeft6SMRD:    
                       case det::kLeft78SMRD:   
                       case det::kRight15SMRD:  
                       case det::kRight6SMRD:   
                       case det::kRight78SMRD:
                         return false;
                         break;
                       default:
                         return true;
                         break;

                     }

                     break;
    case butils::kXY:
                     return true;
                     break;
    default:
                     return true;
                     break;

  }

  return true;

} 

