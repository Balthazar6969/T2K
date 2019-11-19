#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <iomanip>
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
#include <AnalysisTools/TCCplotAnalysisUtils.hxx>


// #include "/T2K/nd/oaEvent/v6r7p3/src/EoaCore.hxx"
// #include "/T2K/nd/oaEvent/v6r7p3/src/ToaEvent.hxx"
// #include "/T2K/nd/oaEvent/v6r7p3/src/TND280Event.hxx"
// #include "../src/EoaAnalysis.hxx"
// #include "../src/oaAnalysis_version.h"
// #include "../src/TTruthVerticesModule.hxx"
// #include "../src/TTrackerReconModule.hxx"
// #include "../src/TAnalysisHeaderModuleBase.hxx"
// //#include <AnalysisTools/TCCreconAnalysisUtils.hxx>
// #include <AnalysisTools/TCCreconAnalysisUtils.cxx>
// //#include <AnalysisTools/TCCweightMA.hxx>
// #include <AnalysisTools/TCCweightMA.cxx>
// //#include <AnalysisTools/TGeneAnalysisUtils.hxx>
// #include <AnalysisTools/TGeneAnalysisUtils.cxx>
// //#include <AnalysisTools/TCCtruthAnalysisUtils.hxx>
// #include <AnalysisTools/TCCtruthAnalysisUtils.cxx>
// #include <AnalysisTools/TNEUTAnalysisUtils.hxx>
// #include <AnalysisTools/TNEUTAnalysisUtils.cxx>
// #include <AnalysisTools/TGENIEAnalysisUtils.hxx>
// #include <AnalysisTools/TGENIEAnalysisUtils.cxx>



TChain mcTruthVertices = TChain("TruthDir/Vertices");
TChain mcTruthTrajectories = TChain("TruthDir/Trajectories");
TChain reconTracker = TChain("ReconDir/Tracker");
TChain reconGlobal = TChain("ReconDir/Global");

Int_t NVtxFGD1; 
Int_t NVtxFGD2;
TClonesArray *VtxFGD1;
TClonesArray *VtxFGD2;
const int NTRAJTYPES=7;
std::string names[NTRAJTYPES];
//std::string selcaseREA;
//std::string selcasePAR;
TClonesArray *TruthTrajs[NTRAJTYPES];
Int_t NVertices;
Int_t NDelayedClusters;
Int_t NTracks;
TClonesArray *Vertices;
TClonesArray *Tracks;
TClonesArray *DelayedClusters;

Int_t NPIDs;
TClonesArray *PIDs;


//*********************
//pid cuts
//*********************
const int curvcut=3;
double pullsigma[NPARTICLEHYP];
double pullmean[NPARTICLEHYP];
double probcut[NPARTICLESEL];
double pullcut[NPARTICLESEL];
double elcut[4];
//*********************
//reaction cuts
//*********************
double dthpcut;
double mnu2cut;
TVector3 fvcut;
double dDelCcut;

//*********************
//tree branch address
//*********************
int _NTRACKS;
int _NPARTICLESEL;
int _NPARTICLEHYP;
int _NPARTICLECUT;
int _NDELAYEDCLUS;
int _NCUTS;
int _NREACTION;
int _NSELREACTION;

int _evid;
float _weightMA;
int _isRecVtxFV;
int _hasMichel;
float _dThp;
float _dEp;
float _Mnu2;
int _ntracks;
int _ntpctracks;
int _nfgdtracks;
int _evSel[NSELREACTION][NCUTS];
float _RecVtx[3];
float _RecPmu;
float _RecCosThmu;
float _RecEnu[NSELREACTION];
float _RecQ2[NSELREACTION];
//true branch address
int _evTrue[NREACTION];
int _evG4Code;
float _TrueVtx[3];
float _TrueMomprim;
float _TrueCosThprim;
float _TrueEnu;
float _TrueQ2;
float _OUTleptonPDG;
float _OUTnuPDG;
int _OUTtrajPDG[NTRACKS];
float _OUTtrajMom[NTRACKS];
float _OUTtrajCosTh[NTRACKS];
//track branch
int _trid[NTRACKS];
int _trSel[NTRACKS][NPARTICLESEL][NPARTICLECUT];
int _trTrue[NTRACKS];
int _trG4Code[NTRACKS];
float _trQuality[NTRACKS];
float _RecMom[NTRACKS];
float _TrueMom[NTRACKS];
float _RecCosTh[NTRACKS];
float _TrueCosTh[NTRACKS];
int _RecCharge[NTRACKS];
int _TrueCharge[NTRACKS];
int _BackWards[NTRACKS];
int _isUsefullTrack[NTRACKS];
float _SigMom[NTRACKS];
float _MomErr[NTRACKS];
int _NNodes[NTRACKS];
float _dEdxmeas[NTRACKS];
float _Pull[NTRACKS][NPARTICLEHYP];
float _Sigma[NTRACKS][NPARTICLEHYP];
float _dEdxexp[NTRACKS][NPARTICLEHYP];
float _LikeLHD[NTRACKS][NPARTICLEHYP];
float _Prob[NTRACKS][NPARTICLEHYP];
float _trDir[NTRACKS][3];
float _delayedClustersPos[NDELAYEDCLUS][3];
float _delayedClustersVar[NDELAYEDCLUS][3];
int _ndelayedClusters;

//*********************
// plots
//*********************
int trueVtxFV;
int TotalInFV[2];
char paramName[7][50];
char paramState[2][5];
TCCplotAnalysisUtils *param[2][5];
TCCplotAnalysisUtils *param2D[2][10];//10=4+3+2+1
TCCplotAnalysisUtils *diffparam_True[NREACTION][5][4];
TCCplotAnalysisUtils *diffparam_SelTrue[NSELREACTION][NREACTION][5][4];
TCCplotAnalysisUtils *diffparam_Sel[NSELREACTION][5][4];
TCCplotAnalysisUtils *counter;
char pidParName[10][50];
char kinParName[11][50];
TCCplotAnalysisUtils *particleHypH1D[5];
TCCplotAnalysisUtils *particleHypH1DpBin[5][2][2];//hyp, {mom,q}, {rec,true}
TCCplotAnalysisUtils *particleHypH2D_RecVsRec[30];//6*5
TCCplotAnalysisUtils *particleHypH2D_RecVsTrue[10];//2*5
TCCplotAnalysisUtils *particleKinH1D_Rec[11];
TCCplotAnalysisUtils *particleKinH1D_True[3];
TCCplotAnalysisUtils *particleKinH2D_RecVsRec[55];//10+9+8+7+6+5+4+3+2+1
TCCplotAnalysisUtils *particleKinH2D_RecVsTrue[22];//2*11
TCCplotAnalysisUtils *particleKinH2D_TrueVsTrue[3];//2+1
TCCplotAnalysisUtils *pCounter[4]; //TOT (0,500MeV),(500,2500),>2500 MeV
//*********************
// functions
//*********************

std::string SetOutputName_cutAnalysis(string GENE_typefile,double MA,string selcasePAR,string selcaseREA,string ndVersion, int fgdstudy,int truestudy);
std::string SetOutputName_ccAnalysis(string GENE_typefile,double MA,string selcasePAR,string selcaseREA,string ndVersion, int fgdstudy,int truestudy);
TCCplotAnalysisUtils* RunPullAnalysis(int fgdstudy);

void ResetPidCuts();
void ResetReaCuts();
void SetPidCutsManually();
void SetReaCutsManually();

void RunTrackCutAnalysis_FillTree(std::string filename,int fgdstudy);
void RunReactionCutAnalysis_FillTree(TFile *file,string GENE_typefile,string selcasePAR, string selcaseREA,int fgdstudy);
void RunProbCutAnalysis(string selcasePAR,TFile *rootfile_output,int stagelevel);
void RunPullCutAnalysis(TFile *rootfile_output);
void RunElectronCutAnalysis(string selcasePAR,TFile *rootfile_output);
void SetPullStats(string GENE,string ndVersion,int fgdstudy);
void SetpidCuts(string GENE_typefile,double MA,string selcasePAR,string selcaseREA,string ndVersion,int fgdstudy);
void RunFVCutAnalysis(string selcasePAR,string selcaseREA,TFile *rootfile_output, int ifgd, int axis);
void RundThpCutAnalysis(string selcasePAR,string selcaseREA,TFile *rootfile_output);
void RunMichelElectronCutAnalysis(string selcasePAR,string selcaseREA,TFile *rootfile_output);
double GetFVCut(TCCplotAnalysisUtils *plot,const int _nstep,double stepwidth,int ifgd, int iselrea,int icut, int irea);
double GetdThpCut(TCCplotAnalysisUtils *plot, int iselrea,int icut, int irea);
double GetdEpCut(TCCplotAnalysisUtils *plot, int iselrea,int icut, int irea);
double GetMnu2Cut(TCCplotAnalysisUtils *plot, int iselrea,int icut, int irea);
double GetMichelElectronCut(TCCplotAnalysisUtils *plot, int iselrea,int icut, int irea);
                
void SetreaCuts(TFile *rootfile_output,string GENE_typefile,double MA,string selcasePAR,string selcaseREA,string ndVersion,int fgdstudy);
double GetProbCut(TCCplotAnalysisUtils *plot, int iselpart,int icut, int itruepart);
double GetPullCut(TCCplotAnalysisUtils *plot, int iselpart,int icut, int itruepart);
std::vector< double > GetElectronCut(TCCplotAnalysisUtils *plot, int iselpart,int icut, int itruepart);
void SetOAAnalysisBranches();
void InitTree(TTree *tree_Ev, int truestudy);
void FillTree(TTree *tree_Ev,string GENE_typefile,double MA,string selcasePAR,string selcaseREA,int fgdstudy, int truestudy);
void InitParticlePlots(TFile *rootfile_output,string selcasePAR);
void FillParticlePlots(TFile *rootfile_output,string selcasePAR,int truestudy);
void WriteParticlePlots(string selcasePAR);
void DeleteParticlePlots();
void InitReactionPlots(TFile *rootfile_output,string selcasePAR,string selcaseREA);
void FillReactionPlots(TFile *rootfile_output,string selcasePAR,string selcaseREA,int truestudy);
void WriteReactionPlots(string selcaseREA);
void DeleteReactionPlots();
//*********************
// // //-------------------------------------------------------------------------------------------------
// // // Melody Ravonel Salzgeber
// // // To run this code for example:


// // //       root 'ccAnalysis.C(<GENE_typefile>,<MA_value>,<selcasePAR>,<selcaseREA>,<ndVersion>,<manualcut>,<fgdstudy>,<truestudy>,<writehisto>)'
// // //       ex: root 'ccAnalysis.C("oldNEUT",1.1,"pullcut","default","nd280v6r3",1,0,1,1)'

// // //       selcasePAR = "onlyprobcut" (default), or  "nopullcut" or "pullcut" or "simple"(only if you choose selcaseREA="simple")
// // //       selcaseREA = "default", or  "simple" or "SKtopo"
// // //       ndVersion = "nd280vXrY"
// // //       MA_value = (axial mass, put the number that you want to have, the default value for Neut: 1.1 and for Genie: 0.99 )
// // //       fgdstudy = 0 (take into acount fgd1 and fgd2)
// // //       fgdstudy = 1 (take into acount fgd1)
// // //       fgdstudy = 2 (take into acount fgd2)
// // //       truestudy = 1 (if your want to read the true information branches), else 0
// // //       writehisto = 1 (if you want to compute all the histograms associated to the analysis), else 0 (faster!!!)


// // // If you don't want to run cut optimization, take care that you have the following files in the same directory:
// // //
// // //    *  pullstat_<GENE_typfile>_<ndVersion>.dat
// // //       ex:  pullstat_Genie_nd280v6r3.dat
// // //       format:
// // //       <pull sigma>   <pull mean>
// // //       -0.0258172	0.914581
// // //       -0.0551978	1.03033
// // //       0.0224594	1.01422
// // //       -0.0714552	1.38802
// // //       -0.0410938	0.720337
// // //    *  cutresults-pid_<GENE_typfile>-MA<MA_value>_<selcasePAR>-<selcaseREA>_<ndVersion>.dat
// // //       ex:  cutresults-pid_Genie-MA1.0_onlyprobcut-default_nd280v6r3.dat
// // //       format (just write in the file like the following... ):
// // //       pull_cut 4
// // //       pull_cut 4
// // //       pull_cut 4
// // //       pull_cut 4
// // //       pull_cut 4
// // //       pull_cut 4
// // //       el_cut 40
// // //       el_cut 1.5
// // //       el_cut 40
// // //       el_cut 1
// // //       prob_cut 0.075
// // //       prob_cut 0.475
// // //       prob_cut 0.7
// // //       prob_cut 0.075
// // //       prob_cut 0.225
// // //       prob_cut 0.25
// // //       prob_cut 0
// // //    *  cutresults-rea_<GENE_typfile>-MA<MA_value>_<selcasePAR>-<selcaseREA>_<ndVersion>.dat
// // //       ex:  cutresults-rea_Genie-MA1.0_onlyprobcut-default_nd280v6r3.dat
// // //       format (just write in the file like the following... ):
// // //       FV_cut 200
// // //       FV_cut 200
// // //       FV_cut 10
// // //       dThp_cut 0.5
// // //       dDelC_cut 800



// // //--------------------------------------------------------------------------------------------------
// void ccAnalysis(){
//     //inputfile type name: ccfgdtpc-<ndVersion>_inputfiles-<GENE_typefile>.txt
//     string GENE_typefile= "oldNEUT";//(should be the same than in the inputfile...this is an exple)
//     //   if genie files are used: => write "Genie"
//     //   if data files is used:   => write "Data"

//     double MA=1.1;
//     string selcasePAR= "nopullcut";//, "nopullcut", "onlyprobcut","simple" 
//     string selcaseREA= "default";//, "simple", "SKtopo"
//     string ndVersion= "nd280v6r3";// "head" (or what you want as soon as it is the same name than in the inputfile...)
//     int manualcut=1; (1 if manual cut wanted, else 0)
//     int fgdstudy=0;
//     int truestudy=1;
//     int writehisto=1;

void ccAnalysis(string GENE_typefile,double MA,string selcasePAR, string selcaseREA,string ndVersion,int manualcut,int fgdstudy,int truestudy, int writehisto){
  

//*************************************************************************
    //Load some important libraries.
    //*************************************************************************
    gROOT->SetBatch(1);
    //  gROOT->ProcessLine(".x oaAnalysisReadInitFile.C");
    gSystem->AddIncludePath("-I$OAANALYSISROOT/src");
    gSystem->AddIncludePath("-I.");
    gSystem->Load("libPhysics.so");
    gSystem->Load("libGeom.so");
    gSystem->Load("libTree.so");
    gSystem->Load("libMatrix.so");
    gSystem->Load("liboaEvent.so");
    gSystem->Load("librecpack.so");
    gSystem->Load("liboaRecPack.so");
    gSystem->Load("liboaAnalysis.so");

    gROOT->ProcessLine(".L AnalysisTools/TGENIEAnalysisUtils.cxx");
    gROOT->ProcessLine(".L AnalysisTools/TNEUTAnalysisUtils.cxx");
    gROOT->ProcessLine(".L AnalysisTools/TGeneAnalysisUtils.cxx");
    gROOT->ProcessLine(".L AnalysisTools/TCCtruthAnalysisUtils.cxx");
    gROOT->ProcessLine(".L AnalysisTools/TCCreconAnalysisUtils.cxx");
    gROOT->ProcessLine(".L AnalysisTools/TCCweightMA.cxx");
    gROOT->ProcessLine(".L AnalysisTools/TCCplotAnalysisUtils.cxx");
    gROOT->ProcessLine(".L AnalysisTools/TGlobalAnalysisUtils.cxx");
    //*************************************************************************
    //Read oaAanalysis input files
    //*************************************************************************
    char inputfileName[256];
    sprintf(inputfileName,"input-%s_%s.txt",ndVersion.c_str(),GENE_typefile.c_str());
 
    TString fileName(inputfileName);
    ifstream inputFile(fileName.Data(), ios::in);
    if (!inputFile) {
        cerr << "Cannot open input file '" << fileName.Data() << "'. Exit!" << endl;
        return;
    }
    std::string inputString;   
    while (inputFile >> inputString) {
        std::cout << "Adding file: " << inputString << std::endl;
        mcTruthVertices.AddFile(inputString.c_str());
        mcTruthTrajectories.AddFile(inputString.c_str());
        reconTracker.AddFile(inputString.c_str());
        reconGlobal.AddFile(inputString.c_str());
    }
    SetOAAnalysisBranches();
   
    //initcuts 
    //*************************************************************************
    ResetPidCuts();
    ResetReaCuts();
    //if simple flag the pulls/pid is not needed
    if(selcaseREA!="simple"){
        SetPullStats(GENE_typefile,ndVersion,fgdstudy);
    }

    if(manualcut==0){
    //TO DO CUT OPTIMISATION:
    //*****************************
    //*************************************************************************
    //GET PULL VALUES for each hypothesis
    //0  1  2   3  4 
    //mu p  pi  e  k
    //GET PROBABILITY CUT FOR PARTICLE SELECTION
    //1) get pull stat
    //2) save prob for these stats
    //3) get pull cut 5
    //4) get elec cut 6
    //5) get prob cut 7
    //6) get prob cut 8
    //7) get prob cut 9
    //*************************************************************************
    //if pull file .dat does not exist, create it and make the corresponding histograms
    //*************************************************************************
    if(truestudy && GENE_typefile!="Data"){
        std::string cutfilename=SetOutputName_cutAnalysis(GENE_typefile,MA,selcasePAR,selcaseREA,ndVersion,fgdstudy,truestudy);
        if(selcaseREA!="simple"){
            //if root file does not exist,
            //create it and make the corresponding histograms
            if(!TFile::Open(cutfilename.c_str()))
            {
                 RunTrackCutAnalysis_FillTree(cutfilename,fgdstudy);
            }
            SetpidCuts(GENE_typefile,MA,selcasePAR,selcaseREA,ndVersion,fgdstudy);
        }
        // the previous part has to set once at least to be able to run the rest of the code!!
        TFile *cutfile=new TFile(cutfilename.c_str(),"UPDATE");
        if(!cutfile->Get("tree_Rea"))
            RunReactionCutAnalysis_FillTree(cutfile,GENE_typefile,selcasePAR,selcaseREA,fgdstudy);

        SetreaCuts(cutfile,GENE_typefile,MA,selcasePAR,selcaseREA,ndVersion,fgdstudy);
        cutfile->Close();
        delete cutfile;
    }
    else{
        //you have to set manually the cuts you want to use,
        //since there is no way to define them at this level
        SetPidCutsManually();
        SetReaCutsManually();
    }
    }
    else{
    //TO APPLY CUTS MANUALLY:
    //*****************************
        SetPidCutsManually();
        SetReaCutsManually();
    }
   
    // DO THE ANALYSIS
    //*****************************
    // Init tree
    // Fill output tree
    // Write tree in the output file
    //*****************************
    if(manualcut==1)
        ndVersion=ndVersion+"-manualCut";
    std::string filename=SetOutputName_ccAnalysis(GENE_typefile,MA,selcasePAR,selcaseREA,ndVersion,fgdstudy,truestudy);
    if(!TFile::Open(filename.c_str()))
    {//if this file exist
        TFile *rootfile_output=new TFile(filename.c_str(),"RECREATE");
        TTree *tree_Ev=new TTree("tree_process","process selection and track identification");
        InitTree(tree_Ev,truestudy);
        FillTree(tree_Ev,GENE_typefile,MA,selcasePAR,selcaseREA,fgdstudy,truestudy);
        cout<<" write tree "<<endl;
        tree_Ev->Write();
        cout<<" tree writen "<<endl;
        delete tree_Ev;
        rootfile_output->Close();
        delete rootfile_output;
    }
    cout<<" start with histos "<<endl;
    //*****************************
    //1) first make it for particle and then for reactions
    //initialize plots and counters for particle
    //fill plots
    //write plots + delete all pointer
    //2) for reaction (idem)
    //*****************************
    if(writehisto){
        TFile *rootfile_output2=new TFile(filename.c_str(),"UPDATE");
        InitParticlePlots(rootfile_output2,selcasePAR);
        FillParticlePlots(rootfile_output2,selcasePAR,truestudy);
        WriteParticlePlots(selcasePAR);
        DeleteParticlePlots();
        cout<<" plots have been deleted "<<endl;
        InitReactionPlots(rootfile_output2,selcasePAR,selcaseREA);
        FillReactionPlots(rootfile_output2,selcasePAR,selcaseREA,truestudy);
        WriteReactionPlots(selcaseREA);
        DeleteReactionPlots();
        rootfile_output2->Close();
        delete rootfile_output2;
    }   
  
}
  

void SetOAAnalysisBranches(){
    // _____________________________________________________________________________________
    // Prepare to read trees.
    // Set the branches for the truth information.
    VtxFGD1 = new TClonesArray("ND::TTruthVerticesModule::TTruthVertex", 20);
    VtxFGD2 = new TClonesArray("ND::TTruthVerticesModule::TTruthVertex", 20);
    // trajectories information
    names[0]="TrajsLepton";
    names[1]="TrajsBaryon";
    names[2]="TrajsMeson";
    names[3]="TrajsPhoton";
    names[4]="TrajsOtherCharged";
    names[5]="TrajsOtherNeutral";
    names[6]="TrajsOther";
    
    for (int ii=0;ii<NTRAJTYPES;ii++){
        TruthTrajs[ii] = new TClonesArray("ND::TTruthTrajectoriesModule::TTruthTrajectory", 30);
        mcTruthTrajectories.SetBranchAddress(names[ii].c_str(),&(TruthTrajs[ii]));
    }
    mcTruthTrajectories.Print("toponly");
    cout<<" set branches adresses "<<endl;
    mcTruthVertices.SetBranchAddress("NVtxFGD1", &NVtxFGD1);
    mcTruthVertices.SetBranchAddress("NVtxFGD2", &NVtxFGD2);
    mcTruthVertices.SetBranchAddress("VtxFGD1", &VtxFGD1);
    mcTruthVertices.SetBranchAddress("VtxFGD2", &VtxFGD2);   
    mcTruthVertices.Print("toponly");
         
  
    PIDs = new TClonesArray("ND::TGlobalReconModule::TGlobalPID", 20);
    reconGlobal.SetBranchAddress("NPIDs", &NPIDs);
    reconGlobal.SetBranchAddress("PIDs", &PIDs);
    GlobalVertices = new TClonesArray("ND::TGlobalReconModule::TGlobalVertex", 5);
    reconGlobal.SetBranchAddress("NVertices", &NVertices);
    reconGlobal.SetBranchAddress("Vertices", &Vertices);
    DelayedClusters = new TClonesArray("ND::TGlobalReconModule::TFgdCluster", 10);
    reconGlobal.SetBranchAddress("NDelayedClusters", &NDelayedClusters);
    reconGlobal.SetBranchAddress("DelayedClusters", &DelayedClusters);

    reconGlobal.Print("toponly");

}

std::string SetOutputName_cutAnalysis(string GENE_typefile,double MA,string selcasePAR,string selcaseREA,string ndVersion, int fgdstudy,int truestudy){
    char fname[256];

    if(truestudy==1){
        if(fgdstudy==0)
            sprintf(fname,"cutAnalysis_%s-MA%.1f_%s-%s_%s-60000.root",GENE_typefile.c_str(),MA,selcasePAR.c_str(),selcaseREA.c_str(),ndVersion.c_str());
        else if(fgdstudy==1)
            sprintf(fname,"cutAnalysisFGD1_%s-MA%.1f_%s-%s_%s-60000.root",GENE_typefile.c_str(),MA,selcasePAR.c_str(),selcaseREA.c_str(),ndVersion.c_str());
        else if(fgdstudy==2)
            sprintf(fname,"cutAnalysisFGD2_%s-MA%.1f_%s-%s_%s-60000.root",GENE_typefile.c_str(),MA,selcasePAR.c_str(),selcaseREA.c_str(),ndVersion.c_str());
    }
    else if(truestudy==0){
        if(fgdstudy==0)
            sprintf(fname,"cutAnalysis_%s-MA%.1f_%s-%s_%s-60000_onlyreco.root",GENE_typefile.c_str(),MA,selcasePAR.c_str(),selcaseREA.c_str(),ndVersion.c_str());
        else if(fgdstudy==1)
            sprintf(fname,"cutAnalysisFGD1_%s-MA%.1f_%s-%s_%s-60000_onlyreco.root",GENE_typefile.c_str(),MA,selcasePAR.c_str(),selcaseREA.c_str(),ndVersion.c_str());
        else if(fgdstudy==2)
            sprintf(fname,"cutAnalysisFGD2_%s-MA%.1f_%s-%s_%s-60000_onlyreco.root",GENE_typefile.c_str(),MA,selcasePAR.c_str(),selcaseREA.c_str(),ndVersion.c_str());
    }
    string filename(fname);
    return filename;
}

