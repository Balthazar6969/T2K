#include "numuCCAnalysis.hxx"
#include "FiducialVolumeDefinition.hxx"
#include "Parameters.hxx"
#include "UseGlobalAltMomCorrection.hxx"
#include "numuCCSelection.hxx"
#include "numuCCFGD2Selection.hxx"
#include "CategoriesUtils.hxx"
#include "BasicUtils.hxx"

#include "NuDirUtils.hxx"
#include "TreeConverterUtils.hxx"
#include "ConfigTreeTools.hxx"

//********************************************************************
numuCCAnalysis::numuCCAnalysis(AnalysisAlgorithm* ana) : baseTrackerAnalysis(ana) {
  //********************************************************************

  // Add the package version
  ND::versioning().AddPackage("numuCCAnalysis", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("NUMUCCANALYSISROOT")));
}

//********************************************************************
bool numuCCAnalysis::Initialize(){
  //********************************************************************

  // Initialize the baseTrackerAnalysis
  if(!baseTrackerAnalysis::Initialize()) return false;

  // Minimum accum level to save event into the output tree
  SetMinAccumCutLevelToSave(ND::params().GetParameterI("numuCCAnalysis.MinAccumLevelToSave"));

  // which analysis: FGD1, FGD2 or FGDs
  _whichFGD = ND::params().GetParameterI("numuCCAnalysis.Selections.whichFGD");
  if (_whichFGD == 3) {
    std::cout << "----------------------------------------------------" << std::endl;
    std::cout << "WARNING: only for events with accum_level > 5 the vars in the output microtree will surely refer to the muon candidate in that FGD" << std::endl;
    std::cout << "----------------------------------------------------" << std::endl;
  }

  // Use alternative FV
  if (ND::params().GetParameterI("numuCCAnalysis.Selections.UseAlternativeFV")) SetFV();

  // store mass weights in a single entry tree
  _saveMassWeights = ND::params().GetParameterI("numuCCAnalysis.MicroTrees.SaveMassWeights");

  // Define categories
  // for FGD2 same categories with prefix "fgd2", i,e, "fgd2reaction" etc.)
  anaUtils::AddStandardCategories();
  anaUtils::AddStandardCategories("fgd2");

  return true;
}

//********************************************************************
void numuCCAnalysis::DefineSelections(){
  //********************************************************************

  // ----- Inclusive CC -----------
  if      (_whichFGD==1) // FGD1
    sel().AddSelection("kTrackerNumuCC",     "inclusive numuCC selection",     new numuCCSelection(false));     // true/false for forcing break
  else if (_whichFGD==2) // FGD2
    sel().AddSelection("kTrackerNumuCCFGD2", "inclusive numuCCFGD2 selection", new numuCCFGD2Selection(false)); // true/false for forcing break
  else if (_whichFGD==3) { // both FGDs, in 2 selections
    sel().AddSelection("kTrackerNumuCC",     "inclusive numuCC selection",     new numuCCSelection(false));     // true/false for forcing break
    sel().AddSelection("kTrackerNumuCCFGD2", "inclusive numuCCFGD2 selection", new numuCCFGD2Selection(false)); // true/false for forcing break
  }
}

//********************************************************************
void numuCCAnalysis::DefineCorrections(){
  //********************************************************************

  // Some corrections are defined in baseTrackerAnalysis
  baseTrackerAnalysis::DefineCorrections();

  //------------ Corrections -----------

  // Change the main fit momentum by the muon alternate momentum, but only for P5
#if !VERSION_HAS_EQUIVALENT_MAIN_AND_ALT_FITS
  if (ND::params().GetParameterI("numuCCAnalysis.Corrections.EnableUseMuonAltMom") == 1) {
    corr().AddCorrection("altmom_corr", new UseGlobalAltMomCorrection(UseGlobalAltMomCorrection::kMuon));
  }
#endif

}

//********************************************************************
void numuCCAnalysis::Finalize(){
  //********************************************************************

  // Fill the single entry in the massWeightTree once all events have been processed
  if (_saveMassWeights)
    output().GetTree(massWeightTree)->Fill();
}

