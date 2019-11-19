#ifndef NumuFlatTreeTools_h
#define NumuFlatTreeTools_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include "NumuFlatTreeDefinition.h"
#include "TreeToolsBase.h"
#include "TRandom3.h"

class NumuFlatTreeTools: public NumuFlatTreeDefinition, public TreeToolsBase {
 public :
  
  NumuFlatTreeTools(){}
  NumuFlatTreeTools(const std::string& file);
  void AddFileToTChain(const std::string& inputString);


  TChain* GetChain(){return dynamic_cast<TChain*>(fChain);}

    
  // ---- Event info -------------

  bool GetIsMC(){return (GetRun()==0);}

  bool GetIsGoodSpill(){return (bool)std_GoodSpill;}
  bool GetHasGoodDQ(){return (bool)std_GoodDaq;}

  int  GetEvent(){return std_Evt;}
  int  GetRun(){return std_Run;}
  int  GetSubrun(){return std_Subrun;}
  int  GetFileRun(){return std_FileRun;}
  int  GetFileSubrun(){return std_FileSubrun;}

  // ---- counters -------------

  int GetNTracksWithTPC(){return stdB_NTracksWithTPC;}
  int GetNTracksWithTPCOther(){return stdB_NTracksWithTPCOther;}
  int GetNTracksWithFGD(){return stdB_NTracksWithFGD;}
  int GetNDelayedClusters(){return stdB_NClusters;}

  int GetNTPCIsoTracks(){return std_NTPCTracks;}
  int GetNECALIsoTracks(){return std_NECALTracks;}
  int GetNP0DIsoTracks(){return std_NP0DTracks;}
  int GetNFGDIsoTracks(){return std_NFGDTracks;}
  int GetNSMRDIsoTracks(){return std_NSMRDTracks;}
  
  int GetNFGDUnusedHits(int fgd){return std_NFGDUnusedHits[fgd-1];}

  // ----- true vertex information --------  

  int GetTrueReacCode(int ivtx){return std_TrueReacCode[ivtx];}
  double* GetTrueVertexPosition(int ivtx){return std_TrueVertexPos[ivtx];}
  int GetTrueNuPDG(int ivtx){return std_TrueNuPDG[ivtx];}
  double* GetTrueNuDir(int ivtx){return std_TrueNuDir[ivtx];}
  double GetTrueNuDir(int ivtx, int comp){return std_TrueNuDir[ivtx][comp];}
  double GetTrueNuEnergy(int ivtx){return std_TrueNuEnergy[ivtx];}
  
  // ----- global track truth information --------

   double GetTruePurity(int itrk){return stdB_trTruePur[itrk];}
   int GetTrueCharge(int itrk){return stdB_trTrueCharge[itrk];}
   double* GetTrueDirection(int itrk){return stdB_trTrueDirection[itrk];}
   double GetTrueDirection(int itrk, int comp){return stdB_trTrueDirection[itrk][comp];}
   double GetTrueMomentum(int itrk){return stdB_trTrueMomentum[itrk];}

   int GetPDG(int itrk){return stdB_trTruePDG[itrk];}
   int GetParentPDG(int itrk){return stdB_trTrueParentPDG[itrk];}
   int GetGrandParentPDG(int itrk){return stdB_trTrueGParentPDG[itrk];}

   int GetTrueVertexIndex(int itrk){return stdB_trTrueVertexIndex[itrk];}   
   int GetTrueVertexDet(int itrk){
     if(stdB_trTrueVertexIndex[itrk]>-1) 
       return std_TrueVertexDet[stdB_trTrueVertexIndex[itrk]];
     else 
       return 0;
   }

   // ---- global track variables --------------

   virtual double GetCharge(int itrk){return stdB_trCharge[itrk];}
   virtual double GetMomentum(int itrk){return stdB_trMom[itrk];}
   virtual double GetMomentumRelError(int itrk){return stdB_trMomRelErr[itrk];}
   virtual double GetMomentumRelErrorLocal(int itrk, int tpc){return ((stdB_trTpcMomRelErr[itrk][tpc]*stdB_trTpcMom[itrk][tpc])/stdB_trMomLocalTpc[itrk]);}
   virtual double GetMomentumLocal(int itrk){return stdB_trMomLocalTpc[itrk];}

