#include "nueOOFVSystematics.hxx"
#include "ND280AnalysisUtils.hxx"
#include "ToyBoxTracker.hxx"
#include "BasicUtils.hxx"
#include "SubDetId.hxx"
#include "FiducialVolumeDefinition.hxx"
#include "VersioningUtils.hxx"
#include "SystematicUtils.hxx"

//********************************************************************
nueOOFVSystematics::nueOOFVSystematics():EventWeightBase(1){
  //********************************************************************

  int npars=0;

  _fgd1 = new BinnedParams("OOFV_reco_fgd1_p6B",BinnedParams::k1D_SYMMETRIC);
  npars=_fgd1->GetNBins();
  _fgd2 = new BinnedParams("OOFV_reco_fgd2_p6B",BinnedParams::k1D_SYMMETRIC);
  npars+=_fgd2->GetNBins();
  _rate = new BinnedParams("nueOOFV_rate"      ,BinnedParams::k1D_SYMMETRIC);
  npars+=_rate->GetNBins();

  SetNParameters(npars);

  for (int i=0;i<9;i++){
    if (!_fgd1->GetBinValues(i, _reco_corr[0][i], _reco_error[0][i],_reco_index[0][i])) _reco_index[0][i]=-1; 
    if (!_fgd2->GetBinValues(i, _reco_corr[1][i], _reco_error[1][i],_reco_index[1][i])) _reco_index[1][i]=-1; 

    //  if (_reco_index[0][i]>=0) _reco_index[0][i] += _rate->GetNBins();
    //  if (_reco_index[1][i]>=0) _reco_index[1][i] += _rate->GetNBins();
  }

}

//********************************************************************
void nueOOFVSystematics::Initialize(){
  //********************************************************************

  systUtils::AddBinnedParamsOffsetToSystematic(*this, *_rate, _rate->GetNBins());
  systUtils::AddBinnedParamsOffsetToSystematic(*this, *_fgd1, _fgd1->GetNBins()); 
  systUtils::AddBinnedParamsOffsetToSystematic(*this, *_fgd2, _fgd2->GetNBins());
}

//********************************************************************
Int_t nueOOFVSystematics::GetDetNumber(SubDetId::SubDetEnum det){
  //********************************************************************

  if(SubDetId::IsP0DDetector(det))
    return 0;
  else if(SubDetId::IsECALDetector(det))
    return 1;
  else if(SubDetId::IsTECALDetector(det))
    return 1;
  else if(SubDetId::IsPECALDetector(det))
    return 1;
  else if(SubDetId::IsSMRDDetector(det))
    return 2;
  else if(SubDetId::IsTPCDetector(det))
    return 3;
  else if(SubDetId::IsFGDDetector(det))
    return -1;
  else
    return 4;
}

