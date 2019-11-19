#include "TDisplayedGlobalModule.hxx"
#include "TChain.h"
#include "TEveManager.h"

#include "TEveBox.h"
#include "TEveVSDStructs.h"
#include "TGLViewer.h"
#include "TOstreamOverloads.hxx"
#include "TSorted4Vector.hxx"

const int NDETSUSED = 19;

TDisplayedGlobalModule::TDisplayedGlobalModule()
    : TDisplayedAnalysisModuleBase(
          TString("ReconDir/Global"),
          TString("ND::TGlobalReconModule::TGlobalPID"),
          TString("ND::TGlobalReconModule::TGlobalVertex"), TString(""),
          TString("")) {
  //    fChain =new TChain("ReconDir/Global");

  //    fTracks=  new TClonesArray("ND::TGlobalReconModule::TGlobalPID", 1000);
  fChain->SetBranchAddress("NPIDs", &fNTracks);
  fChain->SetBranchAddress("PIDs", &fTracks);
  fChain->SetBranchAddress("NVertices", &fNVertices);
  fChain->SetBranchAddress("Vertices", &fVertices);
}
void TDisplayedGlobalModule::Draw(int entry, int bunch) {
  TND280TrackList* gTrackList = PrepareToDraw(entry, "Global Tracks");
  if (gTrackList == NULL) return;
  TEveElementList* gShowerList = new TEveElementList("Global Showers");
  TEveElementList* gVertexList = new TEveElementList("Global Vertices");
  //(TEveElementList*) PrepareToDraw(entry,"Global Tracks");
  TND280TrackPropagator* trkProp = gTrackList->GetPropagator();
  //  cout<<" global tracks "<<endl;
  for (int i = 0; i < fNTracks; i++) {
    //    cout<<i<<endl;
    ND::TGlobalReconModule::TGlobalPID* reconObject =
        (ND::TGlobalReconModule::TGlobalPID*)fTracks->At(i);
    if (!InSelectedBunch(reconObject->FrontPosition.T(), bunch) &&
        !InSelectedBunch(reconObject->BackPosition.T(), bunch))
      continue;
    /*  cout<<" global track "<<i<<" has "
    <<reconObject->NP0Ds<<" pod objects "<<endl
    <<reconObject->NSMRDs<<" SMRD objects "<<endl
    <<reconObject->NTPCs<<" tpc objects "<<endl
    <<reconObject->NTRACKERs<<" tracker objects "<<endl
    <<reconObject->NECALs<<" ecal objects "<<endl
    <<reconObject->NFGDs<<" fgd objects "<<endl;
    */
    // cout<<i<<" reconObject "<<reconObject<<endl;
    TND280Track* track = MakeTrack(trkProp, i, reconObject);
    /*
    If the "track" has zero length, assume it is just a place holder for
    showers copied over from detector reconstruction. In that case they are
    added
    into a different level in the tree.
    */
    if (track) {
      //   cout<<" global recon track "<<i<<" is track "<<endl;
      track->SetSourceObject(reconObject);
      AddEcalObjects(reconObject, track, kTRUE, i);
      AddP0DObjects(reconObject, track, kTRUE);
      AddSMRDObjects(reconObject, track);
      AddTPCObjects(reconObject, track, trkProp);
      AddFGDObjects(reconObject, track);
      gTrackList->AddElement(track);
      track->MakeTrack();
      track->SetMainColor(kWhite);
    } else {
      //    cout<<" global recon track "<<i<<" is shower "<<endl;
      AddEcalObjects(reconObject, gShowerList, kFALSE, i);
      AddP0DObjects(reconObject, gShowerList, kFALSE);
      AddSMRDObjects(reconObject, gShowerList);
      AddTPCObjects(reconObject, gShowerList, trkProp);
      AddFGDObjects(reconObject, gShowerList);
    }
  }  // loop over PIDs

  /*
  Now Add Vertices
  */
  // TEveBox* box = new TEveBox();
  //  gVertexList->SetChildClass(box->IsA());

  for (int i = 0; i < fNVertices; i++) {
    ND::TGlobalReconModule::TGlobalVertex* reconObject =
        (ND::TGlobalReconModule::TGlobalVertex*)fVertices->At(i);
    if (!InSelectedBunch(reconObject->Position.T(), bunch)) continue;
    if (reconObject != NULL)
    //          cout<<" failed to fetch it "<<endl;
    //  else
    {
      TEveBox* box = new TEveBox(Form(" Global Vertex %d", i),
                                 Form(" Global Vertex %d", i));
      float xc = reconObject->Position.X();
      float yc = reconObject->Position.Y();
      float zc = reconObject->Position.Z();
      float deltax = fmax(3, reconObject->Variance.X());
      float deltay = fmax(3, reconObject->Variance.Y());
      float deltaz = fmax(3, reconObject->Variance.Z());
      box->SetMainColor(kGreen);
      if (deltax > 200 || deltay > 200 || deltaz > 200) {
        box->SetMainColor(kRed);
        box->SetRnrState(kFALSE);
      }
      deltax = fmin(200, deltax);
      deltay = fmin(200, deltay);
      deltaz = fmin(200, deltaz);
      box->SetVertex(0, xc - deltax, yc - deltay, zc - deltaz);
      box->SetVertex(1, xc + deltax, yc - deltay, zc - deltaz);
      box->SetVertex(2, xc + deltax, yc - deltay, zc + deltaz);
      box->SetVertex(3, xc - deltax, yc - deltay, zc + deltaz);
      box->SetVertex(4, xc - deltax, yc + deltay, zc - deltaz);
      box->SetVertex(5, xc + deltax, yc + deltay, zc - deltaz);
      box->SetVertex(6, xc + deltax, yc + deltay, zc + deltaz);
      box->SetVertex(7, xc - deltax, yc + deltay, zc + deltaz);
      box->SetLineWidth(4);
      box->SetSourceObject(reconObject);
      box->SetPickable(kTRUE);
      gVertexList->AddElement(box);
    }
  }
  // DrawVertices(fVertices,fNVertices,gVertexList);
  if (gTrackList->HasChildren()) gEve->AddElement(gTrackList);
  if (gShowerList->HasChildren()) gEve->AddElement(gShowerList);
  if (gVertexList->HasChildren()) gEve->AddElement(gVertexList);
}
void TDisplayedGlobalModule::DrawVertices(TClonesArray* Vertices, int NVertices,
                                          TND280TrackList* gVertexList) {
  TND280TrackPropagator* trkProp = gVertexList->GetPropagator();
  for (int i = 0; i < NVertices; i++) {
    ND::TGlobalReconModule::TGlobalVertex* reconVertex =
        (ND::TGlobalReconModule::TGlobalVertex*)Vertices->At(i);
    if (reconVertex) {
      TND280Track* Vertex = MakeVertex(trkProp, i, reconVertex);
      gVertexList->AddElement(Vertex);
      Vertex->MakeTrack();
      Vertex->SetMainColor(kGreen);
      Vertex->SetLineWidth(4);
    }
  }
}
TND280Shower* TDisplayedGlobalModule::CreateShower(TEveVector Direction,
                                                   TEveVector Position,
                                                   TVector3 angle3,
                                                   float length) {
  // currently there are two angles defined, in the x and y components of the 3
  // vector (!)
  const float aMin = 0.05;  // cone must have  a finite angle to be drawn!!
  float angle1 = fmax(aMin, angle3.X());
  // float angle2=fmax(aMin,angle3.Y());
  Direction *= length;
  float r1 = length * tan(angle1);
  // float r2 = length*tan(angle2);
  // Draw a cone with axis at position, direction = direction, size1 = r1, size2
  // = r2, angle = ??!
  TND280Shower* arrow = new TND280Shower(
      -Direction[0], -Direction[1], -Direction[2], Position[0] + Direction[0],
      Position[1] + Direction[1], Position[2] + Direction[2]);

  arrow->SetConeR(r1 / length);
  arrow->SetConeL(1.0);
  arrow->SetDrawQuality(20);
  return arrow;
}
void TDisplayedGlobalModule::AddEcalObjects(
    ND::TGlobalReconModule::TGlobalPID* reconObject, TEveElement* track,
    bool hasTrack, int index) {
  TClonesArray* ECAL = reconObject->ECAL;
  for (int iEcal = 0; iEcal < reconObject->NECALs; iEcal++) {
    ND::TGlobalReconModule::TECALObject* ecal =
        (ND::TGlobalReconModule::TECALObject*)ECAL->At(iEcal);
    //  if(ecal->IsTrackLike)cout<<" its track like "<<endl;
    if (ecal->FilledAsType==1) {
      // shower direction
      TEveVector Direction;
      Direction.Set(ecal->FrontDirection);
      // shower position
      TEveVector Position;
      Position.Set(ecal->FrontPosition.Vect());
      float length = 300;
      TVector3 angle3 = ecal->Cone;

      TND280Shower* arrow = CreateShower(Direction, Position, angle3, length);

      TString longName;
      if (hasTrack)
        longName = "ECAL shower associated to a global recon object.";
      else  //              012345678901234
        longName = Form("ECAL shower (%2d)", index);
      // TString myName("Shower associated to  global recon object");
      arrow->SetNameTitle(longName, longName);
      arrow->SetPickable(kTRUE);
      arrow->SetMainColor(kWhite);
      // ReconstructedShower->AddEllipticCone(Position, Direction, r1,r2,0.0);
      track->AddElement(arrow);
      arrow->SetSourceObject(ecal);
    } else {
      if ((ecal->BackPosition.Vect() - ecal->FrontPosition.Vect()).Mag() > 10) {
        track->AddElement(AddLine(ecal, "ECAL track"));
      }
    }
  }
}
void TDisplayedGlobalModule::AddP0DObjects(
    ND::TGlobalReconModule::TGlobalPID* reconObject, TEveElement* track,
    bool hasTrack) {
  TClonesArray* P0D = reconObject->P0D;
  for (int iP0D = 0; iP0D < reconObject->NP0Ds; iP0D++) {
    ND::TGlobalReconModule::TP0DObject* p0d =
        (ND::TGlobalReconModule::TP0DObject*)P0D->At(iP0D);
    // DescribeP0D(p0d);
    // assume that a shower must have an opening angle of the cone
    if (p0d->Cone.X() > 0) {
      // shower direction
      TEveVector Direction;
      Direction.Set(p0d->FrontDirection);
      // shower position
      TEveVector Position;
      Position.Set(p0d->FrontPosition.Vect());
      float length = 300;
      TVector3 angle3 = p0d->Cone;

      TND280Shower* arrow = CreateShower(Direction, Position, angle3, length);

      TString longName;
      if (hasTrack)
        longName = "P0D shower associated to a global recon object.";
      else  //              012345678901234
        longName = Form("P0D shower (%2d)", iP0D);
      arrow->SetNameTitle(longName, longName);
      arrow->SetPickable(kTRUE);
      arrow->SetMainColor(kWhite);
      track->AddElement(arrow);
    } else {
      if ((p0d->BackPosition.Vect() - p0d->FrontPosition.Vect()).Mag() > 10) {
        track->AddElement(AddLine(p0d, "P0D track"));
      }
    }
  }
}
void TDisplayedGlobalModule::AddSMRDObjects(
    ND::TGlobalReconModule::TGlobalPID* reconObject, TEveElement* track) {
  TClonesArray* SMRD = reconObject->SMRD;
  for (int iSMRD = 0; iSMRD < reconObject->NSMRDs; iSMRD++) {
    ND::TGlobalReconModule::TSMRDObject* smrd =
        (ND::TGlobalReconModule::TSMRDObject*)SMRD->At(iSMRD);
    // SMRD objects are always tracks, not showers
    // They have initial and final position/momentum, but no sign so can not
    // be tracked in magnetic field.
    // Simplest solution is to join front and back position with a straight line
    //
    if ((smrd->BackPosition.Vect() - smrd->FrontPosition.Vect()).Mag() > 10) {
      track->AddElement(AddLine(smrd, "SMRD track"));
    }
  }
}
TEveLine* TDisplayedGlobalModule::AddLine(ND::TSubBaseObject* object,
                                          TString name) {
  TLorentzVector FrontPosition = object->FrontPosition;
  TLorentzVector BackPosition = object->BackPosition;
  TEveLine* line = new TEveLine(name, 2);
  line->SetNextPoint(FrontPosition.X(), FrontPosition.Y(), FrontPosition.Z());
  line->SetNextPoint(BackPosition.X(), BackPosition.Y(), BackPosition.Z());
  line->SetName(name + " associated to global reconstructed object ");
  line->SetRnrState(kFALSE);
  line->SetRnrLine(kTRUE);
  line->SetLineColor(kRed);
  line->SetLineWidth(4);
  line->SetSourceObject(object);
  return line;
}
void TDisplayedGlobalModule::AddTPCObjects(
    ND::TGlobalReconModule::TGlobalPID* reconObject, TEveElement* track,
    TND280TrackPropagator* trkProp) {
  TClonesArray* TPC = reconObject->TPC;
  for (int iTPC = 0; iTPC < reconObject->NTPCs; iTPC++) {
    ND::TGlobalReconModule::TTPCObject* tpc =
        (ND::TGlobalReconModule::TTPCObject*)TPC->At(iTPC);
    //
    if ((tpc->BackPosition.Vect() - tpc->FrontPosition.Vect()).Mag() > 10) {
      //  TEveLine* line = new TEveLine("TPC track",2);
      //   line->SetNextPoint(tpc->FrontPosition.X(),tpc->FrontPosition.Y(),tpc->FrontPosition.Z()
      //   );
      //   line->SetNextPoint(tpc->BackPosition.X(),tpc->BackPosition.Y(),tpc->BackPosition.Z()
      //   );
      TEveRecTrack* rt = new TEveRecTrack();
      ;
      rt->fIndex = iTPC;
      rt->fSign = (tpc->Charge > 0) ? 1 : -1;
      rt->fV.Set(tpc->FrontPosition.Vect());
      rt->fP.Set((tpc->FrontMomentum) * tpc->FrontDirection);
      TND280Track* TPCtrack = new TND280Track(rt, trkProp);

      TPCtrack->AddPathMark(TEvePathMarkD(TEvePathMarkD::kDecay,
                                          LorentzToEve(tpc->BackPosition),
                                          tpc->BackPosition.T()));
      TPCtrack->SetName(
          "TPC TPCtrack associated to global reconstructed object ");
      TPCtrack->SetRnrState(kFALSE);
      TPCtrack->SetRnrLine(kTRUE);
      TPCtrack->SetLineColor(kRed);
      TPCtrack->SetLineWidth(4);
      TPCtrack->SetSourceObject(tpc);
      track->AddElement(TPCtrack);
      TPCtrack->MakeTrack();
    }
  }
}
void TDisplayedGlobalModule::AddFGDObjects(
    ND::TGlobalReconModule::TGlobalPID* reconObject, TEveElement* track) {
  TClonesArray* FGD = reconObject->FGD;
  for (int iFGD = 0; iFGD < reconObject->NFGDs; iFGD++) {
    ND::TGlobalReconModule::TFGDObject* fgd =
        (ND::TGlobalReconModule::TFGDObject*)FGD->At(iFGD);
    // FGD objects are always tracks, not showers
    // They have initial and final position/momentum, but no sign so can not
    // be tracked in magnetic field.
    // Simplest solution is to join front and back position with a straight line
    //
    if ((fgd->BackPosition.Vect() - fgd->FrontPosition.Vect()).Mag() > 10) {
      // TEveLine* line = new TEveLine("FGD track",2);
      //  line->SetNextPoint(fgd->FrontPosition.X(),fgd->FrontPosition.Y(),fgd->FrontPosition.Z()
      //  );
      //  line->SetNextPoint(fgd->BackPosition.X(),fgd->BackPosition.Y(),fgd->BackPosition.Z()
      //  );
      //  line->SetName("FGD track associated to global reconstructed object ");
      //  line->SetRnrState(kFALSE);
      //  line->SetRnrLine(kTRUE);
      //  line->SetLineColor(kYellow);
      //  line->SetLineWidth(2);
      //  track->AddElement(line);
      track->AddElement(AddLine(fgd, "FGD track"));
    }
  }
}
TND280Track* TDisplayedGlobalModule::MakeTrack(
    TND280TrackPropagator* trkProp, Int_t index,
    ND::TGlobalReconModule::TGlobalPID* reconObject) {
  // Helper function creating TEveTrack from T2K global recon track.
  //
  TEveRecTrack* rt = new TEveRecTrack();
  ;
  rt->fIndex = index;
  rt->fSign = (reconObject->Charge > 0) ? 1 : -1;
  rt->fV.Set(reconObject->FrontPosition.Vect());
  rt->fP.Set((reconObject->FrontMomentum) * reconObject->FrontDirection);
  TND280Track* track = new TND280Track(rt, trkProp);
  track->SetName(Form("Global track %2d", index));
  int det[23] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11,
                 11, 12, 12, 13, 14, 15, 15, 16, 16, 17, 18};

  double zmin;
  double zmax;
  if (reconObject->FrontPosition.Z() < reconObject->BackPosition.Z()) {
    zmin = reconObject->FrontPosition.Z();
    zmax = reconObject->BackPosition.Z();
  } else {
    zmin = reconObject->BackPosition.Z();
    zmax = reconObject->FrontPosition.Z();
  }

  std::vector<TVector3> positions;

  TVector3 pos = reconObject->FrontPosition.Vect();
  positions.push_back(pos);

  for (int j = 0; j < NDETSEXTRAP; j++) {
    if (!reconObject->DetectorUsed[det[j]]) continue;

    if (reconObject->EntranceOK[j] &&
        fabs(reconObject->EntrancePosition[j].Z()) > 0.0001) {
      pos = reconObject->EntrancePosition[j].Vect();
      positions.push_back(pos);
    }
    if (reconObject->ExitOK[j] &&
        fabs(reconObject->ExitPosition[j].Z()) > 0.0001) {
      pos = reconObject->ExitPosition[j].Vect();
      positions.push_back(pos);
    }
  }

  std::sort(positions.begin(), positions.end(), SortInZ);

  for (unsigned int j = 1; j < positions.size(); j++) {
    if (positions[j].Z() < zmin || positions[j].Z() > zmax) continue;
    if (j > 0)
      if (fabs(positions[j].Z() - positions[j - 1].Z()) < 0.01) continue;
    /*
    Don't add the start position as it is already included
    */
    if (fabs(positions[j].X() - reconObject->FrontPosition.X()) < 0.01)
      continue;
    if (fabs(positions[j].Y() - reconObject->FrontPosition.Y()) < 0.01)
      continue;
    if (fabs(positions[j].Z() - reconObject->FrontPosition.Z()) < 0.01)
      continue;
    /*
    Should include momentum here too???
    */
    track->AddPathMark(
        TEvePathMarkD(TEvePathMarkD::kReference, Vector3ToEve(positions[j])));
  }
  track->AddPathMark(TEvePathMarkD(TEvePathMarkD::kDecay,
                                   LorentzToEve(reconObject->BackPosition),
                                   reconObject->BackPosition.T()));
  track->SetLineWidth(4);
  track->SetTitle(Form("Global Track %2d ", index));
  // cout<<track->GetTitle()<<endl;
  AddHits(reconObject, track);
  if ((reconObject->BackPosition.Vect() - reconObject->FrontPosition.Vect())
          .Mag() < 0.0001)
    return NULL;
  return track;
}
TND280Track* TDisplayedGlobalModule::MakeVertex(
    TND280TrackPropagator* trkProp, Int_t index,
    ND::TGlobalReconModule::TGlobalVertex* reconObject) {
  // Helper function creating TEveTrack from T2K global recon track.
  //
  TEveRecTrack* rt = new TEveRecTrack();
  ;
  rt->fIndex = index;
  // cout<<" add vertex at "<<reconObject->Position.Vect()<<endl;
  rt->fV.Set(reconObject->Position.Vect());
  // work out momentum from curvature!
  // rt->fP.Set( ( reconObject->FrontMomentum)*reconObject->FrontDirection);
  TND280Track* track = new TND280Track(rt, trkProp);

  // float arbitaryLength=300; // needs work!
  TVector3 Direction(0.0, 0.0,
                     1.0);  // a very small track to represent a vertex!
  TVector3 Start = reconObject->Position.Vect();
  TVector3 End = Start + Direction;

  track->AddPathMark(TEvePathMarkD(TEvePathMarkD::kDecay, Vector3ToEve(End),
                                   reconObject->Position.T()));
  TString myName(Form("Global recon  vertex (%2d)", index));
  track->SetName(myName);
  track->SetTitle(myName);
  track->SetSourceObject(reconObject);
  track->SetRnrPoints(kTRUE);
  return track;
}

