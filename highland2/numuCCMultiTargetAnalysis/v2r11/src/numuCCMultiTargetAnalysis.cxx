#include "numuCCMultiTargetAnalysis.hxx"
#include "numuCCMultiTargetUtils.hxx"
#include "Parameters.hxx"
#include "CategoriesUtils.hxx"

bool _doMultiPi = false;
bool _IsAntinu  = false;

//********************************************************************
numuCCMultiTargetAnalysis::numuCCMultiTargetAnalysis(AnalysisAlgorithm* ana) : baseTrackerAnalysis(ana){
//********************************************************************

  // Add the package version
  ND::versioning().AddPackage("numuCCMultiTargetAnalysis", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("NUMUCCMULTITARGETANALYSISROOT")));
}

//********************************************************************
bool numuCCMultiTargetAnalysis::Initialize(){
//********************************************************************
  
  // CC or CCMultiPi analysis?
  _doMultiPi = ND::params().GetParameterI("numuCCMultiTargetAnalysis.DoMultiPi");

  // Antineutrino or neutrino analysis?
  _IsAntinu = ND::params().GetParameterI("numuCCMultiTargetAnalysis.DoAntiNu");

  // Add new categories (before initializing numuCC, so AddStandardCategories will add no truth and sand types automatically)
  numuccmultitarget_utils::AddCategories();

  // Use reduced FV (needed before numuCCAnalysis->Initialize which calls GetNTargets)
  if (ND::params().GetParameterI("numuCCMultiTargetAnalysis.UseReducedFV")) {
    // make FGD1 and FGD2 FV identical, use smaller FV
    double FVXmin = TMath::Max(FVDef::FVdefminFGD1[0], FVDef::FVdefminFGD2[0]);
    double FVXmax = TMath::Max(FVDef::FVdefmaxFGD1[0], FVDef::FVdefmaxFGD2[0]);
    double FVYmin = TMath::Max(FVDef::FVdefminFGD1[1], FVDef::FVdefminFGD2[1]);
    double FVYmax = TMath::Max(FVDef::FVdefmaxFGD1[1], FVDef::FVdefmaxFGD2[1]);
    double FVZmin = TMath::Max(FVDef::FVdefminFGD1[2], FVDef::FVdefminFGD2[2]);
    double FVZmax = TMath::Max(FVDef::FVdefmaxFGD1[2], FVDef::FVdefmaxFGD2[2]);
    FVDef::FVdefminFGD1[0] = FVXmin; FVDef::FVdefminFGD1[1] = FVYmin; FVDef::FVdefminFGD1[2] = FVZmin;
    FVDef::FVdefmaxFGD1[0] = FVXmax; FVDef::FVdefmaxFGD1[1] = FVYmax; FVDef::FVdefmaxFGD1[2] = FVZmax;
    FVDef::FVdefminFGD2[0] = FVXmin; FVDef::FVdefminFGD2[1] = FVYmin; FVDef::FVdefminFGD2[2] = FVZmin;
    FVDef::FVdefmaxFGD2[0] = FVXmax; FVDef::FVdefmaxFGD2[1] = FVYmax; FVDef::FVdefmaxFGD2[2] = FVZmax;
  }
    // cout new values
    std::cout << "Using a reduced Fiducial Volume: " << std::endl;
    std::cout << "   DetDef::fgd1min = " << DetDef::fgd1min[0] << " " << DetDef::fgd1min[1] << " " << DetDef::fgd1min[2] << std::endl;
    std::cout << "   DetDef::fgd1max =  " << DetDef::fgd1max[0] << " " << DetDef::fgd1max[1] << " " << DetDef::fgd1max[2] << std::endl;
    std::cout << "   DetDef::fgd2min =  " << DetDef::fgd2min[0] << " " << DetDef::fgd2min[1] << " " << DetDef::fgd2min[2] << std::endl;
    std::cout << "   DetDef::fgd2max =  " << DetDef::fgd2max[0] << " " << DetDef::fgd2max[1] << " " << DetDef::fgd2max[2] << std::endl;
    std::cout << "   FVDef::FVdefminFGD1 = " << FVDef::FVdefminFGD1[0] << " " << FVDef::FVdefminFGD1[1] << " " << FVDef::FVdefminFGD1[2] << std::endl;
    std::cout << "   FVDef::FVdefmaxFGD1 = " << FVDef::FVdefmaxFGD1[0] << " " << FVDef::FVdefmaxFGD1[1] << " " << FVDef::FVdefmaxFGD1[2] << std::endl;
    std::cout << "   FVDef::FVdefminFGD2 = " << FVDef::FVdefminFGD2[0] << " " << FVDef::FVdefminFGD2[1] << " " << FVDef::FVdefminFGD2[2] << std::endl;
    std::cout << "   FVDef::FVdefmaxFGD2 = " << FVDef::FVdefmaxFGD2[0] << " " << FVDef::FVdefmaxFGD2[1] << " " << FVDef::FVdefmaxFGD2[2] << std::endl;

  // Create a numuCCAnalysis passing this analysis to the constructor. In that way the same managers are used
  // UseAnalysis will make using the same box and event
  // By default configurations, systematics, corrections and tree variables of the used algorithm will be added

  // use antiNu
  if (_IsAntinu) {
    std::cout << "Processing antiNumuCC selection with numuCCMultiTargetAnalysis package" << std::endl;
    _antiNumuCCAnalysis = new antiNumuCCAnalysis(this);
    UseAnalysis(_antiNumuCCAnalysis);
    if (!_antiNumuCCAnalysis->Initialize()) return false;
  }

  // use numuCCMultPi
  else if (_doMultiPi) {
    std::cout << "Processing numuCCMultiPi selection with numuCCMultiTargetAnalysis package" << std::endl;
    _numuCCMultiPiAnalysis = new numuCCMultiPiAnalysis(this);
    UseAnalysis(_numuCCMultiPiAnalysis);
    if (!_numuCCMultiPiAnalysis->Initialize()) return false;
  }

  // use numuCC
  else {
    std::cout << "Processing numuCC selection with numuCCMultiTargetAnalysis package" << std::endl;
    _numuCCAnalysis = new numuCCAnalysis(this);
    UseAnalysis(_numuCCAnalysis);
    if (!_numuCCAnalysis->Initialize()) return false;
  }

  std::cout << "----------------------------------------------------" << std::endl;
  std::cout << "WARNING: only for events with accum_level > 5 the vars in the output microtree will surely refer to the muon candidate in that FGD" << std::endl;
  std::cout << "----------------------------------------------------" << std::endl;

  // Minimum accum level to save event into the output tree
  SetMinAccumCutLevelToSave(ND::params().GetParameterI("numuCCMultiTargetAnalysis.MinAccumLevelToSave"));

  // Initialize the baseTrackerAnalysis
  if(!baseTrackerAnalysis::Initialize()) return false;

  
  return true;
}

