#include "TEventDisplay.hxx"
#include <iostream>
#include "TApplication.h"
#include "TClonesArray.h"
#include "TControlTab.hxx"
#include "TEveBrowser.h"
#include "TEveDigitSetEditor.h"
#include "TEveEventManager.h"
#include "TEveGeoNode.h"
#include "TEveManager.h"
#include "TEvePointSet.h"
#include "TEveQuadSet.h"
#include "TEveRGBAPalette.h"
#include "TEveScene.h"
#include "TEveTrack.h"
#include "TEveVSDStructs.h"
#include "TEveViewer.h"
#include "TFile.h"
#include "TGLScenePad.h"
#include "TGLViewer.h"
#include "TROOT.h"
#include "TSystem.h"

using std::cout;
using std::endl;

#if !defined(__CINT__)
ClassImp(TEventDisplay)
#endif

    TEventDisplay *TEventDisplay::pEventDisplay =
        NULL;  // initialize pointer to sole instance
// Get the sole instance of this singleton class
TEventDisplay *TEventDisplay::Get() {
  if (pEventDisplay == NULL) {            // is it the first call?
    pEventDisplay = new TEventDisplay();  // create sole instance
  }
  //
  //
  return pEventDisplay;  // address of sole instance
}

TEventDisplay::TEventDisplay() : fTopNode(0), fGUI(0) {
  cout << " warning -TEventDisplay default constructor got called !" << endl;
  cout << " This is probably a mistake " << endl;
  throw "Error, Unexpected TEventDisplay constructor called.";
}

TEventDisplay::TEventDisplay(int, std::string) : fTopNode(0), fGUI(0) {
  // Method added to appease ACLiC
  // Use input variables to remove warnings - this method should never be called
  // anyway
  cout << " warning -TEventDisplay(int,std::string c) default constructor "
          "got called !"
       << endl;
  throw "Error, Unexpected TEventDisplay constructor called.";
}
TEventDisplay::~TEventDisplay() {
  cout << " warning -TEventDisplay destructor got called !" << endl;
}
TEventDisplay::TEventDisplay(std::vector<std::string> filelist,
                             std::string geomfile)
    : fTopNode(0) {
  pEventDisplay = this;
  if (geomfile != "") {
    fGeomFile = geomfile;
  } else {
    fGeomFile = "";
  }

  // Create a control tab
  fGUI = new TControlTab(this);

  fIsMC = kFALSE;

  // Check the first file in the list to see if this is data or MC
  fHeader = new TChain("HeaderDir/BasicHeader");

  fHeader->SetBranchAddress("IsMC", &fIsMC);
  fHeader->SetBranchAddress("FGDCosmicEvent", &fFGDCosmicEvent);
  fHeader->SetBranchAddress("TripTCosmicEvent", &fTripTCosmicEvent);
  fHeader->SetBranchAddress("RunID", &fRunID);
  fHeader->SetBranchAddress("SubrunID", &fSubrunID);
  fHeader->SetBranchAddress("EventID", &fEventID);
  fHeader->SetBranchAddress("FGDCosmicEvent", &fFGDCosmicEvent);
  fHeader->SetBranchAddress("TripTCosmicEvent", &fTripTCosmicEvent);
  fHeader->SetBranchAddress("TimeStamp", &fTimeStamp);
  fHeader->SetBranchAddress("Intensity", &fIntensity);
  fRequestedBunch = -2;
  long level = gErrorIgnoreLevel;
  gErrorIgnoreLevel = kSysError;
  // Setup chains in all the things we might display...
  bool checkedMC = kFALSE;
  for (unsigned int ifile = 0; ifile < filelist.size(); ifile++) {
    const char *inputfilename = filelist[ifile].c_str();
    fHeader->AddFile(inputfilename);
    if (!checkedMC) {
      // read in first header entry, so we know if it isMC or not
      fHeader->GetEntry(0);
      TDisplayedAnalysisModuleBase::fIsMC = this->fIsMC;
      checkedMC = kTRUE;
    }

    fGlobal.AddFile(inputfilename);
    fTrackerECal.AddFile(inputfilename);
    fP0DECal.AddFile(inputfilename);
    fP0D.AddFile(inputfilename);
    if (fIsMC) {
      fTruth.AddFile(inputfilename);
    }
    fTracker.AddFile(inputfilename);
  }
  fHeader->GetEntry(0);

  int entries = fGlobal.GetEntries();
  cout << "There are " << entries << " entries in the chain " << endl;

  gErrorIgnoreLevel = level;
  fCurrentEvent = -1;
  DrawNextEvent();
}

