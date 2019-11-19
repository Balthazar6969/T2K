#ifndef _T2K_GENIE_DEFS_H_
#define _T2K_GENIE_DEFS_H_
//____________________________________________________________________________
//____________________________________________________________________________
//
//  Set of definitions taken from GENIE and T2KReWeight that are used by 
//  GRooTrackerUtils to look at the detailed neutrino generator pass-through 
//  information. Jim Dobson - Dec 19th, 2009.
//          
//  Files adapted from GENIE and T2KReWeight - kept original code/comments.
//
//____________________________________________________________________________
//
//\file     PDGCodes.h
//
//\brief    Most commonly used PDG codes.
//          A set of utility functions to handle PDG codes is provided in PDGUtils
//
//\author   Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
//          STFC, Rutherford Appleton Laboratory
//
//\created  May 06, 2004
//
//\cpright  Copyright (c) 2003-2008, GENIE Neutrino MC Generator Collaboration
//          For the full text of the license visit http://copyright.genie-mc.org
//          or see $GENIE/LICENSE 
//
//____________________________________________________________________________
//namespace GENIEDEFS {

  const int kPdgNuE          =  12;
  const int kPdgAntiNuE      = -12;
  const int kPdgNuMu         =  14;
  const int kPdgAntiNuMu     = -14;
  const int kPdgNuTau        =  16;
  const int kPdgAntiNuTau    = -16;
  
  const int kPdgElectron     =  11;
  const int kPdgPositron     = -11;
  const int kPdgMuon         =  13;
  const int kPdgAntiMuon     = -13;
  const int kPdgTau          =  15;
  const int kPdgAntiTau      = -15;
  
  const int kPdgUQuark       =   2;
  const int kPdgAntiUQuark   =  -2;
  const int kPdgDQuark       =   1;  
  const int kPdgAntiDQuark   =  -1;
  const int kPdgSQuark       =   3;
  const int kPdgAntiSQuark   =  -3;
  const int kPdgCQuark       =   4;
  const int kPdgAntiCQuark   =  -4;
  const int kPdgBQuark       =   5;
  const int kPdgAntiBQuark   =  -5;
  const int kPdgTQuark       =   6;
  const int kPdgAntiTQuark   =  -6;
  
  const int kPdgDDDiquarkS1  =  1103; // dd, spin = 1 
  const int kPdgUDDiquarkS0  =  2101; // ud, spin = 0 
  const int kPdgUDDiquarkS1  =  2103; // ud, spin = 1 
  const int kPdgUUDiquarkS1  =  2203; // uu, spin = 1 
  const int kPdgSDDiquarkS0  =  3101; // sd, spin = 0
  const int kPdgSDDiquarkS1  =  3103; // sd, spin = 1
  const int kPdgSUDiquarkS0  =  3201; // su, spin = 0
  const int kPdgSUDiquarkS1  =  3201; // su, spin = 1
  const int kPdgSSDiquarkS1  =  3303; // ss, spin = 1
  
  const int kPdgProton       =  2212;
  const int kPdgAntiProton   = -2212;
  const int kPdgNeutron      =  2112;
  const int kPdgAntiNeutron  = -2112;
  const int kPdgDeltaM       =  1114; // Delta-
  const int kPdgDeltaP       =  2214; // Delta+
  const int kPdgDeltaPP      =  2214; // Delta++
  const int kPdgLambda       =  3122; // Lambda
  const int kPdgAntiLambda   = -3122; // \bar{Lambda}
  const int kPdgSigmaP       =  3222; // Sigma+
  const int kPdgSigma0       =  3212; // Sigma0
  const int kPdgSigmaM       =  3112; // Sigma-
  const int kPdgAntiSigmaP   = -3222; // \bar{Sigma+}
  const int kPdgAntiSigma0   = -3212; // \bar{Sigma0}
  const int kPdgAntiSigmaM   = -3112; // \bar{Sigma-}
  const int kPdgXi0          =  3322; // Xi0
  const int kPdgXiM          =  3312; // Xi-
  const int kPdgAntiXi0      = -3322; // \bar{Xi0}
  const int kPdgAntiXiP      = -3312; // \bar{Xi+}
  const int kPdgOmegaM       =  3332; // Omega-
  const int kPdgAntiOmegaP   = -3332; // \bar{Omega+}
  const int kPdgLambdaPc     =  4122; // Lambda+_{c}
  const int kPdgSigmaPc      =  4212; // Sigma+_{c}
  const int kPdgSigmaPPc     =  4222; // Sigma++_{c}
  
  const int kPdgPiP          =   211; // pi+
  const int kPdgPiM          =  -211; // pi-
  const int kPdgPi0          =   111; // pi0
  const int kPdgEta          =   221; // eta
  const int kPdgEtaPrm       =   331; // eta' (prime)
  const int kPdgEtac         =   441; // eta_{c}
  const int kPdgEtab         =   551; // eta_{b}
  const int kPdgRhoP         =   213; // rho+
  const int kPdgRhoM         =  -213; // rho-
  const int kPdgRho0         =   113; // rho0
  const int kPdgomega        =   223; // omega (the meson, not Omega the baryon)
  const int kPdgPhi          =   333; // phi
  const int kPdgJpsi         =   443; // J/psi
  const int kPdgY            =   553; // Y
  const int kPdgKP           =   321; // K+
  const int kPdgKM           =  -321; // K-
  const int kPdgK0           =   311; // K0
  const int kPdgAntiK0       =  -311; // \bar{K0}
  const int kPdgK0L          =   130; // K0_{long}
  const int kPdgK0S          =   310; // K0_{short}
  const int kPdgDP           =   411; // D+
  const int kPdgDM           =  -411; // D-
  const int kPdgD0           =   421; // D0
  const int kPdgAntiD0       =  -421; // \bar{D0}
  const int kPdgDPs          =   431; // D+_{s}
  const int kPdgDMs          =  -431; // D-_{s}
  
  const int kPdgGluon        =    21; // gluon
  const int kPdgGamma        =    22; // photon
  const int kPdgZ0           =    23; // Z
  const int kPdgWP           =    24; // W+
  const int kPdgWM           =   -24; // W-
  
  // Note: PDG codes for nuclear targets can be computed using pdg::IonPdgCode(A,Z)
  // PDG2006 convention: 10LZZZAAAI 
  // Define names for some commonly used nuclear PDG codes:
  const int kPdgTgtFreeP     = 1000010010;
  const int kPdgTgtFreeN     = 1000000010;
  const int kPdgTgtC12       = 1000060120;
  const int kPdgTgtO16       = 1000080160;
  const int kPdgTgtFe56      = 1000260560;
  
  // PDG codes for GENIE special particles
  
  const int kPdgHadronicSyst = 2000000001; // dis hadronic system before hadronization
  const int kPdgHadronicBlob = 2000000002; // unmodelled fraction of the hadronic system
  const int kPdgBindino      = 2000000101; // binding energy subtracted from f/s nucleons
  const int kPdgCoulobtron   = 2000000102; // coulomb energy subtracted from f/s leptons
  
  // PDG codes for PYTHIA/JETSET special particles
  const int kPdgCluster      = 91; 
  const int kPdgString       = 92; 
  const int kPdgIndep        = 93; 

