#ifndef oaAnalysisTreeDefinition_hxx
#define oaAnalysisTreeDefinition_hxx


#include "TRandom3.h"
#include <TSystem.h>
#include <TROOT.h>
#include <TApplication.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TTree.h>
#include <TChain.h>
#include <TClonesArray.h>
#include <TFile.h>
#include <stdio.h>
#include <iostream>
#include <math.h>

#include <TObject.h>

//#include <AnalysisTools/libReadoaAnalysis/libReadoaAnalysisProjectHeaders.h>

//#include <Vector.h>
//#include <Matrix.h>
//#include <Random.h>


// Dimensions of arrays:
//_______________________________________________________________________

const int _NMAXTRAJ = 200;
const int _NMAXTRUEVERTICES = 50;
const int _NMAXVERTICES = 10;
const int _NMAXTIMEBIN = 20;
const int _NCONSTITUENTS = 10;
const int _NMAXCLUSTERS = 50;
const int _NVTXTYPES = 11;
const int _NTRAJTYPES = 7;
const int _NBUNCH = 11;
const int _NRUNS = 4;
const int _NHYPO = 5;
const int _NMAXTRACKS = 10;
const int _NOPTIONS = 2;


class oaAnalysisTreeDefinition{

public:

  oaAnalysisTreeDefinition(){}
  oaAnalysisTreeDefinition(const std::string& file);

  TTree* GetChain(){return _reconGlobal;}
  Long64_t LoadTree(Long64_t entry){
    //    oaAnalysisTruthTrajectoriesTreeDefinition::LoadTree(entry);
    //    oaAnalysisTruthVerticesTreeDefinition::LoadTree(entry);
    //    return oaAnalysisGlobalTreeDefinition::LoadTree(entry);
  }
  Int_t  GetEntry(Long64_t entry){
    if(_isMC){
      _mcTruthVertices->GetEntry(entry);    
      _mcTruthTrajectories->GetEntry(entry);    
    }
    _reconGlobal->GetEntry(entry);
    _beamSummary->GetEntry(entry);
    _DQInfo->GetEntry(entry);
  }


  bool _isMC;
  bool _silent;

  TChain* _mcTruthVertices;
  TChain* _mcTruthTrajectories;
  TChain* _reconGlobal;
  TChain* _beamSummary;
  TChain* _DQInfo;
    
  std::string _outfile_name;
  bool _isMC_initialized;
  
  
  int _EventID;       
  int _RunID;
  int _SubrunID;
  
  
  Int_t        _NVertices; //! The number of added primary vertices
  TClonesArray *_Vertices; //! The TGlobalVertexObject vector of vertices.
  Int_t        fPVInd;     //! The last primary vertex index
  Int_t        _NSVertices;//! Number of added secondary vertices
  
  
  Int_t        _NPIDs;     //! The number of global objects
  TClonesArray *_PIDs;     //! The vector of TGlobalPID 
  
  Int_t        _NTPCOthers; //! The number of TPC other objects
  TClonesArray *_TPCOthers; //! The vector of TPC other objects 
  
  
  Int_t _NTPCUnused;         //!   Number of hits unused in TPC   
  
  Int_t _NFGD1Unused;         //!   Number of hits unused in FGD1   
  Int_t _NFGD2Unused;         //!   Number of hits unused in FGD2 
  
  Int_t _NP0DUnused;         //!   Number of hits unused in P0D 
  TClonesArray *_P0DUnused;  //!   The vector unused hits in P0D  
  
  
  Int_t _NSMRDTopUnused;         //!   Number of hits unused in SMRD  
  Int_t _NSMRDBottomUnused;         
  Int_t _NSMRDLeftUnused;         
  Int_t _NSMRDRightUnused;         
  
  //   Int_t _NECALUnused;         //!   Number of hits unused in ECAL   
  
  
  //   TOutermostHits _P0DOutermostHits;
  
  double _EarliestTrackMedianHitTime;   //! Median hit time of the earliest track
  Int_t        _NDelayedClusters;       //! # Delayed clusters in FGD (>200 ns)
  TClonesArray *_DelayedClusters;       //! Delayed clusters in FGD
  
  
  // Information about unused FGD hits and outermost FGD hits is saved for each time bin
  Int_t        _NFgdTimeBins;       //! Number of hit time bins in the FGD as determined by fgdRecon
  TClonesArray *_FgdTimeBins;       //! Information for each hit time bin
  
  Int_t        _NECAL;     //! Number on objects containing ECAL
  Int_t        _NTPC;      //! Number on objects containing TPC
  Int_t        _NFGD;      //! Number on objects containing FGD
  Int_t        _NP0D;      //! Number on objects containing P0D
  Int_t        _NSMRD;     //! Number on objects containing SMRD
  
  Int_t        _NECALIso;  //! Number on objects in ECAL only
  Int_t        _NTPCIso;   //! Number on objects in TPC only
  Int_t        _NFGDIso;   //! Number on objects in FGD only
  Int_t        _NP0DIso;   //! Number on objects in P0D only
  Int_t        _NSMRDIso;  //! Number on objects in SMRD only
  
  
  
  //_______________________________________________________________________
  // Some Truth variables:

  int _NTruthVertices[_NVTXTYPES];
  int _NTruthTrajs[_NTRAJTYPES];
  
  // Set the branches for the truth information.
  TClonesArray *_TruthVertices[_NVTXTYPES];
  TClonesArray *_TruthTrajs[_NTRAJTYPES];
  
  
  
  //beam summary Data
  TClonesArray  *_BeamSummaryData;
  int _ND280Spill;
  
  //DAQ INFO
  Int_t _BarECALFlag;
  Int_t _DSECALFlag;
  Int_t _ECALFlag;
  Int_t _FGD1Flag;
  Int_t _FGD2Flag;
  Int_t _FGDFlag;
  Int_t _INGRIDFlag;
  Int_t _MAGNETFlag;
  Int_t _ND280OffFlag;
  Int_t _P0DECALFlag;
  Int_t _P0DFlag;
  Int_t _SMRDFlag;
  Int_t _TPC1Flag;
  Int_t _TPC2Flag;
  Int_t _TPC3Flag;
  Int_t _TPCFlag;
  
  
  
  
  // set a few std::string arrays
  std::string _vtxnames[_NVTXTYPES];
  std::string _trajnames[_NTRAJTYPES];

};

#endif






