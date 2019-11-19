#include "UseGlobalAltMomCorrection.hxx"
#include "DataClasses.hxx"
#include "HighlandAnalysisUtils.hxx"

//********************************************************************
UseGlobalAltMomCorrection::UseGlobalAltMomCorrection(Hypothesis hypothesis) :  CorrectionBase() {
//********************************************************************
  _hyp = hypothesis;
}

//********************************************************************
void UseGlobalAltMomCorrection::Apply(AnaSpillC& spillBB) {
//********************************************************************

  AnaSpill& spill = *static_cast<AnaSpill*>(&spillBB);

  for (unsigned int i = 0; i < spill.Bunches.size(); i++) {
    AnaTrackB* tracks[100];
    AnaBunch* bunch = static_cast<AnaBunch*>(spill.Bunches[i]);
    int nTPC = anaUtils::GetAllTracksUsingDet(*bunch, SubDetId::kTPC,  tracks);
    for (Int_t j = 0; j < nTPC; j++) {
      AnaTrack* track = static_cast<AnaTrack*>(tracks[j]);

      if (_hyp == kMuon) {
        track->Momentum = track->MomentumMuon;
        track->MomentumError = track->MomentumErrorMuon;
      } else if (_hyp == kElectron) {
        track->Momentum = track->MomentumEle;
        track->MomentumError = track->MomentumErrorEle;
      } else if (_hyp == kProton) {
        track->Momentum = track->MomentumProton;
        track->MomentumError = track->MomentumErrorProton;
      }
    }

    // TODO. Removed because AnaParticle does not have momentum now
    // Sort all particles by momentum after correction (using GetAllTracks the sorting doesn't work!)
    //    std::sort(spill.Bunches.at(i)->Particles.begin(), spill.Bunches.at(i)->Particles.end(), AnaTrack::CompareMomentum);

  }
  /*

  // Sort all particles constituents of each global vertex by momentum after correction
  // to be done after filling all bunches (because there might be associated particles from other bunches)
  for (unsigned int i = 0; i < spill.Bunches.size(); i++) {
    std::vector<AnaVertexB*> allVertices = (*(static_cast<AnaBunch*>(spill.Bunches.at(i)))).Vertices;
    for (unsigned int j = 0; j < allVertices.size(); j++)
      std::sort(allVertices.at(j)->Particles, allVertices.at(j)->Particles+allVertices.at(j)->nParticles, AnaTrack::CompareMomentum);
  }

  // Sort all particles associated to each true vertex by momentum after correction
  std::vector<AnaTrueVertexB*> allTrueVertices = spill.TrueVertices;
  for (unsigned int j = 0; j < allTrueVertices.size(); j++){
    AnaTrueVertex * tvertex = static_cast<AnaTrueVertex*>(allTrueVertices.at(j));
    std::sort(tvertex->ReconParticles.begin(), tvertex->ReconParticles.end(), AnaTrack::CompareMomentum);
  }

  // Sort all particles associated to each true particle by momentum after correction
  std::vector<AnaTrueParticleB*> allTrueParticles = spill.TrueParticles;
  for (unsigned int j = 0; j < allTrueParticles.size(); j++){
    AnaTrueParticle * tparticle = static_cast<AnaTrueParticle*>(allTrueParticles.at(j));
    std::sort(tparticle->ReconParticles.begin(), tparticle->ReconParticles.end(), AnaTrack::CompareMomentum);
  }
  */
}