//____________________________________________________________________________
/*!

\class    genie::GHepStatus

\brief    GHepParticle Status

\author   Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
          STFC, Rutherford Appleton Laboratory

\created  November 20, 2004

\cpright  Copyright (c) 2003-2009, GENIE Neutrino MC Generator Collaboration
          For the full text of the license visit http://copyright.genie-mc.org
          or see $GENIE/LICENSE
*/ 
//____________________________________________________________________________

typedef enum EGHepStatus {
   kIStUndefined                  = -1, 
   kIStInitialState               =  0,   /* generator-level initial state */
   kIStStableFinalState           =  1,   /* generator-level final state: particles to be tracked by detector-level MC */
   kIStIntermediateState          =  2,
   kIStDecayedState               =  3,
   kIStCorrelatedNucleon          = 10,
   kIStNucleonTarget              = 11,
   kIStDISPreFragmHadronicState   = 12,
   kIStPreDecayResonantState      = 13,
   kIStHadronInTheNucleus         = 14,   /* hadrons inside the nucleus: marked for hadron transport modules to act on */
   kIStFinalStateNuclearRemnant   = 15,   /* low energy nuclear fragments entering the record collectively as a 'hadronic blob' pseudo-particle */
   kIStNucleonClusterTarget       = 16
} 
GHepStatus_t; 

class GHepStatus {
 public:

  static const char * AsString(GHepStatus_t Ist) {
     switch (Ist) {
     case kIStUndefined:                
           return  "[undefined status]"; 
           break;
     case kIStInitialState:             
           return  "[initial state]"; 
           break;
     case kIStStableFinalState:         
           return  "[stable final state]"; 
           break;
     case kIStIntermediateState:       
           return  "[intermediate state]"; 
           break;
     case kIStDecayedState:            
           return  "[decayed state]"; 
           break;
     case kIStCorrelatedNucleon:
           return  "[other energetic initial state nucleons]"; 
           break;
     case kIStNucleonTarget:            
           return  "[nucleon target]"; 
           break;
     case kIStDISPreFragmHadronicState: 
           return  "[DIS pre-fragm. hadronic state]"; 
           break;
     case kIStPreDecayResonantState:   
           return  "[resonant pre-decayed state]"; 
           break;
     case kIStHadronInTheNucleus:     
           return  "[hadron in the nucleus]"; 
           break;
     case kIStFinalStateNuclearRemnant:     
           return  "[nuclear remnant]"; 
           break;
     case kIStNucleonClusterTarget:     
           return  "[nucleon cluster target]"; 
           break;
     default:  break;
     }
     return "[-]";
  }
};
/*
------------------------------------------------------------------------------------
Physics constants

Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>, Rutherford Lab, 01/05/2008
------------------------------------------------------------------------------------
*/

  const double kPcWaterRefrIdx     =  1.33;                // water refractive index
  const double kPcElectronMass     =  0.00055857990945;    // GeV
  const double kPcMuonMass         =  0.105658369;         // GeV
  const double kPcPiChMass         =  0.13957018;          // GeV
  const double kPcPi0Mass          =  0.1349766;           // GeV
  const double kPcKChMass          =  0.493677;            // GeV
  const double kPcK0Mass           =  0.497648;            // GeV
  const double kPcProtonMass       =  0.9382720;           // GeV
  const double kPcNeutronMass      =  0.9395653;           // GeV
  const double kPcNucleonMass      =  (kPcProtonMass+kPcNeutronMass)/2.; // GeV, isoscalar

//
// GENIE headers
//
//#include "HadronTransport/INukeHadroFates.h"
  const int kGStdHepNPmax = 100;
  const int kGStdHepIdxPx = 0;
  const int kGStdHepIdxPy = 1;
  const int kGStdHepIdxPz = 2;
  const int kGStdHepIdxE  = 3;
  const int kGStdHepIdxX  = 0;
  const int kGStdHepIdxY  = 1;
  const int kGStdHepIdxZ  = 2;
  const int kGStdHepIdxT  = 3;

//}         // geniedefs


#endif
 
