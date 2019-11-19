#include "antiNumuCCIncFwdAnalysis.hxx"
#include "FiducialVolumeDefinition.hxx"
#include "Parameters.hxx"
#include "UseGlobalAltMomCorrection.hxx"
#include "antiNumuCCMultiTrackSelection.hxx"
#include "antiNumuCCIncFwdSelection.hxx"
#include "antiNumuCCFGD2Selection.hxx"
#include "antiNumuCCMultiTrackFGD2Selection.hxx"
#include "CategoriesUtils.hxx"
#include "BasicUtils.hxx"

#include "ToFDSECalCorrection.hxx"
#include "ToFDSECalVariationSystematic.hxx"
#include "ToFECalOtherVariationSystematic.hxx"
#include "ECalMIPPionVariationSystematic.hxx"

#include "baseAnalysis.hxx"
#include "baseToyMaker.hxx"

//********************************************************************
antiNumuCCIncFwdAnalysis::antiNumuCCIncFwdAnalysis(AnalysisAlgorithm* ana) : baseTrackerAnalysis(ana) {
//********************************************************************

  // Add the package version
  ND::versioning().AddPackage("antiNumuCCIncFwdAnalysis", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("ANTINUMUCCINCFWDANALYSISROOT")));

  // Create a numuCCAnalysis passing this analysis to the constructor. In that way the same managers are used
  _numuCCAnalysis = new numuCCAnalysis(this);

  // Use the numuCCAnalysis (in practice that means that the same box and event will be used for the numuCCAnalysis as for this analysis)
  UseAnalysis(_numuCCAnalysis);
}

//********************************************************************
bool antiNumuCCIncFwdAnalysis::Initialize() {
//********************************************************************

  // Initialize the baseAnalysis instead of numuCCAnalysis
  // since we do not want to call and overload numu categories
  if(!baseTrackerAnalysis::Initialize()) return false;

  // Minimum accum level to save event into the output tree
  SetMinAccumCutLevelToSave(ND::params().GetParameterI("antiNumuCCIncFwdAnalysis.MinAccumLevelToSave"));

  // Define antinu categories (different legend from numuCC)
  // for FGD2 same categories with prefix "fgd2", i,e, "fgd2reaction" etc.)
  anaUtils::AddStandardAntiNumuCategories();
  anaUtils::AddStandardAntiNumuCategories("fgd2");

  return true;
}

//********************************************************************
void antiNumuCCIncFwdAnalysis::DefineSelections(){
//********************************************************************

  sel().AddSelection("kTrackerAntiNumuCCIncFwd", "inclusive antiNumuCC FGD1 fwd selection", new antiNumuCCIncFwdSelection(false));

}

//********************************************************************
void antiNumuCCIncFwdAnalysis::DefineCorrections(){
//********************************************************************

  // Some corrections are defined in baseTrackerAnalysis (have a look at baseTrackerAnalysis/vXrY/src/baseTrackerAnalysis.cxx)
  _numuCCAnalysis->DefineCorrections();

  //----------- Define all corrections ----------------
  if (ND::params().GetParameterI("baseTrackerAnalysis.Corrections.DisableAllCorrections")){
    // Should not be needed, but just in case !!!
    corr().DisableAllCorrections();
  }
  else{
    // Add corrections only when they are enabled. In that way the CorrectionManager does not have to loop over unused corrections
    if ((bool)ND::params().GetParameterI("antiNumuCCIncFwdAnalysis.Corrections.EnableToFDSECalMC"))  corr().AddCorrection("tof_dsecal_mc_corr",   new ToFDSECalCorrection());
  }

}

//********************************************************************
void antiNumuCCIncFwdAnalysis::DefineSystematics(){
//********************************************************************

  _numuCCAnalysis->DefineSystematics();
  
  if (ND::params().GetParameterI("antiNumuCCIncFwdAnalysis.Systematics.EnableToFDSECalVariationSystematic"))
    evar().AddEventVariation(kToFDSECalFGD1,    "ToFDSECalFGD1VarSyst", new ToFDSECalVariationSystematic());

  if (ND::params().GetParameterI("antiNumuCCIncFwdAnalysis.Systematics.EnableToFECalOtherVariationSystematic"))
    evar().AddEventVariation(kToFECalOther,     "ToFECalOtherVarSyst",  new ToFECalOtherVariationSystematic());

  if (ND::params().GetParameterI("antiNumuCCIncFwdAnalysis.Systematics.EnableECalMipPionVariationSystematic"))
    evar().AddEventVariation(kECalMIPPion,      "ECalMipPionVarSyst",   new ECalMIPPionVariationSystematic());

}

