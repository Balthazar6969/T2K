#ifndef DrawingTools_h
#define DrawingTools_h

#include <stdio.h>
#include <iostream>
#include <map>
#include <vector>
#include <math.h> 
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>
#include <TNtuple.h>
#include "TVectorD.h"
#include "TMatrixD.h"
#include <TLegend.h>
#include <TClonesArray.h>

#include "TrackCategory.h"

const int NAUTOCOLORS=10;


//-------------------------------------------------------------------
class DrawingTools {
public :

  DrawingTools();
  virtual ~DrawingTools(){}
  
  virtual void Initialize();
  virtual void InitializeEvent();
  virtual void InitializeConfiguration();

  virtual void Finalize();
  virtual void FinalizeConfiguration();
  virtual void FinalizeAnalysis();

  // reset all saved histograms
  void Reset();

  void InitializeAnalysisVar(const std::string& name, double ini);
  void InitializeVar(const std::string& name, double ini);

  void InitializeCounter(const std::string& counter_name);

  void AddTrackCategory(const std::string& comp_name, int ntypes, std::string* name, int* code, int* color);

  void SetEventWeight(double w){_weight=w;}
  double GetEventWeight(){return _weight;}

  //------------ Systematics ----------------

  TMatrixD GetSystematicCov(const std::string& var, int nx, double xmin, double xmax, const std::string& cut, bool rms);
  TMatrixD GetSystematicCov(const std::string& var, int nx, double* xbins, const std::string& cut, bool rms);
  TMatrixD GetSystematicCov(TH2F& h, bool rms);

  TH2F* GetSystematic2DHisto(const std::string& var, int nx, double* xbins, const std::string& cut, const std::string& opt="");
  TH2F* GetSystematic2DHistoRatio(DrawingTools& data1, DrawingTools& data2, 
				  const std::string& var, int nx, double* xbins, const std::string& cut, double norm, const std::string& opt="");

  TH1F* GetHistoWithSystErrors(TH1F* h, TH2F* h2D, bool rms);

  TH1F* AddErrorsInQuadrature(TH1F* h1,TH1F* h2,const std::string& opt="");

  bool CheckSystComputed(const std::string& var, int nx, double* xbins, const std::string& cut, bool rms);

  //--------- Adding variables to tree ------------

  void AddVar(const std::string& conf, const std::string& name, const std::string& type, double ini=-9999);
  void AddVar(const std::string& name, const std::string& type, double ini=-9999);
  void AddVectorVar(const std::string& conf, const std::string& name, const std::string& type, const std::string& counter_name);
  void AddVectorVar(const std::string& name, const std::string& type, const std::string& counter_name);
  void AddAnalysisVar(const std::string& name, const std::string& type);
  void AddAnalysisVectorVar(const std::string& name, int ncomp, const std::string& type);

  //---------- Filling variables --------------

  void FillVectorVar(const std::string& name, unsigned int indx, int var);
  void FillVectorVar(const std::string& name, unsigned int indx, double var);

  void FillAnalysisVar(const std::string& name, int var);
  void FillAnalysisVar(const std::string& name, double var);
  void FillAnalysisVar(const std::string& name, const std::string& var);

  void FillAnalysisVectorVar(const std::string& name, int comp, int var);

  void FillVar(const std::string& name, double var);
  void FillVar(const std::string& name, int var);
  void FillVar(const std::string& name, const std::string& var);

  //--------- Tree operations ------------

  void FillTree();
  void FillTree(TrackCategory& cat);

  TTree* GetTree(const std::string& conf){return _tree_map[conf];}
  void ReadTree(const std::string& file, const std::string& conf);
  void ReadTrees(const std::string& file);
  void WriteTree(const std::string& file, const std::string& conf);
  void WriteTrees(const std::string& file);

  //---------- Drawing functions -------------

  void DrawEntries(const std::string& cut="", const std::string& opt="", const std::string& leg_name="");
  void DrawEntriesRatio(DrawingTools& data1, DrawingTools& data2,
			const std::string& cut="", double norm=1, const std::string& opt="", const std::string& leg_name="");

