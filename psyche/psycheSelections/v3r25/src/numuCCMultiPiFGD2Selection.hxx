#ifndef numuCCMultiPiFGD2Selection_h
#define numuCCMultiPiFGD2Selection_h

#include "SelectionBase.hxx"
#include "Parameters.hxx"
#include "numuCCMultiPiSelection.hxx"
#include "numuCCFGD2Selection.hxx"

class numuCCMultiPiFGD2Selection: public SelectionBase{

    public:
        numuCCMultiPiFGD2Selection(bool forceBreak = true);
        virtual ~numuCCMultiPiFGD2Selection(){}

        void DefineSteps();
        void DefineDetectorFV();
        ToyBoxB* MakeToyBox(){return new ToyBoxCCMultiPi();}
        bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
        SampleId_h GetSampleId(){return SampleId::kFGD2NuMuCC0Pi;}
        
        bool IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const;
        bool IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const;
        
        bool IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const;
        void InitializeEvent(AnaEventC& event);
        bool CheckRedoSelection(const AnaEventC& event, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep);

        bool IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& box, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const;
        bool IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& box, AnaTrueObjectC* trueTrack, SystId_h systId, Int_t branch) const;
        
        Int_t GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;
        Int_t GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;


    protected:
       numuCCMultiPiSelection _numuCCMultiPiSelection;
       numuCCFGD2Selection    _numuCCFGD2Selection;

};

class FillSummaryAction_numuCCMultiPiFGD2: public StepBase{
    public:
        using StepBase::Apply;
        bool Apply(AnaEventC& event, ToyBoxB& box) const;  
        StepBase* MakeClone(){return new FillSummaryAction_numuCCMultiPiFGD2();}
};

#endif
