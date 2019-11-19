#include "TreeConverterUtils.hxx"
#include <stdio.h>
#include <math.h>


namespace convUtils {}


//****************************************************************************
void convUtils::ConvertTPCDetEnumToBitField(unsigned long &det, int tpc){
//****************************************************************************
    SubDetId::SetDetectorUsed(det, SubDetId::kTPC);
    switch(tpc){
        case 0: SubDetId::SetDetectorUsed(det, SubDetId::kTPC1);
                break;
        case 1: SubDetId::SetDetectorUsed(det, SubDetId::kTPC2);
                break;
        case 2: SubDetId::SetDetectorUsed(det, SubDetId::kTPC3);
                break;
        default: SubDetId::SetDetectorUsed(det, SubDetId::kInvalid);
    }
}

//****************************************************************************
void convUtils::ConvertFGDDetEnumToBitField(unsigned long &det, int fgd){
//****************************************************************************
   SubDetId::SetDetectorUsed(det, SubDetId::kFGD);
    switch(fgd){
        case 0: SubDetId::SetDetectorUsed(det, SubDetId::kFGD1);
                break;
        case 1: SubDetId::SetDetectorUsed(det, SubDetId::kFGD2);
                break;
        default: SubDetId::SetDetectorUsed(det, SubDetId::kInvalid);
    }
}

//****************************************************************************
void convUtils::ConvertSMRDDetEnumToBitField(unsigned long &det, int smrd){
//****************************************************************************
    SubDetId::SetDetectorUsed(det, SubDetId::kSMRD);
    switch(smrd){
        case 0: SubDetId::SetDetectorUsed(det, SubDetId::kTopSMRD);
                break;
        case 1: SubDetId::SetDetectorUsed(det, SubDetId::kBottomSMRD);
                break;
        case 2: SubDetId::SetDetectorUsed(det, SubDetId::kLeftSMRD);
                break;
        case 3: SubDetId::SetDetectorUsed(det, SubDetId::kRightSMRD);
                break;
        default: SubDetId::SetDetectorUsed(det, SubDetId::kInvalid);
    }

    SubDetId::SetDetectorSystemFields(det);

}

//****************************************************************************
void convUtils::ConvertECALDetEnumToBitField(unsigned long &det, int ecal){
//****************************************************************************
 switch(ecal){
        case 0: SubDetId::SetDetectorUsed(det, SubDetId::kLeftPECAL);
                break;
        case 1: SubDetId::SetDetectorUsed(det, SubDetId::kRightPECAL);
                break;
        case 2: SubDetId::SetDetectorUsed(det, SubDetId::kTopPECAL);
                break;
        case 3: SubDetId::SetDetectorUsed(det, SubDetId::kBottomPECAL);
                break;
        case 4: SubDetId::SetDetectorUsed(det, SubDetId::kLeftTECAL);
                break;
        case 5: SubDetId::SetDetectorUsed(det, SubDetId::kRightTECAL);
                break;
        case 6: SubDetId::SetDetectorUsed(det, SubDetId::kTopTECAL);
                break;
        case 7: SubDetId::SetDetectorUsed(det, SubDetId::kBottomTECAL);
                break;
        case 8: SubDetId::SetDetectorUsed(det, SubDetId::kDSECAL);
                break;
        default: SubDetId::SetDetectorUsed(det, SubDetId::kInvalid);
    }
    SubDetId::SetDetectorSystemFields(det);

}

//****************************************************************************
void convUtils::ConvertLocalDetEnumToBitField(unsigned long &det, int index, SubDetId::SubDetEnum subdet_enum){
//****************************************************************************

  switch(subdet_enum){
    case SubDetId::kTPC :
      convUtils::ConvertTPCDetEnumToBitField(det, index);    
      break;
    case SubDetId::kFGD :
      convUtils::ConvertFGDDetEnumToBitField(det, index);
      break;
    case SubDetId::kECAL :
      convUtils::ConvertECALDetEnumToBitField(det, index);
      break;
    case SubDetId::kSMRD :
      convUtils::ConvertSMRDDetEnumToBitField(det, index); 
      break;
    case  SubDetId::kP0D :
      SubDetId::SetDetectorUsed(det, SubDetId::kP0D);
      break;
    default :
      std::cout << "convUtils::ConvertLocalDetEnumToBitField(). Cannot get old local detector enumeration for subsystem: " << subdet_enum << ". Please define whether it is a TPC, FGD, ECAL, SMRD or P0D track." << std::endl;
      SubDetId::SetDetectorUsed(det, SubDetId::kInvalid);
  }
}

