#ifndef TPCdEdxMCCorrection_h
#define TPCdEdxMCCorrection_h

#include "CorrectionBase.hxx"
#include "DataClasses.hxx"

/// This class defines a correction that affects the measured dE/dx of an
/// AnaTpcSegment. This in turn affects the computed pulls (how electron-like a
/// track is, for example).
///
/// This correction is applied to MC, and currently has NO EFFECT.
class TPCdEdxMCCorrection: public CorrectionBase {
  public:

    TPCdEdxMCCorrection();

    virtual ~TPCdEdxMCCorrection() {}

    /// Apply the dE/dx correction to all the AnaTpcSegment members of all
    /// tracks in the spill. See TPCdEdxMCCorrection class documentation for
    /// details.
  void Apply(AnaSpillC& spill);

  private:

    /// Get the correction factor for the given true particle type.
    double GetCorrection(int pdg);

};

#endif
