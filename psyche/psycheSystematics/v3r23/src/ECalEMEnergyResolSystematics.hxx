#ifndef ECalEMEnergyResolSystematics_h
#define ECalEMEnergyResolSystematics_h

#include "ECalEMEnergySystematicsBase.hxx"

class TRandom;

class SystBoxECalEMResol : public SystBoxB{
public: 
  SystBoxECalEMResol();
  virtual ~SystBoxECalEMResol();
  AnaDetCrossingB** RelevantCrossings;
  
};

/// This systematic smears the ECal EM energy by a
/// random amount from a Gaussian distribution.

class ECalEMEnergyResolSystematics: public ECalEMEnergySystematicsBase {
public:
  
  /// Instantiate the momentum resolution systematic. nbins is the number of
  /// bins in the PDF. addResol and addResolError describe
  /// the Gaussian distribution from which the resolution of each virtual
  /// analysis is selected from.
  ECalEMEnergyResolSystematics():ECalEMEnergySystematicsBase(1, "ECalEMEnergyResol", k1D_SYMMETRIC_NOMEAN){
    _RandomGenerator = NULL;
    InitializeRandomGenerator();
  }

  virtual ~ECalEMEnergyResolSystematics();
  
  /// Initialize the SystBox for this event
  void InitializeEvent(const AnaEventC& event, const SelectionBase& sel);
  void InitializeEvent(const AnaEventC& event);
  
  /// Apply the systematic
  virtual void Apply(const ToyExperiment& toy, AnaEventC& event);
  
  /// The only thing we allow for the generator is to get the seed
  UInt_t GetRandomSeed() const; 
  
  /// And change the seed
  void SetRandomSeed(UInt_t seed);
  
  /// Initialze random generator
  void InitializeRandomGenerator();
  
  
protected:
  
  /// A random generator that can be used to generate throws.
  TRandom* _RandomGenerator;
  

};

#endif