void TEventDisplay::DeleteCurrentEvent() {
  TEveEventManager *CurrentEvent = gEve->GetCurrentEvent();
  if (CurrentEvent != 0) {
    CurrentEvent->DestroyElements();
  }
}

void TEventDisplay::DrawNextEvent() {
  DeleteCurrentEvent();
  gEve->GetDefaultGLViewer()->CurrentCamera().SetCenterVec(0.0, 0.0, 0.0);
  NoBunchSelection();
  DrawEvent(++fCurrentEvent);
}

TString TEventDisplay::TimeInJapan(int timestamp) {
  const time_t moment = timestamp;
  struct tm *time_details;
  char old[256];
  if (getenv("TZ") == NULL)
    old[0] = 0;
  else
    snprintf(old, 256, "%s", getenv("TZ"));
  setenv("TZ", "Japan",
         1); /* set to a value in the tree /usr/share/zoneinfo/ */
  time_details = localtime(&moment);
  char buf[80];
  strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", time_details);
  if (old[0] == 0)
    unsetenv("TZ");
  else
    setenv("TZ", old, 1);
  return TString(buf);
}

void TEventDisplay::ListEvent() {
  fHeader->GetEntry(fCurrentEvent);
  cout << " Event information for entry " << fCurrentEvent << endl;
  cout << " Currently reading file: " << endl
       << fHeader->GetCurrentFile()->GetName() << endl;
  cout << " RunID " << fRunID << endl;
  cout << " SubrunID " << fSubrunID << endl;
  cout << " EventID " << fEventID << endl;
  cout << " TimeStamp  " << fTimeStamp << endl;
  cout << " Time       " << TimeInJapan(fTimeStamp) << endl;
  cout << " Intensity  " << fIntensity << endl;
  if (fFGDCosmicEvent) cout << " This is an FGD Cosmic trigger event" << endl;
  if (fTripTCosmicEvent)
    cout << " This is a TripT Cosmic trigger event" << endl;
  if (fIsMC) cout << " This is a Monte Carlo event" << endl;
}
void TEventDisplay::NoBunchSelection() {
  fGUI->fCheckBunch->SetState(kButtonUp);
  fGUI->fBunch->SetState(kFALSE);
  fRequestedBunch = -2;
}
void TEventDisplay::DrawRequestedEvent() {
  if (fGUI != NULL) {
    fCurrentEvent = fGUI->fNumber->GetIntNumber() - 1;
    NoBunchSelection();
    DrawNextEvent();
  }
}
void TEventDisplay::DrawRequestedBunch() {
  if (fGUI != NULL) {
    EButtonState state = fGUI->fCheckBunch->GetState();
    if (state == kButtonDown) {
      fGUI->fBunch->SetState(kTRUE);
      fRequestedBunch = fGUI->fBunch->GetIntNumber();
    } else {
      fGUI->fBunch->SetState(kFALSE);
      fRequestedBunch = -2;
    }
    DeleteCurrentEvent();
    DrawEvent(fCurrentEvent, fRequestedBunch);
  }
}
void TEventDisplay::DrawPreviousEvent() {
  if (fCurrentEvent > 0) {
    DeleteCurrentEvent();
    NoBunchSelection();
    fCurrentEvent--;
    DrawEvent(fCurrentEvent);
  }
}
void TEventDisplay::DrawEvent(int ient, int bunch) {
  if (ient < 0 || ient > (fHeader->GetEntries() - 1)) {
    cout << " Sorry , I can't get entry " << ient
         << " as there are a maximum of " << fHeader->GetEntries()
         << " entries, starting with entry 0. " << endl;
    return;
  }
  fGUI->fEventNumberLabel->SetText(Form(" %-8d ", fCurrentEvent));  // 10 chars
  fGUI->fMain->MapWindow();
  fTrackerECal.Draw(ient, bunch);
  fP0DECal.Draw(ient, bunch);
  fP0D.Draw(ient, bunch);
  if (fIsMC) {
    fTruth.Draw(ient, bunch);
  }
  fTracker.Draw(ient, bunch);
  fGlobal.Draw(ient, bunch);
}
void TEventDisplay::StartDisplay() {
  if (fGeomFile != "") {
    TString geometryFile(fGeomFile);
    TGeoManager::Import(geometryFile);
    gGeoManager->cd("/t2k_1/OA_0");
    TGeoNode *CurrentNode = gGeoManager->GetCurrentNode();
    if (CurrentNode) {
      fTopNode = new TEveGeoTopNode(gGeoManager, CurrentNode, 1, 0, 100000);
      fTopNode->SetRnrSelfChildren(kFALSE, kFALSE);
      gEve->AddGlobalElement(fTopNode);
      TSimpleGeometry G;
      gEve->GetDefaultViewer()->AddScene(G.GetScene());
    }
  } else {
    int use_hardcoded_geometry = 0;
    TSimpleGeometry G(use_hardcoded_geometry);
    gEve->GetDefaultViewer()->AddScene(G.GetScene());
  }
  gEve->GetDefaultGLViewer()->SetStyle(TGLRnrCtx::kWireFrame);

  gEve->GetDefaultGLViewer()->CurrentCamera().SetExternalCenter(kTRUE);
  gEve->GetDefaultGLViewer()->CurrentCamera().SetCenterVec(0.0, 0.0, 0.0);
  gEve->GetDefaultGLViewer()->UpdateScene();
  gEve->GetDefaultGLViewer()->Connect("Clicked(TObject*)", "TEventDisplay",
                                      this, "OnClicked(TObject*)");
}

