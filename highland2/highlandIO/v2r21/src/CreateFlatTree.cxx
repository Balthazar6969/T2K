#include "CreateFlatTree.hxx"
#include "InputManager.hxx"
#include "FlatTreeConverter.hxx"
#include "oaAnalysisTreeConverter.hxx"
#include "HighlandMiniTreeConverter.hxx"
#include "Parameters.hxx"
#include "Versioning.hxx"
#include "HighlandAnalysisUtils.hxx"

#include "CorrId.hxx"

// Corrections
#include "CT4POTCorrection.hxx"
#include "DataQualityCorrection.hxx"
#include "PileUpCorrection.hxx"
#include "TPCdEdxDataCorrection.hxx"
#include "TPCdEdxMCCorrection.hxx"
#include "TPCExpecteddEdxCorrection.hxx"
#include "IgnoreRightECalRuns3and4Correction.hxx"
#include "FlipKinematicsCorrection.hxx"
#include "ToFCorrection.hxx"
#include "MomResolMCCorrection.hxx"
#include "MomRangeCorrection.hxx"
#include "TPCPIDPullMCCorrection.hxx"

//********************************************************************
CreateFlatTree::CreateFlatTree(int argc, char *argv[]):SimpleLoopBase(argc, argv){
//********************************************************************

  // Add the different productions
  anaUtils::AddStandardProductions();  

  // Add package versions to be saved in config tree
  ND::versioning().AddPackage("psycheCore",          anaUtils::GetSoftwareVersionFromPath((std::string)getenv("PSYCHECOREROOT")));
  ND::versioning().AddPackage("psycheUtils",         anaUtils::GetSoftwareVersionFromPath((std::string)getenv("PSYCHEUTILSROOT")));
  ND::versioning().AddPackage("highlandCore",        anaUtils::GetSoftwareVersionFromPath((std::string)getenv("HIGHLANDCOREROOT")));
  ND::versioning().AddPackage("highlandCorrections", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("HIGHLANDCORRECTIONSROOT")));
  ND::versioning().AddPackage("highlandIO",          anaUtils::GetSoftwareVersionFromPath((std::string)getenv("HIGHLANDIOROOT")));


  // add the different converters
  input().AddConverter("FlatTree",       new FlatTreeConverter(true));
  input().AddConverter("MiniTree",       new HighlandMiniTreeConverter(true));
  input().AddConverter("oaAnalysisTree", new oaAnalysisTreeConverter());

}
//********************************************************************
bool CreateFlatTree::Initialize(){
//********************************************************************
  // save only true tracks and vertices with recon association
  _saveTruthWithRecon = (bool)ND::params().GetParameterI("highlandIO.FlatTree.SaveTruthWithReconAssociation");
  _saveRoo = (bool)ND::params().GetParameterI("highlandIO.FlatTree.SaveRooTracker");

  // Filter RooTrackerVtxTree
  _filterRoo = (bool)ND::params().GetParameterI("highlandIO.FlatTree.FilterRooTracker");

  
  _saveTPCInfo     = (bool)ND::params().GetParameterI("highlandIO.FlatTree.SaveTPCInfo");
  _saveFGDInfo     = (bool)ND::params().GetParameterI("highlandIO.FlatTree.SaveFGDInfo");
  _saveECALInfo    = (bool)ND::params().GetParameterI("highlandIO.FlatTree.SaveECALInfo");
  _saveP0DInfo     = (bool)ND::params().GetParameterI("highlandIO.FlatTree.SaveP0DInfo");
  _saveSMRDInfo    = (bool)ND::params().GetParameterI("highlandIO.FlatTree.SaveSMRDInfo");
  _saveTrackerInfo = (bool)ND::params().GetParameterI("highlandIO.FlatTree.SaveTrackerInfo");

  // save info from the local reconstruction to the flat tree?
  _saveReconDirFGDOnlyInfo = (bool)ND::params().GetParameterI("highlandIO.FlatTree.UseReconDirFGDOnly"    );
  _saveReconDirP0DInfo     = (bool)ND::params().GetParameterI("highlandIO.FlatTree.UseReconDirP0D"        );
  _saveReconDirPECALInfo   = (bool)ND::params().GetParameterI("highlandIO.FlatTree.UseReconDirP0DECal"    );
  _saveReconDirTECALInfo   = (bool)ND::params().GetParameterI("highlandIO.FlatTree.UseReconDirTrackerECal");

  _useTPC1     = (bool)ND::params().GetParameterI("highlandIO.FlatTree.UseTPC1");
  _useTPC2     = (bool)ND::params().GetParameterI("highlandIO.FlatTree.UseTPC2");
  _useTPC3     = (bool)ND::params().GetParameterI("highlandIO.FlatTree.UseTPC3");
  _useFGD1     = (bool)ND::params().GetParameterI("highlandIO.FlatTree.UseFGD1");
  _useFGD2     = (bool)ND::params().GetParameterI("highlandIO.FlatTree.UseFGD2");
  _useP0D      = (bool)ND::params().GetParameterI("highlandIO.FlatTree.UseP0D");
  _useDsECal   = (bool)ND::params().GetParameterI("highlandIO.FlatTree.UseDsECal");
  _useTrECal   = (bool)ND::params().GetParameterI("highlandIO.FlatTree.UseTrECal");
  _useP0DECal  = (bool)ND::params().GetParameterI("highlandIO.FlatTree.UseP0DECal");
  _useSMRD     = (bool)ND::params().GetParameterI("highlandIO.FlatTree.UseSMRD");

  _useTPC1outOfBunch     = (bool)ND::params().GetParameterI("highlandIO.FlatTree.UseTPC1outOfBunch");
  _useTPC2outOfBunch     = (bool)ND::params().GetParameterI("highlandIO.FlatTree.UseTPC2outOfBunch");
  _useTPC3outOfBunch     = (bool)ND::params().GetParameterI("highlandIO.FlatTree.UseTPC3outOfBunch");
  _useFGD1outOfBunch     = (bool)ND::params().GetParameterI("highlandIO.FlatTree.UseFGD1outOfBunch");
  _useFGD2outOfBunch     = (bool)ND::params().GetParameterI("highlandIO.FlatTree.UseFGD2outOfBunch");
  _useP0DoutOfBunch      = (bool)ND::params().GetParameterI("highlandIO.FlatTree.UseP0DoutOfBunch");
  _useDsECaloutOfBunch   = (bool)ND::params().GetParameterI("highlandIO.FlatTree.UseDsECaloutOfBunch");
  _useTrECaloutOfBunch   = (bool)ND::params().GetParameterI("highlandIO.FlatTree.UseTrECaloutOfBunch");
  _useP0DECaloutOfBunch  = (bool)ND::params().GetParameterI("highlandIO.FlatTree.UseP0DECaloutOfBunch");
  _useSMRDoutOfBunch     = (bool)ND::params().GetParameterI("highlandIO.FlatTree.UseSMRDoutOfBunch");

  _saveTrueNuNC       = (bool)ND::params().GetParameterI("highlandIO.FlatTree.TrueVertex.NuNC");
  _saveTrueAntiNuNC   = (bool)ND::params().GetParameterI("highlandIO.FlatTree.TrueVertex.AntiNuNC");
  _saveTrueNumuCC     = (bool)ND::params().GetParameterI("highlandIO.FlatTree.TrueVertex.NumuCC");
  _saveTrueAntiNumuCC = (bool)ND::params().GetParameterI("highlandIO.FlatTree.TrueVertex.AntiNumuCC");
  _saveTrueNueCC      = (bool)ND::params().GetParameterI("highlandIO.FlatTree.TrueVertex.NueCC");
  _saveTrueAntiNueCC  = (bool)ND::params().GetParameterI("highlandIO.FlatTree.TrueVertex.AntiNueCC");


  if (ND::params().GetParameterI("highlandIO.FlatTree.TrueVertex.TPC1"))    _saveTrueVertexInDet.push_back(SubDetId::kTPC1);
  if (ND::params().GetParameterI("highlandIO.FlatTree.TrueVertex.TPC2"))    _saveTrueVertexInDet.push_back(SubDetId::kTPC2);
  if (ND::params().GetParameterI("highlandIO.FlatTree.TrueVertex.TPC3"))    _saveTrueVertexInDet.push_back(SubDetId::kTPC3);
  if (ND::params().GetParameterI("highlandIO.FlatTree.TrueVertex.FGD1"))    _saveTrueVertexInDet.push_back(SubDetId::kFGD1);
  if (ND::params().GetParameterI("highlandIO.FlatTree.TrueVertex.FGD2"))    _saveTrueVertexInDet.push_back(SubDetId::kFGD2);
  if (ND::params().GetParameterI("highlandIO.FlatTree.TrueVertex.P0D"))     _saveTrueVertexInDet.push_back(SubDetId::kP0D);
  if (ND::params().GetParameterI("highlandIO.FlatTree.TrueVertex.DsECal"))  _saveTrueVertexInDet.push_back(SubDetId::kDSECAL);
  if (ND::params().GetParameterI("highlandIO.FlatTree.TrueVertex.TrECal"))  _saveTrueVertexInDet.push_back(SubDetId::kTECAL);
  if (ND::params().GetParameterI("highlandIO.FlatTree.TrueVertex.P0DECal")) _saveTrueVertexInDet.push_back(SubDetId::kPECAL);
  if (ND::params().GetParameterI("highlandIO.FlatTree.TrueVertex.SMRD"))    _saveTrueVertexInDet.push_back(SubDetId::kSMRD);


  // Check software version compatibility between oaAnalysisReader and current file
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

  // Dump the production used for corrections, bunching, systematics, etc
  versionUtils::DumpProductions();

  _POTSincePreviousSpill = 0;
  _spill_filled = false;


  // Enable any corrections that have to be done while the flat tree is being created.
  // Ignore right ECal for runs 3 and 4 as part of it is broken.
  if (ND::params().GetParameterI("highlandIO.FlatTree.EnableIgnoreRightECalCorrection")){
    _corrections.AddCorrection(CorrId::kRightECalRun3And4Corr, "ignorerightecal_corr", new IgnoreRightECalRuns3and4Correction());
  }
  
  if (ND::params().GetParameterI("highlandIO.FlatTree.EnableDQCorrection")) {
    // Correct the data quality in periods when a FGD FEB wasn't working.
    _corrections.AddCorrection(CorrId::kDQCorr, "dq_corr", new DataQualityCorrection());
  }


#if !VERSION_HAS_OFFICIAL_POT
  // Only need this correction for P5 files.

  if (ND::params().GetParameterI("highlandIO.FlatTree.EnablePOTCorrection")) {
    // Use CT4 for MR44 POT accounting, as CT5 wasn't working properly.
    _corrections.AddCorrection(CorrId::kCT4POTCorr, "ct4pot_corr", new CT4POTCorrection());
  }
#endif


  if (ND::params().GetParameterI("highlandIO.FlatTree.EnablePileUpCorrection")){
    _corrections.AddCorrection(CorrId::kPileUpCorr, "pileup_corr", new PileUpCorrection());
  }
  // Apply the TPC dE/dx correction in all configurations
  if (ND::params().GetParameterI("highlandIO.FlatTree.EnableTPCdEdxDataCorrection")){
    _corrections.AddCorrection(CorrId::kTPCdEdxDataCorr, "tpcdedx_data_corr", new TPCdEdxDataCorrection());
  }
  // Apply the TPC dE/dx correction in all configurations
  if (ND::params().GetParameterI("highlandIO.FlatTree.EnableTPCdEdxMCCorrection")){
    _corrections.AddCorrection(CorrId::kTPCdEdxMCCorr, "tpcdedx_mc_corr", new TPCdEdxMCCorrection());
  }
  // Apply the TPC expected dE/dx correction in all configurations
  if (ND::params().GetParameterI("highlandIO.FlatTree.EnableTPCExpecteddEdxCorrection")){
    _corrections.AddCorrection(CorrId::kTPCdEdxExpCorr, "tpcexpecteddedx_corr", new TPCExpecteddEdxCorrection());
  }
  
  // Apply the TPC PID correction based on pull params smearing
  if (ND::params().GetParameterI("highlandIO.FlatTree.EnableTPCPIDPullMCCorrection")){
    _corrections.AddCorrection(CorrId::kTPCPIDPullMCCorr, "tpc_pid_pull_mc_corr", new TPCPIDPullMCCorrection());
  }
 
  if (ND::params().GetParameterI("highlandIO.FlatTree.EnableMomResolMCCorrection")){
    _corrections.AddCorrection(CorrId::kMomResolMCCorr, "mom_resol_mc_corr", new MomResolMCCorrection());
  } 

  // Need to be called before the ToF in order to correctly flip a track if required
  if (ND::params().GetParameterI("highlandIO.FlatTree.EnableFlipKinematicsCorrection")){
    _corrections.AddCorrection(CorrId::kFlipKinCorr, "flipkinematics_corr", new FlipKinematicsCorrection());
  }

  if (ND::params().GetParameterI("highlandIO.FlatTree.EnableToFCorrection")){ 
    _corrections.AddCorrection(CorrId::kToFCorr, "tof_corr", new ToFCorrection());
  }

  
  if (ND::params().GetParameterI("highlandIO.FlatTree.EnableMomRangeCorrection")){
    _corrections.AddCorrection(CorrId::kMomRangeCorr, "momrange_corr", new MomRangeCorrection());
  }

  return true;
}

