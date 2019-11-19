#include "TDisplayedTrackerECalModule.hxx"
#include "TChain.h"
#include "TEveArrow.h"
#include "TEveBoxSet.h"
#include "TEveManager.h"
#include "TEveTrack.h"
#include "TEveVSDStructs.h"
#include "TOstreamOverloads.hxx"
#include "TSorted4Vector.hxx"

const int NDETSUSED = 19;
static const Int_t VTColors[] = {kCyan, kRed - 4, kOrange - 2};
static const Int_t StateColors[] = {kOrange - 2, kWhite, kCyan};

#ifdef TTRACKERECALRECONMODULE_USE_EXP
using namespace TTrackerECALReconModuleUtils;
#endif

TDisplayedTrackerECalModule::TDisplayedTrackerECalModule()
    : TDisplayedAnalysisModuleBase(
          "ReconDir/TrackerECal",
          "ND::TTrackerECALReconModule::TECALReconObject"),
      MIP_EM_Thresh(0),
      MIP_Pion_Thresh(0),
      EM_HIP_Thresh(0)
#ifdef TTRACKERECALRECONMODULE_USE_EXP
      ,
      fUsedHits(NULL),
      fVertexCandidates(NULL),
      fVertexCandidateTracks(NULL)
#endif
{
  if (!fKchainOK) {
    return;
  }

#ifdef TTRACKERECALRECONMODULE_USE_EXP
  fUsedHits =
      new TClonesArray("ND::TTrackerECALReconModule::TECALReconHit", 1000);
  fVertexCandidates = new TClonesArray(
      "ND::TTrackerECALReconModule::TECALReconVertexCandidate", 100);
  fVertexCandidateTracks = new TClonesArray(
      "ND::TTrackerECALReconModule::TECALReconVertexTrack", 100);
#endif

  if (fChain->SetBranchAddress("NReconObject", &fNTracks) < 0) {
    fKchainOK = kFALSE;
  }
  if ((fTracks != NULL) &&
      fChain->SetBranchAddress("ReconObject", &fTracks) < 0) {
    fKchainOK = kFALSE;
  }
#ifdef TTRACKERECALRECONMODULE_USE_EXP
  if (fChain->SetBranchAddress("NUsedHits", &fNUsedHits) < 0) {
    throw "Failed to Set NVertex Candidates.";
    fKchainOK = kFALSE;
  }
  if ((fUsedHits != NULL) &&
      fChain->SetBranchAddress("UsedHits", &fUsedHits) < 0) {
    throw "Failed to Set NVertex Candidates.";
    fKchainOK = kFALSE;
  }
  if (fChain->SetBranchAddress("NVertexCandidates", &fNVertexCandidates) < 0) {
    throw "Failed to Set NVertex Candidates.";
    fKchainOK = kFALSE;
  }
  if ((fVertexCandidates != NULL) &&
      fChain->SetBranchAddress("VertexCandidates", &fVertexCandidates) < 0) {
    throw "Failed to Set NVertex Candidates.";
    fKchainOK = kFALSE;
  }
  if (fChain->SetBranchAddress("NVertexCandidateTracks",
                               &fNVertexCandidateTracks) < 0) {
    throw "Failed to Set NVertex Candidates.";
    fKchainOK = kFALSE;
  }
  if ((fVertexCandidateTracks != NULL) &&
      fChain->SetBranchAddress("VertexCandidateTracks",
                               &fVertexCandidateTracks) < 0) {
    throw "Failed to Set NVertex Candidates.";
    fKchainOK = kFALSE;
  }
#endif
}

