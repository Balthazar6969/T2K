#include "CreateMiniTree.hxx"
#include "InputManager.hxx"
#include "FlatTreeConverter.hxx"
#include "HighlandMiniTreeConverter.hxx"
#include "oaAnalysisTreeConverter.hxx"
#include "Parameters.hxx"
#include "Versioning.hxx"
#include "GeometryManager.hxx"
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
CreateMiniTree::CreateMiniTree(int argc, char *argv[]):SimpleLoopBase(argc, argv){
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
bool CreateMiniTree::Initialize(){
  //********************************************************************
  // save RooTrackerVtx tree
  _saveGeometry = (bool)ND::params().GetParameterI("highlandIO.FlatTree.SaveGeometry");

  // save RooTrackerVtx tree
  _saveRoo = (bool)ND::params().GetParameterI("highlandIO.FlatTree.SaveRooTracker");

  // Filter RooTrackerVtxTree
  _filterRoo = (bool)ND::params().GetParameterI("highlandIO.FlatTree.FilterRooTracker");

  _forceSaveAll = static_cast<bool>(ND::params().GetParameterI("highlandIO.MiniTree.ForceSaveAll"));

  _saveTPCInfo     = (bool)ND::params().GetParameterI("highlandIO.FlatTree.SaveTPCInfo");
  _saveFGDInfo     = (bool)ND::params().GetParameterI("highlandIO.FlatTree.SaveFGDInfo");
  _saveECALInfo    = (bool)ND::params().GetParameterI("highlandIO.FlatTree.SaveECALInfo");
  _saveP0DInfo     = (bool)ND::params().GetParameterI("highlandIO.FlatTree.SaveP0DInfo");
  _saveSMRDInfo    = (bool)ND::params().GetParameterI("highlandIO.FlatTree.SaveSMRDInfo");
  _saveTrackerInfo = (bool)ND::params().GetParameterI("highlandIO.FlatTree.SaveTrackerInfo");

  // save info from the local reconstruction to the mini tree?
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


  _currentGeomID=999;


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


  // Enable any corrections that have to be done while the mini tree is being created.

#if !VERSION_HAS_OFFICIAL_POT
  // Only need this correction for P5 files.

  if (ND::params().GetParameterI("highlandIO.FlatTree.EnablePOTCorrection")) {
    // Use CT4 for MR44 POT accounting, as CT5 wasn't working properly.
    _corrections.AddCorrection(CorrId::kCT4POTCorr, "ct4pot_corr", new CT4POTCorrection());
  }
#endif

  if (ND::params().GetParameterI("highlandIO.FlatTree.EnableDQCorrection")) {
    // Correct the data quality in periods when a FGD FEB wasn't working.
    _corrections.AddCorrection(CorrId::kDQCorr, "dq_corr", new DataQualityCorrection());
  }

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

  // Ignore right ECal for runs 3 and 4 as part of it is broken.
  if (ND::params().GetParameterI("highlandIO.FlatTree.EnableIgnoreRightECalCorrection")){
    _corrections.AddCorrection(CorrId::kRightECalRun3And4Corr, "ignorerightecal_corr", new IgnoreRightECalRuns3and4Correction());
  }

  // Need to be called before the ToF in order to correctly flip a track if required
  if (ND::params().GetParameterI("highlandIO.FlatTree.EnableFlipKinematicsCorrection")){
    _corrections.AddCorrection(CorrId::kFlipKinCorr, "flipkinematics_corr", new FlipKinematicsCorrection());
  }

  if (ND::params().GetParameterI("highlandIO.FlatTree.EnableToFCorrection")){
    _corrections.AddCorrection(CorrId::kToFCorr, "tof_corr", new ToFCorrection());
  }

  if (ND::params().GetParameterI("highlandIO.FlatTree.EnableMomResolMCCorrection")){
    _corrections.AddCorrection(CorrId::kMomResolMCCorr, "mom_resol_mc_corr", new MomResolMCCorrection());
  }

  if (ND::params().GetParameterI("highlandIO.FlatTree.EnableMomRangeCorrection")){
    _corrections.AddCorrection(CorrId::kMomRangeCorr, "momrange_corr", new MomRangeCorrection());
  }

  _file->mkdir("geom");

  return true;
}

//********************************************************************
void CreateMiniTree::DefineOutputTree(){
  //********************************************************************

  // Add a new tree called "MiniTree"
  AddTreeWithName(minitree,"MiniTree");

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

  SetCurrentTree(minitree);
  SetFillSingleTree(GetCurrentTree());

  // The full AnaSpill. _spill cannot be undefined here, otherwise it give problems in some platforms
  _spill=NULL;
  GetTree(minitree)->Branch("Spill", "AnaSpill", &_spill,64000,1);

  _POTSincePreviousSavedSpill   =0;
  _SpillsSincePreviousSavedSpill=0;
}


//********************************************************************
bool CreateMiniTree::Process(){
//********************************************************************

  // Set the tree to fill
  SetFillSingleTree(minitree);
  SetCurrentTree(minitree);

  // Get the corrected spill and set the branch address
  _spill = static_cast<AnaSpill*>(&input().GetCorrectedSpill());
  GetTree(minitree)->SetBranchAddress("Spill",&_spill );

  // The number of POT and Spills since the last saved Spill
  _POTSincePreviousSavedSpill    += _spill->Beam->POTSincePreviousSavedSpill;
  _SpillsSincePreviousSavedSpill += _spill->Beam->SpillsSincePreviousSavedSpill;

  // Delete the uninteresting bunches
  DeleteUninterestingBunches();

  _lastSpillSaved = false;

    // The spill is saved when there are bunches or when there are true signal vertices on it (for eff calculation)
  if (_forceSaveAll || _spill->Bunches.size()>0 || CheckTruthFillMiniTree(*_spill)){

    // Write the geometry when it changes
    if (_saveGeometry) WriteGeometry();

    // Fill the RooTrackerVtx tree
    // Must be called before FillMiniTree since when the RooTrackerVtx filtering is applied the RooVtxIndex in true vertices changes
    FillRooTrackerVtxTree();

    // Fill the minitree
    FillMiniTree();

    // Mark this spill as saved
    _lastSpillSaved=true;
  }

  return true;
}

//********************************************************************
void CreateMiniTree::Finalize(){
//********************************************************************
  // Save the last spill (if not save yet) such that we keep track of the total POT since the last saved entry.

  if (_lastSpillSaved) return;

  // Fill the RooTrackerVtx tree
    // Must be called before FillMiniTree since when the RooTrackerVtx filtering is applied the RooVtxIndex in true vertices changes
  FillRooTrackerVtxTree();

  // Fill the minitree
  FillMiniTree();

  // Delete Spills for the last entry
  input().DeleteSpill();
}

//********************************************************************
bool CreateMiniTree::CheckTruthFillMiniTree(const AnaSpill& spill){
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
bool CreateMiniTree::CheckTrueVertexReaction(const AnaTrueVertex& vtx){
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
bool CreateMiniTree::CheckTrueVertexDetector(unsigned long det){
//********************************************************************

  for (unsigned int i = 0;i<_saveTrueVertexInDet.size();i++)
    if (SubDetId::GetDetectorUsed(det,_saveTrueVertexInDet[i])) return true;

  return false;
}

//********************************************************************
bool CreateMiniTree::CheckReconFillMiniTreeOutOfBunch(const AnaBunchB& bunch){
//********************************************************************

  for (std::vector<AnaParticleB*>::const_iterator it = bunch.Particles.begin(); it!=bunch.Particles.end(); it++) {

    unsigned long bitfield = (*it)->Detector;

    if (_useTPC1outOfBunch || _forceSaveAll)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kTPC1)) return true;
    if (_useTPC2outOfBunch || _forceSaveAll)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kTPC2)) return true;
    if (_useTPC3outOfBunch || _forceSaveAll)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kTPC3)) return true;
    if (_useFGD1outOfBunch || _forceSaveAll)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kFGD1)) return true;
    if (_useFGD2outOfBunch || _forceSaveAll)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kFGD2)) return true;
    if (_useP0DoutOfBunch || _forceSaveAll)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kP0D)) return true;
    if (_useDsECaloutOfBunch || _forceSaveAll)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kDSECAL)) return true;
    if (_useTrECaloutOfBunch || _forceSaveAll)
      if (SubDetId::HasTECALDetector(bitfield)) return true;
    if (_useP0DECaloutOfBunch || _forceSaveAll)
      if (SubDetId::HasPECALDetector(bitfield)) return true;
    if (_useSMRDoutOfBunch || _forceSaveAll)
      if (SubDetId::HasSMRDDetector(bitfield)) return true;
  }

  return false;
}