//********************************************************************
void CreateFlatTree::DefineOutputTree(){
//********************************************************************

  // Add a new tree called "flattree"
  AddTreeWithName(flattree,"flattree");

  // Add a tree to save the truth info as a clone of the TruthDir/NRooTrackerVtx tree.
  // oaAnalysis tree converter has already dealt with the NRooTrackerVtx bit for us.

  std::string rooTreeName="";
  if(input().GetChain("NRooTrackerVtx"))
    rooTreeName = "NRooTrackerVtx";
  else if (input().GetChain("GRooTrackerVtx"))
    rooTreeName = "GRooTrackerVtx";

  if(_saveRoo && rooTreeName!=""){
    if (input().InputIsOriginalTree() && _filterRoo){
      AddTreeWithName(RooTrackerVtx, rooTreeName);

      Vtx = new TClonesArray(("ND::"+rooTreeName).c_str(),100);

      GetTree(RooTrackerVtx)->Branch("RunID",       &RunID,       "RunID/I",       32000);
      GetTree(RooTrackerVtx)->Branch("EventID",     &EventID,     "EventID/I",     32000);
      GetTree(RooTrackerVtx)->Branch("Preselected", &Preselected, "Preselected/B", 32000);
      GetTree(RooTrackerVtx)->Branch("SubrunID",    &SubrunID,    "SubrunID/I",    32000);

      GetTree(RooTrackerVtx)->Branch("NVtx", &NVtx, "NVtx/I", 32000);
      GetTree(RooTrackerVtx)->Branch("Vtx","TClonesArray", &Vtx   , 32000, 1);
    }
    else
      AddTreeWithName(RooTrackerVtx, rooTreeName,input().GetChain(rooTreeName));
  }


  SetCurrentTree(flattree);
  SetFillSingleTree(GetCurrentTree());


  // -------- Define variables to be put to the flat tree ----------------------
  AnaBunchIO                *bunch          = new AnaBunchIO                ();
  AnaECALParticleIO         *ecal           = new AnaECALParticleIO         ();
  AnaFgdTimeBinIO           *fgdTimeBin     = new AnaFgdTimeBinIO           ();
  AnaFGDParticleIO          *fgd            = new AnaFGDParticleIO          ();
  AnaP0DParticleIO          *p0d            = new AnaP0DParticleIO          ();
  AnaSMRDParticleIO         *smrd           = new AnaSMRDParticleIO         ();
  AnaSpillIO                *spill          = new AnaSpillIO                ();
  AnaTECALReconObjectIO     *tecalRecon     = new AnaTECALReconObjectIO     ();
  AnaTECALUnmatchedObjectIO *tecalUnmatched = new AnaTECALUnmatchedObjectIO ();
  AnaP0DReconVertexIO       *p0dVertex      = new AnaP0DReconVertexIO       ();
  AnaP0DReconParticleIO     *p0dParticle    = new AnaP0DReconParticleIO     ();
  AnaTPCParticleIO          *tpc            = new AnaTPCParticleIO          ();
  AnaTrackIO                *track          = new AnaTrackIO                ();
  AnaTrackerTrackIO         *tracker        = new AnaTrackerTrackIO         ();
  AnaTrueParticleIO         *trueParticle   = new AnaTrueParticleIO         ();
  AnaTrueVertexIO           *trueVertex     = new AnaTrueVertexIO           ();
  AnaVertexIO               *vertex         = new AnaVertexIO               ();

  //------------ spill variables -------
  // evt, run, spill, etc Includes Beam and Data quality
  DefineSpill(spill);

  AddVar(sPOTSincePreviousSpill,"sPOTSincePreviousSpill","D","");

  // All true vertices
  DefineTrueVertex(trueVertex);

  // All true tracks
  DefineTrueParticle(trueParticle);

  // FGD time bins
  DefineFgdTimeBin(fgdTimeBin);

  // Summary info about bunch
  DefineBunch(bunch);

  if (_saveTPCInfo)     DefineTPCParticle    (tpc    );  // TPC     constituents
  if (_saveFGDInfo)     DefineFGDParticle    (fgd    );  // FGD     constituents
  if (_saveECALInfo)    DefineECALParticle   (ecal   );  // ECAL    constituents
  if (_saveSMRDInfo)    DefineSMRDParticle   (smrd   );  // SMRD    constituents
  if (_saveP0DInfo)     DefineP0DParticle    (p0d    );  // P0D     constituents
  if (_saveTrackerInfo) DefineTrackerTrack(tracker);  // Tracker constiruents

  // Global track info
  DefineTrack(track);

  // Info from local reconstruction
  if (_saveReconDirTECALInfo){
    DefineTECALReconObject    (tecalRecon    ); // ReconDir/TrackerECal
    DefineTECALUnmatchedObject(tecalUnmatched); // ReconDir/TrackerECal
  }
  if (_saveReconDirP0DInfo){
    DefineP0DReconVertex  (p0dVertex  );        // ReconDir/P0D
    DefineP0DReconParticle(p0dParticle);        // ReconDir/P0D
  }

  //------------ Vertices
  DefineVertex(vertex);
  _POTSincePreviousSpill=0;

  delete fgdTimeBin;
  delete bunch;
  delete ecal;
  delete fgd;
  delete p0d;
  delete p0dVertex;
  delete p0dParticle;
  delete smrd;
  delete spill;
  delete tecalRecon;
  delete tecalUnmatched;
  delete tpc;
  delete track;
  delete tracker;
  delete trueParticle;
  delete trueVertex;
  delete vertex;
}


