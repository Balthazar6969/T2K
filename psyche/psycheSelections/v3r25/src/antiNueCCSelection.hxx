#ifndef antiNueCCSelection_h
#define antiNueCCSelection_h

#include "SelectionBase.hxx"
#include "Parameters.hxx"
#include "ToyBoxTracker.hxx"
#include "SystId.hxx"
#include "SubDetId.hxx"

#include "nueCCSelection.hxx"

class antiNueCCSelection: public SelectionBase{
 public:
  antiNueCCSelection(bool forceBreak=true);
  virtual ~antiNueCCSelection(){}

  //---- These are mandatory functions
  void DefineSteps();
  void DefineDetectorFV();
  ToyBoxB* MakeToyBox(){return new ToyBoxNueCC();}
  
  void InitializeEvent(AnaEventC& eventC);
  bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
  SampleId_h GetSampleId(){return SampleId::kFGD1AntiNuECC;}

  bool IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantRecObjectForSystematicInToy(const AnaEventC&, const ToyBoxB&, AnaRecObjectC*, SystId_h systId, Int_t branch) const;
  bool IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantTrueObjectForSystematicInToy(const AnaEventC&, const ToyBoxB&, AnaTrueObjectC*, SystId_h systId, Int_t branch) const;
  bool IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& boxB, SystId_h systId, Int_t branch) const;
  bool CheckRedoSelection(const AnaEventC& eventC, const ToyBoxB& PreviousToyBoxB, Int_t& redoFromStep);

  Int_t GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;
  Int_t GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;

protected:
  nueCCSelection _nueCCSelection;

  Int_t _FindLeadingTracksStepIndex;
  Int_t _TotalMultiplicityCutIndex;
  Int_t _ElecPIDCutIndex;
  Int_t _ElecPIDStepIndex;

  // Variables needed from datacard
  Float_t _protonregion_low;
};

class FillSummaryAction_antiNueCC: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;  
  StepBase* MakeClone(){return new FillSummaryAction_antiNueCC();}
};

class FillLeadingTracksAction_antiNueCC: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new FillLeadingTracksAction_antiNueCC();}
};

class FindSecondMostEnergeticTPCTrack_antiNueCC: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& eventC, ToyBoxB& boxB) const;  
  StepBase* MakeClone(){return new FindSecondMostEnergeticTPCTrack_antiNueCC();}
};

class MostEnergeticTrackCut_antiNueCC: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;  
  StepBase* MakeClone(){return new MostEnergeticTrackCut_antiNueCC();}
};

class SecondTPCPIDCut_antiNueCC: public StepBase{
public:
  SecondTPCPIDCut_antiNueCC(){
    _protonregion_low    = (Float_t)ND::params().GetParameterD("psycheSelections.antiNueCCAnalysis.Analysis.ProtonMomentumRegion.Low");
    _protonregion_high   = (Float_t)ND::params().GetParameterD("psycheSelections.antiNueCCAnalysis.Analysis.ProtonMomentumRegion.High");
    _pullelec_reject_min = (Float_t)ND::params().GetParameterD("psycheSelections.antiNueCCAnalysis.Cuts.PID2.PullElecMin");
    _pullelec_reject_max = (Float_t)ND::params().GetParameterD("psycheSelections.antiNueCCAnalysis.Cuts.PID2.PullElecMax");
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new SecondTPCPIDCut_antiNueCC();}
protected:
  Float_t _protonregion_low;
  Float_t _protonregion_high;
  Float_t _pullelec_reject_min;
  Float_t _pullelec_reject_max;
};

class EcalEMEnergyCut_antiNueCC: public StepBase{
public:
  EcalEMEnergyCut_antiNueCC(){
    _Ethreshold     = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.PID.EThreshold");
    _Emin_ecal      = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.PID.EcalEMin");
    _EoverP         = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.PID.EoverP");
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new EcalEMEnergyCut_antiNueCC();}
protected:
  Float_t _Ethreshold;
  Float_t _Emin_ecal;
  Float_t _EoverP;
};

