#include "AnalysisManager.hxx"
//#include <gperftools/profiler.h>
#include <sys/time.h>
#include <unistd.h>

#include <ToyMakerExample.hxx>
#include <Parameters.hxx>
#include <AnalysisUtils.hxx>


int main(int argc, char *argv[]){

    std::string programName = argv[0];
    std::string paramFile = "";
    int nmax_entries = 100000000;
    int nmax_events = -1;
    UInt_t ntoys=1;
    std::string inputFileName = "";
    std::string inputFileType = "";
    Int_t debug = 0;

    for (;;) {
        int c = getopt(argc, argv, "n:N:t:d:");
        if (c < 0)
            break;
        switch (c) {
            case 'n': {
                          std::istringstream tmp(optarg);
                          tmp >> nmax_events;
                          break;
                      }
            case 'N': {
                          std::istringstream tmp(optarg);
                          tmp >> nmax_entries;
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
    if(ND::params().GetParameterI("psycheSteering.Selections.ForceFillEventSummary"))
        _man.sel().SetForceFillEventSummary(true);

    // Initialize clock
    timeval tim;
    gettimeofday(&tim, NULL);
    double t0=tim.tv_sec+(tim.tv_usec/1000000.0);

    // Initialize the InputManager by specifying the input type and the input file
    if (!_man.input().Initialize(inputFileName,inputFileType, false)) return false;

    // Get the number of entries in the tree
    nmax_entries = std::min(nmax_entries, (int)_man.input().GetEntries());


    if (nmax_events==-1){ 
      // Get the numer of events 
      nmax_events=_man.input().GetNEvents(nmax_entries);
      std::cout << "RunExample: loop over " << nmax_entries << " entries" << std::endl;
    }
    else
      std::cout << "RunExample: loop over " << nmax_events << " events" << std::endl;

    // Print the steps for the different selections
    if (debug>0){
        _man.sel().GetSelection("kTrackerNumuCC")->DumpSteps();
        _man.sel().GetSelection("kTrackerNumuCCQE")->DumpSteps();
        _man.sel().GetSelection("kTrackerNumuCCMultiPi")->DumpSteps();

        _man.syst().DumpVariationSystematics();
        _man.syst().DumpWeightSystematics();
    }

    std::vector<Weight_h> totalWeightSyst;
    std::vector<Weight_h> fluxWeightSyst;

    // Set a ToyMaker to configure the toy experiment. Initialize it with a random seed
    ToyMaker* toyMaker = new ToyMakerExample((UInt_t)ND::params().GetParameterI("psycheSteering.Systematics.RandomSeed"), 
                                             (bool)  ND::params().GetParameterI("psycheSteering.Systematics.ZeroVariation"));

    // Create and fill the Toy experiment with the appropriate format (number of systematics and number of parameters for each systematic)
    toyMaker->CreateToyExperiments(ntoys, _man.syst().GetSystematics()); 

    // Create the array of PreviousToyBox
    _man.sel().CreateToyBoxArray(nmax_events);

    // Create the array of SystBox
    _man.syst().Initialize(_man.sel(),nmax_events);

    //  ProfilerStart("prof.out");
    //--------- Loop over entries in the tree ----------------------------------

    Long64_t entry=0;
    Int_t evt=0;
    Int_t npassed=0;
    while (entry<nmax_entries && evt < nmax_events){

        // 1. ====================================================================
        // Fill the event structure
        if (!_man.LoadEvent(entry)) continue;

        // Dump info about number of entries run
        if (entry%10000==0 || entry == nmax_entries)
          std::cout << "entry: " <<  entry  << " of " << nmax_entries << " (" << (100*entry/nmax_entries) << "%)" << std::endl;        

            // Get the event to be modified. This is the only one used in the analysis
        AnaEventB* event = static_cast<AnaEventB*>(_man.input().GetSuperEvent().Event);

        // Fill the EventBox
        _man.sel().InitializeEvent(*event);

        // Initialize The SystBox for variation systematics
        _man.syst().InitializeEventSystematics(_man.sel(),*event);

        // Toy experiments only for MC
        UInt_t ntoys2 = 1;
        if (event->GetIsMC()) ntoys2=ntoys;

        /// Loop over toy experiments
        for (UInt_t itoy= 0; itoy<ntoys2;itoy++){

          // 2. ====================================================================
          //  Gets the ToyExperiment with index itoy from the ToyMaker
          ToyExperiment* toy = toyMaker->GetToyExperiment(itoy);
          
          // 3. ====================================================================
          // Process the current event (bunch). That means applying the systematics, the selections and computing the weights 
          //  - Input:: the event, The toy experiment configuration (pdfBins), and the Systematics box (systbox)
          //  - Output:: the weight for each selection (weights), the summary event properties (prop) and wether any of the selections was passed (retoorn bool)
          //  - If selections are mutually exclusive only one of the weights should be not 0
          bool passed = _man.ProcessEvent(*toy,*event,totalWeightSyst,fluxWeightSyst);

          // increment number of events passing selection
          if (itoy==0 && passed) npassed++;
          
          // Stop looping over toys when the preselection is not passed
          if (!_man.sel().PreSelectionPassed(*event)) break;
          
          if (debug>0){
            if (passed){
              for (UInt_t i=0;i<totalWeightSyst.size();i++)
                std::cout << "weight, flux weight "<< i << ": " << totalWeightSyst[i] << ", " << fluxWeightSyst[i] << std::endl;
            }
            if (event->Summary && (debug>1 || passed)){
              //                    SampleId samples;
              std::cout << "candidate exists: " << (static_cast<AnaEventSummaryB*>(event->Summary)->LeptonCandidate[static_cast<AnaEventSummaryB*>(event->Summary)->EventSample] != NULL) 
                        << " , sample: " << SampleId::ConvertSample(static_cast<AnaEventSummaryB*>(event->Summary)->EventSample) << std::endl;
            }
          }
        }	

        // Delete the SystBox array
        _man.syst().FinalizeEventSystematics(*event);

        // Reset the PreviousToyBox
        _man.sel().FinalizeEvent(*event);

        evt++;
    }

    std::cout << "RunExample: " << evt << " events processed" << std::endl;
    
    gettimeofday(&tim, NULL);
    double t1=tim.tv_sec+(tim.tv_usec/1000000.0);

    std::cout << "time: " << t1-t0 << std::endl;

    _man.sel().PrintStatistics();
    std::cout << "# events passing selection: " << npassed << std::endl;
    
    //  ProfilerStop();

}
