#ifndef ToyBoxND280_h
#define ToyBoxND280_h

#include "ToyBoxB.hxx"
#include "BaseDataClasses.hxx"

class ToyBoxND280:public ToyBoxB{
public :
  
  ToyBoxND280();
  virtual ~ToyBoxND280(){}

  /// This method should be implemented by the derived class. If so it does nothing here 
  virtual void Reset();

  /// Reset this base class
  virtual void ResetBase();
  
public:

  /// For storing the true vertex, for analyses with no reconstructed primary vertex
  AnaTrueVertexB* TrueVertex;
};

#endif
