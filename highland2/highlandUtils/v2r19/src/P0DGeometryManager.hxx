#ifndef SetupGeometry_H_Seen
#define SetupGeometry_H_Seen

#include <ND280GeomId.hxx>

#include <iostream>
#include <TVector3.h>
#include <map>

/// Internal class.  Find TGeoManager node ids associated with the
/// ND::TGeometryId objects.
class SetupGeometryP0DFinder {
public: 
    SetupGeometryP0DFinder()  
        : fSP0Dule(-1), fP0Dule(-1), fLayer(-1), fBar(-1), fTarget(-1), 
          fECalRadiator(-1), fTargetRadiator(-1) {}
    
    ~SetupGeometryP0DFinder() {}

    bool Search(const std::vector<std::string>& names, ND::TGeometryId& id)  {
        // Assign a geometry identifier to the full P0D.
        if (names.size()<5) return false;
        if (names[4].find("P0D_")==std::string::npos) return false;
        
#ifdef DEBUG_P0DFinder
        for (std::vector<std::string>::const_iterator s = names.begin();
             s != names.end();
             ++s) std::cout << " " << *s;
        std::cout << std::endl;
#endif
        
        if (names.back().find("P0D_")!=std::string::npos) {
            id = ND::GeomId::P0D::Detector();
            return true;
        }
        
        // Assign the super-P0Dule number.
        if (names.back().find("USECal_")!=std::string::npos) {
            fSP0Dule = 0;
            id = ND::GeomId::P0D::SuperP0Dule(fSP0Dule);
            return true;
        }
        else if (names.back().find("USTarget_")!=std::string::npos) {
            fSP0Dule = 1;
            id = ND::GeomId::P0D::SuperP0Dule(fSP0Dule);
            return true;
        }
        else if (names.back().find("CTarget_")!=std::string::npos) {
            fSP0Dule = 2;
            id = ND::GeomId::P0D::SuperP0Dule(fSP0Dule);
            return true;
        }
        else if (names.back().find("CECal_")!=std::string::npos) {
            fSP0Dule = 3;
            id = ND::GeomId::P0D::SuperP0Dule(fSP0Dule);
            return true;
        }
        
        // Assign the p0dule number
        if (names.back().find("P0Dule_")!=std::string::npos) {
            ++fP0Dule;
            fLayer = -1;
            fBar = -1;
            id = ND::GeomId::P0D::P0Dule(fP0Dule);
            return true;
        }
        
        // Assign the target.
        if (names.back().find("Target_") == 0) {
            ++fTarget;
            id = ND::GeomId::P0D::Target(fTarget);
            return true;
        }
        
        // Assign the radiator.
        if (names.back().find("Radiator_") !=std::string::npos) {
            if (fSP0Dule == 0 || fSP0Dule==3) {
                ++fECalRadiator;
                id = ND::GeomId::P0D::ECalRadiator(fECalRadiator);
            }
            else {
                ++fTargetRadiator;
                id = ND::GeomId::P0D::TargetRadiator(fTargetRadiator);
            }
            return true;
        }
        
        // Assign the layer 
        if (names.back().find("X_")!=std::string::npos) {
            fLayer = 0;
            fBar = -1;
            return false;
        }
        else if (names.back().find("Y_")!=std::string::npos) {
            fLayer = 1;
            fBar = -1;
            return false;
        }
        
        if (names.back().find("Bar_")!=std::string::npos) {
            id = ND::GeomId::P0D::Bar(fP0Dule, fLayer, ++fBar);
            return true;
        }
        
        return false;
    }


private: 
    /// The current super-P0Dule
    int fSP0Dule;

    /// The current p0dule within the super-P0Dule
    int fP0Dule;

    /// The current layer within the p0dule
    int fLayer;
    
    /// The current bar within the layer
    int fBar;

    /// The current target.
    int fTarget;

    /// The most recently seen radiator in the ECal.
    int fECalRadiator;
    
    /// The most recently seen radiator in the target.
    int fTargetRadiator;
};

/// A class to update the local state when the input TFile
/// changes. This is added to the gBasicHeader_GeometryNotify vector.
class P0DGeometryManager{
public:
  P0DGeometryManager() {gSetupGeometry_P0DFinder = NULL;Notify(); }

  /// Internal function. A routine to recurse through the TGeoManager geometry. 
  void SetupGeometryRecurseGeomId(std::vector<std::string>& names);

  /// Called when the geometry has changed.
  Bool_t Notify();

  /// Translate a geometry id into a position.  If the geometry id doesn't exist
  /// in the P0D, then this returns a value that is beyond the boundary of the
  /// observable universe.
  TVector3 GeomIdPosition(int geomId);

protected:

  /// Internal pointer. A pointer to the instantiation of the P0D geometry id
  /// finder.
  SetupGeometryP0DFinder* gSetupGeometry_P0DFinder;

  /// Internal map.  A map of the geometry nodes index by the TGeometryId
  /// values.
  std::map<ND::TGeometryId,int> gGeometrySetup_GeomIdMap;
  
  /// Internal map. A map of the TGeometryId values index by the geometry
  /// node.
  std::map<int,ND::TGeometryId> gGeometrySetup_RootIdMap;

};

#endif

