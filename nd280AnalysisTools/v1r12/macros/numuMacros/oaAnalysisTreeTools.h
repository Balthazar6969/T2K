#ifndef oaAnalysisTreeTools_h
#define oaAnalysisTreeTools_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include "TreeToolsBase.h"
#include "oaAnalysisTreeDefinition.h"


#include <AnalysisTools/libReadoaAnalysis/libReadoaAnalysisProjectHeaders.h>


class oaAnalysisTreeTools: public oaAnalysisTreeDefinition, public TreeToolsBase {
public :

  oaAnalysisTreeTools(){}
  oaAnalysisTreeTools(const std::string& file);
  void AddFileToTChain(const std::string& inputString);

  ND::TGlobalReconModule::TGlobalPID* GetGlobalPID(int itrk){
    if (itrk>=_NPIDs) return NULL;
    ND::TGlobalReconModule::TGlobalPID* pid = (ND::TGlobalReconModule::TGlobalPID*)(*_PIDs)[itrk];
    if (pid->NTPCs>0) return pid;
    else return NULL;
  }


  ND::TGlobalReconModule::TTPCObject* GetTPCObject(int itrk, int tpc){
    ND::TGlobalReconModule::TGlobalPID* track = GetGlobalPID(itrk);    
    if (track){
      for(int jj = 0; jj < track->NTPCs; jj ++){
	ND::TGlobalReconModule::TTPCObject *tpcTrack=(ND::TGlobalReconModule::TTPCObject*)(*(track->TPC))[jj];
	if(!tpcTrack) continue;
	if (tpcTrack->Detector == tpc+1) return tpcTrack;
      }
    }
    return NULL;
  }

  ND::TGlobalReconModule::TFGDObject* GetFGDObject(int itrk, int fgd){
    ND::TGlobalReconModule::TGlobalPID* track = GetGlobalPID(itrk);    
    if (track){
      for(int jj = 0; jj < track->NFGDs; jj ++){
	ND::TGlobalReconModule::TFGDObject *fgdTrack=(ND::TGlobalReconModule::TFGDObject*)(*(track->FGD))[jj];
	if(!fgdTrack) continue;
	if (fgdTrack->Detector == fgd+1) return fgdTrack;
      }
    }
    return NULL;
  }


  ND::TTrueParticle* GetTrueParticle(int itrk){
    ND::TGlobalReconModule::TGlobalPID* track = GetGlobalPID(itrk);    
    if (!track) return NULL;
    if (track->TrueParticle.PDG!=0)
      return &(track->TrueParticle);
    else 
      return NULL;
  }


  // ---- Event info -------------

  bool GetIsMC(){return (GetRun()==0);}

  bool GetIsGoodSpill(){return true;}
  bool GetHasGoodDQ(){return true;}

  int  GetEvent(){return _EventID;}
  int  GetRun(){return _RunID;}
  int  GetSubrun(){return _SubrunID;}
  int  GetFileRun(){return 0;}
  int  GetFileSubrun(){return 0;}

  // ---- counters -------------

  int GetNTracksWithTPC(){return _NPIDs;}
  int GetNTracksWithTPCOther(){return _NTPCOthers;}
  int GetNTracksWithFGD(){return 0;}
  int GetNDelayedClusters(){return _NDelayedClusters;}

  int GetNTPCIsoTracks() {return _NTPC;}
  int GetNECALIsoTracks(){return _NECAL;}
  int GetNP0DIsoTracks() {return _NP0D;}
  int GetNFGDIsoTracks() {return _NFGD;}
  int GetNSMRDIsoTracks(){return _NSMRD;}
  
  int GetNFGDUnusedHits(int fgd){
    if (fgd==1) return _NFGD1Unused;
    else if (fgd==2) return _NFGD1Unused;
    else return 0;
  }

  // ----- true vertex information --------  

  int GetTrueReacCode(int ivtx){return 0;}
  double* GetTrueVertexPosition(int ivtx){return 0;}
  int GetTrueNuPDG(int ivtx){return 0;}
  double* GetTrueNuDir(int ivtx){return 0;}
  double GetTrueNuDir(int ivtx, int comp){return 0;}
  double GetTrueNuEnergy(int ivtx){return 0;}
  
  // ----- global track truth information --------

