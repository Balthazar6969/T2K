#define DataClasses_C

#include "DataClasses.hxx"
#include "BasicUtils.hxx"
#include "TMath.h"

// define a constant value for uninitialised parameters
const Float_t  kFloatUnassigned = -999.;
const UInt_t   kUnassigned      = 0xDEADBEEF;
const short    kShortUnassigned = 0xFF;


//********************************************************************
AnaSubTrack::AnaSubTrack(){
//********************************************************************
//  NHits          = -999;
  Length         = -999;
  //  anaUtils::ReserveArray(DirectionEnd, 3);
  //  UniqueID       = -999;
}

//********************************************************************
AnaSubTrack::AnaSubTrack(const AnaSubTrack& seg){
//********************************************************************
//  NHits          = seg.NHits;
  Length         = seg.Length;
  //  anaUtils::CopyArray(seg.DirectionEnd, DirectionEnd, 3);
  //  UniqueID       = seg.UniqueID;
}

//********************************************************************
void AnaSubTrack::Print() const{
//********************************************************************

  std::cout << "-------- AnaSubTrack --------- " << std::endl;

  //  std::cout << "NHits:        " << NHits << std::endl;
  std::cout << "Length:       " << Length << std::endl;
  //  std::cout << "UniqueID:     " << UniqueID << std::endl;
}

//********************************************************************
AnaParticleE::AnaParticleE(){
//********************************************************************

  MomentumError = -999;

  NDOF          = -999;
  Chi2          = -999;
  Bunch         = -999;
  ReconPDG      = -999;

  for (int i=0; i<2; i++) {
    UpstreamHits_Position[i]   = TVector3(-999, -999, -999);
    DownstreamHits_Position[i] = TVector3(-999, -999, -999);
    UpstreamHits_Charge[i]   = -999;
    DownstreamHits_Charge[i] = -999;
  }

  MomentumEle         = -999;
  MomentumMuon        = -999;
  MomentumProton      = -999;
  MomentumErrorEle    = -999;
  MomentumErrorMuon   = -999;
  MomentumErrorProton = -999;

#if VERSION_HAS_REVERSED_REFITS
  anaUtils::ReserveArray(PositionStartFlip,  4);
  anaUtils::ReserveArray(DirectionStartFlip, 3);
  anaUtils::ReserveArray(DirectionEndFlip,   3);
#endif


#if VERSION_HAS_PRANGE_ESTIMATES
  RangeMomentumEle         = -999;
  RangeMomentumProton      = -999;
  RangeMomentumMuonFlip    = -999;
  RangeMomentumProtonFlip  = -999;
#endif

  ReconVertex = NULL;
  ReconVertices.clear();
  anaUtils::ReserveArray(DirectionAtVertex, 3);
  MomentumAtVertex    = -999;

#if VERSION_HAS_TIME_FITS
  TimeNodes.clear();
#endif
}

//********************************************************************
AnaParticleE::~AnaParticleE(){
//********************************************************************


#if VERSION_HAS_TIME_FITS
  for (UInt_t i=0;i<TimeNodes.size();i++)
    delete TimeNodes[i];

  TimeNodes.clear();
#endif

}

//********************************************************************
AnaParticleE::AnaParticleE(const AnaParticleE& track){
//********************************************************************

  MomentumError = track.MomentumError;

  NDOF          = track.NDOF;
  Chi2          = track.Chi2;
  Bunch         = track.Bunch;
  ReconPDG      = track.ReconPDG;

  for (int i=0; i<2; i++) {
    UpstreamHits_Position[i]   = track.UpstreamHits_Position[i];
    DownstreamHits_Position[i] = track.DownstreamHits_Position[i];
    UpstreamHits_Charge[i]     = track.UpstreamHits_Charge[i];
    DownstreamHits_Charge[i]   = track.DownstreamHits_Charge[i];
  }

  MomentumEle         = track.MomentumEle;
  MomentumMuon        = track.MomentumMuon;
  MomentumProton      = track.MomentumProton;
  MomentumErrorEle    = track.MomentumErrorEle;
  MomentumErrorMuon   = track.MomentumErrorMuon;
  MomentumErrorProton = track.MomentumErrorProton;

#if VERSION_HAS_REVERSED_REFITS
  anaUtils::CopyArray(track.PositionStartFlip,   PositionStartFlip,  4);
  anaUtils::CopyArray(track.DirectionStartFlip,  DirectionStartFlip, 3);
  anaUtils::CopyArray(track.DirectionEndFlip,    DirectionEndFlip,   3);
#endif

#if VERSION_HAS_PRANGE_ESTIMATES
  RangeMomentumEle         = track.RangeMomentumEle;
  RangeMomentumProton      = track.RangeMomentumProton;
  RangeMomentumMuonFlip    = track.RangeMomentumMuonFlip;
  RangeMomentumProtonFlip  = track.RangeMomentumProtonFlip;
#endif

  // The most lower PrimaryIndex associated global vertex (if it exists).
  ReconVertex = NULL;//track.ReconVertex;

  ReconVertices.clear();
  /*
  for (UInt_t i=0;i<track.ReconVertices.size();i++)
    ReconVertices.push_back(track.ReconVertices[i]);
  */
  anaUtils::CopyArray(track.DirectionAtVertex, DirectionAtVertex, 3);
  MomentumAtVertex = track.MomentumAtVertex;


#if VERSION_HAS_TIME_FITS
  TimeNodes.clear();
  for (UInt_t i=0;i<track.TimeNodes.size();i++)
    TimeNodes.push_back(track.TimeNodes[i]->Clone());
#endif

}

//********************************************************************
void AnaParticleE::Print() const{
//********************************************************************

  std::cout << "-------- AnaParticleE --------- " << std::endl;

  std::cout << "Chi2:                    " << Chi2 << std::endl;
  std::cout << "MomentumMuon:            " << MomentumMuon << std::endl;
  std::cout << "MomentumEle:             " << MomentumEle << std::endl;
  std::cout << "MomentumProton:          " << MomentumProton << std::endl;

#if VERSION_HAS_PRANGE_ESTIMATES
  std::cout << "RangeMomentumEle:        " << RangeMomentumEle << std::endl;
  std::cout << "RangeMomentumProton:     " << RangeMomentumProton << std::endl;
  std::cout << "RangeMomentumMuonFlip:   " << RangeMomentumMuonFlip << std::endl;
  std::cout << "RangeMomentumProtonFlip: " << RangeMomentumProtonFlip << std::endl;
#endif

#if VERSION_HAS_REVERSED_REFITS
  dump_array4(PositionStartFlip);
  dump_array3(DirectionStartFlip);
  dump_array3(DirectionEndFlip);
#endif

  std::cout << "Bunch:                   " << Bunch << std::endl;
  std::cout << "NReconVertices:          " << (int)ReconVertices.size() << std::endl;
  std::cout << "MomentumAtVertex:        " << MomentumAtVertex << std::endl;
  std::cout << "DirectionAtVertex:       " << DirectionAtVertex[0] << " " << DirectionAtVertex[1] << " " << DirectionAtVertex[2] << std::endl;
}




//********************************************************************
AnaTrueVertex::AnaTrueVertex(): AnaTrueVertexB(){
//********************************************************************

  RooVtxIndex   = -999;
  RooVtxEntry   = -999;
  ReacCode      = -999;
  TargetPDG     = -999;
  anaUtils::ReserveArray(NuDir, 3);
  NuParentPDG   = -999;
  anaUtils::ReserveArray(NuParentDecPoint, 4);

  NBaryons      = -999;
  Q2            = -999;
  LeptonPDG     = -999;
  anaUtils::ReserveArray(TargetDir, 3);
  anaUtils::ReserveArray(LeptonDir, 3);
  anaUtils::ReserveArray(ProtonDir, 3);
  anaUtils::ReserveArray(PionDir, 3);
  TargetMom     = -999;
  LeptonMom     = -999;
  ProtonMom     = -999;
  PionMom       = -999;

  IsPauliBlocked = false;
  IsCohOnH = false;

  for (UInt_t i=0; i<(ParticleId::kLast+1); i++)
      NPrimaryParticles[i]=0;

  AccumLevel.clear();

  ReconParticles.clear();
  ReconVertices.clear();
}

