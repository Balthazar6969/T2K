#include "TND280Track.hxx"
#include "TEventDisplay.hxx"
#include <iostream>
#include "TDisplayedTrackerModule.hxx" 
ClassImp(TND280Track)
void TND280Track::FillImpliedSelectedSet(Set_t& impSelSet)
{
    TEveTrack::FillImpliedSelectedSet(impSelSet);
    
    for (List_ci i=fChildren.begin(); i!=fChildren.end(); ++i)
    {
    	impSelSet.insert(*i);
    }
}
//______________________________________________________________________________
void TND280Track::MakeTrack(Bool_t recurse)
{
   // Calculate track representation based on track data and current
   // settings of the propagator.
   // If recurse is true, descend into children.

   if (!fLockPoints)
   {
      Reset(0);
      fLastPMIdx = 0;

      TND280TrackPropagator& rTP((fND280Propagator != 0) ? *fND280Propagator : TND280TrackPropagator::fgND280Default);

      const Double_t maxRsq = rTP.GetMaxR() * rTP.GetMaxR();
      const Double_t maxZ   = rTP.GetMaxZ();

      if ( ! TEveTrackPropagator::IsOutsideBounds(fV, maxRsq, maxZ))
      {
         TEveVectorD currP = fP;
         Bool_t decay = kFALSE;
         fND280Propagator->InitTrack(fV, fCharge);
         for (vPathMark_i pm = fPathMarks.begin(); pm != fPathMarks.end(); ++pm, ++fLastPMIdx)
         {
            if (rTP.GetFitReferences() && pm->fType == TEvePathMarkD::kReference)
            {
               if (TEveTrackPropagator::IsOutsideBounds(pm->fV, maxRsq, maxZ))
                  break;
               // printf("%s fit reference  \n", fName.Data());
               if (fND280Propagator->GoToVertex(pm->fV, currP)) {
                  currP.fX = pm->fP.fX; currP.fY = pm->fP.fY; currP.fZ = pm->fP.fZ;
               }
               else
               {
                  break;
               }
            }
            else if (rTP.GetFitDaughters() && pm->fType == TEvePathMarkD::kDaughter)
            {
               if (TEveTrackPropagator::IsOutsideBounds(pm->fV, maxRsq, maxZ))
                  break;
               // printf("%s fit daughter  \n", fName.Data());
               if (fND280Propagator->GoToVertex(pm->fV, currP)) {
                  currP.fX -= pm->fP.fX; currP.fY -= pm->fP.fY; currP.fZ -= pm->fP.fZ;
               }
               else
               {
                  break;
               }
            }
            else if (rTP.GetFitDecay() && pm->fType == TEvePathMarkD::kDecay)
            {
               if (TEveTrackPropagator::IsOutsideBounds(pm->fV, maxRsq, maxZ))
                  break;
               // printf("%s fit decay \n", fName.Data());
               fND280Propagator->TND280TrackPropagator::GoToVertex(pm->fV, currP);
               decay = kTRUE;
               ++fLastPMIdx;
               break;
            }
            else if (rTP.GetFitCluster2Ds() && pm->fType == TEvePathMarkD::kCluster2D)
            {
               TEveVectorD itsect;
               if (fND280Propagator->IntersectPlane(currP, pm->fV, pm->fP, itsect))
               {
                  TEveVectorD delta   = itsect - pm->fV;
                  TEveVectorD vtopass = pm->fV + pm->fE*(pm->fE.Dot(delta));
                  if (TEveTrackPropagator::IsOutsideBounds(vtopass, maxRsq, maxZ))
                     break;
                  if ( ! fND280Propagator->GoToVertex(vtopass, currP))
                     break;
               }
               else
               {
                  Warning("TEveTrack::MakeTrack", "Failed to intersect plane for Cluster2D. Ignoring path-mark.");
               }
            }
            else
            {
               if (TEveTrackPropagator::IsOutsideBounds(pm->fV, maxRsq, maxZ))
                  break;
            }            
         } // loop path-marks

         if (!decay)
         {
            // printf("%s loop to bounds  \n",fName.Data() );
            fND280Propagator->GoToBounds(currP);
         }
         fPEnd = currP;
         //  make_polyline:
         fND280Propagator->FillPointSet(this);
         fND280Propagator->ResetTrack();
      }
   }

   if (recurse)
   {
      for (List_i i=fChildren.begin(); i!=fChildren.end(); ++i)
      {
         TEveTrack* t = dynamic_cast<TEveTrack*>(*i);
         if (t) t->MakeTrack(recurse);
      }
   }
}

#ifdef ADDDESCRIBE
void TND280Track::Describe()
{
  TEventDisplay::Get()->OnClicked(this);
}
#endif

