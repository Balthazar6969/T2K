#include "PileUpSystematics.hxx"
#include "ND280AnalysisUtils.hxx"
#include "SystematicUtils.hxx"

/*
\\! [PileUpSystematics_info]
\htmlonly
There are a number of categories of possible pile up, but only the 
effect of sand muons is significant for the \(\nu_\mu\) analyses. For vertex 
selection in FGD1, events with TPC1 activity are rejected by the 
external veto cut because, in most cases, TPC1 activity is due to tracks 
from interactions upstream of the detector (sand muons) or outside the 
tracker fiducial volume. A quick study of MC events rejected due to the 
TPC1 veto indicates that the majority are not true CC interactions. 
Since sand muons are not included in the standard NEUT simulation, the 
Monte Carlo does not include the effect of events that are rejected due 
to coincidence with a~sand muon and a correction must be made.

<br/>
<br/>

For vertex selection in FGD2, an analagous cut is made to veto events 
with TPC2 activity. Therefore, the pileup correction and systematic is 
evaluated in an identical manner for TPC1 and TPC2, with TPC1 pileup 
applicable to selections with FGD1 vertices and TPC2 pileup applicable 
to selections with FGD2 vertices.

<br/>
<br/>

For vertex selection in FGD2, an analagous cut is made to veto events 
with TPC2 activity. Therefore, the pileup correction and systematic is 
evaluated in an identical manner for TPC1 and TPC2, with TPC1 pileup 
applicable to selections with FGD1 vertices and TPC2 pileup applicable 
to selections with FGD2 vertices.

<br/>
<br/>

Evaluation of this correction is made in an identical procedure to the 
2013 analysis (as described in T2K-TN-152~\cite{TN-152}). The correction 
is evaluated for each data set (Run~1, 2, 3b, 3c, 4, 5ab, 5c) separately 
for P0D water-in and water-out using the relevant MC samples for each 
period. (Note sand muons simulations are only available for water-out). 
The procedure is to count the number of TPC1 (TPC2) events \(NTPC_s\) in 
a separate sand muon Monte Carlo sample which relates to a fixed PoT, 
\(POT_s\). The data intensity \(I_{d} = POT/nSpills\), is then derived from 
the data sample and used to calculate the effective number of spills and 
hence the sand TPC1 (TPC2) events/bunch for that data set, where \(N_b\) is 
the number of bunches per spill (6 for Run 1, 8 otherwise). Flux tuning 
is applied using the 13a tuning from the 
nd5_tuned13av1.0_13anom_runX files.

Since the pile up is not taken into account in the Monte Carlo, the 
number of selected events should be reduced in the Monte Carlo. To do 
so, we re-weight the Monte Carlo with the following reduction factor,

\[
w_{c} = (1-C_{s})
\]

where \(C_{s}\) is the correction to be applied and defined as

\[
C_{s} = \frac{NTPC_s\times I_{d}}{POT_s\times N_b} 
\]


Since there is an uncertainty in the sand muon simulation of 10\% (for 
neutrinos, 30\% is taken for antineutrinos) and there are possible 
differences between the data and Monte Carlo arising from differences in 
the actual and simulated beam intensity and different material 
descriptions for side-band materials, there is a~systematic uncertainty 
on this pile-up contribution. The uncertainty is evaluated through 
a~data-MC comparison of the number of TPC1 (TPC2) events/bunch, with the 
sand muon contribution added to the MC. Explicitly, the data-MC 
difference is 

\[ 
\Delta_{\rm data:MC} = C_d - (C_{MC}+C_s) 
\]


where 

\[
C_{d}=\frac{NTPC_d}{nSpills\times N_b} 
\]
\[
C_{MC}=\frac{NTPC_{MC}\times I_{d}}{POT_{MC}\times N_b} . 
\]

The same procedure is applied to evaluate the number of 
TPC1 (TPC2) events per bunch for data and MC (MC is also weighted by the 
data intensity) and the \(\Delta_{\rm data:MC}\) value can then be taken 
as the uncertainty. However, the sand muon uncertainty means that there 
is a 10\% uncertainty (30\% for antineutrinos) in the correction factor, 
but combining the two uncertainties is double counting so the procedure 
is to take either \(\Delta_{\rm data:MC}\) or \(0.1\times C_{s}\) 
(\(0.3\times C_{s}\) for anti-neutrinos), whichever is larger, as 
\(\sigma_{\rm pileup}\). This systematic error is then propagated as the 
normalization error using Eq. \endhtmlonly \ref eq_normsyst \htmlonly


\[
w_{pileup} =  1+\alpha \cdot \sigma_{\rm pileup}
\]

where \(\alpha\)  is the random variation. 


\endhtmlonly
\\! [PileUpSystematics_info]
*/


