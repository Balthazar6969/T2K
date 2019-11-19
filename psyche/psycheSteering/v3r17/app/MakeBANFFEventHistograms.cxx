#include "AnalysisManager.hxx"
//#include <gperftools/profiler.h>
#include <sys/time.h>
#include <iostream>
#include <unistd.h>
#include "sys/types.h"
//#include "sys/sysinfo.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "Parameters.hxx"
#include "ToyMakerExample.hxx"
#include "TRandom3.h"
#include "TH2F.h"
#include "TH1F.h"
#include "MultiThread.hxx"
#include <AnalysisUtils.hxx>

#ifdef MULTITHREAD
#include "omp.h"
#endif 

int parseLine(char* line){
    int i = strlen(line);
    while (*line < '0' || *line > '9') line++;
    line[i-3] = '\0';
    i = atoi(line);
    return i;
}


int getValue(){ //Note: this value is in KB!
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];
    if (!file) return 0;

    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmSize:", 7) == 0){
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result;
}

int main(int argc, char *argv[]){

    // Usage:  MakeBANFFEventHistograms.exe -n N (preloads N events) -d 1/0 (swith on debugging mode, default off) -p X (X = Data/MC POT ratio to apply, default to 1) -b 1/0 (normalise by bin area in histograms, default off) -o outputfilename /path/to/input/file

    std::string programName = argv[0];
    std::string paramFile = "";
    Int_t nmax = -1;
    UInt_t ntoys=1;
    std::string inputFileName = "";
    std::string inputFileType = "kHighlandTree";
    Int_t debug=0;
    double pot_ratio = 1;
    bool normalise = 0;
    const char* outfilename = NULL;

    for (;;) {
        int c = getopt(argc, argv, "n:o:d:p:b:");
        if (c < 0)
            break;
        switch (c) {
            case 'n': {
                          std::istringstream tmp(optarg);
                          tmp >> nmax;
                          break;
                      }
            case 'o': {
                          outfilename = optarg;
                          break;
                      }
            case 'd': {
                          std::istringstream tmp(optarg);
                          tmp >> debug;
                          break;
                      }
            case 'p': {
                          std::istringstream tmp(optarg);
                          tmp >> pot_ratio;
                          break;
                      }
            case 'b': {
                          std::istringstream tmp(optarg);
                          tmp >> normalise;
                          break;
                      }
            default: {
                     }
        }
    }

    // Read the parameters files following the package hierarchy
    // first the top level package. Set the parameters as fixed
    ND::params().LoadParametersFiles(anaUtils::GetPackageHierarchy(), true);

    // Make sure no parameters have been accessed yet
    ND::params().SetReadParamOverrideFilePointPassed();

    //Momentum bins
    //CC0pi
    int npbins_0pi = 14;
    double pbins_0pi[15] = {0, 300, 400, 500, 600, 700, 800, 900, 1000, 1250, 1500, 2000, 3000, 5000, 8000};
    //CC1pi
    int npbins_1pi = 13;
    double pbins_1pi[14] = {0, 300, 400, 500, 600, 700, 800, 900, 1000, 1250, 1500, 2000, 5000, 8000};
    //CCOther
    int npbins_npi = 14;
    double pbins_npi[15] = {0, 300, 400, 500, 600, 700, 800, 900, 1000, 1250, 1500, 2000, 3000, 5000, 8000};

    //cos(theta) bins
    //CC0pi
    int nctbins_0pi = 11;
    double ctbins_0pi[12] = {0, 0.6, 0.7, 0.8, 0.85, 0.9, 0.92, 0.94, 0.96, 0.98, 0.99, 1};
    //CC1pi
    int nctbins_1pi = 11;
    double ctbins_1pi[12] = {0, 0.6, 0.7, 0.8, 0.85, 0.9, 0.92, 0.94, 0.96, 0.98, 0.99, 1};
    //CCOther
    int nctbins_npi = 11;
    double ctbins_npi[12] = {0, 0.6, 0.7, 0.8, 0.85, 0.9, 0.92, 0.94, 0.96, 0.98, 0.99, 1};

    int npbins = 5;
    double pbins[6] = {0, 400, 500, 700, 900, 4000};

    int nctbins = 4;
    double ctbins[5] = {0, 0.84, 0.9, 0.94, 1};

    // get the input file name
    inputFileName = argv[optind++];

    double ccqe_weight[25];
    for(int i = 0; i < 25; ++i){
        ccqe_weight[i] = 1;
    }

// Uncomment this section if you want to apply weights from the BANFF input spline files
    TFile infile(inputFileName.c_str(),"READ");
    TTree* sample_sum;
    bool hasSampleSum=false;
    if(infile.Get("sample_sum")){
      sample_sum= (TTree*) infile.Get("sample_sum");
      sample_sum->SetBranchAddress("CCQETuningWeight",ccqe_weight);
      hasSampleSum=true;
    }
    if(outfilename==NULL) outfilename = "BANFF_SelectedEvent_Histograms.root";
    TFile outfile(outfilename,"RECREATE");

    // Make neutrino mode histograms
    TH2F* tuned_cc0pi = new TH2F("tuned_cc0pi","tuned_cc0pi",1000,0,8000,1000,0,1); 
    TH2F* tuned_cc1pi = new TH2F("tuned_cc1pi","tuned_cc1pi",1000,0,8000,1000,0,1); 
    TH2F* tuned_ccnpi = new TH2F("tuned_ccNpi","tuned_ccNpi",1000,0,8000,1000,0,1); 
    // Make anti-neutrino histograms
    TH2F* tuned_ccAnu0pi = new TH2F("tuned_ccAnu0pi","tuned_ccAnu0pi",1000,0,4000,1000,0,1); 
    TH2F* tuned_ccAnuNpi = new TH2F("tuned_ccAnuNpi","tuned_ccAnuNpi",1000,0,4000,1000,0,1); 
    // Make anti-neutrino mode wrong sign histograms
    TH2F* tuned_ccNu0pi = new TH2F("tuned_ccNu0pi","tuned_ccNu0pi",1000,0,4000,1000,0,1); 
    TH2F* tuned_ccNuNpi = new TH2F("tuned_ccNuNpi","tuned_ccNuNpi",1000,0,4000,1000,0,1); 

    tuned_cc0pi->SetBins(npbins_0pi, pbins_0pi, nctbins_0pi, ctbins_0pi);
    tuned_cc1pi->SetBins(npbins_1pi, pbins_1pi, nctbins_1pi, ctbins_1pi);
    tuned_ccnpi->SetBins(npbins_npi, pbins_npi, nctbins_npi, ctbins_npi);
    tuned_ccAnu0pi->SetBins(npbins, pbins, nctbins, ctbins);
    tuned_ccAnuNpi->SetBins(npbins, pbins, nctbins, ctbins);
    tuned_ccNu0pi->SetBins(npbins, pbins, nctbins, ctbins);
    tuned_ccNuNpi->SetBins(npbins, pbins, nctbins, ctbins);
  
    TTree *tree=new TTree("tree","tree");
    Float_t selmu_mom=-999;
    Float_t selmu_costheta=-999;
    Weight_h niwgW=-999;
    Weight_h detW=-999;
    Weight_h fluxW=-999;
    Float_t potW=-999;
    Weight_h pileupW=-999;
    Int_t   isFGD1NuMuCC=-1;
    Int_t   isFGD1NuMuCC0Pi=-1;
    Int_t   isFGD1NuMuCC1Pi=-1;
    Int_t   isFGD1NuMuCCOther=-1;
    Int_t   isFGD1AntiNuMuCC=-1;
    Int_t   isFGD1AntiNuMuCC1Track=-1;
    Int_t   isFGD1AntiNuMuCCNTracks=-1;
    Int_t   isFGD1NuMuBkgInAntiNuMuModeCC=-1;
    Int_t   isFGD1NuMuBkgInAntiNuMuModeCC1Track=-1;
    Int_t   isFGD1NuMuBkgInAntiNuMuModeCCNTracks=-1;

    tree->Branch("selmu_mom",&selmu_mom,"selmu_mom/F");
    tree->Branch("selmu_costheta",&selmu_mom,"selmu_costheta/F");
    tree->Branch("niwgW",&niwgW,"niwgW/F");
    tree->Branch("detW",&detW,"detW/F");
    tree->Branch("fluxW",&fluxW,"fluxW/F");
    tree->Branch("potW",&potW,"potW/F");
    tree->Branch("pileupW",&pileupW,"pileupW/F");

    tree->Branch("isFGD1NuMuCC",     &isFGD1NuMuCC,     "isFGD1NuMuCC/I");
    tree->Branch("isFGD1NuMuCC0Pi",  &isFGD1NuMuCC0Pi,  "isFGD1NuMuCC0Pi/I");
    tree->Branch("isFGD1NuMuCC1Pi",  &isFGD1NuMuCC1Pi,  "isFGD1NuMuCC1Pi/I");
    tree->Branch("isFGD1NuMuCCOther",&isFGD1NuMuCCOther,"isFGD1NuMuCCOther/I");

    tree->Branch("isFGD1AntiNuMuCC",     &isFGD1AntiNuMuCC,     "isFGD1AntiNuMuCC/I");
    tree->Branch("isFGD1AntiNuMuCC1Track",   &isFGD1AntiNuMuCC1Track,   "isFGD1AntiNuMuCC1Track/I");
    tree->Branch("isFGD1AntiNuMuCCNTracks",  &isFGD1AntiNuMuCCNTracks,  "isFGD1AntiNuMuCCNTracks/I");

    tree->Branch("isFGD1NuMuBkgInAntiNuMuModeCC",     &isFGD1NuMuBkgInAntiNuMuModeCC,     "isFGD1NuMuBkgInAntiNuMuModeCC/I");
    tree->Branch("isFGD1NuMuBkgInAntiNuMuModeCC1Track",   &isFGD1NuMuBkgInAntiNuMuModeCC1Track,   "isFGD1NuMuBkgInAntiNuMuModeCC1Track/I");
    tree->Branch("isFGD1NuMuBkgInAntiNuMuModeCCNTracks",  &isFGD1NuMuBkgInAntiNuMuModeCCNTracks,  "isFGD1NuMuBkgInAntiNuMuModeCCNTracks/I");

  

    // This the analysis manager
    AnalysisManager _man;

    // Make and fill the EventSummary even when the selection is not passed.
    if(ND::params().GetParameterI("psycheSteering.Selections.ForceFillEventSummary")) _man.sel().SetForceFillEventSummary(true);
    // Parameters to control the systematics
    //    bool applyVariationSystematics = (bool)ND::params().GetParameterI("psycheSteering.Systematics.ApplyVariationSystematics");
    //    bool applyWeightSystematics    = (bool)ND::params().GetParameterI("psycheSteering.Systematics.ApplyWeightSystematics");
    //    bool applyFluxWeightSystematic = (bool)ND::params().GetParameterI("psycheSteering.Systematics.ApplyFluxWeightSystematic");

    // Get the initial amount of memory being used
    double initial_mem = getValue();

    // Print the steps for the numuCC selection
    _man.sel().GetSelection("kTrackerNumuCC")->DumpSteps();

    // Create the Toy experiment with the appropriate format (number of systematics and number of parameters for each systematic)
    ToyMaker* toyMaker = new ToyMakerExample((UInt_t)ND::params().GetParameterI("psycheSteering.Systematics.RandomSeed"), 
                                             (bool)  ND::params().GetParameterI("psycheSteering.Systematics.ZeroVariation"));

    // Create and fill the Toy experiment with the appropriate format (number of systematics and number of parameters for each systematic)
    toyMaker->CreateToyExperiments(ntoys, _man.syst().GetSystematics()); 

    //  Gets the ToyExperiment with index itoy from the ToyMaker. Just the first toy
    ToyExperiment* toy = toyMaker->GetToyExperiment(0);

  
    TRandom3 rand(111);
    timeval tim;
    gettimeofday(&tim, NULL);
    double t0=tim.tv_sec+(tim.tv_usec/1000000.0);

    // Initialize clock
    gettimeofday(&tim, NULL);
    double t1=tim.tv_sec+(tim.tv_usec/1000000.0);

    /*
    int n;
    SystematicVariationBase** varSyst = _man.syst().GetVariationSystematics(n);
    SystematicWeightBase** weightSyst = _man.syst().GetWeightSystematics(n);
    UInt_t nw=_man.syst().GetNWeightSystematics();
    UInt_t nv=_man.syst().GetNVariationSystematics();
    for(unsigned int i = 0; i < nw; i++){
        SystematicWeightBase* tmp = weightSyst[i];
        tmp->SetPDF(SystematicBase::kGaussian);
    }
    for(unsigned int i = 0; i < nv; i++){
        SystematicVariationBase* tmp = varSyst[i];
        if(tmp->Name()=="kBFieldDist")
            tmp->SetPDF(SystematicBase::kUniform);
        else
            tmp->SetPDF(SystematicBase::kGaussian);
    }


    ToyExperiment* toy = _man.syst().CreateToyExperiment(); 
    std::cout<<" nsyst     "<<toy->GetNSystematics()<<std::endl;
    std::cout<<" nsyst var "<<toy->GetNVariationSystematics()<<std::endl;
    std::cout<<" nsyst wei "<<toy->GetNWeightSystematics()<<std::endl;

    if(applyVariationSystematics || applyWeightSystematics || applyFluxWeightSystematic){
        for (UInt_t isyst = 0; isyst<toy->GetNSystematics();isyst++){
            for (UInt_t ipar = 0;ipar<toy->GetNParameters(isyst);ipar++){
                toy->SetVariation(isyst,ipar,0);
            }
        }
    }
    */
    double POT_weight = pot_ratio;


    // Preload nmax events from the file
    if (!_man.ReadEvents(inputFileName, nmax)) return 0;

    if(nmax < 0) nmax = _man.GetEntries();

    _man.SetNEventsToProcess(nmax);

    // Now get the amnount of memory in use after loading the events
    double full_mem = getValue();

    std::cout << "The program uses " << full_mem-initial_mem << "Kb of memory to read in the flat tree" << std::endl;
    
    float* mom = new float[nmax*ntoys+1];
    Weight_h* weight = new Weight_h[nmax*ntoys+1];
    for(int j = 0; j<nmax; ++j){
        for(unsigned int k = 0; k < ntoys;  ++k){
            weight[j*ntoys + k] = -99999;
            mom[j*ntoys + k] = -1;
        }
    }


#ifdef MULTITHREAD
    int nCores = atoi(getenv("OMP_NUM_THREADS"));
#define EPT nmax/nCores
#pragma omp parallel for num_threads(nCores) schedule(dynamic,EPT)
#endif
    for(int j = 0; j < nmax; ++j){
        // 1. ----------------------------------------------------------
        // Get the next event in the Experiment
      if(hasSampleSum)
        sample_sum->GetEvent(j);
      AnaEventB* event = _man.GetEvent(j);
      _man.ProcessEvent(*event);

        Weight_h detectorWeight;
        Weight_h fluxWeightSyst;
        // Process the current event (bunch). That means applying the systematics, the selections and computing the weights 
        //  - Input:: the event and the toy experiment configuration (variations).
        //  - Output:: the detector systematic weight for the event, the flux weight for that event and whether any of the selections was passed (return bool)
        _man.ProcessEvent((*toy),*event,detectorWeight,fluxWeightSyst);

        // If event passed a selection fill a histogram with the event detector systematics weight
        AnaEventSummaryB* summary = static_cast<AnaEventSummaryB*>(event->Summary);
        if(summary->EventSample != SampleId::kUnassigned){
          double tune_weight = ccqe_weight[summary->EventSample];
          potW=POT_weight;
          detW=detectorWeight;
          fluxW=fluxWeightSyst;
          pileupW=event->Weight;
          niwgW=tune_weight;
          isFGD1NuMuCC=0;
          isFGD1NuMuCC0Pi=0;
          isFGD1NuMuCC1Pi=0;
          isFGD1NuMuCCOther=0;
          isFGD1AntiNuMuCC=0;
          isFGD1AntiNuMuCC1Track=0;
          isFGD1AntiNuMuCCNTracks=0;
          isFGD1NuMuBkgInAntiNuMuModeCC=0;
          isFGD1NuMuBkgInAntiNuMuModeCC1Track=0;
          isFGD1NuMuBkgInAntiNuMuModeCCNTracks=0;
          AnaParticleMomB* leptonCandidate = static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample]);
          selmu_mom=leptonCandidate->Momentum;
          selmu_costheta=leptonCandidate->DirectionStart[2];
          if(summary->EventSample == SampleId::kFGD1NuMuCC0Pi){
            isFGD1NuMuCC=1;
            isFGD1NuMuCC0Pi=1;
            tuned_cc0pi->Fill(leptonCandidate->Momentum, leptonCandidate->DirectionStart[2], (detectorWeight*fluxWeightSyst*tune_weight*POT_weight*pileupW).Systematic);
          }else if(summary->EventSample == SampleId::kFGD1NuMuCC1Pi){
            isFGD1NuMuCC=1;
            isFGD1NuMuCC1Pi=1;
            tuned_cc1pi->Fill(leptonCandidate->Momentum, leptonCandidate->DirectionStart[2], (detectorWeight*fluxWeightSyst*tune_weight*POT_weight*pileupW).Systematic);
          }else if(summary->EventSample == SampleId::kFGD1NuMuCCOther){
            isFGD1NuMuCC=1;
            isFGD1NuMuCCOther=1;
            tuned_ccnpi->Fill(leptonCandidate->Momentum, leptonCandidate->DirectionStart[2], (detectorWeight*fluxWeightSyst*tune_weight*POT_weight*pileupW).Systematic);
          }else if(summary->EventSample == SampleId::kFGD1AntiNuMuCC1Track){
            isFGD1AntiNuMuCC=1;
            isFGD1AntiNuMuCC1Track=1;
            tuned_ccAnu0pi->Fill(leptonCandidate->Momentum, leptonCandidate->DirectionStart[2], (detectorWeight*fluxWeightSyst*tune_weight*POT_weight*pileupW).Systematic);
          }else if(summary->EventSample == SampleId::kFGD1AntiNuMuCCNTracks){
            isFGD1AntiNuMuCC=1;
            isFGD1AntiNuMuCCNTracks=1;
            tuned_ccAnuNpi->Fill(leptonCandidate->Momentum, leptonCandidate->DirectionStart[2], (detectorWeight*fluxWeightSyst*tune_weight*POT_weight*pileupW).Systematic);
          }else if(summary->EventSample == SampleId::kFGD1NuMuBkgInAntiNuModeCC1Track){
            isFGD1NuMuBkgInAntiNuMuModeCC=1;
            isFGD1NuMuBkgInAntiNuMuModeCC1Track=1;
            tuned_ccNu0pi->Fill(leptonCandidate->Momentum, leptonCandidate->DirectionStart[2], (detectorWeight*fluxWeightSyst*tune_weight*POT_weight*pileupW).Systematic);
          }else if(summary->EventSample == SampleId::kFGD1NuMuBkgInAntiNuModeCCNTracks){
            isFGD1NuMuBkgInAntiNuMuModeCC=1;
            isFGD1NuMuBkgInAntiNuMuModeCCNTracks=1;
            tuned_ccNuNpi->Fill(leptonCandidate->Momentum, leptonCandidate->DirectionStart[2], (detectorWeight*fluxWeightSyst*tune_weight*POT_weight*pileupW).Systematic);
          }
        }
        tree->Fill();
    }
    /*
    if(toy){
      delete toy;
      toy = NULL;
    }
    */
    outfile.cd();
    
    for(int i = 1; i <= tuned_cc0pi->GetNbinsX(); ++i){
      double p_width = 1;
        double q_width = 1;
        if(normalise) p_width = (pbins_0pi[i] - pbins_0pi[i-1])/50.0;
        for(int j = 1; j <= tuned_cc0pi->GetNbinsY(); ++j){
            if(normalise) q_width = (ctbins_0pi[j] - ctbins_0pi[j-1])/0.05;
            double contents = tuned_cc0pi->GetBinContent(i, j);
            tuned_cc0pi->SetBinContent(i, j, contents/(q_width*p_width));
        }
    }

    for(int i = 1; i <= tuned_cc1pi->GetNbinsX(); ++i){
        double p_width = 1;
        double q_width = 1;
        if(normalise) p_width = (pbins_1pi[i] - pbins_1pi[i-1])/50.0;
        for(int j = 1; j <= tuned_cc1pi->GetNbinsY(); ++j){
            if(normalise) q_width = (ctbins_1pi[j] - ctbins_1pi[j-1])/0.05;
            double contents = tuned_cc1pi->GetBinContent(i, j);
            tuned_cc1pi->SetBinContent(i, j, contents/(q_width*p_width));
        }
    }

    for(int i = 1; i <= tuned_ccnpi->GetNbinsX(); ++i){
        double p_width = 1;
        double q_width = 1;
        if(normalise) p_width = (pbins_npi[i] - pbins_npi[i-1])/50.0;
        for(int j = 1; j <= tuned_ccnpi->GetNbinsY(); ++j){
            if(normalise) q_width = (ctbins_npi[j] - ctbins_npi[j-1])/0.05;
            double contents = tuned_ccnpi->GetBinContent(i, j);
            tuned_ccnpi->SetBinContent(i, j, contents/(q_width*p_width));
        }
    }

    for(int i = 1; i <= tuned_ccAnu0pi->GetNbinsX(); ++i){
        double p_width = 1;
        double q_width = 1;
        if(normalise) p_width = (pbins[i] - pbins[i-1])/50.0;
        for(int j = 1; j <= tuned_ccAnu0pi->GetNbinsY(); ++j){
            if(normalise) q_width = (ctbins[j] - ctbins[j-1])/0.05;
            double contents = tuned_ccAnu0pi->GetBinContent(i, j);
            tuned_ccAnu0pi->SetBinContent(i, j, contents/(q_width*p_width));

            contents = tuned_ccAnuNpi->GetBinContent(i, j);
            tuned_ccAnuNpi->SetBinContent(i, j, contents/(q_width*p_width));

            contents = tuned_ccNu0pi->GetBinContent(i, j);
            tuned_ccNu0pi->SetBinContent(i, j, contents/(q_width*p_width));

            contents = tuned_ccNuNpi->GetBinContent(i, j);
            tuned_ccNuNpi->SetBinContent(i, j, contents/(q_width*p_width));
        }
    }



    tuned_cc0pi->Write();
    tuned_cc1pi->Write();
    tuned_ccnpi->Write();
    tuned_ccAnu0pi->Write();
    tuned_ccAnuNpi->Write();
    tuned_ccNu0pi->Write();
    tuned_ccNuNpi->Write();
    tree->Write();
    outfile.Close();

    double final_mem = getValue();

    gettimeofday(&tim, NULL);
    double t2=tim.tv_sec+(tim.tv_usec/1000000.0);

    std::cout << "Time to preload events = " << t1-t0 << std::endl;
    std::cout << "Time to process events = " << t2-t1 << std::endl;
    std::cout << "The program used " << (final_mem-initial_mem)/1024 << "Mb of memory in total" << std::endl;

    //  ProfilerStop();
    _man.sel().PrintStatistics();
}
