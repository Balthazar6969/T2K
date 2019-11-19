#ifndef numuCCAnalysis_h
#define numuCCAnalysis_h

#include "baseTrackerAnalysis.hxx"
#include "ND280AnalysisUtils.hxx"

class numuCCAnalysis: public baseTrackerAnalysis {
 public:
  numuCCAnalysis(AnalysisAlgorithm* ana=NULL);
  virtual ~numuCCAnalysis(){}

  //---- These are mandatory functions
  void DefineSelections();
  void DefineCorrections();
  void DefineMicroTrees(bool addBase=true);
  void DefineTruthTree();

  void FillMicroTrees(bool addBase=true);
  void FillToyVarsInMicroTrees(bool addBase=true);

  bool CheckFillTruthTree(const AnaTrueVertex& vtx);

  using baseAnalysis::FillTruthTree;
  void FillTruthTree(const AnaTrueVertex& vtx);
  using baseTrackerAnalysis::FillTruthTreeBase;
  //--------------------

  void FillConfigTree();
  bool Initialize();
  void FillCategories();

  void Finalize();
  //---- package-specific functions
  void FillTruthTreeBase(const AnaTrueVertex& vtx, bool IsAntinu = false);
  void SetFV();

private:
  Int_t _whichFGD; // 1 for FGD1 analysis, 2 for FGD2, 3 for both

  bool _saveMassWeights;
  
public:

  enum enumStandardMicroTrees_numuCCAnalysis{
    selmu_truemom = enumStandardMicroTreesLast_baseTrackerAnalysis+1,
    
    truelepton_nuErecQE,
    // This	is not the same	as the category particle since
    // particle	corresponds to the pdg code of the true	particle associated to the candidate muon (it would be selmu_pdg), while
    // truelepton_pdg is the pdg code of the true lepton from the true	vertex associated to the candidate
    truelepton_pdg,
    //    truelepton_det, // eqivalent to detector but in different detector convention

    /// Kinematics of each true point crossing the TPCs§
    truelepton_tpc1_entrance_pos, 
    truelepton_tpc1_exit_pos, 
    
    truelepton_tpc2_entrance_pos, 
    truelepton_tpc2_exit_pos, 
    
    truelepton_tpc3_entrance_pos, 
    truelepton_tpc3_exit_pos, 
  
    // ----
    selmu_nuErecQE,
    selmu_closest_tpc,
    selmu_detectors,
    selmu_truepos,
    selmu_trueendpos,
    selmu_truedir,
    selmu_dir,
    selmu_enddir,
    selmu_pos,
    selmu_endpos,
    selmu_det,
    selmu_ntpcs,
    selmu_tpc_det,
    selmu_tpc_nhits,
    selmu_tpc_nnodes,
    selmu_tpc_charge,
    selmu_tpc_mom,
    selmu_tpc_bfield_mom,
    selmu_tpc_efield_mom,
    selmu_tpc_emom,
    selmu_tpc_truemom,
    selmu_tpc_dedx_raw,
    selmu_tpc_dedx_expmu,
    selmu_tpc_dedx_exppi,
    selmu_tpc_dedx_expele,
    selmu_tpc_dedx_expp,
    selmu_nfgds,
    selmu_fgd_det,
    selmu_fgd_x,
    selmu_fgd_V11,
    selmu_fgd_V33,
    selmu_fgd_V55,
    selmu_fgd_V77,
    selmu_fgd_VLayer,
    selmu_fgd_pullmu,

    selmu_necals,
    selmu_ecal_det,

    selmu_ecal_nhits,
    selmu_ecal_nnodes,
    selmu_ecal_length,
    selmu_ecal_showerstartpos,
    selmu_ecal_showerendpos,
    selmu_ecal_showerstartdir,
    selmu_ecal_showerenddir,

    selmu_ecal_EMenergy,
    selmu_ecal_edeposit,
    selmu_ecal_IsShower,

    selmu_ecal_mipem,
    selmu_ecal_mippion,
    selmu_ecal_emhip,
    selmu_ecal_containment,
    selmu_ecal_trshval,
    selmu_ecal_mostupstreamlayerhit,

    // SMRD vars
    selmu_nsmrds,
    selmu_smrd_det,
    selmu_smrd_edeposit,
    selmu_smrd_nhits,
    selmu_smrd_nnodes,
    selmu_smrd_dir,
    selmu_smrd_enddir,

    selmu_mom,
    selmu_costheta,
    shmn_mom,
    selmu_amom,
    selmu_likemip,
    selmu_likemu,
    selmu_tpc_pullmu,
    selmu_tpc_pullpi,
    selmu_tpc_pullele,
    selmu_tpc_pullp,
    selmu_tpc_dedx,

    // (for xsTool)
    truevtx_mass_component,

    enumStandardMicroTreesLast_numuCCAnalysis
  };

  enum enumConfigTree_numuCCAnalysis{
    nNucleonsFGD1 = enumConfigTreeLast_AnalysisAlgorithm+1,
    nNucleonsFGD2scint,
    nNucleonsFGD2water,
    massWeight,

    enumConfigTreeLast_numuCCAnalysis
  };

  enum enumSpecialTrees_numuCCAnalysis{
    massWeightTree = OutputManager::enumSpecialTreesLast+1,
    enumSpecialTreesLast_numuCCAnalysis
  };

  
};

#endif
