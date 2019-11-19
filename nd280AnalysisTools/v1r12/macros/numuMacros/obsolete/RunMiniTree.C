#include "iostream"
#include "MiniTree.h"
#include "vector"
#include <map>

using namespace std;

//========================================================================
//
//   RunMiniTree.C
//   ROOT macro to fill a flat TTree
//   using input from oaAnalysis TTree
//
//   Run command: root -lq 'RunMiniTree(evLow, evHigh, mc, silent, infile)'
//
//   where evLow is the first event to run (default 0)
//   and evHigh the last (default 1e6)
//   mc is 0 for data (default 1 meaning MC)
//   silent is 0 for verbose output (default 1 meaning silent)
//   infile is a string with the name of a list file with input files
//
//   output file is <infile basename>_<evLow>_<evHigh>.root
//
//========================================================================

//Sets all class global variables to default values
//*************************************************************
void ResetSpillVariables(){
  //*************************************************************

  _trueSpillInfo = 0;
  _evt = -1;
  _run = 0;
  _filerun = -1;
  _filesubrun = -1;
  _entry = -1;
  _nbunches = 0;
  _nvalidbunches = 0;
  _bunchnbr = -1;
  _SpillWithTPC1Track = -1; 
  _nTrueVertices = 0;
  _necaltracks = 0;
  _np0dtracks = 0;
  _nsmrdtracks = 0;
  _nfgdtracks = 0;
  _ntpctracks = 0;

  for(int j = 0; j < NMAXTRUEVERTICES; j ++) {
    _reacCode[j] = 0;
    for(int k = 0; k < 4; k ++) _trueVertexPos[j][k] = 0;
    _trueNuEnergy[j] = 0;
    for(int k = 0; k < 3; k ++){
      _trueNuDir[j][k] = 0;
      _trueLeptonDir[j][k] = 0;
      _trueProtonDir[j][k] = 0;
      _trueTargetDir[j][k] = 0;
    }
    _trueNuPDG[j] = 0;
    _trueEnuWeight[j] = 0;
    _trueVertexBunch[j] = 0;
    _trueLeptonMom[j] = 0;
    _trueProtonMom[j] = 0;
    _trueTargetMom[j] = 0;
    _trueTargetPDG[j] = 0;
    _trueBaryonSize[j] = 0;
    _trueQ2[j] = 0;  
    _trueVertexDet[j] = 0;

    for(int t = 0; t < NMAXTRAJ; t ++) _truePrimaryID[j][t] = 0;    

    _trueLeptonEnergy[j] = 0;
  }

  _nFGDUnused[0] = 0;
  _nFGDUnused[1] = 0;

  for(int i = 0; i < NANACUTS; i ++){
    _ntracks_total[i] = 0;
    for(int k = 0; k < 3; k ++) _nTpctracks_total[k][i] = 0;
  }

  _nPrimTraj = 0;
  for(int k = 0; k < 3; k ++) _dirV[k] = 0;
}


//Sets all class global bunch variables to default values
//*************************************************************
void ResetBunchVariables(){
  //*************************************************************

  _BunchWithMaxMomInSpill = -1;
  _tbunch = -1;
  _bunch = -1;
  _newbunch = -1;
  _ntracks_bunch = 0;
  _nTrueVerticesPerBunch = 0;
  _totaltracks = 0;
  _nuInvariantMass = 0;
  _ep = 0;
  _cosThetaF = 0;
  _mf = 0;
  _mf2 = 0;
  _ef = 0;
  _pf = 0;
  _earliestTrackTime = 0;
  _nclusters = 0;  

  for(int i = 0; i < 4; i ++){
    for(int k = 0; k < 4; k ++){
      _vertex[i][k] = 0;
    }
  }

  for(int i = 0; i < NMAXTRACKS; i ++){
    for(int k = 0; k < 4; k ++){
      _pos[i][k] = 0;
      _posEnd[i][k] = 0;
      _fgdpos[i][k] = 0;
      _fgdposEnd[i][k] = 0;
    }
    for(int k = 0; k < 3; k ++){
      _dir[i][k] = 0;
      _trTpc[i][k] = 0;
      _trCT[i][k] = 0;
      _trCTCorr[i][k] = 0;
      _tpcMom[i][k] = 0;
      _tpcMomRelError[i][k] = 0;
      _tpcNHits[i][k] = 0;
      _fgddir[i][k] = 0;

      for(int j = 0; j < NHYPO; j ++){
	_trPull[j][i][k] = 0;
	_trCTexp[j][i][k] = 0;
	_trCTerr[j][i][k] = 0;
      }
    }
    
    for(int j = 0; j < NHYPO; j ++) _trProb[j][i] = 0;

    _det[i] = 0;
    _NHits[i] = 0;
    _chi2[i] = 0;
    _mom[i] = 0;
    _momError[i] = 0;
    _charge[i] = 0;
    _cosTheta[i] = 0;
    _trShVal[i] = 0;
    _enuWeight[i] = 0;
    _nFgds[i] = 0;
    _fgddet[i] = 0;
    _fgdNHits[i] = 0;
    _fgdchi2[i] = 0;
    _eNuRec[i] = 0;
    _deltaThetaP[i] = 0;
    _emu[i] = 0;
    _pQ2[i] = 0;
    _muQ2[i] = 0;
    _deltaThetaP[0] = 0;
    _truePDG[i] = 0;
    _trueParentPDG[i] = 0;
    _trueGParentPDG[i] = 0;
    _truePur[i] = 0;
    for(int k = 0; k < 4; k ++) _truePos[i][k] = 0;
    for(int k = 0; k < 3; k ++) _trueDir[i][k] = 0;
    _trueMom[i] = 0;
    _trueCharge[i] = 0;
    _trueVertexIndex[i] = -1;
  }

  for(int i = 0; i < NANACUTS; i ++){
    for(int k = 0; k < 3; k ++){
      _ntpctracks_bunch[k][i] = 0;
      _nclusterscut[k][i] = 0;
    }
  }

  for(int i = 0; i < NMAXCLUSTERS; i ++){
    for(int k = 0; k < 4; k ++){
      _clusterPos[i][k] = 0;
      _clusterRMS[i][k] = 0;
    }
    for(int k = 0; k < 3; k ++){
      _dist[i][k] = 0;
      _tdist[i][k] = 0;
    }
    _clusterNHits[i] = 0;
    _clusterCharge[i] = 0;
  }
}


