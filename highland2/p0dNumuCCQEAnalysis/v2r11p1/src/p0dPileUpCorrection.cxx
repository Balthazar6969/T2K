#include "p0dPileUpCorrection.hxx"

//********************************************************************
p0dPileUpCorrection::p0dPileUpCorrection() {
//********************************************************************
  char filename[300];
  sprintf(filename, "%s/data/PileUpP0DCCQECorrection.dat", getenv("P0DNUMUCCQEANALYSISROOT"));

  std::cout << " P0D-PileUp correction data " << filename << std::endl;
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
