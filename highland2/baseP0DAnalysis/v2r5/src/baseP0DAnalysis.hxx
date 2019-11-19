#ifndef baseP0DAnalysis_h
#define baseP0DAnalysis_h

#include "baseAnalysis.hxx"
#include "P0DDataClasses.hxx"

/// This class provides a base for user p0d analyses. It handles many common
/// functions, and provides a structure so that users don't have to write too
/// much "boiler-plate" code.
///
/// User analyses should inherit from this class.
class baseP0DAnalysis: public baseAnalysis{
 public:
  /// Constructor, which instantiates the necessary converters for converting
  /// input files to the AnaSpill format.
  baseP0DAnalysis(AnalysisAlgorithm* ana=NULL);
  virtual ~baseP0DAnalysis(){}

  virtual void DefineCorrections();
  virtual void DefineSystematics();
  virtual void DefineConfigurations();
  virtual void DefineMicroTrees(bool addBase=true);
  virtual void DefineTruthTree();

  virtual void FillMicroTrees(bool addBase=true);
  virtual void FillToyVarsInMicroTrees(bool addBase=true);
  virtual void FillTruthTreeBase(const AnaTrueVertex& vtx, const SubDetId::SubDetEnum det=SubDetId::kP0D, bool IsAntinu = false);

  
  /// Get a casted AnaBunchBB to AnaBunch from the InputManager
  AnaP0DBunch& GetBunch(){return *static_cast<AnaP0DBunch*>(&input().GetBunch());}

  /// Get a casted AnaEventC to AnaEvent 
  AnaP0DEvent& GetEvent(){return *static_cast<AnaP0DEvent*>(_event);}


 public:


  enum enumStandardMicroTrees_baseP0DAnalysis{
    np0dtracks = enumStandardMicroTreesLast_baseAnalysis+1,
    np0donlytracks,
    np0dreconvertices,
    np0drecontracks,
    np0dreconclusters,

    enumStandardMicroTreesLast_baseP0DAnalysis
  };

  /*
  enum enumConf_baseP0DAnalysis{
    bfield_syst=baseAnalysis::enumConfLast_baseAnalysis+1,
    enumConfLast_baseP0DAnalysis
  };
  */
};

#endif