void TDisplayedTrackerECalModule::Draw(int entry, int /*bunch*/) {
  if (!fKchainOK) return;
  TND280TrackList *gTrackList = PrepareToDraw(entry, "TrackerECALRecon Tracks");
  if (gTrackList == NULL) return;
  gTrackList->SetRnrState(kFALSE);
  TEveElementList *ReconstructedShowers =
      new TEveElementList("TrackerECALRecon Shower");
  TString longName = "TrackerECALRecon showers";
  ReconstructedShowers->SetTitle(longName);
  ReconstructedShowers->SetRnrState(true);
  TEveElementList *ReconstructedVertices =
      new TEveElementList("TrackerECALRecon Vertex Candidates");
  TString VClongName = "TrackerECALRecon Vertex candidates";
  ReconstructedVertices->SetTitle(VClongName);
  ReconstructedVertices->SetRnrState(kFALSE);
  TND280TrackPropagator *trkProp = gTrackList->GetPropagator();

  for (int i = 0; i < fNTracks; i++) {
    ND::TTrackerECALReconModule::TECALReconObject *reconObject =
        (ND::TTrackerECALReconModule::TECALReconObject *)fTracks->At(i);
    if (reconObject) {
      bool DrawTrackConstituent =
          ((reconObject->PID_LLR_MIP_EM < MIP_EM_Thresh) +
           (reconObject->PID_LLR_MIP_Pion < MIP_Pion_Thresh) * 2 +
           (reconObject->PID_LLR_EM_HIP > EM_HIP_Thresh) * 4) &
          1;

      std::cout << "[INFO]: Drawing object as "
                << (DrawTrackConstituent ? "Track" : "Shower")
                << " (PID_LLR_MIP_EM: " << reconObject->PID_LLR_MIP_EM
                << (DrawTrackConstituent ? " < " : " > ") << MIP_EM_Thresh
                << ")"
                << " [ (PID_LLR_MIP_Pion: " << reconObject->PID_LLR_MIP_Pion
                << ((reconObject->PID_LLR_MIP_Pion < MIP_Pion_Thresh) ? " < "
                                                                      : " > ")
                << MIP_Pion_Thresh << ")"
                << " (PID_LLR_EM_HIP: " << reconObject->PID_LLR_EM_HIP
                << ((reconObject->PID_LLR_EM_HIP < EM_HIP_Thresh) ? " < "
                                                                  : " > ")
                << EM_HIP_Thresh << ") ]" << std::endl;

      if (DrawTrackConstituent) {
        TND280Track *track = MakeTrack(trkProp, i, &reconObject->Track);
        gTrackList->AddElement(track);
        track->MakeTrack();
        track->SetMainColor(kBlue);
        track->SetLineWidth(4);
      } else {
        ND::TTrackerECALReconModule::TECALReconShower Shower =
            reconObject->Shower;
        TEveVector Direction;
        Direction.Set(Shower.Direction);
        // shower position
        TEveVector Position;
        Position.Set(Shower.Position.Vect());
        float length =
            (Shower.BackPosition.Vect() - Shower.Position.Vect()).Mag();
        if (length < 10) {
          length = 10;
        }
        if (length > 10000) {
          length = 500;
        }
        TVector3 angle3 = Shower.ConeAngle;
        const float aMin =
            0.05;  // cone must have  a finite angle to be drawn!!
        float angle1 = fmax(aMin, angle3.X());
        Direction *= length;
        float r1 = length * tan(angle1);
        // Draw an arrow
        TEveArrow *arrow = new TEveArrow(
            -Direction[0], -Direction[1], -Direction[2],
            Position[0] + Direction[0], Position[1] + Direction[1],
            Position[2] + Direction[2]);

        arrow->SetConeR(r1 / length);
        arrow->SetConeL(1.0);
        arrow->SetDrawQuality(20);
        TString myName(Form("TrackerECALRecon  shower (%2d)", i));
        arrow->SetNameTitle(myName, myName);
        arrow->SetPickable(kTRUE);
        arrow->SetMainColor(kBlue);
        ReconstructedShowers->AddElement(arrow);
        arrow->SetSourceObject(reconObject);
      }
    }
  }  // loop over PIDs

#ifdef TTRACKERECALRECONMODULE_USE_EXP
  ECALUtil.AssignVertexCandidateInfoTCA(*fVertexCandidates, fNVertexCandidates);
  ECALUtil.AssignVertexCandidateTrackInfoTCA(*fVertexCandidateTracks,
                                             fNVertexCandidateTracks);
  std::cout << "Event contains: " << fNVertexCandidates << " vertex candidates."
            << std::endl;
  for (int i = 0; i < fNVertexCandidates; i++) {
    ND::TTrackerECALReconModule::TECALReconVertexCandidate const &VC =
        static_cast<
            ND::TTrackerECALReconModule::TECALReconVertexCandidate const &>(
            *fVertexCandidates->At(i));
    std::cout << "[INFO]: Adding " << VC.NTracks << " vertex candidate track"
              << ((VC.NTracks > 1) ? "s" : "") << " : " << std::endl;
    std::vector<ND::TTrackerECALReconModule::TECALReconVertexTrack const *>
        AssocVTs = ECALUtil.GetVertexCandidateTracks(VC);
    TEveBox *VtxCandy = MakeVertex(VC);

    for (ECalVTVectIt vt_it = AssocVTs.begin(); vt_it < AssocVTs.end();
         ++vt_it) {
      std::cout << "[INFO]: Adding Vertex Track..." << std::endl;
      TND280Track *track = MakeTrack(trkProp, (fNTracks + i), (*(*vt_it)));
      track->MakeTrack();
      track->SetLineWidth(6);
      track->SetLineStyle(1);
      VtxCandy->AddElement(track);
    }
    ReconstructedVertices->AddElement(VtxCandy);
  }
#endif

  if (gTrackList->HasChildren() && ReconstructedShowers->HasChildren()) {
    TEveElementList *base = new TEveElementList("TrackerECALRecon");
    base->AddElement(gTrackList);
    base->AddElement(ReconstructedShowers);
    gEve->AddElement(base);
    gTrackList->SetRnrState(kTRUE);
    ReconstructedShowers->SetRnrState(kTRUE);
    ReconstructedVertices->SetRnrState(kTRUE);
    base->SetRnrState(kFALSE);

  } else {
    if (gTrackList->HasChildren()) {
      gEve->AddElement(gTrackList);
    }
    if (ReconstructedShowers->HasChildren()) {
      gEve->AddElement(ReconstructedShowers);
    }
  }
#ifdef TTRACKERECALRECONMODULE_USE_EXP
  if (ReconstructedVertices->HasChildren()) {
    gEve->AddElement(ReconstructedVertices);
  }
#endif
}

