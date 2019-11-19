// Sam Short, Imperial College London (26 January 2013)       
// Class to get the flux tuning to apply to the magnet MC.  

#ifndef FluxTuning_h_seen
#define FluxTuning_h_seen

// ROOT headers:
#include "TObject.h"
#include "TFile.h"
#include "TH1D.h"

// C++ headers:
#include <iostream>
#include <vector>
#include <map>

class FluxTuning : public TObject{

public:
  // Constructor: specify the run period and if you want to use the fine tuning flux histograms.
  FluxTuning(std::string runperiod = "2", bool finetuning = false);
  virtual ~FluxTuning();

  // Get the weight for each true neutrino interaction by providing the neutrino pdg code and
  // true neutrino energy.
  double GetFluxTuningWeight(int true_nu_pdg, double true_nu_energy_in_MeV);

  ClassDef(FluxTuning,0);
private:

  TFile* fInputTuningFile;
  std::map<int,TH1D*> fTuningMap;

};

#endif
