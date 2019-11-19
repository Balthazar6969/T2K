#include <iostream>
#include <vector>
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


// #include "/T2K/nd/oaEvent/v6r1/src/EoaCore.hxx"
// #include "/T2K/nd/oaEvent/v6r1/src/ToaEvent.hxx"
// #include "/T2K/nd/oaEvent/v6r1/src/TND280Event.hxx"
// #include "../src/EoaAnalysis.hxx"
// #include "../src/oaAnalysis_version.h"
// #include "../src/TTruthVerticesModule.hxx"
// #include "../src/TTrackerReconModule.hxx"
// #include "../src/TAnalysisHeaderModuleBase.hxx"
// #include <AnalysisTools/TNEUTAnalysisUtils.hxx>
// #include <AnalysisTools/TTrackerAnalysisUtils.hxx>
// #include <AnalysisTools/TTrackerAnalysisUtils.cxx>
// #include <AnalysisTools/TCCtruthAnalysisUtils.hxx>
// #include <AnalysisTools/TCCtruthAnalysisUtils.cxx>
// #include <AnalysisTools/TCCreconAnalysisUtils.hxx>
// #include <AnalysisTools/TCCreconAnalysisUtils.cxx>
// #include <AnalysisTools/TCCweightMA.hxx>
// #include <AnalysisTools/TCCplotAnalysisUtils.hxx>
// #include <AnalysisTools/TCCplotAnalysisUtils.cxx>


//-------------------------------------------------------------------------------------------------
//
// To run this code:
// root
// .x ccAnalysis(1.1,1) //1.1=axial mass, 1=if you don't want to see the canvas( nb of canvas=1000)
//
//if it does not work delete project headers in TNEUT* and TVNEUT*
//--------------------------------------------------------------------------------------------------
using namespace std;
void cc1TrackAnalysis(){
       bool batch=1;
       double MA=1.1;

       gROOT->SetBatch(1);

//void ccAnalysis(double MA,bool batch){
    //Load some important libraries.
  
    gSystem->AddIncludePath("-I$OAANALYSISROOT/src");
    gSystem->AddIncludePath("-I.");
    
    //   gSystem->Load("libPhysics.so");
    gSystem->Load("libGeom.so");
    gSystem->Load("libTree.so");
    gSystem->Load("libMatrix.so");
    gSystem->Load("libGui.so");
    gSystem->Load("libCint.so");
    gSystem->Load("libCore.so");
    gSystem->Load("liboaEvent.so");
    gSystem->Load("liboaAnalysis.so");
   
    gROOT->ProcessLine(".L AnalysisTools/TNEUTAnalysisUtils.cxx");
    gROOT->ProcessLine(".L AnalysisTools/TTrackerAnalysisUtils.cxx");
    gROOT->ProcessLine(".L AnalysisTools/TCCtruthAnalysisUtils.cxx");
    gROOT->ProcessLine(".L AnalysisTools/TCCreconAnalysisUtils.cxx");
    gROOT->ProcessLine(".L AnalysisTools/TCCweightMA.cxx");
    gROOT->ProcessLine(".L AnalysisTools/TCCplotAnalysisUtils.cxx");
   
    TChain mcTruthVertices = TChain("TruthDir/Vertices");
    TChain mcTruthTrajectories = TChain("TruthDir/Trajectories");
    TChain reconTracker = TChain("ReconDir/Tracker");

    // _____________________________________________________________________________________
    // Load data files
    TString fileName("ccfgdtpc_input_files.txt");
//    TString fileName("cc_input_files.txt");
//    TString fileName("test_input_file.txt");

    //    TString fileName("testfile.txt");
    ifstream inputFile(fileName.Data(), ios::in);
  
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
    }    
    // _____________________________________________________________________________________
    // Prepare to read trees.
    
    // Set the branches for the truth information.
    Int_t NVtxFGD1; 
    Int_t NVtxFGD2;
    TClonesArray *VtxFGD1 = new TClonesArray("ND::TTruthVerticesModule::TTruthVertex", 20);
    TClonesArray *VtxFGD2 = new TClonesArray("ND::TTruthVerticesModule::TTruthVertex", 20);

    cout<<" set branches adresses "<<endl;
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
    TClonesArray *Vertices = new TClonesArray("ND::TTrackerReconModule::TTrackerVertex", 20);
    TClonesArray *Tracks = new TClonesArray("ND::TTrackerReconModule::TTrackerTrack", 20);
    TClonesArray *DelayedClusters = new TClonesArray("ND::TTrackerReconModule::TFgdCluster", 20);
  
    reconTracker.SetBranchAddress("NVertices", &NVertices);
    reconTracker.SetBranchAddress("NTracks", &NTracks);
    reconTracker.SetBranchAddress("NDelayedClusters", &NDelayedClusters);
    reconTracker.SetBranchAddress("Vertices", &Vertices);
    reconTracker.SetBranchAddress("Tracks", &Tracks);
    reconTracker.SetBranchAddress("DelayedClusters", &DelayedClusters);    
    reconTracker.Print("toponly");
   

    //for systematics
    //-------------------
    char fname[256];
    int PARCUT=NPARTICLECUT-1;
    double curvcut=3;
    sprintf(fname,"cc1TrackAnalysis_output_Curv%.2f-Cut%d-MA%.1f.root",curvcut,PARCUT,MA);
