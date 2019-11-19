#ifndef CategToolsNueCC_hxx
#define CategToolsNueCC_hxx

#include "BasicUtils.hxx"
#include "CategoriesUtils.hxx"

namespace nue_categs {

  /// Return the code of the "nufamilynue" category this track (and it's vertex) belong in.
  Int_t GetNuFamilyNuE(AnaTrackB* track, const SubDetId::SubDetEnum det, bool AntiNu);

  /// Return the code of the "nuesimple" category.
  Int_t GetNuESimpleCat(AnaTrackB* track, const SubDetId::SubDetEnum det, bool AntiNu);

  /// Return the code of the "nuesimpleparent" category.
  Int_t GetNuESimpleParent(AnaTrackB* track, const SubDetId::SubDetEnum det, bool AntiNu);

  /// Return the code of the "nuesimplereac" category.
  Int_t GetNuESimpleReac(AnaTrackB* track, const SubDetId::SubDetEnum det, bool AntiNu);

  /// Return the code of the "nuesimpletopology" category.
  Int_t GetNuESimpleTopology(AnaTrackB* track, const SubDetId::SubDetEnum det, bool AntiNu);

  /// Return the code of the "nuereactionnofv" category.  
  Int_t GetNuEReactionNoFgdFV(AnaTrackB* track, bool AntiNu);

  /// Return the code of the "nuereaction" category.  
  Int_t GetNuEReaction(AnaTrackB* track, const SubDetId::SubDetEnum det, bool AntiNu);
  	
  /// Add the custom categories for color drawing.
  void AddCategories(const std::string& prefix, bool AntiNu);

  /// Fill the track categories for color drawing.
  void FillCategories(AnaEventB* event, AnaTrackB* track, const std::string& prefix, const SubDetId::SubDetEnum det, bool AntiNu);

  /// Whether this vertex results in a gamma conversion.
  bool IsGammaSignal(AnaTrackB* track, const SubDetId::SubDetEnum det);

  /// Whether this gamma is coming from a neutrino interaction in FGD.
  bool IsGammaSignalOOFGD(AnaTrackB* track, const SubDetId::SubDetEnum det);

  /// Whether this vertex matches the signal definition (a NuE interaction).
  bool IsNuESignal(const AnaTrueVertexB& vtx, const SubDetId::SubDetEnum det, bool AntiNu);
  
  /// nueCC inFGD but out of FV
  bool IsNuECCinFGDOOFV(const AnaTrueVertexB& vtx, const SubDetId::SubDetEnum det, bool AntiNu);
  
  /// Whether this vertex is true CCQE.
  bool IsNuECCQE(const AnaTrueVertexB& vtx, const SubDetId::SubDetEnum det, bool AntiNu);

  /// Whether this vertex is true CCnonQE.
  bool IsNuECCnonQE(const AnaTrueVertexB& vtx, const SubDetId::SubDetEnum det, bool AntiNu);

  /// Whether this vertex is true CC0pi.
  bool IsNuECC0pi(const AnaTrueVertexB& vtx, const SubDetId::SubDetEnum det, bool AntiNu);
  
  /// Whether this vertex is true CC0pi.
  bool IsNuECCother(const AnaTrueVertexB& vtx, const SubDetId::SubDetEnum det, bool AntiNu);

  /// The number of true charge pions
  Int_t NpiC(const AnaTrueVertexB& vtx);

  /// The number of true pi0s
  Int_t Npi0(const AnaTrueVertexB& vtx);

  /// The number of true kaons, rhos and etas
  Int_t NKaonRhoEta(const AnaTrueVertexB& vtx);

  /// Get the code for the frompi0 category.
  Int_t GetPi0Code(AnaEventB* event, AnaTrackB* track);

  bool HasParentOfType(AnaEventB* event, AnaTrueParticleB* track, int pdg);

  /// Gamma sideband categories
  bool ThereWasGammaInFGD(const AnaTrueVertexB& vtx, const SubDetId::SubDetEnum det);
  bool IsNCGammaSignal(const AnaTrueVertexB& vtx, SubDetId::SubDetEnum FGD);
  
  Int_t GetPiTopo(AnaTrackB* track);
  Int_t GetReactionNoFVGamma(AnaTrackB* track);
  
}

#endif
