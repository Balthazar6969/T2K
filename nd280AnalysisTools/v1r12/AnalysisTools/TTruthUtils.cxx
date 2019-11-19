#include "TTruthUtils.hxx"
#include <TClonesArray.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

namespace {

bool SortTrajectoriesByMomentum(
    const ND::TTruthTrajectoriesModule::TTruthTrajectory *const traj1,
    const ND::TTruthTrajectoriesModule::TTruthTrajectory *const traj2) {
  return (traj1->InitMomentum.P() > traj2->InitMomentum.P());
}
}

TTruthUtils::TTruthUtils()
    : fTrajectories(NULL), fVertices(NULL), fNTrajectories(NULL),
      fNVertices(NULL), verbosity(0), fOrdered(true) {}

TTruthUtils::TTruthUtils(TClonesArray *trajectories, TClonesArray *vertices,
                         Int_t *n_trajectories, Int_t *n_vertices) {
  fTrajectories = trajectories;
  fVertices = vertices;
  fNTrajectories = n_trajectories;
  fNVertices = n_vertices;
  fOrdered = true;
  verbosity = 0;

  if ((fTrajectories != NULL) && (fNTrajectories != NULL)) {
    CheckTrajectoryOrdering();
  }
}

void TTruthUtils::SetTrajectoriesArray(TClonesArray *trajectories) {
  fTrajectories = trajectories;
  if ((fTrajectories != NULL) && (fNTrajectories != NULL)) {
    CheckTrajectoryOrdering();
  }
}

void TTruthUtils::SetTrajectoriesNumber(Int_t *n_trajectories) {
  fNTrajectories = n_trajectories;

  if ((fTrajectories != NULL) && (fNTrajectories != NULL)) {
    CheckTrajectoryOrdering();
  }
}

bool TTruthUtils::EnteredSubdetector(
    ND::TTruthTrajectoriesModule::TTruthTrajectory *traj,
    oaAnalysisEnums::ESubdetector SubDet) {
  if (!traj) {
    return false;
  }
  for (size_t i = 0; i < traj->TraceSubdetectors.size(); ++i) {
    if (traj->TraceSubdetectors[i] == static_cast<int>(SubDet)) {
      return true;
    }
  }
  return false;
}

bool TTruthUtils::ExitedSubdetector(
    ND::TTruthTrajectoriesModule::TTruthTrajectory *traj,
    oaAnalysisEnums::ESubdetector SubDet) {
  if (!traj) {
    return false;
  }
  for (size_t i = 0; i < traj->TraceSubdetectors.size(); ++i) {
    if (traj->TraceSubdetectors[i] == static_cast<int>(SubDet)) {
      return (i != (traj->TraceSubdetectors.size() - 1));
    }
  }
  return false;
}

TLorentzVector TTruthUtils::EntrancePosition(
    ND::TTruthTrajectoriesModule::TTruthTrajectory *traj,
    oaAnalysisEnums::ESubdetector SubDet) {
  if (!traj) {
    return TLorentzVector(0, 0, 0, 0);
  }
  for (size_t i = 0; i < traj->TraceSubdetectors.size(); ++i) {
    if (traj->TraceSubdetectors[i] == static_cast<int>(SubDet)) {
      return traj->TraceEntrancePosition[i];
    }
  }
  return TLorentzVector(0, 0, 0, 0);
}

TLorentzVector
TTruthUtils::ExitPosition(ND::TTruthTrajectoriesModule::TTruthTrajectory *traj,
                          oaAnalysisEnums::ESubdetector SubDet) {
  if (!traj) {
    return TLorentzVector(0, 0, 0, 0);
  }
  for (size_t i = 0; i < traj->TraceSubdetectors.size(); ++i) {
    if (traj->TraceSubdetectors[i] == static_cast<int>(SubDet)) {
      return (i != (traj->TraceSubdetectors.size() - 1))
                 ? traj->TraceExitPosition[i]
                 : TLorentzVector(0, 0, 0, 0);
    }
  }
  return TLorentzVector(0, 0, 0, 0);
}

