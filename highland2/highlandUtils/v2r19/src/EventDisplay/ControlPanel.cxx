#include "ControlPanel.hxx"

ClassImp(ctrl::ControlPanel);

//********************************************************************
ctrl::ControlPanel::ControlPanel(const TGWindow *p, UInt_t w, UInt_t h): TGMainFrame(p,w,h) {
  //********************************************************************

  _button_neutral = NULL;
  _plotEvent      = NULL;
  _readEvent      = NULL;

  _spill_number   = NULL;

  for (int i = 0; i < butils::kNPrjTypes; i++)
    _canvas_control[i] = NULL;

  for (int i = 0; i < butils::kNDrwTypes; i++)
    _obj_control[i] = NULL;

  _cnv_group = NULL;
  _obj_group = NULL;
  _canvas_main = NULL;

}

//********************************************************************
ctrl::ControlPanel::~ControlPanel(void) {
  //********************************************************************
  //Remove objects it owns
  if (_plotEvent) delete _plotEvent;
  _plotEvent = NULL;

  if (_readEvent) delete _readEvent;
  _readEvent = NULL;

}


//********************************************************************
bool ctrl::ControlPanel::Initialize(){
  //********************************************************************

  TGHorizontalFrame* horizontalFrame = new TGHorizontalFrame(this);  
  TGVerticalFrame* leftSide = new TGVerticalFrame(horizontalFrame);


  TGHorizontalFrame* rightSide = new TGHorizontalFrame(horizontalFrame);



  // Create the buttons on the left side.
  TGFrame* widget;
  TGLayoutHints *layout;

  widget = new TGTextButton(this, "&Draw", kDraw);
  layout = new TGLayoutHints(kLHintsCenterX
      | kLHintsCenterY
      | kLHintsExpandX 
      | kLHintsExpandY);
  leftSide->AddFrame(widget, layout);

  _button_neutral = new TGCheckButton(this, "&Neutral", kNeutral);
  _button_neutral->SetState(kButtonUp);   

  layout = new TGLayoutHints(kLHintsCenterX
      | kLHintsCenterY 
      | kLHintsExpandY);
  leftSide->AddFrame(_button_neutral, layout);


  _cnv_group    = new TGHButtonGroup(this, "Projection", kHorizontalFrame);
  _canvas_control[butils::kXZ]  = new TGCheckButton(_cnv_group, "&XZ", kXZ);
  _canvas_control[butils::kYZ]  = new TGCheckButton(_cnv_group, "&YZ", kYZ);
  _canvas_control[butils::kXY]  = new TGCheckButton(_cnv_group, "&XY", kXY);
  
  _canvas_control[butils::kXZ]->SetState(kButtonDown);
  _canvas_control[butils::kYZ]->SetState(kButtonDown);
  _canvas_control[butils::kXY]->SetState(kButtonDown);
  
  _canvas_control[butils::kXZ]->Connect("Toggled(Bool_t)", "ctrl::ControlPanel", this,
      "SetDrawXZCanvas(Bool_t)");
     
  _canvas_control[butils::kYZ]->Connect("Toggled(Bool_t)", "ctrl::ControlPanel", this,
      "SetDrawYZCanvas(Bool_t)");
  
  _canvas_control[butils::kXY]->Connect("Toggled(Bool_t)", "ctrl::ControlPanel", this,
      "SetDrawXYCanvas(Bool_t)");

  _cnv_group->Show();

  layout = new TGLayoutHints(kLHintsCenterX
      | kLHintsCenterY 
      | kLHintsExpandX
      | kLHintsExpandY);
  leftSide->AddFrame(_cnv_group, layout);

  _obj_group    = new TGHButtonGroup(this, "Objects", kVerticalFrame);
  _obj_control[butils::kTrueParticles]  = new TGCheckButton(_obj_group, "&True Objects", kTrue); 
  _obj_control[butils::kRecoParticles]  = new TGCheckButton(_obj_group, "&Reco Objects", kReco);
  
  _obj_control[butils::kTrueParticles]->SetState(kButtonDown);
  _obj_control[butils::kRecoParticles]->SetState(kButtonDown);
   
  _obj_control[butils::kTrueParticles]->Connect("Toggled(Bool_t)", "ctrl::ControlPanel", this,
      "SetDrawTrueParticles(Bool_t)");
     
  _obj_control[butils::kRecoParticles]->Connect("Toggled(Bool_t)", "ctrl::ControlPanel", this,
      "SetDrawRecoParticles(Bool_t)");
  
  _obj_group->Show();

  layout = new TGLayoutHints(kLHintsCenterX
      | kLHintsCenterY
      | kLHintsExpandX
      | kLHintsExpandY);
  leftSide->AddFrame(_obj_group, layout);


  widget = new TGTextButton(this, "&Next Bunch", kNextBunch);
  layout = new TGLayoutHints(kLHintsCenterX
      | kLHintsCenterY
      | kLHintsExpandX 
      | kLHintsExpandY);
  leftSide->AddFrame(widget, layout);

  widget = new TGTextButton(this, "&Previous Bunch", kBackBunch);
  layout = new TGLayoutHints(kLHintsCenterX
      | kLHintsCenterY
      | kLHintsExpandX 
      | kLHintsExpandY);
  leftSide->AddFrame(widget, layout);

  widget = new TGTextButton(this, "&Next Spill", kNextSpill);
  layout = new TGLayoutHints(kLHintsCenterX
      | kLHintsCenterY
      | kLHintsExpandX 
      | kLHintsExpandY);
  leftSide->AddFrame(widget, layout);

  widget = new TGTextButton(this, "&Previous Spill", kBackSpill);
  layout = new TGLayoutHints(kLHintsCenterX
      | kLHintsCenterY
      | kLHintsExpandX 
      | kLHintsExpandY);
  leftSide->AddFrame(widget, layout);


  TGGroupFrame* frame = new TGGroupFrame(this, "Event Control", kHorizontalFrame); 

  TGLabel* label = new TGLabel(frame, "Requested Spill");
  layout = new TGLayoutHints(kLHintsCenterX
      | kLHintsCenterY 
      | kLHintsExpandY);
  frame->AddFrame(label, layout);


  _spill_number = new TGNumberEntry(frame, 0.005, 9, kGoToSpill,
      TGNumberFormat::kNESInteger,   //style
      TGNumberFormat::kNEAPositive,   //input value filter
      TGNumberFormat::kNELNoLimits);
  //  _spill_number->Connect("ValueSet(Long_t)", "ctrl::ControlPanel", this, "GetSpill()");
  (_spill_number->GetNumberEntry())->Connect("ReturnPressed()", "ctrl::ControlPanel", this,
      "GetSpill()");
  layout = new TGLayoutHints(kLHintsCenterX
      | kLHintsCenterY 
      | kLHintsExpandX 
      | kLHintsExpandY);
  frame->AddFrame(_spill_number, layout);


  frame->Layout();
  layout = new TGLayoutHints(kLHintsCenterX
      | kLHintsCenterY
      | kLHintsExpandX
      | kLHintsExpandY);
  leftSide->AddFrame(frame, layout);


  widget = new TGTextButton(this, "&QUIT", kQuit);
  layout = new TGLayoutHints(kLHintsCenterX
      | kLHintsCenterY
      | kLHintsExpandX 
      | kLHintsExpandY);
  leftSide->AddFrame(widget, layout);

  leftSide->Layout();
  layout = new TGLayoutHints(kLHintsLeft |  kLHintsExpandY);
  horizontalFrame->AddFrame(leftSide,layout);

  _canvas_main  = new TRootEmbeddedCanvas(0, rightSide);
  Int_t wid = _canvas_main->GetCanvasWindowId();
  _plotEvent->SetMainCanvas(new TCanvas("MyCanvas", 200, 200,wid));
  _canvas_main->AdoptCanvas(_plotEvent->GetMainCanvas());

  layout = new TGLayoutHints(kLHintsCenterX
      | kLHintsCenterY
      | kLHintsExpandX 
      | kLHintsExpandY);
  rightSide->AddFrame(_canvas_main,layout);

  layout = new TGLayoutHints(kLHintsCenterX
      | kLHintsCenterY
      | kLHintsExpandY);
  //  rightSide->AddFrame(_canvas_def,layout);



  rightSide->Layout();
  layout = new TGLayoutHints(kLHintsRight | kLHintsExpandX | kLHintsExpandY);
  horizontalFrame->AddFrame(rightSide, layout);

  layout = new TGLayoutHints(kLHintsCenterX 
      | kLHintsCenterY
      | kLHintsExpandX
      | kLHintsExpandY);
  AddFrame(horizontalFrame,layout);

  SetCleanup(kDeepCleanup);
  SetWindowName("Button Control");
  SetIconName("Button Control ");
  MapSubwindows();
  Layout();
  MapWindow();

  if (!InitializeBase()) return false; 

  _plotEvent->CleanScreen();
  // DrawEvent();
  return true;

}

//********************************************************************
Bool_t ctrl::ControlPanel::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2) {
  //********************************************************************
  // Process events generated by the buttons in the frame.
  std::cout << "Message " << msg << "  " << parm1 << " " << parm2 << std::endl;

  switch (GET_MSG(msg)) {
    case kC_COMMAND:
      switch (GET_SUBMSG(msg)) {
        case kCM_BUTTON:
          printf("text button id %ld pressed\n", parm1);
          if( parm1 == kDraw        ) DrawEvent();
          if( parm1 == kNextBunch   ) {ChangeBunch(true);  DrawEvent();}
          if( parm1 == kBackBunch   ) {ChangeBunch(false); DrawEvent();}
          if( parm1 == kNextSpill   ) {GetFwdSpill(); DrawEvent();}
          if( parm1 == kBackSpill   ) {GetBwdSpill(); DrawEvent();}
          if( parm1 == kQuit        ) QuitProgram();
        case kCM_CHECKBUTTON:
          printf("check button id %ld pressed\n", parm1);
          if (parm1 == kNeutral){
            SetDrawNeutral(_button_neutral->GetState() == kButtonDown);
          } 

        default:
          break;
      }
  } 


  gROOT->Reset();

  return kTRUE;
}