//********************************************************************
AnaTrueVertex::AnaTrueVertex(const AnaTrueVertex& vertex):AnaTrueVertexB(vertex){
//********************************************************************

  RooVtxIndex   = vertex.RooVtxIndex;
  RooVtxEntry   = vertex.RooVtxEntry;
  ReacCode      = vertex.ReacCode;
  TargetPDG     = vertex.TargetPDG;
  anaUtils::CopyArray(vertex.NuDir, NuDir, 3);
  NuParentPDG   = vertex.NuParentPDG;
  anaUtils::CopyArray(vertex.NuParentDecPoint, NuParentDecPoint, 3);

  NBaryons      = vertex.NBaryons;
  Q2            = vertex.Q2;
  LeptonPDG     = vertex.LeptonPDG;
  anaUtils::CopyArray(vertex.TargetDir, TargetDir, 3);
  anaUtils::CopyArray(vertex.LeptonDir, LeptonDir, 3);
  anaUtils::CopyArray(vertex.ProtonDir, ProtonDir, 3);
  anaUtils::CopyArray(vertex.PionDir,   PionDir,   3);
  TargetMom     = vertex.TargetMom;
  LeptonMom     = vertex.LeptonMom;
  ProtonMom     = vertex.ProtonMom;
  PionMom       = vertex.PionMom;

  IsPauliBlocked = vertex.IsPauliBlocked;
  IsCohOnH       = vertex.IsCohOnH;

  for (UInt_t i=0; i<(ParticleId::kLast+1); i++)
      NPrimaryParticles[i]=vertex.NPrimaryParticles[i];
/*
  PreFSIParticles.clear();
  for (UInt_t i=0;i<vertex.PreFSIParticles.size();i++)
    PreFSIParticles.push_back(vertex.PreFSIParticles[i]);
*/
  AccumLevel.resize(vertex.AccumLevel.size());
  for (UInt_t i=0;i<vertex.AccumLevel.size();i++){
    AccumLevel[i].resize(vertex.AccumLevel[i].size());
    for (UInt_t j=0;j<vertex.AccumLevel[i].size();j++){
      AccumLevel[i][j]=vertex.AccumLevel[i][j];
    }
  }

  ReconParticles.clear();
  /*
  for (UInt_t i=0;i<vertex.ReconParticles.size();i++)
    ReconParticles.push_back(vertex.ReconParticles[i]);
  */
  ReconVertices.clear();
  /*
  for (UInt_t i=0;i<vertex.ReconVertices.size();i++)
    ReconVertices.push_back(vertex.ReconVertices[i]);
  */
}

//********************************************************************
void AnaTrueVertex::Print() const{
//********************************************************************

  std::cout << "-------- AnaTrueVertex --------- " << std::endl;

  AnaTrueVertexB::Print();

  std::cout << "RooVtxIndex:      " << RooVtxIndex << std::endl;
  std::cout << "RooVtxEntry:      " << RooVtxEntry << std::endl;
  std::cout << "ReacCode:         " << ReacCode << std::endl;
  std::cout << "TargetPDG:        " << TargetPDG << std::endl;


  std::cout << "NuDir:            " << NuDir[0]  << " " << NuDir[1]  << " " << NuDir[2] << std::endl;

  std::cout << "NuParentPDG:      " << NuParentPDG << std::endl;
  std::cout << "NuParentDecPoint: " << NuParentDecPoint[0] << " " << NuParentDecPoint[1]  << " " << NuParentDecPoint[2] << " " << NuParentDecPoint[3] << std::endl;


  std::cout << "NBaryons:         " << NBaryons << std::endl;
  std::cout << "LeptonPDG:        " << LeptonPDG << std::endl;
  std::cout << "Q2:               " << Q2 << std::endl;

  std::cout << "TargetDir:        " << TargetDir[0]  << " " << TargetDir[1]  << " " << TargetDir[2] << std::endl;
  std::cout << "LeptonDir:        " << LeptonDir[0]  << " " << LeptonDir[1]  << " " << LeptonDir[2] << std::endl;
  std::cout << "ProtonDir:        " << ProtonDir[0]  << " " << ProtonDir[1]  << " " << ProtonDir[2] << std::endl;
  std::cout << "PionDir:          " << PionDir[0]    << " " << PionDir[1]    << " " << PionDir[2]   << std::endl;

  std::cout << "TargetMom:        " << TargetMom << std::endl;
  std::cout << "LeptonMom:        " << LeptonMom << std::endl;
  std::cout << "ProtonMom:        " << ProtonMom << std::endl;
  std::cout << "PionMom:          " << PionMom << std::endl;

  std::cout << "NReconParticles:  " << (int)ReconParticles.size() << std::endl;
  std::cout << "NReconVertices:   " << (int)ReconVertices.size() << std::endl;
}

//********************************************************************
AnaTrueParticle::AnaTrueParticle(): AnaTrueParticleB(){
//********************************************************************

  Purity                           = -999;
  Bunch                            = -999;
  VertexIndex                      = -999;
  
  ReconParticles.clear();
}

//********************************************************************
AnaTrueParticle::AnaTrueParticle(const AnaTrueParticle& truePart):AnaTrueParticleB(truePart){
//********************************************************************

  Purity                           = truePart.Purity;
  Bunch                            = truePart.Bunch;
  VertexIndex                      = truePart.VertexIndex;
  
  ReconParticles.clear();
  /*
  for (UInt_t i=0;i<truePart.ReconParticles.size();i++)
    ReconParticles.push_back(truePart.ReconParticles[i]);
  */
}

//********************************************************************
void AnaTrueParticle::Print() const{
//********************************************************************

  std::cout << "-------- AnaTrueParticle --------- \n";

  AnaTrueParticleB::Print();

  std::cout << "NReconParticles: "                  << (int)ReconParticles.size()       << "\n"
      << "VertexIndex:     "                  << VertexIndex                      << "\n"
      << "IsTruePrimaryPi0DecayPhoton: "      << IsTruePrimaryPi0DecayPhoton      << "\n"
      << "IsTruePrimaryPi0DecayPhotonChild: " << IsTruePrimaryPi0DecayPhotonChild << std::endl;
}


//********************************************************************
AnaTPCParticle::AnaTPCParticle():AnaTPCParticleB(),AnaSubTrack() {
//********************************************************************

#if VERSION_HAS_BFIELD_REFIT_FULL
  RefitCharge    = -999;
  anaUtils::ReserveArray(RefitDirection,3);
  anaUtils::ReserveArray(RefitPosition,4);
#endif
  Pullmu  = -999;
  Pullele = -999;
  Pullp   = -999;
  Pullpi  = -999;
  Pullk   = -999;
  dEdxexpKaon     = -999;
  dEdxSigmaKaon   = -999;
}

//********************************************************************
AnaTPCParticle::AnaTPCParticle(const AnaTPCParticle& seg):AnaTPCParticleB(seg),AnaSubTrack(seg){
//********************************************************************

#if VERSION_HAS_BFIELD_REFIT_FULL
  RefitCharge         = seg.RefitCharge;
  anaUtils::CopyArray(seg.RefitDirection, RefitDirection,3);
  anaUtils::CopyArray(seg.RefitPosition,  RefitPosition,4);
#endif
  Pullmu  = seg.Pullmu;
  Pullele = seg.Pullele;
  Pullp   = seg.Pullp;
  Pullpi  = seg.Pullpi;
  Pullk   = seg.Pullk;
  dEdxexpKaon   = seg.dEdxexpKaon;
  dEdxSigmaKaon   = seg.dEdxSigmaKaon;
}

//********************************************************************
void AnaTPCParticle::Print() const{
//********************************************************************

  std::cout << "-------- AnaTPCParticle --------- " << std::endl;

  AnaTPCParticleB::Print();
  std::cout << "Pullmu:        " << Pullmu << std::endl;
  std::cout << "Pullele:       " << Pullele << std::endl;
  std::cout << "Pullp:         " << Pullp << std::endl;
  std::cout << "Pullpi:        " << Pullpi << std::endl;
  std::cout << "Pullk:         " << Pullk << std::endl;
}

//********************************************************************
AnaTPCParticle::~AnaTPCParticle(){
//********************************************************************
}


//********************************************************************
AnaFGDParticle::AnaFGDParticle():AnaFGDParticleB(),AnaSubTrack() {
//********************************************************************

  E       = -999;
  AvgTime = -999;

#if VERSION_HAS_FGD_VERTEX_AND_END_ACTIVITY
  HasFgdVA = false;
  HasFgdVA_fmode = -999;
  TypeVA = -999;
#endif

  Vertex1x1 = -999;
  Vertex3x3 = -999;
  Vertex5x5 = -999;
  Vertex7x7 = -999;
  VertexLayer = -999;

#if VERSION_HAS_FGD_VERTEX_AND_END_ACTIVITY
  HasFgdEA = false;
  HasFgdEA_fmode = -999;
  TypeEA = -999;

  End0x0 = -999;
  End1x1 = -999;
  End3x3 = -999;
  End5x5 = -999;
  End7x7 = -999;
#endif

}

