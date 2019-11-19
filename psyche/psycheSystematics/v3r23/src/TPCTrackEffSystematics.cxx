#include "TPCTrackEffSystematics.hxx"
#include "ND280AnalysisUtils.hxx"
#include "EventBoxTracker.hxx"
#include "BasicUtils.hxx"
#include "SystematicUtils.hxx"
#include "SubDetId.hxx"
#include "SystId.hxx"
#include "Parameters.hxx"

//********************************************************************
TPCTrackEffSystematics::TPCTrackEffSystematics(bool comp):BinnedParams("TPCTrackEff",k1D_EFF_ASSYMMETRIC, versionUtils::Extension()){
//********************************************************************

    _computecounters=comp;
    SetNParameters(2*GetNBins());
    if( _computecounters)
        InitializeEfficiencyCounter();

    _full_correlations = ND::params().GetParameterI("psycheSystematics.Tracker.FullCorrelations");
}

//********************************************************************
Weight_h TPCTrackEffSystematics::ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box, const SelectionBase& sel){
//********************************************************************

    // Get the SystBox for this event, and the appropriate selection and branch
    SystBoxB* SystBox = GetSystBox(event,box.SelectionEnabledIndex,box.SuccessfulBranch);

    if(_computecounters)
      InitializeEfficiencyCounter();

    /*
    //this case happen when:
    // -the reco track is broken
    // -the true track don't pass the cut dist>30 and mom> 5MeV
    // -the reco track is absurd and matched to a true track that is not happening in the same bunch
    */
    BinnedParamsParams params;
    Weight_h eventWeight=1;

    // Loop over all TrueParticles in the TPC
    for (Int_t itrue=0;itrue< SystBox->nRelevantTrueObjects; itrue++){      
      AnaTrueParticleB* truePart = static_cast<AnaTrueParticleB*>(SystBox->RelevantTrueObjects[itrue]);            
      // retrieve the reconstructed track associated
      //      AnaTrackB* recoTrack = static_cast<AnaTrackB*>(SystBox->RelevantTrueObjectsReco[itrue]);

      // For example in numuCC inclusive selection, only the TrueTrack associated to the muon candidate, and other true muon tracks should be considered
      if (!sel.IsRelevantTrueObjectForSystematicInToy(event,box,truePart,SystId::kTpcTrackEff,box.SuccessfulBranch)) continue;

      // Is there any reconstructed track associated to this true track ?
      bool found=false;
      int tpc_true=-1; //save the closest tpc to the FGD
      for(Int_t idet=0;idet<truePart->nDetCrossings;idet++){
        if ( box.DetectorFV == SubDetId::kFGD1                                       && SubDetId::GetDetectorUsed(truePart->DetCrossings[idet]->Detector,SubDetId::kTPC1)) tpc_true=1;
        if ((box.DetectorFV == SubDetId::kFGD1 || box.DetectorFV == SubDetId::kFGD2) && SubDetId::GetDetectorUsed(truePart->DetCrossings[idet]->Detector,SubDetId::kTPC2)) tpc_true=2;
        if ( box.DetectorFV == SubDetId::kFGD2                                       && SubDetId::GetDetectorUsed(truePart->DetCrossings[idet]->Detector,SubDetId::kTPC3)) tpc_true=3;
        if ( box.DetectorFV == SubDetId::kP0D && SubDetId::GetDetectorUsed(truePart->DetCrossings[idet]->Detector,SubDetId::kTPC1)) tpc_true = 1;
      }

      int tpc_rec=-1;
      for (Int_t irec=0;irec<(int) SystBox->nRelevantRecObjects; irec++){
        AnaTrackB* track = static_cast<AnaTrackB*>(SystBox->RelevantRecObjects[irec]);
        for(int ii = 0; ii < track->nTPCSegments; ii++){
          AnaTPCParticleB* tpc_track = track->TPCSegments[ii];
          if(!tpc_track->TrueObject)continue;
          if(truePart->ID==tpc_track->TrueObject->ID){            
            SubDetId::SubDetEnum tpcnum = SubDetId::GetSubdetectorEnum(tpc_track->Detector);
            if       (tpcnum == SubDetId::kTPC2) tpc_rec=2;
            else if  (tpcnum == SubDetId::kTPC3) tpc_rec=3;
            else if  (tpcnum == SubDetId::kTPC1) tpc_rec=1;
            if(tpc_true==tpc_rec)//search for the segment that match the one we are looking at, at the true level.
              break;
          }
          // Try to recover some of the particles associated to parents or grandparents of the recon objects. 
          else if ( truePart->ID > tpc_track->TrueObject->ID && (truePart->PDG == tpc_track->GetTrueParticle()->ParentPDG || truePart->PDG == tpc_track->GetTrueParticle()->GParentPDG)) { 
            SubDetId::SubDetEnum tpcnum = SubDetId::GetSubdetectorEnum(tpc_track->Detector);
            if       (tpcnum == SubDetId::kTPC2) tpc_rec=2;
            else if  (tpcnum == SubDetId::kTPC3) tpc_rec=3;
            else if  (tpcnum == SubDetId::kTPC1) tpc_rec=1;
            if(tpc_true==tpc_rec)
              break;
          }
        }
        if(tpc_true==tpc_rec) break;
      }
      if(tpc_true==tpc_rec && tpc_rec>-1) found=true;
      if(tpc_rec==-1) tpc_rec=tpc_true;

      // Get the TPC tracking efficiency for bin 0 (there is a single bin);
      int index;
      if(!GetBinValues((Float_t)tpc_rec, params, index))	  continue;

      // override to ensure same variations for all params (see bugzilla 1271)
      if (_full_correlations) index = 0;

      //      bool found = (recoTrack);
      eventWeight *= systUtils::ComputeEffLikeWeight(found, toy, GetIndex(), index, params);

      if(_computecounters)
        UpdateEfficiencyCounter(index,found);    
      
    }

    return eventWeight;
}

