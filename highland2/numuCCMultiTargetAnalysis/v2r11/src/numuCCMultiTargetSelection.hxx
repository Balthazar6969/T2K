#ifndef numuCCMultiTargetSelection_h
#define numuCCMultiTargetSelection_h

#include "baseSelection.hxx" // needed for EventQualityCut
#include "numuCCSelection.hxx"
#include "numuCCFGD2Selection.hxx"
#include "numuCCMultiPiSelection.hxx"
#include "numuCCMultiPiFGD2Selection.hxx"


class numuCCMultiTargetSelection: public SelectionBase{

    public:
        numuCCMultiTargetSelection(bool doMultiPi=false, bool forceBreak=true);
        virtual ~numuCCMultiTargetSelection(){}

        ToyBoxB* MakeToyBox(){return new ToyBoxCCMultiPi();}
        void InitializeEvent(AnaEventC& event);
        void DefineSteps();
        void DefineDetectorFV();
};

/*
class ToyBoxCCMultiTarget: public ToyBoxB{

    public:
        ToyBoxCCMultiTarget(){
        }

        virtual ~ToyBoxCCMultiTarget(){}
        void Reset(){
            ToyBoxB::Reset();
        }

};

inline ToyBoxB* numuCCMultiTargetSelection::MakeToyBox(){return new ToyBoxCCMultiTarget();}
*/

namespace numuCCMultiTargetUtils{
        //From NumuCCBANFF Standard Actions.
}

class TotalMultiplicityFGD2Cut: public StepBase{
    public:
        using StepBase::Apply;
        bool Apply(AnaEventC& event, ToyBoxB& box) const;
        StepBase* MakeClone(){return new TotalMultiplicityFGD2Cut();}
};


class WaterCut: public StepBase{
    public:
        using StepBase::Apply;
        bool Apply(AnaEventC& event, ToyBoxB& box) const;
        StepBase* MakeClone(){return new WaterCut();}
};


class ScintillatorCut: public StepBase{
    public:
        using StepBase::Apply;
        bool Apply(AnaEventC& event, ToyBoxB& box) const;
        StepBase* MakeClone(){return new ScintillatorCut();}
};


class SetDetectorFVAction: public StepBase{
    public:
        SetDetectorFVAction(SubDetId::SubDetEnum det){_detectorFV = det;}
        using StepBase::Apply;
        bool Apply(AnaEventC& event, ToyBoxB& box) const;
        StepBase* MakeClone(){return new SetDetectorFVAction(_detectorFV);}

    protected:
        SubDetId::SubDetEnum _detectorFV;
};


#endif
