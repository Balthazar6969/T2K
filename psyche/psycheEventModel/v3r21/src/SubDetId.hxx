#ifndef SubDetId_hxx_seen
#define SubDetId_hxx_seen

#include "BasicTypes.hxx"

class SubDetId {

    private:
        /// Bit mask array to select different subdetectors and systems
        const static unsigned long DetMask[28];

        /// Method to make detector bit masks
        static unsigned long MakeMask(int msb, int lsb) {return (((1<<(((msb)-(lsb))+1))-1)<<(lsb));}

        /// Method to count the number of set bits in a given bit field
        static int NumberOfSetBits(unsigned long v);


    public:

        SubDetId(){};
        ~SubDetId(){};

        /// Enumeration of all detector systems and subdetectors
        enum SubDetEnum {
            kFGD1 = 0,
            kFGD2,
            kTPC1,
            kTPC2,
            kTPC3,
            kP0D,
            kDSECAL,
            kTopTECAL,
            kBottomTECAL,
            kLeftTECAL,
            kRightTECAL,
            kTopPECAL,
            kBottomPECAL,
            kLeftPECAL,
            kRightPECAL,
            kTopSMRD,
            kBottomSMRD,
            kLeftSMRD,
            kRightSMRD,
            kInvalidSubdetector,
            kTPC,
            kFGD,
            kECAL,
            kTECAL,
            kPECAL,
            kSMRD,
            kTRACKER,
            kInvalid
        };

        /// Method to set a certain subdetector or subdetector system to used used
        static void SetDetectorUsed(unsigned long &BitField, SubDetId::SubDetEnum det);

        /// Method to see if a certain subdetector or subdetector system is used
        static bool GetDetectorUsed(unsigned long BitField, SubDetId::SubDetEnum det);

        static bool GetDetectorArrayUsed(unsigned long BitField, SubDetId::SubDetEnum dets[], int nDet);

        static bool GetDetectorUsed(unsigned long BitField, SubDetId_h det);
        static bool GetDetectorArrayUsed(unsigned long BitField, SubDetId_h dets[], int nDet);


        /// Returns the TPC number (TPC1 = 1, TPC2 = 2 etc.) of the track
        /// Does not check whether track passed through more than one TPC, just returns the first TPC it finds (checking TPC2, TPC3 then TPC1)
        static int GetTPC(unsigned long BitField);

        /// Method to set the detector system bits using the individual subdetector bits.
        /// e.g. If SetDetectorUsed(BitField, SubDetId::kTPC1) had been called, this function
        /// would call SetDetectorUsed(BitField, SubDetId::kTPC)
        static void SetDetectorSystemFields(unsigned long &BitField);

        /// Method to count the number of Segments a given track has in a detector system
        /// The DetMask bitfield masks are used to select the specified detector system
        static int GetNSegmentsInDet(unsigned long BitFIeld, SubDetId::SubDetEnum det);

        /// Check whether a track only uses a specified subdetector or detector system
        static bool TrackUsesOnlyDet(unsigned long BitFIeld, SubDetId::SubDetEnum det);

        /// Check if a detector enumeration refers to a TPC or not
        static bool IsTPCDetector(SubDetId::SubDetEnum det);

        /// Check if a detector enumeration refers to a FGD or not
        static bool IsFGDDetector(SubDetId::SubDetEnum det);

        /// Check if a detector enumeration refers to a SMRD or not
        static bool IsSMRDDetector(SubDetId::SubDetEnum det);

        /// Check if a detector enumeration refers to a SMRDP0D or not
        static bool IsP0DDetector(SubDetId::SubDetEnum det);

        /// Check if a detector enumeration refers to a ECAL or not
        static bool IsECALDetector(SubDetId::SubDetEnum det);

        /// Check if a detector enumeration refers to a Tracker ECAL or not
        static bool IsTECALDetector(SubDetId::SubDetEnum det);

        /// Check if a detector enumeration refers to a P0D ECAL or not
        static bool IsPECALDetector(SubDetId::SubDetEnum det);

        /// Check if a detector bit field has a Tracker ECAL or not
        static bool HasTECALDetector(unsigned long BitField);

        /// Check if a detector bit field has a P0D ECAL or not
        static bool HasPECALDetector(unsigned long BitField);

        /// Check if a detector bit field has a SMRD or not
        static bool HasSMRDDetector(unsigned long BitField);

        /// Get the single subdetector that this track is from
        static SubDetId::SubDetEnum GetSubdetectorEnum(unsigned long BitField);

};
#endif


