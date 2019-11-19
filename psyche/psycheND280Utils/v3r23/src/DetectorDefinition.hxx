#ifndef DetectorDefinition_h
#define DetectorDefinition_h

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <typeinfo>
#include "TVector3.h"

namespace DetDef {

  //----- Detector volume definitions ---

  /// Minimum of the FGD1 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t fgd1min[3];

  /// Maximum of the FGD1 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t fgd1max[3];

  /// Minimum of the FGD2 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t fgd2min[3];

  /// Maximum of the FGD1 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t fgd2max[3];

  /// Minimum of the P0D det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t p0dmin[3];

  /// Maximum of the P0D det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t p0dmax[3];

  /// Minimum of the DsECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t dsecalmin[3];

  /// Maximum of the DsECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t dsecalmax[3];

  /// Minimum of the Left BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalLmin[3];

  /// Maximum of the Left BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalLmax[3];

  /// Minimum of the Right BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalRmin[3];

  /// Maximum of the Right BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalRmax[3];

  /// Minimum of the Top BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalTLmin[3];

  /// Maximum of the Top BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalTLmax[3];

  /// Minimum of the Top BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalTRmin[3];

  /// Maximum of the Top BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalTRmax[3];

  /// Minimum of the Bottom BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalBLmin[3];

  /// Maximum of the Bottom BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalBLmax[3];

  /// Minimum of the Bottom BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalBRmin[3];

  /// Maximum of the Bottom BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalBRmax[3];

  /// Minimum of the Left P0DECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t pecalLmin[3];

  /// Maximum of the Left P0DECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t pecalLmax[3];

  /// Minimum of the Right P0DECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t pecalRmin[3];

  /// Maximum of the Right P0DECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t pecalRmax[3];

  /// Minimum of the Top P0DECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t pecalTLmin[3];

  /// Maximum of the Top P0DECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t pecalTLmax[3];
  
  /// Minimum of the Top P0DECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t pecalTRmin[3];

  /// Maximum of the Top P0DECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t pecalTRmax[3];

  /// Minimum of the Bottom P0DECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t pecalBLmin[3];

  /// Maximum of the Bottom P0DECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t pecalBLmax[3];

  /// Minimum of the Bottom P0DECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t pecalBRmin[3];

  /// Maximum of the Bottom P0DECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t pecalBRmax[3];

  /// Minimum of the Left SMRD det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t smrd15Lmin[3];

  /// Maximum of the Left SMRD det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t smrd15Lmax[3];

  /// Minimum of the Left SMRD det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t smrd6Lmin[3];

  /// Maximum of the Left SMRD det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t smrd6Lmax[3];

 /// Minimum of the Left SMRD det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t smrd78Lmin[3];

  /// Maximum of the Left SMRD det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t smrd78Lmax[3];


  /// Minimum of the Right SMRD det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t smrd15Rmin[3];

  /// Maximum of the Right SMRD det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t smrd15Rmax[3];

  /// Minimum of the Right SMRD det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t smrd6Rmin[3];

  /// Maximum of the Right SMRD det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t smrd6Rmax[3];

  /// Minimum of the Right SMRD det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t smrd78Rmin[3];

  /// Maximum of the Right SMRD det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t smrd78Rmax[3];


  /// Minimum of the Bottom SMRD det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t smrdBLmin[3];

  /// Maximum of the Bottom SMRD det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t smrdBLmax[3];

  /// Minimum of the Bottom SMRD det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t smrdBRmin[3];

  /// Maximum of the Bottom SMRD det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t smrdBRmax[3];


  /// Minimum of the Top SMRD det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t smrdTLmin[3];

  /// Maximum of the Top SMRD det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t smrdTLmax[3];

  /// Minimum of the Top SMRD det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t smrdTRmin[3];

  /// Maximum of the Top SMRD det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t smrdTRmax[3];


  /// Minimum of the TPC1 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tpc1min[3];

  /// Maximum of the TPC1 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tpc1max[3];

  /// Minimum of the TPC2 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tpc2min[3];

  /// Maximum of the TPC2 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tpc2max[3];

  /// Minimum of the TPC3 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tpc3min[3];

  /// Maximum of the TPC3 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tpc3max[3];
  
  /// Minimum of overall tracker envelope: FGDs+TPCs + their dead material
  extern Float_t trackerAllmin[3];
  
  /// Maximum of overall tracker envelope: FGDs+TPCs + their dead material
  extern Float_t trackerAllmax[3];
  
  // FGD scintillating bars width
  extern Float_t fgdBarWidth;

  // FGD2 water bags width
  extern Float_t fgdWaterWidth;

  /// FGD2 scintillating bars center Z positions in MC and DATA geometry
  extern Float_t fgd2BarCenterMC[14];
  extern Float_t fgd2BarCenterDATA[14];
  
  extern Float_t fgd2BarCenter[14];
  
  extern Float_t fgd1BarCenter[30];

  /// FGD2 water bags center Z positions in MC and DATA geometry
  extern Float_t fgd2WaterCenterMC[6];
  extern Float_t fgd2WaterCenterDATA[6];
  
  extern Float_t fgd2WaterCenter[6];

  // FGD XY module widths
  extern Float_t fgdXYModuleWidth;
  extern Float_t fgdXYAirWidth;
  extern Float_t fgdXYBarWidth;
  extern Float_t fgdXYMiddleGlueWidth;
  extern Float_t fgdXYGlueWidth;
  extern Float_t fgdXYSkinWidth;

  // FGD water module widths
  extern Float_t fgdWaterModuleWidth;
  extern Float_t fgdWaterAirWidth;
  extern Float_t fgdWaterPCWidth;
  extern Float_t fgdWaterGlueWidth;
  extern Float_t fgdWaterSkinWidth;



  /// Production 6-era DATA geometry for ECals
  /// Alignment is applied for the data but as-bulit MC geometry has not yet been changed to account for significant shifts
  /// Minimum of the Left BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalLmin_p6_data[3];

  /// Maximum of the Left BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalLmax_p6_data[3];

  /// Minimum of the Right BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalRmin_p6_data[3];

  /// Maximum of the Right BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalRmax_p6_data[3];

  /// Minimum of the Top BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalTLmin_p6_data[3];

  /// Maximum of the Top BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalTLmax_p6_data[3];

  /// Minimum of the Top BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalTRmin_p6_data[3];

  /// Maximum of the Top BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalTRmax_p6_data[3];
  
  /// Minimum of the Bottom BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalBLmin_p6_data[3];

  /// Maximum of the Bottom BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalBLmax_p6_data[3];

  /// Minimum of the Bottom BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalBRmin_p6_data[3];

  /// Maximum of the Bottom BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalBRmax_p6_data[3];
  
  /// Dump volume definitions
  void DumpVolumes();
  
  /// Dump FGD2 info
  void DumpFGDInfo();
}

#endif
