#include "antiNumuCCAnalysis.hxx"
#include "FiducialVolumeDefinition.hxx"
#include "Parameters.hxx"
#include "UseGlobalAltMomCorrection.hxx"
#include "antiNumuCCSelection.hxx"
#include "antiNumuCCMultiTrackSelection.hxx"
#include "antiNumuCCFGD2Selection.hxx"
#include "antiNumuCCMultiTrackFGD2Selection.hxx"
#include "CategoriesUtils.hxx"
#include "BasicUtils.hxx"

//********************************************************************
antiNumuCCAnalysis::antiNumuCCAnalysis(AnalysisAlgorithm* ana) : baseTrackerAnalysis(ana) {
//********************************************************************

  // Add the package version
  ND::versioning().AddPackage("antiNumuCCAnalysis", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("ANTINUMUCCANALYSISROOT")));

  // Create a numuCCAnalysis passing this analysis to the constructor. In that way the same managers are used
  _numuCCAnalysis = new numuCCAnalysis(this);

  // Use the numuCCAnalysis (in practice that means that the same box and event will be used for the numuCCAnalysis as for this analysis)
  UseAnalysis(_numuCCAnalysis);
}

//********************************************************************
bool antiNumuCCAnalysis::Initialize() {
//********************************************************************

  // Initialize the baseAnalysis instead of numuCCAnalysis
  // since we do not want to call and overload numu categories
  if(!baseTrackerAnalysis::Initialize()) return false;

  // Minimum accum level to save event into the output tree
  SetMinAccumCutLevelToSave(ND::params().GetParameterI("antiNumuCCAnalysis.MinAccumLevelToSave"));

  // which analysis: FGD1, FGD2 or FGDs
  _whichFGD = ND::params().GetParameterI("antiNumuCCAnalysis.Selections.whichFGD");
  if (_whichFGD == 3) {
    std::cout << "----------------------------------------------------" << std::endl;
    std::cout << "WARNING: only for events with accum_level > 6 the vars in the output microtree will surely refer to the muon candidate in that FGD" << std::endl;
    std::cout << "----------------------------------------------------" << std::endl;
  }

  // CC-inclusive or CC-MultiPi
  fCCInclusive = ND::params().GetParameterI("antiNumuCCAnalysis.Selections.CCInc");

  // Define antinu categories (different legend from numuCC)
  // for FGD2 same categories with prefix "fgd2", i,e, "fgd2reaction" etc.)
  anaUtils::AddStandardAntiNumuCategories();
  anaUtils::AddStandardAntiNumuCategories("fgd2");

  return true;
}

//********************************************************************
void antiNumuCCAnalysis::DefineSelections(){
//********************************************************************

  // FGD1 antineutrino analysis:
  if(_whichFGD==1 || _whichFGD==3){
    // ---- Inclusive CC ----
    if(fCCInclusive)
      sel().AddSelection("kTrackerAntiNumuCC",            "inclusive antiNumuCC FGD1 selection",     new antiNumuCCSelection(false));
    // ---- CC Multi Pion Samples ----
    else
      sel().AddSelection("kTrackerAntiNumuCCMultiTrack",     "antiNumu FGD1 CC Multiple Track selection", new antiNumuCCMultiTrackSelection(false));
  }
  // FGD2 antineutrino analysis:
  if (_whichFGD==2 || _whichFGD==3){
    // ---- CC Inclusive ----
    if(fCCInclusive)
      sel().AddSelection("kTrackerAntiNumuCCFGD2",        "Inclusive antiNumu FGD2 CC selection",       new antiNumuCCFGD2Selection(false));
    // ---- CC Multi Track Samples ----
    else
      sel().AddSelection("kTrackerAntiNumuMultiTrackFGD2",   "antiNumu FGD2 CC Multiple Track selection",  new antiNumuCCMultiTrackFGD2Selection(false));
  }
}

