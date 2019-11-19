#include "iostream"
#include "vector"
#include "string"

#include "TMath.h"
#include <Riostream.h>
#include <TH2.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TBranch.h>
#include <TString.h>
#include <TSystem.h>
#include <TROOT.h>
#include <TF1.h>
#include <TTree.h>

void TestBeamViewer(TString fileName, bool verbose = false){ 

  // Load some important libraries.
  gSystem->AddIncludePath("-I$OAANALYSISROOT/src");
  gSystem->AddIncludePath("-I.");
    
  gSystem->Load("libPhysics");
  gSystem->Load("libGeom");
  gSystem->Load("libTree");
  gSystem->Load("libMatrix");
  gSystem->Load("liboaEvent.so");
  gSystem->Load("librecpack.so");
  gSystem->Load("liboaRecPack.so");
  // These libraries only if you running this macro under v8r*
  gSystem->Load("libBeamData.so");
  gSystem->Load("liboaBeamData.so");
  gSystem->Load("liboaDataQuality.so");
  //
  gSystem->Load("liboaAnalysis.so");
  //
  TFile* file = TFile::Open(fileName.Data(), "READ");

  TChain testBeam = TChain("ReconDir/ECalTestbeam");
  TChain reconTracker = TChain("ReconDir/TrackerECal"); 

  // Create histograms.
   TH2F *C1loC2lo18 = new TH2F("C1loC2lo_18", "Cerenkov1LowGain vs. Cerenkov2LowGain for integration window 18", 128, 0, 1024, 256, 0, 1024);
   TH2F *C1loC2lo19 = new TH2F("C1loC2lo_19", "Cerenkov1LowGain vs. Cerenkov2LowGain for integration window 19", 128, 0, 1024, 256, 0, 1024);

   TH1F *hTOF = new TH1F("TOF", "TOF values", 128, 0, 1024);
   TH1F *hPID = new TH1F("PID", "PID values", 100, -6, 6 );

   TH1D *AvHitTimeE = new TH1D("AvHitTimeE","",500,-2000,-1000);
   TH1D *AvHitTimeM = new TH1D("AvHitTimeM","",500,-2000,-1000);
   TH1D *AvHitTimeP = new TH1D("AvHitTimeP","",500,-2000,-1000);

   TH1D *ETrShval = new TH1D("ETrShval","",500,-2,2); 
   TH1D *EAMR = new TH1D("EAMR","",100,0,100); 
   TH1D *EEMEnergyFit = new TH1D("EEMEnergyFit","",500,0,3000); 
   TH1D *EEMEnergyFitUn = new TH1D("EEMEnergyFitUn","",500,0,1000); 
   TH1D *MTrShval = new TH1D("MTrShval","",500,-2,2); 
   TH1D *MAMR = new TH1D("MAMR","",100,0,100); 
  // _____________________________________________________________________________________
  // Load data files   
  
   reconTracker.AddFile(fileName.Data());
   testBeam.AddFile(fileName.Data());
 
   // Set the branches for the tracker recon information.
  Int_t fNTestbeamTriggers;
  Int_t Cerenkov1Hi[23];
  Int_t Cerenkov1Lo[23];
  Int_t Cerenkov2Hi[23];
  Int_t Cerenkov2Lo[23];
  Int_t TOF[23];
  Double_t TestBeamAngle;
  Double_t TestBeamMomentum;
  Int_t TestBeamPID;

  Int_t NReconObject;
  TClonesArray *ReconObject = new TClonesArray("ND::TTrackerECALReconModule::TECALReconObject",20);

  testBeam.SetBranchAddress("Cerenkov1Hi", &Cerenkov1Hi);
  testBeam.SetBranchAddress("Cerenkov1Lo", &Cerenkov1Lo);
  testBeam.SetBranchAddress("Cerenkov2Hi", &Cerenkov2Hi);
  testBeam.SetBranchAddress("Cerenkov2Lo", &Cerenkov2Lo);
  testBeam.SetBranchAddress("TOF", &TOF);
  testBeam.SetBranchAddress("TestBeamAngle", &TestBeamAngle);
  testBeam.SetBranchAddress("TestBeamMomentum", &TestBeamMomentum);
  testBeam.SetBranchAddress("TestBeamPID", &TestBeamPID);

  if(verbose){
  testBeam.Print("toponly");
  }

  reconTracker.SetBranchAddress("NReconObject", &NReconObject);
  reconTracker.SetBranchAddress("ReconObject", &ReconObject);

  if(verbose){
  reconTracker.Print("toponly");
  }

  int totalEvts = reconTracker.GetEntries();
  
  double trueThetaDeg = 0;
  double firstTrueThetaDeg = 0;

  double tThetaDeg = 0;
  double tsThetaDeg = 0;

  
  // Fit the average hit time
  for(int k = 0; k < totalEvts; k++){
    reconTracker.GetEntry(k);
    testBeam.GetEntry(k);
    if(NReconObject == 1){
      ND::TTrackerECALReconModule::TECALReconObject *ECALobj0 = (ND::TTrackerECALReconModule::TECALReconObject*)(*ReconObject)[0];
      // Electrons
      if ( TestBeamPID == 1 || TestBeamPID == 2)
	AvHitTimeE->Fill(ECALobj0->AverageHitTime);
      // MIPs
      if (TestBeamPID == 3 || TestBeamPID == 4)
	AvHitTimeM->Fill(ECALobj0->AverageHitTime);
      // Protons
      if (TestBeamPID == 5)
	AvHitTimeP->Fill(ECALobj0->AverageHitTime);
    }
  }
  //
  // Fit the histiograms to remove non-beam and overlapping events
  //
  double Ecut1 = 0;
  double Ecut2 = 0;
  double Mcut1 = 0;
  double Mcut2 = 0;
  double Pcut1 = 0;
  double Pcut2 = 0;
  // Fit the mips average hit time histogram
  if(AvHitTimeM->GetEntries() > 0){
    double BinCenter = AvHitTimeM->GetBinCenter(AvHitTimeM->GetMaximumBin());
    double min = BinCenter - 20;
    double max = BinCenter + 20;
    TF1 *gfitM = new TF1("MGaussian","gaus",min,max);
    AvHitTimeM->Fit(gfitM,"QR");
    //Double_t amp = gfit->GetParameter(0);
    Double_t Mmean = gfitM->GetParameter(1);
    Double_t Msigma = gfitM->GetParameter(2);
    Mcut1 = Mmean + 1*Msigma;
    Mcut2 = Mmean - 1*Msigma;
    if(verbose){
      cout << "MBin center: " << BinCenter << endl;
      cout << "MMean : " << Mmean << " MSigma: " << Msigma << endl;
      cout << "Cut Up = " << Mcut1 << endl;
      cout << "Cut Down = " << Mcut2 << endl;
    }
  }
  //
  // Fit the proton average hit time histogram
  if(AvHitTimeP->GetEntries() > 0){
    double BinCenter = AvHitTimeP->GetBinCenter(AvHitTimeP->GetMaximumBin());
    double min = BinCenter - 20;
    double max = BinCenter + 20;
    TF1 *gfitP = new TF1("PGaussian","gaus",min,max);
    AvHitTimeP->Fit(gfitP,"QR");
    //Double_t amp = gfit->GetParameter(0);
    Double_t Pmean = gfitP->GetParameter(1);
    Double_t Psigma = gfitP->GetParameter(2);
    Pcut1 = Pmean + 1*Psigma;
    Pcut2 = Pmean - 1*Psigma;
    if(verbose){
      cout << "PBin center: " << BinCenter << endl;
      cout << "PMean : " << Pmean << " PSigma: " << Psigma << endl;
      cout << "PCut Up = " << Pcut1 << endl;
      cout << "PCut Down = " << Pcut2 << endl;
    }
  }
  //
  // Fit the electron average hit time histogram
  if(AvHitTimeE->GetEntries() > 0){
    double BinCenter = AvHitTimeE->GetBinCenter(AvHitTimeE->GetMaximumBin());
    double min = BinCenter - 20;
    double max = BinCenter + 20;
    TF1 *gfitE = new TF1("EGaussian","gaus",min,max);
    AvHitTimeE->Fit(gfitE,"QR");
    //Double_t amp = gfit->GetParameter(0);
    Double_t Emean = gfitE->GetParameter(1);
    Double_t Esigma = gfitE->GetParameter(2);
    Ecut1 = Emean + 1*Esigma;
    Ecut2 = Emean - 1*Esigma;
    if(verbose){
      cout << "EBin center: " << BinCenter << endl;
      cout << "EMean : " << Emean << " ESigma: " << Esigma << endl;
      cout << "ECut Up = " << Ecut1 << endl;
      cout << "ECut Down = " << Ecut2 << endl;
    }
  }
  //
  // Now loop again to include the cuts on the AverageHitTime
  for(int evtNum = 0; evtNum < totalEvts; evtNum++){
    
    reconTracker.GetEntry(evtNum);
    testBeam.GetEntry(evtNum);

    if(verbose && evtNum%1000 == 0){
      std::cout << "Momentum : " << TestBeamMomentum << endl;
      std::cout << "Angle : " << TestBeamAngle << endl;
      std::cout << "Events Processed : " << evtNum << std::endl;
    }

    C1loC2lo18->Fill(Cerenkov1Lo[18], Cerenkov2Lo[18]);
    C1loC2lo19->Fill(Cerenkov1Lo[19], Cerenkov2Lo[19]);
    hTOF->Fill(TOF[21]);
    hPID->Fill(TestBeamPID);

    if(NReconObject == 1){
      ND::TTrackerECALReconModule::TECALReconObject *ECALrec = (ND::TTrackerECALReconModule::TECALReconObject*)(*ReconObject)[0];
      // Electron selection
      if( (TestBeamPID == 1 || TestBeamPID == 2) && ECALrec->AverageHitTime > Ecut2 && ECALrec->AverageHitTime < Ecut1){
	// PID variables
	ETrShval->Fill(ECALrec->PID_TrShval);
	EAMR->Fill(ECALrec->PID_AMR);
	// EMEnergy Fit Result
	EEMEnergyFit->Fill(ECALrec->EMEnergyFit_Result);
	EEMEnergyFitUn->Fill(ECALrec->EMEnergyFit_Uncertainty);
      }
      // MIP selection
      if( (TestBeamPID == 3 || TestBeamPID == 4) && ECALrec->AverageHitTime > Mcut2 && ECALrec->AverageHitTime < Mcut1 ){
	// PID variables
	MTrShval->Fill(ECALrec->PID_TrShval);
	MAMR->Fill(ECALrec->PID_AMR);
      }
      // Proton Selection
      //if (TestBeamPID == 5 && ECALrec->AverageHitTime > Pcut2 && ECALrec->AverageHitTime < Pcut1){
      //PTrShval->Fill(ECALrec->PID_TrShval);
      //PAMR->Fill(ECALrec->PID_AMR);
      //}
    }
  } 
  // -----------------------------------------------------------------------------------------
  // Plot histograms
  TCanvas *c1 = new TCanvas("c1", "", 800, 1000);
  c1->Divide(1,3);

  c1->cd(1);
  AvHitTimeE->Draw();

  c1->cd(2);
  AvHitTimeM->Draw();

  c1->cd(3);
  AvHitTimeP->Draw();
  //
  TCanvas *c2 = new TCanvas("c2", "", 800, 1000);
  c2->SetFillColor(10);
  c2->Divide(2,2);
  c2->SetLogz();

  c2->cd(1);
  c2->GetPad(1)->SetLogz();
  C1loC2lo18->Draw("colz");
  
  c2->cd(2);
  c2->GetPad(2)->SetLogz();
  C1loC2lo19->Draw("colz");

  c2->cd(3);
  c2->GetPad(3)->SetLogy();
  hTOF->Draw();

  c2->cd(4);
  hPID->Draw();
  //
  TCanvas *c3 = new TCanvas("c3", "", 800, 1000);
  c3->Divide(2,2);

  c3->cd(1);
  EEMEnergyFit->Draw();

  c3->cd(2);
  EEMEnergyFitUn->Draw();

  c3->cd(3);
  ETrShval->Draw();

  c3->cd(4);
  EAMR->Draw();
  //
  TCanvas *c4 = new TCanvas("c4", "", 800, 1000);
  c4->Divide(1,2);

  c4->cd(1);
  MTrShval->Draw();

  c4->cd(2);
  MAMR->Draw();
  //return; 
}
