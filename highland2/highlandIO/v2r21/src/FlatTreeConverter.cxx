#include "FlatTreeConverter.hxx"
#include "BasicUtils.hxx"
#include "TreeConverterUtils.hxx"
#include "HighlandTreeConverterUtils.hxx"
#include "HighlandAnalysisUtils.hxx"
#include "Parameters.hxx"

// set true for avoiding error messages that might appear when using old flat trees
bool suppress_err_msg = true;

//********************************************************************
FlatTreeConverter::FlatTreeConverter(bool readRooTrackerVtx):RedoTreeConverter(){
//********************************************************************

  // If true the RooTrackerVtx tree will be read
  _readRooTrackerVtx = readRooTrackerVtx;

  _currentFileIndex=-1;
  _RooVtxEntryInCurrentInputFile=0;
  _entry_roo=0;

  _firstFile=true;
  _firstEntry=true;

  // Initialise all the variables. This is particularly important as some
  // branches many not exist in the flat tree.
  sBeamPOT           = 0;
  sBeamPOTCT4        = 0;
  sBeamSpill         = 0;
  sBeamSpillNumber   = 0;
  sBeamBeamRunNumber = 0;

  sDQND280Flag = 0;
  for(int i = 0; i < 7; i++) sDQDetFlag[i] = 0;
  
  // AnaTrigger related
  sTriggerFGDCosmic   = 0;
  sTriggerTripTCosmic = 0;
  for (int i = 0; i < 6; i++){
    sTriggerCTMTriggerPattern[i] = 0;
  }
  

  sNTrueVertices = 0;
  for (unsigned int i = 0; i < NMAXTRUEVERTICES; i++) {
    sTrueVertexNReconTracks[i]   = 0;
    sTrueVertexNReconVertices[i] = 0;
    sTrueVertexBunch[i]          = 0;
    sTrueVertexLeptonPDG[i]      = 0;
    sTrueVertexQ2[i]             = 0;
    sTrueVertexTargetMom[i]      = 0;
    sTrueVertexLeptonMom[i]      = 0;
    sTrueVertexProtonMom[i]      = 0;
    sTrueVertexPionMom[i]        = 0;
    sTrueVertexIsPauliBlocked[i] = 0;
    sTrueVertexIsCohOnH[i]       = 0;

    for (unsigned int j = 0; j < 3; j++) {
      sTrueVertexTargetDir[i][j] = 0;
      sTrueVertexLeptonDir[i][j] = 0;
      sTrueVertexProtonDir[i][j] = 0;
      sTrueVertexPionDir[i][j]   = 0;
    }

    for (int j = 0; j < Int_t(ParticleId::kLast)+1; j++) {
      sTrueVertexNPrimaryParticles[i][j]=0;
    }
    
  }

  for (unsigned int i = 0; i < NMAXPARTICLES; i++) {
    trReconPDG[i]                = 0;
    trNTRACKERs[i]               = 0;
    trLength[i]                  = 0;
    trRangeMomentumMuonFlip[i]   = 0;
    trRangeMomentumProtonFlip[i] = 0;
    trTrueParticleID[i]          = 0;

    trNTimeNodes[i]              = 0;
    for (unsigned int j = 0; j < NMAXTIMENODES; j++) {
       trTimeNodeDetector[i][j] = 0;
      trTimeNodeTimeStart[i][j] = 0;
        trTimeNodeTimeEnd[i][j] = 0;
    }
    
    for (int j = 0; j < 3; j++) {
      trDirectionAtVertex[i][j]   = 0;
      trDirectionStartFlip[i][j]  = 0;   //! 
      trDirectionEndFlip[i][j]    = 0;   //!
    }

    for (int j = 0; j < 3; j++) {
      trTpcUniqueID[i][j]      = 0;
      trTpcLength[i][j]        = 0;
      trTpcPullmu[i][j]        = 0;
      trTpcPullele[i][j]       = 0;
      trTpcPullp[i][j]         = 0;
      trTpcPullpi[i][j]        = 0;
      trTpcPullk[i][j]         = 0;
      trTpcRawdEdxMeas[i][j]   = 0;
      trTpcRawdEdxExpMu[i][j]  = 0;
      trTpcRawdEdxExpEle[i][j] = 0;
      trTpcRawdEdxExpP[i][j]   = 0;
      trTpcRawdEdxExpPi[i][j]  = 0;
      trTpcRawdEdxExpK[i][j]   = 0;
    }

    for (int j = 0; j < 2; j++) {
      trFgdUniqueID[i][j] = 0;

      trFgdHasFgdVA[i][j] = 0;
      trFgdHasFgdVA_fmode[i][j] = 0;
      trFgdTypeVA[i][j] = 0;

      trFgdHasFgdEA[i][j] = 0;
      trFgdHasFgdEA_fmode[i][j] = 0;
      trFgdTypeEA[i][j] = 0;
      trFgdEnd0x0[i][j] = 0;
      trFgdEnd1x1[i][j] = 0;
      trFgdEnd3x3[i][j] = 0;
      trFgdEnd5x5[i][j] = 0;
      trFgdEnd7x7[i][j] = 0;
    }

    for (int j = 0; j < (int)NECALSUBDETS; j++) {
      trECALUniqueID[i][j]                    = 0;
      trECALPIDMipPion[i][j]                  = 0;
      trECALContainment[i][j]                 = 0;
    }

    for (int j = 0; j < (int)NSMRDSUBDETS; j++) {
      trSMRDUniqueID[i][j] = 0;
      trSMRDEDeposit[i][j] = 0;
    }

    for (int j = 0; j < 1; j++) {
      trP0DUniqueID[i][j] = 0;
    }

    for (int j = 0; j < (int)NMAXTRACKERS; j++) {
      trTRACKERDetector[i][j]    = 0;
      trTRACKERNHits[i][j]       = 0;
      trTRACKERNNodes[i][j]      = 0;
      trTRACKERUniqueID[i][j]    = 0;
      trTRACKERLength[i][j]      = 0;
      trTRACKERCharge[i][j]      = 0;
      trTRACKERMomentum[i][j]    = 0;
      trTRACKERMomentumEnd[i][j] = 0;

      for (int k = 0; k < 3; k++) {
        trTRACKERDirectionStart[i][j][k] = 0;
        trTRACKERDirectionEnd[i][j][k] = 0;
      }

      for (int k = 0; k < 4; k++) {
        trTRACKERPositionStart[i][j][k] = 0;
        trTRACKERPositionEnd[i][j][k] = 0;
      }
    }
  }

  /// AnaLocalReconBunch information
  tecalReconCounter = 0;
  for (unsigned int i=0; i<NMAXTECALRECONOBJECTS; i++){
    tecalReconAverageHitTime          [i] = 0;
    tecalReconAverageZPosition        [i] = 0;
    tecalReconContainment             [i] = 0;
    tecalReconEFitResult              [i] = 0;
    tecalReconEFitUncertainty         [i] = 0;
    tecalReconFirstLayer              [i] = 0;
    tecalReconIsShowerLike            [i] = 0;
    tecalReconIsTrackLike             [i] = 0;
    tecalReconLastLayer               [i] = 0;
    tecalReconLikeMIPEM               [i] = 0;
    tecalReconLikeMIPPion             [i] = 0;
    tecalReconLikeEMHIP               [i] = 0;
    tecalReconLikeMIPEMLow            [i] = 0;
    tecalReconMatchingLike            [i] = 0;
    tecalReconMichelTagNDelayedCluster[i] = 0;
    tecalReconModule                  [i] = 0;
    tecalReconMostDownStreamLayerHit  [i] = 0;
    tecalReconMostUpStreamLayerHit    [i] = 0;
    tecalReconNHits                   [i] = 0;
    tecalReconNLayersHit              [i] = 0;
    tecalReconObjectLength            [i] = 0;
    tecalReconPIDAMR                  [i] = 0;
    tecalReconPIDAngle                [i] = 0;
    tecalReconPIDAsymmetry            [i] = 0;
    tecalReconPIDCircularity          [i] = 0;
    tecalReconPIDFBR                  [i] = 0;
    tecalReconPIDMaxRatio             [i] = 0;
    tecalReconPIDMeanPos              [i] = 0;
    tecalReconPIDShowerAngle          [i] = 0;
    tecalReconPIDShowerWidth          [i] = 0;
    tecalReconPIDTransverseChargeRatio[i] = 0;
    tecalReconPIDTruncatedMaxRatio    [i] = 0;
    tecalReconThrust                  [i] = 0;
    tecalReconTimeBunch               [i] = 0;
    tecalReconTotalHitCharge          [i] = 0;
    tecalReconTrueID                  [i] = 0;
    tecalReconTrueIDPrimary           [i] = 0;
    tecalReconTrueIDRecursive         [i] = 0;
    tecalReconTrueIDSingle            [i] = 0;
    tecalReconUniqueID                [i] = 0;
    tecalReconBunch                   [i] = 0;

    for (int j=0; j<3; j++){
      tecalReconPointing     [i][j] = 0;
      tecalReconThrustAxis   [i][j] = 0;
      tecalReconThrustOrigin [i][j] = 0;

    }
  }


  // Initialise all the branches. This is particularly important as some
  // branches many not exist in the flat tree.
  b_sIsMC              = NULL;
  b_sBeamPOT           = NULL;
  b_sBeamPOTCT4        = NULL;
  b_sBeamSpill         = NULL;
  b_sBeamSpillNumber   = NULL;
  b_sBeamBeamRunNumber = NULL;
  b_sDQND280Flag       = NULL;
  b_sDQDetFlag         = NULL;
 
  // AnaTrigger related
  b_sTriggerFGDCosmic         = NULL;
  b_sTriggerTripTCosmic       = NULL;
  b_sTriggerCTMTriggerPattern = NULL;
                                                         
  b_sTrueVertexLeptonPDG         = NULL;
  b_sTrueVertexLeptonDir         = NULL;
  b_sTrueVertexProtonDir         = NULL;
  b_sTrueVertexPionDir           = NULL;
  b_sTrueVertexLeptonMom         = NULL;
  b_sTrueVertexProtonMom         = NULL;
  b_sTrueVertexPionMom           = NULL;
  b_sTrueVertexIsPauliBlocked    = NULL;
  b_sTrueVertexIsCohOnH          = NULL;
  b_sTrueVertexNPrimaryParticles = NULL;
  b_sTrueVertexPionMom           = NULL;
  b_sTrueVertexTargetDir         = NULL;
  b_sTrueVertexTargetMom         = NULL;
                                                         
  b_trReconPDG = NULL;                                         
  b_trDirectionAtVertex = NULL;                                
                                                         
  b_trNTRACKERs = NULL;                                        
                                                         
  b_trTpcPullmu         = NULL;
  b_trTpcPullele        = NULL;
  b_trTpcPullp          = NULL;
  b_trTpcPullpi         = NULL;
  b_trTpcPullk          = NULL;
  b_trNTimeNodes        = NULL;
  b_trTimeNodeDetector  = NULL;
  b_trTimeNodeTimeStart = NULL;
  b_trTimeNodeTimeEnd   = NULL;
                                                         
  b_trTpcUniqueID  = NULL;
  b_trFgdUniqueID  = NULL;
  b_trECALUniqueID = NULL;
  b_trSMRDUniqueID = NULL;
  b_trP0DUniqueID  = NULL;
                                                         
  b_trTpcRawdEdxMeas   = NULL;
  b_trTpcRawdEdxExpMu  = NULL;
  b_trTpcRawdEdxExpEle = NULL;
  b_trTpcRawdEdxExpP   = NULL;
  b_trTpcRawdEdxExpPi  = NULL;
  b_trTpcRawdEdxExpK   = NULL;
                                                         
  b_trFgdHasFgdVA       = NULL;
  b_trFgdHasFgdVA_fmode = NULL;
  b_trFgdTypeVA         = NULL;
  b_trFgdHasFgdEA       = NULL;
  b_trFgdHasFgdEA_fmode = NULL;
  b_trFgdTypeEA         = NULL;
  b_trFgdEnd0x0         = NULL;
  b_trFgdEnd1x1         = NULL;
  b_trFgdEnd3x3         = NULL;
  b_trFgdEnd5x5         = NULL;
  b_trFgdEnd7x7         = NULL;
                                                         
  b_trP0DAvgTime = NULL;                                       
                                                         
  b_trTRACKERDetector       = NULL;
  b_trTRACKERNHits          = NULL;
  b_trTRACKERNNodes         = NULL;
  b_trTRACKERUniqueID       = NULL;
  b_trTRACKERLength         = NULL;
  b_trTRACKERDirectionStart = NULL;
  b_trTRACKERDirectionEnd   = NULL;
  b_trTRACKERPositionStart  = NULL;
  b_trTRACKERPositionEnd    = NULL;
  b_trTRACKERCharge         = NULL;
  b_trTRACKERMomentum       = NULL;
  b_trTRACKERMomentumEnd    = NULL;
                                                         
  b_trDirectionStartFlip      = NULL;
  b_trDirectionEndFlip        = NULL;
  b_trRangeMomentumMuonFlip   = NULL;
  b_trRangeMomentumProtonFlip = NULL;
  b_trMomentumAtVertex        = NULL;
  b_trLength                  = NULL;                                                       
                                                         
  b_trECALPIDMipPion  = NULL;
  b_trECALContainment = NULL;
                  
  //  SMRD
  b_trSMRDEDeposit = NULL;
                                                         
  b_tecalReconCounter                  = NULL;
  b_tecalReconAverageHitTime           = NULL;
  b_tecalReconAverageZPosition         = NULL;
  b_tecalReconContainment              = NULL;
  b_tecalReconEFitResult               = NULL;
  b_tecalReconEFitUncertainty          = NULL;
  b_tecalReconFirstLayer               = NULL;
  b_tecalReconIsShowerLike             = NULL;
  b_tecalReconIsTrackLike              = NULL;
  b_tecalReconLastLayer                = NULL;
  b_tecalReconLikeMIPEM                = NULL;
  b_tecalReconLikeMIPPion              = NULL;
  b_tecalReconLikeEMHIP                = NULL;
  b_tecalReconLikeMIPEMLow             = NULL;
  b_tecalReconMatchingLike             = NULL;
  b_tecalReconMichelTagNDelayedCluster = NULL;
  b_tecalReconModule                   = NULL;
  b_tecalReconMostDownStreamLayerHit   = NULL;
  b_tecalReconMostUpStreamLayerHit     = NULL;
  b_tecalReconNHits                    = NULL;
  b_tecalReconNLayersHit               = NULL;
  b_tecalReconObjectLength             = NULL;
  b_tecalReconPIDAMR                   = NULL;
  b_tecalReconPIDAngle                 = NULL;
  b_tecalReconPIDAsymmetry             = NULL;
  b_tecalReconPIDCircularity           = NULL;
  b_tecalReconPIDFBR                   = NULL;
  b_tecalReconPIDMaxRatio              = NULL;
  b_tecalReconPIDMeanPos               = NULL;
  b_tecalReconPIDShowerAngle           = NULL;
  b_tecalReconPIDShowerWidth           = NULL;
  b_tecalReconPIDTransverseChargeRatio = NULL;
  b_tecalReconPIDTruncatedMaxRatio     = NULL;
  b_tecalReconPointing                 = NULL;
  b_tecalReconThrust                   = NULL;
  b_tecalReconThrustAxis               = NULL;
  b_tecalReconThrustOrigin             = NULL;
  b_tecalReconTimeBunch                = NULL;
  b_tecalReconTotalHitCharge           = NULL;
  b_tecalReconTrueID                   = NULL;
  b_tecalReconTrueIDPrimary            = NULL;
  b_tecalReconTrueIDRecursive          = NULL;
  b_tecalReconTrueIDSingle             = NULL;
  b_tecalReconUniqueID                 = NULL;
  b_tecalReconBunch                    = NULL;

  RunID=-1;
  SubrunID=-1;
  EventID=-1;

}

