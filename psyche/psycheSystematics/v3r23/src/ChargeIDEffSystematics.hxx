#ifndef ChargeIDEffSystematics_h
#define ChargeIDEffSystematics_h

#include "EventWeightBase.hxx"
#include "BinnedParams.hxx"

/// Charge confusion systematic. This is treated as an efficiency systematic, applying a weight to the event

class ChargeIDEffSystematics: public EventWeightBase {
public:
  
  ChargeIDEffSystematics(bool computecount=false);  
  virtual ~ChargeIDEffSystematics() {
    if (_globalCharge)  delete _globalCharge; _globalCharge = NULL;
    if (_localCharge)   delete _localCharge;  _localCharge  = NULL;
  }
  
  /// Apply the systematic
  Weight_h ComputeWeight(const ToyExperiment&, const AnaEventC&, const ToyBoxB&){
    return 1.;
  }
  
  /// Apply the systematic
  Weight_h ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box, const SelectionBase& sel);

protected:
  
  /// Different cases of charge (non)-equality to be addressed by the systematic
  enum SystCase{
    kUnknown = -1, 
    kSystCase0, 
    kSystCase1, 
    kSystCase2, 
    kSystCase3, 
    kSystCase4
  };
  

  /// Fill the SystBox for this event, selection and branch
  void FillSystBox(const AnaEventC& event, const SelectionBase& sel, Int_t ibranch);

  void ComputeEffFromGlobalParametrization(BinnedParamsParams *par,Float_t mom, Float_t momerr, BinnedParamsParams &params );
  void ComputeEffFromGlobalLocalParametrization(BinnedParamsParams *par,Float_t mom, Float_t momerr, BinnedParamsParams &params );
  void ComputeEffFromLocalParametrization(BinnedParamsParams *par,Float_t mom, Float_t momerr, BinnedParamsParams &params );
  Float_t ComputeEffFromGlobalLocalParametrization(BinnedParamsParams *par,Float_t mom, Float_t momerr );
  Float_t ComputeEffFromLocalParametrization(BinnedParamsParams *par,Float_t mom, Float_t momerr);

  /// Is this track relevant for this systematic ?
  //  bool IsRelevantRecObject(const AnaEventC& event, const AnaRecObjectC& track) const;
protected:

  bool  _computecounters;

  /// value of psycheSystematics.Tracker.FullCorrelations parameter
  bool _full_correlations;

public:
  //TODO

  BinnedParams* _globalCharge;
  BinnedParams* _localCharge;
};

#endif
