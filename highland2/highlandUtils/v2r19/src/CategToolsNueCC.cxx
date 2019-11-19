#include "CategToolsNueCC.hxx"

#include "ND280AnalysisUtils.hxx"

void nue_categs::AddCategories(const std::string& prefix, bool AntiNu) {
 
   // nueBar selection
  if(AntiNu){

    ///  nufamilynue  ///  GetNuFamilyNuE  ///
    const int NNUFAMILYNUE = 7;
    std::string reac_nufamnue[NNUFAMILYNUE] = {"#bar{#nu}_{e} CCQE", "#bar{#nu}_{e} CCnonQE", "#nu_{#mu}/#bar{#nu}_{#mu} e", "#nu_{#mu}/#bar{#nu}_{#mu} #mu", "#nu_{#mu}/#bar{#nu}_{#mu} Other", "OOFV", "#nu_{#mu}/#bar{#nu}_{#mu} Proton"};
    int reac_nufamnuecode[NNUFAMILYNUE] = {5, 6, 2, 1, 3, 0, 8};
    int reac_nufamnuecol[NNUFAMILYNUE]  = {1, 4, 2, 3, 6, 5, 48};
    anaUtils::_categ->AddCategory(prefix + "nufamilynue", NNUFAMILYNUE, reac_nufamnue, reac_nufamnuecode, reac_nufamnuecol);
 
    ///  nuesimple  ///  GetNuESimpleCat  ///
    const int NNUE1 = 7;
    std::string reac_nue1[NNUE1] = {"#bar{#nu}_{e} CC0pi", "#bar{#nu}_{e} CCOther", "OOFGD #gamma background", "inFGD #gamma background", "#mu background", "Other background", "Proton background"};
    int reac_nue1code[NNUE1] = {1, 2, 6, 3, 4, 5, 8};
    int reac_nue1col[NNUE1]  = {6, 7, 4, 5, 3, 1, 48};
    //int reac_nue1col[NNUE1]  = {kRed, kBlue, kYellow, kGreen, kBlack, kCyan, 48, COLNOTRUTH};
    anaUtils::_categ->AddCategory(prefix + "nuesimple", NNUE1, reac_nue1, reac_nue1code, reac_nue1col);
    
    ///  nuesimpleparent  ///  GetNuESimpleParent  ///
    const int NNUE2 = 7;
    std::string reac_nue2[NNUE2] = {"#bar{#nu}_{e} from #mu", "#bar{#nu]_{e} from K", "#bar{#nu}_{e} from other", "#gamma background", "#mu background", "Other background", "Proton background"};
    int reac_nue2code[NNUE2] = {1, 2, 0,          3, 4, 5, 8};
    int reac_nue2col[NNUE2] =  {2, 4, kMagenta+3, 5, 3, 1, 48};
    anaUtils::_categ->AddCategory(prefix + "nuesimpleparent", NNUE2, reac_nue2, reac_nue2code, reac_nue2col);
    
    ///  nuesimplereac  ///  GetNuESimpleReac  ///
    const int NNUE3 = 6;
    std::string reac_nue3[NNUE3] = {"#bar{#nu}_{e} CCQE", "#bar{#nu}_{e} CCnonQE", "#gamma background", "#mu background", "Other background", "Proton background"};
    int reac_nue3code[NNUE3] = {1, 2, 3, 4, 5, 8};
    int reac_nue3col[NNUE3] =  {6, 7, 5, 3, 1, 48};
    anaUtils::_categ->AddCategory(prefix + "nuesimplereac", NNUE3, reac_nue3, reac_nue3code, reac_nue3col);
    
    ///  nuesimpletopo  ///  GetNuESimpleTopology  ///
    const int NNUE4 = 6;
    std::string reac_nue4[NNUE4] = {"#bar{#nu}_{e} CC 0#pi", "#bar{#nu}_{e} CC other", "#gamma background", "#mu background", "Other background", "Proton background"};
    int reac_nue4code[NNUE4] = {1, 2, 3, 4, 5, 8};
    int reac_nue4col[NNUE4] =  {6, 7, 5, 3, 1, 48};
    anaUtils::_categ->AddCategory(prefix + "nuesimpletopo", NNUE4, reac_nue4, reac_nue4code, reac_nue4col);

    ///  nuereaction  ///  GetNuEReaction  ///
    const int NNUE7=13;
    std::string reac_nue7[NNUE7] = {"#bar{#nu}_{e} CCQE", "#bar{#nu}_{e} 2p2h", "#bar{#nu}_{e} RES","#bar{#nu}_{e} DIS","#bar{#nu}_{e} COH","NC", "#bar{#nu}_{#mu}", "#nu_{#mu}","#nu_{e}", "#nu e Elastic" , "InFGD OOFV", "OOFGD", "Other"};
    int reac_nue7code[NNUE7]     = { 0                  , CAT2P2H             , 1                  , 2                 , 3                 , 4  , 5                , 6          , 8       , 10              , 11          , 12     , CATOTHER};
    int reac_nue7col[NNUE7]      = { 2                  , COL2P2H             , 3                  , 4                 , 7                 , 6  , 38               , 8          , 1       , 5               , 96          , 48     , COLOTHER};
    anaUtils::_categ->AddCategory(prefix + "nuereaction",       NNUE7, reac_nue7, reac_nue7code, reac_nue7col);
  
    ///  nuereactionnofv  ///  GetNuEReactionNoFgdFV  ///
    const int NNUE5=11;
    std::string reac_nue5[NNUE5] = {"#bar{#nu}_{e} CCQE", "#bar{#nu}_{e} 2p2h", "#bar{#nu]_{e} RES","#bar{#nu}_{e} DIS","#bar{#nu}_{e} COH","NC", "#bar{#nu}_{#mu}", "#nu_{#mu}", "#nu_{e}", "#nu e Elastic" , "Other"};
    int reac_nue5code[NNUE5]     = { 0                  , CAT2P2H             , 1                  , 2                 , 3                 , 4  , 5                , 6          , 8        , 10              , CATOTHER};
    int reac_nue5col[NNUE5]      = { 2                  , COL2P2H             , 3                  , 4                 , 7                 , 6  , 38               , 8          , 1        , 5               , COLOTHER};
    anaUtils::_categ->AddCategory(prefix + "nuereactionnofv",       NNUE5, reac_nue5, reac_nue5code, reac_nue5col);
  }
  else{ // nue Selection

    ///  nufamilynue  ///  GetNuFamilyNuE  ///
    const int NNUFAMILYNUE = 6;
    std::string reac_nufamnue[NNUFAMILYNUE] = {"#nu_{e} CCQE", "#nu_{e} CCnonQE", "#nu_{#mu} e", "#nu_{#mu} #mu", "#nu_{#mu} other", "OOFV"};
    int reac_nufamnuecode[NNUFAMILYNUE] = {5, 6, 2, 1, 3, 0};
    int reac_nufamnuecol[NNUFAMILYNUE]  = {1, 4, 2, 3, 6, 5};
    anaUtils::_categ->AddCategory(prefix + "nufamilynue", NNUFAMILYNUE, reac_nufamnue, reac_nufamnuecode, reac_nufamnuecol);

    ///  nuesimple  ///  GetNuESimpleCat  ///
    const int NNUE1 = 6;
    std::string reac_nue1[NNUE1] = {"#nu_{e} CC0pi", "#nu_{e} CCOther", "OOFGD #gamma background", "inFGD #gamma background", "#mu background", "Other background"};
    int reac_nue1code[NNUE1] = {1, 2, 6, 3, 4, 5};
    int reac_nue1col[NNUE1]  = {6, 7, 4, 5, 3, 1};
    //int reac_nue1col[NNUE1]  = {kRed, kBlue, kYellow, kGreen, kBlack, kCyan, COLNOTRUTH};
    anaUtils::_categ->AddCategory(prefix + "nuesimple", NNUE1, reac_nue1, reac_nue1code, reac_nue1col);
     
    ///  nuesimpleparent  ///  GetNuESimpleParent  ///
    const int NNUE2 = 6;
    std::string reac_nue2[NNUE2] = {"#nu_{e} from #mu", "#nu_{e} from K", "#nu_{e} from other", "#gamma background", "#mu background", "Other background"};
    int reac_nue2code[NNUE2] = {1, 2, 0,          3, 4, 5};
    int reac_nue2col[NNUE2] =  {2, 4, kMagenta+3, 5, 3, 1};
    anaUtils::_categ->AddCategory(prefix + "nuesimpleparent", NNUE2, reac_nue2, reac_nue2code, reac_nue2col);
    
    ///  nuesimplereac  ///  GetNuESimpleReac  ///
    const int NNUE3 = 5;
    std::string reac_nue3[NNUE3] = {"#nu_{e} CCQE", "#nu_{e} CCnonQE", "#gamma background", "#mu background", "Other background"};
    int reac_nue3code[NNUE3] = {1, 2, 3, 4, 5};
    int reac_nue3col[NNUE3] =  {6, 7, 5, 3, 1};
    anaUtils::_categ->AddCategory(prefix + "nuesimplereac", NNUE3, reac_nue3, reac_nue3code, reac_nue3col);
    
    ///  nuesimpletopo  ///  GetNuESimpleTopology  ///
    const int NNUE4 = 5;
    std::string reac_nue4[NNUE4] = {"#nu_{e} CC 0#pi", "#nu_{e} CC other", "#gamma background", "#mu background", "Other background"};
    int reac_nue4code[NNUE4] = {1, 2, 3, 4, 5};
    int reac_nue4col[NNUE4] =  {6, 7, 5, 3, 1};
    anaUtils::_categ->AddCategory(prefix + "nuesimpletopo", NNUE4, reac_nue4, reac_nue4code, reac_nue4col);

    ///  nuereaction  ///  GetNuEReaction  ///
    const int NNUE7=13;
    std::string reac_nue7[NNUE7] = {"#nu_{e} CCQE", "#nu_{e} 2p2h", "#nu_{e} RES","#nu_{e} DIS","#nu_{e} COH","NC", "#bar{#nu}_{#mu}", "#nu_{#mu}",  "#bar{#nu}_{e}", "#nu e Elastic" , "InFGD OOFV", "OOFGD", "Other"};
    int reac_nue7code[NNUE7]     = { 0            , CAT2P2H       , 1            , 2           , 3           , 4  , 5                , 6          ,  8              , 10              , 11          , 12     , CATOTHER};
    int reac_nue7col[NNUE7]      = { 2            , COL2P2H       , 3            , 4           , 7           , 6  , 38               , 8          ,  1              , 5               , 96          , 48     , COLOTHER};
    anaUtils::_categ->AddCategory(prefix + "nuereaction",       NNUE7, reac_nue7, reac_nue7code, reac_nue7col);
    
    ///  nuereactionnofv  ///  GetNuEReactionNoFgdFV  ///
    const int NNUE5=11;
    std::string reac_nue5[NNUE5] = {"#nu_{e} CCQE", "#nu_{e} 2p2h", "#nu_{e} RES","#nu_{e} DIS","#nu_{e} COH","NC", "anti-#nu_{#mu}", "#nu_{#mu}", "#bar{#nu}_{e}", "#nu e Elastic" , "Other"};
    int reac_nue5code[NNUE5]     = { 0            , CAT2P2H       , 1            , 2           , 3           , 4  , 5               , 6          , 8              , 10              , CATOTHER};
    int reac_nue5col[NNUE5]      = { 2            , COL2P2H       , 3            , 4           , 7           , 6  , 38              , 8          , 1              , 5               , COLOTHER};
    anaUtils::_categ->AddCategory(prefix + "nuereactionnofv",       NNUE5, reac_nue5, reac_nue5code, reac_nue5col);
  }

  ///  frompi0  ///  GetPi0Code  ///
  const int NPI0TYPES=4;
  std::string pi0_types[NPI0TYPES] = {"e^{#pm} from n-induced #pi^{0}", "e^{#pm} from other #pi^{0}", "Other e^{#pm}", "Other"};
  int pi0_codes[NPI0TYPES]         = {1, 2, 3, 4};
  int pi0_colors[NPI0TYPES]        = {1, 2, 3, 4};
  anaUtils::_categ->AddCategory(prefix + "frompi0", NPI0TYPES, pi0_types, pi0_codes, pi0_colors);

  /// gamma sideband
  const int PITOPO=9;
  std::string pitopo_types[PITOPO] = {"NC 1#pi^{0}", "NC 1#pi^{#pm}", "NC Multi #pi",
				      "CC 1#pi^{0}", "CC 1#pi^{#pm}", "CC Multi #pi", "CC 0#pi",
				      "NC single #gamma",
				      "Other"};
  int pitopo_codes[PITOPO]         = {21,            22,              23,
				      11,            12,              13,14,
				      100,
				      -100};
  int pitopo_colors[PITOPO]        = {kMagenta,      kMagenta+1,      kMagenta+2,
				      kCyan+1,       kCyan+2,         kCyan+3,kCyan+4,
				      kYellow,
				      kGray};
  anaUtils::_categ->AddCategory(prefix + "pitopo", PITOPO, pitopo_types, pitopo_codes, pitopo_colors);

  const int NREACTYPESNOFVGAMMA=10;
  std::string reacnofvgamma_types[NREACTYPESNOFVGAMMA] = {"CCQE",     "CC RES 1#pi^{0}", "CC RES other", "CC DIS",   //CC
							              "NC RES 1#pi^{0}", "NC RES other", "NC DIS",   //NC
							  "NC single #gamma",
							  "#nu_{e} CC",
							  "other"};  //Bkg
  int reacnofvgamma_code[NREACTYPESNOFVGAMMA]          = {10,         11,                111,            12,
							              21,                121,            22,
							  100,
							  50,
							  -100};
  int reacnofvgamma_colors[NREACTYPESNOFVGAMMA]        = {kCyan,      kCyan+1,           kCyan+3,        kCyan+2,
						                      kMagenta,          kMagenta+3,     kMagenta+2,
						          kYellow,
							  kGreen,
							  kGray};
  anaUtils::_categ->AddCategory(prefix + "reactnofvgamma", NREACTYPESNOFVGAMMA,reacnofvgamma_types,reacnofvgamma_code,reacnofvgamma_colors);

}

