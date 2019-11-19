#ifndef FGDPIDSystematics_h
#define FGDPIDSystematics_h

#include "EventVariationBase.hxx"
#include "BinnedParams.hxx"

/// This systematic smears the pull of each FGD track segment
class FGDPIDSystematics: public EventVariationBase {
public:

  /// Instantiate the FGD PID systematic. nbins is the number of
  /// bins in the PDF
  FGDPIDSystematics();
  
  virtual ~FGDPIDSystematics() {
    if (_fgd1mean)  delete _fgd1mean;   _fgd1mean   = NULL;
    if (_fgd1sigma) delete _fgd1sigma;  _fgd1sigma  = NULL;
    if (_fgd2mean)  delete _fgd2mean;   _fgd2mean   = NULL;
    if (_fgd2sigma) delete _fgd2sigma;  _fgd2sigma  = NULL;
  }
  
  /// Apply the systematic
  void Apply(const ToyExperiment& toy, AnaEventC& event);
  
  /// Undo  the systematic variations done by ApplyVariation. This is faster tha reseting the full Spill
  bool UndoSystematic(AnaEventC& event);
      
  void Initialize();

  
protected:

  /// Is this track relevant for this systematic ?
  bool IsRelevantRecObject(const AnaEventC& event, const AnaRecObjectC& track) const;

protected:
  BinnedParams* _fgd1mean;
  BinnedParams* _fgd1sigma;
  BinnedParams* _fgd2mean;
  BinnedParams* _fgd2sigma;

};

#endif
