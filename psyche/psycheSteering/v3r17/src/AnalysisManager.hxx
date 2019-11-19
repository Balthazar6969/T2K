#ifndef AnalysisManager_hxx_seen
#define AnalysisManager_hxx_seen


#include "SystematicManager.hxx"
#include "EventWeightManager.hxx"
#include "EventVariationManager.hxx"
#include "InputManager.hxx"
#include "SelectionManager.hxx"
#include "Experiment.hxx"
#include "FluxWeighting.hxx"


typedef std::string SelEnum;
typedef std::string SystEnum;

/// This class is the Top level interface for the library. 


class AnalysisManager{

public:
  
  AnalysisManager();
  virtual ~AnalysisManager(){}

  void DefineProduction();
  void DefineInputConverters();
  void DefineSelections();
  void DefineSystematics();

  /*
  enum SystEnum{
    kMomScale = 0,
    kMomResol,
    kChargeConf,
    kTpcTrackEff
  };

  enum SelEnum{
    kTrackerNumuCC = 0,
  };


  enum InputEnum{
    kHighlandTree = 0,
    kP0dTree
  };
  */

  /// Returns the systematic manager
  SystematicManager& syst(){return _syst;}

  /// Returns the EventWeight manager
  EventWeightManager& eweight(){return _weight;}

  /// Returns the EventVariation manager
  EventVariationManager& evar(){return _var;}

  /// Returns the input manager
  InputManager& input(){return _input;}

  /// Returns the selection manager
  SelectionManager& sel(){return _sel;}

  /// Sets the Experiment
  void SetExperiment(Experiment* exp){_exp=exp;}

  /// Process one event. The input is the event. The output is a boolean (return value) 
  /// telling us whether at least one selection was passes for the current event, and a vector of Weight_hs 
  /// with the weights for each selection
  bool ProcessEvent(const ToyExperiment& toy, AnaEventB& event, std::vector<Weight_h>& totalWeightSystVector, std::vector<Weight_h>& fluxWeightSystVector);
  bool ProcessEvent(const ToyExperiment& toy, AnaEventB& event, std::vector<Weight_h>& totalWeightSystVector, std::vector<Weight_h>& fluxWeightSystVector, Float_t& POTweight);
  bool ProcessEvent(AnaEventB& event, Float_t& POTWeight);
  bool ProcessEvent(AnaEventB& event);

  /// Process one event method for use in BANFF and MaCh3 fitters
  bool ProcessEvent(const ToyExperiment& toy, AnaEventB& event, Weight_h& totalWeight,Weight_h& FluxWeightSyst);

  /// Create the evevt array for preloading events
  void CreateEventArray(Int_t nmax);

  /// Read the specified number of events from the file and save them into an array. Default(-1) is all events
  bool ReadEvents(const std::string& inputFile, Int_t nmax=-1);

  /// Read the specified number of events from the current DataSample and save them into an array. Default(-1) is all events
  bool ReadEvents(Int_t nmax=-1);

  /// Read the specified number of events from the DataSample and save them into an array. Default(-1) is all events
  bool ReadEvents(DataSample* sample, Int_t nmax=-1);

  /// Set the maximum number of events to process
  void SetNEventsToProcess(Int_t nmax){_nEventsToProcess=nmax;}

  /// Returns The SuperEvent with a given index from the array
  AnaSuperEventB* GetSuperEvent(Int_t eventIndex){if (eventIndex>=(Int_t)_nEventsInArray) return NULL; return _eventArray[eventIndex];}

  /// Returns The Event with a given index from the array
  AnaEventB* GetEvent(Int_t eventIndex);
  
  /// Returns The next SuperEvent
  AnaSuperEventB* GetNextSuperEvent();

  /// Returns The next Event
  AnaEventB* GetNextEvent();
  
  /// Returns the number of entries in the input tree
  UInt_t GetEntries();
  

  /// Method to load a given single event, without pre-loading.
  /// Fill the raw AnaEventB structure. 
  /// The argument entry (the entry number in the input tree) 
  /// will be modified inside the method by the specific converter (hence the non-cont reference). 
  /// The way entry is modified will depend on whether the input file is event based 
  /// (every call to this method will increment by one the argument entry) or 
  /// spill based (several events -- one per bunch -- correspond to the same spill, so 
  /// once the first event in a given spill is read entry number will not be varied 
  /// until all events in that spill are processed).
  /// Returns whether the event was successfully filled.
  bool LoadEvent(Long64_t& entry);