std::string SetOutputName_ccAnalysis(string GENE_typefile,double MA,string selcasePAR,string selcaseREA,string ndVersion, int fgdstudy,int truestudy){
  
    char fname[256];
    if(truestudy==1){
        if(fgdstudy==0)
            sprintf(fname,"ccAnalysis_%s-MA%.1f_%s-%s_%s-60000.root",GENE_typefile.c_str(),MA,selcasePAR.c_str(),selcaseREA.c_str(),ndVersion.c_str());
        else if(fgdstudy==1)
            sprintf(fname,"ccAnalysisFGD1_%s-MA%.1f_%s-%s_%s-60000.root",GENE_typefile.c_str(),MA,selcasePAR.c_str(),selcaseREA.c_str(),ndVersion.c_str());
        else if(fgdstudy==2)
            sprintf(fname,"ccAnalysisFGD2_%s-MA%.1f_%s-%s_%s-60000.root",GENE_typefile.c_str(),MA,selcasePAR.c_str(),selcaseREA.c_str(),ndVersion.c_str());
    }
    else if(truestudy==0){
        if(fgdstudy==0)
            sprintf(fname,"ccAnalysis_%s-MA%.1f_%s-%s_%s-60000_onlyreco.root",GENE_typefile.c_str(),MA,selcasePAR.c_str(),selcaseREA.c_str(),ndVersion.c_str());
        else if(fgdstudy==1)
            sprintf(fname,"ccAnalysisFGD1_%s-MA%.1f_%s-%s_%s-60000_onlyreco.root",GENE_typefile.c_str(),MA,selcasePAR.c_str(),selcaseREA.c_str(),ndVersion.c_str());
        else if(fgdstudy==2)
            sprintf(fname,"ccAnalysisFGD2_%s-MA%.1f_%s-%s_%s-60000_onlyreco.root",GENE_typefile.c_str(),MA,selcasePAR.c_str(),selcaseREA.c_str(),ndVersion.c_str());
    }
    
    string filename(fname);
    return filename;
}
 


 
void RunTrackCutAnalysis_FillTree(std::string name, int fgdstudy){
    //********************************************************************
    //RUN PROB CUT
    //********************************************************************
    string filename=name;
    TFile *rootfile_output=new TFile(filename.c_str(),"RECREATE");
    TCCreconAnalysisUtils *recAn=new TCCreconAnalysisUtils();
    TTree *tree_Tr=new TTree("tree_tracks","tracks ");
    recAn->InitTree(tree_Tr);
    tree_Tr->Branch("ntracks",&_ntracks,"ntracks/I");
    recAn->SetPullStat(pullmean, pullsigma);
   

    TStopwatch timer;
    timer.Start();
    int entries = mcTruthVertices.GetEntries(); 
    std::cout << "ent: " << entries << std::endl;
    //entries=600;
    for(int ient = 0; ient < entries; ient++)
    {
        mcTruthVertices.GetEntry(ient);
        reconTracker.GetEntry(ient);
        reconGlobal.GetEntry(ient);
        if(ient%1000 == 0)
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
            isfgd1=true;
        }
        else if (NVtxFGD2 >0){
            true_fgd_vertex = (ND::TTruthVerticesModule::TTruthVertex*) VtxFGD2->UncheckedAt(0);
            isfgd2=true;
        }
        else{
            std::cerr << "There should be at least one vertex in one FGD.  Skipping event."<<std::endl;
            continue;
        }
        // for only fgd1 study
        if(fgdstudy==1){
            if(!isfgd1)
                continue;
        }
        else if(fgdstudy==2){        // for only fgd2 study
            if(!isfgd2)
                continue;
        }
                
        recAn->ResetCutFlag();
        int kTotalTPC=0;
        _ntracks=NPIDs;

        recAn->SetNTracks(NPIDs);
        recAn->FillRecVectors(PIDs);
        recAn->FillTrueVectors(PIDs);
        recAn->SetTotalTracksInTPC(PIDs);
        kTotalTPC=recAn->GetTotalTracksInTPC();
        if(kTotalTPC>0){
            recAn->SetTrackProb();
            recAn->FillTree_trueBR();
            recAn->FillTree_recBR();
        }//end
        tree_Tr->Fill();  
    }//end ientries
    tree_Tr->Write();
    delete recAn;
    delete tree_Tr;
    rootfile_output->Close();
    delete rootfile_output;
}

void RunPullCutAnalysis(TFile *rootfile_output){
    TCCreconAnalysisUtils *recAn=new TCCreconAnalysisUtils();
    recAn->SetPullStat(pullmean, pullsigma);
    recAn->SetCurvCut(curvcut);

    
    TCCplotAnalysisUtils *Pull[NPARTICLESEL];
    for(int isel=0;isel<NPARTICLESEL;isel++){
        Pull[isel]=new TCCplotAnalysisUtils(rootfile_output,NPARTICLECUT,0);
        Pull[isel]->InitParParticleTOTH1D("Rec","Pull");
        Pull[isel]->InitEffPurvsParParticleTOTH1D("Rec","Pull");
        Pull[isel]->InitCutOptParticleCounter();
        Pull[isel]->InitEffPurFOMvsParticleCutOpt("Pull");
    }

    TTree *tree_Tr=(TTree*) rootfile_output->Get("tree_tracks");
    tree_Tr->SetBranchAddress("_NTRACKS",&_NTRACKS);

    tree_Tr->SetBranchAddress("isUsefullTrack",_isUsefullTrack);
    tree_Tr->SetBranchAddress("ntracks",&_ntracks);
    tree_Tr->SetBranchAddress("Prob",_Prob);
    tree_Tr->SetBranchAddress("Pull",_Pull);
    tree_Tr->SetBranchAddress("dEdxmeas",_dEdxmeas);
    tree_Tr->SetBranchAddress("MomErr",_MomErr);
    tree_Tr->SetBranchAddress("BackWards",_BackWards);
    tree_Tr->SetBranchAddress("RecCharge",_RecCharge);
    tree_Tr->SetBranchAddress("trTrue",_trTrue);
    int entries=tree_Tr->GetEntries();
    cout<<" nb of entries "<<entries<<endl;
    int icut=4;
    TStopwatch timer;
    timer.Start();
    //entries=600;
    for(int ient=0;ient<entries;ient++){
        if(ient%5000 == 0)
        {
            std::cout <<  "********************************" << std::endl;
            std::cout <<  "    !!!Events Processed " << ient << std::endl;
            timer.Stop();
            std::cout << " Processing time: " << timer.CpuTime() << " " << timer.RealTime() << std::endl;
            std::cout <<  "********************************" << std::endl;
            timer.Start();
        }
        tree_Tr->GetEntry(ient);
        int tottracks=0;
        if(_ntracks<NTRACKS)tottracks=_ntracks;
        else  tottracks=NTRACKS;
        recAn->FillVectors(tottracks);
        for(int itr=0;itr<tottracks;itr++){
            recAn->SetisUsefullTrack(itr,_isUsefullTrack[itr]);
            if(_isUsefullTrack[itr]){
                for(int ihyp=0;ihyp<NPARTICLEHYP;ihyp++){
                    recAn->SetTrackPull(itr,ihyp,_Pull[itr][ihyp]);
                    recAn->SetTrackProb(itr,ihyp,_Prob[itr][ihyp]);
                }
                recAn->SetTrackRelErr(itr,_MomErr[itr]);
                recAn->SetTrackRecCharge(itr,_RecCharge[itr]);
                recAn->SetTrackIsBackWards(itr,_BackWards[itr]);

                for(int isel=0;isel<6;isel++){
                    int ihyp=-9;
                    if(isel<3)ihyp=isel;
                    if(isel==3)ihyp=2;
                    if(isel==4 || isel==5)ihyp=3;
                    if(recAn->IsSelParticle(itr,isel,icut)){
                        Pull[isel]->FillSelParParticleTOTH1D(isel,icut,_Pull[itr][ihyp]);
                        if(_trTrue[itr]!=-9 ){
                            Pull[isel]->FillSelTrueParParticleTOTH1D(isel,icut,_trTrue[itr],_Pull[itr][ihyp]);
                        }
                    }
                    if(_trTrue[itr]!=-9){
                        Pull[isel]->FillTrueParParticleTOTH1D(_trTrue[itr],_Pull[itr][ihyp]);
                    }
                }
            }
        }
        recAn->ClearVectors();

        
    }
    for(int isel=0;isel<6;isel++){
        double _pullcut=GetPullCut(Pull[isel],isel,icut,isel);
        pullcut[isel]=_pullcut;
        cout<<" isel pull cut "<<isel<<" : "<<_pullcut<<endl;
      
    }

    delete tree_Tr;
    
    for(int isel=0;isel<6;isel++){
        Pull[isel]->DeleteParParticleTOTH1D();
        delete Pull[isel];
    }
}
double GetPullCut(TCCplotAnalysisUtils *plot, int iselpart,int icut, int itruepart){

    int nbin=plot->nbinX;
    int max=plot->maxX;
    int min=plot->minX;
    double binwidth=(double) (max-min)/nbin;
    double stepwidth=0.25;
    const int nstep=15;
    plot->SetNStep(nstep);
    double fom[nstep];
    double step0=2.5;
    int ihyp=iselpart;
    if(iselpart<3) ihyp=iselpart;
    if(iselpart==3) ihyp=2;
    if(iselpart==4 || iselpart==5) ihyp=3;


    for(int istep=0;istep<nstep;istep++){
        fom[istep]=0;
        double value_sel=0;
        double value_seltrue=0;
        double value_true=0;
        for(int ibin=0;ibin<nbin;ibin++){
            double ibin_val=binwidth*ibin+min;
            double istep_val=stepwidth*istep+step0;
            if(fabs(ibin_val)<pullmean[ihyp]+istep_val*pullsigma[ihyp]){
                value_sel+=plot->ParParticleTOT_Sel[iselpart][icut]->GetBinContent(ibin);
                value_seltrue+=plot->ParParticleTOT_SelTrue[iselpart][icut][itruepart]->GetBinContent(ibin);
            }
            value_true+=plot->ParParticleTOT_SelTrue[iselpart][icut][itruepart]->GetBinContent(ibin);
        }
        double _fom=(double)plot->GetStatSignificance(value_seltrue,value_sel);
        fom[istep]=_fom; 
        cout<<" pull fom "<<_fom<<endl;
        plot->FillSelCutOptParticleCounter(istep,iselpart,icut,value_sel);
        plot->FillSelTrueCutOptParticleCounter(istep,iselpart,icut,itruepart,value_seltrue);
        plot->FillTrueCutOptParticleCounter(istep,itruepart,value_true);
       
    }

     double maxfom=-1;
     double maxstep=-1;
     for(int istep=0;istep<nstep;istep++){
         if(fom[istep]>=maxfom){
             maxfom=fom[istep];
             maxstep=istep;
         }
     }

    plot->FillEffPurFOMvsParticleCutOpt(step0,stepwidth);
    plot->PlotEffPurFOMvsParticleCutOpt("Pull",iselpart, icut,itruepart);
    double pullpos=stepwidth*maxstep+step0; 
    cout<<" stepwidth "<<stepwidth<<" maxstep_val "<<maxstep*stepwidth+step0<<endl;
    cout<<" cut  "<<pullmean[ihyp]+pullpos*pullsigma[ihyp]<<endl;

    
    return pullpos;
}
void RunElectronCutAnalysis(string selcasePAR,TFile *rootfile_output){
    TCCreconAnalysisUtils *recAn=new TCCreconAnalysisUtils();
    recAn->SetPullStat(pullmean, pullsigma);
    recAn->SetCurvCut(curvcut);
    int nparticlecuts=recAn->GetNParticleCuts(selcasePAR);
    
    TCCplotAnalysisUtils *SumPullvsRecMom_4;
    SumPullvsRecMom_4=new TCCplotAnalysisUtils(rootfile_output,nparticlecuts,0);
    SumPullvsRecMom_4->SetRange(100,0,2000,"X");
    SumPullvsRecMom_4->SetRange(160,0,40,"Y");
    SumPullvsRecMom_4->InitParParticleH2D("Rec","Mom","Rec","Pull");
    SumPullvsRecMom_4->InitEffPurvsParParticleH2D("Rec","Mom","Rec","Pull");
    SumPullvsRecMom_4->InitCutOptParticleCounter();
    SumPullvsRecMom_4->InitEffPurFOMvsParticleCutOpt(" ");

    TCCplotAnalysisUtils *SumPullvsRecMom_5;
    SumPullvsRecMom_5=new TCCplotAnalysisUtils(rootfile_output,nparticlecuts,0);
    SumPullvsRecMom_5->SetRange(100,0,2000,"X");
    SumPullvsRecMom_5->SetRange(160,0,40,"Y");
    SumPullvsRecMom_5->InitParParticleH2D("Rec","Mom","Rec","Pull");
    SumPullvsRecMom_5->InitEffPurvsParParticleH2D("Rec","Mom","Rec","Pull");
    SumPullvsRecMom_5->InitCutOptParticleCounter();
    SumPullvsRecMom_5->InitEffPurFOMvsParticleCutOpt(" ");

    
    TTree *tree_Tr=(TTree*) rootfile_output->Get("tree_tracks");

    tree_Tr->SetBranchAddress("_NTRACKS",&_NTRACKS);
  
    tree_Tr->SetBranchAddress("isUsefullTrack",_isUsefullTrack);
    tree_Tr->SetBranchAddress("ntracks",&_ntracks);
    tree_Tr->SetBranchAddress("Prob",_Prob);
    tree_Tr->SetBranchAddress("Pull",_Pull);
    tree_Tr->SetBranchAddress("dEdxmeas",_dEdxmeas);
    tree_Tr->SetBranchAddress("MomErr",_MomErr);
    tree_Tr->SetBranchAddress("BackWards",_BackWards);
    tree_Tr->SetBranchAddress("RecCharge",_RecCharge);
    tree_Tr->SetBranchAddress("trTrue",_trTrue);
    tree_Tr->SetBranchAddress("trG4Code",_trG4Code);
    tree_Tr->SetBranchAddress("TrueMom",_TrueMom);
    tree_Tr->SetBranchAddress("RecMom",_RecMom);
    int entries=tree_Tr->GetEntries();
    cout<<" nb of entries "<<entries<<endl;
    int icut=nparticlecuts-5;
    recAn->SetPullCut(pullcut[0]);//4);
    TStopwatch timer;
    timer.Start();
    //entries=600;

    for(int ient=0;ient<entries;ient++){
        if(ient%500 == 0)
        {
            std::cout <<  "********************************" << std::endl;
            std::cout <<  "    !!!Events Processed " << ient << std::endl;
            timer.Stop();
            std::cout << " Processing time: " << timer.CpuTime() << " " << timer.RealTime() << std::endl;
            std::cout <<  "********************************" << std::endl;
            timer.Start();
        }
        tree_Tr->GetEntry(ient);
        int tottracks=0;
        if(_ntracks<NTRACKS)tottracks=_ntracks;
        else  tottracks=NTRACKS;
        recAn->FillVectors(tottracks);

        for(int itr=0;itr<tottracks;itr++){
            recAn->SetisUsefullTrack(itr,_isUsefullTrack[itr]);
            if(_isUsefullTrack[itr]){
                for(int ihyp=0;ihyp<NPARTICLEHYP;ihyp++){
                    recAn->SetTrackPull(itr,ihyp,_Pull[itr][ihyp]);
                    recAn->SetTrackProb(itr,ihyp,_Prob[itr][ihyp]);
                }
                recAn->SetTrackRelErr(itr,_MomErr[itr]);
                recAn->SetTrackRecCharge(itr,_RecCharge[itr]);
                recAn->SetTrackIsBackWards(itr,_BackWards[itr]);
                recAn->SetTrackPDG(itr,_trG4Code[itr]);
                double sumpull=fabs(_Pull[itr][0])+fabs(_Pull[itr][1])+fabs(_Pull[itr][2])+fabs(_Pull[itr][3]);
                
                if(recAn->IsSelParticle(itr,4,icut,selcasePAR)){
                    SumPullvsRecMom_4->FillSelParParticleH2D(4,icut,_RecMom[itr],sumpull);
                    if(_trTrue[itr]!=-9 ){
                        SumPullvsRecMom_4->FillSelTrueParParticleH2D(4,icut,_trTrue[itr],_RecMom[itr],sumpull);
                    }
                }
                if(recAn->IsSelParticle(itr,5,icut,selcasePAR)){
                    SumPullvsRecMom_5->FillSelParParticleH2D(5,icut,_RecMom[itr],sumpull);
                    if(_trTrue[itr]!=-9 ){
                        SumPullvsRecMom_5->FillSelTrueParParticleH2D(5,icut,_trTrue[itr],_RecMom[itr],sumpull);
                    }
                }
                
                if(_trTrue[itr]!=-9 )SumPullvsRecMom_4->FillTrueParParticleH2D(_trTrue[itr],_RecMom[itr],sumpull);
                if(_trTrue[itr]!=-9 )SumPullvsRecMom_5->FillTrueParParticleH2D(_trTrue[itr],_RecMom[itr],sumpull);
            }
        }
        recAn->ClearVectors();
    }


    SumPullvsRecMom_4->PlotParParticleH2D("Rec","Mom","Rec","Pull",4,icut,4);
    SumPullvsRecMom_5->PlotParParticleH2D("Rec","Mom","Rec","Pull",5,icut,5);
    std::vector< double > _elcut_4=GetElectronCut(SumPullvsRecMom_4,4,icut,4);
    std::vector< double > _elcut_5=GetElectronCut(SumPullvsRecMom_5,5,icut,5);
    elcut[0]=_elcut_4[0];
    elcut[1]=_elcut_4[1];
    elcut[2]=_elcut_5[0];
    elcut[3]=_elcut_5[1];
    
    //cout<<" Electron + Positron Cut at cut level "<<icut<<" e-: "<<electroncut<<" e+: "<<positroncut<<endl;
            
    delete tree_Tr;
    SumPullvsRecMom_4->DeleteParParticleH2D();
    SumPullvsRecMom_5->DeleteParParticleH2D();
    delete SumPullvsRecMom_4;
    delete SumPullvsRecMom_5;
      
}


std::vector< double > GetElectronCut(TCCplotAnalysisUtils *plot, int iselpart,int icut, int itruepart){
    //Add function to have other bon in tccplot mom: (0,500), pull...
    // end this cut
    int nbinX=plot->nbinX;
    int maxX=plot->maxX;
    int minX=plot->minX;
    int nbinY=plot->nbinY;
    int maxY=plot->maxY;
    int minY=plot->minY;

    double binwidthX=(double) (maxX-minX)/(double)nbinX;
    double binwidthY=(double) (maxY-minY)/(double)nbinY;
    const int nstepX=10;
    const int nstepY=20;
    plot->SetNStep(nstepX*nstepY);
    double pur[nstepX][nstepY];
    double step0X=binwidthX;
    double step0Y=binwidthY;
    
    
    for(int istep=0;istep<nstepX;istep++){
        double istep_val=(double)binwidthX*istep+step0X;
        for(int istep2=0;istep2<nstepY;istep2++){
            double istep2_val=(double)binwidthY*istep2+step0Y;
            pur[istep][istep2]=0;
            double value_sel=0;
            double value_seltrue=0;
            double value_true=0;
            for(int ibinX=1;ibinX<=nbinX;ibinX++){
                double ibinX_val=binwidthX*ibinX+minX;
                for(int ibinY=1;ibinY<=nbinY;ibinY++){
                    double ibinY_val=binwidthY*ibinY+minY;
                    if( fabs(ibinX_val)<istep_val && ibinY_val<istep2_val){
                        value_sel+=(double)plot->Par2vsPar1Particle_Sel[iselpart][icut]->GetBinContent(ibinX,ibinY);
                        value_seltrue+=(double)plot->Par2vsPar1Particle_SelTrue[iselpart][icut][itruepart]->GetBinContent(ibinX,ibinY);              
                    }
                    value_true+=(double)plot->Par2vsPar1Particle_SelTrue[iselpart][icut][itruepart]->GetBinContent(ibinX,ibinY);
                }
            }
            double _fom=(double)plot->GetStatSignificance(value_seltrue,value_sel);
            double _pur=(double)plot->GetPurity(value_seltrue,value_sel);
            double _eff=(double)plot->GetEfficiency(value_seltrue,value_true);

            pur[istep][istep2]=_pur; 
            int step=(istep)*nstepY+(istep2);
            plot->FillSelCutOptParticleCounter(step,iselpart,icut,value_sel);
            plot->FillSelTrueCutOptParticleCounter(step,iselpart,icut,itruepart,value_seltrue);
            plot->FillTrueCutOptParticleCounter(step,itruepart,value_true);
        }
    }
    

     double maxpur=-1;
     double maxstepX=-1;
     double maxstepY=-1;
     for(int istep=0;istep<nstepX;istep++){
         for(int istep2=0;istep2<nstepY;istep2++){
             if(pur[istep][istep2]>maxpur){
                 maxpur=pur[istep][istep2];
                 maxstepX=istep;
                 maxstepY=istep2;
             }
         }
     }
          
    plot->FillEffPurFOMvsParticleCutOpt(0,1);
    plot->PlotEffPurFOMvsParticleCutOpt("elcut",iselpart, icut,itruepart);
    double posX=binwidthX*maxstepX+step0X;
    double posY=binwidthY*maxstepY+step0Y; 
    cout<<"  pos cut X "<<posX<<endl;
    cout<<"  pos cut Y "<<posY<<endl;
    std::vector< double > electron_cut;
    electron_cut.push_back(posX);
    electron_cut.push_back(posY);
    return electron_cut;
}