#ifdef TTRACKERECALRECONMODULE_USE_EXP

TEveBox *TDisplayedTrackerECalModule::MakeVertex(
    ND::TTrackerECALReconModule::TECALReconVertexCandidate const &VC, int i) {
  TEveBox *box = new TEveBox(Form(" ECalRecon Vertex candidate %d", i),
                             Form(" ECalRecon Vertex candidate %d", i));
  float xc = VC.Position.X();
  float yc = VC.Position.Y();
  float zc = VC.Position.Z();
  float boxhalfedge = 25;
  box->SetMainColor(kRed + 2);
  box->SetVertex(0, xc - boxhalfedge, yc - boxhalfedge, zc - boxhalfedge);
  box->SetVertex(1, xc + boxhalfedge, yc - boxhalfedge, zc - boxhalfedge);
  box->SetVertex(2, xc + boxhalfedge, yc - boxhalfedge, zc + boxhalfedge);
  box->SetVertex(3, xc - boxhalfedge, yc - boxhalfedge, zc + boxhalfedge);
  box->SetVertex(4, xc - boxhalfedge, yc + boxhalfedge, zc - boxhalfedge);
  box->SetVertex(5, xc + boxhalfedge, yc + boxhalfedge, zc - boxhalfedge);
  box->SetVertex(6, xc + boxhalfedge, yc + boxhalfedge, zc + boxhalfedge);
  box->SetVertex(7, xc - boxhalfedge, yc + boxhalfedge, zc + boxhalfedge);
  box->SetLineWidth(4);
  box->SetSourceObject(
      const_cast<TObject *>(static_cast<TObject const *>(&VC)));
  box->SetPickable(kTRUE);
  return box;
}
#endif

