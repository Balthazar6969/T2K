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


// #include "/T2K/nd/oaEvent/v5r9/src/EoaCore.hxx"
// #include "/T2K/nd/oaEvent/v5r9/src/ToaEvent.hxx"
// #include "/T2K/nd/oaEvent/v5r9/src/TND280Event.hxx"
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
void ccFidCutAnalysis(){
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
    TString fileName("ccqe_input_files.txt");
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
    sprintf(fname,"ccFidCutAnalysis_output.root");
    TFile *rootfile_output=new TFile(fname,"RECREATE");
      
    TTrackerAnalysisUtils *AnTUtils=new TTrackerAnalysisUtils();
    TCCtruthAnalysisUtils *trueAn=new TCCtruthAnalysisUtils();
    TCCreconAnalysisUtils *recAn=new TCCreconAnalysisUtils();
    TNEUTAnalysisUtils *NeutUtils=new TNEUTAnalysisUtils();
   
   
    //initialize counters
    //*****************************
    int isreaction[NREACTION];
    int isfidreaction[NREACTION][NSTEP];
    double TotalInFV[NSTEP]={0,0,0,0,0,0,0,0,0,0};
    TCCplotAnalysisUtils *counter=new TCCplotAnalysisUtils(rootfile_output);
    counter->InitCutOptCounter();
        
  
    // Loop over all events.
    // ****************************
    TStopwatch timer;
    timer.Start();
    int entries = mcTruthVertices.GetEntries(); 
    std::cout << "ent: " << entries << std::endl;


    for(int ient = 0; ient <  5000; ient++)
        // for(int ient = 0; ient <  entries; ient++)
    // for(int ient = 0; ient <505;ient++)
    {       
        mcTruthVertices.GetEntry(ient);
        reconTracker.GetEntry(ient);
        if(ient%500 == 0)
        {
            std::cout <<  "Events Processed " << ient << std::endl;
            timer.Stop();
            std::cout << " Processing time: " << timer.CpuTime() << " " << timer.RealTime() << std::endl;
            timer.Start();
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
        AnTUtils->DefineFiducialVolume(50,50,10.);
        isfid=AnTUtils->IsInFiducialVolume(vtxtrue);       
        // if(isfid && (enuOUT!=0 || pmu!=0) ){
        double rxn_code=99999;
        if(isfid ){
            rxn_code=NeutUtils->GetReactionCode(true_fgd_vertex);
            for(int irea=0;irea<NREACTION;irea++){
                isreaction[irea]=0;
                isreaction[irea]=(int)trueAn->IsReaction((int)rxn_code,irea);
            }
        }
        else
            continue;

        if((isreaction[0] && pmu==0))
            continue;
       //  if((enuOUT==0 && isreaction[4])){
//             cout<<"enuOUT "<<nuOUT_momentum.T()
//                 <<" pnuX "<<nuOUT_momentum.X()
//                 <<" pnuY "<<nuOUT_momentum.Y()
//                 <<" pnuZ "<<nuOUT_momentum.Z()<<endl;
//             //continue;
//         }
      

        // Now Start Selection optimisation
        // **************************************************************************************
        for(int istep=0;istep<NSTEP;istep++){
            double dd=50.*(istep+1);
            AnTUtils->DefineFiducialVolume(dd,dd,10.);
            isfid=AnTUtils->IsInFiducialVolume(vtxtrue);
            
            if(isfid){
                TotalInFV[istep]++;
                for(int irea=0;irea<NREACTION;irea++){
                    isfidreaction[irea][istep]=0;
                    isfidreaction[irea][istep]=(int)trueAn->IsReaction((int)rxn_code,irea);
                }
            }
            else
                continue;
            double DeltaThetaP=999;
            double DeltaEnergyP=99999;
            int kTotalTPC=0;
            bool oneAn=false;
            kTotalTPC=recAn->GetTotalTracksInTPC(Tracks,NTracks);
            recAn->ResetCutFlag();

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
            if(NTracks == 1 && kTotalTPC==1 )
                oneAn=recAn->OneTrackAnalysis(Tracks);      
            else if(NTracks >= 2 && kTotalTPC > 0 )
                recAn->MultipleTracksAnalysis(Tracks, NTracks, Vertices, NVertices);
            
            recAn->HasMichelElectron(DelayedClusters, NDelayedClusters);
            DeltaThetaP=recAn->GetDeltaThetaP();
            recAn->HighDeltaThetaP(DeltaThetaP,0.3);
            DeltaEnergyP=recAn->GetDeltaEnergyP(false);//false=ishigh delta theta p..
            recAn->DefineRecCosThmuRecPmuRange(0.5,1,0,1000);
            
            //FILL COUNTERS
            //*******************************
            if( isfid && ((enuOUT!=0 && isreaction[4]) || (isreaction[0] && pmu!=0))){
                for(int irea=0;irea<NREACTION;irea++){
                    if(isfidreaction[irea][istep]){
                        counter->FillTrueCutOptCounter(istep,irea);
                    }
                }
                for(int iselrea=0;iselrea<NREACTION;iselrea++){
                    for(int icut=0;icut<NCUTS;icut++){
                        bool iscut=false;
                        iscut=recAn->DefineCut(iselrea,icut);                                 
                        if(iscut){
                            counter->FillSelCutOptCounter(istep,iselrea,icut);
                            for(int irea=0;irea<NREACTION;irea++){
                                if(isfidreaction[irea][istep]){
                                    counter->FillSelTrueCutOptCounter(istep,iselrea,icut,irea);
                                }
                            }
                        }
                    }
                }
            }
            isfid=false;
        }
    }//end ientries
       
    counter->InitEffPurFOMvsCutOpt("FV");
    counter->FillEffPurFOMvsCutOpt();
    counter->PlotEffPurFOMvsCutOpt("FV");
    
    for(int irea=0;irea<NREACTION;irea++){
        counter->PrintCutOptStat(irea,3,irea);
    }

    delete trueAn;
    delete recAn;
    delete AnTUtils;
    
    gStyle->SetPalette(1);
    
    rootfile_output->Close();   
    
    return;
   

}    
