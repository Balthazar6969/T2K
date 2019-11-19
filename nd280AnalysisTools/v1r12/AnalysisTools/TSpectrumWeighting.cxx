#include "Riostream.h"
#include "TSpectrumWeighting.hxx"
//____________________________________________________________________________

ClassImp(SpectrumWeighting);

SpectrumWeighting::SpectrumWeighting(const char *name, const char *title,
                                     RooAbsReal &_energyNu, TH1 *_numerator,
                                     TH1 *_denominator)
    : RooAbsReal(name, title),
      energyNu("energyNu", "energyNu", this, _energyNu),
      numerator((TH1 *)(_numerator->Clone())),
      denominator((TH1 *)(_denominator->Clone())) {
  numerator->SetDirectory(0);
  denominator->SetDirectory(0);
}

SpectrumWeighting::SpectrumWeighting(const SpectrumWeighting &other,
                                     const char *name)
    : RooAbsReal(other, name), energyNu("energyNu", this, other.energyNu),
      numerator((TH1 *)(other.numerator->Clone())),
      denominator((TH1 *)(other.denominator->Clone())) {
  numerator->SetDirectory(0);
  denominator->SetDirectory(0);
}

Double_t SpectrumWeighting::evaluate() const {
  Double_t denom(denominator->GetBinContent(denominator->FindBin(energyNu)));
  /// 1000))); Energy is in GeV.
  if (denom == 0) {
    return 0.0;
  };
  Double_t value(numerator->GetBinContent(numerator->FindBin(energyNu)) /
                 denom);
  // 1000.0))/denom);
  return value;
}
