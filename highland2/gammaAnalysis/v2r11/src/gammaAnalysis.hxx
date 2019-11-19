#ifndef gammaAnalysis_h
#define gammaAnalysis_h

#include "nueCCAnalysis.hxx"
#include "ND280AnalysisUtils.hxx"

class gammaAnalysis: public baseTrackerAnalysis {

public:
  gammaAnalysis(AnalysisAlgorithm* ana=NULL);
  virtual ~gammaAnalysis(){}

  // --------------------------------
  // These are mandatory functions
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

  // --------------------------------
  void FillConfigTree();
  void FillCategories();
  bool Initialize();

protected:
  nueCCAnalysis* _nueCCAnalysis;

private:

  Int_t _whichFGD; // 1 -> FGD1; 2 -> FGD2; whatever else -> FGDs
  bool _savesecondaries;
  bool _savegammainitiator;

public:
  /*
  enum enumStandardMicroTrees_gammaAnalysis{
    // Information of the primary track
    PairTrack_mom = nueCCAnalysis::enumStandardMicroTreesLast_nueCCAnalysis+1,
    
    enumStandardMicroTreesLast_gammaAnalysis
  };
  */
};

#endif
