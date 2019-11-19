#include "FGDPIDSystematics.hxx"
#include "ND280AnalysisUtils.hxx"
#include "EventBoxTracker.hxx"
#include "VersioningUtils.hxx"
#include "SystematicUtils.hxx"

//#define DEBUG

//********************************************************************
FGDPIDSystematics::FGDPIDSystematics():EventVariationBase(2){
  //********************************************************************


  // Read the systematic source parameters from the data files
  Int_t npar=0;
  _fgd1mean  = new BinnedParams("FGD1PIDMean",    BinnedParams::k1D_EFF_SYMMETRIC, versionUtils::Extension());
  _fgd1sigma = new BinnedParams("FGD1PIDSigRatio",BinnedParams::k1D_SYMMETRIC,     versionUtils::Extension());
  npar=_fgd1mean->GetNBins()+_fgd1sigma->GetNBins();
  if( versionUtils::prod6_systematics){
    _fgd2mean  =  new BinnedParams("FGD2PIDMean",    BinnedParams::k1D_EFF_SYMMETRIC, versionUtils::Extension());
    _fgd2sigma =  new BinnedParams("FGD2PIDSigRatio",BinnedParams::k1D_SYMMETRIC,     versionUtils::Extension());
    npar+=_fgd2mean->GetNBins()+_fgd2sigma->GetNBins();
  }

  //set the total number of parameters
  SetNParameters(npar);
}

//********************************************************************
void FGDPIDSystematics::Initialize(){
  //********************************************************************

  // Register the BinnedParams to have proper offsets for each of them
  systUtils::AddBinnedParamsOffsetToSystematic(*this, *_fgd1mean, _fgd1mean->GetNBins());
  systUtils::AddBinnedParamsOffsetToSystematic(*this, *_fgd1sigma, _fgd1sigma->GetNBins());

  if( versionUtils::prod6_systematics){
    systUtils::AddBinnedParamsOffsetToSystematic(*this, *_fgd2mean, _fgd2mean->GetNBins());
    systUtils::AddBinnedParamsOffsetToSystematic(*this, *_fgd2sigma, _fgd2sigma->GetNBins()); 
  }

}
//********************************************************************
void FGDPIDSystematics::Apply(const ToyExperiment& toy, AnaEventC& event){
  //********************************************************************

  // Get the SystBox for this event
  SystBoxB* box = GetSystBox(event);

#ifdef DEBUG
  std::cout << "FGDPIDSystematics::Apply(): tracks size " << box->nRelevantRecObjects << std::endl;  
#endif

  // Loop over relevant tracks for this systematic
  for (Int_t itrk=0;itrk<box->nRelevantRecObjects;itrk++){
    AnaTrackB* track = static_cast<AnaTrackB*>(box->RelevantRecObjects[itrk]);
    if(!track->TrueObject) continue;
    Int_t pdg;
    if (abs(track->GetTrueParticle()->PDG) == 211 || abs(track->GetTrueParticle()->PDG) == 13)
      pdg = 13;
    else if (abs(track->GetTrueParticle()->PDG) == 2212)
      pdg = 2212;
    else
      continue;


    // Loop over FGD segments
    for (int k = 0; k < track->nFGDSegments; k++) {
      // The segment to be modified
      AnaFGDParticleB* fgdTrack = track->FGDSegments[k];
      if(!fgdTrack) continue;
      // The original (corrected) FGD track 
      const AnaFGDParticleB* original = static_cast<const AnaFGDParticleB*>(fgdTrack->Original);
      // The original should exist
      if (!original)   continue;

#ifdef DEBUG
      std::cout << pdg<<" FGDPIDSystematics::Apply(): nodes = " << fgdTrack->NNodes << std::endl;  	    
#endif	   
      // Get the systematic source values
      Float_t sigR_value;
      Float_t sigR_error;
      Int_t mean_index;
      Int_t sigR_index;
      BinnedParamsParams params;

      bool inFGD1 = SubDetId::GetDetectorUsed(track->Detector, SubDetId::kFGD1);
      bool inFGD2 = SubDetId::GetDetectorUsed(track->Detector, SubDetId::kFGD2);

      if (inFGD1){
        if(!_fgd1mean->GetBinValues((Float_t)pdg,params,  mean_index))  continue;
        
        mean_index += systUtils::GetBinnedParamsOffsetForSystematic(*this, *_fgd1mean);

        
        if(!_fgd1sigma->GetBinValues((Float_t)pdg,sigR_value, sigR_error, sigR_index)) continue;
        sigR_index += systUtils::GetBinnedParamsOffsetForSystematic(*this, *_fgd1sigma);
      }
      else if(inFGD2 && versionUtils::prod6_systematics){
          if(!_fgd2mean->GetBinValues((Float_t)pdg,params,  mean_index))  continue;
          
          mean_index += systUtils::GetBinnedParamsOffsetForSystematic(*this, *_fgd2mean);
          
          if(!_fgd2sigma->GetBinValues((Float_t)pdg,sigR_value, sigR_error, sigR_index)) continue;
          sigR_index += systUtils::GetBinnedParamsOffsetForSystematic(*this, *_fgd2sigma);  
      }
      else
        continue;

#ifdef DEBUG
      std::cout << "FGDPIDSystematics::Apply(): meanmc,  menadata, mean_index  = " << params.meanMC  << " " <<  params.meanDATA  << " " << mean_index << std::endl;  	     
      std::cout << "FGDPIDSystematics::Apply(): sigR_value, sigR_error, sigR_index = " << sigR_value << " " <<  sigR_error  << " " << sigR_index <<std::endl;  
#endif
      // Apply the variation

      // value = pull_mean      
      // Summing value to pullpi0 is effectively a correction, to center the pull at 0
      // Summing error+x is the propagation of the systematic
      Float_t diffdataMC=params.meanDATA-params.meanMC;
      Float_t error=fabs(diffdataMC);

      if (pdg==13){
        // systematic on pull mean
        fgdTrack->Pullpi =  fgdTrack->Pullpi+ diffdataMC + error * toy.GetToyVariations(_index)->Variations[mean_index]; 
        fgdTrack->Pullmu =  fgdTrack->Pullmu + diffdataMC + error * toy.GetToyVariations(_index)->Variations[mean_index]; 

        // systematic on pull sigma ratio
        fgdTrack->Pullpi =  params.meanMC + (fgdTrack->Pullpi-params.meanMC)*(sigR_value + 
            sigR_error * toy.GetToyVariations(_index)->Variations[sigR_index]); 
        fgdTrack->Pullmu =  params.meanMC + (fgdTrack->Pullmu-params.meanMC)*(sigR_value + 
            sigR_error * toy.GetToyVariations(_index)->Variations[sigR_index]); 
      }
      else if (pdg==2212){
        // systematic on pull mean
        fgdTrack->Pullp =  fgdTrack->Pullp + diffdataMC + error * toy.GetToyVariations(_index)->Variations[mean_index]; 
        // systematic on pull sigma ratio
        fgdTrack->Pullp =  params.meanMC + (fgdTrack->Pullp - params.meanMC)*(sigR_value + sigR_error * 
            toy.GetToyVariations(_index)->Variations[sigR_index]); 
      }

#ifdef DEBUG

      // Get the original pulls
      Float_t pullpi0 = original->Pullpi;
      Float_t pullmu0 = original->Pullmu;
      Float_t pullp0  = original->Pullp;
      std::cout << "FGDPIDSystematics::Apply(): Results: " << std::endl;  
      std::cout << " " << track->GetTrueParticle()->PDG << " " << fgdTrack 
        << ", " << pullpi0 << " --> " <<  fgdTrack->Pullpi 
        << ", " << pullmu0 << " --> " <<  fgdTrack->Pullmu 
        << ", " << pullp0 << " --> "  <<  fgdTrack->Pullp  << std::endl;
#endif
    }
  }
}

