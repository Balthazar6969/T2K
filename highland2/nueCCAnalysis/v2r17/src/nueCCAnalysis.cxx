#include "nueCCAnalysis.hxx"
#include "nueCCSelection.hxx"
#include "nueCCFGD2Selection.hxx"

#include "CutUtils.hxx"
#include "nueCCUtils.hxx"
#include "CategToolsNueCC.hxx"

#include "FiducialVolumeDefinition.hxx"
#include "Parameters.hxx"
#include "UseGlobalAltMomCorrection.hxx"
#include "CategoriesUtils.hxx"
#include "BasicUtils.hxx"
#include "baseToyMaker.hxx"

#include "NuDirUtils.hxx"

//********************************************************************
nueCCAnalysis::nueCCAnalysis(AnalysisAlgorithm* ana) : baseTrackerAnalysis(ana) {
//********************************************************************

  // Add the package version
  ND::versioning().AddPackage("nueCCAnalysis", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("NUECCANALYSISROOT")));
}

//********************************************************************
bool nueCCAnalysis::Initialize(){
//********************************************************************
  
  // Initialize the baseTrackerAnalysis
  if(!baseTrackerAnalysis::Initialize()) return false;
  
  // Minimum accum level to save event Int_to the output tree
  SetMinAccumCutLevelToSave(ND::params().GetParameterI("nueCCAnalysis.MinAccumLevelToSave"));

  // which analysis: FGD1, FGD2 or FGDs
  _whichFGD = ND::params().GetParameterI("nueCCAnalysis.Analysis.Sample.whichFGD");

  // Save all the secondary tarcks in the microtree
  _savesecondaries = ND::params().GetParameterI("nueCCAnalysis.Analysis.SaveSecondaries");

  // Extrend the list of toy variables
  _addmoretoyvariables = ND::params().GetParameterI("nueCCAnalysis.MicroTrees.AdditionalToyVars");

  // Save gamma initiator
  _savegammainitiator = ND::params().GetParameterI("nueCCAnalysis.Analysis.SaveGammaInitiator");

  // Add new categories specific for nue analysis
  nue_categs::AddCategories("", false);
  nue_categs::AddCategories("fgd2", false);

  anaUtils::AddStandardCategories();
  anaUtils::AddStandardCategories("fgd2");

  return true;
}

//********************************************************************
void nueCCAnalysis::DefineSelections(){
//********************************************************************

  // ----- Inclusive CC -----------
  if      (_whichFGD==1) // FGD1
  sel().AddSelection("kTrackerNueCC", "inclusive nueCC selection", new nueCCSelection(false));     // true/false for forcing break
  else if (_whichFGD==2) // FGD2
    sel().AddSelection("kTrackerNueCCFGD2", "inclusive nueCCFGD2 selection", new nueCCFGD2Selection(false));     // true/false for forcing break
  else if (_whichFGD==3) { // both FGDs, in 2 selections
    sel().AddSelection("kTrackerNueCC", "inclusive nueCC selection", new nueCCSelection(false));     // true/false for forcing break
    sel().AddSelection("kTrackerNueCCFGD2", "inclusive nueCCFGD2 selection", new nueCCFGD2Selection(false));     // true/false for forcing break
  }
}

//********************************************************************
void nueCCAnalysis::DefineCorrections(){
//********************************************************************

  // Some corrections are defined in baseTrackerAnalysis
  baseTrackerAnalysis::DefineCorrections();

  //------------ Corrections -----------
  
  // Change the main fit momentum by the muon alternate momentum, but only for P5
#if !VERSION_HAS_EQUIVALENT_MAIN_AND_ALT_FITS
  if (ND::params().GetParameterI("nueCCAnalysis.Corrections.UseGlobalEleMom.Enable") == 1) {
    corr().AddCorrection("altmom_corr", new UseGlobalAltMomCorrection(UseGlobalAltMomCorrection::kElectron));
  }
#endif
  
}  

//********************************************************************
void nueCCAnalysis::DefineTruthTree(){
//********************************************************************

  // Variables from baseTrackerAnalysis
  baseTrackerAnalysis::DefineTruthTree();
  
  AddVarF(output(), 	truelepton_costheta_z,       "True Cos(Theta) of the true electron w.r.t z-axis");
  AddVarF(output(), 	truelepton_costheta_offaxis, "True Cos(Theta) of the true electron w.r.t off-axis direction");
  AddVarF(output(), 	truelepton_costheta_nuDir,   "True Cos(Theta) of the true electron w.r.t. nu direction");
  AddVar4VF(output(),   truelepton_pos,              "True position of the true electron");
  AddVarI(output(),     truelepton_det,              "FGD where the true electron start position is");
  AddVarF(output(),     Q2_true,                     "True Q2");

  // Variables to handle the number of targets in xsTool
  AddVarI(output(),     truevtx_mass_component,      "Mass component enum related to the true vtx position (FGD1 / FGD2_scint / FGD2_water");
  // this info is contained in the categories
  // defining these in truth and default was necessary for xsTool use at one point
  // leaving this in for now ... but will become redundent and deleted
  AddVarI(output(), 	nue_signal,                   "Whether this vertex is a true NuE interaction");
  AddVarI(output(), 	nue_ccqe,                     "Whether this vertex is a true NuE CCQE interaction");
  AddVarI(output(), 	nue_ccnonqe,                  "Whether this vertex is a true NuE CCnonQE interaction");
  AddVarI(output(), 	nue_cc0pi,                    "Whether this vertex is a true NuE CC0pi interaction");
  AddVarI(output(), 	nue_ccother,                  "Whether this vertex is a true NuE CCOther interaction");

  if(_savegammainitiator){
    AddVarI(output(),   InitiatorPDG,          "Gamma comes from what particle");
    AddVarI(output(),   InitiatorParentPDG,    "Initiator parentpdg");
    AddVarF(output(),   InitiatorMom,          "initiator momentum");
    AddVar3VF(output(), InitiatorDir,          "initiator direction");
  }
}

