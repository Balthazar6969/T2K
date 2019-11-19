//******************************************************************************
//******************************************************************************
//******************************************************************************
// N.B. This has been verbatim copied from a similar header in oaAnalysis.
// Do not modify here without modifying in oaAnalysis first and then copying
// over here.
//******************************************************************************
//******************************************************************************
//******************************************************************************

#ifndef OANALYSISDEFS_HXX
#define OANALYSISDEFS_HXX

#include <iostream>
#include <stdexcept>
#include <string>

#include "TEnv.h"
#include "TMath.h"
#include "TObject.h"
#include "TParticlePDG.h"
#include "TSystem.h"

/// Utility tools to be shared across analysis classes
///
/// Mostly bookkeeping routines to ensure consistency and
/// easy referencing
namespace oaAnalysisEnums {

enum ESubdetector {
  kFGD1 = 0,
  kFGD2 = 1,
  kP0D = 2,
  kDsECal = 3,
  kBrlECal = 4, // DEPRECATED
  kP0DECal = 5, // DEPRECATED
  kTPC1 = 6,
  kTPC2 = 7,
  kTPC3 = 8,
  kSMRD = 9,
  kOffAxis = 10, /// the rest of the off-axis detector
  kINGRID = 11,
  kCavern = 12,
  kNSubdetectors = 21,
  // Organised as such to try and preclude backwards compatibility errors at
  // the moment, will probably be reorganised in the future [lp208 21-1-2015]
  kP0DECalBottom = 13,
  kP0DECalLeft = 14,
  kP0DECalTop = 15,
  kP0DECalRight = 16,
  kBrlECalBottom = 17,
  kBrlECalLeft = 18,
  kBrlECalTop = 19,
  kBrlECalRight = 20
};

enum EParticleCategory {
  kChargedLepton = 0,
  kChargedBaryon,
  kChargedMeson,
  kPhoton,
  kOtherCharged,
  kOtherNeutral,
  kOther,
  kNParticleCategories
};

inline ESubdetector PathToSubdetector(const std::string path) {

  if (path.find("Basket") != std::string::npos) {
    // P0D
    if (path.find("P0D") != std::string::npos) {
      return kP0D;
    }
    // DsECal
    else if (path.find("DsECal") != std::string::npos) {
      return kDsECal;
    }
    // FGD1
    else if (path.find("FGD1") != std::string::npos) {
      return kFGD1;
    }
    // FGD2
    else if (path.find("FGD2") != std::string::npos) {
      return kFGD2;
    }
    // TPC1
    else if (path.find("TPC1") != std::string::npos) {
      return kTPC1;
    }
    // TPC2
    else if (path.find("TPC2") != std::string::npos) {
      return kTPC2;
    }
    // TPC3
    else if (path.find("TPC3") != std::string::npos) {
      return kTPC3;
    }
  }

  if (path.find("Clam") != std::string::npos) {
    // BrlECal
    if (path.find("BrlECal") != std::string::npos) {
      if (path.find("Top") != std::string::npos) {
        return kBrlECalTop;
      } else if (path.find("Side") != std::string::npos) {
        if (path.find("LeftClam") != std::string::npos) {
          return kBrlECalLeft;
        } else if (path.find("RightClam") != std::string::npos) {
          return kBrlECalRight;
        }
      } else if (path.find("Bottom") != std::string::npos) {
        return kBrlECalBottom;
      }
      return kBrlECal;
    }
    // P0DECal
    else if (path.find("P0DECal") != std::string::npos) {
      if (path.find("Top") != std::string::npos) {
        return kP0DECalTop;
      } else if (path.find("Side") != std::string::npos) {
        if (path.find("LeftClam") != std::string::npos) {
          return kP0DECalLeft;
        } else if (path.find("RightClam") != std::string::npos) {
          return kP0DECalRight;
        }
      } else if (path.find("Bottom") != std::string::npos) {
        return kP0DECalBottom;
      }
      return kP0DECal;
    }
    // SMRD
    else if ((path.find("MRD") != std::string::npos) ||
             (path.find("FluxReturn") != std::string::npos)) {
      return kSMRD;
    }
  }

  // Other parts of the off-axis
  if (path.find("OA") != std::string::npos) {
    return kOffAxis;
  }
  // INGRID
  else if (path.find("INGRID") != std::string::npos) {
    return kINGRID;
  } else {
    return kCavern;
  }
}

inline std::string DetectorName(ESubdetector const subdet) {
  switch (subdet) {
  case kFGD1: {
    return "FGD1";
  }
  case kFGD2: {
    return "FGD2";
  }
  case kP0D: {
    return "P0D";
  }
  case kDsECal: {
    return "DsECal";
  }
  case kBrlECal: {
    return "BrlECal";
  }
  case kP0DECal: {
    return "P0DECal";
  }
  case kTPC1: {
    return "TPC1";
  }
  case kTPC2: {
    return "TPC2";
  }
  case kTPC3: {
    return "TPC3";
  }
  case kSMRD: {
    return "SMRD";
  }
  case kOffAxis: {
    return "OffAxis";
  }
  case kINGRID: {
    return "INGRID";
  }
  case kCavern: {
    return "Cavern";
  }
  case kP0DECalBottom: {
    return "P0DECalBottom";
  }
  case kP0DECalLeft: {
    return "P0DECalLeft";
  }
  case kP0DECalTop: {
    return "P0DECalTop";
  }
  case kP0DECalRight: {
    return "P0DECalRight";
  }
  case kBrlECalBottom: {
    return "BrlECalBottom";
  }
  case kBrlECalLeft: {
    return "BrlECalLeft";
  }
  case kBrlECalTop: {
    return "BrlECalTop";
  }
  case kBrlECalRight: {
    return "BrlECalRight";
  }
  default: { return "NONE"; }
  }
}

inline ESubdetector Next(ESubdetector const subdet) {
  switch (subdet) {
  case kFGD1: {
    return kFGD2;
  }
  case kFGD2: {
    return kP0D;
  }
  case kP0D: {
    return kDsECal;
  }
  case kDsECal: {
    return kBrlECal;
  }
  case kBrlECal: {
    return kP0DECal;
  }
  case kP0DECal: {
    return kTPC1;
  }
  case kTPC1: {
    return kTPC2;
  }
  case kTPC2: {
    return kTPC3;
  }
  case kTPC3: {
    return kSMRD;
  }
  case kSMRD: {
    return kOffAxis;
  }
  case kOffAxis: {
    return kINGRID;
  }
  case kINGRID: {
    return kCavern;
  }
  case kCavern: {
    return kP0DECalBottom;
  }
  case kP0DECalBottom: {
    return kP0DECalLeft;
  }
  case kP0DECalLeft: {
    return kP0DECalTop;
  }
  case kP0DECalTop: {
    return kP0DECalRight;
  }
  case kP0DECalRight: {
    return kBrlECalBottom;
  }
  case kBrlECalBottom: {
    return kBrlECalLeft;
  }
  case kBrlECalLeft: {
    return kBrlECalTop;
  }
  case kBrlECalTop: {
    return kBrlECalRight;
  }
  case kBrlECalRight: {
    return kNSubdetectors;
  }
  default: { return kNSubdetectors; }
  }
}

inline ESubdetector IntToSubdetector(int const intdet) {
  ESubdetector subdet = static_cast<ESubdetector>(intdet);
  switch (subdet) {
  case kFGD1: {
    return kFGD1;
  }
  case kFGD2: {
    return kFGD2;
  }
  case kP0D: {
    return kP0D;
  }
  case kDsECal: {
    return kDsECal;
  }
  case kBrlECal: {
    return kBrlECal;
  }
  case kP0DECal: {
    return kP0DECal;
  }
  case kTPC1: {
    return kTPC1;
  }
  case kTPC2: {
    return kTPC2;
  }
  case kTPC3: {
    return kTPC3;
  }
  case kSMRD: {
    return kSMRD;
  }
  case kOffAxis: {
    return kOffAxis;
  }
  case kINGRID: {
    return kINGRID;
  }
  case kCavern: {
    return kCavern;
  }
  case kP0DECalBottom: {
    return kP0DECalBottom;
  }
  case kP0DECalLeft: {
    return kP0DECalLeft;
  }
  case kP0DECalTop: {
    return kP0DECalTop;
  }
  case kP0DECalRight: {
    return kP0DECalRight;
  }
  case kBrlECalBottom: {
    return kBrlECalBottom;
  }
  case kBrlECalLeft: {
    return kBrlECalLeft;
  }
  case kBrlECalTop: {
    return kBrlECalTop;
  }
  case kBrlECalRight: {
    return kBrlECalRight;
  }
  case kNSubdetectors: {
    return kNSubdetectors;
  }
  default: {
    std::cerr << "[ERROR](" << __FILE__ ":" << __LINE__
              << "): Attempted to cast " << intdet
              << " to a oaAnalysisEnums::ESubdetector." << std::endl;
    throw std::make_pair("oaAnalysisEnums::ESubdetector.", intdet);
  }
  }
}

inline EParticleCategory PDGInfoToCategory(const TParticlePDG *pdgInfo) {

  if (pdgInfo) {
    Int_t pdg = pdgInfo->PdgCode();
    // rounding Double_t charge/3 to closest Int_t (ROOT returns a double in
    // units of e/3!)
    Int_t charge = TMath::Nint(pdgInfo->Charge() / 3.0);
    std::string particleClass = pdgInfo->ParticleClass();
    // Charged Particles
    if (charge != 0) {
      // Leptons
      if ((TMath::Abs(pdg) == 11) || (TMath::Abs(pdg) == 13) ||
          (TMath::Abs(pdg) == 15)) {
        return kChargedLepton;
      }

      if (particleClass.find("Baryon") != std::string::npos) {
        return kChargedBaryon;
      } else if (particleClass.find("Meson") != std::string::npos) {
        return kChargedMeson;
      } else {
        return kOtherCharged;
      }
    } else if (pdg == 22) {
      return kPhoton;
    } else {
      return kOtherNeutral;
    }
  } else {
    return kOther;
  }
}

inline EParticleCategory PDGToCategory(const Int_t pdg) {
  TParticlePDG pdgInfo(pdg);
  return PDGInfoToCategory(&pdgInfo);
}

inline void UseCustomPDGTable() {
  TString tablePath = TString(gSystem->Getenv("ND280ANALYSISTOOLSROOT")) +
                      "/input/supplementary_pdg_table.txt";
  bool Gottit = gSystem->AccessPathName(tablePath);
  if (Gottit) {
    std::cerr << "[ERROR](" << __FILE__ << ":" << __LINE__ << ") " << tablePath
              << " is not accesible, cannot load extra PDG data. "
                 "(Expect: "
                 "${ND280ANALYSISTOOLSROOT}/input/supplementary_pdg_table.txt)"
              << std::endl;
    throw std::exception();
  }
  gEnv->SetValue("Root.DatabasePDG", tablePath);
}

inline std::string ParticleName(EParticleCategory const category) {
  switch (category) {
  case kChargedLepton: {
    return "Charged Lepton";
  }
  case kChargedBaryon: {
    return "Charged Baryon";
  }
  case kChargedMeson: {
    return "Charged Meson";
  }
  case kPhoton: {
    return "Photon";
  }
  case kOtherCharged: {
    return "Charged Other";
  }
  case kOtherNeutral: {
    return "Neutral Other";
  }
  case kOther: {
    return "Other";
  }
  default: { return "NONE"; }
  }
}

inline EParticleCategory Next(EParticleCategory const category) {
  switch (category) {
  case kChargedLepton: {
    return kChargedBaryon;
  }
  case kChargedBaryon: {
    return kChargedMeson;
  }
  case kChargedMeson: {
    return kPhoton;
  }
  case kPhoton: {
    return kOtherCharged;
  }
  case kOtherCharged: {
    return kOtherNeutral;
  }
  case kOtherNeutral: {
    return kOther;
  }
  case kOther: {
    return kNParticleCategories;
  }
  default: { return kNParticleCategories; }
  }
}
}

#endif // OANALYSISDEFS_HXX