//********************************************************************
void numuCCAnalysis::DefineMicroTrees(bool addBase){
  //********************************************************************

  // -------- Add variables to the analysis tree ----------------------

  // Variables from baseTrackerAnalysis (run, event, ...)
  if (addBase) baseTrackerAnalysis::DefineMicroTrees(addBase);

  // --- muon candidate truth variables ---
  AddVarF(  output(), selmu_truemom,    "muon candidate true momentum");
  AddVar4VF(output(), selmu_truepos,    "muon candidate true position");
  AddVar4VF(output(), selmu_trueendpos, "muon candidate true end position");
  AddVar3VF(output(), selmu_truedir,    "muon candidate true direction");

  // --- true lepton truth variables ---
  AddVarI(output(),   truelepton_pdg,       "true lepton PDG");
  AddVarF(output(),   truelepton_mom,       "true lepton true momentum");
  AddVarF(output(),   truelepton_costheta,  "true lepton true cos(theta) w.r.t. neutrino direction");
  //  AddVarI(output(),   truelepton_det,       "detector enum of the true lepton start position");   // equivalent to detector but in different detector convention

  // --- muon candidate recon variables ---
  AddVar3VF(output(), selmu_dir,        "muon candidate reconstructed direction");
  AddVar3VF(output(), selmu_enddir,     "muon candidate reconstructed end direction");
  AddVar4VF(output(), selmu_pos,        "muon candidate reconstructed position");
  AddVar4VF(output(), selmu_endpos,     "muon candidate reconstructed end position");

  AddVarI(  output(), selmu_closest_tpc,"muon candidate closest TPC");
  AddVarI(  output(), selmu_detectors,  "muon candidate detectors");
  AddVarI(  output(), selmu_det,        "detector enum of the muon candidate start position");

  AddToyVarF(output(), selmu_mom,      "muon candidate reconstructed momentum");
  AddToyVarF(output(), selmu_costheta, "muon candidate reconstructed cos(theta). w.r.t. to neutrino direction");

  // --- reconstructed neutrino energy ----
  // (true neutrino energy is already saved in basAnalysis as nu_trueE)
  AddToyVarF(output(), selmu_nuErecQE,   "neutrino reconstructed energy with muon candidate's reconstructed kinematics in ccqe formula");
  AddVarF(output(), truelepton_nuErecQE, "neutrino reconstructed energy with true lepton kinematics in CCQE formula");

  if (ND::params().GetParameterI("numuCCAnalysis.MicroTrees.AdditionalToyVars")){
    AddToyVarF(output(), selmu_amom,     "muon candidate alternate reconstructed momentum ");
    AddToyVarF(output(), selmu_likemu,   "muon candidate muon TPC PID likelihood");
    AddToyVarF(output(), selmu_likemip,  "muon candidate MIP TPC PID likelihood");
    AddToyVarF(output(), shmn_mom,       "second highest momentum negative track reconstructed momentum ");
  }
  else{
    AddVarF(output(), selmu_amom,     "muon candidate alternate reconstructed momentum ");
    AddVarF(output(), selmu_likemu,   "muon candidate muon TPC PID likelihood");
    AddVarF(output(), selmu_likemip,  "muon candidate MIP TPC PID likelihood");
    AddVarF(output(), shmn_mom,       "second highest momentum negative track reconstructed momentum ");
  }

  // --- info by tpc
  AddVarVI(output(), selmu_tpc_det,         "muon candidate TPC number",                               selmu_ntpcs);
  AddVarVI(output(), selmu_tpc_nhits,       "muon candidate #hits in each TPC",                        selmu_ntpcs);
  AddVarVI(output(), selmu_tpc_nnodes,      "muon candidate #nodes in each TPC",                       selmu_ntpcs);
  AddVarVF(output(), selmu_tpc_charge,      "muon candidate reconstructed charge in each TPC",         selmu_ntpcs);
  AddVarVF(output(), selmu_tpc_mom,         "muon candidate reconstructed momentum in each TPC",       selmu_ntpcs);
  AddVarVF(output(), selmu_tpc_bfield_mom,  "muon candidate reconstructed momentum with BField refit in each TPC",       selmu_ntpcs);
  AddVarVF(output(), selmu_tpc_efield_mom,  "muon candidate reconstructed momentum with EFiled refit in each TPC",       selmu_ntpcs);
  AddVarVF(output(), selmu_tpc_emom,        "muon candidate reconstructed momentum error in each TPC", selmu_ntpcs);
  AddVarVF(output(), selmu_tpc_truemom,     "muon candidate true momentum in each TPC",                selmu_ntpcs);
  AddVarVF(output(), selmu_tpc_dedx_raw,    "muon candidate raw dEdx (CT) in each TPC",                selmu_ntpcs);
  AddVarVF(output(), selmu_tpc_dedx_expmu,  "muon candidate expected dEdx for muons in each TPC",      selmu_ntpcs);
  AddVarVF(output(), selmu_tpc_dedx_exppi,  "muon candidate expected dEdx for pions in each TPC",      selmu_ntpcs);
  AddVarVF(output(), selmu_tpc_dedx_expele, "muon candidate expected dEdx for electrons in each TPC",  selmu_ntpcs);
  AddVarVF(output(), selmu_tpc_dedx_expp,   "muon candidate expected dEdx for protons in each TPC",    selmu_ntpcs);

  if (ND::params().GetParameterI("numuCCAnalysis.MicroTrees.AdditionalToyVars")){
    AddToyVarVF(output(), selmu_tpc_pullmu,  "muon candidate muon pull in each TPC",     NMAXTPCS);
    AddToyVarVF(output(), selmu_tpc_pullpi,  "muon candidate pion pull in each TPC",     NMAXTPCS);
    AddToyVarVF(output(), selmu_tpc_pullele, "muon candidate electron pull in each TPC", NMAXTPCS);
    AddToyVarVF(output(), selmu_tpc_pullp,   "muon candidate proton pull in each TPC",   NMAXTPCS);
    AddToyVarVF(output(), selmu_tpc_dedx,    "muon candidate dEdx (CT) in each TPC",     NMAXTPCS);
  }
  else{
    AddVarVF(output(), selmu_tpc_pullmu,  "muon candidate muon pull in each TPC",     selmu_ntpcs);
    AddVarVF(output(), selmu_tpc_pullpi,  "muon candidate pion pull in each TPC",     selmu_ntpcs);
    AddVarVF(output(), selmu_tpc_pullele, "muon candidate electron pull in each TPC", selmu_ntpcs);
    AddVarVF(output(), selmu_tpc_pullp,   "muon candidate proton pull in each TPC",   selmu_ntpcs);
    AddVarVF(output(), selmu_tpc_dedx,    "muon candidate dEdx (CT) in each TPC",     selmu_ntpcs);
  }

  // --- info by FGD
  AddVarVI(output(), selmu_fgd_det,   "fgd index of muon candidate's fgd segments",      selmu_nfgds);
  AddVarVF(output(), selmu_fgd_x,     "muon candidate track length in each FGD",         selmu_nfgds);
  AddVarVF(output(), selmu_fgd_V11,   "muon candidate V11 vertex activity in each FGD",  selmu_nfgds);
  AddVarVF(output(), selmu_fgd_V33,   "muon candidate V33 vertex activity in each FGD",  selmu_nfgds);
  AddVarVF(output(), selmu_fgd_V55,   "muon candidate V55 vertex activity in each FGD",  selmu_nfgds);
  AddVarVF(output(), selmu_fgd_V77,   "muon candidate V77 vertex activity in each FGD",  selmu_nfgds);
  AddVarVF(output(), selmu_fgd_VLayer,"muon candidate VLayer vertex activity in each FGD",  selmu_nfgds);

  if (ND::params().GetParameterI("numuCCAnalysis.MicroTrees.AdditionalToyVars")){
    AddToyVarVF(output(), selmu_fgd_pullmu,"muon candidate muon pull in each FGD ", NMAXFGDS);
  }
  else{
    AddVarVF(output(), selmu_fgd_pullmu,"muon candidate muon pull in each FGD ", selmu_nfgds);
  }

  // --- info by ECAL
  AddVarVI(output(), selmu_ecal_det,   "ECal sub-module",                                         selmu_necals);
  AddVarVI(output(), selmu_ecal_nhits, "The number of hits in the selected track's ECal segment", selmu_necals);
  AddVarVI(output(), selmu_ecal_nnodes,         "", selmu_necals);
  AddVarVF(output(), selmu_ecal_length,         "", selmu_necals);
  AddVarMF(output(), selmu_ecal_showerstartpos, "", selmu_necals,-30,4);
  AddVarMF(output(), selmu_ecal_showerendpos,   "", selmu_necals,-30,4);
  AddVarMF(output(), selmu_ecal_showerstartdir, "", selmu_necals,-30,3);
  AddVarMF(output(), selmu_ecal_showerenddir,   "", selmu_necals,-30,3);

  AddVarVF(output(), selmu_ecal_EMenergy,     "", selmu_necals);
  AddVarVF(output(), selmu_ecal_edeposit,     "", selmu_necals);
  AddVarVI(output(), selmu_ecal_IsShower,     "", selmu_necals);

  AddVarVF(output(), selmu_ecal_mipem,                "MipEm value of the selected track's ECal segment. Negative means more MIP-like, positive means more EM shower-like",     selmu_necals);
  AddVarVF(output(), selmu_ecal_mippion,              "MipPion value the selected track's ECal segment. Negative means more MIP-like, positive means more showering pion-like", selmu_necals);
  AddVarVF(output(), selmu_ecal_emhip,                "EmHip value of the selected track's ECal segment. Negative means more EM shower-like, positive means more HIP-like.",    selmu_necals);
  AddVarVF(output(), selmu_ecal_containment,          "",                                                                                                                       selmu_necals);
  AddVarVF(output(), selmu_ecal_trshval,              "",                                                                                                                       selmu_necals);
  AddVarVI(output(), selmu_ecal_mostupstreamlayerhit, "",                                                                                                                       selmu_necals);

  // --- info by SMRD
  AddVarVI(output(), selmu_smrd_det,    "", selmu_nsmrds);
  AddVarVI(output(), selmu_smrd_nhits,  "", selmu_nsmrds);
  AddVarVI(output(), selmu_smrd_nnodes, "", selmu_nsmrds);
  AddVarMF(output(), selmu_smrd_dir,    "", selmu_nsmrds, -30, 3);
  AddVarMF(output(), selmu_smrd_enddir, "", selmu_nsmrds, -30, 3);
  AddVarVF(output(), selmu_smrd_edeposit,     "", selmu_nsmrds);

  // Variables to handle the number of targets in xsTool
  AddToyVarI(output(), truevtx_mass_component, "mass component enum related to the true vtx position (FGD1 / FGD2_scint / FGD2_water");

  //------- TEMPORARILY HERE. Add a single entry tree to store the mass weights for each mass component ---------------------

  if (_saveMassWeights){
    // Add the tree
    AddTree(output(),massWeightTree,NULL);

    // Add mass weight for each toy and each target
    output().AddMatrixVar(massWeightTree,massWeight, "massWeight","F",     "mass weight (FGD1, FGD1scint, FGD1water)",100,3);

    // initialize that tree
    output().InitializeTree(massWeightTree, true);
  }
}