//Branch definitions for the variables in the mini tree
//*************************************************************
void DefineTree(){

  //----------- filled for each spill -----------
  _mini->Branch("trueSpillInfo",   &_trueSpillInfo,   "trueSpillInfo/I");
  _mini->Branch("Evt",        &_evt,      "evt/I");
  _mini->Branch("Run",        &_run,      "run/I");
  _mini->Branch("FileRun",        &_filerun,      "filerun/I");
  _mini->Branch("FileSubRun",        &_filesubrun,      "filesubrun/I");
  _mini->Branch("Entry",      &_entry,    "entry/I");
  _mini->Branch("NBunches",   &_nbunches, "nbunches/I");
  _mini->Branch("NValidBunches", &_nvalidbunches, "nValidBunches/I");
  _mini->Branch("BunchNbr",   &_bunchnbr, "bunchnbr/I");
  _mini->Branch("SpillWithTPC1Track",   &_SpillWithTPC1Track,   "SpillWithTPC1Track/I");
  _mini->Branch("TrueVertices",     &_nTrueVertices,  "nTrueVertices/I");
  _mini->Branch("ReacCode",       _reacCode,       "reacCode[nTrueVertices]/I");
  _mini->Branch("TrueVertexPos",  _trueVertexPos,  "trueVertexPos[nTrueVertices][4]/D");
  _mini->Branch("TrueNuEnergy",   _trueNuEnergy,   "trueNuEnergy[nTrueVertices]/D");
  _mini->Branch("TrueNuDir",      _trueNuDir,      "trueNuDir[nTrueVertices][3]/D");
  _mini->Branch("TrueNuPDG",      _trueNuPDG,      "trueNuPDG[nTrueVertices]/I");
  _mini->Branch("TrueEnuWeight",  _trueEnuWeight,  "trueEnuWeight[nTrueVertices]/D");
  _mini->Branch("TrueBunch",      _trueVertexBunch,"trueVertexBunch[nTrueVertices]/I");
  _mini->Branch("TrueLeptonMom",      _trueLeptonMom,     "trueLeptonMom[nTrueVertices]/D");
  _mini->Branch("TrueProtonMom",      _trueProtonMom,     "trueProtonMom[nTrueVertices]/D");
  _mini->Branch("TrueTargetMom",      _trueTargetMom,     "trueTargetMom[nTrueVertices]/D");
  _mini->Branch("TrueLeptonDir",      _trueLeptonDir,     "trueLeptonDir[nTrueVertices][3]/D");
  _mini->Branch("TrueProtonDir",      _trueProtonDir,     "trueProtonDir[nTrueVertices][3]/D");
  _mini->Branch("TrueTargetDir",      _trueTargetDir,     "trueTargetDir[nTrueVertices][3]/D");
  _mini->Branch("TrueTargetPDG",      _trueTargetPDG,     "trueTargetPDG[nTrueVertices]/D");
  _mini->Branch("TrueBaryonSize",     _trueBaryonSize,    "trueBaryonSize[nTrueVertices]/I");
  _mini->Branch("TrueQ2",             _trueQ2,            "trueQ2[nTrueVertices]/D");
  _mini->Branch("TrueVertexDet",      _trueVertexDet,      "trueVertexDet[nTrueVertices]/I");
  _mini->Branch("NECAL",      &_necaltracks,  "necal/I");
  _mini->Branch("NP0D",       &_np0dtracks,   "np0d/I");
  _mini->Branch("NSMRD",      &_nsmrdtracks,  "nsmrd/I");
  _mini->Branch("NFGD",       &_nfgdtracks,   "nfgd/I");
  _mini->Branch("NTPC",       &_ntpctracks,   "ntpc/I");
  _mini->Branch("NFGDUnused", _nFGDUnused,    "nFgdUnused[2]/I");
  _mini->Branch("NtracksTotal",    _ntracks_total,     "ntracksTotal[5]/I");
  _mini->Branch("NTpc1tracksTotal",    _nTpctracks_total[0],     "ntpc1tracksTotal[5]/I");
  _mini->Branch("NTpc2tracksTotal",    _nTpctracks_total[1],     "ntpc2tracksTotal[5]/I");
  _mini->Branch("NTpc3tracksTotal",    _nTpctracks_total[2],     "ntpc3tracksTotal[5]/I");

  //----------- filled for each bunch -----------
  _mini->Branch("BunchWithMaxMomInSpill",   &_BunchWithMaxMomInSpill,   "BunchWithMaxMomInSpill/I");
  _mini->Branch("TBunch",     &_tbunch,    "tbunch/D");
  _mini->Branch("Bunch",      &_bunch,    "bunch/I");
  _mini->Branch("NewBunch",      &_newbunch,    "newbunch/I");
  _mini->Branch("NTracksBunch",      &_ntracks_bunch,    "nTracksBunch/I");
  _mini->Branch("Vertex",     _vertex,      "vertex[4][4]/D");
  _mini->Branch("nTrueVerticesPerBunch",   &_nTrueVerticesPerBunch,   "nTrueVerticesPerBunch/I");
  _mini->Branch("totaltracks", &_totaltracks, "totaltracks/I");
  _mini->Branch("Ntracks",    _ntracks,     "ntracks[5]/I");
  _mini->Branch("Npos",       _npostracks,  "npos[5]/I");
  _mini->Branch("Nneg",       _nnegtracks,  "nneg[5]/I");
  _mini->Branch("NTpc1tracks",    _ntpctracks_bunch[0],     "ntpc1tracks[5]/I");
  _mini->Branch("NTpc2tracks",    _ntpctracks_bunch[1],     "ntpc2tracks[5]/I");
  _mini->Branch("NTpc3tracks",    _ntpctracks_bunch[2],     "ntpc3tracks[5]/I");
  _mini->Branch("Detectors",  _det,   "trDet[nTracksBunch]/I");
  _mini->Branch("DetUsed",    _detUsed,  "trDetUsed[nTracksBunch][9]/I");
  _mini->Branch("NHits",      _NHits, "trHits[nTracksBunch]/I");
  _mini->Branch("FitChiInfo", _chi2,  "trChi2[nTracksBunch]/D");
  _mini->Branch("Position",   _pos,   "trPos[nTracksBunch][4]/D");
  _mini->Branch("PositionEnd",   _posEnd,   "trPosEnd[nTracksBunch][4]/D");
  _mini->Branch("Direction",  _dir,   "trDir[nTracksBunch][3]/D");
  _mini->Branch("Momentum",   _mom,   "trMom[nTracksBunch]/D");
  _mini->Branch("MomentumError",  _momError,   "trMomErr[nTracksBunch]/D");
  _mini->Branch("MomRelErr",  _momRelError,  "trMomRelErr[nTracksBunch]/D");
  _mini->Branch("Charge",     _charge,   "trCharge[nTracksBunch]/I");
  _mini->Branch("CosTheta",   _cosTheta, "trCosTheta[nTracksBunch]/D");
  _mini->Branch("TrShVal",    _trShVal,  "trTrShVal[nTracksBunch]/D");
  _mini->Branch("EnuWeight",  _enuWeight,  "trEnuWeight[nTracksBunch]/D");
  _mini->Branch("NFgds",      _nFgds,    "NFgds[nTracksBunch]/I");
  _mini->Branch("TrTpc",     _trTpc,    "tpc[nTracksBunch][3]/I");
  _mini->Branch("TrCT",      _trCT,     "ct[nTracksBunch][3]/D");
  _mini->Branch("TrCTCorr",  _trCTCorr, "ctc[nTracksBunch][3]/D");
  _mini->Branch("TrPullmu",  _trPull[0], "pullmu[nTracksBunch][3]/D");
  _mini->Branch("TrPullele",  _trPull[1], "pullele[nTracksBunch][3]/D");
  _mini->Branch("TrPullp",  _trPull[2], "pullp[nTracksBunch][3]/D");
  _mini->Branch("TrPullpi",  _trPull[3], "pullpi[nTracksBunch][3]/D");
  _mini->Branch("TrPullk",  _trPull[4], "pullk[nTracksBunch][3]/D");
  _mini->Branch("TrPullCorrmu",  _trPullCorr[0], "pullcmu[nTracksBunch][3]/D");
  _mini->Branch("TrPullCorrele",  _trPullCorr[1], "pullcele[nTracksBunch][3]/D");
  _mini->Branch("TrPullCorrp",  _trPullCorr[2], "pullcp[nTracksBunch][3]/D");
  _mini->Branch("TrPullCorrpi",  _trPullCorr[3], "pullcpi[nTracksBunch][3]/D");
  _mini->Branch("TrPullCorrk",  _trPullCorr[4], "pullck[nTracksBunch][3]/D");
  _mini->Branch("TrProbmu",  _trProb[0], "probmu[nTracksBunch]/D");
  _mini->Branch("TrProbele",  _trProb[1], "probele[nTracksBunch]/D");
  _mini->Branch("TrProbp",  _trProb[2], "probp[nTracksBunch]/D");
  _mini->Branch("TrProbpi",  _trProb[3], "probpi[nTracksBunch]/D");
  _mini->Branch("TrProbk",  _trProb[4], "probk[nTracksBunch]/D");
  _mini->Branch("TrCTexpmu",  _trCTexp[0], "ctexpmu[nTracksBunch][3]/D");
  _mini->Branch("TrCTexpele",  _trCTexp[1], "ctexpele[nTracksBunch][3]/D");
  _mini->Branch("TrCTexpp",  _trCTexp[2], "ctexpp[nTracksBunch][3]/D");
  _mini->Branch("TrCTexppi",  _trCTexp[3], "ctexppi[nTracksBunch][3]/D");
  _mini->Branch("TrCTexpk",  _trCTexp[4], "ctexpk[nTracksBunch][3]/D");    
  _mini->Branch("TrCTerrmu",  _trCTerr[0], "cterrmu[nTracksBunch][3]/D");
  _mini->Branch("TrCTerrele",  _trCTerr[1], "cterrele[nTracksBunch][3]/D");
  _mini->Branch("TrCTerrp",  _trCTerr[2], "cterrp[nTracksBunch][3]/D");
  _mini->Branch("TrCTerrpi",  _trCTerr[3], "cterrpi[nTracksBunch][3]/D");
  _mini->Branch("TrCTerrk",  _trCTerr[4], "cterrk[nTracksBunch][3]/D");
  _mini->Branch("TpcCharge",     _tpcCharge,      "trTpcCharge[nTracksBunch][3]/D");
  _mini->Branch("TpcMom",        _tpcMom,         "trTpcMom[nTracksBunch][3]/D");
  _mini->Branch("TpcMomRelErr",  _tpcMomRelError, "trTpcMomRelErr[nTracksBunch][3]/D");
  _mini->Branch("TpcNHits",      _tpcNHits,       "trTpcNHits[nTracksBunch][3]/I");
  _mini->Branch("NFgdTracksBunch",    &_nfgdtracks_bunch, "nFgdtracksBunch/I");    
  _mini->Branch("FgdDetectors",  _fgddet,   "fgdtrDet[nFgdtracksBunch]/I");
  _mini->Branch("FgdNHits",      _fgdNHits, "fgdtrHits[nFgdtracksBunch]/I");
  _mini->Branch("FgdFitChiInfo", _fgdchi2,  "fgdtrChi2[nFgdtracksBunch]/D");
  _mini->Branch("FgdPosition",   _fgdpos,   "fgdtrPos[nFgdtracksBunch][4]/D");
  _mini->Branch("FgdPositionEnd",   _fgdposEnd,   "fgdtrPosEnd[nFgdtracksBunch][4]/D");
  _mini->Branch("FgdDirection",  _fgddir,   "fgdtrDir[nFgdtracksBunch][3]/D");
  _mini->Branch("ENuRec",     _eNuRec,          "enu[nTracksBunch]/D");
  _mini->Branch("DeltaThetaP",_deltaThetaP,     "dthp[nTracksBunch]/D");
  _mini->Branch("NuInvMass",  &_nuInvariantMass, "mnu/D");
  _mini->Branch("EMu",        _emu,     "trEMu[nTracksBunch]/D");
  _mini->Branch("EP",         &_ep,      "ep/D");
  _mini->Branch("CosThetaF",  &_cosThetaF,     "cosThetaF/D");
  _mini->Branch("MF",         &_mf,      "mf/D");
  _mini->Branch("EF",         &_ef,      "ef/D");
  _mini->Branch("PF",         &_pf,      "pf/D");
  _mini->Branch("PQ2",         _pQ2,      "pQ2[nTracksBunch]/D");
  _mini->Branch("MuQ2",        _muQ2,      "muQ2[nTracksBunch]/D");
  _mini->Branch("EarliestTrackTime",  &_earliestTrackTime,         "earliestTrackTime/D");
  _mini->Branch("NClusters",          &_nclusters,    "nclusters/I");
  _mini->Branch("NClustersCut",       _nclusterscut,    "nclusterscut[3][5]/I");
  _mini->Branch("ClusterPos", _clusterPos,       "clusterPos[nclusters][4]/D");
  _mini->Branch("ClusterNHits", _clusterNHits,   "clusterNHits[nclusters]/I");
  _mini->Branch("ClusterRMS",   _clusterRMS,     "clusterRMS[nclusters][4]/D");
  _mini->Branch("ClusterCharge", _clusterCharge, "clusterCharge[nclusters]/D");
  _mini->Branch("Dist",          _dist,         "dist[nclusters][3]/D");
  _mini->Branch("TDist",         _tdist,         "tdist[nclusters][3]/D");
  _mini->Branch("TruePDG",        _truePDG,        "trTruePdg[nTracksBunch]/I");
  _mini->Branch("TrueParentPDG",  _trueParentPDG,  "trTrueParentPdg[nTracksBunch]/I");
  _mini->Branch("TrueGParentPDG", _trueGParentPDG, "trTrueGParentPdg[nTracksBunch]/I");
  _mini->Branch("TruePur",        _truePur,        "trTruePur[nTracksBunch]/D");
  _mini->Branch("TruePosition",   _truePos,        "trTruePos[nTracksBunch][4]/D");
  _mini->Branch("TrueDirection",  _trueDir,        "trTrueDir[nTracksBunch][3]/D");
  _mini->Branch("TrueMomentum",   _trueMom,        "trTrueMom[nTracksBunch]/D");
  _mini->Branch("TrueCharge",     _trueCharge,     "trTrueCharge[nTracksBunch]/I");
  _mini->Branch("TrueVertexIndex",_trueVertexIndex,"trTrueVertexIndex[nTracksBunch]/I");
}


//Fills information about delayed (Michel) FGD clusters
//**********************************************************
void FillDelayedClusters(TClonesArray *DelayedClusters, int NDelayedClusters, double EarliestTrackTime, TLorentzVector vtx1, TLorentzVector vtx2){
  
  //Loop over delayed clusters
  for(int j = 0; j < NDelayedClusters; j ++){            

    ND::TGlobalReconModule::TFgdCluster *cluster = (ND::TGlobalReconModule::TFgdCluster*)(*DelayedClusters)[j];

    if(!cluster) continue;
    if(_nclusters == NMAXCLUSTERS) break;
      
    //Take only the final clusters with unused hits
    _clusterNHits[_nclusters] = cluster->NHits;
    
    //Fill 4-position
    for(int i = 0; i < 4; i ++){
      _clusterRMS[_nclusters][i] = cluster->PosRMS[i];
      _clusterPos[_nclusters][i] = cluster->Position[i];
    }
    
    _clusterCharge[_nclusters] = cluster->TotalCharge;
    _tdist[2][_nclusters] = cluster->Position.T() - EarliestTrackTime;
    _tdist[0][_nclusters] = cluster->Position.T() - vtx1.T();
    _tdist[1][_nclusters] = cluster->Position.T() - vtx2.T();
    
    TLorentzVector dp1 = cluster->Position - vtx1;
    TLorentzVector dp2 = cluster->Position - vtx2;
    
    //Get the distance between the cluster and the vertex
    if(cluster->Position.X() == 0){
      _dist[0][_nclusters] = sqrt(dp1.Y()*dp1.Y() + dp1.Z()*dp1.Z());
      _dist[1][_nclusters] = sqrt(dp2.Y()*dp2.Y() + dp2.Z()*dp2.Z());
    }
    else if(cluster->Position.Y() == 0){
      _dist[0][_nclusters] = sqrt(dp1.X()*dp1.X() + dp1.Z()*dp1.Z());
      _dist[1][_nclusters] = sqrt(dp2.X()*dp2.X() + dp2.Z()*dp2.Z());
    }
    else{ 
      _dist[0][_nclusters] = sqrt(dp1.X()*dp1.X() + dp1.Y()*dp1.Y() + dp1.Z()*dp1.Z());
      _dist[1][_nclusters] = sqrt(dp2.X()*dp2.X() + dp2.Y()*dp2.Y() + dp2.Z()*dp2.Z());
    }
    
    if(!_silent){
      cout << "_dist1 = " << _dist[0][_nclusters] << endl;
      cout << "_dist2 = " << _dist[1][_nclusters] << endl;
      cout << "_tdist1 = " << _tdist[0][_nclusters] << endl;
      cout << "_tdist2 = " << _tdist[1][_nclusters] << endl;
    }
    
    for(int imcut1 = 0; imcut1 < NANACUTS; imcut1 ++){
      if(!_silent){
	cout << "_nclusters = " << _nclusters << endl;
	cout << "_dist[0][_nclusters] = " << _dist[0][_nclusters] << endl;
	cout << "_dist[1][_nclusters] = " << _dist[1][_nclusters] << endl;
	cout << "_michelCut[imcut1] = "<< _michelCut[imcut1] << endl;
      }
      if(_dist[0][_nclusters] < _michelCut[imcut1]){
	_nclusterscut[0][imcut1] ++;
      }
    }
    
    for(int imcut2 = 0; imcut2 < NANACUTS; imcut2 ++){
      if(!_silent){
	cout << "_nclusters = " << _nclusters << endl;
	cout << "_dist[0][_nclusters] = " << _dist[0][_nclusters] << endl;
	cout << "_dist[1][_nclusters] = " << _dist[1][_nclusters] << endl;
	cout << "_michelCut[imcut2] = "<< _michelCut[imcut2] << endl;
      }
      if(_dist[1][_nclusters] < _michelCut[imcut2]){
	_nclusterscut[1][imcut2] ++;
      }
    }      
    
    if(!_silent){
      for(int i = 0; i < NANACUTS; i ++){
	cout << "_nclusterscut[0][" << i << "] = " << _nclusterscut[0][i] << endl;
	cout << "_nclusterscut[1][" << i << "] = " << _nclusterscut[1][i] << endl;
      }
    }
    
    _nclusters++;
  }//End loop over clusters
}


