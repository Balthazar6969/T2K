#ifndef RooTrackerVtxManager_h
#define RooTrackerVtxManager_h

#include <string.h>
#include <stdio.h>
#include <iostream>
#include <map>
#include <vector>
#include <stdexcept>

#include "BasicTypes.hxx"

class RooTrackerVtxManager{
public :

  RooTrackerVtxManager();
  virtual ~RooTrackerVtxManager(){}
  
  bool InitializeEntry();

  /// Methods to set and get the current entry in the RooTrackerVtx tree
  void SetRooVtxEntry(Int_t entry){_RooVtxEntry=entry;}
  Int_t GetRooVtxEntry() const {return _RooVtxEntry;}

  /// Methods to check whether the current RooTrackerVtxEntry has been already saved.
  void SetRooVtxEntrySaved(bool saved){_RooVtxEntrySaved=saved;}
  bool GetRooVtxEntrySaved() const {return _RooVtxEntrySaved;}

  /// Methods to check whether the current RooTrackerVtxEntry has been already scheduled for saving
  void SetSaveRooVtxEntry(bool save){_saveRooVtxEntry=save;}
  bool GetSaveRooVtxEntry() const {return _saveRooVtxEntry;}


  /// Increment by one the current entry in the RooTrackerVtx tree
  void IncrementRooVtxEntry(){_RooVtxEntry++;}

  /// Reset to 0 the current entry in the RooTrackerVtx tree
  void ResetRooVtxEntry(){_RooVtxEntry=0;}

 protected:

  /// current RooTrackerVtx entry
  Int_t _RooVtxEntry;

  /// has the current RooTrackerVtx entry been saved ?
  bool _RooVtxEntrySaved;

  /// has the current RooTrackerVtx entry scheduled to be saved?
  bool _saveRooVtxEntry;

};


#endif