//********************************************************************
void nueCCAnalysis::DefineMicroTrees(bool addBase){
//********************************************************************
  
  // -------- Add variables to the analysis tree ---------------------- 
  // Variables from baseTrackerAnalysis
  if (addBase) baseTrackerAnalysis::DefineMicroTrees(addBase);

  // Toy variables
  // Variables to handle the number of targets in xsTool
  AddToyVarI(output(), truevtx_mass_component, "mass component enum related to the true vtx position (FGD1 / FGD2_scint / FGD2_water");

  // Recon variables
  AddToyVarF(output(),selelec_mom,                     "Recon momentum of the selected track");
  AddToyVarF(output(),selelec_costheta,                "Electron candidate reconstructed polar angle wrt neutrino direction");
  AddToyVarF(output(),selelec_ENuRec,                  "The reconstructed neutrino energy, assuming CCQE kinematics");
  AddToyVarF(output(),selelec_Q2Rec,                   "The reconstructed Momentum Transfer, assuming CCQE kinematics");

  if(_addmoretoyvariables){
     // 1st TPC variables
    AddToyVarF(output(),   selelec_tpcdedx,            "First TPC dEdx");
    AddToyVarF(output(),   selelec_pullmuon,           "Muon pull in the most upstream TPC");
    AddToyVarF(output(),   selelec_pullelec,           "Electron pull in the most upstream TPC");
    AddToyVarF(output(),   selelec_pullpion,           "Pion pull in the most upstream TPC");
    AddToyVarF(output(),   selelec_pullprot,           "Proton pull in the most upstream TPC");
    
    // 2nd TPC variables
    AddToyVarF(output(),   selelec_pullmuon2nd,        "Muon pull of the second TPC segment, if the track uses more than 1 TPC");
    AddToyVarF(output(),   selelec_pullelec2nd,        "Electron pull of the second TPC segment, if the track uses more than 1 TPC");

    // Ecal
    AddToyVarF(output(),   selelec_ecalemene,          "Reconstructed EM energy deposit (in MeV) of the selected track's ECal segment");

    // FGD
    AddToyVarVF(output(),  selelec_fgd_pullprot,       "Electron candidate protn pull in each FGD",             NMAXFGDS);
  }
  else{
    // 1st TPC variables
    AddVarF(output(),   selelec_tpcdedx,               "First TPC dEdx");
    AddVarF(output(),   selelec_pullmuon,              "Muon pull in the most upstream TPC");
    AddVarF(output(),   selelec_pullelec,              "Electron pull in the most upstream TPC");
    AddVarF(output(),   selelec_pullpion,              "Pion pull in the most upstream TPC");
    AddVarF(output(),   selelec_pullprot,              "Proton pull in the most upstream TPC");
    
    // 2nd TPC variables
    AddVarF(output(),   selelec_pullmuon2nd,           "Muon pull of the second TPC segment, if the track uses more than 1 TPC");
    AddVarF(output(),   selelec_pullelec2nd,           "Electron pull of the second TPC segment, if the track uses more than 1 TPC");

    // Ecal
    AddVarF(output(),   selelec_ecalemene,             "Reconstructed EM energy deposit (in MeV) of the selected track's ECal segment");

    // FGD
    AddVarVF(output(),  selelec_fgd_pullprot,          "Electron candidate protn pull in each FGD",             selelec_nfgds);
  }

  // Add the standard tree variables
  AddVarF(output(),   selelec_costheta_z,              "Recon Cos(Theta) of the selected track w.r.t z-axis");
  AddVarF(output(),   selelec_costheta_offaxis,        "Recon Cos(Theta) of the selected track w.r.t. off-axis direction");
  AddVarF(output(),   selelec_charge,                  "Recon charge of the selected track");
  AddVarF(output(),   selelec_phi,                     "Recon Phi of the selected track");
  AddVarF(output(),   selelec_pt,                      "Pt of the selected track");
  AddVar4VF(output(), selelec_startpos,                "Recon position of the selected track");
  AddVar4VF(output(), selelec_startdir,                "Recon direction of the selected track");  
  AddVarF(output(),   selelec_momErr,                  "Uncertainty on the reconstructed momentum of the selected track");
  AddVarF(output(),   selelec_momEleFit,               "Reconstructed momentum of selected track, assuming the electron hypothesis");
  AddVarF(output(),   selelec_momMuonFit,              "Reconstructed momentum of selected track, assuming the muon hypothesis");
  AddVarF(output(),   selelec_ElemomECalEntrance,      "Reconstructed momentum of selected track, at the entrance to the ECal");
  AddVarI(output(),   selelec_tpcNNodes,               "The number of nodes in the most upstream TPC segment of the selected track");
  AddVarI(output(),   selelec_FGD,                     "The FGD in which the selected track starts (either 0 or 1)");
  AddVarF(output(),   selelec_ElemomTPCBack,           "The momentum of the selected track at the back of the most downstream TPC segment");
  AddVar4VF(output(), selelec_tpcBackPos,              "The position of the selected track at the back of the most downstream TPC segment");
  AddVar3VF(output(), selelec_tpcBackDir,              "The direction of the selected track at the back of the most downstream TPC segment");
  AddVarI(output(),   selelec_inp0d,                   "Is the main track goes in the P0D");
  
  // FGD variables
  AddVarVI(output(),  selelec_fgd_det,                 "Electron candidate FGD number",                         selelec_nfgds);
  AddVarVF(output(),  selelec_fgd_x,                   "Electron candidate track length in each FGD",           selelec_nfgds);
  AddVarVF(output(),  selelec_fgd_E,                   "Electron candidate energy deposit in each FGD",         selelec_nfgds);
  AddVarVF(output(),  selelec_fgd_V11,                 "Electron candidate V11 vertex activity in each FGD",    selelec_nfgds);
  AddVarVF(output(),  selelec_fgd_V33,                 "Electron candidate V33 vertex activity in each FGD",    selelec_nfgds);
  AddVarVF(output(),  selelec_fgd_V55,                 "Electron candidate V55 vertex activity in each FGD",    selelec_nfgds);
  AddVarVF(output(),  selelec_fgd_V77,                 "Electron candidate V77 vertex activity in each FGD",    selelec_nfgds);
  AddVarVF(output(),  selelec_fgd_VLayer,              "Electron candidate VLayer vertex activity in each FGD", selelec_nfgds);
  AddVarVF(output(),  selelec_fgd_pullmu,              "Electron candidate muon pull in each FGD",              selelec_nfgds);
  AddVarVF(output(),  selelec_fgd_pullpi,              "Electron candidate pion pull in each FGD",              selelec_nfgds);
  AddVarVF(output(),  selelec_fgd_pullno,              "Check if the FGD pulls should be used",                 selelec_nfgds);
  AddVarVI(output(),  selelec_fgd_containment,         "Electron candidate containment in each FGD",            selelec_nfgds);
  
  // ECal variables
  AddVarI(output(),   selelec_nECALs,                  "Number of Ecal clusters found");
  AddVarI(output(),   selelec_ecaldetector,            "The detector in which the selected track's ECal segment was found,  DsECal is 6.  Barrel ECal: 7- top, 8-bottom, 9-left, 10-right ");
  AddVarI(output(),   selelec_ecalnhits,               "The number of hits in the selected track's ECal segment");
  AddVarI(output(),   selelec_ecalMostUpStreamLayerHit,"The first layer hit in the selected track's ECal segment");
  AddVarF(output(),   selelec_ecalmipem,               "MipEm value of the selected track's ECal segment, if it uses the DSECal. Negative means more MIP-like, positive means more EM shower-like.");
  AddVarF(output(),   selelec_ecalmippion,             "MipPion value of the selected track's ECal segment");
  AddVarF(output(),   selelec_ecalemhip,               "EmHip value of the selected track's ECal segment");
  AddVarI(output(),   selelec_ecalcontained,           "Whether the selected track's ECal segment is 'contained' or not, according to the definition used in the analysis");
  AddVar3VF(output(), selelec_ecalshowerpos,           "The reconstructed position of the selected track's ECal segment (according to the shower-based reconstruction)");
  AddVarF(output(),   selelec_ecallength,              "Length of the ECal segment");
  
  // ToF
  AddVarF(output(),   selelec_ToF_P0D_FGD1,            "ToF between P0D and FGD1");
  AddVarF(output(),   selelec_ToF_ECal_FGD1,           "ToF between BrEcal and FGD1");
  AddVarF(output(),   selelec_ToF_ECal_FGD2,           "ToF between BrEcal and FGD2");
  AddVarF(output(),   selelec_ToF_FGD1_FGD2,           "ToF between FGD1 and FGD2");
  AddVarF(output(),   selelec_ToF_DsECal_FGD1,         "ToF between DsEcal and FGD1");
  AddVarF(output(),   selelec_ToF_DsECal_FGD2,         "ToF between DsEcal and FGD2");
  
  // Truth variables of the selected track (taken from true track)
  AddVarI(output(),   selelec_true_pdg,                "PDG code of true particle");
  AddVarI(output(),   selelec_true_det,                "True detector of the selected track");
  AddVarF(output(),   selelec_true_mom,                "True momentum of the selected track");
  AddVar4VF(output(), selelec_true_pos,                "True position of the selected track");
  AddVar4VF(output(), selelec_true_endpos,             "True endposition of the selected track");
  AddVar3VF(output(), selelec_true_dir,                "True directionof the selected track"); 
  AddVarF(output(),   selelec_true_costheta_z,         "True Cos(Theta) of the selected track w.r.t z-axis");
  AddVarF(output(),   selelec_true_costheta_offaxis,   "True Cos(Theta) of the selected track w.r.t off-axis direction");
  AddVarF(output(),   selelec_true_costheta_nuDir,     "True Cos(Theta) of the selected track w.r.t nu direction");
  
  // truth variables of true electron (taken from true vertex)
  AddVarI(output(),   truelepton_pdg,                  "Pdg of the true lepton");
  AddVarI(output(),   truelepton_det,                  "Detector where the true lepton start position is");
  AddVarF(output(),   truelepton_mom,                  "True momentum of the true lepton");
  AddVarF(output(),   truelepton_costheta_z,           "True Cos(Theta) of the true lepton w.r.t. z-axis");
  AddVarF(output(),   truelepton_costheta_nuDir,       "True Cos(Theta) of the true lepton w.r.t. nu direction");
  AddVarF(output(),   truelepton_costheta_offaxis,     "True Cos(Theta) of the true lepton w.r.t off-axis direction");
  AddVar4VF(output(), truelepton_pos,                  "True position of the true lepton");
  AddVar3VF(output(), truelepton_dir,                  "True direction of the true lepton");
  AddVarF(output(),   truelepton_nuErecQE,             "Neutrino reconstructed energy with true lepton kinematics in ccqe formula");
  
  // TPC veto variables
  AddVarF(output(),   TPCVetoDeltaz,                   "Z difference between the selected track and the track used in the TPC1 veto");
  AddVarF(output(),   TPCVetoMomentum,                 "Momentum of the track used in the TPC1 veto");
  AddVarI(output(),   TPCVetoCharge,                   "Charge (+1/-1) of the track used in the TPC1 veto.");
  AddVarI(output(),   TPCVetoNNearP0DTracks,           "Number of FGD-TPC tracks near the vertex entering the P0D");
  AddVarI(output(),   TPCVetoNNearTracks,              "Number of FGD-TPC tracks near the vertex");
  AddVarI(output(),   TPCVetoNFarTracks,               "Number of FGD-TPC tracks far from vertex");
  AddVarF(output(),   SecMomFGDDeltaZ,                    "DZ between the selected track and the second most energetic track in the same FGD");
  
  // P0D veto variables
  AddVarI(output(),   NP0DVetoP0DTracks,               "Number of reconstructed objects in the P0D FV");
  AddVarI(output(),   NP0DVetoFGD1Tracks,              "Number of reconstructed objects in the FGD1 FV");
  AddVarI(output(),   NP0DVetoP0DEcalTracks,           "Number of P0DEcal objects");
  AddVarI(output(),   FGD2ShowerNFGD1TPC2Tracks,       "Number of FGD1-TPC2 tracks used in the FGD2 shower cut");
  AddVarI(output(),   FGD2ShowerNFGD2TPC3Tracks,       "Number of FGD2-TPC3 tracks used in the FGD2 shower cut");
  AddVarI(output(),   FGDOOFVActivity,                 "Check for OOFV FGD Iso");
  AddVarI(output(),   NFGDOOFVTracksReqTPC,            "Number of OOFV FGD-TPC tracks");
  
  // ECal veto variables
  AddVarF(output(),   ECALMinZ,                        "Z distance between the selected track and the most upstream ECal segment");
  AddVarF(output(),   ECALNCMinZ,                      "Z distance between the selected track and the most upstream ECal neutral cluster segment");
  AddVarF(output(),   TPCECALdist,                     "Distance between the end of the most downstream TPC segment of the selected track and the nearest ECal track");
  AddVar3VF(output(), ECALPosECALSegment,              "Position of the ECal segment of the closest ECal track to the selected track");
  AddVar3VF(output(), ECALPosGlobalTrack,              "Start position of the closest ECal track to the selected track");
  AddVar3VF(output(), ECALTrueVertexPos,               "True vertex position of the closest ECal track to the selected track");

  // Pair-finder variables
  AddVarF(output(),   PairTrack_mom,                   "Momentum of the matched e-/e+ pair");
  AddVarF(output(),   PairTrack_costheta,              "Angle of the matched e-/e+ pair");
  AddVarI(output(),   PairTrack_pdg,                   "Pdg of the matched e-/e+ pair");
  AddVarF(output(),   PairTrack_charge,                "Charge of the matched e-/e+ pair");
  AddVarF(output(),   PairTrack_InvMass,               "Invariant mass of the matched e-/e+ pair");
  AddVar4VF(output(), PairTrack_startdir,              "Recon direction of the matched e-/e+ paired track");
  AddVarF(output(),   PairTrack_DistToMainTrack,       "Distance of the matched e-/e+ pair");
  AddVar4VF(output(), PairTrack_startpos,              "Recon position of the matched e-/e+ paired track");  
  AddVar4VF(output(), PairTrack_true_startdir,         "True direction of the matched e-/e+ paired track"); 
  AddVar4VF(output(), PairTrack_true_startpos,         "True position of the matched e-/e+ paired track");
  AddVarI(output(),   PairTrack_tpcNNodes,             "Number of TPC hits of the matched e-/e+ pair");
  AddVarF(output(),   PairTrack_pullmuon,              "Paired track Muon pull in the most upstream TPC");
  AddVarF(output(),   PairTrack_pullelec,              "Paired track Electron pull in the most upstream TPC");
  AddVarF(output(),   PairTrack_pullpion,              "Paired track Pion pull in the most upstream TPC");
  AddVarF(output(),   PairTrack_pullprot,              "Paired track Proton pull in the most upstream TPC");
  AddVar3VF(output(), PairTrack_gamma_dir, 	       "Direction of (PairTrack + SelectedTrack) ") ;
  AddVar3VF(output(), PairTrack_gamma_true_dir,        "True direction of (PairTrack + SelectedTrack) ") ;

  AddVarF(output(),   TruePairTrack_true_mom,          "Momentum of the true paired track from gamma background");
  AddVarI(output(),   TruePairTrack_pdg,               "PDG of the true paired track from gamma background");
  AddVarF(output(),   selelec_parent_true_mom,         "Momentum of the selected track's parent - only for background events");
  AddVarI(output(),   selelec_parent_pdg,              "PDG of the selected track's parent - only for background events");
  AddVarI(output(),   selelec_parent_id,               "ID of the selected track's parent - only for background events");
  AddVarF(output(),   selelec_gparent_true_mom,        "Momentum of the selected tracks's grandparent - only for background events");
  AddVarI(output(),   selelec_gparent_pdg,             "PDG of the selected tracks's grandparent - only for background events");
  AddVarI(output(),   selelec_gparent_id,              "ID of the selected track's grandparent - only for background events");
  AddVarVF(output(),  selelec_true_brem_mom,           "Momentum of the Brem photon",           selelec_true_nbrem);
  AddVarVI(output(),  selelec_true_brem_detector,      "Detector of the Brem photon",           selelec_true_nbrem);
 
  // Truth variables
  AddVarI(output(),   NuParentPDGRaw,                  "True parent that decayed to create the neutrino");
  AddVarI(output(),   TargetPDG,                       "True target of the neutrino Int_teraction (oxygen, carbon etc.)");
  AddVar4VF(output(), NuParentDecPoint,                "True position at which the neutrino was created");
  AddVarI(output(),   NTruePions,                      "Number of true pions ejected from the nucleus");
  AddVarI(output(),   NTruePi0,                        "Number of true pi0 ejected from the nucleus");
  AddVarI(output(),   NTrueKaonRhoEta,                 "Number of true kaons-rhos-etas ejected from the nucleus");
  AddVarF(output(),   Q2_true,                         "true Q2");
  // this info is contained in the categories
  // defining these in truth and default was necessary for xsTool use at one point
  // leaving this in for now ... but will become redundent and deleted
  AddVarI(output(),   nue_signal,                      "Whether this vertex is a true NuE Interaction");
  AddVarI(output(),   nue_ccqe,                        "Whether this vertex is a true NuE CCQE Interaction");
  AddVarI(output(),   nue_ccnonqe,                     "Whether this vertex is a true NuE CCnonQE Interaction");
  AddVarI(output(),   nue_cc0pi,                       "Whether this vertex is a true NuE CC0pi Interaction");
  AddVarI(output(),   nue_ccother,                     "Whether this vertex is a true NuE CCOther Interaction");
  
  // All secondary FGD-TPC tracks
  if(_savesecondaries){
    AddVarVI(output(), FGDTPCTrack_det,                     "FGD detector of the FGD-TPC track",  Nfgdtpc);
    AddVarVF(output(), FGDTPCTrack_mom,                     "Recon momentum FGD-TPC track",       Nfgdtpc);
    AddVarVF(output(), FGDTPCTrack_charge,                  "Charge FGD-TPC track",               Nfgdtpc);
    AddVarVF(output(), FGDTPCTrack_costheta,                "Recon cos(theta) FGD-TPC track",     Nfgdtpc);
    AddVarMF(output(), FGDTPCTrack_startpos,                "Recon position FGD-TPC track",       Nfgdtpc, -40 ,4);
    AddVarMF(output(), FGDTPCTrack_startdir,                "Recon direction FGD-TPC track",      Nfgdtpc, -40, 3);
    AddVarVF(output(), FGDTPCTrack_pullmuon,                "Muon pull FGD-TPC track",            Nfgdtpc);
    AddVarVF(output(), FGDTPCTrack_pullelec,                "Electron pullFGD-TPC track",         Nfgdtpc);
    AddVarVF(output(), FGDTPCTrack_pullpion,                "Pion pull FGD-TPC track",            Nfgdtpc);
    AddVarVF(output(), FGDTPCTrack_pullprot,                "Proton pull FGD-TPC track",          Nfgdtpc);
    AddVarVI(output(), FGDTPCTrack_tpcNNodes,               "TPC nodes FGD-TPC track",            Nfgdtpc);
    AddVarVI(output(), FGDTPCTrack_pdg,                     "PDG FGD-TPC track",                  Nfgdtpc);
    
    AddVarVF(output(), FGDTPCTrack_MomECalEntrance,         "Momentum at the entrance of the Ecal",      Nfgdtpc);
    AddVarVF(output(), FGDTPCTrack_EMEnergy,                "Ecal EM Energy",                            Nfgdtpc);
    AddVarVF(output(), FGDTPCTrack_MipEm,                   "Ecal MipEm",                                Nfgdtpc);
    AddVarVF(output(), FGDTPCTrack_MipPion,                 "Ecal MipPion",                              Nfgdtpc);
    AddVarVF(output(), FGDTPCTrack_EmHip,                   "Ecal EmHip",                                Nfgdtpc);
    AddVarMF(output(), FGDTPCTrack_ShowerPosition,          "Ecal Shower Position",                      Nfgdtpc, -40 ,4);
    AddVarVI(output(), FGDTPCTrack_ECALdet,                 "Ecal detector",                             Nfgdtpc);
    AddVarVI(output(), FGDTPCTrack_NHits,                   "Ecal number of shower hits",                Nfgdtpc);
    AddVarVI(output(), FGDTPCTrack_MostUpStreamLayerHit,    "Ecal first layer",                          Nfgdtpc);
    AddVarVI(output(), FGDTPCTrack_IsEcalContained,         "Ecal containtment",                         Nfgdtpc);
    
    AddVarVI(output(), fgdiso_det,                          "In which FGD is the FGD-iso selected",    NisoFgds);
    AddVarVF(output(), fgdiso_x,                            "Length of the FGD-iso tracks",            NisoFgds);
    AddVarVF(output(), fgdiso_E,                            "Energy deposit of the FGD-iso tracks",    NisoFgds);
    AddVarVF(output(), fgdiso_pullmu,                       "Muon pull of the FGD-iso tracks",         NisoFgds);
    AddVarVF(output(), fgdiso_pullpi,                       "Pion pull of the FGD-iso tracks",         NisoFgds);
    AddVarVF(output(), fgdiso_pullprot,                     "Proton pull of the FGD-iso tracks",       NisoFgds);
    AddVarMF(output(), fgdiso_position,                     "Recon position of the the FGD-iso tracks",NisoFgds,-40,4);
    AddVarVI(output(), fgdiso_containment,                  "Containment of the FGD-iso tracks",       NisoFgds);
    AddVarVF(output(), fgdiso_costheta,                     "Cos(theta) of the FGD-iso tracks",        NisoFgds);
    AddVarVI(output(), fgdiso_pdg,                          "PDG of the FGD-iso tracks",               NisoFgds);

    AddVarI(output(),   NFGDTracksReqTPC,                   "Number of FGD-TPC tracks in the same FGD as the selected track");
    AddVarI(output(),   NFGD1TracksReqTPC1,                 "Number of FGD1-TPC1 tracks");
    AddVarI(output(),   NFGD1TracksReqTPC2,                 "Number of FGD1-TPC2 tracks");
    AddVarI(output(),   NFGD2TracksReqTPC2,                 "Number of FGD2-TPC2 tracks");
    AddVarI(output(),   NFGD2TracksReqTPC3,                 "Number of FGD2-TPC3 tracks");
    AddVarI(output(),   NFGD1TracksReqEcal,                 "Number of FGD1-Ecal tracks");
    AddVarI(output(),   NFGD2TracksReqEcal,                 "Number of FGD2-Ecal tracks");
    AddVarF(output(),   FGDDistanceReqTpc,                  "Distance between the selected track and the nearest FGD-TPC track");
    AddVarF(output(),   FGDMaxDistanceReqTpc,               "Distance between the selected track and the most far away FGD-TPC track");
    AddVarF(output(),   FGDDistanceNoReqTpc,                "Distance between the selected track and the nearest FGD-TPC or FGD-only track");
    AddVarF(output(),   FGDMaxDistanceNoReqTpc,             "Distance between the selected track and the most far away FGD-TPC or FGD-only track");
    AddVarF(output(),   FGD2XYMaxTrackDist,                 "Maximum XY distance of any tracks in FGD2");
    
    // Neutral clusters and brem 
    AddVarI(output(),   NECalTracks,                        "Number of ECal tracks that aren't attached to the selected track");
    AddVarI(output(),   NDsEcalNeutralClusters,             "Number of DsEcal neutral cluster");
    AddVarI(output(),   NBrEcalNeutralClusters,             "Number of BrEcal neutral cluster");
    AddVarF(output(),   MostEnergeticEcalNeutralCluster,    "Most energetic ecal neutral cluster");
    AddVarF(output(),   EcalNeutralClusterEnergySum,        "The energy sum of the all the ecal neutral clusters");
    AddVarI(output(),   EcalClusterMostEnergeticThanHMT,    "Number of Tracker-ECal tracks that aren't the HM track and have EMEnergy larger than the momentum of the HM track");
    AddVarI(output(),   BremNeutralClusters,                "Number of ECal neutral cluster selected as Brem candidates associated with the HM track");
    AddVarF(output(),   BremNeutralClustersEnergy,          "Energy of the ECal neutral cluster selected as Brem candidates associated with the HM track");
    AddVarF(output(),   BremNeutralClustersMinTheta,        "Minimum theta of the ECal neutral cluster selected as Brem candidates associated with the HM track");
    AddVarF(output(),   BremNeutralClustersMinPhi,          "Minimum phi of the ECal neutral cluster selected as Brem candidates associated with the HM track");
    AddVarI(output(),   FragmentsNeutralClusters,           "Number of ECal neutral cluster classified as shower fragments from the main shower");
    AddVarF(output(),   FragmentsDistanceToShower,          "Distance of the fragment ecal shower to electron shower");
    AddVarI(output(),   NTPCTracksReqEcal,                  "Number of TPC-Ecal tracks");
  }

  if(_savegammainitiator){
    AddVarI(output(),   InitiatorPDG,          "Gamma comes from what particle");
    AddVarI(output(),   InitiatorParentPDG,    "Initiator parentpdg");
    AddVarF(output(),   InitiatorMom,          "initiator momentum");
    AddVar3VF(output(), InitiatorDir,          "initiator direction");
  }

}

