#include "ECalEMEnergyResolSystematics.hxx"
#include "ND280AnalysisUtils.hxx"
#include "HEPConstants.hxx"
#include "Parameters.hxx"
#include "TRandom3.h"


//#define DEBUG
//#define TRUECROSS

//********************************************************************
SystBoxECalEMResol::SystBoxECalEMResol(){
//******************************************************************** 

  RelevantCrossings = NULL;
}

//********************************************************************
SystBoxECalEMResol::~SystBoxECalEMResol(){
//********************************************************************

  if (RelevantCrossings)
    delete [] RelevantCrossings;
  RelevantCrossings = NULL;
} 


//********************************************************************
ECalEMEnergyResolSystematics::~ECalEMEnergyResolSystematics() {
//********************************************************************
  if (_RandomGenerator)
    delete _RandomGenerator;
}
  
//********************************************************************
void ECalEMEnergyResolSystematics::InitializeRandomGenerator(){
//********************************************************************
  if (!_RandomGenerator){
    _RandomGenerator = new TRandom3();
    SetRandomSeed(ND::params().GetParameterI("psycheSystematics.ECalEMResol.RandomSeed")); 
  }
}

//********************************************************************
UInt_t ECalEMEnergyResolSystematics::GetRandomSeed() const{
//********************************************************************
  if (_RandomGenerator)
      return _RandomGenerator->GetSeed();
  return 0XDEADBEEF;
}
  
//******************************************************************** 
void ECalEMEnergyResolSystematics::SetRandomSeed(UInt_t seed){
//********************************************************************
  if (_RandomGenerator)
      _RandomGenerator->SetSeed(seed); 
}

//********************************************************************
void ECalEMEnergyResolSystematics::InitializeEvent(const AnaEventC& event, const SelectionBase& sel){
//********************************************************************

  Int_t uniqueID = 0;

#ifdef MULTITHREAD
  uniqueID = event.UniqueID;
#endif

  // Delete the SystBox when it exists and create a new one. 
  // TODO: It is probably faster to just reset the arrays

  // Create the box
  if(!_systBoxes[0][0][uniqueID])
    _systBoxes[0][0][uniqueID] = new SystBoxECalEMResol();
  
  // Fill the SystBox with the relevant tracks and true tracks tor this systematic
  FillSystBox(event, sel, *_systBoxes[0][0][uniqueID]);
}

//********************************************************************
void ECalEMEnergyResolSystematics::InitializeEvent(const AnaEventC& event){
//********************************************************************

  Int_t uniqueID = 0;

#ifdef MULTITHREAD
  uniqueID = event.UniqueID;
#else 
  (void)event;
#endif
  
#ifndef TRUECROSS
  return ECalEMEnergySystematicsBase::InitializeEvent(event);
#endif

  SystBoxECalEMResol& box =  static_cast<SystBoxECalEMResol&>(*(_systBoxes[0][0][uniqueID]));
  int ntracks = box.RelevantRecObjectsSet.size();
  if (box.RelevantRecObjects)     delete [] box.RelevantRecObjects;
  if (box.RelevantCrossings)  delete [] box.RelevantCrossings;
  anaUtils::CreateArray(box.RelevantRecObjects,    ntracks);
  anaUtils::CreateArray(box.RelevantCrossings, ntracks);
  
  box.nRelevantRecObjects=0;  
  for (std::set<AnaRecObjectC*>::iterator it = box.RelevantRecObjectsSet.begin();it!=box.RelevantRecObjectsSet.end();it++){
    box.RelevantRecObjects[box.nRelevantRecObjects] = *it;

    
    AnaDetCrossingB* cross = NULL;
    
    // get true track
    // TODO!!: should be careful here,  may work ok for ecal-tracker tracks, but for iso ones and showers, true track should be 
    // retrieved in another way
    AnaTrueParticleB* trueTrack = static_cast<AnaTrueParticleB*>((*it)->TrueObject);
    
    if (trueTrack){
      //get detector crossing in ECal
      for (int i=0; i<trueTrack->nDetCrossings; i++){
        AnaDetCrossingB* cross_tmp = trueTrack->DetCrossings[i];
        if (!cross_tmp) continue;
        if (((cross_tmp->Detector & (1<<SubDetId::kTECAL)) || (cross_tmp->Detector & (1<<SubDetId::kDSECAL))) &&
            cross_tmp->InActive){
          cross = cross_tmp;
          break;
        }
      }
    } //trueTrack
    
   box.RelevantCrossings[box.nRelevantRecObjects] = cross;
   
   box.nRelevantRecObjects++;
    
  }

}