//********************************************************************
bool FlatTreeConverter::Initialize(){
//********************************************************************
 
   // Use corrected values or raw values
   _useCorrectedValues = ND::params().GetParameterI("highlandIO.ReadFlatTree.UseCorrectedValues");

   /// Is AnaLocalReconBunch information enabled in the parameters file?
   _isUsingReconDirFGDOnly = ND::params().GetParameterI("highlandIO.FlatTree.UseReconDirFGDOnly"    );
   _isUsingReconDirP0D     = ND::params().GetParameterI("highlandIO.FlatTree.UseReconDirP0D"        );
   _isUsingReconDirPECAL   = ND::params().GetParameterI("highlandIO.FlatTree.UseReconDirP0DECal"    );
   _isUsingReconDirTECAL   = ND::params().GetParameterI("highlandIO.FlatTree.UseReconDirTrackerECal");


   return RedoTreeConverter::Initialize();
}

//********************************************************************
void FlatTreeConverter::DefineBranches(){
//********************************************************************

  RedoTreeConverter::DefineBranches();

  // The branches commented below are not read becouse they are not currently needed by any analysis. 
  // This is equivalent to setting their status to 0, as it was done in the previous version


   anaUtils::ConfigureTreeBranch(fChain, "sIsMC", &sIsMC, &b_sIsMC);
   anaUtils::ConfigureTreeBranch(fChain, "sEventTime", &sEventTime, &b_sEventTime);
   anaUtils::ConfigureTreeBranch(fChain, "sNTotalTrueTracks", &sNTotalTrueParticles, &b_sNTotalTrueParticles);
   anaUtils::ConfigureTreeBranch(fChain, "sNTotalTrueVertices", &sNTotalTrueVertices, &b_sNTotalTrueVertices);

   anaUtils::ConfigureTreeBranch(fChain, "sInputFileIndex", &sInputFileIndex, &b_sInputFileIndex);
   anaUtils::ConfigureTreeBranch(fChain, "sRooVtxEntry", &sRooVtxEntry, &b_sRooVtxEntry);

   //   anaUtils::ConfigureTreeBranch(fChain, "sPOTSincePreviousSpill", &sPOTSincePreviousSpill, &b_sPOTSincePreviousSpill);
   anaUtils::ConfigureTreeBranch(fChain, "sBeamPOT", &sBeamPOT, &b_sBeamPOT);
#if !VERSION_HAS_OFFICIAL_POT
   anaUtils::ConfigureTreeBranch(fChain, "sBeamPOTCT4", &sBeamPOTCT4, &b_sBeamPOTCT4);
#endif
   anaUtils::ConfigureTreeBranch(fChain, "sBeamSpill", &sBeamSpill, &b_sBeamSpill);
   anaUtils::ConfigureTreeBranch(fChain, "sBeamSpillNumber", &sBeamSpillNumber, &b_sBeamSpillNumber);
   anaUtils::ConfigureTreeBranch(fChain, "sBeamGoodSpill", &sBeamGoodSpill, &b_sBeamGoodSpill);
   anaUtils::ConfigureTreeBranch(fChain, "sBeamBeamRunNumber", &sBeamBeamRunNumber, &b_sBeamBeamRunNumber);
   anaUtils::ConfigureTreeBranch(fChain, "sDQGoodDaq", &sDQGoodDaq, &b_sDQGoodDaq);
   anaUtils::ConfigureTreeBranch(fChain, "sDQND280Flag", &sDQND280Flag, &b_sDQND280Flag);
   anaUtils::ConfigureTreeBranch(fChain, "sDQDetFlag", sDQDetFlag, &b_sDQDetFlag);
   
   anaUtils::ConfigureTreeBranch(fChain, "sTriggerFGDCosmic",   &sTriggerFGDCosmic,   &b_sTriggerFGDCosmic);
   anaUtils::ConfigureTreeBranch(fChain, "sTriggerTripTCosmic", &sTriggerTripTCosmic, &b_sTriggerTripTCosmic);
   anaUtils::ConfigureTreeBranch(fChain, "sTriggerCTMTriggerPattern", sTriggerCTMTriggerPattern, &b_sTriggerCTMTriggerPattern);
   
   anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexRooVtxIndex", sTrueVertexRooVtxIndex, &b_sTrueVertexRooVtxIndex);
   anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexRooVtxEntry", sTrueVertexRooVtxEntry, &b_sTrueVertexRooVtxEntry);
   anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexReacCode", sTrueVertexReacCode, &b_sTrueVertexReacCode);
   anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexNTrueTracks", sTrueVertexNTrueParticles, &b_sTrueVertexNTrueParticles);
   //   anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexNReconTracks", sTrueVertexNReconTracks, &b_sTrueVertexNReconTracks);
   //   anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexNReconVertices", sTrueVertexNReconVertices, &b_sTrueVertexNReconVertices);
   //   anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexNBaryons", sTrueVertexNBaryons, &b_sTrueVertexNBaryons);
   anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexDetector", sTrueVertexDetector, &b_sTrueVertexDetector);
   anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexLeptonPDG", sTrueVertexLeptonPDG, &b_sTrueVertexLeptonPDG);
   anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexTargetPDG", sTrueVertexTargetPDG, &b_sTrueVertexTargetPDG);
   anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexNuEnergy", sTrueVertexNuEnergy, &b_sTrueVertexNuEnergy);
   //   anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexQ2", sTrueVertexQ2, &b_sTrueVertexQ2);
   anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexPosition", sTrueVertexPosition, &b_sTrueVertexPosition);
   anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexNuParentPDG", sTrueVertexNuParentPDG, &b_sTrueVertexNuParentPDG);
   anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexNuParentDecPoint", sTrueVertexNuParentDecPoint, &b_sTrueVertexNuParentDecPoint);
   anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexNuDir", sTrueVertexNuDir, &b_sTrueVertexNuDir);
   anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexTargetDir", sTrueVertexTargetDir, &b_sTrueVertexTargetDir);
   anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexLeptonDir", sTrueVertexLeptonDir, &b_sTrueVertexLeptonDir);
   anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexProtonDir", sTrueVertexProtonDir, &b_sTrueVertexProtonDir);
   anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexPionDir", sTrueVertexPionDir, &b_sTrueVertexPionDir);
   anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexTargetMom", sTrueVertexTargetMom, &b_sTrueVertexTargetMom);
   anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexLeptonMom", sTrueVertexLeptonMom, &b_sTrueVertexLeptonMom);
   anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexProtonMom", sTrueVertexProtonMom, &b_sTrueVertexProtonMom);
   anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexPionMom", sTrueVertexPionMom, &b_sTrueVertexPionMom);
   anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexIsPauliBlocked", sTrueVertexIsPauliBlocked, &b_sTrueVertexIsPauliBlocked);
   anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexIsCohOnH", sTrueVertexIsCohOnH, &b_sTrueVertexIsCohOnH);
   anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexNPrimaryParticles", sTrueVertexNPrimaryParticles, &b_sTrueVertexNPrimaryParticles);
 
   // True track info
   anaUtils::ConfigureTreeBranch(fChain, "sTrueTrackBunch", sTrueParticleBunch, &b_sTrueParticleBunch);
   anaUtils::ConfigureTreeBranch(fChain, "sTrueTrackPurity", sTrueParticlePurity, &b_sTrueParticlePurity);
   //   anaUtils::ConfigureTreeBranch(fChain, "sTrueTrackNReconTracks", sTrueParticleNReconTracks, &b_sTrueParticleNReconTracks);
  
   anaUtils::ConfigureTreeBranch(fChain, "sFgdTimeBinMinTime", sFgdTimeBinMinTime, &b_sFgdTimeBinMinTime);
   anaUtils::ConfigureTreeBranch(fChain, "sFgdTimeBinMaxTime", sFgdTimeBinMaxTime, &b_sFgdTimeBinMaxTime);
   anaUtils::ConfigureTreeBranch(fChain, "sFgdTimeBinRawChargeSum1", sFgdTimeBinRawChargeSum1, &b_sFgdTimeBinRawChargeSum1);
   anaUtils::ConfigureTreeBranch(fChain, "sFgdTimeBinRawChargeSum2", sFgdTimeBinRawChargeSum2, &b_sFgdTimeBinRawChargeSum2);
   //   anaUtils::ConfigureTreeBranch(fChain, "sFgdTimeBinG4ID", sFgdTimeBinG4ID, &b_sFgdTimeBinG4ID);

   //   anaUtils::ConfigureTreeBranch(fChain, "NVertices", &NVertices, &b_NVertices);
   //   anaUtils::ConfigureTreeBranch(fChain, "vPrimaryIndex", &vPrimaryIndex, &b_vPrimaryIndex);
   //   anaUtils::ConfigureTreeBranch(fChain, "vBunch", &vBunch, &b_vBunch);
   //   anaUtils::ConfigureTreeBranch(fChain, "vPosition", &vPosition, &b_vPosition);
   //   anaUtils::ConfigureTreeBranch(fChain, "vVariance", &vVariance, &b_vVariance);
   //   anaUtils::ConfigureTreeBranch(fChain, "vChi2", &vChi2, &b_vChi2);
   //   anaUtils::ConfigureTreeBranch(fChain, "vNDOF", &vNDOF, &b_vNDOF);
   anaUtils::ConfigureTreeBranch(fChain, "vNReconTracks", &vNReconParticles, &b_vNReconParticles);
   anaUtils::ConfigureTreeBranch(fChain, "vNTrueVerticesMatch", &vNTrueVerticesMatch, &b_vNTrueVerticesMatch);
   anaUtils::ConfigureTreeBranch(fChain, "vTracksUniqueID", &vParticlesUniqueID, &b_vParticlesUniqueID);
   anaUtils::ConfigureTreeBranch(fChain, "vTrueVertexIndex", &vTrueVertexIndex, &b_vTrueVertexIndex);
   anaUtils::ConfigureTreeBranch(fChain, "vTrueVerticesClean", &vTrueVerticesClean, &b_vTrueVerticesClean);
   anaUtils::ConfigureTreeBranch(fChain, "vTrueVerticesCompl", &vTrueVerticesCompl, &b_vTrueVerticesCompl);

   //   anaUtils::ConfigureTreeBranch(fChain, "trStatus", trStatus, &b_trStatus);
   anaUtils::ConfigureTreeBranch(fChain, "trReconPDG", trReconPDG, &b_trReconPDG);
   //   anaUtils::ConfigureTreeBranch(fChain, "trBunch", trBunch, &b_trBunch);
   //   anaUtils::ConfigureTreeBranch(fChain, "trNReconVertices", &trNReconVertices, &b_trNReconVertices);
   anaUtils::ConfigureTreeBranch(fChain, "trDetectors", trDetectors, &b_trDetectors);
   //   anaUtils::ConfigureTreeBranch(fChain, "trNTPCs", trNTPCs, &b_trNTPCs);
   //   anaUtils::ConfigureTreeBranch(fChain, "trNFGDs", trNFGDs, &b_trNFGDs);
   //   anaUtils::ConfigureTreeBranch(fChain, "trNECALs", trNECALs, &b_trNECALs);
   //   anaUtils::ConfigureTreeBranch(fChain, "trNSMRDs", trNSMRDs, &b_trNSMRDs);
   //   anaUtils::ConfigureTreeBranch(fChain, "trNP0Ds", trNP0Ds, &b_trNP0Ds);
   anaUtils::ConfigureTreeBranch(fChain, "trNTRACKERs", trNTRACKERs, &b_trNTRACKERs);
   anaUtils::ConfigureTreeBranch(fChain, "trNHits", trNHits, &b_trNHits);
   anaUtils::ConfigureTreeBranch(fChain, "trNNodes", trNNodes, &b_trNNodes);
   anaUtils::ConfigureTreeBranch(fChain, "trNDOF", trNDOF, &b_trNDOF);
   anaUtils::ConfigureTreeBranch(fChain, "trChi2", trChi2, &b_trChi2);
   anaUtils::ConfigureTreeBranch(fChain, "trCharge", trCharge, &b_trCharge);
   anaUtils::ConfigureTreeBranch(fChain, "trMomentum", trMomentum, &b_trMomentum);
   //   anaUtils::ConfigureTreeBranch(fChain, "trMomentumError", trMomentumError, &b_trMomentumError);
   anaUtils::ConfigureTreeBranch(fChain, "trMomentumMuon", trMomentumMuon, &b_trMomentumMuon);
   //   anaUtils::ConfigureTreeBranch(fChain, "trMomentumErrorMuon", trMomentumErrorMuon, &b_trMomentumErrorMuon);
   anaUtils::ConfigureTreeBranch(fChain, "trMomentumProton", trMomentumProton, &b_trMomentumProton);
   //   anaUtils::ConfigureTreeBranch(fChain, "trMomentumErrorProton", trMomentumErrorProton, &b_trMomentumErrorProton);
   anaUtils::ConfigureTreeBranch(fChain, "trMomentumEle", trMomentumEle, &b_trMomentumEle);
   //   anaUtils::ConfigureTreeBranch(fChain, "trMomentumErrorEle", trMomentumErrorEle, &b_trMomentumErrorEle);
    
   
   anaUtils::ConfigureTreeBranch(fChain, "trLength", trLength, &b_trLength);

#if VERSION_HAS_REVERSED_REFITS
   anaUtils::ConfigureTreeBranch(fChain, "trDirectionStartFlip", trDirectionStartFlip, &b_trDirectionStartFlip);
   anaUtils::ConfigureTreeBranch(fChain, "trDirectionEndFlip",   trDirectionEndFlip,   &b_trDirectionEndFlip);
#endif
 
   
#if VERSION_HAS_PRANGE_ESTIMATES
 //  anaUtils::ConfigureTreeBranch(fChain, "trRangeMomentumEle", trRangeMomentumEle, &b_trRangeMomentumEle);
   anaUtils::ConfigureTreeBranch(fChain, "trRangeMomentumProton", trRangeMomentumProton, &b_trRangeMomentumProton);
 //  anaUtils::ConfigureTreeBranch(fChain, "trRangeMomentumMuonFlip", trRangeMomentumMuonFlip, &b_trRangeMomentumMuonFlip);
//   anaUtils::ConfigureTreeBranch(fChain, "trRangeMomentumProtonFlip", trRangeMomentumProtonFlip, &b_trRangeMomentumProtonFlip);
#endif

   anaUtils::ConfigureTreeBranch(fChain, "trMomentumAtVertex", trMomentumAtVertex, &b_trMomentumAtVertex);
   anaUtils::ConfigureTreeBranch(fChain, "trDirectionAtVertex", trDirectionAtVertex, &b_trDirectionAtVertex);
   anaUtils::ConfigureTreeBranch(fChain, "trDirectionStart", trDirectionStart, &b_trDirectionStart);
   anaUtils::ConfigureTreeBranch(fChain, "trDirectionEnd", trDirectionEnd, &b_trDirectionEnd);
   anaUtils::ConfigureTreeBranch(fChain, "trPositionStart", trPositionStart, &b_trPositionStart);
   anaUtils::ConfigureTreeBranch(fChain, "trPositionEnd", trPositionEnd, &b_trPositionEnd);
   anaUtils::ConfigureTreeBranch(fChain, "trTrueTrackID", trTrueParticleID, &b_trTrueParticleID);
 
#if VERSION_HAS_TIME_FITS 
   //   anaUtils::ConfigureTreeBranch(fChain, "trNTimeNodes", trNTimeNodes, &b_trNTimeNodes);
   //   anaUtils::ConfigureTreeBranch(fChain, "trTimeNodeDetector",  trTimeNodeDetector,  &b_trTimeNodeDetector);
   //   anaUtils::ConfigureTreeBranch(fChain, "trTimeNodeTimeStart", trTimeNodeTimeStart, &b_trTimeNodeTimeStart);
   //   anaUtils::ConfigureTreeBranch(fChain, "trTimeNodeTimeEnd",   trTimeNodeTimeEnd,   &b_trTimeNodeTimeEnd);

   anaUtils::ConfigureTreeBranch(fChain, "trToFFGD1_FGD2",      trToFFGD1_FGD2,      &b_trToFFGD1_FGD2);
   anaUtils::ConfigureTreeBranch(fChain, "trToFP0D_FGD1",       trToFP0D_FGD1,       &b_trToFP0D_FGD1);
   anaUtils::ConfigureTreeBranch(fChain, "trToFECal_FGD1",      trToFECal_FGD1,      &b_trToFECal_FGD1);
   anaUtils::ConfigureTreeBranch(fChain, "trToFECal_FGD2",      trToFECal_FGD2,      &b_trToFECal_FGD2);
   anaUtils::ConfigureTreeBranch(fChain, "trToFFlag_FGD1_FGD2", trToFFlag_FGD1_FGD2, &b_trToFFlag_FGD1_FGD2);
   anaUtils::ConfigureTreeBranch(fChain, "trToFFlag_P0D_FGD1",  trToFFlag_P0D_FGD1,  &b_trToFFlag_P0D_FGD1);
   anaUtils::ConfigureTreeBranch(fChain, "trToFFlag_ECal_FGD1", trToFFlag_ECal_FGD1, &b_trToFFlag_ECal_FGD1);
   anaUtils::ConfigureTreeBranch(fChain, "trToFFlag_ECal_FGD2", trToFFlag_ECal_FGD2, &b_trToFFlag_ECal_FGD2);

#endif   

   anaUtils::ConfigureTreeBranch(fChain, "trTpcNHits",          trTpcNHits,          &b_trTpcNHits);
   anaUtils::ConfigureTreeBranch(fChain, "trTpcUniqueID",       trTpcUniqueID,       &b_trTpcUniqueID);
   anaUtils::ConfigureTreeBranch(fChain, "trTpcLength",         trTpcLength,         &b_trTpcLength);
   anaUtils::ConfigureTreeBranch(fChain, "trTpcDirectionStart", trTpcDirectionStart, &b_trTpcDirectionStart);
   anaUtils::ConfigureTreeBranch(fChain, "trTpcDirectionEnd",   trTpcDirectionEnd,   &b_trTpcDirectionEnd);
   anaUtils::ConfigureTreeBranch(fChain, "trTpcPositionStart",  trTpcPositionStart,  &b_trTpcPositionStart);
   anaUtils::ConfigureTreeBranch(fChain, "trTpcPositionEnd",    trTpcPositionEnd,    &b_trTpcPositionEnd);
   anaUtils::ConfigureTreeBranch(fChain, "trTpcCharge",         trTpcCharge,         &b_trTpcCharge);
   anaUtils::ConfigureTreeBranch(fChain, "trTpcMomentum",       trTpcMomentum,       &b_trTpcMomentum);
   anaUtils::ConfigureTreeBranch(fChain, "trTpcMomentumError",  trTpcMomentumError,  &b_trTpcMomentumError);
   anaUtils::ConfigureTreeBranch(fChain, "trTpcBackMomentum",   trTpcBackMomentum,   &b_trTpcBackMomentum);
#if VERSION_HAS_BFIELD_REFIT
   anaUtils::ConfigureTreeBranch(fChain, "trTpcRefitMomentum", trTpcRefitMomentum, &b_trTpcRefitMomentum);
#endif
#if VERSION_HAS_BFIELD_REFIT_FULL
   anaUtils::ConfigureTreeBranch(fChain, "trTpcRefitCharge",   trTpcRefitCharge, &b_trTpcRefitCharge);
   anaUtils::ConfigureTreeBranch(fChain, "trTpcRefitDirection",trTpcRefitDirection, &b_trTpcRefitDirection);
   anaUtils::ConfigureTreeBranch(fChain, "trTpcRefitPosition", trTpcRefitPosition, &b_trTpcRefitPosition);
#endif
#if VERSION_HAS_EFIELD_REFIT
   anaUtils::ConfigureTreeBranch(fChain, "trTpcEFieldRefitMomentum", trTpcEFieldRefitMomentum, &b_trTpcEFieldRefitMomentum);
#endif
   anaUtils::ConfigureTreeBranch(fChain, "trTpcPullmu",         trTpcPullmu,         &b_trTpcPullmu);
   anaUtils::ConfigureTreeBranch(fChain, "trTpcPullele",        trTpcPullele,        &b_trTpcPullele);
   anaUtils::ConfigureTreeBranch(fChain, "trTpcPullp",          trTpcPullp,          &b_trTpcPullp);
   anaUtils::ConfigureTreeBranch(fChain, "trTpcPullpi",         trTpcPullpi,         &b_trTpcPullpi);
   anaUtils::ConfigureTreeBranch(fChain, "trTpcPullk",          trTpcPullk,          &b_trTpcPullk);
   anaUtils::ConfigureTreeBranch(fChain, "trTpcdEdxMeas",       trTpcdEdxMeas,       &b_trTpcdEdxMeas);
   anaUtils::ConfigureTreeBranch(fChain, "trTpcdEdxExpMu",      trTpcdEdxExpMu,      &b_trTpcdEdxExpMu);
   anaUtils::ConfigureTreeBranch(fChain, "trTpcdEdxExpEle",     trTpcdEdxExpEle,     &b_trTpcdEdxExpEle);
   anaUtils::ConfigureTreeBranch(fChain, "trTpcdEdxExpP",       trTpcdEdxExpP,       &b_trTpcdEdxExpP);
   anaUtils::ConfigureTreeBranch(fChain, "trTpcdEdxExpPi",      trTpcdEdxExpPi,      &b_trTpcdEdxExpPi);
   anaUtils::ConfigureTreeBranch(fChain, "trTpcdEdxExpK",       trTpcdEdxExpK,       &b_trTpcdEdxExpK);
   anaUtils::ConfigureTreeBranch(fChain, "trTpcRawdEdxMeas",    trTpcRawdEdxMeas,    &b_trTpcRawdEdxMeas);
   anaUtils::ConfigureTreeBranch(fChain, "trTpcRawdEdxExpMu",   trTpcRawdEdxExpMu,   &b_trTpcRawdEdxExpMu);
   anaUtils::ConfigureTreeBranch(fChain, "trTpcRawdEdxExpEle",  trTpcRawdEdxExpEle,  &b_trTpcRawdEdxExpEle);
   anaUtils::ConfigureTreeBranch(fChain, "trTpcRawdEdxExpP",    trTpcRawdEdxExpP,    &b_trTpcRawdEdxExpP);
   anaUtils::ConfigureTreeBranch(fChain, "trTpcRawdEdxExpPi",   trTpcRawdEdxExpPi,   &b_trTpcRawdEdxExpPi);
   anaUtils::ConfigureTreeBranch(fChain, "trTpcRawdEdxExpK",    trTpcRawdEdxExpK,    &b_trTpcRawdEdxExpK);
   anaUtils::ConfigureTreeBranch(fChain, "trTpcdEdxSigmaMu",    trTpcdEdxSigmaMu,    &b_trTpcdEdxSigmaMu);
   anaUtils::ConfigureTreeBranch(fChain, "trTpcdEdxSigmaEle",   trTpcdEdxSigmaEle,   &b_trTpcdEdxSigmaEle);
   anaUtils::ConfigureTreeBranch(fChain, "trTpcdEdxSigmaP",     trTpcdEdxSigmaP,     &b_trTpcdEdxSigmaP);
   anaUtils::ConfigureTreeBranch(fChain, "trTpcdEdxSigmaPi",    trTpcdEdxSigmaPi,    &b_trTpcdEdxSigmaPi);
   anaUtils::ConfigureTreeBranch(fChain, "trTpcdEdxSigmaK",     trTpcdEdxSigmaK,     &b_trTpcdEdxSigmaK);
   //   anaUtils::ConfigureTreeBranch(fChain, "trTpcPurity", trTpcPurity, &b_trTpcPurity);
   anaUtils::ConfigureTreeBranch(fChain, "trTpcTrueTrackID",    trTpcTrueParticleID, &b_trTpcTrueParticleID);
   anaUtils::ConfigureTreeBranch(fChain, "trFgdDetector",       trFgdDetector,       &b_trFgdDetector);
   anaUtils::ConfigureTreeBranch(fChain, "trFgdNHits",          trFgdNHits,          &b_trFgdNHits);
   anaUtils::ConfigureTreeBranch(fChain, "trFgdNNodes",         trFgdNNodes,         &b_trFgdNNodes);
   anaUtils::ConfigureTreeBranch(fChain, "trFgdUniqueID",       trFgdUniqueID,       &b_trFgdUniqueID);
   anaUtils::ConfigureTreeBranch(fChain, "trFgdLength",         trFgdLength,         &b_trFgdLength);
   anaUtils::ConfigureTreeBranch(fChain, "trFgdDirectionStart", trFgdDirectionStart, &b_trFgdDirectionStart);
   anaUtils::ConfigureTreeBranch(fChain, "trFgdDirectionEnd",   trFgdDirectionEnd,   &b_trFgdDirectionEnd);
   anaUtils::ConfigureTreeBranch(fChain, "trFgdPositionStart",  trFgdPositionStart,  &b_trFgdPositionStart);
   anaUtils::ConfigureTreeBranch(fChain, "trFgdPositionEnd",    trFgdPositionEnd,    &b_trFgdPositionEnd);
   anaUtils::ConfigureTreeBranch(fChain, "trFgdX",              trFgdX,              &b_trFgdX);
   anaUtils::ConfigureTreeBranch(fChain, "trFgdE",              trFgdE,              &b_trFgdE);
   anaUtils::ConfigureTreeBranch(fChain, "trFgdPullmu",         trFgdPullmu,         &b_trFgdPullmu);
   anaUtils::ConfigureTreeBranch(fChain, "trFgdPullp",          trFgdPullp,          &b_trFgdPullp);
   anaUtils::ConfigureTreeBranch(fChain, "trFgdPullpi",         trFgdPullpi,         &b_trFgdPullpi);
   anaUtils::ConfigureTreeBranch(fChain, "trFgdPullno",         trFgdPullno,         &b_trFgdPullno);
   anaUtils::ConfigureTreeBranch(fChain, "trFgdContainment",    trFgdContainment,    &b_trFgdContainment);
   //   anaUtils::ConfigureTreeBranch(fChain, "trFgdAvgTime", trFgdAvgTime, &b_trFgdAvgTime);

#if VERSION_HAS_FGD_VERTEX_AND_END_ACTIVITY
   anaUtils::ConfigureTreeBranch(fChain, "trFgdHasFgdVA",        trFgdHasFgdVA,        &b_trFgdHasFgdVA);
   anaUtils::ConfigureTreeBranch(fChain, "trFgdHasFgdVA_fmode",  trFgdHasFgdVA_fmode,  &b_trFgdHasFgdVA_fmode);
   anaUtils::ConfigureTreeBranch(fChain, "trFgdTypeVA",          trFgdTypeVA,          &b_trFgdTypeVA);
#endif
   anaUtils::ConfigureTreeBranch(fChain, "trFgdVertex1x1",   trFgdVertex1x1,   &b_trFgdVertex1x1);
   anaUtils::ConfigureTreeBranch(fChain, "trFgdVertex3x3",   trFgdVertex3x3,   &b_trFgdVertex3x3);
   anaUtils::ConfigureTreeBranch(fChain, "trFgdVertex5x5",   trFgdVertex5x5,   &b_trFgdVertex5x5);
   anaUtils::ConfigureTreeBranch(fChain, "trFgdVertex7x7",   trFgdVertex7x7,   &b_trFgdVertex7x7);
   anaUtils::ConfigureTreeBranch(fChain, "trFgdVertexLayer", trFgdVertexLayer, &b_trFgdVertexLayer);

#if VERSION_HAS_FGD_VERTEX_AND_END_ACTIVITY
   anaUtils::ConfigureTreeBranch(fChain, "trFgdHasFgdEA",       trFgdHasFgdEA,       &b_trFgdHasFgdEA);
   anaUtils::ConfigureTreeBranch(fChain, "trFgdHasFgdEA_fmode", trFgdHasFgdEA_fmode, &b_trFgdHasFgdEA_fmode);
   anaUtils::ConfigureTreeBranch(fChain, "trFgdTypeEA",         trFgdTypeEA,         &b_trFgdTypeEA);
   anaUtils::ConfigureTreeBranch(fChain, "trFgdEnd0x0",         trFgdEnd0x0,         &b_trFgdEnd0x0);
   anaUtils::ConfigureTreeBranch(fChain, "trFgdEnd0x0",         trFgdEnd0x0,         &b_trFgdEnd0x0);
   anaUtils::ConfigureTreeBranch(fChain, "trFgdEnd1x1",         trFgdEnd1x1,         &b_trFgdEnd1x1);
   anaUtils::ConfigureTreeBranch(fChain, "trFgdEnd3x3",         trFgdEnd3x3,         &b_trFgdEnd3x3);
   anaUtils::ConfigureTreeBranch(fChain, "trFgdEnd5x5",         trFgdEnd5x5,         &b_trFgdEnd5x5);
   anaUtils::ConfigureTreeBranch(fChain, "trFgdEnd7x7",         trFgdEnd7x7,         &b_trFgdEnd7x7);
#endif   
   
   //   anaUtils::ConfigureTreeBranch(fChain, "trECALDetector", trECALDetector, &b_trECALDetector);
   anaUtils::ConfigureTreeBranch(fChain, "trECALNHits", trECALNHits, &b_trECALNHits);
   //   anaUtils::ConfigureTreeBranch(fChain, "trECALNNodes", trECALNNodes, &b_trECALNNodes);
   anaUtils::ConfigureTreeBranch(fChain, "trECALUniqueID", trECALUniqueID, &b_trECALUniqueID);
   //   anaUtils::ConfigureTreeBranch(fChain, "trECALLength", trECALLength, &b_trECALLength);
   anaUtils::ConfigureTreeBranch(fChain, "trECALDirectionStart", trECALDirectionStart, &b_trECALDirectionStart);
   anaUtils::ConfigureTreeBranch(fChain, "trECALDirectionEnd",   trECALDirectionEnd,   &b_trECALDirectionEnd);
   anaUtils::ConfigureTreeBranch(fChain, "trECALPositionStart",  trECALPositionStart,  &b_trECALPositionStart);
   anaUtils::ConfigureTreeBranch(fChain, "trECALPositionEnd",    trECALPositionEnd,    &b_trECALPositionEnd);
   //   anaUtils::ConfigureTreeBranch(fChain, "trECALEMEnergy", trECALEMEnergy, &b_trECALEMEnergy);
   anaUtils::ConfigureTreeBranch(fChain, "trECALEDeposit",     trECALEDeposit,     &b_trECALEDeposit);
   anaUtils::ConfigureTreeBranch(fChain, "trECALIsShowerLike", trECALIsShowerLike, &b_trECALIsShowerLike);
   anaUtils::ConfigureTreeBranch(fChain, "trECALAvgTime",      trECALAvgTime,      &b_trECALAvgTime);
   //ECal PID 
   anaUtils::ConfigureTreeBranch(fChain, "trECALPIDMipPion",           trECALPIDMipPion,           &b_trECALPIDMipPion);
   //   anaUtils::ConfigureTreeBranch(fChain, "trECALPIDMipEm",             trECALPIDMipEm,             &b_trECALPIDMipEm);
   //   anaUtils::ConfigureTreeBranch(fChain, "trECALPIDEmHip",             trECALPIDEmHip,             &b_trECALPIDEmHip);
   anaUtils::ConfigureTreeBranch(fChain, "trECALContainment",          trECALContainment,          &b_trECALContainment);
   //   anaUtils::ConfigureTreeBranch(fChain, "trECALMostUpStreamLayerHit", trECALMostUpStreamLayerHit, &b_trECALMostUpStreamLayerHit);
   
   //   anaUtils::ConfigureTreeBranch(fChain, "trSMRDDetector", trSMRDDetector, &b_trSMRDDetector);
   //   anaUtils::ConfigureTreeBranch(fChain, "trSMRDNHits", trSMRDNHits, &b_trSMRDNHits);
   //   anaUtils::ConfigureTreeBranch(fChain, "trSMRDNNodes", trSMRDNNodes, &b_trSMRDNNodes);
   //   anaUtils::ConfigureTreeBranch(fChain, "trSMRDUniqueID", trSMRDUniqueID, &b_trSMRDUniqueID);
   //   anaUtils::ConfigureTreeBranch(fChain, "trSMRDLength", trSMRDLength, &b_trSMRDLength);
   //   anaUtils::ConfigureTreeBranch(fChain, "trSMRDDirectionStart", trSMRDDirectionStart, &b_trSMRDDirectionStart);
   //   anaUtils::ConfigureTreeBranch(fChain, "trSMRDDirectionEnd", trSMRDDirectionEnd, &b_trSMRDDirectionEnd);
   //   anaUtils::ConfigureTreeBranch(fChain, "trSMRDPositionStart", trSMRDPositionStart, &b_trSMRDPositionStart);
   //   anaUtils::ConfigureTreeBranch(fChain, "trSMRDPositionEnd", trSMRDPositionEnd, &b_trSMRDPositionEnd);
   //   anaUtils::ConfigureTreeBranch(fChain, "trSMRDAvgTime", trSMRDAvgTime, &b_trSMRDAvgTime);
   anaUtils::ConfigureTreeBranch(fChain, "trSMRDEDeposit", trSMRDEDeposit, &b_trSMRDEDeposit);
   
   
     
   //   anaUtils::ConfigureTreeBranch(fChain, "trP0DDetector", trP0DDetector, &b_trP0DDetector);
   //   anaUtils::ConfigureTreeBranch(fChain, "trP0DNHits", trP0DNHits, &b_trP0DNHits);
      anaUtils::ConfigureTreeBranch(fChain, "trP0DNNodes", trP0DNNodes, &b_trP0DNNodes);
   //   anaUtils::ConfigureTreeBranch(fChain, "trP0DUniqueID", trP0DUniqueID, &b_trP0DUniqueID);
   //   anaUtils::ConfigureTreeBranch(fChain, "trP0DLength", trP0DLength, &b_trP0DLength);
   //   anaUtils::ConfigureTreeBranch(fChain, "trP0DDirectionStart", trP0DDirectionStart, &b_trP0DDirectionStart);
   //   anaUtils::ConfigureTreeBranch(fChain, "trP0DDirectionEnd", trP0DDirectionEnd, &b_trP0DDirectionEnd);
   //   anaUtils::ConfigureTreeBranch(fChain, "trP0DPositionStart", trP0DPositionStart, &b_trP0DPositionStart);
   //   anaUtils::ConfigureTreeBranch(fChain, "trP0DPositionEnd", trP0DPositionEnd, &b_trP0DPositionEnd);

#if VERSION_HAS_P0D_AVERAGED_TIME     
   anaUtils::ConfigureTreeBranch(fChain, "trP0DAvgTime", trP0DAvgTime, &b_trP0DAvgTime);
#endif
   
   anaUtils::ConfigureTreeBranch(fChain, "trTRACKERDetector", trTRACKERDetector, &b_trTRACKERDetector);
   anaUtils::ConfigureTreeBranch(fChain, "trTRACKERNHits", trTRACKERNHits, &b_trTRACKERNHits);
   anaUtils::ConfigureTreeBranch(fChain, "trTRACKERNNodes", trTRACKERNNodes, &b_trTRACKERNNodes);
   anaUtils::ConfigureTreeBranch(fChain, "trTRACKERUniqueID", trTRACKERUniqueID, &b_trTRACKERUniqueID);
   anaUtils::ConfigureTreeBranch(fChain, "trTRACKERLength", trTRACKERLength, &b_trTRACKERLength);
   anaUtils::ConfigureTreeBranch(fChain, "trTRACKERDirectionStart", trTRACKERDirectionStart, &b_trTRACKERDirectionStart);
   anaUtils::ConfigureTreeBranch(fChain, "trTRACKERDirectionEnd", trTRACKERDirectionEnd, &b_trTRACKERDirectionEnd);
   anaUtils::ConfigureTreeBranch(fChain, "trTRACKERPositionStart", trTRACKERPositionStart, &b_trTRACKERPositionStart);
   anaUtils::ConfigureTreeBranch(fChain, "trTRACKERPositionEnd", trTRACKERPositionEnd, &b_trTRACKERPositionEnd);
   anaUtils::ConfigureTreeBranch(fChain, "trTRACKERCharge", trTRACKERCharge, &b_trTRACKERCharge);
   anaUtils::ConfigureTreeBranch(fChain, "trTRACKERMomentum", trTRACKERMomentum, &b_trTRACKERMomentum);
   anaUtils::ConfigureTreeBranch(fChain, "trTRACKERMomentumEnd", trTRACKERMomentumEnd, &b_trTRACKERMomentumEnd);

   /// AnaLocalReconBunch information
   if(_isUsingReconDirTECAL){
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconCounter",                &tecalReconCounter,                  &b_tecalReconCounter		  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconAverageHitTime",          tecalReconAverageHitTime,           &b_tecalReconAverageHitTime	  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconAverageZPosition",        tecalReconAverageZPosition,         &b_tecalReconAverageZPosition	  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconContainment",             tecalReconContainment,              &b_tecalReconContainment		  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconEFitResult",              tecalReconEFitResult,               &b_tecalReconEFitResult		  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconEFitUncertainty",         tecalReconEFitUncertainty,          &b_tecalReconEFitUncertainty	  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconFirstLayer",              tecalReconFirstLayer,               &b_tecalReconFirstLayer		  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconIsShowerLike",            tecalReconIsShowerLike,             &b_tecalReconIsShowerLike		  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconIsTrackLike",             tecalReconIsTrackLike,              &b_tecalReconIsTrackLike		  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconLastLayer",               tecalReconLastLayer,                &b_tecalReconLastLayer		  );
#if VERSION_HAS_ECAL_LLR
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconLikeMIPEM",               tecalReconLikeMIPEM,                &b_tecalReconLikeMIPEM		  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconLikeMIPPion",             tecalReconLikeMIPPion,              &b_tecalReconLikeMIPPion		  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconLikeEMHIP",               tecalReconLikeEMHIP,                &b_tecalReconLikeEMHIP		  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconLikeMIPEMLow",            tecalReconLikeMIPEMLow,             &b_tecalReconLikeMIPEMLow		  );
#endif
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconMatchingLike",            tecalReconMatchingLike,             &b_tecalReconMatchingLike		  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconMichelTagNDelayedCluster",tecalReconMichelTagNDelayedCluster, &b_tecalReconMichelTagNDelayedCluster);
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconModule",                  tecalReconModule,                   &b_tecalReconModule		  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconMostDownStreamLayerHit",  tecalReconMostDownStreamLayerHit,   &b_tecalReconMostDownStreamLayerHit  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconMostUpStreamLayerHit",    tecalReconMostUpStreamLayerHit,     &b_tecalReconMostUpStreamLayerHit	  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconNHits",                   tecalReconNHits,                    &b_tecalReconNHits			  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconNLayersHit",              tecalReconNLayersHit,               &b_tecalReconNLayersHit		  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconObjectLength",            tecalReconObjectLength,             &b_tecalReconObjectLength		  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconPIDAMR",                  tecalReconPIDAMR,                   &b_tecalReconPIDAMR		  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconPIDAngle",                tecalReconPIDAngle,                 &b_tecalReconPIDAngle		  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconPIDAsymmetry",            tecalReconPIDAsymmetry,             &b_tecalReconPIDAsymmetry		  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconPIDCircularity",          tecalReconPIDCircularity,           &b_tecalReconPIDCircularity	  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconPIDFBR",                  tecalReconPIDFBR,                   &b_tecalReconPIDFBR		  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconPIDMaxRatio",             tecalReconPIDMaxRatio,              &b_tecalReconPIDMaxRatio		  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconPIDMeanPos",              tecalReconPIDMeanPos,               &b_tecalReconPIDMeanPos		  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconPIDShowerAngle",          tecalReconPIDShowerAngle,           &b_tecalReconPIDShowerAngle	  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconPIDShowerWidth",          tecalReconPIDShowerWidth,           &b_tecalReconPIDShowerWidth	  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconPIDTransverseChargeRatio",tecalReconPIDTransverseChargeRatio, &b_tecalReconPIDTransverseChargeRatio);
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconPIDTruncatedMaxRatio",    tecalReconPIDTruncatedMaxRatio,     &b_tecalReconPIDTruncatedMaxRatio	  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconPointing",                tecalReconPointing,                 &b_tecalReconPointing		  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconThrust",                  tecalReconThrust,                   &b_tecalReconThrust		  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconThrustAxis",              tecalReconThrustAxis,               &b_tecalReconThrustAxis		  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconThrustOrigin",            tecalReconThrustOrigin,             &b_tecalReconThrustOrigin		  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconTimeBunch",               tecalReconTimeBunch,                &b_tecalReconTimeBunch		  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconTotalHitCharge",          tecalReconTotalHitCharge,           &b_tecalReconTotalHitCharge	  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconTrueID",                  tecalReconTrueID,                   &b_tecalReconTrueID		  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconTrueIDPrimary",           tecalReconTrueIDPrimary,            &b_tecalReconTrueIDPrimary		  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconTrueIDRecursive",         tecalReconTrueIDRecursive,          &b_tecalReconTrueIDRecursive	  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconTrueIDSingle",            tecalReconTrueIDSingle,             &b_tecalReconTrueIDSingle		  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconUniqueID",                tecalReconUniqueID,                 &b_tecalReconUniqueID		  );
     anaUtils::ConfigureTreeBranch(fChain, "tecalReconBunch",                   tecalReconBunch,                    &b_tecalReconBunch			  );
   }
}

