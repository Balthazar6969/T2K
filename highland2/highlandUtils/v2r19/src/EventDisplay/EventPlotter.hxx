#ifndef EventPlotter_h
#define EventPlotter_h
#include <TCanvas.h>
#include <TH2F.h>
#include <TTree.h>
#include <TBox.h>
#include <TLine.h>
#include <TMarker.h>
#include <TPolyLine.h>
#include <TPolyMarker.h>
#include <TEllipse.h>


#include "BitUtils.hxx"
#include "DetectorForager.hxx"
#include "DrawingAttributes.hxx"

#include "HEPConstants.hxx"
#include "BaseDataClasses.hxx"

namespace plot{

  /// Associate projection type with pad
  extern const butils::PrjType PrjUsed[butils::kNCnvTypes];
   
  /// Associate name with pad
  extern const std::string NmsUsed[butils::kNCnvTypes]; 
  
  // A struct to store some information relevant for drawing in the pad
  // Projection used, overall histo and vector of boxes
  struct PadData{
     
    PadData(){
      Histo = NULL;
      BBoxes.clear();
    }
     
    virtual ~PadData(){
      if (Histo) delete Histo;
      Histo = NULL;
      
      std::vector<TBox*>::iterator it = BBoxes.begin();
      for (; it != BBoxes.end(); it++){
        if (*it) delete (*it);
        (*it) = NULL;
      }
      
      BBoxes.clear();
      
    }
  
    PadData(const PadData& a){
      Proj = a.Proj;
      Histo = NULL;
      BBoxes.clear();
      if (a.Histo) Histo = (TH2*)a.Histo->Clone();
      for (size_t i = 0; i < a.BBoxes.size(); i++){
        BBoxes.push_back(NULL);
        if (a.BBoxes[i]) BBoxes[i] = (TBox*)(a.BBoxes[i])->Clone();  
      }  
    } 
    
    void Draw() const{
      if (!Histo) return;
      Histo->Draw();
      std::vector<TBox*>::const_iterator it = BBoxes.begin();
      for (; it != BBoxes.end(); it++){
        if (!(*it)) continue;
        (*it)->Draw();
      }
    }
    
    butils::PrjType Proj;
    TH2* Histo;
    std::vector<TBox*> BBoxes;  
  };


  /// A class to do the drawing for highland2 object containing in event
  class EventPlotter{

  public:

    EventPlotter(){
      _canvasMain        = NULL;
      _detForager        = NULL;
      
      _canvasDrawField    = 0;
      
      // By default set all reco canvas
      SetDrawCanvas(butils::kXZReco, true); 
      SetDrawCanvas(butils::kYZReco, true); 
      SetDrawCanvas(butils::kXYReco, true); 
       
      UpdateCanvasDrawToExpected();
      
      _objDrawField       = 0;
      SetDrawObject(butils::kTrueParticles, true);
      SetDrawObject(butils::kRecoParticles, true);
      
      _drawNeutral = false;
      
    }

    virtual ~EventPlotter(){
      
      // Clean the object scheduled for delete
      for (std::vector<TObject*>::iterator it = _cleanupVector.begin(); it != _cleanupVector.end(); it++) {
        delete *it;
      } 
      
      if (_canvasMain)
        delete _canvasMain;
      _canvasMain = NULL;
      
      _cleanupVector.clear();
      
    }

    /// Do the initialization, 
    /// prepare for drawing
    bool Initialize();


    /// Set the canvas to be able to provide it externally
    /// e.g embedded one,  the class wont own the canvas,  it will be deteted
    /// elsewhere
    bool SetMainCanvas(TCanvas* canvas){
      if (!canvas) return false;
      if (_canvasMain) delete _canvasMain;
      _canvasMain = canvas;
      UpdateMainCanvas(); 
      return true;
    }
    
    TCanvas* GetMainCanvas(){
      return _canvasMain;
    }

    /// Set the canvas to be able to provide it externally
    /// e.g embedded one,  the class wont own the canvas,  it will be deteted
    /// elsewhere
    bool SetDetForager(det::DetectorForager* det){
      if (!det) return false;
      if (_detForager) delete _detForager;
      _detForager = det;
      return true;
    }

