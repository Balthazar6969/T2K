#ifndef MichelElectronEffSystematics_h
#define MichelElectronEffSystematics_h

#include "EventWeightBase.hxx"
#include "BinnedParams.hxx"

/// Michel electron effciency systematic

class MichelElectronEffSystematics: public EventWeightBase {
public:
  
  /// Instantiate the michel electron efficiency systematic. nana is the number of
  /// virtual analyses that will be run.
  /// There is only 1 analysis (+ the unchanged) for this systematic, as we only store the weight for a change of one sigma.
  /// The mean and sigma are defined in a data file.
  MichelElectronEffSystematics(bool computecounters=false);

  virtual ~MichelElectronEffSystematics() {
    if (_fgd1eff) delete _fgd1eff; _fgd1eff = NULL;
    if (_fgd1pur) delete _fgd1pur; _fgd1pur = NULL;
    if (_fgd2eff) delete _fgd2eff; _fgd2eff = NULL; 
    if (_fgd2pur) delete _fgd2pur; _fgd2pur = NULL; 
  }
  
  /// Apply the weight
  using EventWeightBase::ComputeWeight;
  Weight_h ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box);

  void Initialize();
  
  BinnedParams* _fgd1eff;
  BinnedParams* _fgd1pur;
  BinnedParams* _fgd2eff;
  BinnedParams* _fgd2pur;

protected:
  bool  _computecounters;
  bool  _prod5Cut;

};

#endif