//********************************************************************
void numuCCMultiTargetAnalysis::DefineSelections(){
//********************************************************************

  // Add the selection to the Selection Manager
  if(_IsAntinu)
    sel().AddSelection("kTrackerAntiNumuCCMultiTarget", "antiNumuCC multi-target selection", new antiNumuCCFGD2Selection(false));
  else
    sel().AddSelection("kTrackerNumuCCMultiTarget",     "numuCC multi-target selection",     new numuCCMultiTargetSelection(_doMultiPi));
}

//********************************************************************
void numuCCMultiTargetAnalysis::DefineCorrections(){
//********************************************************************

  if (_IsAntinu)          _antiNumuCCAnalysis->DefineCorrections();
  else if (_doMultiPi) _numuCCMultiPiAnalysis->DefineCorrections();
  else                        _numuCCAnalysis->DefineCorrections();
}

//********************************************************************
void numuCCMultiTargetAnalysis::DefineMicroTrees(bool addBase){
//********************************************************************

  // -------- Add variables to the analysis tree ----------------------

  // Variables from the numuCCAnalysis analysis (including those in baseTrackerAnalysis by default, otherwise addBase should be false
  if (_IsAntinu)          _antiNumuCCAnalysis->DefineMicroTrees(addBase);
  else if (_doMultiPi) _numuCCMultiPiAnalysis->DefineMicroTrees(addBase);
  else                        _numuCCAnalysis->DefineMicroTrees(addBase);

  // --- Add here more variables for the numuCCMultiTarget analysis -----

  AddVarI(output(), ntracks, "");

  // selmu vars
  AddVarI(output(),   selmu_fgdmoduletype,   "");
  AddVarI(output(),   selmu_fgdlayer,        "");
  AddVar3VF(output(), selmu_hit1_pos,        "");
  AddVar3VF(output(), selmu_hit2_pos,        "");
  AddVarF(output(),   selmu_hit1_charge,     "");
  AddVarF(output(),   selmu_hit2_charge,     "");
  AddVarI(output(),   selmu_hit1_fgdmoduletype,   "");

  // truelepton vars
  AddVarI(output(),   truelepton_fgdmoduletype , "");
  AddVarI(output(),   truelepton_fgdlayer,       "");
  AddVarI(output(),   truelepton_targetZ,        "");
  AddVarI(output(),   truelepton_targetPDG,      "");

  // Extra variables used for backward migrations systematics study
  AddVarF(output(),   distance_track_hit1,        "");
  AddVarF(output(),   distance_track_hit2,        "");
  AddVar3VF(output(), selmu_fittrack_hit1_pos,    "");
  AddVar3VF(output(), selmu_fittrack_hit2_pos,    "");
  AddVarF(output(),   selmu_deltachi2_hit1,       "");
  AddVarF(output(),   selmu_deltachi2_hit2,       "");

  // Extra variables for reconstructed energy
  AddVarF(output(),   selmu_nuErecQEoxygen,      "neutrino reconstructed energy with muon candidate's reconstructed kinematics in ccqe formula with Eb for oxygen");
  AddVarF(output(),   truelepton_nuErecQEoxygen, "neutrino reconstructed energy with true lepton kinematics in ccqe formula with Eb for oxygen");
  AddVarF(output(),   selmu_chi2,            ""); // Chi2 of the muon candidate track

}

