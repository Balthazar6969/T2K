#include "NumuFlatTree_GeneralUtils.h"

// This class contains the following method:
//  1) GetModeDefinition
//  2) GetBeamRun
//  3) GetStartOfTrack
//  4) CheckTpcUsed
//  5) GetGenieReactionCode
//  6) GetBunchTime
//  7) GetBunch
//

NumuFlatTree_GeneralUtils::NumuFlatTree_GeneralUtils(){
  cout << "Defining class NumuFlatTree_GeneralUtils " << endl;
}


// Returns which mode were run / Define you module here 
//********************************
void NumuFlatTree_GeneralUtils::GetModeDefinition(int themode){
  
  //Returns the definition of which mode have been run
  // This follow a binary scheme:
  // If you want to run modules 1, 3 and 4, it looks like you want to run
  // modules: 1101 which is 13 in binary so you need to set the mode to 13.
  //          ||||
  //          4321

  int NbModule = 0;
 p_Module_g10a = 1 << NbModule++;
 p_Module_bbb  = 1 << NbModule++;
 p_Module_ccc  = 1 << NbModule++;
 p_Module_ddd  = 1 << NbModule++;

 cout << "EXPLANATION OF MODE SELECTION:" << endl;
 cout << "  Sum the the numbers associated to the modules that you need" << endl;
 cout << "  to come up with your running mode: " << endl;
 cout << "    Module_g10a : " << p_Module_g10a << endl;
 cout << "    Module_bbb : "  << p_Module_bbb << endl;
 cout << "    Module_ccc : "  << p_Module_ccc << endl;
 cout << "    Module_ddd : "  << p_Module_ddd << endl;
 
 
 
 cout << "***************************************" << endl;
 cout << "  You chose to run with mode = " << themode << endl;
 cout << "  which selects the following modules:" << endl;
 if(themode==0) {
   cout << "    Mode = 0 : Standard mode only" << endl;
 }
 
 if((themode & p_Module_g10a)==p_Module_g10a) { 
   cout << "     - The global analysis 2010a (g10a)" << endl;
 }
 if((themode & p_Module_bbb)==p_Module_bbb) { 
   cout << "     - The module Test BBB (bbb)" << endl;
 }
 if((themode & p_Module_ccc)==p_Module_ccc) { 
   cout << "     - The module Test CCC (ccc)" << endl;
 }
 if((themode & p_Module_ddd)==p_Module_ddd) { 
   cout << "     - The module Test DDD (ddd)" << endl;
 }
}


//Method that returns the beam run number
//**********************************************************
int NumuFlatTree_GeneralUtils::GetBeamRun(int run){

  int brun = 0;

  if(run < 4300) brun = 0;
  else if(run < 4600) brun = 1;
  else if(run < 4900) brun = 2;
  else brun = 3;
  
  return brun;
}


//Finds the start position
//**********************************************************
TLorentzVector NumuFlatTree_GeneralUtils::GetStartOfTrack(TLorentzVector back,  
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


//**********************************************************
bool NumuFlatTree_GeneralUtils::CheckTpcUsed(int tpc, int det){ 

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
int NumuFlatTree_GeneralUtils::GetGenieReactionCode(string reactionCode){

  int code = 0;
  
  //if(!p_silent) cout << "3: " << reactionCode << endl;
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
double NumuFlatTree_GeneralUtils::GetBunchTime(int bunchno){

  double bunchtime = -999.;

  if(p_isMC){
    bunchtime = p_time_mc[bunchno];
  }
  else{
    bunchtime = p_time_data[bunchno];
  }

  return bunchtime;
}


//
//**********************************************************
int NumuFlatTree_GeneralUtils::GetBunch(double tTrack){

  int bunch = 8;

  // ---- For MC rec and data -----------
  // Remove half the distance between next peak and current one
  // in order to have lower edge of the bunch

  double time_mc[p_NBUNCH-2];
  double time_data[p_NBUNCH-2];


  // Deal with first 5 bunches
  for(int d = 0; d < p_NBUNCH-4; d ++){
    time_mc[d] = p_time_mc[d+1]-(0.5*(p_time_mc[d+2]-p_time_mc[d+1]));
    time_data[d] = p_time_data[d+1]-(0.5*(p_time_data[d+2]-p_time_data[d+1]));
  }
  //  Deal with lower edge of last two bins separatly:
  time_mc[p_NBUNCH-4] = p_time_mc[p_NBUNCH-3]-(0.5*(p_time_mc[p_NBUNCH-3]-p_time_mc[p_NBUNCH-4]));
  time_data[p_NBUNCH-4] = p_time_data[p_NBUNCH-3]-(0.5*(p_time_data[p_NBUNCH-3]-p_time_data[p_NBUNCH-4]));

  time_mc[p_NBUNCH-3] = p_time_mc[p_NBUNCH-3]+(0.5*(p_time_mc[p_NBUNCH-3]-p_time_mc[p_NBUNCH-4]));
  time_data[p_NBUNCH-3] = p_time_data[p_NBUNCH-3]+(0.5*(p_time_data[p_NBUNCH-3]-p_time_data[p_NBUNCH-4]));


//  for(int d = 0; d < p_NBUNCH-2; d ++){
//    cout << "check MC: " << time_mc[d] <<  " " << d << endl;
//  }
//  for(int d = 0; d < p_NBUNCH-2; d ++){
//    cout << "check DATA: " << time_data[d] <<  " " << d << endl;
//  }
//

  //  double time_mc[7]    = {-202.5, 379.5, 961.5, 1543.5, 2125.5, 2707.5, 3289.5};
  //  double time_data[7]  = {3066.2, 3648.2, 4230.2, 4812.2, 5394.2, 5976.2, 6558.2};
  
  if(p_isMC){
        
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
    // Check for time being exactly zero:
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
    // check for time being exactly zero
    if(fabs(tTrack) < 1e-5){
      bunch = 0;
    }
  }

  return bunch;  
}


//
//**********************************************************
void NumuFlatTree_GeneralUtils::FillPosDirAtVertex(int itrk, 
						   TLorentzVector backPos,  
						   TLorentzVector frontPos, 
						   TVector3 backDir,  
						   TVector3 frontDir, 
						   bool isForward,
						   TLorentzVector &posstart,
						   TLorentzVector &posend,
						   TVector3 &outdir,
						   double &outcostheta){
  
  double oa_angle = 2.5*TMath::DegToRad();
  TVector3 nuDir(0, sin(oa_angle), cos(oa_angle));
  
  double frontzi = frontPos.Z();
  double backzi = backPos.Z();
  // Forwards going track
  if((isForward && (frontzi < backzi)) || (!isForward && (frontzi > backzi))){
    for(int c = 0; c < 4; c ++){
      
      posstart[c] = frontPos[c];
      posend[c] = backPos[c];
    }
    outdir = frontDir;
    outcostheta = frontDir*nuDir;
    
  }
  // Backwards going track
  else{
    for(int c = 0; c < 4; c ++){
      posstart[c] = backPos[c];
      posend[c] = frontPos[c];
    }
    outdir = backDir;
    outcostheta = backDir*nuDir;
  }
}




