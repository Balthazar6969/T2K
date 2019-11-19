#include "CT4POTCorrection.hxx"
#include "Parameters.hxx"
#include "Versioning.hxx"
#include "DataClasses.hxx"

//********************************************************************
CT4POTCorrection::CT4POTCorrection() :  CorrectionBase() {
//********************************************************************
  MRrunLower = ND::params().GetParameterI("highlandCorrections.CT4POTCorrection.MRrunLower");
  MRrunUpper = ND::params().GetParameterI("highlandCorrections.CT4POTCorrection.MRrunUpper");
}

//********************************************************************
void CT4POTCorrection::Apply(AnaSpillC& spillC) {
//********************************************************************

#if !VERSION_HAS_OFFICIAL_POT
  AnaSpill* spill = static_cast<AnaSpill*>(&spillC);
  AnaBeam* beam = static_cast<AnaBeam*>(spill->Beam);
  int MRrun = beam->BeamRunNumber;

  if (spill->GetIsMC() && MRrun >= MRrunLower && MRrun <= MRrunUpper) {
    beam->POT = beam->POTCT4;
  }
#else
  (void)spillC;
#endif

}
