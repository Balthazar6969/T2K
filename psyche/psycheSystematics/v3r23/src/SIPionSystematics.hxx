#ifndef SIPionSystematics_h
#define SIPionSystematics_h

#include "EventWeightBase.hxx"
#include "BinnedParams.hxx"
#include "PionInteractionSystematic.hxx"

const unsigned int NINTERACTIONS=200;
const unsigned int NMAXMOMBINS=201;

class SIPionSystematics: public EventWeightBase, public BinnedParams {
  public:

  SIPionSystematics();
  
  virtual ~SIPionSystematics(); 
  
  /// Apply the systematic
  Weight_h ComputeWeight(const ToyExperiment&    , const AnaEventC&      , const ToyBoxB&){return 1;}
  Weight_h ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box, const SelectionBase& sel);

  /// Create the array of PionInteractionSystematic
  void InitializeSystBoxes(Int_t nsel, Int_t isel, Int_t nbranch, Int_t nevents);
 
  /// Delete the PionInteractionSystematic for this event
  void FinalizeEvent(const AnaEventC& event);
  
  
  /// Initilaize the systematics itself, basically the manager
  virtual void Initialize();

protected:

  /// Fill the SystBox for this event, selection and branch
  void FillSystBox(const AnaEventC& event, const SelectionBase& sel, Int_t ibranch);
 
  Float_t _xsec_data[NINTERACTIONS][NMAXMOMBINS];
  Float_t _err_data[NINTERACTIONS][NMAXMOMBINS];

  /// Pion SI systematic associated quantities. TODO: better way of handeling this
  PionInteractionSystematic** _pionWeightInfo;
  
  
  /// A manager that grabs the needed data for systematics calculation
  PionSIManager* _pionSIManager;
  
  
  Int_t _interactionID[3];
  
  bool _initialized;
      
};

#endif