void RunProbCutAnalysis(string selcasePAR,TFile *rootfile_output,int stagelevel){
    TCCreconAnalysisUtils *recAn=new TCCreconAnalysisUtils();
    recAn->SetPullStat(pullmean, pullsigma);
    recAn->SetCurvCut(curvcut);
    recAn->SetProbCut(probcut);
    int nparticlecuts=recAn->GetNParticleCuts(selcasePAR);
    TCCplotAnalysisUtils *Prob_mupiminus;
    TCCplotAnalysisUtils *Prob[NPARTICLESEL];
    for(int isel=0;isel<NPARTICLESEL;isel++){
        Prob[isel]=new TCCplotAnalysisUtils(rootfile_output,nparticlecuts,0);
        Prob[isel]->InitParParticleTOTH1D("Rec","Prob");
        Prob[isel]->InitEffPurvsParParticleTOTH1D("Rec","Prob");
        Prob[isel]->InitCutOptParticleCounter();
        Prob[isel]->InitEffPurFOMvsParticleCutOpt("Prob");
    }
    if(stagelevel==0){
        Prob_mupiminus=new TCCplotAnalysisUtils(rootfile_output,nparticlecuts,0);
        Prob_mupiminus->InitParParticleTOTH1D("Rec","Prob");
        Prob_mupiminus->InitEffPurvsParParticleTOTH1D("Rec","Prob");
        Prob_mupiminus->InitCutOptParticleCounter();
        Prob_mupiminus->InitEffPurFOMvsParticleCutOpt("Prob");
     }
    
    TTree *tree_Tr=(TTree*) rootfile_output->Get("tree_tracks");
    tree_Tr->SetBranchAddress("_NTRACKS",&_NTRACKS);

    tree_Tr->SetBranchAddress("isUsefullTrack",_isUsefullTrack);
    tree_Tr->SetBranchAddress("ntracks",&_ntracks);
    tree_Tr->SetBranchAddress("Prob",_Prob);
    tree_Tr->SetBranchAddress("Pull",_Pull);
    tree_Tr->SetBranchAddress("dEdxmeas",_dEdxmeas);
    tree_Tr->SetBranchAddress("MomErr",_MomErr);
    tree_Tr->SetBranchAddress("BackWards",_BackWards);
    tree_Tr->SetBranchAddress("RecCharge",_RecCharge);
    tree_Tr->SetBranchAddress("trTrue",_trTrue);
    tree_Tr->SetBranchAddress("trG4Code",_trG4Code);
    tree_Tr->SetBranchAddress("TrueMom",_TrueMom);
    tree_Tr->SetBranchAddress("RecMom",_RecMom);
    int entries=tree_Tr->GetEntries();
    cout<<" nb of entries "<<entries<<endl;
    int icut=nparticlecuts-4;
    recAn->SetPullCut(pullcut[0]);//4);
    TStopwatch timer;
    timer.Start();
    //entries=600;
    for(int ient=0;ient<entries;ient++){
        if(ient%5000 == 0)
        {
            std::cout <<  "********************************" << std::endl;
            std::cout <<  "    !!!Events Processed " << ient << std::endl;
            timer.Stop();
            std::cout << " Processing time: " << timer.CpuTime() << " " << timer.RealTime() << std::endl;
            std::cout <<  "********************************" << std::endl;
            timer.Start();
        }
        tree_Tr->GetEntry(ient);
        int tottracks=0;
        if(_ntracks<NTRACKS)tottracks=_ntracks;
        else  tottracks=NTRACKS;
        recAn->FillVectors(tottracks);
        
        for(int itr=0;itr<tottracks;itr++){
            recAn->SetisUsefullTrack(itr,_isUsefullTrack[itr]);
            if(_isUsefullTrack[itr]){
                for(int ihyp=0;ihyp<NPARTICLEHYP;ihyp++){
                    recAn->SetTrackPull(itr,ihyp,_Pull[itr][ihyp]);
                    recAn->SetTrackProb(itr,ihyp,_Prob[itr][ihyp]);
                }
             
                recAn->SetTrackRelErr(itr,_MomErr[itr]);
                recAn->SetTrackRecCharge(itr,_RecCharge[itr]);
                recAn->SetTrackIsBackWards(itr,_BackWards[itr]);
                recAn->SetTrackPDG(itr,_trG4Code[itr]);

                if(stagelevel==0){
                    if(recAn->IsSelParticle(itr,0,icut,selcasePAR)){
                        Prob_mupiminus->FillSelParParticleTOTH1D(0,icut,_Prob[itr][0]);
                        if(_trTrue[itr]==0 || _trTrue[itr]==3)  Prob_mupiminus->FillSelTrueParParticleTOTH1D(0,icut,0,_Prob[itr][0]);
                        if(_trTrue[itr]==1 || _trTrue[itr]==2 || _trTrue[itr]==4 || _trTrue[itr]==5 ){
                            Prob_mupiminus->FillSelTrueParParticleTOTH1D(0,icut,_trTrue[itr],_Prob[itr][0]);
                        }
                    }
                    if(recAn->IsSelParticle(itr,1,icut,selcasePAR)){
                        Prob[1]->FillSelParParticleTOTH1D(1,icut,_Prob[itr][1]);
                        if(_trTrue[itr]!=-9){
                            Prob[1]->FillSelTrueParParticleTOTH1D(1,icut,_trTrue[itr],_Prob[itr][1]);
                        }
                    }

                    if(_trTrue[itr]!=-9) Prob_mupiminus->FillTrueParParticleTOTH1D(_trTrue[itr],_Prob[itr][0]);
                    if(_trTrue[itr]!=-9) Prob[1]->FillTrueParParticleTOTH1D(_trTrue[itr],_Prob[itr][1]);
                }
                if(stagelevel==1){
                    //e-
                    if(recAn->IsSelParticle(itr,4,icut+stagelevel,selcasePAR)){
                        Prob[4]->FillSelParParticleTOTH1D(4,icut+stagelevel,_Prob[itr][3]);
                        if(_trTrue[itr]!=-9){
                            Prob[4]->FillSelTrueParParticleTOTH1D(4,icut+stagelevel,_trTrue[itr],_Prob[itr][3]);
                        }
                        
                    }
                    //pi+
                    if(recAn->IsSelParticle(itr,2,icut+stagelevel,selcasePAR)){
                        Prob[2]->FillSelParParticleTOTH1D(2,icut+stagelevel,_Prob[itr][2]);
                        if(_trTrue[itr]!=-9){
                            Prob[2]->FillSelTrueParParticleTOTH1D(2,icut+stagelevel,_trTrue[itr],_Prob[itr][2]);
                        }
                    }
                    if(_trTrue[itr]!=-9) Prob[4]->FillTrueParParticleTOTH1D(_trTrue[itr],_Prob[itr][3]);
                    if(_trTrue[itr]!=-9) Prob[2]->FillTrueParParticleTOTH1D(_trTrue[itr],_Prob[itr][2]);
                }
                if(stagelevel==2){
                    if(recAn->IsSelParticle(itr,5,icut+stagelevel,selcasePAR)){
                        Prob[5]->FillSelParParticleTOTH1D(5,icut+stagelevel,_Prob[itr][3]);
                        if(_trTrue[itr]!=-9){
                            Prob[5]->FillSelTrueParParticleTOTH1D(5,icut+stagelevel,_trTrue[itr],_Prob[itr][3]);
                        }
                    }
                    if(_trTrue[itr]!=-9) Prob[5]->FillTrueParParticleTOTH1D(_trTrue[itr],_Prob[itr][3]);
                }
            }
        }
        recAn->ClearVectors();
    }

    if(stagelevel==0){
        Prob_mupiminus->PlotParParticleTOTH1D("Rec","Prob",0,icut,0);
        Prob[1]->PlotParParticleTOTH1D("Rec","Prob",1,icut,1);
        
        double probcut_mupiminus=GetProbCut(Prob_mupiminus,0,icut,0);
        double probcut_p=GetProbCut(Prob[1],1,icut,1);

        cout<<" Prob Cut for mu - piminus at cut level "<<icut<<" : "<<probcut_mupiminus<<endl;
        cout<<" Prob Cut for p at cut level "<<icut<<"            : "<<probcut_p<<endl;

        probcut[0]=probcut_mupiminus;
        probcut[3]=probcut_mupiminus;
        probcut[1]=probcut_p;
        Prob[1]->DeleteParParticleTOTH1D();
        Prob[1]->DeleteEffPurvsParParticleTOTH1D();
        Prob_mupiminus->DeleteParParticleTOTH1D();
        Prob_mupiminus->DeleteEffPurvsParParticleTOTH1D();
        delete Prob[1];
        delete Prob_mupiminus;
    }
    if(stagelevel==1){
        Prob[4]->PlotParParticleTOTH1D("Rec","Prob",4,icut+stagelevel,4);
        Prob[2]->PlotParParticleTOTH1D("Rec","Prob",2,icut+stagelevel,2);
        double probcut_electron=GetProbCut(Prob[4],4,icut+stagelevel,4);
        double probcut_piplus=GetProbCut(Prob[2],2,icut+stagelevel,2);

        cout<<" Prob Cut for electron at cut level "<<icut+stagelevel<<" : "<<probcut_electron<<endl;
        cout<<" Prob Cut for piplus at cut level "<<icut+stagelevel<<"            : "<<probcut_piplus<<endl;

        probcut[4]=probcut_electron;
        probcut[2]=probcut_piplus;
        Prob[2]->DeleteParParticleTOTH1D();
        Prob[2]->DeleteEffPurvsParParticleTOTH1D();
        Prob[4]->DeleteParParticleTOTH1D();
        Prob[4]->DeleteEffPurvsParParticleTOTH1D();
        delete Prob[2];
        delete Prob[4];
    }
    if(stagelevel==2){
        Prob[5]->PlotParParticleTOTH1D("Rec","Prob",5,icut+stagelevel,5);
        double probcut_positron=GetProbCut(Prob[5],5,icut+stagelevel,5);
        
        cout<<" Prob Cut for positron at cut level "<<icut+stagelevel<<" : "<<probcut_positron<<endl;

        probcut[5]=probcut_positron;
        Prob[5]->DeleteParParticleTOTH1D();
        Prob[5]->DeleteEffPurvsParParticleTOTH1D();
        delete Prob[5];
    }

    delete tree_Tr;

}


double GetProbCut(TCCplotAnalysisUtils *plot, int iselpart,int icut, int itruepart){

    int nbin=plot->nbinX;
    int max=plot->maxX;
    int min=plot->minX;
    double stepwidth=(double) (max-min)/(double)nbin;
    plot->SetNStep(nbin);
    
    double fom[40];
  
    for(int istep=0;istep<nbin;istep++){
        fom[istep]=0;
        double value_sel=0;
        double value_seltrue=0;
        double value_true=0;
        for(int ibin=istep+1;ibin<=nbin;ibin++){
            value_sel+=plot->ParParticleTOT_Sel[iselpart][icut]->GetBinContent(ibin);
            value_seltrue+=plot->ParParticleTOT_SelTrue[iselpart][icut][itruepart]->GetBinContent(ibin);
        }
        for(int ibin=1;ibin<=nbin;ibin++){
            value_true+=plot->ParParticleTOT_SelTrue[iselpart][icut][itruepart]->GetBinContent(ibin);
        }
        double _fom=(double)plot->GetStatSignificance(value_seltrue,value_sel);
        fom[istep]=_fom; 
        plot->FillSelCutOptParticleCounter(istep,iselpart,icut,value_sel);
        plot->FillSelTrueCutOptParticleCounter(istep,iselpart,icut,itruepart,value_seltrue);
        plot->FillTrueCutOptParticleCounter(istep,itruepart,value_true);
    }

     double maxfom=-1;
     double maxstep=-1;
     for(int istep=0;istep<nbin;istep++){
         if(fom[istep]>maxfom){
             maxfom=fom[istep];
             maxstep=istep;
         }
     }
          
    plot->FillEffPurFOMvsParticleCutOpt(0,stepwidth);
    plot->PlotEffPurFOMvsParticleCutOpt("Prob",iselpart, icut,itruepart);
    double probpos=stepwidth*maxstep; 
    cout<<" stepwidth "<<stepwidth<<" maxstep "<<maxstep<<endl;

    return probpos;
}

void SetpidCuts(string GENE_typefile,double MA,string selcasePAR,string selcaseREA,string ndVersion,int fgdstudy){
    std::string filename=SetOutputName_cutAnalysis(GENE_typefile,MA,selcasePAR,selcaseREA,ndVersion,fgdstudy,1);
    char datname[256];

    if(fgdstudy==0)
        sprintf(datname,"cutresults-pid_%s-MA%.1f_%s-%s_%s.dat",GENE_typefile.c_str(),MA,selcasePAR.c_str(),selcaseREA.c_str(),ndVersion.c_str());
    else if(fgdstudy==1)
        sprintf(datname,"cutresultsFGD1-pid_%s-MA%.1f_%s-%s_%s.dat",GENE_typefile.c_str(),MA,selcasePAR.c_str(),selcaseREA.c_str(),ndVersion.c_str());
    else if(fgdstudy==2)
        sprintf(datname,"cutresultsFGD2-pid_%s-MA%.1f_%s-%s_%s.dat",GENE_typefile.c_str(),MA,selcasePAR.c_str(),selcaseREA.c_str(),ndVersion.c_str());
    
    ifstream infile;
    infile.open(datname);
    if(!infile){
        cout<<" file "<<datname<<" has not been found "<<endl;
        TFile *rootfile_output=new TFile(filename.c_str(),"UPDATE");
        cout<<" OPENING root FILE to update it: "<<filename<<endl;
        if(selcasePAR=="pullcut"){
            RunPullCutAnalysis(rootfile_output);
        }
        if(selcasePAR!="onlyprobcut")
            RunElectronCutAnalysis(selcasePAR,rootfile_output);
        RunProbCutAnalysis(selcasePAR,rootfile_output,0);
        RunProbCutAnalysis(selcasePAR,rootfile_output,1);
        RunProbCutAnalysis(selcasePAR,rootfile_output,2);
        rootfile_output->Close();
        // delete rootfile_output;

        ofstream outfile(datname);//,ofstream::out);
        for(int isel=0;isel<NPARTICLESEL;isel++){
            outfile<<"pull_cut\t"<<pullcut[isel]<<endl;
            cout<<"***pull cut: isel :"<<isel<<"\t"<<pullcut[isel]<<endl;
        }
        for(int iel=0;iel<4;iel++){
            cout<<"***el cut:  pos:"<<iel<<"\t"<<elcut[iel]<<endl;
            outfile<<"el_cut\t"<<elcut[iel]<<endl;
        }
        for(int isel=0;isel<NPARTICLESEL;isel++){
            cout<<"***prob cut: isel :"<<isel<<"\t"<<probcut[isel]<<endl;
            outfile<<"prob_cut\t"<<probcut[isel]<<endl;
        }
        outfile.close();
    }
    else{
        double cut;
        std::string cut_name;
        cout<<" file "<<datname<<" has been found "<<endl;
        const int totparam=NPARTICLESEL*2+4;
        double allcut[totparam];
        std::string allcut_name[totparam];
        for(int i=0;i<NPARTICLEHYP;i++){allcut_name[i]=" ";allcut[i]=0;}
        int count=0;
        
        while(infile>>cut_name>>cut){
            allcut_name[count]=cut_name;
            allcut[count]=cut;
            count++;
        }
        infile.close();
      
        for(int isel=0;isel<NPARTICLESEL;isel++){
            pullcut[isel]=allcut[isel];
            cout<<"***pull cut: isel :"<<isel<<"\t"<<pullcut[isel]<<endl;
        }
        for(int iel=0;iel<4;iel++){
            elcut[iel]=allcut[iel+NPARTICLESEL];
            cout<<"***el cut:  pos:"<<iel<<"\t"<<elcut[iel]<<endl;
        }
        for(int isel=0;isel<NPARTICLESEL;isel++){
            probcut[isel]=allcut[isel+NPARTICLESEL+4];
            if(isel==3)
                probcut[isel]=allcut[0+NPARTICLESEL+4];
            cout<<"***prob cut: isel :"<<isel<<"\t"<<probcut[isel]<<endl;
        }
    }
}

TCCplotAnalysisUtils* RunPullAnalysis(int fgdstudy){

    //********************************************************************
    //FIRST GET THE PULL PARAMETER (mean, sigma) FOR THE FILES TO ANALIZE
    //********************************************************************
    TCCreconAnalysisUtils *recAn=new TCCreconAnalysisUtils();
    TCCplotAnalysisUtils *Pull=new TCCplotAnalysisUtils();
    Pull->InitParParticleHypH1D("Rec","Pull");

    TStopwatch timer;
    timer.Start();
    int entries = mcTruthVertices.GetEntries(); 
    std::cout << "ent: " << entries << std::endl;
    //entries=600;
    for(int ient = 0; ient < entries; ient++)
    {
        mcTruthVertices.GetEntry(ient);
        reconTracker.GetEntry(ient);
        reconGlobal.GetEntry(ient);
        if(ient%1000 == 0)
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
            isfgd1=true;
        }
        else if (NVtxFGD2 >0){
            true_fgd_vertex = (ND::TTruthVerticesModule::TTruthVertex*) VtxFGD2->UncheckedAt(0);
            isfgd2=true;
        }
        else{
            std::cerr << "There should be at least one vertex in one FGD.  Skipping event."<<std::endl;
            continue;
        }
        // for only fgd1 study
        if(fgdstudy==1){
            if(!isfgd1)
                continue;
        }
        else if(fgdstudy==2){        // for only fgd2 study
            if(!isfgd2)
                continue;
        }
                
        recAn->ResetCutFlag();
        int kTotalTPC=0;
        recAn->SetNTracks(NPIDs);
        recAn->FillRecVectors(PIDs);
        recAn->FillTrueVectors(PIDs);
        recAn->SetTotalTracksInTPC(PIDs);
        kTotalTPC=recAn->GetTotalTracksInTPC();
        if(kTotalTPC>0){
            for(int itr=0;itr<NPIDs;itr++){
                bool pass=recAn->fisUsefullTrack[itr];
                if(pass){
                    for(int ihyp=0;ihyp<NPARTICLEHYP;ihyp++){
                        double pull=recAn->GetPull(ihyp,itr);
                        recAn->FillHypH1D(Pull,itr,ihyp,pull,true);
                    }
                }
            }
        }
    }//end ientries

    Pull->MergeParParticleHypH1D_True2Hyp("Rec","Pull");

    return Pull;
}

void SetPullStats(string GENE_typefile,string ndVersion,int fgdstudy){
    std::vector< double > stat;
    TCCplotAnalysisUtils *Pull=new TCCplotAnalysisUtils();
    char datname[256];
    if(GENE_typefile=="Data")
    {
        for(int ihyp=0;ihyp<NPARTICLEHYP;ihyp++){
            pullsigma[ihyp]=1;
            pullmean[ihyp]=0;
        }
    }
    else{
        sprintf(datname,"pullstat_%s_%s.dat",GENE_typefile.c_str(),ndVersion.c_str());
        ifstream infile;
        infile.open(datname);
        if(!infile){
            cout<<" file "<<datname<<" has not been found "<<endl;
            Pull=RunPullAnalysis(fgdstudy);
            ofstream outfile(datname);//,ofstream::out);
            for(int i=0;i<NPARTICLEHYP;i++){
                std::vector< double > _stat;
                _stat=Pull->GetStatFitParParticleHypH1D_True2Hyp("Rec","Pull",i);
                pullmean[i]=_stat[0];
                pullsigma[i]=_stat[1];
                cout<<" ihyp: "<<i<<", mean: "<<_stat[0]<<", sigma: "<<_stat[1]<<endl;
                outfile<<pullmean[i]<<"\t"<<pullsigma[i]<<endl;
            }
            outfile.close();
            //for kaons:
            pullmean[4]=0;
            pullsigma[4]=1;
            //*************
        }
        else{
            cout<<" file "<<datname<<" has been found "<<endl;
            double mean;
            double sigma;
            std::vector< std::vector< double > > vec;
            double allstat[NPARTICLEHYP][2];
            for(int i=0;i<NPARTICLEHYP;i++){allstat[i][0]=0;allstat[i][1]=0;}
            int count=0;
            
            while(infile>>mean>>sigma){
                allstat[count][0]=mean;
                allstat[count][1]=sigma;
                count++;
            }
            infile.close();
            for(int i=0;i<NPARTICLEHYP;i++){
                pullmean[i]=allstat[i][0];
                pullsigma[i]=allstat[i][1];
                cout<<" ihyp :"<<i<<" mean"<<pullmean[i]<<" sigma : "<<pullsigma[i]<<endl;
            }
            //for kaons:
            pullmean[4]=0;
            pullsigma[4]=1;
            //*************
        }
    }
    delete Pull;
}   

