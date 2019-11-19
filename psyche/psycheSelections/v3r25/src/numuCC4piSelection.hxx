#ifndef numuCC4piSelection_h
#define numuCC4piSelection_h

#include "SelectionBase.hxx"
#include "Parameters.hxx"
#include "ToyBoxTracker.hxx"
#include "EventBoxId.hxx"
#include "SystId.hxx"
#include "SubDetId.hxx"

#include "ToFSenseCorrector.hxx"

class numuCC4piSelection: public SelectionBase{
public:
    numuCC4piSelection(bool forceBreak=true);
    virtual ~numuCC4piSelection(){}
    
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
  
    Int_t GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;
    Int_t GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;
    
    /// Override the base method to set back FGD FV
    void FinalizeEvent(const AnaEventC&){}  
    
private:
    ToFSenseCorrector _tofCorrector;
    
};

class ToyBoxCC4pi: public ToyBoxTracker{
    
public:
    
    ToyBoxCC4pi() {
        MainTrack = NULL;
        LowAngle.clear(); HighAngle.clear();
        FwdTracks.clear();      BwdTracks.clear();      HAFwdTracks.clear();      HABwdTracks.clear();
        FwdTracks_Veto.clear(); BwdTracks_Veto.clear(); HAFwdTracks_Veto.clear(); HABwdTracks_Veto.clear();
        FwdTracks_PID.clear();  BwdTracks_PID.clear();  HAFwdTracks_PID.clear();  HABwdTracks_PID.clear(); CSFD2Tracks_PID.clear(); CSECALTracks_PID.clear();
    }
    
    virtual void Reset(){
        ResetBase();
        MainTrack = NULL;
        LowAngle.clear(); HighAngle.clear();
        FwdTracks.clear();      BwdTracks.clear();      HAFwdTracks.clear();      HABwdTracks.clear();
        FwdTracks_Veto.clear(); BwdTracks_Veto.clear(); HAFwdTracks_Veto.clear(); HABwdTracks_Veto.clear();
        FwdTracks_PID.clear();  BwdTracks_PID.clear();  HAFwdTracks_PID.clear();  HABwdTracks_PID.clear(); CSFD2Tracks_PID.clear(); CSECALTracks_PID.clear();
    }
    
    virtual ~ToyBoxCC4pi(){}
    
    /// For storing tracks information in the bunch.
    AnaTrackB* MainTrack;
    std::vector<AnaTrackB*> LowAngle, HighAngle;
    std::vector<AnaTrackB*> FwdTracks,      BwdTracks,      HAFwdTracks,      HABwdTracks;
    std::vector<AnaTrackB*> FwdTracks_Veto, BwdTracks_Veto, HAFwdTracks_Veto, HABwdTracks_Veto;
    std::vector<AnaTrackB*> FwdTracks_PID,  BwdTracks_PID,  HAFwdTracks_PID,  HABwdTracks_PID, CSFD2Tracks_PID, CSECALTracks_PID;
    
};

inline ToyBoxB* numuCC4piSelection::MakeToyBox() {return new ToyBoxCC4pi();}

class FillSummaryAction_numuCC4pi: public StepBase{
public:
    using StepBase::Apply;
    bool Apply(AnaEventC& event, ToyBoxB& box) const;
    StepBase* MakeClone(){return new FillSummaryAction_numuCC4pi();}
};

namespace numuCC4pi{
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

	class FindVertexAction: public StepBase{
	public:
		using StepBase::Apply;
 	   bool Apply(AnaEventC& event, ToyBoxB& box) const;
 	   StepBase* MakeClone(){return new FindVertexAction();}
	};

	class Fwd_Quality: public StepBase{
	public:
	    using StepBase::Apply;
	    bool Apply(AnaEventC& event, ToyBoxB& box) const;
	    StepBase* MakeClone(){return new Fwd_Quality();}
	};
	class Fwd_Veto: public StepBase{
	public:
	    using StepBase::Apply;
	    bool Apply(AnaEventC& event, ToyBoxB& box) const;
	    StepBase* MakeClone(){return new Fwd_Veto();}
	};
	class Fwd_PID: public StepBase{
	public:
	    using StepBase::Apply;
	    bool Apply(AnaEventC& event, ToyBoxB& box) const;
	    StepBase* MakeClone(){return new Fwd_PID();}
	};
	class Fwd: public StepBase{
	public:
	    using StepBase::Apply;
	    bool Apply(AnaEventC& event, ToyBoxB& box) const;
	    StepBase* MakeClone(){return new Fwd();}
	};

