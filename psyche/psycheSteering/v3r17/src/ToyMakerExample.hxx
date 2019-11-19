#ifndef ToyMakerExample_h
#define ToyMakerExample_h

#include "ToyMaker.hxx"
#include "TRandom3.h"

/// Creates ToyExperiments

class ToyMakerExample: public ToyMaker{
  
public:
  
  /// Create the Toy experiment 
  ToyMakerExample(UInt_t seed, bool zero_var=false);
  
  /// Everyone should have a destructor.
  virtual ~ToyMakerExample() {}

  /// Fills the Toy Experiment with a given index
  void FillToyExperiment(ToyExperiment& toy);

  /// returns the random seed
  UInt_t GetSeed() const {return _seed;}

protected:

  /// A random generator that can be used to generate throws.
  TRandom3 _RandomGenerator;

  /// The random seed used
  UInt_t _seed;

  /// Apply 0 variation
  bool _zero_var;
};

#endif