//********************************************************************
void numuCCMultiTargetAnalysis::DefineTruthTree(){
//********************************************************************

  // Variables from baseTrackerAnalysis
  if (_IsAntinu)          _antiNumuCCAnalysis->DefineTruthTree();
  else if (_doMultiPi) _numuCCMultiPiAnalysis->DefineTruthTree();
  else                        _numuCCAnalysis->DefineTruthTree();

  AddVarI(output(), truevtx_fgdmoduletype , "");
  AddVarI(output(), truevtx_fgdlayer,  "");
  AddVarI(output(), truevtx_targetZ,   "");
  AddVarI(output(), truevtx_targetPDG, "");
}

//********************************************************************
void numuCCMultiTargetAnalysis::FillMicroTrees(bool addBase){
//********************************************************************

  // Variables from the numuCCAnalysis analysis (including the ones in baseTrackerAnalysis by default, otherwise addBase should be false)
  if (_IsAntinu)          _antiNumuCCAnalysis->FillMicroTrees(addBase);
  else if (_doMultiPi) _numuCCMultiPiAnalysis->FillMicroTrees(addBase);
  else                        _numuCCAnalysis->FillMicroTrees(addBase);

  // --- Fill here more variables for the numuCCMultiTarget analysis -----

  output().FillVar(ntracks, (int)GetBunch().Particles.size());

  if( ! box().MainTrack) return;

  AnaTrack* selmu = static_cast<AnaTrack*>(box().MainTrack);

  output().FillVar(selmu_fgdmoduletype, anaUtils::GetFgdModuleType(GetEvent().GetIsMC(), selmu->PositionStart, SubDetId::kFGD2));
  output().FillVar(selmu_fgdlayer,      numuccmultitarget_utils::GetFgdLayer(GetEvent().GetIsMC(), selmu->PositionStart));

  if (selmu->GetTrueParticle() && selmu->GetTrueParticle()->TrueVertex) { // sand muons might not have the true vertex
    AnaTrueVertex* truevtx = static_cast<AnaTrueVertex*>(selmu->GetTrueParticle()->TrueVertex);
    output().FillVar(truelepton_fgdmoduletype, anaUtils::GetFgdModuleType(GetEvent().GetIsMC(), truevtx->Position, SubDetId::kFGD2));
    output().FillVar(truelepton_fgdlayer,      numuccmultitarget_utils::GetFgdLayer(GetEvent().GetIsMC(), truevtx->Position));
    output().FillVar(truelepton_targetZ,       anaUtils::GetTargetCode(truevtx));
    output().FillVar(truelepton_targetPDG,     truevtx->TargetPDG);
    // Reconstructed neutrino energy (muon kinematics only)
    Float_t Erec = anaUtils::ComputeRecNuEnergyCCQE(output().GetVarValueF(numuCCAnalysis::truelepton_mom), units::mass_muon, output().GetVarValueF(numuCCAnalysis::truelepton_costheta), 27.);
    output().FillVar(truelepton_nuErecQEoxygen, Erec);
  }

  // Muon candidate's first and second hit study

  // if flattrees, these vars are not saved
  if ( ! input().InputIsFlatTree()) {

    // for forward tracks get the upstream hits
    Float_t hit1_pos[3];
    Float_t hit2_pos[3];
    if (selmu->PositionStart[2] <= selmu->PositionEnd[2]) {
      for (int i=0; i<3; i++) hit1_pos[i] = selmu->UpstreamHits_Position[0][i];
      for (int i=0; i<3; i++) hit2_pos[i] = selmu->UpstreamHits_Position[1][i];
      output().FillVar(selmu_hit1_charge, selmu->UpstreamHits_Charge[0]);
      output().FillVar(selmu_hit2_charge, selmu->UpstreamHits_Charge[1]);
    } else {
      // for backward tracks get the downstream hits
      for (int i=0; i<3; i++) hit1_pos[i] = selmu->DownstreamHits_Position[0][i];
      for (int i=0; i<3; i++) hit2_pos[i] = selmu->DownstreamHits_Position[1][i];
      output().FillVar(selmu_hit1_charge, selmu->DownstreamHits_Charge[0]);
      output().FillVar(selmu_hit2_charge, selmu->DownstreamHits_Charge[1]);
    }

    output().FillVectorVarFromArray(selmu_hit1_pos, hit1_pos, 3);
    output().FillVectorVarFromArray(selmu_hit2_pos, hit2_pos, 3);
    output().FillVar(selmu_hit1_fgdmoduletype, anaUtils::GetFgdModuleType(GetEvent().GetIsMC(), hit1_pos, SubDetId::kFGD2));

    // variables for backward migration studies
    TVector3 fittedPos1 = numuccmultitarget_utils::GetFittedPos(anaUtils::ArrayToTVector3(selmu->PositionStart),
                                                                anaUtils::ArrayToTVector3(selmu->PositionEnd),
                                                                hit1_pos[2] );
    TVector3 fittedPos2 = numuccmultitarget_utils::GetFittedPos(anaUtils::ArrayToTVector3(selmu->PositionStart),
                                                                anaUtils::ArrayToTVector3(selmu->PositionEnd),
                                                                hit2_pos[2] );
    Float_t ft1pos[3]; anaUtils::VectorToArray(fittedPos1,ft1pos);
    Float_t ft2pos[3]; anaUtils::VectorToArray(fittedPos2,ft2pos);
    output().FillVectorVarFromArray(selmu_fittrack_hit1_pos, ft1pos, 3);
    output().FillVectorVarFromArray(selmu_fittrack_hit2_pos, ft2pos, 3);
    output().FillVar(selmu_deltachi2_hit1, numuccmultitarget_utils::GetDeltaChi2(hit1_pos,fittedPos1));
    output().FillVar(selmu_deltachi2_hit2, numuccmultitarget_utils::GetDeltaChi2(hit2_pos,fittedPos2));

    TVector3 track_start_pos = anaUtils::ArrayToTVector3(selmu->PositionStart);
    TVector3 track_end_pos = anaUtils::ArrayToTVector3(selmu->PositionEnd);
    output().FillVar(distance_track_hit1, numuccmultitarget_utils::CalculateLineLineDistance(track_start_pos, track_end_pos, hit1_pos));
    output().FillVar(distance_track_hit2, numuccmultitarget_utils::CalculateLineLineDistance(track_start_pos, track_end_pos, hit2_pos));

    // the second hit might be outside the FGD, but the first shouldn't
    if (selmu_hit1_charge == -999) std::cout << "ERROR: selmu_hit1_charge is -999" << std::endl;

  } // end if ! FlatTrees

  // Reconstructed neutrino energy (muon kinematics only)
  Float_t Erec = anaUtils::ComputeRecNuEnergyCCQE(output().GetVarValueF(numuCCAnalysis::selmu_mom), units::mass_muon, output().GetVarValueF(numuCCAnalysis::selmu_costheta), 27.);
  output().FillVar(selmu_nuErecQEoxygen, Erec);

  output().FillVar(selmu_chi2, selmu->Chi2);
}