//********************************************************************
AnaFGDParticle::AnaFGDParticle(const AnaFGDParticle& seg):AnaFGDParticleB(seg),AnaSubTrack(seg){
//********************************************************************

  E  = seg.E;
  AvgTime     = seg.AvgTime;

#if VERSION_HAS_FGD_VERTEX_AND_END_ACTIVITY
  HasFgdVA = seg.HasFgdVA;
  HasFgdVA_fmode = seg.HasFgdVA_fmode;
  TypeVA = seg.TypeVA;
#endif

  Vertex1x1 = seg.Vertex1x1;
  Vertex3x3 = seg.Vertex3x3;
  Vertex5x5 = seg.Vertex5x5;
  Vertex7x7 = seg.Vertex7x7;
  VertexLayer = seg.VertexLayer;

#if VERSION_HAS_FGD_VERTEX_AND_END_ACTIVITY
  HasFgdEA = seg.HasFgdEA;
  HasFgdEA_fmode = seg.HasFgdEA_fmode;
  TypeEA = seg.TypeEA;
  End0x0 = seg.End0x0;
  End1x1 = seg.End1x1;
  End3x3 = seg.End3x3;
  End5x5 = seg.End5x5;
  End7x7 = seg.End7x7;
#endif
}

//********************************************************************
AnaECALParticle::AnaECALParticle() : AnaECALParticleB() {
  //********************************************************************

  PIDMipPion = -9999;
  Containment = -9999;

#if VERSION_PROD7_DEVEL

  PIDMIPEM_LowMomentum = -9999;
  AverageZPosition = -9999;
  EMEnergyFit_Uncertainty = -9999;
  FilledAsType = -9999;
  PID_Circularity = -9999;
  PID_Angle = -9999;
  PID_ShowerAngle = -9999;
  PID_Asymmetry = -9999;
  PID_TruncatedMaxRatio = -9999;
  PID_TransverseChargeRatio = -9999;
  PID_FrontBackRatio = -9999;
  PIDMipHip_VACut = -9999;
  Thrust = -9999;
  anaUtils::ReserveArray(ThrustOrigin, 3, -9999);
  anaUtils::ReserveArray(ThrustAxis, 3, -9999);
  InnerMostLayerHit = -9999;
  OuterMostLayerHit = -9999;
  anaUtils::ReserveArray(Width, 3, -9999);
#endif
}

//********************************************************************
AnaECALParticle::AnaECALParticle(const AnaECALParticle& seg)
    : AnaECALParticleB(seg) {
  //********************************************************************

  PIDMipPion = seg.PIDMipPion;
  Containment = seg.Containment;

#if VERSION_PROD7_DEVEL

  PIDMIPEM_LowMomentum = seg.PIDMIPEM_LowMomentum;
  AverageZPosition = seg.AverageZPosition;
  EMEnergyFit_Uncertainty = seg.EMEnergyFit_Uncertainty;
  FilledAsType = seg.FilledAsType;
  PID_Circularity = seg.PID_Circularity;
  PID_Angle = seg.PID_Angle;
  PID_ShowerAngle = seg.PID_ShowerAngle;
  PID_Asymmetry = seg.PID_Asymmetry;
  PID_TruncatedMaxRatio = seg.PID_TruncatedMaxRatio;
  PID_TransverseChargeRatio = seg.PID_TransverseChargeRatio;
  PID_FrontBackRatio = seg.PID_FrontBackRatio;
  PIDMipHip_VACut = seg.PIDMipHip_VACut;
  Thrust = seg.Thrust;
  anaUtils::CopyArray(seg.ThrustOrigin, ThrustOrigin,3);
  anaUtils::CopyArray(seg.ThrustAxis, ThrustAxis,3);
  InnerMostLayerHit = seg.InnerMostLayerHit;
  OuterMostLayerHit = seg.OuterMostLayerHit;
  anaUtils::CopyArray(seg.Width, Width,3);
#endif
}

//********************************************************************
AnaSMRDParticle::AnaSMRDParticle():AnaSMRDParticleB(),AnaSubTrack() {
//********************************************************************

  EDeposit = -9999;

}

//********************************************************************
AnaSMRDParticle::AnaSMRDParticle(const AnaSMRDParticle& seg):AnaSMRDParticleB(seg),AnaSubTrack(seg){
//********************************************************************

  EDeposit = seg.EDeposit;

}

//********************************************************************
AnaTrackerTrack::AnaTrackerTrack():AnaTrackerTrackB(),AnaSubTrack() {
//********************************************************************

}

//********************************************************************
AnaTrackerTrack::AnaTrackerTrack(const AnaTrackerTrack& seg):AnaTrackerTrackB(seg),AnaSubTrack(seg){
//********************************************************************


}

//********************************************************************
AnaTrack::AnaTrack():AnaTrackB(){
//********************************************************************

  Length    = -999;
  Detectors = -999;

  TRACKERSegments.clear();
}

//********************************************************************
AnaTrack::~AnaTrack(){
//********************************************************************

  for (UInt_t i=0;i<TRACKERSegments.size();i++)
    delete TRACKERSegments[i];

  TRACKERSegments.clear();

}

//********************************************************************
AnaTrack::AnaTrack(const AnaTrack& track):AnaTrackB(track),AnaParticleE(track){
//********************************************************************

  Length        = track.Length;
  Detectors     = track.Detectors;

  TRACKERSegments.clear();
  for (UInt_t i=0;i<track.TRACKERSegments.size();i++)
    TRACKERSegments.push_back(track.TRACKERSegments[i]->Clone());

}

//********************************************************************
void AnaTrack::Print() const{
//********************************************************************

  std::cout << "-------- AnaTrack --------- " << std::endl;

  AnaParticleE::Print();
  AnaTrackB::Print();

  std::cout << "Length:               " << Length                      << std::endl;
  std::cout << "NTRACKERs:            " << (int)TRACKERSegments.size() << std::endl;
}


//********************************************************************
AnaTimeNode::AnaTimeNode(){
//********************************************************************

  Detector  = -999;
  TimeStart = -999;
  TimeEnd   = -999;
}

//********************************************************************
AnaTimeNode::AnaTimeNode(const AnaTimeNode& anaTime){
//********************************************************************

  Detector  = anaTime.Detector;
  TimeStart = anaTime.TimeStart;
  TimeEnd   = anaTime.TimeEnd;
}

//********************************************************************
AnaBunch::AnaBunch(){
//********************************************************************

}

//********************************************************************
AnaBunch::~AnaBunch(){
//********************************************************************

}

//********************************************************************
AnaBunch::AnaBunch(const AnaBunch& bunch):AnaBunchB(bunch){
  //AnaBunch::AnaBunch(const AnaBunch& bunch){
//********************************************************************

/*
    Bunch  = bunch.Bunch;
    Weight = bunch.Weight;

    Tracks.clear();
    for (UInt_t i=0;i<bunch.Tracks.size();i++){
        AnaTrackB* clonedTrack = bunch.Tracks[i]->Clone();
        (static_cast<AnaTrack*>(clonedTrack))->ReconVertices.clear();
        Tracks.push_back(clonedTrack);
    }

    Vertices.clear();
    for (UInt_t i=0;i<bunch.Vertices.size();i++){
      AnaVertexB* clonedVertex = bunch.Vertices[i]->Clone();
      Vertices.push_back(clonedVertex);
      for (Int_t j=0;j<clonedVertex->nTracks;j++){
        for (UInt_t k=0;k<Tracks.size();k++){
          if (clonedVertex->Tracks[j] == bunch.Tracks[k]){
            clonedVertex->Tracks[j] = Tracks[k];
            (static_cast<AnaTrack*>(Tracks[k]))->ReconVertices.push_back(clonedVertex);
          }
        }
      }
    }
*/
}

//********************************************************************
void AnaBunch::Print() const{
//********************************************************************

  AnaBunchB::Print();

}

//********************************************************************
AnaFgdTimeBin::AnaFgdTimeBin():AnaFgdTimeBinB(){
//********************************************************************

  MaxTime = -999;
  G4ID    = -999;

  FgdHits = NULL;
  nFgdHits = 0;
  FgdHitsVect.clear();

}