//********************************************************************
FlatTreeConverter::~FlatTreeConverter(){
//********************************************************************
  
}

//****************************************************************************
bool FlatTreeConverter::AddFileToTChain(const std::string& inputString){
//****************************************************************************

  std::cout << "FlatTreeConverter::AddFileToTChain(). Adding file: " << inputString << std::endl;

  if (!RedoTreeConverter::AddFileToTChain(inputString)) return false;
  
  // Open the file to do few checks
  TFile *f = TFile::Open(inputString.c_str()); 
  f->cd(); 

  // ----------  Deal with RooTrackerVtx trees. Only for the first file


  if( _firstFile ) {
    _firstFile = false; 

    if (_readRooTrackerVtx){
      if (fNeut) {
        NRooTrackerVTX = GetChain("NRooTrackerVtx");
        NRooTrackerVTX->SetBranchAddress("RunID",    &RunID);
        NRooTrackerVTX->SetBranchAddress("SubrunID", &SubrunID);
        NRooTrackerVTX->SetBranchAddress("EventID",  &EventID);
        NVtx = new TClonesArray("ND::NRooTrackerVtx",100);
        NRooTrackerVTX->SetBranchAddress("NVtx",&NNVtx);
        NRooTrackerVTX->SetBranchAddress("Vtx",&NVtx);
        std::cout << "FlatTreeConverter::AddFileToTChain(). NEUT RooTrackerVtx tree found !!" << std::endl; 
      }
      else if(fGenie) {
        GRooTrackerVTX = GetChain("GRooTrackerVtx");
        GRooTrackerVTX->SetBranchAddress("RunID",    &RunID);
        GRooTrackerVTX->SetBranchAddress("SubrunID", &SubrunID);
        GRooTrackerVTX->SetBranchAddress("EventID",  &EventID);
        GVtx = new TClonesArray("ND::GRooTrackerVtx",100);
        GRooTrackerVTX->SetBranchAddress("NVtx",&NGVtx);
        GRooTrackerVTX->SetBranchAddress("Vtx",&GVtx);
        std::cout << "FlatTreeConverter::AddFileToTChain(). GENIE RooTrackerVtx tree found !!" << std::endl; 
      }
    }
  }

  // Close the file
  f->Close(); 
  
  // Define Data/MC status: use the first valid entry in the flat tree
  Long64_t nentries = GetEntries();  
  for (Long64_t i = 0; i<nentries; i++){
    Int_t entry_temp = flattree->GetEntry(i);
    if (entry_temp<=0) continue;    
    // Set the data/MC mode and return false when mixing data and MC files
    if (!header().SetIsMC(sIsMC)) return false;
    break;
  }

  return true;
}