//    sprintf(fname,"test.root",curvcut,PARCUT,MA);
    TFile *rootfile_output=new TFile(fname,"RECREATE");
    TTrackerAnalysisUtils *AnTUtils=new TTrackerAnalysisUtils();
    TCCtruthAnalysisUtils *trueAn=new TCCtruthAnalysisUtils();
    TCCreconAnalysisUtils *recAn=new TCCreconAnalysisUtils();

    TCCweightMA *weightMA=new TCCweightMA(1.1);
    TNEUTAnalysisUtils *NeutUtils=new TNEUTAnalysisUtils();

    int countprob=0;
    //initialize the name of the different reaction, subreaction
    // or define the systematics
    //*********************************************************
    
    //0  1   2   3
    double systematics[CCNCSUBNREACTION]={1,0.3,0.2,0.3};  
    int isreaction[NREACTION];
    int isccsubreaction[CCNCSUBNREACTION];
   
    //initialize plots and counters
    //*****************************
    double TotalInFV[PHASESPACE]={0,0};
    
    char paramName[7][50];
    char paramState[2][5];

    sprintf(paramName[0],"Q2");
    sprintf(paramName[1],"Enu");
    sprintf(paramName[2],"CosThmu");
    sprintf(paramName[3],"Pmu");
    sprintf(paramName[4],"VtxZ");
    sprintf(paramName[5],"VtxX");
    sprintf(paramName[6],"VtxY");
  
    
    sprintf(paramState[0],"True");
    sprintf(paramState[1],"Rec"); 
    
    TCCplotAnalysisUtils *param[2][5];
    TCCplotAnalysisUtils *param2D[2][15];//15=5+4+3+2+1
     
    for(int istate=0;istate<2;istate++){
        for(int ipar=0;ipar<5;ipar++){
            param[istate][ipar]=new TCCplotAnalysisUtils(rootfile_output);
            param[istate][ipar]->InitParH1D(paramState[istate],paramName[ipar]);
            param[istate][ipar]->InitEffPurvsParH1D(paramState[istate],paramName[ipar]);

            //for inclusive cc study the systematic (irea=0)
            param[istate][ipar]->InitSysH1D(0,paramState[istate],paramName[ipar]);
            param[istate][ipar]->InitEffPurvsParSysH1D(0,paramState[istate],paramName[ipar]);
        }
    }
    
    TCCplotAnalysisUtils *diffparam_True[NREACTION][5][4];
    TCCplotAnalysisUtils *diffparam_SelTrue[NREACTION][NCUTS][NREACTION][5][4];
    TCCplotAnalysisUtils *diffparam_Sel[NREACTION][NCUTS][5][4];
    char eventState[50];
    std::string reaction[NREACTION];
    for(int irea=0;irea<NREACTION;irea++){
        reaction[irea]=param[0][0]->GetReactionName(irea);
    }

    for(int ipar=0;ipar<5;ipar++){
        for(int ipar2=0;ipar2<5;ipar2++){
            if(ipar2!=ipar){
                for(int irea=0;irea<NREACTION;irea++){
                    diffparam_True[irea][ipar][ipar2]=new TCCplotAnalysisUtils(rootfile_output);
                    sprintf(eventState,"True%s",reaction[irea].c_str());
                    diffparam_True[irea][ipar][ipar2]->InitDiffPar1Par2TOT("True",paramName[ipar],"Rec",paramName[ipar],"True",paramName[ipar2],eventState);
                    //     diffparam_True[irea][ipar]->InitDiffPar1Par2TOT("True",paramName[ipar],"Rec",paramName[ipar],eventState);
                    int icut=NCUTS-1;
                    
                    diffparam_Sel[irea][icut][ipar][ipar2]=new TCCplotAnalysisUtils(rootfile_output);
                    sprintf(eventState,"Sel%s_Cut%d",reaction[irea].c_str(),icut);
                    diffparam_Sel[irea][icut][ipar][ipar2]->InitDiffPar1Par2TOT("True",paramName[ipar],"Rec",paramName[ipar],"True",paramName[ipar2],eventState);
                    for(int itrrea=0;itrrea<NREACTION;itrrea++){
                        diffparam_SelTrue[irea][icut][itrrea][ipar][ipar2]=new TCCplotAnalysisUtils(rootfile_output);
                        sprintf(eventState,"Sel%sTrue%s_Cut%d",reaction[irea].c_str(),reaction[itrrea].c_str(),icut);
                        diffparam_SelTrue[irea][icut][itrrea][ipar][ipar2]->InitDiffPar1Par2TOT("True",paramName[ipar],"Rec",paramName[ipar],"True",paramName[ipar2],eventState);
                    }
                }
            }
        }
    }
    //(Q2,Q2)
    //(Q2,Enu) 
    //(Q2,CosThmu)
    //(Q2,Pmu)
    //(Q2,VtxZ)
    //(Enu,Enu) 
    //(Enu,CosThmu) 
    //(Enu,Pmu)
    //(Enu,VtxZ)
    //(CosThmu,CosThmu)
    //(CosThmu,Pmu)
    //(CosThmu,VtxZ)
    //(Pmu,Pmu)
    //(Pmu,VtxZ)
    //(VtxZ,VtxZ)
    for(int istate=0;istate<2;istate++){
        int iter=0;
        for(int ipar=0;ipar<5;ipar++){
            for(int ipar2=ipar;ipar2<5;ipar2++){
                param2D[istate][iter]=new TCCplotAnalysisUtils(rootfile_output);
                param2D[istate][iter]->InitParH2D(paramState[istate],paramName[ipar],paramState[istate],paramName[ipar2]);
                param2D[istate][iter]->InitEffPurvsParH2D(paramState[istate],paramName[ipar],paramState[istate],paramName[ipar2]);
                iter++;
            }
        }
    }

    char pidParName[10][50];
    sprintf(pidParName[0],"Prob");
    sprintf(pidParName[1],"LikeLHD");
    sprintf(pidParName[2],"Pull");
    sprintf(pidParName[3],"Sigma");
    sprintf(pidParName[4],"dEdxexp");

    char kinParName[8][50];
  
    sprintf(kinParName[0],"dEdxmeas");
    sprintf(kinParName[1],"NNodes");
    sprintf(kinParName[2],"MomErr");
    sprintf(kinParName[3],"SigMom");
    sprintf(kinParName[4],"Mom");
    sprintf(kinParName[5],"CosTh");
    sprintf(kinParName[6],"Mom");
    sprintf(kinParName[7],"CosTh");
    
    TCCplotAnalysisUtils *particleHypH1D[5];
    TCCplotAnalysisUtils *particleKinH1D[8];
    TCCplotAnalysisUtils *particleHypH2D[40];///8*5
    TCCplotAnalysisUtils *particleKinH2D[28];//7+6+5+4+3+2+1
    for(int ipar=0;ipar<5;ipar++){
       particleHypH1D[ipar]=new TCCplotAnalysisUtils(rootfile_output);
       particleHypH1D[ipar]->InitParParticleTOTH1D("Rec",pidParName[ipar]);
       for(int ipar2=0;ipar2<6;ipar2++){
           particleHypH2D[ipar*8+ipar2]=new TCCplotAnalysisUtils(rootfile_output);
           particleHypH2D[ipar*8+ipar2]->InitParParticleTOTH2D("Rec",kinParName[ipar2],"Rec",pidParName[ipar]);
       }
       for(int ipar2=6;ipar2<8;ipar2++){
           particleHypH2D[ipar*8+ipar2]=new TCCplotAnalysisUtils(rootfile_output);
           particleHypH2D[ipar*8+ipar2]->InitParParticleTOTH2D("True",kinParName[ipar2],"Rec",pidParName[ipar]);
       }
       particleHypH2D[ipar*8+0]->InitParpParParticleTOTH1D("Rec",pidParName[ipar],"Rec","Mom");
       particleHypH2D[ipar*8+1]->InitParpParParticleTOTH1D("Rec",pidParName[ipar],"Rec","Charge");
       particleHypH2D[ipar*8+2]->InitParpParParticleTOTH1D("Rec",pidParName[ipar],"True","Mom");
       particleHypH2D[ipar*8+3]->InitParpParParticleTOTH1D("Rec",pidParName[ipar],"True","Charge");
    }
    
    int count=0;
    for(int ipar=0;ipar<6;ipar++){
       particleKinH1D[ipar]=new TCCplotAnalysisUtils(rootfile_output);
       particleKinH1D[ipar]->InitParParticleH1D("Rec",kinParName[ipar]);
       particleKinH1D[ipar]->InitParChargeH1D("Rec",kinParName[ipar]);
       particleKinH1D[ipar]->InitEffPurvsParParticleH1D("Rec",kinParName[ipar]);
       particleKinH1D[ipar]->InitEffPurvsParChargeH1D("Rec",kinParName[ipar]);
   
        for(int ipar2=ipar+1;ipar2<6;ipar2++){
            particleKinH2D[count]=new TCCplotAnalysisUtils(rootfile_output);
            particleKinH2D[count]->InitParParticleH2D("Rec",kinParName[ipar2],"Rec",kinParName[ipar]);
            particleKinH2D[count]->InitParChargeH2D("Rec",kinParName[ipar2],"Rec",kinParName[ipar]);
            particleKinH2D[count]->InitEffPurvsParParticleH2D("Rec",kinParName[ipar2],"Rec",kinParName[ipar]);
            particleKinH2D[count]->InitEffPurvsParChargeH2D("Rec",kinParName[ipar2],"Rec",kinParName[ipar]);
            count++;
        }
        for(int ipar2=6;ipar2<8;ipar2++){
            particleKinH2D[count]=new TCCplotAnalysisUtils(rootfile_output);
            particleKinH2D[count]->InitParParticleH2D("True",kinParName[ipar2],"Rec",kinParName[ipar]);
            particleKinH2D[count]->InitParChargeH2D("True",kinParName[ipar2],"Rec",kinParName[ipar]);
            particleKinH2D[count]->InitEffPurvsParParticleH2D("True",kinParName[ipar2],"Rec",kinParName[ipar]);
            particleKinH2D[count]->InitEffPurvsParChargeH2D("True",kinParName[ipar2],"Rec",kinParName[ipar]);
            count++;
        }
    }
    //TOT (0,500MeV),(500,2500),>2500 MeV
    for(int ipar=1;ipar<4;ipar++){
        particleKinH1D[ipar]->InitParticleCounter();
        particleKinH1D[ipar]->InitEffPurFOMvsParticleCut();
    }
    particleKinH1D[0]->InitParticleCounter();
    particleKinH1D[0]->InitEffPurFOMvsParticleCut();
    for(int ipar=6;ipar<8;ipar++){
        particleKinH1D[ipar]=new TCCplotAnalysisUtils(rootfile_output);
        particleKinH1D[ipar]->InitParParticleH1D("True",kinParName[ipar]);
        particleKinH1D[ipar]->InitEffPurvsParParticleH1D("True",kinParName[ipar]);
        particleKinH1D[ipar]->InitParChargeH1D("True",kinParName[ipar]);
        particleKinH1D[ipar]->InitEffPurvsParChargeH1D("True",kinParName[ipar]);
        for(int ipar2=ipar+1;ipar2<8;ipar2++){
            particleKinH2D[count]=new TCCplotAnalysisUtils(rootfile_output);
            particleKinH2D[count]->InitParParticleH2D("True",kinParName[ipar2],"True",kinParName[ipar]);
            particleKinH2D[count]->InitParChargeH2D("True",kinParName[ipar2],"True",kinParName[ipar]);
            particleKinH2D[count]->InitEffPurvsParParticleH2D("True",kinParName[ipar2],"True",kinParName[ipar]);
            particleKinH2D[count]->InitEffPurvsParChargeH2D("True",kinParName[ipar2],"True",kinParName[ipar]);
            count++;
        }
    }
   
    TCCplotAnalysisUtils *counter=new TCCplotAnalysisUtils(rootfile_output);
    counter->InitReaCounter();
    counter->InitSysReaCounter();
  

    //HighDeltaThetap
    //*******************
    int TotNumberHighDeltaThetap[NREACTION];
    int TotNumberLowDeltaThetap[NREACTION];
    for(int irea=0;irea<NREACTION;irea++)
    {
        TotNumberHighDeltaThetap[irea]=0;
        TotNumberLowDeltaThetap[irea]=0;
    }

