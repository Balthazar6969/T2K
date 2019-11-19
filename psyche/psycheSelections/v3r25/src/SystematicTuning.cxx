#include "SystematicTuning.hxx"
#include "Parameters.hxx"

namespace systTuning{
  bool APPLY_SYST_FINE_TUNING = false;

}

//********************************************************************
void systTuning::Initialize(){
//********************************************************************
  
  systTuning::APPLY_SYST_FINE_TUNING = (bool)ND::params().GetParameterI("psycheSelections.Systematics.ApplyFineTuning");
  
}