ND::TTruthTrajectoriesModule::TTruthTrajectory *
TTruthUtils::BinaryTrajectorySearch(Int_t id, Int_t low, Int_t high) const {
  if (!fOrdered) {
    return FindTrajectorySlowly(id);
  }

  if ((high == -1) || (high > *fNTrajectories)) {
    high = *fNTrajectories - 1;
  }

  if (high < low) {
    if (verbosity == 1) {
      std::cerr << "[ERROR][TTruthUtils] Trajectory was not found - it is "
                   "not present in the oaAnalysis file"
                << std::endl;
    }
    return 0;
  }

  Int_t size = (high - low) / 2;
  Int_t mid = low + size;

  ND::TTruthTrajectoriesModule::TTruthTrajectory *mid_traj =
      static_cast<ND::TTruthTrajectoriesModule::TTruthTrajectory *>(
          fTrajectories->At(mid));

  if (mid_traj == NULL) {
    std::cerr << "[ERROR][TTruthUtils] Trajectory not found - The number of "
                 "trajectories in your TClonesArray does not match the number "
                 "TTruthUtils thinks are there."
              << std::endl;
    return 0;
  }

  Int_t mid_traj_id = mid_traj->ID;

  if (mid_traj_id == id) {
    return mid_traj;
  } else if (high == low) {
    if (verbosity == 1) {
      std::cerr << "[ERROR][TTruthUtils] Trajectory was not found - "
                   "it is not present in the oaAnalysis file "
                << std::endl;
    }
    return 0;
  } else if (mid_traj_id > id) {
    return BinaryTrajectorySearch(id, low, mid);
  } else if (mid_traj_id < id) {
    return BinaryTrajectorySearch(id, mid + 1, high);
  } else {
    if (verbosity == 1) {
      std::cerr << "[ERROR][TTruthUtils] Trajectory was not found - "
                   "it is not present in the oaAnalysis file"
                << std::endl;
    }
    return 0;
  }
}

ND::TTruthTrajectoriesModule::TTruthTrajectory *
TTruthUtils::FindTrajectorySlowly(Int_t id) const {
  ND::TTruthTrajectoriesModule::TTruthTrajectory *traj;

  for (Int_t i = 0; i < (*fNTrajectories); ++i) {
    traj = static_cast<ND::TTruthTrajectoriesModule::TTruthTrajectory *>(
        fTrajectories->At(i));
    if (id == traj->ID) {
      return traj;
    }
  }
  return 0;
}

void TTruthUtils::CheckTrajectoryOrdering() {
  ND::TTruthTrajectoriesModule::TTruthTrajectory *traj;
  int old_id = -1;
  for (Int_t i = 0; i < (*fNTrajectories); ++i) {
    traj = static_cast<ND::TTruthTrajectoriesModule::TTruthTrajectory *>(
        fTrajectories->At(i));
    if (old_id > traj->ID) {
      fOrdered = false;
    }
    old_id = traj->ID;
  }
  return;
}

ND::TTruthTrajectoriesModule::TTruthTrajectory *
TTruthUtils::GetTrajectoryById(Int_t id) const {
  if ((fTrajectories == 0) || (fNTrajectories == 0)) {
    std::cerr << "[ERROR][TTruthUtils] Trajectories clones array / "
                 "number not available"
              << std::endl;
    return 0;
  }

  // Check for common 'not set' type values
  if (id <= 0) {
    return 0;
  }

  return BinaryTrajectorySearch(id);
}

ND::TTruthTrajectoriesModule::TTruthTrajectory *
TTruthUtils::GetParentTrajectory(Int_t child_id) const {
  return GetParentTrajectory(GetTrajectoryById(child_id));
}

