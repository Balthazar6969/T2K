#ifndef antiNumuCCAnalysis_h
#define antiNumuCCAnalysis_h

#include "numuCCAnalysis.hxx"
#include "ND280AnalysisUtils.hxx"

class antiNumuCCAnalysis: public baseTrackerAnalysis {
 public:
  antiNumuCCAnalysis(AnalysisAlgorithm* ana=NULL);
  virtual ~antiNumuCCAnalysis(){}

  //---- These are mandatory functions
  void DefineSelections();
  void DefineCorrections(){_numuCCAnalysis->DefineCorrections();}
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

  enum enumStandardMicroTrees_antiNumuCCAnalysis{
    shmp_mom = numuCCAnalysis::enumStandardMicroTreesLast_numuCCAnalysis+1,
    enumStandardMicroTreesLast_antiNumuCCAnalysis
  };

private:

  bool fCCInclusive;  // = true to run the selection for the CC-inclusive analysis
  Int_t _whichFGD; // 1 for FGD1 analysis, 2 for FGD2, 3 for both
  
};

#endif
