#ifndef gammaFGD2Selection_h
#define gammaFGD2Selection_h

#include "SelectionBase.hxx"
#include "Parameters.hxx"
#include "ToyBoxTracker.hxx"
#include "SystId.hxx"
#include "SubDetId.hxx"

#include "gammaSelection.hxx"

class gammaFGD2Selection: public SelectionBase{
public:
  gammaFGD2Selection(bool forceBreak=true);
  virtual ~gammaFGD2Selection(){}

  //---- These are mandatory functions
  void DefineSteps();
  void DefineDetectorFV();
  ToyBoxB* MakeToyBox(){return new ToyBoxNueCC();}

  void InitializeEvent(AnaEventC& event);
  bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
  SampleId_h GetSampleId(){return SampleId::kFGD2Gamma;}

  bool CheckRedoSelection(const AnaEventC& eventC, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep);
  bool IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* track, SystId_h systId, Int_t branch) const;
  bool IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueTrack, SystId_h systId, Int_t branch) const;
  bool IsRelevantRecObjectForSystematicInToy(const AnaEventC&, const ToyBoxB&, AnaRecObjectC*, SystId_h systId, Int_t branch=0) const;
  bool IsRelevantTrueObjectForSystematicInToy(const AnaEventC&, const ToyBoxB&, AnaTrueObjectC*, SystId_h systId, Int_t branch=0) const;
  bool IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const;
 
  Int_t GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;
  Int_t GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;


protected:
  Bool_t _useVetoes;
  gammaSelection _gammaSelection;

  Int_t _FindLeadingTracksStepIndex;
  Int_t _TotalMultiplicityCutIndex;
  Int_t _ElecPIDCutIndex;
  Int_t _ElecPIDStepIndex;
};

class FillSummaryActionGamma_FGD2: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& eventC, ToyBoxB& boxB) const;  
  StepBase* MakeClone(){return new FillSummaryActionGamma_FGD2();}
};

class FindFGD1PairsAction_gamma: public StepBase{
public:
  FindFGD1PairsAction_gamma(){
    _delta_pos_max = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.PairFinder.DeltaPosMax");
    _pullel_min    = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.PairFinder.PullElecMin");
    _pullel_max    = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.PairFinder.PullElecMax");
  }
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new FindFGD1PairsAction_gamma();}
protected:
  Float_t _delta_pos_max;
  Float_t _pullel_min;
  Float_t _pullel_max;
};

class FGD1PairCut_gamma: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new FGD1PairCut_gamma();}
};
#endif
