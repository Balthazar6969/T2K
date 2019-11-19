#ifndef tutorialCorrection_h
#define tutorialCorrection_h

#include "CorrectionBase.hxx"

const UInt_t NMAXRUNPERIODS=20;

class tutorialCorrection: public CorrectionBase {
 public:

  tutorialCorrection();
  virtual ~tutorialCorrection(){}

  void Apply(AnaSpillC& spill);  

protected:

  UInt_t _nRunPeriods;
  Float_t _correction[NMAXRUNPERIODS];

};

#endif
