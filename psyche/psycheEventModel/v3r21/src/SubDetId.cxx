#include "SubDetId.hxx"
#include <iostream>
#include <bitset>

const unsigned long SubDetId::DetMask[] = {
    1<<SubDetId::kFGD1,
    1<<SubDetId::kFGD2,
    1<<SubDetId::kTPC1,
    1<<SubDetId::kTPC2,
    1<<SubDetId::kTPC3,
    1<<SubDetId::kP0D,
    1<<SubDetId::kDSECAL,
    1<<SubDetId::kTopTECAL,
    1<<SubDetId::kBottomTECAL, 
    1<<SubDetId::kLeftTECAL, 
    1<<SubDetId::kRightTECAL, 
    1<<SubDetId::kTopPECAL,
    1<<SubDetId::kBottomPECAL,
    1<<SubDetId::kLeftPECAL,
    1<<SubDetId::kRightPECAL,
    1<<SubDetId::kTopSMRD,
    1<<SubDetId::kBottomSMRD,
    1<<SubDetId::kLeftSMRD,
    1<<SubDetId::kRightSMRD,
    1<<SubDetId::kInvalidSubdetector,
    1<<SubDetId::kTPC     | SubDetId::MakeMask(SubDetId::kTPC3,       SubDetId::kTPC1),
    1<<SubDetId::kFGD     | SubDetId::MakeMask(SubDetId::kFGD2,       SubDetId::kFGD1),
    1<<SubDetId::kECAL    | SubDetId::MakeMask(SubDetId::kRightPECAL, SubDetId::kDSECAL),
    1<<SubDetId::kTECAL   | SubDetId::MakeMask(SubDetId::kRightTECAL, SubDetId::kTopTECAL),
    1<<SubDetId::kPECAL   | SubDetId::MakeMask(SubDetId::kRightPECAL, SubDetId::kTopPECAL),
    1<<SubDetId::kSMRD    | SubDetId::MakeMask(SubDetId::kRightSMRD,  SubDetId::kTopSMRD),
    1<<SubDetId::kTRACKER | SubDetId::MakeMask(SubDetId::kTPC3,       SubDetId::kFGD1),
    1<<SubDetId::kInvalid
};

void SubDetId::SetDetectorUsed(unsigned long &BitField, SubDetId::SubDetEnum det){
    BitField = BitField | 1<<det;
}

bool SubDetId::GetDetectorUsed(unsigned long BitField, SubDetId::SubDetEnum det){
    return BitField & DetMask[det];
}

bool SubDetId::GetDetectorUsed(unsigned long BitField, SubDetId_h det){
    return BitField & DetMask[det];
}


bool SubDetId::GetDetectorArrayUsed(unsigned long BitField, SubDetId::SubDetEnum dets[], int nDet){

    bool passed = 1;

    for(int i = 0; i < nDet; ++i){
        if(!(BitField & DetMask[dets[i]])){
            passed = 0;
        }
    }
    return passed;
}

bool SubDetId::GetDetectorArrayUsed(unsigned long BitField, SubDetId_h dets[], int nDet){

    bool passed = 1;

    for(int i = 0; i < nDet; ++i){
        if(!(BitField & DetMask[dets[i]])){
            passed = 0;
        }
    }
    return passed;
}


int SubDetId::GetTPC(unsigned long BitField){
    if      (BitField & DetMask[SubDetId::kTPC2]) return 2;
    else if (BitField & DetMask[SubDetId::kTPC3]) return 3;
    else if (BitField & DetMask[SubDetId::kTPC1]) return 1;
    else return -1;
}