ND::TTruthTrajectoriesModule::TTruthTrajectory *
TTruthUtils::GetParentTrajectory(
    ND::TTruthTrajectoriesModule::TTruthTrajectory *child_trajectory) const {
  if (child_trajectory == NULL) {
    return NULL;
  } else {
    return GetTrajectoryById(child_trajectory->ParentID);
  }
}

ND::TTruthTrajectoriesModule::TTruthTrajectory *
TTruthUtils::GetPrimaryParentTrajectory(Int_t child_id) const {
  return GetPrimaryParentTrajectory(GetTrajectoryById(child_id));
}

ND::TTruthTrajectoriesModule::TTruthTrajectory *
TTruthUtils::GetPrimaryParentTrajectory(
    ND::TTruthTrajectoriesModule::TTruthTrajectory *child_trajectory) const {
  return GetTrajectoryById(child_trajectory->PrimaryID);
}

std::vector<ND::TTruthTrajectoriesModule::TTruthTrajectory *>
TTruthUtils::GetMomentumSortedTrajectories() const {
  std::vector<ND::TTruthTrajectoriesModule::TTruthTrajectory *> trajectories;

  trajectories = GetTrajectoriesVector();

  std::sort(trajectories.begin(), trajectories.end(),
            SortTrajectoriesByMomentum);

  return trajectories;
}

std::vector<ND::TTruthTrajectoriesModule::TTruthTrajectory *>
TTruthUtils::GetTrajectoriesByCategory(
    oaAnalysisEnums::EParticleCategory category) const {
  std::vector<ND::TTruthTrajectoriesModule::TTruthTrajectory *> trajectories;

  if ((fTrajectories == 0) || (fNTrajectories == 0)) {
    std::cerr << "[ERROR][TTruthUtils] Trajectories clones array / "
                 "number not available"
              << std::endl;
    return trajectories;
  }

  ND::TTruthTrajectoriesModule::TTruthTrajectory *traj;

  for (Int_t i = 0; i < (*fNTrajectories); ++i) {
    traj =
        (ND::TTruthTrajectoriesModule::TTruthTrajectory *)(*fTrajectories)[i];
    if (traj->Category == static_cast<int>(category)) {
      trajectories.push_back(traj);
    }
  }

  return trajectories;
}

std::vector<ND::TTruthTrajectoriesModule::TTruthTrajectory *>
TTruthUtils::GetTrajectoriesByFinalSubdetector(
    oaAnalysisEnums::ESubdetector final_subdetector) const {
  std::vector<ND::TTruthTrajectoriesModule::TTruthTrajectory *> trajectories;

  if ((fTrajectories == 0) || (fNTrajectories == 0)) {
    std::cerr << "[ERROR][TTruthUtils] Trajectories clones array / "
                 "number not available"
              << std::endl;
    return trajectories;
  }

  ND::TTruthTrajectoriesModule::TTruthTrajectory *traj;

  for (Int_t i = 0; i < (*fNTrajectories); ++i) {
    traj = static_cast<ND::TTruthTrajectoriesModule::TTruthTrajectory *>(
        fTrajectories->At(i));
    if (traj->TraceSubdetectors.size() &&
        (traj->TraceSubdetectors.back() ==
         static_cast<int>(final_subdetector))) {
      trajectories.push_back(traj);
    }
  }

  return trajectories;
}

std::vector<ND::TTruthTrajectoriesModule::TTruthTrajectory *>
TTruthUtils::GetTrajectoriesByInitSubdetector(
    oaAnalysisEnums::ESubdetector init_subdetector) const {
  std::vector<ND::TTruthTrajectoriesModule::TTruthTrajectory *> trajectories;

  if ((fTrajectories == 0) || (fNTrajectories == 0)) {
    std::cerr << "[ERROR][TTruthUtils] Trajectories clones array / "
                 "number not available"
              << std::endl;
    return trajectories;
  }

  ND::TTruthTrajectoriesModule::TTruthTrajectory *traj;

  for (Int_t i = 0; i < (*fNTrajectories); ++i) {
    traj = static_cast<ND::TTruthTrajectoriesModule::TTruthTrajectory *>(
        fTrajectories->At(i));
    if (traj->TraceSubdetectors.size() &&
        (traj->TraceSubdetectors.front() ==
         static_cast<int>(init_subdetector))) {
      trajectories.push_back(traj);
    }
  }

  return trajectories;
}

