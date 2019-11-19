#ifndef psycheND280Tools_h
#define psycheND280Tools_h

#include "ND280AnalysisUtils.hxx"
#include "ConstituentsUtils.hxx"


/* This are utilities that can be instantiated from an interactive root session. Also from the code */

class psycheND280Tools{
  
public:

  psycheND280Tools(){}
  virtual ~psycheND280Tools(){}

  /// Calculate the number of targets
  /// int target_nucleons: 1 for protons, 2 for neutrons, 3 for both
  /// anaUtils::massComponentEnum: kFGD1=1, kFGD2=2, kFGDs=3, kFGD2xymodules=4, kFGD2watermodules=5, kP0Dwater=6, kP0Dair=7, kFGD2waterlike=8, kFGD2xylike=9, kInvalid=-1

  double GetNTargets(anaUtils::massComponentEnum massComponent = anaUtils::kFGD1, int target_nucleons = 3)
  {return anaUtils::GetNTargets(massComponent,target_nucleons);}

  double GetNucleonsPerNucleus(anaUtils::massComponentEnum massComponent = anaUtils::kFGD1, int target_nucleons = 3)
  {return anaUtils::GetNucleonsPerNucleus(massComponent,target_nucleons);}

  double GetAreaFV(anaUtils::massComponentEnum massComponent = anaUtils::kFGD1)
  {return anaUtils::GetAreaFV(massComponent);}

  void DumpFV() {FVDef::DumpFV();}
  void DumpVolume() {DetDef::DumpVolumes();}
  void DumpFGDInfo() {DetDef::DumpFGDInfo();}

};


#endif
