#include "tutorialVariationSystematics.hxx"
#include "EventBoxTracker.hxx"
#include "ND280AnalysisUtils.hxx"

/*
  A variation systematic is run before the selection, modifying the input data. The selection will then proceed 
  on that modified data, such that we can test the effect of any source variation on the final number of selected events and on 
  the distributions of any observable. As advanced, a variation systematic could have two effects:
  - Change the number of selected events because a variable we can on (or depends on it) is varied.
  - Change the value of an observable. 

  In this particular example we scale the momentum of all tracks, this will have a small effect on the number of selected events 
  (through the PID cut, which depends on momentum), because there is no direct cut on momentum, but a large effect on the distribution of 
  the muon candidate momentum

*/


//********************************************************************
tutorialVariationSystematics::tutorialVariationSystematics():EventVariationBase(1),BinnedParams("MomentumScale",k1D_SYMMETRIC){
//********************************************************************

  /* A variation systematic inherits from two base classes:
     - EventVariationBase: The 1 in the constructor is the number of systematic parameters (one in this case).
     - BinnedParams: Allows dealing with the source parameters to be propagated (reads the .dat file, etc). In this particular case the 
       .dat file will be MomentumScale.dat under psycheSystematics/vXrY/data. The name of the systematic data file is the one given to the 
       BinnedParams constructor (above). The second argument in the constructor is the type of systematics source. Thse are the available types:

       For standard weight systematics: 
       k1D_SYMMETRIC:       the systematic depends on a single observable (i.e. momentum) and its error is symmetric
       k2D_SYMMETRIC:       the systematic depends on two observables (i.e. momentum and angle) and its error is symmetric
       k3D_SYMMETRIC:       the systematic depends on three observables and its error is symmetric
       k1D_SYMMETRIC_NOMEAN:   the systematic depends on a single observable (i.e. momentum) and its error is symmetric. The correction is not specified
       k2D_SYMMETRIC_NOMEAN:   the systematic depends on two observables (i.e. momentum and angle) and its error is symmetric. The correction is not specified
       k3D_SYMMETRIC_NOMEAN:   the systematic depends on three observables and its error is symmetric. The correction is not specified


       For efficiency-like weight systematics:
       k1D_EFF_SYMMETRIC:   the systematic depends on a single observable (i.e. momentum) and its error is symmetric
       k2D_EFF_SYMMETRIC,   the systematic depends on two observables (i.e. momentum and angle) and its error is symmetric
       k3D_EFF_SYMMETRIC,   the systematic depends on three observables and its error is symmetric
       k1D_EFF_ASSYMMETRIC: the systematic depends on a single observable (i.e. momentum) and its error is assymmetric
       k2D_EFF_ASSYMMETRIC: the systematic depends on two observables (i.e. momentum and angle) and its error is assymmetric
       k3D_EFF_ASSYMMETRIC, the systematic depends on three observables and its error is assymmetric

       Efficiency-like weight systematics need the values for data and MC control samples (with errors) and also the nominal for the MC analysis sample
   */

  // Read the parameters for bin 0. This particular systematic has only one bin, becaouse we consider the same 
  // momentum scale systematic for all phase space (momentum, angle, etc)
  // Since this is a 1D sysmetric systematisc source only two parameters should be read for each bin: 
  // the mean (_scale) and its error (_scaleError).
  GetParametersForBin(0, _scale, _scaleError);
}

//********************************************************************
void tutorialVariationSystematics::Apply(const ToyExperiment& toy, AnaEventC& event){
//********************************************************************

  /*
    This method takes two arguments:
    - ToyExperiment: Contains the variation to be applied (in number of sigmas)
    - AnaEventC: Sometimes we need extra information not present in the systematics box


    In addition we need the SystBox: 
    - SystBoxB: Sometimes the information needed to compute the systematic is the same for all toys (in this case all tracks with a TPC segment 
       and starting in the FGD1 FV). The operation of getting that set of tracks is time comsuming. In order not to repeat that opperation for each toy
       it is better to do it only once for each event. This information is stored in this systematics box
  */


  // Get the SystBox for this event
  SystBoxB* box = GetSystBox(event);
 
#ifdef DEBUG
    std::cout << "MomentumScaleSystematics::ApplyVariation(): " << box->nRelevantRecObjects <<" and? "<<box->nRelevantRecObjects<< std::endl;  
#endif

  // loop over the relevant tracks for this systematic
  for (Int_t itrk=0;itrk<box->nRelevantRecObjects;itrk++){
    AnaTrackB* track = static_cast<AnaTrackB*>(box->RelevantRecObjects[itrk]);

#ifdef DEBUG
      std::cout << itrk << " --> p0 = " << track->Momentum << std::endl;
#endif

    // Apply the momentum scale factor
    track->Momentum *=(1+_scale +_scaleError*toy.GetToyVariations(_index)->Variations[0]);

#ifdef DEBUG
    std::cout << "p = " << p << std::endl;
#endif
  }
}

//********************************************************************
bool tutorialVariationSystematics::UndoSystematic(AnaEventC& event){
//********************************************************************

  /* This method is very important. When this method returns true, the whole AnaSpill class is reseted such the all possible variations 
     are undone (we get the spill as it was after corrections). This is however very time consuming since the AnaSpill class is a very big one. 

     To speed up this action we can move back to its original value only the variables that were modified. Each systematic variation will do that operation 
     for the variables modified by it.     
  */

  // Get the SystBox for this event
  SystBoxB* box = GetSystBox(event);

  // Get all relevant tracks for this systematic
  AnaRecObjectC** recObjs = box->RelevantRecObjects;

  for (Int_t itrk=0;itrk<box->nRelevantRecObjects;itrk++){
    AnaTrackB* track = static_cast<AnaTrackB*>(recObjs[itrk]);
    
    // Go back to the corrected momentum
    track->Momentum = track->GetOriginalTrack()->Momentum;
  }

  // Don't reset the spill to corrected
  return false;

}

//********************************************************************
Int_t tutorialVariationSystematics::GetRelevantRecObjectGroups(const SelectionBase& sel, Int_t* IDs) const{
//********************************************************************

  /*
    In this method we specify what are the trackGroups that are relevant for this systematic. This depends on the detectorFV specified in the selection
    Using track groups we avoid looping over all tracks to select a portion of them for every toy, which would be very expensive in CPU time
  */

  SubDetId_h det = sel.GetDetectorFV();
  
  if      (det == SubDetId::kFGD1){
    IDs[0] = EventBoxTracker::kTracksWithTPCInFGD1FV;
    return 1;
  }
  else if (det == SubDetId::kFGD2){
    IDs[0] = EventBoxTracker::kTracksWithTPCInFGD2FV;
    return 1;
  }
  else if (det == SubDetId::kFGD){
    IDs[0] = EventBoxTracker::kTracksWithTPCInFGD1FV;
    IDs[1] = EventBoxTracker::kTracksWithTPCInFGD2FV;
    return 2;
  }

  return 0;
}
