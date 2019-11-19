#ifndef DrawingAttributes_h
#define DrawingAttributes_h

#include <TAttLine.h>
#include <TLine.h>
#include <TText.h>

#include "ParticleId.hxx"


/// A class to contain the params for a line to be drawn
/// also owns the corresponding text and line so that
/// to draw definitions if needed
namespace datt{
  
  // Simple struct with parameters
  struct HLAttLine{
    HLAttLine(Color_t lcolor, Width_t lwidth,  Style_t lstyle){
      Color = lcolor;
      Width = lwidth;
      Style = lstyle;
    }
    Color_t Color; 
    Width_t Width;
    Style_t Style;
  };
  
  class PlotAttLine: public TAttLine{
  public:
    PlotAttLine(){
      _name = "";
      _line = NULL;
      _text = NULL; 
    }

    virtual ~PlotAttLine(){
      delete _line;
      _line = NULL;
      delete _text;
      _text = NULL;
    }

    PlotAttLine(const std::string& str, Color_t lcolor, Width_t lwidth,  Style_t lstyle){
      SetLineColor(lcolor);
      SetLineWidth(lwidth);
      SetLineStyle(lstyle);

      _name = str;
      _line  = NULL;
      _text  = NULL;
    }
    
    PlotAttLine(const PlotAttLine& a){
      SetName(a.GetName());
      _line = NULL;
      _text = NULL;
      if (a.GetLine()) SetLine((TLine*)(a.GetLine()->Clone()));
      if (a.GetText()) SetText((TText*)(a.GetText()->Clone()));
    }
    
     void SetLine(TLine* line){
      _line = line;
      if (!_line) return;
      _line->SetLineColor(GetLineColor());
      _line->SetLineWidth(GetLineWidth());
      _line->SetLineStyle(GetLineStyle());
    }
     
     const TLine* GetLine() const{
      return _line;
    }
    
    
    void SetText(TText* text){
      _text = text;
      if (!_text) return;
      _text->SetTextColor(GetLineColor());
    }
    
    const TText* GetText()const{
      return _text;
    }
  
    const std::string& GetName() const{
      return _name;
    }
    
    void SetName(const std::string& name){
      _name = name;
    }
   
    HLAttLine GetAttributes() const{
      return HLAttLine(GetLineColor(), GetLineWidth(), GetLineStyle());
    }
   
    void Draw() const{
      if (_line) _line->Draw();
      if (_text) _text->Draw();
    }
  private:
    
    std::string _name;

    TLine* _line;
    TText* _text;
  };

  class PlotAttLineDef{

  public:
    PlotAttLineDef() : _default_params("Other", 48, 2, 1){
    }

    HLAttLine GetAttLine(Int_t indx) const{
      
      std::map<Int_t, PlotAttLine>::const_iterator it = _params.find(indx); 
      if (it == _params.end())
        return _default_params.GetAttributes();

      return (*it).second.GetAttributes();
    }

    void Draw() const{

      std::map<Int_t, PlotAttLine>::const_iterator it = _params.begin();

      for (; it != _params.end(); it++){
        (*it).second.Draw();
      }

      _default_params.Draw();      
    }

  protected:
    std::map<Int_t, PlotAttLine> _params;
    PlotAttLine _default_params;

  };


  class PlotAttLineDefTrueParticle: public PlotAttLineDef{

  public:
    PlotAttLineDefTrueParticle(){

      // Define the map
     _params[ParticleId::kMuonPDG]     = PlotAttLine("Muon",     2,  2, 1);
     _params[ParticleId::kElectronPDG] = PlotAttLine("Electron", 3,  2, 1);
     _params[ParticleId::kPiNegPDG]    = PlotAttLine("Pi-",      4,  2, 1);
     _params[ParticleId::kKNegPDG]     = PlotAttLine("K-",       94, 2, 1);
     _params[ParticleId::kAntiMuonPDG] = PlotAttLine("Antimuon", 7,  2, 1);
     _params[ParticleId::kPositronPDG] = PlotAttLine("Positron", 6,  2, 1);
     _params[ParticleId::kPiPosPDG]    = PlotAttLine("Pi+",      31, 2, 1);
     _params[ParticleId::kKPosPDG]     = PlotAttLine("K+",       92, 2, 1);
     _params[ParticleId::kProtonPDG]   = PlotAttLine("Proton",   8,  2, 1);

      Float_t pos_dif = 0.8 / _params.size();

      Float_t posX = 0.3;
      Float_t posY = 0.9;

      std::map<Int_t, PlotAttLine>:: iterator it = _params.begin();

      for (; it != _params.end(); it++){
        
       (*it).second.SetLine(new TLine(posX, posY, posX + 0.05, posY)); 
        
        TText* tmpt = new TText(posX+0.1, posY, (*it).second.GetName().c_str());  
        tmpt->SetTextSize(pos_dif * 0.5); 
        (*it).second.SetText(tmpt);
        
        posY -= pos_dif;
      }
      TLine* tmpl = new TLine(posX, posY, posX + 0.05, posY);
      _default_params.SetLine(tmpl);

      TText* tmpt = new TText(posX+0.1, posY, _default_params.GetName().c_str());  
      tmpt->SetTextSize(pos_dif * 0.5); 
      _default_params.SetText(tmpt);
    }

  };

};

#endif



