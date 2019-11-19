#ifndef MultiPiBox_h
#define MultiPiBox_h

#include "BaseDataClasses.hxx"
#include "CoreUtils.hxx"

namespace multipi{
  /// This is a structure to define parameters relevant for pion info selection
  /// Basically whether to fill certain variables and which type of cuts to
  /// apply
  /// It also has a reference track: e.g. the main track not to be considered as
  /// pion candidate
  struct PionSelectionParams{
  public:
    PionSelectionParams(){
      useTPCPions                 = false;
      useME                       = false;
      useFGDPions                 = false;
      useOldSecondaryPID          = false;
      prod5Cut                    = false;
      useECalPiZeroInfo           = false;
      ECalMostUpstreamLayerHitCut = 0;
      ECalEMEnergyCut             = 0.;
      ECalPIDMipEmCut             = 0.;
      refTrack                    = NULL;
      
    }

    bool       useTPCPions;
    bool       useME;
    bool       useFGDPions;
    bool       useOldSecondaryPID;
    bool       prod5Cut;
    bool       useECalPiZeroInfo;
    Int_t      ECalMostUpstreamLayerHitCut;
    Float_t    ECalEMEnergyCut;
    Float_t    ECalPIDMipEmCut;
    AnaTrackB* refTrack;
  };



  /// This is a collection of objects useful e.g. for ToyBoxes used 
  /// for analysis which utilize various pion topologies
  struct MultiPiBox{
    MultiPiBox(){
      nPositivePionTPCtracks = 0;
      nNegativePionTPCtracks = 0;
      nMichelElectrons       = 0;
      nIsoFGDPiontracks      = 0;
      nPosPi0TPCtracks       = 0;
      nElPi0TPCtracks        = 0;
      nIsoFGDElPi0tracks     = 0;
      nPosPions              = 0;
      nNegPions              = 0;
      nOtherPions            = 0;
      
      Detector = SubDetId::kInvalid;

      anaUtils::ClearArray(PositivePionTPCtracks, 100);
      anaUtils::ClearArray(NegativePionTPCtracks, 100);
      anaUtils::ClearArray(MichelElectrons,       100);
      anaUtils::ClearArray(IsoFGDPiontracks,      100);
      anaUtils::ClearArray(PosPi0TPCtracks,       100);
      anaUtils::ClearArray(ElPi0TPCtracks,        100);
      anaUtils::ClearArray(IsoFGDElPi0tracks,     100);

      ECalPi0Photon = NULL; 
    }

    virtual ~MultiPiBox(){}


    void Reset(){
      anaUtils::ClearArray(PositivePionTPCtracks, nPositivePionTPCtracks);
      anaUtils::ClearArray(NegativePionTPCtracks, nNegativePionTPCtracks);
      anaUtils::ClearArray(MichelElectrons,       nMichelElectrons);
      anaUtils::ClearArray(IsoFGDPiontracks,      nIsoFGDPiontracks);
      anaUtils::ClearArray(PosPi0TPCtracks,       nPosPi0TPCtracks);
      anaUtils::ClearArray(ElPi0TPCtracks,        nElPi0TPCtracks);
      anaUtils::ClearArray(IsoFGDElPi0tracks,     nIsoFGDElPi0tracks);

      nPositivePionTPCtracks = 0;
      nNegativePionTPCtracks = 0;
      nMichelElectrons       = 0;
      nIsoFGDPiontracks      = 0;
      nPosPi0TPCtracks       = 0;
      nElPi0TPCtracks        = 0;
      nIsoFGDElPi0tracks     = 0;
      nPosPions              = 0;
      nNegPions              = 0;
      nOtherPions            = 0;

      ECalPi0Photon = NULL; 
    }

    // positive  TPC pion candidates 
    AnaTrackB* PositivePionTPCtracks[100];
    int nPositivePionTPCtracks;

    // negative TPC pion candidates 
    AnaTrackB* NegativePionTPCtracks[100];
    int nNegativePionTPCtracks;

    //  Michel Electron candidates 
    AnaFgdTimeBinB* MichelElectrons[100];
    int nMichelElectrons;

    // IsoFGD Pion candidates 
    AnaTrackB* IsoFGDPiontracks[100];
    int nIsoFGDPiontracks;

    // Electrons and positrons from Pio in the TPC
    AnaTrackB* PosPi0TPCtracks[100];
    AnaTrackB* ElPi0TPCtracks[100];
    int nPosPi0TPCtracks;
    int nElPi0TPCtracks;

    // IsoFGD electron and positron candidates 
    AnaTrackB* IsoFGDElPi0tracks[100];
    int nIsoFGDElPi0tracks;

    AnaTrackB* ECalPi0Photon; 

    int nPosPions;
    int nNegPions;
    int nOtherPions;
    
    SubDetId::SubDetEnum Detector;

  };
};
#endif
