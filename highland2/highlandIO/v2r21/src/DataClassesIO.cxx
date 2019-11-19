#include "DataClassesIO.hxx"
#include "Versioning.hxx"

Int_t prefix_index = 0;
Int_t counter_index = 0;

// TEMP, the correspondence between the psyche detector enumeration and old
// highland DetUsed one
int subdet_enum[NDETECTORS] = {2,  3,  4,  0,  1,  5, 6, 15, 16, 17,
                               18, 11, 12, 13, 14, 7, 8, 9,  10};

// define offsets
int offset = 1 + TreeVars::subUniqueID - TreeVars::subDetector;
unsigned int TreeVars::offsetTPC = 0;
unsigned int TreeVars::offsetFGD = offset;
unsigned int TreeVars::offsetECAL = offset * 2;
unsigned int TreeVars::offsetSMRD = offset * 3;
unsigned int TreeVars::offsetP0D = offset * 4;
unsigned int TreeVars::offsetTRACKER = offset * 5;

//********************************************************************
void AnaSubTrackIO::Define(OutputManager& man, const std::string& counter,
                           const Int_t size2, const std::string& prefix) {
  //********************************************************************
  man.AddMatrixVar(prefix_index + TreeVars::subDetector, prefix + "Detector",
                   "I", "", counter_index, counter, -NMAXPARTICLES, size2);
  man.AddMatrixVar(prefix_index + TreeVars::subNNodes, prefix + "NNodes", "I",
                   "", counter_index, counter, -NMAXPARTICLES, size2);
  man.AddMatrixVar(prefix_index + TreeVars::subNHits, prefix + "NHits", "I", "",
                   counter_index, counter, -NMAXPARTICLES, size2);
  man.AddMatrixVar(prefix_index + TreeVars::subUniqueID, prefix + "UniqueID",
                   "I", "", counter_index, counter, -NMAXPARTICLES, size2);

  man.AddMatrixVar(prefix_index + TreeVars::subLength, prefix + "Length", "F",
                   "", counter_index, counter, -NMAXPARTICLES, size2);

  man.Add3DMatrixVar(prefix_index + TreeVars::subDirectionStart,
                     prefix + "DirectionStart", "F", "", counter_index, counter,
                     -NMAXPARTICLES, size2, 3);
  man.Add3DMatrixVar(prefix_index + TreeVars::subPositionStart,
                     prefix + "PositionStart", "F", "", counter_index, counter,
                     -NMAXPARTICLES, size2, 4);
  man.Add3DMatrixVar(prefix_index + TreeVars::subPositionEnd,
                     prefix + "PositionEnd", "F", "", counter_index, counter,
                     -NMAXPARTICLES, size2, 4);
  man.Add3DMatrixVar(prefix_index + TreeVars::subDirectionEnd,
                     prefix + "DirectionEnd", "F", "", counter_index, counter,
                     -NMAXPARTICLES, size2, 3);
}

//********************************************************************
void AnaSubTrackIO::Write(OutputManager& man, const AnaParticleB& seg,
                          const std::string& prefix, Int_t indx1, Int_t indx2) {
  //********************************************************************

  (void)prefix;

  if (man.GetCounterValueForVar(prefix_index + TreeVars::subDetector) >=
      (int)NMAXPARTICLES) {
    std::cout << "AnaParticleB:: too many tracks. Maximum is " << NMAXPARTICLES
              << std::endl;
    return;
  }

  man.FillMatrixVar(prefix_index + TreeVars::subNNodes, seg.NNodes, indx1,
                    indx2);
  man.FillMatrixVar(prefix_index + TreeVars::subNHits, seg.NHits, indx1, indx2);
  man.FillMatrixVar(prefix_index + TreeVars::subUniqueID, seg.UniqueID, indx1,
                    indx2);

  man.Fill3DMatrixVarFromArray(prefix_index + TreeVars::subDirectionStart,
                               seg.DirectionStart, -1, indx2, 3);
  man.Fill3DMatrixVarFromArray(prefix_index + TreeVars::subPositionStart,
                               seg.PositionStart, -1, indx2, 4);
  man.Fill3DMatrixVarFromArray(prefix_index + TreeVars::subPositionEnd,
                               seg.PositionEnd, -1, indx2, 4);
  man.Fill3DMatrixVarFromArray(prefix_index + TreeVars::subDirectionEnd,
                               seg.DirectionEnd, -1, indx2, 3);
}

//********************************************************************
void AnaTPCParticleIO::Define(OutputManager& man, const std::string& counter,
                              const std::string& prefix) const {
  //********************************************************************

  // AnaTpcTrackBIO
  prefix_index = TreeVars::offsetTPC;
  AnaSubTrackIO::Define(man, counter, NMAXTPCS, prefix);
  prefix_index = 0;

  man.AddMatrixVar(prefix_index + TreeVars::tpcCharge, prefix + "Charge", "F",
                   "", counter_index, counter, -NMAXPARTICLES, NMAXTPCS);
  man.AddMatrixVar(prefix_index + TreeVars::tpcMomentum, prefix + "Momentum",
                   "F", "", counter_index, counter, -NMAXPARTICLES, NMAXTPCS);
  man.AddMatrixVar(prefix_index + TreeVars::tpcMomentumError,
                   prefix + "MomentumError", "F", "", counter_index, counter,
                   -NMAXPARTICLES, NMAXTPCS);

#if VERSION_HAS_BFIELD_REFIT
  man.AddMatrixVar(prefix_index + TreeVars::tpcRefitMomentum,
                   prefix + "RefitMomentum", "F", "", counter_index, counter,
                   -NMAXPARTICLES, NMAXTPCS);
#endif
#if VERSION_HAS_EFIELD_REFIT
  man.AddMatrixVar(prefix_index + TreeVars::tpcEFieldRefitMomentum,
                   prefix + "EFieldRefitMomentum", "F", "", counter_index,
                   counter, -NMAXPARTICLES, NMAXTPCS);
#endif

  man.AddMatrixVar(prefix_index + TreeVars::tpcdEdxMeas, prefix + "dEdxMeas",
                   "F", "", counter_index, counter, -NMAXPARTICLES, NMAXTPCS);

  man.AddMatrixVar(prefix_index + TreeVars::tpcdEdxExpMu, prefix + "dEdxExpMu",
                   "F", "", counter_index, counter, -NMAXPARTICLES, NMAXTPCS);
  man.AddMatrixVar(prefix_index + TreeVars::tpcdEdxExpEle,
                   prefix + "dEdxExpEle", "F", "", counter_index, counter,
                   -NMAXPARTICLES, NMAXTPCS);
  man.AddMatrixVar(prefix_index + TreeVars::tpcdEdxExpP, prefix + "dEdxExpP",
                   "F", "", counter_index, counter, -NMAXPARTICLES, NMAXTPCS);
  man.AddMatrixVar(prefix_index + TreeVars::tpcdEdxExpPi, prefix + "dEdxExpPi",
                   "F", "", counter_index, counter, -NMAXPARTICLES, NMAXTPCS);

  man.AddMatrixVar(prefix_index + TreeVars::tpcRawdEdxMeas,
                   prefix + "RawdEdxMeas", "F", "", counter_index, counter,
                   -NMAXPARTICLES, NMAXTPCS);
  man.AddMatrixVar(prefix_index + TreeVars::tpcRawdEdxExpMu,
                   prefix + "RawdEdxExpMu", "F", "", counter_index, counter,
                   -NMAXPARTICLES, NMAXTPCS);
  man.AddMatrixVar(prefix_index + TreeVars::tpcRawdEdxExpEle,
                   prefix + "RawdEdxExpEle", "F", "", counter_index, counter,
                   -NMAXPARTICLES, NMAXTPCS);
  man.AddMatrixVar(prefix_index + TreeVars::tpcRawdEdxExpP,
                   prefix + "RawdEdxExpP", "F", "", counter_index, counter,
                   -NMAXPARTICLES, NMAXTPCS);
  man.AddMatrixVar(prefix_index + TreeVars::tpcRawdEdxExpPi,
                   prefix + "RawdEdxExpPi", "F", "", counter_index, counter,
                   -NMAXPARTICLES, NMAXTPCS);

  man.AddMatrixVar(prefix_index + TreeVars::tpcdEdxSigmaMu,
                   prefix + "dEdxSigmaMu", "F", "", counter_index, counter,
                   -NMAXPARTICLES, NMAXTPCS);
  man.AddMatrixVar(prefix_index + TreeVars::tpcdEdxSigmaEle,
                   prefix + "dEdxSigmaEle", "F", "", counter_index, counter,
                   -NMAXPARTICLES, NMAXTPCS);
  man.AddMatrixVar(prefix_index + TreeVars::tpcdEdxSigmaP,
                   prefix + "dEdxSigmaP", "F", "", counter_index, counter,
                   -NMAXPARTICLES, NMAXTPCS);
  man.AddMatrixVar(prefix_index + TreeVars::tpcdEdxSigmaPi,
                   prefix + "dEdxSigmaPi", "F", "", counter_index, counter,
                   -NMAXPARTICLES, NMAXTPCS);

  man.AddMatrixVar(prefix_index + TreeVars::tpcTrueTrackID,
                   prefix + "TrueTrackID", "I", "", counter_index, counter,
                   -NMAXPARTICLES, NMAXTPCS);

  // AnaTpcTrack
  man.AddMatrixVar(prefix_index + TreeVars::tpcBackMomentum,
                   prefix + "BackMomentum", "F", "", counter_index, counter,
                   -NMAXPARTICLES, NMAXTPCS);

#if VERSION_HAS_BFIELD_REFIT_FULL
  man.AddMatrixVar(prefix_index + TreeVars::tpcRefitCharge,
                   prefix + "RefitCharge", "F", "", counter_index, counter,
                   -NMAXPARTICLES, NMAXTPCS);
  man.Add3DMatrixVar(prefix_index + TreeVars::tpcRefitPosition,
                     prefix + "RefitPosition", "F", "", counter_index, counter,
                     -NMAXPARTICLES, NMAXTPCS, 4);
  man.Add3DMatrixVar(prefix_index + TreeVars::tpcRefitDirection,
                     prefix + "RefitDirection", "F", "", counter_index, counter,
                     -NMAXPARTICLES, NMAXTPCS, 3);
#endif

  man.AddMatrixVar(prefix_index + TreeVars::tpcPullmu, prefix + "Pullmu", "F",
                   "", counter_index, counter, -NMAXPARTICLES, NMAXTPCS);
  man.AddMatrixVar(prefix_index + TreeVars::tpcPullele, prefix + "Pullele", "F",
                   "", counter_index, counter, -NMAXPARTICLES, NMAXTPCS);
  man.AddMatrixVar(prefix_index + TreeVars::tpcPullp, prefix + "Pullp", "F", "",
                   counter_index, counter, -NMAXPARTICLES, NMAXTPCS);
  man.AddMatrixVar(prefix_index + TreeVars::tpcPullpi, prefix + "Pullpi", "F",
                   "", counter_index, counter, -NMAXPARTICLES, NMAXTPCS);
  man.AddMatrixVar(prefix_index + TreeVars::tpcPullk, prefix + "Pullk", "F", "",
                   counter_index, counter, -NMAXPARTICLES, NMAXTPCS);

  man.AddMatrixVar(prefix_index + TreeVars::tpcdEdxExpK, prefix + "dEdxExpK",
                   "F", "", counter_index, counter, -NMAXPARTICLES, NMAXTPCS);

  man.AddMatrixVar(prefix_index + TreeVars::tpcRawdEdxExpK,
                   prefix + "RawdEdxExpK", "F", "", counter_index, counter,
                   -NMAXPARTICLES, NMAXTPCS);

  man.AddMatrixVar(prefix_index + TreeVars::tpcdEdxSigmaK,
                   prefix + "dEdxSigmaK", "F", "", counter_index, counter,
                   -NMAXPARTICLES, NMAXTPCS);

  man.AddMatrixVar(prefix_index + TreeVars::tpcPurity, prefix + "Purity", "F",
                   "", counter_index, counter, -NMAXPARTICLES, NMAXTPCS);
}

//********************************************************************
void AnaTPCParticleIO::Write(OutputManager& man, const std::string& prefix,
                             Int_t indx1, Int_t indx2) const {
  //********************************************************************

  (void)prefix;

  if (man.GetCounterValueForVar(TreeVars::offsetTPC + TreeVars::subDetector) >=
      (int)NMAXPARTICLES) {
    std::cout << "AnaTPCParticleB:: too many tracks. Maximum is "
              << NMAXPARTICLES << std::endl;
    return;
  }
  // to avoid warning
  (void)indx2;

  // AnaTpcTrackB

  // this is to keep highland like structure where the local detector
  // enumeration was
  // aslo used as an index in the output tree
  Int_t index_old = convUtils::GetLocalDetEnum(
      SubDetId::kTPC, SubDetId::GetSubdetectorEnum(Detector));
  man.FillMatrixVar(prefix_index + TreeVars::tpcCharge, Charge, indx1,
                    index_old);
  man.FillMatrixVar(prefix_index + TreeVars::tpcMomentum, Momentum, indx1,
                    index_old);
  man.FillMatrixVar(prefix_index + TreeVars::tpcMomentumError, MomentumError,
                    indx1, index_old);

#if VERSION_HAS_BFIELD_REFIT
  man.FillMatrixVar(prefix_index + TreeVars::tpcRefitMomentum, RefitMomentum,
                    indx1, index_old);
#endif
#if VERSION_HAS_EFIELD_REFIT
  man.FillMatrixVar(prefix_index + TreeVars::tpcEFieldRefitMomentum,
                    EFieldRefitMomentum, indx1, index_old);
#endif

  man.FillMatrixVar(prefix_index + TreeVars::tpcdEdxMeas, dEdxMeas, indx1,
                    index_old);
  man.FillMatrixVar(prefix_index + TreeVars::tpcdEdxExpMu, dEdxexpMuon, indx1,
                    index_old);
  man.FillMatrixVar(prefix_index + TreeVars::tpcdEdxExpEle, dEdxexpEle, indx1,
                    index_old);
  man.FillMatrixVar(prefix_index + TreeVars::tpcdEdxExpP, dEdxexpProton, indx1,
                    index_old);
  man.FillMatrixVar(prefix_index + TreeVars::tpcdEdxExpPi, dEdxexpPion, indx1,
                    index_old);

  if (Original) {
    const AnaTPCParticleB* original =
        static_cast<const AnaTPCParticleB*>(Original);
    man.FillMatrixVar(prefix_index + TreeVars::tpcRawdEdxMeas,
                      original->dEdxMeas, indx1, index_old);
    man.FillMatrixVar(prefix_index + TreeVars::tpcRawdEdxExpMu,
                      original->dEdxexpMuon, indx1, index_old);
    man.FillMatrixVar(prefix_index + TreeVars::tpcRawdEdxExpEle,
                      original->dEdxexpEle, indx1, index_old);
    man.FillMatrixVar(prefix_index + TreeVars::tpcRawdEdxExpP,
                      original->dEdxexpProton, indx1, index_old);
    man.FillMatrixVar(prefix_index + TreeVars::tpcRawdEdxExpPi,
                      original->dEdxexpPion, indx1, index_old);
  }

  man.FillMatrixVar(prefix_index + TreeVars::tpcdEdxSigmaMu, dEdxSigmaMuon,
                    indx1, index_old);
  man.FillMatrixVar(prefix_index + TreeVars::tpcdEdxSigmaEle, dEdxSigmaEle,
                    indx1, index_old);
  man.FillMatrixVar(prefix_index + TreeVars::tpcdEdxSigmaP, dEdxSigmaProton,
                    indx1, index_old);
  man.FillMatrixVar(prefix_index + TreeVars::tpcdEdxSigmaPi, dEdxSigmaPion,
                    indx1, index_old);

  if (TrueObject)
    man.FillMatrixVar(prefix_index + TreeVars::tpcTrueTrackID, TrueObject->ID,
                      indx1, index_old);

  // AnaTpcTrack
  man.FillMatrixVar(prefix_index + TreeVars::tpcBackMomentum, MomentumEnd,
                    indx1, index_old);

#if VERSION_HAS_BFIELD_REFIT_FULL
  man.FillMatrixVar(prefix_index + TreeVars::tpcRefitCharge, RefitCharge, indx1,
                    index_old);
  man.Fill3DMatrixVarFromArray(prefix_index + TreeVars::tpcRefitPosition,
                               RefitPosition, indx1, index_old, 4);
  man.Fill3DMatrixVarFromArray(prefix_index + TreeVars::tpcRefitDirection,
                               RefitDirection, indx1, index_old, 3);
#endif

  man.FillMatrixVar(prefix_index + TreeVars::tpcPullmu, Pullmu, indx1,
                    index_old);
  man.FillMatrixVar(prefix_index + TreeVars::tpcPullele, Pullele, indx1,
                    index_old);
  man.FillMatrixVar(prefix_index + TreeVars::tpcPullp, Pullp, indx1, index_old);
  man.FillMatrixVar(prefix_index + TreeVars::tpcPullpi, Pullpi, indx1,
                    index_old);
  man.FillMatrixVar(prefix_index + TreeVars::tpcPullk, Pullk, indx1, index_old);

  man.FillMatrixVar(prefix_index + TreeVars::tpcdEdxExpK, dEdxexpKaon, indx1,
                    index_old);
  man.FillMatrixVar(prefix_index + TreeVars::tpcdEdxSigmaK, dEdxSigmaKaon,
                    indx1, index_old);

  if (Original) {
    const AnaTPCParticle* original =
        static_cast<const AnaTPCParticle*>(Original);
    man.FillMatrixVar(prefix_index + TreeVars::tpcRawdEdxExpK,
                      original->dEdxexpKaon, indx1, index_old);
  }

  man.FillMatrixVar(prefix_index + TreeVars::tpcPurity, Purity, indx1,
                    index_old);

  prefix_index = TreeVars::offsetTPC;

  AnaSubTrackIO::Write(man, *this, prefix, indx1, index_old);

  man.FillMatrixVar(prefix_index + TreeVars::subDetector, index_old + 1, indx1,
                    index_old);

  man.FillMatrixVar(prefix_index + TreeVars::subLength, Length, indx1,
                    index_old);

  prefix_index = 0;
}

