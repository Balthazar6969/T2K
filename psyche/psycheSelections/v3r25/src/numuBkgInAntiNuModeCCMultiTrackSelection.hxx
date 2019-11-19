#ifndef numuBkgInAntiNuModeCCMultiTrackSelection_h
#define numuBkgInAntiNuModeCCMultiTrackSelection_h

#include "numuCCMultiPiSelection.hxx"
#include "numuBkgInAntiNuModeCCSelection.hxx"

class numuBkgInAntiNuModeCCMultiTrackSelection: public SelectionBase{
 public:
  numuBkgInAntiNuModeCCMultiTrackSelection(bool forceBreak=true);
  virtual ~numuBkgInAntiNuModeCCMultiTrackSelection(){}

  //---- These are mandatory functions
  void DefineSteps();
  void DefineDetectorFV();
  ToyBoxB* MakeToyBox(){return new ToyBoxTracker();}
  bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
  SampleId_h GetSampleId(){return SampleId::kFGD1NuMuBkgInAntiNuModeCC;}
  bool IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const;
  void InitializeEvent(AnaEventC& event);
  bool CheckRedoSelection(const AnaEventC& event, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep);

  Int_t GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;
  Int_t GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;

protected:
  
  numuBkgInAntiNuModeCCSelection _numuBkgInAntiNuModeCCSelection;
  numuCCMultiPiSelection _numuCCMultiPiSelection;
};

class FillSummaryAction_numuBkg_CCMultiTrack: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;  
  StepBase* MakeClone(){return new FillSummaryAction_numuBkg_CCMultiTrack();}
};

#endif