std::vector<ND::TTruthTrajectoriesModule::TTruthTrajectory *>
TTruthUtils::GetTrajectoriesByName(std::string name) const {
  std::vector<ND::TTruthTrajectoriesModule::TTruthTrajectory *> trajectories;

  if ((fTrajectories == 0) || (fNTrajectories == 0)) {
    std::cerr << "[ERROR][TTruthUtils] Trajectories clones array / "
                 "number not available"
              << std::endl;
    return trajectories;
  }

  ND::TTruthTrajectoriesModule::TTruthTrajectory *traj;

  for (Int_t i = 0; i < (*fNTrajectories); ++i) {
    traj = static_cast<ND::TTruthTrajectoriesModule::TTruthTrajectory *>(
        fTrajectories->At(i));
    if (traj->Name == name) {
      trajectories.push_back(traj);
    }
  }

  return trajectories;
}

std::vector<ND::TTruthTrajectoriesModule::TTruthTrajectory *>
TTruthUtils::GetTrajectoriesByPDG(Int_t pdg) const {
  std::vector<ND::TTruthTrajectoriesModule::TTruthTrajectory *> trajectories;

  if ((fTrajectories == 0) || (fNTrajectories == 0)) {
    std::cerr << "[ERROR][TTruthUtils] Trajectories clones array / "
                 "number not available"
              << std::endl;
    return trajectories;
  }

  ND::TTruthTrajectoriesModule::TTruthTrajectory *traj;

  for (Int_t i = 0; i < (*fNTrajectories); ++i) {
    traj = static_cast<ND::TTruthTrajectoriesModule::TTruthTrajectory *>(
        fTrajectories->At(i));
    if (traj->PDG == pdg) {
      trajectories.push_back(traj);
    }
  }

  return trajectories;
}

std::vector<ND::TTruthTrajectoriesModule::TTruthTrajectory *>
TTruthUtils::GetTrajectoriesByParent(Int_t parent_id) const {
  std::vector<ND::TTruthTrajectoriesModule::TTruthTrajectory *> trajectories;

  if ((fTrajectories == 0) || (fNTrajectories == 0)) {
    std::cerr << "[ERROR][TTruthUtils] Trajectories clones array / "
                 "number not available"
              << std::endl;
    return trajectories;
  }

  ND::TTruthTrajectoriesModule::TTruthTrajectory *traj;

  // TODO
  // This loop could be optimised utilising the fact that the parent's
  // ID will always be lower than the child's ID

  for (Int_t i = 0; i < (*fNTrajectories); ++i) {
    traj = static_cast<ND::TTruthTrajectoriesModule::TTruthTrajectory *>(
        fTrajectories->At(i));
    if (traj->ParentID == parent_id) {
      trajectories.push_back(traj);
    }
  }

  return trajectories;
}

std::vector<ND::TTruthTrajectoriesModule::TTruthTrajectory *>
TTruthUtils::GetTrajectoriesByParent(
    ND::TTruthTrajectoriesModule::TTruthTrajectory *parent) const {
  return GetTrajectoriesByParent(parent->ID);
}

