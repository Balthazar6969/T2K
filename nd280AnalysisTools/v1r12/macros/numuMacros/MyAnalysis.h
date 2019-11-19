#ifndef MyAnalysis_h
#define MyAnalysis_h

#include "AnalysisBase.h"

class MyAnalysis: public AnalysisBase {
 protected:  


  int _level[7];
  int _accum_level;

 public:

  MyAnalysis(const std::string& file);
  virtual ~MyAnalysis(){}

  void Initialize();
  void InitializeEvent();
  void Finalize();

  void FinalizeEvent();
  void FinalizeConfiguration();
  void FinalizeAnalysis();

  bool Process();
};

#endif
