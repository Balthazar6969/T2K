#include "numuBkgInAntiNuModeAnalysis.hxx"
#include "FiducialVolumeDefinition.hxx"
#include "Parameters.hxx"
#include "numuBkgInAntiNuModeCCSelection.hxx"
#include "numuBkgInAntiNuModeCCMultiTrackSelection.hxx"
#include "numuBkgInAntiNuModeCCMultiPiSelection.hxx"
#include "numuBkgInAntiNuModeCCFGD2Selection.hxx"
#include "numuBkgInAntiNuModeCCMultiTrackFGD2Selection.hxx"
#include "numuBkgInAntiNuModeCCMultiPiFGD2Selection.hxx"
#include "CategoriesUtils.hxx"


//********************************************************************
numuBkgInAntiNuModeAnalysis::numuBkgInAntiNuModeAnalysis(AnalysisAlgorithm* ana) : baseTrackerAnalysis(ana) {
//********************************************************************

  // Add the package version
  ND::versioning().AddPackage("numuBkgInAntiNuModeAnalysis", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("NUMUBKGINANTINUMODEANALYSISROOT")));

  // Create a numuCCAnalysis passing this analysis to the constructor. In that way the same managers are used
  _numuCCAnalysis = new numuCCAnalysis(this);
  _numuCCMultiPiAnalysis = new numuCCMultiPiAnalysis(this);

  // Use the numuCCAnalysis (in practice that means that the same box and event will be used for the numuCCAnalysis as for this analysis)
  UseAnalysis(_numuCCAnalysis);
  UseAnalysis(_numuCCMultiPiAnalysis);

}

//********************************************************************
bool numuBkgInAntiNuModeAnalysis::Initialize() {
//********************************************************************

  // Minimum accum level to save event into the output tree
  SetMinAccumCutLevelToSave(ND::params().GetParameterI("numuBkgInAntiNuModeAnalysis.MinAccumLevelToSave"));

  // CC-inclusive?
  fCCInclusive = ND::params().GetParameterI("numuBkgInAntiNuModeAnalysis.Selections.OnlyCCInc");

  // Multiple track
  _doMultiTrack = ND::params().GetParameterI("numuBkgInAntiNuModeAnalysis.Selections.doMultiTrack");

  // Multiple pion  
  _doMultiPi = ND::params().GetParameterI("numuBkgInAntiNuModeAnalysis.Selections.doMultiPi");
  
  // which analysis: FGD1, FGD2 or FGDs
  _whichFGD = ND::params().GetParameterI("numuBkgInAntiNuModeAnalysis.Selections.whichFGD");

  // use numuCC selection
  if (_doMultiTrack)
    if (!_numuCCAnalysis->Initialize()) return false;
    
  // use Multiple Pion selection  
  if (_doMultiPi)
    if (!_numuCCMultiPiAnalysis->Initialize()) return false;

  return true;
}

//********************************************************************
void numuBkgInAntiNuModeAnalysis::DefineSelections(){
//********************************************************************

  // ----- Inclusive CC -----------
  if (_whichFGD==1){ // FGD1
    if(fCCInclusive)
      sel().AddSelection("kTrackerNumuBkgInAntiNuModeCCInc",      "inclusive numuBkg selection in antinu mode",     new numuBkgInAntiNuModeCCSelection(false));
    // ---- CC Multi Pion Samples----
    else{
      if (_doMultiTrack) sel().AddSelection("kTrackerNumuBkgInAntiNuModeMultiTrack",    "numuBkg Multiple Track selection in antinu mode", new numuBkgInAntiNuModeCCMultiTrackSelection(false));
      else if (_doMultiPi) sel().AddSelection("kTrackerNumuBkgInAntiNuModeMultiPi",    "numuBkg Multiple Pion selection in antinu mode", new numuBkgInAntiNuModeCCMultiPiSelection(false));
    }
  }
  else if (_whichFGD==2){ // FGD2
    if(fCCInclusive)
      sel().AddSelection("kTrackerNumuBkgInAntiNuModeCCFGD2",      "inclusive numuBkg selection in antinu mode in FGD2",     new numuBkgInAntiNuModeCCFGD2Selection(false));
    // ---- CC Multi Pion Samples----
    else{
      if (_doMultiTrack) sel().AddSelection("kTrackerNumuBkgInAntiNuModeMultiTrackFGD2",    "numuBkg Multiple Track selection in antinu mode in FGD2", new numuBkgInAntiNuModeCCMultiTrackFGD2Selection(false));
      else if (_doMultiPi) sel().AddSelection("kTrackerNumuBkgInAntiNuModeMultiPiFGD2",    "numuBkg Multiple Pion selection in antinu mode in FGD2", new numuBkgInAntiNuModeCCMultiPiFGD2Selection(false));
    }
  }
  else if (_whichFGD==3){ // both FGDs
    // ------------FGD1
    if(fCCInclusive)
      sel().AddSelection("kTrackerNumuBkgInAntiNuModeCCInc",      "inclusive numuBkg selection in antinu mode",     new numuBkgInAntiNuModeCCSelection(false));
    // ---- CC Multi Pion Samples----
    else{
      if (_doMultiTrack) sel().AddSelection("kTrackerNumuBkgInAntiNuModeMultiTrack",    "numuBkg Multiple Track selection in antinu mode", new numuBkgInAntiNuModeCCMultiTrackSelection(false));
      else if (_doMultiPi) sel().AddSelection("kTrackerNumuBkgInAntiNuModeMultiPi",    "numuBkg Multiple Pion selection in antinu mode", new numuBkgInAntiNuModeCCMultiPiSelection(false));
    }
    // ------------FGD2
    if(fCCInclusive)
      sel().AddSelection("kTrackerNumuBkgInAntiNuModeCCFGD2",      "inclusive numuBkg selection in antinu mode in FGD2",     new numuBkgInAntiNuModeCCFGD2Selection(false));
    // ---- CC Multi Pion Samples----
    else{
      if (_doMultiTrack) sel().AddSelection("kTrackerNumuBkgInAntiNuModeMultiTrackFGD2",    "numuBkg Multiple Track selection in antinu mode in FGD2", new numuBkgInAntiNuModeCCMultiTrackFGD2Selection(false));
      else if (_doMultiPi) sel().AddSelection("kTrackerNumuBkgInAntiNuModeMultiPiFGD2",    "numuBkg Multiple Pion selection in antinu mode in FGD2", new numuBkgInAntiNuModeCCMultiPiFGD2Selection(false));
    }
  }
}

