//////////////////////////////////////////////////////////
//   This class has been generated by TFile::MakeProject
//     (Tue Mar 20 17:07:54 2018 by ROOT version 5.34/34)
//      from the StreamerInfo in file /mnt/data/backup/production006/B/mcp/neut/2010-11-air/magnet/run4/anal/oa_nt_beam_90400000-0000_jrvqqzb53okw_anal_000_prod6amagnet201011airc-bsdv01_2.root
//////////////////////////////////////////////////////////


#ifndef ND__TBeamSummaryDataModule_h
#define ND__TBeamSummaryDataModule_h
namespace ND {
class TBeamSummaryDataModule;
} // end of namespace.

#include "ND__TAnalysisHeaderModuleBase.h"
#include "TClonesArray.h"
#include "TObject.h"
#include <TString.h>
#include "ND__TBeamSummaryDataModule.h"

namespace ND {
class TBeamSummaryDataModule : public ND::TAnalysisHeaderModuleBase {

public:
// Nested classes forward declaration.
class TBeamSummaryData;

public:
// Nested classes declaration.
class TBeamSummaryData : public TObject {

public:
// Nested classes forward declaration.
class TOtherData;

public:
// Nested classes declaration.
class TOtherData : public TObject {

public:
// Nested classes declaration.

public:
// Data Members.
   Int_t       MidasEvent;    ///< MIDAS Event number in BSD root file. not for nd280 analysis.
   Int_t       BeamRunNumber;    ///< Beam Run Number in Neutrino beamline.
   Int_t       SpillNumber;      ///< Spill Number in 32 bit
   Int_t       MRRunNumber;      ///< Main Ring Run Number.
   Int_t       GPSStatus[2];     ///< Status of GPS. [0] = GPS1, [1] = GPS2.
   Int_t       TriggerTime[3];    ///< Trigger Time (Unix Time in sec), [0] = GPS1, [1] = GPS2, [2] = Rb clock system.
   Int_t       TriggerTimeNanoSecond[3];    ///< Trigger Time for sub-second (in nano sec), [0] = GPS1, [1] = GPS2, [2] = Rb clock system.
   Double_t    ProtonsPerSpill[5];          ///< Number of Protons in the SPILL counted by CTs, [0] = CT1,..[4] = CT5.
   Double_t    ProtonsPerBunch[5][9];       ///< Number of Protons in the BUNCH counted by CTs, [0-4] = CT1-CT5, [0-8] = Whole of bunches, 1st bunch, ... 8th bunch.
   Double_t    BeamTiming[5];               ///< Timing of Proton beam measured by CTs. [0-4] = CT1-CT5. Time 0 is Trigger Time. [micro sec]
   Double_t    BeamBunchTiming[5][9];       ///< Timing of Proton beam BUNCH measured by CTs. [0-4] = CT1-CT5, [0-8] = Whole of bunches, 1st bunch, ... 8th bunch. Time 0 is Trigger Time. [micro sec]
   Double_t    BeamFlag[5];                 ///< Flag describing that the Proton Beam is detected by CTs or not. 1 = extracted, 0 = not extracted.
   Double_t    BeamBunchFlag[5][9];         ///< Flag describing that the Proton Beam is detected by CTs or not in the BUNCH (1st-8th). 1 = extracted, 0 = not extracted.
   Double_t    HornCurrent[3];              ///< Horn Current. [0-3] = Horn1 - Horn3. [kA]
   Double_t    HornBusBarCurrent[3][5];     ///< Horn Current for each busbar. [0..3][] = Horn1..Horn3. [][0..4] = individual ch. [kA]
   Double_t    BeamPositionOnTarget[2];     ///< Beam Position on the target. [0..1] = [x and y]. [mm]
   Double_t    BeamDirectionOnTarget[2];    ///< Beam Direction on the target. [0..1] = [x and y]. [mm]
   Double_t    BeamSizeOnTarget[2];         ///< Beam Size on the target. [0..1] = [x and y]. [mm]
   Double_t    MumonSiTotalQ;               ///< MUMON Total Charge measured by the Si detectors.
   Double_t    MumonSiPeak;                 ///< MUMON Si Peak 
   Double_t    MumonSiX;                    ///< Beam X Position on MUMON measured by the Si detectors. [mm]
   Double_t    MumonSiwX;                   ///< Beam X Width on MUMON measured by the Si detectors. [mm]
   Double_t    MumonSiY;                    ///< Beam Y Position on MUMON measured by the Si detectors. [mm]
   Double_t    MumonSiwY;                   ///< Beam Y Width on MUMON measured by the Si detectors. [mm]
   Double_t    MumonICTotalQ;               ///< MUMON Total Charge measured by the ionization chambers.
   Double_t    MumonICPeak;                 ///< MUMON Peak measured by the ionization chambers.
   Double_t    MumonICX;                    ///< Beam X Position on MUMON measured by the ionization chambers [mm]
   Double_t    MumonICwX;                   ///< Beam X Width on MUMON measured by the ionization chambers [mm]
   Double_t    MumonICY;                    ///< Beam Y Position on MUMON measured by the ionization chambers [mm]
   Double_t    MumonICwY;                   ///< Beam Y Width on MUMON measured by the ionization chambers [mm]
   Double_t    OTRLightYield;               ///< Light Yield of OTR
   Double_t    OTRX;                        ///< Beam X Position measured by the OTR. [mm]
   Double_t    OTRwX;                       ///< Beam X Width measured by the OTR. [mm]
   Double_t    OTRY;                        ///< Beam Y Position measured by the OTR. [mm]
   Double_t    OTRwY;                       ///< Beam Y Width measured by the OTR. [mm]
   Double_t    OTRXError;                   ///< Error of OTRX [mm]
   Double_t    OTRwXError;                  ///< Error of OTRwX [mm]
   Double_t    OTRYError;                   ///< Error of OTRY [mm]
   Double_t    OTRwYError;                  ///< Error of OTRwY [mm]
   Int_t       GoodGPSFlag;                 ///< Status Flag of GPSs. 1 = GOOD. 0 = BAD.
   Int_t       TriggerFlag;                 ///< Status Flag of Beam Trrigger. 1 = GOOD. 0 = BAD.
   Int_t       SpillFlag;                   ///< Status Flag of Spill. 1 = GOOD. 0 = BAD.
   Int_t       GoodSpillFlag;               ///< Status Flag of Spill. This Flag contains all information above and should be used to definde the flag is suitable for physics analysis or not. 0 = BAD.  Any others starts depending on run conditions.
   Double_t    TargetEfficiency[3];         ///< Efficiency of the Trager.