//********************************************************************
bool CreateMiniTree::CheckReconFillMiniTree(const AnaBunchB& bunch){
//********************************************************************

  if (bunch.Bunch == -1) return CheckReconFillMiniTreeOutOfBunch(bunch);

  Int_t necal = 0;

  for (std::vector<AnaParticleB*>::const_iterator it = bunch.Particles.begin(); it!=bunch.Particles.end(); it++) {

    unsigned long bitfield = (*it)->Detector;

    if (_useFGD1 || _forceSaveAll)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kFGD1)) return true;
    if (_useFGD2 || _forceSaveAll)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kFGD2)) return true;
    if (_useP0D || _forceSaveAll)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kP0D)) return true;
    if (_useTPC1 || _forceSaveAll)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kTPC1)) return true;
    if (_useTPC2 || _forceSaveAll)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kTPC2)) return true;
    if (_useTPC3 || _forceSaveAll)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kTPC3)) return true;
    if (_useDsECal || _forceSaveAll)
      if (SubDetId::GetDetectorUsed(bitfield, SubDetId::kDSECAL)){
        necal++;
        if (necal>1) return true;
      }
    if (_useTrECal || _forceSaveAll)
      if (SubDetId::HasTECALDetector(bitfield)){
        necal++;
        if (necal>1) return true;
      }
    if (_useP0DECal || _forceSaveAll)
      if (SubDetId::HasPECALDetector(bitfield)) return true;
    if (_useSMRD || _forceSaveAll)
      if (SubDetId::HasSMRDDetector(bitfield)) return true;


  }

  return false;
}

