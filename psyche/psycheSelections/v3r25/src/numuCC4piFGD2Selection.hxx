#ifndef numuCC4piFGD2Selection_h
#define numuCC4piFGD2Selection_h

#include "SelectionBase.hxx"
#include "Parameters.hxx"
#include "ToyBoxTracker.hxx"
#include "EventBoxId.hxx"
#include "SystId.hxx"
#include "SubDetId.hxx"
#include "numuCC4piSelection.hxx"

#include "ToFSenseCorrector.hxx"

class numuCC4piFGD2Selection: public SelectionBase{
public:
    numuCC4piFGD2Selection(bool forceBreak=true);
    virtual ~numuCC4piFGD2Selection(){}
    
    //---- These are mandatory functions
    void DefineSteps();
    void DefineDetectorFV();
    ToyBoxB* MakeToyBox(){return new ToyBoxCC4pi();}
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

class FillSummaryAction_numuCC4piFGD2: public StepBase{
public:
    using StepBase::Apply;
    bool Apply(AnaEventC& event, ToyBoxB& box) const;
    StepBase* MakeClone(){return new FillSummaryAction_numuCC4piFGD2();}
};

namespace numuCC4piFGD2{

	class TotalMultiplicityCut: public StepBase{
	public:
	    using StepBase::Apply;
	    bool Apply(AnaEventC& event, ToyBoxB& box) const;
	    StepBase* MakeClone(){return new TotalMultiplicityCut();}
	};

	class SortTracksAction: public StepBase{
	public:
	    using StepBase::Apply;
	    bool Apply(AnaEventC& event, ToyBoxB& box) const;
	    StepBase* MakeClone(){return new SortTracksAction();}
	};

	class TrackGQandFVCut: public StepBase{
	public:
	    using StepBase::Apply;
	    bool Apply(AnaEventC& event, ToyBoxB& box) const;
	    StepBase* MakeClone(){return new TrackGQandFVCut();}
	};

	class VetoAction: public StepBase{
	public:
	    using StepBase::Apply;
	    bool Apply(AnaEventC& event, ToyBoxB& box) const;
	    StepBase* MakeClone(){return new VetoAction();}
	};

	class PIDAction: public StepBase{
	public:
	    using StepBase::Apply;
	    bool Apply(AnaEventC& event, ToyBoxB& box) const;
	    StepBase* MakeClone(){return new PIDAction();}
	};
}

namespace numuCC4piFGD2_utils{
    const Float_t LAFVmin[3] = {57.66, 57.66, 10.125};
    const Float_t LAFVmax[3] = {57.66, 57.66, 0.};
    const Float_t HAFVmin[3] = {57.66, 57.66, 10.125};
    const Float_t HAFVmax[3] = {57.66, 57.66, 10.125};
    
    bool ExternalCut(int topo, const AnaEventC& event, AnaTrackB& candidate);
    int PIDCut(int topo, const AnaTrackB& candidate);
    
}

#endif
