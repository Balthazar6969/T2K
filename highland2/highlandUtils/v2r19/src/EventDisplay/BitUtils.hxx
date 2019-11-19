#ifndef BitUtils_h
#define BitUtils_h

namespace butils{

  /// Projection enum
  enum PrjType{
    kXZ = 0, 
    kYZ, 
    kXY,
    kNPrjTypes
  };

  /// Canvas type
  enum CnvType{
    kXZTrue = 0, 
    kYZTrue, 
    kXYTrue,
    kXZReco, 
    kYZReco, 
    kXYReco, 
    kNCnvTypes
  };


  enum DrwType{
    kTrueParticles = 0, 
    kRecoParticles,
//    kRecoConstituents, 
  //  kRecoHits,
  //  kRecoClusters, 
  //  kDelayedClusters, 
    kNDrwTypes
  };

  class BitUtils{

  public:
    /// Method to count the number of set bits in a given bit field
    static int NumberOfSetBits(unsigned long);

    /// Method to set a certain bit as used
    static void SetCnvBitStatus(unsigned long&, butils::CnvType, bool);

    /// Method to see if a certain bit is set as used
    static bool GetCnvBitUsed(unsigned long, butils::CnvType);

    /// Method to set a certain bit as used
    static void SetDrwBitStatus(unsigned long&, butils::DrwType bit, bool);

    /// Method to see if a certain bit is set as used
    static bool GetDrwBitUsed(unsigned long BitField, butils::DrwType bit);

  protected:
    /// Bit mask to associate bits with canvas type
    const static unsigned long CnvMask[kNCnvTypes];

    /// Bit mask to associate bits with drawing type
    const static unsigned long DrwMask[kNDrwTypes];  

  };
};


#endif