//********************************************************************
void numuCCMultiTargetAnalysis::FillToyVarsInMicroTrees(bool addBase){
//********************************************************************

  // Variables from the numuCCAnalysis analysis (including the ones in baseTrackerAnalysis by default, otherwise addBase should be false)
  if (_IsAntinu)          _antiNumuCCAnalysis->FillToyVarsInMicroTrees(addBase);
  else if (_doMultiPi) _numuCCMultiPiAnalysis->FillToyVarsInMicroTrees(addBase);
  else                        _numuCCAnalysis->FillToyVarsInMicroTrees(addBase);

  // Fill here variables specific for the numuCCMultiTarget analysis
}

//********************************************************************
bool numuCCMultiTargetAnalysis::CheckFillTruthTree(const AnaTrueVertex& vtx){
//********************************************************************
  // do not just call the methods in numuCCAnalysis so to avoid to set _whichFGD parameter
  // GetReactionCC get also 2p2h interactions
  bool numuCCinFV = anaUtils::GetReactionCC(vtx, SubDetId::kFGD, _IsAntinu)==1;
  // Since our topology definition doesn't consider the reaction code
  // and since a muon pair can be created in the FSI (likely in DIS)
  // in principle we might have a non-CC vertex categorized as CCother
  // (nevertheless I didn't find any.)
  int topo = anaUtils::GetTopology(vtx, SubDetId::kFGD, _IsAntinu);
  bool topoCCinFV = (topo == 0 || topo == 1 || topo == 2);

  if ( ! numuCCinFV && topoCCinFV) std::cout << "WOW here you have a topoCC non-numuCC! TruthVertex ID: " << vtx.ID << std::endl;

  return (numuCCinFV || topoCCinFV);
}

