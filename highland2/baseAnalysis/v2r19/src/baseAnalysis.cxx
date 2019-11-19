#include "baseAnalysis.hxx"

#include "IgnoreRightECalRuns3and4Correction.hxx"
#include "CT4POTCorrection.hxx"
#include "DataQualityCorrection.hxx"

#include "SIPionSystematics.hxx"
#include "SIProtonSystematics.hxx"
#include "FluxWeightSystematics.hxx"

#include "FlatTreeConverter.hxx"
#include "HighlandMiniTreeConverter.hxx"
#include "oaAnalysisTreeConverter.hxx"

#include "baseToyMaker.hxx"

#include "ConfigTreeTools.hxx"
#include "NuDirUtils.hxx"

//********************************************************************
baseAnalysis::baseAnalysis(AnalysisAlgorithm* ana): AnalysisAlgorithm(ana){
  //********************************************************************

  // Add the package version
  // Add package versions
  ND::versioning().AddPackage("psychePolicy",        anaUtils::GetSoftwareVersionFromPath((std::string)getenv("PSYCHEPOLICYROOT")));
  ND::versioning().AddPackage("psycheEventModel",    anaUtils::GetSoftwareVersionFromPath((std::string)getenv("PSYCHEEVENTMODELROOT")));
  ND::versioning().AddPackage("psycheCore",          anaUtils::GetSoftwareVersionFromPath((std::string)getenv("PSYCHECOREROOT")));
  ND::versioning().AddPackage("psycheUtils",         anaUtils::GetSoftwareVersionFromPath((std::string)getenv("PSYCHEUTILSROOT")));
  ND::versioning().AddPackage("psycheND280Utils",    anaUtils::GetSoftwareVersionFromPath((std::string)getenv("PSYCHEND280UTILSROOT")));
  ND::versioning().AddPackage("psycheIO",            anaUtils::GetSoftwareVersionFromPath((std::string)getenv("PSYCHEIOROOT")));
  ND::versioning().AddPackage("psycheSelections",    anaUtils::GetSoftwareVersionFromPath((std::string)getenv("PSYCHESELECTIONSROOT")));
  ND::versioning().AddPackage("psycheSystematics",   anaUtils::GetSoftwareVersionFromPath((std::string)getenv("PSYCHESYSTEMATICSROOT")));
  ND::versioning().AddPackage("highlandEventModel",  anaUtils::GetSoftwareVersionFromPath((std::string)getenv("HIGHLANDEVENTMODELROOT")));
  ND::versioning().AddPackage("highlandTools",       anaUtils::GetSoftwareVersionFromPath((std::string)getenv("HIGHLANDTOOLSROOT")));
  ND::versioning().AddPackage("highlandCore",        anaUtils::GetSoftwareVersionFromPath((std::string)getenv("HIGHLANDCOREROOT")));
  ND::versioning().AddPackage("highlandCorrections", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("HIGHLANDCORRECTIONSROOT")));
  ND::versioning().AddPackage("highlandIO",          anaUtils::GetSoftwareVersionFromPath((std::string)getenv("HIGHLANDIOROOT")));

  ND::versioning().AddPackage("baseAnalysis", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("BASEANALYSISROOT")));

  /// TODO
  versionUtils::prod6_POT    = false;
#if VERSION_HAS_OFFICIAL_POT
  versionUtils::prod6_POT    = true;
#endif


  _rooVtxManager=NULL;
}

//********************************************************************
void baseAnalysis::DefineProductions(){
//********************************************************************

  anaUtils::AddStandardProductions();  
}

//********************************************************************
void baseAnalysis::DefineInputConverters(){
//********************************************************************

  // Read or not the RooTrackerVtxTree
  bool readRooTrackerVtxTree = (bool)ND::params().GetParameterI("baseAnalysis.ReadRooTrackerVtxTree");

  // add the different converters
  input().AddConverter("FlatTree",       new FlatTreeConverter(readRooTrackerVtxTree));
  input().AddConverter("MiniTree",       new HighlandMiniTreeConverter(readRooTrackerVtxTree));
  input().AddConverter("oaAnalysisTree", new oaAnalysisTreeConverter());  

}

//********************************************************************
bool baseAnalysis::Initialize(){
  //********************************************************************
 
  
  return true;
}

