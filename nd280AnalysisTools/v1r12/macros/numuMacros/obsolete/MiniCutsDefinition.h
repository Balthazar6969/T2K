#ifndef MiniCutsDefinition_h
#define MiniCutsDefinition_h

#include "string.h"
#include "GeomDefinition.h"

/*

  This is the list of cumulative cuts. The second index is the cut number. 
  The first index reffers to the tracks number: 0=HM, 1=SHM.
  So you can apply a a cut to the first or the second track. 

    // tracks with TPC constituents
    scut[itrk][0]

    // bunching
    scut[itrk][1]

    // vertex in fiducial volume
    scut[itrk][2]

    // Track quality. >18 TPC hits 
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

    // no michel electron
    scut[itrk][9]

    // vertex cut. Make sure that both tracks have a common vertex. Bunching 
    scut[itrk][10]

    // Track quality for 2nd track (>18 TPC hits) 
    scut[itrk][11]

    // PID quality for 2nd track. at least one particle with pulls<4
    scut[itrk][12]

    // Most energetic positive track should be a proton (positive track PID)
    scut[itrk][13]

    // Delta_theta_p
    scut[itrk][14]

    // Neutrino invariant mass
    scut[itrk][15]
    icutCCQE = 15;
*/



const int NCUTS = 16;

class CutsDefinition{

 public:
      
  std::string scut[2][NCUTS]; 
  std::string scutin[2][NCUTS+1]; 

  std::string quality[2];
  std::string pidquality[2];
  std::string mupid[2];
  std::string ppid[2];

  int imupid;
  int ippid;

  int icutCC;
  int icutCCQE;

  std::string scutmc[2]; 
  std::string scutmcFGD1[2]; 
  std::string scutmcFGD2[2]; 
  std::string scut0FGD1[2]; 
  std::string scut0FGD2[2]; 
  std::string fgd1FV_cut[2];
  std::string fgd2FV_cut[2];
  std::string fgdFV_cut[2];
  std::string fgd1FV_cutMC[2];
  std::string fgd2FV_cutMC[2];
  std::string fgdFV_cutMC[2];

  CutsDefinition(int charge == -1){

    reset(charge);

  }

