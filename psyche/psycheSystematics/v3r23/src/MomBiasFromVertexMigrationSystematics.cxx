#include "MomBiasFromVertexMigrationSystematics.hxx"
#include "ND280AnalysisUtils.hxx"
#include "EventBoxTracker.hxx"
#include "Parameters.hxx"

//#define DEBUG


//********************************************************************
MomBiasFromVertexMigrationSystematics::MomBiasFromVertexMigrationSystematics():EventVariationBase(1),BinnedParams("MomBiasFromVertexMigration",k2D_SYMMETRIC_NOMEAN){
  //********************************************************************

  SetNParameters(GetNBins());

}

//********************************************************************
void MomBiasFromVertexMigrationSystematics::Apply(const ToyExperiment& toy, AnaEventC& event){
  //********************************************************************

  // Get the SystBox for this event
  SystBoxB* box = GetSystBox(event);

#ifdef DEBUG
  std::cout << "MomBiasFromVertexMigrationSystematics::ApplyVariation(): " << box->nRelevantRecObjects <<" and? "<<box->nRelevantRecObjects<< std::endl;  
#endif

  // loop over the relevant tracks for this systematic
  for (Int_t itrk=0;itrk<box->nRelevantRecObjects;itrk++){
    AnaTrackB* track = static_cast<AnaTrackB*>(box->RelevantRecObjects[itrk]);

#ifdef DEBUG
    std::cout << itrk << " --> p0 = "      << track->Momentum     << std::endl;
    std::cout << itrk << " --> pflip_0 = " << track->MomentumFlip << std::endl;
#endif

    // Get the bias value: bins are defined by angle and momentum
    Float_t bias = 0;
    Int_t index;

    if (!GetBinSigmaValue(track->Momentum, fabs(track->DirectionStart[2]), bias, index)) continue;

    // Apply the momentum smearing
    track->Momentum           += bias * toy.GetToyVariations(_index)->Variations[index];

    // Apply the momentum smearing to the reversed fit as well
    track->MomentumFlip       += bias * toy.GetToyVariations(_index)->Variations[index];
    
    // Apply the momentum smearing for momentum by range as well 
    track->RangeMomentumMuon  += bias * toy.GetToyVariations(_index)->Variations[index];


#ifdef DEBUG
    std::cout << "p = "     << track->Momentum     << std::endl;
    std::cout << "pflip = " << track->MomentumFlip << std::endl;
#endif
  }
}
//********************************************************************
bool MomBiasFromVertexMigrationSystematics::UndoSystematic(AnaEventC& event){
  //********************************************************************

  // Get the SystBox for this event
  SystBoxB* box = GetSystBox(event);

  for (Int_t itrk=0;itrk<box->nRelevantRecObjects;itrk++){
    AnaTrackB* track = static_cast<AnaTrackB*>(box->RelevantRecObjects[itrk]);
    // Go back to the corrected momentum
    track->Momentum           = track->GetOriginalTrack()->Momentum;
    track->MomentumFlip       = track->GetOriginalTrack()->MomentumFlip;
    track->RangeMomentumMuon  = track->GetOriginalTrack()->RangeMomentumMuon;
  }

  // Don't reset the spill to corrected
  return false;
}
