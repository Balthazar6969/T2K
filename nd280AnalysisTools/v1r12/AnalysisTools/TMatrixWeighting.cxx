#include "Riostream.h"
#include "TMatrixWeighting.hxx"
//_____________________________________________________________________________

ClassImp(MatrixWeighting);

MatrixWeighting::MatrixWeighting(const char *name, const char *title,
                                 RooAbsReal &_energyNu, TH1 *_genFlux,
                                 TH2 *_matrix)
    : RooAbsReal(name, title),
      energyNu("energyNu", "energyNu", this, _energyNu),
      genFlux((TH1 *)_genFlux->Clone()), matrix((TH2 *)_matrix->Clone()) {
  genFlux->SetDirectory(0);
  matrix->SetDirectory(0);
}

MatrixWeighting::MatrixWeighting(const MatrixWeighting &other, const char *name)
    : RooAbsReal(other, name), energyNu("energyNu", this, other.energyNu),
      genFlux((TH1 *)other.genFlux->Clone()),
      matrix((TH2 *)other.matrix->Clone()) {
  genFlux->SetDirectory(0);
  matrix->SetDirectory(0);
}
// Get the weight on the flux
Double_t MatrixWeighting::evaluate() const {

  Double_t denom(genFlux->GetBinContent(genFlux->FindBin(energyNu)));
  if (denom == 0) {
    return 0.0;
  };
  Double_t value = 0.;
  UInt_t nbins = genFlux->GetNbinsX();
  for (UInt_t i = 1; i <= nbins; i++) {
    value += genFlux->GetBinContent(i) *
             matrix->GetBinContent(
                 matrix->FindBin(energyNu, genFlux->GetBinCenter(i)));
  }

  value /= denom;
  return value;
}
