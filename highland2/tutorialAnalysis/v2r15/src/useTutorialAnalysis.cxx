#include "useTutorialAnalysis.hxx"
#include "FiducialVolumeDefinition.hxx"
#include "Parameters.hxx"
#include "tutorialSelection.hxx"
#include "tutorialBranchesSelection.hxx"
#include "tutorialCorrection.hxx"
#include "tutorialWeightSystematics.hxx"
#include "tutorialVariationSystematics.hxx"
#include "CategoriesUtils.hxx"
#include "BasicUtils.hxx"


/*  
    This is an example of how to use several existing analyses in our analysis.
    In this way we can for example add to the micro-tree variables that are already defined in other analyses. 

    !!!!!!!! IMPORTANT !!!!!!!
    To avoid major modifications in AnalysisAlgorithm several workarounds has been taken. 
    The functionality below will improve (and made easier) once we have clearer ideas about what we want
*/

//********************************************************************
useTutorialAnalysis::useTutorialAnalysis(AnalysisAlgorithm* ana) : baseTrackerAnalysis(ana) {
//********************************************************************
  
  // Add the package version (to be stored in the "config" tree)
  ND::versioning().AddPackage("tutorialAnalysis", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("TUTORIALANALYSISROOT")));
}

//********************************************************************
bool useTutorialAnalysis::Initialize(){
//********************************************************************

  // Create a tutorialAnalysis passing this analysis to the constructor. In that way the same managers are used
  _tutorialAnalysis = new tutorialAnalysis(this);

  // Use the tutorialAnalysis (in practice that means that the same box and event will be used for the tutorialAnalysis as for this analysis)
  UseAnalysis(_tutorialAnalysis);

  // Create a nueCCAnalysis passing this analysis to the constructor. In that way the same managers are used
  _nueCCAnalysis = new nueCCAnalysis(this);

  // Use the nueCCAnalysis (in practice that means that the same box and event will be used for the nueCCAnalysis as for this analysis)
  UseAnalysis(_nueCCAnalysis);

  // Initialize the used analyses
  // The system complaints with messages like this "category 'nuparent' already exists !!! Replace properties ..."
  // because the same categories are added twice. Need to work out how to solve that
  if (!_tutorialAnalysis->Initialize()) return false;
  if (!_nueCCAnalysis->Initialize()) return false;

  // Minimum accum level to save event into the output Micro-trees
  SetMinAccumCutLevelToSave(ND::params().GetParameterI("tutorialAnalysis.MinAccumLevelToSave"));

  return true;
}

//********************************************************************
void useTutorialAnalysis::DefineSelections(){
//********************************************************************

  // Define the selections of tutorialAnalysis
  _tutorialAnalysis->DefineSelections();

  // Define the selections of nueCCAnalysis
  _nueCCAnalysis->DefineSelections();
}

//********************************************************************
void useTutorialAnalysis::DefineCorrections(){
//********************************************************************

  // Define the corrections of tutorialAnalysis
  _tutorialAnalysis->DefineCorrections();

  // Define the corrections of nueCCAnalysis
  _nueCCAnalysis->DefineCorrections();
}

//********************************************************************
void useTutorialAnalysis::DefineSystematics(){
//********************************************************************

  // Define the systematics of tutorialAnalysis
  _tutorialAnalysis->DefineSystematics();

  // Define the systematics of nueCCAnalysis
  // The system complains when this line is uncommented because the systematics in baseTrackerAnalysis are added twice. 
  // We need modifications in AnalysisAlgorithm to avoid that
  //_nueCCAnalysis->DefineSystematics();
}

//********************************************************************
void useTutorialAnalysis::DefineConfigurations(){
//********************************************************************

  // Define the configurations of tutorialAnalysis
  _tutorialAnalysis->DefineConfigurations();

  // Define the configurations of nueCCAnalysis. Don't addnueCCAnalysis configurations since it may gives problems
  // We need changes in AnalysisAlgorithm to safely do that
  //  _nueCCAnalysis->DefineConfigurations();
}

