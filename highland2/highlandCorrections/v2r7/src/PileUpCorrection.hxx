#ifndef PileUpCorrection_h
#define PileUpCorrection_h

#include "CorrectionBase.hxx"
#include "BinnedParams.hxx"

const UInt_t NMAXRUNPERIODS=20;

class PileUpCorrection: public CorrectionBase, public BinnedParams {
 public:

  PileUpCorrection();
  virtual ~PileUpCorrection(){}

  void Apply(AnaSpillC& spill);  

protected:

  UInt_t _nRunPeriods;
  Float_t _correction[NMAXRUNPERIODS];

};

#endif
