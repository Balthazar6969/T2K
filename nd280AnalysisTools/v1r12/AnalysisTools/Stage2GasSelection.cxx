#include "Stage2GasSelection.hxx"
//#include "libReadoaAnalysis/libReadoaAnalysisProjectHeaders.h"
//#include <TTruthUtils.hxx>

//===========================================================================================
// Short function for sorting paths by momentum
bool MomentumGreaterThan_V1(path_pattern_pair path_A,
                            path_pattern_pair path_B) {
  return (path_A.first->Momentum > path_B.first->Momentum);
}

//===========================================================================================
// Short function for sorting paths by momentum (alternate input)
bool MomentumGreaterThan_V2(path_junctions_pair path_A,
                            path_junctions_pair path_B) {
  return (path_A.first->Momentum > path_B.first->Momentum);
}

//===========================================================================================
// Short function to test whether a point is inside the fidicual volume
bool isInFV(TVector3 max_coords, TVector3 min_coords, TVector3 testpos) {
  bool fv_flag = true;

  if ((std::abs(testpos.X()) > max_coords.X()) ||
      (std::abs(testpos.X()) < min_coords.X())) {
    fv_flag = false;
  }
  if ((testpos.Y() > max_coords.Y()) || (testpos.Y() < min_coords.Y())) {
    fv_flag = false;
  }
  if ((testpos.Z() > max_coords.Z()) || (testpos.Z() < min_coords.Z())) {
    fv_flag = false;
  }

  return fv_flag;
}

//============================================================================================
// Short function to test whether a true vertex is a signal interaction. Returns
// multiple codes.
int trueVertexTruthCode(ND::TTruthVerticesModule::TTruthVertex *vert,
                        TVector3 max_coords, TVector3 min_coords, int tpc,
                        bool isGENIE) {
  int det = vert->Subdetector;
  TLorentzVector pos = vert->Position;
  std::string reaccode = vert->ReactionCode;
  int target_pdg = vert->TargetPDG;
  int nu_pdg = vert->NeutrinoPDG;
  int atomic_number = (target_pdg % 1000000000) / 10000;

  int returnvalue = -1;

  bool isCC = false;
  if (isGENIE) {
    if (reaccode.find("[CC]") != std::string::npos)
      isCC = true;
  } else {
    if (atoi(reaccode.c_str()) < 30)
      isCC = true;
  }

  if (det != tpc + 5) // Vertex is not in TPC
  {
    returnvalue = 0;
  } else // Vertex is in TPC
  {
    returnvalue = 1;

    if ((nu_pdg == 14) && (isCC) && (atomic_number == 18)) {
      returnvalue = 6;
    }

    else {
      if (isInFV(max_coords, min_coords, pos.Vect())) // Vertex is in TPC FV
      {
        returnvalue = 2;
        if (nu_pdg == 14) // Incoming particle is numu, not nue or anti-nu
        {
          returnvalue = 3;
          if (isCC) // Interaction is CC
          {
            returnvalue = 4;
            // if (atomic_number == 18) // Target nucleus is Ar
            //{
            //  returnvalue = 6;
            //}
          }
        }
      }
    }
  }

  return returnvalue;
}

//=============================================================================================
// Function to match *patterns* to their parent true vertex
int patternTrueVertexMatch(ND::TTRExReconModule::TTPCAnaPattern *patt,
                           TTruthUtils trutil) {
  TClonesArray *paths = patt->Paths;

  // Get all true vertices matched to paths in the pattern
  std::set<int> vertex_ids;
  for (int i = 0; i < patt->NPaths; i++) {
    ND::TTRExReconModule::TTPCAnaPath *path =
        (ND::TTRExReconModule::TTPCAnaPath *)paths->At(i);
    ND::TTruthVerticesModule::TTruthVertex *parent_vertex = NULL;
    parent_vertex = trutil.GetVertexOfTrajectory(path->G4ID);
    if (!parent_vertex)
      continue;
    vertex_ids.insert(parent_vertex->ID);
  }

  // Count paths associated to each true vertex
  std::vector<int> id_count;
  for (std::set<int>::iterator listIt = vertex_ids.begin();
       listIt != vertex_ids.end(); listIt++) {
    int path_count = 0;
    for (int j = 0; j < patt->NPaths; j++) {
      ND::TTRExReconModule::TTPCAnaPath *path =
          (ND::TTRExReconModule::TTPCAnaPath *)paths->At(j);
      ND::TTruthVerticesModule::TTruthVertex *parent_vertex = NULL;
      parent_vertex = trutil.GetVertexOfTrajectory(path->G4ID);
      if (!parent_vertex)
        continue;
      if (*listIt == parent_vertex->ID)
        path_count++;
    }
    id_count.push_back(path_count);
  }

  // Choose the true vertex with the most associated paths
  int most_associations = 0;
  int chosen_index = -1;
  for (unsigned int i = 0; i < vertex_ids.size(); i++) {
    if (id_count[i] > most_associations) {
      most_associations = id_count[i];
      chosen_index = i;
    }
  }

  // Grab the ID from the list
  if (chosen_index != -1) {
    int chosen_ID = -1;
    int counter = 0;
    for (std::set<int>::iterator listIt = vertex_ids.begin();
         listIt != vertex_ids.end(); listIt++) {
      if (counter == chosen_index)
        chosen_ID = *listIt;

      counter++;
    }
    return chosen_ID;
  } else {
    std::cout << "WARNING: pattern not matched" << std::endl;
    return chosen_index;
  }
}

//=============================================================================================
// Function to match junctions to their true parent vertex
int getTrueVertexFromJunction(
    std::vector<ND::TTRExReconModule::TTPCAnaPath *> vertex_paths,
    TTruthUtils trutil) {
  // Get all true vertices matched to paths from the vertex
  std::set<int> vertex_ids;
  for (unsigned int i = 0; i < vertex_paths.size(); i++) {
    ND::TTruthVerticesModule::TTruthVertex *parent_vertex = NULL;
    parent_vertex = trutil.GetVertexOfTrajectory(vertex_paths[i]->G4ID);
    if (!parent_vertex)
      continue;
    vertex_ids.insert(parent_vertex->ID);
  }

  // Count paths associated to each true vertex
  std::vector<int> id_count;
  for (std::set<int>::iterator listIt = vertex_ids.begin();
       listIt != vertex_ids.end(); listIt++) {
    int path_count = 0;
    for (unsigned int j = 0; j < vertex_paths.size(); j++) {
      ND::TTruthVerticesModule::TTruthVertex *parent_vertex = NULL;
      parent_vertex = trutil.GetVertexOfTrajectory(vertex_paths[j]->G4ID);
      if (!parent_vertex)
        continue;
      if (*listIt == parent_vertex->ID)
        path_count++;
    }
    id_count.push_back(path_count);
  }

  // Choose the true vertex with the most associated paths
  int most_associations = 0;
  int chosen_index = -1;
  int chosen_ID = -1;

  int counter = 0;
  for (std::set<int>::iterator listIt = vertex_ids.begin();
       listIt != vertex_ids.end(); listIt++) {
    if (id_count[counter] > most_associations) {
      most_associations = id_count[counter];
      chosen_index = counter;
      chosen_ID = *listIt;
    }
    counter++;
  }

  if (chosen_index != -1)
    return chosen_ID;
  else {
    std::cout << "WARNING: junction not matched" << std::endl;
    return chosen_index;
  }
}

//=============================================================================================
// Function to evaluate truth status of a path
int evaluatePathTruth(
    ND::TTRExReconModule::TTPCAnaPath *path,
    std::vector<ND::TTRExReconModule::TTPCAnaPattern *> other_matched_patterns,
    TTruthUtils trutil, TVector3 fv_maxima, TVector3 fv_minima, int tpc,
    bool isGENIE) {
  int code = -1;

  ND::TTruthVerticesModule::TTruthVertex *parent_vertex = NULL;
  parent_vertex = trutil.GetVertexOfTrajectory(path->G4ID);
  if (!parent_vertex) {
    std::cout << "Path with no parent" << std::endl;
    return code;
  }

  code = trueVertexTruthCode(parent_vertex, fv_maxima, fv_minima, tpc, isGENIE);

  if ((code > 1) &&
      (other_matched_patterns.size() > 0)) // This may be a split pattern
  {
    // Check to see if any other pattern is closer to the true vertex
    TVector3 start_separation =
        path->FirstPosition - parent_vertex->Position.Vect();
    TVector3 end_separation =
        path->LastPosition - parent_vertex->Position.Vect();
    double min_separation = std::min(std::abs(start_separation.Mag()),
                                     std::abs(end_separation.Mag()));

    bool split_end = false;
    for (unsigned int i = 0; i < other_matched_patterns.size(); i++) {
      std::vector<TVector3> possible_positions;
      TClonesArray *pattern_paths = other_matched_patterns[i]->Paths;
      TClonesArray *pattern_juncs = other_matched_patterns[i]->Junctions;

      for (int j = 0; j < other_matched_patterns[i]->NJunctions; j++) {
        ND::TTRExReconModule::TTPCAnaJunction *junc =
            (ND::TTRExReconModule::TTPCAnaJunction *)pattern_juncs->At(j);
        possible_positions.push_back(junc->Position.Vect());
      }
      for (int j = 0; j < other_matched_patterns[i]->NPaths; j++) {
        ND::TTRExReconModule::TTPCAnaPath *otherpath =
            (ND::TTRExReconModule::TTPCAnaPath *)pattern_paths->At(j);
        possible_positions.push_back(otherpath->FirstPosition);
        possible_positions.push_back(otherpath->LastPosition);
      }

      for (unsigned int j = 0; j < possible_positions.size(); j++) {
        double displacement = std::abs(
            (possible_positions[j] - parent_vertex->Position.Vect()).Mag());
        if ((displacement < min_separation) && (displacement > 100.0))
          split_end = true;
      }
    }

    /*
    if (split_end)
    {
      if (code == 0) //Split path from OOFV interaction
        code = 1;
      if (code < 6) //Split path from background in FV
        code = 5;
      if (code == 6) //Split path from signal
        code = 7;
    }
    */
  }

  return code;
}

//=============================================================================================
// Function to evaluate truth status of a junction
int evaluateJunctionTruth(
    TVector3 junction_pos,
    std::vector<ND::TTRExReconModule::TTPCAnaPath *> vertex_paths,
    std::vector<ND::TTRExReconModule::TTPCAnaPattern *> other_matched_patterns,
    TTruthUtils trutil, TVector3 fv_maxima, TVector3 fv_minima, int tpc,
    bool isGENIE) {
  int code = -1;

  ND::TTruthVerticesModule::TTruthVertex *parent_vertex =
      trutil.GetVertexById(getTrueVertexFromJunction(vertex_paths, trutil));
  if (!parent_vertex) {
    std::cout << "Junction with no parent" << std::endl;
    return code;
  }

  code = trueVertexTruthCode(parent_vertex, fv_maxima, fv_minima, tpc, isGENIE);

  if ((code > 1) &&
      (other_matched_patterns.size() > 0)) // This may be a split pattern
  {
    // Check to see if any other pattern is closer to the true vertex
    TVector3 vertex_separation = junction_pos - parent_vertex->Position.Vect();
    double absolute_separation = std::abs(vertex_separation.Mag());

    bool split_end = false;
    for (unsigned int i = 0; i < other_matched_patterns.size(); i++) {
      std::vector<TVector3> possible_positions;
      TClonesArray *pattern_paths = other_matched_patterns[i]->Paths;
      TClonesArray *pattern_juncs = other_matched_patterns[i]->Junctions;

      for (int j = 0; j < other_matched_patterns[i]->NJunctions; j++) {
        ND::TTRExReconModule::TTPCAnaJunction *junc =
            (ND::TTRExReconModule::TTPCAnaJunction *)pattern_juncs->At(j);
        possible_positions.push_back(junc->Position.Vect());
      }
      for (int j = 0; j < other_matched_patterns[i]->NPaths; j++) {
        ND::TTRExReconModule::TTPCAnaPath *path =
            (ND::TTRExReconModule::TTPCAnaPath *)pattern_paths->At(j);
        possible_positions.push_back(path->FirstPosition);
        possible_positions.push_back(path->LastPosition);
      }

      for (unsigned int j = 0; j < possible_positions.size(); j++) {
        double displacement = std::abs(
            (possible_positions[j] - parent_vertex->Position.Vect()).Mag());
        if (displacement < absolute_separation)
          split_end = true;
      }
    }

    /*
    if (split_end)
    {
      if (code < 6) //Split path from background
        code = 5;
      if (code == 6) //Split path from signal
        code = 7;
    }
    */
  }

  return code;
}