TND280Track *TDisplayedTrackerECalModule::MakeTrack(
    TND280TrackPropagator *trkProp, Int_t index,
    ND::TTrackerECALReconModule::TECALReconTrack *reconObject) {
  // Helper function creating TEveTrack from T2K global recon track.
  //
  TEveRecTrack *rt = new TEveRecTrack();

  rt->fIndex = index;
  // lp208 2016-03-13: curvature doesn't exist any more.
  // rt->fSign = (reconObject->Curvature > 0) ? 1 : -1;
  rt->fV.Set(reconObject->Position.Vect());

  TND280Track *track = new TND280Track(rt, trkProp);

  track->AddPathMark(TEvePathMarkD(TEvePathMarkD::kDecay,
                                   LorentzToEve(reconObject->BackPosition),
                                   reconObject->BackPosition.T()));

  TString myName(Form("TrackerECALRecon track (%2d)", index));
  track->SetName(myName);
  track->SetTitle(myName);
  track->SetSourceObject(reconObject);
  return track;
}

#ifdef TTRACKERECALRECONMODULE_USE_EXP
TND280Track *TDisplayedTrackerECalModule::MakeTrack(
    TND280TrackPropagator *trkProp, Int_t index,
    ND::TTrackerECALReconModule::TECALReconVertexTrack const &VT) {
  // Helper function creating TEveTrack from T2K global recon track.
  //
  TEveRecTrack *rt = new TEveRecTrack();

  rt->fIndex = index;

  rt->fV.Set(VT.FrontPosition.Vect());

  TND280Track *track = new TND280Track(rt, trkProp);

  track->AddPathMark(TEvePathMarkD(TEvePathMarkD::kDecay,
                                   LorentzToEve(VT.BackPosition),
                                   VT.BackPosition.T()));

  TString myName(Form("TrackerECALRecon vertex track (%2d)", index));
  track->SetName(myName);
  track->SetTitle(myName);
  track->SetSourceObject(
      const_cast<TObject *>(static_cast<TObject const *>(&VT)));
  track->SetMainColor(VTColors[VT.Status]);

  TEveBox *boxFrntP =
      new TEveBox(Form(" ECalRecon VertexTrack End FrntP %d", index * 2),
                  Form(" ECalRecon VertexTrack End FrntP %d", index * 2));
  float xcFrntP = VT.FrontPosition.X();
  float ycFrntP = VT.FrontPosition.Y();
  float zcFrntP = VT.FrontPosition.Z();

  TEveBox *boxBckP =
      new TEveBox(Form(" ECalRecon VertexTrack End BckP %d", index * 2 + 1),
                  Form(" ECalRecon VertexTrack End BckP %d", index * 2 + 1));
  float xcBckP = VT.BackPosition.X();
  float ycBckP = VT.BackPosition.Y();
  float zcBckP = VT.BackPosition.Z();

  float boxhalfedge = 5;
  boxFrntP->SetMainColor(kSpring);
  boxFrntP->SetVertex(0, xcFrntP - boxhalfedge, ycFrntP - boxhalfedge,
                      zcFrntP - boxhalfedge);
  boxFrntP->SetVertex(1, xcFrntP + boxhalfedge, ycFrntP - boxhalfedge,
                      zcFrntP - boxhalfedge);
  boxFrntP->SetVertex(2, xcFrntP + boxhalfedge, ycFrntP - boxhalfedge,
                      zcFrntP + boxhalfedge);
  boxFrntP->SetVertex(3, xcFrntP - boxhalfedge, ycFrntP - boxhalfedge,
                      zcFrntP + boxhalfedge);
  boxFrntP->SetVertex(4, xcFrntP - boxhalfedge, ycFrntP + boxhalfedge,
                      zcFrntP - boxhalfedge);
  boxFrntP->SetVertex(5, xcFrntP + boxhalfedge, ycFrntP + boxhalfedge,
                      zcFrntP - boxhalfedge);
  boxFrntP->SetVertex(6, xcFrntP + boxhalfedge, ycFrntP + boxhalfedge,
                      zcFrntP + boxhalfedge);
  boxFrntP->SetVertex(7, xcFrntP - boxhalfedge, ycFrntP + boxhalfedge,
                      zcFrntP + boxhalfedge);
  boxFrntP->SetLineWidth(2);
  boxFrntP->SetSourceObject(
      const_cast<TObject *>(static_cast<TObject const *>(&VT)));
  boxFrntP->SetPickable(false);

  boxBckP->SetMainColor(kSpring);
  boxBckP->SetVertex(0, xcBckP - boxhalfedge, ycBckP - boxhalfedge,
                     zcBckP - boxhalfedge);
  boxBckP->SetVertex(1, xcBckP + boxhalfedge, ycBckP - boxhalfedge,
                     zcBckP - boxhalfedge);
  boxBckP->SetVertex(2, xcBckP + boxhalfedge, ycBckP - boxhalfedge,
                     zcBckP + boxhalfedge);
  boxBckP->SetVertex(3, xcBckP - boxhalfedge, ycBckP - boxhalfedge,
                     zcBckP + boxhalfedge);
  boxBckP->SetVertex(4, xcBckP - boxhalfedge, ycBckP + boxhalfedge,
                     zcBckP - boxhalfedge);
  boxBckP->SetVertex(5, xcBckP + boxhalfedge, ycBckP + boxhalfedge,
                     zcBckP - boxhalfedge);
  boxBckP->SetVertex(6, xcBckP + boxhalfedge, ycBckP + boxhalfedge,
                     zcBckP + boxhalfedge);
  boxBckP->SetVertex(7, xcBckP - boxhalfedge, ycBckP + boxhalfedge,
                     zcBckP + boxhalfedge);
  boxBckP->SetLineWidth(2);
  boxBckP->SetSourceObject(
      const_cast<TObject *>(static_cast<TObject const *>(&VT)));
  boxBckP->SetPickable(false);

  // track->AddElement(boxFrntP);
  // track->AddElement(boxBckP);

  float xcFrntD = VT.FrontDirection.X()*100.;
  float ycFrntD = VT.FrontDirection.Y()*100.;
  float zcFrntD = VT.FrontDirection.Z()*100.;

  float xcBckD = VT.BackDirection.X()*100.;
  float ycBckD = VT.BackDirection.Y()*100.;
  float zcBckD = VT.BackDirection.Z()*100.;

  TEveArrow *FrntDirn =
      new TEveArrow(xcFrntD, ycFrntD, zcFrntD, xcFrntP, ycFrntP, zcFrntP);
  FrntDirn->SetMainColor(StateColors[VT.Status]);
  track->AddElement(FrntDirn);

  TEveArrow *BckDirn =
      new TEveArrow(xcBckD, ycBckD, zcBckD, xcBckP, ycBckP, zcBckP);
  BckDirn->SetMainColor(StateColors[VT.Status]);
  track->AddElement(BckDirn);

  return track;
}
#endif

