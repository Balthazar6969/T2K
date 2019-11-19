#ifndef EventReaderND280_h
#define EventReaderND280_h

#include "EventDisplay/EventReader.hxx"
#include "FlatTreeConverter.hxx"
#include "HighlandMiniTreeConverter.hxx"
#include "oaAnalysisTreeConverter.hxx"

namespace evrd{
 class EventReaderND280: public EventReader{
 public:
   
  EventReaderND280(int argc, char *argv[]): EventReader(argc, argv){
  
    // Add the different converters
    input().AddConverter("FlatTree",       new FlatTreeConverter(true));
    input().AddConverter("MiniTree",       new HighlandMiniTreeConverter(true));
    input().AddConverter("oaAnalysisTree", new oaAnalysisTreeConverter());
   }
   
   virtual ~EventReaderND280(){};
 
 };
 


};


#endif