//==============================================================================================
// Function to impose bunch timing cuts (hardcoded for now, should be done more
// betterly)
bool isInBunch(int run_number, double T0, bool isMC) {
  // Set bunch times to MC defaults
  double bunch_times[8] = {2750.2, 3332.0, 3914.7, 4497.0,
                           5078.4, 5659.7, 6243.4, 6824.2};

  if (!isMC) {
    if ((run_number >= 0) && (run_number < 6000)) {
      bunch_times[0] = 2839.7;
      bunch_times[1] = 3423.5;
      bunch_times[2] = 4005.4;
      bunch_times[3] = 4588.6;
      bunch_times[4] = 5172.2;
      bunch_times[5] = 5754.6;
      bunch_times[6] = -1000.0;
      bunch_times[7] = -1000.0;
    }
    if ((run_number >= 6000) && (run_number < 7000)) {
      bunch_times[0] = 2853.95;
      bunch_times[1] = 3444.15;
      bunch_times[2] = 4030.41;
      bunch_times[3] = 4620.34;
      bunch_times[4] = 5180.28;
      bunch_times[5] = 5770.12;
      bunch_times[6] = 6343.77;
      bunch_times[7] = 6924.67;
    }
    if ((run_number >= 7000) && (run_number < 8000)) {
      bunch_times[0] = 3019.11;
      bunch_times[1] = 3597.74;
      bunch_times[2] = 4180.73;
      bunch_times[3] = 4763.93;
      bunch_times[4] = 5346.49;
      bunch_times[5] = 5927.83;
      bunch_times[6] = 6508.5;
      bunch_times[7] = 7093.56;
    }
    if ((run_number >= 8000) && (run_number < 8550)) {
      bunch_times[0] = 3024.22;
      bunch_times[1] = 3606.11;
      bunch_times[2] = 4188.01;
      bunch_times[3] = 4769.9;
      bunch_times[4] = 5351.79;
      bunch_times[5] = 5933.68;
      bunch_times[6] = 6515.58;
      bunch_times[7] = 7097.47;
    }
    if ((run_number >= 8550) && (run_number < 8800)) {
      bunch_times[0] = 3013.55;
      bunch_times[1] = 3597.55;
      bunch_times[2] = 4178.24;
      bunch_times[3] = 4758.78;
      bunch_times[4] = 5338.21;
      bunch_times[5] = 5927.31;
      bunch_times[6] = 6505.71;
      bunch_times[7] = 7086.80;
    }
    if ((run_number >= 8895) && (run_number < 9300)) {
      bunch_times[0] = 3014.69;
      bunch_times[1] = 3600.57;
      bunch_times[2] = 4178.89;
      bunch_times[3] = 4764.24;
      bunch_times[4] = 5342.35;
      bunch_times[5] = 5931.73;
      bunch_times[6] = 6506.37;
      bunch_times[7] = 7090.15;
    }
    if ((run_number >= 9301) && (run_number < 9999)) {
      bunch_times[0] = 3014.69;
      bunch_times[1] = 3600.57;
      bunch_times[2] = 4178.89;
      bunch_times[3] = 4764.24;
      bunch_times[4] = 5342.35;
      bunch_times[5] = 5931.73;
      bunch_times[6] = 6506.37;
      bunch_times[7] = 7090.15;
    }
  }

  // All numbers taken from BunchPosition.dat in highlandIO.
  // These are PRODUCTION 6 numbers and will not work with pre-production 6
  // files
  // What are you doing passing pre-P6 files through TREx anyway?

  double sigma = 15.0;
  double nsigma = 4.0;

  // Again default values taken from highland. Worth checking to see if these
  // change.

  bool t0_in_bunch = false;
  for (int i = 0; i < 8; i++) {
    double upper_limit = bunch_times[i] + nsigma * sigma;
    double lower_limit = bunch_times[i] - nsigma * sigma;
    if ((T0 >= lower_limit) && (T0 <= upper_limit))
      t0_in_bunch = true;
  }

  return t0_in_bunch;
}

//==============================================================================================
// Function to identify muon paths
bool isMuon(double muon_pull, double pion_pull, double proton_pull,
            double electron_pull, double momentum) {
  double muon_probability = std::exp(-1.0 * (muon_pull) * (muon_pull) / 2);
  double pion_probability = std::exp(-1.0 * (pion_pull) * (pion_pull) / 2);
  double electron_probability =
      std::exp(-1.0 * (electron_pull) * (electron_pull) / 2);
  double proton_probability =
      std::exp(-1.0 * (proton_pull) * (proton_pull) / 2);

  double prob_sum = muon_probability + pion_probability + electron_probability +
                    proton_probability;
  double muon_likelihood = muon_probability / prob_sum;
  double pion_likelihood = pion_probability / prob_sum;
  double proton_likelihood = proton_probability / prob_sum;
  double electron_likelihood = electron_probability / prob_sum;

  double magic_number_1 =
      (muon_likelihood + pion_likelihood) / (1.0 - proton_likelihood);
  if ((1.0 - proton_likelihood) == 0)
    magic_number_1 = -2;
  double magic_number_2 =
      (muon_likelihood + pion_likelihood) / (1.0 - electron_likelihood);
  if ((1.0 - electron_likelihood == 0))
    magic_number_2 = -2;

  if ((magic_number_1 < -1) || (magic_number_2 < -1))
    return false;

  bool not_corner = true;
  bool not_edge = true;

  if ((magic_number_1 > 0.5) && (magic_number_2 < 0.05))
    not_corner = false;
  // if ((magic_number_1 < 0.1)||(magic_number_2 < 0.1)) not_edge = false;

  bool magic_numbers_ok = false;
  if (not_corner && not_edge)
    magic_numbers_ok = true;

  bool first_condition = false;
  bool second_condition = false;

  if (magic_numbers_ok || (momentum > 500))
    first_condition = true;
  if (muon_likelihood > 0.05)
    second_condition = true;

  if (first_condition && second_condition)
    return true;
  else
    return false;
}

//=============================================================================================
// Function to evaluate whether a path is a proton
bool isProton(double muon_pull, double pion_pull, double proton_pull,
              double electron_pull, double momentum) {
  if (!isMuon(muon_pull, pion_pull, proton_pull, electron_pull, momentum)) {
    double muon_probability = std::exp(-1.0 * (muon_pull) * (muon_pull) / 2);
    double pion_probability = std::exp(-1.0 * (pion_pull) * (pion_pull) / 2);
    double electron_probability =
        std::exp(-1.0 * (electron_pull) * (electron_pull) / 2);
    double proton_probability =
        std::exp(-1.0 * (proton_pull) * (proton_pull) / 2);

    double prob_sum = muon_probability + pion_probability +
                      electron_probability + proton_probability;
    double proton_likelihood = proton_probability / prob_sum;

    if (proton_likelihood > 0.5)
      return true;
  }

  return false;
}

//=============================================================================================
// Function to evaluate whether a junction represents a delta ray
bool isDelta(ND::TTRExReconModule::TTPCAnaJunction *junc,
             ND::TTRExReconModule::TTPCAnaPattern *parent_pattern) {
  if (!junc)
    return false;

  TClonesArray *Paths = parent_pattern->Paths;

  std::vector<ND::TTRExReconModule::TTPCAnaPath *> junc_paths;
  std::vector<ND::TTRExReconModule::TTPCAnaPath *> junc_mu_paths;
  std::vector<ND::TTRExReconModule::TTPCAnaPath *> junc_e_paths;
  std::vector<ND::TTRExReconModule::TTPCAnaPath *> pid_failed;
  std::vector<ND::TTRExReconModule::TTPCAnaPath *> none_of_the_above;

  for (int i = 0; i < parent_pattern->NPaths; i++) {
    ND::TTRExReconModule::TTPCAnaPath *pattern_path =
        (ND::TTRExReconModule::TTPCAnaPath *)Paths->At(i);

    for (int j = 0; j < junc->NPaths; j++) {
      if (pattern_path->PathID == junc->PathIDs[j]) {
        bool already_pushed = false;
        junc_paths.push_back(pattern_path);
        if ((pattern_path->LikFit) && (pattern_path->Success)) {
          if (isMuon(pattern_path->MuonPull, pattern_path->PionPull,
                     pattern_path->ProtonPull, pattern_path->ElePull,
                     pattern_path->Momentum)) {
            junc_mu_paths.push_back(pattern_path);
            already_pushed = true;
          }
          if (pattern_path->PID == 4) {
            junc_e_paths.push_back(pattern_path);
            already_pushed = true;
          }
        }
        if (pattern_path->PID == 3) {
          junc_e_paths.push_back(pattern_path);
          already_pushed = true;
        }
        if ((pattern_path->PID == 0) || (pattern_path->PID == 1)) {
          already_pushed = true;
          pid_failed.push_back(pattern_path);
        }

        if (!already_pushed)
          none_of_the_above.push_back(pattern_path);
      }
    }
  }

  // Look for electron-looking bits attached to a muon with no other particles
  if ((junc_e_paths.size() + pid_failed.size() > 0) &&
      (junc_mu_paths.size() > 0) && (none_of_the_above.size() == 0))
    return true;

  // Look at extent of junction
  TVector3 max_coords = junc->MaximumCoordinates;
  TVector3 min_coords = junc->MinimumCoordinates;
  TVector3 extent_3d = max_coords - min_coords;
  // TODO: fancify extent cut
  if (std::abs(extent_3d.X()) > 42.0)
    return true;

  // Look at total junction momentum
  double total_momentum = 0;
  for (unsigned int i = 0; i < junc_paths.size(); i++) {
    if ((junc_paths[i]->LikFit) && (junc_paths[i]->Success))
      total_momentum += junc_paths[i]->Momentum;
  }
  // if (total_momentum < 648.0) return true;
  if (total_momentum < 100.0)
    return true;

  return false;
}

