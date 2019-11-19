#include "TDisplayedP0DModule.hxx"
#include "TChain.h"
#include "TEveBoxSet.h"
#include "TEveManager.h"
#include "TEveVSDStructs.h"
#include "TOstreamOverloads.hxx"
#include "TSorted4Vector.hxx"

const int NDETSUSED = 19;

TDisplayedP0DModule::TDisplayedP0DModule()
    : TDisplayedAnalysisModuleBase(
          TString("ReconDir/P0D"), TString("ND::TP0DReconModule::TP0DTrack"),
          TString("ND::TP0DReconModule::TP0DVertex"),
          TString("ND::TP0DReconModule::TP0DShower"),
          TString("ND::TP0DReconModule::TP0DParticle")),
      fClusters(NULL),
      fHits(NULL),
      fNodes(NULL) {
  if (!fKchainOK) {
    cout << " p0dmodule NOT setting branch addresses due to " << fKchainOK
         << endl;
    return;
  }

  fClusters = new TClonesArray("ND::TP0DReconModule::TP0DCluster",1000);
  fHits = new TClonesArray("ND::TP0DReconModule::TP0DHit",1000);
  fNodes = new TClonesArray("ND::TP0DReconModule::TP0DNode",1000);

  if (fChain->SetBranchAddress("NTracks", &fNTracks) < 0) {
    fKchainOK = kFALSE;
  }
  if (fTracks && fChain->SetBranchAddress("Tracks", &fTracks) < 0) {
    fKchainOK = kFALSE;
  }
  if (fChain->SetBranchAddress("NShowers", &fNShowers) < 0) {
    fKchainOK = kFALSE;
  }
  if (fShowers && fChain->SetBranchAddress("Showers", &fShowers) < 0) {
    fKchainOK = kFALSE;
  }
  if (fChain->SetBranchAddress("NVertices", &fNVertices) < 0) {
    fKchainOK = kFALSE;
  }
  if (fVertices && fChain->SetBranchAddress("Vertices", &fVertices) < 0) {
    fKchainOK = kFALSE;
  }
  if (fChain->SetBranchAddress("NParticles", &fNParticles) < 0) {
    fKchainOK = kFALSE;
  }
  if (fParticles && fChain->SetBranchAddress("Particles", &fParticles) < 0) {
    fKchainOK = kFALSE;
  }
  if (fChain->SetBranchAddress("NClusters", &fNClusters) < 0) {
    fKchainOK = kFALSE;
  }
  if (fClusters && fChain->SetBranchAddress("Clusters", &fClusters) < 0) {
    fKchainOK = kFALSE;
  }
  if (fChain->SetBranchAddress("NHits", &fNHits) < 0) {
    fKchainOK = kFALSE;
  }
  if (fHits && fChain->SetBranchAddress("Hits", &fHits) < 0) {
    fKchainOK = kFALSE;
  }
  if (fChain->SetBranchAddress("NNodes", &fNNodes) < 0) {
    fKchainOK = kFALSE;
  }
  if (fNodes && fChain->SetBranchAddress("Nodes", &fNodes) < 0) {
    fKchainOK = kFALSE;
  }
}
void TDisplayedP0DModule::Draw(int entry, int bunch) {
  fBunchToDraw = bunch;
  if (!fKchainOK) {
    return;
  }
  TND280TrackList *gTrackList = PrepareToDraw(entry, "P0DRecon Tracks");
  if (gTrackList == NULL) {
    return;
  }
  gTrackList->SetRnrState(kFALSE);
  TEveElementList *ReconstructedShowers =
      new TEveElementList("P0DRecon Shower");
  TString longName = "P0DRecon  showers";
  ReconstructedShowers->SetTitle(longName);
  ReconstructedShowers->SetRnrState(kFALSE);

  DrawTracks<ND::TP0DReconModule::TP0DTrack *>(fTracks, fNTracks, gTrackList);

  DrawShowers(fShowers, fNShowers, ReconstructedShowers);

  TND280TrackList *gVertexList = defaultTrackList("P0DRecon Vertices");
  DrawTracks<ND::TP0DReconModule::TP0DVertex *>(fVertices, fNVertices,
                                                gVertexList);

  TND280TrackList *gParticleList = defaultTrackList("P0DRecon Particles");
  DrawTracks<ND::TP0DReconModule::TP0DParticle *>(fParticles, fNParticles,
                                                  gParticleList);

  int nSubs = 0;
  if (gTrackList->HasChildren()) {
    nSubs++;
  }
  if (ReconstructedShowers->HasChildren()) {
    nSubs++;
  }
  if (gParticleList->HasChildren()) {
    nSubs++;
  }
  if (gVertexList->HasChildren()) {
    nSubs++;
  }
  if (nSubs > 1) {
    TEveElementList *base = new TEveElementList("P0DRecon");
    if (gTrackList->HasChildren()) {
      base->AddElement(gTrackList);
    }
    if (gParticleList->HasChildren()) {
      base->AddElement(gParticleList);
    }
    if (gVertexList->HasChildren()) {
      base->AddElement(gVertexList);
    }
    if (ReconstructedShowers->HasChildren()) {
      base->AddElement(ReconstructedShowers);
    }
    gEve->AddElement(base);
    gTrackList->SetRnrState(kTRUE);
    gParticleList->SetRnrState(kTRUE);
    gVertexList->SetRnrState(kTRUE);
    ReconstructedShowers->SetRnrState(kTRUE);
    base->SetRnrState(kFALSE);

  } else {
    if (gTrackList->HasChildren()) {
      gEve->AddElement(gTrackList);
    }
    if (gParticleList->HasChildren()) {
      gEve->AddElement(gParticleList);
    }
    if (gVertexList->HasChildren()) {
      gEve->AddElement(gVertexList);
    }
    if (ReconstructedShowers->HasChildren()) {
      gEve->AddElement(ReconstructedShowers);
    }
  }
}

