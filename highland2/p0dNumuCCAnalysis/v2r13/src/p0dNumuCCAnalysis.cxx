#include "p0dNumuCCAnalysis.hxx"
#include "p0dNumuCCSelection.hxx"
#include "FiducialVolumeDefinition.hxx"
#include "Parameters.hxx"
#include "UseGlobalAltMomCorrection.hxx"
#include "CategoriesUtils.hxx"
#include "BasicUtils.hxx"
#include "P0dMassSystematics.hxx"
#include "P0dTrackerOOFVSystematics.hxx"
#include "p0dAnaUtils.hxx"
#include "baseToyMaker.hxx"
#include <iostream>
//********************************************************************
p0dNumuCCAnalysis::p0dNumuCCAnalysis(AnalysisAlgorithm* ana) : baseTrackerAnalysis(ana) {
//********************************************************************

  // Add the package version
  ND::versioning().AddPackage("p0dNumuCCAnalysis", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("P0DNUMUCCANALYSISROOT")));

  // Create a numuCCAnalysis passing this analysis to the constructor. In that way the same managers are used
  // N.B. this resets the _flux to NULL in _numuCCAnalysis so if that is used to fill, we must call
  // _numuCCAnalysis->DefineConfigurations() which sets up the _flux weighting.
  _numuCCAnalysis = new numuCCAnalysis(this);

  // Use the numuCCAnalysis (in practice that means that the same box and event will be used for the numuCCAnalysis as for this analysis)
  UseAnalysis(_numuCCAnalysis);
}

//********************************************************************
void p0dNumuCCAnalysis::DefineInputConverters(){
//********************************************************************

  baseTrackerAnalysis::DefineInputConverters();
  
}

//********************************************************************
bool p0dNumuCCAnalysis::Initialize(){
//********************************************************************

  // Initialize the numuCCAnalysis
  if (!_numuCCAnalysis->Initialize()) return false;

  if (ND::params().GetParameterI("p0dNumuCCAnalysis.Systematics.UseP0DCentralECal") )
  {
    std::cout <<"Using P0D Central ECal rather than water target"<<std::endl;
    // Normal P0D limits: -3296.48 --> -938.753
    // -1233 < z < -970
    FVDef::FVdefminP0D[2] = 2063.48;//2032.48 is the same as the usual max z
    FVDef::FVdefmaxP0D[2] = 31.247;
  }

  // for whether or not P0D Recon directory variables are read
  _isUsingReconDirP0D = ND::params().GetParameterI("highlandIO.FlatTree.UseReconDirP0D");

  // for whether or not P0D Recon directory variables are read and new P0DDataClasses are used
  _isUsingReconDirP0DNew = ND::params().GetParameterI("highlandIO.P0DDataClasses.UseReconDirP0DNew");

  // Minimum accum level to save event into the output tree
  SetMinAccumCutLevelToSave(ND::params().GetParameterI("p0dNumuCCAnalysis.MinAccumLevelToSave"));

  // save all true numuCC on lead in truth tree
  _saveNumuCCOnPb = ND::params().GetParameterI("p0dNumuCCAnalysis.TruthTree.SaveNumuCCOnPb");

  return true;
}

//********************************************************************
void p0dNumuCCAnalysis::DefineSelections(){
//********************************************************************

  // ----- Inclusive CC -----------
  sel().AddSelection("kP0DNuMuCC",           "inclusive p0dNumuCC selection",     new p0dNumuCCSelection(false));
}

//********************************************************************
void p0dNumuCCAnalysis::DefineCorrections(){
//********************************************************************
  // Need to call this again to enable flux weights for analyses that use this one
  baseTrackerAnalysis::DefineCorrections();

  _numuCCAnalysis->DefineCorrections();
}

//********************************************************************
void p0dNumuCCAnalysis::DefineSystematics(){
//********************************************************************
  baseTrackerAnalysis::DefineSystematics();
  eweight().AddEventWeight(kP0dMass,              "P0DMass",             new P0dMassSystematics());
  eweight().AddEventWeight(kP0dOOFV,              "P0DOOFV",             new P0dTrackerOOFVSystematics());

}