//********************************************************************
bool CreateFlatTree::Process(){
//********************************************************************

  _spill_filled=false;

  // Set the tree to fill
  SetFillSingleTree(flattree);
  SetCurrentTree(flattree);

  AnaSpill& spill = *static_cast<AnaSpill*>(&input().GetCorrectedSpill());

  // Accumulate POT since previous spill saved
  _POTSincePreviousSpill += static_cast<const AnaBeam*>(spill.Beam)->POT;

  // Analyse OutOfBunch for first for having AnaTrack when associating vertex constituents
  std::vector<AnaBunchC*> outplusbunches;
  outplusbunches.push_back(spill.OutOfBunch);
  for (unsigned int b = 0; b < spill.Bunches.size(); b++){
    outplusbunches.push_back(spill.Bunches[b]);
  }

  //------ Loop over outofbunch+bunches --------------
  for (unsigned int b=0; b < outplusbunches.size(); b++){

    AnaBunch* bunch = static_cast<AnaBunch*>(outplusbunches[b]);

    if (!bunch) continue;
   
    // Check whether there are FGD or TPC reconstructed tracks in the bunch
    bool ok_recon = CheckReconFillFlatTree(*bunch);

    // Stop if there is no interesting recon info
    if (!ok_recon) continue;

    // Put all variables to their default value
    InitializeTree(GetCurrentTree());

    // Summary info for the spill + Beam + DQ + trigger
    AnaSpill* spill = static_cast<AnaSpill*>(&input().GetSpill());
    AnaSpillIO spill_io(*spill);

    // When the RooTrackerVtx tree is saved in the output file the entry must be recomputed (is not the one in the input file)
    if (HasTree(RooTrackerVtx))
      spill_io.RooVtxEntry = _rooVtxManager.GetRooVtxEntry();
    
    // Summary info for the spill + Beam + DQ
    spill_io.Write(*this, "s");
    
    
    AnaBunchIO bunch_io(*bunch); 
    
    // Fill Bunch variables
    WriteBunch(&bunch_io);

    // Fill spill first for having AnaTrueParticle and AnaTrueVertex to point to
    // Fill spill && truth info only for the first bunch saved
    if (!_spill_filled) {
      // TrueVertices and FgdTimeBins
      FillSpillVariables();

      // Fill the RooTrackerVtx tree
      FillRooTrackerVtxTree();
      
      _spill_filled = true;
    }

    //------ Particles ----------
    for (UInt_t i = 0; i < bunch->Particles.size(); i++){
      AnaTrack* track = static_cast<AnaTrack*>(bunch->Particles[i]);

      if (!track) continue;
   
      
      // TPC constituent variables
      if (_saveTPCInfo) {
        ResetCheckWriteSegment();
        for (int j = 0; j < track->nTPCSegments; j++){
          AnaTPCParticle* particle = static_cast<AnaTPCParticle*>(track->TPCSegments[j]);
          AnaTPCParticleIO particle_io(*particle);
          if (CheckWriteSegment(SubDetId::kTPC, *track->TPCSegments[j])) WriteTPCParticle(&particle_io);
        }
      }

      // FGD constituent variables
      if (_saveFGDInfo) {
        ResetCheckWriteSegment();
        for (int j = 0; j < track->nFGDSegments; j++){
          AnaFGDParticle* particle = static_cast<AnaFGDParticle*>(track->FGDSegments[j]);
          AnaFGDParticleIO particle_io(*particle);
          if (CheckWriteSegment(SubDetId::kFGD, *track->FGDSegments[j])) WriteFGDParticle(&particle_io);
        }  
      }

      // ECAL constituent variables
      if (_saveECALInfo) {
        ResetCheckWriteSegment(); 
        for (int j = 0; j < track->nECALSegments; j++){
          AnaECALParticle* particle = static_cast<AnaECALParticle*>(track->ECALSegments[j]);
          AnaECALParticleIO particle_io(*particle);
          if (CheckWriteSegment(SubDetId::kECAL, *track->ECALSegments[j])) WriteECALParticle(&particle_io);
        }
      }

      // SMRD constituent variables
      if (_saveSMRDInfo) {
        ResetCheckWriteSegment();
        for (int j = 0; j < track->nSMRDSegments; j++){ 
          AnaSMRDParticle* particle = static_cast<AnaSMRDParticle*>(track->SMRDSegments[j]);
          AnaSMRDParticleIO particle_io(*particle);

          if (CheckWriteSegment(SubDetId::kSMRD, *track->SMRDSegments[j])) WriteSMRDParticle(&particle_io);
        }
      }

      // P0D constituent variables
      if (_saveP0DInfo) {
        ResetCheckWriteSegment();
        for (int j = 0; j < track->nP0DSegments; j++){
          AnaP0DParticle* particle = static_cast<AnaP0DParticle*>(track->P0DSegments[j]);
          AnaP0DParticleIO particle_io(*particle);

          if (CheckWriteSegment(SubDetId::kP0D, *track->P0DSegments[j])) WriteP0DParticle(&particle_io);
        }
      }

      // TRACKER constituent variables
      if (_saveTrackerInfo) {
        std::vector<AnaTrackerTrackB*> trackers = track->TRACKERSegments;
        for (unsigned int j = 0; j < trackers.size(); j++){
          AnaTrackerTrack* tracker = static_cast<AnaTrackerTrack*>(trackers[j]);

          AnaTrackerTrackIO tracker_io(*tracker);
           
          WriteTrackerTrack(&tracker_io, j);
        }
      }
      
      AnaTrackIO track_io(*track);
      
      // Summary info for the track (adds a new track to the vector)
      WriteTrack(&track_io);
    }

    // Write TECALReconObjects to the bunch ?
    if (_saveReconDirTECALInfo){

      // Cast the AnaBunch pointer to an AnaLocalReconBunch
      AnaLocalReconBunch* localBunch = static_cast<AnaLocalReconBunch*>(bunch);

      // Iterate over the vector of AnaTECALReconObjects belonging to the bunch and write them
      // to the flat tree.
      for (std::vector<AnaTECALReconObject*>::const_iterator it = localBunch->TECALReconObjects.begin(); it != localBunch->TECALReconObjects.end(); it++){
        
        AnaTECALReconObject* object = static_cast<AnaTECALReconObject*>(*it);
        AnaTECALReconObjectIO object_ecal(*object);

        WriteTECALReconObject(&object_ecal);
      }

      // Iterate over the vector of AnaTECALUnmatchedObjects belonging to the bunch and write them
      // to the flat tree.
      for (std::vector<AnaTECALUnmatchedObject*>::const_iterator it=localBunch->TECALUnmatchedObjects.begin(); it!=localBunch->TECALUnmatchedObjects.end(); it++){
        
        AnaTECALUnmatchedObject* object = static_cast<AnaTECALUnmatchedObject*>(*it);
        AnaTECALUnmatchedObjectIO object_ecal(*object);


        WriteTECALUnmatchedObject(&object_ecal);
      }
    }

    // Write P0DReconObjects to the bunch?
    if (_saveReconDirP0DInfo){

      // Cast the AnaBunch pointer to an AnaLocalReconBunch
      AnaLocalReconBunch* localBunch = static_cast<AnaLocalReconBunch*>(bunch);

     
      // Iterate over the vector of AnaP0DReconVertex objects belonging to the bunch and write them
      for (std::vector<AnaP0DReconVertex*>::const_iterator it = localBunch->P0DReconVertices.begin(); it != localBunch->P0DReconVertices.end(); it++){
       
        AnaP0DReconVertex* object = static_cast<AnaP0DReconVertex*>(*it);
        AnaP0DReconVertexIO object_p0d(*object);
        
        WriteP0DReconVertex(&object_p0d);
      }
      
      // Iterate over the vector of AnaP0DReconParticle objects belonging to the bunch and write them
      for (std::vector<AnaP0DReconParticle*>::const_iterator it = localBunch->P0DReconParticles.begin(); it!=localBunch->P0DReconParticles.end(); it++){
        
        AnaP0DReconParticle* object = static_cast<AnaP0DReconParticle*>(*it);
        AnaP0DReconParticleIO object_p0d(*object);
        
        WriteP0DReconParticle(&object_p0d);
      }
    }


    //------ Vertices ----------
    std::vector<int> trueVertexIndices;
    for (unsigned int i = 0; i < bunch->Vertices.size(); i++) {
      //Save the trueVertexIndex instead of ID to speed up associations in FlatTreeConverter
      AnaTrueVertexB* MatchedTrueVertices[100];
      int nMatchedTrueVertices = bunch->Vertices[i]->GetTrueVertices(MatchedTrueVertices);
      trueVertexIndices.clear();
      for (int j = 0; j < nMatchedTrueVertices; j++) {
        int trueVertexIndex = FindTrueVertexIndex(MatchedTrueVertices[j]);
        trueVertexIndices.push_back(trueVertexIndex);
      }
      
      AnaVertex* vertex_tmp = static_cast<AnaVertex*>(bunch->Vertices[i]);
      AnaVertexIO vertex(*vertex_tmp);
      
      // Summary info for the vertex (adds a new vertex to the vector)
      WriteVertex(&vertex, trueVertexIndices);
    }

    // fill the flat tree
    FillFlatTree();
  }

  // Fill spill and truth info when there are no bunches
  if (!_spill_filled){

    // Check whether this event is true signal (for eff calculation)
    bool ok_truth = CheckTruthFillFlatTree(spill);
    if (ok_truth){

      InitializeTree(GetCurrentTree());

      AnaSpill* spill_tmp = static_cast<AnaSpill*>(&input().GetSpill());
      AnaSpillIO spill(*spill_tmp);

      // When the RooTrackerVtx tree is saved in the output file the entry must be recomputed (is not the one in the input file)
      if (HasTree(RooTrackerVtx))
        spill.RooVtxEntry = _rooVtxManager.GetRooVtxEntry();
      
      // Summary info for the spill + Beam + DQ
      spill.Write(*this, "s");

      // TrueVertices and FgdTimeBins
      FillSpillVariables();

      // Fill the flattree
      FillFlatTree();

      // Fill the RooTrackerVtx tree
      FillRooTrackerVtxTree();
        
      _spill_filled = true;
    }
  }

  return true;
}

