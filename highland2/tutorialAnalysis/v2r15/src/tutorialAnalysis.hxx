#ifndef tutorialAnalysis_h
#define tutorialAnalysis_h

#include "baseTrackerAnalysis.hxx"
#include "ND280AnalysisUtils.hxx"
#include "nueCCAnalysis.hxx"

/// Inherit from baseTrackerAnalysis, which inherit from basAnslysis, which inherits from AnalysisAlgorithm (in highlandCore)
class tutorialAnalysis: public baseTrackerAnalysis {
 public:
  tutorialAnalysis(AnalysisAlgorithm* ana=NULL);
  virtual ~tutorialAnalysis(){}

  //---- These are mandatory functions (pure virtual, =0, in AnalysisAlgorithm) ---

  // However most of them were already implemented in baseAnalysis and baseTracker analysis
  // Some of them are extend it here and some others are not   

  bool Initialize();

  //  void DefineProductions();     // No need to extend it
  //  void DefineInputConverters(); // no need to extend it

  /// Add to the selection manager the selections for this analysis
  void DefineSelections();
  void DefineCorrections();
  void DefineSystematics();
  void DefineConfigurations();
  void DefineMicroTrees(bool addBase=true);
  void DefineTruthTree();

  void FillMicroTrees(bool addBase=true);
  void FillToyVarsInMicroTrees(bool addBase=true);

  bool CheckFillTruthTree(const AnaTrueVertex& vtx);

  using baseTrackerAnalysis::FillTruthTree;
  void FillTruthTree(const AnaTrueVertex& vtx);

  //--------------------

  void FillConfigTree();
  void FillCategories();

public:

  /* Here we create an enum with all variables we want to add to the trees, starting at the last variable it was added by used analyses or base analyses, 
     in this case the last of the nueCCAnalysis
  */

  enum enumStandardMicroTrees_tutorialAnalysis{
    nLongTPCTracks= nueCCAnalysis::enumStandardMicroTreesLast_nueCCAnalysis+1,
    selmu_truemom, 
    selmu_detectors,
    selmu_sense,
    selmu_costheta,
    selmu_deltaPhi,
    selmu_pT,
    selmu_dir,
    selmu_pos,
    selmu_ntpcs,
    selmu_tpc_det,
    selmu_tpc_nnodes,
    selmu_tpc_mom,
    selmu_tpc_pos,
    selmu_tpc_dir,
    selmu_tpc_truemom,

    selmu_mom,
    selmu_tpc_dedx,

    truelepton_pdg,
    truelepton_mom,
    truelepton_pos,
    truelepton_dir,
    truelepton_costheta,
    enumStandardMicroTreesLast_tutorialAnalysis
  };

  // Here we create an enum with all systematics we want to add starting at the last systematic it was added by used analyses or base analyses, 
  enum enumSyst_tutorialAnalysis{
    kTutorialWeight = SystId::SystEnumLast_SystId+1,
    kTutorialVariation,
    enumSystLast_tutorialAnalysis
  };

  // Here we create an enum with all configurations we want to add starting at the last configuration it was added by used analyses or base analyses, 
  enum enumConf_tutorialAnalysis{
    tutorial_syst=enumConfLast_baseTrackerAnalysis+1,
    enumConfLast_btutorialAnalysis
  };

  // Here we create an enum with all variables we want to add to the config tree starting with the last it wad added by used analyses or base analyses, 
  enum enumConfigTree_tutorialAnalysis{
    nNucleonsFGD1 = enumConfigTreeLast_AnalysisAlgorithm+1,
    nNucleonsFGD2scint,
    nNucleonsFGD2water,
    enumConfigTreeLast_tutorialAnalysis
  };
  
};

#endif
