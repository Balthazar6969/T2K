#include "TPCFGDMatchEffSystematics.hxx"
#include "CutUtils.hxx"
#include "SystematicUtils.hxx"
#include "VersioningUtils.hxx"
#include "SystId.hxx"
#include "Parameters.hxx"

//#define DEBUG

//********************************************************************
TPCFGDMatchEffSystematics::TPCFGDMatchEffSystematics(bool comp):BinnedParams("TPCFGDMatchEff",k1D_EFF_ASSYMMETRIC,versionUtils::Extension()){
//********************************************************************

  _computecounters=comp;
  SetNParameters(2*GetNBins());
  if (_computecounters)
    InitializeEfficiencyCounter();
  
  // In production 6 one uses ToF difference between two FGDs to reverse an object
  // hence it has to be taken into account through matching systematics, if a track crosses two
  // FGDs then the systematics should be applied to either of the FGD segments that has the few NNodes requirement
  // or both
 
  // For the moment control with a parameter
  _apply_both_FGD1_FGD2 = (bool)ND::params().GetParameterI("psycheSystematics.TPCFGDMatchEffSystematics.ApplyBoth_FGD1_FGD2");
  _prod6_nnodes_cut     = ND::params().GetParameterI("psycheSystematics.TPCFGDMatchEffSystematics.Prod6NNodesCut");
  
}

