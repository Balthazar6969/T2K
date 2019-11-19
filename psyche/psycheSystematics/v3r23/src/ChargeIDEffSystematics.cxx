#include "ChargeIDEffSystematics.hxx"
#include "ND280AnalysisUtils.hxx"
#include "CutUtils.hxx"
#include "SystematicUtils.hxx"
#include "VersioningUtils.hxx"
#include "MultiThread.hxx"
#include "SystId.hxx"
#include "EventBoxId.hxx"
#include "Parameters.hxx"


//********************************************************************
ChargeIDEffSystematics::ChargeIDEffSystematics(bool comp):EventWeightBase(2){
  //********************************************************************
  _computecounters=comp;

  _globalCharge = new BinnedParams("globChargeIDEff", BinnedParams::k2D_EFF_SYMMETRIC, versionUtils::Extension());
  //_globalCharge=BinnedParams("globTrueChargeIDEff",BinnedParams::k2D_EFF_SYMMETRIC,versionUtils::Extension());
  _localCharge  = new BinnedParams("localChargeIDEff",BinnedParams::k2D_EFF_SYMMETRIC, versionUtils::Extension());

  //the number of parameter is divided by 5(global), 4(local) because there are
  //5(4) parameters to paremetrize the efficiency
  int nbins=  _globalCharge->GetNBins()/5.+_localCharge->GetNBins()/4;
  if(versionUtils::prod6_systematics)
    SetNParameters(nbins);
  else
    SetNParameters(_globalCharge->GetNBins()*2);
  if( _computecounters)
    _globalCharge->InitializeEfficiencyCounter();

  _full_correlations = ND::params().GetParameterI("psycheSystematics.Tracker.FullCorrelations");
}

