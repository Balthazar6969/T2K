#include "numuCCMultiPiSelection.hxx"
#include "numuCCSelection.hxx"
#include "baseSelection.hxx"
#include "SystematicTuning.hxx"
#include "EventBoxUtils.hxx"
#include "Parameters.hxx"
#include "SubDetId.hxx"
#include "SystId.hxx"
#include "SystematicUtils.hxx"
#include <DetectorDefinition.hxx>


/**
  \\! [numuCCMultiPiSelection_info]   
  This selection adds tree branches on top of the numuCCSelection. As descrived in numuCCMultiPiSelection::DefineSteps "numuCCMultiPiSelection::DefineSteps "the cuts of the numuCCSelection are copied to this selection. 
  Then new cuts for each branch are added: 

  Cuts for Branch 0: This is the branch requiring no pions
  <table>
  <tr><th> #cut  <th> cut class                        <th>  from source file   <th>  Cut description
  <tr><td> 6     <td> ExternalVetoCut::Apply           <td> numuCCSelection.cxx <td> External veto cut 
  <tr><td> 7     <td> ExternalFGD1lastlayersCut::Apply <td> numuCCSelection.cxx <td> External FGD1 last layers cut
  </table>

  Cuts for Branch 1: This is the branch requiring 1 and only 1 pion
  <table>
  <tr><th> #cut  <th> cut class                        <th>  from source file   <th>  Cut description
  <tr><td> 6     <td> ExternalVetoCut::Apply           <td> numuCCSelection.cxx <td> External veto cut 
  <tr><td> 7     <td> ExternalFGD1lastlayersCut::Apply <td> numuCCSelection.cxx <td> External FGD1 last layers cut
  </table>

  Cuts for Branch 2: This is the branch requiring more than 1 pion
  <table>
  <tr><th> #cut  <th> cut class                        <th>  from source file   <th>  Cut description
  <tr><td> 6     <td> ExternalVetoCut::Apply           <td> numuCCSelection.cxx <td> External veto cut 
  <tr><td> 7     <td> ExternalFGD1lastlayersCut::Apply <td> numuCCSelection.cxx <td> External FGD1 last layers cut
  </table>


  \\! [numuCCMultiPiSelection_info]
  */

//*******************************************************************************
numuCCMultiPiSelection::numuCCMultiPiSelection(bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxTracker){
  //*******************************************************************************

  // Initialize systematic tuning parameters
  systTuning::Initialize();
  
  _useECalPiZeroInfo = (bool)ND::params().GetParameterI("psycheSelections.numuCCMultiPi.UseECalPiZeroInfo");
}

/// This is the method where the actual steps are added to the selection
/// \anchor numuCCMultiPiSelection_DefineSteps
//*******************************************************************************
void numuCCMultiPiSelection::DefineSteps(){
  //*******************************************************************************

  // Copy all steps from the numuCCSelection
  CopySteps(_numuCCSelection);

  //Additional actions for the multi-pi selection.
  AddStep(StepBase::kAction, "fill_summary_multiPi", new FillSummaryAction_numuCCMultiPi());
  AddStep(StepBase::kAction, "find_pions",           new FindPionsAction());

  //Add a split to the trunk with 3 branches.
  AddSplit(3);

  //First branch is for CC-0pi
  AddStep(0, StepBase::kCut, "CC-0pi",        new NoPionCut());

  AddStep(0, StepBase::kCut, "ECal Pi0 veto", new EcalPi0VetoCut());
  

  //Second branch is for CC-1pi: true -- Positive pion
  AddStep(1, StepBase::kCut, "CC-1pi",        new OnePionCut(true));

  AddStep(1, StepBase::kCut, "ECal Pi0 veto", new EcalPi0VetoCut());

  //Third branch is for CC-Other
  AddStep(2, StepBase::kCut, "CC-Other", new OthersCut());

  // Set the branch aliases to the three branches
  SetBranchAlias(0,"CC-0pi",  0);
  SetBranchAlias(1,"CC-1pi",  1);
  SetBranchAlias(2,"CC-Other",2);

  // By default the preselection correspond to cuts 0-2
  SetPreSelectionAccumLevel(2);

  // Step and Cut numbers needed by CheckRedoSelection
  _MuonPIDCutIndex     = GetCutNumber("muon PID");
  _FindPionsStepIndex  = GetStepNumber("find_pions");
}