//********************************************************************
Weight_h TPCFGDMatchEffSystematics::ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box,  const SelectionBase& sel){
//********************************************************************

  if(_computecounters)
    InitializeEfficiencyCounter();

  // Get the SystBox for this event, and the appropriate selection and branch
  SystBoxB* SystBox = GetSystBox(event,box.SelectionEnabledIndex,box.SuccessfulBranch);

#ifdef DEBUG
  std::cout << " TPCFGDMatchEffSystematics::ComputeWeight() " << std::endl;
  std::cout << " Event " << static_cast<const AnaEventB&>(event).EventInfo.Event << std::endl;
#endif

  Weight_h eventWeight=1;

  if( !versionUtils::prod6_systematics){
    // Loop over all charged trajectories traversing TPC or FGD 
    for (Int_t itrue=0;itrue<SystBox->nRelevantTrueObjects;itrue++){
      AnaTrueParticleB* trueTrack = static_cast<AnaTrueParticleB*>(SystBox->RelevantTrueObjects[itrue]);
      
      AnaTrackB* recoTrack = static_cast<AnaTrackB*>(SystBox->RelevantTrueObjectsReco[itrue]);
      bool tpc_rec_exists=false;
      
      // Use by default the true momentum
      Float_t p0 = trueTrack->Momentum;
      if (recoTrack) p0 = recoTrack->Momentum;	
      
      // Retrieve the systematic
      BinnedParamsParams params;
      Int_t index;
      if (!GetBinValues(p0,params,index)) continue;
      
      //here the big inefficiency that is observed in MC and in Data is not trusted. 
      //Because the values jump from 0.7 to 0.98 from one bin to the other, 
      //we decided to interpolate the inefficiency linearly to the next bin.    
      if(p0<100 ){
        BinnedParamsParams params1;
        GetParametersForBin(1,params1);
        Float_t mMCANA   = params.meanMCANA   + p0/100*(params1.meanMCANA - params.meanMCANA);
        Float_t mMC      = params.meanMC      + p0/100*(params1.meanMC    - params.meanMC);
        Float_t mDATA    = params.meanDATA    + p0/100*(params1.meanDATA  - params.meanDATA);
        Float_t sigMCl   = params1.sigmaMCl   + (100-p0)/100.*params.sigmaMCl;
        Float_t sigMCh   = params1.sigmaMCh   + (100-p0)/100.*params.sigmaMCh;
        Float_t sigDATAl = params1.sigmaDATAl + (100-p0)/100.*params.sigmaDATAl;
        Float_t sigDATAh = params1.sigmaDATAh + (100-p0)/100.*params.sigmaDATAh;
        params.meanMCANA  = mMCANA;
        params.meanMC     = mMC;
        params.meanDATA   = mDATA; 
        params.sigmaMCl   = sigMCl;
        params.sigmaMCh   = sigMCh;
        params.sigmaDATAl = sigDATAl; 
        params.sigmaDATAh = sigDATAh; 
      }
      
      // Efficiency weight when a complete TPC-FGD track exists
      bool found=false;
      if(recoTrack) found=true;
      //      else if(tpc_rec_exists || inFGDTPCseg) found=false;
      else if(tpc_rec_exists) found=false;
      else continue;

      eventWeight *= systUtils::ComputeEffLikeWeight(found, toy, GetIndex(), index, params);

      if(_computecounters)
        UpdateEfficiencyCounter(index,found);    
    }
  }
  else{
 /// for long tracks crossing the FGD, TPC-FGD matching is working at 100% in production6 therefore we only consider for production 6 very short tracks starting at the edge of the FGD. This is the case that has not been taken into account with the TPC-FGD matching package which use through-going muons crossing TPC1 and TPC2 or TPC2 and TPC3.
    /// for very short tracks in the FGD starting at the edge, tpc-fgd matching will depend on the efficiency to really get a hit in one of the two layers under consideration. Therefore the following propagation.
    ////// NHITS
    /// assume we observe nhits in the reco tracks.
    /// 1) we have the possibility that there were really nhits before (we did not lose anything:
    /// probability is :p = eff^nhits
    /// 2) we have the possibility that one hit is lost, so there nhits+1 before
    /// probability is :p= (nhits+1)*eff^nhits*(1-eff)
    /// 3) we lost 2 hits, so there were nhits+2 before
    /// probability is :p= (nhits+2)*(nhits+1)/2*eff^nhits*(1-eff)^2
    /// sum p= eff^nhits*[1+(nhits+1)*(1-eff)+(nhits+2)*(nhits+1)/2*(1-eff)^2]
    ///      = eff^nhits*[1+(nhhits+1)(1-eff)[1+(nhits+2)/2*(1-eff)]]
    
    //if( SystBox->nRelevantRecObjects == 0 ) return eventWeight;
    // Loop over relevant tracks for this systematic

    for (Int_t itrk = 0; itrk < SystBox->nRelevantRecObjects; itrk++){
      AnaTrackB* track = static_cast<AnaTrackB*>(SystBox->RelevantRecObjects[itrk]);

#ifdef DEBUG
  std::cout << " Track " << itrk << std::endl;
#endif

      if (!track) continue;
      
      // For example in numuCC inclusive selection, only the Candidate is important at first order
      if (!sel.IsRelevantRecObjectForSystematicInToy(event, box, track, SystId::kTpcFgdMatchEff, box.SuccessfulBranch)) continue;
    
    
      AnaParticleB* FGD1Segment = NULL;
      AnaParticleB* FGD2Segment = NULL;
    
      bool isInFGD1 = anaUtils::InFiducialVolume(SubDetId::kFGD1, track->PositionStart); 
      bool isInFGD2 = anaUtils::InFiducialVolume(SubDetId::kFGD2, track->PositionStart); 
   
      if (!isInFGD1 && !isInFGD2) return false;
    
      if (isInFGD1 || _apply_both_FGD1_FGD2){
        FGD1Segment = anaUtils::GetSegmentWithMostNodesInDet(*track, SubDetId::kFGD1);
      }
      
      if (isInFGD2 || _apply_both_FGD1_FGD2){
        FGD2Segment = anaUtils::GetSegmentWithMostNodesInDet(*track, SubDetId::kFGD2);
      }
    
      if (!FGD1Segment && !FGD2Segment) continue;

      // Primary candidate
      AnaParticleB* prim_cand = isInFGD1 ? FGD1Segment : FGD2Segment;
      // Secondary candidate
      AnaParticleB* sec_cand  = isInFGD1 ? FGD2Segment : FGD1Segment;
    
      eventWeight *= GetWeight(static_cast<AnaFGDParticleB*>(prim_cand), toy);
        
#ifdef DEBUG
  Weight_h weight_tmp = GetWeight(static_cast<AnaFGDParticleB*>(prim_cand), toy);
  std::cout << "weight  prim corr " << weight_tmp.Correction << " syst " << weight_tmp.Systematic << std::endl;
#endif
      if (_apply_both_FGD1_FGD2){
#ifdef DEBUG
        std::cout << " applying second weight " << std::endl;
        if (sec_cand){
          std::cout << " FGD " << SubDetId::GetSubdetectorEnum(sec_cand->Detector)  <<std::endl;
        }
        else
          std::cout << "does not exist " << std::endl;
#endif        
        eventWeight *= GetWeight(static_cast<AnaFGDParticleB*>(sec_cand), toy);
      
#ifdef DEBUG
        Weight_h weight_tmp = GetWeight(static_cast<AnaFGDParticleB*>(sec_cand), toy);
        std::cout << "weight second corr " << weight_tmp.Correction << " syst " << weight_tmp.Systematic << std::endl;
#endif
        
        
      }
    }
  }
  
#ifdef DEBUG
  std::cout << "weight final event corr " << eventWeight.Correction << " syst " << eventWeight.Systematic << std::endl;
#endif

  return eventWeight;
}
//**************************************************
Weight_h TPCFGDMatchEffSystematics::GetWeight(const AnaFGDParticleB* FGDSegment, const ToyExperiment& toy){
//**************************************************

  Weight_h weight;
  if (!FGDSegment) return weight;
  
  Float_t nn = FGDSegment->NNodes;
  
  // Do nothing if nnodes above the cut
  if (nn > _prod6_nnodes_cut) return weight;

  int fgdnum = 0;
  
  if (SubDetId::GetSubdetectorEnum(FGDSegment->Detector) == SubDetId::kFGD1)
    fgdnum = 1;
  else if (SubDetId::GetSubdetectorEnum(FGDSegment->Detector) == SubDetId::kFGD2)
    fgdnum = 2;
   
  // Read the systematic source parameters
  BinnedParamsParams params;
  int index;
  
  if (!GetBinValues(fgdnum, params, index)) return weight;

  Weight_h eff_w = systUtils::ComputeEffLikeWeight(true, toy, GetIndex(), 0, params);
  
  Float_t Pnom = params.meanMCANA;
  
  Weight_h Pineff = 1-eff_w; //this is true, only if assume that effMC=1 
  
  weight *= Weight_h(TMath::Power(eff_w.Correction,nn)*(1.+(nn+1)*(Pineff.Correction)*(((nn+2)/2.)*Pineff.Correction) )/ (1.+(nn+1)*(1-Pnom)*((nn+2)/2.*(1-Pnom))),
      TMath::Power(eff_w.Systematic,nn)*(1.+(nn+1)*(Pineff.Systematic)*(((nn+2)/2.)*Pineff.Systematic) )/ (1.+(nn+1)*(1-Pnom)*((nn+2)/2.*(1-Pnom))));

  
  //   std::cout<<" eff_w "<<eff_w<<" effmc "<<params.meanMC<<" effdata "<<params.meanDATA<< std::endl;
   //   std::cout<<" nnodes "<<nnodes<<" pos "<<track->PositionStart[2]<<" truepos "<<track->GetTrueParticle()->Position[2]<<" weight "<<eventWeight<<std::endl;
   

  
  
  return weight;
  
}


