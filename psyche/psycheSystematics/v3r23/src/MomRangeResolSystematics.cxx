#include "MomRangeResolSystematics.hxx"
#include "ND280AnalysisUtils.hxx"
#include "EventBoxTracker.hxx"
#include "VersioningUtils.hxx"

//#define DEBUG


//********************************************************************
MomRangeResolSystematics::MomRangeResolSystematics():EventVariationBase(1),BinnedParams("MomRangeResol", k3D_SYMMETRIC_NOMEAN){
  //********************************************************************

  SetNParameters(GetNBins());
}

//********************************************************************
void MomRangeResolSystematics::Apply(const ToyExperiment& toy, AnaEventC& event){
  //********************************************************************

  // Get the SystBox for this event
  SystBoxB* box = GetSystBox(event);

#ifdef DEBUG
  std::cout << "MomRangeResolSystematics::ApplyVariation(): " << box->nRelevantRecObjects << std::endl;  
#endif

  // Loop over relevant tracks for this systematic
  for (Int_t itrk=0;itrk<box->nRelevantRecObjects;itrk++){

    AnaTrackB* track = static_cast<AnaTrackB*>(box->RelevantRecObjects[itrk]);

    if (!track) continue;
    
    if (!track->TrueObject) continue;

#ifdef DEBUG
    std::cout << itrk << std::endl;
    track->Print();
    std::cout<<"\n"<<std::endl;
#endif

    // Get the true momentum
    Float_t p0_true = track->GetTrueParticle()->Momentum;

    // Get momentum from range for muon hypothesis
    Float_t p0_range = track->RangeMomentumMuon;

#ifdef DEBUG
    std::cout << "track true momentum p0_true           -- " << p0_true  << std::endl;
    std::cout << "track range momentum  muon p0_range   -- " << p0_range << std::endl;
#endif

    // Get the angle of a track wrt to beam axis: this is used to determine the systematic bins
    Float_t costheta = fabs(track->DirectionStart[2]);

    // Get the detectors involved
    // FGD
    int bin_fgddet = GetFGDDet(track->Detector);
    int bin_outerdet = 0;
    SubDetId::SubDetEnum smrd_det   = GetSMRDDet(  track->Detector);
    SubDetId::SubDetEnum tecal_det  = GetTECalDet( track->Detector);
    if(smrd_det!=SubDetId::kInvalid)
      bin_outerdet = smrd_det; 
    else if(tecal_det!=SubDetId::kInvalid)
      bin_outerdet = tecal_det;
      
#ifdef DEBUG
    std::cout << " bin values: bin_fgddet "<< bin_fgddet << " bin_outerdet " << bin_outerdet << " costheta " << costheta << "\n";
#endif

    // Get the extra resolution values: bins are defined by angle
    Float_t sigma;
    Int_t index;

    if (!GetBinSigmaValue(bin_fgddet, bin_outerdet, costheta, sigma, index)) continue;

#ifdef DEBUG
    std::cout << " bin values: sigma " << sigma << " index " << index << std::endl;
#endif

    // Apply the additional momentum resolution smearing with a sigma=x
    Float_t prangev = (1 + sigma * toy.GetToyVariations(_index)->Variations[index]) * (p0_range - p0_true) + p0_true;
#ifdef DEBUG
    std::cout << "muon candidate p_range after tweak = "<< prangev << std::endl;
#endif

    // Apply the variation
    track->RangeMomentumMuon = prangev;
  }

}

//********************************************************************
bool MomRangeResolSystematics::UndoSystematic(AnaEventC& event){
  //********************************************************************

  // Get the SystBox for this event
  SystBoxB* box = GetSystBox(event);

  for (Int_t itrk=0;itrk<box->nRelevantRecObjects;itrk++){
    AnaTrackB* track = static_cast<AnaTrackB*>(box->RelevantRecObjects[itrk]);
    // Go back to the corrected momentum
    track->RangeMomentumMuon = track->GetOriginalTrack()->RangeMomentumMuon;
  }
  // Don't reset the spill to corrected
  return false;
}

//********************************************************************
bool MomRangeResolSystematics::IsRelevantRecObject(const AnaEventC& event, const AnaRecObjectC& recObj) const{
//********************************************************************
  (void)(event);

  const AnaTrackB* track = static_cast<const AnaTrackB*>(&recObj);
  
  // do nothing if the momentum value is a default one (-999),  and less than RP default 0.1
  if (track->RangeMomentumMuon < 0.5) return false;
  
  //ignore tracks with more than one segment in SMRD or ECal
  if (track->nECALSegments>1 || track->nSMRDSegments>1) return false;
  
  //want the candidate with no TPC
  if(SubDetId::GetDetectorUsed(track->Detector, SubDetId::kTPC))
    return false;
    
  return true;
}

//**************************************************
SubDetId::SubDetEnum MomRangeResolSystematics::GetFGDDet(unsigned long Detector) {
  //**************************************************
  SubDetId::SubDetEnum det = SubDetId::kInvalid;
  if(SubDetId::GetDetectorUsed(Detector,      SubDetId::kFGD1))
    det = SubDetId::kFGD1;
  else if(SubDetId::GetDetectorUsed(Detector, SubDetId::kFGD2))
    det = SubDetId::kFGD2;       
  
  return det;
}


//**************************************************
SubDetId::SubDetEnum MomRangeResolSystematics::GetTECalDet(unsigned long Detector) {
  //**************************************************
  SubDetId::SubDetEnum det = SubDetId::kInvalid;
  if(SubDetId::GetDetectorUsed(Detector,      SubDetId::kTopTECAL))
    det = SubDetId::kTopTECAL;
  else if(SubDetId::GetDetectorUsed(Detector, SubDetId::kLeftTECAL))
    det = SubDetId::kLeftTECAL;                    
  else if(SubDetId::GetDetectorUsed(Detector, SubDetId::kRightTECAL))
    det = SubDetId::kRightTECAL;
  else if(SubDetId::GetDetectorUsed(Detector, SubDetId::kBottomTECAL))
    det = SubDetId::kBottomTECAL;

  return det;
}



//**************************************************
SubDetId::SubDetEnum MomRangeResolSystematics::GetSMRDDet(unsigned long Detector) {
  //**************************************************
  SubDetId::SubDetEnum det = SubDetId::kInvalid;
  if(SubDetId::GetDetectorUsed(Detector,      SubDetId::kTopSMRD))
    det = SubDetId::kTopSMRD;
  else if(SubDetId::GetDetectorUsed(Detector, SubDetId::kLeftSMRD))
    det = SubDetId::kLeftSMRD;                    
  else if(SubDetId::GetDetectorUsed(Detector, SubDetId::kRightSMRD))
    det = SubDetId::kRightSMRD;
  else if(SubDetId::GetDetectorUsed(Detector, SubDetId::kBottomSMRD))
    det = SubDetId::kBottomSMRD;

  return det;
}
