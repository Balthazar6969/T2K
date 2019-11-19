#ifndef TDisplayedP0DECALMODULE_HXX
#define  TDisplayedP0DECALMODULE_HXX
#include "TChain.h"
#include "TND280Track.hxx"
#include "TEveBoxSet.h"
#include "TND280TrackPropagator.hxx"
//#include "ND__TP0DECALReconModule.h"
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TDisplayedAnalysisModuleBase.hxx"
#include "TGlobalAnalysisUtils.hxx"
#endif
/// Class to display information from the P0D Ecal module in event display.
///\author Alex Finch (A.Finch@lancaster.ac.uk)
class TDisplayedP0DECALModule : public TDisplayedAnalysisModuleBase
{
public:
    TDisplayedP0DECALModule();
    /// Draw P0D Ecal Module information in event `entry'.
    void Draw(int entry,int bunch=-2);
 //   void DescribeTrack(int index);
  //  void DescribeShower(TString Name);
  /// Describe a TP0DECALReconModule::TP0DECALReconTrack object on standard output.
    void Describe(ND::TP0DECALReconModule::TP0DECALReconTrack* Track);
  /// Describe a TP0DECALReconModule::TP0DECALReconShower object on standard output.
    void Describe(ND::TP0DECALReconModule::TP0DECALReconShower* Shower);

private:
      /// Construct a TND280Track to represent  a TP0DECALReconTrack.
      /// index is used to name it and trkProp is the propogator used to draw it.
    TND280Track* MakeTrack(TND280TrackPropagator*   trkProp,
    			 Int_t                  index,
    			 ND::TP0DECALReconModule::TP0DECALReconTrack* reconObject);


};
#endif
