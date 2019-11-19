#ifndef IgnoreRightECalRuns3and4Correction_h
#define IgnoreRightECalRuns3and4Correction_h

#include "CorrectionBase.hxx"
#include <vector>
#include <utility>

/// This correction removes all information from the right ECal
/// in runs 3 and 4 data, and runs 3 and 4 equivalent MC. The reason
/// for this is that a TFB died during the earthquake, and has not been
/// replaced. This affects the ECal PID and energy resolution in a way that
/// a systematic has not been evaluated for yet. So the simple solution is
/// to just ignore that ECal entirely.
///
/// The parameters in baseAnalysis.parameters.dat define the run periods for which
/// this correction should be applied.
class IgnoreRightECalRuns3and4Correction: public CorrectionBase {
  public:

    /// Constructor, which reads in the parameter file to set up the run
    /// periods for which the correction should be applied.
    IgnoreRightECalRuns3and4Correction();

    virtual ~IgnoreRightECalRuns3and4Correction() {}

    /// Apply the variation to all tracks with TPC info.
  void Apply(AnaSpillC& spill);

  private:

    /// Whether we should apply the correction to this event.
    bool ShouldApplyCorrection(AnaSpillC& spill);

    /// Join the base string with the pairing integer.
    std::string GetParamString(std::string base, int pairing);

    /// The run periods to apply this correction for in MC. List of start-end
    /// run numbers, inclusive.
    std::vector<std::pair<int, int> > _runs_MC;

    /// The run periods to apply this correction for in data. List of start-end
    /// run numbers, inclusive.
    std::vector<std::pair<int, int> > _runs_Data;
};

#endif
