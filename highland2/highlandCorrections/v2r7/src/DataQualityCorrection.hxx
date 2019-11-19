#ifndef DataQualityCorrection_h
#define DataQualityCorrection_h

#include "CorrectionBase.hxx"
#include <vector>
#include <utility>

/// This correction changes the data quality flag for certain run periods. The current
/// usage is for a period in run 1, when a FEB wasn't working properly (but good
/// DQ flags were uploaded to the DB).
///
/// The parameters in baseAnalysis.parameters.dat define the run periods for which
/// this correction should be applied.
class DataQualityCorrection: public CorrectionBase {
  public:

    /// Constructor, which reads in the parameter file to set up the run
    /// periods for which the correction should be applied.
    DataQualityCorrection();

    virtual ~DataQualityCorrection() {}

    /// Apply the variation to all tracks with TPC info.
    void Apply(AnaSpillC& spill);

  private:

    /// Join the base string with the pairing integer.
    std::string GetParamString(std::string base, int pairing);

    /// The run periods to apply this correction for. Pairing is starttime-endtime.
    std::vector<std::pair<int, int> > _time_periods;

    /// The new DQ flag to apply to the corresponding entry in _time_periods.
    std::vector<int> _new_values;
};

#endif