//*****************************************************************************
Int_t FlatTreeConverter::GetSpill(Long64_t& entry, AnaSpillC*& spill){
//*****************************************************************************

  // Read contents of entry.
  if (!fChain) return 0;

  // Create an instance of AnaSpill
  spill = MakeSpill();

  // get a new entry from the flat tree. entry_temp >0 when succesfull
  Int_t entry_temp = flattree->GetEntry(entry);

  // If this is not one of the events to skim just go to the next entry (and don't process that event --> return <=0)
  if (!anaUtils::CheckSkimmedEvent(sRun,sSubrun,sEvt)){
    entry++;
    return 0;
  }
      
  if (_readRooTrackerVtx){
    // sEvt should be positive since sEvt=-999 is used for the last flatree entry
    if (entry_temp>0 && sIsMC && (fGenie || fNeut) && sEvt>=0) {
      // Loop over RooTrackerVtx entries until we get the same run, subrun and event numbers
      // In general we will get the right entry in the first iteration, but just in case
      do{       
        if      (NRooTrackerVTX)  NRooTrackerVTX->GetEntry(_entry_roo);
        else if (GRooTrackerVTX)  GRooTrackerVTX->GetEntry(_entry_roo);
        if ((RunID> sRun                                      ) ||
            (RunID==sRun && SubrunID> sSubrun                 ) ||
            (RunID==sRun && SubrunID==sSubrun && EventID>sEvt )){
          _entry_roo--;  
          _RooVtxEntryInCurrentInputFile--;
        }
        else{
          _entry_roo++;
          _RooVtxEntryInCurrentInputFile++;
        }
        }while(EventID!=sEvt || RunID!=sRun || SubrunID!=sSubrun);
    }
  }

  

  // Print the current file
  TString filename(flattree->GetFile()->GetName());   
  if( filename != _currentFileName ) {
    _RooVtxEntryInCurrentInputFile=0;
    _currentFileIndex++;
    std::cout << " Running on file (" << _currentFileIndex << " ): " << filename << std::endl; 
    _currentFileName = filename;
  }
  
  // Caset the spill
  _spill = static_cast<AnaSpillB*>(spill);
  //  _spill = spill;

  int evt_prev=sEvt;

  // This is needed to make sure that we read the first bunch of the spill when skiping entries
  if (_firstEntry){
    _firstEntry=false;
    Long64_t requested_first_entry = entry;
    while (sEvt==evt_prev && entry>0){
      entry--;
      entry_temp = flattree->GetEntry(entry);
    }
    if (sEvt!=evt_prev){
      entry++;
      entry_temp = flattree->GetEntry(entry);
    }

    // Fill the general spill info
    FillInfo(_spill);

    entry = requested_first_entry;
    entry_temp = flattree->GetEntry(entry);
  }
  else{
    // Fill the general spill info
    FillInfo(_spill);
  }

  // Fill tracks->vertices and vertices->tracks with null pointers for tracks and vertices that are not in this bunch
  //  FillNotInBunchObjWithNullPointers(_spill);

  // Always make the out of bunch
  if (_isUsingReconDirTECAL || _isUsingReconDirPECAL || _isUsingReconDirFGDOnly || _isUsingReconDirP0D)
    _spill->OutOfBunch = MakeLocalReconBunch();
  else
    _spill->OutOfBunch = MakeBunch();
  
  evt_prev = sEvt;  

  while (sEvt==evt_prev && entry < _nentries){
    // Create and fill a new bunch

    // First the Out of bunch stuff
    if (Bunch==-1) {
      FillBunchInfo(_spill->TrueParticles, _spill->OutOfBunch);
      // Fill tracks->vertices and vertices->tracks with null pointers for tracks and vertices that are not in this bunch
      //      FillNotInBunchObjWithNullPointers(spill->OutOfBunch);
    } else if (Bunch>-1){   // Bunch=-999 when the entry in the FlatTree is just for the Spill info,
      // because there are no reconstructed tracks. We should skip this. Actual bunches are numbered: 0-7
      
      AnaBunch* bunch = NULL; 
      if (_isUsingReconDirTECAL || _isUsingReconDirPECAL || _isUsingReconDirFGDOnly || _isUsingReconDirP0D)
        bunch = static_cast<AnaLocalReconBunch*>(MakeLocalReconBunch());
      else 
        bunch = static_cast<AnaBunch*>          (MakeBunch          ());

      _spill->Bunches.push_back(bunch);
      FillBunchInfo(_spill->TrueParticles, bunch);

      // Fill tracks->vertices and vertices->tracks with null pointers for tracks and vertices that are not in this bunch
      //      FillNotInBunchObjWithNullPointers(bunch);
    }
    
    // Check that null pointers are still filled properly
    //    CheckNotInBunchObjWithNullPointers(_spill);

    // increment the entry number
    entry++;

    // break if we have reach the maximum entry number
    if (entry>=_nentries) break;

    // save the previous event number
    evt_prev=sEvt;

    // get a new entry from the input tree
    entry_temp = flattree->GetEntry(entry);
  }

  return entry_temp;
}