//********************************************************************
void AnaFGDParticleIO::Define(OutputManager& man, const std::string& counter,
                              const std::string& prefix) const {
  //********************************************************************
  prefix_index = TreeVars::offsetFGD;
  AnaSubTrackIO::Define(man, counter, NMAXFGDS, prefix);
  prefix_index = 0;

  // AnaFgdTrackB
  man.AddMatrixVar(prefix_index + TreeVars::fgdX, prefix + "X", "F", "",
                   counter_index, counter, -NMAXPARTICLES, NMAXFGDS);
  man.AddMatrixVar(prefix_index + TreeVars::fgdPullmu, prefix + "Pullmu", "F",
                   "", counter_index, counter, -NMAXPARTICLES, NMAXFGDS);
  man.AddMatrixVar(prefix_index + TreeVars::fgdPullp, prefix + "Pullp", "F", "",
                   counter_index, counter, -NMAXPARTICLES, NMAXFGDS);
  man.AddMatrixVar(prefix_index + TreeVars::fgdPullpi, prefix + "Pullpi", "F",
                   "", counter_index, counter, -NMAXPARTICLES, NMAXFGDS);
  man.AddMatrixVar(prefix_index + TreeVars::fgdPullno, prefix + "Pullno", "F",
                   "", counter_index, counter, -NMAXPARTICLES, NMAXFGDS);
  man.AddMatrixVar(prefix_index + TreeVars::fgdContainment,
                   prefix + "Containment", "I", "", counter_index, counter,
                   -NMAXPARTICLES, NMAXFGDS);

  // AnaFgdTrack

  man.AddMatrixVar(prefix_index + TreeVars::fgdE, prefix + "E", "F", "",
                   counter_index, counter, -NMAXPARTICLES, NMAXFGDS);
  man.AddMatrixVar(prefix_index + TreeVars::fgdAvgTime, prefix + "AvgTime", "F",
                   "", counter_index, counter, -NMAXPARTICLES, NMAXFGDS);

#if VERSION_HAS_FGD_VERTEX_AND_END_ACTIVITY
  man.AddMatrixVar(prefix_index + TreeVars::fgdHasFgdVA, prefix + "HasFgdVA",
                   "I", "", counter_index, counter, -NMAXPARTICLES, NMAXFGDS);
  man.AddMatrixVar(prefix_index + TreeVars::fgdHasFgdVA_fmode,
                   prefix + "HasFgdVA_fmode", "I", "", counter_index, counter,
                   -NMAXPARTICLES, NMAXFGDS);
  man.AddMatrixVar(prefix_index + TreeVars::fgdTypeVA, prefix + "TypeVA", "I",
                   "", counter_index, counter, -NMAXPARTICLES, NMAXFGDS);
#endif

  man.AddMatrixVar(prefix_index + TreeVars::fgdVertex1x1, prefix + "Vertex1x1",
                   "F", "", counter_index, counter, -NMAXPARTICLES, NMAXFGDS);
  man.AddMatrixVar(prefix_index + TreeVars::fgdVertex3x3, prefix + "Vertex3x3",
                   "F", "", counter_index, counter, -NMAXPARTICLES, NMAXFGDS);
  man.AddMatrixVar(prefix_index + TreeVars::fgdVertex5x5, prefix + "Vertex5x5",
                   "F", "", counter_index, counter, -NMAXPARTICLES, NMAXFGDS);
  man.AddMatrixVar(prefix_index + TreeVars::fgdVertex7x7, prefix + "Vertex7x7",
                   "F", "", counter_index, counter, -NMAXPARTICLES, NMAXFGDS);
  man.AddMatrixVar(prefix_index + TreeVars::fgdVertexLayer,
                   prefix + "VertexLayer", "F", "", counter_index, counter,
                   -NMAXPARTICLES, NMAXFGDS);

#if VERSION_HAS_FGD_VERTEX_AND_END_ACTIVITY
  man.AddMatrixVar(prefix_index + TreeVars::fgdHasFgdEA, prefix + "HasFgdEA",
                   "I", "", counter_index, counter, -NMAXPARTICLES, NMAXFGDS);
  man.AddMatrixVar(prefix_index + TreeVars::fgdHasFgdEA_fmode,
                   prefix + "HasFgdEA_fmode", "I", "", counter_index, counter,
                   -NMAXPARTICLES, NMAXFGDS);
  man.AddMatrixVar(prefix_index + TreeVars::fgdTypeEA, prefix + "TypeEA", "I",
                   "", counter_index, counter, -NMAXPARTICLES, NMAXFGDS);

  man.AddMatrixVar(prefix_index + TreeVars::fgdEnd0x0, prefix + "End0x0", "F",
                   "", counter_index, counter, -NMAXPARTICLES, NMAXFGDS);
  man.AddMatrixVar(prefix_index + TreeVars::fgdEnd1x1, prefix + "End1x1", "F",
                   "", counter_index, counter, -NMAXPARTICLES, NMAXFGDS);
  man.AddMatrixVar(prefix_index + TreeVars::fgdEnd3x3, prefix + "End3x3", "F",
                   "", counter_index, counter, -NMAXPARTICLES, NMAXFGDS);
  man.AddMatrixVar(prefix_index + TreeVars::fgdEnd5x5, prefix + "End5x5", "F",
                   "", counter_index, counter, -NMAXPARTICLES, NMAXFGDS);
  man.AddMatrixVar(prefix_index + TreeVars::fgdEnd7x7, prefix + "End7x7", "F",
                   "", counter_index, counter, -NMAXPARTICLES, NMAXFGDS);
#endif
}

//********************************************************************
void AnaFGDParticleIO::Write(OutputManager& man, const std::string& prefix,
                             Int_t indx1, Int_t indx2) const {
  //********************************************************************

  (void)prefix;

  if (man.GetCounterValueForVar(TreeVars::offsetFGD + TreeVars::subDetector) >=
      (int)NMAXPARTICLES) {
    std::cout << "AnaFGDParticleB:: too many tracks. Maximum is "
              << NMAXPARTICLES << std::endl;
    return;
  }

  // to avoid warning
  (void)indx2;

  // AnaFgdTracB
  // this is to keep highland like structure where the local detector
  // enumeration was
  // aslo used as an index in the output tree
  Int_t index_old = convUtils::GetLocalDetEnum(
      SubDetId::kFGD, SubDetId::GetSubdetectorEnum(Detector));
  man.FillMatrixVar(prefix_index + TreeVars::fgdX, X, indx1, index_old);
  man.FillMatrixVar(prefix_index + TreeVars::fgdPullmu, Pullmu, indx1,
                    index_old);
  man.FillMatrixVar(prefix_index + TreeVars::fgdPullp, Pullp, indx1, index_old);
  man.FillMatrixVar(prefix_index + TreeVars::fgdPullpi, Pullpi, indx1,
                    index_old);
  man.FillMatrixVar(prefix_index + TreeVars::fgdPullno, Pullno, indx1,
                    index_old);
  man.FillMatrixVar(prefix_index + TreeVars::fgdContainment, Containment, indx1,
                    index_old);

  // AnaFgdTrack
  man.FillMatrixVar(prefix_index + TreeVars::fgdE, E, indx1, index_old);
  man.FillMatrixVar(prefix_index + TreeVars::fgdAvgTime, AvgTime, indx1,
                    index_old);

#if VERSION_HAS_FGD_VERTEX_AND_END_ACTIVITY
  man.FillMatrixVar(prefix_index + TreeVars::fgdHasFgdVA, HasFgdVA, indx1,
                    index_old);
  man.FillMatrixVar(prefix_index + TreeVars::fgdHasFgdVA_fmode, HasFgdVA_fmode,
                    indx1, index_old);
  man.FillMatrixVar(prefix_index + TreeVars::fgdTypeVA, TypeVA, indx1,
                    index_old);
#endif

  man.FillMatrixVar(prefix_index + TreeVars::fgdVertex1x1, Vertex1x1, indx1,
                    index_old);
  man.FillMatrixVar(prefix_index + TreeVars::fgdVertex3x3, Vertex3x3, indx1,
                    index_old);
  man.FillMatrixVar(prefix_index + TreeVars::fgdVertex5x5, Vertex5x5, indx1,
                    index_old);
  man.FillMatrixVar(prefix_index + TreeVars::fgdVertex7x7, Vertex7x7, indx1,
                    index_old);
  man.FillMatrixVar(prefix_index + TreeVars::fgdVertexLayer, VertexLayer, indx1,
                    index_old);

#if VERSION_HAS_FGD_VERTEX_AND_END_ACTIVITY
  man.FillMatrixVar(prefix_index + TreeVars::fgdHasFgdEA, HasFgdEA, indx1,
                    index_old);
  man.FillMatrixVar(prefix_index + TreeVars::fgdHasFgdEA_fmode, HasFgdEA_fmode,
                    indx1, index_old);
  man.FillMatrixVar(prefix_index + TreeVars::fgdTypeEA, TypeEA, indx1,
                    index_old);

  man.FillMatrixVar(prefix_index + TreeVars::fgdEnd0x0, End0x0, indx1,
                    index_old);
  man.FillMatrixVar(prefix_index + TreeVars::fgdEnd1x1, End1x1, indx1,
                    index_old);
  man.FillMatrixVar(prefix_index + TreeVars::fgdEnd3x3, End3x3, indx1,
                    index_old);
  man.FillMatrixVar(prefix_index + TreeVars::fgdEnd5x5, End5x5, indx1,
                    index_old);
  man.FillMatrixVar(prefix_index + TreeVars::fgdEnd7x7, End7x7, indx1,
                    index_old);
#endif

  prefix_index = TreeVars::offsetFGD;
  AnaSubTrackIO::Write(man, *this, prefix, indx1, index_old);

  man.FillMatrixVar(prefix_index + TreeVars::subDetector, index_old + 1, indx1,
                    index_old);
  man.FillMatrixVar(prefix_index + TreeVars::subLength, Length, indx1,
                    index_old);

  prefix_index = 0;
}

//********************************************************************
void AnaECALParticleIO::Define(OutputManager& man, const std::string& counter,
                               const std::string& prefix) const {
  //********************************************************************
  // NMAXECALS corresponds to the most possbile number of ecal objects in the
  // tracks
  // Here for the moment (highland-like) we store all possible ECal segments
  // (sub-detector based),  so use hard-coded number
  prefix_index = TreeVars::offsetECAL;
  AnaSubTrackIO::Define(man, counter, NECALSUBDETS, prefix);
  prefix_index = 0;

  // AnaEcalTrackB
  man.AddMatrixVar(prefix_index + TreeVars::ecalTrShVal, prefix + "TrShVal",
                   "F", "", counter_index, counter, -NMAXPARTICLES,
                   NECALSUBDETS);
  man.AddMatrixVar(prefix_index + TreeVars::ecalPIDMipEm, prefix + "PIDMipEm",
                   "F", "", counter_index, counter, -NMAXPARTICLES,
                   NECALSUBDETS);
  man.AddMatrixVar(prefix_index + TreeVars::ecalPIDEmHip, prefix + "PIDEmHip",
                   "F", "", counter_index, counter, -NMAXPARTICLES,
                   NECALSUBDETS);
  man.AddMatrixVar(prefix_index + TreeVars::ecalEMEnergy, prefix + "EMEnergy",
                   "F", "", counter_index, counter, -NMAXPARTICLES,
                   NECALSUBDETS);
  man.AddMatrixVar(prefix_index + TreeVars::ecalEDeposit, prefix + "EDeposit",
                   "F", "", counter_index, counter, -NMAXPARTICLES,
                   NECALSUBDETS);
  man.AddMatrixVar(prefix_index + TreeVars::ecalIsShowerLike,
                   prefix + "IsShowerLike", "I", "", counter_index, counter,
                   -NMAXPARTICLES, NECALSUBDETS);
  man.AddMatrixVar(prefix_index + TreeVars::ecalAvgTime, prefix + "AvgTime",
                   "F", "", counter_index, counter, -NMAXPARTICLES,
                   NECALSUBDETS);
  man.AddMatrixVar(prefix_index + TreeVars::ecalMostUpStreamLayerHit,
                   prefix + "MostUpStreamLayerHit", "I", "", counter_index,
                   counter, -NMAXPARTICLES, NECALSUBDETS);
  man.Add3DMatrixVar(prefix_index + TreeVars::ecalShowerPosition,
                     prefix + "ShowerPosition", "F", "", counter_index, counter,
                     -NMAXPARTICLES, NECALSUBDETS, 3);

  // AnaEcalTrack
  man.AddMatrixVar(prefix_index + TreeVars::ecalPIDMipPion,
                   prefix + "PIDMipPion", "F", "", counter_index, counter,
                   -NMAXPARTICLES, NECALSUBDETS);
  man.AddMatrixVar(prefix_index + TreeVars::ecalContainment,
                   prefix + "Containment", "F", "", counter_index, counter,
                   -NMAXPARTICLES, NECALSUBDETS);
}