//********************************************************************
void numuCCMultiTargetAnalysis::FillTruthTree(const AnaTrueVertex& vtx){
//********************************************************************

  // Fill the common variables
  if (_IsAntinu)          _antiNumuCCAnalysis->FillTruthTree(vtx);
  else if (_doMultiPi) _numuCCMultiPiAnalysis->FillTruthTree(vtx);
  else                        _numuCCAnalysis->FillTruthTree(vtx);

  output().FillVar(truevtx_fgdmoduletype, anaUtils::GetFgdModuleType(GetSpill().GetIsMC(), vtx.Position, SubDetId::kFGD2));
  output().FillVar(truevtx_fgdlayer,      numuccmultitarget_utils::GetFgdLayer(GetSpill().GetIsMC(), vtx.Position));
  output().FillVar(truevtx_targetZ,       anaUtils::GetTargetCode(&vtx));
  output().FillVar(truevtx_targetPDG,     vtx.TargetPDG);

  // Fill the extra categories in truth tree

  if (GetSpill().GetIsSandMC()) {
    cat().SetCode("fgd2moduletype",   CATSAND);
    cat().SetCode("fgd2moduletypeCC", CATSAND);
    cat().SetCode("fgd2targetCCQE",   CATSAND);
    cat().SetCode("fgd2targetCC",     CATSAND);
    return;
  }

  cat().SetCode("fgd2moduletype",   anaUtils::GetFgdModuleType(GetSpill().GetIsMC(), vtx.Position, SubDetId::kFGD2));
  cat().SetCode("fgd2moduletypeCC", numuccmultitarget_utils::GetFgdModuleTypeCC(GetSpill().GetIsMC(), &vtx, SubDetId::kFGD2), _IsAntinu);
  cat().SetCode("fgd2targetCCQE",   numuccmultitarget_utils::GetTargetCCQE(&vtx, SubDetId::kFGD2));
  cat().SetCode("fgd2targetCC",     numuccmultitarget_utils::GetTargetCC(&vtx, SubDetId::kFGD2));
}