//*****************************************************************************
void FlatTreeConverter::FillInfo(AnaSpillB* spillB){
//*****************************************************************************

  RedoTreeConverter::FillInfo(spillB);
 
  AnaSpill* spill = static_cast<AnaSpill*>(spillB);

  AnaEventInfo& info = *static_cast<AnaEventInfo*>(spill->EventInfo);

  info.IsMC   = sIsMC;
  info.SubRun = sSubrun;
  info.EventTime = sEventTime;

  if ( fNeut || fGenie) {
    spill->InputFileIndex = _currentFileIndex;
    spill->RooVtxEntry    = _RooVtxEntryInCurrentInputFile;
  }
  else{
    spill->InputFileIndex = sInputFileIndex;
    spill->RooVtxEntry    = sRooVtxEntry;
  }


  // Give the proper size to the TrueParticles vector for each TrueVertex.
  // This can be done only after filling the general True Track vector (above) since is in this process
  // when the TrueVertex <---> TrueParticle association is done
  for (UInt_t i=0;i<spill->TrueVertices.size();i++){
    anaUtils::ResizeArray(spill->TrueVertices[i]->TrueParticles, spill->TrueVertices[i]->nTrueParticles, sNTrueParticles);
  }

  // if RooTrackerVtx is not available fill the new NPrimaryParticles (used in Categories.cxx) using TrueParticles
  if ( ! fNeut && ! fGenie) {
    for (UInt_t i=0;i<spill->TrueVertices.size();i++){
      AnaTrueVertex* vertex = static_cast<AnaTrueVertex*>(spill->TrueVertices[i]);
      FillNPrimaryParticles(vertex);
    }
  }


  FillTriggerInfo(&spill->Trigger);

}

//*****************************************************************************
void FlatTreeConverter::FillBunchInfo(std::vector<AnaTrueParticleB*>& trueParticles, AnaBunchB* bunchB){
//*****************************************************************************

  RedoTreeConverter::FillBunchInfo(trueParticles, bunchB);

  AnaBunch* bunch = static_cast<AnaBunch*>(bunchB);
        
  /// Is there AnaLocalReconBunch information to be filled?
  if(_isUsingReconDirTECAL){
    for (int i=0; i<tecalReconCounter; i++){
      AnaTECALReconObject* anaTECAL = static_cast<AnaTECALReconObject*>(MakeTECALReconObject());
      FillTECALReconObjectInfo(i,anaTECAL,bunch);
      static_cast<AnaLocalReconBunch*>(bunchB)->TECALReconObjects.push_back(anaTECAL);
    }
  }
}

//*****************************************************************************
void FlatTreeConverter::FillNotInBunchObjWithNullPointers(AnaBunch* bunch){
//*****************************************************************************

  // Fill ReconParticles->ReconVertices (that should be already sorted) with NULL pointers for vertices not in this bunch
  for (int i=0; i<NParticles; i++) {
    AnaTrack* track = static_cast<AnaTrack*>(bunch->Particles.at(i));
    for (int j = track->ReconVertices.size(); j < trNReconVertices[i]; j++){
      track->ReconVertices.push_back(NULL);
    }
    if ( ! suppress_err_msg)
      if ((int)track->ReconVertices.size() != trNReconVertices[i])
        std::cout << "minor error 01011" << std::endl;
  }

  for (int i=0; i<NVertices; i++) {

    //Fill ReconVertices->ReconParticles (that should be already sorted as stored) with NULL pointers for vertices not in this bunch
    for (int j = bunch->Vertices.at(i)->nParticles; j < vNReconParticles[i]; j++)
        bunch->Vertices.at(i)->Particles[j] = NULL;
    if ( ! suppress_err_msg)
      if ((int)bunch->Vertices.at(i)->nParticles != vNReconParticles[i])
        std::cout << "minor error 01012" << std::endl;

    //Fill ReconVertices->TrueVertices (that should be already sorted as stored) with NULL pointers for vertices not in this bunch
    std::pair<AnaTrueVertex*, AnaRecTrueMatch> myMatch;
    myMatch.first = NULL;
    myMatch.second.Cleanliness  = -999;
    myMatch.second.Completeness = -999;
    for (int j = bunch->Vertices.at(i)->TrueVerticesMatch.size(); j < vNTrueVerticesMatch[i]; j++)
        bunch->Vertices.at(i)->TrueVerticesMatch.push_back(myMatch);
    if ( ! suppress_err_msg) 
      if ((int)bunch->Vertices.at(i)->TrueVerticesMatch.size() != vNTrueVerticesMatch[i])
        std::cout<<"minor error 01013"<<std::endl;

  }
}