//********************************************************************
void nue_categs::FillCategories(AnaEventB* event, AnaTrackB* track, const std::string& prefix, const SubDetId::SubDetEnum det, bool AntiNu){
//********************************************************************

  // Fill the standard categories using the baseAnalysis version of this function.
  //anaUtils::FillCategories(event, static_cast<AnaTrack*>(track), prefix, subdet);

  anaUtils::_categ->SetCode(prefix + "frompi0",         GetPi0Code(event, track));
  anaUtils::_categ->SetCode(prefix + "nufamilynue",     GetNuFamilyNuE(track, det, AntiNu));
  anaUtils::_categ->SetCode(prefix + "nuesimple",       GetNuESimpleCat(track, det, AntiNu));
  anaUtils::_categ->SetCode(prefix + "nuesimpleparent", GetNuESimpleParent(track, det, AntiNu));
  anaUtils::_categ->SetCode(prefix + "nuesimplereac",   GetNuESimpleReac(track, det, AntiNu));
  anaUtils::_categ->SetCode(prefix + "nuesimpletopo",   GetNuESimpleTopology(track, det, AntiNu));
  anaUtils::_categ->SetCode(prefix + "nuereaction",     GetNuEReaction(track, det, AntiNu));
  anaUtils::_categ->SetCode(prefix + "nuereactionnofv", GetNuEReactionNoFgdFV(track, AntiNu));
  // Gamma specific categories
  anaUtils::_categ->SetCode(prefix + "pitopo",          GetPiTopo(track));
  anaUtils::_categ->SetCode(prefix + "reactnofvgamma",  GetReactionNoFVGamma(track));
}