//********************************************************************
void numuCCAnalysis::DefineTruthTree(){
  //********************************************************************

  // Variables from baseTrackerAnalysis (run, event, ...)
  baseTrackerAnalysis::DefineTruthTree();

  // Variables to handle the number of targets in xsTool
  AddVarI(output(), truevtx_mass_component, "mass component enum related to the true vtx position (FGD1 / FGD2_scint / FGD2_water");

  // --- Reconstructed neutrino energy ----
  AddVarF(output(), truelepton_nuErecQE, "neutrino reconstructed energy with true lepton kinematics in CCQE formula");

  // --- Lepton candidate kinematics inside sub-detectors crossed --- 
  AddVar4VF(output(), truelepton_tpc1_entrance_pos, "true lepton entrance position in TPC1");
  AddVar4VF(output(), truelepton_tpc1_exit_pos,     "true lepton exit position in TPC1");

  AddVar4VF(output(), truelepton_tpc2_entrance_pos, "true lepton entrance position in TPC2");
  AddVar4VF(output(), truelepton_tpc2_exit_pos,     "true lepton exit position in TPC2");

  AddVar4VF(output(), truelepton_tpc3_entrance_pos, "true lepton entrance position in TPC3");
  AddVar4VF(output(), truelepton_tpc3_exit_pos,     "true lepton exit position in TPC3");


}