	class Bwd_Quality: public StepBase{
	public:
	    using StepBase::Apply;
	    bool Apply(AnaEventC& event, ToyBoxB& box) const;
	    StepBase* MakeClone(){return new Bwd_Quality();}
	};
	class Bwd_Veto: public StepBase{
	public:
	    using StepBase::Apply;
	    bool Apply(AnaEventC& event, ToyBoxB& box) const;
	    StepBase* MakeClone(){return new Bwd_Veto();}
	};
	class Bwd_PID: public StepBase{
	public:
	    using StepBase::Apply;
	    bool Apply(AnaEventC& event, ToyBoxB& box) const;
	    StepBase* MakeClone(){return new Bwd_PID();}
	};
	class Bwd: public StepBase{
	public:
	    using StepBase::Apply;
	    bool Apply(AnaEventC& event, ToyBoxB& box) const;
	    StepBase* MakeClone(){return new Bwd();}
	};

	class HAFwd_Quality: public StepBase{
	public:
	    using StepBase::Apply;
	    bool Apply(AnaEventC& event, ToyBoxB& box) const;
	    StepBase* MakeClone(){return new HAFwd_Quality();}
	};
	class HAFwd_Veto: public StepBase{
	public:
	    using StepBase::Apply;
	    bool Apply(AnaEventC& event, ToyBoxB& box) const;
	    StepBase* MakeClone(){return new HAFwd_Veto();}
	};
	class HAFwd_PID: public StepBase{
	public:
	    using StepBase::Apply;
	    bool Apply(AnaEventC& event, ToyBoxB& box) const;
	    StepBase* MakeClone(){return new HAFwd_PID();}
	};
	class HAFwd: public StepBase{
	public:
	    using StepBase::Apply;
	    bool Apply(AnaEventC& event, ToyBoxB& box) const;
	    StepBase* MakeClone(){return new HAFwd();}
	};

	class HABwd_Quality: public StepBase{
	public:
	    using StepBase::Apply;
	    bool Apply(AnaEventC& event, ToyBoxB& box) const;
	    StepBase* MakeClone(){return new HABwd_Quality();}
	};
	class HABwd_Veto: public StepBase{
	public:
	    using StepBase::Apply;
	    bool Apply(AnaEventC& event, ToyBoxB& box) const;
	    StepBase* MakeClone(){return new HABwd_Veto();}
	};
	class HABwd_PID: public StepBase{
	public:
	    using StepBase::Apply;
	    bool Apply(AnaEventC& event, ToyBoxB& box) const;
	    StepBase* MakeClone(){return new HABwd_PID();}
	};
	class HABwd: public StepBase{
	public:
	    using StepBase::Apply;
	    bool Apply(AnaEventC& event, ToyBoxB& box) const;
	    StepBase* MakeClone(){return new HABwd();}
	};
}
namespace numuCC4pi_utils{
    
    const Float_t LAFVmin[3] = {57.66, 57.66, 10.125};
    const Float_t LAFVmax[3] = {57.66, 57.66, 0.};
    const Float_t HAFVmin[3] = {57.66, 57.66, 10.125};
    const Float_t HAFVmax[3] = {57.66, 57.66, 10.125};
    
    const Float_t _FVdefminDsECal[3] = {40, 40, 25.};   // "z == 25." to reject peak near the DsECal beginning in both data and MC
    const Float_t _FVdefmaxDsECal[3] = {40, 40, 18.17}; // reject last layer in both data and MC
    const Float_t _FVdefminFGD2[3]   = {76.88, 38.44, 0.};     //9.61*8, 9.61*4
    const Float_t _FVdefmaxFGD2[3]   = {76.88, 38.44, 10.125}; //9.61*8, 9.61*4

    int GetFgdLayer(Float_t* pos);
    
    bool VetoCut(int topo, const AnaEventB& event, AnaTrackB& candidate);
    bool ExternalCut(int topo, const AnaEventC& event, AnaTrackB& candidate);
    int PIDCut(int topo, const AnaTrackB& candidate);
    
}


#endif
