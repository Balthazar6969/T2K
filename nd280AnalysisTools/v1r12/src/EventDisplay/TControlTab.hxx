#ifndef TControlTab_HXX
#define TControlTab_HXX
#include "TGButton.h"
#include "TGCanvas.h"
#include "TGFrame.h"
#include "TGLabel.h"
#include "TGNumberEntry.h"

class TEventDisplay;
/// Class to create additional control tab to contain all GUI items for event
/// display.
///\author Alex Finch (A.Finch@lancaster.ac.uk)
class TControlTab {
public:
  TGMainFrame *fMain;

private:
  TGCanvas *fCanvasWindow;
  TGLayoutHints *fMenuBarLayout, *fMenuBarItemLayout, *fMenuBarHelpLayout;
  TGCompositeFrame *fFrame;

public:
  TGLabel *fEventNumberLabel;

  TGNumberEntry *fNumber, *fRunNumber, *fSubRunNumber, *fEventNumber;
  TGNumberEntry *fBunch;
  TGCheckButton *fCheckBunch;

  TControlTab(TEventDisplay *Parent);
  unsigned int GetRunNumber();
  unsigned int GetSubRunNumber();
  unsigned int GetEventNumber();
};
#endif
