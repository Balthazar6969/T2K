#ifndef NumuEffTools_h
#define NumuEffTools_h

#include "NumuCutsDefinition.h"
#include "NumuDataFiles.h"


#include "TROOT.h"
#include "TStyle.h"
#include "TTree.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TH1.h"
#include "TF1.h"
#include "TH2.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TLegend.h"
#include "TVector3.h"



#define NMCFILES 2
#define NFILES 3
#define NTOTCUTS 100

class NumuEffTools{

 private:

    int  _seltrueEvents[NMCFILES][NTOTCUTS][3];
    int  _fracEvents[NFILES][NTOTCUTS][3];
    int  _Events[NFILES][NTOTCUTS];
    TGraphAsymmErrors* _geff[NMCFILES];
    TGraphAsymmErrors* _gpur[NMCFILES];
    TGraphAsymmErrors* _gcont[NMCFILES];
    TGraphAsymmErrors* _gfrac[NFILES];
    
    TGraphErrors *_gevt[NFILES];
    TGraphErrors *_gdoubleratio[NFILES];
    TGraphErrors *_gratio;
    TGraphErrors *_gratio_neut;
    TGraphErrors *_gratio_genie;
    
    TGraphAsymmErrors* _geffvsvar[NMCFILES];
    TGraphAsymmErrors* _gpurvsvar[NMCFILES];
    
    
public:

    bool _trueParam;
    bool _useDir;
    bool _useDir2;
    bool _fillarrays;
    bool _fillgraphs;
    bool _takestepdef;
    bool _write;
    
    TFile *_myfile;
    TCanvas *_c1;
    TCanvas *_c2;
    TCanvas *_c3;
    NumuDataFiles _data;
    TFile *_ffiles[3];//0=NEUT,1=GENIE, 2=DATA


    double _dstep[NTOTCUTS];
    int _cutSTART;
    int _fileSTART;
    int _NMCFILES;

    TH1F *_heffvsvar[NMCFILES];
    TH1F *_hpurvsvar[NMCFILES];

    TH2F* _heffvsvar2d[NMCFILES];
    TH2F* _hpurvsvar2d[NMCFILES];

    
    NumuEffTools();
    
    void setstep(int dim,double *idstep);
    double getstep(int icut);
    
    void FillSelTrueEventsArray( std::string *true_cut,  std::string *seltrue_cut, std::string *sel_cut,int NCUTS2, double xmin=0,double xmax=0);
    void FillEffPurGraphs(int NCUTS2,double xmin,double xmax);
    void FillContGraphs(int NCUTS2,double xmin,double xmax,int color);
    void DrawEffPur( std::string *true_cut,  std::string *seltrue_cut, std::string *sel_cut,int NCUTS2,std::string name,string title,double xmin=0,double xmax=0,double ymin=0,double ymax=0,string xAxisLabel="cut", int legPos=0);
    void DrawEff( std::string *true_cut,  std::string *seltrue_cut, std::string *sel_cut,int NCUTS2,std::string name,string title,double xmin=0,double xmax=0,double ymin=0,double ymax=0,string xAxisLabel="cut", int legPos=0,string drawOption="");
    void DrawPur( std::string *true_cut,  std::string *seltrue_cut, std::string *sel_cut,int NCUTS2,std::string name,string title,double xmin=0,double xmax=0,double ymin=0,double ymax=0,string xAxisLabel="cut", int legPos=0,string drawOption=""); 
    void DrawCont( std::string *true_cut,  std::string *seltrue_cut, std::string *sel_cut,int NCUTS2,std::string name,string title,double xmin=0,double xmax=0,double ymin=0,double ymax=0,string xAxisLabel="cut", int legPos=0,string legname="",int color =56,string drawOption="");
    
    void FillFracEventsArray( std::string *num_cut,std::string *denom_cut,int NCUTS2, double xmin=0,double xmax=0,int NFILES2=NFILES);
    void FillFracGraphs(int NCUTS2, double xmin,double xmax,int NFILES2,int color);
    void FillFracGraphs(TGraphAsymmErrors **gfrac,int NCUTS2, double xmin,double xmax,int NFILES2,int color);
    void DrawFrac( std::string *num_cut, std::string *denom_cut, int NCUTS2,std::string name,string title,double xmin=0,double xmax=0,double ymin=0,double ymax=0,int NFILES2=NFILES,string xAxisLabel="cut", int legPos=0,string legname="",int color =95,string drawOption="same");
    
    void FillEventsArray(std::string *sel_cut,int NCUTS2, double xmin=0,double xmax=0);
    void FillEventsGraphs(int NCUTS2, double xmin,double xmax);
    void DrawEv( std::string *sel_cut,int NCUTS2,std::string nameEv,string titleEv, double xmin=0,double xmax=0,double ymin=0,double ymax=0,string xAxisLabel="cut", int legPos=0);
    void DrawEvRatio(std::string *sel_cut,int NCUTS2,string nameEvRatio, string titleEvRatio, double xmin=0,double xmax=0,double ymin=0,double ymax=0,string xAxisLabel="cut", int legPos=0,bool dofit=false);
    void FillDoubleRatioGraphs( TGraphAsymmErrors **gfrac1, TGraphAsymmErrors **gfrac2,int NCUTS2, double xmin,double xmax,int NFILES2,int color);
    void DrawDoubleRatio( std::string *num_cut1, std::string *denom_cut1,std::string *num_cut2, std::string *denom_cut2, int NCUTS2,std::string name,string title,double xmin=0,double xmax=0,double ymin=0,double ymax=0,int NFILES2=NFILES,string xAxisLabel="cut", int legPos=0,string legname="",int color =1,bool dofit=false);

    void FillEffPurvsVar(std::string var,std::string true_cut,  std::string seltrue_cut, std::string sel_cut,double nbins,double xmin=0,double xmax=0);
    void DrawEffvsVar(std::string var,std::string true_cut,  std::string seltrue_cut, std::string sel_cut,std::string name,string title,double nbins,double xmin=0,double xmax=0,string xAxisLabel=" ", int legPos=0);
    void DrawPurvsVar(std::string var,std::string true_cut,  std::string seltrue_cut, std::string sel_cut,std::string name,string title,double nbins,double xmin=0,double xmax=0,string xAxisLabel=" ", int legPos=0);
    
    void FillEffPurvsVar2d(std::string varx,std::string vary,std::string true_cut,  std::string seltrue_cut, std::string sel_cut,double nxbins,double xmin,double xmax,double nybins,double ymin,double ymax);
    void DrawEffvsVar2d(std::string varx,std::string vary,std::string true_cut,  std::string seltrue_cut, std::string sel_cut,std::string name,string title,double nxbins,double xmin,double xmax,double nybins, double ymin, double ymax,string xAxisLabel=" ",string yAxisLabel=" " );
    void DrawPurvsVar2d(std::string varx,std::string vary,std::string true_cut,  std::string seltrue_cut, std::string sel_cut,std::string name,string title,double nxbins,double xmin,double xmax,double nybins, double ymin, double ymax,string xAxisLabel=" ",string yAxisLabel=" " );
        
  
    
};


#endif

