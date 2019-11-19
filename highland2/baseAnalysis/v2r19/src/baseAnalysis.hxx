#ifndef baseAnalysis_h
#define baseAnalysis_h

#include "HighlandAnalysisUtils.hxx"
#include "FluxWeighting.hxx"
#include "AnalysisAlgorithm.hxx"

#include "Parameters.hxx"
#include "CutUtils.hxx"
#include "VersioningUtils.hxx"
#include "SystId.hxx"
#include "Versioning.hxx"
#include "CategoriesUtils.hxx"
#include "CategoryManager.hxx"
#include "ToyBoxTracker.hxx"
#include "RooTrackerVtxManager.hxx"


/// This class provides a base for user analyses. It handles many common
/// functions, and provides a structure so that users don't have to write too
/// much "boiler-plate" code.
///
/// User analyses should inherit from this class.
class baseAnalysis: public AnalysisAlgorithm{
 public:
  /// Constructor, which instantiates the necessary converters for converting
  /// input files to the AnaSpill format.
  baseAnalysis(AnalysisAlgorithm* ana=NULL);
  virtual ~baseAnalysis(){}

  /// This method sould not be virtual
  bool InitializeBase();
  
  virtual bool Initialize();
  virtual bool InitializeSpill();
  virtual bool FinalizeConfiguration();

  virtual void DefineProductions();
  virtual void DefineInputConverters();

  virtual void DefineCorrections();
  virtual void DefineSystematics();
  virtual void DefineConfigurations();
  virtual void DefineMicroTrees(bool addBase=true);
  virtual void DefineTruthTree();

  virtual bool CheckFillTruthTree(const AnaTrueVertex& vtx) = 0;
  virtual void FillTruthTree(const AnaTrueVertex& vtx) = 0;

  void FillMicroTreesBase(bool addBase=true);
  void FillToyVarsInMicroTreesBase(bool addBase=true);
  void FillTruthTree();
  void FillTruthTreeBase(const AnaTrueVertex& vtx, const SubDetId::SubDetEnum det=SubDetId::kFGD1, bool IsAntinu = false);

  /// Get a casted AnaSpillC to AnaSpill from the InputManager
  AnaSpill& GetSpill(){return *static_cast<AnaSpill*>(&input().GetSpill());}
  
  /// Get a casted AnaBunchBB to AnaBunch from the InputManager  (TODO the protection)
  AnaBunch& GetBunch(){
    if (_analysisPoint < AnalysisAlgorithm::kInitializeSpill || _analysisPoint> kFinalizeBunch){
      std::cout << "baseAnalysis::GetBunch(). The Bunch is not available before InitializeBunch or after FinalizeBunch (see AnalysisAlgorithm.hxx) !!!" << std::endl;
      exit(1); 
    }
    return *static_cast<AnaBunch*>(&input().GetBunch());
  }

  /// Get a casted AnaEventC to AnaEvent 
  AnaEvent& GetEvent(){
    if (_analysisPoint < AnalysisAlgorithm::kInitializeBunch || _analysisPoint> kFinalizeBunch){
      std::cout << "baseAnalysis::GetEvent(). The Event is not available before InitializeBunch or after FinalizeBunch (see AnalysisAlgorithm.hxx) !!!" << std::endl;
      exit(1); 
    }
    return *static_cast<AnaEvent*>(_event);
  }

  /// Create the appropriate event time from an Spill and a Bunch in that spill
  virtual AnaEventC* MakeEvent(){
    return new AnaEvent(GetSpill(),GetBunch());
  }

  // Return the vertex from the ToyBox (some ToyBox derived classes could not have that member)
  virtual AnaVertexB* GetVertex() const = 0;

  // Return the true vertex from the ToyBox (some ToyBox derived classes could not have that member)
  virtual AnaTrueVertexB* GetTrueVertex() const = 0;

  /// Set whether to save the RooTrackerVtx tree or not
  void SetSaveRooTracker(bool save);

  /// Set the RooTrackerVtxManager
  void SetRooVtxManager(RooTrackerVtxManager* man);
  
 protected:

  /// the manager of the RooTrackerVtx
  RooTrackerVtxManager* _rooVtxManager;

  /// Save RooTracker tree in output file
  bool _saveRooTrackerVtxTree;
  
  /// Access to the flux weighting.
  FluxWeighting* _flux;

  /// Flag to enable/disable flux weight
  bool _applyFluxWeight;

  /// Flux file and option
  std::string _fluxFile;
  std::string _fluxTuning;


  bool _enableSingleVariationSystConf;
  bool _enableSingleWeightSystConf;
  bool _enableAllSystConfig;
  
  Int_t _ntoys;
  Int_t _randomSeed;
  
public:

  enum enumStandardMicroTrees_baseAnalysis{
    run = enumStandardMicroTreesLast_AnalysisAlgorithm+1,
    subrun,
    evt,
    evt_time, 
    bunch,
    NWEIGHTS,
    weight,
    TruthVertexID,
    TruthVertexNB,
    RooVtxIndex,
    RooVtxEntry,
    RooVtxEntry2,
    RooVtxFile,
    nu_pdg,
    nu_trueE,
    nu_truereac,
    nu_truedir,
    //    truelepton_pdg, // is the same as the category particle
    truelepton_mom,
    truelepton_costheta,
    truelepton_dir,
    truevtx_pos,
    //    truevtx_det, // is the same as the category detector
    selvtx_det,
    selvtx_pos,
    selvtx_truepos,
    true_signal, // declared here, filled in each analysis package
    weight_syst_total_noflux,
    enumStandardMicroTreesLast_baseAnalysis
  };

  enum enumConf_baseAnalysis{
    sipion_syst=ConfigurationManager::enumConfLast_ConfigurationManager+1,    
    siproton_syst,
    nuflux_syst,
    antinuflux_syst,
    all_syst,
    enumConfLast_baseAnalysis
  };

};

#define AddConfiguration(conf, name, ntoys, randomSeed,toyMaker)    conf.AddConfiguration(name,#name,ntoys,randomSeed,toyMaker);
#define GetConfigurationIndex(name) conf().GetConfigurationIndex(#name)



#endif