//********************************************************************
void numuCCAnalysis::FillMicroTrees(bool addBase){
  //********************************************************************

  // WARNING: IF YOU RUN FGD1 AND FGD2 AT ONCE ( "whichFGD" parameter = 3), only for events with accum_level > 5
  // the vars in the output microtree will surely refer to the muon candidate in that FGD

  // Variables from baseTrackerAnalysis (run, event, ...)
  if (addBase) baseTrackerAnalysis::FillMicroTrees(addBase);

  // Muon candidate variables
  if (box().MainTrack){

    // Properties of the true muon
    if (box().MainTrack->TrueObject) {
      AnaTrueVertex *vtx = static_cast<AnaTrueVertex*>(box().MainTrack->GetTrueParticle()->TrueVertex);
      if (vtx) {
        output().FillVar(truelepton_pdg,      vtx->LeptonPDG);
        output().FillVar(truelepton_mom,      vtx->LeptonMom);
        double costheta_mu_nu = cos(anaUtils::ArrayToTVector3(vtx->LeptonDir).Angle(anaUtils::ArrayToTVector3(vtx->NuDir)));
        output().FillVar(truelepton_costheta, (Float_t)costheta_mu_nu);
        //        output().FillVar(truelepton_det, anaUtils::GetDetector(vtx->Position)); // equivalent to detector but in different detector convention
        Float_t Erec = anaUtils::ComputeRecNuEnergyCCQE(vtx->LeptonMom, units::mass_muon, costheta_mu_nu);
        output().FillVar(truelepton_nuErecQE, Erec);
      } // end if (vtx)
    } // end if (MainTrack->TrueObject)

    output().FillVectorVarFromArray(selmu_pos,    box().MainTrack->PositionStart, 4);
    output().FillVectorVarFromArray(selmu_endpos, box().MainTrack->PositionEnd, 4);
    output().FillVectorVarFromArray(selmu_dir,    box().MainTrack->DirectionStart, 3);
    output().FillVectorVarFromArray(selmu_enddir, box().MainTrack->DirectionEnd, 3);
    output().FillVar(selmu_det,                   anaUtils::GetDetector(box().MainTrack->PositionStart));



    // Additional Toy Vars
    if (!ND::params().GetParameterI("numuCCAnalysis.MicroTrees.AdditionalToyVars")){
      output().FillVar(selmu_amom, (static_cast<AnaTrack*>(box().MainTrack))->MomentumMuon);
      // PID likelihoods
      output().FillVar(selmu_likemu, anaUtils::GetPIDLikelihood( *(box().MainTrack),0));
      output().FillVar(selmu_likemip,anaUtils::GetPIDLikelihoodMIP( *(box().MainTrack)));
    }

    // Properties of the true particle associated to the muon candidate
    if(  box().MainTrack->TrueObject ) {
      output().FillVar(selmu_truemom,          box().MainTrack->GetTrueParticle()->Momentum);
      output().FillVectorVarFromArray(selmu_truepos,   box().MainTrack->GetTrueParticle()->Position, 4);
      output().FillVectorVarFromArray(selmu_trueendpos,box().MainTrack->GetTrueParticle()->PositionEnd, 4);
      output().FillVectorVarFromArray(selmu_truedir,   box().MainTrack->GetTrueParticle()->Direction, 3);
    }

    // The closest TPC
    SubDetId::SubDetEnum tpc = anaUtils::GetClosestTPC(*(box().MainTrack));
    output().FillVar(selmu_closest_tpc, tpc+1);

    // Track composition
    output().FillVar(selmu_detectors, static_cast<AnaTrack*>(box().MainTrack)->Detectors);

    // Info in all TPCs

    for (Int_t subdet = 0; subdet<3; subdet++) {
      if (!SubDetId::GetDetectorUsed(box().MainTrack->Detector, static_cast<SubDetId::SubDetEnum >(subdet+2))) continue;
      AnaTPCParticle* TPCSegment = static_cast<AnaTPCParticle*>(anaUtils::GetSegmentInDet( *box().MainTrack,static_cast<SubDetId::SubDetEnum >(subdet+2)));
      if (!TPCSegment) continue;
      output().FillVectorVar(selmu_tpc_det,     subdet);
      output().FillVectorVar(selmu_tpc_mom,     TPCSegment->Momentum);

      output().FillVectorVar(selmu_tpc_bfield_mom, TPCSegment->RefitMomentum);
      output().FillVectorVar(selmu_tpc_efield_mom, TPCSegment->EFieldRefitMomentum);
      output().FillVectorVar(selmu_tpc_charge,  TPCSegment->Charge);
      output().FillVectorVar(selmu_tpc_nhits,   TPCSegment->NHits);
      output().FillVectorVar(selmu_tpc_nnodes,  TPCSegment->NNodes);
      output().FillVectorVar(selmu_tpc_emom,    TPCSegment->MomentumError);

      if (TPCSegment->TrueObject)
        output().FillVectorVar(selmu_tpc_truemom, TPCSegment->GetTrueParticle()->Momentum);

      if (TPCSegment->Original->Original)
        output().FillVectorVar(selmu_tpc_dedx_raw,    (static_cast< const AnaTPCParticle* >(TPCSegment->Original->Original))->dEdxMeas);

      output().FillVectorVar(selmu_tpc_dedx_expmu,  TPCSegment->dEdxexpMuon);
      output().FillVectorVar(selmu_tpc_dedx_exppi,  TPCSegment->dEdxexpPion);
      output().FillVectorVar(selmu_tpc_dedx_expele, TPCSegment->dEdxexpEle);
      output().FillVectorVar(selmu_tpc_dedx_expp,   TPCSegment->dEdxexpProton);

      if (!ND::params().GetParameterI("numuCCAnalysis.MicroTrees.AdditionalToyVars")){
        output().FillVectorVar(selmu_tpc_dedx,   TPCSegment->dEdxMeas);
        output().FillVectorVar(selmu_tpc_pullmu, TPCSegment->Pullmu);
        output().FillVectorVar(selmu_tpc_pullpi, TPCSegment->Pullpi);
        output().FillVectorVar(selmu_tpc_pullele,TPCSegment->Pullele);
        output().FillVectorVar(selmu_tpc_pullp,  TPCSegment->Pullp);
      }

      output().IncrementCounterForVar(selmu_tpc_det);
    }

    // Info in all FGDs
    for (Int_t subdet = 0; subdet<2; subdet++) {
      if (!SubDetId::GetDetectorUsed(box().MainTrack->Detector, static_cast<SubDetId::SubDetEnum >(subdet))) continue;
      AnaFGDParticle* FGDSegment = static_cast<AnaFGDParticle*>(anaUtils::GetSegmentInDet( *box().MainTrack,static_cast<SubDetId::SubDetEnum >(subdet)));
      if (!FGDSegment) continue;

      // output().FillVectorVar("selmu_fgd_det",     (Int_t)FGDSegment->Detector);
      output().FillVectorVar(selmu_fgd_det,      subdet);
      output().FillVectorVar(selmu_fgd_x,        FGDSegment->X);
      output().FillVectorVar(selmu_fgd_V11,      FGDSegment->Vertex1x1);
      output().FillVectorVar(selmu_fgd_V33,      FGDSegment->Vertex3x3);
      output().FillVectorVar(selmu_fgd_V55,      FGDSegment->Vertex5x5);
      output().FillVectorVar(selmu_fgd_V77,      FGDSegment->Vertex7x7);
      output().FillVectorVar(selmu_fgd_VLayer,   FGDSegment->VertexLayer);

      if (!ND::params().GetParameterI("numuCCAnalysis.MicroTrees.AdditionalToyVars"))
        output().FillVectorVar(selmu_fgd_pullmu,   FGDSegment->Pullmu);

      output().IncrementCounterForVar(selmu_fgd_det);
    }

    // Info in all ECALs
    for (Int_t subdet = 0; subdet<9; subdet++) {
      if (!SubDetId::GetDetectorUsed(box().MainTrack->Detector, static_cast<SubDetId::SubDetEnum >(subdet+6))) continue;

      AnaECALParticle* ECALSegment = static_cast<AnaECALParticle*>(anaUtils::GetSegmentInDet( *box().MainTrack,static_cast<SubDetId::SubDetEnum >(subdet+6)));

      if (!ECALSegment) continue;

      output().FillVectorVar(selmu_ecal_det,                     subdet);
      output().FillVectorVar(selmu_ecal_nhits,                   ECALSegment->NHits);
      output().FillVectorVar(selmu_ecal_nnodes,                  ECALSegment->NNodes);
      output().FillVectorVar(selmu_ecal_length,                  ECALSegment->Length);
      output().FillMatrixVarFromArray(selmu_ecal_showerstartpos, ECALSegment->PositionStart,  4 );
      output().FillMatrixVarFromArray(selmu_ecal_showerendpos,   ECALSegment->PositionEnd,    4 );
      output().FillMatrixVarFromArray(selmu_ecal_showerstartdir, ECALSegment->DirectionStart, 3 );
      output().FillMatrixVarFromArray(selmu_ecal_showerenddir,   ECALSegment->DirectionEnd,   3 );

      output().FillVectorVar(selmu_ecal_EMenergy,      ECALSegment->EMEnergy);
      output().FillVectorVar(selmu_ecal_edeposit,      ECALSegment->EDeposit);
      output().FillVectorVar(selmu_ecal_IsShower,      ECALSegment->IsShowerLike);

      output().FillVectorVar(selmu_ecal_trshval,              ECALSegment->TrShVal);
      output().FillVectorVar(selmu_ecal_emhip,                ECALSegment->PIDEmHip);
      output().FillVectorVar(selmu_ecal_mippion,              ECALSegment->PIDMipPion);
      output().FillVectorVar(selmu_ecal_mipem,                ECALSegment->PIDMipEm);
      output().FillVectorVar(selmu_ecal_containment,          ECALSegment->Containment);
      output().FillVectorVar(selmu_ecal_mostupstreamlayerhit, ECALSegment->MostUpStreamLayerHit);

      output().IncrementCounterForVar(selmu_ecal_det);

    }

    // Info in SMRDs
    for (Int_t i = 0; i < box().MainTrack->nSMRDSegments; i++){
      AnaSMRDParticle* smrdTrack = static_cast<AnaSMRDParticle*>(box().MainTrack->SMRDSegments[i]);
      if (!smrdTrack) continue;

      output().FillVectorVar(selmu_smrd_det,
          convUtils::GetLocalDetEnum(SubDetId::kSMRD, SubDetId::GetSubdetectorEnum(smrdTrack->Detector)));
      output().FillVectorVar(selmu_smrd_nhits,    smrdTrack->NHits);
      output().FillVectorVar(selmu_smrd_nnodes,   smrdTrack->NNodes);
      output().FillVectorVar(selmu_smrd_edeposit, smrdTrack->EDeposit);
      output().FillMatrixVarFromArray(selmu_smrd_dir,    smrdTrack->DirectionStart,    3 );
      output().FillMatrixVarFromArray(selmu_smrd_enddir, smrdTrack->DirectionEnd, 3 );
      output().IncrementCounterForVar(selmu_smrd_det);
    }

  } // end if MainTrack

  if ( ! ND::params().GetParameterI("numuCCAnalysis.MicroTrees.AdditionalToyVars")) {
    if (box().SHMNtrack) output().FillVar(shmn_mom, box().SHMNtrack->Momentum);
  }

}

