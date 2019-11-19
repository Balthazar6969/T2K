#ifndef antiNumuCCIncFwdAnalysis_h
#define antiNumuCCIncFwdAnalysis_h

#include "numuCCAnalysis.hxx"
#include "ND280AnalysisUtils.hxx"

class antiNumuCCIncFwdAnalysis: public baseTrackerAnalysis {
 public:
  antiNumuCCIncFwdAnalysis(AnalysisAlgorithm* ana=NULL);
  virtual ~antiNumuCCIncFwdAnalysis(){}

  //---- These are mandatory functions
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

  bool Initialize();
  void FillCategories();

protected:
  
  numuCCAnalysis* _numuCCAnalysis;

public:

  enum enumStandardMicroTrees_antiNumuCCIncFwdAnalysis{
    shmp_mom = numuCCAnalysis::enumStandardMicroTreesLast_numuCCAnalysis+1,

    // Which sub-det does the maintrack use?
    selmu_myvars_tpc1_maintrack,
    selmu_myvars_tpc2_maintrack,
    selmu_myvars_tpc3_maintrack,
    selmu_myvars_fgd1_maintrack,
    selmu_myvars_fgd2_maintrack,

    // Time of flight between FGD1 and ECal
	selmu_nECals,

    selmu_myvars_tof_fgd1_fgd2,
    selmu_myvars_tof_p0d_fgd1,
    selmu_myvars_tof_tecal_fgd1,
    selmu_myvars_tof_tecal_fgd2,
    selmu_myvars_tof_dsecal_fgd1,
    selmu_myvars_tof_dsecal_fgd2,
    selmu_myvars_tof_ecal_fgd1_other,

    selmu_myvars_time_fgd1,
    selmu_myvars_time_tecal,
    selmu_myvars_time_dsecal,

    // ECal PID
    selmu_myvars_ecal_mippion,

    // Which ECal is used?
    selmu_myvars_kECAL,
      selmu_myvars_kPECAL,
        selmu_myvars_kTopPECAL,
        selmu_myvars_kBottomPECAL,
        selmu_myvars_kLeftPECAL,
        selmu_myvars_kRightPECAL,
      selmu_myvars_kTECAL,
        selmu_myvars_kTopTECAL,
        selmu_myvars_kBottomTECAL,
        selmu_myvars_kLeftTECAL,
        selmu_myvars_kRightTECAL,
      selmu_myvars_kDSECAL,

    enumStandardMicroTreesLast_antiNumuCCIncFwdAnalysis
  };

  // Here we create an enum with all systematics we want to add starting at the last systematic it was added by used analyses or base analyses, 
  enum enumSyst_antiNumuControlSamples{
    kToFResol = SystId::SystEnumLast_SystId+1,
    kToFDSECalFGD1,
    kToFECalOther,
    kECalMIPPion,
    enumSystLast_antiNumuControlSamples
  };

  // Here we create an enum with all configurations we want to add starting at the last configuration it was added by used analyses or base analyses, 
  enum enumConf_antiNumuControlSamples{
    tof_resol_syst=enumConfLast_baseAnalysis+1,
    tofdsecalfgd1_syst,
    tofecalother_syst,
    ecalmippion_syst,
    enumConfLast_antiNumuControlSamples
  };

};

#endif