void RunReactionCutAnalysis_FillTree(TFile *rootfile_output,string GENE_typefile,string selcasePAR, string selcaseREA,int fgdstudy){
    //********************************************************************
    //RUN PROB CUT
    //********************************************************************
    TGENIEAnalysisUtils *genieUtils=new TGENIEAnalysisUtils();
    TNEUTAnalysisUtils *NeutUtils=new TNEUTAnalysisUtils();
    TCCtruthAnalysisUtils *trueAn=new TCCtruthAnalysisUtils();
    TCCreconAnalysisUtils *recAn=new TCCreconAnalysisUtils();
    TTree *tree_Rea=new TTree("tree_Rea","reaction ");
    recAn->InitTree_selBR(tree_Rea);
    tree_Rea->Branch("_NSELREACTION",&_NSELREACTION,"_NSELREACTION/I");
    tree_Rea->Branch("_NDELAYEDCLUS",&_NDELAYEDCLUS,"_NDELAYEDCLUS/I");
    _NSELREACTION=NSELREACTION;
    _NDELAYEDCLUS=NDELAYEDCLUS;
//***********************************
    tree_Rea->Branch("evid",&_evid,"evid/I");
    tree_Rea->Branch("ntracks",&_ntracks,"ntracks/I");
    tree_Rea->Branch("ntpctracks",&_ntpctracks,"ntpctracks/I");
    tree_Rea->Branch("nfgdtracks",&_nfgdtracks,"nfgdtracks/I");
    tree_Rea->Branch("ndelayedClusters",&_ndelayedClusters,"ndelayedClusters/I");  
    tree_Rea->Branch("evSel",_evSel,"evSel[_NSELREACTION][9]/I");
    tree_Rea->Branch("RecVtx",_RecVtx,"RecVtx[3]/F");
    tree_Rea->Branch("dThp",&_dThp,"dThp/F");
    tree_Rea->Branch("dEp",&_dEp,"dEp/F");
    tree_Rea->Branch("Mnu2",&_Mnu2,"Mnu2/F");  
    tree_Rea->Branch("evG4Code",&_evG4Code,"evG4Code/I");
    tree_Rea->Branch("TrueVtx",_TrueVtx,"TrueVtx[3]/F"); 
    tree_Rea->Branch("OUTleptonPDG",&_OUTleptonPDG,"OUTleptonPDG/F");
    tree_Rea->Branch("OUTnuPDG",&_OUTnuPDG,"OUTnuPDG/F");
    tree_Rea->Branch("delayedClustersPos",_delayedClustersPos,"delayedClustersPos[_NDELAYEDCLUS][3]/F");
    tree_Rea->Branch("delayedClustersVar",_delayedClustersVar,"delayedClustersVar[_NDELAYEDCLUS][3]/F");
    
    recAn->SetCurvCut(curvcut);
    recAn->SetPullStat(pullmean, pullsigma);
    recAn->SetProbCut(probcut);
    recAn->SetPullCut(pullcut[0]);
    recAn->SetElectronCut(elcut);
    recAn->SetNParticleCuts(selcasePAR);
    recAn->SetNCuts(selcaseREA);

    
    TStopwatch timer;
    timer.Start();
    int entries = mcTruthVertices.GetEntries(); 
    //entries=600;
    for(int ient=0;ient<entries;ient++){
         if(ient%500 == 0)
        {
            std::cout <<  "********************************" << std::endl;
            std::cout <<  "    !!!Events Processed " << ient << std::endl;
            timer.Stop();
            std::cout << " Processing time: " << timer.CpuTime() << " " << timer.RealTime() << std::endl;
            std::cout <<  "********************************" << std::endl;
            timer.Start();
        }
         reconTracker.GetEntry(ient);
         reconGlobal.GetEntry(ient);
        //***************************
        //initialize the tree branches
        //***************************
        
         _evid=ient;
         _dEp=-9999;
         _dThp=-9;
         _Mnu2=-9999;
         _RecVtx[0]=-999;
         _RecVtx[1]=-999;
         _RecVtx[2]=-999;
         _ntpctracks=-9;
         _nfgdtracks=-9;
         _ntracks=-9;
         _evG4Code=-999;
         _TrueVtx[0]=-999;
         _TrueVtx[1]=-999;
         _TrueVtx[2]=-999;
         _OUTleptonPDG=0;
         _OUTnuPDG=0;
         for(int j=0;j<NDELAYEDCLUS;j++){
             _delayedClustersPos[j][0]=-999;
             _delayedClustersPos[j][1]=-999;
             _delayedClustersPos[j][2]=-999;
             _delayedClustersVar[j][0]=-999;
             _delayedClustersVar[j][1]=-999;
             _delayedClustersVar[j][2]=-999;
         }
        //*****************************************
        ND::TTruthVerticesModule::TTruthVertex *true_fgd_vertex;      

        mcTruthTrajectories.GetEntry(ient);
        mcTruthVertices.GetEntry(ient);
        bool isfgd1=false;
        if(NVtxFGD1 >0){
            true_fgd_vertex = (ND::TTruthVerticesModule::TTruthVertex*) VtxFGD1->UncheckedAt(0);
            isfgd1=true;
        }
        else if (NVtxFGD2 >0){
            true_fgd_vertex = (ND::TTruthVerticesModule::TTruthVertex*) VtxFGD2->UncheckedAt(0);
            isfgd1=false;
        }
        else{
            std::cerr << "There should be at least one vertex in one FGD.  Skipping event."<<std::endl;
            continue;
        }
        // for only fgd1 study
        if(fgdstudy==1){
            if(!isfgd1)
                continue;
        }
        else if(fgdstudy==2){        // for only fgd2 study
            if(isfgd1)
                continue;
        }


        int OUTleptonPDG=0;
        int OUTnuPDG=0;
        //      True kinematic of the different particles
        // *****************************************************************
        // (for CC)
        // ****************
        TLorentzVector trueLEP_momentum;
        int leptonSize=true_fgd_vertex->LeptonIDs.size();
        if(leptonSize>0){
            trueLEP_momentum=trueAn->GetChargedLeptonMaxMomentum(true_fgd_vertex,13);
            if(trueLEP_momentum.T()!=0){
                OUTleptonPDG=13;
            }
            else{
                trueLEP_momentum=trueAn->GetChargedLeptonMaxMomentum(true_fgd_vertex,11);
                if(trueLEP_momentum.T()!=0){
                    OUTleptonPDG=11;
                }else{
                    trueLEP_momentum=trueAn->GetChargedLeptonMaxMomentum(true_fgd_vertex,15);
                    if(trueLEP_momentum.T()!=0)
                        OUTleptonPDG=15;
                }
            }
        }
        _OUTleptonPDG=OUTleptonPDG;
        // (for NC)
        // ****************        
        TLorentzVector trueNU_momentum;
        int neutralSize=true_fgd_vertex->OtherNeutralIDs.size();        
        if(neutralSize>0){
            trueNU_momentum=trueAn->GetOtherNeutralMaxMomentum(true_fgd_vertex,14);
            if(trueNU_momentum.T()!=0){
                OUTnuPDG=14;
            }
            else{
                trueNU_momentum=trueAn->GetOtherNeutralMaxMomentum(true_fgd_vertex,12);
                if(trueNU_momentum.T()!=0){
                    OUTnuPDG=12;
                }else{
                    trueNU_momentum=trueAn->GetOtherNeutralMaxMomentum(true_fgd_vertex,16);
                    if(trueNU_momentum.T()!=0)
                        OUTnuPDG=16;
                }
            }
        }
        _OUTnuPDG=OUTnuPDG;
        //accept only when the following is NOT happening
        //true vtx in FV and ... (for reco only event )

         //Provide the mode of the reaction:
        //*************************************
        int rxn_code=-999;
        if(GENE_typefile=="Genie"){
            rxn_code=trueAn->GetNeutCodeForGenie(true_fgd_vertex);
            if(rxn_code==0)
                if(genieUtils->IsELNC(true_fgd_vertex)) rxn_code=50;
        }
        else
            rxn_code=NeutUtils->GetReactionCode(true_fgd_vertex);
        _evG4Code=rxn_code;  
        TLorentzVector TrueVtx = true_fgd_vertex->Vertex;
        _TrueVtx[0]=TrueVtx.X();
        _TrueVtx[1]=TrueVtx.Y();
        _TrueVtx[2]=TrueVtx.Z();


               
        //Do reconstruction
        //********************
        //DO TRACK ANALYSIS
        //***************************
        //At least 1 TPC tracks
        //if no tpc track return false;
        //Check for useless tracks
        //Fill all track reco info (mom, ch, pulls, ...)
        //Fill all track true info (mom, ch, pulls, ...) -  if required
        //Set track prob
        //Make the selection
        //Fill true counters - if required
        //***************************
        recAn->ResetCutFlag();      
        recAn->SetRecVertex(PIDs, NPIDs, Vertices, NVertices,ient);
        recAn->TracksAnalysis(PIDs,selcasePAR,selcaseREA,1);//At least one tpc track
     
                
        //BUG: more than 1 true muon (=same traj. matched to 2 diff. track)
        if(recAn->fNumberTpcTr_True[0]>1 ){
            cout<<"!!!!!!!  number of muminus > 1 "
                <<" useless tracks "<<recAn->fnevent_withuselesstracks
                <<endl;
            
            //continue;
        }
       
        recAn->FillTree_selBR();
        //DO PROCESS ANALYSIS
        //*********************************************3
        _ndelayedClusters=NDelayedClusters;
        int totdel=0;
        if(NDelayedClusters<NDELAYEDCLUS)totdel=NDelayedClusters;
        else totdel=NDELAYEDCLUS;

        if(totdel>0) cout<<" delayed clus. "<<NDelayedClusters<<endl;
        
        for(int j = 0; j < totdel; j++){
            ND::TTrackerReconModule::TFgdCluster *cluster
                = (ND::TTrackerReconModule::TFgdCluster*)(*DelayedClusters)[j];
            _delayedClustersPos[j][0]=(cluster->Position.X());
            _delayedClustersPos[j][1]=(cluster->Position.Y());
            _delayedClustersPos[j][2]=(cluster->Position.Z());
            _delayedClustersVar[j][0]=(cluster->Variance.X());
            _delayedClustersVar[j][1]=(cluster->Variance.Y());
            _delayedClustersVar[j][2]=(cluster->Variance.Z());
        }


        
        //Fill the tree branches
        //***************************
        TLorentzVector recVTX= recAn->GetRecVertex();
        TVector3 vtxrec(recVTX.X(),recVTX.Y(),recVTX.Z());
        _RecVtx[0]=vtxrec.X();
        _RecVtx[1]=vtxrec.Y();
        _RecVtx[2]=vtxrec.Z();
        _dThp=recAn->GetDeltaThetaP();
    //     if(_dThp!=-9)
//             cout<<" delta theta p "<<_dThp<<" rxn cdoe "<<rxn_code<<endl;
        _dEp=recAn->GetDeltaEnergyP();
        _Mnu2=recAn->GetMnuSquare();
        
        int kTotalTPC=recAn->fTotalTPC;
        int kTotalFGD=recAn->fTotalTPCFGD-kTotalTPC;
        _ntpctracks=kTotalTPC;//only usefull tpc tracks
        _nfgdtracks=kTotalFGD;
        _ntracks=NPIDs; //here take all the tracks also the tracks that are useless
        tree_Rea->Fill();
    }
    tree_Rea->Write();
    delete genieUtils;
    delete NeutUtils;    
    delete tree_Rea;
    delete recAn;
    delete trueAn;

}


void SetreaCuts(TFile *rootfile_output,string GENE_typefile,double MA,string selcasePAR,string selcaseREA,string ndVersion,int fgdstudy){
    char datname[256];    
    if(fgdstudy==0)
        sprintf(datname,"cutresults-rea_%s-MA%.1f_%s-%s_%s.dat",GENE_typefile.c_str(),MA,selcasePAR.c_str(),selcaseREA.c_str(),ndVersion.c_str());
    else if(fgdstudy==1)
        sprintf(datname,"cutresultsFGD1-rea_%s-MA%.1f_%s-%s_%s.dat",GENE_typefile.c_str(),MA,selcasePAR.c_str(),selcaseREA.c_str(),ndVersion.c_str());
    else if(fgdstudy==2)
        sprintf(datname,"cutresultsFGD2-rea_%s-MA%.1f_%s-%s_%s.dat",GENE_typefile.c_str(),MA,selcasePAR.c_str(),selcaseREA.c_str(),ndVersion.c_str());
    
    ifstream infile;
    infile.open(datname);
    if(!infile){
        cout<<" file "<<datname<<" has not been found "<<endl;
       //  for(int ifgd=0;ifgd<2;ifgd++){
//             RunFVCutAnalysis(selcasePAR,selcaseREA,rootfile_output,ifgd,0);
//             RunFVCutAnalysis(selcasePAR,selcaseREA,rootfile_output,ifgd,1);
//             RunFVCutAnalysis(selcasePAR,selcaseREA,rootfile_output,ifgd,2);
//         }
        fvcut.SetXYZ(200,200,10);
        
        if(selcaseREA!="simple"){
            RundThpCutAnalysis(selcasePAR,selcaseREA,rootfile_output);
            RunMichelElectronCutAnalysis(selcasePAR,selcaseREA,rootfile_output);
        }
        
        ofstream outfile(datname);//,ofstream::out);
       
        for(int iaxis=0;iaxis<3;iaxis++){
            outfile<<"FV_cut\t"<<fvcut[iaxis]<<endl;
            cout<<"***FV cut: axis :"<<iaxis<<"\t"<<fvcut[iaxis]<<endl;
        }
        outfile<<"dThpcut\t"<<dthpcut<<endl;
        cout<<"***dThp cut:"<<dthpcut<<endl;
        outfile<<"dDelC_cut\t"<<dDelCcut<<endl;
        cout<<"***dDelC cut:"<<dDelCcut<<endl;
        outfile.close();
    }
    else{
        double cut;
        std::string cut_name;
        cout<<" file "<<datname<<" has been found "<<endl;
        const int totparam=NPARTICLESEL*2+4;
        double allcut[totparam];
        std::string allcut_name[totparam];
        for(int i=0;i<NPARTICLEHYP;i++){allcut_name[i]=" ";allcut[i]=0;}
        int count=0;
        
        while(infile>>cut_name>>cut){
            allcut_name[count]=cut_name;
            allcut[count]=cut;
            count++;
        }
        infile.close();
        for(int iaxis=0;iaxis<3;iaxis++){
            fvcut[iaxis]=allcut[iaxis];
            cout<<"***FV cut: axis :"<<iaxis<<"\t"<<fvcut[iaxis]<<endl;
        }
        dthpcut=allcut[3];
        cout<<"***dthp cut:"<<dthpcut<<endl;
        dDelCcut=allcut[4];
        cout<<"***dDelC cut:"<<dDelCcut<<endl;
    }
}
void RunFVCutAnalysis(string selcasePAR,string selcaseREA,TFile *rootfile_output,int ifgd, int axis){
    TGeneAnalysisUtils *anUtils=new TGeneAnalysisUtils();
    TCCreconAnalysisUtils *recAn=new TCCreconAnalysisUtils();
    recAn->SetPullStat(pullmean, pullsigma);
    recAn->SetCurvCut(curvcut);
    recAn->SetNParticleCuts(selcasePAR);
    recAn->SetNCuts(selcaseREA);
    int nparticlecuts=recAn->GetNParticleCuts(selcasePAR);
    int ncuts=recAn->GetNCuts(selcaseREA);
    char cutname[256];
    char caxis[100];
    if(axis==0)sprintf(caxis,"FVx");
    else if(axis==1)sprintf(caxis,"FVxy");
    else if(axis==2)sprintf(caxis,"FVxyz");
        
    sprintf(cutname,"%s-FGD%d",caxis,ifgd+1);
    TCCplotAnalysisUtils *FV;
    FV=new TCCplotAnalysisUtils(rootfile_output,nparticlecuts,ncuts);
    FV->InitCutOptReaCounter();
    FV->InitEffPurFOMvsReaCutOpt(cutname);

    //axis X: -931.6,931.6 => nstep<=931.6/stepwidth or stepwidth<=931.6/(nstep-1)
    //axis Y: -906.6,956.6 => nstep<=931.6/stepwidth or stepwidth<=931.6/(nstep-1) 
    //axis Z: 170.3,490.5 => nstep<=320.2/stepwidth or stepwidth<=320.2/(nstep-1) 
    int _nstep=20;
    FV->SetNStep(_nstep);
    double stepwidth;
    if(axis==0 || axis==1) 
        stepwidth=50;
    else if(axis==2)
        stepwidth=5;

    TTree *tree_Rea=(TTree*) rootfile_output->Get("tree_Rea");

    tree_Rea->SetBranchAddress("RecVtx",_RecVtx);
    tree_Rea->SetBranchAddress("TrueVtx",_TrueVtx);
    tree_Rea->SetBranchAddress("ntpctracks",&_ntpctracks);
    tree_Rea->SetBranchAddress("evG4Code",&_evG4Code);
          
    int entries=tree_Rea->GetEntries();
    cout<<" nb of entries "<<entries<<endl;
    cout<<" ncuts !!! "<<ncuts<<endl;

     
    int icut=0;
    TStopwatch timer;
    timer.Start();
    //entries=600;
    
    for(int ient=0;ient<entries;ient++){
        if(ient%1000 == 0)
        {
            std::cout <<  "********************************" << std::endl;
            std::cout <<  "    !!!Events Processed " << ient <<std::endl;
            timer.Stop();
            std::cout << " Processing time: " << timer.CpuTime() << " " << timer.RealTime() << std::endl;
            std::cout <<  "********************************" << std::endl;
            timer.Start();
        }
        tree_Rea->GetEntry(ient);

//       int isCC=trueAn->IsReaction(_evG4Code,0);
//       double distz, stepwidthz=20.85/2; // (un module +1/2 air gap)/2
        
        TVector3 trueVtx(_TrueVtx[0],_TrueVtx[1],_TrueVtx[2]);
        TVector3 recVtx(_RecVtx[0],_RecVtx[1],_RecVtx[2]);

        double dist;
       
        if(ifgd==0){
            if(anUtils->IsInFGD2(trueVtx)) continue;
        }
        else if(ifgd==1)
            if(anUtils->IsInFGD1(trueVtx)) continue;
        
        for(int istep=0;istep<_nstep;istep++)
        {
            dist=stepwidth*istep;
            if(axis==0){
                TVector3 distV(dist,0,0);
                anUtils->DefineFiducialVolume(distV);
            }
            if(axis==1){
                TVector3 distV(fvcut[0],dist,0);
                anUtils->DefineFiducialVolume(distV);
            }
            if(axis==2)
            {
                TVector3 distV(fvcut[0],fvcut[1],dist);
                anUtils->DefineFiducialVolume(distV);
            }
            bool isrecFV=anUtils->IsInFiducialVolume(recVtx);
          
            bool istrueFV=anUtils->IsInFiducialVolume(trueVtx);       


            //the fiducial volume is optimized to have the best efficiency when we have at least a track in the tpc
            if(isrecFV && _ntpctracks>=1)
                FV->FillSelCutOptReaCounter(istep,0,icut);
            if(isrecFV && istrueFV && _ntpctracks>=1 )
                FV->FillSelTrueCutOptReaCounter(istep,0,icut,0);
            if(istrueFV )
                FV->FillTrueCutOptReaCounter(istep,0);

        }
    }
       
    
    double FVcut=GetFVCut(FV,_nstep, stepwidth,ifgd,0,icut,0);
    cout<<cutname<<" Cut at cut level "<<icut<<" : "<<FVcut<<endl;
    for(int iaxis=0;iaxis<3;iaxis++){
        if(axis==iaxis)
            fvcut[iaxis]=FVcut;
    }

    delete FV;


}

double GetFVCut(TCCplotAnalysisUtils *plot,const int _nstep,double stepwidth,int ifgd, int iselrea,int icut, int irea){

    char cutname[256];
    sprintf(cutname,"FV-FGD%d",ifgd);
    double eff[_nstep];
    for(int istep=0;istep<_nstep;istep++)
    {
        double _eff=(double)plot->GetEfficiency(plot->TotalCutOpt_SelTrue[iselrea][icut][irea][istep],plot->TotalCutOpt_True[irea][istep]);
        eff[istep]=_eff;
    }

     double maxeff=-1;
     double maxstep=-1;
     for(int istep=0;istep<_nstep;istep++){
         if(eff[istep]>maxeff){
             maxeff=eff[istep];
             maxstep=istep;
         }
     }
          
    plot->FillEffPurFOMvsReaCutOpt(0,stepwidth);
    plot->PlotEffPurFOMvsReaCutOpt(cutname,iselrea,icut,irea);
    double probpos=stepwidth*maxstep; 
    cout<<" stepwidth "<<stepwidth<<" maxstep "<<maxstep<<endl;

    return probpos;
}





void RundThpCutAnalysis(string selcasePAR,string selcaseREA,TFile *rootfile_output){
    //This cut is meant for selected CCQE until this level
    //In particular it is only for event that have only 2 tracks in particular 2 tpc tracks

   
    TCCtruthAnalysisUtils *trueAn=new TCCtruthAnalysisUtils();
    TCCreconAnalysisUtils *recAn=new TCCreconAnalysisUtils();
    recAn->SetPullStat(pullmean, pullsigma);
    recAn->SetCurvCut(curvcut);
    recAn->SetNParticleCuts(selcasePAR);
    recAn->SetNCuts(selcaseREA);
    int nparticlecuts=recAn->GetNParticleCuts(selcasePAR);
    int ncuts=recAn->GetNCuts(selcaseREA);
    TCCplotAnalysisUtils *dThp;
    TCCplotAnalysisUtils *dEp;
    TCCplotAnalysisUtils *Mnu2;

    dThp=new TCCplotAnalysisUtils(rootfile_output,nparticlecuts,ncuts);
    dThp->SetRange(60,0,TMath::Pi(),"X");
    dThp->InitParH1D("Rec","dThp");
    dThp->InitEffPurvsParH1D("Rec","dThp");
    
    dEp=new TCCplotAnalysisUtils(rootfile_output,nparticlecuts,ncuts);
    dEp->InitParH1D("Rec","dEp");
    dEp->InitEffPurvsParH1D("Rec","dEp");

    Mnu2=new TCCplotAnalysisUtils(rootfile_output,nparticlecuts,ncuts);
    Mnu2->InitParH1D("Rec","Mnu2");
    Mnu2->InitEffPurvsParH1D("Rec","Mnu2");
     
    
    TTree *tree_Rea=(TTree*) rootfile_output->Get("tree_Rea");
    tree_Rea->SetBranchAddress("_NTRACKS",&_NTRACKS);
    tree_Rea->SetBranchAddress("_NDELAYEDCLUS",&_NDELAYEDCLUS);
    tree_Rea->SetBranchAddress("_NSELREACTION",&_NSELREACTION);

    tree_Rea->SetBranchAddress("evid",&_evid);
    tree_Rea->SetBranchAddress("ntracks",&_ntracks);
    tree_Rea->SetBranchAddress("ntpctracks",&_ntpctracks);
    tree_Rea->SetBranchAddress("nfgdtracks",&_nfgdtracks);
    tree_Rea->SetBranchAddress("dThp",&_dThp);
    tree_Rea->SetBranchAddress("dEp",&_dEp);
    tree_Rea->SetBranchAddress("Mnu2",&_Mnu2);
    tree_Rea->SetBranchAddress("ndelayedClusters",&_ndelayedClusters);
    tree_Rea->SetBranchAddress("RecVtx",_RecVtx);
    tree_Rea->SetBranchAddress("evG4Code",&_evG4Code);
    tree_Rea->SetBranchAddress("TrueVtx",_TrueVtx);
    tree_Rea->SetBranchAddress("OUTleptonPDG",&_OUTleptonPDG);
    tree_Rea->SetBranchAddress("OUTnuPDG",&_OUTnuPDG);
    tree_Rea->SetBranchAddress("trSel",_trSel);
    
    int entries=tree_Rea->GetEntries();
    cout<<" nb of entries "<<entries<<endl;
    cout<<" ncuts !!! "<<ncuts<<endl;
    int icut=ncuts-3;//
    TStopwatch timer;
    timer.Start();
    //entries=600;
    int isreaction[NREACTION];
    for(int ient=0;ient<entries;ient++){
        if(ient%5000 == 0)
        {
            std::cout <<  "********************************" << std::endl;
            std::cout <<  "    !!!Events Processed " << ient << std::endl;
            timer.Stop();
            std::cout << " Processing time: " << timer.CpuTime() << " " << timer.RealTime() << std::endl;
            std::cout <<  "********************************" << std::endl;
            timer.Start();
        }
        tree_Rea->GetEntry(ient);
        for(int irea=0;irea<NREACTION;irea++){
            isreaction[irea]=0;
            isreaction[irea]=(int)trueAn->IsReaction(_evG4Code,irea);
        }
     

        TVector3 recVtx(_RecVtx[0],_RecVtx[1],_RecVtx[2]);
        recAn->SetRecVertex(recVtx);
        recAn->SetFiducialVolumeCut(fvcut);
        recAn->SetNTPCTracks(_ntpctracks);
        recAn->SetNTPCFGDTracks(_ntpctracks+_nfgdtracks);
        recAn->SetNTracks(_ntracks);
        recAn->ResetTrackSelInfo();
        int tottracks=0;
        if(_ntracks<NTRACKS)tottracks=_ntracks;
        else  tottracks=NTRACKS;
        for(int itr=0;itr<tottracks;itr++){
            for(int iparcut=0;iparcut<nparticlecuts;iparcut++){
                for(int iselpar=0;iselpar<NPARTICLESEL;iselpar++){
                    recAn->SetTrackSelInfo(itr,_trSel[itr][iselpar][iparcut],iparcut);
                }
            }
            recAn->IncrementTrackSelInfo(itr);
        }

       
        //this cut is only for selected CCQE
        int iselrea=1;
        bool iscut=false;
   
        if(selcaseREA=="default") iscut=recAn->IsSelReaction(iselrea,icut);
        else if(selcaseREA=="SKtopo") iscut=recAn->IsSelReactionSKtopo(iselrea,icut);               
        if(iscut){
            dThp->FillSelReaH1D(iselrea,icut,_dThp,1);
            dEp->FillSelReaH1D(iselrea,icut,_dEp,1);
            Mnu2->FillSelReaH1D(iselrea,icut,_Mnu2,1);
            for(int irea=1;irea<4;irea++){
                if(isreaction[irea]){
                    dThp->FillSelTrueReaH1D(iselrea,icut,irea,_dThp,1);
                    dEp->FillSelTrueReaH1D(iselrea,icut,irea,_dEp,1);
                    Mnu2->FillSelTrueReaH1D(iselrea,icut,irea,_Mnu2,1);
                }
            }
        }
        for(int irea=1;irea<4;irea++){
            if(isreaction[irea]){
                dThp->FillTrueReaH1D(irea,_dThp,1);
                dEp->FillTrueReaH1D(irea,_dEp,1);
                Mnu2->FillTrueReaH1D(irea,_Mnu2,1);
            }
        }
    }

    int iselrea=1;
    for(int irea=1;irea<4;irea++){
        dThp->PlotParH1D("Rec","dThp",iselrea,icut,irea);
        dEp->PlotParH1D("Rec","dEp",iselrea,icut,irea);
        Mnu2->PlotParH1D("Rec","Mnu2p",iselrea,icut,irea);
    }
    double dthpcut_ccqe=GetdThpCut(dThp,1,icut,1);
    double dthpcut_ccnqe=GetdThpCut(dThp,1,icut,2);
    double dthpcut_ccppiplus=GetdThpCut(dThp,1,icut,3);    
    cout<<" dThp Cut for ccqe  at cut level "<<icut<<" : "<<dthpcut_ccqe<<endl;
    cout<<" dThp Cut for ccnqe at cut level "<<icut<<" : "<<dthpcut_ccnqe<<endl;
    cout<<" dThp Cut for ccppi+ at cut level "<<icut<<": "<<dthpcut_ccppiplus<<endl;    
    dthpcut=dthpcut_ccqe;

    double depcut_ccqe=GetdEpCut(dEp,1,icut,1);
    double depcut_ccnqe=GetdEpCut(dEp,1,icut,2);
    double depcut_ccppiplus=GetdEpCut(dEp,1,icut,3);    
    cout<<" dEp Cut for ccqe  at cut level "<<icut<<" : "<<depcut_ccqe<<endl;
    cout<<" dEp Cut for ccnqe at cut level "<<icut<<" : "<<depcut_ccnqe<<endl;
    cout<<" dEp Cut for ccppi+ at cut level "<<icut<<": "<<depcut_ccppiplus<<endl;

    double mnu2cut_ccqe=GetMnu2Cut(Mnu2,1,icut,1);
    double mnu2cut_ccnqe=GetMnu2Cut(Mnu2,1,icut,2);
    double mnu2cut_ccppiplus=GetMnu2Cut(Mnu2,1,icut,3);    
    cout<<" Mnu2 Cut for ccqe  at cut level "<<icut<<" : "<<mnu2cut_ccqe<<endl;
    cout<<" Mnu2 Cut for ccnqe at cut level "<<icut<<" : "<<mnu2cut_ccnqe<<endl;
    cout<<" Mnu2 Cut for ccppi+ at cut level "<<icut<<": "<<mnu2cut_ccppiplus<<endl;

    mnu2cut=mnu2cut_ccqe;
    
    dThp->DeleteParH1D();
    dThp->DeleteEffPurvsParH1D();
    delete dThp;
    
    dEp->DeleteParH1D();
    dEp->DeleteEffPurvsParH1D();
    delete dEp;
    
    Mnu2->DeleteParH1D();
    Mnu2->DeleteEffPurvsParH1D();
    delete Mnu2;
    


}