//*******************************************************************************
void numuCCMultiPiSelection::DefineDetectorFV(){
  //*******************************************************************************

  // The detector in which the selection is applied
  SetDetectorFV(SubDetId::kFGD1);
}

//********************************************************************
bool numuCCMultiPiSelection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
  //********************************************************************

  //CC0pi
  if(allCutsPassed[0]){
    static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD1NuMuCC0Pi;
  }  
  //CC1pi
  else if (allCutsPassed[1]){
    static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD1NuMuCC1Pi;
  }
  //CCOther
  else if (allCutsPassed[2]){
    static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD1NuMuCCOther;
  }

  // otherwise kUnassigned is used from the EventSummary constructor
  return (static_cast<AnaEventSummaryB*>(event.Summary)->EventSample != SampleId::kUnassigned);
}


//*********************************************************************
bool FillSummaryAction_numuCCMultiPi::Apply(AnaEventC& event, ToyBoxB& boxB) const{
  //*********************************************************************

  // Cast the ToyBox to the appropriate type
  ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 


  if(!box.HMNtrack) return 1;

  static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD1NuMuCC0Pi] = box.HMNtrack;
  static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD1NuMuCC1Pi] = box.HMNtrack;
  static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD1NuMuCCOther] = box.HMNtrack;

  for(int i = 0; i < 4; ++i){
    static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD1NuMuCC0Pi][i] = box.HMNtrack->PositionStart[i];
    static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD1NuMuCC1Pi][i] = box.HMNtrack->PositionStart[i];
    static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD1NuMuCCOther][i] = box.HMNtrack->PositionStart[i];
  }
  if(box.HMNtrack->GetTrueParticle()){
    static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD1NuMuCC0Pi] = box.HMNtrack->GetTrueParticle()->TrueVertex;
    static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD1NuMuCC1Pi] = box.HMNtrack->GetTrueParticle()->TrueVertex;
    static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD1NuMuCCOther] = box.HMNtrack->GetTrueParticle()->TrueVertex;
  }
  return 1;
}

//*********************************************************************
bool FindPionsAction::Apply(AnaEventC& event, ToyBoxB& box) const{
  //*********************************************************************

  ToyBoxCCMultiPi* ccmultipibox = static_cast<ToyBoxCCMultiPi*>(&box);

  pionSelParams.refTrack = ccmultipibox->MainTrack;

  ccmultipibox->pionBox.Detector = (SubDetId::SubDetEnum)box.DetectorFV;

  // Fill the info
  cutUtils::FillPionInfo(event, ccmultipibox->pionBox, pionSelParams);

  int nnegpions        = ccmultipibox->pionBox.nNegativePionTPCtracks;
  int npospions        = ccmultipibox->pionBox.nPositivePionTPCtracks;
  int nisofgdpions     = ccmultipibox->pionBox.nIsoFGDPiontracks;
  int nmichelelectrons = ccmultipibox->pionBox.nMichelElectrons;
  int npi0             = ccmultipibox->pionBox.nPosPi0TPCtracks + ccmultipibox->pionBox.nElPi0TPCtracks;

  int pionFGD = nmichelelectrons;
  if (!nmichelelectrons && nisofgdpions > 0) pionFGD = 1;

  ccmultipibox->pionBox.nPosPions   = npospions + pionFGD;
  ccmultipibox->pionBox.nOtherPions = nnegpions + npi0;
  return true;
}

//**************************************************
bool numuCCMultiPiSelection::IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* track, SystId_h systId, Int_t branch) const{
  //**************************************************

  return _numuCCSelection.IsRelevantRecObjectForSystematic(event,track,systId,branch);
}

//**************************************************
bool numuCCMultiPiSelection::IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueTrack, SystId_h systId, Int_t branch) const{
  //**************************************************

  return _numuCCSelection.IsRelevantTrueObjectForSystematic(event,trueTrack,systId,branch);
}

