#include "NuParentDecayPosSystematics.hxx"
#include "AnalysisUtils.hxx"
#include "HEPConstants.hxx"
#include "Parameters.hxx"
#include "TFile.h"
#include "TRandom3.h"
#include "TH3.h"
#include <fstream>
#include <cassert>
#include "NuDirUtils.hxx"


//#define DEBUG


//********************************************************************
NuParentDecayPosSystematics::NuParentDecayPosSystematics(): EventVariationBase(0){
  //********************************************************************

  _RandomGenerator   = NULL;
  _DistributionHisto = NULL;

  InitializeRandomGenerator();


  std::string filename = getenv("HIGHLANDSYSTEMATICSROOT");
  filename += "/data/NuParentDecayPosDistr.root";
  
  std::ifstream infile(filename.c_str());
  assert(infile.good());
  
  // Read the histogram 
  TFile* file = new TFile(filename.c_str(), "READ");

  assert(file);

  _DistributionHisto = (TH3*)file->Get("NuParentDecayPos")->Clone();
 
  assert(_DistributionHisto);
  
  _DistributionHisto->ComputeIntegral();

  _DistributionHisto->SetDirectory(0);
  
    
  // Store the original
  anaUtils::CopyArray(anaUtils::NeutrinoParentDecay, _origNuParentDecayPos, 3);

  file->Close();

}


//********************************************************************
NuParentDecayPosSystematics::~NuParentDecayPosSystematics() {
  //********************************************************************
  if (_RandomGenerator)
    delete _RandomGenerator;

  _RandomGenerator = NULL;

  if (_DistributionHisto)
    delete _DistributionHisto;

  _DistributionHisto = NULL;

}


//********************************************************************
void NuParentDecayPosSystematics::InitializeRandomGenerator(){
//********************************************************************
  if (!_RandomGenerator){
    _RandomGenerator = new TRandom3();
    SetRandomSeed(ND::params().GetParameterI("highlandSystematics.NuParentDecayPos.RandomSeed")); 
  }
}

//********************************************************************
UInt_t NuParentDecayPosSystematics::GetRandomSeed() const{
//********************************************************************
  if (_RandomGenerator)
      return _RandomGenerator->GetSeed();
  return 0XDEADBEEF;
}
  
//******************************************************************** 
void NuParentDecayPosSystematics::SetRandomSeed(UInt_t seed){
//********************************************************************
  if (_RandomGenerator)
      _RandomGenerator->SetSeed(seed); 
}


//********************************************************************
void NuParentDecayPosSystematics::Apply(const ToyExperiment& toy, AnaEventC& event){
  //********************************************************************

  (void)event;
  (void)toy;
  
#ifdef DEBUG
  std::cout << " NuParentDecayPosSystematics::ApplyVariation(): begin "<< std::endl;  
#endif
  
  // Sample the nu parent decay point
  Float_t pos[3];
  
  if (!GetRandom(pos[0], pos[1], pos[2])) return; 
   
  // Apply the variation
  anaUtils::CopyArray(pos, anaUtils::NeutrinoParentDecay, 3);

#ifdef DEBUG
  std::cout << " NuParentDecayPosSystematics::ApplyVariation(): Sampled point X: " <<
    pos[0] << " Y: " << pos[1] << " Z: " << pos[2] << std::endl;  
  std::cout << " NuParentDecayPosSystematics::ApplyVariation(): end " << std::endl;  
#endif
}

//********************************************************************
bool NuParentDecayPosSystematics::UndoSystematic(AnaEventC& event){
  //********************************************************************

  (void)event;
  
  // Change the parameter in transverse anaUtils to its original value 
  anaUtils::CopyArray(_origNuParentDecayPos, anaUtils::NeutrinoParentDecay, 3);

  return false;
}

//********************************************************************
bool NuParentDecayPosSystematics::GetRandom(Float_t& x, Float_t& y, Float_t& z){
  //********************************************************************

  // There may be some reason why the ROOT function was copied,  but do not
  // remember
  
  if (!_DistributionHisto)
    return false;

  if  (!_RandomGenerator)
    return false;

  Int_t nbinsx = _DistributionHisto->GetNbinsX();
  Int_t nbinsy = _DistributionHisto->GetNbinsY();
  Int_t nbinsz = _DistributionHisto->GetNbinsZ();
  Int_t nxy    = nbinsx * nbinsy;
  Int_t nbins  = nxy * nbinsz;

  if (nbins == 0) return false; 

  // Get cumultive function (each bin gets the sum of contents of the predecessors + its own, normalized to one)
  Double_t* cIntegral = _DistributionHisto->GetIntegral();

  // Should be valid one
  if (cIntegral[nbins] == TMath::QuietNaN()) {return false;}

  // Generate the random number [0, 1]
  Double_t u =  _RandomGenerator->Rndm();

  // Get the bin, i.e. solve F(x) = u
  Int_t ibin = TMath::BinarySearch(nbins, cIntegral, (Double_t)u);
  Int_t binz = ibin / nxy;
  Int_t biny = (ibin - nxy * binz) / nbinsx;
  Int_t binx = ibin - nbinsx * (biny + nbinsy * binz);
  x = _DistributionHisto->GetXaxis()->GetBinLowEdge(binx + 1);
  if (u > cIntegral[ibin]) x +=
    _DistributionHisto->GetXaxis()->GetBinWidth(binx + 1) * (u - cIntegral[ibin]) / 
      (cIntegral[ibin + 1] - cIntegral[ibin]);
  y = _DistributionHisto->GetYaxis()->GetBinLowEdge(biny + 1) + _DistributionHisto->GetXaxis()->GetBinWidth(biny + 1) * 
    _RandomGenerator->Rndm();
  z = _DistributionHisto->GetZaxis()->GetBinLowEdge(binz + 1) + _DistributionHisto->GetXaxis()->GetBinWidth(binz + 1) * 
    _RandomGenerator->Rndm();

  return true;
}

