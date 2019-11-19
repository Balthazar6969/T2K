#ifndef TPCExpecteddEdxCorrection_h
#define TPCExpecteddEdxCorrection_h

#include "CorrectionBase.hxx"
#include "DataClasses.hxx"

/// This correction uses a new parameterisation of the TPC dEdx lines to
/// re-calculate the TPC pulls.
/// The parameterisation is different to that use in tpcRecon, and comes from
/// work Claudio Giganti did to improve the Data/MC comparisons for Production 5.
class TPCExpecteddEdxCorrection: public CorrectionBase {
 public:

  TPCExpecteddEdxCorrection();
  virtual ~TPCExpecteddEdxCorrection(){}

  /// Apply the TPCExpecteddEdxCorrection. Updates the expected dEdx for all
  /// particle hypotheses, and recomputes the pulls based on these values.
  void Apply(AnaSpillC& spill);
};

#endif
