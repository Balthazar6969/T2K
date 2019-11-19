#ifndef TDISPLAYEDGLOBALMODULE_HXX
#define  TDISPLAYEDGLOBALMODULE_HXX
#include "TChain.h"
#include "TND280Track.hxx"
#include "TEveBoxSet.h"
#include "TND280Shower.hxx"
#include "TND280TrackPropagator.hxx"
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TDisplayedAnalysisModuleBase.hxx"
#include "TGlobalAnalysisUtils.hxx"
#endif 
/// Display the Global Module, used by EventDisplay.
class TDisplayedGlobalModule : public TDisplayedAnalysisModuleBase
{
    
    
    
    
public:
    /// Constructor.
    TDisplayedGlobalModule();  
    /// Draw entry.
    void Draw(int entry,int bunch=-2);
    /// Centre view on RO.
    void    Centre(ND::TGlobalReconModule::TGlobalPID* RO);
    /// Describe global track number index on standard out.
    void Describe(int index);
    /// Describe global PID  on standard out.
    void Describe( ND::TGlobalReconModule::TGlobalPID* RO);
    /// Describe P0DObject  on standard out.
    void Describe( ND::TGlobalReconModule::TP0DObject* P0D );
    /// Describe FGDObject  on standard out.
    void Describe( ND::TGlobalReconModule::TFGDObject* FGD );
    /// Describe TPCObject  on standard out.
    void Describe( ND::TGlobalReconModule::TTPCObject* TPC );
    /// Describe ECALObject  on standard out.
    void Describe( ND::TGlobalReconModule::TECALObject* ECAL );
    /// Describe SMRDObject  on standard out.
    void Describe( ND::TGlobalReconModule::TSMRDObject* SMRD );
    /// Describe Shower called Name  on standard out.
    void DescribeShower(TString Name);
    /// Describe Verted object on standard output.
    void Describe(ND::TGlobalReconModule::TGlobalVertex* Vertex);
    /// Describe vertex constituent on stdout.
    void Describe(ND::TGlobalReconModule::TVertexConstituent* RO);
/// Describe a true vertex on stdout.
    void Describe(ND::TTrueVertex* RO);


private:
    /// Returns TND280Track corresponding to reconObject. 
    /// Positions along the track are taken from detector entrance and exit positions.
    TND280Track* MakeTrack(TND280TrackPropagator*   trkProp,
    	Int_t                  index,
    	ND::TGlobalReconModule::TGlobalPID* reconObject);
    /// Determine how close two hits are in either x,y, or z 
    /// depending on their relative orientation. e.g. For an xy hit and an xz 
    /// hit the measurement is in x direction.
    float Closeness(
    ND::TGlobalReconModule::TGlobalHit*  hit1,
    ND::TGlobalReconModule::TGlobalHit*  hit2
    );
    /// Add hits from GlobalReconModuel object to corresponding TND280Track object.
    void AddHits(ND::TGlobalReconModule::TGlobalPID* reconObject,TND280Track* track);
/// Create a box from hit and add it to hits.
    void AddBoxToBoxSet(TEveBoxSet* hits ,ND::TGlobalReconModule::TGlobalHit* hit);
/// Create a box from position and extent  and add it to hits.
    void AddBoxToBoxSet(TVector3 position , TVector3 extent,TEveBoxSet* hits);    
    
/// Loop over the TClonesArray of ECALobjects associated to this PID and add them as daughters to track in the Eve structure.
    void AddEcalObjects(ND::TGlobalReconModule::TGlobalPID* reconObject, TEveElement* track ,bool hasTrack,int index );
/// Loop over the TClonesArray of P0Dobjects associated to this PID and add them as daughters to track in the Eve structure.
    void AddP0DObjects(ND::TGlobalReconModule::TGlobalPID* reconObject, TEveElement* track ,bool hasTrack );
/// Loop over the TClonesArray of SMRDobjects associated to this PID and add them as daughters to track in the Eve structure.
    void AddSMRDObjects(ND::TGlobalReconModule::TGlobalPID* reconObject, TEveElement* track  );
/// Loop over the TClonesArray of TPCobjects associated to this PID and add them as daughters to track in the Eve structure.
    void AddTPCObjects(ND::TGlobalReconModule::TGlobalPID* reconObject, 
    	TEveElement* track ,
    	TND280TrackPropagator*   trkProp );
    void AddFGDObjects(ND::TGlobalReconModule::TGlobalPID* reconObject, TEveElement* track  );
/// \returns a TND280Shower to represent the cone which describes a shower specified by its position, directon angle and length.
    TND280Shower* CreateShower( TEveVector Direction,TEveVector Position,TVector3  angle3,float length) ;  
/// \returns a TEveLine from FrontPosition to BackPosition of object and named name+" associated to global reconstructed object "
    TEveLine* AddLine(ND::TSubBaseObject* object,TString name);
/// Change one component of position to that of position2 depending on type.
/// Type 110, set x position.
/// Type 101, set y position.
/// Type  11, set z position.
/// Used with pairs of hits to take the unknown coordinate from the matching hit in the "other" orientation.
    void  AdjustPosition(TVector3& position,int Type,TVector3 position2);
/// Add Global Vertices to the eve list
    void DrawVertices(TClonesArray* Vertices,int NVertices,TND280TrackList* gVertexList);
/// Draw a single vertex
  TND280Track* MakeVertex(TND280TrackPropagator*   trkProp,
    Int_t                  index,
    ND::TGlobalReconModule::TGlobalVertex* reconObject) ;
};
#endif
