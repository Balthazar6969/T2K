#include "TTrackerECALReconModuleTools.hxx"

namespace {
#ifdef TTRACKERECALRECONMODULE_USE_EXP
typedef ND::TTrackerECALReconModule::TECALReconHit TECALReconHit;
#endif
typedef ND::TTrackerECALReconModule::TECALReconVertexCandidate
    TECALReconVertexCandidate;
}

#ifdef TTRACKERECALRECONMODULE_USE_EXP
std::set<TECALReconHit const *>
TTrackerECALReconModuleTools::GetHitInfo(unsigned int UID) {
  if (!fHitInfoArray) {
    std::cerr << "Do not have valid HitInfoArray, please call "
                 "TTrackerECALReconModuleTools::AssignHitInfoTCA for each "
                 "TTrackerECALReconModule entry."
              << std::endl;
  }
  std::set<TECALReconHit const *> ret;

  for (int i = 0; i < fNHitInfo; ++i) {
    const TECALReconHit &trh =
        static_cast<const TECALReconHit &>(*fHitInfoArray->At(i));
    for (int j = 0; j < trh.UsedByNObjs; ++j) {
      if (trh.ObjUIDs[j] == UID) {
        ret.insert(&trh);
      }
    }
  }
  return ret;
}

std::vector<TECALReconHit const *>
TTrackerECALReconModuleTools::GetHitInfoV(unsigned int UID) {
  return TTrackerECALReconModuleUtils::SetAsVect(GetHitInfo(UID));
}
#endif

std::vector<TECALReconVertexCandidate const *>
TTrackerECALReconModuleTools::GetVertexCandidates(unsigned int VUID) {
  if (!fVertexCandyArray) {
    std::cerr << "Do not have valid VertexCandyArray, please call "
                 "TTrackerECALReconModuleTools::AssignVertexCandidateInfoTCA "
                 "for each "
                 "TTrackerECALReconModule entry."
              << std::endl;
  }
  std::vector<TECALReconVertexCandidate const *> ret;
  for (int i = 0; i < fNVertexCandy; ++i) {
    TECALReconVertexCandidate const &candy =
        static_cast<TECALReconVertexCandidate const &>(
            *fVertexCandyArray->At(i));
    if (candy.VertexClusterUID == VUID) {
      ret.push_back(&candy);
    }
  }
  return ret;
}

std::vector<TECALReconVertexCandidate const *>
TTrackerECALReconModuleTools::GetVertexCandidates(
    ND::TTrackerECALReconModule::TECALReconVertexCluster const &vc) {
  return GetVertexCandidates(vc.UniqueID);
}

std::vector<ND::TTrackerECALReconModule::TECALReconVertexTrack const *>
TTrackerECALReconModuleTools::GetVertexCandidateTracks(
    const ND::TTrackerECALReconModule::TECALReconVertexCandidate &candy) {
  std::vector<ND::TTrackerECALReconModule::TECALReconVertexTrack const *> ret;

  for (int i = 0; i < fNVertexCandyTracks; ++i) {
    ND::TTrackerECALReconModule::TECALReconVertexTrack const &track =
        static_cast<ND::TTrackerECALReconModule::TECALReconVertexTrack const &>(
            *fVertexCandyTrackArray->At(i));

    for (int j = 0; j < candy.NTracks; ++j) {
      if (track.UniqueID == candy.TrackUIDs[j]) {
        ret.push_back(&track);
      }
    }
  }
  return ret;
}

#ifdef TTRACKERECALRECONMODULE_USE_EXP
std::set<ND::TTrackerECALReconModule::TECALReconHit const *>
TTrackerECALReconModuleTools::GetHitsUsedByVertexTrack(
    ND::TTrackerECALReconModule::TECALReconVertexTrack const &track) {
  return GetHitInfo(track.UniqueID);
}