//********************************************************************
bool baseAnalysis::InitializeBase(){
//********************************************************************


  /*
    This method should not be overrwritten by derived classes. In this way we make sure the implenentation in baseAnalysis is always called by AnalysisLoop. 
    This method is called after the Initialize() method, which can be overwritted by derived classes
   */

  // Flux weight pointer
  _flux         = NULL;

  // Initialize trees or not at the beginnng of each configuration
  SetInitializeTrees(ND::params().GetParameterI("baseAnalysis.InitializeTrees"));
    
  if (_versionCheck){
    if(!ND::versioning().CheckVersionCompatibility(ND::versioning().GetProduction(input().GetSoftwareVersion()),
                                                   anaUtils::GetProductionIdFromoaAnalysisReader())) return false;
  }

  // Select the production based on the software version of the input file. This will be used to select the appropriate corrections, 
  // bunching, systematics, etc. Assume production 5 when no software version exists
  versionUtils::prod6_bunching    = false;
  versionUtils::prod6_corrections = false;
  versionUtils::prod6_systematics = false;

  if (_versionCheck){
    if (ND::versioning().GetProduction(input().GetSoftwareVersion()) >= ProdId::PROD6PRE){
      versionUtils::prod6_bunching    = true;
      versionUtils::prod6_corrections = true;
      versionUtils::prod6_systematics = true;
    }
  }

  // Select the production from the parameters file. This will be used for bunching, ...
  if (ND::params().GetParameterI("baseAnalysis.Bunching.Production")==6){ 
    versionUtils::prod6_bunching = true;
    std::cout << "WARNING: production has been overwritten by parameter baseAnalysis.Bunching.Production == 6" << std::endl;
  }
  else if (ND::params().GetParameterI("baseAnalysis.Bunching.Production")==5){ 
    versionUtils::prod6_bunching = false;
    std::cout << "WARNING: production has been overwritten by parameter baseAnalysis.Bunching.Production == 5" << std::endl;
  }

  // Select the production from the parameters file. This will be used for corrections
  if (ND::params().GetParameterI("baseAnalysis.Corrections.Production")==6){ 
    versionUtils::prod6_corrections = true;
    std::cout << "WARNING: production has been overwritten by parameter baseAnalysis.Corrections.Production == 6" << std::endl;
  }
  else if (ND::params().GetParameterI("baseAnalysis.Corrections.Production")==5){ 
    versionUtils::prod6_corrections = false;
    std::cout << "WARNING: production has been overwritten by parameter baseAnalysis.Corrections.Production == 5" << std::endl;
  }

  // Select the production from the parameters file. This will be used for systematics
  if (ND::params().GetParameterI("baseAnalysis.Systematics.Production")==6){ 
    versionUtils::prod6_systematics = true;
    std::cout << "WARNING: production has been overwritten by parameter baseAnalysis.Systematics.Production == 6" << std::endl;
  }
  else if (ND::params().GetParameterI("baseAnalysis.Systematics.Production")==5){ 
    versionUtils::prod6_systematics = false;
    std::cout << "WARNING: production has been overwritten by parameter baseAnalysis.Systematics.Production == 5" << std::endl;
  }

  // Dump the production used for corrections, bunching, systematics, etc
  versionUtils::DumpProductions();

  // This will take care of data/MC differences in detector volumes definitions 	 
  // Should be applied after the version has been defined
  // TODO. Moved temporarily from AnalysisLoop to avoid dependency of highlandCore on psycheND280Utils
  ND::hgman().InitializeGeometry(input().GetIsMC()); 

  
  // Save or not the RooTrackerVtxTree
  _saveRooTrackerVtxTree = (bool)ND::params().GetParameterI("baseAnalysis.SaveRooTrackerVtxTree");
  
  // Create a RooTrackerVtxManager
  _rooVtxManager = new RooTrackerVtxManager();
  
  // Set this parameter to all used algorithms
  SetSaveRooTracker(_saveRooTrackerVtxTree);

  // Use the same manager for all used algorithms
  SetRooVtxManager(_rooVtxManager);

  // Read and set the neutrino position variables 
  anaUtils::InitializeNeutrinoBeamLineParameters();
  
  return true;
}

