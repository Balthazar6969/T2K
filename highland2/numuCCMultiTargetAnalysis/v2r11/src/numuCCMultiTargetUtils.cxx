#include "numuCCMultiTargetUtils.hxx"
#include "CategoriesUtils.hxx"
#include "DetectorDefinition.hxx"
#include "ND280AnalysisUtils.hxx"
#include "CategoryManager.hxx" // needed for AddCategory


//********************************************************************
void numuccmultitarget_utils::AddCategories(const std::string& prefix) {
//********************************************************************

  // Categories for module types in which the interaction happens (water, vert layer, horizontal layer, gap)
  const int NMODULETYPETYPES = 6;
  std::string moduletype_types[NMODULETYPETYPES] = {"vert layers (X)" , "horiz layers (Y)", "water modules" , "gaps", "BKG", "out FV"};
  int moduletype_codes[NMODULETYPETYPES] = {0, 1, 2, 3, CATOTHER, CATOUTFV};
  int moduletype_colors[NMODULETYPETYPES] = {2, 3, 4, 7, COLBKG, COLOUTFV};
  anaUtils::_categ->AddCategory("fgd2moduletypeCC", NMODULETYPETYPES, moduletype_types, moduletype_codes, moduletype_colors);
  anaUtils::_categ->AddCategory("fgd2moduletype",   NMODULETYPETYPES, moduletype_types, moduletype_codes, moduletype_colors);

  //Category for drawing based on the target information and CCQE reaction
  const int NTARGETCCQE = 8;
  std::string targetccqe_types[NTARGETCCQE] = {"#nu_{#mu} CCQE on Oxygen",
                                               "#nu_{#mu} CC-nonQE on Oxygen",
                                               "Other on Oxygen",
                                               "#nu_{#mu} CCQE on Carbon",
                                               "#nu_{#mu} CC-nonQE on Carbon",
                                               "Other on Carbon",
                                               "Other Material",
                                               "OOFV"
                                              };
  int targetccqe_codes[NTARGETCCQE] = { 1, 2, 3, 4, 5, 6, CATOTHER, CATOUTFV};
  int targetccqe_colors[NTARGETCCQE] = { 7, 4, 3, 2, 6, 31, COLOTHER, COLOUTFV};
  anaUtils::_categ->AddCategory(prefix + "fgd2targetCCQE", NTARGETCCQE, targetccqe_types, targetccqe_codes, targetccqe_colors);

  //same as above, but for CC-Inclusive purposes
  const int NTARGETCC = 6;
  std::string targetcc_types[NTARGETCC] = {"#nu_{#mu} CC on Oxygen",
                                           "Other on Oxygen",
                                           "#nu_{#mu} CC on Carbon",
                                           "Other on Carbon",
                                           "Other Material",
                                           "OOFV"
                                          };
  int targetcc_codes[NTARGETCC] = { 1, 3, 4, 6, CATOTHER, CATOUTFV};
  int targetcc_colors[NTARGETCC] = { 7, 3, 2, 31, COLOTHER, COLOUTFV};
  anaUtils::_categ->AddCategory(prefix + "fgd2targetCC", NTARGETCC, targetcc_types, targetcc_codes, targetcc_colors);

}


