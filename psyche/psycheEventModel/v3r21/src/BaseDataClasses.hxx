#ifndef BaseDataClasses_hxx
#define BaseDataClasses_hxx

#include <sstream>
#include <fstream>
#include <stdio.h>
#include <iostream>

#include "TVector3.h"
#include "TLorentzVector.h"


#include "CoreDataClasses.hxx"
#include "SampleId.hxx"
#include "SubDetId.hxx"

/// Classes for high level analysis. All input formats (oaAnalysis, flatTree, etc)
/// are converted into these data classes, which are later used in the actual analysis.
///

///Array printing

#define dump_array4(par) std::cout<< #par << ": \t" << par[0] << " " << par[1] << " " << par[2] << " " << par[3] << std::endl;  
#define dump_array3(par) std::cout<< #par << ": \t" << par[0] << " " << par[1] << " " << par[2] << std::endl; 

#define dump_array(par, n) \
  std::cout<< #par << " *** start *** \n "; \
  for (int i = 0; i < n; i++) std::cout << " \t " << par[i] << " \n "; \
  std::cout << #par << " *** end *** " << std::endl; 

#define dump_vector(par, n) \
  std::cout<< #par << " *** start *** \n "; \
  for (int i = 0; i < n; i++) std::cout << " \t " << par[i] << " \n "; \
  std::cout << #par << " *** end *** " << std::endl; 



/// The number of subdetectors (not including the "Tracker" detector)
const UInt_t NDETECTORS=19;

/// The maximum number of objects to be saved in the FlatTree
const UInt_t NMAXPARTICLES = 100;
const UInt_t NMAXTRUEPARTICLES = 500;
const UInt_t NMAXTRUEVERTICES = 50; // per spill
const UInt_t NMAXVERTICES = 20;
const UInt_t NMAXFGDTIMEBINS = 10;
const UInt_t NMAXCROSSEDDET = 30;

const UInt_t NMAXTPCS  = 3;
const UInt_t NMAXFGDS  = 2;
const UInt_t NMAXECALS = 3;
const UInt_t NMAXP0DS  = 1;
const UInt_t NMAXSMRDS = 4;
const UInt_t NMAXTRACKERS = 8;
const UInt_t NMAXTRUEVERTICESMATCH = 3;
const UInt_t NMAXPARTICLESINVERTEX = 7;

/// Total numbers of particular sub-detectors
const UInt_t NECALSUBDETS = 9;
const UInt_t NSMRDSUBDETS = 4;

// forward declarations
class AnaTrueParticleB;


/// Representation of a detector crossing info for a true particle (G4 trajectory).
class AnaDetCrossingB{
 public :

  AnaDetCrossingB();
  virtual ~AnaDetCrossingB(){}

  /// Clone this object.
  virtual AnaDetCrossingB* Clone() {
    return new AnaDetCrossingB(*this);
  }

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaDetCrossingB(const AnaDetCrossingB& cross);

 public:  

  /// for each subdetector tell the entrance position
  Float_t EntrancePosition[4];
  
  /// for each subdetector tell the exit position
  Float_t ExitPosition[4];

  /// for each subdetector tell the entrance momentum
  Float_t EntranceMomentum[3];

  /// for each subdetector tell the exit momentum
  Float_t ExitMomentum[3];
  
  /// If the particle passes through an active part of the subdetector
  bool InActive;

  /// Bit field describing the detector that the object is in. 
  /// See SubDetId.hxx for the detector enumerations used to 
  /// interrogate this object
  unsigned long Detector;

};

/// Representation of a true Monte Carlo vertex.
class AnaTrueVertexB: public AnaTrueObjectC{
 public :

  AnaTrueVertexB();
  virtual ~AnaTrueVertexB();

  /// Clone this object.
  virtual AnaTrueVertexB* Clone() {
    return new AnaTrueVertexB(*this);
  }

  /// Dump the object to screen.
  void Print() const;
 
 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaTrueVertexB(const AnaTrueVertexB& vertex);

 public:
  
  /// The PDG code of the incoming neutrino.
  Int_t NuPDG;

  /// The true energy of the incoming neutrino.
  Float_t NuEnergy;

  /// The position the true interaction happened at.
  Float_t Position[4];

  /// The bunch in which this true interaction occurred, based on the time of
  /// the interaction and the bunching used for the reconstructed objects.
  Int_t Bunch;

  /// The true particles associated with this vertex.
  AnaTrueParticleB** TrueParticles; //!
  /// How many true particles are associated with this vertex
  int nTrueParticles; //!

