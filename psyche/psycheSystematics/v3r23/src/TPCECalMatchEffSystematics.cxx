#include "TPCECalMatchEffSystematics.hxx"
#include "CutUtils.hxx"
#include "SystematicUtils.hxx"
#include "SystId.hxx"
#include <cmath>

//#define DEBUG

//********************************************************************
SystBoxTPCECalMatchEff::SystBoxTPCECalMatchEff(){
//******************************************************************** 

  RelevantTrueECalDetCrossed = NULL;
}

//********************************************************************
SystBoxTPCECalMatchEff::~SystBoxTPCECalMatchEff(){
//********************************************************************

  if (RelevantTrueECalDetCrossed)
    delete [] RelevantTrueECalDetCrossed;
  RelevantTrueECalDetCrossed = NULL;
} 

//********************************************************************
TPCECalMatchEffSystematics::TPCECalMatchEffSystematics(bool comp):BinnedParams("TPCECalMatchEff",k3D_EFF_ASSYMMETRIC){
  //********************************************************************

  _computecounters=comp;
  SetNParameters(2*GetNBins());
  if(_computecounters)
    InitializeEfficiencyCounter();
}

//********************************************************************
void TPCECalMatchEffSystematics::InitializeEvent(const AnaEventC& event, const SelectionBase& sel, Int_t ibranch){
//********************************************************************

  Int_t uniqueID = 0;

#ifdef MULTITHREAD
  uniqueID = event.UniqueID;
#endif

  // Delete the SystBox when it exists and create a new one. 
  // TODO: It is probably faster to just reset the arrays

  // Get the selection index;
  Int_t isel=sel.GetEnabledIndex();  

  // Nothing to do when this box already created and filled
  if(_systBoxes[isel][ibranch][uniqueID]) return;

  // Create the box
  _systBoxes[isel][ibranch][uniqueID] = new SystBoxTPCECalMatchEff();
  
  // Fill the SystBox with the relevant tracks and true tracks tor this systematic
  FillSystBox(event, sel, ibranch);
}

//********************************************************************
void TPCECalMatchEffSystematics::FillSystBox(const AnaEventC& event, const SelectionBase& sel, Int_t ibranch){
//********************************************************************
  //call the base method
  EventWeightBase::FillSystBox(event, sel, ibranch);
  
  Int_t uniqueID = 0;
#ifdef MULTITHREAD
  uniqueID = event.UniqueID;
#endif

  // Get the selection index;
  Int_t isel=sel.GetEnabledIndex();  

  // Get the SystBox
  SystBoxTPCECalMatchEff& box = static_cast<SystBoxTPCECalMatchEff&>(*_systBoxes[isel][ibranch][uniqueID]);

  if (box.nRelevantTrueObjects==0) return;
  
  // create the array of RelevantTrueObjects
  if (box.RelevantTrueECalDetCrossed) delete box.RelevantTrueECalDetCrossed;
  anaUtils::CreateArray(box.RelevantTrueECalDetCrossed, box.nRelevantTrueObjects);

  // Loop over relevant true tracks
  for (Int_t itrk=0; itrk<box.nRelevantTrueObjects; itrk++){

    AnaTrueParticleB* trueTrack = static_cast<AnaTrueParticleB*>(box.RelevantTrueObjects[itrk]);
    box.RelevantTrueECalDetCrossed[itrk] = SubDetId::kInvalid;
    
    if (!trueTrack) continue;
    SubDetId::SubDetEnum det[10];
    
    int ndet = anaUtils::GetECalDetCrossed(trueTrack, det);
    if (ndet<1)  continue;
    box.RelevantTrueECalDetCrossed[itrk] = det[0]; 
  }
}