TEveElement *TDisplayedP0DModule::DrawShowers(TClonesArray *showers,
                                              std::vector<short> pointers,
                                              TString Name) {
  TEveElementList *ShowerList = new TEveElementList(Name);
  TEveArrow *arrow = NULL;
  for (size_t i = 0; i < pointers.size(); i++) {
    ND::TP0DReconModule::TP0DShower *Shower =
        (ND::TP0DReconModule::TP0DShower *)showers->At(i);
    if (Shower) {
      arrow = MakeShower(Shower, i);
      ShowerList->AddElement(arrow);
    }
  }
  if (pointers.size() == 1) return arrow;
  return ShowerList;
}

TEveArrow *TDisplayedP0DModule::MakeShower(
    ND::TP0DReconModule::TP0DShower *Shower, int index) {
  TEveVector Direction;
  Direction.Set(Shower->Direction);
  // shower position
  TEveVector Position;
  Position.Set(Shower->Position.Vect());
  float length = 500;

  float angle1 = fmax(0.05, Shower->Cone);
  Direction *= length;
  float r1 = length * tan(angle1);
  // Draw an arrow
  TEveArrow *arrow = new TEveArrow(
      -Direction[0], -Direction[1], -Direction[2], Position[0] + Direction[0],
      Position[1] + Direction[1], Position[2] + Direction[2]);

  arrow->SetConeR(r1 / length);
  arrow->SetConeL(1.0);
  arrow->SetDrawQuality(20);
  TString myName(Form("P0DRecon  shower (%2d)", index));
  arrow->SetNameTitle(myName, myName);
  arrow->SetPickable(kTRUE);
  arrow->SetMainColor(kGreen);
  arrow->SetSourceObject(Shower);
  return arrow;
}
void TDisplayedP0DModule::DrawShowers(TClonesArray *showers, int nShowers,
                                      TEveElementList *ReconstructedShowers) {
  for (int i = 0; i < nShowers; i++) {
    ND::TP0DReconModule::TP0DShower *Shower =
        (ND::TP0DReconModule::TP0DShower *)showers->At(i);
    if (Shower) {
      ReconstructedShowers->AddElement(MakeShower(Shower, i));
    } else {
      cout << " failed to find p0d shower " << i << endl;
    }
  }
}
template <class objectType>
TEveElement *TDisplayedP0DModule::DrawTracks(TClonesArray *Tracks,
                                             std::vector<short> pointers,
                                             TString Name) {
  TND280TrackList *gTrackList = defaultTrackList(Name);
  TND280TrackPropagator *trkProp = gTrackList->GetPropagator();
  TND280Track *track = NULL;
  for (size_t i = 0; i < pointers.size(); i++) {
    objectType reconTrack = (objectType)Tracks->At(pointers[i]);
    if (reconTrack) {
      if (!InSelectedBunch(reconTrack->Position.T(), fBunchToDraw)) {
        continue;
      }
      track = MakeTrack(trkProp, i, reconTrack);
      gTrackList->AddElement(track);
      track->MakeTrack();
      track->SetMainColor(kGreen);
      track->SetLineWidth(4);
      track->SetRnrState(kFALSE);
    }
  }
  if (pointers.size() == 1) {
    return track;
  }
  return gTrackList;
}
template <class objectType>
void TDisplayedP0DModule::DrawTracks(TClonesArray *Tracks, int NTracks,
                                     TND280TrackList *gTrackList) {
  TND280TrackPropagator *trkProp = gTrackList->GetPropagator();
  for (int i = 0; i < NTracks; i++) {
    objectType reconTrack = (objectType)Tracks->At(i);
    if (reconTrack) {
      if (!InSelectedBunch(reconTrack->Position.T(), fBunchToDraw)) {
        continue;
      }
      TND280Track *track = MakeTrack(trkProp, i, reconTrack);
      gTrackList->AddElement(track);
      track->MakeTrack();
      track->SetMainColor(kGreen);
      track->SetLineWidth(4);
      if (reconTrack->Tracks.size() > 0) {
        track->AddElement(DrawTracks<ND::TP0DReconModule::TP0DTrack *>(
            fTracks, reconTrack->Tracks, "Tracks"));
      }
      if (reconTrack->Particles.size() > 0) {
        track->AddElement(DrawTracks<ND::TP0DReconModule::TP0DParticle *>(
            fParticles, reconTrack->Particles, "Particles"));
      }
      if (reconTrack->Vertices.size() > 0) {
        track->AddElement(DrawTracks<ND::TP0DReconModule::TP0DVertex *>(
            fVertices, reconTrack->Vertices, "Vertices"));
      }
      if (reconTrack->Showers.size() > 0) {
        track->AddElement(
            DrawShowers(fShowers, reconTrack->Showers, "Showers"));
      }
    }
  }
}
TND280Track *TDisplayedP0DModule::MakeTrack(
    TND280TrackPropagator *trkProp, Int_t index,
    ND::TP0DReconModule::TP0DTrack *reconObject) {
  // Helper function creating TEveTrack from T2K global recon track.
  //
  TEveRecTrack *rt = new TEveRecTrack();
  ;
  rt->fIndex = index;
  rt->fV.Set(reconObject->Position.Vect());
  TND280Track *track = new TND280Track(rt, trkProp);

  TVector3 Direction = reconObject->Length * (reconObject->Direction);
  TVector3 Start = reconObject->Position.Vect();
  TVector3 End = Start + Direction;

  // as these tracks apparently have no length, make one up!
  track->AddPathMark(TEvePathMarkD(TEvePathMarkD::kDecay, Vector3ToEve(End),
                                   reconObject->Position.T()));

  TString myName(Form("P0DRecon  track (%2d)", index));
  track->SetName(myName);
  track->SetTitle(myName);
  track->SetSourceObject(reconObject);

  return track;
}