  /// A copy of TrueParticles but using an std::vector, which can be saved into a tree
  std::vector<AnaTrueParticleB*> TrueParticlesVect;

  /// Bit field describing the detector that the object is in. 
  /// See SubDetId.hxx for the detector enumerations used to 
  /// interrogate this object
  unsigned long Detector;

};

/// Representation of a true Monte Carlo trajectory/particle.
class AnaTrueParticleB: public AnaTrueObjectC{
 public :

  AnaTrueParticleB();
  virtual ~AnaTrueParticleB();

  /// Clone this object.
  virtual AnaTrueParticleB* Clone() {
    return new AnaTrueParticleB(*this);
  }

  /// Dump the object to screen.
  void Print() const;

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaTrueParticleB(const AnaTrueParticleB& truePart);

 public:

  /// The PDG code of this particle.
  Int_t PDG;

  /// The ID of the primary particle that created this particle. You can use the
  /// anaUtils::GetTrueTrackByID(Int_t ID) or anaUtils::GetPrimaryTrueTrack(AnaTrueParticleB* track)
  /// functions to find the AnaTrueParticleB representing the primary particle.
  Int_t PrimaryID;
  
  /// The ID of this particle's immediate parent, or 0 if there is no parent.
  Int_t ParentID;

  /// The PDG code of this particle's immediate parent, or 0 if there is no parent.
  Int_t ParentPDG;

  /// The ID of this particle's  grandparent, or 0 if there is no grandparent.
  Int_t GParentID;
  
  /// The PDG code of this particle's grandparent, or 0 if there is no grandparent.
  Int_t GParentPDG;

  /// The initial position of the true particle.
  Float_t Position[4];

  /// The end position of the true particle.
  Float_t PositionEnd[4];

  /// The initial direction of the true particle.
  Float_t Direction[3];

  /// The initial momentum of the true particle.
  Float_t Momentum;

  /// The true charge of the particle.
  Float_t Charge;

  /// The TruthVertexID of the AnaTrueVertexB of the interaction that created this AnaTrueParticleB.
  Int_t VertexID;

  /// Pointer to the AnaTrueVertexB of the interaction that created this AnaTrueParticleB.
  AnaTrueVertexB* TrueVertex; 

  /// Vector of detector crossings. These are in the order in which the
  /// detectors were crossed.
  AnaDetCrossingB** DetCrossings; //!
  /// The number of DetCrossing objects
  int nDetCrossings; //!

  /// A copy of DetCrossings but using an std::vector, which can be saved into a tree
  std::vector<AnaDetCrossingB*> DetCrossingsVect;
  
  
  /// Is this a true primary pi0 decay photon or the child of one?
  bool IsTruePrimaryPi0DecayPhoton;
  bool IsTruePrimaryPi0DecayPhotonChild;
};

//Time of flight information
class AnaToF{

public :

  AnaToF(){
    FGD1_FGD2   = -999.0;	Flag_FGD1_FGD2    = false;
    P0D_FGD1    = -999.0;	Flag_P0D_FGD1     = false;
 	  DSECal_FGD1 = -999.0; Flag_DSECal_FGD1  = false;
    ECal_FGD1   = -999.0;	Flag_ECal_FGD1    = false;
    ECal_FGD2   = -999.0;	Flag_ECal_FGD2    = false;
 	  DSECal_FGD2 = -999.0; Flag_DSECal_FGD2  = false;
  };

  AnaToF& operator=(const AnaToF& B){
    this->FGD1_FGD2        = B.FGD1_FGD2;
    this->Flag_FGD1_FGD2   = B.Flag_FGD1_FGD2;
    this->P0D_FGD1         = B.P0D_FGD1;
    this->Flag_P0D_FGD1    = B.Flag_P0D_FGD1;
    this->DSECal_FGD1      = B.DSECal_FGD1;
    this->Flag_DSECal_FGD1 = B.Flag_DSECal_FGD1;
    this->ECal_FGD1        = B.ECal_FGD1;
    this->Flag_ECal_FGD1   = B.Flag_ECal_FGD1;
    this->ECal_FGD2        = B.ECal_FGD2;
    this->Flag_ECal_FGD2   = B.Flag_ECal_FGD2;
    this->DSECal_FGD2      = B.DSECal_FGD2;
    this->Flag_DSECal_FGD2 = B.Flag_DSECal_FGD2;
    return *this;
  }

