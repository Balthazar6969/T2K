#define AnalysisBase_cxx
#include "AnalysisBase.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TVector3.h>
#include <TLegend.h>
#include <stdio.h>
#include <iostream>



TVector3 FVdefminFGD1(100,100,7.4);
TVector3 FVdefmaxFGD1(100,100,0);
TVector3 FVdefminFGD2(100,100,7.4);
TVector3 FVdefmaxFGD2(100,100,0);

TVector3 FVdefminP0D(250,250,100.);
TVector3 FVdefmaxP0D(250,250,0);

TVector3 fgd1min(-932.17,-877.17,115.95);
TVector3 fgd1max(932.17,987.17,447.05);
TVector3 fgd2min(-932.17,-877.17,1473.95);
TVector3 fgd2max(932.17,987.17,1807.05);

TVector3 p0dmin(-1092.79,-1107.39,-3296.48);
TVector3 p0dmax(1012.45,1130.99,-938.753);
TVector3 ecalmin(-1000,-1000,2887);
TVector3 ecalmax(1000,1000,3315);
TVector3 smrdLmin(-2848.99,-3098,-3898);
TVector3 smrdLmax(-1832.99,3098,3898);
TVector3 smrdRmin(1832.99,-3098,-3898);
TVector3 smrdRmax(2848.99,3098,3898);
TVector3 smrdDmin(-1832.99,-3098,-3898);
TVector3 smrdDmax(1832.99,-1770.99,3898);
TVector3 smrdUmin(-1832.99,1770.99,-3898);
TVector3 smrdUmax(1832.99,3098,3898);

TVector3 tpc1min(-1092.79,-1107.39,-938.753);
TVector3 tpc1max(1012.45,1130.99,116.045);
TVector3 tpc2min(-1092.79,-1107.39,446.955);
TVector3 tpc2max(1012.45,1130.99,1474.05);
TVector3 tpc3min(-1092.79,-1107.39,1806.96);
TVector3 tpc3max(1012.45,1130.99,2887);


//********************************************************************
AnalysisBase::AnalysisBase(const std::string& file):SystematicsAnalysis(file){
//********************************************************************

}


//********************************************************************
void AnalysisBase::Initialize(){
//********************************************************************

  SystematicsAnalysis::Initialize();


}


//********************************************************************
void AnalysisBase::Finalize(){
//********************************************************************

  DrawingTools::Finalize();

}

//********************************************************************
void AnalysisBase::InitializeEvent(){
//********************************************************************

  SystematicsAnalysis::InitializeEvent();
  DrawingTools::InitializeEvent();

  _like_computed.resize(GetTreeTools().GetNTracksWithTPC());
  _likelihood.resize(GetTreeTools().GetNTracksWithTPC());
  for (int i=0;i<GetTreeTools().GetNTracksWithTPC();i++){
    _like_computed[i]=false;
    _likelihood[i].resize(4);
    for (int j=0;j<4;j++)
      _likelihood[i][j]=-1;
  }

}

//********************************************************************
void AnalysisBase::InitializeConfiguration(){
//********************************************************************

  DrawingTools::InitializeConfiguration();

}


//********************************************************************
void AnalysisBase::InitializeAnalysis(){
//********************************************************************

  for (int i=0;i<NMAXCUTS;i++)
    _cut_passed[i]=false;

  _accum_cut_level=0;

}

//********************************************************************
void AnalysisBase::FinalizeAnalysis(){
//********************************************************************

  DrawingTools::FinalizeAnalysis();
}

//********************************************************************
void AnalysisBase::FinalizeConfiguration(){
//********************************************************************

  DrawingTools::FinalizeConfiguration();
}

  
//********************************************************************
void AnalysisBase::Loop(int nmax,int imin){
//********************************************************************


  if (GetTreeTools().GetChain() == 0) return;
  
  Initialize();

  
  Long64_t nentries = GetTreeTools().GetChain()->GetEntriesFast();
  
   if (nmax==0 || imin+nmax>nentries) nmax=nentries;

   //--------- Loop over events ----------------------------------

   std::cout << "loop over " << nmax << " entries from entry number "<< imin << std::endl;

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=imin; jentry<imin+nmax;jentry++) {
     Long64_t ientry = GetTreeTools().LoadTree(jentry);
     if (ientry < 0) break;
      nb = GetTreeTools().GetEntry(jentry);   nbytes += nb;

      if (jentry%10000 == 0) std::cout << "entry: " << jentry << std::endl; 


      InitializeEvent();

      //---- For each event loop over configurations ----
      std::map< std::string, std::map< std::string, bool > >::iterator it;
      for (it= GetConfigurations().begin();it!=GetConfigurations().end();it++){
	if (!ConfigurationEnabled(it->first)) continue;
	SetCurrentConfiguration(it->first);
	InitializeConfiguration();

	//---- For each configuration loop over analyses --------
	for (int n=0;n<GetNAnalyses();n++){
	  SetAnalysisIndex(n);
	  InitializeAnalysis();
	  Process();      
	  FinalizeAnalysis();
	}
	FinalizeConfiguration();
      }      
      FinalizeEvent();
   }


   Finalize();

}

