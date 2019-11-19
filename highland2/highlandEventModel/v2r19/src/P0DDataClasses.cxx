#define DataClasses_C

#include "P0DDataClasses.hxx"
#include "BasicUtils.hxx"
#include "TMath.h"

// define a constant value for uninitialised parameters
const Float_t  kFloatUnassigned = -999.;
const UInt_t   kUnassigned      = 0xDEADBEEF;
const short    kShortUnassigned = 0xFF;

//********************************************************************
AnaP0DHit::AnaP0DHit(){
//********************************************************************
  
  GeomID = kUnassigned;
  Time   = kFloatUnassigned;    
  //  ChanID = kUnassigned;
  //  Type   = kUnassigned;
  Charge = kFloatUnassigned;
  //  anaUtils::VectorToArray (TLorentzVector(0,0,0,0), Position   );
}

//********************************************************************
AnaP0DHit::AnaP0DHit(const AnaP0DHit& p0dHit){
//********************************************************************

  GeomID = p0dHit.GeomID;
  //  ChanID = p0dHit.ChanID;
  Time   = p0dHit.Time;  
  //  Type   = p0dHit.Type;
  Charge = p0dHit.Charge;
  //  anaUtils::CopyArray(p0dHit.Position,    Position,    4);
}

//********************************************************************
AnaP0DCluster::AnaP0DCluster(){
//********************************************************************
  AlgorithmName   = "";
  ValidDimensions = kShortUnassigned;  
  UniqueID        = kUnassigned;
  Cycle           = kShortUnassigned;
  EDeposit        = kFloatUnassigned;

  anaUtils::VectorToArray (TLorentzVector(0,0,0,0), Position   );
  anaUtils::VectorToArray (TLorentzVector(0,0,0,0), PosVariance);

  for (UInt_t i=0;i<9;i++) Moments[i]=0;

  TrueParticle = NULL;

  Hits = NULL;
  nHits = 0;
}

//********************************************************************
AnaP0DCluster::AnaP0DCluster(const AnaP0DCluster& p0dCluster){
//********************************************************************

  AlgorithmName     = p0dCluster.AlgorithmName;
  ValidDimensions   = p0dCluster.ValidDimensions;
  UniqueID          = p0dCluster.UniqueID;
  Cycle             = p0dCluster.Cycle;
  EDeposit          = p0dCluster.EDeposit;

  anaUtils::CopyArray      (p0dCluster.Moments,     Moments,     9);

  anaUtils::CopyArray      (p0dCluster.Position,    Position,    4);
  anaUtils::CopyArray      (p0dCluster.PosVariance, PosVariance, 4);

  TrueParticle         = p0dCluster.TrueParticle;

  anaUtils::CreateArray(Hits, p0dCluster.nHits);   
  anaUtils::CopyArray(p0dCluster.Hits, Hits, p0dCluster.nHits);  
  nHits = p0dCluster.nHits;
}

//********************************************************************
AnaP0DCluster::~AnaP0DCluster(){
//********************************************************************

  // Must delete array of pointers, since we re-create this every time we apply a selection
  if(Hits != NULL) delete [] Hits;
  Hits = NULL;
}
 
//********************************************************************
AnaP0DParticle::AnaP0DParticle():AnaP0DParticleB(),AnaParticleMomE(){
//********************************************************************

#if VERSION_HAS_P0D_AVERAGED_TIME
  AvgTime     = -999.;
#endif


  Type                 = kUnknown; 
  AlgorithmName        = "";
  //  Status               = kUnassigned;
  ValidDimensions      = kShortUnassigned;
  SideDeposit          = kFloatUnassigned;
  EndDeposit           = kFloatUnassigned;
  EDeposit             = kFloatUnassigned;

  realPIDNames         .clear();
  realPIDValues        .clear();
  integerPIDNames      .clear();
  integerPIDValues     .clear();
  PID                  .clear();
  PID_weight           .clear();

  Clusters = NULL;
  nClusters = 0;
}