//********************************************************************
void useTutorialAnalysis::DefineMicroTrees(bool addBase){
//********************************************************************

  // Add all variables in tutorialAnalysis (including the ones in its base package)
  _tutorialAnalysis->DefineMicroTrees(addBase);

  // Add all variables in nueCCAnalysis. No need to add again the variables in the base package (baseTrackerAnalysis). Hence the false
  _nueCCAnalysis->DefineMicroTrees(false);

  // Add some extra variables to the micro-trees
  AddVar4VF(output(), selmu_endpos,     "muon candidate reconstructed end position");
}

//********************************************************************
void useTutorialAnalysis::DefineTruthTree(){
//********************************************************************

  // Add all variables of tutorialAnalysis
  _tutorialAnalysis->DefineTruthTree();

  // Add all variables of nueCCAnalysis
  // The system complains when this line is uncommented because the variables in baseTrackerAnalysis are added twice. 
  // We need modifications in AnalysisAlgorithm to avoid that
  //  _nueCCAnalysis->DefineTruthTree();
}

//********************************************************************
void useTutorialAnalysis::FillMicroTrees(bool addBase){
//********************************************************************

  // Fill all variables in tutorialAnalysis  
  _tutorialAnalysis->FillMicroTrees(addBase);  

  // Fill all variables in nueCCAnalysis
  SetSelectedSelection(1);  // We need this line to tell _nueCCAnalysis->FillMicroTrees() which box should be used
  _nueCCAnalysis->FillMicroTrees(false);  // false because we don't wnat to fill variables in baseTrackerAnalysis again
  SetSelectedSelection(0);

  const ToyBoxTracker* box = static_cast<const ToyBoxTracker*>(&boxB());

  // Fill the extra variable
  if (box->HMNtrack){
    output().FillVectorVarFromArray(selmu_endpos, box->HMNtrack->PositionEnd, 4);
  }
}

//********************************************************************
void useTutorialAnalysis::FillToyVarsInMicroTrees(bool addBase){
//********************************************************************

  // Fill all variables in tutorialAnalysis  
  _tutorialAnalysis->FillToyVarsInMicroTrees(addBase);  

  // Fill all variables in nueCCAnalysis. Don't fill again the variables in baseTrackerAnalysis. Hence the false  
  SetSelectedSelection(1); // We need this line to tell _nueCCAnalysis->FillToyVarsInMicroTrees() which box should be used
  _nueCCAnalysis->FillToyVarsInMicroTrees(false);  
  SetSelectedSelection(0);
}

//********************************************************************
bool useTutorialAnalysis::CheckFillTruthTree(const AnaTrueVertex& vtx){
//********************************************************************
 
  return (_tutorialAnalysis->CheckFillTruthTree(vtx) || _nueCCAnalysis->CheckFillTruthTree(vtx));
}

//********************************************************************
void useTutorialAnalysis::FillTruthTree(const AnaTrueVertex& vtx){
//********************************************************************

  // Fill all variables in tutorialAnalysis  
  _tutorialAnalysis->FillTruthTree(vtx);

  // Fill all variables in nueCCAnalysis  
  // must uncomment the DefineTruthTree to uncomment this
  //  _nueCCAnalysis->FillTruthTree(vtx);
}

//********************************************************************
//void useTutorialAnalysis::FillTruthTree(){
//********************************************************************
/*
  static_cast<baseAnalysis*>(_tutorialAnalysis)->FillTruthTree();
}
*/
//********************************************************************
void useTutorialAnalysis::FillCategories(){
//********************************************************************

  // Fill categories in tutorialAnalysis  
  _tutorialAnalysis->FillCategories();

  // Fill all variables in nueCCAnalysis  
  SetSelectedSelection(1);  // We need this line to tell _nueCCAnalysis->FillCategories() which box should be used
  _nueCCAnalysis->FillCategories();
  SetSelectedSelection(0);

}