//********************************************************************
void CreateFlatTree::Finalize(){
//********************************************************************

  if (_spill_filled) return;

  // Fill the info for th last processed spill such that we keep track of the total POT since the last saved entry.

  InitializeTree(GetCurrentTree());

  // Fill the RooTrackerVtx tree
    // Must be called before FillMiniTree since when the RooTrackerVtx filtering is applied the RooVtxIndex in true vertices changes
  FillRooTrackerVtxTree();

  // Fill the flattree
  FillFlatTree();

  // Delete Spills for the last entry
  input().DeleteSpill();
}

//********************************************************************
void CreateFlatTree::FillSpillVariables(){
//********************************************************************

  // True vertex variables
  _savedTrueVertices.clear();

  AnaSpill& spill = *static_cast<AnaSpill*>(&input().GetCorrectedSpill());
  
  for (unsigned int i=0; i<spill.TrueVertices.size(); i++){
    
    AnaTrueVertex* vertex_tmp = static_cast<AnaTrueVertex*>(spill.TrueVertices[i]);
    AnaTrueVertexIO vertex(*vertex_tmp);
    
    //    if (vertex->ReconParticles.size()>0 || !_saveTruthWithRecon) { // scanta: now this should work
    WriteTrueVertex(&vertex);
    // Save vertices in a temporary vector for getting their index for WriteTrueTrack and WriteVertex
    // (so it is correct even if we don't save all vertices)
    _savedTrueVertices.push_back(vertex_tmp);
  }

  if (_saveTruthWithRecon) CreateListOfTrueParticlesToSave();

  //  True track variables
  for (unsigned int i=0;i<spill.TrueParticles.size();i++){
    if (!_saveTruthWithRecon || _trueParticleIDs.find(spill.TrueParticles[i]->ID) != _trueParticleIDs.end()) {
      //Save the trueVertexIndex instead of ID to speed up associations in FlatTreeConverter
      int trueVertexIndex = -2;  // this will put the default value (for a true track without a true vertex).
      if (spill.TrueParticles[i]->TrueVertex)
        trueVertexIndex = FindTrueVertexIndex(spill.TrueParticles[i]->TrueVertex);
      
      AnaTrueParticle* particle_tmp = static_cast<AnaTrueParticle*>(spill.TrueParticles[i]);
      AnaTrueParticleIO particle(*particle_tmp);

      WriteTrueParticle(&particle, trueVertexIndex);
    }
  }

  // FGD time bin variables
  for (unsigned int i=0;i<spill.FgdTimeBins.size();i++){
    
    AnaFgdTimeBin* bin_tmp = static_cast<AnaFgdTimeBin*>(spill.FgdTimeBins[i]);
    AnaFgdTimeBinIO bin(*bin_tmp);
    
    WriteFgdTimeBin(&bin);
  }


}

