#ifndef TPCECalMatchEffSystematics_h
#define TPCECalMatchEffSystematics_h

#include "EventWeightBase.hxx"
#include "BinnedParams.hxx"

class SystBoxTPCECalMatchEff: public SystBoxB{
public: 
  SystBoxTPCECalMatchEff();
  virtual ~SystBoxTPCECalMatchEff();
  
  //an array of ecal sub-detector crossed for each revelant true track
  SubDetId::SubDetEnum* RelevantTrueECalDetCrossed;
  
};

class TPCECalMatchEffSystematics: public EventWeightBase, public BinnedParams {
public:

  TPCECalMatchEffSystematics(bool computecounters=false);  
  
  virtual ~TPCECalMatchEffSystematics() {}

  /// Apply the systematic
  Weight_h ComputeWeight(const ToyExperiment&, const AnaEventC&, const ToyBoxB&){return 1;}
  
  Weight_h ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box, const SelectionBase& sel);

  /// Initialize the SystBox for this event
  virtual void InitializeEvent(const AnaEventC& event, const SelectionBase& sel, Int_t ibranch);
  
protected:
  
  // Fills the SystBox
  void FillSystBox(const AnaEventC& event, const SelectionBase& sel, Int_t ibranch);

  /// Is this track relevant for this systematic ?
  bool IsRelevantRecObject(const AnaEventC& event, const AnaRecObjectC& track) const;

  /// Is this track relevant for this systematic ?
  bool IsRelevantTrueObject(const AnaEventC& event, const AnaTrueObjectC& track) const;
   
  bool  _computecounters;
};

#endif