//********************************************************************
void baseAnalysis::DefineSystematics(){
//********************************************************************

  // We add here EventWeights and EventVariations that are common to all analyses

  eweight().AddEventWeight(SystId::kSIPion,           "SIPion",           new SIPionSystematics());
  eweight().AddEventWeight(SystId::kSIProton,         "SIProton",         new SIProtonSystematics());

  eweight().AddEventWeight(SystId::kFluxWeight,     "FluxWeight",       new FluxWeightSystematics());
}

//********************************************************************
void baseAnalysis::DefineCorrections(){
  //********************************************************************

  //----------- Define all corrections ----------------
  if (ND::params().GetParameterI("baseAnalysis.Corrections.DisableAllCorrections")){
    // Should not be needed, but just in case !!!
    corr().DisableAllCorrections();
  }
  else{
    // Add corrections only when they are enabled. In that way the CorrectionManager does not have to loop over unused corrections

    // Ignore right ECal for runs 3 and 4 as part of it is broken.
    if (ND::params().GetParameterI("baseAnalysis.Corrections.EnableIgnoreRightECal"))  corr().AddCorrection("ignorerightecal_corr", new IgnoreRightECalRuns3and4Correction());     
    // Correct the data quality in periods when a FGD FEB wasn't working.
    if (ND::params().GetParameterI("baseAnalysis.Corrections.EnableDQ"))               corr().AddCorrection("dq_corr",              new DataQualityCorrection());   


#if !VERSION_HAS_OFFICIAL_POT
    // Use CT4 for MR44 POT accounting, as CT5 wasn't working properly. Only need this correction for P5 files.
    if (ND::params().GetParameterI("baseAnalysis.Corrections.EnableCT4POT"))          corr().AddCorrection("ct4pot_corr",          new CT4POTCorrection());
#endif

  } 

  // if (ND::params().GetParameterI("baseAnalysis.Extensions.EcalPID")) {
  //  // ECal EM energy correction only applies if we've calculated the
  //  // ECal EM energy variables and put them into the extended
  //  // AnaECALParticleEcalPID class.
  //  corr().AddCorrection("ecalemene_corr", new EcalEMEnergyCorrection());
  //}


  // The Flux Weight is a correction, but for the moment is not defined as such (TODO). Keep it here
  _flux= NULL;
  if ((bool)ND::params().GetParameterI("baseAnalysis.FluxWeighting.Enable")){
    _flux = new FluxWeighting();
  }

  // If set to true corrections applied in the input file are applied again
  corr().SetForceApplyCorrections((bool)ND::params().GetParameterI("baseAnalysis.Corrections.ForceApplyCorrections"));
}