//**************************************************
bool numuCCMultiPiSelection::IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& box, AnaRecObjectC* track, SystId_h systId, Int_t branch) const{
  //**************************************************

  if (!track) return false;
  (void)event;

  // all CC inclusive cases
  // APPLY_SYST_FINE_TUNING is taken into account as well (will return true if not used), hence in what follows assume
  // APPLY_SYST_FINE_TUNING case ON
  // This will also take care of the "main" track 
  if (_numuCCSelection.IsRelevantSystematic (event, box, systId, branch) && 
      _numuCCSelection.IsRelevantRecObjectForSystematicInToy(event,box,track,systId,branch)) return true;

  const ToyBoxCCMultiPi* ccmultipibox = static_cast<const ToyBoxCCMultiPi*>(&box);

  switch (systId){
    // Fall back through the relevant ones
    // TPC reco
    case SystId::kChargeIDEff:
      // TPC matching
    case SystId::kTpcFgdMatchEff:

      // Use the APPLY_SYST_FINE_TUNING concept as well,  since we worry about the "main" track that defines the topology 
      if (branch == 1){
        // For CC-1pi also the positive Pion in the TPC matters
        if (ccmultipibox->pionBox.nPositivePionTPCtracks==1 && track==ccmultipibox->pionBox.PositivePionTPCtracks[0]) return true; 
      }
      else if (branch == 2){
        // For CC-Other consider the negative pion when there are no other pions
        if (ccmultipibox->pionBox.nNegativePionTPCtracks==1 && track==ccmultipibox->pionBox.NegativePionTPCtracks[0] && 
            ccmultipibox->pionBox.nOtherPions==1 && ccmultipibox->pionBox.nPosPions == 0) return true;  

      }
      // Failed above + 
      // CC0pi explicit false (should have been covered by numuCC if tuning is ON)
      return false;
      break;
    case SystId::kECalPID:
      // The ECal pi-zero photon veto takes highest-enrgetic iso ECal object and
      // checks whether it is a shower,  reflect in in the tuning
      
      // If the track corresponds to the selected candidate
      if (ccmultipibox->pionBox.ECalPi0Photon){
        return (track == ccmultipibox->pionBox.ECalPi0Photon);
      }
      
      // Then if it is a highest energetic iso object since it is the one used
      // for the cuts
      if ( track == cutUtils::GetMostEnergeticIsoTrackInECal(event))
        return true;

      return false;
       
      // The rest of the systematics go here, tuning to come
    default:
      return true;
      break;
  }

  return true;

}


//**************************************************
bool numuCCMultiPiSelection::IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& box, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
  //**************************************************

  const ToyBoxCCMultiPi* ccmultipibox = static_cast<const ToyBoxCCMultiPi*>(&box);
  
  AnaTrueParticleB* trueTrack = static_cast<AnaTrueParticleB*>(trueObj);
  if (!trueTrack) return false;

  // All CC inclusive cases
  // MAIN track mode is taken into account there
  if (_numuCCSelection.IsRelevantSystematic (event, box, systId, branch) && 
      _numuCCSelection.IsRelevantTrueObjectForSystematicInToy(event,box,trueTrack,systId,branch)) return true;

  switch (systId){
    case SystId::kTpcTrackEff:
      
      // Don't consider pions when there are many TPC ones (a conservative check)
      // Cases in which we must loose two or more pions are excluded (0.01*0.01 probability)
      if (ccmultipibox->pionBox.nNegativePionTPCtracks+ccmultipibox->pionBox.nPositivePionTPCtracks > 2 ||
          ccmultipibox->pionBox.nNegativePionTPCtracks > 1) return false; 
      
      // If the are few TPC pions consider them, muons and electrons 
      if (abs(trueTrack->PDG)==211 || abs(trueTrack->PDG)==13 || abs(trueTrack->PDG)==11) return true;  
      
      return false;
      break;
      
    // SI
    case SystId::kSIPion:
      
      // No Pion SI systematic when there are many pions 
      if (ccmultipibox->pionBox.nOtherPions >1 || ccmultipibox->pionBox.nPosPions > 2 ) return false; 
      return true;
      break;
    
    // Contributors for relevant objects for proton SI
    case SystId::kSIProton:
      
      // Main track is done by numuCC
      // If there are  many pions, i.e. topology cannot change, then do not use
      // the systematic
      if (ccmultipibox->pionBox.nOtherPions >1 || ccmultipibox->pionBox.nPosPions > 2 ) return false; 
      // Otherwise check that a pion contributes to any of the objects in the
      // box
      
      return cutUtils::numuCCTrkMultiPi::CheckTrueCausesBoxSimple(*trueTrack, ccmultipibox->pionBox);
      break;
    case SystId::kECalTrackEff:
      
      // Have to check an object is from a real pi0 corresponding to the vertex
      if (ccmultipibox->pionBox.ECalPi0Photon && 
          ccmultipibox->pionBox.ECalPi0Photon->GetTrueParticle() == trueTrack)
        return true;
      
      // Check if a true object is a daughter from a primary pi-zero: 
      // Need to be careful here due to the true/reco association for showering
      // objects! Consider daughters from pi-zero gammas for the moment: TODO
      // Only do this if have not found and pi-zero photon candidate in ECal
      if (!ccmultipibox->pionBox.ECalPi0Photon && trueTrack->IsTruePrimaryPi0DecayPhotonChild){
        return true;
      }
      
      return false;
      break;
    default:
      return true;
      break;
  }

  // For the rest of the systematic return true
  return true;
}

