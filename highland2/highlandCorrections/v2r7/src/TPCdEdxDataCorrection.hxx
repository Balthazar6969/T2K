#ifndef TPCdEdxDataCorrection_h
#define TPCdEdxDataCorrection_h

#include "CorrectionBase.hxx"
#include "DataClasses.hxx"

/// This class defines a correction that affects the measured dE/dx of an
/// AnaTpcSegment. This in turn affects the computed pulls (how electron-like a
/// track is, for example).
///
/// This correction is applied to data, and is based on the run/subrun of this
/// spill.
///
/// The corrections are defined in the baseAnalysis/data/TPCdEdxCorrection.dat.
/// The format of this file is
///
/// run subrun corr_tpc1 err_tpc1 corr_tpc2 err_tpc2 corr_tpc3 err_tpc3
///
/// where corr_tpcN is the CT correction to apply to TPC N, and err_tpcN is the
/// error on that correction.
class TPCdEdxDataCorrection: public CorrectionBase {
  public:

    /// Constructor, which reads the corrections from the text file, and fills
    /// the member variables with the details.
    TPCdEdxDataCorrection();

    virtual ~TPCdEdxDataCorrection() {}

    /// Apply the dE/dx correction to all the AnaTpcSegment members of all
    /// tracks in the spill. See TPCdEdxDataCorrection class documentation for
    /// details.
  void Apply(AnaSpillC& spill);

  private:

    /// Get the correction for the given run, subrun and TPC combination.
    double GetCorrection(int run, int subrun, unsigned long det);

    /// Current run-subrun pair.
    std::pair<int, int> comb;

    /// Temporary run-subrun pair.
    std::pair<int, int> comb0;

    /// Corrections read for TPC1. (run, subrun)->correction.
    std::map<std::pair<int, int>, double> corrTPC1;

    /// Corrections read for TPC2. (run, subrun)->correction.
    std::map<std::pair<int, int>, double> corrTPC2;

    /// Corrections read for TPC3. (run, subrun)->correction.
    std::map<std::pair<int, int>, double> corrTPC3;

    /// Iterator for looping over the data that was read in.
    std::map<std::pair<int, int>, double>::iterator itr;

};

#endif
