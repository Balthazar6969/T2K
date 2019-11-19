#include "EventDisplay/ControlPanel.hxx"
#include "EventDisplayND280/DetectorForagerND280.hxx"
#include "EventDisplayND280/EventPlotterND280.hxx"
#include "EventDisplayND280/EventReaderND280.hxx"
#include "Parameters.hxx"
#include <TApplication.h>

int main(int argc, char **argv){
 
  using namespace ctrl;
  
  evrd::EventReader* reader = new evrd::EventReaderND280(argc, argv);
  
  // Initialize GUI
  TApplication theApp("App",&argc,argv);
  
  ctrl::ControlPanel* control = new ctrl::ControlPanel(gClient->GetRoot(), 1400, 700);

  
  control->SetEventReader(reader);
  control->SetEventPlotter(new plot::EventPlotterND280());
  control->SetDetForager(new det::DetectorForagerND280());
  if (!control->Initialize()) exit(1);
  
  theApp.Run();

  return 0;
}


