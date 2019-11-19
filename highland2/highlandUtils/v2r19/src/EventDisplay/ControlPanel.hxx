#ifndef ControlPanel_h
#define ControlPanel_h

#include <TROOT.h>
#include <TRootEmbeddedCanvas.h>
#include <TGButtonGroup.h>
#include <TGNumberEntry.h>
#include <TGClient.h> 
#include <TGLabel.h>

#include "BitUtils.hxx"
#include "EventPlotter.hxx"
#include "EventReader.hxx"
#include <RQ_OBJECT.h>

namespace ctrl{

  /// Create a control panel frame with buttons.

  class ControlPanel : public TGMainFrame {
    
  public: 
    ControlPanel(const TGWindow *p, UInt_t w, UInt_t h); 
    virtual  ~ControlPanel();


    // Build the actual control
    virtual bool Initialize();

    bool InitializeBase(){
      if (!GetReadyStatus())         return false;
      if (!_plotEvent->Initialize()) return false;
      if (!_readEvent->Initialize()) return false;
      return true;
    }

    bool GetReadyStatus(){
      return (bool)(_plotEvent && _readEvent);
    }

    void SetEventPlotter(plot::EventPlotter* plot){
      if (!plot) return;
      if (_plotEvent) delete _plotEvent;
      _plotEvent = plot;
    }
    
    void SetDetForager(det::DetectorForager* dfor){
      if (!dfor) return;
      if (!_plotEvent) return;
      _plotEvent->SetDetForager(dfor);
    }
    
    void SetEventReader(evrd::EventReader* read){  
      if (!read) return;
      if (_readEvent) delete _readEvent;
      _readEvent = read;
      
    }
        
    void GetSpill(){
      if (!GetReadyStatus()) return;
      Int_t event = _spill_number->GetNumberEntry()->GetIntNumber(); 
      _readEvent->GetSpill(event); 
    }
    
    
    void SetDrawXZCanvas(Bool_t draw){
      if (!GetReadyStatus()) return;
      _plotEvent->SetDrawCanvas(butils::kXZReco, draw); 
    }
    
    void SetDrawYZCanvas(Bool_t draw){
      if (!GetReadyStatus()) return;
      _plotEvent->SetDrawCanvas(butils::kYZReco, draw); 
    }
    
    void SetDrawXYCanvas(Bool_t draw){
      if (!GetReadyStatus()) return;
      _plotEvent->SetDrawCanvas(butils::kXYReco, draw); 
    }
    

         
    void SetDrawTrueParticles(Bool_t draw){
      if (!GetReadyStatus()) return;
      _plotEvent->SetDrawObject(butils::kTrueParticles, draw); 
    }
    
    void SetDrawRecoParticles(Bool_t draw){
      if (!GetReadyStatus()) return;
      _plotEvent->SetDrawObject(butils::kRecoParticles, draw); 
    }
  
  protected: 

    enum {kDraw = 57, 
      kNeutral, 
      kXZ, kYZ, kXY,
      kTrue, kReco, 
      kNextBunch, kBackBunch, 
      kNextSpill, kBackSpill, 
      kGoToSpill, 
      kQuit};


    virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2); 

    void QuitProgram(void){
      exit(1);
    }
    
    void DrawEvent(){
      if (!GetReadyStatus()) return;
      
      _plotEvent->DrawEvent(_readEvent->GetEvent());
      
    }
    
    void ChangeBunch(bool forward){
      if (!GetReadyStatus()) return;
      _readEvent->ChangeBunch(forward); 
    }
    
    void GetFwdSpill(){
      if (!GetReadyStatus()) return;
      _readEvent->GetFwdSpill(); 
    }

    void GetBwdSpill(){
      if (!GetReadyStatus()) return;
      _readEvent->GetBwdSpill(); 
    }

    
    void SetDrawNeutral(bool draw){
      if (!GetReadyStatus()) return;
      _plotEvent->SetDrawNeutral(draw); 
    }
 



    /// Event plotter
    plot::EventPlotter*     _plotEvent;
    evrd::EventReader*      _readEvent;

    // Some buttons
    // Control plotting truth neutrals
    TGCheckButton* _button_neutral;

    // Control entering requested spill
    TGNumberEntry* _spill_number;

    // Control plotting projections
    TGCheckButton* _canvas_control[butils::kNPrjTypes];
    // Control plotting objects
    TGCheckButton*  _obj_control[butils::kNDrwTypes];

    TGButtonGroup*  _cnv_group; 
    TGButtonGroup*  _obj_group; 

    TRootEmbeddedCanvas* _canvas_main;
    
    ClassDef(ctrl::ControlPanel, 0)

  };
};

#endif


