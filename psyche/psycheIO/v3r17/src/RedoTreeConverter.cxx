#include "RedoTreeConverter.hxx"
#include "BasicUtils.hxx"
#include "ND280AnalysisUtils.hxx"
#include "TreeConverterUtils.hxx"
#include "TDataType.h"
#include "TLeaf.h"
#include "TROOT.h"

// set true for avoiding error messages that might appear when using old flat trees
bool suppress_err_msg = true;

//********************************************************************
RedoTreeConverter::RedoTreeConverter():InputConverter("flattree"){
  //********************************************************************

  _firstFile = true; 
  _entry_roo=0;
  _currentfilename=""; 


  AddChain(_treeName);

  flattree = GetChain(_treeName);
  NRooTrackerVTX = NULL;
  GRooTrackerVTX = NULL;
  _spill = NULL;
  _prevEvent=NULL;


  fChain = flattree;


  // initialize to 0 the number of entries
  _nentries =0;

  // Initialise all the variables. This is particularly important as some
  // branches many not exist in the flat tree.
  sPOTSincePreviousSpill = 0;
  sRun                   = 0;
  sSubrun                = 0;
  sEvt                   = 0;
  sEventTime             = 0;
  sNTotalTrueParticles   = 0;
  sNTotalTrueVertices    = 0;

  sBeamGoodSpill = 0;
  sDQGoodDaq     = 0;


  sNTrueVertices = 0;
  for (UInt_t i = 0; i < NMAXTRUEVERTICES; i++) {
    sTrueVertexID[i]             = 0;
    sTrueVertexRooVtxIndex[i]    = 0;
    sTrueVertexRooVtxEntry[i]    = 0;
    sTrueVertexReacCode[i]       = 0;
    sTrueVertexNTrueParticles[i] = 0;
    sTrueVertexNBaryons[i]       = 0;
    sTrueVertexBunch[i]          = 0;
    sTrueVertexDetector[i]       = 0;
    sTrueVertexNuPDG[i]          = 0;
    sTrueVertexTargetPDG[i]      = 0;
    sTrueVertexNuEnergy[i]       = 0;
    sTrueVertexQ2[i]             = 0;
    sTrueVertexNuParentPDG[i]    = 0;

    for (UInt_t j = 0; j < 4; j++) {
      sTrueVertexPosition[i][j]         = 0;
      sTrueVertexNuParentDecPoint[i][j] = 0;
    }

    for (UInt_t j = 0; j < 3; j++) {
      sTrueVertexNuDir[i][j] = 0;
    }
  }

  sNTrueParticles = 0;
  for (UInt_t i = 0; i < NMAXTRUEPARTICLES; i++) {
    sTrueParticleID                              [i] = 0;
    sTrueParticleBunch                           [i] = 0;
    sTrueParticlePDG                             [i] = 0;
    sTrueParticleParentPDG                       [i] = 0;
    sTrueParticleGParentPDG                      [i] = 0;
    sTrueParticlePrimaryID                       [i] = 0;
    sTrueParticleParentID                        [i] = 0;
    sTrueParticleGParentID                       [i] = 0;
    sTrueParticlePurity                          [i] = 0;
    sTrueParticleMomentum                        [i] = 0;
    sTrueParticleCharge                          [i] = 0;
    sTrueParticleVertexIndex                     [i] = 0;
    sTrueParticleTruthVertexID                   [i] = 0;
    sTrueParticleNReconTracks                    [i] = 0;
    sTrueParticleIsTruePrimaryPi0DecayPhoton     [i] = 0;
    sTrueParticleIsTruePrimaryPi0DecayPhotonChild[i] = 0;

    for (int j = 0; j < 4; j++) {
      sTrueParticlePosition[i][j]    = 0;
      sTrueParticlePositionEnd[i][j] = 0;
    }
    for (int j = 0; j < 3; j++) {
      sTrueParticleDirection[i][j] = 0;
    }
    sTrueParticleNDetCrossings[i] = 0;
    for (UInt_t j = 0; j < NMAXCROSSEDDET; j++) {
      for (int k = 0; k < 4; k++) {
        sTrueParticleEntrancePosition[i][j][k] = 0;
        sTrueParticleExitPosition[i][j][k]     = 0;
      }
      for (int k = 0; k < 3; k++) {
        sTrueParticleEntranceMomentum[i][j][k] = 0;
        sTrueParticleExitMomentum[i][j][k]     = 0;
      }
      sTrueParticleDetector[i][j] = 0;
      sTrueParticleInActive[i][j] = 0;
    }
  }

  sNFgdTimeBins = 0;
  for (UInt_t i = 0; i < NMAXFGDTIMEBINS; i++) {
    sFgdTimeBinMinTime[i]       = 0;
    sFgdTimeBinMaxTime[i]       = 0;
    sFgdTimeBinNHits1[i]        = 0;
    sFgdTimeBinNHits2[i]        = 0;
    sFgdTimeBinRawChargeSum1[i] = 0;
    sFgdTimeBinRawChargeSum2[i] = 0;
    sFgdTimeBinG4ID[i]          = 0;
  }

  Bunch  = 0;
  Weight = 1;
  NVertices = 0;

  for (UInt_t i = 0; i < NMAXVERTICES; i++) {
    vPrimaryIndex[i]       = 0;
    vBunch[i]              = 0;
    vChi2[i]               = 0;
    vNDOF[i]               = 0;
    vNReconParticles[i]    = 0;
    vNTrueVerticesMatch[i] = 0;

    for (int j = 0; j < 4; j++) {
      vPosition[i][j] = 0;
      vVariance[i][j] = 0;
    }

    for (UInt_t j = 0; j < NMAXPARTICLESINVERTEX; j++) {
      vParticlesUniqueID[i][j] = 0;
    }

    for (UInt_t j = 0; j < NMAXTRUEVERTICESMATCH; j++) {
      vTrueVertexIndex[i][j]   = 0;
      vTrueVerticesClean[i][j] = 0;
      vTrueVerticesCompl[i][j] = 0;
    }
  }

  NParticles = 0;
  for (UInt_t i = 0; i < NMAXPARTICLES; i++) {
    trUniqueID[i]            = 0;
    trStatus[i]              = 0;
    trBunch[i]               = 0;
    trNReconVertices[i]      = 0;
    trDetectors[i]           = 0;
    trNTPCs[i]               = 0;
    trNFGDs[i]               = 0;
    trNECALs[i]              = 0;
    trNSMRDs[i]              = 0;
    trNP0Ds[i]               = 0;
    trNHits[i]               = 0;
    trNNodes[i]              = 0;
    trNDOF[i]                = 0;
    trChi2[i]                = 0;
    trCharge[i]              = 0;
    trMomentum[i]            = 0;
    trMomentumFlip[i]        = 0;
    trToFFGD1_FGD2[i]        = 0;
    trToFP0D_FGD1[i]         = 0;
    trToFDSECal_FGD1[i]      = 0;
    trToFECal_FGD1[i]        = 0;
    trToFDSECal_FGD2[i]      = 0;
    trToFECal_FGD2[i]        = 0;
    trToFFlag_FGD1_FGD2[i]   = 0;
    trToFFlag_P0D_FGD1[i]    = 0;
    trToFFlag_DSECal_FGD1[i] = 0;
    trToFFlag_ECal_FGD1[i]   = 0;
    trToFFlag_DSECal_FGD2[i] = 0;
    trToFFlag_ECal_FGD2[i]   = 0;
    trMomentumError[i]       = 0;
    trMomentumMuon[i]        = 0;
    trMomentumErrorMuon[i]   = 0;
    trMomentumProton[i]      = 0;
    trMomentumErrorProton[i] = 0;
    trMomentumEle[i]         = 0;
    trMomentumErrorEle[i]    = 0;
    trRangeMomentumEle[i]    = 0;
    trRangeMomentumMuon[i]   = 0;
    trRangeMomentumProton[i] = 0;
    trTrueParticleID[i]      = 0;

    for (UInt_t j = 0; j < NDETECTORS; j++) {
      trDetUsed[i][j] = 0;
    }

    for (int j = 0; j < 3; j++) {
      trDirectionStart[i][j] = 0;
      trDirectionEnd[i][j]   = 0;
    }

    for (int j = 0; j < 4; j++) {
      trPositionStart[i][j] = 0;
      trPositionEnd[i][j]   = 0;
    }

    for (int j = 0; j < 3; j++) {
      trTpcDetector[i][j]            = 0;
      trTpcNHits[i][j]               = 0;
      trTpcNNodes[i][j]              = 0;
      trTpcLength[i][j]              = 0;
      trTpcCharge[i][j]              = 0;
      trTpcMomentum[i][j]            = 0;
      trTpcMomentumError[i][j]       = 0;
      trTpcBackMomentum[i][j]        = 0;
      trTpcRefitCharge[i][j]         = 0;
      trTpcRefitMomentum[i][j]       = 0;
      trTpcEFieldRefitMomentum[i][j] = 0;
      trTpcdEdxMeas[i][j]            = 0;
      trTpcdEdxExpMu[i][j]           = 0;
      trTpcdEdxExpEle[i][j]          = 0;
      trTpcdEdxExpP[i][j]            = 0;
      trTpcdEdxExpPi[i][j]           = 0;
      trTpcdEdxExpK[i][j]            = 0;
      trTpcdEdxSigmaMu[i][j]         = 0;
      trTpcdEdxSigmaEle[i][j]        = 0;
      trTpcdEdxSigmaP[i][j]          = 0;
      trTpcdEdxSigmaPi[i][j]         = 0;
      trTpcdEdxSigmaK[i][j]          = 0;
      trTpcPurity[i][j]              = 0;
      trTpcTrueParticleID[i][j]      = 0;

      for (int k = 0; k < 3; k++) {
        trTpcRefitDirection[i][j][k] = 0;
        trTpcDirectionStart[i][j][k] = 0;
        trTpcDirectionEnd[i][j][k]   = 0;
      }

      for (int k = 0; k < 4; k++) {
        trTpcRefitPosition[i][j][k] = 0;
        trTpcPositionStart[i][j][k] = 0;
        trTpcPositionEnd[i][j][k]   = 0;
      }
    }

    for (int j = 0; j < 2; j++) {
      trFgdDetector[i][j]    = 0;
      trFgdNHits[i][j]       = 0;
      trFgdNNodes[i][j]      = 0;
      trFgdLength[i][j]      = 0;
      trFgdX[i][j]           = 0;
      trFgdE[i][j]           = 0;
      trFgdPullmu[i][j]      = 0;
      trFgdPullp[i][j]       = 0;
      trFgdPullpi[i][j]      = 0;
      trFgdPullno[i][j]      = 0;
      trFgdContainment[i][j] = 0;
      trFgdAvgTime[i][j]     = 0;
      trFgdVertex1x1[i][j]   = 0;
      trFgdVertex3x3[i][j]   = 0;
      trFgdVertex5x5[i][j]   = 0;
      trFgdVertex7x7[i][j]   = 0;
      trFgdVertexLayer[i][j] = 0;

      for (int k = 0; k < 3; k++) {
        trFgdDirectionStart[i][j][k] = 0;
        trFgdDirectionEnd[i][j][k]   = 0;
      }

      for (int k = 0; k < 4; k++) {
        trFgdPositionStart[i][j][k] = 0;
        trFgdPositionEnd[i][j][k]   = 0;
      }
    }


    for (UInt_t j = 0; j < NECALSUBDETS; j++) {
      trECALDetector[i][j]             = 0;
      trECALNHits[i][j]                = 0;
      trECALNNodes[i][j]               = 0;
      trECALLength[i][j]               = 0;
      trECALEMEnergy[i][j]             = 0;
      trECALEDeposit[i][j]             = 0;
      trECALIsShowerLike[i][j]         = 0;
      trECALAvgTime[i][j]              = 0;
      trECALPIDMipEm[i][j]             = 0;
      trECALPIDEmHip[i][j]             = 0;
      trECALMostUpStreamLayerHit[i][j] = 0;


      for (int k = 0; k < 3; k++) {
        trECALDirectionStart[i][j][k] = 0;
        trECALDirectionEnd[i][j][k]   = 0;
        trECALShowerPosition[i][j][k] = 0;
      }

      for (int k = 0; k < 4; k++) {
        trECALPositionStart[i][j][k] = 0;
        trECALPositionEnd[i][j][k]   = 0;
      }
    }

    for (UInt_t j = 0; j < NSMRDSUBDETS; j++) {
      trSMRDDetector[i][j] = 0;
      trSMRDNHits[i][j]    = 0;
      trSMRDNNodes[i][j]   = 0;
      trSMRDLength[i][j]   = 0;
      trSMRDAvgTime[i][j]  = 0;

      for (int k = 0; k < 3; k++) {
        trSMRDDirectionStart[i][j][k] = 0;
        trSMRDDirectionEnd[i][j][k]   = 0;
      }

      for (int k = 0; k < 4; k++) {
        trSMRDPositionStart[i][j][k] = 0;
        trSMRDPositionEnd[i][j][k]   = 0;
      }
    }

    for (int j = 0; j < 1; j++) {
      trP0DDetector[i][j] = 0;
      trP0DNHits[i][j]    = 0;
      trP0DNNodes[i][j]   = 0;
      trP0DLength[i][j]   = 0;
      trP0DELoss[i][j]   = 0;

      for (int k = 0; k < 3; k++) {
        trP0DDirectionStart[i][j][k] = 0;
        trP0DDirectionEnd[i][j][k]   = 0;
      }

      for (int k = 0; k < 4; k++) {
        trP0DPositionStart[i][j][k] = 0;
        trP0DPositionEnd[i][j][k]   = 0;
      }
    }
  }


  // Initialise all the branches. This is particularly important as some
  // branches many not exist in the flat tree.
  b_sPOTSincePreviousSpill = NULL;
  b_sRun                   = NULL;
  b_sSubrun                = NULL;
  b_sEvt                   = NULL;
  b_sEventTime             = NULL;
  b_sNTotalTrueParticles   = NULL;
  b_sNTotalTrueVertices    = NULL;
  b_sBeamGoodSpill         = NULL;
  b_sDQGoodDaq             = NULL;
 
  b_sNTrueVertices = NULL;

  b_sTrueVertexID               = NULL;
  b_sTrueVertexRooVtxIndex      = NULL;
  b_sTrueVertexRooVtxEntry      = NULL;
  b_sTrueVertexReacCode         = NULL;
  b_sTrueVertexNTrueParticles   = NULL;
  b_sTrueVertexNBaryons         = NULL;
  b_sTrueVertexBunch            = NULL;
  b_sTrueVertexDetector         = NULL;
  b_sTrueVertexNuPDG            = NULL;
  b_sTrueVertexTargetPDG        = NULL;
  b_sTrueVertexNuEnergy         = NULL;
  b_sTrueVertexQ2               = NULL;
  b_sTrueVertexPosition         = NULL;
  b_sTrueVertexNuParentPDG      = NULL;
  b_sTrueVertexNuParentDecPoint = NULL;
  b_sTrueVertexNuDir            = NULL;

  b_sNTrueParticles = NULL;

  b_sTrueParticleID                               = NULL;
  b_sTrueParticleBunch                            = NULL;
  b_sTrueParticlePDG                              = NULL;
  b_sTrueParticleParentPDG                        = NULL;
  b_sTrueParticleGParentPDG                       = NULL;
  b_sTrueParticlePrimaryID                        = NULL;
  b_sTrueParticleParentID                         = NULL;
  b_sTrueParticleGParentID                        = NULL;
  b_sTrueParticlePurity                           = NULL;
  b_sTrueParticleMomentum                         = NULL;
  b_sTrueParticleCharge                           = NULL;
  b_sTrueParticleVertexIndex                      = NULL;
  b_sTrueParticleTruthVertexID                    = NULL;
  b_sTrueParticleNReconTracks                     = NULL;
  b_sTrueParticlePosition                         = NULL;
  b_sTrueParticlePositionEnd                      = NULL;
  b_sTrueParticleDirection                        = NULL;
  b_sTrueParticleNDetCrossings                    = NULL;
  b_sTrueParticleEntrancePosition                 = NULL;
  b_sTrueParticleExitPosition                     = NULL;
  b_sTrueParticleEntranceMomentum                 = NULL;
  b_sTrueParticleExitMomentum                     = NULL;
  b_sTrueParticleDetector                         = NULL;
  b_sTrueParticleInActive                         = NULL;
  b_sTrueParticleIsTruePrimaryPi0DecayPhoton      = NULL;
  b_sTrueParticleIsTruePrimaryPi0DecayPhotonChild = NULL;

  b_sNFgdTimeBins = NULL;

  b_sFgdTimeBinMinTime       = NULL;
  b_sFgdTimeBinMaxTime       = NULL;
  b_sFgdTimeBinNHits1        = NULL;
  b_sFgdTimeBinNHits2        = NULL;
  b_sFgdTimeBinRawChargeSum1 = NULL;
  b_sFgdTimeBinRawChargeSum2 = NULL;
  b_sFgdTimeBinG4ID          = NULL;

  b_Bunch  = NULL;
  b_Weight = NULL;

  b_NVertices = NULL;

  b_vPrimaryIndex       = NULL;
  b_vBunch              = NULL;
  b_vPosition           = NULL;
  b_vVariance           = NULL;
  b_vChi2               = NULL;
  b_vNDOF               = NULL;
  b_vNReconParticles    = NULL;
  b_vNTrueVerticesMatch = NULL;
  b_vParticlesUniqueID  = NULL;
  b_vTrueVertexIndex    = NULL;
  b_vTrueVerticesClean  = NULL;
  b_vTrueVerticesCompl  = NULL;

  b_NParticles = NULL;

  b_trUniqueID            = NULL;
  b_trStatus              = NULL;
  b_trBunch               = NULL;
  b_trNReconVertices      = NULL;
  b_trDetectors           = NULL;
  b_trDetUsed             = NULL;
  b_trNTPCs               = NULL;
  b_trNFGDs               = NULL;
  b_trNECALs              = NULL;
  b_trNSMRDs              = NULL;
  b_trNP0Ds               = NULL;
  b_trNHits               = NULL;
  b_trNNodes              = NULL;
  b_trNDOF                = NULL;
  b_trChi2                = NULL;
  b_trCharge              = NULL;
  b_trMomentum            = NULL;
  b_trMomentumFlip        = NULL;
  b_trToFFGD1_FGD2        = NULL;
  b_trToFP0D_FGD1         = NULL;
  b_trToFDSECal_FGD1      = NULL;
  b_trToFECal_FGD1        = NULL;
  b_trToFDSECal_FGD2      = NULL;
  b_trToFECal_FGD2        = NULL;
  b_trToFFlag_FGD1_FGD2   = NULL;
  b_trToFFlag_P0D_FGD1    = NULL;
  b_trToFFlag_DSECal_FGD1 = NULL;
  b_trToFFlag_ECal_FGD1   = NULL;
  b_trToFFlag_DSECal_FGD2 = NULL;
  b_trToFFlag_ECal_FGD2   = NULL;
  b_trMomentumError       = NULL;
  b_trMomentumMuon        = NULL;
  b_trMomentumErrorMuon   = NULL;
  b_trMomentumProton      = NULL;
  b_trMomentumErrorProton = NULL;
  b_trMomentumEle         = NULL;
  b_trMomentumErrorEle    = NULL;
  b_trRangeMomentumEle    = NULL;
  b_trRangeMomentumMuon   = NULL;
  b_trRangeMomentumProton = NULL;
  b_trDirectionStart      = NULL;
  b_trDirectionEnd        = NULL;
  b_trPositionStart       = NULL;
  b_trPositionEnd         = NULL;

  b_trTpcDetector            = NULL;
  b_trTpcNHits               = NULL;
  b_trTpcNNodes              = NULL;
  b_trTpcLength              = NULL;
  b_trTpcDirectionStart      = NULL;
  b_trTpcDirectionEnd        = NULL;
  b_trTpcPositionStart       = NULL;
  b_trTpcPositionEnd         = NULL;
  b_trTpcCharge              = NULL;
  b_trTpcMomentum            = NULL;
  b_trTpcMomentumError       = NULL;
  b_trTpcBackMomentum        = NULL;
  b_trTpcRefitCharge         = NULL;
  b_trTpcRefitMomentum       = NULL;
  b_trTpcRefitPosition       = NULL;
  b_trTpcRefitDirection      = NULL;
  b_trTpcEFieldRefitMomentum = NULL;
  b_trTpcdEdxMeas            = NULL;
  b_trTpcdEdxExpMu           = NULL;
  b_trTpcdEdxExpEle          = NULL;
  b_trTpcdEdxExpP            = NULL;
  b_trTpcdEdxExpPi           = NULL;
  b_trTpcdEdxExpK            = NULL;
  b_trTpcdEdxSigmaMu         = NULL;
  b_trTpcdEdxSigmaEle        = NULL;
  b_trTpcdEdxSigmaP          = NULL;
  b_trTpcdEdxSigmaPi         = NULL;
  b_trTpcdEdxSigmaK          = NULL;
  b_trTpcPurity              = NULL;

  b_trFgdDetector       = NULL;
  b_trFgdNHits          = NULL;
  b_trFgdNNodes         = NULL;
  b_trFgdLength         = NULL;
  b_trFgdDirectionStart = NULL;
  b_trFgdDirectionEnd   = NULL;
  b_trFgdPositionStart  = NULL;
  b_trFgdPositionEnd    = NULL;
  b_trFgdX              = NULL;
  b_trFgdE              = NULL;
  b_trFgdPullmu         = NULL;
  b_trFgdPullp          = NULL;
  b_trFgdPullpi         = NULL;
  b_trFgdPullno         = NULL;
  b_trFgdContainment    = NULL;
  b_trFgdAvgTime        = NULL;
  b_trFgdVertex1x1      = NULL;
  b_trFgdVertex3x3      = NULL;
  b_trFgdVertex5x5      = NULL;
  b_trFgdVertex7x7      = NULL;
  b_trFgdVertexLayer    = NULL;

  b_trECALDetector             = NULL;
  b_trECALNHits                = NULL;
  b_trECALNNodes               = NULL;
  b_trECALLength               = NULL;
  b_trECALDirectionStart       = NULL;
  b_trECALDirectionEnd         = NULL;
  b_trECALPositionStart        = NULL;
  b_trECALPositionEnd          = NULL;
  b_trECALEMEnergy             = NULL;
  b_trECALEDeposit             = NULL;
  b_trECALIsShowerLike         = NULL;
  b_trECALAvgTime              = NULL;
  b_trECALMostUpStreamLayerHit = NULL;
  b_trECALPIDMipEm             = NULL;
  b_trECALPIDEmHip             = NULL;
  b_trECALShowerPosition       = NULL;

  
  b_trSMRDDetector       = NULL;
  b_trSMRDNHits          = NULL;
  b_trSMRDNNodes         = NULL;
  b_trSMRDLength         = NULL;
  b_trSMRDDirectionStart = NULL;
  b_trSMRDDirectionEnd   = NULL;
  b_trSMRDPositionStart  = NULL;
  b_trSMRDPositionEnd    = NULL;
  b_trSMRDAvgTime        = NULL;

  b_trP0DDetector       = NULL;
  b_trP0DNHits          = NULL;
  b_trP0DNNodes         = NULL;
  b_trP0DLength         = NULL;
  b_trP0DELoss         = NULL;
  b_trP0DDirectionStart = NULL;
  b_trP0DDirectionEnd   = NULL;
  b_trP0DPositionStart  = NULL;
  b_trP0DPositionEnd    = NULL;

  b_trTrueParticleID = NULL;
  b_trTpcTrueParticleID = NULL;
}