//********************************************************************
void nueCCAnalysis::FillMicroTrees(bool addBase){
//********************************************************************

  // Variables from baseTrackerAnalysis
  if (addBase) baseTrackerAnalysis::FillMicroTrees(addBase);

  if (!box().MainTrack) return;

  // Save the basic variables 
  FillTrackVars();
  FillTPCVetoVars();
  FillP0DVetoVars();
  FillECalVetoVars();
  FillPairFinderVars();
  FillTruthVars();
  FillToFVars();
  
  if(_savesecondaries){
    FillEcalNeutralClusterVars();
    FillAllTracksVars();
  }  
}

//********************************************************************
void nueCCAnalysis::FillTruthVars() {
//********************************************************************
  
  AnaTrack* track = static_cast<AnaTrack*> ( box().MainTrack );
  if (!track) return;
	
  if(!track->GetTrueParticle()) return;
  
  // True Track
  AnaTrueParticle* truth = static_cast<AnaTrueParticle*> (box().MainTrack->TrueObject);
  if(!truth) return;
  
  output().FillVar(selelec_true_pdg, truth->PDG);
  output().FillVar(selelec_true_mom, truth->Momentum);
  output().FillVar(selelec_true_det, anaUtils::GetDetector(truth->Position));
  output().FillVectorVarFromArray(selelec_true_pos, truth->Position, 4);
  output().FillVectorVarFromArray(selelec_true_endpos, truth->PositionEnd, 4);
  output().FillVectorVarFromArray(selelec_true_dir, truth->Direction, 3);

  Float_t selelec_true_cosT_z  = truth->Direction[2];
  output().FillVar(selelec_true_costheta_z, selelec_true_cosT_z);

  // ND280 position to get off-axis angle
  Float_t _nd280_pos[3] = {-3.221999, -8.14599, 280.10};

  Float_t selelec_true_cosT_offaxis = (Float_t)cos(anaUtils::ArrayToTVector3(truth->Direction).Angle(_nd280_pos));
  output().FillVar(selelec_true_costheta_offaxis, selelec_true_cosT_offaxis);
  	  
  // True Vertex
  AnaTrueVertex* vtx = static_cast<AnaTrueVertex*> (box().MainTrack->GetTrueParticle()->TrueVertex);
  if(!vtx) return;

  Float_t selelec_true_cosT_nuDir = (Float_t)cos(anaUtils::ArrayToTVector3(truth->Direction).Angle(anaUtils::ArrayToTVector3(vtx->NuDir)));
  output().FillVar(selelec_true_costheta_nuDir, selelec_true_cosT_nuDir);
	
  output().FillVar(NuParentPDGRaw, vtx->NuParentPDG);
  output().FillVar(TargetPDG, vtx->TargetPDG);
  output().FillVectorVarFromArray(NuParentDecPoint, vtx->NuParentDecPoint, 4);

  // Truth variables to match the truth tree for xsTool/consistency
  // True electron true variables
  output().FillVar(truelepton_pdg,    vtx->LeptonPDG);
  output().FillVar(truelepton_mom,    vtx->LeptonMom);
  output().FillVar(truelepton_det,    anaUtils::GetDetector(vtx->Position));
  Float_t trueelec_true_cosT_z = anaUtils::ArrayToTLorentzVector(vtx->LeptonDir).CosTheta();
  output().FillVar(truelepton_costheta_z, trueelec_true_cosT_z);

  Float_t trueelec_true_cosT_offaxis = (Float_t)cos(  anaUtils::ArrayToTVector3(vtx->LeptonDir).Angle(_nd280_pos)  ) ;
  output().FillVar(truelepton_costheta_offaxis, trueelec_true_cosT_offaxis);

  Float_t trueelec_true_cosT_nuDir = (Float_t)cos(  anaUtils::ArrayToTVector3(vtx->LeptonDir).Angle(anaUtils::ArrayToTVector3(vtx->NuDir) ) );
  output().FillVar(truelepton_costheta_nuDir, trueelec_true_cosT_nuDir); 

  Float_t Erec = anaUtils::ComputeRecNuEnergyCCQE(vtx->LeptonMom, units::mass_electron, trueelec_true_cosT_nuDir);
  output().FillVar(truelepton_nuErecQE, Erec);

  output().FillVar(Q2_true, anaUtils::ComputeQ2(vtx->LeptonMom, units::mass_electron, trueelec_true_cosT_nuDir, vtx->NuEnergy));

  output().FillVectorVarFromArray(truelepton_dir, vtx->LeptonDir,3);
  output().FillVectorVarFromArray(truelepton_pos, vtx->Position, 4);

  // True particles exiting nucleus
  output().FillVar(NTruePions,      nue_categs::NpiC(*vtx));
  output().FillVar(NTruePi0,        nue_categs::Npi0(*vtx));
  output().FillVar(NTrueKaonRhoEta, nue_categs::NKaonRhoEta(*vtx));

  // Signal/non-signal vertex
  bool isAntiNu = false;
  if(vtx->LeptonPDG < 0) isAntiNu = true;

  SubDetId::SubDetEnum fgdID;
  if (_whichFGD == 1) fgdID = SubDetId::kFGD1;
  else if (_whichFGD == 2) fgdID = SubDetId::kFGD2;
  else fgdID = SubDetId::kFGD;

  output().FillVar(nue_signal,      nue_categs::IsNuESignal(*vtx, fgdID, isAntiNu));
  output().FillVar(nue_ccqe,        nue_categs::IsNuECCQE(*vtx, fgdID, isAntiNu));
  output().FillVar(nue_ccnonqe,     nue_categs::IsNuECCnonQE(*vtx, fgdID, isAntiNu));
  output().FillVar(nue_cc0pi,       nue_categs::IsNuECC0pi(*vtx, fgdID, isAntiNu));
  output().FillVar(nue_ccother,     nue_categs::IsNuECCother(*vtx, fgdID, isAntiNu));

  // Find Brem photons
  for(int i = 0; i < vtx->nTrueParticles; i++){
    AnaTrueParticle* ttruth = static_cast<AnaTrueParticle*> (vtx->TrueParticles[i]);
    if(ttruth == truth) continue;
    if(ttruth->PDG != 22) continue;
    if(anaUtils::GetDetector(ttruth->Position) > 1 || anaUtils::GetDetector(ttruth->Position) < 0) continue; // Only count Brems in FGDs
    if(ttruth->ParentID == truth->ID){
      output().FillVectorVar(selelec_true_brem_mom,      ttruth->Momentum);
      output().FillVectorVar(selelec_true_brem_detector, anaUtils::GetDetector(ttruth->Position));

      output().IncrementCounterForVar(selelec_true_brem_detector);
    }
  }

  if(_savegammainitiator){
    const AnaTrueVertex *vtx1 = static_cast<const AnaTrueVertex*>(vtx);
    AnaTrueParticleB* Init=NULL;
    
    if(vtx1)
      Init = GoBackToPrimaryParticle(box().MainTrack->GetTrueParticle(), vtx1);
    
    if(Init){
      output().FillVar(InitiatorPDG,       (Int_t)  Init->PDG);
      output().FillVar(InitiatorParentPDG, (Int_t)  Init->ParentPDG);
      output().FillVar(InitiatorMom,       (Float_t)Init->Momentum);
      output().FillVectorVarFromArray(InitiatorDir, Init->Direction, 3);
    }
  }

  // Get (grad)parent momentum
  AnaTrueParticle* parent = static_cast<AnaTrueParticle*> ( anaUtils::GetTrueParticleByID(static_cast<const AnaEventB&> (GetEvent()), truth->ParentID) );
  if(!parent) return;
  
  output().FillVar(selelec_parent_true_mom,      parent->Momentum);
  output().FillVar(selelec_parent_pdg,           parent->PDG);
  output().FillVar(selelec_parent_id,            parent->ID);

  AnaTrueParticle* gparent = static_cast<AnaTrueParticle*> ( anaUtils::GetTrueParticleByID(static_cast<const AnaEventB&> (GetEvent()), parent->ParentID) );
  if(gparent){
    output().FillVar(selelec_gparent_true_mom,     gparent->Momentum);
    output().FillVar(selelec_gparent_pdg,          gparent->PDG);
    output().FillVar(selelec_gparent_id,           gparent->ID);
  }
  
  // Find pair track for the gamma background
  for(int i = 0; i < vtx->nTrueParticles; i++){
    AnaTrueParticle* ttruth = static_cast<AnaTrueParticle*> (vtx->TrueParticles[i]);
    if(ttruth == truth) continue;
    if(ttruth->ParentID == parent->ID){
      output().FillVar(TruePairTrack_true_mom,      ttruth->Momentum);
      output().FillVar(TruePairTrack_pdg,           ttruth->PDG);
      break;
    }
  }
  
}


