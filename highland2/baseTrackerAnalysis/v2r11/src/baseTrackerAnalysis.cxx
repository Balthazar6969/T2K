#include "baseTrackerAnalysis.hxx"

#include "BFieldDistortionSystematics.hxx"
#include "MomentumScaleSystematics.hxx"
#include "MomentumResolSystematics.hxx"
#include "MomRangeResolSystematics.hxx"
#include "MomBiasFromVertexMigrationSystematics.hxx"

#include "TPCPIDSystematics.hxx"
#include "TPCClusterEffSystematics.hxx"
#include "FGDPIDSystematics.hxx"

#include "PileUpSystematics.hxx"
#include "FGDMassSystematics.hxx"
#include "OOFVSystematics.hxx"
#include "SandMuonsSystematics.hxx"

// ECal EM
#include "ECalEMEnergyResolSystematics.hxx"
#include "ECalEMEnergyScaleSystematics.hxx"

// ToF
#include "ToFResolSystematics.hxx"

// nue Pile-up systematics
#include "nueP0DPileUpSystematics.hxx"
#include "nueTPCPileUpSystematics.hxx"
#include "nueECalPileUpSystematics.hxx"
#include "nueOOFVSystematics.hxx"

//P0D Systematics stuff
#include "P0DVetoSystematics.hxx"
#include "P0DELossScaleSystematics.hxx"
#include "P0DELossResolSystematics.hxx"

// TPC PID Corrections
#include "TPCdEdxDataCorrection.hxx"
#include "TPCdEdxMCCorrection.hxx"
#include "TPCExpecteddEdxCorrection.hxx"
#include "TPCPIDPullMCCorrection.hxx"

#include "PileUpCorrection.hxx"
#include "MomResolMCCorrection.hxx"

#include "FlipKinematicsCorrection.hxx"
#include "ToFCorrection.hxx"
#include "MomRangeCorrection.hxx"

#include "baseToyMaker.hxx"

//********************************************************************
baseTrackerAnalysis::baseTrackerAnalysis(AnalysisAlgorithm* ana): baseAnalysis(ana){
//********************************************************************

  _chargeid               = NULL;
  _tpcfgdmatch            = NULL;
  _tpctr                  = NULL;
  _fgdtr                  = NULL;
  _fgdhybtr               = NULL;
  _me                     = NULL;
  _tpc_ecal_matcheff      = NULL; 
  _tpc_p0d_matcheff       = NULL;
  _fgd_ecal_matcheff      = NULL;
  _fgd_ecal_smrd_matcheff = NULL;
  _ecal_trackeff          = NULL;
  _ecal_pid               = NULL;
  _fgd2shower             = NULL;
  
  // Add the package version
  ND::versioning().AddPackage("baseTrackerAnalysis", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("BASETRACKERANALYSISROOT")));

  // Don't compute efficiencies by default
  _computeEfficiency = false;
}