///  frompi0  ///  GetPi0Code  ///
//********************************************************************
Int_t nue_categs::GetPi0Code(AnaEventB* event, AnaTrackB* track) {
//********************************************************************

  if (!track || !track->GetTrueParticle() || track->GetTrueParticle()->PDG == 0) return CATNOTRUTH;
	
  if (abs(track->GetTrueParticle()->PDG) == 11) {
    if (HasParentOfType(event, track->GetTrueParticle(), 111)) {
      if (HasParentOfType(event, track->GetTrueParticle(), 2112)) {
        return 1;
      } else {
        return 2;
      }
    } else {
      return 3;
    }
  }
  return 4;
}

///  nufamilynue  ///  GetNuFamilyNuE  ///
//********************************************************************
Int_t nue_categs::GetNuFamilyNuE(AnaTrackB* track, const SubDetId::SubDetEnum det, bool AntiNu) {
//********************************************************************

  if (!track || !track->GetTrueParticle() || track->GetTrueParticle()->PDG == 0) return CATNOTRUTH; // no truth

  AnaTrueVertex* vtx = static_cast<AnaTrueVertex*> (track->GetTrueParticle()->TrueVertex);
  if (!vtx) return CATNOTRUTH; // no truth

  //if (!anaUtils::InFiducialVolume(det,vtx->Position)) return 0; // out of FGD

  if (fabs(vtx->NuPDG) == 14) { //numu

    if (fabs(track->GetTrueParticle()->PDG) == 13) return 4; //numu misidentified muon 
    if (AntiNu && (abs(track->GetTrueParticle()->PDG) == 2212)) return 8; // extra category for NuEBar selection
    else if (IsGammaSignal(track,det)) return 3; //numu electron
    else return 5; // other particle
  } 
  else if (fabs(vtx->NuPDG) == 12) { //nue
    if (vtx->ReacCode == 1) return 1; // -> nue from CCQE
    else return 2; //nue from CCnonQE
  }

  return 0;
}