   TOtherData();
   TOtherData(const TOtherData & );
   virtual ~TOtherData();

   ClassDef(TOtherData,2); // Generated by MakeProject.
};

public:
// Data Members.
   Int_t       BeamRunNumber;    ///< Beam Run Number in neutrino beamline.
   Int_t       SpillNumber;      ///< Spill Number in 32 bit.
   Int_t       GPS1TriggerTime;    ///< GPS1 Trigger Time (Unix Time in sec)
   Int_t       GPS1TriggerTimeNanoSecond;    ///< GPS1 Trigger Time for sub-second (in nano sec)
   Double_t    CT5ProtonsPerSpill;           ///< Number of Protons in the SPILL counted by CT5, which is the most downstream charge transferer. [POT]
   Double_t    CT5ProtonsPerBunch[8];        ///< Number of Protons in the BUNCH (1st-8th) counted by CT5, which is the most downstream charge transferer. [POT]
   Double_t    CT5BeamBunchTiming[8];        ///< Timing of Proton beam BUNCH (1st-8th) measured by CT5, which is the most downstream charge transferer. Time 0 is Trigger Time. [micro sec]
   Int_t       CT5BeamBunchFlag[8];          ///< Flag describing that the Proton Beam is detected by CT5 or not in the BUNCH (1st-8th). 1 = extracted, 0 = not extracted.
   Double_t    OfficialProtonsPerSpill;      ///< Number of Protons in the SPILL for the official analysis, normally CT5. [POT]
   Double_t    OfficialCTNumber;             ///< Gives the CT used for the POT calculation.  Usually CT5.
   Double_t    OfficialProtonsPerBunch[8];    ///< Number of Protons in the BUNCH (1st-8th) counted in the official analysis, normally from CT5. [POT]
   Double_t    OfficialBeamBunchTiming[8];    ///< Timing of Proton beam BUNCH (1st-8th) in the official analysis, usually CT5. Time 0 is Trigger Time. [micro sec]
   Int_t       OfficialBeamBunchFlag[8];      ///< Flag describing that the Proton Beam is detected or not in the BUNCH (1st-8th) for the official CT for a given spill. 1 = extracted, 0 = not extracted.
   Int_t       OfficialBeamFlag;              ///< Flag describing that the Proton Beam is detected or not for the official CT for a given spill. 1 = extracted, 0 = not extracted.
   Double_t    Horn1CurrentSum;               ///< Horn1 Current. [kA]
   Double_t    Horn2CurrentSum;               ///< Horn2 Current. [kA]
   Double_t    Horn3CurrentSum;               ///< Horn3 Current. [kA]
   Int_t       GoodSpillFlag;                 ///< Good Spill Flag. Values >=1 = Good Spill, with 1 being at 250 kA, 2 at 200 kA, 100 at 0 kA, etc.. Suitable for analysis., 0 = Bad Spill. Improper for physics analysis., Flag == 99 is applied for Horn-off run in early Run 3 (early 2012) (need to check if this is still valid).
   TString     BSDVersion;                    ///< BSD version information. Currently "v01" is the newest.
   ND::TBeamSummaryDataModule::TBeamSummaryData::TOtherData OtherData;                     //

   TBeamSummaryData();
   TBeamSummaryData(const TBeamSummaryData & );
   virtual ~TBeamSummaryData();

   ClassDef(TBeamSummaryData,2); // Generated by MakeProject.
};

public:
// Data Members.
   Int_t       fBeamSummaryDataStatus;    ///< Flag : Beam Summary Data is available or not in the spill. 1 = BSD is available. 0 is not.
   Int_t       fND280Spill;               ///< ND280 Spill Number. 16 bit.
   TClonesArray* fBeamSummaryData;          ///< Data structure which contains beam summary data information.
   int           fTotalEvents;              //
   int           fEventsWithNoBSD;          //

   TBeamSummaryDataModule();
   TBeamSummaryDataModule(const TBeamSummaryDataModule & );
   virtual ~TBeamSummaryDataModule();

   ClassDef(TBeamSummaryDataModule,2); // Generated by MakeProject.
};
} // namespace
#endif
