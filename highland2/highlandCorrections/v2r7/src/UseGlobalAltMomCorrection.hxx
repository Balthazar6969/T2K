#ifndef UseGlobalAltMomCorrection_h
#define UseGlobalAltMomCorrection_h

#include "CorrectionBase.hxx"

/// This correction replaces the momentum of a global track with the momentum
/// from a refit done assuming a particle hypothesis. This refit is only done
/// for tracks with TPC constituents, so if the refit value isn't present we
/// leave the existing momentum.
class UseGlobalAltMomCorrection: public CorrectionBase {
  public:

    /// Enumerator describing the refits that are available
    enum Hypothesis {
      kMuon, kElectron, kProton
    };

    /// Constructor which accepts as an argument the hypothesis to use when
    /// updating the Momentum and MomentumError members of each track.
    UseGlobalAltMomCorrection(Hypothesis hyp);

    virtual ~UseGlobalAltMomCorrection() {}

    /// Apply the variation to all tracks with TPC info.
  void Apply(AnaSpillC& spill);

  private:

    /// The hypothesis to use.
    Hypothesis _hyp;

};

#endif
