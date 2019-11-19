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

    std::string programName = argv[0];
    std::string paramFile = "";
    Int_t nmax_events = -1;
    UInt_t ntoys=1;
    std::string inputFileName = "";
    std::string inputFileType = "kHighlandTree";
    Int_t debug=0;

    for (;;) {
        int c = getopt(argc, argv, "n:t:d:");
        if (c < 0)
            break;
        switch (c) {
            case 'n': {
                          std::istringstream tmp(optarg);
                          tmp >> nmax_events;
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
            default: {
                     }
        }
    }

#ifndef MULTITHREAD
    std::cout << "For preloading mode you must uncomment the line #define MULTITHREAD in psycheCore/vXrY/srx/MultiThread.hxx and recompile everything !!!" << std::endl;
    return 0;
#endif

    TFile outfile("test_systematics.root","RECREATE");
    TH1F* weight_hist = new TH1F("Systematic weight","Systematic weight",10000,0,2);
    TH1F* var_hist = new TH1F("Systematic variation","Systematic momentum variation",100000,0,10000);
    //    std::vector<double>* systpull = new std::vector<double>[ntoys];

    // get the input file name
    inputFileName = argv[optind++];

    // Read the parameters files following the package hierarchy
    // first the top level package. Set the parameters as fixed
    ND::params().LoadParametersFiles(anaUtils::GetPackageHierarchy(), true);

    // Make sure no parameters have been accessed yet
    ND::params().SetReadParamOverrideFilePointPassed();

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



    timeval tim;
    gettimeofday(&tim, NULL);
    double t0=tim.tv_sec+(tim.tv_usec/1000000.0);

    // Preload nmax_events events from the file
    if (!_man.ReadEvents(inputFileName, nmax_events)) return 0;

    // Get the number of preloaded events
    Int_t nPreloadedEvents = _man.GetNPreloadedEvents();

    //    _man.SetNEventsToProcess(nPreloadedEvents);

    // Now get the amnount of memory in use after loading the events
    double full_mem = getValue();

    std::cout << "The program uses " << full_mem-initial_mem << "Kb of memory to read in the flat tree" << std::endl;

    TRandom3 rand(111);

    Float_t* mom = new Float_t[nPreloadedEvents*ntoys+1];
    Weight_h* weight = new Weight_h[nPreloadedEvents*ntoys+1];
    for(int j = 0; j<nPreloadedEvents; ++j){
        for(unsigned int k = 0; k < ntoys;  ++k){
            weight[j*ntoys + k] = -99999;
            mom[j*ntoys + k] = -1;
        }
    }
 
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
    /// Loop over toy experiments, setting each variation from a random draw of a Gaussian (mean 0, width 1)
    for (UInt_t itoy= 0; itoy<ntoys; itoy++){
        if((applyVariationSystematics || applyWeightSystematics || applyFluxWeightSystematic) ){
            for(unsigned int isyst = 0; isyst < nv; isyst++){
                SystematicVariationBase* tmp = varSyst[isyst];
                for (UInt_t ipar = 0;ipar<toy->GetNParameters(isyst);ipar++){
                    Float_t var;
                    if(tmp->PDF()==SystematicBase::kGaussian){
                        var=rand.Gaus(0.,1.);
                    }else if(tmp->PDF()==SystematicBase::kUniform){
                        var=rand.Uniform(0.,1.);
                    }
                    if(itoy==0)var = 0;
                    systpull[itoy].push_back(var);
                }
            }
            for(unsigned int isyst = 0; isyst < nw; isyst++){
                SystematicWeightBase* tmp = weightSyst[isyst];
                for (UInt_t ipar = 0;ipar<toy->GetNParameters(isyst+nv);ipar++){
                    Float_t var;
                    if(tmp->PDF()==SystematicBase::kGaussian){
                        var=rand.Gaus(0.,1.);
                    }else if(tmp->PDF()==SystematicBase::kUniform){
                        var=rand.Uniform(0.,1.);
                    }
                    if(itoy==0)var = 0;
                    systpull[itoy].push_back(var);
                }
            }
        }
    }
    */
    //--------- Loop over all preloaded events ----------------------------------

    Int_t npassed=0;

    for (UInt_t itoy= 0; itoy<ntoys; itoy++){
      /*
        int isy=0;
        if(applyVariationSystematics || applyWeightSystematics || applyFluxWeightSystematic){
            for (UInt_t isyst = 0; isyst<toy->GetNVariationSystematics()+toy->GetNWeightSystematics();isyst++){
                for (UInt_t ipar = 0;ipar<toy->GetNParameters(isyst);ipar++){
                    toy->SetVariation(isyst,ipar,systpull[itoy][isy]);
                    isy++;
                }
            }
        }
      */

        //  Gets the ToyExperiment with index itoy from the ToyMaker
        ToyExperiment* toy = toyMaker->GetToyExperiment(itoy);
#ifdef MULTITHREAD
        char* nCoresChar = getenv("OMP_NUM_THREADS");
        int nCores = 1;
        if (nCoresChar) nCores = atoi(nCoresChar);
        else{
          std::cout << "ERROR: To use MULTITHREAD mode OMP_NUM_THREADS environment variable must be set !!!!" << std::endl;
          exit(1);
        }
#define EPT nPreloadedEvents/nCores
#pragma omp parallel for num_threads(nCores) schedule(dynamic,EPT) reduction(+:npassed)
#endif
        for(int j = 0; j < nPreloadedEvents; ++j){
            // 1. ----------------------------------------------------------
            // Get the next event in the Experiment
            AnaEventB* event = _man.GetEvent(j);
	    //            _man.ProcessEvent(*event);

            Weight_h detectorWeight;
            Weight_h fluxWeightSyst;
            // Process the current event (bunch). That means applying the systematics, the selections and computing the weights 
            //  - Input:: the event and the toy experiment configuration (variations).
            //  - Output:: the detector systematic weight for the event, the flux weight for that event and whether any of the selections was passed (return bool)
            bool passed = _man.ProcessEvent((*toy),*event,detectorWeight,fluxWeightSyst);

            // increment number of events passing selection
            if (itoy==0 && passed) npassed++;

            // If event passed a selection fill a histogram with the event detector systematics weight
            AnaEventSummaryB* summary = static_cast<AnaEventSummaryB*>(event->Summary);
            if(summary->EventSample != SampleId::kUnassigned){
                weight[j*ntoys + itoy] = detectorWeight;
                mom[j*ntoys + itoy] = static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample])->Momentum;
            }
        }
    }
    for(int j = 0; j < nPreloadedEvents; ++j){
        for (UInt_t itoy= 0; itoy<ntoys; itoy++){
            if(weight[j*ntoys + itoy].Systematic > -99998){
                weight_hist->Fill(weight[j*ntoys + itoy].Systematic);
                var_hist->Fill(mom[j*ntoys + itoy]);
            }
        }
    }
    /*
    if(toy){
        delete toy;
        toy = NULL;
    }
    */
    outfile.cd();
    weight_hist->Write();
    var_hist->Write();

    outfile.Close();

    double final_mem = getValue();

    gettimeofday(&tim, NULL);
    double t2=tim.tv_sec+(tim.tv_usec/1000000.0);

    std::cout << "Time to preload events = " << t1-t0 << std::endl;
    std::cout << "Time to process events = " << t2-t1 << std::endl;
    std::cout << "The program used " << (final_mem-initial_mem)/1024 << "Mb of memory in total" << std::endl;

    //  ProfilerStop();
    _man.sel().PrintStatistics();
    std::cout << "# events passing selection: " << npassed << std::endl;
}