//********************************************************************
int numuccmultitarget_utils::GetTargetCCQE(const AnaTrueVertex* trueVertex, SubDetId::SubDetEnum det){
//********************************************************************
/// not generalized for antinu
  /* Classify target types
     1 = CCQE Oxygen
     2 = CC-nonQE Oxygen
     3 = Other reaction Oxygen (NC, nue, anti-numu)
     4 = CCQE Carbon
     5 = CC-nonQE Carbon
     6 = Other reaction Carbon
     7 = Other Material (G10, glue, iron, etc.)
     -1 = no truth
  */

  if ( ! anaUtils::InFiducialVolume(det, trueVertex->Position)) return CATOUTFV; // out of FGD

  int reac   = trueVertex->ReacCode;
  int nutype = trueVertex->NuPDG;
  int targetcode = anaUtils::GetTargetCode(trueVertex);

  // Nuclear codes are given as 10-digit numbers +-100ZZZAAAI.
  //For a nucleus consisting of np protons and nn neutrons
  // A = np + nn and Z = np.
  // I gives the isomer level, with I = 0 corresponding
  // to the ground state and I >0 to excitations
  //
  //!!! I = 1 is assigned for all excitation states in Geant4

  if (targetcode==6) { //1000060120)//Carbon
    if (nutype!=14) return 6; // Other reaction (not numu)
    if (reac == 1) return 4; //CCQE
    if ((reac >10 && reac<14) ||
        (reac >16 && reac<30) ||
        (reac ==16)
       ) return 5; //CC other (RES || DIS || COH)
    else return 6;
  }

  if (targetcode==8) { //1000080160)//Oxygen
    if (nutype!=14) return 3; // Other reaction (not numu)
    if (reac == 1) return 1; //CCQE
    if ((reac >10 && reac<14) ||
        (reac >16 && reac<30) ||
        (reac ==16)
       ) return 2; //CC other (RES || DIS || COH)
    else return 3;
  }

  return 7; //Other material

}


//********************************************************************
int numuccmultitarget_utils::GetTargetCC(const AnaTrueVertex* trueVertex, SubDetId::SubDetEnum det){
//********************************************************************
  /* Classify target types
     1 = CC Oxygen
     3 = Other reaction Oxygen (NC, nue, anti-numu)
     4 = CC Carbon
     6 = Other reaction Carbon
     7 = Other Material (G10, glue, iron, etc.)
     8 = OOFV (neither FGD)
     -1 = no truth
  */

  int targetccqe = numuccmultitarget_utils::GetTargetCCQE(trueVertex, det);
  if (targetccqe==2) targetccqe=1;
  if (targetccqe==5) targetccqe=4;
  return targetccqe;
}

//********************************************************************
int numuccmultitarget_utils::GetFgdLayer(bool IsMC, const Float_t* pos){
//********************************************************************

    // this has to be done like this because Float_t approximation leaves gaps
    Float_t initZ, finalZ;
    Float_t barCenter[30];

    // FGD1 bars (0-29)
    for (Int_t i=0; i<30; i++) {

      barCenter[i] = DetDef::fgd1BarCenter[i];

      if (i == 0) initZ = barCenter[0] - (DetDef::fgdXYBarWidth/2) - DetDef::fgdXYGlueWidth - DetDef::fgdXYSkinWidth - DetDef::fgdXYAirWidth;
      else initZ = finalZ;

      if (i%2 == 0)
        finalZ = barCenter[i] + (DetDef::fgdXYBarWidth/2) + (DetDef::fgdXYMiddleGlueWidth/2);
      else if (i%2 == 1)
        finalZ = barCenter[i] + (DetDef::fgdXYBarWidth/2) + DetDef::fgdXYGlueWidth + DetDef::fgdXYSkinWidth + (DetDef::fgdXYAirWidth/2);
      if (i == 29) finalZ += (DetDef::fgdXYAirWidth/2);

      if (pos[2] > initZ && pos[2] < finalZ) return i;
    }


    // FGD2 bars (30-43)
    for (Int_t i=0; i<14; i++) {

      if (IsMC) barCenter[i] = DetDef::fgd2BarCenterMC[i];
      else                 barCenter[i] = DetDef::fgd2BarCenterDATA[i];

      if (i == 0) initZ = barCenter[0] - (DetDef::fgdXYBarWidth/2) - DetDef::fgdXYGlueWidth - DetDef::fgdXYSkinWidth - DetDef::fgdXYAirWidth;
      else initZ = finalZ;

      if (i%2 == 0)
        finalZ = barCenter[i] + (DetDef::fgdXYBarWidth/2) + (DetDef::fgdXYMiddleGlueWidth/2);
      else if (i%2 == 1)
        finalZ = barCenter[i] + (DetDef::fgdXYBarWidth/2) + DetDef::fgdXYGlueWidth + DetDef::fgdXYSkinWidth + DetDef::fgdXYAirWidth + (DetDef::fgdWaterModuleWidth/2);
      if (i == 13) finalZ -= (DetDef::fgdWaterModuleWidth/2);

      if (pos[2] > initZ && pos[2] < finalZ) return (i+30);
    }

//    std::cout << "Error in GetFgdLayer: layer not found for this position: " << pos[2] << std::endl; // triggered for truemu OOFV
    return -1;
}