class EcalEMHIPPIDCut_antiNueCC: public StepBase{
public:
  EcalEMHIPPIDCut_antiNueCC(){
    _emhip             = (Float_t)ND::params().GetParameterD("psycheSelections.antiNueCCAnalysis.Cuts.PID.EmHip");
    _emhipfhc          = (Float_t)ND::params().GetParameterD("psycheSelections.antiNueCCAnalysis.Cuts.PID.FHC.EmHip");
    _protonregion_low  = (Float_t)ND::params().GetParameterD("psycheSelections.antiNueCCAnalysis.Analysis.ProtonMomentumRegion.Low");
  }
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new EcalEMHIPPIDCut_antiNueCC();}
protected:
  Float_t _emhip;
  Float_t _emhipfhc;
  Float_t _protonregion_low;
};

class EoverPCut_antiNueCC: public StepBase{
public:
  EoverPCut_antiNueCC(){
    _protonregion_low  = (Float_t)ND::params().GetParameterD("psycheSelections.antiNueCCAnalysis.Analysis.ProtonMomentumRegion.Low");
    _eoverp            = (Float_t)ND::params().GetParameterD("psycheSelections.antiNueCCAnalysis.Cuts.PID.EoverP");
    _eoverpfhc         = (Float_t)ND::params().GetParameterD("psycheSelections.antiNueCCAnalysis.Cuts.PID.FHC.EoverP");
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new EoverPCut_antiNueCC();}
protected:
  Float_t _protonregion_low;
  Float_t _eoverp;
  Float_t _eoverpfhc;
 };

class FGDShowerCut_antiNueCC: public StepBase{
public:
  FGDShowerCut_antiNueCC(){
    _protonregion_low  = (Float_t)ND::params().GetParameterD("psycheSelections.antiNueCCAnalysis.Analysis.ProtonMomentumRegion.Low");
    _protonregion_high = (Float_t)ND::params().GetParameterD("psycheSelections.antiNueCCAnalysis.Analysis.ProtonMomentumRegion.High");
  }
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new FGDShowerCut_antiNueCC();}
protected:
  Float_t _protonregion_low;
  Float_t _protonregion_high;
};

class ToFCut_antiNueCC: public StepBase{
public:
  ToFCut_antiNueCC(){
    _protonregion_low    = (Float_t)ND::params().GetParameterD("psycheSelections.antiNueCCAnalysis.Analysis.ProtonMomentumRegion.Low");
    _fgdecaltof          = (Float_t)ND::params().GetParameterD("psycheSelections.antiNueCCAnalysis.Cuts.ToF.FGDEcal");
    _fgd1fgd2tof         = (Float_t)ND::params().GetParameterD("psycheSelections.antiNueCCAnalysis.Cuts.ToF.FGD1FGD2");
    _fgd1fgd2tof_relaxed = (Float_t)ND::params().GetParameterD("psycheSelections.antiNueCCAnalysis.Cuts.ToF.RelaxedFGD1FGD2");
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& eventC, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new ToFCut_antiNueCC();}
protected:
  Float_t _protonregion_low;
  Float_t _fgdecaltof;
  Float_t _fgd1fgd2tof;
  Float_t _fgd1fgd2tof_relaxed;
};

class FGD2ProtonPID_antiNueCC: public StepBase{
public:
  FGD2ProtonPID_antiNueCC(){
    _fgd2protonpull      = (Float_t)ND::params().GetParameterD("psycheSelections.antiNueCCAnalysis.Cuts.FGD2.ProtonPull");
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& eventC, ToyBoxB& boxB) const;  
  StepBase* MakeClone(){return new FGD2ProtonPID_antiNueCC();}
protected:
  Float_t _fgd2protonpull;
};


#endif