double GetdThpCut(TCCplotAnalysisUtils *plot, int iselrea,int icut, int irea){

    int nbin=plot->nbinX;
    int max=plot->maxX;
    int min=plot->minX;
    double stepwidth=(double) (max-min)/(double)(nbin);
    plot->SetNStep(nbin);
    const int _nbin=nbin;
    double fom[_nbin];
     int ncuts=plot->GetNCuts();
    int nparticlecuts=plot->GetNParticleCuts();
    TFile *rootfile_output=plot->rootfile_output;
    TCCplotAnalysisUtils *graph;
    graph=new TCCplotAnalysisUtils(rootfile_output,nparticlecuts,ncuts);
    graph->SetNStep(nbin);
    graph->InitCutOptReaCounter();
    for(int istep=0;istep<nbin;istep++){
        fom[istep]=0;
        double value_sel=0;
        double value_seltrue=0;
        double value_true=0;
        double value_bg=0;
        if(irea==1){
            for(int ibin=1;ibin<=istep+1;ibin++){
                value_sel+=plot->Par_Sel[iselrea][icut]->GetBinContent(ibin);
                value_seltrue+=plot->Par_SelTrue[iselrea][icut][irea]->GetBinContent(ibin);

            }
            for(int ibin=1;ibin<=nbin;ibin++){
                value_true+=plot->Par_SelTrue[iselrea][icut][irea]->GetBinContent(ibin);
            }
        }
        else if(irea>1){
            for(int ibin=istep+1;ibin<=nbin;ibin++){
                value_sel+=plot->Par_Sel[iselrea][icut]->GetBinContent(ibin);
                value_seltrue+=plot->Par_SelTrue[iselrea][icut][irea]->GetBinContent(ibin);
            }
            for(int ibin=1;ibin<=nbin;ibin++){
                value_true+=plot->Par_SelTrue[iselrea][icut][irea]->GetBinContent(ibin);
            }

        }
        double _fom=(double)plot->GetStatSignificance(value_seltrue,value_sel);
        fom[istep]=_fom;
      
        graph->FillSelCutOptReaCounter(istep,iselrea,icut,value_sel);
        graph->FillSelTrueCutOptReaCounter(istep,iselrea,icut,irea,value_seltrue);
        graph->FillTrueCutOptReaCounter(istep,irea,value_true);
    }

     double maxfom=-1;
     double maxstep=-1;
     for(int istep=0;istep<nbin;istep++){
         if(fom[istep]>maxfom){
             maxfom=fom[istep];
             maxstep=istep;
         }
     }
    graph->InitEffPurFOMvsReaCutOpt("dThp");
    graph->FillEffPurFOMvsReaCutOpt(iselrea,icut,irea,0,stepwidth);
    graph->PlotEffPurFOMvsReaCutOpt("dThp",iselrea,icut,irea);
    delete graph;
    double probpos=stepwidth*maxstep; 
    cout<<" stepwidth "<<stepwidth<<" maxstep "<<maxstep<<endl;

    return probpos;
}

double GetdEpCut(TCCplotAnalysisUtils *plot, int iselrea,int icut, int irea){

    int nbin=plot->nbinX;
    int max=plot->maxX;
    int min=plot->minX;
    double stepwidth=(double) (max-min)/(double)nbin;
    plot->SetNStep(nbin);
    const int _nbin=nbin;
    double fom[_nbin];
     int ncuts=plot->GetNCuts();
    int nparticlecuts=plot->GetNParticleCuts();
    TFile *rootfile_output=plot->rootfile_output;
    TCCplotAnalysisUtils *graph;
    graph=new TCCplotAnalysisUtils(rootfile_output,nparticlecuts,ncuts);
    graph->SetNStep(nbin);
    graph->InitCutOptReaCounter();

    for(int istep=0;istep<nbin;istep++){
        fom[istep]=0;
        double value_sel=0;
        double value_seltrue=0;
        double value_true=0;
        if(irea>1){
            for(int ibin=1;ibin<=istep+1;ibin++){
                value_sel+=plot->Par_Sel[iselrea][icut]->GetBinContent(ibin);
                value_seltrue+=plot->Par_SelTrue[iselrea][icut][irea]->GetBinContent(ibin);
            }
            for(int ibin=1;ibin<=nbin;ibin++){
                value_true+=plot->Par_SelTrue[iselrea][icut][irea]->GetBinContent(ibin);
            }
        }
        else if(irea==1){
            for(int ibin=istep+1;ibin<=nbin;ibin++){
                value_sel+=plot->Par_Sel[iselrea][icut]->GetBinContent(ibin);
                value_seltrue+=plot->Par_SelTrue[iselrea][icut][irea]->GetBinContent(ibin);
            }
            for(int ibin=1;ibin<=nbin;ibin++){
                value_true+=plot->Par_SelTrue[iselrea][icut][irea]->GetBinContent(ibin);
            }

        }
        double _fom=(double)plot->GetStatSignificance(value_seltrue,value_sel);
        fom[istep]=_fom;
      
        graph->FillSelCutOptReaCounter(istep,iselrea,icut,value_sel);
        graph->FillSelTrueCutOptReaCounter(istep,iselrea,icut,irea,value_seltrue);
        graph->FillTrueCutOptReaCounter(istep,irea,value_true);
    }

     double maxfom=-1;
     double maxstep=-1;
     for(int istep=0;istep<nbin;istep++){
         if(fom[istep]>maxfom){
             maxfom=fom[istep];
             maxstep=istep;
         }
     }
     graph->InitEffPurFOMvsReaCutOpt("dEp");
    graph->FillEffPurFOMvsReaCutOpt(iselrea,icut,irea,min,stepwidth);
    graph->PlotEffPurFOMvsReaCutOpt("dEp",iselrea,icut,irea);
    delete graph;
    double probpos=stepwidth*maxstep+min; 
    cout<<" stepwidth "<<stepwidth<<" maxstep "<<maxstep<<endl;

    return probpos;
}

double GetMnu2Cut(TCCplotAnalysisUtils *plot, int iselrea,int icut, int irea){

    int nbin=plot->nbinX;
    int max=plot->maxX;
    int min=plot->minX;
    double stepwidth=(double) (max-min)/(double)nbin;
    plot->SetNStep(nbin);
    const int _nbin=nbin;
    double fom[_nbin];
     int ncuts=plot->GetNCuts();
    int nparticlecuts=plot->GetNParticleCuts();
    TFile *rootfile_output=plot->rootfile_output;
    TCCplotAnalysisUtils *graph;
    graph=new TCCplotAnalysisUtils(rootfile_output,nparticlecuts,ncuts);
    graph->SetNStep(nbin);
    graph->InitCutOptReaCounter();
    
    for(int istep=0;istep<nbin;istep++){
        fom[istep]=0;
        double value_sel=0;
        double value_seltrue=0;
        double value_true=0;
        if(irea>1){
            for(int ibin=1;ibin<=istep+1;ibin++){
                value_sel+=plot->Par_Sel[iselrea][icut]->GetBinContent(ibin);
                value_seltrue+=plot->Par_SelTrue[iselrea][icut][irea]->GetBinContent(ibin);
            }
            for(int ibin=1;ibin<=nbin;ibin++){
                value_true+=plot->Par_SelTrue[iselrea][icut][irea]->GetBinContent(ibin);
            }
        }
        else if(irea==1){
            for(int ibin=istep+1;ibin<=nbin;ibin++){
                value_sel+=plot->Par_Sel[iselrea][icut]->GetBinContent(ibin);
                value_seltrue+=plot->Par_SelTrue[iselrea][icut][irea]->GetBinContent(ibin);
            }
            for(int ibin=1;ibin<=nbin;ibin++){
                value_true+=plot->Par_SelTrue[iselrea][icut][irea]->GetBinContent(ibin);
            }

        }
        double _fom=(double)plot->GetStatSignificance(value_seltrue,value_sel);
        fom[istep]=_fom;
      
        graph->FillSelCutOptReaCounter(istep,iselrea,icut,value_sel);
        graph->FillSelTrueCutOptReaCounter(istep,iselrea,icut,irea,value_seltrue);
        graph->FillTrueCutOptReaCounter(istep,irea,value_true);
    }

     double maxfom=-1;
     double maxstep=-1;
     for(int istep=0;istep<nbin;istep++){
         if(fom[istep]>maxfom){
             maxfom=fom[istep];
             maxstep=istep;
         }
     }
     graph->InitEffPurFOMvsReaCutOpt("Mnu2");
     graph->FillEffPurFOMvsReaCutOpt(iselrea, icut,irea,min,stepwidth);
     graph->PlotEffPurFOMvsReaCutOpt("Mnu2",iselrea,icut,irea);
     delete graph;
     double probpos=stepwidth*maxstep+min; 
     cout<<" stepwidth "<<stepwidth<<" maxstep "<<maxstep<<endl;

    return probpos;
}



void RunMichelElectronCutAnalysis(string selcasePAR,string selcaseREA,TFile *rootfile_output){
    //Michel electron cut: destinated for events selected as CCQE until this level
//    fvcut.SetXYZ(200,200,10);

    TCCtruthAnalysisUtils *trueAn=new TCCtruthAnalysisUtils();
    TCCreconAnalysisUtils *recAn=new TCCreconAnalysisUtils();
    recAn->SetPullStat(pullmean, pullsigma);
    recAn->SetCurvCut(curvcut);
    recAn->SetNParticleCuts(selcasePAR);
    recAn->SetNCuts(selcaseREA);
    int nparticlecuts=recAn->GetNParticleCuts(selcasePAR);
    int ncuts=recAn->GetNCuts(selcaseREA);
    TCCplotAnalysisUtils *dDelC;
    TCCplotAnalysisUtils *NDel;
    NDel=new TCCplotAnalysisUtils(rootfile_output,nparticlecuts,ncuts);
    NDel->InitParH1D("Rec","NDel");
    dDelC=new TCCplotAnalysisUtils(rootfile_output,nparticlecuts,ncuts);
    dDelC->InitParH1D("Rec","dDelC");
    dDelC->InitParH2D("Rec","dDelC","Rec","NDel");
    dDelC->InitEffPurvsParH1D("Rec","dDelC");    
  
    
    TTree *tree_Rea=(TTree*) rootfile_output->Get("tree_Rea");
    tree_Rea->SetBranchAddress("_NTRACKS",&_NTRACKS);
    tree_Rea->SetBranchAddress("_NDELAYEDCLUS",&_NDELAYEDCLUS);
    tree_Rea->SetBranchAddress("_NSELREACTION",&_NSELREACTION);

    tree_Rea->SetBranchAddress("evid",&_evid);
    tree_Rea->SetBranchAddress("ntracks",&_ntracks);
    tree_Rea->SetBranchAddress("ntpctracks",&_ntpctracks);
    tree_Rea->SetBranchAddress("nfgdtracks",&_nfgdtracks);
    tree_Rea->SetBranchAddress("dThp",&_dThp);
    tree_Rea->SetBranchAddress("dEp",&_dEp);
    tree_Rea->SetBranchAddress("Mnu2",&_Mnu2);
    tree_Rea->SetBranchAddress("ndelayedClusters",&_ndelayedClusters);
    tree_Rea->SetBranchAddress("delayedClustersPos",_delayedClustersPos);
    tree_Rea->SetBranchAddress("delayedClustersVar",_delayedClustersVar);
    tree_Rea->SetBranchAddress("RecVtx",_RecVtx);
    tree_Rea->SetBranchAddress("evG4Code",&_evG4Code);
    tree_Rea->SetBranchAddress("TrueVtx",_TrueVtx);
    tree_Rea->SetBranchAddress("OUTleptonPDG",&_OUTleptonPDG);
    tree_Rea->SetBranchAddress("OUTnuPDG",&_OUTnuPDG);
    tree_Rea->SetBranchAddress("trSel",_trSel);
    
    int entries=tree_Rea->GetEntries();
    cout<<" nb of entries "<<entries<<endl;
    cout<<" ncuts !!! "<<ncuts<<endl;
    int icut=ncuts-2;//
    TStopwatch timer;
    timer.Start();
    //entries=600;
    int isreaction[NREACTION];
    
    for(int ient=0;ient<entries;ient++){
        if(ient%500 == 0)
        {
            std::cout <<  "********************************" << std::endl;
            std::cout <<  "    !!!Events Processed " << ient << std::endl;
            timer.Stop();
            std::cout << " Processing time: " << timer.CpuTime() << " " << timer.RealTime() << std::endl;
            std::cout <<  "********************************" << std::endl;
            timer.Start();
        }
        tree_Rea->GetEntry(ient);
        for(int irea=0;irea<NREACTION;irea++){
            isreaction[irea]=0;
            isreaction[irea]=(int)trueAn->IsReaction(_evG4Code,irea);
        }
       
        TVector3 recVtx(_RecVtx[0],_RecVtx[1],_RecVtx[2]);
        recAn->SetRecVertex(recVtx);
        recAn->SetFiducialVolumeCut(fvcut);
        recAn->SetNTPCTracks(_ntpctracks);
        recAn->SetNTPCFGDTracks(_ntpctracks+_nfgdtracks);
        recAn->SetNTracks(_ntracks);
        recAn->ResetTrackSelInfo();
        int tottracks=0;
        if(_ntracks<NTRACKS)tottracks=_ntracks;
        else  tottracks=NTRACKS;
        for(int itr=0;itr<tottracks;itr++){
            for(int iparcut=0;iparcut<nparticlecuts;iparcut++)
                for(int iselpar=0;iselpar<NPARTICLESEL;iselpar++){
                    recAn->SetTrackSelInfo(itr,_trSel[itr][iselpar][iparcut],iparcut);
                }

            recAn->IncrementTrackSelInfo(itr);
        }
        recAn->HighDeltaThetaP(_dThp,dthpcut);//0.5=dthp cut
        recAn->HighMnuSquare(_Mnu2,mnu2cut);//-0.4=mnu2 cut
        int totdel=0;
        if(_ndelayedClusters<NDELAYEDCLUS)totdel=_ndelayedClusters;
        else totdel=NDELAYEDCLUS;

        double _dDelC=99999;
        for(int idel=0;idel<totdel;idel++){
            TVector3 DelC(_delayedClustersPos[idel][0],_delayedClustersPos[idel][1],_delayedClustersPos[idel][2]);
            TVector3 Var(_delayedClustersVar[idel][0],_delayedClustersVar[idel][1],_delayedClustersVar[idel][2]);
            TVector3 dp=(DelC-recVtx);
            //take only the distance for each plane: if the scintillator does not have the y coordinate
            //(because it is X-Z scint),
            //then take the distance to the vertex in this plane, if it is a Y-Z scint,
            //then take the distance in the other plane.
            if(Var.X() == 0.0){
                _dDelC = sqrt(dp.X()*dp.X() + dp.Z()*dp.Z());
//                cout<<" varX=0  dp.y "<<dp.Y()<<" delC y "<<DelC.Y()<<endl;
            }
            else{
                _dDelC = sqrt(dp.Y()*dp.Y() + dp.Z()*dp.Z());       
//                cout<<" varY=0  dp.x "<<dp.X()<<" delC x "<<DelC.X()<<endl;
            }
        }

        //FOR SEL CCQE: <=> iselrea=1
        int iselrea=1;
        bool iscut=false;
        if(selcaseREA=="default") iscut=recAn->IsSelReaction(iselrea,icut);
        else if(selcaseREA=="SKtopo") iscut=recAn->IsSelReactionSKtopo(iselrea,icut);               
        if(iscut){
            NDel->FillSelReaH1D(iselrea,icut,_ndelayedClusters,1);
            if(_dDelC!=99999){
                dDelC->FillSelReaH1D(iselrea,icut,_dDelC,1);
                dDelC->FillSelReaH2D(iselrea,icut,_dDelC,_ndelayedClusters,1);
            }
            for(int irea=1;irea<NREACTION-1;irea++){
                if(isreaction[irea]){
                    NDel->FillSelTrueReaH1D(iselrea,icut,irea,_ndelayedClusters,1);
                    if(_dDelC!=99999){
                        dDelC->FillSelTrueReaH1D(iselrea,icut,irea,_dDelC,1);
                        dDelC->FillSelTrueReaH2D(iselrea,icut,irea,_dDelC,_ndelayedClusters,1);
                    }
                }
            }
        }
        
        for(int irea=1;irea<NREACTION-1;irea++){
            if(isreaction[irea]){
                NDel->FillTrueReaH1D(irea,_ndelayedClusters,1);
                if(_dDelC!=99999){
                    dDelC->FillTrueReaH1D(irea,_dDelC,1);
                    dDelC->FillTrueReaH2D(irea,_dDelC,_ndelayedClusters,1);
                }
            }
        }
    }

    int iselrea=1;
    for(int irea=1;irea<NREACTION-1;irea++){
        NDel->PlotParH1D("Rec","NDel",iselrea,icut,irea);
        dDelC->PlotParH1D("Rec","dDelC",iselrea,icut,irea);
        dDelC->PlotParH2D("Rec","dDelC","Rec","NDel",iselrea,icut,irea);
    }
    double dDelCcut_ccqe=GetMichelElectronCut(dDelC,1,icut,1);
    double dDelCcut_ccnqe=GetMichelElectronCut(dDelC,1,icut,2);
    cout<<" MichelElectron Cut for sel ccqe true ccnqe  at cut level "<<icut<<" : "<<dDelCcut_ccnqe<<endl;

    dDelCcut=dDelCcut_ccnqe;
    dDelC->DeleteParH1D();
    dDelC->DeleteEffPurvsParH1D();
    delete dDelC;
    
}



double GetMichelElectronCut(TCCplotAnalysisUtils *plot, int iselrea,int icut, int irea){

    int nbin=plot->nbinX;
    int max=plot->maxX;
    int min=plot->minX;
    double stepwidth=(double) (max-min)/(double)nbin;
    plot->SetNStep(nbin);
    const int _nbin=nbin;
    double fom[_nbin];
    int ncuts=plot->GetNCuts();
    int nparticlecuts=plot->GetNParticleCuts();
    TFile *rootfile_output=plot->rootfile_output;
    TCCplotAnalysisUtils *graph;
    graph=new TCCplotAnalysisUtils(rootfile_output,nparticlecuts,ncuts);
    graph->SetNStep(nbin);
    graph->InitCutOptReaCounter();
    for(int istep=0;istep<nbin;istep++){
        fom[istep]=0;
        double value_sel=0;
        double value_seltrue=0;
        double value_true=0;
     
        for(int ibin=1;ibin<=istep+1;ibin++){
            value_sel+=plot->Par_Sel[iselrea][icut]->GetBinContent(ibin);
            value_seltrue+=plot->Par_SelTrue[iselrea][icut][irea]->GetBinContent(ibin);
        }
   
        for(int ibin=1;ibin<=nbin;ibin++){
            value_true+=plot->Par_SelTrue[iselrea][icut][irea]->GetBinContent(ibin);
        }
   
        double _fom=(double)plot->GetStatSignificance(value_seltrue,value_sel);
        fom[istep]=_fom;

       
        graph->FillSelCutOptReaCounter(istep,iselrea,icut,value_sel);
        graph->FillSelTrueCutOptReaCounter(istep,iselrea,icut,irea,value_seltrue);
        graph->FillTrueCutOptReaCounter(istep,irea,value_true);
    }

     double maxfom=-1;
     double maxstep=-1;
     for(int istep=0;istep<nbin;istep++){
         if(fom[istep]>maxfom){
             maxfom=fom[istep];
             maxstep=istep;
         }
     }
   
     graph->InitEffPurFOMvsReaCutOpt("dDelC");              
     graph->FillEffPurFOMvsReaCutOpt(iselrea,icut,irea,0,stepwidth);
     graph->PlotEffPurFOMvsReaCutOpt("dDelC",iselrea,icut,irea);
     delete graph;
     double probpos=stepwidth*maxstep; 
     cout<<" stepwidth "<<stepwidth<<" maxstep "<<maxstep<<endl;

    return probpos;
}









void InitTree(TTree *tree_Ev, int truestudy){
    tree_Ev->Branch("_NTRACKS",&_NTRACKS,"_NTRACKS/I");
    tree_Ev->Branch("_NREACTION",&_NREACTION,"_NREACTION/I");
    tree_Ev->Branch("_NSELREACTION",&_NSELREACTION,"_NSELREACTION/I");
    _NTRACKS=NTRACKS;
    _NREACTION=NREACTION;
    _NSELREACTION=NSELREACTION;
//***********************************
    tree_Ev->Branch("evid",&_evid,"evid/I");
    tree_Ev->Branch("weightMA",&_weightMA,"weightMA/F");
    tree_Ev->Branch("isRecVtxFV",&_isRecVtxFV,"isRecVtxFV/I");
    tree_Ev->Branch("hasMichel",&_hasMichel,"hasMichel/I");
    tree_Ev->Branch("ntracks",&_ntracks,"ntracks/I");
    tree_Ev->Branch("ntpctracks",&_ntpctracks,"ntpctracks/I");
    tree_Ev->Branch("nfgdtracks",&_nfgdtracks,"nfgdtracks/I");  
    tree_Ev->Branch("ndelayedClusters",&_ndelayedClusters,"ndelayedClusters/I");  
    tree_Ev->Branch("evSel",_evSel,"evSel[_NSELREACTION][9]/I");//9=NCUTS
    tree_Ev->Branch("RecVtx",_RecVtx,"RecVtx[3]/F");
    tree_Ev->Branch("RecPmu",&_RecPmu,"RecPmu/F");
    tree_Ev->Branch("RecEnu",_RecEnu,"RecEnu[_NSELREACTION]/F");
    tree_Ev->Branch("RecQ2",_RecQ2,"RecQ2[_NSELREACTION]/F");  
    tree_Ev->Branch("RecCosThmu",&_RecCosThmu,"RecCosThmu/F");
    tree_Ev->Branch("dThp",&_dThp,"dThp/F");
    tree_Ev->Branch("dEp",&_dEp,"dEp/F");
    tree_Ev->Branch("Mnu2",&_Mnu2,"Mnu2/F");  

    if(truestudy){   
        tree_Ev->Branch("evTrue",_evTrue,"evTrue[_NREACTION]/I");
        tree_Ev->Branch("evG4Code",&_evG4Code,"evG4Code/I");
        tree_Ev->Branch("TrueVtx",_TrueVtx,"TrueVtx[3]/F");
        tree_Ev->Branch("TrueMomprim",&_TrueMomprim,"TrueMomprim/F");
        tree_Ev->Branch("TrueEnu",&_TrueEnu,"TrueEnu/F");
        tree_Ev->Branch("TrueQ2",&_TrueQ2,"TrueQ2/F");  
        tree_Ev->Branch("TrueCosThprim",&_TrueCosThprim,"TrueCosThprim/F");
        tree_Ev->Branch("OUTleptonPDG",&_OUTleptonPDG,"OUTleptonPDG/F");
        tree_Ev->Branch("OUTnuPDG",&_OUTnuPDG,"OUTnuPDG/F");
        tree_Ev->Branch("OUTtrajPDG",_OUTtrajPDG,"OUTtrajPDG[_NTRACKS]/I");
        tree_Ev->Branch("OUTtrajMom",_OUTtrajMom,"OUTtrajMom[_NTRACKS]/F");
        tree_Ev->Branch("OUTtrajCosTh",_OUTtrajCosTh,"OUTtrajCosTh[_NTRACKS]/F");
    }
}   

