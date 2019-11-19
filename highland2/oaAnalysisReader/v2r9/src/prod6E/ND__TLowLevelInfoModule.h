//////////////////////////////////////////////////////////
//   This class has been generated by TFile::MakeProject
//     (Thu Nov 17 16:53:10 2016 by ROOT version 5.34/34)
//      from the StreamerInfo in file /hep/T2K/DataDir/PROD6/oa_nd_spl_00011000-0000_4stgpe5m7bc7_anal_000_v11r31p5-wg-bugaboo-bsdv01_2_prod6E.root
//////////////////////////////////////////////////////////


#ifndef ND__TLowLevelInfoModule_h
#define ND__TLowLevelInfoModule_h
namespace ND {
class TLowLevelInfoModule;
} // end of namespace.

#include "ND__TAnalysisModuleBase.h"
#include "TClonesArray.h"
#include "TObject.h"

namespace ND {
class TLowLevelInfoModule : public ND::TAnalysisModuleBase {

public:
// Nested classes forward declaration.
class TLowLevelTFBDigit;
class TLowLevelHit;

public:
// Nested classes declaration.
class TLowLevelHit : public TObject {

public:
// Nested classes declaration.

public:
// Data Members.
   Float_t     Time;        //
   Float_t     Charge;      //
   UInt_t      GeomId;      //
   Float_t     PosX;        //
   Float_t     PosY;        //
   Float_t     PosZ;        //
   Int_t       XLayer;      //
   Int_t       YLayer;      //
   Int_t       Bar;         //
   Int_t       End;         //
   Float_t     TimeRead;    //
   Int_t       Event;       //

   TLowLevelHit();
   TLowLevelHit(const TLowLevelHit & );
   virtual ~TLowLevelHit();

   ClassDef(TLowLevelHit,2); // Generated by MakeProject.
};
class TLowLevelTFBDigit : public TObject {

public:
// Nested classes declaration.

public:
// Data Members.
   UInt_t      Chan;        //
   UInt_t      Tfb;         //
   UInt_t      Rmm;         //
   UInt_t      Subdet;      //
   Int_t       Side;        //
   UInt_t      Err;         //
   Long64_t    Tdc;         //
   Long64_t    TdcTrig;     //
   Long64_t    TdcRead;     //
   Int_t       Adc_lo;      //
   Int_t       Adc_hi;      //

   TLowLevelTFBDigit();
   TLowLevelTFBDigit(const TLowLevelTFBDigit & );
   virtual ~TLowLevelTFBDigit();

   ClassDef(TLowLevelTFBDigit,2); // Generated by MakeProject.
};

public:
// Data Members.
   TClonesArray* fLowLevelTFBDigits;    //
   int           fNLowLevelTFBDigits;    //
   TClonesArray* fLowLevelHits;          //
   int           fNLowLevelHits;         //

   TLowLevelInfoModule();
   TLowLevelInfoModule(const TLowLevelInfoModule & );
   virtual ~TLowLevelInfoModule();

   ClassDef(TLowLevelInfoModule,2); // Generated by MakeProject.
};
} // namespace
#endif