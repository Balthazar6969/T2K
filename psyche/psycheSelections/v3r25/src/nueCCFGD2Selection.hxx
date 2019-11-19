#ifndef nueCCFGD2Selection_h
#define nueCCFGD2Selection_h

#include "SelectionBase.hxx"
#include "Parameters.hxx"
#include "ToyBoxTracker.hxx"
#include "SystId.hxx"
#include "SubDetId.hxx"

#include "nueCCSelection.hxx"

class nueCCFGD2Selection: public SelectionBase{
 public:
  nueCCFGD2Selection(bool forceBreak=true);
  virtual ~nueCCFGD2Selection(){}

  //---- These are mandatory functions
  void DefineSteps();
  void DefineDetectorFV();
  ToyBoxB* MakeToyBox(){return new ToyBoxNueCC();}

  void InitializeEvent(AnaEventC& event);
  bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
  SampleId_h GetSampleId(){return SampleId::kFGD2NuECC;}

  bool CheckRedoSelection(const AnaEventC& eventC, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep);
  bool IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* track, SystId_h systId, Int_t branch) const;
  bool IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueTrack, SystId_h systId, Int_t branch) const;
  bool IsRelevantRecObjectForSystematicInToy(const AnaEventC&, const ToyBoxB&, AnaRecObjectC*, SystId_h systId, Int_t branch=0) const;
  bool IsRelevantTrueObjectForSystematicInToy(const AnaEventC&, const ToyBoxB&, AnaTrueObjectC*, SystId_h systId, Int_t branch=0) const;
  bool IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box,SystId_h systId , Int_t branch) const;

  Int_t GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;
  Int_t GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;

protected:
  nueCCSelection _nueCCSelection;
  Int_t _FindLeadingTracksStepIndex;
  Int_t _TotalMultiplicityCutIndex;
  Int_t _ElecPIDCutIndex;
  Int_t _ElecPIDStepIndex;
};


class FillSummaryAction_nueCCFGD2Selection: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;  
  StepBase* MakeClone(){return new FillSummaryAction_nueCCFGD2Selection();}
};

class EOverPCut: public StepBase{
public:
  EOverPCut(){
    _eoverp_threshold = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.PID.EOverPThreshold");
    _eoverp_minmom    = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.PID.EOverPMinMomentum");
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new EOverPCut();}
protected:
  Float_t _eoverp_threshold;
  Float_t _eoverp_minmom;
};

class P0DFGD1VetoCut: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new P0DFGD1VetoCut();}
};

class TPCVetoCut_FGD2: public StepBase{
public:
  TPCVetoCut_FGD2(){
    _delta_z_min = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.TPCVeto.DeltaZMin.FGD2");
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new TPCVetoCut_FGD2();}
protected:
  Float_t _delta_z_min;
};

class ExternalFGD2layersCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new ExternalFGD2layersCut();}
};

#endif