//********************************************************************
AnaFgdTimeBin::~AnaFgdTimeBin(){
//********************************************************************

  MaxTime = -999;
  G4ID = -999;

  // It owns the hits!
  if (FgdHits){
    for (int i = 0; i < nFgdHits; i++){
      if (FgdHits[i]) delete FgdHits[i];
      FgdHits[i] = NULL;
    }
  }
  delete [] FgdHits;
  FgdHits = NULL;
  nFgdHits = 0;

  FgdHitsVect.clear();

}


//********************************************************************
AnaFgdTimeBin::AnaFgdTimeBin(const AnaFgdTimeBin& bin): AnaFgdTimeBinB(bin){
//********************************************************************

  MaxTime = bin.MaxTime;
  G4ID    = bin.G4ID;

  nFgdHits = bin.nFgdHits;
  anaUtils::CreateArray(FgdHits, nFgdHits);
  for (int i = 0; i < nFgdHits; i++){
    FgdHits[i] = bin.FgdHits[i]->Clone();
  }

  FgdHitsVect.clear();
}

//********************************************************************
void AnaFgdTimeBin::Print() const{
//********************************************************************

  std::cout << "-------- AnaFgdTimeBin --------- " << std::endl;

  AnaFgdTimeBinB::Print();

  std::cout << "MaxTime:  "  << MaxTime  << std::endl;
  std::cout << "G4ID:  "     << G4ID     << std::endl;
  std::cout << "NFgdHits:  " << nFgdHits << std::endl;

}

//********************************************************************
void AnaFgdTimeBin::CopyArraysIntoVectors(){
//********************************************************************
  FgdHitsVect.clear();
  for (int i = 0; i < nFgdHits; i++){
    if (!FgdHits[i]) continue;
    FgdHits[i]->CopyArraysIntoVectors();
    FgdHitsVect.push_back(FgdHits[i]);
  }
}

//*****************************************************************************
void AnaFgdTimeBin::CopyVectorsIntoArrays(){
//*****************************************************************************
  nFgdHits = 0;
  anaUtils::CreateArray(FgdHits, FgdHitsVect.size());
  for (std::vector<AnaHit*>::iterator it = FgdHitsVect.begin();
      it != FgdHitsVect.end(); it++){
    if (!*it) continue;
    (*it)->CopyVectorsIntoArrays();
    FgdHits[nFgdHits++] = *it;
  }
}

//********************************************************************
AnaSpill::AnaSpill():AnaSpillB(){
//********************************************************************

  InputFileIndex=-999;
  RooVtxEntry = -999;
}

//********************************************************************
AnaSpill::~AnaSpill(){
//********************************************************************
}

//********************************************************************
AnaSpill::AnaSpill(const AnaSpill& spill):AnaSpillB(spill){
//********************************************************************

  RooVtxEntry = spill.RooVtxEntry;
  InputFileIndex = spill.InputFileIndex;
  Trigger = spill.Trigger;
}

//********************************************************************
void AnaSpill::CopyArraysIntoVectors(){
//********************************************************************

  for (std::vector<AnaFgdTimeBinB*>::iterator it = FgdTimeBins.begin();
      it != FgdTimeBins.end(); it++){
    AnaFgdTimeBin* bin = dynamic_cast<AnaFgdTimeBin*>(*it);
    if (!bin) continue;
    bin->CopyArraysIntoVectors();
  }

  AnaSpillB::CopyArraysIntoVectors();

}

//*****************************************************************************
void AnaSpill::CopyVectorsIntoArrays(){
//*****************************************************************************
  for (std::vector<AnaFgdTimeBinB*>::iterator it = FgdTimeBins.begin();
      it != FgdTimeBins.end(); it++){
    AnaFgdTimeBin* bin = dynamic_cast<AnaFgdTimeBin*>(*it);
    if (!bin) continue;
    bin->CopyVectorsIntoArrays();
  }

  AnaSpillB::CopyVectorsIntoArrays();

}
//********************************************************************
void AnaSpill::RedoLinks(){
//********************************************************************

  // Reset the true->Reco links since truth is not cloned and the vectors of ReconParticles and ReconVertices are already filled
  for (UInt_t i=0;i<TrueVertices.size();i++){
    (static_cast<AnaTrueVertex*>(TrueVertices[i]))->ReconParticles.clear();
    (static_cast<AnaTrueVertex*>(TrueVertices[i]))->ReconVertices.clear();
  }
  for (UInt_t i=0;i<TrueParticles.size();i++)
    (static_cast<AnaTrueParticle*>(TrueParticles[i]))->ReconParticles.clear();

  // Redo the links
  AnaSpillB::RedoLinks();
}

//********************************************************************
void AnaSpill::associateVertexToParticle(AnaParticleB* particle, AnaVertexB* vertex) const{
//********************************************************************

  // TODO. IS the dynamic_cast OK here ?
//  if (!SubDetId::TrackUsesOnlyDet(particle->Detector,SubDetId::kP0D))
    //    (static_cast<AnaParticleE*>(particle))->ReconVertices.push_back(vertex);
  AnaTrack* track = dynamic_cast<AnaTrack*>(particle);
  if (track){
    track->ReconVertices.push_back(vertex);
    return;
  }
  AnaParticle* part = dynamic_cast<AnaParticle*>(particle);
  if (part){
    part->ReconVertices.push_back(vertex);
    return;
  }


}

//********************************************************************
void AnaSpill::associateVertexToTrueVertex(AnaVertexB* vertex) const{
//********************************************************************

  if (vertex->TrueVertex)
    (static_cast<AnaTrueVertex*>(vertex->TrueVertex))->ReconVertices.push_back(vertex);
}

//********************************************************************
void AnaSpill::associateParticleToTrueParticle(AnaParticleB* particle) const{
//********************************************************************

  if (particle->TrueObject){
    (static_cast<AnaTrueParticle*>(particle->TrueObject))->ReconParticles.push_back(particle);
    if (particle->GetTrueParticle()->TrueVertex)
      (static_cast<AnaTrueVertex*>(particle->GetTrueParticle()->TrueVertex))->ReconParticles.push_back(particle);
  }
}

//********************************************************************
void AnaSpill::Print() const{
//********************************************************************

  std::cout << "-------- AnaSpill --------- " << std::endl;

  AnaSpillB::Print();
}


//********************************************************************
AnaEvent::AnaEvent():AnaEventB(){
//********************************************************************

}

//********************************************************************
AnaEvent::~AnaEvent(){
//********************************************************************

}

//********************************************************************
AnaEvent::AnaEvent(const AnaEvent& event):AnaEventB(event){
//********************************************************************

  Trigger = event.Trigger;

}

//*****************************************************************************
AnaEvent::AnaEvent(const AnaSpill& spill, const AnaBunch& bunch):AnaEventB(spill,bunch) {
//*****************************************************************************

  Trigger = spill.Trigger;
}

//********************************************************************
void AnaEvent::Print() const{
//********************************************************************

  std::cout << "-------- AnaEvent --------- " << std::endl;

  AnaEventB::Print();
  Trigger.Print();
}

//********************************************************************
AnaHit::AnaHit(){
//********************************************************************

  anaUtils::ReserveArray(Position, 4);

  Type     = kUnknown;
  Detector = 0;
  Layer    = -999;
  Charge   = -999;

  Particles      = NULL;
  nParticles     = 0;
  TrueParticles  = NULL;
  nTrueParticles = 0;

  ParticlesVect.clear();
  TrueParticlesVect.clear();
  TrueG4IDs.clear();

}
//********************************************************************
AnaHit::~AnaHit(){
//********************************************************************

  nParticles = 0;
  if (Particles) delete [] Particles;
  Particles = NULL;

  nTrueParticles = 0;
  if (TrueParticles) delete [] TrueParticles;
  TrueParticles = NULL;

  ParticlesVect.clear();
  TrueParticlesVect.clear();
}

//********************************************************************
AnaHit::AnaHit(const AnaHit& hit){
//********************************************************************

  anaUtils::CopyArray(hit.Position, Position, 4);

  Type     = hit.Type;
  Detector = hit.Detector;
  Layer    = hit.Layer;
  Charge   = hit.Charge;

  nParticles = hit.nParticles;
  anaUtils::CreateArray(Particles, nParticles);
  anaUtils::CopyArray(hit.Particles, Particles, nParticles);

  nTrueParticles = hit.nTrueParticles;
  anaUtils::CreateArray(TrueParticles, nTrueParticles);
  anaUtils::CopyArray(hit.TrueParticles, TrueParticles, nTrueParticles);

  TrueG4IDs.clear();
  for (size_t i = 0; i < hit.TrueG4IDs.size(); i++){
    TrueG4IDs.push_back(hit.TrueG4IDs[i]);
  }

  // These are for utility usage
  ParticlesVect.clear();
  TrueParticlesVect.clear();

}

