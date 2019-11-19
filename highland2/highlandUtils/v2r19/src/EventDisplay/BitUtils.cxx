#include "BitUtils.hxx"


//********************************************************************
const unsigned long butils::BitUtils::CnvMask[] = {
//********************************************************************
  1<<butils::kXZTrue,
  1<<butils::kYZTrue, 
  1<<butils::kXYTrue, 
  1<<butils::kXZReco, 
  1<<butils::kYZReco, 
  1<<butils::kXYReco  
};

//********************************************************************
const unsigned long butils::BitUtils::DrwMask[] = {
//********************************************************************
  1<<butils::kTrueParticles,
  1<<butils::kRecoParticles,
 // 1<<butils::kRecoConstituents, 
 // 1<<butils::kRecoHits,
//  1<<butils::kRecoClusters, 
 // 1<<butils::kDelayedClusters
};


//********************************************************************
int butils::BitUtils::NumberOfSetBits(unsigned long v){
//********************************************************************
  int c; // c accumulates the total bits set in v
    for (c = 0; v; c++){
        v &= v - 1; // clear the least significant bit set
    }
    return c;
}

//********************************************************************
void butils::BitUtils::SetCnvBitStatus(unsigned long &BitField, butils::CnvType bit, bool used){
  //********************************************************************
  if (used){
    BitField = BitField | 1 << bit;
    return;
  } 
  BitField &= ~(1 << bit);
}

//********************************************************************
bool butils::BitUtils::GetCnvBitUsed(unsigned long BitField, butils::CnvType bit){
  //********************************************************************
  return BitField & CnvMask[bit];
}

//********************************************************************
void butils::BitUtils::SetDrwBitStatus(unsigned long &BitField, butils::DrwType bit, bool used){
  //********************************************************************
  
  if (used){
    BitField = BitField | 1 << bit;
    return;
  } 
  BitField &= ~(1 << bit);
}

//********************************************************************
bool butils::BitUtils::GetDrwBitUsed(unsigned long BitField, butils::DrwType bit){
  //********************************************************************
  return BitField & DrwMask[bit];
}