//********************************************************************
void nueCCAnalysis::FillTrackVars() {
//********************************************************************

  AnaTrack* track = static_cast<AnaTrack*> (box().MainTrack);
  if (!track) return;

  Float_t selelec_cosT_z  = track->DirectionStart[2];
  output().FillVar(selelec_costheta_z, selelec_cosT_z);

  // ND280 position to get off-axis angle
  Float_t _nd280_pos[3] = {-3.221999, -8.14599, 280.10};

  Float_t selelec_cosT_offaxis = (Float_t)cos(anaUtils::ArrayToTVector3(track->DirectionStart).Angle(_nd280_pos));
  output().FillVar(selelec_costheta_offaxis, selelec_cosT_offaxis);
    
  Float_t cosT = selelec_cosT_z;	
  Float_t phia  = anaUtils::ArrayToTLorentzVector(track->DirectionStart).Phi();
  //Float_t invpt = anaUtils::ComputeInversePT(*box().MainTrack);
  Float_t pt = (track->Momentum)*sqrt((1-cosT*cosT));
  
  output().FillVar(selelec_phi, phia);
  output().FillVar(selelec_pt, pt);
  output().FillVar(selelec_charge, track->Charge);
  output().FillVectorVarFromArray(selelec_startpos, track->PositionStart, 4);
  output().FillVectorVarFromArray(selelec_startdir, track->DirectionStart, 4);
  
  output().FillVar(selelec_momErr, track->MomentumError);
  output().FillVar(selelec_momEleFit, track->MomentumEle);
  output().FillVar(selelec_momMuonFit, track->MomentumMuon); 

  output().FillVar(selelec_FGD, anaUtils::GetDetector(track->PositionStart));

  AnaTPCParticleB* backTpc = 
      static_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInClosestTpc(*track, track->PositionEnd, true));

  if(backTpc){
    output().FillVar(selelec_ElemomTPCBack, backTpc->MomentumEnd);
    output().FillVectorVarFromArray(selelec_tpcBackPos, backTpc->PositionEnd, 4);
    output().FillVectorVarFromArray(selelec_tpcBackDir, track->DirectionEnd, 3);
  }

  AnaParticleB* TpcSegment = anaUtils::GetSegmentWithMostNodesInClosestTpc(*track);
  if(TpcSegment)
    output().FillVar(selelec_tpcNNodes, TpcSegment->NNodes);

  FillFirstTPCVars();
  FillSecondTPCVars();
  FillECalVars();
  FillFGDVars();
}

//********************************************************************
void nueCCAnalysis::FillFirstTPCVars() {
//********************************************************************

  if(!_addmoretoyvariables){
    AnaTrackB* track = box().MainTrack;
    if (!track) return;
    
    AnaTPCParticleB* longCloseTPC = static_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInClosestTpc(*track));
    if(!longCloseTPC ) return;
    
    output().FillVar(selelec_tpcdedx,  longCloseTPC->dEdxMeas);
    output().FillVar(selelec_pullmuon, (longCloseTPC->dEdxMeas-longCloseTPC->dEdxexpMuon)/longCloseTPC->dEdxSigmaMuon);
    output().FillVar(selelec_pullelec, (longCloseTPC->dEdxMeas-longCloseTPC->dEdxexpEle)/longCloseTPC->dEdxSigmaEle);
    output().FillVar(selelec_pullpion, (longCloseTPC->dEdxMeas-longCloseTPC->dEdxexpPion)/longCloseTPC->dEdxSigmaPion);
    output().FillVar(selelec_pullprot, (longCloseTPC->dEdxMeas-longCloseTPC->dEdxexpProton)/longCloseTPC->dEdxSigmaProton);
  }
}

//********************************************************************
void nueCCAnalysis::FillSecondTPCVars() {
//********************************************************************

  if(!_addmoretoyvariables){
    AnaTrackB* track = box().MainTrack;
    if (!track) return;
    
    if(anaUtils::InDetVolume(SubDetId::kFGD1, track->PositionStart)){
      AnaTPCParticleB* longestTPC3 = static_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInDet(*track,SubDetId::kTPC3));
      
      if(!longestTPC3) return;
      // Pass track quality cut
      if(!cutUtils::TPCTrackQualityCut(*longestTPC3)) return;
      
      output().FillVar(selelec_pullmuon2nd,   (longestTPC3->dEdxMeas-longestTPC3->dEdxexpMuon)/longestTPC3->dEdxSigmaMuon);
      output().FillVar(selelec_pullelec2nd,   (longestTPC3->dEdxMeas-longestTPC3->dEdxexpEle)/longestTPC3->dEdxSigmaEle);
    }
  }
}
//********************************************************************
void nueCCAnalysis::FillECalVars() {
//********************************************************************
  
  AnaTrackB* track = box().MainTrack;
  if (!track) return;

  AnaECALParticleB* ecaltracks[NMAXPARTICLES];
  Int_t nECAL = anaUtils::GetTrackerDsEcals(box().MainTrack, ecaltracks);
  
  if(nECAL >=1 && nueCCUtils::UseEcal(track) ){

    AnaECALParticle* EcalSegment = static_cast<AnaECALParticle*> (ecaltracks[nECAL - 1]);
    //AnaECALParticleB* EcalSegmentB = ecaltracks[nECAL - 1];
    if(EcalSegment && EcalSegment->EMEnergy > 0){
      
      SubDetId::SubDetEnum ECALdet = SubDetId::GetSubdetectorEnum(EcalSegment->Detector);
      Int_t detector = ECALdet;
      
      AnaTPCParticleB* backTPC = 
	static_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInClosestTpc(*track, track->PositionEnd, true));
      if(backTPC)
	output().FillVar(selelec_ElemomECalEntrance, backTPC->Momentum);
      
      if(!_addmoretoyvariables)
	output().FillVar(selelec_ecalemene, EcalSegment->EMEnergy);
      
      output().FillVar(selelec_ecalmipem, EcalSegment->PIDMipEm);
      output().FillVar(selelec_ecalmippion, EcalSegment->PIDMipPion); 	   
      output().FillVar(selelec_ecalemhip, EcalSegment->PIDEmHip);
      output().FillVectorVarFromArray(selelec_ecalshowerpos, EcalSegment->ShowerPosition, 3 ); 
      output().FillVar(selelec_ecallength,EcalSegment->Length);
      output().FillVar(selelec_ecalnhits, EcalSegment->NHits);
      output().FillVar(selelec_ecalMostUpStreamLayerHit, EcalSegment->MostUpStreamLayerHit);
      output().FillVar(selelec_ecalcontained, nueCCUtils::IsEcalContained(EcalSegment));
      output().FillVar(selelec_ecaldetector, detector);    
    }
  }

  output().FillVar(selelec_nECALs, nECAL);
} 

//********************************************************************
void nueCCAnalysis::FillTPCVetoVars() {
//********************************************************************
  
  if(!box().MainTrack) return;

  output().FillVar(TPCVetoNNearTracks, nuebox().TPCVetoNearTracks);
  output().FillVar(TPCVetoNFarTracks, nuebox().TPCVetoFarTracks);
  output().FillVar(TPCVetoNNearP0DTracks, nuebox().TPCVetoP0DNearTracks);

  //
  AnaTrackB* SecFGCTPCTrack = nuebox().SecondMostEnergeticFGDTPCTrack;
  Float_t DZ = -999.0;
  if(SecFGCTPCTrack){
    DZ = box().MainTrack->PositionStart[2]-SecFGCTPCTrack->PositionStart[2];
  }

  output().FillVar(SecMomFGDDeltaZ, DZ);
  
  //
  Int_t fgdoofv = -1;
  if(nuebox().OOFVtrack)
    fgdoofv = 1;

  output().FillVar(FGDOOFVActivity,    fgdoofv);
  output().FillVar(NFGDOOFVTracksReqTPC, nuebox().NOOFVTracks);
  //

  AnaTrackB* VetoTrack = nuebox().VetoTrack;
  if(!VetoTrack) return;

  Float_t hmnZstart = box().MainTrack->PositionStart[2];
  Float_t vetoZ     = VetoTrack->Original->PositionStart[2];
  Float_t deltaZ    = vetoZ - hmnZstart;

  Float_t mom 	    = VetoTrack->Momentum;
  Int_t charge	    = VetoTrack->Charge;

  output().FillVar(TPCVetoDeltaz, deltaZ);
  output().FillVar(TPCVetoMomentum, mom);
  output().FillVar(TPCVetoCharge, charge);
}

