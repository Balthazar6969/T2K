#ifndef SIProtonSystematics_h
#define SIProtonSystematics_h

#include "EventWeightBase.hxx"
#include "BinnedParams.hxx"
#include "ProtonInteractionSystematic.hxx"

class SIProtonSystematics: public EventWeightBase, public BinnedParams {
  public:

  SIProtonSystematics();
  
  virtual ~SIProtonSystematics();
  
  /// Apply the systematic
  Weight_h ComputeWeight(const ToyExperiment&    , const AnaEventC&      , const ToyBoxB&){return 1;}
  Weight_h ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box, const SelectionBase& sel);
  
  /// Create the array of ProtonInteractionSystematic
  void InitializeSystBoxes(Int_t nsel, Int_t isel, Int_t nbranch, Int_t nevents);
 
  /// Delete the ProtonInteractionSystematic for this event
  void FinalizeEvent(const AnaEventC& event);

  /// Initilaize the systematics itself, basically the manager
  virtual void Initialize();
  
protected:

  /// Fill the SystBox for this event, selection and branch
  void FillSystBox(const AnaEventC& event, const SelectionBase& sel, Int_t ibranch);
 
protected:
  
  ///Scale factor for cross-section
  Float_t _xsecScale;
  
  ///Uncertainty for cross-section
  Float_t _xsecError;
  
  Int_t _nevents;
  
  /// Manager to collect proton interactions
  si_syst::ParticleSIManager _pInfoManager;
  si_syst::SISystInput**     _ProtonInteractionsInfo;
   
  bool _initialized;
      
};

#endif
