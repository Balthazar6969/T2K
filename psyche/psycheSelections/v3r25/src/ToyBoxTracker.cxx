#include "ToyBoxTracker.hxx"
#include <typeinfo>

//********************************************************************
ToyBoxTracker::ToyBoxTracker():ToyBoxND280(){
//******************************************************************** 
  
  Vertex=NULL;
  MainTrack=HMtrack=SHMtrack=HMNtrack=SHMNtrack=HMPtrack=SHMPtrack=NULL;
  VetoTrack=NULL;
  OOFVtrack=NULL;
  NegativeTPCtracks = NULL;
  PositiveTPCtracks = NULL;
  nNegativeTPCtracks = 0;
  nPositiveTPCtracks = 0;
  nLikeComputed = 0;
  nLikelihood = 0;
} 

//********************************************************************
ToyBoxTracker::~ToyBoxTracker(){
//******************************************************************** 

  if (NegativeTPCtracks) delete [] NegativeTPCtracks;
  if (PositiveTPCtracks) delete [] PositiveTPCtracks;
} 

//********************************************************************
void ToyBoxTracker::Reset(){
//******************************************************************** 

  if (!_ResetCheckDone){
    if( typeid(*this) !=  typeid(ToyBoxTracker)){
      std::cerr << "ERROR in ToyBoxTracker::Reset(). Either this mandatory method is not implemented " 
                << "by the derived class '" << typeid(*this).name() << "' "
                << "or ToyBoxTracker::Reset() is called from the Reset method of the derived class. "
                << "Please correct any of these bugs. " << std::endl;
      
      exit(1);
    }
    _ResetCheckDone=true;
  }
  
}

//********************************************************************
void ToyBoxTracker::ResetBase(){
//******************************************************************** 

  /// Must delete these objects, which are created in the steps when applying the selection.
  ///  TODO: should avoid that in the next version since multithreading requires 
  ///        no heap allocation of memory
  if (Vertex) delete Vertex; 
  if (NegativeTPCtracks) delete [] NegativeTPCtracks;
  if (PositiveTPCtracks) delete [] PositiveTPCtracks;
  
  Vertex=NULL;
  MainTrack=HMtrack=SHMtrack=HMNtrack=SHMNtrack=HMPtrack=SHMPtrack=NULL;
  VetoTrack=NULL;
  OOFVtrack=NULL;
  NegativeTPCtracks = NULL;
  PositiveTPCtracks = NULL;
  nNegativeTPCtracks = 0;
  nPositiveTPCtracks = 0;
  nLikeComputed = 0;
  nLikelihood = 0;

  ToyBoxND280::ResetBase();
}  

