#ifndef DataClasses_hxx
#define DataClasses_hxx

#include "BaseDataClasses.hxx"
#include "Versioning.hxx"
#include "ParticleId.hxx"

const UInt_t NMAXTIMENODES=10;
class AnaHit;

/// Representation of detector time info
class AnaTimeNode{
public :

  AnaTimeNode();
  virtual ~AnaTimeNode(){}

  /// Clone this object.
  virtual AnaTimeNode* Clone() {
    return new AnaTimeNode(*this);
  }

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaTimeNode(const AnaTimeNode& anaTime);

public:

  /// Detector associated to this times
  /// The systems are numbered as:
  /// 1: TPC1
  /// 2: TPC2
  /// 3: TPC3
  /// 4: FGD1
  /// 5: FGD2
  /// 6: P0D
  /// 7: DSECal
  /// 8: Any SMRD module
  /// 9: Any TECal or PECal module
  Int_t Detector;

  /// Start time
  Float_t TimeStart;

  /// End time
  Float_t TimeEnd;
};

/// Representation of a true Monte Carlo vertex.
class AnaTrueVertex: public AnaTrueVertexB{
public :

  AnaTrueVertex();
  virtual ~AnaTrueVertex(){}

  /// Clone this object.
  virtual AnaTrueVertex* Clone() {
    return new AnaTrueVertex(*this);
  }

  /// Dump the object to screen.
  void Print() const;

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaTrueVertex(const AnaTrueVertex& vertex);

public:

  /// Entry in the RooTrackerVtx tree (not set directly)
  Int_t RooVtxEntry;

  /// The index of the associated RooTrackerVtx vertex from its position in the TClonesArray
  Int_t RooVtxIndex;

  /// The NEUT reaction code. Genie files have their code translated to the
  /// Neut equivalent.
  Int_t ReacCode;

  /// The true (unit) direction of the incoming neutrino.
  Float_t NuDir[3];

  /// The PDG code of the target nucleus.
  Int_t TargetPDG;

  /// Neutrino parent PDG code
  Int_t NuParentPDG;

  /// Decay point of the neutrino parent.
  Float_t NuParentDecPoint[4];

  /// Accumulated cut level for all selections and cut branches. Tell us if a true vertex has been selected or not
  std::vector< std::vector<Int_t> > AccumLevel; //!

  /// The Q2 of the true interaction
  Float_t Q2;

  /// The PDG code of the primary outgoing electron/muon.
  Int_t LeptonPDG;

  /// The momentum of the primary outgoing electron/muon.
  Float_t LeptonMom;

  /// The direction of the primary outgoing electron/muon.
  Float_t LeptonDir[3];

  /// The momentum of the primary outgoing proton (most energetic).
  Float_t ProtonMom;

  /// The direction of the primary outgoing proton (most energetic).
  Float_t ProtonDir[3];

  /// The momentum of the primary outgoing pion (most energetic).
  Float_t PionMom;

  /// The direction of the primary outgoing pion (most energetic).
  Float_t PionDir[3];

  /// Array to count the outgoing primary particles of each type (
  Int_t NPrimaryParticles[Int_t(ParticleId::kLast)+1];

  // Vector to store particles pre-FSI info (only available for NEUT by now)
  // TODO: check how much the file size increases
  // Only store particles escaping the nucleus w/o beeing affected by FSI (other than the lepton, for which FSI in not applied)
//  std::vector<TVector3> PreFSIParticles; // shortcut to store PDG, Momentum and CosTheta (all as Float_t)

  /// The total number of primary baryons that were ejected.
  // OBSOLETE, NOT FILLED, TO BE DELETED, use NPrimaryParticles instead
  Int_t NBaryons;

  /// The momentum of the target nucleus.
  Float_t TargetMom;

  /// The direction of the target nucleus.
  Float_t TargetDir[3];

  /// Vector of pointers to AnaParticleB associated with this true vertex
  /// Do we really need this? It makes stuff more complicated
  std::vector<AnaParticleB*> ReconParticles; //!

  /// Vector of pointers to AnaVertexB (global vertices) associated with this true vertex
  std::vector<AnaVertexB*> ReconVertices; //!

  /// Neut keep also 1pi and single gamma Pauli blocked vertices (unphysical, to be removed)
  /// see https://bugzilla.nd280.org/show_bug.cgi?id=1011
  bool IsPauliBlocked;

  // For a Neut bug, unphysical coherent interactions on hydrogen were produced (in prod 5 and 6)
  // see https://bugzilla.nd280.org/show_bug.cgi?id=1056
  bool IsCohOnH;

};

/// Representation of a true Monte Carlo trajectory/particle.
class AnaTrueParticle: public AnaTrueParticleB{
public :

  AnaTrueParticle();
  virtual ~AnaTrueParticle(){}

  /// Clone this object.
  virtual AnaTrueParticle* Clone() {
    return new AnaTrueParticle(*this);
  }

  /// Dump the object to screen.
  void Print() const;

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaTrueParticle(const AnaTrueParticle& track);

public:

  /// The purity with which this particle was matched to a reconstructed object.
  Float_t Purity;

  /// The bunch in which this true interaction occurred, based on the time of
  /// this true particle Start Position and the bunching used for the reconstructed objects.
  Int_t Bunch;