  Float_t FGD1_FGD2;
  bool    Flag_FGD1_FGD2;
  Float_t P0D_FGD1;
  bool    Flag_P0D_FGD1;
  Float_t DSECal_FGD1;
  bool    Flag_DSECal_FGD1;
  Float_t ECal_FGD1;
  bool    Flag_ECal_FGD1;
  Float_t ECal_FGD2;
  bool    Flag_ECal_FGD2;
  Float_t DSECal_FGD2;
  bool    Flag_DSECal_FGD2;
};


//Timing information
class AnaTime{

public :

  AnaTime(){
    FGD1   = -999.0;
    FGD2   = -999.0;
    P0D    = -999.0;
    ECal   = -999.0;
    DSECal = -999.0;
  };

  AnaTime& operator=(const AnaTime& B){
    this->FGD1             = B.FGD1;
    this->Flag_FGD1        = B.Flag_FGD1;
    this->FGD2             = B.FGD2;
    this->Flag_FGD2        = B.Flag_FGD2;
    this->P0D              = B.P0D;
    this->Flag_P0D         = B.Flag_P0D;
    this->ECal             = B.ECal;
    this->Flag_ECal        = B.Flag_ECal;
    this->DSECal           = B.DSECal;
    this->Flag_DSECal      = B.Flag_DSECal;

    return *this;
  }

  Float_t FGD1;
  bool    Flag_FGD1;
  Float_t FGD2;
  bool    Flag_FGD2;
  Float_t P0D;
  bool    Flag_P0D;
  Float_t ECal;
  bool    Flag_ECal;
  Float_t DSECal;
  bool    Flag_DSECal;
};


/// Representation of a reconstructed particle (track or shower).
class AnaParticleB: public AnaRecObjectC{
 public :

  AnaParticleB();
  virtual ~AnaParticleB();

