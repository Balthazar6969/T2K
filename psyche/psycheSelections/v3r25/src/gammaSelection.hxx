#ifndef gammaSelection_h
#define gammaSelection_h

#include "SelectionBase.hxx"
#include "Parameters.hxx"
#include "ToyBoxTracker.hxx"
#include "SystId.hxx"
#include "SubDetId.hxx"

#include "nueCCSelection.hxx"

class gammaSelection: public SelectionBase{
public:
  gammaSelection(bool forceBreak=true);
  virtual ~gammaSelection(){}

  //---- These are mandatory functions
  void DefineSteps();
  void DefineDetectorFV();
  ToyBoxB* MakeToyBox(){return new ToyBoxNueCC();}

  void InitializeEvent(AnaEventC& event);
  bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
  SampleId_h GetSampleId(){return SampleId::kFGD1Gamma;}

  bool CheckRedoSelection(const AnaEventC& eventC, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep);
  bool IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* trackRecC, SystId_h systId, Int_t branch) const;
  bool IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueTrack, SystId_h systId, Int_t branch) const;
  bool IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const;

  Int_t GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;
  Int_t GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;



protected:
  Bool_t _useVetoes;
  nueCCSelection _nueCCSelection;

  Int_t _FindLeadingTracksStepIndex;
  Int_t _TotalMultiplicityCutIndex;
  Int_t _ElecPIDCutIndex;
  Int_t _ElecPIDStepIndex;

};

class FillSummaryActionGamma: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& eventC, ToyBoxB& boxB) const;  
  StepBase* MakeClone(){return new FillSummaryActionGamma();}
};

class FindLeadingTracksAction_gamma: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& eventC, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new FindLeadingTracksAction_gamma();}
};

class PairTrackCut_gamma: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& eventC, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new PairTrackCut_gamma();}
};

class PrimaryTrackQualityCut_gamma: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& eventC, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new PrimaryTrackQualityCut_gamma();}
};

class MinvCut_gamma: public StepBase{
public:
  MinvCut_gamma(){
    _inv_mass_min = (Float_t)ND::params().GetParameterD("psycheSelections.gammaAnalysis.Cuts.PairFinder.InvMassMin");
  }
  using StepBase::Apply;
  bool Apply(AnaEventC& eventC, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new MinvCut_gamma();}
protected:
  Float_t _inv_mass_min;
};

class PIDCut_gamma: public StepBase{
public:
  PIDCut_gamma(){
    _elepullpri_min = (Float_t)ND::params().GetParameterD("psycheSelections.gammaAnalysis.Cuts.PID.PullElecMin");
    _elepullpri_max = (Float_t)ND::params().GetParameterD("psycheSelections.gammaAnalysis.Cuts.PID.PullElecMax");
  }
  using StepBase::Apply;
  bool Apply(AnaEventC& eventC, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new PIDCut_gamma();}
protected:
  Float_t _elepullpri_min;
  Float_t _elepullpri_max;
};

#endif
