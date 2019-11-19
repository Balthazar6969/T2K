#ifndef antiNumuCCIncFwdSelection_h
#define antiNumuCCIncFwdSelection_h

#include "SelectionBase.hxx"
#include "antiNumuCCSelection.hxx"

class antiNumuCCIncFwdSelection: public SelectionBase{
    public:
        antiNumuCCIncFwdSelection(bool forceBreak=true);
        virtual ~antiNumuCCIncFwdSelection(){}

        //---- These are mandatory functions
        void DefineSteps();
        void DefineDetectorFV();
        ToyBoxB* MakeToyBox(){return new ToyBoxTracker();}
        bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
        SampleId::SampleEnum GetSampleEnum(){return SampleId::kFGD1AntiNuMuCC;}
        bool IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const;
        bool IsRelevantRecObjectForSystematicInToy(const AnaEventC&, const ToyBoxB&, AnaRecObjectC*, SystId_h systId, Int_t branch=0) const;
        bool IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const;
        bool IsRelevantTrueObjectForSystematicInToy(const AnaEventC&, const ToyBoxB&, AnaTrueObjectC*, SystId_h systId, Int_t branch=0) const;
        bool IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const;
        Int_t GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const; 
        Int_t GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;
        void InitializeEvent(AnaEventC& event);
        bool CheckRedoSelection(const AnaEventC& event, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep);

    protected:
       antiNumuCCSelection _antiNumuCCSelection;

       Int_t _AntiMuonPIDCutIndex;
       Int_t _AntiMuonPIDStepIndex;
       Int_t _FindLeadingTracksStepIndex;
       Int_t _TotalMultiplicityCutIndex;
};

class OOFVTECalFGD1Cut: public StepBase{
public:
  OOFVTECalFGD1Cut(){
    _time_main_ecal_min = (Float_t)ND::params().GetParameterD("antiNumuCCIncFwdAnalysis.Cuts.OOFV.TimeTECalMin");
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new OOFVTECalFGD1Cut();}
private:
  Float_t _time_main_ecal_min;
};

class OOFVFGD2FGD1Cut: public StepBase{
public:
  OOFVFGD2FGD1Cut(){
    _time_main_fgd2_min = (Float_t)ND::params().GetParameterD("antiNumuCCIncFwdAnalysis.Cuts.OOFV.TimeFGD2Min");
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new OOFVFGD2FGD1Cut();}
private:
  Float_t _time_main_fgd2_min;
};

class OOFVOtherCut: public StepBase{
public:
  OOFVOtherCut(){
    _time_other_min = (Float_t)ND::params().GetParameterD("antiNumuCCIncFwdAnalysis.Cuts.OOFV.TimeOtherMin");
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new OOFVOtherCut();}
private:
  Float_t _time_other_min;
};

class TPCAntiMuonPIDCut: public StepBase{
public:
  TPCAntiMuonPIDCut(){
    _likemip_min = (Float_t)ND::params().GetParameterD("antiNumuCCIncFwdAnalysis.Cuts.PID.LikeMIPMin");
    _likemu_min = (Float_t)ND::params().GetParameterD("antiNumuCCIncFwdAnalysis.Cuts.PID.LikeMuMin");
    _likemu_max = (Float_t)ND::params().GetParameterD("antiNumuCCIncFwdAnalysis.Cuts.PID.LikeMuMax");
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new TPCAntiMuonPIDCut();}
private:
  Float_t _likemip_min;
  Float_t _likemu_min;
  Float_t _likemu_max;
};

class EcalContainedPIDCut: public StepBase{
public:
  EcalContainedPIDCut(){
    _cont_mom_max = (Float_t)ND::params().GetParameterD("antiNumuCCIncFwdAnalysis.Cuts.PID.ECalContMomMax");
    _cont_mippion_max = (Float_t)ND::params().GetParameterD("antiNumuCCIncFwdAnalysis.Cuts.PID.ECalContMipPionMax");
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new EcalContainedPIDCut();}
protected:
  Float_t _cont_mom_max;
  Float_t _cont_mippion_max;
};

class EcalOtherPIDCut: public StepBase{
public:
  EcalOtherPIDCut(){
    _other_mippion_max = (Float_t)ND::params().GetParameterD("antiNumuCCIncFwdAnalysis.Cuts.PID.ECalOtherMipPionMax");
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new EcalOtherPIDCut();}
protected:
  Float_t _other_mippion_max;
};

class FGD2VetoCut: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new FGD2VetoCut();}
};

#endif