//********************************************************************
bool RedoTreeConverter::Initialize(){
  //********************************************************************

  // Set branch addresses and branch pointers
  if (!fChain) return false;

  // Check the existence of the _treeName tree
  if (!gDirectory->FindObjectAny(_treeName.c_str())) return false;

  fCurrent = -1;
  fChain->SetMakeClass(1);

  return true;
}

//********************************************************************
void RedoTreeConverter::DefineBranches(){
  //********************************************************************



  // The branches commented below are not read becouse they are not currently needed by any analysis. 
  // This is equivalent to setting their status to 0, as it was done in the previous version

  // Now set branch addresses for integer variables
  anaUtils::ConfigureTreeBranch(fChain, "sRun", &sRun, &b_sRun);
  
  anaUtils::ConfigureTreeBranch(fChain, "sSubRun", &sSubrun, &b_sSubrun);
  
  anaUtils::ConfigureTreeBranch(fChain, "sEvt", &sEvt, &b_sEvt);
  //    anaUtils::ConfigureTreeBranch(fChain, "sEventTime", &sEventTime, &b_sEventTime);
  //    anaUtils::ConfigureTreeBranch(fChain, "sNTotalTrueTracks", &sNTotalTrueParticles, &b_sNTotalTrueParticles);
  //    anaUtils::ConfigureTreeBranch(fChain, "sNTotalTrueVertices", &sNTotalTrueVertices, &b_sNTotalTrueVertices);
  anaUtils::ConfigureTreeBranch(fChain, "sBeamGoodSpill", &sBeamGoodSpill, &b_sBeamGoodSpill);
  anaUtils::ConfigureTreeBranch(fChain, "sDQGoodDaq", &sDQGoodDaq, &b_sDQGoodDaq);

  anaUtils::ConfigureTreeBranch(fChain, "sNTrueVertices", &sNTrueVertices, &b_sNTrueVertices);
  anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexID", sTrueVertexID, &b_sTrueVertexID);
  //    anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexRooVtxIndex", sTrueVertexRooVtxIndex, &b_sTrueVertexRooVtxIndex);
  //    anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexRooVtxEntry", sTrueVertexRooVtxEntry, &b_sTrueVertexRooVtxEntry);
  //    anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexReacCode", sTrueVertexReacCode, &b_sTrueVertexReacCode);
  //    anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexNTrueParticles", sTrueVertexNTrueParticles, &b_sTrueVertexNTrueParticles);
  //    anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexNBaryons", sTrueVertexNBaryons, &b_sTrueVertexNBaryons);
  anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexBunch", sTrueVertexBunch, &b_sTrueVertexBunch);
  anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexNuPDG", sTrueVertexNuPDG, &b_sTrueVertexNuPDG);
  //    anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexTargetPDG", sTrueVertexTargetPDG, &b_sTrueVertexTargetPDG);
  //    anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexNuParentPDG", sTrueVertexNuParentPDG, &b_sTrueVertexNuParentPDG);

  // True track info
  anaUtils::ConfigureTreeBranch(fChain, "sNTrueTracks",                               &sNTrueParticles,                              &b_sNTrueParticles);
  anaUtils::ConfigureTreeBranch(fChain, "sTrueTrackID",                               sTrueParticleID,                               &b_sTrueParticleID);
  anaUtils::ConfigureTreeBranch(fChain, "sTrueTrackPDG",                              sTrueParticlePDG,                              &b_sTrueParticlePDG);
  anaUtils::ConfigureTreeBranch(fChain, "sTrueTrackParentPDG",                        sTrueParticleParentPDG,                        &b_sTrueParticleParentPDG);
  anaUtils::ConfigureTreeBranch(fChain, "sTrueTrackGParentPDG",                       sTrueParticleGParentPDG,                       &b_sTrueParticleGParentPDG);
  anaUtils::ConfigureTreeBranch(fChain, "sTrueTrackPrimaryID",                        sTrueParticlePrimaryID,                        &b_sTrueParticlePrimaryID);
  anaUtils::ConfigureTreeBranch(fChain, "sTrueTrackParentID",                         sTrueParticleParentID,                         &b_sTrueParticleParentID);
  anaUtils::ConfigureTreeBranch(fChain, "sTrueTrackGParentID",                        sTrueParticleGParentID,                        &b_sTrueParticleGParentID);
  anaUtils::ConfigureTreeBranch(fChain, "sTrueTrackVertexIndex",                      sTrueParticleVertexIndex,                      &b_sTrueParticleVertexIndex);
  anaUtils::ConfigureTreeBranch(fChain, "sTrueTrackTruthVertexID",                    sTrueParticleTruthVertexID,                    &b_sTrueParticleTruthVertexID);
  anaUtils::ConfigureTreeBranch(fChain, "sTrueTrackMomentum",                         sTrueParticleMomentum,                         &b_sTrueParticleMomentum);
  anaUtils::ConfigureTreeBranch(fChain, "sTrueTrackCharge",                           sTrueParticleCharge,                           &b_sTrueParticleCharge);
  anaUtils::ConfigureTreeBranch(fChain, "sTrueTrackPosition",                         sTrueParticlePosition,                         &b_sTrueParticlePosition);
  anaUtils::ConfigureTreeBranch(fChain, "sTrueTrackPositionEnd",                      sTrueParticlePositionEnd,                      &b_sTrueParticlePositionEnd);
  anaUtils::ConfigureTreeBranch(fChain, "sTrueTrackDirection",                        sTrueParticleDirection,                        &b_sTrueParticleDirection);
  anaUtils::ConfigureTreeBranch(fChain, "sTrueTrackEntrancePosition",                 sTrueParticleEntrancePosition,                 &b_sTrueParticleEntrancePosition);
  anaUtils::ConfigureTreeBranch(fChain, "sTrueTrackExitPosition",                     sTrueParticleExitPosition,                     &b_sTrueParticleExitPosition);
  anaUtils::ConfigureTreeBranch(fChain, "sTrueTrackInActive",                         sTrueParticleInActive,                         &b_sTrueParticleInActive);
  anaUtils::ConfigureTreeBranch(fChain, "sTrueTrackDetector",                         sTrueParticleDetector,                         &b_sTrueParticleDetector);
  anaUtils::ConfigureTreeBranch(fChain, "sTrueTrackNDetCrossings",                    sTrueParticleNDetCrossings,                    &b_sTrueParticleNDetCrossings);
  anaUtils::ConfigureTreeBranch(fChain, "sTrueTrackIsTruePrimaryPi0DecayPhoton",      sTrueParticleIsTruePrimaryPi0DecayPhoton,      &b_sTrueParticleIsTruePrimaryPi0DecayPhoton);
  anaUtils::ConfigureTreeBranch(fChain, "sTrueTrackIsTruePrimaryPi0DecayPhotonChild", sTrueParticleIsTruePrimaryPi0DecayPhotonChild, &b_sTrueParticleIsTruePrimaryPi0DecayPhotonChild);
   
  
  
  anaUtils::ConfigureTreeBranch(fChain, "sNFgdTimeBins", &sNFgdTimeBins, &b_sNFgdTimeBins);
  anaUtils::ConfigureTreeBranch(fChain, "sFgdTimeBinNHits1", sFgdTimeBinNHits1, &b_sFgdTimeBinNHits1);
  anaUtils::ConfigureTreeBranch(fChain, "sFgdTimeBinNHits2", sFgdTimeBinNHits2, &b_sFgdTimeBinNHits2);
  //    anaUtils::ConfigureTreeBranch(fChain, "sFgdTimeBinG4ID", sFgdTimeBinG4ID, &b_sFgdTimeBinG4ID);

  anaUtils::ConfigureTreeBranch(fChain, "Bunch", &Bunch, &b_Bunch);
  anaUtils::ConfigureTreeBranch(fChain, "Weight", &Weight, &b_Weight);
  //    anaUtils::ConfigureTreeBranch(fChain, "NVertices", &NVertices, &b_NVertices);
  //    anaUtils::ConfigureTreeBranch(fChain, "vPrimaryIndex", &vPrimaryIndex, &b_vPrimaryIndex);
  //    anaUtils::ConfigureTreeBranch(fChain, "vBunch", &vBunch, &b_vBunch);
  //    anaUtils::ConfigureTreeBranch(fChain, "vNDOF", &vNDOF, &b_vNDOF);
  anaUtils::ConfigureTreeBranch(fChain, "vNReconParticles", &vNReconParticles, &b_vNReconParticles);
  anaUtils::ConfigureTreeBranch(fChain, "vNTrueVerticesMatch", &vNTrueVerticesMatch, &b_vNTrueVerticesMatch);
  anaUtils::ConfigureTreeBranch(fChain, "vParticlesUniqueID", &vParticlesUniqueID, &b_vParticlesUniqueID);
  anaUtils::ConfigureTreeBranch(fChain, "vTrueVertexIndex", &vTrueVertexIndex, &b_vTrueVertexIndex);

  anaUtils::ConfigureTreeBranch(fChain, "NTracks", &NParticles, &b_NParticles);
  anaUtils::ConfigureTreeBranch(fChain, "trUniqueID", trUniqueID, &b_trUniqueID);
  //    anaUtils::ConfigureTreeBranch(fChain, "trStatus", trStatus, &b_trStatus);
  //    anaUtils::ConfigureTreeBranch(fChain, "trBunch", trBunch, &b_trBunch);
  //    anaUtils::ConfigureTreeBranch(fChain, "trNReconVertices", &trNReconVertices, &b_trNReconVertices);
  //    anaUtils::ConfigureTreeBranch(fChain, "trDetectors", trDetectors, &b_trDetectors);
  anaUtils::ConfigureTreeBranch(fChain, "trDetUsed", trDetUsed, &b_trDetUsed);
  //    anaUtils::ConfigureTreeBranch(fChain, "trNTPCs", trNTPCs, &b_trNTPCs);
  //    anaUtils::ConfigureTreeBranch(fChain, "trNFGDs", trNFGDs, &b_trNFGDs);
  //    anaUtils::ConfigureTreeBranch(fChain, "trNECALs", trNECALs, &b_trNECALs);
  //    anaUtils::ConfigureTreeBranch(fChain, "trNSMRDs", trNSMRDs, &b_trNSMRDs);
  //    anaUtils::ConfigureTreeBranch(fChain, "trNP0Ds", trNP0Ds, &b_trNP0Ds);
  anaUtils::ConfigureTreeBranch(fChain, "trNHits", trNHits, &b_trNHits);
  //    anaUtils::ConfigureTreeBranch(fChain, "trNNodes", trNNodes, &b_trNNodes);
  //    anaUtils::ConfigureTreeBranch(fChain, "trNDOF", trNDOF, &b_trNDOF);
  anaUtils::ConfigureTreeBranch(fChain, "trTrueTrackID", trTrueParticleID, &b_trTrueParticleID);
  anaUtils::ConfigureTreeBranch(fChain, "trTpcDetector", trTpcDetector, &b_trTpcDetector);
  anaUtils::ConfigureTreeBranch(fChain, "trTpcNNodes", trTpcNNodes, &b_trTpcNNodes);
  anaUtils::ConfigureTreeBranch(fChain, "trTpcTrueTrackID", trTpcTrueParticleID, &b_trTpcTrueParticleID);
  anaUtils::ConfigureTreeBranch(fChain, "trFgdDetector", trFgdDetector, &b_trFgdDetector);
  anaUtils::ConfigureTreeBranch(fChain, "trFgdNNodes", trFgdNNodes, &b_trFgdNNodes);
  anaUtils::ConfigureTreeBranch(fChain, "trFgdContainment", trFgdContainment, &b_trFgdContainment);
 
  //    anaUtils::ConfigureTreeBranch(fChain, "trSMRDDetector", trSMRDDetector, &b_trSMRDDetector);
  //    anaUtils::ConfigureTreeBranch(fChain, "trSMRDNNodes", trSMRDNNodes, &b_trSMRDNNodes);
  //    anaUtils::ConfigureTreeBranch(fChain, "trP0DDetector", trP0DDetector, &b_trP0DDetector);
  //    anaUtils::ConfigureTreeBranch(fChain, "trP0DNNodes", trP0DNNodes, &b_trP0DNNodes);

  //        anaUtils::ConfigureTreeBranch(fChain, "sPOTSincePreviousSpill", &sPOTSincePreviousSpill, &b_sPOTSincePreviousSpill);
  anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexNuEnergy", sTrueVertexNuEnergy, &b_sTrueVertexNuEnergy);
  //        anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexQ2", sTrueVertexQ2, &b_sTrueVertexQ2);
  anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexPosition", sTrueVertexPosition, &b_sTrueVertexPosition);
  //        anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexNuParentDecPoint", sTrueVertexNuParentDecPoint, &b_sTrueVertexNuParentDecPoint);
  //        anaUtils::ConfigureTreeBranch(fChain, "sTrueVertexNuDir", sTrueVertexNuDir, &b_sTrueVertexNuDir);

  //        anaUtils::ConfigureTreeBranch(fChain, "sTrueTrackPurity", sTrueParticlePurity, &b_sTrueParticlePurity);
  anaUtils::ConfigureTreeBranch(fChain, "sFgdTimeBinMinTime",       sFgdTimeBinMinTime,       &b_sFgdTimeBinMinTime);
  anaUtils::ConfigureTreeBranch(fChain, "sFgdTimeBinMaxTime",       sFgdTimeBinMaxTime,       &b_sFgdTimeBinMaxTime);
  anaUtils::ConfigureTreeBranch(fChain, "sFgdTimeBinRawChargeSum1", sFgdTimeBinRawChargeSum1, &b_sFgdTimeBinRawChargeSum1);
  anaUtils::ConfigureTreeBranch(fChain, "sFgdTimeBinRawChargeSum2", sFgdTimeBinRawChargeSum2, &b_sFgdTimeBinRawChargeSum2);

  anaUtils::ConfigureTreeBranch(fChain, "vPosition", &vPosition, &b_vPosition);
  //        anaUtils::ConfigureTreeBranch(fChain, "vVariance", &vVariance, &b_vVariance);
  //        anaUtils::ConfigureTreeBranch(fChain, "vChi2", &vChi2, &b_vChi2);
  anaUtils::ConfigureTreeBranch(fChain, "vTrueVerticesClean", &vTrueVerticesClean, &b_vTrueVerticesClean);
  anaUtils::ConfigureTreeBranch(fChain, "vTrueVerticesCompl", &vTrueVerticesCompl, &b_vTrueVerticesCompl);

  //        anaUtils::ConfigureTreeBranch(fChain, "trChi2", trChi2, &b_trChi2);
  anaUtils::ConfigureTreeBranch(fChain, "trCharge",               trCharge,               &b_trCharge);
  anaUtils::ConfigureTreeBranch(fChain, "trToFFGD1_FGD2",         trToFFGD1_FGD2,         &b_trToFFGD1_FGD2);
  anaUtils::ConfigureTreeBranch(fChain, "trToFP0D_FGD1",          trToFP0D_FGD1,          &b_trToFP0D_FGD1);
  anaUtils::ConfigureTreeBranch(fChain, "trToFDSECal_FGD1",       trToFDSECal_FGD1,       &b_trToFDSECal_FGD1);
  anaUtils::ConfigureTreeBranch(fChain, "trToFECal_FGD1",         trToFECal_FGD1,         &b_trToFECal_FGD1);
  anaUtils::ConfigureTreeBranch(fChain, "trToFDSECal_FGD2",       trToFDSECal_FGD2,       &b_trToFDSECal_FGD2);
  anaUtils::ConfigureTreeBranch(fChain, "trToFECal_FGD2",         trToFECal_FGD2,         &b_trToFECal_FGD2);
  anaUtils::ConfigureTreeBranch(fChain, "trToFFlag_FGD1_FGD2",    trToFFlag_FGD1_FGD2,    &b_trToFFlag_FGD1_FGD2);
  anaUtils::ConfigureTreeBranch(fChain, "trToFFlag_P0D_FGD1",     trToFFlag_P0D_FGD1,     &b_trToFFlag_P0D_FGD1);
  anaUtils::ConfigureTreeBranch(fChain, "trToFFlag_DSECal_FGD1",  trToFFlag_DSECal_FGD1,  &b_trToFFlag_DSECal_FGD1);
  anaUtils::ConfigureTreeBranch(fChain, "trToFFlag_ECal_FGD1",    trToFFlag_ECal_FGD1,    &b_trToFFlag_ECal_FGD1);
  anaUtils::ConfigureTreeBranch(fChain, "trToFFlag_DSECal_FGD2",  trToFFlag_DSECal_FGD2,  &b_trToFFlag_DSECal_FGD2);
  anaUtils::ConfigureTreeBranch(fChain, "trToFFlag_ECal_FGD2",    trToFFlag_ECal_FGD2,    &b_trToFFlag_ECal_FGD2);
  
  anaUtils::ConfigureTreeBranch(fChain, "trMomentum",             trMomentum,             &b_trMomentum);
  anaUtils::ConfigureTreeBranch(fChain, "trMomentumFlip",         trMomentumFlip,         &b_trMomentumFlip);

  //        anaUtils::ConfigureTreeBranch(fChain, "trMomentumError", trMomentumError, &b_trMomentumError);
  //        anaUtils::ConfigureTreeBranch(fChain, "trMomentumMuon", trMomentumMuon, &b_trMomentumMuon);
  //        anaUtils::ConfigureTreeBranch(fChain, "trMomentumErrorMuon", trMomentumErrorMuon, &b_trMomentumErrorMuon);
  //        anaUtils::ConfigureTreeBranch(fChain, "trMomentumProton", trMomentumProton, &b_trMomentumProton);
  //        anaUtils::ConfigureTreeBranch(fChain, "trMomentumErrorProton", trMomentumErrorProton, &b_trMomentumErrorProton);
  //        anaUtils::ConfigureTreeBranch(fChain, "trMomentumEle", trMomentumEle, &b_trMomentumEle);
  //        anaUtils::ConfigureTreeBranch(fChain, "trMomentumErrorEle", trMomentumErrorEle, &b_trMomentumErrorEle);
  //        anaUtils::ConfigureTreeBranch(fChain, "trRangeMomentumEle", trRangeMomentumEle, &b_trRangeMomentumEle);
  anaUtils::ConfigureTreeBranch(fChain, "trRangeMomentumMuon", trRangeMomentumMuon, &b_trRangeMomentumMuon);
  //        anaUtils::ConfigureTreeBranch(fChain, "trRangeMomentumProton", trRangeMomentumProton, &b_trRangeMomentumProton);

  anaUtils::ConfigureTreeBranch(fChain, "trDirectionStart",                        trDirectionStart,         &b_trDirectionStart);
  anaUtils::ConfigureTreeBranch(fChain, "trDirectionEnd",                          trDirectionEnd,           &b_trDirectionEnd);
  anaUtils::ConfigureTreeBranch(fChain, "trPositionStart",                         trPositionStart,          &b_trPositionStart);
  anaUtils::ConfigureTreeBranch(fChain, "trPositionEnd",                           trPositionEnd,            &b_trPositionEnd);
  anaUtils::ConfigureTreeBranch(fChain, "trTpcDirectionStart",                     trTpcDirectionStart,      &b_trTpcDirectionStart);
  anaUtils::ConfigureTreeBranch(fChain, "trTpcPositionStart",                      trTpcPositionStart,       &b_trTpcPositionStart);
  anaUtils::ConfigureTreeBranch(fChain, "trTpcPositionEnd",                        trTpcPositionEnd,         &b_trTpcPositionEnd);
  anaUtils::ConfigureTreeBranch(fChain, "trTpcCharge",                             trTpcCharge,              &b_trTpcCharge);
  anaUtils::ConfigureTreeBranch(fChain, "trTpcMomentum",                           trTpcMomentum,            &b_trTpcMomentum);
  anaUtils::ConfigureTreeBranch(fChain, "trTpcMomentumError",                      trTpcMomentumError,       &b_trTpcMomentumError);
  anaUtils::ConfigureTreeBranch(fChain, "trTpcBackMomentum",                       trTpcBackMomentum,        &b_trTpcBackMomentum);
  anaUtils::ConfigureTreeBranch(fChain, "trTpcRefitMomentum",                      trTpcRefitMomentum,       &b_trTpcRefitMomentum);
  anaUtils::ConfigureTreeBranch(fChain, "trTpcRefitCharge",                        trTpcRefitCharge,         &b_trTpcRefitCharge);
  anaUtils::ConfigureTreeBranch(fChain, "trTpcRefitDirection",                     trTpcRefitDirection,      &b_trTpcRefitDirection);
  anaUtils::ConfigureTreeBranch(fChain, "trTpcRefitPosition",                      trTpcRefitPosition,       &b_trTpcRefitPosition);
  anaUtils::ConfigureTreeBranch(fChain, "trTpcEFieldRefitMomentum",                trTpcEFieldRefitMomentum, &b_trTpcEFieldRefitMomentum);
  anaUtils::ConfigureTreeBranch(fChain, "trTpcdEdxMeas",                           trTpcdEdxMeas,            &b_trTpcdEdxMeas);
  anaUtils::ConfigureTreeBranch(fChain, "trTpcdEdxExpMu",                          trTpcdEdxExpMu,           &b_trTpcdEdxExpMu);
  anaUtils::ConfigureTreeBranch(fChain, "trTpcdEdxExpEle",                         trTpcdEdxExpEle,          &b_trTpcdEdxExpEle);
  anaUtils::ConfigureTreeBranch(fChain, "trTpcdEdxExpP",                           trTpcdEdxExpP,            &b_trTpcdEdxExpP);
  anaUtils::ConfigureTreeBranch(fChain, "trTpcdEdxExpPi",                          trTpcdEdxExpPi,           &b_trTpcdEdxExpPi);
  //        anaUtils::ConfigureTreeBranch(fChain, "trTpcdEdxExpK", trTpcdEdxExpK, &b_trTpcdEdxExpK);
  anaUtils::ConfigureTreeBranch(fChain, "trTpcdEdxSigmaMu",                        trTpcdEdxSigmaMu,         &b_trTpcdEdxSigmaMu);
  anaUtils::ConfigureTreeBranch(fChain, "trTpcdEdxSigmaEle",                       trTpcdEdxSigmaEle,        &b_trTpcdEdxSigmaEle);
  anaUtils::ConfigureTreeBranch(fChain, "trTpcdEdxSigmaP",                         trTpcdEdxSigmaP,          &b_trTpcdEdxSigmaP);
  anaUtils::ConfigureTreeBranch(fChain, "trTpcdEdxSigmaPi",                        trTpcdEdxSigmaPi,         &b_trTpcdEdxSigmaPi);
  //        anaUtils::ConfigureTreeBranch(fChain, "trTpcdEdxSigmaK", trTpcdEdxSigmaK, &b_trTpcdEdxSigmaK);
  //        anaUtils::ConfigureTreeBranch(fChain, "trTpcPurity", trTpcPurity, &b_trTpcPurity);

  anaUtils::ConfigureTreeBranch(fChain, "trFgdDirectionStart", trFgdDirectionStart, &b_trFgdDirectionStart);
  anaUtils::ConfigureTreeBranch(fChain, "trFgdPositionStart",  trFgdPositionStart,  &b_trFgdPositionStart);
  anaUtils::ConfigureTreeBranch(fChain, "trFgdPositionEnd",    trFgdPositionEnd,    &b_trFgdPositionEnd);
  anaUtils::ConfigureTreeBranch(fChain, "trFgdX",              trFgdX,              &b_trFgdX);
  anaUtils::ConfigureTreeBranch(fChain, "trFgdE",              trFgdE,              &b_trFgdE);
  anaUtils::ConfigureTreeBranch(fChain, "trFgdPullmu",         trFgdPullmu,         &b_trFgdPullmu);
  anaUtils::ConfigureTreeBranch(fChain, "trFgdPullp",          trFgdPullp,          &b_trFgdPullp);
  anaUtils::ConfigureTreeBranch(fChain, "trFgdPullpi",         trFgdPullpi,         &b_trFgdPullpi);
  anaUtils::ConfigureTreeBranch(fChain, "trFgdPullno",         trFgdPullno,         &b_trFgdPullno);
  //        anaUtils::ConfigureTreeBranch(fChain, "trFgdAvgTime", trFgdAvgTime, &b_trFgdAvgTime);
  //        anaUtils::ConfigureTreeBranch(fChain, "trFgdVertex1x1", trFgdVertex1x1, &b_trFgdVertex1x1);
  //        anaUtils::ConfigureTreeBranch(fChain, "trFgdVertex3x3", trFgdVertex3x3, &b_trFgdVertex3x3);
  //        anaUtils::ConfigureTreeBranch(fChain, "trFgdVertex5x5", trFgdVertex5x5, &b_trFgdVertex5x5);
  //        anaUtils::ConfigureTreeBranch(fChain, "trFgdVertex7x7", trFgdVertex7x7, &b_trFgdVertex7x7);
  //        anaUtils::ConfigureTreeBranch(fChain, "trFgdVertexLayer", trFgdVertexLayer, &b_trFgdVertexLayer);

  
  // ECal variables
 // anaUtils::ConfigureTreeBranch(fChain, "trECALDetector",             trECALDetector,             &b_trECALDetector);
  anaUtils::ConfigureTreeBranch(fChain, "trECALNNodes",               trECALNNodes,               &b_trECALNNodes);
//  anaUtils::ConfigureTreeBranch(fChain, "trECALIsShowerLike",         trECALIsShowerLike,         &b_trECALIsShowerLike);
  anaUtils::ConfigureTreeBranch(fChain, "trECALDirectionStart",       trECALDirectionStart,       &b_trECALDirectionStart);
//  anaUtils::ConfigureTreeBranch(fChain, "trECALDirectionEnd",         trECALDirectionEnd,         &b_trECALDirectionEnd);
  anaUtils::ConfigureTreeBranch(fChain, "trECALPositionStart",        trECALPositionStart,        &b_trECALPositionStart);
//  anaUtils::ConfigureTreeBranch(fChain, "trECALPositionEnd",          trECALPositionEnd,          &b_trECALPositionEnd);
//  anaUtils::ConfigureTreeBranch(fChain, "trECALEDeposit",             trECALEDeposit,             &b_trECALEDeposit);
  anaUtils::ConfigureTreeBranch(fChain, "trECALAvgTime",              trECALAvgTime,              &b_trECALAvgTime);

  anaUtils::ConfigureTreeBranch(fChain, "trECALEMEnergy",             trECALEMEnergy,             &b_trECALEMEnergy);
  anaUtils::ConfigureTreeBranch(fChain, "trECALLength",               trECALLength,               &b_trECALLength);
  anaUtils::ConfigureTreeBranch(fChain, "trECALPIDMipEm",             trECALPIDMipEm,             &b_trECALPIDMipEm);
  anaUtils::ConfigureTreeBranch(fChain, "trECALPIDEmHip",             trECALPIDEmHip,             &b_trECALPIDEmHip);
  anaUtils::ConfigureTreeBranch(fChain, "trECALNNodes",               trECALNNodes,               &b_trECALNNodes);
  anaUtils::ConfigureTreeBranch(fChain, "trECALMostUpStreamLayerHit", trECALMostUpStreamLayerHit, &b_trECALMostUpStreamLayerHit);
  anaUtils::ConfigureTreeBranch(fChain, "trECALShowerPosition",       trECALShowerPosition,       &b_trECALShowerPosition);


  
  
  //        anaUtils::ConfigureTreeBranch(fChain, "trSMRDDirectionStart", trSMRDDirectionStart, &b_trSMRDDirectionStart);
  //        anaUtils::ConfigureTreeBranch(fChain, "trSMRDPositionStart", trSMRDPositionStart, &b_trSMRDPositionStart);
  //        anaUtils::ConfigureTreeBranch(fChain, "trSMRDPositionEnd", trSMRDPositionEnd, &b_trSMRDPositionEnd);
  //        anaUtils::ConfigureTreeBranch(fChain, "trSMRDAvgTime", trSMRDAvgTime, &b_trSMRDAvgTime);

  anaUtils::ConfigureTreeBranch(fChain, "trP0DLength", trP0DLength, &b_trP0DLength);
  anaUtils::ConfigureTreeBranch(fChain, "trP0DELoss", trP0DELoss, &b_trP0DELoss);
  //        anaUtils::ConfigureTreeBranch(fChain, "trP0DDirectionStart", trP0DDirectionStart, &b_trP0DDirectionStart);
  //        anaUtils::ConfigureTreeBranch(fChain, "trP0DPositionStart", trP0DPositionStart, &b_trP0DPositionStart);
  //        anaUtils::ConfigureTreeBranch(fChain, "trP0DPositionEnd", trP0DPositionEnd, &b_trP0DPositionEnd);

  fChain->SetBranchStatus("*",1);
  /*
     fChain->SetBranchStatus("*",0);

     fChain->SetBranchStatus("sPOTSincePreviousSpill",0);   //!
     fChain->SetBranchStatus("sRun",1);   //!
     fChain->SetBranchStatus("sEvt",1);   //!
     fChain->SetBranchStatus("sEventTime",0);   //!
     fChain->SetBranchStatus("sNTotalTrueParticles",0);   //!
     fChain->SetBranchStatus("sNTotalTrueVertices",0);   //!
     fChain->SetBranchStatus("sBeamGoodSpill",1);   //!
     fChain->SetBranchStatus("sDQGoodDaq",1);   //!
     fChain->SetBranchStatus("sNTrueVertices",1);   //!
     fChain->SetBranchStatus("sTrueVertexID",1);   //!
     fChain->SetBranchStatus("sTrueVertexRooVtxIndex",0);   //!
     fChain->SetBranchStatus("sTrueVertexRooVtxEntry",0);   //!
     fChain->SetBranchStatus("sTrueVertexReacCode",0);   //!
     fChain->SetBranchStatus("sTrueVertexNTrueParticles",0);   //!
     fChain->SetBranchStatus("sTrueVertexNReconTracks",0);   //!
     fChain->SetBranchStatus("sTrueVertexNReconVertices",0);   //!
     fChain->SetBranchStatus("sTrueVertexNBaryons",0);   //!
     fChain->SetBranchStatus("sTrueVertexBunch",1);   //!
     fChain->SetBranchStatus("sTrueVertexNuPDG",1);   //!
     fChain->SetBranchStatus("sTrueVertexTargetPDG",0);   //!
     fChain->SetBranchStatus("sTrueVertexNuEnergy",1);   //!
     fChain->SetBranchStatus("sTrueVertexQ2",0);   //!
     fChain->SetBranchStatus("sTrueVertexPosition",1);   //!
     fChain->SetBranchStatus("sTrueVertexNuParentPDG",0);   //!
     fChain->SetBranchStatus("sTrueVertexNuParentDecPoint",0);   //!
     fChain->SetBranchStatus("sTrueVertexNuDir",0);   //!
     fChain->SetBranchStatus("sNTrueParticles",1);   //!
     fChain->SetBranchStatus("sTrueTrackID",1);   //!
     fChain->SetBranchStatus("sTrueTrackBunch",0);   //!
     fChain->SetBranchStatus("sTrueTrackPDG",1);   //!
     fChain->SetBranchStatus("sTrueTrackParentPDG",1);   //!
     fChain->SetBranchStatus("sTrueTrackGParentPDG",1);   //!
     fChain->SetBranchStatus("sTrueTrackPrimaryID",1);   //!
     fChain->SetBranchStatus("sTrueTrackParentID",1);   //!
     fChain->SetBranchStatus("sTrueTrackPurity",0);   //!
     fChain->SetBranchStatus("sTrueTrackMomentum",1);   //!
     fChain->SetBranchStatus("sTrueTrackCharge",1);   //!
     fChain->SetBranchStatus("sTrueTrackVertexIndex",1);   //!
     fChain->SetBranchStatus("sTrueTrackTruthVertexID",1);   //!
     fChain->SetBranchStatus("sTrueTrackNReconTracks",0);   //!
     fChain->SetBranchStatus("sTrueTrackPosition",1);   //!
     fChain->SetBranchStatus("sTrueTrackPositionEnd",1);   //!
     fChain->SetBranchStatus("sTrueTrackDirection",1);   //!
     fChain->SetBranchStatus("sTrueTrackNDetCrossings",1);
     fChain->SetBranchStatus("sTrueTrackEntrancePosition",1);
     fChain->SetBranchStatus("sTrueTrackExitPosition",1);
     fChain->SetBranchStatus("sTrueTrackDetector",1);
     fChain->SetBranchStatus("sTrueTrackInActive",1);
     fChain->SetBranchStatus("sNFgdTimeBins",1);   //!
     fChain->SetBranchStatus("sFgdTimeBinMinTime",1);   //!
     fChain->SetBranchStatus("sFgdTimeBinMaxTime",1);   //!
     fChain->SetBranchStatus("sFgdTimeBinNHits1",1);   //!
     fChain->SetBranchStatus("sFgdTimeBinNHits2",1);   //!
     fChain->SetBranchStatus("sFgdTimeBinRawChargeSum1",1);   //!
     fChain->SetBranchStatus("sFgdTimeBinRawChargeSum2",1);   //!
     fChain->SetBranchStatus("sFgdTimeBinG4ID",0);   //!
     fChain->SetBranchStatus("Bunch",1);   //!
     fChain->SetBranchStatus("Weight",1);   //!
     fChain->SetBranchStatus("NVertices",1);   //!
     fChain->SetBranchStatus("vPrimaryIndex",1);   //!
     fChain->SetBranchStatus("vBunch",0);   //!
     fChain->SetBranchStatus("vPosition",1);   //!
     fChain->SetBranchStatus("vVariance",0);   //!
     fChain->SetBranchStatus("vChi2",0);   //!
     fChain->SetBranchStatus("vNDOF",0);   //!
     fChain->SetBranchStatus("vNReconParticles",1);   //!
  fChain->SetBranchStatus("vNTrueVerticesMatch",1);   //!
  fChain->SetBranchStatus("vParticlesUniqueID",1);   //!
  fChain->SetBranchStatus("vTrueVertexIndex",1);   //!
  fChain->SetBranchStatus("vTrueVerticesClean",1);   //!
  fChain->SetBranchStatus("vTrueVerticesCompl",1);   //!
  fChain->SetBranchStatus("NTracks",1);   //!
  fChain->SetBranchStatus("trUniqueID",1);   //!
  fChain->SetBranchStatus("trStatus",0);   //!
  fChain->SetBranchStatus("trBunch",0);   //!
  fChain->SetBranchStatus("trNReconVertices",0);   //!
  fChain->SetBranchStatus("trDetectors",0);   //!
  fChain->SetBranchStatus("trDetUsed",1);   //!
  fChain->SetBranchStatus("trNTPCs",0);   //!
  fChain->SetBranchStatus("trNFGDs",0);   //!
  fChain->SetBranchStatus("trNECALs",0);   //!
  fChain->SetBranchStatus("trNSMRDs",0);   //!
  fChain->SetBranchStatus("trNP0Ds",0);   //!
  fChain->SetBranchStatus("trNHits",1);   //!
  fChain->SetBranchStatus("trNNodes",0);   //!
  fChain->SetBranchStatus("trNDOF",0);   //!
  fChain->SetBranchStatus("trChi2",0);   //!
  fChain->SetBranchStatus("trCharge",1);   //!
  fChain->SetBranchStatus("trToFFGD1_FGD2",       1);   //!
  fChain->SetBranchStatus("trToFP0D_FGD1",       1);   //!
  fChain->SetBranchStatus("trToFECal_FGD1",       1);   //!
  fChain->SetBranchStatus("trToFECal_FGD2",       1);   //!
  fChain->SetBranchStatus("trToFFlag_FGD1_FGD2",       1);   //!
  fChain->SetBranchStatus("trToFFlag_P0D_FGD1",       1);   //!
  fChain->SetBranchStatus("trToFFlag_ECal_FGD1",       1);   //!
  fChain->SetBranchStatus("trToFFlag_ECal_FGD2",       1);   //!
  fChain->SetBranchStatus("trMomentum",1);   //!
  fChain->SetBranchStatus("trMomentumError",0);   //!
  fChain->SetBranchStatus("trMomentumMuon",0);   //!
  fChain->SetBranchStatus("trMomentumErrorMuon",0);   //!
  fChain->SetBranchStatus("trMomentumProton",0);   //!
  fChain->SetBranchStatus("trMomentumErrorProton",0);   //!
  fChain->SetBranchStatus("trMomentumEle",0);   //!
  fChain->SetBranchStatus("trMomentumErrorEle",0);   //!
  fChain->SetBranchStatus("trRangeMomentumEle",0);   //!
  fChain->SetBranchStatus("trRangeMomentumMuon",0);   //!
  fChain->SetBranchStatus("trRangeMomentumProton",0);   //!
  fChain->SetBranchStatus("trDirectionStart",1);   //!
  fChain->SetBranchStatus("trDirectionEnd",0);   //!
  fChain->SetBranchStatus("trPositionStart",1);   //!
  fChain->SetBranchStatus("trPositionEnd",1);   //!
  fChain->SetBranchStatus("trTpcDetector",1);   //!
  fChain->SetBranchStatus("trTpcNNodes",1);   //!
  fChain->SetBranchStatus("trTpcDirectionStart",1);   //!
  fChain->SetBranchStatus("trTpcPositionStart",1);   //!
  fChain->SetBranchStatus("trTpcPositionEnd",1);   //!
  fChain->SetBranchStatus("trTpcCharge",1);   //!
  fChain->SetBranchStatus("trTpcMomentum",1);   //!
  fChain->SetBranchStatus("trTpcMomentumError",1);   //!
  fChain->SetBranchStatus("trTpcRefitMomentum",1);
  fChain->SetBranchStatus("trTpcBackMomentum",1);   //!
  fChain->SetBranchStatus("trTpcdEdxMeas",1);   //!
  fChain->SetBranchStatus("trTpcdEdxExpMu",1);   //!
  fChain->SetBranchStatus("trTpcdEdxExpEle",1);   //!
  fChain->SetBranchStatus("trTpcdEdxExpP",1);   //!
  fChain->SetBranchStatus("trTpcdEdxExpPi",1);   //!
  fChain->SetBranchStatus("trTpcdEdxExpK",0);   //!
  fChain->SetBranchStatus("trTpcdEdxSigmaMu",1);   //!
  fChain->SetBranchStatus("trTpcdEdxSigmaEle",1);   //!
  fChain->SetBranchStatus("trTpcdEdxSigmaP",1);   //!
  fChain->SetBranchStatus("trTpcdEdxSigmaPi",1);   //!
  fChain->SetBranchStatus("trTpcdEdxSigmaK",0);   //!
  fChain->SetBranchStatus("trTpcPurity",0);   //!
  fChain->SetBranchStatus("trFgdDetector",1);   //!
  fChain->SetBranchStatus("trFgdNNodes",1);   //!
  fChain->SetBranchStatus("trFgdDirectionStart",1);   //!
  fChain->SetBranchStatus("trFgdPositionStart",1);   //!
  fChain->SetBranchStatus("trFgdPositionEnd",1);   //!
  fChain->SetBranchStatus("trFgdX",1);   //!
  fChain->SetBranchStatus("trFgdE",1);   //!
  fChain->SetBranchStatus("trFgdPullmu",1);   //!
  fChain->SetBranchStatus("trFgdPullp",1);   //!
  fChain->SetBranchStatus("trFgdPullpi",1);   //!
  fChain->SetBranchStatus("trFgdPullno",1);   //!
  fChain->SetBranchStatus("trFgdContainment",1);   //!
  fChain->SetBranchStatus("trFgdAvgTime",0);   //!
  fChain->SetBranchStatus("trFgdVertex1x1",0);   //!
  fChain->SetBranchStatus("trFgdVertex3x3",0);   //!
  fChain->SetBranchStatus("trFgdVertex5x5",0);   //!
  fChain->SetBranchStatus("trFgdVertex7x7",0);   //!
  fChain->SetBranchStatus("trFgdVertexLayer",0);   //!
  fChain->SetBranchStatus("trECALDetector",0);   //!
  fChain->SetBranchStatus("trECALNNodes",0);   //!
  fChain->SetBranchStatus("trECALDirectionStart",0);   //!
  fChain->SetBranchStatus("trECALPositionStart",0);   //!
  fChain->SetBranchStatus("trECALPositionEnd",0);   //!
  fChain->SetBranchStatus("trECALEMEnergy",0);   //!
  fChain->SetBranchStatus("trECALEDeposit",0);   //!
  fChain->SetBranchStatus("trECALIsShowerLike",0);   //!
  fChain->SetBranchStatus("trECALAvgTime",0);   //!
  fChain->SetBranchStatus("trSMRDDetector",0);   //!
  fChain->SetBranchStatus("trSMRDNNodes",0);   //!
  fChain->SetBranchStatus("trSMRDDirectionStart",0);   //!
  fChain->SetBranchStatus("trSMRDPositionStart",0);   //!
  fChain->SetBranchStatus("trSMRDPositionEnd",0);   //!
  fChain->SetBranchStatus("trSMRDAvgTime",0);   //!
  fChain->SetBranchStatus("trP0DDetector",0);   //!
  fChain->SetBranchStatus("trP0DNNodes",0);   //!
  fChain->SetBranchStatus("trP0DDirectionStart",0);   //!
  fChain->SetBranchStatus("trP0DPositionStart",0);   //!
  fChain->SetBranchStatus("trP0DPositionEnd",0);   //!
  fChain->SetBranchStatus("trTrueParticleID",1); //!
  fChain->SetBranchStatus("trTpcTrueParticleID",1); //!
  */
}