//********************************************************************
bool TPCTrackEffSystematics::IsRelevantTrueObject(const AnaEventC& event, const AnaTrueObjectC& trueObj) const{
//********************************************************************

    (void)event;

    const AnaTrueParticleB& truePart = *static_cast<const AnaTrueParticleB*>(&trueObj);

    if(truePart.Charge==0) return false;

    // true momentum larger than 60 MeV
    if( truePart.Momentum < 60. ) return false;

    // compute the linear length 
    Float_t zlength;
    Float_t length = anaUtils::GetTrueLinearLengthInTPC(truePart,zlength);
    
    // linear length must be larger than 18 cm
    if( length < 180. ) return false;
    // and start position in FGD1 FV
    //    if (!anaUtils::InFiducialVolume(event.EventBoxes[EventBoxId::kEventBoxTracker]->DetectorFV, truePart.Position)) return false;
    
    Float_t dist=-9999999;
    for(Int_t idet=0;idet<truePart.nDetCrossings;idet++){
      //i.e crossing the active part of the tpc
      if(SubDetId::GetDetectorUsed(truePart.DetCrossings[idet]->Detector, SubDetId::kTPC) && truePart.DetCrossings[idet]->InActive) {
        Float_t sep = anaUtils::GetSeparationSquared(truePart.DetCrossings[idet]->EntrancePosition, truePart.DetCrossings[idet]->ExitPosition);
        if(sep>dist) dist=sep;
      }
    }
    // 30* 30 originally
    //bigger than 3 TPC hits (30*30 is faster that sqrt(dist)), and momentum > 5 MeV 
    if((dist)<900) return false;

    return true;
}


//********************************************************************
bool TPCTrackEffSystematics::CheckTrueRecoAssociation(const AnaTrueObjectC& truePart, const AnaRecObjectC& track) const{
//********************************************************************

  (void) truePart;
  (void) track;

/*
  if(truePart.ID==track.TrueObject->ID || 
     // Try to recover some of the particles associated to parents or grandparents of the recon objects. 
     (truePart.ID > track.TrueObject->ID && (truePart.PDG == track.GetTrueParticle()->ParentPDG || truePart.PDG == track.GetTrueParticle()->GParentPDG))) { 
    return true;
  }
*/

  return false;
}