//****************************************************************************
void convUtils::ConvertTrueParticleDetEnumToBitField(unsigned long &det, int DetUsed){
//****************************************************************************

/*   This is the enumeration used in oaAnalysis (ToaAnalysisUtils.hxx)
  kFGD1 = 0,
  kFGD2,
  kP0D,
  kDsECal,
  kBrlECal,
  kP0DECal,
  kTPC1,
  kTPC2,
  kTPC3,
  kMRD,
  kOffAxis,                                                         /// the rest of the off-axis detector
  kINGRID,
  kCavern,
  kNSubdetectors
    
  // Organised as such to try and preclude backwards compatibility errors at
  // the moment, will probably be reorganised in the future [lp208 21-1-2015]
  kP0DECalBottom = 13,
  kP0DECalLeft = 14,
  kP0DECalTop = 15,
  kP0DECalRight = 16,
  kBrlECalBottom = 17,
  kBrlECalLeft = 18,
  kBrlECalTop = 19,
  kBrlECalRight = 20
*/

  switch(DetUsed){
    case 0: SubDetId::SetDetectorUsed(det, SubDetId::kFGD1);
            break;
    case 1: SubDetId::SetDetectorUsed(det, SubDetId::kFGD2);
            break;
    case 2: SubDetId::SetDetectorUsed(det, SubDetId::kP0D);
            break;
    case 3: SubDetId::SetDetectorUsed(det, SubDetId::kDSECAL);
            break;
    case 4: SubDetId::SetDetectorUsed(det, SubDetId::kECAL);  // Temporary fix whilst SubDetId in truth is not available from oaAnalysis
            SubDetId::SetDetectorUsed(det, SubDetId::kTECAL);
            break;
    case 5: SubDetId::SetDetectorUsed(det, SubDetId::kECAL);  // Temporary fix whilst SubDetId in truth is not available from oaAnalysis
            SubDetId::SetDetectorUsed(det, SubDetId::kPECAL);
            break;
    case 6: SubDetId::SetDetectorUsed(det, SubDetId::kTPC1);
            break;
    case 7: SubDetId::SetDetectorUsed(det, SubDetId::kTPC2);
            break;
    case 8: SubDetId::SetDetectorUsed(det, SubDetId::kTPC3);
            break;
    case 9: SubDetId::SetDetectorUsed(det, SubDetId::kSMRD);
            break;
    case 13: SubDetId::SetDetectorUsed(det, SubDetId::kBottomPECAL);
            break;          
    case 14: SubDetId::SetDetectorUsed(det, SubDetId::kLeftPECAL);
            break;         
    case 15: SubDetId::SetDetectorUsed(det, SubDetId::kTopPECAL);
            break;          
    case 16: SubDetId::SetDetectorUsed(det, SubDetId::kRightPECAL);
            break;         
    case 17: SubDetId::SetDetectorUsed(det, SubDetId::kBottomTECAL);
            break;          
    case 18: SubDetId::SetDetectorUsed(det, SubDetId::kLeftTECAL);
            break;         
    case 19: SubDetId::SetDetectorUsed(det, SubDetId::kTopTECAL);
            break;          
    case 20: SubDetId::SetDetectorUsed(det, SubDetId::kRightTECAL);
            break;

    default: SubDetId::SetDetectorUsed(det, SubDetId::kInvalid);
  }
  SubDetId::SetDetectorSystemFields(det);
}

//****************************************************************************
void convUtils::ConvertTrackerDetEnumToBitField(unsigned long &det, int DetUsed){
//****************************************************************************
  switch(DetUsed){
    case 1: SubDetId::SetDetectorUsed(det, SubDetId::kTPC1);
            break;
    case 2: SubDetId::SetDetectorUsed(det, SubDetId::kTPC2);
            break;
    case 3: SubDetId::SetDetectorUsed(det, SubDetId::kTPC3);
            break;
    case 4: SubDetId::SetDetectorUsed(det, SubDetId::kFGD1);
            break;
    case 5: SubDetId::SetDetectorUsed(det, SubDetId::kFGD2);
            break;
    default: SubDetId::SetDetectorUsed(det, SubDetId::kInvalid);
  }
  SubDetId::SetDetectorSystemFields(det);
}


