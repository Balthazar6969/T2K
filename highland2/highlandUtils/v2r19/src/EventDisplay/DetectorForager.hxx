#ifndef DetectorForager_h
#define DetectorForager_h

#include <TGeoManager.h>
#include <TGeoNode.h>
#include <TGeoVolume.h>
#include <TGeoShape.h>
#include <TGeoBBox.h>

#include <TMath.h>
#include <TVector3.h>

/// A class to collect the information for the detector and
/// provide it for future drawing
/// assume only boxes for the moment,  will review if needed

// I saw the Forager name in oaEvent plot-event and liked it
namespace det{

  /// Bound box for a detector
  typedef std::pair<TVector3,TVector3> DetBBox; 

  class DetectorForager{
  public:

    DetectorForager(){
      _detBBoxes.clear();
      _geoManager = NULL;
    }
    virtual ~DetectorForager(){
      _detBBoxes.clear(); 
    }

    /// Set geometry manager
    void SetGeoManager(TGeoManager* gem){
      _geoManager = gem;
    }
    /// Fill the detector information
    virtual bool FillDetectorInfo(){
      // Just for example
      DetBBox tmp;
      tmp.first.SetXYZ(500,500,500);
      tmp.second = - tmp.first;
      
      _detBBoxes[0] = tmp;
      
      return true;
    };
    

    /// Update a detector bounding box
    static void Bound(std::pair<TVector3,TVector3>& bnd, 
        const TVector3& c1, 
        const TVector3& c2);

    const std::map<Int_t, DetBBox>& GetDetBBoxes(){
      return _detBBoxes;
    }
    
  protected:

    /// Added boxes that store extensions of certain sub-detectors
    std::map<Int_t, DetBBox> _detBBoxes;

    /// An instance of the geometry manager
    /// geometry
    TGeoManager* _geoManager;

    // Various utility functions to navigate through geometry
    /// To visit all the nodes in the geometry.
    void VisitGeometry();

    virtual bool VisitNode(int, 
        const std::string& name, 
        const TGeoNode* node) {

      (void)name;

      TGeoVolume *volume = node->GetVolume();
      TGeoBBox* shape = dynamic_cast<TGeoBBox*>(volume->GetShape());
      TVector3 high(shape->GetDX(), shape->GetDY(), shape->GetDZ());
      TVector3 low = -high;

      return false;
    }


    /// Provide a convenient way to get the global coordinate of a node.
    TVector3 LocalToMaster(const TVector3& local);

    /// Update a detector bounding box with the conversion to local
    void BoundLocalToMaster(std::pair<TVector3,TVector3>& bnd, 
        const TVector3& c1, 
        const TVector3& c2);

    void RecurseGeometry(int depth, std::string name);

  };


};


#endif