//********************************************************************
void nueCCAnalysis::FillPairFinderVars() {
//********************************************************************

  AnaTrackB* track     = nuebox().MainTrack;
  if (!track ) return;

  // pair track
  AnaTrackB* pairtrack = nuebox().PairTrack;
  if (!pairtrack ) return;
  // selected track truth
  AnaTrueParticle* sel_truth = static_cast<AnaTrueParticle*> (track->TrueObject);
  // pair track truth
  AnaTrueParticle* pair_truth = static_cast<AnaTrueParticle*> (pairtrack->TrueObject);
  
  // Fill the output tree
  output().FillVar(PairTrack_mom, pairtrack->Momentum);
  output().FillVar(PairTrack_costheta, pairtrack->DirectionStart[2]);
  output().FillVar(PairTrack_charge, pairtrack->Charge);
  
  Float_t mass = 0.511;
  TLorentzVector secondary_PE = nueCCUtils::GetMomLorentzVector(*pairtrack, mass);
  TLorentzVector primary_PE   = nueCCUtils::GetMomLorentzVector(*track, mass);
  Float_t minv = 2. * mass * mass + 2. * (secondary_PE.Dot(primary_PE));

  if(minv > 0)
    minv = sqrt(minv);

  output().FillVar(PairTrack_InvMass, minv);

  // recon pair track dir
  output().FillVectorVarFromArray(PairTrack_startdir, pairtrack->DirectionStart, 4);
  output().FillVectorVarFromArray(PairTrack_startpos, pairtrack->PositionStart, 4);

  // Distance of the pair track to main track
  TVector3 pairposition  = anaUtils::ArrayToTVector3(pairtrack->PositionStart);
  TVector3 trackposition = anaUtils::ArrayToTVector3(track->PositionStart);
  Float_t tempdist = (trackposition-pairposition).Mag();

  output().FillVar(PairTrack_DistToMainTrack, tempdist);

  AnaTPCParticleB* longCloseTPC = static_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInClosestTpc(*pairtrack));

  if(longCloseTPC){
    output().FillVar(PairTrack_tpcNNodes, longCloseTPC->NNodes);
    output().FillVar(PairTrack_pullmuon, (longCloseTPC->dEdxMeas-longCloseTPC->dEdxexpMuon)/longCloseTPC->dEdxSigmaMuon);
    output().FillVar(PairTrack_pullelec, (longCloseTPC->dEdxMeas-longCloseTPC->dEdxexpEle)/longCloseTPC->dEdxSigmaEle);
    output().FillVar(PairTrack_pullpion, (longCloseTPC->dEdxMeas-longCloseTPC->dEdxexpPion)/longCloseTPC->dEdxSigmaPion);
    output().FillVar(PairTrack_pullprot, (longCloseTPC->dEdxMeas-longCloseTPC->dEdxexpProton)/longCloseTPC->dEdxSigmaProton);  
  }

  // Direction of the gamma
  Float_t gamma_dir[3];
  for(int i = 0; i < 3; i++){
    gamma_dir[i] = track->DirectionStart[i] + pairtrack->DirectionStart[i];
  }
  output().FillVectorVarFromArray(PairTrack_gamma_dir, gamma_dir, 3);

  if(pair_truth){
    output().FillVectorVarFromArray(PairTrack_true_startdir, pair_truth->Direction, 4);
    output().FillVectorVarFromArray(PairTrack_true_startpos, pair_truth->Position, 4);
    output().FillVar(PairTrack_pdg, pair_truth->PDG);
    if(sel_truth){
      for(int i = 0; i < 3; i++){
        gamma_dir[i] = sel_truth->Direction[i] + pair_truth->Direction[i];
      }
      output().FillVectorVarFromArray(PairTrack_gamma_true_dir, gamma_dir, 3);
    }
  }

}

//********************************************************************
void nueCCAnalysis::FillP0DVetoVars() {
//********************************************************************
  
  output().FillVar(NP0DVetoP0DTracks,     nuebox().P0DVetoP0DTracks);
  output().FillVar(NP0DVetoFGD1Tracks,    nuebox().P0DVetoFGD1Tracks);
  output().FillVar(NP0DVetoP0DEcalTracks, nuebox().P0DVetoP0DEcalTracks);

  // Temporary
  output().FillVar(FGD2ShowerNFGD1TPC2Tracks,    nuebox().FGD2ShowerNFGD1TPC2Tracks);
  output().FillVar(FGD2ShowerNFGD2TPC3Tracks,    nuebox().FGD2ShowerNFGD2TPC3Tracks);

  AnaTrackB* track = nuebox().MainTrack;
  if(track)
    output().FillVar(selelec_inp0d, anaUtils::TrackUsesDet(*track, SubDetId::kP0D));
}

//********************************************************************
void nueCCAnalysis::FillECalVetoVars() {
//********************************************************************

  AnaTrackB* track = nuebox().MainTrack;
  AnaTrackB* ecal = nuebox().ECalVetoTrack;
  
  if (!track || !ecal) return;

  Float_t start_z = track->PositionStart[2];
  Float_t ecal_z = 0;
  AnaECALParticleB* ecaltrack = anaUtils::GetMostUpstreamECalSegment(ecal);
  
  if (ecaltrack){
    ecal_z = ecaltrack->PositionStart[2];
    if (ecaltrack->PositionEnd[2] < ecaltrack->PositionStart[2]) 
      ecal_z = ecaltrack->PositionEnd[2];

    Float_t* ecal_pos = ecaltrack->PositionStart;
    if (ecaltrack->PositionEnd[2] < ecaltrack->PositionStart[2])
      ecal_pos = ecaltrack->PositionEnd;
    output().FillVectorVarFromArray(ECALPosECALSegment, ecal_pos, 3);

  }

  output().FillVar(ECALMinZ,  ecal_z - start_z); 

  
  Float_t ecaldist = (anaUtils::ArrayToTVector3(ecal->PositionStart) - anaUtils::ArrayToTVector3(track->PositionStart)).Mag();
  output().FillVar(TPCECALdist, ecaldist);
  
  output().FillVectorVarFromArray(ECALPosGlobalTrack, ecal->PositionStart, 3);
  
  if(ecal->GetTrueParticle() && ecal->GetTrueParticle()->TrueVertex)
    output().FillVectorVarFromArray(ECALTrueVertexPos, ecal->GetTrueParticle()->TrueVertex->Position, 3);

  // Look for ecal neutral clusters
  AnaTrackB* ecalB = nuebox().ECalNCVetoTrack;
  if(ecalB){
    AnaECALParticleB* ecaltrackB = nueCCUtils::GetMostUpstreamECalNCSegment(ecalB);
    ecal_z = 0;
    if (ecaltrackB){
      ecal_z = ecaltrackB->PositionStart[2];
      if (ecaltrackB->PositionEnd[2] < ecaltrackB->PositionStart[2]) 
	ecal_z = ecaltrackB->PositionEnd[2];
      
      output().FillVar(ECALNCMinZ,  ecal_z - start_z);
    }
  }

}

//********************************************************************
void nueCCAnalysis::FillFGDVars() {
//********************************************************************

  AnaTrackB* HMTrack = box().MainTrack;
  if (!HMTrack) return;

  for (Int_t subdet = 0; subdet<2; subdet++) {
    if (!SubDetId::GetDetectorUsed(HMTrack->Detector, static_cast<SubDetId::SubDetEnum >(subdet))) continue;
    AnaFGDParticle* FGDSegment = static_cast<AnaFGDParticle*>(anaUtils::GetSegmentInDet(*HMTrack,static_cast<SubDetId::SubDetEnum >(subdet)));
    if (!FGDSegment) continue;

    output().FillVectorVar(selelec_fgd_det,         subdet);
    output().FillVectorVar(selelec_fgd_x,           FGDSegment->X);
    output().FillVectorVar(selelec_fgd_E,           FGDSegment->E);
    output().FillVectorVar(selelec_fgd_V11,         FGDSegment->Vertex1x1);
    output().FillVectorVar(selelec_fgd_V33,         FGDSegment->Vertex3x3);
    output().FillVectorVar(selelec_fgd_V55,         FGDSegment->Vertex5x5);
    output().FillVectorVar(selelec_fgd_V77,         FGDSegment->Vertex7x7);
    output().FillVectorVar(selelec_fgd_VLayer,      FGDSegment->VertexLayer);
    output().FillVectorVar(selelec_fgd_pullmu,      FGDSegment->Pullmu);
    output().FillVectorVar(selelec_fgd_pullpi,      FGDSegment->Pullpi);
    output().FillVectorVar(selelec_fgd_pullno,      FGDSegment->Pullno);
    output().FillVectorVar(selelec_fgd_containment, FGDSegment->Containment);
    if(!_addmoretoyvariables)
      output().FillVectorVar(selelec_fgd_pullprot,    FGDSegment->Pullp);
    
    output().IncrementCounterForVar(selelec_fgd_det);
  }
}