// Proportion of charged track
    int nTPCFGD[NREACTION][5][5];//0,1,2,3,>3
    for(int irea=0;irea<NREACTION;irea++){
        for(int i=0;i<5;i++){
            for(int j=0;j<5;j++){
                nTPCFGD[irea][i][j]=0;
            }
        }
    }
    
    // Proportion of charged track
    //*****************************
    //0,1,>1
    int chargedTrack[3][3][NREACTION];
    int chargedTrackFV[3][3][NREACTION];
    int chargedTrackTopo[3][3][6561][NREACTION];
    int chargedTrackTopoFV[3][3][6561][NREACTION];

    int chargedTraj[3][3][NREACTION];
    int chargedTrajFV[3][3][NREACTION];
    int chargedTrajTopo[3][3][6561][NREACTION];
    int chargedTrajTopoFV[3][3][6561][NREACTION];
    
    for(int irea=0;irea<NREACTION;irea++){
        for(int ineg=0;ineg<3;ineg++){
            for(int ipos=0;ipos<3;ipos++){
                chargedTrack[ineg][ipos][irea]=0;
                chargedTrackFV[ineg][ipos][irea]=0;
                chargedTraj[ineg][ipos][irea]=0;
                chargedTrajFV[ineg][ipos][irea]=0;
                for(int ipar=0;ipar<6561;ipar++){
                    chargedTrackTopo[ineg][ipos][ipar][irea]=0;
                    chargedTrackTopoFV[ineg][ipos][ipar][irea]=0;
                    chargedTrajTopo[ineg][ipos][ipar][irea]=0;
                    chargedTrajTopoFV[ineg][ipos][ipar][irea]=0;
                }
            }
        }
    }
  

    // Vertex
    //*****************************
    int trueVtxFV=0;
    int NotrueVtxFV=0;
    int NotrueYesrecFV=0;
    int NotrueNorecFV=0;
    int YestrueYesrecFV=0;
    int YestrueNorecFV=0;

    int recVtxFV=0;
    int NorecVtxFV=0;
    int NorecYestrueFV=0;
    int NorecNotrueFV=0;
    int YesrecNotrueFV=0;
    int YesrecYestrueFV=0;
    int recVtxTotal=0;

    int michel=0;
    int michelccqe=0;
    int michelccqeHdThp=0;
    int michelccnqe=0;
    int michelccnqeHdThp=0;

    int iter2=0;
    int notrackcounter=0;
    // Loop over all events.
    // ****************************
    TStopwatch timer;
    timer.Start();
    int entries = mcTruthVertices.GetEntries(); 
    std::cout << "ent: " << entries << std::endl;


//     for(int ient = 0; ient <  10000; ient++)

    
      for(int ient = 0; ient < entries; ient++)
