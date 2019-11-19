#include <iostream>

#include "TECALActiveBBFromGeom.hxx"

inline std::ostream &operator<<(std::ostream &os, TVector3 const tv3) {
  return os << "[ " << tv3[0] << ", " << tv3[1] << ", " << tv3[2] << " ]"
            << std::flush;
}

int main(int argc, char const *argv[]) {
  if (argc != 2) {
    std::cerr << "[ERROR]: Expected input file containing ROOT geometry."
              << std::endl;
    return 1;
  }
  TFile *inp = new TFile(argv[1], "OPEN");
  if (!inp) {
    std::cerr << "[ERROR]: CLI Argument is not the location of a valid TFile: "
              << argv[1] << std::endl;
    return 1;
  }

  TGeoManager *GeoM = TECALGeom::GetGeoManagerFromFile(inp);
  if (!GeoM) {
    std::cerr << "[ERROR]: Could not find TGeoManager object in file: "
              << inp->GetName() << std::endl;
    return 1;
  }
  std::cout << "[INFO]: ECAL Scint-Absorber BBs from geometry." << std::endl;
  std::map<std::string, std::pair<TVector3, TVector3> > ABBs =
      TECALGeom::GetECalFirstScintLastAbsorberBBs(GeoM);
  for (std::map<std::string, std::pair<TVector3, TVector3> >::iterator bbi =
           ABBs.begin();
       bbi != ABBs.end(); ++bbi) {
    std::cout << "[INFO]: " << bbi->first << ": " << bbi->second.first << " -- "
              << bbi->second.second << std::endl;
  }

  std::cout
      << "\n[INFO]: ECAL Scint-Absorber BBs from D. Brailsford's analysis code."
      << std::endl;
  TECALGeom::PrintReferenceGeometry(GeoM);
  return 0;
}
