#ifndef SystematicsAnalysis_h
#define SystematicsAnalysis_h

#include "NumuFlatTreeTools.h"
//#include "oaAnalysisTreeTools.h"
#include "DrawingTools.h"
#include "TRandom3.h"


class SystematicsAnalysis: public DrawingTools {
 protected:
  
  TRandom3 fRandomGenerator; 

  std::map<std::pair<int,int>,std::pair<double,double> > _CT_correction[3]; 

  bool _pull_computed;
  double _CTexp[5];
  double _CTerr[5];

  bool _systematics_enabled;

  std::string _CT_correction_file;

  NumuFlatTreeTools* _flatTreeTools;
  //  oaAnalysisTreeTools* _oaAnalysisTreeTools;

 public:

  SystematicsAnalysis(){}
  SystematicsAnalysis(const std::string& file);
  virtual ~SystematicsAnalysis(){}

  void Build();

  void InitializeAnalysisParameters();


  NumuFlatTreeTools& GetTreeTools(){return *_flatTreeTools;}
  //  oaAnalysisTreeTools& GetTreeTools(){return *_oaAnalysisTreeTools;}

  virtual void Initialize();
  virtual bool Process();

  bool RecalculatePullQuantities(int itrk, int tpc);
  double TrackMomError(double momentum, double momerr, double Mparticle);
  double ExpecteddEdx(double bg);
  //  double GetTpcPullmuMomBased(int momentum, int tpc);
  //  double GetTpcMisCalibration(int tpc, int run);

  double GetCharge(int itrk);
  double GetMomentum(int itrk);
  double GetMomentumRelError(int itrk){return GetTreeTools().GetMomentumRelError(itrk)/GetParameter("mom_scale",GetAnalysisIndex());}
  double GetTpcMomentum(int itrk, int tpc);
  double GetTpcMomentumForPull(int itrk, int tpc);
  double GetTpcMomentumRelError(int itrk, int tpc){return GetTreeTools().GetTpcMomentumRelError(itrk,tpc)/GetParameter("mom_scale",GetAnalysisIndex());}

  double GetTpcPullmu(int itrk, int tpc);
  double GetTpcPullpi(int itrk, int tpc);
  double GetTpcPullele(int itrk, int tpc);
  double GetTpcPullp(int itrk, int tpc);

  double GetTpcCT(int itrk, int tpc);

  double GetTpcCTexpmu(int itrk, int tpc);
  double GetTpcCTexppi(int itrk, int tpc);
  double GetTpcCTexpele(int itrk, int tpc);
  double GetTpcCTexpp(int itrk, int tpc);

  double GetTpcCTerrmu(int itrk, int tpc);



  double GetDeltaChargeMisId(int itrk);

  int GetCTCorrection(int run, int subrun, int tpc, double& deltaCT, double& deltaCT_error);
  bool ReadCTCorrections(const std::string& filename);

  bool SystematicsEnabled(){return _systematics_enabled;}
  void EnableSystematics(){_systematics_enabled=true;}
  void DisableSystematics(){_systematics_enabled=false;}

  void SetCTCorrectionFile(const std::string& file){_CT_correction_file=file;}



};

#endif