std::vector<ND::TTruthTrajectoriesModule::TTruthTrajectory *>
TTruthUtils::GetTrajectoriesByPrimaryParent(Int_t primary_parent_id) const {
  std::vector<ND::TTruthTrajectoriesModule::TTruthTrajectory *> trajectories;

  if ((fTrajectories == 0) || (fNTrajectories == 0)) {
    std::cerr << "[ERROR][TTruthUtils] Trajectories clones array / "
                 "number not available"
              << std::endl;
    return trajectories;
  }

  ND::TTruthTrajectoriesModule::TTruthTrajectory *traj;

  // TODO
  // This loop could be optimised utilising the fact that the primary parent's
  // ID will always be lower than the child's ID

  for (Int_t i = 0; i < (*fNTrajectories); ++i) {
    traj = static_cast<ND::TTruthTrajectoriesModule::TTruthTrajectory *>(
        fTrajectories->At(i));
    if (traj->PrimaryID == primary_parent_id) {
      trajectories.push_back(traj);
    }
  }

  return trajectories;
}

std::vector<ND::TTruthTrajectoriesModule::TTruthTrajectory *>
TTruthUtils::GetTrajectoriesByPrimaryParent(
    ND::TTruthTrajectoriesModule::TTruthTrajectory *primary_parent) const {
  return GetTrajectoriesByPrimaryParent(primary_parent->ID);
}

std::vector<ND::TTruthTrajectoriesModule::TTruthTrajectory *>
TTruthUtils::GetTrajectoriesBySubdetector(
    oaAnalysisEnums::ESubdetector subdetector) const {
  std::vector<ND::TTruthTrajectoriesModule::TTruthTrajectory *> trajectories;

  if ((fTrajectories == 0) || (fNTrajectories == 0)) {
    std::cerr << "[ERROR][TTruthUtils] Trajectories clones array / "
                 "number not available"
              << std::endl;
    return trajectories;
  }

  ND::TTruthTrajectoriesModule::TTruthTrajectory *traj;

  for (Int_t i = 0; i < (*fNTrajectories); ++i) {
    traj = static_cast<ND::TTruthTrajectoriesModule::TTruthTrajectory *>(
        fTrajectories->At(i));
    for (std::vector<int>::const_iterator subdet_it =
             traj->TraceSubdetectors.begin();
         subdet_it != traj->TraceSubdetectors.end(); ++subdet_it) {
      if ((*subdet_it) == static_cast<int>(subdetector)) {
        trajectories.push_back(traj);
      }
    }
  }
  return trajectories;
}

std::vector<ND::TTruthTrajectoriesModule::TTruthTrajectory *>
TTruthUtils::GetTrajectoriesVector() const {
  std::vector<ND::TTruthTrajectoriesModule::TTruthTrajectory *> trajectories;

  if ((fTrajectories == 0) || (fNTrajectories == 0)) {
    std::cerr << "[ERROR][TTruthUtils] Trajectories clones array / "
                 "number not available"
              << std::endl;
    return trajectories;
  }

  ND::TTruthTrajectoriesModule::TTruthTrajectory *traj;

  // TODO
  // This loop could be optimised utilising the fact that the primary parent's
  // ID will always be lower than the child's ID

  for (Int_t i = 0; i < (*fNTrajectories); ++i) {
    traj = static_cast<ND::TTruthTrajectoriesModule::TTruthTrajectory *>(
        fTrajectories->At(i));
    trajectories.push_back(traj);
  }

  return trajectories;
}

ND::TTruthVerticesModule::TTruthVertex *
TTruthUtils::GetVertexById(Int_t id) const {
  if ((fVertices == 0) || (fNVertices == 0)) {
    std::cerr << "[ERROR][TTruthUtils] Vertices clones array / "
                 "number not available"
              << std::endl;
    return 0;
  }

  // Check for common 'not set' type values
  if (id < 0) {
    return NULL;
  }

  // TODO
  // This array is sorted in ascending ID order, so we can implement a
  // binary search to significantly speed this up. However, I'm just
  // going to put a linear search here now so I can compile and commit.

  ND::TTruthVerticesModule::TTruthVertex *vtx;

  for (Int_t i = 0; i < (*fNVertices); ++i) {
    vtx =
        static_cast<ND::TTruthVerticesModule::TTruthVertex *>(fVertices->At(i));
    if (id == vtx->ID) {
      return vtx;
    }
  }
  return 0;
}

