#include <iostream>
#include <sstream>

#include "TBuffer3D.h"
#include "TChain.h"
#include "TFile.h"
#include "TGeoBBox.h"
#include "TGeoShape.h"
#include "TGeoVolume.h"
#include "TKey.h"
#include "TString.h"
#include "TVector3.h"

#include "TECALActiveBBFromGeom.hxx"

#//define DEBUG_TECALGeom

namespace TECALGeom {

inline std::ostream &operator<<(std::ostream &os, TVector3 const tv3) {
  return os << "[ " << tv3[0] << ", " << tv3[1] << ", " << tv3[2] << " ]"
            << std::flush;
}
inline std::ostream &operator<<(std::ostream &os,
                                std::pair<TVector3, TVector3> const tv3s) {
  return os << tv3s.first << " -- " << tv3s.second << std::flush;
}

// Get the ECal coords from the geometry
std::pair<TVector3, TVector3> GetECalCoords(TGeoManager *GeoM,
                                            std::vector<int> directories) {
  // Store the top path
  GeoM->PushPath();

  for (unsigned int i = 0; i < directories.size(); i++) {
    GeoM->CdDown(directories[i]);
  }
  // Check that we are in an active volume
  // std::cout << "[INFO]: Examining " << GeoM->GetPath() << std::endl;
  std::string node_name = GeoM->GetCurrentNode()->GetName();
  if (node_name != "Active_0") {
    std::cout << "[ERROR]: Expected to be in Active_0, instead in: "
              << node_name << " -- " << GeoM->GetPath() << std::endl;
  }

#ifdef DEBUG_TECALGeom
  std::cout << "[INFO]: Inside node: " << GeoM->GetCurrentNode()->GetName()
            << ", has " << GeoM->GetCurrentNode()->GetNdaughters()
            << " daughter nodes." << std::endl;

  unsigned int ndaught = GeoM->GetCurrentNode()->GetNdaughters();
  for (unsigned int i = 0; i < ndaught; ++i) {
    GeoM->CdDown(i);
    std::cout << "\t" << (((i == 0) ||
                           (i == (ndaught - 2)))
                              ? "+"
                              : " ")
              << " " << i << " " << GeoM->GetCurrentNode()->GetName()
              << std::endl;

    Double_t *local_coords_shape = GeoM->GetCurrentVolume()
                                       ->GetShape()
                                       ->GetBuffer3D(TBuffer3D::kAll, true)
                                       .fPnts;
    Double_t master_coords_shape[24];
    for (int j = 0; j < 24; j = j + 3) {
      GeoM->LocalToMaster(&local_coords_shape[j], &master_coords_shape[j]);
      std::cout << "\t\t Corner " << (j / 3)
                << TVector3(master_coords_shape[j], master_coords_shape[j + 1],
                            master_coords_shape[j + 2])
                << std::endl;
    }

    GeoM->CdUp();
  }
#endif

  // Get the final layer of lead
  GeoM->CdDown(GeoM->GetCurrentNode()->GetNdaughters() - 2);

  Double_t *local_coords_leadmax = GeoM->GetCurrentVolume()
                                       ->GetShape()
                                       ->GetBuffer3D(TBuffer3D::kAll, true)
                                       .fPnts;
  Double_t master_coords_leadmax[24];
  for (int i = 0; i < 24; i = i + 3) {
    GeoM->LocalToMaster(&local_coords_leadmax[i], &master_coords_leadmax[i]);
  }
  // We now have the global coordinates of the final absorber layer
  // This now needs to be translated such that we have a copy of this layer
  // overlapping the lowest part of the first scintillator layer
  GeoM->CdUp();
  GeoM->CdDown(0);
  // We should now be in the first scintillator layer directory
  // Convert the global coordinates of the final lead layer into the local
  // coordinates of the scintillator layer
  Double_t local_coords_leadmin[24];
  for (int i = 0; i < 24; i = i + 3) {
    GeoM->MasterToLocal(&master_coords_leadmax[i], &local_coords_leadmin[i]);
  }
  // We now need the local coordinates of the actual scintillator layer
  Double_t *local_coords_scintmin = GeoM->GetCurrentVolume()
                                        ->GetShape()
                                        ->GetBuffer3D(TBuffer3D::kAll, true)
                                        .fPnts;
  // Now replace the local Z coordinate of the lead with that of the
  // scintillator.  This will also scale the local height of the lead absorber
  // to be the same as the scintillator.  We will only be interested in one of
  // the bottom corners so this shouldn't matter
  for (int i = 0; i < 24; i = i + 3)
    local_coords_leadmin[i + 2] = local_coords_scintmin[i + 2];
  // Now, we can convert the coordinates of the translated lead layer into
  // global coordinates
  Double_t master_coords_leadmin[24];
  for (int i = 0; i < 24; i = i + 3) {
    GeoM->LocalToMaster(&local_coords_leadmin[i], &master_coords_leadmin[i]);
  }

  // Go back up a directory
  GeoM->CdUp();

  // We should now have enough information to define the active bounding box

  // Create the TVector3s for the min and max.  Need to take any two opposing
  // corners of the active volume
  TVector3 min(master_coords_leadmin[0], master_coords_leadmin[1],
               master_coords_leadmin[2]);
  TVector3 max(master_coords_leadmax[18], master_coords_leadmax[19],
               master_coords_leadmax[20]);

  // Return to the starting path
  GeoM->PopPath();

  // We are done here
  return std::make_pair(max, min);
}

void PrintReferenceGeometry(TGeoManager *GeoM) {
  int InitNode = GeoM->GetCurrentNodeId();
  GeoM->CdNode(0);
  // Get the barrel ECal active coordinates from the geometry
  std::vector<int> dirs;
  std::pair<TVector3, TVector3> BB;

  dirs.clear();
  dirs.push_back(0);
  dirs.push_back(0);
  dirs.push_back(1);
  dirs.push_back(1945);
  dirs.push_back(0);
  dirs.push_back(2);
  BB = GetECalCoords(GeoM, dirs);
  std::cout << "[INFO]: ECAL kBrBottomLeft: " << BB.first << " -- " << BB.second
            << std::endl;

  dirs.clear();
  dirs.push_back(0);
  dirs.push_back(0);
  dirs.push_back(2);
  dirs.push_back(1945);
  dirs.push_back(0);
  dirs.push_back(2);
  BB = GetECalCoords(GeoM, dirs);
  std::cout << "[INFO]: ECAL kBrBottomRight: " << BB.first << " -- "
            << BB.second << std::endl;

  dirs.clear();
  dirs.push_back(0);
  dirs.push_back(0);
  dirs.push_back(1);
  dirs.push_back(1944);
  dirs.push_back(0);
  dirs.push_back(2);
  BB = GetECalCoords(GeoM, dirs);
  std::cout << "[INFO]: ECAL kBrLeft: " << BB.first << " -- " << BB.second
            << std::endl;

  dirs.clear();
  dirs.push_back(0);
  dirs.push_back(0);
  dirs.push_back(2);
  dirs.push_back(1944);
  dirs.push_back(0);
  dirs.push_back(2);
  BB = GetECalCoords(GeoM, dirs);
  std::cout << "[INFO]: ECAL kBrRight" << BB.first << " -- " << BB.second
            << std::endl;

  dirs.clear();
  dirs.push_back(0);
  dirs.push_back(0);
  dirs.push_back(1);
  dirs.push_back(1943);
  dirs.push_back(0);
  dirs.push_back(2);
  BB = GetECalCoords(GeoM, dirs);
  std::cout << "[INFO]: ECAL kBrTopLeft: " << BB.first << " -- " << BB.second
            << std::endl;

  dirs.clear();
  dirs.push_back(0);
  dirs.push_back(0);
  dirs.push_back(2);
  dirs.push_back(1943);
  dirs.push_back(0);
  dirs.push_back(2);
  BB = GetECalCoords(GeoM, dirs);
  std::cout << "[INFO]: ECAL kBrTopRight: " << BB.first << " -- " << BB.second
            << std::endl;

  // Now the DS
  dirs.clear();
  dirs.push_back(0);
  dirs.push_back(0);
  dirs.push_back(0);
  dirs.push_back(46);
  dirs.push_back(1);
  BB = GetECalCoords(GeoM, dirs);
  std::cout << "[INFO]: ECAL kDs: " << BB.first << " -- " << BB.second
            << std::endl;
  GeoM->CdNode(InitNode);
}

std::pair<TVector3, TVector3> MergeBBs(
    std::pair<TVector3, TVector3> const &bb1,
    std::pair<TVector3, TVector3> const &bb2) {
  std::pair<TVector3, TVector3> rtn;

  rtn.first[0] =
      std::min(bb1.first[0],
               std::min(bb1.second[0], std::min(bb2.first[0], bb2.second[0])));
  rtn.first[1] =
      std::min(bb1.first[1],
               std::min(bb1.second[1], std::min(bb2.first[1], bb2.second[1])));
  rtn.first[2] =
      std::min(bb1.first[2],
               std::min(bb1.second[2], std::min(bb2.first[2], bb2.second[2])));

  rtn.second[0] =
      std::max(bb1.first[0],
               std::max(bb1.second[0], std::max(bb2.first[0], bb2.second[0])));
  rtn.second[1] =
      std::max(bb1.first[1],
               std::max(bb1.second[1], std::max(bb2.first[1], bb2.second[1])));
  rtn.second[2] =
      std::max(bb1.first[2],
               std::max(bb1.second[2], std::max(bb2.first[2], bb2.second[2])));
  return rtn;
}

std::pair<TVector3, TVector3> GetVolumeBB(TGeoManager *GeoM,
                                          std::string const &path = "") {
  int InitNode = GeoM->GetCurrentNodeId();

  std::pair<TVector3, TVector3> rtn(TVector3(0, 0, 0), TVector3(0, 0, 0));

  if (path.length()) {
    GeoM->cd(path.c_str());
  }
  int NodeId = GeoM->GetCurrentNodeId();
  TGeoNode *node = GeoM->GetCurrentNode();
  TGeoVolume *volume = node->GetVolume();
  TGeoBBox *shape = dynamic_cast<TGeoBBox *>(volume->GetShape());

  if (!shape) {
    return rtn;
  }

  double LOrig[] = {0, 0, 0};
  double MOrig[] = {0, 0, 0};
  GeoM->LocalToMaster(LOrig, MOrig);

  double LHalfSizes[] = {0, 0, 0};
  LHalfSizes[0] = shape->GetDX();
  LHalfSizes[1] = shape->GetDY();
  LHalfSizes[2] = shape->GetDZ();

  double MHalfSizesV[] = {0, 0, 0};
  GeoM->LocalToMasterVect(LHalfSizes, MHalfSizesV);

  GeoM->CdNode(InitNode);
  return std::make_pair(
      TVector3(MOrig[0] + MHalfSizesV[0], MOrig[1] + MHalfSizesV[1],
               MOrig[2] + MHalfSizesV[2]),
      TVector3(MOrig[0] - MHalfSizesV[0], MOrig[1] - MHalfSizesV[1],
               MOrig[2] - MHalfSizesV[2]));
}

void FindAbsScintRecursive(TGeoManager *GeoM, std::string &FirstScint,
                           std::string &LastAbsorb, std::string indent = "") {
  TGeoNode *node = GeoM->GetCurrentNode();
  TGeoVolume *vol = node->GetVolume();
  TGeoBBox *bokusu = NULL;

  if (!vol->GetNdaughters() && dynamic_cast<TGeoShape *>(vol->GetShape()) &&
      (bokusu = dynamic_cast<TGeoBBox *>(vol->GetShape()))) {
    std::string cPath = GeoM->GetPath();
    if ((!FirstScint.length()) &&
        (cPath.find("Absorber") != std::string::npos)) {
      throw 5;
    }
    if ((!FirstScint.length()) && (cPath.find("Scint") != std::string::npos)) {
      FirstScint = cPath;
    }
    if (cPath.find("Absorber") != std::string::npos) {
      LastAbsorb = cPath;
    }
    return;
  } else if (!vol->GetNdaughters()) {
    return;
  }
  for (Int_t i = 0; i < vol->GetNdaughters(); ++i) {
    GeoM->CdDown(i);
    FindAbsScintRecursive(GeoM, FirstScint, LastAbsorb, indent + "|-");
    GeoM->CdUp();
  }
}

TGeoManager *GetGeoManagerFromFile(TFile *GeomFile) {
  // Get TGeoManager key name
  TIter iter(GeomFile->GetListOfKeys());
  bool foundGeometry;
  std::string GeomKeyName;
  TObject *keyptr = NULL;
  while ((keyptr = iter())) {
    if (foundGeometry) {
      break;
    }
    TKey const *Key = dynamic_cast<TKey const *>(keyptr);
    if (!Key) {
      std::cout << "[ERROR]: Failed to cast TKey." << std::endl;
      continue;
    }
    TString keyName = Key->GetName();
    std::string className = Key->GetClassName();
    std::cout << "Found key: " << keyName << "(" << className << ")"
              << std::endl;
    if (className == "TGeoManager" && keyName.BeginsWith("ND280Geometry")) {
      std::cout << "[INFO]: Found the geometry manager." << std::endl;
      GeomKeyName = keyName.Data();
      foundGeometry = true;
    }
  }

  if (!foundGeometry) {
    return NULL;
  }

  return dynamic_cast<TGeoManager *>(GeomFile->Get(GeomKeyName.c_str()));
}

std::map<std::string, std::pair<TVector3, TVector3> >
GetECalFirstScintLastAbsorberBBs(TGeoManager *GeoM) {
  std::map<std::string, std::pair<TVector3, TVector3> > ABBMap;
  char const *Modules[] = {
      "/t2k_1/OA_0/Magnet_0/Basket_0/DsECal_0/Active_0",
      "/t2k_1/OA_0/Magnet_0/RightClam_0/BrlECal_1/Side_0/Active_0",
      "/t2k_1/OA_0/Magnet_0/LeftClam_0/BrlECal_1/Side_0/Active_0",
      "/t2k_1/OA_0/Magnet_0/RightClam_0/BrlECal_0/Top_0/Active_0",
      "/t2k_1/OA_0/Magnet_0/LeftClam_0/BrlECal_0/Top_0/Active_0",
      "/t2k_1/OA_0/Magnet_0/RightClam_0/BrlECal_2/Bottom_0/Active_0",
      "/t2k_1/OA_0/Magnet_0/LeftClam_0/BrlECal_2/Bottom_0/Active_0"};

  char const *ModuleNames[] = {"kDs",          "kBrRight",   "kBrLeft",
                               "kBrTopRight",  "kBrTopLeft", "kBrBottomRight",
                               "kBrBottomLeft"};

  std::cout << "[INFO]: Building ID Array..." << std::endl;
  GeoM->GetCache()->BuildIdArray();
  std::cout << "[INFO]: Built" << std::endl;

  std::string InitPath(GeoM->GetPath());

  size_t NModules = sizeof(Modules) / sizeof(char const *);
  for (size_t modit = 0; modit < NModules; ++modit) {
    std::cout << "[INFO]: Looking at volume: " << Modules[modit] << std::endl;
    if (!GeoM->cd(Modules[modit])) {
      std::cerr << "[ERROR]: Failed to cd into volume: " << Modules[modit]
                << std::endl;
      continue;
    }
    std::string FS = "", LA = "";
    FindAbsScintRecursive(GeoM, FS, LA, "");

    std::pair<TVector3, TVector3> BBfs = GetVolumeBB(GeoM, FS);
    std::pair<TVector3, TVector3> BBla = GetVolumeBB(GeoM, LA);

    std::cout << "FS: " << BBfs << ", LA: " << BBla << std::endl;

    std::pair<TVector3, TVector3> BB = MergeBBs(BBfs, BBla);
    ABBMap[ModuleNames[modit]] = BB;
  }
  return ABBMap;
}
}
