#include "SIProtonSystematics.hxx"
#include "ND280AnalysisUtils.hxx"
#include "MultiThread.hxx"
#include "SystId.hxx"

//#define DEBUG

//********************************************************************
SIProtonSystematics::SIProtonSystematics():EventWeightBase(1),BinnedParams("ProtonSI",k1D_SYMMETRIC){
  //********************************************************************

  SetNParameters(GetNBins());

  // Get the systematic source values 
  GetParametersForBin(0, _xsecScale, _xsecError);

  _ProtonInteractionsInfo = NULL;
  _initialized = false;
  
}

//********************************************************************
SIProtonSystematics::~SIProtonSystematics(){
  //********************************************************************

  if (_ProtonInteractionsInfo){
    for (Int_t k= 0; k<_nevents; k++){
      if(_ProtonInteractionsInfo[k])
        delete _ProtonInteractionsInfo[k];   
      _ProtonInteractionsInfo[k] =NULL;
    }
  }

  delete [] _ProtonInteractionsInfo;

}

//********************************************************************
void SIProtonSystematics::Initialize(){
  //********************************************************************

  // Set the proper propagator 
  _pInfoManager.SetPropagator(new proton_si::ProtonSIPropagator());

}


//********************************************************************
Weight_h SIProtonSystematics:: ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, 
    const ToyBoxB& box, const SelectionBase& sel){
  //********************************************************************

  (void)box;

  Weight_h eventWeightFinal;
   
  Float_t eventWeight   = 1.;

  Float_t totalReWeight = 1.;

  Int_t uniqueID = 0;

#ifdef MULTITHREAD
  uniqueID = event.UniqueID;
#else
  (void)event;
#endif

  // Retrieve the proton secondary interaction information for the event  
  si_syst::SISystInput& protonSI = *_ProtonInteractionsInfo[uniqueID];

#ifdef DEBUG
  std::cout << " ===== " << std::endl;
  std::cout << " SIProtonSystematics::Apply" << std::endl;
  std::cout << "nProtons = " << protonSI.nParticles << std::endl;
#endif

  // Only bother doing anything if there is at least one proton in the event.
  if (protonSI.nParticles == 0) return eventWeight;

  // Loop over the protons
  for (int i = 0; i < protonSI.nParticles; i++){


    // Weight for survivial probability
    Float_t survWeight_tmp = 0.;

    // Weight for interactions
    Float_t intWeight_tmp = 1.;   

    // Fine-tuning on whether a track is relevant for the selection
    if (!sel.IsRelevantTrueObjectForSystematicInToy(event, box, const_cast<AnaTrueParticleB*>(protonSI.Particles[i].trueTrack), 
          SystId::kSIProton, box.SuccessfulBranch)){ 
 
#ifdef DEBUG
      std::cout << " Not relevant object  " << std::endl;
#endif      
      continue;
    
    
    }


    // Start with survival probability, consider the steps available for each proton

    size_t nSteps = protonSI.Particles[i].propSteps.size();

#ifdef DEBUG
    std::cout << "nSteps = " << nSteps << std::endl;          
#endif

    for (size_t ns = 0; ns < nSteps; ns++){
      // For each step loop over the interactions
      size_t nInts = protonSI.Particles[i].propSteps[ns].size();
      for (size_t nint = 0; nint < nInts; nint++){
        // Get the variation index depending on particle and interaction type -->ToDo
        // ToDo: add interaction type into the cross-section data
        Float_t xsec_tmp = protonSI.Particles[i].propSteps[ns][nint].XSec + 
            protonSI.Particles[i].propSteps[ns][nint].XSecErr * toy.GetToyVariations(_index)->Variations[0];
        survWeight_tmp += std::max<Double_t>(xsec_tmp, 0.) - 
          protonSI.Particles[i].propSteps[ns][nint].XSec;
      }
    }


    // Get the variation index depending on particle and interaction type  
    Float_t var =  toy.GetToyVariations(_index)->Variations[0]; // one for the moment 
    
    intWeight_tmp *= 1 + protonSI.Particles[i].XSecData.XSecErr * var;

    totalReWeight *= protonSI.Particles[i].Weight;

#ifdef DEBUG
    std::cout << " ===== Particle weight ===== " << std::endl;  
    std::cout << "survWeight        \t= " << survWeight_tmp                << std::endl;
    std::cout << "exp(-survWeight)  \t= " << exp(-survWeight_tmp)          << std::endl;
    std::cout << "intWeight         \t= " << intWeight_tmp                 << std::endl;
    std::cout << "ReWeight          \t= " << protonSI.Particles[i].Weight << std::endl;
    std::cout << " ========== " << std::endl;  
#endif

    Float_t factor = exp(-survWeight_tmp) * intWeight_tmp;
    if (factor == 0 || !TMath::Finite(factor) || factor != factor)
      factor = 1.; 

    eventWeight *= factor;

  }//loop over protons

#ifdef DEBUG
  std::cout << " ===== Total weight ===== " << std::endl;  
  std::cout << " eventWeight    \t= " << eventWeight   << std::endl;
  std::cout << " totalReWeight  \t= " << totalReWeight << std::endl;
  std::cout << " ========== " << std::endl;  
#endif



  if (eventWeight == 0 || !TMath::Finite(eventWeight) || eventWeight != eventWeight)
    eventWeight = 1.; 

#ifdef DEBUG
  std::cout << " eventWeight    \t= " << eventWeight                  << std::endl;
  std::cout << " totalReWeight  \t= " << totalReWeight                << std::endl;
  std::cout << " finalWeightt   \t= " << totalReWeight * eventWeight  << std::endl;
  std::cout << " =====  " << std::endl;
#endif


  eventWeightFinal.Correction = totalReWeight;
  eventWeightFinal.Systematic = totalReWeight * eventWeight;

  
  return eventWeightFinal;

}