std::vector<ND::TTrackerECALReconModule::TECALReconHit const *>
TTrackerECALReconModuleTools::GetHitsUsedByVertexTrackV(
    ND::TTrackerECALReconModule::TECALReconVertexTrack const &track) {
  return TTrackerECALReconModuleUtils::SetAsVect(
      GetHitsUsedByVertexTrack(track));
}

std::set<ND::TTrackerECALReconModule::TECALReconHit const *>
TTrackerECALReconModuleTools::GetHitsUsedByVertexCandidateTracks(
    const ND::TTrackerECALReconModule::TECALReconVertexCandidate &candy,
    ND::TTrackerECALReconModule::TECALReconVertexCluster const &vc) {
  std::set<ND::TTrackerECALReconModule::TECALReconHit const *> ret;
  std::vector<ND::TTrackerECALReconModule::TECALReconVertexTrack const *>
      tracks = GetVertexCandidateTracks(candy);
  for (unsigned int i = 0; i < tracks.size(); ++i) {
    std::set<ND::TTrackerECALReconModule::TECALReconHit const *> hits =
        GetHitsUsedByVertexTrack(*tracks[i]);
    for (std::set<ND::TTrackerECALReconModule::TECALReconHit const
                      *>::const_iterator jit = hits.begin();
         jit != hits.end(); ++jit) {
      ret.insert(*jit);
    }
  }
  return ret;
}

std::vector<ND::TTrackerECALReconModule::TECALReconHit const *>
TTrackerECALReconModuleTools::GetHitsUsedByVertexCandidateTracksV(
    const ND::TTrackerECALReconModule::TECALReconVertexCandidate &candy,
    ND::TTrackerECALReconModule::TECALReconVertexCluster const &vc) {
  return TTrackerECALReconModuleUtils::SetAsVect(
      GetHitsUsedByVertexCandidateTracks(candy, vc));
}

std::set<ND::TTrackerECALReconModule::TECALReconHit const *>
TTrackerECALReconModuleTools::GetHitsUsedByAllVertexCandidateTracks(
    ND::TTrackerECALReconModule::TECALReconVertexCluster const &vc) {
  std::set<ND::TTrackerECALReconModule::TECALReconHit const *> ret;
  std::vector<TECALReconVertexCandidate const *> candy =
      GetVertexCandidates(vc);
  for (unsigned int i = 0; i < candy.size(); ++i) {
    std::set<ND::TTrackerECALReconModule::TECALReconHit const *> hits =
        GetHitsUsedByVertexCandidateTracks(*candy[i], vc);
    for (std::set<ND::TTrackerECALReconModule::TECALReconHit const
                      *>::const_iterator jit = hits.begin();
         jit != hits.end(); ++jit) {
      ret.insert(*jit);
    }
  }
  return ret;
}

std::vector<ND::TTrackerECALReconModule::TECALReconHit const *>
TTrackerECALReconModuleTools::GetHitsUsedByAllVertexCandidateTracksV(
    ND::TTrackerECALReconModule::TECALReconVertexCluster const &vc) {
  return TTrackerECALReconModuleUtils::SetAsVect(
      GetHitsUsedByAllVertexCandidateTracks(vc));
}

std::set<ND::TTrackerECALReconModule::TECALReconHit const *>
TTrackerECALReconModuleTools::GetHitsUsedByVertexClusterTracks(
    ND::TTrackerECALReconModule::TECALReconVertexCluster const &vc) {
  std::set<ND::TTrackerECALReconModule::TECALReconHit const *> ret;
  for (int i = 0; i < vc.NTracks; ++i) {
    ND::TTrackerECALReconModule::TECALReconVertexTrack const &track =
        static_cast<ND::TTrackerECALReconModule::TECALReconVertexTrack const &>(
            *vc.Tracks->At(i));

    std::set<ND::TTrackerECALReconModule::TECALReconHit const *> hits =
        GetHitsUsedByVertexTrack(track);
    for (std::set<ND::TTrackerECALReconModule::TECALReconHit const
                      *>::const_iterator jit = hits.begin();
         jit != hits.end(); ++jit) {
      ret.insert(*jit);
    }
  }
  return ret;
}

