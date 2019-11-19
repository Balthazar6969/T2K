#define GOODCURRENTONLY 0

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

#define NBUNCH 8
double time_mc[NBUNCH]={2750.2,3332.0,3914.7,4497.0,5078.4,5659.7,6243.4,6824.2};
double time_data_run1[NBUNCH]={2839.7,3423.5,4005.4,4588.6,5172.2,5754.6,-1000.,-1000.};
double time_data_run2[NBUNCH]={2844.55,3434.65,4021.11,4610.74,5170.98,5760.62,6334.47,6915.37};
double time_data_run3[NBUNCH]={3008.71,3587.54,4170.73,4753.63,5336.29,5917.53,6498.3,7083.16};
double time_data_run4a[NBUNCH]={3014.42,3596.01,4177.71,4759.5 ,5341.69,5923.88,6504.88,7087.07};
double time_data_run4b[NBUNCH]={3004.25,3588.05,4168.84,4749.28,5328.71,5917.91,6496.31,7077.30};
double time_data_run4c[NBUNCH]={3014.69,3600.57,4178.89,4764.24,5342.35,5931.73,6506.37,7090.15};

//bunch histograms
TH1F *h1 = new TH1F("h1","h1",100,2500,7500);
TH1F *bunch1 = new TH1F("bunch1","bunch1",100,2800,3200);
TH1F *bunch2 = new TH1F("bunch2","bunch2",100,3450,3750);
TH1F *bunch3 = new TH1F("bunch3","bunch3",100,4000,4300);
TH1F *bunch4 = new TH1F("bunch4","bunch4",100,4650,4950);
TH1F *bunch5 = new TH1F("bunch5","bunch5",100,5200,5700);
TH1F *bunch6 = new TH1F("bunch6","bunch6",100,5800,6200);
TH1F *bunch7 = new TH1F("bunch7","bunch7",100,6300,6700);
TH1F *bunch8 = new TH1F("bunch8","bunch8",100,6800,7300);


int GetBunch(double tTrack, int run, bool isMC, double sigma = 15., double NSigmas = 4.0)
{
    int bunch = -1;
    //  double sigma;
    double time;

    for (int i=0;i<NBUNCH;i++) {
        if (isMC) {
            time = time_mc[i];
            //sigma = 15.;
        }
        else {
            if (run < 6000)
                time = time_data_run1[i];
            else if (run < 7000)
                time = time_data_run2[i];
            else
                time = time_data_run3[i];
            //sigma = 15.;
        }

        if( time < 0 ) return bunch;

        if (fabs(tTrack-time)<sigma*NSigmas) {
            bunch = i;
            break;
        }
    }

    return bunch;
}

//______________________________________________________________________________________

