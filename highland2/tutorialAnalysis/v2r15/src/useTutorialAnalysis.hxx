#ifndef useTutorialAnalysis_h
#define useTutorialAnalysis_h

#include "tutorialAnalysis.hxx"
#include "nueCCAnalysis.hxx"

class useTutorialAnalysis: public baseTrackerAnalysis {
 public:
  useTutorialAnalysis(AnalysisAlgorithm* ana=NULL);
  virtual ~useTutorialAnalysis(){}

  //---- These are mandatory functions
  bool Initialize();

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
  //  void FillTruthTree();
  //--------------------

  void FillCategories();

protected:

  // The analyses to be used
  tutorialAnalysis* _tutorialAnalysis;
  nueCCAnalysis*    _nueCCAnalysis;

public:

  /*
    enum for the additional micro-tree variables added in this analysis. It should start at the last variables added in used analyses + 1.
   */
  enum enumStandardMicroTrees_useTutorialAnalysis{
    selmu_endpos= tutorialAnalysis::enumStandardMicroTreesLast_tutorialAnalysis+1,
    enumStandardMicroTreesLast_useTutorialAnalysis
  };
  
};

#endif
