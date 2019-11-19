#ifndef baseTrackerAnalysis_h
#define baseTrackerAnalysis_h

#include "baseAnalysis.hxx"

#include "ChargeIDEffSystematics.hxx"
#include "TPCTrackEffSystematics.hxx"
#include "FGDTrackEffSystematics.hxx"
#include "FGDHybridTrackEffSystematics.hxx"
#include "TPCFGDMatchEffSystematics.hxx"
#include "MichelElectronEffSystematics.hxx"

// High-angle matching
#include "FGDECalMatchEffSystematics.hxx"
#include "FGDECalSMRDMatchEffSystematics.hxx"

// TPC->P0D/ECal matching
#include "TPCP0DMatchEffSystematics.hxx"
#include "TPCECalMatchEffSystematics.hxx"

//ECal
#include "ECalTrackEffSystematics.hxx"

// ECal PID
#include "ECalPIDSystematics.hxx"

// NuE specific
#include "ECalEmHipPIDSystematics.hxx"
#include "FGD2ShowerSystematics.hxx"

/// This class provides a base for user analyses. It handles many common
/// functions, and provides a structure so that users don't have to write too
/// much "boiler-plate" code.
///
/// User analyses should inherit from this class.
class baseTrackerAnalysis: public baseAnalysis{
 public:
  /// Constructor, which instantiates the necessary converters for converting
  /// input files to the AnaSpill format.
  baseTrackerAnalysis(AnalysisAlgorithm* ana=NULL);
  virtual ~baseTrackerAnalysis(){}

  virtual bool Initialize(){return baseAnalysis::Initialize();}

  virtual void DefineCorrections();
  virtual void DefineSystematics();
  virtual void DefineConfigurations();
  virtual void DefineMicroTrees(bool addBase=true);
  virtual void DefineTruthTree();

  virtual void FillMicroTrees(bool addBase=true);
  virtual void FillToyVarsInMicroTrees(bool addBase=true);
  virtual void FillTruthTreeBase(const AnaTrueVertex& vtx, const SubDetId::SubDetEnum det=SubDetId::kFGD1, bool IsAntinu = false);

  /// Returns the ToyBoxTracker
  virtual const ToyBoxTracker& box(Int_t isel=-1) const {return *static_cast<const ToyBoxTracker*>(&boxB(isel));}
  
  /// Returns the vertex for the ToyBoxTracker
  virtual AnaVertexB* GetVertex() const{return box().Vertex;}

  /// Returns the true vertex for the ToyBoxTracker
  virtual AnaTrueVertexB* GetTrueVertex() const {return box().TrueVertex;}


 protected:


  /// Compute analysis sample efficiency
  bool _computeEfficiency;

  ChargeIDEffSystematics          *_chargeid;
  TPCFGDMatchEffSystematics       *_tpcfgdmatch;
  TPCTrackEffSystematics          *_tpctr;
  FGDTrackEffSystematics          *_fgdtr;
  FGDHybridTrackEffSystematics    *_fgdhybtr;
  MichelElectronEffSystematics    *_me;
  TPCECalMatchEffSystematics      *_tpc_ecal_matcheff; 
  TPCP0DMatchEffSystematics       *_tpc_p0d_matcheff;
  FGDECalMatchEffSystematics      *_fgd_ecal_matcheff;
  FGDECalSMRDMatchEffSystematics  *_fgd_ecal_smrd_matcheff;
  // Keep ECal-iso tracking efficiency here for the moment since used for tracker-based analysis so far
  ECalTrackEffSystematics         *_ecal_trackeff;
  ECalPIDSystematics              *_ecal_pid;
  ECalEmHipPIDSystematics         *_ecal_emhippid;
  FGD2ShowerSystematics           *_fgd2shower;

public:

  enum enumStandardMicroTrees_baseTrackerAnalysis{
    ntpctracks = enumStandardMicroTreesLast_baseAnalysis+1,
    ntpcposQualityFV,
    ntpcnegQualityFV,
    nfgdtracks,
    nfgdonlytracks,
    chargeid_ncorrect,
    chargeid_nwrong,
    tpcfgdmatch_ncorrect,
    tpcfgdmatch_nwrong,
    tpctr_ncorrect,
    tpctr_nwrong,
    fgdtr_ncorrect,
    fgdtr_nwrong,
    fgdhybtr_ncorrect,
    fgdhybtr_nwrong,
    meeff_ncorrect,
    meeff_nwrong,
    mepur_ncorrect,
    mepur_nwrong,
    tpc_ecal_match_ncorrect,
    tpc_ecal_match_nwrong,
    tpc_p0d_match_ncorrect,
    tpc_p0d_match_nwrong,
    fgd_ecal_match_ncorrect,
    fgd_ecal_match_nwrong,
    fgd_ecal_smrd_match_ncorrect,
    fgd_ecal_smrd_match_nwrong,
    ecal_pid_ncorrect, 
    ecal_pid_nwrong, 
    ecal_tr_ncorrect, 
    ecal_tr_nwrong,
    ecal_emhippid_ncorrect, 
    ecal_emhippid_nwrong,
    fgd2shower_ncorrect,
    fgd2shower_nwrong,

    enumStandardMicroTreesLast_baseTrackerAnalysis
  };

  enum enumConf_baseTrackerAnalysis{
    bfield_syst=baseAnalysis::enumConfLast_baseAnalysis+1,
    momscale_syst,
    momresol_syst,
    momrange_resol_syst,
    mombias_from_vertex_migration_syst, 
    tpcpid_syst,
    fgdpid_syst,
    chargeideff_syst,
    tpctrackeff_syst,
    fgdtrackeff_syst,
    fgdhybridtrackeff_syst,
    tpcfgdmatcheff_syst,
    tpcclustereff_syst,
    michel_syst,
    oofv_syst,
    pileup_syst,
    fgdmass_syst,
    sandmu_syst,
    ecal_emresol_syst, 
    ecal_emscale_syst,
    tpc_ecal_matcheff_syst, 
    tpc_p0d_matcheff_syst, 
    fgd_ecal_matcheff_syst, 
    fgd_ecal_smrd_matcheff_syst,
    ecal_trackeff_syst,
    ecal_pid_syst, 
    tof_resol_syst, 
    ecal_emhippid_syst,
    fgd2shower_syst,
    nuetpcpileup_syst,
    nuep0dpileup_syst,
    nueecalpileup_syst,
    nueoofv_syst,
    p0d_elossscale_syst,
    p0d_elossresol_syst,
    p0d_veto_syst,
    enumConfLast_baseTrackerAnalysis
  };

};

#endif
