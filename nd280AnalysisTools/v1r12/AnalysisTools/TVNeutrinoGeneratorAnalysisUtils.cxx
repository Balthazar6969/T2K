#include "TVNeutrinoGeneratorAnalysisUtils.hxx"

Bool_t TVNeutrinoGeneratorAnalysisUtils::IsCCQE(
    ND::TTruthVerticesModule::TTruthVertex *vertex) const {
  return this->IsQELCC(vertex);
}
