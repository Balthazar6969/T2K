#include "MomentumResolVariation.hxx"
#include "ND280AnalysisUtils.hxx"
#include "EventBoxTracker.hxx"
#include "VersioningUtils.hxx"
#include "ConstituentsUtils.hxx"
#include "CutUtils.hxx"


//#define DEBUG true

//********************************************************************
void MomentumResolVariation::ApplyVariation(AnaTrackB* track, const ToyExperiment& exp){
  //********************************************************************

  if (!track) return;

  // Initial momenum to be varied
  Float_t p = track->Momentum;

  // Get the true momentum
  if (!track->TrueObject) return;

  // Then the change cannot be undone
  if(!track->Original) return;

  
#ifdef DEBUG
  std::cout << " MomentumResolVariation::ApplyVariation() - start " << std::endl;
#endif
  
  Float_t variation;

  if (!GetVariation(track, variation, exp)) return; // for prod6 only depends on coordinates

  Float_t p0_true = track->GetTrueParticle()->Momentum;

#ifdef DEBUG
  std::cout << " MomentumResolVariation::ApplyVariation() p0, p0_true = " << p << " " << p0_true << std::endl;
#endif

  // Get the inverse transverse momentum 
  Float_t pt0_inv = anaUtils::ComputeInversePT(*track);
  
  Float_t pt0_inv_flip = anaUtils::ComputeInversePTFlip(*track);
  
  if (pt0_inv < 0) return;

  // Get the true inverse transverse momentum
  Float_t pt0_inv_true  = anaUtils::ComputeInversePT(*(track->GetTrueParticle())); 

#ifdef DEBUG
  std::cout << " MomentumResolVariation::ApplyVariation() 1/pt0, 1/pt0_true = " << pt0_inv << " " << pt0_inv_true << std::endl;    
  std::cout << " MomentumResolVariation::ApplyVariation() 1/pt0_flip = " << pt0_inv_flip << std::endl;    
  std::cout << " MomentumResolVariation::ApplyVariation() var = " << variation << std::endl;
#endif

  // Apply momentum resolution smearing
  Float_t pt_inv = (1 + variation) * (pt0_inv - pt0_inv_true) + pt0_inv_true;
  
  // Same for the flip,  since we assume this will be used as a correct one when ToF applied...
  Float_t pt_inv_flip = (1 + variation) * (pt0_inv_flip - pt0_inv_true) + pt0_inv_true;
 
  // Convert back to the full momentum
  p = fabs(anaUtils::ComputeMomentumFromInversePT( *(track), pt_inv));

  if (p < 0) return;
   

  // Set the new momentum
  track->Momentum = p;

  Float_t p_flip = fabs(anaUtils::ComputeMomentumFromInversePTFlip( *(track), pt_inv_flip));
  if (p_flip > 0)
    track->MomentumFlip = p_flip;
  
  
  // Apply the smearing to the TPC constituents
  if (track->nTPCSegments == 0) return;


  Float_t p0 = track->GetOriginalTrack()->Momentum;

  // Translate the smearing on the error of the local momentum and its error
  Float_t smearingFact    = (p - p0_true)/(p0 - p0_true);
  Float_t smearingFactMom = p / p0;

  for(int iseg = 0; iseg < track->nTPCSegments; iseg++){
    AnaTPCParticleB* tpcTrackOrig = static_cast<const AnaTrackB*>(track->Original)->TPCSegments[iseg];
    AnaTPCParticleB* tpcTrack     = track->TPCSegments[iseg];
    Float_t          tpcmomerr    = tpcTrackOrig->MomentumError;
    Float_t          tpcmom       = tpcTrackOrig->Momentum;

    tpcTrack->MomentumError = tpcmomerr * smearingFact;
    tpcTrack->Momentum      = tpcmom    * smearingFactMom;
  }

#ifdef DEBUG
  std::cout << " MomentumResolVariation::ApplyVariation() p0 new = "     << track->Momentum     << std::endl;
  std::cout << " MomentumResolVariation::ApplyVariation() p0flip new = " << track->MomentumFlip << std::endl;
#endif 

}


