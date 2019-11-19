#ifndef TEventDisplay_HXX
#define TEventDisplay_HXX

#include <string>

#include "TControlTab.hxx"
#include <TGFrame.h>

//#include "TEveBoxSet.h"
#include "TEveScene.h"
#include "TString.h"

#include "TDisplayedGlobalModule.hxx"
#include "TDisplayedP0DECALModule.hxx"
#include "TDisplayedP0DModule.hxx"
#include "TDisplayedTrackerECalModule.hxx"
#include "TDisplayedTrackerModule.hxx"
#include "TDisplayedTruthModule.hxx"
#include "TEveGeoNode.h"
#include "TGeoManager.h"
#include "TSimpleGeometry.hxx"

/// Class used to display events in oaAnalysis format using ROOT Eve classes.
///\author    Alex Finch (A.Finch@lancaster.ac.uk)
class TEventDisplay : public TGMainFrame {
public:
  //  TChain       *fGlobalRecon;
  Int_t fCurrentEvent;
  Int_t fRequestedBunch;
  //  Int_t         fNPID;
  //  TClonesArray *fGlobalTracks;
  std::string filename;
  TEveGeoTopNode *fTopNode;
  TControlTab *fGUI;
  std::string fGeomFile;
  Int_t fRunID;             //	RunID/I
  Int_t fEventID;           //	EventID/I
  Bool_t fPreselected;      //	Preselected/B
  Int_t fSubrunID;          //	SubrunID/I
  UInt_t fEventTime;        //	EventTime/i
  ULong64_t fTriggerWord;   //	TriggerWord/l
  Bool_t fFGDCosmicEvent;   //	FGDCosmicEvent/O
  Bool_t fTripTCosmicEvent; //	TripTCosmicEvent/O
  //    ULong64_t	fCTMTriggerPattern ;//	CTMTriggerPattern[3]/l
  //    Int_t	fTripTCosmicTriggerType ;//	TripTCosmicTriggerType/I
  //    Int_t	fTripTCosmicTriggerUpward ;//	TripTCosmicTriggerUpward/I
  Char_t fSoftwareVersion; //	SoftwareVersion/C
  //    Int_t	fPartition ;//	Partition/I
  //    Int_t	fSpill ;//	Spill/I
  Int_t fTimeStamp;   //	TimeStamp/I
  Bool_t fIsDetector; //	IsDetector/B
  Bool_t fIsMC;       //	IsMC/B
  Float_t fIntensity; //	Intensity/F

public:
  /// Return single instance of TEventDisplay.
  static TEventDisplay *Get();
  /// Constructor passed list of files and name of a geometry file.
  TEventDisplay(std::vector<std::string> filelist, std::string geomfile);
  TEventDisplay();
  TEventDisplay(int, std::string c);
  ~TEventDisplay();
  /// Start the display, do all the initialisation of Eve etc.
  void StartDisplay();
  /// Draw event number ient, with specific bunch number if set.
  void DrawEvent(int ient, int bunch = -2);
  /// Get next event and draw it.
  void DrawNextEvent();
  /// Draw Previous event in file.
  void DrawPreviousEvent();
  /// Remove current event from Eve's lists.
  void DeleteCurrentEvent();
  /// Respond to an object being clicked. Describe it.
  void OnClicked(TObject *);
  /// Find out what event the user has requested in the GUI and draw it.
  void DrawRequestedEvent();
  /// If user requests a different bunch, redraw current event with appropriate
  /// selection.
  void DrawRequestedBunch();
  /// Turn off bunch selection. Called whenever drawing a new event.
  void NoBunchSelection();
  /// List basic information about this event. eg. event and run number
  void ListEvent();
  /// Find and event using run/subrun/event number from the GUI
  void FindEvent();
  /// Find and event using run/subrun/event number from arguments
  bool GoToEvent(int RunToFind, int SubRunToFind, int EventToFind);

private:
  static TEventDisplay *pEventDisplay;
  TString TimeInJapan(int timestamp);
  ClassDef(TEventDisplay, 0);
  // add the global recon information

  TDisplayedGlobalModule fGlobal;
  TDisplayedTruthModule fTruth;
  TDisplayedTrackerModule fTracker;
  TDisplayedTrackerECalModule fTrackerECal;
  TDisplayedP0DECALModule fP0DECal;
  TDisplayedP0DModule fP0D;
  TChain *fHeader;

  TSimpleGeometry *fSimpleGeometry;
};
#endif