//********************************************************************
void AnaECALParticleIO::Write(OutputManager& man, const std::string& prefix,
                              Int_t indx1, Int_t indx2) const {
  //********************************************************************

  (void)prefix;

  if (man.GetCounterValueForVar(TreeVars::offsetECAL + TreeVars::subDetector) >=
      (int)NMAXPARTICLES) {
    std::cout << "AnECALParticleB:: too many tracks. Maximum is "
              << NMAXPARTICLES << std::endl;
    return;
  }

  // to avoid warning
  (void)indx2;

  // AnaEcalTrackB
  // this is to keep highland like structure where the local detector
  // enumeration was
  // aslo used as an index in the output tree
  Int_t index_old = convUtils::GetLocalDetEnum(
      SubDetId::kECAL, SubDetId::GetSubdetectorEnum(Detector));
  man.FillMatrixVar(prefix_index + TreeVars::ecalTrShVal, TrShVal, indx1,
                    index_old);
  man.FillMatrixVar(prefix_index + TreeVars::ecalPIDMipEm, PIDMipEm, indx1,
                    index_old);
  man.FillMatrixVar(prefix_index + TreeVars::ecalEMEnergy, EMEnergy, indx1,
                    index_old);
  man.FillMatrixVar(prefix_index + TreeVars::ecalEDeposit, EDeposit, indx1,
                    index_old);
  man.FillMatrixVar(prefix_index + TreeVars::ecalIsShowerLike, IsShowerLike,
                    indx1, index_old);
  man.FillMatrixVar(prefix_index + TreeVars::ecalAvgTime, AvgTime, indx1,
                    index_old);

  man.FillMatrixVar(prefix_index + TreeVars::ecalMostUpStreamLayerHit,
                    MostUpStreamLayerHit, indx1, index_old);

  man.Fill3DMatrixVarFromArray(prefix_index + TreeVars::ecalShowerPosition,
                               ShowerPosition, indx1, index_old, 3);

  // AnaEcalTrack
  man.FillMatrixVar(prefix_index + TreeVars::ecalPIDMipPion, PIDMipPion, indx1,
                    index_old);
  man.FillMatrixVar(prefix_index + TreeVars::ecalPIDEmHip, PIDEmHip, indx1,
                    index_old);
  man.FillMatrixVar(prefix_index + TreeVars::ecalContainment, Containment,
                    indx1, index_old);

  prefix_index = TreeVars::offsetECAL;
  AnaSubTrackIO::Write(man, *this, prefix, indx1, index_old);

  man.FillMatrixVar(prefix_index + TreeVars::subDetector, index_old + 1, indx1,
                    index_old);
  man.FillMatrixVar(prefix_index + TreeVars::subLength, Length, indx1,
                    index_old);

  prefix_index = 0;
}

//********************************************************************
void AnaSMRDParticleIO::Define(OutputManager& man, const std::string& counter,
                               const std::string& prefix) const {
  //********************************************************************
  // NMAXSMRDS corresponds to the most possbile number of smrd objects in the
  // tracks
  // Here for the moment (highland-like) we store all possible SMRD segments
  // (sub-detector based),  so use hard-coded number

  prefix_index = TreeVars::offsetSMRD;
  AnaSubTrackIO::Define(man, counter, NSMRDSUBDETS, prefix);
  prefix_index = 0;

  man.AddMatrixVar(prefix_index + TreeVars::smrdEDeposit, prefix + "EDeposit",
                   "F", "", counter_index, counter, -NMAXPARTICLES,
                   NSMRDSUBDETS);
  man.AddMatrixVar(prefix_index + TreeVars::smrdAvgTime, prefix + "AvgTime",
                   "F", "", counter_index, counter, -NMAXPARTICLES,
                   NSMRDSUBDETS);
}

//********************************************************************
void AnaSMRDParticleIO::Write(OutputManager& man, const std::string& prefix,
                              Int_t indx1, Int_t indx2) const {
  //********************************************************************

  (void)prefix;

  if (man.GetCounterValueForVar(TreeVars::offsetSMRD + TreeVars::subDetector) >=
      (int)NMAXPARTICLES) {
    std::cout << "AnaSMRDParticleB:: too many tracks. Maximum is "
              << NMAXPARTICLES << std::endl;
    return;
  }

  // to avoid warning
  (void)indx2;

  // this is to keep highland like structure where the local detector
  // enumeration was
  // aslo used as an index in the output tree
  Int_t index_old = convUtils::GetLocalDetEnum(
      SubDetId::kSMRD, SubDetId::GetSubdetectorEnum(Detector));
  man.FillMatrixVar(prefix_index + TreeVars::smrdAvgTime, AvgTime, indx1,
                    index_old);

  man.FillMatrixVar(prefix_index + TreeVars::smrdEDeposit, EDeposit, indx1,
                    index_old);

  prefix_index = TreeVars::offsetSMRD;
  AnaSubTrackIO::Write(man, *this, prefix, indx1, index_old);

  man.FillMatrixVar(prefix_index + TreeVars::subDetector, index_old + 1, indx1,
                    index_old);
  man.FillMatrixVar(prefix_index + TreeVars::subLength, Length, indx1,
                    index_old);

  prefix_index = 0;
}

//********************************************************************
void AnaP0DParticleIO::Define(OutputManager& man, const std::string& counter,
                              const std::string& prefix) const {
  //********************************************************************
  prefix_index = TreeVars::offsetP0D;
  AnaSubTrackIO::Define(man, counter, 1, prefix);
  prefix_index = 0;

#if VERSION_HAS_P0D_AVERAGED_TIME
  man.AddMatrixVar(prefix_index + TreeVars::p0dAvgTime, prefix + "AvgTime", "F",
                   "", counter_index, counter, -NMAXPARTICLES, NMAXP0DS);
#endif
}

//********************************************************************
void AnaP0DParticleIO::Write(OutputManager& man, const std::string& prefix,
                             Int_t indx1, Int_t indx2) const {
  //********************************************************************

  (void)prefix;

  if (man.GetCounterValueForVar(TreeVars::offsetP0D + TreeVars::subDetector) >=
      (int)NMAXPARTICLES) {
    std::cout << "AnaP0DParticleB too many tracks. Maximum is " << NMAXPARTICLES
              << std::endl;
    return;
  }

  indx2 = 0;

#if VERSION_HAS_P0D_AVERAGED_TIME
  man.FillMatrixVar(prefix_index + TreeVars::p0dAvgTime, AvgTime, indx1, 0);
#endif

  prefix_index = TreeVars::offsetP0D;
  AnaSubTrackIO::Write(man, *this, prefix, indx1, 0);
  man.FillMatrixVar(prefix_index + TreeVars::subLength, Length, indx1, 0);
  prefix_index = 0;
}

//********************************************************************
void AnaTrackerTrackIO::Define(OutputManager& man, const std::string& counter,
                               const std::string& prefix) const {
  //********************************************************************
  prefix_index = TreeVars::offsetTRACKER;
  AnaSubTrackIO::Define(man, counter, NMAXTRACKERS, prefix);
  prefix_index = 0;

  man.AddMatrixVar(prefix_index + TreeVars::trackerCharge, prefix + "Charge",
                   "F", "", counter_index, counter, -NMAXPARTICLES,
                   NMAXTRACKERS);
  man.AddMatrixVar(prefix_index + TreeVars::trackerMomentum,
                   prefix + "Momentum", "F", "", counter_index, counter,
                   -NMAXPARTICLES, NMAXTRACKERS);
  man.AddMatrixVar(prefix_index + TreeVars::trackerMomentumEnd,
                   prefix + "MomentumEnd", "F", "", counter_index, counter,
                   -NMAXPARTICLES, NMAXTRACKERS);
}

//********************************************************************
void AnaTrackerTrackIO::Write(OutputManager& man, const std::string& prefix,
                              Int_t indx1, Int_t indx2) const {
  //********************************************************************

  (void)prefix;

  if (man.GetCounterValueForVar(TreeVars::offsetTRACKER +
                                TreeVars::subDetector) >= (int)NMAXPARTICLES) {
    std::cout << "AnaTrackerTrackB:: too many tracks. Maximum is "
              << NMAXPARTICLES << std::endl;
    return;
  }

  if (indx2 >= (int)NMAXTRACKERS) {
    std::cout << "too many tracker segments " << indx2 << ". Maximum is "
              << NMAXTRACKERS << std::endl;
    return;
  }

  man.FillMatrixVar(prefix_index + TreeVars::trackerCharge, Charge, indx1,
                    indx2);
  man.FillMatrixVar(prefix_index + TreeVars::trackerMomentum, Momentum, indx1,
                    indx2);
  man.FillMatrixVar(prefix_index + TreeVars::trackerMomentumEnd, MomentumEnd,
                    indx1, indx2);

  prefix_index = TreeVars::offsetTRACKER;
  AnaSubTrackIO::Write(man, *this, prefix, indx1, indx2);

  Int_t trackerDet;
  convUtils::ConvertBitFieldToTrackerDetField(Detector, trackerDet);
  man.FillMatrixVar(prefix_index + TreeVars::subDetector, trackerDet, indx1,
                    indx2);
  man.FillMatrixVar(prefix_index + TreeVars::subLength, Length, indx1, indx2);

  prefix_index = 0;
}

//********************************************************************
AnaTrackIO::AnaTrackIO(const AnaTrack& track) {
  //********************************************************************

  // Only get the information needed to fill the flat-trees

  // AnaRecObjectC
  Status = track.Status;
  Detector = track.Detector;
  UniqueID = track.UniqueID;

  // The associated true recObj
  TrueObject = track.TrueObject;

  // AnaTrackB
  TPCQualityCut = track.TPCQualityCut;
  ToF = track.ToF;

  nTPCSegments = track.nTPCSegments;
  nFGDSegments = track.nFGDSegments;
  nP0DSegments = track.nP0DSegments;
  nECALSegments = track.nECALSegments;
  nSMRDSegments = track.nSMRDSegments;

  for (Int_t i = 0; i < nTPCSegments; i++) {
    TPCSegments[i] = NULL;
  }
  for (Int_t i = 0; i < nFGDSegments; i++) {
    FGDSegments[i] = NULL;
  }
  for (Int_t i = 0; i < nECALSegments; i++) {
    ECALSegments[i] = NULL;
  }
  for (Int_t i = 0; i < nSMRDSegments; i++) {
    SMRDSegments[i] = NULL;
  }
  for (Int_t i = 0; i < nP0DSegments; i++) {
    P0DSegments[i] = NULL;
  }

  // AnaParticleB
  Index = track.Index;
  NHits = track.NHits;
  NNodes = track.NNodes;
  Charge = track.Charge;
  Momentum = track.Momentum;
  RangeMomentumMuon = track.RangeMomentumMuon;

  anaUtils::CopyArray(track.DirectionStart, DirectionStart, 3);
  anaUtils::CopyArray(track.DirectionEnd, DirectionEnd, 3);
  anaUtils::CopyArray(track.PositionStart, PositionStart, 4);
  anaUtils::CopyArray(track.PositionEnd, PositionEnd, 4);

  MomentumFlip = track.MomentumFlip;
  anaUtils::CopyArray(track.DirectionStartFlip, DirectionStartFlip, 3);

  // AnaParticleE
  MomentumError = track.MomentumError;

  NDOF = track.NDOF;
  Chi2 = track.Chi2;
  Bunch = track.Bunch;
  ReconPDG = track.ReconPDG;

  for (int i = 0; i < 2; i++) {
    UpstreamHits_Position[i] = track.UpstreamHits_Position[i];
    DownstreamHits_Position[i] = track.DownstreamHits_Position[i];
    UpstreamHits_Charge[i] = track.UpstreamHits_Charge[i];
    DownstreamHits_Charge[i] = track.DownstreamHits_Charge[i];
  }

  MomentumEle = track.MomentumEle;
  MomentumMuon = track.MomentumMuon;
  MomentumProton = track.MomentumProton;
  MomentumErrorEle = track.MomentumErrorEle;
  MomentumErrorMuon = track.MomentumErrorMuon;
  MomentumErrorProton = track.MomentumErrorProton;

#if VERSION_HAS_REVERSED_REFITS
  anaUtils::CopyArray(track.PositionStartFlip, PositionStartFlip, 4);
  anaUtils::CopyArray(track.DirectionEndFlip, DirectionEndFlip, 3);
#endif

#if VERSION_HAS_PRANGE_ESTIMATES
  RangeMomentumEle = track.RangeMomentumEle;
  RangeMomentumProton = track.RangeMomentumProton;
  RangeMomentumMuonFlip = track.RangeMomentumMuonFlip;
  RangeMomentumProtonFlip = track.RangeMomentumProtonFlip;
#endif

  // The most lower PrimaryIndex associated global vertex (if it exists).
  ReconVertex = NULL;  // track.ReconVertex;

  ReconVertices.clear();
  /*
     for (UInt_t i=0;i<track.ReconVertices.size();i++)
     ReconVertices.push_back(track.ReconVertices[i]);
     */
  anaUtils::CopyArray(track.DirectionAtVertex, DirectionAtVertex, 3);
  MomentumAtVertex = track.MomentumAtVertex;

#if VERSION_HAS_TIME_FITS
  TimeNodes.clear();
  for (UInt_t i = 0; i < track.TimeNodes.size(); i++)
    TimeNodes.push_back(new AnaTimeNodeIO(*track.TimeNodes[i]));
#endif

  // AnaTrack
  Length = track.Length;
  Detectors = track.Detectors;
}