//=============================================================================================
// Function to evaluate whether a junction represents a broken track
bool isBroken(ND::TTRExReconModule::TTPCAnaJunction *junc,
              ND::TTRExReconModule::TTPCAnaPattern *parent_pattern,
              TClonesArray *Patterns, int NPatterns, TVector3 fv_maxima,
              TVector3 fv_minima) {
  // For single path case
  if (!junc) {
    return true;
  }

  // For junction case
  if (junc) {
    TClonesArray *Paths = parent_pattern->Paths;

    std::vector<ND::TTRExReconModule::TTPCAnaPath *> junc_paths;
    std::vector<ND::TTRExReconModule::TTPCAnaPath *> junc_mu_paths;

    for (int i = 0; i < parent_pattern->NPaths; i++) {
      ND::TTRExReconModule::TTPCAnaPath *pattern_path =
          (ND::TTRExReconModule::TTPCAnaPath *)Paths->At(i);

      for (int j = 0; j < junc->NPaths; j++) {
        if (pattern_path->PathID == junc->PathIDs[j]) {
          junc_paths.push_back(pattern_path);
          if (isMuon(pattern_path->MuonPull, pattern_path->PionPull,
                     pattern_path->ProtonPull, pattern_path->ElePull,
                     pattern_path->Momentum))
            junc_mu_paths.push_back(pattern_path);
        }
      }
    }

    // Veto mu-mu junctions
    if (junc_mu_paths.size() == junc_paths.size())
      return true;

    // Veto colinear tracks forming false junctions
    for (unsigned int i = 0; i < junc_paths.size(); i++) {
      for (unsigned int j = 0; j < junc_paths.size(); j++) {
        if (i == j)
          continue;

        double costheta =
            std::abs(junc_paths[i]->Direction.Dot(junc_paths[j]->Direction));

        if (costheta > 0.85)
          return true;
      }
    }
  }

  return false;
}

//=============================================================================================
TLorentzVector
primaryMuonMomentum(ND::TTruthVerticesModule::TTruthVertex *parent_vertex,
                    int nvtx, TClonesArray *vertices, bool is_it_neut) {
  TLorentzVector momentum;

  ND::NRooTrackerVtx *neut_vertex = NULL;
  ND::GRooTrackerVtx *genie_vertex = NULL;
  for (int i = 0; i < nvtx; i++) {
    if (is_it_neut) {
      ND::NRooTrackerVtx *vtx = (ND::NRooTrackerVtx *)vertices->At(i);
      int vertex_id = vtx->TruthVertexID;
      if (vertex_id == parent_vertex->ID)
        neut_vertex = vtx;
    } else {
      ND::GRooTrackerVtx *vtx = (ND::GRooTrackerVtx *)vertices->At(i);
      int vertex_id = vtx->TruthVertexID;
      if (vertex_id == parent_vertex->ID)
        genie_vertex = vtx;
    }
  }

  if (neut_vertex) {
    for (int i = 0; i < neut_vertex->StdHepN; i++) {
      if (neut_vertex->StdHepStatus[i] == 0)
        continue; // Final state particles only (NEUT convention)
      if (neut_vertex->StdHepPdg[i] == 13) {
        momentum.SetPx(neut_vertex->StdHepP4[i][0]);
        momentum.SetPy(neut_vertex->StdHepP4[i][1]);
        momentum.SetPz(neut_vertex->StdHepP4[i][2]);
        momentum.SetE(neut_vertex->StdHepP4[i][3]);
      }
    }
  }
  if (genie_vertex) {
    for (int i = 0; i < genie_vertex->StdHepN; i++) {
      if (genie_vertex->StdHepStatus[i] != 1)
        continue; // Final state particles only (GENIE convention)
      if (genie_vertex->StdHepPdg[i] == 13) {
        momentum.SetPx(neut_vertex->StdHepP4[i][0]);
        momentum.SetPy(neut_vertex->StdHepP4[i][1]);
        momentum.SetPz(neut_vertex->StdHepP4[i][2]);
        momentum.SetE(neut_vertex->StdHepP4[i][3]);
      }
    }
  }

  if ((!neut_vertex) && (!genie_vertex))
    std::cout << "WARNING: attempted to find primary muon with no RooTracker "
                 "information. This should not happen!"
              << std::endl;

  return momentum;
}

//=============================================================================================
// Function to count charged trajectories from truth vertex
int countChargedTraj(ND::TTruthVerticesModule::TTruthVertex *parent_vertex,
                     int pdg, int nvtx, TClonesArray *vertices,
                     bool is_it_neut) {
  int n_traj = 0;

  ND::NRooTrackerVtx *neut_vertex = NULL;
  ND::GRooTrackerVtx *genie_vertex = NULL;
  for (int i = 0; i < nvtx; i++) {
    if (is_it_neut) {
      ND::NRooTrackerVtx *vtx = (ND::NRooTrackerVtx *)vertices->At(i);
      int vertex_id = vtx->TruthVertexID;
      if (vertex_id == parent_vertex->ID)
        neut_vertex = vtx;
    } else {
      ND::GRooTrackerVtx *vtx = (ND::GRooTrackerVtx *)vertices->At(i);
      int vertex_id = vtx->TruthVertexID;
      if (vertex_id == parent_vertex->ID)
        genie_vertex = vtx;
    }
  }

  if (neut_vertex) {
    for (int i = 0; i < neut_vertex->StdHepN; i++) {
      if (neut_vertex->StdHepStatus[i] == 0)
        continue; // Final state particles only (NEUT convention)
      if (neut_vertex->StdHepPdg[i] == pdg)
        n_traj++;
    }
    return n_traj;
  }
  if (genie_vertex) {
    for (int i = 0; i < genie_vertex->StdHepN; i++) {
      if (genie_vertex->StdHepStatus[i] != 1)
        continue; // Final state particles only (GENIE convention)
      if (genie_vertex->StdHepPdg[i] == pdg)
        n_traj++;
    }
    return n_traj;
  }
  if ((!neut_vertex) && (!genie_vertex))
    std::cout << "WARNING: attempted to count trajectories for vertex with no "
                 "RooTracker information. This should not happen!"
              << std::endl;

  return 0;
}

//=============================================================================================
// Function to assess total momentum in charged trajectories leaving the vertex
// (from specific particle types if necessary)
double findTotalMomentum(ND::TTruthVerticesModule::TTruthVertex *parent_vertex,
                         TTruthUtils trutil, int pdg) {
  double total_momentum = 0;
  for (int i = 0; i < parent_vertex->NPrimaryTraj; i++) {
    ND::TTruthTrajectoriesModule::TTruthTrajectory *primary_traj =
        trutil.GetTrajectoryById(parent_vertex->PrimaryTrajIDs[i]);
    if (!primary_traj)
      continue;

    if ((pdg != 0) && (primary_traj->PDG != pdg))
      continue;
    if (primary_traj->Charge != 0)
      total_momentum += primary_traj->InitMomentum.Vect().Mag();
  }
  return total_momentum;
}

//=============================================================================================
// Function to evaluate vertex completeness
double evaluateVertexCompleteness(
    std::vector<ND::TTRExReconModule::TTPCAnaPath *> paths,
    ND::TTruthVerticesModule::TTruthVertex *parent_vertex, TTruthUtils trutil) {
  std::vector<int> true_charged_primary_IDs;
  for (int i = 0; i < parent_vertex->NPrimaryTraj; i++) {
    ND::TTruthTrajectoriesModule::TTruthTrajectory *primary_traj =
        trutil.GetTrajectoryById(parent_vertex->PrimaryTrajIDs[i]);
    if (!primary_traj)
      continue;
    if (primary_traj->Charge != 0)
      true_charged_primary_IDs.push_back(primary_traj->ID);
  }

  std::set<int> reconstructed_traj_ids;
  for (unsigned int i = 0; i < paths.size(); i++) {
    for (unsigned int j = 0; j < true_charged_primary_IDs.size(); j++) {
      if (paths[i]->G4ID == true_charged_primary_IDs[j]) {
        int id = true_charged_primary_IDs[j];
        reconstructed_traj_ids.insert(id);
      }
    }
  }

  double n_traj_reconstructed = reconstructed_traj_ids.size();
  double n_traj_in_truth = true_charged_primary_IDs.size();

  if (n_traj_reconstructed > n_traj_in_truth) {
    std::cout << "Completeness > 1" << std::endl;
    std::cout << "Reconstructed trajectories match to true IDs:" << std::endl;
    for (std::set<int>::iterator listIt = reconstructed_traj_ids.begin();
         listIt != reconstructed_traj_ids.end(); listIt++) {
      std::cout << *listIt << std::endl;
    }
    std::cout << "True IDs associated to true vertex:" << std::endl;
    for (unsigned int i = 0; i < true_charged_primary_IDs.size(); i++) {
      std::cout << true_charged_primary_IDs[i] << std::endl;
    }
    std::cout << "-------------------------------------------" << std::endl;
  }

  return n_traj_reconstructed / n_traj_in_truth;
}

//=============================================================================================
// Function to evaluate vertex cleanliness
double evaluateVertexCleanliness(
    std::vector<ND::TTRExReconModule::TTPCAnaPath *> paths,
    ND::TTruthVerticesModule::TTruthVertex *parent_vertex, TTruthUtils trutil) {
  std::vector<int> true_charged_primary_IDs;
  for (int i = 0; i < parent_vertex->NPrimaryTraj; i++) {
    ND::TTruthTrajectoriesModule::TTruthTrajectory *primary_traj =
        trutil.GetTrajectoryById(parent_vertex->PrimaryTrajIDs[i]);
    if (!primary_traj)
      continue;
    if (primary_traj->Charge != 0)
      true_charged_primary_IDs.push_back(primary_traj->ID);
  }

  int n_matched_paths = 0;
  int n_unmatched_paths = 0;
  for (unsigned int i = 0; i < paths.size(); i++) {
    bool matched = false;
    for (unsigned int j = 0; j < true_charged_primary_IDs.size(); j++) {
      if (paths[i]->G4ID == true_charged_primary_IDs[j]) {
        matched = true;
        continue;
      }
    }
    if (matched)
      n_matched_paths++;
    else
      n_unmatched_paths++;
  }

  double cleanliness = (double)n_matched_paths /
                       ((double)n_matched_paths + (double)n_unmatched_paths);
  return cleanliness;
}

//=============================================================================================
// Function to get the ends of a path
std::vector<PathEnd> getPathEnds(
    ND::TTRExReconModule::TTPCAnaPath *muon_path,
    std::vector<ND::TTRExReconModule::TTPCAnaJunction *> connected_junctions) {
  std::vector<PathEnd> path_ends;
  for (unsigned int j = 0; j < connected_junctions.size(); j++) {
    PathEnd juncpoint;
    juncpoint.Position = connected_junctions[j]->Position.Vect();
    juncpoint.Path = muon_path;
    juncpoint.Junction = connected_junctions[j];
    path_ends.push_back(juncpoint);
  }
  if (connected_junctions.size() == 0) {
    ND::TTRExReconModule::TTPCAnaJunction *empty_junc = NULL;

    PathEnd firstpoint;
    firstpoint.Position = muon_path->FirstPosition;
    firstpoint.Path = muon_path;
    firstpoint.Junction = empty_junc;

    PathEnd lastpoint;
    lastpoint.Position = muon_path->LastPosition;
    lastpoint.Path = muon_path;
    lastpoint.Junction = empty_junc;

    path_ends.push_back(firstpoint);
    path_ends.push_back(lastpoint);
  }
  if (connected_junctions.size() == 1) {
    ND::TTRExReconModule::TTPCAnaJunction *empty_junc = NULL;
    PathEnd freepoint;

    double firstpos_diff =
        (muon_path->FirstPosition - connected_junctions[0]->Position.Vect())
            .Mag();
    double lastpos_diff =
        (muon_path->LastPosition - connected_junctions[0]->Position.Vect())
            .Mag();
    if (firstpos_diff < lastpos_diff) {
      freepoint.Position = muon_path->LastPosition;
      freepoint.Path = muon_path;
      freepoint.Junction = empty_junc;
    } else {
      freepoint.Position = muon_path->FirstPosition;
      freepoint.Path = muon_path;
      freepoint.Junction = empty_junc;
    }
    path_ends.push_back(freepoint);
  }
  return path_ends;
}

