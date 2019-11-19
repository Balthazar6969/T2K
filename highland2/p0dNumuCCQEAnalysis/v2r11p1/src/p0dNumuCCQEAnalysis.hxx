#ifndef p0dNumuCCQEAnalysis_h
#define p0dNumuCCQEAnalysis_h

#include "baseTrackerAnalysis.hxx"
#include "p0dNumuCCAnalysis.hxx"
#include "ND280AnalysisUtils.hxx"

class p0dNumuCCQEAnalysis: public baseTrackerAnalysis {
 public:
  p0dNumuCCQEAnalysis(AnalysisAlgorithm* ana=NULL);
  virtual ~p0dNumuCCQEAnalysis(){delete _p0dNumuCCAnalysis;}

  //---- These are mandatory functions  
  /// overload the function from the base class
  // virtual void InitializeConfiguration();

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

  enum enumSyst_p0dNumuCCQEAnalysis{
    kp0dCCQEOOFV = p0dNumuCCAnalysis::enumSystLast_p0dNumuCCAnalysis+1,
    enumSystLast_p0dNumuCCQEAnalysis
  };

  enum enumConf_p0dNumuCCQEAnalysis{
    p0dccqeoofv_syst = p0dNumuCCAnalysis::enumConfLast_p0dNumuCCAnalysis+1,
    enumConfLast_p0dNumuCCQEAnalysis
  };

protected:

  p0dNumuCCAnalysis* _p0dNumuCCAnalysis;

  bool _saveAllNumuCC;
  
};

#endif