void beam(char *filename, int linecount,char *output)
{
    // file name should be a text file containing one or several root files (one per line)
    // linecount - number of files to load
    // Load some important libraries.
    static int first = 0;
    Int_t local_run_flag = -1;
    Int_t current_run_flag = -1;
    Double_t POTs(0);

    gROOT->SetBatch(1);

    if (first == 0)
    {
        first = 1;
        gSystem->AddIncludePath("-I$OAANALYSISROOT/src");
        gSystem->AddIncludePath("-I.");

        gSystem->Load("libPhysics");
        gSystem->Load("libGeom");
        gSystem->Load("libTree");
        gSystem->Load("libMatrix");
        gSystem->Load("libMinuit");
        gSystem->Load("libCLHEP.so");
        gSystem->Load("librecpack.so");
        gSystem->Load("liboaEvent.so");
        gSystem->Load("liboaRuntimeParameters.so");
        gSystem->Load("libEG.so");
        gSystem->Load("liboaOfflineDatabase.so");
        gSystem->Load("liboaUtility.so");
        gSystem->Load("libBeamData.so");
        gSystem->Load("liboaBeamData.so");
        gSystem->Load("liboaAnalysis.so");
    }

    TChain reconGlobal = TChain("ReconDir/Global");
    TChain header = TChain("HeaderDir/BasicHeader");
    TChain BasicDataQuality = TChain("HeaderDir/BasicDataQuality");
    TChain BeamSummaryData("HeaderDir/BeamSummaryData");

// _____________________________________________________________________________________
// Load data files 
    TString fileName(filename);
 
    ifstream inputFile(fileName.Data(), ios::in);
    if (!inputFile)
    {
        cerr << "Cannot open input file list '" << fileName.Data() << "'." << endl;
        return;
    }

    string inputString;
    ifstream ifile;
    int count = 0;
    while (inputFile >> inputString)
    {
        ifile.open(inputString.c_str());
        if (ifile.is_open())
        {
            ifile.close();
            std::cout << "Adding file: " << inputString << std::endl;
            reconGlobal.AddFile(inputString.c_str());
            header.AddFile(inputString.c_str());
            BasicDataQuality.AddFile(inputString.c_str());
            BeamSummaryData.AddFile(inputString.c_str());

            count++;
            if (count == linecount) break;
        }
        else
        {
            std::cout << "Can't open: " << inputString << std::endl;
        }
    }
    std::cout << count << " files opened." << std::endl;
// _____________________________________________________________________________________
// Prepare to read trees.
//
    Int_t NPIDs;
    Int_t NTPC;

    TClonesArray *PIDs = new TClonesArray("ND::TGlobalReconModule::TGlobalPID",20);
    reconGlobal.SetBranchAddress("NPIDs",  &NPIDs);
    reconGlobal.SetBranchAddress("NTPC",   &NTPC);
    reconGlobal.SetBranchAddress("PIDs",   &PIDs);
    //    
    Int_t DQEventID;
    Int_t DQRunID;
    Int_t ND280OffFlag;
    BasicDataQuality.SetBranchAddress("EventID", &DQEventID);                                  
    BasicDataQuality.SetBranchAddress("RunID", &DQRunID);                                  
    BasicDataQuality.SetBranchAddress("ND280OffFlag", &ND280OffFlag);
    
    //Int_t SpillNumber;
    Int_t ND280Spill;
    Int_t BRunID;
    Int_t BSubrunID;
    Int_t BEventID;
    Int_t BeamSummaryDataStatus;

    BeamSummaryData.SetBranchAddress("RunID",&BRunID);
    BeamSummaryData.SetBranchAddress("SubrunID",&BSubrunID);
    BeamSummaryData.SetBranchAddress("EventID",&BEventID);
    BeamSummaryData.SetBranchAddress("BeamSummaryDataStatus",&BeamSummaryDataStatus);
    BeamSummaryData.SetBranchAddress("ND280Spill", &ND280Spill);
    TClonesArray *TClonesBSD = new TClonesArray("ND::TBeamSummaryDataModule::TBeamSummaryData");
    BeamSummaryData.SetBranchAddress("BeamSummaryData",&TClonesBSD);
    ND::TBeamSummaryDataModule::TBeamSummaryData *bsd=(ND::TBeamSummaryDataModule::TBeamSummaryData*)(*TClonesBSD)[0];

    Int_t eventnumber, runnumber, subrun;
    UInt_t eventtime;
    header.SetBranchAddress("RunID",    &runnumber);
    header.SetBranchAddress("EventID",  &eventnumber);
    header.SetBranchAddress("SubrunID", &subrun);
    header.SetBranchAddress("EventTime",&eventtime);

    int good_event;

// __________________________________________________________________________
// Loop over all events.

    int entries = reconGlobal.GetEntries();
    std::cout << "Number of entries reconGlobal: " << entries << std::endl;
 
//------------------------------------------------
//-------------------- Start analysis
//------------------------------------------------

    Int_t eveTot(0), eveBeam(0), eveDQ(0);

    for (int i = 0; i < entries; i++)
    {
        reconGlobal.GetEntry(i);
        header.GetEntry(i);
        BasicDataQuality.GetEntry(i);
        BeamSummaryData.GetEntry(i);
        good_event = 0;
#if GOODCURRENTONLY
        if (BeamSummaryDataStatus == 1 && (Int_t (bsd->GoodSpillFlag)) != 1)
            continue;
#endif
        if ( ND280OffFlag == 0 && BeamSummaryDataStatus == 1 && (Int_t (bsd->GoodSpillFlag)))
        {
            POTs += bsd->CT5ProtonsPerSpill;
            eveTot += 1;
            eveBeam += 1;
            eveDQ += 1;
            good_event=1;
        }

        current_run_flag = Int_t (bsd->GoodSpillFlag);
        if (/*current_run_flag &&*/ local_run_flag != current_run_flag)
        {
            local_run_flag = current_run_flag;
            std::cout << "Beam Run number " << bsd->BeamRunNumber << " ND280 Run " << BRunID << " ND280 subrun " << BSubrunID <<" has beam flag " << local_run_flag << std::endl;
        }
	if(bsd->GoodSpillFlag == -1){
	  for( int j=0; j<NPIDs; j++) {
	    ND::TGlobalReconModule::TGlobalPID *globalTrack = (ND::TGlobalReconModule::TGlobalPID*)(*PIDs)[j];
	    //std::cout<<"TPC tracks: "<<globalTrack->NTPCs<<" Tracker tracks: "<<globalTrack->NTRACKERs<<std::endl;
	    if( globalTrack->NTPCs != 0 &&  globalTrack->NTRACKERs != 0 ){
	      //std::cout<<"T): "<<globalTrack->FrontPosition.T()<<std::endl;

	      h1->Fill(globalTrack->FrontPosition.T());
	      bunch1->Fill(globalTrack->FrontPosition.T());
	      bunch2->Fill(globalTrack->FrontPosition.T());
	      bunch3->Fill(globalTrack->FrontPosition.T());
	      bunch4->Fill(globalTrack->FrontPosition.T());
	      bunch5->Fill(globalTrack->FrontPosition.T());
	      bunch6->Fill(globalTrack->FrontPosition.T());
	      bunch7->Fill(globalTrack->FrontPosition.T());
	      bunch8->Fill(globalTrack->FrontPosition.T());
	    }
	  }
	}

        if ((entries/20)!=0)
        {
            if ((i)%(entries/20) == 0)
                std::cout << "Events Processed " << (100*i)/entries << "% " << std::endl;
        }
        else
           std::cout << "Events Processed " << (100*i)/entries <<  "% " << std::endl;

    }; //end loop entries
    //gStyle->SetOptStat(1111110);
    //gStyle->SetOptFit(111);
    //TCanvas *c1 = new TCanvas("c1","c1",1);
    TFile *f=new TFile(output,"new");
    h1->Write();
    //bunch1->Fit("gaus");
    bunch1->Write();
    //bunch2->Fit("gaus");
    bunch2->Write();
    //bunch3->Fit("gaus");
    bunch3->Write();
    //bunch4->Fit("gaus");
    bunch4->Write();
    //bunch5->Fit("gaus");
    bunch5->Write();
    //bunch6->Fit("gaus");
    bunch6->Write();
    //bunch7->Fit("gaus");
    bunch7->Write();
    //bunch8->Fit("gaus");
    bunch8->Write();
    f->Close();
    std::cout<<" End of Run "<<std::endl;

//------------------------------------------
// save results
//------------------------------------------
    std::cout << "Number of entries : " << entries << std::endl;

    std::cout << "------------------------------------------------------" << std::endl;
    std::cout << ">>>> entries            : " << entries << std::endl;
    std::cout << ">>>> Total POTs         : " << POTs << std::endl;
#if GOODCURRENTONLY
    std::cout << ">>>> (only 250kA)\n";
#endif
    std::cout << "------------------------------------------------------" << std::endl;
}
