#ifndef VERSIONING_HXX_SEEN
#define VERSIONING_HXX_SEEN
#include "oaAnalysisReaderProjectHeaders.h"

/// This file contains pre-processor directives related to the
/// oaAnalysis file format that was used to generate the  oaAnalysisReader classes
/// Depending on the file format, different bits of highland need to
/// be conditionally compiled.
///
/// oaAnalysisReader advertises a few low-level directives. This file
/// defines higher-level directives (related to specific features that
/// are enabled/disabled in each version), to make using the directives
/// easier and more transparent.
///
/// http://www.t2k.org/nd280/datacomp/production005)
///
/// Respin 5D: this is a finished sand-MC production, using software version v10r11p11.  Users should use respin 5D for their sand interaction MC samples.
/// Respin 5E: this is a finished full MC production, using software version v10r11p17.
/// Respin 5F: this is an finished full MC and full data production, using software version v10r11p21 (and v10r11p23).
///            The data reprocessing includes improvements to the calibration constants.  Users should use this respin for their data and MC samples.
/// Respin 5G: this is a finished spill data production, using software version v10r11p27.  This processing was done at request of pi0 calibration task force.
///            It includes improvements to p0dRecon aimed at fixing data/MC discrepancies seen in pi0 analysis.
/// Respin 5H: this is a respin for Read Data Run 5 (First Pass) only, using software version v10r11p23 and updated Beam Summary Data Files (of Jul 5, 2014).
///
/// http://www.t2k.org/nd280/datacomp/production006/production006
///
/// Respin 6A: this is a stopped,failed production, using software version v11r29.  The production was stopped because of several serious bugs (989: 990: 992:995).
/// Respin 6B: this is an ongoing production, using software version v11r31. For Real Data run 5, only run 55 was processed as First Pass with early Beam Summary Data Files.
///            Analysers should use this respin for MCP.
/// Respin 6C: this is a v11r31 respin for Real Data runs 5 and 6. The First Pass Processing (fpp) used pre-existing calibration tables and early BSD.
///            Partially Calibration 1 (pc1) contains updated calibration tables, updated BSD but no inter-detector timing.
///            Real Data Processing (rdp) contains the full set of calibrations, corrections, BSD and inter-detector timing. For run6 the rdp has incomplete constants and BSD.
/// Respin 6D: this is a MC only production using nd280 v11r31 on top of neut 5.3.2.2 (additional proton FSI)
/// Respin 6E: RDP only using v11r31p5 which features new P0D ECal timing calibrations. Run 7 FPP goes as this respin.
/// Respin 6F: this an RDP and MCP respin using v11r31 with nd280Analysis v1r47p1 prepended to the CMTPATH.
///            It features additional FGD vertex information frozen into oaAnalysis v5r29p25. The RDP files are missing the BSD.
/// Respin 6G: this is an RDP-only respin using nd280 v11r31p7. It features additional FGD vertex information frozen into oaAnalysis v5r29p25.
/// Respin 6H: this is an MCP-only respin using nd280 v11r31p9 (same as v11r31p7 but with incorrect software tags). It features the trexRecon replacement of tpcRecon and is
///            intended both for trexRecon validation and tpcGas analysis.
/// Respin 6I: this is an RDP-only respin using nd280 v11r37 (same as v11r31p9 but with correct software tags). It features the trexRecon replacement of tpcRecon and is
///            intended both for trexRecon validation and tpcGas analysis.
/// Respin 6J: this is a Neut MCP-only respin using nd280 v11r31p11. It features the new NeutG4CascadeInterface_QGSP_BERT physics list for nd280mc.
/// Respin 6K: this is an RDP-only respin using nd280 v11r31p5.  This has correct ECAL timing corrections (and other ECAL corrections) for runs 5 and 6.
///            For runs 1-4 the output is the same as respin 6E and has just been copied over from 6E. Analysers should use this respin for RDP. 
/// Respin 6L: this is an MCP-only production using nd280 v11r31p13 to produce run6 and run7 magnet MCP with the 2015-08 baseline (featuring 5/6 fgd2 water panels).


/// The table below shows the versions 

