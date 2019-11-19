#include "p0dNumuCCQEAnalysis.hxx"
#include "p0dNumuCCQESelection.hxx"
#include "Parameters.hxx"
#include "p0dCCQEOOFVSystematics.hxx"
#include "p0dPileUpCorrection.hxx"
#include "baseToyMaker.hxx"
#include <iostream>
using std::cout;
using std::endl;
//********************************************************************
p0dNumuCCQEAnalysis::p0dNumuCCQEAnalysis(AnalysisAlgorithm* ana) : baseTrackerAnalysis(ana) {
//********************************************************************

  // Add the package version
  ND::versioning().AddPackage("p0dNumuCCQEAnalysis", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("P0DNUMUCCQEANALYSISROOT")));

  // Create a p0dNumuCCAnalysis passing this analysis to the constructor. In that way the same managers are used
  // N.B. this resets the _flux to NULL in _p0dNumuCCAnalysis so if that is used to fill, we must call
  // _p0dNumuCCAnalysis->DefineConfigurations() which sets up the _flux weighting.
  _p0dNumuCCAnalysis = new p0dNumuCCAnalysis(this);

  // Use the p0dNumuCCAnalysis (in practice that means that the sabe box and event will be used for the p0dNumuCCAnalysis as for this analysis)
  UseAnalysis(_p0dNumuCCAnalysis);
}

//********************************************************************
bool p0dNumuCCQEAnalysis::Initialize(){
//********************************************************************

  // initialize the used analysis
  if (!_p0dNumuCCAnalysis->Initialize()) return false;

  // Minimum accum level to save event into the output tree
  SetMinAccumCutLevelToSave(ND::params().GetParameterI("p0dNumuCCQEAnalysis.MinAccumLevelToSave"));

  // save all numuCC in truth tree
  _saveAllNumuCC = ND::params().GetParameterI("p0dNumuCCQEAnalysis.TruthTree.SaveAllNumuCC");

  return true;
}

//********************************************************************
void p0dNumuCCQEAnalysis::DefineSelections(){
//********************************************************************

  // ----- Inclusive CC -----------
  sel().AddSelection("kP0DNumuCCQE",           "p0dNumuCCQE selection",     new p0dNumuCCQESelection(false));
}

//********************************************************************
void p0dNumuCCQEAnalysis::DefineCorrections(){
//********************************************************************
  //
  _p0dNumuCCAnalysis->DefineCorrections();

  if (ND::params().GetParameterI("p0dNumuCCQEAnalysis.Corrections.EnableP0DPileUp")){
    corr().AddCorrection("p0dpileup_corr", new p0dPileUpCorrection());
  }
}

//********************************************************************
void p0dNumuCCQEAnalysis::DefineSystematics(){
//********************************************************************
  //
  _p0dNumuCCAnalysis->DefineSystematics();

  syst().AddWeightSystematic(kp0dCCQEOOFV,"P0DCCQEOOFV",new p0dCCQEOOFVSystematics());
                             
}

//********************************************************************
void p0dNumuCCQEAnalysis::DefineConfigurations(){
//********************************************************************
  //
  _p0dNumuCCAnalysis->DefineConfigurations();

  Int_t ntoys = ND::params().GetParameterI("baseAnalysis.Systematics.NumberOfToys");
  Int_t randomSeed = ND::params().GetParameterI("baseAnalysis.Systematics.RandomSeed");
  bool enableSingleWeightSystConf = (bool) ND::params().GetParameterI("baseAnalysis.Configurations.EnableSingleWeightSystConfigurations");
  if (enableSingleWeightSystConf){
    if (ND::params().GetParameterI("p0dNumuCCQEAnalysis.Weights.EnableOOFV")){
      AddConfiguration(conf(),p0dccqeoofv_syst,ntoys,randomSeed,new baseToyMaker(randomSeed));
      conf().EnableSystematic(kp0dCCQEOOFV,p0dccqeoofv_syst);
    }
  }

  if (ND::params().GetParameterI("baseAnalysis.Configurations.EnableAllSystematics")){
    if (ND::params().GetParameterI("p0dNumuCCQEAnalysis.Weights.EnableOOFV"))
      conf().EnableSystematic(kp0dCCQEOOFV,all_syst);
  }

}

//********************************************************************
void p0dNumuCCQEAnalysis::DefineMicroTrees(bool addBase){
//********************************************************************
  _p0dNumuCCAnalysis->DefineMicroTrees(addBase);
}
//

//********************************************************************
void p0dNumuCCQEAnalysis::DefineTruthTree(){
  //********************************************************************

  // Variables from baseTrackerAnalysis (run, event, ...)
  _p0dNumuCCAnalysis->DefineTruthTree();

  //--- muon variables -------
}

//********************************************************************
void p0dNumuCCQEAnalysis::FillMicroTrees(bool addBase){
  //********************************************************************

  // Fill variables from p0dNumuCCAnalysis
  _p0dNumuCCAnalysis->FillMicroTrees(addBase);
}

//********************************************************************
void p0dNumuCCQEAnalysis::FillToyVarsInMicroTrees(bool addBase){
  //********************************************************************

  // Variables from the p0dNumuCCAnalysis analysis (including the ones in baseTrackerAnalysis by default, otherwise addBase should be false
  _p0dNumuCCAnalysis->FillToyVarsInMicroTrees(addBase);
}

//********************************************************************
bool p0dNumuCCQEAnalysis::CheckFillTruthTree(const AnaTrueVertex& vtx){
//********************************************************************

  if (_saveAllNumuCC) return _p0dNumuCCAnalysis->CheckFillTruthTree(vtx);
  else return (_p0dNumuCCAnalysis->CheckFillTruthTree(vtx) &&
               anaUtils::GetTopology(vtx, SubDetId::kP0D) == 0);
}

//********************************************************************
void p0dNumuCCQEAnalysis::FillTruthTree(const AnaTrueVertex& vtx){
//********************************************************************
  _p0dNumuCCAnalysis->FillTruthTree(vtx);
}

//********************************************************************
void p0dNumuCCQEAnalysis::FillCategories(){
//********************************************************************
  _p0dNumuCCAnalysis->FillCategories();
}