//********************************************************************
void ECalEMEnergyResolSystematics::Apply(const ToyExperiment& toy, AnaEventC& event){
  //********************************************************************

  // Get the SystBox for this event
  SystBoxB* box = GetSystBox(event);

#ifdef DEBUG
  std::cout << "ECalEMEnergyResolSystematics::ApplyVariation(): " << box->nRelevantRecObjects << std::endl;  
#endif

  // Loop over relevant tracks for this systematic
  for (Int_t itrk=0;itrk<box->nRelevantRecObjects;itrk++){

    AnaTrackB* track = static_cast<AnaTrackB*>(box->RelevantRecObjects[itrk]);

    if (!track) continue;
     
    AnaECALParticleB* ecalTrack = track->ECALSegments[0];
    if(!ecalTrack) continue;

    // Protection against tracks with no physical values
    if(ecalTrack->EMEnergy <= 0) continue;
    
#ifdef TRUECROSS

    //should have true track
    if(!track->TrueObject) continue;

    
    //should have valid ECal crossing
    AnaDetCrossingB* cross = static_cast<SystBoxECalEM*>(box)->RelevantCrossings[itrk];
    if (!cross) continue;

# ifdef DEBUG
    std::cout << itrk << std::endl;
    track->Print();
    std::cout<<"\n"<<std::endl;
    track->TrueObject->Print();
    
# endif
    
    // Get the true energy loss
    // For the momentum assume the true track stops in ECal: ToDo (collect info from all track and its daughters` chain?)
    Float_t p0_true_entrance  = anaUtils::ArrayToTVector3(cross->EntranceMomentum).Mag();
  //  Float_t p0_true_exit2      = anaUtils::ArrayToTVector3(cross->ExitMomentum).Mag2();

    
    //Get mass
    Float_t mass = 0.;
    
    TParticlePDG* particle = units::pdgBase->GetParticle(track->GetTrueParticle()->PDG);
    
    if (particle)
      mass = particle->Mass()*units::GeV; 
    
    Float_t em0_true = sqrt(p0_true_entrance*p0_true_entrance + mass*mass) - mass;
      
    
# ifdef DEBUG
    std::cout << "track true mom entrance in ECal -- " << anaUtils::ArrayToTVector3(cross->EntranceMomentum).Mag() << std::endl;
    std::cout << "track true mom exit in ECal     -- " << anaUtils::ArrayToTVector3(cross->ExitMomentum).Mag()     << std::endl;
    std::cout << "track true mass                 -- " << mass                                                  << std::endl;
    std::cout << "track true eloss in ECal        -- " << em0_true                                              << std::endl;
# endif

#endif

    // Get reco value
    Float_t em0 = ecalTrack->EMEnergy;

#ifdef DEBUG
    std::cout << "ecal track EM Energy   -- " << em0 << std::endl;
#endif

    // Get the extra resolution values: bins are defined by EM energy
    Float_t sigma;
    Int_t index;

    if (!GetBinSigmaValue(em0, sigma, index)) continue;

#ifdef DEBUG
    std::cout << " bin values: sigma " << sigma << " index " << index << std::endl;
#endif

    
#ifdef TRUECROSS 
    // Apply the additional resolution smearing, ie smear around mean with a sigma=x
    Float_t emv = (1 + sigma*toy.GetToyVariations(_index)->Variations[index])*(em0 - em0_true) + em0_true;
#else
    //Note one will have to take care of meaning of the parameters
    // Apply the additional resolution smearing with a gaussian throw
    Float_t emv = em0 + _RandomGenerator->Gaus(0., (sigma*toy.GetToyVariations(_index)->Variations[index])*em0);
    
#endif
    
    
#ifdef DEBUG
    std::cout << "candidate EM Energy after tweak = "<< emv << std::endl;
#endif

    // Apply the variation
    ecalTrack->EMEnergy = emv;
  }

}

