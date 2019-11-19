#ifndef FiducialVolumeDefinition_h
#define FiducialVolumeDefinition_h

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <typeinfo>
#include "TVector3.h"

namespace FVDef {

  //----- FiducialVolume volume definitions ---

  /// Fiducial volume cut for FGD1. See definition in StandardActions.cxx for
  /// default value. This is the amount by which DetDef::fgd1min is shrunk
  /// in the FiducialCut function.
  extern Float_t FVdefminFGD1[3];

  /// Fiducial volume cut for FGD1. See definition in StandardActions.cxx for
  /// default value. This is the amount by which DetDef::fgd1max is shrunk
  /// in the FiducialCut function.
  extern Float_t FVdefmaxFGD1[3];

  /// Fiducial volume cut for FGD2. See definition in StandardActions.cxx for
  /// default value. This is the amount by which DetDef::fgd2min is shrunk
  /// in the FiducialCut function.
  extern Float_t FVdefminFGD2[3];

  /// Fiducial volume cut for FGD2. See definition in StandardActions.cxx for
  /// default value. This is the amount by which DetDef::fgd2max is shrunk
  /// in the FiducialCut function.
  extern Float_t FVdefmaxFGD2[3];

  /// Fiducial volume cut for P0D. See definition in StandardActions.cxx for
  /// default value. This is the amount by which DetDef::p0dmin is shrunk
  /// in the FiducialCut function.
  extern Float_t FVdefminP0D[3];

  /// Fiducial volume cut for P0D. See definition in StandardActions.cxx for
  /// default value. This is the amount by which DetDef::p0dmax is shrunk
  /// in the FiducialCut function.
  extern Float_t FVdefmaxP0D[3];

	/// Fiducial volume cut for stopping in TrackerECal. Used to compute momentum by range. 
	extern Float_t FVdefminTECALLeft[3];
	extern Float_t FVdefmaxTECALLeft[3];
	extern Float_t FVdefminTECALRight[3];
	extern Float_t FVdefmaxTECALRight[3];
	extern Float_t FVdefminTECALTop[3];
	extern Float_t FVdefmaxTECALTop[3];
	extern Float_t FVdefminTECALBottom[3];
	extern Float_t FVdefmaxTECALBottom[3];

	/// Fiducial volume cut for stopping in SMRD. Used to compute momentum by range. 
	extern Float_t FVdefminSMRDLeft[3];
	extern Float_t FVdefmaxSMRDLeft[3];
	extern Float_t FVdefminSMRDRight[3];
	extern Float_t FVdefmaxSMRDRight[3];
	extern Float_t FVdefminSMRDTop[3];
	extern Float_t FVdefmaxSMRDTop[3];
	extern Float_t FVdefminSMRDBottom[3];
	extern Float_t FVdefmaxSMRDBottom[3];

  /// Dump Fiducial Volume definitions
  void DumpFV();
}

#endif