//=============================================================================================
// Function to trace muon paths back across junctions from secondary
// interactions
// Returns ALL paths with muon PIDs that connect to the original muon path, and
// the junctions that connect to them.
// N.B. Charge ordering assumes junctions are vertices => charge of the
// connected paths may well have been flipped.
// Therefore no charge requirements are imposed.
std::vector<std::pair<ND::TTRExReconModule::TTPCAnaPath *,
                      std::vector<ND::TTRExReconModule::TTPCAnaJunction *> > >
leapFrog(
    ND::TTRExReconModule::TTPCAnaPath *muon_path,
    std::vector<ND::TTRExReconModule::TTPCAnaJunction *> connected_junctions,
    ND::TTRExReconModule::TTPCAnaPattern *parent_pattern) {
  std::vector<std::pair<ND::TTRExReconModule::TTPCAnaPath *,
                        std::vector<ND::TTRExReconModule::TTPCAnaJunction *> > >
      connected_muon_paths;
  TClonesArray *Junctions = parent_pattern->Junctions;
  TClonesArray *Paths = parent_pattern->Paths;

  for (unsigned int j = 0; j < connected_junctions.size(); j++) {
    for (int k = 0; k < parent_pattern->NPaths; k++) {
      ND::TTRExReconModule::TTPCAnaPath *pattern_path =
          (ND::TTRExReconModule::TTPCAnaPath *)Paths->At(k);
      for (int l = 0; l < connected_junctions[j]->NPaths; l++) {
        if ((connected_junctions[j]->PathIDs[l] == pattern_path->PathID) &&
            (pattern_path->PathID != muon_path->PathID)) {
          if (isMuon(pattern_path->MuonPull, pattern_path->PionPull,
                     pattern_path->ProtonPull, pattern_path->ElePull,
                     pattern_path->Momentum)) {
            // Have found a connected path with a muon PID

            // Get new connected junctions!
            std::vector<ND::TTRExReconModule::TTPCAnaJunction *>
                new_connected_junctions;
            for (int m = 0; m < parent_pattern->NJunctions; m++) {
              ND::TTRExReconModule::TTPCAnaJunction *pattern_junction =
                  (ND::TTRExReconModule::TTPCAnaJunction *)Junctions->At(m);
              for (int n = 0; n < pattern_junction->NPaths; n++) {
                if (pattern_path->PathID == pattern_junction->PathIDs[n]) {
                  for (unsigned int o = 0; o < connected_junctions.size();
                       o++) {
                    if (pattern_junction->JunctionID !=
                        connected_junctions[o]->JunctionID)
                      new_connected_junctions.push_back(pattern_junction);
                  }
                }
              }
            }

            // All done with this connected path
            connected_muon_paths.push_back(
                std::pair<ND::TTRExReconModule::TTPCAnaPath *,
                          std::vector<ND::TTRExReconModule::TTPCAnaJunction *> >(
                    pattern_path, new_connected_junctions));
          }
        }
      }
    }
  }

  return connected_muon_paths;
}

//=============================================================================================
// Function to evaluate final vertex cuts
int evaluateFinalCuts(PathEnd end,
                      ND::TTRExReconModule::TTPCAnaPattern *parent_pattern,
                      TClonesArray *Patterns, int NPatterns, TVector3 fv_maxima,
                      TVector3 fv_minima) {
  int cutvalue = 4;

  // Fiducial volume
  TVector3 fv_maxlimits = fv_maxima;
  TVector3 fv_minlimits = fv_minima;
  if (!(end.Junction)) {
    fv_maxlimits.SetX(fv_maxima.X() - 50.0);
    fv_minlimits.SetX(fv_minima.X() + 50.0);
  }
  if (!isInFV(fv_maxlimits, fv_minlimits, end.Position))
    return cutvalue;

  if (!(end.Junction)) {
    double gap_maxima[3] = {-395.0, 974.0, 2333.0};
    double gap_minima[3] = {-412.0, 948.0, 2307.0};
    for (int i = 0; i < 3; i++) {
      if ((end.Position.Z() < gap_maxima[i] + 30.0) &&
          (end.Position.Z() > gap_minima[i] - 30.0))
        return cutvalue;
    }
  }

  cutvalue = 5;

  // Delta veto
  if (isDelta(end.Junction, parent_pattern))
    return cutvalue;

  cutvalue = 6;

  // Veto broken tracks
  if (isBroken(end.Junction, parent_pattern, Patterns, NPatterns, fv_maxima,
               fv_minima))
    return cutvalue;

  cutvalue = 7;
  return cutvalue;
}

//=============================================================================================
// Function to save information about the vertex after the selection has been
// made
void fillVertexInfo(
    VertexCandidate &vert_cand,
    ND::TTRExReconModule::TTPCAnaPattern *parent_pattern, PathEnd selected_end,
    std::map<int, std::vector<ND::TTRExReconModule::TTPCAnaPattern *> >
        pattern_truth,
    TTruthUtils trutil, TVector3 fv_maxima, TVector3 fv_minima, int tpc,
    bool isMC, Int_t N_RooTrackerVertices, TClonesArray *RooTrackerVertices,
    bool is_it_neut, bool hasTrackerVertices) {
  TClonesArray *PatternPaths = parent_pattern->Paths;

  vert_cand.Time = -9999.9;
  if (selected_end.Path)
    vert_cand.Time = selected_end.Path->Time;

  if (selected_end.Junction) // Selected point was a junction with multiple
                             // paths emerging
  {
    // Get all paths associated to junction
    std::vector<ND::TTRExReconModule::TTPCAnaPath *> junc_paths;
    for (int i = 0; i < parent_pattern->NPaths; i++) {
      ND::TTRExReconModule::TTPCAnaPath *pattern_path =
          (ND::TTRExReconModule::TTPCAnaPath *)PatternPaths->At(i);
      for (int j = 0; j < selected_end.Junction->NPaths; j++) {
        if (pattern_path->PathID == selected_end.Junction->PathIDs[j])
          junc_paths.push_back(pattern_path);
      }
    }

    // Examine paths, count fits and protons
    int n_fits = 0;
    int n_protons = 0;
    for (unsigned int i = 0; i < junc_paths.size(); i++) {
      if ((junc_paths[i]->LikFit) && (junc_paths[i]->Success)) {
        n_fits++;
        if (isProton(junc_paths[i]->MuonPull, junc_paths[i]->PionPull,
                     junc_paths[i]->ProtonPull, junc_paths[i]->ElePull,
                     junc_paths[i]->Momentum))
          n_protons++;
      }
    }

    // Reco information
    //****************
    vert_cand.NTracks = selected_end.Junction->NPaths;
    vert_cand.NFittedTracks = n_fits;
    vert_cand.NProtonTracks = n_protons;
    vert_cand.Position = selected_end.Position;

    if (isMC) {
      // Truth matching information
      //**************************
      ND::TTruthVerticesModule::TTruthVertex *parent_vertex =
          trutil.GetVertexById(getTrueVertexFromJunction(junc_paths, trutil));
      std::vector<ND::TTRExReconModule::TTPCAnaPattern *>
          other_matched_patterns = pattern_truth.at(parent_vertex->ID);

      bool is_genie = false;
      if (!is_it_neut)
        is_genie = true;

      vert_cand.MatchedTruthCode = evaluateJunctionTruth(
          selected_end.Junction->Position.Vect(), junc_paths,
          other_matched_patterns, trutil, fv_maxima, fv_minima, tpc, is_genie);
      vert_cand.MatchedPosition = parent_vertex->Position.Vect();
      vert_cand.MatchedSubdetector = parent_vertex->Subdetector;
      vert_cand.MatchedTargetZ =
          ((parent_vertex->TargetPDG) % 1000000000) / 10000;
      if (hasTrackerVertices)
        vert_cand.MatchedNProtons =
            countChargedTraj(parent_vertex, 2212, N_RooTrackerVertices,
                             RooTrackerVertices, is_it_neut);
      vert_cand.MatchedNeutrinoMomentum =
          parent_vertex->NeutrinoMomentum.Vect().Mag();
      vert_cand.MatchedMuonMomentum =
          findTotalMomentum(parent_vertex, trutil, 13);
      vert_cand.MatchedTotalMomentum =
          findTotalMomentum(parent_vertex, trutil, 0);

      // Recon performance information
      //*****************************
      vert_cand.VertexCleanliness =
          evaluateVertexCleanliness(junc_paths, parent_vertex, trutil);
      vert_cand.VertexCompleteness =
          evaluateVertexCompleteness(junc_paths, parent_vertex, trutil);
    }

    // Cut development information
    //***************************
    TVector3 max_coords = selected_end.Junction->MaximumCoordinates;
    TVector3 min_coords = selected_end.Junction->MinimumCoordinates;

    double x_extent = std::abs(max_coords.X() - min_coords.X());
    double y_extent = std::abs(max_coords.Y() - min_coords.Y());
    double z_extent = std::abs(max_coords.Z() - min_coords.Z());
    TVector3 vertex_extent;
    vertex_extent.SetX(x_extent);
    vertex_extent.SetY(y_extent);
    vertex_extent.SetZ(z_extent);

    vert_cand.VertexExtent = vertex_extent;

    double total_reco_momentum = 0;
    double lowest_reco_momentum = -9999.9;
    double hmnt_momentum = -9999.9;
    int hmnt_index = -1;
    for (unsigned int i = 0; i < junc_paths.size(); i++) {
      if ((junc_paths[i]->LikFit) && (junc_paths[i]->Success)) {
        total_reco_momentum += junc_paths[i]->Momentum;

        if (junc_paths[i]->Momentum < std::abs(lowest_reco_momentum))
          lowest_reco_momentum = junc_paths[i]->Momentum;

        double firstdiff = (junc_paths[i]->FirstPosition -
                            selected_end.Junction->Position.Vect())
                               .Mag();
        double seconddiff = (junc_paths[i]->LastPosition -
                             selected_end.Junction->Position.Vect())
                                .Mag();
        double charge = junc_paths[i]->Charge;
        if (seconddiff < firstdiff)
          charge = charge * (-1.0);

        if (charge < 0) {
          if ((junc_paths[i]->Momentum > hmnt_momentum) &&
              (isMuon(junc_paths[i]->MuonPull, junc_paths[i]->PionPull,
                      junc_paths[i]->ProtonPull, junc_paths[i]->ElePull,
                      junc_paths[i]->Momentum))) {
            hmnt_momentum = junc_paths[i]->Momentum;
            hmnt_index = i;
          }
        }
      }
    }
    vert_cand.TotalRecoMomentum = total_reco_momentum;
    vert_cand.LowestRecoMomentum = lowest_reco_momentum;
    vert_cand.HMNTMomentum = hmnt_momentum;

    if (isMC) {
      if (hmnt_index != -1) {
        ND::TTruthTrajectoriesModule::TTruthTrajectory *hmnt_true_traj =
            trutil.GetTrajectoryById(junc_paths[hmnt_index]->G4ID);
        if (!hmnt_true_traj) {
          std::cout << " WARNING: no true trajectory found for HMNT"
                    << std::endl;
          vert_cand.HMNTIsMuon = false;
        }
        if (hmnt_true_traj->PDG == 13)
          vert_cand.HMNTIsMuon = true;
        else
          vert_cand.HMNTIsMuon = false;
      } else {
        vert_cand.HMNTIsMuon = false;
      }
    }

    vert_cand.MuMomDiff = 9999.9;
    vert_cand.LowestAngle = 90.0;
    vert_cand.NAssociatedPaths = 0;

    for (unsigned int i = 0; i < junc_paths.size(); i++) {
      TVector3 firstpos = junc_paths[i]->FirstPosition;
      TVector3 lastpos = junc_paths[i]->LastPosition;
      TVector3 juncpos = selected_end.Junction->Position.Vect();

      double diff1 = (juncpos - lastpos).Mag();
      double diff2 = (juncpos - firstpos).Mag();
      double mindiff = std::min(diff1, diff2);
      if (mindiff < 100.0)
        vert_cand.NAssociatedPaths++;

      for (unsigned int j = 0; j < junc_paths.size(); j++) {
        if (i == j)
          continue;

        bool firstpath_muon =
            isMuon(junc_paths[i]->MuonPull, junc_paths[i]->PionPull,
                   junc_paths[i]->ProtonPull, junc_paths[i]->ElePull,
                   junc_paths[i]->Momentum);
        bool secondpath_muon =
            isMuon(junc_paths[j]->MuonPull, junc_paths[j]->PionPull,
                   junc_paths[j]->ProtonPull, junc_paths[j]->ElePull,
                   junc_paths[j]->Momentum);

        if ((firstpath_muon) && (secondpath_muon)) {
          double diff =
              std::abs(junc_paths[i]->Momentum - junc_paths[j]->Momentum);
          if (diff < vert_cand.MuMomDiff)
            vert_cand.MuMomDiff = diff;
        }

        double angle = std::acos(
            std::abs(junc_paths[i]->Direction.Dot(junc_paths[j]->Direction)));
        double angle_degrees = angle * (180.0 / 3.14159265359);
        if (angle_degrees < vert_cand.LowestAngle)
          vert_cand.LowestAngle = angle_degrees;
      }
    }
  } else // Selected point was associated to a single path
  {
    // Reco information
    //****************
    // Assuming here that a muon candidate has been identified, we already know
    // the number of fits (1) and the number of protons (0)
    vert_cand.NTracks = 1;
    vert_cand.NFittedTracks = 1;
    vert_cand.NProtonTracks = 0;
    vert_cand.Position = selected_end.Position;

    if (isMC) {
      // Truth matching information
      //**************************
      ND::TTruthVerticesModule::TTruthVertex *parent_vertex = NULL;
      parent_vertex = trutil.GetVertexOfTrajectory(selected_end.Path->G4ID);
      std::vector<ND::TTRExReconModule::TTPCAnaPattern *>
          other_matched_patterns = pattern_truth.at(parent_vertex->ID);

      bool is_genie = false;
      if (!is_it_neut)
        is_genie = true;

      vert_cand.MatchedTruthCode =
          evaluatePathTruth(selected_end.Path, other_matched_patterns, trutil,
                            fv_maxima, fv_minima, tpc, is_genie);
      vert_cand.MatchedPosition = parent_vertex->Position.Vect();
      vert_cand.MatchedSubdetector = parent_vertex->Subdetector;
      vert_cand.MatchedTargetZ =
          ((parent_vertex->TargetPDG) % 1000000000) / 10000;
      if (hasTrackerVertices)
        vert_cand.MatchedNProtons =
            countChargedTraj(parent_vertex, 2212, N_RooTrackerVertices,
                             RooTrackerVertices, is_it_neut);
      vert_cand.MatchedNeutrinoMomentum =
          parent_vertex->NeutrinoMomentum.Vect().Mag();
      vert_cand.MatchedMuonMomentum =
          findTotalMomentum(parent_vertex, trutil, 13);
      vert_cand.MatchedTotalMomentum =
          findTotalMomentum(parent_vertex, trutil, 0);

      // Recon performance information
      //*****************************
      std::vector<ND::TTRExReconModule::TTPCAnaPath *> path_vec;
      path_vec.push_back(selected_end.Path);
      vert_cand.VertexCleanliness =
          evaluateVertexCleanliness(path_vec, parent_vertex, trutil);
      vert_cand.VertexCompleteness =
          evaluateVertexCompleteness(path_vec, parent_vertex, trutil);
    }

    // Cut development information
    //***************************
    TVector3 no_extent(0.0, 0.0, 0.0);
    vert_cand.VertexExtent = no_extent;

    vert_cand.TotalRecoMomentum = selected_end.Path->Momentum;
    vert_cand.LowestRecoMomentum = selected_end.Path->Momentum;
    vert_cand.HMNTMomentum = selected_end.Path->Momentum;
    if (isMC) {
      ND::TTruthTrajectoriesModule::TTruthTrajectory *true_traj =
          trutil.GetTrajectoryById(selected_end.Path->G4ID);
      if (true_traj->PDG == 13)
        vert_cand.HMNTIsMuon = true;
      else
        vert_cand.HMNTIsMuon = false;
    }

    vert_cand.MuMomDiff = 9999.9;
    vert_cand.LowestAngle = 90.0;
    vert_cand.NAssociatedPaths = 0;
  }
}