//********************************************************************
AnaP0DParticle::AnaP0DParticle(const AnaP0DParticle& p0dParticle):AnaP0DParticleB(p0dParticle),AnaParticleMomE(p0dParticle){
//********************************************************************

#if VERSION_HAS_P0D_AVERAGED_TIME
  AvgTime     = p0dParticle.AvgTime;
#endif


  Type                 = p0dParticle.Type;
  AlgorithmName        = p0dParticle.AlgorithmName;
  //  Status               = p0dParticle.Status;
  ValidDimensions      = p0dParticle.ValidDimensions;
  SideDeposit          = p0dParticle.SideDeposit;
  EndDeposit           = p0dParticle.EndDeposit;
  EDeposit             = p0dParticle.EDeposit;
  realPIDNames         = p0dParticle.realPIDNames;
  realPIDValues        = p0dParticle.realPIDValues;
  integerPIDNames      = p0dParticle.integerPIDNames;
  integerPIDValues     = p0dParticle.integerPIDValues;
  PID                  = p0dParticle.PID;
  PID_weight           = p0dParticle.PID_weight;

  anaUtils::CreateArray(Clusters, p0dParticle.nClusters);   
  anaUtils::CopyArray(p0dParticle.Clusters, Clusters, p0dParticle.nClusters);
  
  nClusters = p0dParticle.nClusters;
}

//********************************************************************
AnaP0DParticle::~AnaP0DParticle(){
//********************************************************************

  // Must delete array of pointers, since we re-create this every time we apply a selection
  if(Clusters != NULL) delete [] Clusters;
  Clusters = NULL;
}

//********************************************************************
void AnaP0DParticle::Print() const{
//********************************************************************

  std::cout << "-------- AnaP0DParticle --------- " << std::endl;

  AnaParticleB::Print();

  std::cout << "Type:                  " << Type                << std::endl;
  std::cout << "#Clusters:             " << nClusters           << std::endl;
}
 
//********************************************************************
AnaP0DVertex::AnaP0DVertex():AnaVertexB(){
//********************************************************************

  ValidDimensions = kShortUnassigned;
  AlgorithmName   = "";
  NHits           = kShortUnassigned;
  Fiducial        = kFloatUnassigned;
  //  UniqueID        = kUnassigned;

  anaUtils::VectorToArray (TLorentzVector(0,0,0,0), PosVariance);

  Clusters = NULL;
  nClusters = 0;
}

//********************************************************************
AnaP0DVertex::AnaP0DVertex(const AnaP0DVertex& p0dVertex):AnaVertexB(p0dVertex){
//********************************************************************

  AlgorithmName     = p0dVertex.AlgorithmName;
  ValidDimensions   = p0dVertex.ValidDimensions;  
  NHits             = p0dVertex.NHits;
  //  UniqueID          = p0dVertex.UniqueID;
  Fiducial          = p0dVertex.Fiducial;

  anaUtils::CopyArray      (p0dVertex.PosVariance, PosVariance, 4);

  anaUtils::CreateArray(Clusters, p0dVertex.nClusters);   
  anaUtils::CopyArray(p0dVertex.Clusters, Clusters, p0dVertex.nClusters);
  
  nClusters = p0dVertex.nClusters;
}

//********************************************************************
AnaP0DVertex::~AnaP0DVertex(){
//********************************************************************

  // Must delete array of pointers, since we re-create this every time we apply a selection
  if(Clusters != NULL) delete [] Clusters;
  Clusters = NULL;
}

//********************************************************************
AnaP0DBunch::AnaP0DBunch():AnaBunch(){
//********************************************************************
  Clusters = NULL;
  nClusters = 0;

  AlternateBunches.clear();

  FullName="";
  P0DWaterStatus = false;
}

//********************************************************************
AnaP0DBunch::AnaP0DBunch(const AnaP0DBunch& p0dBunch):AnaBunch(p0dBunch){
//********************************************************************

  anaUtils::CreateArray(Clusters, p0dBunch.nClusters);   
  anaUtils::CopyArray(p0dBunch.Clusters, Clusters, p0dBunch.nClusters);

  P0DWaterStatus = p0dBunch.P0DWaterStatus;  
  nClusters = p0dBunch.nClusters;

  for (std::vector<AnaP0DBunch*>::const_iterator it=p0dBunch.AlternateBunches.begin();it!=p0dBunch.AlternateBunches.end();it++)
    AlternateBunches.push_back((*it)->Clone());

  FullName=p0dBunch.FullName;
}

