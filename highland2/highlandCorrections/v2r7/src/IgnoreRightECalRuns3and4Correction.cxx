#include "IgnoreRightECalRuns3and4Correction.hxx"
#include "HighlandAnalysisUtils.hxx"
#include <VersioningUtils.hxx>
#include "Parameters.hxx"
#include <iostream>

//********************************************************************
IgnoreRightECalRuns3and4Correction::IgnoreRightECalRuns3and4Correction() :  CorrectionBase() {
  //********************************************************************
  _runs_MC.clear();
  _runs_Data.clear();

  std::string base = "highlandCorrections.IgnoreRightECal.Runs.";
  int pairing = 1;

  // Read in the MC run periods to apply the correction for
  std::string curr = GetParamString(base + "MC.Lower.", pairing);

  while (ND::params().HasParameter(curr)) {
    int lower = ND::params().GetParameterI(curr);
    curr = GetParamString(base + "MC.Upper.", pairing);
    int upper = ND::params().GetParameterI(curr);
    std::pair<int, int> p(lower, upper);
    _runs_MC.push_back(p);
    pairing++;
    curr = GetParamString(base + "MC.Lower.", pairing);
  }

  // Read in the Data run periods to apply the correction for
  pairing = 1;
  curr = GetParamString(base + "Data.Lower.", pairing);

  while (ND::params().HasParameter(curr)) {
    int lower = ND::params().GetParameterI(curr);
    curr = GetParamString(base + "Data.Upper.", pairing);
    int upper = ND::params().GetParameterI(curr);
    std::pair<int, int> p(lower, upper);
    _runs_Data.push_back(p);
    pairing++;
    curr = GetParamString(base + "Data.Lower.", pairing);
  }
}

//********************************************************************
std::string IgnoreRightECalRuns3and4Correction::GetParamString(std::string base, int pairing) {
  //********************************************************************
  std::stringstream ss;
  ss << base << pairing;
  return ss.str();
}

//********************************************************************
void IgnoreRightECalRuns3and4Correction::Apply(AnaSpillC& spillBB) {
  //********************************************************************

  AnaSpill& spill = *static_cast<AnaSpill*>(&spillBB);

  if (!ShouldApplyCorrection(spill)) {
    return;
  }

  for (unsigned int i = 0; i < spill.Bunches.size(); i++) {

    AnaTrackB* allEcalTracks[100];
    int nECAL = anaUtils::GetAllTracksUsingDet(*(static_cast<AnaBunch*>(spill.Bunches[i])), SubDetId::kECAL,  allEcalTracks);

    for (Int_t j = 0; j < nECAL; j++) {
      AnaTrackB* track = allEcalTracks[j];
      AnaECALParticleB* ECALSegments[100];
      int nECALSegments = 0;

      // Only keep the ECal segment if it's not in the Right ECal
      for (Int_t i=0;i<track->nECALSegments;i++){
        AnaECALParticleB* ecal = track->ECALSegments[i];

        if (ecal->Detector != SubDetId::kRightTECAL) {
          ECALSegments[i] = ecal;
          nECALSegments++;
          track->ECALSegments[i] = NULL;
          continue;
        }

        delete track->ECALSegments[i];    
        track->ECALSegments[i] = NULL;
      }

      track->nECALSegments = nECALSegments;
      for (Int_t i=0;i<nECALSegments;i++)
        track->ECALSegments[i] = ECALSegments[i];

      // should also change the detector bit so to remove the right ecal
      // unfortunately not way to change the kinematics of the end point in a consistent way
      unsigned long det = 0;
      for(UInt_t i = 0; i < SubDetId::kInvalidSubdetector; i++){
        SubDetId::SubDetEnum idet = static_cast<SubDetId::SubDetEnum>(i);

        if (idet == SubDetId::kRightTECAL)
          continue;

        if(!SubDetId::GetDetectorUsed(track->Detector, idet)) continue;

        SubDetId::SetDetectorUsed(det, idet);
      }

      SubDetId::SetDetectorSystemFields(det);

      track->Detector = det;


    }
  }
}

//********************************************************************
bool IgnoreRightECalRuns3and4Correction::ShouldApplyCorrection(AnaSpillC& spillBB) {
  //********************************************************************

  // Run 3 and 4 dead Right ECal TFBs are simulated in MC appropriately  for production 6
  // so there is no need to apply the correction in this case 
  if (versionUtils::prod6_corrections)
    return false;


  AnaSpill& spill = *static_cast<AnaSpill*>(&spillBB);

  int run = spill.EventInfo->Run;
  std::vector<std::pair<int, int> >& runs = spill.GetIsMC() ? _runs_MC : _runs_Data;

  for (unsigned int i = 0; i < runs.size(); i++) {
    if (run >= runs[i].first && run <= runs[i].second) {
      return true;
    }
  }

  return false;

}