//**************************************************
bool AnalysisBase::TrackQualityCut(int itrk){
//**************************************************

  int tpc= GetClosestTPC(itrk);
  if (tpc>-1){
    double cut = 18;

    if (GetTreeTools().GetTpcNHits(itrk,tpc)> cut) return true;

  }

  return false;

}

//**************************************************
int AnalysisBase::GetClosestTPC(int itrk){
//**************************************************

  // returns the TPC closer to the vertex in the direction of the track (0,1,2)

  int tpc=-1;
  //if (FiducialCut("FGD1",GetTreeTools().GetPositionStart(itrk))){
  if (InDetVolume("FGD1",GetTreeTools().GetPositionStart(itrk))){
    if (GetTreeTools().GetDetUsed(itrk,0))
      tpc=0;
    else if (GetTreeTools().GetDetUsed(itrk,1))
      tpc=1;
  } 
  //else if (FiducialCut("FGD2",GetTreeTools().GetPositionStart(itrk))){
  else if (InDetVolume("FGD2",GetTreeTools().GetPositionStart(itrk))){
    if (GetTreeTools().GetDetUsed(itrk,2))
      tpc=2;
    else if (GetTreeTools().GetDetUsed(itrk,1))
      tpc=1;
  }

  return tpc;

}

//**************************************************
bool AnalysisBase::MuonPIDCutPull(int itrk){
//**************************************************

  double cut1 = 3.;
  double cut2 = 2.;
  double pcut = 500;
  bool found = false;

  for (int tpc=0;tpc<3;tpc++){
    if (!GetTreeTools().GetDetUsed(itrk,tpc)) continue;

    found = true;

    if (fabs(GetTpcPullmu(itrk,tpc))>cut1 || 
	(fabs(GetTpcPullele(itrk,tpc))<cut2 && GetTpcMomentum(itrk,tpc)<pcut)) return false;
  }

  return found;

}


//**************************************************
bool AnalysisBase::MuonPIDCutLike(int itrk){
//**************************************************

  double cut1 = 0.8;
  double cut2 = 0.05;
  double pcut = 500;
  
  if (((GetPIDLikelihood(itrk,0)+GetPIDLikelihood(itrk,3))/(1-GetPIDLikelihood(itrk,2)) > cut1  || GetTreeTools().GetMomentumLocal(itrk) ) &&
      //if (((GetPIDLikelihood(itrk,0)+GetPIDLikelihood(itrk,3))/(1-GetPIDLikelihood(itrk,2)) > cut1  || GetMomentum(itrk)>pcut ) &&
      //      (GetPIDLikelihood(itrk,0)>cut2 || GetTreeTools().GetMomentumLocal(itrk)>pcut)) return true; 
      (GetPIDLikelihood(itrk,0)>cut2)) return true; 

  return false;
}


//**************************************************
bool AnalysisBase::ProtonPIDCutPull(int itrk){
//**************************************************
  
  //if ((g10aB_trProbmu(itrk)+g10aB_trProbpi(itrk))/(1-g10aB_trProbk(itrk)-g10aB_trProbp(itrk)+0.000001)<0.9) continue; 

  if  ((GetTreeTools().GetDetUsed(itrk,0)==1 && fabs(GetTpcPullp(itrk,0))<2 && fabs(GetTpcPullele(itrk,0))>2 && fabs(GetTpcPullmu(itrk,0))>2 ) || 
       (GetTreeTools().GetDetUsed(itrk,1)==1 && fabs(GetTpcPullp(itrk,1))<2 && fabs(GetTpcPullele(itrk,1))>2 && fabs(GetTpcPullmu(itrk,1))>2) || 
       (GetTreeTools().GetDetUsed(itrk,2)==1 && fabs(GetTpcPullp(itrk,2))<2 && fabs(GetTpcPullele(itrk,2))>2 && fabs(GetTpcPullmu(itrk,2))>2 )) return true; 
  
       
  return false;

}

