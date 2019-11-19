#ifndef nueCCSelection_h
#define nueCCSelection_h

#include "SelectionBase.hxx"
#include "Parameters.hxx"
#include "ToyBoxTracker.hxx"
#include "EventBoxId.hxx"
#include "SystId.hxx"
#include "SubDetId.hxx"

class nueCCSelection: public SelectionBase{
public:
  nueCCSelection(bool forceBreak=true);
  virtual ~nueCCSelection(){}

  //---- These are mandatory functions
  void DefineSteps();
  void DefineDetectorFV();
  inline ToyBoxB* MakeToyBox();
  void InitializeEvent(AnaEventC& event);
  bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);

  SampleId_h GetSampleId(){return SampleId::kFGD1NuECC;}

  bool CheckRedoSelection(const AnaEventC& eventC, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep);
  bool IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* track, SystId_h systId, Int_t branch) const;
  bool IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueTrack, SystId_h systId, Int_t branch) const;
  bool IsRelevantRecObjectForSystematicInToy(const AnaEventC&, const ToyBoxB&, AnaRecObjectC*, SystId_h systId, Int_t branch=0) const;
  bool IsRelevantTrueObjectForSystematicInToy(const AnaEventC&, const ToyBoxB&, AnaTrueObjectC*, SystId_h systId, Int_t branch=0) const;
  bool IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const;

  Int_t GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;
  Int_t GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;

protected:
  Int_t _FindLeadingTracksStepIndex;
  Int_t _TotalMultiplicityCutIndex;
  Int_t _ElecPIDCutIndex;
  Int_t _ElecPIDStepIndex;

  // Variables needed from datacard
  Float_t _Ethreshold;

};

/// This class stores the objects used by the package.
/// E.g. Selected tracks, vertex, veto track etc...
class ToyBoxNueCC: public ToyBoxTracker{

public:
  ToyBoxNueCC(){
    VetoTrack = NULL;
    ECalVetoTrack = NULL;
    ECalNCVetoTrack = NULL;
    SecondMostEnergeticTPCTrack = NULL;
    SecondMostEnergeticFGDTPCTrack = NULL;
    PairTrack = NULL;
    PairTrack_ForSyst = NULL;

    P0DVetoP0DTracks = 0;
    P0DVetoFGD1Tracks = 0;
    P0DVetoP0DEcalTracks = 0;
    TPCVetoP0DNearTracks = 0;
    TPCVetoNearTracks = 0;
    TPCVetoFarTracks = 0;
    FGD2ShowerNFGD2TPC3Tracks = 0;
    FGD2ShowerNFGD1TPC2Tracks = 0;
    NOOFVTracks = 0;
  }

  void Reset(){

    VetoTrack = NULL;
    ECalVetoTrack = NULL;
    ECalNCVetoTrack = NULL;
    SecondMostEnergeticTPCTrack = NULL;
    SecondMostEnergeticFGDTPCTrack = NULL;
    PairTrack = NULL;
    PairTrack_ForSyst = NULL;

    P0DVetoP0DTracks = 0;
    P0DVetoFGD1Tracks = 0;
    P0DVetoP0DEcalTracks = 0;
    TPCVetoP0DNearTracks = 0;
    TPCVetoNearTracks = 0;
    TPCVetoFarTracks = 0;
    FGD2ShowerNFGD2TPC3Tracks = 0;
    FGD2ShowerNFGD1TPC2Tracks = 0;
    NOOFVTracks = 0;
  }  

  virtual ~ToyBoxNueCC(){};

  /// The TPC veto track
  AnaTrackB* VetoTrack;

  /// The most-upstream track containing ECal information.
  AnaTrackB* ECalVetoTrack;
  AnaTrackB* ECalNCVetoTrack;

  /// The second most energetic TPC track
  AnaTrackB* SecondMostEnergeticTPCTrack;

  /// The second most energetic TPC track in the same FGD as the main track
  AnaTrackB* SecondMostEnergeticFGDTPCTrack;

  /// The particle that isn't HM track selected that forms the e+e- pair with the lowest invariant mass
  AnaTrackB* PairTrack;

  /// Pair track that has the same charge as the HM track. Only used for systematics
  AnaTrackB* PairTrack_ForSyst;

  /// The number of tracks that start in the P0D. Used in the P0D veto cut
  Int_t P0DVetoP0DTracks;

  /// The number of tracks that start in FGD1. Used in the P0D veto cut
  Int_t P0DVetoFGD1Tracks;

  /// Number of P0DEcal tracks. Used in the P0D veto cut
  Int_t P0DVetoP0DEcalTracks;

  /// Updated TPC veto
  Int_t TPCVetoNearTracks;
  Int_t TPCVetoFarTracks;
  Int_t TPCVetoP0DNearTracks;

  /// FGD2 shower variables: Number of FGD1-TPC2 tracks and FGD2-TPC3 tracks
  Int_t FGD2ShowerNFGD2TPC3Tracks;
  Int_t FGD2ShowerNFGD1TPC2Tracks;

  /// OOFV tracks
  Int_t NOOFVTracks;
};

inline ToyBoxB* nueCCSelection::MakeToyBox() {return new ToyBoxNueCC();}

class FillSummaryAction_nueCC: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;  
  StepBase* MakeClone(){return new FillSummaryAction_nueCC();}
};

class FillLeadingTracksAction: public StepBase{
public:
  FillLeadingTracksAction(){
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new FillLeadingTracksAction();}
};

