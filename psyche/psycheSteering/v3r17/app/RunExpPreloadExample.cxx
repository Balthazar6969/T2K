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
#include "MultiThread.hxx"
#include <AnalysisUtils.hxx>

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

Experiment* getExperiment(){

    Experiment* exp = new Experiment("t2k"); 

    std::cout << "Processing run1 ..." << std::endl;


    DataSample* data1  = new DataSample("/neut/data21/mscott/T2K/BANFF_Inputs/Prod5_FlatTree_Data_Run1.root");
    DataSample* mc1    = new DataSample("/neut/data21/mscott/T2K/BANFF_Inputs/Prod5_Run1_WaterIn_BeamA_NeutMC_Splines.root");

    //    DataSample* sand1  = new DataSample("",1);

    SampleGroup run1("run1");
    run1.AddDataSample(data1);
    run1.AddMCSample("magnet",mc1);
    //    run1.AddMCSample("sand",sand1);  
    exp->AddSampleGroup("run1", run1); 

    /*
       std::cout << "Processing run2w ..." << std::endl;

       DataSample* data2w  = new DataSample("/hep/T2K/DataDir/MicroTrees/laura/data2w_5F_laura.root");
       DataSample* mc2w    = new DataSample("/hep/T2K/nd280rep/preProd6/v11r12/ANALYSIS/HIGHLAND/highland/numuCCQEAnalysis/v0r14/cmt/wsyst_laura/mc2w_5E_wsyst.root");
       DataSample* sand2w  = new DataSample("/hep/T2K/DataDir/MicroTrees/laura/mc_sandmu_laura.root",1);
       SampleGroup run2w("run2w");
       run2w.AddDataSample(data2w);
       run2w.AddMCSample("magnet",mc2w);
       run2w.AddMCSample("sand",sand2w);  
       exp->AddSampleGroup("run2w", run2w); 
    //  mc2w->SetCurrentTree("all_syst");


    std::cout << "Processing run2a ..." << std::endl;

    DataSample* data2a  = new DataSample("/hep/T2K/DataDir/MicroTrees/laura/data2a_5F_laura.root");
    //  DataSample* mc2a    = new DataSample("/hep/T2K/DataDir/MicroTrees/laura/mc2a_5E_laura.root");
    DataSample* mc2a    = new DataSample("/hep/T2K/nd280rep/preProd6/v11r12/ANALYSIS/HIGHLAND/highland/numuCCQEAnalysis/v0r14/cmt/wsyst_laura/mc2a_5E_wsyst.root");
    DataSample* sand2a  = new DataSample("/hep/T2K/DataDir/MicroTrees/laura/mc_sandmu_laura.root",1);
    SampleGroup run2a("run2a");
    run2a.AddDataSample(data2a);
    run2a.AddMCSample("magnet",mc2a);
    run2a.AddMCSample("sand",sand2a);  
    exp->AddSampleGroup("run2a", run2a); 
    //  mc2a->SetCurrentTree("all_syst");

    std::cout << "Processing run3 ..." << std::endl;

    DataSample* data3  = new DataSample("/hep/T2K/DataDir/MicroTrees/laura/data3_5F_laura.root");
    DataSample* mc3    = new DataSample("/hep/T2K/nd280rep/preProd6/v11r12/ANALYSIS/HIGHLAND/highland/numuCCQEAnalysis/v0r14/cmt/wsyst_laura/mc3_5E_wsyst.root");
    DataSample* sand3  = new DataSample("/hep/T2K/DataDir/MicroTrees/laura/mc_sandmu_laura.root",1);
    SampleGroup run3("run3");
    run3.AddDataSample(data3);
    run3.AddMCSample("magnet",mc3);
    run3.AddMCSample("sand",sand3);  
    exp->AddSampleGroup("run3", run3); 
    //  mc3->SetCurrentTree("all_syst");

    std::cout << "Processing run4w ..." << std::endl;

    DataSample* data4w  = new DataSample("/hep/T2K/DataDir/MicroTrees/laura/data4w_5F_laura.root");
    DataSample* mc4w    = new DataSample("/hep/T2K/nd280rep/preProd6/v11r12/ANALYSIS/HIGHLAND/highland/numuCCQEAnalysis/v0r14/cmt/wsyst_laura/mc4_5E_wsyst.root",4.915e20,"run>=90410000 && run<=90410005");
    DataSample* sand4w  = new DataSample("/hep/T2K/DataDir/MicroTrees/laura/mc_sandmu_laura.root",1);
    SampleGroup run4w("run4w");
    run4w.AddDataSample(data4w);
    run4w.AddMCSample("magnet",mc4w);
    run4w.AddMCSample("sand",sand4w);  
    exp->AddSampleGroup("run4w", run4w); 
    //  mc4w->SetCurrentTree("all_syst");


    std::cout << "Processing run4a ..." << std::endl;

    DataSample* data4a  = new DataSample("/hep/T2K/DataDir/MicroTrees/laura/data4a_5F_laura.root");
    DataSample* mc4a    = new DataSample("/hep/T2K/nd280rep/preProd6/v11r12/ANALYSIS/HIGHLAND/highland/numuCCQEAnalysis/v0r14/cmt/wsyst_laura/mc4_5E_wsyst.root",5.058e20,"run>90410005 && run<=90410016");
    DataSample* sand4a  = new DataSample("/hep/T2K/DataDir/MicroTrees/laura/mc_sandmu_laura.root",1);
    SampleGroup run4a("run4a");
    run4a.AddDataSample(data4a);
    run4a.AddMCSample("magnet",mc4a);
    run4a.AddMCSample("sand",sand4a);  
    exp->AddSampleGroup("run4a", run4a); 
    //  mc4a->SetCurrentTree("all_syst");

    std::cout << "Processing systematics ..." << std::endl;
    */
    return exp;
}

