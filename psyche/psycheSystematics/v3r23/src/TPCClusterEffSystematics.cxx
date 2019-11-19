#include "TPCClusterEffSystematics.hxx"
#include "ND280AnalysisUtils.hxx"
#include "CutUtils.hxx"
#include "SystematicUtils.hxx"
#include "SystId.hxx"

//********************************************************************
TPCClusterEffSystematics::TPCClusterEffSystematics():BinnedParams("TPCClusterEff",k1D_EFF_SYMMETRIC, versionUtils::Extension()){
//********************************************************************

    // Get the TPC cluster extra inefficiency
    //GetParametersForBin(0, _extraIneff, _extraIneffError);
    //GetParametersForBin(0, _params);
    SetNParameters(2*GetNBins());
}

//********************************************************************
Weight_h TPCClusterEffSystematics::ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box, const SelectionBase& sel){
//********************************************************************

    // Get the SystBox for this event, and the appropriate selection and branch
    SystBoxB* SystBox = GetSystBox(event,box.SelectionEnabledIndex,box.SuccessfulBranch);


    Weight_h eventWeight = 1.0;

    // loop over reconstructed tracks in the TPC
    for (Int_t irec=0;irec<(int) SystBox->nRelevantRecObjects;irec++){
      AnaTrackB* track = static_cast<AnaTrackB*>(SystBox->RelevantRecObjects[irec]);
      
      if (!track) continue;
      
      // For example in numuCC inclusive selection, only the Candidate is important at first order
      if (!sel.IsRelevantRecObjectForSystematicInToy(event, box, track, SystId::kTpcClusterEff, box.SuccessfulBranch)) continue;
                   
      //SubDetId::SubDetEnum tpc=anaUtils::GetClosestTPC(*track);
      //AnaTPCParticleB *tpcTrack=static_cast<AnaTPCParticleB*> (anaUtils::GetSegmentInDet(*track,tpc));

      //      AnaTPCParticleB* tpcTrack = static_cast<AnaTPCParticleB*>(track->TPCSegments[0]);

      // Use the TPC segment with more nodes in closest TPC to the start position of the track
      AnaTPCParticleB* tpcTrack = static_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInClosestTpc(*track));      
      if (!tpcTrack)continue;
      float dir=fabs(tpcTrack->DirectionStart[2]);
  
      BinnedParamsParams params;
      int index;
      if(!GetBinValues(dir, params, index)) continue;

      int nn= tpcTrack->NNodes;           
     
      //           pvar_data/pvar_mc *pana
      //eff_w   = ----------------------
      //                   pana

      //           1-  pvar_data/pvar_mc *pana
      //ineff_w =---------------------------
      //                1 - pana
      
      //meanDATA  <=> effdata/effmc
      //meanMC    <=> 1
      //meanMCANA <=> 1
      //sigmaMC   <=> 0
      //sigmaDATA <=> ratio error

      /// extra hit inefficiency for MC: 0.0007 = effdata-effMC/effMC  
      /// => effdata/effmc=1-0.0007 

      // weight = effvarWeight ^nn *( (nn+1) * Pineffvar + 1) / ( (nn+1) * Pineff +1)
 
#if useNewWeights 
      Weight_h eff_w= systUtils::ComputeEffLikeWeight(true, toy.GetToyVariations(_index)->Variations[0], params);//new way with data-mc error included
#else
      Weight_h eff_w= systUtils::ComputeEffLikeWeight(true, toy.GetToyVariations(_index)->Variations[0], toy.GetToyVariations(_index)->Variations[1], params);
#endif
      Float_t Pnom=params.meanMCANA;
      Weight_h Pineff=(eff_w*(-1))+1; //this is true, only if assume that effMC=1 

      Weight_h weight(TMath::Power(eff_w.Correction,(Float_t)nn)*((nn+1)*(Pineff.Correction)+1. )/ ((nn+1)*(1-Pnom)+1. ),
                      TMath::Power(eff_w.Systematic,(Float_t)nn)*((nn+1)*(Pineff.Systematic)+1. )/ ((nn+1)*(1-Pnom)+1. ));
      eventWeight*=weight; 
    }

   
  return eventWeight;
    
}
