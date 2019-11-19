#ifndef CutUtils_h
#define CutUtils_h

#include <typeinfo>

#include "ND280AnalysisUtils.hxx"
#include "BaseDataClasses.hxx"
#include "EventBoxTracker.hxx"
#include "MultiPiBox.hxx"

namespace cutUtils{

  /// Whether the closest TPC segment has > 18 nodes.
  bool TrackQualityCut(const AnaTrackB& track);
  bool TrackQualityCut(AnaTrackB& track);

  /// Whether the TPC segment has > 18 nodes.
  bool TPCTrackQualityCut(const AnaParticleB& tpcSeg);

  /// Whether the track starts in the specified fiducial volume. FGD, FGD1, FGD2
  /// and P0D fiducial volumes are supported, as well as TrECal and DsECal active 
  /// volumes. The detector volumes are defined in DetDef::fgd1min and similar 
  /// variables. The amount by which the detector volume is shrunk to form the 
  /// fiducial volume is specified by FVdefminFGD1 and similar variables.
  bool FiducialCut(const AnaTrackB& track, const SubDetId::SubDetEnum det = SubDetId::kFGD1);
  bool FiducialCut(const Float_t* pos, const SubDetId::SubDetEnum det = SubDetId::kFGD1);

  /// Whether the track is muon-like, as defined for the NuMu analysis.
  bool MuonPIDCut(const AnaTrackB& track, bool prod5Cut = 0);

  /// Whether the track is muon-like, as defined for the anti-NuMu analysis.
  bool AntiMuonPIDCut(const AnaTrackB& track);

  /// Whether the track is muon-like, as defined for the numuBkgInAntiNuMode analysis.
  bool NumuBkgMuonPIDCut(const AnaTrackB& track);

  /// Whether the track is pion-like
  bool PionPIDCut(const AnaTrackB& track);

  /// Whether the track is electron-like
  bool ElectronPIDCut(const AnaTrackB& track);

  /// Whether the track is proton-like
  bool ProtonPIDCut(const AnaTrackB& track);

  /// Whether the track is muon-like using ECAL information.
  bool MuonECALPIDCut(const AnaTrackB& track);

  /// Whether the position is within Barrel ECAL or SMRD FV
  int StoppingBrECALorSMRDCut(const Float_t* pos);

  /// The external veto cut, as defined for the NuMu analysis.
  AnaTrackB* FindVetoTrack(const AnaEventB& event, const AnaTrackB& track); // to save the veto track
  bool ExternalVetoCut(const AnaTrackB& cand, AnaTrackB* vetoTrack);

  /// Returns false if the track starts in the FGD1 FV, and has a start position
  /// that is more downstream than the end position.
  bool DeltaZCut(const AnaTrackB& track);

  /// Find the OOFV FGD track for a given detector and candidate track (this info is in the boxes);
  AnaTrackB* FindFGDOOFVtrack(const AnaEventB& event, const AnaTrackB& mainTrack, SubDetId::SubDetEnum det);

  /// Retuns true if the two tracks start within the input tolerance 
  bool CommonVertexCut(const AnaTrackB& track1, const AnaTrackB& track2, int cutx, int cuty, int cutz);

  /// Returns a veto track if found
  AnaTrackB* FindP0DVetoTrack(const AnaEventC& event);

  /// Tests if a track starts outside a region of the P0D surrounding the FV used to ID veto tracks
  bool IsOutsideP0DVetoCorridor(const AnaTrackB& track);

  /// Tests if a track starting outside the veto corridor ends inside the corridor, will generally veto the bunch if true
  bool IsP0DVetoTrack(const AnaTrackB& track);

  /// Check if a track is FGD contained pion
  bool FGDContainedPionCut(const AnaTrackB& track, SubDetId::SubDetEnum det);

  /// Check that a track corresponds to FGD iso elecron candidate which does not
  /// probably correspond to ME (but e.g. to gamma conversion)
  bool FGDContainedElectronNonMECut(const AnaEventC& event, const AnaTrackB& track, SubDetId::SubDetEnum det);

  /// Collect pion related candidates for the MultiPiBox:
  /// Positive and negative pions in TPC and FGD FV 
  /// Positron and electron candidates for pi0
  /// All considered tracks are required to be of a good TPC quality
  /// Reference track may be provided so not to be considered, 
  /// E.g. already selected as other candidate (e.g. main muon track )
  void FindGoodQualityTPCPionInfoInFGDFV(const AnaEventC& event, const AnaTrackB* reftrack, multipi::MultiPiBox& pionBox, 
      bool useOldSecondaryPID);

