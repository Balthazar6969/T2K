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
// root ccFidAnalysis.C
//
//--------------------------------------------------------------------------------------------------

using namespace std;
void ccChargeCutAnalysis(){
    gROOT->SetBatch(1);
    //Load some important libraries.
    gSystem->AddIncludePath("-I$OAEVENTROOT/src");
    gSystem->AddIncludePath("-I$OAANALYSISROOT/src");
    gSystem->AddIncludePath("-I.");
  
    gSystem->Load("liboaEvent.so");
    gSystem->Load("liboaAnalysis.so");
    gSystem->Load("libPhysics");
    gSystem->Load("libGeom");
    gSystem->Load("libTree");
    gSystem->Load("libMatrix");
    gSystem->Load("libGui");
    
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
    TString fileName("cc_input_files.txt");
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
    double curvcut0=0;
    double dstep=0.015;
    sprintf(fname,"ccChargeCutAnalysis_output_Curv%.2f-%.2f_Cut%d.root",curvcut0+dstep,dstep*(NSTEP)+curvcut0,PARCUT);
    TFile *rootfile_output=new TFile(fname,"RECREATE");
      
    TTrackerAnalysisUtils *AnTUtils=new TTrackerAnalysisUtils();
    TCCtruthAnalysisUtils *trueAn=new TCCtruthAnalysisUtils();
    TCCreconAnalysisUtils *recAn=new TCCreconAnalysisUtils();
    TNEUTAnalysisUtils *NeutUtils=new TNEUTAnalysisUtils();
   
   
    //initialize counters
    //*****************************
    int isreaction[NREACTION];
    double TotalInFV=0;



    TCCplotAnalysisUtils *curv=new TCCplotAnalysisUtils(rootfile_output);
    TCCplotAnalysisUtils *particleMatch[5];//9+14+14
    for(int i=0;i<5;i++)
        particleMatch[i]=new TCCplotAnalysisUtils(rootfile_output);
    particleMatch[0]->InitParParticleTOTH1D("Rec","MomErr");
    particleMatch[0]->InitCutOptChargeCounter();
    particleMatch[0]->InitChargeEffPurFOMvsParticleCutOpt("charge");
    particleMatch[1]->InitParParticleH2D("True","Mom","Rec","MomErr");
    particleMatch[2]->InitParParticleH2D("Rec","Mom","Rec","MomErr");
    particleMatch[3]->InitParParticleH2D("True","CosTh","Rec","MomErr");  
    particleMatch[4]->InitParParticleH2D("Rec","CosTh","Rec","MomErr");

    
    // Loop over all events.
    // ****************************
    TStopwatch timer;
    timer.Start();
    int entries = mcTruthVertices.GetEntries(); 
    std::cout << "ent: " << entries << std::endl;


    //  for(int ient = 0; ient <  1000; ient++)
     for(int ient = 0; ient <  entries; ient++)
    //      for(int ient = 0; ient <5;ient++)
    {       
        mcTruthVertices.GetEntry(ient);
        reconTracker.GetEntry(ient);
        if(ient%500 == 0)
        {
            std::cout <<  "********************************" << std::endl;
            std::cout <<  "!!!!Events Processed " << ient << std::endl;
            timer.Stop();
            std::cout << " Processing time: " << timer.CpuTime() << " " << timer.RealTime() << std::endl;
            timer.Start();
            std::cout <<  "********************************" << std::endl;
        }
        
        // Look at the true vertex and copy the array info in a TTruthVertex
        // *****************************************************************
        ND::TTruthVerticesModule::TTruthVertex *true_fgd_vertex;
        if(NVtxFGD1 >0){
            true_fgd_vertex = (ND::TTruthVerticesModule::TTruthVertex*) VtxFGD1->UncheckedAt(0);
        }
        else if (NVtxFGD2 >0){
            true_fgd_vertex = (ND::TTruthVerticesModule::TTruthVertex*) VtxFGD2->UncheckedAt(0);
        }
        else{
            std::cerr << "There should be at least one vertex in one FGD.  Skipping event."<<std::endl;
            continue;
        }
        TLorentzVector TrueVtx;
        TrueVtx = true_fgd_vertex->Vertex;  
        TVector3 vtxtrue(TrueVtx.X(),TrueVtx.Y(),TrueVtx.Z());
        
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
        }
        double enuOUT=nuOUT_momentum.T();
        double costhnuOUT=trueAn->GetTrueCosThetamu(nuOUT_momentum);//the way for the neutrino is exactly the same...

 
   
        // check reaction corresponding to the vertex
        // 1: is TrueVtx in FV ? if not => next event
        // 1: CC must have a MUON, NC must have a NU. If not => BUG in NEUT => pass to next event
        // 2: check the reaction of the event 0: ccAll 1:ccqe 2:ccnqe 3:ccppi+ 4:ncAll
        // 3: if CC check what is the sub reaction 0:coh-pi 1:ccDIS 2:ccnqe 3:cc/nc
        // **************************************************************************************
        bool isfid=false;
        AnTUtils->DefineFiducialVolume(200,200,10.);
        isfid=AnTUtils->IsInFiducialVolume(vtxtrue);       
        // if(isfid && (enuOUT!=0 || pmu!=0) ){
        double rxn_code=99999;
        if(isfid ){
            TotalInFV++;
            rxn_code=NeutUtils->GetReactionCode(true_fgd_vertex);
            for(int irea=0;irea<NREACTION;irea++){
                isreaction[irea]=0;
                isreaction[irea]=(int)trueAn->IsReaction((int)rxn_code,irea);
            }
        }
    //     else
//             continue;

//         if((isreaction[0] && pmu==0))
//             continue;

        
        // Apply cut selection
        // **************************************************************************************
        double DeltaThetaP=999;
        double DeltaEnergyP=99999;
        int kTotalTPC=0;
        bool oneAn=false;
      
     

        // 1: - For 1TrackAnalysis:
        //      look if there is 1 negative track in the fiducial volume
        //    - For >2TrackAnalysis:
        //      look if there is only and only  1 negative track and the rest of the tracks should be positive
        // 2: All the tracks have to be in the fiducial volume
        // 3: Look for the existence of a Michel electron
        // 4: For 1neg and 1pos track:
        //    - look for the delta theta_p, if < 0.3 => ccqe
        //                                  if > 0.3 => ccnqe
        //    - look for delta ep
        // 5: Define the box region in costhmu and pmu where efficiencies between ND and SK are similar   
        // *****************************************************************************************************
        if( isfid && ((enuOUT!=0 && isreaction[4]) || (isreaction[0] && pmu!=0))){
            recAn->ResetCutFlag();
            kTotalTPC=recAn->GetTotalTracksInTPC(Tracks,NTracks);
            recAn->SetCurvParticleCut(1000,PARCUT);
            double trueINFO[4]={rxn_code,vtxtrue.X(),vtxtrue.Y(),vtxtrue.Z()};
            if(NTracks == 1 && kTotalTPC==1 )
                oneAn=recAn->OneTrackAnalysis(Tracks,ient);
            else if(NTracks >= 2 && kTotalTPC > 0 )
                recAn->MultipleTracksAnalysis(Tracks, NTracks, Vertices, NVertices,ient);
      
            recAn->FillTrackInfo_ChargeCutStudy(trueINFO,particleMatch);
            double curvcut=curvcut0;
            for(int istep=1;istep<=NSTEP;istep++){
                curvcut=istep*dstep+curvcut0;
                recAn->SetCurvParticleCut(curvcut,PARCUT);
                recAn->FillTrackInfo_ChargeCutOpt(trueINFO,particleMatch,istep-1);
            }
        }
    }//end ientries
    
     particleMatch[0]->FillChargeEffPurFOMvsParticleCutOpt(curvcut0,dstep);
     particleMatch[0]->PlotChargeEffPurFOMvsParticleCutOpt("Charge");


    delete trueAn;
    delete recAn;
    delete AnTUtils;
    
    gStyle->SetPalette(1);
    
    rootfile_output->Close();   
    
    return;
   

}    
