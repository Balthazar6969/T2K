#include <stdio.h>
#include "TPCdEdxDataCorrection.hxx"
#include "HighlandAnalysisUtils.hxx"
#include "VersioningUtils.hxx"

//********************************************************************
TPCdEdxDataCorrection::TPCdEdxDataCorrection():CorrectionBase(){
//********************************************************************

  char filename[300];
  if (versionUtils::prod6_corrections)
    sprintf(filename, "%s/data/TPCdEdxCorrection_p6B.dat", getenv("HIGHLANDCORRECTIONSROOT"));
  else
    sprintf(filename, "%s/data/TPCdEdxCorrection_p5F.dat", getenv("HIGHLANDCORRECTIONSROOT"));
  

  std::cout << " CT correction data " << filename << std::endl;
  FILE *pFile = fopen(filename, "r");

  if (pFile == NULL) {
    printf("Cannot open file.\n");
    exit(1);
  }

  int run;
  int subrun;

  float c[3]; // CT_expected -CT_measured
  float ec[3]; // error on CT_expected - CT_measured

  while (fscanf(pFile, "%d%d%f%f%f%f%f%f", &run, &subrun, &c[0], &ec[0], &c[1], &ec[1], &c[2], &ec[2]) == 8) {
    comb = std::make_pair(run, subrun);
    corrTPC1[comb] = c[0];
    corrTPC2[comb] = c[1];
    corrTPC3[comb] = c[2];
  }

  itr = corrTPC1.begin();
  fclose(pFile);
}

//********************************************************************
double TPCdEdxDataCorrection::GetCorrection(int run, int subrun, unsigned long det) {
//********************************************************************

  comb = std::make_pair(run, subrun);

  for (; itr != corrTPC1.end(); itr++) {
    if (comb.first * 1000 + comb.second >= itr->first.first * 1000 + itr->first.second) {
      itr++;

      if (comb.first * 1000 + comb.second < itr->first.first * 1000 + itr->first.second) {
        itr--;
        comb0 = std::make_pair(itr->first.first, itr->first.second);

        if (SubDetId::GetSubdetectorEnum(det) == SubDetId::kTPC1)
          return corrTPC1[comb0];
        else if (SubDetId::GetSubdetectorEnum(det) == SubDetId::kTPC2)
          return corrTPC2[comb0];
        else if (SubDetId::GetSubdetectorEnum(det) == SubDetId::kTPC3)
          return corrTPC3[comb0];
      } else {
        itr--;
        continue;
      }
    }

    if (itr == corrTPC1.begin())
      break;
  }

  std::cout << "combination not found!!!!!" << run << "  " << subrun << std::endl;
  itr = corrTPC1.begin();

  return GetCorrection(run, subrun, det);
}


//********************************************************************
void TPCdEdxDataCorrection::Apply(AnaSpillC& spillBB){
//********************************************************************

  AnaSpill& spill = *static_cast<AnaSpill*>(&spillBB);

  // No correction for MC
  if (spill.GetIsMC())
    return;

  Int_t subrun = spill.EventInfo->SubRun;

  for (unsigned int i = 0; i < spill.Bunches.size(); i++) {
    AnaBunch* bunch = static_cast<AnaBunch*>(spill.Bunches[i]);
    AnaTrackB* allTpcTracks[100];
    int nTPC = anaUtils::GetAllTracksUsingDet(*bunch,SubDetId::kTPC,   allTpcTracks);
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
          tpcTrack->dEdxMeas = CT0 / GetCorrection(spill.EventInfo->Run, subrun, tpcTrack->Detector);
          
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
