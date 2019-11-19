#ifndef EventReader_h
#define EventReader_h

#include "BaseDataClasses.hxx"
#include "HighlandInputManager.hxx"

/// This class reads the events
namespace evrd{
  
  const int OutBunchIndex = 100;
  
  class EventReader{
  public :

    EventReader(int argc, char *argv[]);
    virtual ~EventReader(){
    
      // Clean up
      input().RemoveConverters();

      if (_event) delete _event;
      _event = NULL;
      if (_spill)
        delete _spill;
    }

    const AnaEventB* GetEvent(){ 
      return _event;
    }
    
    virtual bool Initialize();
    
    bool GetSpill(Int_t eventID);
    
    bool GetFwdSpill();
    
    bool GetBwdSpill();
     
    /// Change the current bunch forwards or backwards
    bool ChangeBunch(bool forward);   
 
  protected:

    /// Print the program's usage statement.
    void PrintUsage(const std::string& programName);

    // Return the Input Manager
    HighlandInputManager& input(){return _input;}

    
    bool CheckValidEntry(Int_t entry){
      return (entry >= 0 && entry < input().GetEntries());
    }
    
    
    AnaSpillB* GetGivenSpill(Int_t eventID,  Long64_t& entry);
    
    AnaSpillB* GetFwdSpill(Int_t eventID,  Long64_t entry_start,  Long64_t& entry);
    
    AnaSpillB* GetBwdSpill(Int_t eventID, Long64_t entry_start,  Long64_t& entry);
    
    AnaSpillB* ReadSpill(Long64_t entry);
    
    void MakeEvent(); 
    
    /// Input file name, as specified by the user.
    std::string _inputFileName;


    /// Whether to run in "cosmics" mode, where all the tracks are saved in a
    /// single bunch.
    bool _cosmicMode;

    /// Check version compatibility between oaAnalysisReader and oaAnalysis file
    bool _versionCheck;
    
    /// Input file type. The file type is now found automatically, so this is
    /// deprecated.
    std::string _inputFileType;

    Long64_t   _entry;
  
    AnaEventB* _event;
  
    AnaSpillB*  _spill;
  
    Int_t  _index_bunch;
    

    HighlandInputManager _input;

  };
};

#endif
 