//********************************************************************
void AnaHit::Print() const{
//********************************************************************

  std::cout << "-------- AnaHit --------- " << std::endl;

  dump_array4(Position);

  std::cout << "Type:           " << Type           << std::endl;
  std::cout << "Detector:       " << Detector       << std::endl;
  std::cout << "Layer:          " << Layer          << std::endl;
  std::cout << "Charge:         " << Charge         << std::endl;
  std::cout << "NParticles:     " << nParticles     << std::endl;
  std::cout << "NTrueParticles: " << nTrueParticles << std::endl;

}
//********************************************************************
void AnaHit::CopyArraysIntoVectors(){
//********************************************************************
  ParticlesVect.clear();
  for (int i = 0; i < nParticles; i++){
    if (!Particles[i]) continue;
    ParticlesVect.push_back(Particles[i]);
  }

  TrueParticlesVect.clear();
  for (int i = 0; i < nTrueParticles; i++){
    if (!TrueParticles[i]) continue;
    TrueParticlesVect.push_back(TrueParticles[i]);
  }
}

//*****************************************************************************
void AnaHit::CopyVectorsIntoArrays(){
//*****************************************************************************
  nParticles = 0;
  anaUtils::CreateArray(Particles, ParticlesVect.size());
  for (std::vector<AnaParticleB*>::iterator it = ParticlesVect.begin();
      it != ParticlesVect.end(); it++){
    if (!*it) continue;
    Particles[nParticles++] = *it;
  }

  nTrueParticles = 0;
  anaUtils::CreateArray(TrueParticles, TrueParticlesVect.size());
  for (std::vector<AnaTrueParticleB*>::iterator it = TrueParticlesVect.begin();
      it != TrueParticlesVect.end(); it++){
    if (!*it) continue;
    TrueParticles[nTrueParticles++] = *it;
  }
}




//********************************************************************
AnaLocalReconEvent::AnaLocalReconEvent():AnaEvent(){
//********************************************************************
  TECALReconObjects    .clear();
  TECALUnmatchedObjects.clear();
  PECALReconObjects    .clear();
  PECALUnmatchedObjects.clear();
  FGDOnlyReconObjects  .clear();
  P0DReconVertices     .clear();
  P0DReconParticles    .clear();
  P0DReconClusters     .clear();
}

//********************************************************************
AnaLocalReconEvent::~AnaLocalReconEvent(){
//********************************************************************
  for (std::vector<AnaTECALReconObject*>::iterator it = TECALReconObjects.begin();
       it != TECALReconObjects.end(); ++it) {
    delete (*it);
  }
  TECALReconObjects.clear();

  for (std::vector<AnaTECALUnmatchedObject*>::iterator it = TECALUnmatchedObjects.begin();
       it != TECALUnmatchedObjects.end(); ++it) {
    delete (*it);
  }
  TECALUnmatchedObjects.clear();

  for (std::vector<AnaPECALReconObject*>::iterator it = PECALReconObjects.begin();
       it != PECALReconObjects.end(); ++it) {
    delete (*it);
  }
  PECALReconObjects.clear();

  for (std::vector<AnaPECALUnmatchedObject*>::iterator it = PECALUnmatchedObjects.begin();
       it != PECALUnmatchedObjects.end(); ++it) {
    delete (*it);
  }
  PECALUnmatchedObjects.clear();

  for (std::vector<AnaP0DReconVertex*>::iterator it = P0DReconVertices.begin();
       it != P0DReconVertices.end(); ++it) {
    delete (*it);
  }
  P0DReconVertices.clear();

  for (std::vector<AnaP0DReconParticle*>::iterator it = P0DReconParticles.begin();
       it != P0DReconParticles.end(); ++it) {
    delete (*it);
  }
  P0DReconParticles.clear();

  for (std::vector<AnaP0DReconCluster*>::iterator it = P0DReconClusters.begin();
       it != P0DReconClusters.end(); ++it) {
    delete (*it);
  }
  P0DReconClusters.clear();

  for (std::vector<AnaFGDOnlyReconObject*>::iterator it = FGDOnlyReconObjects.begin();
       it != FGDOnlyReconObjects.end(); ++it) {
    delete (*it);
  }
  FGDOnlyReconObjects.clear();
}

//********************************************************************
AnaLocalReconEvent::AnaLocalReconEvent(const AnaLocalReconEvent& event):AnaEvent(event){
//********************************************************************

  TECALReconObjects.clear();
  for (std::vector<AnaTECALReconObject*>::const_iterator it = event.TECALReconObjects.begin();
       it != event.TECALReconObjects.end(); ++it) {
    TECALReconObjects.push_back((*it)->Clone());
  }

  TECALUnmatchedObjects.clear();
  for (std::vector<AnaTECALUnmatchedObject*>::const_iterator it = event.TECALUnmatchedObjects.begin();
       it != event.TECALUnmatchedObjects.end(); ++it) {
    TECALUnmatchedObjects.push_back((*it)->Clone());
  }

  PECALReconObjects.clear();
  for (std::vector<AnaPECALReconObject*>::const_iterator it = event.PECALReconObjects.begin();
       it != event.PECALReconObjects.end(); ++it) {
    PECALReconObjects.push_back((*it)->Clone());
  }

  PECALUnmatchedObjects.clear();
  for (std::vector<AnaPECALUnmatchedObject*>::const_iterator it = event.PECALUnmatchedObjects.begin();
       it != event.PECALUnmatchedObjects.end(); ++it) {
    PECALUnmatchedObjects.push_back((*it)->Clone());
  }

  FGDOnlyReconObjects.clear();
  for (std::vector<AnaFGDOnlyReconObject*>::const_iterator it = event.FGDOnlyReconObjects.begin();
       it != event.FGDOnlyReconObjects.end(); ++it) {
    FGDOnlyReconObjects.push_back((*it)->Clone());
  }

  P0DReconVertices.clear();
  for (std::vector<AnaP0DReconVertex*>::const_iterator it = event.P0DReconVertices.begin();
       it != event.P0DReconVertices.end(); ++it) {
    P0DReconVertices.push_back((*it)->Clone());
  }

  P0DReconParticles.clear();
  for (std::vector<AnaP0DReconParticle*>::const_iterator it = event.P0DReconParticles.begin();
       it != event.P0DReconParticles.end(); ++it) {
    P0DReconParticles.push_back((*it)->Clone());
  }

  P0DReconClusters.clear();
  for (std::vector<AnaP0DReconCluster*>::const_iterator it = event.P0DReconClusters.begin();
       it != event.P0DReconClusters.end(); ++it) {
    P0DReconClusters.push_back((*it)->Clone());
  }
}

//*****************************************************************************
AnaLocalReconEvent::AnaLocalReconEvent(const AnaSpill& spill, const AnaLocalReconBunch& bunch):AnaEvent(spill,bunch) {
//*****************************************************************************

  TECALReconObjects.clear();
  for (std::vector<AnaTECALReconObject*>::const_iterator it = bunch.TECALReconObjects.begin();
       it != bunch.TECALReconObjects.end(); ++it) {
    TECALReconObjects.push_back((*it)->Clone());
  }

  TECALUnmatchedObjects.clear();
  for (std::vector<AnaTECALUnmatchedObject*>::const_iterator it = bunch.TECALUnmatchedObjects.begin();
       it != bunch.TECALUnmatchedObjects.end(); ++it) {
    TECALUnmatchedObjects.push_back((*it)->Clone());
  }

  PECALReconObjects.clear();
  for (std::vector<AnaPECALReconObject*>::const_iterator it = bunch.PECALReconObjects.begin();
       it != bunch.PECALReconObjects.end(); ++it) {
    PECALReconObjects.push_back((*it)->Clone());
  }

  PECALUnmatchedObjects.clear();
  for (std::vector<AnaPECALUnmatchedObject*>::const_iterator it = bunch.PECALUnmatchedObjects.begin();
       it != bunch.PECALUnmatchedObjects.end(); ++it) {
    PECALUnmatchedObjects.push_back((*it)->Clone());
  }

  FGDOnlyReconObjects.clear();
  for (std::vector<AnaFGDOnlyReconObject*>::const_iterator it = bunch.FGDOnlyReconObjects.begin();
       it != bunch.FGDOnlyReconObjects.end(); ++it) {
    FGDOnlyReconObjects.push_back((*it)->Clone());
  }

  P0DReconVertices.clear();
  for (std::vector<AnaP0DReconVertex*>::const_iterator it = bunch.P0DReconVertices.begin();
       it != bunch.P0DReconVertices.end(); ++it) {
    P0DReconVertices.push_back((*it)->Clone());
  }

  P0DReconParticles.clear();
  for (std::vector<AnaP0DReconParticle*>::const_iterator it = bunch.P0DReconParticles.begin();
       it != bunch.P0DReconParticles.end(); ++it) {
    P0DReconParticles.push_back((*it)->Clone());
  }

  P0DReconClusters.clear();
  for (std::vector<AnaP0DReconCluster*>::const_iterator it = bunch.P0DReconClusters.begin();
       it != bunch.P0DReconClusters.end(); ++it) {
    P0DReconClusters.push_back((*it)->Clone());
  }
}

