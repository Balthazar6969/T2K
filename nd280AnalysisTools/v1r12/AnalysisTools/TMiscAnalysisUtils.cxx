#include "TMiscAnalysisUtils.hxx"

TLorentzVector TMiscAnalysisUtils::LorentzVectorSTLVectorElementKludge(
    std::vector<TLorentzVector> lorentzVectors, Int_t i) {
  /// A kludge which used to allow choosing an element from a
  /// vector<TLorentzVector>,
  /// which is now doable from within python without this kludge, using the
  /// header file vectorTLorentzVector_Dict.h

  std::cout
      << "The function TMiscAnalysisUtils::LorentzVectorSTLVectorElementKludge"
      << std::endl;
  std::cout << "is deprecated, because std::vector<TLorentzVector>s can be "
            << std::endl;
  std::cout << "accessed from within python now. For example, if "
               "\"true_fgd_vertex.LeptomMoms\" is a"
            << std::endl;
  std::cout
      << "std::vector<TLorentzVector>, then its elements can be accessed with"
      << std::endl;
  std::cout << "true_fgd_vertex.LeptonMoms[idx]" << std::endl;

  return lorentzVectors[i];
}
