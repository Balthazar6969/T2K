#ifndef NumuFlatTree_GeneralUtils_h
#define NumuFlatTree_GeneralUtils_h

class NumuFlatTree_GeneralUtils{

 private :
  // Create objects for class I need:

 public :
  // Start Defining methods:
  NumuFlatTree_GeneralUtils();

  void GetModeDefinition(int themode);

  int GetBeamRun(int run);

  TLorentzVector GetStartOfTrack(TLorentzVector back,  
				 TLorentzVector front, 
				 bool isForward);

  bool CheckTpcUsed(int tpc, int det);
  int GetGenieReactionCode(string reactionCode);
  double GetBunchTime(int bunchno);
  int GetBunch(double tTrack);
  void FillPosDirAtVertex(int itrk,
			  TLorentzVector backPos,
			  TLorentzVector frontPos,
			  TVector3 backDir,
			  TVector3 frontDir,
			  bool isForward,
			  TLorentzVector &posstart,
			  TLorentzVector &posend,
			  TVector3 &outdir,
			  double &outcostheta);
    
};
#endif
