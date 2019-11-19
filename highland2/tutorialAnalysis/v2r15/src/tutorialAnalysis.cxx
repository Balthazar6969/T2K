#include "tutorialAnalysis.hxx"
#include "FiducialVolumeDefinition.hxx"
#include "Parameters.hxx"
#include "tutorialSelection.hxx"
#include "tutorialBranchesSelection.hxx"
#include "tutorialCorrection.hxx"
#include "tutorialWeightSystematics.hxx"
#include "tutorialVariationSystematics.hxx"
#include "CategoriesUtils.hxx"
#include "BasicUtils.hxx"
#include "baseToyMaker.hxx"
#include "NuDirUtils.hxx"
#include "KinematicsUtils.hxx"


/*  CHANGES IN NEW VERSION

    Updated documentation
*/



/*
  A highland2 Analysis inherits in ultimate instance from AnalysisAlgorithm. 
  In this particular case an intermediate class baseTrackerAnalysis is used, 
  which does all the work that is common for tracker analysis 
  Then tutorialAnalysis inherits from baseTrackerAnalysis, which inherits from baseAnalysis, which inherits from AnalysisAlgorithm.
  
  The class that does the loops (over events, over toys, etc) is AnalysisLoop (under highlandCore). 
  There you can see the analysis flow, which is as follows

  LOOP over Spills{
    InitializeSpill
    LOOP over Bunches in the Spill{
      InitializeBunch
      LOOP over Configurations{
        InitializeConfiguration
        LOOP over Toy experiments for each configuration{
          InitializeToy
          LOOP over Enabled Selections{
            InitializeSelection
            Apply The Selection
            FinalizeSelection
          }
          FinalizeToy
          FillMicroTrees (Fill toy-dependent variables in micro-trees)
        }
        FillToyVarsInMicroTrees (Fill toy-independent variables)
        FillCategories (Fill categories for color code drawing)
        FinalizeConfiguration
      }
      FinalizeBunch
    }
    FillTruthTree (Fill the truth tree)
    FinalizeSpill
  }

  The Initialize.. and Finalize... methods can be implemented by the user to do more complicated analyses, but are not mandatory


  These is the list of mandatory methods to configure the analysis (call at initialization level):
  
  - Initialize:           This is all initializations that cannot be done in the constructor should be put 
                          (the ones that require access to a parameter in the parameters file)
  - DefineSelections:     Add to the SelectionManager the selections  we have defined in other files
  - DefineSystematics:    Add to the SystematicManager the systematics to be run (defined in other files)
  - DefineCorrections:    Add to the CorrectionManager the corrections to be run (defined in other files)
  - DefineConfigurations: Add to the ConfigurationManager the configurations to be considered
  - DefineMicroTrees:     Define the micro-tree variables
  - DefineTruthTree:      Define the variables of the truth tree

  These are the mandatory methods that are run for each Toy Experiment 

  - FillToyVarsInMicroTrees: Fill the micro-trees toy-dependent   variables (run for each toy experiment)

  These are the methods that are run for Event (Bunch)

  - FillMicroTrees:          Fill the micro-trees toy-independent variables. (MANDATORY) 
  - FillCategories:          Fill the micro-tree variables used for color code drawing (OPTIONAL)

  These are the mandatory methods that are run for each Spill

  - FillTruthTree:           Fill the truth tree variables
  - CheckFillTruthTree:      Check whether to include or not  a given true vertex in the truth tree

  These are the methods that are run at initialization level

  - FillConfigTree:           Fill the user defined variables in the single-entry config tree
  
*/


//********************************************************************
tutorialAnalysis::tutorialAnalysis(AnalysisAlgorithm* ana) : baseTrackerAnalysis(ana) {
//********************************************************************
  // Add the package version (to be stored in the "config" tree)
  ND::versioning().AddPackage("tutorialAnalysis", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("TUTORIALANALYSISROOT")));
}