//********************************************************************
int CreateFlatTree::FindTrueVertexIndex(AnaTrueVertexB* vertex) {
//********************************************************************
  if (!vertex) {
    AnaSpill& spill = *static_cast<AnaSpill*>(&input().GetCorrectedSpill());
    if (_savedTrueVertices.size() == spill.TrueVertices.size())
      std::cout << "debug error 1234 TrueVertex not associated." << std::endl;
    return -1;
  }
  for (unsigned int i = 0; i < _savedTrueVertices.size(); i++) {
    if (vertex == _savedTrueVertices[i]) return i;
  }
  std::cout << "debug error 1235: TrueVertexIndex not found" << std::endl;
  return -1;
}

//********************************************************************
void CreateFlatTree::CreateListOfTrueParticlesToSave() {
//********************************************************************
  _trueParticleIDs.clear();

  // First find all the true tracks that are matched to recon objects.
  // These are obviosuly important.
  std::set<AnaTrueObjectC*> matched;

  AnaSpill& spill = *static_cast<AnaSpill*>(&input().GetCorrectedSpill());

  std::vector<AnaBunch*> outplusbunches;
  outplusbunches.push_back(static_cast<AnaBunch*>(spill.OutOfBunch));
  for (unsigned int b = 0; b < spill.Bunches.size(); b++) {
    outplusbunches.push_back(static_cast<AnaBunch*>(spill.Bunches[b]));
  }
  //------ Loop over outofbunch+bunches --------------
  for (unsigned int b = 0; b < outplusbunches.size(); b++) {

    AnaBunch* bunch = static_cast<AnaBunch*>(outplusbunches[b]);

    for (unsigned int i = 0; i < bunch->Particles.size(); i++) {
      AnaTrackB* track = static_cast<AnaTrackB*>(bunch->Particles[i]);
      if (track->TrueObject) {
        matched.insert(track->TrueObject);
      }
      for (int k = 0; k < track->nTPCSegments; k++) {
        AnaTPCParticleB* tpcTrack = track->TPCSegments[k];
        if (tpcTrack->TrueObject) {
          matched.insert(tpcTrack->TrueObject);
        }
      }
    }
  }

  // Recursively work upwards adding all the parents of the tracks we already selected.
  // We need these so the "parent", "gparent" categories work, among other things.
  for (std::set<AnaTrueObjectC*>::iterator it = matched.begin(); it != matched.end(); it++){
    AnaTrueParticleB* parent = static_cast<AnaTrueParticleB*>(*it);
    if(!parent)
      continue;
    _trueParticleIDs.insert(parent->ID);
    
    while (parent && _trueParticleIDs.find(parent->ParentID) == _trueParticleIDs.end()) {
      parent = GetTrueParticleByID(parent->ParentID);
      if(parent)
        _trueParticleIDs.insert(parent->ID);
      
    }
  }
  
  // Finally add all the primary tracks for each vertex. We need these so the "topology"
  // category works, for example.
  for (unsigned int i=0;i<spill.TrueVertices.size();i++){
    AnaTrueVertexB* vtx = spill.TrueVertices[i];
    for (int j = 0; j < vtx->nTrueParticles; j++) {
      if (vtx->TrueParticles[j]->ParentID == 0) {
        _trueParticleIDs.insert(vtx->TrueParticles[j]->ID);
      }
    }
  }
}

