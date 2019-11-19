#include "EventBoxTracker.hxx"


//********************************************************************
EventBoxTracker::EventBoxTracker():EventBoxB(){
//******************************************************************** 

    for (Int_t i=0;i<2;i++){
      nFGDMichelElectrons[i] = 0;
      FGDMichelElectrons[i] = NULL;
    }
}

//********************************************************************
EventBoxTracker::~EventBoxTracker(){
//********************************************************************

  for (Int_t i=0;i<2;i++){
    if (FGDMichelElectrons[i])
      delete [] FGDMichelElectrons[i];
    FGDMichelElectrons[i] = NULL;
  }
} 
