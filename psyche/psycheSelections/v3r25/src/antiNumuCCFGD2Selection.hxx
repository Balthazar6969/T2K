#ifndef antiNumuCCFGD2Selection_h
#define antiNumuCCFGD2Selection_h

#include "antiNumuCCSelection.hxx"
#include "numuCCFGD2Selection.hxx"

class antiNumuCCFGD2Selection: public SelectionBase{
 public:
  antiNumuCCFGD2Selection(bool forceBreak=true);
  virtual ~antiNumuCCFGD2Selection(){}

  //---- These are mandatory functions
  void DefineSteps();
  void DefineDetectorFV();
  ToyBoxB* MakeToyBox(){return new ToyBoxTracker();}
  bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
  SampleId_h GetSampleId(){return SampleId::kFGD2AntiNuMuCC;}
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
  
  antiNumuCCSelection _antiNumuCCSelection;
  
};

class FillSummaryAction_antiNumuCCFGD2: public StepBase{
public:
using StepBase::Apply;
bool Apply(AnaEventC& event, ToyBoxB& box) const;  
StepBase* MakeClone(){return new FillSummaryAction_antiNumuCCFGD2();}
};


class ExternalFGD2VetoCut_antiNumuCCFGD2: public StepBase{
public:
using StepBase::Apply;
bool Apply(AnaEventC& event, ToyBoxB& box) const;  
StepBase* MakeClone(){return new ExternalFGD2VetoCut_antiNumuCCFGD2();}
};

//class PositiveMultiplicityCut_antiNumuCCFGD2: public StepBase{
//public:
//using StepBase::Apply;
//bool Apply(AnaEventC& event, ToyBoxB& box) const;  
//StepBase* MakeClone(){return new PositiveMultiplicityCut_antiNumuCCFGD2();}
//};

//class AntiNuMuFGD2HighestMomentumTrack: public StepBase{
//public:
//using StepBase::Apply;
//bool Apply(AnaEventC& event, ToyBoxB& box) const;  
//StepBase* MakeClone(){return new AntiNuMuFGD2HighestMomentumTrack();}
//};


//class AntiNuMuFGD2NegativeMuonVeto: public StepBase{
//public:
//using StepBase::Apply;
//bool Apply(AnaEventC& event, ToyBoxB& box) const;  
//StepBase* MakeClone(){return new AntiNuMuFGD2NegativeMuonVeto();}
//};


//class AntiNuMuFGD2FGDLayer: public StepBase{
//public:
//using StepBase::Apply;
//bool Apply(AnaEventC& event, ToyBoxB& box) const;  
//StepBase* MakeClone(){return new AntiNuMuFGD2FGDLayer();}
//};


//class AntiNuMuFGD2MuPlusMomentum: public StepBase{
//public:
//using StepBase::Apply;
//bool Apply(AnaEventC& event, ToyBoxB& box) const;  
//StepBase* MakeClone(){return new AntiNuMuFGD2MuPlusMomentum();}
//};



#endif
