#ifndef antiNueCCAnalysis_h
#define antiNueCCAnalysis_h

#include "nueCCAnalysis.hxx"
#include "ND280AnalysisUtils.hxx"

class antiNueCCAnalysis: public baseTrackerAnalysis {
public:
  antiNueCCAnalysis(AnalysisAlgorithm* ana=NULL);
  virtual ~antiNueCCAnalysis(){}

  //---- These are mandatory functions
  void DefineSelections();
  void DefineConfigurations(){_nueCCAnalysis->DefineConfigurations();}
  void DefineCorrections(){_nueCCAnalysis->DefineCorrections();}
  void DefineSystematics(){_nueCCAnalysis->DefineSystematics();}
  void DefineMicroTrees(bool addBase=true);
  void DefineTruthTree();

  void FillMicroTrees(bool addBase=true);
  void FillToyVarsInMicroTrees(bool addBase=true);

  bool CheckFillTruthTree(const AnaTrueVertex& vtx);
  using baseTrackerAnalysis::FillTruthTree;
  void FillTruthTree(const AnaTrueVertex& vtx);

  //------------------------------------
  void FillConfigTree();
  void FillCategories();
  bool Initialize();

protected:
  
  nueCCAnalysis* _nueCCAnalysis;

private:

  Int_t _whichFGD; // 1 for FGD1 analysis, 2 for FGD2, 3 for both
  bool _savesecondaries;
  bool _savegammainitiator;

 public:
  /*
  enum enumStandardMicroTrees_antiNueCCAnalysis{

    // = enumStandardMicroTreesLast_nueCCAnalysi+1,
   enumStandardMicroTreesLast_antiNueCCAnalysis
  };
  */
};



#endif
