#ifndef NuParentDecayPosSystematics_h
#define NuParentDecayPosSystematics_h

#include "EventVariationBase.hxx"

class TH3;

/// What its own generator so to be able to set the seed to reproduce the results
class TRandom;


/// This systematic samples the position of the neutrino parent decay point
/// from a given distribution
class NuParentDecayPosSystematics: public EventVariationBase{

public:
 
  NuParentDecayPosSystematics();

  virtual ~NuParentDecayPosSystematics();
  
  /// Apply the systematic
  /// Randomly sample a neutrino parent decay point and set it to transversity utils
  virtual void Apply(const ToyExperiment&, AnaEventC&);
  
  /// The only thing we allow for the generator is to get the seed
  UInt_t GetRandomSeed() const; 
  
  /// And change the seed
  void SetRandomSeed(UInt_t);
  
  /// Initialze random generator
  void InitializeRandomGenerator();
  
  /// Re-implement TH3 method to make use of local Random Generator
  bool GetRandom(Float_t&, Float_t&, Float_t&);
  
  /// Undo the systematic, set the values back to thier original ones 
  bool UndoSystematic(AnaEventC&);


protected:
  
  // Get the IDs of the RecObjects groups that are relevant for this systematic in a given selection
  Int_t GetRelevantRecObjectGroups(const SelectionBase&, Int_t*) const {return 0;}
  
  /// A random generator that can be used to generate throws.
  TRandom* _RandomGenerator;
  
  /// A histogram that provides the destribution of nu parent` decay points (X, Y, Z)
  /// Normalized one is used for sampling
  TH3* _DistributionHisto;
  
  /// Store the original nu decay pos from the transversity utils to be able to set it later
  Float_t _origNuParentDecayPos[3];
  
  
};

#endif