//********************************************************************
void AnaTrackIO::Define(OutputManager& man, const std::string& counter,
                        const std::string& prefix) const {
  //********************************************************************

  // From AnaTrackBIO
  man.AddVectorVar(prefix_index + TreeVars::globalUniqueID, prefix + "UniqueID",
                   "I", "", counter_index, counter, -NMAXPARTICLES);
  man.AddMatrixVar(prefix_index + TreeVars::globalDetUsed, prefix + "DetUsed",
                   "I", "", counter_index, counter, -NMAXPARTICLES, NDETECTORS);
  man.AddVectorVar(prefix_index + TreeVars::globalNTPCs, prefix + "NTPCs", "I",
                   "", counter_index, counter, -NMAXPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::globalNFGDs, prefix + "NFGDs", "I",
                   "", counter_index, counter, -NMAXPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::globalNECALs, prefix + "NECALs",
                   "I", "", counter_index, counter, -NMAXPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::globalNSMRDs, prefix + "NSMRDs",
                   "I", "", counter_index, counter, -NMAXPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::globalNP0Ds, prefix + "NP0Ds", "I",
                   "", counter_index, counter, -NMAXPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::globalNHits, prefix + "NHits", "I",
                   "", counter_index, counter, -NMAXPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::globalCharge, prefix + "Charge",
                   "F", "", counter_index, counter, -NMAXPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::globalMomentum, prefix + "Momentum",
                   "F", "", counter_index, counter, -NMAXPARTICLES);

  man.AddVectorVar(prefix_index + TreeVars::globalToFFGD1_FGD2,
                   prefix + "ToFFGD1_FGD2", "F", "", counter_index, counter,
                   -NMAXPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::globalToFP0D_FGD1,
                   prefix + "ToFP0D_FGD1", "F", "", counter_index, counter,
                   -NMAXPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::globalToFDSECal_FGD1,
                   prefix + "ToFDSECal_FGD1", "F", "", counter_index, counter,
                   -NMAXPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::globalToFECal_FGD1,
                   prefix + "ToFECal_FGD1", "F", "", counter_index, counter,
                   -NMAXPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::globalToFDSECal_FGD2,
                   prefix + "ToFDSECal_FGD2", "F", "", counter_index, counter,
                   -NMAXPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::globalToFECal_FGD2,
                   prefix + "ToFECal_FGD2", "F", "", counter_index, counter,
                   -NMAXPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::globalToFFlag_FGD1_FGD2,
                   prefix + "ToFFlag_FGD1_FGD2", "I", "", counter_index,
                   counter, -NMAXPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::globalToFFlag_P0D_FGD1,
                   prefix + "ToFFlag_P0D_FGD1", "I", "", counter_index, counter,
                   -NMAXPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::globalToFFlag_DSECal_FGD1,
                   prefix + "ToFFlag_DSECal_FGD1", "I", "", counter_index,
                   counter, -NMAXPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::globalToFFlag_ECal_FGD1,
                   prefix + "ToFFlag_ECal_FGD1", "I", "", counter_index,
                   counter, -NMAXPARTICLES);
   man.AddVectorVar(prefix_index + TreeVars::globalToFFlag_DSECal_FGD2,
                   prefix + "ToFFlag_DSECal_FGD2", "I", "", counter_index,
                   counter, -NMAXPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::globalToFFlag_ECal_FGD2,
                   prefix + "ToFFlag_ECal_FGD2", "I", "", counter_index,
                   counter, -NMAXPARTICLES);

  man.AddVectorVar(prefix_index + TreeVars::globalRangeMomentumMuon,
                   prefix + "RangeMomentumMuon", "F", "", counter_index,
                   counter, -NMAXPARTICLES);
  man.AddMatrixVar(prefix_index + TreeVars::globalDirectionStart,
                   prefix + "DirectionStart", "F", "", counter_index, counter,
                   -NMAXPARTICLES, 3);
  man.AddMatrixVar(prefix_index + TreeVars::globalDirectionEnd,
                   prefix + "DirectionEnd", "F", "", counter_index, counter,
                   -NMAXPARTICLES, 3);
  man.AddMatrixVar(prefix_index + TreeVars::globalPositionStart,
                   prefix + "PositionStart", "F", "", counter_index, counter,
                   -NMAXPARTICLES, 4);
  man.AddMatrixVar(prefix_index + TreeVars::globalPositionEnd,
                   prefix + "PositionEnd", "F", "", counter_index, counter,
                   -NMAXPARTICLES, 4);

  man.AddVectorVar(prefix_index + TreeVars::globalMomentumFlip,
                   prefix + "MomentumFlip", "F", "", counter_index, counter,
                   -NMAXPARTICLES);
  man.AddMatrixVar(prefix_index + TreeVars::globalDirectionStartFlip,
                   prefix + "DirectionStartFlip", "F", "", counter_index,
                   counter, -NMAXPARTICLES, 3);

  man.AddVectorVar(prefix_index + TreeVars::globalTrueTrackID,
                   prefix + "TrueTrackID", "I", "", counter_index, counter,
                   -NMAXPARTICLES);

  // From AnaTrack

  man.AddVectorVar(prefix_index + TreeVars::globalStatus, prefix + "Status",
                   "I", "", counter_index, counter, -NMAXPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::globalNNodes, prefix + "NNodes",
                   "I", "", counter_index, counter, -NMAXPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::globalMomentumError,
                   prefix + "MomentumError", "F", "", counter_index, counter,
                   -NMAXPARTICLES);

  man.AddVectorVar(prefix_index + TreeVars::globalNTRACKERs,
                   prefix + "NTRACKERs", "I", "", counter_index, counter,
                   -NMAXPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::globalNDOF, prefix + "NDOF", "I",
                   "", counter_index, counter, -NMAXPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::globalChi2, prefix + "Chi2", "F",
                   "", counter_index, counter, -NMAXPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::globalMomentumMuon,
                   prefix + "MomentumMuon", "F", "", counter_index, counter,
                   -NMAXPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::globalMomentumErrorMuon,
                   prefix + "MomentumErrorMuon", "F", "", counter_index,
                   counter, -NMAXPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::globalMomentumProton,
                   prefix + "MomentumProton", "F", "", counter_index, counter,
                   -NMAXPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::globalMomentumErrorProton,
                   prefix + "MomentumErrorProton", "F", "", counter_index,
                   counter, -NMAXPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::globalMomentumEle,
                   prefix + "MomentumEle", "F", "", counter_index, counter,
                   -NMAXPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::globalMomentumErrorEle,
                   prefix + "MomentumErrorEle", "F", "", counter_index, counter,
                   -NMAXPARTICLES);

#if VERSION_HAS_REVERSED_REFITS
  man.AddMatrixVar(prefix_index + TreeVars::globalPositionStartFlip,
                   prefix + "PositionStartFlip", "F", "", counter_index,
                   counter, -NMAXPARTICLES, 4);
  man.AddMatrixVar(prefix_index + TreeVars::globalDirectionEndFlip,
                   prefix + "DirectionEndFlip", "F", "", counter_index, counter,
                   -NMAXPARTICLES, 3);
#endif

#if VERSION_HAS_PRANGE_ESTIMATES
  man.AddVectorVar(prefix_index + TreeVars::globalRangeMomentumEle,
                   prefix + "RangeMomentumEle", "F", "", counter_index, counter,
                   -NMAXPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::globalRangeMomentumProton,
                   prefix + "RangeMomentumProton", "F", "", counter_index,
                   counter, -NMAXPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::globalRangeMomentumMuonFlip,
                   prefix + "RangeMomentumMuonFlip", "F", "", counter_index,
                   counter, -NMAXPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::globalRangeMomentumProtonFlip,
                   prefix + "RangeMomentumProtonFlip", "F", "", counter_index,
                   counter, -NMAXPARTICLES);
#endif

  man.AddVectorVar(prefix_index + TreeVars::globalReconPDG, prefix + "ReconPDG",
                   "I", "", counter_index, counter, -NMAXPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::globalBunch, prefix + "Bunch", "I",
                   "", counter_index, counter, -NMAXPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::globalNReconVertices,
                   prefix + "NReconVertices", "I", "", counter_index, counter,
                   -NMAXPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::globalMomentumAtVertex,
                   prefix + "MomentumAtVertex", "F", "", counter_index, counter,
                   -NMAXPARTICLES);
  man.AddMatrixVar(prefix_index + TreeVars::globalDirectionAtVertex,
                   prefix + "DirectionAtVertex", "F", "", counter_index,
                   counter, -NMAXPARTICLES, 3);

#if VERSION_HAS_TIME_FITS
  man.AddVectorVar(prefix_index + TreeVars::globalNTimeNodes,
                   prefix + "NTimeNodes", "I", "", counter_index, counter,
                   -NMAXPARTICLES);
  AnaTimeNodeIO time;
  time.Define(man, counter, prefix + "TimeNode");
#endif

  // AnaTrack
  man.AddVectorVar(prefix_index + TreeVars::globalDetectors,
                   prefix + "Detectors", "I", "", counter_index, counter,
                   -NMAXPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::globalLength, prefix + "Length",
                   "F", "", counter_index, counter, -NMAXPARTICLES);

  counter_index++;
}

//********************************************************************
void AnaTrackIO::Write(OutputManager& man, const std::string& prefix) const {
  //********************************************************************

  (void)prefix;

  if (man.GetCounterValueForVar(prefix_index + TreeVars::globalUniqueID) >=
      (int)NMAXPARTICLES) {
    std::cout << "AnaTrackB:: too many tracks. Maximum is " << NMAXPARTICLES
              << std::endl;
    return;
  }

  // AnaTrackBIO

  man.FillVectorVar(prefix_index + TreeVars::globalUniqueID,       UniqueID);
  man.FillVectorVar(prefix_index + TreeVars::globalNHits,          NHits);
  man.FillVectorVar(prefix_index + TreeVars::globalCharge,         Charge);
  man.FillVectorVar(prefix_index + TreeVars::globalMomentum,       Momentum);
  man.FillVectorVar(prefix_index + TreeVars::globalToFFGD1_FGD2,   ToF.FGD1_FGD2);
  man.FillVectorVar(prefix_index + TreeVars::globalToFP0D_FGD1,    ToF.P0D_FGD1);
  man.FillVectorVar(prefix_index + TreeVars::globalToFDSECal_FGD1, ToF.DSECal_FGD1);
  man.FillVectorVar(prefix_index + TreeVars::globalToFECal_FGD1,   ToF.ECal_FGD1);
  man.FillVectorVar(prefix_index + TreeVars::globalToFDSECal_FGD2, ToF.DSECal_FGD2);
  man.FillVectorVar(prefix_index + TreeVars::globalToFECal_FGD2,   ToF.ECal_FGD2);
  
  man.FillVectorVar(prefix_index + TreeVars::globalToFFlag_FGD1_FGD2,
                    ToF.Flag_FGD1_FGD2);
  man.FillVectorVar(prefix_index + TreeVars::globalToFFlag_P0D_FGD1,
                    ToF.Flag_P0D_FGD1); 
  man.FillVectorVar(prefix_index + TreeVars::globalToFFlag_DSECal_FGD1,
                    ToF.Flag_DSECal_FGD1);
  man.FillVectorVar(prefix_index + TreeVars::globalToFFlag_ECal_FGD1,
                    ToF.Flag_ECal_FGD1);
  man.FillVectorVar(prefix_index + TreeVars::globalToFFlag_DSECal_FGD2,
                    ToF.Flag_DSECal_FGD2);
  man.FillVectorVar(prefix_index + TreeVars::globalToFFlag_ECal_FGD2,
                    ToF.Flag_ECal_FGD2);
  
  man.FillVectorVar(prefix_index + TreeVars::globalRangeMomentumMuon,
                    RangeMomentumMuon);

  for (UInt_t i = 0; i < NDETECTORS; i++) {
    Int_t used = (Int_t)(SubDetId::GetDetectorUsed(
        Detector, static_cast<SubDetId::SubDetEnum>(subdet_enum[i])));
    man.FillMatrixVar(prefix_index + TreeVars::globalDetUsed, used, -1, i);
  }

  man.FillVectorVar(prefix_index + TreeVars::globalNTPCs,
                    nTPCSegments);  // (int)TPCSegments .size()); - used to be a
                                    // vector, now a fixed length array with
                                    // hard coded no. of elements - may change
  man.FillVectorVar(prefix_index + TreeVars::globalNFGDs,
                    nFGDSegments);  // (int)FGDSegments .size());
  man.FillVectorVar(prefix_index + TreeVars::globalNECALs,
                    nECALSegments);  // (int)ECALSegments.size());
  man.FillVectorVar(prefix_index + TreeVars::globalNP0Ds,
                    nP0DSegments);  // (int)P0DSegments .size());
  man.FillVectorVar(prefix_index + TreeVars::globalNSMRDs,
                    nSMRDSegments);  // (int)SMRDSegments.size());

  man.FillMatrixVarFromArray(prefix_index + TreeVars::globalDirectionStart,
                             DirectionStart, -1, 3);
  man.FillMatrixVarFromArray(prefix_index + TreeVars::globalDirectionEnd,
                             DirectionEnd, -1, 3);

  man.FillMatrixVarFromArray(prefix_index + TreeVars::globalPositionStart,
                             PositionStart, -1, 4);

  man.FillMatrixVarFromArray(prefix_index + TreeVars::globalPositionEnd,
                             PositionEnd, -1, 4);

  man.FillMatrixVarFromArray(prefix_index + TreeVars::globalDirectionStartFlip,
                             DirectionStartFlip, -1, 3);

  man.FillVectorVar(prefix_index + TreeVars::globalMomentumFlip, MomentumFlip);

  if (TrueObject)
    man.FillVectorVar(prefix_index + TreeVars::globalTrueTrackID,
                      TrueObject->ID);

  man.FillVectorVar(prefix_index + TreeVars::globalStatus, Status);
  man.FillVectorVar(prefix_index + TreeVars::globalNNodes, NNodes);
  man.FillVectorVar(prefix_index + TreeVars::globalMomentumError,
                    MomentumError);

  man.FillVectorVar(prefix_index + TreeVars::globalNDOF, NDOF);
  man.FillVectorVar(prefix_index + TreeVars::globalChi2, Chi2);
  man.FillVectorVar(prefix_index + TreeVars::globalMomentumMuon, MomentumMuon);
  man.FillVectorVar(prefix_index + TreeVars::globalMomentumErrorMuon,
                    MomentumErrorMuon);
  man.FillVectorVar(prefix_index + TreeVars::globalMomentumProton,
                    MomentumProton);
  man.FillVectorVar(prefix_index + TreeVars::globalMomentumErrorProton,
                    MomentumErrorProton);
  man.FillVectorVar(prefix_index + TreeVars::globalMomentumEle, MomentumEle);
  man.FillVectorVar(prefix_index + TreeVars::globalMomentumErrorEle,
                    MomentumErrorEle);
#if VERSION_HAS_REVERSED_REFITS
  man.FillMatrixVarFromArray(prefix_index + TreeVars::globalPositionStartFlip,
                             PositionStartFlip, -1, 4);
  man.FillMatrixVarFromArray(prefix_index + TreeVars::globalDirectionEndFlip,
                             DirectionEndFlip, -1, 3);
#endif

#if VERSION_HAS_PRANGE_ESTIMATES
  man.FillVectorVar(prefix_index + TreeVars::globalRangeMomentumEle,
                    RangeMomentumEle);
  man.FillVectorVar(prefix_index + TreeVars::globalRangeMomentumProton,
                    RangeMomentumProton);
  man.FillVectorVar(prefix_index + TreeVars::globalRangeMomentumMuonFlip,
                    RangeMomentumMuonFlip);
  man.FillVectorVar(prefix_index + TreeVars::globalRangeMomentumProtonFlip,
                    RangeMomentumProtonFlip);

#endif

  man.FillVectorVar(prefix_index + TreeVars::globalNTRACKERs,
                    (int)TRACKERSegments.size());
  man.FillVectorVar(prefix_index + TreeVars::globalReconPDG, ReconPDG);
  man.FillVectorVar(prefix_index + TreeVars::globalBunch, Bunch);
  man.FillVectorVar(prefix_index + TreeVars::globalNReconVertices,
                    (int)ReconVertices.size());
  man.FillVectorVar(prefix_index + TreeVars::globalMomentumAtVertex,
                    MomentumAtVertex);

  man.FillMatrixVarFromArray(prefix_index + TreeVars::globalDirectionAtVertex,
                             DirectionAtVertex, -1, 3);

#if VERSION_HAS_TIME_FITS
  for (UInt_t i = 0; i < TimeNodes.size(); i++) {
    (static_cast<AnaTimeNodeIO*>(TimeNodes[i]))->Write(man, prefix, -1, i);
  }
  man.FillVectorVar(prefix_index + TreeVars::globalNTimeNodes,
                    (Int_t)TimeNodes.size());
#endif

  // AnaTrack
  man.FillVectorVar(prefix_index + TreeVars::globalDetectors, Detectors);
  man.FillVectorVar(prefix_index + TreeVars::globalLength, Length);

  man.IncrementCounterForVar(prefix_index + TreeVars::globalUniqueID);
}

//********************************************************************
void AnaDetCrossingIO::Define(OutputManager& man, const std::string& counter,
                              const std::string& prefix) const {
  //********************************************************************
  man.AddMatrixVar(prefix_index + TreeVars::detCrossInActive,
                   prefix + "InActive", "I", "", counter_index, counter,
                   -NMAXTRUEPARTICLES, NMAXCROSSEDDET);
  man.AddMatrixVar(prefix_index + TreeVars::detCrossDetector,
                   prefix + "Detector", "I", "", counter_index, counter,
                   -NMAXTRUEPARTICLES, NMAXCROSSEDDET);

  man.Add3DMatrixVar(prefix_index + TreeVars::detCrossEntrancePosition,
                     prefix + "EntrancePosition", "F", "", counter_index,
                     counter, -NMAXTRUEPARTICLES, NMAXCROSSEDDET, 4);
  man.Add3DMatrixVar(prefix_index + TreeVars::detCrossExitPosition,
                     prefix + "ExitPosition", "F", "", counter_index, counter,
                     -NMAXTRUEPARTICLES, NMAXCROSSEDDET, 4);

  man.Add3DMatrixVar(prefix_index + TreeVars::detCrossEntranceMomentum,
                     prefix + "EntranceMomentum", "F", "", counter_index,
                     counter, -NMAXTRUEPARTICLES, NMAXCROSSEDDET, 4);
  man.Add3DMatrixVar(prefix_index + TreeVars::detCrossExitMomentum,
                     prefix + "ExitMomentum", "F", "", counter_index, counter,
                     -NMAXTRUEPARTICLES, NMAXCROSSEDDET, 4);

  // counter_index++;
}

//********************************************************************
void AnaDetCrossingIO::Write(OutputManager& man, const std::string& prefix,
                             Int_t indx1, Int_t indx2) const {
  //********************************************************************

  (void)prefix;

  if (man.GetCounterValueForVar(prefix_index + TreeVars::trueTrackPDG) >=
      (int)NMAXTRUEPARTICLES) {
    std::cout << "too many DetCrossingB's. Maximum is " << NMAXTRUEPARTICLES
              << std::endl;
    return;
  }

  Int_t trueDet;
  convUtils::ConvertBitFieldToTrueParticleDetEnum(Detector, trueDet);

  man.FillMatrixVar(prefix_index + TreeVars::detCrossInActive, (int)InActive,
                    indx1, indx2);
  man.FillMatrixVar(prefix_index + TreeVars::detCrossDetector, trueDet, indx1,
                    indx2);

  man.Fill3DMatrixVarFromArray(
      prefix_index + TreeVars::detCrossEntrancePosition, EntrancePosition,
      indx1, indx2, 4);
  man.Fill3DMatrixVarFromArray(prefix_index + TreeVars::detCrossExitPosition,
                               ExitPosition, indx1, indx2, 4);

  man.Fill3DMatrixVarFromArray(
      prefix_index + TreeVars::detCrossEntranceMomentum, EntranceMomentum,
      indx1, indx2, 4);
  man.Fill3DMatrixVarFromArray(prefix_index + TreeVars::detCrossExitMomentum,
                               ExitMomentum, indx1, indx2, 4);
}

