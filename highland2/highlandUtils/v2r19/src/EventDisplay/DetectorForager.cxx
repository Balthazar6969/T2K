#include "DetectorForager.hxx"

//********************************************************************
void det::DetectorForager::VisitGeometry() {
  //********************************************************************
  _geoManager->PushPath();
  _geoManager->CdTop();
  RecurseGeometry(0,"");
  _geoManager->PopPath();
}


//********************************************************************
TVector3 det::DetectorForager::LocalToMaster(const TVector3& local) {
  //********************************************************************
  double loc[3] = {local.X(), local.Y(), local.Z()};
  double mas[3];
  _geoManager->LocalToMaster(loc,mas);
  return TVector3(mas[0], mas[1], mas[2]);
}


//********************************************************************
void det::DetectorForager::Bound(std::pair<TVector3,TVector3>& bnd, 
    const TVector3& low, 
    const TVector3& high){
  //********************************************************************

  for (int i=0; i<3; ++i) {
    bnd.first[i] = std::min(bnd.first[i],low[i]);
    bnd.first[i] = std::min(bnd.first[i],high[i]);
    bnd.second[i] = std::max(bnd.second[i],low[i]);
    bnd.second[i] = std::max(bnd.second[i],high[i]);
  }
}

//********************************************************************
void det::DetectorForager::BoundLocalToMaster(std::pair<TVector3,TVector3>& bnd, 
    const TVector3& c1, 
    const TVector3& c2){
  //********************************************************************
  TVector3 low = LocalToMaster(c1);
  TVector3 high = LocalToMaster(c2);
  return Bound(bnd, low, high);
  
}


//********************************************************************
void det::DetectorForager::RecurseGeometry(int depth, std::string name)  {
//********************************************************************

  TGeoNode* node = gGeoManager->GetCurrentNode();
  name = name + "/" + node->GetName();

  if (!VisitNode(depth, name, node)) {
    _geoManager->CdUp();
    return;
  }

  // Navigate through the daughter volumes
  for (int i=0; i<node->GetNdaughters(); ++i) {
    _geoManager->CdDown(i);
    RecurseGeometry(depth + 1, name);
  }

  gGeoManager->CdUp();
}






