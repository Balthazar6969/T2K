#ifndef NumuDataMcCompTools_h
#define NumuDataMcCompTools_h


#include "NumuCutsDefinition.h"
#include "NumuDataFiles.h"

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include "TROOT.h"
#include "TStyle.h"
#include "TTree.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TH1.h"
#include "TH2.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TLegend.h"
#include "TVector3.h"

const int NMCMAX=2;
const int NMC0MAX=16;

class NumuDataMcCompTools{

 private:
  TTree *_numutree;

public: 
  
  NumuDataFiles _data;
  TFile *_fdata;
  TFile *_fmc0[4];
  
  // if the option is " " then no output file is written
  NumuDataMcCompTools(string option="RECREATE", int canvas_nbr=0);
  
  void SetNorm(double datapot, double datamc0, double datamc1);

    void MakeNorm(TH1F *hist,int mcfile,double ndata,int option=0);

    void MakeNorm_2d(TH2F *hist,int mcfile,double ndata,int option=0);

    double GetMcPOT(string inputname);

    double GetDataPOT(string inputname);

// add the cuts of each tracks all together 
    std::string GetCut(int icut,string acut,int itrkCut=0);
    
    //define the different MC subsample and attributes name and color to the samples for the plotting
    void DefineMcComp(string *mc_cut,string *mc_cat_name,int *mc_cat_color,int itrkCut );

    void draw_histo(TH1F* h, int lc, int lw, int fc, int fs, char* opt);
   
    void draw_graph(TGraphAsymmErrors* h, int lc, int lw, char* opt);

    void draw_var( TH1F *mup1,int icut,string *var,string *acut,string add_cut,  int dim = NMAXTRACKS, int min=0,int itrkCut=0,bool isMC=1);
  
    void draw_var( int icut, string *var, char* title, int nbins, double xmin, double xmax, double ymax, string *acut,int dim = NMAXTRACKS, int min=0,int itrkCut=0);

    void draw_var_ratio(int icut1, int icut2,string* var, char* title, int nbins, double xmin, double xmax, double ymax, std::string *acut, int dim= NMAXTRACKS, int min=0,int itrkCut=0);

    void draw_var_DataMC_ratio(int icut1,string* var, char* title,int nxbins, double xmin, double xmax,  double ymax, string *acut,int dim= NMAXTRACKS, int min=0,int itrkCut=0,int legPos=0);

    void draw_histo_2d(TH2F* h,  char* opt);

    void draw_var_2d( TH2F *mup1_2d,int icut,string *var,string *acut,string add_cut,  int dim = NMAXTRACKS, int min=0,int itrkCut=0,bool isMC=1);

    void draw_var_2d( int icut, string *var, char* title, int nxbins, double xmin, double xmax,  int nybins, double ymin, double ymax, double zmax, string *acut,int dim = NMAXTRACKS, int min=0,int itrkCut=0);

    void draw_var_ratio_2d(int icut1, int icut2,string* var, char* title,int nxbins, double xmin, double xmax,  int nybins, double ymin, double ymax, double zmax, string *acut,int dim = NMAXTRACKS, int min=0,int itrkCut=0);

    void draw_var_DataMC_ratio_2d(int icut1,string* var, char* title,int nxbins, double xmin, double xmax,  int nybins, double ymin, double ymax, double zmax, string *acut,int dim = NMAXTRACKS, int min=0,int itrkCut=0);

    TFile *_outfile;
    bool _ccqe;
    bool _cc;
    bool _bg_fv_ccqe;
    bool _bg_fv_cc;
    bool _bg_out_fv;
    bool _run_3132;
    bool _run_3334;
    bool _bg;
    bool _comp_genie;
    bool _positive_ana;
    bool _commonVtx_bg;
    //bool _part_types;
    bool _part_types_tr1;
    bool _part_types_tr2;
    bool _out_fv;
    bool _out_fv_int;
    bool _out_fv_part_tr1;
    bool _out_fv_part_tr2;

    bool _nc_bkg;
    bool _draw_histo;
    bool _draw_MC;
    bool _draw_data;
    bool _draw_leg;
    bool _write_in_rootfile;
    bool _normalize_area;
    bool _save_histos;
    bool _logY;
    bool _batch;
    
    TH1F* _h[5];
    TH2F* _h2[5];
    std::string _plot_name;
    TCanvas *_c1;
    int _ncanvas;
    int _nmc;
    int _nmc0;

    string _plotpath;

    NumuCutsDefinition* _cuts;

};


#endif