TND280Track *TDisplayedP0DModule::MakeTrack(
    TND280TrackPropagator *trkProp, Int_t index,
    ND::TP0DReconModule::TP0DParticle *reconObject) {
  // Helper function creating TEveTrack from T2K P0D  track.
  //
  TEveRecTrack *rt = new TEveRecTrack();
  ;
  rt->fIndex = index;
  rt->fV.Set(reconObject->Position.Vect());
  TND280Track *track = new TND280Track(rt, trkProp);

  float arbitaryLength = 300;  // needs work!
  TVector3 Direction = arbitaryLength * (reconObject->Direction);
  TVector3 Start = reconObject->Position.Vect();
  TVector3 End = Start + Direction;

  // as these tracks apparently have no length, make one up!
  track->AddPathMark(TEvePathMarkD(TEvePathMarkD::kDecay, Vector3ToEve(End),
                                   reconObject->Position.T()));

  TString myName(Form("P0DRecon  particle (%2d)", index));
  track->SetName(myName);
  track->SetTitle(myName);
  track->SetSourceObject(reconObject);

  return track;
}

TND280Track *TDisplayedP0DModule::MakeTrack(
    TND280TrackPropagator *trkProp, Int_t index,
    ND::TP0DReconModule::TP0DVertex *reconObject) {
  // Helper function creating TEveTrack from T2K global recon track.
  //
  TEveRecTrack *rt = new TEveRecTrack();
  ;
  rt->fIndex = index;
  rt->fV.Set(reconObject->Position.Vect());
  TND280Track *track = new TND280Track(rt, trkProp);

  TVector3 Direction(0.0, 0.0,
                     1.0);  // a very small track to represent a vertex!
  TVector3 Start = reconObject->Position.Vect();
  TVector3 End = Start + Direction;

  track->AddPathMark(TEvePathMarkD(TEvePathMarkD::kDecay, Vector3ToEve(End),
                                   reconObject->Position.T()));
  TString myName(Form("P0DRecon  vertex (%2d)", index));
  track->SetName(myName);
  track->SetTitle(myName);
  track->SetSourceObject(reconObject);
  track->SetRnrPoints(kTRUE);
  return track;
}