void TDisplayedTrackerECalModule::DescribeShower(TString Name) {
  TString Number(Name(25, 2));
  int N = atoi(Number.Data());
  //    cout<<" Shower number "<<N<<endl;
  ND::TTrackerECALReconModule::TECALReconObject *reconObject =
      (ND::TTrackerECALReconModule::TECALReconObject *)fTracks->At(N);
  if (reconObject != NULL) {
    ND::TTrackerECALReconModule::TECALReconShower RO = reconObject->Shower;
    cout << "========================================================" << endl;
    cout << "Shower " << N << " reconstructed by TrackerECal reconstruction "
         << endl;
    cout << "========================================================" << endl;

    Describe(&RO);
    Describe(reconObject);
  }
}
void TDisplayedTrackerECalModule::Describe(
    ND::TTrackerECALReconModule::TECALReconShower *Shower) {
  Centre(Shower->Position, Shower->BackPosition);
  Output(Shower->BackPosition, "BackPosition", "The back position");
  Output(Shower->ConeAngle, "ConeAngle", "The shower opening angle.");
  Output(Shower->Direction, "Direction", "The shower direction.");
  Output(Shower->EDeposit, "EDeposit", "The energy deposited in the shower.");
  Output(Shower->Position, "Position", "The shower starting position.");
  Output(Shower->PositionVar, "PositionVar", "");
}
void TDisplayedTrackerECalModule::DescribeTrack(int index) {
  cout << "=======================================================" << endl;
  cout << "Track " << index << " reconstructed by TrackerECal reconstruction "
       << endl;
  cout << "=======================================================" << endl;
  ND::TTrackerECALReconModule::TECALReconObject *RO =
      (ND::TTrackerECALReconModule::TECALReconObject *)fTracks->At(index);
  ND::TTrackerECALReconModule::TECALReconTrack Track = RO->Track;
  Describe(&Track);
  Describe(RO);
}

