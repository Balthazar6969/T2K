//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Jul  6 16:31:52 2011 by ROOT version 5.28/00
// from TTree numutree/numutree analysis tree
// found on file: ../prod4C/NeutMCRun2.root
//////////////////////////////////////////////////////////

#ifndef NumuFlatTreeDefinition_h
#define NumuFlatTreeDefinition_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class NumuFlatTreeDefinition {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Double_t        std_TotalGoodSpillPOT;
   Double_t        std_TotalGoodSpillDaqPOT;
   Double_t        std_TotalGoodSpillAndDQPOT;
   Double_t        std_TotalPOT;
   Double_t        std_TotalRemoveDaqPOT;
   Double_t        std_SpillPOT;
   Int_t           std_TrueSpillInfo;
   Int_t           std_Evt;
   Int_t           std_MRRun;
   Int_t           std_Run;
   Int_t           std_Subrun;
   Int_t           std_FileRun;
   Int_t           std_FileSubrun;
   Int_t           std_Spill;
   Int_t           std_GoodSpill;
   Int_t           std_GoodDaq;
   Int_t           std_ND280OffFlag;
   Int_t           std_TPCFlag ;
   Int_t           std_FGDFlag ;
   Int_t           std_ECALFlag;
   Int_t           std_DSECALFlag;
   Int_t           std_P0DFlag;
   Int_t           std_SMRDFlag;
   Int_t           std_MAGNETFlag;
   Int_t           std_Entry;
   Int_t           std_NBunches;
   Int_t           std_NValidBunches;
   Int_t           std_BunchNbr;
   Int_t           std_SpillWithTPC1Track;
   Int_t           std_NTrueVertices;
   Int_t           std_TrueReacCode[24];   //[sNTrueVertices]
   Double_t        std_TrueVertexPos[24][4];   //[sNTrueVertices]
   Double_t        std_TrueNuEnergy[24];   //[sNTrueVertices]
   Double_t        std_TrueNuDir[24][3];   //[sNTrueVertices]
   Int_t           std_TrueNuPDG[24];   //[sNTrueVertices]
   Double_t        std_TrueEnuWeight[24];   //[sNTrueVertices]
   Int_t           std_TrueVertexBunch[24];   //[sNTrueVertices]
   Double_t        std_TrueLeptonMom[24];   //[sNTrueVertices]
   Double_t        std_TrueProtonMom[24];   //[sNTrueVertices]
   Double_t        std_TrueTargetMom[24];   //[sNTrueVertices]
   Double_t        std_TrueLeptonDir[24][3];   //[sNTrueVertices]
   Double_t        std_TrueProtonDir[24][3];   //[sNTrueVertices]
   Double_t        std_TrueTargetDir[24][3];   //[sNTrueVertices]
   Double_t        std_TrueTargetPDG[24];   //[sNTrueVertices]
   Int_t           std_TrueBaryonSize[24];   //[sNTrueVertices]
   Double_t        std_TrueQ2[24];   //[sNTrueVertices]
   Int_t           std_TrueVertexDet[24];   //[sNTrueVertices]
   Int_t           std_NECALTracks;
   Int_t           std_NP0DTracks;
   Int_t           std_NSMRDTracks;
   Int_t           std_NFGDTracks;
   Int_t           std_NTPCTracks;
   Int_t           std_NFGDUnusedHits[2];
   Int_t           std_NP0DUnusedHits;
   Int_t           std_NSMRDUnusedHits[4];
   Int_t           std_NTracksWithFGDTotal;
   Int_t           std_NTracksWithTPCTotal;
   Int_t           std_NOtherTracksWithTPCTotal;
   Int_t           std_NTpc1TracksTotal;
   Int_t           std_NTpc2TracksTotal;
   Int_t           std_NTpc3TracksTotal;
   Int_t           std_NVertices;
   Double_t        stdB_POTBunch;
   Int_t           stdB_Bunch;
   Double_t        stdB_BeamBunchTime;
   Int_t           stdB_BunchWithMaxMomInSpill;
   Double_t        stdB_TBunch;
   Double_t        stdB_Vertex[4][4];
   Int_t           stdB_NVertices;
   Int_t           stdB_vtxPrimaryIndex[10];   //[NVertices]
   Double_t        stdB_vtxPosition[10][4];   //[NVertices]
   Double_t        stdB_vtxTrueVertexPosition[10][4];   //[NVertices]
   Double_t        stdB_vtxVariance[10][4];   //[NVertices]
   Double_t        stdB_vtxQuality[10];   //[NVertices]
   Int_t           stdB_vtxNConstituents[10];   //[NVertices]
   Int_t           stdB_NTrueVerticesPerBunch;
   Int_t           stdB_NFgdTimeBin;
   Int_t           stdB_TimeBinNHits[9][2];   //[NFgdTimeBin]
   Double_t        stdB_TimeBinMinTime[9];   //[NFgdTimeBin]
   Double_t        stdB_TimeBinMaxTime[9];   //[NFgdTimeBin]
   Int_t           stdB_TimeBinNFGD1UnusedHits[9];   //[NFgdTimeBin]
   Int_t           stdB_TimeBinNFGD2UnusedHits[9];   //[NFgdTimeBin]
   Int_t           stdB_NTracksGlobal;
   Int_t           stdB_NTracksWithTPCPos;
   Int_t           stdB_NTracksWithTPCNeg;
   Int_t           stdB_NTracksWithTPC1;
   Int_t           stdB_NTracksWithTPC2;
   Int_t           stdB_NTracksWithTPC3;
   Int_t           stdB_NTracksWithTPCOther;
   Int_t           stdB_NTracksWithDSECAL;
   Int_t           stdB_NTracksWithECAL;
   Int_t           stdB_ecalDetectors[10];   //[NTracksWithECAL]
   Int_t           stdB_ecalDetUsed[10][19];   //[NTracksWithECAL]
   Int_t           stdB_ecalNHits[10];   //[NTracksWithECAL]
   Int_t           stdB_ecalNDOF[10];   //[NTracksWithECAL]
   Double_t        stdB_ecalChi2[10];   //[NTracksWithECAL]
   Double_t        stdB_ecalPositionStart[10][4];   //[NTracksWithECAL]
   Double_t        stdB_ecalPositionEnd[10][4];   //[NTracksWithECAL]
   Double_t        stdB_ecalDir[10][3];   //[NTracksWithECAL]
   Double_t        stdB_ecalCosTheta[10];   //[NTracksWithECAL]
   Double_t        stdB_ecalEDeposit[10];   //[NTracksWithECAL]
   Int_t           stdB_ecalTrTruePDG[10];   //[NTracksWithECAL]
   Double_t        stdB_ecalECALTrShVal[10];   //[NTracksWithECAL]
   Double_t        stdB_ecalECALEMHadVal[10];   //[NTracksWithECAL]
   Double_t        stdB_ecalECALEMEnergy[10];   //[NTracksWithECAL]
   Double_t        stdB_ecalECALEDeposit[10];   //[NTracksWithECAL]
   Double_t        stdB_ecalECALIsShowerLike[10];   //[NTracksWithECAL]
   Double_t        stdB_ecalECALIsTrackLike[10];   //[NTracksWithECAL]
   Int_t           stdB_NTracksWithSMRD;
   Int_t           stdB_smrdDetectors[10];   //[NTracksWithSMRD]
   Int_t           stdB_smrdDetUsed[10][19];   //[NTracksWithSMRD]
   Int_t           stdB_smrdNHits[10];   //[NTracksWithSMRD]
   Int_t           stdB_smrdNDOF[10];   //[NTracksWithSMRD]
   Double_t        stdB_smrdChi2[10];   //[NTracksWithSMRD]
   Double_t        stdB_smrdPositionStart[10][4];   //[NTracksWithSMRD]
   Double_t        stdB_smrdPositionEnd[10][4];   //[NTracksWithSMRD]
   Double_t        stdB_smrdDir[10][3];   //[NTracksWithSMRD]
   Double_t        stdB_smrdCosTheta[10];   //[NTracksWithSMRD]
   Double_t        stdB_smrdEDeposit[10];   //[NTracksWithSMRD]
   Int_t           stdB_smrdSMRDNHits[10];   //[NTracksWithSMRD]
   Int_t           stdB_smrdTrTruePDG[10];   //[NTracksWithSMRD]
   Int_t           stdB_NTracksWithP0D;
   Int_t           stdB_p0dDetectors[10];   //[NTracksWithP0D]
   Int_t           stdB_p0dDetUsed[10][19];   //[NTracksWithP0D]
   Int_t           stdB_p0dNHits[10];   //[NTracksWithP0D]
   Int_t           stdB_p0dNDOF[10];   //[NTracksWithP0D]
   Double_t        stdB_p0dChi2[10];   //[NTracksWithP0D]
   Double_t        stdB_p0dPositionStart[10][4];   //[NTracksWithP0D]
   Double_t        stdB_p0dPositionEnd[10][4];   //[NTracksWithP0D]
   Double_t        stdB_p0dDir[10][3];   //[NTracksWithP0D]
   Double_t        stdB_p0dCosTheta[10];   //[NTracksWithP0D]
   Double_t        stdB_p0dEDeposit[10];   //[NTracksWithP0D]
   Int_t           stdB_p0dP0DNHits[10];   //[NTracksWithP0D]
   Int_t           stdB_p0dTrTruePDG[10];   //[NTracksWithP0D]
   Int_t           stdB_NTracksWithTPC;
   Int_t           stdB_trDetectors[10];   //[NTracksWithTPC]
   Int_t           stdB_trDetUsed[10][19];   //[NTracksWithTPC]
   Int_t           stdB_trNHits[10];   //[NTracksWithTPC]
   Int_t           stdB_trNDOF[10];   //[NTracksWithTPC]
   Double_t        stdB_trChi2[10];   //[NTracksWithTPC]
   Double_t        stdB_trPositionStart[10][4];   //[NTracksWithTPC]
   Double_t        stdB_trPositionEnd[10][4];   //[NTracksWithTPC]
   Double_t        stdB_trDir[10][3];   //[NTracksWithTPC]
   Double_t        stdB_trMom[10];   //[NTracksWithTPC]
   Double_t        stdB_trMomError[10];   //[NTracksWithTPC]
   Double_t        stdB_trMomRelErr[10];   //[NTracksWithTPC]
   Int_t           stdB_trCharge[10];   //[NTracksWithTPC]
   Double_t        stdB_trCosTheta[10];   //[NTracksWithTPC]
   Double_t        stdB_trEnuWeight[10];   //[NTracksWithTPC]
   Int_t           stdB_trNFgds[10];   //[NTracksWithTPC]
   Int_t           stdB_trNTracker[10];   //[NTracksWithTPC]
   Double_t        stdB_trL[10];   //[NTracksWithTPC]
   Double_t        stdB_trMomLocal[10];   //[NTracksWithTPC]
   Double_t        stdB_trMomLocalTpc[10];   //[NTracksWithTPC]
   Double_t        stdB_trChargeLocal[10];   //[NTracksWithTPC]
   Double_t        stdB_trECALTrShVal[10];   //[NTracksWithTPC]
   Double_t        stdB_trECALEMHadVal[10];   //[NTracksWithTPC]
   Double_t        stdB_trECALEMEnergy[10];   //[NTracksWithTPC]
   Double_t        stdB_trECALEDeposit[10];   //[NTracksWithTPC]
   Double_t        stdB_trECALIsShowerLike[10];   //[NTracksWithTPC]
   Double_t        stdB_trECALIsTrackLike[10];   //[NTracksWithTPC]
   Int_t           stdB_trSmrdNHits[10];   //[NTracksWithTPC]
   Int_t           stdB_trFgdDet[10][2];   //[NTracksWithTPC]
   Int_t           stdB_trFgdNHits[10][2];   //[NTracksWithTPC]
   Double_t        stdB_trFgdE[10][2];   //[NTracksWithTPC]
   Double_t        stdB_trFgdX[10][2];   //[NTracksWithTPC]
   Double_t        stdB_trFgdL[10][2];   //[NTracksWithTPC]
   Double_t        stdB_trFgdPullmu[10][2];   //[NTracksWithTPC]
   Double_t        stdB_trFgdPullpi[10][2];   //[NTracksWithTPC]
   Double_t        stdB_trFgdPullp[10][2];   //[NTracksWithTPC]
   Double_t        stdB_trFgdPullno[10][2];   //[NTracksWithTPC]
   Double_t        stdB_trFgdPositionStart[10][2][4];   //[NTracksWithTPC]
   Double_t        stdB_trFgdPositionEnd[10][2][4];   //[NTracksWithTPC]
   Int_t           stdB_trTpcDet[10][3];   //[NTracksWithTPC]
   Double_t        stdB_trTpcCharge[10][3];   //[NTracksWithTPC]
   Double_t        stdB_trTpcMom[10][3];   //[NTracksWithTPC]
   Double_t        stdB_trTpcDir[10][3][3];   //[NTracksWithTPC]
   Double_t        stdB_trTpcMomRelErr[10][3];   //[NTracksWithTPC]
   Int_t           stdB_trTpcNHits[10][3];   //[NTracksWithTPC]
   Double_t        stdB_trTpcL[10][3];   //[NTracksWithTPC]
   Int_t           stdB_trTpcTruePDG[10][3];   //[NTracksWithTPC]
   Double_t        stdB_trCT[10][3];   //[NTracksWithTPC]
   Double_t        stdB_trPullmu[10][3];   //[NTracksWithTPC]
   Double_t        stdB_trPullele[10][3];   //[NTracksWithTPC]
   Double_t        stdB_trPullp[10][3];   //[NTracksWithTPC]
   Double_t        stdB_trPullpi[10][3];   //[NTracksWithTPC]
   Double_t        stdB_trPullk[10][3];   //[NTracksWithTPC]
   Double_t        stdB_trCTexpmu[10][3];   //[NTracksWithTPC]
   Double_t        stdB_trCTexpele[10][3];   //[NTracksWithTPC]
   Double_t        stdB_trCTexpp[10][3];   //[NTracksWithTPC]
   Double_t        stdB_trCTexppi[10][3];   //[NTracksWithTPC]
   Double_t        stdB_trCTexpk[10][3];   //[NTracksWithTPC]
   Double_t        stdB_trCTerrmu[10][3];   //[NTracksWithTPC]
   Double_t        stdB_trCTerrele[10][3];   //[NTracksWithTPC]
   Double_t        stdB_trCTerrp[10][3];   //[NTracksWithTPC]
   Double_t        stdB_trCTerrpi[10][3];   //[NTracksWithTPC]
   Double_t        stdB_trCTerrk[10][3];   //[NTracksWithTPC]
   Int_t           stdB_trTruePDG[10];   //[NTracksWithTPC]
   Int_t           stdB_trTrueParentPDG[10];   //[NTracksWithTPC]
   Int_t           stdB_trTrueGParentPDG[10];   //[NTracksWithTPC]
   Double_t        stdB_trTruePur[10];   //[NTracksWithTPC]
   Double_t        stdB_trTruePosition[10][4];   //[NTracksWithTPC]
   Double_t        stdB_trTrueDirection[10][3];   //[NTracksWithTPC]
   Double_t        stdB_trTrueMomentum[10];   //[NTracksWithTPC]
   Int_t           stdB_trTrueCharge[10];   //[NTracksWithTPC]
   Int_t           stdB_trTrueVertexIndex[10];   //[NTracksWithTPC]
   Int_t           stdB_NTracksWithFGD;
   Int_t           stdB_fgdDetectors[10];   //[NTracksWithFGD]
   Int_t           stdB_fgdDetUsed[10][19];   //[NTracksWithFGD]
   Int_t           stdB_fgdNHits[10];   //[NTracksWithFGD]
   Int_t           stdB_fgdNDOF[10];   //[NTracksWithFGD]
   Double_t        stdB_fgdChi2[10];   //[NTracksWithFGD]
   Double_t        stdB_fgdPositionStart[10][4];   //[NTracksWithFGD]
   Double_t        stdB_fgdPositionEnd[10][4];   //[NTracksWithFGD]
   Double_t        stdB_fgdDir[10][3];   //[NTracksWithFGD]
   Double_t        stdB_fgdCosTheta[10];   //[NTracksWithFGD]
   Int_t           stdB_fgdTrTruePDG[10];   //[NTracksWithFGD]
   Double_t        stdB_fgdECALTrShVal[10];   //[NTracksWithFGD]
   Double_t        stdB_fgdECALEMHadVal[10];   //[NTracksWithFGD]
   Double_t        stdB_fgdECALEMEnergy[10];   //[NTracksWithFGD]
   Double_t        stdB_fgdECALEDeposit[10];   //[NTracksWithFGD]
   Double_t        stdB_fgdECALIsShowerLike[10];   //[NTracksWithFGD]
   Double_t        stdB_fgdECALIsTrackLike[10];   //[NTracksWithFGD]
   Int_t           stdB_fgdFgdNHits[10];   //[NTracksWithFGD]
   Double_t        stdB_fgdE[10];   //[NTracksWithFGD]
   Double_t        stdB_fgdX[10];   //[NTracksWithFGD]
   Double_t        stdB_fgdPullmu[10];   //[NTracksWithFGD]
   Double_t        stdB_fgdPullpi[10];   //[NTracksWithFGD]
   Double_t        stdB_fgdPullp[10];   //[NTracksWithFGD]
   Double_t        stdB_fgdPullno[10];   //[NTracksWithFGD]
   Int_t           stdB_fgdSmrdNHits[10];   //[NTracksWithFGD]
   Double_t        stdB_trENuRec_1Muon[10];   //[NTracksWithTPC]
   Double_t        stdB_trEMuRec_1Muon[10];   //[NTracksWithTPC]
   Double_t        stdB_trQ2_1Muon[10];   //[NTracksWithTPC]
   Double_t        stdB_ENuRec_CCQE;
   Double_t        stdB_Q2_CCQE;
   Double_t        stdB_PQ2_CCQE;
   Double_t        stdB_NuInvMass_CCQE;
   Double_t        stdB_EProton_CCQE;
   Double_t        stdB_FinalCosTheta_CCQE;
   Double_t        stdB_FinalMass_CCQE;
   Double_t        stdB_FinalEnergy_CCQE;
   Double_t        stdB_FinalMom_CCQE;
   Double_t        stdB_EarliestTrackTime;
   Int_t           stdB_NClusters;
   Int_t           stdB_NClusterscut[3][5];
   Double_t        stdB_ClusterPos[34][4];   //[NClusters]
   Int_t           stdB_ClusterNHits[34];   //[NClusters]
   Double_t        stdB_ClusterRMS[34][4];   //[NClusters]
   Double_t        stdB_ClusterCharge[34];   //[NClusters]
   Double_t        stdB_ClusterDistVertex[34][3];   //[NClusters]
   Double_t        stdB_ClusterTimeDistVertex[34][3];   //[NClusters]
   Double_t        g10aB_trCTCorr[10][3];   //[NTracksWithTPC]
   Double_t        g10aB_trPullCorrmu[10][3];   //[NTracksWithTPC]
   Double_t        g10aB_trPullCorrele[10][3];   //[NTracksWithTPC]
   Double_t        g10aB_trPullCorrp[10][3];   //[NTracksWithTPC]
   Double_t        g10aB_trPullCorrpi[10][3];   //[NTracksWithTPC]
   Double_t        g10aB_trPullCorrk[10][3];   //[NTracksWithTPC]
   Double_t        g10aB_trProbmu[10];   //[NTracksWithTPC]
   Double_t        g10aB_trProbele[10];   //[NTracksWithTPC]
   Double_t        g10aB_trProbp[10];   //[NTracksWithTPC]
   Double_t        g10aB_trProbpi[10];   //[NTracksWithTPC]
   Double_t        g10aB_trProbk[10];   //[NTracksWithTPC]
   Double_t        g10aB_trCTexpCorrmu[10][3];   //[NTracksWithTPC]
   Double_t        g10aB_trCTexpCorrele[10][3];   //[NTracksWithTPC]
   Double_t        g10aB_trCTexpCorrp[10][3];   //[NTracksWithTPC]
   Double_t        g10aB_trCTexpCorrpi[10][3];   //[NTracksWithTPC]
   Double_t        g10aB_trCTexpCorrk[10][3];   //[NTracksWithTPC]
   Int_t           g10aB_NTPCTracksCut[3];
   Int_t           g10aB_NTPCTracksPosCut[3];
   Int_t           g10aB_NTPCTracksNegCut[3];
   Double_t        g10aB_ENuRec[2];
   Double_t        g10aB_EMuRec[2];
   Double_t        g10aB_PQ2[2];
   Double_t        g10aB_MuQ2[2];
   Double_t        g10aB_DeltaThetaP[2];
   Double_t        g10aB_EProton;
   Double_t        g10aB_FinalCosTheta;
   Double_t        g10aB_FinalMass;
   Double_t        g10aB_FinalEnergy;
   Double_t        g10aB_FinalMom;

   // List of branches
   TBranch        *b_sPOTTotalGoodSpill;   //!
   TBranch        *b_sPOTTotalGoodSpillDaq;   //!
   TBranch        *b_sPOTTotalGoodSpillAndDQ;   //!
   TBranch        *b_sPOTTotal;   //!
   TBranch        *b_sPOTTotalRemoveDaq;   //!
   TBranch        *b_sPOTSpill;   //!
   TBranch        *b_sTrueSpillInfo;   //!
   TBranch        *b_sEvt;   //!
   TBranch        *b_sMRRun;   //!
   TBranch        *b_sRun;   //!
   TBranch        *b_sSubrun;   //!
   TBranch        *b_sFileRun;   //!
   TBranch        *b_sFileSubrun;   //!
   TBranch        *b_sSpill;   //!
   TBranch        *b_sSpillGood;   //!
   TBranch        *b_sDaqGood;   //!
   TBranch        *b_sND280OffFlag;   //!
   TBranch        *b_sTPCFlag;   //!
   TBranch        *b_sFGDFlag;   //!
   TBranch        *b_sECALFlag;   //!
   TBranch        *b_sDSECALFlag;   //!
   TBranch        *b_sP0DFlag;   //!
   TBranch        *b_sSMRDFlag;   //!
   TBranch        *b_sMAGNETFlag;   //!
   TBranch        *b_sEntry;   //!
   TBranch        *b_sNBunches;   //!
   TBranch        *b_sNValidBunches;   //!
   TBranch        *b_sBunchNbr;   //!
   TBranch        *b_sSpillWithTPC1Track;   //!
   TBranch        *b_sNTrueVertices;   //!
   TBranch        *b_std_TrueReacCode;   //!
   TBranch        *b_std_TrueVertexPos;   //!
   TBranch        *b_std_TrueNuEnergy;   //!
   TBranch        *b_std_TrueNuDir;   //!
   TBranch        *b_std_TrueNuPDG;   //!
   TBranch        *b_std_TrueEnuWeight;   //!
   TBranch        *b_std_TrueVertexBunch;   //!
   TBranch        *b_std_TrueLeptonMom;   //!
   TBranch        *b_std_TrueProtonMom;   //!
   TBranch        *b_std_TrueTargetMom;   //!
   TBranch        *b_std_TrueLeptonDir;   //!
   TBranch        *b_std_TrueProtonDir;   //!
   TBranch        *b_std_TrueTargetDir;   //!
   TBranch        *b_std_TrueTargetPDG;   //!
   TBranch        *b_std_TrueBaryonSize;   //!
   TBranch        *b_std_TrueQ2;   //!
   TBranch        *b_std_TrueVertexDet;   //!
   TBranch        *b_sNECALIso;   //!
   TBranch        *b_sNP0DIso;   //!
   TBranch        *b_sNSMRDIso;   //!
   TBranch        *b_sNFGDIso;   //!
   TBranch        *b_sNTPCIso;   //!
   TBranch        *b_sNFgdUnusedHits;   //!
   TBranch        *b_sP0DUnusedHits;   //!
   TBranch        *b_sNSMRDUnusedHits;   //!
   TBranch        *b_sNTracksWithFGDTotal;   //!
   TBranch        *b_sNTracksWithTPCTotal;   //!
   TBranch        *b_sNOtherTracksWithTPCTotal;   //!
   TBranch        *b_sNTracksWithTPC1Total;   //!
   TBranch        *b_sNTracksWithTPC2Total;   //!
   TBranch        *b_sNTracksWithTPC3Total;   //!
   TBranch        *b_sNVertices;   //!
   TBranch        *b_POTBunch;   //!
   TBranch        *b_Bunch;   //!
   TBranch        *b_BunchTimeBeam;   //!
   TBranch        *b_BunchWithMaxMomInSpill;   //!
   TBranch        *b_BunchTime;   //!
   TBranch        *b_Vertex;   //!
   TBranch        *b_NVertices;   //!
   TBranch        *b_stdB_vtxPrimaryIndex;   //!
   TBranch        *b_stdB_vtxPosition;   //!
   TBranch        *b_stdB_vtxTrueVertexPosition;   //!
   TBranch        *b_stdB_vtxVariance;   //!
   TBranch        *b_stdB_vtxQuality;   //!
   TBranch        *b_stdB_vtxNConstituents;   //!
   TBranch        *b_NTrueVerticesPerBunch;   //!
   TBranch        *b_NFgdTimeBin;   //!
   TBranch        *b_stdB_TimeBinNHits;   //!
   TBranch        *b_stdB_TimeBinMinTime;   //!
   TBranch        *b_stdB_TimeBinMaxTime;   //!
   TBranch        *b_stdB_TimeBinNFGD1UnusedHits;   //!
   TBranch        *b_stdB_TimeBinNFGD2UnusedHits;   //!
   TBranch        *b_NTracksGlobal;   //!
   TBranch        *b_NTracksWithTPCPos;   //!
   TBranch        *b_NTracksWithTPCNeg;   //!
   TBranch        *b_NTracksWithTPC1;   //!
   TBranch        *b_NTracksWithTPC2;   //!
   TBranch        *b_NTracksWithTPC3;   //!
   TBranch        *b_NTracksWithTPCOther;   //!
   TBranch        *b_NTracksWithDSECAL;   //!
   TBranch        *b_NTracksWithECAL;   //!
   TBranch        *b_stdB_ecalDetectors;   //!
   TBranch        *b_stdB_ecalDetUsed;   //!
   TBranch        *b_stdB_ecalNHits;   //!
   TBranch        *b_stdB_ecalNDOF;   //!
   TBranch        *b_stdB_ecalChi2;   //!
   TBranch        *b_stdB_ecalPositionStart;   //!
   TBranch        *b_stdB_ecalPositionEnd;   //!
   TBranch        *b_stdB_ecalDir;   //!
   TBranch        *b_stdB_ecalCosTheta;   //!
   TBranch        *b_stdB_ecalEDeposit;   //!
   TBranch        *b_stdB_ecalTrTruePDG;   //!
   TBranch        *b_stdB_ecalECALTrShVal;   //!
   TBranch        *b_stdB_ecalECALEMHadVal;   //!
   TBranch        *b_stdB_ecalECALEMEnergy;   //!
   TBranch        *b_stdB_ecalECALEDeposit;   //!
   TBranch        *b_stdB_ecalECALIsShowerLike;   //!
   TBranch        *b_stdB_ecalECALIsTrackLike;   //!
   TBranch        *b_NTracksWithSMRD;   //!
   TBranch        *b_stdB_smrdDetectors;   //!
   TBranch        *b_stdB_smrdDetUsed;   //!
   TBranch        *b_stdB_smrdNHits;   //!
   TBranch        *b_stdB_smrdNDOF;   //!
   TBranch        *b_stdB_smrdChi2;   //!
   TBranch        *b_stdB_smrdPositionStart;   //!
   TBranch        *b_stdB_smrdPositionEnd;   //!
   TBranch        *b_stdB_smrdDir;   //!
   TBranch        *b_stdB_smrdCosTheta;   //!
   TBranch        *b_stdB_smrdEDeposit;   //!
   TBranch        *b_stdB_smrdSMRDNHits;   //!
   TBranch        *b_stdB_smrdTrTruePDG;   //!
   TBranch        *b_NTracksWithP0D;   //!
   TBranch        *b_stdB_p0dDetectors;   //!
   TBranch        *b_stdB_p0dDetUsed;   //!
   TBranch        *b_stdB_p0dNHits;   //!
   TBranch        *b_stdB_p0dNDOF;   //!
   TBranch        *b_stdB_p0dChi2;   //!
   TBranch        *b_stdB_p0dPositionStart;   //!
   TBranch        *b_stdB_p0dPositionEnd;   //!
   TBranch        *b_stdB_p0dDir;   //!
   TBranch        *b_stdB_p0dCosTheta;   //!
   TBranch        *b_stdB_p0dEDeposit;   //!
   TBranch        *b_stdB_p0dP0DNHits;   //!
   TBranch        *b_stdB_p0dTrTruePDG;   //!
   TBranch        *b_NTracksWithTPC;   //!
   TBranch        *b_stdB_trDetectors;   //!
   TBranch        *b_stdB_trDetUsed;   //!
   TBranch        *b_stdB_trNHits;   //!
   TBranch        *b_stdB_trNDOF;   //!
   TBranch        *b_stdB_trChi2;   //!
   TBranch        *b_stdB_trPositionStart;   //!
   TBranch        *b_stdB_trPositionEnd;   //!
   TBranch        *b_stdB_trDir;   //!
   TBranch        *b_stdB_trMom;   //!
   TBranch        *b_stdB_trMomError;   //!
   TBranch        *b_stdB_trMomRelErr;   //!
   TBranch        *b_stdB_trCharge;   //!
   TBranch        *b_stdB_trCosTheta;   //!
   TBranch        *b_stdB_trEnuWeight;   //!
   TBranch        *b_stdB_trNFgds;   //!
   TBranch        *b_stdB_trNTracker;   //!
   TBranch        *b_stdB_trL;   //!
   TBranch        *b_stdB_trMomLocal;   //!
   TBranch        *b_stdB_trMomLocalTpc;   //!
   TBranch        *b_stdB_trChargeLocal;   //!
   TBranch        *b_stdB_trECALTrShVal;   //!
   TBranch        *b_stdB_trECALEMHadVal;   //!
   TBranch        *b_stdB_trECALEMEnergy;   //!
   TBranch        *b_stdB_trECALEDeposit;   //!
   TBranch        *b_stdB_trECALIsShowerLike;   //!
   TBranch        *b_stdB_trECALIsTrackLike;   //!
   TBranch        *b_stdB_trSmrdNHits;   //!
   TBranch        *b_stdB_trFgdDet;   //!
   TBranch        *b_stdB_trFgdNHits;   //!
   TBranch        *b_stdB_trFgdE;   //!
   TBranch        *b_stdB_trFgdX;   //!
   TBranch        *b_stdB_trFgdL;   //!
   TBranch        *b_stdB_trFgdPullmu;   //!
   TBranch        *b_stdB_trFgdPullpi;   //!
   TBranch        *b_stdB_trFgdPullp;   //!
   TBranch        *b_stdB_trFgdPullno;   //!
   TBranch        *b_stdB_trFgdPositionStart;   //!
   TBranch        *b_stdB_trFgdPositionEnd;   //!
   TBranch        *b_stdB_trTpcDet;   //!
   TBranch        *b_stdB_trTpcCharge;   //!
   TBranch        *b_stdB_trTpcMom;   //!
   TBranch        *b_stdB_trTpcDir;   //!
   TBranch        *b_stdB_trTpcMomRelErr;   //!
   TBranch        *b_stdB_trTpcNHits;   //!
   TBranch        *b_stdB_trTpcL;   //!
   TBranch        *b_stdB_trTpcTruePDG;   //!
   TBranch        *b_stdB_trCT;   //!
   TBranch        *b_stdB_trPullmu;   //!
   TBranch        *b_stdB_trPullele;   //!
   TBranch        *b_stdB_trPullp;   //!
   TBranch        *b_stdB_trPullpi;   //!
   TBranch        *b_stdB_trPullk;   //!
   TBranch        *b_stdB_trCTexpmu;   //!
   TBranch        *b_stdB_trCTexpele;   //!
   TBranch        *b_stdB_trCTexpp;   //!
   TBranch        *b_stdB_trCTexppi;   //!
   TBranch        *b_stdB_trCTexpk;   //!
   TBranch        *b_stdB_trCTerrmu;   //!
   TBranch        *b_stdB_trCTerrele;   //!
   TBranch        *b_stdB_trCTerrp;   //!
   TBranch        *b_stdB_trCTerrpi;   //!
   TBranch        *b_stdB_trCTerrk;   //!
   TBranch        *b_stdB_trTruePDG;   //!
   TBranch        *b_stdB_trTrueParentPDG;   //!
   TBranch        *b_stdB_trTrueGParentPDG;   //!
   TBranch        *b_stdB_trTruePur;   //!
   TBranch        *b_stdB_trTruePosition;   //!
   TBranch        *b_stdB_trTrueDirection;   //!
   TBranch        *b_stdB_trTrueMomentum;   //!
   TBranch        *b_stdB_trTrueCharge;   //!
   TBranch        *b_stdB_trTrueVertexIndex;   //!
   TBranch        *b_NTracksWithFGD;   //!
   TBranch        *b_stdB_fgdDetectors;   //!
   TBranch        *b_stdB_fgdDetUsed;   //!
   TBranch        *b_stdB_fgdNHits;   //!
   TBranch        *b_stdB_fgdNDOF;   //!
   TBranch        *b_stdB_fgdChi2;   //!
   TBranch        *b_stdB_fgdPositionStart;   //!
   TBranch        *b_stdB_fgdPositionEnd;   //!
   TBranch        *b_stdB_fgdDir;   //!
   TBranch        *b_stdB_fgdCosTheta;   //!
   TBranch        *b_stdB_fgdTrTruePDG;   //!
   TBranch        *b_stdB_fgdECALTrShVal;   //!
   TBranch        *b_stdB_fgdECALEMHadVal;   //!
   TBranch        *b_stdB_fgdECALEMEnergy;   //!
   TBranch        *b_stdB_fgdECALEDeposit;   //!
   TBranch        *b_stdB_fgdECALIsShowerLike;   //!
   TBranch        *b_stdB_fgdECALIsTrackLike;   //!
   TBranch        *b_stdB_fgdFgdNHits;   //!
   TBranch        *b_stdB_fgdE;   //!
   TBranch        *b_stdB_fgdX;   //!
   TBranch        *b_stdB_fgdPullmu;   //!
   TBranch        *b_stdB_fgdPullpi;   //!
   TBranch        *b_stdB_fgdPullp;   //!
   TBranch        *b_stdB_fgdPullno;   //!
   TBranch        *b_stdB_fgdSmrdNHits;   //!
   TBranch        *b_stdB_trENuRec_1Muon;   //!
   TBranch        *b_stdB_trEMuRec_1Muon;   //!
   TBranch        *b_stdB_trQ2_1Muon;   //!
   TBranch        *b_ENuRec_CCQE;   //!
   TBranch        *b_Q2_CCQE;   //!
   TBranch        *b_PQ2_CCQE;   //!
   TBranch        *b_NuInvMass_CCQE;   //!
   TBranch        *b_EProton_CCQE;   //!
   TBranch        *b_FinalCosTheta_CCQE;   //!
   TBranch        *b_FinalMass_CCQE;   //!
   TBranch        *b_FinalEnergy_CCQE;   //!
   TBranch        *b_FinalMom_CCQE;   //!
   TBranch        *b_EarliestTrackTime;   //!
   TBranch        *b_NClusters;   //!
   TBranch        *b_NClusterscut;   //!
   TBranch        *b_stdB_ClusterPos;   //!
   TBranch        *b_stdB_ClusterNHits;   //!
   TBranch        *b_stdB_ClusterRMS;   //!
   TBranch        *b_stdB_ClusterCharge;   //!
   TBranch        *b_stdB_ClusterDistVertex;   //!
   TBranch        *b_stdB_ClusterTimeDistVertex;   //!
   TBranch        *b_g10aB_trCTCorr;   //!
   TBranch        *b_g10aB_trPullCorrmu;   //!
   TBranch        *b_g10aB_trPullCorrele;   //!
   TBranch        *b_g10aB_trPullCorrp;   //!
   TBranch        *b_g10aB_trPullCorrpi;   //!
   TBranch        *b_g10aB_trPullCorrk;   //!
   TBranch        *b_g10aB_trProbmu;   //!
   TBranch        *b_g10aB_trProbele;   //!
   TBranch        *b_g10aB_trProbp;   //!
   TBranch        *b_g10aB_trProbpi;   //!
   TBranch        *b_g10aB_trProbk;   //!
   TBranch        *b_g10aB_trCTexpCorrmu;   //!
   TBranch        *b_g10aB_trCTexpCorrele;   //!
   TBranch        *b_g10aB_trCTexpCorrp;   //!
   TBranch        *b_g10aB_trCTexpCorrpi;   //!
   TBranch        *b_g10aB_trCTexpCorrk;   //!
   TBranch        *b_g10aB_NTPCTracksCut;   //!
   TBranch        *b_g10aB_NTPCTracksPosCut;   //!
   TBranch        *b_g10aB_NTPCTracksNegCut;   //!
   TBranch        *b_g10aB_ENuRec;   //!
   TBranch        *b_g10aB_EMuRec;   //!
   TBranch        *b_g10aB_PQ2;   //!
   TBranch        *b_g10aB_MuQ2;   //!
   TBranch        *b_g10aB_DeltaThetaP;   //!
   TBranch        *b_g10aB_EProton;   //!
   TBranch        *b_g10aB_FinalCosTheta;   //!
   TBranch        *b_g10aB_FinalMass;   //!
   TBranch        *b_g10aB_FinalEnergy;   //!
   TBranch        *b_g10aB_FinalMom;   //!

   NumuFlatTreeDefinition(TTree *tree=0);
   virtual ~NumuFlatTreeDefinition();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef NumuFlatTreeDefinition_cxx