///  nuesimple  ///  GetNuESimpleCat  ///
//********************************************************************
Int_t nue_categs::GetNuESimpleCat(AnaTrackB* track, const SubDetId::SubDetEnum det, bool AntiNu) {
//********************************************************************

  if (!track || !track->GetTrueParticle() || track->GetTrueParticle()->PDG == 0) return CATNOTRUTH; // no truth

  AnaTrueVertex* vtx = static_cast<AnaTrueVertex*> (track->GetTrueParticle()->TrueVertex);
  if (!vtx) return CATNOTRUTH; // no truth

  //if(!anaUtils::InFiducialVolume(det, vtx->Position)) return CATOUTFV;

  if (IsNuESignal(*vtx, det, AntiNu) && abs(track->GetTrueParticle()->PDG) == 11 && track->GetTrueParticle()->ParentPDG != 111 && track->GetTrueParticle()->GParentPDG != 111){

    if (IsNuECC0pi(*vtx, det, AntiNu))
      return 1; // nue CC0pi
    else 
      return 2; // nue CCother
  }

  if (abs(track->GetTrueParticle()->PDG) == 13) return 4; // muon

  if (AntiNu && (abs(track->GetTrueParticle()->PDG) == 2212)) return 8; // extra category for NuEBar selection

  if (IsGammaSignalOOFGD(track, det)) return 6; // gamma OOFGD background

  if (IsGammaSignal(track, det)) return 3; // gamma inFGD background

  return 5; // Other
}

