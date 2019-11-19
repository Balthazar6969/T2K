#ifndef antiNumuCCMultiTrackFGD2Selection_h
#define antiNumuCCMultiTrackFGD2Selection_h

#include "SelectionBase.hxx"
#include "antiNumuCCFGD2Selection.hxx"
#include "antiNumuCCMultiTrackSelection.hxx"

class antiNumuCCMultiTrackFGD2Selection: public SelectionBase{
 public:
  antiNumuCCMultiTrackFGD2Selection(bool forceBreak=true);
  virtual ~antiNumuCCMultiTrackFGD2Selection(){}

  //---- These are mandatory functions
  void DefineSteps();
  void DefineDetectorFV();
  ToyBoxB* MakeToyBox() {return new ToyBoxTracker();}
  bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
  SampleId_h GetSampleId(){return SampleId::kFGD2AntiNuMuCC1Track;}
  bool IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const;
  void InitializeEvent(AnaEventC& event);
  bool CheckRedoSelection(const AnaEventC& event, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep);
  
  Int_t GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;
  Int_t GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;

protected:
  
  antiNumuCCFGD2Selection    _antiNumuCCFGD2Selection;
  antiNumuCCMultiTrackSelection _antiNumuCCMultiTrackSelection;

};

class AntiNuMuCCMultiTrackFGD2_FillSummaryAction: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;  
  StepBase* MakeClone(){return new AntiNuMuCCMultiTrackFGD2_FillSummaryAction();}
};

class AntiNuMuCCMultiTrackFGD2_CC1TrackCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new AntiNuMuCCMultiTrackFGD2_CC1TrackCut();}
};

class AntiNuMuCCMultiTrackFGD2_CCNTracksCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new AntiNuMuCCMultiTrackFGD2_CCNTracksCut();}
};
#endif