//********************************************************************
bool tutorialAnalysis::Initialize(){
//********************************************************************
  
  /* In this method we Initialize everything that requires access to parameters in the parameters file. 
     This is because in order to the overwride parameters file 
     option (-p param.dat) to work, parameters cannot be accessed in the constructors. 
   */

  // Initialize the base class
  if (!baseTrackerAnalysis::Initialize()) return false;

  // Define categories
  anaUtils::AddStandardCategories();

  // We could add a second set of categories with a given prefix ("fgd2" in this case)
  //  anaUtils::AddStandardCategories("fgd2");
  
  // Minimum accum level to save event into the output Micro-trees
  SetMinAccumCutLevelToSave(ND::params().GetParameterI("tutorialAnalysis.MinAccumLevelToSave"));

  return true;
}

//! [tutorialAnalysis_DefineSelections]
//********************************************************************
void tutorialAnalysis::DefineSelections(){
//********************************************************************
  
  /* In this method the user will add to the SelectionManager (accessed by  sel() ) the selections to be run, 
     defined in other files. In general an analysis has a single selection, which could have multiple branches. 
     Each of the branches is in fact a different selection (different cuts and actions), but defining them as branches 
     we ussualy gain in speed and simplicity since the steps that are common are applied only once. 
     Sometimes the user does not want to expend time merging two selections in a single one (as branches), but preffers to run 
     both independently. This is in general done for quick checks (are the selections mutually exclusive ?, etc). 
     This is possible in highland2. An example on how to do that is shown below. 

     In this case we add two selections to the SelectionManager provided:
     - Name of the selection (kTrackerTutorial, kTrackerTutorialBranches)
     - Title, the one dumped by the DrawingTools::DumpSelections() method. It is an explaination of the selection
     - Pointer to the selection. The argument in the constructor (false) indicates that the 
       step sequence is not broken when a cut is not passed. In this way we can save intermediate information for events 
       not passing the entire selection
   */


  // Add a simple selection with no branches
  sel().AddSelection("kTrackerTutorial",          "tutorial selection",                   new tutorialSelection(false));

  // Add a more complicated selection with branches
  sel().AddSelection("kTrackerTutorialBranches",  "tutorial selection with branches",     new tutorialBranchesSelection(false));

  // Disable the ones not enabled in parameters file
  if (!ND::params().GetParameterI("tutorialAnalysis.Selections.RunSelectionWithoutBranches"))
    sel().DisableSelection("kTrackerTutorial");

  if (!ND::params().GetParameterI("tutorialAnalysis.Selections.RunSelectionWithBranches"))
    sel().DisableSelection("kTrackerTutorialBranches");

}
//! [tutorialAnalysis_DefineSelections]

//! [tutorialAnalysis_DefineCorrections]
//********************************************************************
void tutorialAnalysis::DefineCorrections(){
//********************************************************************

  /* Corrections change the input data to rectify a problem in the MC or in the real data. Imagine for example that the energy deposited by a particle
     in a given sub-detector is overestimated in the MC by about 5%, and that this effect depends on the particle type (6% for muons and 4% for electrons).
     We could introduce a correction for the MC, which would scale the deposited energy by 0.94 for true muons and by 0.96 for true electrons. In this way
     we make sure that any cut using the deposited energy will have the same effect in data and MC, avoiding the corresponding systematic.

     The entire analysis will proceed on the modified data
     Corrections are only applied once per spill.
  */

  // Some corrections are defined in baseTrackerAnalysis (have a look at baseTrackerAnalysis/vXrY/src/baseTrackerAnalysis.cxx)
  baseTrackerAnalysis::DefineCorrections();

  // ----- We can add here extra corrections ----------

  // A dummy correction which moves forward the position of all global tracks by some mm, as specified in 
  // an external data file (data/tutorialCorrection.dat)
  corr().AddCorrection("tutorialCorrection", new tutorialCorrection());
}
//! [tutorialAnalysis_DefineCorrections]