//********************************************************************
void numuCCAnalysis::FillToyVarsInMicroTrees(bool addBase){
  //********************************************************************

  // Fill all tree variables that vary for each virtual analysis (toy experiment)

  // Fill the common variables
  if (addBase) baseTrackerAnalysis::FillToyVarsInMicroTrees(addBase);

  // variables specific for this analysis
  if (box().MainTrack){

    // Momentum
    output().FillToyVar(selmu_mom, box().MainTrack->Momentum);

    // Costheta
    // vertex defined as start of muon track, neutrino direction calculated from this
    TVector3 nuDirVec = anaUtils::GetNuDirRec(box().MainTrack->PositionStart);
    TVector3 muDirVec = anaUtils::ArrayToTVector3(box().MainTrack->DirectionStart);
    // The cosine of the angle between the muon and the neutrino
    double costheta_mu_nu = nuDirVec.Dot(muDirVec);
    output().FillToyVar(selmu_costheta, (Float_t)costheta_mu_nu);

    // Erec
    Float_t Erec = anaUtils::ComputeRecNuEnergyCCQE(box().MainTrack->Momentum, units::mass_muon, costheta_mu_nu);
    output().FillToyVar(selmu_nuErecQE, Erec);

    if (ND::params().GetParameterI("numuCCAnalysis.MicroTrees.AdditionalToyVars")){
      output().FillToyVar(selmu_amom, (static_cast<AnaTrack*>(box().MainTrack))->MomentumMuon);
      // PID likelihoods
      output().FillToyVar(selmu_likemu, anaUtils::GetPIDLikelihood( *(box().MainTrack),0));
      output().FillToyVar(selmu_likemip,anaUtils::GetPIDLikelihoodMIP( *(box().MainTrack)));

      // Info in all TPCs
      for (Int_t subdet = 0; subdet<3; subdet++) {
        if (!SubDetId::GetDetectorUsed(box().MainTrack->Detector, static_cast<SubDetId::SubDetEnum >(subdet+2))) continue;
        AnaTPCParticle* TPCSegment = static_cast<AnaTPCParticle*>(anaUtils::GetSegmentInDet( *box().MainTrack, static_cast<SubDetId::SubDetEnum >(subdet+2)));
        if (!TPCSegment) continue;
        output().FillToyVectorVar(selmu_tpc_dedx,   TPCSegment->dEdxMeas, subdet);
        output().FillToyVectorVar(selmu_tpc_pullmu, TPCSegment->Pullmu,   subdet);
        output().FillToyVectorVar(selmu_tpc_pullpi, TPCSegment->Pullpi,   subdet);
        output().FillToyVectorVar(selmu_tpc_pullele,TPCSegment->Pullele,  subdet);
        output().FillToyVectorVar(selmu_tpc_pullp,  TPCSegment->Pullp,    subdet);
      }

      // Info in all FGDs
      for (Int_t subdet = 0; subdet<2; subdet++) {
        if (!SubDetId::GetDetectorUsed(box().MainTrack->Detector, static_cast<SubDetId::SubDetEnum >(subdet))) continue;
        AnaFGDParticle* FGDSegment = static_cast<AnaFGDParticle*>(anaUtils::GetSegmentInDet(*(box().MainTrack), static_cast<SubDetId::SubDetEnum >(subdet)));
        if (!FGDSegment) continue;
        output().FillToyVectorVar(selmu_fgd_pullmu, FGDSegment->Pullmu, subdet);
      }
    }

    // if there is a true vertex associated
    if(box().MainTrack->TrueObject) {
      AnaTrueVertex *vtx = static_cast<AnaTrueVertex*>(box().MainTrack->GetTrueParticle()->TrueVertex);
      if (vtx) {
        // Fill the mass component related to the mass weight
        anaUtils::massComponentEnum massComponent = anaUtils::GetMassComponent(GetEvent().GetIsMC(), vtx->Position);
        output().FillToyVar(truevtx_mass_component, massComponent);

        // Fill whether it is signal or bkg
        if (anaUtils::GetReactionCC(*vtx, SubDetId::kFGD1) == 1)
          output().FillToyVar(true_signal, 1);
        else if (anaUtils::GetReactionCC(*vtx, SubDetId::kFGD2) == 1)
          output().FillToyVar(true_signal, 2);
        else
          output().FillToyVar(true_signal, 0);


        static Int_t mass_weight_index=-2;
        if (_saveMassWeights && conf().GetCurrentConfigurationIndex() == all_syst && mass_weight_index!=-1){
          // Get the index of the mass weight in the all_syst configuration, but only once
          if (mass_weight_index==-2){
            ConfigTreeTools config(syst(),conf());
            mass_weight_index = config.GetWeightIndex(all_syst, SystId::kFgdMass);
          }                
          if (mass_weight_index>=0){
            Float_t massWeightValue = output().GetToyVectorVarValueF(AnalysisAlgorithm::weight_syst, mass_weight_index);

            // Get the current tree
            Int_t index = output().GetCurrentTree();

            // Set as current tree the massWeightTree
            output().SetCurrentTree(massWeightTree);

            Int_t massComponentIndex=-1; 
            if      (massComponent == anaUtils::kFGD1)             massComponentIndex=0;
            else if (massComponent == anaUtils::kFGD2xymodules)    massComponentIndex=1;
            else if (massComponent == anaUtils::kFGD2watermodules) massComponentIndex=2;

            // if not yet filled fill it
            if (massComponentIndex>= 0 && output().GetMatrixVarValueF(massWeight, output().GetToyIndex(),massComponentIndex) <=0)
              output().FillMatrixVar( massWeight, massWeightValue, output().GetToyIndex(),massComponentIndex);

            // Go back to the standard current tree
            output().SetCurrentTree(index);
          }
        }
      }
    }

  } // end if MainTrack


  if (ND::params().GetParameterI("numuCCAnalysis.MicroTrees.AdditionalToyVars")){
    if (box().SHMNtrack){
      output().FillToyVar(shmn_mom, box().SHMNtrack->Momentum);
    }
  }

}