  reset(int charge == -1){

    TVector3 fgd1minFV = fgd1min + FVdefminFGD1;
    TVector3 fgd1maxFV = fgd1max - FVdefmaxFGD1;
    TVector3 fgd2minFV = fgd2min + FVdefminFGD2;
    TVector3 fgd2maxFV = fgd2max - FVdefmaxFGD2;

    // ------------ true vertex in fiducial volume --------------------------
    for(int i = 0; i < 2; i ++){
      fgd1FV_cutMC[i] = DefineVolume_Cut(fgd1minFV,fgd1maxFV,101,i);  // true vertex
      fgd2FV_cutMC[i] = DefineVolume_Cut(fgd2minFV,fgd2maxFV,101,i);
      fgdFV_cutMC[i] = "("+ fgd1FV_cutMC[i]+" || "+fgd2FV_cutMC[i]+")";
      
      scutmc[i] = fgdFV_cutMC[i];
      scutmcFGD1[i] = fgd1FV_cutMC[i];
      scutmcFGD2[i] = fgd2FV_cutMC[i];
    }

    // ----------- tracks with TPC constituents---- ---------------
    for(int i = 0; i < 2; i ++) scutin[i][0] = "ntracks[0]>0";

    // ----------- bunching  -------------------------------------------
    scutin[0][1] = "( bunch>0 && bunch<7 && abs(trPos[0][3]-tbunch) < 4*21.5 )";
    scutin[1][1] = "( bunch>0 && bunch<7 && abs(trPos[1][3]-tbunch) < 4*21.5 )";

    // ------------ vertex in fiducial volume --------------------------
    for(int i = 0; i < 2; i ++){
      fgd1FV_cut[i] = DefineVolume_Cut(fgd1minFV,fgd1maxFV,3,i);  // recon vertex
      fgd2FV_cut[i] = DefineVolume_Cut(fgd2minFV,fgd2maxFV,3,i);
      fgdFV_cut[i] = "("+ fgd1FV_cut[i]+" || "+fgd2FV_cut[i]+")";
    } 

    for(int i = 0; i < 2; i ++){
      scutin[i][2] = fgd1FV_cut[i];
      scutin[i][3] = fgd2FV_cut[i];
    }

    // ------------- Track quality -------------------------------------
    quality[0] = "(trTpcNHits[0][0]+trTpcNHits[0][1]+trTpcNHits[0][2]>18)";
    quality[1] = "(trTpcNHits[1][0]+trTpcNHits[1][1]+trTpcNHits[1][2]>18)";

    for(int i = 0; i < 2; i ++) scutin[i][4] = quality[i];

    // ----------- charge of the HM track --------------------------------
    //scutin[3] = "nneg[0]>=1";
    scutin[0][5] = "(trCharge[0] == -1)";
    scutin[1][5] = "(trCharge[1] == -1)";
    if(charge == 1){     
      scutin[0][5] = "(trCharge[0] == 1)";
      scutin[1][5] = "(trCharge[1] == 1)";
    }

    // ------------- PID quality -------------------------------------

    pidquality[0] = "((( (abs(pullcmu[0][0])<4 || abs(pullcele[0][0])<4 || abs(pullcp[0][0])<4) && pullcmu[0][0]!=0) || trDetUsed[0][0]==0) && (( (abs(pullcmu[0][1])<4 || abs(pullcele[0][1])<4 || abs(pullcp[0][1])<4) && pullcmu[0][1]!=0) || trDetUsed[0][1]==0) &&  (( (abs(pullcmu[0][2])<4 || abs(pullcele[0][2])<4 || abs(pullcp[0][2])<4) && pullcmu[0][2]!=0) || trDetUsed[0][2]==0))";
    pidquality[1] = "((( (abs(pullcmu[1][0])<4 || abs(pullcele[1][0])<4 || abs(pullcp[1][0])<4) && pullcmu[1][0]!=0) || trDetUsed[1][0]==0) && (( (abs(pullcmu[1][1])<4 || abs(pullcele[1][1])<4 || abs(pullcp[1][1])<4) && pullcmu[1][1]!=0) || trDetUsed[1][1]==0) &&  (( (abs(pullcmu[1][2])<4 || abs(pullcele[1][2])<4 || abs(pullcp[1][2])<4) && pullcmu[1][2]!=0) || trDetUsed[1][2]==0))";

    for(int i = 0; i < 2; i ++) scutin[i][6] = pidquality[i] ;

    // ----------------- muon PID ---------------------------------
    mupid[0] = "((probmu[0]+probpi[0])/(1-probk[0]-probp[0])>0.95)";
    mupid[0] = "((probmu[1]+probpi[1])/(1-probk[1]-probp[1])>0.95)";

    // Most energetic negative track should be a muon
    for(int i = 0; i < 2; i ++) scutin[i][7] = mupid[i];

    // ------------- track multiplicity --------------------
    for(int i = 0; i < 2; i ++) scutin[i][8] = "ntracks[0]<3";

    // ------------- negative track multiplicity ----------
    for(int i = 0; i < 2; i ++) scutin[i][9] = "nneg[0]==1";

    // ------------- no michel electron -------------------
    for(int i = 0; i < 2; i ++) scutin[i][10] = "nclusterscut[0][2]==0";

    // -------------- vertex cut. Make sure that both tracks have a common vertex -------------    
    scutin[0][11] = "((abs(trPos[0][0]-trPos[1][0])<50 && abs(trPos[0][1]-trPos[1][1])<50 && abs(trPos[0][2]-trPos[1][2])<30)) && " + scutin[1][1];
    scutin[1][11] = "((abs(trPos[0][0]-trPos[1][0])<50 && abs(trPos[0][1]-trPos[1][1])<50 && abs(trPos[0][2]-trPos[1][2])<30)) && " + scutin[0][1];

    // ------------ Positive Track quality -------------------------
    scutin[0][12] = quality[1] + " && trCharge[1]==1";
    scutin[1][12] = quality[0] + " && trCharge[0]==1";

    // ------------ Positive PID quality -------------------------
    scutin[0][13] = pidquality[1] + " && trCharge[1]==1";
    scutin[1][13] = pidquality[0] + " && trCharge[0]==1";

    // ------------- proton PID  ---------------------
    ppid[0] = "(probp[1]/(1-probk[1])>0.6)  && trCharge[1]==1";
    ppid[1] = "(probp[0]/(1-probk[0])>0.6)  && trCharge[0]==1";

    // Most energetic positive track should be a proton
    for(int i = 0; i < 2; i ++) scutin[i][14] = "(" + ppid[i] + ") ";

    // -------------- Delta_theta_p --------------------------------
    for(int i = 0; i < 2; i ++) scutin[i][15] = "(dthp[0]<0.6)";

    // -------------- Neutrino invariant mass ----------------------
    for(int i = 0; i < 2; i ++) scutin[i][16] = "(mnu>-0.4)";

    //============== Cumulative cuts ===================================
    for(int k = 0; k < 2; k ++){
      scut[k][0] = scutin[k][0];
      scut[k][1] = scut[k][0]   + " && " + scutin[k][1];
      scut[k][2] = scut[k][1]   + " && ("+scutin[k][2] +" || " + scutin[k][3] + ")";
      scut[k][3] = scut[k][2]   + " && " + scutin[k][4];
      scut[k][4] = scut[k][3]   + " && " + scutin[k][5];
      scut[k][5] = scut[k][4]   + " && " + scutin[k][6];
      scut[k][6] = scut[k][5]   + " && " + scutin[k][7];
      imupid = 6;
      icutCC = 6;
      scut[k][7]  = scut[k][6]  + " && "  + scutin[k][8];
      scut[k][8]  = scut[k][7]  + " && "  + scutin[k][9];
      scut[k][9]  = scut[k][8]  + " && "  + scutin[k][10];
      scut[k][10] = scut[k][9]  + " && (" + scutin[k][11] + " || npos[0]==0)";
      scut[k][11] = scut[k][10] + " && (" + scutin[k][12] + " || npos[0]==0)";
      scut[k][12] = scut[k][11] + " && (" + scutin[k][13] + " || npos[0]==0)";
      scut[k][13] = scut[k][11] + " && (" + scutin[k][14] + " || npos[0]==0)";
      ippid = 13;
      scut[k][14] = scut[k][13] + " && (" + scutin[k][15] + " || npos[0]==0)";
      scut[k][15] = scut[k][14] + " && (" + scutin[k][16] + " || npos[0]==0)";
      icutCCQE = 15;
    }
  }
};

#endif