//********************************************************************
Weight_h ChargeIDEffSystematics::ComputeWeight(const ToyExperiment& toy, const AnaEventC& eventBB, const ToyBoxB& box, const SelectionBase& sel){
//********************************************************************

  const AnaEventB& event = *static_cast<const AnaEventB*>(&eventBB); 

  if(_computecounters)
    _globalCharge->InitializeEfficiencyCounter();

  // Get the SystBox for this event, and the appropriate selection and branch
  SystBoxB* SystBox = GetSystBox(event,box.SelectionEnabledIndex,box.SuccessfulBranch);
  
  // Initial weight is one
  Weight_h eventWeight=1;
  Weight_h localWeight=1;
  
  for (Int_t itrack=0;itrack<SystBox->nRelevantRecObjects;itrack++){
    AnaTrackB* track = static_cast<AnaTrackB*>(SystBox->RelevantRecObjects[itrack]);

    //TODO: has to be put here for now since it is not the same as puting it in the isRelevantRecObject method.
    //in future we should consider putting it in the isRelevantRecObject method
    if (!track->TrueObject) continue;
    if (track->Momentum<0)continue;
    if (track->nTPCSegments==0)continue;
    if (!track->Original) continue;
    // Only for tracks passing the quality cut
    if (!cutUtils::TrackQualityCut(*track)) continue;
    // std::cout<<" momorig "<<track->Original->Momentum<<" mom "<<track->Momentum<<std::endl;


    // For example in numuCC inclusive selection, only the Candidate is important at first order
    if (!sel.IsRelevantRecObjectForSystematicInToy(event,box,track,SystId::kChargeIDEff,box.SuccessfulBranch)) continue;
   
    // Get array of TPC segments, but only one per TPC, the one with more nodes, are ordered in increasing TPC number
    AnaTPCParticleB* TPCSegments[3];
    Int_t nTPCSegments = anaUtils::GetOneSegmentPerTPC(track->TPCSegments, track->nTPCSegments, TPCSegments);
    if (nTPCSegments==0) continue;

    AnaTPCParticleB* tpcTrack1          = static_cast<AnaTPCParticleB*>(TPCSegments[0]);
    const AnaTPCParticleB* tpcTrack1orig = static_cast<const AnaTPCParticleB*>(TPCSegments[0]->Original);
    if (!tpcTrack1orig) continue;

    Float_t momerr        = tpcTrack1->MomentumError;
    Float_t momerrMAX     = tpcTrack1->MomentumError;
    Float_t momerrOrig    = tpcTrack1orig->MomentumError;
    Float_t momerrMAXOrig = tpcTrack1orig->MomentumError;
    Float_t mom           = tpcTrack1->Momentum;
    Float_t momMAX        = tpcTrack1->Momentum;
    Float_t momorig       = tpcTrack1orig->Momentum;
    Float_t momMAXorig    = tpcTrack1orig->Momentum;

    Int_t charge     =track->Charge;
    Int_t locCharge  =tpcTrack1->Charge;
    Int_t syst_case=-1;
    Float_t globWeight_momerr=1;
    Float_t locWeight_momerr=1;

    if (nTPCSegments==1){
      if(momerr!=momerr || !TMath::Finite(momerr) || momerr < 0.00001) continue;
      if(mom!=mom || !TMath::Finite(mom)) continue;
      syst_case=0;
    }else if(nTPCSegments==2){
      AnaTPCParticleB* tpcTrack2           = static_cast<AnaTPCParticleB*>(TPCSegments[1]);
      const AnaTPCParticleB* tpcTrack2orig = static_cast<const AnaTPCParticleB*>(TPCSegments[1]->Original);
      if (!tpcTrack2orig) continue;
      Float_t tmpmomerr2 =tpcTrack2->MomentumError;
      Float_t tmpmomerr1 =tpcTrack1->MomentumError;
      if(tmpmomerr1!=tmpmomerr1 || !TMath::Finite(tmpmomerr1) || tmpmomerr1 < 0.00001) tmpmomerr1 = 99999999;
      if(tmpmomerr2!=tmpmomerr2 || !TMath::Finite(tmpmomerr2) || tmpmomerr2 < 0.00001) tmpmomerr2 = 99999999;
      momerr=std::min(tmpmomerr1,tmpmomerr2);
      if(tmpmomerr1==99999999)tmpmomerr1 = -99999999;
      if(tmpmomerr2==99999999)tmpmomerr2 = -99999999;
      momerrMAX=std::max(tmpmomerr1,tmpmomerr2);

      if(momerr==99999999)continue;
      if(momerrMAX==-99999999)continue;

      if(momerr==tpcTrack2->MomentumError){ 
        mom       =tpcTrack2->Momentum;
        momorig   =tpcTrack2orig->Momentum;
        locCharge =tpcTrack2->Charge;
        momerrOrig=tpcTrack2orig->MomentumError;
      }
      if(mom!=mom || !TMath::Finite(mom) || mom < 0.00001) continue;
      if(momerrMAX==tpcTrack2->MomentumError){
        momMAX=tpcTrack2->Momentum;
        momMAXorig   =tpcTrack2orig->Momentum;
        momerrMAXOrig=tpcTrack2orig->MomentumError;
      }
      Int_t Q1=tpcTrack1->Charge;
      Int_t Q2=tpcTrack2->Charge;

      if(Q1==Q2)syst_case=1;
      else      syst_case=2;
    }
    else if (nTPCSegments==3){

      AnaTPCParticleB* tpcTrack2 = static_cast<AnaTPCParticleB*>(TPCSegments[1]);
      AnaTPCParticleB* tpcTrack3 = static_cast<AnaTPCParticleB*>(TPCSegments[2]);
      const AnaTPCParticleB* tpcTrack2orig = static_cast<const AnaTPCParticleB*>(TPCSegments[1]->Original);
      const AnaTPCParticleB* tpcTrack3orig = static_cast<const AnaTPCParticleB*>(TPCSegments[2]->Original);
      if (!tpcTrack2orig) continue;
      if (!tpcTrack3orig) continue;

      Float_t tmpmomerr3 =tpcTrack3->MomentumError;
      Float_t tmpmomerr2 =tpcTrack2->MomentumError;
      Float_t tmpmomerr1 =tpcTrack1->MomentumError;
      if(tmpmomerr1!=tmpmomerr1 || !TMath::Finite(tmpmomerr1) || tmpmomerr1 < 0.00001) tmpmomerr1 = 99999999;
      if(tmpmomerr2!=tmpmomerr2 || !TMath::Finite(tmpmomerr2) || tmpmomerr2 < 0.00001) tmpmomerr2 = 99999999;
      if(tmpmomerr3!=tmpmomerr3 || !TMath::Finite(tmpmomerr3) || tmpmomerr3 < 0.00001) tmpmomerr3 = 99999999;
      Float_t momerr12=std::min(tmpmomerr1,tmpmomerr2);
      momerr=std::min(momerr12,tmpmomerr3);
      if(momerr==99999999)continue;
      if(momerr==tpcTrack2->MomentumError){
        mom=tpcTrack2->Momentum;
        locCharge =tpcTrack2->Charge;
        momorig   =tpcTrack2orig->Momentum;
        momerrOrig=tpcTrack2orig->MomentumError;
        
      }
      else if(momerr==tpcTrack3->MomentumError){
        mom=tpcTrack3->Momentum;
        locCharge =tpcTrack3->Charge;
        momorig   =tpcTrack2orig->Momentum;
        momerrOrig=tpcTrack3orig->MomentumError;
      }
      if(mom!=mom || !TMath::Finite(mom) || mom < 0.00001) continue;
      if(tmpmomerr1==99999999)tmpmomerr1 = -99999999;
      if(tmpmomerr2==99999999)tmpmomerr2 = -99999999;
      if(tmpmomerr3==99999999)tmpmomerr3 = -99999999;
      Float_t momerrMAX12=std::max(tmpmomerr1,tmpmomerr2);
      momerrMAX=std::max(momerrMAX12,tmpmomerr3);
      if(momerrMAX==-99999999)continue;
      if(momerrMAX==tpcTrack2->MomentumError){
        momMAX=tpcTrack2->Momentum;
        momMAXorig   =tpcTrack2orig->Momentum;
        momerrMAXOrig=tpcTrack2orig->MomentumError;
      }else if(momerrMAX==tpcTrack3->MomentumError){
        momMAX=tpcTrack3->Momentum;
        momMAXorig   =tpcTrack3orig->Momentum;
        momerrMAXOrig=tpcTrack3orig->MomentumError;
      }
      
      Int_t Q1=tpcTrack1->Charge;
      Int_t Q2=tpcTrack2->Charge;
      Int_t Q3=tpcTrack3->Charge;
      if( Q1==Q2 && Q2==Q3) syst_case=3;
      if((Q1!=Q2 && Q2==Q3) || (Q1!=Q3 && Q1==Q2) || (Q1!=Q2 && Q1==Q3)) syst_case=4;
    }
    //compute weight for local charge systematics:
    //we don't compute any systematics in case of local charge for 1 segment
    if(versionUtils::prod6_systematics && syst_case>0){
      if(momMAX<1 )continue; //those tracks are not selected
      bool areEqual= (syst_case==1 || syst_case==3) ? true : false;
      BinnedParamsParams parloc[4];
      for(Int_t ipar=0;ipar<4;ipar++){
        _localCharge->GetBinValues(syst_case,ipar, parloc[ipar]);
      }
      BinnedParamsParams paramsloc;
      Int_t index_loc=0;//there are 8 parameters in the file but in fact only 2 errors...
      if(syst_case>2) index_loc=1;
      Float_t effmc=ComputeEffFromLocalParametrization(parloc,momMAX, momerrMAX);
      Float_t effmcorig=ComputeEffFromLocalParametrization(parloc,momMAXorig, momerrMAXOrig);
      Float_t ww= (areEqual) ? effmc/effmcorig : (1-effmc)/(1-effmcorig);
      if(effmcorig==0 && areEqual) ww=1;
      if(effmcorig==1 && !areEqual) ww=1;
      // std::cout<<" mom "<<momMAX<<" err "<<momerrMAX<<" eff "<<effmc
      // 	       <<"\n momO "<<momMAXorig<<" errO "<<momerrMAXOrig<<" eff "<<effmcorig
      // 	       <<" ww "<<ww<<std::endl;
      locWeight_momerr*=ww;

      ComputeEffFromLocalParametrization(parloc,momMAX,momerrMAX,paramsloc );
      //the same variation is applied since it is a fake variation for the mc 

      // override to ensure same variations for all params (see bugzilla 1271)
      if (_full_correlations) index_loc = 0;

      localWeight *= systUtils::ComputeEffLikeWeight(areEqual, toy, GetIndex(), index_loc, paramsloc);
    }

    if(localWeight.Correction!=0 ) eventWeight.Correction*=(localWeight.Correction);
    if(localWeight.Systematic!=0 ) eventWeight.Systematic*=(localWeight.Systematic*locWeight_momerr);

    //NOW looking at global charge.
    //for prod5:
    Float_t p0 = track->Momentum;
    // Get charge and true charge
    Int_t trueCharge=track->GetTrueParticle()->Charge;
    // Get rec and true direction in Z
    Float_t dir = track->DirectionStart[2];
    Float_t trueDir=track->GetTrueParticle()->Direction[2];

    BinnedParamsParams params;
    Int_t index;
    bool found;
    if(!versionUtils::prod6_systematics){
      // Get the charge confusion for this momentum
      if(!_globalCharge->GetBinValues(0,p0, params, index)) continue;
      // is the track correctly identified?
      // correct when charge and direction are correct, or charge and direction are wrong
      found= (trueCharge*charge>0 && trueDir*dir>0) || (trueCharge*charge<0 && trueDir*dir<0);
    }
    else{
      // // this case is neglected for global charge
      // if(syst_case==3)continue;
      // BinnedParamsParams par[4];
      // //get the parametrization values
      // for(Int_t ipar=0;ipar<4;ipar++){
      // 	_globalCharge->GetBinValues(syst_case,ipar, par[ipar], index);
      // }
      // ComputeEffFromGlobalParametrization(par,mom, momerr,params );
      // found= (trueCharge*charge>0 && trueDir*dir>0) || (trueCharge*charge<0 && trueDir*dir<0);

      BinnedParamsParams par[5];
      //get the parametrization values
      for(Int_t ipar=0;ipar<5;ipar++){
        _globalCharge->GetBinValues(syst_case,ipar, par[ipar]);
      }
      found = (charge*locCharge>0);
      if(mom<1 )continue; //those tracks are not selected
      Float_t effmc=ComputeEffFromGlobalLocalParametrization(par,mom, momerr);
      Float_t effmcorig=ComputeEffFromGlobalLocalParametrization(par,momorig, momerrOrig);
      Float_t ww= (found) ? effmc/effmcorig : (1-effmc)/(1-effmcorig);
      if(effmcorig==0 && found) ww=1;
      if(effmcorig==1 && !found) ww=1;

      globWeight_momerr*=ww;

      ComputeEffFromGlobalLocalParametrization(par,mom, momerr,params );

      //there 4 errors but 20 lines, one error for each case...
      index = syst_case +_localCharge->GetNBins()/4;
    }

    // override to ensure same variations for all params (see bugzilla 1271)
    if (_full_correlations) index = 0;

    //the same variation is applied since it is a fake variation for the mc
    Weight_h globWeight = systUtils::ComputeEffLikeWeight(found, toy, GetIndex(), index, params);

    if(globWeight!=globWeight){
      /*
      std::cout << "ChargeIDEffSystematics. globWeight for track " << itrack << " has value = "<< globWeight << " in run, event, subrun = "
                << event.EventInfo.Run << " "
                << event.EventInfo.Event << " "
                << event.EventInfo.SubRun << " ==> Skip track. Problem under investigation !!!! " << std::endl;
      continue;
      */
    }

    if(globWeight.Correction!=0 ) eventWeight.Correction*=(globWeight.Correction);
    if(globWeight.Systematic!=0 ) eventWeight.Systematic*=(globWeight.Systematic*globWeight_momerr);

    // std::cout<<" evw "<<eventWeight<<" glob "<<globWeight<<" globmom "<<globWeight_momerr
    // 	     <<" loc "<<localWeight<<" locmom "<<locWeight_momerr
    // 	     <<std::endl;
    if(_computecounters)
      _globalCharge->UpdateEfficiencyCounter(index,found); 

  }

  return eventWeight;
}

