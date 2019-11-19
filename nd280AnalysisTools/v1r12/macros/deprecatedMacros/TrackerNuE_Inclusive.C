
// C. Giganti - November 2009
// ////Cut list/////////////////////////////////////////////////////////////
// 
// Cut 0 --> require at least 1 track in the TPC (eventually negative)    //
// Cut 1 --> FGD Fiducial Volume                                          //
// Cut 2 --> Less than 3 tracks (if CCQE)                                 // 
// Cut 3 --> Negative track                                               //
// Cut 4 --> TPC PID cut                                                  //
// Cut 5 --> momentum > 200 MeV                                           // 
// Cut 6 --> N proton tracks (if CCQE)                                    //
// Cut 7 --> N elike tracks                                               //
// Cut 8 --> ECAL PID                                                     //
////////////////////////////////////////////////////////////////////////////

#define AllNeg false; ///if true select the most energetic and negative track if false select the most energetic track

#define CCQESel false;  //perform a CCQE analysis, requiring max 2 tr and 2nd track proton like

#define MoreMomBinCurv false; ///cut on the curvature on 2 momentum bin
#define FirstMomBin 2000;  //define the 2 momentum bins
#define SecondMomBin 3000; 

#define CurvCut 0;  ///number of sigma for the cut on the curvature (in all the range or in the first bin)

#define PullMin -2.5;  ///default cut on the pull in the electron hypothesis
#define PullMax 3;   

#define PIDMomBin false;  //possibility of define 2 momentum bin for the pull cut
#define FirstPullBinMin -2.5 ;
#define FirstPullBinMax 3 ;
#define SecondPullBinMin -1.5 ;
#define SecondPullBinMax 2 ;

#define MomentumMin 200;  ///cut all the tracks below this momentum

#define ECALTrSh 0.5;   //cut on the ECAL PID to select muons or electrons

#define FileName "MDC0_input_NuE.txt";

#define verboseclean false;

#define DoPIDPlots false;

#include "stdio.h"
#include "iostream"
#include "vector"
#include <utility>
#include <fstream>

#include <TSystem.h>
#include <TROOT.h>
#include <TApplication.h>
#include <memory>
#include <string>
#include <TCanvas.h>
#include <TStyle.h>
#include <TF1.h>
#include <TH1.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TMultiGraph.h>
#include <TBox.h>
#include <TNtuple.h>
#include <TFile.h>
#include <TChain.h>
#include <TProfile.h>
#include <TRefArray.h>
#include <TClonesArray.h>
#include <TLegend.h>
#include <TGeoManager.h>
#include <TTree.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TStopwatch.h>
#include <TLatex.h>
#include <TString.h>
#include <TRandom.h>
#include <time.h>
#include <TGClient.h>


//______________________________________________________________________________________
// 
// This routine allows one to do a rudimentary CCQE selection using Tracker 
// reconstruction information stored in oaAnalysis ROOT tree.
// 
// In order to run this script, you must do the following.
//
// 1) You must have a file (or files) containing the oaAnalysis summary ROOT tree.  
//    This must be generated using OAAnalysis.exe, if you don't already 
//    have it.
// 2) Build the oaAnalysis library in the normal way.
// 3) Go to the macros directory.
// 4) Edit the file called 'ccqe_input_files.txt': this should be a list of all
//    the summary files that you want to process.
// 5) Start root and run this macro by doing:
//    .x doCcqeAnalysis.C()
// 
//    The macro will loop over all the events in your selected files and 
//    output some CCQE selection efficiency and purity distributions.
//
//    A description of the selection cuts is given here:
//
//    http://www.nd280.org/meetings/nd280200801/talks/software/tracker_ccqe_event_selection_v2.pdf
//
// T. Lindner - Jan 28, 2008
//





char s[1];