//********************************************************************
void nueCCAnalysis::FillEcalNeutralClusterVars() {
//********************************************************************

  if(!box().MainTrack) return;

  // Default values for the brem search
  Float_t _thetacut     = 15.0; // degrees
  Float_t _phicut       = 15.0; // degrees
  Int_t _firstlayercut  = 7;
  Float_t _ecalfrag     = 0.15;
  Float_t _ecalfragdist = 400.0; 

  AnaTrackB* ecaltracks[NMAXPARTICLES];
  Int_t ntracks = anaUtils::GetAllTracksUsingECAL(static_cast<const AnaEventB&> (GetEvent()), ecaltracks);
  output().FillVar(NECalTracks, ntracks);

  // Get all the ecal neutral clusters
  AnaTrackB* dsecaltracks[NMAXPARTICLES];
  AnaTrackB* brecaltracks[NMAXPARTICLES];
  Int_t nDSEcal  = anaUtils::GetAllTracksUsingDet(static_cast<const AnaEventB&> (GetEvent()), SubDetId::kDSECAL, dsecaltracks);
  Int_t nBREcal  = anaUtils::GetAllTracksUsingDet(static_cast<const AnaEventB&> (GetEvent()), SubDetId::kTECAL,  brecaltracks);

  Int_t nDSEcalNC = 0, nBREcalNC = 0, nBremEcalNC = 0, nFragmentEcalNC = 0;
  Float_t mostenenc = 0.0, allenenc = 0.0, neutclustenergy = 0.0, fragmentdist = 0.0;
  Float_t mintheta = 10000.0, minphi = 10000.0;
  // Angular information about the HM track
  Float_t track_theta = anaUtils::ArrayToTLorentzVector(box().MainTrack->DirectionStart).Theta()*180/3.14159;
  Float_t track_phi   = anaUtils::ArrayToTLorentzVector(box().MainTrack->DirectionStart).Phi()*180/3.14159;
  // Ecal of the HM track
  AnaECALParticleB* HMecaltrack[NMAXPARTICLES];
  Int_t nECAL = anaUtils::GetTrackerDsEcals(box().MainTrack, HMecaltrack);
  Int_t ecaldetector = 0;
  TVector3 showerposition;

  Float_t emenergy = 0.0;
  if(nECAL > 0){
    AnaECALParticle* EcalSegment = static_cast<AnaECALParticle*> (HMecaltrack[nECAL - 1]);
    if(EcalSegment){
      ecaldetector   = SubDetId::GetSubdetectorEnum(EcalSegment->Detector);
      showerposition = anaUtils::ArrayToTVector3(EcalSegment->ShowerPosition);
      emenergy       = EcalSegment->EMEnergy;
    }
  }

  for (Int_t i=0; i<nDSEcal; i++) {
    if(SubDetId::GetDetectorUsed(dsecaltracks[i]->Detector, SubDetId::kTPC) || SubDetId::GetDetectorUsed(dsecaltracks[i]->Detector, SubDetId::kFGD) || SubDetId::GetDetectorUsed(dsecaltracks[i]->Detector, SubDetId::kP0D) || SubDetId::GetDetectorUsed(dsecaltracks[i]->Detector, SubDetId::kPECAL) ){continue;}
    nDSEcalNC++;
    Int_t nseg = dsecaltracks[i]->nECALSegments;
    for(Int_t j=0; j<nseg; j++){
      AnaECALParticle* ecaltrack = static_cast<AnaECALParticle*> (dsecaltracks[i]->ECALSegments[j]);
      //AnaECALParticleB* ecaltrack = dsecaltracks[i]->ECALSegments[j];

      // Do not consider Ecal clusters with unphysical values
      if(ecaltrack->EMEnergy <= 0.0) continue;

      // Brem tag
      Float_t theta  = anaUtils::ArrayToTVector3(ecaltrack->DirectionStart).Theta()*180/3.14159;
      Float_t phia   = anaUtils::ArrayToTVector3(ecaltrack->DirectionStart).Phi()*180/3.14159;
      
      if(fabs(theta-track_theta) < mintheta)
        mintheta = fabs(theta-track_theta);
      if(fabs(phia-track_phi) < minphi)
        minphi = fabs(phia-track_phi);
      
      if(fabs(theta-track_theta) < _thetacut && fabs(phia-track_phi) < _phicut && ecaltrack->MostUpStreamLayerHit < _firstlayercut){
        nBremEcalNC++;
        neutclustenergy += ecaltrack->EMEnergy;
      }
      else{
        if(emenergy > 0 && ecaltrack->EMEnergy/emenergy < _ecalfrag && ecaldetector == SubDetId::GetSubdetectorEnum(ecaltrack->Detector)){
 	  TVector3 fragmentposition = anaUtils::ArrayToTVector3(ecaltrack->ShowerPosition);
	  Float_t tempdist = (fragmentposition-showerposition).Mag();
	  if(tempdist > fragmentdist)
	    fragmentdist = tempdist;
	  // count only if it near the main shower
	  if(tempdist < _ecalfragdist)
	    nFragmentEcalNC++;
	}
      }

      allenenc += ecaltrack->EMEnergy;
      if(ecaltrack->EMEnergy > mostenenc)
        mostenenc = ecaltrack->EMEnergy;
    }
  }

  for (Int_t i=0; i<nBREcal; i++) {
    if(SubDetId::GetDetectorUsed(brecaltracks[i]->Detector, SubDetId::kTPC) || SubDetId::GetDetectorUsed(brecaltracks[i]->Detector, SubDetId::kFGD) || SubDetId::GetDetectorUsed(brecaltracks[i]->Detector, SubDetId::kP0D) || SubDetId::GetDetectorUsed(brecaltracks[i]->Detector, SubDetId::kPECAL) ){continue;}
    Int_t nseg = brecaltracks[i]->nECALSegments;
    nBREcalNC++;
    for(Int_t j=0; j<nseg; j++){
      AnaECALParticle* ecaltrack = static_cast<AnaECALParticle*> (brecaltracks[i]->ECALSegments[j]);
      //AnaECALParticleB* ecaltrack = brecaltracks[i]->ECALSegments[j];

      // Do not consider Ecal clusters with unphysical values
      if(ecaltrack->EMEnergy <= 0.0) continue;
      
      // Brem tag
      Float_t theta  = anaUtils::ArrayToTVector3(ecaltrack->DirectionStart).Theta()*180/3.14159;
      Float_t phia   = anaUtils::ArrayToTVector3(ecaltrack->DirectionStart).Phi()*180/3.14159;
      
      if(fabs(theta-track_theta) < mintheta)
        mintheta = fabs(theta-track_theta);
      if(fabs(phia-track_phi) < minphi)
        minphi = fabs(phia-track_phi);
      
      if(fabs(theta-track_theta) < _thetacut && fabs(phia-track_phi) < _phicut && ecaltrack->MostUpStreamLayerHit < _firstlayercut){
        nBremEcalNC++;
        neutclustenergy += ecaltrack->EMEnergy;
      }
      else{
        if(emenergy > 0 && ecaltrack->EMEnergy/emenergy < _ecalfrag && ecaldetector == SubDetId::GetSubdetectorEnum(ecaltrack->Detector)){
  	  TVector3 fragmentposition = anaUtils::ArrayToTVector3(ecaltrack->ShowerPosition);
	  Float_t tempdist = (fragmentposition-showerposition).Mag();
	  if(tempdist > fragmentdist)
	    fragmentdist = tempdist;
	  // count only if it near the main shower
	  if(tempdist < _ecalfragdist)
	    nFragmentEcalNC++;
	}
      }
      
      allenenc += ecaltrack->EMEnergy;
      if(ecaltrack->EMEnergy > mostenenc)
        mostenenc = ecaltrack->EMEnergy;
    }
  }

  output().FillVar(NDsEcalNeutralClusters,  nDSEcalNC);
  output().FillVar(NBrEcalNeutralClusters,  nBREcalNC);
  output().FillVar(MostEnergeticEcalNeutralCluster, mostenenc);
  output().FillVar(EcalNeutralClusterEnergySum, allenenc);
  output().FillVar(BremNeutralClusters,  nBremEcalNC);
  output().FillVar(BremNeutralClustersEnergy,  neutclustenergy);
  output().FillVar(BremNeutralClustersMinTheta,  mintheta);
  output().FillVar(BremNeutralClustersMinPhi,  minphi);
  output().FillVar(FragmentsNeutralClusters,  nFragmentEcalNC);
  output().FillVar(FragmentsDistanceToShower,  fragmentdist);
  
  // Find other ecal tracks that EMEnergy > p of the HMT
  Int_t nfound = 0;
  for(Int_t i=0; i<ntracks; i++) {
    AnaTrackB* track = ecaltracks[i];
    if(track){
      if(track == box().MainTrack){continue;}
      if(track->nTPCSegments == 0 && track->nFGDSegments == 0){continue;}
      for(Int_t j=0; j<track->nECALSegments; j++) {
        AnaECALParticleB* EcalSegment = track->ECALSegments[j];
        if(EcalSegment->EMEnergy > box().MainTrack->Momentum)
          nfound++;
      }
    }
  }
  
  output().FillVar(EcalClusterMostEnergeticThanHMT, nfound);

}