//! [tutorialAnalysis_DefineSystematics]
//********************************************************************
void tutorialAnalysis::DefineSystematics(){
//********************************************************************
  
  /*  Systematics will modify the effective number of events passing the selection and the distribution of the observables. 

      After corrections have been applied it is possible to 
      "manipulate" again the information using Systematics. In this case the purpose is different: we want to test several values of 
      a given detector property and check the impact on the number of selected events. This allows propagating systematic errors numerically. 
      
      There are two kind of systematics:
      - Variations: modify the input data (momentum, PID variables, etc). The selection is run on the modified data such that 
        the result of the selection can be different 
      - Weights: do not modify the input data. The selection is not affected. Simply a weight is added to the event. 
        Since events with different values of a given observable (i.e. momentum ) can have different weights, 
        distributions of that observable may change.
      
      In the above example about the deposited energy (in DefineCorrections(), the correction introduced cannot be perfectly known. 
      The 4% and 6% mentioned have an error (i.e. 0.5%). 
      This error should be propagated as a systematic. A given number of toy experiments will be run with different values of the scaling parameter 
      for the deposited energy (i.e. for muons 0.93, 0.935, 0.95, ..., following
      a gaussian distribution with mean 0.94 and sigma 0.005). If a cut on the deposited energy (or a variable using it)
      is applied the number of selected events could differ depending on the scaling applied.
      The RMS of the number of selected events for all toy experiments represents the systematic error induced by the deposited energy scaling.
   */


  // Some systematics are defined in baseTrackerAnalysis (have a look at baseTrackerAnalysis/vXrY/src/baseTrackerAnalysis.cxx)
  baseTrackerAnalysis::DefineSystematics();

  // ----- We can add here extra systematics ----------

  // An example of variation systematic, which is added to the EventVariationManager
  evar().AddEventVariation(kTutorialVariation, "TutorialVarSyst",   new tutorialVariationSystematics());

  // A example of weight systematic, which is added to the EventWeightManager
  eweight().AddEventWeight(   kTutorialWeight,    "TutorialWeightSyst", new tutorialWeightSystematics());
}
//! [tutorialAnalysis_DefineSystematics]

//! [tutorialAnalysis_DefineConfigurations]
//********************************************************************
void tutorialAnalysis::DefineConfigurations(){
//********************************************************************

  /*  
      The user can run several analyses in parallel minimising the acces to disk. 
      Those parallel analyses are call configurations. Although this might be extended in the future, currenly 
      configurations only allow you to specify which systematics errors will be propagated, the number of toy experiments 
      and the random seed. 
      
      A tree for each configuration is produced in the output file, with the same name as the configuration. 
      By default a single configuration (called "default") is run, producing
      a single tree (with name default). This tree does not have any systematics enabled and hence it represents the nominal selection. 
  */

  // Some configurations are defined in baseTrackerAnalysis (have a look at baseTrackerAnalysis/vXrY/src/baseTrackerAnalysis.cxx)
  baseTrackerAnalysis::DefineConfigurations();

  Int_t ntoys = ND::params().GetParameterI("baseAnalysis.Systematics.NumberOfToys");
  Int_t randomSeed = ND::params().GetParameterI("baseAnalysis.Systematics.RandomSeed");

  // Add a new configuration called "tutorial_syst" with the following properties:
  // - the number of toys defined in the baseAnalysis parameters file
  // - the random seed for toy experiments defined in the baseAnalysis parameters file
  // - The ToyMaker used to CreateTheToyExperiments (random throws for each systematic parameter).
  //   Have a look at baseToyMaker in baseAnalysis/vXrY/src
  // - This configuration has two systematics kTutorialWeight and kTutorialVariation, defined above
  
  AddConfiguration(conf(), tutorial_syst, ntoys, randomSeed, new baseToyMaker(randomSeed));
  conf().EnableSystematic(kTutorialWeight,    tutorial_syst);  // Enable the kTutorialWeight in the tutorial_syst configuration
  conf().EnableSystematic(kTutorialVariation, tutorial_syst);  // Enable the kTutorialVariation in the tutorial_syst configuration

  // Disable the configuration when requested
  if (!ND::params().GetParameterI("tutorialAnalysis.Systematics.EnableTutorialSystematics"))
    conf().DisableConfiguration(tutorial_syst);
}
//! [tutorialAnalysis_DefineConfigurations]

