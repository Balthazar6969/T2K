#include "ToyBoxND280.hxx"
#include <typeinfo>

//********************************************************************
ToyBoxND280::ToyBoxND280():ToyBoxB(){
//******************************************************************** 
  
  TrueVertex=NULL;
} 

//********************************************************************
void ToyBoxND280::Reset(){
//******************************************************************** 

  if (!_ResetCheckDone){
    if( typeid(*this) !=  typeid(ToyBoxND280)){
      std::cerr << "ERROR in ToyBoxND280::Reset(). Either this mandatory method is not implemented " 
                << "by the derived class '" << typeid(*this).name() << "' "
                << "or ToyBoxND280::Reset() is called from the Reset method of the derived class. "
                << "Please correct any of these bugs. " << std::endl;
      
      exit(1);
    }
    _ResetCheckDone=true;
  }
  
}

//********************************************************************
void ToyBoxND280::ResetBase(){
//******************************************************************** 

  TrueVertex=NULL;
  SoftReset();
}  

