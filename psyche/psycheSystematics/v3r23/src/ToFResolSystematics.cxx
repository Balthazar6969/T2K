#include "ToFResolSystematics.hxx"
#include "EventBoxTracker.hxx"
#include "Parameters.hxx"

//********************************************************************
ToFResolSystematics::ToFResolSystematics():EventVariationBase(1), BinnedParams("ToFResolution", k1D_SYMMETRIC_NOMEAN){
//********************************************************************
  
  SetNParameters(GetNBins());
  
  // Get the systematic source values 
  GetSigmaValueForBin(0, _sigma_FGD1FGD2 );
  GetSigmaValueForBin(1, _sigma_FGD1P0D_Track  );
  GetSigmaValueForBin(2, _sigma_FGD1ECAL_LAStartFgd_Track );
  GetSigmaValueForBin(3, _sigma_FGD1ECAL_LAStartFgd_Shower );
  GetSigmaValueForBin(4, _sigma_FGD1ECAL_LAEndFgd_Track );
  GetSigmaValueForBin(5, _sigma_FGD1ECAL_LAEndFgd_Shower );
  GetSigmaValueForBin(6, _sigma_FGD1ECAL_HAStartFgd_Track );
  GetSigmaValueForBin(7, _sigma_FGD1ECAL_HAStartFgd_Shower );
  GetSigmaValueForBin(8, _sigma_FGD1ECAL_HAEndFgd_Track );
  GetSigmaValueForBin(9, _sigma_FGD1ECAL_HAEndFgd_Shower );
}

//********************************************************************
void ToFResolSystematics::Apply(const ToyExperiment& toy, AnaEventC& event){
//********************************************************************

  // Get the SystBox for this event
  SystBoxB* box = GetSystBox(event);
  if (!box) return;
 
  // Loop though the tracks and apply the smearing
  for (int i = 0; i<box->nRelevantRecObjects; i++){
    AnaTrackB* track = static_cast<AnaTrackB*>(box->RelevantRecObjects[i]);

    ToFSenseCorrector::ToFTopology topo = ToFSenseCorrector::GetToFTopology(*track);

     if(topo == ToFSenseCorrector::kToFTopo_FGD1FGD2)
      track->ToF.FGD1_FGD2 += _sigma_FGD1FGD2 * toy.GetToyVariations(_index)->Variations[0];

    if(topo == ToFSenseCorrector::kToFTopo_FGD1P0D_Track) 
      track->ToF.P0D_FGD1 +=  _sigma_FGD1P0D_Track * toy.GetToyVariations(_index)->Variations[1];

    if(topo == ToFSenseCorrector::kToFTopo_FGD1ECAL_LAStartFgd_Track) 
      track->ToF.ECal_FGD1 +=  _sigma_FGD1ECAL_LAStartFgd_Track*toy.GetToyVariations(_index)->Variations[2];
    if(topo == ToFSenseCorrector::kToFTopo_FGD1ECAL_LAStartFgd_Shower) 
      track->ToF.ECal_FGD1 +=  _sigma_FGD1ECAL_LAStartFgd_Shower*toy.GetToyVariations(_index)->Variations[3];
    if(topo == ToFSenseCorrector::kToFTopo_FGD1ECAL_LAEndFgd_Track)  
      track->ToF.ECal_FGD1 +=  _sigma_FGD1ECAL_LAEndFgd_Track*toy.GetToyVariations(_index)->Variations[4];
    if(topo == ToFSenseCorrector::kToFTopo_FGD1ECAL_LAEndFgd_Shower) 
      track->ToF.ECal_FGD1 +=  _sigma_FGD1ECAL_LAEndFgd_Shower*toy.GetToyVariations(_index)->Variations[5];
    if(topo == ToFSenseCorrector::kToFTopo_FGD1ECAL_HAStartFgd_Track)  
      track->ToF.ECal_FGD1 +=  _sigma_FGD1ECAL_HAStartFgd_Track*toy.GetToyVariations(_index)->Variations[6];
    if(topo == ToFSenseCorrector::kToFTopo_FGD1ECAL_HAStartFgd_Shower) 
      track->ToF.ECal_FGD1 +=  _sigma_FGD1ECAL_HAStartFgd_Shower*toy.GetToyVariations(_index)->Variations[7];
    if(topo == ToFSenseCorrector::kToFTopo_FGD1ECAL_HAEndFgd_Track)  
      track->ToF.ECal_FGD1 +=  _sigma_FGD1ECAL_HAEndFgd_Track*toy.GetToyVariations(_index)->Variations[8];
    if(topo == ToFSenseCorrector::kToFTopo_FGD1ECAL_HAEndFgd_Shower) 
      track->ToF.ECal_FGD1 +=  _sigma_FGD1ECAL_HAEndFgd_Shower*toy.GetToyVariations(_index)->Variations[9];
  }

}

//********************************************************************
bool ToFResolSystematics::UndoSystematic(AnaEventC& event){
//********************************************************************

  // Get the SystBox for this event
  SystBoxB* box = GetSystBox(event);
  
  for (int i = 0; i < box->nRelevantRecObjects; i++){
    AnaTrackB* track = static_cast<AnaTrackB*>(box->RelevantRecObjects[i]);
    if(!track->Original){ std::cout<<"ToFResolSystematics::UndoSystematic:: No original object , not able to undo variation"<<std::endl; continue; }
    const AnaTrackB* orig = static_cast<const AnaTrackB*>(track->Original);
    
    if(track->ToF.Flag_FGD1_FGD2)
      track->ToF.FGD1_FGD2 = orig->ToF.FGD1_FGD2;
    
    if(track->ToF.Flag_P0D_FGD1)
      track->ToF.P0D_FGD1 = orig->ToF.P0D_FGD1;
    
    if(track->ToF.Flag_ECal_FGD1)
      track->ToF.ECal_FGD1 = orig->ToF.ECal_FGD1;
    
    if(track->ToF.Flag_ECal_FGD2)
      track->ToF.ECal_FGD2 = orig->ToF.ECal_FGD2;
  }
  
  return false;
}