///  nuesimpleparent  ///  GetNuESimpleParent  ///
//********************************************************************
Int_t nue_categs::GetNuESimpleParent(AnaTrackB* track, const SubDetId::SubDetEnum det, bool AntiNu) {
//********************************************************************

  if (!track || !track->GetTrueParticle() || track->GetTrueParticle()->PDG == 0) return CATNOTRUTH; // no truth
 
  AnaTrueVertex* vtx = static_cast<AnaTrueVertex*> (track->GetTrueParticle()->TrueVertex);
  if (!vtx) return CATNOTRUTH; // no truth  

  //if(!anaUtils::InFiducialVolume(det, vtx->Position)) return CATOUTFV;

  if (IsNuESignal(*vtx, det, AntiNu) && abs(track->GetTrueParticle()->PDG) == 11 && track->GetTrueParticle()->ParentPDG != 111 && track->GetTrueParticle()->GParentPDG != 111) {
    // nue from mu    
    if (abs(vtx->NuParentPDG) == 13) return 1;
    // nue from K    
    else if (abs(vtx->NuParentPDG) == 321 || abs(vtx->NuParentPDG) == 130) return 2; 
    // nue other
    else return 0;
  }

  // muon
  if (abs(track->GetTrueParticle()->PDG) == 13) return 4;

  if (AntiNu && (abs(track->GetTrueParticle()->PDG) == 2212)) return 8; // extra category for NuEBar selection

  // gamma background
  if (IsGammaSignal(track, det)) return 3;

  // Other
  return 5;
}

//  nuesimplereac  ///  GetNuESimpleReac  ///
//********************************************************************
Int_t nue_categs::GetNuESimpleReac(AnaTrackB* track, const SubDetId::SubDetEnum det, bool AntiNu){
//********************************************************************

  // No truth
  if (!track || !track->GetTrueParticle() || track->GetTrueParticle()->PDG == 0) return CATNOTRUTH; // no truth
    
  AnaTrueVertex* vtx = static_cast<AnaTrueVertex*> (track->GetTrueParticle()->TrueVertex);
  if (!vtx) return CATNOTRUTH; // no truth

  //if(!anaUtils::InFiducialVolume(det, vtx->Position)) return CATOUTFV;
    
  if (IsNuECCQE(*vtx, det, AntiNu)  && abs(track->GetTrueParticle()->PDG) == 11 && track->GetTrueParticle()->ParentPDG != 111 && track->GetTrueParticle()->GParentPDG != 111) return 1;
  if (IsNuECCnonQE(*vtx, det, AntiNu)  && track->GetTrueParticle()->ParentPDG != 111 && track->GetTrueParticle()->GParentPDG != 111) return 2;
  if (abs(track->GetTrueParticle()->PDG) == 13) return 4; // muon
  if (AntiNu && (abs(track->GetTrueParticle()->PDG) == 2212)) return 8; // extra category for NuEBar selection
  if (IsGammaSignal(track, det)) return 3; // gamma background

  return 5; // Other
}

///  nuesimpletopo  ///  GetNuESimpleTopology  ///
//********************************************************************
Int_t nue_categs::GetNuESimpleTopology(AnaTrackB* track, const SubDetId::SubDetEnum det, bool AntiNu){
//********************************************************************

  if (!track || !track->GetTrueParticle() || track->GetTrueParticle()->PDG == 0) return CATNOTRUTH; // no truth

  AnaTrueVertex* vtx = static_cast<AnaTrueVertex*> (track->GetTrueParticle()->TrueVertex);
  if (!vtx) return CATNOTRUTH; // no truth

  // if(!anaUtils::InFiducialVolume(det, vtx->Position)) return CATOUTFV;

  if (IsNuESignal(*vtx, det, AntiNu) && abs(track->GetTrueParticle()->PDG) == 11 && track->GetTrueParticle()->ParentPDG != 111 && track->GetTrueParticle()->GParentPDG != 111){

    if (IsNuECC0pi(*vtx, det, AntiNu))
      return 1; // nue CC0pi
    else 
      return 2; // nue CCother
  
  }

  if (abs(track->GetTrueParticle()->PDG) == 13) return 4; // muon
  if (AntiNu && (abs(track->GetTrueParticle()->PDG) == 2212)) return 8; // extra category for NuEBar selection
  if (IsGammaSignal(track,det)) return 3; // gamma background
  
  return 5; // Other
}

///  nuereaction  ///  GetNuEReaction  ///
//**************************************************
Int_t nue_categs::GetNuEReaction(AnaTrackB* track, const SubDetId::SubDetEnum det, bool AntiNu) {
//**************************************************

  if (!track || !track->GetTrueParticle() || track->GetTrueParticle()->PDG == 0) return CATNOTRUTH; // no truth

  AnaTrueVertex* vtx = static_cast<AnaTrueVertex*>(track->GetTrueParticle()->TrueVertex);
  if(!vtx) return CATNOTRUTH;

  //if(!anaUtils::InFiducialVolume(det,vtx->Position)) return CATOUTFV; // 7
  if(!anaUtils::InFiducialVolume(det,vtx->Position)){
    if(anaUtils::InDetVolume(det, vtx->Position)) return 11; // OOFV but in FGD
    
    return 12; // OOFGD
  }

  return GetNuEReactionNoFgdFV(track, AntiNu);
}