//********************************************************************
void p0dNumuCCAnalysis::DefineConfigurations(){
//********************************************************************
  // We do not define any baseAnalysis systematics so we should not
  // enable their configurations
  baseTrackerAnalysis::DefineConfigurations();

  Int_t ntoys = ND::params().GetParameterI("baseAnalysis.Systematics.NumberOfToys");
  Int_t randomSeed = ND::params().GetParameterI("baseAnalysis.Systematics.RandomSeed");

  //bool enableSingleVariationSystConf = (bool)ND::params().GetParameterI("baseAnalysis.Configurations.EnableSingleVariationSystConfigurations");
  bool enableSingleWeightSystConf    = (bool)ND::params().GetParameterI("baseAnalysis.Configurations.EnableSingleWeightSystConfigurations");
  // std::cout <<"Enable single syst conf: "<<enableSingleSystConf<<std::endl;
  // std::cout <<"Enable P0D Mom scale: "<<ND::params().GetParameterI("p0dNumuCCAnalysis.Variations.EnableP0DMomentumScale")<<std::endl;
  // std::cout <<"Number of toys: "<<ntoys<<std::endl;
  if (enableSingleWeightSystConf){
    if (ND::params().GetParameterI("p0dNumuCCAnalysis.Weights.EnableP0DMass")){
      AddConfiguration(conf(), p0dmass_syst,ntoys,randomSeed,new baseToyMaker(randomSeed));
      conf().EnableEventWeight(kP0dMass,p0dmass_syst);
    }
    if (ND::params().GetParameterI("p0dNumuCCAnalysis.Weights.EnableP0DOOFV")){
      AddConfiguration(conf(), p0doofv_syst,ntoys,randomSeed,new baseToyMaker(randomSeed));
      conf().EnableEventWeight(kP0dOOFV,p0doofv_syst);
    }
  }


  // Enable all Event Weights in the default and all_syst configurations
  for (std::vector<ConfigurationBase* >::iterator it= conf().GetConfigurations().begin();it!=conf().GetConfigurations().end();it++){
    Int_t index = (*it)->GetIndex();
    if (index != ConfigurationManager::default_conf && (index != all_syst || !_enableAllSystConfig)) continue;

    if (ND::params().GetParameterI("p0dNumuCCAnalysis.Weights.EnableP0DMass"))  
      conf().EnableEventWeight(kP0dMass, index);
    if (ND::params().GetParameterI("p0dNumuCCAnalysis.Weights.EnableP0DOOFV"))
      conf().EnableEventWeight(kP0dOOFV,index);
  }
}

//********************************************************************
void p0dNumuCCAnalysis::DefineMicroTrees(bool addBase){
//********************************************************************
  _numuCCAnalysis->DefineMicroTrees(addBase);
  AddVar4VF(output(), truelepton_pos,  "true lepton position");
  AddVar3VF(output(), truelepton_dir,  "true lepton direction");
  AddVarF(output(),   truep_mom,   "true proton momentum");
  AddVar3VF(output(), truep_dir,   "true proton direction");
  AddVarF(output(),   truelepton_momt, "true lepton momentum in transverse plane");
  AddVarF(output(),   truep_momt,  "true proton momentum in transverse plane");
  AddVarF(output(),   true_dphi,   "true angle between muon and proton in transverse plane");
  AddVarF(output(),   selmu_momt,  "selected muon reconstructed momentum in transverse plane");
  AddVarF(output(),   sel_dphi,    "reconstructed transverse angle between 2-track samples");
  AddVarF(output(),   hmp_mom,     "highest momentum positive track momentum");
  AddVarI(output(),   hmp_pdg,     "highest momentum positive track pdg code");
  AddVarI(output(),   hmp_intpc,   "does HMP track go into TPC");
  AddVarF(output(),   hmp_likemu,  "HMP muon likelihood");
  AddVarF(output(),   hmp_likemip, "HMP MIP likelihood");
  AddVarF(output(),   hmp_likep,   "HMP proton likelihood");
  AddVarF(output(),   hmp_likepi,  "HMP pion likelihood");
  AddVar4VF(output(), hmp_pos,     "HMP track position");
  AddVar3VF(output(), hmp_dir,     "HMP track position");
  AddVarF(output(),   hmp_costheta, "HMP cos(theta)");
  AddVarI(output(),   np0d,        "number of p0d reconstructed tracks in bunch");
  AddVarI(output(),   nmichel,     "number of p0drecon clusters with AlgorithmName==TP0DTagMuonDecay after HMN track");
  AddVarI(output(),   nproton,     "number of primary protons from RooTracker vtx");
  AddVarI(output(),   nneutron,    "number of primary neutrons from RooTracker vtx");
  AddVarI(output(),   nmeson,      "number of primary mesons from RooTracker vtx");
  AddVarI(output(),   nphoton,     "number of primary photons from RooTracker vtx");
  AddVarI(output(),   true_1mu1p,  "flag for precisely 1 muon 1 proton in FS.");
}
//