class FillVertex: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new FillVertex();}
};

class FindECalVetoTrackAction: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new FindECalVetoTrackAction();}
};

class FindP0DVetoTrackAction: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new FindP0DVetoTrackAction();}
};

class MultiplicityCut: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new MultiplicityCut();}
};

class TrackQualityCut: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new TrackQualityCut();}
};

class FillFGDIsoTracksAction: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new FillFGDIsoTracksAction();}
};

class TPCElectronPullCut: public StepBase{
public:
  TPCElectronPullCut(){
    /// Loose e-pull cut all events must pass, regardless of TPC path
    _pullel_accept_min       = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.PID.PullElecMin");
    _pullel_accept_max       = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.PID.PullElecMax");
    /// Tight electron pull applied only to the events not going in the Ecal
    _pullel_accept_tight_min = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.PID.PullElecTightMin");
    _pullel_accept_tight_max = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.PID.PullElecTightMax");

    _minMom_ecal             = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.EcalThreshold");
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new TPCElectronPullCut();}
protected:
  Float_t _minMom_ecal;
  Float_t _pullel_accept_min;
  Float_t _pullel_accept_max;
  Float_t _pullel_accept_tight_min;
  Float_t _pullel_accept_tight_max;
};

class TPCMuonPullCut: public StepBase{
public:
  TPCMuonPullCut(){
    _pullmu_reject_min       = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.PID.PullMuonMin");
    _pullmu_reject_max       = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.PID.PullMuonMax");

    _minMom_ecal             = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.EcalThreshold");
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new TPCMuonPullCut();}
protected:
  Float_t _minMom_ecal;
  Float_t _pullmu_reject_min;
  Float_t _pullmu_reject_max;
};

class TPCPionPullCut: public StepBase{
public:
  TPCPionPullCut(){
    _pullpi_reject_min       = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.PID.PullPionMin");
    _pullpi_reject_max       = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.PID.PullPionMax");
    _minMom_ecal             = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.EcalThreshold");
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new TPCPionPullCut();}
protected:
  Float_t _minMom_ecal;
  Float_t _pullpi_reject_min;
  Float_t _pullpi_reject_max;
};

class EcalEMEnergyCut: public StepBase{
public:
  EcalEMEnergyCut(){
    _Ethreshold     = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.PID.EThreshold");
    _Emin_ecal      = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.PID.EcalEMin");
    _EoverP         = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.PID.EoverP");
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new EcalEMEnergyCut();}
protected:
  Float_t _Ethreshold;
  Float_t _Emin_ecal;
  Float_t _EoverP;
};

class EcalMIPEMPIDCut: public StepBase{
public:
  EcalMIPEMPIDCut(){
    _Ethreshold     = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.PID.EThreshold");
    _ecal_mipem_min = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.PID.EcalMipEmMin");
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new EcalMIPEMPIDCut();}
protected:
  Float_t _Ethreshold;
  Float_t _ecal_mipem_min;
};

class SecondTPCPIDCut: public StepBase{
public:
  SecondTPCPIDCut(){
    _pullmu_reject_min = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.PID2.PullMuonMin");
    _pullmu_reject_max = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.PID2.PullMuonMax");
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new SecondTPCPIDCut();}
protected:
  Float_t _pullmu_reject_min;
  Float_t _pullmu_reject_max;
};

class FillTPCVetoTrackAction: public StepBase{
public:
  FillTPCVetoTrackAction(){
    _vertex_activity = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.GammaIso.VertexActivity");
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new FillTPCVetoTrackAction();}
protected:
  Float_t _vertex_activity;
};

class TPCVetoCut: public StepBase{
public:
  TPCVetoCut(){
    _delta_z_min = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.TPCVeto.DeltaZMin.FGD1");
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new TPCVetoCut();}
protected:
  Float_t _delta_z_min;
};

class GammaIsoVetoCut: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new GammaIsoVetoCut();}
};

class FindPairsAction: public StepBase{
public:
  FindPairsAction(){
    _delta_pos_max = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.PairFinder.DeltaPosMax");
    _pullel_min    = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.PairFinder.PullElecMin");
    _pullel_max    = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.PairFinder.PullElecMax");
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new FindPairsAction();}
protected:
  Float_t _delta_pos_max;
  Float_t _pullel_min;
  Float_t _pullel_max;
};

class PairCut: public StepBase{
public:
  PairCut(){
    _inv_mass_min = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.PairFinder.InvMassMin");
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new PairCut();}
protected:
  Float_t _inv_mass_min;
};

class P0DVetoCut: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new P0DVetoCut();}
};

class HighestMomentumCut: public StepBase{
public:
  HighestMomentumCut(){
    _min_mom = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.HiMom.MinMom");
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new HighestMomentumCut();}
protected:
  Float_t _min_mom;
};

class ECalVetoCut: public StepBase{
public:
  ECalVetoCut(){
    _delta_z_min = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.ECalVeto.DeltaZMin");
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new ECalVetoCut();}
protected:
  Float_t _delta_z_min;
};

class ToFCut: public StepBase{
public:
  ToFCut(){
    _fgdecaltof        = (Float_t)ND::params().GetParameterD("psycheSelections.nueCCAnalysis.Cuts.ToF.FGDEcal");
  };
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new ToFCut();}
protected:
  Float_t _fgdecaltof;
};

class ExternalFGD1layersCut: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new ExternalFGD1layersCut();}
};

class MomentumQualityCut: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new MomentumQualityCut();}
};

#endif
