#include "AnalysisManager.hxx"
#include "ND280AnalysisUtils.hxx"
//#include <gperftools/profiler.h>
#include <sys/time.h>
#include <unistd.h>

#include "ToyMakerExample.hxx"
#include <Parameters.hxx>
#include <TCanvas.h>
#include <TH1.h>
#include <TH1F.h>
#include "TRandom3.h"
#include "MultiThread.hxx"

int main(int argc, char *argv[]){

  std::string programName = argv[0];
  std::string paramFile = "";
  int nmax = 100000000;
  UInt_t ntoys=1;
  std::string inputFileName = "";
  std::string inputFileType = "kHighlandTree";
  std::string outputFileName= "";
  Int_t debug = 0;
  Int_t preload=1;
  for (;;) {
    int c = getopt(argc, argv, "n:t:d:o:p:");
    if (c < 0)
      break;
    switch (c) {
    case 'n': {
      std::istringstream tmp(optarg);
      tmp >> nmax;
      break;
    }
    case 't': {
      std::istringstream tmp(optarg);
      tmp >> ntoys;
      break;
    }
    case 'd': {
      std::istringstream tmp(optarg);
      tmp >> debug;
      break;
    }
    case 'o': {
      outputFileName= optarg;
      TFile *test=TFile::Open(outputFileName.c_str(),"READ");
      if(test){
        std::cout<<" OUTPUT ALREADY EXISTS ! please give another name..."<<std::endl;
        return 0;
      }
      //      test->Close();
      //      delete test;
      break;
    }
    case 'p': {
      std::istringstream tmp(optarg);
      tmp >> preload;
      break;
    }
    default: {
    }
    }
  }

#ifndef MULTITHREAD
      if (preload==1){
        std::cout << "For preloading mode you must uncomment the line #define MULTITHREAD in psycheCore/vXrY/srx/MultiThread.hxx and recompile everything !!!" << std::endl;
        return 0;
      }
#else
      if (preload==0){
        std::cout << "For interactive mode you must comment out the line #define MULTITHREAD psycheCore/vXrY/srx/MultiThread.hxx and recompile everything !!!" << std::endl;
        return 0;
      }
#endif
  
  // get the input file name
  inputFileName = argv[optind++];

  // Read the parameters files following the package hierarchy
  // first the top level package. Set the parameters as fixed
  ND::params().LoadParametersFiles(anaUtils::GetPackageHierarchy(), true);

  // Make sure no parameters have been accessed yet
  ND::params().SetReadParamOverrideFilePointPassed();

  // Parameters to control the systematics
  bool applyVariationSystematics = (bool)ND::params().GetParameterI("psycheSteering.Systematics.ApplyVariationSystematics");
  bool applyWeightSystematics    = (bool)ND::params().GetParameterI("psycheSteering.Systematics.ApplyWeightSystematics");
  bool applyFluxWeightSystematics = (bool)ND::params().GetParameterI("psycheSteering.Systematics.ApplyFluxWeightSystematic");
  if(!applyFluxWeightSystematics && !applyVariationSystematics && !applyWeightSystematics) { 
    std::cout<<" no systematics is set to be applied "<<std::endl;return false;
  }
  
  bool applySyst[20]={false};                    
  applySyst[0]= (bool)ND::params().GetParameterI("psycheSteering.Variations.EnableBFieldDist");
  applySyst[1]= (bool)ND::params().GetParameterI("psycheSteering.Variations.EnableMomScale");
  applySyst[2]= (bool)ND::params().GetParameterI("psycheSteering.Variations.EnableMomResol");
  applySyst[3]= (bool)ND::params().GetParameterI("psycheSteering.Variations.EnableTpcPid");
  applySyst[4]= (bool)ND::params().GetParameterI("psycheSteering.Variations.EnableFgdPid");
  
  applySyst[5]= (bool)ND::params().GetParameterI("psycheSteering.Weights.EnableChargeConf");
  applySyst[6]= (bool)ND::params().GetParameterI("psycheSteering.Weights.EnableTpcClusterEff");
  applySyst[7]= (bool)ND::params().GetParameterI("psycheSteering.Weights.EnableTpcTrackEff");
  applySyst[8]= (bool)ND::params().GetParameterI("psycheSteering.Weights.EnableTpcFgdMatchEff");
  applySyst[9]= (bool)ND::params().GetParameterI("psycheSteering.Weights.EnableFgdTrackEff");
  applySyst[10]= (bool)ND::params().GetParameterI("psycheSteering.Weights.EnableFgdHybridTrackEff");
  applySyst[11]= (bool)ND::params().GetParameterI("psycheSteering.Weights.EnableMichelEleEff");
  applySyst[12]= (bool)ND::params().GetParameterI("psycheSteering.Weights.EnablePileUp");
  applySyst[13]= (bool)ND::params().GetParameterI("psycheSteering.Weights.EnableFgdMass");
  applySyst[14]= (bool)ND::params().GetParameterI("psycheSteering.Weights.EnableOOFV");
  
  applySyst[15]= (bool)ND::params().GetParameterI("psycheSteering.Weights.EnableSIPion");
  applySyst[16]= (bool)ND::params().GetParameterI("psycheSteering.Weights.EnableSandMuons");
  applySyst[17]= (bool)ND::params().GetParameterI("psycheSteering.Weights.EnableFluxNeutrino");
  applySyst[18]= (bool)ND::params().GetParameterI("psycheSteering.Weights.EnableFluxAntiNeutrino");
  applySyst[19]=1;
  UInt_t NSYST=20;

  std::vector<Int_t> npassed(NSYST,0);
  // Initialize clock
  timeval tim;
  gettimeofday(&tim, NULL);
  double t0=tim.tv_sec+(tim.tv_usec/1000000.0);
  std::cout<<" DEFINING MANAGER "<<std::endl;
  AnalysisManager _man;

  // Make and fill the EventSummary even when the selection is not passed.
  if(ND::params().GetParameterI("psycheSteering.Selections.ForceFillEventSummary"))
    _man.sel().SetForceFillEventSummary(true);
  
  // Initialize the InputManager by specifying the input type and the input file
  if (!_man.input().Initialize(inputFileName,inputFileType, false)) return false;
  std::vector<SystematicBase*>  allSyst = _man.syst().GetSystematics();



  // Create a ToyMaker to configure the toy experiment. Initialize it with a random seed
  ToyMaker* toyMaker = new ToyMakerExample((UInt_t)ND::params().GetParameterI("psycheSteering.Systematics.RandomSeed"), 
                                           (bool)  ND::params().GetParameterI("psycheSteering.Systematics.ZeroVariation"));

  // Create and fill the Toy experiment with the appropriate format (number of systematics and number of parameters for each systematic)
  toyMaker->CreateToyExperiments(ntoys, _man.syst().GetSystematics()); 

  std::vector<float> weights;
  // Print the steps for the different selections
  if (debug>0){
    _man.sel().GetSelection("kTrackerNumuCC")->DumpSteps();
    _man.sel().GetSelection("kTrackerNumuCCMultiPi")->DumpSteps();
    _man.syst().DumpVariationSystematics();
    _man.syst().DumpWeightSystematics();
  }

  if( preload){
    std::cout<<" preloading!!!! "<<std::endl;
    // Preload nmax events from the file
    if (!_man.ReadEvents(inputFileName, nmax)) return 0;
    if(nmax < 0) nmax = _man.GetEntries();
    _man.SetNEventsToProcess(nmax);
  }
  else{
    // Create the array of PreviousToyBox
    _man.sel().CreateToyBoxArray(nmax);
  }

   //#endif
  TTree *tree;
  TFile *outfile=new TFile(outputFileName.c_str(),"NEW");
  TString systnametree[20]={"bfield_syst","momscale_syst","momresol_syst","tpcpid_syst","fgdpid_syst","chargeideff_syst","tpcclustereff_syst","tpctrackeff_syst","tpcfgdmatcheff_syst","fgdtrackeff_syst","fgdhybridtrackeff_syst","michel_syst","pileup_syst","fgdmass_syst","oofv_syst","sipion_syst","sandmu_syst","nuflux_syst","antinuflux_syst","all_syst"};

  for(unsigned int isyst=0;isyst<NSYST;isyst++){
    if( !applySyst[isyst] ) continue;

    std::string systname = "all_syst";
    if (_man.syst().GetSystematic(isyst)) systname = _man.syst().GetSystematic(isyst)->Name();

    std::cout << "RunSyst: Process systematic " << systname << std::endl;

    // This the analysis manager
    Float_t selmu_momNom=-999;
    Float_t selmu_momToy[1000]={-999};
    Weight_h weightsToy[1000]={-999};
    Int_t   isFGD1CCNom=-1;
    Int_t   isFGD1CC0PiNom=-1;
    Int_t   isFGD1CC1PiNom=-1;
    Int_t   isFGD1CCOtherNom=-1;
    Int_t   isFGD1CCToy[1000]={-1};
    Int_t   isFGD1CC0PiToy[1000]={-1};
    Int_t   isFGD1CC1PiToy[1000]={-1};
    Int_t   isFGD1CCOtherToy[1000]={-1};
    
    Int_t   isFGD2CCNom=-1;
    Int_t   isFGD2CC0PiNom=-1;
    Int_t   isFGD2CC1PiNom=-1;
    Int_t   isFGD2CCOtherNom=-1;
    Int_t   isFGD2CCToy[1000]={-1};
    Int_t   isFGD2CC0PiToy[1000]={-1};
    Int_t   isFGD2CC1PiToy[1000]={-1};
    Int_t   isFGD2CCOtherToy[1000]={-1};

    Int_t   isFGD1AntiCCNom=-1;
    Int_t   isFGD1AntiCC1TrNom=-1;
    Int_t   isFGD1AntiCCNTrNom=-1;
    Int_t   isFGD1AntiCCToy[1000]={-1};
    Int_t   isFGD1AntiCC1TrToy[1000]={-1};
    Int_t   isFGD1AntiCCNTrToy[1000]={-1};
    
    Int_t   isFGD2AntiCCNom=-1;
    Int_t   isFGD2AntiCC1TrNom=-1;
    Int_t   isFGD2AntiCCNTrNom=-1;
    Int_t   isFGD2AntiCCToy[1000]={-1};
    Int_t   isFGD2AntiCC1TrToy[1000]={-1};
    Int_t   isFGD2AntiCCNTrToy[1000]={-1};


    
    Int_t   isFGD1NuMuBkInAntiCCNom=-1;
    Int_t   isFGD1NuMuBkInAntiCC1TrNom=-1;
    Int_t   isFGD1NuMuBkInAntiCCNTrNom=-1;
    Int_t   isFGD1NuMuBkInAntiCCToy[1000]={-1};
    Int_t   isFGD1NuMuBkInAntiCC1TrToy[1000]={-1};
    Int_t   isFGD1NuMuBkInAntiCCNTrToy[1000]={-1};
    
    Int_t   isFGD2NuMuBkInAntiCCNom=-1;
    Int_t   isFGD2NuMuBkInAntiCC1TrNom=-1;
    Int_t   isFGD2NuMuBkInAntiCCNTrNom=-1;
    Int_t   isFGD2NuMuBkInAntiCCToy[1000]={-1};
    Int_t   isFGD2NuMuBkInAntiCC1TrToy[1000]={-1};
    Int_t   isFGD2NuMuBkInAntiCCNTrToy[1000]={-1};




    Double_t fluxweight=1;
    Double_t pileupweight=1;
    Int_t   anaToy[1000]={-1};
    UInt_t   nToys= ntoys;
    if(ntoys>1000)nToys=1000;
    tree= new TTree(systnametree[isyst].Data(),systnametree[isyst].Data());
    // tree= new TTree(systname.c_str(),systname.c_str());
    tree->Branch("nToys",&nToys,"nToys/I");
    tree->Branch("fluxweight",&fluxweight,"fluxweight/D");
    tree->Branch("pileupweight",&pileupweight,"pileupweight/D");
    tree->Branch("selmu_momNom",&selmu_momNom,"selmu_momNom/F");
    tree->Branch("selmu_momToy",selmu_momToy,"selmu_momToy[nToys]/F");
    tree->Branch("weightsToy",weightsToy,"weightsToy[nToys]/F");
    tree->Branch("isFGD1CCNom",&isFGD1CCNom,"isFGD1CCNom/I");
    tree->Branch("isFGD1CCToy",isFGD1CCToy,"isFGD1CCToy[nToys]/I");
    tree->Branch("isFGD1CC0PiNom",&isFGD1CC0PiNom,"isFGD1CC0PiNom/I");
    tree->Branch("isFGD1CC0PiToy",isFGD1CC0PiToy,"isFGD1CC0PiToy[nToys]/I");
    tree->Branch("isFGD1CC1PiNom",&isFGD1CC1PiNom,"isFGD1CC1PiNom/I");
    tree->Branch("isFGD1CC1PiToy",isFGD1CC1PiToy,"isFGD1CC1PiToy[nToys]/I");
    tree->Branch("isFGD1CCOtherNom",&isFGD1CCOtherNom,"isFGD1CCOtherNom/I");
    tree->Branch("isFGD1CCOtherToy",isFGD1CCOtherToy,"isFGD1CCOtherToy[nToys]/I");
    tree->Branch("isFGD2CCNom",&isFGD2CCNom,"isFGD2CCNom/I");
    tree->Branch("isFGD2CCToy",isFGD2CCToy,"isFGD2CCToy[nToys]/I");
    tree->Branch("isFGD2CC0PiNom",&isFGD2CC0PiNom,"isFGD2CC0PiNom/I");
    tree->Branch("isFGD2CC0PiToy",isFGD2CC0PiToy,"isFGD2CC0PiToy[nToys]/I");
    tree->Branch("isFGD2CC1PiNom",&isFGD2CC1PiNom,"isFGD2CC1PiNom/I");
    tree->Branch("isFGD2CC1PiToy",isFGD2CC1PiToy,"isFGD2CC1PiToy[nToys]/I");
    tree->Branch("isFGD2CCOtherNom",&isFGD2CCOtherNom,"isFGD2CCOtherNom/I");
    tree->Branch("isFGD2CCOtherToy",isFGD2CCOtherToy,"isFGD2CCOtherToy[nToys]/I");

    tree->Branch("isFGD1AntiCCNom",&isFGD1AntiCCNom,"isFGD1AntiCCNom/I");
    tree->Branch("isFGD1AntiCCToy",isFGD1AntiCCToy,"isFGD1AntiCCToy[nToys]/I");
    tree->Branch("isFGD1AntiCC1TrNom",&isFGD1AntiCC1TrNom,"isFGD1AntiCC1TrNom/I");
    tree->Branch("isFGD1AntiCC1TrToy",isFGD1AntiCC1TrToy,"isFGD1AntiCC1TrToy[nToys]/I");
    tree->Branch("isFGD1AntiCCNTrNom",&isFGD1AntiCCNTrNom,"isFGD1AntiCCNTrNom/I");
    tree->Branch("isFGD1AntiCCNTrToy",isFGD1AntiCCNTrToy,"isFGD1AntiCCNTrToy[nToys]/I");
    tree->Branch("isFGD2AntiCCNom",&isFGD2AntiCCNom,"isFGD2AntiCCNom/I");
    tree->Branch("isFGD2AntiCCToy",isFGD2AntiCCToy,"isFGD2AntiCCToy[nToys]/I");
    tree->Branch("isFGD2AntiCC1TrNom",&isFGD2AntiCC1TrNom,"isFGD2AntiCC1TrNom/I");
    tree->Branch("isFGD2AntiCC1TrToy",isFGD2AntiCC1TrToy,"isFGD2AntiCC1TrToy[nToys]/I");
    tree->Branch("isFGD2AntiCCNTrNom",&isFGD2AntiCCNTrNom,"isFGD2AntiCCNTrNom/I");
    tree->Branch("isFGD2AntiCCNTrToy",isFGD2AntiCCNTrToy,"isFGD2AntiCCNTrToy[nToys]/I");

    tree->Branch("isFGD1NuMuBkInAntiCCNom",&isFGD1NuMuBkInAntiCCNom,"isFGD1NuMuBkInAntiCCNom/I");
    tree->Branch("isFGD1NuMuBkInAntiCCToy",isFGD1NuMuBkInAntiCCToy,"isFGD1NuMuBkInAntiCCToy[nToys]/I");
    tree->Branch("isFGD1NuMuBkInAntiCC1TrNom",&isFGD1NuMuBkInAntiCC1TrNom,"isFGD1NuMuBkInAntiCC1TrNom/I");
    tree->Branch("isFGD1NuMuBkInAntiCC1TrToy",isFGD1NuMuBkInAntiCC1TrToy,"isFGD1NuMuBkInAntiCC1TrToy[nToys]/I");
    tree->Branch("isFGD1NuMuBkInAntiCCNTrNom",&isFGD1NuMuBkInAntiCCNTrNom,"isFGD1NuMuBkInAntiCCNTrNom/I");
    tree->Branch("isFGD1NuMuBkInAntiCCNTrToy",isFGD1NuMuBkInAntiCCNTrToy,"isFGD1NuMuBkInAntiCCNTrToy[nToys]/I");
    tree->Branch("isFGD2NuMuBkInAntiCCNom",&isFGD2NuMuBkInAntiCCNom,"isFGD2NuMuBkInAntiCCNom/I");
    tree->Branch("isFGD2NuMuBkInAntiCCToy",isFGD2NuMuBkInAntiCCToy,"isFGD2NuMuBkInAntiCCToy[nToys]/I");
    tree->Branch("isFGD2NuMuBkInAntiCC1TrNom",&isFGD2NuMuBkInAntiCC1TrNom,"isFGD2NuMuBkInAntiCC1TrNom/I");
    tree->Branch("isFGD2NuMuBkInAntiCC1TrToy",isFGD2NuMuBkInAntiCC1TrToy,"isFGD2NuMuBkInAntiCC1TrToy[nToys]/I");
    tree->Branch("isFGD2NuMuBkInAntiCCNTrNom",&isFGD2NuMuBkInAntiCCNTrNom,"isFGD2NuMuBkInAntiCCNTrNom/I");
    tree->Branch("isFGD2NuMuBkInAntiCCNTrToy",isFGD2NuMuBkInAntiCCNTrToy,"isFGD2NuMuBkInAntiCCNTrToy[nToys]/I");


      
    tree->Branch("anaToy",anaToy,"anaToy[nToys]/I");

    if(ntoys>nToys){
      std::cout<<" number of toys bigger than nToys "<<std::endl;
      return 0;
    }
      
    std::vector<float> weights;
    Weight_h totalweight;
    Weight_h fluxWeightSyst;
      
    //  ProfilerStart("prof.out");
    //--------- Loop over entries in the tree ----------------------------------
    // Get the number of entries in the tree
    nmax = std::min(nmax, (int)_man.input().GetEntries());
    std::cout << "RunSyst: loop over " << nmax << " entries" << std::endl;
    for (Long64_t entry=0; entry<nmax; entry++) {
      // 1. ====================================================================
      // Fill the event structure
      AnaEventB* event;
      if(preload)
        event=  _man.GetEvent(entry);
      else{
        event = static_cast<AnaEventB*>((_man.LoadSuperEvent(entry))->Event);
      }
      
      // Fill the EventBox
      if (!preload)
        _man.sel().InitializeEvent(*event);
      
      selmu_momNom=-999;

      isFGD1CCNom =0;
      isFGD1CC0PiNom  =0;
      isFGD1CC1PiNom  =0;
      isFGD1CCOtherNom=0;
      isFGD2CCNom =0;
      isFGD2CC0PiNom  =0;
      isFGD2CC1PiNom  =0;
      isFGD2CCOtherNom=0;

      isFGD1AntiCCNom =0;
      isFGD1AntiCC1TrNom  =0;
      isFGD1AntiCCNTrNom  =0;
      isFGD2AntiCCNom =0;
      isFGD2AntiCC1TrNom  =0;
      isFGD2AntiCCNTrNom  =0;

      isFGD1NuMuBkInAntiCCNom =0;
      isFGD1NuMuBkInAntiCC1TrNom  =0;
      isFGD1NuMuBkInAntiCCNTrNom  =0;
      isFGD2NuMuBkInAntiCCNom =0;
      isFGD2NuMuBkInAntiCC1TrNom  =0;
      isFGD2NuMuBkInAntiCCNTrNom  =0;

      // Run the  nominal selection
      bool passednom = _man.ProcessEvent(*event);      
      if(passednom){
	npassed[isyst]++;
        AnaEventSummaryB* summary = static_cast<AnaEventSummaryB*>(event->Summary);
        selmu_momNom=static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample])->Momentum;
        pileupweight=event->Weight;
        //std::cout<<" pos "<<summary->LeptonCandidate[summary->EventSample]->PositionStart[2]<<std::endl;
        if(summary->EventSample == SampleId::kFGD1NuMuCC0Pi){
          isFGD1CCNom     =1;
          isFGD1CC0PiNom  =1;
        }else if(summary->EventSample == SampleId::kFGD1NuMuCC1Pi){
          isFGD1CCNom     =1;
          isFGD1CC1PiNom  =1;
        }else if(summary->EventSample == SampleId::kFGD1NuMuCCOther){
          isFGD1CCNom     =1;
          isFGD1CCOtherNom=1;
        }
        else if(summary->EventSample == SampleId::kFGD2NuMuCC0Pi){
          isFGD2CCNom     =1;
          isFGD2CC0PiNom  =1;
        }else if(summary->EventSample == SampleId::kFGD2NuMuCC1Pi){
          isFGD2CCNom     =1;
          isFGD2CC1PiNom  =1;
        }else if(summary->EventSample == SampleId::kFGD2NuMuCCOther){
          isFGD2CCNom     =1;
          isFGD2CCOtherNom=1;
        }
	else if(summary->EventSample == SampleId::kFGD1AntiNuMuCC1Track){
          isFGD1AntiCCNom     =1;
          isFGD1AntiCC1TrNom  =1;
        }else if(summary->EventSample == SampleId::kFGD1AntiNuMuCCNTracks){
          isFGD1AntiCCNom     =1;
          isFGD1AntiCCNTrNom  =1;
        }
	else if(summary->EventSample == SampleId::kFGD2AntiNuMuCC1Track){
          isFGD2AntiCCNom     =1;
          isFGD2AntiCC1TrNom  =1;
        }else if(summary->EventSample == SampleId::kFGD2AntiNuMuCCNTracks){
          isFGD2AntiCCNom     =1;
          isFGD2AntiCCNTrNom  =1;
        }
	else if(summary->EventSample == SampleId::kFGD1NuMuBkgInAntiNuModeCC1Track){
          isFGD1NuMuBkInAntiCCNom     =1;
          isFGD1NuMuBkInAntiCC1TrNom  =1;
        }else if(summary->EventSample == SampleId::kFGD1NuMuBkgInAntiNuModeCCNTracks){
          isFGD1NuMuBkInAntiCCNom     =1;
          isFGD1NuMuBkInAntiCCNTrNom  =1;
        }
	else if(summary->EventSample == SampleId::kFGD2NuMuBkgInAntiNuModeCC1Track){
          isFGD2NuMuBkInAntiCCNom     =1;
          isFGD2NuMuBkInAntiCC1TrNom  =1;
        }else if(summary->EventSample == SampleId::kFGD2NuMuBkgInAntiNuModeCCNTracks){
          isFGD2NuMuBkInAntiCCNom     =1;
          isFGD2NuMuBkInAntiCCNTrNom  =1;
        }

      }
      
      // Enable the appropriate systematics
      if(event->GetIsMC()){
        _man.syst().DisableAllSystematics();
        for (UInt_t jsyst = 0; jsyst<NSYST;jsyst++){
          if(isyst!=NSYST-1 && jsyst!=isyst)continue;
          if(applySyst[jsyst] ){
            _man.syst().EnableSystematic(jsyst);
            ////for correct treatment of the charge,we must take into account the change
            ////in the momentum error due to the error on the resolution.
            ////the following code make show the influence of the momentum resolution on the charge id error
            /*
              if(jsyst==5){//charge id
              _man.syst().EnableSystematic(2);//momentum resol
              MomentumResolSystematics* mmm=static_cast< MomentumResolSystematics* >(_man.syst().GetSystematic(2));
              mmm->_applymomvar=false;
            */
          }
        }
      }

      // Create the SystBox array (only the first time it is called for each systematic)
      _man.syst().Initialize(_man.sel(),nmax);
      
      // Initialize The SystBox for variation systematics
      if (!preload)
        _man.syst().InitializeEventSystematics(_man.sel(),*event);

      /// 2. ====================================================================
      /// Loop over toy experiments
      for (UInt_t itoy= 0; itoy<ntoys;itoy++){

        anaToy[itoy]=itoy;	
        isFGD1CCToy[itoy]     =0;
        isFGD1CC0PiToy[itoy]  =0;
        isFGD1CC1PiToy[itoy]  =0;
        isFGD1CCOtherToy[itoy]=0;
        isFGD2CCToy[itoy]     =0;
        isFGD2CC0PiToy[itoy]  =0;
        isFGD2CC1PiToy[itoy]  =0;
        isFGD2CCOtherToy[itoy]=0;
	
	isFGD1AntiCCToy[itoy]     =0;
	isFGD1AntiCC1TrToy[itoy]  =0;
	isFGD1AntiCCNTrToy[itoy]  =0;
	isFGD2AntiCCToy[itoy]     =0;
	isFGD2AntiCC1TrToy[itoy]  =0;
	isFGD2AntiCCNTrToy[itoy]  =0;

	isFGD1NuMuBkInAntiCCToy[itoy]     =0;
	isFGD1NuMuBkInAntiCC1TrToy[itoy]  =0;
	isFGD1NuMuBkInAntiCCNTrToy[itoy]  =0;
	isFGD2NuMuBkInAntiCCToy[itoy]     =0;
	isFGD2NuMuBkInAntiCC1TrToy[itoy]  =0;
	isFGD2NuMuBkInAntiCCNTrToy[itoy]  =0;


        selmu_momToy[itoy]    =-999;
        weightsToy[itoy]      =-999;
        
        ToyExperiment* toy = toyMaker->GetToyExperiment(itoy);
     
        // 3. ====================================================================
        // Process the current event (bunch). That means applying the systematics, the selections and computing the weights 
        bool passed = _man.ProcessEvent(*toy,*event,totalweight,fluxWeightSyst);
        

        if(passed){
	  
          AnaEventSummaryB* summary = static_cast<AnaEventSummaryB*>(event->Summary);
          if(event->Summary && summary->EventSample){
            if (_man.GetFluxWeighting() && event->Summary){
              if (summary->TrueVertex[summary->EventSample]){
                fluxweight=_man.GetFluxWeighting()->GetWeight(summary->TrueVertex[summary->EventSample], anaUtils::GetRunPeriod(event->EventInfo.Run));	  
              }
            }
            selmu_momToy[itoy]=static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample])->Momentum;
          }
          weightsToy[itoy]=totalweight;

          if(summary->EventSample == SampleId::kFGD1NuMuCC0Pi){
            isFGD1CCToy[itoy]     =1;
            isFGD1CC0PiToy[itoy]  =1;
          }else if(summary->EventSample == SampleId::kFGD1NuMuCC1Pi){
            isFGD1CCToy[itoy]     =1;
            isFGD1CC1PiToy[itoy]  =1;
          }else if(summary->EventSample == SampleId::kFGD1NuMuCCOther){
            isFGD1CCToy[itoy]     =1;
            isFGD1CCOtherToy[itoy]=1;
          }
          else if(summary->EventSample == SampleId::kFGD2NuMuCC0Pi){
            isFGD2CCToy[itoy]     =1;
            isFGD2CC0PiToy[itoy]  =1;
          }else if(summary->EventSample == SampleId::kFGD2NuMuCC1Pi){
            isFGD2CCToy[itoy]     =1;
            isFGD2CC1PiToy[itoy]  =1;
          }else if(summary->EventSample == SampleId::kFGD2NuMuCCOther){
            isFGD2CCToy[itoy]     =1;
            isFGD2CCOtherToy[itoy]=1;
          }
	  else if(summary->EventSample == SampleId::kFGD1AntiNuMuCC1Track){
	    isFGD1AntiCCToy[itoy]     =1;
	    isFGD1AntiCC1TrToy[itoy]  =1;
	  }else if(summary->EventSample == SampleId::kFGD1AntiNuMuCCNTracks){
	    isFGD1AntiCCToy[itoy]     =1;
	    isFGD1AntiCCNTrToy[itoy]  =1;
	  }
	  else if(summary->EventSample == SampleId::kFGD2AntiNuMuCC1Track){
	    isFGD2AntiCCToy[itoy]     =1;
	    isFGD2AntiCC1TrToy[itoy]  =1;
	  }else if(summary->EventSample == SampleId::kFGD2AntiNuMuCCNTracks){
	    isFGD2AntiCCToy[itoy]     =1;
	    isFGD2AntiCCNTrToy[itoy]  =1;
	  }
	  else if(summary->EventSample == SampleId::kFGD1NuMuBkgInAntiNuModeCC1Track){
	    isFGD1NuMuBkInAntiCCToy[itoy]     =1;
	    isFGD1NuMuBkInAntiCC1TrToy[itoy]  =1;
	  }else if(summary->EventSample == SampleId::kFGD1NuMuBkgInAntiNuModeCCNTracks){
	    isFGD1NuMuBkInAntiCCToy[itoy]     =1;
	    isFGD1NuMuBkInAntiCCNTrToy[itoy]  =1;
	  }
	  else if(summary->EventSample == SampleId::kFGD2NuMuBkgInAntiNuModeCC1Track){
	    isFGD2NuMuBkInAntiCCToy[itoy]     =1;
	    isFGD2NuMuBkInAntiCC1TrToy[itoy]  =1;
	  }else if(summary->EventSample == SampleId::kFGD2NuMuBkgInAntiNuModeCCNTracks){
	    isFGD2NuMuBkInAntiCCToy[itoy]     =1;
	    isFGD2NuMuBkInAntiCCNTrToy[itoy]  =1;
	  }

        }
        
        
        if(event->GetIsMC()  && _man.syst().GetNEnabledVariationSystematics()>0)
          _man.syst().UndoVariationSystematics(*event);
      }
      
      tree->Fill();

      // Delete the SystBox for the current event
      if (!preload && _man.syst().GetNEnabledVariationSystematics()+_man.syst().GetNEnabledWeightSystematics()>0)
        _man.syst().FinalizeEventSystematics(*event);

      // The PreviousToyBox must be deleted after processing one event because 
      // it is only deleted internally when processing the next toy
      //      if (!preload)
      //        _man.sel().FinalizeEvent(*event);

      // save every 5000 entries
      if (entry%5000 == 0)
        tree->AutoSave("SaveSelf");
    }
    tree->AutoSave("SaveSelf");
    //    tree->Print();
  }
  //  outfile->Write();
  outfile->Close();
  delete outfile;
  
  gettimeofday(&tim, NULL);
  double t1=tim.tv_sec+(tim.tv_usec/1000000.0);
  
  std::cout << "time: " << t1-t0 << std::endl;
  
  _man.sel().PrintStatistics();
  
  //  ProfilerStop();
  for ( unsigned int i = 0; i<NSYST; i++ ){
	  std::cout << "# events passing selection, syst "<< i << ": " << npassed[i] << std::endl;
  }
}
