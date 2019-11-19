#include "tutorialCorrection.hxx"
#include "ND280AnalysisUtils.hxx"
#include "VersioningUtils.hxx"
#include "DataClasses.hxx"


/* Corrections modify some aspect of the input data (real data or MC), such that data and MC match better each other. 
   The entire analysis will proceed on the modified data. 
   The difference with variation systematics is that this modification is based in very well understood studies (or true facts) 
   which allow us the change with confidence our data. Also the correction is the same for all toy experiments while variation systematics 
   will vary from one toy to another

   This particular correction moves forward the position of all global tracks by some mm, as specified in 
   an external data file. (tutorialAnalysis/vXrY/tutorialCorrection....dat). This correction is only applied to Real Data

   For real examples of corrections have a look at highlandCorrections/vXrY/src
*/


//********************************************************************
tutorialCorrection::tutorialCorrection() :  CorrectionBase(){
//********************************************************************

  // Discriminate between production 6B and production 5F
  char filename[300];
  if (versionUtils::prod6_corrections)
    sprintf(filename, "%s/data/tutorialCorrection_p6B.dat", getenv("TUTORIALANALYSISROOT"));
  else
    sprintf(filename, "%s/data/tutorialCorrection_p5F.dat", getenv("TUTORIALANALYSISROOT"));

  // Read the data file
  std::cout << " tutorial correction data " << filename << std::endl;
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
void tutorialCorrection::Apply(AnaSpillC& spillC) {
//********************************************************************

  // cast to AnaSpill type because AnaSpillC does not have event info
  AnaSpill& spill = *static_cast<AnaSpill*>(&spillC);

  // No correction for MC
  if (spill.GetIsMC()) return;

  // Get the run period (from 0 to 6)
  Int_t runPeriod = anaUtils::GetRunPeriod(spill.EventInfo->Run);

  // Need a valid runPeriod
  if (runPeriod<0 && runPeriod>=(Int_t)_nRunPeriods) return;

  // Move both the start and end position of all global tracks in all bunches
  for (UInt_t i = 0; i < spill.Bunches.size(); i++) {
    AnaBunchB* bunch = static_cast<AnaBunchB*>(spill.Bunches[i]);    
    for (UInt_t j = 0; j < bunch->Particles.size(); j++) {
      AnaTrack* track = static_cast<AnaTrack*>(bunch->Particles[j]);
      track->PositionStart[2] += _correction[runPeriod];
      track->PositionEnd[2]   += _correction[runPeriod];
    }
  }
      
}

