#ifndef tutorialSelection_h
#define tutorialSelection_h

#include "SelectionBase.hxx"
#include "Parameters.hxx"
#include "ToyBoxTracker.hxx"

/* In this file there is an example of selection with a single branch. This is identical to the numuCCSelection (under psycheSelections) 
   used by the numuCCAnalysis in highland2.
   Two define a selection you need in this file:
   - If a specific box is needed (the ToyBoxB is not sufficient) define it first such that it is known when the Selection is defined
   - Declare the selection inheriting from SelectionBase or another existing selection
   - Declare all steps needed by the selection
*/

/*  CHANGES IN NEW VERSION

*/


//---- Define the class for the new selection, which should inherit from SelectionBase or from another existing selection -------
class tutorialSelection: public SelectionBase{
 public:
  tutorialSelection(bool forceBreak=true);
  virtual ~tutorialSelection(){}


  ///========= These are mandatory functions ==================

  /// In this method all steps are added to the selection
  void DefineSteps();
  
  /// Set detector FV
  void DefineDetectorFV();

  /// Create a proper instance of the box (ToyBoxB) to store all relevant 
  /// information to be passed from one step to the next
  ToyBoxB* MakeToyBox() {return new ToyBoxTracker();}

  /// Fill the EventBox with the objects needed by this selection
  void InitializeEvent(AnaEventC& event);

  //==========================================================


  //---- These are optional functions, needed by FITTERS but not by highland2 analyses --------------

  bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
  SampleId_h GetSampleId(){return SampleId::kFGD1NuMuCC;}

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

///---- Define all steps -------
class FillSummaryAction_tutorial: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;  
  StepBase* MakeClone(){return new FillSummaryAction_tutorial();}
};

class TrackQualityFiducialCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new TrackQualityFiducialCut();}
};

class TotalMultiplicityCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new TotalMultiplicityCut();}
};

class MuonPIDCut: public StepBase{
 public:
  MuonPIDCut(){
    _prod5Cut = false;
  }
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new MuonPIDCut();}
 private:
  bool _prod5Cut;
};

class ExternalVetoCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new ExternalVetoCut();}
};

class DeltaZCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new DeltaZCut();}
};

class ExternalFGD1lastlayersCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new ExternalFGD1lastlayersCut();}
};

/// Leading tracks with good quality in FGD1
class FindLeadingTracksAction: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FindLeadingTracksAction();}
};

/// Find the Vertex. For the moment it's just the Star position of the HM track
class FindVertexAction: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FindVertexAction();}
};


class FindVetoTrackAction: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FindVetoTrackAction();}
};

class FindOOFVTrackAction: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FindOOFVTrackAction();}
};


#endif