//********************************************************************
void baseAnalysis::DefineConfigurations(){
  //********************************************************************

  //-------  Add and define individual configurations with one systematic only ------------------

  _enableSingleVariationSystConf = (bool)ND::params().GetParameterI("baseAnalysis.Configurations.EnableSingleVariationSystConfigurations");
  _enableSingleWeightSystConf    = (bool)ND::params().GetParameterI("baseAnalysis.Configurations.EnableSingleWeightSystConfigurations");
  _enableAllSystConfig           = (bool)ND::params().GetParameterI("baseAnalysis.Configurations.EnableAllSystematics");

  _ntoys = (Int_t)ND::params().GetParameterI("baseAnalysis.Systematics.NumberOfToys");   // The number of Toy Experiments 
  _randomSeed = (Int_t)ND::params().GetParameterI("baseAnalysis.Systematics.RandomSeed");   // The random seed to generate the ToyExperiments

  if (_enableSingleWeightSystConf){
    if (ND::params().GetParameterI("baseAnalysis.Weights.EnableSIPion")){
      AddConfiguration(conf(), sipion_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventWeight(SystId::kSIPion,sipion_syst);
    }
    if (ND::params().GetParameterI("baseAnalysis.Weights.EnableSIProton")){
      AddConfiguration(conf(), siproton_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventWeight(SystId::kSIProton,siproton_syst);
    }
    if (ND::params().GetParameterI("baseAnalysis.Weights.EnableFlux")) {
      AddConfiguration(conf(), nuflux_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
      conf().EnableEventWeight(SystId::kFluxWeight    , nuflux_syst);
    }
  }

  // A configuration with all systematics
  if (_enableAllSystConfig){ 
    AddConfiguration(conf(), all_syst, _ntoys, _randomSeed, new baseToyMaker(_randomSeed));
  }

  // Enable all Event Weights in the default and all_syst configurations
  for (std::vector<ConfigurationBase* >::iterator it= conf().GetConfigurations().begin();it!=conf().GetConfigurations().end();it++){
    Int_t index = (*it)->GetIndex();
    if (index != ConfigurationManager::default_conf && (index != all_syst || !_enableAllSystConfig)) continue;
    if (ND::params().GetParameterI("baseAnalysis.Weights.EnableSIPion"))        conf().EnableEventWeight(SystId::kSIPion    , index);
    if (ND::params().GetParameterI("baseAnalysis.Weights.EnableSIProton"))      conf().EnableEventWeight(SystId::kSIProton  , index);
    if (ND::params().GetParameterI("baseAnalysis.Weights.EnableFlux"))          conf().EnableEventWeight(SystId::kFluxWeight, index);
  }

}

//********************************************************************
void baseAnalysis::DefineMicroTrees(bool addBase){
  //********************************************************************

  (void)addBase;

  // -------- Add variables to the analysis tree ----------------------

  //--- event variables -------
  AddVarI(output(), run,      "run number");
  AddVarI(output(), subrun,   "subrun number ");
  AddVarI(output(), evt,      "event number ");
  AddVarI(output(), evt_time, "event time MCM"); // highlandEventModel
  AddVarI(output(), bunch,    "bunch number ");

  //--- link with truth ----
  AddToyVarI(output(), TruthVertexID,"The unique ID of the true vertex associated to the selected event");
  AddToyVarI(output(), RooVtxIndex,  "The index of the associated RooTrackerVtx vertex from its position in the TClonesArray");
  AddToyVarI(output(), RooVtxEntry,  "The entry of the event in the saved RooTrackerVtx tree");
  AddVarI(output(),    RooVtxEntry2,  "The entry of the event in the saved RooTrackerVtx tree");
  AddVarI(output(),    RooVtxFile,   "The File of the event in the saved RooTrackerVtx tree");

  // --- neutrino truth variables ----
  AddVarI(  output(), nu_pdg,       "neutrino PDG code");
  AddVarF(  output(), nu_trueE,     "true neutrino energy");
  AddVarI(  output(), nu_truereac,  "true netrino reaction code");
  AddVar3VF(output(), nu_truedir,   "true neutrino direction");

  // --- Vertex info
  AddVarI(  output(), selvtx_det,       "detector in which the reconstructed vertex is");
  AddVar4VF(output(), selvtx_pos,       "reconstructed vertex position");
  AddVar4VF(output(), selvtx_truepos,   "position of the true vertex associated to the reconstructed vertex");

  // --- true signal definition ---
  AddToyVarI(output(), true_signal, "true signal appraisal (filled differently in each analysis package)");

  // --- Total systematic without flux systematic in all_syst configuration------
  if (conf().GetConfiguration(all_syst))
    output().AddToyVar(conf().GetConfiguration(all_syst)->GetTreeIndex(), weight_syst_total_noflux, "weight_syst_total_noflux", "F", "Total weight (corr+syst) with no flux systematic");

}

//********************************************************************
void baseAnalysis::DefineTruthTree(){
  //********************************************************************

  // Add a tree to save the truth info as a clone of the NRooTrackerVtx or GRooTrackerVtx tree
  if (_saveRooTrackerVtxTree){
    if (input().GetChain("NRooTrackerVtx"))
      output().AddTreeWithName(OutputManager::RooTrackerVtx, "RooTrackerVtx", input().GetChain("NRooTrackerVtx"));
    else if (input().GetChain("GRooTrackerVtx"))
      output().AddTreeWithName(OutputManager::RooTrackerVtx, "RooTrackerVtx", input().GetChain("GRooTrackerVtx"));
  }
  
  //--- event variables -------
  AddVarI(output(),   evt,    "event number");
  AddVarI(output(),   run,    "run number");
  AddVarI(output(),   subrun, "subrun number");

  // ---- link with ...
  AddVarI(output(),   TruthVertexID, "The unique ID of the true vertex associated to the selected event");
  //  AddVarI(output(),   TruthVertexNB, "");
  AddVarI(output(),   RooVtxIndex,  "The index of the associated RooTrackerVtx vertex from its position in the TClonesArray");
  AddVarI(output(),   RooVtxEntry,  "The entry of the event in the saved RooTrackerVtx tree");
  AddVarI(output(),   RooVtxEntry2, "The entry of the event in the saved RooTrackerVtx tree");
  AddVarI(output(),   RooVtxFile,   "The File of the event in the saved RooTrackerVtx tree");
  
  // ---- neutrino variables
  AddVarI(output(),   nu_pdg,            "neutrino pdg code");
  AddVarF(output(),   nu_trueE,          "true neutrino energy");  // true neutrino energy
  AddVarI(output(),   nu_truereac,       "true neutrino reaction type");  // true neutrino reaction code
  AddVar3VF(output(), nu_truedir,        "true neutrino direction");  // true neutrino direction
  AddVar4VF(output(), truevtx_pos,       "true neutrino interaction position");
  //  AddVarI(  output(), truevtx_det,       "detector enum of the true neutrino interaction");  // is the same as the category detector


  //--- main lepton variables -------
  //  AddVarF(output(), "truth","truemu_rec",  "I");  // Is the recon muon a true muon
  //  AddVarI(  output(), truelepton_pdg,      "true lepton PDG");  // is the same as the category particle
  AddVarF(  output(), truelepton_mom,      "true lepton momentum");
  AddVarF(  output(), truelepton_costheta, "true lepton cos(theta) w.r.t. neutrino direction");
  AddVar3VF(output(), truelepton_dir,      "true lepton direction");


  //--- add event weights ----------------------
  AddVarVF(output(),  weight,          "global weights (flux, pileup, ...)",NWEIGHTS);

  // --- true signal definition (one could fill this var wih a sub-sample of truth entries) ---
  AddVarI(output(), true_signal, "true signal appraisal (filled differently in each analysis package)");
}

//********************************************************************
bool baseAnalysis::InitializeSpill(){
//********************************************************************

  return _rooVtxManager->InitializeEntry();
}
  
//********************************************************************
bool baseAnalysis::FinalizeConfiguration(){
  //********************************************************************

  // This is called before FillMicroTrees()

  // Save the accum level for the true vertex associated to the recon one such that efficiencies can be computed from the truth tree
  if (conf().GetCurrentConfigurationIndex() != ConfigurationManager::default_conf)
    return true;


  std::vector<AnaTrueVertex*> trueVertices;
  trueVertices.reserve(NMAXTRUEVERTICES);
  std::vector<AnaTrueVertex*>::const_iterator iter;


  AnaTrueVertex* trueVertex = NULL;
  if (GetVertex()) trueVertex = static_cast<AnaTrueVertex*>(GetVertex()->TrueVertex);
  else             trueVertex = static_cast<AnaTrueVertex*>(GetTrueVertex());

  if (trueVertex) trueVertices.push_back(trueVertex);


  // If true vertex does not exist (e.g. can happen that reco vertex is not yet available at this step of the selection) then 
  // store the accum_level to all true vertices of the bunch -> i.e. this basically corresponds to the fact that event as a whole 
  // passed some cuts
  if (trueVertices.size() == 0){
    // Loop over all true vertices in the event and found those that belong to the bunch being processed
    std::vector<AnaTrueVertexB*> vertices = GetSpill().TrueVertices;
    for (std::vector<AnaTrueVertexB*>::iterator it = vertices.begin(); it!=vertices.end(); it++) {
      if (!(*it)) continue;
      AnaTrueVertex* vtx = static_cast<AnaTrueVertex*>(*it);

      // Check the bunch
      if (GetBunch().Bunch != vtx->Bunch) continue;

      trueVertices.push_back(vtx); 
    }
  }

  // Loop over all true vertices of interest
  for (iter = trueVertices.begin(); iter != trueVertices.end(); iter++){ 
    AnaTrueVertex* vtx = *iter;
    if (!vtx) continue;

    // When the AccumLevel has not been already saved for this vertex 
    if (vtx->AccumLevel.size() == 0)
      vtx->AccumLevel.resize(sel().GetNEnabledSelections());

    for (std::vector<SelectionBase*>::iterator it = sel().GetSelections().begin(); it != sel().GetSelections().end(); it++){

      if (!(*it)->IsEnabled()) continue;

      Int_t isel = (*it)->GetEnabledIndex();

      // resize to the number of branches and assign 0 as value to the accum level of each branch
      if (vtx->AccumLevel[isel].size() == 0)
        vtx->AccumLevel[isel].resize((*it)->GetNBranches(),0);

      for (UInt_t ibranch=0;ibranch<(*it)->GetNBranches();ibranch++){
        /*
        if (vtx->AccumLevel[isel][ibranch]!=0){
          // Sometimes the same true vertex is asigned to the candidate in another bunch, most likely because of a delayed track. In this case 
          //  save the higher accum level
            std::cout << "baseAnalysis::FinalizeConfiguration(). This true vertex was used in another bunch (likely a delayed track). Save higher accum_level" << std::endl;
        }
        */
        // when this vertex has not been used in another bunch it will have AccumLevel=0. Then it is sufficient to compare the current accum level with the one in vtx->AccumLevel[isel][ibranch])
        // to save the one high highest accum level
        if ((*it)->GetAccumCutLevel(ibranch)>vtx->AccumLevel[isel][ibranch])
          vtx->AccumLevel[isel][ibranch]=(*it)->GetAccumCutLevel(ibranch);
      }
    }
  }
  return true;
}

//********************************************************************
void baseAnalysis::FillMicroTreesBase(bool addBase){
  //********************************************************************

  (void)addBase;

  // This is called after FinalizeConfiguration

  // Fill all tree variables that are common to all toys in a given configuration

  // The index of the input file in which the RooTracker vtx is
  output().FillVar(RooVtxFile,   GetSpill().InputFileIndex);
  output().FillVar(RooVtxEntry2, GetSpill().RooVtxEntry);
  
  // Event variables
  output().FillVar(run,      GetSpill().EventInfo->Run);
  output().FillVar(subrun,   GetSpill().EventInfo->SubRun);
  output().FillVar(evt,      GetSpill().EventInfo->Event);
  
  output().FillVar(evt_time, static_cast<AnaEventInfo*>(GetSpill().EventInfo)->EventTime);
  
  output().FillVar(bunch,    GetBunch().Bunch);

  // Vertex info
  if (GetVertex()){
    output().FillVar(selvtx_det,anaUtils::GetDetector(GetVertex()->Position));
    output().FillVectorVarFromArray(selvtx_pos,GetVertex()->Position, 4);
    if (GetVertex()->TrueVertex)
      output().FillVectorVarFromArray(selvtx_truepos, GetVertex()->TrueVertex->Position, 4);
  }

  // Neutrino truth variables
  if (GetVertex()){
    if (GetVertex()->TrueVertex){
      AnaTrueVertex* trueVertex = static_cast<AnaTrueVertex*>(GetVertex()->TrueVertex);
      output().FillVar(nu_pdg,        trueVertex->NuPDG);
      output().FillVar(nu_trueE,      trueVertex->NuEnergy);
      output().FillVar(nu_truereac,   trueVertex->ReacCode);

      output().FillVectorVarFromArray(nu_truedir,        trueVertex->NuDir,    3);
    }
  }

}

//********************************************************************
void baseAnalysis::FillToyVarsInMicroTreesBase(bool addBase){
  //********************************************************************

  (void)addBase;

  if (!GetVertex()) return;
  if (!GetVertex()->TrueVertex) return;

  AnaTrueVertex* trueVertex = static_cast<AnaTrueVertex*>(GetVertex()->TrueVertex);

  output().FillToyVar(TruthVertexID, trueVertex->ID);  
  output().FillToyVar(RooVtxIndex,   trueVertex->RooVtxIndex);

  if (_saveRooTrackerVtxTree){
    _rooVtxManager->SetSaveRooVtxEntry(true);
    output().FillToyVar(RooVtxEntry, _rooVtxManager->GetRooVtxEntry());
  }
  else
    output().FillToyVar(RooVtxEntry, trueVertex->RooVtxEntry);

  // weight_syst_total without flux weight systematic (with flux weight correction)
  static Int_t flux_weight_index=-2;
  if (conf().GetCurrentConfigurationIndex() == all_syst && flux_weight_index!=-1){
    if (flux_weight_index==-2){
      ConfigTreeTools config(syst(),conf());
      flux_weight_index = config.GetWeightIndex(all_syst, SystId::kFluxWeight);
    }
    if (flux_weight_index!=-1){
      Float_t flux_corr = output().GetToyVectorVarValueF(AnalysisAlgorithm::weight_corr,flux_weight_index);
      Float_t flux_syst = output().GetToyVectorVarValueF(AnalysisAlgorithm::weight_syst,flux_weight_index);
      
      Float_t total_noflux = output().GetToyVarValueF(AnalysisAlgorithm::weight_syst_total);
      if (flux_syst!=0) total_noflux = total_noflux*flux_corr/flux_syst;
      output().FillToyVar(weight_syst_total_noflux, total_noflux);
    }
  }
}

//********************************************************************
void baseAnalysis::FillTruthTreeBase(const AnaTrueVertex& vtx, const SubDetId::SubDetEnum det, bool IsAntinu){
  //********************************************************************

  // Fill track categories for color drawing
  anaUtils::FillCategories(&vtx, det, IsAntinu, GetSpill().GetIsSandMC());

  // Event variables
  output().FillVar(run,    GetSpill().EventInfo->Run);
  output().FillVar(subrun, GetSpill().EventInfo->SubRun);
  output().FillVar(evt,    GetSpill().EventInfo->Event);

  // true variables
  output().FillVar(nu_pdg,        vtx.NuPDG);
  output().FillVar(nu_trueE,      vtx.NuEnergy);
  output().FillVar(nu_truereac,   vtx.ReacCode);
  output().FillVar(TruthVertexID, vtx.ID);
  output().FillVar(RooVtxIndex,   vtx.RooVtxIndex);
  output().FillVar(RooVtxEntry,   vtx.RooVtxEntry);
  output().FillVar(RooVtxEntry2,  GetSpill().RooVtxEntry);
  output().FillVar(RooVtxFile,    GetSpill().InputFileIndex);

  output().FillVectorVarFromArray(nu_truedir,  vtx.NuDir, 3);
  output().FillVectorVarFromArray(truevtx_pos, vtx.Position, 4);
  //  output().FillVar(               truevtx_det, anaUtils::GetDetector(vtx.Position)); // is the same as the category detector

  // True lepton variables
  //  output().FillVar(               truelepton_pdg,      vtx.LeptonPDG); // is the same as the category particle
  output().FillVar(               truelepton_costheta, (Float_t)cos(anaUtils::ArrayToTVector3(vtx.LeptonDir).Angle(anaUtils::ArrayToTVector3(vtx.NuDir))));
  output().FillVar(               truelepton_mom,      vtx.LeptonMom);
  output().FillVectorVarFromArray(truelepton_dir,      vtx.LeptonDir,3);


  // Fill event weights. Currently, only the weight from flux tuning is filled here.
  if (_flux && !GetSpill().GetIsSandMC()) {
    output().FillVectorVar(weight, _flux->GetWeight(vtx,anaUtils::GetRunPeriod(GetSpill().EventInfo->Run)));
    output().IncrementCounterForVar(weight);
  }
}

//********************************************************************
void baseAnalysis::FillTruthTree(){
//********************************************************************

  // Fill the "truth" tree

  if (!output().HasTree(OutputManager::truth)) return;

  output().SetCurrentTree(OutputManager::truth);

  // Loop over all true vertices
  std::vector<AnaTrueVertexB*> vertices = GetSpill().TrueVertices;
  for (std::vector<AnaTrueVertexB*>::iterator it = vertices.begin(); it!=vertices.end(); it++) {
    AnaTrueVertex& vtx = *static_cast<AnaTrueVertex*>(*it);

    // Check if this vertex needs to be saved in the truth tree
    if (!CheckFillTruthTree(vtx)) continue;

    // Initialize the truth tree. We must do that for each saved vertex since several entries may correspond to the save spill
    output().InitializeTree(OutputManager::truth);

    // Check whether to save the RooTracker info for this vertex. 
    // Should not be saved if already saved in baseAnalysis::FillMicroTreesBase
    if (_saveRooTrackerVtxTree){
      if (!_rooVtxManager->GetRooVtxEntrySaved()) {
        // The current RooVtxEntry should be saved in the RooTrackerVtx tree
        _rooVtxManager->SetSaveRooVtxEntry(true); 
      }
      
      // Associate the vertex with the current RooTrackerVtx entry
      // We should do this here (even when the current RooTrackerVtx entry has been already saved).
      // if the RooVtxEntry has been already save in  baseAnalysis::FillMicroTreesBase, but there is more than one vertex to be saved for
      // this entry the link has not been yet established
      vtx.RooVtxEntry =  _rooVtxManager->GetRooVtxEntry();
    }
      
    // accumulated cut levels to compute efficiencies. This is taken directly from the AnaTrueVertex
    Int_t accumLevel=1;  // intialize to 1 because EventQuality is passed by definition in MC (TODO: what about analysis in which the first cut is not EventQuality)
    for (std::vector<SelectionBase*>::iterator itf=sel().GetSelections().begin();itf!=sel().GetSelections().end();itf++){
      if (!(*itf)->IsEnabled()) continue;

      Int_t isel = (*itf)->GetEnabledIndex();

      for (UInt_t ibranch=0;ibranch<(*itf)->GetNBranches();ibranch++){
        if (vtx.AccumLevel.size()>0) accumLevel = vtx.AccumLevel[isel][ibranch];
        if (sel().GetNEnabledSelections()>1){
          if (sel().GetNMaxBranches()>1)
            output().FillMatrixVar(accum_level, accumLevel, isel, ibranch);
          else
            output().FillVectorVar(accum_level, accumLevel, isel);
        }
        else{
          if (sel().GetNMaxBranches()>1)
            output().FillVectorVar(accum_level, accumLevel, ibranch);
          else
            output().FillVar(accum_level, accumLevel);
        }
      }
    }

    // Reset the categories for the current track
    cat().ResetCurrentCategories();

    // Call the derive classes functions, that also fills the categories
    FillTruthTree(vtx);

    // Fill the truth tree provided the true codes for color drawing
    std::map< std::string, TrackCategoryDefinition* >::iterator its;
    Int_t categ_index = AnalysisAlgorithm::firstCategory;
    for (its=cat().GetCategories().begin();its!=cat().GetCategories().end();its++, categ_index++ ){
      std::string categ_name = its->first;
      TrackCategoryDefinition& categ = *(its->second);
      if (categ.IsMultiType()){
        for (unsigned int i=0;i<categ.GetNTypes();i++)
           output().FillVectorVar(categ_index, (int)cat().CheckCategoryType(categ_name,i),i);
      }
      else output().FillVar(categ_index, cat().GetCode(categ_name));
    }

    // Fill the tree
    output().GetTree(OutputManager::truth)->Fill();
  }

  if (_saveRooTrackerVtxTree){
    // Fill the entry in the RooTrackerVertex tree (only if it has been missed in the default tree, see baseAnalysis::FillMicroTreesBase)
    if (_rooVtxManager->GetSaveRooVtxEntry()) {
      output().FillTree(OutputManager::RooTrackerVtx);
      _rooVtxManager->SetRooVtxEntrySaved(true); // Not really needed, but just in case we do something related later
      _rooVtxManager->IncrementRooVtxEntry();
    }
  }
}



//********************************************************************
void baseAnalysis::SetSaveRooTracker(bool save){
//********************************************************************

  // Any of the methods that sets things into used analysis has been called
  _setMethodCalled = true;

  _saveRooTrackerVtxTree = save;

  for (std::vector<AnalysisAlgorithm*>::iterator it=_usedAnalyses.begin();it!=_usedAnalyses.end();it++){
    // Set this parameter to all used algorithms
    static_cast<baseAnalysis*>( (*it))->SetSaveRooTracker(save);
  }
}


//********************************************************************
void baseAnalysis::SetRooVtxManager(RooTrackerVtxManager* man){
//********************************************************************

  // Any of the methods that sets things into used analysis has been called
  _setMethodCalled = true;
  
  _rooVtxManager=man;

  for (std::vector<AnalysisAlgorithm*>::iterator it=_usedAnalyses.begin();it!=_usedAnalyses.end();it++){
    // Use the same manager for all used algorithms
    static_cast<baseAnalysis*>( (*it))->SetRooVtxManager(man);
  }
}