void TDisplayedGlobalModule::AddHits(
    ND::TGlobalReconModule::TGlobalPID* reconObject, TND280Track* track) {
  if (reconObject->NHitsSaved > 0) {
    TClonesArray* HitsSaved = reconObject->HitsSaved;
    TEveBoxSet* hits = new TEveBoxSet();
    hits->Reset(TEveBoxSet::kBT_FreeBox, kFALSE, reconObject->NHitsSaved);
    for (int i = 0; i < reconObject->NHitsSaved; i++) {
      ND::TGlobalReconModule::TGlobalHit* hit =
          (ND::TGlobalReconModule::TGlobalHit*)HitsSaved->At(i);
      TVector3 position = hit->Position;
      TVector3 uncertainty = hit->PositionError;
      if (hit->Type > 110)
        AddBoxToBoxSet(position, uncertainty, hits);
      else {
        //    cout<<" hit "<<i<<" associated to global "<<hit->Position<<"
        //  "<<hit->Type<<endl
        //    <<uncertainty<<endl;
        //  if(hit->Type==110)cout<<" determine x"<<endl;
        //  if(hit->Type==101)cout<<" determine y"<<endl;
        //  if(hit->Type== 11)cout<<" determine z"<<endl;
        float toPrevious = 10000;
        float toNext = 10000;
        TVector3 previousPosition, nextPosition;
        if (i > 0) {
          //   cout<<" compare to previous "<<endl;
          ND::TGlobalReconModule::TGlobalHit* hit2 =
              (ND::TGlobalReconModule::TGlobalHit*)HitsSaved->At(i - 1);
          previousPosition = hit2->Position;
          toPrevious = Closeness(hit, hit2);
          //   cout<<" to previous "<<toPrevious<<endl;
        }
        if (i < ((reconObject->NHitsSaved) - 1)) {
          //   cout<<" compare to next "<<endl;
          ND::TGlobalReconModule::TGlobalHit* hit2 =
              (ND::TGlobalReconModule::TGlobalHit*)HitsSaved->At(i + 1);
          nextPosition = hit2->Position;
          toNext = Closeness(hit, hit2);
          //   cout<<" to next "<<toNext<<endl;
        }
        if (toPrevious < toNext) {
          if (toPrevious < 100) {
            //   cout<<" use previous position "<<endl;
            AdjustPosition(position, hit->Type, previousPosition);
          }
        } else {
          if (toNext < 100) {
            //    cout<<" use next position "<<endl;
            AdjustPosition(position, hit->Type, nextPosition);
          }
        }
        //  cout<<" new position "<<position<<endl;
        // position=hit->Position;    // comment out once works ok
        AddBoxToBoxSet(position, uncertainty, hits);
      }
      //        AddBoxToBoxSet(hits,hit);
    }
    hits->SetName("Hits associated to global recon object ");
    //  hits->SetRnrState(kFALSE);
    track->AddElement(hits);
  }
  return;
}
float TDisplayedGlobalModule::Closeness(
    ND::TGlobalReconModule::TGlobalHit* hit1,
    ND::TGlobalReconModule::TGlobalHit* hit2) {
  float it = hit1->Type + hit2->Type;
  if (it == 112) return fabs(hit1->Position.X() - hit2->Position.X());
  if (it == 121) return fabs(hit1->Position.Y() - hit2->Position.Y());
  if (it == 211) return fabs(hit1->Position.Z() - hit2->Position.Z());
  return 10000;
}
void TDisplayedGlobalModule::AdjustPosition(TVector3& position, int Type,
                                            TVector3 position2) {
  // cout<<" old position "<<position<<endl;

  if (Type == 110) {
    position.SetX(position2.X());
    //  cout<<" new x position "<<position<<endl;
    return;
  }
  if (Type == 101) {
    position.SetY(position2.Y());
    //  cout<<" new y position "<<position<<endl;
    return;
  }
  if (Type == 11) {
    position.SetZ(position2.Z());
    // cout<<"  new z position "<<position<<endl;
    return;
  }
}
void TDisplayedGlobalModule::AddBoxToBoxSet(
    TEveBoxSet* hits, ND::TGlobalReconModule::TGlobalHit* hit) {
  TVector3 position = hit->Position;
  TVector3 uncertainty = hit->PositionError;
  AddBoxToBoxSet(position, uncertainty, hits);
}
void TDisplayedGlobalModule::AddBoxToBoxSet(TVector3 position,
                                            TVector3 uncertainty,
                                            TEveBoxSet* hits) {
  const int npoints = 8;
  const int ndimensions = 3;

  Float_t master[npoints * ndimensions];

  master[0] = position[0] + uncertainty[0] / 2.0;
  master[1] = position[1] - uncertainty[1] / 2.0;
  master[2] = position[2] - uncertainty[2] / 2.0;

  master[3] = master[0];
  master[4] = master[1] + uncertainty[1];
  master[5] = master[2];

  master[6] = master[3] - uncertainty[0];
  master[7] = master[4];
  master[8] = master[2];

  master[9] = master[6];
  master[10] = master[7] - uncertainty[1];
  master[11] = master[2];

  for (int i = 12; i < npoints * ndimensions; i += 3)
    master[i] = master[i - 12];
  for (int i = 13; i < npoints * ndimensions; i += 3)
    master[i] = master[i - 12];
  for (int i = 14; i < npoints * ndimensions; i += 3)
    master[i] = position[2] + uncertainty[2] / 2.0;
  hits->AddBox(master);
  return;
}
void TDisplayedGlobalModule::Describe(int index) {
  cout << endl
       << "=================================================================="
       << endl;
  cout << endl << " This is global track number:" << index << endl;
  cout << endl
       << "=================================================================="
       << endl;
  ND::TGlobalReconModule::TGlobalPID* RO =
      (ND::TGlobalReconModule::TGlobalPID*)fTracks->At(index);
  Describe(RO);
}
void TDisplayedGlobalModule::Centre(ND::TGlobalReconModule::TGlobalPID* RO) {
  float x = (RO->FrontPosition.X() + RO->BackPosition.X()) / 2.0;
  float y = (RO->FrontPosition.Y() + RO->BackPosition.Y()) / 2.0;
  float z = (RO->FrontPosition.Z() + RO->BackPosition.Z()) / 2.0;
  gEve->GetDefaultGLViewer()->CurrentCamera().SetCenterVec(x, y, z);
  // gEve->GetDefaultGLViewer()->UpdateScene();
}
void TDisplayedGlobalModule::Describe(ND::TGlobalReconModule::TGlobalPID* RO) {
  //    cout<<" Charge "<<RO->Charge
  //    <<" Front Position "<<RO->FrontPosition<<" Front Direction "<<endl
  //    <<RO->FrontDirection<<" Front Momentum "<<RO->FrontMomentum<<" "<<endl;
  //    cout<<" Back Position "<<RO->BackPosition<<endl<<" Back Direction
  //    "<<RO->BackDirection<<endl<<
  //    " Back Momentum "<<RO->BackMomentum<<endl;
  //    cout<<" Number of constituents "<<RO->NConstituents<<endl;
  cout << " Hello " << endl;
  Centre(RO);

  Output(RO->AlgorithmName, "AlgorithmName    ",
         "  The name of the algorithm that created this object.");
  Output(RO->BackDirection, "BackDirection", "");
  Output(RO->BackMomentum, "BackMomentum", "");
  Output(RO->BackMomentumError, "BackMomentumError", "");
  Output(RO->BackPosition, "BackPosition", "");
  Output(RO->BackStateCov, "BackStateCov", "");
  Output(RO->Charge, "Charge", "  The Charge.");
  Output(RO->Chi2, "Chi2", "  The chi2 of the fit.");
  Output(RO->Cone, "Cone",
         "  The opening angles of the cone (only for showers).");
  Output(RO->DirectionAtTrueVertex, "DirectionAtTrueVertex", "");
  Output(RO->StateCovAtTrueVertex, "StateCovAtTrueVertex", "");
  Output(RO->EDeposit, "EDeposit", "  The deposited charge for the object.");
  Output(RO->FrontDirection, "FrontDirection",
         "  The direction of the object.");
  Output(RO->FrontMomentum, "FrontMomentum", "");
  Output(RO->FrontMomentumError, "FrontMomentumError", "");
  Output(RO->FrontPosition, "FrontPosition", "  The position of the object.");
  Output(RO->FrontStateCov, "FrontStateCov", "  The front state covariance;.");
  Output(RO->isForward, "isForward", "  Sense of object.");
  Output(RO->Length, "Length",
         "  The length of the track or shower (RMS).");
  Output(RO->MomentumAtTrueVertex, "MomentumAtTrueVertex", "");
  Output(RO->MomentumErrorAtTrueVertex, "MomentumErrorAtTrueVertex", "");
  Output(RO->NConstituents, "NConstituents", "  The number of constituents.");

  Output(RO->NDOF, "NDOF", "  The number of degrees of freedom.");
  Output(RO->NECALs, "NECALs", "");
  Output(RO->NFGDs, "NFGDs", "");
  Output(RO->NHits, "NHits", "  The number of hits.");
  Output(RO->NHitsSaved, "NHitsSaved", "");
  Output(RO->NNodes, "NNodes", "  The number of nodes.");
  Output(RO->NP0Ds, "NP0Ds", "");
  Output(RO->NSMRDs, "NSMRDs", "");
  Output(RO->NTPCs, "NTPCs", "");
  Output(RO->NTRACKERs, "NTRACKERs", "");
  Output(RO->PositionAtTrueVertex, "PositionAtTrueVertex",
         "  Kinematics at vertex.");
  Output(RO->Chi2, "Quality", "   The quality of the fit.(Chi2/NDOF).");
  Output(RO->SenseOK, "SenseOK", "  Sense of object.");
  Output(RO->Status, "Status", "  The status for the fit.");
  Output(RO->UniqueID, "UniqueID",
         " Unique identification code for this object");
  Output(RO->Width, "Width",
         "  The width of the shower (perpendicular to the direction). ");
  cout << " This reconstructed object: " << endl;
  for (int idet = 0; idet < NDETSUSED; idet++) {
    if (RO->DetectorUsed[idet])
      cout << " Uses detector:" << DetectorUsedName(idet) << endl;
  }
  cout << " and... " << endl;
  if (RO->NECALs > 0)
    cout << " Contains " << RO->NECALs << " ECAL objects " << endl;
  if (RO->NFGDs > 0)
    cout << " contains " << RO->NFGDs << " FGD objects " << endl;
  if (RO->NP0Ds > 0)
    cout << " contains " << RO->NP0Ds << " P0D objects " << endl;
  if (RO->NTRACKERs > 0)
    cout << " contains " << RO->NTRACKERs << " Tracker objects " << endl;
  if (RO->NTPCs > 0)
    cout << " contains " << RO->NTPCs << " TPC objects " << endl;
  if (RO->NSMRDs > 0)
    cout << " contains " << RO->NSMRDs << " SMRD objects " << endl;

  cout << endl
       << "===================================================================="
          "================================================================"
       << endl;
}
void TDisplayedGlobalModule::Describe(ND::TGlobalReconModule::TFGDObject* RO) {
  TDisplayedAnalysisModuleBase::Centre(RO);
  // TDisplayedAnalysisModuleBase::Centre((ND::TSubBaseObject*) RO);
  Output(RO->BackDirection, "BackDirection", "");
  Output(RO->BackDirectionVar, "BackDirectionVar", "");
  Output(RO->BackMomentum, "BackMomentum", "");
  Output(RO->BackMomentumError, "BackMomentumError", "");
  Output(RO->BackPosition, "BackPosition", "");
  Output(RO->BackPositionVar, "BackPositionVar", "");
  Output(RO->Chi2, "Chi2", "");
  Output(RO->Detector, "Detector", "");
  Output(RO->EDeposit, "EDeposit", "");
  Output(RO->FrontDirection, "FrontDirection", "The direction of the object.");
  Output(RO->FrontDirectionVar, "FrontDirectionVar",
         "The direction of the object.");
  Output(RO->FrontMomentum, "FrontMomentum", "");
  Output(RO->FrontMomentumError, "FrontMomentumError", "");
  Output(RO->FrontPosition, "FrontPosition", "The position of the object.");
  Output(RO->FrontPositionVar, "FrontPositionVar", "The position variance;.");
  Output(RO->Length, "Length", "The length of the track or shower (RMS).");
  Output(RO->NConstituents, "NConstituents", "");
  Output(RO->NDOF, "NDOF", "");
  Output(RO->NHits, "NHits", "");
  Output(RO->NNodes, "NNodes", "");
  Output(RO->Status, "Status", "The status for the fit.");
  Output(RO->UniqueID, "UniqueID",
         " Unique identification code for this object");
}
void TDisplayedGlobalModule::Describe(ND::TGlobalReconModule::TTPCObject* RO) {
  TDisplayedAnalysisModuleBase::Centre(RO);
  Output(RO->BackDirection, "BackDirection", "");
  Output(RO->BackDirectionVar, "BackDirectionVar", "");
  Output(RO->BackMomentum, "BackMomentum", "");
  Output(RO->BackMomentumError, "BackMomentumError", "");
  Output(RO->BackPosition, "BackPosition", "");
  Output(RO->BackPositionVar, "BackPositionVar", "");
  Output(RO->Ccorr, "Ccorr", "");
  Output(RO->Charge, "Charge", "");
  Output(RO->Chi2, "Chi2", "");
  Output(RO->dEdxexpEle, "dEdxexpEle", "");
  Output(RO->dEdxexpKaon, "dEdxexpKaon", "");
  Output(RO->dEdxexpMuon, "dEdxexpMuon", "");
  Output(RO->dEdxexpPion, "dEdxexpPion", "");
  Output(RO->dEdxexpProton, "dEdxexpProton", "");
  Output(RO->Detector, "Detector", "");
  Output(RO->EDeposit, "EDeposit", "");
  Output(RO->FrontDirection, "FrontDirection", "The direction of the object.");
  Output(RO->FrontDirectionVar, "FrontDirectionVar",
         "The direction of the object.");
  Output(RO->FrontMomentum, "FrontMomentum", "");
  Output(RO->FrontMomentumError, "FrontMomentumError", "");
  Output(RO->FrontPosition, "FrontPosition", "The position of the object.");
  Output(RO->FrontPositionVar, "FrontPositionVar", "The position variance;.");
  Output(RO->Length, "Length", "The length of the track or shower (RMS).");
  Output(RO->NConstituents, "NConstituents", "");
  Output(RO->NDOF, "NDOF", "");
  Output(RO->NHits, "NHits", "");
  Output(RO->NNodes, "NNodes", "");
  Output(RO->NTrun, "NTrun", "");
  Output(RO->PullEle, "PullEle", "");
  Output(RO->PullKaon, "PullKaon", "");
  Output(RO->PullMuon, "PullMuon", "");
  Output(RO->PullPion, "PullPion", "");
  Output(RO->PullProton, "PullProton", "");
  Output(RO->SigmaEle, "SigmaEle", "");
  Output(RO->SigmaKaon, "SigmaKaon", "");
  Output(RO->SigmaMuon, "SigmaMuon", "");
  Output(RO->SigmaPion, "SigmaPion", "");
  Output(RO->SigmaProton, "SigmaProton", "");
  Output(RO->Status, "Status", "The status for the fit.");
  Output(RO->UniqueID, "UniqueID",
         " Unique identification code for this object");
}
void TDisplayedGlobalModule::Describe(ND::TGlobalReconModule::TECALObject* RO) {
  TDisplayedAnalysisModuleBase::Centre(RO);
    Output(RO->Status, "Status", "The status for the fit.");
  Output(RO->UniqueID, "UniqueID",
         " Unique identification code for this object");
  Output(RO->BackDirection, "BackDirection", "");
  Output(RO->BackDirectionVar, "BackDirectionVar", "");
  Output(RO->BackMomentum, "BackMomentum", "");
  Output(RO->BackMomentumError, "BackMomentumError", "");
  Output(RO->BackPosition, "BackPosition", "");
  Output(RO->BackPositionVar, "BackPositionVar", "");
  Output(RO->Chi2, "Chi2", "");
  Output(RO->Cone, "Cone", "The opening angles of the cone (only for showers)");
  Output(RO->Detector, "Detector", "");
  Output(RO->EDeposit, "EDeposit", "");
  Output(RO->FrontDirection, "FrontDirection", "The direction of the object.");
  Output(RO->FrontDirectionVar, "FrontDirectionVar",
         "The direction of the object.");
  Output(RO->FrontMomentum, "FrontMomentum", "");
  Output(RO->FrontMomentumError, "FrontMomentumError", "");
  Output(RO->FrontPosition, "FrontPosition", "The position of the object.");
  Output(RO->FrontPositionVar, "FrontPositionVar", "The position variance");
  Output(RO->NConstituents, "NConstituents", "");
  Output(RO->NDOF, "NDOF", "");
  Output(RO->NHits, "NHits", "");
  Output(RO->NNodes, "NNodes", "");

  // ECal-specific properties
  Output(RO->LLR_MIP_EM, "LLR_MIP_EM", "");
  Output(RO->LLR_MIP_Pion, "LLR_MIP_Pion", "");
  Output(RO->LLR_EM_HIP, "LLR_EM_HIP", "");
  Output(RO->AverageHitTime, "AverageHitTime", "");
  Output(RO->EMEnergyFit_Result, "EMEnergyFit_Result", "");
  Output(RO->EMEnergyFit_Uncertainty, "EMEnergyFit_Uncertainty", "");
  Output(RO->FilledAsType, "FilledAsType", "");
  Output(RO->PID_Circularity, "PID_Circularity", "");
  Output(RO->PID_Angle, "PID_Angle", "");
  Output(RO->PID_ShowerAngle, "PID_ShowerAngle", "");
  Output(RO->PID_Asymmetry, "PID_Asymmetry", "");
  Output(RO->PID_TruncatedMaxRatio, "PID_TruncatedMaxRatio", "");
  Output(RO->PID_TransverseChargeRatio, "PID_TransverseChargeRatio", "");
  Output(RO->PID_FrontBackRatio, "PID_FrontBackRatio", "");
  Output(RO->Containment, "Containment", "");
  Output(RO->InnerMostLayerHit, "InnerMostLayerHit", "");
  Output(RO->OuterMostLayerHit, "OuterMostLayerHit", "");
  Output(RO->Width, "Width", "");

}
void TDisplayedGlobalModule::Describe(ND::TGlobalReconModule::TSMRDObject* RO) {
  TDisplayedAnalysisModuleBase::Centre(RO);
  Output(RO->BackDirection, "BackDirection", "");
  Output(RO->BackDirectionVar, "BackDirectionVar", "");
  Output(RO->BackMomentum, "BackMomentum", "");
  Output(RO->BackMomentumError, "BackMomentumError", "");
  Output(RO->BackPosition, "BackPosition", "");
  Output(RO->BackPositionVar, "BackPositionVar", "");
  Output(RO->Chi2, "Chi2", "");
  Output(RO->Detector, "Detector", "");
  Output(RO->EDeposit, "EDeposit", "");
  Output(RO->FrontDirection, "FrontDirection", "The direction of the object.");
  Output(RO->FrontDirectionVar, "FrontDirectionVar",
         "The direction of the object.");
  Output(RO->FrontMomentum, "FrontMomentum", "");
  Output(RO->FrontMomentumError, "FrontMomentumError", "");
  Output(RO->FrontPosition, "FrontPosition", "The position of the object.");
  Output(RO->FrontPositionVar, "FrontPositionVar", "The position variance;.");
  Output(RO->Length, "Length", "The length of the track or shower (RMS).");
  Output(RO->NConstituents, "NConstituents", "");
  Output(RO->NDOF, "NDOF", "");
  Output(RO->NHits, "NHits", "");
  Output(RO->NNodes, "NNodes", "");
  Output(RO->Status, "Status", "The status for the fit.");
  Output(RO->UniqueID, "UniqueID",
         " Unique identification code for this object");
}