  /// Collect pion info using Iso FGD objects
  void FindIsoFGDPionInfo(const AnaEventC& event, multipi::MultiPiBox& pionBox);

  /// Tests if a given ECal object actually corresponds to an ECal shower
  /// There is also a possibility to cut on the position of the most upstream
  /// layer
  bool IsECalShower(const AnaECALParticleB& particle, int MostUpstreamLayerHitCut = 0, 
      Float_t EMEnergyCut = 30., Float_t PIDMipEmCut = 0.);

  /// Same test as above but given a general track
  bool IsECalShower(const AnaTrackB& track, int MostUpstreamLayerHitCut = 0, 
      Float_t EMEnergyCut = 30., Float_t PIDMipEmCut = 0.);

  /// Get the most energetic ECal iso object 
  AnaTrackB* GetMostEnergeticIsoTrackInECal(const AnaEventC& event);


  /// A general function that fill all the information relevant for pions
  /// selection. Fills various variables of MultiPiBox: TPC/FGD pions, 
  /// ECal photons,  TPC/FGD pi0 el/pos candidate,  michel electrons
  /// One has to provide a list of parameters to be considered
  void FillPionInfo(const AnaEventC& event, multipi::MultiPiBox& pionBox, const multipi::PionSelectionParams& params);

  /// Checks that in an event the most energetic ECal iso object 
  /// is not a shower (i.e. vetoes a probably pi-0)
  class  ECalPi0VetoGeneralCutBase: public StepBase{
  public:
    ECalPi0VetoGeneralCutBase(UInt_t layer = 0){
      _MostUpstreamLayerHitCut = layer;
    }

    using StepBase::Apply;

    bool Apply(AnaEventC& event, ToyBoxB& box) const; 
    StepBase* MakeClone(){return new  ECalPi0VetoGeneralCutBase(_MostUpstreamLayerHitCut);}

  protected:  
    UInt_t _MostUpstreamLayerHitCut;
  };

  /// An obsolete method, will be removed when all the selectio adopted
  bool IsECALPi0Photon(AnaEventC& event, AnaTrackB *HO, int MostUpstreamLayerHitCut, SubDetId::SubDetEnum det);

  // An additional namespace to store general cuts and related utils for numu CC multi-pion selection in
  // tracker numu
  namespace numuCCTrkMultiPi{
    
    // Check whether a true object contributes to the MultiBox object (i.e. is a
    // conributor or ancestor to one of them), MEs not considered yet, only
    // "tracks"
    bool CheckTrueCausesBoxSimple(const AnaTrueParticleB&, const multipi::MultiPiBox&);

    class MultiPiBoxGetter{
    public:
      MultiPiBoxGetter(){}
      virtual ~MultiPiBoxGetter(){}
      
    protected:  
      virtual const multipi::MultiPiBox& GetPionBox(const ToyBoxB&) const{
        std::cout << " cutUtils::numuCCTrkMultiPi::MultiPiBoxGetter::GetPionBox() must be implemented in \n";
        std::cout << " class " << typeid(*this).name() << std::endl;
        exit(1); 
      };

    };

    class  NoPionCutBase: public StepBase, MultiPiBoxGetter{
    public:

      using StepBase::Apply;

      bool Apply(AnaEventC& event, ToyBoxB& box) const; 
      StepBase* MakeClone(){return new NoPionCutBase();}

    };

    class  OnePionCutBase: public StepBase, MultiPiBoxGetter{
    public:
      OnePionCutBase(bool mode){
        _posPionMode = mode;
      }
      virtual ~OnePionCutBase(){}

      using StepBase::Apply;

      bool Apply(AnaEventC& event, ToyBoxB& box) const; 
      StepBase* MakeClone(){return new OnePionCutBase(_posPionMode);}


    protected:  
      bool _posPionMode;
    };


    class  OthersCutBase: public StepBase, MultiPiBoxGetter{
    public:
      using StepBase::Apply;

      bool Apply(AnaEventC& event, ToyBoxB& box) const; 
      StepBase* MakeClone(){return new OthersCutBase();} 
    protected:
    };

    class ECalPi0VetoCutBase: public StepBase, MultiPiBoxGetter{  
    public:
      using StepBase::Apply;
      bool Apply(AnaEventC& event, ToyBoxB& box) const;
      StepBase* MakeClone(){return new ECalPi0VetoCutBase();}
    };
  };
  
  



};

#endif
