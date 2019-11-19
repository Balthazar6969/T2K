#include "RooTrackerVtxManager.hxx"

//********************************************************************
RooTrackerVtxManager::RooTrackerVtxManager(){
//********************************************************************

  _RooVtxEntry=0;

}

//********************************************************************
bool RooTrackerVtxManager::InitializeEntry(){
//********************************************************************

  _RooVtxEntrySaved=false;
  _saveRooVtxEntry=false;

  return true;
}
