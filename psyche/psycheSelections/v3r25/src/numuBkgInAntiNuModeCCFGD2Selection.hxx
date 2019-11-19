#ifndef numuBkgInAntiNuModeCCFGD2Selection_h
#define numuBkgInAntiNuModeCCFGD2Selection_h

#include "numuCCFGD2Selection.hxx"
#include "numuBkgInAntiNuModeCCSelection.hxx"


class numuBkgInAntiNuModeCCFGD2Selection: public SelectionBase{
 public:
  numuBkgInAntiNuModeCCFGD2Selection(bool forceBreak=true);
  virtual ~numuBkgInAntiNuModeCCFGD2Selection(){}

  //---- These are mandatory functions
  void DefineSteps();
  void DefineDetectorFV();
  ToyBoxB* MakeToyBox(){return new ToyBoxTracker();}
  bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
  SampleId_h GetSampleId(){return SampleId::kFGD2NuMuBkgInAntiNuModeCC;}
  bool IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const;
  void InitializeEvent(AnaEventC& event);
  bool CheckRedoSelection(const AnaEventC& event, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep);

  Int_t GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;
  Int_t GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;
 
  // Fine tuning in toy level
  bool IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& box, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& box, AnaTrueObjectC* trueTrack, SystId_h systId, Int_t branch) const;
  

protected:
  
  numuCCFGD2Selection _numuCCFGD2Selection;
  numuBkgInAntiNuModeCCSelection _numuBkgInAntiNuModeCCSelection;
};

class FillSummaryAction_numuBkgFGD2: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;  
  StepBase* MakeClone(){return new FillSummaryAction_numuBkgFGD2();}
};

#endif