//********************************************************************
Weight_h TPCECalMatchEffSystematics::ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box, const SelectionBase& sel){
  //********************************************************************


  if(_computecounters)
    InitializeEfficiencyCounter();

  (void)event;

  // Get the SystBox for this event, and the appropriate selection and branch
  SystBoxTPCECalMatchEff* SystBox = static_cast<SystBoxTPCECalMatchEff*>(GetSystBox(event, box.SelectionEnabledIndex, box.SuccessfulBranch));


  Weight_h eventWeight = 1.;

#ifdef DEBUG
  std::cout << "TPCECalMatchEffSystematics::Apply(): " << SystBox->nRelevantTrueObjects<< std::endl; 
#endif

  // Loop over relevant true tracks
  for (Int_t itrk=0;itrk<SystBox->nRelevantTrueObjects;itrk++){

    AnaTrueParticleB* truePart = static_cast<AnaTrueParticleB*>(SystBox->RelevantTrueObjects[itrk]);

    AnaTrackB* recoTrack = static_cast<AnaTrackB*>(SystBox->RelevantTrueObjectsReco[itrk]);

    if (!truePart || !recoTrack) continue;
    
    // Do fine-tuning of the track relevance via the selection
    if (!sel.IsRelevantTrueObjectForSystematicInToy(event, box, truePart, SystId::kTpcECalMatchEff, box.SuccessfulBranch)) continue;

    
    //get relevant ECal detector crossed
    SubDetId::SubDetEnum ecal_det = SystBox->RelevantTrueECalDetCrossed[itrk];
    
    if (ecal_det==SubDetId::kInvalid) continue;
    
    // Get TPC closest to the stopping point (may be should also do at box filling level)
    AnaTPCParticleB* tpcTrack =  static_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInClosestTpc(*recoTrack, truePart->PositionEnd));
    
    if (!tpcTrack) continue;
    
    BinnedParamsParams params;
    int index;
   
    int pdg = truePart->PDG;
 
    // Pions are treated as muons for the moment
    if(pdg == 211)
      pdg = 13;
    else if(pdg == -211)
      pdg = -13;
    // One bin for electron
    else if(pdg == -11)
      pdg = 11;
    // One bin for protons
    else if(pdg == -2212)
      pdg = 2212;

    if (!GetBinValues(pdg, ecal_det, tpcTrack->MomentumEnd, params, index)) continue;

    // Now check detector bits
    bool found = SubDetId::GetDetectorUsed(recoTrack->Detector, ecal_det);

#if useNewWeights 
    eventWeight *= systUtils::ComputeEffLikeWeight(found, toy.GetToyVariations(_index)->Variations[index], params); // New way including data-mc diff
#else
    eventWeight *= systUtils::ComputeEffLikeWeight(found, toy.GetToyVariations(_index)->Variations[2*index],
        toy.GetToyVariations(_index)->Variations[2*index+1], params);
#endif


#ifdef DEBUG
    std::cout<<"tpc-ecal found "<< found<<" eventWeight "<<eventWeight<<std::endl; 
#endif

    if(_computecounters)
      UpdateEfficiencyCounter(index,found);    

  }

  return eventWeight;
}

//********************************************************************
bool TPCECalMatchEffSystematics::IsRelevantTrueObject(const AnaEventC& event, const AnaTrueObjectC& track) const{
  //********************************************************************

  (void)event;

  //this is tricky,  a track should have a potential to be reconstructed in ECal,  hence probably enough momentum on entrance point
  //this can be PID dependent
 
  //for the moment just require a point of interest
  
  //a track should cross only one ECal sub-detector: these are the ones we want for analysis
  SubDetId::SubDetEnum det[20];  
 
  int necal_det = anaUtils::GetECalDetCrossed(static_cast<const AnaTrueParticleB*>(&track), det); 

  if(necal_det!=1) return false;
  
  if (SubDetId::IsTECALDetector(det[0]) || det[0] == SubDetId::kDSECAL) 
   return true;

  return false;

}

//**************************************************
bool TPCECalMatchEffSystematics::IsRelevantRecObject(const AnaEventC& event, const AnaRecObjectC& track) const{
  //**************************************************

  (void)event;

  //should use TPC: probably good quality as well?
  if (!cutUtils::TrackQualityCut(*static_cast<const AnaTrackB*>(&track))) return false;
  return true; 
}