  /// Clone this object.
  virtual AnaParticleB* Clone() {
    return new AnaParticleB(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

  /// Compare the NHits of two particles. Return whether t1 is higher NHts than t2.
  static bool CompareNHits(const AnaParticleB* t1, const AnaParticleB* t2);

  /// Return a casted version of the AnaTrueObject associated
  AnaTrueParticleB* GetTrueParticle() const {return static_cast<AnaTrueParticleB*>(TrueObject);}

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaParticleB(const AnaParticleB& part);

 public:

  /// The number of hits in the particle.
  Int_t NHits;

  /// The number of nodes in the reconstructed object.
  Int_t NNodes;  
  
  /// The reconstructed start direction of the particle.
  Float_t DirectionStart[3];

  /// The reconstructed end direction of the particle.
  Float_t DirectionEnd[3];
  
  /// The reconstructed start position of the particle.
  Float_t PositionStart[4];

  /// The reconstructed end position of the particle.
  Float_t PositionEnd[4];

  /// Objects can be manipulated by input variations (which apply corrections
  /// and systematics). There are three copies of each spill and this member
  /// provides links between the different versions of this object. The link
  /// goes as Spill->CorrectedSpill or CorrectedSpill->RawSpill. If this object
  /// is in the RawSpill, then this member is NULL.
  const AnaParticleB* Original; //!
};


/// Extension to AnaParticleB containing momentum and charge info, to be used by aggregation
class AnaParticleMomE{
 public :

  AnaParticleMomE();
  virtual ~AnaParticleMomE();

  /// Dump the object to screen.
  virtual void Print() const;

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaParticleMomE(const AnaParticleMomE& part);

 public:

  /// The reconstructed charge of the particle.
  Float_t Charge;

  /// The reconstructed momentum of the particle, at the start position.
  Float_t Momentum;  

  /// The reconstructed momentum of the particle, at the end position.
  Float_t MomentumEnd;
};


/// /// Extension to AnaParticleB containing momentum and charge info
class AnaParticleMomB: public AnaParticleB, public AnaParticleMomE{
 public :

  AnaParticleMomB();
  virtual ~AnaParticleMomB();

  /// Clone this object.
  virtual AnaParticleMomB* Clone() {
    return new AnaParticleMomB(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

  /// Function used to sort momentum in decreasing order
  static bool CompareMomentum(const AnaParticleMomB* t1, const AnaParticleMomB* t2);

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaParticleMomB(const AnaParticleMomB& part);
};


/// Representation of a TPC segment of a global track.
class AnaTPCParticleB:public AnaParticleMomB {
 public :

  AnaTPCParticleB();
  virtual ~AnaTPCParticleB();

  /// Clone this object.
  virtual AnaTPCParticleB* Clone() {
    return new AnaTPCParticleB(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaTPCParticleB(const AnaTPCParticleB& seg);

 public:

  /// Error of the momentum at the start of the segment.
  Float_t MomentumError;

  /// Reconstructed momentum with the empirical distortion corrections
  Float_t RefitMomentum;

  /// Reconstructed momentum with the E-field distortion corrections
  Float_t EFieldRefitMomentum;

  /// dE/dx as measured by the TPC.
  Float_t dEdxMeas;

  /// Expected dE/dx for a muon, based on the reconstructed momentum.
  Float_t dEdxexpMuon; 

  /// Expected dE/dx for an electron, based on the reconstructed momentum.
  Float_t dEdxexpEle; 

  /// Expected dE/dx for a pion, based on the reconstructed momentum.
  Float_t dEdxexpPion; 

  /// Expected dE/dx for a proton, based on the reconstructed momentum.
  Float_t dEdxexpProton; 

  /// Expected error on the dE/dx measurement, for the muon hypothesis.
  Float_t dEdxSigmaMuon; 

  /// Expected error on the dE/dx measurement, for the electron hypothesis.
  Float_t dEdxSigmaEle; 

  /// Expected error on the dE/dx measurement, for the pion hypothesis.
  Float_t dEdxSigmaPion; 

  /// Expected error on the dE/dx measurement, for the proton hypothesis.
  Float_t dEdxSigmaProton; 
};

/// Representation of a FGD segment of a global track.
class AnaFGDParticleB:public AnaParticleB{
 public :

  AnaFGDParticleB();
  virtual ~AnaFGDParticleB(){}

  /// Clone this object.
  virtual AnaFGDParticleB* Clone() {
    return new AnaFGDParticleB(*this);
  }

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaFGDParticleB(const AnaFGDParticleB& seg);

 public:

  /// Input to the pull calculations, FGD-iso length calculated assuming a straight line and 
  /// applying a correction retrieved from truth based studies
  Float_t X;

  /// Muon pull, according to FGD information.
  Float_t Pullmu;

  /// Proton pull, according to FGD information.
  Float_t Pullp;

  /// Pion pull, according to FGD information.
  Float_t Pullpi;

  /// Dummy pull. If the FGD pulls weren't set, this is set to 1.
  Float_t Pullno;

  /// Containment flag required for proper PID analysis. 
  Int_t Containment;

};

/// Representation of an ECAL segment of a global track.
class AnaECALParticleB:public AnaParticleB{
 public :

  AnaECALParticleB();
  virtual ~AnaECALParticleB(){}

  /// Clone this object.
  virtual AnaECALParticleB* Clone() {
    return new AnaECALParticleB(*this);
  }

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaECALParticleB(const AnaECALParticleB& seg);

 public:

  /// The Ecal Track-Shower PID variable.
  /// Track-like objects tend to 1, shower-like to 0
  Float_t TrShVal; 
  
  /// The mipEM LLR PID variable
  /// Negative means MIP-like, positive EM shower-like
  Float_t PIDMipEm; 

  Float_t PIDEmHip;
  
  /// The length of the ECal segment
  Float_t Length;
  
  /// The energy deposited in the ECal, as determined by an EM energy fitter.
  /// This is the recommended value to use when examining the ECal energy.
  Float_t EMEnergy;

  /// The "raw" charge deposited in the ECal. "raw" means that it has
  /// calibrations applied, but is still in units the detector understands,
  /// rather than units applicable to physics. Use EMEnergy instead.
  Float_t EDeposit;

  /// Whether the object is shower-like or track-like. A default cut of 0.5
  /// is applied - below 0.5 means the object is shower-like.
  Int_t IsShowerLike;
  
  /// Time charged averaged over hits 
  Float_t AvgTime;

  /// Innermost layer hit of the ecal object (used in ecal pi0 veto)
  Int_t MostUpStreamLayerHit;

  /// The position of the shower-fit to this object by ecalRecon.
  Float_t ShowerPosition[3];
};

/// Representation of an SMRD segment of a global track.
class AnaSMRDParticleB:public AnaParticleB{
 public :

  AnaSMRDParticleB();
  virtual ~AnaSMRDParticleB(){}

  /// Clone this object.
  virtual AnaSMRDParticleB* Clone() {
    return new AnaSMRDParticleB(*this);
  }

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaSMRDParticleB(const AnaSMRDParticleB& seg);

 public:

  /// The averaged (using estimated  uncertainties) times 
  /// of track reconstructed hits
  Float_t AvgTime;
};

/// Representation of a P0D segment of a global track.
class AnaP0DParticleB:public AnaParticleB{
 public :

  AnaP0DParticleB();
  virtual ~AnaP0DParticleB(){}

  /// Clone this object.
  virtual AnaP0DParticleB* Clone() {
    return new AnaP0DParticleB(*this);
  }

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaP0DParticleB(const AnaP0DParticleB& seg);

 public:

   Float_t Length;
   Float_t ELoss;

};

/// Representation of a Tracker segment of a global track.
class AnaTrackerTrackB:public AnaParticleMomB{
 public :

  AnaTrackerTrackB();
  virtual ~AnaTrackerTrackB(){}

  /// Clone this object.
  virtual AnaTrackerTrackB* Clone() {
    return new AnaTrackerTrackB(*this);
  }

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaTrackerTrackB(const AnaTrackerTrackB& seg);

 public:

};

class AnaRecTrueMatchB{
 public :

  AnaRecTrueMatchB(){}
  virtual ~AnaRecTrueMatchB(){}

  /// Clone this object.
  virtual AnaRecTrueMatchB* Clone() {
    return new AnaRecTrueMatchB(*this);
  }

 public:
  
  /// The cleanliness of the true-reco matching.
  Float_t Cleanliness;

  /// The completeness of the true-reco matching.
  Float_t Completeness;
};

/// Representation of a global track.
class AnaTrackB: public AnaParticleMomB{
 public :

  AnaTrackB();
  virtual ~AnaTrackB();

  /// Clone this object.
  virtual AnaTrackB* Clone() {
    return new AnaTrackB(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

  /// Return a casted version of the original AnaParticleB associated
  const AnaTrackB* GetOriginalTrack() const {return static_cast<const AnaTrackB*>(Original);}
  
  /// Function used to sort range momentum muon in decreasing order
  static bool CompareMuonRangeMomentum(const AnaTrackB* t1, const AnaTrackB* t2);

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaTrackB(const AnaTrackB& track);

 public:

  /// The index of this particle track in the vector of particles. TODO: Not sure it is needed (only use in PID utils)
  Int_t Index;

  ///Momentum by range calculated with muon hypothesis 
  Float_t RangeMomentumMuon;

  /// Variables in order to be able to flip a particle based on ToF info
  /// Contains kinematics parameters which results of the particle fit 
  /// with a reverse sense hypothesis

  /// Momentum for the main PID hypothesis and reverse sense
  Float_t MomentumFlip; 

  /// Whether track passes TPC quality cut.
  Int_t TPCQualityCut;
  
  /// Times of flight between pairs of detectors
  AnaToF ToF;

  /// Mean times of detectors
  AnaTime Time;
 
  /// The TPC segments that contributed to this global track.
  AnaTPCParticleB* TPCSegments[NMAXTPCS]; //!
  /// How many TPC tracks are associated with this track
  int nTPCSegments; //!

  /// The FGD segments that contributed to this global track.
  AnaFGDParticleB* FGDSegments[NMAXFGDS]; //!
  /// How many FGD tracks are associated with this track
  int nFGDSegments; //!

  /// The ECAL segments that contributed to this global track.
  AnaECALParticleB* ECALSegments[NMAXECALS]; //!
  /// How many ECAL tracks are associated with this track
  int nECALSegments; //!

  /// The SMRD segments that contributed to this global track.
  AnaSMRDParticleB* SMRDSegments[NMAXSMRDS]; //!
  /// How many SMRD tracks are associated with this track
  int nSMRDSegments; //!

  /// The P0D segments that contributed to this global track.
  AnaP0DParticleB* P0DSegments[NMAXP0DS]; //!
  /// How many P0D tracks are associated with this track
  int nP0DSegments; //!

  /// A copy of the segment arrays but using an std::vector, which can be saved into a tree
  std::vector<AnaTPCParticleB*>  TPCSegmentsVect; 
  std::vector<AnaFGDParticleB*>  FGDSegmentsVect; 
  std::vector<AnaECALParticleB*> ECALSegmentsVect; 
  std::vector<AnaSMRDParticleB*> SMRDSegmentsVect; 
  std::vector<AnaP0DParticleB*>  P0DSegmentsVect; 
};

/// Representation of a global vertex.
class AnaVertexB: public AnaRecObjectC{
 public :

  AnaVertexB();
  virtual ~AnaVertexB();

  /// Clone this object.
  virtual AnaVertexB* Clone() {
    return new AnaVertexB(*this);
  }

  /// Dump the object to screen.
  void Print() const;

  /// Function used to sort PrimaryIndex in increasing order
  static bool ComparePrimaryIndex(const AnaVertexB* t1, const AnaVertexB* t2);

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaVertexB(const AnaVertexB& vertex);

 public:
  
  /// The bunch of the global vertex, based on the Position.T()
  Int_t Bunch;

  /// Index of the global vertex.
  Int_t PrimaryIndex;

  /// The identified position of the global vertex.
  Float_t Position[4];

  /// The particles that are associated to this global vertex.
  /// It would be better to rename this into ReconParticles
  AnaParticleB** Particles; //!
  int nParticles; //!

  /// A copy of Particles but using an std::vector, which can be saved into a tree
  std::vector<AnaParticleB*> ParticlesVect;

  /// The true vertex that is associated with this global vertex.
  /// vertex->GetMainTrueVertex() could replace this info.
  AnaTrueVertexB* TrueVertex;

  /// The true vertices that are associated with this global vertex, with the related cleanliness and completeness.
  std::vector< std::pair<AnaTrueVertexB*, AnaRecTrueMatchB> > TrueVerticesMatch; //!

  /// Objects can be manipulated by corrections
  /// There are three copies of each spill and this member
  /// provides links between the different versions of this object. The link
  /// goes as Spill->CorrectedSpill or CorrectedSpill->RawSpill. If this object
  /// is in the RawSpill, then this member is NULL.
  const AnaVertexB* Original; //!

 public:

  /// The true vertices that is associated with this global vertex.
  int GetTrueVertices(AnaTrueVertexB** arr);

  /// The main true vertex that is associated with this global vertex.
  AnaTrueVertexB* GetMainTrueVertex(bool warning=true);

  /// The cleanliness of this global vertex respect to the main associated true vertex
  Float_t GetMainTrueVertexCleanliness();

  /// The completeness of this global vertex respect to the main associated true vertex
  Float_t GetMainTrueVertexCompleteness();

  /// The cleanliness of this global vertex respect to an associated true vertex
  Float_t GetTrueVertexCleanliness(const AnaTrueVertexB& trueVertex);

  /// The completeness of this global vertex respect to an associated true vertex
  Float_t GetTrueVertexCompleteness(const AnaTrueVertexB& trueVertex);

};

/// Representation of the beam quality and perhaps other beam information as needed.
class AnaBeamB{
 public :

  AnaBeamB();
  virtual ~AnaBeamB(){}

  /// Clone this object.
  virtual AnaBeamB* Clone() {
    return new AnaBeamB(*this);
  }

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaBeamB(const AnaBeamB& beam);

 public:
  
  /// Good spill flag, as defined in Beam Summary Data. 0 is bad.
  Int_t GoodSpill;

  /// The POT since the previously saved spill. For data, this comes from the Beam Summary Data 
  /// Is the single Spill POT for oaAnalysis input and accumulated for MiniTree input
  Double_t POTSincePreviousSavedSpill;

  /// The number of spills since the previously saved spill
  /// Is the 1 for oaAnalysis input and accumulated for MiniTree input
  Int_t SpillsSincePreviousSavedSpill;
};

/// Representation of the ND280 data quality flags.
class AnaDataQualityB {
  public:

    AnaDataQualityB();
    virtual ~AnaDataQualityB() {}

    /// Clone this object.
    virtual AnaDataQualityB* Clone() {
      return new AnaDataQualityB(*this);
    }

  protected:

    /// Copy constructor is protected, as Clone() should be used to copy this object.
    AnaDataQualityB(const AnaDataQualityB& dq);

  public:

    /// Simple boolean of whether there was good data quality or not.
    /// True means there was good data quality.
    bool GoodDaq;
};

class AnaDelayedClustersB{
public :

  AnaDelayedClustersB();
  virtual ~AnaDelayedClustersB(){}

  /// Clone this object.
  virtual AnaDelayedClustersB* Clone() {
    return new AnaDelayedClustersB(*this);
  }

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaDelayedClustersB(const AnaDelayedClustersB& clusters);

public:

  Float_t      MinTime;       //  Minimum time of a hit in the cluster
  Float_t      MaxTime;       //  Maximum time of a hit in the cluster
  Int_t        NHits;         //  Number of hits in the cluster
  Float_t      RawChargeSum;  //  Sum of the RawCharge of all hits
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
class AnaBunchB: public AnaBunchC{
 public :

  AnaBunchB();
  virtual ~AnaBunchB();

  /// Clone this object.
  virtual AnaBunchB* Clone() {
    return new AnaBunchB(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

  /// The primary reconstructed vertex associated with this bunch.
  AnaVertexB* GetPrimaryVertex();

  /// Returns true if the bunch is completely empty
  virtual bool IsEmpty() const;

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaBunchB(const AnaBunchB& bunch);

 public:

  /// The reconstructed vertices in this bunch after deleting CohOnH, PauliBlocked and fully contained events
  /// See https://bugzilla.nd280.org/show_bug.cgi?id=1174
  std::vector<AnaVertexB*> Vertices; 

  /// The reconstructed particles in this bunch after deleting CohOnH, PauliBlocked and fully contained events
  /// See https://bugzilla.nd280.org/show_bug.cgi?id=1174
  std::vector<AnaParticleB*> Particles;

  /// The delayed cluster groups
  AnaDelayedClustersB** DelayedClusters; //! 
  Int_t nDelayedClusters; //! 
};

class AnaFgdTimeBinB{
 public :

  AnaFgdTimeBinB();
  virtual ~AnaFgdTimeBinB();

  /// Clone this object.
  virtual AnaFgdTimeBinB* Clone() {
    return new AnaFgdTimeBinB(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaFgdTimeBinB(const AnaFgdTimeBinB& timeBin);

 public:
  
  Float_t      MinTime;     //
  Int_t        NHits[2];    //
  Float_t      RawChargeSum[2];    //  
};

class AnaEventInfoB{
 public :

  AnaEventInfoB();
  virtual ~AnaEventInfoB();

  /// Clone this object.
  virtual AnaEventInfoB* Clone() {
   return new AnaEventInfoB(*this);
  }

  /// Dump the object to screen.
  virtual void Print() const;

  /// Return whether this spill is from Monte Carlo or not.
  bool GetIsMC() const {return IsMC;}

  /// Return whether this spill is from Sand Monte Carlo or not.
  bool GetIsSandMC() const {return IsSand;}

  /// Set whether this event is from Sand MC
  void SetIsSandMC();

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaEventInfoB(const AnaEventInfoB& event);

 public:

  /// The ND280 run number.
  Int_t Run;

  /// The subrun number.
  Int_t SubRun;

  /// The ND280 event number.
  Int_t Event;
  
  /// Says if the event is MC or data
  bool IsMC;

  /// Is this event from the Sand MC
  bool IsSand;
};


/// The top-level object for representing information from the input files.
/// A single AnaSpillB contains information from an entire ND280 event.
/// The reconstructed objects are separated into bunches based on their time
/// information.
///
/// There are multiple copies of each spill available through the InputManager
/// [ND::input()], showing the raw state of the spill after reading it in from
/// the input files; after applying corrections; and after applying corrections
/// and systematics.
class AnaSpillB: public AnaSpillC{
 public :

  AnaSpillB();
  virtual ~AnaSpillB();

  /// Clone this object.
  virtual AnaSpillB* Clone() {
    AnaSpillB* spill = new AnaSpillB(*this);
    spill->RedoLinks();
    spill->isClone=true;
    return spill;
  }

  /// Dump the object to screen.
  virtual void Print() const;

  /// Return whether this spill is from Monte Carlo or not.
  bool GetIsMC() const {return EventInfo->GetIsMC();}

  /// Return whether this spill is from Sand Monte Carlo or not.
  bool GetIsSandMC() const {return EventInfo->GetIsSandMC();}

  /// Copy the arrays into std vectors, needed for MiniTree creation
  virtual void CopyArraysIntoVectors();

  /// Copy the vectore into the arrays, needed after MiniTree reading
  virtual void CopyVectorsIntoArrays();

  /// Redo reco-reco and reco-truth after cloning or reading MiniTree
  virtual void RedoLinks();

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaSpillB(const AnaSpillB& spill);

  /// Add to a particle a given associated vertex (not in the base class)
  virtual void associateVertexToParticle(AnaParticleB*, AnaVertexB*) const{}

  /// Add to a true vertex a given associated reconstructed vertex (not in the base class)
  virtual void associateVertexToTrueVertex(AnaVertexB*) const{}

  /// Add to a true particle a given associated reconstructed particle (not in the base class)
  virtual void associateParticleToTrueParticle(AnaParticleB*) const{}

 public:

  /// Is this the original Spill or a clone
  //  bool isClone;

  /// The unique ID of the Geometry for this spill
  UInt_t GeomID;
  
  /// Run, sunrun, event, time  stamp, etc
  AnaEventInfoB* EventInfo;

  /// The ND280 data quality flags for this spill.
  AnaDataQualityB* DataQuality;

  /// The beam quality flags for this spill.
  AnaBeamB* Beam;

  /// Total number of true vertices in the oaAnalysis Tree (even those not saved in the flat tree) 
  /// after deleting CohOnH, PauliBlocked and fully contained events (see https://bugzilla.nd280.org/show_bug.cgi?id=1174)
  UInt_t NTotalTrueVertices;

  /// Total number of true particles in the oaAnalysis Tree (even those not saved in the flat tree) 
  /// after deleting CohOnH, PauliBlocked and fully contained events (see https://bugzilla.nd280.org/show_bug.cgi?id=1174)
  UInt_t NTotalTrueParticles;

  /// The true MC vertices used in this spill.
  std::vector<AnaTrueVertexB*> TrueVertices;

  /// The true MC particles used in this spill.
  std::vector<AnaTrueParticleB*> TrueParticles;

  /// Reconstructed objects that didn't fit into one of the timing bunches.
  AnaBunchB* OutOfBunch;

  /// The FGD time bins
  std::vector<AnaFgdTimeBinB*> FgdTimeBins;
};

/// An AnaEventSummaryB is a class used for storing high level objects and quantities 
/// output of the analysis. For example 
///  - LeptonCandidate
///  - Event vertex
///  - NeutrinoEnergy
/// 

class AnaEventSummaryB:public AnaEventSummaryC{
 public :

  AnaEventSummaryB();
  virtual ~AnaEventSummaryB(){}

  void ResetSummary();

public:

  /// A pointer to the Lepton Candidate
  AnaParticleB* LeptonCandidate[SampleId::kNSamples];

  /// A pointer to the event's true vertex
  AnaTrueVertexB* TrueVertex[SampleId::kNSamples];

  /// Event recon vertex position
  float VertexPosition[SampleId::kNSamples][4];

  /// Event sample - whether it passes the NuMu-CCQE selection, or CC1Pi selection for example
  SampleId::SampleEnum EventSample;

  /// The link to the RooTrackerVtx tree
  UInt_t RooVertexIndex[SampleId::kNSamples];
};


class AnaEventB: public AnaEventC{
 public :

  AnaEventB();
  AnaEventB(const AnaSpillB& spill, const AnaBunchB& bunch);
  virtual ~AnaEventB();

  /// Copy the entire object
  using AnaEventC::Copy;
  virtual void Copy(const AnaEventC& event, bool copyBunchInfo = true, bool cloneTruth=true);

  /// Clone this object.
  virtual AnaEventB* Clone() {
    AnaEventB* event = new AnaEventB(*this);
    event->isClone=true;
    return event;
  }

  /// Dump the object to screen.
  virtual void Print() const;

  /// Return whether this event is from Monte Carlo or not.
  bool GetIsMC() const {return EventInfo.GetIsMC();}

  /// Return whether this event is from Sand Monte Carlo or not.
  bool GetIsSandMC() const {return EventInfo.GetIsSandMC();}

  /// User-frienly method to get event info (run,subrun,etc)
  virtual std::string GetEventInfoString() const;

 protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaEventB(const AnaEventB& event);

 public:

  /// Run, sunrun, event, time  stamp, etc
  AnaEventInfoB EventInfo;

  /// The ND280 data quality flags for this event.
  AnaDataQualityB* DataQuality;

  /// The beam quality flags for this event.
  AnaBeamB* Beam;

  /// The index of this bunch (0-7).
  Int_t Bunch;

  /// The reconstructed objects in this bunch.
  AnaParticleB** Particles;
  /// The number of reconstructed particles in the event
  int nParticles;

  /// The reconstructed objects in this bunch.
  AnaVertexB** Vertices;
  int nVertices;

  /// The FGD time bins
  AnaFgdTimeBinB** FgdTimeBins;
  int nFgdTimeBins;

  /// The delayed cluster groups
  AnaDelayedClustersB** DelayedClusters; //! 
  Int_t nDelayedClusters; //! 

  /// The true MC particles used in this spill.
  AnaTrueParticleB** TrueParticles;
  int nTrueParticles;

  /// The true MC vertices used in this spill.
  AnaTrueVertexB** TrueVertices;
  int nTrueVertices;
};

#endif

