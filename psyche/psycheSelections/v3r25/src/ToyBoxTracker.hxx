#ifndef ToyBoxTracker_h
#define ToyBoxTracker_h

#include "ToyBoxND280.hxx"

class ToyBoxTracker:public ToyBoxND280{
public :
  
  ToyBoxTracker();
  virtual ~ToyBoxTracker();

  /// This method should be implemented by the derived class. If so it does nothing here 
  virtual void Reset();

  /// Reset this base class
  virtual void ResetBase();
  
public:

  /// For storing the reconstructed vertex.
  AnaVertexB* Vertex;

  /// For storing the Main Track (The lepton candidate in geranal: HMN or HMP track)
  AnaTrackB* MainTrack;

  /// For storing the highest momentum track.
  AnaTrackB* HMtrack;

  /// For storing the highest momentum negative track.
  AnaTrackB* HMNtrack;

  /// For storing the highest momentum positive track.
  AnaTrackB* HMPtrack;

  /// For storing the second highest momentum track.
  AnaTrackB* SHMtrack;

  /// For storing the second highest momentum negative track.
  AnaTrackB* SHMNtrack;

  /// For storing the second highest momentum positive track.
  AnaTrackB* SHMPtrack;

  /// For storing all negative tracks in the event with TPC information.
  AnaTrackB** NegativeTPCtracks;
  int nNegativeTPCtracks;
  /// For storing all positive tracks in the event with TPC information.
  AnaTrackB** PositiveTPCtracks;
  int nPositiveTPCtracks;

  /// For storing whether TPC PID likelihoods have been computed.
  bool LikeComputed[NMAXPARTICLES];
  int nLikeComputed;

  /// For storing TPC PID likelihoods.
  Float_t Likelihood[NMAXPARTICLES][4];
  int nLikelihood;

  /// For storing the veto track.
  AnaTrackB* VetoTrack;

  /// For storing the oofv fgd tracks
  AnaTrackB* OOFVtrack;
};

#endif