void TEventDisplay::OnClicked(TObject *object) {
  if (object != NULL) {
    cout << endl;
    for (int i = 0; i < 120; i++) cout << "=";
    cout << endl;
    TString Name(object->GetName());
    TString Title(object->GetTitle());
    TString ClassName1(object->ClassName());
    cout << " You clicked on the following object:" << endl;
    cout << " Name       :" << Name << endl;
    cout << " Title      :" << Title << endl;
    cout << " Class Name :" << ClassName1 << endl;
    cout << endl;
    for (int i = 0; i < 120; i++) cout << "=";
    cout << endl;
/*
First try getting the source object if it exists
*/
#define DCAST(objectType) \
  objectType reconObject = dynamic_cast<objectType>(SourceObject);
#define DESCRIBE(source)          \
  if (reconObject) {              \
    source.indent = 0;            \
    source.Describe(reconObject); \
    return;                       \
  }
    TEveElement *EveElement = dynamic_cast<TEveElement *>(object);
    if (EveElement) {
      TObject *SourceObject = EveElement->GetSourceObject();
      if (SourceObject) {
        SourceObject->Print();

        {
          DCAST(ND::TGlobalReconModule::TGlobalPID *);
          DESCRIBE(fGlobal);
        }
        {
          DCAST(ND::TGlobalReconModule::TP0DObject *);
          DESCRIBE(fGlobal);
        }
        {
          DCAST(ND::TGlobalReconModule::TFGDObject *);
          DESCRIBE(fGlobal);
        }
        {
          DCAST(ND::TGlobalReconModule::TTPCObject *);
          DESCRIBE(fGlobal);
        }
        {
          DCAST(ND::TGlobalReconModule::TECALObject *);
          DESCRIBE(fGlobal);
        }
        {
          DCAST(ND::TGlobalReconModule::TSMRDObject *);
          DESCRIBE(fGlobal);
        }
        {
          DCAST(ND::TGlobalReconModule::TGlobalVertex *);
          DESCRIBE(fGlobal);
        }
        {
          DCAST(ND::TTruthTrajectoriesModule::TTruthTrajectory *);
          DESCRIBE(fTruth);
        }
        {
          DCAST(ND::TTrackerECALReconModule::TECALReconTrack *);
          DESCRIBE(fTrackerECal);
        }
        {
          DCAST(ND::TTrackerECALReconModule::TECALReconShower *);
          DESCRIBE(fTrackerECal);
        }
        {
          DCAST(ND::TTrackerECALReconModule::TECALReconShower *);
          DESCRIBE(fTrackerECal);
        }
        {
          DCAST(ND::TP0DECALReconModule::TP0DECALReconTrack *);
          DESCRIBE(fP0DECal);
        }
        {
          DCAST(ND::TP0DECALReconModule::TP0DECALReconShower *);
          DESCRIBE(fP0DECal);
        }
        {
          DCAST(ND::TP0DReconModule::TP0DParticle *);
          DESCRIBE(fP0D);
        }
        {
          DCAST(ND::TP0DReconModule::TP0DShower *);
          DESCRIBE(fP0D);
        }
        {
          DCAST(ND::TP0DReconModule::TP0DTrack *);
          DESCRIBE(fP0D);
        }
        {
          DCAST(ND::TP0DReconModule::TP0DVertex *);
          DESCRIBE(fP0D);
        }
        {
          DCAST(ND::TTruthVerticesModule::TTruthVertex *);
          DESCRIBE(fTruth);
        }
        {
          DCAST(ND::TTrackerReconModule::TTrackerResult *);
          DESCRIBE(fTracker);
        }
        {
          DCAST(ND::TTrackerReconModule::TTrackerConstituent *);
          DESCRIBE(fTracker);
        }
        {
          DCAST(ND::TTrackerReconModule::TTPCTrack *);
          DESCRIBE(fTracker);
        }
        {
          DCAST(ND::TTrackerReconModule::TFGDTrack *);
          DESCRIBE(fTracker);
        }
      } else
        cout << " no source object for this object" << endl;
    }
    cout << " failed to describe via source object " << endl;
    if (ClassName1.Contains("TEveTrack")) {
      TEveTrack *track = dynamic_cast<TEveTrack *>(object);
      if (track) {
        if (Name.Contains("Global track")) {
          fGlobal.Describe(track->GetIndex());
          return;
        }
        if (Name.Contains("TrackerECALRecon")) {
          fTrackerECal.DescribeTrack(track->GetIndex());
          return;
        }
        if (Title.BeginsWith("True")) {
          fTruth.Describe(track->GetIndex());
          return;
        }
      }
    }
    if (ClassName1.Contains("TEveArrow")) {
      if (Name.Contains("TrackerECALRecon")) {
        fTrackerECal.DescribeShower(Name);
        return;
      }
      if (Name.Contains("ECAL shower")) {
        fGlobal.DescribeShower(Name);
        return;
      }
    }
    cout << " Clicked on unidentified object:" << endl;
    cout << " Name:" << Name << endl;
    cout << " Title:" << Title << endl;
    cout << " Class Name:" << ClassName1 << endl;
  } else
    cout << "OnClicked: object is NULL !" << endl;
}

