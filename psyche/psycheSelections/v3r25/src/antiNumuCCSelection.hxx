#ifndef antiNumuCCSelection_h
#define antiNumuCCSelection_h

#include "SelectionBase.hxx"
#include "numuCCSelection.hxx"

class antiNumuCCSelection: public SelectionBase{
    public:
        antiNumuCCSelection(bool forceBreak=true);
        virtual ~antiNumuCCSelection(){}

        //---- These are mandatory functions
        void DefineSteps();
        void DefineDetectorFV();
        ToyBoxB* MakeToyBox(){return new ToyBoxTracker();}
        bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
        SampleId_h GetSampleId(){return SampleId::kFGD1AntiNuMuCC;}
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
       numuCCSelection _numuCCSelection;

       Int_t _MuonPIDCutIndex;
       Int_t _MuonPIDStepIndex;
       Int_t _FindLeadingTracksStepIndex;
       Int_t _TotalMultiplicityCutIndex;
};

class FillSummaryAction_antinu: public StepBase{
    public:
        using StepBase::Apply;
        bool Apply(AnaEventC& event, ToyBoxB& box) const;  
        StepBase* MakeClone(){return new FillSummaryAction_antinu();}
};

class PositiveMultiplicityCut: public StepBase{
    public:
        using StepBase::Apply;
        bool Apply(AnaEventC& event, ToyBoxB& box) const;
        StepBase* MakeClone(){return new PositiveMultiplicityCut();}
};

class AntiMuonPIDCut: public StepBase{
    public:
        using StepBase::Apply;
        bool Apply(AnaEventC& event, ToyBoxB& box) const;
        StepBase* MakeClone(){return new AntiMuonPIDCut();}
};

/// Leading tracks with good quality in FGD1
class FindLeadingTracksAction_antinu: public StepBase{
    public:
        using StepBase::Apply;
        bool Apply(AnaEventC& event, ToyBoxB& box) const;
        StepBase* MakeClone(){return new FindLeadingTracksAction_antinu();}
};

#endif
