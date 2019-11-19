#ifndef TDISPLAYEDTruthMODULE_HXX
#define  TDISPLAYEDTruthMODULE_HXX
#include "TChain.h"
#include "TND280Track.hxx"
#include "TND280TrackPropagator.hxx"
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TDisplayedAnalysisModuleBase.hxx"
#include "TTruthUtils.hxx"
#include "TGENIEAnalysisUtils.hxx"
#include "TNEUTAnalysisUtils.hxx"
#endif 
/// Class to display information in TruthModule in the event display.
/// \author Alex Finch (A.Finch@lancaster.ac.uk)
class TDisplayedTruthModule : public TDisplayedAnalysisModuleBase
{
public:
/// Add file named inputfilename to the chain.
     void AddFile(const char * inputfilename );   
    TDisplayedTruthModule();
/// Draw truth information in event number 'entry'.
    void Draw(int entry,int bunch=-2);
    /// Describe truth trajectory index on standard output.
    void Describe(int index);
    /// Describe truth trajectory T on standard output.
    void Describe(ND::TTruthTrajectoriesModule::TTruthTrajectory* T);
    /// Describe truth vertex V  on standard output.
    void Describe(ND::TTruthVerticesModule::TTruthVertex* V);

private:
    /// Construct a TND280Track from truth trajectory. index is used for naming and trkProp is 
    /// the propogator used to draw it.
    TND280Track* MakeTrack(TND280TrackPropagator*   trkProp,
    	Int_t                  index,
    ND::TTruthTrajectoriesModule::TTruthTrajectory* trajectory);
    /// Replace integer reaction code with a meaningful string describing it.
    void NeutReaction(TString& Reaction);
 
    TChain           *fChainGenieRooTracker;
    TChain           *fChainNEUTRooTracker;

    int fIsGENIE;
    int fIsNEUT;
    
    Int_t            fNVtxGenie,fNVtxNEUT;
    TClonesArray     *fVtxGenie,*fVtxNEUT;    

    TGENIEAnalysisUtils *GENIEUtils;
    TNEUTAnalysisUtils *NEUTUtils;
    
};
#endif