//********************************************************************
RedoTreeConverter::~RedoTreeConverter(){
  //********************************************************************

  if (!fChain) return;

  if (flattree)       delete   flattree->GetCurrentFile();
  if (GRooTrackerVTX) delete   GRooTrackerVTX->GetCurrentFile();
  if (NRooTrackerVTX) delete   NRooTrackerVTX->GetCurrentFile();
  if (flattree)       delete   flattree;
  if (GRooTrackerVTX) delete   GRooTrackerVTX;
  if (NRooTrackerVTX) delete   NRooTrackerVTX;
}


//****************************************************************************
bool RedoTreeConverter::AddFileToTChain(const std::string& inputString){
  //****************************************************************************

  std::cout << "RedoTreeConverter::AddFileToTChain(). Adding file: " << inputString << std::endl;

  // ------------- Check that the file has some entries. Otherwise ignore it -----------------
  TChain dummy(_treeName.c_str());
  dummy.AddFile(inputString.c_str());
  if (dummy.GetEntries("sEvt>=0") == 0){
    std::cout << "      ----> This file does not contain any entries. IGNORED !!!!" << std::endl;
    return true;
  }


  // Open the file to do few checks
  TFile *f = TFile::Open(inputString.c_str()); 
  f->cd(); 

  // ------------- Check that the header tree exists (needed for POT counting). If it doesn't ignore the file -----------------

  if (!gDirectory->FindObjectAny("header")){
    std::cout << "      ----> This file does not contain a header tree. IGNORED !!!!" << std::endl;
    return true;
  }


  // ----------  Deal with RooTrackerVtx trees. Only for the first file

  if( _firstFile ) {
    fGenie=fNeut=false;

    if( gDirectory->FindObjectAny("NRooTrackerVtx")) {
      fNeut = true;
      AddChain("NRooTrackerVtx");
      NRooTrackerVTX = GetChain("NRooTrackerVtx");
      /*
         NRooTrackerVTX->SetBranchAddress("RunID",    &RunID);
         NRooTrackerVTX->SetBranchAddress("SubrunID", &SubrunID);
         NRooTrackerVTX->SetBranchAddress("EventID",  &EventID);
         NVtx = new TClonesArray("ND::NRooTrackerVtx",100);
         NRooTrackerVTX->SetBranchAddress("NVtx",&NNVtx);
         NRooTrackerVTX->SetBranchAddress("Vtx",&NVtx);
         */
      std::cout << "RedoTreeConverter::AddFileToTChain(). NEUT RooTrackerVtx tree found !!" << std::endl; 
    }
    else if(gDirectory->FindObjectAny("GRooTrackerVtx")) {
      fGenie = true;
      AddChain("GRooTrackerVtx");
      GRooTrackerVTX = GetChain("GRooTrackerVtx");
      /*
         GRooTrackerVTX->SetBranchAddress("RunID",    &RunID);
         GRooTrackerVTX->SetBranchAddress("SubrunID", &SubrunID);
         GRooTrackerVTX->SetBranchAddress("EventID",  &EventID);
         GVtx = new TClonesArray("ND::GRooTrackerVtx",100);
         GRooTrackerVTX->SetBranchAddress("NVtx",&NGVtx);
         GRooTrackerVTX->SetBranchAddress("Vtx",&GVtx);
         */
      std::cout << "redoTreeConverter::AddFileToTChain(). GENIE RooTrackerVtx tree found !!" << std::endl; 
    }
  }

  f->Close(); 

  // ------------- Add the file to the RooTrackerVtx chain

  if( fGenie && GRooTrackerVTX) {
    GRooTrackerVTX->AddFile(inputString.c_str());
  }
  else if( fNeut  && NRooTrackerVTX) {
    NRooTrackerVTX->AddFile(inputString.c_str());
  }

  // ------------- Add the file to the FlatTree chain
  flattree->AddFile(inputString.c_str());  


  // Define the branches to be read from the input tree only once after reading the first file
  // Need to re-initialize now we know which type of highland tree we are dealing with
  if( _firstFile ) {
    _firstFile = false;
    DefineBranches();
  }
  /*
  // Define Data/MC status: use the first valid entry in the flat tree
  Long64_t nentries = GetEntries();  
  for (Long64_t i = 0; i<nentries; i++){
  Int_t entry_temp = flattree->GetEntry(i);
  if (entry_temp<=0) continue;    
// Set the data/MC mode and return false when mixing data and MC files
if (!header().SetIsMC(sIsMC)) return false;
break;
}
*/
// Read the header tree for POT counting (when running over a file list POT is incremented), IsMC and SoftwareVersion
return header().AddHeader(inputString);  
}