//********************************************************************
AnaVertex::AnaVertex():AnaVertexB(){
//********************************************************************

  Bunch        = -999;
  anaUtils::ReserveArray(Variance, 4);
  NDOF         = -999;
  Chi2         = -999;
}

//********************************************************************
AnaVertex::AnaVertex(const AnaVertex& vertex):AnaVertexB(vertex){
//********************************************************************

  Bunch        = vertex.Bunch;
  anaUtils::CopyArray(vertex.Variance, Variance, 4);
  NDOF         = vertex.NDOF;
  Chi2         = vertex.Chi2;
}

//********************************************************************
void AnaVertex::Print() const{
//********************************************************************

  std::cout << "-------- AnaVertex --------- " << std::endl;

  AnaVertexB::Print();

  std::cout << "Bunch:               " << Bunch << std::endl;
  std::cout << "Variance:            " << Variance[0] << " " << Variance[1] << " " << Variance[2] << " " << Variance[3] << std::endl;
  std::cout << "NDOF:                " << NDOF << std::endl;
  std::cout << "Chi2:                " << Chi2 << std::endl;
}

//********************************************************************
AnaEventInfo::AnaEventInfo():AnaEventInfoB(){
//********************************************************************

  EventTime = -999;
}

//********************************************************************
AnaEventInfo::AnaEventInfo(const AnaEventInfo& spill):AnaEventInfoB(spill){
//********************************************************************

  EventTime = spill.EventTime;
}

//********************************************************************
void AnaEventInfo::Print() const{
//********************************************************************

  std::cout << "-------- AnaEventInfo --------- " << std::endl;

  AnaEventInfoB::Print();

  std::cout << "EventTime:          " << EventTime << std::endl;
}

//********************************************************************
AnaDataQuality::AnaDataQuality(){
//********************************************************************

  ND280Flag   = -999;
  for (UInt_t i=0;i<7;i++){
    DetFlag[i] = -999;
  }
}

//********************************************************************
AnaDataQuality::AnaDataQuality(const AnaDataQuality& dq):AnaDataQualityB(dq){
//********************************************************************

  ND280Flag   = dq.ND280Flag;
  for (UInt_t i=0;i<7;i++)
    DetFlag[i] = dq.DetFlag[i];
}

//********************************************************************
AnaBeam::AnaBeam(){
//********************************************************************

  POT           = -999;
#if !VERSION_HAS_OFFICIAL_POT
  POTCT4        = -999;
#endif
  Spill         = -999;
  SpillNumber   = -999;
  BeamRunNumber = -999;
}

//********************************************************************
AnaBeam::AnaBeam(const AnaBeam& beam):AnaBeamB(beam){
//********************************************************************

  POT           = beam.POT;
#if !VERSION_HAS_OFFICIAL_POT
  POTCT4        = beam.POTCT4;
#endif
  Spill         = beam.Spill;
  SpillNumber   = beam.SpillNumber;
  BeamRunNumber = beam.BeamRunNumber;
}

//********************************************************************
AnaTrigger::AnaTrigger(): CTMTriggerPattern(6, 0) {
//********************************************************************
  FGDCosmic   = false;
  TripTCosmic = false;
}

//********************************************************************
AnaTrigger::AnaTrigger(const AnaTrigger& tr) {
//********************************************************************
  FGDCosmic   = tr.FGDCosmic;
  TripTCosmic = tr.TripTCosmic;
  CTMTriggerPattern.clear();

  size_t npatterns = tr.CTMTriggerPattern.size();

  for (size_t i = 0; i < npatterns; i++) {
    CTMTriggerPattern.push_back(tr.CTMTriggerPattern[i]);
  }
}

//********************************************************************
void AnaTrigger::Print() const{
//********************************************************************

  std::cout << "-------- AnaTrigger --------- " << std::endl;
  if (FGDCosmic)
    std::cout << "Trigger:\t FGDCosmic" << std::endl;
  else if (TripTCosmic)
    std::cout << "Trigger:\t TripTCosmic" << std::endl;
  else
    std::cout << "Trigger:\t Unknown" << std::endl;

  dump_vector(CTMTriggerPattern, 6);

}

//********************************************************************
AnaTECALReconObject::AnaTECALReconObject(){
//********************************************************************

  AverageHitTime                  = kUnassigned;
  AverageZPos                     = kUnassigned;
  Containment                     = kUnassigned;
  EFitResult                      = kUnassigned;
  EFitUncertainty                 = kUnassigned;
  FirstLayer                      = kUnassigned;
  IsShowerLike                    = kUnassigned;
  IsTrackLike                     = kUnassigned;
  LastLayer                       = kUnassigned;
  LikeMIPEM                       = kUnassigned;
  LikeMIPPion                     = kUnassigned;
  LikeEMHIP                       = kUnassigned;
  LikeMIPEMLow                    = kUnassigned;
  MatchingLike                    = kUnassigned;
  MichelTagNDelayedCluster        = kUnassigned;
  Module                          = "UNKOWN";
  MostDownStreamLayerHit          = kUnassigned;
  MostUpStreamLayerHit            = kUnassigned;
  NHits                           = kUnassigned;
  NLayersHit                      = kUnassigned;
  ObjectLength                    = kUnassigned;
  PIDAMR                          = kUnassigned;
  PIDAngle                        = kUnassigned;
  PIDAsymmetry                    = kUnassigned;
  PIDCircularity                  = kUnassigned;
  PIDFBR                          = kUnassigned;
  PIDMaxRatio                     = kUnassigned;
  PIDMeanPos                      = kUnassigned;
  PIDShowerAngle                  = kUnassigned;
  PIDShowerWidth                  = kUnassigned;
  PIDTransverseChargeRatio        = kUnassigned;
  PIDTruncatedMaxRatio            = kUnassigned;
  anaUtils::VectorToArray            (TVector3(0,0,0), Pointing    );
  Thrust                          = kUnassigned;
  anaUtils::VectorToArray            (TVector3(0,0,0), ThrustAxis  );
  anaUtils::VectorToArray            (TVector3(0,0,0), ThrustOrigin);
  TimeBunch                       = kUnassigned;
  TotalHitCharge                  = kUnassigned;
  TrueID                          = kUnassigned;
  TrueIDPrimary                   = kUnassigned;
  TrueIDRecursive                 = kUnassigned;
  TrueIDSingle                    = kUnassigned;
  UniqueID                        = kUnassigned;

  Bunch                           = kUnassigned;

}