//**************************************************
bool numuCCMultiPiSelection::IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const{
  //**************************************************

  if (!systTuning::APPLY_SYST_FINE_TUNING) return true;

  (void)event;
  const ToyBoxCCMultiPi* ccmultipibox = static_cast<const ToyBoxCCMultiPi*>(&box);
  
    switch (systId){
      case SystId::kECalTrackEff:
      case SystId::kECalPID:
      
        // If ECal pi-zero is not used, then no ECal syst
        if (!_useECalPiZeroInfo)
          return false;

  
        // Apply for first two branches
        if (branch == 0 || branch == 1){
          return true;
        }
      
        // For branch==2,  apply if the ECal is the one that defines the topology
      if (ccmultipibox->pionBox.nOtherPions == 0 && ccmultipibox->pionBox.nPosPions < 2 && ccmultipibox->pionBox.nNegPions < 2){ //Check both (Neg + Pos) so to re-use in antinumu
        return true;
      }
    
      return false;  
      break;
    default:
      return true;
      break;
  }
  return true;
}

//**************************************************
Int_t numuCCMultiPiSelection::GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
  //**************************************************
  Int_t ngroups = _numuCCSelection.GetRelevantRecObjectGroupsForSystematic(systId, IDs, branch);
  if (ngroups) return ngroups;

  // Consider the systematics specific for the analysis
  std::vector<std::vector<Int_t> > groups(SystId::SystEnumLast_SystId, std::vector<Int_t>(2, -1));


  // Explicitely set the groups for various systeamtics that need them, 
  // this allows to have a transparent set/control of the systematics by the
  // selection  

  using namespace SystId;

  // --- Systematic         Detectors                                   FGD1                                    FGD2 
  groups[kFgdPid            ] =  anaUtils::CreateVectorI(2, EventBoxTracker::kTracksWithFGD1AndNoTPC,  EventBoxTracker::kTracksWithFGD2AndNoTPC);
  groups[kFgdHybridTrackEff ] =  anaUtils::CreateVectorI(2, EventBoxTracker::kTracksWithFGD1AndNoTPC,  EventBoxTracker::kTracksWithFGD2AndNoTPC);
  groups[kECalEMResol       ] =  anaUtils::CreateVectorI(2, EventBoxTracker::kTracksWithECal,          EventBoxTracker::kTracksWithECal);
  groups[kECalEMScale       ] =  anaUtils::CreateVectorI(2, EventBoxTracker::kTracksWithECal,          EventBoxTracker::kTracksWithECal);
  groups[kECalTrackEff      ] =  anaUtils::CreateVectorI(2, EventBoxTracker::kTracksWithECal,          EventBoxTracker::kTracksWithECal);
  groups[kECalPID           ] =  anaUtils::CreateVectorI(2, EventBoxTracker::kTracksWithECal,          EventBoxTracker::kTracksWithECal);

  // The systematics not mentioned above will get no groups
  // Check the systematic index is ok
  try {
    groups.at(systId);
  }
  catch (const std::out_of_range& oor) {
    std::cerr << this->Name() << " GetRelevantTrueObjectGroupsForSystematic: syst index beyond limits "<< systId << " Error " << oor.what() << '\n';
    exit(1);
  }

  SubDetId_h det = GetDetectorFV(branch);

  if (det == SubDetId::kFGD1){
    if (groups[systId][0] >= 0) IDs[ngroups++] = groups[systId][0];
  }
  else   if (det == SubDetId::kFGD2){
    if (groups[systId][1] >= 0) IDs[ngroups++] = groups[systId][1];
  }
  else if (det == SubDetId::kFGD){
    if (groups[systId][0] >= 0) IDs[ngroups++] = groups[systId][0];
    if (groups[systId][0] != groups[systId][1]){
      IDs[ngroups++] = groups[systId][1];
    }   
  }
  return ngroups;

}

