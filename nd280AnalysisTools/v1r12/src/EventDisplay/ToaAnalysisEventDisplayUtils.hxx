#ifndef ToaAnalysisEventDisplayUtils_h_seen
#define ToaAnalysisEventDisplayUtils_h_seen

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <TApplication.h>
#include <memory>
#include <TCanvas.h>
#include <TF1.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TBox.h>
#include <TEllipse.h>
#include <TGraph.h>
#include <TLine.h>
#include <TText.h>
#include <TMarker.h>
#include <TStyle.h>
#include <TFile.h>
#include <TChain.h>
#include <TGClient.h>
#include <TGButton.h>
#include <TGNumberEntry.h>
#include <TGFrame.h>
#include <map>
#include "libReadoaAnalysisProjectHeaders.h"
#include <TGeoManager.h>
#include "TROOT.h"
#include "TGeoBBox.h"


class ToaAnalysisEventDisplayUtils : public TObject
{

  public:

    class MyMainFrame;

    class TBoxPad;

  public:

    ToaAnalysisEventDisplayUtils();
    ~ToaAnalysisEventDisplayUtils(){};

    void GeomInit();
    void runEvents(std::string,std::string);

    void DrawDetector();
    void DrawEventInfo(int run, int evt);
    void DrawTrack(const ND::TGlobalReconModule::TGlobalPID& globalTrack );
    void DrawTrackDebug(const ND::TGlobalReconModule::TGlobalPID& globalTrack );

    void DrawHits(TClonesArray* hits, int nhits, int hitColor);
    void DrawOutermostHits(const ND::TGlobalReconModule::TOutermostHits& hits, int hitColor);
    void DrawHit(const ND::TGlobalReconModule::TGlobalHit& hit, int hitColor);
    void DrawVertex(const ND::TGlobalReconModule::TGlobalVertex& vertex, int vertexColor);

    void DrawMCTrajectories(int truthColor);
    void DrawMCVertices(int vertexColor);
    void DrawVertices(int npoints, double *x, double *y, double *z, int color, int marker);
    void DrawPoints(int npoints, double *x, double *y, double *z, int color);

    bool FindEvent(TChain& chain,int requestedEvent);

    void InitExplanation();
    void DrawExplanation();

    static bool SortInZ(TVector3 h1, TVector3 h2){return (h1.Z()<h2.Z());}


    void Initialized();

    static const int p_NVTXTYPES = 11;
    static const int p_NTRAJTYPES = 7;
    std::string p_vtxnames[p_NVTXTYPES];
    std::string p_trajnames[p_NTRAJTYPES];
    int p_NTruthVertices[p_NVTXTYPES];
    int p_NTruthTrajs[p_NTRAJTYPES];
    std::string p_ntrajnames[p_NTRAJTYPES];

    int p_EventID;
    int p_RunID;
    int p_SubrunID;

    const static int NDETSEXTRAP = 23;


    Int_t p_NVertices; //! The number of added primary vertices
    Int_t fPVInd; //! The last primary vertex index
    Int_t p_NSVertices;//! Number of added secondary vertices


    Int_t p_NPIDs; //! The number of global objects

    Int_t p_NTPCOthers; //! The number of TPC other objects
    TClonesArray *p_TPCOthers; //! The vector of TPC other objects


    Int_t p_NTPCUnused; //! Number of hits unused in TPC

    Int_t p_NFGD1Unused; //! Number of hits unused in FGD1
    Int_t p_NFGD2Unused;         //!   Number of hits unused in FGD2
    Int_t p_NP0DUnused;         //!   Number of hits unused in P0D


    Int_t p_NSMRDTopUnused;         //!   Number of hits unused in SMRD
    Int_t p_NSMRDBottomUnused;
    Int_t p_NSMRDLeftUnused;
    Int_t p_NSMRDRightUnused;

    //  Int_t p_NECALUnused;         //!   Number of hits unused in ECAL


    //  TOutermostHits p_P0DOutermostHits;

    double p_EarliestTrackMedianHitTime;   //! Median hit time of the earliest track
    Int_t        p_NDelayedClusters;       //! # Delayed clusters in FGD (>200ns)

    // Information about unused FGD hits and outermost FGD hits is saved for
    // each time bin

    Int_t        p_NFgdTimeBins;       //! Number of hit time bins in the FGD as determined by fgdRecon

    Int_t        p_NECAL;     //! Number on objects containing ECAL
    Int_t        p_NTPC;      //! Number on objects containing TPC
    Int_t        p_NFGD;      //! Number on objects containing FGD
    Int_t        p_NP0D;      //! Number on objects containing P0D
    Int_t        p_NSMRD;     //! Number on objects containing SMRD

    Int_t        p_NECALIso;  //! Number on objects in ECAL only
    Int_t        p_NTPCIso;   //! Number on objects in TPC only
    Int_t        p_NFGDIso;   //! Number on objects in FGD only
    Int_t        p_NP0DIso;   //! Number on objects in P0D only
    Int_t        p_NSMRDIso;  //! Number on objects in SMRD only

  public:

    int NBINS;


    TH2F* YZHisto;
    TH2F* XZHisto;
    TH2F* XYHisto;


    TCanvas *YZ;
    TCanvas *XZ;
    TCanvas *XY;

    TBox *boxfgd1_XZ, *boxfgd2_XZ;
    TBox *boxfgd1_YZ, *boxfgd2_YZ;

