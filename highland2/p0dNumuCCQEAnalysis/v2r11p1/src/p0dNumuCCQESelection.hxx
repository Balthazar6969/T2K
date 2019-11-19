#ifndef p0dNumuCCQESelection_h
#define p0dNumuCCQESelection_h

#include "SelectionBase.hxx"

class p0dNumuCCQESelection: public SelectionBase{
public:
  p0dNumuCCQESelection(bool forceBreak=true);
  virtual ~p0dNumuCCQESelection(){}

  //========= These are mandatory functions ==================

  void DefineSteps();
  void DefineDetectorFV();
  ToyBoxB* MakeToyBox() {return new ToyBoxTracker();}
  bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
  /// Fill the EventBox with the objects needed by this selection
  virtual void InitializeEvent(AnaEventC&);
  SampleId_h GetSampleId(){return SampleId::kP0DNuMuCC;}
  //==========================================================
};

class SingleP0DPIDCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new SingleP0DPIDCut();}
};

class MultipleP0DPIDCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new MultipleP0DPIDCut();}
};
#endif
