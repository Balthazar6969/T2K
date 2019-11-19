#ifndef antiNueCCFGD2Selection_h
#define antiNueCCFGD2Selection_h

#include "SelectionBase.hxx"
#include "Parameters.hxx"
#include "ToyBoxTracker.hxx"
#include "SystId.hxx"
#include "SubDetId.hxx"

#include "nueCCFGD2Selection.hxx"

class antiNueCCFGD2Selection: public SelectionBase{
 public:
  antiNueCCFGD2Selection(bool forceBreak=true);
  virtual ~antiNueCCFGD2Selection(){}

  //---- These are mandatory functions
  void DefineSteps();
  void DefineDetectorFV();
  ToyBoxB* MakeToyBox(){return new ToyBoxNueCC();}

  void InitializeEvent(AnaEventC& event);
  bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
  SampleId_h GetSampleId(){return SampleId::kFGD2AntiNuECC;}

  bool CheckRedoSelection(const AnaEventC& eventC, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep);
  bool IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* track, SystId_h systId, Int_t branch) const;
  bool IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueTrack, SystId_h systId, Int_t branch) const;
  bool IsRelevantRecObjectForSystematicInToy(const AnaEventC&, const ToyBoxB&, AnaRecObjectC*, SystId_h systId, Int_t branch=0) const;
  bool IsRelevantTrueObjectForSystematicInToy(const AnaEventC&, const ToyBoxB&, AnaTrueObjectC*, SystId_h systId, Int_t branch=0) const;
  bool IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box,SystId_h systId , Int_t branch) const;

  Int_t GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;
  Int_t GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;


protected:
  nueCCFGD2Selection _nueCCFGD2Selection;

  Int_t _FindLeadingTracksStepIndex;
  Int_t _TotalMultiplicityCutIndex;
  Int_t _ElecPIDCutIndex;
  Int_t _ElecPIDStepIndex;

  // Variables needed from datacard
  Float_t _protonregion_low;
};

class FillSummaryAction_antiNueCCFGD2Selection: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;  
  StepBase* MakeClone(){return new FillSummaryAction_antiNueCCFGD2Selection();}
};

#endif