//********************************************************************
bool numuCCAnalysis::CheckFillTruthTree(const AnaTrueVertex& vtx){
  //********************************************************************
  SubDetId::SubDetEnum fgdID;
  if (_whichFGD == 1) fgdID = SubDetId::kFGD1;
  if (_whichFGD == 2) fgdID = SubDetId::kFGD2;
  if (_whichFGD >  2) fgdID = SubDetId::kFGD;
  // GetReactionCC already takes into account the outFV and also
  // the NuWro reaction code for 2p2h in prod5 (that is 70)
  bool numuCCinFV = (anaUtils::GetReactionCC(vtx, fgdID) == 1);
  return (numuCCinFV);
}

//********************************************************************
void numuCCAnalysis::FillTruthTree(const AnaTrueVertex& vtx){
  //********************************************************************

  // workaround to use the same code for the antuNumu package
  // calling numuCCAnalysis::FillTruthTreeBase(vtx,true)
  bool IsAntinu = false;
  FillTruthTreeBase(vtx, IsAntinu);

  // Fill some truth kinematics for the true lepton candidate inside the
  // detectors crossers
  // loop over the truth trajectories and find the one the corresponds to the
  // current lepton candidate
  for (int i = 0; i < vtx.nTrueParticles; i++){
    if (!vtx.TrueParticles[i]) continue;

    // Should be enough?
    if (vtx.TrueParticles[i]->PDG != vtx.LeptonPDG || 
        vtx.TrueParticles[i]->Momentum != vtx.LeptonMom)
      continue;

    AnaTrueParticleB* trueTrack = vtx.TrueParticles[i];
    
    // Loop over detector crossing points
    // to review if there are more than one point in one TPC --> should not be
    // the case
    for (int j = 0; j < trueTrack->nDetCrossings; j++){
      AnaDetCrossingB* cross = trueTrack->DetCrossings[j];
      if (!cross) continue;

      if (!cross->InActive) continue;
      
      int tpc_num = SubDetId::GetTPC(cross->Detector);

      // Fill the vars
      switch (tpc_num){
        case 1:
          output().FillVectorVarFromArray(truelepton_tpc1_entrance_pos, cross->EntrancePosition, 4);
          output().FillVectorVarFromArray(truelepton_tpc1_exit_pos,     cross->ExitPosition,     4);
          break; 
        case 2:
          output().FillVectorVarFromArray(truelepton_tpc2_entrance_pos, cross->EntrancePosition, 4);
          output().FillVectorVarFromArray(truelepton_tpc2_exit_pos,     cross->ExitPosition,     4);
          break;
        case 3:
          output().FillVectorVarFromArray(truelepton_tpc3_entrance_pos, cross->EntrancePosition, 4);
          output().FillVectorVarFromArray(truelepton_tpc3_exit_pos,     cross->ExitPosition,     4);
          break;
        default:
          break;
      } 
    }
  }
}

