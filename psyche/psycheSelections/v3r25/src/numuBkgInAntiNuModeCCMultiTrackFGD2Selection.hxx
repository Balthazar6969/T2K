#ifndef numuBkgInAntiNuModeCCMultiTrackFGD2Selection_h
#define numuBkgInAntiNuModeCCMultiTrackFGD2Selection_h

#include "SelectionBase.hxx"
#include "Parameters.hxx"
#include "numuCCMultiPiFGD2Selection.hxx"
#include "numuCCFGD2Selection.hxx"
#include "numuBkgInAntiNuModeCCFGD2Selection.hxx"
#include "numuBkgInAntiNuModeCCMultiTrackSelection.hxx"

class numuBkgInAntiNuModeCCMultiTrackFGD2Selection: public SelectionBase{

    public:
        numuBkgInAntiNuModeCCMultiTrackFGD2Selection(bool forceBreak=true);
        virtual ~numuBkgInAntiNuModeCCMultiTrackFGD2Selection(){}

        void DefineSteps();
        void DefineDetectorFV();
        ToyBoxB* MakeToyBox(){return new ToyBoxCCMultiPi();}
        bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
        SampleId_h GetSampleId(){return SampleId::kFGD2NuMuBkgInAntiNuModeCC;}
        bool IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const;
        bool IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const;
        bool IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const;
        void InitializeEvent(AnaEventC& event);
        bool CheckRedoSelection(const AnaEventC& event, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep);
        
        Int_t GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;
        Int_t GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;

    protected:
       numuCCMultiPiFGD2Selection _numuCCMultiPiFGD2Selection;
       numuBkgInAntiNuModeCCFGD2Selection _numuBkgInAntiNuModeCCFGD2Selection;
       numuBkgInAntiNuModeCCMultiTrackSelection _numuBkgInAntiNuModeCCMultiTrackSelection;
};

class FillSummaryAction_numuBkgMultiTrackFGD2: public StepBase{
    public:
        using StepBase::Apply;
        bool Apply(AnaEventC& event, ToyBoxB& box) const;  
        StepBase* MakeClone(){return new FillSummaryAction_numuBkgMultiTrackFGD2();
	}
  
};

#endif