//*****************************************************************************
void FlatTreeConverter::FillNotInBunchObjWithNullPointers(AnaSpill* spill){
//*****************************************************************************

  // Fill TrueParticles->ReconTracks with NULL pointers for tracks not in this bunch
  for (int i=0; i<(int) spill->TrueParticles.size(); i++) {
    for (int j = 0; j < sTrueParticleNReconTracks[i] ; j++) static_cast<AnaTrueParticle*>(spill->TrueParticles.at(i))->ReconParticles.push_back(NULL);
  }

  // Fill TrueVertices->ReconTracks and TrueVertices->ReconVertex with NULL pointers for tracks/vertices not in this bunch
  for (int i=0; i<(int) spill->TrueVertices.size(); i++) {
    AnaTrueVertex* vertex = static_cast<AnaTrueVertex*>(spill->TrueVertices.at(i));
      for (int j = 0; j < sTrueVertexNReconVertices[i] ; j++) vertex->ReconVertices.push_back(NULL);
      for (int j = 0; j < sTrueVertexNReconTracks[i]   ; j++) vertex->ReconParticles.push_back(NULL);
  }
}

//*****************************************************************************
void FlatTreeConverter::CheckNotInBunchObjWithNullPointers(AnaSpill* spill){
//*****************************************************************************

  // Check TrueParticles->ReconTracks NULL pointers for tracks not in this bunch
  for (int i=0; i<(int) spill->TrueParticles.size(); i++) {
    AnaTrueParticle* thistrueobj = static_cast<AnaTrueParticle*>(spill->TrueParticles.at(i));
    if ( ! suppress_err_msg)
      if ((int)thistrueobj->ReconParticles.size() != sTrueParticleNReconTracks[i])
        std::cout << "minor error 0102" << std::endl;
  }

  // Check TrueVertices->ReconParticles and TrueVertices->ReconVertex NULL pointers for tracks/vertices not in this bunch
  for (int i=0; i<(int) spill->TrueVertices.size(); i++) {
    AnaTrueVertex* thistrueobj = static_cast<AnaTrueVertex*>(spill->TrueVertices.at(i));
    if ( ! suppress_err_msg){
      if ((int)thistrueobj->ReconVertices.size() != sTrueVertexNReconVertices[i])
        std::cout << "minor error 0103" << std::endl;
      if ((int)thistrueobj->ReconParticles.size() != sTrueVertexNReconTracks[i])
        std::cout << "minor error 0104" << std::endl;
    }
  }
}

//*****************************************************************************
void FlatTreeConverter::FillFgdTimeBinInfo(std::vector<AnaFgdTimeBinB*>& FgdTimeBins){
//*****************************************************************************

  RedoTreeConverter::FillFgdTimeBinInfo(FgdTimeBins);

  //loop over fgd time bins
  for(UInt_t ibin=0; ibin<FgdTimeBins.size(); ibin++) {      

    AnaFgdTimeBin* abin = static_cast<AnaFgdTimeBin*>(FgdTimeBins[ibin]);

    abin->MaxTime = sFgdTimeBinMaxTime[ibin];
    abin->G4ID = sFgdTimeBinG4ID[ibin];
  }
}  

//*****************************************************************************
void FlatTreeConverter::FillDQInfo(AnaDataQualityB* dqB){
//*****************************************************************************

  RedoTreeConverter::FillDQInfo(dqB);

  AnaDataQuality* dq = static_cast<AnaDataQuality*>(dqB);

  dq->GoodDaq   = sDQGoodDaq;
  dq->ND280Flag = sDQND280Flag;
  
  for (int i=0;i<7;i++)
    dq->DetFlag[i] = sDQDetFlag[i];
}

//*****************************************************************************
void FlatTreeConverter::FillBeamInfo(AnaBeamB* beamB){
//*****************************************************************************

  RedoTreeConverter::FillBeamInfo(beamB);

  AnaBeam* beam = static_cast<AnaBeam*>(beamB);

  beam->SpillNumber = sBeamSpillNumber;
  beam->Spill  = sBeamSpill;
  beam->GoodSpill = sBeamGoodSpill;
  beam->POT = sBeamPOT;
#if !VERSION_HAS_OFFICIAL_POT
  beam->POTCT4 = sBeamPOTCT4;
#endif
  beam->BeamRunNumber = sBeamBeamRunNumber;
}

//*****************************************************************************
void FlatTreeConverter::FillTriggerInfo(AnaTrigger* trigger){
//*****************************************************************************

  trigger->FGDCosmic    = sTriggerFGDCosmic;
  trigger->TripTCosmic  = sTriggerTripTCosmic;
  
  for (int i = 0; i < 6; i++){
    trigger->CTMTriggerPattern[i] = (UInt_t)sTriggerCTMTriggerPattern[i];
  }
}

//*****************************************************************************
void FlatTreeConverter::FillTrackInfo(std::vector<AnaTrueParticleB*>& trueParticles, int itrk, AnaTrackB* trackB){
//*****************************************************************************
  
  RedoTreeConverter::FillTrackInfo(trueParticles,itrk,trackB);

  AnaTrack* track = static_cast<AnaTrack*>(trackB);
  
  track->Status        = (trStatus)[itrk];
  track->ReconPDG      = (trReconPDG)[itrk];
  track->Bunch         = (trBunch)[itrk];
  track->Detectors     = (trDetectors)[itrk];
  track->NDOF          = (trNDOF)[itrk];
  track->Chi2          = (trChi2)[itrk];
  track->Charge        = (trCharge)[itrk];
  track->MomentumError = (trMomentumError)[itrk];
  track->MomentumMuon        = (trMomentumMuon)[itrk];
  track->MomentumErrorMuon   = (trMomentumErrorMuon)[itrk];
  track->MomentumProton      = (trMomentumProton)[itrk];
  track->MomentumErrorProton = (trMomentumErrorProton)[itrk];
  track->MomentumEle         = (trMomentumEle)[itrk];
  track->MomentumErrorEle    = (trMomentumErrorEle)[itrk];
  track->Length              = (trLength)[itrk];

#if VERSION_HAS_REVERSED_REFITS
  anaUtils::CopyArray(trDirectionStartFlip[itrk], track->DirectionStartFlip, 3);
  anaUtils::CopyArray(trDirectionEndFlip[itrk],   track->DirectionEndFlip,   3);
#endif
 
#if VERSION_HAS_PRANGE_ESTIMATES
  track->RangeMomentumEle       = (trRangeMomentumEle)[itrk];
  track->RangeMomentumProton    = (trRangeMomentumProton)[itrk];
  track->RangeMomentumMuonFlip      = (trRangeMomentumMuonFlip)[itrk];
  track->RangeMomentumProtonFlip    = (trRangeMomentumProtonFlip)[itrk];
#endif

  track->MomentumAtVertex    = (trMomentumAtVertex)[itrk];
  anaUtils::CopyArray(trDirectionAtVertex[itrk],   track->DirectionAtVertex,   3);


#if VERSION_HAS_TIME_FITS
 if(trNTimeNodes[itrk]>0){
  for (Int_t i=0;i<trNTimeNodes[itrk];i++){

    AnaTimeNode* node = new AnaTimeNode();

    node->Detector  = trTimeNodeDetector[itrk][i];
    node->TimeStart = trTimeNodeTimeStart[itrk][i];
    node->TimeEnd   = trTimeNodeTimeEnd[itrk][i];
    
    track->TimeNodes.push_back(node);
  }
 }

#endif

  // Save all recon tracks associated to this true track
  if (track->TrueObject){
    AnaTrueParticle* thistrueobj = static_cast<AnaTrueParticle*>(track->TrueObject);

    // Vectors are already filled with null pointers --> fill, remove last one (that should be null) and sort
    thistrueobj->ReconParticles.insert(thistrueobj->ReconParticles.begin(),track);

    if ( ! suppress_err_msg)
      if (thistrueobj->ReconParticles.back())
        std::cout << "minor error 73731" << std::endl;
    if ( ! thistrueobj->ReconParticles.back()) thistrueobj->ReconParticles.pop_back();
    // sort here to keep handle associations from different bunches
//    std::sort(thistrueobj->ReconParticles.begin(), thistrueobj->ReconParticles.end(), AnaTrack::CompareMomentum);

    // Save all recon tracks associated to this true  vertex
    if (track->GetTrueParticle()->TrueVertex){
      AnaTrueVertex* thistrueobj = static_cast<AnaTrueVertex*>(track->GetTrueParticle()->TrueVertex);

      // Vectors are already filled with null pointers --> fill, remove last one (that should be null) and sort
      thistrueobj->ReconParticles.insert(thistrueobj->ReconParticles.begin(),track);
      if ( ! suppress_err_msg)
        if (thistrueobj->ReconParticles.back())
          std::cout << "minor error 73741" << std::endl;
      if ( ! thistrueobj->ReconParticles.back()) thistrueobj->ReconParticles.pop_back();
      // sort here to keep handle associations from different bunches
//      std::sort(thistrueobj->ReconParticles.begin(), thistrueobj->ReconParticles.end(), AnaTrack::CompareMomentum);
    }
  }
}
//*****************************************************************************
void FlatTreeConverter::FillSubdetectorInfo(std::vector<AnaTrueParticleB*>& trueParticles, int itrk, AnaTrackB* trackB){
//*****************************************************************************

  RedoTreeConverter::FillSubdetectorInfo(trueParticles,itrk,trackB);

  AnaTrack* track = static_cast<AnaTrack*>(trackB);

  track->nSMRDSegments = 0;
  track->nP0DSegments  = 0;

  for (int i = 0; i < SubDetId::kInvalidSubdetector; ++i){
    SubDetId::SubDetEnum idet = static_cast<SubDetId::SubDetEnum>(i);
    if(!SubDetId::GetDetectorUsed(track->Detector, idet)) continue;
    if(SubDetId::IsP0DDetector(idet)){
      if (track->nP0DSegments==(int)NMAXP0DS)
        continue;
      AnaP0DParticle* seg = static_cast<AnaP0DParticle*>(MakeP0dTrack());
      int p0d = 0;
      FillP0dInfo(itrk, p0d, seg);
      track->P0DSegments[track->nP0DSegments++] = seg;
    }
    else if(SubDetId::IsSMRDDetector(idet)){
      if (track->nSMRDSegments==(int)NMAXSMRDS)
        continue;
      AnaSMRDParticle* seg = static_cast<AnaSMRDParticle*>(MakeSmrdTrack());
      int smrd = convUtils::GetLocalDetEnum(SubDetId::kSMRD, idet);
      FillSmrdInfo(itrk, smrd, seg);
      track->SMRDSegments[track->nSMRDSegments++] = seg;
    }
  }

  int nTracker = std::min(trNTRACKERs[itrk], (int)NMAXTRACKERS);
  for (int j=0;j<nTracker;j++){
    AnaTrackerTrack* seg = static_cast<AnaTrackerTrack*>(MakeTrackerTrack());
    FillTrackerInfo(itrk, j, seg);
    track->TRACKERSegments.push_back(seg);
  }


}