//**************************************************
bool AnalysisBase::ProtonPIDCutLike(int itrk){
//**************************************************

  if (GetPIDLikelihood(itrk,2)>0.9) return true;

  return false;

}

//**************************************************
bool AnalysisBase::CommonVertexCut(int itrk1, int itrk2){
//**************************************************

  if (fabs(GetTreeTools().GetPositionStart(itrk1)[0]-GetTreeTools().GetPositionStart(itrk2)[0]) > 30 ) return false; 
  if (fabs(GetTreeTools().GetPositionStart(itrk1)[1]-GetTreeTools().GetPositionStart(itrk2)[1]) > 30 ) return false; 
  if (fabs(GetTreeTools().GetPositionStart(itrk1)[2]-GetTreeTools().GetPositionStart(itrk2)[2]) > 50 ) return false;   

  return true;

}

//********************************************************************
//bool AnalysisBase::ExternalVetoCut(int itrk, bool qualityCut){
//********************************************************************
/*
  for (int i=0;i<GetTreeTools().GetNTracksWithTPC();i++){	
    if (i==itrk) continue;
    if (qualityCut)
      if (!TrackQualityCut(i)) continue;
    if (GetTreeTools().GetPositionStart(itrk)[2]-GetTreeTools().GetPositionStart(i)[2]<-150) return false;
  }
  
  return true;

}
*/


//**************************************************
bool AnalysisBase::FiducialCut(std::string det, double *pos){
//**************************************************
  
  bool fgd1 = false;
  bool fgd2 = false;
  bool p0d = false;

  if (det =="FGD" || det=="FGD1" || det=="FGD-P0D")
    if (pos[0] > fgd1min.X()+FVdefminFGD1.X() &&
	pos[0] < fgd1max.X()-FVdefmaxFGD1.X() &&
	pos[1] > fgd1min.Y()+FVdefminFGD1.Y() &&
	pos[1] < fgd1max.Y()-FVdefmaxFGD1.Y() &&
	pos[2] > fgd1min.Z()+FVdefminFGD1.Z() &&
	pos[2] < fgd1max.Z()-FVdefmaxFGD2.Z())
      fgd1 = true;

  if (det =="FGD" || det=="FGD2" || det=="FGD-P0D")
    if (pos[0] > fgd2min.X()+FVdefminFGD2.X() &&
	pos[0] < fgd2max.X()-FVdefmaxFGD2.X() &&
	pos[1] > fgd2min.Y()+FVdefminFGD2.Y() &&
	pos[1] < fgd2max.Y()-FVdefmaxFGD2.Y() &&
	pos[2] > fgd2min.Z()+FVdefminFGD2.Z() &&
	pos[2] < fgd2max.Z()-FVdefmaxFGD2.Z())
      fgd2=true;


  if (det =="P0D" || det=="FGD-P0D")
    if (pos[0] > p0dmin.X()+FVdefminP0D.X() &&
	pos[0] < p0dmax.X()-FVdefmaxP0D.X() &&
	pos[1] > p0dmin.Y()+FVdefminP0D.Y() &&
	pos[1] < p0dmax.Y()-FVdefmaxP0D.Y() &&
	pos[2] > p0dmin.Z()+FVdefminP0D.Z() &&
	pos[2] < p0dmax.Z()-FVdefmaxP0D.Z())
      p0d=true;
  
  
  if (fgd1 || fgd2 || p0d) return true;
  else return false;

}

//**************************************************
bool AnalysisBase::InDetVolume(std::string det, double *pos){
//**************************************************
  
  bool fgd1 = false;
  bool fgd2 = false;
  bool p0d = false;

  if(det =="FGD" || det=="FGD1" || det=="FGD-P0D")
    if (pos[0] > fgd1min.X() &&
	pos[0] < fgd1max.X() &&
	pos[1] > fgd1min.Y() &&
	pos[1] < fgd1max.Y() &&
	pos[2] > fgd1min.Z() &&
	pos[2] < fgd1max.Z())
      fgd1 = true;

  if (det =="FGD" || det=="FGD2" || det=="FGD-P0D")
    if (pos[0] > fgd2min.X() &&
	pos[0] < fgd2max.X() &&
	pos[1] > fgd2min.Y() &&
	pos[1] < fgd2max.Y() &&
	pos[2] > fgd2min.Z() &&
	pos[2] < fgd2max.Z())
      fgd2=true;


  if (det =="P0D" || det=="FGD-P0D")
    if (pos[0] > p0dmin.X() &&
	pos[0] < p0dmax.X() &&
	pos[1] > p0dmin.Y() &&
	pos[1] < p0dmax.Y() &&
	pos[2] > p0dmin.Z() &&
	pos[2] < p0dmax.Z())
      p0d=true;
  
  
  if (fgd1 || fgd2 || p0d) return true;
  else return false;

}


