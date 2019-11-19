#ifndef numuCCSelection_h
#define numuCCSelection_h

#include "SelectionBase.hxx"
#include "Parameters.hxx"
#include "ToyBoxTracker.hxx"
#include "EventBoxId.hxx"
#include "SystId.hxx"
#include "SubDetId.hxx"
#include "BasicUtils.hxx"

class numuCCSelection: public SelectionBase{
 public:
  
  numuCCSelection(bool forceBreak=true);
  virtual ~numuCCSelection(){}

  //---- These are mandatory functions
  void DefineSteps();
  void DefineDetectorFV();
  ToyBoxB* MakeToyBox(){return new ToyBoxTracker();}
  bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
  SampleId_h GetSampleId(){return SampleId::kFGD1NuMuCC;}
  bool IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantRecObjectForSystematicInToy(const AnaEventC&, const ToyBoxB&, AnaRecObjectC*, SystId_h systId, Int_t branch=0) const;
  bool IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantTrueObjectForSystematicInToy(const AnaEventC&, const ToyBoxB&, AnaTrueObjectC*, SystId_h systId, Int_t branch=0) const;
  bool IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const;
  void InitializeEvent(AnaEventC& event);
  bool CheckRedoSelection(const AnaEventC& event, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep);

  Int_t GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const; 
  Int_t GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;

protected:

  Int_t _MuonPIDCutIndex;
  Int_t _MuonPIDStepIndex;
  Int_t _FindLeadingTracksStepIndex;
  Int_t _TotalMultiplicityCutIndex;


};

class FillSummaryAction_numuCC: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;  
  StepBase* MakeClone(){return new FillSummaryAction_numuCC();}
};

class TrackQualityFiducialCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new TrackQualityFiducialCut();}
};

class TotalMultiplicityCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new TotalMultiplicityCut();}
};


class MuonPIDCut: public StepBase{
 public:
  MuonPIDCut(){
      _prod5Cut = (bool) ND::params().GetParameterI("psycheSelections.numuCCMultiPi.Prod5Cuts");
  }
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new MuonPIDCut();}
 private:
  bool _prod5Cut;
};

class ExternalVetoCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new ExternalVetoCut();}
};

class ExternalFGD1lastlayersCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new ExternalFGD1lastlayersCut();}
};

/// Leading tracks with good quality in FGD1
class FindLeadingTracksAction: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FindLeadingTracksAction();}
};

  /// Find the Vertex. For the moment it's just the Star position of the HM track
class FindVertexAction: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FindVertexAction();}
};


class FindVetoTrackAction: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FindVetoTrackAction();}
};

class FindOOFVTrackAction: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FindOOFVTrackAction();}
};


#endif