//*****************************************************************************
void FlatTreeConverter::FillTpcInfo(std::vector<AnaTrueParticleB*>& trueParticles, int itrk, int tpc, AnaTPCParticleB* segB){
//*****************************************************************************    
    
    RedoTreeConverter::FillTpcInfo(trueParticles,itrk, tpc, segB);

    AnaTPCParticle* seg = static_cast<AnaTPCParticle*>(segB);

    seg->NHits          = (int)(trTpcNHits)[itrk][tpc];
    seg->UniqueID       = (int)(trTpcUniqueID)[itrk][tpc];
    seg->MomentumEnd    = (trTpcBackMomentum)[itrk][tpc];
    seg->Length         = (trTpcLength)[itrk][tpc];

#if VERSION_HAS_BFIELD_REFIT
    seg->RefitMomentum  = (trTpcRefitMomentum)[itrk][tpc];
#endif
#if VERSION_HAS_BFIELD_REFIT_FULL
    seg->RefitCharge    = (trTpcRefitCharge)[itrk][tpc];
    anaUtils::VectorToArray(TLorentzVector((trTpcRefitPosition)[itrk][tpc][0],
					(trTpcRefitPosition)[itrk][tpc][1],
					(trTpcRefitPosition)[itrk][tpc][2],
					(trTpcRefitPosition)[itrk][tpc][3]), seg->RefitPosition);
    anaUtils::VectorToArray(TVector3((trTpcRefitDirection)[itrk][tpc][0],
				  (trTpcRefitDirection)[itrk][tpc][1],
				  (trTpcRefitDirection)[itrk][tpc][2]), seg->RefitDirection);
#endif
#if VERSION_HAS_EFIELD_REFIT
    seg->EFieldRefitMomentum  = (trTpcEFieldRefitMomentum)[itrk][tpc];
#endif

    seg->Pullmu  = (trTpcPullmu)[itrk][tpc];
    seg->Pullele = (trTpcPullele)[itrk][tpc];
    seg->Pullp   = (trTpcPullp)[itrk][tpc];
    seg->Pullpi  = (trTpcPullpi)[itrk][tpc];
    seg->Pullk   = (trTpcPullk)[itrk][tpc];

    if(_useCorrectedValues){
      seg->dEdxMeas      = (trTpcdEdxMeas)[itrk][tpc];      
      seg->dEdxexpMuon   = (trTpcdEdxExpMu)[itrk][tpc];
      seg->dEdxexpEle    = (trTpcdEdxExpEle)[itrk][tpc];
      seg->dEdxexpProton = (trTpcdEdxExpP)[itrk][tpc];
      seg->dEdxexpPion   = (trTpcdEdxExpPi)[itrk][tpc];
      seg->dEdxexpKaon   = (trTpcdEdxExpK)[itrk][tpc];
    }
    else{
      seg->dEdxMeas      = (trTpcRawdEdxMeas)[itrk][tpc];      
      seg->dEdxexpMuon   = (trTpcRawdEdxExpMu)[itrk][tpc];
      seg->dEdxexpEle    = (trTpcRawdEdxExpEle)[itrk][tpc];
      seg->dEdxexpProton = (trTpcRawdEdxExpP)[itrk][tpc];
      seg->dEdxexpPion   = (trTpcRawdEdxExpPi)[itrk][tpc];
      seg->dEdxexpKaon   = (trTpcRawdEdxExpK)[itrk][tpc];
    }

    seg->dEdxSigmaKaon   = (trTpcdEdxSigmaK)[itrk][tpc];

    seg->Purity   = (trTpcPurity)[itrk][tpc];


    for (int i=0;i<3;i++){
      seg->DirectionEnd[i]   = (trTpcDirectionEnd)[itrk][tpc][i];
    }
}

//*****************************************************************************
void FlatTreeConverter::FillFgdInfo(int itrk, int fgd, AnaFGDParticleB* segB){
//*****************************************************************************    

  RedoTreeConverter::FillFgdInfo(itrk, fgd, segB);
  
  AnaFGDParticle* seg = static_cast<AnaFGDParticle*>(segB);

  seg->NHits          = (int)(trFgdNHits)[itrk][fgd];
  seg->UniqueID       = (int)(trFgdUniqueID)[itrk][fgd];
  seg->Length         = (trFgdLength)[itrk][fgd];
  
  seg->E  = (trFgdE)[itrk][fgd];
  
  seg->AvgTime      = (trFgdAvgTime)[itrk][fgd];

#if VERSION_HAS_FGD_VERTEX_AND_END_ACTIVITY  
  seg->HasFgdVA       = (trFgdHasFgdVA)[itrk][fgd];
  seg->HasFgdVA_fmode = (trFgdHasFgdVA_fmode)[itrk][fgd];
  seg->TypeVA         = (trFgdTypeVA)[itrk][fgd];
#endif
  
  seg->Vertex1x1  = (trFgdVertex1x1)[itrk][fgd];
  seg->Vertex3x3  = (trFgdVertex3x3)[itrk][fgd];
  seg->Vertex5x5  = (trFgdVertex5x5)[itrk][fgd];
  seg->Vertex7x7  = (trFgdVertex7x7)[itrk][fgd];
  seg->VertexLayer  = (trFgdVertexLayer)[itrk][fgd];

#if VERSION_HAS_FGD_VERTEX_AND_END_ACTIVITY  
  seg->HasFgdEA       = (trFgdHasFgdEA)[itrk][fgd];
  seg->HasFgdEA_fmode = (trFgdHasFgdEA_fmode)[itrk][fgd];
  seg->TypeEA         = (trFgdTypeEA)[itrk][fgd];
  
  seg->End0x0  = (trFgdEnd0x0)[itrk][fgd];
  seg->End1x1  = (trFgdEnd1x1)[itrk][fgd];
  seg->End3x3  = (trFgdEnd3x3)[itrk][fgd];
  seg->End5x5  = (trFgdEnd5x5)[itrk][fgd];
  seg->End7x7  = (trFgdEnd7x7)[itrk][fgd];
#endif

  for (int i=0;i<3;i++){
    seg->DirectionEnd[i]   = (trFgdDirectionEnd)[itrk][fgd][i];
  }
}

//*****************************************************************************
void FlatTreeConverter::FillEcalInfo(int itrk, int ecal, AnaECALParticleB* segB){
//*****************************************************************************    

  RedoTreeConverter::FillEcalInfo(itrk, ecal, segB);
  
  AnaECALParticle* seg = static_cast<AnaECALParticle*>(segB);

  seg->NHits                = (int)(trECALNHits)[itrk][ecal];
  seg->UniqueID             = (int)(trECALUniqueID)[itrk][ecal];
  seg->PIDMipPion           = trECALPIDMipPion[itrk][ecal];
  seg->Containment          = trECALContainment[itrk][ecal];
  seg->EDeposit             = (trECALEDeposit)[itrk][ecal];
  seg->IsShowerLike         = (int)(trECALIsShowerLike)[itrk][ecal];
  seg->AvgTime              = (int)(trECALAvgTime)[itrk][ecal];

  for (int i = 0; i < 4; i++){
    seg->PositionStart[i] = (trECALPositionStart)[itrk][ecal][i];
    seg->PositionEnd[i]   = (trECALPositionEnd)[itrk][ecal][i];  
  }

  for (int i = 0; i < 3; i++){
    seg->DirectionStart[i] = (trECALDirectionStart)[itrk][ecal][i];
    seg->DirectionEnd[i]   = (trECALDirectionEnd)[itrk][ecal][i];
  }
    
}

//*****************************************************************************
void FlatTreeConverter::FillSmrdInfo(int itrk, int smrd, AnaSMRDParticleB* segB){
//*****************************************************************************    

  RedoTreeConverter::FillSmrdInfo(itrk, smrd, segB);
  
  AnaSMRDParticle* seg = static_cast<AnaSMRDParticle*>(segB);

  seg->NHits    = (int)(trSMRDNHits)[itrk][smrd];
  seg->NNodes   = (int)(trSMRDNNodes)[itrk][smrd];
  seg->UniqueID = (int)(trSMRDUniqueID)[itrk][smrd];
  seg->Length   = (trSMRDLength)[itrk][smrd];
    
  seg->EDeposit = (trSMRDEDeposit)[itrk][smrd];
  
  for (int i=0;i<3;i++){
    seg->DirectionEnd[i]   = (trSMRDDirectionEnd)[itrk][smrd][i];
  }
}

//*****************************************************************************
void FlatTreeConverter::FillP0dInfo(int itrk, int p0d, AnaP0DParticleB* segB){
//*****************************************************************************    

  RedoTreeConverter::FillP0dInfo(itrk, p0d, segB);
  
  AnaP0DParticle* seg = static_cast<AnaP0DParticle*>(segB);

  seg->NHits          = (int)(trP0DNHits)[itrk][p0d];
  seg->NNodes         = (int)(trP0DNNodes)[itrk][p0d];
  seg->UniqueID       = (int)(trP0DUniqueID)[itrk][p0d];
  seg->Length         = (trP0DLength)[itrk][p0d];

  for (int i=0;i<3;i++){
    seg->DirectionEnd[i]   = (trP0DDirectionEnd)[itrk][p0d][i];
  }
  
  
#if VERSION_HAS_P0D_AVERAGED_TIME  
  seg->AvgTime        = (trP0DAvgTime)[itrk][p0d];
#endif  

}

//*****************************************************************************
void FlatTreeConverter::FillTrackerInfo(int itrk, int tracker, AnaTrackerTrack* seg){
//*****************************************************************************    

  convUtils::ConvertTrackerDetEnumToBitField(seg->Detector, (trTRACKERDetector)[itrk][tracker]);

  seg->NHits          = (int)(trTRACKERNHits)[itrk][tracker];
  seg->NNodes         = (int)(trTRACKERNNodes)[itrk][tracker];
  seg->UniqueID       = (int)(trTRACKERUniqueID)[itrk][tracker];
  seg->Length         = (trTRACKERLength)[itrk][tracker];
  seg->Charge         = (trTRACKERCharge)[itrk][tracker];
  seg->Momentum       = (trTRACKERMomentum)[itrk][tracker];
  seg->MomentumEnd    = (trTRACKERMomentumEnd)[itrk][tracker];

  for (int i=0;i<4;i++){
    seg->PositionStart[i] = (trTRACKERPositionStart)[itrk][tracker][i];
    seg->PositionEnd[i]   = (trTRACKERPositionEnd)[itrk][tracker][i];
  }
  
  for (int i=0;i<3;i++){
    seg->DirectionStart[i] = (trTRACKERDirectionStart)[itrk][tracker][i];
    seg->DirectionEnd[i]   = (trTRACKERDirectionEnd)[itrk][tracker][i];
  }
}

//*****************************************************************************
void FlatTreeConverter::FillTrueParticleInfo(int ntrueVertices, AnaTrueVertexB** trueVertices, int itrk, AnaTrueParticleB* partB){
//*****************************************************************************

  RedoTreeConverter::FillTrueParticleInfo(ntrueVertices, trueVertices, itrk, partB);

  AnaTrueParticle* part = static_cast<AnaTrueParticle*>(partB);

  part->Purity     = (sTrueParticlePurity)[itrk];
  part->Bunch      = (sTrueParticleBunch)[itrk];

  part->VertexIndex  = (sTrueParticleVertexIndex)[itrk];


  // TODO: Use the same code as in psyche for the moment. This is however not optimised since we loop over all vertices again 
  // this was already done in oaAnalysisTreeConverter to create the FlatTree
  // The code below is supposed to be warking

  /*
  int nMaxTrueParticles = (int)(anaUtils::size_array(part->TrueVertex->TrueParticles));

  if (part->VertexIndex>-1 && part->VertexIndex<(int)(_spill->TrueVertices.size())){

    if(_spill->TrueVertices[part->VertexIndex]){
      // Get the true vertex associated to this part
      part->TrueVertex = _spill->TrueVertices[part->VertexIndex];

      if(part->TrueVertex->nTrueParticles<nMaxTrueParticles)
        // add this part to the list of true parts in the true vertex
        part->TrueVertex->TrueParticles[part->TrueVertex->nTrueParticles++] = part;	
    }

  } else if (part->VertexIndex!=-1)
      if ( ! suppress_err_msg) std::cout << "minor error 6543" << std::endl;
  */

}

//*****************************************************************************
void FlatTreeConverter::FillTrueVertexInfo(int ivtx, AnaTrueVertexB* vertexB){
//*****************************************************************************

  RedoTreeConverter::FillTrueVertexInfo(ivtx,vertexB);

  AnaTrueVertex* vertex = static_cast<AnaTrueVertex*>(vertexB);

  vertex->RooVtxEntry = _RooVtxEntryInCurrentInputFile; 
  
  vertex->ReacCode = (sTrueVertexReacCode)[ivtx];

  anaUtils::CopyArray(sTrueVertexPosition[ivtx], vertex->Position, 4);

  vertex->Q2       = (sTrueVertexQ2)[ivtx];
  vertex->NuEnergy = (sTrueVertexNuEnergy)[ivtx];

  anaUtils::CopyArray(sTrueVertexNuDir[ivtx],     vertex->NuDir, 3);
  anaUtils::CopyArray(sTrueVertexLeptonDir[ivtx], vertex->LeptonDir, 3);
  anaUtils::CopyArray(sTrueVertexPionDir[ivtx],   vertex->PionDir, 3);
  anaUtils::CopyArray(sTrueVertexTargetDir[ivtx], vertex->TargetDir, 3);
  anaUtils::CopyArray(sTrueVertexProtonDir[ivtx], vertex->ProtonDir, 3);

  vertex->NuParentPDG      = sTrueVertexNuParentPDG[ivtx];

  anaUtils::CopyArray(sTrueVertexNuParentDecPoint[ivtx], vertex->NuParentDecPoint, 3);

  vertex->NuPDG     = (sTrueVertexNuPDG)[ivtx];
  vertex->TargetPDG = (sTrueVertexTargetPDG)[ivtx];
  vertex->LeptonPDG = (sTrueVertexLeptonPDG)[ivtx];
  vertex->LeptonMom = sTrueVertexLeptonMom[ivtx];
  vertex->ProtonMom = sTrueVertexProtonMom[ivtx];
  vertex->PionMom   = sTrueVertexPionMom[ivtx];
  vertex->TargetMom = sTrueVertexTargetMom[ivtx];


  vertex->Detector = (sTrueVertexDetector)[ivtx];
  vertex->NBaryons = sTrueVertexNBaryons[ivtx];
  vertex->Bunch    = sTrueVertexBunch[ivtx];

  vertex->RooVtxIndex = sTrueVertexRooVtxIndex[ivtx];
  vertex->RooVtxEntry = sTrueVertexRooVtxEntry[ivtx];
  vertex->ID = sTrueVertexID[ivtx];

  // will be updated in FillTrueParticleInfo
  anaUtils::CreateArray(vertex->TrueParticles, sNTrueParticles);
  vertex->nTrueParticles = 0;

  vertex->IsPauliBlocked = sTrueVertexIsPauliBlocked[ivtx];
  vertex->IsCohOnH       = sTrueVertexIsCohOnH[ivtx];

  for (int j = 0; j < Int_t(ParticleId::kLast)+1; j++) {
    vertex->NPrimaryParticles[j]       = sTrueVertexNPrimaryParticles[ivtx][j];
  }
}