  void Draw(const std::string& name, int nbins, double* xbins, const std::string& comp, const std::string& cut="", const std::string& opt="", double norm=1,bool scale_errors=true);
  void Draw(const std::string& name, int nbins, double xmin, double xmax, const std::string& comp, const std::string& cut="", const std::string& opt="", double norm=1,bool scale_errors=true);

  void Draw(const std::string& name, int nx, double* xbins, int ny, double* ybins, 
	    const std::string& comp, const std::string& cut="", const std::string& opt="", double norm=1);
  void Draw(const std::string& name, int nx, double xmin, double xmax, int ny, double ymin, double ymax, 
	    const std::string& comp, const std::string& cut="", const std::string& opt="", double norm=1);

  void DrawRatio(const std::string& name, int nbins, double* xbins, const std::string& cut1, const std::string& cut2, const std::string& opt="", const std::string& leg_name="");
  void DrawEff(const std::string& name, int nbins, double* xbins, const std::string& cut1, const std::string& cut2, const std::string& opt="", const std::string& leg_name="");
  void DrawSignificance(const std::string& name, int nbins, double* xbins, const std::string& cut1, const std::string& cut2, 
			double norm=1, double rel_syst=0,const std::string& opt="", const std::string& leg_name="");

  void DrawRatio(const std::string& name, int nx, double xmin, double xmax, const std::string& cut1, const std::string& cut2,  const std::string& opt="", const std::string& leg_name="");
  void DrawEff(const std::string& name, int nx, double xmin, double xmax, const std::string& cut1, const std::string& cut2, const std::string& opt="", const std::string& leg_name="");
  void DrawSignificance(const std::string& name, int nbins, double xmin, double xmax, const std::string& cut1, const std::string& cut2, 
			double norm=1, double rel_syst=0,const std::string& opt="", const std::string& leg_name="");

  // ---------- Data/MC comparisons -----------------

  void Draw(DrawingTools& data, DrawingTools& mc, const std::string& var, int nx, double xmin, double xmax, 
	    const std::string& comp, const std::string& cut="", double norm=1, const std::string& opt="");

  //void Draw(DrawingTools& data, DrawingTools& mc, const std::string& var, int nx, double* xbins, 
  //	    const std::string& comp, const std::string& cut="", double norm=1, const std::string& opt="");
  void Draw(DrawingTools& data, DrawingTools& mc, const std::string& var, int nx, double* xbins, 
	    const std::string& comp, const std::string& cut="", double norm=1, const std::string& opt="",bool scale_errors=true);


  void DrawRatio(DrawingTools& data, DrawingTools& mc, const std::string& var, int nx, double xmin, double xmax, 
		 const std::string& cut="",  double norm=1,const std::string& opt="", const std::string& leg_name="");
  void DrawRatio(DrawingTools& data, DrawingTools& mc, const std::string& var, int nx, double* xbins, 
		 const std::string& cut="",  double norm=1,const std::string& opt="", const std::string& leg_name="");


  void DrawDeltaRatio(DrawingTools& data1, DrawingTools& data2, const std::string& var, int nx, double xmin, double xmax, 
		      const std::string& cut, const std::string& wocut, double norm=1, const std::string& opt="", const std::string& leg_name="");

  void DrawDeltaRatio(DrawingTools& data1, DrawingTools& data2, const std::string& var, int nx, double* xbins, 
		      const std::string& cut, const std::string& wocut, double norm=1, const std::string& opt="", const std::string& leg_name="");


  void DrawDeltaRatioCut(DrawingTools& data1, DrawingTools& data2, const std::string& var, int nx, double xmin, double xmax, 
		      const std::string& cut, const std::string& wocut, double norm=1, const std::string& opt="", const std::string& leg_name="");

  void DrawDeltaRatioCut(DrawingTools& data1, DrawingTools& data2, const std::string& var, int nx, double* xbins, 
		      const std::string& cut, const std::string& wocut, double norm=1, const std::string& opt="", const std::string& leg_name="");