//********************************************************************
void nueCCAnalysis::FillAllTracksVars() {
//********************************************************************

  AnaTrackB* track = box().MainTrack;
  if(!track) return;

  // Get all the FGD-iso tracks
  AnaTrackB* fgdonlytracks[NMAXPARTICLES];
  Int_t nFGDOnly = anaUtils::GetAllTracksUsingOnlyDet(static_cast<const AnaEventB&> (GetEvent()), SubDetId::kFGD, fgdonlytracks);
  for(Int_t i=0; i<nFGDOnly; i++){
    Int_t subdet = -1;
    if (SubDetId::GetDetectorUsed(fgdonlytracks[i]->Detector, SubDetId::kFGD1))
      subdet = 0;
    else if(SubDetId::GetDetectorUsed(fgdonlytracks[i]->Detector, SubDetId::kFGD2))
      subdet = 1;

    AnaFGDParticle* FGDSegment = static_cast<AnaFGDParticle*>(anaUtils::GetSegmentInDet(*fgdonlytracks[i],static_cast<SubDetId::SubDetEnum >(subdet)));
    if (!FGDSegment) continue;
    
    output().FillVectorVar(fgdiso_det,         subdet);
    output().FillVectorVar(fgdiso_x,           FGDSegment->X);
    output().FillVectorVar(fgdiso_E,           FGDSegment->E);
    output().FillVectorVar(fgdiso_pullmu,      FGDSegment->Pullmu);
    output().FillVectorVar(fgdiso_pullpi,      FGDSegment->Pullpi);
    output().FillVectorVar(fgdiso_pullprot,    FGDSegment->Pullp);
    output().FillVectorVar(fgdiso_containment, FGDSegment->Containment);
    output().FillMatrixVarFromArray(fgdiso_position, static_cast<AnaTrack*> (fgdonlytracks[i])->PositionStart, 4);
    Float_t cosT  = anaUtils::ArrayToTLorentzVector(static_cast<AnaTrack*> (fgdonlytracks[i])->DirectionStart).CosTheta();
    output().FillVectorVar(fgdiso_costheta, cosT);
    if(static_cast<AnaTrueParticle*> (fgdonlytracks[i]->TrueObject))
       output().FillVectorVar(fgdiso_pdg, fgdonlytracks[i]->GetTrueParticle()->PDG);

    output().IncrementCounterForVar(fgdiso_det);
  
  }

  // Get all the FGD1-TPC tracks
  AnaTrackB* fgdtpctracks[NMAXPARTICLES];
  Int_t nFGDs = anaUtils::GetAllTracksUsingDet(static_cast<const AnaEventB&> (GetEvent()), SubDetId::kFGD, fgdtpctracks);
  Int_t nFGD1TPC1 = 0, nFGD1TPC2 = 0, nFGD1Ecal = 0;
  Int_t nFGD2TPC2 = 0, nFGD2TPC3 = 0, nFGD2Ecal = 0;
  Float_t xymaxdist = -10.0, xinit = -999.0, yinit = -999.0;
  for(Int_t i=0; i<nFGDs; i++) {
    if(fgdtpctracks[i] == track){continue;}

    if(SubDetId::GetDetectorUsed(fgdtpctracks[i]->Detector, SubDetId::kFGD1)){ // FGD1
      if ( !SubDetId::GetDetectorUsed(fgdtpctracks[i]->Detector, SubDetId::kTPC2) && SubDetId::GetDetectorUsed(fgdtpctracks[i]->Detector, SubDetId::kTPC1))
        nFGD1TPC1++;

      if ( SubDetId::GetDetectorUsed(fgdtpctracks[i]->Detector, SubDetId::kTPC2) && !SubDetId::GetDetectorUsed(fgdtpctracks[i]->Detector, SubDetId::kTPC1))
        nFGD1TPC2++;
      
      if ( SubDetId::GetDetectorUsed(fgdtpctracks[i]->Detector, SubDetId::kECAL) && !SubDetId::GetDetectorUsed(fgdtpctracks[i]->Detector, SubDetId::kTPC))
        nFGD1Ecal++;
    }
    else if(SubDetId::GetDetectorUsed(fgdtpctracks[i]->Detector, SubDetId::kFGD2)){ // FGD2
      if ( SubDetId::GetDetectorUsed(fgdtpctracks[i]->Detector, SubDetId::kTPC3) && !SubDetId::GetDetectorUsed(fgdtpctracks[i]->Detector, SubDetId::kTPC2) && !SubDetId::GetDetectorUsed(fgdtpctracks[i]->Detector, SubDetId::kFGD1)){
        nFGD2TPC3++;
	if(xinit == -999.0 && yinit == -999.0){
	  xinit = fgdtpctracks[i]->PositionStart[0];
	  yinit = fgdtpctracks[i]->PositionStart[1];
	}
	else{
	  Float_t temp = sqrt( (fgdtpctracks[i]->PositionStart[0] - xinit)*(fgdtpctracks[i]->PositionStart[0] - xinit) + (fgdtpctracks[i]->PositionStart[1] - yinit)*(fgdtpctracks[i]->PositionStart[1] - yinit));
	  if(temp > xymaxdist)
	    xymaxdist = temp;
	}
      }

      if ( !SubDetId::GetDetectorUsed(fgdtpctracks[i]->Detector, SubDetId::kTPC3) && SubDetId::GetDetectorUsed(fgdtpctracks[i]->Detector, SubDetId::kTPC2) && !SubDetId::GetDetectorUsed(fgdtpctracks[i]->Detector, SubDetId::kFGD1))
        nFGD2TPC2++;
      
      if ( SubDetId::GetDetectorUsed(fgdtpctracks[i]->Detector, SubDetId::kECAL) && !SubDetId::GetDetectorUsed(fgdtpctracks[i]->Detector, SubDetId::kTPC))
        nFGD2Ecal++;
    }
  }

  output().FillVar(NFGD1TracksReqTPC1, nFGD1TPC1);
  output().FillVar(NFGD1TracksReqTPC2, nFGD1TPC2);
  output().FillVar(NFGD1TracksReqEcal, nFGD1Ecal);
  output().FillVar(NFGD2TracksReqTPC2, nFGD2TPC2);
  output().FillVar(NFGD2TracksReqTPC3, nFGD2TPC3);
  output().FillVar(NFGD2TracksReqEcal, nFGD2Ecal);
  output().FillVar(FGD2XYMaxTrackDist, xymaxdist);

  // Get all the TPC-Ecal tracks and save all the secondary FGD-TPC tracks
  AnaTrackB* tpctracks[NMAXPARTICLES];
  Int_t ntpctracks = anaUtils::GetAllTracksUsingDet(static_cast<const AnaEventB&> (GetEvent()), SubDetId::kTPC, tpctracks);
  Int_t nTPCECAL = 0;
  for (Int_t i=0; i<ntpctracks; i++) {
    if(tpctracks[i] == track) {continue;}
    // First find the TPC-Ecal tracks
    if ( SubDetId::GetDetectorUsed(tpctracks[i]->Detector, SubDetId::kECAL) && !SubDetId::GetDetectorUsed(tpctracks[i]->Detector, SubDetId::kFGD))
      nTPCECAL++;

    // Only take track if it's in the FGD
    if(!SubDetId::GetDetectorUsed(tpctracks[i]->Detector, SubDetId::kFGD)) {continue;}
    
    // Check in which TPC the track goes
    Int_t tpcdet = -1;
    
    Float_t cosT = anaUtils::ArrayToTLorentzVector(tpctracks[i]->DirectionStart).CosTheta();
    
    if(SubDetId::GetDetectorUsed(tpctracks[i]->Detector, SubDetId::kFGD1) && SubDetId::GetDetectorUsed(tpctracks[i]->Detector, SubDetId::kFGD2) && cosT < 0)
      tpcdet = 1;
    else if(SubDetId::GetDetectorUsed(tpctracks[i]->Detector, SubDetId::kFGD1))
      tpcdet = 0;
    else if(SubDetId::GetDetectorUsed(tpctracks[i]->Detector, SubDetId::kFGD2))
      tpcdet = 1;
    
    AnaTPCParticleB* longCloseTPC = static_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInClosestTpc(*tpctracks[i]));
    if(!longCloseTPC) {continue;}
    
    output().FillVectorVar(FGDTPCTrack_det,      tpcdet );
    output().FillVectorVar(FGDTPCTrack_pullmuon, (longCloseTPC->dEdxMeas-longCloseTPC->dEdxexpMuon)/longCloseTPC->dEdxSigmaMuon);
    output().FillVectorVar(FGDTPCTrack_pullelec, (longCloseTPC->dEdxMeas-longCloseTPC->dEdxexpEle)/longCloseTPC->dEdxSigmaEle);
    output().FillVectorVar(FGDTPCTrack_pullpion, (longCloseTPC->dEdxMeas-longCloseTPC->dEdxexpPion)/longCloseTPC->dEdxSigmaPion);
    output().FillVectorVar(FGDTPCTrack_pullprot, (longCloseTPC->dEdxMeas-longCloseTPC->dEdxexpProton)/longCloseTPC->dEdxSigmaProton);                 
    output().FillVectorVar(FGDTPCTrack_tpcNNodes,longCloseTPC->NNodes);
    
    output().FillVectorVar(FGDTPCTrack_mom, tpctracks[i]->Momentum);
    
    output().FillVectorVar(FGDTPCTrack_costheta, cosT);
    output().FillVectorVar(FGDTPCTrack_charge, tpctracks[i]->Charge);
    output().FillMatrixVarFromArray(FGDTPCTrack_startpos, tpctracks[i]->PositionStart, 4);
    output().FillMatrixVarFromArray(FGDTPCTrack_startdir, tpctracks[i]->DirectionStart, 3);
    
    AnaTrueParticle* truth = static_cast<AnaTrueParticle*> (tpctracks[i]->TrueObject);
    if(truth)
      output().FillVectorVar(FGDTPCTrack_pdg, truth->PDG);
    
    // Check if the track goes in the Ecal
    AnaECALParticleB* ecaltracks[NMAXPARTICLES];
    Int_t nECAL = anaUtils::GetTrackerDsEcals(tpctracks[i], ecaltracks);
    
    if(nECAL >=1 ){
      AnaECALParticle* EcalSegment = static_cast<AnaECALParticle*> (ecaltracks[nECAL - 1]);
      //AnaECALParticleB* EcalSegmentB = ecaltracks[nECAL - 1];
      SubDetId::SubDetEnum ECALdet = SubDetId::GetSubdetectorEnum(EcalSegment->Detector);

      AnaTPCParticleB* backTPC = 
      static_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInClosestTpc(*tpctracks[i], tpctracks[i]->PositionEnd, true));
      if(backTPC)
	output().FillVectorVar(FGDTPCTrack_MomECalEntrance, backTPC->MomentumEnd);
      
      output().FillVectorVar(FGDTPCTrack_EMEnergy, EcalSegment->EMEnergy);
      output().FillVectorVar(FGDTPCTrack_MipEm, EcalSegment->PIDMipEm);
      output().FillVectorVar(FGDTPCTrack_MipPion, EcalSegment->PIDMipPion); 	   
      output().FillVectorVar(FGDTPCTrack_EmHip, EcalSegment->PIDEmHip);
      output().FillMatrixVarFromArray(FGDTPCTrack_ShowerPosition, EcalSegment->ShowerPosition, 3 );
      output().FillVectorVar(FGDTPCTrack_ECALdet, ECALdet);
      output().FillVectorVar(FGDTPCTrack_NHits, EcalSegment->NHits);
      output().FillVectorVar(FGDTPCTrack_MostUpStreamLayerHit, EcalSegment->MostUpStreamLayerHit);
      output().FillVectorVar(FGDTPCTrack_IsEcalContained, nueCCUtils::IsEcalContained(EcalSegment));
    }
      
    output().IncrementCounterForVar(FGDTPCTrack_det);
  }

  output().FillVar(NTPCTracksReqEcal, nTPCECAL);
  
  // -------------------------------------------------------
  SubDetId::SubDetEnum det = SubDetId::kInvalid;
  if(anaUtils::InDetVolume(SubDetId::kFGD1, track->PositionStart)) det = SubDetId::kFGD1;
  else if(anaUtils::InDetVolume(SubDetId::kFGD2, track->PositionStart)) det = SubDetId::kFGD2;

  AnaTrackB* fgdtracks[NMAXPARTICLES]; 
  Int_t nFGD = anaUtils::GetAllTracksUsingDet(static_cast<const AnaEventB&> (GetEvent()), det, fgdtracks);

  Int_t nFGDTPC = 0;
  Float_t min_reqtpc = 1e6, max_reqtpc = -1e6;
  Float_t min = 1e6, max = -1e6;

  for (Int_t i=0; i<nFGD; i++) {
    if(fgdtracks[i] == track) {continue;}
    
    Float_t diff = (anaUtils::ArrayToTVector3(track->PositionStart) - anaUtils::ArrayToTVector3(fgdtracks[i]->PositionStart)).Mag();
    min = std::min(min, diff);
    max = std::max(max, diff);
    
    if(SubDetId::GetDetectorUsed(fgdtracks[i]->Detector, SubDetId::kTPC)){
      nFGDTPC++;
      min_reqtpc = std::min(min_reqtpc, diff);
      max_reqtpc = std::max(max_reqtpc, diff);
    }
  }

  output().FillVar(NFGDTracksReqTPC, nFGDTPC);
  output().FillVar(FGDDistanceReqTpc, min_reqtpc);
  output().FillVar(FGDDistanceNoReqTpc, min);
  output().FillVar(FGDMaxDistanceReqTpc, max_reqtpc);
  output().FillVar(FGDMaxDistanceNoReqTpc, max);
}

//********************************************************************
void nueCCAnalysis::FillToFVars() {
//********************************************************************
  
  AnaTrackB* track     = box().MainTrack;
  if (!track ) return;

  if(anaUtils::InDetVolume(SubDetId::kFGD1, track->PositionStart)){
    if( track->ToF.Flag_P0D_FGD1 )
      output().FillVar(selelec_ToF_P0D_FGD1, track->ToF.P0D_FGD1);
    else if(track->ToF.Flag_ECal_FGD1)
      output().FillVar(selelec_ToF_ECal_FGD1, track->ToF.ECal_FGD1);
    else if( track->ToF.Flag_FGD1_FGD2 )
      output().FillVar(selelec_ToF_FGD1_FGD2, track->ToF.FGD1_FGD2);
  }
  else{
    if(track->ToF.Flag_ECal_FGD2)
      output().FillVar(selelec_ToF_ECal_FGD2, track->ToF.ECal_FGD2);
    else if( track->ToF.Flag_FGD1_FGD2 )
      output().FillVar(selelec_ToF_FGD1_FGD2, track->ToF.FGD1_FGD2);
  }

#if VERSION_HAS_TIME_FITS
  // Check for DsEcal
  const AnaTrack* atrack = static_cast<AnaTrack*> (box().MainTrack);
  if (!atrack ) return;
  bool fgd1 = false, fgd2 = false, ecal = false;
  Float_t tfgd1 = 0, tfgd2 = 0, tdsecal = 0;
  std::vector<AnaTimeNode*> timenodes = atrack->TimeNodes;
  for (unsigned int i=0;i<timenodes.size();i++){
    AnaTimeNode* node = timenodes.at(i);
    float t = (node->TimeStart + node->TimeEnd)/2.0;

	
    if(node->Detector == 4){
      if(!fgd1) tfgd1 = t;
      fgd1 = true;
    }
    else if(node->Detector == 5){
      if(!fgd2) tfgd2 = t;
      fgd2 = true;
    }
    else if(node->Detector == 7){
      if(!ecal) tdsecal = t;
      ecal = true;
    }
  }

  if(fgd1 && ecal)
    output().FillVar(selelec_ToF_DsECal_FGD1, tdsecal-tfgd1);
  else if(fgd2 && ecal)
    output().FillVar(selelec_ToF_DsECal_FGD2, tdsecal-tfgd2);
#endif

}

//********************************************************************
void nueCCAnalysis::FillToyVarsInMicroTrees(bool addBase){
//********************************************************************
  
  if(addBase) baseTrackerAnalysis::FillToyVarsInMicroTreesBase(addBase);

  if(box().MainTrack){
    output().FillToyVar(selelec_mom,   box().MainTrack->Momentum);

    // Costheta
    // vertex defined as start of electron track, neutrino direction calculated from this
    TVector3 nuDirVec = anaUtils::GetNuDirRec(box().MainTrack->PositionStart);
    TVector3 elDirVec = anaUtils::ArrayToTVector3(box().MainTrack->DirectionStart);
    // The cosine of the angle between the electron and the neutrino
    double costheta_el_nu = nuDirVec.Dot(elDirVec);
    output().FillToyVar(selelec_costheta, (Float_t)costheta_el_nu);

    // Erec
    Float_t Erec = anaUtils::ComputeRecNuEnergyCCQE(box().MainTrack->Momentum, units::mass_electron, costheta_el_nu);
    output().FillToyVar(selelec_ENuRec, Erec);
    Float_t Q2 = anaUtils::ComputeQ2(box().MainTrack->Momentum, units::mass_electron, costheta_el_nu, Erec);
    output().FillToyVar(selelec_Q2Rec, Q2);

    if(_addmoretoyvariables){
      // First TPC
      AnaTPCParticleB* longCloseTPC = static_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInClosestTpc(*box().MainTrack));
      if(longCloseTPC){
	output().FillToyVar(selelec_tpcdedx,  longCloseTPC->dEdxMeas);
	output().FillToyVar(selelec_pullmuon, (longCloseTPC->dEdxMeas-longCloseTPC->dEdxexpMuon)/longCloseTPC->dEdxSigmaMuon);
	output().FillToyVar(selelec_pullelec, (longCloseTPC->dEdxMeas-longCloseTPC->dEdxexpEle)/longCloseTPC->dEdxSigmaEle);
	output().FillToyVar(selelec_pullpion, (longCloseTPC->dEdxMeas-longCloseTPC->dEdxexpPion)/longCloseTPC->dEdxSigmaPion);
	output().FillToyVar(selelec_pullprot, (longCloseTPC->dEdxMeas-longCloseTPC->dEdxexpProton)/longCloseTPC->dEdxSigmaProton);
      }

      // Second TPC
      if(anaUtils::InDetVolume(SubDetId::kFGD1, box().MainTrack->PositionStart)){
	AnaTPCParticleB* longestTPC3 = static_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInDet(*box().MainTrack,SubDetId::kTPC3));
      
	if(longestTPC3 && cutUtils::TPCTrackQualityCut(*longestTPC3)){
	  output().FillToyVar(selelec_pullmuon2nd,   (longestTPC3->dEdxMeas-longestTPC3->dEdxexpMuon)/longestTPC3->dEdxSigmaMuon);
	  output().FillToyVar(selelec_pullelec2nd,   (longestTPC3->dEdxMeas-longestTPC3->dEdxexpEle)/longestTPC3->dEdxSigmaEle);
	}
      }

      // Ecal
      AnaECALParticleB* ecaltracks[NMAXPARTICLES];
      Int_t nECAL = anaUtils::GetTrackerDsEcals(box().MainTrack, ecaltracks);
      
      if(nECAL >=1 && nueCCUtils::UseEcal(box().MainTrack) ){
	
	AnaECALParticle* EcalSegment = static_cast<AnaECALParticle*> (ecaltracks[nECAL - 1]);
	if(EcalSegment)
	  output().FillToyVar(selelec_ecalemene, EcalSegment->EMEnergy);
      }

      // Info in all FGDs
      for (Int_t subdet = 0; subdet<2; subdet++) {
	if (!SubDetId::GetDetectorUsed(box().MainTrack->Detector, static_cast<SubDetId::SubDetEnum >(subdet))) continue;
	AnaFGDParticle* FGDSegment = static_cast<AnaFGDParticle*>(anaUtils::GetSegmentInDet(*(box().MainTrack), static_cast<SubDetId::SubDetEnum >(subdet)));
	if (!FGDSegment) continue;
	output().FillToyVectorVar(selelec_fgd_pullprot, FGDSegment->Pullp, subdet);
      }
    }

    // if there is a true vertex associated
    if(box().MainTrack->TrueObject) {
      AnaTrueVertex *vtx = static_cast<AnaTrueVertex*>(box().MainTrack->GetTrueParticle()->TrueVertex);
      if (vtx) {
        // Fill the mass component related to the mass weight
        anaUtils::massComponentEnum massComponent = anaUtils::GetMassComponent(GetEvent().GetIsMC(), vtx->Position);
        output().FillToyVar(truevtx_mass_component, massComponent);
        
        bool AntiNu = false;
        // Fill whether it is signal or bkg
        if (nue_categs::IsNuESignal(*vtx, SubDetId::kFGD1, AntiNu))
          output().FillToyVar(true_signal, 1);
        else if (nue_categs::IsNuESignal(*vtx, SubDetId::kFGD2, AntiNu))
          output().FillToyVar(true_signal, 2);
        else
          output().FillToyVar(true_signal, 0);
	
      }  
    }

  } // end if MainTrack

}