//********************************************************************
AnaTrueParticleB* CreateFlatTree::GetTrueParticleByID(int ID) {
//********************************************************************

  AnaSpill& spill = *static_cast<AnaSpill*>(&input().GetCorrectedSpill());
  std::vector<AnaTrueParticleB*> all = spill.TrueParticles;

  for (unsigned int i = 0; i < all.size(); i++) {
    if (all[i]->ID == ID) {
      return all[i];
    }
  }

  return NULL;
}

//********************************************************************
bool CreateFlatTree::CheckTruthFillFlatTree(const AnaSpill& spill){
//********************************************************************

  bool ok_det = false;
  bool ok_reac = false;

  const std::vector<AnaTrueVertexB*>& vertices = spill.TrueVertices;
  for (std::vector<AnaTrueVertexB*>::const_iterator it = vertices.begin(); it!=vertices.end(); it++) {
    AnaTrueVertex& vtx = *static_cast<AnaTrueVertex*>(*it);

    // Check the reaction code
    ok_reac = CheckTrueVertexReaction(vtx);

    // Check the detector only if the reaction is OK
    if (ok_reac)
      ok_det = CheckTrueVertexDetector(vtx.Detector);

    // return true when it finds a tru vertex fulfilling both conditions
    if (ok_reac && ok_det) return true;
  }

  return false;
}

