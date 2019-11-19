#include "P0DELossScaleSystematics.hxx"
#include "ND280AnalysisUtils.hxx"
#include "BaseDataClasses.hxx"
#include "EventBoxTracker.hxx"
#include <iostream>

const bool debug = false;

//********************************************************************
P0DELossScaleSystematics::P0DELossScaleSystematics():EventVariationBase(1),BinnedParams(){
//********************************************************************

  BinnedParams::SetName("P0DELossScale");
  BinnedParams::SetType(k1D_SYMMETRIC);
  // SetBaseIndex(0);

  char dirname[256];
  sprintf(dirname,"%s/data",getenv("PSYCHESYSTEMATICSROOT"));
  BinnedParams::Read(dirname);
  //  BinnedParams::Print();
  if (debug){
    std::cout <<"Nbins: "<<GetNBins();
    for (int i = 0 ; i < GetNBins(); i++)
    {
      Float_t mean,sigma; 
      GetParametersForBin(i,mean,sigma);
      std::cout<<"Mean, sigma = "<<mean<<" "<<sigma<<std::endl;
      
    }

  }
}

//********************************************************************
void P0DELossScaleSystematics::Apply(const ToyExperiment& toy, AnaEventC& event){
//********************************************************************

  (void)event;


  // Get the relevant tracks for this systematic
  //AnaTrackB** tracks = box.RelevantRecObjects;

  SystBoxB* box = GetSystBox(event);

  // Nothing to do If there are no relevant tracks
  if (box->nRelevantRecObjects == 0) return;

#ifdef DEBUG
  std::cout << "P0DELossScaleSystematics::Apply(): " << std::endl;
#endif 
  // loop over the relevant tracks for this systematic
  for (Int_t itrk=0;itrk<box->nRelevantRecObjects;itrk++){
#ifdef DEBUG
    std::cout << itrk << std::endl;
#endif

    AnaTrackB* track = static_cast<AnaTrackB*>(box->RelevantRecObjects[itrk]);    

#ifdef DEBUG
    std::cout <<"P0D Segments: "<<track->nP0DSegments<<std::endl;
    std::cout <<"TPC Segments: "<<track->nTPCSegments<<std::endl;
#endif
    //At the moment only one P0D track can be saved
    if (!track->nP0DSegments) continue;
    AnaP0DParticleB* p0d = (AnaP0DParticleB*) track->P0DSegments[0];//returns an AnaP0DParticleB*

    // Get a reference to the momentum to be varied
    Float_t& p = track->Momentum;

    Float_t p0dlength = p0d->Length;
    Float_t p0dEloss = p0d->ELoss;
    Float_t scale, scaleError;
    if (p0dEloss < 0 ) continue;
#ifdef DEBUG
      std::cout << "Toy variation: "<<toy.GetToyVariations(_index)->Variations[0]<<std::endl;
      
      std::cout << "p0 = " << p << std::endl;
//      std::cout << "TPC1 Mom = "<<track_pt->TrackerMomentum<<std::endl;
      std::cout <<"P0D Length: "<<p0dlength<<std::endl;
      std::cout <<"P0D Eloss: "<<p0dEloss<<std::endl;
      std::cout <<"Getting bin values:"<<std::endl;
#endif


    if (!GetBinValues(p0dlength,scale,scaleError)) continue;
    //if (!GetParametersForBin(0,scale,scaleError)) continue;
#ifdef DEBUG
      std::cout <<"Bin values retrieved"<<std::endl;
      std::cout << "Scale = "<<scale<<" Scale Err = " <<scaleError<<std::endl;
#endif
    // Apply the momentum scale factor
    p += (scale +scaleError*toy.GetToyVariations(_index)->Variations[0])*(p0dEloss);

#ifdef DEBUG
    std::cout << "p = " << p << std::endl;
#endif
  }
}

//********************************************************************
bool P0DELossScaleSystematics::UndoSystematic(AnaEventC& event){
//********************************************************************

  SystBoxB* box = GetSystBox(event);
  // Get the relevant tracks for this systematic
  AnaRecObjectC** tracks = box->RelevantRecObjects;

  for (Int_t itrk=0;itrk<box->nRelevantRecObjects;itrk++){
    AnaTrackB* track = static_cast<AnaTrackB*>(tracks[itrk]);
    if(!track->Original) continue;
    
    // Go back to the corrected momentum
    track->Momentum = track->GetOriginalTrack()->Momentum;
  }

  // Don't reset the spill to corrected
  return false;
}