//********************************************************************
void ChargeIDEffSystematics::ComputeEffFromGlobalParametrization(BinnedParamsParams *par,Float_t mom, Float_t momerr, BinnedParamsParams &params ){
//********************************************************************
  //the hard coded values are here to get a pull with sigma=1
  Float_t vvv=fabs((0.48+7.7E-4*mom)*momerr/mom);
  if(vvv>2.5) vvv=2.5;
  Float_t var=TMath::Log(vvv);
  Float_t effMC      = par[0].meanMC  +par[1].meanMC*var  +par[2].meanMC*pow(var,2)  +par[3].meanMC*pow(var,3);

  Float_t ratio = par[0].meanDATA; // in charge misID .dat, meanDATA is the ratio between data and MC in the control sample.
  Float_t err = par[0].sigmaDATAl; // in charge misID .dat, sigmaDATAl is the error on the data/MC ratio in the control sample
  // if (1-ratio) i.e. is larger that err, let's use (1-ratio) as error!
  if (fabs(1-ratio) > err) err = fabs(1-ratio);
  Float_t effDATA    = ratio * effMC;
  Float_t effDATA_err= err  *effMC;

  // if(effMC<0){
  //   std::cout<<" var "<<var<<" mom "<<mom<<" momerr "<<momerr
  // 	     <<" 0 "<<par[0].meanMC
  // 	     <<" 1 "<<par[1].meanMC*var
  // 	     <<" 2 "<<par[2].meanMC*pow(var,2)
  // 	     <<" 3 "<<par[3].meanMC*pow(var,3)<<std::endl;
  //   std::cout<<" =====> "<<effMC<<std::endl;
  // }

  // pvar_data = effdata + efferrdata*var 
  // pvar_mc   = effmc   + efferrmc  *var
  // => ratiovar = ratio + ratio_err *var

  // pvar_data/pvar_mc *pmcANA          ratiovar*pmcANA
  //-----------------------------   =   -------------------   
  //       pmcANA                        pmcANA

  // 1- pvar_data/pvar_mc *pmcANA      1-ratiovar*pmcANA
  //-----------------------------   =  ---------------
  //    1 - pmcANA                      1-pmcANA

  //filling params as the following is equivalent: 
  params.meanDATA  =effDATA;
  params.sigmaDATAl=effDATA_err;
  params.sigmaDATAh=effDATA_err;
  params.meanMC    =effMC;
  params.sigmaMCl  =0;
  params.sigmaMCh  =0;
  params.meanMCANA =effMC;
}

