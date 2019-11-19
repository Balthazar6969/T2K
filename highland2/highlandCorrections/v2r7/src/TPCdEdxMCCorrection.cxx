#include <stdio.h>
#include "TPCdEdxMCCorrection.hxx"
#include "HighlandAnalysisUtils.hxx"
#include "VersioningUtils.hxx"

//********************************************************************
TPCdEdxMCCorrection::TPCdEdxMCCorrection():CorrectionBase(){
//********************************************************************

}

//********************************************************************
double TPCdEdxMCCorrection::GetCorrection(int pdg) {
//********************************************************************
  double ccorr = 1.;

  if (abs(pdg) == 11) {
    if (versionUtils::prod6_corrections)
      // in prod6 MC overestimates the energy loss of true electrons by 1%
      ccorr = ccorr * 1.01;
    else
      // in prod5 MC underestimates the energy loss of true electrons by 4%
      ccorr = ccorr / 1.04;
  }

  return ccorr;
}

//********************************************************************
void TPCdEdxMCCorrection::Apply(AnaSpillC& spillBB){
//********************************************************************

  AnaSpill& spill = *static_cast<AnaSpill*>(&spillBB);

  // No correction for data
  if (!spill.GetIsMC())
    return;

  for (unsigned int i = 0; i < spill.Bunches.size(); i++) {
    AnaTrackB* allTpcTracks[100];
    AnaBunch* bunch = static_cast<AnaBunch*>(spill.Bunches[i]);
    int nTPC = anaUtils::GetAllTracksUsingDet(*bunch, SubDetId::kTPC,  allTpcTracks);
    for (Int_t j = 0; j < nTPC; j++) {
      for (int k = 0; k < allTpcTracks[j]->nTPCSegments; k++) {

        // The raw TPC track
        AnaTPCParticleB* original = static_cast<const AnaTrackB*>(allTpcTracks[j]->Original)->TPCSegments[k];
        // The corrected TPC track
        AnaTPCParticle* tpcTrack = static_cast<AnaTPCParticle*>(allTpcTracks[j]->TPCSegments[k]);

        // Get the raw CT
        double CT0 = original->dEdxMeas;

        // Apply the correction
        if (CT0 != 999999) {
          // Apply correction only if the CT is a valid number.
          int pdg = 0;
          if (tpcTrack->TrueObject) {
            pdg = tpcTrack->GetTrueParticle()->PDG;
          }

          tpcTrack->dEdxMeas = CT0 / GetCorrection(pdg);
 
          //recompute TPC pulls
          Float_t pulls[4];
          anaUtils::ComputeTPCPull(*tpcTrack, pulls);
          
          tpcTrack->Pullele = pulls[1];
          tpcTrack->Pullmu  = pulls[0];
          tpcTrack->Pullp   = pulls[2];
          tpcTrack->Pullpi  = pulls[3];
          
          //for the kaon need to recompute explicitely   
          tpcTrack->Pullk = ((tpcTrack->dEdxMeas - tpcTrack->dEdxexpKaon) / tpcTrack->dEdxSigmaKaon); 
        
        }
      }
    }
  }
}

