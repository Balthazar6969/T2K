#include "p0dCCQEOOFVSystematics.hxx"
#include "ND280AnalysisUtils.hxx"
#include "BasicUtils.hxx"
#include "SubDetId.hxx"
#include "FiducialVolumeDefinition.hxx"
#include "VersioningUtils.hxx"
#include "ToyBoxTracker.hxx"


const double p0d_fv_minz = -2969;
const double p0d_fv_maxz = -1264;

//********************************************************************
p0dCCQEOOFVSystematics::p0dCCQEOOFVSystematics():EventWeightBase(1){
//********************************************************************

  _name = "P0DCCQEOOFV";

  char dirname[256];
  sprintf(dirname,"%s/data",getenv("P0DNUMUCCQEANALYSISROOT"));
  
  _p0d = new BinnedParams();
  _p0d->SetType(BinnedParams::k1D_SYMMETRIC);
  _p0d->SetName("P0DCCQEOOFV_reco");
  _p0d->Read(dirname);

  _rate = new BinnedParams();  
  _rate->SetType(BinnedParams::k2D_SYMMETRIC);
  _rate->SetName("P0DCCQEOOFV_rate");
  _rate->Read(dirname);
  int npars = 0;
  npars = _p0d->GetNBins();
  npars += _rate->GetNBins();
  SetNParameters(npars);
    for (int i=0;i<9;i++){
      if (!_p0d->GetBinValues(i, _reco_corr[i], _reco_error[i],_reco_index[i])) _reco_index[i]=-1; 

      if (_reco_index[i]>=0) _reco_index[i] += _rate->GetNBins();
    }
 

}

//********************************************************************
Int_t p0dCCQEOOFVSystematics::GetBeamNumber(Int_t runperiod,AnaTrackB *maintrack){
//********************************************************************
  if(runperiod==8){
    if(maintrack->Charge<0)
      return 1;
    else
      return 2;
  }else
    return 0;
}

//********************************************************************
Int_t p0dCCQEOOFVSystematics::GetDetNumber(SubDetId::SubDetEnum det){
//********************************************************************

  if(SubDetId::IsP0DDetector(det))
    return 0;
  else if(SubDetId::IsFGDDetector(det))
    return 1;
  else if(SubDetId::IsECALDetector(det))
    return 2;
  else if(SubDetId::IsTECALDetector(det))
    return 2;
  else if(SubDetId::IsPECALDetector(det))
    return 2;
  else if(SubDetId::IsSMRDDetector(det))
    return 3;
  else if(SubDetId::IsTPCDetector(det))
    return 4;

  else
    return 5;
}

//********************************************************************
Weight_h p0dCCQEOOFVSystematics::ComputeWeight(const ToyExperiment& toy, const AnaEventC& eventC, const ToyBoxB& boxB){
//********************************************************************

    const AnaEventB& event = *static_cast<const AnaEventB*>(&eventC); 

    // Cast the ToyBox to the appropriate type
    const ToyBoxTracker& box = *static_cast<const ToyBoxTracker*>(&boxB); 

    Weight_h eventWeight=1;
    if (!box.MainTrack) return eventWeight;                     // HMN track should exist
    if (!box.MainTrack->GetTrueParticle()) return eventWeight;          // True track associated to HMN track should exist
    if (!box.MainTrack->GetTrueParticle()->TrueVertex) return eventWeight;  // True vertex associated to HMN track should exist

    // Get the true vertex position
    Float_t* tvertex = box.MainTrack->GetTrueParticle()->TrueVertex->Position;
    
    // if the true vertex is inside the P0D FV this is not OOFV (RETURN EVENTWEIGHT=1)    
    if(anaUtils::InFiducialVolume(static_cast<SubDetId::SubDetEnum>(box.DetectorFV), tvertex)) return eventWeight;
        
    // Get the true track direction and position
    //Float_t* tdir = box.MainTrack->GetTrueParticle()->Direction;
    //Float_t* pos  = box.MainTrack->GetTrueParticle()->Position;
    
    Float_t* p0d_det_min = DetDef::p0dmin;
    Float_t* p0d_det_max = DetDef::p0dmax;


    double Zmin_p0d_fv,Zmax_p0d_fv;
    p0d_det_min = DetDef::p0dmin;
    p0d_det_max = DetDef::p0dmax;
    Zmin_p0d_fv = p0d_fv_minz;
    Zmax_p0d_fv = p0d_fv_maxz;

    const SubDetId::SubDetEnum detector = static_cast<const SubDetId::SubDetEnum>(anaUtils::GetDetector(tvertex));

    Int_t categ =-1;
    //In P0D CEcal.
    if (SubDetId::IsP0DDetector(detector) && tvertex[2] > Zmax_p0d_fv)
      categ = 0;
    //In P0D US ECal
    else if (SubDetId::IsP0DDetector(detector) && tvertex[2] < Zmin_p0d_fv)
      categ = 1;
    //In P0D WT
    else if (SubDetId::IsP0DDetector(detector))
      categ = 2;
    else if (SubDetId::IsFGDDetector(detector))
      categ = 3;
    else if (SubDetId::IsECALDetector(detector))
      categ = 4;
    else if (SubDetId::IsPECALDetector(detector))
      categ = 4;
    else if (SubDetId::IsTECALDetector(detector))
      categ = 4;
    else if (SubDetId::IsSMRDDetector(detector))
      categ = 5;
    else if (SubDetId::IsTPCDetector(detector))
      categ = 6;

    if(categ>=0 ){
        SubDetId::SubDetEnum detector=anaUtils::GetDetector(tvertex);
        Int_t runPeriod = anaUtils::GetRunPeriod(event.EventInfo.Run);
   
        if (!_rate->GetBinValues(GetBeamNumber(runPeriod,box.MainTrack), GetDetNumber(detector), _rate_corr, _rate_error,_rate_index)) _rate_index=-1; 

        if (_reco_index[categ]>=0){
          eventWeight.Systematic *= (1+ _reco_corr[categ] + _reco_error[categ]*toy.GetToyVariations(_index)->Variations[_reco_index[categ]]);
          eventWeight.Correction *= (1+ _reco_corr[categ]);
        }
        if (_rate_index>=0){
          eventWeight.Systematic *= (1+ _rate_corr + _rate_error*toy.GetToyVariations(_index)->Variations[_rate_index]);
          eventWeight.Correction *= (1+ _rate_corr);
        }

    }


    if (eventWeight.Systematic < 0) eventWeight.Systematic = 0;

    return eventWeight;
}

