#ifndef TECALGeom_hxx_seen
#define TECALGeom_hxx_seen

#include <map>
#include <string>

// ROOT
#include "TFile.h"
#include "TGeoManager.h"
#include "TVector3.h"

namespace TECALGeom {
///\brief Gets the bounding boxes for the scintillator and lead layers in each
/// ECal module from a proffered geometry manager.
///\details
///\note The geometry manage will be left at the same path as when starting
/// but will have the nodeid cache rebuilt.
///\note The bounding boxes are slightly smaller than the Active_0 volumes
/// within each module, this was chosen so that these bounding boxes contain as
/// little non-target material as possible.
/// Recursively looks for the first node named \"Scint\" and the last node named
///\"Absorber\" inside the \"Active\" volumes of each ECal module.
/// These are then merged to form the overall bounding box.
/// The following volumes are used as root volumes of the search:
/// * kDs : /t2k_1/OA_0/Magnet_0/Basket_0/DsECal_0/Active_0
/// * kBrRight : /t2k_1/OA_0/Magnet_0/RightClam_0/BrlECal_1/Side_0/Active_0
/// * kBrLeft : /t2k_1/OA_0/Magnet_0/LeftClam_0/BrlECal_1/Side_0/Active_0
/// * kBrTopRight : /t2k_1/OA_0/Magnet_0/RightClam_0/BrlECal_0/Top_0/Active_0
/// * kBrTopLeft : /t2k_1/OA_0/Magnet_0/LeftClam_0/BrlECal_0/Top_0/Active_0
/// * kBrBottomRight :
/// /t2k_1/OA_0/Magnet_0/RightClam_0/BrlECal_2/Bottom_0/Active_0
/// * kBrBottomLeft :
/// /t2k_1/OA_0/Magnet_0/LeftClam_0/BrlECal_2/Bottom_0/Active_0
std::map<std::string, std::pair<TVector3, TVector3> >
GetECalFirstScintLastAbsorberBBs(TGeoManager *GeoM);
///\brief Prints the ECal module bounding boxes using the code from
/// D. Brailsford's thesis.
///\details These should be the same as
/// TECALGeom::GetECalFirstScintLastAbsorberBBs.
void PrintReferenceGeometry(TGeoManager *GeoM);
///\brief Looks for ND280 Geometry manager in proferred TFile, returns it if
/// found.
TGeoManager *GetGeoManagerFromFile(TFile *GeomFile);
}

#endif
