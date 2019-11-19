#ifndef numuCC4piMultiPiSelection_h
#define numuCC4piMultiPiSelection_h

#include "SelectionBase.hxx"
#include "Parameters.hxx"
#include "ToyBoxTracker.hxx"
#include "EventBoxId.hxx"
#include "SystId.hxx"
#include "SubDetId.hxx"

#include "ToFSenseCorrector.hxx"

#include "numuCC4piSelection.hxx"

#include "CutUtils.hxx"


class numuCC4piMultiPiSelection: public SelectionBase{
public:
  numuCC4piMultiPiSelection(bool forceBreak=true);
  virtual ~numuCC4piMultiPiSelection(){}

  //---- These are mandatory functions
  void DefineSteps();
  void DefineDetectorFV();
  inline ToyBoxB* MakeToyBox();
  bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
  SampleId_h GetSampleId(){return SampleId::kFGD1NuMuCC;}
  bool IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantRecObjectForSystematicInToy(const AnaEventC&, const ToyBoxB&, AnaRecObjectC*, SystId_h systId, Int_t branch=0) const;
  bool IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantTrueObjectForSystematicInToy(const AnaEventC&, const ToyBoxB&, AnaTrueObjectC*, SystId_h systId, Int_t branch=0) const;
  bool IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const;
  void InitializeEvent(AnaEventC& event);
  bool CheckRedoSelection(const AnaEventC& event, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep);

private:
  ToFSenseCorrector _tofCorrector;

};

class ToyBoxCC4piMultiPi: public ToyBoxCC4pi{

public:

  ToyBoxCC4piMultiPi() {
    MainTrack = NULL;
    LowAngle.clear(); HighAngle.clear();
    FwdTracks.clear();      BwdTracks.clear();      HAFwdTracks.clear();      HABwdTracks.clear();
    FwdTracks_Veto.clear(); BwdTracks_Veto.clear(); HAFwdTracks_Veto.clear(); HABwdTracks_Veto.clear();
    FwdTracks_PID.clear();  BwdTracks_PID.clear();  HAFwdTracks_PID.clear();  HABwdTracks_PID.clear();
  }

  virtual void Reset(){
    ResetBase();
    MainTrack = NULL;
    LowAngle.clear(); HighAngle.clear();
    FwdTracks.clear();      BwdTracks.clear();      HAFwdTracks.clear();      HABwdTracks.clear();
    FwdTracks_Veto.clear(); BwdTracks_Veto.clear(); HAFwdTracks_Veto.clear(); HABwdTracks_Veto.clear();
    FwdTracks_PID.clear();  BwdTracks_PID.clear();  HAFwdTracks_PID.clear();  HABwdTracks_PID.clear(); 

    pionBox.Reset();
  }

  virtual ~ToyBoxCC4piMultiPi(){}


  multipi::MultiPiBox pionBox;
};

inline ToyBoxB* numuCC4piMultiPiSelection::MakeToyBox() {return new ToyBoxCC4piMultiPi();}

class FillSummaryAction_numuCC4piMultiPi: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FillSummaryAction_numuCC4piMultiPi();}
};

namespace numuCC4piMultiPi{
  
  const multipi::MultiPiBox& GetPionBox(const ToyBoxB& box);

  class NoPionCut: public cutUtils::numuCCTrkMultiPi::NoPionCutBase{  
  public:

    const multipi::MultiPiBox& GetPionBox(const ToyBoxB& box) const{
      return numuCC4piMultiPi::GetPionBox(box);  
    }
    StepBase* MakeClone(){return new NoPionCut();}
  };

  class OnePionCut: public cutUtils::numuCCTrkMultiPi::OnePionCutBase{  
  public:

    OnePionCut(bool posPionMode): cutUtils::numuCCTrkMultiPi::OnePionCutBase(posPionMode){}
    virtual ~OnePionCut(){}

    const multipi::MultiPiBox& GetPionBox(const ToyBoxB& box) const{
      return numuCC4piMultiPi::GetPionBox(box);  
    }

    StepBase* MakeClone(){return new OnePionCut(_posPionMode);}
  };

  class OthersCut: public cutUtils::numuCCTrkMultiPi::OthersCutBase{  
  public:
    const multipi::MultiPiBox& GetPionBox(const ToyBoxB& box) const{
      return numuCC4piMultiPi::GetPionBox(box);  
    }
    StepBase* MakeClone(){return new OthersCut();}
  };

  class EcalPi0VetoCut: public cutUtils::numuCCTrkMultiPi::ECalPi0VetoCutBase{  
  public:
    const multipi::MultiPiBox& GetPionBox(const ToyBoxB& box) const{
      return numuCC4piMultiPi::GetPionBox(box);  
    }

    StepBase* MakeClone(){return new EcalPi0VetoCut();}
  };


  class FindPionsAction: public StepBase{
  public:
    using StepBase::Apply;
    FindPionsAction(){
      pionSelParams.useTPCPions                 = (bool)ND::params().GetParameterI("psycheSelections.numuCC4piMultiPi.UseTPCPions");
      pionSelParams.useME                       = (bool)ND::params().GetParameterI("psycheSelections.numuCC4piMultiPi.UseME");
      pionSelParams.useFGDPions                 = (bool)ND::params().GetParameterI("psycheSelections.numuCC4piMultiPi.UseFGDPions");
      pionSelParams.useOldSecondaryPID          = (bool)ND::params().GetParameterI("psycheSelections.numuCC4piMultiPi.OldSecondaryPID");
      pionSelParams.useECalPiZeroInfo           = (bool)ND::params().GetParameterI("psycheSelections.numuCC4piMultiPi.UseECalPiZeroInfo");

      pionSelParams.ECalMostUpstreamLayerHitCut = (Int_t)ND::params().GetParameterI("psycheSelections.numuCC4piMultiPi.MostUpstreamLayerHitCut");
      // Default
      pionSelParams.ECalEMEnergyCut = 30.;
      pionSelParams.ECalPIDMipEmCut = 0.;
    } 
    bool Apply(AnaEventC& event, ToyBoxB& box) const;  
    StepBase* MakeClone(){return new FindPionsAction();}

  protected:
    mutable multipi::PionSelectionParams pionSelParams;
  };


};

#endif