//********************************************************************
void antiNumuCCIncFwdAnalysis::DefineConfigurations(){
//********************************************************************

  _numuCCAnalysis->DefineConfigurations();

  Int_t ntoys = ND::params().GetParameterI("baseAnalysis.Systematics.NumberOfToys");
  Int_t randomSeed = ND::params().GetParameterI("baseAnalysis.Systematics.RandomSeed");

  if (ND::params().GetParameterI("antiNumuCCIncFwdAnalysis.Systematics.EnableToFDSECalVariationSystematic")){
    AddConfiguration(conf(), tofdsecalfgd1_syst, ntoys, randomSeed, new baseToyMaker(randomSeed));
    conf().EnableEventVariation(kToFDSECalFGD1, tofdsecalfgd1_syst);
  }

  if (ND::params().GetParameterI("antiNumuCCIncFwdAnalysis.Systematics.EnableToFECalOtherVariationSystematic")){
    AddConfiguration(conf(), tofecalother_syst, ntoys, randomSeed, new baseToyMaker(randomSeed));
    conf().EnableEventVariation(kToFECalOther, tofecalother_syst);
  }

  if (ND::params().GetParameterI("antiNumuCCIncFwdAnalysis.Systematics.EnableECalMipPionVariationSystematic")){
    AddConfiguration(conf(), ecalmippion_syst, ntoys, randomSeed, new baseToyMaker(randomSeed));
    conf().EnableEventVariation(kECalMIPPion, ecalmippion_syst);
  }

  if (ND::params().GetParameterI("baseAnalysis.Configurations.EnableAllSystematics")){
    conf().EnableEventVariation(kToFDSECalFGD1, all_syst);
    conf().EnableEventVariation(kToFECalOther,  all_syst);
    conf().EnableEventVariation(kECalMIPPion,   all_syst);
  }

}

//********************************************************************
void antiNumuCCIncFwdAnalysis::DefineMicroTrees(bool addBase){
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

  // ECal PID related variables
  AddToyVarVF(output(), selmu_myvars_ecal_mippion,              "ECal PID wheter track is MIP- or Pion-like", 9);

  // Time of flight related variables
  AddToyVarF(output(), selmu_myvars_tof_fgd1_fgd2,              "time of flight from FGD2 to FGD1 main track");
  AddToyVarF(output(), selmu_myvars_tof_p0d_fgd1,               "time of flight from FGD1 to P0D main track");
  AddToyVarF(output(), selmu_myvars_tof_tecal_fgd1,             "time of flight from FGD1 to TECal main track");
  AddToyVarF(output(), selmu_myvars_tof_tecal_fgd2,             "time of flight from FGD2 to TECal main track");
  AddToyVarF(output(), selmu_myvars_tof_dsecal_fgd1,            "time of flight from FGD1 to DSECal main track");
  AddToyVarF(output(), selmu_myvars_tof_dsecal_fgd2,            "time of flight from FGD2 to DSECal main track");
  AddToyVarF(output(), selmu_myvars_tof_ecal_fgd1_other,        "time of flight from FGD1 to ECal earliest track");

  AddToyVarF(output(), selmu_myvars_time_fgd1,           "mean time FGD1");
  AddToyVarF(output(), selmu_myvars_time_tecal,          "mean time TECal");
  AddToyVarF(output(), selmu_myvars_time_dsecal,         "mean time DSECal");

  // Subdet matching efficiency related variables
  AddVarI(output(), selmu_myvars_tpc1_maintrack, "main track is in TPC1");
  AddVarI(output(), selmu_myvars_tpc2_maintrack, "main track is in TPC2");
  AddVarI(output(), selmu_myvars_tpc3_maintrack, "main track is in TPC3");
  AddVarI(output(), selmu_myvars_fgd1_maintrack, "main track is in FGD1");
  AddVarI(output(), selmu_myvars_fgd2_maintrack, "main track is in FGD2");

  // ECal sub-detectors used
  AddVarI(output(), selmu_myvars_kECAL,           "ECal");
  AddVarI(output(), selmu_myvars_kPECAL,          "POD ECal");
  AddVarI(output(), selmu_myvars_kTopPECAL,       "top POD ECal");
  AddVarI(output(), selmu_myvars_kBottomPECAL,    "bottom POD ECal");
  AddVarI(output(), selmu_myvars_kLeftPECAL,      "left POD ECal");
  AddVarI(output(), selmu_myvars_kRightPECAL,     "right POD ECal");
  AddVarI(output(), selmu_myvars_kTECAL,          "tracker ECal");
  AddVarI(output(), selmu_myvars_kTopTECAL,       "top tracker ECal");
  AddVarI(output(), selmu_myvars_kBottomTECAL,    "bottom tracker ECal");
  AddVarI(output(), selmu_myvars_kLeftTECAL,      "left tracker ECal");
  AddVarI(output(), selmu_myvars_kRightTECAL,     "right tracker ECal");
  AddVarI(output(), selmu_myvars_kDSECAL,         "downstream ECal");

}