//********************************************************************
Float_t ChargeIDEffSystematics::ComputeEffFromGlobalLocalParametrization(BinnedParamsParams *par,Float_t mom, Float_t momerr ){
//********************************************************************
  Float_t vvv=-999;
  if(mom!=0)  
    vvv= fabs((0.48+7.7E-4*mom)*momerr/mom);
  if(vvv>2.5) vvv=2.5;
  Float_t var=TMath::Log(vvv);
  Float_t effMC      = (par[0].meanMC  +par[1].meanMC*var  +par[2].meanMC*pow(var,2)  +par[3].meanMC*pow(var,3))*(1+par[4].meanMC*pow(vvv,2));
  Float_t eff=std::max(effMC,(Float_t)0.);
  eff=std::min(effMC,(Float_t)1.);

  return eff;

}

//********************************************************************
void ChargeIDEffSystematics::ComputeEffFromGlobalLocalParametrization(BinnedParamsParams *par,Float_t mom, Float_t momerr, BinnedParamsParams &params ){
//********************************************************************
  //the hard coded values are here to get a pull with sigma=1
  Float_t effMC      = ComputeEffFromGlobalLocalParametrization(par,mom,momerr);

  Float_t ratio = par[0].meanDATA; // in charge misID .dat, meanDATA is the ratio between data and MC in the control sample.
  Float_t err = par[0].sigmaDATAl; // in charge misID .dat, sigmaDATAl is the error on the data/MC ratio in the control sample
  // if (1-ratio) i.e. is larger that err, let's use (1-ratio) as error!
  if (fabs(1-ratio) > err) err = fabs(1-ratio);
  Float_t effDATA    = ratio * effMC;
  Float_t effDATA_err= err  *effMC;

  //  if(effMC<0){
  //    std::cout<<" var "<<var<<" mom "<<mom<<" momerr "<<momerr
  //	     <<" 0 "<<par[0].meanMC
  //   	   <<" 1 "<<par[1].meanMC*var
  //   	   <<" 2 "<<par[2].meanMC*pow(var,2)
  //   	   <<" 3 "<<par[3].meanMC*pow(var,3)
  //   	   <<" 4 "<<par[4].meanMC*pow(vvv,2)+1<<std::endl;
  //    std::cout<<" =====> "<<effMC<<std::endl;
  //  }
  // pvar_data = effdata + efferrdata*var 
  // pvar_mc   = effmc   + efferrmc  *var
  // => ratiovar = ratio + ratio_err *var

  // pvar_data/pvar_mc *pmcANA          ratiovar*pmcANA
  //-----------------------------   =   -------------------   
  //       pmcANA                        pmcANA

  // 1- pvar_data/pvar_mc *pmcANA      1-ratiovar*pmcANA
  //-----------------------------   =  ---------------
  //    1 - pmcANA                      1-pmcANA

  //filling params as the following is equivalent: 
  params.meanDATA  =effDATA;
  params.sigmaDATAl=effDATA_err;
  params.sigmaDATAh=effDATA_err;
  params.meanMC    =effMC;
  params.sigmaMCl  =0;
  params.sigmaMCh  =0;
  params.meanMCANA =effMC;
}
//********************************************************************
Float_t ChargeIDEffSystematics::ComputeEffFromLocalParametrization(BinnedParamsParams *par,Float_t mom, Float_t momerr ){
//********************************************************************

  //the hard coded values are here to get a pull with sigma=1
  Float_t var=-999;
  if(mom!=0) 
    var=fabs((0.48+7.7E-4*mom)*momerr/mom);
  if(var>2.5) var=2.5;//parametrization valid up to this range...
  Float_t effMC      = par[0].meanMC  +(par[1].meanMC +par[2].meanMC*var)*exp(-par[3].meanMC/var);
    
  Float_t eff=std::max(effMC,(Float_t)0.);
  eff=std::min(effMC,(Float_t)1.);
  return eff;

}
//********************************************************************
void ChargeIDEffSystematics::ComputeEffFromLocalParametrization(BinnedParamsParams *par,Float_t mom, Float_t momerr, BinnedParamsParams &params ){
//********************************************************************

  Float_t effMC      = ComputeEffFromLocalParametrization(par,mom,momerr);

  Float_t ratio = par[0].meanDATA; // in charge misID .dat, meanDATA is the ratio between data and MC in the control sample.
  Float_t err = par[0].sigmaDATAl; // in charge misID .dat, sigmaDATAl is the error on the data/MC ratio in the control sample
  // if (1-ratio) i.e. is larger that err, let's use (1-ratio) as error!
  if (fabs(1-ratio) > err) err = fabs(1-ratio);
  Float_t effDATA    = ratio * effMC;
  Float_t effDATA_err= err  *effMC;

  // pvar_data = effdata + efferrdata*var 
  // pvar_mc   = effmc   + efferrmc  *var
  // => ratiovar = ratio + ratio_err *var

  // pvar_data/pvar_mc *pmcANA          ratiovar*pmcANA
  //-----------------------------   =   -------------------   
  //       pmcANA                        pmcANA

  // 1- pvar_data/pvar_mc *pmcANA      1-ratiovar*pmcANA
  //-----------------------------   =  ---------------
  //    1 - pmcANA                      1-pmcANA

  //filling params as the following is equivalent: 
  params.meanDATA  =effDATA;
  params.sigmaDATAl=effDATA_err;
  params.sigmaDATAh=effDATA_err;
  params.meanMC    =effMC;
  params.sigmaMCl  =0;
  params.sigmaMCh  =0;
  params.meanMCANA =effMC;

}

