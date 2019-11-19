#include "TRandom3.h"

//***********************************************************************
//Include file for RunMiniTree.C
//***********************************************************************

//Global parameters
//_______________________________________________________________________
const int NMAXTRAJ = 200;
const int NMAXTRUEVERTICES = 50;
const int NMAXCLUSTERS = 50;
const int NVTXTYPES = 11;
const int NTRAJTYPES = 7;
const int NBUNCH = 9;
const int NANACUTS = 5;
const int NRUNS = 4;
const int NHYPO = 5;
const int NMAXTRACKS = 10;

//Cuts
const double _michelCut[5] = {400., 600., 800., 1200., 1600.};
const int _cutNbr = 0;

//cutNbr meaning:
//cutNbr = 0 --> no cut
//cutNbr = 1 --> 0.3
//cutNbr = 2 --> 0.2
//cutNbr = 3 --> 0.15
//cutNbr = 4 --> 0.1

//Global objects
//_______________________________________________________________________
TRandom3 fRandomGenerator; 
TTree *_mini;
bool _isMC;
bool _silent;

//Internal veriables
int _nPrimTraj;
int _truePrimaryID[NMAXTRUEVERTICES][NMAXTRAJ];
double _trueLeptonEnergy[NMAXTRUEVERTICES];
TVector3 _dirV[NMAXTRACKS];
double _trPullCorr[NHYPO][NMAXTRACKS][3];

// Tree variables
//_______________________________________________________________________

//Filled for each spill
int _trueSpillInfo;
int _evt;
int _run;
int _filerun;
int _filesubrun;
int _entry;
int _nbunches;
int _nvalidbunches;
int _bunchnbr;
int _SpillWithTPC1Track;
int _nTrueVertices; 
int _reacCode[NMAXTRUEVERTICES];
double _trueVertexPos[NMAXTRUEVERTICES][4];
double _trueNuEnergy[NMAXTRUEVERTICES];
double _trueNuDir[NMAXTRUEVERTICES][3];
int    _trueNuPDG[NMAXTRUEVERTICES];
double _trueEnuWeight[NMAXTRUEVERTICES];
int _trueVertexBunch[NMAXTRUEVERTICES];
double _trueLeptonMom[NMAXTRUEVERTICES];
double _trueProtonMom[NMAXTRUEVERTICES];
double _trueTargetMom[NMAXTRUEVERTICES];
double _trueLeptonDir[NMAXTRUEVERTICES][3];
double _trueProtonDir[NMAXTRUEVERTICES][3];
double _trueTargetDir[NMAXTRUEVERTICES][3];
double _trueTargetPDG[NMAXTRUEVERTICES];
int _trueBaryonSize[NMAXTRUEVERTICES];
double _trueQ2[NMAXTRUEVERTICES];
int _trueVertexDet[NMAXTRUEVERTICES];
int _necaltracks;
int _np0dtracks;
int _nsmrdtracks;
int _nfgdtracks;
int _ntpctracks;
int _nFGDUnused[2];
int _ntracks_total[NANACUTS];
int _nTpctracks_total[3][NANACUTS];

//Filled for each bunch
int _BunchWithMaxMomInSpill;
double _tbunch;
int _bunch;
int _newbunch;
int _ntracks_bunch;
double _vertex[4][4];
int _nTrueVerticesPerBunch;
int _totaltracks;
int _ntracks[NANACUTS];
int _npostracks[NANACUTS];
int _nnegtracks[NANACUTS];
int _ntpctracks_bunch[3][NANACUTS];
int _det[NMAXTRACKS];
int _detUsed[NMAXTRACKS][9];
int _NHits[NMAXTRACKS];
double _chi2[NMAXTRACKS];
double _pos[NMAXTRACKS][4];
double _posEnd[NMAXTRACKS][4];
double _dir[NMAXTRACKS][3];
double _mom[NMAXTRACKS];
double _momError[NMAXTRACKS];
double _momRelError[NMAXTRACKS];
int    _charge[NMAXTRACKS];
double _cosTheta[NMAXTRACKS];
double _trShVal[NMAXTRACKS];
double _enuWeight[NMAXTRACKS];
int _nFgds[NMAXTRACKS];
int _trTpc[NMAXTRACKS][3];
double _trCT[NMAXTRACKS][3];
double _trCTCorr[NMAXTRACKS][3];
double _trProb[NHYPO][NMAXTRACKS];
double _trPull[NHYPO][NMAXTRACKS][3];
double _trCTexp[NHYPO][NMAXTRACKS][3];
double _trCTerr[NHYPO][NMAXTRACKS][3];
double _tpcCharge[NMAXTRACKS][3];
double _tpcMom[NMAXTRACKS][3];
double _tpcMomRelError[NMAXTRACKS][3];
int _tpcNHits[NMAXTRACKS][3];
int _nfgdtracks_bunch;
int _fgddet[NMAXTRACKS];
int _fgdNHits[NMAXTRACKS];
double _fgdchi2[NMAXTRACKS];
double _fgdpos[NMAXTRACKS][4];
double _fgdposEnd[NMAXTRACKS][4];
double _fgddir[NMAXTRACKS][3];
double _eNuRec[NMAXTRACKS];
double _deltaThetaP[NMAXTRACKS];
double _nuInvariantMass;
double _emu[NMAXTRACKS];
double _ep;
double _cosThetaF;
double _mf;
double _mf2;
double _ef;
double _pf;
double _pQ2[NMAXTRACKS];
double _muQ2[NMAXTRACKS];
double _earliestTrackTime;
int _nclusters;
int _nclusterscut[3][NANACUTS];
double _clusterPos[NMAXCLUSTERS][4];
double _clusterRMS[NMAXCLUSTERS][4];
int    _clusterNHits[NMAXCLUSTERS];
double _clusterCharge[NMAXCLUSTERS];
double _dist[NMAXCLUSTERS][3];
double _tdist[NMAXCLUSTERS][3];
int _truePDG[NMAXTRACKS];
int _trueParentPDG[NMAXTRACKS];
int _trueGParentPDG[NMAXTRACKS];
double _truePur[NMAXTRACKS];
double _truePos[NMAXTRACKS][4];
double _trueDir[NMAXTRACKS][3];
double _trueMom[NMAXTRACKS];
int _trueCharge[NMAXTRACKS];
int _trueVertexIndex[NMAXTRACKS];


//_______________________________________________________________________