void TDisplayedTrackerECalModule::Describe(
    ND::TTrackerECALReconModule::TECALReconTrack *Track) {
  Centre(Track->Position);
  Output(Track->Position, "Position", "");
  Output(Track->PositionVar, "PositionVar", "");
  Output(Track->Direction, "Direction", "");
  Output(Track->EDeposit, "EDeposit", " The energy deposited in the Track->");
  ;
  cout << endl << endl;
}
void TDisplayedTrackerECalModule::Describe(
    ND::TTrackerECALReconModule::TECALReconObject *O) {
  Output(O->AverageHitTime, "AverageHitTime", "");
  Output(O->AverageZPosition, "AverageZPosition", "");
  Output(O->EMEnergyFit_Likelihood_energy_qsumGrad,
         "EMEnergyFit_Likelihood_energy_qsumGrad", "");
  Output(O->EMEnergyFit_Likelihood_energyGrad,
         "EMEnergyFit_Likelihood_energyGrad", "");
  Output(O->EMEnergyFit_Likelihood_qsum_like,
         "EMEnergyFit_Likelihood_qsum_like", "");
  Output(O->EMEnergyFit_Para_QMean, "EMEnergyFit_Para_QMean", "");
  Output(O->EMEnergyFit_Para_QRMS, "EMEnergyFit_Para_QRMS", "");
  Output(O->EMEnergyFit_Para_QSkew, "EMEnergyFit_Para_QSkew", "");
  Output(O->EMEnergyFit_Para_QSum, "EMEnergyFit_Para_QSum", "");
  Output(O->EMEnergyFit_Result, "EMEnergyFit_Result", "");
  Output(O->EMEnergyFit_Uncertainty, "EMEnergyFit_Uncertainty", "");

  Output(O->Module, "Module", "");
  Output(O->MostDownStreamLayerHit, "MostDownStreamLayerHit", "");
  Output(O->MostUpStreamLayerHit, "MostUpStreamLayerHit", "");
  Output(O->NHits, "NHits", "");
  Output(O->NIsXHits, "NIsXHits", "");
  Output(O->NIsYHits, "NIsYHits", "");
  Output(O->NIsZHits, "NIsZHits", "");
  Output(O->NLayersHit, "NLayersHit", "");
  Output(O->ObjectLength, "ObjectLength", "");

  Output(O->PID_Circularity, "PID_Circularity",
         "The combined \"circularity\" of the object.");
  Output(O->PID_Angle, "PID_Angle",
         "The zenith angle with respect to each detector.");
  Output(O->PID_ShowerAngle, "PID_ShowerAngle",
         "The angle from the start of an object to its width at its charge "
         "centre");
  Output(O->PID_Asymmetry, "PID_Asymmetry",
         "Ratio of the big width of an object by its small width.");
  Output(O->PID_TruncatedMaxRatio, "PID_TruncatedMaxRatio",
         "A truncated Max Ratio. See ecalRecon docs for the full definition.");
  Output(O->PID_TransverseChargeRatio, "PID_TransverseChargeRatio",
         "A variable sensitive to the charge distribution in the plane "
         "transverse to a shower/track direction");
  Output(O->PID_FrontBackRatio, "PID_FrontBackRatio",
         "The ratio of the charge in equal length blocks at each end of the "
         "track.");
  Output(O->Containment, "Containment",
         "Containment: contained = 1.0, not-contained = 0.0, default = -1.0.");

  Output(O->Thrust, "Thrust", "");
  Output(O->ThrustAxis, "ThrustAxis", "");
  Output(O->ThrustOrigin, "ThrustOrigin", "");

  Output(O->TotalHitCharge, "TotalHitCharge", "");
  Output(O->UniqueID, "UniqueID",
         "Unique ID number to allow matching to Global Recon object. ");
}
