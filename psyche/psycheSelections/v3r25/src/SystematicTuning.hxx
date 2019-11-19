#ifndef SystematicTuning_h
#define SystematicTuning_h

#include "CoreDataClasses.hxx"

/// This file defines  some parameters to tune the systematics for specific selections

namespace systTuning{
  /// General tuning,  the concept of apply the systematic for only the "relevant" objects
  extern bool APPLY_SYST_FINE_TUNING;

  void Initialize();
};

#endif