//THIS IS FOR PROD6 PROP
//**************************************************
bool TPCFGDMatchEffSystematics::IsRelevantRecObject(const AnaEventC& event, const AnaRecObjectC& recObj) const{
//**************************************************

  (void)event;

  const AnaTrackB& track = *static_cast<const AnaTrackB*>(&recObj);

  if(!track.TrueObject) return false;
  if( versionUtils::prod6_systematics){
    if(track.nFGDSegments == 0) return false;
    AnaParticleB* FGD1Segment = NULL;
    AnaParticleB* FGD2Segment = NULL;
    
    bool isInFGD1 = anaUtils::InFiducialVolume(SubDetId::kFGD1, track.PositionStart); 
    bool isInFGD2 = anaUtils::InFiducialVolume(SubDetId::kFGD2, track.PositionStart); 
   
    if (!isInFGD1 && !isInFGD2) return false;
    
    if (isInFGD1 || _apply_both_FGD1_FGD2){
      FGD1Segment = anaUtils::GetSegmentWithMostNodesInDet(track,SubDetId::kFGD1);
    }
    if (isInFGD2 || _apply_both_FGD1_FGD2){
      FGD2Segment = anaUtils::GetSegmentWithMostNodesInDet(track,SubDetId::kFGD2);
    }
    
    if (!FGD1Segment && !FGD2Segment) return false;

    // Primary candidate
    AnaParticleB* prim_cand = isInFGD1 ? FGD1Segment : FGD2Segment;
    // Secondary candidate
    AnaParticleB* sec_cand  = isInFGD1 ? FGD2Segment : FGD1Segment;
    
    if (!prim_cand) return false;
    
    Int_t nnodes = prim_cand->NNodes;
    
    if (_apply_both_FGD1_FGD2 && sec_cand)
      nnodes = std::min(nnodes, sec_cand->NNodes);
    
  
    //only consider the very short case tracks, since those are suceptible to change something in data
    //normally one hit in the FGD is needed so that there is a matching, 
    // std::cout<<" nn "<<nnodes<<std::endl;
    if (nnodes > _prod6_nnodes_cut) return false;
    return true;
  }
  else{
    if (track.Momentum < 30) return false;  
    return true;
  }
  
}
//THIS IS FOR PROD5 PROP
//********************************************************************
bool TPCFGDMatchEffSystematics::CheckTrueRecoAssociation(const AnaTrueObjectC& trueTrack, const AnaRecObjectC& track) const{
//********************************************************************

  // True track should always exist
  if(trueTrack.ID != track.TrueObject->ID) return false;
    
  // Check for a good TPC-FGD track
  bool tinFGD1 = SubDetId::GetDetectorUsed(track.Detector, SubDetId::kFGD1);
  bool tinFGD2 = SubDetId::GetDetectorUsed(track.Detector, SubDetId::kFGD2);
  bool tinTPC1 = SubDetId::GetDetectorUsed(track.Detector, SubDetId::kTPC1);
  bool tinTPC2 = SubDetId::GetDetectorUsed(track.Detector, SubDetId::kTPC2);
  bool tinTPC3 = SubDetId::GetDetectorUsed(track.Detector, SubDetId::kTPC3);
    
  // Complete TPC-FGD track found
  if((tinFGD1 && (tinTPC1 || tinTPC2)) || (tinFGD2 && (tinTPC2 || tinTPC3))) return true;
    
  return false;
}

