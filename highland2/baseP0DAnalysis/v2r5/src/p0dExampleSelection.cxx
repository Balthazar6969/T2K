#include "p0dExampleSelection.hxx"
#include "baseSelection.hxx"
#include "DataClasses.hxx"
#include "CutUtils.hxx"
#include "SubDetId.hxx"
#include "EventBoxUtils.hxx"
#include "P0DAnalysisUtils.hxx"


bool _debug=false;
bool _isUsingReconDirP0D=false;
bool _isUsingReconDirP0DNew=false;

//**********************************************************************
p0dExampleSelection::p0dExampleSelection(bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxP0D) {
//**********************************************************************

  // Minimum accum level to save event into the output Micro-trees
  _debug = ND::params().GetParameterI("baseP0DAnalysis.Example.DebugSelection");

  // for whether or not P0D Recon directory variables are read
  _isUsingReconDirP0D = ND::params().GetParameterI("highlandIO.FlatTree.UseReconDirP0D");

  // for whether or not P0D Recon directory variables are read
  _isUsingReconDirP0DNew = ND::params().GetParameterI("highlandIO.P0DDataClasses.UseReconDirP0DNew");

}

//************************************************************************
void p0dExampleSelection::DefineDetectorFV() {
//************************************************************************
  SetDetectorFV(SubDetId::kP0D);  
}

//************************************************************************
void p0dExampleSelection::DefineSteps() {
//************************************************************************

  // Cuts must be added in the right order
  // last "true" means the step sequence is broken if cut is not passed (default is "false")
  AddStep(StepBase::kCut,    "event quality",       new EventQualityCut(),        true);   // from baseSelection in psycheSelections
  AddStep(StepBase::kAction, "count showers/tracks",new CountTracksAndShowersAction());
  AddStep(StepBase::kAction, "find true vertex",    new FindTrueVertexAction());
  AddStep(StepBase::kCut,    "2 showers",           new TwoShowersCut());
  AddStep(StepBase::kCut,    "0 tracks",            new NoTracksCut());
  AddStep(StepBase::kCut,    "0 michel electrons",  new NoMichelElectronsCut());

  // This is mandatory. An alias should be given to each of the branches
  SetBranchAlias(0,"trunk");  
}

//************************************************************************
void p0dExampleSelection::InitializeEvent(AnaEventC& eventC){
//************************************************************************

  //Create the appropriate EventBox if it does not exist yet

  // The code below is currently not needed !!!!

  AnaEventB& event = *static_cast<AnaEventB*>(&eventC); 

  if(!event.EventBoxes[EventBoxId::kEventBoxP0D])
    event.EventBoxes[EventBoxId::kEventBoxP0D] = new EventBoxP0D2();
  
  boxP0DUtils::FillVerticesFinal(event);
  boxP0DUtils::FillP0DParticles(event); 
}

//***********************************************************************
bool CountTracksAndShowersAction::Apply(AnaEventC& eventC, ToyBoxB& box) const {
//***********************************************************************

  // downcast the AnaEventC to AnaEventB
  AnaEventB& event = *static_cast<AnaEventB*>(&eventC); 

  // downcast the box to the appropriate type
  ToyBoxP0D* p0dBox = static_cast<ToyBoxP0D*>(&box);

  if (_isUsingReconDirP0DNew){    
    // Get the relevant EventBox. No need to downcast since we only want things in the base class 
    EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxP0D];

    p0dBox->nShowers = EventBox->nRecObjectsInGroup[EventBoxP0D2::kP0DShowers];
    p0dBox->nTracks  = EventBox->nRecObjectsInGroup[EventBoxP0D2::kP0DTracks];

    if (p0dBox->nShowers>0) p0dBox->Shower1 = static_cast<AnaP0DParticle*>(EventBox->RecObjectsInGroup[EventBoxP0D2::kP0DShowers][0]);
    if (p0dBox->nShowers>1) p0dBox->Shower2 = static_cast<AnaP0DParticle*>(EventBox->RecObjectsInGroup[EventBoxP0D2::kP0DShowers][1]);
  }
  else if (_isUsingReconDirP0D){
    AnaLocalReconEvent* localEvent = static_cast<AnaLocalReconEvent*>(&event);  
    for(std::vector<AnaP0DReconVertex*>::iterator it =localEvent->P0DReconVertices.begin(); it!= localEvent->P0DReconVertices.end(); ++it ){
      if (_debug)
        std::cout << "P0DReconVertices:AlgorithmName : " << (*it)->AlgorithmName << " Cycle: " << (*it)->Cycle 
                  << " Bunch: " << (*it)->Bunch << std::endl;
      
      Int_t i=0;
      for(std::vector<AnaP0DReconParticle*>::iterator it2 =(*it)->ParticlesP.begin(); it2!= (*it)->ParticlesP.end(); ++it2, i++ ){
        if (_debug)
          std::cout << " - " << i << ": P0DReconParticles:AlgorithmName : " << (*it2)->AlgorithmName << " Cycle: " << (*it2)->Cycle 
                    << " Bunch: " << (*it2)->Bunch << " p = " << (*it2)->Momentum << " E = " << (*it2)->EDeposit << " l = " << (*it2)->Length 
                    << " nShowers = " << (*it2)->Showers.size() << " nTracks = " << (*it2)->Tracks.size() << std::endl;
        
        if ((*it2)->AlgorithmName.find("Shower")!=std::string::npos){
          p0dBox->nShowers++;
        }
      }
      if (_debug)
        std::cout << "NShowers = " << p0dBox->nShowers << std::endl;
    }
  }

  // Return value is ignored for actions
  return true;
}

//***********************************************************************
bool TwoShowersCut::Apply(AnaEventC& eventC, ToyBoxB& box) const {
//***********************************************************************

  (void) eventC;

  // downcast the box to the appropriate type
  ToyBoxP0D* p0dBox = static_cast<ToyBoxP0D*>(&box);

  // Two showers
  return (p0dBox->nShowers==2);
}

//***********************************************************************
bool NoTracksCut::Apply(AnaEventC& eventC, ToyBoxB& box) const {
//***********************************************************************

  (void) eventC;

  // downcast the box to the appropriate type
  ToyBoxP0D* p0dBox = static_cast<ToyBoxP0D*>(&box);

  // No Tracks
  return (p0dBox->nTracks==0);
}

//***********************************************************************
bool NoMichelElectronsCut::Apply(AnaEventC& eventC, ToyBoxB& box) const {
//***********************************************************************

  (void) box;

  // downcast the AnaEventC to AnaEventB
  AnaEventB& event = *static_cast<AnaEventB*>(&eventC); 


  // No Tracks
  return (static_cast<AnaEvent*>(&event)->nDelayedClusters==0);
}

//***********************************************************************
bool FindTrueVertexAction::Apply(AnaEventC& eventC, ToyBoxB& box) const {
//***********************************************************************

  (void) eventC;

  // downcast the box to the appropriate type
  ToyBoxP0D* p0dBox = static_cast<ToyBoxP0D*>(&box);

  if (_isUsingReconDirP0DNew){
    if (p0dBox->Shower1 && p0dBox->Shower1->GetTrueParticle())
      p0dBox->TrueVertex = p0dBox->Shower1->GetTrueParticle()->TrueVertex;
  }
  /*
  else if (_isUsingReconDirP0D){
    AnaLocalReconEvent* localEvent = static_cast<AnaLocalReconEvent*>(&event);  
    if (localEvent->P0DReconVertices.size()>0)
      box.TrueVertex = localEvent->P0DReconVertices[0]->TrueVertex
      
  }
  */

  return true;
}