void TDisplayedP0DModule::Describe(ND::TP0DReconModule::TP0DHit *RO) {
  Output((int)RO->ChanID, "ChanID", "Channel ID.");
  Output(RO->Charge, "Charge", "Hit charge without attenuation correction.");
  Output(RO->GeomID, "GeomID", "Geometry ID.");
  Output(RO->Time, "Time", "Hit time as reported by hit->GetTime().   ");
}
void TDisplayedP0DModule::Describe(ND::TP0DReconModule::TP0DCluster *RO) {
  OutputList<ND::TP0DReconModule::TP0DCluster *>(
      RO->Clusters, fClusters, "Clusters associated with this object.");
  Output(RO->Cycle, "Cycle", "The cycle number, based on the first hit.");
  Output(RO->EDeposit, "EDeposit", "The deposited charge for the shower.");
  OutputList<ND::TP0DReconModule::TP0DHit *>(
      RO->Hits, fHits, "Hits associated with this object.");
  // float  Moments [arraySize]
  Output(RO->NFiducialHits, "NFiducialHits", "The number of fiducial hits.");
  Output(RO->NHits, "NHits",
         "A count of the Hits associated with this object.");
  Output(RO->Nodes, "Nodes", "A vector of nodes associated with this object.");
  OutputList<ND::TP0DReconModule::TP0DParticle *>(
      RO->Particles, fParticles, "Particles associated with this object.");
  Output(RO->Position, "Position", "The start position of the object.");
  Output(RO->PosVariance, "PosVariance", "The position variance;.");
  OutputList<ND::TP0DReconModule::TP0DShower *>(
      RO->Showers, fShowers, "Showers associated with this object.");
  OutputList<ND::TP0DReconModule::TP0DTrack *>(
      RO->Tracks, fTracks, "Tracks associated with this object.");
  Output(RO->Truth_ChargeShare, "Truth_ChargeShare",
         "The vector of charge shares.");
  Output(RO->Truth_HitCount, "Truth_HitCount", "The vector of hit counts.");
  Output(RO->Truth_TrajIDs, "Truth_TrajIDs", "The vector of Trajectory IDs.");
  Output(RO->UniqueID, "UniqueID", "Unique id for the object.");
  Output(RO->ValidDimensions, "ValidDimensions",
         "The number of valid dimensions for the object.");
  OutputList<ND::TP0DReconModule::TP0DVertex *>(
      RO->Vertices, fVertices, "Vertices associated with this object.");
}
void TDisplayedP0DModule::Describe(ND::TP0DReconModule::TP0DNode *RO) {
  Output(RO->Direction, "Direction", "The direction of the track.");
  Output(RO->DirVariance, "DirVariance", "");
  Output(RO->EDeposit, "EDeposit", "The deposited charge for the node.");
  OutputList<ND::TP0DReconModule::TP0DHit *>(
      RO->Hits, fHits, "Hits associated with this object.");
  Output(RO->Position, "Position", "The position of the track.");
  Output(RO->PosVariance, "PosVariance", "");
  Output(RO->Truth_ChargeShare, "Truth_ChargeShare",
         "The vector of charge shares.");
  Output(RO->Truth_HitCount, "Truth_HitCount", "The vector of hit counts.");
  Output(RO->Truth_TrajIDs, "Truth_TrajIDs", "The vector of Trajectory IDs.");
  Output(RO->ValidDimensions, "ValidDimensions",
         "The number of valid dimensions for the object. ");
}