std::vector<ND::TTrackerECALReconModule::TECALReconHit const *>
TTrackerECALReconModuleTools::GetHitsUsedByVertexClusterTracksV(
    ND::TTrackerECALReconModule::TECALReconVertexCluster const &vc) {
  return TTrackerECALReconModuleUtils::SetAsVect(
      GetHitsUsedByVertexClusterTracks(vc));
}

namespace {
inline unsigned long TECALHitHasher(TECALReconHit const *hit) {
  return (hit->GeomId * 1000 + (int(hit->Position.T() / 100) % 1000));
}
}

std::set<ND::TTrackerECALReconModule::TECALReconHit const *>
TTrackerECALReconModuleTools::GetHitsNotUsedByVertexClusterTracks(
    ND::TTrackerECALReconModule::TECALReconVertexCluster const &vc) {
  std::set<ND::TTrackerECALReconModule::TECALReconHit const *> trackHits =
      GetHitsUsedByVertexClusterTracks(vc);
  std::set<ND::TTrackerECALReconModule::TECALReconHit const *> clusterHits =
      GetHitInfo(vc.UniqueID);
  std::set<ND::TTrackerECALReconModule::TECALReconHit const *> ret;
  for (std::set<ND::TTrackerECALReconModule::TECALReconHit const
                    *>::const_iterator ijit = clusterHits.begin();
       ijit != clusterHits.end(); ++ijit) {
    bool found = false;
    for (std::set<ND::TTrackerECALReconModule::TECALReconHit const
                      *>::const_iterator jit = trackHits.begin();
         jit != trackHits.end(); ++jit) {
      if (TECALHitHasher(*jit) == TECALHitHasher(*ijit)) {
        found = true;
        break;
      }
    }
    if (!found) {
      ret.insert(*ijit);
    }
  }
  return ret;
}
std::vector<ND::TTrackerECALReconModule::TECALReconHit const *>
TTrackerECALReconModuleTools::GetHitsNotUsedByVertexClusterTracksV(
    ND::TTrackerECALReconModule::TECALReconVertexCluster const &vc) {
  return TTrackerECALReconModuleUtils::SetAsVect(
      GetHitsNotUsedByVertexClusterTracks(vc));
}
#endif

int TTrackerECALReconModuleTools::GetSumVertexTrackNHits(
    ND::TTrackerECALReconModule::TECALReconVertexCluster const &vc) {
  int sum = 0;
  for (int i = 0; i < vc.NTracks; ++i) {
    ND::TTrackerECALReconModule::TECALReconVertexTrack const &track =
        static_cast<ND::TTrackerECALReconModule::TECALReconVertexTrack const &>(
            *vc.Tracks->At(i));
    sum += track.NHits;
  }
  return sum;
}

#ifdef TTRACKERECALRECONMODULE_USE_EXP
void TTrackerECALReconModuleTools::AssignHitInfoTCA(TClonesArray const &hitInfo,
                                                    int NHitInfo) {
  fHitInfoArray = &hitInfo;
  fNHitInfo = NHitInfo;
}
#endif
void TTrackerECALReconModuleTools::AssignVertexCandidateInfoTCA(
    TClonesArray const &vertexCandy, int NVertexCandy) {
  fVertexCandyArray = &vertexCandy;
  fNVertexCandy = NVertexCandy;
}
void TTrackerECALReconModuleTools::AssignVertexCandidateTrackInfoTCA(
    TClonesArray const &vertexCandyTracks, int NVertexCandyTracks) {
  fVertexCandyTrackArray = &vertexCandyTracks;
  fNVertexCandyTracks = NVertexCandyTracks;
}
