#ifndef TMiscAnalysisUtils_h_seen
#define TMiscAnalysisUtils_h_seen
#include "libReadoaAnalysis/libReadoaAnalysisProjectHeaders.h"
#include <TObject.h>

/// oaAnalysis read-out utility in macros/AnalysisTools,
/// which contains miscellaneous utilities.
///
/// This class contains miscellaneous utilities to help read out
/// oaAnalysis files, but is empty for now except for the legacy
/// function LorentzVectorSTLVectorElementKludge, which is not
/// needed any more
///
class TMiscAnalysisUtils{

public:
  TMiscAnalysisUtils(){};

  virtual ~TMiscAnalysisUtils(){};

  static TLorentzVector LorentzVectorSTLVectorElementKludge(
      std::vector<TLorentzVector> lorentzVectors, Int_t i);
};
#endif