//********************************************************************
void p0dNumuCCAnalysis::DefineTruthTree(){
  //********************************************************************

  // Variables from baseTrackerAnalysis (run, event, ...)
  _numuCCAnalysis->DefineTruthTree();

  // p0dNumuCC variables
  AddVarF(output(),   truep_mom,   "true proton momentum");
  AddVar3VF(output(), truep_dir,   "true proton direction");
  AddVarF(output(),   truelepton_momt, "true lepton momentum in transverse plane");
  AddVarF(output(),   truep_momt,  "true proton momentum in transverse plane");
  AddVarF(output(),   true_dphi,   "true angle between muon and proton in transverse plane");
  AddVarI(output(),   nproton,     "number of primary protons from RooTracker vtx");
  AddVarI(output(),   nneutron,    "number of primary neutrons from RooTracker vtx");
  AddVarI(output(),   nmeson,      "number of primary mesons from RooTracker vtx");
  AddVarI(output(),   nphoton,     "number of primary photons from RooTracker vtx");
  AddVarI(output(),   true_1mu1p,  "flag for precisely 1 muon 1 proton in FS.");
}

//********************************************************************
void p0dNumuCCAnalysis::FillMicroTrees(bool addBase){
  //********************************************************************

  // Fill variables from numuCCAnalysis
  _numuCCAnalysis->FillMicroTrees(addBase);

  // Get some more truelepton info
  if (box().MainTrack){
    if(box().MainTrack->GetTrueParticle()) {
      AnaTrueVertex *vtx = static_cast<AnaTrueVertex*>(box().MainTrack->GetTrueParticle()->TrueVertex);  
      if (vtx) {
        output().FillVectorVarFromArray(truelepton_dir, vtx->LeptonDir,3);
        output().FillVectorVarFromArray(truelepton_pos, vtx->Position, 4);
        output().FillVar(truep_mom,vtx->ProtonMom);
        output().FillVectorVarFromArray(truep_dir, vtx->ProtonDir,3);
        if (vtx->LeptonMom > 0) {
          output().FillVar(truelepton_momt,p0dAnaUtils::GetTransverseMom(vtx->NuDir,
                                                                         vtx->LeptonDir,
                                                                         vtx->LeptonMom));
        }
        if (vtx->ProtonMom > 0) {
          output().FillVar(truep_momt,p0dAnaUtils::GetTransverseMom(vtx->NuDir,
                                                                    vtx->ProtonDir,
                                                                    vtx->ProtonMom));
        }
        if (vtx->LeptonMom > 0 && vtx->ProtonMom > 0) {
          output().FillVar(true_dphi, p0dAnaUtils::GetDPhi(vtx->NuDir,
                                                           vtx->LeptonDir,
                                                           vtx->ProtonDir));
        }
        output().FillVar(nproton, vtx->NPrimaryParticles[ParticleId::kProton]);
        output().FillVar(nneutron, vtx->NPrimaryParticles[ParticleId::kNeutron]);
        output().FillVar(nmeson, vtx->NPrimaryParticles[ParticleId::kMesons]);
        output().FillVar(nphoton, vtx->NPrimaryParticles[ParticleId::kPhoton]);
        output().FillVar(true_1mu1p, p0dAnaUtils::True1Mu1P(*vtx));
      }
    }

    if (box().HMPtrack){
      output().FillVar(hmp_mom, box().HMPtrack->Momentum);
      output().FillVar(hmp_intpc, SubDetId::GetDetectorUsed(box().HMPtrack->Detector,SubDetId::kTPC1));
      output().FillVar(hmp_likemu,anaUtils::GetPIDLikelihood( *(box().HMPtrack),0));
      output().FillVar(hmp_likemip,anaUtils::GetPIDLikelihoodMIP( *(box().HMPtrack)));
      output().FillVar(hmp_likepi,anaUtils::GetPIDLikelihood( *(box().HMPtrack),3));
      output().FillVar(hmp_likep,anaUtils::GetPIDLikelihood( *(box().HMPtrack),2));
      output().FillVectorVarFromArray(hmp_pos, box().HMPtrack->PositionStart ,4);
      output().FillVectorVarFromArray(hmp_dir, box().HMPtrack->DirectionStart,3);
      output().FillVar(hmp_costheta, box().HMPtrack->DirectionStart[2]);
      if (box().HMPtrack->GetTrueParticle()){
        output().FillVar(hmp_pdg,box().HMPtrack->GetTrueParticle()->PDG);
      }
    }

    // Count the number of P0D showers and P0D tracks
    AnaTrackB* p0dTracks[NMAXPARTICLES];
    int np0dtracks = anaUtils::GetAllTracksUsingP0D(GetEvent(),p0dTracks);
    output().FillVar(np0d, np0dtracks);

    // Count the number of P0D michel candidates
    if (_isUsingReconDirP0D) {
      // @todo: make into utils function
      int nmichels = 0;
      std::vector<int> clustersUsed;
      // Check clusters in bunches equal to or after bunch of HMNtrack
      for (std::vector<AnaBunchC*>::iterator it = GetSpill().Bunches.begin();
           it != GetSpill().Bunches.end(); ++it) {
        AnaLocalReconBunch* p0dRecoBunch = dynamic_cast<AnaLocalReconBunch*> (*it);
        if (!p0dRecoBunch) continue;
        // Skip bunches before HMNtrack
        if (p0dRecoBunch->Bunch < ((AnaTrack*) box().MainTrack)->Bunch) continue;

        for (std::vector<AnaP0DReconCluster*>::iterator it = p0dRecoBunch->P0DReconClusters.begin();
             it != p0dRecoBunch->P0DReconClusters.end(); ++it) {
          if ((*it)->AlgorithmName.compare("TP0DTagMuonDecay") == 0 && find(clustersUsed.begin(),clustersUsed.end(),(*it)->UniqueID) == clustersUsed.end() ) {
            nmichels += 1; 
            clustersUsed.push_back((*it)->UniqueID);
          }
        }
      }

      // Check clusters in out of bunch with time > HMNtrack->time
      AnaLocalReconBunch* p0dRecoOutOfBunch = dynamic_cast<AnaLocalReconBunch*> (GetSpill().OutOfBunch);
      if (p0dRecoOutOfBunch) {
        for (std::vector<AnaP0DReconCluster*>::iterator it = p0dRecoOutOfBunch->P0DReconClusters.begin();
             it != p0dRecoOutOfBunch->P0DReconClusters.end(); ++it) {
          if ((*it)->Position[3] > box().MainTrack->PositionStart[3] &&
              (*it)->AlgorithmName.compare("TP0DTagMuonDecay") == 0 && find(clustersUsed.begin(),clustersUsed.end(),(*it)->UniqueID) == clustersUsed.end() ) {
                nmichels += 1;
                clustersUsed.push_back( (*it)->UniqueID );
          }
        }
      }
      output().FillVar(nmichel, nmichels);
    }
    else if (_isUsingReconDirP0DNew) {
      // Count the number of P0D michel candidates. This is precomputed inside oaAnalysisTreeConverter using the method p0dUtils::GetMichelElectrons  
      Int_t nmichels = static_cast<AnaEvent*>(_event)->nDelayedClusters;  
      output().FillVar(nmichel, nmichels);
    }

    // Calculate sel_dphi
    if (box().Vertex) {
      Float_t nudir[3];
      anaUtils::VectorToArray(p0dAnaUtils::GetNuDirRec(box().Vertex->Position), nudir);

      output().FillVar(selmu_momt,p0dAnaUtils::GetTransverseMom(nudir,
                                                                box().MainTrack->DirectionStart,
                                                                box().MainTrack->Momentum));

      // 2 p0d tracks
      if (np0dtracks == 2) {
        output().FillVar(sel_dphi, p0dAnaUtils::GetDPhi(nudir,
                                                        p0dTracks[0]->DirectionStart,
                                                        p0dTracks[1]->DirectionStart));
      }
    }
  }
}

