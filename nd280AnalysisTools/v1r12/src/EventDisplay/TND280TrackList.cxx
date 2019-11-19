#include "TND280TrackList.hxx"
#include <iostream>
ClassImp(TND280TrackList)
void TND280TrackList::FillImpliedSelectedSet(Set_t& impSelSet)
{
    TEveElementList::FillImpliedSelectedSet(impSelSet);
    
    for (List_ci i=fChildren.begin(); i!=fChildren.end(); ++i)
    {
    	impSelSet.insert(*i);
    }
}
void TND280TrackList::SetPropagator(TND280TrackPropagator* prop)
{
   // Set default propagator for tracks.
   // This is not enforced onto the tracks themselves but this is the
   // propagator that is shown in the TEveTrackListEditor.

   if (fND280Propagator == prop) return;
   if (fND280Propagator) fND280Propagator->DecRefCount();
   fND280Propagator = prop;
   if (fND280Propagator) prop->IncRefCount();
}