//********************************************************************
void numuCCAnalysis::FillTruthTreeBase(const AnaTrueVertex& vtx, bool IsAntinu){
  //********************************************************************

  // this method is also called from the antiNumu package with IsAntinu = true

  // Fill the common variables
  baseTrackerAnalysis::FillTruthTreeBase(vtx, SubDetId::kFGD1, IsAntinu);

  // Muon true-rec association.
  //  if (box().MainTrack)
  //    if (box().MainTrack->TrueObject)
  //      output().FillVar("mu_true_rec", (box().MainTrack->GetTrueParticle()->PDG==13));   // Is the recon muon a true muon ?

  // Fill truth categories for FGD2, same categories with prefix "fgd2", i,e, "fgd2reaction" etc.
  // We could directly set which FGD in the usual categories, but this is not possible for the true vertices
  anaUtils::FillCategories(&vtx, "fgd2", SubDetId::kFGD2, IsAntinu,GetSpill().GetIsSandMC());

  // @todo: worth a parameter to enable these
  // Fill truth categories (anti)numu contamination, same categories with prefix "bkg_", i,e, "bkg_reaction" etc.
//  anaUtils::FillCategories(&vtx, "bkg_", SubDetId::kFGD1, ! IsAntinu, GetSpill().GetIsSandMC());
//  anaUtils::FillCategories(&vtx, "bkg_fgd2", SubDetId::kFGD2, ! IsAntinu, GetSpill().GetIsSandMC());

  // Fill whether it is signal or bkg
  if (anaUtils::GetReactionCC(vtx, SubDetId::kFGD1, IsAntinu) == 1)
    output().FillVar(true_signal, 1);
  else if (anaUtils::GetReactionCC(vtx, SubDetId::kFGD2, IsAntinu) == 1)
    output().FillVar(true_signal, 2);
  else
    output().FillVar(true_signal, 0);

  // Fill the mass component related to the mass weight
  anaUtils::massComponentEnum massComponent = anaUtils::GetMassComponent(GetSpill().GetIsMC(), vtx.Position);
  output().FillVar(truevtx_mass_component, massComponent);

  // Fill the reconstructed energy with true lepton kinematics in CCQE formula
  double costheta_mu_nu = cos(anaUtils::ArrayToTVector3(vtx.LeptonDir).Angle(anaUtils::ArrayToTVector3(vtx.NuDir)));
  Float_t Erec = anaUtils::ComputeRecNuEnergyCCQE(vtx.LeptonMom, units::mass_muon, costheta_mu_nu);
  output().FillVar(truelepton_nuErecQE, Erec);
}