//********************************************************************** 
void convUtils::ConvertTrackerDetFieldToBitField(unsigned long &det, int Detectors){
//**********************************************************************
  //try to avoid string comparison
   
  //cannot be a tracker object
  if (Detectors>12345){
    SubDetId::SetDetectorUsed(det, SubDetId::kInvalid);
    return;
  }

  for(int i=5; i>0; i--){
    int value = Detectors % 10;
    convUtils::ConvertTrackerDetEnumToBitField(det, value);
    Detectors /=10;
    if (Detectors == 0)
      break; 
  }
    
}


//****************************************************************************
void convUtils::ConvertTrackDetEnumToBitField(unsigned long &det, int DetUsed[]){
 //****************************************************************************
  for(UInt_t i = 0; i < NDETECTORS; ++i){
    if(DetUsed[i]==1){
     convUtils::SetDetectorUsedField(det, i);
    }
  }
  SubDetId::SetDetectorSystemFields(det);
}

//****************************************************************************
void convUtils::ConvertTrackDetEnumToBitField(unsigned long &det, bool DetUsed[]){
//****************************************************************************
  for(UInt_t i = 0; i < NDETECTORS; ++i){
    if(DetUsed[i]){
     convUtils::SetDetectorUsedField(det, i);
    }
  }
  SubDetId::SetDetectorSystemFields(det);
}


//****************************************************************************
void convUtils::SetDetectorUsedField(unsigned long &det, unsigned int i){
//****************************************************************************
  switch(i){
    case 0: SubDetId::SetDetectorUsed(det, SubDetId::kTPC1); 
            break;
    case 1: SubDetId::SetDetectorUsed(det, SubDetId::kTPC2);
            break;
    case 2: SubDetId::SetDetectorUsed(det, SubDetId::kTPC3);
            break;
    case 3: SubDetId::SetDetectorUsed(det, SubDetId::kFGD1);
            break;
    case 4: SubDetId::SetDetectorUsed(det, SubDetId::kFGD2);
            break;
    case 5: SubDetId::SetDetectorUsed(det, SubDetId::kP0D);
            break;
    case 6: SubDetId::SetDetectorUsed(det, SubDetId::kDSECAL);
            break;
    case 7: SubDetId::SetDetectorUsed(det, SubDetId::kTopSMRD);
            break;
    case 8: SubDetId::SetDetectorUsed(det, SubDetId::kBottomSMRD);
            break;
    case 9: SubDetId::SetDetectorUsed(det, SubDetId::kLeftSMRD);
            break;
    case 10: SubDetId::SetDetectorUsed(det, SubDetId::kRightSMRD);
             break;
    case 11: SubDetId::SetDetectorUsed(det, SubDetId::kTopPECAL);
             break;
    case 12: SubDetId::SetDetectorUsed(det, SubDetId::kBottomPECAL);
             break;
    case 13: SubDetId::SetDetectorUsed(det, SubDetId::kLeftPECAL);
             break;
    case 14: SubDetId::SetDetectorUsed(det, SubDetId::kRightPECAL);
             break;
    case 15: SubDetId::SetDetectorUsed(det, SubDetId::kTopTECAL);
             break;
    case 16: SubDetId::SetDetectorUsed(det, SubDetId::kBottomTECAL);
             break;
    case 17: SubDetId::SetDetectorUsed(det, SubDetId::kLeftTECAL);
             break;
    case 18: SubDetId::SetDetectorUsed(det, SubDetId::kRightTECAL);
             break;
    case 19: SubDetId::SetDetectorUsed(det, SubDetId::kTRACKER);
             break;
    default: SubDetId::SetDetectorUsed(det, SubDetId::kInvalid);
  }
  return;
}