//********************************************************************
AnaTrueParticleIO::AnaTrueParticleIO(const AnaTrueParticle& particle) {
  //********************************************************************
  // AnaTrueObjectC
  ID = particle.ID;

  // AnaTrueParticleB
  PDG        = particle.PDG;
  ParentID   = particle.ParentID;
  GParentID  = particle.GParentID;
  ParentPDG  = particle.ParentPDG;
  GParentPDG = particle.GParentPDG;

  anaUtils::CopyArray(particle.Position, Position, 4);
  anaUtils::CopyArray(particle.PositionEnd, PositionEnd, 4);
  anaUtils::CopyArray(particle.Direction, Direction, 3);

  Momentum = particle.Momentum;
  Charge = particle.Charge;
  VertexID = particle.VertexID;
  TrueVertex = particle.TrueVertex;

  /// store for each subdetector
  /// if the true truePart enter the active volume of the subdetector i
  /// what is the entrance position of the subdetector i
  ///  "    "  "   exit       "               "

  anaUtils::CreateArray(DetCrossings, particle.nDetCrossings);
  for (Int_t i = 0; i < particle.nDetCrossings; i++) {
    DetCrossings[i] = new AnaDetCrossingIO(*particle.DetCrossings[i]);
  }
  nDetCrossings = particle.nDetCrossings;

  DetCrossingsVect.clear();

  // AnaTrueTrack
  PrimaryID = particle.PrimaryID;
  Purity = particle.Purity;
  Bunch = particle.Bunch;
  VertexIndex = particle.VertexIndex;

  IsTruePrimaryPi0DecayPhoton = particle.IsTruePrimaryPi0DecayPhoton;
  IsTruePrimaryPi0DecayPhotonChild = particle.IsTruePrimaryPi0DecayPhotonChild;

  ReconParticles.clear();

  // The vector of "NReconTracks" should be filled with NULL to at least keep
  // the size
  // this is stored in the flat-trees
  ReconParticles =
      std::vector<AnaParticleB*>(particle.ReconParticles.size(), NULL);
}

//********************************************************************
void AnaTrueParticleIO::Define(OutputManager& man, const std::string& counter,
                               const std::string& prefix) const {
  //********************************************************************

  // AnaTrueTrackBIO
  man.AddVectorVar(prefix_index + TreeVars::trueTrackPrimaryID,
                   prefix + "PrimaryID", "I", "", counter_index, counter,
                   -NMAXTRUEPARTICLES); 
  man.AddVectorVar(prefix_index + TreeVars::trueTrackID, prefix + "ID", "I", "",
                   counter_index, counter, -NMAXTRUEPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::trueTrackPDG, prefix + "PDG", "I",
                   "", counter_index, counter, -NMAXTRUEPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::trueTrackParentPDG,
                   prefix + "ParentPDG", "I", "", counter_index, counter,
                   -NMAXTRUEPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::trueTrackGParentPDG,
                   prefix + "GParentPDG", "I", "", counter_index, counter,
                   -NMAXTRUEPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::trueTrackParentID,
                   prefix + "ParentID", "I", "", counter_index, counter,
                   -NMAXTRUEPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::trueTrackGParentID,
                   prefix + "GParentID", "I", "", counter_index, counter,
                   -NMAXTRUEPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::trueTrackMomentum,
                   prefix + "Momentum", "F", "", counter_index, counter,
                   -NMAXTRUEPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::trueTrackCharge, prefix + "Charge",
                   "F", "", counter_index, counter, -NMAXTRUEPARTICLES);
  man.AddMatrixVar(prefix_index + TreeVars::trueTrackPosition,
                   prefix + "Position", "F", "", counter_index, counter,
                   -NMAXTRUEPARTICLES, 4);
  man.AddMatrixVar(prefix_index + TreeVars::trueTrackPositionEnd,
                   prefix + "PositionEnd", "F", "", counter_index, counter,
                   -NMAXTRUEPARTICLES, 4);
  man.AddMatrixVar(prefix_index + TreeVars::trueTrackDirection,
                   prefix + "Direction", "F", "", counter_index, counter,
                   -NMAXTRUEPARTICLES, 3);
  man.AddVectorVar(prefix_index + TreeVars::trueTrackTruthVertexID,
                   prefix + "TruthVertexID", "I", "", counter_index, counter,
                   -NMAXTRUEPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::trueTrackNDetCrossings,
                   prefix + "NDetCrossings", "I", "", counter_index, counter,
                   -NMAXTRUEPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::trueTrackIsTruePrimaryPi0DecayPhoton,
                   prefix + "IsTruePrimaryPi0DecayPhoton", "I", "", counter_index, counter,
                   -NMAXTRUEPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::trueTrackIsTruePrimaryPi0DecayPhotonChild,
                   prefix + "IsTruePrimaryPi0DecayPhotonChild", "I", "", counter_index, counter, 
                   -NMAXTRUEPARTICLES);

  AnaDetCrossingIO cross;
  cross.Define(man, counter, prefix);

  // AnaTrueTrack

  man.AddVectorVar(prefix_index + TreeVars::trueTrackPurity, prefix + "Purity",
                   "F", "", counter_index, counter, -NMAXTRUEPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::trueTrackBunch, prefix + "Bunch",
                   "I", "", counter_index, counter, -NMAXTRUEPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::trueTrackNReconTracks,
                   prefix + "NReconTracks", "I", "", counter_index, counter,
                   -NMAXTRUEPARTICLES);
  man.AddVectorVar(prefix_index + TreeVars::trueTrackVertexIndex,
                   prefix + "VertexIndex", "I", "", counter_index, counter,
                   -NMAXTRUEPARTICLES);

  counter_index++;
}

//********************************************************************
void AnaTrueParticleIO::Write(OutputManager& man, const Int_t trueVertexIndex,
                              const std::string& prefix) const {
  //********************************************************************

  (void)trueVertexIndex;
  (void)prefix;

  if (man.GetCounterValueForVar(prefix_index + TreeVars::trueTrackPDG) >=
      (int)NMAXTRUEPARTICLES) {
    std::cout << "too many TrueParticle's. Maximum is " << NMAXTRUEPARTICLES
              << std::endl;
    return;
  }

  // AnaTrueTrackBIO
  man.FillVectorVar(prefix_index + TreeVars::trueTrackID,         ID);
  man.FillVectorVar(prefix_index + TreeVars::trueTrackPDG,        PDG);
  man.FillVectorVar(prefix_index + TreeVars::trueTrackPrimaryID,  PrimaryID);
  man.FillVectorVar(prefix_index + TreeVars::trueTrackParentPDG,  ParentPDG);
  man.FillVectorVar(prefix_index + TreeVars::trueTrackGParentPDG, GParentPDG);
  man.FillVectorVar(prefix_index + TreeVars::trueTrackParentID,   ParentID);
  man.FillVectorVar(prefix_index + TreeVars::trueTrackGParentID,  GParentID);
  
  man.FillVectorVar(prefix_index + TreeVars::trueTrackMomentum,   Momentum);
  man.FillVectorVar(prefix_index + TreeVars::trueTrackCharge,     Charge);

  man.FillMatrixVarFromArray(prefix_index + TreeVars::trueTrackDirection,
                             Direction, -1, 3);
  man.FillMatrixVarFromArray(prefix_index + TreeVars::trueTrackPosition,
                             Position, -1, 4);
  man.FillMatrixVarFromArray(prefix_index + TreeVars::trueTrackPositionEnd,
                             PositionEnd, -1, 4);
  
  man.FillVectorVar(
      prefix_index + TreeVars::trueTrackIsTruePrimaryPi0DecayPhoton,
      IsTruePrimaryPi0DecayPhoton);
  man.FillVectorVar(
      prefix_index + TreeVars::trueTrackIsTruePrimaryPi0DecayPhotonChild,
      IsTruePrimaryPi0DecayPhoton);

  man.FillVectorVar(prefix_index + TreeVars::trueTrackTruthVertexID, VertexID);

  for (Int_t i = 0; i < nDetCrossings; i++) {
    // need to fill
    static_cast<const AnaDetCrossingIO*>(DetCrossings[i])
        ->AnaDetCrossingIO::Write(man, prefix, -1, i);
  }
  man.FillVectorVar(prefix_index + TreeVars::trueTrackNDetCrossings,
                    nDetCrossings);

  // AnaTrueTrack
  man.FillVectorVar(prefix_index + TreeVars::trueTrackPurity, Purity);
  man.FillVectorVar(prefix_index + TreeVars::trueTrackBunch, Bunch);
  man.FillVectorVar(prefix_index + TreeVars::trueTrackNReconTracks,
                    (int)ReconParticles.size());
 
  // Save the indices with which true vertices are saved (instead of ID) in
  // order to speed up the associations (-1 if vertex not found, maybe not
  // saved).
  // (-2 -> default value, when the track doesn't have a vertex).
  if (trueVertexIndex == -2)
    man.FillVectorVar(prefix_index + TreeVars::trueTrackVertexIndex,
                      VertexIndex);
  else
    man.FillVectorVar(prefix_index + TreeVars::trueTrackVertexIndex,
                      trueVertexIndex);

  man.IncrementCounterForVar(prefix_index + TreeVars::trueTrackPDG);
}

//********************************************************************
void AnaTrueVertexIO::Define(OutputManager& man, const std::string& counter,
                             const std::string& prefix) const {
  //********************************************************************

  // AnaTrueVertexBIO
  man.AddVectorVar(prefix_index + TreeVars::trueVertexID, prefix + "ID", "I",
                   "", counter_index, counter, -NMAXTRUEVERTICES);
  man.AddVectorVar(prefix_index + TreeVars::trueVertexNuPDG, prefix + "NuPDG",
                   "I", "", counter_index, counter, -NMAXTRUEVERTICES);
  man.AddVectorVar(prefix_index + TreeVars::trueVertexNuEnergy,
                   prefix + "NuEnergy", "F", "", counter_index, counter,
                   -NMAXTRUEVERTICES);
  man.AddMatrixVar(prefix_index + TreeVars::trueVertexPosition,
                   prefix + "Position", "F", "", counter_index, counter,
                   -NMAXTRUEVERTICES, 4);
  man.AddVectorVar(prefix_index + TreeVars::trueVertexNTrueTracks,
                   prefix + "NTrueTracks", "I", "", counter_index, counter,
                   -NMAXTRUEVERTICES);
  man.AddVectorVar(prefix_index + TreeVars::trueVertexBunch, prefix + "Bunch",
                   "I", "", counter_index, counter, -NMAXTRUEVERTICES);
  man.AddVectorVar(prefix_index + TreeVars::trueVertexDetector,
                   prefix + "Detector", "I", "", counter_index, counter,
                   -NMAXTRUEVERTICES);

  // AnaTrueVertex
  man.AddMatrixVar(prefix_index + TreeVars::trueVertexLeptonDir,
                   prefix + "LeptonDir", "F", "", counter_index, counter,
                   -NMAXTRUEVERTICES, 3);
  man.AddVectorVar(prefix_index + TreeVars::trueVertexLeptonMom,
                   prefix + "LeptonMom", "F", "", counter_index, counter,
                   -NMAXTRUEVERTICES);
  man.AddVectorVar(prefix_index + TreeVars::trueVertexLeptonPDG,
                   prefix + "LeptonPDG", "I", "", counter_index, counter,
                   -NMAXTRUEVERTICES);
  man.AddVectorVar(prefix_index + TreeVars::trueVertexNBaryons,
                   prefix + "NBaryons", "I", "", counter_index, counter,
                   -NMAXTRUEVERTICES);
  man.AddVectorVar(prefix_index + TreeVars::trueVertexNReconTracks,
                   prefix + "NReconTracks", "I", "", counter_index, counter,
                   -NMAXTRUEVERTICES);
  man.AddVectorVar(prefix_index + TreeVars::trueVertexNReconVertices,
                   prefix + "NReconVertices", "I", "", counter_index, counter,
                   -NMAXTRUEVERTICES);
  man.AddMatrixVar(prefix_index + TreeVars::trueVertexNuDir, prefix + "NuDir",
                   "F", "", counter_index, counter, -NMAXTRUEVERTICES, 3);
  man.AddMatrixVar(prefix_index + TreeVars::trueVertexNuParentDecPoint,
                   prefix + "NuParentDecPoint", "F", "", counter_index, counter,
                   -NMAXTRUEVERTICES, 4);
  man.AddVectorVar(prefix_index + TreeVars::trueVertexNuParentPDG,
                   prefix + "NuParentPDG", "I", "", counter_index, counter,
                   -NMAXTRUEVERTICES);
  man.AddMatrixVar(prefix_index + TreeVars::trueVertexProtonDir,
                   prefix + "ProtonDir", "F", "", counter_index, counter,
                   -NMAXTRUEVERTICES, 3);
  man.AddVectorVar(prefix_index + TreeVars::trueVertexProtonMom,
                   prefix + "ProtonMom", "F", "", counter_index, counter,
                   -NMAXTRUEVERTICES);
  man.AddMatrixVar(prefix_index + TreeVars::trueVertexPionDir,
                   prefix + "PionDir", "F", "", counter_index, counter,
                   -NMAXTRUEVERTICES, 3);
  man.AddVectorVar(prefix_index + TreeVars::trueVertexPionMom,
                   prefix + "PionMom", "F", "", counter_index, counter,
                   -NMAXTRUEVERTICES);
  man.AddVectorVar(prefix_index + TreeVars::trueVertexQ2, prefix + "Q2", "F",
                   "", counter_index, counter, -NMAXTRUEVERTICES);
  man.AddVectorVar(prefix_index + TreeVars::trueVertexReacCode,
                   prefix + "ReacCode", "I", "", counter_index, counter,
                   -NMAXTRUEVERTICES);
  man.AddVectorVar(prefix_index + TreeVars::trueVertexRooVtxEntry,
                   prefix + "RooVtxEntry", "I", "", counter_index, counter,
                   -NMAXTRUEVERTICES);
  man.AddVectorVar(prefix_index + TreeVars::trueVertexRooVtxIndex,
                   prefix + "RooVtxIndex", "I", "", counter_index, counter,
                   -NMAXTRUEVERTICES);
  man.AddMatrixVar(prefix_index + TreeVars::trueVertexTargetDir,
                   prefix + "TargetDir", "F", "", counter_index, counter,
                   -NMAXTRUEVERTICES, 3);
  man.AddVectorVar(prefix_index + TreeVars::trueVertexTargetMom,
                   prefix + "TargetMom", "F", "", counter_index, counter,
                   -NMAXTRUEVERTICES);
  man.AddVectorVar(prefix_index + TreeVars::trueVertexTargetPDG,
                   prefix + "TargetPDG", "I", "", counter_index, counter,
                   -NMAXTRUEVERTICES);
  man.AddVectorVar(prefix_index + TreeVars::trueVertexIsPauliBlocked,
                   prefix + "IsPauliBlocked", "I", "", counter_index, counter,
                   -NMAXTRUEVERTICES);
  man.AddVectorVar(prefix_index + TreeVars::trueVertexIsCohOnH,
                   prefix + "IsCohOnH", "I", "", counter_index, counter,
                   -NMAXTRUEVERTICES);
  man.AddMatrixVar(prefix_index + TreeVars::trueVertexNPrimaryParticles,
                   prefix + "NPrimaryParticles", "I", "", counter_index,
                   counter, -NMAXTRUEVERTICES, Int_t(ParticleId::kLast) + 1);
  counter_index++;
}

