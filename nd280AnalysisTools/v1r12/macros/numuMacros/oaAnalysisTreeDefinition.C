#include "oaAnalysisTreeDefinition.h"

//****************************************************************************
oaAnalysisTreeDefinition::oaAnalysisTreeDefinition(const std::string& infile_name){
//****************************************************************************

  

  // set a few std::string arrays
  std::string _vtxnames_ini[_NVTXTYPES] = {"VtxFGD1","VtxFGD2","VtxP0D","VtxTPC","VtxMRD","VtxDsECal","VtxBrlECal","VtxRestOfOffAxis","VtxOther","VtxINGRID","VtxP0DECal"};  
  std::string _trajnames_ini[_NTRAJTYPES] = {"TrajsLepton","TrajsBaryon","TrajsMeson","TrajsPhoton","TrajsOtherCharged","TrajsOtherNeutral","TrajsOther"};

  for (int i=0;i<_NVTXTYPES;i++){
    _vtxnames[i]=_vtxnames_ini[i];
  }  
  for (int i=0;i<_NTRAJTYPES;i++){
    _trajnames[i] = _trajnames_ini[i];
  }  


  
  //Create Tree objects
  _mcTruthVertices = new TChain("TruthDir/Vertices");
  _mcTruthTrajectories = new TChain("TruthDir/Trajectories");
  _reconGlobal = new TChain("ReconDir/Global");
  _beamSummary = new TChain("HeaderDir/BeamSummaryData");
  _DQInfo= new  TChain("HeaderDir/BasicDataQuality");
       
  // _____________________________________________________________________________________
  // Prepare to read trees.


  //For MC fill true info (need to decide if variables are defined in standard or general)
  if(_isMC){  
    cout<<" set branches adresses "<<endl;
    for(int ii = 0; ii < _NVTXTYPES; ii ++){
      _TruthVertices[ii] = new TClonesArray("ND::TTruthVerticesModule::TTruthVertex", 50);
      _mcTruthVertices->SetBranchAddress(_vtxnames[ii].c_str(), &(_TruthVertices[ii]));
      _mcTruthVertices->SetBranchAddress(("N"+_vtxnames[ii]).c_str(), &(_NTruthVertices[ii]));
    }
    
    for(int ii = 0; ii < _NTRAJTYPES; ii ++){
      _TruthTrajs[ii] = new TClonesArray("ND::TTruthTrajectoriesModule::TTruthTrajectory", 200);
      _mcTruthTrajectories->SetBranchAddress(_trajnames[ii].c_str(), &(_TruthTrajs[ii]));
      _mcTruthTrajectories->SetBranchAddress(("N"+_trajnames[ii]).c_str(), &(_NTruthTrajs[ii]));
    }
  }  

  //Set branches from global reconstructions:
    
  _PIDs = new TClonesArray("ND::TGlobalReconModule::TGlobalPID", 20);
  _DelayedClusters = new TClonesArray("ND::TGlobalReconModule::TFgdCluster", 10);
  _TPCOthers = new TClonesArray("ND::TGlobalReconModule::TTPCOtherObject", 20);
  _Vertices = new TClonesArray("ND::TGlobalReconModule::TGlobalVertex", 20);
  _FgdTimeBins = new TClonesArray("ND::TGlobalReconModule::TFgdTimeBin", 20);
  _P0DUnused = new TClonesArray("ND::TGlobalReconModule::TGlobalHit", 100);

  _reconGlobal->SetBranchAddress("EventID",  &_EventID);
  _reconGlobal->SetBranchAddress("RunID",    &_RunID);
  _reconGlobal->SetBranchAddress("SubrunID", &_SubrunID);

  _reconGlobal->SetBranchAddress("NVertices",  &_NVertices);
  _reconGlobal->SetBranchAddress("Vertices",   &_Vertices);
  _reconGlobal->SetBranchAddress("NTPCOthers", &_NTPCOthers);
  _reconGlobal->SetBranchAddress("TPCOthers",  &_TPCOthers);
  _reconGlobal->SetBranchAddress("NPIDs",      &_NPIDs);
  _reconGlobal->SetBranchAddress("PIDs",       &_PIDs);
  _reconGlobal->SetBranchAddress("NTPC",       &_NTPC);
  _reconGlobal->SetBranchAddress("NTPCIso",  &_NTPCIso);
  _reconGlobal->SetBranchAddress("NECALIso", &_NECALIso);
  _reconGlobal->SetBranchAddress("NP0DIso",  &_NP0DIso);
  _reconGlobal->SetBranchAddress("NSMRDIso", &_NSMRDIso);
  _reconGlobal->SetBranchAddress("NFGDIso",  &_NFGDIso);  

  _reconGlobal->SetBranchAddress("NFgdTimeBins", &_NFgdTimeBins);
  _reconGlobal->SetBranchAddress("FgdTimeBins",  &_FgdTimeBins);

  _reconGlobal->SetBranchAddress("NTPCUnused",  &_NTPCUnused);
  _reconGlobal->SetBranchAddress("NFGD1Unused", &_NFGD1Unused);
  _reconGlobal->SetBranchAddress("NFGD2Unused", &_NFGD2Unused);
  _reconGlobal->SetBranchAddress("NP0DUnused",  &_NP0DUnused);
  _reconGlobal->SetBranchAddress("P0DUnused",   &_P0DUnused);
  _reconGlobal->SetBranchAddress("NSMRDTopUnused",    &_NSMRDTopUnused);
  _reconGlobal->SetBranchAddress("NSMRDBottomUnused", &_NSMRDBottomUnused);
  _reconGlobal->SetBranchAddress("NSMRDLeftUnused",   &_NSMRDLeftUnused);
  _reconGlobal->SetBranchAddress("NSMRDRightUnused",  &_NSMRDRightUnused);

  _reconGlobal->SetBranchAddress("NDelayedClusters",  &_NDelayedClusters);
  _reconGlobal->SetBranchAddress("DelayedClusters",   &_DelayedClusters);    
  _reconGlobal->SetBranchAddress("EarliestTrackTime", &_EarliestTrackMedianHitTime);


  //---- branches from BeamSummaryData -----------


  _beamSummary->SetBranchAddress("BeamSummaryData", &_BeamSummaryData);
  _beamSummary->SetBranchAddress("ND280Spill", &_ND280Spill);
  

  //---- branches from DataQuality -----------

  _DQInfo->SetBranchAddress("ND280OffFlag", &_ND280OffFlag);
  _DQInfo->SetBranchAddress("TPCFlag",    &_TPCFlag);
  _DQInfo->SetBranchAddress("TPC1Flag",   &_TPC1Flag);
  _DQInfo->SetBranchAddress("TPC2Flag",   &_TPC2Flag);
  _DQInfo->SetBranchAddress("TPC3Flag",   &_TPC3Flag);
  _DQInfo->SetBranchAddress("FGDFlag",    &_FGDFlag);
  _DQInfo->SetBranchAddress("FGD1Flag",   &_FGD1Flag);
  _DQInfo->SetBranchAddress("FGD2Flag",   &_FGD2Flag);
  _DQInfo->SetBranchAddress("ECALFlag",   &_ECALFlag);
  _DQInfo->SetBranchAddress("DSECALFlag", &_DSECALFlag);
  _DQInfo->SetBranchAddress("BarECALFlag", &_BarECALFlag);
  _DQInfo->SetBranchAddress("P0DECALFlag", &_P0DECALFlag);
  _DQInfo->SetBranchAddress("P0DFlag",    &_P0DFlag);
  _DQInfo->SetBranchAddress("SMRDFlag",   &_SMRDFlag);
  _DQInfo->SetBranchAddress("MAGNETFlag", &_MAGNETFlag);
  _DQInfo->SetBranchAddress("INGRIDFlag", &_INGRIDFlag);

}
