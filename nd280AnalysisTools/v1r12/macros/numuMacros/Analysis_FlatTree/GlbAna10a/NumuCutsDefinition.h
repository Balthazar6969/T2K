#ifndef NumuCutsDefinition_h
#define NumuCutsDefinition_h

#include "TVector3.h"
#include "string.h"
#include "NumuGeomDefinition.h"

/*

  This is the list of cumulative cuts. The second index is the cut number. 
  The first index reffers to the tracks number: 0=HM, 1=SHM.
  So you can apply a a cut to the first or the second track. 

    // tracks with TPC constituents
    scut[itrk][0]

    // Track quality. >18 TPC hits 
    scut[itrk][1]

    // bunching
    scut[itrk][2]

    // vertex in fiducial volume
    scut[itrk][3]

    // Charge cut
    scut[itrk][4]

    // PID quality. At least one particle with pulls<4
    scut[itrk][5]

    // Most energetic negative track should be a muon (negative track PID)
    scut[itrk][6]
    icutCC = 6;

    // track multiplicity (1 or 2 tracks)
    scut[itrk][7]

    // negative track multiplicity (1 and only 1 negative track)
    scut[itrk][8]

    // unused hits cut
    scut[itrk][9]

    // no michel electron
    scut[itrk][10]

    // vertex cut. Make sure that both tracks have a common vertex. Bunching 
    scut[itrk][11]

    // Track quality for 2nd track (>18 TPC hits) 
    scut[itrk][12]

    // PID quality for 2nd track. at least one particle with pulls<4
    scut[itrk][13]

    // Most energetic positive track should be a proton (positive track PID)
    scut[itrk][14]

    // Delta_theta_p
    scut[itrk][15]

    // Neutrino invariant mass
    scut[itrk][16]
    icutCCQE = 16;
*/


const int NMAXTRACKS = 2;
const int NMAXCUTS = 17;

class NumuCutsDefinition{

 public:
  
  int icutNegpidq;
  int icutTrq;
  int imupid;
  int ippid;

  int icutCC;
  int icutCCQE;
  
  std::string scut[NMAXTRACKS][NMAXCUTS]; 
  std::string scutin[NMAXTRACKS][NMAXCUTS+1]; 

  std::string quality[NMAXTRACKS];
  std::string pidquality[NMAXTRACKS];
  std::string mupid[NMAXTRACKS];
  std::string ppid[NMAXTRACKS];

  std::string qualityTPC[NMAXTRACKS][3];
  std::string pidqualityTPC[NMAXTRACKS][3];
  std::string mupidTPC[NMAXTRACKS][3];

  std::string fgd1FV_cutMC[NMAXTRACKS];
  std::string fgd2FV_cutMC[NMAXTRACKS];
  std::string fgdFV_cutMC[NMAXTRACKS];

  std::string fgd1FV_cutMC2[NMAXTRACKS];
  std::string fgd2FV_cutMC2[NMAXTRACKS];
  std::string fgdFV_cutMC2[NMAXTRACKS];

  std::string fgd1FV_cut[NMAXTRACKS];
  std::string fgd2FV_cut[NMAXTRACKS];
  std::string fgdFV_cut[NMAXTRACKS];

  
  


  //TYPEAcuts(TYPEBcuts=false):
  //use always the most energetic track: if the most energetic track is positive then the event is rejected
  //it uses the pid cut based on likelihood
  //TYPEBcuts(TYPEBcuts=true):
  //use the most energetic negative track: this means that if the most energetic track is a proton and the second highest energetic track is negative we will use this second track as the muon and continue the selection
  //it uses also pid cut based only on pulls and not likelihood
  //it does not use corrected pulls
  NumuCutsDefinition(int charge = -1, bool useTYPEBcuts=false){
    reset(charge,useTYPEBcuts);
    DoCumulativeCut();
  }