void FillTree(TTree *tree_Ev,string GENE_typefile,double MA,string selcasePAR,string selcaseREA,int fgdstudy, int truestudy)
{
    
    TGeneAnalysisUtils *anUtils=new TGeneAnalysisUtils();
    TCCtruthAnalysisUtils *trueAn=new TCCtruthAnalysisUtils();
    TGENIEAnalysisUtils *genieUtils=new TGENIEAnalysisUtils();
    TNEUTAnalysisUtils *NeutUtils=new TNEUTAnalysisUtils();
    TCCweightMA *weightMA;
    if(GENE_typefile=="Genie")
        weightMA=new TCCweightMA(0.99);//Genie
    else{
        weightMA=new TCCweightMA(1.1);//Neut
    }
    TCCreconAnalysisUtils *recAn=new TCCreconAnalysisUtils();    
    recAn->InitTree(tree_Ev);
    recAn->SetCurvCut(curvcut);
    recAn->SetPullStat(pullmean, pullsigma);
    recAn->SetProbCut(probcut);
    recAn->SetPullCut(pullcut[0]);
    recAn->SetElectronCut(elcut);
    recAn->SetNParticleCuts(selcasePAR);
    recAn->SetNCuts(selcaseREA);
    int nparticlecuts=recAn->GetNParticleCuts(selcasePAR);
    int ncuts=recAn->GetNCuts(selcaseREA);
    int isreaction[NREACTION];
    
    TStopwatch timer;
    timer.Start();
    int entries = mcTruthVertices.GetEntries(); 
    //entries=600;
    for(int ient=0;ient<entries;ient++){
         if(ient%500 == 0)
        {
            std::cout <<  "********************************" << std::endl;
            std::cout <<  "    !!!Events Processed " << ient << std::endl;
            timer.Stop();
            std::cout << " Processing time: " << timer.CpuTime() << " " << timer.RealTime() << std::endl;
            std::cout <<  "********************************" << std::endl;
            timer.Start();
        }
            
        reconTracker.GetEntry(ient);
        reconGlobal.GetEntry(ient);

       
        
        //***************************
        //initialize the tree branches
        //***************************
        _evid=ient;
        _dEp=-9999;
        _dThp=-9;
        _Mnu2=-9999;
        _RecVtx[0]=-999;
        _RecVtx[1]=-999;
        _RecVtx[2]=-999;

        _RecCosThmu=9;//outgoing muon
        _RecPmu=-999;//outgoing muon
        _ntpctracks=-9;
        _nfgdtracks=-9;
        _ntracks=-9;
        _hasMichel=0;
        
        for(int iselrea=0;iselrea<NSELREACTION;iselrea++){
            _RecEnu[iselrea]=-999;
            _RecQ2[iselrea]=-999;
            for(int icut=0;icut<NCUTS;icut++){
                _evSel[iselrea][icut]=-9;
            }
        }
        //***********************************************************
        //TRUE STUDY
        //***********************************************************
        //initialize the tree true branch address
        //*****************************************
        for(int ip=0;ip<20;ip++){
            _OUTtrajPDG[ip]=0;
            _OUTtrajMom[ip]=0;
            _OUTtrajCosTh[ip]=-9;
        }
        _OUTleptonPDG=0;
        _OUTnuPDG=0;
        
        _weightMA=999;
        _evG4Code=9999;
        for(int irea=0;irea<NREACTION;irea++){
            _evTrue[irea]=-9;
        }
        _TrueCosThprim=9;//outgoing lepton or neutrino
        _TrueMomprim=-999;//outgoing lepton or neutrino
        _TrueEnu=-999;//incoming neutrino
        _TrueQ2=-999;
        _TrueVtx[0]=-999;
        _TrueVtx[1]=-999;
        _TrueVtx[2]=-999;
        //*****************************************
        int OUTleptonPDG=0;
        int OUTnuPDG=0;
        bool isfid=false;
        ND::TTruthVerticesModule::TTruthVertex *true_fgd_vertex;      
        if(truestudy){

            mcTruthTrajectories.GetEntry(ient);
            mcTruthVertices.GetEntry(ient);            
            bool isfgd1=false;
            if(NVtxFGD1 >0){
                true_fgd_vertex = (ND::TTruthVerticesModule::TTruthVertex*) VtxFGD1->UncheckedAt(0);
                isfgd1=true;
            }
            else if (NVtxFGD2 >0){
                true_fgd_vertex = (ND::TTruthVerticesModule::TTruthVertex*) VtxFGD2->UncheckedAt(0);
                isfgd1=false;
            }
            else{
                std::cerr << "There should be at least one vertex in one FGD.  Skipping event."<<std::endl;
                continue;
            }
            // for only fgd1 study
            if(fgdstudy==1){
                if(!isfgd1)
                    continue;
            }
            else if(fgdstudy==2){        // for only fgd2 study
                if(isfgd1)
                    continue;
            }
            
            //Fill the tree true branch address
            //*****************************************
            int ntraj=0;
            for (int tt=0;tt<NTRAJTYPES;tt++){
                for (int ii=0; ii< TruthTrajs[tt]->GetEntries(); ii++){                
                    ND::TTruthTrajectoriesModule::TTruthTrajectory* traj = (ND::TTruthTrajectoriesModule::TTruthTrajectory*)TruthTrajs[tt]->UncheckedAt(ii);
                    // Find the true muon momentum
                    int pdg=traj->TrajPDG;
                    TLorentzVector mom=traj->InitMom;
                    float cos=trueAn->GetTrueCosTheta(mom);//if mu don't exist=> cos=9
                    if(ntraj<20)
                    {
                        TVector3 mom3(mom.X(),mom.Y(),mom.Z());
                        _OUTtrajPDG[ntraj]=pdg;
                        _OUTtrajMom[ntraj]=mom3.Mag();
                        _OUTtrajCosTh[ntraj]=cos;
                    }
                    ntraj++;
                }
            }
            
            //      True kinematic of the different particles
            // *****************************************************************
            // (for CC)
            // ****************
          
            TLorentzVector trueLEP_momentum;
            int leptonSize=true_fgd_vertex->LeptonIDs.size();
            
            if(leptonSize>0){
                trueLEP_momentum=trueAn->GetChargedLeptonMaxMomentum(true_fgd_vertex,13);
                if(trueLEP_momentum.T()!=0){
                    OUTleptonPDG=13;
                }
                else{
                    trueLEP_momentum=trueAn->GetChargedLeptonMaxMomentum(true_fgd_vertex,11);
                    if(trueLEP_momentum.T()!=0){
                        OUTleptonPDG=11;
                    }else{
                        trueLEP_momentum=trueAn->GetChargedLeptonMaxMomentum(true_fgd_vertex,15);
                        if(trueLEP_momentum.T()!=0)
                            OUTleptonPDG=15;
                    }
                }
            }

            double trueLEP_mass=trueLEP_momentum.Mag();
            double trueLEP_e=trueLEP_momentum.T();
            double trueLEP_p=sqrt(trueLEP_e*trueLEP_e-trueLEP_mass*trueLEP_mass);
            double trueLEP_costh=trueAn->GetTrueCosTheta(trueLEP_momentum);//if mu don't exist=> cos=9
            _OUTleptonPDG=OUTleptonPDG;
             // (for NC)
            // ****************
          
            TLorentzVector trueNU_momentum;
            int neutralSize=true_fgd_vertex->OtherNeutralIDs.size();
         
            if(neutralSize>0){
                trueNU_momentum=trueAn->GetOtherNeutralMaxMomentum(true_fgd_vertex,14);
                if(trueNU_momentum.T()!=0){
                    OUTnuPDG=14;
                }
                else{
                    trueNU_momentum=trueAn->GetOtherNeutralMaxMomentum(true_fgd_vertex,12);
                    if(trueNU_momentum.T()!=0){
                        OUTnuPDG=12;
                    }else{
                        trueNU_momentum=trueAn->GetOtherNeutralMaxMomentum(true_fgd_vertex,16);
                        if(trueNU_momentum.T()!=0)
                            OUTnuPDG=16;
                    }
                }
            }
            double trueNU_mass=trueNU_momentum.Mag();
            double trueNU_e=trueNU_momentum.T();
            double trueNU_p=sqrt(trueNU_e*trueNU_e-trueNU_mass*trueNU_mass);
            double trueNU_costh=trueAn->GetTrueCosTheta(trueNU_momentum);//if mu don't exist=> cos=9
            _OUTnuPDG=OUTnuPDG;

            // Provide the reweight factor depending on MA
            // *******************************************
            // NU_MU
            // ********************
          
            TLorentzVector neutrino_momentum = true_fgd_vertex->MomNu;
            double enu=neutrino_momentum.T();
         
            // NUCLEON_TARGET (p or n)
            // ***********************
          
            TLorentzVector nucleon_target_momentum =true_fgd_vertex->MomTarget;
            TVector3  nucleon_target_mom=TVector3(nucleon_target_momentum.X(),nucleon_target_momentum.Y(),nucleon_target_momentum.Z());


            // Q2 (MeV^2)
            // = (Pmu-Pnu)^2     for CC
            // = (PnuOUT-Pnu)^2  for NC
            // ************************
            double q2=-999;
            if(leptonSize>0){
                q2=trueAn->GetQ2(neutrino_momentum,trueLEP_momentum);
            }
            else{
                q2=trueAn->GetQ2(neutrino_momentum,trueNU_momentum);
            }
            double enuGeV=enu/1000;
            double q2GeV=q2/1000000;
            double weight=1;
            if(fgdstudy==2)weight=weightMA->Weight(enuGeV, q2GeV, MA,2);
            else weight=weightMA->Weight(enuGeV, q2GeV, MA,0);
            _weightMA=weight;
            
            //Provide the mode of the reaction:
            //*************************************
            int rxn_code=-999;
            if(GENE_typefile=="Genie"){
                rxn_code=trueAn->GetNeutCodeForGenie(true_fgd_vertex);
                if(rxn_code==0)
                    if(genieUtils->IsELNC(true_fgd_vertex)) rxn_code=50;
            }
            else
                rxn_code=NeutUtils->GetReactionCode(true_fgd_vertex);
            _evG4Code=rxn_code;  

          
             for(int irea=0;irea<NREACTION;irea++){
                 isreaction[irea]=0;
                 isreaction[irea]=(int)trueAn->IsReaction((int)rxn_code,irea);
             }
             //true parameters:
             //****************
             double true_p=-999;
             double true_costh=9;
             if(isreaction[0]){
                 true_p=trueLEP_p;
                 true_costh=trueLEP_costh;
             }
             else if(isreaction[4]){
                 true_p=trueNU_p;
                 true_costh=trueNU_costh;
             }
             _TrueMomprim=true_p;
             _TrueCosThprim=true_costh;
             _TrueEnu=enu;
             _TrueQ2=q2;

              TLorentzVector TrueVtx = true_fgd_vertex->Vertex;
             _TrueVtx[0]=TrueVtx.X();
             _TrueVtx[1]=TrueVtx.Y();
             _TrueVtx[2]=TrueVtx.Z();

             //bugs
             if(weight<0 ||
                (isreaction[0] && trueLEP_p==0) ||
                (isreaction[4] && trueNU_p==0))
                 continue;

             TVector3 vtxtrue(TrueVtx.X(),TrueVtx.Y(),TrueVtx.Z());
             cout<<TrueVtx.Z()<<endl;
             anUtils->DefineFiducialVolume(fvcut);
             isfid=anUtils->IsInFiducialVolume(vtxtrue);       
       
        }//truestudy

//accept only when the following is NOT happening
        //true vtx in FV and ... (for reco only event )
        if((isfid && truestudy) || !truestudy){
             
        
        //Do reconstruction
        //********************
        //DO TRACK ANALYSIS
        //***************************
        //At least 1 TPC tracks
        //if no tpc track return false;
        //Check for useless tracks
        //Fill all track reco info (mom, ch, pulls, ...)
        //Fill all track true info (mom, ch, pulls, ...) -  if required
        //Set track prob
        //Make the selection
        //Fill true counters - if required
        //***************************
          //   for( int j=0; j<NPIDs; j++) {
//                 ND::TGlobalReconModule::TGlobalPID *globalTrack= (ND::TGlobalReconModule::TGlobalPID*)(*PIDs)[j];

//                 cout<<" KKKKKKKKKK "<<  globalTrack->TrueParticle.PDG<<endl;
//             }
        recAn->ResetCutFlag();
        cout<<"********************************"<<endl;
        recAn->SetRecVertex(PIDs, NPIDs, Vertices, NVertices,ient);
        recAn->SetFiducialVolumeCut(fvcut); // set fvcut and look if rec is in FV
        recAn->TracksAnalysis(PIDs,selcasePAR,selcaseREA,truestudy);//At least one tpc track

        cout<<" ient "<<ient<<" nvtx "<<NVertices
//        if(recAn->fTotalTPC>0)
            <<"\n ntpc "<<recAn->fTotalTPC
            <<"\n pass vtx "<<recAn->fPassedVertexCut<<" pos.z "<<recAn->fReconVertexPos.Z()
            <<"\n # track with recmom/mom>3 "<<recAn->fNumberTpcTr_Sel[0][2]
            <<"\n # neg track "<<recAn->fNumberTpcTr_Sel[0][4]
            <<"\n # muon "<< recAn->fNumberTpcTr_Sel[0][nparticlecuts-1]
            <<endl;
        
        //BUG: more than 1 true muon (=same traj. matched to 2 diff. track)
        if(truestudy){
            if(recAn->fNumberTpcTr_True[0]>1 ){
                cout<<"!!!!!!! ient :"<<ient<<"  number of muminus : "<<recAn->fNumberTpcTr_True[0]
                    <<" useless tracks "<<recAn->fnevent_withuselesstracks
                    <<endl;
                // continue;
            }          
        }
        
        //DO PROCESS ANALYSIS
        //*********************************************3
        _ndelayedClusters=NDelayedClusters;
        if(NDelayedClusters>0)
            recAn->HasMichelElectron(DelayedClusters, NDelayedClusters,dDelCcut);
      

        //Fill the tree branches
        //***************************
        if(truestudy)
            recAn->FillTree_trueBR();
        recAn->FillTree_recBR();
        recAn->FillTree_selBR();
        TLorentzVector recVTX= recAn->GetRecVertex();
        TVector3 vtxrec(recVTX.X(),recVTX.Y(),recVTX.Z());
        _RecVtx[0]=vtxrec.X();
        _RecVtx[1]=vtxrec.Y();
        _RecVtx[2]=vtxrec.Z();
        _dThp=recAn->GetDeltaThetaP();
        _dEp=recAn->GetDeltaEnergyP();
        _Mnu2=recAn->GetMnuSquare();
        bool highDeltaThetap=false;
        highDeltaThetap=recAn->HighDeltaThetaP(_dThp,dthpcut);
        bool highMnuSquare=false;
        highMnuSquare=recAn->HighMnuSquare(_Mnu2,mnu2cut);
        if(recAn->fFoundMichelCluster)
            _hasMichel=1;
            
        int kTotalTPC=recAn->fTotalTPC;
        int kTotalFGD=recAn->fTotalTPCFGD-kTotalTPC;
        _ntpctracks=kTotalTPC;//only usefull tpc tracks
        _nfgdtracks=kTotalFGD;
        _ntracks=NPIDs; //here take all the tracks also the tracks that are useless
        double pmurec=recAn->GetRecMuonMomentumMag();       
        double costhmurec=recAn->GetRecMuonCosTheta();
        _RecCosThmu=costhmurec;
        _RecPmu=pmurec;

        double enurec[NSELREACTION];
        double q2rec[NSELREACTION];
        TLorentzVector muon_momentumRec=recAn->GetRecMuonMomentum();
        for(int iselrea=0;iselrea<NSELREACTION;iselrea++){
            enurec[iselrea]= recAn->GetRecNeutrinoEnergy(iselrea);
            TLorentzVector neutrino_momentumRec=TLorentzVector(0,0,enurec[iselrea],enurec[iselrea]);
            q2rec[iselrea]=trueAn->GetQ2(neutrino_momentumRec,muon_momentumRec);
            _RecEnu[iselrea]=enurec[iselrea];
            _RecQ2[iselrea]=q2rec[iselrea];
        }


        //STUDY ONLY TRUE NUMU INTERACTION
        if(truestudy){
            if((OUTleptonPDG!=13 && isreaction[0]) ||
               (OUTnuPDG!=14 && isreaction[4]) )continue;
        }

        if(truestudy){
            for(int irea=0;irea<NREACTION;irea++){
                //TRUE
                //*******************************
                if(isreaction[irea]){
                    _evTrue[irea]=irea;
                }
            }
        }
        int ncuts=recAn->GetNCuts(selcaseREA);
        for(int iselrea=0;iselrea<NSELREACTION;iselrea++){
         
            for(int icut=0;icut<ncuts;icut++){
                bool iscut=false;
                if(selcaseREA=="default")
                    iscut=recAn->IsSelReaction(iselrea,icut);
                else if(selcaseREA=="simple")
                    iscut=recAn->IsSelReaction_Simple(iselrea,icut);
                else if(selcaseREA=="SKtopo")
                    iscut=recAn->IsSelReactionSKtopo(iselrea,icut);

                // look for the selected event (S+B)
                //********************************
                if(iscut){
                    _evSel[iselrea][icut]=iselrea;
                }
            }
        }//end loop iselrea
        }
    
        tree_Ev->Fill();


    }
    
    delete anUtils;
    delete trueAn;
    delete genieUtils;
    delete NeutUtils;
    delete weightMA;
    delete recAn;

}


   



void  InitParticlePlots(TFile *rootfile_output,string selcasePAR){
    //HISTOGRAMS FOR TRACK ANALYSIS:
     
    
    sprintf(pidParName[0],"Prob");
    sprintf(pidParName[1],"LikeLHD");
    sprintf(pidParName[2],"Pull");
    sprintf(pidParName[3],"Sigma");
    sprintf(pidParName[4],"dEdxexp");
  
    sprintf(kinParName[0],"dEdxmeas");
    sprintf(kinParName[1],"NNodes");
    sprintf(kinParName[2],"MomErr");
    sprintf(kinParName[3],"SigMom");
    sprintf(kinParName[4],"Mom");
    sprintf(kinParName[5],"CosTh");
    sprintf(kinParName[6],"Charge");
    sprintf(kinParName[7],"ProbMU");
    sprintf(kinParName[8],"ProbP");
    sprintf(kinParName[9],"ProbPI");
    sprintf(kinParName[10],"ProbE");

    TCCreconAnalysisUtils *recAn=new TCCreconAnalysisUtils();
    int nparticlecuts=recAn->GetNParticleCuts(selcasePAR);
    delete recAn;
    
    for(int ipar=0;ipar<5;ipar++){
       particleHypH1D[ipar]=new TCCplotAnalysisUtils(rootfile_output,nparticlecuts,0);
       particleHypH1D[ipar]->InitParParticleHypH1D("Rec",pidParName[ipar]);
       for(int ik=0;ik<2;ik++)
           for(int is=0;is<2;is++)
               particleHypH1DpBin[ipar][ik][is]=new TCCplotAnalysisUtils(rootfile_output,nparticlecuts,0);

       particleHypH1DpBin[ipar][0][0]->InitParpParParticleHypH1D("Rec",pidParName[ipar],"Rec","Mom");
       particleHypH1DpBin[ipar][1][0]->InitParpParParticleHypH1D("Rec",pidParName[ipar],"Rec","Charge");
       particleHypH1DpBin[ipar][0][1]->InitParpParParticleHypH1D("Rec",pidParName[ipar],"True","Mom");
       particleHypH1DpBin[ipar][1][1]->InitParpParParticleHypH1D("Rec",pidParName[ipar],"True","Charge");
       
       for(int ipar2=0;ipar2<6;ipar2++){
           particleHypH2D_RecVsRec[ipar*6+ipar2]=new TCCplotAnalysisUtils(rootfile_output,nparticlecuts,0);
           particleHypH2D_RecVsRec[ipar*6+ipar2]->InitParParticleHypH2D("Rec",kinParName[ipar2],"Rec",pidParName[ipar]);
       }
       for(int ipar2=4;ipar2<6;ipar2++){
           particleHypH2D_RecVsTrue[ipar*2+ipar2-4]=new TCCplotAnalysisUtils(rootfile_output,nparticlecuts,0);
           particleHypH2D_RecVsTrue[ipar*2+ipar2-4]->InitParParticleHypH2D("True",kinParName[ipar2],"Rec",pidParName[ipar]);
       }
    }

    int count=0;
    int count2=0;
    for(int ipar=0;ipar<11;ipar++){
        particleKinH1D_Rec[ipar]=new TCCplotAnalysisUtils(rootfile_output,nparticlecuts,0);
        particleKinH1D_Rec[ipar]->InitParParticleH1D("Rec",kinParName[ipar]);
        particleKinH1D_Rec[ipar]->InitEffPurvsParParticleH1D("Rec",kinParName[ipar]);
        particleKinH1D_Rec[ipar]->InitParParticleTOTH1D("Rec",kinParName[ipar]);
        particleKinH1D_Rec[ipar]->InitEffPurvsParParticleTOTH1D("Rec",kinParName[ipar]);
        
        for(int ipar2=ipar+1;ipar2<11;ipar2++){
            particleKinH2D_RecVsRec[count]=new TCCplotAnalysisUtils(rootfile_output,nparticlecuts,0);
            particleKinH2D_RecVsRec[count]->InitParParticleH2D("Rec",kinParName[ipar2],"Rec",kinParName[ipar]);
            particleKinH2D_RecVsRec[count]->InitEffPurvsParParticleH2D("Rec",kinParName[ipar2],"Rec",kinParName[ipar]);
            count++;
        }
        for(int ipar2=4;ipar2<6;ipar2++){
            particleKinH2D_RecVsTrue[count2]=new TCCplotAnalysisUtils(rootfile_output,nparticlecuts,0);
            particleKinH2D_RecVsTrue[count2]->InitParParticleH2D("True",kinParName[ipar2],"Rec",kinParName[ipar]);
            particleKinH2D_RecVsTrue[count2]->InitEffPurvsParParticleH2D("True",kinParName[ipar2],"Rec",kinParName[ipar]);
            count2++;
        }
    }

    count2=0;
    for(int ipar=4;ipar<7;ipar++){
        particleKinH1D_True[ipar-4]=new TCCplotAnalysisUtils(rootfile_output,nparticlecuts,0);
        particleKinH1D_True[ipar-4]->InitParParticleH1D("True",kinParName[ipar]);
        particleKinH1D_True[ipar-4]->InitEffPurvsParParticleH1D("True",kinParName[ipar]);
        for(int ipar2=ipar+1;ipar2<7;ipar2++){
            particleKinH2D_TrueVsTrue[count2]=new TCCplotAnalysisUtils(rootfile_output,nparticlecuts,0);
            particleKinH2D_TrueVsTrue[count2]->InitParParticleH2D("True",kinParName[ipar2],"True",kinParName[ipar]);
            particleKinH2D_TrueVsTrue[count2]->InitEffPurvsParParticleH2D("True",kinParName[ipar2],"True",kinParName[ipar]);
            count2++;
        }
    }
    


    for(int ipar=0;ipar<4;ipar++){
        pCounter[ipar]=new TCCplotAnalysisUtils(rootfile_output,nparticlecuts,0);
        pCounter[ipar]->InitParticleCounter();
        pCounter[ipar]->InitEffPurFOMvsParticleCut();
    }

    cout<<" PLOT INITIALIZED "<<endl;
    
}