void TEventDisplay::FindEvent() {
  int RunToFind = fGUI->GetRunNumber();
  int SubRunToFind = fGUI->GetSubRunNumber();
  int EventToFind = fGUI->GetEventNumber();
  cout << " you want me to find:" << RunToFind << " " << SubRunToFind << " "
       << EventToFind << endl;
  if (!GoToEvent(RunToFind, SubRunToFind, EventToFind)) {
    cout << " Sorry - couldn't find that event " << endl;
  }
}

bool TEventDisplay::GoToEvent(int RunToFind, int SubRunToFind,
                              int EventToFind) {
  cout << "TEventDisplay::GoToEvent you want me to find:" << RunToFind << " "
       << SubRunToFind << " " << EventToFind << endl;
  if (RunToFind == 0 && SubRunToFind == 0 && EventToFind == 0) return false;
  int originalEntry = fCurrentEvent;
  int nEvent = fHeader->GetEntries();
  for (fCurrentEvent = 0; fCurrentEvent < nEvent; fCurrentEvent++) {
    fHeader->GetEntry(fCurrentEvent);
    if (fCurrentEvent % 10000 == 0) {
      printf("\r Searching %10d %10d %10d %10d ", fCurrentEvent, fRunID,
             fSubrunID, fEventID);
      cout << std::flush;
    }
    if (fCurrentEvent % 100000 > 50000) {
      printf("\r           %10d %10d %10d %10d ", fCurrentEvent, fRunID,
             fSubrunID, fEventID);
      cout << std::flush;
    }
    if (fRunID == RunToFind && fSubrunID == SubRunToFind &&
        fEventID == EventToFind) {
      cout << endl
           << endl
           << "  Found the event you requested " << endl
           << endl;
      DeleteCurrentEvent();
      NoBunchSelection();
      DrawEvent(fCurrentEvent);
      ListEvent();
      return true;
    }
  }
  fCurrentEvent = originalEntry;
  return false;
}
