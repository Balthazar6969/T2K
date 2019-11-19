#ifndef tutorialBranchesSelection_h
#define tutorialBranchesSelection_h

#include "SelectionBase.hxx"
#include "SubDetId.hxx"
#include "ToyBoxTracker.hxx"

/* In this file there is an example of selection with several branches. The CopySteps method is also used to copy steps from tutorialSelection 
   to this selection. Four new steps are added on top of the tutorialSelection, one for each of the three branches defined:
    1. Only one TPC track
    2. Two TPC tracks
    3. More than two TPC tracks
    3. More than two TPC tracks with more than 65 nodes in the closest TPC to the vertex

   Two define a selection you need in this file:
   - If a specific box is needed (the ToyBoxB is not sufficient) define it first such that it known when the Selection is defined
   - Declare the selection inheriting from SelectionBase or another existing selection
   - Declare all steps needed by the selection
*/

//---- Define an specific box for this selection -------
class ToyBoxTutorial: public ToyBoxTracker{
  
public:
  ToyBoxTutorial(){
    nLongTPCTracks = 0;
  }

  virtual void Reset(){
    nLongTPCTracks = 0;
  }
  
  virtual ~ToyBoxTutorial(){}
  
  // number of TPC tracks with more that 65 nodes
  int nLongTPCTracks;
};

//---- Define the class for the new selection, which should inherit from SelectionBase or from another existing selection -------
class tutorialBranchesSelection: public SelectionBase{
  
public:
  tutorialBranchesSelection(bool forceBreak=true);
  virtual ~tutorialBranchesSelection(){}

  ///========= These are mandatory functions ==================

  /// In this method all steps are added to the selection
  void DefineSteps();
  
  /// Set detector FV
  void DefineDetectorFV();

  /// Create a proper instance of the box (ToyBoxB) to store all relevent information to be passed from one step to the next
  ToyBoxB* MakeToyBox() {return new ToyBoxTutorial();}

  /// Fill the EventBox with the objects needed by this selection
  void InitializeEvent(AnaEventC& event);

  //==========================================================

  //---- These are optional functions, needed by FITTERS but not by highland2 analyses --------------

  /// Fill the event summary information, which is needed by the fitters (BANFF, Mach3)
  bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);

  /// Return the appropriate sample type (only needed by fitters)
  SampleId_h GetSampleId(){return SampleId::kFGD1NuMuCC0Pi;}

  /*  
  //---- These are optional functions, but recommended to customize systematics and increase speed --------------

  bool IsRelevantRecObjectForSystematic(const AnaEventC&, AnaTrackB*, Int_t syst_index, Int_t branch=0) const {(void)syst_index;(void)branch;return true;}
  bool IsRelevantTrueObjectForSystematic(const AnaEventC&, AnaTrueParticleB*, Int_t syst_index, Int_t branch=0) const {(void)syst_index;(void)branch;return true;}
  bool IsRelevantRecObjectForSystematicInToy(const AnaEventC&, const ToyBoxB&, AnaTrackB*, Int_t syst_index, Int_t branch=0) const {(void)syst_index;(void)branch;return true;}
  bool IsRelevantTrueObjectForSystematicInToy(const AnaEventC&, const ToyBoxB&, AnaTrueParticleB*, Int_t syst_index, Int_t branch=0) const {(void)syst_index;(void)branch;return true;}
  bool IsRelevantSystematic(const AnaEventC&, const ToyBoxB&, Int_t syst_index, Int_t branch=0) const {(void)syst_index;(void)branch;return true;}
  bool CheckRedoSelection(const AnaEventB&, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep){(void)PreviousToyBox;redoFromStep=0;return true;}

  */

};

//---- Define all steps -------
class FillSummaryAction_tutorialBranches: public StepBase{
    public:
        using StepBase::Apply;
        bool Apply(AnaEventC& event, ToyBoxB& box) const;  
        StepBase* MakeClone(){return new FillSummaryAction_tutorialBranches();}
};

class OneTPCTrackCut: public StepBase{  
    public:
        using StepBase::Apply;
        bool Apply(AnaEventC& event, ToyBoxB& box) const;
        StepBase* MakeClone(){return new OneTPCTrackCut();}
};

class TwoTPCTracksCut: public StepBase{  
    public:
        using StepBase::Apply;
        bool Apply(AnaEventC& event, ToyBoxB& box) const;
        StepBase* MakeClone(){return new TwoTPCTracksCut();}
};

class MoreThanTwoTPCTracksCut: public StepBase{  
    public:
        using StepBase::Apply;
        bool Apply(AnaEventC& event, ToyBoxB& box) const;
        StepBase* MakeClone(){return new MoreThanTwoTPCTracksCut();}
};

class MoreThanTwoLongTPCTracksCut: public StepBase{  
    public:
        using StepBase::Apply;
        bool Apply(AnaEventC& event, ToyBoxB& box) const;
        StepBase* MakeClone(){return new MoreThanTwoLongTPCTracksCut();}
};

class FindLongTPCTracks: public StepBase{
    public:
        using StepBase::Apply;
        bool Apply(AnaEventC& event, ToyBoxB& box) const;  
        StepBase* MakeClone(){return new FindLongTPCTracks();}
};


#endif