//********************************************************************
bool AnalysisBase::FindLeadingTracks(int& HM_track, int& HMN_track, int& HMP_track, bool qualityCut){
//********************************************************************

  // Get the highest momentum track (HM),
  // the highest momentum negative track (HMN) and the 
  // highest momentum positive track (HMP)
  // from all tracks passing the quality cut
  
  HM_track=-1;
  HMN_track=-1;
  HMP_track=-1;
  //loop over tpc tracks
  for (int i=0;i<GetTreeTools().GetNTracksWithTPC();i++){	   

    if (qualityCut)
      //apply the quality cut
      if (!TrackQualityCut(i)) continue;
    
    if (!FiducialCut("FGD1",GetTreeTools().GetPositionStart(i))){
      //tracks are sorted by momentum, so the first track is the most energetic one
      if (HM_track==-1) HM_track=i;
      //check the charge of the most energetic track
      if (GetCharge(i)==-1 && HMN_track==-1) HMN_track= i;
      if (GetCharge(i)==+1 && HMP_track==-1) HMP_track= i;
      
      if (HMN_track>-1 && HMP_track>-1) break;
    }
  }

  return (HM_track!=-1);
  
}


//**************************************************
int AnalysisBase::GetNHitsInClosestTpc(int itrk){
//**************************************************
  int tpchits = -10;
  int tpc = GetClosestTPC(itrk);
  if (tpc!=-1)
    tpchits = GetTreeTools().GetTpcNHits(itrk,tpc);
  else
    tpchits = -10;

  return tpchits;
}


//**************************************************
double AnalysisBase::GetMomentumInClosestTpc(int itrk){
//**************************************************
  int tpcmom = -10;
  int tpc = GetClosestTPC(itrk);
  if (tpc!=-1)
    tpcmom = GetTpcMomentum(itrk,tpc);
  else 
    tpcmom = -10;  

  return tpcmom;
}

//**************************************************
double AnalysisBase::GetMomentumRelErrorInClosestTpc(int itrk){
//**************************************************
  int tpcmomrelerr = -10;
  int tpc = GetClosestTPC(itrk);
  if (tpc!=-1)
    tpcmomrelerr = GetTreeTools().GetMomentumRelErrorLocal(itrk,tpc);
  else
    tpcmomrelerr = -10;
  
  return tpcmomrelerr;
}



//**************************************************
int AnalysisBase::GetReaction(int itrk){
//**************************************************
  
  /* Classify reaction types
     -1 = no tru vertex associated to track
     0 = CCQE
     1 = CC RES
     2 = CC DIS
     3 = CC COH
     4 = NC
     5 = anti-numu
     6 = out of FGD FV
     7 = <70% true-rec matching purity
   */

  
  int ivtx = GetTreeTools().GetTrueVertexIndex(itrk);

  // no true vertex associated to track
  if (GetTreeTools().GetTrueVertexIndex(itrk)==-1) return -1;

  // This is the true-rec matching purity
  if (GetTreeTools().GetTruePurity(itrk)<0.7) return 7;

  
  int reac   = GetTreeTools().GetTrueReacCode(ivtx);
  int nutype = GetTreeTools().GetTrueNuPDG(ivtx);

  // out of FGD FV
  if (!FiducialCut("FGD", GetTreeTools().GetTrueVertexPosition(ivtx))) return 6;

  // standard reactions
  if (nutype==14){
    if (reac == 1) return 0;
    if (reac >10 && reac<14) return 1;
    if (reac >16 && reac<30) return 2;
    if (reac ==16) return 3;
    if (reac >30)  return 4;
  }
  // antinumu
  if (nutype==-14){
    return 5;
  }  

  return -1;

}