//Method that returns the beam run number
//**********************************************************
int GetBeamRun(int run){

  int brun = 0;

  if(run < 4300) brun = 0;
  else if(run < 4600) brun = 1;
  else if(run < 4900) brun = 2;
  else brun = 3;
  
  return brun;
}


//Method that applies PID corrections
//**********************************************************
void ApplyPIDCorrections(int run){

  double CTexp_corr_data = 0.9060;
  double CT_corr_MC = 1.0025;
  double smearing_sigma = 20.55;

  double CT_corr, CTexp_corr;

  double mean_data_all       = -0.040;//TpcNHits>40&&CTc>0&&TpcMom<2000&&TpcMom>200, negative, fit in [-2,2]
  double sigma_data_all      = 1.105;
  double mean_data_all_elec  = -0.562;//TpcNHits>20&&CTc<900&&TpcMom<80, all charges, fit in [-1.5,0.5]-not used
  double sigma_data_all_elec = 1.054;//TpcNHits>20&&CTc<900&&TpcMom<80, all charges, fit in [-1.5,0.5]-not used

  double mean_MC_all       = 0.00;//as it should be - no good reason to have this
  double sigma_MC_all      = 1.08;//this is still contested - will leave it in
  double mean_MC_all_elec  = -0.214;//no need to have this - it's small and very uncertain
  double sigma_MC_all_elec = 0.98;//we inflate/smear them, anyway!
  double ElePull_correction_MC_muons = 1.525;//how to move the true mu in MC to match data

  //Symmetric fits
  double mean_data[NRUNS][3] = {{ 0.346, 0.067, 0.264},
				{ 0.285, 0.108, 0.256},
				{ 0.363,-0.318,-0.026},
				{ 0.036,-0.569,-0.296}};

  double sigma_data[NRUNS][3] = {{ 1.024, 1.029, 1.030},
				 { 1.084, 1.035, 1.047},
				 { 1.095, 1.062, 1.038},
				 { 1.041, 1.030, 1.037}};

  double mean  = mean_MC_all;
  double sigma = sigma_data_all;

  int irun = 0; 

  if(!_isMC) irun = GetBeamRun(run);
  
  if(_isMC){
    CT_corr = CT_corr_MC;
    CTexp_corr = 1;
  } 
  else{
    CT_corr = 1;
    CTexp_corr = CTexp_corr_data;
  }

  //Loop over the tracks
  for(int itr = 0; itr < _ntracks[_cutNbr]; itr ++){

    if(itr == NMAXTRACKS) break;

    //Loop over the TPCs    
    for(int itpc = 0; itpc < 3; itpc ++){

      if(_trCT[itr][itpc] > 0 && _trCT[itr][itpc] < 5e4){
	_trCTCorr[itr][itpc] = _trCT[itr][itpc]*CT_corr;
	if(_isMC) _trCTCorr[itr][itpc] += fRandomGenerator.Gaus(0, smearing_sigma);
      }
      else _trCTCorr[itr][itpc] = -999.0;
      
      //Loop over hypotheses
      for(int h = 0; h < NHYPO; h ++){

	//MC
	if(_isMC){
	  if(h != 1){
	    mean = mean_MC_all;
	    sigma = sigma_MC_all;
	  }
	  // for electrons
	  else{
	    if(abs(_truePDG[0]) == 13){
	      mean = mean_MC_all_elec - ElePull_correction_MC_muons;
	    }
	    else{
	      mean = mean_MC_all_elec;
	      sigma = sigma_MC_all_elec;
	    }
	  }
	}

	//Data
	else{
	  mean = mean_data[irun][itpc];
	  sigma = sigma_data[irun][itpc];
	  if(h != 1) CTexp_corr = CTexp_corr_data;
	  else CTexp_corr = CTexp_corr_data*0.959;
	}	

	if(_trCTCorr[itr][itpc] > 0 && _trCTCorr[itr][itpc] < 5e4){
	  _trCTexp[h][itr][itpc] *= CTexp_corr;
	  _trPullCorr[h][itr][itpc] = ((_trCTCorr[itr][itpc] - _trCTexp[h][itr][itpc])/(_trCTerr[h][itr][itpc]) - mean)/sigma;
	}
	else _trPullCorr[h][itr][itpc] = -999.0;

      }//End loop over hypotheses
    }//End loop over TPCs
  }//End loop over tracks
}


//Computes PID probabilities
//**********************************************************
void ComputePIDProbabilities(){

  double mean  = 0;
  double sigma = 1;

  vector<double> sum;

  //Loop over the tracks
  for(int itr = 0; itr < _ntracks[_cutNbr]; itr ++){

    sum.push_back(0);

    if(itr == NMAXTRACKS) break;

    //Loop over hypotheses
    for(int h = 0; h < NHYPO; h ++){

      _trProb[h][itr] = 1;
    
      for(int itpc = 0; itpc < 3; itpc ++){
	
	if(_trPullCorr[h][itr][itpc] != 0 && fabs(_trPullCorr[h][itr][itpc]) < 100){
	  _trProb[h][itr] *= exp(-0.5*pow((_trPullCorr[h][itr][itpc] - mean)/sigma, 2));
	}
      }      
      sum[itr] += _trProb[h][itr];
    }//End loop over hypotheses
  }//End loop over tracks  

  //Normalize  
  for(int itr = 0; itr < _ntracks[_cutNbr]; itr ++){
    for(int h = 0; h < NHYPO; h ++){
      if(sum[itr] != 0) _trProb[h][itr] /= sum[itr];
    }    
  }
}


//Finds the start position
//**********************************************************
TLorentzVector GetStartOfTrack(TLorentzVector back,  
			       TLorentzVector front, 
			       bool isForward){

  TLorentzVector thisPos;
  
  double frontz = front.Z();
  double backz = back.Z();
  if((isForward && (frontz < backz)) ||
     (!isForward && (frontz > backz))){
    thisPos = front;
  }
  else{
    thisPos = back;
  }

  return thisPos;
}


//
//**********************************************************
void FillPosDirAtVertex(int itrk, int det, 
			TLorentzVector backPos,  
			TLorentzVector frontPos, 
			TVector3 backDir,  
			TVector3 frontDir, 
			bool isForward){

  double oa_angle = 2.5*TMath::DegToRad();
  TVector3 nuDir(0, sin(oa_angle), cos(oa_angle));

  double frontzi = frontPos.Z();
  double backzi = backPos.Z();
  // Forwards going track
  if((isForward && (frontzi < backzi)) || (!isForward && (frontzi > backzi))){
    for(int c = 0; c < 4; c ++){
      if(det == 4 || det == 5){
	_fgdpos[itrk][c] = frontPos[c];
	_fgdposEnd[itrk][c] = backPos[c];
      }
      else{
	_pos[itrk][c] = frontPos[c];
	_posEnd[itrk][c] = backPos[c];
      }
    }
    for(int c = 0; c < 3; c ++){
      if(det == 4 || det == 5) _fgddir[itrk][c] = frontDir[c];
      else _dir[itrk][c] = frontDir[c];
    }

    if(det == 0){
      _dirV[itrk] = frontDir;    
      _cosTheta[itrk] = frontDir*nuDir;
    }    
  }
  // Backwards going track
  else{
    for(int c = 0; c < 4; c ++){
      if(det == 4 || det == 5){
	_fgdpos[itrk][c] = backPos[c];
	_fgdposEnd[itrk][c] = frontPos[c];
      }
      else{
	_pos[itrk][c] = backPos[c];
	_posEnd[itrk][c] = frontPos[c];
      }
    }
    for(int c = 0; c < 3; c ++){
      if(det == 4 || det == 5) _fgddir[itrk][c] = backDir[c];
      else _dir[itrk][c] = backDir[c];
    }

    if(det==0){      
      _dirV[itrk] = backDir;    
      _cosTheta[itrk] = backDir*nuDir;
    }
  }
}  


