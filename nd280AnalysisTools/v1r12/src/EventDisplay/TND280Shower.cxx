#include "TND280Shower.hxx"
#include "TEventDisplay.hxx"
#include <iostream>
#include "TDisplayedTrackerModule.hxx" 
ClassImp(TND280Shower)
void TND280Shower::FillImpliedSelectedSet(Set_t& impSelSet)
{
    TEveArrow::FillImpliedSelectedSet(impSelSet);
    
    for (List_ci i=fChildren.begin(); i!=fChildren.end(); ++i)
    {
    	impSelSet.insert(*i);
    }
}
#ifdef ADDDESCRIBE
void TND280Shower::Describe()
{
  TEventDisplay::Get()->OnClicked(this);
}
#endif
 