//********************************************************************
bool FGDPIDSystematics::UndoSystematic(AnaEventC& event){
  //********************************************************************

  // Get the SystBox for this event
  SystBoxB* box = GetSystBox(event);

  for (int itrk=0;itrk<box->nRelevantRecObjects;itrk++){
    for (int k = 0; k < static_cast<AnaTrackB*>(box->RelevantRecObjects[itrk])->nFGDSegments; k++) {
      // The new FGD track
      AnaFGDParticleB* fgdTrack = static_cast<AnaTrackB*>(box->RelevantRecObjects[itrk])->FGDSegments[k];
      // The original (corrected) fgd track
      const AnaFGDParticleB* original = static_cast<const AnaFGDParticleB*>(fgdTrack->Original);

      // revert to initial pulls
      fgdTrack->Pullpi = original->Pullpi;
      fgdTrack->Pullmu = original->Pullmu;
      fgdTrack->Pullp  = original->Pullp;
    }
  }

  // Don't reset the spill to corrected
  return false;
}


//**************************************************
bool FGDPIDSystematics::IsRelevantRecObject(const AnaEventC& event, const AnaRecObjectC& recObj) const{
  //**************************************************

  (void)event;

  // True track should always exist
  if (!recObj.TrueObject) return false;

  const AnaTrueParticleB* truePart = static_cast<const AnaTrueParticleB*>(recObj.TrueObject);

  bool ok = false;
  // only consider true protons, pions, muons and electrons
  if      (abs(truePart->PDG) == 211 ) ok = true;      
  else if (abs(truePart->PDG) == 2212) ok = true;
  else if (abs(truePart->PDG) == 13)   ok = true;

  return ok;
}
