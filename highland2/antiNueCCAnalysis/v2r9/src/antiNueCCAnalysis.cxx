#include "antiNueCCAnalysis.hxx"
#include "nueCCAnalysis.hxx"
#include "nueCCSelection.hxx"
#include "antiNueCCFGD2Selection.hxx"
#include "antiNueCCSelection.hxx"

#include "CutUtils.hxx"
#include "nueCCUtils.hxx"
#include "CategToolsNueCC.hxx"

#include "FiducialVolumeDefinition.hxx"
#include "Parameters.hxx"
#include "UseGlobalAltMomCorrection.hxx"
#include "CategoriesUtils.hxx"
#include "BasicUtils.hxx"
#include "baseToyMaker.hxx"

//********************************************************************
antiNueCCAnalysis::antiNueCCAnalysis(AnalysisAlgorithm* ana) : baseTrackerAnalysis(ana) {
//********************************************************************

  // Add the package version
  ND::versioning().AddPackage("antiNueCCAnalysis", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("NUECCANALYSISROOT")));

 // Create a nueCCAnalysis passing this analysis to the constructor. In that way the same managers are used
  _nueCCAnalysis = new nueCCAnalysis(this);

  // Use the nueCCAnalysis (in practice that means that the same box and event will be used for the nueCCAnalysis as for this analysis)
  UseAnalysis(_nueCCAnalysis);
}

//********************************************************************
bool antiNueCCAnalysis::Initialize(){
//********************************************************************

  // Initialize the baseTrackerAnalysis
  if(!baseTrackerAnalysis::Initialize()) return false;
  
  // which analysis: FGD1, FGD2 or FGDs
  _whichFGD = ND::params().GetParameterI("antiNueCCAnalysis.Analysis.Sample.whichFGD");

  // Minimum accum level to save event Int_to the output tree
  if(_whichFGD == 1)
    SetMinAccumCutLevelToSave(ND::params().GetParameterI("antiNueCCAnalysis.MinAccumLevelToSave.FGD1"));
  else
    SetMinAccumCutLevelToSave(ND::params().GetParameterI("antiNueCCAnalysis.MinAccumLevelToSave.FGD2"));
    

  // Save all the secondary tarcks in the microtree
  _savesecondaries = ND::params().GetParameterI("antiNueCCAnalysis.Analysis.SaveSecondaries");
  _nueCCAnalysis->SetSaveSecondaries(_savesecondaries);

  // GammaInitiator
  _savegammainitiator = ND::params().GetParameterI("antiNueCCAnalysis.Analysis.SaveGammaInitiator");
  _nueCCAnalysis->SetSaveGammaInitiator(_savegammainitiator);

  // Add new categories specific for nue analysis
  nue_categs::AddCategories("",true);
  nue_categs::AddCategories("fgd2", true);

  //anaUtils::AddStandardCategories();
  //anaUtils::AddStandardCategories("fgd2");

  anaUtils::AddStandardAntiNumuCategories();
  anaUtils::AddStandardAntiNumuCategories("fgd2");

  return true;
}

//********************************************************************
void antiNueCCAnalysis::DefineSelections(){
//********************************************************************

  // ----- Inclusive CC -----------
  if      (_whichFGD==1) // FGD1
    sel().AddSelection("kTrackerAntiNueCC", "inclusive antiNueCC selection", new antiNueCCSelection(false));     // true/false for forcing break
  else if (_whichFGD==2) // FGD2
    sel().AddSelection("kTrackerAntiNueCCFGD2", "inclusive antiNueCCFGD2 selection", new antiNueCCFGD2Selection(false));     // true/false for forcing break
  else if (_whichFGD==3) { // both FGDs, in 2 selections
    std::cout << "WARNING::Minimum accum_level for FGD1 and FGD2 selections is not the same. Last two cuts in FGD1 selection will be ignored. You should run the FGD1 and FGD2 selections separately." << std::endl;
    sel().AddSelection("kTrackerAntiNueCC", "inclusive antiNueCC selection", new antiNueCCSelection(false));     // true/false for forcing break
    sel().AddSelection("kTrackerAntiNueCCFGD2", "inclusive antiNueCCFGD2 selection", new antiNueCCFGD2Selection(false));     // true/false for forcing break
  }
}

//********************************************************************
void antiNueCCAnalysis::DefineTruthTree(){
//********************************************************************
    // Variables from nueCCAnalysis
  _nueCCAnalysis->DefineTruthTree();
}

//********************************************************************
void antiNueCCAnalysis::DefineMicroTrees(bool addBase){
//********************************************************************
  // -------- Add variables to the analysis tree ----------------------
  
  // Variables from nueCCAnalysis
  if (addBase) _nueCCAnalysis->DefineMicroTrees(addBase);
}

//********************************************************************
void antiNueCCAnalysis::FillMicroTrees(bool addBase){
//********************************************************************
  // Variables from nueCCAnalysis
  if (addBase) _nueCCAnalysis->FillMicroTrees(addBase);
}

//********************************************************************
void antiNueCCAnalysis::FillToyVarsInMicroTrees(bool addBase){
//********************************************************************
  // Fill all tree variables that vary for each virtual analysis (toy experiment)

  if (addBase) _nueCCAnalysis->FillToyVarsInMicroTrees(addBase);
}

//********************************************************************
bool antiNueCCAnalysis::CheckFillTruthTree(const AnaTrueVertex& vtx){
//********************************************************************
  
  SubDetId::SubDetEnum fgdID;
  if (_whichFGD == 1) fgdID = SubDetId::kFGD1;
  if (_whichFGD == 2) fgdID = SubDetId::kFGD2;
  if (_whichFGD >  2) fgdID = SubDetId::kFGD;

  bool AntiNu = true;
  return nue_categs::IsNuESignal(vtx, fgdID, AntiNu);
}

//********************************************************************
void antiNueCCAnalysis::FillTruthTree(const AnaTrueVertex& vtx){
//********************************************************************

  bool AntiNu = true;
  _nueCCAnalysis->FillTruthTreeBase(vtx, AntiNu);
}

//********************************************************************
void antiNueCCAnalysis::FillCategories(){
//********************************************************************
  // Fill the track categories for color drawing

  bool IsAntinu = true;

  anaUtils::FillCategories(&GetEvent(), static_cast<AnaTrack*>(box().MainTrack),"",SubDetId::kFGD1,IsAntinu);
  anaUtils::FillCategories(&GetEvent(), static_cast<AnaTrack*>(box().MainTrack), "fgd2", SubDetId::kFGD2,IsAntinu);

  nue_categs::FillCategories(&GetEvent(), box().MainTrack, "", SubDetId::kFGD1, IsAntinu);
  nue_categs::FillCategories(&GetEvent(), box().MainTrack, "fgd2", SubDetId::kFGD2, IsAntinu);
}

//********************************************************************
void antiNueCCAnalysis::FillConfigTree(){
//********************************************************************
  _nueCCAnalysis->FillConfigTree();
}