//=============================================================================================
// Main code
void Stage2GasSelection(char *inputfilename, char *outputfilename,
                        int max_entries) {
  // _________________________________________________________________________
  // PREPARE TO READ TREES
  //

  // Add input files to TChains
  TChain tRExTreeChain("ReconDir/TREx");
  TChain headerChain("HeaderDir/BasicHeader");
  TChain dataQualityChain("HeaderDir/BasicDataQuality");

  std::cout << "Adding file(s) matching string: " << inputfilename << std::endl;
  std::cout << "Added " << tRExTreeChain.Add(inputfilename) << " file(s)"
            << std::endl;
  dataQualityChain.Add(inputfilename);
  headerChain.Add(inputfilename);

  // Initialise variables to be read in
  Int_t NPatterns;
  TClonesArray *Patterns =
      new TClonesArray("ND::TTRExReconModule::TTPCAnaPattern", 100);

  Bool_t IsMC;
  Int_t RunID;
  Int_t SubrunID;
  Int_t EventID;
  Int_t TPC1Flag;
  Int_t TPC2Flag;
  Int_t TPC3Flag;

  // Set Branch Addresses
  tRExTreeChain.SetBranchAddress("NPatterns", &NPatterns);
  tRExTreeChain.SetBranchAddress("Patterns", &Patterns);

  headerChain.SetBranchAddress("IsMC", &IsMC);
  headerChain.SetBranchAddress("RunID", &RunID);
  headerChain.SetBranchAddress("SubrunID", &SubrunID);
  headerChain.SetBranchAddress("EventID", &EventID);

  dataQualityChain.SetBranchAddress("TPC1Flag", &TPC1Flag);
  dataQualityChain.SetBranchAddress("TPC2Flag", &TPC2Flag);
  dataQualityChain.SetBranchAddress("TPC3Flag", &TPC3Flag);

  // Do the same for truth trees
  TChain trueVertexChain("TruthDir/Vertices");
  TChain trueTrajChain("TruthDir/Trajectories");
  TChain nRooTrackerChain("TruthDir/NRooTrackerVtx");
  TChain gRooTrackerChain("TruthDir/GRooTrackerVtx");
  TChain *rooTrackerChain;

  Int_t NVtx;
  TClonesArray *TrueVertices =
      new TClonesArray("ND::TTruthVerticesModule::TTruthVertex", 100);
  Int_t NTraj;
  TClonesArray *TrueTrajectories =
      new TClonesArray("ND::TTruthTrajectoriesModule::TTruthTrajectory", 100);

  Int_t N_RooTrackerVertices;
  TClonesArray *NRooTrackerVertices = new TClonesArray("ND::NRooTrackerVtx");
  TClonesArray *GRooTrackerVertices = new TClonesArray("ND::GRooTrackerVtx");
  TClonesArray *RooTrackerVertices = NULL;

  bool is_it_neut = false;
  bool is_it_genie = false;

  std::stringstream ss;
  std::string input_name;
  ss << inputfilename;
  ss >> input_name;

  if (input_name.find("oa_nt_") != std::string::npos)
    is_it_neut = true;
  if (is_it_neut)
    std::cout << "Definitely NEUT!" << std::endl;
  if (input_name.find("oa_gn_") != std::string::npos)
    is_it_genie = true;
  if (is_it_genie && is_it_neut)
    std::cout << "WARNING: filename parsing failed, is it GENIE or NEUT?"
              << std::endl;

  headerChain.GetEntry(0);
  bool hasTrackerVertices = true;
  if (IsMC) {
    trueVertexChain.Add(inputfilename);
    trueTrajChain.Add(inputfilename);

    trueVertexChain.SetBranchAddress("NVtx", &NVtx);
    trueVertexChain.SetBranchAddress("Vertices", &TrueVertices);

    trueTrajChain.SetBranchAddress("NTraj", &NTraj);
    trueTrajChain.SetBranchAddress("Trajectories", &TrueTrajectories);

    if (is_it_neut) {
      std::cout << "Checking for nRooTracker tree" << std::endl;
      nRooTrackerChain.Add(inputfilename);
      if (nRooTrackerChain.GetListOfBranches() != 0) {
        rooTrackerChain = (TChain *)nRooTrackerChain.Clone();
        RooTrackerVertices = NRooTrackerVertices;
        std::cout << "Found nRooTrackerTree" << std::endl;
      }
    }
    if (is_it_genie) {
      std::cout << "Checking for gRooTracker tree" << std::endl;
      gRooTrackerChain.Add(inputfilename);
      if (gRooTrackerChain.GetListOfBranches() != 0) {
        rooTrackerChain = (TChain *)gRooTrackerChain.Clone();
        RooTrackerVertices = GRooTrackerVertices;
        std::cout << "Found gRooTrackerTree" << std::endl;
      }
    }

    if ((nRooTrackerChain.GetListOfBranches() == 0) &&
        (gRooTrackerChain.GetListOfBranches() == 0)) {
      std::cout << "         "
                   "-----------------------------------------------------------"
                   "----------------------------------------"
                << std::endl;
      std::cout << "         * Found neither nRooTracker or gRooTracker "
                   "information in MC file.                                *"
                << std::endl;
      std::cout << "         * RooTracker information is required for an "
                   "accurate true proton count.                           *"
                << std::endl;
      std::cout << "         *                                                 "
                   "                                                *"
                << std::endl;
      std::cout << "         * NOTE: if you think the files DO have RooTracker "
                   "information and used a wildcard input, make     *"
                << std::endl;
      std::cout << "         * sure you included enough of the filename for "
                   "the code to recognise the generator prefix.        *"
                << std::endl;
      std::cout << "         "
                   "-----------------------------------------------------------"
                   "----------------------------------------"
                << std::endl;
      hasTrackerVertices = false;
      // return;
    }

    if (hasTrackerVertices) {
      rooTrackerChain->SetBranchAddress("NVtx", &N_RooTrackerVertices);
      rooTrackerChain->SetBranchAddress("Vtx", &RooTrackerVertices);
      gRooTrackerChain.SetBranchAddress("NVtx", &N_RooTrackerVertices);
      gRooTrackerChain.SetBranchAddress("Vtx", &RooTrackerVertices);
      nRooTrackerChain.SetBranchAddress("NVtx", &N_RooTrackerVertices);
      nRooTrackerChain.SetBranchAddress("Vtx", &RooTrackerVertices);
    }
  }
  std::cout << "Setup done" << std::endl;

  //__________________________________________________________________________
  // CREATE OUTPUT
  //

  ofstream skimlist;
  skimlist.open("skimlist.list");

  // ROOT output file
  TFile *outputfile = new TFile(outputfilename, "RECREATE");
  outputfile->cd();

  // Reco Tree
  //-------------------------
  // Information about selected object
  int CutLevel;
  int NTracks;
  int NFittedTracks;
  int NProtonTracks;
  double Position_X;
  double Position_Y;
  double Position_Z;
  double Time;
  // Information about truth-matched interaction
  int MatchedTruthCode;
  double MatchedPosition_X;
  double MatchedPosition_Y;
  double MatchedPosition_Z;
  int MatchedSubdetector;
  int MatchedTargetZ;
  int MatchedNProtons;
  double MatchedNeutrinoMomentum;
  double MatchedMuonMomentum;
  double MatchedTotalMomentum;
  // Information about vertexing performance
  double VertexCleanliness;
  double VertexCompleteness;
  // Information used for cut development
  double VertexExtent_X;
  double VertexExtent_Y;
  double VertexExtent_Z;
  double TotalRecoMomentum;
  double LowestRecoMomentum;
  double HMNTMomentum;
  bool HMNTIsMuon;
  double MuMomDiff;
  double LowestAngle;
  int NAssociatedPaths;

  TTree *tpc1 = new TTree("tpc1", "TPC1");
  TTree *tpc2 = new TTree("tpc2", "TPC2");
  TTree *tpc3 = new TTree("tpc3", "TPC3");
  std::vector<TTree *> treevec;
  treevec.push_back(tpc1);
  treevec.push_back(tpc2);
  treevec.push_back(tpc3);

  for (std::vector<TTree *>::iterator treeIt = treevec.begin();
       treeIt != treevec.end(); treeIt++) {
    TTree *outputTree = *treeIt;

    outputTree->SetDirectory(0);
    // Event Information
    outputTree->Branch("RunID", &RunID, "RunID/i");
    outputTree->Branch("SubRunID", &SubrunID, "SubRunID/i");
    outputTree->Branch("EventID", &EventID, "EventID/i");
    outputTree->Branch("IsMC", &IsMC, "IsMC/B");
    // Information about selected object
    outputTree->Branch("CutLevel", &CutLevel, "CutLevel/i");
    outputTree->Branch("NTracks", &NTracks, "NTracks/i");
    outputTree->Branch("NFittedTracks", &NFittedTracks, "NFittedTracks/i");
    outputTree->Branch("NProtonTracks", &NProtonTracks, "NProtonTracks/i");
    outputTree->Branch("Position_X", &Position_X, "Position_X/D");
    outputTree->Branch("Position_Y", &Position_Y, "Position_Y/D");
    outputTree->Branch("Position_Z", &Position_Z, "Position_Z/D");
    outputTree->Branch("Time", &Time, "Time/D");
    // Information about true interaction
    outputTree->Branch("MatchedTruthCode", &MatchedTruthCode,
                       "MatchedTruthCode/I");
    outputTree->Branch("MatchedPosition_X", &MatchedPosition_X,
                       "MatchedPosition_X/D");
    outputTree->Branch("MatchedPosition_Y", &MatchedPosition_Y,
                       "MatchedPosition_Y/D");
    outputTree->Branch("MatchedPosition_Z", &MatchedPosition_Z,
                       "MatchedPosition_Z/D");
    outputTree->Branch("MatchedSubdetector", &MatchedSubdetector,
                       "MatchedSubdetector/I");
    outputTree->Branch("MatchedTargetZ", &MatchedTargetZ, "MatchedTargetZ/I");
    outputTree->Branch("MatchedNProtons", &MatchedNProtons,
                       "MatchedNProtons/I");
    outputTree->Branch("MatchedNeutrinoMomentum", &MatchedNeutrinoMomentum,
                       "MatchedNeutrinoMomentum/D");
    outputTree->Branch("MatchedMuonMomentum", &MatchedMuonMomentum,
                       "MatchedMuonMomentum/D");
    outputTree->Branch("MatchedTotalMomentum", &MatchedTotalMomentum,
                       "MatchedTotalMomentum/D");
    // Information about vertexing performance
    outputTree->Branch("VertexCleanliness", &VertexCleanliness,
                       "VertexCleanliness/D");
    outputTree->Branch("VertexCompleteness", &VertexCompleteness,
                       "VertexCompleteness/D");
    // Information for use in cut development
    outputTree->Branch("VertexExtent_X", &VertexExtent_X, "VertexExtent_X/D");
    outputTree->Branch("VertexExtent_Y", &VertexExtent_Y, "VertexExtent_Y/D");
    outputTree->Branch("VertexExtent_Z", &VertexExtent_Z, "VertexExtent_Z/D");
    outputTree->Branch("TotalRecoMomentum", &TotalRecoMomentum,
                       "TotalRecoMomentum/D");
    outputTree->Branch("LowestRecoMomentum", &LowestRecoMomentum,
                       "LowestRecoMomentum/D");
    outputTree->Branch("HMNTMomentum", &HMNTMomentum, "HMNTMomentum/D");
    outputTree->Branch("HMNTIsMuon", &HMNTIsMuon, "HMNTIsMuon/B");
    outputTree->Branch("NPatterns", &NPatterns, "NPatterns/I");
    outputTree->Branch("MuMomDiff", &MuMomDiff, "MuMomDiff/D");
    outputTree->Branch("LowestAngle", &LowestAngle, "LowestAngle/D");
    outputTree->Branch("NAssociatedPaths", &NAssociatedPaths,
                       "NAssociatedPaths/i");
  }

  // Truth Tree
  //--------------------
  int MatchedCutLevel;
  int VertexTruthCode;
  int NReconstructedProtons;
  int NTrueProtons;
  double NeutrinoEnergy;
  double QSquared;
  double MuonMomentum;
  double MuonAngle;
  int TargetZ;
  int ReactionCode;
  int NeutrinoPDG;
  ND::NRooTrackerVtx *NRTVtx = NULL;
  ND::GRooTrackerVtx *GRTVtx = NULL;

  TTree *tpc1truth = new TTree("tpc1truth", "TPC1truth");
  TTree *tpc2truth = new TTree("tpc2truth", "TPC2truth");
  TTree *tpc3truth = new TTree("tpc3truth", "TPC3truth");
  std::vector<TTree *> truthtreevec;
  truthtreevec.push_back(tpc1truth);
  truthtreevec.push_back(tpc2truth);
  truthtreevec.push_back(tpc3truth);

  for (std::vector<TTree *>::iterator treeIt = truthtreevec.begin();
       treeIt != truthtreevec.end(); treeIt++) {
    TTree *outputTruthTree = *treeIt;

    outputTruthTree->Branch("RunID", &RunID, "RunID/i");
    outputTruthTree->Branch("SubRunID", &SubrunID, "SubRunID/i");
    outputTruthTree->Branch("EventID", &EventID, "EventID/i");
    outputTruthTree->Branch("MatchedCutLevel", &MatchedCutLevel,
                            "MatchedCutLevel/i");
    outputTruthTree->Branch("VertexTruthCode", &VertexTruthCode,
                            "VertexTruthCode/i");
    outputTruthTree->Branch("NReconstructedProtons", &NReconstructedProtons,
                            "NReconstructedProtons/i");
    outputTruthTree->Branch("NTrueProtons", &NTrueProtons, "NTrueProtons/i");
    outputTruthTree->Branch("NeutrinoEnergy", &NeutrinoEnergy,
                            "NeutrinoEnergy/d");
    outputTruthTree->Branch("QSquared", &QSquared, "QSquared/D");
    outputTruthTree->Branch("MuonMomentum", &MuonMomentum, "MuonMomentum/D");
    outputTruthTree->Branch("MuonAngle", &MuonAngle, "MuonAngle/D");
    outputTruthTree->Branch("TargetZ", &TargetZ, "TargetZ/I");
    outputTruthTree->Branch("ReactionCode", &ReactionCode, "ReactionCode/I");
    outputTruthTree->Branch("NeutrinoPDG", &NeutrinoPDG, "NeutrinoPDG/I");
    // Information for use in cut development
    outputTruthTree->Branch("VertexExtent_X", &VertexExtent_X,
                            "VertexExtent_X/D");
    outputTruthTree->Branch("VertexExtent_Y", &VertexExtent_Y,
                            "VertexExtent_Y/D");
    outputTruthTree->Branch("VertexExtent_Z", &VertexExtent_Z,
                            "VertexExtent_Z/D");
    outputTruthTree->Branch("TotalRecoMomentum", &TotalRecoMomentum,
                            "TotalRecoMomentum/D");
    outputTruthTree->Branch("LowestRecoMomentum", &LowestRecoMomentum,
                            "LowestRecoMomentum/D");
    outputTruthTree->Branch("HMNTMomentum", &HMNTMomentum, "HMNTMomentum/D");
    outputTruthTree->Branch("HMNTIsMuon", &HMNTIsMuon, "HMNTIsMuon/B");
    outputTruthTree->Branch("NPatterns", &NPatterns, "NPatterns/I");
    outputTruthTree->Branch("MuMomDiff", &MuMomDiff, "MuMomDiff/D");
    outputTruthTree->Branch("LowestAngle", &LowestAngle, "LowestAngle/D");
    outputTruthTree->Branch("NTracks", &NTracks, "NTracks/i");
    outputTruthTree->Branch("NAssociatedPaths", &NAssociatedPaths,
                            "NAssociatedPaths/i");
    // Passthrough information
    if (hasTrackerVertices) {
      if (is_it_neut)
        outputTruthTree->Branch("NRTVtx", "ND::NRooTrackerVtx", &NRTVtx, 128000,
                                1);
      // if (is_it_genie) outputTruthTree->Branch("GRTVtx",
      // "ND::GRooTrackerVtx", &GRTVtx, 128000, 1);
    }
  }
  std::cout << "Trees created" << std::endl;

  //__________________________________________________________________________
  // DEFINE SELECTION PARAMETERS
  //

  // Fiducial volume
  TVector3 tpc1_max(870, 1030, -71.15);
  TVector3 tpc1_min(20, -930, -724.85);

  TVector3 tpc2_max(870, 1030, 1287.85);
  TVector3 tpc2_min(20, -930, 634.15);

  TVector3 tpc3_max(870, 1030, 2646.85);
  TVector3 tpc3_min(20, -930, 1993.55);

  std::vector<TVector3> fv_min;
  std::vector<TVector3> fv_max;

  fv_min.push_back(tpc1_min);
  fv_min.push_back(tpc2_min);
  fv_min.push_back(tpc3_min);

  fv_max.push_back(tpc1_max);
  fv_max.push_back(tpc2_max);
  fv_max.push_back(tpc3_max);

  std::cout << "Fiducial volume set up" << std::endl;

  // _________________________________________________________________________
  // LOOP OVER ENTRIES IN INPUT TREE
  //

  int entries = tRExTreeChain.GetEntries();
  if (max_entries > 0 && max_entries < entries)
    entries = max_entries;
  std::cout << "Looping over " << entries << " events " << endl;

  for (int ient = 0; ient < entries; ient++) {
    if ((ient % 10000 == 0) && (ient != 0))
      std::cout << "Processed " << ient << " events" << std::endl;

    headerChain.GetEntry(ient);
    dataQualityChain.GetEntry(ient);
    if (IsMC) {
      trueVertexChain.GetEntry(ient);
      trueTrajChain.GetEntry(ient);
      // rooTrackerChain->GetEntry(ient);
      if (hasTrackerVertices) {
        if (is_it_neut)
          nRooTrackerChain.GetEntry(ient);
        if (is_it_genie)
          gRooTrackerChain.GetEntry(ient);
      }
    }
    tRExTreeChain.GetEntry(ient);

    TTruthUtils truthy(TrueTrajectories, TrueVertices, &NTraj, &NVtx);

    //------------------------------
    // LOOP OVER TPC TREES
    //------------------------------
    for (int tpc = 1; tpc < 4; tpc++) {
      std::vector<VertexCandidate> vertex_candidates;

      TTree *outputTree = treevec[tpc - 1];
      TTree *outputTruthTree = truthtreevec[tpc - 1];
      TVector3 fv_minima = fv_min[tpc - 1];
      TVector3 fv_maxima = fv_max[tpc - 1];

      //------------------------------------------------------------
      // TRUTH MATCHING: Map association of true vertices to patterns
      //------------------------------------------------------------
      std::map<int, std::vector<ND::TTRExReconModule::TTPCAnaPattern *> >
          pattern_truth;
      if (IsMC) {
        std::vector<int> true_vertex_ids;
        for (int i = 0; i < NVtx; i++) {
          ND::TTruthVerticesModule::TTruthVertex *vertex =
              (ND::TTruthVerticesModule::TTruthVertex *)TrueVertices->At(i);
          int vertex_id = vertex->ID;
          std::vector<ND::TTRExReconModule::TTPCAnaPattern *>
              associated_patterns;

          for (int j = 0; j < NPatterns; j++) {
            ND::TTRExReconModule::TTPCAnaPattern *pattern =
                (ND::TTRExReconModule::TTPCAnaPattern *)Patterns->At(j);
            int matched_vertex_id = patternTrueVertexMatch(pattern, truthy);
            if (matched_vertex_id == vertex_id) {
              associated_patterns.push_back(pattern);
            }
          }

          pattern_truth.insert(
              std::pair<int,
                        std::vector<ND::TTRExReconModule::TTPCAnaPattern *> >(
                  vertex_id, associated_patterns));
        }
      }

      //----------------------------------------------
      // SELECTION: Examine patterns, assign cut level
      //----------------------------------------------

      VertexCandidate BestVertex;
      TVector3 default_vec(-9999.9, -9999.9, -9999.9);
      // Set default values that cannot be mistaken for physics
      BestVertex.NTracks = 0;
      BestVertex.NFittedTracks = 0;
      BestVertex.NProtonTracks = 0;
      BestVertex.Position = default_vec;
      BestVertex.MatchedTruthCode = -1;
      BestVertex.MatchedSubdetector = -1;
      BestVertex.MatchedTargetZ = 0;
      BestVertex.MatchedNProtons = 0;
      BestVertex.MatchedNeutrinoMomentum = -9999.9;
      BestVertex.MatchedMuonMomentum = -9999.9;
      BestVertex.MatchedTotalMomentum = -9999.9;
      BestVertex.MatchedPosition = default_vec;
      BestVertex.VertexCleanliness = -1.0;
      BestVertex.VertexCompleteness = -1.0;
      BestVertex.VertexExtent = default_vec;
      BestVertex.TotalRecoMomentum = -9999.9;
      BestVertex.LowestRecoMomentum = -9999.9;
      BestVertex.HMNTMomentum = -9999.9;
      BestVertex.HMNTIsMuon = false;

      CutLevel = 0;

      // FIRST CUTS: Basic event quality
      //*******************************

      // CUT 1: data quality;
      bool dataquality = true;
      if (!IsMC) {
        if ((tpc == 1) && (TPC1Flag != 0))
          dataquality = false;
        if ((tpc == 2) && (TPC2Flag != 0))
          dataquality = false;
        if ((tpc == 3) && (TPC3Flag != 0))
          dataquality = false;
      }

      // TODO: needs moar flags

      if (dataquality)
        CutLevel = 1;

      // CUT 2: found S1S-passing pattern in TPC
      std::vector<ND::TTRExReconModule::TTPCAnaPattern *> passable_patterns;
      for (int i = 0; i < NPatterns; i++) {
        ND::TTRExReconModule::TTPCAnaPattern *pattern =
            (ND::TTRExReconModule::TTPCAnaPattern *)Patterns->At(i);

        if (pattern->TPC != tpc)
          continue;
        if (pattern->S1Sflag)
          continue;

        passable_patterns.push_back(pattern);
      }

      if ((passable_patterns.size() > 0) && (CutLevel == 1))
        CutLevel = 2;

      // CUT 3: found pattern with T0 in bunch window
      std::vector<ND::TTRExReconModule::TTPCAnaPattern *> eligible_patterns;
      for (unsigned int i = 0; i < passable_patterns.size(); i++) {
        TClonesArray *Paths = passable_patterns[i]->Paths;
        bool found_t0_in_bunch = false;
        for (int j = 0; j < passable_patterns[i]->NPaths; j++) {
          ND::TTRExReconModule::TTPCAnaPath *path =
              (ND::TTRExReconModule::TTPCAnaPath *)Paths->At(j);
          if (path->T0Source != 0) {
            if (isInBunch(RunID, path->Time, IsMC))
              found_t0_in_bunch = true;
          }
        }
        if (!found_t0_in_bunch)
          continue;

        eligible_patterns.push_back(passable_patterns[i]);
      }

      if ((eligible_patterns.size() > 0) && (CutLevel == 2))
        CutLevel = 3;

      // MUON CUTS: Identify muon candidate
      //**********************************

      // Get all paths, from highest to lowest momentum
      std::vector<std::pair<ND::TTRExReconModule::TTPCAnaPath *,
                            ND::TTRExReconModule::TTPCAnaPattern *> >
          eligible_paths;
      for (unsigned int i = 0; i < eligible_patterns.size(); i++) {
        TClonesArray *Paths = eligible_patterns[i]->Paths;
        for (int j = 0; j < eligible_patterns[i]->NPaths; j++) {
          ND::TTRExReconModule::TTPCAnaPath *path =
              (ND::TTRExReconModule::TTPCAnaPath *)Paths->At(j);
          if ((path->LikFit) && (path->Success))
            eligible_paths.push_back(
                std::pair<ND::TTRExReconModule::TTPCAnaPath *,
                          ND::TTRExReconModule::TTPCAnaPattern *>(
                    path, eligible_patterns[i]));
        }
      }
      std::stable_sort(eligible_paths.begin(), eligible_paths.end(),
                       MomentumGreaterThan_V1);

      // Remove positive paths
      std::vector<std::pair<ND::TTRExReconModule::TTPCAnaPath *,
                            ND::TTRExReconModule::TTPCAnaPattern *> >
          eligible_negative_paths;
      for (unsigned int i = 0; i < eligible_paths.size(); i++) {
        double charge = eligible_paths[i].first->Charge;
        ND::TTRExReconModule::TTPCAnaPattern *parent_pattern =
            eligible_paths[i].second;

        if (parent_pattern->NJunctions == 0) // Single path pattern
        {
          double path_firstZ = eligible_paths[i].first->FirstPosition.Z();
          double path_lastZ = eligible_paths[i].first->LastPosition.Z();

          if (path_firstZ > path_lastZ)
            charge = charge * (-1.0);
        }

        if (parent_pattern->NJunctions > 0) // Multi-path pattern
        {
          std::vector<ND::TTRExReconModule::TTPCAnaJunction *>
              connected_junctions;

          TClonesArray *Junctions = parent_pattern->Junctions;
          for (int j = 0; j < parent_pattern->NJunctions; j++) {
            ND::TTRExReconModule::TTPCAnaJunction *junc =
                (ND::TTRExReconModule::TTPCAnaJunction *)Junctions->At(j);
            for (int k = 0; k < junc->NPaths; k++) {
              if (junc->PathIDs[k] == eligible_paths[i].first->PathID)
                connected_junctions.push_back(junc);
            }
          }

          if (connected_junctions.size() == 1) {
            TVector3 path_firstpos = eligible_paths[i].first->FirstPosition;
            TVector3 path_lastpos = eligible_paths[i].first->LastPosition;
            TVector3 juncpos = connected_junctions[0]->Position.Vect();

            TVector3 firstdiff = juncpos - path_firstpos;
            TVector3 lastdiff = juncpos - path_lastpos;
            if (lastdiff.Mag() < firstdiff.Mag())
              charge = charge * (-1.0);
          }

          if (connected_junctions.size() > 1) {
            // TODO: bring in vertex checking
            double path_firstZ = eligible_paths[i].first->FirstPosition.Z();
            double path_lastZ = eligible_paths[i].first->LastPosition.Z();

            if (path_firstZ > path_lastZ)
              charge = charge * (-1.0);
          }
        }

        if (charge < 0)
          eligible_negative_paths.push_back(eligible_paths[i]);
      }

      std::vector<std::pair<ND::TTRExReconModule::TTPCAnaPath *,
                            ND::TTRExReconModule::TTPCAnaPattern *> >
          eligible_muon_paths;
      for (unsigned int i = 0; i < eligible_negative_paths.size(); i++) {
        ND::TTRExReconModule::TTPCAnaPath *neg_path =
            eligible_negative_paths[i].first;
        if (isMuon(neg_path->MuonPull, neg_path->PionPull, neg_path->ProtonPull,
                   neg_path->ElePull, neg_path->Momentum))
          eligible_muon_paths.push_back(eligible_negative_paths[i]);
      }

      // CUT 4: found any muon candidates?
      if ((eligible_muon_paths.size() > 0) && (CutLevel == 3))
        CutLevel = 4;

      // CUT 5: fiducial volume

      std::vector<std::pair<ND::TTRExReconModule::TTPCAnaPath *,
                            ND::TTRExReconModule::TTPCAnaPattern *> >
          eligible_muon_paths_inFV;
      for (unsigned int i = 0; i < eligible_muon_paths.size(); i++) {
        ND::TTRExReconModule::TTPCAnaPath *muon_path =
            eligible_muon_paths[i].first;
        TVector3 firstpos = muon_path->FirstPosition;
        TVector3 lastpos = muon_path->LastPosition;

        if ((isInFV(fv_maxima, fv_minima, firstpos)) ||
            (isInFV(fv_maxima, fv_minima, lastpos)))
          eligible_muon_paths_inFV.push_back(eligible_muon_paths[i]);
      }

      // The cut level is not set to 5 here because we want to see the highest
      // momentum muon that *fails* the next cut. See code below.

      // VERTEX CUTS: Examine origin point of muon candidate
      //***************************************************

      int highest_cutlevel = 4;
      int highest_cutlevel_index = -1;
      PathEnd highest_cutlevel_end;
      for (unsigned int i = 0; i < eligible_muon_paths_inFV.size(); i++) {
        int temp_cutlevel = 4;
        PathEnd temp_selected_end;
        TVector3 vertex_pos;
        ND::TTRExReconModule::TTPCAnaPath *muon_path =
            eligible_muon_paths_inFV[i].first;
        ND::TTRExReconModule::TTPCAnaPattern *parent_pattern =
            eligible_muon_paths_inFV[i].second;

        // Get all junctions connected to muon candidate
        std::vector<ND::TTRExReconModule::TTPCAnaJunction *>
            connected_junctions;
        TClonesArray *Junctions = parent_pattern->Junctions;
        for (int j = 0; j < parent_pattern->NJunctions; j++) {
          ND::TTRExReconModule::TTPCAnaJunction *junc =
              (ND::TTRExReconModule::TTPCAnaJunction *)Junctions->At(j);
          for (int k = 0; k < junc->NPaths; k++) {
            if (junc->PathIDs[k] == muon_path->PathID)
              connected_junctions.push_back(junc);
          }
        }

        // Get both ends of the muon path
        std::vector<PathEnd> path_ends =
            getPathEnds(muon_path, connected_junctions);

        // Examine both ends of the muon path
        for (unsigned int j = 0; j < path_ends.size(); j++) {
          int final_cutlevel =
              evaluateFinalCuts(path_ends[j], parent_pattern, Patterns,
                                NPatterns, fv_maxima, fv_minima);
          if (final_cutlevel > temp_cutlevel) {
            temp_cutlevel = final_cutlevel;
            temp_selected_end = path_ends[j];
          }
          if (temp_cutlevel == 7)
            break;
        }

        if (temp_cutlevel < 7) // Failed to find a valid vertex candidate: jump
                               // back over secondary junctions if necessary
        {
          // TODO: ensure correct combination of information is returned when
          // leapfrogging

          std::vector<
              std::pair<ND::TTRExReconModule::TTPCAnaPath *,
                        std::vector<ND::TTRExReconModule::TTPCAnaJunction *> > >
              connected_muon_paths;

          // Get all paths with muon PIDs connected to the muon path
          connected_muon_paths =
              leapFrog(muon_path, connected_junctions, parent_pattern);

          // Sort connected muon paths from highest to lowest momentum
          std::stable_sort(connected_muon_paths.begin(),
                           connected_muon_paths.end(), MomentumGreaterThan_V2);

          // Get path ends of connected paths, discounting the ends that connect
          // to the original path
          std::vector<PathEnd> new_ends;
          for (unsigned int j = 0; j < connected_muon_paths.size(); j++) {
            std::vector<PathEnd> connected_path_ends = getPathEnds(
                connected_muon_paths[j].first, connected_muon_paths[j].second);
            for (unsigned int k = 0; k < connected_path_ends.size(); k++) {
              for (unsigned int l = 0; l < path_ends.size(); l++) {
                double diff =
                    (connected_path_ends[k].Position - path_ends[l].Position)
                        .Mag();
                if (diff > 30.0)
                  new_ends.push_back(connected_path_ends[k]);
              }
            }
          }

          // Apply cuts to new path ends
          for (unsigned int j = 0; j < new_ends.size(); j++) {
            int final_cutlevel =
                evaluateFinalCuts(new_ends[j], parent_pattern, Patterns,
                                  NPatterns, fv_maxima, fv_minima);
            if (final_cutlevel > temp_cutlevel) {
              temp_cutlevel = final_cutlevel;
              temp_selected_end = new_ends[j];
            }
            if (temp_cutlevel == 7)
              break;
          }
        }

        if (temp_cutlevel > highest_cutlevel) {
          highest_cutlevel = temp_cutlevel;
          highest_cutlevel_index = i;
          highest_cutlevel_end = temp_selected_end;
        }
        if (temp_cutlevel == 7) {
          break;
        }
      }

      if (CutLevel == 4)
        CutLevel = highest_cutlevel;

      //__________________________________________________________________________________________________________________
      // SELECTION FINISHED, CUT LEVEL SET

      // Saving vertex information
      //**************************
      if (CutLevel == 7) {
        // A muon candidate was selected. Save the information about that muon
        // candidate.

        ND::TTRExReconModule::TTPCAnaPattern *parent_pattern =
            eligible_muon_paths_inFV[highest_cutlevel_index].second;

        fillVertexInfo(BestVertex, parent_pattern, highest_cutlevel_end,
                       pattern_truth, truthy, fv_maxima, fv_minima, tpc, IsMC,
                       N_RooTrackerVertices, RooTrackerVertices, is_it_neut,
                       hasTrackerVertices);
      }
      //-------------------------------------
      if ((CutLevel == 6) || (CutLevel == 5)) {
        // The muon candidate that got furthest was rejected as a broken track
        // or a delta ray. Save the information about that muon candidate.
        // If there are more than one, the highest momentum muon will be
        // preferred

        ND::TTRExReconModule::TTPCAnaPattern *parent_pattern =
            eligible_muon_paths_inFV[highest_cutlevel_index].second;

        fillVertexInfo(BestVertex, parent_pattern, highest_cutlevel_end,
                       pattern_truth, truthy, fv_maxima, fv_minima, tpc, IsMC,
                       N_RooTrackerVertices, RooTrackerVertices, is_it_neut,
                       hasTrackerVertices);
      }
      //-------------------------------------
      if (CutLevel == 4) {
        // The muon candidate that got furthest failed the fiducial volume cut.
        // Save the information about that muon candidate.
        // If there are more than one, the highest momentum muon will be
        // preferred.

        ND::TTRExReconModule::TTPCAnaPath *chosen_path =
            eligible_muon_paths[0].first;
        ND::TTRExReconModule::TTPCAnaPattern *parent_pattern =
            eligible_muon_paths[0].second;

        // Get any junctions connected to OOFV path (could happen!)
        TClonesArray *PatternJuncs = parent_pattern->Junctions;
        std::vector<ND::TTRExReconModule::TTPCAnaJunction *> connected_juncs;
        for (int i = 0; i < parent_pattern->NJunctions; i++) {
          ND::TTRExReconModule::TTPCAnaJunction *junc =
              (ND::TTRExReconModule::TTPCAnaJunction *)PatternJuncs->At(i);
          for (int j = 0; j < junc->NPaths; j++) {
            if (junc->PathIDs[j] == chosen_path->PathID)
              connected_juncs.push_back(junc);
          }
        }

        // Fill vertex info from most upstream end of OOFV path (hey why not)
        std::vector<PathEnd> chosen_path_ends =
            getPathEnds(chosen_path, connected_juncs);
        PathEnd upstreamEnd;
        double most_upstream_Z = 9999.9;
        for (unsigned int i = 0; i < chosen_path_ends.size(); i++) {
          if (chosen_path_ends[i].Position.Z() < most_upstream_Z) {
            upstreamEnd = chosen_path_ends[i];
            most_upstream_Z = chosen_path_ends[i].Position.Z();
          }
        }
        fillVertexInfo(BestVertex, parent_pattern, upstreamEnd, pattern_truth,
                       truthy, fv_maxima, fv_minima, tpc, IsMC,
                       N_RooTrackerVertices, RooTrackerVertices, is_it_neut,
                       hasTrackerVertices);
      }
      //-----------------------------------------
      if (CutLevel <= 3) {
        // No muon candidates were observed. Save default fills.
        // Do nothing!
      }

      // Put information from BestVertex into tree
      NTracks = BestVertex.NTracks;
      NFittedTracks = BestVertex.NFittedTracks;
      NProtonTracks = BestVertex.NProtonTracks;
      Position_X = BestVertex.Position.X();
      Position_Y = BestVertex.Position.Y();
      Position_Z = BestVertex.Position.Z();

      MatchedTruthCode = BestVertex.MatchedTruthCode;
      MatchedPosition_X = BestVertex.MatchedPosition.X();
      MatchedPosition_Y = BestVertex.MatchedPosition.Y();
      MatchedPosition_Z = BestVertex.MatchedPosition.Z();
      MatchedSubdetector = BestVertex.MatchedSubdetector;
      MatchedTargetZ = BestVertex.MatchedTargetZ;
      MatchedNProtons = BestVertex.MatchedNProtons;
      MatchedNeutrinoMomentum = BestVertex.MatchedNeutrinoMomentum;
      MatchedMuonMomentum = BestVertex.MatchedMuonMomentum;
      MatchedTotalMomentum = BestVertex.MatchedTotalMomentum;

      VertexCleanliness = BestVertex.VertexCleanliness;
      VertexCompleteness = BestVertex.VertexCompleteness;

      VertexExtent_X = BestVertex.VertexExtent.X();
      VertexExtent_Y = BestVertex.VertexExtent.Y();
      VertexExtent_Z = BestVertex.VertexExtent.Z();
      TotalRecoMomentum = BestVertex.TotalRecoMomentum;
      LowestRecoMomentum = BestVertex.LowestRecoMomentum;
      HMNTMomentum = BestVertex.HMNTMomentum;
      HMNTIsMuon = BestVertex.HMNTIsMuon;

      MuMomDiff = BestVertex.MuMomDiff;
      LowestAngle = BestVertex.LowestAngle;
      NAssociatedPaths = BestVertex.NAssociatedPaths;

      outputTree->Fill();

      // Find most signal-y true vertex in tpc
      if (IsMC) {
        int best_code = -1;
        int n_protons = 0;
        int nu_pdg = 0;
        int nu_energy = -9999.9;
        int vertex_id = -1;
        int target_z = -1;
        int reaccode = 0;
        double mu_mom = -1;
        double q_sqrd = -1;
        double mu_ang = 0;
        for (int i = 0; i < NVtx; i++) {
          ND::TTruthVerticesModule::TTruthVertex *vertex =
              (ND::TTruthVerticesModule::TTruthVertex *)TrueVertices->At(i);
          vertex_id = vertex->ID;
          int truthcode = trueVertexTruthCode(vertex, fv_maxima, fv_minima, tpc,
                                              is_it_genie);
          int nvertprotons = 0;
          if (hasTrackerVertices)
            nvertprotons = countChargedTraj(vertex, 2212, N_RooTrackerVertices,
                                            RooTrackerVertices, is_it_neut);
          int current_pdg = vertex->NeutrinoPDG;
          double current_energy = vertex->NeutrinoMomentum.E();
          int current_z = ((vertex->TargetPDG) % 1000000000) / 10000;
          int current_reaccode = atoi(vertex->ReactionCode.c_str());

          double current_mu_mom = -1;
          double current_q_sqrd = -1;
          double current_mu_ang = -1;
          if ((current_reaccode < 30) &&
              (current_reaccode > 0)) // CC nu-mu interaction
          {
            if (hasTrackerVertices) {
              TLorentzVector muon_momentum = primaryMuonMomentum(
                  vertex, N_RooTrackerVertices, RooTrackerVertices, is_it_neut);
              current_mu_mom = muon_momentum.Vect().Mag();
              TLorentzVector diff = vertex->NeutrinoMomentum - muon_momentum;
              current_q_sqrd = (-1.0) * (diff * diff);
              TVector3 Z_axis(0, 0, 1);
              current_mu_ang = std::acos(((muon_momentum.Vect()).Dot(Z_axis)) /
                                         (muon_momentum.Vect().Mag()));
            }
          }

          if ((truthcode > best_code) && (best_code != 6)) {
            best_code = truthcode;
            n_protons = nvertprotons;
            nu_pdg = current_pdg;
            nu_energy = current_energy;
            target_z = current_z;
            reaccode = current_reaccode;
            mu_mom = current_mu_mom;
            q_sqrd = current_q_sqrd;
            mu_ang = current_mu_ang;
          }
        }
        VertexTruthCode = best_code;
        MatchedCutLevel = CutLevel;
        NReconstructedProtons = BestVertex.NProtonTracks;
        NTrueProtons = n_protons;
        NeutrinoPDG = nu_pdg;
        NeutrinoEnergy = nu_energy;
        TargetZ = target_z;
        ReactionCode = reaccode;
        QSquared = q_sqrd;
        MuonMomentum = mu_mom;
        MuonAngle = mu_ang;

        NRTVtx = NULL;
        GRTVtx = NULL;

        if (hasTrackerVertices) {
          for (int i = 0; i < N_RooTrackerVertices; i++) {
            if (is_it_neut) {
              ND::NRooTrackerVtx *vtx =
                  (ND::NRooTrackerVtx *)RooTrackerVertices->At(i);
              if (vtx->TruthVertexID == vertex_id) {
                NRTVtx = vtx;
                break;
              }
            }

            if (is_it_genie) {
              ND::GRooTrackerVtx *vtx =
                  (ND::GRooTrackerVtx *)RooTrackerVertices->At(i);
              if (vtx->TruthVertexID == vertex_id) {
                GRTVtx = vtx;
                break;
              }
            }
          }
        }

        if ((VertexTruthCode != 6) && (CutLevel == 6))
          skimlist << RunID << "," << SubrunID << "," << EventID << std::endl;

        outputTruthTree->Fill();
      }
    }
  }

  skimlist.close();

  // Write output file
  //*****************

  outputfile->cd();
  for (int i = 0; i < 3; i++) {
    treevec[i]->Write();
    if (IsMC)
      truthtreevec[i]->Write();
  }
  outputfile->Close();
  delete outputfile;

  return;
}