void SubDetId::SetDetectorSystemFields(unsigned long &BitField){
    for(int i = SubDetId::kFGD1; i < SubDetId::kInvalidSubdetector; ++i){ //loop through sub-detectors list
        if (GetDetectorUsed(BitField, static_cast<SubDetId::SubDetEnum>(i))){
            // FGD
            if (i == SubDetId::kFGD1 || i == SubDetId::kFGD2)
              SetDetectorUsed(BitField, SubDetId::kFGD);
            // TPC
            else if (i == SubDetId::kTPC1 || i == SubDetId::kTPC2 || i == SubDetId::kTPC3)
              SetDetectorUsed(BitField, SubDetId::kTPC);
            // P0D (for consistency)
            else if (i == SubDetId::kP0D) continue;
            //ECal
            else if (i >= SubDetId::kDSECAL && i <= SubDetId::kRightPECAL){
                SetDetectorUsed(BitField, SubDetId::kECAL);
                //TECal
                if (i >= SubDetId::kTopTECAL && i <= SubDetId::kRightTECAL) SetDetectorUsed(BitField, SubDetId::kTECAL); 
                //PECal
                else if(i >= SubDetId::kTopPECAL && i <= SubDetId::kRightPECAL) SetDetectorUsed(BitField, SubDetId::kPECAL); 
            }
            //SMRD
            else if (i >= SubDetId::kTopSMRD && i <= SubDetId::kRightSMRD) SetDetectorUsed(BitField, SubDetId::kSMRD);
        }
    }
}
int SubDetId::NumberOfSetBits(unsigned long v){
    int c; // c accumulates the total bits set in v
    for (c = 0; v; c++){
        v &= v - 1; // clear the least significant bit set
    }
    return c;
}

int SubDetId::GetNSegmentsInDet(unsigned long BitField, SubDetId::SubDetEnum det){
    return NumberOfSetBits(BitField & DetMask[det]);
}

bool SubDetId::TrackUsesOnlyDet(unsigned long BitField, SubDetId::SubDetEnum det){
    BitField = BitField & MakeMask(SubDetId::kRightSMRD, SubDetId::kFGD1);
    // Should have at least on bit set
    if (!BitField) return false;
    return !(BitField & ~DetMask[det]);
}

bool SubDetId::IsTPCDetector(SubDetId::SubDetEnum det){
    return (det <= SubDetId::kTPC3 && det >= SubDetId::kTPC1);
}

bool SubDetId::IsFGDDetector(SubDetId::SubDetEnum det){
    return (det <= SubDetId::kFGD2 && det >= SubDetId::kFGD1);
}

bool SubDetId::IsECALDetector(SubDetId::SubDetEnum det){
    return (det <= SubDetId::kRightPECAL && det >= SubDetId::kDSECAL);
}

bool SubDetId::IsTECALDetector(SubDetId::SubDetEnum det){
    return (det <= SubDetId::kRightTECAL && det >= SubDetId::kTopTECAL);
}

bool SubDetId::IsPECALDetector(SubDetId::SubDetEnum det){
    return (det <= SubDetId::kRightPECAL && det >= SubDetId::kTopPECAL);
}

bool SubDetId::IsSMRDDetector(SubDetId::SubDetEnum det){
    return (det <= SubDetId::kRightSMRD && det >= SubDetId::kTopSMRD);
}

bool SubDetId::IsP0DDetector(SubDetId::SubDetEnum det){
  return (det ==kP0D);
}

bool SubDetId::HasTECALDetector(unsigned long BitField){
  if(BitField & DetMask[SubDetId::kTECAL]) return true;
  return false;
}

bool SubDetId::HasPECALDetector(unsigned long BitField){
  if(BitField & DetMask[SubDetId::kPECAL]) return true;
  return false;
}

bool SubDetId::HasSMRDDetector(unsigned long BitField){
  if(BitField & DetMask[SubDetId::kSMRD]) return true;
  return false;
}

SubDetId::SubDetEnum SubDetId::GetSubdetectorEnum(unsigned long BitField){
 
    BitField = BitField & MakeMask(SubDetId::kRightSMRD, SubDetId::kFGD1);
    int nBits = NumberOfSetBits(BitField);
    if(nBits!=1){
        std::cout << " Error: Track contains multiple subdetectors, cannot find a single subdetector enum to return." << std::endl;
        return SubDetId::kInvalid;
    }
    BitField = BitField | (BitField-1);
    nBits = NumberOfSetBits(BitField);
    return static_cast<SubDetId::SubDetEnum>(nBits-1);
}