  double GetTruePurity(int itrk){if (GetTrueParticle(itrk)) return GetTrueParticle(itrk)->Pur;    else return 0;}
  int GetTrueCharge(int itrk)   {if (GetTrueParticle(itrk)) return GetTrueParticle(itrk)->Charge; else return 0;}
  double* GetTrueDirection(int itrk){
    ND::TTrueParticle* part = GetTrueParticle(itrk);
    if (part){
      for(int c = 0; c < 3; c ++)
	_trueDir[c] = part->InitMom.Vect()[c]/part->InitMom.Vect().Mag();
    }
    else{
      _trueDir[0]=_trueDir[1]=_trueDir[2]=-9999.;
    }
    return _trueDir;
  }

  double GetTrueDirection(int itrk, int comp){return GetTrueDirection(itrk)[comp];}
  double GetTrueMomentum(int itrk){if (GetTrueParticle(itrk)) return GetTrueParticle(itrk)->InitMom.Vect().Mag(); else return -9999.;}
  
  int GetPDG(int itrk)            {if (GetTrueParticle(itrk)) return GetTrueParticle(itrk)->PDG;            else return 0;}
  int GetParentPDG(int itrk)      {if (GetTrueParticle(itrk)) return GetTrueParticle(itrk)->ParentPDG;      else return 0;}
  int GetGrandParentPDG(int itrk) {if (GetTrueParticle(itrk)) return GetTrueParticle(itrk)->GrandParentPDG; else return 0;}
  
  int GetTrueVertexIndex(int itrk){return -1;}   
  int GetTrueVertexDet(int itrk)  {return 0;}
  
  // ---- global track variables --------------
  
  virtual double GetCharge(int itrk)                        {if (GetGlobalPID(itrk)) return GetGlobalPID(itrk)->Charge; else return -9999.;}
  virtual double GetMomentum(int itrk)                      {if (GetGlobalPID(itrk)) return GetGlobalPID(itrk)->FrontMomentum; else return -9999.;}
  virtual double GetMomentumRelError(int itrk)              {if (GetGlobalPID(itrk)) return GetGlobalPID(itrk)->FrontMomentumError/GetGlobalPID(itrk)->FrontMomentum; else return -9999.;}
  virtual double GetMomentumRelErrorLocal(int itrk, int tpc){if (GetGlobalPID(itrk)) return GetGlobalPID(itrk)->FrontMomentumError/GetGlobalPID(itrk)->FrontMomentum; else return -9999.;}
  virtual double GetMomentumLocal(int itrk)                 {if (GetGlobalPID(itrk)) return GetGlobalPID(itrk)->FrontMomentum; else return -9999.;}
  
  double GetCosTheta(int itrk){if (GetGlobalPID(itrk)) return 0; else return -9999.;}
  double* GetPositionStart(int itrk){
    ND::TGlobalReconModule::TGlobalPID* track = GetGlobalPID(itrk);
    if (track){
      for(int c = 0; c < 4; c ++)
	_pos[c]=track->FrontPosition[c];
    }
    else{
      _pos[0]=_pos[1]=_pos[2]=_pos[3]-9999.;
    }
    return _pos;
  }
  double* GetDirection(int itrk){
    ND::TGlobalReconModule::TGlobalPID* track = GetGlobalPID(itrk);
    if (track){
      for(int c = 0; c < 3; c ++)
	_dir[c]=track->FrontDirection[c];
    }
    else{
      _dir[0]=_dir[1]=_dir[2]=-9999.;
    }
    return _dir;
  }
  
  double GetDirection(int itrk, int comp){if (GetGlobalPID(itrk)) return GetGlobalPID(itrk)->FrontDirection[comp]; else return -9999.;}
  bool GetIsForward(int itrk)            {if (GetGlobalPID(itrk)) return (GetGlobalPID(itrk)->FrontDirection[2]>0); else return -9999.;}
  
  
  int GetNDOF(int itrk)   {if (GetGlobalPID(itrk)) return GetGlobalPID(itrk)->NDOF;  else return 0;}
  int GetNHits(int itrk)  {if (GetGlobalPID(itrk)) return GetGlobalPID(itrk)->NHits; else return 0;}
  double GetChi2(int itrk){if (GetGlobalPID(itrk)) return GetGlobalPID(itrk)->Chi2;  else return -9999.;}
  
  bool   GetDetUsed(int itrk, int det){if (GetGlobalPID(itrk)) return (bool)GetGlobalPID(itrk)->DetectorUsed[det]; else return false;}
  double GetECALTrShVal(int itrk){return 0;}
  
  //---- TPC variables -----
  int    GetTpcNHits(int itrk, int tpc){if (GetTPCObject(itrk,tpc)) return GetTPCObject(itrk,tpc)->NHits; else return 0;}
  int    GetTpcNHits(int itrk){return GetTpcNHits(itrk,0)+GetTpcNHits(itrk,1)+GetTpcNHits(itrk,2);}
  