//********************************************************************
void MomentumResolVariation::ApplyVariationTPCBased(AnaTrackB* track, const ToyExperiment& exp){
  //********************************************************************

#ifdef DEBUG
  std::cout << " MomentumResolVariation::ApplyVariationTPCBased() - start " << std::endl;
#endif

  if (!track) return;

  // Get the true momentum
  if (!track->TrueObject) return;

  // Then the change cannot be undone
  if(!track->Original) return;

#ifdef DEBUG
  Float_t p0_true = track->GetTrueParticle()->Momentum;
  std::cout << " MomentumResolVariation::ApplyVariationTPCBased() p0, p0_true = "     << track->Momentum     << " " << p0_true << std::endl;
  std::cout << " MomentumResolVariation::ApplyVariationTPCBased() p0flip, p0_true = " << track->MomentumFlip << " " << p0_true << std::endl;
#endif 

  // Do nothing if there are no  TPC constituents
  if (track->nTPCSegments == 0) return;

  // Smear each constituent separately
  for (int iseg = 0; iseg < track->nTPCSegments; iseg++){
    if (!track->TPCSegments[iseg]) continue;

    // Smear the TPC momentum
    ApplyVariationTPC(track->TPCSegments[iseg], exp);
  }

  // Using the TPC closest to the starting point smear the global momentum
  AnaParticleMomB* tpcStart = static_cast<AnaParticleMomB*>(
      GetClosestTPCSegmentWithGoodDQ(*track, track->PositionStart));

  if (tpcStart)
    // Retrieve the original momentum and apply the differnece to the one of the global track
    if (tpcStart->Original)
      track->Momentum += tpcStart->Momentum - static_cast<const AnaParticleMomB*>(tpcStart->Original)->Momentum ;


  // Using the TPC closest to the end point smear the flip momentum
  AnaParticleMomB* tpcEnd = static_cast<AnaParticleMomB*>(
      GetClosestTPCSegmentWithGoodDQ(*track, track->PositionEnd));
  if (tpcEnd)
    // Retrieve the original momentum and apply the differnece to the one of the global track
    if (tpcEnd->Original)
      track->MomentumFlip += tpcEnd->Momentum - static_cast<const AnaParticleMomB*>(tpcEnd->Original)->Momentum ;

#ifdef DEBUG
  std::cout << " MomentumResolVariation::ApplyVariationTPCBased() p0 new = "     << track->Momentum     << std::endl;
  std::cout << " MomentumResolVariation::ApplyVariationTPCBased() p0flip new = " << track->MomentumFlip << std::endl;
#endif 

}



//********************************************************************
void MomentumResolVariation::ApplyVariationTPC(AnaTPCParticleB* track, const ToyExperiment& exp){
  //********************************************************************

  if (!track) return;

  // Initial momenum to be varied
  Float_t p = track->Momentum;

  // Get the true momentum
  if (!track->TrueObject) return;

  // Then the change cannot be undone
  if(!track->Original) return;

  Float_t variation;

  if (!GetVariationTPC(track, variation, exp)) return; 

  SubDetId::SubDetEnum tpc_det = SubDetId::GetSubdetectorEnum(track->Detector);
  if (!SubDetId::IsTPCDetector(tpc_det)) return;


  AnaTrueParticleB* trueTrack = static_cast<AnaTrueParticleB*>(track->TrueObject);

  if (!trueTrack) return;


  AnaDetCrossingB* cross = anaUtils::GetAnaDetCrossing(trueTrack, tpc_det);

  if (!cross)
    return;

  Float_t p0_true = anaUtils::GetEntranceMomentum(*cross);

#ifdef DEBUG
  std::cout << " MomentumResolVariation::ApplyVariationTPC() p0, p0_true = " << p << " " << p0_true << std::endl;
#endif

  // Get the inverse transverse momentum 
  Float_t pt0_inv = anaUtils::ComputeInversePT(*track);


  if (pt0_inv < 0) return;

  // Get the true inverse transverse momentum
  Float_t pt0_inv_true  = anaUtils::ComputeInversePT(*cross); 

#ifdef DEBUG
  std::cout << " MomentumResolVariation::ApplyVariationTPC() 1/pt0, 1/pt0_true = " << pt0_inv << " " << pt0_inv_true << std::endl;    
  std::cout << " MomentumResolVariation::ApplyVariationTPC() var = " << variation << std::endl;
#endif

  // Apply momentum resolution smearing
  Float_t pt_inv = (1 + variation) * (pt0_inv - pt0_inv_true) + pt0_inv_true;

  // Convert back to the full momentum
  p = fabs(anaUtils::ComputeMomentumFromInversePT( *(track), pt_inv));

  if (p < 0) return;


  // Set the new momentum
  track->Momentum = p;

#ifdef DEBUG
  std::cout << " MomentumResolVariation::ApplyVariationTPC() p_err = " << track->MomentumError << " "  << std::endl;
#endif

  Float_t p0 = static_cast<const AnaParticleMomB*>(track->Original)->Momentum;

  if (p0 - p0_true != 0)
    track->MomentumError *= (p - p0_true)/(p0 - p0_true);

#ifdef DEBUG
  std::cout << " MomentumResolVariation::ApplyVariationTPC() p = " << p << " "  << std::endl;
  std::cout << " MomentumResolVariation::ApplyVariationTPC() p_err = " << track->MomentumError << " "  << std::endl;
#endif

  return;  
}




