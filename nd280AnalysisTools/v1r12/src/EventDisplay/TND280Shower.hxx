#ifndef TND280Shower_HXX
#define TND280Shower_HXX
#include "TEveArrow.h"
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TTrackerAnalysisUtils.hxx"
#endif 
//#define ADDDESCRIBE
/// An ND280  shower displayed in Eve.
///Class to add additional functions to an EveArrow.
///\author Alex Finch (A.Finch@lancaster.ac.uk)
class TND280Shower: 
public TEveArrow
{
    
private:
    
public:
    /// 6 float constructor, can all be missing though!
    TND280Shower(Float_t xVec = 0, Float_t yVec = 0, Float_t zVec = 1, Float_t xOrg = 0, Float_t yOrg = 0, Float_t zOrg = 0) :
    TEveArrow(xVec,yVec,zVec,xOrg,yOrg,zOrg){};
    /// When this item is selected, select all its daughters.
    void FillImpliedSelectedSet(Set_t& impSelSet);
#ifdef ADDDESCRIBE
    /// Describe this object when selected from context menu.
    void Describe(); // *MENU*
#endif
private:
    TND280Shower(const TND280Shower& t);
    ClassDef(TND280Shower, 0)  
    
};
#endif