//! [tutorialAnalysis_DefineMicroTrees]
//********************************************************************
void tutorialAnalysis::DefineMicroTrees(bool addBase){
//********************************************************************

  /*  We call Micro-trees to the standard analysis trees appearing in the output file. 
      There is always a Micro-Tree call "default" which should contain the basic info to understand our selection. 
      The user can add extra Micro-Trees by adding configurations to the analysis (see DefineConfigurations method above).

      Here we give an example of different variables that can be added. Have a look at OutputManager.hxx under highlandCore
      to see all available methods.
  */

  // -------- Add variables to the analysis tree ----------------------

  // Variables from baseTrackerAnalysis (run, event, ..., tracker related stuff, ...)
  if (addBase) baseTrackerAnalysis::DefineMicroTrees(addBase);

  // --- Single variables -------
  AddVarD(  output(), selmu_costheta,   "muon candidate reconstructed cos(theta) w.r.t. to neutrino direction");     // Double 
  AddVarF(  output(), selmu_truemom,    "muon candidate true momentum");           // Float
  AddVarI(  output(), selmu_detectors,  "muon candidate detectors");               // Integer
  AddVarC(  output(), selmu_sense,      "muon candidate sense");                   // Char

  AddVarF(  output(), selmu_deltaPhi,   "angle between muon candidate and the proton candidate (HMP track)");             // Float 
  AddVarF(  output(), selmu_pT,         "muon candidate reconstructed transverse momentum w.r.t to neutrino direction");  // Float 

  AddVarI(  output(), nLongTPCTracks,   "number of long TPC tracks");               // Integer

  // --- Vector variables -------
  // selmu_ntpcs is the counter, which is added automatically as integer variable  
  AddVarVI(output(), selmu_tpc_det,         "muon candidate TPC number",                               selmu_ntpcs);
  AddVarVI(output(), selmu_tpc_nnodes,      "muon candidate #nodes in each TPC",                       selmu_ntpcs);  // Vector of integers
  AddVarVF(output(), selmu_tpc_mom,         "muon candidate reconstructed momentum in each TPC",       selmu_ntpcs);  // Vector of floats
  AddVarVD(output(), selmu_tpc_truemom,     "muon candidate true momentum in each TPC",                selmu_ntpcs);  // Vector of doubles

  // --- 3D and 4D vector variables (i.e. directions and positions) -------
  AddVar4VF(output(), selmu_pos,    "muon candidate reconstructed position");     // 4-Vector of floats    
  AddVar3VF(output(), selmu_dir,    "muon candidate reconstructed direction");    // 3-Vector of floats     

  // --- Matrix variables -------
  //  AddVarMF(output(), selmu_tpc, "",selmu_necals,-30,4);

  // --- 3D and 4D matrix variables (i.e. directions and positions for constituents) -------
  AddVar4MF(output(), selmu_tpc_pos,    "muon candidate true position in each tpc",  selmu_ntpcs);         
  AddVar3MF(output(), selmu_tpc_dir,    "muon candidate true direction in each tpc", selmu_ntpcs);         


  // Now we had toy-dependent variables
  // There could be many variables that are toy-dependent. We don't need to save all of them as toy variables, 
  // but only the ones we are interested in plotting for different toys. 
  // TOY VARIABLES ARE VERY SPACE CONSUMING SO WE SHOULD MINIMISE ITS NUMBER !!!!

  // --- single toy variables -------
  AddToyVarF(output(), selmu_mom,      "muon candidate reconstructed momentum");

  // --- vector toy variables -------
  AddToyVarVF(output(), selmu_tpc_dedx,    "muon candidate dEdx (CT) in each TPC", NMAXTPCS);
}
//! [tutorialAnalysis_DefineMicroTrees]