//**************************************************
int AnalysisBase::GetNSegmentsInDet(int itrk, const std::string& det){
//**************************************************
  
  // for a given track compute the number of segments in a given detector

  int ndetcomp = 0;
  
  if (det=="TPC"){
    for (int i=0;i<3;i++)
      if (GetTreeTools().GetDetUsed(itrk,i)) ndetcomp++;
  }
  else if (det=="FGD"){
    for (int i=3;i<5;i++)
      if (GetTreeTools().GetDetUsed(itrk,i)) ndetcomp++;
  }
  else if (det=="ECAL"){    
    for (int i=11;i<19;i++)
      if (GetTreeTools().GetDetUsed(itrk,i)) ndetcomp++;

    if (GetTreeTools().GetDetUsed(itrk,6)) ndetcomp++;
  }
  else if (det=="SMRD"){
    for (int i=7;i<11;i++)
      if (GetTreeTools().GetDetUsed(itrk,i)) ndetcomp++;
  }
  else if (det=="P0D"){    
    if (GetTreeTools().GetDetUsed(itrk,5)) ndetcomp++;
  }

  return ndetcomp;

}

//********************************************************************
void AnalysisBase::FillCategories(int itrk, TrackCategory& track){
//********************************************************************

  // This is used to do the plots with color codes.
  // For each classification 

  if (itrk==-1) return;

  // ----- Particle ------------------------------
  if (GetTreeTools().GetPDG(itrk)!=0){
    if (GetTreeTools().GetTruePurity(itrk)>0.7)
      track.SetCode("particle",    GetTreeTools().GetPDG(itrk));
    else
      track.SetCode("particle",    -1);
  }
  else
    track.SetCode("particle",    0);

  track.SetCode("reaction",    GetReaction(itrk));
  track.SetCode("parent",      GetTreeTools().GetParentPDG(itrk));
  track.SetCode("grandparent", GetTreeTools().GetGrandParentPDG(itrk));

  // ----- vertex ------------------------------
  if (GetTreeTools().GetTruePurity(itrk)>0.7)
    track.SetCode("vertex",      GetTreeTools().GetTrueVertexDet(itrk)+1);
  else
    track.SetCode("vertex",      10);

  // ----- sense ------------------------------
  if (GetTreeTools().GetTruePurity(itrk)>0.7){
    if (     GetTreeTools().GetDirection(itrk)[2]>0 && GetTreeTools().GetTrueDirection(itrk)[2]>0) 
      track.SetCode("sense", 0);
    else if (GetTreeTools().GetDirection(itrk)[2]>0 && GetTreeTools().GetTrueDirection(itrk)[2]<0) 
      track.SetCode("sense", 1);
    else if (GetTreeTools().GetDirection(itrk)[2]<0 && GetTreeTools().GetTrueDirection(itrk)[2]<0) 
      track.SetCode("sense", 2);
    else if (GetTreeTools().GetDirection(itrk)[2]<0 && GetTreeTools().GetTrueDirection(itrk)[2]>0) 
      track.SetCode("sense", 3);
    else if (GetTreeTools().GetDirection(itrk)[2]==0) 
      track.SetCode("sense", 4);
    else 
      track.SetCode("sense", 5);
  }
  else 
    track.SetCode("sense", 6);

  // ----- Number of TPC hits ------------------------------
  /*
  if      ( GetTreeTools().GetTpcNHits(itrk) <= 18)
    track.SetCode("ntpchits", 0);
  else if ( GetTreeTools().GetTpcNHits(itrk) > 18 && GetTreeTools().GetTpcNHits(itrk) <= 40)
    track.SetCode("ntpchits", 1);
  else if ( GetTreeTools().GetTpcNHits(itrk) > 40 && GetTreeTools().GetTpcNHits(itrk) <= 64)
    track.SetCode("ntpchits", 2);
  else if ( GetTreeTools().GetTpcNHits(itrk) > 64 && GetTreeTools().GetTpcNHits(itrk) <= 72)
    track.SetCode("ntpchits", 3);
  else if ( GetTreeTools().GetTpcNHits(itrk) > 72)
    track.SetCode("ntpchits", 4);
  */
  if      ( GetTreeTools().GetTpcNHits(itrk) <= 14)
    track.SetCode("ntpchits", 0);
  else if ( GetTreeTools().GetTpcNHits(itrk) > 14 && GetTreeTools().GetTpcNHits(itrk) <= 18)
    track.SetCode("ntpchits", 1);
  else if ( GetTreeTools().GetTpcNHits(itrk) > 18 && GetTreeTools().GetTpcNHits(itrk) <= 22)
    track.SetCode("ntpchits", 2);
  else if ( GetTreeTools().GetTpcNHits(itrk) > 22 && GetTreeTools().GetTpcNHits(itrk) <= 26)
    track.SetCode("ntpchits", 3);
  else if ( GetTreeTools().GetTpcNHits(itrk) > 26 && GetTreeTools().GetTpcNHits(itrk) <= 30)
    track.SetCode("ntpchits", 4);
  else if ( GetTreeTools().GetTpcNHits(itrk) > 30 && GetTreeTools().GetTpcNHits(itrk) <= 34)
    track.SetCode("ntpchits", 5);
  else if ( GetTreeTools().GetTpcNHits(itrk) > 34 && GetTreeTools().GetTpcNHits(itrk) <= 40)
    track.SetCode("ntpchits", 6);
  else if ( GetTreeTools().GetTpcNHits(itrk) > 40 && GetTreeTools().GetTpcNHits(itrk) <= 64)
    track.SetCode("ntpchits", 7);
  else if ( GetTreeTools().GetTpcNHits(itrk) > 64 && GetTreeTools().GetTpcNHits(itrk) <= 72)
    track.SetCode("ntpchits", 8);
  else if ( GetTreeTools().GetTpcNHits(itrk) > 72)
    track.SetCode("ntpchits", 9);

  // ----- reaction 2 ------------------------------
  if (GetReaction(itrk) <4)
    track.SetCode("reaction2",  0);
  else
    track.SetCode("reaction2",  GetReaction(itrk));

  // ----- detnotused ------------------------------
  bool SMRD = false;
  if (GetTreeTools().GetDetUsed(itrk, 7) || GetTreeTools().GetDetUsed(itrk, 8) || GetTreeTools().GetDetUsed(itrk, 9) || GetTreeTools().GetDetUsed(itrk, 10))
    SMRD=true;
  bool TrECAL = false;
  if (GetTreeTools().GetDetUsed(itrk, 15) || GetTreeTools().GetDetUsed(itrk, 16) || GetTreeTools().GetDetUsed(itrk, 17) || GetTreeTools().GetDetUsed(itrk, 18))
    TrECAL=true;

  track.SetCode("detnotused",  "TPC1", !GetTreeTools().GetDetUsed(itrk, 0));
  track.SetCode("detnotused",  "TPC2", !GetTreeTools().GetDetUsed(itrk, 1));
  track.SetCode("detnotused",  "TPC3", !GetTreeTools().GetDetUsed(itrk, 2));
  track.SetCode("detnotused",  "FGD1", !GetTreeTools().GetDetUsed(itrk, 3));
  track.SetCode("detnotused",  "FGD2", !GetTreeTools().GetDetUsed(itrk, 4));
  track.SetCode("detnotused",  "P0D",  !GetTreeTools().GetDetUsed(itrk, 5));
  track.SetCode("detnotused",  "DsECAL", !GetTreeTools().GetDetUsed(itrk, 6));

  track.SetCode("detnotused",  "SMRD", !SMRD);
  track.SetCode("detnotused",  "TrECAL", !TrECAL);

  // ----- detused ------------------------------
  track.SetCode("detused",  "TPC1", GetTreeTools().GetDetUsed(itrk, 0));
  track.SetCode("detused",  "TPC2", GetTreeTools().GetDetUsed(itrk, 1));
  track.SetCode("detused",  "TPC3", GetTreeTools().GetDetUsed(itrk, 2));
  track.SetCode("detused",  "FGD1", GetTreeTools().GetDetUsed(itrk, 3));
  track.SetCode("detused",  "FGD2", GetTreeTools().GetDetUsed(itrk, 4));
  track.SetCode("detused",  "P0D",  GetTreeTools().GetDetUsed(itrk, 5));
  track.SetCode("detused",  "DsECAL", GetTreeTools().GetDetUsed(itrk, 6));

  track.SetCode("detused",  "SMRD", SMRD);
  track.SetCode("detused",  "TrECAL", TrECAL);

  // ----- momrelerr ------------------------------
  int tpc = GetClosestTPC(itrk);
  if (tpc>-1) {    
    double err = GetTreeTools().GetMomentumRelErrorLocal(itrk, tpc);
    if      ( err>0 && err <= 0.05)
      track.SetCode("momrelerr", 0);
    else if      ( err>0.05 && err <= 0.1)
      track.SetCode("momrelerr", 1);
    else if      ( err>0.1 && err <= 0.2)
      track.SetCode("momrelerr", 2);
    else if      ( err>0.2 && err <= 0.3)
      track.SetCode("momrelerr", 3);
    else if      ( err>0.3 && err <= 0.4)
      track.SetCode("momrelerr", 4);
    else if      ( err>0.4 && err <= 0.5)
      track.SetCode("momrelerr", 5);
    else if      ( err>0.5 && err <= 0.6)
      track.SetCode("momrelerr", 6);
    else if      ( err>0.6 && err <= 0.8)
      track.SetCode("momrelerr", 7);
    else if      ( err>0.8 && err <= 1.)
      track.SetCode("momrelerr", 8);
  }


}