ND::TTruthVerticesModule::TTruthVertex *
TTruthUtils::GetVertexOfTrajectory(Int_t trajectory_id) const {
  return GetVertexOfTrajectory(GetTrajectoryById(trajectory_id));
}

ND::TTruthVerticesModule::TTruthVertex *TTruthUtils::GetVertexOfTrajectory(
    ND::TTruthTrajectoriesModule::TTruthTrajectory *trajectory) const {
  if (trajectory == NULL) {
    std::cerr << "[ERROR][TTruthUtils] Trajectory does not exist in the "
                 "analysis file therefore cannot get its vertex"
              << std::endl;
    return 0;
  }
  return GetVertexOfPrimaryTrajectory(trajectory->PrimaryID);
}

ND::TTruthVerticesModule::TTruthVertex *
TTruthUtils::GetVertexOfPrimaryTrajectory(Int_t primary_trajectory_id) const {
  if ((fVertices == 0) || (fNVertices == 0)) {
    std::cerr << "[ERROR][TTruthUtils] Vertices clones array / "
                 "number not available"
              << std::endl;
    return 0;
  }

  // Check for common 'not set' type values
  if (primary_trajectory_id < 0) {
    return NULL;
  }

  ND::TTruthVerticesModule::TTruthVertex *vtx;

  for (Int_t i = 0; i < (*fNVertices); ++i) {
    vtx =
        static_cast<ND::TTruthVerticesModule::TTruthVertex *>(fVertices->At(i));

    for (size_t j = 0; j != vtx->PrimaryTrajIDs.size(); ++j) {
      if (vtx->PrimaryTrajIDs.at(j) == primary_trajectory_id) {
        return vtx;
      }
    }
  }
  return 0;
}

ND::TTruthVerticesModule::TTruthVertex *
TTruthUtils::GetVertexOfPrimaryTrajectory(
    ND::TTruthTrajectoriesModule::TTruthTrajectory *primary_trajectory) const {
  return GetVertexOfPrimaryTrajectory(primary_trajectory->PrimaryID);
}

std::vector<ND::TTruthVerticesModule::TTruthVertex *>
TTruthUtils::GetVerticesByLocation(const TVector3 &min_corner,
                                   const TVector3 &max_corner) const {
  std::vector<ND::TTruthVerticesModule::TTruthVertex *> vertices;

  if ((fVertices == 0) || (fNVertices == 0)) {
    std::cerr << "[ERROR][TTruthUtils] Vertices clones array / "
                 "number not available"
              << std::endl;
    return vertices;
  }

  ND::TTruthVerticesModule::TTruthVertex *vtx;
  TVector3 pos;

  for (Int_t i = 0; i < (*fNVertices); ++i) {
    vtx =
        static_cast<ND::TTruthVerticesModule::TTruthVertex *>(fVertices->At(i));
    pos = vtx->Position.Vect();

    if ((pos.X() > min_corner.X()) && (pos.X() < max_corner.X()) &&
        (pos.Y() > min_corner.Y()) && (pos.Y() < max_corner.Y()) &&
        (pos.Z() > min_corner.Z()) && (pos.Z() < max_corner.Z())) {
      vertices.push_back(vtx);
    }
  }
  return vertices;
}