//********************************************************************
void CreateMiniTree::WriteGeometry(){
//********************************************************************

  // Write the geometry when it changes
  if (_spill->GeomID != _currentGeomID){

    // Write the geometry in the geom directory
    if(_file) _file->cd("geom");
    ND::hgman().GeoManager()->Write();
    _file->cd();

    // Update the current GeomID
    _currentGeomID= _spill->GeomID;
  }

}

//********************************************************************
void CreateMiniTree::FillMiniTree(){
//********************************************************************

  // Copy arrays into std::vectors
  _spill->CopyArraysIntoVectors();

  // Update the POT and spills
  _spill->Beam->POTSincePreviousSavedSpill    = _POTSincePreviousSavedSpill;
  _spill->Beam->SpillsSincePreviousSavedSpill = _SpillsSincePreviousSavedSpill;

  // When the RooTrackerVtx tree is saved in the output file the entry must be recomputed (is not the one in the input file)
  if (HasTree(RooTrackerVtx))
    _spill->RooVtxEntry = _rooVtxManager.GetRooVtxEntry();

  // Fill the tree
  FillTree(minitree);

  // Reset to 0
  _POTSincePreviousSavedSpill   =0;
  _SpillsSincePreviousSavedSpill=0;
}

//********************************************************************
void CreateMiniTree::FillRooTrackerVtxTree(){
//********************************************************************

  if (HasTree(RooTrackerVtx)){
    // Select the interesting vertices
    if (_filterRoo) FilterRooTrackerVtxTree();

    // Fill the RooTrackerVtx tree
    OutputManager::FillTree(RooTrackerVtx);

    _rooVtxManager.IncrementRooVtxEntry();
  }
}

//********************************************************************
void CreateMiniTree::FilterRooTrackerVtxTree(){
//********************************************************************

  // only available for oaAnalysis input
  if (!input().InputIsOriginalTree()) return;

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
void CreateMiniTree::DeleteUninterestingBunches(){
//********************************************************************

  std::vector<AnaParticleB*> particlesToConsider;

  // Loop over bunches and schedule for deletion the ones not satisfying the conditions about reconstructed objects
  std::vector<Int_t> bunchesToDelete;
  for (std::vector<AnaBunchC*>::iterator it=_spill->Bunches.begin();it!=_spill->Bunches.end();it++){
    AnaBunchB* bunch = static_cast<AnaBunchB*>(*it);
    // Schedule bunches not fulfilling the requirements for deletion
    if (!CheckReconFillMiniTree(*bunch))
      bunchesToDelete.push_back(bunch->Bunch);
    else{
      // Save in a vector all belonging to vertices in that bunch
      for (std::vector<AnaVertexB*>::iterator it2=bunch->Vertices.begin();it2!=bunch->Vertices.end();it2++){
        AnaVertexB* vertex = *it2;
        for (Int_t i = 0;i<vertex->nParticles;i++){
          AnaParticleB* particle = vertex->Particles[i];
          particlesToConsider.push_back(particle);
        }
      }
    }
  }

  // Delete the uninteresting bunches
  for (UInt_t i=0;i<bunchesToDelete.size();i++){
    for (std::vector<AnaBunchC*>::iterator it=_spill->Bunches.begin();it!=_spill->Bunches.end();it++){
      AnaBunchB* bunch = static_cast<AnaBunchB*>(*it);
      if (bunch->Bunch == bunchesToDelete[i]){
        bool found=false;
        // Don't delete if there is a particle in this bunch which belongs to a vertex in another bunch that is not deleted
        for (std::vector<AnaParticleB*>::iterator it2=bunch->Particles.begin();it2!=bunch->Particles.end();it2++){
          AnaParticleB* particle2 = *it2;
          for (std::vector<AnaParticleB*>::iterator it3=particlesToConsider.begin();it3!=particlesToConsider.end();it3++){
            AnaParticleB* particle3 = *it3;
            if (particle2==particle3){
              found=true;
              break;
            }
          }
          if (found) break;
        }
        if (!found){
          delete bunch;
          _spill->Bunches.erase(it);
          break;
        }
      }
    }
  }

  // Delete OutOfBunch
  if (_spill->OutOfBunch && !CheckReconFillMiniTreeOutOfBunch(*_spill->OutOfBunch)){

    bool found=false;
    for (std::vector<AnaParticleB*>::iterator it2=_spill->OutOfBunch->Particles.begin();it2!=_spill->OutOfBunch->Particles.end();it2++){
      AnaParticleB* particle2 = *it2;
      for (std::vector<AnaParticleB*>::iterator it3=particlesToConsider.begin();it3!=particlesToConsider.end();it3++){
        AnaParticleB* particle3 = *it3;
        if (particle2==particle3){
          found=true;
          break;
        }
      }
      if (found) break;
    }
    if (!found){

      delete _spill->OutOfBunch;
      _spill->OutOfBunch=NULL;
    }
  }

}
