#ifndef TTRACKERECALRECONMODULETOOLS_HXX_SEEN
#define TTRACKERECALRECONMODULETOOLS_HXX_SEEN

#include <set>
#include <vector>

#include "TClonesArray.h"

#include "libReadoaAnalysis/libReadoaAnalysisProjectHeaders.h"

namespace TTrackerECALReconModuleUtils {
///
template <class T> std::vector<T> SetAsVect(std::set<T> const &setto) {
  std::vector<T> ret;
  ret.reserve(setto.size());
  for (typename std::set<T>::const_iterator sit = setto.begin();
       sit != setto.end(); ++sit) {
    ret.push_back(*sit);
  }
  return ret;
}

#ifdef TTRACKERECALRECONMODULE_USE_EXP
typedef std::set<ND::TTrackerECALReconModule::TECALReconHit const *>::iterator
    ECalHitSetIt;

typedef std::vector<
    ND::TTrackerECALReconModule::TECALReconHit const *>::iterator ECalHitVectIt;
#endif

typedef std::vector<ND::TTrackerECALReconModule::TECALReconVertexCandidate const
                        *>::iterator ECalVCVectIt;

typedef std::vector<ND::TTrackerECALReconModule::TECALReconVertexTrack const
                        *>::iterator ECalVTVectIt;
}

///\brief Utility class to get TECALReconVertexCluster/Track associated
/// HitInfo/VertexCandidates
///
///\details \note For each entry in the TTrackerECALReconModule tree one, or
/// both of AssignHitInfoTCA, AssignVertexCandidateInfoTCA need to be called.
class TTrackerECALReconModuleTools {
#ifdef TTRACKERECALRECONMODULE_USE_EXP
  /// The number of hit info clones in fHitInfoArray.
  int fNHitInfo;
  /// The hit infos for this tree entry.
  TClonesArray const *fHitInfoArray;
#endif
  /// The number of vertex candidate clones in fVertexCandyArray.
  int fNVertexCandy;
  /// The vertex candidates for this tree entry.
  TClonesArray const *fVertexCandyArray;

  int fNVertexCandyTracks;
  TClonesArray const *fVertexCandyTrackArray;

public:
  TTrackerECALReconModuleTools() {
#ifdef TTRACKERECALRECONMODULE_USE_EXP
    fHitInfoArray = 0;
#endif
    fVertexCandyArray = 0;
    fVertexCandyTrackArray = 0;

#ifdef TTRACKERECALRECONMODULE_USE_EXP
    fNHitInfo = 0;
#endif
    fNVertexCandy = 0;
    fNVertexCandyTracks = 0;
  }

#ifdef TTRACKERECALRECONMODULE_USE_EXP
  /// Assign current event's TClonesArray of HitInfo
  void AssignHitInfoTCA(TClonesArray const &hitInfo, int NHitInfo);
#endif
  /// Assign current event's TClonesArray of VertexCandidates
  void AssignVertexCandidateInfoTCA(TClonesArray const &vertexCandy,
                                    int NVertexCandy);
  void AssignVertexCandidateTrackInfoTCA(TClonesArray const &vertexCandyTracks,
                                         int NVertexCandyTracks);

#ifdef TTRACKERECALRECONMODULE_USE_EXP
  /// Gets the set of TECALReconHits which are associated with this UniqueId.
  std::set<ND::TTrackerECALReconModule::TECALReconHit const *>
  GetHitInfo(unsigned int UID);

  /// Gets the set of TECALReconHits which are associated with this UniqueId.
  std::vector<ND::TTrackerECALReconModule::TECALReconHit const *>
  GetHitInfoV(unsigned int UID);
#endif

  ///\brief Gets the TECALReconVertexCandidates that are associated
  /// with this vertex cluster UniqueId.
  std::vector<ND::TTrackerECALReconModule::TECALReconVertexCandidate const *>
  GetVertexCandidates(unsigned int VUID);

  ///\brief Gets the TECALReconVertexCandidates that are associated
  /// with this vertex cluster.
  std::vector<ND::TTrackerECALReconModule::TECALReconVertexCandidate const *>
  GetVertexCandidates(
      ND::TTrackerECALReconModule::TECALReconVertexCluster const &);