//********************************************************************
void antiNumuCCAnalysis::DefineMicroTrees(bool addBase){
//********************************************************************

  // -------- Add variables to the analysis tree ----------------------

  // Variables from numuCCAnalysis
  if (addBase) _numuCCAnalysis->DefineMicroTrees(addBase);

  if (ND::params().GetParameterI("numuCCAnalysis.MicroTrees.AdditionalToyVars")){
    AddToyVarF(output(), shmp_mom, "second highest momentum positive track reconstructed momentum");
  }
  else{
    AddVarF(output(), shmp_mom, "second highest momentum positive track reconstructed momentum");
  }

}

//********************************************************************
void antiNumuCCAnalysis::DefineTruthTree(){
//********************************************************************

  // Variables from numuCCAnalysis
  _numuCCAnalysis->DefineTruthTree();

}

//********************************************************************
void antiNumuCCAnalysis::FillMicroTrees(bool addBase){
//********************************************************************

  // Variables from numuCCAnalysis
  if (addBase) _numuCCAnalysis->FillMicroTrees(addBase);

  if (!ND::params().GetParameterI("numuCCAnalysis.MicroTrees.AdditionalToyVars")){
    if (box().SHMPtrack)
      output().FillVar(shmp_mom, box().SHMPtrack->Momentum);
  }
  
}

//********************************************************************
void antiNumuCCAnalysis::FillToyVarsInMicroTrees(bool addBase){
//********************************************************************

  // Fill all tree variables that vary for each virtual analysis (toy experiment)

  // Variables from numuCCAnalysis
  if (addBase) _numuCCAnalysis->FillToyVarsInMicroTrees(addBase);

  if (ND::params().GetParameterI("numuCCAnalysis.MicroTrees.AdditionalToyVars")){
    if (box().SHMPtrack){
      output().FillToyVar(shmp_mom, box().SHMPtrack->Momentum);
    }
  }
}

//********************************************************************
bool antiNumuCCAnalysis::CheckFillTruthTree(const AnaTrueVertex& vtx){
//********************************************************************

  // we cannot call the numuCC method here because 1) the setting of _whichFGD would be lost
  // and 2) the called methods need the IsAntinu paramenter set to true

  SubDetId::SubDetEnum fgdID;
  if (_whichFGD == 1) fgdID = SubDetId::kFGD1;
  if (_whichFGD == 2) fgdID = SubDetId::kFGD2;
  if (_whichFGD == 3) fgdID = SubDetId::kFGD;

  bool IsAntinu = true;

  // GetReactionCC already takes into account the outFV and also
  // the NuWro reaction code for 2p2h in prod5 (that is 70)
  bool antiNumuCCinFV = (anaUtils::GetReactionCC(vtx, fgdID, IsAntinu) == 1);

  // Since our topology definition doesn't consider the reaction code
  // and since a muon pair can be created in the FSI (likely in DIS)
  // in principle we might have a non-CC vertex categorized as CCother
  // (nevertheless I didn't find any)
  int topo = anaUtils::GetTopology(vtx, fgdID, IsAntinu);
  bool topoCCinFV = (topo == 0 || topo == 1 || topo == 2);

  return (antiNumuCCinFV || topoCCinFV);
}

//********************************************************************
void antiNumuCCAnalysis::FillTruthTree(const AnaTrueVertex& vtx){
//********************************************************************

  // Variables from numuCCAnalysis, with antinu categories
  bool IsAntinu = true;
  _numuCCAnalysis->FillTruthTreeBase(vtx,IsAntinu);

}

//********************************************************************
void antiNumuCCAnalysis::FillCategories(){
//********************************************************************

  // Fill the track categories for color drawing

  bool IsAntinu = true;

  // For the muon candidate
  anaUtils::FillCategories(&GetEvent(), static_cast<AnaTrack*>(box().MainTrack),"",SubDetId::kFGD1,IsAntinu);

  // For FGD2, same categories with prefix "fgd2", i,e, "fgd2reaction" etc.
  // We could directly set which FGD in the usual categories, but this cannot be done for the true vertices
  anaUtils::FillCategories(&GetEvent(), static_cast<AnaTrack*>(box().MainTrack), "fgd2", SubDetId::kFGD2,IsAntinu);

}