//********************************************************************
void numuCCMultiTargetAnalysis::FillCategories(){
//********************************************************************

  // Fill the track categories for color drawing
  if (_IsAntinu)          _antiNumuCCAnalysis->FillCategories();
  else if (_doMultiPi) _numuCCMultiPiAnalysis->FillCategories();
  else                        _numuCCAnalysis->FillCategories();

  // Fill the extra categories in default tree

  if (GetSpill().GetIsSandMC()) {
    cat().SetCode("fgd2moduletype",   CATSAND);
    cat().SetCode("fgd2moduletypeCC", CATSAND);
    cat().SetCode("fgd2targetCCQE",   CATSAND);
    cat().SetCode("fgd2targetCC",     CATSAND);
    return;
  }

  cat().SetCode("fgd2moduletype",   CATNOTRUTH);
  cat().SetCode("fgd2moduletypeCC", CATNOTRUTH);
  cat().SetCode("fgd2targetCCQE",   CATNOTRUTH);
  cat().SetCode("fgd2targetCC",     CATNOTRUTH);

  if (box().MainTrack && box().MainTrack->GetTrueParticle() && box().MainTrack->GetTrueParticle()->TrueVertex) { // sand muons might not have the true vertex
    AnaTrueVertex* truevtx = static_cast<AnaTrueVertex*>(box().MainTrack->GetTrueParticle()->TrueVertex);
    (void)truevtx;
    cat().SetCode("fgd2moduletype",   anaUtils::GetFgdModuleType(GetEvent().GetIsMC(), truevtx->Position, SubDetId::kFGD2));
    cat().SetCode("fgd2moduletypeCC", numuccmultitarget_utils::GetFgdModuleTypeCC(GetEvent().GetIsMC(), truevtx, SubDetId::kFGD2), _IsAntinu);
    cat().SetCode("fgd2targetCCQE",   numuccmultitarget_utils::GetTargetCCQE(truevtx, SubDetId::kFGD2));
    cat().SetCode("fgd2targetCC",     numuccmultitarget_utils::GetTargetCC(truevtx, SubDetId::kFGD2));
  }

}