//********************************************************************
int convUtils::GetLocalDetEnum(SubDetId::SubDetEnum det, SubDetId::SubDetEnum idet){
//********************************************************************

    switch(det){
        case SubDetId::kTPC :
            if(SubDetId::IsTPCDetector(idet)) return (idet - 2);
	          else std::cout << "Error: Should not be here, detector " << idet << " is not in the TPC" << std::endl;
            break;
        case SubDetId::kFGD :
            if(SubDetId::IsFGDDetector(idet)) return (idet);
	          else std::cout << "Error: Should not be here, detector " << idet << " is not in the FGD" << std::endl;
            break;
        case SubDetId::kP0D :
            if(SubDetId::IsP0DDetector(idet)) return 0;
	          else std::cout << "Error: Should not be here, detector " << idet << " is not in the P0D" << std::endl;
            break;
        case SubDetId::kECAL :
            if(SubDetId::IsECALDetector(idet)){
                switch(idet){
                    case SubDetId::kLeftPECAL: 
                        return 0;
                        break;
                    case SubDetId::kRightPECAL: 
                        return 1;
                        break;
                    case SubDetId::kTopPECAL: 
                        return 2;
                        break;
                    case SubDetId::kBottomPECAL: 
                        return 3;
                        break;
                    case SubDetId::kLeftTECAL: 
                        return 4;
                        break;
                    case SubDetId::kRightTECAL: 
                        return 5;
                        break;
                    case SubDetId::kTopTECAL: 
                        return 6;
                        break;
                    case SubDetId::kBottomTECAL: 
                        return 7;
                        break;
                    case SubDetId::kDSECAL: 
                        return 8;
                        break;
                    default:
                        std::cout << "Error: Should not be here, detector " << idet << " is not in the ECal" << std::endl;
                        return -1;
                }
            }
            break;
        case SubDetId::kSMRD :
            if(SubDetId::IsSMRDDetector(idet)){
                switch(idet){
                    case SubDetId::kLeftSMRD:
                        return 0;
                        break;
                    case SubDetId::kRightSMRD:
                        return 1;
                        break;
                    case SubDetId::kTopSMRD:
                        return 2;
                        break;
                    case SubDetId::kBottomSMRD:
                        return 3;
                        break;
                    default:
                        std::cout << "Error: Should not be here, detector " << idet << " is not in the SMRD" << std::endl;
                        return -1;
                }
            }
            break;
        default :
            std::cout << "convUtils::GetLocalDetEnum(). Cannot get old local detector enumeration for subsystem: " << det << ". Please define whether it is a TPC, FGD, ECAL, P0D or SMRD track." << std::endl;
            return -1;
    }
    std::cout << "convUtils::GetLocalDetEnum(). Cannot get old local detector enumeration for subsystem: " << det << ". Please define whether it is a TPC, FGD, ECAL, P0D or SMRD track." << std::endl;
    return -1;
}


//********************************************************************** 
void convUtils::ConvertBitFieldToTrackerDetField(unsigned long det, int& trackerDet){
//**********************************************************************
  //try to avoid string comparison
   
  trackerDet=0;

  if (SubDetId::GetDetectorUsed(det,SubDetId::kTPC1)) trackerDet = 1;
  if (SubDetId::GetDetectorUsed(det,SubDetId::kTPC2)) trackerDet = trackerDet*10+2;
  if (SubDetId::GetDetectorUsed(det,SubDetId::kTPC3)) trackerDet = trackerDet*10+3;
  if (SubDetId::GetDetectorUsed(det,SubDetId::kFGD1)) trackerDet = trackerDet*10+4;
  if (SubDetId::GetDetectorUsed(det,SubDetId::kFGD2)) trackerDet = trackerDet*10+5;    

  if (trackerDet==0) trackerDet = -1;
}

//****************************************************************************
void convUtils::ConvertBitFieldToTrueParticleDetEnum(unsigned long det, int &trueDet){
//****************************************************************************

  //Note! special treatment since oaAnalysis true info does not allow to split between 
  //TECal/PECal/SMRD sub-detectors and hence now sub-detector bits are set
  
  trueDet=-1;

  if (      SubDetId::GetDetectorUsed(det, SubDetId::kFGD1))    trueDet = 0;
  else if ( SubDetId::GetDetectorUsed(det, SubDetId::kFGD2))    trueDet = 1;
  else if ( SubDetId::GetDetectorUsed(det, SubDetId::kP0D))     trueDet = 2;
  else if ( SubDetId::GetDetectorUsed(det, SubDetId::kDSECAL))  trueDet = 3;
  else if ((SubDetId::GetDetectorUsed(det, SubDetId::kTECAL)) ||
      (     det & (1<<SubDetId::kTECAL)))                       trueDet = 4;
  else if ((SubDetId::GetDetectorUsed(det, SubDetId::kPECAL)) ||
      (     det & (1<<SubDetId::kPECAL)))                       trueDet = 5;
  else if ( SubDetId::GetDetectorUsed(det, SubDetId::kTPC1))    trueDet = 6;
  else if ( SubDetId::GetDetectorUsed(det, SubDetId::kTPC2))    trueDet = 7;
  else if ( SubDetId::GetDetectorUsed(det, SubDetId::kTPC3))    trueDet = 8;
  else if ((SubDetId::GetDetectorUsed(det, SubDetId::kSMRD)) ||
      (     det & (1<<SubDetId::kSMRD)))                        trueDet = 9;
}

