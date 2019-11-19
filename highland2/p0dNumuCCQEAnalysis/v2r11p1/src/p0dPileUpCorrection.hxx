#ifndef p0dPileUpCorrection_h
#define p0dPileUpCorrection_h

#include "PileUpCorrection.hxx"

// This correction should only be applied to the p0dNumuCCQE
// selection, which is a single-track selection.  Because there are
// cosmics and sand muon tracks that occur in data but not MC, we need
// to weight the MC by a correction factor to account for this.
class p0dPileUpCorrection: public PileUpCorrection {
public:
  
  p0dPileUpCorrection();
  virtual ~p0dPileUpCorrection(){}
};

#endif