void FillParticlePlots(TFile *rootfile_output,string selcasePAR,int truestudy){
 
    TCCreconAnalysisUtils *recAn=new TCCreconAnalysisUtils();
    recAn->SetNParticleCuts(selcasePAR);
    TTree *tree_Ev=(TTree*) rootfile_output->Get("tree_process");
    tree_Ev->SetBranchAddress("_NTRACKS",&_NTRACKS);
    tree_Ev->SetBranchAddress("_NDELAYEDCLUS",&_NDELAYEDCLUS);
    tree_Ev->SetBranchAddress("_NREACTION",&_NREACTION);
    tree_Ev->SetBranchAddress("_NSELREACTION",&_NSELREACTION);
  
    tree_Ev->SetBranchAddress("evid",&_evid);
    tree_Ev->SetBranchAddress("weightMA",&_weightMA);
    tree_Ev->SetBranchAddress("isRecVtxFV",&_isRecVtxFV);
    tree_Ev->SetBranchAddress("hasMichel",&_hasMichel);
    tree_Ev->SetBranchAddress("ntracks",&_ntracks);
    tree_Ev->SetBranchAddress("ntpctracks",&_ntpctracks);
    tree_Ev->SetBranchAddress("nfgdtracks",&_nfgdtracks);
    tree_Ev->SetBranchAddress("evSel",_evSel);
    tree_Ev->SetBranchAddress("RecVtx",_RecVtx);
    tree_Ev->SetBranchAddress("RecPmu",&_RecPmu);
    tree_Ev->SetBranchAddress("RecEnu",_RecEnu);
    tree_Ev->SetBranchAddress("RecQ2",_RecQ2);
    tree_Ev->SetBranchAddress("RecCosThmu",&_RecCosThmu);
    
    if(truestudy){   
        tree_Ev->SetBranchAddress("evTrue",_evTrue);
        tree_Ev->SetBranchAddress("evG4Code",&_evG4Code);
        tree_Ev->SetBranchAddress("TrueVtx",_TrueVtx);
        tree_Ev->SetBranchAddress("TrueMomprim",&_TrueMomprim);
        tree_Ev->SetBranchAddress("TrueEnu",&_TrueEnu);
        tree_Ev->SetBranchAddress("TrueQ2",&_TrueQ2);
        tree_Ev->SetBranchAddress("TrueCosThprim",&_TrueCosThprim);
        tree_Ev->SetBranchAddress("OUTleptonPDG",&_OUTleptonPDG);
        tree_Ev->SetBranchAddress("OUTnuPDG",&_OUTnuPDG);
        tree_Ev->SetBranchAddress("OUTtrajPDG",_OUTtrajPDG);
        tree_Ev->SetBranchAddress("OUTtrajMom",_OUTtrajMom);
        tree_Ev->SetBranchAddress("OUTtrajCosTh",_OUTtrajCosTh);
    }
    
    
    tree_Ev->SetBranchAddress("isUsefullTrack",_isUsefullTrack);
    tree_Ev->SetBranchAddress("trid",_trid);
    tree_Ev->SetBranchAddress("trSel",_trSel);
    tree_Ev->SetBranchAddress("trQuality",_RecMom);
    tree_Ev->SetBranchAddress("RecMom",_RecMom);
    tree_Ev->SetBranchAddress("RecCosTh",_RecCosTh);
    tree_Ev->SetBranchAddress("RecCharge",_RecCharge);
    tree_Ev->SetBranchAddress("BackWards",_BackWards);
    tree_Ev->SetBranchAddress("SigMom",_SigMom);
    tree_Ev->SetBranchAddress("MomErr",_MomErr);
    tree_Ev->SetBranchAddress("NNodes",_NNodes);
    tree_Ev->SetBranchAddress("dEdxmeas",_dEdxmeas);
    tree_Ev->SetBranchAddress("Pull",_Pull);
    tree_Ev->SetBranchAddress("Sigma",_Sigma);
    tree_Ev->SetBranchAddress("dEdxexp",_dEdxexp);
    tree_Ev->SetBranchAddress("LikeLHD",_LikeLHD);
    tree_Ev->SetBranchAddress("Prob",_Prob);
    tree_Ev->SetBranchAddress("trDir",_trDir);

    
    if(truestudy){   
        tree_Ev->SetBranchAddress("TrueMom",_TrueMom);
        tree_Ev->SetBranchAddress("TrueCosTh",_TrueCosTh);
        tree_Ev->SetBranchAddress("TrueCharge",_TrueCharge);
        tree_Ev->SetBranchAddress("trTrue",_trTrue);
        tree_Ev->SetBranchAddress("trG4Code",_trG4Code);
    }

    
    int entries=tree_Ev->GetEntries();
    cout<<" nb of entries "<<entries<<endl;
 
    TStopwatch timer;
    timer.Start();
    //   //entries=600;
    for(int ient=0;ient<entries;ient++){
        if(ient%500 == 0)
        {
            std::cout <<  "********************************" << std::endl;
            std::cout <<  "    !!!Events Processed " << ient << std::endl;
            timer.Stop();
            std::cout << " Processing time: " << timer.CpuTime() << " " << timer.RealTime() << std::endl;
            std::cout <<  "********************************" << std::endl;
            timer.Start();
        }
        tree_Ev->GetEntry(ient);
        int tottracks=0;
        if(_ntracks<20)tottracks=_ntracks;
        else  tottracks=20;
        recAn->ResetCutFlag();
        recAn->SetNTracks(_ntracks);
        recAn->FillVectors(tottracks);
        recAn->SetReactionMode(_evG4Code);
        for(int itr=0;itr<tottracks;itr++){
            recAn->SetisUsefullTrack(itr,_isUsefullTrack[itr]);
            for(int iparcut=0;iparcut<NPARTICLECUT;iparcut++)
                for(int iselpar=0;iselpar<NPARTICLESEL;iselpar++)
                    recAn->SetTrackSelInfo(itr,_trSel[itr][iselpar][iparcut],iparcut);
            if(truestudy){
                recAn->SetTrackPDG(itr,_trG4Code[itr]);
                recAn->SetTrackTrueMom(itr,_TrueMom[itr]);
            }
        }
        recAn->FillCounter(pCounter,(bool)truestudy);

        //FILL HYP
        for(int itr=0;itr<tottracks;itr++){
            TVector3 dir(_trDir[itr][0],_trDir[itr][1],_trDir[itr][2]);            
            bool pass=_isUsefullTrack[itr];
            if(pass){
                double SigMom=_MomErr[itr]*_RecMom[itr]*_RecCharge[itr];
                double RecCosTh=recAn->GetRecCosTheta(dir);
                double paramkin[8]={_dEdxmeas[itr],_NNodes[itr],_MomErr[itr],SigMom,_RecMom[itr],RecCosTh,_TrueMom[itr],_TrueCosTh[itr]};
                for(int ihyp=0;ihyp<NPARTICLEHYP;ihyp++){
                    double paramhyp[5]={_Prob[itr][ihyp],_LikeLHD[itr][ihyp],_Pull[itr][ihyp],_Sigma[itr][ihyp],_dEdxexp[itr][ihyp]};
                    for(int ii=0;ii<5;ii++){
                        recAn->FillHypH1D(particleHypH1D[ii],itr,ihyp,(double)paramhyp[ii],(bool)truestudy);
                        recAn->FillHypH1DpBin(particleHypH1DpBin[ii][0][0],itr,ihyp,(double) paramhyp[ii],(double) _RecMom[itr],(bool)truestudy);
                        recAn->FillHypH1DpBin(particleHypH1DpBin[ii][0][1],itr,ihyp,(double) paramhyp[ii],(double) _RecCharge[itr],(bool)truestudy);
                        if(truestudy){
                            recAn->FillHypH1DpBin(particleHypH1DpBin[ii][1][0],itr,ihyp,(double) paramhyp[ii],(double) _TrueMom[itr],(bool)truestudy);
                            recAn->FillHypH1DpBin(particleHypH1DpBin[ii][1][1],itr,ihyp,(double) paramhyp[ii],(double) _TrueCharge[itr],(bool)truestudy);
                        }
                        for(int ipar2=0;ipar2<6;ipar2++){
                            recAn->FillHypH2D(particleHypH2D_RecVsRec[ii*6+ipar2],itr,ihyp,(double)paramhyp[ii],(double)paramkin[ipar2],(bool)truestudy);
                        }
                        if(truestudy){
                            for(int ipar2=6;ipar2<8;ipar2++){
                                recAn->FillHypH2D(particleHypH2D_RecVsTrue[ii*2+ipar2-6],itr,ihyp,(double)paramhyp[ii],(double)paramkin[ipar2],(bool)truestudy);
                            }
                        }
                    }
                }
        
                //FILL HISTOGRAMS FOR THE KINETIC PART AND COUNTER
                //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                if(truestudy){
                    int count=0;
                    int count2=0;
                    double param[11]={_dEdxmeas[itr],_NNodes[itr],_MomErr[itr],SigMom,_RecMom[itr],RecCosTh, _Prob[itr][0],_Prob[itr][1],_Prob[itr][2],_Prob[itr][3]};
                    double trueparam[2]={_TrueMom[itr],_TrueCosTh[itr]};                
                    for(int ipar=0;ipar<11;ipar++){
                        
                        recAn->FillKinH1D(particleKinH1D_Rec[ipar],itr,param[ipar],(bool)truestudy);
                        
                        for(int ipar2=ipar+1;ipar2<11;ipar2++){
                            recAn->FillKinH2D(particleKinH2D_RecVsRec[count],itr,param[ipar],param[ipar2],(bool)truestudy);
                            count++;
                        }
                        for(int ipar2=0;ipar2<2;ipar2++){
                            recAn->FillKinH2D(particleKinH2D_RecVsTrue[count2],itr,param[ipar],trueparam[ipar2],(bool)truestudy);
                            count2++;
                        }
                    }
                }
            }
        }
        recAn->ClearVectors();
    }
    delete recAn;
 
}

void WriteParticlePlots(string selcasePAR){
    gStyle->SetPalette(1);
    TCCreconAnalysisUtils *recAn=new TCCreconAnalysisUtils();
    int nparticlecuts=recAn->GetNParticleCuts(selcasePAR);
    delete recAn;
    //Plot histograms for the particle selection
    //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
    for(int ipar=0;ipar<5;ipar++){
        particleHypH1D[ipar]->WriteParParticleHypH1D("Rec",pidParName[ipar]);
        particleHypH1DpBin[ipar][0][0]->WriteParpParParticleHypH1D("Rec",pidParName[ipar],"Rec","Mom");
        particleHypH1DpBin[ipar][1][0]->WriteParpParParticleHypH1D("Rec",pidParName[ipar],"Rec","Charge");
        particleHypH1DpBin[ipar][0][1]->WriteParpParParticleHypH1D("Rec",pidParName[ipar],"True","Mom");
        particleHypH1DpBin[ipar][1][1]->WriteParpParParticleHypH1D("Rec",pidParName[ipar],"True","Charge");
        for(int ipar2=0;ipar2<6;ipar2++){
            particleHypH2D_RecVsRec[ipar*6+ipar2]->WriteParParticleHypH2D("Rec",kinParName[ipar2],"Rec",pidParName[ipar]);
        }
         for(int ipar2=4;ipar2<6;ipar2++){
             particleHypH2D_RecVsTrue[ipar*2+ipar2-4]->WriteParParticleHypH2D("True",kinParName[ipar2],"Rec",pidParName[ipar]);
         }
    }
 

    int count=0;
    int count2=0;
    for(int ipar=0;ipar<11;ipar++){
        particleKinH1D_Rec[ipar]->WriteParParticleH1D("Rec",kinParName[ipar]);
        particleKinH1D_Rec[ipar]->FillEffPurvsParParticleH1D();
        particleKinH1D_Rec[ipar]->WriteEffPurvsParParticleH1D("Rec",kinParName[ipar]);
        particleKinH1D_Rec[ipar]->WriteParParticleTOTH1D("Rec",kinParName[ipar]);
        particleKinH1D_Rec[ipar]->FillEffPurvsParParticleTOTH1D();
        particleKinH1D_Rec[ipar]->WriteEffPurvsParParticleTOTH1D("Rec",kinParName[ipar]);
        for(int ipar2=ipar+1;ipar2<11;ipar2++){
            particleKinH2D_RecVsRec[count]->WriteParParticleH2D("Rec",kinParName[ipar2],"Rec",kinParName[ipar]);
            particleKinH2D_RecVsRec[count]->FillEffPurvsParParticleH2D();
            particleKinH2D_RecVsRec[count]->WriteEffPurvsParParticleH2D("Rec",kinParName[ipar2],"Rec",kinParName[ipar]);
            count++;
        }
        for(int ipar2=4;ipar2<6;ipar2++){
            particleKinH2D_RecVsTrue[count2]->WriteParParticleH2D("True",kinParName[ipar2],"Rec",kinParName[ipar]);
            particleKinH2D_RecVsTrue[count2]->FillEffPurvsParParticleH2D();
            particleKinH2D_RecVsTrue[count2]->WriteEffPurvsParParticleH2D("True",kinParName[ipar2],"Rec",kinParName[ipar]);
            count2++;
        }
    }
 
    count2=0;
    for(int ipar=4;ipar<7;ipar++){
        particleKinH1D_True[ipar-4]->WriteParParticleH1D("True",kinParName[ipar]);
        particleKinH1D_True[ipar-4]->FillEffPurvsParParticleH1D();
        particleKinH1D_True[ipar-4]->WriteEffPurvsParParticleH1D("True",kinParName[ipar]);
        for(int ipar2=ipar+1;ipar2<7;ipar2++){
            particleKinH2D_TrueVsTrue[count2]->WriteParParticleH2D("True",kinParName[ipar2],"True",kinParName[ipar]);
            particleKinH2D_TrueVsTrue[count2]->FillEffPurvsParParticleH2D();
            particleKinH2D_TrueVsTrue[count2]->WriteEffPurvsParParticleH2D("True",kinParName[ipar2],"True",kinParName[ipar]);
            count2++;
        }
    }


    cout<<"Counter histograms on particle writing .... "<<endl;
    pCounter[0]->InitEffPurFOMvsParticleCut();
    pCounter[0]->FillEffPurFOMvsParticleCut();
    pCounter[0]->PlotEffPurFOMvsParticleCut();
    pCounter[0]->FillEffPurFOMvsParticleCutTOT();
    pCounter[0]->PlotEffPurFOMvsParticleCutTOT();
    
    pCounter[0]->InitChargeEffPurFOMvsParticleCut();
    pCounter[0]->FillChargeEffPurFOMvsParticleCut();
    pCounter[0]->PlotChargeEffPurFOMvsParticleCut();
    pCounter[0]->FillChargeEffPurFOMvsParticleCutTOT();
    pCounter[0]->PlotChargeEffPurFOMvsParticleCutTOT();

    cout<<"\n********* Part. id. stats ***************"<<endl;
    pCounter[0]-> SetNParticleCuts(nparticlecuts);
    pCounter[0]->PrintParticleEffMatrix(nparticlecuts-1);
    pCounter[0]->PrintParticlePurMatrix(nparticlecuts-1);
    pCounter[0]->PrintParticleSelTrueMatrix(nparticlecuts-1);
    for(int isel=0;isel<NPARTICLESEL;isel++){
        pCounter[0]->PrintStatvsParticleCut(isel,isel);
    }
    

}
void DeleteParticlePlots(){

    cout<<" Deleting all histograms plots on tracks from memory ... 1 "<<endl;
 //DELETE histograms for the particle selection
    //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
    for(int ipar=0;ipar<5;ipar++){
        particleHypH1D[ipar]->DeleteParParticleHypH1D();
        particleHypH1DpBin[ipar][0][0]->DeleteParpParParticleHypH1D();
        particleHypH1DpBin[ipar][1][0]->DeleteParpParParticleHypH1D();
        particleHypH1DpBin[ipar][0][1]->DeleteParpParParticleHypH1D();
        particleHypH1DpBin[ipar][1][1]->DeleteParpParParticleHypH1D();
        for(int ipar2=0;ipar2<6;ipar2++){
            particleHypH2D_RecVsRec[ipar*6+ipar2]->DeleteParParticleHypH2D();
        }
         for(int ipar2=4;ipar2<6;ipar2++){
             particleHypH2D_RecVsTrue[ipar*2+ipar2-4]->DeleteParParticleHypH2D();
         }
    }
 
    cout<<" Deleting all histograms plots on tracks from memory ... 2 "<<endl;
    int count=0;
    int count2=0;
    for(int ipar=0;ipar<11;ipar++){
        particleKinH1D_Rec[ipar]->DeleteParParticleH1D();
        particleKinH1D_Rec[ipar]->DeleteEffPurvsParParticleH1D();
        particleKinH1D_Rec[ipar]->DeleteParParticleTOTH1D();
        particleKinH1D_Rec[ipar]->DeleteEffPurvsParParticleTOTH1D();
        for(int ipar2=ipar+1;ipar2<11;ipar2++){
            particleKinH2D_RecVsRec[count]->DeleteParParticleH2D();
            particleKinH2D_RecVsRec[count]->DeleteEffPurvsParParticleH2D();
            count++;
        }
        for(int ipar2=4;ipar2<6;ipar2++){
            particleKinH2D_RecVsTrue[count2]->DeleteParParticleH2D();
            particleKinH2D_RecVsTrue[count2]->DeleteEffPurvsParParticleH2D();
            count2++;
        }
    }
     cout<<" Deleting all histograms plots on tracks from memory ... 3 "<<endl;
    count2=0;
    for(int ipar=4;ipar<7;ipar++){
        particleKinH1D_True[ipar-4]->DeleteParParticleH1D();
        particleKinH1D_True[ipar-4]->DeleteEffPurvsParParticleH1D();
        for(int ipar2=ipar+1;ipar2<7;ipar2++){
            particleKinH2D_TrueVsTrue[count2]->DeleteParParticleH2D();
            particleKinH2D_TrueVsTrue[count2]->DeleteEffPurvsParParticleH2D();
            count2++;
        }
    }
   

    cout<<" Deleting all histograms plots on tracks from memory ... 4 "<<endl;
    for(int ipar=0;ipar<5;ipar++){
       delete particleHypH1D[ipar];
       for(int ik=0;ik<2;ik++)
           for(int is=0;is<2;is++)
              delete particleHypH1DpBin[ipar][ik][is];
       for(int ipar2=0;ipar2<6;ipar2++){
          delete particleHypH2D_RecVsRec[ipar*6+ipar2];
       }
       for(int ipar2=4;ipar2<6;ipar2++){
          delete particleHypH2D_RecVsTrue[ipar*2+ipar2-4];
       }
    }
    count=0;
    count2=0;
    for(int ipar=0;ipar<11;ipar++){
        delete particleKinH1D_Rec[ipar];
         for(int ipar2=ipar+1;ipar2<11;ipar2++){
           delete  particleKinH2D_RecVsRec[count];
         }
        for(int ipar2=4;ipar2<6;ipar2++){
            delete particleKinH2D_RecVsTrue[count2];
        }
    }
    count2=0;
    for(int ipar=4;ipar<7;ipar++){
       delete particleKinH1D_True[ipar-4];
        for(int ipar2=ipar+1;ipar2<7;ipar2++){
            delete particleKinH2D_TrueVsTrue[count2];
        }
    }
    for(int ipar=0;ipar<4;ipar++){
       delete pCounter[ipar];
    }
        cout<<" Deleting all histograms plots on tracks from memory ... 5 "<<endl;
    return;
    
    
}  

void  InitReactionPlots(TFile *rootfile_output,string selcasePAR,string selcaseREA){
    //HISTOGRAMS FOR TRACK ANALYSIS:

    sprintf(paramName[0],"Q2");
    sprintf(paramName[1],"Enu");
    sprintf(paramName[2],"CosThmu");
    sprintf(paramName[3],"Pmu");
    sprintf(paramName[4],"VtxZ");
    sprintf(paramName[5],"VtxX");
    sprintf(paramName[6],"VtxY");

    sprintf(paramState[0],"True");
    sprintf(paramState[1],"Rec"); 

    TCCreconAnalysisUtils *recAn=new TCCreconAnalysisUtils();
    int ncuts=recAn->GetNCuts(selcaseREA);
    int nparticlecuts=recAn->GetNParticleCuts(selcasePAR);
    delete recAn;
    
    cout<<" Init histo for reaction ... 1 "<<endl;
    for(int istate=0;istate<2;istate++){
        for(int ipar=0;ipar<5;ipar++){
            param[istate][ipar]=new TCCplotAnalysisUtils(rootfile_output,nparticlecuts,ncuts);
            param[istate][ipar]->InitParH1D(paramState[istate],paramName[ipar]);
            param[istate][ipar]->InitEffPurvsParH1D(paramState[istate],paramName[ipar]);

            //for inclusive cc study the systematic (irea=0)
            param[istate][ipar]->InitSysH1D(0,paramState[istate],paramName[ipar]);
            param[istate][ipar]->InitEffPurvsParSysH1D(0,paramState[istate],paramName[ipar]);
        }
    }
    cout<<" Init histo for reaction ... 2 "<<endl;
    std::string reaction[NSELREACTION];
    for(int iselrea=0;iselrea<NSELREACTION;iselrea++){
        reaction[iselrea]=param[0][0]->GetReactionName(iselrea);
    }
    char eventState[50];
   
    for(int ipar=0;ipar<5;ipar++){
        int iter=0;
        for(int ipar2=0;ipar2<5;ipar2++){
            if(ipar2!=ipar){
                for(int irea=0;irea<3;irea++){
                    diffparam_True[irea][ipar][iter]=new TCCplotAnalysisUtils(rootfile_output,nparticlecuts,ncuts);
                    sprintf(eventState,"True%s",reaction[irea].c_str());
                    diffparam_True[irea][ipar][iter]->InitDiffPar1Par2pPar3TOT("True",paramName[ipar],"Rec",paramName[ipar],"True",paramName[ipar2],eventState);
                }
                int icut=NCUTS-1;

                for(int iselrea=0;iselrea<3;iselrea++){
                    diffparam_Sel[iselrea][ipar][iter]=new TCCplotAnalysisUtils(rootfile_output,nparticlecuts,ncuts);
                    sprintf(eventState,"Sel%s_Cut%d",reaction[iselrea].c_str(),icut);
                    diffparam_Sel[iselrea][ipar][iter]->InitDiffPar1Par2pPar3TOT("True",paramName[ipar],"Rec",paramName[ipar],"True",paramName[ipar2],eventState);
                    for(int itrrea=0;itrrea<3;itrrea++){
                        diffparam_SelTrue[iselrea][itrrea][ipar][iter]=new TCCplotAnalysisUtils(rootfile_output,nparticlecuts,ncuts);
                        sprintf(eventState,"Sel%sTrue%s_Cut%d",reaction[iselrea].c_str(),reaction[itrrea].c_str(),icut);
                        diffparam_SelTrue[iselrea][itrrea][ipar][iter]->InitDiffPar1Par2pPar3TOT("True",paramName[ipar],"Rec",paramName[ipar],"True",paramName[ipar2],eventState);
                    }
                }
                iter++;
            }
        }
    }
    cout<<" Init histo for reaction ... 3 "<<endl;
    //(Q2,Enu) 
    //(Q2,CosThmu)
    //(Q2,Pmu)
    //(Q2,VtxZ)
    //(Enu,CosThmu) 
    //(Enu,Pmu)
    //(Enu,VtxZ)
    //(CosThmu,Pmu)
    //(CosThmu,VtxZ)
    //(Pmu,VtxZ)
    //(VtxZ,VtxZ)
    for(int istate=0;istate<2;istate++){
        int iter=0;
        for(int ipar=0;ipar<5;ipar++){
            for(int ipar2=ipar;ipar2<5;ipar2++){
                if(ipar!=ipar2){
                    param2D[istate][iter]=new TCCplotAnalysisUtils(rootfile_output,nparticlecuts,ncuts);
                    param2D[istate][iter]->InitParH2D(paramState[istate],paramName[ipar],paramState[istate],paramName[ipar2]);
                    param2D[istate][iter]->InitEffPurvsParH2D(paramState[istate],paramName[ipar],paramState[istate],paramName[ipar2]);
                    iter++;
                }
            }
        }
    }
    cout<<" Init histo for reaction ... 4 "<<endl;
    counter=new TCCplotAnalysisUtils(rootfile_output,nparticlecuts,ncuts);
    counter->InitReaCounter();
    counter->InitSysReaCounter();

}