//Fills information for two track events
//**********************************************************
void FillTwoTracksVariables(bool track1, bool track2){ 

  // set constants
  //--------------
  double muMass = 105.658367; //MeV (muon mass)
  double pMass = 938.27203; //MeV (proton mass)
  double nMass = 939.56536; //MeV (neutron mass)
  double V = 25.; //MeV (nuclear potential) 

  double posMom = 0;
  double negMom = 0;
  double Emu = 0;
  double Ep = 0;

  //For two tracks
  if(track1 && track2){
    //The 2 most energetic tracks are one positive and one negative 
    if((_charge[0] ==  1 && _charge[1] == -1) || (_charge[0] == -1 && _charge[1] == 1)){
	
      TVector3 negDir;
      TVector3 posDir;
      TVector3 posMomVect;
      TVector3 negMomVect;
      double Mf2 = 0;
      double Ef = 0;
      TVector3 finalStateMom(0,0,0); 
      double fMom = 0;
      double fMomZ = 0;
      double cosThetaF = 0;
      int posIndex, negIndex;
	
      if(_charge[0] == 1){    
	posIndex = 0;
	negIndex = 1;
      }
      else{
	posIndex = 1;
	negIndex = 0;
      }

      posDir = _dirV[posIndex];
      posMom = _mom[posIndex];
      posMomVect = posMom * posDir;
      negDir = _dirV[negIndex];
      negMom = _mom[negIndex];
      negMomVect = negMom * negDir;
	
      Emu = sqrt(pow(muMass,2) + pow(_mom[negIndex],2));
      Ep = sqrt(pow(pMass, 2) + pow(_mom[posIndex],2));
	
      //final state kinematics
      Ef = Emu + Ep;
      finalStateMom = negMomVect + posMomVect;
      fMom = finalStateMom.Mag();
      fMomZ = finalStateMom.Z();
      Mf2 = pow(Ef,2)- pow(fMom,2);

      if(!_silent){
	cout << "          ------------------>  Mf2 = " << Mf2 << "      Ef = " << Ef << "      pow(finalStateMom.X(),2) + pow(finalStateMom.Y(),2) + pow(finalStateMom.Z(),2) = " << pow(finalStateMom.X(),2) + pow(finalStateMom.Y(),2) + pow(finalStateMom.Z(),2) << endl;
      }

      //we assume the neutrino moving along the z axis
      cosThetaF = fMomZ/fMom;

      _emu[0] = Emu;
      _ep = Ep;
      _cosThetaF = cosThetaF;
      _mf = sqrt(Mf2);
      _mf2 = Mf2;
      _ef = Ef;
      _pf = fMom;
	
      //neutrino energy
      //---------------
      _eNuRec[0] = ( ( (pow(pMass,2) - pow(muMass,2)) + 2*Emu*(nMass-V) - pow(nMass-V,2) )/ (-Emu + (nMass-V) + (_mom[negIndex] * _cosTheta[negIndex])) )/2;
      _eNuRec[1] = (Mf2 - nMass*nMass + 2*V*Ef + V*V) / (2*(V + Ef - fMom*cosThetaF));

      if(!_silent) cout << "          ------------------>  _eNuRec[0] = " << _eNuRec[0] << "     _eNuRec[1] = " << _eNuRec[1]<< endl;

      _muQ2[0] = (2*_eNuRec[0]*(_emu[0] - negMomVect.Mag()*_cosTheta[negIndex]) - pow(muMass,2))/1e6; //GeV
      _muQ2[1] = (2*_eNuRec[1]*(_emu[0] - negMomVect.Mag()*_cosTheta[negIndex]) - pow(muMass,2))/1e6; //GeV

      _pQ2[0] = (_eNuRec[0]*_eNuRec[0] - _ep*_ep + (finalStateMom - posMomVect)*(finalStateMom - posMomVect) + pow(nMass-V,2)*((2*_ep)/(nMass-V) -1) + 2*_eNuRec[0]*(_ep*_cosTheta[posIndex] - _pf*_cosThetaF))/1e6;

      _pQ2[1] = (_eNuRec[1]*_eNuRec[1] - _ep*_ep + (finalStateMom - posMomVect)*(finalStateMom - posMomVect) + pow(nMass-V,2)*((2*_ep)/(nMass-V) -1) + 2*_eNuRec[1]*(_ep*_cosTheta[posIndex] - _pf*_cosThetaF))/1e6;

      // deltaThetaP
      //------------
      double cosDeltaThetaP1 = (posDir.Z()*_eNuRec[0] - posDir*negMomVect) / (sqrt(negMom*negMom + _eNuRec[0]*_eNuRec[0] - 2*negMomVect.Z()*_eNuRec[0]));
      double cosDeltaThetaP2 = (posDir.Z()*_eNuRec[1] - posDir*negMomVect) / (sqrt(negMom*negMom + _eNuRec[1]*_eNuRec[1] - 2*negMomVect.Z()*_eNuRec[1]));
      _deltaThetaP[0] = acos(cosDeltaThetaP1);
      _deltaThetaP[1] = acos(cosDeltaThetaP2);
	
      // neutrino invariant mass
      //------------------------
      TLorentzVector proton4vector(posMomVect, Ep);
      TLorentzVector muon4vector(negMomVect, Emu);
      TLorentzVector neutron4vector(0, 0, 0, nMass);
	
      TLorentzVector sum = proton4vector + muon4vector - neutron4vector;
      _nuInvariantMass = (sum*sum)*1e-6; //GeV^2
    }
    //End if HM neg and SHM pos

    //The 2 most energetic tracks are both negative 
    else if(_charge[0] == -1 && _charge[1] == -1){
      double Emu_0 = sqrt(muMass*muMass + _mom[0]*_mom[0]);
      double Emu_1 = sqrt(muMass*muMass + _mom[1]*_mom[1]);

      _emu[0] = Emu_0;
      _emu[1] = Emu_1;
      
      _eNuRec[0] = ((pMass*pMass - muMass*muMass + 2*Emu_0*(nMass-V) - pow(nMass-V,2))/(-Emu_0 + (nMass-V) + (_mom[0]*_cosTheta[0])))/2;	
      _eNuRec[1] = ((pMass*pMass - muMass*muMass + 2*Emu_1*(nMass-V) - pow(nMass-V,2))/(-Emu_1 + (nMass-V) + (_mom[1]*_cosTheta[1])))/2;
      
      _muQ2[0] = (2*_eNuRec[0]*(_emu[0] - _mom[0]*_cosTheta[0]) - muMass*muMass)/1e6; //GeV
      _muQ2[1] = (2*_eNuRec[1]*(_emu[1] - _mom[1]*_cosTheta[1]) - muMass*muMass)/1e6; //GeV
      
      _pQ2[0] = 0;
      _pQ2[1] = 0;
    } 
    
    // the 2 most energetic tracks are both positive 
    else{
      _eNuRec[0] = 0;
      _eNuRec[1] = 0;
      _muQ2[0] = 0;
      _muQ2[1] = 0;
      _pQ2[0] = 0;
      _pQ2[1] = 0;
    }     
  }//End if two tracks
  
  //If there is only one track 
  else if(track1 && !track2){
    
    //If it's negative
    if(_charge[0] == -1){
      
      Emu = sqrt(muMass*muMass + _mom[0]*_mom[0]);
      _emu[0] = Emu;
      _eNuRec[0] = ((pMass*pMass - muMass*muMass + 2*Emu*(nMass-V) - pow(nMass-V,2))/(-Emu + (nMass-V) + (_mom[0]*_cosTheta[0])))/2;	
      _eNuRec[1] = 0;      
      _muQ2[0] = (2 * _eNuRec[0] * (_emu[0]- _mom[0] * _cosTheta[0]) - muMass*muMass)/1e6; //GeV
    }
  }
  else if(track2 && !track1){

    //If it's negative
    if(_charge[1] == -1){

      Emu = sqrt(muMass*muMass + _mom[1]*_mom[1]);
      _emu[0] = Emu;
      _eNuRec[0] = ((pMass*pMass - muMass*muMass + 2*Emu*(nMass-V) - pow(nMass-V,2))/(-Emu + (nMass-V) + (_mom[1]*_cosTheta[1])))/2;	
      _eNuRec[1] = 0;
      _muQ2[0] = (2*_eNuRec[0]*(_emu[0] - _mom[1]*_cosTheta[1]) - pow(muMass,2))/1e6; //GeV
    }
  }
  //No tracks
  else{
    _eNuRec[0] = 0;
    _eNuRec[1] = 0;
  }
}    


//
//**********************************************************
void FillDetectorsUsed(int itrk, int det){ 

  char cdet[50];
  sprintf(cdet,"%d",det);
  string idets[9]={"1", "2", "3", "4", "5", "6", "7", "8", "9"}

  string sdet;
  sdet.assign(cdet);

  //Loop over bunch containers
  for(int d = 0; d < NBUNCH; d ++){
    if(sdet.find(idets[d]) != string::npos) _detUsed[itrk][d] = 1;
  }
}


//
//**********************************************************
bool CheckTpcUsed(int tpc, int det){ 

  bool check = false;

  char cdet[50];
  sprintf(cdet, "%d", det);
  string sdet;
  sdet.assign(cdet);

  char ctpc[50];
  sprintf(ctpc, "%d", tpc);
  string stpc;
  stpc.assign(ctpc);

  size_t found;
  found = sdet.find(stpc);
  if(found != string::npos) check = true;

  return check;
}


//
//**********************************************************
int GetGenieReactionCode(string reactionCode){

  int code = 0;
  
  if(!_silent) cout << "3: " << reactionCode << endl;
  if(reactionCode.find("charm") == string::npos && reactionCode.find("Weak[CC],QES") != string::npos){
    code = 1;
  }
  else if(reactionCode.find("Weak[CC],RES") != string::npos){
    code = 11;
  }
  else if(reactionCode.find("charm") == string::npos && reactionCode.find("Weak[CC],DIS") != string::npos){
    code = 21;
  }
  else if(reactionCode.find("Weak[CC],COH") != string::npos){
    code = 16;
  }
  else if(reactionCode.find("Weak[NC],RES") != string::npos){
    code = 31;
  }
  else if(reactionCode.find("Weak[NC],QES") != string::npos){
    code = 51;
  }
  else if(reactionCode.find("Weak[NC],COH") != string::npos){
    code = 36;
  }
  else if(reactionCode.find("Weak[NC],DIS") != string::npos){
    code = 41;
  }

  return code;  
}


//
//**********************************************************
double GetBunchTime(int bunchno){

  double bunchtime = -999.;

  double time_mc[9] = {0, 87.5, 671.6, 1252.9, 1837.3, 2416.1, 2998.6, 10000.0, -5000.0};
  double time_data[9] = {0, 3357.2, 3940.8, 4523.9, 5105.9, 5689.6, 6272.3, 10000.0, -5000.0};

  if(_isMC){
    bunchtime = time_mc[bunchno];
  }
  else{
    bunchtime = time_data[bunchno];
  }

  return bunchtime;
}


//
//**********************************************************
int GetBunch(double tTrack){

  int bunch = 8;

  // ---- For MC rec and data -----------
  double time_mc[7]    = {-202.5, 379.5, 961.5, 1543.5, 2125.5, 2707.5, 3289.5};
  
  double time_data[7]  = {3066.2, 3648.2, 4230.2, 4812.2, 5394.2, 5976.2, 6558.2};
  
  if(_isMC){
        
    if(tTrack > time_mc[6]){
      bunch = 7;    
    }
    else if(tTrack > time_mc[5] && tTrack < time_mc[6]){
      bunch = 6;    
    }
    else if(tTrack > time_mc[4] && tTrack < time_mc[5]){
      bunch = 5;    
    }
    else if(tTrack > time_mc[3] && tTrack < time_mc[4]){
      bunch = 4;    
    }
    else if(tTrack > time_mc[2] && tTrack < time_mc[3]){
      bunch = 3;    
    }
    else if(tTrack > time_mc[1] && tTrack < time_mc[2]){
      bunch = 2;    
    }
    else if(tTrack > time_mc[0] && tTrack < time_mc[1]){
      bunch = 1;    
    }
    if(fabs(tTrack) < 1e-5){
      bunch = 0;
    }
  }
  else{
        
    if(tTrack > time_data[6]){
      bunch = 7;    
    }
    else if(tTrack > time_data[5] && tTrack < time_data[6]){
      bunch = 6;    
    }
    else if(tTrack > time_data[4] && tTrack < time_data[5]){
      bunch = 5;    
    }
    else if(tTrack > time_data[3] && tTrack < time_data[4]){
      bunch = 4;    
    }
    else if(tTrack > time_data[2] && tTrack < time_data[3]){
      bunch = 3;    
    }
    else if(tTrack > time_data[1] && tTrack < time_data[2]){
      bunch = 2;    
    }
    else if(tTrack > time_data[0] && tTrack < time_data[1]){
      bunch = 1;    
    }
    if(fabs(tTrack) < 1e-5){
      bunch = 0;
    }
  }

  if(!_silent){
    cout << time_mc[0] << " " << time_mc[1] << " " << time_mc[2] << " " << time_mc[3] << " " << time_mc[4] << " " << time_mc[5] << " " << time_mc[6] << " " << endl;
    cout << " Bunch   " << bunch << "  " << tTrack << endl;
  }

  return bunch;  
}