//********************************************************************
bool CreateFlatTree::CheckTrueVertexReaction(const AnaTrueVertex& vtx){
//********************************************************************

  // NuWro prod5 uses 70 for 2p2h code (Neut does not have 70 at all, and uses 2 instead)
  // NuWro prod6 uses 2 as NEUT
  if (_saveTrueNuNC       && vtx.ReacCode>=+30 && abs(vtx.ReacCode)!=70) return true;
  if (_saveTrueAntiNuNC   && vtx.ReacCode<=-30 && abs(vtx.ReacCode)!=70) return true;
  if (_saveTrueNumuCC     && vtx.NuPDG==+14 && ((vtx.ReacCode>0 && vtx.ReacCode<+30) || vtx.ReacCode==+70)) return true;
  if (_saveTrueAntiNumuCC && vtx.NuPDG==-14 && ((vtx.ReacCode<0 && vtx.ReacCode>-30) || vtx.ReacCode==-70)) return true;
  if (_saveTrueNueCC      && vtx.NuPDG==+12 && ((vtx.ReacCode>0 && vtx.ReacCode<+30) || vtx.ReacCode==+70)) return true;
  if (_saveTrueAntiNueCC  && vtx.NuPDG==-12 && ((vtx.ReacCode<0 && vtx.ReacCode>-30) || vtx.ReacCode==-70)) return true;
  return false;
}

//********************************************************************
bool CreateFlatTree::CheckTrueVertexDetector(unsigned long det){
//********************************************************************

  for (unsigned int i = 0;i<_saveTrueVertexInDet.size();i++)
    if (SubDetId::GetDetectorUsed(det,_saveTrueVertexInDet[i])) return true;

  return false;
}

//********************************************************************
bool CreateFlatTree::CheckReconFillFlatTreeOutOfBunch(const AnaBunch& bunch){
//********************************************************************

  for (std::vector<AnaParticleB*>::const_iterator it = bunch.Particles.begin(); it!=bunch.Particles.end(); it++) {

    unsigned long bitfield = (*it)->Detector;

    if (_useTPC1outOfBunch)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kTPC1)) return true;
    if (_useTPC2outOfBunch)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kTPC2)) return true;
    if (_useTPC3outOfBunch)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kTPC3)) return true;
    if (_useFGD1outOfBunch)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kFGD1)) return true;
    if (_useFGD2outOfBunch)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kFGD2)) return true;
    if (_useP0DoutOfBunch)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kP0D)) return true;
    if (_useDsECaloutOfBunch)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kDSECAL)) return true;
    if (_useTrECaloutOfBunch)
      if (SubDetId::HasTECALDetector(bitfield)) return true;
    if (_useP0DECaloutOfBunch)
      if (SubDetId::HasPECALDetector(bitfield)) return true;
    if (_useSMRDoutOfBunch)
      if (SubDetId::HasSMRDDetector(bitfield)) return true;
  }

  return false;
}