//********************************************************************
bool MomentumResolVariation::GetXBinnedValues(
    AnaTrackB* track, 
    Float_t& value1, 
    Float_t& value2, 
    Int_t& index1, 
    Int_t& index2, 
    ModeEnum mode){
  //********************************************************************

  if (!track) return false;

  if (!_params) return false;

  // Use a x dependence for the moment for prod6
  Float_t x_zmin = -10000;
  Float_t x_zmax = -10000;
  Float_t zmin   = 10000;
  Float_t zmax   = -10000;

  // Get array of TPC segments, but only one per TPC, the one with more nodes
  AnaTPCParticleB* TPCSegments[3];
  Int_t nTPCSegments = anaUtils::GetOneSegmentPerTPC(track->TPCSegments, track->nTPCSegments, TPCSegments);

  // Loop over TPC segments
  for (int k = 0; k < nTPCSegments; k++) {
    AnaTPCParticleB* tpcTrack = TPCSegments[k];

    // Find the Most upstream TPC point in the global track
    if (tpcTrack->PositionStart[2]<zmin){
      x_zmin = tpcTrack->PositionStart[0];
      zmin = tpcTrack->PositionStart[2];
    }
    // Find the Most downstream TPC point in the global track
    if (tpcTrack->PositionEnd[2]>zmax){
      x_zmax = tpcTrack->PositionEnd[0];
      zmax = tpcTrack->PositionEnd[2];
    }
  }
  
#ifdef DEBUG
  std::cout <<" MomentumResolVariation::GetXBinnedValues()  mode " << mode
    <<  " bin: " << x_zmin << " " << x_zmax << " " << zmin << " " << zmax 
    << std::endl;
#endif

  return GetXBinnedValues(x_zmin, x_zmax, value1, value2, index1, index2, mode);
  
}


//********************************************************************
bool MomentumResolVariation::GetXBinnedValues(
    AnaTPCParticleB* tpcTrack, 
    Float_t& value1, 
    Float_t& value2, 
    Int_t& index1, 
    Int_t& index2, 
    ModeEnum mode){
  //********************************************************************

  if (!tpcTrack) return false;

  if (!_params) return false;

  // Use a x dependence for the moment for prod6
  Float_t xmin = tpcTrack->PositionStart[0];
  Float_t xmax = tpcTrack->PositionEnd[0];

  return GetXBinnedValues(xmin, xmax, value1, value2, index1, index2, mode);
  
}

//**************************************************
AnaParticleB* MomentumResolVariation::GetClosestTPCSegmentWithGoodDQ(const AnaTrackB& track, const Float_t* pos){
  //**************************************************
  
  Float_t dist = 9999999.;

  AnaParticleB* tpcTrack = NULL;

  for(int i = 0; i < track.nTPCSegments; ++i){
    AnaTPCParticleB* tpc_track_tmp = track.TPCSegments[i];

    if (!tpc_track_tmp) continue;

    if (!cutUtils::TPCTrackQualityCut(*tpc_track_tmp)) continue;

    Float_t dist_tmp = std::min(
        anaUtils::GetSeparationSquared(pos, tpc_track_tmp->PositionStart),  
        anaUtils::GetSeparationSquared(pos, tpc_track_tmp->PositionEnd)
        );

    if (dist_tmp < dist){
      dist     = dist_tmp;
      tpcTrack = tpc_track_tmp;
    }
  }

  return tpcTrack;
}

  
//**************************************************
bool MomentumResolVariation::GetXBinnedValues(Float_t xmin, Float_t xmax, 
    Float_t& value1, Float_t& value2, 
    Int_t& index1, Int_t& index2, 
    ModeEnum mode){
//**************************************************

  // A protection against crazy z values leading to x values not found
  if (xmin < -1000 || xmax < -1000) return false;


  Float_t val1_tmp, val2_tmp;

  // Get parameters for upstream point
  bool ok1 = mode == kCorr ? _params->GetBinValues(xmin, value1, val1_tmp, index1) :
    _params->GetBinValues(xmin, val1_tmp, value1, index1);

  // Get parameters for downstream point
  bool ok2 = mode == kCorr ? _params->GetBinValues(xmax, value2, val2_tmp, index2) :
    _params->GetBinValues(xmax, val2_tmp, value2, index2);


#ifdef DEBUG
  std::cout <<" MomentumResolVariation::GetXBinnedValues() " << mode
    <<  " bin: " << xmin << " " << xmax   
    << " " << value1 << " " << value2 << std::endl;
#endif

  if (ok1 && ok2){
    return true;
  }
  else if (ok1 && !ok2){
    value2 = value1;
    index2 = index1;
    return true;
  }
  else if  (!ok1 && ok2){
    value1 = value2;
    index1 = index2;
    return true;
  }
  else 
    return false;

  
  
  
}