  virtual double GetTpcMomentum(int itrk, int tpc){if (GetTPCObject(itrk,tpc)) return GetTPCObject(itrk,tpc)->FrontMomentum; else return -9999.;}
  virtual double GetTpcMomentumRelError(int itrk, int tpc){if (GetTPCObject(itrk,tpc)) return GetTPCObject(itrk,tpc)->FrontMomentumError/GetTPCObject(itrk,tpc)->FrontMomentum; else return -9999.;}
  double GetTpcMomentumError(int itrk, int tpc){if (GetTPCObject(itrk,tpc)) return GetTPCObject(itrk,tpc)->FrontMomentumError; else return -9999.;}
  
  virtual double GetTpcPullmu(int itrk, int tpc){if (GetTPCObject(itrk,tpc)) return GetTPCObject(itrk,tpc)->PullMuon; else return -9999.;}
  double GetTpcPullp(int itrk, int tpc)  {if (GetTPCObject(itrk,tpc)) return GetTPCObject(itrk,tpc)->PullProton;      else return -9999.;}
  double GetTpcPullele(int itrk, int tpc){if (GetTPCObject(itrk,tpc)) return GetTPCObject(itrk,tpc)->PullEle;         else return -9999.;}
  double GetTpcPullk(int itrk, int tpc)  {if (GetTPCObject(itrk,tpc)) return GetTPCObject(itrk,tpc)->PullKaon;        else return -9999.;}
  double GetTpcPullpi(int itrk, int tpc) {if (GetTPCObject(itrk,tpc)) return GetTPCObject(itrk,tpc)->PullPion;        else return -9999.;}
  
  virtual double GetTpcCT(int itrk, int tpc)      {if (GetTPCObject(itrk,tpc)) return GetTPCObject(itrk,tpc)->Ccorr;         else return -9999.;}
  virtual double GetTpcCTexpmu(int itrk, int tpc) {if (GetTPCObject(itrk,tpc)) return GetTPCObject(itrk,tpc)->dEdxexpMuon;   else return -9999.;}
  virtual double GetTpcCTexppi(int itrk, int tpc) {if (GetTPCObject(itrk,tpc)) return GetTPCObject(itrk,tpc)->dEdxexpPion;   else return -9999.;}
  virtual double GetTpcCTexpp(int itrk, int tpc)  {if (GetTPCObject(itrk,tpc)) return GetTPCObject(itrk,tpc)->dEdxexpProton; else return -9999.;}
  virtual double GetTpcCTexpele(int itrk, int tpc){if (GetTPCObject(itrk,tpc)) return GetTPCObject(itrk,tpc)->dEdxexpEle;    else return -9999.;}
  virtual double GetTpcCTerrmu(int itrk, int tpc) {if (GetTPCObject(itrk,tpc)) return GetTPCObject(itrk,tpc)->SigmaMuon;     else return -9999.;}
  virtual double GetTpcCTerrpi(int itrk, int tpc) {if (GetTPCObject(itrk,tpc)) return GetTPCObject(itrk,tpc)->SigmaPion;     else return -9999.;}
  virtual double GetTpcCTerrp(int itrk, int tpc)  {if (GetTPCObject(itrk,tpc)) return GetTPCObject(itrk,tpc)->SigmaProton;   else return -9999.;}
  virtual double GetTpcCTerrele(int itrk, int tpc){if (GetTPCObject(itrk,tpc)) return GetTPCObject(itrk,tpc)->SigmaEle;      else return -9999.;}
  
  //------- FGD variables ------
  double GetFgdPullp(int itrk, int fgd) {if (GetFGDObject(itrk,fgd)) return GetFGDObject(itrk,fgd)->PullProton[fgd]; else return -9999;}
  double GetFgdPullmu(int itrk, int fgd){if (GetFGDObject(itrk,fgd)) return GetFGDObject(itrk,fgd)->PullMuon[fgd];   else return -9999;}
  double GetFgdPullno(int itrk, int fgd){if (GetFGDObject(itrk,fgd)) return GetFGDObject(itrk,fgd)->PullNotSet[fgd]; else return -9999;}
  double GetFgdPullpi(int itrk, int fgd){if (GetFGDObject(itrk,fgd)) return GetFGDObject(itrk,fgd)->PullPion[fgd];   else return -9999;}
  
 protected:
  
  double _pos[4];
  double _dir[3];
  double _trueDir[3];
  

};

#endif