//********************************************************************
void antiNumuCCIncFwdAnalysis::DefineTruthTree(){
//********************************************************************

  // Variables from numuCCAnalysis
  _numuCCAnalysis->DefineTruthTree();

}

//********************************************************************
void antiNumuCCIncFwdAnalysis::FillMicroTrees(bool addBase){
//********************************************************************

  // Variables from numuCCAnalysis
  if (addBase) _numuCCAnalysis->FillMicroTrees(addBase);

  if (!ND::params().GetParameterI("numuCCAnalysis.MicroTrees.AdditionalToyVars")){
    if (box().SHMPtrack)
      output().FillVar(shmp_mom, box().SHMPtrack->Momentum);
  }

  // Muon candidate variables. The MainTrack (The muon candidate) should exist in the box
  if (box().HMPtrack) {
    output().FillVar(selmu_myvars_tpc1_maintrack, anaUtils::TrackUsesDet(*static_cast<AnaTrackB*>(box().HMPtrack), SubDetId::kTPC1));
    output().FillVar(selmu_myvars_tpc2_maintrack, anaUtils::TrackUsesDet(*static_cast<AnaTrackB*>(box().HMPtrack), SubDetId::kTPC2));
    output().FillVar(selmu_myvars_tpc3_maintrack, anaUtils::TrackUsesDet(*static_cast<AnaTrackB*>(box().HMPtrack), SubDetId::kTPC3));
    output().FillVar(selmu_myvars_fgd1_maintrack, anaUtils::TrackUsesDet(*static_cast<AnaTrackB*>(box().HMPtrack), SubDetId::kFGD1));
    output().FillVar(selmu_myvars_fgd2_maintrack, anaUtils::TrackUsesDet(*static_cast<AnaTrackB*>(box().HMPtrack), SubDetId::kFGD2));

    output().FillVar(selmu_myvars_kECAL,        SubDetId::GetDetectorUsed(box().HMPtrack->Detector, static_cast<SubDetId::SubDetEnum>(SubDetId::kECAL)));
    output().FillVar(selmu_myvars_kPECAL,       SubDetId::GetDetectorUsed(box().HMPtrack->Detector, static_cast<SubDetId::SubDetEnum>(SubDetId::kPECAL)));
    output().FillVar(selmu_myvars_kTopPECAL,    SubDetId::GetDetectorUsed(box().HMPtrack->Detector, static_cast<SubDetId::SubDetEnum>(SubDetId::kTopPECAL)));
    output().FillVar(selmu_myvars_kBottomPECAL, SubDetId::GetDetectorUsed(box().HMPtrack->Detector, static_cast<SubDetId::SubDetEnum>(SubDetId::kBottomPECAL)));
    output().FillVar(selmu_myvars_kLeftPECAL,   SubDetId::GetDetectorUsed(box().HMPtrack->Detector, static_cast<SubDetId::SubDetEnum>(SubDetId::kLeftPECAL)));
    output().FillVar(selmu_myvars_kRightPECAL,  SubDetId::GetDetectorUsed(box().HMPtrack->Detector, static_cast<SubDetId::SubDetEnum>(SubDetId::kRightPECAL)));
    output().FillVar(selmu_myvars_kTECAL,       SubDetId::GetDetectorUsed(box().HMPtrack->Detector, static_cast<SubDetId::SubDetEnum>(SubDetId::kTECAL)));
    output().FillVar(selmu_myvars_kTopTECAL,    SubDetId::GetDetectorUsed(box().HMPtrack->Detector, static_cast<SubDetId::SubDetEnum>(SubDetId::kTopTECAL)));
    output().FillVar(selmu_myvars_kBottomTECAL, SubDetId::GetDetectorUsed(box().HMPtrack->Detector, static_cast<SubDetId::SubDetEnum>(SubDetId::kBottomTECAL)));
    output().FillVar(selmu_myvars_kLeftTECAL,   SubDetId::GetDetectorUsed(box().HMPtrack->Detector, static_cast<SubDetId::SubDetEnum>(SubDetId::kLeftTECAL)));
    output().FillVar(selmu_myvars_kRightTECAL,  SubDetId::GetDetectorUsed(box().HMPtrack->Detector, static_cast<SubDetId::SubDetEnum>(SubDetId::kRightTECAL)));
    output().FillVar(selmu_myvars_kDSECAL,      SubDetId::GetDetectorUsed(box().HMPtrack->Detector, static_cast<SubDetId::SubDetEnum>(SubDetId::kDSECAL)));

    // Info in all ECALs
    for (Int_t subdet = 0; subdet<9; subdet++) {

      if (!SubDetId::GetDetectorUsed(box().MainTrack->Detector,static_cast<SubDetId::SubDetEnum>(subdet+6))) continue;

      AnaECALParticle* ECALSegment = static_cast<AnaECALParticle*>(anaUtils::GetSegmentInDet(*box().MainTrack,static_cast<SubDetId::SubDetEnum >(subdet+6)));

      if (!ECALSegment) continue;

      output().IncrementCounterForVar(selmu_nECals);
    }
  }
}