//********************************************************************
void AnaTrueVertexIO::Write(OutputManager& man,
                            const std::string& prefix) const {
  //********************************************************************

  (void)prefix;

  if (man.GetCounterValueForVar(prefix_index + TreeVars::trueVertexNuPDG) >=
      (int)NMAXTRUEVERTICES) {
    std::cout << "too many true vertices. Maximum is " << NMAXTRUEVERTICES
              << std::endl;
    return;
  }
  man.FillVectorVar(prefix_index + TreeVars::trueVertexID, ID);
  man.FillVectorVar(prefix_index + TreeVars::trueVertexNuPDG, NuPDG);
  man.FillVectorVar(prefix_index + TreeVars::trueVertexNuEnergy, NuEnergy);
  man.FillMatrixVarFromArray(prefix_index + TreeVars::trueVertexPosition,
                             Position, -1, 4);
  man.FillVectorVar(prefix_index + TreeVars::trueVertexNTrueTracks,
                    nTrueParticles);  //  (int)TrueParticles.size());
  man.FillVectorVar(prefix_index + TreeVars::trueVertexBunch, Bunch);
  man.FillVectorVar(prefix_index + TreeVars::trueVertexDetector,
                    (Int_t)Detector);

  man.FillVectorVar(prefix_index + TreeVars::trueVertexRooVtxIndex,
                    RooVtxIndex);
  man.FillVectorVar(prefix_index + TreeVars::trueVertexRooVtxEntry,
                    RooVtxEntry);
  man.FillVectorVar(prefix_index + TreeVars::trueVertexReacCode, ReacCode);

  man.FillVectorVar(prefix_index + TreeVars::trueVertexTargetPDG, TargetPDG);

  man.FillMatrixVarFromArray(prefix_index + TreeVars::trueVertexNuDir, NuDir,
                             -1, 3);

  man.FillVectorVar(prefix_index + TreeVars::trueVertexNuParentPDG,
                    NuParentPDG);
  man.FillMatrixVarFromArray(
      prefix_index + TreeVars::trueVertexNuParentDecPoint, NuParentDecPoint, -1,
      4);

  man.FillVectorVar(prefix_index + TreeVars::trueVertexNBaryons, NBaryons);
  man.FillVectorVar(prefix_index + TreeVars::trueVertexLeptonPDG, LeptonPDG);
  man.FillVectorVar(prefix_index + TreeVars::trueVertexQ2, Q2);

  man.FillMatrixVarFromArray(prefix_index + TreeVars::trueVertexTargetDir,
                             TargetDir, -1, 3);
  man.FillMatrixVarFromArray(prefix_index + TreeVars::trueVertexLeptonDir,
                             LeptonDir, -1, 3);
  man.FillMatrixVarFromArray(prefix_index + TreeVars::trueVertexProtonDir,
                             ProtonDir, -1, 3);
  man.FillMatrixVarFromArray(prefix_index + TreeVars::trueVertexPionDir,
                             PionDir, -1, 3);

  man.FillVectorVar(prefix_index + TreeVars::trueVertexTargetMom, TargetMom);
  man.FillVectorVar(prefix_index + TreeVars::trueVertexLeptonMom, LeptonMom);
  man.FillVectorVar(prefix_index + TreeVars::trueVertexProtonMom, ProtonMom);
  man.FillVectorVar(prefix_index + TreeVars::trueVertexPionMom, PionMom);

  man.FillVectorVar(prefix_index + TreeVars::trueVertexNReconTracks,
                    (int)ReconParticles.size());
  man.FillVectorVar(prefix_index + TreeVars::trueVertexNReconVertices,
                    (int)ReconVertices.size());

  man.FillVectorVar(prefix_index + TreeVars::trueVertexIsPauliBlocked,
                    (int)IsPauliBlocked);
  man.FillVectorVar(prefix_index + TreeVars::trueVertexIsCohOnH, (int)IsCohOnH);

  man.FillMatrixVarFromArray(
      prefix_index + TreeVars::trueVertexNPrimaryParticles, NPrimaryParticles,
      -1, Int_t(ParticleId::kLast) + 1);

  man.IncrementCounterForVar(prefix_index + TreeVars::trueVertexID);
}

//********************************************************************
AnaBunchIO::AnaBunchIO(const AnaBunch& bunch) {
  //********************************************************************

  // Only get the info needed to store the flat-trees
  Bunch = bunch.Bunch;
  Weight = bunch.Weight;
}
//********************************************************************
void AnaBunchIO::Define(OutputManager& man, const std::string& prefix) const {
  //********************************************************************

  man.AddVar(prefix_index + TreeVars::bunchBunch, prefix + "Bunch", "I", "");
  man.AddVar(prefix_index + TreeVars::bunchWeight, prefix + "Weight", "F", "");
}

//********************************************************************
void AnaBunchIO::Write(OutputManager& man, const std::string& prefix) const {
  //********************************************************************

  (void)prefix;

  man.FillVar(prefix_index + TreeVars::bunchBunch, Bunch);
  man.FillVar(prefix_index + TreeVars::bunchWeight, Weight);
}

//********************************************************************
void AnaFgdTimeBinIO::Define(OutputManager& man, const std::string& counter,
                             const std::string& prefix) const {
  //********************************************************************

  man.AddVectorVar(prefix_index + TreeVars::fgdBinMinTime, prefix + "MinTime",
                   "F", "", counter_index, counter, -NMAXFGDTIMEBINS);
  man.AddVectorVar(prefix_index + TreeVars::fgdBinNHits1, prefix + "NHits1",
                   "I", "", counter_index, counter, -NMAXFGDTIMEBINS);
  man.AddVectorVar(prefix_index + TreeVars::fgdBinNHits2, prefix + "NHits2",
                   "I", "", counter_index, counter, -NMAXFGDTIMEBINS);
  man.AddVectorVar(prefix_index + TreeVars::fgdBinRawChargeSum1,
                   prefix + "RawChargeSum1", "F", "", counter_index, counter,
                   -NMAXFGDTIMEBINS);
  man.AddVectorVar(prefix_index + TreeVars::fgdBinRawChargeSum2,
                   prefix + "RawChargeSum2", "F", "", counter_index, counter,
                   -NMAXFGDTIMEBINS);

  counter_index++;
}

//********************************************************************
void AnaFgdTimeBinIO::Write(OutputManager& man,
                            const std::string& prefix) const {
  //********************************************************************

  (void)prefix;

  if (man.GetCounterValueForVar(prefix_index + TreeVars::fgdBinMinTime) >=
      (int)NMAXFGDTIMEBINS) {
    std::cout << "too many time bins. Maximum is " << NMAXFGDTIMEBINS
              << std::endl;
    return;
  }

  man.FillVectorVar(prefix_index + TreeVars::fgdBinMinTime, MinTime);
  man.FillVectorVar(prefix_index + TreeVars::fgdBinNHits1, NHits[0]);
  man.FillVectorVar(prefix_index + TreeVars::fgdBinNHits2, NHits[1]);
  man.FillVectorVar(prefix_index + TreeVars::fgdBinRawChargeSum1,
                    RawChargeSum[0]);
  man.FillVectorVar(prefix_index + TreeVars::fgdBinRawChargeSum2,
                    RawChargeSum[1]);

  man.IncrementCounterForVar(prefix_index + TreeVars::fgdBinMinTime);
}

//********************************************************************
AnaSpillIO::AnaSpillIO(const AnaSpill& spill) {
  //********************************************************************

  // Only get the info need to dump it into the flat-tree

  GeomID = spill.GeomID;

  NTotalTrueVertices = spill.NTotalTrueVertices;
  NTotalTrueParticles = spill.NTotalTrueParticles;

  TriggerIO = new AnaTriggerIO(spill.Trigger);

  InputFileIndex = spill.InputFileIndex;
  RooVtxEntry = spill.RooVtxEntry;

  EventInfo = NULL;
  if (spill.EventInfo)
    EventInfo =
        new AnaEventInfoIO(*static_cast<const AnaEventInfo*>(spill.EventInfo));

  Beam = NULL;
  if (spill.Beam)
    Beam = new AnaBeamIO(*static_cast<const AnaBeam*>(spill.Beam));

  DataQuality = NULL;
  if (spill.DataQuality)
    DataQuality = new AnaDataQualityIO(
        *static_cast<const AnaDataQuality*>(spill.DataQuality));
}
//********************************************************************
void AnaSpillIO::Define(OutputManager& man, const std::string& prefix) const {
  //********************************************************************

  //--- spill variables -------
  man.AddVar(prefix_index + TreeVars::spillNBunches, prefix + "NBunches", "I",
             "");
  man.AddVar(prefix_index + TreeVars::spillNTotalTrueVertices,
             prefix + "NTotalTrueVertices", "I", "");
  man.AddVar(prefix_index + TreeVars::spillNTotalTrueTracks,
             prefix + "NTotalTrueTracks", "I", "");

  AnaEventInfoIO e;
  e.Define(man, prefix);
  AnaBeamIO b;
  b.Define(man, prefix + "Beam");
  AnaDataQualityIO d;
  d.Define(man, prefix + "DQ");

  AnaTriggerIO t;
  t.Define(man, prefix + "Trigger");

  man.AddVar(prefix_index + TreeVars::spillRooVtxEntry, prefix + "RooVtxEntry",
             "I", "");
  man.AddVar(prefix_index + TreeVars::spillInputFileIndex,
             prefix + "InputFileIndex", "I", "");
}

//********************************************************************
void AnaSpillIO::Write(OutputManager& man, const std::string& prefix) const {
  //********************************************************************

  (void)prefix;

  // Spill variables
  man.FillVar(prefix_index + TreeVars::spillNBunches, (int)Bunches.size());
  man.FillVar(prefix_index + TreeVars::spillNTotalTrueVertices,
              (int)NTotalTrueVertices);
  man.FillVar(prefix_index + TreeVars::spillNTotalTrueTracks,
              (int)NTotalTrueParticles);

  // because at this moment we can be only more or less sure that the object is
  // of IOB type defined the variables
  (static_cast<const AnaEventInfoIO*>(EventInfo))->Write(man, prefix);
  (static_cast<const AnaBeamIO*>(Beam))->Write(man, prefix + "Beam");
  (static_cast<const AnaDataQualityIO*>(DataQuality))
      ->Write(man, prefix + "DQ");

  TriggerIO->Write(man, prefix + "Trigger");

  man.FillVar(prefix_index + TreeVars::spillInputFileIndex, InputFileIndex);
  man.FillVar(prefix_index + TreeVars::spillRooVtxEntry, RooVtxEntry);
}

//********************************************************************
void AnaEventInfoIO::Define(OutputManager& man,
                            const std::string& prefix) const {
  //********************************************************************

  //--- spill variables -------
  man.AddVar(prefix_index + TreeVars::eventInfoIsMC, prefix + "IsMC", "I", "");
  man.AddVar(prefix_index + TreeVars::eventInfoIsSand, prefix + "IsSand", "I",
             "");
  man.AddVar(prefix_index + TreeVars::eventInfoRun, prefix + "Run", "I", "");
  man.AddVar(prefix_index + TreeVars::eventInfoSubRun, prefix + "SubRun", "I",
             "");
  man.AddVar(prefix_index + TreeVars::eventInfoEvt, prefix + "Evt", "I", "");
}

//********************************************************************
void AnaEventInfoIO::Write(OutputManager& man,
                           const std::string& prefix) const {
  //********************************************************************

  (void)prefix;

  // Spill variables
  man.FillVar(prefix_index + TreeVars::eventInfoIsMC, (Int_t)IsMC);
  man.FillVar(prefix_index + TreeVars::eventInfoIsSand, (Int_t)IsSand);
  man.FillVar(prefix_index + TreeVars::eventInfoRun, Run);
  man.FillVar(prefix_index + TreeVars::eventInfoSubRun, SubRun);
  man.FillVar(prefix_index + TreeVars::eventInfoEvt, Event);
}

//********************************************************************
void AnaDataQualityIO::Define(OutputManager& man,
                              const std::string& prefix) const {
  //********************************************************************

  //--- spill variables -------
  man.AddVar(prefix_index + TreeVars::dqGoodDaq, prefix + "GoodDaq", "I", "");
}

//********************************************************************
void AnaDataQualityIO::Write(OutputManager& man,
                             const std::string& prefix) const {
  //********************************************************************

  (void)prefix;

  // Spill variables
  man.FillVar(prefix_index + TreeVars::dqGoodDaq, GoodDaq);
}

//********************************************************************
void AnaBeamIO::Define(OutputManager& man, const std::string& prefix) const {
  //********************************************************************

  man.AddVar(prefix_index + TreeVars::beamGoodSpill, prefix + "GoodSpill", "I",
             "");
}

//********************************************************************
void AnaBeamIO::Write(OutputManager& man, const std::string& prefix) const {
  //********************************************************************

  (void)prefix;

  man.FillVar(prefix_index + TreeVars::beamGoodSpill, GoodSpill);
}

//********************************************************************
void AnaVertexIO::Define(OutputManager& man, const std::string& counter,
                         const std::string& prefix) const {
  //********************************************************************

  man.AddVectorVar(prefix_index + TreeVars::vertexPrimaryIndex,
                   prefix + "PrimaryIndex", "I", "", counter_index, counter,
                   -NMAXVERTICES);
  man.AddMatrixVar(prefix_index + TreeVars::vertexPosition, prefix + "Position",
                   "F", "", counter_index, counter, -NMAXVERTICES, 4);
  man.AddVectorVar(prefix_index + TreeVars::vertexNReconTracks,
                   prefix + "NReconTracks", "I", "", counter_index, counter,
                   -NMAXVERTICES);
  man.AddVectorVar(prefix_index + TreeVars::vertexNTrueVerticesMatch,
                   prefix + "NTrueVerticesMatch", "I", "", counter_index,
                   counter, -NMAXVERTICES);
  man.AddMatrixVar(prefix_index + TreeVars::vertexTracksUniqueID,
                   prefix + "TracksUniqueID", "I", "", counter_index, counter,
                   -NMAXVERTICES, NMAXPARTICLESINVERTEX);
  man.AddMatrixVar(prefix_index + TreeVars::vertexTrueVertexIndex,
                   prefix + "TrueVertexIndex", "I", "", counter_index, counter,
                   -NMAXVERTICES, NMAXTRUEVERTICESMATCH);
  man.AddMatrixVar(prefix_index + TreeVars::vertexTrueVerticesClean,
                   prefix + "TrueVerticesClean", "F", "", counter_index,
                   counter, -NMAXVERTICES, NMAXTRUEVERTICESMATCH);
  man.AddMatrixVar(prefix_index + TreeVars::vertexTrueVerticesCompl,
                   prefix + "TrueVerticesCompl", "F", "", counter_index,
                   counter, -NMAXVERTICES, NMAXTRUEVERTICESMATCH);
  counter_index++;
}

//********************************************************************
void AnaVertexIO::Write(OutputManager& man,
                        const std::vector<int> trueVertexIndices,
                        const std::string& prefix) const {
  //********************************************************************

  (void)prefix;

  if (man.GetCounterValueForVar(prefix_index + TreeVars::vertexPosition) >=
      (int)NMAXVERTICES) {
    std::cout << "AnaVertex:: too many global Vertices. Maximum is "
              << NMAXVERTICES << std::endl;
    return;
  }

  man.FillVectorVar(prefix_index + TreeVars::vertexPrimaryIndex, PrimaryIndex);
  man.FillMatrixVarFromArray(prefix_index + TreeVars::vertexPosition, Position,
                             -1, 4);

  man.FillVectorVar(prefix_index + TreeVars::vertexNTrueVerticesMatch,
                    (int)TrueVerticesMatch.size());
  man.FillVectorVar(prefix_index + TreeVars::vertexNReconTracks,
                    nParticles);  // (int)Particles.size());

  for (UInt_t i = 0; i < (UInt_t)nParticles; i++) {
    if (i >= NMAXPARTICLESINVERTEX) {
      std::cout
          << "AnaVertex:: too many global Vertex Constituents. Maximum is "
          << NMAXPARTICLESINVERTEX << std::endl;
      break;
    }
    if (Particles[i])
      man.FillMatrixVar(prefix_index + TreeVars::vertexTracksUniqueID,
                        Particles[i]->UniqueID, -1, i);
  }

  for (UInt_t i = 0; i < TrueVerticesMatch.size(); i++) {
    if (i >= NMAXTRUEVERTICESMATCH) {
      std::cout << "AnaVertex:: too many TrueVertices associated to a global "
                   "Vertex. Maximum is "
                << NMAXTRUEVERTICESMATCH << std::endl;
      break;
    }
    // Save the indices with which true vertices are saved (instead of ID) in
    // order to speed up the associations (-1 if vertex not found, maybe not
    // saved).
    //    man.FillMatrixVar(prefix_index+TreeVars::TrueVertexIndex,   -1,
    //    i,TrueVerticesMatch[i].first->ID);
    man.FillMatrixVar(prefix_index + TreeVars::vertexTrueVertexIndex,
                      trueVertexIndices[i], -1, i);
    man.FillMatrixVar(prefix_index + TreeVars::vertexTrueVerticesClean,
                      TrueVerticesMatch[i].second.Cleanliness, -1, i);
    man.FillMatrixVar(prefix_index + TreeVars::vertexTrueVerticesCompl,
                      TrueVerticesMatch[i].second.Completeness, -1, i);
  }

  man.IncrementCounterForVar(prefix_index + TreeVars::vertexPrimaryIndex);
}

