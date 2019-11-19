#include <iostream>
#include <vector>
#include <utility>
#include <fstream>

#include <TSystem.h>
#include <TROOT.h>
#include <TApplication.h>
#include <memory>
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

// #include "/T2K/nd/oaEvent/v6r0/src/EoaCore.hxx"
// #include "/T2K/nd/oaEvent/v6r0/src/ToaEvent.hxx"
// #include "/T2K/nd/oaEvent/v6r0/src/TND280Event.hxx"
// #include "../src/EoaAnalysis.hxx"
// #include "../src/oaAnalysis_version.h"
// #include "../src/TTruthVerticesModule.hxx"
// #include "../src/TTrackerReconModule.hxx"
// #include "../src/TAnalysisHeaderModuleBase.hxx"
// #include <AnalysisTools/TNEUTAnalysisUtils.hxx>
// #include <AnalysisTools/TTrackerAnalysisUtils.hxx>
// #include <AnalysisTools/TCCtruthAnalysisUtils.hxx>
// #include <AnalysisTools/TCCreconAnalysisUtils.hxx>


using namespace std;
void reconAnalysis(){
  
    
    //Load some important libraries.
    gSystem->Load("liboaEvent.so");
    gSystem->Load("liboaAnalysis.so");
    gSystem->Load("libPhysics");
    gSystem->Load("libGeom");
    gSystem->Load("libTree");
    gSystem->Load("libMatrix");
   
    gROOT->ProcessLine(".L AnalysisTools/TNEUTAnalysisUtils.cxx");
    gROOT->ProcessLine(".L AnalysisTools/TTrackerAnalysisUtils.cxx");
    gROOT->ProcessLine(".L AnalysisTools/TCCtruthAnalysisUtils.cxx");
    gROOT->ProcessLine(".L AnalysisTools/TCCreconAnalysisUtils.cxx");
   
    TChain mcTruthVertices = TChain("TruthDir/Vertices");
    TChain mcTruthTrajectories = TChain("TruthDir/Trajectories");
    TChain reconTracker = TChain("ReconDir/Tracker");

    // _____________________________________________________________________________________
    // Load data files   
    //TString fileName("ccqe_input_files.txt");

    TString fileName("testfile.txt");
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

   
    
    // -------------------------------------------------------------------------------------
    // Create histograms.
    // -------------------------------------------------------------------------------------

    
    TH1F *dthetapTOT=new TH1F("dthetap_TOT"," Delta theta_p between expected and reconstructed proton track",63,0,3.14);
    TH1F *dthetapTopo=new TH1F("dthetap_Topo"," Delta theta_p between expected and reconstructed proton track for ccqe AND nb part_f >2",63,0,3.14);
    TH1F *denergypTOT=new TH1F("denergyp_TOT"," dE_p between expected and reconstructed proton track",1000,-10000,10000);
    TH1F *dThetaTrueRec=new TH1F("dThetaTrueRec"," costheta_mu_true- costheta_mu_rec   ",100,-1,1);
    TH2F *dThpvsdEpTOT=new TH2F("dThvsdEpTOT","dThp vs dEp ",200,-2000,2000,63,0,3.14);
         
   
    TH2F *dThpvsdEp[NREACTION];
    TH1F *dthetap[NREACTION];
    TH1F *denergyp[NREACTION];

    TH1F *denergyp_sel[NREACTION];
    TH1F *smalldTHdenergyp_seltrue[NREACTION];
    TH1F *highdTHdenergyp_seltrue[NREACTION];
    TH1F *denergypTrue[NREACTION];
    TH1F *denergypTrue_blind[NREACTION];
    TH1F *denergypTrue_sabelotodo[NREACTION];
    TH1F *denergypTopo[NREACTION];
    TH1F *denergypFermi[NREACTION];
    TGraphErrors *eff[NREACTION];
    TGraphErrors *pur[NREACTION];
    TGraphErrors *FOM[NREACTION];  
    TH1F *postrackEvent[NREACTION];
    TH2F *postrackNBvsprotonNB[NREACTION];
    TH2F *postrackNBvspiplusNB[NREACTION];
    TH1F *muonNB_1negtrack[NREACTION];

    TH1F *dEnuTrueRec[NREACTION];
    TH1F *dEnuTrueTrue[NREACTION];
    TH1F *dEnuTrueTrue_blind[NREACTION];
    TH1F *dEmuTrueRec[NREACTION];
    TH1F *dEnTrueRec[NREACTION];
    TH1F *dEpTrueRec[NREACTION];

    TH2F *Sel_Enuvsq2[NREACTION];
    TH2F *Seltrue_Enuvsq2[NREACTION];
    TH2F *True_Enuvsq2[NREACTION];
    TH2F *Eff_Enuvsq2[NREACTION];
    TH2F *Pur_Enuvsq2[NREACTION];

    TH2F *Sel_EmuvsThmu[NREACTION];
    TH2F *Seltrue_EmuvsThmu[NREACTION];
    TH2F *True_EmuvsThmu[NREACTION];
    TH2F *Eff_EmuvsThmu[NREACTION];
    TH2F *Pur_EmuvsThmu[NREACTION];

        
    TH1F *EnuRecTrue [NREACTION];
    TH1F *EnuRecSel[NREACTION];
    TH1F *EnuRecSelTrue[NREACTION];
    TH1F *EnuRecSelTopo[NREACTION];
    TH1F *EnuRecSelTrueTopo[NREACTION];
    TH1F *PurityvsEnuRec[NREACTION];

    
    TH1F *EnuTrue [NREACTION];
    TH1F *EnuSel[NREACTION];
    TH1F *EnuSelTrue[NREACTION];
    TH1F *PurityvsEnu[NREACTION];
         
    TH1F *q2True[NREACTION];
    TH1F *q2Sel[NREACTION];
    TH1F *q2SelTrue[NREACTION];
    TH1F *PurityvsQ2[NREACTION];

    TH1F *EmuTrue [NREACTION];
    TH1F *EmuSel[NREACTION];
    TH1F *EmuSelTrue[NREACTION];
    TH1F *PurityvsEmu[NREACTION];

    TH1F *ThmuTrue [NREACTION];
    TH1F *ThmuSel[NREACTION];
    TH1F *ThmuSelTrue[NREACTION];
    TH1F *PurityvsThmu[NREACTION];


    
    TH1F *eff_vs_q2[NREACTION];
    TH1F *eff_vs_Enu[NREACTION];
    TH1F *eff_vs_X[NREACTION];
    TH1F *eff_vs_Y[NREACTION];
    TH1F *eff_vs_Emu[NREACTION];
    TH1F *eff_vs_Thmu[NREACTION];  

    TList *list=new TList();
    TFile *rootfile_output=new TFile("ccAnalysis_output.root","RECREATE");
    char reaction[NREACTION][256];
    int isreaction[NREACTION];

    for(int irea=0;irea<NREACTION;irea++)
    { 
        isreaction[irea]=0;
        char title[256];
        char name[256];
        if(irea==0) sprintf(reaction[irea],"ccqe");
        if(irea==1) sprintf(reaction[irea],"ccppiplus");
        if(irea==2) sprintf(reaction[irea],"ccppiplus_1piplus ");
        if(irea==3) sprintf(reaction[irea],"ccppiplus_1neg2pos ");
        if(irea==4) sprintf(reaction[irea],"ccqe_1neg1pos ");
        if(irea==5) sprintf(reaction[irea],"ccppiplus_1neg1pos ");
        if(irea==6) sprintf(reaction[irea],"CCall ");
        
        sprintf(title,"%s Efficiency, Purity and FOM ",reaction[irea]);
        sprintf(name,"eff_%s ",reaction[irea]);
        eff[irea]= new TGraphErrors();
        eff[irea]->SetNameTitle(name,title);
        sprintf(name,"pur_%s ",reaction[irea]);
        pur[irea]= new TGraphErrors();
        pur[irea]->SetNameTitle(name,title);
        sprintf(name,"FOM_%s ",reaction[irea]);
        FOM[irea]= new TGraphErrors();
        FOM[irea]->SetNameTitle(name,title);

        sprintf(title,"  EnuRec  for true %s  ",reaction[irea]);
        sprintf(name,"EnuRecTrue_%s",reaction[irea]);
        EnuRecTrue[irea] = new TH1F(name,title,100,0,8000);
        sprintf(title," Sel  EnuRec  for %s ",reaction[irea]);
        sprintf(name,"EnuRecSel_%s",reaction[irea]);
        EnuRecSel[irea] = new TH1F(name,title,100,0,8000);
        sprintf(title," Sel true  EnuRec  for %s ",reaction[irea]);
        sprintf(name,"EnuRecSelTrue_%s",reaction[irea]);
        EnuRecSelTrue[irea] = new TH1F(name,title,100,0,8000);
        sprintf(title," Sel  EnuRec topo for %s ",reaction[irea]);
        sprintf(name,"EnuRecSelTopo_%s",reaction[irea]);
        EnuRecSelTopo[irea] = new TH1F(name,title,100,0,8000);
        sprintf(title," Sel true  EnuRec topo  for %s ",reaction[irea]);
        sprintf(name,"EnuRecSelTrueTopo_%s",reaction[irea]);
        EnuRecSelTrueTopo[irea] = new TH1F(name,title,100,0,8000);       
        sprintf(title," Purity vs  EnuRectrue  for %s ",reaction[irea]);
        sprintf(name,"PurityvsEnuRec_%s",reaction[irea]);
        PurityvsEnuRec[irea]= new TH1F(name,title,100,0,8000);
        PurityvsEnuRec[irea]->SetXTitle("Energy (MeV)");
        
        sprintf(title,"  Enu  for true %s ",reaction[irea]);
        sprintf(name,"EnuTrue_%s",reaction[irea]);
        EnuTrue[irea] = new TH1F(name,title,100,0,8000);
        sprintf(title," Sel  Enu  for %s ",reaction[irea]);
        sprintf(name,"EnuSel_%s",reaction[irea]);
        EnuSel[irea] = new TH1F(name,title,100,0,8000);
        sprintf(title," Sel true  Enu  for %s ",reaction[irea]);
        sprintf(name,"EnuSelTrue_%s",reaction[irea]);
        EnuSelTrue[irea] = new TH1F(name,title,100,0,8000);
        sprintf(title," Purity vs  Enutrue  for %s ",reaction[irea]);
        sprintf(name,"PurityvsEnu_%s",reaction[irea]);
        PurityvsEnu[irea]= new TH1F(name,title,100,0,8000);
        PurityvsEnu[irea]->SetXTitle("Energy (MeV)");

        sprintf(title," Emu  for true %s ",reaction[irea]);
        sprintf(name,"EmuTrue_%s",reaction[irea]);
        EmuTrue[irea] = new TH1F(name,title,100,0,8000);
        sprintf(title," Sel  Emu  for %s ",reaction[irea]);
        sprintf(name,"EmuSel_%s",reaction[irea]);
        EmuSel[irea] = new TH1F(name,title,100,0,8000);
        sprintf(title," Sel true  Emu  for %s ",reaction[irea]);
        sprintf(name,"EmuSelTrue_%s",reaction[irea]);
        EmuSelTrue[irea] = new TH1F(name,title,100,0,8000);
        sprintf(title," Purity vs  Emutrue  for %s ",reaction[irea]);
        sprintf(name,"PurityvsEmu_%s",reaction[irea]);
        PurityvsEmu[irea]= new TH1F(name,title,100,0,8000);
        PurityvsEmu[irea]->SetXTitle("Energy (MeV)");

        sprintf(title," Thmu for true %s ",reaction[irea]);
        sprintf(name,"ThmuTrue_%s",reaction[irea]);
        ThmuTrue[irea] = new TH1F(name,title,60,0,3.14);
        sprintf(title," Sel  Thmu  for %s ",reaction[irea]);
        sprintf(name,"ThmuSel_%s",reaction[irea]);
        ThmuSel[irea] = new TH1F(name,title,60,0,3.14);
        sprintf(title," Sel true  Thmu  for %s ",reaction[irea]);
        sprintf(name,"ThmuSelTrue_%s",reaction[irea]);
        ThmuSelTrue[irea] = new TH1F(name,title,60,0,3.14);
        sprintf(title," Purity vs  Thmutrue  for %s ",reaction[irea]);
        sprintf(name,"PurityvsThmu_%s",reaction[irea]);
        PurityvsThmu[irea]= new TH1F(name,title,60,0,3.14);
        PurityvsThmu[irea]->SetXTitle("muon angle ");
        
        sprintf(title," q2  for true %s ",reaction[irea]);
        sprintf(name,"q2True_%s",reaction[irea]);
        q2True[irea] = new TH1F(name,title,100,0,3);
        sprintf(title," Sel q2  for %s ",reaction[irea]);
        sprintf(name,"q2 Sel_%s",reaction[irea]);
        q2Sel[irea] = new TH1F(name,title,100,0,3);
        sprintf(title," Sel true  q2  for %s ",reaction[irea]);
        sprintf(name,"q2 SelTrue_%s",reaction[irea]);
        q2SelTrue[irea] = new TH1F(name,title,100,0,3);
        sprintf(title," Purity vs q2 true  for %s ",reaction[irea]);
        sprintf(name,"PurityvsQ2_%s",reaction[irea]);
        PurityvsQ2[irea]= new TH1F(name,title,100,0,3);
        PurityvsQ2[irea]->SetXTitle("Energy (GeV)");
        
        sprintf(title," Total sel for Enu true and q2  for %s ",reaction[irea]);
        sprintf(name,"Sel_Enuvsq2_%s",reaction[irea]);
        Sel_Enuvsq2[irea]=new TH2F(name,title,10,0,3.,10,0,8000);
        sprintf(title," Total sel true for Enu true and q2  for %s ",reaction[irea]);
        sprintf(name,"Seltrue_Enuvsq2_%s",reaction[irea]);
        Seltrue_Enuvsq2[irea]=new TH2F(name,title,10,0,3.,10,0,8000);
        sprintf(title," Total true for Enu true and q2  for %s ",reaction[irea]);
        sprintf(name,"True_Enuvsq2_%s",reaction[irea]);
        True_Enuvsq2[irea]=new TH2F(name,title,10,0,3.,10,0,8000);
        sprintf(title," Efficiency for Enu true and q2  for %s ",reaction[irea]);
        sprintf(name,"Eff_Enuvsq2_%s",reaction[irea]);
        Eff_Enuvsq2[irea]=new TH2F(name,title,10,0,3.,10,0,8000);
        sprintf(title," Purity for Enu true and q2  for %s ",reaction[irea]);
        sprintf(name,"Pur_Enuvsq2_%s",reaction[irea]);
        Pur_Enuvsq2[irea]=new TH2F(name,title,10,0,3.,10,0,8000);

        sprintf(title," Total sel for Emu true and Thmu  for %s ",reaction[irea]);
        sprintf(name,"Sel_EmuvsThmu_%s",reaction[irea]);
        Sel_EmuvsThmu[irea]=new TH2F(name,title,10,0,3.,10,0,8000);
        sprintf(title," Total sel true for Emu true and Thmu  for %s ",reaction[irea]);
        sprintf(name,"Seltrue_EmuvsThmu_%s",reaction[irea]);
        Seltrue_EmuvsThmu[irea]=new TH2F(name,title,10,0,3.,10,0,8000);
        sprintf(title," Total true for Emu true and Thmu  for %s ",reaction[irea]);
        sprintf(name,"True_EmuvsThmu_%s",reaction[irea]);
        True_EmuvsThmu[irea]=new TH2F(name,title,10,0,3.,10,0,8000);
        sprintf(title," Efficiency for Emu true and Thmu  for %s ",reaction[irea]);
        sprintf(name,"Eff_EmuvsThmu_%s",reaction[irea]);
        Eff_EmuvsThmu[irea]=new TH2F(name,title,10,0,3.,10,0,8000);
        sprintf(title," Purity for Emu true and Thmu  for %s ",reaction[irea]);
        sprintf(name,"Pur_EmuvsThmu_%s",reaction[irea]);
        Pur_EmuvsThmu[irea]=new TH2F(name,title,10,0,3.,10,0,8000);
           
        sprintf(title," Enu_true - Enu_rec_seltrue for %s ",reaction[irea]);
        sprintf(name,"dEnuTrueRec_%s",reaction[irea]);
        dEnuTrueRec[irea]=new TH1F(name,title,100,-2000,2000);

        sprintf(title," Enu_true - Enu(EmuTrue) for %s ",reaction[irea]);
        sprintf(name,"dEnuTrueTrue_%s",reaction[irea]);
        dEnuTrueTrue[irea]=new TH1F(name,title,100,-2000,2000);

        sprintf(title," Enu_true - Enu(EmuTrue)ccqe for %s ",reaction[irea]);
        sprintf(name,"dEnuTrueTrue_B_%s",reaction[irea]);
        dEnuTrueTrue_blind[irea]=new TH1F(name,title,100,-2000,2000);
        
        sprintf(title," Emu_true - Emu_rec_seltrue for %s ",reaction[irea]);
        sprintf(name,"dEmuTrueRec_%s",reaction[irea]);
        dEmuTrueRec[irea]=new TH1F(name,title,100,-2000,2000);

        sprintf(title," En_true - (Mn+Eb) for %s ",reaction[irea]);
        sprintf(name,"dEnTrueRec_%s",reaction[irea]);
        dEnTrueRec[irea]=new TH1F(name,title,100,-200,200);

        sprintf(title," Ep_trueExp - Ep_recExp_seltrue for %s ",reaction[irea]);
        sprintf(name,"dEpTrueRecExp_%s",reaction[irea]);
        dEpTrueRec[irea]=new TH1F(name,title,100,-500,1000);

        sprintf(title," dThp vs dEp for %s",reaction[irea]);
        sprintf(name,"dThpvsdEp_%s",reaction[irea]);
        dThpvsdEp[irea]=new TH2F(name,title,200,-2000,2000,63,0,3.14);
        
        dEnuTrueRec[irea]->SetXTitle("Energy (MeV)");
        dEnuTrueTrue[irea]->SetXTitle("Energy (MeV)");
        dEnuTrueTrue_blind[irea]->SetXTitle("Energy (MeV)");
        dEmuTrueRec[irea]->SetXTitle("Energy (MeV)");
        dEnTrueRec[irea]->SetXTitle("Energy (MeV)");
        dEpTrueRec[irea]->SetXTitle("Energy (MeV)");
         
         
        sprintf(title,"%s Efficiency vs Muon angle ",reaction[irea]);
        sprintf(name,"eff_vs_Thmu_%s",reaction[irea]);
        eff_vs_Thmu[irea]=new TH1F(name,title,60,0,3.14);
        eff_vs_Thmu[irea]->SetXTitle("Muon Angle ");
        eff_vs_Thmu[irea]->SetYTitle("Efficiency");

        sprintf(title,"%s Efficiency vs Muon Energy ",reaction[irea]);
        sprintf(name,"eff_vs_Emu_%s",reaction[irea]);
        eff_vs_Emu[irea]=new TH1F(name,title,100,0,8000);
        eff_vs_Emu[irea]->SetXTitle("Energy (MeV) ");
        eff_vs_Emu[irea]->SetYTitle("Efficiency");
          
        sprintf(title,"%s Efficiency vs q2  ",reaction[irea]);
        sprintf(name,"eff_vs_q2_%s",reaction[irea]);
        eff_vs_q2[irea]=new TH1F(name,title,100,0.,3.);
        eff_vs_q2[irea]->SetXTitle("q2 (GeV^2)");   
        eff_vs_q2[irea]->SetYTitle("Efficiency");
        
        sprintf(title,"%s Efficiency vs X  ",reaction[irea]);
        sprintf(name,"eff_vs_X_%s",reaction[irea]);
        eff_vs_X[irea]=new TH1F(name,title,60,-1000,1000);
        eff_vs_X[irea]->SetXTitle("X (cm)");   
        eff_vs_X[irea]->SetYTitle("Efficiency");
        
        sprintf(title,"%s Efficiency vs Y  ",reaction[irea]);
        sprintf(name,"eff_vs_Y_%s",reaction[irea]);
        eff_vs_Y[irea]=new TH1F(name,title,100,-1000,1000);
        eff_vs_Y[irea]->SetXTitle("Y (cm)");   
        eff_vs_Y[irea]->SetYTitle("Efficiency");

        sprintf(title,"%s Efficiency vs Enu  ",reaction[irea]);
        sprintf(name,"eff_vs_Enu_%s",reaction[irea]);
        eff_vs_Enu[irea]=new TH1F(name,title,100,0,8000);
        eff_vs_Enu[irea]->SetXTitle("Enu (MeV)");   
        eff_vs_Enu[irea]->SetYTitle("Efficiency");
                
        sprintf(title,"delta theta p for %s",reaction[irea]);
        sprintf(name,"dthetap_%s",reaction[irea]);
        dthetap[irea]=new TH1F(name,title,63,0,3.14);
      
        sprintf(title,"Ep_RecExp-Ep_Rec for %s",reaction[irea]);
        sprintf(name,"denergy_%s",reaction[irea]);
        denergyp[irea]=new TH1F(name,title,1000,-10000,10000);

        sprintf(title,"Ep_RecExp-Ep_Rec for event sel. as %s",reaction[irea]);
        sprintf(name,"denergySel_%s",reaction[irea]);
        denergyp_sel[irea]=new TH1F(name,title,1000,-10000,10000);

        sprintf(title,"Ep_RecExp-Ep_Rec for event true %s and dth>0.5",reaction[irea]);
        sprintf(name,"HdenergySelTrue_%s",reaction[irea]);
        highdTHdenergyp_seltrue[irea]=new TH1F(name,title,1000,-10000,10000);

        sprintf(title,"Ep_RecExp-Ep_Rec for event true %s and dth<0.5",reaction[irea]);
        sprintf(name,"SdenergySelTrue_%s",reaction[irea]);
        smalldTHdenergyp_seltrue[irea]=new TH1F(name,title,1000,-10000,10000);

        //the proton energy is calculated with the nu energy :
        //if the reaction is ccqe, then using the formula of 2-body scattering
        //if the reaction is ccppi+, then the value of nuE is the one with the proton and pion at rest
        sprintf(title,"Ep_TrueExp(EnuExprea)-Ep_True for %s",reaction[irea]);
        sprintf(name,"denergyTrue_%s",reaction[irea]);
        denergypTrue[irea]=new TH1F(name,title,1000,-10000,10000);

        //the proton energy is calculated with the nu energy coming from a 2 body scattering (as if ccqe)
        sprintf(title,"Ep_TrueExp(EnuExpccqe)-Ep_True for %s",reaction[irea]);
        sprintf(name,"denergyTrue_B_%s",reaction[irea]);
        denergypTrue_blind[irea]=new TH1F(name,title,1000,-10000,10000);
        
        //the proton energy is calculated with taking the true value of the neutrino
         sprintf(title,"Ep_TrueExp-Ep_True for %s",reaction[irea]);
        sprintf(name,"denergyTrue_SLT_%s",reaction[irea]);
        denergypTrue_sabelotodo[irea]=new TH1F(name,title,1000,-10000,10000);

        
        sprintf(title,"delta energy p for %s when final part >2",reaction[irea]);
        sprintf(name,"denergyTopo_%s",reaction[irea]);
        denergypTopo[irea]=new TH1F(name,title,1000,-10000,10000);

        sprintf(title,"delta energy p for %s when E_target_true-E_target>15MeV",reaction[irea]);
        sprintf(name,"denergyFermi_%s",reaction[irea]);
        denergypFermi[irea]=new TH1F(name,title,1000,-10000,10000);
     
        
        sprintf(title,"Number of positive tracks for %s ",reaction[irea]);
        sprintf(name,"postrackEvent_%s",reaction[irea]);
        postrackEvent[irea]=new TH1F(name,title,4,0,4);

        sprintf(title,"Number of positive tracks vs number of proton for %s ",reaction[irea]);
        sprintf(name,"postrackNBvspNB_%s",reaction[irea]);
        postrackNBvsprotonNB[irea]=new TH2F(name,title,4,0,4,4,0,4);
        sprintf(title,"Number of positive tracks vs number of #pi^{+} for %s ",reaction[irea]);
        sprintf(name,"postrackNBvspiNB_%s",reaction[irea]);
        postrackNBvspiplusNB[irea]=new TH2F(name,title,4,0,4,4,0,4);
        sprintf(title,"Number of #mu^{-} for one 1neg track for %s ",reaction[irea]);
        sprintf(name,"muonNB_1negtrack_%s",reaction[irea]);
        muonNB_1negtrack[irea]=new TH1F(name,title,4,0,4);

        list= gDirectory->GetList();
    }
    // __________________________________________________________________________
    // Loop over all events.
  
    int entries = mcTruthVertices.GetEntries();
  
    std::cout << "ent: " << entries << std::endl;
          
    int protonNumber=0;
    int muonNumber=0;
    int neutronNumber=0;
    int pizeroNumber=0;
    int piplusNumber=0;
    int piminusNumber=0;

    int TotalInFiducialVolume=0;
    int TotalOfReaction[NREACTION];
    int TotalTrueEventSelected[NREACTION][NCUTS];
    int TotalEventSelected[NREACTION][NCUTS];
    int countreaction[NREACTION][NPIPLUS][NPROTONS][NPTracks][NCUTS]; //nb reactions, nb pions, nb protons, nb positive tracks, nb of cuts
    const int TOTAL=NREACTION*NCUTS*NPTracks*NPROTONS*NPIPLUS;
    const int TOTAL2=NREACTION*NCUTS*NPROTONS*NPIPLUS;
    int countrea[TOTAL];
    int TotalTrueEventTopo[TOTAL2];

    for(int irea=0;irea<NREACTION;irea++){
        TotalOfReaction[irea]=0;
        for(int icut=0;icut<NCUTS;icut++){
            TotalTrueEventSelected[irea][icut]=0;
            TotalEventSelected[irea][icut]=0;
            for(int ipi=0;ipi<NPIPLUS;ipi++){
                for(int ip=0;ip<NPROTONS;ip++){
                    int jjj=ip+NPROTONS*ipi+NPROTONS*NPIPLUS*irea;
                    TotalTrueEventTopo[jjj]=0;
                    for(int ipt=0;ipt<NPTracks;ipt++){
                        countreaction[irea][ipi][ip][ipt][icut]=0;
                        int iii=icut+NCUTS*ipt+NCUTS*NPTracks*ip+NCUTS*NPTracks*NPROTONS*ipi+NCUTS*NPTracks*NPROTONS*NPIPLUS*irea;
                        countrea[iii]=0;
                    }
                }
            }
        }
    }
   
    TTrackerAnalysisUtils *AnTUtils=new TTrackerAnalysisUtils();
    TCCtruthAnalysisUtils *trueAn=new TCCtruthAnalysisUtils();
    TCCreconAnalysisUtils *recAn=new TCCreconAnalysisUtils();

    std::vector< TLorentzVector > muMomentumVEC;
    std::vector<TLorentzVector > protMomentumVEC;
    
    for(int ient = 0; ient < entries; ient++)
    {       
        mcTruthVertices.GetEntry(ient);
        reconTracker.GetEntry(ient);
        if(ient%500 == 0)
            std::cout <<  "Events Processed " << ient << std::endl;

        if(ient>50000)
            std::cout <<  "Events Processed " << ient << std::endl;
        // ------------------------------------------------------------------------
        // Look at the true vertex
        // ------------------------------------------------------------------------
        
        ND::TTruthVerticesModule::TTruthVertex *true_fgd_vertex;
        
        if(NVtxFGD1 >0){
            true_fgd_vertex = (ND::TTruthVerticesModule::TTruthVertex*) VtxFGD1->UncheckedAt(0);
        }else if (NVtxFGD2 >0){
            true_fgd_vertex = (ND::TTruthVerticesModule::TTruthVertex*) VtxFGD2->UncheckedAt(0);
        }else{
            std::cerr << "There should be at least one vertex in one FGD.  Skipping event."<<std::endl;
            continue;
        }
        TLorentzVector VV;
        VV = true_fgd_vertex->Vertex;  
        
        TVector3 pos(VV.X(),VV.Y(),VV.Z());
        
        
          //-----------------------------------------------------------------
        // Get the cinematic values for topology study
        TLorentzVector proton_momentum;
        TVector3 proton_mom;
        if(true_fgd_vertex->BaryonIDs.size()>0){
           
            proton_momentum=trueAn->GetChargedBaryonMaxMomentum(true_fgd_vertex,2212);//take the one with max of energy
            proton_mom=TVector3(proton_momentum.X(),proton_momentum.Y(),proton_momentum.Z());     
        }
        
        double pEnergy=proton_momentum.T();//first element of protMomentumVEC
        
        TLorentzVector proton_th_momentum;
        TLorentzVector proton_th_momentum_sabelotodo;
        TLorentzVector proton_th_momentum_blind;
        double pMom_th=0;
        double pEnergy_th=0;
        double pEnergy_sabelotodo_th=0;
        double pEnergy_blind_th=0;
        if(ient>50000)
            cout<<"hhhhhhh0"<<endl;  
        
        protonNumber=trueAn->GetChargedBaryonNB(true_fgd_vertex,2212);
        piplusNumber=trueAn->GetChargedMesonNB(true_fgd_vertex,211);
        piminusNumber=trueAn->GetChargedMesonNB(true_fgd_vertex,-211);
        neutronNumber=trueAn->GetOtherNeutralNB(true_fgd_vertex,2112);
        pizeroNumber=trueAn->GetOtherNeutralNB(true_fgd_vertex,111);
        muonNumber=trueAn->GetChargedLeptonNB(true_fgd_vertex,13);
        
        double totfinalpart=0;
        double nTargetMom2=0;
        double nuMom2=0;
        double q2 =0;
        bool isfid=false;
        
        totfinalpart=neutronNumber+protonNumber+piplusNumber+pizeroNumber+piminusNumber;
        if(ient>50000)
            cout<<"hhhhhhh1"<<endl;  
        TLorentzVector muon_momentum;
        if(true_fgd_vertex->LeptonIDs.size()>0){
           
            muon_momentum=trueAn->GetChargedLeptonMaxMomentum(true_fgd_vertex,13);
        }
        //    cout<<ient<<endl;
        //by defaut all vector are initialized at 0
        TLorentzVector neutrino_momentum;
        TLorentzVector nucleon_target_momentum;
        TVector3 neutrino_mom;
        TVector3 nucleon_target_mom;
        TVector3 muon_mom;

        muon_mom =TVector3(muon_momentum.X(),muon_momentum.Y(),muon_momentum.Z());     
        neutrino_momentum = true_fgd_vertex->MomNu;
        neutrino_mom =TVector3(neutrino_momentum.X(),neutrino_momentum.Y(),neutrino_momentum.Z());     
        
        nucleon_target_momentum=true_fgd_vertex->MomTarget;
        nucleon_target_mom=TVector3(nucleon_target_momentum.X(),nucleon_target_momentum.Y(),nucleon_target_momentum.Z());     
        
        nTargetMom2=nucleon_target_mom.Mag2();
        nuMom2=neutrino_mom.Mag2();
        double mumom=muon_mom.Mag();
        double emu=muon_momentum.T();
        float eb = -27.0;double mp = 938.27;double mn = 939.56;  double mmu= 105.65;double mpi=139.57; //MeV  

        double nuEtrueExp[2];
        nuEtrueExp[0]= mp*mp/2+(mn+eb)*emu-(2.*mn*eb+eb*eb+mn*mn+mmu*mmu)/2.;
        nuEtrueExp[1]= (pEnergy+mpi)*(pEnergy+mpi)/2+(mn+eb)*emu-(2.*mn*eb+eb*eb+mn*mn+mmu*mmu)/2.;
        double costheta=0;
        double thetaMU=0;
        if(mumom>0){
            costheta=muon_momentum.Z()/mumom;
            thetaMU=TMath::ACos(costheta);
        }
        // cout<<"mu.Z "<<muon_momentum.Z()<<endl;
        double costh=recAn->GetRecCosTheta();
        if(costh>0 && costheta>0)
            dThetaTrueRec->Fill(costheta-costh);
        
        if((mn+eb-emu+mumom*costheta)>0){
            nuEtrueExp[0] /= (mn+eb-emu+mumom*costheta);
            nuEtrueExp[1] /= (mn+eb-emu+mumom*costheta);
        }
       
       
        //-----------------------------------------------------------------
        // Get the q2 value
    if(ient>50000)
          cout<<"hhhhhhh2"<<endl;  
        if(true_fgd_vertex->LeptonIDs.size()>0){
            q2=trueAn->GetQ2(neutrino_momentum,muon_momentum);         
        }
        isfid=AnTUtils->IsInFiducialVolume(pos);
        
        //   cout<<"hhh"<<endl;
        for(int irea=0;irea<NREACTION;irea++){
            isreaction[irea]=0;
            isreaction[irea]=(int)trueAn->IsFiducialReaction(true_fgd_vertex,irea,isfid);
            
        }
       
        if(ient>50000)
            cout<<"hhhhhhh3"<<endl;
         
//         if(isreaction[0]){
                            
//             if( q2 < 0 && true_fgd_vertex->LeptonIDs.size()>0) {
//                 std::cout << " --------------------------------- " << std::endl; 
//                 std::cout << neutrino_momentum.Mag() << "   " << muon_momentum.Mag() << std::endl; 
//                 std::cout << neutrino_momentum.T() << " q2 "<<q2<<std::endl; 
//             }
//         }
        // -------------------------------------------------------------------------
        // Apply event selection criterion.
        // -------------------------------------------------------------------------
        double DeltaThetaP=999;
        double DeltaEnergyP=99999;
        int kTotalTPC=0;
        bool oneAn=false;
        bool isHigh=false;
        kTotalTPC=recAn->GetTotalTracksInTPC(Tracks,NTracks);
        
        recAn->ResetCutFlag();
        
        if(NTracks == 1 && kTotalTPC==1 )
            oneAn=recAn->OneTrackAnalysis(Tracks);      
        else if(NTracks >= 2 && kTotalTPC > 0 )
            recAn->MultipleTracksAnalysis(Tracks, NTracks, Vertices, NVertices);

        if(ient>50000)
            cout<<"hhhhhhh4"<<endl;      
        recAn->HasMichelElectron(DelayedClusters, NDelayedClusters);

        DeltaThetaP=recAn->GetDeltaThetaP();
        DeltaEnergyP=recAn->GetDeltaEnergyP(false);
        
        if(DeltaThetaP<999){
            for(int irea=0;irea<2;irea++){
                isHigh= recAn->HighDeltaThetaP(DeltaThetaP);
                dthetapTOT->Fill(DeltaThetaP);
                 dThpvsdEpTOT->Fill(DeltaEnergyP,DeltaThetaP);
                if(isreaction[irea]){
                    dthetap[irea]->Fill(DeltaThetaP);
                    if(irea==0 && totfinalpart>=2 && muonNumber==1) { 
                        dthetapTopo->Fill(DeltaThetaP);
                        
                    }
                    if(DeltaEnergyP<99999){
                        denergyp[irea]->Fill(DeltaEnergyP);
                      if(irea==0 && (sqrt(nTargetMom2+(mn+eb)*(mn+eb))-(mn+eb))>20)  
                          denergypFermi[0]->Fill(DeltaEnergyP);
                      if(irea==0 &&  totfinalpart>=2 && muonNumber==1) {
                          denergypTopo[0]->Fill(DeltaEnergyP);
                      }
                      dThpvsdEp[irea]->Fill(DeltaEnergyP,DeltaThetaP);
                    }
                }
            }
        }
        
        if(DeltaEnergyP<99999){     
            DeltaEnergyP=recAn->GetDeltaEnergyP(isHigh);
            denergypTOT->Fill(DeltaEnergyP);
            if(!isHigh){
                denergyp_sel[0]->Fill(DeltaEnergyP);
                
                if(isreaction[0])
                    smalldTHdenergyp_seltrue[0]->Fill(DeltaEnergyP);
                if(isreaction[1])
                    smalldTHdenergyp_seltrue[1]->Fill(DeltaEnergyP);
            }
            else{
                recAn->HighDeltaEp(DeltaEnergyP);
                denergyp_sel[1]->Fill(DeltaEnergyP);
                if(isreaction[0])
                    highdTHdenergyp_seltrue[0]->Fill(DeltaEnergyP);

                if(isreaction[1])
                    highdTHdenergyp_seltrue[1]->Fill(DeltaEnergyP);
            }
        }
        if(ient>50000)
            cout<<"hhhhhhh5"<<endl;  
           
                        
        for(int irea=0;irea<2;irea++){
            if(isreaction[irea]){
                dEnuTrueTrue_blind[irea]->Fill(sqrt(nuMom2)-nuEtrueExp[0]);
                dEnuTrueTrue[irea]->Fill(sqrt(nuMom2)-nuEtrueExp[irea]);
                if( true_fgd_vertex->LeptonIDs.size()>0){
                    proton_th_momentum_sabelotodo=nucleon_target_momentum+neutrino_momentum-muon_momentum;
                    proton_th_momentum=nucleon_target_momentum-muon_momentum;
                    proton_th_momentum_blind=nucleon_target_momentum-muon_momentum;
                    
                    pMom_th = sqrt(pow(proton_th_momentum.X(),2) +pow( proton_th_momentum.Y(),2) + pow(proton_th_momentum.Z(),2));
                    pEnergy_th=proton_th_momentum.T()+eb+nuEtrueExp[irea];
                    pEnergy_blind_th=proton_th_momentum_blind.T()+eb+nuEtrueExp[0];
                    pEnergy_sabelotodo_th=proton_th_momentum_sabelotodo.T()+eb;
                }
                
                double deltaE=pEnergy_th-pEnergy;
                double deltaE_blind=pEnergy_blind_th-pEnergy;
                double deltaE_sabelotodo=pEnergy_sabelotodo_th-pEnergy;
                if(pEnergy_th>0 && pEnergy>0)
                    denergypTrue[irea]->Fill(deltaE);

                if(pEnergy_blind_th>0 && pEnergy>0)
                    denergypTrue_blind[irea]->Fill(deltaE_blind);

                if(pEnergy_sabelotodo_th>0 && pEnergy>0)
                    denergypTrue_sabelotodo[irea]->Fill(deltaE_sabelotodo);
                    
            }
        }
        
        
        recAn->Is1NegXPosEvent(2);

        if(isfid)
            TotalInFiducialVolume++;
       
        for(int irea=0;irea<NREACTION;irea++){
            double enu=0;
            enu=recAn->GetRecNeutrinoEnergy(irea);       
              
            if(isreaction[irea]){
                EnuRecTrue[irea]->Fill(enu);
                EmuTrue[irea]->Fill(mumom);
                ThmuTrue[irea]->Fill(thetaMU);
                EnuTrue[irea]->Fill(sqrt(nuMom2));
                q2True[irea]->Fill(q2);
                True_Enuvsq2[irea]->Fill(q2,sqrt(nuMom2));
                True_EmuvsThmu[irea]->Fill(thetaMU,mumom); 
                TotalOfReaction[irea]++;
                int jjj=protonNumber+NPROTONS*piplusNumber+NPROTONS*NPIPLUS*irea;
                TotalTrueEventTopo[jjj]++;
            }
            for(int icut=0;icut<NCUTS;icut++){
            
                bool iscut=false;
                iscut=recAn->DefineCut(irea,icut);                                 
                if(iscut){
                    TotalEventSelected[irea][icut]++;
                   
                    if(icut==3 )
                    {
                        EnuSel[irea]->Fill(sqrt(nuMom2));
                        EmuSel[irea]->Fill(mumom);
                        ThmuSel[irea]->Fill(thetaMU);
                        q2Sel[irea]->Fill(q2);
                        Sel_Enuvsq2[irea]->Fill(q2,sqrt(nuMom2));
                        Sel_EmuvsThmu[irea]->Fill(thetaMU,mumom);
                        EnuRecSel[irea]->Fill(enu);
                        if( totfinalpart>=2 && muonNumber==1)  
                            EnuRecSelTopo[irea]->Fill(enu);
                    }
                    if(isreaction[irea]){
                        TotalTrueEventSelected[irea][icut]++;
                       
                        if(icut==3){
                            EmuSelTrue[irea]->Fill(mumom);
                            ThmuSelTrue[irea]->Fill(thetaMU);
                            EnuSelTrue[irea]->Fill(sqrt(nuMom2));
                            q2SelTrue[irea]->Fill(q2);
                            Seltrue_EmuvsThmu[irea]->Fill(thetaMU,mumom);
                            Seltrue_Enuvsq2[irea]->Fill(q2,sqrt(nuMom2));
                            dEnuTrueRec[irea]->Fill(sqrt(nuMom2)-enu);
                            double muE_rec=0;
                            muE_rec=recAn->GetRecMuonEnergy();
                            if(muE_rec>0)
                                dEmuTrueRec[irea]->Fill(muon_momentum.T()-muE_rec);

                            dEnTrueRec[irea]->Fill( nucleon_target_momentum.T()-(mn+eb));
                            double pE_recExp=0;
                            pE_recExp=recAn->GetRecExpProtonEnergy(irea);
                            if(pE_recExp>0)
                                dEpTrueRec[irea]->Fill(pEnergy_th-pE_recExp);                          
                            EnuRecSelTrue[irea]->Fill(enu);
                            if( totfinalpart>=2 && muonNumber==1)  
                                EnuRecSelTrueTopo[irea]->Fill(enu);
                            
                        }
                    }
                }
            }
        }
        if(ient>50000)
            cout<<"hhhhhhh6"<<endl;  
    }//end ientries
    
    
    cout<<"\n -------------------------------------------------------------------------"<<endl;

    
    for(int irea=0;irea<NREACTION;irea++){
           
        Eff_EmuvsThmu[irea]->Divide(Seltrue_EmuvsThmu[irea],True_EmuvsThmu[irea],1,1," ");
        Eff_Enuvsq2[irea]->Divide(Seltrue_Enuvsq2[irea],True_Enuvsq2[irea],1,1," ");
        
        Pur_EmuvsThmu[irea]->Divide(Seltrue_EmuvsThmu[irea],Sel_EmuvsThmu[irea],1,1," ");
        Pur_Enuvsq2[irea]->Divide(Seltrue_Enuvsq2[irea],Sel_Enuvsq2[irea],1,1," ");

        PurityvsThmu[irea]->Divide(ThmuSelTrue[irea],ThmuSel[irea],1,1," ");
        PurityvsEmu[irea]->Divide(EmuSelTrue[irea],EmuSel[irea],1,1," ");
        PurityvsEnu[irea]->Divide(EnuSelTrue[irea],EnuSel[irea],1,1," ");
        PurityvsEnuRec[irea]->Divide(EnuRecSelTrue[irea],EnuRecSel[irea],1,1," ");
        PurityvsQ2[irea]->Divide(q2SelTrue[irea],q2Sel[irea],1,1," ");
        eff_vs_Emu[irea]->Divide(EmuSelTrue[irea],EmuTrue[irea],1,1," ");
        eff_vs_Thmu[irea]->Divide(ThmuSelTrue[irea],ThmuTrue[irea],1,1," ");
        eff_vs_q2[irea]->Divide(q2SelTrue[irea],q2True[irea],1,1," ");
        eff_vs_Enu[irea]->Divide(EnuSelTrue[irea],EnuTrue[irea],1,1," ");

        
        if(TotalInFiducialVolume>0){
           cout<<"total of true "<<reaction[irea]<<" in fiducial volume: "<<TotalOfReaction[irea]
               <<" over total event in fiducial: "<<TotalInFiducialVolume<<"  =>  "
               <<(double) TotalOfReaction[irea]/((double)TotalInFiducialVolume)*100 << " % "<<endl;
        }
        cout<<"total  "<<reaction[irea]<<" selected and true "<<reaction[irea]<<" fiducial       ";
        for(int icut=0;icut<NCUTS;icut++)
            cout<<TotalTrueEventSelected[irea][icut]<<"   ";
               
        cout<<"\n total of  "<<reaction[irea]<<" selected                      ";
        for(int icut=0;icut<NCUTS;icut++)
            cout<< TotalEventSelected[irea][icut]<<"   ";

        cout<<" "<<endl;
        
        for(int icut=0;icut<NCUTS;icut++){
            if( TotalOfReaction[irea] > 0){
                double EffError=recAn->GetEfficiencyError(TotalTrueEventSelected[irea][icut],TotalEventSelected[irea][icut]);
                std::cout << "Efficiency: " <<recAn->GetEfficiency(TotalTrueEventSelected[irea][icut],TotalOfReaction[irea])
                          <<" Error: "<<EffError
                          <<std::endl;
                eff[irea]->SetPoint(icut,icut+1,recAn->GetEfficiency(TotalTrueEventSelected[irea][icut],TotalOfReaction[irea])*100.0);
                eff[irea]->SetPointError(icut,0.0001,EffError*100.0);
            }               
            if(TotalEventSelected[irea][icut]> 0){
                double StatSignError=recAn->GetStatSignificanceError(TotalTrueEventSelected[irea][icut],TotalEventSelected[irea][icut]);
                double PurityError=recAn->GetPurityError(TotalTrueEventSelected[irea][icut],TotalEventSelected[irea][icut]);
                
                std::cout << "Purity: "  <<recAn->GetPurity(TotalTrueEventSelected[irea][icut],TotalEventSelected[irea][icut])
                          <<" Error: "<<PurityError<<std::endl;
                pur[irea]->SetPoint(icut,icut+1,recAn->GetPurity(TotalTrueEventSelected[irea][icut],TotalEventSelected[irea][icut])*100.0);
                pur[irea]->SetPointError(icut,0.0001,PurityError*100.0);

                std::cout << "Stat Sign.: "  <<recAn->GetStatSignificance(TotalTrueEventSelected[irea][icut],TotalEventSelected[irea][icut])
                          <<" Error: "<<StatSignError
                          <<std::endl;

                FOM[irea]->SetPoint(icut,icut+1,recAn->GetStatSignificance(TotalTrueEventSelected[irea][icut],TotalEventSelected[irea][icut]));
                FOM[irea]->SetPointError(icut,0.0001,StatSignError);
            }
        }
        cout<<".................................................................................."<<endl;

    }
   
    delete trueAn;
    delete recAn;
    delete AnTUtils;
    
    gStyle->SetPalette(1);
    TCanvas *cc=new TCanvas("cc");
    cc->SetFillColor(0);cc->Divide(2,2);
    dthetap[0]->SetFillColor(5);
    dthetap[1]->SetFillColor(8);
    dthetapTopo->SetFillColor(7);
    cc->cd(1);
    dthetapTOT->DrawCopy();
    cc->cd(2);
    dthetap[0]->DrawCopy();
    dthetapTopo->Draw("same");
    cc->cd(3);
    dthetap[1]->DrawCopy();
    cc->cd(4);
    dthetapTOT->DrawCopy();
    dthetap[0]->Draw("same");
    dthetap[1]->Draw("same");
    cc->Update();
    cc->Write();
   
  
   
    for(int irea=0;irea<2;irea++){
        denergyp[irea]->SetFillColor(5+irea*3);
        denergyp_sel[irea]->SetFillColor(5+irea*3);
        highdTHdenergyp_seltrue[irea]->SetFillColor(5+irea*3);
        smalldTHdenergyp_seltrue[irea]->SetFillColor(5+irea*3);
        denergypTopo[irea]->SetFillColor(6+irea);
        denergypTrue[irea]->SetFillColor(5+irea*3);
        denergypTrue_blind[irea]->SetFillColor(5+irea*3);
        denergypTrue_sabelotodo[irea]->SetFillColor(5+irea*3);
    }

    TCanvas *cc1=new TCanvas("cc1");
    cc1->SetFillColor(0);
    cc1->Divide(2,2);
    cc1->cd(1);
    denergypTrue[0]->DrawCopy();
    denergypTrue[1]->Draw("same");
    cc1->cd(2);
    denergypTrue_blind[0]->DrawCopy();
    denergypTrue_blind[1]->Draw("same");
    cc1->cd(3);
    denergypTrue_sabelotodo[0]->DrawCopy();
    denergypTrue_sabelotodo[1]->Draw("same");
    cc1->Update();
    cc1->Write();

    TCanvas *cc11=new TCanvas("cc11");
    cc11->SetFillColor(0);
    cc11->Divide(2,2);
    cc11->cd(1);
    denergyp[0]->DrawCopy();
    denergyp[1]->Draw("same");
    cc11->cd(2);
    denergyp_sel[0]->DrawCopy();
    denergyp_sel[1]->Draw("same");
    cc11->cd(3);
    denergyp_sel[1]->DrawCopy();
    highdTHdenergyp_seltrue[1]->SetFillColor(29);
    highdTHdenergyp_seltrue[1]->Draw("same");
    highdTHdenergyp_seltrue[0]->SetFillColor(41);
    highdTHdenergyp_seltrue[0]->Draw("same");
    cc11->cd(4);
    denergyp_sel[0]->DrawCopy();
    smalldTHdenergyp_seltrue[0]->SetFillColor(41);
    smalldTHdenergyp_seltrue[0]->Draw("same");
    smalldTHdenergyp_seltrue[1]->SetFillColor(29);
    smalldTHdenergyp_seltrue[1]->Draw("same");
    cc11->Update();
    cc11->Write();
    
    TCanvas *cc12=new TCanvas("cc12");
    cc12->SetFillColor(0);
    cc12->Divide(2,3);
    cc12->cd(1);
    dThpvsdEp[0]->DrawCopy("colz");
    cc12->cd(2);
    dThpvsdEp[1]->DrawCopy("colz");
    cc12->cd(3);
    denergyp[0]->DrawCopy();
    denergypTopo[0]->Draw("same");
    cc12->cd(4);
    denergyp[0]->DrawCopy();
    denergypFermi[0]->SetFillColor(9);
    denergypFermi[0]->Draw("same");
    cc12->cd(5);
    dThpvsdEpTOT->DrawCopy("colz");
    cc12->Update();
    cc12->Write();

    //--------------------------------------------------------------

    TCanvas *cc2[NREACTION];
    TCanvas *cc21[NREACTION];
    TCanvas *cc22[NREACTION];
    TCanvas *cc23[NREACTION];
    TCanvas *cc24[NREACTION];
    for(int irea=0;irea<NREACTION;irea++){
        char name[256];
        sprintf(name,"cc2_%s",reaction[irea]);
        cc2[irea]=new TCanvas(name);
        cc2[irea]->SetFillColor(0);
        cc2[irea]->Divide(2,2);
        cc2[irea]->cd(1);
        PurityvsEnu[irea]->DrawCopy();
        cc2[irea]->cd(2);
        EnuSel[irea]->DrawCopy();
        cc2[irea]->cd(3);
        EnuSelTrue[irea]->DrawCopy();
        cc2[irea]->cd(4);
        EnuTrue[irea]->DrawCopy();
        cc2[irea]->Update();
        cc2[irea]->Write();
        
        sprintf(name,"cc21_%s",reaction[irea]);
        cc21[irea]=new TCanvas(name);
        cc21[irea]->SetFillColor(0);
        cc21[irea]->Divide(2,2);
        cc21[irea]->cd(1);
        PurityvsEnuRec[irea]->DrawCopy();
        cc21[irea]->cd(2);
        EnuRecSel[irea]->DrawCopy();
        cc21[irea]->cd(3);
        EnuRecSelTrue[irea]->DrawCopy();
        cc21[irea]->cd(4);
        EnuRecTrue[irea]->DrawCopy();
        cc21[irea]->Update();
        cc21[irea]->Write();
         
        sprintf(name,"cc22_%s",reaction[irea]);
        cc22[irea]=new TCanvas(name);
        cc22[irea]->SetFillColor(0);
        cc22[irea]->Divide(2,2);
        cc22[irea]->cd(1);
        PurityvsQ2[irea]->DrawCopy();
        cc22[irea]->cd(2);
        q2Sel[irea]->DrawCopy();
        cc22[irea]->cd(3);
        q2SelTrue[irea]->DrawCopy();
        cc22[irea]->cd(4);
        q2True[irea]->DrawCopy();
        cc22[irea]->Update();
        cc22[irea]->Write();
        
        sprintf(name,"cc23_%s",reaction[irea]);
        cc23[irea]=new TCanvas(name);
        cc23[irea]->SetFillColor(0);
        cc23[irea]->Divide(2,2);
        cc23[irea]->cd(1);
        PurityvsEmu[irea]->DrawCopy();
        cc23[irea]->cd(2);
        EmuSel[irea]->DrawCopy();
        cc23[irea]->cd(3);
        EmuSelTrue[irea]->DrawCopy();
        cc23[irea]->cd(4);
        EmuTrue[irea]->DrawCopy();
        cc23[irea]->Update();
        cc23[irea]->Write();

        sprintf(name,"cc24_%s",reaction[irea]);
        cc24[irea]=new TCanvas(name);
        cc24[irea]->SetFillColor(0);
        cc24[irea]->Divide(2,2);
        cc24[irea]->cd(1);
        PurityvsThmu[irea]->DrawCopy();
        cc24[irea]->cd(2);
        ThmuSel[irea]->DrawCopy();
        cc24[irea]->cd(3);
        ThmuSelTrue[irea]->DrawCopy();
        cc24[irea]->cd(4);
        ThmuTrue[irea]->DrawCopy();
        cc24[irea]->Update();
        cc24[irea]->Write();
    }
     
    TCanvas *ceff0[NREACTION];
    TCanvas *ceff00[NREACTION];
    for(int irea=0;irea<NREACTION;irea++)
    {
        char name[256];
        sprintf(name,"ceff0_%s",reaction[irea]);
        ceff0[irea]=new TCanvas(name);
        sprintf(name,"ceff00_%s",reaction[irea]);
        ceff00[irea]=new TCanvas(name);

        ceff0[irea]->SetFillColor(10);
        ceff0[irea]->Divide(2,3);
        ceff0[irea]->cd(1);
        dEnuTrueRec[irea]->DrawCopy();
        ceff0[irea]->cd(2);
        dEmuTrueRec[irea]->DrawCopy();
        ceff0[irea]->cd(3);
        dEnTrueRec[irea]->DrawCopy();
        ceff0[irea]->cd(4);
        dEpTrueRec[irea]->DrawCopy();
        ceff0[irea]->cd(5);
        dEnuTrueTrue[irea]->DrawCopy();
        ceff0[irea]->cd(6);
        dEnuTrueTrue_blind[irea]->DrawCopy();
        ceff0[irea]->Update();
        ceff0[irea]->Write();
        
        ceff00[irea]->SetFillColor(10);
        ceff00[irea]->Divide(2,2);
        ceff00[irea]->cd(1);
        Eff_EmuvsThmu[irea]->DrawCopy("colz");
        ceff00[irea]->cd(2);
        Eff_Enuvsq2[irea]->DrawCopy("colz");
        ceff00[irea]->cd(3);
        Pur_EmuvsThmu[irea]->DrawCopy("colz");
        ceff00[irea]->cd(4);
        Pur_Enuvsq2[irea]->DrawCopy("colz");
        ceff00[irea]->Update();

        ceff00[irea]->Write();
        
    }

    //--------------------------------------------------------------
    //  plot eff and purity plot
    //--------------------------------------------------------------
    TCanvas *ceff;
    ceff=new TCanvas("ceff"); ceff->SetFillColor(10);
    ceff->Divide(4,2);
    TCanvas *ceff1[NREACTION];
    TCanvas *ceff11[NREACTION];
    TMultiGraph *mg[NREACTION];
    
    for(int irea=0;irea<NREACTION;irea++)
    {
        char name[256];
        char title[256];
          
        ceff->cd(irea+1);
        eff[irea]->Draw("APL*");
        eff[irea]->GetXaxis()->SetLabelSize(0);
        eff[irea]->SetMarkerStyle(7);
        eff[irea]->SetMarkerColor(4);
        eff[irea]->SetLineColor(4);
        eff[irea]->SetMarkerSize(eff[irea]->GetMarkerSize()*1.8);
        eff[irea]->GetYaxis()->SetTitle("Percentage");
        sprintf(title,"%s Efficiency, Purity and FOM ",reaction[irea]);
        eff[irea]->SetTitle(title);
        eff[irea]->SetMaximum(110);
        eff[irea]->SetMinimum(0);
        eff[irea]->GetXaxis()->SetNdivisions(4);
        
        pur[irea]->Draw("*L");
        pur[irea]->SetMarkerStyle(7);
        pur[irea]->SetMarkerColor(2);
        pur[irea]->SetLineColor(2);
        pur[irea]->SetMarkerSize(pur[irea]->GetMarkerSize()*1.8);

        FOM[irea]->Draw("*L");
        FOM[irea]->SetMarkerStyle(7);
        FOM[irea]->SetMarkerColor(7);
        FOM[irea]->SetLineColor(7);
        FOM[irea]->SetMarkerSize(FOM[irea]->GetMarkerSize()*1.8);

        TLatex t;
        t.SetTextSize(t.GetTextSize()*0.5);
        t.DrawText(0.8,-7,"No Cut");
        t.DrawText(1.8,-7,"Fiducial FGD");
        t.DrawText(1.7,-12,"Vertex Cut");
        t.DrawText(2.8,-7,"1 Neg TPC");
        t.DrawText(2.8,-12,"Track Cut");
        if(irea==6){
            t.DrawText(3.8,-7,"no other  ");
        }
        else
            t.DrawLatex(3.8,-7,"FGD #pi^{+} Cut");
        
        if(irea==0 || irea==5 || irea==4)
            t.DrawText(3.8,-12,"or dth_p Cut");
        else if(irea==6)
            t.DrawText(3.8,-12," cut  ");
        else
            t.DrawText(3.8,-12,"dth_p cut or 1-,2+ track");
        //   t.DrawText(4.8,-7,"dEp Cut");
        
        TLegend *leg = new TLegend(0.6,0.68,0.89,0.85);
        leg->SetFillColor(10);
        leg->AddEntry(eff[irea],"Efficiency (%)","p");
        leg->AddEntry(pur[irea],"Purity (%)","p");
        leg->AddEntry(FOM[irea],"Stat. Sign. (FOM)","p");
        leg->Draw("SAME");

        mg[irea]=new TMultiGraph();
        sprintf(name,"gr_%s",reaction[irea]);
        sprintf(title,"%s Efficiency, Purity and FOM ",reaction[irea]);
        mg[irea]->SetTitle(title);
        mg[irea]->SetName(name);
        mg[irea]->Add(eff[irea],"lp ");
        mg[irea]->Add(pur[irea],"lp ");
        mg[irea]->Add(FOM[irea],"lp ");
        mg[irea]->Write();
        
        sprintf(name,"ceff1_%s",reaction[irea]);
        ceff1[irea]=new TCanvas(name);
        ceff1[irea]->SetFillColor(10);
        ceff1[irea]->Divide(2,2);
        
        ceff1[irea]->cd(1);
        q2SelTrue[irea]->DrawCopy();
        ceff1[irea]->cd(2);        
        eff_vs_q2[irea]->DrawCopy();
        ceff1[irea]->cd(3);
        EnuSelTrue[irea]->DrawCopy();
        ceff1[irea]->cd(4);
        eff_vs_Enu[irea]->DrawCopy();
         ceff1[irea]->Update();
        ceff1[irea]->Write();

        sprintf(name,"ceff11_%s",reaction[irea]);
        ceff11[irea]=new TCanvas(name);
        ceff11[irea]->SetFillColor(10);
        ceff11[irea]->Divide(2,2);
        ceff11[irea]->cd(1);
        ThmuSelTrue[irea]->DrawCopy();
        ceff11[irea]->cd(2);        
        eff_vs_Thmu[irea]->DrawCopy();
        ceff11[irea]->cd(3);
        EmuSelTrue[irea]->DrawCopy();
        ceff11[irea]->cd(4);
        eff_vs_Emu[irea]->DrawCopy();
        ceff11[irea]->Update();
        ceff11[irea]->Write();

      
    }
    //ceff->Update();
    ceff->Write();    
    list->Write("allhists",TObject::kSingleKey);
    rootfile_output->Close(); 

    // return;
   

}    