//********************************************************************
void numuCCAnalysis::FillCategories(){
  //********************************************************************

  // Fill the track categories for color drawing

  // For the muon candidate
  anaUtils::FillCategories(&GetEvent(), static_cast<AnaTrack*>(box().MainTrack),"", SubDetId::kFGD1);

  // For FGD2, same categories with prefix "fgd2", i,e, "fgd2reaction" etc.
  // We could directly set which FGD in the usual categories, but this cannot be done for the true vertices
  anaUtils::FillCategories(&GetEvent(), static_cast<AnaTrack*>(box().MainTrack), "fgd2", SubDetId::kFGD2);
}

//********************************************************************
void numuCCAnalysis::FillConfigTree(){
  //********************************************************************

  // Add and fill number of nucleons in each of the targets
  AddVarD(output(), nNucleonsFGD1,      "number of targets in FGD1");
  AddVarD(output(), nNucleonsFGD2scint, "number of targets in FGD2 scintillator");
  AddVarD(output(), nNucleonsFGD2water, "number of targets in FGD2 water");

  output().FillVar(nNucleonsFGD1,      anaUtils::GetNTargets(anaUtils::kFGD1));
  output().FillVar(nNucleonsFGD2scint, anaUtils::GetNTargets(anaUtils::kFGD2xymodules));
  output().FillVar(nNucleonsFGD2water, anaUtils::GetNTargets(anaUtils::kFGD2watermodules));

}

//********************************************************************
void numuCCAnalysis::SetFV(){
  //********************************************************************

  // Use reduced FV (needed before numuCCAnalysis->Initialize which calls GetNTargets)
  if (ND::params().GetParameterI("numuCCAnalysis.Selections.UseAlternativeFV")) {

    // make FGD1 and FGD2 FV identical

    // use smaller FV in X and Y
    double FVXmin = TMath::Max(FVDef::FVdefminFGD1[0], FVDef::FVdefminFGD2[0]);
    double FVXmax = TMath::Max(FVDef::FVdefmaxFGD1[0], FVDef::FVdefmaxFGD2[0]);
    double FVYmin = TMath::Max(FVDef::FVdefminFGD1[1], FVDef::FVdefminFGD2[1]);
    double FVYmax = TMath::Max(FVDef::FVdefmaxFGD1[1], FVDef::FVdefmaxFGD2[1]);
    FVDef::FVdefminFGD1[0] = FVDef::FVdefminFGD2[0] = FVXmin;
    FVDef::FVdefmaxFGD1[0] = FVDef::FVdefmaxFGD2[0] = FVXmax;
    FVDef::FVdefminFGD1[1] = FVDef::FVdefminFGD2[1] = FVYmin;
    FVDef::FVdefmaxFGD1[1] = FVDef::FVdefmaxFGD2[1] = FVYmax;

    // discard first and last layer of both FGDs
    // (official FV only discards first layer of FGD2 and first two layers of FGD1)
    double halfmodule = DetDef::fgdXYModuleWidth/2.;
    FVDef::FVdefminFGD1[2] = FVDef::FVdefminFGD2[2] = halfmodule;
    FVDef::FVdefmaxFGD1[2] = FVDef::FVdefmaxFGD2[2] = halfmodule;

    // cout new values
    std::cout << "Using an alternative Fiducial Volume: " << std::endl;
    std::cout << "   DetDef::fgd1min = " << DetDef::fgd1min[0] << " "
      << DetDef::fgd1min[1] << " "
      << DetDef::fgd1min[2]
      << std::endl;
    std::cout << "   DetDef::fgd1max =  " << DetDef::fgd1max[0] << " "
      << DetDef::fgd1max[1] << " "
      << DetDef::fgd1max[2]
      << std::endl;
    std::cout << "   DetDef::fgd2min =  " << DetDef::fgd2min[0] << " "
      << DetDef::fgd2min[1] << " "
      << DetDef::fgd2min[2]
      << std::endl;
    std::cout << "   DetDef::fgd2max =  " << DetDef::fgd2max[0] << " "
      << DetDef::fgd2max[1] << " "
      << DetDef::fgd2max[2]
      << std::endl;
    std::cout << "   FVDef::FVdefminFGD1 = " << FVDef::FVdefminFGD1[0] << " "
      << FVDef::FVdefminFGD1[1] << " "
      << FVDef::FVdefminFGD1[2]
      << std::endl;
    std::cout << "   FVDef::FVdefmaxFGD1 = " << FVDef::FVdefmaxFGD1[0] << " "
      << FVDef::FVdefmaxFGD1[1] << " "
      << FVDef::FVdefmaxFGD1[2]
      << std::endl;
    std::cout << "   FVDef::FVdefminFGD2 = " << FVDef::FVdefminFGD2[0] << " "
      << FVDef::FVdefminFGD2[1] << " "
      << FVDef::FVdefminFGD2[2]
      << std::endl;
    std::cout << "   FVDef::FVdefmaxFGD2 = " << FVDef::FVdefmaxFGD2[0] << " "
      << FVDef::FVdefmaxFGD2[1] << " "
      << FVDef::FVdefmaxFGD2[2]
      << std::endl;
  }
}
