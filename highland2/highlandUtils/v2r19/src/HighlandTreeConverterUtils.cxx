#include "HighlandTreeConverterUtils.hxx"

//*****************************************************************************
bool anaUtils::FillTrueVertexRooInfo(AnaTrueVertexB* vertexB, Int_t RunID, bool Neut, bool Genie, 
                                     Int_t NNVtx, TClonesArray* NVtx, Int_t NGVtx, TClonesArray* GVtx, 
                                     bool& foundCohOnH, bool& foundPauliBlocked ) {
//*****************************************************************************

// TODO: a template could avoid the doubled code for genie and neut:
// template<class T>
// void HandleRooTrackerVertex(T *RooTrackerVtx){
//   Do stuff with data members that are identical between N/GRooTrackerVtx
// }
// ND::RooTrackerVtxBase lvtx = (ND::RooTrackerVtxBase) (*NVtx)[roov];
// if (Neut)  HandleRooTrackerVertex(static_cast<(ND::NRooTrackerVtx) (NVtx)[roov]>(lvtx));
// if (Genie) HandleRooTrackerVertex(static_cast<(ND::GRooTrackerVtx) (NVtx)[roov]>(lvtx));
//
// NOTE: This was done in xsTool, check $XSTOOLROOT/src/xsGenerator.hxx

  AnaTrueVertex* vertex = static_cast<AnaTrueVertex*>(vertexB);

  if ( ! Neut && ! Genie) return true;

  std::stringstream ssRun; ssRun << RunID;
  bool issand = ((ssRun.str())[4]=='7');

  bool foundVertex = false;
  bool foundLepton = false;
  int leptonPDG_StdHep = 0;
  TLorentzVector leptonMom_StdHep;
  TLorentzVector neutrinoMom_StdHep;

  if (Neut) {
    for (int roov = 0; roov < NNVtx; roov++) {
      ND::NRooTrackerVtx *lvtx = (ND::NRooTrackerVtx*) (*NVtx)[roov];

      // this is needed otherwise when running with highland for pro6 over a prod5 file
      // it crashes before doing the versioning check (as far as IsMC is taken reading the first entry)
      if ( ! lvtx->StdHepPdg) continue;

      // look for the current vertex
      if (vertex->ID != lvtx->TruthVertexID) continue;

      foundVertex = true;
      neutrinoMom_StdHep = lvtx->StdHepP4[0]; // needed for Q2 later
      vertex->RooVtxIndex = roov;
      vertex->NuParentPDG = lvtx->NuParentPdg;
      anaUtils::CopyArray(lvtx->NuParentDecX4,vertex->NuParentDecPoint,4);

      // tag coherent interaction on hydrogen, wrongly produced because of a bug in neut,
      // both in prod 5 and 6, see bugzilla 1056:
      // the related reconstructed tracks will be discarded, as well as these true vertices
      if (lvtx->StdHepPdg[1] == 1000010010) { // hit nucleus is hydrogen
        if (abs(vertex->ReacCode) == 16 || abs(vertex->ReacCode) == 36) { // coherent interactions
          foundCohOnH = true;
          vertex->IsCohOnH = true;
        } else if (lvtx->StdHepPdg[2] == 1000000010) // should happens only for coh on H
             std::cout << "minor error in oaAnalysisConverter (ref 1056)" << std::endl;
      }

      // tag Pauli blocked vertices
      // there are single gamma and 1pi Pauli-blocked events without outgoing particles (see bugzilla 1011), but in prod 5
      // for a neut bug, only the lepton was set as not-trackable (NEiflgvc == 5), while the other particles were saved:
      // the related reconstructed tracks will be discarded, as well as these true vertices
      if (lvtx->NEipvc && lvtx->NEipvc[0] != -1) { //otherwise it is NuWro or sand muons prod 5
        int leptonIndex = 2;
        if (abs(vertex->ReacCode) == 2) leptonIndex = 3; // lepton is shifted for 2p2h that have an extra row for the 2nd nucleon
        if (abs(lvtx->NEipvc[leptonIndex]) > 10 && abs(lvtx->NEipvc[leptonIndex]) < 19) { // it is the primary lepton
          if (lvtx->NEiflgvc[leptonIndex] == 5) vertex->IsPauliBlocked = true; // the primary lepton is set as not trackable
        } else {
          std::cout << "ERROR in NRooTrackerVtx: (see bug 1090) the particle with the neutrino as parent is not a lepton, PDG code " << lvtx->NEipvc[leptonIndex] << ", in ID " << vertex->ID <<std::endl;
        }
/*
        // look into pre-FSI vars (it works, but need to check how much the file size increases)
        // store in PreFSIParticles particles escaping the nucleus w/o being affected by FSI (other than the lepton, for which FSI in not applied)
        for (int i = 0; i < lvtx->NEnvc; i++) {
          // only consider particle escaping the nucleus
          if (lvtx->NEicrnvc[i] == 0 ) continue;
          // consider only particle pre-FSI, i.e. the parent is the hit nucleon (nucleons if 2p2h)
          if (lvtx->NEiorgvc[i] == 2 || (abs(vertex->ReacCode) == 2 && lvtx->NEiorgvc[i] == 3)) {
            Float_t thispdg = (Float_t)lvtx->NEipvc[i];
            TLorentzVector thisMomentum = lvtx->NEpvc[i]; // already in MeV
            Float_t thismom = thisMomentum.Vect().Mag();
            Float_t thiscostheta = -999;
            if (thismom > 0) thiscostheta = 1/thismom*thisMomentum.Z();
            TVector3 thistraj(thispdg,thismom,thiscostheta);
            vertex->PreFSIParticles.push_back(thistraj);
          }
        }
*/
      }

      // loop over StdHep
      for (int i = 0; i < lvtx->StdHepN; i++) {

        int outgoingCode = 1; // 0: initial state particle (incoming); 1: stable final state outgoing (GENIE has also other values for intermediate states)
        if (issand) outgoingCode = 2; // 0: idem; 2: stable final state outgoing; 1: reach nd280 (listed again, see bug 1128); no sand for genie

        if (lvtx->StdHepStatus[i] != outgoingCode) continue;

        // fill NPrimaryParticles: count the primary particles of each type (i.e. particles escaping the nucleus)
        int index = ParticleId::GetParticle((int)lvtx->StdHepPdg[i],false);
        vertex->NPrimaryParticles[index]++;
        if (abs(lvtx->StdHepPdg[i]) > 1000 && abs(lvtx->StdHepPdg[i]) < 10000) vertex->NPrimaryParticles[ParticleId::kBaryons]++;
        if (abs(lvtx->StdHepPdg[i]) > 100 && abs(lvtx->StdHepPdg[i]) < 1000) vertex->NPrimaryParticles[ParticleId::kMesons]++;
        if (abs(lvtx->StdHepPdg[i]) > 10 && abs(lvtx->StdHepPdg[i]) < 19) vertex->NPrimaryParticles[ParticleId::kLeptons]++;
        if (lvtx->StdHepPdg[i] == +12 || lvtx->StdHepPdg[i] == +14 || lvtx->StdHepPdg[i] == +16) vertex->NPrimaryParticles[ParticleId::kNeutrinos]++;
        if (lvtx->StdHepPdg[i] == -12 || lvtx->StdHepPdg[i] == -14 || lvtx->StdHepPdg[i] == -16) vertex->NPrimaryParticles[ParticleId::kAntiNeutrinos]++;

        // primary lepton
        if ( ! foundLepton && lvtx->StdHepStatus[i] == outgoingCode) { // the lepton is the first with Status = 1
          if (abs(lvtx->StdHepPdg[i]) > 10 && abs(lvtx->StdHepPdg[i]) < 19) { // it is the primary lepton; this is needed for bug 1090
            foundLepton = true;
            leptonPDG_StdHep = lvtx->StdHepPdg[i];
            leptonMom_StdHep = lvtx->StdHepP4[i];
          }
        }

        // fill true proton vars
        if (lvtx->StdHepPdg[i] == 2212) {
          // if many, take the first one, that more likely should be the interacted one
          // 08/02/2017[AI], the above treatment is confusing for the user (e.g. if
          // one tries to use the info to unfold to, do studies etc) and the
          // underlying assumptions in sorting  may be different between
          // generators + we already use the info of the post FSI proton
          // so changed to use the one with highest momentum
          TLorentzVector thisMomentum = lvtx->StdHepP4[i];
          Double_t p = thisMomentum.Vect().Mag() * 1000.; // converting from GeV to MeV

          if (vertex->ProtonMom < p) {
            vertex->ProtonMom = p;
           
            if (vertex->ProtonMom > 0){
                anaUtils::VectorToArray((1. / vertex->ProtonMom) * thisMomentum.Vect(), vertex->ProtonDir);
            }  
          }
        }

        // fill true pion vars
        if (lvtx->StdHepPdg[i] == 211) {
          // if many, take the highest momentum one, that should be most likely to be reconstructed
          TLorentzVector thisMomentum = lvtx->StdHepP4[i];
          Double_t p = thisMomentum.Vect().Mag()*1000; // converting from GeV to MeV
          if (vertex->PionMom < p) {
            vertex->PionMom = p;
            if (vertex->PionMom > 0)
              anaUtils::VectorToArray((1. / vertex->PionMom) * thisMomentum.Vect(), vertex->PionDir);
          }
        }

      } // end loop over StdHep

      break;
    } // end loop over vertices

  } // end if Neut

  else if (Genie) {
    for (int roov = 0; roov < NGVtx; roov++) {
      ND::GRooTrackerVtx *lvtx = (ND::GRooTrackerVtx*) (*GVtx)[roov];

      // this is needed otherwise when running with highland for pro6 over a prod5 file
      // it crashes before doing the versioning check
      if ( ! lvtx->StdHepPdg) continue;

      // look for the current vertex
      if (vertex->ID != lvtx->TruthVertexID) continue;

      foundVertex = true;
      neutrinoMom_StdHep = lvtx->StdHepP4[0]; // needed for Q2 later
      vertex->RooVtxIndex = roov;
      vertex->NuParentPDG = lvtx->NuParentPdg;
      anaUtils::CopyArray(lvtx->NuParentDecX4,vertex->NuParentDecPoint,4);
//      if (lvtx->G2NeutEvtCode != vertex->ReacCode) // it happens often!
      vertex->ReacCode = lvtx->G2NeutEvtCode;

      // loop over StdHep
      for (int i = 0; i < lvtx->StdHepN; i++) {

        if (lvtx->StdHepStatus[i] != 1) continue; // 0 is incoming particle, 1 for outgoing (GENIE has also other values for intermediate states)

        // fill NPrimaryParticles: count the primary particles of each type
        int index = ParticleId::GetParticle((int)lvtx->StdHepPdg[i],false);
        vertex->NPrimaryParticles[index]++;
        if (abs(lvtx->StdHepPdg[i]) > 1000 && abs(lvtx->StdHepPdg[i]) < 10000) vertex->NPrimaryParticles[ParticleId::kBaryons]++;
        if (abs(lvtx->StdHepPdg[i]) > 100 && abs(lvtx->StdHepPdg[i]) < 1000) vertex->NPrimaryParticles[ParticleId::kMesons]++;
        if (abs(lvtx->StdHepPdg[i]) > 10 && abs(lvtx->StdHepPdg[i]) < 19) vertex->NPrimaryParticles[ParticleId::kLeptons]++;
        if (lvtx->StdHepPdg[i] == +12 || lvtx->StdHepPdg[i] == +14 || lvtx->StdHepPdg[i] == +16) vertex->NPrimaryParticles[ParticleId::kNeutrinos]++;
        if (lvtx->StdHepPdg[i] == -12 || lvtx->StdHepPdg[i] == -14 || lvtx->StdHepPdg[i] == -16) vertex->NPrimaryParticles[ParticleId::kAntiNeutrinos]++;

        // this is the primary lepton
        // in Genie the first with Status 1 is not enough, it might be listed later, but it's always the one with StdHepFm = 0
        if ( ! foundLepton && lvtx->StdHepStatus[i] == 1 && lvtx->StdHepFm[i] == 0) {
          foundLepton = true;
          leptonPDG_StdHep = lvtx->StdHepPdg[i];
          leptonMom_StdHep = lvtx->StdHepP4[i];
        }

        // fill true proton vars
        if (lvtx->StdHepPdg[i] == 2212) {
          // if many, take the first one, that more likely should be the interacted one
          // 08/02/2017[AI], the above treatment is confusing for the user (e.g. if
          // one tries to use the info to unfold to, do studies etc) and the
          // underlying assumptions in sorting  may be different between
          // generators + we already use the info of the post FSI proton
          // so changed to use the one with highest momentum
          TLorentzVector thisMomentum = lvtx->StdHepP4[i];
          Double_t p = thisMomentum.Vect().Mag() * 1000.; // converting from GeV to MeV

          if (vertex->ProtonMom < p) {
            vertex->ProtonMom = p;
           
            if (vertex->ProtonMom > 0){
                anaUtils::VectorToArray((1. / vertex->ProtonMom) * thisMomentum.Vect(), vertex->ProtonDir);
            }  
          } 
        }

        // fill true pion vars
        if (lvtx->StdHepPdg[i] == 211) {
          // if many, take the highest momentum one, that should be most likely to be reconstructed
          TLorentzVector thisMomentum = lvtx->StdHepP4[i];
          Double_t p = thisMomentum.Vect().Mag()*1000; // converting from GeV to MeV
          if (vertex->PionMom < p) {
            vertex->PionMom = p;
            if (vertex->PionMom > 0)
              anaUtils::VectorToArray((1. / vertex->PionMom) * thisMomentum.Vect(), vertex->PionDir);
          }
        }

      } // end loop StdHep

      break;
    } // end loop over vertices

  } // end if Genie

  if ( ! foundVertex) {
    std::cout << "ERROR: true vertex not found in RooTrackerVtx!!! ID = " << vertex->ID << std::endl;
    return true;
  }

  // Fill NPrimaryParticles for kPions and Kaons
  vertex->NPrimaryParticles[ParticleId::kPions] = vertex->NPrimaryParticles[ParticleId::kPi0]   +
                                                  vertex->NPrimaryParticles[ParticleId::kPiPos] +
                                                  vertex->NPrimaryParticles[ParticleId::kPiNeg] ;
  vertex->NPrimaryParticles[ParticleId::kKaons] = vertex->NPrimaryParticles[ParticleId::kK0]     +
                                                  vertex->NPrimaryParticles[ParticleId::kAntiK0] +
                                                  vertex->NPrimaryParticles[ParticleId::kK0L]    +
                                                  vertex->NPrimaryParticles[ParticleId::kK0S]    +
                                                  vertex->NPrimaryParticles[ParticleId::kKPos]   +
                                                  vertex->NPrimaryParticles[ParticleId::kKNeg]   ;

  if (vertex->NPrimaryParticles[ParticleId::kMuon] > 1) std::cout << "INFO: lepton pair produced by FSI: Nmuon " << vertex->NPrimaryParticles[ParticleId::kMuon] << ", Nantimuon " << vertex->NPrimaryParticles[ParticleId::kAntiMuon] << ", NuPDG " << vertex->NuPDG << ", ReacCode " << vertex->ReacCode << ", in detector " << vertex->Detector << std::endl;

  // no lepton --> Pauli blocked event
  if ( ! foundLepton || abs(leptonPDG_StdHep) > 18) {
    // double-check that this is a Pauli blocked vertex
    if (vertex->IsPauliBlocked &&
        (abs(vertex->ReacCode) == 11 || abs(vertex->ReacCode) == 12 || abs(vertex->ReacCode) == 13 || // 1pi nu/antinu CC
         abs(vertex->ReacCode) == 31 || abs(vertex->ReacCode) == 32 || abs(vertex->ReacCode) == 33 || abs(vertex->ReacCode) == 34  || // 1pi nu/antinu NC
         abs(vertex->ReacCode) == 17 || abs(vertex->ReacCode) == 38 || abs(vertex->ReacCode) == 39 ) // single gamma from delta resonance nu/antinu
       ) {
      foundPauliBlocked = true;
//      std::cout << "INFO: this is a Pauli blocked vertex (single gamma or 1pi), ReacCode " << vertex->ReacCode << std::endl; // TOO MANY!

    } else {
      if ( ! issand) // suppress for sand muons even though it's not clear why it happens: see bug 1136
         std::cout << "ERROR in oaAnalysisConverter (ignore for run 91300010-0028 since it's caused by bug 1368): primary lepton in StdHepPdg with PDG code "
                   << leptonPDG_StdHep << " (0='not found') for ID " << vertex->ID <<std::endl;
    }
  }


  // fill DataClasses members
  neutrinoMom_StdHep = neutrinoMom_StdHep*1000; // converting from GeV to MeV
  neutrinoMom_StdHep[3] = sqrt(pow(neutrinoMom_StdHep[0],2)+pow(neutrinoMom_StdHep[1],2)+pow(neutrinoMom_StdHep[2],2));
  // not sure why (likely float casting) but StdHepP4[vertex][0][3] is not equal to the sqrt of the power of the other 3 components
  // while Vertices.NeutrinoMomentum.E() it is! (evidently it is re-calculated in TruthTrajectoriesModule of oaAnalysis)
  vertex->NuEnergy = neutrinoMom_StdHep[3];
  if (foundLepton) {
    leptonMom_StdHep = leptonMom_StdHep*1000; // converting from GeV to MeV
    vertex->LeptonPDG = leptonPDG_StdHep;
    vertex->Q2 = - (leptonMom_StdHep - neutrinoMom_StdHep).Mag2();
    vertex->LeptonMom = leptonMom_StdHep.Vect().Mag();
    if (vertex->LeptonMom > 0)
        anaUtils::VectorToArray((1 / vertex->LeptonMom) * leptonMom_StdHep.Vect(),vertex->LeptonDir);
  }

  return true; // return false not to save this true vertex
}
