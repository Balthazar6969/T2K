#ifndef TDisplayedP0DMODULE_HXX
#define TDisplayedP0DMODULE_HXX
#include "TChain.h"
#include "TEveArrow.h"
#include "TEveBoxSet.h"
#include "TND280Track.hxx"
#include "TND280TrackPropagator.hxx"
//#include "ND__TP0DReconModule.h"
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TDisplayedAnalysisModuleBase.hxx"
#include "TGlobalAnalysisUtils.hxx"
#endif
/// Display the results stored in the P0DModule
/// \author Alex Finch (A.Finch@lancaster.ac.uk)
class TDisplayedP0DModule : public TDisplayedAnalysisModuleBase {
 public:
  TDisplayedP0DModule();
  /// Make and Eve representation of all objects in P0DModule in entry'th event.
  void Draw(int entry, int bunch = -2);
  /// Describe a TP0DTrack on standard output.
  void Describe(ND::TP0DReconModule::TP0DTrack* Track);
  /// Describe a TP0DShower on standard output.
  void Describe(ND::TP0DReconModule::TP0DShower* Shower);
  /// Describe a TP0DVertex on standard output.
  void Describe(ND::TP0DReconModule::TP0DVertex* Vertex);
  /// Describe a TP0DParticle on standard output.
  void Describe(ND::TP0DReconModule::TP0DParticle* Particle);
  /// Describe a TP0DHit on standard output.
  void Describe(ND::TP0DReconModule::TP0DHit* RO);
  /// Describe a TP0DNode on standard output.
  void Describe(ND::TP0DReconModule::TP0DNode* RO);
  /// Describe a TP0DCluster on standard output.
  void Describe(ND::TP0DReconModule::TP0DCluster* RO);

 private:
  template <class objectType>
  /// Templated method to draw track object found in Tracks which is contains
  /// Ntracks objects and add the object to gTrackList.
  void DrawTracks(TClonesArray* Tracks, int Ntracks,
                  TND280TrackList* gTrackList);
  template <class objectType>
  /// Templated method to draw tracks in Tracks, indexed by
  /// pointers and add to returned TND280TrackList.
  TEveElement* DrawTracks(TClonesArray* Tracks, std::vector<short> pointers,
                          TString Name);
  // TEveElement* DrawHits(TClonesArray* fHits,std::vector<short> pointers,
  // TString Name);
  /// Templated method to draw shower like object found in showers
  /// and indexex by and add the object to returned TND280TrackList.
  TEveElement* DrawShowers(TClonesArray* showers, std::vector<short> pointers,
                           TString Name);
  /// Templated method to draw showers in `showers' which contains nShowers and
  /// add them to
  /// EveElementList ReconstructedShowers.
  void DrawShowers(TClonesArray* showers, int nShowers,
                   TEveElementList* ReconstructedShowers);
  /// Create a TEveArrow to represent Shower. index used to name it.
  TEveArrow* MakeShower(ND::TP0DReconModule::TP0DShower* Shower, int index);
  /// Create TEveTrack from ND::TP0DReconModule::TP0DTrack.
  TND280Track* MakeTrack(TND280TrackPropagator* trkProp, Int_t index,
                         ND::TP0DReconModule::TP0DTrack* reconObject);
  /// Create TEveTrack from       ND::TP0DReconModule::TP0DParticle*
  /// reconObject);
  TND280Track* MakeTrack(TND280TrackPropagator* trkProp, Int_t index,
                         ND::TP0DReconModule::TP0DParticle* reconObject);
  /// Create TEveTrack from             ND::TP0DReconModule::TP0DVertex*
  /// reconObject

  TND280Track* MakeTrack(TND280TrackPropagator* trkProp, Int_t index,
                         ND::TP0DReconModule::TP0DVertex* reconObject);
  template <class objectType>
  /// Templated method to describe items in list pointed to by pointers.
  void OutputList(std::vector<short> pointers, TClonesArray* list,
                  TString name);

  Int_t fNClusters;
  Int_t fNHits;
  Int_t fNNodes;
  TClonesArray* fClusters;
  TClonesArray* fHits;
  TClonesArray* fNodes;
  int fBunchToDraw;
};
#endif