//! [tutorialAnalysis_DefineTruthTree]
//********************************************************************
void tutorialAnalysis::DefineTruthTree(){
//********************************************************************

  /*
//! [tutorialAnalysis_DefineTruthTree_info]
    The main trees in the output file are the "default" and "truth" trees. The "default" tree contains all selected events (the ones that passed a given cut, ussualy 
    specified in the parameters file). This is the tree used to optimise the selection cuts, to make selection plots and to compute or plot the purity of the selection. 
    The main reason for having a separate tree, call "truth", is to allow the efficiency computation. The "default" tree does not contain all true signal events, 
    because it will be too big. The preselection for that tree is only based in reconstructed quantities (selection cuts). Instead, to compute true signal efficiencies 
    we need all true signal events to be present in the output tree. The "truth" tree fullfils this condition. Since, in general, this tree will contain many events, only a 
    reduced set of variables (the ones needed to compute the efficiency) is stored in that tree. 
    
//! [tutorialAnalysis_DefineTruthTree_info]

    SUMMARY: The "truth" tree also appears in the output file. It contains all interactions in which we are interested in regardless on whether 
    the selection was passed or not. This is the tree that should be used to compute signal efficiencies
*/


  // Variables from baseTrackerAnalysis (run, event, ...)
  baseTrackerAnalysis::DefineTruthTree();

  //--- muon variables -------
  AddVarI(  output(), truelepton_pdg,      "true lepton PDG");
  AddVar4VF(output(), truelepton_pos,      "true lepton position");
  // Moved to baseAnalysis !!!!!!!
  //  AddVarF(  output(), truelepton_mom,      "true lepton momentum");
  //  AddVarF(  output(), truelepton_costheta, "true lepton cos(theta) w.r.t. neutrino direction");
  //  AddVar3VF(output(), truelepton_dir,      "true lepton direction"); 
}
//! [tutorialAnalysis_DefineTruthTree]

//! [tutorialAnalysis_FillMicroTrees]
//********************************************************************
void tutorialAnalysis::FillMicroTrees(bool addBase){
//********************************************************************

  /*  In this method we fill all toy-independent variables (all except the ones added with AddToy...) defined in the method DefineMicroTrees. 
      This method is called once all toys has been run, what means that the value of all variables for the last toy will be saved. This is not a problem 
      for variables that are not expected to change from a toy to another.
  */

  // Variables from baseTrackerAnalysis (run, event, ...)
  if (addBase) baseTrackerAnalysis::FillMicroTreesBase(addBase);

  // Add a variable that is only in the derived box AnaBoxTutorial. We need to cast the base box to have access 
  output().FillVar(nLongTPCTracks, static_cast<const ToyBoxTutorial*>(&box())->nLongTPCTracks);      

  // Muon candidate variables. The MainTrack (The muon candidate) should exist in the box
  if (box().MainTrack){    

    // Since Detectors is not in BaseDataClasses (in psycheEventModel) but in DataClasses in (highlandEventModel) we need to cast 
    // the AnaTrackB pointer in box().MainTrack to the derived class AnaTrack.
    output().FillVar(selmu_detectors, static_cast<AnaTrack*>(box().MainTrack)->Detectors);      

    // Compute the cosine of the angle between the muon and the neutrino
    // vertex defined as start of muon track, neutrino direction calculated from this
    TVector3 nuDirVec = anaUtils::GetNuDirRec(box().MainTrack->PositionStart);  // Method in NuDirUtils (in psycheND280Utils)
    TVector3 muDirVec = anaUtils::ArrayToTVector3(box().MainTrack->DirectionStart);
    double costheta_mu_nu = nuDirVec.Dot(muDirVec);

    // Cast it to a double because we have defined this variable as double in DefineMicroTrees 
    // (no need to be a double, it is just to have an example of double variable)
    output().FillVar(selmu_costheta, costheta_mu_nu);
    
    // Transverse component of muon momentum calculated relative to reconstructed neutrino direction
    Float_t  nuDirArr[3]; 
    nuDirVec.GetXYZ(nuDirArr);
    Float_t  pT = anaUtils::GetTransverseMom(nuDirArr,box().MainTrack->DirectionStart,box().MainTrack->Momentum);  // Method in NuDirUtils (in psycheND280Utils)
    output().FillVar(selmu_pT, pT);

    // Coplanarity angle requires a proton... for now just assume use highest momentum +ve track    
    Float_t deltaPhi=0;
    if(box().HMPtrack)
      deltaPhi = anaUtils::GetDPhi(nuDirArr,box().MainTrack->DirectionStart,box().HMPtrack->DirectionStart); // Method in NuDirUtils (in psycheND280Utils)
    output().FillVar(selmu_deltaPhi, deltaPhi);

    // This is unnecessary here (we could use 1 and -1 which are smaller types), but serves as an example
    if (box().MainTrack->DirectionStart[2]>0)
      output().FillVar(selmu_sense,                 "forward");
    else
      output().FillVar(selmu_sense,                 "backward");

    // This is the way we fill 3 and 4-vectors. Must specify the array size when using FillVectorVarFromArray
    output().FillVectorVarFromArray(selmu_pos,    box().MainTrack->PositionStart,  4);
    output().FillVectorVarFromArray(selmu_dir,    box().MainTrack->DirectionStart, 3);

    // Properties of the true particle associated to the muon candidate. 
    // We can also acces the truth with box().MainTrack->TrueObject, but this is of the more basic type AnaTrueObjectC, which does not have Momentum. The GetTrueParticle() method 
    // just do the casting
    if(  box().MainTrack->GetTrueParticle() ) {
      output().FillVar(selmu_truemom,             box().MainTrack->GetTrueParticle()->Momentum);
    }

    // Info in all TPCs 
    for (Int_t subdet = 0; subdet<3; subdet++) {
      if (!SubDetId::GetDetectorUsed(box().MainTrack->Detector, static_cast<SubDetId::SubDetEnum >(subdet+2))) continue;
      AnaTPCParticle* TPCSegment = static_cast<AnaTPCParticle*>(anaUtils::GetSegmentInDet( *box().MainTrack,static_cast<SubDetId::SubDetEnum >(subdet+2)));
      if (!TPCSegment) continue;
      output().FillVectorVar(selmu_tpc_det,     subdet);
      output().FillVectorVar(selmu_tpc_mom,     TPCSegment->Momentum);
      output().FillVectorVar(selmu_tpc_nnodes,  TPCSegment->NNodes);

      // Must specify the array size when using FillMatrixVarFromArray
      output().FillMatrixVarFromArray(selmu_tpc_pos,    TPCSegment->PositionStart,  4);
      output().FillMatrixVarFromArray(selmu_tpc_dir,    TPCSegment->DirectionStart, 3);

      if (TPCSegment->GetTrueParticle()){
        // Cast it to a double because we have defined this variable as double in DefineMicroTrees
        // (no need to be a double, it is just to have an example of double variable)
        output().FillVectorVar(selmu_tpc_truemom, (Double_t)TPCSegment->GetTrueParticle()->Momentum);
      }
      
      // increment the value of the counter for all TPC variables (selmu_ntpcs)
      // The same effect will have output().IncrementCounter(selmu_ntpcs);
      output().IncrementCounterForVar(selmu_tpc_det);
    }
  }
  
}
//! [tutorialAnalysis_FillMicroTrees]