///  nuereactionnofv  ///  GetNuEReactionNFgdFv  ///
//**************************************************
Int_t nue_categs::GetNuEReactionNoFgdFV(AnaTrackB* track, bool AntiNu) {
//**************************************************

  if (!track || !track->GetTrueParticle() || track->GetTrueParticle()->PDG == 0) return CATNOTRUTH; // no truth: -1

  AnaTrueVertex* vtx = static_cast<AnaTrueVertex*>(track->GetTrueParticle()->TrueVertex);
  if(!vtx) return CATNOTRUTH;

  Int_t reac   = vtx->ReacCode;

  if (abs(reac) == 59) return 10; // nu-e elastic
  if (abs(reac) > 30)  return 4; // nc
  if (vtx->NuPDG ==  14) return 6; // numu
  if (vtx->NuPDG == -14) return 5; // antinumu
  if (AntiNu  && vtx->NuPDG ==  12) return 8; // nue bkg in anti-nue
  if (!AntiNu && vtx->NuPDG == -12) return 8; // anti-nue bkg in nue

  if(abs(vtx->NuPDG) == 12){
    if (abs(reac) == 1) return 0; // CCQEL
    if (abs(reac) == 25) return 0; // CCQEL Charm
    if (abs(reac) == 2 ) return CAT2P2H; // 9
    if (abs(reac) == 70 ) return CAT2P2H;
    if (abs(reac) == 16) return 3; // CC COH
    if (abs(reac) >10 && abs(reac) <14) return 1; // single pi from res
    //if (abs(reac) == 17) return 1; // single gamma from res
    //if (abs(reac) == 22) return 1; // single eta from res
    //if (abs(reac) == 23) return 1; // single K from res
    if (abs(reac) >16 && abs(reac) <30) return 2;
  }

  return CATOTHER; //other
  
}

//********************************************************************
bool nue_categs::IsGammaSignalOOFGD(AnaTrackB* track, const SubDetId::SubDetEnum det){
//********************************************************************

  AnaTrueParticleB* truetrack = track->GetTrueParticle();
  if(!truetrack) return false;

  AnaTrueVertex* vtx = static_cast<AnaTrueVertex*> (track->GetTrueParticle()->TrueVertex);
  if(!vtx) return false;

  if(truetrack->ParentPDG == 22 && abs(truetrack->PDG) == 11 && !(anaUtils::InDetVolume(det, vtx->Position)))
    return true;

  return false;
}

//********************************************************************
bool nue_categs::IsGammaSignal(AnaTrackB* track, const SubDetId::SubDetEnum det) {
//********************************************************************

  AnaTrueParticleB* truetrack = track->GetTrueParticle();
  if(!truetrack) return false;

  //AnaTrueVertex* vtx = static_cast<AnaTrueVertex*> (track->GetTrueParticle()->TrueVertex);
  //if(!vtx) return false;

  if ( truetrack->ParentPDG == 22 && abs(truetrack->PDG) == 11 && anaUtils::InFiducialVolume(det,truetrack->Position) )
    return true;

  return false;
}

//********************************************************************
bool nue_categs::IsNuESignal(const AnaTrueVertexB& vtx, const SubDetId::SubDetEnum det, bool AntiNu) {
//********************************************************************

  const AnaTrueVertex *vtx2 = static_cast<const AnaTrueVertex*> (&vtx);
  if(!vtx2) return 0;
 
  if (AntiNu)
    return (anaUtils::InFiducialVolume(det,vtx2->Position) && vtx2->NuPDG == -12  && vtx2->ReacCode < 0 && vtx2->ReacCode > -31);
  else
    return (anaUtils::InFiducialVolume(det,vtx2->Position) && fabs(vtx2->NuPDG) == 12  && vtx2->ReacCode > 0 && vtx2->ReacCode < 31);
}

//********************************************************************
bool nue_categs::IsNuECCinFGDOOFV(const AnaTrueVertexB& vtx, const SubDetId::SubDetEnum det, bool AntiNu) {
//********************************************************************

  const AnaTrueVertex *vtx2 = static_cast<const AnaTrueVertex*> (&vtx);
  if(!vtx2) return 0;
 
  if (AntiNu)
    return (  (!anaUtils::InFiducialVolume(det,vtx2->Position)) && (anaUtils::InDetVolume(det, vtx2->Position) )   && vtx2->NuPDG == -12  && vtx2->ReacCode < 0 && vtx2->ReacCode > -31);
  else
    return (  (!anaUtils::InFiducialVolume(det,vtx2->Position)) && (anaUtils::InDetVolume(det, vtx2->Position) )  && fabs(vtx2->NuPDG) == 12  && vtx2->ReacCode > 0 && vtx2->ReacCode < 31);
}