void TDisplayedP0DModule::Describe(ND::TP0DReconModule::TP0DParticle *RO) {
  Centre(RO->Position);
  Output(RO->AlgorithmName, "AlgorithmName",
         "The name of the algorithm that created this object.");
  Output(RO->Charge, "Charge", "The charge of the identified particle.");
  OutputList<ND::TP0DReconModule::TP0DCluster *>(
      RO->Clusters, fClusters, "Cluster associated with this object.");
  Output(RO->Cycle, "Cycle", "The cycle number, based on the first hit.");
  Output(RO->Direction, "Direction", "The direction of the object.");
  Output(RO->DirVariance, "DirVariance", "  EndDeposit");
  //  Output(RO->Hits,"Hits"," Hit associated with this object.");
  OutputList<ND::TP0DReconModule::TP0DHit *>(
      RO->Hits, fHits, "Hits associated with this object ");

  Output(RO->Momentum, "Momentum", "The momentum of the object.");
  Output(RO->NDOF, "NDOF", "The number of degrees of freedom.");
  Output(RO->NHits, "NHits",
         "A count of the Hits associated with this object.");
  // Output(RO->Nodes,"Nodes","Number of nodes associated with this object.");
  OutputList<ND::TP0DReconModule::TP0DNode *>(
      RO->Nodes, fNodes, " Nodes associated with this object. ");
  OutputList<ND::TP0DReconModule::TP0DParticle *>(
      RO->Particles, fParticles,
      "Particles articles associated with this object.");
  for (size_t i = 0; i < RO->PID.size(); i++) {
    Output(RO->PID[i], "PID number", "weight");
    Output(RO->PID_weight[i], "PID_weight", "");
  }
  Output(RO->Position, "Position", "The start position of the object.");
  Output(RO->PosVariance, "PosVariance", "The position variance;.");
  Output(RO->Quality, "Quality", "The quality of the fit.");
  OutputList<ND::TP0DReconModule::TP0DParticle *>(
      RO->Showers, fShowers, "Showers associated with this object.");
  Output(RO->SideDeposit, "SideDeposit",
         "The deposited charge in the side bars.");
  Output(RO->Status, "Status", "The status for the fit.");
  Output(RO->Tracks, "Tracks", "Number of tracks associated with this object.");
  Output(RO->Truth_ChargeShare, "Truth_ChargeShare",
         "The vector of charge shares.");
  Output(RO->Truth_HitCount, "Truth_HitCount", "The vector of hit counts.");
  Output(RO->Truth_TrajIDs, "Truth_TrajIDs", "The vector of Trajectory IDs.");
  Output(RO->UniqueID, "UniqueID", "Unique id for the object.");
  Output(RO->ValidDimensions, "ValidDimensions",
         "The number of valid dimensions for the object.");
  OutputList<ND::TP0DReconModule::TP0DVertex *>(
      RO->Vertices, fVertices, "Vertices associated with this object.  ");
}