//! [tutorialAnalysis_FillToyVarsInMicroTrees]
//********************************************************************
void tutorialAnalysis::FillToyVarsInMicroTrees(bool addBase){
//********************************************************************

  /*  In this method we fill all toy-dependent variables (the ones added with AddToy...) defined in the method DefineMicroTrees. 
      This method is called at the end of each toy.

      There could be many variables that are toy-dependent. We don't need to save all of them as toy variables, but only the ones we are interested in plotting 
      for different toys. 

      TOY VARIABLES ARE VERY SPACE CONSUMING SO WE SHOULD MINIMISE ITS NUMBER !!!!
  */

  // Fill the common variables
  if (addBase) baseTrackerAnalysis::FillToyVarsInMicroTreesBase(addBase);

  //---- variables specific for this analysis -----
  if (box().MainTrack){
    // Since we are applying a systematic that varies the momentum we need to save the momentum for each toy
    output().FillToyVar(selmu_mom, box().MainTrack->Momentum);    

    // Info in all TPCs 
    for (Int_t subdet = 0; subdet<3; subdet++) {
      if (!SubDetId::GetDetectorUsed(box().MainTrack->Detector, static_cast<SubDetId::SubDetEnum >(subdet+2))) continue;
      AnaTPCParticle* TPCSegment = static_cast<AnaTPCParticle*>(anaUtils::GetSegmentInDet( *box().MainTrack, static_cast<SubDetId::SubDetEnum >(subdet+2)));
      if (!TPCSegment) continue;
      // In principle we need this variable here when PID systematics are run
      output().FillToyVectorVar(selmu_tpc_dedx,   TPCSegment->dEdxMeas, subdet);
    }
  }
}
//! [tutorialAnalysis_FillToyVarsInMicroTrees]

