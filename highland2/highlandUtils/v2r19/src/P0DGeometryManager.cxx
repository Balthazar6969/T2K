#include "P0DGeometryManager.hxx"
#include "TGeoNode.h"
#include "TGeoManager.h"
#include "TGeoVolume.h"

//****************************************************************
void P0DGeometryManager::SetupGeometryRecurseGeomId(std::vector<std::string>& names) {
//****************************************************************
    
  if (!gSetupGeometry_P0DFinder) return;
  
  TGeoNode* node = gGeoManager->GetCurrentNode();
  names.push_back(node->GetName());
  
  // Search with a particular finder.  The finder will throw an
  // EGeomIdStop exception when it doesn't want to search any further.
  ND::TGeometryId id;
  if (gSetupGeometry_P0DFinder->Search(names, id)) {
    
    int rootKey = gGeoManager->GetCurrentNodeId();
    
    std::map<ND::TGeometryId,int>::iterator g 
      = gGeometrySetup_GeomIdMap.find(id);
    if (g != gGeometrySetup_GeomIdMap.end()) {
      std::cout << "ERROR: Duplicate id: " << id << std::endl;;
    }
    gGeometrySetup_GeomIdMap[id] = rootKey;
    gGeometrySetup_RootIdMap[rootKey] = id;
  }
  
  // Recurse through all of the daughters.
  for (int i=0; i<node->GetNdaughters(); ++i) {
    gGeoManager->CdDown(i);
    SetupGeometryRecurseGeomId(names);
  }
  
  // Pop this node off the "stack"
  gGeoManager->CdUp();
  names.pop_back();
}

//****************************************************************
Bool_t P0DGeometryManager::Notify() {
//****************************************************************

  if (!gGeoManager) return true;
  
  if (gSetupGeometry_P0DFinder) delete gSetupGeometry_P0DFinder;
  gSetupGeometry_P0DFinder = new SetupGeometryP0DFinder();
  
  // Make sure that the node id array has been initialized in the
  // TGeoNodeCache.
  if (!gGeoManager->GetCache()->HasIdArray()) {
    gGeoManager->GetCache()->BuildIdArray();
  }
  
  // Set the top volume.
  TGeoVolume* top = gGeoManager->GetTopVolume();
  std::string topName(top->GetName());
  if (topName != "t2k") {
    top = gGeoManager->GetVolume("t2k");
    gGeoManager->SetTopVolume(top);
  }
  
  // Clear the current geom id map.
  gGeometrySetup_GeomIdMap.clear();
  gGeometrySetup_RootIdMap.clear();
  
  // Save the current geometry state.
  gGeoManager->PushPath();
  gGeoManager->CdTop();
  
  std::vector<std::string> names(20); names.clear();
  SetupGeometryRecurseGeomId(names);
  
  // Restore the state.
  gGeoManager->PopPath();
  
  std::cout << "Geometry identifier map with " 
            << gGeometrySetup_GeomIdMap.size() << " entries." 
            << std::endl;;
  
  return true; 
}

//****************************************************************
TVector3 P0DGeometryManager::GeomIdPosition(int geomId) {
//****************************************************************

  std::map<ND::TGeometryId,int>::const_iterator pair = gGeometrySetup_GeomIdMap.find(geomId);
  if (pair == gGeometrySetup_GeomIdMap.end()) {
    return TVector3(9E+99,9E+99,9E+99);
  }
  int nodeId = pair->second;
  gGeoManager->PushPath();
  gGeoManager->CdNode(nodeId);
  // Find the global position
  double local[3] = {0,0,0};
  double master[3] = {0,0,0};
  gGeoManager->LocalToMaster(local,master);
  gGeoManager->PopPath();
  return TVector3(master[0],master[1],master[2]);
}