    /// Draw the full event
    virtual void DrawEvent(const AnaEventB*);
    

    virtual void DrawEvent(const AnaEventB& event, butils::PrjType prj);
    
    /// Set whether to draw a certain canvas
    void SetDrawCanvas(butils::CnvType type, bool status){  
      butils::BitUtils::SetCnvBitStatus(_canvasDrawFieldExp, type, status);
    }
    
    /// Set whether to draw a certain object type
    void SetDrawObject(butils::DrwType type, bool status){
      butils::BitUtils::SetDrwBitStatus(_objDrawField, type, status);
    }
    
    /// Get whether to draw a certain canvas
    bool GetDrawCanvas(butils::CnvType type){  
      return butils::BitUtils::GetCnvBitUsed(_canvasDrawFieldExp, type);
    }
    
    /// Get whether to draw a certain object type
    bool GetDrawObject(butils::DrwType type){
      return butils::BitUtils::GetDrwBitUsed(_objDrawField, type);
    }
    
    /// Draw neutrals
    void SetDrawNeutral(bool draw){
      _drawNeutral = draw;
    
    }
    
    /// Clean to prepare to draw next event
    void CleanScreen();
 

 
  protected:

    /// Draw the definitions
    virtual void DrawDefinitions(){
      _defTrueParticle.Draw();
      gPad->Update();
    }

    /// Draw true particles
    void DrawTrueParticles(const AnaEventB& event, butils::PrjType);

    /// Draw recon particles
    void DrawParticles(const AnaEventB& event, butils::PrjType);

    /// Draw true particle
    virtual void DrawTrueParticle(const AnaTrueParticleB&, butils::PrjType);

    /// Draw recon particle
    virtual void DrawParticle(const AnaParticleB&, butils::PrjType);

    /// Draw a line object given a collection of points and type
    /// and line attributes
    virtual void DrawLineObject(butils::PrjType, const std::vector<TVector3>&, const datt::HLAttLine&);

    virtual datt::HLAttLine GetLineAttributes(const AnaParticleB& particle) const{ 
      if (particle.GetTrueParticle())
        return GetLineAttributes(*particle.GetTrueParticle());
      AnaTrueParticleB part;
      return GetLineAttributes(part);
    }

    virtual datt::HLAttLine GetLineAttributes(const AnaTrueParticleB& particle) const{
      return  _defTrueParticle.GetAttLine(particle.PDG);
    }
    
    TBox* BoundingBox(butils::PrjType, const det::DetBBox&, const datt::HLAttLine&) const;
    
    virtual datt::HLAttLine GetLineAttributesBox(Int_t)const{
      return datt::HLAttLine(15, 1, 3);
    }
  
    virtual bool CheckAddForProjection(butils::PrjType proj, Int_t index) const{
      (void)proj;
      (void)index;
      
      return true;
    } 
    
  
    TH2* BuildBoundHistogram(butils::PrjType type, const std::string& name, const det::DetBBox& box) const;
      
  private:

    void UpdateMainCanvas();
    
    /// Update to expected and return whether an update was needed
    bool UpdateCanvasDrawToExpected(){
      if (_canvasDrawField != _canvasDrawFieldExp){
        _canvasDrawField = _canvasDrawFieldExp;
        return false;
      }
      return true; 
    } 
   
    /// Reset
    void ResetZoom() const {
      for (int i = 0; i < butils::kNCnvTypes; i++){
        if (!_padDataInfo[i].Histo) continue;
        _padDataInfo[i].Histo->GetXaxis()->UnZoom();
        _padDataInfo[i].Histo->GetYaxis()->UnZoom();
      }
    }
   
    
    /// Main canvas where objects are drawn
    TCanvas* _canvasMain;

    /// Detector info
    det::DetectorForager* _detForager;

    /// A collection of objects to be destroyed when going to draw next event
    std::vector<TObject*> _cleanupVector;

    PadData _padDataInfo[butils::kNCnvTypes];

    
    unsigned long int _canvasDrawField;
    unsigned long int _canvasDrawFieldExp;
    unsigned long int _objDrawField;
    bool _drawNeutral;
   
    datt::PlotAttLineDefTrueParticle _defTrueParticle;


  };
};


#endif


