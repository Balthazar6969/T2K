#include "gammaAnalysis.hxx"
#include "gammaSelection.hxx"
#include "gammaFGD2Selection.hxx"

#include "FiducialVolumeDefinition.hxx"
#include "Parameters.hxx"
#include "UseGlobalAltMomCorrection.hxx"
#include "CategoriesUtils.hxx"
#include "BasicUtils.hxx"
#include "CutUtils.hxx"
#include "nueCCUtils.hxx"
#include "CategToolsNueCC.hxx"
#include "baseToyMaker.hxx"

//********************************************************************
gammaAnalysis::gammaAnalysis(AnalysisAlgorithm* ana) : baseTrackerAnalysis(ana) {
//********************************************************************

  // Add the package version
  ND::versioning().AddPackage("gammaAnalysis", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("GAMMAANALYSISROOT")));

  // Create a nueCCAnalysis passing this analysis to the constructor. In that way the same managers are used
  _nueCCAnalysis = new nueCCAnalysis(this);

  // Use the neCCAnalysis (in practice that means that the same box and event will be used for the nueCCAnalysis as for this analysis)
  UseAnalysis(_nueCCAnalysis);

}

//********************************************************************
bool gammaAnalysis::Initialize(){
//********************************************************************

  // Initialize the baseTrackerAnalysis
  if(!baseTrackerAnalysis::Initialize()) return false;
  
  // Minimum accum level to save event into the output tree
  SetMinAccumCutLevelToSave(ND::params().GetParameterI("gammaAnalysis.MinAccumLevelToSave"));

  // which FGD 
  _whichFGD = ND::params().GetParameterI("gammaAnalysis.Analysis.FV.FGD");

  // Save all the secondary tarcks in the microtree
  _savesecondaries = ND::params().GetParameterI("gammaAnalysis.Analysis.SaveSecondaries");
  _nueCCAnalysis->SetSaveSecondaries(_savesecondaries);

  // GammaInitiator
  _savegammainitiator = ND::params().GetParameterI("gammaAnalysis.Analysis.SaveGammaInitiator");
  _nueCCAnalysis->SetSaveGammaInitiator(_savegammainitiator);
  
  // Add new categories specific for nue-gamma analysis
  nue_categs::AddCategories("",     false);
  nue_categs::AddCategories("fgd2", false);

  anaUtils::AddStandardCategories("");
  anaUtils::AddStandardCategories("fgd2");

  return true;
  
}

//********************************************************************
void gammaAnalysis::DefineSelections(){
//********************************************************************
 
  if      (_whichFGD == 1) // FGD1
    sel().AddSelection("kTrackerGamma",     "inclusive Gamma selection",     new gammaSelection(false));
  else if (_whichFGD == 2) // FGD2
    sel().AddSelection("kTrackerGammaFGD2", "inclusive GammaFGD2 selection", new gammaFGD2Selection(false));
  else { // both FGDs, in 2 selections
    sel().AddSelection("kTrackerGamma",     "inclusive Gamma selection",     new gammaSelection(false));
    sel().AddSelection("kTrackerGammaFGD2", "inclusive GammaFGD2 selection", new gammaFGD2Selection(false));
    std::cout << "WARNING::Accum_level is different for FGD1 and FGD2 gamma selections. Better to run choosing FGD1 OR FGD2." << std::endl;
  } 
}

//********************************************************************
void gammaAnalysis::DefineTruthTree(){
//********************************************************************

  _nueCCAnalysis->DefineTruthTree();

}

//********************************************************************
void gammaAnalysis::DefineMicroTrees(bool addBase){
//********************************************************************

  // -------- Add variables to the analysis tree ----------------------
  if(addBase) _nueCCAnalysis->DefineMicroTrees(addBase);

}

//********************************************************************
void gammaAnalysis::FillMicroTrees(bool addBase){
//********************************************************************

  if (addBase) _nueCCAnalysis->FillMicroTrees(addBase);
  
}

//********************************************************************
void gammaAnalysis::FillTruthTree(const AnaTrueVertex& vtx){
//********************************************************************
  
  _nueCCAnalysis->FillTruthTreeBase(vtx, false);
  
}

//********************************************************************
void gammaAnalysis::FillToyVarsInMicroTrees(bool addBase){
//********************************************************************

  if (addBase) _nueCCAnalysis->FillToyVarsInMicroTrees(addBase);
  
}

//********************************************************************
bool gammaAnalysis::CheckFillTruthTree(const AnaTrueVertex& vtx){
//********************************************************************

  SubDetId::SubDetEnum fgdID;
  if (_whichFGD == 1) fgdID = SubDetId::kFGD1;
  if (_whichFGD == 2) fgdID = SubDetId::kFGD2;
  if (_whichFGD >  2) fgdID = SubDetId::kFGD;

  return (nue_categs::ThereWasGammaInFGD(vtx, fgdID));

}

//********************************************************************
void gammaAnalysis::FillCategories(){
//********************************************************************

  anaUtils::FillCategories(&GetEvent(), static_cast<AnaTrack*>(box().MainTrack),"",           SubDetId::kFGD1, false);
  anaUtils::FillCategories(&GetEvent(), static_cast<AnaTrack*>(box().MainTrack),"fgd2",       SubDetId::kFGD2, false);
  
  nue_categs::FillCategories(&GetEvent(), static_cast<AnaTrack*>(box().MainTrack), "",           SubDetId::kFGD1, false);
  nue_categs::FillCategories(&GetEvent(), static_cast<AnaTrack*>(box().MainTrack), "fgd2",       SubDetId::kFGD2, false);
}

//********************************************************************
void gammaAnalysis::FillConfigTree(){
//********************************************************************
  _nueCCAnalysis->FillConfigTree();
}