std::vector<ND::TTruthVerticesModule::TTruthVertex *>
TTruthUtils::GetVerticesByTime(Double_t min_time, Double_t max_time) const {
  std::vector<ND::TTruthVerticesModule::TTruthVertex *> vertices;

  if ((fVertices == 0) || (fNVertices == 0)) {
    std::cerr << "[ERROR][TTruthUtils] Vertices clones array / "
                 "number not available"
              << std::endl;
    return vertices;
  }

  ND::TTruthVerticesModule::TTruthVertex *vtx;
  Double_t time;

  for (Int_t i = 0; i < (*fNVertices); ++i) {
    vtx =
        static_cast<ND::TTruthVerticesModule::TTruthVertex *>(fVertices->At(i));
    time = vtx->Position.T();

    if ((time < max_time) && (time > min_time))
      vertices.push_back(vtx);
  }

  return vertices;
}

std::vector<ND::TTruthVerticesModule::TTruthVertex *>
TTruthUtils::GetVerticesByNeutrinoPDG(Int_t neutrino_pdg) const {
  std::vector<ND::TTruthVerticesModule::TTruthVertex *> vertices;

  if ((fVertices == 0) || (fNVertices == 0)) {
    std::cerr << "[ERROR][TTruthUtils] Vertices clones array / "
                 "number not available"
              << std::endl;
    return vertices;
  }

  ND::TTruthVerticesModule::TTruthVertex *vtx;

  for (Int_t i = 0; i < (*fNVertices); ++i) {
    vtx =
        static_cast<ND::TTruthVerticesModule::TTruthVertex *>(fVertices->At(i));
    if (vtx->NeutrinoPDG == neutrino_pdg) {
      vertices.push_back(vtx);
    }
  }

  return vertices;
}

std::vector<ND::TTruthVerticesModule::TTruthVertex *>
TTruthUtils::GetVerticesByTargetPDG(Int_t target_pdg) const {
  std::vector<ND::TTruthVerticesModule::TTruthVertex *> vertices;

  if ((fVertices == 0) || (fNVertices == 0)) {
    std::cerr << "[ERROR][TTruthUtils] Vertices clones array / "
                 "number not available"
              << std::endl;
    return vertices;
  }

  ND::TTruthVerticesModule::TTruthVertex *vtx;

  for (Int_t i = 0; i < (*fNVertices); ++i) {
    vtx =
        static_cast<ND::TTruthVerticesModule::TTruthVertex *>(fVertices->At(i));
    if (vtx->TargetPDG == target_pdg) {
      vertices.push_back(vtx);
    }
  }

  return vertices;
}

std::vector<ND::TTruthVerticesModule::TTruthVertex *>
TTruthUtils::GetVerticesBySubdetector(
    oaAnalysisEnums::ESubdetector subdetector) const {
  std::vector<ND::TTruthVerticesModule::TTruthVertex *> vertices;

  if ((fVertices == 0) || (fNVertices == 0)) {
    std::cerr << "[ERROR][TTruthUtils] Vertices clones array / "
                 "number not available"
              << std::endl;
    return vertices;
  }

  ND::TTruthVerticesModule::TTruthVertex *vtx;

  for (Int_t i = 0; i < (*fNVertices); ++i) {
    vtx =
        static_cast<ND::TTruthVerticesModule::TTruthVertex *>(fVertices->At(i));
    if (vtx->Subdetector == static_cast<int>(subdetector)) {
      vertices.push_back(vtx);
    }
  }

  return vertices;
}

std::vector<ND::TTruthVerticesModule::TTruthVertex *>
TTruthUtils::GetVerticesVector() const {
  std::vector<ND::TTruthVerticesModule::TTruthVertex *> vertices;

  if ((fVertices == 0) || (fNVertices == 0)) {
    std::cerr << "[ERROR][TTruthUtils] Vertices clones array / "
                 "number not available"
              << std::endl;
    return vertices;
  }

  ND::TTruthVerticesModule::TTruthVertex *vtx;

  for (Int_t i = 0; i < (*fNVertices); ++i) {
    vtx =
        static_cast<ND::TTruthVerticesModule::TTruthVertex *>(fVertices->At(i));
    vertices.push_back(vtx);
  }

  return vertices;
}
