#include "baseP0DAnalysis.hxx"

//********************************************************************
baseP0DAnalysis::baseP0DAnalysis(AnalysisAlgorithm* ana): baseAnalysis(ana){
//********************************************************************
    
  // Add the package version
  ND::versioning().AddPackage("baseP0DAnalysis", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("BASEP0DANALYSISROOT")));
}

//********************************************************************
void baseP0DAnalysis::DefineSystematics(){
//********************************************************************

  baseAnalysis::DefineSystematics();
}

//********************************************************************
void baseP0DAnalysis::DefineCorrections(){
//********************************************************************

  baseAnalysis::DefineCorrections();
}

//********************************************************************
void baseP0DAnalysis::DefineConfigurations(){
//********************************************************************

  baseAnalysis::DefineConfigurations();  
}

//********************************************************************
void baseP0DAnalysis::DefineMicroTrees(bool addBase){
//********************************************************************

  if (addBase) baseAnalysis::DefineMicroTrees(addBase);

  // -------- Add variables to the analysis tree ----------------------

  AddVarI(output(), np0dtracks,          "number of tracks containing P0D segment");
  AddVarI(output(), np0donlytracks,      "number of tracks with only P0D");

   int isUsingReconDirP0D     = ND::params().GetParameterI("highlandIO.FlatTree.UseReconDirP0D");

  if (isUsingReconDirP0D){
    AddVarI(output(), np0dreconvertices,   "number of P0DRecon vertices in bunch");
    AddVarI(output(), np0drecontracks,   "number of P0DRecon tracks in bunch");
    AddVarI(output(), np0dreconclusters,   "number of P0DRecon clusters in bunch");
  }
}

//********************************************************************
void baseP0DAnalysis::DefineTruthTree(){
//********************************************************************

  baseAnalysis::DefineTruthTree();
}

//********************************************************************
void baseP0DAnalysis::FillMicroTrees(bool addBase){
//********************************************************************

  if (addBase) baseAnalysis::FillMicroTreesBase(addBase);

  // Track counters
  AnaTrackB* dummy[NMAXPARTICLES];
  int nP0D         = anaUtils::GetAllTracksUsingDet(GetBunch(), SubDetId::kP0D,  dummy);
  int nP0DOnly     = anaUtils::GetAllTracksUsingOnlyDet(GetBunch(), SubDetId::kP0D,  dummy);

 
  output().FillVar(np0dtracks, nP0D);
  output().FillVar(np0donlytracks,nP0DOnly);
 

 //Local Recon counters
  
  //P0D local recon parameter must be enabled in highlandIO

  int isUsingReconDirP0D     = ND::params().GetParameterI("highlandIO.FlatTree.UseReconDirP0D");

  
  if (isUsingReconDirP0D == 1){
    
    AnaSpill* spill = static_cast<AnaSpill*>(&(input().GetSpill()));

    AnaLocalReconBunch*  localBunch = NULL; 
  
    int nP0DReconTracks    = -1;
    int nP0DReconClusters  = -1;
    int nP0DReconVertices  = -1;

    // Loop over all bunches
    for (unsigned int iBunch=0; iBunch<spill->Bunches.size(); iBunch++){

      // Cast this bunch to the local variety
      localBunch = dynamic_cast<AnaLocalReconBunch*>(spill->Bunches[iBunch]);
      
      // Pointer will be NULL if dynamic_cast fails
      if(localBunch){
	nP0DReconTracks    = localBunch->P0DReconParticles.size();
	nP0DReconClusters  = localBunch->P0DReconClusters.size();
	nP0DReconVertices  = localBunch->P0DReconVertices.size();
	
      }
      else {
	std::cerr<<"Invalid local bunch, did you enable UseReconDirP0D = 1 in highlandIO.parameters.dat?\n";
      }
      
    } // End of loop over Bunches
  
    output().FillVar(np0drecontracks, nP0DReconTracks);
    output().FillVar(np0dreconvertices, nP0DReconVertices);
    output().FillVar(np0dreconclusters, nP0DReconClusters);
  } // End of local recon


}

//********************************************************************
void baseP0DAnalysis::FillToyVarsInMicroTrees(bool addBase){
//********************************************************************

  if (addBase) baseAnalysis::FillToyVarsInMicroTreesBase(addBase);
}

//********************************************************************
void baseP0DAnalysis::FillTruthTreeBase(const AnaTrueVertex& vtx, const SubDetId::SubDetEnum det, bool IsAntinu){
//********************************************************************

  // Fill the common variables
  baseAnalysis::FillTruthTreeBase(vtx, det, IsAntinu);
}
