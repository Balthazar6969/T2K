#ifndef SPECTRUMWEIGHTING_HXX
#define SPECTRUMWEIGHTING_HXX

#include "RooAbsReal.h"
#include "RooRealProxy.h"
#include "TH1.h"
//_____________________________________________________________________________

class SpectrumWeighting : public RooAbsReal {
public:
  SpectrumWeighting(){};
  SpectrumWeighting(const char *name, const char *title, RooAbsReal &_energyNu,
                    TH1 *_numerator, TH1 *_denominator);

  SpectrumWeighting(const SpectrumWeighting &other, const char *name = 0);

  virtual TObject *clone(const char *newname) const {
    return new SpectrumWeighting(*this, newname);
  }

  // inline virtual ~SpectrumWeighting() {delete numerator;delete denominator;}
  virtual ~SpectrumWeighting() {
    delete numerator;
    delete denominator;
  }

protected:
  RooRealProxy energyNu;
  Double_t evaluate() const;

private:
  TH1 *numerator;
  TH1 *denominator;

  ClassDef(SpectrumWeighting, 1);
};

#endif