//********************************************************************
double AnalysisBase::GetPIDLikelihood(int itrk, int hypo){
//********************************************************************

  if (_like_computed[itrk]) return _likelihood[itrk][hypo];

  for (int i=0;i<4;i++)
    _likelihood[itrk][i]=-1;

  double prob[4]={1,1,1,1};
  double total_prob=0;
  double PULLCUT=5;
  double PULLMAX=1e4;
  double PULLMIN=1e-6;
  bool found=false;

  for (int tpc=0;tpc<3;tpc++){
    if (!GetTreeTools().GetDetUsed(itrk,tpc)) continue;
    if ((fabs(GetTpcPullmu(itrk,tpc))<PULLCUT || 
	 fabs(GetTpcPullp(itrk,tpc))<PULLCUT ||
	 fabs(GetTpcPullpi(itrk,tpc))<PULLCUT ||
	 fabs(GetTpcPullele(itrk,tpc))<PULLCUT) && 
	fabs(GetTpcPullmu(itrk,tpc))>PULLMIN && 
	fabs(GetTpcPullp(itrk,tpc))>PULLMIN && 
	fabs(GetTpcPullpi(itrk,tpc))>PULLMIN && 
	fabs(GetTpcPullele(itrk,tpc))>PULLMIN &&
	fabs(GetTpcPullmu(itrk,tpc))<PULLMAX && 
	fabs(GetTpcPullp(itrk,tpc))<PULLMAX && 
	fabs(GetTpcPullpi(itrk,tpc))<PULLMAX && 
	fabs(GetTpcPullele(itrk,tpc))<PULLMAX){

      prob[0] *= exp(-pow(GetTpcPullmu(itrk,tpc),2)/2);
      prob[1] *= exp(-pow(GetTpcPullele(itrk,tpc),2)/2);
      prob[2] *= exp(-pow(GetTpcPullp(itrk,tpc),2)/2);
      prob[3] *= exp(-pow(GetTpcPullpi(itrk,tpc),2)/2);

      found = true;
    }
  }

  if (found){

    for (int h=0;h<4;h++){
      //      prob[h] *= GetPIDPrior(h,itrk);
      total_prob += prob[h] ;
    }

    if (total_prob>0){
      for (int h=0;h<4;h++){
	_likelihood[itrk][h] = prob[h]/total_prob ;
      }
    }
  }

  _like_computed[itrk]=true;

  return _likelihood[itrk][hypo];

}