//**************************************************
Int_t numuCCMultiPiSelection::GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
  //**************************************************

  Int_t ngroups = _numuCCSelection.GetRelevantTrueObjectGroupsForSystematic(systId, IDs, branch);
  if (ngroups) return ngroups;

  // Consider the systematics specific for the analysis
  std::vector<std::vector<Int_t> > groups(SystId::SystEnumLast_SystId, std::vector<Int_t>(2, -1));


  // Explicitely set the groups for various systeamtics that need them, 
  // this allows to have a transparent set/control of the systematics by the
  // selection  

  using namespace SystId;
  using namespace anaUtils;

  // --- Systematic         Detectors                                   FGD1                                                    FGD2 
  groups[kFgdHybridTrackEff ] =  CreateVectorI(2, EventBoxTracker::kTrueParticlesChargedInFGD1AndNoTPCInBunch,  EventBoxTracker::kTrueParticlesChargedInFGD1AndNoTPCInBunch);
  groups[kECalTrackEff      ] =  CreateVectorI(2, EventBoxTracker::kTrueParticlesInECalInBunch,                 EventBoxTracker::kTrueParticlesInECalInBunch);


  // The systematics not mentioned above will get no groups
  // Check the systematic index is ok
  try {
    groups.at(systId);
  }
  catch (const std::out_of_range& oor) {
    std::cerr << this->Name() << " GetRelevantTrueObjectGroupsForSystematic: syst index beyond limits "<< systId << " Error " << oor.what() << '\n';
    exit(1);
  }

  SubDetId_h det = GetDetectorFV(branch);

  if (det == SubDetId::kFGD1){
    if (groups[systId][0] >= 0) IDs[ngroups++] = groups[systId][0];
  }
  else   if (det == SubDetId::kFGD2){
    if (groups[systId][1] >= 0) IDs[ngroups++] = groups[systId][1];
  }
  else if (det == SubDetId::kFGD){
    if (groups[systId][0] >= 0) IDs[ngroups++] = groups[systId][0];
    if (groups[systId][0] != groups[systId][1]){
      IDs[ngroups++] = groups[systId][1];
    }   
  }


  return ngroups;

}
//**************************************************
void numuCCMultiPiSelection::InitializeEvent(AnaEventC& eventBB){
  //**************************************************

  AnaEventB& event = *static_cast<AnaEventB*>(&eventBB); 

  _numuCCSelection.InitializeEvent(event);
  boxUtils::FillTracksWithECal(event);
  boxUtils::FillFGDMichelElectrons(event, (SubDetId::SubDetEnum)GetDetectorFV(),
      (bool)ND::params().GetParameterI("psycheSelections.numuCCMultiPi.Prod5Cuts"));

  boxUtils::FillTrajsInECal(event);
}

//********************************************************************
bool numuCCMultiPiSelection::CheckRedoSelection(const AnaEventC& event, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep){
  //********************************************************************

  // Must redo selection if numuCCSelection decides so
  if( _numuCCSelection.CheckRedoSelection(event,PreviousToyBox,redoFromStep)) return true;

  // Otherwise selection must be redone From the FindPionsAction, but only when the Muon PID cut is passed in the previous toy 
  if (PreviousToyBox.MaxAccumLevel>_MuonPIDCutIndex){
    redoFromStep=_FindPionsStepIndex;
    return true;
  }
  return false;
}

//********************************************************************
const multipi::MultiPiBox& numuCCMultiPi::GetPionBox(const ToyBoxB& box){
  //********************************************************************
  const ToyBoxCCMultiPi& ccmultipibox = static_cast<const ToyBoxCCMultiPi&>(box);
  return ccmultipibox.pionBox;  
}