//********************************************************************
AnaTECALReconObject::AnaTECALReconObject(const AnaTECALReconObject& tecalRecon) {
//********************************************************************

  AverageHitTime           = tecalRecon.AverageHitTime;
  AverageZPos              = tecalRecon.AverageZPos;
  Containment              = tecalRecon.Containment;
  EFitResult               = tecalRecon.EFitResult;
  EFitUncertainty          = tecalRecon.EFitUncertainty;
  FirstLayer               = tecalRecon.FirstLayer;
  IsShowerLike             = tecalRecon.IsShowerLike;
  IsTrackLike              = tecalRecon.IsTrackLike;
  LastLayer                = tecalRecon.LastLayer;
  LikeMIPEM                = tecalRecon.LikeMIPEM;
  LikeMIPPion              = tecalRecon.LikeMIPPion;
  LikeEMHIP                = tecalRecon.LikeEMHIP;
  LikeMIPEMLow             = tecalRecon.LikeMIPEMLow;
  MatchingLike             = tecalRecon.MatchingLike;
  MichelTagNDelayedCluster = tecalRecon.MichelTagNDelayedCluster;
  Module                   = tecalRecon.Module;
  MostDownStreamLayerHit   = tecalRecon.MostDownStreamLayerHit;
  MostUpStreamLayerHit     = tecalRecon.MostUpStreamLayerHit;
  NHits                    = tecalRecon.NHits;
  NLayersHit               = tecalRecon.NLayersHit;
  ObjectLength             = tecalRecon.ObjectLength;
  PIDAMR                   = tecalRecon.PIDAMR;
  PIDAngle                 = tecalRecon.PIDAngle;
  PIDAsymmetry             = tecalRecon.PIDAsymmetry;
  PIDCircularity           = tecalRecon.PIDCircularity;
  PIDFBR                   = tecalRecon.PIDFBR;
  PIDMaxRatio              = tecalRecon.PIDMaxRatio;
  PIDMeanPos               = tecalRecon.PIDMeanPos;
  PIDShowerAngle           = tecalRecon.PIDShowerAngle;
  PIDShowerWidth           = tecalRecon.PIDShowerWidth;
  PIDTransverseChargeRatio = tecalRecon.PIDTransverseChargeRatio;
  PIDTruncatedMaxRatio     = tecalRecon.PIDTruncatedMaxRatio;
  anaUtils::CopyArray         (tecalRecon.Pointing,     Pointing,     3);
  Thrust                   = tecalRecon.Thrust;
  anaUtils::CopyArray         (tecalRecon.ThrustAxis,   ThrustAxis,   3);
  anaUtils::CopyArray         (tecalRecon.ThrustOrigin, ThrustOrigin, 3);
  TimeBunch                = tecalRecon.TimeBunch;
  TotalHitCharge           = tecalRecon.TotalHitCharge;
  TrueID                   = tecalRecon.TrueID;
  TrueIDPrimary            = tecalRecon.TrueIDPrimary;
  TrueIDRecursive          = tecalRecon.TrueIDRecursive;
  TrueIDSingle             = tecalRecon.TrueIDSingle;
  UniqueID                 = tecalRecon.UniqueID;

  Bunch                    = tecalRecon.Bunch;

}

//********************************************************************
void AnaTECALReconObject::Print() const {
//********************************************************************

  std::cout<<"-- AnaTECALReconObject.Print() --\n"
           <<"NHits               : "<<NHits         <<"\n"
           <<"Bunch               : "<<Bunch         <<"\n"
           <<"FirstLayer          : "<<FirstLayer    <<"\n"
           <<"LastLayer           : "<<LastLayer     <<"\n"
           <<"EFitResult          : "<<EFitResult    <<"\n"
           <<"TotalHitCharge      : "<<TotalHitCharge<<"\n"
           <<"IsTrack(Shower)Like : "<<IsTrackLike<<"("<<IsShowerLike<<")"
           <<std::endl;

}

//********************************************************************
AnaTECALUnmatchedObject::AnaTECALUnmatchedObject(){
//********************************************************************

  AverageHitTime                = kUnassigned;
  anaUtils::VectorToArray          (TVector3(0,0,0), BackPos );
  anaUtils::VectorToArray          (TVector3(0,0,0), FrontPos);
  MostDownStreamLayerHit        = kUnassigned;
  MostUpStreamLayerHit          = kUnassigned;
  NHits                         = kUnassigned;
  TotalHitCharge                = kUnassigned;
  TrueID                        = kUnassigned;
  TrueIDPrimary                 = kUnassigned;
  TrueIDRecursive               = kUnassigned;
  TrueIDSingle                  = kUnassigned;
  View                          = kUnassigned;

  Bunch                         = kUnassigned;

}

//********************************************************************
AnaTECALUnmatchedObject::AnaTECALUnmatchedObject(const AnaTECALUnmatchedObject& tecalUnmatched){
//********************************************************************

  AverageHitTime         = tecalUnmatched.AverageHitTime;
  anaUtils::CopyArray        (tecalUnmatched.BackPos,  BackPos,  3);
  anaUtils::CopyArray        (tecalUnmatched.FrontPos, FrontPos, 3);
  MostDownStreamLayerHit = tecalUnmatched.MostDownStreamLayerHit;
  MostUpStreamLayerHit   = tecalUnmatched.MostUpStreamLayerHit;
  NHits                  = tecalUnmatched.NHits;
  TotalHitCharge         = tecalUnmatched.TotalHitCharge;
  TrueID                 = tecalUnmatched.TrueID;
  TrueIDPrimary          = tecalUnmatched.TrueIDPrimary;
  TrueIDRecursive        = tecalUnmatched.TrueIDRecursive;
  TrueIDSingle           = tecalUnmatched.TrueIDSingle;
  View                   = tecalUnmatched.View;

  Bunch                  = tecalUnmatched.Bunch;

}

//********************************************************************
void AnaTECALUnmatchedObject::Print() const {
//********************************************************************

  std::cout<<"-- AnaTECALUnmatchedObject.Print() --\n"
           <<"NHits                  : "<<NHits                  <<"\n"
           <<"Bunch                  : "<<Bunch                  <<"\n"
           <<"MostUpStreamLayerHit   : "<<MostUpStreamLayerHit   <<"\n"
           <<"MostDownStreamLayerHit : "<<MostDownStreamLayerHit <<"\n"
           <<"TotalHitCharge         : "<<TotalHitCharge         <<"\n"
           <<"View                   : "<<View
           <<std::endl;

}

//********************************************************************
AnaP0DReconObject::AnaP0DReconObject(){
//********************************************************************

  VerticesP     .clear();
  ParticlesP    .clear();
  ClustersP     .clear();

  Vertices      .clear();
  Particles     .clear();
  Tracks        .clear();
  Showers       .clear();
  Clusters      .clear();
  Nodes         .clear();
  Hits          .clear();
  AlgorithmName = "";
  Cycle         = kShortUnassigned;
  NHits         = kShortUnassigned;
  UniqueID      = kUnassigned;
  Bunch         = kUnassigned;
  ValidDimensions      = kShortUnassigned;

  Truth_PrimaryTrajIDs .clear();
  Truth_TrajIDs        .clear();
  Truth_HitCount       .clear();
  Truth_ChargeShare    .clear();
  anaUtils::VectorToArray (TLorentzVector(0,0,0,0), Position   );
  anaUtils::VectorToArray (TLorentzVector(0,0,0,0), PosVariance);

}

//********************************************************************
AnaP0DReconObject::AnaP0DReconObject(const AnaP0DReconObject& p0dObject){
//********************************************************************

  VerticesP      = p0dObject.VerticesP;
  ParticlesP     = p0dObject.ParticlesP;
  ClustersP      = p0dObject.ClustersP;

  Vertices      = p0dObject.Vertices;
  Particles     = p0dObject.Particles;
  Tracks        = p0dObject.Tracks;
  Showers       = p0dObject.Showers;
  Clusters      = p0dObject.Clusters;
  Nodes         = p0dObject.Nodes;
  Hits          = p0dObject.Hits;
  AlgorithmName = p0dObject.AlgorithmName;
  Cycle         = p0dObject.Cycle;
  NHits         = p0dObject.NHits;
  UniqueID      = p0dObject.UniqueID;
  Bunch         = p0dObject.Bunch;

  ValidDimensions      = p0dObject.ValidDimensions;
  Truth_PrimaryTrajIDs = p0dObject.Truth_PrimaryTrajIDs;
  Truth_TrajIDs        = p0dObject.Truth_TrajIDs;
  Truth_HitCount       = p0dObject.Truth_HitCount;
  Truth_ChargeShare    = p0dObject.Truth_ChargeShare;
  anaUtils::CopyArray      (p0dObject.Position,    Position,    4);
  anaUtils::CopyArray      (p0dObject.PosVariance, PosVariance, 4);

}

//********************************************************************
void AnaP0DReconObject::Print() const {
//********************************************************************

  std::cout<<"-- AnaP0DReconObject.Print() --\n"
           <<"Vertices   .size() : "<<Vertices .size()<<"\n"
           <<"Particles  .size() : "<<Particles.size()<<"\n"
           <<"Tracks     .size() : "<<Tracks   .size()<<"\n"
           <<"Showers    .size() : "<<Showers  .size()<<"\n"
           <<"Clusters   .size() : "<<Clusters .size()<<"\n"
           <<"Nodes      .size() : "<<Nodes    .size()<<"\n"
           <<"Hits       .size() : "<<Hits     .size()<<"\n"
           <<"AlgorithmName      : "<<AlgorithmName   <<"\n"
           <<"Cycle              : "<<Cycle           <<"\n"
           <<"NHits              : "<<NHits           <<"\n"
           <<"UniqueID           : "<<UniqueID        <<"\n"
           <<"Bunch              : "<<Bunch           <<std::endl;

}

//********************************************************************
AnaP0DReconVertex::AnaP0DReconVertex(){
//********************************************************************
  Status               = kUnassigned;
  Quality              = kFloatUnassigned;
  NDOF                 = kUnassigned;
  Fiducial             = kFloatUnassigned;

}

