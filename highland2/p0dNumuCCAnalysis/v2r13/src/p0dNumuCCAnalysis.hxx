#ifndef p0dNumuCCAnalysis_h
#define p0dNumuCCAnalysis_h

#include "baseTrackerAnalysis.hxx"
#include "numuCCAnalysis.hxx"
#include "ND280AnalysisUtils.hxx"
#include "SystId.hxx"

class p0dNumuCCAnalysis: public baseTrackerAnalysis {
 public:
  p0dNumuCCAnalysis(AnalysisAlgorithm* ana=NULL);
  virtual ~p0dNumuCCAnalysis(){}

  //---- These are mandatory functions

  void DefineInputConverters();
  
  void DefineSelections();
  void DefineCorrections();
  void DefineSystematics();
  void DefineMicroTrees(bool addBase=true);
  void DefineTruthTree();
  void DefineConfigurations();

  void FillCategories();

  void FillMicroTrees(bool addBase=true);
  void FillToyVarsInMicroTrees(bool addBase=true);

  bool CheckFillTruthTree(const AnaTrueVertex& vtx);

  using baseTrackerAnalysis::FillTruthTree;
  void FillTruthTree(const AnaTrueVertex& vtx);
  //--------------------

  // Set parameters
  bool Initialize();
  /// Redefine the FV
  void DefineP0DFV();

  enum enumSyst_p0dNumuCCAnalysis{
    kP0dMass = SystId::SystEnumLast_SystId + 1,
    kP0dOOFV,
    enumSystLast_p0dNumuCCAnalysis
  };
 
  enum enumConf_p0dNumuCCAnalysis{
    p0dmass_syst = baseTrackerAnalysis::enumConfLast_baseTrackerAnalysis+1,
    p0doofv_syst,
    enumConfLast_p0dNumuCCAnalysis
  };


  enum enumStandardMicroTrees_p0dNumuCCAnalysis{
    truelepton_pos=numuCCAnalysis::enumStandardMicroTreesLast_numuCCAnalysis + 1,
    truelepton_dir,
    truep_mom,
    truep_dir,
    truelepton_momt,
    truep_momt,
    true_dphi, // true angle between muon and proton in nu-transverse plane
    selmu_momt,
    sel_dphi, // reconstructed transverse angle between 2-track samples
    hmp_mom,
    hmp_pdg,
    hmp_intpc,
    hmp_likemu,
    hmp_likemip,
    hmp_likep,
    hmp_likepi,
    hmp_pos,
    hmp_dir,
    hmp_costheta,
    np0d,
    nmichel,
    nproton,
    nneutron,
    nmeson,
    nphoton,
    true_1mu1p,
    enumStandardMicroTreesLast_p0dNumuCCAnalysis
  };

protected:

  numuCCAnalysis* _numuCCAnalysis;
  bool _saveNumuCCOnPb;
  bool _isUsingReconDirP0D;
  bool _isUsingReconDirP0DNew;
};

#endif
