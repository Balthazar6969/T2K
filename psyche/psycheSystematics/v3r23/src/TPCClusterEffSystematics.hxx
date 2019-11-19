#ifndef TPCClusterEffSystematics_h
#define TPCClusterEffSystematics_h

#include "EventWeightBase.hxx"
#include "BinnedParams.hxx"

/// This systematic affects the number of reconstructed nodes in an
/// AnaTpcSegment. The extra inefficiency for each virtual analysis is chosen
/// from a Gaussian, with mean and sigma specified by the user.
///
/// The correction for each AnaTpcSegment is then chosen from a uniform
/// distribution between 0 and the inefficiency for this virtual analysis.
class TPCClusterEffSystematics: public EventWeightBase, public BinnedParams {
public:
  
  /// Instantiate the TPC track quality systematic. nana is the number of
  /// virtual analyses to run. 
  TPCClusterEffSystematics();
  
  virtual ~TPCClusterEffSystematics() {}
  
  /// Apply the systematic to each AnaTpcSegment, varying the number of
  /// reconstructed nodes. See TPCClusterEffSystematics class
  /// documentation for details.
  Weight_h ComputeWeight(const ToyExperiment&, const AnaEventC&, const ToyBoxB&){return 1;}
  
  Weight_h ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box, const SelectionBase& sel);
};

#endif