    TBox *boxtpc1_XZ, *boxtpc2_XZ;
    TBox *boxtpc1_YZ, *boxtpc3_XZ;
    TBox *boxtpc2_YZ, *boxtpc3_YZ;

    TBox* boxtpc_XY;

    TBox *boxP0D_YZ, *boxP0D_XZ;
    TBox *boxDsECAL_YZ, *boxDsECAL_XZ;

    std::vector<TBox> boxTECAL_XZ, boxTECAL_YZ;
    std::vector<TBox> boxPECAL_XZ, boxPECAL_YZ;

    TBox *boxSMRD_YZ[4], *boxSMRD_XZ[4], *boxSMRD_XY[4];

    TLine *tline[3];
    TText *text[8];
    TObject *tmarker[5];

    TText *EvtLabel;

    vector<TBox*> fBoxList;
    vector<TGraph*>  fGraphList;

    int fOutermostHitsColor;
    int fUnusedHitsColor;
    int fOtherColor;
    int fObjectHitsColor;
    int fTrackColor;
    int fTruthColor;
    int fTrueVertexColor;
    int fRecVertexColor;

    int fHitMarker;
    int fTrueVertexMarker;
    int fRecVertexMarker;


    bool fDrawSMRD;
    bool fProportionalHits;

    bool geomInitialized;

    int fCurrentEntry;
    int fNumberEntries;

    bool fDebug;
    bool p_isMC;
    bool using_geometry;

    vector<TBox*> MMvector;
    vector<TBox*> MMvectorXZ;
    vector<TBox*> MMvectorXY;
    map<int,TBoxPad*> waveform;

    TApplication *theApp;
    TCanvas *WaveformCanvas;

    ToaAnalysisEventDisplayUtils::MyMainFrame *mf;

    ND::TGeometrySummaryModule::TDetectorBoundingBox *p_FGD1Active;
    ND::TGeometrySummaryModule::TDetectorBoundingBox *p_FGD2Active;
    ND::TGeometrySummaryModule::TDetectorBoundingBox *p_TPC1Extent;
    ND::TGeometrySummaryModule::TDetectorBoundingBox *p_TPC2Extent;
    ND::TGeometrySummaryModule::TDetectorBoundingBox *p_TPC3Extent;
    ND::TGeometrySummaryModule::TDetectorBoundingBox *p_DSECALExtent;
    ND::TGeometrySummaryModule::TDetectorBoundingBox *p_P0DActive;
    ND::TGeometrySummaryModule::TDetectorBoundingBox *p_TECAL1Extent, *p_TECAL2Extent, *p_TECAL3Extent, *p_TECAL4Extent, *p_TECAL5Extent, *p_TECAL6Extent;
    ND::TGeometrySummaryModule::TDetectorBoundingBox *p_PECAL1Extent, *p_PECAL2Extent, *p_PECAL3Extent, *p_PECAL4Extent, *p_PECAL5Extent, *p_PECAL6Extent;
    ND::TGeometrySummaryModule::TDetectorBoundingBox *p_SMRD1Extent, *p_SMRD2Extent, *p_SMRD3Extent, *p_SMRD4Extent;
    ND::TGeometrySummaryModule::TTPCMicroMegasVolumes *p_MM;

    TClonesArray *p_Vertices, *p_PIDs, *p_FgdTimeBins, *p_DelayedClusters, *p_P0DUnused;

    TClonesArray *p_TruthVertices[p_NVTXTYPES], *p_TruthTrajs[p_NTRAJTYPES];

    TGeoManager* geo;

    ClassDef(ToaAnalysisEventDisplayUtils, 1);

};

class ToaAnalysisEventDisplayUtils::MyMainFrame : public TGMainFrame{

      private:

        TGNumberEntry *fNumber;
        TGTextButton  *fButton1;
        TGTextButton  *fButton2;
        TGTextButton  *fButton3;
        TGTextButton  *fButton4;
        TGTextButton  *fButton5;
        TGLayoutHints *fLayout;
        int val;

        bool fDrawSMRD;
        int fRequestedEvent;
        int fCurrentEntry;
        int fMaxEntry;
        bool fProportionalHits;
        bool fDebug;

     public:

        MyMainFrame(const TGWindow *p, UInt_t w, UInt_t h);
        ~MyMainFrame();
        Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
        void GoToEventNumber();

        int GetRequestedEvent(){return fRequestedEvent;}
        void SetRequestedEvent(int evt){fRequestedEvent=evt;}

        int GetCurrentEntry(){return fCurrentEntry;}
        void SetCurrentEntry(int entry){fCurrentEntry = entry;}
        void SetMaxEntry(int entry){fMaxEntry = entry;}

        bool GetDebug(){return fDebug;}
        bool GetDrawSMRD(){return fDrawSMRD;}
        bool GetProportionalHits(){return fProportionalHits;}

        ClassDef(ToaAnalysisEventDisplayUtils::MyMainFrame, 1);

    };

class ToaAnalysisEventDisplayUtils::TBoxPad : public TBox{

      public:

        int tpc;
        int half;
        int mm;
        int pad;
        vector<double> time;
        vector<double> adc;
        double timemin;
        double timemax;
        TH1F *histo;

        void Add(double t, double a);
        TH1F* histogram();

        using TBox::ExecuteEvent;
        void ExecuteEvent(Int_t event);

        TBoxPad(){};

        ~TBoxPad(){
            if(histo)
                delete histo;
                histo = NULL;
        };

        ClassDef(ToaAnalysisEventDisplayUtils::TBoxPad, 1);

    };




#endif