  void DrawEffRatio(DrawingTools& data1, DrawingTools& data2, const std::string& var, int nx, double xmin, double xmax, 
		      const std::string& cut, const std::string& wocut, double norm=1, const std::string& opt="", const std::string& leg_name="");

  void DrawEffRatio(DrawingTools& data1, DrawingTools& data2, const std::string& var, int nx, double* xbins, 
		      const std::string& cut, const std::string& wocut, double norm=1, const std::string& opt="", const std::string& leg_name="");

  //------- Basic drawing functions -------------

  void DrawHisto(TH1F* h, int lc, int lw, int fc, int fs, const std::string& opt="",const std::string& leg_opt="",const std::string& int_opt="",int mode=0);
  void DrawHisto(TH2F* h, int lc, int lw, int fc, int fs, const std::string& opt="");
  void DrawHisto(TH3F* h, int lc, int lw, int fc, int fs, const std::string& opt="");

  void ScaleHisto(TH1* h, double scale, bool scale_errors=true);

  double* GetVariableBins(int nx, double xmin, double xmax, double*);

  std::string GetSameLevel(const std::string& opt );

  TH1F* GetHisto(const std::string& slevel,const std::string& name, const std::string& var, int nx, double* xbins, const std::string& cut, const std::string& opt, double scale, bool scale_errors=true);

  //------ Drawing style ---------------

  void SetLegendParam(double a, double b, double c, double d);
  void SetLegendPos(const std::string&);
  void SetOptStat(int opt){_stat_option=opt;}

  void SetTitleX(const std::string& titleX){_titleX=titleX;}
  void SetTitleY(const std::string& titleY){_titleY=titleY;}
  void SetTitle(const std::string& title){_title=title;}

  void SetMinY(double minY){_minY=minY;}

  void CreateLegend();

  //--------- functions to control different analyses ----------

  // Set and gets the index of the current virtual analysis
  void SetAnalysisIndex(int index){_analysis_index=index;}
  int GetAnalysisIndex(){return _analysis_index;}
  int GetAnalysisIndex(const std::string& conf){
    if (conf == GetCurrentConfiguration())
      return _analysis_index;
    else
      return GetRefAnalysisIndex(conf);
  }

  // Sets and gets the analysis used as refference 
  void SetRefAnalysisIndex(const std::string& conf, int index){_conf_ref_analysis_index[conf]=index;}
  int GetRefAnalysisIndex(const std::string& conf){return _conf_ref_analysis_index[conf];}
  int GetRefAnalysisIndex(){return _conf_ref_analysis_index[GetCurrentConfiguration()];}

  // Sets and gets the number of analyses for a given configuration
  void SetNAnalyses(const std::string& conf, int nana){
    _conf_nanalyses[conf]=nana; 
    _ana_enabled[conf] = std::vector<bool>(); 
    _ana_enabled[conf].resize(nana);
    EnableAllAnalyses(conf);
  }
  int GetNAnalyses(const std::string& conf){return _conf_nanalyses[conf];}
  int GetNAnalyses(){return _conf_nanalyses[GetCurrentConfiguration()];}
  int GetNMaxAnalyses();

  bool AnalysisEnabled(const std::string& conf, int ana){return _ana_enabled[conf][ana];}
  void EnableAnalysis(const std::string& conf, int ana){_ana_enabled[conf][ana]=true;}
  void DisableAnalysis(const std::string& conf, int ana){_ana_enabled[conf][ana]=false;}
  void DisableAllAnalyses(const std::string& conf){for (int i=0;i<GetNAnalyses(conf);i++) DisableAnalysis(conf,i);}
  void EnableAllAnalyses(const std::string& conf){for (int i=0;i<GetNAnalyses(conf);i++) EnableAnalysis(conf,i);}

  // -------- Parameters for the different analyses ---------------