  /// The index of the AnaTrueVertexB of the interaction that created this AnaTrueParticleB. Doesn't
  /// match to any variable in the AnaTrueVertexB class, but is needed to set up
  /// the Vertex pointer, which should be used to access the vertex that
  /// created this particle.
  Int_t VertexIndex;

  /// Vector of pointers to AnaParticle's associated with this true particle
  std::vector<AnaParticleB*> ReconParticles; //!

};

/// Extension to AnaParticleB to be used by aggregation
class AnaParticleE{
public :

  AnaParticleE();
  virtual ~AnaParticleE();

  /// Clone this object.
  virtual AnaParticleE* Clone() {
    return new AnaParticleE(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaParticleE(const AnaParticleE& track);

public:

  /// The error on the reconstructed momentum.
  Float_t MomentumError;

  /// The number of degrees of freedom when the track was fitted with a Kalman filter.
  Int_t NDOF;

  /// The chi2 value when the track was fitted using a Kalman filter.
  Float_t Chi2;

  /// Momentum from refitting the track assuming the electron hypothesis.
  Float_t MomentumEle;

  /// Error on momentum from refitting the track assuming the electron hypothesis.
  Float_t MomentumErrorEle;

  /// Momentum from refitting the track assuming the muon hypothesis.
  Float_t MomentumMuon;

  /// Error on momentum from refitting the track assuming the muon hypothesis.
  Float_t MomentumErrorMuon;

  /// Momentum from refitting the track assuming the proton hypothesis.
  Float_t MomentumProton;

  /// Error on momentum from refitting the track assuming the proton hypothesis.
  Float_t MomentumErrorProton;

  /// Direction at the start for the main PID hypothesis and reverse sense
  Float_t DirectionStartFlip[3];

#if VERSION_HAS_REVERSED_REFITS

  /// Position at the start for the main PID hypothesis and reverse sense
  Float_t PositionStartFlip[4];

  /// Direction at the end for the main PID hypothesis and reverse sense
  Float_t DirectionEndFlip[3];
#endif


#if VERSION_HAS_PRANGE_ESTIMATES

  /// Momentum by range calculated with electron hypothesis
  Float_t RangeMomentumEle;

  // Muon hypothesis is in AnaParticleEB

  /// Momentum by range calculated with proton hypothesis
  Float_t RangeMomentumProton;

  /// Momentum by range calculated with muon hypothesis and flipped track
  Float_t RangeMomentumMuonFlip;

  /// Momentum by range calculated with proton hypothesis and flipped track
  Float_t RangeMomentumProtonFlip;
#endif

  /// PDG of the most probable particle hypothesis used at reconstruction level
  Int_t ReconPDG;

  /// The bunch of the track, based on the PositionStart.T()
  Int_t Bunch;

  /// Position and charge of 1 or 2 hits at the edges: if the more
  /// upstream/downstream hit does not contain both X and Y info,
  /// a second hit (containing the missing info) is also saved.
  /// (Time and PositionError are also available in oaAnalysis files)
  /// (variables not in FlatTreeConverter yet)
  TVector3 UpstreamHits_Position[2];
  TVector3 DownstreamHits_Position[2];
  Float_t UpstreamHits_Charge[2];
  Float_t DownstreamHits_Charge[2];

  /// The pointer to the most primary AnaVertexB (global vertex) associated with this track
  AnaVertexB* ReconVertex; //!

  /// Vector of pointers to AnaVertexB (global vertices) associated with this track
  std::vector<AnaVertexB*> ReconVertices; //!

  /// The reconstructed momentum of the track, at the most primary global vertex (if exists).
  Float_t MomentumAtVertex;
  //  std::vector<Float_t> MomentumAtVertices;

  /// The reconstructed direction of the track at the most primary global vertex (if exists).
  Float_t DirectionAtVertex[3];
  //  std::vector<TVector3> DirectionAtVertices;

#if VERSION_HAS_TIME_FITS
  /// Time info; the results of "partial" tof fits for a group of nodes in each sub-detector included
  std::vector<AnaTimeNode*> TimeNodes;
#endif
};

/// AnaParticle
class AnaParticle: public AnaParticleB, public AnaParticleE{
public :

  AnaParticle():AnaParticleB(),AnaParticleE(){}
  virtual ~AnaParticle(){}

  /// Clone this object.
  virtual AnaParticle* Clone() {
    return new AnaParticle(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const{}

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaParticle(const AnaParticle& part):AnaParticleB(part),AnaParticleE(part){}
};

class AnaSubTrack{
public :

  AnaSubTrack();
  virtual ~AnaSubTrack(){}

  /// Clone this object.
  virtual AnaSubTrack* Clone() {
    return new AnaSubTrack(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

  //  ClassDef(AnaSubTrack, 1);

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaSubTrack(const AnaSubTrack& seg);

public :

  /// The number of hits in the reconstructed object.
  //  Int_t NHits;

  /// The length of the reconstructed object, accounting for curvature etc.
  Float_t Length;

  /// The direction at the end of the reconstructed track.
  //  Float_t DirectionEnd[3];

  /// The unique ID of the reconstructed object for matchign between global and local reconstruction
  //  Int_t UniqueID;

};

/// Representation of a TPC segment of a global track.
class AnaTPCParticle:public AnaTPCParticleB, public AnaSubTrack{
public :

  AnaTPCParticle();
  virtual ~AnaTPCParticle();

  using AnaTPCParticleB::Print;

  /// Clone this object.
  virtual AnaTPCParticle* Clone() {
    return new AnaTPCParticle(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaTPCParticle(const AnaTPCParticle& seg);

public:

#if VERSION_HAS_BFIELD_REFIT_FULL
  /// Reconstructed charge with the empirical distortion corrections
  Float_t RefitCharge;

  /// Reconstructed direction with the empirical distortion corrections
  Float_t RefitDirection[3];

  /// Reconstructed position with the empirical distortion corrections
  Float_t RefitPosition[4];
#endif

  /// Muon pull of the segment: (dEdxMeas-dEdxexpMuon)/dEdxSigmaMuon.
  Float_t Pullmu;

  /// Electron pull of the segment: (dEdxMeas-dEdxexpEle)/dEdxSigmaEle.
  Float_t Pullele;

  /// Proton pull of the segment: (dEdxMeas-dEdxexpProton)/dEdxSigmaProton.
  Float_t Pullp;

  /// Pion pull of the segment: (dEdxMeas-dEdxexpPion)/dEdxSigmaPion.
  Float_t Pullpi;

  /// Kaon pull of the segment: (dEdxMeas-dEdxexpPion)/dEdxSigmaKaon.
  Float_t Pullk;

  /// Expected dE/dx for a proton, based on the reconstructed momentum.
  Float_t dEdxexpKaon;

  /// Expected error on the dE/dx measurement, for the proton hypothesis.
  Float_t dEdxSigmaKaon;

  /// The purity of the associated true track with respect to this TPC segment.
  /// TrueTrack->Purity gives the purity of the true track with respect to the
  /// global object.
  Float_t Purity;

  //  ClassDef(AnaTPCParticle, 1);
};

class AnaFGDParticle:public AnaFGDParticleB, public AnaSubTrack{
public :

  AnaFGDParticle();
  virtual ~AnaFGDParticle(){};

  /// Clone this object.
  virtual AnaFGDParticle* Clone() {
    return new AnaFGDParticle(*this);
  }

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaFGDParticle(const AnaFGDParticle& seg);

public:

  /// Input to the pull calculations. Needs to be documented properly in oaAnalysis.
  // Float_t X;

  /// Input to the pull calculations. Needs to be documented properly in oaAnalysis.
  Float_t E;

  /// Average Time for the iso FGD hits.
  Float_t AvgTime;

  /// Vertex activity variables
  /// Split this for two cases: entrance(start ) and exit(stop) activity

#if VERSION_HAS_FGD_VERTEX_AND_END_ACTIVITY
  /// Whether any vertex activity variables were calculated (start/entrance point is in FGD)
  bool HasFgdVA;
  /// Failure mode
  Int_t HasFgdVA_fmode;
  /// FGD vertex activity type:  Vertex (1) or Entrance (2)
  Int_t  TypeVA;
#endif

  /// Vertex activity variables
  Float_t      Vertex1x1;
  Float_t      Vertex3x3;
  Float_t      Vertex5x5;
  Float_t      Vertex7x7;
  Float_t      VertexLayer;

#if VERSION_HAS_FGD_VERTEX_AND_END_ACTIVITY
  /// Whether any vertex activity variables were calculated (start/entrance point is in FGD)
  bool HasFgdEA;
  /// Failure mode
  Int_t HasFgdEA_fmode;
  /// FGD vertex activity type:  Vertex (1) or Entrance (2)
  Int_t  TypeEA;

  /// Vertex activity variables
  Float_t      End0x0;
  Float_t      End1x1;
  Float_t      End3x3;
  Float_t      End5x5;
  Float_t      End7x7;

#endif

};

class AnaECALParticle : public AnaECALParticleB {
 public:
  AnaECALParticle();
  virtual ~AnaECALParticle(){};

  /// Clone this object.
  virtual AnaECALParticle* Clone() { return new AnaECALParticle(*this); }

 protected:
  /// Copy constructor is protected, as Clone() should be used to copy this
  /// object.
  AnaECALParticle(const AnaECALParticle& seg);

 public:
  Float_t PIDMipPion;
  Float_t Containment;

#if VERSION_PROD7_DEVEL
  Float_t PIDMIPEM_LowMomentum;
  Float_t AverageZPosition;
  Float_t EMEnergyFit_Uncertainty;
  ///\brief Whether the object was filled as a shower (or a track.). This is
  /// NOT a PID.
  ///
  /// This lets you know how some of these variables were filled from the
  /// original ECal cluster. ECal-iso vertex tracks do not fill many of these
  /// variables. Showers and Tracks differ in their definitions of Width and
  /// Length, and EDeposit.
  /// Cone is filled from the alternate Shower hypothesis for Track-like
  /// clusters.
  /// 2. ECal-iso vertex track
  /// 1. Shower-like
  /// 0. Track-like
  /// -1. Default value
  Int_t FilledAsType;
  Float_t PID_Circularity;
  Float_t PID_Angle;
  Float_t PID_ShowerAngle;
  Float_t PID_Asymmetry;
  Float_t PID_TruncatedMaxRatio;
  Float_t PID_TransverseChargeRatio;
  Float_t PID_FrontBackRatio;

  ///\brief The value of the MIP/HIP discriminator when ignoring a fraction of
  /// track hits closest to the reconstructed vertex candidate
  ///
  /// By default this ignores 20% of hits which will not work well for short
  /// tracks.
  Float_t PIDMipHip_VACut;

  Float_t Thrust;
  Float_t ThrustOrigin[3];
  Float_t ThrustAxis[3];
  ///\brief Zero-indexed layer number of tracker-closest layer with an ECal
  ///hit belonging to this object.
  ///
  /// Currently doubled up with MostUpstreamLayerHit in Psyche... but that name
  /// is so grossly inaccurate this is here so that it can be moved to in the
  /// future.
  Int_t InnerMostLayerHit;
  Int_t OuterMostLayerHit;
  Float_t Width[3];
#endif
};

class AnaSMRDParticle:public AnaSMRDParticleB, public AnaSubTrack{
public :

  AnaSMRDParticle();
  virtual ~AnaSMRDParticle(){};

  /// Clone this object.
  virtual AnaSMRDParticle* Clone() {
    return new AnaSMRDParticle(*this);
  }


  Float_t EDeposit;


protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaSMRDParticle(const AnaSMRDParticle& seg);
};

class AnaTrackerTrack:public AnaTrackerTrackB, public AnaSubTrack{
public :

  AnaTrackerTrack();
  virtual ~AnaTrackerTrack(){};

  /// Clone this object.
  virtual AnaTrackerTrack* Clone() {
    return new AnaTrackerTrack(*this);
  }

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaTrackerTrack(const AnaTrackerTrack& seg);

public:


};

/// Representation of a global track.
class AnaTrack: public AnaTrackB, public AnaParticleE{
public :

  AnaTrack();
  virtual ~AnaTrack();

  /// Clone this object.
  virtual AnaTrack* Clone() {
    return new AnaTrack(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaTrack(const AnaTrack& track);

public:

  /// A different representation of the detectors used by this track.
  ///
  /// The systems are numbered as:
  /// 1: TPC1
  /// 2: TPC2
  /// 3: TPC3
  /// 4: FGD1
  /// 5: FGD2
  /// 6: P0D
  /// 7: DSECal
  /// 8: Any SMRD module
  /// 9: Any TECal or PECal module
  ///
  /// If this field then has a value of "235", it means the track used TPC2, TPC3
  /// and FGD2. You will have to do some mathematical manipulation to extract the
  /// information you want, so it is recommended to just use the DetUsed array
  /// instead.
  Int_t Detectors;


  /// The length of this global track
  Float_t Length;

  /// The TRACKER segments that contributed to this global track.
  std::vector<AnaTrackerTrackB*> TRACKERSegments;

};

class AnaVertex:public AnaVertexB{
public :

  AnaVertex();
  virtual ~AnaVertex(){}

  /// Clone this object.
  virtual AnaVertex* Clone() {
    return new AnaVertex(*this);
  }

  /// Dump the object to screen.
  void Print() const;

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaVertex(const AnaVertex& vertex);

public:

  /// The variance values of the fit using a Kalman filter.
  Float_t Variance[4];

  /// The chi2 value of the fit using a Kalman filter.
  Float_t Chi2;

  /// The number of degrees of freedom of the fit using a Kalman filter.
  Int_t NDOF;
};

/// Representation of the beam information, including POT and quality.
class AnaBeam: public AnaBeamB{
public :

  AnaBeam();
  virtual ~AnaBeam(){}

  /// Clone this object.
  virtual AnaBeam* Clone() {
    return new AnaBeam(*this);
  }

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaBeam(const AnaBeam& beam);

public:

  /// The POT for this spill. For data, this comes from the Beam Summary Data.
  Float_t POT;

#if !VERSION_HAS_OFFICIAL_POT
  /// The POT from CT4 for this spill. This is only needed for P5 files.
  Float_t POTCT4;
#endif

  /// Spill number from BeamSummaryDataModule::ND280Spill
  Int_t Spill;

  /// Spill number from BeamSummaryDataModule::BeamSummaryData::SpillNumber
  Int_t SpillNumber;

  /// Beam run, in form RRSSSS, where RR is main ring run (e.g. 44), and SSSS
  /// is the main ring subrun.
  Int_t BeamRunNumber;
};

/// Representation of the ND280 data quality flags.
class AnaDataQuality: public AnaDataQualityB {
public:

  AnaDataQuality();
  virtual ~AnaDataQuality() {}

  /// Clone this object.
  virtual AnaDataQuality* Clone() {
    return new AnaDataQuality(*this);
  }

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaDataQuality(const AnaDataQuality& dq);

public:

  /// Flag for the ND280 as a whole. The values the flag can take aren't
  /// documented in the oaAnalysis code, but the important thing is that
  /// 0 means good quality.
  Int_t ND280Flag;

  /// Flags for each sub-detector. The values each flag can take aren't
  /// documented in the oaAnalysis code, but the important thing is that
  /// 0 means good quality.
  ///
  /// Indexing is:
  /// [0] = TPC
  /// [1] = FGD
  /// [2] = ECAL
  /// [3] = DSECAL
  /// [4] = P0D
  /// [5] = SMRD
  /// [6] = MAGNET
  Int_t DetFlag[7];
};


class AnaEventInfo:public AnaEventInfoB{
public :

  AnaEventInfo();
  virtual ~AnaEventInfo(){}

  /// Clone this object.
  virtual AnaEventInfo* Clone() {
    return new AnaEventInfo(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaEventInfo(const AnaEventInfo& event);

public:

  /// The ND280 subrun number.
  //  Int_t SubRun;

  /// UNIX timestamp of the event, from the MCM (master clock module).
  Int_t EventTime;
};



/// Representation of the ND280 trigger bits
class AnaTrigger {
public:

  AnaTrigger();
  virtual ~AnaTrigger() {}

  /// Clone this object.
  virtual AnaTrigger* Clone() {
    return new AnaTrigger(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaTrigger(const AnaTrigger& tr);

public:

  /// FGD cosmic trigger flag
  bool FGDCosmic;

  /// TripT cosmic trigger flag
  bool TripTCosmic;


  /// Contains the bit patterns from the cosmic trigger module.
  /// For details see oaEvent/TND280Event
  /// Definitions from oaCosmicTrigger (TTripTTrigger)
  /// Set CTM trigger bits. I believe these should be arranged in the following way:
  ///    word 0, bits  0-31:  SMRD top
  ///    word 0, bits 32-63:  SMRD left (south)
  ///    word 1, bits  0-31:  SMRD bottom
  ///    word 1, bits 32-63:  SMRD right (north)
  ///    word 2, bits  0-31:  p0d
  ///    word 2, bits 32-63:  dsecal
  /// Profound details on CTM trigger: http://www.t2k.org/nd280/elec/CTMdoc
  /// and patterns
  /// Definitions of SMRD patterns:
  /// http://www.t2k.org/nd280/daq/Documentation/expert/HowTos/SMRD-trigmap
  ///
  /// For easier access and storing in psyche (only Float_t, Int_t and Double_t
  /// available) split to have 32 bit patterns
  /// Top, Left, Bottom, Right, P0D, DSECal

  std::vector<UInt_t> CTMTriggerPattern; //! For the sake of mini-trees use vector instead of array

  inline bool CheckCTMTriggerBitSet(unsigned int pattern, unsigned int bit){
    if (pattern > 5) return false;
    if (bit > 31) return false;
    return (CTMTriggerPattern[pattern] & (1<<bit));
  }


};

/// An AnaBunch contains all the reconstructed objects in a given time window.
/// The normal bunches are indexed from 0-7.
///
/// The bunch windows for each run period are defined in the text file
/// $HIGHLEVELANALYSISIOROOT/data/BunchPosition.dat, where the run period of -1
/// means Monte Carlo.
///
/// Each bunch has a weighting associated with it, to allow for flux
/// re-weighting.
class AnaBunch: public AnaBunchB{
public :

  AnaBunch();
  virtual ~AnaBunch();

  /// Clone this object.
  virtual AnaBunch* Clone() {
    return new AnaBunch(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaBunch(const AnaBunch& bunch);

public:
};

/// The top-level object for representing information from the input files.
/// A single AnaSpill contains information from an entire ND280 event.
/// The reconstructed objects are separated into bunches based on their time
/// information.
///
/// There are multiple copies of each spill available through the InputManager
/// [ND::input()], showing the raw state of the spill after reading it in from
/// the input files; after applying corrections; and after applying corrections
/// and systematics.
class AnaSpill: public AnaSpillB{
public :

  AnaSpill();
  virtual ~AnaSpill();

  /// Clone this object.
  virtual AnaSpill* Clone() {
    AnaSpill* spill = new AnaSpill(*this);
    spill->RedoLinks();
    spill->isClone=true;
    return spill;
  }

  /// Dump the object to screen.
  virtual void Print() const;

  /// Copy the arrays into std vectors, needed for FlatTree creation
  virtual void CopyArraysIntoVectors();

  /// Copy the vectore into the arrays, needed after FlatTree reading
  virtual void CopyVectorsIntoArrays();

  /// Redo reco-reco and reco-truth after cloning or reading MiniTree
  virtual void RedoLinks();

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaSpill(const AnaSpill& spill);

  /// Add to a particle a given associated vertex (not in the base class)
  virtual void associateVertexToParticle(AnaParticleB*, AnaVertexB*) const;

  /// Add to a true vertex a given associated reconstructed vertex (not in the base class)
  virtual void associateVertexToTrueVertex(AnaVertexB*) const;

  /// Add to a true particle a given associated reconstructed particle (not in the base class)
  virtual void associateParticleToTrueParticle(AnaParticleB*) const;

public:

  /// Index of the input file producing this spill
  Int_t InputFileIndex; //! Not in the MiniTree for the Moment since it produces a seg fault

  /// The entry of the RooVtxTracker tree producing this spill
  Int_t RooVtxEntry; //! Not in the MiniTree for the Moment since it produces a seg fault

  /// The trigger flags for this spill.
  AnaTrigger Trigger;
};


class AnaEvent: public AnaEventB{
public :

  AnaEvent();
  AnaEvent(const AnaSpill& spill, const AnaBunch& bunch);
  virtual ~AnaEvent();

  /// Clone this object.
  virtual AnaEvent* Clone() {
    AnaEvent* spill = new AnaEvent(*this);
    spill->isClone=true;
    return spill;
  }

  /// Dump the object to screen.
  virtual void Print() const;

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaEvent(const AnaEvent& event);

public:

  /// The trigger flags for this event.
  AnaTrigger Trigger;
};


class AnaFgdTimeBin: public AnaFgdTimeBinB{
public :

  AnaFgdTimeBin();
  virtual ~AnaFgdTimeBin();

  /// Clone this object.
  virtual AnaFgdTimeBin* Clone() {
    return new AnaFgdTimeBin(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

  /// Utility method to fill the vector needed for MiniTree storage
  void CopyArraysIntoVectors();

  /// Utility method to fill the arrays from vector, needed for MiniTree reading
  void CopyVectorsIntoArrays();


protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaFgdTimeBin(const AnaFgdTimeBin& timein);

public:

  Float_t      MaxTime;     //
  Int_t        G4ID; //

  /// A collection of associated hits
  AnaHit** FgdHits; //!
  Int_t nFgdHits; //!

  /// A vector to be used in mini-trees
  std::vector<AnaHit*> FgdHitsVect;
};


class AnaRecTrueMatch: public AnaRecTrueMatchB{
public :

  AnaRecTrueMatch():AnaRecTrueMatchB(){}
  virtual ~AnaRecTrueMatch(){}

};


/// A class to collect basic information for a hit
class AnaHit{

public:
  AnaHit();
  virtual ~AnaHit();

  /// Clone this object
  virtual AnaHit* Clone(){
    return new AnaHit(*this);
  }

  /// Dump the object to the screen
  virtual void Print() const;

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaHit(const AnaHit& hit);

public:
  /// Type of the hit, may be should be done general since can be used for reco
  /// objects as well
  enum TypeEnum{
    kXY = 0,
    kXZ,
    kYZ,
    kUnknown
  };

  /// Position
  Float_t Position[4];

  /// Type of the hit
  Int_t Type;

  /// Detector bits of the detector the hit belongs to
  unsigned long Detector;

  /// Layer of the hit
  Int_t Layer;

  /// Charge of the hit
  Float_t Charge;

  /// Reconstructed objects that this hit contribute to
  AnaParticleB** Particles; //!
  /// How many reconstructed objects that this hit contribute to
  Int_t nParticles; //!

  /// True particles that this hit contribute to
  AnaTrueParticleB** TrueParticles; //!

  /// How many reconstructed objects that this hit contribute to
  Int_t nTrueParticles; //!

  /// May be useful for subsequent true association
  std::vector<Int_t> TrueG4IDs;

  /// This is for storing in mini-tree
  std::vector<AnaParticleB*>     ParticlesVect;
  std::vector<AnaTrueParticleB*> TrueParticlesVect;

  /// Utility method to fill the vector needed for MiniTree storage
  void CopyArraysIntoVectors();

  /// Utility method to fill the arrays from vector, needed for MiniTree reading
  void CopyVectorsIntoArrays();

};





const UInt_t NMAXTECALRECONOBJECTS   =20; // Arbitrarily define a maximum number of reconstructed objects to flatten from ReconDir/TrackerECal and assign it to an unbound global variable.
const UInt_t NMAXTECALUNMATCHEOBJECTS=20; // Do the same for unmatched objects. Other such instances can be found in psycheCore BasicDataClasses.hxx.

/// This Ana* object is used to flatten TECALReconObjects from ReconDir/TrackerECal
class AnaTECALReconObject {

public :

  AnaTECALReconObject();
  virtual ~AnaTECALReconObject(){}

  /// Clone this object.
  virtual AnaTECALReconObject* Clone() {
    return new AnaTECALReconObject(*this);
  }

  /// Dump the object to screen.
  void Print() const;

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaTECALReconObject(const AnaTECALReconObject& );

public :

  // Variables copied directly from TECALReconObject
  Float_t  AverageHitTime;           // AverageHitTime
  Float_t  AverageZPos;              // AverageZPosition
  Float_t  Containment;              // Containment
  Float_t  EFitResult;               // EMEnergyFit_Result
  Float_t  EFitUncertainty;          // EMEnergyFit_Uncertainty
  Int_t    FirstLayer;               // FirstLayer;
  bool     IsShowerLike;             // IsShowerLike
  bool     IsTrackLike;              // IsTrackLike
  Int_t    LastLayer;                // LastLayer;
  Float_t  LikeMIPEM;                // PID_LLR_MIP_EM
  Float_t  LikeMIPPion;              // PID_LLR_MIP_Pion
  Float_t  LikeEMHIP;                // PID_LLR_EM_HIP
  Float_t  LikeMIPEMLow;             // PID_LLR_MIP_EM_LowMomentum
  Float_t  MatchingLike;             // MatchingLikelihood
  Int_t    MichelTagNDelayedCluster; // MElectronTag_NDelayedCluster
  std::string   Module;                   // Module
  Int_t    MostDownStreamLayerHit;   // mostDownStreamLayerHit
  Int_t    MostUpStreamLayerHit;     // mostUpStreamLayerHit
  Int_t    NHits;                    // NHits
  Int_t    NLayersHit;               // NLayersHit
  Float_t  ObjectLength;             // ObjectLength
  Float_t  PIDAMR;                   // PID_AMR
  Float_t  PIDAngle;                 // PID_Angle
  Float_t  PIDAsymmetry;             // PID_Asymmetry
  Float_t  PIDCircularity;           // PID_Circularity
  Float_t  PIDFBR;                   // PID_FrontBackRatio
  Float_t  PIDMaxRatio;              // PID_Max_Ratio
  Float_t  PIDMeanPos;               // PID_MeanPosition
  Float_t  PIDShowerAngle;           // PID_ShowerAngle
  Float_t  PIDShowerWidth;           // PID_ShowerWidth
  Float_t  PIDTransverseChargeRatio; // PID_TransverseChargeRatio
  Float_t  PIDTruncatedMaxRatio;     // PID_TruncatedMaxRatio
  Float_t  Pointing[3];              // Pointing
  Float_t  Thrust;                   // Thrust
  Float_t  ThrustAxis[3];            // ThrustAxis
  Float_t  ThrustOrigin[3];          // ThrustOrigin
  Int_t    TimeBunch;                // TimeBunch
  Float_t  TotalHitCharge;           // TotalHitCharge
  Int_t    TrueID;                   // G4ID
  Int_t    TrueIDPrimary;            // G4ID_Primary
  Int_t    TrueIDRecursive;          // G4ID_Recursive
  Int_t    TrueIDSingle;             // G4ID_Single
  Int_t    UniqueID;                 // UniqueID

  // Additional info
  Int_t  Bunch;

};

/// This Ana* object is used to flatten TECALUnmatchedObjects from ReconDir/TrackerECal
class AnaTECALUnmatchedObject{

public :

  AnaTECALUnmatchedObject();
  virtual ~AnaTECALUnmatchedObject(){}

  /// Clone this object.
  virtual AnaTECALUnmatchedObject* Clone() {
    return new AnaTECALUnmatchedObject(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaTECALUnmatchedObject(const AnaTECALUnmatchedObject& );

public :

  // Variables copied directly from TECALUnmatchedObject
  Float_t  AverageHitTime;           // AverageHitTime
  Float_t  BackPos [3];              // BackPosition
  Float_t  FrontPos[3];              // FrontPosition
  Int_t    MostDownStreamLayerHit;   // mostDownStreamLayerHit
  Int_t    MostUpStreamLayerHit;     // mostUpStreamLayerHit
  Int_t    NHits;                    // NHits
  Float_t  TotalHitCharge;           // TotalHitCharge
  Int_t    TrueID;                   // G4ID
  Int_t    TrueIDPrimary;            // G4ID_Primary
  Int_t    TrueIDRecursive;          // G4ID_Recursive
  Int_t    TrueIDSingle;             // G4ID_Single
  Int_t    View;                     // View

  // Additional info
  Int_t  Bunch;

};


/// This Ana* object is used to flatten PECALReconObjects from ReconDir/P0DECal
class AnaPECALReconObject {

public :

  AnaPECALReconObject(){}
  virtual ~AnaPECALReconObject(){}

  /// Clone this object.
  virtual AnaPECALReconObject* Clone() {
    return new AnaPECALReconObject(*this);
  }

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaPECALReconObject(const AnaTECALReconObject& ){}
};

class AnaPECALUnmatchedObject{

public :

  AnaPECALUnmatchedObject(){}
  virtual ~AnaPECALUnmatchedObject(){}

  /// Clone this object.
  virtual AnaPECALUnmatchedObject* Clone() {
    return new AnaPECALUnmatchedObject(*this);
  }

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaPECALUnmatchedObject(const AnaTECALReconObject& ){}
};


const UInt_t NMAXP0DRECONOBJECTS = 100;

class AnaP0DReconCluster;
class AnaP0DReconVertex;
class AnaP0DReconParticle;

/// This is a base P0D object that shares the common attributes of Vertex, Particle, Track, Shower, Cluster, Hit
class AnaP0DReconObject{

public:

  AnaP0DReconObject();
  virtual ~AnaP0DReconObject(){}

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaP0DReconObject(const AnaP0DReconObject& );

public:
  /// Dump the object to screen.
  virtual void Print() const;

  std::vector<AnaP0DReconVertex*>     VerticesP;
  std::vector<AnaP0DReconParticle*>   ParticlesP;
  std::vector<AnaP0DReconCluster*>    ClustersP;


  std::vector<Short_t>   Vertices;
  std::vector<Short_t>   Particles;
  std::vector<Short_t>   Tracks;
  std::vector<Short_t>   Showers;
  std::vector<Short_t>   Clusters;
  std::vector<Short_t>   Nodes;
  std::vector<Short_t>   Hits;

  std::vector<Int_t>   Truth_PrimaryTrajIDs;
  std::vector<Int_t>   Truth_TrajIDs;
  std::vector<Short_t> Truth_HitCount;
  std::vector<Float_t> Truth_ChargeShare;
  Float_t              Position   [4];
  Float_t              PosVariance[4];   // ? For the vertex only and probably for cluster
  Short_t              ValidDimensions;

  std::string       AlgorithmName;    // --> enum
  Short_t           Cycle;
  Short_t           NHits;
  unsigned int      UniqueID;

  Int_t             Bunch;
};

/// P0DRecon Vertex
class AnaP0DReconVertex : public AnaP0DReconObject {

public:

  AnaP0DReconVertex();
  virtual ~AnaP0DReconVertex(){}

  /// Clone this object.
  virtual AnaP0DReconVertex* Clone() {
    return new AnaP0DReconVertex(*this);
  }

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaP0DReconVertex(const AnaP0DReconVertex& );

public:

  Int_t                Status;
  Float_t              Quality;       // ?
  Int_t                NDOF;          // ?
  Float_t              Fiducial;

};

/// P0DRecon Particle
class AnaP0DReconParticle : public AnaP0DReconObject {

public:

  AnaP0DReconParticle();
  virtual ~AnaP0DReconParticle(){}

  /// Clone this object.
  virtual AnaP0DReconParticle* Clone() {
    return new AnaP0DReconParticle(*this);
  }

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaP0DReconParticle(const AnaP0DReconParticle& );

public:

  Int_t                Status;
  Float_t              Quality;                 // ?
  Int_t                NDOF;                    // ?
  Float_t              SideDeposit;
  Float_t              EndDeposit;
  Float_t              Direction  [3];
  Float_t              DirVariance[3];   //
  Float_t              Momentum;   //
  Float_t              Charge;   //
  Float_t              Length;
  Float_t              EDeposit;

  std::vector<std::string>           realPIDNames;
  std::vector<std::vector<Float_t> > realPIDValues;
  std::vector<std::string>           integerPIDNames;
  std::vector<std::vector<Short_t> > integerPIDValues;


  std::vector<Short_t>               PID;
  std::vector<Float_t>               PID_weight;

  AnaTrueParticleB* TrueParticle;

};


//P0DReconCluster

class AnaP0DReconCluster : public AnaP0DReconObject {

public:

  AnaP0DReconCluster();
  virtual ~AnaP0DReconCluster(){}

  /// Clone this object.
  virtual AnaP0DReconCluster* Clone() {
    return new AnaP0DReconCluster(*this);
  }

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaP0DReconCluster(const AnaP0DReconCluster& );

public:

  Short_t              NFiducialHits;    // Clark has to tell me what it means
  Float_t              EDeposit;

};

class AnaFGDOnlyReconObject{

public :

  AnaFGDOnlyReconObject(){}
  virtual ~AnaFGDOnlyReconObject(){}

  /// Clone this object.
  virtual AnaFGDOnlyReconObject* Clone() {
    return new AnaFGDOnlyReconObject(*this);
  }

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaFGDOnlyReconObject(const AnaTECALReconObject& ){}
};


// When saving information from the local reconstruction, the reconstructed objects
// must be added to the bunch.
// A derived bunch object is defined here to encapsulate these additional data.
// Since the choice of which data to flatten is made at run time, this object requires
// place holders for all locally reconstructed objects potentially added to the flat tree
class AnaLocalReconBunch: public AnaBunch{
public :

  AnaLocalReconBunch();
  virtual ~AnaLocalReconBunch();

  /// Clone this object.
  virtual AnaLocalReconBunch* Clone() {
    return new AnaLocalReconBunch(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

  /// Returns true if the bunch is completely empty
  virtual bool IsEmpty() const;

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaLocalReconBunch(const AnaLocalReconBunch& localBunch);

public:

  std::vector<AnaTECALReconObject*    > TECALReconObjects;
  std::vector<AnaTECALUnmatchedObject*> TECALUnmatchedObjects;
  std::vector<AnaPECALReconObject*    > PECALReconObjects;
  std::vector<AnaPECALUnmatchedObject*> PECALUnmatchedObjects;
  std::vector<AnaP0DReconVertex*      > P0DReconVertices;
  std::vector<AnaP0DReconParticle*    > P0DReconParticles;
  std::vector<AnaP0DReconCluster*     > P0DReconClusters;
  std::vector<AnaFGDOnlyReconObject*  > FGDOnlyReconObjects;
};

class AnaLocalReconEvent: public AnaEvent{
public :

  AnaLocalReconEvent();
  AnaLocalReconEvent(const AnaSpill& spill, const AnaLocalReconBunch& bunch);
  virtual ~AnaLocalReconEvent();

  /// Clone this object.
  virtual AnaLocalReconEvent* Clone() {
    AnaLocalReconEvent* event = new AnaLocalReconEvent(*this);
    event->isClone=true;
    return event;
  }

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaLocalReconEvent(const AnaLocalReconEvent& event);

public:

  std::vector<AnaTECALReconObject*    > TECALReconObjects;
  std::vector<AnaTECALUnmatchedObject*> TECALUnmatchedObjects;
  std::vector<AnaPECALReconObject*    > PECALReconObjects;
  std::vector<AnaPECALUnmatchedObject*> PECALUnmatchedObjects;
  std::vector<AnaP0DReconVertex*      > P0DReconVertices;
  std::vector<AnaP0DReconParticle*    > P0DReconParticles;
  std::vector<AnaP0DReconCluster*     > P0DReconClusters;
  std::vector<AnaFGDOnlyReconObject*  > FGDOnlyReconObjects;

};

#endif
