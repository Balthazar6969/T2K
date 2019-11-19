// Sam Short, Imperial College London (26 January 2013)
//
// Class to get the flux tuning to apply to the magnet MC.
//
// You need to get the most up to date flux tuning from t2k.org and change the file location (below).
// Then, in your script: make an instance of the object i.e. 
//   FluxTuning* tune1 = new FluxTuning("1",false); // ("run_number",fine tuning?)
//   FluxTuning* tune2 = new FluxTuning("2",false); 
//   FluxTuning* tune3 = new FluxTuning("3",false); 
// Then, for each true neutrino interaction you do:
//   double fluxweight1 = tune1->GetFluxTuningWeight(true_neutrino_pdg_code, true_neutrino_energy_in_mev);
//   double fluxweight2 = tune2->GetFluxTuningWeight(true_neutrino_pdg_code, true_neutrino_energy_in_mev);
//   double fluxweight3 = tune3->GetFluxTuningWeight(true_neutrino_pdg_code, true_neutrino_energy_in_mev);
// Then, you can weight each fluxweight according to the data POT and apply it when plotting histograms:
//   hist->Fill(value,fluxweight);

#include "FluxTuning.h"

// ROOT etc.
#include "TTree.h"

// C headers
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <iomanip>



// ---------------------------------------------------------
FluxTuning::FluxTuning(std::string runperiod, bool finetuning){

  // Find the file and decide which histogram:
  std::string location = "/data/short/t2k/software/nd280/v11r29/highland2_psyche/flux/tuned13av1.1/";
  if(runperiod == "2"){
    location += "run2/";
    if(finetuning){location += "nd5_tuned13av1.1_13anom_run2_fine.root";}
    else{          location += "nd5_tuned13av1.1_13anom_run2.root";}
  }
  else{std::cout<<"Must supply a valid run period for flux tuning."<<std::endl; exit(1);}

  // Open the file:
  fInputTuningFile = new TFile(location.c_str(),"READ");

  // Get the histograms:
  fTuningMap[14]  = (TH1D*)(fInputTuningFile->Get("enu_nd5_tuned13a_numu_ratio"));
  fTuningMap[-14] = (TH1D*)(fInputTuningFile->Get("enu_nd5_tuned13a_numub_ratio"));
  fTuningMap[12]  = (TH1D*)(fInputTuningFile->Get("enu_nd5_tuned13a_nue_ratio"));
  fTuningMap[-12] = (TH1D*)(fInputTuningFile->Get("enu_nd5_tuned13a_nueb_ratio"));

}

// ---------------------------------------------------------
FluxTuning::~FluxTuning(){
  for(std::map<int,TH1D*>::iterator iter = fTuningMap.begin();
      iter != fTuningMap.end(); iter++){
    delete (iter->second);
  }
  fInputTuningFile->Close();
}  

// ---------------------------------------------------------
double FluxTuning::GetFluxTuningWeight(int pdg, double energy){

  if(pdg != 14 && pdg != -14 && pdg != 12 && pdg != -12){
    std::cout<<"ERROR! FluxTuning::GetFluxTuningWeight("<<pdg<<","<<energy<<"). "
	     <<"You must supply a valid neutrino PDG code"<<std::endl;
    exit(1);
  }

  int flux_bin = fTuningMap[pdg]->FindBin(energy/1000); // Energy stored in GeV in histogram.
  double weight = fTuningMap[pdg]->GetBinContent(flux_bin);

  return weight;
}