  void AddParameter(const std::string& name){_parameter[name]=std::vector<double>(); _parameter[name].resize(GetNMaxAnalyses());}
  void AddParameterVector(const std::string& name, int nbins){
    _parameter_vector[name]=std::vector< std::vector<double> >(); 
    _parameter_vector[name].resize(GetNMaxAnalyses());
    for (int i=0;i<GetNMaxAnalyses();i++)
      _parameter_vector[name][i].resize(nbins);      
  }

  void SetParameter(const std::string& name, int index, double par){_parameter[name][index]=par;}
  double GetParameter(const std::string& name, int index){return _parameter[name][index];}

  void SetParameter(const std::string& name, int index1, int index2, double par){_parameter_vector[name][index1][index2]=par;}
  double GetParameter(const std::string& name, int index1, int index2){return _parameter_vector[name][index1][index2];}

  //----------- Functions to control configurations ------------------

  std::map< std::string, std::map<std::string, bool> >& GetConfigurations(){return _conf_map;}
  std::string GetCurrentConfiguration(){return _configuration;}
  void SetCurrentConfiguration(const std::string& conf){_configuration=conf;}
  void AddConfiguration(const std::string& conf);
  bool ConfigurationEnabled(const std::string& conf){return _conf_enabled[conf];}
  void EnableConfiguration(const std::string& conf){_conf_enabled[conf]=true;}
  void DisableConfiguration(const std::string& conf){_conf_enabled[conf]=false;}
  void DisableAllConfigurations();
  void EnableAllConfigurations();

  void SetEnabled(const std::string& conf, const std::string& name, bool enabled){_conf_map[conf][name]=enabled;}
  bool IsEnabled(const std::string& conf,  const std::string& name){return _conf_map[conf][name];}
  bool IsEnabled(const std::string& name){return _conf_map[GetCurrentConfiguration()][name];}

 protected:


  double _weight;
  int _analysis_index; 

  TLegend *_leg;
  double _legendParam[4];
  int _stat_option;
  std::string _titleX;
  std::string _titleY;
  std::string _title;
  double _minY;

  std::map< std::string, std::vector<TrackType> > _track_category_map;

  TFile *_file;

  int _same_level;
  int _auto_colors[NAUTOCOLORS];

  TMatrixD _cov;
  bool _syst_computed;
  std::string _var_syst;
  int _nbins_syst;
  double *_xbins_syst;
  std::string _cut_syst;
  bool _rms_syst;

  std::vector< TH1* > _saved_histos;
  std::map< std::string, TTree* > _tree_map;
  std::map< std::string, std::map<std::string, std::string> > _tree_map_vector_counter;

  // Branches in the tree

  std::map< std::string, std::map<std::string, std::vector<Int_t>* > > _tree_map_int_vector_var;
  std::map< std::string, std::map<std::string, std::vector<Double_t>* > > _tree_map_double_vector_var;
  std::map< std::string, std::map<std::string, std::vector<const char*>* > > _tree_map_char_vector_var;

  std::map< std::string, std::map<std::string, int> > _tree_map_int_var;
  std::map< std::string, std::map<std::string, double> > _tree_map_double_var;
  std::map< std::string, std::map<std::string, const char*> > _tree_map_char_var;

  std::map< std::string, std::map<std::string, std::vector<Int_t*>* > > _tree_map_ana_int_vector_var;
  std::map< std::string, std::map<std::string, std::vector<Double_t*>* > > _tree_map_ana_double_vector_var;
  std::map< std::string, std::map<std::string, std::vector<const char*>* > > _tree_map_ana_char_vector_var;

  // configurations

  std::string _configuration;
  std::map< std::string, std::map<std::string, bool> > _conf_map;
  std::map< std::string, int > _conf_nanalyses;
  std::map< std::string, int > _conf_ref_analysis_index;
  std::map< std::string, bool > _conf_enabled;

  // analyses
  std::map< std::string, std::vector<bool> > _ana_enabled;

  // parameters

  std::map<std::string, std::vector<double> > _parameter;
  std::map<std::string, std::vector<std::vector<double> > > _parameter_vector;

};

#endif


