#ifndef ProductionUtils_h
#define ProductionUtils_h

#include "VersionManager.hxx"
#include "oaAnalysisReaderProjectHeaders.h"

  /// Enum for the different productions suported
namespace ProdId{
  enum ProdEnum{
    unknown=UNKNOWN_PRODUCTION,
    PROD5D,
    PROD5E,
    PROD5F,
    PROD5G,
    PROD6PRE,
    PROD6A,
    PROD6B,
    PROD6C,
    PROD6D,
    PROD6E,
    PROD6F,
    PROD6G,
    PROD6H,
    PROD6I,
    PROD6J,
    PROD6K,
    PROD6L,
    PROD6M,
    PROD7DEVEL
  };
}

/// This namespace contains useful functions for analyses. 
/// This is in addition to those defined in psycheUtils
namespace anaUtils{

    /// Get Production Id from nd280AnalysisTools
    ProdId_h GetProductionIdFromoaAnalysisReader();

    /// Add the standard production versions
    void AddStandardProductions();
}
#endif


