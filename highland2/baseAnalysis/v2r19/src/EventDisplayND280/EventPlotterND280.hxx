#ifndef EventPlotterND280_h
#define EventPlotterND280_h

#include "EventDisplay/EventPlotter.hxx"

namespace plot{
  class EventPlotterND280: public plot::EventPlotter{

  public:
    EventPlotterND280(){}
    virtual ~EventPlotterND280(){}


  protected:
   datt::HLAttLine GetLineAttributesBox(Int_t) const;
  
   bool CheckAddForProjection(butils::PrjType proj, Int_t index) const;
  };
};


#endif
