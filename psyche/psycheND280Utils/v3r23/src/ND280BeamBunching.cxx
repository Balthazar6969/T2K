#include "ND280BeamBunching.hxx"
#include "VersioningUtils.hxx"

//*****************************************************************************
ND280BeamBunching::ND280BeamBunching(): BeamBunching(){
//*****************************************************************************                 
  
  char filename[256];

  if (versionUtils::prod6_bunching)
    sprintf(filename,"%s/data/BunchPosition_PreProd6.dat",getenv("PSYCHEND280UTILSROOT"));
  else
    sprintf(filename,"%s/data/BunchPosition.dat",getenv("PSYCHEND280UTILSROOT"));

  ReadBeamBunchStructure(filename);

}

