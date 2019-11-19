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
// // #include "../src/EoaAnalysis.hxx"
// // #include "../src/oaAnalysis_version.h"
// #include "../src/TTruthVerticesModule.hxx"
// #include "../src/TTrackerReconModule.hxx"
// // #include "../src/TAnalysisHeaderModuleBase.hxx"
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
void ccThpCutAnalysis(){
    gROOT->SetBatch(1);
    //Load some important libraries.
    gSystem->AddIncludePath("-I$OAANALYSISROOT/src");
    gSystem->AddIncludePath("-I.");

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
    double curvcut=0.2;
    int PARCUT=NPARTICLECUT-1;
    sprintf(fname,"ccThpCutAnalysis_output-Curv%.2f-Cut%d.root",curvcut,PARCUT);
    TFile *rootfile_output=new TFile(fname,"RECREATE");
      
    TTrackerAnalysisUtils *AnTUtils=new TTrackerAnalysisUtils();
    TCCtruthAnalysisUtils *trueAn=new TCCtruthAnalysisUtils();
    TCCreconAnalysisUtils *recAn=new TCCreconAnalysisUtils();
    TNEUTAnalysisUtils *NeutUtils=new TNEUTAnalysisUtils();
   
   
    //initialize counters
    //*****************************
    int isreaction[NREACTION];
    double TotalInFV=0;
    TCCplotAnalysisUtils *counter=new TCCplotAnalysisUtils(rootfile_output);
    counter->InitCutOptReaCounter();
   
    
    TCCplotAnalysisUtils *diffTrueExpEnuTrueEnu=new TCCplotAnalysisUtils(rootfile_output);
    diffTrueExpEnuTrueEnu->InitDiffPar1Par2("TrueExp","Enu","True","Enu","True");
    TCCplotAnalysisUtils *diffTrueExpCCQEEnuTrueEnu=new TCCplotAnalysisUtils();
    diffTrueExpCCQEEnuTrueEnu->InitDiffPar1Par2("TrueExpCCQE","Enu","True","Enu","True");

    TCCplotAnalysisUtils *diffTrueExpEpTrueEp=new TCCplotAnalysisUtils(rootfile_output);
    diffTrueExpEpTrueEp->InitDiffPar1Par2("TrueExp","Ep","True","Ep","True");
    TCCplotAnalysisUtils *diffTrueExpCCQEEpTrueEp=new TCCplotAnalysisUtils(rootfile_output);
    diffTrueExpCCQEEpTrueEp->InitDiffPar1Par2("TrueExpCCQE","Ep","True","Ep","True");
    TCCplotAnalysisUtils *diffTrueExpReaEpTrueEp=new TCCplotAnalysisUtils(rootfile_output);
    diffTrueExpReaEpTrueEp->InitDiffPar1Par2("TrueExpRea","Ep","True","Ep","True");

    TCCplotAnalysisUtils *dThp=new TCCplotAnalysisUtils(rootfile_output);
    dThp->InitParH1D("Rec","dThp");
    TCCplotAnalysisUtils *dThp_topo=new TCCplotAnalysisUtils(rootfile_output);
    dThp_topo->InitParH1D("topoRec","dThp");
    TCCplotAnalysisUtils *dThp_fermi=new TCCplotAnalysisUtils(rootfile_output);
    dThp_fermi->InitParH1D("fermiRec","dThp");

    TCCplotAnalysisUtils *dEp=new TCCplotAnalysisUtils(rootfile_output);
    dEp->InitParH1D("Rec","dEp");
    TCCplotAnalysisUtils *dEp_topo=new TCCplotAnalysisUtils(rootfile_output);
    dEp_topo->InitParH1D("topoRec","dEp");
    TCCplotAnalysisUtils *dEp_fermi=new TCCplotAnalysisUtils(rootfile_output);
    dEp_fermi->InitParH1D("fermiRec","dEp");

    TCCplotAnalysisUtils *dThpvsEp=new TCCplotAnalysisUtils(rootfile_output);
    dThpvsEp->InitParH2D("Rec","Ep","Rec","dThp");

    char name[256];
    TCCplotAnalysisUtils *PMUVSCOSTHMU[NSTEP];
    TCCplotAnalysisUtils *PMU[NSTEP];
    TCCplotAnalysisUtils *COSTHMU[NSTEP];
    TCCplotAnalysisUtils *counterREA[NSTEP];
     
    for(int idthp=0;idthp<NSTEP;idthp++){
        sprintf(name,"dthp%d",idthp);
        PMUVSCOSTHMU[idthp]=new TCCplotAnalysisUtils(rootfile_output);
        PMUVSCOSTHMU[idthp]->InitParH2D("True","CosThmu","True","Pmu",name);
        PMUVSCOSTHMU[idthp]->InitEffPurvsParH2D("True","CosThmu","True","Pmu",name);
        PMU[idthp]=new TCCplotAnalysisUtils(rootfile_output);
        PMU[idthp]->InitParH1D("True","Pmu",name);
        PMU[idthp]->InitEffPurvsParH1D("True","Pmu",name);
        COSTHMU[idthp]=new TCCplotAnalysisUtils(rootfile_output);
        COSTHMU[idthp]->InitParH1D("True","CosThmu",name);
        COSTHMU[idthp]->InitEffPurvsParH1D("True","CosThmu",name);
        counterREA[idthp]=new TCCplotAnalysisUtils(rootfile_output);
        counterREA[idthp]->InitReaCounter();
    }
    // Loop over all events.
    // ****************************
    TStopwatch timer;
    timer.Start();
    int entries = mcTruthVertices.GetEntries(); 
    std::cout << "ent: " << entries << std::endl;


    //   for(int ient = 0; ient <  25000; ient++)
    for(int ient = 0; ient <  entries; ient++)
    // for(int ient = 0; ient <505;ient++)
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
        else
            continue;

        if((isreaction[0] && pmu==0))
            continue;

      // calculation of the expected proton energy with the information of the muon, proton
        // **************************************************************************************
        double ep_th=0;
        double ep_sabelotodo_th=0;
        double ep_blind_th=0;
        double Enuexp=0;
        double Enuexpccqe=trueAn->GetTrueExpNeutrinoEnergy(muon_momentum, ep, 0);
        TLorentzVector nu_expmomentumccqe=TLorentzVector(0,0,Enuexpccqe,Enuexpccqe); //as ccqe
        TLorentzVector nu_expmomentum;

        if(leptonSize>0){            
            ep_blind_th=trueAn->GetTrueExpProtonEnergy(muon_momentum, nucleon_target_momentum, nu_expmomentumccqe);
            ep_sabelotodo_th=trueAn->GetTrueExpProtonEnergy(muon_momentum, nucleon_target_momentum, neutrino_momentum);
            for(int irea=0;irea<NREACTION;irea++)
            {
                if(isreaction[irea]){
                    Enuexp=trueAn->GetTrueExpNeutrinoEnergy(muon_momentum, ep, irea);
                    nu_expmomentum=TLorentzVector(0,0,Enuexp,Enuexp);
                    ep_th=trueAn->GetTrueExpProtonEnergy(muon_momentum, nucleon_target_momentum, nu_expmomentum);
                    diffTrueExpCCQEEnuTrueEnu->FillDiffPar1Par2(irea,enu,Enuexpccqe);
                    diffTrueExpEnuTrueEnu->FillDiffPar1Par2(irea,enu,Enuexp);

                    //calculated with the help of the knowledge of the reaction
                    diffTrueExpReaEpTrueEp->FillDiffPar1Par2(irea,ep,ep_th);
                    //calculated as if ccqe
                    diffTrueExpCCQEEpTrueEp->FillDiffPar1Par2(irea,ep,ep_blind_th);
                    //calculated taking directly enutrue
                    diffTrueExpEpTrueEp->FillDiffPar1Par2(irea,ep,ep_sabelotodo_th);
                

                }
            }
        }  
        // Apply cut selection
        // **************************************************************************************
        double DeltaThetaP=999;
        double DeltaEnergyP=99999;
        int kTotalTPC=0;
        bool oneAn=false;
        recAn->ResetCutFlag();
        kTotalTPC=recAn->GetTotalTracksInTPC(Tracks,NTracks);
     

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
        recAn->SetCurvParticleCut(curvcut,PARCUT);
        if(NTracks == 1 && kTotalTPC==1 )
            oneAn=recAn->OneTrackAnalysis(Tracks,ient);
        else if(NTracks >= 2 && kTotalTPC > 0 )
            recAn->MultipleTracksAnalysis(Tracks, NTracks, Vertices, NVertices,ient);
        if(NDelayedClusters>0)
            recAn->HasMichelElectron(DelayedClusters, NDelayedClusters);

        if(recAn->fNumberTpcTr_True[0]>1 ){
            cout<<" costhmu "<<costhmu<<" pmu "<<pmu<<" muon "<<recAn->fNumberTpcTr_Sel[0][NPARTICLECUT-1]<<" neg "<<recAn->fNumberTpcTr_Sel[0][NPARTICLECUT-2]<<" true "<<recAn->fNumberTpcTr_True[0]<<" ient "<<ient<<endl;
            continue;
        }
        if(recAn->fNumberTpcTr_True[1]>1 ){
            cout<<" ep "<<ep<<" proton "<<recAn->fNumberTpcTr_Sel[1][NPARTICLECUT-1]<<" pos "<<recAn->fNumberTpcTr_Sel[1][NPARTICLECUT-2]<<" true "<<recAn->fNumberTpcTr_True[1]<<" ient "<<ient<<endl;
        }
        DeltaThetaP=recAn->GetDeltaThetaP();
        DeltaEnergyP=recAn->GetDeltaEnergyP(false);//false no correlation with thp
        recAn->HighDeltaThetaP(DeltaThetaP,0.5);
        int totf=trueAn->GetTotalFinalParticle(true_fgd_vertex);
        double mn=939.56;
        double eb=-27.0;
                   
        if(DeltaThetaP!=999){
            cout<<" dthp "<<DeltaThetaP<<endl;
            for(int irea=0;irea<NREACTION-1;irea++){
                if(isreaction[irea]){
                    dThp->FillTrueReaH1D(irea,DeltaThetaP,1);
                    dEp->FillTrueReaH1D(irea,DeltaEnergyP,1);
                    dThpvsEp->FillTrueReaH2D(irea,DeltaEnergyP,DeltaThetaP,1);
                    if(totf>=3){
                        dThp_topo->FillTrueReaH1D(irea,DeltaThetaP,1);
                        dEp_topo->FillTrueReaH1D(irea,DeltaEnergyP,1);
                 
                    }
                    if(sqrt(nTargetMom2+(mn+eb)*(mn+eb))-(mn+eb)>20){
                        dThp_fermi->FillTrueReaH1D(irea,DeltaThetaP,1);
                        dEp_fermi->FillTrueReaH1D(irea,DeltaEnergyP,1);
                    }
                }
            }
            for(int iselrea=0;iselrea<NREACTION-1;iselrea++){
                for(int icut=0;icut<NCUTS;icut++){
                    bool iscut=false;
                    iscut=recAn->IsSelReaction(iselrea,icut);
                    if(iscut){
                        dThp->FillSelReaH1D(iselrea,icut,DeltaThetaP,1);
                        dEp->FillSelReaH1D(iselrea,icut,DeltaEnergyP,1);
                        dThpvsEp->FillSelReaH2D(iselrea,icut,DeltaEnergyP,DeltaThetaP,1);
                        if(totf>=3){
                            dThp_topo->FillSelReaH1D(iselrea,icut,DeltaThetaP,1);
                            dEp_topo->FillSelReaH1D(iselrea,icut,DeltaEnergyP,1);
                        }
                        if(sqrt(nTargetMom2+(mn+eb)*(mn+eb))-(mn+eb)>20){
                            dThp_fermi->FillSelReaH1D(iselrea,icut,DeltaThetaP,1);
                            dEp_fermi->FillSelReaH1D(iselrea,icut,DeltaEnergyP,1);
                        }
                        for(int irea=0;irea<NREACTION-1;irea++){
                            if(isreaction[irea]){
                                dThp->FillSelTrueReaH1D(iselrea,icut,irea,DeltaThetaP,1);
                                dEp->FillSelTrueReaH1D(iselrea,icut,irea,DeltaEnergyP,1);
                                dThpvsEp->FillSelTrueReaH2D(iselrea,icut,irea,DeltaEnergyP,DeltaThetaP,1);
                                if(totf>=3){
                                    dThp_topo->FillSelTrueReaH1D(iselrea,icut,irea,DeltaThetaP,1);
                                    dEp_topo->FillSelTrueReaH1D(iselrea,icut,irea,DeltaEnergyP,1);
                                }
                                if(sqrt(nTargetMom2+(mn+eb)*(mn+eb))-(mn+eb)>20){
                                    dThp_fermi->FillSelTrueReaH1D(iselrea,icut,irea,DeltaThetaP,1);
                                    dEp_fermi->FillSelTrueReaH1D(iselrea,icut,irea,DeltaEnergyP,1);
                                }
                            }
                        }
                    }
                }
            }
        }
             
        // Now Start Selection optimisation
        // **************************************************************************************
        if( isfid && ((enuOUT!=0 && isreaction[4]) || (isreaction[0] && pmu!=0))){
            for(int istep=0;istep<NSTEP;istep++){
                double ithpmax=0;
                ithpmax=DTHP0+istep*0.1;
                recAn->HighDeltaThetaP(DeltaThetaP,ithpmax);
                //  cout<<"istep "<<istep<<" thpmax "<<ithpmax<<" dthp "<<DeltaThetaP<<" ishigh "<< recAn->HighDeltaThetaP(DeltaThetaP,ithpmax)<<endl;
                recAn->DefineRecCosThmuRecPmuRange(0.5,1,0,1000);
            
                //FILL COUNTERS
                //*******************************
                for(int irea=0;irea<NREACTION;irea++){
                    if(isreaction[irea]){
                        counter->FillTrueCutOptReaCounter(istep,irea);
                        counterREA[istep]->FillTrueReaCounter(irea,1,1);
                        PMU[istep]->FillTrueReaH1D(irea,pmu,1);
                        COSTHMU[istep]->FillTrueReaH1D(irea,costhmu,1);
                        PMUVSCOSTHMU[istep]->FillTrueReaH2D(irea,costhmu,pmu,1);
                    }
                }
                for(int iselrea=0;iselrea<NREACTION;iselrea++){
                    for(int icut=0;icut<NCUTS;icut++){
                        bool iscut=false;
                        iscut=recAn->IsSelReaction(iselrea,icut);
                        if(iscut){
                            PMU[istep]->FillSelReaH1D(iselrea,icut,pmu,1);
                            COSTHMU[istep]->FillSelReaH1D(iselrea,icut,costhmu,1);
                            PMUVSCOSTHMU[istep]->FillSelReaH2D(iselrea,icut,costhmu,pmu,1);
                            counterREA[istep]->FillSelReaCounter(iselrea,icut,1,1);
                            counter->FillSelCutOptReaCounter(istep,iselrea,icut);
                            for(int irea=0;irea<NREACTION;irea++){
                                if(isreaction[irea]){
                                    counterREA[istep]->FillSelTrueReaCounter(iselrea,icut,irea,1,1);
                                    counter->FillSelTrueCutOptReaCounter(istep,iselrea,icut,irea);
                                    PMU[istep]->FillSelTrueReaH1D(iselrea,icut,irea,pmu,1);
                                    COSTHMU[istep]->FillSelTrueReaH1D(iselrea,icut,irea,costhmu,1);
                                    PMUVSCOSTHMU[istep]->FillSelTrueReaH2D(iselrea,icut,irea,costhmu,pmu,1);
                                }
                            }
                        }
                    }
                }
            }
            isfid=false;
        }
    }//end ientries

     
    
     for(int idthp=0;idthp<NSTEP;idthp++){
         sprintf(name,"dthp%d",idthp);
         counterREA[idthp]->InitEffPurFOMvsReaCut();
         counterREA[idthp]->FillEffPurFOMvsReaCut();
         counterREA[idthp]->PlotEffPurFOMvsReaCut(name);
         PMU[idthp]->FillEffPurvsParH1D();
         COSTHMU[idthp]->FillEffPurvsParH1D();
         PMUVSCOSTHMU[idthp]->FillEffPurvsParH2D();
         
         PMUVSCOSTHMU[idthp]->PlotParH2D("True","CosThmu","True","Pmu",1);
         PMUVSCOSTHMU[idthp]->PlotEffPurvsParH2D("True","CosThmu","True","Pmu",1);
         PMU[idthp]->PlotParH1D("True","Pmu",1);
         PMU[idthp]->PlotEffPurvsParH1D("True","Pmu",1);
         COSTHMU[idthp]->PlotParH1D("True","CosThmu",1);
         COSTHMU[idthp]->PlotEffPurvsParH1D("True","CosThmu",1);
         
         PMUVSCOSTHMU[idthp]->WriteParH2D("True","CosThmu","True","Pmu");
         PMUVSCOSTHMU[idthp]->WriteEffPurvsParH2D("True","CosThmu","True","Pmu");
         PMU[idthp]->WriteParH1D("True","Pmu");
         PMU[idthp]->WriteEffPurvsParH1D("True","Pmu");
         COSTHMU[idthp]->WriteParH1D("True","CosThmu");
         COSTHMU[idthp]->WriteEffPurvsParH1D("True","CosThmu");
     }
     
     
   
    diffTrueExpEnuTrueEnu->PlotDiffPar1Par2("TrueExp","Enu","True","Enu","True",0);
    diffTrueExpCCQEEnuTrueEnu->PlotDiffPar1Par2("TrueExpCCQE","Enu","True","Enu","True",0);
    diffTrueExpEpTrueEp->PlotDiffPar1Par2("TrueExp","Ep","True","Ep","True",0);
    diffTrueExpCCQEEpTrueEp->PlotDiffPar1Par2("TrueExpCCQE","Ep","True","Ep","True",0);
    diffTrueExpReaEpTrueEp->PlotDiffPar1Par2("TrueExpRea","Ep","True","Ep","True",0);
    
    diffTrueExpEnuTrueEnu->WriteDiffPar1Par2("TrueExp","Enu","True","Enu","True");
    diffTrueExpCCQEEnuTrueEnu->WriteDiffPar1Par2("TrueExpCCQE","Enu","True","Enu","True");
    diffTrueExpEpTrueEp->WriteDiffPar1Par2("TrueExp","Ep","True","Ep","True");
    diffTrueExpCCQEEpTrueEp->WriteDiffPar1Par2("TrueExpCCQE","Ep","True","Ep","True");
    diffTrueExpReaEpTrueEp->WriteDiffPar1Par2("TrueExpRea","Ep","True","Ep","True");
            
    dThp->PlotParH1D("Rec","dThp",0);
    dThp_topo->PlotParH1D("Rec","dThp",0);
    dThp_fermi->PlotParH1D("Rec","dThp",0);
    dEp->PlotParH1D("Rec","dEp",0);
    dEp_topo->PlotParH1D("Rec","dEp",0);
    dEp_fermi->PlotParH1D("Rec","dEp",0);
    dThpvsEp->PlotParH2D("Rec","Ep","Rec","dThp",0);

    dThp->WriteParH1D("Rec","dThp");
    dThp_topo->WriteParH1D("Rec","dThp");
    dThp_fermi->WriteParH1D("Rec","dThp");
    dEp->WriteParH1D("Rec","dEp");
    dEp_topo->WriteParH1D("Rec","dEp");
    dEp_fermi->WriteParH1D("Rec","dEp");
    dThpvsEp->WriteParH2D("Rec","Ep","Rec","dThp");
    
    counter->InitEffPurFOMvsReaCutOpt("Thp");
    counter->FillEffPurFOMvsReaCutOpt();
    counter->PlotEffPurFOMvsReaCutOpt("Thp");
    
    for(int irea=0;irea<NREACTION;irea++){
        counter->PrintCutOptStat(irea,NCUTS-2,irea);
    }
    cout<<" NMUONS >1 with fTOTTPCFGD>2 :"<<recAn->ftimesb<<" with fTOTTPCFGD<2 :"<<recAn->ftimesa<<" total "<<recAn->ftimes<<endl;
    delete trueAn;
    delete recAn;
    delete AnTUtils;
    
    gStyle->SetPalette(1);
    
    rootfile_output->Close();   
    
    return;
   

}    
