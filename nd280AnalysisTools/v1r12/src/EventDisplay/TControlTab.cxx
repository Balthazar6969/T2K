#include <iostream>

#include "TEveBrowser.h"
#include "TEveManager.h"
#include "TGButton.h"
#include "TGFrame.h"
#include "TROOT.h"
#include "TSystem.h"

#include "TControlTab.hxx"

using std::cout;
using std::endl;
TControlTab::TControlTab(TEventDisplay *Parent) {
  gEve->GetBrowser()->StartEmbedding(TRootBrowser::kLeft);

  fMain = new TGMainFrame(gClient->GetRoot(), 200, 220);
  // use hierarchical cleaning
  fMain->SetCleanup(kDeepCleanup);
  fCanvasWindow = new TGCanvas(fMain, 200, 220);
  fFrame = new TGCompositeFrame(fCanvasWindow->GetViewPort(), 10, 10,
                                kVerticalFrame);
  fFrame->SetLayoutManager(new TGVerticalLayout(fFrame));
  fCanvasWindow->SetContainer(fFrame);
  // use hierarchical cleaning for container
  fFrame->SetCleanup(kDeepCleanup);
  TString icondir(gSystem->ExpandPathName("${ROOTSYS}/root/icons/"));
  {
    FileStat_t buf;
    if (gSystem->GetPathInfo(icondir.Data(), buf)) {
      icondir = gSystem->ExpandPathName("${ROOTSYS}/icons/");
      if (gSystem->GetPathInfo(icondir.Data(), buf))
        icondir = gSystem->ExpandPathName("${ROOTSYS}/share/root/icons/");
    }
  }
  TGLayoutHints *fButtonLayout =
      new TGLayoutHints(kLHintsCenterY | kLHintsCenterX);
  TGHorizontalFrame
      *hf;  // controls which get re-used for several parts of the gui
  TGGroupFrame *Group;
  /*
  Add the event controls
  */
  {
    Group = new TGGroupFrame(fCanvasWindow->GetContainer(), "Entry Selection");
    /*
    Add a label to keep track of the current event
    */

    hf = new TGHorizontalFrame(Group, 32, 32);
    {
      TGLabel *Label = new TGLabel(hf, "Current Entry:");
      hf->AddFrame(Label, new TGLayoutHints(kLHintsLeft, 1, 1, 5, 5));
      fEventNumberLabel = new TGLabel(hf, " 00000000 ");  // 10 chars
      hf->AddFrame(fEventNumberLabel,
                   new TGLayoutHints(kLHintsTop | kLHintsRight, 5, 5, 5, 5));
    }

    Group->AddFrame(hf);
    hf = new TGHorizontalFrame(Group, 32, 32);
    /*
    create the " previous event " button
    */
    TGPictureButton *Button =
        new TGPictureButton(hf, gClient->GetPicture(icondir + "GoBack.gif"));
    Button->SetToolTipText("Go back to previous Entry.");
    Button->Connect("Clicked()", "TEventDisplay", Parent,
                    "DrawPreviousEvent()");
    hf->AddFrame(Button, new TGLayoutHints(kLHintsCenterY | kLHintsLeft));

    /*
    create the "go to event" box
    */

    TGVerticalFrame *vf = new TGVerticalFrame(hf);
    {
      {
        TGLabel *Label = new TGLabel(vf, "Go To Entry:");
        vf->AddFrame(
            Label, new TGLayoutHints(kLHintsCenterX | kLHintsTop, 1, 1, 5, 5));
      }
      {
        fNumber =
            new TGNumberEntry(vf, 0, 8, -1, TGNumberFormat::kNESInteger,
                              TGNumberFormat::kNEANonNegative,
                              TGNumberFormat::kNELLimitMinMax, 0, 99999999);

        fNumber->Connect("ValueSet(Long_t)", "TEventDisplay", Parent,
                         "DrawRequestedEvent()");
        vf->AddFrame(fNumber, new TGLayoutHints(kLHintsCenterX | kLHintsTop, 5,
                                                5, 5, 5));
      }
    }
    hf->AddFrame(vf, fButtonLayout);
    /*
    Create the Next event box
    */
    Button =
        new TGPictureButton(hf, gClient->GetPicture(icondir + "GoForward.gif"));
    Button->SetToolTipText("Go forward to next Entry.");
    Button->Connect("Clicked()", "TEventDisplay", Parent, "DrawNextEvent()");

    // TQObject::Connect(Button,"Clicked()","ND::TEveDisplay",this,"DisplayNextSelectedEvent()");
    hf->AddFrame(Button, new TGLayoutHints(kLHintsCenterY | kLHintsRight));
    /*
    Create the Next User selected event box
    */
    /*
    TString srcdir( Form("%s/src/", gSystem->Getenv("EVENTDISPLAYROOT")) );
    Button = new TGPictureButton(hf, gClient->GetPicture(srcdir + "sieve.png"));
    Button->SetToolTipText("Go forward to next event selected by user.");
    TQObject::Connect(Button,"Clicked()","ND::TEveDisplay",this,"DisplayNextSelectedEvent()");
    hf->AddFrame(Button, fButtonLayout);
    */
    Group->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));
  }

  fCanvasWindow->AddFrame(Group, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));

  /*
  Add the event selection controls
  */
  {
    Group = new TGGroupFrame(fCanvasWindow->GetContainer(), "Bunch Selection");

    /*
    Control for selecting by bunch number
    */
    {
      TGLabel *Label;
      fCheckBunch = new TGCheckButton(Group, "Select Bunch");
      fCheckBunch->SetState(kButtonUp);  // initially disabled
      fCheckBunch->Connect("Toggled(Bool_t)", "TEventDisplay", Parent,
                           "DrawRequestedBunch()");
      Group->AddFrame(
          fCheckBunch,
          new TGLayoutHints(kLHintsCenterX | kLHintsTop, 5, 5, 5, 5));
      hf = new TGHorizontalFrame(Group, 32, 32);
      Label = new TGLabel(hf, "Bunch number");
      hf->AddFrame(Label,
                   new TGLayoutHints(kLHintsCenterX | kLHintsTop, 1, 1, 5, 5));
      fBunch = new TGNumberEntry(hf, 0, 1, -1, TGNumberFormat::kNESInteger,
                                 TGNumberFormat::kNEANonNegative,
                                 TGNumberFormat::kNELLimitMinMax, 0, 7);
      fBunch->Connect("ValueSet(Long_t)", "TEventDisplay", Parent,
                      "DrawRequestedBunch()");
      fBunch->SetState(kFALSE);  // initially disabled
      hf->AddFrame(fBunch,
                   new TGLayoutHints(kLHintsCenterX | kLHintsTop, 5, 5, 5, 5));
      Group->AddFrame(
          hf, new TGLayoutHints(kLHintsCenterX | kLHintsTop, 5, 5, 5, 5));
    }
  }
  fCanvasWindow->AddFrame(Group, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));

  /*
  Add a button to request print out of basic event information
  */

  {
    Group = new TGGroupFrame(fCanvasWindow->GetContainer(), "Information");

    {
      TGButton *ListEvent = new TGTextButton(Group, "Event Info.");
      ListEvent->Connect("Clicked()", "TEventDisplay", Parent, "ListEvent()");
      Group->AddFrame(ListEvent, new TGLayoutHints(kLHintsCenterX | kLHintsTop,
                                                   5, 5, 5, 5));
    }
  }
  fCanvasWindow->AddFrame(Group, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));

  /*
  Add a controls to select event by run/subrun/event
  */

  {
    Group = new TGGroupFrame(fCanvasWindow->GetContainer(), "Event Selection");
    TGVerticalFrame *vf = new TGVerticalFrame(Group);
    {
      TGLabel *Label = new TGLabel(vf, "Run Number:");
      vf->AddFrame(Label, new TGLayoutHints(kLHintsLeft, 1, 1, 5, 5));
    }
    {
      fRunNumber =
          new TGNumberEntry(vf, 0, 8, -1, TGNumberFormat::kNESInteger,
                            TGNumberFormat::kNEANonNegative,
                            TGNumberFormat::kNELLimitMinMax, 0, 99999999);
      vf->AddFrame(fRunNumber,
                   new TGLayoutHints(kLHintsCenterX | kLHintsTop, 5, 5, 5, 5));
    }
    {
      TGLabel *Label = new TGLabel(vf, "SubRun Number:");
      vf->AddFrame(Label, new TGLayoutHints(kLHintsLeft, 1, 1, 5, 5));
    }
    {
      fSubRunNumber =
          new TGNumberEntry(vf, 0, 8, -1, TGNumberFormat::kNESInteger,
                            TGNumberFormat::kNEANonNegative,
                            TGNumberFormat::kNELLimitMinMax, 0, 99999999);
      vf->AddFrame(fSubRunNumber,
                   new TGLayoutHints(kLHintsCenterX | kLHintsTop, 5, 5, 5, 5));
    }
    {
      TGLabel *Label = new TGLabel(vf, "Event Number:");
      vf->AddFrame(Label, new TGLayoutHints(kLHintsLeft, 1, 1, 5, 5));
    }
    {
      fEventNumber =
          new TGNumberEntry(vf, 0, 8, -1, TGNumberFormat::kNESInteger,
                            TGNumberFormat::kNEANonNegative,
                            TGNumberFormat::kNELLimitMinMax, 0, 99999999);
      vf->AddFrame(fEventNumber,
                   new TGLayoutHints(kLHintsCenterX | kLHintsTop, 5, 5, 5, 5));
    }
    {
      TGButton *ListEvent = new TGTextButton(vf, "Find Event");
      ListEvent->Connect("Clicked()", "TEventDisplay", Parent, "FindEvent()");
      vf->AddFrame(ListEvent,
                   new TGLayoutHints(kLHintsCenterX | kLHintsTop, 5, 5, 5, 5));
    }
    Group->AddFrame(vf);
  }
  fCanvasWindow->AddFrame(Group, new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5));

  fMain->AddFrame(
      fCanvasWindow,
      new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5));

  fMain->SetWindowName("ND280 oaAnalysis Event Dispplay");

  fMain->MapSubwindows();

  fMain->Resize();
  fMain->MapWindow();
  gEve->GetBrowser()->StopEmbedding("ND280 Event Control");
  gEve->GetBrowser()->SetWindowName("ND280 oaAnalysis Event Display");
}
unsigned int TControlTab::GetRunNumber() { return fRunNumber->GetIntNumber(); }
unsigned int TControlTab::GetSubRunNumber() {
  return fSubRunNumber->GetIntNumber();
}
unsigned int TControlTab::GetEventNumber() {
  return fEventNumber->GetIntNumber();
}