//*****************************************************************************
bool FlatTreeConverter::FillTrueVertexRooInfo(AnaTrueVertexB* vertexB) {
//*****************************************************************************

  // Don't recompute RooTrackerVtx derived variables, when: 
  //   1. We don't read the input RooTrackerVtx, 
  //   2. When all variables are already available in the FlatTree and we don't want to recompute them from the input RooTrackerVtx
  // Note that with flattrees produced with highland2 v0 these vars were slighty wrong and should be recomputed

  if (!_readRooTrackerVtx || (fChain->FindLeaf("sTrueVertexIsPauliBlocked"))) return true;

  bool foundCohOnH,foundPauliBlocked;
  return anaUtils::FillTrueVertexRooInfo(vertexB,RunID,fNeut,fGenie,NNVtx,NVtx,NGVtx,GVtx,foundCohOnH,foundPauliBlocked);
}

//*****************************************************************************
void FlatTreeConverter::FillNPrimaryParticles(AnaTrueVertex* vertex){
//*****************************************************************************

  // if RooTrackerVtx is not available fill the new NPrimaryParticles (used in Categories.cxx) using TrueParticles

  for (int i = 0; i<vertex->nTrueParticles; i++) {
    AnaTrueParticleB* trueTrack = vertex->TrueParticles[i];

    if(!trueTrack) continue;

    if(trueTrack->ParentPDG != 0) continue; //should correspond to the primary vertex

    int index = ParticleId::GetParticle(trueTrack->PDG,false);
    vertex->NPrimaryParticles[index]++;
    if (abs(trueTrack->PDG) > 1000 && abs(trueTrack->PDG) < 10000) vertex->NPrimaryParticles[ParticleId::kBaryons]++;
    if (abs(trueTrack->PDG) > 100 && abs(trueTrack->PDG) < 1000) vertex->NPrimaryParticles[ParticleId::kMesons]++;
    if (abs(trueTrack->PDG) > 10 && abs(trueTrack->PDG) < 19) vertex->NPrimaryParticles[ParticleId::kLeptons]++;
    if (trueTrack->PDG == +12 || trueTrack->PDG == +14 || trueTrack->PDG == +16) vertex->NPrimaryParticles[ParticleId::kNeutrinos]++;
    if (trueTrack->PDG == -12 || trueTrack->PDG == -14 || trueTrack->PDG == -16) vertex->NPrimaryParticles[ParticleId::kAntiNeutrinos]++;

  } // end loop over vertex->TrueParticles

  // Fill NPrimaryParticles for kPions and Kaons
  vertex->NPrimaryParticles[ParticleId::kPions] = vertex->NPrimaryParticles[ParticleId::kPi0]   +
                                                  vertex->NPrimaryParticles[ParticleId::kPiPos] +
                                                  vertex->NPrimaryParticles[ParticleId::kPiNeg] ;
  vertex->NPrimaryParticles[ParticleId::kKaons] = vertex->NPrimaryParticles[ParticleId::kK0]     +
                                                  vertex->NPrimaryParticles[ParticleId::kAntiK0] +
                                                  vertex->NPrimaryParticles[ParticleId::kK0L]    +
                                                  vertex->NPrimaryParticles[ParticleId::kK0S]    +
                                                  vertex->NPrimaryParticles[ParticleId::kKPos]   +
                                                  vertex->NPrimaryParticles[ParticleId::kKNeg]   ;

}


// // NOT USED
// //*****************************************************************************
// int FlatTreeConverter::FindTrueVertex(int ID, AnaTrueVertex*& trueVertex){
// //*****************************************************************************

//   // Compare the IDs to find the AnaTrueVertex associated with this recon particle.

//   trueVertex = NULL;
//   int index=0;
//   std::vector<AnaTrueVertex*>::iterator it;
//   for (it = _spill->TrueVertices.begin(); it!=_spill->TrueVertices.end();it++){
//     AnaTrueVertex* trueVertex2 = *it;
//     if (ID == trueVertex2->ID){
//       trueVertex = trueVertex2;
//       return index;
//     }
//     index++;
//   }
//   return -1;
// }


//*****************************************************************************
void FlatTreeConverter::FillVertexInfo(int ivtx, AnaVertexB* vertexB, AnaBunchB* bunch){
//*****************************************************************************

  RedoTreeConverter::FillVertexInfo(ivtx, vertexB, bunch);

  AnaVertex* vertex = static_cast<AnaVertex*>(vertexB);

  anaUtils::CopyArray(vVariance[ivtx], vertex->Variance, 4);

  vertex->Chi2         = (vChi2)[ivtx];
  vertex->NDOF         = (vNDOF)[ivtx];

  // Find AnaTrack associated with this global vertex
  vertex->nParticles=0;

  anaUtils::CreateArray(vertex->Particles, vNReconParticles[ivtx]);

  for (int d = 0; d < vNReconParticles[ivtx]; d++) {
    AnaParticle* part = static_cast<AnaParticle*>(FindParticle(vParticlesUniqueID[ivtx][d], bunch));
    if ( ! part) continue;

    // fill only if in this bunch, should be already sorted in flat tree, then null pointers will be added at the end
    if (part->Bunch != Bunch) continue;
    vertex->Particles[vertex->nParticles] = part;
    vertex->nParticles++;

    // Save vertices info in AnaTrack
    part->ReconVertices.push_back(vertex);

    // choose the vertex more primary (in this bunch)
    if ( ! part->ReconVertex || part->ReconVertex->PrimaryIndex > vertex->PrimaryIndex)
      part->ReconVertex = vertex;
  }

  // Find true vertices associated with this global vertex (by index)
  for (int d = 0; d < vNTrueVerticesMatch[ivtx]; d++) {
    std::pair<AnaTrueVertex*, AnaRecTrueMatch> myMatch;
    int thisTrueVertexIndex = vTrueVertexIndex[ivtx][d];
    if (thisTrueVertexIndex == -1) continue;
    if (thisTrueVertexIndex < -1) {
      if(_spill->TrueVertices.size() == NMAXTRUEVERTICES)
          std::cout << "The true vertex associated to this global vertex was not saved because NTrueVertices > NMAXTRUEVERTICES" << std::endl;
      else
          std::cout << "global vertex error 6789" << std::endl;
      continue;
    }

    myMatch.first = static_cast<AnaTrueVertex*>(_spill->TrueVertices.at(thisTrueVertexIndex));
    myMatch.second.Cleanliness  = (vTrueVerticesClean)[ivtx][d];
    myMatch.second.Completeness = (vTrueVerticesCompl)[ivtx][d];
    vertex->TrueVerticesMatch.push_back(myMatch);

    // Vectors are already filled with null pointers --> fill, remove last one (that should be null) and sort
    myMatch.first->ReconVertices.insert(myMatch.first->ReconVertices.begin(),vertex);
    if ( ! suppress_err_msg)
      if (myMatch.first->ReconVertices.back())
        std::cout << "minor error 73751" << std::endl;
    if ( ! myMatch.first->ReconVertices.back()) myMatch.first->ReconVertices.pop_back();
    // sort here to keep handle associations from different bunches
    std::sort(myMatch.first->ReconVertices.begin(), myMatch.first->ReconVertices.end(), AnaVertex::ComparePrimaryIndex);
  }

  // Let's keep also vertex->TrueVertex by now (the user could just called GetMainTrueVertex instead)
  vertex->TrueVertex = vertex->GetMainTrueVertex(false); // argument false to suppress warnings
}

//*****************************************************************************
AnaParticleB* FlatTreeConverter::FindParticle(int ID, AnaBunchB* bunch){
//*****************************************************************************

  // Compare the UniqueID to find the corresponding AnaTrack
  // AnaTrack already exists because it is saved for first

  AnaParticleB* part = RedoTreeConverter::FindParticle(ID,bunch);
  if (!part) return part;

  // If not found, look into all bunches
  // in prod 5 this could happens only when the vertex is OutOfBunch
#if ! VERSION_HAS_TIME_FITS
  if (bunch->Bunch != -1) {
    if ((int)bunch->Particles.size() == (int)NMAXPARTICLES) std::cout << "minor error: vertex constituent not found (UniqueID " << ID << "), likely because this event has too many tracks!" << std::endl;
    else if ( ! suppress_err_msg) std::cout << "minor error in oaAnalysisConverter (ref 6056)" << std::endl;
    return NULL;
  }
#endif

  // Look into all bunches
  std::vector<AnaBunchC*>::iterator itbunch;
  std::vector<AnaParticleB*>::iterator ittrack;
  for (itbunch = _spill->Bunches.begin(); itbunch != _spill->Bunches.end(); itbunch++) {
    AnaBunchB* bunch0 = static_cast<AnaBunchB*>(*itbunch);    
    for (ittrack = bunch0->Particles.begin(); ittrack != bunch0->Particles.end(); ittrack++) {
      if (ID == (*ittrack)->UniqueID) {
        if (bunch->Bunch != -1) { // often a global vertex out of bunch has constituents in the near bunch
          std::cout << "INFO: this global vertex (in bunch " << bunch->Bunch << ") has a constituent stored in another bunch!" << std::endl;
        }
        return static_cast<AnaTrack*>(*ittrack);
      }
    }
  }

  if ( ! suppress_err_msg) {
    if ((int)bunch->Particles.size() == (int)NMAXPARTICLES) std::cout << "minor error: vertex constituent not found (UniqueID " << ID << "), likely because this event has too many tracks!" << std::endl;
    else std::cout << "minor error: vertex constituent not found in bunch " << bunch->Bunch << ": in prod6 it should be a flipped track with start position in a bunch not yet loaded!" << std::endl;
  }
  return NULL;
}


//*****************************************************************************
void FlatTreeConverter::FillTECALReconObjectInfo(int iObj, AnaTECALReconObject* anaTECAL, AnaBunch* bunch){
//*****************************************************************************

  (void)bunch;

  anaTECAL->AverageHitTime           = tecalReconAverageHitTime          [iObj];
  anaTECAL->AverageZPos              = tecalReconAverageZPosition        [iObj];
  anaTECAL->Containment              = tecalReconContainment             [iObj];
  anaTECAL->EFitResult               = tecalReconEFitResult              [iObj];
  anaTECAL->EFitUncertainty          = tecalReconEFitUncertainty         [iObj];
  anaTECAL->FirstLayer               = tecalReconFirstLayer              [iObj];
  anaTECAL->IsShowerLike             = tecalReconIsShowerLike            [iObj];
  anaTECAL->IsTrackLike              = tecalReconIsTrackLike             [iObj];
  anaTECAL->LastLayer                = tecalReconLastLayer               [iObj];

#if VERSION_HAS_ECAL_LLR
  anaTECAL->LikeMIPEM                = tecalReconLikeMIPEM               [iObj];
  anaTECAL->LikeMIPPion              = tecalReconLikeMIPPion             [iObj];
  anaTECAL->LikeEMHIP                = tecalReconLikeEMHIP               [iObj];
  anaTECAL->LikeMIPEMLow             = tecalReconLikeMIPEMLow            [iObj];
#endif

  anaTECAL->MatchingLike             = tecalReconMatchingLike            [iObj];
  anaTECAL->MichelTagNDelayedCluster = tecalReconMichelTagNDelayedCluster[iObj];
  anaTECAL->Module                   = tecalReconModule                  [iObj];
  anaTECAL->MostDownStreamLayerHit   = tecalReconMostDownStreamLayerHit  [iObj];
  anaTECAL->MostUpStreamLayerHit     = tecalReconMostUpStreamLayerHit    [iObj];
  anaTECAL->NHits                    = tecalReconNHits                   [iObj];
  anaTECAL->NLayersHit               = tecalReconNLayersHit              [iObj];
  anaTECAL->ObjectLength             = tecalReconObjectLength            [iObj];
  anaTECAL->PIDAMR                   = tecalReconPIDAMR                  [iObj];
  anaTECAL->PIDAngle                 = tecalReconPIDAngle                [iObj];
  anaTECAL->PIDAsymmetry             = tecalReconPIDAsymmetry            [iObj];
  anaTECAL->PIDCircularity           = tecalReconPIDCircularity          [iObj];
  anaTECAL->PIDFBR                   = tecalReconPIDFBR                  [iObj];
  anaTECAL->PIDMaxRatio              = tecalReconPIDMaxRatio             [iObj];
  anaTECAL->PIDMeanPos               = tecalReconPIDMeanPos              [iObj];
  anaTECAL->PIDShowerAngle           = tecalReconPIDShowerAngle          [iObj];
  anaTECAL->PIDShowerWidth           = tecalReconPIDShowerWidth          [iObj];
  anaTECAL->PIDTransverseChargeRatio = tecalReconPIDTransverseChargeRatio[iObj];
  anaTECAL->PIDTruncatedMaxRatio     = tecalReconPIDTruncatedMaxRatio    [iObj];
  anaTECAL->Thrust                   = tecalReconThrust                  [iObj];
  anaTECAL->TimeBunch                = tecalReconTimeBunch               [iObj];
  anaTECAL->TotalHitCharge           = tecalReconTotalHitCharge          [iObj];
  anaTECAL->TrueID                   = tecalReconTrueID                  [iObj];
  anaTECAL->TrueIDPrimary            = tecalReconTrueIDPrimary           [iObj];
  anaTECAL->TrueIDRecursive          = tecalReconTrueIDRecursive         [iObj];
  anaTECAL->TrueIDSingle             = tecalReconTrueIDSingle            [iObj];
  anaTECAL->UniqueID                 = tecalReconUniqueID                [iObj];
  anaTECAL->Bunch                    = tecalReconBunch                   [iObj];

  anaUtils::CopyArray(tecalReconPointing    [iObj], anaTECAL->Pointing,     3);
  anaUtils::CopyArray(tecalReconThrustAxis  [iObj], anaTECAL->ThrustAxis,   3);
  anaUtils::CopyArray(tecalReconThrustOrigin[iObj], anaTECAL->ThrustOrigin, 3);
  return;
}