//
//**********************************************************
void FillTrueQ2(){
 
  double leptonMass = 0;
  double trueCosTheta = 0;
  
  TVector3 nuDir(_trueNuDir[_nTrueVertices][0], _trueNuDir[_nTrueVertices][1], _trueNuDir[_nTrueVertices][2]);
  TVector3 dir(_trueLeptonDir[_nTrueVertices][0], _trueLeptonDir[_nTrueVertices][1], _trueLeptonDir[_nTrueVertices][2]);
  trueCosTheta = (dir*nuDir);  

  //Muon
  if(abs(_trueNuPDG[_nTrueVertices]) == 14) leptonMass = 105.6583668;
  //Electron
  else if(abs(_trueNuPDG[_nTrueVertices]) == 12) leptonMass = 0.510998910;

  _trueQ2[_nTrueVertices] = (2*_trueNuEnergy[_nTrueVertices]*(_trueLeptonEnergy[_nTrueVertices] - _trueLeptonMom[_nTrueVertices] * trueCosTheta) - pow(leptonMass,2))/1e6; //GeV

}


//
//**********************************************************
double GetEnuReweight(double enu){

  double weight[20] = {  
    1.02943,  
    0.979603,  
    1.00123,  
    1.03182,  
    1.03621, 
    1.03053, 
    1.01495, 
    0.997691, 
    0.974313, 
    0.949989, 
    0.906395, 
    0.90201, 
    0.928674, 
    0.959829, 
    0.917201, 
    0.940934, 
    0.92232, 
    0.961158, 
    0.995986, 
    0.982822
  };

  int bin = (int)(enu/100);

  if(bin < 20 && bin >= 0) return weight[bin];
  
  return 1;
}


