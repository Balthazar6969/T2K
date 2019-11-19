#ifndef numuCC4piMultiPiFGD2Selection_h
#define numuCC4piMultiPiFGD2Selection_h

#include "SelectionBase.hxx"
#include "Parameters.hxx"
#include "ToyBoxTracker.hxx"
#include "EventBoxId.hxx"
#include "SystId.hxx"
#include "SubDetId.hxx"
#include "numuCC4piFGD2Selection.hxx"
#include "numuCC4piMultiPiSelection.hxx"

#include "ToFSenseCorrector.hxx"

class numuCC4piMultiPiFGD2Selection: public SelectionBase{
public:
    numuCC4piMultiPiFGD2Selection(bool forceBreak=true);
    virtual ~numuCC4piMultiPiFGD2Selection(){}
    
    //---- These are mandatory functions
    void DefineSteps();
    void DefineDetectorFV();
    ToyBoxB* MakeToyBox(){return new ToyBoxCC4piMultiPi();}
    bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
    SampleId_h GetSampleId(){return SampleId::kFGD2NuMuCC;}
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

class FillSummaryAction_numuCC4piMultiPiFGD2: public StepBase{
public:
    using StepBase::Apply;
    bool Apply(AnaEventC& event, ToyBoxB& box) const;
    StepBase* MakeClone(){return new FillSummaryAction_numuCC4piMultiPiFGD2();}
};

#endif
