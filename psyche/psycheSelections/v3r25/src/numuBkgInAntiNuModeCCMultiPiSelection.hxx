#ifndef numuBkgInAntiNuModeCCMultiPiSelection_h
#define numuBkgInAntiNuModeCCMultiPiSelection_h

#include "numuCCMultiPiSelection.hxx"
#include "numuBkgInAntiNuModeCCSelection.hxx"

class numuBkgInAntiNuModeCCMultiPiSelection: public SelectionBase{
 public:
  numuBkgInAntiNuModeCCMultiPiSelection(bool forceBreak=true);
  virtual ~numuBkgInAntiNuModeCCMultiPiSelection(){}

  //---- These are mandatory functions
  void DefineSteps();
  void DefineDetectorFV();
  ToyBoxB* MakeToyBox(){return new ToyBoxCCMultiPi();}
  bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
  SampleId_h GetSampleId(){return SampleId::kFGD1NuMuBkgInAntiNuModeCC;}
  bool IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const;
  void InitializeEvent(AnaEventC& event);
  bool CheckRedoSelection(const AnaEventC& event, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep);
        
  Int_t GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;    
  Int_t GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;

  bool IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& box, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& box, AnaTrueObjectC* trueTrack, SystId_h systId, Int_t branch) const;


protected:
  
  numuBkgInAntiNuModeCCSelection _numuBkgInAntiNuModeCCSelection;
  numuCCMultiPiSelection _numuCCMultiPiSelection;
};

class FillSummaryAction_numuBkg_CCMultiPi: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;  
  StepBase* MakeClone(){return new FillSummaryAction_numuBkg_CCMultiPi();}
};

#endif