//*****************************************************************************
Int_t RedoTreeConverter::GetSpill(Long64_t& entry, AnaSpillC*& spill){
  //*****************************************************************************

  // Read contents of entry.
  if (!fChain) return 0;

  // Create an instance of AnaSpill
  spill = MakeSpill();

  Int_t entry_temp;

  // Get a new entry from the flat tree
  entry_temp = flattree->GetEntry(entry);
 

  if (entry_temp>0 && sRun > 1000000) {
    if (fGenie && GRooTrackerVTX) GRooTrackerVTX->GetEntry(entry);
    if (fNeut  && NRooTrackerVTX){
      NRooTrackerVTX->GetEntry(_entry_roo);
      _entry_roo++;
    }
  }

  // Print the current file
  TString filename(flattree->GetFile()->GetName());   
  if( filename != _currentfilename ) {
    std::cout << " Running on file : " << filename << std::endl; 
    _currentfilename = filename;
  }

  // Fill the general spill info
  _spill = static_cast<AnaSpillB*>(spill);
  FillInfo(_spill);

  // Always make the out of bunch
  _spill->OutOfBunch = MakeBunch();

  int evt_prev=sEvt;

  while (sEvt==evt_prev && entry < _nentries){

    // Create and fill a new bunch
    if (Bunch==-1) {
      FillBunchInfo(_spill->TrueParticles, _spill->OutOfBunch);
    } else {
      AnaBunchB* bunch = MakeBunch();
      _spill->Bunches.push_back(bunch);
      FillBunchInfo(_spill->TrueParticles, bunch);
    }

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
Int_t RedoTreeConverter::GetEvent(Long64_t& entry, AnaEventC*& event){
  //*****************************************************************************

  // Read contents of entry.
  if (!fChain) return 0;
  Int_t entry_temp;

  // get a new entry from the input tree
  entry_temp = flattree->GetEntry(entry);

  // Reed the RooTrackerVtx when exists
  if (entry_temp>0 && sRun > 1000000) {
    if (fGenie && GRooTrackerVTX) GRooTrackerVTX->GetEntry(entry);
    if (fNeut  && NRooTrackerVTX){
      NRooTrackerVTX->GetEntry(_entry_roo);
      _entry_roo++;
    }
  }

  // Print the current file
  TString filename(flattree->GetFile()->GetName());   
  if( filename != _currentfilename ) {
    std::cout << " Running on file : " << filename << std::endl; 
    _currentfilename = filename;
  }

  // Create a new event
  event = new AnaEventB();

  // Fill the event info;
  FillEventInfo(static_cast<AnaEventB*>(event));

  // Increment entry number
  entry++;

  _prevEvent=static_cast<AnaEventB*>(event);
  return entry_temp;
}

//********************************************************************
void RedoTreeConverter::IncrementPOTBySpill() {
  //********************************************************************

  //  ND::header().IncrementPOTBySpill(*_spill);
}

//*****************************************************************************
Long64_t RedoTreeConverter::GetEntries(){
  //*****************************************************************************

  // if this is the first time this function is called get the number of entries
  // Comment out for now, since Asher needs to call GetEntries for every tree when not using preloading
  //if (_nentries==0)
  _nentries = GetChain()->GetEntries("sEvt>=0");

  // just returns the number of entries otherwise
  return _nentries;
}

//*****************************************************************************
void RedoTreeConverter::FillEventInfo(AnaEventB* event){
  //*****************************************************************************

  // When the previous event has the same event number it means that we are in the same Spill.
  // In this case we can copy all common spill info from the previous event
  bool filled = false;
  if (_prevEvent){
    if (sEvt == _prevEvent->EventInfo.Event){
      // Don't copy bunch info but clone truth
      event->Copy(*_prevEvent, false);
      filled = true;
    }
  }
  // otherwise this is a new spill and everything should be filled
  if (!filled)
    FillEventSpillInfo(event);

  // Now we fill the Bunch info
  event->Bunch = Bunch;
  event->Weight = Weight;


  event->nParticles = 0;
  event->nVertices = 0;
  anaUtils::CreateArray(event->Particles, NParticles);
  anaUtils::CreateArray(event->Vertices, NVertices);
  for (int i=0;i<NParticles;i++){
    AnaTrackB* track = MakeTrack();
    FillTrackInfo(event, i, track);
    event->Particles[event->nParticles] = track;
    event->nParticles++;
  }
  for (int i=0;i<NVertices;i++){
    AnaVertexB* vertex = MakeVertex();
    FillVertexInfo(i, vertex, event);
    event->Vertices[event->nVertices] = vertex;
    event->nVertices++;
  }

}

//*****************************************************************************
void RedoTreeConverter::FillEventSpillInfo(AnaEventB* event){
  //*****************************************************************************

  if (sRun==0 || sRun>1000000)
    event->EventInfo.IsMC = true;
  else
    event->EventInfo.IsMC = false;

  event->EventInfo.Run    = sRun;
  event->EventInfo.Event  = sEvt;
  event->EventInfo.SetIsSandMC();
  // Fill the true vertices vector. This should be called before filling the true particles vector
  event->nTrueVertices = 0;
  anaUtils::CreateArray(event->TrueVertices, sNTrueVertices);
  for (int i=0;i<sNTrueVertices;i++){
    AnaTrueVertexB* vertex = MakeTrueVertex();
    FillTrueVertexInfo(i, vertex);
    event->TrueVertices[event->nTrueVertices] = vertex;
    event->nTrueVertices++;
  }

  // Fill the true particles vector
  event->nTrueParticles = 0;
  anaUtils::CreateArray(event->TrueParticles, sNTrueParticles);
  for (int i=0;i<sNTrueParticles;i++){
    AnaTrueParticleB* particle = MakeTrueParticle();
    FillTrueParticleInfo(event, i, particle);
    // if(!particle->TrueVertex){
    //   delete particle;
    //   continue;
    // }
    event->TrueParticles[event->nTrueParticles] = particle;
    event->nTrueParticles++;
  }

  // Give the proper size to the TrueParticles vector for each TrueVertex. 
  // This can be done only after filling the general True Particle vector (above) since is in this process 
  // when the TrueVertex <---> TrueParticle association is done
  for (int i=0;i<event->nTrueVertices;i++){
    anaUtils::ResizeArray(event->TrueVertices[i]->TrueParticles, event->TrueVertices[i]->nTrueParticles, sNTrueParticles);
  }

  event->DataQuality = MakeDataQuality();
  event->Beam = MakeBeam();
  FillDQInfo(event->DataQuality);
  FillBeamInfo(event->Beam);

  // FGD time bin info
  FillFgdTimeBinInfo(event);    
}

//*****************************************************************************
void RedoTreeConverter::FillInfo(AnaSpillB* spill){
  //*****************************************************************************
  spill->EventInfo = MakeEventInfo();

  if (sRun==0 || sRun>1000000)
    spill->EventInfo->IsMC = true;
  else
    spill->EventInfo->IsMC = false;

  spill->EventInfo->Run    = sRun;
  spill->EventInfo->Event  = sEvt;
  spill->EventInfo->SetIsSandMC();

  spill->NTotalTrueParticles   = sNTotalTrueParticles;
  spill->NTotalTrueVertices = sNTotalTrueVertices;

  // Fill the true vertices vector. This should be called before filling the true particles vector
  spill->TrueVertices.clear();
  int nVertices = std::min((int)NMAXTRUEVERTICES, sNTrueVertices);
  for (int i=0;i<nVertices;i++){
    AnaTrueVertexB* vertex = MakeTrueVertex();
    FillTrueVertexInfo(i, vertex);
    FillTrueVertexRooInfo(vertex);
    spill->TrueVertices.push_back(vertex);
  }

  // Fill the true particles vector
  spill->TrueParticles.clear();
  int nParts = std::min((int)NMAXTRUEPARTICLES, sNTrueParticles);
  for (int i=0;i<nParts;i++){
    AnaTrueParticleB* particle = MakeTrueParticle();
    // Create an array to have the same input as for event 
    AnaTrueVertexB** trueVertices = NULL;
    
    anaUtils::CreateArray(trueVertices, spill->TrueVertices.size());
    for (size_t i = 0; i < spill->TrueVertices.size(); i++){
      trueVertices[i] = spill->TrueVertices[i];
    }
     
    FillTrueParticleInfo(spill->TrueVertices.size(), trueVertices, i, particle);
    delete [] trueVertices; 
    spill->TrueParticles.push_back(particle);
  }

  spill->DataQuality = MakeDataQuality();
  spill->Beam = MakeBeam();
  FillDQInfo(spill->DataQuality);
  FillBeamInfo(spill->Beam);
  //  FillTriggerInfo(&spill->Trigger);

  // FGD time bin info
  FillFgdTimeBinInfo(spill->FgdTimeBins);
}

//*****************************************************************************
void RedoTreeConverter::FillBunchInfo(std::vector<AnaTrueParticleB*>& trueParticles, AnaBunchB* bunch){
  //*****************************************************************************

  bunch->Bunch  = Bunch;
  bunch->Weight = Weight;
  bunch->Particles.clear();
  bunch->Vertices.clear();

  for (int i=0;i<NParticles;i++){
    AnaTrackB* track = MakeTrack();
    FillTrackInfo(trueParticles, i, track);
    bunch->Particles.push_back(track);
  }

  for (int i=0;i<NVertices;i++){
    AnaVertexB* vertex = MakeVertex();
    FillVertexInfo(i, vertex, bunch);
    bunch->Vertices.push_back(vertex);
  }
}

//*****************************************************************************
void RedoTreeConverter::FillFgdTimeBinInfo(std::vector<AnaFgdTimeBinB*>& FgdTimeBins){
  //*****************************************************************************

  //loop over fgd time bins
  for(int ibin=0; ibin<sNFgdTimeBins; ibin++) {      

    AnaFgdTimeBinB* abin = MakeFgdTimeBin();
    FgdTimeBins.push_back(abin);

    abin->MinTime = sFgdTimeBinMinTime[ibin];
    abin->NHits[0]        = sFgdTimeBinNHits1[ibin];
    abin->NHits[1]        = sFgdTimeBinNHits2[ibin];
    abin->RawChargeSum[0] = sFgdTimeBinRawChargeSum1[ibin];
    abin->RawChargeSum[1] = sFgdTimeBinRawChargeSum2[ibin];    
    //    abin->G4ID = sFgdTimeBinG4ID[ibin];
  }
}  

//*****************************************************************************
void RedoTreeConverter::FillFgdTimeBinInfo(AnaEventB* event){
  //*****************************************************************************

  //loop over fgd time bins
  event->nFgdTimeBins = 0;
  anaUtils::CreateArray(event->FgdTimeBins, sNFgdTimeBins);
  for(int ibin=0; ibin<sNFgdTimeBins; ibin++) {      

    AnaFgdTimeBinB* abin = MakeFgdTimeBin();
    event->FgdTimeBins[event->nFgdTimeBins] = abin;
    event->nFgdTimeBins++;

    abin->MinTime = sFgdTimeBinMinTime[ibin];
    abin->NHits[0]        = sFgdTimeBinNHits1[ibin];
    abin->NHits[1]        = sFgdTimeBinNHits2[ibin];
    abin->RawChargeSum[0] = sFgdTimeBinRawChargeSum1[ibin];
    abin->RawChargeSum[1] = sFgdTimeBinRawChargeSum2[ibin];    
    //    abin->G4ID = sFgdTimeBinG4ID[ibin];
  }
}  

//*****************************************************************************
void RedoTreeConverter::FillDQInfo(AnaDataQualityB* dq){
  //*****************************************************************************

  dq->GoodDaq   = sDQGoodDaq;
}

//*****************************************************************************
void RedoTreeConverter::FillBeamInfo(AnaBeamB* beam){
  //*****************************************************************************

  beam->GoodSpill = sBeamGoodSpill;
}


//*****************************************************************************
void RedoTreeConverter::FillTrackInfo(AnaEventB* event, int itrk, AnaTrackB* track){
  //*****************************************************************************
  track->Index                = itrk;
  track->UniqueID             = (trUniqueID)[itrk];
  //  track->Status           = (trStatus)[itrk];
  //  track->Detectors        = (trDetectors)[itrk];
  track->Charge               = (trCharge)[itrk];
  track->NHits                = (trNHits)[itrk];
  track->NNodes               = (trNNodes)[itrk];
  track->Momentum             = (trMomentum)[itrk];
  track->MomentumFlip         = (trMomentumFlip)[itrk];
  track->ToF.FGD1_FGD2        = (trToFFGD1_FGD2)[itrk];
  track->ToF.P0D_FGD1         = (trToFP0D_FGD1)[itrk];
  track->ToF.DSECal_FGD1      = (trToFDSECal_FGD1)[itrk];
  track->ToF.ECal_FGD1        = (trToFECal_FGD1)[itrk]; 
  track->ToF.DSECal_FGD2      = (trToFDSECal_FGD2)[itrk];
  track->ToF.ECal_FGD2        = (trToFECal_FGD2)[itrk];
  track->ToF.Flag_FGD1_FGD2   = (trToFFlag_FGD1_FGD2)[itrk];
  track->ToF.Flag_P0D_FGD1    = (trToFFlag_P0D_FGD1)[itrk];
  track->ToF.Flag_DSECal_FGD1 = (trToFFlag_DSECal_FGD1)[itrk];
  track->ToF.Flag_ECal_FGD1   = (trToFFlag_ECal_FGD1)[itrk];
  track->ToF.Flag_DSECal_FGD2 = (trToFFlag_DSECal_FGD2)[itrk];
  track->ToF.Flag_ECal_FGD2   = (trToFFlag_ECal_FGD2)[itrk];
  //  track->MomentumError = (trMomentumError)[itrk];

  track->RangeMomentumMuon  = (trRangeMomentumMuon)[itrk];


  anaUtils::CopyArray(trPositionStart[itrk],  track->PositionStart,  4);
  anaUtils::CopyArray(trPositionEnd[itrk],    track->PositionEnd,    4);
  anaUtils::CopyArray(trDirectionStart[itrk], track->DirectionStart, 3);
  anaUtils::CopyArray(trDirectionEnd[itrk],   track->DirectionEnd,   3);

  convUtils::ConvertTrackDetEnumToBitField(track->Detector, (trDetUsed)[itrk]);

  FillSubdetectorInfo(event, itrk, track);

  // Find the true particle associated with this recon track
  FindTrueParticle(event, trTrueParticleID[itrk], track->TrueObject);

  // Save all recon tracks associated to this true particle
  /*
     if (track->TrueObject){
     AnaTrueObjectC* thistrueobj = track->TrueObject;

  // Vectors are already filled with null pointers --> fill, remove last one (that should be null) and sort
  thistrueobj->ReconParticles.insert(thistrueobj->ReconParticles.begin(),track);
  if (thistrueobj->ReconParticles.back())
  if ( ! suppress_err_msg) std::cout << "minor error 73731" << std::endl;
  if ( ! thistrueobj->ReconParticles.back()) thistrueobj->ReconParticles.pop_back();
  // sort here to keep handle associations from different bunches
  std::sort(thistrueobj->ReconParticles.begin(), thistrueobj->ReconParticles.end(), AnaTrackB::CompareMomentum);

  }
  */
}

//*****************************************************************************
void RedoTreeConverter::FillTrackInfo(std::vector<AnaTrueParticleB*>& trueParticles, int itrk, AnaTrackB* track){
  //*****************************************************************************

  track->Index              = itrk;
  track->UniqueID           = (trUniqueID)[itrk];
  //  track->Status        = (trStatus)[itrk];
  //  track->Detectors     = (trDetectors)[itrk];
  track->Charge             = (trCharge)[itrk];
  track->NHits              = (trNHits)[itrk];
  track->NNodes             = (trNNodes)[itrk];
  track->Momentum           = (trMomentum)[itrk];
  track->MomentumFlip       = (trMomentumFlip)[itrk];
 
  track->ToF.FGD1_FGD2        = (trToFFGD1_FGD2)[itrk];
  track->ToF.P0D_FGD1         = (trToFP0D_FGD1)[itrk];
  track->ToF.DSECal_FGD1      = (trToFDSECal_FGD1)[itrk];
  track->ToF.ECal_FGD1        = (trToFECal_FGD1)[itrk]; 
  track->ToF.DSECal_FGD2      = (trToFDSECal_FGD2)[itrk];
  track->ToF.ECal_FGD2        = (trToFECal_FGD2)[itrk];
  track->ToF.Flag_FGD1_FGD2   = (trToFFlag_FGD1_FGD2)[itrk];
  track->ToF.Flag_P0D_FGD1    = (trToFFlag_P0D_FGD1)[itrk];
  track->ToF.Flag_DSECal_FGD1 = (trToFFlag_DSECal_FGD1)[itrk];
  track->ToF.Flag_ECal_FGD1   = (trToFFlag_ECal_FGD1)[itrk];
  track->ToF.Flag_DSECal_FGD2 = (trToFFlag_DSECal_FGD2)[itrk];
  track->ToF.Flag_ECal_FGD2   = (trToFFlag_ECal_FGD2)[itrk];
  
  //  track->MomentumError = (trMomentumError)[itrk];

  track->RangeMomentumMuon  = (trRangeMomentumMuon)[itrk];


  anaUtils::CopyArray(trPositionStart[itrk],  track->PositionStart,  4);
  anaUtils::CopyArray(trPositionEnd[itrk],    track->PositionEnd,    4);
  anaUtils::CopyArray(trDirectionStart[itrk], track->DirectionStart, 3);
  anaUtils::CopyArray(trDirectionEnd[itrk],   track->DirectionEnd,   3);

  convUtils::ConvertTrackDetEnumToBitField(track->Detector, (trDetUsed)[itrk]);

  FillSubdetectorInfo(trueParticles, itrk, track);

  // Find the true track associated with this recon track
  FindTrueParticle(trueParticles, trTrueParticleID[itrk], track->TrueObject);

  // Save all recon tracks associated to this true particle
  /*
     if (track->TrueObject){
     AnaTrueObjectC* thistrueobj = track->TrueObject;

  // Vectors are already filled with null pointers --> fill, remove last one (that should be null) and sort
  thistrueobj->ReconParticles.insert(thistrueobj->ReconParticles.begin(),track);
  if (thistrueobj->ReconParticles.back())
  if ( ! suppress_err_msg) std::cout << "minor error 73731" << std::endl;
  if ( ! thistrueobj->ReconParticles.back()) thistrueobj->ReconParticles.pop_back();
  // sort here to keep handle associations from different bunches
  std::sort(thistrueobj->ReconParticles.begin(), thistrueobj->ReconParticles.end(), AnaTrackB::CompareMomentum);

  }
  */
}

//*****************************************************************************
void RedoTreeConverter::FillSubdetectorInfo(std::vector<AnaTrueParticleB*>& trueParticles, int itrk, AnaTrackB* track){
  //*****************************************************************************

  track->nTPCSegments  = 0;
  track->nFGDSegments  = 0;
  track->nECALSegments = 0;
  for (int i = 0; i < SubDetId::kInvalidSubdetector; ++i){
    SubDetId::SubDetEnum idet = static_cast<SubDetId::SubDetEnum>(i);
    if(!SubDetId::GetDetectorUsed(track->Detector, idet)) continue;
    if(SubDetId::IsTPCDetector(idet)){ 
      if (track->nTPCSegments==(int)NMAXTPCS)
        continue;
      AnaTPCParticleB* seg = MakeTpcTrack();
      int tpc = convUtils::GetLocalDetEnum(SubDetId::kTPC, idet);
      FillTpcInfo(trueParticles, itrk, tpc, seg);
      track->TPCSegments[track->nTPCSegments] = seg;
      track->nTPCSegments++;
    }
    else if(SubDetId::IsFGDDetector(idet)){
      if (track->nFGDSegments==(int)NMAXFGDS)
        continue;
      AnaFGDParticleB* seg = MakeFgdTrack();
      int fgd = convUtils::GetLocalDetEnum(SubDetId::kFGD, idet);
      FillFgdInfo(itrk, fgd, seg);
      track->FGDSegments[track->nFGDSegments] = seg;
      track->nFGDSegments++;
    }

    else if(SubDetId::IsECALDetector(idet)){
      if (track->nECALSegments==(int)NMAXECALS)
        continue;

      AnaECALParticleB* seg = MakeEcalTrack();
      int ecal = convUtils::GetLocalDetEnum(SubDetId::kECAL, idet);
      FillEcalInfo(itrk, ecal, seg);
      track->ECALSegments[track->nECALSegments++] = seg;
    }

  }

}

//*****************************************************************************
void RedoTreeConverter::FillSubdetectorInfo(AnaEventB* event, int itrk, AnaTrackB* track){
  //*****************************************************************************

  track->nTPCSegments  = 0;
  track->nFGDSegments  = 0;
  track->nECALSegments = 0;
  for (int i = 0; i < SubDetId::kInvalidSubdetector; ++i){
    SubDetId::SubDetEnum idet = static_cast<SubDetId::SubDetEnum>(i);
    if(!SubDetId::GetDetectorUsed(track->Detector, idet)) continue;
    if(SubDetId::IsTPCDetector(idet)){ 
      AnaTPCParticleB* seg = MakeTpcTrack();
      int tpc = convUtils::GetLocalDetEnum(SubDetId::kTPC, idet);
      FillTpcInfo(event, itrk, tpc, seg);
      track->TPCSegments[track->nTPCSegments] = seg;
      track->nTPCSegments++;
    }
    else if(SubDetId::IsFGDDetector(idet)){
      AnaFGDParticleB* seg = MakeFgdTrack();
      int fgd = convUtils::GetLocalDetEnum(SubDetId::kFGD, idet);
      FillFgdInfo(itrk, fgd, seg);
      track->FGDSegments[track->nFGDSegments] = seg;
      track->nFGDSegments++;
    }

    else if(SubDetId::IsECALDetector(idet)){
      if (track->nECALSegments==(int)NMAXECALS)
        continue;
      AnaECALParticleB* seg = MakeEcalTrack();
      int ecal = convUtils::GetLocalDetEnum(SubDetId::kECAL, idet);
      FillEcalInfo(itrk, ecal, seg);
      track->ECALSegments[track->nECALSegments++] = seg;
    }
  }



}

//*****************************************************************************
void RedoTreeConverter::FillTpcInfo(std::vector<AnaTrueParticleB*>& trueParticles, int itrk, int tpc, AnaTPCParticleB* seg){
  //*****************************************************************************    

  convUtils::ConvertTPCDetEnumToBitField(seg->Detector, tpc);

  seg->Charge         = (trTpcCharge)[itrk][tpc];
  seg->NNodes         = (int)(trTpcNNodes)[itrk][tpc];
  seg->Momentum       = (trTpcMomentum)[itrk][tpc];
  seg->MomentumError  = (trTpcMomentumError)[itrk][tpc];
  //    seg->Length         = (trTpcLength)[itrk][tpc];

  seg->RefitMomentum  = (trTpcRefitMomentum)[itrk][tpc];
  seg->EFieldRefitMomentum  = (trTpcEFieldRefitMomentum)[itrk][tpc];

  seg->dEdxMeas  = (trTpcdEdxMeas)[itrk][tpc];

  seg->dEdxexpMuon   = (trTpcdEdxExpMu)[itrk][tpc];
  seg->dEdxexpEle    = (trTpcdEdxExpEle)[itrk][tpc];
  seg->dEdxexpProton = (trTpcdEdxExpP)[itrk][tpc];
  seg->dEdxexpPion   = (trTpcdEdxExpPi)[itrk][tpc];

  seg->dEdxSigmaMuon   = (trTpcdEdxSigmaMu)[itrk][tpc];
  seg->dEdxSigmaEle    = (trTpcdEdxSigmaEle)[itrk][tpc];
  seg->dEdxSigmaProton = (trTpcdEdxSigmaP)[itrk][tpc];
  seg->dEdxSigmaPion   = (trTpcdEdxSigmaPi)[itrk][tpc];

  //    seg->Purity   = (trTpcPurity)[itrk][tpc];


  for (int i=0;i<4;i++){
    seg->PositionStart[i] = (trTpcPositionStart)[itrk][tpc][i];
    seg->PositionEnd[i]   = (trTpcPositionEnd)[itrk][tpc][i];
  }

  for (int i=0;i<3;i++){
    seg->DirectionStart[i] = (trTpcDirectionStart)[itrk][tpc][i];
    //     seg->DirectionEnd[i]   = (trTpcDirectionEnd)[itrk][tpc][i];
  }

  // Find the true particle assiciated with this recon particle
  FindTrueParticle(trueParticles, (trTpcTrueParticleID)[itrk][tpc], seg->TrueObject);
}

//*****************************************************************************
void RedoTreeConverter::FillTpcInfo(AnaEventB* event, int itrk, int tpc, AnaTPCParticleB* seg){
  //*****************************************************************************    

  convUtils::ConvertTPCDetEnumToBitField(seg->Detector, tpc);

  seg->Charge         = (trTpcCharge)[itrk][tpc];
  seg->NNodes         = (int)(trTpcNNodes)[itrk][tpc];
  seg->Momentum       = (trTpcMomentum)[itrk][tpc];
  seg->MomentumError  = (trTpcMomentumError)[itrk][tpc];
  //    seg->Length         = (trTpcLength)[itrk][tpc];


  seg->RefitMomentum  = (trTpcRefitMomentum)[itrk][tpc];
  seg->EFieldRefitMomentum  = (trTpcEFieldRefitMomentum)[itrk][tpc];

  seg->dEdxMeas  = (trTpcdEdxMeas)[itrk][tpc];

  seg->dEdxexpMuon   = (trTpcdEdxExpMu)[itrk][tpc];
  seg->dEdxexpEle    = (trTpcdEdxExpEle)[itrk][tpc];
  seg->dEdxexpProton = (trTpcdEdxExpP)[itrk][tpc];
  seg->dEdxexpPion   = (trTpcdEdxExpPi)[itrk][tpc];

  seg->dEdxSigmaMuon   = (trTpcdEdxSigmaMu)[itrk][tpc];
  seg->dEdxSigmaEle    = (trTpcdEdxSigmaEle)[itrk][tpc];
  seg->dEdxSigmaProton = (trTpcdEdxSigmaP)[itrk][tpc];
  seg->dEdxSigmaPion   = (trTpcdEdxSigmaPi)[itrk][tpc];

  //    seg->Purity   = (trTpcPurity)[itrk][tpc];


  for (int i=0;i<4;i++){
    seg->PositionStart[i] = (trTpcPositionStart)[itrk][tpc][i];
    seg->PositionEnd[i]   = (trTpcPositionEnd)[itrk][tpc][i];
  }

  for (int i=0;i<3;i++){
    seg->DirectionStart[i] = (trTpcDirectionStart)[itrk][tpc][i];
    //       seg->DirectionEnd[i]   = (trTpcDirectionEnd)[itrk][tpc][i];
  }

  // Find the true track assiciated with this recon particle
  FindTrueParticle(event, (trTpcTrueParticleID)[itrk][tpc], seg->TrueObject);
}

//*****************************************************************************
void RedoTreeConverter::FillFgdInfo(int itrk, int fgd, AnaFGDParticleB* seg){
  //*****************************************************************************    

  convUtils::ConvertFGDDetEnumToBitField(seg->Detector, fgd);

  seg->NNodes         = (int)(trFgdNNodes)[itrk][fgd];
  //  seg->Length         = (trFgdLength)[itrk][fgd];

  seg->Pullmu       = (trFgdPullmu)[itrk][fgd];
  seg->Pullp        = (trFgdPullp)[itrk][fgd];
  seg->Pullpi       = (trFgdPullpi)[itrk][fgd];
  seg->Pullno       = (trFgdPullno)[itrk][fgd];
  seg->Containment  = (trFgdContainment)[itrk][fgd];
  seg->X            = (trFgdX)[itrk][fgd];

  /*
     seg->AvgTime      = (trFgdAvgTime)[itrk][fgd];


     seg->Vertex1x1  = (trFgdVertex1x1)[itrk][fgd];
     seg->Vertex3x3  = (trFgdVertex3x3)[itrk][fgd];
     seg->Vertex5x5  = (trFgdVertex5x5)[itrk][fgd];
     seg->Vertex7x7  = (trFgdVertex7x7)[itrk][fgd];
     seg->VertexLayer  = (trFgdVertexLayer)[itrk][fgd];
     */
  for (int i=0;i<4;i++){
    seg->PositionStart[i] = (trFgdPositionStart)[itrk][fgd][i];
    seg->PositionEnd[i]   = (trFgdPositionEnd)[itrk][fgd][i];
  }

  for (int i=0;i<3;i++){
    seg->DirectionStart[i] = (trFgdDirectionStart)[itrk][fgd][i];
    //        seg->DirectionEnd[i]   = (trFgdDirectionEnd)[itrk][fgd][i];
  }

}

//*****************************************************************************
void RedoTreeConverter::FillEcalInfo(int itrk, int ecal, AnaECALParticleB* seg){
  //*****************************************************************************    

  convUtils::ConvertECALDetEnumToBitField(seg->Detector, ecal);

  seg->NNodes               = (int)(trECALNNodes)[itrk][ecal];
  seg->Length               = (trECALLength)[itrk][ecal];
  seg->EMEnergy             = (trECALEMEnergy)[itrk][ecal];
  seg->PIDMipEm             = trECALPIDMipEm[itrk][ecal];
  seg->PIDEmHip             = trECALPIDEmHip[itrk][ecal];

  //seg->IsShowerLike         = trECALIsShowerLike[itrk][ecal];
  //seg->AvgTime              = trECALAvgTime[itrk][ecal];
  //seg->EDeposit             = trECALEDeposit[itrk][ecal];
  
  
  seg->MostUpStreamLayerHit = (int)trECALMostUpStreamLayerHit[itrk][ecal];
  
  for (int i = 0; i < 3; i++){
    seg->ShowerPosition[i] = (trECALShowerPosition)[itrk][ecal][i];
    seg->DirectionStart[i] = (trECALDirectionStart)[itrk][ecal][i];
    //seg->DirectionEnd[i]   = (trECALDirectionStart)[itrk][ecal][i];
  }
  
  for (int i = 0; i < 4; i++){
    seg->PositionStart[i] = (trECALPositionStart)[itrk][ecal][i];
  //  seg->PositionEnd[i]   = (trECALPositionEnd)[itrk][ecal][i];
  }
  
}

//*****************************************************************************
void RedoTreeConverter::FillSmrdInfo(int itrk, int smrd, AnaSMRDParticleB* seg){
  //*****************************************************************************    

  convUtils::ConvertSMRDDetEnumToBitField(seg->Detector, smrd);

  for (int i=0;i<4;i++){
    seg->PositionStart[i] = (trSMRDPositionStart)[itrk][smrd][i];
    seg->PositionEnd[i]   = (trSMRDPositionEnd)[itrk][smrd][i];
  }

  for (int i=0;i<3;i++){
    seg->DirectionStart[i] = (trSMRDDirectionStart)[itrk][smrd][i];
  }
}

//*****************************************************************************
void RedoTreeConverter::FillP0dInfo(int itrk, int p0d, AnaP0DParticleB* seg){
  //*****************************************************************************    

  SubDetId::SetDetectorUsed(seg->Detector, SubDetId::kP0D);

  seg->Length = (trP0DLength)[itrk][p0d];
  seg->ELoss = (trP0DELoss)[itrk][p0d];

  for (int i=0;i<4;i++){
    seg->PositionStart[i] = (trP0DPositionStart)[itrk][p0d][i];
    seg->PositionEnd[i]   = (trP0DPositionEnd)[itrk][p0d][i];
  }

  for (int i=0;i<3;i++){
    seg->DirectionStart[i] = (trP0DDirectionStart)[itrk][p0d][i];
  }
}

//*****************************************************************************
void RedoTreeConverter::FillTrueParticleInfo(int ntrueVertices, AnaTrueVertexB** trueVertices, int itrk, AnaTrueParticleB* particle){
  //*****************************************************************************
  particle->ID         = (sTrueParticleID)[itrk];
  particle->PrimaryID  = (sTrueParticlePrimaryID)[itrk];
  particle->ParentID   = (sTrueParticleParentID)[itrk];
  particle->GParentID  = (sTrueParticleGParentID)[itrk];
  particle->PDG        = (sTrueParticlePDG)[itrk];
  particle->ParentPDG  = (sTrueParticleParentPDG)[itrk];
  particle->GParentPDG = (sTrueParticleGParentPDG)[itrk];

  particle->Charge     = (sTrueParticleCharge)[itrk];
  particle->Momentum   = (sTrueParticleMomentum)[itrk];
  
  particle->IsTruePrimaryPi0DecayPhoton      = (sTrueParticleIsTruePrimaryPi0DecayPhoton)[itrk];
  particle->IsTruePrimaryPi0DecayPhotonChild = (sTrueParticleIsTruePrimaryPi0DecayPhotonChild)[itrk];
  
  

  anaUtils::CopyArray(sTrueParticlePosition[itrk],    particle->Position,    4);
  anaUtils::CopyArray(sTrueParticlePositionEnd[itrk], particle->PositionEnd, 4);
  anaUtils::CopyArray(sTrueParticleDirection[itrk],   particle->Direction,   3);

  particle->nDetCrossings = 0;
  UInt_t nCrossings = sTrueParticleNDetCrossings[itrk];
  // for backward compatibility since  sTrueParticleNDetCrossings have been just introduced in FlatTree
  if( nCrossings==0 || nCrossings > NMAXCROSSEDDET ) nCrossings = NMAXCROSSEDDET; 
  anaUtils::CreateArray(particle->DetCrossings, nCrossings);
  for(UInt_t i=0;i<nCrossings;i++){
    if (sTrueParticleDetector[itrk][i]<0) continue;
    AnaDetCrossingB* cross = new AnaDetCrossingB();
    convUtils::ConvertTrueParticleDetEnumToBitField(cross->Detector, sTrueParticleDetector[itrk][i]);

    cross->InActive = (bool) sTrueParticleInActive[itrk][i];

    anaUtils::CopyArray(sTrueParticleEntrancePosition[itrk][i], cross->EntrancePosition, 4);
    anaUtils::CopyArray(sTrueParticleExitPosition[itrk][i], cross->ExitPosition, 4);
    
    anaUtils::CopyArray(sTrueParticleEntranceMomentum[itrk][i], cross->EntranceMomentum, 3);
    anaUtils::CopyArray(sTrueParticleExitMomentum[itrk][i],     cross->ExitMomentum,     3);

    particle->DetCrossings[particle->nDetCrossings] = cross;
    particle->nDetCrossings++;
  }

  particle->TrueVertex = NULL;
  particle->VertexID  = (sTrueParticleTruthVertexID)[itrk];

  for (int i = 0; i < ntrueVertices; ++i){
    if (particle->VertexID == trueVertices[i]->ID){
      // Associate the vertex to this track
      particle->TrueVertex = trueVertices[i];
      // add this track to the list of true tracks in the true vertex
      if((UInt_t)particle->TrueVertex->nTrueParticles >= NMAXTRUEPARTICLES) break;
      particle->TrueVertex->TrueParticles[particle->TrueVertex->nTrueParticles] = particle;	
      particle->TrueVertex->nTrueParticles++;
      break;
    }
  } 
}

//*****************************************************************************
void RedoTreeConverter::FillTrueParticleInfo(AnaEventB* event, int itrk, AnaTrueParticleB* particle){
  //*****************************************************************************
 
  if (!event) 
    return;
  
  return FillTrueParticleInfo(event->nTrueVertices, event->TrueVertices, itrk, particle);
}

//*****************************************************************************
void RedoTreeConverter::FillTrueVertexInfo(int ivtx, AnaTrueVertexB* vertex){
  //*****************************************************************************

  vertex->ID = sTrueVertexID[ivtx];
  vertex->NuPDG = sTrueVertexNuPDG[ivtx];
  vertex->NuEnergy = sTrueVertexNuEnergy[ivtx];

  anaUtils::CopyArray(sTrueVertexPosition[ivtx], vertex->Position, 4);

  vertex->Bunch    = sTrueVertexBunch[ivtx];

  // will be updated in FillTrueParticleInfo
  anaUtils::CreateArray(vertex->TrueParticles, sNTrueParticles);
  vertex->nTrueParticles = 0;	
}

//*****************************************************************************
void RedoTreeConverter::FindTrueParticle(std::vector<AnaTrueParticleB*>& trueParticles, int ID, AnaTrueObjectC*& trueTrack){
  //*****************************************************************************

  // Compare the IDs to find the AnaTrueObjectC associated with this recon particle.
  trueTrack = NULL;

  if (ID==-999) return; // there's no true track association

  for (UInt_t it = 0; it < trueParticles.size(); it++){
    AnaTrueObjectC* trueTrack2 = trueParticles[it];
    if (ID == trueTrack2->ID){
      trueTrack = trueTrack2;
      break; // Stop when association is found.
    }
  }
}

//*****************************************************************************
void RedoTreeConverter::FindTrueParticle(AnaEventB* event, int ID, AnaTrueObjectC*& trueTrack){
  //*****************************************************************************

  // Compare the IDs to find the AnaTrueObjectC associated with this recon particle.
  trueTrack = NULL;
  if (ID==-999) return; // there's no true track association

  for (int it = 0; it < event->nTrueParticles; it++){
    AnaTrueObjectC* trueTrack2 = event->TrueParticles[it];
    if (ID == trueTrack2->ID){
      trueTrack = trueTrack2;
      break; // Stop when association is found.
    }
  }
}

//*****************************************************************************
void RedoTreeConverter::FillVertexInfo(int ivtx, AnaVertexB* vertex, AnaEventB* event){
  //*****************************************************************************

  vertex->PrimaryIndex = (vPrimaryIndex)[ivtx];
  anaUtils::CopyArray(vPosition[ivtx], vertex->Position, 4);

  // Find AnaParticleB associated with this global vertex
  for (int d = 0; d < vNReconParticles[ivtx]; d++) {
    AnaParticleB* matchedParticle = FindParticle(vParticlesUniqueID[ivtx][d], event);
    if ( ! matchedParticle) continue;

    // fill only if in this bunch, should be already sorted in flat tree, then null pointers will be added at the end
    //    if (matchedParticle->Bunch != Bunch) continue;
    //    vertex->Particles.push_back(matchedParticle);

    // Save vertices info in AnaParticleB
    //    matchedParticle->ReconVertices.push_back(vertex);

    // choose the vertex more primary (in this bunch)
    //        if ( ! matchedParticle->Vertex || matchedParticle->Vertex->PrimaryIndex > vertex->PrimaryIndex)
    //            matchedParticle->Vertex = vertex;
  }

  // Find true vertices associated with this global vertex (by index)
  for (int d = 0; d < vNTrueVerticesMatch[ivtx]; d++) {
    std::pair<AnaTrueVertexB*, AnaRecTrueMatchB> myMatch;
    int thisTrueVertexIndex = vTrueVertexIndex[ivtx][d];
    if (thisTrueVertexIndex == -1) continue;
    if (thisTrueVertexIndex < -1 || thisTrueVertexIndex >= event->nTrueVertices)
      std::cout << "minor error 6789" << std::endl;

    myMatch.first = event->TrueVertices[thisTrueVertexIndex];
    myMatch.second.Cleanliness  = (vTrueVerticesClean)[ivtx][d];
    myMatch.second.Completeness = (vTrueVerticesCompl)[ivtx][d];
    vertex->TrueVerticesMatch.push_back(myMatch);
  }

  // Let's keep also vertex->TrueVertex by now (the user could just called GetMainTrueVertex instead)
  vertex->TrueVertex = vertex->GetMainTrueVertex(false); // vertex->GetMainTrueVertex() could replace this info
}


//*****************************************************************************
void RedoTreeConverter::FillVertexInfo(int ivtx, AnaVertexB* vertex, AnaBunchB* bunch){
  //*****************************************************************************

  vertex->PrimaryIndex = (vPrimaryIndex)[ivtx];
  vertex->Bunch        = (vBunch)[ivtx];
  anaUtils::CopyArray(vPosition[ivtx], vertex->Position, 4);

  // Find AnaParticleB associated with this global vertex
  for (int d = 0; d < vNReconParticles[ivtx]; d++) {
    AnaParticleB* matchedParticle = FindParticle(vParticlesUniqueID[ivtx][d], bunch);
    if ( ! matchedParticle) continue;

    // fill only if in this bunch, should be already sorted in flat tree, then null pointers will be added at the end
    //    if (matchedParticle->Bunch != Bunch) continue;
    //    vertex->Particles.push_back(matchedParticle);

    // Save vertices info in AnaParticleB
    //    matchedParticle->ReconVertices.push_back(vertex);

    // choose the vertex more primary (in this bunch)
    //        if ( ! matchedParticle->Vertex || matchedParticle->Vertex->PrimaryIndex > vertex->PrimaryIndex)
    //            matchedParticle->Vertex = vertex;
  }

  // Find true vertices associated with this global vertex (by index)
  for (int d = 0; d < vNTrueVerticesMatch[ivtx]; d++) {
    std::pair<AnaTrueVertexB*, AnaRecTrueMatchB> myMatch;
    int thisTrueVertexIndex = vTrueVertexIndex[ivtx][d];
    if (thisTrueVertexIndex == -1) continue;
    if (thisTrueVertexIndex < -1 || thisTrueVertexIndex >= (int)_spill->TrueVertices.size())
      std::cout << "minor error 6789" << std::endl;

    myMatch.first = _spill->TrueVertices[thisTrueVertexIndex];
    myMatch.second.Cleanliness  = (vTrueVerticesClean)[ivtx][d];
    myMatch.second.Completeness = (vTrueVerticesCompl)[ivtx][d];
    vertex->TrueVerticesMatch.push_back(myMatch);
  }

  // Let's keep also vertex->TrueVertex, by now
  vertex->TrueVertex = vertex->GetMainTrueVertex(); // vertex->GetMainTrueVertex() could replace this info
}

//*****************************************************************************
AnaParticleB* RedoTreeConverter::FindParticle(int ID, AnaEventB* event){
  //*****************************************************************************

  // Compare the UniqueID to find the corresponding AnaParticleB
  // AnaParticleB already exists because it is saved for first
  // Look first in the same event bunch as the vertex
  for (int ittrack = 0; ittrack < event->nParticles; ittrack++) {

    if (ID == event->Particles[ittrack]->UniqueID) return event->Particles[ittrack];
  }

  //We are not using out of bunch tracks in the BANFF fit, so cannot do these checks.

  //    // If not found, look into OutOfBunch (saved for first in the Flat Tree, if parameters enabled)
  //    if (_spill->OutOfBunch) {
  //        for (ittrack = _spill->OutOfBunch->Particles.begin(); ittrack != _spill->OutOfBunch->Particles.end(); ittrack++) {
  //            if (ID == (*ittrack)->UniqueID) return *ittrack;
  //        }
  //    }

  if (event->Bunch != -1) //&& _spill->OutOfBunch->Bunch != -999)
    if ( ! suppress_err_msg) std::cout << "minor error 0999" << std::endl;

  return NULL;
}

//*****************************************************************************
AnaParticleB* RedoTreeConverter::FindParticle(int ID, AnaBunchB* bunch){
  //*****************************************************************************

  // Compare the UniqueID to find the corresponding AnaParticleB
  // AnaParticleB already exists because it is saved for first
  std::vector<AnaParticleB*>::iterator ittrack;

  // Look first in the same bunch of the vertex
  for (ittrack = bunch->Particles.begin(); ittrack != bunch->Particles.end(); ittrack++) {
    if (ID == (*ittrack)->UniqueID) return *ittrack;
  }

  // If not found, look into OutOfBunch (saved for first in the Flat Tree, if parameters enabled)
  if (_spill->OutOfBunch) {
    for (ittrack = _spill->OutOfBunch->Particles.begin(); ittrack != _spill->OutOfBunch->Particles.end(); ittrack++) {
      if (ID == (*ittrack)->UniqueID) return *ittrack;
    }
  }

  if (bunch->Bunch != -1 && _spill->OutOfBunch->Bunch != -999)
    if ( ! suppress_err_msg) std::cout << "minor error 0999" << std::endl;

  return NULL;
}