//********************************************************************
double AnalysisBase::GetPIDPrior(int itrk, int hypo){
//********************************************************************

  // This function is not used yet

  double xbins[18] = {0., 100., 200., 300., 400., 500., 600., 700, 800, 1000., 1200., 1400., 1700, 2000., 2500., 3000., 4000., 5000.};

  double eprior[17] = {800., 250., 100,  40,    30,  25,   10,    5,    0,   0,     0,     0,     0,     0,     0,     0,     0}; 

  for (int i=0;i<17;i++){
    eprior[i] /=400.;
  }

  int pbin = 16;
  for (int i=0;i<17;i++){
    pbin = i-1;
    if (GetMomentum(itrk)>0 && GetMomentum(itrk) < xbins[i]) break;
  }


  if (hypo==1)  return eprior[pbin];
  else return 1.;



}

//********************************************************************
double AnalysisBase::GetTrueCosTheta(int itrk) {
//********************************************************************

  int ivtx = GetTreeTools().GetTrueVertexIndex(itrk);
  if (GetTreeTools().GetTrueVertexIndex(itrk)==-1) return -1;

  double trueCosTheta = -999;

  trueCosTheta = GetTreeTools().GetTrueNuDir(ivtx,0)*GetTreeTools().GetTrueDirection(itrk,0) + GetTreeTools().GetTrueNuDir(ivtx,1)*GetTreeTools().GetTrueDirection(itrk,1) + GetTreeTools().GetTrueNuDir(ivtx,2)*GetTreeTools().GetTrueDirection(itrk,2);

  return trueCosTheta;

}

