#include <stdio.h>
#include "TPCExpecteddEdxCorrection.hxx"
#include "HighlandAnalysisUtils.hxx"

//********************************************************************
TPCExpecteddEdxCorrection::TPCExpecteddEdxCorrection() : CorrectionBase() {
//********************************************************************
}

//********************************************************************
void TPCExpecteddEdxCorrection::Apply(AnaSpillC& spillBB) {
//********************************************************************

  AnaSpill& spill = *static_cast<AnaSpill*>(&spillBB);

  for (unsigned int i = 0; i < spill.Bunches.size(); i++) {
    AnaTrackB* allTpcTracks[100];
    AnaBunch* bunch = static_cast<AnaBunch*>(spill.Bunches[i]);
    int nTPC = anaUtils::GetAllTracksUsingDet(*bunch,  SubDetId::kTPC, allTpcTracks);
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
          tpcTrack->dEdxexpEle = anaUtils::ExpectedTPCdEdx(*tpcTrack, "electron");
          tpcTrack->dEdxexpMuon = anaUtils::ExpectedTPCdEdx(*tpcTrack, "muon");
          tpcTrack->dEdxexpProton = anaUtils::ExpectedTPCdEdx(*tpcTrack, "proton");
          tpcTrack->dEdxexpPion = anaUtils::ExpectedTPCdEdx(*tpcTrack, "pion");
	        tpcTrack->dEdxexpKaon = anaUtils::ExpectedTPCdEdx(*tpcTrack, "kaon");

        
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

