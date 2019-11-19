// @(#)root/eve:$Id: TND280TrackPropagator.hxx,v 1.1 2016/04/08 13:23:04 lp208 Exp $
// Authors: Matevz Tadel & Alja Mrak-Tadel: 2006, 2007

/*************************************************************************
* Copyright (C) 1995-2007, Rene Brun and Fons Rademakers.               *
* All rights reserved.                                                  *
*                                                                       *
* For the licensing terms see $ROOTSYS/LICENSE.                         *
* For the list of contributors see $ROOTSYS/README/CREDITS.             *
*************************************************************************/

#ifndef ROOT_TND280TrackPropagator
#define ROOT_TND280TrackPropagator

#include "TEveTrackPropagator.h"
#include <iostream>
using std::cout;
using std::endl;

//==============================================================================
/// A special propagator class to allow overloading LoopToVertex. This
///  takes care of cases where standard Eve propogator misses end vertex
/// and keeps going to the edge of the detector. When this happens we go back
/// to the point of closest approach.
//==============================================================================

class TND280TrackPropagator : public TEveTrackPropagator
{
private:
    //  TND280TrackPropagator(){};  
public:
    TND280TrackPropagator(const char* n="TND280TrackPropagator", const char* t="",TEveMagField *field=0, Bool_t own_field=kTRUE): 
    TEveTrackPropagator(n,t,field,own_field) {}
    /// This
    ///  takes care of cases where standard Eve propogator misses end vertex
    /// and keeps going to the edge of the detector. When this happens we go back
    /// to the point of closest approach.
    Bool_t LoopToVertex(TEveVectorD& v, TEveVectorD& p);
    Bool_t GoToVertex(TEveVectorD& v, TEveVectorD& p);
    Bool_t GoToVertex(TEveVectorF& v, TEveVectorF&p);
public:
    static TND280TrackPropagator  fgND280Default;     // Default track propagator.
    ClassDef(TND280TrackPropagator, 0); // Calculates path of a particle taking into account special path-marks and imposed boundaries.
};

#endif