//********************************************************************
bool nueCCAnalysis::CheckFillTruthTree(const AnaTrueVertex& vtx){
//********************************************************************

  SubDetId::SubDetEnum fgdID;
  if (_whichFGD == 1) fgdID = SubDetId::kFGD1;
  else if (_whichFGD == 2) fgdID = SubDetId::kFGD2;
  else fgdID = SubDetId::kFGD;
  
  bool AntiNu = false;
  return (nue_categs::IsNuESignal(vtx, fgdID, AntiNu));
}

//********************************************************************
void nueCCAnalysis::FillTruthTree(const AnaTrueVertex& vtx){
//********************************************************************

  bool AntiNu = false;
  return FillTruthTreeBase(vtx, AntiNu);
}

//********************************************************************
void nueCCAnalysis::FillTruthTreeBase(const AnaTrueVertex& vtx, bool IsAntinu){
//********************************************************************

  // Fill the common variables
  //baseTrackerAnalysis::FillTruthTreeBase(vtx);
  baseTrackerAnalysis::FillTruthTreeBase(vtx, SubDetId::kFGD1, IsAntinu);
  
  // Electron true variables
  output().FillVectorVarFromArray(truelepton_pos, vtx.Position, 4);
  output().FillVar(truelepton_det, anaUtils::GetDetector(vtx.Position));
    
  Float_t trueelec_true_cosT_z = anaUtils::ArrayToTLorentzVector(vtx.LeptonDir).CosTheta();
  output().FillVar(truelepton_costheta_z, trueelec_true_cosT_z);

  // ND280 position to get off-axis angle
  Float_t _nd280_pos[3] = {-3.221999, -8.14599, 280.10};

  Float_t trueelec_true_cosT_offaxis = (Float_t)cos(  anaUtils::ArrayToTVector3(vtx.LeptonDir).Angle(_nd280_pos)  ) ;
  output().FillVar(truelepton_costheta_offaxis, trueelec_true_cosT_offaxis);
  
  Float_t trueelec_true_cosT_nuDir = (Float_t)cos(  anaUtils::ArrayToTVector3(vtx.LeptonDir).Angle(anaUtils::ArrayToTVector3(vtx.NuDir) ) );
  output().FillVar(truelepton_costheta_nuDir, trueelec_true_cosT_nuDir);

  output().FillVar(Q2_true, anaUtils::ComputeQ2(vtx.LeptonMom, units::mass_electron, trueelec_true_cosT_nuDir, vtx.NuEnergy));

  // Fill whether it is signal or bkg
  if (nue_categs::IsNuESignal(vtx, SubDetId::kFGD1, IsAntinu))
    output().FillVar(true_signal, 1);
  else if (nue_categs::IsNuESignal(vtx, SubDetId::kFGD2, IsAntinu))
    output().FillVar(true_signal, 2);
  else
    output().FillVar(true_signal, 0);

  // Fill the mass component related to the mass weight
  anaUtils::massComponentEnum massComponent = anaUtils::GetMassComponent(GetSpill().GetIsMC(), vtx.Position);
  output().FillVar(truevtx_mass_component, massComponent);

  SubDetId::SubDetEnum fgdID;
  if (_whichFGD == 1) fgdID = SubDetId::kFGD1;
  else if (_whichFGD == 2) fgdID = SubDetId::kFGD2;
  else fgdID = SubDetId::kFGD;

  output().FillVar(nue_signal,  nue_categs::IsNuESignal(vtx, fgdID, IsAntinu));
  output().FillVar(nue_ccqe,    nue_categs::IsNuECCQE(vtx, fgdID, IsAntinu));
  output().FillVar(nue_ccnonqe, nue_categs::IsNuECCnonQE(vtx, fgdID, IsAntinu));
  output().FillVar(nue_cc0pi,   nue_categs::IsNuECC0pi(vtx, fgdID, IsAntinu));
  output().FillVar(nue_ccother, nue_categs::IsNuECCother(vtx, fgdID, IsAntinu));

  // Fill the extra categories
  anaUtils::FillCategories(&vtx, "fgd2", SubDetId::kFGD2, IsAntinu, GetSpill().GetIsSandMC());
  //nue_categs::FillCategories(&GetEvent(), box().MainTrack, "", SubDetId::kFGD1, IsAntinu);
  //nue_categs::FillCategories(&GetEvent(), box().MainTrack, "fgd2", SubDetId::kFGD2, IsAntinu);

  if(_savegammainitiator){
    const AnaTrueVertex *vtx1 = static_cast<const AnaTrueVertex*>(&vtx);
    AnaTrueParticleB* Init=NULL;
    AnaTrueParticleB* Main=NULL;
    
    double mom = 0;
    for(int i = 0; i < vtx.nTrueParticles; i++){
      AnaTrueParticleB* truth = vtx.TrueParticles[i];
      if(abs(truth->PDG) == 11 && truth->Momentum > mom && truth->ParentPDG == 22){
	Main = truth;
      }
    }

    if(vtx1)
      Init = GoBackToPrimaryParticle(Main, vtx1);

    if(Init){
      output().FillVar(InitiatorPDG,       (Int_t)  Init->PDG);
      output().FillVar(InitiatorParentPDG, (Int_t)  Init->ParentPDG);
      output().FillVar(InitiatorMom,       (Float_t)Init->Momentum);
      output().FillVectorVarFromArray(InitiatorDir, Init->Direction, 3);
    }
  }

}

//********************************************************************
void nueCCAnalysis::FillCategories(){
//********************************************************************

  bool IsAntinu = false;

  anaUtils::FillCategories(&GetEvent(), static_cast<AnaTrack*>(box().MainTrack), "", SubDetId::kFGD1);
  anaUtils::FillCategories(&GetEvent(), static_cast<AnaTrack*>(box().MainTrack), "fgd2", SubDetId::kFGD2);

  nue_categs::FillCategories(&GetEvent(), box().MainTrack, "", SubDetId::kFGD1, IsAntinu);
  nue_categs::FillCategories(&GetEvent(), box().MainTrack, "fgd2", SubDetId::kFGD2, IsAntinu);
}

//********************************************************************
void nueCCAnalysis::FillConfigTree(){
//********************************************************************


  // Add and fill number of nucleons in each of the targets
  AddVarD(output(), nNucleonsFGD1,      "number of targets in FGD1");  
  AddVarD(output(), nNucleonsFGD2scint, "number of targets in FGD2 scintillator");  
  AddVarD(output(), nNucleonsFGD2water, "number of targets in FGD2 water");  

  output().FillVar(nNucleonsFGD1,      anaUtils::GetNTargets(anaUtils::kFGD1));  
  output().FillVar(nNucleonsFGD2scint, anaUtils::GetNTargets(anaUtils::kFGD2xymodules));  
  output().FillVar(nNucleonsFGD2water, anaUtils::GetNTargets(anaUtils::kFGD2watermodules));
  
}




//*********************************************************************
AnaTrueParticleB* nueCCAnalysis::GoBackToPrimaryParticle(const AnaTrueParticleB* Gamma, const AnaTrueVertex *vtx){
//*********************************************************************
  
  if(!Gamma)
    return NULL;

  if(!vtx)
    vtx = static_cast<const AnaTrueVertex*>(Gamma->TrueVertex);
  
  if(!vtx)
    return NULL;
 
  std::vector<AnaTrueParticleB*> EMParticle;
  std::vector<AnaTrueParticleB*> Pi0;
  std::vector<AnaTrueParticleB*> PiPM;
  
  EMParticle.clear();
  Pi0.clear();
  PiPM.clear();

  EMParticle.reserve(200);
  Pi0.reserve(200);
  PiPM.reserve(200);

  int imax = vtx->nTrueParticles;
  for(int i = 0; i < imax; i++){

    AnaTrueParticleB* trueTrack = vtx->TrueParticles[i];

    if(!trueTrack) continue;

    if(trueTrack->PDG  == 111) Pi0.push_back(trueTrack);
    if(abs(trueTrack->PDG) == 211) PiPM.push_back(trueTrack);

    if(trueTrack->PDG == 22 || abs(trueTrack->PDG) == 11){ // Store all the gammas and electron positron
      EMParticle.push_back(trueTrack);
  
    }
  }
  
  int gammaID = -99;
  int ParentID  = Gamma->ParentID;
  int ParentPDG = Gamma->ParentPDG;
  
  int itry = 0;

  while(abs(ParentPDG) == 11 || ParentPDG == 22){ // go back to the gamma from the pi0
    itry++; // for infinite loop
    int i = 0;
    while(i < vtx->nTrueParticles){
      AnaTrueParticleB* trueTrack = vtx->TrueParticles[i];
      //std::cout << "trueTrack[" << i << "]" << std::endl;
      //std::cout << "trueTrack->ID        " << trueTrack->ID        << std::endl;
      //std::cout << "trueTrack->ParentID  " << trueTrack->ParentID  << std::endl;
      //std::cout << "trueTrack->ParentPDG " << trueTrack->ParentPDG << std::endl;
      
      if(ParentID == trueTrack->ID){
	gammaID   = trueTrack->ID;	 
	ParentID  = trueTrack->ParentID; 
	ParentPDG = trueTrack->ParentPDG;
	//std::cout << "gammaID   " << gammaID   << std::endl;
	//std::cout << "ParentID  " << ParentID  << std::endl;
	//std::cout << "ParentPDG " << ParentPDG << std::endl;
	break;
      }
      i++;
    }
    
    if(gammaID == -99)
      itry = 1500;
    if(itry > 1000)
	break;
      }
    
  for(int i = 0; i < vtx->nTrueParticles; i++){
      AnaTrueParticleB* trueTrack = vtx->TrueParticles[i];

      if(!trueTrack) continue;
    if(trueTrack->ID == ParentID) return trueTrack;
  }
    
  EMParticle.clear();
  Pi0.clear();
  PiPM.clear();
  return NULL;
}