//********************************************************************
void p0dNumuCCAnalysis::FillToyVarsInMicroTrees(bool addBase){
  //********************************************************************

  // Variables from the numuCCAnalysis analysis (including the ones in baseTrackerAnalysis by default, otherwise addBase should be false
  _numuCCAnalysis->FillToyVarsInMicroTrees(addBase);
}

//********************************************************************
bool p0dNumuCCAnalysis::CheckFillTruthTree(const AnaTrueVertex& vtx){
//********************************************************************

  bool numuCC=vtx.ReacCode>0 && vtx.ReacCode<30 && vtx.NuPDG==14;// && vtx.LeptonPDG==13;  
  bool inFV = anaUtils::InFiducialVolume(SubDetId::kP0D, vtx.Position);
  bool onPb = anaUtils::GetTargetCode(&vtx) == 82;
  if (_saveNumuCCOnPb) return ((inFV && numuCC) || (onPb && numuCC));
  return (inFV && numuCC);
}

//********************************************************************
void p0dNumuCCAnalysis::FillTruthTree(const AnaTrueVertex& vtx){
//********************************************************************
  // Fill the common variables
  // cannot simply call numuCCAnalysis->FillTruthTree because we need to specify
  // the detector which is different
  baseTrackerAnalysis::FillTruthTreeBase(vtx, SubDetId::kP0D);
  
  // p0dNumuCC variables
  output().FillVar(truep_mom,vtx.ProtonMom);
  output().FillVectorVarFromArray(truep_dir, vtx.ProtonDir,3);
  if (vtx.LeptonMom > 0) {
    output().FillVar(truelepton_momt,p0dAnaUtils::GetTransverseMom(vtx.NuDir,
                                                                   vtx.LeptonDir,
                                                                   vtx.LeptonMom));
  }
  if (vtx.ProtonMom > 0) {
    output().FillVar(truep_momt,p0dAnaUtils::GetTransverseMom(vtx.NuDir,
                                                              vtx.ProtonDir,
                                                              vtx.ProtonMom));
  }
  if (vtx.LeptonMom > 0 && vtx.ProtonMom > 0) {
    output().FillVar(true_dphi, p0dAnaUtils::GetDPhi(vtx.NuDir,
                                                     vtx.LeptonDir,
                                                     vtx.ProtonDir));
  }
  output().FillVar(nproton, vtx.NPrimaryParticles[ParticleId::kProton]);
  output().FillVar(nneutron, vtx.NPrimaryParticles[ParticleId::kNeutron]);
  output().FillVar(nmeson, vtx.NPrimaryParticles[ParticleId::kMesons]);
  output().FillVar(nphoton, vtx.NPrimaryParticles[ParticleId::kPhoton]);
  output().FillVar(true_1mu1p, p0dAnaUtils::True1Mu1P(vtx));
}

//********************************************************************
void p0dNumuCCAnalysis::FillCategories(){
//********************************************************************

  // Fill the track categories for color drawing

  // For the muon candidate
  anaUtils::FillCategories(&GetEvent(), static_cast<AnaTrack*>(box().MainTrack),"",SubDetId::kP0D);
}