  void PreloadEvents(bool preloadData = 1, bool preloadMC = 1);
  bool PreloadMCEvents(std::vector<std::string> inputFiles, std::vector<float> POT_weights);
  bool PreloadDataEvents(std::vector<std::string> inputFiles);
  AnaSuperEventB* GetPreloadedMCSuperEvent(int i);
  AnaSuperEventB* GetPreloadedDataSuperEvent(int i);
  void CreateMCEventArray(Int_t nmax);
  void CreateDataEventArray(Int_t nmax);
  UInt_t GetNMCEvents(){return _nEventsInMCArray;}
  UInt_t GetNDataEvents(){return _nEventsInDataArray;}
  bool Initialize(Int_t nmax);
  void InitialiseDataTChain();
  void InitialiseMCTChain();

  void ResetEventsProcessed();

  /// Fill the raw AnaEventB structure and create a AnaSuperEventB from 
  /// the raw event (all done by InputManagerB calls).  
  /// The input entry number will be modified internally 
  /// by the specific converter (hence the non-cont reference) . 
  /// This will depend on whether the input file is spill based or event based. 
  AnaSuperEventB* LoadSuperEvent(Long64_t& evtIndex);

  FluxWeighting* GetFluxWeighting(){return _flux;}

  /// Check to see whether the event is from the right run period for the selection
  /// This is important for making sure we do not run the anti-neutrino mode selections
  /// on neutrino mode data
  bool CheckSelectionAgainstRunPeriod(SelectionBase* selec, int RunPeriod);
  
  /// Return the number of preloaded events
  UInt_t GetNPreloadedEvents() const {return _nEventsInArray;}

protected:

  /// An instance of the systematic manager
  SystematicManager _syst;

  /// An instance of the EventWeight manager
  EventWeightManager _weight;

  /// An instance of the EventVariation manager
  EventVariationManager _var;

  /// An instance of the input manager
  InputManager      _input;

  /// An instance of the selection manager
  SelectionManager  _sel;

  /// The array of events
  AnaSuperEventB** _eventArray;

  /// the number of events in the array
  UInt_t _nEventsInArray;

  /// whether to apply detector event weight or not
  bool _applyEventWeights;

  /// whether to apply detector event variation or not
  bool _applyEventVariations;

  /// whether to apply Flux weight systematic or not
  bool _applyFluxWeightSystematic;

  /// The current event in the event array
  Int_t _currentEvent;

  /// The Experiment
  Experiment* _exp;

  /// The Toy Experiment
  ToyExperiment _toy;

  std::vector<DataSample*>::iterator _currentSample;

  SampleGroup* _currentSampleGroup;

  std::vector<DataSample*> _samples;

  /// the maximum number of events to process
  Int_t _nEventsToProcess;

  /// the number of events processed so far
  Int_t _nEventsProcessed;

  /// Access to the flux weighting.
  FluxWeighting* _flux;

  /// Flag to enable/disable flux weight
  bool _applyFluxWeight;

  /// Flag to tell psyche whether to initialise pion systematics, taken from parameters file
  /// Pion systematic initialisation is very very slow, so disabling this when it is not needed speeds
  /// the event loading by a factor of 30
  bool _initialisePionSystematics;

  /// Flux file and option
  std::string _fluxFile;
  std::string _fluxTuning;
  std::string _fluxVersion;

  /// The array of preloaded MC events
  AnaSuperEventB** _mcEventArray;

  /// The array of preloaded Data events
  AnaSuperEventB** _dataEventArray;

  /// the number of events in the preloaded MC array
  UInt_t _nEventsInMCArray;

  /// the number of events in the preloaded Data array
  UInt_t _nEventsInDataArray;

  /// The POT weight for each MC file loaded into the Analysis Manager
  std::vector<Float_t> _POT_weights;
  
  bool _doFGD1;
  bool _doFGD2;

};

#endif