void TrackerNuE_Inclusive() {

// Load some important libraries.

  gSystem->AddIncludePath("-I$OAEVENTROOT/src");
  gSystem->AddIncludePath("-I$OAANALYSISROOT/src");
  gSystem->AddIncludePath("-I.");
  // gROOT->ProcessLine(".x rootlogon.C");
  
  //TEMPORARY SOLUTION
  gSystem->Load("libPhysics");
 


  gSystem->Load("libGeom");
  gSystem->Load("libTree");
  gSystem->Load("libMatrix");
  gSystem->Load("liboaEvent.so");
  gSystem->Load("liboaAnalysis.so");
  gROOT->ProcessLine(".L tracker_utilities.C");
  

 
  TChain mcTruthVertices = TChain("TruthDir/Vertices");
  TChain mcTruthTrajectories = TChain("TruthDir/Trajectories");
  TChain reconTracker = TChain("ReconDir/Tracker");
  TChain reconECAL = TChain("ReconDir/TrackerECal");

  //______________________________________________________________________________________
  // Set constants for analysis

  // Parameters defines how many mm to trim off the active 
  // FGD volume in order to define the fiducial FGD volume.
  //TVector3 fgd_fiducial_trim = TVector3(0.0,0.0,0.0);
  TVector3 fgd_fiducial_trim = TVector3(100.0,100.0,10.0);

  // These are the dimensions of the FGD1 active volume.
  TVector3 fFGD1ActiveMin = TVector3(-931.6, -906.6, 170.229);
  TVector3 fFGD1ActiveMax = TVector3(931.6, 956.6, 490.499);

  // These are the dimensions of the FGD1 fiducial volume.
  TVector3 fFGD1FiducialMin = fFGD1ActiveMin + fgd_fiducial_trim;
  TVector3 fFGD1FiducialMax = fFGD1ActiveMax - fgd_fiducial_trim;;

  // These are the dimensions of the FGD2 active volume.
  TVector3 fFGD2ActiveMin = TVector3(-931.6, -906.6, 1528.6);
  TVector3 fFGD2ActiveMax = TVector3(931.6, 956.6, 1850.2);

  // These are the dimensions of the FGD2 fiducial volume.
  TVector3 fFGD2FiducialMin = fFGD2ActiveMin + fgd_fiducial_trim;
  TVector3 fFGD2FiducialMax = fFGD2ActiveMax - fgd_fiducial_trim;;


  ////new purity and efficiency
  double NSigAna[10];
  double NQEAna[10];
  double NBackAna[10];
  int AnaCut=0;

  
  bool AnaSignal = false;
  bool QESignal = false;

  for(int k=0;k<10;k++)
    {
      NQEAna[k]=0;
      NSigAna[k]=0;
      NBackAna[k]=0;
    }

  int NegativeTrack=0;
  int PositiveTrack=0;
  int UndefinedTrack=0;
  

  

  int cutsel=10;
  
  
  int Nev=0;
  
  // -------------------------------------------------------------------------------------
  // Create histograms.

  TH1F *ChargeCut=new TH1F("ChargeCut","Charge of the selected track",20,-2,2);

  TH1F *TotalTracks=new TH1F("TotalTracks","Number of reconstructed tracks",10,0,10);

  TH1F *TracksTPC1=new TH1F("TracksTPC1","Number of tracks in TPC 1",10,0,10);
  TH1F *TracksTPC2=new TH1F("TracksTPC2","Number of tracks in TPC 2",10,0,10);
  TH1F *TracksTPC3=new TH1F("TracksTPC3","Number of tracks in TPC 3",10,0,10);
  TH1F *TrackSign=new TH1F("TrackSign","Number of signal tracks in TPC",10,0,10);
  TH1F *TrackBack=new TH1F("TrackBack","Number of background tracks in TPC",10,0,10);


  TH1F *Charge_1track=new TH1F("Charge_1track","Charge of the 1 track",10,-5,5);


  TH1F *LeptonMomentum=new TH1F("LeptonMomentum","Lepton momentum",50,0,5000);
  TH1F *LeptonMomentum1GeV=new TH1F("LeptonMomentum1GeV","Lepton momentum",50,0,1000);
  TH1F *LeptonResolution=new TH1F("LeptonResolution","Lepton resolution",100,-10000,10000);
  TH1F *LeptonQuality=new TH1F("LeptonQuality","Lepton quality",1000,0,1000);
  TH1F *LeptonCosAngle=new TH1F("LeptonCosAngle","Lepton cos angle",50,-1,1);
  TH1F *LeptondEdx=new TH1F("LeptondEdx","Lepton dEdx",50,0,20000);
  TH1F *PositiveMomentum=new TH1F("PositiveMomentum","Positive momentum",50,0,10000);
  TH1F *PositivedEdx=new TH1F("PositivedEdx","Positive dEdx",50,0,20000);
  TH1F *PositiveCosAngle=new TH1F("PositiveCosAngle","Positive cos angle",50,-1,1);

  TH1F *MomentumUndef=new TH1F("MomentumUndef","Momentum of the undefined tracks",50,0,5000);

  TH1F *NumVertexFGD1=new TH1F("NumVertexFGD1","Number of vertex in FGD1",20,0,20);

  TH1F *MomentumSignal=new TH1F("MomentumSignal","Momentum of the signal before cut",100,0,10000);
  TH1F *MomentumBackground=new TH1F("MomentumBackground","Momentum of the background before cut",100,0,10000);
  TH1F *MomentumNuE=new TH1F("MomentumNuE","Momentum of the selected track if nue",100,0,10000);
  TH1F *MomentumNuMu=new TH1F("MomentumNuMu","Momentum of the selected track if numu",100,0,10000);
  TH1F *EnergyNuECont=new TH1F("EnergyNuECont","Recontructed neutrino energy background",100,0,10000);
  TH1F *EnergyNuESignal=new TH1F("EnergyNuESignal","Recontructed neutrino energy signal",100,0,10000);
  TH1F *AngleNuECont=new TH1F("AngleNuECont","Recontructed neutrino angle background",50,-1,1);
  TH1F *AngleNuESignal=new TH1F("AngleNuESignal","Recontructed neutrino angle signal",50,-1,1);
  TH1F *MomentumEnergyBack=new TH1F("MomentumEnergyBack","Momentum vs Energy back",100,0,2);
  TH1F *MomentumEnergySignal=new TH1F("MomentumEnergySignal","Momentum vs Energy signal",100,0,2);


  TH1F *PullSignal=new TH1F("PullSignal","Pull variable for signal",80,-15,15);

  TH1F *PullElectrons=new TH1F("PullElectrons","Pull variable for electrons",80,-15,15);
  TH1F *PullMuons=new TH1F("PullMuons","Pull variable for muons",50,-10,10);
  TH1F *PullMuonsCont=new TH1F("PullMuonsCont","Pull variable for muons cont",50,-10,20);
  TH1F *PullPions=new TH1F("PullPions","Pull variable for pions",50,-10,10);
  TH1F *PullProtons=new TH1F("PullProtons","Pull variable for protons",50,-10,10);
  TH1F *PullPositive=new TH1F("PullPositive","Pull variable for positive particles",50,-10,10);

  TH1F *PDGContamination0=new TH1F("PDGContamination0","PDG of particle contamination 0",50,-25,25);
  TH1F *PDGContamination1=new TH1F("PDGContamination1","PDG of particle contamination 1",50,-25,25);
  TH1F *PDGContamination2=new TH1F("PDGContamination2","PDG of particle contamination 2",50,-25,25);
  TH1F *PDGContamination3=new TH1F("PDGContamination3","PDG of particle contamination 3",50,-25,25);
  TH1F *PDGContamination4=new TH1F("PDGContamination4","PDG of particle contamination 4",50,-25,25);

  
  TH1F *PDGSignal0=new TH1F("PDGSignal0","PDG of particle signal 0",50,-25,25);
  TH1F *PDGSignal1=new TH1F("PDGSignal1","PDG of particle signal 1",50,-25,25);
  TH1F *PDGSignal2=new TH1F("PDGSignal2","PDG of particle signal 2",50,-25,25);
  TH1F *PDGSignal3=new TH1F("PDGSignal3","PDG of particle signal 3",50,-25,25);
  TH1F *PDGSignal4=new TH1F("PDGSignal4","PDG of particle signal 4",50,-25,25);

  TH1F *MomentumSignal0=new TH1F("MomentumSignal0","Momentum Signal 0",50,0,5000);
  TH1F *MomentumSignal1=new TH1F("MomentumSignal1","Momentum Signal 1",50,0,5000);
  TH1F *MomentumSignal2=new TH1F("MomentumSignal2","Momentum Signal 2",50,0,5000);
  TH1F *MomentumSignal3=new TH1F("MomentumSignal3","Momentum Signal 3",50,0,5000);
  TH1F *MomentumSignal4=new TH1F("MomentumSignal4","Momentum Signal 4",50,0,5000);

  TH1F *MomentumContamination0=new TH1F("MomentumContamination0","Momentum Contamination 0",50,0,5000);
  TH1F *MomentumContamination1=new TH1F("MomentumContamination1","Momentum Contamination 1",50,0,5000);
  TH1F *MomentumContamination2=new TH1F("MomentumContamination2","Momentum Contamination 2",50,0,5000);
  TH1F *MomentumContamination3=new TH1F("MomentumContamination3","Momentum Contamination 3",50,0,5000);
  TH1F *MomentumContamination4=new TH1F("MomentumContamination4","Momentum Contamination 4",50,0,5000);

  TH1F *PDGnu=new TH1F("PDGnu","PDG of incoming neutrino",50,-25,25);  
  TH1F *PDGnuSign=new TH1F("PDGnuSign","PDG of incoming neutrino in the signal",50,-25,25);  
  TH1F *VertexDistance=new TH1F("VertexDistance","Distance from the vertex",1000,-100,100);
  TH1F *VertexDistanceSign=new TH1F("VertexDistanceSign","Distance from the vertex signal",1000,-100,100);
  TH1F *VertexDistanceCont=new TH1F("VertexDistanceCont","Distance from the vertex background",1000,-100,100);
  TH1F *DistEleSign=new TH1F("DistEleSign","Distance of the second electron signal",2000,-500,500);
  TH1F *DistEleCont=new TH1F("DistEleCont","Distance of the second electron background",2000,-500,500);

  TH1F *FirstTrackSelectionSign = new TH1F("FirstTrackSelectionSign","Track selection efficiency",100,0,10000);
  TH1F *FirstTrackSelectionBack = new TH1F("FirstTrackSelectionBack","Track selection efficiency",100,0,10000);



  TH2F *SigmaKvsMom = new TH2F("SigmaKvsMom","Sigma k/k vs Momentum",100,0,5000,100,0,5);

  TH2F *ScatterDe = new TH2F("ScatterDe","Scatter plot C_{T} vs Momentum",100,0,5000,100,0,50000);

  TH2F *ScatterDeMu = new TH2F("ScatterDeMu","Scatter plot C_{T} vs Momentum (muons)",5000,0,5000,100,0,50000);
  TH2F *ScatterDeMuMinus = new TH2F("ScatterDeMuMinus","Scatter plot C_{T} vs Momentum (muons minus)",5000,0,5000,100,0,50000);
  TH2F *ScatterDePi = new TH2F("ScatterDePi","Scatter plot C_{T} vs Momentum (pions)",5000,0,5000,100,0,50000);
  TH2F *ScatterDePro = new TH2F("ScatterDePro","Scatter plot C_{T} vs Momentum",5000,0,5000,100,0,50000);
  TH2F *ScatterDeEle = new TH2F("ScatterDeEle","Scatter plot C_{T} vs Momentum (electrons)",5000,0,5000,100,0,50000);
  TH2F *ScatterDePosi = new TH2F("ScatterDePosi","Scatter plot C_{T} vs Momentum (positrons)",5000,0,5000,100,0,50000);
  TH2F *ScatterDeKa = new TH2F("ScatterDeKa","Scatter plot C_{T} vs Momentum (kaons)",500,0,5000,100,0,50000);

  TH2F *ScatterDeMuRaw = new TH2F("ScatterDeMuRaw","Scatter plot C_{T} raw vs Momentum (muons)",500,0,5000,100,0,50000);
  TH2F *ScatterDePiRaw = new TH2F("ScatterDePiRaw","Scatter plot C_{T} raw vs Momentum (pions)",500,0,5000,100,0,50000);
  TH2F *ScatterDeProRaw = new TH2F("ScatterDeProRaw","Scatter plot C_{T} raw vs Momentum",500,0,5000,100,0,50000);
  TH2F *ScatterDeEleRaw = new TH2F("ScatterDeEleRaw","Scatter plot C_{T} raw vs Momentum (electrons)",500,0,5000,100,0,50000);
  TH2F *ScatterDePosiRaw = new TH2F("ScatterDePosiRaw","Scatter plot C_{T} raw vs Momentum (positrons)",500,0,5000,100,0,50000);
  TH2F *ScatterDeKaRaw = new TH2F("ScatterDeKaRaw","Scatter plot C_{T}raw vs Momentum (kaons)",500,0,5000,100,0,50000);

  TH2F *PullEle_Mu = new TH2F("PullEle_Mu","Pull Electron hypo vs Momentum (muons)",500,0,5000,100,-20,20);
  TH2F *PullEle_Pi = new TH2F("PullEle_Pi","Pull Electron hypo vs Momentum (pions)",500,0,5000,100,-20,20);
  TH2F *PullEle_Pro = new TH2F("PullEle_Pro","Pull Electron hypo vs Momentum",500,0,5000,100,-20,20);
  TH2F *PullEle_Ele = new TH2F("PullEle_Ele","Pull Electron hypo vs Momentum (electrons)",500,0,5000,100,-20,20);
  TH2F *PullEle_Posi = new TH2F("PullEle_Posi","Pull Electron hypo vs Momentum (positrons)",500,0,5000,100,-20,20);
  TH2F *PullEle_Ka = new TH2F("PullEle_Ka","Pull Electron hypo vs Momentum (kaons)",500,0,5000,100,-20,20);

  TH2F *PullMu_Mu = new TH2F("PullMu_Mu","Pull Muon hypo vs Momentum (muons)",500,0,5000,100,-20,20);
  TH2F *PullMu_Pi = new TH2F("PullMu_Pi","Pull Muon hypo vs Momentum (pions)",500,0,5000,100,-20,20);
  TH2F *PullMu_Pro = new TH2F("PullMu_Pro","Pull Muon hypo vs Momentum",500,0,5000,100,-20,20);
  TH2F *PullMu_Ele = new TH2F("PullMu_Ele","Pull Muon hypo vs Momentum (electrons)",500,0,5000,100,-20,20);
  TH2F *PullMu_Posi = new TH2F("PullMu_Posi","Pull Muon hypo vs Momentum (positrons)",500,0,5000,100,-20,20);
  TH2F *PullMu_Ka = new TH2F("PullMu_Ka","Pull Muon hypo vs Momentum (kaons)",500,0,5000,100,-20,20);
 
  TH2F *PullPro_Mu = new TH2F("PullPro_Mu","Pull Proton hypo vs Momentum (muons)",500,0,5000,100,-20,20);
  TH2F *PullPro_Pi = new TH2F("PullPro_Pi","Pull Proton hypo vs Momentum (pions)",500,0,5000,100,-20,20);
  TH2F *PullPro_Pro = new TH2F("PullPro_Pro","Pull Proton hypo vs Momentum",500,0,5000,100,-20,20);
  TH2F *PullPro_Ele = new TH2F("PullPro_Ele","Pull Proton hypo vs Momentum (electrons)",500,0,5000,100,-20,20);
  TH2F *PullPro_Posi = new TH2F("PullPro_Posi","Pull Proton hypo vs Momentum (positrons)",500,0,5000,100,-20,20);
  TH2F *PullPro_Ka = new TH2F("PullPro_Ka","Pull Proton hypo vs Momentum (kaons)",500,0,5000,100,-20,20);


  TH2F *ScatterDeMuTrue = new TH2F("ScatterDeMuTrue","Scatter plot C_{T} vs Momentum True (muons)",500,0,5000,100,0,50000);
  TH2F *ScatterDePiTrue = new TH2F("ScatterDePiTrue","Scatter plot C_{T} vs Momentum True (pions)",500,0,5000,100,0,50000);
  TH2F *ScatterDeProTrue = new TH2F("ScatterDeProTrue","Scatter plot C_{T} vs Momentum True (protons)",500,0,5000,100,0,50000);
  TH2F *ScatterDeEleTrue = new TH2F("ScatterDeEleTrue","Scatter plot C_{T} vs Momentum True (electrons)",500,0,5000,100,0,50000);
  TH2F *ScatterDePosiTrue = new TH2F("ScatterDePosiTrue","Scatter plot C_{T} vs Momentum True (positrons)",500,0,5000,100,0,50000);
  TH2F *ScatterDeKaTrue = new TH2F("ScatterDeKaTrue","Scatter plot C_{T} vs Momentum True (kaons)",500,0,5000,100,0,50000);

  TH2F *PullEle_MuTrue = new TH2F("PullEle_MuTrue","Pull Electron hypo vs Momentum True (muons)",500,0,5000,100,-20,20);
  TH2F *PullEle_PiTrue = new TH2F("PullEle_PiTrue","Pull Electron hypo vs Momentum True (pions)",500,0,5000,100,-20,20);
  TH2F *PullEle_ProTrue = new TH2F("PullEle_ProTrue","Pull Electron hypo vs Momentum True (protons)",500,0,5000,100,-20,20);
  TH2F *PullEle_EleTrue = new TH2F("PullEle_EleTrue","Pull Electron hypo vs Momentum True (electrons)",500,0,5000,100,-20,20);
  TH2F *PullEle_PosiTrue = new TH2F("PullEle_PosiTrue","Pull Electron hypo vs Momentum True (positrons)",500,0,5000,100,-20,20);
  TH2F *PullEle_KaTrue = new TH2F("PullEle_KaTrue","Pull Electron hypo vs Momentum True (kaons)",500,0,5000,100,-20,20);

  TH2F *PullMu_MuTrue = new TH2F("PullMu_MuTrue","Pull Muon hypo vs Momentum True (muons)",500,0,5000,100,-20,20);
  TH2F *PullMu_PiTrue = new TH2F("PullMu_PiTrue","Pull Muon hypo vs Momentum True (pions)",500,0,5000,100,-20,20);
  TH2F *PullMu_ProTrue = new TH2F("PullMu_ProTrue","Pull Muon hypo vs Momentum True (protons)",500,0,5000,100,-20,20);
  TH2F *PullMu_EleTrue = new TH2F("PullMu_EleTrue","Pull Muon hypo vs Momentum True (electrons)",500,0,5000,100,-20,20);
  TH2F *PullMu_PosiTrue = new TH2F("PullMu_PosiTrue","Pull Muon hypo vs Momentum True (positrons)",500,0,5000,100,-20,20);
  TH2F *PullMu_KaTrue = new TH2F("PullMu_KaTrue","Pull Muon hypo vs Momentum True (kaons)",500,0,5000,100,-20,20);
 
  TH2F *PullPro_MuTrue = new TH2F("PullPro_MuTrue","Pull Proton hypo vs Momentum True (muons)",500,0,5000,100,-20,20);
  TH2F *PullPro_PiTrue = new TH2F("PullPro_PiTrue","Pull Proton hypo vs Momentum True (pions)",500,0,5000,100,-20,20);
  TH2F *PullPro_ProTrue = new TH2F("PullPro_ProTrue","Pull Proton hypo vs Momentum True (protons)",500,0,5000,100,-20,20);
  TH2F *PullPro_EleTrue = new TH2F("PullPro_EleTrue","Pull Proton hypo vs Momentum True (electrons)",500,0,5000,100,-20,20);
  TH2F *PullPro_PosiTrue = new TH2F("PullPro_PosiTrue","Pull Proton hypo vs Momentum True (positrons)",500,0,5000,100,-20,20);
  TH2F *PullPro_KaTrue = new TH2F("PullPro_KaTrue","Pull Proton hypo vs Momentum True (kaons)",500,0,5000,100,-20,20);


  TH1F *PullEleHypo=new TH1F("PullEleHypo","Pull variable for electrons true if prec>0 MeV",80,-15,15);
 TH1F *PullEleHypo100MeV=new TH1F("PullEleHypo100MeV","Pull variable for electrons true if prec>50 MeV",80,-15,15);
 TH1F *PullEleHypo300MeV=new TH1F("PullEleHypo300MeV","Pull variable for electrons true if prec>100 MeV",80,-15,15);
 TH1F *PullEleHypo500MeV=new TH1F("PullEleHypo500MeV","Pull variable for electrons true if prec>200 MeV",80,-15,15);
  TH1F *PullPosiHypo=new TH1F("PullPosiHypo","Pull variable for positrons true if prec>0 MeV",80,-15,15);
 TH1F *PullPosiHypo100MeV=new TH1F("PullPosiHypo100MeV","Pull variable for positrons true if prec>50 MeV",80,-15,15);
 TH1F *PullPosiHypo200MeV=new TH1F("PullPosiHypo200MeV","Pull variable for positrons true if prec>100 MeV",80,-15,15);
 TH1F *PullPosiHypo300MeV=new TH1F("PullPosiHypo300MeV","Pull variable for positrons true if prec>200 MeV",80,-15,15);

 TH1F *PullEleHypoTrue=new TH1F("PullEleHypoTrue","Pull variable for electrons true if ptrue>0 MeV",80,-15,15);
 TH1F *PullEleHypoTrue100MeV=new TH1F("PullEleHypoTrue100MeV","Pull variable for electrons true if ptrue>50 MeV",80,-15,15);
 TH1F *PullEleHypoTrue200MeV=new TH1F("PullEleHypoTrue200MeV","Pull variable for electrons true if ptrue>100 MeV",80,-15,15);
 TH1F *PullEleHypoTrue300MeV=new TH1F("PullEleHypoTrue300MeV","Pull variable for electrons true if ptrue>200 MeV",80,-15,15);
  TH1F *PullPosiHypoTrue=new TH1F("PullPosiHypoTrue","Pull variable for positrons true if ptrue>0 MeV",80,-15,15);
 TH1F *PullPosiHypoTrue100MeV=new TH1F("PullPosiHypoTrue100MeV","Pull variable for positrons true if ptrue>50 MeV",80,-15,15);
 TH1F *PullPosiHypoTrue200MeV=new TH1F("PullPosiHypoTrue200MeV","Pull variable for positrons true if ptrue>100 MeV",80,-15,15);
 TH1F *PullPosiHypoTrue300MeV=new TH1F("PullPosiHypoTrue300MeV","Pull variable for positrons true if ptrue>200 MeV",80,-15,15);

  TH1F *PullMuHypo300=new TH1F("PullMuHypo300","Pull variable for muons if prec < 300",80,-15,15);
  TH1F *PullMuHypo500=new TH1F("PullMuHypo500","Pull variable for muons if prec < 500 && > 300",80,-15,15);
  TH1F *PullMuHypo700=new TH1F("PullMuHypo700","Pull variable for muons if prec < 700 && > 500",80,-15,15);
  TH1F *PullMuHypo900=new TH1F("PullMuHypo900","Pull variable for muons if prec < 900 && > 700",80,-15,15);
  TH1F *PullMuHypo1300=new TH1F("PullMuHypo1300","Pull variable for muons if prec < 1300 && > 900",80,-15,15);
  TH1F *PullMuHypo2000=new TH1F("PullMuHypo2000","Pull variable for muons if prec < 2000 &&  > 1300",80,-15,15);
  TH1F *PullMuHypo3000=new TH1F("PullMuHypo3000","Pull variable for muons if prec < 3000 &&  > 2000",80,-15,15);
  TH1F *PullMuHypo5000=new TH1F("PullMuHypo5000","Pull variable for muons if prec > 2000",80,-15,15);

  TH1F *CTMu500 = new TH1F("CTMu500","C_{T} muons if prec >300 && < 700",100,0,20000);
  TH1F *CTMu5000 = new TH1F("CTMu5000","C_{T} muons if prec >300 && > 3000",100,0,20000);


  TH1F *PullMuHypo=new TH1F("PullMuHypo","Pull variable for muons true",80,-15,15);
  TH1F *PullPiHypo=new TH1F("PullPiHypo","Pull variable for pions true",80,-15,15);
  TH1F *PullProHypo=new TH1F("PullProHypo","Pull variable for protons true",80,-15,15);
  TH1F *PullKaHypo=new TH1F("PullKaHypo","Pull variable for kaons true",80,-15,15);

  TH1F *NelikeSignal=new TH1F("NelikeSignal","N elike tracks Signal",10,0,10);
  TH1F *NelikeBack=new TH1F("NelikeBack","N elike tracks Background",10,0,10);
  TH1F *NTotTrackSignal=new TH1F("NTotTrackSignal","N tot tracks Signal",10,0,10);
  TH1F *NTotTrackBack=new TH1F("NTotTrackBack","N tot tracks Background",10,0,10);
  TH1F *EelikeSignal=new TH1F("EelikeSignal","Energy elike tracks Signal",50,0,2000);
  TH1F *EelikeBack=new TH1F("EelikeBack","Energy elike tracks Background",50,0,2000);

  TH1F *ECALPidSignal = new TH1F("ECALPidSignal","ECAL PID nue",100,-2,2);
  TH1F *ECALPidBack = new TH1F("ECALPidBack","ECAL PID numu",100,-2,2);
  TH1F *ECALPidMatchSignal = new TH1F("ECALPidMatchSignal","ECAL PID match nue",100,-2,2);
  TH1F *ECALPidMatchBack = new TH1F("ECALPidMatchBack","ECAL PID match numu",100,-2,2);
  TH1F *ECALPidShowerSignal = new TH1F("ECALPidShowerSignal","ECAL PID shower nue",100,-2,2);
  TH1F *ECALPidShowerBack = new TH1F("ECALPidShowerBack","ECAL PID shower numu",100,-2,2);
  TH1F *ECALPidShowerMatchSignal = new TH1F("ECALPidShowerMatchSignal","ECAL PID shower match nue",100,-2,2);
  TH1F *ECALPidShowerMatchBack = new TH1F("ECALPidShowerMatchBack","ECAL PID shower match numu",100,-2,2);


  TH1F *EvsPSignal = new TH1F("EvsPSignal","E vs P signal",500,0,5);
  TH1F *EvsPBack = new TH1F("EvsPBack","E vs P back",500,0,5);
  TH1F *ECALRecEnergySignal = new TH1F("ECALRecEnergySignal","ECAL PID energy",100,0,1500);
  TH1F *ECALRecEnergyBack = new TH1F("ECALRecEnergyBack","ECAL PID energy back",100,0,1500);
  TH1F *ECALRecEnergyNoMSignal = new TH1F("ECALRecEnergyNoMSignal","ECAL PID energy no sel",100,0,1500);
  TH1F *ECALRecEnergyNoMBack = new TH1F("ECALRecEnergyNoMBack","ECAL PID energy back no sel",100,0,1500);

  TH1F *MomResMuons = new TH1F("MomResMuons","Momentum Resolution for muons",200,-1000,1000);
  TH1F *MomResPions = new TH1F("MomResPions","Momentum Resolution for pions",200,-1000,1000);
  TH1F *MomResProtons = new TH1F("MomResProtons","Momentum Resolution for electrons",200,-1000,1000);
  TH1F *MomResElectrons = new TH1F("MomResElectrons","Momentum Resolution for protons",200,-1000,1000);

 TH1F *CurvatureError = new TH1F("CurvatureError","Error on the curvature",100,0,1);

 TH1F *DiffAngleSignal = new TH1F("DiffAngleSignal","Cos angle difference signal",100,-2,2);
 TH1F *DiffAngleBack = new TH1F("DiffAngleBack","Cos angle diff back",100,-2,2);


 TH1F *MomEleSel = new TH1F("MomEleSel","momentm of selected electrons",100,0,10000);
 TH1F *MomEleMiss = new TH1F("MomEleMiss","momentm of missed electrons",100,0,10000);

 TH1F *NClSel = new TH1F("NClSel","number of cluster for ele sel",75,0,75);
 TH1F *NClMiss = new TH1F("NClMiss","number of cluster for ele miss",75,0,75);

  // _____________________________________________________________________________________
  // Load data files   
  //TString fileName("Claudio_input_OARECON.txt");
  //TString fileName("MDC0_input_All.txt");
 TString fileName(FileName);


  ifstream inputFile(fileName.Data(), ios::in);
  

  int geniefile=1;
  int neutfile=0;

  if (!inputFile) {
    cerr << "Cannot open input file '" << fileName.Data() << "'. Exit!" << endl;
    return;
  }
  
  string inputString;

  while (inputFile >> inputString) {
    std::cout << "Adding file: " << inputString << std::endl;
    mcTruthVertices.AddFile(inputString.c_str());
    mcTruthTrajectories.AddFile(inputString.c_str());
    reconTracker.AddFile(inputString.c_str());
    reconECAL.AddFile(inputString.c_str());
  }

  // _____________________________________________________________________________________
  // Prepare to read trees.

  // Set the branches for the truth information.
  Int_t NVtxFGD1; 
  Int_t NVtxFGD2;
  TClonesArray *VtxFGD1 = new TClonesArray("ND::TTruthVerticesModule::TTruthVertex", 20);
  TClonesArray *VtxFGD2 = new TClonesArray("ND::TTruthVerticesModule::TTruthVertex", 20);

  mcTruthVertices.SetBranchAddress("NVtxFGD1", &NVtxFGD1);
  mcTruthVertices.SetBranchAddress("NVtxFGD2", &NVtxFGD2);
  mcTruthVertices.SetBranchAddress("VtxFGD1", &VtxFGD1);
  mcTruthVertices.SetBranchAddress("VtxFGD2", &VtxFGD2);

  mcTruthVertices.Print("toponly");

  TRefArray *AllTruthTrajectories = new TRefArray();
  mcTruthTrajectories.SetBranchAddress("AllTrajectories",AllTruthTrajectories);

  mcTruthTrajectories.Print("toponly");

  //AllTrajectories(new TRefArray()

  // Set the branches for the tracker recon information.
  Int_t NVertices;
  Int_t NTracks;
  Int_t NDelayedClusters;
  Int_t EventID;
  TClonesArray *Vertices = new TClonesArray("ND::TTrackerReconModule::TTrackerVertex", 20);
  TClonesArray *Tracks = new TClonesArray("ND::TTrackerReconModule::TTrackerTrack", 20);
  TClonesArray *DelayedClusters = new TClonesArray("ND::TTrackerReconModule::TFgdCluster", 20);

  reconTracker.SetBranchAddress("EventID", &EventID);
  reconTracker.SetBranchAddress("NVertices", &NVertices);
  reconTracker.SetBranchAddress("NTracks", &NTracks);
  reconTracker.SetBranchAddress("NDelayedClusters", &NDelayedClusters);
  reconTracker.SetBranchAddress("Vertices", &Vertices);
  reconTracker.SetBranchAddress("Tracks", &Tracks);
  reconTracker.SetBranchAddress("DelayedClusters", &DelayedClusters);

  reconTracker.Print("toponly");
  
  Int_t NReconObject;
  TClonesArray *ReconObject = new TClonesArray("ND::TTrackerECALReconModule::TECALReconObject");

  reconECAL.SetBranchAddress("NReconObject",&NReconObject);
  reconECAL.SetBranchAddress("ReconObject",&ReconObject);
  
  reconECAL.Print("toponly");

  // __________________________________________________________________________
  // Loop over all events.  
  int entries = mcTruthVertices.GetEntries();

  std::cout << "ent: " << entries << std::endl;
  int ntotal = 0, nfid = 0;

  int total_ccqe_fiducial = 0;
  int total_selected[4] = {0,0,0,0};
  int total_selected_ccqe_fiducial[4] = {0,0,0,0};

  TStopwatch timer;
  timer.Start();

  
  Int_t nuint=0;  

  Int_t numuTot=0;
  Int_t numuCCint=0;
  Int_t numuCCqeint=0;
  Int_t antinumuTot=0;
  Int_t antinumuCCint=0;
  Int_t antinumuCCqeint=0;

  Int_t CCQE=0;

  Int_t nueTot=0;
  Int_t nueCCint=0;
  Int_t nueCCqeint=0;
  Int_t antinueTot=0;
  Int_t antinueCCint=0;
  Int_t antinueCCqeint=0;
  
  Int_t PDGneutrino=0;

  Int_t Charged=0;
  Int_t Neutral=0;
  Int_t Other=0;

  int count=0;
  int nue=0;
  Double_t badPID[10000];
  Double_t NueEvent[10000];

  for(int i=0;i<10000;i++)
    {
      badPID[i]=0;
      NueEvent[i]=0;
    }

  
  bool elehere=false;
  int elesel=0;
  int elepassed=0;
  int elemiss=0;
  int posisel=0;
  int allsel=0;
  int muonsel=0;
  int muonpossel=0;
  int pionsel=0;
  int pionnegsel=0;
  int protonsel=0;
  int kaonsel=0;
  

  //entries=5000;

  for(int i = 0; i < entries; i++){

    int Cut=0;

    AnaSignal = false;
    QESignal = false;

    if(verboseclean)
      {
	if(i!=0)
	  {
	    std::cout<<"End of the previous event"<<std::endl;
	    gets(s);
	  }
	std::cout<<"START THE EVENT "<<i<<std::endl;
      }

    //if(entries!=169)continue;

    mcTruthVertices.GetEntry(i);

    
    reconTracker.GetEntry(i);


 

    reconECAL.GetEntry(i);


 

    if(i%500 == 0)
      std::cout <<  "Events Processed " << i << std::endl;



    //std::cout<<i<<std::endl;



    

    
    // ------------------------------------------------------------------------
    // Check vertex truth information.

    ND::TTruthVerticesModule::TTruthVertex *true_fgd_vertex;

    
 

    bool trueFiducialFGD1 = false, trueFiducialFGD2 = false;
    bool VertexConn = false;
    bool flagpi0 = false;

    bool trackFGD1=false;
    bool trackFGD2=false;
    bool ECALtracklike = false;



    if(NVtxFGD1 >0){
      true_fgd_vertex = (ND::TTruthVerticesModule::TTruthVertex*)VtxFGD1->UncheckedAt(0);
      trueFiducialFGD1 = true;
    }else if (NVtxFGD2 >0){
      true_fgd_vertex = (ND::TTruthVerticesModule::TTruthVertex*)VtxFGD2->UncheckedAt(0);
      trueFiducialFGD2 = true;
    }else{
      //std::cerr << "There should be at least one vertex in one FGD.  Skipping event."<<std::endl;
      //continue;
    }
 

    string input=true_fgd_vertex->ReactionCode;

    if(input.find("nu:14")!=std::string::npos)PDGneutrino=14;
    else if(input.find("nu:-14")!=std::string::npos)PDGneutrino=-14;
    else if(input.find("nu:12")!=std::string::npos)PDGneutrino=12;
    else if(input.find("nu:-12")!=std::string::npos)PDGneutrino=-12;
    PDGnu->Fill(PDGneutrino);

    if(true_fgd_vertex)
      {
	Bool_t CCinteraction = false;
	Bool_t CCqeinteraction = false;
	
	nuint++;


	if(geniefile==1)
	  {

	    std::string reactionCode(true_fgd_vertex->ReactionCode);
	    //CCinteraction = reactionCode.find("Weak[CC]"!=std::string::npos);
	    //CCqeinteraction = reactionCode.find("Weak[CC],QES"!=std::string::npos);
	  
	    if(verboseclean)std::cout<<"reaction "<<reactionCode<<std::endl;


	    if(reactionCode.find("nu:14")!=std::string::npos)
	      {
		numuTot++;
		if(reactionCode.find("Weak[CC]")!=std::string::npos)
		  numuCCint++;
		if(reactionCode.find("Weak[CC],QES")!=std::string::npos)
		  numuCCqeint++;
	      }
	    else if(reactionCode.find("nu:-14")!=std::string::npos)
	      {
		antinumuTot++;
		if(reactionCode.find("Weak[CC]")!=std::string::npos)
		  antinumuCCint++;
		if(reactionCode.find("Weak[CC],QES")!=std::string::npos)
		  antinumuCCqeint++;
	      }
	    else if(reactionCode.find("nu:12")!=std::string::npos)
	      {
		nueTot++;
		if(reactionCode.find("Weak[CC]")!=std::string::npos)
		  {
		    nueCCint++;
		    AnaSignal = true;
		    NueEvent[nue]=i;
		    nue++;
		  }

		if(reactionCode.find("Weak[CC],QES")!=std::string::npos)
		  {
		    nueCCqeint++;
		    QESignal = true;
		  }
	      }
	    if(reactionCode.find("nu:-12")!=std::string::npos)
	      {
		antinueTot++;
		if(reactionCode.find("Weak[CC]")!=std::string::npos)
		  antinueCCint++;
		if(reactionCode.find("Weak[CC],QES")!=std::string::npos)
		  antinueCCqeint++;
	      }


	  }

      }


    // Apply event selection criterion.
    
    // reconstructed initial vertex 
    TLorentzVector recon_vertex_pos;
    TLorentzVector recon_front_pos;
    TLorentzVector recon_other_front_pos;
    // reconstructed muon track
    ND::TTrackerReconModule::TTrackerTrack *leptonTrack = 0;
    ND::TTrackerReconModule::TTrackerTrack *leptonTrack2 = 0;
    ND::TTrackerReconModule::TTrackerTrack *second_track = 0;

    ND::TTrackerReconModule::TTrackerTrack *TrackFirstTPC = 0;
    ND::TTrackerReconModule::TTrackerTrack *TrackSecondTPC = 0;
	     
    // Reset variables
   

    int total_tracks = NTracks; // Total number of tracks.


    TotalTracks->Fill(NTracks);



    if(verboseclean)std::cout<<"Number of TPC tracks "<<NTracks<<std::endl;

    

    int total_tpc[3] = {0,0,0};    // Total number of TPC tracks.

    double tmpmom1=0.;
    double tmpmom2=0.;
    bool VertexCon=false;
    


    int TPC1=0;
    bool flagTrack1=true;

    int TPC2=0;
    bool flagTrack2=true;

    double momall1[500];
    double momall2[500];


    bool AlreadyCount0 = false;
    int TotTr0=0;
    double othermom0[500];
    
    int TotTr1=0;
    double othermom1[500];
    bool AlreadyCount2 = false;
    int TotTr2=0;
    double othermom2[500];

    bool first1 = true;
    bool first2 = true;

    //std::cout<<"Ntracks "<<NTracks<<std::endl;



    for(unsigned int j = 0; j < NTracks; j++){


      bool AlreadyCount1 = false;
      bool AlreadyCount2 = false;
       
      ND::TTrackerReconModule::TTrackerTrack *track 
	= (ND::TTrackerReconModule::TTrackerTrack*)(*Tracks)[j];
      bool hasTPC[3] = false;

      if(DoPIDPlots)
	{
	  for(int tt = 0; tt < 3; tt++){
	    
	    if(track->tpc_charge[tt] != 0)            
	      {
		
		double tracktruemom=track->True_MomTPC[tt];
		double momres = (track->tpc_momentum[tt]-tracktruemom);
		{
		  ScatterDe->Fill(track->tpc_momentum[tt],track->tpcPID_Ccorr[tt]);
		  if(track->True_TrajPDG[tt]==11)
		    {
		      MomResElectrons->Fill(momres);
		      
		      ScatterDeEle->Fill(track->tpc_momentum[tt],track->tpcPID_Ccorr[tt]);
		      ScatterDeEleRaw->Fill(track->tpc_momentum[tt],track->tpcPID_Craw[tt]);
		      
		      PullEle_Ele->Fill(track->tpc_momentum[tt],track->tpcPID_PullEle[tt]);
		      PullMu_Ele->Fill(track->tpc_momentum[tt],track->tpcPID_PullMuon[tt]);
		      PullPro_Ele->Fill(track->tpc_momentum[tt],track->tpcPID_PullProton[tt]);
		      PullEleHypo->Fill(track->tpcPID_PullEle[tt]);
		      PullEleHypoTrue->Fill(track->tpcPID_PullEle[tt]);
		      
		      if(track->tpc_momentum[tt]>100)PullEleHypo100MeV->Fill(track->tpcPID_PullEle[tt]);
		      if(track->tpc_momentum[tt]>300)PullEleHypo300MeV->Fill(track->tpcPID_PullEle[tt]);
		      if(track->tpc_momentum[tt]>500)PullEleHypo500MeV->Fill(track->tpcPID_PullEle[tt]);
		      
		      if(tracktruemom>100)PullEleHypoTrue100MeV->Fill(track->tpcPID_PullEle[tt]);
		      if(tracktruemom>200)PullEleHypoTrue200MeV->Fill(track->tpcPID_PullEle[tt]);
		      if(tracktruemom>300)PullEleHypoTrue300MeV->Fill(track->tpcPID_PullEle[tt]);
		  
		      
		      ////////////true momentum 
		      ScatterDeEleTrue->Fill(tracktruemom,track->tpcPID_Ccorr[tt]);
		      PullEle_EleTrue->Fill(tracktruemom,track->tpcPID_PullEle[tt]);
		      PullMu_EleTrue->Fill(tracktruemom,track->tpcPID_PullMuon[tt]);
		      PullPro_EleTrue->Fill(tracktruemom,track->tpcPID_PullProton[tt]);
		    }
		  else if(track->True_TrajPDG[tt]==-11)
		    {
		      ScatterDePosi->Fill(track->tpc_momentum[tt],track->tpcPID_Ccorr[tt]);
		      ScatterDePosiRaw->Fill(track->tpc_momentum[tt],track->tpcPID_Craw[tt]);
		      PullEle_Posi->Fill(track->tpc_momentum[tt],track->tpcPID_PullEle[tt]);
		      PullMu_Posi->Fill(track->tpc_momentum[tt],track->tpcPID_PullMuon[tt]);
		      PullPro_Posi->Fill(track->tpc_momentum[tt],track->tpcPID_PullProton[tt]);
		      PullPosiHypo->Fill(track->tpcPID_PullEle[tt]);
		      
		      if(track->tpc_momentum[tt]>100)PullPosiHypo100MeV->Fill(track->tpcPID_PullEle[tt]);
		      if(track->tpc_momentum[tt]>200)PullPosiHypo200MeV->Fill(track->tpcPID_PullEle[tt]);
		      if(track->tpc_momentum[tt]>300)PullPosiHypo300MeV->Fill(track->tpcPID_PullEle[tt]);
		      
		      if(tracktruemom>100)PullPosiHypoTrue100MeV->Fill(track->tpcPID_PullEle[tt]);
		      if(tracktruemom>200)PullPosiHypoTrue200MeV->Fill(track->tpcPID_PullEle[tt]);
		      if(tracktruemom>300)PullPosiHypoTrue300MeV->Fill(track->tpcPID_PullEle[tt]);
		      
		      
		      ////////////true momentum 
		      ScatterDePosiTrue->Fill(tracktruemom,track->tpcPID_Ccorr[tt]);
		      PullEle_PosiTrue->Fill(tracktruemom,track->tpcPID_PullEle[tt]);
		      PullMu_PosiTrue->Fill(tracktruemom,track->tpcPID_PullMuon[tt]);
		      PullPro_PosiTrue->Fill(tracktruemom,track->tpcPID_PullProton[tt]);
		      
		      
		    }
		  else if(fabs(track->True_TrajPDG[tt])==13)
		    {
		      
		      SigmaKvsMom->Fill(track->tpc_curv_error[tt]/track->tpc_curv[tt],track->tpc_momentum[tt]);
		      
		      MomResMuons->Fill(momres);
		      ScatterDeMu->Fill(track->tpc_momentum[tt],track->tpcPID_Ccorr[tt]);
		      
		      ScatterDeMuRaw->Fill(track->tpc_momentum[tt],track->tpcPID_Craw[tt]);
		      PullEle_Mu->Fill(track->tpc_momentum[tt],track->tpcPID_PullEle[tt]);
		      PullMu_Mu->Fill(track->tpc_momentum[tt],track->tpcPID_PullMuon[tt]);
		      PullPro_Mu->Fill(track->tpc_momentum[tt],track->tpcPID_PullProton[tt]);
		      PullMuHypo->Fill(track->tpcPID_PullMuon[tt]);
		      
		      ////////////true momentum 
		      ScatterDeMuTrue->Fill(tracktruemom,track->tpcPID_Ccorr[tt]);
		      PullEle_MuTrue->Fill(tracktruemom,track->tpcPID_PullEle[tt]);
		      PullMu_MuTrue->Fill(tracktruemom,track->tpcPID_PullMuon[tt]);
		      PullPro_MuTrue->Fill(tracktruemom,track->tpcPID_PullProton[tt]);
		      
		      
		      ////pull vs mom
		      if(track->tpc_momentum[tt]<300)PullMuHypo300->Fill(track->tpcPID_PullMuon[tt]);
		      else if(track->tpc_momentum[tt]<500)PullMuHypo500->Fill(track->tpcPID_PullMuon[tt]);
		      else if(track->tpc_momentum[tt]<700)PullMuHypo700->Fill(track->tpcPID_PullMuon[tt]);
		      else if(track->tpc_momentum[tt]<900)PullMuHypo900->Fill(track->tpcPID_PullMuon[tt]);
		      else if(track->tpc_momentum[tt]<1300)PullMuHypo1300->Fill(track->tpcPID_PullMuon[tt]);
		      else if(track->tpc_momentum[tt]<2000)PullMuHypo2000->Fill(track->tpcPID_PullMuon[tt]);
		      else if(track->tpc_momentum[tt]<3000)PullMuHypo3000->Fill(track->tpcPID_PullMuon[tt]);
		      else PullMuHypo5000->Fill(track->tpcPID_PullMuon[tt]);
		      
		      
		      if(track->tpc_momentum[tt]>500 && track->tpc_momentum[tt]<700)CTMu500->Fill(track->tpcPID_Ccorr[tt]);
		      else if(track->tpc_momentum[tt]>3000 && track->tpc_momentum[tt]<5000)CTMu5000->Fill(track->tpcPID_Ccorr[tt]);
		      
		      
		    }
		  else if(fabs(track->True_TrajPDG[tt])==2212)
		    {
		      MomResProtons->Fill(momres);
		      ScatterDePro->Fill(track->tpc_momentum[tt],track->tpcPID_Ccorr[tt]);
		      ScatterDeProRaw->Fill(track->tpc_momentum[tt],track->tpcPID_Craw[tt]);
		      PullEle_Pro->Fill(track->tpc_momentum[tt],track->tpcPID_PullEle[tt]);
		      PullMu_Pro->Fill(track->tpc_momentum[tt],track->tpcPID_PullMuon[tt]);
		      PullPro_Pro->Fill(track->tpc_momentum[tt],track->tpcPID_PullProton[tt]);
		      PullProHypo->Fill(track->tpcPID_PullProton[tt]);
		      
		      ////////////true momentum 
		      ScatterDeProTrue->Fill(tracktruemom,track->tpcPID_Ccorr[tt]);
		      PullEle_ProTrue->Fill(tracktruemom,track->tpcPID_PullEle[tt]);
		      PullMu_ProTrue->Fill(tracktruemom,track->tpcPID_PullMuon[tt]);
		      PullPro_ProTrue->Fill(tracktruemom,track->tpcPID_PullProton[tt]);
		      
		      
		    }	
		  else if(fabs(track->True_TrajPDG[tt])==211)
		    {
		      
		      MomResPions->Fill(momres);
		      
		      ScatterDePi->Fill(track->tpc_momentum[tt],track->tpcPID_Ccorr[tt]);
		      ScatterDePiRaw->Fill(track->tpc_momentum[tt],track->tpcPID_Craw[tt]);
		      PullEle_Pi->Fill(track->tpc_momentum[tt],track->tpcPID_PullEle[tt]);
		      PullMu_Pi->Fill(track->tpc_momentum[tt],track->tpcPID_PullMuon[tt]);
		      PullPro_Pi->Fill(track->tpc_momentum[tt],track->tpcPID_PullProton[tt]);
		      PullPiHypo->Fill(track->tpcPID_PullPion[tt]);
		      
		      ////////////true momentum 
		      ScatterDePiTrue->Fill(tracktruemom,track->tpcPID_Ccorr[tt]);
		      PullEle_PiTrue->Fill(tracktruemom,track->tpcPID_PullEle[tt]);
		      PullMu_PiTrue->Fill(tracktruemom,track->tpcPID_PullMuon[tt]);
		      PullPro_PiTrue->Fill(tracktruemom,track->tpcPID_PullProton[tt]);
		      
		    }
		  else if(fabs(track->True_TrajPDG[tt])==321)
		    {
		      ScatterDeKa->Fill(track->tpc_momentum[tt],track->tpcPID_Ccorr[tt]);
		      ScatterDeKaRaw->Fill(track->tpc_momentum[tt],track->tpcPID_Craw[tt]);
		      PullEle_Ka->Fill(track->tpc_momentum[tt],track->tpcPID_PullEle[tt]);
		      PullMu_Ka->Fill(track->tpc_momentum[tt],track->tpcPID_PullMuon[tt]);
		      PullPro_Ka->Fill(track->tpc_momentum[tt],track->tpcPID_PullProton[tt]);
		      PullKaHypo->Fill(track->tpcPID_PullKaon[tt]);
		      
		      ////////////true momentum 
		      ScatterDeKaTrue->Fill(tracktruemom,track->tpcPID_Ccorr[tt]);
		      PullEle_KaTrue->Fill(tracktruemom,track->tpcPID_PullEle[tt]);
		      PullMu_KaTrue->Fill(tracktruemom,track->tpcPID_PullMuon[tt]);
		      PullPro_KaTrue->Fill(tracktruemom,track->tpcPID_PullProton[tt]);
		      
		    }
		}
	      }
	  }
	} ///end of loop on DoPids

    


      if(track->tpc_momentum[1]!=0)
	{
	  if(first1)
		{
		  if(AllNeg)
		    {
		      if(track->tpc_charge[1]<0)
			{
			  leptonTrack=track;
			  tmpmom1=track->tpc_momentum[1];
			  first1 = false;
			}
		    }
		  else
		    {
		      leptonTrack=track;
		      tmpmom1=track->tpc_momentum[1];
		      first1 = false;
		    }
		}
	  othermom1[TotTr1]=track->tpc_momentum[1];
	  TotTr1++;
	      //total_tpc[1]++;
	}
	
     
      
      //std::cout<<"total tracks "<<total_tpc[1]<<" "<<total_tpc[2]<<std::endl;
      //std::cout<<"momentum "<<track->tpc_momentum[1]<<" "<<track->tpc_momentum[2]<<std::endl;

      if(TotTr1>0)
	{
	  for(int k=0;k<TotTr1-1;k++)
	    {
	      if(track->tpc_momentum[1]==othermom1[k])
		AlreadyCount1=true;

	      //std::cout<<"compare momentum"<<std::endl;
	      
	      
	      if(fabs(track->tpc_momentum[1])>fabs(tmpmom1))
		{
		  if(AllNeg)
		    {
		      if(track->tpc_charge[1]<0)
			{
			  //std::cout<<"in the loop"<<std::endl;
			  leptonTrack=track;
			  tmpmom1=track->tpc_momentum[1];
			}
		    }
		  else
		    {   
		      leptonTrack=track;
		      tmpmom1=track->tpc_momentum[1];
		    }
		}
	      
	    	      
	      //std::cout<<"othermom track "<<k<<" mom "<<othermom1[k]<<std::endl;
	    }
	}
    

      if(track->tpc_momentum[1]!=0 && !AlreadyCount1)
	total_tpc[1]++;


      if(track->tpc_momentum[2]!=0)
	{

	  if(first2)
	    {
	      if(AllNeg)
		{
		  if(track->tpc_charge[2]<0)
		    {
		      leptonTrack2=track;
		      tmpmom2=track->tpc_momentum[2];
		      first2 = false;
		    }
		}
	      else
		{
		  leptonTrack2=track;
		  tmpmom2=track->tpc_momentum[2];
		  first2 = false;
		}
	    }
	  
	  othermom2[TotTr2]=track->tpc_momentum[2];
	  TotTr2++;
	}

      if(TotTr2>0)
	{
	  for(int k=0;k<TotTr2-1;k++)
	    {
	      if(track->tpc_momentum[2]==othermom2[k])
		AlreadyCount2=true;
	    	    
	      if(fabs(track->tpc_momentum[2])>fabs(tmpmom2))
		{
		  if(AllNeg)
		    {
		      if(track->tpc_charge[2]<0)
			{
			  leptonTrack2=track;
			  tmpmom2=track->tpc_momentum[2];
			}
		    }
		  else
		    {
		      leptonTrack2=track;
		      tmpmom2=track->tpc_momentum[2];
		    }
		}
	      
	      //std::cout<<"othermom track "<<k<<" mom "<<othermom2[k]<<std::endl;
	    }
	}
 

      if(track->tpc_momentum[2]!=0 && !AlreadyCount2)
	total_tpc[2]++;

    }


  

    if(verboseclean)std::cout<<"End of the loop on the tracks"<<std::endl;

    int trueId=0;

    if(leptonTrack)
      {
	trackFGD1=true;
	trueId = leptonTrack->True_TrajPDG[1];     
// 	if(trueId==11)elesel++;
// 	else if(trueId==-11)posisel++;
// 	else if(trueId==13)muonsel++;
// 	else if(trueId==-13)muonpossel++;
// 	else if(trueId==211)pionsel++;
// 	else if(trueId==-211)pionnegsel++;
// 	else if(trueId==2212)protonsel++;
// 	else if(trueId==321)kaonsel++;
// 	allsel++;

	if(trueId==11)FirstTrackSelectionSign->Fill(leptonTrack->tpc_momentum[1]);
	else FirstTrackSelectionBack->Fill(leptonTrack->tpc_momentum[1]);

	//std::cout<<i<<" TPC1 "<<trueId<<std::endl;
      }
	
    else if(!leptonTrack && leptonTrack2)
      {
	trackFGD2=true;
	trueId = leptonTrack2->True_TrajPDG[2];     
// 	if(trueId==11)elesel++;
// 	else if(trueId==-11)posisel++;
// 	else if(trueId==13)muonsel++;
// 	else if(trueId==-13)muonpossel++;
// 	else if(trueId==211)pionsel++;
// 	else if(trueId==-211)pionnegsel++;
// 	else if(trueId==2212)protonsel++;
// 	else if(trueId==321)kaonsel++;
// 	allsel++;
	//std::cout<<i<<" TPC2 "<<trueId<<std::endl;

	if(trueId==11)FirstTrackSelectionSign->Fill(leptonTrack2->tpc_momentum[2]);
	else FirstTrackSelectionBack->Fill(leptonTrack2->tpc_momentum[2]);


      }	

    else continue;


    if(trueId!=11)
      {
	for(unsigned int j = 0; j < NTracks; j++){
	  ND::TTrackerReconModule::TTrackerTrack *othertrack 
	    = (ND::TTrackerReconModule::TTrackerTrack*)(*Tracks)[j];
  
	  if(trackFGD1)
	    {
	      //std::cout<<othertrack->True_TrajPDG[1]<<" "<<othertrack->tpc_momentum[1]<<std::endl;
	      if(othertrack->True_TrajPDG[1]==11)elehere = true;
	    }


	  if(trackFGD2)
	    {
	      //std::cout<<othertrack->True_TrajPDG[2]<<" "<<othertrack->tpc_momentum[2]<<std::endl;
	      if(othertrack->True_TrajPDG[2]==11)elehere = true;
	    }
	}
      }

    if(elehere)
      {
	//if(trackFGD1)std::cout<<"TPC 1 "<<std::endl;
	//if(trackFGD2)std::cout<<"TPC 2 "<<std::endl;
	
	//std::cout<<"evento "<<EventID<<std::endl;
	//gets(s);
	//elemiss++;
	elehere = false;
      }	


//     if(QESignal && !trackFGD1 && !trackFGD2)
//       {
// 	//std::cout<<i<<"evento "<<EventID<<std::endl;
// 	//gets(s);
//       }

    if(verboseclean)std::cout<<"Tracks find in TPC, event number "<<EventID<<std::endl;

    if(trackFGD1 || trackFGD2)
      {

	//if(trackFGD2)leptonTrack=leptonTrack2;
	
	if(verboseclean)
	  {
	    //std::cout<<"Track selected with mom TPC 1 "<<leptonTrack->tpc_momentum[1]<<" TPC2 "<<leptonTrack->tpc_momentum[2]<<" Pass first cut "<<std::endl;
	  }

	if(QESignal)NQEAna[Cut]++;
	if(AnaSignal)NSigAna[Cut]++;
	else NBackAna[Cut]++;
	Cut++;


	

	
	//std::cout<<"Cut "<<Cut<<" NSig Ana "<<NSigAna[Cut]<<" back "<<NBackAna[Cut]<<std::endl;


	///require that the track is in the FV

	if(trackFGD1)ND::TTrackerReconModule::TTrackerTrack *track = leptonTrack;
	if(trackFGD2)ND::TTrackerReconModule::TTrackerTrack *track = leptonTrack2;
	
	// Assume that upstream end of track is initial vertex.
	recon_front_pos  = track->FrontPosition;
	TVector3 tmp_pos(recon_front_pos.X(),recon_front_pos.Y(),recon_front_pos.Z());
	
	
	if(verboseclean)
	  {
	    std::cout<<"check that the track is in the FGD FV"<<std::endl;
	    std::cout<<"Track starting position "<<tmp_pos.X()<<" "<<tmp_pos.Y()<<" "<<tmp_pos.Z()<<std::endl;
	   
	  }

	if(IsInvolume(tmp_pos,fFGD1FiducialMin,fFGD1FiducialMax)==false &&  
	   IsInvolume(tmp_pos,fFGD2FiducialMin,fFGD2FiducialMax)==false)
	  {

	    //std::cout<<i<<" Event "<<EventID<<std::endl;
	    continue;
	  }


	  {
	    
	    if(verboseclean)std::cout<<"the track is in FGD FV, pass cut 2"<<std::endl;
	
	    if(QESignal)NQEAna[Cut]++;
	    if(AnaSignal)NSigAna[Cut]++;
	    else NBackAna[Cut]++;
	    Cut++;
	    
	
	    if(CCQESel)
	      {
		if(trackFGD1)
		  if(total_tpc[1]==0 || total_tpc[1]>2)
		    {
		      continue;
		    }
		if(trackFGD2)
		  if(total_tpc[2]==0 || total_tpc[2]>2)
		    {
		      continue;
		    }
	

		if(QESignal)NQEAna[Cut]++;
		if(AnaSignal)NSigAna[Cut]++;
		else NBackAna[Cut]++;
		Cut++;
		
	      }
		

	    


	    if(verboseclean)std::cout<<"check negative curvature!!! "<<track->tpc_curv[1]<<" error "<<track->tpc_curv_error[1]<<std::endl;	    
	    if(verboseclean)std::cout<<track->tpc_curv[2]<<" error "<<track->tpc_curv_error[2]<<std::endl;	    
	    



    int charge = getTrackTpcCharge(track);
    double dEdx = getTrackdEdx(track);
    double momentum = getTrackMomentum(track);
    int trueId = getTrackTruePDG(track);
    
    double momX = getTrackTrueMomX(track);
    double momY = getTrackTrueMomY(track);
    double angle = getTrackCosAngle(track);
    
    
    double leptontruemom = getTrackTrueMomTPC(track);
    
	      
    double resolution=(momentum-leptontruemom);///leptontruemom;


   

	    //std::cout<<CurvCut<<std::endl;
	    
	    if(trackFGD1)
	      {	
		CurvatureError->Fill(track->tpc_curv_error[1]/track->tpc_curv[1]);
		
		if(MoreMomBinCurv)
		  {
		    if(track->tpc_momentum[1]<FirstMomBin)
		      {
			if(track->tpc_curv[1]-CurvCut*(track->tpc_curv_error[1])<0)// && track->tpcPID_NTrun[1]<20)
			  {
			    if(trueId==11)MomEleMiss->Fill(momentum);

			    continue;
			  }
		      }
		    
		    else if(track->tpc_momentum[1]<SecondMomBin)
		      {
			if(track->tpc_curv[1]<0)
			  {
			    if(trueId==11)MomEleMiss->Fill(momentum);
			    continue;
			  }
		      }
		  }

		else
		  if(track->tpc_curv[1]-CurvCut*(track->tpc_curv_error[1])<0)//   track->tpcPID_NTrun[1]<20)
		    {
		      if(trueId==11)
			{
			  MomEleMiss->Fill(momentum);
			  NClMiss->Fill(track->tpcPID_NTrun[1]/0.7);
			}

		      continue; 
		    }

	      }
		
		
	  
	    if(trackFGD2)
	      {
		CurvatureError->Fill(track->tpc_curv_error[2]/track->tpc_curv[2]);
		
		if(MoreMomBinCurv)
		  {
		    if(track->tpc_momentum[2]<FirstMomBin)
		      {
			if(track->tpc_curv[2]-CurvCut*(track->tpc_curv_error[2])<0)
			  {
			    if(trueId==11)MomEleMiss->Fill(momentum);
			     continue;
			  }
		      }
		    
		    else if(track->tpc_momentum[2]<SecondMomBin)
		      {
			if(track->tpc_curv[2]<0)
			  {
			    if(trueId==11)MomEleMiss->Fill(momentum);
			    continue;
			  }
		      }
		  }

		else
		  if(track->tpc_curv[2]-CurvCut*(track->tpc_curv_error[2])<0)// &&  track->tpcPID_NTrun[2]<20)
		    {
		      if(trueId==11)
			{
			  MomEleMiss->Fill(momentum);
			  NClMiss->Fill(track->tpcPID_NTrun[2]/0.7);
			}
		      continue; 
		    }
	      }
	    
	    if(trueId==11)
	      {
		elepassed++;
		MomEleSel->Fill(momentum);
		if(trackFGD1)NClSel->Fill(track->tpcPID_NTrun[1]/0.7);
		else if(trackFGD2)NClSel->Fill(track->tpcPID_NTrun[2]/0.7);
	      }
		

	    
	    if(verboseclean)std::cout<<"pass curvature cut!"<<std::endl;
	      

    if(leptonTrack)
      {
	trueId = leptonTrack->True_TrajPDG[1];     
	if(trueId==11)elesel++;
	else if(trueId==-11)posisel++;
	else if(trueId==13)muonsel++;
	else if(trueId==-13)muonpossel++;
	else if(trueId==211)pionsel++;
	else if(trueId==-211)pionnegsel++;
	else if(trueId==2212)protonsel++;
	else if(trueId==321)kaonsel++;
	allsel++;
	//std::cout<<i<<" TPC1 "<<trueId<<std::endl;

     }
	
    else if(leptonTrack2)
      {
	trueId = leptonTrack2->True_TrajPDG[2];     
	if(trueId==11)elesel++;
	else if(trueId==-11)posisel++;
	else if(trueId==13)muonsel++;
	else if(trueId==-13)muonpossel++;
	else if(trueId==211)pionsel++;
	else if(trueId==-211)pionnegsel++;
	else if(trueId==2212)protonsel++;
	else if(trueId==321)kaonsel++;
	allsel++;
	//std::cout<<i<<" TPC2 "<<trueId<<std::endl;
      }	


    if(trueId!=11)
      {
	for(unsigned int j = 0; j < NTracks; j++){
	  ND::TTrackerReconModule::TTrackerTrack *othertrack 
	    = (ND::TTrackerReconModule::TTrackerTrack*)(*Tracks)[j];
  
	  if(trackFGD1)
	    {
	      //std::cout<<othertrack->True_TrajPDG[1]<<" "<<othertrack->tpc_momentum[1]<<std::endl;
	      if(othertrack->True_TrajPDG[1]==11)elehere = true;
	    }


	  if(trackFGD2)
	    {
	      //std::cout<<othertrack->True_TrajPDG[2]<<" "<<othertrack->tpc_momentum[2]<<std::endl;
	      if(othertrack->True_TrajPDG[2]==11)elehere = true;
	    }
	}
      }

    if(elehere)
      {
	//if(trackFGD1)std::cout<<"TPC 1 "<<std::endl;
	//if(trackFGD2)std::cout<<"TPC 2 "<<std::endl;
	
	//std::cout<<"evento "<<EventID<<std::endl;
	//gets(s);
	elemiss++;
	elehere = false;
      }	



	      
	    if(QESignal)NQEAna[Cut]++;
	      if(AnaSignal)NSigAna[Cut]++;
	      else NBackAna[Cut]++;
	      Cut++;


	      	      
	      Charge_1track->Fill(charge);
	      
	    	      
	      if(verboseclean)std::cout<<"TPC PID PUll ele hypo TPC 1"<<track->tpcPID_PullEle[1]<<" TPC 2 "<<track->tpcPID_PullEle[2]<<std::endl;


	      if(PDGneutrino==12)
		{

		  if(trackFGD2)
		    {
		      PullSignal->Fill(track->tpcPID_PullEle[2]);
		  
		    }
		  if(trackFGD1)
		    {
		      PullSignal->Fill(track->tpcPID_PullEle[1]);
		  
		    }
		}


		LeptonResolution->Fill(resolution);
		
		LeptonMomentum->Fill(momentum);
		LeptonMomentum1GeV->Fill(momentum);
		LeptonCosAngle->Fill(angle);
		LeptondEdx->Fill(dEdx);
		
		if(trackFGD1)
		  {
		    PullElectrons->Fill(track->tpcPID_PullEle[1]);
		    PullMuons->Fill(track->tpcPID_PullMuon[1]);
		  }
		if(trackFGD2)
		  {
		    PullElectrons->Fill(track->tpcPID_PullEle[2]);
		    PullMuons->Fill(track->tpcPID_PullMuon[2]);
		  }

		////////////////////from this point cut select electrons!




		if(PIDMomBin)
		  {
		    if(momentum<1500)
		      {
			if(trackFGD1)
			  if(track->tpcPID_PullEle[1]>=FirstPullBinMax || track->tpcPID_PullEle[1]<FirstPullBinMin)
			    continue;
			
			if(trackFGD2)
			  if(track->tpcPID_PullEle[2]>=FirstPullBinMax || track->tpcPID_PullEle[2]<<FirstPullBinMin)
			    continue;
		      }
		  
		    else
		      {
			if(trackFGD1)
			  if(track->tpcPID_PullEle[1]>=SecondPullBinMax || track->tpcPID_PullEle[1]<SecondPullBinMin)continue;
		
			if(trackFGD2)
			  if(track->tpcPID_PullEle[2]>=SecondPullBinMax || track->tpcPID_PullEle[2]<SecondPullBinMin)continue;
		      }
		  }

	

		else
		  {
		    if(trackFGD1)
		      if(track->tpcPID_PullEle[1]>=PullMax || track->tpcPID_PullEle[1]<PullMin)
			continue;
			
		    if(trackFGD2)
		      if(track->tpcPID_PullEle[2]>=PullMax || track->tpcPID_PullEle[2]<PullMin)
			continue; 
		  }


		if(verboseclean)std::cout<<"Passed cut on the PID"<<std::endl;
		

		{
				

		ChargeCut->Fill(-1);
		
		
		
		/////////////////pid electrons
		if(QESignal)NQEAna[Cut]++;
		if(AnaSignal)NSigAna[Cut]++;
		else NBackAna[Cut]++;
		Cut++;
		/////////////////////
		
		

			
		
		////////try to study how to remove electronc coming from pi0 decays looking at the vertex

		recon_front_pos  = track->FrontPosition;
		TVector3 sel_pos(recon_front_pos.X(),recon_front_pos.Y(),recon_front_pos.Z());
		double distVertex=0;
		
				     
		/////////////////////////////////
		


	      if(PDGneutrino==12)
		{

		  if(trackFGD2)
		    {
		      MomentumSignal->Fill(track->tpc_momentum[2]);
		    }
		  if(trackFGD1)
		    {
		      MomentumSignal->Fill(track->tpc_momentum[1]);
		    }
		}
	      else
		{
		  if(trackFGD1)MomentumBackground->Fill(track->tpc_momentum[1]);
		  else MomentumBackground->Fill(track->tpc_momentum[2]);
		}
	      


	      
	      if(verboseclean)std::cout<<"Cut on the momentum (required larger than 200 MeV): meas momentum "<<momentum<<std::endl;

		if(trackFGD1)
		  if(fabs(leptonTrack->tpc_momentum[1])<MomentumMin || leptonTrack->tpcPID_NTrun[1]<20)continue;
		
		if(trackFGD2)
		  if(fabs(leptonTrack2->tpc_momentum[2])<MomentumMin || leptonTrack2->tpcPID_NTrun[2]<20)continue;

		if(verboseclean)std::cout<<"Cut on momentum passed!"<<std::endl;
		
		
		//badPID[count]=i;
		count++;
		
			
		if(QESignal)NQEAna[Cut]++;
		if(AnaSignal)NSigAna[Cut]++;
		else NBackAna[Cut]++;
		Cut++;

		

		if(fabs(PDGneutrino)==12)
		  {
		    MomentumSignal0->Fill(momentum);
		  }
		else 
		  if(fabs(trueId)<14)
		    MomentumContamination0->Fill(momentum);

		    

		
		if(trackFGD1)
		  {
		    LeptonQuality->Fill(track->tpc_quality[1]);
		    //if(track->tpc_quality[1]>200)
		    //std::cout<<"i "<<i<<" Event ID "<<EventID<<" Quality "<<track->tpc_quality[1]<<std::endl;
		  }
		else
		  {
		    LeptonQuality->Fill(track->tpc_quality[2]);
		    //if(track->tpc_quality[2]>200)
		    //std::cout<<"i "<<i<<" Event ID "<<EventID<<" Quality "<<track->tpc_quality[2]<<std::endl;

		  }



	

		int Nelike=0;
		int Nprotonlike=0;
		int NtotTrack=0;
		double Eelike=0;
		double elikecosangle=0;

		////loop over all the tracks to check distance between the selected track and the neutrino interaction
		for(unsigned int j = 0; j < NTracks; j++){
		  ND::TTrackerReconModule::TTrackerTrack *othertrack 
		    = (ND::TTrackerReconModule::TTrackerTrack*)(*Tracks)[j];
		  

		  ///////////vertex of the others tracks
		  recon_other_front_pos  = othertrack->FrontPosition;
		  TVector3 other_pos(recon_other_front_pos.X(),recon_other_front_pos.Y(),recon_other_front_pos.Z());
		  double dist=sqrt((other_pos.X()-sel_pos.X())**2+(other_pos.Y()-sel_pos.Y())**2+(other_pos.Z()-sel_pos.Z())**2);
		  double distZ = other_pos.Z()-sel_pos.Z();
		  double distY = other_pos.Y()-sel_pos.Y();

		  ///number of tracks elike
		  if(trackFGD1)
		    {

		      if(othertrack->tpc_momentum[1]!=leptonTrack->tpc_momentum[1] && othertrack->tpc_momentum[1]!=Eelike)
			{
			  
			  //std::cout<<PDGneutrino<<" "<<othertrack->tpcPID_PullProton[1]<<std::endl;
			  if(fabs(othertrack->tpcPID_PullProton[1])<3)
			    Nprotonlike++;

			  //NtotTrack=total_tpc[1];
			  if(fabs(othertrack->tpcPID_PullEle[1])<1.5)// && othertrack->tpc_curv[1]>0)
			    {
			      elikecosangle=getTrackCosAngle(othertrack);
			      Nelike++;
			      Eelike+=othertrack->tpc_momentum[1];
			    }
			}
		    }
		  else if(trackFGD2)
		    {

		      if(othertrack->tpc_momentum[2]!=leptonTrack2->tpc_momentum[2] && othertrack->tpc_momentum[2]!=Eelike)
			{
			  //std::cout<<PDGneutrino<<" "<<othertrack->tpcPID_PullProton[2]<<std::endl;

			  if(fabs(othertrack->tpcPID_PullProton[2])<2)
			    Nprotonlike++;


			  //NtotTrack=total_tpc[2];
			  if(fabs(othertrack->tpcPID_PullEle[2])<1.5)// && othertrack->tpc_curv[2]>0)
			    {
			      elikecosangle=getTrackCosAngle(othertrack);
			      Nelike++;
			      Eelike+=othertrack->tpc_momentum[2];
			    }
			}
		    }




		} ///end loop on all the tracks

		if(fabs(PDGneutrino)==12)
		  {
		    //std::cout<<" signal "<<Nelike<<" "<<Eelike<<std::endl;
		    NelikeSignal->Fill(Nelike);
		    if(trackFGD1)NTotTrackSignal->Fill(total_tpc[1]);
		    else if(trackFGD2)NTotTrackSignal->Fill(total_tpc[2]);
		    if(Nelike>0)EelikeSignal->Fill(Eelike);
		  }
		else 
		  {
		    //std::cout<<" background "<<Nelike<<" "<<Eelike<<std::endl;
		    //NTotTrackBack->Fill(NtotTrack);
		    if(trackFGD1)NTotTrackBack->Fill(total_tpc[1]);
		    else if(trackFGD2)NTotTrackBack->Fill(total_tpc[2]);
		    NelikeBack->Fill(Nelike);
		    if(Nelike>0)EelikeBack->Fill(Eelike);
		  }
		
		
	       
		if(verboseclean)
		  {
		    //std::cout<<"entry "<<EventID<<std::endl;
		    //std::cout<<"PDG neutrino "<<PDGneutrino<<std::endl;
		    //std::cout<<"reaction "<<reactionCode<<" momentum "<<momentum<<std::endl;
		    if(trackFGD1)std::cout<<"Momentum max "<<leptonTrack->tpc_momentum[1]<<" N cluster truncated "<<leptonTrack->tpcPID_NTrun[1]<<std::endl;
		    else std::cout<<"Momentum max "<<leptonTrack2->tpc_momentum[2]<<" N cluster truncated "<<leptonTrack2->tpcPID_NTrun[2]<<std::endl;
		    //std::cout<<"dist vertex "<<distVertex<<" distance in Y "<<distY<<" distance in Z "<<distZ<<std::endl;
		    std::cout<<"n elike "<<Nelike<<" E elike"<<Eelike<<" n tot track "<<NtotTrack<<std::endl;
		    std::cout<<" "<<std::endl;

		  }

		    ///Try ECAL
		

		    double ECALpidTrack = 0;
		    double ECALpidShower = 0;
		    double ECALenergy = 0;
		    double ECALenergyNoMatch = 0;
		    double ECALAssEne = 0;

		    int total_tracks = NReconObject; // Total number of tracks.
		    int total_ECAL = 0;    // Total number of ECAL tracks.
		    for(unsigned int j = 0; j < NReconObject; j++){
		      // NB currently does tracks change for clusters
		      ND::TTrackerECALReconModule::TECALReconObject *ECALrec 
			= (ND::TTrackerECALReconModule::TECALReconObject*)(*ReconObject)[j];
		      
		      // Assume that downstream end of track is final vertex.
		      ECALpidTrack  = ECALrec->TrShval;
		      ECALpidShower  = ECALrec->EMHadVal;
		      
		      ECALenergy += ECALrec->Shower.EDeposit;
		      
		      
		      //TVector3 tmp_pos(recon_ECAL_back_pos.X(),recon_ECAL_back_pos.Y(),recon_ECAL_back_pos.Z());
	
			  if(PDGneutrino==12)
			    {
			      
			      ECALPidSignal->Fill(ECALpidTrack);
			      ECALPidShowerSignal->Fill(ECALpidShower);
			    }
			  else 
			    {
			      ECALPidBack->Fill(ECALpidTrack);
			      ECALPidShowerBack->Fill(ECALpidShower);
			    }


		      if(fabs(ECALrec->Shower.Position.Z())-fabs(track->BackPosition.Z())<300 && fabs(ECALrec->Shower.Position.X())-fabs(track->BackPosition.X())<300 &&fabs(ECALrec->Shower.Position.Y())-fabs(track->BackPosition.Y())<300)
			{
			  //doubles distance = sqrt(fabs(ECALrec->Shower.Position.Z())-fabs(track->BackPosition.Z())
			  if(ECALrec->Shower.EDeposit>ECALAssEne)ECALAssEne = ECALrec->Shower.EDeposit;

			  
			  if(PDGneutrino==12)
			    {
			      //if(ECALAssEne>26)EvsPSignal->Fill(ECALAssEne/momentum);
			      ECALPidMatchSignal->Fill(ECALpidTrack);
			      ECALPidShowerMatchSignal->Fill(ECALpidShower);
			    }
			  else 
			    {
			      //if(ECALAssEne>26)EvsPBack->Fill(ECALAssEne/momentum);
			      ECALPidMatchBack->Fill(ECALpidTrack);
			      ECALPidShowerMatchBack->Fill(ECALpidShower);
			    }
			}
		      else
			ECALenergyNoMatch += ECALrec->Shower.EDeposit;



		      
		      if(ECALpidTrack>ECALTrSh)
			{
			  if(fabs(ECALrec->Shower.Position.Z())-fabs(track->BackPosition.Z())<300 && fabs(ECALrec->Shower.Position.X())-fabs(track->BackPosition.X())<300 &&fabs(ECALrec->Shower.Position.Y())-fabs(track->BackPosition.Y())<300)
			    {
			      ECALtracklike = true;
			    }
			}

		    }
		    
		   
		    double diffangle = angle-elikecosangle;

		    if(CCQESel)
		      {
			if(verboseclean)std::cout<<" Number of proton tracks "<<Nprotonlike<<std::endl;
			if(trackFGD1)
			  if(total_tpc[1]>1 && Nprotonlike==0)continue;
			if(trackFGD2)
			  if(total_tpc[2]>1 && Nprotonlike==0)continue;
			
			if(verboseclean)std::cout<<"Pass Number of proton tracks cuts"<<std::endl;
		      

			if(QESignal)NQEAna[Cut]++;
			if(AnaSignal)NSigAna[Cut]++;
			else NBackAna[Cut]++;
			Cut++;
		    
		      }

		    if(Nelike==1)
		      {
			if(fabs(PDGneutrino)==12)DiffAngleSignal->Fill(diffangle);
			else if(fabs(PDGneutrino)==14)DiffAngleBack->Fill(diffangle);
		      }

		    
		    if(verboseclean)std::cout<<" Number of elike tracks "<<Nelike<<std::endl;
		    if(Nelike>0)continue;
		    if(verboseclean)std::cout<<"Pass Number of elike tracks cuts"<<std::endl;

		    if(QESignal)NQEAna[Cut]++;
		    if(AnaSignal)NSigAna[Cut]++;
		    else NBackAna[Cut]++;
		    Cut++;




		  
		    if(fabs(PDGneutrino)==12)
		      {
			MomentumSignal1->Fill(momentum);
		      }
		    else 
		      if(fabs(trueId)<14)MomentumContamination1->Fill(momentum);

		
		///////////true track PDG
		if(fabs(trueId<15))
		  {
		    if(PDGneutrino==12)
		      PDGSignal1->Fill(trueId);
		    else
		      PDGContamination1->Fill(trueId);
		  }
		else if(trueId==211)
		  {
		    if(PDGneutrino==12)
		      PDGSignal1->Fill(1);
		    else
		      PDGContamination1->Fill(1); ///pions+
		  }
		else if(trueId==-211)
		  {
		    if(PDGneutrino==12)
		      PDGSignal1->Fill(-1);
		    else
		      PDGContamination1->Fill(-1); //pions-
		  }
		else if(trueId==321)
		  {
		    if(PDGneutrino==12)
		      PDGSignal1->Fill(3); //kaons
		    else  
		      PDGContamination1->Fill(3); //kaons
		  }
		else if(trueId==-321)
		  {
		    if(PDGneutrino==12)
		      PDGSignal1->Fill(-3); //kaons
		    else
		      PDGContamination1->Fill(-3); //kaons-
		  }
		else if(trueId==2212)
		  {
		    if(PDGneutrino==12)
		      PDGSignal1->Fill(5); //kaons
		    else
		      PDGContamination1->Fill(5); //protons
		  }		    
		

		if(verboseclean)std::cout<<"ECAL track like variable = "<<ECALpidTrack<<" ECAL track variable "<<std::endl;
		
		if(ECALtracklike)
		  continue;
		
		if(verboseclean)
		  {
		    std::cout<<"Pass ECAL PID cuts"<<std::endl;
		    std::cout<<"Accepted in the analysis"<<std::endl;
		      }
		
		
		if(reactionCode.find("Weak[CC],QES")!=std::string::npos)
		  CCQE++;
		
		if(PDGneutrino==12)
		  {
		    if(ECALAssEne>26)EvsPSignal->Fill(ECALAssEne/momentum);
		    ECALRecEnergySignal->Fill(ECALenergy);
		    ECALRecEnergyNoMSignal->Fill(ECALenergyNoMatch);
		  }
		
		else
		  {
		    if(ECALAssEne>26)EvsPBack->Fill(ECALAssEne/momentum);
		    ECALRecEnergyBack->Fill(ECALenergy);
		    ECALRecEnergyNoMBack->Fill(ECALenergyNoMatch);
		  }
		
		
		
		if(QESignal)NQEAna[Cut]++;
		if(AnaSignal)NSigAna[Cut]++;
		else NBackAna[Cut]++;
		Cut++;

		AnaCut = Cut;

		if(fabs(PDGneutrino)==12)
		  {
		    MomentumSignal2->Fill(momentum);
		  }
		else 
		  if(fabs(trueId)<14)MomentumContamination2->Fill(momentum);
      
		
		///////////true track PDG
		if(fabs(trueId<15))
		  {
		    if(PDGneutrino==12)
		      PDGSignal2->Fill(trueId);
		    else
		      PDGContamination2->Fill(trueId);
		  }
		else if(trueId==211)
		  {
		    if(PDGneutrino==12)
		      PDGSignal2->Fill(1);
		    else
		      PDGContamination2->Fill(1); ///pions+
		  }
		else if(trueId==-211)
		  {
		    if(PDGneutrino==12)
		      PDGSignal2->Fill(-1);
		    else
		      PDGContamination2->Fill(-1); //pions-
		  }
		else if(trueId==321)
		  {
		    if(PDGneutrino==12)
		      PDGSignal2->Fill(3); //kaons
		    else  
		      PDGContamination2->Fill(3); //kaons
		  }
		else if(trueId==-321)
		  {
		    if(PDGneutrino==12)
		      PDGSignal2->Fill(-3); //kaons
		    else
		      PDGContamination2->Fill(-3); //kaons-
		  }
		else if(trueId==2212)
		  {
		    if(PDGneutrino==12)
		      PDGSignal2->Fill(5); //kaons
		    else
		      PDGContamination2->Fill(5); //protons
		      }		    
		
				    
		if(PDGneutrino!=12)
		  {
		    if(input.find("Weak[CC]")!=std::string::npos)Charged++;
		    else if(input.find("Weak[NC]")!=std::string::npos)Neutral++;
		    else Other++;
		  }
		
				
		PDGnuSign->Fill(PDGneutrino);	      
		
		double EnergyNu=(938*momentum)/(938-momentum*(1-angle));
	        double pvsE = momentum/EnergyNu;

		if(fabs(PDGneutrino)==12)
		  {
		    MomentumNuE->Fill(momentum);
		    EnergyNuESignal->Fill(EnergyNu);
		    AngleNuESignal->Fill(angle);
		    MomentumEnergySignal->Fill(pvsE);
		  }
		else 
		  {
		    MomentumNuMu->Fill(momentum);
		    EnergyNuECont->Fill(EnergyNu);
		    AngleNuECont->Fill(angle);
		    MomentumEnergyBack->Fill(pvsE);
		  }
		
		
		
		



	      } // End step 2 --> negative tracks. 
		  
	  
	      ///require that the track is positive at 3 sigma
	      //else
	      if(track->tpc_curv[1]+2*(track->tpc_curv_error[1])<0)
		{

		  ChargeCut->Fill(1);

		  PositiveTrack++;

		  PositiveMomentum->Fill(momentum);
		  PositiveCosAngle->Fill(angle);
		  PositivedEdx->Fill(dEdx);
		  PullPions->Fill(track->tpcPID_PullPion[1]);
		  PullProtons->Fill(track->tpcPID_PullProton[1]);
		  
		  if(fabs(track->tpcPID_PullPion[1])<fabs(track->tpcPID_PullProton[1]))
		    PullPositive->Fill(track->tpcPID_PullPion[1]);
		  else
		    PullPositive->Fill(track->tpcPID_PullProton[1]);
		  
		  
		}
	      else
		{
		  UndefinedTrack++;
		  
		  ChargeCut->Fill(0);
		  MomentumUndef->Fill(momentum);

		}
	      
		  
	    }// Track in the FGD fiducial volume
	  

	
	
      }///Exist a lepton track
    

	     
    

    ///number of tracks in the 3 TPCs
    TracksTPC1->Fill(total_tpc[0]);
    TracksTPC2->Fill(total_tpc[1]);
    TracksTPC3->Fill(total_tpc[2]);

    if(fabs(PDGneutrino==12))TrackSign->Fill(total_tpc[1]);
    else TrackBack->Fill(total_tpc[1]);
    
    
  }///loop on all the entries



  ////Plot histograms

  

  double PUR[10];
  double FOM[10];
  double FOM1sig[10];
  double FOM2sig[10];

  TGraph *EffAna=new TGraph(10);
  TGraph *EffQEAna=new TGraph(10);
  TGraph *PurAna= new TGraph(10);
  TGraph *FOMAna=new TGraph(10);
  TGraph *FOM1SigAna=new TGraph(10);
  TGraph *FOM2SigAna=new TGraph(10);

  EffAna->SetName("EffAna");
  EffQEAna->SetName("EffQEAna");
  FOMAna->SetName("FOMAna");
  FOM1SigAna->SetName("FOM1SigAna");
  FOM2SigAna->SetName("FOM2SigAna");
  

  EffAna->SetMarkerColor(2);
  EffQEAna->SetMarkerColor(2);
  FOMAna->SetMarkerColor(4);
  FOM1SigAna->SetMarkerColor(1);
  FOM2SigAna->SetMarkerColor(3);

  EffAna->SetMarkerStyle(20);
  EffQEAna->SetMarkerStyle(20);
  FOMAna->SetMarkerStyle(21);
  FOM1SigAna->SetMarkerStyle(22);
  FOM2SigAna->SetMarkerStyle(23);

  for(int w=0;w<AnaCut;w++)
    {
      std::cout<<"Cut "<<w<<" N QE "<<NQEAna[w]<<" N Signal "<<NSigAna[w]<<" N background "<<NBackAna[w]<<" CC nue "<<nueCCint<<" CCQE nue "<<nueCCqeint<<std::endl;
      
      PUR[w]=NSigAna[w]/(NSigAna[w]+NBackAna[w]);
      FOM[w]=NSigAna[w]/sqrt(NSigAna[w]+NBackAna[w]);
      FOM1sig[w]=NSigAna[w]/sqrt(NSigAna[w]+NBackAna[w]+(0.1*NBackAna[w])*(0.1*NBackAna[w]));
      FOM2sig[w]=NSigAna[w]/sqrt(NSigAna[w]+NBackAna[w]+(0.2*NBackAna[w])*(0.2*NBackAna[w]));
      NSigAna[w]=NSigAna[w]/nueCCint;
      NQEAna[w]=NQEAna[w]/nueCCqeint;

      EffQEAna->SetPoint(w,w,NQEAna[w]);
      EffAna->SetPoint(w,w,NSigAna[w]);
      PurAna->SetPoint(w,w,PUR[w]);
      FOMAna->SetPoint(w,w,FOM[w]);
      FOM1SigAna->SetPoint(w,w,FOM1sig[w]);
      FOM2SigAna->SetPoint(w,w,FOM2sig[w]);

    }



      



 
  //std::cout<<"Neutrino true interactions "<<nuint<<" CC "<<nuCCint<<" QE "<<nuCCqeint<<std::endl;


  TCanvas *ctotaltrack=new TCanvas("ctotaltrack","Tracks");
  TotalTracks->Draw();




  TCanvas *cTPCtrack=new TCanvas("cTPCtrack","Tracks TPC",1000,600);
  cTPCtrack->Divide(3,1);
  cTPCtrack->cd(1);
  TracksTPC1->Draw();
  cTPCtrack->cd(2);
  TracksTPC2->Draw();
  cTPCtrack->cd(3);
  TracksTPC3->Draw();
  TrackSign->SetLineColor(4);
  TrackSign->Draw("same");
  TrackBack->SetLineColor(2);
  TrackBack->Draw("same");
  

  TCanvas *cTrackCharge=new TCanvas("cTrackCharge","Track charge");
  Charge_1track->Draw();
  
  TCanvas *clepton=new TCanvas("clepton","Lepton info",1000,700);
  clepton->Divide(2,2);
  clepton->cd(1);
  LeptonMomentum->Draw();
  clepton->cd(2);
  LeptonResolution->Draw();
  clepton->cd(3);
  LeptonQuality->Draw();
  clepton->cd(4);
  LeptonCosAngle->Draw();

  TCanvas *cchargetrack=new TCanvas("cchargetrack","Track Charge sigma");
  ChargeCut->Draw();


  TCanvas *cpositive=new TCanvas("cpositive","Positive info",900,600);
  cpositive->Divide(3,1);
  cpositive->cd(1);
  PositiveMomentum->Draw();
  cpositive->cd(2);
  PositivedEdx->Draw();
  cpositive->cd(3);
  PositiveCosAngle->Draw();

  TCanvas *cpulllep=new TCanvas("cpulllep","Lepton pulls",900,600);
  cpulllep->Divide(2,1);
  cpulllep->cd(1);
  PullElectrons->Draw();
  cpulllep->cd(2);
  //PullMuons->Draw();
  PullSignal->Draw();

  TCanvas *cmomsignal=new TCanvas("cmomsignal","Signal Momentum",700,500);
  MomentumSignal->Draw();
  MomentumNuE->SetLineColor(2);
  MomentumNuE->Draw("same");
  MomentumNuMu->SetLineColor(4);
  MomentumNuMu->Draw("same");

  float contmu[3];
  float index[3]={1,2,3};

 


  TCanvas *cpullpos=new TCanvas("cpullpos","Positive pulls",900,600);
  cpullpos->Divide(3,1);
  cpullpos->cd(1);
  PullPions->Draw();
  cpullpos->cd(2);
  PullProtons->Draw();
  cpullpos->cd(3);
 PullPositive->Draw();


  TGraph *NeutrinoIntTot=new TGraph(4);
  NeutrinoIntTot->SetName("NeutrinoIntTot");
  TGraph *NeutrinoIntCC=new TGraph(4);
  NeutrinoIntCC->SetName("NeutrinoIntCC");
  TGraph *NeutrinoIntCCQE=new TGraph(4);
  NeutrinoIntCCQE->SetName("NeutrinoIntCCQE");
  
  NeutrinoIntTot->SetMarkerColor(2);
  NeutrinoIntTot->SetMarkerStyle(20);
  NeutrinoIntCC->SetMarkerColor(4);
  NeutrinoIntCC->SetMarkerStyle(21);
  NeutrinoIntCCQE->SetMarkerColor(3);
  NeutrinoIntCCQE->SetMarkerStyle(22);
  

  NeutrinoIntTot->SetPoint(0,1,numuTot);
  NeutrinoIntCC->SetPoint(0,1,numuCCint);
  NeutrinoIntCCQE->SetPoint(0,1,numuCCqeint);

  NeutrinoIntTot->SetPoint(1,2,antinumuTot);
  NeutrinoIntCC->SetPoint(1,2,antinumuCCint);
  NeutrinoIntCCQE->SetPoint(1,2,antinumuCCqeint);

  NeutrinoIntTot->SetPoint(2,3,nueTot);
  NeutrinoIntCC->SetPoint(2,3,nueCCint);
  NeutrinoIntCCQE->SetPoint(2,3,nueCCqeint);

  NeutrinoIntTot->SetPoint(3,4,antinueTot);
  NeutrinoIntCC->SetPoint(3,4,antinueCCint);
  NeutrinoIntCCQE->SetPoint(3,4,antinueCCqeint);



  TCanvas *cnupdg=new TCanvas("cnupdg","neutrino pdg");
  cnupdg->Divide(2,1);
  cnupdg->cd(1);
  PDGnu->Draw("");
  cnupdg->cd(2);
  PDGnuSign->Draw("");

  NeutrinoIntTot->SetMinimum(0);
  TCanvas *cneutrino=new TCanvas("cneutrino","neutrino int");
  NeutrinoIntTot->Draw("AP");
  NeutrinoIntCC->Draw("P");
  NeutrinoIntCCQE->Draw("P");

  TCanvas *cpdgcont=new TCanvas("cpdgcont","pdg cont",1200,1200);
  cpdgcont->Divide(3,2);
  cpdgcont->cd(1);
  PDGContamination0->Draw("");
  cpdgcont->cd(2);
  PDGContamination1->Draw("");
  cpdgcont->cd(3);
  PDGContamination2->Draw("");
  cpdgcont->cd(4);
  PDGContamination3->Draw("");
  cpdgcont->cd(5);
  PDGContamination4->Draw("");
  cpdgcont->cd(6);

  TCanvas *cpdgsign=new TCanvas("cpdgsign","pdg sign",1200,1200);
  cpdgsign->Divide(3,2);
  cpdgsign->cd(1);
  PDGSignal0->Draw("");
  cpdgsign->cd(2);
  PDGSignal1->Draw("");
  cpdgsign->cd(3);
  PDGSignal2->Draw("");
  cpdgsign->cd(4);
  PDGSignal3->Draw("");
  cpdgsign->cd(5);
  PDGSignal4->Draw("");
  
  

  TCanvas *cvertex=new TCanvas("cvertex","vertex distance");
  cvertex->Divide(2,1);
  cvertex->cd(1);
  VertexDistanceSign->Draw();
  cvertex->cd(2);
  VertexDistanceCont->Draw();


  ScatterDePro->SetMarkerColor(1);
  ScatterDeMu->SetMarkerColor(2);
  ScatterDeMuMinus->SetMarkerColor(2);
  ScatterDePi->SetMarkerColor(3);
  ScatterDeEle->SetMarkerColor(4);
  ScatterDePosi->SetMarkerColor(6);
  ScatterDeKa->SetMarkerColor(7);

  PullEle_Pro->SetMarkerColor(1);
  PullEle_Mu->SetMarkerColor(2);
  PullEle_Pi->SetMarkerColor(3);
  PullEle_Ele->SetMarkerColor(4);
  PullEle_Posi->SetMarkerColor(6);
  PullEle_Ka->SetMarkerColor(7);

  PullMu_Pro->SetMarkerColor(1);
  PullMu_Mu->SetMarkerColor(2);
  PullMu_Pi->SetMarkerColor(3);
  PullMu_Ele->SetMarkerColor(4);
  PullMu_Posi->SetMarkerColor(6);
  PullMu_Ka->SetMarkerColor(7);

  PullPro_Pro->SetMarkerColor(1);
  PullPro_Mu->SetMarkerColor(2);
  PullPro_Pi->SetMarkerColor(3);
  PullPro_Ele->SetMarkerColor(4);
  PullPro_Posi->SetMarkerColor(6);
  PullPro_Ka->SetMarkerColor(7);


  ScatterDeProTrue->SetMarkerColor(1);
  ScatterDeMuTrue->SetMarkerColor(2);
  ScatterDePiTrue->SetMarkerColor(3);
  ScatterDeEleTrue->SetMarkerColor(4);
  ScatterDePosiTrue->SetMarkerColor(6);
  ScatterDeKaTrue->SetMarkerColor(7);

  PullEle_ProTrue->SetMarkerColor(1);
  PullEle_MuTrue->SetMarkerColor(2);
  PullEle_PiTrue->SetMarkerColor(3);
  PullEle_EleTrue->SetMarkerColor(4);
  PullEle_PosiTrue->SetMarkerColor(6);
  PullEle_KaTrue->SetMarkerColor(7);

  PullMu_ProTrue->SetMarkerColor(1);
  PullMu_MuTrue->SetMarkerColor(2);
  PullMu_PiTrue->SetMarkerColor(3);
  PullMu_EleTrue->SetMarkerColor(4);
  PullMu_PosiTrue->SetMarkerColor(6);
  PullMu_KaTrue->SetMarkerColor(7);

  PullPro_ProTrue->SetMarkerColor(1);
  PullPro_MuTrue->SetMarkerColor(2);
  PullPro_PiTrue->SetMarkerColor(3);
  PullPro_EleTrue->SetMarkerColor(4);
  PullPro_PosiTrue->SetMarkerColor(6);
  PullPro_KaTrue->SetMarkerColor(7);



  TLegend *legvsmom = new TLegend(0.7,0.6,0.9,0.9);
  legvsmom->AddEntry(ScatterDePro,"Protons","FP");
  legvsmom->AddEntry(ScatterDeMu,"Muons","FP");
  legvsmom->AddEntry(ScatterDePi,"Pions","FP");
  legvsmom->AddEntry(ScatterDeEle,"Electrons","FP");
  legvsmom->AddEntry(ScatterDePosi,"Positrons","FP");
  legvsmom->AddEntry(ScatterDeKa,"Kaons","FP");


  TCanvas *cscatterraw=new TCanvas("cscatterraw","scatter dE raw");
  ScatterDeProRaw->Draw();
  ScatterDeMuRaw->Draw("same");
  ScatterDePiRaw->Draw("same");
  ScatterDeEleRaw->Draw("same");
  ScatterDePosiRaw->Draw("same");
  ScatterDeKaRaw->Draw("same");
  legvsmom->Draw();
  
  TCanvas *cscatter=new TCanvas("cscatter","scatter dE");
  ScatterDePro->Draw();
  ScatterDeMu->Draw("same");
  ScatterDePi->Draw("same");
  ScatterDeEle->Draw("same");
  ScatterDePosi->Draw("same");
  ScatterDeKa->Draw("same");
  legvsmom->Draw();
  
  TCanvas *cscattertrue=new TCanvas("cscattertrue","scatter dE true mom");
  ScatterDeProTrue->Draw();
  ScatterDeMuTrue->Draw("same");
  ScatterDePiTrue->Draw("same");
  ScatterDeEleTrue->Draw("same");
  ScatterDePosiTrue->Draw("same");
  ScatterDeKaTrue->Draw("same");
  legvsmom->Draw();
  

  TCanvas *cpullelehypo=new TCanvas("cpullelehypo","pull ele hypothesis");
  PullEle_Pro->Draw();
  PullEle_Mu->Draw("same");
  PullEle_Pi->Draw("same");
  PullEle_Ele->Draw("same");
  PullEle_Posi->Draw("same");
  PullEle_Ka->Draw("same");
  legvsmom->Draw();
  
  TCanvas *cpullprohypo=new TCanvas("cpullprohypo","pull proton hypothesis");
  PullPro_Pro->Draw();
  PullPro_Mu->Draw("same");
  PullPro_Pi->Draw("same");
  PullPro_Ele->Draw("same");
  PullPro_Posi->Draw("same");
  PullPro_Ka->Draw("same");
  legvsmom->Draw();
  
  TCanvas *cpullmuhypo=new TCanvas("cpullmuhypo","pull muon hypothesis");
  PullMu_Pro->Draw();
  PullMu_Mu->Draw("same");
  PullMu_Pi->Draw("same");
  PullMu_Ele->Draw("same");
  PullMu_Posi->Draw("same");
  PullMu_Ka->Draw("same");
  legvsmom->Draw();
  
  gStyle->SetOptFit(1);
  TCanvas *cpullhypo = new TCanvas("cpullhypo","pull different hypothesis",1200,800);
  cpullhypo->Divide(3,2);
  cpullhypo->cd(1);
  PullEleHypo->Fit("gaus");
  cpullhypo->cd(2);
  PullPosiHypo->Fit("gaus");
  cpullhypo->cd(3);
  PullMuHypo->Fit("gaus");
  cpullhypo->cd(4);
  PullPiHypo->Fit("gaus");
  cpullhypo->cd(5);
  PullProHypo->Fit("gaus");
  cpullhypo->cd(6);
  PullKaHypo->Fit("gaus");


  TCanvas *cpullhypoele = new TCanvas("cpullhypoele","pull different hypothesis ele",1200,800);
  cpullhypoele->Divide(2,2);
  cpullhypoele->cd(1);
  PullEleHypo->Draw();
  cpullhypoele->cd(2);
  PullEleHypo100MeV->Draw();
  cpullhypoele->cd(3);
  PullEleHypo300MeV->Draw();
  cpullhypoele->cd(4);
  PullEleHypo500MeV->Draw();

  TCanvas *cpullhypoposi = new TCanvas("cpullhypoposi","pull different hypothesis posi",1200,800);
  cpullhypoposi->Divide(2,2);
  cpullhypoposi->cd(1);
  PullPosiHypo->Draw();
  cpullhypoposi->cd(2);
  PullPosiHypo100MeV->Draw();
  cpullhypoposi->cd(3);
  PullPosiHypo200MeV->Draw();
  cpullhypoposi->cd(4);
  PullPosiHypo300MeV->Draw();

  TCanvas *cpullhypoeletrue = new TCanvas("cpullhypoeletrue","pull different hypothesis ele true",1200,800);
  cpullhypoeletrue->Divide(2,2);
  cpullhypoeletrue->cd(1);
  PullEleHypoTrue->Draw();
  cpullhypoeletrue->cd(2);
  PullEleHypoTrue100MeV->Draw();
  cpullhypoeletrue->cd(3);
  PullEleHypoTrue200MeV->Draw();
  cpullhypoeletrue->cd(4);
  PullEleHypoTrue300MeV->Draw();

  TCanvas *cpullhypopositrue = new TCanvas("cpullhypopositrue","pull different hypothesis posi true",1200,800);
  cpullhypopositrue->Divide(2,2);
  cpullhypopositrue->cd(1);
  PullPosiHypoTrue->Draw();
  cpullhypopositrue->cd(2);
  PullPosiHypoTrue100MeV->Draw();
  cpullhypopositrue->cd(3);
  PullPosiHypoTrue200MeV->Draw();
  cpullhypopositrue->cd(4);
  PullPosiHypoTrue300MeV->Draw();


 TCanvas *cpullmumom = new TCanvas("cpullmumom","pull muon momentum",1200,800);
 cpullmumom->Divide(3,3);
 cpullmumom->cd(1);
 PullMuHypo300->Fit("gaus");
 cpullmumom->cd(2);
 PullMuHypo500->Fit("gaus");
 cpullmumom->cd(3);
 PullMuHypo700->Fit("gaus");
 cpullmumom->cd(4);
 PullMuHypo900->Fit("gaus");
 cpullmumom->cd(5);
 PullMuHypo1300->Fit("gaus");
 cpullmumom->cd(6);
 PullMuHypo2000->Fit("gaus");
 cpullmumom->cd(7);
 PullMuHypo3000->Fit("gaus");
 cpullmumom->cd(8);
 PullMuHypo5000->Fit("gaus");


 TCanvas *celikeev = new TCanvas("celikeev","elike events",1200,800);
 celikeev->Divide(3,2);
 celikeev->cd(1);
 NelikeSignal->Draw();
 celikeev->cd(2);
 NelikeBack->Draw();
 celikeev->cd(3);
 EelikeSignal->Draw();
 celikeev->cd(4);
 EelikeBack->Draw();
 celikeev->cd(5);
 NTotTrackSignal->Draw();
 celikeev->cd(6);
 NTotTrackBack->Draw();

 ECALPidSignal->SetLineColor(2);
 ECALPidMatchSignal->SetLineColor(2);
 ECALPidShowerSignal->SetLineColor(2);
 ECALPidShowerMatchSignal->SetLineColor(2);
 ECALRecEnergySignal->SetLineColor(2);
 ECALRecEnergyNoMSignal->SetLineColor(2);
 EvsPBack->SetLineColor(2);


 TCanvas *cEvsP = new TCanvas("cEvsP","E versus P",700,500);
 EvsPSignal->Draw();
 EvsPBack->Draw("same");

 TCanvas *cecalpid = new TCanvas("cecalpid","ecal pid",700,500);
 ECALPidSignal->Draw();
 ECALPidBack->Draw("same");

 TCanvas *cecalpidmatch = new TCanvas("cecalpidmatch","ecal pid match",700,500);
 ECALPidMatchSignal->Draw();
 ECALPidMatchBack->Draw("same");

 TCanvas *cecalpidshower = new TCanvas("cecalpidshower","ecal pid shower",700,500);
 ECALPidShowerSignal->Draw();
 ECALPidShowerBack->Draw("same");

 TCanvas *cecalpidshowermatch = new TCanvas("cecalpidshowermatch","ecal pid match shower",700,500);
 ECALPidShowerMatchSignal->Draw();
 ECALPidShowerMatchBack->Draw("same");

 TCanvas *cecalen = new TCanvas("cecalen","ecal en",700,500);
 ECALRecEnergySignal->Draw();
 ECALRecEnergyBack->Draw("same");

 TCanvas *cecalennom = new TCanvas("cecalennom","ecal en no match",700,500);
 ECALRecEnergyNoMSignal->Draw();
 ECALRecEnergyNoMBack->Draw("same");

 TCanvas *cmomvsene = new TCanvas("cmomvsene","momentum vs energy",700,500);
 MomentumEnergySignal->Draw();
 MomentumEnergyBack->Draw("same");

 TCanvas *cmomres = new TCanvas("cmomres","momentum resolution",700,500);
 cmomres->cd(1); 
 MomResMuons->Draw();
 cmomres->cd(2);
 MomResPions->Draw();
 cmomres->cd(3);
 MomResElectrons->Draw();
 cmomres->cd(4);
 MomResProtons->Draw();



  TFile *AnaNtuple=new TFile("Analysis_Ntuple.root","RECREATE");
  TotalTracks->Write();
  TracksTPC1->Write();
  TracksTPC2->Write();
  TracksTPC3->Write();
  Charge_1track->Write();
  LeptonMomentum->Write();
  LeptonMomentum1GeV->Write();
  LeptonResolution->Write();
  LeptonQuality->Write();
  LeptonCosAngle->Write();
  LeptondEdx->Write();
  PositiveMomentum->Write();
  PositivedEdx->Write();
  NumVertexFGD1->Write();
  PullElectrons->Write();
  PullMuons->Write();
  PullMuonsCont->Write();
  PullPions->Write();
  PullProtons->Write();
  PullPositive->Write();


  NeutrinoIntTot->Write();
  NeutrinoIntCC->Write();
  NeutrinoIntCCQE->Write();
  PDGContamination0->Write();
  PDGContamination1->Write();
  PDGContamination2->Write();
  PDGContamination3->Write();
  PDGContamination4->Write();
  PDGSignal0->Write();
  PDGSignal1->Write();
  PDGSignal2->Write();
  PDGSignal3->Write();
  PDGSignal4->Write();
  VertexDistance->Write();
  VertexDistanceSign->Write();
  VertexDistanceCont->Write();
  DistEleSign->Write();
  DistEleCont->Write();
  ChargeCut->Write();
  MomentumUndef->Write();
  PDGnu->Write();
  PDGnuSign->Write();
  PullSignal->Write();
  MomentumSignal->Write();
  MomentumBackground->Write();
  MomentumNuE->Write();
  MomentumNuMu->Write();
 
  ScatterDe->Write();
  ScatterDePro->Write();
  ScatterDeMu->Write();
  ScatterDeMuMinus->Write();
  
  ScatterDePi->Write();
  ScatterDeEle->Write();
  ScatterDePosi->Write();
  ScatterDeKa->Write();
  

  ScatterDeProRaw->Write();
  ScatterDeMuRaw->Write();
  ScatterDePiRaw->Write();
  ScatterDeEleRaw->Write();
  ScatterDePosiRaw->Write();
  ScatterDeKaRaw->Write();
  

  PullEle_Pro->Write();
  PullEle_Mu->Write();
  PullEle_Pi->Write();
  PullEle_Ele->Write();
  PullEle_Posi->Write();
  PullEle_Ka->Write();


  PullMu_Pro->Write();
  PullMu_Mu->Write();
  PullMu_Pi->Write();
  PullMu_Ele->Write();
  PullMu_Posi->Write();
  PullMu_Ka->Write();

  PullPro_Pro->Write();
  PullPro_Mu->Write();
  PullPro_Pi->Write();
  PullPro_Ele->Write();
  PullPro_Posi->Write();
  PullPro_Ka->Write();
 
  ScatterDeProTrue->Write();
  ScatterDeMuTrue->Write();
  ScatterDePiTrue->Write();
  ScatterDeEleTrue->Write();
  ScatterDePosiTrue->Write();
  ScatterDeKaTrue->Write();
  

  PullEle_ProTrue->Write();
  PullEle_MuTrue->Write();
  PullEle_PiTrue->Write();
  PullEle_EleTrue->Write();
  PullEle_PosiTrue->Write();
  PullEle_KaTrue->Write();


  PullMu_ProTrue->Write();
  PullMu_MuTrue->Write();
  PullMu_PiTrue->Write();
  PullMu_EleTrue->Write();
  PullMu_PosiTrue->Write();
  PullMu_KaTrue->Write();

  PullPro_ProTrue->Write();
  PullPro_MuTrue->Write();
  PullPro_PiTrue->Write();
  PullPro_EleTrue->Write();
  PullPro_PosiTrue->Write();
  PullPro_KaTrue->Write();
 
  PullEleHypo->Write();
  PullEleHypo100MeV->Write();
  PullEleHypo300MeV->Write();
  PullEleHypo500MeV->Write();
  PullPosiHypo->Write();
  PullPosiHypo100MeV->Write();
  PullPosiHypo200MeV->Write();
  PullPosiHypo300MeV->Write();

  PullEleHypoTrue->Write();
  PullEleHypoTrue100MeV->Write();
  PullEleHypoTrue200MeV->Write();
  PullEleHypoTrue300MeV->Write();
  PullPosiHypoTrue->Write();
  PullPosiHypoTrue100MeV->Write();
  PullPosiHypoTrue200MeV->Write();
  PullPosiHypoTrue300MeV->Write();


  PullMuHypo->Write();
  PullPiHypo->Write();
  PullProHypo->Write();
  PullKaHypo->Write();

  EnergyNuESignal->Write();
  EnergyNuECont->Write();
  AngleNuESignal->Write();
  AngleNuECont->Write();

  MomentumEnergySignal->Write();
  MomentumEnergyBack->Write();
  

 PullMuHypo300->Write();
 PullMuHypo500->Write();
 PullMuHypo700->Write();
 PullMuHypo900->Write();
 PullMuHypo1300->Write();
 PullMuHypo2000->Write();
 PullMuHypo3000->Write();
 PullMuHypo5000->Write();
 CTMu500->Write();
 CTMu5000->Write();
 

  MomentumSignal0->Write();
  MomentumSignal1->Write();
  MomentumSignal2->Write();
  MomentumSignal3->Write();

  MomentumContamination0->Write();
  MomentumContamination1->Write();
  MomentumContamination2->Write();
  MomentumContamination3->Write();


  EffAna->Write();
  EffQEAna->Write();
  PurAna->Write();
  FOMAna->Write();
  FOM1SigAna->Write();
  FOM2SigAna->Write();
  
  NelikeSignal->Write();
  NelikeBack->Write();
  EelikeSignal->Write();
  EelikeBack->Write();
  NTotTrackSignal->Write();
  NTotTrackBack->Write();

  ECALPidSignal->Write();
  ECALPidBack->Write();
  ECALPidMatchSignal->Write();
  ECALPidMatchBack->Write();
  ECALPidShowerSignal->Write();
  ECALPidShowerBack->Write();
  ECALPidShowerMatchSignal->Write();
  ECALPidShowerMatchBack->Write();

  ECALRecEnergySignal->Write();
  ECALRecEnergyBack->Write();
  ECALRecEnergyNoMSignal->Write();
  ECALRecEnergyNoMBack->Write();


  MomResMuons->Write();
  MomResPions->Write();
  MomResProtons->Write();
  MomResElectrons->Write();

  CurvatureError->Write();

  DiffAngleSignal->Write();
  DiffAngleBack->Write();

  SigmaKvsMom->Write();
  EvsPSignal->Write();
  EvsPBack->Write();

  FirstTrackSelectionSign->Write();
  FirstTrackSelectionBack->Write();

  MomEleSel->Write();
  MomEleMiss->Write();
  NClMiss->Write();
  NClSel->Write();

  std::cout<<"dEdx Entries background "<<PDGContamination0->GetEntries()<<" signal "<<PDGSignal0->GetEntries()<<std::endl; 
  std::cout<<"elike Entries background "<<PDGContamination1->GetEntries()<<" signal "<<PDGSignal1->GetEntries()<<std::endl; 
  std::cout<<"ECAL Entries background "<<PDGContamination2->GetEntries()<<" signal "<<PDGSignal2->GetEntries()<<std::endl; 
  std::cout<<"Cut 3 Entries background "<<PDGContamination3->GetEntries()<<" signal "<<PDGSignal3->GetEntries()<<std::endl; 
  std::cout<<"Cut 4 Entries background "<<PDGContamination4->GetEntries()<<" signal "<<PDGSignal4->GetEntries()<<std::endl; 

  std::cout<<"Back compoentent "<<"charged "<<Charged<<" neutral "<<Neutral<<" others "<<Other<<std::endl;

  FILE *pFile;
  pFile = fopen("badPID.txt","w");

  for(int k=0;k<count;k++)
    {
      fprintf(pFile," i== %d ||",badPID[k]);
      //std::cout<<" i== "<<badPID[k]<<" || "<<std::endl;
    }
  fclose(pFile);

  FILE *pFile2;
  pFile2 = fopen("NueEvent.txt","w");

  for(int k=0;k<nue;k++)
    {
      fprintf(pFile2," %d ",NueEvent[k]);
      //std::cout<<" i== "<<badPID[k]<<" || "<<std::endl;
    }
  fclose(pFile2);



  double x,y;
  double yper;
  double fomx, fomy, fomx1s, fomy1s, fomx2s, fomy2s;


  for(int w=0;w<AnaCut;w++)
    {

      EffAna->GetPoint(w,x,y);
      yper = y*1098;
      FOMAna->GetPoint(w,fomx,fomy);
      FOM1SigAna->GetPoint(w,fomx1s,fomy1s);
      FOM2SigAna->GetPoint(w,fomx2s,fomy2s);
      
      double Nb = (yper/fomy)**2 - yper;

      //std::cout<<"Cut "<<x<<"N selected ev "<<yper<<" N background "<<Nb<<" efficiencies "<<y<<" FOM "<<fomy<<" FOM 1 s "<<fomy1s<<" FOM 2s "<<fomy2s<<std::endl;
      std::cout<<"Cut "<<x<<" & "<<NSigAna[w]*1098<<" & "<<NBackAna[w]<<" & "<<y<<" & "<<fomy<<" & "<<fomy1s<<" & "<<fomy2s<<std::endl;
      std::cout<<"\\hline"<<std::endl;
    }
  
  std::cout<<" TPC PID "<<" & "<< PDGSignal0->GetEntries()<<" & "<<PDGSignal0->GetBinContent(37)<<" & "<<PDGSignal0->GetBinContent(39)<<" & "<<PDGSignal0->GetBinContent(15)<<" & "<<PDGSignal0->GetBinContent(31)<<" & "<<PDGSignal0->GetBinContent(27)<<" & "<<PDGContamination0->GetEntries()<<" & "<< PDGContamination0->GetBinContent(37)<<" & "<<PDGContamination0->GetBinContent(39)<<" & "<<PDGContamination0->GetBinContent(15)<<" & "<<PDGContamination0->GetBinContent(31)<<" & "<<PDGContamination0->GetBinContent(27)<<" \\"<<std::endl;
  std::cout<<"\\hline"<<std::endl;
  std::cout<<" Elike tracks "<<" & "<< PDGSignal1->GetEntries()<<" & "<<PDGSignal1->GetBinContent(37)<<" & "<<PDGSignal1->GetBinContent(39)<<" & "<<PDGSignal1->GetBinContent(15)<<" & "<<PDGSignal1->GetBinContent(31)<<" & "<<PDGSignal1->GetBinContent(27)<<" & "<<PDGContamination1->GetEntries()<<" & "<< PDGContamination1->GetBinContent(37)<<" & "<<PDGContamination1->GetBinContent(39)<<" & "<<PDGContamination1->GetBinContent(15)<<" & "<<PDGContamination1->GetBinContent(31)<<" & "<<PDGContamination1->GetBinContent(27)<<" \\"<<std::endl;
  std::cout<<"\\hline"<<std::endl;
  std::cout<<" ECAL PID "<<" & "<< PDGSignal2->GetEntries()<<" & "<<PDGSignal2->GetBinContent(37)<<" & "<<PDGSignal2->GetBinContent(39)<<" & "<<PDGSignal2->GetBinContent(15)<<" & "<<PDGSignal2->GetBinContent(31)<<" & "<<PDGSignal2->GetBinContent(27)<<" & "<<PDGContamination2->GetEntries()<<" & "<<PDGContamination2->GetBinContent(37)<<" & "<<PDGContamination2->GetBinContent(39)<<" & "<<PDGContamination2->GetBinContent(15)<<" & "<<PDGContamination2->GetBinContent(31)<<" & "<<PDGContamination2->GetBinContent(27)<<" \\"<<std::endl;
  std::cout<<"\\hline"<<std::endl;
  

  std::cout<<CCQE<<" "<< PDGSignal2->GetEntries()<<std::endl;

  std::cout<<"Cut 1 selection "<<"electrons "<<elesel<<" muon "<<muonsel<<" muon positive "<<muonpossel<<" pion "<<pionsel<<" pion neg sel "<<pionnegsel<<" proton sel "<<protonsel<<" positron sel "<<posisel<<" kaon "<<kaonsel<<" total "<<allsel<<std::endl;

  std::cout<<"ele miss "<<elemiss<<std::endl;

  std::cout<<"ele passed cut on the curvature "<<elepassed<<std::endl;

} //end of the function



  		
	