//********************************************************************
void baseTrackerAnalysis::DefineSystematics(){
//********************************************************************

  //------------ Add Variation systematics ------------

  //! [baseTrackerAnalysis_varsyst]
  evar().AddEventVariation(SystId::kBFieldDist,                 "BFieldDist",                 new BFieldDistortionSystematics());
  evar().AddEventVariation(SystId::kMomScale,                   "MomScale",                   new MomentumScaleSystematics());
  evar().AddEventVariation(SystId::kMomResol,                   "MomResol",                   new MomentumResolSystematics());
  evar().AddEventVariation(SystId::kMomRangeResol,              "MomRangeResol",              new MomRangeResolSystematics());
  evar().AddEventVariation(SystId::kMomBiasFromVertexMigration, "MomBiasFromVertexMigration", new MomBiasFromVertexMigrationSystematics());
  evar().AddEventVariation(SystId::kTpcPid,                     "TpcPid",                     new TPCPIDSystematics());
  evar().AddEventVariation(SystId::kFgdPid,                     "FgdPid",                     new FGDPIDSystematics());
  //! [baseTrackerAnalysis_varsyst]
  
  evar().AddEventVariation(SystId::kECalEMResol,                "ECalEMResol",                new ECalEMEnergyResolSystematics());
  evar().AddEventVariation(SystId::kECalEMScale,                "ECalEMScale",                new ECalEMEnergyScaleSystematics());

  evar().AddEventVariation(SystId::kToFResol,                   "ToFResol",                   new ToFResolSystematics());
  evar().AddEventVariation(SystId::kP0DELossScale,              "P0DELossScale",              new P0DELossScaleSystematics());
  evar().AddEventVariation(SystId::kP0DELossResol,              "P0DELossResol",              new P0DELossResolSystematics());

  
  //-------------- Add Event Weights ------------------

  // compute efficiency using truth for eff-like systematics
  _computeEfficiency =  (bool)ND::params().GetParameterI("baseTrackerAnalysis.Systematics.ComputeEfficiency"); 

  _chargeid                 = new ChargeIDEffSystematics(         _computeEfficiency);
  _tpcfgdmatch              = new TPCFGDMatchEffSystematics(      _computeEfficiency);
  _tpctr                    = new TPCTrackEffSystematics(         _computeEfficiency);
  _fgdtr                    = new FGDTrackEffSystematics(         _computeEfficiency);
  _fgdhybtr                 = new FGDHybridTrackEffSystematics(   _computeEfficiency);
  _me                       = new MichelElectronEffSystematics(   _computeEfficiency);
  _tpc_ecal_matcheff        = new TPCECalMatchEffSystematics(     _computeEfficiency);
  _tpc_p0d_matcheff         = new TPCP0DMatchEffSystematics(      _computeEfficiency);
  _fgd_ecal_matcheff        = new FGDECalMatchEffSystematics(     _computeEfficiency);
  _fgd_ecal_smrd_matcheff   = new FGDECalSMRDMatchEffSystematics( _computeEfficiency);
  _ecal_trackeff            = new ECalTrackEffSystematics(        _computeEfficiency);
  _ecal_pid                 = new ECalPIDSystematics(             _computeEfficiency);
  _ecal_emhippid            = new ECalEmHipPIDSystematics(        _computeEfficiency);
  _fgd2shower               = new FGD2ShowerSystematics(          _computeEfficiency);
  
  //BELOW the order is the same as in psyche, so that each throws can be exactly the same as in psyche.
  eweight().AddEventWeight(SystId::kChargeIDEff,          "ChargeIDEff",          _chargeid);
  eweight().AddEventWeight(SystId::kTpcClusterEff,        "TpcClusterEff",        new TPCClusterEffSystematics());
  eweight().AddEventWeight(SystId::kTpcTrackEff,          "TpcTrackEff",          _tpctr);
  eweight().AddEventWeight(SystId::kTpcFgdMatchEff,       "TpcFgdMatchEff",       _tpcfgdmatch);
  eweight().AddEventWeight(SystId::kFgdTrackEff,          "FgdTrackEff",          _fgdtr);
  eweight().AddEventWeight(SystId::kFgdHybridTrackEff,    "FgdHybridTrackEff",    _fgdhybtr);
  eweight().AddEventWeight(SystId::kMichelEleEff,         "MichelEleEff",         _me);
  //! [baseTrackerAnalysis_weightsyst]
  eweight().AddEventWeight(SystId::kPileUp,               "PileUp",               new PileUpSystematics());
  eweight().AddEventWeight(SystId::kFgdMass,              "FgdMass",              new FGDMassSystematics());
  eweight().AddEventWeight(SystId::kOOFV,                 "OOFV",                 new OOFVSystematics());
  eweight().AddEventWeight(SystId::kSandMu,               "SandMu",               new SandMuonsSystematics());
  //! [baseTrackerAnalysis_weightsyst]
  eweight().AddEventWeight(SystId::kTpcECalMatchEff,      "TpcECalMatchEff",      _tpc_ecal_matcheff);
  eweight().AddEventWeight(SystId::kTpcP0dMatchEff,       "TpcP0dMatchEff",       _tpc_p0d_matcheff);
  eweight().AddEventWeight(SystId::kFgdECalMatchEff,      "FgdECalMatchEff",      _fgd_ecal_matcheff);
  eweight().AddEventWeight(SystId::kFgdECalSmrdMatchEff,  "FgdECalSMRDMatchEff",  _fgd_ecal_smrd_matcheff);
  eweight().AddEventWeight(SystId::kECalTrackEff,         "ECalTrackEff",         _ecal_trackeff);
  eweight().AddEventWeight(SystId::kECalPID,              "ECalPID",              _ecal_pid);
  eweight().AddEventWeight(SystId::kECalEmHipPID,         "ECalEmHipPID",         _ecal_emhippid);
  eweight().AddEventWeight(SystId::kFGD2Shower,           "Fgd2Shower",           _fgd2shower);
  eweight().AddEventWeight(SystId::kNuETPCPileUp,         "nueTPCPileup",         new nueTPCPileUpSystematics());
  eweight().AddEventWeight(SystId::kNuEP0DPileUp,         "nueP0DPileup",         new nueP0DPileUpSystematics());
  eweight().AddEventWeight(SystId::kNuEECalPileUp,        "nueEcalPileup",        new nueECalPileUpSystematics());
  eweight().AddEventWeight(SystId::kNuEOOFV,              "nueOOFV",              new nueOOFVSystematics());
  eweight().AddEventWeight(SystId::kP0DVeto,              "P0DVeto",              new P0DVetoSystematics());
  
  // This must be called here (after adding all other systematics) such that the order in which they are added is the same as before (in baseAnalysis). 
  // Otherwise the random throws will be different. TODO. Is this still true ?
  baseAnalysis::DefineSystematics();
}