//********************************************************************
double AnalysisBase::GetRecCosTheta(int itrk) {
//********************************************************************

  int ivtx = GetTreeTools().GetTrueVertexIndex(itrk);
  if (GetTreeTools().GetTrueVertexIndex(itrk)==-1) return -1;

  double recCosTheta = -999;

  recCosTheta = GetTreeTools().GetTrueNuDir(ivtx,0)*GetTreeTools().GetDirection(itrk,0) + GetTreeTools().GetTrueNuDir(ivtx,1)*GetTreeTools().GetDirection(itrk,1) + GetTreeTools().GetTrueNuDir(ivtx,2)*GetTreeTools().GetDirection(itrk,2);

  return recCosTheta;

}

//********************************************************************
double AnalysisBase::GetDeltaCosTheta(int itrk) {
//********************************************************************

  double deltaCosTheta = -999;

  //deltaCosTheta = GetCosTheta(itrk) - GetTrueCosTheta(itrk);
  deltaCosTheta = GetRecCosTheta(itrk) - GetTrueCosTheta(itrk);

  return deltaCosTheta;

}

//********************************************************************
void AnalysisBase::CutPassed(int icut) {
//********************************************************************

  // the event has passed cut number "icut"
  _cut_passed[icut]=true;

  // The event has passed all previous cuts and this one
  if (_accum_cut_level==icut || icut==0)
    _accum_cut_level++;


}


//********************************************************************
double AnalysisBase::GetPtLocal(int itrk) {
//********************************************************************

  double pt = 0;
  pt = GetTreeTools().GetMomentumLocal(itrk) * sqrt(pow(GetTreeTools().GetDirection(itrk,1),2) + pow(GetTreeTools().GetDirection(itrk,2),2));
  
  return pt;
  
}

//********************************************************************
double AnalysisBase::GetSigmaPtLocal(int itrk, int tpc) {
//********************************************************************

  double spt = 0;
  spt = GetTreeTools().GetTpcMomentumRelError(itrk,tpc) * sqrt(pow(GetTreeTools().GetDirection(itrk,1),2) + pow(GetTreeTools().GetDirection(itrk,2),2));
  
  return spt;
  
}


//********************************************************************
double AnalysisBase::GetSigmaInvPtLocal(int itrk, int tpc) {
//********************************************************************

  double sinvpt = 0;
  sinvpt = GetSigmaPtLocal(itrk,tpc) / (pow(GetPtLocal(itrk),2));
  
  return sinvpt;
  
}


//********************************************************************
double AnalysisBase::GetTruePt(int itrk) {
//********************************************************************

  double pt = 0;
  pt = GetTreeTools().GetTrueMomentum(itrk) * sqrt(pow(GetTreeTools().GetTrueDirection(itrk,1),2) + pow(GetTreeTools().GetTrueDirection(itrk,2),2));
  
  return pt;
  
}


//********************************************************************
bool AnalysisBase::DeltaZCut(int itrk){
//********************************************************************
  
  double startposz = GetTreeTools().GetPositionStart(itrk,2);
  double endposz = GetTreeTools().GetPositionEnd(itrk,2);
  
  if (FiducialCut("FGD1",GetTreeTools().GetPositionStart(itrk)) && endposz < startposz) return true;
  
  return false;
}


//********************************************************************
bool AnalysisBase::ExternalVetoCut(int itrk){
//********************************************************************

  double TPCVetoDistance = -150.;

  double startposz = GetTreeTools().GetPositionStart(itrk,2);
  double endposz = GetTreeTools().GetPositionEnd(itrk,2);
  if(endposz < 115. && endposz < startposz && FiducialCut("FGD2",GetTreeTools().GetPositionStart(itrk)) ) return true;

  double Hmom = -9999; 
  int vetoTrack = -1;
  //loop over tpc tracks
  for (int i=0;i<GetTreeTools().GetNTracksWithTPC();i++){
    double mom = GetTreeTools().GetMomentumLocal(itrk);
    if (i == itrk) continue;  // same particle as muon candidate 
    if( mom < 0 ) continue;   // protection for bad momentum
    if( mom > Hmom ) {        // Search for highest momentum track in the bunch that is not he muon. 
      vetoTrack =  i;
      Hmom = mom; 
    }
  }
  
  if(vetoTrack != -1) {
    if(GetTreeTools().GetPositionStart(vetoTrack,2) - GetTreeTools().GetPositionStart(itrk,2) < TPCVetoDistance)  return true; 
  }

  return false;
}