//********************************************************************
AnaP0DBunch::~AnaP0DBunch(){
//********************************************************************

  // Must delete array of pointers, since we re-create this every time we apply a selection
  if(Clusters != NULL) delete [] Clusters;
  Clusters = NULL;

  for (std::vector<AnaP0DBunch*>::iterator it=AlternateBunches.begin();it!=AlternateBunches.end();it++)
    delete *it;

  AlternateBunches.clear();
}

//********************************************************************
AnaP0DEvent::AnaP0DEvent():AnaEventB(){
//********************************************************************
  Clusters = NULL;
  nClusters = 0;

  AlternateEvents.clear();

  FullName="";

  P0DWaterStatus = false;
}

//********************************************************************
AnaP0DEvent::AnaP0DEvent(const AnaP0DEvent& p0dEvent):AnaEventB(p0dEvent){
//********************************************************************

  anaUtils::CreateArray(Clusters, p0dEvent.nClusters);   
  anaUtils::CopyArray(p0dEvent.Clusters, Clusters, p0dEvent.nClusters);
  
  P0DWaterStatus = p0dEvent.P0DWaterStatus;
  nClusters = p0dEvent.nClusters;


  for (std::vector<AnaP0DEvent*>::const_iterator it=p0dEvent.AlternateEvents.begin();it!=p0dEvent.AlternateEvents.end();it++)
    AlternateEvents.push_back((*it)->Clone());

  FullName=p0dEvent.FullName;
}

//*****************************************************************************
AnaP0DEvent::AnaP0DEvent(const AnaSpillB& spill, const AnaP0DBunch& p0dBunch):AnaEventB(spill,p0dBunch) {
//*****************************************************************************

  anaUtils::CreateArray(Clusters, p0dBunch.nClusters);   
  anaUtils::CopyArray(p0dBunch.Clusters, Clusters, p0dBunch.nClusters);
  
  P0DWaterStatus = p0dBunch.P0DWaterStatus;
  nClusters = p0dBunch.nClusters;

  FullName=p0dBunch.FullName;

  for (std::vector<AnaP0DBunch*>::const_iterator it=p0dBunch.AlternateBunches.begin();it!=p0dBunch.AlternateBunches.end();it++){
    AnaP0DEvent* alt = new AnaP0DEvent(spill, **it);
    AlternateEvents.push_back(alt);
  }

}

//********************************************************************
AnaP0DEvent::~AnaP0DEvent(){
//********************************************************************

  // Must delete array of pointers, since we re-create this every time we apply a selection
  if(Clusters != NULL) delete [] Clusters;
  Clusters = NULL;


  for (std::vector<AnaP0DEvent*>::iterator it=AlternateEvents.begin();it!=AlternateEvents.end();it++)
    delete *it;

  AlternateEvents.clear();
}

//********************************************************************
void anaUtils::CopyArray(AnaP0DCluster** const &src, AnaP0DCluster** &dest, int n){
//********************************************************************

    for(int i = 0; i < n; ++i){
        dest[i] = src[i];
    }
    return;
}

//********************************************************************
void anaUtils::CreateArray(AnaP0DCluster** &tgtArr, int nObj){
//********************************************************************

    tgtArr = new AnaP0DCluster*[nObj];
    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = NULL;
    }
}

//********************************************************************
void anaUtils::CopyArray(AnaP0DHit** const &src, AnaP0DHit** &dest, int n){
//********************************************************************

    for(int i = 0; i < n; ++i){
        dest[i] = src[i];
    }
    return;
}

//********************************************************************
void anaUtils::CreateArray(AnaP0DHit** &tgtArr, int nObj){
//********************************************************************

    tgtArr = new AnaP0DHit*[nObj];
    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = NULL;
    }
}

//********************************************************************
EventBoxP0D2::EventBoxP0D2():EventBoxB(){
//******************************************************************** 

    for (UInt_t i=0;i<NMAXVERTEXGROUPS;i++){
      nVerticesInGroup[i]=0;
      VerticesInGroup[i]=NULL;
    }
}

//********************************************************************
EventBoxP0D2::~EventBoxP0D2(){
//********************************************************************
  for (UInt_t i=0;i<NMAXVERTEXGROUPS;i++){
    if (VerticesInGroup[i])
      delete [] VerticesInGroup[i];
    VerticesInGroup[i] = NULL;
  }
} 

