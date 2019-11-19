#ifndef numuCCMultiTargetAnalysis_h
#define numuCCMultiTargetAnalysis_h

#include "ND280AnalysisUtils.hxx"
#include "numuCCMultiTargetSelection.hxx" // needed here for the box
#include "antiNumuCCFGD2Selection.hxx"
#include "numuCCAnalysis.hxx"
#include "numuCCMultiPiAnalysis.hxx"
#include "antiNumuCCAnalysis.hxx"


class numuCCMultiTargetAnalysis: public baseTrackerAnalysis {
public:

  numuCCMultiTargetAnalysis(AnalysisAlgorithm* ana=NULL);
  virtual ~numuCCMultiTargetAnalysis(){}

  //---- These are mandatory functions

  void DefineSelections();
  void DefineCorrections();
  void DefineMicroTrees(bool addBase=true);
  void DefineTruthTree();

  void FillMicroTrees(bool addBase=true);
  void FillToyVarsInMicroTrees(bool addBase=true);

  bool CheckFillTruthTree(const AnaTrueVertex& vtx);

  using baseTrackerAnalysis::FillTruthTree;
  void FillTruthTree(const AnaTrueVertex& vtx);


  //--------------------

  void FillCategories();
  bool Initialize();

protected:

  numuCCAnalysis* _numuCCAnalysis;
  numuCCMultiPiAnalysis* _numuCCMultiPiAnalysis;
  antiNumuCCAnalysis* _antiNumuCCAnalysis;

public:

  enum enumStandardMicroTrees_numuCCMultiTargetAnalysis{
    enumStandardMicroTreesFirst_numuCCMultiTargetAnalysis = numuCCMultiPiAnalysis::enumStandardMicroTreesLast_numuCCMultiPiAnalysis + antiNumuCCAnalysis::enumStandardMicroTreesLast_antiNumuCCAnalysis + 1,
    ntracks,
    selmu_fgdmoduletype,
    selmu_fgdlayer,
    selmu_hit1_pos,
    selmu_hit2_pos,
    selmu_hit1_charge,
    selmu_hit2_charge,
    distance_track_hit1,
    distance_track_hit2,
    selmu_fittrack_hit1_pos,
    selmu_fittrack_hit2_pos,
    selmu_deltachi2_hit1,
    selmu_deltachi2_hit2,
    selmu_hit1_fgdmoduletype,
    truelepton_fgdmoduletype,
    truelepton_fgdlayer,
    truelepton_targetZ,
    truelepton_targetPDG,
    selmu_nuErecQEoxygen,
    truelepton_nuErecQEoxygen,
    selmu_chi2,
    // truth tree
    truevtx_fgdmoduletype,
    truevtx_fgdlayer,
    truevtx_targetZ,
    truevtx_targetPDG,
    enumStandardMicroTreesLast_numuCCMultiTargetAnalysis
  };
};

#endif