//********************************************************************
void numuBkgInAntiNuModeAnalysis::DefineMicroTrees(bool addBase){
//********************************************************************

  // -------- Add variables to the analysis tree ----------------------

  if (addBase){
    // Variables from numuCCAnalysis   
    if (_doMultiTrack) _numuCCAnalysis->DefineMicroTrees(addBase);
    // Variables from numuCCMultiPiAnalysis
    else if (_doMultiPi) _numuCCMultiPiAnalysis->DefineMicroTrees(addBase);
  }
}

//********************************************************************
void numuBkgInAntiNuModeAnalysis::DefineTruthTree(){
//********************************************************************
 
  // Variables from numuCCAnalysis
  if (_doMultiTrack) _numuCCAnalysis->DefineTruthTree();
  // Variables from numuCCMultiPiAnalysis
  else if (_doMultiPi) _numuCCMultiPiAnalysis->DefineTruthTree();
 
}

//********************************************************************
void numuBkgInAntiNuModeAnalysis::FillMicroTrees(bool addBase){
//********************************************************************

  if (addBase){
    //Variables from numuCCAnalysis
    if (_doMultiTrack) _numuCCAnalysis->FillMicroTrees(addBase); 
    // Variables from numuCCMultiPiAnalysis
    else if (_doMultiPi) _numuCCMultiPiAnalysis->FillMicroTrees(addBase);
  }
}

//********************************************************************
void numuBkgInAntiNuModeAnalysis::FillToyVarsInMicroTrees(bool addBase){
//********************************************************************

  // Fill all tree variables that vary for each virtual analysis (toy experiment)

  if (addBase){
    // Variables from numuCCAnalysis
    if (_doMultiTrack) _numuCCAnalysis->FillToyVarsInMicroTrees(addBase);
    // Variables from numuCCMultiPiAnalysis
    else if (_doMultiPi) _numuCCMultiPiAnalysis->FillToyVarsInMicroTrees(addBase);
  }
}

//********************************************************************
bool numuBkgInAntiNuModeAnalysis::CheckFillTruthTree(const AnaTrueVertex& vtx){
//********************************************************************

  SubDetId::SubDetEnum _fgdID;
  if (_whichFGD == 1) _fgdID = SubDetId::kFGD1;
  if (_whichFGD == 2) _fgdID = SubDetId::kFGD2;
  if (_whichFGD >  2) _fgdID = SubDetId::kFGD;
  // GetReactionCC already takes into account the outFV and also
  // the NuWro reaction code for 2p2h in prod5 (that is 70)
  bool numuBkg = (anaUtils::GetReactionCC(vtx, _fgdID) == 1);
  return(numuBkg);
}

//********************************************************************
void numuBkgInAntiNuModeAnalysis::FillTruthTree(const AnaTrueVertex& vtx){
//********************************************************************

  // Fill the common variables
  if (_doMultiTrack) _numuCCAnalysis->FillTruthTree(vtx);  
  else if (_doMultiPi) _numuCCMultiPiAnalysis->FillTruthTree(vtx);
  
}

//********************************************************************
void numuBkgInAntiNuModeAnalysis::FillCategories(){
//********************************************************************

  // Fill the track categories for color drawing
  if (_doMultiTrack) _numuCCAnalysis->FillCategories();
  else if (_doMultiPi) _numuCCMultiPiAnalysis->FillCategories();

}