//    for(int ient = 0; ient <20000;ient++)
          //     for(int ient = 0; ient <1000;ient++)
    {
   
        mcTruthVertices.GetEntry(ient);
        reconTracker.GetEntry(ient);
          if(ient%500 == 0)
        {
            std::cout <<  "********************************" << std::endl;
            std::cout <<  "    !!!Events Processed " << ient << std::endl;
            timer.Stop();
            std::cout << " Processing time: " << timer.CpuTime() << " " << timer.RealTime() << std::endl;
            std::cout <<  "********************************" << std::endl;
            timer.Start();
        }
        
        // Look at the true vertex and copy the array info in a TTruthVertex
        // *****************************************************************
        bool isfgd1=false;
        bool isfgd2=false;
        ND::TTruthVerticesModule::TTruthVertex *true_fgd_vertex;
        if(NVtxFGD1 >0){
            true_fgd_vertex = (ND::TTruthVerticesModule::TTruthVertex*) VtxFGD1->UncheckedAt(0);
            isfgd2=false;
            isfgd1=true;
        }
        else if (NVtxFGD2 >0){
            true_fgd_vertex = (ND::TTruthVerticesModule::TTruthVertex*) VtxFGD2->UncheckedAt(0);
            isfgd2=true;
            isfgd1=false;
        }
        else{
            std::cerr << "There should be at least one vertex in one FGD.  Skipping event."<<std::endl;
            continue;
        }
        TLorentzVector TrueVtx;
        TrueVtx = true_fgd_vertex->Vertex;

        recAn->SetTrueVertexPos(TrueVtx);
        TVector3 vtxtrue(TrueVtx.X(),TrueVtx.Y(),TrueVtx.Z());

            
        // Get the numbers of each particle associated to the vertex
        // => for topology study
        // *****************************************************************
        int totfinalpart=trueAn->GetTotalFinalParticle(true_fgd_vertex);
        int muonNumber=trueAn->kmuonNumber;
        int electronNumber=trueAn->kelectronNumber;
        int protonNumber=trueAn->kprotonNumber;
        int piplusNumber=trueAn->kpiplusNumber;
        int piminusNumber=trueAn->kpiplusNumber;
        int NTrajCharged=trueAn->GetTotalChargedFinalParticle(true_fgd_vertex);
        
        //      True kinematic of the different particles
        // *****************************************************************
        // MUON_OUT (for CC)
        // ****************
        TLorentzVector muon_momentum;
        int leptonSize=true_fgd_vertex->LeptonIDs.size();
        if(leptonSize>0){
            muon_momentum=trueAn->GetChargedLeptonMaxMomentum(true_fgd_vertex,13);
        }
        double mmu=muon_momentum.Mag();
        double emu=muon_momentum.T();
        double pmu=sqrt(emu*emu-mmu*mmu);
        double costhmu=trueAn->GetTrueCosThetamu(muon_momentum);//if mu don't exist=> cos=9
               
        // NU_OUT (for NC)
        // ***************
        TLorentzVector nuOUT_momentum;
        int neutralSize=true_fgd_vertex->OtherNeutralIDs.size();
        if(neutralSize>0){
            nuOUT_momentum=trueAn->GetOtherNeutralMaxMomentum(true_fgd_vertex,14);
            if(nuOUT_momentum.T()==0)
                nuOUT_momentum=trueAn->GetOtherNeutralMaxMomentum(true_fgd_vertex,12);
            if(nuOUT_momentum.T()==0)
                nuOUT_momentum=trueAn->GetOtherNeutralMaxMomentum(true_fgd_vertex,16);
        }
        
        double enuOUT=nuOUT_momentum.T();
        double costhnuOUT=trueAn->GetTrueCosThetamu(nuOUT_momentum);//the way for the neutrino is exactly the same...
        
        // PROTON (for NC or CC)
        // *********************
        double ep=trueAn->GetTrueProtonEnergy(true_fgd_vertex);

        // NU_MU
        // ********************
        TLorentzVector neutrino_momentum = true_fgd_vertex->MomNu;
        double enu=neutrino_momentum.T();

        // NUCLEON_TARGET (p or n)
        // ***********************
        TLorentzVector nucleon_target_momentum =true_fgd_vertex->MomTarget;
        TVector3  nucleon_target_mom=TVector3(nucleon_target_momentum.X(),nucleon_target_momentum.Y(),nucleon_target_momentum.Z());
        double nTargetMom2=nucleon_target_mom.Mag2();

        // Q2 (MeV^2)
        // = (Pmu-Pnu)^2     for CC
        // = (PnuOUT-Pnu)^2  for NC
        // ************************
        double q2=0;
        if(leptonSize>0){
            q2=trueAn->GetQ2(neutrino_momentum,muon_momentum);
        }
        else{
            q2=trueAn->GetQ2(neutrino_momentum,nuOUT_momentum);
        }
        
        if(pmu==0){//when NC use enuOUT 
            pmu=enuOUT;
            costhmu=costhnuOUT;
        }
        bool truebox=false;
        if(pmu<1000 && costhmu>0.5)
            truebox=true;

        // check reaction corresponding to the vertex
        // 1: is TrueVtx in FV ? if not => next event
        // 1: CC must have a MUON, NC must have a NU. If not => BUG in NEUT => pass to next event
        // 2: check the reaction of the event 0: ccAll 1:ccqe 2:ccnqe 3:ccppi+ 4:ncAll
        // 3: if CC check what is the sub reaction 0:coh-pi 1:ccDIS 2:ccnqe 3:cc/nc
        // **************************************************************************************
        bool isfid=false;
        double rxn_code=999;
    
        isfid=AnTUtils->IsInFiducialVolume(vtxtrue);       
        // if(isfid && (enuOUT!=0 || pmu!=0) ){
        if(isfid ){
            rxn_code=NeutUtils->GetReactionCode(true_fgd_vertex);
            for(int irea=0;irea<NREACTION;irea++){
                isreaction[irea]=0;
                isreaction[irea]=(int)trueAn->IsReaction((int)rxn_code,irea);
            }
            for(int isub=0;isub<CCNCSUBNREACTION;isub++){
                isccsubreaction[isub]=0;
                isccsubreaction[isub]=(int)trueAn->IsCCNCsubReaction((int)rxn_code,isub);
            }
        }


        // Now Start Selection
        // **************************************************************************************
        double DeltaThetaP=999;
        double DeltaEnergyP=99999;
        int kTotalTPC=0;
        int kTotalFGD=0;
        bool oneAn=false;
        bool highDeltaThetap=false;
        recAn->ResetCutFlag();
        kTotalTPC=recAn->GetTotalTracksInTPC(Tracks,NTracks);
      
       

        // 1: - For 1TrackAnalysis:
        //      look if there is 1 negative track in the fiducial volume
        //    - For >2TrackAnalysis:
        //      look if there is only and only  1 negative track and the rest of the tracks should be positive
        // 2: All the tracks have to be in the fiducial volume
        // 3: Look for the existence of a Michel electron
        // 4: For 1neg and 1pos track:
        //    - look for the delta theta_p, if < 0.45 => ccqe
        //                                  if > 0.45 => ccnqe
        //    - look for delta ep
        // 5: Define the box region in costhmu and pmu where efficiencies between ND and SK are similar   
        // *****************************************************************************************************
        //momentum defined if: momerr/mom<0.8
        if(isfid){
        recAn->SetCurvParticleCut(curvcut,PARCUT);
        double trueINFO[4]={rxn_code,vtxtrue.X(),vtxtrue.Y(),vtxtrue.Z()};
        if(NTracks == 1 && kTotalTPC==1 )
            oneAn=recAn->OneTrackAnalysis(Tracks,ient);
        
        else if(NTracks >= 2 && kTotalTPC > 0 ){
            recAn->MultipleTracksAnalysis(Tracks, NTracks, Vertices, NVertices,ient);
        }

        if(NDelayedClusters>0)
            recAn->HasMichelElectron(DelayedClusters, NDelayedClusters);

        if(recAn->fNumberTpcTr_True[0]>1 ){
            countprob++;
            continue;
        }
    
        if(!recAn->fPassedVertexCut ){
            if(kTotalTPC==0){
                notrackcounter++;
            }
        }
        
        if((NTracks == 1 && kTotalTPC==1) || (NTracks >= 2 && kTotalTPC > 0))
        {       
            recAn->FillHistoKin(trueINFO,particleKinH1D,particleKinH2D);
            recAn->FillHistoHyp(trueINFO,particleHypH1D,particleHypH2D);
        }
        DeltaThetaP=recAn->GetDeltaThetaP();
        highDeltaThetap=recAn->HighDeltaThetaP(DeltaThetaP,0.5);
        for(int irea=0;irea<NREACTION;irea++){
            if(isreaction[irea] && DeltaThetaP!=999)
                if(highDeltaThetap){
                    TotNumberHighDeltaThetap[irea]++;
                }
                else{
                    TotNumberLowDeltaThetap[irea]++;
                }
        }
        DeltaEnergyP=recAn->GetDeltaEnergyP(false);//false=ishigh delta theta p..
//        recAn->DefineRecCosThmuRecPmuRange(0.5,1,0,1000); 
     
        // Study if we have true vtx in FV if reco vtx is also in FV
        // Study if we have reco vtx in FV if true vtx is also in FV
        // ********************************************************
        double vtxXrec=(recAn->fReconVertexPos).X();
        double vtxYrec=(recAn->fReconVertexPos).Y();
        double vtxZrec=(recAn->fReconVertexPos).Z();
        TVector3 vtxrec(vtxXrec,vtxYrec,vtxZrec);
        
        if( kTotalTPC>=1)
             cout<<"ient "<<ient<<" rec "<<vtxZrec<<" true "<<vtxtrue.Z()<<endl;
        bool istrueFV=false;
        bool isrecFV=false;
        istrueFV=AnTUtils->IsInFiducialVolume(vtxtrue);
        isrecFV=AnTUtils->IsInFiducialVolume(vtxrec);
        if(istrueFV){
            trueVtxFV++;
            if(!isrecFV && vtxrec.Mag()!=0 )
                YestrueNorecFV++;
            else if(isrecFV)
                YestrueYesrecFV++;
        }else if(!istrueFV){
            NotrueVtxFV++;
            if(isrecFV)
                NotrueYesrecFV++;
            else if(!isrecFV && vtxrec.Mag()!=0 )
                NotrueNorecFV++;
        }
        
        if(isrecFV){
            recVtxFV++;
            if(istrueFV)
               YesrecYestrueFV++; 
            else if(!istrueFV)
                YesrecNotrueFV++;
        }
        else if(!isrecFV && vtxrec.Mag()!=0){
            NorecVtxFV++;
            if(istrueFV)
                NorecYestrueFV++;
            else if(!istrueFV)
                NorecNotrueFV++;
        }
        if(vtxrec.Mag()!=0)
            recVtxTotal++;
        
        // Study the number of charged track
        // ********************************************************
        int negtrack=0;
        int postrack=0;
        postrack=recAn->fNumberTpcTr_Sel[1][3];
        negtrack=recAn->fNumberTpcTr_Sel[0][3];          
        int nneg=0;
        int npos=0;
        if(negtrack<=1)
            nneg=negtrack;
        else if(negtrack>1)
            nneg=2;
        if(postrack<=1)
            npos=postrack;
        else if(postrack>1)
            npos=2;

        int negtraj=0;
        int postraj=0;
        negtraj=recAn->fNumberTpcTr_True[0]+recAn->fNumberTpcTr_True[3]+
            recAn->fNumberTpcTr_True[4]+recAn->fNumberTpcTr_True[7];
        postraj=recAn->fNumberTpcTr_True[1]+recAn->fNumberTpcTr_True[2]+
            recAn->fNumberTpcTr_True[5]+recAn->fNumberTpcTr_True[6];

        int nnegtr=0;
        int npostr=0;
        if(negtraj<=1)
            nnegtr=negtraj;
        else if(negtraj>1)
            nnegtr=2;
        if(postraj<=1)
            npostr=postraj;
        else if(postraj>1)
            npostr=2;
       
      
        // Study of Michel cluster cut
        // ********************************************************
        if(recAn->fFoundMichelCluster){
            michel++;
            if(isreaction[1])
            {
                michelccqe++;
                if(recAn->fHighDeltaThetap)
                    michelccqeHdThp++;
            }
            else if(isreaction[2])
            {
                michelccnqe++;
                if(recAn->fHighDeltaThetap)
                    michelccnqeHdThp++;
            }
        }

        // Provide the reweight factor depending on MA
        // *******************************************
        double enuGeV=enu/1000;
        double q2GeV=q2/1000000;
        double weight=weightMA->Weight(enuGeV, q2GeV, MA,0);

       //   if(weight!=1)
//              cout<<" q2 "<<q2GeV<<" enuGeV "<<enuGeV<<" weight "<<weight<<" pmu "<<pmu<<endl;
      
        // Provide kinematic reconstructed parameter
        // *******************************************
        double pmurec=recAn->GetRecMuonMomentumMag();       
        double costhmurec=recAn->GetRecMuonCosTheta();
        double enurec[NREACTION];
        double q2rec[NREACTION];
        TLorentzVector muon_momentumRec=recAn->GetRecMuonMomentum();
        double truepar[7]={q2,enu,costhmu,pmu,vtxtrue.X(),vtxtrue.Y(),vtxtrue.Z()};
        for(int irea=0;irea<NREACTION;irea++){
            enurec[irea]= recAn->GetRecNeutrinoEnergy(irea);
            TLorentzVector neutrino_momentumRec=TLorentzVector(0,0,enurec[irea],enurec[irea]);
            q2rec[irea]=trueAn->GetQ2(neutrino_momentumRec,muon_momentumRec);
        }
       
        
        //FILL COUNTERS
        //*******************************
     //    if(pmurec<100 )
//             cout<<" pmurec "<<pmurec<<endl;
  
        int NVtxFGD1=0;
        int NVtxFGD2=0;
        
        int nvtx=0;
        int ntpc=0;
        int nfgd=0;
     
        if(weight>0 && isfid && ((enuOUT!=0 && isreaction[4]) || (isreaction[0] && pmu!=0)))
        {
            kTotalTPC=recAn->fTotalTPC;
            kTotalFGD=recAn->fTotalTPCFGD-kTotalTPC;
              
            if(NVertices>1) nvtx=2;
            else nvtx=NVertices;
            
            if(kTotalTPC>2) ntpc=3;
            else ntpc=kTotalTPC;

            if(kTotalFGD>2) nfgd=3;
            else nfgd=kTotalFGD;
            //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            TotalInFV[0]++;
            if(truebox)
                TotalInFV[1]++;
            {
            for(int irea=0;irea<NREACTION;irea++){
                double recpar[7]={q2rec[irea],enurec[irea],costhmurec,pmurec,vtxrec.Z(),vtxrec.X(),vtxrec.Y()};
                //TRUE
                //*******************************
                if(isreaction[irea]){
                    if(kTotalTPC<=3 && kTotalFGD<=3)
                        nTPCFGD[irea][kTotalTPC][kTotalFGD]++;
                    else if(kTotalTPC>3 && kTotalFGD<=3){
                        nTPCFGD[irea][4][kTotalFGD]++;
                    }
                    else if(kTotalTPC<=3 && kTotalFGD>3){
                        nTPCFGD[irea][kTotalTPC][4]++;
                    }
                    else if(kTotalTPC>3 && kTotalFGD>3){
                        nTPCFGD[irea][4][4]++;
                    }
                    if(kTotalTPC==1)
                        cout<<"))))))))))))))))))))))))) ient "<<ient<<" ktotaltpc "<<kTotalTPC<<" fgd "<<kTotalFGD<<endl;
                    
                    counter->FillTrueReaCounter(irea,truebox, weight);
                    for(int ipar=0;ipar<5;ipar++){
                        param[0][ipar]->FillTrueReaH1D(irea,truepar[ipar], weight);
                        param[1][ipar]->FillTrueReaH1D(irea,recpar[ipar], weight);
                    }
                    int iter=0;
                    for(int ipar=0;ipar<5;ipar++){
                        for(int ipar2=0;ipar2<5;ipar2++){
                            if(ipar!=ipar2){
                                diffparam_True[irea][ipar][ipar2]->FillDiffPar1Par2TOT(truepar[ipar],recpar[ipar],truepar[ipar2]);
                            }
                        }
                        for(int ipar2=ipar;ipar2<5;ipar2++){
                            param2D[0][iter]->FillTrueReaH2D(irea,truepar[ipar],truepar[ipar2],weight);
                            param2D[1][iter]->FillTrueReaH2D(irea,recpar[ipar],recpar[ipar2], weight);
                            iter++;
                        }
                    }
                    //For TrueCC only
                    //***************
                    if(irea==0){
                        for(int isub=0;isub<CCNCSUBNREACTION;isub++){
                            if(isccsubreaction[isub]){
                                //for cc/nc systematic it should never enter here
                                counter->FillTrueSysReaCounter(isub,truebox, weight);
                                for(int ipar=0;ipar<5;ipar++){
                                    param[0][ipar]->FillTrueSysH1D(isub,truepar[ipar],systematics[isub]);
                                    param[1][ipar]->FillTrueSysH1D(isub,recpar[ipar], systematics[isub]);
                                }
                            }
                            else{
                                counter->FillTrueSysReaCounter(isub,truebox,0);
                                for(int ipar=0;ipar<5;ipar++){
                                    param[0][ipar]->FillTrueSysH1D(isub,truepar[ipar],0);
                                    param[1][ipar]->FillTrueSysH1D(isub,recpar[ipar],0);
                                }
                            }
                        }
                    }//end irea=0 (ccAll case)
                }//end loop on is true rea
            }//end loop on true rea
            for(int iselrea=0;iselrea<NREACTION;iselrea++){
                double recpar[7]={q2rec[iselrea],enurec[iselrea],costhmurec,pmurec,vtxrec.Z(),vtxrec.X(),vtxrec.Y()};
                for(int icut=0;icut<NCUTS;icut++){
                    bool iscut=false;
                    iscut=recAn->IsSel1TrackReaction(iselrea,icut);
                    // look for the selected event (S+B)
                    //********************************
                    if(iscut){
                        counter->FillSelReaCounter(iselrea,icut,truebox,weight);
                        for(int ipar=0;ipar<5;ipar++){
                            param[0][ipar]->FillSelReaH1D(iselrea,icut,truepar[ipar], weight);
                            param[1][ipar]->FillSelReaH1D(iselrea,icut,recpar[ipar], weight);
                        }
                      
                        int iter=0;
                        for(int ipar=0;ipar<5;ipar++){
                            for(int ipar2=0;ipar2<5;ipar2++){
                                if(ipar!=ipar2){
                                    if(icut==NCUTS-1)
                                          diffparam_Sel[iselrea][icut][ipar][ipar2]->FillDiffPar1Par2TOT(truepar[ipar],recpar[ipar],truepar[ipar2]);
                                }
                            }
                            for(int ipar2=ipar;ipar2<5;ipar2++){
                                    param2D[0][iter]->FillSelReaH2D(iselrea,icut,truepar[ipar],truepar[ipar2],weight);
                                    param2D[1][iter]->FillSelReaH2D(iselrea,icut,recpar[ipar],recpar[ipar2], weight);
                                    iter++;
                            }
                        }
                        //selected as ccAll
                        //******************
                        if(iselrea==0){
                            for(int isub=0;isub<CCNCSUBNREACTION;isub++){                               
                                if(isccsubreaction[isub]){
                                    counter->FillSelSysReaCounter(isub,icut,truebox,systematics[isub]);
                                    for(int ipar=0;ipar<5;ipar++){
                                        param[0][ipar]->FillSelSysH1D(isub,icut,truepar[ipar],systematics[isub]);
                                        param[1][ipar]->FillSelSysH1D(isub,icut,recpar[ipar], systematics[isub]);
                                    }
                                }
                                else{
                                    counter->FillSelSysReaCounter(isub,icut,truebox,0);
                                    for(int ipar=0;ipar<5;ipar++){
                                        param[0][ipar]->FillSelSysH1D(isub,icut,truepar[ipar],0);
                                        param[1][ipar]->FillSelSysH1D(isub,icut,recpar[ipar],0);
                                    }
                                }
                            }
                        }//end on iselrea=0
                        //look to the selected and true event (S)
                        //****************************************
                        for(int irea=0;irea<NREACTION;irea++){
                            if(isreaction[irea]){
                                counter->FillSelTrueReaCounter(iselrea,icut,irea,truebox, weight);
                                for(int ipar=0;ipar<5;ipar++){
                                    param[0][ipar]->FillSelTrueReaH1D(iselrea,icut,irea,truepar[ipar], weight);
                                    param[1][ipar]->FillSelTrueReaH1D(iselrea,icut,irea,recpar[ipar], weight);
                                }
                                int iter=0;
                                for(int ipar=0;ipar<5;ipar++){
                                    for(int ipar2=0;ipar2<5;ipar2++){
                                        if(ipar!=ipar2){
                                            if(icut==NCUTS-1)
                                                diffparam_SelTrue[iselrea][icut][irea][ipar][ipar2]->FillDiffPar1Par2TOT(truepar[ipar],recpar[ipar],truepar[ipar2]);

                                        }
                                    }
                                    for(int ipar2=ipar;ipar2<5;ipar2++){
                                        param2D[0][iter]->FillSelTrueReaH2D(iselrea,icut,irea,truepar[ipar],truepar[ipar2],weight);
                                        param2D[1][iter]->FillSelTrueReaH2D(iselrea,icut,irea,recpar[ipar],recpar[ipar2], weight);
                                        iter++;
                                    }
                                }
                                //selected and true ccAll
                                //************************
                                if(irea==0 && iselrea==0){
                                    for(int isub=0;isub<CCNCSUBNREACTION;isub++){
                                        if(isccsubreaction[isub]){
                                            counter->FillSelTrueSysReaCounter(isub,icut,truebox,systematics[isub]);
                                            for(int ipar=0;ipar<5;ipar++){
                                                param[0][ipar]->FillSelTrueSysH1D(isub,icut,truepar[ipar],systematics[isub]);
                                                param[1][ipar]->FillSelTrueSysH1D(isub,icut,recpar[ipar], systematics[isub]);
                                            }
                                        }
                                        else{
                                            counter->FillSelTrueSysReaCounter(isub,icut,truebox,0);
                                            for(int ipar=0;ipar<5;ipar++){
                                                param[0][ipar]->FillSelTrueSysH1D(isub,icut,truepar[ipar],0);
                                                param[1][ipar]->FillSelTrueSysH1D(isub,icut,recpar[ipar],0);
                                            }
                                        }
                                    }
                                }//end if irea=iselrea=0
                            }//end isreaction
                        }//end loop irea
                    }//end iscut
                }//end loop icut
            }//end loop iselrea
        }//end weight>0
        }
        }//end isfid
    }//end ientries

     cout<<" for only 1 tpc track "<<endl;
     cout<<" fgd - fgd : "<<recAn->fcount<<endl;
     cout<<" fgd - tpc : "<<recAn->fcount1<<endl;
     cout<<" tpc - fgd : "<<recAn->fcount2<<endl;

     cout<<"**************************************"<<endl;
     cout<<" no track counter "<<notrackcounter<<endl;
      
     for(int irea=0;irea<NREACTION;irea++){
         cout<<" reaction "<<counter->GetReactionName(irea)<<endl;
         for(int itpc=0;itpc<5;itpc++){
             cout<<" "<<itpc<<" TPC TRACKS "<<endl;
             for(int ifgd=0;ifgd<5;ifgd++){
                 cout<<ifgd<<" fgd: "<<nTPCFGD[irea][itpc][ifgd]<<endl;
             }
         }
     }


     
    for(int istate=0;istate<2;istate++){
        for(int ipar=0;ipar<5;ipar++){
            param[istate][ipar]->FillEffPurvsParH1D();
            param[istate][ipar]->FillEffPurvsParSysH1D();
        }
    }
    for(int istate=0;istate<2;istate++){
        for(int ipar=0;ipar<5;ipar++){
            param[istate][ipar]->PlotParH1D(paramState[istate],paramName[ipar],1);
            param[istate][ipar]->PlotParSysH1D(paramState[istate],paramName[ipar],1);
            param[istate][ipar]->PlotEffPurvsParH1D(paramState[istate],paramName[ipar],1);
            param[istate][ipar]->PlotParSysH1D(paramState[istate],paramName[ipar],1);
        }
    }
    for(int istate=0;istate<2;istate++){
        int iter=0;
        for(int ipar=0;ipar<5;ipar++){
            for(int ipar2=ipar+1;ipar2<5;ipar2++){
                if(ipar!=ipar2){
                    param2D[istate][iter]->FillEffPurvsParH2D();
                    iter++;
                }
            }
        }
    }
    for(int istate=0;istate<2;istate++){
        int iter=0;
        for(int ipar=0;ipar<5;ipar++){
            for(int ipar2=ipar;ipar2<5;ipar2++){
                if(ipar!=ipar2){
                    param2D[istate][iter]->PlotEffPurvsParH2D(paramState[istate],paramName[ipar],paramState[istate],paramName[ipar2],1);
                    param2D[istate][iter]->PlotParH2D(paramState[istate],paramName[ipar],paramState[istate],paramName[ipar2],1);
                    iter++;
                }
            }
        }
    }
      
    counter->InitEffPurFOMvsReaCut();
    counter->FillEffPurFOMvsReaCut();
    counter->PlotEffPurFOMvsReaCut();
    
    for(int istate=0;istate<2;istate++){
         for(int ipar=0;ipar<5;ipar++){
             param[istate][ipar]->WriteParH1D(paramState[istate],paramName[ipar]);
             param[istate][ipar]->WriteParSysH1D(paramState[istate],paramName[ipar]);
             param[istate][ipar]->WriteEffPurvsParH1D(paramState[istate],paramName[ipar]);
             param[istate][ipar]->WriteEffPurvsParSysH1D(paramState[istate],paramName[ipar]);
        }
    }
    for(int istate=0;istate<2;istate++){
        int iter=0;
        for(int ipar=0;ipar<5;ipar++){
            for(int ipar2=ipar;ipar2<5;ipar2++){
                param2D[istate][iter]->WriteEffPurvsParH2D(paramState[istate],paramName[ipar],paramState[istate],paramName[ipar2]);
                param2D[istate][iter]->WriteParH2D(paramState[istate],paramName[ipar],paramState[istate],paramName[ipar2]);
                iter++;
            }
        }
    }

    
    
     for(int irea=0;irea<NREACTION;irea++){
         counter->PrintStatvsCut(irea,irea,0);
     }
    
    counter->PrintSysStat(0,NCUTS-1,0);
    counter->PrintEffMatrix(NCUTS-1,0);
    counter->PrintPurMatrix(NCUTS-1,0);
    counter->PrintSelTrueMatrix(NCUTS-1,0);
    counter->PrintTopoFraction(TotalInFV[0],0);
    counter->PrintTopoFraction(TotalInFV[1],1);


    for(int ipar=0;ipar<5;ipar++){
        cout<<"Histograms on particle writing .... "<<ipar<<endl;
        particleHypH2D[ipar*8+0]->PlotParpParParticleTOTH1D("Rec",pidParName[ipar],"Rec","Mom",1);
        particleHypH2D[ipar*8+1]->PlotParpParParticleTOTH1D("Rec",pidParName[ipar],"Rec","Charge",1);
        particleHypH2D[ipar*8+2]->PlotParpParParticleTOTH1D("Rec",pidParName[ipar],"True","Mom",1);
        particleHypH2D[ipar*8+3]->PlotParpParParticleTOTH1D("Rec",pidParName[ipar],"True","Charge",1);
        particleHypH2D[ipar*8+0]->WriteParpParParticleTOTH1D("Rec",pidParName[ipar],"Rec","Mom");
        particleHypH2D[ipar*8+1]->WriteParpParParticleTOTH1D("Rec",pidParName[ipar],"Rec","Charge");
        particleHypH2D[ipar*8+2]->WriteParpParParticleTOTH1D("Rec",pidParName[ipar],"True","Mom");
        particleHypH2D[ipar*8+3]->WriteParpParParticleTOTH1D("Rec",pidParName[ipar],"True","Charge");
        particleHypH1D[ipar]->PlotParParticleTOTH1D("Rec",pidParName[ipar],1);
        particleHypH1D[ipar]->WriteParParticleTOTH1D("Rec",pidParName[ipar]);
        for(int ipar2=0;ipar2<6;ipar2++){
            particleHypH2D[ipar*8+ipar2]->InitParParticleTOTNormH2D("Rec",kinParName[ipar2],"Rec",pidParName[ipar]);
            for(int ipart=0;ipart<NPARTICLEHYP;ipart++){
                particleHypH2D[ipar*8+ipar2]->FillParParticleTOTNormH2D(ipart);
                for(int ipartrue=0;ipartrue<NPARTICLETRUE;ipartrue++){
                    particleHypH2D[ipar*8+ipar2]->FillTrueParParticleTOTNormH2D(ipart,ipartrue);
                }
            }
            particleHypH2D[ipar*8+ipar2]->PlotParParticleTOTNormH2D("Rec",kinParName[ipar2],"Rec",pidParName[ipar],1);
            particleHypH2D[ipar*8+ipar2]->WriteParParticleTOTNormH2D("Rec",kinParName[ipar2],"Rec",pidParName[ipar]);
            particleHypH2D[ipar*8+ipar2]->PlotParParticleTOTH2D("Rec",kinParName[ipar2],"Rec",pidParName[ipar],1);
            particleHypH2D[ipar*8+ipar2]->WriteParParticleTOTH2D("Rec",kinParName[ipar2],"Rec",pidParName[ipar]);
        }
        for(int ipar2=6;ipar2<8;ipar2++){
            particleHypH2D[ipar*8+ipar2]->InitParParticleTOTNormH2D("True",kinParName[ipar2],"Rec",pidParName[ipar]);
            for(int ipart=0;ipart<NPARTICLEHYP;ipart++){
                particleHypH2D[ipar*8+ipar2]->FillParParticleTOTNormH2D(ipart);
                for(int ipartrue=0;ipartrue<NPARTICLETRUE;ipartrue++){
                    particleHypH2D[ipar*8+ipar2]->FillTrueParParticleTOTNormH2D(ipart,ipartrue);
                }
            }
            particleHypH2D[ipar*8+ipar2]->PlotParParticleTOTNormH2D("True",kinParName[ipar2],"Rec",pidParName[ipar],1);
            particleHypH2D[ipar*8+ipar2]->WriteParParticleTOTNormH2D("True",kinParName[ipar2],"Rec",pidParName[ipar]);
            particleHypH2D[ipar*8+ipar2]->PlotParParticleTOTH2D("True",kinParName[ipar2],"Rec",pidParName[ipar],1);
            particleHypH2D[ipar*8+ipar2]->WriteParParticleTOTH2D("True",kinParName[ipar2],"Rec",pidParName[ipar]);
        }
    }
    count=0;
    for(int ipar=0;ipar<6;ipar++){
        cout<<"2nd set of histograms on particle writing .... "<<ipar<<endl;
        particleKinH1D[ipar]->PlotParParticleH1D("Rec",kinParName[ipar],1);
        particleKinH1D[ipar]->WriteParParticleH1D("Rec",kinParName[ipar]);
        particleKinH1D[ipar]->FillEffPurvsParParticleH1D();
        particleKinH1D[ipar]->PlotEffPurvsParParticleH1D("Rec",kinParName[ipar],1);
        particleKinH1D[ipar]->WriteEffPurvsParParticleH1D("Rec",kinParName[ipar]);
        particleKinH1D[ipar]->FillEffPurvsParChargeH1D();
        particleKinH1D[ipar]->PlotEffPurvsParChargeH1D("Rec",kinParName[ipar],1);
        particleKinH1D[ipar]->WriteEffPurvsParChargeH1D("Rec",kinParName[ipar]);
        for(int ipar2=ipar+1;ipar2<6;ipar2++){
            cout<<"--->  "<<ipar<<" "<<ipar2<<endl;
            particleKinH2D[count]->PlotParParticleH2D("Rec",kinParName[ipar2],"Rec",kinParName[ipar],1);
            particleKinH2D[count]->WriteParParticleH2D("Rec",kinParName[ipar2],"Rec",kinParName[ipar]);
            particleKinH2D[count]->FillEffPurvsParParticleH2D();
            particleKinH2D[count]->WriteEffPurvsParParticleH2D("Rec",kinParName[ipar2],"Rec",kinParName[ipar]);
            particleKinH2D[count]->FillEffPurvsParChargeH2D();
            particleKinH2D[count]->WriteEffPurvsParChargeH2D("Rec",kinParName[ipar2],"Rec",kinParName[ipar]);
            particleKinH2D[count]->WriteParChargeH2D("Rec",kinParName[ipar2],"Rec",kinParName[ipar]);
            count++;
        }
        for(int ipar2=6;ipar2<8;ipar2++){
            cout<<"--->  "<<ipar<<" "<<ipar2<<endl;
            particleKinH2D[count]->PlotParParticleH2D("True",kinParName[ipar2],"Rec",kinParName[ipar],1);
            particleKinH2D[count]->WriteParParticleH2D("True",kinParName[ipar2],"Rec",kinParName[ipar]);
            particleKinH2D[count]->FillEffPurvsParParticleH2D();
            particleKinH2D[count]->WriteEffPurvsParParticleH2D("True",kinParName[ipar2],"Rec",kinParName[ipar]);
            particleKinH2D[count]->FillEffPurvsParChargeH2D();
            particleKinH2D[count]->WriteEffPurvsParChargeH2D("True",kinParName[ipar2],"Rec",kinParName[ipar]);
            particleKinH2D[count]->WriteParChargeH2D("True",kinParName[ipar2],"Rec",kinParName[ipar]);
            count++;
        }
    }
    for(int ipar=6;ipar<8;ipar++){
        cout<<"2nd set of histograms on particle writing .... "<<ipar<<endl;
        particleKinH1D[ipar]->PlotParParticleH1D("True",kinParName[ipar],1);
        particleKinH1D[ipar]->WriteParParticleH1D("True",kinParName[ipar]);
        particleKinH1D[ipar]->FillEffPurvsParParticleH1D();
        particleKinH1D[ipar]->PlotEffPurvsParParticleH1D("True",kinParName[ipar],1);
        particleKinH1D[ipar]->WriteEffPurvsParParticleH1D("True",kinParName[ipar]);
        particleKinH1D[ipar]->FillEffPurvsParChargeH1D();
        particleKinH1D[ipar]->PlotEffPurvsParChargeH1D("True",kinParName[ipar],1);
        particleKinH1D[ipar]->WriteEffPurvsParChargeH1D("True",kinParName[ipar]);
        particleKinH1D[ipar]->WriteParChargeH1D("True",kinParName[ipar]);
        for(int ipar2=ipar+1;ipar2<8;ipar2++){
            cout<<"--->  "<<ipar<<" "<<ipar2<<endl;
            particleKinH2D[count]->PlotParParticleH2D("True",kinParName[ipar2],"True",kinParName[ipar],1);
            particleKinH2D[count]->WriteParParticleH2D("True",kinParName[ipar2],"True",kinParName[ipar]);
            particleKinH2D[count]->FillEffPurvsParParticleH2D();
            particleKinH2D[count]->WriteEffPurvsParParticleH2D("True",kinParName[ipar2],"True",kinParName[ipar]);
            particleKinH2D[count]->FillEffPurvsParChargeH2D();
            particleKinH2D[count]->WriteEffPurvsParChargeH2D("True",kinParName[ipar2],"True",kinParName[ipar]);
            particleKinH2D[count]->WriteParChargeH2D("True",kinParName[ipar2],"True",kinParName[ipar]);
            count++;
        }
    }

    cout<<"Counter histograms on particle writing .... "<<endl;
    particleKinH1D[0]->InitEffPurFOMvsParticleCut();
    particleKinH1D[0]->FillEffPurFOMvsParticleCut();
    particleKinH1D[0]->PlotEffPurFOMvsParticleCut();
    particleKinH1D[0]->FillEffPurFOMvsParticleCutTOT();
    particleKinH1D[0]->PlotEffPurFOMvsParticleCutTOT();
    
    particleKinH1D[0]->InitChargeEffPurFOMvsParticleCut();
    particleKinH1D[0]->FillChargeEffPurFOMvsParticleCut();
    particleKinH1D[0]->PlotChargeEffPurFOMvsParticleCut();
    particleKinH1D[0]->FillChargeEffPurFOMvsParticleCutTOT();
    particleKinH1D[0]->PlotChargeEffPurFOMvsParticleCutTOT();
  
    // for(int irea=0;irea<NREACTION;irea++){
