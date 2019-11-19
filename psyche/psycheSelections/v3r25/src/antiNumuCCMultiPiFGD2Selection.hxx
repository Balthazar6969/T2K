#ifndef antiNumuCCMultiPiFGD2Selection_h
#define antiNumuCCMultiPiFGD2Selection_h

#include "SelectionBase.hxx"
#include "Parameters.hxx"
#include "antiNumuCCMultiPiSelection.hxx"
#include "antiNumuCCFGD2Selection.hxx"

class antiNumuCCMultiPiFGD2Selection: public SelectionBase{

public:
  antiNumuCCMultiPiFGD2Selection(bool forceBreak=true);
  virtual ~antiNumuCCMultiPiFGD2Selection(){}

  void DefineSteps();
  void DefineDetectorFV();
  ToyBoxB* MakeToyBox(){return new ToyBoxAntiCCMultiPi();}
  bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
  SampleId_h GetSampleId(){return SampleId::kFGD2AntiNuMuCC0Pi;}
  bool IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const;
  void InitializeEvent(AnaEventC& event);
  bool CheckRedoSelection(const AnaEventC& event, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep);
  
  bool IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& box, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& box, AnaTrueObjectC* trueTrack, SystId_h systId, Int_t branch) const;
  
  Int_t GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;    
  Int_t GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;



protected:
  antiNumuCCMultiPiSelection _antiNumuCCMultiPiSelection;
  antiNumuCCFGD2Selection _antiNumuCCFGD2Selection;

};

class FillSummaryAction_antiNumuCCMultiPiFGD2: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;  
  StepBase* MakeClone(){return new FillSummaryAction_antiNumuCCMultiPiFGD2();}
};

#endif