NumuFlatTreeDefinition::NumuFlatTreeDefinition(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../prod4C/NeutMCRun2.root");
      if (!f) {
         f = new TFile("../prod4C/NeutMCRun2.root");
      }
      tree = (TTree*)gDirectory->Get("numutree");

   }
   Init(tree);
}

NumuFlatTreeDefinition::~NumuFlatTreeDefinition()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t NumuFlatTreeDefinition::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t NumuFlatTreeDefinition::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void NumuFlatTreeDefinition::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("std_TotalGoodSpillPOT", &std_TotalGoodSpillPOT, &b_sPOTTotalGoodSpill);
   fChain->SetBranchAddress("std_TotalGoodSpillDaqPOT", &std_TotalGoodSpillDaqPOT, &b_sPOTTotalGoodSpillDaq);
   fChain->SetBranchAddress("std_TotalGoodSpillAndDQPOT", &std_TotalGoodSpillAndDQPOT, &b_sPOTTotalGoodSpillAndDQ);
   fChain->SetBranchAddress("std_TotalPOT", &std_TotalPOT, &b_sPOTTotal);
   fChain->SetBranchAddress("std_TotalRemoveDaqPOT", &std_TotalRemoveDaqPOT, &b_sPOTTotalRemoveDaq);
   fChain->SetBranchAddress("std_SpillPOT", &std_SpillPOT, &b_sPOTSpill);
   fChain->SetBranchAddress("std_TrueSpillInfo", &std_TrueSpillInfo, &b_sTrueSpillInfo);
   fChain->SetBranchAddress("std_Evt", &std_Evt, &b_sEvt);
   fChain->SetBranchAddress("std_MRRun", &std_MRRun, &b_sMRRun);
   fChain->SetBranchAddress("std_Run", &std_Run, &b_sRun);
   fChain->SetBranchAddress("std_Subrun", &std_Subrun, &b_sSubrun);
   fChain->SetBranchAddress("std_FileRun", &std_FileRun, &b_sFileRun);
   fChain->SetBranchAddress("std_FileSubrun", &std_FileSubrun, &b_sFileSubrun);
   fChain->SetBranchAddress("std_Spill", &std_Spill, &b_sSpill);
   fChain->SetBranchAddress("std_GoodSpill", &std_GoodSpill, &b_sSpillGood);
   fChain->SetBranchAddress("std_GoodDaq", &std_GoodDaq, &b_sDaqGood);
   fChain->SetBranchAddress("std_ND280OffFlag", &std_ND280OffFlag, &b_sND280OffFlag);
   fChain->SetBranchAddress("std_TPCFlag ", &std_TPCFlag , &b_sTPCFlag);
   fChain->SetBranchAddress("std_FGDFlag ", &std_FGDFlag , &b_sFGDFlag);
   fChain->SetBranchAddress("std_ECALFlag", &std_ECALFlag, &b_sECALFlag);
   fChain->SetBranchAddress("std_DSECALFlag", &std_DSECALFlag, &b_sDSECALFlag);
   fChain->SetBranchAddress("std_P0DFlag", &std_P0DFlag, &b_sP0DFlag);
   fChain->SetBranchAddress("std_SMRDFlag", &std_SMRDFlag, &b_sSMRDFlag);
   fChain->SetBranchAddress("std_MAGNETFlag", &std_MAGNETFlag, &b_sMAGNETFlag);
   fChain->SetBranchAddress("std_Entry", &std_Entry, &b_sEntry);
   fChain->SetBranchAddress("std_NBunches", &std_NBunches, &b_sNBunches);
   fChain->SetBranchAddress("std_NValidBunches", &std_NValidBunches, &b_sNValidBunches);
   fChain->SetBranchAddress("std_BunchNbr", &std_BunchNbr, &b_sBunchNbr);
   fChain->SetBranchAddress("std_SpillWithTPC1Track", &std_SpillWithTPC1Track, &b_sSpillWithTPC1Track);
   fChain->SetBranchAddress("std_NTrueVertices", &std_NTrueVertices, &b_sNTrueVertices);
   fChain->SetBranchAddress("std_TrueReacCode", std_TrueReacCode, &b_std_TrueReacCode);
   fChain->SetBranchAddress("std_TrueVertexPos", std_TrueVertexPos, &b_std_TrueVertexPos);
   fChain->SetBranchAddress("std_TrueNuEnergy", std_TrueNuEnergy, &b_std_TrueNuEnergy);
   fChain->SetBranchAddress("std_TrueNuDir", std_TrueNuDir, &b_std_TrueNuDir);
   fChain->SetBranchAddress("std_TrueNuPDG", std_TrueNuPDG, &b_std_TrueNuPDG);
   fChain->SetBranchAddress("std_TrueEnuWeight", std_TrueEnuWeight, &b_std_TrueEnuWeight);
   fChain->SetBranchAddress("std_TrueVertexBunch", std_TrueVertexBunch, &b_std_TrueVertexBunch);
   fChain->SetBranchAddress("std_TrueLeptonMom", std_TrueLeptonMom, &b_std_TrueLeptonMom);
   fChain->SetBranchAddress("std_TrueProtonMom", std_TrueProtonMom, &b_std_TrueProtonMom);
   fChain->SetBranchAddress("std_TrueTargetMom", std_TrueTargetMom, &b_std_TrueTargetMom);
   fChain->SetBranchAddress("std_TrueLeptonDir", std_TrueLeptonDir, &b_std_TrueLeptonDir);
   fChain->SetBranchAddress("std_TrueProtonDir", std_TrueProtonDir, &b_std_TrueProtonDir);
   fChain->SetBranchAddress("std_TrueTargetDir", std_TrueTargetDir, &b_std_TrueTargetDir);
   fChain->SetBranchAddress("std_TrueTargetPDG", std_TrueTargetPDG, &b_std_TrueTargetPDG);
   fChain->SetBranchAddress("std_TrueBaryonSize", std_TrueBaryonSize, &b_std_TrueBaryonSize);
   fChain->SetBranchAddress("std_TrueQ2", std_TrueQ2, &b_std_TrueQ2);
   fChain->SetBranchAddress("std_TrueVertexDet", std_TrueVertexDet, &b_std_TrueVertexDet);
   fChain->SetBranchAddress("std_NECALTracks", &std_NECALTracks, &b_sNECALIso);
   fChain->SetBranchAddress("std_NP0DTracks", &std_NP0DTracks, &b_sNP0DIso);
   fChain->SetBranchAddress("std_NSMRDTracks", &std_NSMRDTracks, &b_sNSMRDIso);
   fChain->SetBranchAddress("std_NFGDTracks", &std_NFGDTracks, &b_sNFGDIso);
   fChain->SetBranchAddress("std_NTPCTracks", &std_NTPCTracks, &b_sNTPCIso);
   fChain->SetBranchAddress("std_NFGDUnusedHits", std_NFGDUnusedHits, &b_sNFgdUnusedHits);
   fChain->SetBranchAddress("std_NP0DUnusedHits", &std_NP0DUnusedHits, &b_sP0DUnusedHits);
   fChain->SetBranchAddress("std_NSMRDUnusedHits", std_NSMRDUnusedHits, &b_sNSMRDUnusedHits);
   fChain->SetBranchAddress("std_NTracksWithFGDTotal", &std_NTracksWithFGDTotal, &b_sNTracksWithFGDTotal);
   fChain->SetBranchAddress("std_NTracksWithTPCTotal", &std_NTracksWithTPCTotal, &b_sNTracksWithTPCTotal);
   fChain->SetBranchAddress("std_NOtherTracksWithTPCTotal", &std_NOtherTracksWithTPCTotal, &b_sNOtherTracksWithTPCTotal);
   fChain->SetBranchAddress("std_NTpc1TracksTotal", &std_NTpc1TracksTotal, &b_sNTracksWithTPC1Total);
   fChain->SetBranchAddress("std_NTpc2TracksTotal", &std_NTpc2TracksTotal, &b_sNTracksWithTPC2Total);
   fChain->SetBranchAddress("std_NTpc3TracksTotal", &std_NTpc3TracksTotal, &b_sNTracksWithTPC3Total);
   fChain->SetBranchAddress("std_NVertices", &std_NVertices, &b_sNVertices);
   fChain->SetBranchAddress("stdB_POTBunch", &stdB_POTBunch, &b_POTBunch);
   fChain->SetBranchAddress("stdB_Bunch", &stdB_Bunch, &b_Bunch);
   fChain->SetBranchAddress("stdB_BeamBunchTime", &stdB_BeamBunchTime, &b_BunchTimeBeam);
   fChain->SetBranchAddress("stdB_BunchWithMaxMomInSpill", &stdB_BunchWithMaxMomInSpill, &b_BunchWithMaxMomInSpill);
   fChain->SetBranchAddress("stdB_TBunch", &stdB_TBunch, &b_BunchTime);
   fChain->SetBranchAddress("stdB_Vertex", stdB_Vertex, &b_Vertex);
   fChain->SetBranchAddress("stdB_NVertices", &stdB_NVertices, &b_NVertices);
   fChain->SetBranchAddress("stdB_vtxPrimaryIndex", stdB_vtxPrimaryIndex, &b_stdB_vtxPrimaryIndex);
   fChain->SetBranchAddress("stdB_vtxPosition", stdB_vtxPosition, &b_stdB_vtxPosition);
   fChain->SetBranchAddress("stdB_vtxTrueVertexPosition", stdB_vtxTrueVertexPosition, &b_stdB_vtxTrueVertexPosition);
   fChain->SetBranchAddress("stdB_vtxVariance", stdB_vtxVariance, &b_stdB_vtxVariance);
   fChain->SetBranchAddress("stdB_vtxQuality", stdB_vtxQuality, &b_stdB_vtxQuality);
   fChain->SetBranchAddress("stdB_vtxNConstituents", stdB_vtxNConstituents, &b_stdB_vtxNConstituents);
   fChain->SetBranchAddress("stdB_NTrueVerticesPerBunch", &stdB_NTrueVerticesPerBunch, &b_NTrueVerticesPerBunch);
   fChain->SetBranchAddress("stdB_NFgdTimeBin", &stdB_NFgdTimeBin, &b_NFgdTimeBin);
   fChain->SetBranchAddress("stdB_TimeBinNHits", stdB_TimeBinNHits, &b_stdB_TimeBinNHits);
   fChain->SetBranchAddress("stdB_TimeBinMinTime", stdB_TimeBinMinTime, &b_stdB_TimeBinMinTime);
   fChain->SetBranchAddress("stdB_TimeBinMaxTime", stdB_TimeBinMaxTime, &b_stdB_TimeBinMaxTime);
   fChain->SetBranchAddress("stdB_TimeBinNFGD1UnusedHits", stdB_TimeBinNFGD1UnusedHits, &b_stdB_TimeBinNFGD1UnusedHits);
   fChain->SetBranchAddress("stdB_TimeBinNFGD2UnusedHits", stdB_TimeBinNFGD2UnusedHits, &b_stdB_TimeBinNFGD2UnusedHits);
   fChain->SetBranchAddress("stdB_NTracksGlobal", &stdB_NTracksGlobal, &b_NTracksGlobal);
   fChain->SetBranchAddress("stdB_NTracksWithTPCPos", &stdB_NTracksWithTPCPos, &b_NTracksWithTPCPos);
   fChain->SetBranchAddress("stdB_NTracksWithTPCNeg", &stdB_NTracksWithTPCNeg, &b_NTracksWithTPCNeg);
   fChain->SetBranchAddress("stdB_NTracksWithTPC1", &stdB_NTracksWithTPC1, &b_NTracksWithTPC1);
   fChain->SetBranchAddress("stdB_NTracksWithTPC2", &stdB_NTracksWithTPC2, &b_NTracksWithTPC2);
   fChain->SetBranchAddress("stdB_NTracksWithTPC3", &stdB_NTracksWithTPC3, &b_NTracksWithTPC3);
   fChain->SetBranchAddress("stdB_NTracksWithTPCOther", &stdB_NTracksWithTPCOther, &b_NTracksWithTPCOther);
   fChain->SetBranchAddress("stdB_NTracksWithDSECAL", &stdB_NTracksWithDSECAL, &b_NTracksWithDSECAL);
   fChain->SetBranchAddress("stdB_NTracksWithECAL", &stdB_NTracksWithECAL, &b_NTracksWithECAL);
   fChain->SetBranchAddress("stdB_ecalDetectors", stdB_ecalDetectors, &b_stdB_ecalDetectors);
   fChain->SetBranchAddress("stdB_ecalDetUsed", stdB_ecalDetUsed, &b_stdB_ecalDetUsed);
   fChain->SetBranchAddress("stdB_ecalNHits", stdB_ecalNHits, &b_stdB_ecalNHits);
   fChain->SetBranchAddress("stdB_ecalNDOF", stdB_ecalNDOF, &b_stdB_ecalNDOF);
   fChain->SetBranchAddress("stdB_ecalChi2", stdB_ecalChi2, &b_stdB_ecalChi2);
   fChain->SetBranchAddress("stdB_ecalPositionStart", stdB_ecalPositionStart, &b_stdB_ecalPositionStart);
   fChain->SetBranchAddress("stdB_ecalPositionEnd", stdB_ecalPositionEnd, &b_stdB_ecalPositionEnd);
   fChain->SetBranchAddress("stdB_ecalDir", stdB_ecalDir, &b_stdB_ecalDir);
   fChain->SetBranchAddress("stdB_ecalCosTheta", stdB_ecalCosTheta, &b_stdB_ecalCosTheta);
   fChain->SetBranchAddress("stdB_ecalEDeposit", stdB_ecalEDeposit, &b_stdB_ecalEDeposit);
   fChain->SetBranchAddress("stdB_ecalTrTruePDG", stdB_ecalTrTruePDG, &b_stdB_ecalTrTruePDG);
   fChain->SetBranchAddress("stdB_ecalECALTrShVal", stdB_ecalECALTrShVal, &b_stdB_ecalECALTrShVal);
   fChain->SetBranchAddress("stdB_ecalECALEMHadVal", stdB_ecalECALEMHadVal, &b_stdB_ecalECALEMHadVal);
   fChain->SetBranchAddress("stdB_ecalECALEMEnergy", stdB_ecalECALEMEnergy, &b_stdB_ecalECALEMEnergy);
   fChain->SetBranchAddress("stdB_ecalECALEDeposit", stdB_ecalECALEDeposit, &b_stdB_ecalECALEDeposit);
   fChain->SetBranchAddress("stdB_ecalECALIsShowerLike", stdB_ecalECALIsShowerLike, &b_stdB_ecalECALIsShowerLike);
   fChain->SetBranchAddress("stdB_ecalECALIsTrackLike", stdB_ecalECALIsTrackLike, &b_stdB_ecalECALIsTrackLike);
   fChain->SetBranchAddress("stdB_NTracksWithSMRD", &stdB_NTracksWithSMRD, &b_NTracksWithSMRD);
   fChain->SetBranchAddress("stdB_smrdDetectors", stdB_smrdDetectors, &b_stdB_smrdDetectors);
   fChain->SetBranchAddress("stdB_smrdDetUsed", stdB_smrdDetUsed, &b_stdB_smrdDetUsed);
   fChain->SetBranchAddress("stdB_smrdNHits", stdB_smrdNHits, &b_stdB_smrdNHits);
   fChain->SetBranchAddress("stdB_smrdNDOF", stdB_smrdNDOF, &b_stdB_smrdNDOF);
   fChain->SetBranchAddress("stdB_smrdChi2", stdB_smrdChi2, &b_stdB_smrdChi2);
   fChain->SetBranchAddress("stdB_smrdPositionStart", stdB_smrdPositionStart, &b_stdB_smrdPositionStart);
   fChain->SetBranchAddress("stdB_smrdPositionEnd", stdB_smrdPositionEnd, &b_stdB_smrdPositionEnd);
   fChain->SetBranchAddress("stdB_smrdDir", stdB_smrdDir, &b_stdB_smrdDir);
   fChain->SetBranchAddress("stdB_smrdCosTheta", stdB_smrdCosTheta, &b_stdB_smrdCosTheta);
   fChain->SetBranchAddress("stdB_smrdEDeposit", stdB_smrdEDeposit, &b_stdB_smrdEDeposit);
   fChain->SetBranchAddress("stdB_smrdSMRDNHits", stdB_smrdSMRDNHits, &b_stdB_smrdSMRDNHits);
   fChain->SetBranchAddress("stdB_smrdTrTruePDG", stdB_smrdTrTruePDG, &b_stdB_smrdTrTruePDG);
   fChain->SetBranchAddress("stdB_NTracksWithP0D", &stdB_NTracksWithP0D, &b_NTracksWithP0D);
   fChain->SetBranchAddress("stdB_p0dDetectors", stdB_p0dDetectors, &b_stdB_p0dDetectors);
   fChain->SetBranchAddress("stdB_p0dDetUsed", stdB_p0dDetUsed, &b_stdB_p0dDetUsed);
   fChain->SetBranchAddress("stdB_p0dNHits", stdB_p0dNHits, &b_stdB_p0dNHits);
   fChain->SetBranchAddress("stdB_p0dNDOF", stdB_p0dNDOF, &b_stdB_p0dNDOF);
   fChain->SetBranchAddress("stdB_p0dChi2", stdB_p0dChi2, &b_stdB_p0dChi2);
   fChain->SetBranchAddress("stdB_p0dPositionStart", stdB_p0dPositionStart, &b_stdB_p0dPositionStart);
   fChain->SetBranchAddress("stdB_p0dPositionEnd", stdB_p0dPositionEnd, &b_stdB_p0dPositionEnd);
   fChain->SetBranchAddress("stdB_p0dDir", stdB_p0dDir, &b_stdB_p0dDir);
   fChain->SetBranchAddress("stdB_p0dCosTheta", stdB_p0dCosTheta, &b_stdB_p0dCosTheta);
   fChain->SetBranchAddress("stdB_p0dEDeposit", stdB_p0dEDeposit, &b_stdB_p0dEDeposit);
   fChain->SetBranchAddress("stdB_p0dP0DNHits", stdB_p0dP0DNHits, &b_stdB_p0dP0DNHits);
   fChain->SetBranchAddress("stdB_p0dTrTruePDG", stdB_p0dTrTruePDG, &b_stdB_p0dTrTruePDG);
   fChain->SetBranchAddress("stdB_NTracksWithTPC", &stdB_NTracksWithTPC, &b_NTracksWithTPC);
   fChain->SetBranchAddress("stdB_trDetectors", stdB_trDetectors, &b_stdB_trDetectors);
   fChain->SetBranchAddress("stdB_trDetUsed", stdB_trDetUsed, &b_stdB_trDetUsed);
   fChain->SetBranchAddress("stdB_trNHits", stdB_trNHits, &b_stdB_trNHits);
   fChain->SetBranchAddress("stdB_trNDOF", stdB_trNDOF, &b_stdB_trNDOF);
   fChain->SetBranchAddress("stdB_trChi2", stdB_trChi2, &b_stdB_trChi2);
   fChain->SetBranchAddress("stdB_trPositionStart", stdB_trPositionStart, &b_stdB_trPositionStart);
   fChain->SetBranchAddress("stdB_trPositionEnd", stdB_trPositionEnd, &b_stdB_trPositionEnd);
   fChain->SetBranchAddress("stdB_trDir", stdB_trDir, &b_stdB_trDir);
   fChain->SetBranchAddress("stdB_trMom", stdB_trMom, &b_stdB_trMom);
   fChain->SetBranchAddress("stdB_trMomError", stdB_trMomError, &b_stdB_trMomError);
   fChain->SetBranchAddress("stdB_trMomRelErr", stdB_trMomRelErr, &b_stdB_trMomRelErr);
   fChain->SetBranchAddress("stdB_trCharge", stdB_trCharge, &b_stdB_trCharge);
   fChain->SetBranchAddress("stdB_trCosTheta", stdB_trCosTheta, &b_stdB_trCosTheta);
   fChain->SetBranchAddress("stdB_trEnuWeight", stdB_trEnuWeight, &b_stdB_trEnuWeight);
   fChain->SetBranchAddress("stdB_trNFgds", stdB_trNFgds, &b_stdB_trNFgds);
   fChain->SetBranchAddress("stdB_trNTracker", stdB_trNTracker, &b_stdB_trNTracker);
   fChain->SetBranchAddress("stdB_trL", stdB_trL, &b_stdB_trL);
   fChain->SetBranchAddress("stdB_trMomLocal", stdB_trMomLocal, &b_stdB_trMomLocal);
   fChain->SetBranchAddress("stdB_trMomLocalTpc", stdB_trMomLocalTpc, &b_stdB_trMomLocalTpc);
   fChain->SetBranchAddress("stdB_trChargeLocal", stdB_trChargeLocal, &b_stdB_trChargeLocal);
   fChain->SetBranchAddress("stdB_trECALTrShVal", stdB_trECALTrShVal, &b_stdB_trECALTrShVal);
   fChain->SetBranchAddress("stdB_trECALEMHadVal", stdB_trECALEMHadVal, &b_stdB_trECALEMHadVal);
   fChain->SetBranchAddress("stdB_trECALEMEnergy", stdB_trECALEMEnergy, &b_stdB_trECALEMEnergy);
   fChain->SetBranchAddress("stdB_trECALEDeposit", stdB_trECALEDeposit, &b_stdB_trECALEDeposit);
   fChain->SetBranchAddress("stdB_trECALIsShowerLike", stdB_trECALIsShowerLike, &b_stdB_trECALIsShowerLike);
   fChain->SetBranchAddress("stdB_trECALIsTrackLike", stdB_trECALIsTrackLike, &b_stdB_trECALIsTrackLike);
   fChain->SetBranchAddress("stdB_trSmrdNHits", stdB_trSmrdNHits, &b_stdB_trSmrdNHits);
   fChain->SetBranchAddress("stdB_trFgdDet", stdB_trFgdDet, &b_stdB_trFgdDet);
   fChain->SetBranchAddress("stdB_trFgdNHits", stdB_trFgdNHits, &b_stdB_trFgdNHits);
   fChain->SetBranchAddress("stdB_trFgdE", stdB_trFgdE, &b_stdB_trFgdE);
   fChain->SetBranchAddress("stdB_trFgdX", stdB_trFgdX, &b_stdB_trFgdX);
   fChain->SetBranchAddress("stdB_trFgdL", stdB_trFgdL, &b_stdB_trFgdL);
   fChain->SetBranchAddress("stdB_trFgdPullmu", stdB_trFgdPullmu, &b_stdB_trFgdPullmu);
   fChain->SetBranchAddress("stdB_trFgdPullpi", stdB_trFgdPullpi, &b_stdB_trFgdPullpi);
   fChain->SetBranchAddress("stdB_trFgdPullp", stdB_trFgdPullp, &b_stdB_trFgdPullp);
   fChain->SetBranchAddress("stdB_trFgdPullno", stdB_trFgdPullno, &b_stdB_trFgdPullno);
   fChain->SetBranchAddress("stdB_trFgdPositionStart", stdB_trFgdPositionStart, &b_stdB_trFgdPositionStart);
   fChain->SetBranchAddress("stdB_trFgdPositionEnd", stdB_trFgdPositionEnd, &b_stdB_trFgdPositionEnd);
   fChain->SetBranchAddress("stdB_trTpcDet", stdB_trTpcDet, &b_stdB_trTpcDet);
   fChain->SetBranchAddress("stdB_trTpcCharge", stdB_trTpcCharge, &b_stdB_trTpcCharge);
   fChain->SetBranchAddress("stdB_trTpcMom", stdB_trTpcMom, &b_stdB_trTpcMom);
   fChain->SetBranchAddress("stdB_trTpcDir", stdB_trTpcDir, &b_stdB_trTpcDir);
   fChain->SetBranchAddress("stdB_trTpcMomRelErr", stdB_trTpcMomRelErr, &b_stdB_trTpcMomRelErr);
   fChain->SetBranchAddress("stdB_trTpcNHits", stdB_trTpcNHits, &b_stdB_trTpcNHits);
   fChain->SetBranchAddress("stdB_trTpcL", stdB_trTpcL, &b_stdB_trTpcL);
   fChain->SetBranchAddress("stdB_trTpcTruePDG", stdB_trTpcTruePDG, &b_stdB_trTpcTruePDG);
   fChain->SetBranchAddress("stdB_trCT", stdB_trCT, &b_stdB_trCT);
   fChain->SetBranchAddress("stdB_trPullmu", stdB_trPullmu, &b_stdB_trPullmu);
   fChain->SetBranchAddress("stdB_trPullele", stdB_trPullele, &b_stdB_trPullele);
   fChain->SetBranchAddress("stdB_trPullp", stdB_trPullp, &b_stdB_trPullp);
   fChain->SetBranchAddress("stdB_trPullpi", stdB_trPullpi, &b_stdB_trPullpi);
   fChain->SetBranchAddress("stdB_trPullk", stdB_trPullk, &b_stdB_trPullk);
   fChain->SetBranchAddress("stdB_trCTexpmu", stdB_trCTexpmu, &b_stdB_trCTexpmu);
   fChain->SetBranchAddress("stdB_trCTexpele", stdB_trCTexpele, &b_stdB_trCTexpele);
   fChain->SetBranchAddress("stdB_trCTexpp", stdB_trCTexpp, &b_stdB_trCTexpp);
   fChain->SetBranchAddress("stdB_trCTexppi", stdB_trCTexppi, &b_stdB_trCTexppi);
   fChain->SetBranchAddress("stdB_trCTexpk", stdB_trCTexpk, &b_stdB_trCTexpk);
   fChain->SetBranchAddress("stdB_trCTerrmu", stdB_trCTerrmu, &b_stdB_trCTerrmu);
   fChain->SetBranchAddress("stdB_trCTerrele", stdB_trCTerrele, &b_stdB_trCTerrele);
   fChain->SetBranchAddress("stdB_trCTerrp", stdB_trCTerrp, &b_stdB_trCTerrp);
   fChain->SetBranchAddress("stdB_trCTerrpi", stdB_trCTerrpi, &b_stdB_trCTerrpi);
   fChain->SetBranchAddress("stdB_trCTerrk", stdB_trCTerrk, &b_stdB_trCTerrk);
   fChain->SetBranchAddress("stdB_trTruePDG", stdB_trTruePDG, &b_stdB_trTruePDG);
   fChain->SetBranchAddress("stdB_trTrueParentPDG", stdB_trTrueParentPDG, &b_stdB_trTrueParentPDG);
   fChain->SetBranchAddress("stdB_trTrueGParentPDG", stdB_trTrueGParentPDG, &b_stdB_trTrueGParentPDG);
   fChain->SetBranchAddress("stdB_trTruePur", stdB_trTruePur, &b_stdB_trTruePur);
   fChain->SetBranchAddress("stdB_trTruePosition", stdB_trTruePosition, &b_stdB_trTruePosition);
   fChain->SetBranchAddress("stdB_trTrueDirection", stdB_trTrueDirection, &b_stdB_trTrueDirection);
   fChain->SetBranchAddress("stdB_trTrueMomentum", stdB_trTrueMomentum, &b_stdB_trTrueMomentum);
   fChain->SetBranchAddress("stdB_trTrueCharge", stdB_trTrueCharge, &b_stdB_trTrueCharge);
   fChain->SetBranchAddress("stdB_trTrueVertexIndex", stdB_trTrueVertexIndex, &b_stdB_trTrueVertexIndex);
   fChain->SetBranchAddress("stdB_NTracksWithFGD", &stdB_NTracksWithFGD, &b_NTracksWithFGD);
   fChain->SetBranchAddress("stdB_fgdDetectors", stdB_fgdDetectors, &b_stdB_fgdDetectors);
   fChain->SetBranchAddress("stdB_fgdDetUsed", stdB_fgdDetUsed, &b_stdB_fgdDetUsed);
   fChain->SetBranchAddress("stdB_fgdNHits", stdB_fgdNHits, &b_stdB_fgdNHits);
   fChain->SetBranchAddress("stdB_fgdNDOF", stdB_fgdNDOF, &b_stdB_fgdNDOF);
   fChain->SetBranchAddress("stdB_fgdChi2", stdB_fgdChi2, &b_stdB_fgdChi2);
   fChain->SetBranchAddress("stdB_fgdPositionStart", stdB_fgdPositionStart, &b_stdB_fgdPositionStart);
   fChain->SetBranchAddress("stdB_fgdPositionEnd", stdB_fgdPositionEnd, &b_stdB_fgdPositionEnd);
   fChain->SetBranchAddress("stdB_fgdDir", stdB_fgdDir, &b_stdB_fgdDir);
   fChain->SetBranchAddress("stdB_fgdCosTheta", stdB_fgdCosTheta, &b_stdB_fgdCosTheta);
   fChain->SetBranchAddress("stdB_fgdTrTruePDG", stdB_fgdTrTruePDG, &b_stdB_fgdTrTruePDG);
   fChain->SetBranchAddress("stdB_fgdECALTrShVal", stdB_fgdECALTrShVal, &b_stdB_fgdECALTrShVal);
   fChain->SetBranchAddress("stdB_fgdECALEMHadVal", stdB_fgdECALEMHadVal, &b_stdB_fgdECALEMHadVal);
   fChain->SetBranchAddress("stdB_fgdECALEMEnergy", stdB_fgdECALEMEnergy, &b_stdB_fgdECALEMEnergy);
   fChain->SetBranchAddress("stdB_fgdECALEDeposit", stdB_fgdECALEDeposit, &b_stdB_fgdECALEDeposit);
   fChain->SetBranchAddress("stdB_fgdECALIsShowerLike", stdB_fgdECALIsShowerLike, &b_stdB_fgdECALIsShowerLike);
   fChain->SetBranchAddress("stdB_fgdECALIsTrackLike", stdB_fgdECALIsTrackLike, &b_stdB_fgdECALIsTrackLike);
   fChain->SetBranchAddress("stdB_fgdFgdNHits", stdB_fgdFgdNHits, &b_stdB_fgdFgdNHits);
   fChain->SetBranchAddress("stdB_fgdE", stdB_fgdE, &b_stdB_fgdE);
   fChain->SetBranchAddress("stdB_fgdX", stdB_fgdX, &b_stdB_fgdX);
   fChain->SetBranchAddress("stdB_fgdPullmu", stdB_fgdPullmu, &b_stdB_fgdPullmu);
   fChain->SetBranchAddress("stdB_fgdPullpi", stdB_fgdPullpi, &b_stdB_fgdPullpi);
   fChain->SetBranchAddress("stdB_fgdPullp", stdB_fgdPullp, &b_stdB_fgdPullp);
   fChain->SetBranchAddress("stdB_fgdPullno", stdB_fgdPullno, &b_stdB_fgdPullno);
   fChain->SetBranchAddress("stdB_fgdSmrdNHits", stdB_fgdSmrdNHits, &b_stdB_fgdSmrdNHits);
   fChain->SetBranchAddress("stdB_trENuRec_1Muon", stdB_trENuRec_1Muon, &b_stdB_trENuRec_1Muon);
   fChain->SetBranchAddress("stdB_trEMuRec_1Muon", stdB_trEMuRec_1Muon, &b_stdB_trEMuRec_1Muon);
   fChain->SetBranchAddress("stdB_trQ2_1Muon", stdB_trQ2_1Muon, &b_stdB_trQ2_1Muon);
   fChain->SetBranchAddress("stdB_ENuRec_CCQE", &stdB_ENuRec_CCQE, &b_ENuRec_CCQE);
   fChain->SetBranchAddress("stdB_Q2_CCQE", &stdB_Q2_CCQE, &b_Q2_CCQE);
   fChain->SetBranchAddress("stdB_PQ2_CCQE", &stdB_PQ2_CCQE, &b_PQ2_CCQE);
   fChain->SetBranchAddress("stdB_NuInvMass_CCQE", &stdB_NuInvMass_CCQE, &b_NuInvMass_CCQE);
   fChain->SetBranchAddress("stdB_EProton_CCQE", &stdB_EProton_CCQE, &b_EProton_CCQE);
   fChain->SetBranchAddress("stdB_FinalCosTheta_CCQE", &stdB_FinalCosTheta_CCQE, &b_FinalCosTheta_CCQE);
   fChain->SetBranchAddress("stdB_FinalMass_CCQE", &stdB_FinalMass_CCQE, &b_FinalMass_CCQE);
   fChain->SetBranchAddress("stdB_FinalEnergy_CCQE", &stdB_FinalEnergy_CCQE, &b_FinalEnergy_CCQE);
   fChain->SetBranchAddress("stdB_FinalMom_CCQE", &stdB_FinalMom_CCQE, &b_FinalMom_CCQE);
   fChain->SetBranchAddress("stdB_EarliestTrackTime", &stdB_EarliestTrackTime, &b_EarliestTrackTime);
   fChain->SetBranchAddress("stdB_NClusters", &stdB_NClusters, &b_NClusters);
   fChain->SetBranchAddress("stdB_NClusterscut", stdB_NClusterscut, &b_NClusterscut);
   fChain->SetBranchAddress("stdB_ClusterPos", stdB_ClusterPos, &b_stdB_ClusterPos);
   fChain->SetBranchAddress("stdB_ClusterNHits", stdB_ClusterNHits, &b_stdB_ClusterNHits);
   fChain->SetBranchAddress("stdB_ClusterRMS", stdB_ClusterRMS, &b_stdB_ClusterRMS);
   fChain->SetBranchAddress("stdB_ClusterCharge", stdB_ClusterCharge, &b_stdB_ClusterCharge);
   fChain->SetBranchAddress("stdB_ClusterDistVertex", stdB_ClusterDistVertex, &b_stdB_ClusterDistVertex);
   fChain->SetBranchAddress("stdB_ClusterTimeDistVertex", stdB_ClusterTimeDistVertex, &b_stdB_ClusterTimeDistVertex);
   fChain->SetBranchAddress("g10aB_trCTCorr", g10aB_trCTCorr, &b_g10aB_trCTCorr);
   fChain->SetBranchAddress("g10aB_trPullCorrmu", g10aB_trPullCorrmu, &b_g10aB_trPullCorrmu);
   fChain->SetBranchAddress("g10aB_trPullCorrele", g10aB_trPullCorrele, &b_g10aB_trPullCorrele);
   fChain->SetBranchAddress("g10aB_trPullCorrp", g10aB_trPullCorrp, &b_g10aB_trPullCorrp);
   fChain->SetBranchAddress("g10aB_trPullCorrpi", g10aB_trPullCorrpi, &b_g10aB_trPullCorrpi);
   fChain->SetBranchAddress("g10aB_trPullCorrk", g10aB_trPullCorrk, &b_g10aB_trPullCorrk);
   fChain->SetBranchAddress("g10aB_trProbmu", g10aB_trProbmu, &b_g10aB_trProbmu);
   fChain->SetBranchAddress("g10aB_trProbele", g10aB_trProbele, &b_g10aB_trProbele);
   fChain->SetBranchAddress("g10aB_trProbp", g10aB_trProbp, &b_g10aB_trProbp);
   fChain->SetBranchAddress("g10aB_trProbpi", g10aB_trProbpi, &b_g10aB_trProbpi);
   fChain->SetBranchAddress("g10aB_trProbk", g10aB_trProbk, &b_g10aB_trProbk);
   fChain->SetBranchAddress("g10aB_trCTexpCorrmu", g10aB_trCTexpCorrmu, &b_g10aB_trCTexpCorrmu);
   fChain->SetBranchAddress("g10aB_trCTexpCorrele", g10aB_trCTexpCorrele, &b_g10aB_trCTexpCorrele);
   fChain->SetBranchAddress("g10aB_trCTexpCorrp", g10aB_trCTexpCorrp, &b_g10aB_trCTexpCorrp);
   fChain->SetBranchAddress("g10aB_trCTexpCorrpi", g10aB_trCTexpCorrpi, &b_g10aB_trCTexpCorrpi);
   fChain->SetBranchAddress("g10aB_trCTexpCorrk", g10aB_trCTexpCorrk, &b_g10aB_trCTexpCorrk);
   fChain->SetBranchAddress("g10aB_NTPCTracksCut", g10aB_NTPCTracksCut, &b_g10aB_NTPCTracksCut);
   fChain->SetBranchAddress("g10aB_NTPCTracksPosCut", g10aB_NTPCTracksPosCut, &b_g10aB_NTPCTracksPosCut);
   fChain->SetBranchAddress("g10aB_NTPCTracksNegCut", g10aB_NTPCTracksNegCut, &b_g10aB_NTPCTracksNegCut);
   fChain->SetBranchAddress("g10aB_ENuRec", g10aB_ENuRec, &b_g10aB_ENuRec);
   fChain->SetBranchAddress("g10aB_EMuRec", g10aB_EMuRec, &b_g10aB_EMuRec);
   fChain->SetBranchAddress("g10aB_PQ2", g10aB_PQ2, &b_g10aB_PQ2);
   fChain->SetBranchAddress("g10aB_MuQ2", g10aB_MuQ2, &b_g10aB_MuQ2);
   fChain->SetBranchAddress("g10aB_DeltaThetaP", g10aB_DeltaThetaP, &b_g10aB_DeltaThetaP);
   fChain->SetBranchAddress("g10aB_EProton", &g10aB_EProton, &b_g10aB_EProton);
   fChain->SetBranchAddress("g10aB_FinalCosTheta", &g10aB_FinalCosTheta, &b_g10aB_FinalCosTheta);
   fChain->SetBranchAddress("g10aB_FinalMass", &g10aB_FinalMass, &b_g10aB_FinalMass);
   fChain->SetBranchAddress("g10aB_FinalEnergy", &g10aB_FinalEnergy, &b_g10aB_FinalEnergy);
   fChain->SetBranchAddress("g10aB_FinalMom", &g10aB_FinalMom, &b_g10aB_FinalMom);
   Notify();
}

Bool_t NumuFlatTreeDefinition::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void NumuFlatTreeDefinition::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t NumuFlatTreeDefinition::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef NumuFlatTreeDefinition_cxx
