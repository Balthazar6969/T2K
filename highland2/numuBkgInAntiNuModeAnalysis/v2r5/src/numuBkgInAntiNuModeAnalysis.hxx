#ifndef numuBkgInAntiNuModeAnalysis_h
#define numuBkgInAntiNuModeAnalysis_h

#include "numuCCAnalysis.hxx"
#include "numuCCMultiPiAnalysis.hxx"

class numuBkgInAntiNuModeAnalysis: public baseTrackerAnalysis {
 public:
  numuBkgInAntiNuModeAnalysis(AnalysisAlgorithm* ana=NULL);
  virtual ~numuBkgInAntiNuModeAnalysis(){}

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
  numuCCMultiPiAnalysis* _numuCCMultiPiAnalysis;

private:

  bool fCCInclusive;  // = true to run the selection for the CC-inclusive analysis
  bool _doMultiTrack;  // = true to run Multiple Track selection
  bool _doMultiPi;  // = true to run Multiple Pion selection
  Int_t _whichFGD; // 1 for FGD1 analysis, 2 for FGD2, 3 for both

  
};

#endif