//********************************************************************
bool nue_categs::IsNuECCQE(const AnaTrueVertexB& vtx, const SubDetId::SubDetEnum det, bool AntiNu) {
//********************************************************************

  const AnaTrueVertex *vtx2 = static_cast<const AnaTrueVertex*> (&vtx);
  if(!vtx2) return 0;

  if (AntiNu)
    return (IsNuESignal(vtx, det, AntiNu) && vtx2->ReacCode == -1);
  else
    return (IsNuESignal(vtx, det, AntiNu) && vtx2->ReacCode == 1);
}

//********************************************************************
bool nue_categs::IsNuECCnonQE(const AnaTrueVertexB& vtx, const SubDetId::SubDetEnum det, bool AntiNu) {
//********************************************************************

  const AnaTrueVertex *vtx2 = static_cast<const AnaTrueVertex*> (&vtx);
  if(!vtx2) return 0;

  if (AntiNu)
    return (IsNuESignal(vtx, det, AntiNu) && vtx2->ReacCode < -1 && vtx2->ReacCode > -31);
  else
    return (IsNuESignal(vtx, det, AntiNu) && vtx2->ReacCode > 1 && vtx2->ReacCode < 31);
}

//********************************************************************
bool nue_categs::IsNuECC0pi(const AnaTrueVertexB& vtx, const SubDetId::SubDetEnum det, bool AntiNu){
//********************************************************************

  if (IsNuESignal(vtx, det, AntiNu)) {
    const AnaTrueVertex *vtx2 = static_cast<const AnaTrueVertex*> (&vtx);
    if(!vtx2) return 0;
    Int_t Nmeson =    vtx2->NPrimaryParticles[ParticleId::kMesons];
    if(Nmeson > 0) return 0;
    else return 1;
  }
  
  return 0; // NOT nue CC0pi
}

//********************************************************************
bool nue_categs::IsNuECCother(const AnaTrueVertexB& vtx, const SubDetId::SubDetEnum det, bool AntiNu){
//********************************************************************

  if (IsNuESignal(vtx, det, AntiNu) && !IsNuECC0pi(vtx,det, AntiNu) ) return 1;
  
  return 0;
}

//********************************************************************
Int_t nue_categs::NpiC(const AnaTrueVertexB& vtx){
//********************************************************************

  const AnaTrueVertex *vtx2 = static_cast<const AnaTrueVertex*> (&vtx);
  if(!vtx2) return 0;

  Int_t Npipos =   vtx2->NPrimaryParticles[ParticleId::kPiPos];
  Int_t Npineg =   vtx2->NPrimaryParticles[ParticleId::kPiNeg];

  return (Npipos+Npineg);
}

//********************************************************************
Int_t nue_categs::Npi0(const AnaTrueVertexB& vtx){
//********************************************************************

  const AnaTrueVertex *vtx2 = static_cast<const AnaTrueVertex*> (&vtx);
  if(!vtx2) return 0;

  return vtx2->NPrimaryParticles[ParticleId::kPiZero];
}

//********************************************************************
Int_t nue_categs::NKaonRhoEta(const AnaTrueVertexB& vtx){
//******************************************************************** 
 
  const AnaTrueVertex *vtx2 = static_cast<const AnaTrueVertex*> (&vtx);
  if(!vtx2) return 0;

  Int_t Nkaon0 = vtx2->NPrimaryParticles[ParticleId::kK0] + vtx2->NPrimaryParticles[ParticleId::kAntiK0] + vtx2->NPrimaryParticles[ParticleId::kK0L]  + vtx2->NPrimaryParticles[ParticleId::kK0S];
  Int_t NkaonC = vtx2->NPrimaryParticles[ParticleId::kKPos] + vtx2->NPrimaryParticles[ParticleId::kKNeg];
  Int_t Neta   = vtx2->NPrimaryParticles[ParticleId::kEta];
  Int_t Nrho   = vtx2->NPrimaryParticles[ParticleId::kRho0];

  return (Nkaon0 + NkaonC + Neta + Nrho);
}

//********************************************************************
bool nue_categs::HasParentOfType(AnaEventB* event, AnaTrueParticleB* track, int pdg) {
//********************************************************************

  if (!track) return false;

  bool matches = false;
  AnaTrueParticleB* parent = anaUtils::GetTrueParticleByID(*event, track->ParentID);

  while (parent) {
    if (parent->PDG == pdg) {
      matches = true;
      break;
    }
    parent = anaUtils::GetTrueParticleByID(*event, parent->ParentID);
  }

  return matches;
}

//********************************************************************
bool nue_categs::ThereWasGammaInFGD(const AnaTrueVertexB& vtxB, const SubDetId::SubDetEnum det){
//********************************************************************
  
  bool flag = false;

  for(int i = 0; i < vtxB.nTrueParticles; i++){
    AnaTrueParticleB* truth = vtxB.TrueParticles[i];
    if(truth->PDG != 22)
      continue;

    if(anaUtils::GetDetector(truth->PositionEnd) == det){
      flag = true;
      break;
    }
  }

  return flag;
}

