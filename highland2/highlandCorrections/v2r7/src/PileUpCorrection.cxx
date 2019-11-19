#include "PileUpCorrection.hxx"
#include "ND280AnalysisUtils.hxx"
#include "VersioningUtils.hxx"
#include "DataClasses.hxx"

//********************************************************************
PileUpCorrection::PileUpCorrection() :  CorrectionBase(){
//********************************************************************

  char filename[300];
  if (versionUtils::prod6_corrections)
    sprintf(filename, "%s/data/PileUpFGD1Correction_p6B.dat", getenv("HIGHLANDCORRECTIONSROOT"));
  else
    sprintf(filename, "%s/data/PileUpFGD1Correction_p5F.dat", getenv("HIGHLANDCORRECTIONSROOT"));

  std::cout << " PileUp correction data " << filename << std::endl;
  FILE *pFile = fopen(filename, "r");

  if (pFile == NULL) {
    printf("Cannot open file.\n");
    exit(1);
  }

  Int_t runPeriod;
  Float_t corr;

  _nRunPeriods=0;

  while (fscanf(pFile, "%d%f", &runPeriod, &corr) == 2) {
    _correction[runPeriod] = corr;
    _nRunPeriods++;
  }

  fclose(pFile);
}

//********************************************************************
void PileUpCorrection::Apply(AnaSpillC& spillBB) {
//********************************************************************

  AnaSpill& spill = *static_cast<AnaSpill*>(&spillBB);

  // No correction for data
  if (!spill.GetIsMC()) return;

  // Get the run period (from 0 to 6)
  Int_t runPeriod = anaUtils::GetRunPeriod(spill.EventInfo->Run);

  // Need a valid runPeriod
  if (runPeriod<0 && runPeriod>=(Int_t)_nRunPeriods) return;

  // Apply the corection
  for (UInt_t i = 0; i < spill.Bunches.size(); i++) 
    spill.Bunches[i]->Weight *= 1-_correction[runPeriod];
}

