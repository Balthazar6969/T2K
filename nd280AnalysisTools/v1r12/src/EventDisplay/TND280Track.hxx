#ifndef TND280Track_HXX
#define TND280Track_HXX
#include "TEveTrack.h"
#include "TND280TrackPropagator.hxx"
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TTrackerAnalysisUtils.hxx"
#endif
//#define ADDDESCRIBE
/// An ND280  track displayed in Eve.
/// Class to add additional functions to an EveTrack.
///\author Alex Finch (A.Finch@lancaster.ac.uk)
class TND280Track : public TEveTrack {

private:
  TND280TrackPropagator *fND280Propagator;

public:
  /// Default constructor.
  TND280Track() : TEveTrack(){};
  /// Constructor from TParticle.
  TND280Track(TParticle *t, Int_t label, TND280TrackPropagator *prop = 0)
      : TEveTrack(t, label, prop), fND280Propagator(prop){};
  /// Constructor from TEveUtil Monte Carlo track.
  TND280Track(TEveMCTrack *t, TND280TrackPropagator *prop = 0)
      : TEveTrack(t, prop), fND280Propagator(prop){};
  /// Constructor from TEveRecTrack<double> reconstructed track.
  TND280Track(TEveRecTrackD *t, TND280TrackPropagator *prop = 0)
      : TEveTrack(t, prop), fND280Propagator(prop){};
  /// Constructor from TEveRecTrack<float> reconstructed track.
  /// It is recomended to use constructor with  TEveRecTrack<double> since
  /// TND280TrackPropagator operates with double type.
  TND280Track(TEveRecTrack *t, TND280TrackPropagator *prop = 0)
      : TEveTrack(t, prop), fND280Propagator(prop){};
  /// Copy constructor. Track paremeters are copied but the
  /// extrapolation is not perfermed so you should still call
  /// MakeTrack() to do that.
  /// If points of 't' are locked, they are cloned.
  TND280Track(const TND280Track &t) : TEveTrack(t) {
    this->fND280Propagator = t.fND280Propagator;
  };
  /// When this item is selected, select all its daughters.
  void FillImpliedSelectedSet(Set_t &impSelSet);
  /// Calculate track representation based on track data and current
  /// settings of the propagator.
  /// If recurse is true, descend into children. Overrides one from
  /// TEveTrack because it sometimes missed the end vertex and kept going
  /// to the edge of the detector. Have to use an ND280TrackPropogator to avoid
  /// this
  ///
  void MakeTrack(Bool_t recurse = kTRUE);
#ifdef ADDDESCRIBE
  /// Describe this object when selected from context menu.
  void Describe(); // *MENU*
#endif
private:
  ClassDef(TND280Track, 0)
};
#endif