/// prod       nd280      nd280Analysis     oaAnalysis 
///------------------------------------------------------
/// 5D         v10r11                       v5r11       
/// 5E         v10r11p17                    v5r15p1
/// 5F         v10r11p19                    v5r15p3
/// 5G         v10r11p27                    v5r15p3
///
/// 6A         v11r29     v1r39             v5r29p17 
/// 6B-6D      v11r31     v1r43             v5r29p19     *
///            v11r31p1   v1r45             v5r29p19   
///            v11r31p3   v1r43p1           v5r29p21     
/// 6E, 6K     v11r31p5   v1r43p1           v5r29p21     *  Vector to store the ID of the Trajectory that created the hit in FGD1
/// 6F         v11r31     v1r47p1           v5r29p25     *  (nd280Analysis v1r47p1 preappend to CMTPATH)
/// 6G         v11r31p7   v1r47p1           v5r29p25        fgdVA_verNearQ ... _rect
/// 6H         v11r31p9   v1r43p5           v5r31p9      *
/// 6I         v11r37     v1r43p5           v5r31p9
/// 6J         v11r31p11  v1r43             v5r29p19 
/// 6L         v11r31p13  v1r47p1           v5r29p25
/// 6M         v11r31p5     ?               v5r29p21


/// Show those are the readers we need 

/// productions  oaAnalysis           oaAnalysisReader
///--------------------------------------------
/// 5D           v5r11                prod5D   
/// 5E           v5r15p1               
/// 5F, 5G       v5r15p3              prod5F
/// 
/// 6B-6D_6J     v5r29p19             prod6B 
/// 6E-6K        v5r29p21             prod6E
/// 6F-6G-6L     v5r29p25             prod6G
/// 6H-6I        v5r31p9  (TREX)      prod6TREx 

///--------------------------------------------------------------

/// nd280 version for production 5D is v10r11p0 to v10r11p15 
#define VERSION_PROD5D (BEFORE_ANATOOLS_FILE(10,11,17))

/// nd280 version for production 5E is v10r11p17 
#define VERSION_PROD5E (EQUALS_ANATOOLS_FILE(10,11,17))

/// nd280 version for production 5F goes from v10r11p19 to v10r11p23
#define VERSION_PROD5F (!BEFORE_ANATOOLS_FILE(10,11,19) && BEFORE_ANATOOLS_FILE(10,11,25))

/// nd280 version for production 5G is v10r11p27
#define VERSION_PROD5G (EQUALS_ANATOOLS_FILE(10,11,27))

/// nd280 version for pre production 6 goes from v11r17 to v11r27
#define VERSION_PROD6PRE (!BEFORE_ANATOOLS_FILE(11,17,0) && BEFORE_ANATOOLS_FILE(11,29,0))

/// nd280 version for Production 6A is v11r29
#define VERSION_PROD6A (EQUALS_ANATOOLS_FILE(11,29,0))

/// nd280 version for Production 6B is v11r31
#define VERSION_PROD6B (EQUALS_ANATOOLS_FILE(11,31,0))

/// v11r31p9 was used for TREx validation/production based on prod-6 for the rest of
/// the software (only reconstruction was updated for TREx usage)
/// New oaAnalysis module and new reconstruction was introduced:
#define VERSION_PROD6HI (EQUALS_ANATOOLS_FILE(11,31,9))

/// Production 7 development version
#define VERSION_PROD7_DEVEL (!BEFORE_ANATOOLS_FILE(12,0,0))

///--------------------------------------------------------------


/// For production 6 pre-production files, we need to disable all corrections.
#define VERSION_DISABLE_CORRECTIONS VERSION_PROD6PRE

/// For production 6 pre-production or production 6A files, we need to disable all production dependent corrections
#define VERSION_DISABLE_PROD_CORRECTIONS (VERSION_PROD6PRE || VERSION_PROD6A)

/// The B-field distortion refit info changed interface between P5 and P6.
/// In P5F, we had Charge, Momentum, Position and Direction saved
#define VERSION_HAS_BFIELD_REFIT_FULL VERSION_PROD5F

/// In P6, we have only the B-field refit Momentum (and it changed name).
#define VERSION_HAS_BFIELD_REFIT_BASIC (!BEFORE_ANATOOLS_FILE(11,17,0))