//********************************************************************
bool CreateFlatTree::CheckReconFillFlatTree(const AnaBunch& bunch){
//********************************************************************

  if (bunch.Bunch == -1) return CheckReconFillFlatTreeOutOfBunch(bunch);

  for (std::vector<AnaParticleB*>::const_iterator it = bunch.Particles.begin(); it!=bunch.Particles.end(); it++) {

    unsigned long bitfield = (*it)->Detector;

    if (_useTPC1)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kTPC1)) return true;
    if (_useTPC2)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kTPC2)) return true;
    if (_useTPC3)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kTPC3)) return true;
    if (_useFGD1)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kFGD1)) return true;
    if (_useFGD2)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kFGD2)) return true;
    if (_useP0D)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kP0D)) return true;
    if (_useDsECal)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kDSECAL)) return true;
    if (_useTrECal)
      if (SubDetId::HasTECALDetector(bitfield)) return true;
    if (_useP0DECal)
      if (SubDetId::HasPECALDetector(bitfield)) return true;
    if (_useSMRD)
      if (SubDetId::HasSMRDDetector(bitfield)) return true;


  }

  return false;
}


//********************************************************************
void CreateFlatTree::FillFlatTree(){
//********************************************************************

  // Save the accumulated POT since the last spill saved
  FillVar(sPOTSincePreviousSpill, _POTSincePreviousSpill);

  // Reset it to 0
  _POTSincePreviousSpill = 0;

  FillTree(flattree);
}


//********************************************************************
void CreateFlatTree::ResetCheckWriteSegment(){
//********************************************************************

  for (Int_t i=0;i<10;i++){
    _max_nodes[i] = 0;
  }
}

//********************************************************************
bool CreateFlatTree::CheckWriteSegment(SubDetId::SubDetEnum det, const AnaParticleB& seg){
//********************************************************************

  // Only save the longest segment in a given sub-detector. This is mainly to deal with tracks with more than one TPC segment in the same TPC

  // Get the subtetector number
  Int_t idet =   convUtils::GetLocalDetEnum(det, SubDetId::GetSubdetectorEnum(seg.Detector));

  if (seg.NNodes>_max_nodes[idet]){
    _max_nodes[idet] = seg.NNodes;
    return true;
  }
  else
    return false;
}


//********************************************************************
void CreateFlatTree::FilterRooTrackerVtxTree(){
//********************************************************************

  // only available for oaAnalysis input
  if (!input().InputIsOriginalTree()) return;

  AnaSpill* _spill = static_cast<AnaSpill*>(&input().GetCorrectedSpill());
  
  // Save only RooTrackerVtx with a AnaTrueVertex link

  // event, run and subrun info
  if (_spill->EventInfo){
    RunID    = _spill->EventInfo->Run;
    SubrunID = _spill->EventInfo->SubRun;
    EventID  = _spill->EventInfo->Event;
  }

  // Get RooTrackerVtx info from the converter
  TClonesArray* Vtx_input  = static_cast<oaAnalysisTreeConverter*>(&input().GetConverter())->GetVtx();
  Int_t NVtx_input         = static_cast<oaAnalysisTreeConverter*>(&input().GetConverter())->GetNVtx();

  // Get the AnaTrueVertices in the spill
  const std::vector<AnaTrueVertexB*>& vertices = _spill->TrueVertices;

  NVtx=0;
  Vtx->Clear();
  for (int roov = 0; roov < NVtx_input; roov++) {
    if (input().GetChain("NRooTrackerVtx")){
      ND::NRooTrackerVtx *lvtx = (ND::NRooTrackerVtx*) (*Vtx_input)[roov];
      if ( ! lvtx->StdHepPdg) continue;

      for (std::vector<AnaTrueVertexB*>::const_iterator it = vertices.begin(); it!=vertices.end(); it++) {
        AnaTrueVertex* vertex = static_cast<AnaTrueVertex*>(*it);
        // look for the current vertex
        if (vertex->ID == lvtx->TruthVertexID){
          // The Index of the vertex is going to change since we have now less vertices in the RooTrackerVtx tree
          vertex->RooVtxIndex=NVtx;
          (*Vtx)[NVtx++]=lvtx;
          break;
        }
      }
    }
    else{
      ND::GRooTrackerVtx *lvtx = (ND::GRooTrackerVtx*) (*Vtx_input)[roov];
      if ( ! lvtx->StdHepPdg) continue;

      for (std::vector<AnaTrueVertexB*>::const_iterator it = vertices.begin(); it!=vertices.end(); it++) {
        AnaTrueVertex* vertex = static_cast<AnaTrueVertex*>(*it);
        // look for the current vertex
        if (vertex->ID == lvtx->TruthVertexID){
          // The Index of the vertex is going to change since we have now less vertices in the RooTrackerVtx tree
          vertex->RooVtxIndex=NVtx;
          (*Vtx)[NVtx++]=lvtx;
          break;
        }
      }
    }
  }
}


//********************************************************************
void CreateFlatTree::FillRooTrackerVtxTree(){
//********************************************************************

  if (HasTree(RooTrackerVtx)){
    // Select the interesting vertices
    if (_filterRoo) FilterRooTrackerVtxTree();

    // Fill the RooTrackerVtx tree
    OutputManager::FillTree(RooTrackerVtx);

    _rooVtxManager.IncrementRooVtxEntry();
  }
}