//********************************************************************
void antiNumuCCIncFwdAnalysis::FillToyVarsInMicroTrees(bool addBase){
//********************************************************************

  // Fill all tree variables that vary for each virtual analysis (toy experiment)

  // Variables from numuCCAnalysis
  if (addBase) _numuCCAnalysis->FillToyVarsInMicroTrees(addBase);

  if (ND::params().GetParameterI("numuCCAnalysis.MicroTrees.AdditionalToyVars")){
    if (box().SHMPtrack){
      output().FillToyVar(shmp_mom, box().SHMPtrack->Momentum);
    }
  }

  //---- variables specific for this analysis -----
  if (box().HMPtrack){

    // Info in all ECALs
    for (Int_t subdet = 0; subdet<9; subdet++) {

      if (!SubDetId::GetDetectorUsed(box().MainTrack->Detector,static_cast<SubDetId::SubDetEnum>(subdet+6))) continue;

      AnaECALParticle* ECALSegment = static_cast<AnaECALParticle*>(anaUtils::GetSegmentInDet(*box().MainTrack,static_cast<SubDetId::SubDetEnum >(subdet+6)));

      if (!ECALSegment) continue;

      output().FillToyVectorVar(selmu_myvars_ecal_mippion, ECALSegment->PIDMipPion, subdet);
    }

    // Time of flight variables
    // ToF FGD2->FGD1
    output().FillToyVar(selmu_myvars_tof_fgd1_fgd2, box().HMPtrack->ToF.FGD1_FGD2);
    // ToF FGD1->P0D
    output().FillToyVar(selmu_myvars_tof_p0d_fgd1, box().HMPtrack->ToF.P0D_FGD1);
    // ToF FGD1->BarrelECal
    output().FillToyVar(selmu_myvars_tof_tecal_fgd1, box().HMPtrack->ToF.ECal_FGD1);
    // ToF FGD2->BarrelECal
    output().FillToyVar(selmu_myvars_tof_tecal_fgd2, box().HMPtrack->ToF.ECal_FGD2);
    // ToF FGD1->DSECal
    output().FillToyVar(selmu_myvars_tof_dsecal_fgd1, box().HMPtrack->ToF.DSECal_FGD1);
    // ToF FGD2->DSECal
    output().FillToyVar(selmu_myvars_tof_dsecal_fgd2, box().HMPtrack->ToF.DSECal_FGD2);

    float ToF_ECal = false;
    float ToF_min_ECal = false;

    AnaTrackB* primary = static_cast<AnaTrackB*>(box().HMPtrack);

    AnaTrackB* ecalTracks[100];
    Int_t ntracks = anaUtils::GetAllTracksUsingDet(*static_cast<AnaEventB*>(&GetEvent()), SubDetId::kECAL, ecalTracks);

    if (ntracks > 0) {

      for (int i = 0; i < ntracks; ++i) {

        AnaTrackB* thisTrack = ecalTracks[i];

        if (thisTrack == primary) {
          // Mean Time FGD1
          if (thisTrack->Time.Flag_FGD1) {
            output().FillToyVar(selmu_myvars_time_fgd1, thisTrack->Time.FGD1);
          }
          // Mean Time TECal
          if (thisTrack->Time.Flag_ECal) {
            output().FillToyVar(selmu_myvars_time_tecal, thisTrack->Time.ECal);
          }

          // Mean Time DSECal
          if (thisTrack->Time.Flag_DSECal) {
            output().FillToyVar(selmu_myvars_time_dsecal, thisTrack->Time.DSECal);
          }
          continue;
        }

        if (primary->Time.Flag_FGD1) {

          if (thisTrack->Time.Flag_DSECal) {
            ToF_ECal = thisTrack->Time.DSECal-primary->Time.FGD1;
          }

          else if (thisTrack->Time.Flag_ECal) {
            ToF_ECal = thisTrack->Time.ECal-primary->Time.FGD1;
          }

          if (!ToF_min_ECal) ToF_min_ECal = ToF_ECal;
          else if (ToF_ECal < ToF_min_ECal) ToF_min_ECal = ToF_ECal;
        }
      }
    }

    if (ToF_min_ECal) output().FillToyVar(selmu_myvars_tof_ecal_fgd1_other, ToF_min_ECal);
  }
}