void FillReactionPlots(TFile *rootfile_output,string selcasePAR,string selcaseREA,int truestudy){
  
    TCCreconAnalysisUtils *recAn=new TCCreconAnalysisUtils();
    int ncuts=recAn->GetNCuts(selcaseREA);
    recAn->SetNCuts(selcaseREA);
    recAn->SetNParticleCuts(selcasePAR);
    TCCtruthAnalysisUtils *trueAn;
    TGeneAnalysisUtils *anUtils=new TGeneAnalysisUtils();
    if(truestudy) trueAn=new TCCtruthAnalysisUtils();
    TTree *tree_Ev=(TTree*) rootfile_output->Get("tree_process");

    tree_Ev->SetBranchAddress("_NTRACKS",&_NTRACKS);
    tree_Ev->SetBranchAddress("_NDELAYEDCLUS",&_NDELAYEDCLUS);
    tree_Ev->SetBranchAddress("_NREACTION",&_NREACTION);
    tree_Ev->SetBranchAddress("_NSELREACTION",&_NSELREACTION);
    
    tree_Ev->SetBranchAddress("evid",&_evid);
    tree_Ev->SetBranchAddress("weightMA",&_weightMA);
    tree_Ev->SetBranchAddress("isRecVtxFV",&_isRecVtxFV);
    tree_Ev->SetBranchAddress("hasMichel",&_hasMichel);
    tree_Ev->SetBranchAddress("ntracks",&_ntracks);
    tree_Ev->SetBranchAddress("ntpctracks",&_ntpctracks);
    tree_Ev->SetBranchAddress("nfgdtracks",&_nfgdtracks);
    tree_Ev->SetBranchAddress("evSel",_evSel);
    tree_Ev->SetBranchAddress("RecVtx",_RecVtx);
    tree_Ev->SetBranchAddress("RecPmu",&_RecPmu);
    tree_Ev->SetBranchAddress("RecEnu",_RecEnu);
    tree_Ev->SetBranchAddress("RecQ2",_RecQ2);
    tree_Ev->SetBranchAddress("RecCosThmu",&_RecCosThmu);
    
    if(truestudy){   
        tree_Ev->SetBranchAddress("evTrue",_evTrue);
        tree_Ev->SetBranchAddress("evG4Code",&_evG4Code);
        tree_Ev->SetBranchAddress("TrueVtx",_TrueVtx);
        tree_Ev->SetBranchAddress("TrueMomprim",&_TrueMomprim);
        tree_Ev->SetBranchAddress("TrueEnu",&_TrueEnu);
        tree_Ev->SetBranchAddress("TrueQ2",&_TrueQ2);
        tree_Ev->SetBranchAddress("TrueCosThprim",&_TrueCosThprim);
        tree_Ev->SetBranchAddress("OUTleptonPDG",&_OUTleptonPDG);
        tree_Ev->SetBranchAddress("OUTnuPDG",&_OUTnuPDG);
        tree_Ev->SetBranchAddress("OUTtrajPDG",_OUTtrajPDG);
        tree_Ev->SetBranchAddress("OUTtrajMom",_OUTtrajMom);
        tree_Ev->SetBranchAddress("OUTtrajCosTh",_OUTtrajCosTh);
    }

    tree_Ev->SetBranchAddress("isUsefullTrack",_isUsefullTrack);
    tree_Ev->SetBranchAddress("trid",_trid);
    tree_Ev->SetBranchAddress("trSel",_trSel);
    tree_Ev->SetBranchAddress("trQuality",_RecMom);
    tree_Ev->SetBranchAddress("RecMom",_RecMom);
    tree_Ev->SetBranchAddress("RecCosTh",_RecCosTh);
    tree_Ev->SetBranchAddress("RecCharge",_RecCharge);
    tree_Ev->SetBranchAddress("BackWards",_BackWards);
    tree_Ev->SetBranchAddress("SigMom",_SigMom);
    tree_Ev->SetBranchAddress("MomErr",_MomErr);
    tree_Ev->SetBranchAddress("NNodes",_NNodes);
    tree_Ev->SetBranchAddress("dEdxmeas",_dEdxmeas);
    tree_Ev->SetBranchAddress("Pull",_Pull);
    tree_Ev->SetBranchAddress("Sigma",_Sigma);
    tree_Ev->SetBranchAddress("dEdxexp",_dEdxexp);
    tree_Ev->SetBranchAddress("LikeLHD",_LikeLHD);
    tree_Ev->SetBranchAddress("Prob",_Prob);
    tree_Ev->SetBranchAddress("trDir",_trDir);

    if(truestudy){   
        tree_Ev->SetBranchAddress("TrueMom",_TrueMom);
        tree_Ev->SetBranchAddress("TrueCosTh",_TrueCosTh);
        tree_Ev->SetBranchAddress("TrueCharge",_TrueCharge);
        tree_Ev->SetBranchAddress("trTrue",_trTrue);
        tree_Ev->SetBranchAddress("trG4Code",_trG4Code);
    }

    trueVtxFV=0;
    TotalInFV[0]=0;
    TotalInFV[1]=0;
    
    int entries=tree_Ev->GetEntries();
    cout<<" nb of entries "<<entries<<endl;
 
    TStopwatch timer;
    timer.Start();
    //   //entries=600;
    //initialize the name of the different reaction, subreaction
    // or define the systematics
    //*********************************************************
    //0  1   2   3
    double systematics[CCNCSUBNREACTION]={1,0.3,0.2,0.3}; 
    int isccsubreaction[CCNCSUBNREACTION];
    
    for(int ient=0;ient<entries;ient++){
        if(ient%5000 == 0)
        {
            std::cout <<  "********************************" << std::endl;
            std::cout <<  "    !!!Events Processed " << ient << std::endl;
            timer.Stop();
            std::cout << " Processing time: " << timer.CpuTime() << " " << timer.RealTime() << std::endl;
            std::cout <<  "********************************" << std::endl;
            timer.Start();
        }
        tree_Ev->GetEntry(ient);

        bool truebox=false;
        if(truestudy){
            if(_TrueMomprim<1000 && _TrueCosThprim>0.5)
                truebox=true;
            for(int isub=0;isub<CCNCSUBNREACTION;isub++){
                isccsubreaction[isub]=0;
                isccsubreaction[isub]=(int)trueAn->IsCCNCsubReaction((int)_evG4Code,isub);
            }
        }

        for(int iselrea=0;iselrea<NSELREACTION;iselrea++){
            double recpar[7]={_RecQ2[iselrea],_RecEnu[iselrea],_RecCosThmu,_RecPmu,_RecVtx[2],_RecVtx[0],_RecVtx[1]};
            for(int icut=0;icut<ncuts;icut++){
                // look for the selected event (S+B)
                //********************************
                if(iselrea==_evSel[iselrea][icut]){
                    counter->FillSelReaCounter(iselrea,icut,truebox,_weightMA);
                    for(int ipar=0;ipar<5;ipar++){
                        if(truestudy){
                            
                            double truepar[7]={_TrueQ2,_TrueEnu,_TrueCosThprim,_TrueMomprim,_TrueVtx[2],_TrueVtx[0],_TrueVtx[1]};  
                            param[0][ipar]->FillSelReaH1D(iselrea,icut,truepar[ipar], _weightMA);
                        }
                        param[1][ipar]->FillSelReaH1D(iselrea,icut,recpar[ipar], _weightMA);
                    }
                    if(truestudy){
                        double truepar[7]={_TrueQ2,_TrueEnu,_TrueCosThprim,_TrueMomprim,_TrueVtx[2],_TrueVtx[0],_TrueVtx[1]};  
                        for(int ipar=0;ipar<5;ipar++){
                            int iter=0;
                            for(int ipar2=0;ipar2<5;ipar2++){
                                if(ipar!=ipar2){
                                    if(icut==ncuts-1  && iselrea<3)
                                        diffparam_Sel[iselrea][ipar][iter]->FillDiffPar1Par2pPar3TOT(truepar[ipar],recpar[ipar],truepar[ipar2]);
                                    iter++;
                                }
                            }
                        }
                    }
                    int iter=0;
                    for(int ipar=0;ipar<5;ipar++){
                        for(int ipar2=ipar;ipar2<5;ipar2++){
                            if(ipar!=ipar2){
                                if(truestudy){
                                    double truepar[7]={_TrueQ2,_TrueEnu,_TrueCosThprim,_TrueMomprim,_TrueVtx[2],_TrueVtx[0],_TrueVtx[1]};  
                                    param2D[0][iter]->FillSelReaH2D(iselrea,icut,truepar[ipar],truepar[ipar2],_weightMA);
                                }
                                param2D[1][iter]->FillSelReaH2D(iselrea,icut,recpar[ipar],recpar[ipar2], _weightMA);
                                iter++;
                            }
                        }
                    }
                    //selected as ccAll
                    //******************
                    if(iselrea==0){
                        for(int isub=0;isub<CCNCSUBNREACTION;isub++){                               
                            if(isccsubreaction[isub]){
                                counter->FillSelSysReaCounter(isub,icut,truebox,systematics[isub]);
                                for(int ipar=0;ipar<5;ipar++){
                                    if(truestudy){
                                        double truepar[7]={_TrueQ2,_TrueEnu,_TrueCosThprim,_TrueMomprim,_TrueVtx[2],_TrueVtx[0],_TrueVtx[1]};  
                                        param[0][ipar]->FillSelSysH1D(isub,icut,truepar[ipar],systematics[isub]);
                                    }
                                    param[1][ipar]->FillSelSysH1D(isub,icut,recpar[ipar], systematics[isub]);
                                }
                            }
                            else{
                                counter->FillSelSysReaCounter(isub,icut,truebox,0);
                                for(int ipar=0;ipar<5;ipar++){
                                    if(truestudy){
                                        double truepar[7]={_TrueQ2,_TrueEnu,_TrueCosThprim,_TrueMomprim,_TrueVtx[2],_TrueVtx[0],_TrueVtx[1]};  
                                        param[0][ipar]->FillSelSysH1D(isub,icut,truepar[ipar],0);
                                    }
                                    param[1][ipar]->FillSelSysH1D(isub,icut,recpar[ipar],0);
                                }
                            }
                        }
                    }//end on iselrea=0
                }
            }
        }
        
        if(truestudy){
            if((_OUTleptonPDG!=13 && _evTrue[0]==0) ||( _OUTnuPDG!=14 && _evTrue[4]==4))
                continue;
                       
           
            //take into account, vtx in FV, and all event not taken into account (bug gene+bug reco)
            if(_evTrue[0]==0 || _evTrue[4]==4){
                trueVtxFV++;
                TotalInFV[0]++;
                if(truebox)
                    TotalInFV[1]++;
            }
            
            double truepar[7]={_TrueQ2,_TrueEnu,_TrueCosThprim,_TrueMomprim,_TrueVtx[2],_TrueVtx[0],_TrueVtx[1]};  
  
            for(int irea=0;irea<NREACTION;irea++){
                //TRUE
                //*******************************

                if( irea==_evTrue[irea]){
                    
                    double recpar[7]={_RecQ2[irea],_RecEnu[irea],_RecCosThmu,_RecPmu,_RecVtx[2],_RecVtx[0],_RecVtx[1]};
                    counter->FillTrueReaCounter(irea,truebox, _weightMA);
                    for(int ipar=0;ipar<5;ipar++){
                        param[0][ipar]->FillTrueReaH1D(irea,truepar[ipar], _weightMA);
                        param[1][ipar]->FillTrueReaH1D(irea,recpar[ipar], _weightMA);
                    }
                    int iter=0;
                    for(int ipar=0;ipar<5;ipar++){
                        iter=0;
                        for(int ipar2=0;ipar2<5;ipar2++){
                            if(ipar!=ipar2 && irea<3){
                                diffparam_True[irea][ipar][iter]->FillDiffPar1Par2pPar3TOT(truepar[ipar],recpar[ipar],truepar[ipar2]);
                                iter++;
                            }
                        }
                        iter=0;
                        for(int ipar2=ipar;ipar2<5;ipar2++){
                            if(ipar!=ipar2  && irea<3){
                                param2D[0][iter]->FillTrueReaH2D(irea,truepar[ipar],truepar[ipar2],_weightMA);
                                param2D[1][iter]->FillTrueReaH2D(irea,recpar[ipar],recpar[ipar2], _weightMA);
                                iter++;
                            }
                        }
                    }
                    
                    //For TrueCC only
                    //***************
                    if(irea==0){
                        for(int isub=0;isub<CCNCSUBNREACTION;isub++){
                            if(isccsubreaction[isub]){
                                //for cc/nc systematic it should never enter here
                                counter->FillTrueSysReaCounter(isub,truebox,systematics[isub] );
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


            for(int iselrea=0;iselrea<NSELREACTION;iselrea++){
                double recpar[7]={_RecQ2[iselrea],_RecEnu[iselrea],_RecCosThmu,_RecPmu,_RecVtx[2],_RecVtx[0],_RecVtx[1]};
                for(int icut=0;icut<ncuts;icut++){
                    if(iselrea==_evSel[iselrea][icut]){
                        //look to the selected and true event (S)
                        //****************************************
                        for(int irea=0;irea<NREACTION;irea++){
                            if(irea==_evTrue[irea]){
                                counter->FillSelTrueReaCounter(iselrea,icut,irea,truebox, _weightMA);
                                for(int ipar=0;ipar<5;ipar++){
                                    param[0][ipar]->FillSelTrueReaH1D(iselrea,icut,irea,truepar[ipar], _weightMA);
                                    param[1][ipar]->FillSelTrueReaH1D(iselrea,icut,irea,recpar[ipar], _weightMA);
                                }
                                int iter=0;
                                for(int ipar=0;ipar<5;ipar++){
                                    iter=0;
                                    for(int ipar2=0;ipar2<5;ipar2++){
                                        if(ipar!=ipar2){
                                            if(icut==ncuts-1  && irea<3 && iselrea<3)
                                                diffparam_SelTrue[iselrea][irea][ipar][iter]->FillDiffPar1Par2pPar3TOT(truepar[ipar],recpar[ipar],truepar[ipar2]);
                                            iter++;
                                        }
                                    }
                                    iter=0;
                                    for(int ipar2=ipar;ipar2<5;ipar2++){
                                        if(ipar!=ipar2){
                                            param2D[0][iter]->FillSelTrueReaH2D(iselrea,icut,irea,truepar[ipar],truepar[ipar2],_weightMA);
                                            param2D[1][iter]->FillSelTrueReaH2D(iselrea,icut,irea,recpar[ipar],recpar[ipar2], _weightMA);
                                            iter++;
                                        }
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
        }//end truestudy
    }//end entries
           

    delete recAn;
    delete trueAn;
    delete anUtils;

}

void WriteReactionPlots(string selcaseREA){
    TCCreconAnalysisUtils *recAn=new TCCreconAnalysisUtils();
    int ncuts=recAn->GetNCuts(selcaseREA);
    delete recAn;
    //NORMALISATION FACTOR so that the number of event is the number of expected event:
    //we have form the IPNS doc (nov. 2006) for 40 GeV/protons and 10e21 POT
    //the number of expected event is about 1.7e5 events/ton/year <=> 640 kW for 10e7 s.

    //We expect 750 kW for 10e7 s. and 30 GeV/protons:
    //@ full intensity: 1.7e5 x 750/640 = 2e5 events
    //@ 15% intensity (1st year): 2e5 x 0.15 = 3e4 events

    //Number of events/ton in FV:
    //FGD FV: (146, 146, 30) cm3 x 1 g/cm3 = 1 ton
    //Nsim= N event simulated in the FGD
    //NsimFV = N event sim. in the FGD with true vtx in FV= Nsim x 47,3 % = trueVtxFV
    //Nexp_true = Nsim x 47.3% /(2 FGDFV) = trueVtxFV/2
    //norm. factor: 3e4/Nexp_true
    double normFactor=1;
    if(trueVtxFV!=0)normFactor=30000/trueVtxFV*2;

    cout<<" normalisation factor "<<normFactor<<endl;
    
    //PLOT AND WRITE HISTOGRAMS IN THE FILE
    //££££££££££££££££££££££££££££££££££££££
    cout<<" fill eff pur vs param "<<endl;
    for(int istate=0;istate<2;istate++){
        for(int ipar=0;ipar<5;ipar++){
            param[istate][ipar]->FillEffPurvsParH1D();
            param[istate][ipar]->FillEffPurvsParSysH1D();
        }
    }
     cout<<" fill eff pur vs param 2d"<<endl;
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
         for(int ipar=0;ipar<5;ipar++){
             cout<<" istate "<<istate<<" ipar "<<ipar<<" "<<paramName[ipar]<<" "<<paramState[istate]<<" norm "<<normFactor<<endl;
             param[istate][ipar]->WriteParH1D(paramState[istate],paramName[ipar],normFactor);
             param[istate][ipar]->WriteParSysH1D(paramState[istate],paramName[ipar],normFactor);
             param[istate][ipar]->WriteEffPurvsParH1D(paramState[istate],paramName[ipar]);
             param[istate][ipar]->WriteEffPurvsParSysH1D(paramState[istate],paramName[ipar]);
        }
    }
    for(int istate=0;istate<2;istate++){
        int iter=0;
        for(int ipar=0;ipar<5;ipar++){
            for(int ipar2=ipar;ipar2<5;ipar2++){
                if(ipar!=ipar2){
                    param2D[istate][iter]->WriteEffPurvsParH2D(paramState[istate],paramName[ipar],paramState[istate],paramName[ipar2]);
                    param2D[istate][iter]->WriteParH2D(paramState[istate],paramName[ipar],paramState[istate],paramName[ipar2],normFactor);
                    iter++;
                }
            }
        }
    }

    
    
     for(int iselrea=0;iselrea<NSELREACTION;iselrea++){
         int irea=0;
         if(iselrea<NREACTION)
             irea=iselrea;
         counter->PrintStatvsCut(iselrea,irea,0,normFactor);
         counter->PrintStatvsCut(iselrea,irea,0);

     }
    
    counter->PrintSysStat(0,ncuts-1,0);
    counter->PrintEffMatrix(ncuts-1,0);
    counter->PrintPurMatrix(ncuts-1,0);
    counter->PrintSelTrueMatrix(ncuts-1,0);
    counter->PrintTopoFraction(TotalInFV[0],0);
    counter->PrintTopoFraction(TotalInFV[1],1);

    counter->InitEffPurFOMvsReaCut();
    counter->FillEffPurFOMvsReaCut();
    counter->PlotEffPurFOMvsReaCut();
 
    
    cout<<"write diff param "<<endl;
    std::string reaction[NSELREACTION];
    for(int iselrea=0;iselrea<NSELREACTION;iselrea++){
        reaction[iselrea]=param[0][0]->GetReactionName(iselrea);
    }

    
    char eventState[50];
    for(int ipar=0;ipar<5;ipar++){
        int iter=0;
        for(int ipar2=0;ipar2<5;ipar2++){
            if(ipar2!=ipar){
                for(int irea=0;irea<3;irea++){
                    sprintf(eventState,"True%s",reaction[irea].c_str());
                    diffparam_True[irea][ipar][iter]->WriteDiffPar1Par2pPar3TOT("True",paramName[ipar],"Rec",paramName[ipar],"True",paramName[ipar2],eventState);
                }
                int icut=ncuts-1;
                for(int iselrea=0;iselrea<3;iselrea++){
                    sprintf(eventState,"Sel%s_Cut%d",reaction[iselrea].c_str(),icut);
                    diffparam_Sel[iselrea][ipar][iter]->WriteDiffPar1Par2pPar3TOT("True",paramName[ipar],"Rec",paramName[ipar],"True",paramName[ipar2],eventState);
                    for(int itrrea=0;itrrea<3;itrrea++){
                        sprintf(eventState,"Sel%sTrue%s_Cut%d",reaction[iselrea].c_str(),reaction[itrrea].c_str(),icut);
                        diffparam_SelTrue[iselrea][itrrea][ipar][iter]->WriteDiffPar1Par2pPar3TOT("True",paramName[ipar],"Rec",paramName[ipar],"True",paramName[ipar2],eventState);
                    }
                }
                iter++;
            }
        }
    }
}

void DeleteReactionPlots(){

    //DELETE HISTOGRAMS IN THE FILE
    //££££££££££££££££££££££££££££££££££££££
          
    for(int istate=0;istate<2;istate++){
         for(int ipar=0;ipar<5;ipar++){
             param[istate][ipar]->DeleteParH1D();
             param[istate][ipar]->DeleteParSysH1D();
             param[istate][ipar]->DeleteEffPurvsParH1D();
             param[istate][ipar]->DeleteEffPurvsParSysH1D();
        }
    }
    for(int istate=0;istate<2;istate++){
        int iter=0;
        for(int ipar=0;ipar<5;ipar++){
            for(int ipar2=ipar;ipar2<5;ipar2++){
                if(ipar!=ipar2){
                    param2D[istate][iter]->DeleteEffPurvsParH2D();
                    param2D[istate][iter]->DeleteParH2D();
                    iter++;
                }
            }
        }
    }

    for(int istate=0;istate<2;istate++){
        for(int ipar=0;ipar<5;ipar++){
            delete param[istate][ipar];
        }
    }
    for(int istate=0;istate<2;istate++){
        int iter=0;
        for(int ipar=0;ipar<5;ipar++){
            for(int ipar2=ipar;ipar2<5;ipar2++){
                if(ipar!=ipar2){
                    delete param2D[istate][iter];
                    iter++;
                }
            }
        }
    }

    for(int irea=0;irea<3;irea++){
        for(int ip=0;ip<5;ip++){
            int iter=0;
            for(int ipar2=0;ipar2<5;ipar2++){
                if(ip!=ipar2){
                    diffparam_True[irea][ip][iter]->DeleteDiffPar1Par2pPar3TOT();
                    iter++;
                }
            }
        }
    }

    for(int iselrea=0;iselrea<3;iselrea++){
        for(int ipar=0;ipar<5;ipar++){
            int iter=0;
            for(int ipar2=0;ipar2<5;ipar2++){
                if(ipar2!=ipar){
                    diffparam_Sel[iselrea][ipar][iter]->DeleteDiffPar1Par2pPar3TOT();
                    for(int itrrea=0;itrrea<3;itrrea++){
                        diffparam_SelTrue[iselrea][itrrea][ipar][iter]->DeleteDiffPar1Par2pPar3TOT();
                    }
                    iter++;
                }
            }
        }
    }
  

     for(int ipar=0;ipar<5;ipar++){
         int iter=0;
        for(int ipar2=0;ipar2<5;ipar2++){
            if(ipar2!=ipar){
                for(int irea=0;irea<3;irea++){
                   delete  diffparam_True[irea][ipar][iter];
                }
                for(int iselrea=0;iselrea<3;iselrea++){
                    delete diffparam_Sel[iselrea][ipar][iter];
                    for(int itrrea=0;itrrea<3;itrrea++){
                        delete diffparam_SelTrue[iselrea][itrrea][ipar][iter];
                    }
                }
                iter++;
            }
        }
     }
     delete counter;
     
     cout<<" Deleting all histograms plots on reaction from memory ...  "<<endl;
     
        
}
void ResetPidCuts(){
    for(int isel=0;isel<NPARTICLESEL;isel++){
        pullcut[isel]=0;
    }
    for(int iel=0;iel<4;iel++){
        elcut[iel]=0;
    }
    for(int isel=0;isel<NPARTICLESEL;isel++){
        probcut[isel]=0;
    }
}
void ResetReaCuts(){
    fvcut.SetXYZ(0,0,0);
    dthpcut=0;
    mnu2cut=0;
    dDelCcut=0;
}
void SetPidCutsManually(){
 
    probcut[0]=0.075;
    probcut[1]=0.475;
    probcut[2]=0.7;
    probcut[3]=0.075;//pi- same as mu
    probcut[4]=0.225;
    probcut[5]=0.25;
    probcut[6]=0;
    pullcut[0]=4;
    elcut[0]=100;//recmom neg ch
    elcut[1]=1;//recmom pos ch
    elcut[2]=100;//sumpull neg ch
    elcut[3]=1;//sumpull pos ch
}
void SetReaCutsManually(){
    fvcut.SetXYZ(200,200,10);
    dthpcut=0.5;
    mnu2cut=-0.4;
    dDelCcut=800;
}