//********************************************************************
void baseTrackerAnalysis::DefineCorrections(){
//********************************************************************

  baseAnalysis::DefineCorrections();

  //----------- Define all corrections ----------------
  if (ND::params().GetParameterI("baseTrackerAnalysis.Corrections.DisableAllCorrections")){
    // Should not be needed, but just in case !!!
    corr().DisableAllCorrections();
  }
  else{
    // Add corrections only when they are enabled. In that way the CorrectionManager does not have to loop over unused corrections
    if ((bool)ND::params().GetParameterI("baseTrackerAnalysis.Corrections.EnablePileUp"))          corr().AddCorrection("pileup_corr",          new PileUpCorrection());
    if ((bool)ND::params().GetParameterI("baseTrackerAnalysis.Corrections.EnableTPCdEdxData"))     corr().AddCorrection("tpcdedx_data_corr",    new TPCdEdxDataCorrection());
    if ((bool)ND::params().GetParameterI("baseTrackerAnalysis.Corrections.EnableTPCdEdxMC"))       corr().AddCorrection("tpcdedx_mc_corr",      new TPCdEdxMCCorrection());
    if ((bool)ND::params().GetParameterI("baseTrackerAnalysis.Corrections.EnableTPCExpecteddEdx")) corr().AddCorrection("tpcexpecteddedx_corr", new TPCExpecteddEdxCorrection());
    
    // TPC PID correction based on pull params smearing
    if ((bool)ND::params().GetParameterI("baseTrackerAnalysis.Corrections.EnableTPCPIDPullMC")) corr().AddCorrection("tpc_pid_pull_mc_corr", new TPCPIDPullMCCorrection());
    
    // Smear MC momentum to match resolution in data
    if ((bool)ND::params().GetParameterI("baseTrackerAnalysis.Corrections.EnableMomResolMC"))   corr().AddCorrection("mom_resol_mc_corr",    new MomResolMCCorrection());
   
    // Need to be called before the ToF in order to correctly flip a track if required
    if ((bool)ND::params().GetParameterI("baseTrackerAnalysis.Corrections.EnableFlipKinematicsCorrection")) 
      corr().AddCorrection("flipkinematics_corr", new FlipKinematicsCorrection());
    
    if ((bool)ND::params().GetParameterI("baseTrackerAnalysis.Corrections.EnableToFCorrection"))
      // need to make sure that the kinematics is ready for flip, i.e. alternate reverse momentum was fixed 
      corr().AddCorrection("tof_corr", new ToFCorrection());
  
    if ((bool)ND::params().GetParameterI("baseTrackerAnalysis.Corrections.EnableMomRangeCorrection"))
      corr().AddCorrection("momrange_corr", new MomRangeCorrection());
  
  
  }
}