//********************************************************************
bool antiNumuCCIncFwdAnalysis::CheckFillTruthTree(const AnaTrueVertex& vtx){
//********************************************************************

  // we cannot call the numuCC method here because 1) the setting of _whichFGD would be lost
  // and 2) the called methods need the IsAntinu paramenter set to true

  SubDetId::SubDetEnum fgdID;
  fgdID = SubDetId::kFGD1;

  bool IsAntinu = true;

  // GetReactionCC already takes into account the outFV and also
  // the NuWro reaction code for 2p2h in prod5 (that is 70)
  bool antiNumuCCIncFwdinFV = (anaUtils::GetReactionCC(vtx, fgdID, IsAntinu) == 1);

  // Since our topology definition doesn't consider the reaction code
  // and since a muon pair can be created in the FSI (likely in DIS)
  // in principle we might have a non-CC vertex categorized as CCother
  // (nevertheless I didn't find any)
  int topo = anaUtils::GetTopology(vtx, fgdID, IsAntinu);
  bool topoCCinFV = (topo == 0 || topo == 1 || topo == 2);

  return (antiNumuCCIncFwdinFV || topoCCinFV);
}

//********************************************************************
void antiNumuCCIncFwdAnalysis::FillTruthTree(const AnaTrueVertex& vtx){
//********************************************************************

  // Variables from numuCCAnalysis, with antinu categories
  bool IsAntinu = true;
  _numuCCAnalysis->FillTruthTreeBase(vtx,IsAntinu);

}

//********************************************************************
void antiNumuCCIncFwdAnalysis::FillCategories(){
//********************************************************************

  // Fill the track categories for color drawing

  bool IsAntinu = true;

  // For the muon candidate
  anaUtils::FillCategories(&GetEvent(), static_cast<AnaTrack*>(box().MainTrack),"",SubDetId::kFGD1,IsAntinu);

  // For FGD2, same categories with prefix "fgd2", i,e, "fgd2reaction" etc.
  // We could directly set which FGD in the usual categories, but this cannot be done for the true vertices
  anaUtils::FillCategories(&GetEvent(), static_cast<AnaTrack*>(box().MainTrack), "fgd2", SubDetId::kFGD2,IsAntinu);

}