  void reset(int charge = -1, bool useTYPEBcuts=false){
   
    TVector3 fgd1minFV = fgd1min + FVdefminFGD1;
    TVector3 fgd1maxFV = fgd1max - FVdefmaxFGD1;
    TVector3 fgd2minFV = fgd2min + FVdefminFGD2;
    TVector3 fgd2maxFV = fgd2max - FVdefmaxFGD2;

    // ------------ true vertex in fiducial volume --------------------------
    for(int i = 0; i < NMAXTRACKS; i ++){
      //here we count the true vertex only if a tpc track is visible
      //this count for numerator of the purity and efficiency
      fgd1FV_cutMC[i] = DefineVolume_Cut(fgd1minFV,fgd1maxFV,101,i);  
      fgd2FV_cutMC[i] = DefineVolume_Cut(fgd2minFV,fgd2maxFV,101,i);
      fgdFV_cutMC[i] = "("+ fgd1FV_cutMC[i]+" || "+fgd2FV_cutMC[i]+")";
     
      //here we count the true vertex when there is one in FV (even if we don't see anything)
      //this count for the denominator of the efficiency
      fgd1FV_cutMC2[i]= DefineVolume_Cut(fgd1minFV,fgd1maxFV,100,i); 
      fgd2FV_cutMC2[i]= DefineVolume_Cut(fgd2minFV,fgd2maxFV,100,i);
      fgdFV_cutMC2[i]="("+ fgd1FV_cutMC2[i]+" || "+fgd2FV_cutMC2[i]+")";
     
     
    }
    cerr<<" defining cuts ... "<<endl;

    // ----------- tracks with TPC constituents---- ---------------
    for(int i = 0; i < NMAXTRACKS; i ++) scutin[i][0] = "stdB_NTracksWithTPC>0";
    
    // ------------- Track quality -------------------------------------
    for(int i = 0; i < NMAXTRACKS; i ++){
      stringstream out_itrk;out_itrk<<i;
      string s_itrk=out_itrk.str();
      quality[i] = "(Alt$(stdB_trTpcNHits["+s_itrk+"][0],0)+Alt$(stdB_trTpcNHits["+s_itrk+"][1],0)+Alt$(stdB_trTpcNHits["+s_itrk+"][2],0)>18)";
      scutin[i][1] = quality[i];   
    }

    // ----------- bunching  -------------------------------------------
    for(int i = 0; i < NMAXTRACKS; i ++){
      stringstream out_itrk;out_itrk<<i;
      string s_itrk=out_itrk.str();
      scutin[i][2] = "( stdB_Bunch>0 && stdB_Bunch<7 && abs(Alt$(stdB_trPositionStart["+s_itrk+"][3],0)-stdB_TBunch) < 4*21.5 )";
    }

    // ------------ vertex in fiducial volume --------------------------
    for(int i = 0; i < NMAXTRACKS; i ++){
      fgd1FV_cut[i] = DefineVolume_Cut(fgd1minFV,fgd1maxFV,3,i);  // recon vertex
      fgd2FV_cut[i] = DefineVolume_Cut(fgd2minFV,fgd2maxFV,3,i);
      fgdFV_cut[i] = "("+ fgd1FV_cut[i]+" || "+fgd2FV_cut[i]+")";
    } 

    for(int i = 0; i < NMAXTRACKS; i ++){
      scutin[i][3] = fgd1FV_cut[i];
      scutin[i][4] = fgd2FV_cut[i];
    }

   

    // ----------- charge of the HM track --------------------------------
    for(int i = 0; i < NMAXTRACKS; i ++){
      stringstream out_itrk;out_itrk<<i;
      string s_itrk=out_itrk.str();
      if(i==0 && useTYPEBcuts)
        scutin[i][5] = "( (Alt$(stdB_trCharge[0],0) == -1) ||  (Alt$(stdB_trCharge[0],0)== 1 && Alt$(stdB_trCharge[1],0)==-1))"; 
      else
        scutin[i][5] = "(Alt$(stdB_trCharge["+s_itrk+"],0) == -1)";
      
      if(charge == 1) scutin[i][5] = "(Alt$(stdB_trCharge["+s_itrk+"],0) == 1)";
    }

    // ------------- PID quality -------------------------------------
    for(int i = 0; i < NMAXTRACKS; i ++){
      stringstream out_itrk;out_itrk<<i;
      string s_itrk=out_itrk.str();
      //uncorrected pulls
      if(useTYPEBcuts){
        pidqualityTPC[i][0] = "(( (abs(Alt$(stdB_trPullmu["+s_itrk+"][0],0))<4 || abs(Alt$(stdB_trPullele["+s_itrk+"][0],0))<4 || abs(Alt$(stdB_trPullp["+s_itrk+"][0],0))<4) && Alt$(stdB_trPullmu["+s_itrk+"][0],0)!=0) || Alt$(stdB_trDetUsed["+s_itrk+"][0],0)==0)"; 
        pidqualityTPC[i][1] = "(( (abs(Alt$(stdB_trPullmu["+s_itrk+"][1],0))<4 || abs(Alt$(stdB_trPullele["+s_itrk+"][1],0))<4 || abs(Alt$(stdB_trPullp["+s_itrk+"][1],0))<4) && Alt$(stdB_trPullmu["+s_itrk+"][1],0)!=0) || Alt$(stdB_trDetUsed["+s_itrk+"][1],0)==0)"; 
        pidqualityTPC[i][2] = "(( (abs(Alt$(stdB_trPullmu["+s_itrk+"][2],0))<4 || abs(Alt$(stdB_trPullele["+s_itrk+"][2],0))<4 || abs(Alt$(stdB_trPullp["+s_itrk+"][2],0))<4) && Alt$(stdB_trPullmu["+s_itrk+"][2],0)!=0) || Alt$(stdB_trDetUsed["+s_itrk+"][2],0)==0)"; 
      }
      else{
        pidqualityTPC[i][0] = "(( (abs(Alt$(g10aB_trPullCorrmu["+s_itrk+"][0],0))<4 || abs(Alt$(g10aB_trPullCorrele["+s_itrk+"][0],0))<4 || abs(Alt$(g10aB_trPullCorrp["+s_itrk+"][0],0))<4) && Alt$(g10aB_trPullCorrmu["+s_itrk+"][0],0)!=0) || Alt$(stdB_trDetUsed["+s_itrk+"][0],0)==0)"; 
        pidqualityTPC[i][1] = "(( (abs(Alt$(g10aB_trPullCorrmu["+s_itrk+"][1],0))<4 || abs(Alt$(g10aB_trPullCorrele["+s_itrk+"][1],0))<4 || abs(Alt$(g10aB_trPullCorrp["+s_itrk+"][1],0))<4) && Alt$(g10aB_trPullCorrmu["+s_itrk+"][1],0)!=0) || Alt$(stdB_trDetUsed["+s_itrk+"][1],0)==0)"; 
        pidqualityTPC[i][2] = "(( (abs(Alt$(g10aB_trPullCorrmu["+s_itrk+"][2],0))<4 || abs(Alt$(g10aB_trPullCorrele["+s_itrk+"][2],0))<4 || abs(Alt$(g10aB_trPullCorrp["+s_itrk+"][2],0))<4) && Alt$(g10aB_trPullCorrmu["+s_itrk+"][2],0)!=0) || Alt$(stdB_trDetUsed["+s_itrk+"][2],0)==0)"; 
      }
      pidquality[i] = "("+pidqualityTPC[i][0]+" && "+pidqualityTPC[i][1]+" && "+pidqualityTPC[i][2]+")";
      scutin[i][6] =  pidquality[i] ;
    }
    // ----------------- muon PID ---------------------------------
    // Most energetic negative track should be a muon
    for(int i = 0; i < NMAXTRACKS; i ++){
      stringstream out_itrk;out_itrk<<i;
      string s_itrk=out_itrk.str();
      if(useTYPEBcuts){
        mupid[i] = " (( Alt$(stdB_trDetUsed["+s_itrk+"][0],0)==1 && abs(Alt$(stdB_trPullmu["+s_itrk+"][0],0))<2 && abs(Alt$(stdB_trPullele["+s_itrk+"][0],0))>2 ) || ( Alt$(stdB_trDetUsed["+s_itrk+"][1],0)==1 && abs(Alt$(stdB_trPullmu["+s_itrk+"][1],0))<2 && abs(Alt$(stdB_trPullele["+s_itrk+"][1],0))>2 ) || ( Alt$(stdB_trDetUsed["+s_itrk+"][2],0)==1 && abs(Alt$(stdB_trPullmu["+s_itrk+"][2],0))<2 && abs(Alt$(stdB_trPullele["+s_itrk+"][2],0))>2 )) ";
      }
      else{
        mupid[i] = "((g10aB_trProbmu["+s_itrk+"]+g10aB_trProbpi["+s_itrk+"])/(1-g10aB_trProbk["+s_itrk+"]-g10aB_trProbp["+s_itrk+"])>0.72)";
      }
     
    }
    for(int i = 0; i < NMAXTRACKS; i ++){
      if(useTYPEBcuts && i==0)      
        scutin[i][7] = "( ( stdB_NTracksWithTPC>0 &&"+mupid[0]+") || ( stdB_NTracksWithTPC>=0 && "+mupid[1]+"&& Alt$(stdB_trCharge[0],0)==1 && Alt$(stdB_trCharge[1],0)==-1) )";
      else
        scutin[i][7] = "("+mupid[i]+")";

    }

    // ------------- track multiplicity --------------------
    //    for(int i = 0; i < NMAXTRACKS; i ++) scutin[i][8] = "stdB_NTracksWithTPC<3";
    for(int i=0;i<NMAXTRACKS;i++)scutin[i][8]="stdB_NTracksWithTPC<3 ";
  
    // ------------- negative track multiplicity ----------
    for(int i = 0; i < NMAXTRACKS; i ++) scutin[i][9] = "stdB_NNegTracksWithTPC==1";

    // -------------fgd unused hits --------------------
    for(int i=0;i<NMAXTRACKS;i++)scutin[i][10]="( ("+ fgd1FV_cut[0]+" && std_NFGDUnusedHits[0]<15 ) || ("+fgd2FV_cut[0]+" && std_NFGDUnusedHits[1]<8))";

    // ------------- no michel electron -------------------
    for(int i = 0; i < NMAXTRACKS; i ++) scutin[i][11] = "stdB_NClusterscut[0][2]==0";

    // -------------- vertex cut. Make sure that both tracks have a common vertex ------------- 
    scutin[0][12] = "((abs(Alt$(stdB_trPositionStart[0][0],0)-Alt$(stdB_trPositionStart[1][0],0))<50 && abs(Alt$(stdB_trPositionStart[0][1],0)-Alt$(stdB_trPositionStart[1][1],0))<50 && abs(Alt$(stdB_trPositionStart[0][2],0)-Alt$(stdB_trPositionStart[1][2],0))<30 ) && " + scutin[1][2]+")";
    scutin[1][12] = "((abs(Alt$(stdB_trPositionStart[0][0],0)-Alt$(stdB_trPositionStart[1][0],0))<50 && abs(Alt$(stdB_trPositionStart[0][1],0)-Alt$(stdB_trPositionStart[1][1],0))<50 && abs(Alt$(stdB_trPositionStart[0][2],0)-Alt$(stdB_trPositionStart[1][2],0))<30 ) && " + scutin[0][2]+")";
    

    // ------------ Positive Track quality -------------------------
    scutin[0][13] ="("+ quality[1] + " &&  Alt$(stdB_trCharge[1],0)==1)";
    scutin[1][13] ="("+ quality[0] + " &&  Alt$(stdB_trCharge[0],0)==1)";

    // ------------ Positive PID quality -------------------------
    scutin[0][14] ="("+ pidquality[1] + " &&  Alt$(stdB_trCharge[1],0)==1)";
    scutin[1][14] ="("+ pidquality[0] + " &&  Alt$(stdB_trCharge[0],0)==1)";

    // ------------- proton PID  ---------------------
    ppid[0] = "(( Alt$(g10aB_trProbp[1],0)/(1- Alt$(g10aB_trProbk[1],0))>0.97)  &&  Alt$(stdB_trCharge[1],0)==1)";
    ppid[1] = "((g10aB_trProbp[0]/(1-g10aB_trProbk[0])>0.97)  && stdB_trCharge[0]==1)";

    // Most energetic positive track should be a proton
    for(int i = 0; i < NMAXTRACKS; i ++) scutin[i][15] = "(" + ppid[i] + ") ";

    // -------------- Delta_theta_p --------------------------------
    for(int i = 0; i < NMAXTRACKS; i ++) scutin[i][16] = "(stdB_DeltaThetaP[0]<0.6)";

    // -------------- Neutrino invariant mass ----------------------
    for(int i = 0; i < NMAXTRACKS; i ++) scutin[i][17] = "(stdB_NuInvMass>-0.4)";

 }

