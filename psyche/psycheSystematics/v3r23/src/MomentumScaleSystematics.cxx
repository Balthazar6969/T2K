#include "MomentumScaleSystematics.hxx"
#include "ND280AnalysisUtils.hxx"
#include "EventBoxTracker.hxx"
#include "Parameters.hxx"

//#define DEBUG

  
//********************************************************************
MomentumScaleSystematics::MomentumScaleSystematics():EventVariationBase(1),BinnedParams("MomentumScale",k1D_SYMMETRIC_NOMEAN){
//********************************************************************

  _useP0DFV = (bool) ND::params().GetParameterI("psycheSystematics.Tracker.UseP0DFV");

  // Get the systematic source values 
  GetSigmaValueForBin(0, _scaleError);
}

//********************************************************************
void MomentumScaleSystematics::Apply(const ToyExperiment& toy, AnaEventC& event){
//********************************************************************

  // Get the SystBox for this event
  SystBoxB* box = GetSystBox(event);
 
#ifdef DEBUG
    std::cout << "MomentumScaleSystematics::ApplyVariation(): " << box->nRelevantRecObjects <<" and? "<<box->nRelevantRecObjects<< std::endl;  
#endif
  if (_useP0DFV) {
    ApplyP0DTPC(toy,event);
    return;
  }

  // loop over the relevant tracks for this systematic
  for (Int_t itrk=0;itrk<box->nRelevantRecObjects;itrk++){
    AnaTrackB* track = static_cast<AnaTrackB*>(box->RelevantRecObjects[itrk]);

#ifdef DEBUG
      std::cout << itrk << " --> p0 = " << track->Momentum << std::endl;
#endif

    // Apply the momentum scale factor
    track->Momentum     *= (1 + _scaleError * toy.GetToyVariations(_index)->Variations[0]);

    // Apply the momentum scale factor for the flip momentum
    track->MomentumFlip *= (1 + _scaleError * toy.GetToyVariations(_index)->Variations[0]);

#ifdef DEBUG
    std::cout << "p = " << p << std::endl;
#endif
  }
}

//********************************************************************
void MomentumScaleSystematics::ApplyP0DTPC(const ToyExperiment& toy, AnaEventC& event)
//********************************************************************
{
  SystBoxB* box = GetSystBox(event);

#ifdef DEBUG
    std::cout << "MomentumScaleSystematics::ApplyVariation(): " << box->nRelevantRecObjects <<" and? "<<toy.Variations[0]<< std::endl;  
#endif

  // loop over the relevant tracks for this systematic
  for (Int_t itrk=0;itrk<box->nRelevantRecObjects;itrk++){
    AnaTrackB* track = static_cast<AnaTrackB*>(box->RelevantRecObjects[itrk]);
    AnaP0DParticleB* p0dTrack = track->P0DSegments[0];
    if (!p0dTrack || p0dTrack->ELoss < 0) continue;

    double trackerMomentum = track->Momentum - p0dTrack->ELoss;

#ifdef DEBUG
      std::cout << itrk << " --> p0 = " << track->Momentum << std::endl;
#endif

    // Apply the momentum scale factor. This is modified from the
    // default MomentumScaleSystematics to scale only the
    // TrackerMomentum component.
    track->Momentum     = track->Momentum     + trackerMomentum * (_scaleError*toy.GetToyVariations(_index)->Variations[0]);

    track->MomentumFlip = track->MomentumFlip + trackerMomentum * (_scaleError*toy.GetToyVariations(_index)->Variations[0]);
#ifdef DEBUG
    std::cout << "p = " << track->Momentum << std::endl;
#endif
  }


}

//********************************************************************
bool MomentumScaleSystematics::UndoSystematic(AnaEventC& event){
//********************************************************************

  // Get the SystBox for this event
  SystBoxB* box = GetSystBox(event);

  for (Int_t itrk=0;itrk<box->nRelevantRecObjects;itrk++){
    AnaTrackB* track = static_cast<AnaTrackB*>(box->RelevantRecObjects[itrk]);
    // Go back to the corrected momentum
    track->Momentum     = track->GetOriginalTrack()->Momentum;
    
    track->MomentumFlip = track->GetOriginalTrack()->MomentumFlip;
  }

  // Don't reset the spill to corrected
  return false;
}

