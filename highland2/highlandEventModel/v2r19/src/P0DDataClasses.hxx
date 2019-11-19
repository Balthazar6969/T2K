#ifndef P0DDataClasses_hxx
#define P0DDataClasses_hxx


#include "DataClasses.hxx"

const UInt_t NMAXVERTEXGROUPS = 20;

class AnaP0DHit{
public :

  AnaP0DHit();
  virtual ~AnaP0DHit(){}

  /// Clone this object.
  virtual AnaP0DHit* Clone() {
    return new AnaP0DHit(*this);
  }

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaP0DHit(const AnaP0DHit& hit);

public:

  enum TypeEnum{
    kXHit=0,
    kYHit,
    kUnassigned
  };

  UInt_t  GeomID;    //  Geometry ID    
  //  UInt_t  ChanID;    //  Channel ID    
  Float_t Time;      //  Hit time as reported by hit->GetTime()

  //  TypeEnum Type;        //  Hit type (x or y)
  Float_t Charge;       //  Hit charge without attenuation correction    
  //  Float_t Position[4];  // 3D-Position and time of the hit. 3D pos from geomID
};


class AnaP0DCluster{
public :

  AnaP0DCluster();
  virtual ~AnaP0DCluster();

  /// Clone this object.
  virtual AnaP0DCluster* Clone() {
    return new AnaP0DCluster(*this);
  }

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaP0DCluster(const AnaP0DCluster& cluster);

public:

  AnaP0DHit**       Hits;             // Is it really needed ?
  Int_t             nHits;

  Float_t           Position   [4];
  Float_t           PosVariance[4];   // Is it really needed ?
  Short_t           ValidDimensions;

  std::string       AlgorithmName;    // --> enum
  unsigned int      UniqueID;
  Short_t           Cycle;
  //  Short_t           NFiducialHits;    // Clark has to tell me what it means
  Float_t           EDeposit;
  Float_t           Moments[9]; ///< Moments of the Cluster

  AnaTrueParticleB* TrueParticle;
};


class AnaP0DParticle: public AnaP0DParticleB, public AnaParticleMomE {
public :

  AnaP0DParticle();
  virtual ~AnaP0DParticle();

  /// Clone this object.
  virtual AnaP0DParticle* Clone() {
    return new AnaP0DParticle(*this);
  }

  /// Dump the object to screen.
  void Print() const;

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaP0DParticle(const AnaP0DParticle& particle);

public:

  enum ParticleTypeEnum{
    kTrack=0,
    kShower,
    kUnknown
  };


  ParticleTypeEnum     Type;

  AnaP0DCluster**      Clusters;  //!
  Int_t                nClusters; //!

  Short_t              ValidDimensions;
  std::string          AlgorithmName;    // --> enum
  Float_t              SideDeposit;
  Float_t              EndDeposit;
  Float_t              EDeposit;
  std::vector<Short_t> PID;
  std::vector<Float_t> PID_weight;

  // --------- Only for systematics -------------------
  std::vector<std::string>           realPIDNames;
  std::vector<std::vector<Float_t> > realPIDValues;
  std::vector<std::string>           integerPIDNames;
  std::vector<std::vector<Short_t> > integerPIDValues;

#if VERSION_HAS_P0D_AVERAGED_TIME
  /// Average Time: using results from time vs distance fit
  Float_t AvgTime;
#endif

};

class AnaP0DVertex: public AnaVertexB{
public :

  AnaP0DVertex();
  virtual ~AnaP0DVertex();

  /// Clone this object.
  virtual AnaP0DVertex* Clone() {
    return new AnaP0DVertex(*this);
  }

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaP0DVertex(const AnaP0DVertex& vertex);

public:

  AnaP0DCluster**   Clusters;
  Int_t             nClusters;

  Float_t           PosVariance[4];   // Is it really needed ?
  Short_t           ValidDimensions;
  std::string       AlgorithmName;    // --> enum
  Short_t           NHits;
  //  unsigned int      UniqueID;     // not needed for the moment
  Float_t           Fiducial;
};

class AnaP0DBunch: public AnaBunch{  // TODO. Should inherit from AnaBunchB
public :

  AnaP0DBunch();
  virtual ~AnaP0DBunch();

  /// Clone this object.
  virtual AnaP0DBunch* Clone() {
    return new AnaP0DBunch(*this);
  }

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaP0DBunch(const AnaP0DBunch& bunch);

public:

  Bool_t               P0DWaterStatus;         

  AnaP0DCluster**      Clusters;
  Int_t                nClusters;

  std::vector<AnaP0DBunch*> AlternateBunches;

  std::string FullName;
};


class AnaP0DEvent: public AnaEventB{
public :

  AnaP0DEvent();
  virtual ~AnaP0DEvent();

  /// Clone this object.
  virtual AnaP0DEvent* Clone() {
    return new AnaP0DEvent(*this);
  }

  /// Create AnaP0dEvent from a Spill and a bunch in that spill
  AnaP0DEvent(const AnaSpillB& spill, const AnaP0DBunch& p0dBunch);

protected:

  /// Copy constructor is protected, as Clone() should be used to copy this object.
  AnaP0DEvent(const AnaP0DEvent& event);

public:

  Bool_t P0DWaterStatus;         

  AnaP0DCluster**      Clusters;
  Int_t                nClusters;

  std::vector<AnaP0DEvent*> AlternateEvents;

  std::string FullName;
};


class EventBoxP0D2:public EventBoxB{
 public :

  enum VertexGroupEnum{
    kVertexUnassigned=0,
    kVerticesInP0DFinal
  };

  enum PaticleGroupEnum{
    kParticleUnassigned=0,
    kAllP0DParticles,
    kP0DTracks,
    kP0DShowers
  };


  EventBoxP0D2();
  virtual ~EventBoxP0D2();

  /// Different groups of tracks used for selection and systematics
  Int_t nVerticesInGroup[NMAXVERTEXGROUPS];
  AnaP0DVertex** VerticesInGroup[NMAXVERTEXGROUPS];
};


namespace anaUtils{
  void CreateArray(AnaP0DCluster**          &tgtArr,  int nObj);
  void CopyArray(  AnaP0DCluster** const    &src,     AnaP0DCluster**     &dest,  int n);
  void CreateArray(AnaP0DHit**              &tgtArr,  int nObj);
  void CopyArray(  AnaP0DHit** const        &src,     AnaP0DHit**         &dest,  int n);
}

#endif
