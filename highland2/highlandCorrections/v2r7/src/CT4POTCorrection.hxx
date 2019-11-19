#ifndef CT4POTCorrection_h
#define CT4POTCorrection_h

#include "CorrectionBase.hxx"
#include <vector>
#include <utility>

/// This correction ensures that the CT4 POT is used for POT accounting when
/// looking at MR44 data (when CT5 wasn't performing correctly).
class CT4POTCorrection: public CorrectionBase {
  public:

    CT4POTCorrection();

    virtual ~CT4POTCorrection() {}

    /// Apply the correction to the spill.
    void Apply(AnaSpillC& spill);

  private:

    /// The lower limit of MR runs to apply the correction to.
    int MRrunLower;

    /// The upper limit of MR runs to apply the correction to.
    int MRrunUpper;
};

#endif