//********************************************************************
void SIProtonSystematics::FillSystBox(const AnaEventC& eventC, const SelectionBase& sel, Int_t ibranch){
  //********************************************************************

  Int_t uniqueID = 0;    
#ifdef MULTITHREAD
  uniqueID = eventC.UniqueID;
#endif

  const AnaEventB& event = *static_cast<const AnaEventB*>(&eventC); 

  // TODO:Need a better way of doing this
  AnaTrueParticleB* allTrajInBunch[NMAXTRUEPARTICLES];
  int count = 0;
  count = anaUtils::GetAllTrajInBunch(event, allTrajInBunch);
  if((UInt_t)count > NMAXTRUEPARTICLES) count = NMAXTRUEPARTICLES;

  // Compute Proton weight info needed by ProtonSISystematics (TODO, only when this systematic is enabled)
  //  if (ProtonWeightInfo) delete ProtonWeightInfo;
  
  _ProtonInteractionsInfo[uniqueID] = _pInfoManager.CollectParticleInteractions(allTrajInBunch, count, 
      static_cast<SubDetId::SubDetEnum>(sel.GetDetectorFV(ibranch)));

}

//********************************************************************
void SIProtonSystematics::InitializeSystBoxes(Int_t nsel, Int_t isel, Int_t nbranches, Int_t nevents){
  //********************************************************************
  
  //  TODO: This is a temporary solution to make this work with multithreading. Instead we would need a derived SystBox

  // Although the SystBox is actually not used we need to create it because SystematicBase::InitializeEvent uses it
  EventWeightBase::InitializeSystBoxes(nsel,isel,nbranches,nevents);

  // Since this is called for several selections use for the moment a single one for all selections
  if (_ProtonInteractionsInfo) return;

#ifndef MULTITHREAD
  nevents=1;
#endif

  _ProtonInteractionsInfo = new si_syst::SISystInput*[nevents];
  _nevents = nevents;
  for (Int_t k = 0; k < nevents; k++)
    _ProtonInteractionsInfo[k] = NULL;

}


//********************************************************************
void SIProtonSystematics::FinalizeEvent(const AnaEventC& event){
  //********************************************************************

  //  TODO: This is a temporary solution to make this work with multithreading. Instead we would need a derived SystBox

  // Although the SystBox is actually not used we need to create it because SystematicBase::FinalizeEvent uses it
  EventWeightBase::FinalizeEvent(event);

  Int_t uniqueID = 0;
#ifdef MULTITHREAD
  uniqueID = event.UniqueID;
#endif

  if (_ProtonInteractionsInfo[uniqueID])
    delete _ProtonInteractionsInfo[uniqueID];
  _ProtonInteractionsInfo[uniqueID] = NULL;

}