//********************************************************************
AnaP0DReconVertex::AnaP0DReconVertex(const AnaP0DReconVertex& p0dVertex):AnaP0DReconObject(p0dVertex){
//********************************************************************

  Status               = p0dVertex.Status;
  Quality              = p0dVertex.Quality;
  NDOF                 = p0dVertex.NDOF;
  Fiducial             = p0dVertex.Fiducial;

}

//********************************************************************
AnaP0DReconParticle::AnaP0DReconParticle(){
//********************************************************************

  Status               = kUnassigned;
  Quality              = kFloatUnassigned;
  NDOF                 = kUnassigned;
  SideDeposit          = kFloatUnassigned;
  EndDeposit           = kFloatUnassigned;
  anaUtils::VectorToArray (TVector3(0,0,0),Direction  );
  anaUtils::VectorToArray (TVector3(0,0,0),DirVariance);
  Momentum             = kFloatUnassigned;
  Charge               = kFloatUnassigned;
  Length               = kFloatUnassigned;
  EDeposit             = kFloatUnassigned;
  realPIDNames         .clear();
  realPIDValues        .clear();
  integerPIDNames      .clear();
  integerPIDValues     .clear();
  PID                  .clear();
  PID_weight           .clear();


  TrueParticle = NULL;
}

//********************************************************************
AnaP0DReconParticle::AnaP0DReconParticle(const AnaP0DReconParticle& p0dParticle):AnaP0DReconObject(p0dParticle){
//********************************************************************

  Status               = p0dParticle.Status;
  Quality              = p0dParticle.Quality;
  NDOF                 = p0dParticle.NDOF;
  SideDeposit          = p0dParticle.SideDeposit;
  EndDeposit           = p0dParticle.EndDeposit;
  anaUtils::CopyArray      (p0dParticle.Direction,   Direction,   3);
  anaUtils::CopyArray      (p0dParticle.DirVariance, DirVariance, 3);
  Momentum             = p0dParticle.Momentum;
  Charge               = p0dParticle.Charge;
  Length               = p0dParticle.Length;
  EDeposit             = p0dParticle.EDeposit;
  realPIDNames         = p0dParticle.realPIDNames;
  realPIDValues        = p0dParticle.realPIDValues;
  integerPIDNames      = p0dParticle.integerPIDNames;
  integerPIDValues     = p0dParticle.integerPIDValues;
  PID                  = p0dParticle.PID;
  PID_weight           = p0dParticle.PID_weight;

  TrueParticle         = p0dParticle.TrueParticle;
}

//********************************************************************
AnaP0DReconCluster::AnaP0DReconCluster(){
//********************************************************************

  NFiducialHits        = kShortUnassigned;
  EDeposit             = kFloatUnassigned;

}

//********************************************************************
AnaP0DReconCluster::AnaP0DReconCluster(const AnaP0DReconCluster& p0dCluster):AnaP0DReconObject(p0dCluster){
//********************************************************************

  NFiducialHits        = p0dCluster.NFiducialHits;
  EDeposit             = p0dCluster.EDeposit;

}

//********************************************************************
AnaLocalReconBunch::AnaLocalReconBunch(){
//********************************************************************

  TECALReconObjects    .clear();
  TECALUnmatchedObjects.clear();
  PECALReconObjects    .clear();
  PECALUnmatchedObjects.clear();
  FGDOnlyReconObjects  .clear();
  P0DReconVertices     .clear();
  P0DReconParticles    .clear();
  P0DReconClusters     .clear();
}

//********************************************************************
AnaLocalReconBunch::~AnaLocalReconBunch(){
//********************************************************************
  for (std::vector<AnaTECALReconObject*>::iterator it = TECALReconObjects.begin();
       it != TECALReconObjects.end(); ++it) {
    delete (*it);
  }
  TECALReconObjects.clear();

  for (std::vector<AnaTECALUnmatchedObject*>::iterator it = TECALUnmatchedObjects.begin();
       it != TECALUnmatchedObjects.end(); ++it) {
    delete (*it);
  }
  TECALUnmatchedObjects.clear();

  for (std::vector<AnaPECALReconObject*>::iterator it = PECALReconObjects.begin();
       it != PECALReconObjects.end(); ++it) {
    delete (*it);
  }
  PECALReconObjects.clear();

  for (std::vector<AnaPECALUnmatchedObject*>::iterator it = PECALUnmatchedObjects.begin();
       it != PECALUnmatchedObjects.end(); ++it) {
    delete (*it);
  }
  PECALUnmatchedObjects.clear();

  for (std::vector<AnaP0DReconVertex*>::iterator it = P0DReconVertices.begin();
       it != P0DReconVertices.end(); ++it) {
    delete (*it);
  }
  P0DReconVertices.clear();

  for (std::vector<AnaP0DReconParticle*>::iterator it = P0DReconParticles.begin();
       it != P0DReconParticles.end(); ++it) {
    delete (*it);
  }
  P0DReconParticles.clear();

  for (std::vector<AnaP0DReconCluster*>::iterator it = P0DReconClusters.begin();
       it != P0DReconClusters.end(); ++it) {
    delete (*it);
  }
  P0DReconClusters.clear();

  for (std::vector<AnaFGDOnlyReconObject*>::iterator it = FGDOnlyReconObjects.begin();
       it != FGDOnlyReconObjects.end(); ++it) {
    delete (*it);
  }
  FGDOnlyReconObjects.clear();
}

//********************************************************************
AnaLocalReconBunch::AnaLocalReconBunch(const AnaLocalReconBunch& bunch):AnaBunch(bunch){
//********************************************************************
  TECALReconObjects.clear();
  for (std::vector<AnaTECALReconObject*>::const_iterator it = bunch.TECALReconObjects.begin();
       it != bunch.TECALReconObjects.end(); ++it) {
    TECALReconObjects.push_back((*it)->Clone());
  }

  TECALUnmatchedObjects.clear();
  for (std::vector<AnaTECALUnmatchedObject*>::const_iterator it = bunch.TECALUnmatchedObjects.begin();
       it != bunch.TECALUnmatchedObjects.end(); ++it) {
    TECALUnmatchedObjects.push_back((*it)->Clone());
  }

  PECALReconObjects.clear();
  for (std::vector<AnaPECALReconObject*>::const_iterator it = bunch.PECALReconObjects.begin();
       it != bunch.PECALReconObjects.end(); ++it) {
    PECALReconObjects.push_back((*it)->Clone());
  }

  PECALUnmatchedObjects.clear();
  for (std::vector<AnaPECALUnmatchedObject*>::const_iterator it = bunch.PECALUnmatchedObjects.begin();
       it != bunch.PECALUnmatchedObjects.end(); ++it) {
    PECALUnmatchedObjects.push_back((*it)->Clone());
  }

  FGDOnlyReconObjects.clear();
  for (std::vector<AnaFGDOnlyReconObject*>::const_iterator it = bunch.FGDOnlyReconObjects.begin();
       it != bunch.FGDOnlyReconObjects.end(); ++it) {
    FGDOnlyReconObjects.push_back((*it)->Clone());
  }

  P0DReconVertices.clear();
  for (std::vector<AnaP0DReconVertex*>::const_iterator it = bunch.P0DReconVertices.begin();
       it != bunch.P0DReconVertices.end(); ++it) {
    P0DReconVertices.push_back((*it)->Clone());
  }

  P0DReconParticles.clear();
  for (std::vector<AnaP0DReconParticle*>::const_iterator it = bunch.P0DReconParticles.begin();
       it != bunch.P0DReconParticles.end(); ++it) {
    P0DReconParticles.push_back((*it)->Clone());
  }

  P0DReconClusters.clear();
  for (std::vector<AnaP0DReconCluster*>::const_iterator it = bunch.P0DReconClusters.begin();
       it != bunch.P0DReconClusters.end(); ++it) {
    P0DReconClusters.push_back((*it)->Clone());
  }
}

//********************************************************************
void AnaLocalReconBunch::Print() const{
//********************************************************************

  AnaBunch::Print();

}

//********************************************************************
bool AnaLocalReconBunch::IsEmpty() const{
//********************************************************************
  return (Particles.empty() &&
          Vertices.empty() &&
          TECALReconObjects.empty() &&
          TECALUnmatchedObjects.empty() &&
          PECALReconObjects.empty() &&
          PECALUnmatchedObjects.empty() &&
          P0DReconVertices.empty() &&
          P0DReconParticles.empty() &&
          P0DReconClusters.empty() &&
          FGDOnlyReconObjects.empty());
}
