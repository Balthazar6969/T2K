#ifndef numuBkgInAntiNuModeCCSelection_h
#define numuBkgInAntiNuModeCCSelection_h

#include "numuCCSelection.hxx"

class numuBkgInAntiNuModeCCSelection: public SelectionBase{
    public:
        numuBkgInAntiNuModeCCSelection(bool forceBreak=true);
        virtual ~numuBkgInAntiNuModeCCSelection(){}

        //---- These are mandatory functions
        void DefineSteps();
        void DefineDetectorFV();
        ToyBoxB* MakeToyBox(){return new ToyBoxTracker();}
        bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
        SampleId_h GetSampleId(){return SampleId::kFGD1NuMuBkgInAntiNuModeCC;}
        bool IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const;
        bool IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const;
        bool IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const;
        void InitializeEvent(AnaEventC& event);
        bool CheckRedoSelection(const AnaEventC& event, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep);
        
        Int_t GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;
        Int_t GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;
        
        // Fine tuning on toy level
        bool IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& box, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const;
        bool IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& box, AnaTrueObjectC* trueTrack, SystId_h systId, Int_t branch) const;
  

protected:
  
  numuCCSelection _numuCCSelection;

  Int_t _MuonPIDCutIndex;
  Int_t _MuonPIDStepIndex;
  Int_t _FindLeadingTracksStepIndex;
  Int_t _TotalMultiplicityCutIndex;
};

class FillSummaryAction_numuBkg: public StepBase{
    public:
        using StepBase::Apply;
        bool Apply(AnaEventC& event, ToyBoxB& box) const;  
        StepBase* MakeClone(){return new FillSummaryAction_numuBkg();}
};


class NegativeMultiplicityCut: public StepBase{
    public:
        using StepBase::Apply;
        bool Apply(AnaEventC& event, ToyBoxB& box) const;
        StepBase* MakeClone(){return new NegativeMultiplicityCut();}
};

class NumuBkgMuonPIDCut: public StepBase{
    public:
        using StepBase::Apply;
        bool Apply(AnaEventC& event, ToyBoxB& box) const;
        StepBase* MakeClone(){return new NumuBkgMuonPIDCut();}
};

#endif