int main(int argc, char *argv[]){

    std::string programName = argv[0];
    std::string paramFile = "";
    Int_t nmax = -1;
    UInt_t ntoys=1;
    Int_t debug = 0;

    for (;;) {
        int c = getopt(argc, argv, "n:t:d:");
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
            default: {
                     }
        }
    }

#ifndef MULTITHREAD
    std::cout << "For preloading mode you must uncomment the line #define MULTITHREAD in psycheCore/vXrY/srx/MultiThread.hxx and recompile everything !!!" << std::endl;
    return 0;
#endif

    // Read the parameters files following the package hierarchy
    // first the top level package. Set the parameters as fixed
    ND::params().LoadParametersFiles(anaUtils::GetPackageHierarchy(), true);

    // Make sure no parameters have been accessed yet
    ND::params().SetReadParamOverrideFilePointPassed();

    // This the analysis manager
    AnalysisManager _man;

    // Make and fill the EventSummary even when the selection is not passed.
    if(ND::params().GetParameterI("psycheSteering.Selections.ForceFillEventSummary"))
        _man.sel().SetForceFillEventSummary(true);

    // Create the Experiment
    Experiment* exp = getExperiment();

    double initial_mem = getValue();

    // Set the Experiment into the Manager
    _man.SetExperiment(exp);

    // Dump information about the experiment
    exp->Dump();

    _man.SetNEventsToProcess(nmax);

    // Set a ToyMaker to configure the toy experiment. Initialize it with a random seed
    ToyMaker* toyMaker = new ToyMakerExample((UInt_t)ND::params().GetParameterI("psycheSteering.Systematics.RandomSeed"), 
                                             (bool)  ND::params().GetParameterI("psycheSteering.Systematics.ZeroVariation"));


    // Create and fill the Toy experiment with the appropriate format (number of systematics and number of parameters for each systematic)
    toyMaker->CreateToyExperiments(ntoys, _man.syst().GetSystematics()); 

    // Print the steps for the numuCC selection
    _man.sel().GetSelection("kTrackerNumuCC")->DumpSteps();

    // Initialize clock
    timeval tim;

    //  Gets the ToyExperiment with index itoy from the ToyMaker
    ToyExperiment* toy = toyMaker->GetToyExperiment(0);

    //--------- Loop over events in the Experiment ----------------------------------
    _man.PreloadEvents(false, false);

    gettimeofday(&tim, NULL);
    double t0=tim.tv_sec+(tim.tv_usec/1000000.0);

    _man.InitialiseMCTChain();
    int nMC = _man.GetNMCEvents();
    double* mc = new double[nMC];
    double* weight = new double[nMC];

    // Create the array of PreviousToyBox
    _man.sel().CreateToyBoxArray(nMC);

    // Create the array of SystBox
    _man.syst().Initialize(_man.sel(),nMC);

    UInt_t evt=0;
    Long64_t entry=0;
    Int_t npassed=0;
    while(evt<_man.GetNMCEvents() && entry<_man.GetEntries()){
        Weight_h totalWeightSyst;
        Weight_h fluxWeightSyst;

        // 1. ----------------------------------------------------------
        // Get the next event in the Experiment
        AnaSuperEventB* sevent = _man.LoadSuperEvent(entry);   
        if (!sevent) continue;
        AnaEventB* event = static_cast<AnaEventB*>(sevent->Event);    

        // Fill the EventBox
        _man.sel().InitializeEvent(*event);

        // Initialize The SystBox for variation systematics
        _man.syst().InitializeEventSystematics(_man.sel(),*event);
        
        // 2. ====================================================================
        // Process the current event (bunch). That means applying the systematics, the selections and computing the weights 
        //  - Input:: the event and the toy experiment configuration (variations).
        //  - Output:: the detector systematic weight for the event, the flux weight for that event and whether any of the selections was passed (return bool)

        if (  _man.ProcessEvent((*toy),*event,totalWeightSyst,fluxWeightSyst) ) {
		npassed++;
        }
        AnaEventSummaryB* summary = static_cast<AnaEventSummaryB*>(event->Summary);
        if(summary->EventSample != SampleId::kUnassigned){
          mc[evt] = static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample])->Momentum;
            weight[evt] = sevent->POTWeight;
        }
        else weight[evt] = 0;

        evt++;
    }	


    _man.InitialiseDataTChain();
    int nData = _man.GetNDataEvents();
    double* data = new double[nData];

    // Create the array of PreviousToyBox
    _man.sel().CreateToyBoxArray(nData);

    evt=0;
    entry=0;
    while(evt<_man.GetNDataEvents() && entry<_man.GetEntries()){

        Weight_h totalWeightSyst;
        Weight_h fluxWeightSyst;
        // 1. ----------------------------------------------------------
        // Get the next event in the Experiment
        AnaSuperEventB* sevent = _man.LoadSuperEvent(entry);   
        AnaEventB* event = static_cast<AnaEventB*>(sevent->Event);    

        // Fill the EventBox
        _man.sel().InitializeEvent(*event);

        // 2. ====================================================================
        // Process the current event (bunch). That means applying the systematics, the selections and computing the weights 
        //  - Input:: the event and the toy experiment configuration (variations).
        //  - Output:: the detector systematic weight for the event, the flux weight for that event and whether any of the selections was passed (return bool)

        _man.ProcessEvent((*toy),*event,totalWeightSyst,fluxWeightSyst);
        data[evt] = -999;
        AnaEventSummaryB* summary = static_cast<AnaEventSummaryB*>(event->Summary);
        if(summary->EventSample != SampleId::kUnassigned){
            data[evt] = static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample])->Momentum;
        }
        evt++;
    }

    TFile outfile("test_experiment.root","RECREATE");
    TH1F* data_hist = new TH1F("data_momentum","Selected data events",1000,0,10000);
    TH1F* mc_hist = new TH1F("mc_momentum","Selected MC events",1000,0,10000);


    for(int j = 0; j < (int)_man.GetNMCEvents(); ++j){
        mc_hist->Fill(mc[j],weight[j]);
    }
    for(int j = 0; j < (int)_man.GetNDataEvents(); ++j){
        if(data[j] > -900){
            data_hist->Fill(data[j]);
        }
    }
    if(toy){
        delete toy;
        toy = NULL;
    }

    outfile.cd();
    data_hist->Write();
    mc_hist->Write();

    outfile.Close();

    double final_mem = getValue();

    gettimeofday(&tim, NULL);
    double t2=tim.tv_sec+(tim.tv_usec/1000000.0);

    std::cout << "Time to run program = " << t2-t0 << std::endl;
    std::cout << "The program used " << (final_mem-initial_mem)/1024 << "Mb of memory in total" << std::endl;

    _man.sel().PrintStatistics();
    std::cout << "# events passing selection: " << npassed << std::endl;
}