//**********************************************************
//Main method to be called from outside
//**********************************************************
void RunMiniTree(int evLow = 0, int evHigh = 1e6, int mc = 1, int silent = 1, string infile_name){
  
  fRandomGenerator = TRandom3(0);

  //------------------------------
  // Load some important libraries.
  //------------------------------
  
  gROOT->SetBatch(1);
  gSystem->AddIncludePath("-I$OAANALYSISROOT/src");
  gSystem->AddIncludePath("-I.");
  
  //TEMPORARY SOLUTION
  gSystem->Load("libPhysics");
  gSystem->Load("libGeom");
  gSystem->Load("libTree");
  gSystem->Load("libMatrix");
  gSystem->Load("liboaEvent.so");
  gSystem->Load("librecpack.so");
  gSystem->Load("liboaRecPack.so");
  gSystem->Load("liboaAnalysis.so");

  gROOT->ProcessLine(".L $OAANALYSISROOT/macros/AnalysisTools/TNEUTAnalysisUtils.cxx");
  gROOT->ProcessLine(".L $OAANALYSISROOT/macros/AnalysisTools/TGENIEAnalysisUtils.cxx");
  gROOT->ProcessLine(".L $OAANALYSISROOT/macros/AnalysisTools/TTrackerAnalysisUtils.cxx");
  gROOT->ProcessLine(".L $OAANALYSISROOT/macros/AnalysisTools/TCCtruthAnalysisUtils.cxx");
  gROOT->ProcessLine(".L $OAANALYSISROOT/macros/AnalysisTools/TGlobalAnalysisUtils.cxx");

  //Set input parameters
  _isMC = (bool)mc;
  TString infileName(infile_name);  
  _silent = (bool)silent;
  
  //Create Tree objects
  TChain mcTruthVertices = TChain("TruthDir/Vertices");
  TChain mcTruthTrajectories = TChain("TruthDir/Trajectories");
  TChain reconGlobal = TChain("ReconDir/Global");
  
  //Create utils objects
  TNEUTAnalysisUtils *NeutUtils = new TNEUTAnalysisUtils();
  TGENIEAnalysisUtils *GenieUtils = new TGENIEAnalysisUtils();
  TTrackerAnalysisUtils *AnTUtils = new TTrackerAnalysisUtils();  
  TCCtruthAnalysisUtils *trueAn = new TCCtruthAnalysisUtils();
  TGlobalAnalysisUtils *globAn = new TGlobalAnalysisUtils();
  
  //Define the analysis tree
  _mini = new TTree("mini", "mini analysis tree");
  DefineTree();

  // _____________________________________________________________________________________
  // Load data files     
  if(!_silent) cout<<"Minitree reading data..."<<endl;

  ifstream inputFile(infileName.Data(), ios::in);
  
  if(!inputFile){
    cerr << "Cannot open input file '" << fileName.Data() << "'. Exit!" << endl;
    return;
  }
  
  //Loop over the input files
  string inputString;   
  while(inputFile >> inputString){
    if(!_silent) cout << "Adding file: " << inputString << endl;
    if(_isMC){
      mcTruthVertices.AddFile(inputString.c_str());
      mcTruthTrajectories.AddFile(inputString.c_str());
    }
    reconGlobal.AddFile(inputString.c_str());
  }    
  
  // Open the output file
  string outfile_name = infile_name.substr(0,infile_name.rfind('.'));
  TString outfileName(outfile_name);   
  outfileName += "_";
  outfileName += evLow;
  outfileName += "_";
  outfileName += evHigh;
  outfileName += ".root";
  TFile *outputFile = new TFile(outfileName, "RECREATE");
    
  // _____________________________________________________________________________________
  // Prepare to read trees.

  // Set the branches for the truth information.
  TClonesArray *TruthVertices[NVTXTYPES];
  TClonesArray *TruthTrajs[NTRAJTYPES];

  int NTruthVertices[NVTXTYPES];
  int NTruthTrajs[NTRAJTYPES];

  string vtxnames[NVTXTYPES] = {"VtxFGD1","VtxFGD2","VtxP0D","VtxTPC","VtxMRD","VtxDsECal","VtxBrlECal","VtxRestOfOffAxis","VtxOther","VtxINGRID","VtxP0DECal"};
  int TruthVertexDet[NVTXTYPES] = {4, 5, 8, 1, 9, 6, 7, 10, 11, 12, 7};
  string trajnames[NTRAJTYPES] = {"TrajsLepton","TrajsBaryon","TrajsMeson","TrajsPhoton","TrajsOtherCharged","TrajsOtherNeutral","TrajsOther"};
  string ntrajnames[NTRAJTYPES] = {"NTrajsLepton","NTrajsBaryon","NTrajsMeson","NTrajsPhoton","NTrajsOtherCharged","NTrajsOtherNeutral","NTrajsOther"};

  //For MC fill true info
  if(_isMC){  
    if(!_silent) cout<<" set branches adresses "<<endl;
    for(int ii = 0; ii < NVTXTYPES; ii ++){
      TruthVertices[ii] = new TClonesArray("ND::TTruthVerticesModule::TTruthVertex", 50);
      mcTruthVertices.SetBranchAddress(vtxnames[ii].c_str(), &(TruthVertices[ii]));
      mcTruthVertices.SetBranchAddress(("N"+vtxnames[ii]).c_str(), &(NTruthVertices[ii]));
    }
    mcTruthVertices.Print("toponly");
        
    for(int ii = 0; ii < NTRAJTYPES; ii ++){
      TruthTrajs[ii] = new TClonesArray("ND::TTruthTrajectoriesModule::TTruthTrajectory", 200);
      mcTruthTrajectories.SetBranchAddress(trajnames[ii].c_str(), &(TruthTrajs[ii]));
      mcTruthTrajectories.SetBranchAddress(ntrajnames[ii].c_str(), &(NTruthTrajs[ii]));
    }
    mcTruthTrajectories.Print("toponly");
  }  
    
  //Set the branch addresses for variables from the oaAnalysis tree
  //-------------------------------------------------------------------------
  int NPIDs;
  int NTPC;
  int NTPCIso;
  int NECALIso;
  int NP0DIso;
  int NSMRDIso;
  int NFGDIso;
  int NFGD1Unused, NFGD2Unused; 
  int NDelayedClusters;                          
  double EarliestTrackTime;
  int EventID;
  int RunID;

  TClonesArray *PIDs = new TClonesArray("ND::TGlobalReconModule::TGlobalPID", 20);
  TClonesArray *DelayedClusters = new TClonesArray("ND::TGlobalReconModule::TFgdCluster", 10);

  reconGlobal.SetBranchAddress("NPIDs", &NPIDs);
  reconGlobal.SetBranchAddress("PIDs", &PIDs);
  reconGlobal.SetBranchAddress("NTPC", &NTPC);
  reconGlobal.SetBranchAddress("NTPCIso", &NTPCIso);
  reconGlobal.SetBranchAddress("NECALIso", &NECALIso);
  reconGlobal.SetBranchAddress("NP0DIso", &NP0DIso);
  reconGlobal.SetBranchAddress("NSMRDIso", &NSMRDIso);
  reconGlobal.SetBranchAddress("NFGDIso", &NFGDIso);  
  reconGlobal.SetBranchAddress("NFGD1Unused", &NFGD1Unused);
  reconGlobal.SetBranchAddress("NFGD2Unused", &NFGD2Unused);
  reconGlobal.SetBranchAddress("NDelayedClusters", &NDelayedClusters);
  reconGlobal.SetBranchAddress("DelayedClusters", &DelayedClusters);    
  reconGlobal.SetBranchAddress("EarliestTrackTime", &EarliestTrackTime);
  reconGlobal.SetBranchAddress("EventID", &EventID);
  reconGlobal.SetBranchAddress("RunID", &RunID);

  reconGlobal.Print("toponly");

  // __________________________________________________________________________
  // Loop over all events.  
  int totalNbrOfEntries = reconGlobal.GetEntries();
  if(!_silent){
    cout << "**************************************"<< endl;
    cout << "-------------------> totalNbrOfEntries = " << totalNbrOfEntries << endl; 
    cout << "**************************************"<< endl;
  }

  int numberEntries = reconGlobal.GetEntries();
  cout << "Number entries: " << numberEntries << endl;
  if(evHigh > numberEntries) evHigh = numberEntries;

  if(!_silent) cout << "Number entries: " << numberEntries << endl;
  
  // time before event loop
  TStopwatch timer;
  timer.Start();

  //Loop over events (spill) 
  //-------------------------------------------------------------
  for(int ient = evLow; ient < evHigh; ient ++){

    //Reset tree variables
    //---------------------
    ResetSpillVariables();
    
    if(!_silent) cout << "--------> begin event " << ient << endl;

    if(_isMC){
      mcTruthVertices.GetEntry(ient);    
      mcTruthTrajectories.GetEntry(ient);    
    }

    reconGlobal.GetEntry(ient);
    string filenametoParse;    
    string ParsedRun;    
    string ParsedSubRun;    
    stringstream filenameString;
    filenameString << reconGlobal.GetFile()->GetName();
    filenametoParse = filenameString.str();
    int arxi = filenametoParse.find("oa_", 0);

    if(_isMC){
      ParsedRun = filenametoParse.substr(arxi + 11, 8);
      ParsedSubRun = filenametoParse.substr(arxi + 20, 4);
    }
    else{
      ParsedRun = filenametoParse.substr(arxi + 10, 8);
      ParsedSubRun = filenametoParse.substr(arxi + 19, 4);
    }

    if(!_silent){ 
      cout << "--------> File---->   " << reconGlobal.GetFile()->GetName() << endl;
      cout << "--------> FileRun---->   " << ParsedRun << endl;
      cout << "--------> FieSubRun---->   " << ParsedSubRun << endl;
    }

    if(ient%100 == 0 && !_silent) cout <<  "Events Processed " << ient << endl;

    // get the global tracks
    //----------------------
    int momMax = -999;    
    int negMomMax = -999;    
    int HitsMax;    
    int NMaxNegReconTrack = 20;
    int NMaxPosReconTrack = 20;
    int NMaxRecTrack = 20;
    int NMaxFgdTrack = 20;
    int NFgdTrack[NBUNCH] = {0};
    int NNegReconTrack[NANACUTS][NBUNCH] = {0};
    int NPosReconTrack[NANACUTS][NBUNCH] = {0};
    int NRecTrack[NANACUTS][NBUNCH] = {0};
    int NRecTrack_total[NANACUTS] = {0};
    int NGlobalTracks_total[NBUNCH] = {0};
    ND::TGlobalReconModule::TGlobalPID recTrack[NMAXTRACKS][NBUNCH];    
    ND::TGlobalReconModule::TGlobalPID recFgdTrack[NMAXTRACKS][NBUNCH];
    int NTpc1RecTrack[NANACUTS][NBUNCH] = {0};
    int NTpc2RecTrack[NANACUTS][NBUNCH] = {0};
    int NTpc3RecTrack[NANACUTS][NBUNCH] = {0};
    int NTpc1RecTrack_total[NANACUTS] = {0};
    int NTpc2RecTrack_total[NANACUTS] = {0};
    int NTpc3RecTrack_total[NANACUTS] = {0};

    double momRelErrCut[NANACUTS] = {1e13, 0.3, 0.2, 0.15, 0.1};
    int nvtxPerBunch[NBUNCH] = {0};

    //-------------------------
    //Reset counters 
    for(int i = 0; i < NANACUTS; i ++){
      NRecTrack_total[i] = 0;

      NTpc1RecTrack_total[i] = 0;
      NTpc2RecTrack_total[i] = 0;
      NTpc3RecTrack_total[i] = 0;

      for(int j = 0; j < NBUNCH; j ++){
	NNegReconTrack[i][j] = 0;
	NPosReconTrack[i][j] = 0;
	NRecTrack[i][j] = 0;
	NFgdTrack[j] = 0;
	NTpc1RecTrack[i][j] = 0;
	NTpc2RecTrack[i][j] = 0;
	NTpc3RecTrack[i][j] = 0;
      }
    }
    
    //Fill MC info for event
    //---------------------------------------------------------
    if(_isMC){
      ND::TTruthVerticesModule::TTruthVertex *true_vertex = NULL;
      //Loop over vertex types
      for(int vv = 0; vv < NVTXTYPES; vv ++){

	//Loop over true vertices
	for(int v = 0; v < NTruthVertices[vv]; v ++){

	  true_vertex = (ND::TTruthVerticesModule::TTruthVertex*) TruthVertices[vv]->UncheckedAt(v);

	  if(!true_vertex) continue;
	  if(_nTrueVertices == NMAXTRUEVERTICES) break;
	     	     
	  _trueVertexDet[_nTrueVertices] = TruthVertexDet[vv];
	     
	  if((true_vertex->ReactionCode).find(":") != string::npos){
	    _reacCode[_nTrueVertices] = GetGenieReactionCode(true_vertex->ReactionCode);
	  }	  
	  else{
	    _reacCode[_nTrueVertices] = atoi((true_vertex->ReactionCode).c_str());
	  }	     
	  for(int k = 0; k < 4; k ++){
	    _trueVertexPos[_nTrueVertices][k] = true_vertex->Vertex[k];
	  }

	  _trueVertexBunch[_nTrueVertices] = GetBunch(_trueVertexPos[_nTrueVertices][3]);

	  // get the number of true vertices per bunch
	  nvtxPerBunch[ _trueVertexBunch[_nTrueVertices] ] ++;
	     
	  _trueNuEnergy[_nTrueVertices] = true_vertex->MomNu.Vect().Mag();

	  _trueEnuWeight[_nTrueVertices] = GetEnuReweight(_trueNuEnergy[_nTrueVertices]);
	     
	  _trueBaryonSize[_nTrueVertices] = true_vertex->BaryonMoms.size();
	     
	  if(true_vertex->LeptonMoms.size() > 0){
	    _trueLeptonEnergy[_nTrueVertices] = true_vertex->LeptonMoms[0].E();
	  }

	  for(int ii = 0; ii < 3; ii ++){
	    if(true_vertex->MomNu.Vect().Mag() > 0){
	      _trueNuDir[_nTrueVertices][ii] = true_vertex->MomNu.Vect()[ii]/true_vertex->MomNu.Vect().Mag();
	    }

	    if(true_vertex->MomTarget.Vect().Mag() > 0){
	      _trueTargetDir[_nTrueVertices][ii] = true_vertex->MomTarget.Vect()[ii]/true_vertex->MomTarget.Vect().Mag();
	    }

	    if(true_vertex->LeptonMoms.size() > 0){
	      if(true_vertex->LeptonMoms[0].Vect().Mag() > 0){
		_trueLeptonDir[_nTrueVertices][ii] = true_vertex->LeptonMoms[0].Vect()[ii]/true_vertex->LeptonMoms[0].Vect().Mag();
	      }
	    }	       

	    if( true_vertex->BaryonMoms.size() > 0){
	      if(true_vertex->BaryonMoms[0].Vect().Mag() > 0){
		_trueProtonDir[_nTrueVertices][ii] = true_vertex->BaryonMoms[0].Vect()[ii]/true_vertex->BaryonMoms[0].Vect().Mag();
	      }
	    }
	  }
	     
	  _trueNuPDG[_nTrueVertices] = true_vertex->PDGnu;
	     
	  if(true_vertex->LeptonMoms.size() > 0){
	    _trueLeptonMom[_nTrueVertices] = true_vertex->LeptonMoms[0].Vect().Mag();
	  }

	  if(true_vertex->BaryonMoms.size() > 0){
	    _trueProtonMom[_nTrueVertices] = true_vertex->BaryonMoms[0].Vect().Mag();
	  }

	  _trueTargetMom[_nTrueVertices] = true_vertex->MomTarget.Vect().Mag();
	  _trueTargetPDG[_nTrueVertices] = true_vertex->PDGtarget;
	     
	  if(true_vertex->LeptonMoms.size() > 0) FillTrueQ2();
	  else _trueQ2[_nTrueVertices] = 0;

	  //Fill primary trajectory info for each type of vertices
	  //----------------------------------------------------------
	  for(unsigned int ll = 0; ll < true_vertex->LeptonIDs.size(); ll ++){
	    _truePrimaryID[_nTrueVertices][_nPrimTraj] = true_vertex->LeptonIDs[ll];
	    _nPrimTraj ++;
	  }
	  for(unsigned int ll = 0; ll < true_vertex->BaryonIDs.size(); ll ++){
	    _truePrimaryID[_nTrueVertices][_nPrimTraj] = true_vertex->BaryonIDs[ll];
	    _nPrimTraj ++;
	  }
	  for(unsigned int ll = 0; ll < true_vertex->OtherNeutralIDs.size(); ll ++){
	    _truePrimaryID[_nTrueVertices][_nPrimTraj] = true_vertex->OtherNeutralIDs[ll];
	    _nPrimTraj ++;
	  }
	  for(unsigned int ll = 0; ll < true_vertex->MesonIDs.size(); ll ++){
	    _truePrimaryID[_nTrueVertices][_nPrimTraj] = true_vertex->MesonIDs[ll];
	    _nPrimTraj ++;
	  }
	  for(unsigned int ll = 0; ll < true_vertex->OtherChargedIDs.size(); ll ++){
	    _truePrimaryID[_nTrueVertices][_nPrimTraj] = true_vertex->OtherChargedIDs[ll];
	    _nPrimTraj ++;
	  }
	  for(unsigned int ll = 0; ll < true_vertex->PhotonIDs.size(); ll ++){
	    _truePrimaryID[_nTrueVertices][_nPrimTraj] = true_vertex->PhotonIDs[ll];
	    _nPrimTraj ++;
	  }

	  _nTrueVertices ++;

	}//End loop over vertices
      }//End loop over vertex types
    }//End if MC
    //----------------------------------------------------------------------

    //Containers for momentum sorting
    vector<int> global_index_usort[NANACUTS][NBUNCH];
    vector<double> momentum_usort[NANACUTS][NBUNCH]; //unsorted momentum
    vector<int> fgd_index_usort[NBUNCH]; 
    vector<double> fgd_nhits_usort[NBUNCH]; //unsorted nhits

    //Loop over PIDs in event
    //-------------------------------------------------------------
    for(int j = 0; j < NPIDs; j ++){
      ND::TGlobalReconModule::TGlobalPID *globalTrack = (ND::TGlobalReconModule::TGlobalPID*)(*PIDs)[j];      

      if(!globalTrack) continue;
  
      // --- Get the bunch number -----
      double tTrack = 0;	
      // Forward going track      
      TLorentzVector frontPos = globalTrack->FrontPosition;
      TLorentzVector backPos = globalTrack->BackPosition;
      double frontzi = frontPos.Z();
      double backzi = backPos.Z();

      //Forwards going track
      if((globalTrack->isForward && (frontzi < backzi)) || (!globalTrack->isForward && (frontzi > backzi))){
	tTrack = frontPos.T();
      }
      // Backwards going track
      else{
	tTrack = backPos.T();
      }
      
      int ibunch = GetBunch(tTrack);
      NGlobalTracks_total[ibunch]++;    
      if(!_silent) cout << "tTrack = " << tTrack << "      ibunch = " << ibunch << endl;
      
      if(globalTrack->Momentum == 0) continue;
      
      //Fgd iso tracks
      if(globalTrack->Detectors == 4 || globalTrack->Detectors == 5){ 

	if(NFgdTrack[ibunch] < NMaxFgdTrack) break;

	recFgdTrack[ NFgdTrack[ibunch] ][ibunch] = *globalTrack;
	NFgdTrack[ibunch] ++;	
	double fgdhits = globalTrack->NHits;
	fgd_index_usort[ibunch].push_back(j);
	fgd_nhits_usort[ibunch].push_back(fgdhits);
      }
      
      //For TPC tracks
      if(!globalTrack->NTPCs) continue;

      //Loop over cuts	
      for(int icut = 0; icut < NANACUTS; icut ++){

	//Apply relative momentum cut
	if(globalTrack->MomentumError/globalTrack->Momentum > momRelErrCut[icut]) continue;
	
	if(NRecTrack[icut][ibunch] < NMaxRecTrack){
	  recTrack[NRecTrack[icut][ibunch]][ibunch] = *globalTrack;
	}	    

	global_index_usort[icut][ibunch].push_back(j);
	momentum_usort[icut][ibunch].push_back(globalTrack->Momentum);
	    
	NRecTrack[icut][ibunch] ++;
	NRecTrack_total[icut] ++;
	    
	int detec = globalTrack->Detectors;
	if(!_silent) cout << " detec = " << detec << endl;
	    
	if(CheckTpcUsed(1, detec)){
	  if(!_silent) cout << " TPC1 " << endl;
	  NTpc1RecTrack[icut][ibunch] ++;
	  NTpc1RecTrack_total[icut] ++;
	}
	if(CheckTpcUsed(2, detec)){
	  if(!_silent) cout << " TPC2 " << endl;
	  NTpc2RecTrack[icut][ibunch] ++;
	  NTpc2RecTrack_total[icut] ++;
	}
	if(CheckTpcUsed(3, detec)){
	  if(!_silent) cout << " TPC3 " << endl;
	  NTpc3RecTrack[icut][ibunch] ++;
	  NTpc3RecTrack_total[icut] ++;
	}
	if(globalTrack->Charge > 0){
	  NPosReconTrack[icut][ibunch] ++;
	}
	if(globalTrack->Charge < 0){
	  NNegReconTrack[icut][ibunch] ++;
	}
      }//End loop over cuts
    }//End loop over PIDs in event
    //---------------------------------------------------------------------

    //Check whether we have a true interaction in
    //the interesting detectors
    bool true_in_det = false;
    for(int i = 0; i < _nTrueVertices; i ++){
      int det = _trueVertexDet[i];
      if(det == 4 || det == 5 || det == 8) true_in_det = true;
    }

    //If no tracks remaining at this cut, nothing to do  
    // ---- consider also events with FGD iso tracks ------------
    if(!NRecTrack_total[_cutNbr] && !NFGDIso && !true_in_det) continue;
      
    _evt = EventID;
    _entry = ient;
    _filerun = atoi(ParsedRun.c_str());
    if(!_silent) cout << "--------> FileRun---->   " << _filerun << endl;
    _filesubrun = atoi(ParsedSubRun.c_str());
    if(!_silent) cout << "--------> FieSubRun---->   " << _filesubrun << endl;

    if(_isMC) _run = 0;
    else _run = RunID;
      
    for(int i = 0; i < NANACUTS; i ++){
      _ntracks_total[i] = NRecTrack_total[i];
      _nTpctracks_total[0][i] = NTpc1RecTrack_total[i];
      _nTpctracks_total[1][i] = NTpc2RecTrack_total[i];
      _nTpctracks_total[2][i] = NTpc3RecTrack_total[i];
    }
      
    _nfgdtracks  = NFGDIso;
    _necaltracks = NECALIso;
    _np0dtracks  = NP0DIso;
    _nsmrdtracks = NSMRDIso;
    _ntpctracks =  NTPCIso;

    _earliestTrackTime = EarliestTrackTime;      
      
    _nFGDUnused[0] = NFGD1Unused;
    _nFGDUnused[1] = NFGD2Unused;
                  
    // Find the bunch which contains the highest momentum track of the spill
    double max_mom = -999.;
    int max_mom_bunch = -1;
    //Loop over cuts
    for(int i = 0; i < NANACUTS; i ++){
      //Loop over bunch containers
      for(int j = 0; j < NBUNCH; j ++){
	//Loop over the selected tracks
	for(int ii = 0; ii < NRecTrack[i][j]; ii ++){
	  if(!_silent) cout << "evt " << _evt <<" mom unsort  " << momentum_usort[i][j][ii] << " bunch " << j<< endl;
	  if(momentum_usort[i][j][ii] > max_mom){
	    if(!_silent) cout << " keep bunch " << j <<endl;       
	    max_mom = momentum_usort[i][j][ii];
	    max_mom_bunch = j;
	  }
	}
      }
    }//End loop over cuts
                  
    int haveTPC1track = 0;
    bool savespillonce = false;
      
    // Now: Bunch 0-->time exactly zero;1-6 real bunches but no sigma,
    // 7 huge times, 8 everything else(e.g. "very" negative times for MC)

    //Loop over bunches in event
    //-----------------------------------------------------------------------------------
    for(int ibunch = 0; ibunch < NBUNCH; ibunch ++){
  
      if(NRecTrack[_cutNbr][ibunch] > 0){ 
	_nbunches ++;
	if(ibunch != 0 && ibunch < 7) _nvalidbunches++;
      }    

      //If no tracks, nothing to do
      if(!NRecTrack[_cutNbr][ibunch] && !NFgdTrack[ibunch] && !true_in_det) continue;
	  
      _bunchnbr ++; //same as _bunches?

      //Clear containers (but not the spill ones!)
      ResetBunchVariables();

      _ntracks_bunch = NRecTrack[_cutNbr][ibunch];
      _nfgdtracks_bunch = NFgdTrack[ibunch];
      _bunch = ibunch;
      _tbunch = GetBunchTime(ibunch);
      _totaltracks = NGlobalTracks_total[ibunch];
      _trueSpillInfo = 0;
      if(!savespillonce){
	savespillonce = true;
	_trueSpillInfo = 1;
      }
      
      //get the number of true vertices per recbunches
      if(_isMC) _nTrueVerticesPerBunch = nvtxPerBunch[ibunch];
      
      //Fill _BunchWithMaxMomInSpill
      if(ibunch == max_mom_bunch) _BunchWithMaxMomInSpill = 1;
      else _BunchWithMaxMomInSpill = 0;

      ND::TGlobalReconModule::TGlobalPID fgdtracks_bunch[NMAXTRACKS];
      bool fgdtracks_bunch_exist[NMAXTRACKS];
	  
      ND::TGlobalReconModule::TGlobalPID tracks_bunch[NMAXTRACKS];
      bool tracks_bunch_exist[NMAXTRACKS];
	  
      for(int i = 0; i < NMAXTRACKS; i ++){
	fgdtracks_bunch_exist[i] = false;
	tracks_bunch_exist[i] = false;
      }

      for(int i = 0; i < NANACUTS; i ++){
	_ntracks[i] = NRecTrack[i][ibunch];
	_npostracks[i] = NPosReconTrack[i][ibunch];
	_nnegtracks[i] = NNegReconTrack[i][ibunch];  

	_ntpctracks_bunch[0][i] = NTpc1RecTrack[i][ibunch];
	_ntpctracks_bunch[1][i] = NTpc2RecTrack[i][ibunch];
	_ntpctracks_bunch[2][i] = NTpc3RecTrack[i][ibunch];
      }

      //sort all the tracks by momentum
      //--------------------------------------------------------------------
      vector<double> momentum; //momentum in falling order
      vector<int> index; //index of momentum order
      //Copy vector and sort in momentum
      for(int i = 0; i < _ntracks_bunch; i ++){
	momentum.push_back(momentum_usort[_cutNbr][ibunch][i]);
      }
      //Sort according to increasing momentum
      sort(momentum.begin(),momentum.end());
      //Mapping of index for decreasing momentum
      for(int i = 0; i < _ntracks_bunch; i ++){
	for(int ii = 0; ii < _ntracks_bunch; ii ++){
	  if(momentum[_ntracks_bunch - 1 - i] == momentum_usort[_cutNbr][ibunch][ii]){
	    index.push_back(ii);
	  }
	}
      }

      for(int j = 0; j < _ntracks_bunch; j ++){

	if(j == NMAXTRACKS) break;

	int it = index[j];
	int index_pos = global_index_usort[_cutNbr][ibunch][it];
	ND::TGlobalReconModule::TGlobalPID *gTrack = (ND::TGlobalReconModule::TGlobalPID*)(*PIDs)[index_pos];
	tracks_bunch[j] = *gTrack;
	tracks_bunch_exist[j] = true;
      }
      
      // Get the FGD iso tracks with higher number of hits
      // get the track with highest momentum:sort tracks       
      vector<double> fgd_nhits; //nhits in falling order
      vector<int> fgd_index_hits; //index of nhits order

      //Copy vector and sort in nhits
      for(int i = 0; i < _nfgdtracks_bunch; i ++) fgd_nhits.push_back(fgd_nhits_usort[ibunch][i]);

      //Sort according to increasing nhits
      sort(fgd_nhits.begin(),fgd_nhits.end());

      //Mapping of index for decreasing nhits
      for(int i = 0; i < _nfgdtracks_bunch; i ++){

	for(int ii = 0; ii < NPIDs; ii ++){

	  if(fgd_nhits[_nfgdtracks_bunch - 1 - i] == fgd_nhits_usort[ibunch][ii]) fgd_index_hits.push_back(ii);
	}
      }
      for(int j = 0; j < _nfgdtracks_bunch; j ++){

	if(j == NMAXTRACKS) break;

	int it = fgd_index_hits[j];
	int index_pos = fgd_index_usort[ibunch][it];
	ND::TGlobalReconModule::TGlobalPID *gTrack = (ND::TGlobalReconModule::TGlobalPID*)(*PIDs)[index_pos];
	fgdtracks_bunch[j] = *gTrack;
	fgdtracks_bunch_exist[j] = true;
      }	 
	  
      //Fill the tree for this bunch
      //------------------------------------------------------------
	  
      // get the vertex 
      // 1. start point of the track with highest momentum)    
      TLorentzVector thisPos = GetStartOfTrack(tracks_bunch[0].BackPosition,tracks_bunch[0].FrontPosition, tracks_bunch[0].isForward);
	  
      for(int i = 0; i < 4; i ++) _vertex[0][i] = thisPos[i];  
      TLorentzVector vtx1(_vertex[0][0], _vertex[0][1], _vertex[0][2], _vertex[0][3]); 
	  
      // 2. start point of the negative track with highest momentum    
      if(tracks_bunch[0].Charge == -1){
	for(int i = 0; i < 4; i ++){
	  _vertex[1][i] = thisPos[i];
	}
      }
      else if(tracks_bunch[1].Charge == -1){
	thisPos = GetStartOfTrack(tracks_bunch[1].BackPosition,tracks_bunch[1].FrontPosition, tracks_bunch[1].isForward);
	for(int i = 0; i < 4; i ++){
	  _vertex[1][i] = thisPos[i];
	}
      }	  
      TLorentzVector vtx2(_vertex[1][0], _vertex[1][1], _vertex[1][2], _vertex[1][3]);       

      /*	  
      //To be used at a later stage?
      //---------------------------------------------------------------
      //3. Vertex 1 from GlobalAnalysisTools
      double quality = 0;
      vector< int > charge_mod;
      TLorentzVector vtx3 = globAn->ComputeRecVertex(PIDs,charge_mod,quality,1,ibunch,isMC);
      for(int i = 0; i < 4; i ++){
	_vertex[0][i] = vtx3[i];
      }
     
      //4. Vertex 2 from GlobalAnalysisTools
      TVector3 vpos_a(_vertex[0][0],_vertex[0][1],_vertex[0][2]);
      _vtxquality[0] = quality;
      TLorentzVector vtx4 = globAn->ComputeRecVertex(PIDs,charge_mod,quality,2,ibunch,isMC);
      for(int i = 0; i < 4; i ++){
	_vertex[1][i] = vtx4[i];
      }
      //---------------------------------------------------------------
      */

      //Fill the delayed clusters
      FillDelayedClusters(DelayedClusters, NDelayedClusters, EarliestTrackTime, vtx1, vtx2); 
	  
      //Fill FGD iso tracks 	  
      //Loop over the tracks in bunch
      //----------------------------------------------------------------
      for(int itrk = 0; itrk < _nfgdtracks_bunch; itrk ++){

	if(itrk == NMAXTRACKS) break;

	//If FGD track fill FGD info
	if(!fgdtracks_bunch_exist[itrk]) continue;
	  
	_fgdNHits[itrk] = fgdtracks_bunch[itrk].NHits;
	_fgdchi2[itrk] = fgdtracks_bunch[itrk].Quality;
	_fgddet[itrk] = fgdtracks_bunch[itrk].Detectors;
	
	// Position and Direction at vertex
	FillPosDirAtVertex(itrk, _fgddet[itrk], 
			   fgdtracks_bunch[itrk].BackPosition,
			   fgdtracks_bunch[itrk].FrontPosition,
			   fgdtracks_bunch[itrk].BackDirection, 
			   fgdtracks_bunch[itrk].FrontDirection, 
			   fgdtracks_bunch[itrk].isForward);
      }//End loop over FGD tracks

      //Fill global tracks with TPC constituents
      //Loop over the tracks in bunch
      //----------------------------------------------------------------
      for(int itrk = 0; itrk < _ntracks_bunch; itrk ++){

	if(itrk == NMAXTRACKS) break;

	//If TPC track
	if(!tracks_bunch_exist[itrk]) continue;
	    
	FillDetectorsUsed(itrk, tracks_bunch[itrk].Detectors);
	if(_detUsed[itrk][0] == 1){
	  haveTPC1track = 1;
	}
	if(!_silent) cout << "  Fanny 1: " << haveTPC1track <<" " << _detUsed[itrk][0]<< " " << _evt << " " << ibunch << endl; 

	//For MC
	//-----------------------------------------------------------
       	if(_isMC){
	  if(tracks_bunch[itrk].TrueParticle.PDG != 0){
		
	    _truePur[itrk] = tracks_bunch[itrk].TrueParticle.Pur;
	    _trueMom[itrk] = tracks_bunch[itrk].TrueParticle.InitMom.Vect().Mag();
		
	    for(int c = 0; c < 4; c ++){
	      _truePos[itrk][c] = tracks_bunch[itrk].TrueParticle.InitPos[c];
	    }
	    if(_trueMom[itrk] != 0){
	      for(int c = 0; c < 3; c ++){
		_trueDir[itrk][c] = tracks_bunch[itrk].TrueParticle.InitMom.Vect()[c]/_trueMom[itrk];
	      }
	    }
		
	    _trueCharge[itrk] = tracks_bunch[itrk].TrueParticle.Charge;
	    _truePDG[itrk] = tracks_bunch[itrk].TrueParticle.PDG;
	  }//End if PDG != 0

	  _trueParentPDG[itrk] = tracks_bunch[itrk].TrueParticle.ParentPDG;
	  _trueGParentPDG[itrk] = tracks_bunch[itrk].TrueParticle.GrandParentPDG;

	  bool trajfound = false;

	  //look for the true vertex 
	  //*****************************************
	  int primID;
	  //Loop over trajectory types
	  for(int tt = 0; tt < NTRAJTYPES; tt ++){

	    //Loop over trajectories
	    for(int ii = 0; ii< NTruthTrajs[tt]; ii++){                
	      ND::TTruthTrajectoriesModule::TTruthTrajectory* traj = (ND::TTruthTrajectoriesModule::TTruthTrajectory*)TruthTrajs[tt]->UncheckedAt(ii);

	      if(tracks_bunch[itrk].TrueParticle.ID != traj->TrajID) continue;

	      trajfound = true;
	      primID = traj->PrimaryID;
		
	      if(primID == 0){
		primID = traj->TrajID;
	      }
		
	      //Loop over true vertices
	      for(int v = 0; v < _nTrueVertices; v ++){
		//Loop over primaries
		for(int prim = 0; prim < _nPrimTraj; prim ++){
		  if( _truePrimaryID[v][prim] == primID){
		    _trueVertexIndex[itrk] = v;
		  }
		}//End loop over primaries
	      }//End loop over true vertices

	      break; //Only find one match per trajectory

	    }//End loop over trajectories
	  }//End loop over trajectory types

	  //this can be solved if in the globalreconmodule we add the info of the primID directly
	  if(!trajfound){
	    if(!_silent) cout<<" it has not been found "<<endl;	      	      	    	      
	  }

	  //If no true vertex
	  if(_trueVertexIndex[itrk] == -1){

	    //Find true vertex
	    int tvertex = -1;
	    double dist_min = 1e5;
	    double dist;
	    double dist2;
	    double dist3;

	    //Loop over true vertices
	    for(int v = 0; v < _nTrueVertices; v ++){
	      dist = sqrt(pow(tracks_bunch[itrk].TrueParticle.Vertex.Position[0] - _trueVertexPos[v][0], 2) + 
			  pow(tracks_bunch[itrk].TrueParticle.Vertex.Position[1] - _trueVertexPos[v][1], 2) + 
			  pow(tracks_bunch[itrk].TrueParticle.Vertex.Position[2] - _trueVertexPos[v][2], 2));
		  
	      dist2 = sqrt(pow(tracks_bunch[itrk].TrueParticle.ParentInitPos[0] - _trueVertexPos[v][0], 2) + 
			   pow(tracks_bunch[itrk].TrueParticle.ParentInitPos[1] - _trueVertexPos[v][1], 2) + 
			   pow(tracks_bunch[itrk].TrueParticle.ParentInitPos[2] - _trueVertexPos[v][2], 2));
		  
	      dist3 = sqrt(pow(tracks_bunch[itrk].TrueParticle.GrandParentInitPos[0] - _trueVertexPos[v][0], 2) + 
			   pow(tracks_bunch[itrk].TrueParticle.GrandParentInitPos[1] - _trueVertexPos[v][1], 2) + 
			   pow(tracks_bunch[itrk].TrueParticle.GrandParentInitPos[2] - _trueVertexPos[v][2], 2));
		  
	      if(dist == 0 || dist2 == 0 || dist3 == 0){
		_trueVertexIndex[itrk] = v;
		if(!_silent) cout<<"vertex (method 1) "<<v
				 <<" tr id "<<tracks_bunch[itrk].TrueParticle.ID
				 <<" p "
				 <<tracks_bunch[itrk].TrueParticle.ParentID<<" gp "<<tracks_bunch[itrk].TrueParticle.GrandParentID
				 <<" dist "<<dist<<" dist2 "<<dist2<<" dist3 "<<dist3
				 <<endl;
	      }
	    }//End loop over true vertices
	  }//End if no true vertex
	      
	  if(_trueVertexIndex[itrk] == -1){
	    cerr<<" problem no vertex associated. ibunch ="<<ibunch<<endl;
	  }	     
	  else{
	    _enuWeight[itrk] = _trueEnuWeight[ _trueVertexIndex[itrk]];
	  }

	  //Loop over ECALs
	  for(int jj = 0; jj < tracks_bunch[itrk].NECALs; jj ++){
	    
	    ND::TGlobalReconModule::TECALObject *ecalTrack = (ND::TGlobalReconModule::TECALObject*)(*globalTrack->ECAL)[jj];
	    
	    if(ecalTrack){
	      _trShVal[itrk] = ecalTrack->TrShVal;
	    }
	  }//End loop over ECALs

       	}//End if MC
	//----------------------------------------------------------------------
                          
	_NHits[itrk] = tracks_bunch[itrk].NHits;
	_chi2[itrk] = tracks_bunch[itrk].Quality;
	_det[itrk] = tracks_bunch[itrk].Detectors;
	_mom[itrk] = tracks_bunch[itrk].Momentum;
	_momError[itrk] = tracks_bunch[itrk].MomentumError;
	_charge[itrk] = tracks_bunch[itrk].Charge;
	_nFgds[itrk] = tracks_bunch[itrk].NFGDs;
	    
	if(tracks_bunch[itrk].Momentum != 0){
	  _momRelError[itrk] = tracks_bunch[itrk].MomentumError/tracks_bunch[itrk].Momentum;
	}
	    
	// Position and Direction at vertex
	FillPosDirAtVertex(itrk, 0, 
			   tracks_bunch[itrk].BackPosition,
			   tracks_bunch[itrk].FrontPosition,
			   tracks_bunch[itrk].BackDirection, 
			   tracks_bunch[itrk].FrontDirection, 
			   tracks_bunch[itrk].isForward);

	//---------- PID Pulls and TPC and ECAL variables -----------------------------
	//Loop over TPCs for this track
	for(int jj = 0; jj < tracks_bunch[itrk].NTPCs; jj ++){
	  ND::TGlobalReconModule::TTPCObject *tpcTrack=(ND::TGlobalReconModule::TTPCObject*)(*(tracks_bunch[itrk].TPC))[jj];
	  if(!tpcTrack) continue;

	  _trTpc[itrk][tpcTrack->Detector-1] = tpcTrack->Detector;
	  
	  _trPull[0][itrk][tpcTrack->Detector-1] = tpcTrack->PullMuon;
	  _trPull[1][itrk][tpcTrack->Detector-1] = tpcTrack->PullEle;
	  _trPull[2][itrk][tpcTrack->Detector-1] = tpcTrack->PullProton;
	  _trPull[3][itrk][tpcTrack->Detector-1] = tpcTrack->PullPion;
	  _trPull[4][itrk][tpcTrack->Detector-1] = tpcTrack->PullKaon;	
	  
	  _trCT[itrk][tpcTrack->Detector-1] = tpcTrack->Ccorr;
	  
	  _trCTexp[0][itrk][tpcTrack->Detector-1] = tpcTrack->dEdxexpMuon;
	  _trCTexp[1][itrk][tpcTrack->Detector-1] = tpcTrack->dEdxexpEle;
	  _trCTexp[2][itrk][tpcTrack->Detector-1] = tpcTrack->dEdxexpProton;
	  _trCTexp[3][itrk][tpcTrack->Detector-1] = tpcTrack->dEdxexpPion;
	  _trCTexp[4][itrk][tpcTrack->Detector-1] = tpcTrack->dEdxexpKaon;
	  
	  _trCTerr[0][itrk][tpcTrack->Detector-1] = tpcTrack->SigmaMuon;
	  _trCTerr[1][itrk][tpcTrack->Detector-1] = tpcTrack->SigmaEle;
	  _trCTerr[2][itrk][tpcTrack->Detector-1] = tpcTrack->SigmaProton;
	  _trCTerr[3][itrk][tpcTrack->Detector-1] = tpcTrack->SigmaPion;
	  _trCTerr[4][itrk][tpcTrack->Detector-1] = tpcTrack->SigmaKaon;
	  
	  if(_isMC) _tpcNHits[itrk][tpcTrack->Detector-1]  = tpcTrack->NHits;
	  else _tpcNHits[itrk][tpcTrack->Detector-1]  = (tpcTrack->Ndof) + 8;
	  
	  _tpcCharge[itrk][tpcTrack->Detector-1] = tpcTrack->Charge;
	  _tpcMom[itrk][tpcTrack->Detector-1] = tpcTrack->Momentum;
	  _tpcMomRelError[itrk][tpcTrack->Detector-1] = tpcTrack->MomentumError/tpcTrack->Momentum;

	}//End loop over TPCs
      }// end loop over the tracks in this bunch
      //-------------------------------------------------------------------
	  
      //Fill _SpillWithTPC1Track  
      _SpillWithTPC1Track = haveTPC1track;

      if(!_silent) cout << "     Fanny 2: " << _SpillWithTPC1Track << " " << _evt <<endl;      

      // apply corrections to get centered pulls
      ApplyPIDCorrections(RunID);
	  
      // convert pulls into probabilities
      ComputePIDProbabilities();
	  
      //2 tracks analysis
      FillTwoTracksVariables(tracks_bunch_exist[0], tracks_bunch_exist[1]);
	  	  
      int fillcatch = _mini->Fill();
	  
    }  // End loop over bunches     
  }  // End of event loop
  //------------------------------------------------------------------------  
  
  // final time
  timer.Stop();
  
  if(!_silent) cout << "Processing time: " << timer.CpuTime() << " " << timer.RealTime() << endl;       
  
  //Write tree to file
  //---------------------------------------------------------------------------
  if(!_silent) cout<<"Writing file: "<<outfileName<<endl;

  outputFile->cd();
  
  _mini->Write();
  
  outputFile->Close();
  if(!_silent) cout<<"--------Done--------"<<endl;
}
//=======================================================================================