//********************************************************************
void AnaEventIO::Define(OutputManager& man, const std::string& prefix) const {
  //********************************************************************

  man.AddVar(prefix_index + TreeVars::eventBunch, prefix + "Bunch", "I", "");

  static_cast<const AnaEventInfoIO*>(&EventInfo)->Define(man, prefix);
  static_cast<const AnaBeamIO*>(Beam)->Define(man, prefix + "Beam");
  static_cast<const AnaDataQualityIO*>(DataQuality)->Define(man, prefix + "DQ");
}

//********************************************************************
void AnaEventIO::Write(OutputManager& man, const std::string& prefix) const {
  //********************************************************************

  (void)prefix;

  man.FillVar(prefix_index + TreeVars::eventBunch, Bunch);

  static_cast<const AnaEventInfoIO*>(&EventInfo)->Write(man, prefix);
  static_cast<const AnaBeamIO*>(Beam)->Write(man, "Beam");
  static_cast<const AnaDataQualityIO*>(DataQuality)->Write(man, "DQ");
}

//********************************************************************
void AnaTriggerIO::Define(OutputManager& man, const std::string& prefix) const {
  //********************************************************************

  man.AddVar(prefix_index + TreeVars::triggerFGDCosmic, prefix + "FGDCosmic",
             "I", "");
  man.AddVar(prefix_index + TreeVars::triggerTripTCosmic,
             prefix + "TripTCosmic", "I", "");
 
  // Double_t gives ~16 decimal digits over ~6 in Float_t (need 10)
  man.AddVectorVar(prefix_index + TreeVars::triggerCTMTriggerPattern, prefix + "CTMTriggerPattern",
      "D", "", 6);
}

//********************************************************************
void AnaTriggerIO::Write(OutputManager& man, const std::string& prefix) const {
  //********************************************************************

  (void)prefix;

  man.FillVar(prefix_index + TreeVars::triggerFGDCosmic, FGDCosmic);
  man.FillVar(prefix_index + TreeVars::triggerTripTCosmic, TripTCosmic);
  
// Double_t gives ~16 decimal digits over ~6 in Float_t (need 10)
 for (UInt_t i = 0; i < 6; i++){ 
   man.FillVectorVar(prefix_index + TreeVars::triggerCTMTriggerPattern, (Double_t)CTMTriggerPattern[i], i);
 }
}

//********************************************************************
void AnaTimeNodeIO::Define(OutputManager& man, const std::string& counter,
                           const std::string& prefix) const {
  //********************************************************************

  man.AddMatrixVar(prefix_index + TreeVars::globalTimeNodeDetector,
                   prefix + "Detector", "I", "", counter_index, counter,
                   -NMAXPARTICLES, NMAXTIMENODES);
  man.AddMatrixVar(prefix_index + TreeVars::globalTimeNodeTimeStart,
                   prefix + "TimeStart", "F", "", counter_index, counter,
                   -NMAXPARTICLES, NMAXTIMENODES);
  man.AddMatrixVar(prefix_index + TreeVars::globalTimeNodeTimeEnd,
                   prefix + "TimeEnd", "F", "", counter_index, counter,
                   -NMAXPARTICLES, NMAXTIMENODES);
}

//********************************************************************
void AnaTimeNodeIO::Write(OutputManager& man, const std::string& prefix,
                          Int_t indx1, Int_t indx2) const {
  //********************************************************************

  (void)prefix;

  if (man.GetCounterValueForVar(prefix_index + TreeVars::globalStatus) >=
      (Int_t)NMAXPARTICLES) {
    std::cout << "AnaTrack:: too many tracks. Maximum is " << NMAXPARTICLES
              << std::endl;
    return;
  }

  man.FillMatrixVar(prefix_index + TreeVars::globalTimeNodeDetector, Detector,
                    indx1, indx2);
  man.FillMatrixVar(prefix_index + TreeVars::globalTimeNodeTimeStart, TimeStart,
                    indx1, indx2);
  man.FillMatrixVar(prefix_index + TreeVars::globalTimeNodeTimeEnd, TimeEnd,
                    indx1, indx2);
}

//********************************************************************
void AnaTECALReconObjectIO::Define(OutputManager& man,
                                   const std::string& counter,
                                   const std::string& prefix) const {
  //********************************************************************

  man.AddVectorVar(prefix_index + TreeVars::tecalReconAverageHitTime,
                   prefix + "AverageHitTime", "F", "", counter_index, counter,
                   -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconAverageZPos,
                   prefix + "AverageZPosition", "F", "", counter_index, counter,
                   -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconContainment,
                   prefix + "Containment", "F", "", counter_index, counter,
                   -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconEFitResult,
                   prefix + "EFitResult", "F", "", counter_index, counter,
                   -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconEFitUncertainty,
                   prefix + "EFitUncertainty", "F", "", counter_index, counter,
                   -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconFirstLayer,
                   prefix + "FirstLayer", "I", "", counter_index, counter,
                   -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconIsShowerLike,
                   prefix + "IsShowerLike", "I", "", counter_index, counter,
                   -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconIsTrackLike,
                   prefix + "IsTrackLike", "I", "", counter_index, counter,
                   -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconLastLayer,
                   prefix + "LastLayer", "I", "", counter_index, counter,
                   -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconLikeMIPEM,
                   prefix + "LikeMIPEM", "F", "", counter_index, counter,
                   -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconLikeMIPPion,
                   prefix + "LikeMIPPion", "F", "", counter_index, counter,
                   -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconLikeEMHIP,
                   prefix + "LikeEMHIP", "F", "", counter_index, counter,
                   -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconLikeMIPEMLow,
                   prefix + "LikeMIPEMLow", "F", "", counter_index, counter,
                   -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconMatchingLike,
                   prefix + "MatchingLike", "F", "", counter_index, counter,
                   -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconMichelTagNDelayedCluster,
                   prefix + "MichelTagNDelayedCluster", "I", "", counter_index,
                   counter, -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconModule, prefix + "Module",
                   "C", "", counter_index, counter, -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconMostDownStreamLayerHit,
                   prefix + "MostDownStreamLayerHit", "I", "", counter_index,
                   counter, -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconMostUpStreamLayerHit,
                   prefix + "MostUpStreamLayerHit", "I", "", counter_index,
                   counter, -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconNHits, prefix + "NHits",
                   "I", "", counter_index, counter, -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconNLayersHit,
                   prefix + "NLayersHit", "I", "", counter_index, counter,
                   -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconObjectLength,
                   prefix + "ObjectLength", "F", "", counter_index, counter,
                   -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconPIDAMR, prefix + "PIDAMR",
                   "F", "", counter_index, counter, -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconPIDAngle,
                   prefix + "PIDAngle", "F", "", counter_index, counter,
                   -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconPIDAsymmetry,
                   prefix + "PIDAsymmetry", "F", "", counter_index, counter,
                   -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconPIDCircularity,
                   prefix + "PIDCircularity", "F", "", counter_index, counter,
                   -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconPIDFBR, prefix + "PIDFBR",
                   "F", "", counter_index, counter, -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconPIDMaxRatio,
                   prefix + "PIDMaxRatio", "F", "", counter_index, counter,
                   -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconPIDMeanPos,
                   prefix + "PIDMeanPos", "F", "", counter_index, counter,
                   -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconPIDShowerAngle,
                   prefix + "PIDShowerAngle", "F", "", counter_index, counter,
                   -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconPIDShowerWidth,
                   prefix + "PIDShowerWidth", "F", "", counter_index, counter,
                   -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconPIDTransverseChargeRatio,
                   prefix + "PIDTransverseChargeRatio", "F", "", counter_index,
                   counter, -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconPIDTruncatedMaxRatio,
                   prefix + "PIDTruncatedMaxRatio", "F", "", counter_index,
                   counter, -NMAXTECALRECONOBJECTS);
  man.AddMatrixVar(prefix_index + TreeVars::tecalReconPointing,
                   prefix + "Pointing", "F", "", counter_index, counter,
                   -NMAXTECALRECONOBJECTS, 3);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconThrust, prefix + "Thrust",
                   "F", "", counter_index, counter, -NMAXTECALRECONOBJECTS);
  man.AddMatrixVar(prefix_index + TreeVars::tecalReconThrustAxis,
                   prefix + "ThrustAxis", "F", "", counter_index, counter,
                   -NMAXTECALRECONOBJECTS, 3);
  man.AddMatrixVar(prefix_index + TreeVars::tecalReconThrustOrigin,
                   prefix + "ThrustOrigin", "F", "", counter_index, counter,
                   -NMAXTECALRECONOBJECTS, 3);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconTimeBunch,
                   prefix + "TimeBunch", "I", "", counter_index, counter,
                   -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconTotalHitCharge,
                   prefix + "TotalHitCharge", "F", "", counter_index, counter,
                   -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconTrueID, prefix + "TrueID",
                   "I", "", counter_index, counter, -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconTrueIDPrimary,
                   prefix + "TrueIDPrimary", "I", "", counter_index, counter,
                   -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconTrueIDRecursive,
                   prefix + "TrueIDRecursive", "I", "", counter_index, counter,
                   -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconTrueIDSingle,
                   prefix + "TrueIDSingle", "I", "", counter_index, counter,
                   -NMAXTECALRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalReconUniqueID,
                   prefix + "UniqueID", "I", "", counter_index, counter,
                   -NMAXTECALRECONOBJECTS);

  man.AddVectorVar(prefix_index + TreeVars::tecalReconBunch, prefix + "Bunch",
                   "I", "", counter_index, counter, -NMAXTECALRECONOBJECTS);

  counter_index++;
}

//********************************************************************
void AnaTECALReconObjectIO::Write(OutputManager& man,
                                  const std::string& prefix) const {
  //********************************************************************

  (void)prefix;

  man.FillVectorVar(prefix_index + TreeVars::tecalReconAverageHitTime,
                    AverageHitTime);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconAverageZPos,
                    AverageZPos);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconContainment,
                    Containment);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconEFitResult, EFitResult);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconEFitUncertainty,
                    EFitUncertainty);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconFirstLayer, FirstLayer);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconIsShowerLike,
                    IsShowerLike);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconIsTrackLike,
                    IsTrackLike);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconLastLayer, LastLayer);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconLikeMIPEM, LikeMIPEM);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconLikeMIPPion,
                    LikeMIPPion);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconLikeEMHIP, LikeEMHIP);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconLikeMIPEMLow,
                    LikeMIPEMLow);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconMatchingLike,
                    MatchingLike);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconMichelTagNDelayedCluster,
                    MichelTagNDelayedCluster);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconModule, Module);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconMostDownStreamLayerHit,
                    MostDownStreamLayerHit);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconMostUpStreamLayerHit,
                    MostUpStreamLayerHit);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconNHits, NHits);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconNLayersHit, NLayersHit);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconObjectLength,
                    ObjectLength);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconPIDAMR, PIDAMR);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconPIDAngle, PIDAngle);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconPIDAsymmetry,
                    PIDAsymmetry);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconPIDCircularity,
                    PIDCircularity);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconPIDFBR, PIDFBR);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconPIDMaxRatio,
                    PIDMaxRatio);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconPIDMeanPos, PIDMeanPos);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconPIDShowerAngle,
                    PIDShowerAngle);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconPIDShowerWidth,
                    PIDShowerWidth);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconPIDTransverseChargeRatio,
                    PIDTransverseChargeRatio);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconPIDTruncatedMaxRatio,
                    PIDTruncatedMaxRatio);
  man.FillMatrixVarFromArray(prefix_index + TreeVars::tecalReconPointing,
                             Pointing, -1, 3);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconThrust, Thrust);
  man.FillMatrixVarFromArray(prefix_index + TreeVars::tecalReconThrustAxis,
                             ThrustAxis, -1, 3);
  man.FillMatrixVarFromArray(prefix_index + TreeVars::tecalReconThrustOrigin,
                             ThrustOrigin, -1, 3);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconTimeBunch, TimeBunch);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconTotalHitCharge,
                    TotalHitCharge);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconTrueID, TrueID);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconTrueIDPrimary,
                    TrueIDPrimary);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconTrueIDRecursive,
                    TrueIDRecursive);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconTrueIDSingle,
                    TrueIDSingle);
  man.FillVectorVar(prefix_index + TreeVars::tecalReconUniqueID, UniqueID);

  man.FillVectorVar(prefix_index + TreeVars::tecalReconBunch, Bunch);

  man.IncrementCounterForVar(prefix_index + TreeVars::tecalReconBunch);
}

//********************************************************************
void AnaTECALUnmatchedObjectIO::Define(OutputManager& man,
                                       const std::string& counter,
                                       const std::string& prefix) const {
  //********************************************************************

  man.AddVectorVar(prefix_index + TreeVars::tecalUnmatchedAverageHitTime,
                   prefix + "AverageHitTime", "F", "", counter_index, counter,
                   -NMAXTECALUNMATCHEOBJECTS);
  man.AddMatrixVar(prefix_index + TreeVars::tecalUnmatchedBackPos,
                   prefix + "BackPos", "F", "", counter_index, counter,
                   -NMAXTECALUNMATCHEOBJECTS, 3);
  man.AddMatrixVar(prefix_index + TreeVars::tecalUnmatchedFrontPos,
                   prefix + "FrontPos", "F", "", counter_index, counter,
                   -NMAXTECALUNMATCHEOBJECTS, 3);
  man.AddVectorVar(
      prefix_index + TreeVars::tecalUnmatchedMostDownStreamLayerHit,
      prefix + "MostDownStreamLayerHit", "I", "", counter_index, counter,
      -NMAXTECALUNMATCHEOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalUnmatchedMostUpStreamLayerHit,
                   prefix + "MostUpStreamLayerHit", "I", "", counter_index,
                   counter, -NMAXTECALUNMATCHEOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalUnmatchedNHits,
                   prefix + "NHits", "I", "", counter_index, counter,
                   -NMAXTECALUNMATCHEOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalUnmatchedTotalHitCharge,
                   prefix + "TotalHitCharge", "F", "", counter_index, counter,
                   -NMAXTECALUNMATCHEOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalUnmatchedTrueID,
                   prefix + "TrueID", "I", "", counter_index, counter,
                   -NMAXTECALUNMATCHEOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalUnmatchedTrueIDPrimary,
                   prefix + "TrueIDPrimary", "I", "", counter_index, counter,
                   -NMAXTECALUNMATCHEOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalUnmatchedTrueIDRecursive,
                   prefix + "TrueIDRecursive", "I", "", counter_index, counter,
                   -NMAXTECALUNMATCHEOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalUnmatchedTrueIDSingle,
                   prefix + "TrueIDSingle", "I", "", counter_index, counter,
                   -NMAXTECALUNMATCHEOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::tecalUnmatchedView, prefix + "View",
                   "I", "", counter_index, counter, -NMAXTECALUNMATCHEOBJECTS);

  man.AddVectorVar(prefix_index + TreeVars::tecalUnmatchedBunch,
                   prefix + "Bunch", "I", "", counter_index, counter,
                   -NMAXTECALUNMATCHEOBJECTS);

  counter_index++;
}

