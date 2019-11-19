#ifndef MATRIXWEIGHTING_HXX
#define MATRIXWEIGHTING_HXX

#include "RooAbsReal.h"
#include "RooRealProxy.h"
#include "TH1.h"
#include "TH2.h"
//____________________________________________________________________________
// Re-weight a spectrum using a matrix
class MatrixWeighting : public RooAbsReal {

public:
  MatrixWeighting(){};
  MatrixWeighting(const char *name, const char *title, RooAbsReal &_energyNu,
                  TH1 *_genFlux, TH2 *_matrix);

  MatrixWeighting(const MatrixWeighting &other, const char *name = 0);

  virtual TObject *clone(const char *newname) const {
    return new MatrixWeighting(*this, newname);
  }

  // inline virtual ~MatrixWeighting() {delete genFlux;delete matrix;}
  virtual ~MatrixWeighting() {
    delete genFlux;
    delete matrix;
  }

protected:
  RooRealProxy energyNu;
  Double_t evaluate() const;

private:
  // Take the flux information from a histogram
  TH1 *genFlux;
  // Get the re-weighting matrix
  TH2 *matrix;

  ClassDef(MatrixWeighting, 1);
};

#endif