//********************************************************************
int numuccmultitarget_utils::GetFgdModuleTypeCC(bool IsMC, const AnaTrueVertex* vtx, SubDetId::SubDetEnum det, bool IsAntinu){
//********************************************************************
  // always OOFV excluded first
  if ( ! anaUtils::InFiducialVolume(det,vtx->Position))  return CATOUTFV;
  if (anaUtils::GetReactionCC(*vtx, det, IsAntinu) != 1) return CATOTHER; //BKG
  return anaUtils::GetFgdModuleTypeNoFV(IsMC, vtx->Position);
}

// ********************************************************************
float numuccmultitarget_utils::CalculateLineLineDistance(TVector3 track_start_pos,
                                                         TVector3 track_end_pos,
                                                         TVector3 hit_pos) {
// ********************************************************************
  // Calculate the minimum distance between the fitted track and the hit
  // (the hit is actually a line in space, as we always don't know either
  // the x or the y co-ordinate)
  // (For info, see: http://mathworld.wolfram.com/Line-LineDistance.html)
  //
  // Equations of the line:
  //   r1 = track_start_pos + (track_end_pos - track_start_pos)*s
  //   r2 = hit_pos         + (unit_vector_in_direction_of_bar)*t
  //
  // Calculating the distance:
  //  - define some vectors:
  //   a = track_end_pos - track_start_pos
  //   b = unit_vector_in_direction_of_bar
  //   c = hit_pos - track_start_pos
  //  - then, distance is easy:
  //   distance = |c.(a x b)| / |(a x b)|

  TVector3 a = track_end_pos - track_start_pos;
  TVector3 b(-9999,-9999,-9999);
  TVector3 c = hit_pos - track_start_pos;

  float distance = -9999;

  if(hit_pos.X() < 0.0001){ // i.e. is equal to zero
    // No x info
    b.SetXYZ(1,0,0);
  }
  else if(hit_pos.Y() < 55.0001 && hit_pos.Y() > 49.999) {// i.e. is equal to 55
    // No y info
    b.SetXYZ(0,1,0);
  }
  else{
    std::cout<<"ERROR: Can't determine bar type (x or y), hit_pos = "<<hit_pos.X()<<", "<<hit_pos.Y()<<", "<<hit_pos.Z()<<std::endl;
    return distance;
  }

  TVector3 a_cross_b = a.Cross(b);
  double c_dot_a_cross_b = c.Dot(a_cross_b);

  double numerator = fabs(c_dot_a_cross_b);
  double denominator = a_cross_b.Mag();

  distance = numerator / denominator;

  return distance;
}

// ********************************************************************
TVector3 numuccmultitarget_utils::GetFittedPos(TVector3 A, TVector3 B, double z){
// ********************************************************************

  TVector3 pos(-9999,-9999,-9999);

  if(z == -999){
    return pos;
  }

  TVector3 direction;
  direction = B - A;

  double t = (z - A.Z())/direction.Z();

  double x = A.X() + direction.X()*t;
  double y = A.Y() + direction.Y()*t;

  pos.SetXYZ(x, y, z);

  return pos;
}


// ********************************************************************
float numuccmultitarget_utils::GetDeltaChi2(TVector3 hit, TVector3 fit){
// ********************************************************************

  float delta = 0;

  if (hit.X() == 0 ) delta = hit.Y()*hit.Y() - fit.Y()*fit.Y();
  if (hit.Y() == 55 ) delta = hit.X()*hit.X() - fit.X()*fit.X();

  return delta;
}
