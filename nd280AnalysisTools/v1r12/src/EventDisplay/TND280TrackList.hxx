#ifndef TND280TrackList_HXX
#define TND280TrackList_HXX

#include "TEveTrack.h"
#include <iostream>
using std::cout;
using std::endl;
#include "TND280TrackPropagator.hxx"
/// An ND280  track list displayed in Eve.
///Class to add additional functions to a EveTrackList.
/// Also allows for use of a TND280TrackPropogator.
///\author Alex Finch (A.Finch@lancaster.ac.uk)
class TND280TrackList: 
public TEveTrackList
{
    TND280TrackPropagator* fND280Propagator;
private:
    
public:
    TND280TrackList(const char* n = "TND280TrackList",TND280TrackPropagator* prop=0)
    : TEveTrackList( n ) ,fND280Propagator(0)
    { if (prop == 0) prop = new TND280TrackPropagator("propogator");
    SetPropagator(prop);};        
    /// When this item is selected, select all its daughters.
    void FillImpliedSelectedSet(Set_t& impSelSet);
    TND280TrackPropagator* GetPropagator(){ return fND280Propagator; }
    void SetPropagator(TND280TrackPropagator* prop);

private:
    ClassDef(TND280TrackList, 0)  
    
};
#endif