//********************************************************************
void AnaTECALUnmatchedObjectIO::Write(OutputManager& man,
                                      const std::string& prefix) const {
  //********************************************************************

  (void)prefix;

  man.FillVectorVar(prefix_index + TreeVars::tecalUnmatchedAverageHitTime,
                    AverageHitTime);
  man.FillMatrixVarFromArray(prefix_index + TreeVars::tecalUnmatchedBackPos,
                             BackPos, -1, 3);
  man.FillMatrixVarFromArray(prefix_index + TreeVars::tecalUnmatchedFrontPos,
                             FrontPos, -1, 3);
  man.FillVectorVar(
      prefix_index + TreeVars::tecalUnmatchedMostDownStreamLayerHit,
      MostDownStreamLayerHit);
  man.FillVectorVar(prefix_index + TreeVars::tecalUnmatchedMostUpStreamLayerHit,
                    MostUpStreamLayerHit);
  man.FillVectorVar(prefix_index + TreeVars::tecalUnmatchedNHits, NHits);
  man.FillVectorVar(prefix_index + TreeVars::tecalUnmatchedTotalHitCharge,
                    TotalHitCharge);
  man.FillVectorVar(prefix_index + TreeVars::tecalUnmatchedTrueID, TrueID);
  man.FillVectorVar(prefix_index + TreeVars::tecalUnmatchedTrueIDPrimary,
                    TrueIDPrimary);
  man.FillVectorVar(prefix_index + TreeVars::tecalUnmatchedTrueIDRecursive,
                    TrueIDRecursive);
  man.FillVectorVar(prefix_index + TreeVars::tecalUnmatchedTrueIDSingle,
                    TrueIDSingle);
  man.FillVectorVar(prefix_index + TreeVars::tecalUnmatchedView, View);

  man.FillVectorVar(prefix_index + TreeVars::tecalUnmatchedBunch, Bunch);

  man.IncrementCounterForVar(prefix_index + TreeVars::tecalUnmatchedBunch);
}

//********************************************************************
void AnaP0DReconObjectIO::Define(OutputManager& man, const std::string& counter,
                                 const std::string& prefix) const {
  //********************************************************************

  int p0dPrefix = prefix_index;
  if (prefix == "p0dParticle")
    p0dPrefix += TreeVars::p0dParticleAlgorithmName -
                 TreeVars::p0dVertexAlgorithmName;  // calculate offset to
                                                    // p0dParticle enum

  man.AddMatrixVar(p0dPrefix + TreeVars::p0dClusterTruth_PrimaryTrajIDs,
                   prefix + "Truth_PrimaryTrajIDs", "I", "", counter_index,
                   counter, -NMAXP0DRECONOBJECTS, 500);
  man.AddMatrixVar(p0dPrefix + TreeVars::p0dClusterTruth_TrajIDs,
                   prefix + "Truth_TrajIDs", "I", "", counter_index, counter,
                   -NMAXP0DRECONOBJECTS, 500);
  man.AddMatrixVar(p0dPrefix + TreeVars::p0dClusterTruth_HitCount,
                   prefix + "Truth_HitCount", "I", "", counter_index, counter,
                   -NMAXP0DRECONOBJECTS, 500);
  man.AddMatrixVar(p0dPrefix + TreeVars::p0dClusterTruth_ChargeShare,
                   prefix + "Truth_ChargeShare", "F", "", counter_index,
                   counter, -NMAXP0DRECONOBJECTS, 500);
  man.AddMatrixVar(p0dPrefix + TreeVars::p0dClusterPosition,
                   prefix + "Position", "F", "", counter_index, counter,
                   -NMAXP0DRECONOBJECTS, 4);
  man.AddMatrixVar(p0dPrefix + TreeVars::p0dClusterPosVariance,
                   prefix + "PosVariance", "F", "", counter_index, counter,
                   -NMAXP0DRECONOBJECTS, 4);

  man.AddVectorVar(p0dPrefix + TreeVars::p0dVertexAlgorithmName,
                   prefix + "AlgorithmName", "C", "", counter_index, counter,
                   -NMAXP0DRECONOBJECTS);
  man.AddVectorVar(p0dPrefix + TreeVars::p0dVertexCycle, prefix + "Cycle", "I",
                   "", counter_index, counter, -NMAXP0DRECONOBJECTS);
  man.AddVectorVar(p0dPrefix + TreeVars::p0dVertexNHits, prefix + "NHits", "I",
                   "", counter_index, counter, -NMAXP0DRECONOBJECTS);
  man.AddVectorVar(p0dPrefix + TreeVars::p0dVertexUniqueID, prefix + "UniqueID",
                   "I", "", counter_index, counter, -NMAXP0DRECONOBJECTS);
  man.AddVectorVar(p0dPrefix + TreeVars::p0dVertexBunch, prefix + "Bunch", "I",
                   "", counter_index, counter, -NMAXP0DRECONOBJECTS);

  man.AddVectorVar(p0dPrefix + TreeVars::p0dVertexValidDimensions,
                   prefix + "ValidDimensions", "I", "", counter_index, counter,
                   -NMAXP0DRECONOBJECTS);

  man.AddMatrixVar(p0dPrefix + TreeVars::p0dVertexVertices, prefix + "Vertices",
                   "I", "", counter_index, counter, -NMAXP0DRECONOBJECTS,
                   NMAXP0DRECONOBJECTS);
  man.AddMatrixVar(p0dPrefix + TreeVars::p0dVertexParticles,
                   prefix + "Particles", "I", "", counter_index, counter,
                   -NMAXP0DRECONOBJECTS, NMAXP0DRECONOBJECTS);
  man.AddMatrixVar(p0dPrefix + TreeVars::p0dVertexTracks, prefix + "Tracks",
                   "I", "", counter_index, counter, -NMAXP0DRECONOBJECTS,
                   NMAXP0DRECONOBJECTS);
  man.AddMatrixVar(p0dPrefix + TreeVars::p0dVertexShowers, prefix + "Showers",
                   "I", "", counter_index, counter, -NMAXP0DRECONOBJECTS,
                   NMAXP0DRECONOBJECTS);
  man.AddMatrixVar(p0dPrefix + TreeVars::p0dVertexClusters, prefix + "Clusters",
                   "I", "", counter_index, counter, -NMAXP0DRECONOBJECTS,
                   NMAXP0DRECONOBJECTS);
  man.AddMatrixVar(p0dPrefix + TreeVars::p0dVertexNodes, prefix + "Nodes", "I",
                   "", counter_index, counter, -NMAXP0DRECONOBJECTS, 1000);
  man.AddMatrixVar(p0dPrefix + TreeVars::p0dVertexHits, prefix + "Hits", "I",
                   "", counter_index, counter, -NMAXP0DRECONOBJECTS, 1000);

  counter_index++;
}

//********************************************************************
void AnaP0DReconObjectIO::Write(OutputManager& man,
                                const std::string& prefix) const {
  //********************************************************************

  (void)prefix;

  int p0dPrefix = prefix_index;
  if (prefix == "p0dParticle")
    p0dPrefix += TreeVars::p0dParticleAlgorithmName -
                 TreeVars::p0dVertexAlgorithmName;  // calculate offset to
                                                    // p0dParticle enum

  man.FillVectorVar(p0dPrefix + TreeVars::p0dVertexAlgorithmName,
                    AlgorithmName);
  man.FillVectorVar(p0dPrefix + TreeVars::p0dVertexCycle, (Int_t)Cycle);
  man.FillVectorVar(p0dPrefix + TreeVars::p0dVertexNHits, (Int_t)NHits);
  man.FillVectorVar(p0dPrefix + TreeVars::p0dVertexUniqueID, (Int_t)UniqueID);
  man.FillVectorVar(p0dPrefix + TreeVars::p0dVertexBunch, Bunch);

  unsigned int iObj = 0;
  for (iObj = 0; iObj < Vertices.size(); iObj++)
    man.FillMatrixVar(p0dPrefix + TreeVars::p0dVertexVertices, Vertices[iObj],
                      -1, iObj);
  for (iObj = 0; iObj < Particles.size(); iObj++)
    man.FillMatrixVar(p0dPrefix + TreeVars::p0dVertexParticles, Particles[iObj],
                      -1, iObj);
  for (iObj = 0; iObj < Tracks.size(); iObj++)
    man.FillMatrixVar(p0dPrefix + TreeVars::p0dVertexTracks, Tracks[iObj], -1,
                      iObj);
  for (iObj = 0; iObj < Showers.size(); iObj++)
    man.FillMatrixVar(p0dPrefix + TreeVars::p0dVertexShowers, Showers[iObj], -1,
                      iObj);
  for (iObj = 0; iObj < Clusters.size(); iObj++)
    man.FillMatrixVar(p0dPrefix + TreeVars::p0dVertexClusters, Clusters[iObj],
                      -1, iObj);
  for (iObj = 0; iObj < Nodes.size(); iObj++)
    man.FillMatrixVar(p0dPrefix + TreeVars::p0dVertexNodes, Nodes[iObj], -1,
                      iObj);
  for (iObj = 0; iObj < Hits.size(); iObj++)
    man.FillMatrixVar(p0dPrefix + TreeVars::p0dVertexHits, Hits[iObj], -1,
                      iObj);

  for (iObj = 0; iObj < Truth_PrimaryTrajIDs.size(); iObj++)
    man.FillMatrixVar(prefix_index + TreeVars::p0dClusterTruth_PrimaryTrajIDs,
                      Truth_PrimaryTrajIDs[iObj], -1, iObj);
  for (iObj = 0; iObj < Truth_TrajIDs.size(); iObj++)
    man.FillMatrixVar(prefix_index + TreeVars::p0dClusterTruth_TrajIDs,
                      Truth_TrajIDs[iObj], -1, iObj);
  for (iObj = 0; iObj < Truth_HitCount.size(); iObj++)
    man.FillMatrixVar(prefix_index + TreeVars::p0dClusterTruth_HitCount,
                      Truth_HitCount[iObj], -1, iObj);
  for (iObj = 0; iObj < Truth_ChargeShare.size(); iObj++)
    man.FillMatrixVar(prefix_index + TreeVars::p0dClusterTruth_ChargeShare,
                      Truth_ChargeShare[iObj], -1, iObj);

  man.FillVectorVar(prefix_index + TreeVars::p0dVertexValidDimensions,
                    ValidDimensions);

  man.FillMatrixVarFromArray(prefix_index + TreeVars::p0dClusterPosition,
                             Position, -1, 4);
  man.FillMatrixVarFromArray(prefix_index + TreeVars::p0dClusterPosVariance,
                             PosVariance, -1, 4);

  man.IncrementCounterForVar(p0dPrefix + TreeVars::p0dVertexAlgorithmName);
}

//********************************************************************
void AnaP0DReconVertexIO::Define(OutputManager& man, const std::string& counter,
                                 const std::string& prefix) const {
  //********************************************************************

  man.AddVectorVar(prefix_index + TreeVars::p0dVertexStatus, prefix + "Status",
                   "I", "", counter_index, counter, -NMAXP0DRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::p0dVertexQuality,
                   prefix + "Quality", "F", "", counter_index, counter,
                   -NMAXP0DRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::p0dVertexNDOF, prefix + "NDOF", "I",
                   "", counter_index, counter, -NMAXP0DRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::p0dVertexFiducial,
                   prefix + "Fiducial", "F", "", counter_index, counter,
                   -NMAXP0DRECONOBJECTS);

  //  AnaP0DReconObjectIO::Define(man,counter,prefix);
}

//********************************************************************
void AnaP0DReconVertexIO::Write(OutputManager& man,
                                const std::string& prefix) const {
  //********************************************************************

  (void)prefix;

  man.FillVectorVar(prefix_index + TreeVars::p0dVertexStatus, Status);
  man.FillVectorVar(prefix_index + TreeVars::p0dVertexQuality, Quality);
  man.FillVectorVar(prefix_index + TreeVars::p0dVertexNDOF, NDOF);

  man.FillVectorVar(prefix_index + TreeVars::p0dVertexFiducial, Fiducial);

  //  AnaP0DReconObjectIO::Write(man,prefix);
}

//********************************************************************
void AnaP0DReconParticleIO::Define(OutputManager& man,
                                   const std::string& counter,
                                   const std::string& prefix) const {
  //********************************************************************

  (void)prefix;

  man.AddVectorVar(prefix_index + TreeVars::p0dParticleStatus,
                   prefix + "Status", "I", "", counter_index, counter,
                   -NMAXP0DRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::p0dParticleQuality,
                   prefix + "Quality", "F", "", counter_index, counter,
                   -NMAXP0DRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::p0dParticleNDOF, prefix + "NDOF",
                   "I", "", counter_index, counter, -NMAXP0DRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::p0dParticleSideDeposit,
                   prefix + "SideDeposit", "F", "", counter_index, counter,
                   -NMAXP0DRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::p0dParticleEndDeposit,
                   prefix + "EndDeposit", "F", "", counter_index, counter,
                   -NMAXP0DRECONOBJECTS);
  man.AddMatrixVar(prefix_index + TreeVars::p0dParticleDirection,
                   prefix + "Direction", "F", "", counter_index, counter,
                   -NMAXP0DRECONOBJECTS, 3);
  man.AddMatrixVar(prefix_index + TreeVars::p0dParticleDirVariance,
                   prefix + "DirVariance", "F", "", counter_index, counter,
                   -NMAXP0DRECONOBJECTS, 3);
  man.AddVectorVar(prefix_index + TreeVars::p0dParticleMomentum,
                   prefix + "Momentum", "F", "", counter_index, counter,
                   -NMAXP0DRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::p0dParticleCharge,
                   prefix + "Charge", "F", "", counter_index, counter,
                   -NMAXP0DRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::p0dParticleLength,
                   prefix + "Length", "F", "", counter_index, counter,
                   -NMAXP0DRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::p0dParticleEDeposit,
                   prefix + "EDeposit", "F", "", counter_index, counter,
                   -NMAXP0DRECONOBJECTS);

  //  AnaP0DReconObjectIO::Define(man,counter,prefix);
}

//********************************************************************
void AnaP0DReconParticleIO::Write(OutputManager& man,
                                  const std::string& prefix) const {
  //********************************************************************

  (void)prefix;

  man.FillVectorVar(prefix_index + TreeVars::p0dParticleStatus, Status);
  man.FillVectorVar(prefix_index + TreeVars::p0dParticleQuality, Quality);
  man.FillVectorVar(prefix_index + TreeVars::p0dParticleNDOF, NDOF);
  man.FillVectorVar(prefix_index + TreeVars::p0dParticleSideDeposit,
                    SideDeposit);
  man.FillVectorVar(prefix_index + TreeVars::p0dParticleEndDeposit, EndDeposit);

  man.FillMatrixVarFromArray(prefix_index + TreeVars::p0dParticleDirection,
                             Direction, -1, 3);
  man.FillMatrixVarFromArray(prefix_index + TreeVars::p0dParticleDirVariance,
                             DirVariance, -1, 3);
  man.FillVectorVar(prefix_index + TreeVars::p0dParticleMomentum, Momentum);
  man.FillVectorVar(prefix_index + TreeVars::p0dParticleCharge, Charge);
  man.FillVectorVar(prefix_index + TreeVars::p0dParticleLength, Length);
  man.FillVectorVar(prefix_index + TreeVars::p0dParticleEDeposit, EDeposit);

  //  AnaP0DReconObjectIO::Write(man,prefix);
}

//********************************************************************
void AnaP0DReconClusterIO::Define(OutputManager& man,
                                  const std::string& counter,
                                  const std::string& prefix) const {
  //********************************************************************

  man.AddVectorVar(prefix_index + TreeVars::p0dClusterNFiducialHits,
                   prefix + "NFiducialHits", "I", "", counter_index, counter,
                   -NMAXP0DRECONOBJECTS);
  man.AddVectorVar(prefix_index + TreeVars::p0dClusterEDeposit,
                   prefix + "EDeposit", "F", "", counter_index, counter,
                   -NMAXP0DRECONOBJECTS);

  //  AnaP0DReconObjectIO::Define(man,counter,prefix);
}

//********************************************************************
void AnaP0DReconClusterIO::Write(OutputManager& man,
                                 const std::string& prefix) const {
  //********************************************************************

  (void)prefix;

  man.FillVectorVar(prefix_index + TreeVars::p0dClusterNFiducialHits,
                    NFiducialHits);
  man.FillVectorVar(prefix_index + TreeVars::p0dClusterEDeposit, EDeposit);

  //  AnaP0DReconObjectIO::Write(man,prefix);
}

//********************************************************************
void AnaLocalReconBunchIO::Define(OutputManager& man,
                                  const std::string& prefix) const {
  //********************************************************************

  (void)man;
  (void)prefix;
  //  AnaBunchIO::Define(man,prefix);
}

//********************************************************************
void AnaLocalReconBunchIO::Write(OutputManager& man,
                                 const std::string& prefix) const {
  //********************************************************************

  (void)man;
  (void)prefix;
  //   AnaBunchIO::Write(man,prefix);
}
