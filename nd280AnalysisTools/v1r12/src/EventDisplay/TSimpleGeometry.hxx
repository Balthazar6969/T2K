#ifndef TSIMPLEGEOMETRY_HXX
#define TSIMPLEGEOMETRY_HXX
#include "TEveElement.h"
#include "TEveScene.h"
#include "TString.h"
#include "TEveTrack.h"
#include "TEveGeoNode.h"
#include "TGeoManager.h"
#include "TBuffer3D.h"

/// Class to create a simple representation of the ND280 geometry for Eve to use in event display.
///\author Alex Finch (A.Finch@lancaster.ac.uk)
class TSimpleGeometry {
private:
    TEveScene*     fSimpleGeometryScene;
    TEveElementList*     fBoxGeometry;

public:
    /// Construct the simple geometry (fSimpleGeometryScene).
    TSimpleGeometry();
    /// Construct the simple geometry (fSimpleGeometryScene). This constructor is
    /// to keep compiler happy, not actually used.
    TSimpleGeometry(int hard_coded_build);
    /// Return the pointer to the TEveScene which is the simple geometry.
    TEveScene* GetScene(){return fSimpleGeometryScene;};
    

private:
  //  void addSimpleGeometry();
    /// Find object called `path' in the geometry and add it to the nd280 box set.
    void addElementToSimpleGeometry(TEveElementList* nd280,TString path);
    /// Find object called `path' in the geometry and add it to the nd280 box set with
    /// a specific color.
    void addElementToSimpleGeometry(TEveElementList* nd280,TString path,int color);
/// Add the current volume in the geometry to the nd280 box set.
    void AddCurrentVolumeToSimpleGeometry(TEveElementList* nd280 );
/// Add the current volume in the geometry to the nd280 box set with a specific color.
    void AddCurrentVolumeToSimpleGeometry(TEveElementList* nd280, int color ) ; 
/// Add box to box set with a given color. `text' is for use in warning messages if
/// box capacity is exceeded. This is currently 5000 objects. If this is 
/// exceeded obect is not added.
    void AddBoxToElementList(TEveElementList* set,Float_t* box,int color,TString text);

};
#endif
