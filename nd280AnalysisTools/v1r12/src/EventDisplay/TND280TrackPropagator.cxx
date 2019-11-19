// @(#)root/eve:$Id: TND280TrackPropagator.cxx,v 1.1 2016/04/08 13:23:04 lp208 Exp $
// Authors: Matevz Tadel & Alja Mrak-Tadel: 2006, 2007

/*************************************************************************
* Copyright (C) 1995-2007, Rene Brun and Fons Rademakers.               *
* All rights reserved.                                                  *
*                                                                       *
* For the licensing terms see $ROOTSYS/LICENSE.                         *
* For the list of contributors see $ROOTSYS/README/CREDITS.             *
*************************************************************************/


#include "TND280TrackPropagator.hxx"
#include "TEveTrans.h"
namespace EventDisplay
{
    const Double_t kStepEps  = 1e-3;
}
TND280TrackPropagator TND280TrackPropagator::fgND280Default;

//______________________________________________________________________________
Bool_t TND280TrackPropagator::LoopToVertex(TEveVectorD& v, TEveVectorD& p)
{
    // Propagate charged particle with momentum p to vertex v.
    // It is expected that Update() with full-update was called before.
    
    const Double_t maxRsq = fMaxR * fMaxR;
    
    TEveVector4D currV(fV);
    TEveVector4D forwV(fV);
    TEveVectorD  forwP(p);
    TEveVectorD  savedP(p);
    
    Int_t first_point = fPoints.size();
    Int_t np          = first_point;
    
    Double_t prod0=0, prod1;
    Double_t saveProd0=0, saveProd1=0;
    float oldDistance=999999;
    float dmin=9999999;
    int minLocation=-1;
    bool skipStretch=false;
    //printf(" \n ");
    do
    {
    	Step(currV, p, forwV, forwP);
    	Update(forwV, forwP);
    	
    //	printf(" check point over vertex %f %f %f - %f %f %f \n",
    //	    v.fX,v.fY,v.fZ,forwV.fX,forwV.fY,forwV.fZ);
    	if (PointOverVertex(v, forwV, &prod1))
    	{
    	  //  printf(" break \n");
    	    break;
    	}
    	
    	if (IsOutsideBounds(forwV, maxRsq, fMaxZ))
    	{
    	  //  printf(" Outside bounds:minimum distance = %f, number of points %d \n",dmin,np);
    	  // If we hit the bounds, then go back to nearest point to vertex we found
    	  if(minLocation>0 && dmin<2.0*oldDistance)
    	    {
    	    	//printf(" minimum distance is less than 2.0 times final, use point at %d ?\n",minLocation);
    	    	np=minLocation;
    	    	fPoints.erase(fPoints.begin()+np,fPoints.end());
    	    //	printf(" np set to %d and fPoints size = %d ",np,fPoints.size());
    	    //	printf(" last point now %f %f %f \n",fPoints.back().fX,fPoints.back().fY,fPoints.back().fZ);
    	    	//	currV=fPoints.back();
    	    	currV.fX=fPoints[fPoints.size()-2].fX;
    	    	currV.fY=fPoints[fPoints.size()-2].fY;
    	    	currV.fZ=fPoints[fPoints.size()-2].fZ;
    	    	currV.fT=fPoints[fPoints.size()-2].fT;
    	    	forwV.fX=fPoints.back().fX;
    	    	forwV.fY=fPoints.back().fY;
    	    	forwV.fZ=fPoints.back().fZ;
    	    	forwV.fT=fPoints.back().fT;
    	    	prod0=saveProd0;
    	    	prod1=saveProd1;
    	    	forwP     = savedP;
    	    	skipStretch=true;
    	    	//printf(" setting current point to %f %f %f %f \n",currV.fX,currV.fY,currV.fZ,currV.fT);
    	    	break;
    	    }
    	    fV = currV;
    	    return kFALSE;
    	}
    	TEveVectorD dv; dv.Sub(v, forwV);
    	// Keep track of the point of closest approach, in case we need 
    	float distance=dv.Mag();
    	//printf(" TND280TrackPropagator 557 old distance %f , new distance %f \n",oldDistance,distance );
    	
    	oldDistance=distance;
    	dmin=fmin(distance,dmin);
    	if(minLocation <0 && distance>dmin){
    	  //  printf(" maybe we should stop as distance is increasing \n");
    	    minLocation=np;
    	  //  printf(" current location is %f %f %f \n",currV.fX,currV.fY,currV.fZ);
    	    saveProd0=prod0;
    	    saveProd1=prod1;
    	    savedP=forwP;
    	    
    	}
    	//printf("  TND280TrackPropagator 550 add point to track %f %f %f \n",forwV.fX,forwV.fY,forwV.fZ);
    	fPoints.push_back(forwV);
    	currV = forwV;
    	p     = forwP;
    	prod0 = prod1;
    	++np;
    } while (np < fNMax);
   // printf(" minimum distance = %f, number of points %d \n",dmin,np);
  /*  if(minLocation>0 && dmin<2.0*oldDistance)
    {
    	printf(" minimum distance is less than 2.0 times final, use point at %d ?\n",minLocation);
    }
  */  
    // make the remaining fractional step
    if (np > first_point)
    {
    	if ((v - currV).Mag() > EventDisplay::kStepEps)
    	{
    	    
    	    if(!skipStretch)
    	    {
    	    	Double_t step_frac = prod0 / (prod0 - prod1);
    	    	if (step_frac > 0)
    	    	{
    	    	    // Step for fraction of previous step size.
    	    	    // We pass 'enforce_max_step' flag to Update().
    	    	    Float_t orig_max_step = fH.fMaxStep;
    	    	    fH.fMaxStep = step_frac * (forwV - currV).Mag();
    	    	    /*
    	    	    printf(" max step set to %f \n",fH.fMaxStep);
    	    	    printf(" update point %f %f %f %f %f %f \n ",currV.fX,currV.fY,currV.fZ,p.fX,p.fY,p.fZ);
    	    	*/
    	    	    Update(currV, p, kTRUE, kTRUE);
    	    	    /*
    	    	    printf(" step  point %f %f %f %f %f %f \n",currV.fX,currV.fY,currV.fZ,p.fX,p.fY,p.fZ);
    	    	    printf("  %f %f %f %f %f %f ",forwV.fX,forwV.fY,forwV.fZ,forwP.fX,forwP.fY,forwP.fZ);
    	    	    */
    	    	    Step(currV, p, forwV, forwP);
    	    	  //  printf(" after step forwV = %f %f %f %f \n",forwV.fX,forwV.fY,forwV.fZ,forwV.fT);
    	    	    p     = forwP;
    	    	    currV = forwV;
    	    	 //   printf("  TND280TrackPropagator: 574 add point to track %f %f %f \n",currV.fX,currV.fY,currV.fZ);
    	    	    
    	    	    fPoints.push_back(currV);
    	    	    ++np;
    	    	    fH.fMaxStep = orig_max_step;
    	    	}
    	    }
    	    else
    	    {
    	    	currV=forwV;
    	    }
    	    
    	    // Distribute offset to desired crossing point over all segment.
    	    
    	    TEveVectorD off(v - currV);
    	    off *= 1.0f / currV.fT;
    	   // printf(" offset is difference ( %f %f %f ) between %f %f %f and %f %f %f times 1 / %f \n",
    	    //	off.fX,off.fY,off.fZ,v.fX,v.fY,v.fZ,currV.fX,currV.fY,currV.fZ,currV.fT);
    	    
    	    // Calculate the required momentum rotation.
    	    // lpd - last-points-delta
    	    TEveVectorD lpd0(fPoints[np-1]);
    	    lpd0 -= fPoints[np-2];
    	    lpd0.Normalize();
    	   // printf(" scaling points by offset %f %f %f \n ",off.fX,off.fY,off.fZ);
    	    for (Int_t i = first_point; i < np; ++i)
    	    {
    	    //	printf(" old point %d %f %f %f %f\n ",i,fPoints[i].fX,fPoints[i].fY,fPoints[i].fZ,fPoints[i].fT);
    	    	fPoints[i] += off * fPoints[i].fT;
    	    //	printf(" new point  %f %f %f %f \n ",fPoints[i].fX,fPoints[i].fY,fPoints[i].fZ,fPoints[i].fT);
    	    }
    	    
    	    TEveVectorD lpd1(fPoints[np-1]);
    	    lpd1 -= fPoints[np-2];
    	    lpd1.Normalize();
    	    
    	    TEveTrans tt;
    	    tt.SetupFromToVec(lpd0, lpd1);
    	    
    	    // TEveVectorD pb4(p);
    	    // printf("Rotating momentum: p0 = "); p.Dump();
    	    tt.RotateIP(p);
    	    // printf("                   p1 = "); p.Dump();
    	    // printf("  n1=%f, n2=%f, dp = %f deg\n", pb4.Mag(), p.Mag(),
    	    //        TMath::RadToDeg()*TMath::ACos(p.Dot(pb4)/(pb4.Mag()*p.Mag())));
    	    
    	    fV = v;
    	   // printf(" \n ");
    	    return kTRUE;
    	}
    }
   // printf("  TND280TrackPropagator 615 add point (target vertex) to track %f %f %f \n",v.fX,v.fY,v.fZ);
    
    fPoints.push_back(v);
    fV = v;
    return kTRUE;
}
//______________________________________________________________________________
Bool_t TND280TrackPropagator::GoToVertex(TEveVectorD& v, TEveVectorD& p)
{
    // Propagate particle with momentum p to vertex v.
    
    Update(fV, p, kTRUE);
    
    if ((v-fV).Mag() < EventDisplay::kStepEps)
    {
    	//printf("  TND280TrackPropagator::GoToVertex add point to track %f %f %f \n ",v.fX,v.fY,v.fZ);
    	fPoints.push_back(v);
    	return kTRUE;
    }
    
    return fH.fValid ? LoopToVertex(v, p) : LineToVertex(v);
}

//______________________________________________________________________________
Bool_t TND280TrackPropagator::GoToVertex(TEveVectorF& v, TEveVectorF& p)
{
    // TEveVectorF wrapper.
    
    TEveVectorD vd(v), pd(p);
    Bool_t result = TND280TrackPropagator::GoToVertex(vd, pd);
    v = vd; p = pd;
    return result;
}

//_________