  ///\brief Gets the TECALReconVertexTracks that are associated with this vertex
  /// Candidate.
  std::vector<ND::TTrackerECALReconModule::TECALReconVertexTrack const *>
  GetVertexCandidateTracks(
      ND::TTrackerECALReconModule::TECALReconVertexCandidate const &);

#ifdef TTRACKERECALRECONMODULE_USE_EXP
  /// Gets all the hits that are used by a TReconVertexTrack.
  /// Wraps GetHitInfo for convinience---and extra call instructions!
  std::set<ND::TTrackerECALReconModule::TECALReconHit const *>
  GetHitsUsedByVertexTrack(
      ND::TTrackerECALReconModule::TECALReconVertexTrack const &);

  /// Gets all the hits that are used by a TReconVertexTrack.
  /// Wraps GetHitInfo for convinience---and extra call instructions!
  std::vector<ND::TTrackerECALReconModule::TECALReconHit const *>
  GetHitsUsedByVertexTrackV(
      ND::TTrackerECALReconModule::TECALReconVertexTrack const &);

  ///\brief Gets all the hits that are used by the TReconVertexTracks associated
  /// with this TECALReconVertexCandidate.
  std::set<ND::TTrackerECALReconModule::TECALReconHit const *>
  GetHitsUsedByVertexCandidateTracks(
      ND::TTrackerECALReconModule::TECALReconVertexCandidate const &,
      ND::TTrackerECALReconModule::TECALReconVertexCluster const &);

  ///\brief Gets all the hits that are used by the TReconVertexTracks associated
  /// with this TECALReconVertexCandidate.
  std::vector<ND::TTrackerECALReconModule::TECALReconHit const *>
  GetHitsUsedByVertexCandidateTracksV(
      ND::TTrackerECALReconModule::TECALReconVertexCandidate const &,
      ND::TTrackerECALReconModule::TECALReconVertexCluster const &);

  ///\brief Gets all the hits that are used by the TReconVertexTracks associated
  /// with all the TECALReconVertexCandidates associated with this
  /// TECALReconVertexCluster.
  ///\details When you find it useful, then you can thank me.
  std::set<ND::TTrackerECALReconModule::TECALReconHit const *>
  GetHitsUsedByAllVertexCandidateTracks(
      ND::TTrackerECALReconModule::TECALReconVertexCluster const &);

  ///\brief Gets all the hits that are used by the TReconVertexTracks associated
  /// with all the TECALReconVertexCandidates associated with this
  /// TECALReconVertexCluster.
  ///\details When you find it useful, then you can thank me.
  std::vector<ND::TTrackerECALReconModule::TECALReconHit const *>
  GetHitsUsedByAllVertexCandidateTracksV(
      ND::TTrackerECALReconModule::TECALReconVertexCluster const &);

  ///\brief Gets all the hits that are used by the TReconVertexTracks associated
  /// with this TECALReconVertexCluster.
  std::set<ND::TTrackerECALReconModule::TECALReconHit const *>
  GetHitsUsedByVertexClusterTracks(
      ND::TTrackerECALReconModule::TECALReconVertexCluster const &);

  ///\brief Gets all the hits that are used by the TReconVertexTracks associated
  /// with this TECALReconVertexCluster.
  std::vector<ND::TTrackerECALReconModule::TECALReconHit const *>
  GetHitsUsedByVertexClusterTracksV(
      ND::TTrackerECALReconModule::TECALReconVertexCluster const &);

  ///\brief Gets all the hits that are used by this TECALReconVertexCluster but
  /// not by the TReconVertexTracks associated with this it.
  std::set<ND::TTrackerECALReconModule::TECALReconHit const *>
  GetHitsNotUsedByVertexClusterTracks(
      ND::TTrackerECALReconModule::TECALReconVertexCluster const &);

  ///\brief Gets all the hits that are used by this TECALReconVertexCluster but
  /// not by the TReconVertexTracks associated with this it.
  std::vector<ND::TTrackerECALReconModule::TECALReconHit const *>
  GetHitsNotUsedByVertexClusterTracksV(
      ND::TTrackerECALReconModule::TECALReconVertexCluster const &);
#endif

  /// Get the number of hits which appear in tracks
  int GetSumVertexTrackNHits(
      ND::TTrackerECALReconModule::TECALReconVertexCluster const &);
};

#endif