//********************************************************************
void baseTrackerAnalysis::DefineConfigurations(){
//********************************************************************

  // Define all configuration of the base package
  baseAnalysis::DefineConfigurations();  

  //-------  Add and define individual configurations with one systematic only ------------------

   if (_enableSingleVariationSystConf){
    if (ND::params().GetParameterI("baseTrackerAnalysis.Variations.EnableBFieldDist")){
      AddConfiguration(conf(), bfield_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventVariation(SystId::kBFieldDist,bfield_syst);
    }
    if (ND::params().GetParameterI("baseTrackerAnalysis.Variations.EnableMomScale")){
      AddConfiguration(conf(), momscale_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventVariation(SystId::kMomScale,momscale_syst);
    }
    if (ND::params().GetParameterI("baseTrackerAnalysis.Variations.EnableMomRes")){
      AddConfiguration(conf(), momresol_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventVariation(SystId::kMomResol,momresol_syst);
    }
    if (ND::params().GetParameterI("baseTrackerAnalysis.Variations.EnableMomRangeRes")){
      AddConfiguration(conf(), momrange_resol_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventVariation(SystId::kMomRangeResol,momrange_resol_syst);
    }
    if (ND::params().GetParameterI("baseTrackerAnalysis.Variations.EnableMomBiasFromVertexMigration")){
      AddConfiguration(conf(), mombias_from_vertex_migration_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventVariation(SystId::kMomBiasFromVertexMigration, mombias_from_vertex_migration_syst);
    }
    if (ND::params().GetParameterI("baseTrackerAnalysis.Variations.EnableTPCPID")){
      AddConfiguration(conf(), tpcpid_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventVariation(SystId::kTpcPid,tpcpid_syst);
    }
    if (ND::params().GetParameterI("baseTrackerAnalysis.Variations.EnableFGDPID")){
      AddConfiguration(conf(), fgdpid_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventVariation(SystId::kFgdPid, fgdpid_syst);
    }    
    if (ND::params().GetParameterI("baseTrackerAnalysis.Variations.EnableECalEMResol")){
      AddConfiguration(conf(), ecal_emresol_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventVariation(SystId::kECalEMResol, ecal_emresol_syst);
    }    
    if (ND::params().GetParameterI("baseTrackerAnalysis.Variations.EnableECalEMScale")){
      AddConfiguration(conf(), ecal_emscale_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventVariation(SystId::kECalEMScale, ecal_emscale_syst);
    }    
    if (ND::params().GetParameterI("baseTrackerAnalysis.Variations.EnableToFResol")){
      AddConfiguration(conf(), tof_resol_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventVariation(SystId::kToFResol, tof_resol_syst);
    }
    if (ND::params().GetParameterI("baseTrackerAnalysis.Variations.EnableP0DELossScale")){
      AddConfiguration(conf(), p0d_elossscale_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventVariation(SystId::kP0DELossScale, p0d_elossscale_syst);
    }
    if (ND::params().GetParameterI("baseTrackerAnalysis.Variations.EnableP0DELossResol")){
      AddConfiguration(conf(), p0d_elossresol_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventVariation(SystId::kP0DELossResol, p0d_elossresol_syst);
    }
  }

  if (_enableSingleWeightSystConf){
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableChargeConf")){
      AddConfiguration(conf(), chargeideff_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventWeight(SystId::kChargeIDEff, chargeideff_syst);
    }
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableTPCTrackEff")){
      AddConfiguration(conf(), tpctrackeff_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventWeight(SystId::kTpcTrackEff,tpctrackeff_syst);
    }
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableFGDTrackEff")){
      AddConfiguration(conf(), fgdtrackeff_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventWeight(SystId::kFgdTrackEff,fgdtrackeff_syst);
    }
      if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableFGDHybridTrackEff")){
      AddConfiguration(conf(), fgdhybridtrackeff_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventWeight(SystId::kFgdHybridTrackEff,fgdhybridtrackeff_syst);
    }
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableTPCFGDMatchEff")){
      AddConfiguration(conf(), tpcfgdmatcheff_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventWeight(SystId::kTpcFgdMatchEff,tpcfgdmatcheff_syst);
    }
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableOOFV")){
      AddConfiguration(conf(), oofv_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventWeight(SystId::kOOFV,oofv_syst);
    }
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnablePileUp")){
      AddConfiguration(conf(), pileup_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventWeight(SystId::kPileUp,pileup_syst);
    }
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableFGDMass")){
      AddConfiguration(conf(), fgdmass_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventWeight(SystId::kFgdMass,fgdmass_syst);
    }
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableMichelEleEff")){
      AddConfiguration(conf(), michel_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventWeight(SystId::kMichelEleEff,michel_syst);
    }
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableSandMu")){
      AddConfiguration(conf(), sandmu_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventWeight(SystId::kSandMu,sandmu_syst);
    }
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableTPCClusterEff")){
      AddConfiguration(conf(), tpcclustereff_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventWeight(SystId::kTpcClusterEff,tpcclustereff_syst);
    }
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableTPCECalMatchEff")){
      AddConfiguration(conf(), tpc_ecal_matcheff_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventWeight(SystId::kTpcECalMatchEff, tpc_ecal_matcheff_syst);
    }
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableTPCP0DMatchEff")){
      AddConfiguration(conf(), tpc_p0d_matcheff_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventWeight(SystId::kTpcP0dMatchEff, tpc_p0d_matcheff_syst);
    }
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableFGDECalMatchEff")){
      AddConfiguration(conf(), fgd_ecal_matcheff_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventWeight(SystId::kFgdECalMatchEff, fgd_ecal_matcheff_syst);
    }
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableFGDECalSMRDMatchEff")){
      AddConfiguration(conf(), fgd_ecal_smrd_matcheff_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventWeight(SystId::kFgdECalSmrdMatchEff, fgd_ecal_smrd_matcheff_syst);
    }
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableECalTrackEff")){
      AddConfiguration(conf(), ecal_trackeff_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventWeight(SystId::kECalTrackEff, ecal_trackeff_syst);
    }
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableECalPID")){
      AddConfiguration(conf(), ecal_pid_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventWeight(SystId::kECalPID, ecal_pid_syst);
    }
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableECalEmHipPID")){
      AddConfiguration(conf(), ecal_emhippid_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventWeight(SystId::kECalEmHipPID, ecal_emhippid_syst);
    }
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableFGD2Shower")){
      AddConfiguration(conf(), fgd2shower_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventWeight(SystId::kFGD2Shower, fgd2shower_syst);
    }
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableNuETPCPileUp")){
      AddConfiguration(conf(), nuetpcpileup_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventWeight(SystId::kNuETPCPileUp, nuetpcpileup_syst);
    }
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableNuEP0DPileUp")){
      AddConfiguration(conf(), nuep0dpileup_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventWeight(SystId::kNuEP0DPileUp, nuep0dpileup_syst);
    }
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableNuEECalPileUp")){
      AddConfiguration(conf(), nueecalpileup_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventWeight(SystId::kNuEECalPileUp, nueecalpileup_syst);
    }
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableNuEOOFV")){
      AddConfiguration(conf(), nueoofv_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventWeight(SystId::kNuEOOFV,nueoofv_syst);
    }
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableP0DVeto")){
      AddConfiguration(conf(), p0d_veto_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventWeight(SystId::kP0DVeto,p0d_veto_syst);
    }
  }

  //! [baseTrackerAnalysis_all_syst]
  // Enable all variation systematics in the all_syst configuration (created in baseAnalysis)
  if ( _enableAllSystConfig){
    if (ND::params().GetParameterI("baseTrackerAnalysis.Variations.EnableBFieldDist"))                 conf().EnableEventVariation(SystId::kBFieldDist                 , all_syst);
    if (ND::params().GetParameterI("baseTrackerAnalysis.Variations.EnableMomRes"))                     conf().EnableEventVariation(SystId::kMomResol                   , all_syst);
    if (ND::params().GetParameterI("baseTrackerAnalysis.Variations.EnableMomRangeRes"))                conf().EnableEventVariation(SystId::kMomRangeResol              , all_syst);
    if (ND::params().GetParameterI("baseTrackerAnalysis.Variations.EnableMomScale"))                   conf().EnableEventVariation(SystId::kMomScale                   , all_syst);
    if (ND::params().GetParameterI("baseTrackerAnalysis.Variations.EnableMomBiasFromVertexMigration")) conf().EnableEventVariation(SystId::kMomBiasFromVertexMigration , all_syst);
    if (ND::params().GetParameterI("baseTrackerAnalysis.Variations.EnableTPCPID"))                     conf().EnableEventVariation(SystId::kTpcPid                     , all_syst);
    if (ND::params().GetParameterI("baseTrackerAnalysis.Variations.EnableFGDPID"))                     conf().EnableEventVariation(SystId::kFgdPid                     , all_syst);
    if (ND::params().GetParameterI("baseTrackerAnalysis.Variations.EnableECalEMResol"))                conf().EnableEventVariation(SystId::kECalEMResol                , all_syst);
    if (ND::params().GetParameterI("baseTrackerAnalysis.Variations.EnableECalEMScale"))                conf().EnableEventVariation(SystId::kECalEMScale                , all_syst);
    if (ND::params().GetParameterI("baseTrackerAnalysis.Variations.EnableToFResol"))                   conf().EnableEventVariation(SystId::kToFResol                   , all_syst);
    if (ND::params().GetParameterI("baseTrackerAnalysis.Variations.EnableP0DELossScale"))              conf().EnableEventVariation(SystId::kP0DELossScale              , all_syst);
    if (ND::params().GetParameterI("baseTrackerAnalysis.Variations.EnableP0DELossResol"))              conf().EnableEventVariation(SystId::kP0DELossResol              , all_syst);
  }

  // Enable all Event Weights in the default and all_syst configurations
  for (std::vector<ConfigurationBase* >::iterator it= conf().GetConfigurations().begin();it!=conf().GetConfigurations().end();it++){
    Int_t index = (*it)->GetIndex();
    if (index != ConfigurationManager::default_conf && (index != all_syst || !_enableAllSystConfig)) continue;

    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableChargeConf"))           conf().EnableEventWeight(SystId::kChargeIDEff          , index);     
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnablePileUp"))               conf().EnableEventWeight(SystId::kPileUp               , index);      
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableMichelEleEff"))         conf().EnableEventWeight(SystId::kMichelEleEff         , index);      
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableTPCTrackEff"))          conf().EnableEventWeight(SystId::kTpcTrackEff          , index);      
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableFGDTrackEff"))          conf().EnableEventWeight(SystId::kFgdTrackEff          , index);      
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableFGDHybridTrackEff"))    conf().EnableEventWeight(SystId::kFgdHybridTrackEff    , index);      
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableTPCFGDMatchEff"))       conf().EnableEventWeight(SystId::kTpcFgdMatchEff       , index);      
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableOOFV"))                 conf().EnableEventWeight(SystId::kOOFV                 , index);
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableTPCClusterEff"))        conf().EnableEventWeight(SystId::kTpcClusterEff        , index);
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableFGDMass"))              conf().EnableEventWeight(SystId::kFgdMass              , index);      
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableSandMu"))               conf().EnableEventWeight(SystId::kSandMu               , index);
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableTPCECalMatchEff"))      conf().EnableEventWeight(SystId::kTpcECalMatchEff      , index);
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableTPCP0DMatchEff"))       conf().EnableEventWeight(SystId::kTpcP0dMatchEff       , index);
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableFGDECalMatchEff"))      conf().EnableEventWeight(SystId::kFgdECalMatchEff      , index);
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableFGDECalSMRDMatchEff"))  conf().EnableEventWeight(SystId::kFgdECalSmrdMatchEff  , index);
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableECalTrackEff"))         conf().EnableEventWeight(SystId::kECalTrackEff         , index);
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableECalPID"))              conf().EnableEventWeight(SystId::kECalPID              , index); 
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableECalEmHipPID"))         conf().EnableEventWeight(SystId::kECalEmHipPID         , index); 
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableFGD2Shower"))           conf().EnableEventWeight(SystId::kFGD2Shower           , index);
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableNuETPCPileUp"))         conf().EnableEventWeight(SystId::kNuETPCPileUp         , index);
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableNuEP0DPileUp"))         conf().EnableEventWeight(SystId::kNuEP0DPileUp         , index);
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableNuEECalPileUp"))        conf().EnableEventWeight(SystId::kNuEECalPileUp        , index);
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableNuEOOFV"))              conf().EnableEventWeight(SystId::kNuEOOFV              , index);
    if (ND::params().GetParameterI("baseTrackerAnalysis.Weights.EnableP0DVeto"))              conf().EnableEventWeight(SystId::kP0DVeto              , index);
  }
  //! [baseTrackerAnalysis_all_syst]
  
}

//********************************************************************
void baseTrackerAnalysis::DefineMicroTrees(bool addBase){
//********************************************************************

  if (addBase) baseAnalysis::DefineMicroTrees(addBase);

  // -------- Add variables to the analysis tree ----------------------

  AddVarI(output(), ntpctracks,      "number of tracks containing TPC");
  AddVarI(output(), ntpcposQualityFV,"number of positive tracks containing TPC, with good quality and in FV");
  AddVarI(output(), ntpcnegQualityFV,"number of negative tracks containing TPC, with good quality and in FV");
  AddVarI(output(), nfgdtracks,      "number of tracks contining FGD and no TPC");
  AddVarI(output(), nfgdonlytracks,  "number of tracks in FGD only");

  //--- add nb of efficiant/inefficiant tracks that pass systematics cut for each event
  if (_computeEfficiency){
    AddVarFixVI(output(), chargeid_nwrong,              "number of tracks with wrong charge for each systematic bin",                           NMAXSYSTSOURCEBINS);
    AddVarFixVI(output(), chargeid_ncorrect,            "number of tracks with correct charge for each systematic bin",                         NMAXSYSTSOURCEBINS);
    AddVarFixVI(output(), tpcfgdmatch_nwrong,           "number of tracks with wrong tpcfgd match for each systematic bin",                     NMAXSYSTSOURCEBINS);
    AddVarFixVI(output(), tpcfgdmatch_ncorrect,         "number of tracks with correct tpcfgd match for each systematic bin",                   NMAXSYSTSOURCEBINS);
    AddVarFixVI(output(), tpctr_nwrong,                 "number of tracks with wrong tpc tracking for each systematic bin",                     NMAXSYSTSOURCEBINS);
    AddVarFixVI(output(), tpctr_ncorrect,               "number of tracks with correct tpc tracking for each systematic bin",                   NMAXSYSTSOURCEBINS);
    AddVarFixVI(output(), fgdtr_nwrong,                 "number of tracks with wrong fgd tracking for each systematic bin",                     NMAXSYSTSOURCEBINS);
    AddVarFixVI(output(), fgdtr_ncorrect,               "number of tracks with correct fgd tracking for each systematic bin",                   NMAXSYSTSOURCEBINS);
    AddVarFixVI(output(), fgdhybtr_nwrong,              "number of tracks with wrong fgdhyb tracking for each systematic bin",                  NMAXSYSTSOURCEBINS);
    AddVarFixVI(output(), fgdhybtr_ncorrect,            "number of tracks with correct fgdhyb tracking for each systematic bin",                NMAXSYSTSOURCEBINS);
    AddVarFixVI(output(), meeff_nwrong,                 "number of events with true me but no selected me for each systematic bin",             NMAXSYSTSOURCEBINS);
    AddVarFixVI(output(), meeff_ncorrect,               "number of events with true me and selected me for each systematic bin",                NMAXSYSTSOURCEBINS);
    AddVarFixVI(output(), mepur_nwrong,                 "number of events with me found selected when no true me for each systematic bin",      NMAXSYSTSOURCEBINS);
    AddVarFixVI(output(), mepur_ncorrect,               "number of events with me found selected when true me exists for each systematic bin",  NMAXSYSTSOURCEBINS);
    
    AddVarFixVI(output(), tpc_ecal_match_ncorrect,      "number of events with correct tpc-ecal match for each systematic bin",                 NMAXSYSTSOURCEBINS);
    AddVarFixVI(output(), tpc_ecal_match_nwrong,        "number of events with wrong tpc-ecal match for each systematic bin",                   NMAXSYSTSOURCEBINS); 
    
    AddVarFixVI(output(), tpc_p0d_match_ncorrect,       "number of events with correct tpc-p0d match for each systematic bin",                  NMAXSYSTSOURCEBINS);
    AddVarFixVI(output(), tpc_p0d_match_nwrong,         "number of events with wrong tpc-p0d match for each systematic bin",                    NMAXSYSTSOURCEBINS);
    
    AddVarFixVI(output(), fgd_ecal_match_ncorrect,      "number of events with correct fgd-ecal match for each systematic bin",                 NMAXSYSTSOURCEBINS);
    AddVarFixVI(output(), fgd_ecal_match_nwrong,        "number of events with wrong fgd-ecal match for each systematic bin",                   NMAXSYSTSOURCEBINS);
    
    AddVarFixVI(output(), fgd_ecal_smrd_match_ncorrect, "number of events with correct fgd-ecal-smrd match for each systematic bin",            NMAXSYSTSOURCEBINS);
    AddVarFixVI(output(), fgd_ecal_smrd_match_nwrong,   "number of events with wrong fgd-ecal-srmd match for each systematic bin",              NMAXSYSTSOURCEBINS); 
    
    AddVarFixVI(output(), ecal_tr_ncorrect,             "number of tracks with correct ecal tracking for each systematic bin",                  NMAXSYSTSOURCEBINS);
    AddVarFixVI(output(), ecal_tr_nwrong,               "number of tracks with wrong ecal tracking for each systematic bin",                    NMAXSYSTSOURCEBINS); 
  
    AddVarFixVI(output(), ecal_pid_ncorrect,             "number of tracks with correct ecal PID for each systematic bin",                      NMAXSYSTSOURCEBINS);
    AddVarFixVI(output(), ecal_pid_nwrong,               "number of tracks with wrong ecal PID for each systematic bin",                        NMAXSYSTSOURCEBINS);

    AddVarFixVI(output(), ecal_emhippid_ncorrect,        "number of tracks with correct ecal EmHip PID for each systematic bin",                NMAXSYSTSOURCEBINS);
    AddVarFixVI(output(), ecal_emhippid_nwrong,          "number of tracks with wrong ecal EmHip PID for each systematic bin",                  NMAXSYSTSOURCEBINS);

    AddVarFixVI(output(), fgd2shower_ncorrect,           "number of FGD1 tracks that do shower in FGD2 for each systematic bin",                NMAXSYSTSOURCEBINS);
    AddVarFixVI(output(), fgd2shower_nwrong,             "number of FGD1 tracks that do not shower in FGD2 for each systematic bin",            NMAXSYSTSOURCEBINS);

  }
}

//********************************************************************
void baseTrackerAnalysis::DefineTruthTree(){
//********************************************************************

  baseAnalysis::DefineTruthTree();
}

//********************************************************************
void baseTrackerAnalysis::FillMicroTrees(bool addBase){
//********************************************************************

  if (addBase) baseAnalysis::FillMicroTreesBase(addBase);

  // Track counters
  AnaTrackB* dummy[NMAXPARTICLES];
  int nTPC     = anaUtils::GetAllTracksUsingDet(GetBunch(), SubDetId::kTPC,  dummy);
  int nFGD     = anaUtils::GetAllTracksUsingFGDAndNoTPC(GetBunch(), dummy);
  int nFGDonly = anaUtils::GetAllTracksUsingOnlyDet(GetBunch(), SubDetId::kFGD, dummy);

  output().FillVar(ntpctracks,nTPC);
  output().FillVar(nfgdtracks,nFGD);  // tracks with no TPC
  output().FillVar(nfgdonlytracks,nFGDonly);  // tracks with only FGD
  output().FillVar(ntpcnegQualityFV,(Int_t)box().nNegativeTPCtracks);  // Tracks containing TPC, with good quality and in FV
  output().FillVar(ntpcposQualityFV,(Int_t)box().nPositiveTPCtracks);  // Tracks containing TPC, with good quality and in FV

  // Counters needed to compute efficiencies elated with systematics
  if (_computeEfficiency){
    for (UInt_t i=0;i<NMAXSYSTSOURCEBINS;i++){
      output().FillVectorVar(chargeid_ncorrect,             _chargeid->_globalCharge->GetNCorrectAssoc(i),    i);
      output().FillVectorVar(chargeid_nwrong,               _chargeid->_globalCharge->GetNWrongAssoc(i),      i);
      output().FillVectorVar(tpcfgdmatch_ncorrect,          _tpcfgdmatch->GetNCorrectAssoc(i),                i);
      output().FillVectorVar(tpcfgdmatch_nwrong,            _tpcfgdmatch->GetNWrongAssoc(i),                  i);
      output().FillVectorVar(tpctr_ncorrect,                _tpctr->GetNCorrectAssoc(i),                      i);
      output().FillVectorVar(tpctr_nwrong,                  _tpctr->GetNWrongAssoc(i),                        i);
      output().FillVectorVar(fgdtr_ncorrect,                _fgdtr->_fgd1->GetNCorrectAssoc(i),               i);
      output().FillVectorVar(fgdtr_nwrong,                  _fgdtr->_fgd1->GetNWrongAssoc(i),                 i);
      output().FillVectorVar(fgdhybtr_ncorrect,             _fgdhybtr->_fgd1->GetNCorrectAssoc(i),            i);
      output().FillVectorVar(fgdhybtr_nwrong,               _fgdhybtr->_fgd1->GetNWrongAssoc(i),              i);
      output().FillVectorVar(meeff_ncorrect,                _me->_fgd1eff->GetNCorrectAssoc(i),               i);
      output().FillVectorVar(meeff_nwrong,                  _me->_fgd1eff->GetNWrongAssoc(i),                 i);
      output().FillVectorVar(mepur_ncorrect,                _me->_fgd1pur->GetNCorrectAssoc(i),               i);
      output().FillVectorVar(mepur_nwrong,                  _me->_fgd1pur->GetNWrongAssoc(i),                 i);
      
      output().FillVectorVar(tpc_ecal_match_ncorrect,       _tpc_ecal_matcheff->GetNCorrectAssoc(i),        i);
      output().FillVectorVar(tpc_ecal_match_nwrong,         _tpc_ecal_matcheff->GetNWrongAssoc(i),          i);
      
      output().FillVectorVar(tpc_p0d_match_ncorrect,        _tpc_p0d_matcheff->GetNCorrectAssoc(i),         i);
      output().FillVectorVar(tpc_p0d_match_nwrong,          _tpc_p0d_matcheff->GetNWrongAssoc(i),           i);
      
      output().FillVectorVar(fgd_ecal_match_ncorrect,       _fgd_ecal_matcheff->GetNCorrectAssoc(i),        i);
      output().FillVectorVar(fgd_ecal_match_nwrong,         _fgd_ecal_matcheff->GetNWrongAssoc(i),          i);
      
      output().FillVectorVar(fgd_ecal_smrd_match_ncorrect,  _fgd_ecal_smrd_matcheff->GetNCorrectAssoc(i),   i);
      output().FillVectorVar(fgd_ecal_smrd_match_nwrong,    _fgd_ecal_smrd_matcheff->GetNWrongAssoc(i),     i);
      
      output().FillVectorVar(ecal_tr_ncorrect,              _ecal_trackeff->GetNCorrectAssoc(i),            i);
      output().FillVectorVar(ecal_tr_nwrong,                _ecal_trackeff->GetNWrongAssoc(i),              i);
      
      output().FillVectorVar(ecal_pid_ncorrect,             _ecal_pid->GetNCorrectAssoc(i),                 i);
      output().FillVectorVar(ecal_pid_nwrong,               _ecal_pid->GetNWrongAssoc(i),                   i);

      output().FillVectorVar(ecal_emhippid_ncorrect,        _ecal_emhippid->GetNCorrectAssoc(i),            i);
      output().FillVectorVar(ecal_emhippid_nwrong,          _ecal_emhippid->GetNWrongAssoc(i),              i);

      output().FillVectorVar(fgd2shower_ncorrect,           _fgd2shower->GetNCorrectAssoc(i),               i);
      output().FillVectorVar(fgd2shower_nwrong,             _fgd2shower->GetNWrongAssoc(i),                 i);
      
    }
  }

}

//********************************************************************
void baseTrackerAnalysis::FillToyVarsInMicroTrees(bool addBase){
//********************************************************************

  if (addBase) baseAnalysis::FillToyVarsInMicroTreesBase(addBase);
}

//********************************************************************
void baseTrackerAnalysis::FillTruthTreeBase(const AnaTrueVertex& vtx, const SubDetId::SubDetEnum det, bool IsAntinu){
//********************************************************************

  // Fill the common variables
  baseAnalysis::FillTruthTreeBase(vtx, det, IsAntinu);
}