//********************************************************************
void ChargeIDEffSystematics::FillSystBox(const AnaEventC& event, const SelectionBase& sel, Int_t ibranch){
//********************************************************************

  Int_t uniqueID = 0;

#ifdef MULTITHREAD
  uniqueID = event.UniqueID;
#endif

  // Get the selection index;
  Int_t isel=sel.GetEnabledIndex();  

  // Get the SystBox
  SystBoxB& box = *_systBoxes[isel][ibranch][uniqueID];

  Int_t* groups;
  anaUtils::CreateArray(groups, 10);
  Int_t nGroups = sel.GetRelevantRecObjectGroupsForSystematic(_index, groups, ibranch);
  anaUtils::ResizeArray(groups, nGroups, 10);
  
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];

  if (nGroups>0){
    Int_t nMaxTracks=0;
    for (Int_t g = 0; g<nGroups;g++)
      nMaxTracks += EventBox->nRecObjectsInGroup[groups[g]];
    
    if (box.RelevantRecObjects) delete box.RelevantRecObjects;
    anaUtils::CreateArray(box.RelevantRecObjects, nMaxTracks);
    box.nRelevantRecObjects=0;
    
    for (Int_t g = 0; g<nGroups;g++){
      
      AnaRecObjectC** tracks0 = EventBox->RecObjectsInGroup[groups[g]];
      int nOrig= EventBox->nRecObjectsInGroup[groups[g]];
            
      bool used[NMAXPARTICLES]={false};
      bool used_forQC[NMAXPARTICLES]={true};
      //take only into account 1 association between 1 track and trajectory.
      for (Int_t it1=0;it1<nOrig;it1++){
        if(it1>=99) break;
        AnaTrackB* track1 = static_cast<AnaTrackB*>(tracks0[it1]);
        
        if (!IsRelevantRecObject(event,*track1)) continue;
        if (!sel.IsRelevantRecObjectForSystematic(event,track1,_index)) continue;
        
        if(!track1) continue;
        if(!track1->TrueObject) continue;
        if(!used[it1]) used_forQC[it1]=true;
        Float_t Q1=track1->Charge;
        Float_t ID1=track1->TrueObject->ID;
        for (Int_t it2 = it1+1; it2<nOrig; it2++){
          if(it2>=99) break;
          AnaTrackB* track2 = static_cast<AnaTrackB*>(tracks0[it2]);
          
          if (!IsRelevantRecObject(event,*track2)) continue;
          if (!sel.IsRelevantRecObjectForSystematic(event,track2,_index)) continue;
          if(!track2)continue;
          if(!track2->TrueObject)continue;     
          Float_t Q2=track2->Charge;
          Float_t trueQ2=track2->GetTrueParticle()->Charge;
          Float_t ID2=track2->TrueObject->ID;
          if(Q1*Q2<0 && ID1==ID2){
            if(Q2*trueQ2>0 ){
              used_forQC[it1]=false;
              used_forQC[it2]=true;
              used[it1]=used[it2]=true;
            }
            else{
              used_forQC[it1]=true;
              used_forQC[it2]=false;
              used[it1]=used[it2]=true;
            }
          }
        }
      }
      for (Int_t it1=0;it1<nOrig;it1++){
        if(used_forQC[it1]){ 
          box.RelevantRecObjects[box.nRelevantRecObjects++]= (tracks0[it1]);
        }
      }
    }
    if (box.nRelevantRecObjects != nMaxTracks)
      anaUtils::ResizeArray(box.RelevantRecObjects, box.nRelevantRecObjects, nMaxTracks);
  }

  if (groups) delete [] groups;
  
}

// //**************************************************
// bool ChargeIDEffSystematics::IsRelevantRecObject(const AnaEventC& event, const AnaRecObjectC& track) const{
// //**************************************************

//   // TODO. Commented out for the moment for validation purposes

//   (void)event;



//   // Only for tracks passing the quality cut (Put this condition first, since it is more restrictive)
//   if (!cutUtils::TrackQualityCut(track)) return false;

//   if (!track.TrueObject) return false;
//   if (track.Momentum<0) return false;
//   //  if (track.nTPCSegments==0) return false;
//   if (!track.Original) return false;

//   return true;
// }