//         cout<<"\n %%%%%%%%%%%%%%%%%%%%%% "<<counter->GetReactionName(irea)<<" %%%%%%%%%%%%%%%%%%%%%%%% "<<endl;
//         for(int ipart=0;ipart<NPARTICLESEL;ipart++)
//             particleKinH1D[0]->PrintStatvsParticleCut(irea,ipart,ipart);
//     }
    cout<<"\n %%%%%%%%%%% TOTAL %%%%%%%%%%"<<endl;
    for(int ipart=0;ipart<NPARTICLESEL;ipart++)
        particleKinH1D[0]->PrintStatvsParticleCut(ipart,ipart);
 
    cout<<"\n %%%%%%%%%%% ccqe  TOTAL%%%%%%%%%%"<<endl;
    particleKinH1D[0]->PrintParticleEffMatrix(1,NPARTICLECUT-1);
    particleKinH1D[0]->PrintParticlePurMatrix(1,NPARTICLECUT-1);
    cout<<"\n %%%%%%%%%%% ccnqe TOTAL %%%%%%%%%%"<<endl;
    particleKinH1D[0]->PrintParticleEffMatrix(2,NPARTICLECUT-1);
    particleKinH1D[0]->PrintParticlePurMatrix(2,NPARTICLECUT-1);
    cout<<"\n %%%%%%%%%%% TOTAL TOTAL  %%%%%%%%%%"<<endl;
    particleKinH1D[0]->PrintParticleEffMatrix(NPARTICLECUT-1);
    particleKinH1D[0]->PrintParticlePurMatrix(NPARTICLECUT-1);
    particleKinH1D[0]->PrintParticleSelTrueMatrix(NPARTICLECUT-1);
    
    cout<<"\n %%%%%%%%%%% ccqe  0<mom<500 MeV%%%%%%%%%%"<<endl;
    particleKinH1D[1]->PrintParticleEffMatrix(1,NPARTICLECUT-1);
    particleKinH1D[1]->PrintParticlePurMatrix(1,NPARTICLECUT-1);
    cout<<"\n %%%%%%%%%%% ccnqe 0<mom<500 MeV %%%%%%%%%%"<<endl;
    particleKinH1D[1]->PrintParticleEffMatrix(2,NPARTICLECUT-1);
    particleKinH1D[1]->PrintParticlePurMatrix(2,NPARTICLECUT-1);
    cout<<"\n %%%%%%%%%%% TOTAL  0<mom<500 MeV %%%%%%%%%%"<<endl;
    particleKinH1D[1]->PrintParticleEffMatrix(NPARTICLECUT-1);
    particleKinH1D[1]->PrintParticlePurMatrix(NPARTICLECUT-1);
    particleKinH1D[1]->PrintParticleSelTrueMatrix(NPARTICLECUT-1);

    cout<<"\n %%%%%%%%%%% ccqe  500<mom<2500 MeV%%%%%%%%%%"<<endl;
    particleKinH1D[2]->PrintParticleEffMatrix(1,NPARTICLECUT-1);
    particleKinH1D[2]->PrintParticlePurMatrix(1,NPARTICLECUT-1);
    cout<<"\n %%%%%%%%%%% ccnqe 500<mom<2500 MeV %%%%%%%%%%"<<endl;
    particleKinH1D[2]->PrintParticleEffMatrix(2,NPARTICLECUT-1);
    particleKinH1D[2]->PrintParticlePurMatrix(2,NPARTICLECUT-1);
    cout<<"\n %%%%%%%%%%% TOTAL  500<mom<2500 MeV %%%%%%%%%%"<<endl;
    particleKinH1D[2]->PrintParticleEffMatrix(NPARTICLECUT-1);
    particleKinH1D[2]->PrintParticlePurMatrix(NPARTICLECUT-1);
    particleKinH1D[2]->PrintParticleSelTrueMatrix(NPARTICLECUT-1);

    cout<<"\n %%%%%%%%%%% ccqe  mom>2500 MeV%%%%%%%%%%"<<endl;
    particleKinH1D[3]->PrintParticleEffMatrix(1,NPARTICLECUT-1);
    particleKinH1D[3]->PrintParticlePurMatrix(1,NPARTICLECUT-1);
    cout<<"\n %%%%%%%%%%% ccnqe mom>2500 MeV %%%%%%%%%%"<<endl;
    particleKinH1D[3]->PrintParticleEffMatrix(2,NPARTICLECUT-1);
    particleKinH1D[3]->PrintParticlePurMatrix(2,NPARTICLECUT-1);
    cout<<"\n %%%%%%%%%%% TOTAL  mom>2500 MeV %%%%%%%%%%"<<endl;
    particleKinH1D[3]->PrintParticleEffMatrix(NPARTICLECUT-1);
    particleKinH1D[3]->PrintParticlePurMatrix(NPARTICLECUT-1);
    particleKinH1D[3]->PrintParticleSelTrueMatrix(NPARTICLECUT-1);


    cout<<"\n %%%%%%%%%%% TOTAL %%%%%%%%%%"<<endl;
    for(int ipart=0;ipart<2;ipart++)
        particleKinH1D[0]->PrintChargeStatvsParticleCut(ipart,ipart);
    cout<<"\n %%%%%%%%%%% TOTAL TOTAL  %%%%%%%%%%"<<endl;
    particleKinH1D[0]->PrintChargeEffMatrix(NPARTICLECUT-2);
    particleKinH1D[0]->PrintChargePurMatrix(NPARTICLECUT-2);
    particleKinH1D[0]->PrintChargeSelTrueMatrix(NPARTICLECUT-2);

   //  //***************************************
    for(int ipar=0;ipar<5;ipar++){
        cout<<" DIFF HISTO writing and plotting .... "<<ipar<<endl;
        for(int ipar2=0;ipar2<5;ipar2++){
            if(ipar2!=ipar){
                for(int irea=0;irea<NREACTION;irea++){
                    sprintf(eventState,"True%s",reaction[irea].c_str());
                    diffparam_True[irea][ipar][ipar2]->PlotDiffPar1Par2TOT("True",paramName[ipar],"Rec",paramName[ipar],"True",paramName[ipar2],eventState,1,1);
                    diffparam_True[irea][ipar][ipar2]->WriteDiffPar1Par2TOT("True",paramName[ipar],"Rec",paramName[ipar],"True",paramName[ipar2],eventState);

                    int icut=NCUTS-1;
                    sprintf(eventState,"Sel%s_Cut%d",reaction[irea].c_str(),icut);               
                    diffparam_Sel[irea][icut][ipar][ipar2]->PlotDiffPar1Par2TOT("True",paramName[ipar],"Rec",paramName[ipar],"True",paramName[ipar2],eventState,1,1);
                    diffparam_Sel[irea][icut][ipar][ipar2]->WriteDiffPar1Par2TOT("True",paramName[ipar],"Rec",paramName[ipar],"True",paramName[ipar2],eventState);
                    for(int itrrea=0;itrrea<NREACTION;itrrea++){
                        sprintf(eventState,"Sel%sTrue%s_Cut%d",reaction[irea].c_str(),reaction[itrrea].c_str(),icut);
                        diffparam_SelTrue[irea][icut][itrrea][ipar][ipar2]->PlotDiffPar1Par2TOT("True",paramName[ipar],"Rec",paramName[ipar],"True",paramName[ipar2],eventState,1,1);
                        diffparam_SelTrue[irea][icut][itrrea][ipar][ipar2]->WriteDiffPar1Par2TOT("True",paramName[ipar],"Rec",paramName[ipar],"True",paramName[ipar2],eventState);
                    }
                }
            }
        }
    }