void TDisplayedP0DModule::Describe(ND::TP0DReconModule::TP0DShower *RO) {
  Output(RO->AlgorithmName, "AlgorithmName",
         "The name of the algorithm that created this object.");
  OutputList<ND::TP0DReconModule::TP0DCluster *>(
      RO->Clusters, fClusters, "Clusters associated with this object.");
  Output(RO->Cone, "Cone", "The opening angle of the cone.");
  Output(RO->Cycle, "Cycle", "The cycle number, based on the first hit.");
  Output(RO->Direction, "Direction", "The direction of the object.");
  Output(RO->DirVariance, "DirVariance", "");
  Output(RO->EDeposit, "EDeposit", "The deposited charge for the shower.");
  Output(RO->EndDeposit, "EndDeposit", "");
  OutputList<ND::TP0DReconModule::TP0DParticle *>(
      RO->Hits, fHits, "Hits associated with this object.");
  Output(RO->Length, "Length", "The length of the shower (RMS).");
  Output(RO->NDOF, "NDOF", "The number of degrees of freedom.");
  Output(RO->NHits, "NHits",
         "A count of the Hits associated with this object.");
  OutputList<ND::TP0DReconModule::TP0DNode *>(
      RO->Nodes, fNodes, "Nodes associated with this object.");
  OutputList<ND::TP0DReconModule::TP0DParticle *>(
      RO->Particles, fParticles, "Particles associated with this object.");
  Output(RO->Position, "Position", "The start position of the object.");
  Output(RO->PosVariance, "PosVariance", "");
  Output(RO->Quality, "Quality", "The quality of the fit.");
  OutputList<ND::TP0DReconModule::TP0DShower *>(
      RO->Showers, fShowers, "Showers associated with this object.");
  Output(RO->SideDeposit, "SideDeposit",
         "The deposited charge in the side bars.");
  Output(RO->Status, "Status", "The status for the fit.");
  OutputList<ND::TP0DReconModule::TP0DTrack *>(
      RO->Tracks, fTracks, "Tracks associated with this object.");
  Output(RO->Truth_ChargeShare, "Truth_ChargeShare",
         "The vector of charge shares.");
  Output(RO->Truth_HitCount, "Truth_HitCount", "The vector of hit counts.");
  Output(RO->Truth_TrajIDs, "Truth_TrajIDs", "The vector of Trajectory IDs.");
  Output(RO->UniqueID, "UniqueID", "Unique id for the object.");
  Output(RO->ValidDimensions, "ValidDimensions",
         "The number of valid dimensions for the object.");
  OutputList<ND::TP0DReconModule::TP0DVertex *>(
      RO->Vertices, fVertices, "Vertices associated with this object.");
  Output(RO->Width, "Width",
         "The width of the shower (perpendicular to the direction).  ");
}
void TDisplayedP0DModule::Describe(ND::TP0DReconModule::TP0DTrack *RO) {
  Centre(RO->Position);
  Output(RO->AlgorithmName, "AlgorithmName",
         "The name of the algorithm that created this object.");
  OutputList<ND::TP0DReconModule::TP0DCluster *>(
      RO->Clusters, fClusters, "Clusters associated with this object.");
  Output(RO->Cycle, "Cycle", "The cycle number, based on the first hit.");
  Output(RO->Direction, "Direction", "The direction of the track.");
  Output(RO->DirVariance, "DirVariance", "");
  Output(RO->EDeposit, "EDeposit", "The deposited charge for the track.");
  Output(RO->EndDeposit, "EndDeposit", "");
  OutputList<ND::TP0DReconModule::TP0DHit *>(
      RO->Hits, fHits, "Hits associated with this object.");
  Output(RO->Length, "Length", "The length of the track in the P0D.");
  Output(RO->NDOF, "NDOF", "The number of degrees of freedom.");
  Output(RO->NHits, "NHits",
         "A count of the Hits associated with this object.");
  OutputList<ND::TP0DReconModule::TP0DNode *>(
      RO->Nodes, fNodes, "Nodes associated with this object.");
  OutputList<ND::TP0DReconModule::TP0DParticle *>(
      RO->Particles, fParticles, "Particles associated with this object.");
  Output(RO->Position, "Position", "The position of the track.");
  Output(RO->PosVariance, "PosVariance", "");
  Output(RO->Quality, "Quality", "The quality of the fit.");
  OutputList<ND::TP0DReconModule::TP0DShower *>(
      RO->Showers, fShowers, "Showers associated with this object.");
  Output(RO->SideDeposit, "SideDeposit",
         "The deposited charge in the side bars.");
  Output(RO->Status, "Status", "The status for the fit.");
  OutputList<ND::TP0DReconModule::TP0DTrack *>(
      RO->Tracks, fTracks, " Tracks associated to this object");
  Output(RO->UniqueID, "UniqueID", "");
  Output(RO->ValidDimensions, "ValidDimensions", "");
}
void TDisplayedP0DModule::Describe(ND::TP0DReconModule::TP0DVertex *RO) {
  Centre(RO->Position);
  Output(RO->AlgorithmName, "AlgorithmName",
         "The name of the algorithm that created this object.");
  OutputList<ND::TP0DReconModule::TP0DCluster *>(
      RO->Clusters, fClusters, "Clusters associated with this object.");
  Output(RO->Cycle, "Cycle", "The cycle number, based on the first hit.");
  Output(RO->Fiducial, "Fiducial",
         "The distance to the outside of the "
         "fiducial volume (negative if the vertex is "
         "outside).");
  OutputList<ND::TP0DReconModule::TP0DHit *>(
      RO->Hits, fHits, "Hits associated with this object.");
  Output(RO->NDOF, "NDOF", "The number of degrees of freedom.");
  Output(RO->NHits, "NHits",
         "A count of the Hits associated with this object.");
  OutputList<ND::TP0DReconModule::TP0DNode *>(
      RO->Nodes, fNodes, "Nodes associated with this object.");
  OutputList<ND::TP0DReconModule::TP0DParticle *>(
      RO->Particles, fParticles, "Particles associated with this object.");
  Output(RO->Position, "Position", "The position of the vertex.");
  Output(RO->PosVariance, "PosVariance", "The position variance;.");
  Output(RO->Quality, "Quality", "The quality of the fit.");
  OutputList<ND::TP0DReconModule::TP0DShower *>(
      RO->Showers, fShowers, "Showers associated with this object.");
  Output(RO->Status, "Status", "The status for the fit.");
  OutputList<ND::TP0DReconModule::TP0DTrack *>(
      RO->Tracks, fTracks, "Tracks associated with this object.");
  Output(RO->Truth_ChargeShare, "Truth_ChargeShare",
         "The vector of charge shares.");
  Output(RO->Truth_HitCount, "Truth_HitCount", "The vector of hit counts.");
  Output(RO->Truth_TrajIDs, "Truth_TrajIDs", "The vector of Trajectory IDs.");
  Output(RO->UniqueID, "UniqueID", "Unique id for the object.");
  Output(RO->ValidDimensions, "ValidDimensions",
         "The number of valid dimensions for the object.");
  OutputList<ND::TP0DReconModule::TP0DVertex *>(
      RO->Vertices, fVertices, "Vertices associated with this object.   ");
}
template <class objectType>
void TDisplayedP0DModule::OutputList(std::vector<short> pointers,
                                     TClonesArray *list, TString name) {
  if (pointers.size() > 0) {
    doIndent();
    if (pointers.size() == 1) {
      cout << "There is one item in the list: " << name << endl;
    } else {
      cout << "There are " << pointers.size() << " items in the list: " << name
           << endl;
    }

    indent++;
    doIndent("+");
    cout << endl;
    for (size_t i = 0; i < pointers.size(); i++) {
      divider();
      doIndent();
      cout << "Item number " << i << endl;
      divider();
      Describe((objectType)list->At(i));
    }
    divider();
    doIndent("+");
    cout << endl;
    indent--;
  }
}