//********************************************************************
Weight_h nueOOFVSystematics::ComputeWeight(const ToyExperiment& toy, const AnaEventC& eventBB, const ToyBoxB& boxB){
  //********************************************************************

  const AnaEventB& event = *static_cast<const AnaEventB*>(&eventBB); 

  // Cast the ToyBox to the appropriate type
  const ToyBoxTracker& box = *static_cast<const ToyBoxTracker*>(&boxB); 

  (void)event;

  Weight_h eventWeight=1;
  if (!box.MainTrack) return eventWeight;                     // HMN track should exist
  if (!box.MainTrack->TrueObject) return eventWeight;          // True track associated to HMN track should exist
  if (!box.MainTrack->GetTrueParticle()->TrueVertex) return eventWeight;  // True vertex associated to HMN track should exist

  // Get the true vertex position
  Float_t* tvertex = box.MainTrack->GetTrueParticle()->TrueVertex->Position;

  // if the true vertex is inside the FGD FV this is not OOFV (RETURN EVENTWEIGHT=1)    
  if(anaUtils::InFiducialVolume(static_cast<SubDetId::SubDetEnum>(box.DetectorFV), tvertex)) return eventWeight;

  // Get the true track direction and position
  Float_t* tdir = box.MainTrack->GetTrueParticle()->Direction;
  Float_t* pos  = box.MainTrack->GetTrueParticle()->Position;

  Float_t* fgd_det_min;
  Float_t* fgd_det_max;
  double Zmin_lastmodule,zup,zdown;
  bool lastmodule;
  Int_t fgd;
  if(box.DetectorFV == SubDetId::kFGD1){
    fgd=0;

    fgd_det_min     = DetDef::fgd1min;
    fgd_det_max     = DetDef::fgd1max;
    Zmin_lastmodule = fgd_det_max[2]-DetDef::fgdXYModuleWidth; //427;
    zup             =-60;
    zdown           = 623;
  }
  else if(box.DetectorFV == SubDetId::kFGD2){
    fgd=1;

    fgd_det_min     = DetDef::fgd2min;
    fgd_det_max     = DetDef::fgd2max;
    Zmin_lastmodule = 1780;
    zup             = 1298;
    zdown           = 1983.095;
  }
  else return eventWeight;

  lastmodule=( pos[2] >Zmin_lastmodule && pos[2] < fgd_det_max[2]);

  Int_t categ =-1;
  //inFGD1scint. Put z condition first to be faster
  if      ((tvertex[2] > fgd_det_min[2]  && tvertex[2] < fgd_det_max[2]) &&
      (tvertex[0] > fgd_det_min[0]  && tvertex[0] < fgd_det_max[0]) && 
      (tvertex[1] > fgd_det_min[1]  && tvertex[1] < fgd_det_max[1]))
    categ=0;
  //upstreamFGD1scint    
  else if ((tvertex[2] > zup             && tvertex[2] < fgd_det_min[2]) && 
      (tvertex[0] > -1335           && tvertex[0] < 1335)           && 
      (tvertex[1] > -1280.5         && tvertex[1] < 1280.5))
    categ=1;
  //downstreamFGD1scint
  else if((tvertex[2] > fgd_det_max[2]   && tvertex[2] < zdown)  && 
      (tvertex[0] > -1335            && tvertex[0] < 1335)   && 
      (tvertex[1] > -1280.5          && tvertex[1] < 1280.5))
    categ=2;
  //neutralparent
  else if( box.MainTrack->GetTrueParticle()->ParentPDG  == 2112 || box.MainTrack->GetTrueParticle()->ParentPDG  == 22 ||  
      box.MainTrack->GetTrueParticle()->GParentPDG == 2112 || box.MainTrack->GetTrueParticle()->GParentPDG == 22)
    categ=3;
  //backwards
  else if(tdir[2]<=-0.5)
    categ=4;
  //highangle
  else if(tdir[2]>-0.5 && tdir[2]<0.5)
    categ=5;
  //veryforward
  else if(fabs(tdir[0]/tdir[2])<0.07 || fabs(tdir[1]/tdir[2])<0.07)
    categ=6;
  //lastmodule 
  else if ( lastmodule  && anaUtils::InDetVolume(static_cast<SubDetId::SubDetEnum>(box.DetectorFV), box.MainTrack->PositionStart))
    categ=7;
  else 
    categ=8;

  // Note that this is wrong for layer28-29, veryhighangle and veryforward,
  // for those one matching reco should be rerun to understand how reco is changed, by now it is applied as a rate uncertainty... but....  
  if(categ>=0 ){
    SubDetId::SubDetEnum detector=anaUtils::GetDetector(tvertex);

    if (!_rate->GetBinValues(GetDetNumber(detector), _rate_corr[0][0], _rate_error[0][0],_rate_index[0][0])) _rate_index[0][0]=-1; 

    
    if (_reco_index[fgd][categ]>=0){
      Int_t reco_index = _reco_index[fgd][categ];
      reco_index += (fgd == 0) ? systUtils::GetBinnedParamsOffsetForSystematic(*this, *_fgd1) :
          systUtils::GetBinnedParamsOffsetForSystematic(*this, *_fgd2);

      eventWeight.Systematic *= (1+ _reco_corr[fgd][categ] + _reco_error[fgd][categ] * toy.GetToyVariations(_index)->Variations[reco_index]);
      eventWeight.Correction *= (1+ _reco_corr[fgd][categ]);
    }
    if (_rate_index[0][0]>=0){
      Int_t rate_index = _rate_index[0][0];
      rate_index += systUtils::GetBinnedParamsOffsetForSystematic(*this, *_rate); 
      eventWeight.Systematic *= (1+ _rate_corr[0][0]       + _rate_error[0][0]      * toy.GetToyVariations(_index)->Variations[rate_index]);
      eventWeight.Correction *= (1+ _rate_corr[0][0]);
    }      
  }

  if (eventWeight.Systematic < 0) eventWeight.Systematic = 0;

  return eventWeight;
}

