#ifndef p0dNumuCCSelection_h
#define p0dNumuCCSelection_h

#include "SelectionBase.hxx"
#include "numuCCSelection.hxx"
#include "EventBoxUtils.hxx"

class p0dNumuCCSelection: public SelectionBase{
public:
  p0dNumuCCSelection(bool forceBreak=true);
  virtual ~p0dNumuCCSelection(){}

  //========= These are mandatory functions ==================

  void DefineSteps();
  void DefineDetectorFV();
  ToyBoxB* MakeToyBox() {return new ToyBoxTracker();}
  bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
  /// Fill the EventBox with the objects needed by this selection
  virtual void InitializeEvent(AnaEventC&);
  SampleId_h GetSampleId(){return SampleId::kP0DNuMuCC;}
 
  Int_t GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;
  Int_t GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;

 
  //==========================================================
};

class FindP0DLeadingTracksAction: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FindP0DLeadingTracksAction();}
};

class FindP0DVetoAction: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FindP0DVetoAction();}
};

class P0DSelectionVetoCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new P0DSelectionVetoCut();}
};

#endif