//********************************************************************
Int_t nue_categs::GetReactionNoFVGamma(AnaTrackB* track){
//********************************************************************

  if (!track || !track->GetTrueParticle() || track->GetTrueParticle()->PDG == 0) return CATNOTRUTH; // no truth

  AnaTrueVertex* vtx = static_cast<AnaTrueVertex*> (track->GetTrueParticle()->TrueVertex);
  if(!vtx) return CATNOTRUTH;
  //const AnaTrueVertex* vtx = static_cast<const AnaTrueVertex*>(&vtxB);

  int reac = vtx->ReacCode;
  if(abs(vtx->NuPDG) == 12 && abs(reac) < 30)
    return 50;
  else if(abs(vtx->NuPDG) == 14){
    if (abs(reac) == 1 || abs(reac) == 2)
      return 10;  // CCQE
    else if(abs(reac) == 12)
      return 11;  // CCRES 1pi0
    else if((abs(reac) >= 11 && abs(reac) <= 23 && abs(reac) != 12) || abs(reac) == 16)
      return 111; // CCRES other
    else if(abs(reac) == 26)
      return 12;  // CC DIS
    else if(abs(reac) == 31 || abs(reac) == 32)
      return 21;  // NC RES 1pi0
    else if(abs(reac) == 38 || abs(reac) == 39)
      return 100; // NC 1gamma
    else if((abs(reac) >= 33 && abs(reac) <= 45 && (abs(reac) == 38 || abs(reac) == 39)) || abs(reac) == 36)
      return 121; // NC RES other
    else if(abs(reac) == 46)
      return 22;  // NC DIS
    else
      return -100;
  }
  else return -100;
  return -1;

}

//********************************************************************
Int_t nue_categs::GetPiTopo(AnaTrackB* track) {
//********************************************************************

  if (!track || !track->GetTrueParticle() || track->GetTrueParticle()->PDG == 0) return CATNOTRUTH; // no truth

  AnaTrueVertex* vtx = static_cast<AnaTrueVertex*> (track->GetTrueParticle()->TrueVertex);
  if(!vtx) return CATNOTRUTH;

  // "NC 1#pi^{0}"      21
  // "NC 1#pi^{#pm}"    22
  // "NC multipi"       23
  // "CC 1#pi^{0}"      11
  // "CC 1#pi^{#pm}"    12
  // "CC multipi"       13
  // "CC 0pi"           14
  // "NC single #gamma" 100
  // "Other"            -100
  // "no truth"         -1
  int reac = vtx->ReacCode;
  if(IsNCGammaSignal(*vtx, SubDetId::kFGD)) return 100;
  if(abs(vtx->NuPDG) == 12 && abs(reac) < 30)
    return -100;

  // Topologies
  int topo;
  
  if     (vtx->NPrimaryParticles[ParticleId::kPiZero] == 1 &&
	  vtx->NPrimaryParticles[ParticleId::kPiPos]  == 0 &&
	  vtx->NPrimaryParticles[ParticleId::kPiNeg]  == 0)
    topo = 21; // 1#pi^{0} 
  else if(vtx->NPrimaryParticles[ParticleId::kPiZero]  == 0 &&
	  ((vtx->NPrimaryParticles[ParticleId::kPiPos] == 1 &&
	    vtx->NPrimaryParticles[ParticleId::kPiNeg] == 0) ||
	   (vtx->NPrimaryParticles[ParticleId::kPiPos] == 0 &&
	    vtx->NPrimaryParticles[ParticleId::kPiNeg] == 1)))
    topo = 22; // 1#pi^{#pm}
  else if(vtx->NPrimaryParticles[ParticleId::kPiZero] +
	  vtx->NPrimaryParticles[ParticleId::kPiPos]  +
	  vtx->NPrimaryParticles[ParticleId::kPiNeg] > 1)
    topo = 23; // multi #pi
  else if(vtx->NPrimaryParticles[ParticleId::kPiPos] == 0 &&
          vtx->NPrimaryParticles[ParticleId::kPiNeg] == 0 &&
          vtx->NPrimaryParticles[ParticleId::kPiZero] == 0 && abs(reac) < 30)
    topo = 14; // CC0#pi
  else
    return -100;

  if(abs(reac) > 30) // NC (no muon)
    return topo;
  else
    return topo - 10;
}


//********************************************************************
bool nue_categs::IsNCGammaSignal(const AnaTrueVertexB& vtx, SubDetId::SubDetEnum FGD){
//********************************************************************

  (void) FGD;

  const AnaTrueVertex *vtx2 = static_cast<const AnaTrueVertex*> (&vtx);
  
  return (vtx2->ReacCode == 38 || vtx2->ReacCode == 39);// && anaUtils::InFiducialVolume(FGD, vtx2->Position);

}