//********************************************************************
PileUpSystematics::PileUpSystematics():EventWeightBase(1){
//********************************************************************

  _fgd1 = new BinnedParams("PileUpFGD1",BinnedParams::k1D_SYMMETRIC, versionUtils::Extension());
  _fgd2 = new BinnedParams("PileUpFGD2",BinnedParams::k1D_SYMMETRIC, versionUtils::Extension());
  SetNParameters(_fgd1->GetNBins()+_fgd2->GetNBins());
}

//********************************************************************
void PileUpSystematics::Initialize(){
//********************************************************************
  
  // Register the BinnedParams to have proper offsets for each of them
  systUtils::AddBinnedParamsOffsetToSystematic(*this, *_fgd1, _fgd1->GetNBins());
  systUtils::AddBinnedParamsOffsetToSystematic(*this, *_fgd2, _fgd2->GetNBins());
  
}

//********************************************************************
Weight_h PileUpSystematics::ComputeWeight(const ToyExperiment& toy, const AnaEventC& eventBB, const ToyBoxB& box, const SelectionBase& sel){
//********************************************************************

  Weight_h eventWeight;

  const AnaEventB& event = *static_cast<const AnaEventB*>(&eventBB); 

  
  // Get FV corresponding to the current branch
  SubDetId_h det = sel.GetDetectorFV(box.SuccessfulBranch);
  
  if (det != SubDetId::kFGD1 && det != SubDetId::kFGD2) return eventWeight;

  // Get the run period (from 0 to 8)
  Int_t runPeriod = anaUtils::GetRunPeriod(event.EventInfo.Run);
  
  // MainTrack should be in FV. This condition is already in box.DetectorFV
  //  if  (!anaUtils::InFiducialVolume(box.DetectorFV, box.MainTrack->PositionStart))  return eventWeight;
  
  // Get the pileup values for this run period
  Float_t pileup; // a correction
  Float_t pileup_error; // a systematic error
  Int_t index;
  if     (det == SubDetId::kFGD1){
    if(!_fgd1->GetBinValues(runPeriod, pileup, pileup_error, index)) return eventWeight;
    index += systUtils::GetBinnedParamsOffsetForSystematic(*this, *_fgd1);
  }
  else if(det  == SubDetId::kFGD2){
    if(!_fgd2->GetBinValues(runPeriod, pileup, pileup_error, index)) return eventWeight;
    index += systUtils::GetBinnedParamsOffsetForSystematic(*this, *_fgd2);
  }
  else return eventWeight;
  
  // compute the weight (corr + syst)
  //  eventWeight= 1 - pileup - pileup_error * toy.GetToyVariations(_index)->Variations[index];    // TO BE UNCOMMENTED
  //  eventWeight= 1 - (pileup_error * toy.GetToyVariations(_index)->Variations[index])/(1 - pileup);  // TO BE REMOVED  


  eventWeight.Systematic = 1 - pileup - pileup_error * toy.GetToyVariations(_index)->Variations[index];
  eventWeight.Correction = 1 - pileup;

  return eventWeight;
}