  void DoCumulativeCut(){
    //============== Cumulative cuts ===================================
    for(int k = 0; k < NMAXTRACKS; k ++){
      // tracks with TPC constituents
      scut[k][0] = scutin[k][0];
      // Track quality. >18 TPC hits 
      scut[k][1] = scut[k][0]   + " && " + scutin[k][1];
      // bunching
      scut[k][2] = scut[k][1]   + " && " + scutin[k][2];
      // vertex in fiducial volume
      // the vertex is defined by the most energetic track in the bunch with good quality
      // when the muon candidate is the second track (k==1) the vertex is still defined by the first track, when this is in the bunch and has good quality. Otherwise 
      // it is defined by the second track
      if(k==0)
        scut[k][3] = scut[k][2]   + " && ("+scutin[k][3] +" || " + scutin[k][4] + ")";
      else if(k==1)
	scut[k][3] = scut[k][2] + " && ((("+scutin[0][3] +" || "+scutin[0][4] +") && ("+scut[0][2]+")) || (("+scutin[1][3] +" || "+scutin[1][4] +") && !("+scut[0][2]+")))" ;
      // Charge cut
      scut[k][4] = scut[k][3]   + " && " + scutin[k][5];
      // PID quality. At least one particle with pulls<4
      scut[k][5] = scut[k][4]   + " && " + scutin[k][6];
      icutNegpidq=5;
      // Most energetic negative track should be a muon (negative track PID)
      scut[k][6] = scut[k][5]   + " && " + scutin[k][7];
      imupid = 6;
      icutCC = 6;
      // track multiplicity (1 or 2 tracks)
      scut[k][7]  = scut[k][6]  + " && "  + scutin[k][8];
      // negative track multiplicity (1 and only 1 negative track)
      scut[k][8]  = scut[k][7]  + " && "  + scutin[k][9];
      // fgd unused hits
      scut[k][9]  = scut[k][8]  + " && "  + scutin[k][10];
      // no michel electron
      scut[k][10]  = scut[k][9]  + " && "  + scutin[k][11];
      // vertex cut. Make sure that both tracks have a common vertex. Bunching 
      scut[k][11] = scut[k][10]  + " && (" + scutin[k][12] + " || stdB_NPosTracksWithTPC[0]==0)";
      // Track quality for 2nd track (>18 TPC hits) 
      scut[k][12] = scut[k][11] + " && (" + scutin[k][13] + " || stdB_NPosTracksWithTPC[0]==0)";
      // PID quality for 2nd track. at least one particle with pulls<4
      scut[k][13] = scut[k][12] + " && (" + scutin[k][14] + " || stdB_NPosTracksWithTPC[0]==0)";
      // Most energetic positive track should be a proton (positive track PID)
      scut[k][14] = scut[k][13] + " && (" + scutin[k][15] + " || stdB_NPosTracksWithTPC[0]==0)";
      ippid = 14;
      // Delta_theta_p
      scut[k][15] = scut[k][14] + " && (" + scutin[k][16] + " || stdB_NPosTracksWithTPC[0]==0)";
      // Neutrino invariant mass
      scut[k][16] = scut[k][15] + " && (" + scutin[k][17] + " || stdB_NPosTracksWithTPC[0]==0)";
      icutCCQE = 16;
    }

  }

 

};

#endif
