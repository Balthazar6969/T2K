#include "MichelElectronEffSystematics.hxx"
#include "ND280AnalysisUtils.hxx"
#include "EventBoxTracker.hxx"
#include "ToyBoxTracker.hxx"
#include "SubDetId.hxx"
#include "SystId.hxx"
#include "EventBoxId.hxx"
#include "SystematicUtils.hxx"
#include "Parameters.hxx"
#include "VersioningUtils.hxx"

//********************************************************************
MichelElectronEffSystematics::MichelElectronEffSystematics(bool comp):EventWeightBase(2){
//********************************************************************
  _computecounters = comp;
  // Get the michel electron efficiency values
  Int_t baseindex = 0;
  
  _fgd1eff = new BinnedParams("FGD1MichelElectronEff", BinnedParams::k1D_EFF_SYMMETRIC, versionUtils::Extension());
  baseindex += systUtils::GetNumberSystParams(*_fgd1eff, true);
  _fgd1pur = new BinnedParams("FGD1MichelElectronPur", BinnedParams::k1D_EFF_SYMMETRIC, versionUtils::Extension());
  baseindex += systUtils::GetNumberSystParams(*_fgd1pur, true); 
  if(versionUtils::prod6_systematics){
    _fgd2eff = new BinnedParams("FGD2MichelElectronEff", BinnedParams::k1D_EFF_SYMMETRIC, versionUtils::Extension());
    baseindex += systUtils::GetNumberSystParams(*_fgd2eff, true); 
    _fgd2pur = new BinnedParams("FGD2MichelElectronPur", BinnedParams::k1D_EFF_SYMMETRIC, versionUtils::Extension());
    baseindex += systUtils::GetNumberSystParams(*_fgd2pur, true); 
  }
  
  if(_computecounters){
    _fgd1pur->InitializeEfficiencyCounter();
    _fgd1eff->InitializeEfficiencyCounter();
    if(versionUtils::prod6_systematics){
      _fgd2pur->InitializeEfficiencyCounter();
      _fgd2eff->InitializeEfficiencyCounter();
    }
  }
  
  SetNParameters(baseindex);
  
  _prod5Cut = ND::params().GetParameterI("psycheSystematics.Prod5Cuts"); 
  
}

//********************************************************************
void MichelElectronEffSystematics::Initialize(){
//********************************************************************
  
  //Register BinnedParams to get proper offsets
  systUtils::AddBinnedParamsOffsetToSystematic(*this, *_fgd1eff, systUtils::GetNumberSystParams(*_fgd1eff, true));
  systUtils::AddBinnedParamsOffsetToSystematic(*this, *_fgd1pur, systUtils::GetNumberSystParams(*_fgd1pur, true));

  if (versionUtils::prod6_systematics){
    systUtils::AddBinnedParamsOffsetToSystematic(*this, *_fgd2eff, systUtils::GetNumberSystParams(*_fgd2eff, true));
    systUtils::AddBinnedParamsOffsetToSystematic(*this, *_fgd2pur, systUtils::GetNumberSystParams(*_fgd2pur, true));
  }
  

}


//********************************************************************
Weight_h MichelElectronEffSystematics::ComputeWeight(const ToyExperiment& toy, const AnaEventC& eventBB, const ToyBoxB& boxB){
//********************************************************************

  const AnaEventB& event = *static_cast<const AnaEventB*>(&eventBB); 

  // Cast the ToyBox to the appropriate type
  const ToyBoxTracker& box = *static_cast<const ToyBoxTracker*>(&boxB); 

  if(_computecounters){
    _fgd1pur->InitializeEfficiencyCounter();
    _fgd1eff->InitializeEfficiencyCounter();
    if(versionUtils::prod6_systematics){
      _fgd2pur->InitializeEfficiencyCounter();
      _fgd2eff->InitializeEfficiencyCounter();
    }
  }
  
  Weight_h eventWeight=1;
  
  // Vertex and True vertex should exist
  if (!box.Vertex->TrueVertex)   return eventWeight;
  
  Int_t runPeriod = anaUtils::GetRunPeriod(event.EventInfo.Run);
  // Get the number of true michel electrons
  Int_t trueNME = anaUtils::GetNMichelElectrons(*(box.Vertex->TrueVertex), static_cast<SubDetId::SubDetEnum>(box.DetectorFV));
  if (trueNME<0 ) trueNME=0;

  // Must cast to EventBoxTracker since MichelElectrons are not in EventBoxB
  EventBoxTracker* EventBox = static_cast<EventBoxTracker*>(event.EventBoxes[EventBoxId::kEventBoxTracker]);

  // Get the number of reconstructed michel electrons
  UInt_t NME=EventBox->nFGDMichelElectrons[box.DetectorFV];
  // Get the number of reconstructed michel electrons
  BinnedParamsParams effparams,purparams;
  Int_t effindex, purindex;
  Int_t effindex_syst, purindex_syst;
  bool doeff= false;
  bool dopur= false; _fgd1pur->GetBinValues((float)runPeriod, purparams, purindex);  
  if(box.DetectorFV == SubDetId::kFGD1){
    doeff= _fgd1eff->GetBinValues((float)runPeriod, effparams, effindex); 
    dopur= _fgd1pur->GetBinValues((float)runPeriod, purparams, purindex);
    
    effindex_syst = effindex +  systUtils::GetBinnedParamsOffsetForSystematic(*this, *_fgd1eff);
    purindex_syst = purindex +  systUtils::GetBinnedParamsOffsetForSystematic(*this, *_fgd1pur);
    
  }
  else if(box.DetectorFV == SubDetId::kFGD2){
    if(versionUtils::prod6_systematics){
      doeff= _fgd2eff->GetBinValues((float)runPeriod, effparams, effindex); 
      dopur= _fgd2pur->GetBinValues((float)runPeriod, purparams, purindex);  
   
      effindex_syst = effindex +  systUtils::GetBinnedParamsOffsetForSystematic(*this, *_fgd2eff);
      purindex_syst = purindex +  systUtils::GetBinnedParamsOffsetForSystematic(*this, *_fgd2pur); 
    
    }
  }
  
  if (trueNME>0){
    bool found= ( NME>0 );
    
    if (doeff)
      eventWeight *= systUtils::ComputeEffLikeWeight(found, toy, GetIndex(), effindex_syst, effparams);
    
    if(_computecounters){
      if(box.DetectorFV == SubDetId::kFGD1)
        _fgd1eff->UpdateEfficiencyCounter(effindex,found); 
      else if(box.DetectorFV == SubDetId::kFGD2 && versionUtils::prod6_systematics)
        _fgd2eff->UpdateEfficiencyCounter(effindex,found); 
    }
    
    
    
  }
  if(NME>0){
    //we don't want to apply the same variation as the efficiency
    bool found = (trueNME>0);
    
    if (dopur)
      eventWeight *= systUtils::ComputeEffLikeWeight(found, toy, GetIndex(), purindex_syst, purparams);
    
    if(_computecounters){
      if(box.DetectorFV == SubDetId::kFGD1)
        _fgd1pur->UpdateEfficiencyCounter(purindex,found); 
      else if(box.DetectorFV == SubDetId::kFGD2 && versionUtils::prod6_systematics)
        _fgd2pur->UpdateEfficiencyCounter(purindex,found); 
    }
  }

  return eventWeight;
}