//********************************************************************
bool tutorialAnalysis::CheckFillTruthTree(const AnaTrueVertex& vtx){
//********************************************************************

  /* To avoid unecessary events in the "truth" tree in this method we define the condition to include or not a given 
     true vertex in the tree. In principle we whould only save the true vertices that match our signal definition
  */

  // In this case we only save numu (NuPDG=14) charged current  (0<ReacCode<30) interactions in the FGD1 FV
  bool numuCC=vtx.ReacCode>0 && vtx.ReacCode<30 && vtx.NuPDG==14;// && vtx.LeptonPDG==13;  
  return (anaUtils::InFiducialVolume(SubDetId::kFGD1, vtx.Position, FVDef::FVdefminFGD1,FVDef::FVdefmaxFGD1) && numuCC);
}

//********************************************************************
void tutorialAnalysis::FillTruthTree(const AnaTrueVertex& vtx){
//********************************************************************

  // Fill the common variables defined in baseTrackerAnalysis/vXrY/src/baseTrackerAnalysis.cxx
  baseTrackerAnalysis::FillTruthTreeBase(vtx);
  
  // ---- Fill the extra variables ------
  output().FillVar(truelepton_pdg,vtx.LeptonPDG);
  output().FillVectorVarFromArray(truelepton_pos, vtx.Position, 4);
  // Moved to baseAnalysis !!!!!!!
  //  output().FillVar(truelepton_costheta,(Float_t)cos(anaUtils::ArrayToTVector3(vtx.LeptonDir).Angle(anaUtils::ArrayToTVector3(vtx.NuDir))));
  //  output().FillVar(truelepton_mom,vtx.LeptonMom);
  //  output().FillVectorVarFromArray(truelepton_dir, vtx.LeptonDir,3);
}

//********************************************************************
void tutorialAnalysis::FillCategories(){
//********************************************************************

  /* This method fills the micro-tree variables related with event and particle categories for color drawing. 
     Those variables are added automatically (no need to explicitely add them in DefineMicroTrees) to the 
     micro-trees, but must be filled by the analyser, provided the event and the relevant track (the lepton candidate, 
     that is the HMN track, in this case). 

     This method is not in baseTrackerAnalysis anymore, meaning that if it is not implemented in your analysis, the cattegories will not be filled
  */

  // For the muon candidate
  anaUtils::FillCategories(&GetEvent(), static_cast<AnaTrack*>(box().MainTrack),"", SubDetId::kFGD1);

  // We could add a second set of categories with a given prefix ("fgd2" in this case). See tutorialAnalysis::Initialize() above
  //  anaUtils::FillCategories(&GetEvent(), static_cast<AnaTrack*>(box().MainTrack),"fgd2", SubDetId::kFGD2);

}

//********************************************************************
void tutorialAnalysis::FillConfigTree(){
//********************************************************************

  /* 
     In this method we fill variables in the config tree. This is a single entry tree (event independent) where information about the 
     analysis being run is stored (enabled systematics, corrections and selections, description of the selection, info about the input file, etc). 
     All this is done automatically. However the user can add its own variables. Since this is a single entry tree, the same method can be used 
     to add and fill the variables
  */
  
  
  // Add and fill number of nucleons in each of the targets
  AddVarD(output(), nNucleonsFGD1,      "number of targets in FGD1");
  AddVarD(output(), nNucleonsFGD2scint, "number of targets in FGD2 scintillator");
  AddVarD(output(), nNucleonsFGD2water, "number of targets in FGD2 water");

  output().FillVar(nNucleonsFGD1,      anaUtils::GetNTargets(anaUtils::kFGD1));
  output().FillVar(nNucleonsFGD2scint, anaUtils::GetNTargets(anaUtils::kFGD2xymodules));
  output().FillVar(nNucleonsFGD2water, anaUtils::GetNTargets(anaUtils::kFGD2watermodules));
}