void TDisplayedGlobalModule::Describe(ND::TGlobalReconModule::TP0DObject* P0D) {
  TDisplayedAnalysisModuleBase::Centre(P0D);

  Output(P0D->Cone, "Cone",
         "The opening angles of the cone (only for showers).");
  Output(P0D->Detector, "Detector", "");
  Output(P0D->EDeposit, "EDeposit", "");
  Output(P0D->FrontDirection, "FrontDirection", "The direction of the object.");
  Output(P0D->FrontDirectionVar, "FrontDirectionVar",
         "The direction of the object.");
  Output(P0D->FrontMomentum, "FrontMomentum", "");
  Output(P0D->FrontMomentumError, "FrontMomentumError", "");
  Output(P0D->FrontPosition, "FrontPosition", "The position of the object.");
  Output(P0D->FrontPositionVar, "FrontPositionVar", "The position variance;.");
  Output(P0D->Length, "Length", "The length of the track or shower (RMS).");
  Output(P0D->NConstituents, "NConstituents", "");
  Output(P0D->NDOF, "NDOF", "");
  Output(P0D->NHits, "NHits", "");
  Output(P0D->NNodes, "NNodes", "");
  Output(P0D->Status, "Status", "The status for the fit.");
  Output(P0D->Width, "Width",
         "The width of the shower (perpendicular to the direction). ");
}
void TDisplayedGlobalModule::DescribeShower(TString Name) {
  TString Number(Name(13, 2));
  int N = atoi(Number.Data());

  ND::TGlobalReconModule::TGlobalPID* reconObject =
      (ND::TGlobalReconModule::TGlobalPID*)fTracks->At(N);

  TClonesArray* ECAL = reconObject->ECAL;
  if (reconObject->NECALs < 1) {
    cout << "TDisplayedGlobalModule::DescribeShower: Something is wrong , this "
            "object has no ECAL object, how did I get here?"
         << endl;
    return;
  }
  if (reconObject->NECALs > 1) {
    cout << "TDisplayedGlobalModule::DescribeShower: Surprisingly, this Global "
            "object has more than one ecal object"
         << endl;
  }
  for (int iEcal = 0; iEcal < reconObject->NECALs; iEcal++) {
    ND::TGlobalReconModule::TECALObject* RO =
        (ND::TGlobalReconModule::TECALObject*)ECAL->At(iEcal);
    if (RO->FilledAsType == 1) {
      // shower direction
      cout << "================================================================"
              "================================================"
           << endl;
      cout << " Shower associated to global track number: " << N << ". "
           << endl;
      cout << "================================================================"
              "================================================"
           << endl;
      Describe(RO);
    }
  }
}
void TDisplayedGlobalModule::Describe(
    ND::TGlobalReconModule::TGlobalVertex* RO) {
  TDisplayedAnalysisModuleBase::Centre(RO->Position);
  Output(RO->AlgorithmName, "AlgorithmName",
         "The name of the algorithm that created this object.");
  //  Output(RO->Constituents,"Constituents","");
  //   Output(RO->hackVertexConstituentObject,"hackVertexConstituentObject","
  //   NConstituents");
  Output(RO->NDOF, "NDOF", "The number of degrees of freedom.");
  //    Output(RO->NTrueVertices,"NTrueVertices","");
  Output(RO->Position, "Position", "The position of the vertex.");
  Output(RO->PrimaryIndex, "PrimaryIndex",
         "The index of the corresponding primary vertex.");
  Output(RO->Quality, "Quality", "The quality of the fit.");
  Output(RO->Status, "Status", "The status for the fit.");
  // Output(RO->UniqueID,"UniqueID"," Unique identification code for this
  // object");
  //   Output(RO->TrueVertices,"TrueVertices","");
  Output(RO->Variance, "Variance", "The position variance;.   ");
  Output(RO->NConstituents, "NConstituents", "The number of Constituents.");
  for (int ic = 0; ic < RO->NConstituents; ic++) {
    cout << " Constituent " << ic << endl;
    ND::TGlobalReconModule::TVertexConstituent* Constituent =
        (ND::TGlobalReconModule::TVertexConstituent*)RO->Constituents->At(ic);
    if (Constituent != NULL) Describe(Constituent);
  }
  Output(RO->NTrueVertices, "NTrueVertices", "The number of true vertices.");
  for (int iv = 0; iv < RO->NTrueVertices; iv++) {
    cout << " True Vertex " << iv << endl;
    ND::TTrueVertex* TrueVertex = (ND::TTrueVertex*)RO->TrueVertices->At(iv);
    if (TrueVertex != NULL) Describe(TrueVertex);
  }
}
void TDisplayedGlobalModule::Describe(ND::TTrueVertex* RO) {
  Output(RO->Eff, "Eff", "Efficiency of vertex association of tracks.");
  Output(RO->ID, "ID", "The vertex ID from G4.");
  Output(RO->Position, "Position", "The position of the vertex.");
  Output(RO->Pur, "Pur", "Purity of reconstructed - true vertex association. ");
}
void TDisplayedGlobalModule::Describe(
    ND::TGlobalReconModule::TVertexConstituent* RO) {
  Output(RO->Charge, "Charge", "");
  Output(RO->Momentum, "Momentum", "");
  Output(RO->PID, "PID", "");
  Output(RO->Quality, "Quality", "");
}