/// Whether there is any B-field information. Although the momentum name is
/// different in P5/P6 oaAnalysis files, we kept the same name in highland.
/// Note that there is a gap between P5 and P6 pre-production files where
/// no B-field info was saved...
#define VERSION_HAS_BFIELD_REFIT (VERSION_HAS_BFIELD_REFIT_FULL || VERSION_HAS_BFIELD_REFIT_BASIC )

/// TPC E-field refit information was added in v11r17.
#define VERSION_HAS_EFIELD_REFIT (!BEFORE_ANATOOLS_FILE(11,17,0))

/// P5 files didn't store the ECal LLR (PID) variables directly, and they had
/// to be calculated using a tool in nd280AnalysisTools. In P6, they are
/// stored directly in the oaAnalysis file.
#define VERSION_HAS_ECAL_LLR (!BEFORE_ANATOOLS_FILE(11,0,0))

/// P5 files had CT5POT (and needed a correction to use CT4POT for some beam runs). P6 files
/// have an OfficalPOT variable.
#define VERSION_HAS_OFFICIAL_POT (!BEFORE_ANATOOLS_FILE(11,17,0))

/// P6 files have refits of each global track for electron/muon/proton hypotheses
/// in both the forwards and backwards directions.
#define VERSION_HAS_REVERSED_REFITS (!BEFORE_ANATOOLS_FILE(11,17,0))

/// P6 files have momentum from range estiamtates for a global track, assuming electron/muon/proton hypotheses
/// in both the forwards and backwards directions.
#define VERSION_HAS_PRANGE_ESTIMATES (!BEFORE_ANATOOLS_FILE(11,17,0))

/// P6 files have node times updated with t_vs_dist fit results
/// independently for each sub-detector
#define VERSION_HAS_TIME_FITS (!BEFORE_ANATOOLS_FILE(11,17,0))

/// P6 files have have both vertex (start or entrance) and end (stop or exit) activities
/// calculated for FGD tracks
#define VERSION_HAS_FGD_VERTEX_AND_END_ACTIVITY (!BEFORE_ANATOOLS_FILE(11,17,0))

/// P6 files have or to store information for single hits in FgdTimeBin
#define VERSION_HAS_FGD_HIT_INFO (!BEFORE_ANATOOLS_FILE(11,17,0))

/// P6 iteration with Vector to store the ID of the Trajectory that created the hit in FGD
#define VERSION_HAS_FGD_HIT_TRUE_TRAJ_ID (!BEFORE_ANATOOLS_FILE(11,31,3))

/// P6 files use the same method for main track fitting and alternates fitting
#define VERSION_HAS_EQUIVALENT_MAIN_AND_ALT_FITS (!BEFORE_ANATOOLS_FILE(11,17,0))

/// For preproduction 6 the number of nodes for TPC objects was 2
#define VERSION_HAS_TWO_TPC_NODES VERSION_PROD6PRE

/// For production 6 P0D objects got hits/nodes averaged time stamps 
#define VERSION_HAS_P0D_AVERAGED_TIME (VERSION_PROD6A || VERSION_PROD6B) 

/// For production 6 FGDTimeBins objects have hit information
#define VERSION_HAS_FGDTIMEBIN_ALL_HITS (VERSION_PROD6A || VERSION_PROD6B) 

/// This is to check whether some obsolete ECal PID variables were deleted 
#define VERSION_HAS_REMOVED_OBSOLETE_ECAL_PID_VARS (VERSION_PROD6HI  || VERSION_PROD7_DEVEL)

/// This is to check whether a version has some obsolete TruthVars removed
/// (those related to detector intersection) 
#define VERSION_HAS_REMOVED_OBSOLETE_TRUTH_SUBDETECTOR_NON_TRACE_VARS (VERSION_PROD6HI || VERSION_PROD7_DEVEL)


/// For production 7 iteration substitute the NNodes (later treated as a figure
/// of merit for track quality)
/// with the actual number of horizontal and vertical clusters used in the fit
/// (as recommended by TREx group and can be indeed
/// considered as a proper characteristic of track quality)
#define VERSION_HAS_NUMBER_OF_FITTED_CLUSTERS (VERSION_PROD6HI || VERSION_PROD7_DEVEL)


#endif