   double GetCosTheta(int itrk){return stdB_trCosTheta[itrk];}
   double* GetPositionStart(int itrk){return stdB_trPositionStart[itrk];}
   double* GetDirection(int itrk){return stdB_trDir[itrk];}
   double GetDirection(int itrk, int comp){return stdB_trDir[itrk][comp];}
   double GetNHits(int itrk){return stdB_trNHits[itrk];}

   double GetPositionStart(int itrk, int comp){return stdB_trPositionStart[itrk][comp];}
   double GetPositionEnd(int itrk, int comp){return stdB_trPositionEnd[itrk][comp];}

   int GetNDOF(int itrk){return stdB_trNDOF[itrk];}
   double GetChi2(int itrk){return stdB_trChi2[itrk];}

   bool   GetDetUsed(int itrk, int det){return (bool)stdB_trDetUsed[itrk][det];}
   double GetECALTrShVal(int itrk){return stdB_trECALTrShVal[itrk];}

   bool IsForward(int itrk){ return (stdB_trDir[itrk][2]>0);}

   //---- TPC variables -----

   int    GetTpcNHits(int itrk, int tpc){return stdB_trTpcNHits[itrk][tpc];}
   int    GetTpcNHits(int itrk){return stdB_trTpcNHits[itrk][0]+stdB_trTpcNHits[itrk][1]+stdB_trTpcNHits[itrk][2];}

   int    GetFgdNHits(int itrk){return stdB_trFgdNHits[itrk][0]+stdB_trFgdNHits[itrk][1];} 
   

   virtual double GetTpcMomentum(int itrk, int tpc){return stdB_trTpcMom[itrk][tpc];}
   virtual double GetTpcMomentumRelError(int itrk, int tpc){return stdB_trTpcMomRelErr[itrk][tpc];}
   double GetTpcMomentumError(int itrk, int tpc){return stdB_trTpcMom[itrk][tpc]*stdB_trTpcMomRelErr[itrk][tpc];}

   virtual double GetTpcPullmu(int itrk, int tpc){return stdB_trPullmu[itrk][tpc];}
   double GetTpcPullp(int itrk, int tpc){return stdB_trPullp[itrk][tpc];}
   double GetTpcPullele(int itrk, int tpc){return stdB_trPullele[itrk][tpc];}
   double GetTpcPullk(int itrk, int tpc){return stdB_trPullk[itrk][tpc];}
   double GetTpcPullpi(int itrk, int tpc){return stdB_trPullpi[itrk][tpc];}
   
   virtual double GetTpcCT(int itrk, int tpc){return stdB_trCT[itrk][tpc];}
   virtual double GetTpcCTexpmu(int itrk, int tpc){return stdB_trCTexpmu[itrk][tpc];}
   virtual double GetTpcCTexppi(int itrk, int tpc){return stdB_trCTexppi[itrk][tpc];}
   virtual double GetTpcCTexpp(int itrk, int tpc){return stdB_trCTexpp[itrk][tpc];}
   virtual double GetTpcCTexpele(int itrk, int tpc){return stdB_trCTexpele[itrk][tpc];}
   virtual double GetTpcCTerrmu(int itrk, int tpc){return stdB_trCTerrmu[itrk][tpc];}
   virtual double GetTpcCTerrpi(int itrk, int tpc){return stdB_trCTerrpi[itrk][tpc];}
   virtual double GetTpcCTerrp(int itrk, int tpc){return stdB_trCTerrp[itrk][tpc];}
   virtual double GetTpcCTerrele(int itrk, int tpc){return stdB_trCTerrele[itrk][tpc];}

   //------- FGD variables ------
   double GetFgdPullp(int itrk, int fgd){return stdB_trFgdPullp[itrk][fgd];}
   double GetFgdPullmu(int itrk, int fgd){return stdB_trFgdPullmu[itrk][fgd];}
   double GetFgdPullno(int itrk, int fgd){return stdB_trFgdPullno[itrk][fgd];}
   double GetFgdPullpi(int itrk, int fgd){return stdB_trFgdPullpi[itrk][fgd];}



};

#endif