//     //***************************************
    
    cout<<" true vtx in FV: "<<trueVtxFV<<"        no true in FV "<<NotrueVtxFV<<endl;
    cout<<" trueFV recFV: "<<YestrueYesrecFV<<"      trueFV recNOTFV: "<<YestrueNorecFV<<endl;
    cout<<" trueNOTFV recFV: "<<NotrueYesrecFV<<"       trueNOTFV recNOTFV: "<<NotrueNorecFV<<endl;

    cout<<" rec vtx in FV: "<<recVtxFV<<"      no rec in FV "<<NorecVtxFV<<endl;
    cout<<" recFV trueFV: "<<YesrecYestrueFV<<"        recFV trueNOTFV: "<<YesrecNotrueFV<<endl;
    cout<<" recNOTFV trueFV: "<<NorecYestrueFV<<"        recNOTFV trueNOTFV: "<<NorecNotrueFV<<endl;    
    cout<<" Total reco vtx "<<recVtxTotal<<endl;

             
    //******************************************************    
    cout<<"\n michel cluster found: "<<michel
        <<"\n michel and ccqe: "<<michelccqe<< " over total michel : "<<(double)michelccqe/(double)michel
        <<"\n michel and HdThp and ccqe: "<<michelccqeHdThp<< " over total ccqe michel : "<<(double)michelccqeHdThp/(double)michelccqe
        <<"\n michel and ccnqe: "<<michelccnqe<< " over total michel : "<<(double)michelccnqe/(double)michel
        <<"\n michel and HdThp and ccnqe: "<<michelccnqeHdThp<< " over total ccnqe michel : "<<(double)michelccnqeHdThp/(double)michelccnqe<<endl;

 
    for(int irea=0;irea<NREACTION;irea++){
        cout<<" ***** "<<counter->GetReactionName(irea)<<endl;
        cout<<" High DeltaThp: "<<TotNumberHighDeltaThetap[irea]<<endl;
        cout<<"  Low DeltaThp: "<<TotNumberLowDeltaThetap[irea]<<endl;
    }

    cout<<" NMUONS >1 with fTOTTPCFGD>2 :"<<recAn->ftimesb<<" with fTOTTPCFGD<2 :"<<recAn->ftimesa<<" total "<<recAn->ftimes<<endl;

    cout<<" problematic event "<<countprob<<endl;
    delete trueAn;
    delete recAn;
    delete AnTUtils;
    
    gStyle->SetPalette(1);
    
    rootfile_output->Close();   
    
    return;
    
    
}    
