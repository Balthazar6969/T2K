#define DrawingTools_cxx
#include "DrawingTools.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TVector3.h>
#include <TDirectory.h>
#include <TList.h>
#include <TGraphAsymmErrors.h>

#include <sstream>

const int NPARTTYPES=9;
const int NPARENTTYPES=10;
const int NREACTYPES=9;
const int NREACTYPES2=5;
const int NVERTEXTYPES=10;
const int NDETTYPES=9;

const int NSENSETYPES=7;
//const int NNTPCHITSTYPES=5;
const int NNTPCHITSTYPES=10;
const int NMOMRELERRTYPES=9;
int auto_colors[NAUTOCOLORS]  = { 1, 2 ,3 ,4 , 6 ,7, 8, 9, 31, 32};


/*
std::string ntpchits_types[NNTPCHITSTYPES] = {"#hits<19", "18<#hits<41"   , "40<#hits<65", "64<#hits<73", "#hits>72"};
int ntpchits_code[NNTPCHITSTYPES]          = { 0        , 1               , 2            , 3            , 4  };
int ntpchits_colors[NNTPCHITSTYPES]        = { 2        , 3               , 4            , 7            , 6  };
*/

std::string ntpchits_types[NNTPCHITSTYPES] = {"#hits<15", "14<#hits<19", "18<#hits<23",  "22<#hits<27", "26<#hits<31", "30<#hits<35", "34<#hits<41", "40<#hits<65", "64<#hits<73", "#hits>72"};
int ntpchits_code[NNTPCHITSTYPES]          = { 0        , 1            , 2            , 3             , 4            , 5            , 6            , 7            , 8            , 9 };
int ntpchits_colors[NNTPCHITSTYPES]        = { 2        ,3             ,4             , 7             ,6             ,31            ,1             ,5             , 8            , 9};

std::string part_types[NPARTTYPES] = {"mu-" ,"e-","pi-","mu+","e+","pi+","p"  ,"unknown","<70% true-rec pur"};
int pdg[NPARTTYPES]                = {13    ,11  ,-211 ,-13  ,-11 ,211  ,2212 , 0       ,  -1};
int part_colors[NPARTTYPES]             = { 2    ,3   ,4    , 7   ,6   ,31   , 8    , 1      ,   5};
//int colors[NPARTTYPES]             = { 2    ,3   ,4    , 7   ,6   ,31   , 8    , 1      ,   5};


std::string parent_types[NPARENTTYPES] = {"mu-","e-","pi-","mu+","e+","pi+","p","numu","gamma","pi0"};
int parent_pdg[NPARENTTYPES]             = {13       ,11      ,-211     ,-13     ,-11    , 211    ,2212    ,14    , 22     , 113};
int parent_colors[NPARENTTYPES]          = { 2       ,3       ,4        , 7      ,6      ,31      ,1       ,8     , 9       , 9};


std::string reac_types[NREACTYPES] = {"CCQE","RES","DIS","COH","NC","anti-numu","out FGD FV","unknown","<70% true-rec pur"};
int reac_code[NREACTYPES]          = { 0    ,1    ,2    , 3   , 4  , 5         ,6           , -1      , 7 };
int reac_colors[NREACTYPES]        = { 2    ,3    ,4    , 7   , 6  , 31        ,1           ,  8      , 5 };

std::string reac_types2[NREACTYPES2] = {"CC","NC","anti-numu","out FGD FV","<70% true-rec pur"};
int reac_code2[NREACTYPES2]          = { 0    , 4  , 5       , 6          , 7  };
int reac_colors2[NREACTYPES2]        = { 2    , 6  , 31      , 1          , 5 };


std::string sense_types[NSENSETYPES] = {"forward-correct","forward-wrong"   , "backward-correct", "backward-wrong", "no z component", "unknown","<70% true-rec pur"};
int sense_code[NSENSETYPES]          = { 0               , 1                , 2                 , 3               , 4               , 5        , 6};
int sense_colors[NSENSETYPES]        = { 2               , 3                , 4                 , 7               , 6               , 31       , 5};

std::string vertex_types[NVERTEXTYPES] = {"TPC1","TPC2","TPC3","FGD1","FGD2","DsECAL","TrECAL","P0D","SMRD","<70% true-rec pur"};
int vertex_code[NVERTEXTYPES]          = { 1    ,2     ,3     , 4    , 5    , 6   , 7      , 8      , 9    , 10};
int vertex_colors[NVERTEXTYPES]        = { 2    ,3     ,4     , 7    ,6     ,31   ,1       ,8       , 9    , 5};


std::string det_types[NDETTYPES] = {"TPC1","TPC2","TPC3","FGD1","FGD2","P0D","DsECAL","TrECAL","SMRD"};
int det_code[NDETTYPES]          = { 1    ,2     ,3     , 4    , 5    , 6   , 7      , 8      , 9};
int det_colors[NDETTYPES]        = { 2    ,3     ,4     , 7    ,6     ,31   ,1       , 8      , 9};


std::string momrelerr_types[NMOMRELERRTYPES] = {"0<momrelerr<0.05","0.05<momrelerr<0.1","0.1<momrelerr<0.2","0.2<momrelerr<0.3","0.3<momrelerr<0.4","0.4<momrelerr<0.5","0.5<momrelerr<0.6","0.6<momrelerr<0.8","0.8<momrelerr<1"};
int momrelerr_code[NMOMRELERRTYPES]          = { 0    ,1     ,2     , 3    , 4    , 5   , 6      , 7      , 8};
int momrelerr_colors[NMOMRELERRTYPES]        = { 1    ,2     ,4     , 3    , 6    , 7   ,31      , 5      , 8};

//const int NANALYSES=100;

#if defined (__MAKECINT__)
#pragma link C++ class std::vector<Int_t>+;
#pragma link C++ class std::vector<Double_t>+;
#pragma link C++ class std::vector<Int_t*>+;
#pragma link C++ class std::vector<Double_t*>+;
#endif


//********************************************************************
DrawingTools::DrawingTools(){
//********************************************************************

  // By default legend on the right
  SetLegendPos("r");

  // Add track categories for color drawing 
  AddTrackCategory("particle", NPARTTYPES,part_types,pdg,part_colors);
  AddTrackCategory("reaction", NREACTYPES,reac_types,reac_code,reac_colors);
  AddTrackCategory("reaction2", NREACTYPES2,reac_types2,reac_code2,reac_colors2);
  AddTrackCategory("vertex", NVERTEXTYPES,vertex_types,vertex_code,vertex_colors);
  AddTrackCategory("sense", NSENSETYPES,sense_types,sense_code,sense_colors);
  AddTrackCategory("ntpchits", NNTPCHITSTYPES,ntpchits_types,ntpchits_code,ntpchits_colors);
  AddTrackCategory("parent", NPARENTTYPES,parent_types,parent_pdg,parent_colors);
  AddTrackCategory("grandparent", NPARENTTYPES,parent_types,parent_pdg,parent_colors);
  AddTrackCategory("detnotused", NDETTYPES,det_types,det_code,det_colors);
  AddTrackCategory("detused", NDETTYPES,det_types,det_code,det_colors);
  AddTrackCategory("momrelerr", NMOMRELERRTYPES,momrelerr_types,momrelerr_code,momrelerr_colors);

  for (int i=0;i<NAUTOCOLORS;i++)
    _auto_colors[i] = auto_colors[i]; 

  _weight=1.;
  _analysis_index=0; 
  _configuration="";

  _stat_option = 10;
  _titleX="";
  _titleY="";
  _title="";
  _minY = 0;

  _same_level=0;

  _leg=NULL;

}


//**************************************************
void DrawingTools::AddConfiguration(const std::string& conf){
//**************************************************
  
  _conf_map[conf] = std::map<std::string, bool>();

  // 1 single analysis by default
  SetNAnalyses(conf,1);
  SetRefAnalysisIndex(conf,0);
  
  // configuration enabled
  EnableConfiguration(conf);

  // add the TTree of this configuration
  _tree_map[conf] = new TTree(conf.c_str(),conf.c_str());
  _tree_map[conf]->SetDirectory(0);

  _tree_map_vector_counter[conf] = std::map<std::string, std::string>();

  _tree_map_int_vector_var[conf] = std::map<std::string, std::vector<Int_t>* >();  
  _tree_map_double_vector_var[conf] = std::map<std::string, std::vector<Double_t>* >();
  _tree_map_char_vector_var[conf] = std::map<std::string, std::vector<const char*>* >();

  _tree_map_int_var[conf] = std::map<std::string, int>();
  _tree_map_double_var[conf] = std::map<std::string, double>();
  _tree_map_char_var[conf] = std::map<std::string, const char*>();

  // Add variables to the tree for the different track categories
  std::map< std::string, std::vector<TrackType> >::iterator it;  
  for (it=_track_category_map.begin();it!=_track_category_map.end();it++ ){
    std::string categ = it->first;
    AddVar(conf,categ,"I");
  }
  
}


//**************************************************
void DrawingTools::EnableAllConfigurations(){
//**************************************************

  std::map< std::string, std::map< std::string, bool > >::iterator it;
  for (it= GetConfigurations().begin();it!=GetConfigurations().end();it++){
    EnableConfiguration(it->first);
  }

}


//**************************************************
void DrawingTools::DisableAllConfigurations(){
//**************************************************

  std::map< std::string, std::map< std::string, bool > >::iterator it;
  for (it= GetConfigurations().begin();it!=GetConfigurations().end();it++){
    DisableConfiguration(it->first);
  }

}


//**************************************************
int DrawingTools::GetNMaxAnalyses(){
//**************************************************

  int nmax =1;
  std::map< std::string, std::map< std::string, bool > >::iterator it;
  for (it= GetConfigurations().begin();it!=GetConfigurations().end();it++){
    //    if (!ConfigurationEnabled(it->first)) continue;
    if (GetNAnalyses(it->first) > nmax) nmax = GetNAnalyses(it->first);
  }

  return nmax;

}


//********************************************************************
void DrawingTools::Initialize(){
//********************************************************************

  // this function is called before the event loop
  
  // delete all saved histograms
  Reset();

  // add analysis index to the tree
  AddAnalysisVar("ana","I");
  AddVar("refana","I");

}


//********************************************************************
void DrawingTools::InitializeEvent(){
//********************************************************************

  // this function is called at the beginning of each event

  std::map< std::string, std::map<std::string, int> >::iterator cit;
  std::map<std::string, int>::iterator cit2;
  for (cit= _tree_map_int_var.begin();cit!=_tree_map_int_var.end();cit++){    
    std::string conf = cit->first;        
    for (cit2= _tree_map_int_var[conf].begin();cit2!=_tree_map_int_var[conf].end();cit2++){    
      cit2->second = 0;
    }
  }

#if 0 

  std::map< std::string, std::map<std::string, std::vector<Int_t>* > >::iterator  vit;
  std::map<std::string, std::vector<Int_t>* >::iterator vit2;
  std::vector<Int_t>::iterator vit3;
  for (vit= _tree_map_int_vector_var.begin();vit!=_tree_map_int_vector_var.end();vit++){    
    std::string conf = vit->first;        
    for (vit2= _tree_map_int_vector_var[conf].begin();vit2!=_tree_map_int_vector_var[conf].end();vit2++){    
      std::string var = vit2->first;        
      _tree_map_int_vector_var[conf][var]->clear();
     
    }
  }

  std::map< std::string, std::map<std::string, std::vector<Double_t>* > >::iterator  dvit;
  std::map<std::string, std::vector<Double_t>* >::iterator dvit2;
  std::vector<Double_t>::iterator dvit3;
  for (dvit= _tree_map_double_vector_var.begin();dvit!=_tree_map_double_vector_var.end();dvit++){    
    std::string conf = dvit->first;      
    for (dvit2= _tree_map_double_vector_var[conf].begin();dvit2!=_tree_map_double_vector_var[conf].end();dvit2++){    
      std::string var = dvit2->first;
      _tree_map_double_vector_var[conf][var]->clear();
    }
  }

#endif 
}


//********************************************************************
void DrawingTools::InitializeConfiguration(){
//********************************************************************

 // this function is called at the beginning of each configuration
  std::map< std::string, std::map<std::string, int> >::iterator cit;
  std::map<std::string, int>::iterator cit2;
  for (cit= _tree_map_int_var.begin();cit!=_tree_map_int_var.end();cit++){    
    std::string conf = cit->first;        
    for (cit2= _tree_map_int_var[conf].begin();cit2!=_tree_map_int_var[conf].end();cit2++){    
      cit2->second = -999;
    }
  }

  std::map< std::string, std::map<std::string, double> >::iterator dcit;
  std::map<std::string, double>::iterator dcit2;
  for (dcit= _tree_map_double_var.begin();dcit!=_tree_map_double_var.end();dcit++){    
    std::string conf = dcit->first;        
    for (dcit2= _tree_map_double_var[conf].begin();dcit2!=_tree_map_double_var[conf].end();dcit2++){    
      dcit2->second = -999.;
    }
  }

  std::map< std::string, std::map<std::string, std::vector<Int_t>* > >::iterator  vit;
  std::map<std::string, std::vector<Int_t>* >::iterator vit2;
  std::vector<Int_t>::iterator vit3;
  for (vit= _tree_map_int_vector_var.begin();vit!=_tree_map_int_vector_var.end();vit++){    
    std::string conf = vit->first;        
    for (vit2= _tree_map_int_vector_var[conf].begin();vit2!=_tree_map_int_vector_var[conf].end();vit2++){    
      std::string var = vit2->first;        
      for (vit3= _tree_map_int_vector_var[conf][var]->begin();vit3!=_tree_map_int_vector_var[conf][var]->end();vit3++){    
	*vit3 = -999;
      }
    }
  }

  std::map< std::string, std::map<std::string, std::vector<Double_t>* > >::iterator  dvit;
  std::map<std::string, std::vector<Double_t>* >::iterator dvit2;
  std::vector<Double_t>::iterator dvit3;
  for (dvit= _tree_map_double_vector_var.begin();dvit!=_tree_map_double_vector_var.end();dvit++){    
    std::string conf = dvit->first;        
    for (dvit2= _tree_map_double_vector_var[conf].begin();dvit2!=_tree_map_double_vector_var[conf].end();dvit2++){    
      std::string var = dvit2->first;        
      for (dvit3= _tree_map_double_vector_var[conf][var]->begin();dvit3!=_tree_map_double_vector_var[conf][var]->end();dvit3++){    
	*dvit3 = -999;
      }
    }
  }

}


//********************************************************************
void DrawingTools::FinalizeAnalysis(){
//********************************************************************

  FillAnalysisVar("ana", GetAnalysisIndex());

}


//********************************************************************
void DrawingTools::FinalizeConfiguration(){
//********************************************************************

}


//********************************************************************
void DrawingTools::AddTrackCategory(const std::string& categ_name, int ntypes, std::string* name, int* code, int* color){
//********************************************************************

  std::vector<TrackType> types ;
  for (int i=0;i<ntypes;i++){
    TrackType categ;
    categ._color = color[i];
    categ._code = code[i];
    categ._enabled=true;
    categ._name=name[i];
    types.push_back(categ);
  }

  _track_category_map[categ_name] = types;

}


//********************************************************************
void DrawingTools::Finalize(){
//********************************************************************

  // this function is called at the end of each event

}


//********************************************************************
void DrawingTools::Reset(){
//********************************************************************

  // delete all saved histograms

  std::vector<TH1*>::iterator it;
  for (it=_saved_histos.begin();it!=_saved_histos.end(); it++){
    delete *it;
  }
  _saved_histos.clear();

}


//*********************************************************
void DrawingTools::SetLegendParam(double a, double b, double c, double d){
//*********************************************************

  // a = x min (in percentage, from 0 to 1) 
  // b = x max 
  // c = y min 
  // d = y max 

  _legendParam[0]=a;
  _legendParam[1]=b;
  _legendParam[2]=c;
  _legendParam[3]=d;

}


//*********************************************************
void DrawingTools::SetLegendPos(const std::string& pos){
//*********************************************************

  if (pos=="r")
    SetLegendParam(0.7,0.6,0.9,0.9);
  else if (pos=="l")
    SetLegendParam(0.1,0.6,0.3,0.9);
  else if (pos=="c")
    SetLegendParam(0.4,0.6,0.6,0.9);

}


//********************************************************************
void DrawingTools::AddVar(const std::string& name, const std::string& type, double ini){
//********************************************************************

  // Add a variable to the tree for all configurations

  std::map< std::string, std::map< std::string, bool > >::iterator cit;
  for (cit= GetConfigurations().begin();cit!=GetConfigurations().end();cit++){    
    std::string conf = cit->first;
    if (!ConfigurationEnabled(conf)) continue;

    AddVar(conf,name,type,ini);
  }

}


//********************************************************************
void DrawingTools::AddVar(const std::string& conf, const std::string& name, const std::string& type, double ini){
//********************************************************************

  // Add a variable to the tree (for a single configuration)

  if (type=="I"){
    if (_tree_map_int_var[conf].find(name) == _tree_map_int_var[conf].end()){
      _tree_map_int_var[conf][name]=(int)ini;  
      _tree_map[conf]->Branch(name.c_str(), &_tree_map_int_var[conf][name], (name+"/"+type).c_str());
    }
  }
  else if (type=="D"){
    if (_tree_map_double_var[conf].find(name) == _tree_map_double_var[conf].end()){
      _tree_map_double_var[conf][name]=ini;  
      _tree_map[conf]->Branch(name.c_str(), &_tree_map_double_var[conf][name], (name+"/"+type).c_str());
    }
  }
  else if (type=="C"){
    if (_tree_map_char_var[conf].find(name) == _tree_map_char_var[conf].end()){
      _tree_map_char_var[conf][name]="";  
      _tree_map[conf]->Branch(name.c_str(), &_tree_map_char_var[conf][name], (name+"/"+type).c_str());
    }
  }
}


//********************************************************************
void DrawingTools::InitializeVar(const std::string& name, double ini){
//********************************************************************

  // Initialise int and double variables to the value ini

  std::map< std::string, std::map< std::string, bool > >::iterator cit;
  for (cit= GetConfigurations().begin();cit!=GetConfigurations().end();cit++){    
    std::string conf = cit->first;
    if (!ConfigurationEnabled(conf)) continue;
    if (_tree_map_int_var[conf].find(name)!=_tree_map_int_var[conf].end()){
      _tree_map_int_var[conf][name]=(Int_t)ini;	
    }
    else if (_tree_map_double_var[conf].find(name)!=_tree_map_double_var[conf].end()){
      _tree_map_double_var[conf][name]=(Double_t)ini;
    }
  }

}


//********************************************************************
void DrawingTools::AddVectorVar(const std::string& name, const std::string& counter_name,const std::string& type){
//********************************************************************

  // Add a vector variable to the tree for all configurations

  std::map< std::string, std::map< std::string, bool > >::iterator cit;
  for (cit= GetConfigurations().begin();cit!=GetConfigurations().end();cit++){    
    std::string conf = cit->first;
    if (!ConfigurationEnabled(conf)) continue;
  
    AddVectorVar(conf,name,counter_name,type);
  }

}


//********************************************************************
void DrawingTools::AddVectorVar(const std::string& conf, const std::string& name, const std::string& counter_name,const std::string& type){
//********************************************************************

  // Add a vector variable to the tree (for a single configuration)
    
  if (type=="I"){    
    AddVar(conf,counter_name,"I",0);
    _tree_map_vector_counter[conf][name]=counter_name;  
    _tree_map_int_vector_var[conf][name]= new std::vector<Int_t>;  
    _tree_map[conf]->Branch(name.c_str(), &_tree_map_int_vector_var[conf][name]);
  }
  else if (type=="D"){
    AddVar(conf,counter_name,"I",0);
    _tree_map_vector_counter[conf][name]=counter_name;  
    _tree_map_double_vector_var[conf][name]= new std::vector<Double_t>;  
    _tree_map[conf]->Branch(name.c_str(), &_tree_map_double_vector_var[conf][name]);
  }
  else if (type=="C"){
    AddVar(conf,counter_name,"I",0);
    _tree_map_char_vector_var[conf][name]=new std::vector<const char*>;  
    _tree_map[conf]->Branch(name.c_str(), &_tree_map_char_vector_var[conf][name]);
  }
}


//********************************************************************
void DrawingTools::InitializeCounter(const std::string& counter_name){
//********************************************************************

  std::map< std::string, std::map< std::string, bool > >::iterator cit;
  for (cit= GetConfigurations().begin();cit!=GetConfigurations().end();cit++){    
    std::string conf = cit->first;
    if (!ConfigurationEnabled(conf)) continue;
    if (_tree_map_int_var[conf].find(counter_name)!=_tree_map_int_var[conf].end()){
      _tree_map_int_var[conf][counter_name]=0;	
      

      std::map<std::string, std::vector<Int_t>* >::iterator it;
      for(it = _tree_map_int_vector_var[conf].begin();it!=_tree_map_int_vector_var[conf].end();it++){
	std::string name = it->first;
	if (_tree_map_vector_counter[conf][name] == counter_name){
	  _tree_map_int_vector_var[conf][name]->clear();  
	}
      }

      std::map<std::string, std::vector<Double_t>* >::iterator it2;
      for(it2 = _tree_map_double_vector_var[conf].begin();it2!=_tree_map_double_vector_var[conf].end();it2++){
	std::string name = it2->first;
	if (_tree_map_vector_counter[conf][name] == counter_name){
	  _tree_map_double_vector_var[conf][name]->clear();  
	  //	  it->second->clear();  
	}
      }
    }
  }

}


//********************************************************************
void DrawingTools::AddAnalysisVar(const std::string& name, const std::string& type){
//********************************************************************

  // Add an analysis variable to the tree 
  // This is a variable that is different for each virtual analysis 

  std::map< std::string, std::map< std::string, bool > >::iterator cit;
  for (cit= GetConfigurations().begin();cit!=GetConfigurations().end();cit++){    
    std::string conf = cit->first;
    if (!ConfigurationEnabled(conf)) continue;
    
    AddVectorVar(conf,name,"NANALYSES",type);
    
    if (type=="I"){    
      _tree_map_int_vector_var[conf][name]->resize(GetNAnalyses(conf));
    }
    else if (type=="D"){    
      _tree_map_double_vector_var[conf][name]->resize(GetNAnalyses(conf));
    }
    else if (type=="C"){    
      _tree_map_char_vector_var[conf][name]->resize(GetNAnalyses(conf));
    }
  }
}


//********************************************************************
void DrawingTools::InitializeAnalysisVar(const std::string& name, double ini){
//********************************************************************

  // Initialise int and double analysis variables to the value ini

  std::map< std::string, std::map< std::string, bool > >::iterator cit;
  for (cit= GetConfigurations().begin();cit!=GetConfigurations().end();cit++){    
    std::string conf = cit->first;
    if (!ConfigurationEnabled(conf)) continue;
    if (_tree_map_int_vector_var[conf].find(name)!=_tree_map_int_vector_var[conf].end()){
      for (unsigned int i=0;i<_tree_map_int_vector_var[conf][name]->size();i++){
	(*(_tree_map_int_vector_var[conf][name]))[i]=(Int_t)ini;	
      }
    }
    else if (_tree_map_double_vector_var[conf].find(name)!=_tree_map_double_vector_var[conf].end()){
      for (unsigned int i=0;i<_tree_map_double_vector_var[conf][name]->size();i++){
	(*(_tree_map_double_vector_var[conf][name]))[i]=(Double_t)ini;
	
      }
    }
  }

}


//********************************************************************
void DrawingTools::AddAnalysisVectorVar(const std::string& name, int ncomp, const std::string& type){
//********************************************************************

  std::stringstream sncomp;
  sncomp << ncomp;
  
  std::map< std::string, std::map< std::string, bool > >::iterator cit;
  for (cit= GetConfigurations().begin();cit!=GetConfigurations().end();cit++){    
    std::string conf = cit->first;
    if (!ConfigurationEnabled(conf)) continue;
    
    
    if (type=="I"){
      if (_tree_map_ana_int_vector_var[conf].find(name) == _tree_map_ana_int_vector_var[conf].end()){
	_tree_map_ana_int_vector_var[conf][name]= new std::vector<Int_t*>();  
	_tree_map[conf]->Branch(name.c_str(), &_tree_map_ana_int_vector_var[conf][name], (name+"["+sncomp.str()+"]/"+type).c_str());
      }
    }
    else if (type=="D"){
      if (_tree_map_ana_double_vector_var[conf].find(name) == _tree_map_ana_double_vector_var[conf].end()){
	_tree_map_ana_double_vector_var[conf][name]= new std::vector<Double_t*>();  
	_tree_map[conf]->Branch(name.c_str(), &_tree_map_ana_double_vector_var[conf][name], (name+"["+sncomp.str()+"]/"+type).c_str());
      }
    }    
    else if (type=="C"){
    }
  }  
}


//********************************************************************
void DrawingTools::FillVar(const std::string& name, int var){
//********************************************************************
  
  _tree_map_int_var[GetCurrentConfiguration()][name]=var;  
}


//********************************************************************
void DrawingTools::FillVar(const std::string& name, double var){
//********************************************************************
  
  _tree_map_double_var[GetCurrentConfiguration()][name]=var;  
}


//********************************************************************
void DrawingTools::FillVar(const std::string& name, const std::string& var){
//********************************************************************
  
  _tree_map_char_var[GetCurrentConfiguration()][name]=var.c_str();  

}


//********************************************************************
void DrawingTools::FillVectorVar(const std::string& name, unsigned int indx, int var){
//********************************************************************

  std::string counter_name = _tree_map_vector_counter[GetCurrentConfiguration()][name];

  if( indx >=   _tree_map_int_vector_var[GetCurrentConfiguration()][name]->size() ) {

    _tree_map_int_vector_var[GetCurrentConfiguration()][name]->push_back(var);  

    _tree_map_int_var[GetCurrentConfiguration()][counter_name] = _tree_map_int_vector_var[GetCurrentConfiguration()][name]->size();  

  } else {
    (*_tree_map_int_vector_var[GetCurrentConfiguration()][name])[indx] = var;  

    _tree_map_int_var[GetCurrentConfiguration()][counter_name] = indx+1;  

  }

}


//********************************************************************
void DrawingTools::FillVectorVar(const std::string& name, unsigned int indx, double var){
//********************************************************************
  
  std::string counter_name = _tree_map_vector_counter[GetCurrentConfiguration()][name];

  if(  _tree_map_double_vector_var[GetCurrentConfiguration()][name] ) {

    if( indx >=  _tree_map_double_vector_var[GetCurrentConfiguration()][name]->size()  ) {

      _tree_map_double_vector_var[GetCurrentConfiguration()][name]->push_back(var);  

      _tree_map_int_var[GetCurrentConfiguration()][counter_name] = _tree_map_double_vector_var[GetCurrentConfiguration()][name]->size();

    }
    else {
      (*_tree_map_double_vector_var[GetCurrentConfiguration()][name])[indx] = var ;  
      _tree_map_int_var[GetCurrentConfiguration()][counter_name] = indx+1;  
    }

  }

}


//********************************************************************
void DrawingTools::FillAnalysisVar(const std::string& name, int var){
//********************************************************************
  
  (*(_tree_map_int_vector_var[GetCurrentConfiguration()][name]))[GetAnalysisIndex()]=(Int_t)var;  
}


//********************************************************************
void DrawingTools::FillAnalysisVar(const std::string& name, double var){
//********************************************************************
  
  (*(_tree_map_double_vector_var[GetCurrentConfiguration()][name]))[GetAnalysisIndex()]=(Double_t)var;  
}


//********************************************************************
void DrawingTools::FillAnalysisVar(const std::string& name, const std::string& var){
//********************************************************************
  
  (*(_tree_map_char_vector_var[GetCurrentConfiguration()][name]))[GetAnalysisIndex()]=(char *)var.c_str();  
}


//********************************************************************
void DrawingTools::FillAnalysisVectorVar(const std::string& name, const int comp, int var){
//********************************************************************
 
  (*(_tree_map_ana_int_vector_var[GetCurrentConfiguration()][name]))[GetAnalysisIndex()][comp]=(Int_t)var;  
}


//********************************************************************
void DrawingTools::FillTree(){
//********************************************************************

  // Fill the tree for all configurations

  std::map< std::string, std::map< std::string, bool > >::iterator cit;
  for (cit= GetConfigurations().begin();cit!=GetConfigurations().end();cit++){    
    std::string conf = cit->first;
    if (!ConfigurationEnabled(conf)) continue;
    FillVar("NANALYSES", GetNAnalyses(conf));
    FillVar("refana", GetRefAnalysisIndex(conf));
    _tree_map[conf]->Fill();
    
  }
  
}


//********************************************************************
void DrawingTools::FillTree(TrackCategory& track){
//********************************************************************

  // Fill the tree for the current configuration provided the true codes for color drawing

  std::map< std::string, std::vector<TrackType> >::iterator it;
  std::vector<TrackType>::iterator it2;
  
  for (it=_track_category_map.begin();it!=_track_category_map.end();it++ ){
    std::string categ = it->first;
    for (it2=_track_category_map[categ].begin();it2!=_track_category_map[categ].end();it2++ ){    
      std::string type = it2->_name;   
      int code = it2->_code;   
      if (track.CheckTrackType(categ,type,code)) _tree_map_int_var[GetCurrentConfiguration()][categ] = code;   
    }
  }

  FillVar("NANALYSES", GetNAnalyses());
  FillVar("refana", GetRefAnalysisIndex());
  _tree_map[GetCurrentConfiguration()]->Fill();
}


//********************************************************************
void DrawingTools::ReadTree(const std::string& file, const std::string& conf){
//********************************************************************

  // Load a tree from a file

  _file = new TFile(file.c_str(),"READ");
  TTree *t = (TTree*)(_file->Get(conf.c_str()));
  _tree_map[conf]= t;

  // Set as current configuration
  SetCurrentConfiguration(conf);
  //  SetNAnalyses();
  //  SetRefAnalysisIndex();
  
}


//********************************************************************
void DrawingTools::ReadTrees(const std::string& file){
//********************************************************************

  // Load a tree from a file

  _file = new TFile(file.c_str(),"READ");

  TList *keys = _file->GetListOfKeys();
  for (int i=0;i<keys->GetEntries();i++){
    std::string conf = keys->At(i)->GetName();
    TTree *t = (TTree*)(_file->Get(conf.c_str()));
    _tree_map[conf]= t;

    // Set as current configuration
    SetCurrentConfiguration(conf);
    //  SetNAnalyses(conf);
    //  SetRefAnalysisIndex(conf);

  }

}


//********************************************************************
void DrawingTools::WriteTree(const std::string& file, const std::string& conf){
//********************************************************************

  // Write a tree to a file

  _file = new TFile(file.c_str(),"NEW");

  GetTree(conf.c_str())->Write();

  _file->Close();

}


//********************************************************************
void DrawingTools::WriteTrees(const std::string& file){
//********************************************************************

  // Write all trees to a file

  _file = new TFile(file.c_str(),"NEW");

  std::map< std::string, std::map< std::string, bool > >::iterator it;
  for (it= GetConfigurations().begin();it!=GetConfigurations().end();it++){
    std::string conf = it->first;
    if (ConfigurationEnabled(conf))
      GetTree(conf.c_str())->Write();
  }
  _file->Close();  

}


//********************************************************************
//void DrawingTools::PrintSummary(){
//********************************************************************

/*

  std::cout << "   - Avg relative systematic error:  " << _avg_rel_syst_err << std::endl;
  std::cout << "   - Avg relative statistical error: " << _avg_rel_stat_err << std::endl;
  std::cout << "   - Avg relative total error:       " << _avg_rel_total_err << std::endl;
*/
//}


//**************************************************
//void DrawingTools::PrintHisto(const std::string& name, const std::string& categ){
//**************************************************

/*
  std::cout << "-------------------------------------------------------" << std::endl;
  std::cout << "Summary info for histo: " << name << std::endl;
  std::cout << "-------------------------------------------------------" << std::endl;

  if (opt!=0){
    std::map< std::string, std::map< std::string, bool > >::iterator it;
    for (it= GetConfigurations().begin();it!=GetConfigurations().end();it++){
      if (!ConfigurationEnabled(it->first)) continue;
      std::cout << "- configuration: " << it->first << std::endl;
      PrintSummary(it->first, name);
    }

  }
  else{
    std::cout << "avg systematic  error: " << std::endl; 
    std::map< std::string, std::map< std::string, bool > >::iterator it;
    for (it= GetConfigurations().begin();it!=GetConfigurations().end();it++){
      if (!ConfigurationEnabled(it->first)) continue;
      GetCounter(it->first, name).ComputeAvgErrors();
      std::cout << " - " << it->first <<  "\t" <<  GetCounter(it->first, name).GetAvgRelSystError() << std::endl;
    }
    std::cout << "avg statistical error (default):  " << GetCounter("default", name).GetAvgRelStatError() << std::endl; 
    if (CheckHistoConf("all_syst", name,"all")){
      std::cout << "avg statistical error (all syst): " << GetCounter("all_syst", name).GetAvgRelStatError() << std::endl; 
      std::cout << "avg total  error:                 " << GetCounter("all_syst", name).GetAvgRelTotalError() << std::endl; 
    }
  }

  std::cout << "-------------------------------------------------------" << std::endl;
*/
//}

//}


//*********************************************************
TMatrixD DrawingTools::GetSystematicCov(const std::string& var, int nx, double xmin, double xmax, const std::string& cut, bool rms){
//*********************************************************

  double xbins[nx+1];
  return GetSystematicCov(var, nx, GetVariableBins(nx,xmin,xmax,xbins), cut, rms);

}


//*********************************************************
TMatrixD DrawingTools::GetSystematicCov(const std::string& var, int nx, double* xbins, const std::string& cut, bool rms){
//*********************************************************

  // compute the covariance matrix for a 1D histo
  
  if (CheckSystComputed(var,nx,xbins,cut,rms)) return _cov;
    
  double abins[GetNAnalyses()+1];
  
  TH2F h("en","en",nx,xbins,GetNAnalyses(),GetVariableBins(GetNAnalyses(),0,GetNAnalyses(),abins));
  _tree_map[GetCurrentConfiguration()]->Project("en",("ana:"+var).c_str(),cut.c_str());

  return GetSystematicCov(h,rms);

}


//*********************************************************
TH2F* DrawingTools::GetSystematic2DHisto(const std::string& var, int nx, double* xbins, const std::string& cut, const std::string& slevel_name){
//*********************************************************

  // compute the covariance matrix for a 1D histo

  //  if (CheckSystComputed(var,nx,xbins,cut,rms)) return _cov;
      
  double abins[GetNAnalyses()+1];

  std::string name2 = "syst2D_"+slevel_name;

  TH2F *h2 = new TH2F(name2.c_str(),"en",nx,xbins,GetNAnalyses(),GetVariableBins(GetNAnalyses(),0,GetNAnalyses(),abins));
  _saved_histos.push_back(h2);
  _tree_map[GetCurrentConfiguration()]->Project(name2.c_str(),("ana:"+var).c_str(),cut.c_str());

  return h2;

}


//*********************************************************
TH2F* DrawingTools::GetSystematic2DHistoRatio(DrawingTools& data1, DrawingTools& data2, 
					      const std::string& var, int nx, double* xbins, const std::string& cut, double norm,const std::string& slevel_name){
//*********************************************************

  double abins[GetNAnalyses()+1];

  std::string name3 = "ratio_"+slevel_name;

  TH2F *h1 = new TH2F("syst2D_1","en1",nx,xbins,GetNAnalyses(),GetVariableBins(GetNAnalyses(),0,GetNAnalyses(),abins));
  data1.GetTree(GetCurrentConfiguration())->Project("syst2D_1",("ana:"+var).c_str(),cut.c_str());

  TH2F *h2 = new TH2F("syst2D_2","en2",nx,xbins,GetNAnalyses(),GetVariableBins(GetNAnalyses(),0,GetNAnalyses(),abins));
  data2.GetTree(GetCurrentConfiguration())->Project("syst2D_2",("ana:"+var).c_str(),cut.c_str());

  ScaleHisto(h2,norm);

  h1->Sumw2();
  h2->Sumw2();

  // compute the ratio
  TH2F* ratio = new TH2F(*h1);
  ratio->SetName(name3.c_str());
  _saved_histos.push_back(ratio);

  ratio->Divide(h1,h2);

  delete h1;
  delete h2;

  return ratio;

}


//*********************************************************
TMatrixD DrawingTools::GetSystematicCov(TH2F& h, bool rms){
//*********************************************************

  // compute the covariance matrix for a 1D histo
  
  int nx=h.GetNbinsX();

  double nentries[nx][GetNAnalyses()];    
  double avg[nx];
  double nentries_min[nx];
  double nentries_max[nx];
  
  for (int bin=0;bin<nx;bin++){
    avg[bin]=0;
    nentries_min[bin]=1e10;
    nentries_max[bin]=0;
    int nana=0;
    for (int ana=0;ana<GetNAnalyses();ana++){      
      if (!AnalysisEnabled(GetCurrentConfiguration(),ana)) continue;
      nentries[bin][ana]=h.GetBinContent(bin+1,ana+1);
      //      if (nentries[bin][ana]==0) continue;

      avg[bin]+=nentries[bin][ana];      
      nana++;

      if (nentries[bin][ana] > nentries_max[bin])
	nentries_max[bin] = nentries[bin][ana];
      if (nentries[bin][ana] < nentries_min[bin])
	nentries_min[bin] = nentries[bin][ana];
    }
    avg[bin]/=nana;
  }

  _cov.ResizeTo(nx, nx);

  for (int i=0;i<nx;i++){
    for (int j=0;j<nx;j++){
      _cov(i,j)=0;      
      if (rms){
	// standard rms
	int nana=0;
	for (int ana=0;ana<GetNAnalyses();ana++){
	  if (!AnalysisEnabled(GetCurrentConfiguration(),ana)) continue;
	  //	  if (nentries[i][ana]==0 || nentries[j][ana]==0) continue;
	  _cov(i,j) += (nentries[i][ana]- avg[i])*(nentries[j][ana]- avg[j]);
	  nana++;
	}
	_cov(i,j) /= nana;
      }
      else
	// dispersion
	_cov(i,j) = (nentries_max[i]-nentries_min[i])*(nentries_max[j]-nentries_min[j]);
    }
    //    std::cout << i << " \t" << xbins[i] << "-" << xbins[i+1] << "\t ---> " << avg[i] << "+-" << sqrt(_cov(i,i)) << std::endl; 
  }  

  return _cov;

}


//*********************************************************
TH1F* DrawingTools::GetHistoWithSystErrors(TH1F* hstat, TH2F* hsyst2D, bool rms){
//*********************************************************

  TMatrixD cov = GetSystematicCov(*hsyst2D, rms);

  std::string name2 = "hsyst";

  TH1F *hsyst = new TH1F(*hstat);
  hsyst->SetName(name2.c_str());
  _saved_histos.push_back(hsyst);

  // Set the sqrt of the diagonal as error
  for (int i=0;i<hstat->GetNbinsX();i++){
    hsyst->SetBinContent(i+1,hstat->GetBinContent(i+1));
    hsyst->SetBinError(i+1,sqrt(cov(i,i)));
  }

  return hsyst;
    
}


//*********************************************************
TH1F* DrawingTools::AddErrorsInQuadrature(TH1F* h1,TH1F* h2, const std::string& slevel){
//*********************************************************

  std::string name2 = "errors_quad_sum"+slevel;
  
  if (h1->GetNbinsX() != h2->GetNbinsX()) return NULL;

  TH1F *h3 = new TH1F(*h1);
  h3->SetName(name2.c_str());
  _saved_histos.push_back(h3);

  // Set the sqrt of the diagonal as error
  for (int i=0;i<h1->GetNbinsX();i++){
    double err = sqrt(pow(h1->GetBinError(i+1),2)+pow(h2->GetBinError(i+1),2));
    h3->SetBinError(i+1,err);
    h3->SetBinContent(i+1,h1->GetBinContent(i+1));
  }

  return h3;
    
}


//*********************************************************
void DrawingTools::DrawEntries(const std::string& cut, const std::string& opt, const std::string& leg_name){
//*********************************************************

  // Draw the distribution of entries for all refference analyses

  std::string slevel = GetSameLevel(opt);
  if (slevel=="0" && leg_name!="")
    CreateLegend();

  double nentries[GetNAnalyses()];
  double hmin = 1e10;
  double hmax = 0;
  
  TH1F h("en","en",GetNAnalyses(),0,GetNAnalyses());
  _tree_map[GetCurrentConfiguration()]->Project("en","ana",cut.c_str());
  for (int ana=0;ana<GetNAnalyses();ana++){
    nentries[ana]=h.GetBinContent(ana+1);
    if (!AnalysisEnabled(GetCurrentConfiguration(),ana)) continue;
    if (nentries[ana]>hmax) hmax = nentries[ana];
    if (nentries[ana]<hmin) hmin = nentries[ana];    
  }

  TH1F* hentries = new TH1F(("hentries_"+slevel).c_str(),leg_name.c_str(),1000,hmin-(hmax-hmin)/10.,hmax+(hmax-hmin)/10.);
  _saved_histos.push_back(hentries);
  
  for (int ana=0;ana<GetNAnalyses();ana++){
    if (!AnalysisEnabled(GetCurrentConfiguration(),ana)) continue;
    hentries->Fill(nentries[ana]);
  }
  
  int statTemp = _stat_option;
  SetOptStat(1110);
  DrawHisto(hentries,_auto_colors[_same_level],1,1,0,opt,"LE1P");
  SetOptStat(statTemp);

  if (opt.find("N") == std::string::npos && leg_name!="")
    _leg->Draw();

  return;
}


//*********************************************************
void DrawingTools::DrawEntriesRatio(DrawingTools& data1, DrawingTools& data2, const std::string& cut, double norm, const std::string& opt, const std::string& leg_name){
//*********************************************************

  // Draw the distribution of entries for all analyses

  std::string slevel = GetSameLevel(opt);
  if (slevel=="0" && leg_name!="")
    CreateLegend();

  // to reset all histograms of data1 and data2
  std::string slevel_dummy1 = data1.GetSameLevel(opt);
  std::string slevel_dummy2 = data2.GetSameLevel(opt);

  double nentries[GetNAnalyses()];
  double hmin = 1e10;
  double hmax = 0;

  double abins[GetNAnalyses()+1];
  double *xbins = GetVariableBins(GetNAnalyses(),0,GetNAnalyses(),abins);
  
  TH1F* h1 = data1.GetHisto(slevel,"entries_ratio1","ana",GetNAnalyses(),xbins,cut,"",1);
  TH1F* h2 = data2.GetHisto(slevel,"entries_ratio2","ana",GetNAnalyses(),xbins,cut,"",norm);

  //  h1->Sumw2();
  //  h2->Sumw2();
  
  //  ScaleHisto(h2,scale);

  // compute the ratio
  TH1F* ratio = new TH1F(*h1);
  ratio->SetName(("ratio_"+slevel).c_str());
  _saved_histos.push_back(ratio);

  ratio->Divide(h1,h2);

  for (int ana=0;ana<GetNAnalyses();ana++){
    nentries[ana]=ratio->GetBinContent(ana+1);
    if (!AnalysisEnabled(GetCurrentConfiguration(),ana)) continue;
    if (nentries[ana]>hmax) hmax = nentries[ana];
    if (nentries[ana]<hmin) hmin = nentries[ana];    
  }

  TH1F* hentries = new TH1F(("hentries_"+slevel).c_str(),leg_name.c_str(),1000,hmin-(hmax-hmin)/10.,hmax+(hmax-hmin)/10.);
  _saved_histos.push_back(hentries);
  
  for (int ana=0;ana<GetNAnalyses();ana++){
    if (!AnalysisEnabled(GetCurrentConfiguration(),ana)) continue;
    hentries->Fill(nentries[ana]);
  }
  
  int statTemp = _stat_option;
  SetOptStat(1110);
  DrawHisto(hentries,_auto_colors[_same_level],1,1,0,opt,"LE1P");
  SetOptStat(statTemp);

  if (opt.find("N") == std::string::npos && leg_name!="")
    _leg->Draw();

  return;
}


//*********************************************************
void DrawingTools::Draw(const std::string& var, int nx, double xmin, double xmax, const std::string& categ, const std::string& cut,  const std::string& opt, double norm, bool scale_errors){
//*********************************************************

  double xbins[nx+1];
Draw(var, nx, GetVariableBins(nx,xmin,xmax,xbins), categ, cut, opt, norm, scale_errors);

}


//*********************************************************
void DrawingTools::Draw(const std::string& var, int nbins, double* xbins, const std::string& categ, const std::string& cut,  const std::string& opt, double norm,  bool scale_errors){
//*********************************************************

  // Draw 1D histogram for a given set of cuts and binning

  std::string slevel = GetSameLevel(opt);
  // Create the legend
  if (opt.find("N") == std::string::npos)
    CreateLegend();

  // Draw the total
  if (categ=="all"){
        
    TH1F* hall = GetHisto(slevel,"all",var,nbins,xbins,cut,opt,norm,scale_errors);
    hall->SetTitle("all");
    DrawHisto(hall,1,1,1,0,opt,"LE1P","",2);

    return;
  }

  //-------- Now draw the MC track categories -----------------------

  // Summup the track categories
  const int ntypes = _track_category_map[categ].size();
  TH1F* hcum[ntypes];

  int i=0;
  std::vector<TrackType>::reverse_iterator rit;
  for (rit=_track_category_map[categ].rbegin();rit!=_track_category_map[categ].rend();rit++, i++ ){
    std::string type = rit->_name;   
    int code = rit->_code;   
    std::stringstream scode;
    scode << code;

    std::string cut2 = categ+"=="+scode.str();
    if (cut!="") cut2 = cut+" && "+cut2;

    TH1F* ht = GetHisto(slevel,type,var,nbins,xbins,cut2,opt,norm,false);

    hcum[i] = new TH1F(*ht);
    _saved_histos.push_back(hcum[i]);
    hcum[i]->SetName((type+"_cum_"+slevel).c_str());
    hcum[i]->SetTitle(type.c_str());

    if (i>0)
      hcum[i]->Add(hcum[i-1],ht);
  }

  // Draw the track categories
  double maxy = hcum[ntypes-1]->GetBinContent(hcum[ntypes-1]->GetMaximumBin())*1.1;
  i=0;
  std::vector<TrackType>::iterator it;
  for (it=_track_category_map[categ].begin();it!=_track_category_map[categ].end();it++, i++ ){
    TH1F *h = hcum[ntypes-1-i];
    int color = it->_color;
    if (it==_track_category_map[categ].begin() && opt.find("same")==std::string::npos){
      h->GetYaxis()->SetRangeUser(0,maxy);
      DrawHisto(h,color,1,color,1001,"","f","",1);
    }
    else
      DrawHisto(h,color,1,color,1001,"same","f","",1);
  }

  // Draw the legend
  if (opt.find("N") == std::string::npos)
    _leg->Draw();

}


//*********************************************************
void DrawingTools::Draw(const std::string& var, int nx, double xmin, double xmax, int ny, double ymin, double ymax, 
			const std::string& categ, const std::string& cut,  const std::string& opt, double norm){
//*********************************************************

  double xbins[nx+1];
  double ybins[ny+1];
  Draw(var, nx, GetVariableBins(nx,xmin,xmax,xbins), ny, GetVariableBins(ny,ymin,ymax,ybins), categ, cut, opt, norm);

}


//*********************************************************
void DrawingTools::Draw(const std::string& var, int nx, double* xbins, int ny, double* ybins, 
			const std::string& categ, const std::string& cut,  const std::string& opt, double norm){
//*********************************************************

  // Draw 2D histogram for a given set of cuts and binning

  std::string slevel = GetSameLevel(opt);

  // Create the legend
  TLegend *leg = new TLegend(_legendParam[0],_legendParam[1],_legendParam[2],_legendParam[3]);
  leg->SetShadowColor(0);


  std::string name2 = "all_"+slevel;
  
  TH2F* hall = new TH2F(name2.c_str(),"all",nx,xbins,ny,ybins);
  _saved_histos.push_back(hall);
  _tree_map[GetCurrentConfiguration()]->Project(name2.c_str(),var.c_str(),cut.c_str());

  // Draw the total
  if (categ=="all"){
    //    ScaleHisto(hall,norm);
    DrawHisto(hall,1,1,1,0,opt);
    /*
    leg->AddEntry( hall, "data","LE1P");
    if (opt.find("N") == std::string::npos)
      leg->Draw();
    */
    return;
  }

  const int ntypes = _track_category_map[categ].size();
  TH2F* ht[ntypes];

  // Draw the track categories
  std::vector<TrackType>::iterator it;
  int i=0;
  bool first_drawn_categ=false;
  for (it=_track_category_map[categ].begin();it!=_track_category_map[categ].end();it++, i++){
    std::string type = it->_name;   
    int code = it->_code;   
    std::string name3 = type+"_"+slevel;
    ht[i] = new TH2F(name3.c_str(),"h",nx,xbins,ny,ybins);
    _saved_histos.push_back(ht[i]);

    std::stringstream scode;
    scode << code;

    std::string cut2 = categ+"=="+scode.str();
    if (cut!="") cut2 = cut+" && "+cut2;
    _tree_map[GetCurrentConfiguration()]->Project(name3.c_str(),var.c_str(),cut2.c_str());

    if (ht[i]->GetEntries()==0) continue;

    int color = it->_color;

    leg->AddEntry( ht[i], type.c_str(),"f");

    if (opt.find("same")!=std::string::npos){
      DrawHisto(ht[i],color,1,color,1001,opt.c_str());
    }
    else{
      if (!first_drawn_categ){
	ht[i]->SetEntries(hall->GetEntries());
	DrawHisto(ht[i],color,1,color,1001,opt.c_str());
	first_drawn_categ=true;
      }
      else
	DrawHisto(ht[i],color,1,color,1001,(opt+" same").c_str());
    }
  }

  // Draw the legend
  if (opt.find("N") == std::string::npos)
    leg->Draw();

}


//*********************************************************
void DrawingTools::DrawEff(const std::string& var, int nx, double xmin, double xmax, const std::string& cut1, const std::string& cut2, const std::string& opt, const std::string& leg_name){
//*********************************************************


  double xbins[nx+1];
  DrawEff(var, nx, GetVariableBins(nx,xmin,xmax,xbins), cut1, cut2, opt, leg_name);

}


//*********************************************************
void DrawingTools::DrawEff(const std::string& var, int nbins, double* xbins, const std::string& cut1, const std::string& cut2, const std::string& opt, const std::string& leg_name){
//*********************************************************

  // Draw 1D efficiency, meaning the BayesDivide ratio of two histograms in which the numerator (cut1+cut2) is a subset of the denominator (cut2).

  std::string slevel = GetSameLevel(opt);
  if (slevel=="0" && leg_name!="")
    CreateLegend();


  TH1F* h1 = GetHisto(slevel,"eff1",var,nbins,xbins,(cut1+" && "+cut2),opt,1);
  TH1F* h2 = GetHisto(slevel,"eff2",var,nbins,xbins,cut2,opt,1);

  // compute the efficiency
  TGraphAsymmErrors* eff = new TGraphAsymmErrors(h1);
  eff->BayesDivide(h1,h2);

  eff->GetXaxis()->SetTitle(_titleX.c_str());
  eff->GetYaxis()->SetTitle(_titleY.c_str());
  eff->GetYaxis()->SetRangeUser(0,1.2);    

  eff->SetTitle("");
  eff->SetLineColor(_auto_colors[_same_level]);
  eff->SetMarkerColor(_auto_colors[_same_level]);

  gStyle->SetOptStat(0);
  
  if (opt.find("same")!=std::string::npos)
    //    DrawHisto(eff,1,1,1,0,"P"+opt,"LE1P");
    eff->Draw(("P"+opt).c_str());
  else
    //    DrawHisto(eff,1,1,1,0,"AP"+opt,"LE1P");
    eff->Draw(("AP"+opt).c_str());

  if (opt.find("N") == std::string::npos && leg_name!=""){
    _leg->AddEntry( eff, leg_name.c_str(),"LE1P");
    _leg->Draw();
  }

}


//*********************************************************
void DrawingTools::DrawSignificance(const std::string& var, int nx, double xmin, double xmax, const std::string& cut1, const std::string& cut2, 
				    double norm, double rel_syst, const std::string& opt, const std::string& leg_name){
//*********************************************************

  double xbins[nx+1];
  DrawSignificance(var, nx, GetVariableBins(nx,xmin,xmax,xbins), cut1, cut2, norm, rel_syst, opt, leg_name);

}


//*********************************************************
void DrawingTools::DrawSignificance(const std::string& var, int nbins, double* xbins, const std::string& cut1, const std::string& cut2, 
				    double norm, double rel_syst, const std::string& opt, const std::string& leg_name){
//*********************************************************

  // Draw 1D significance, defined as the number of signal events (bkg subtracted) divided by its error
  // In this case cut2 defines the selected events, while cut1+cut2 is a sunset (the true signal events)

  std::string slevel = GetSameLevel(opt);
  if (slevel=="0" && leg_name!="")
    CreateLegend();

  TH1F* h1 = GetHisto(slevel,"sig1",var,nbins,xbins,(cut1+" && "+cut2),opt,norm);
  TH1F* h2 = GetHisto(slevel,"sig2",var,nbins,xbins,cut2,opt,norm);

  // compute the significance
  TH1F* sig = new TH1F(*h1);
  sig->SetName(("sig_"+slevel).c_str());
  _saved_histos.push_back(sig);


  for (int ibin=0;ibin<sig->GetXaxis()->GetNbins();ibin++){
    double total = h2->GetBinContent(ibin+1);
    double s=0;
    if (total>0){
      double pur = h1->GetBinContent(ibin+1)/total;
      s = pur/(sqrt(1/(total*norm) + pow(rel_syst*(1-pur),2)));
    }
    sig->SetBinContent(ibin+1,s);
    sig->SetBinError(ibin+1,0);
  }

  sig->GetXaxis()->SetTitle(_titleX.c_str());
  sig->GetYaxis()->SetTitle("significance");
  sig->SetTitle(leg_name.c_str());

  DrawHisto(sig,_auto_colors[_same_level],1,1,0,opt,"LE1P","NOSTAT");

  if (opt.find("N") == std::string::npos && leg_name!="")
    _leg->Draw();

}


//*********************************************************
void DrawingTools::DrawRatio(const std::string& var, int nx, double xmin, double xmax, const std::string& cut1, const std::string& cut2, const std::string& opt, const std::string& leg_name){
//*********************************************************

  double xbins[nx+1];
  DrawRatio(var, nx, GetVariableBins(nx,xmin,xmax,xbins), cut1, cut2, opt, leg_name);
}


//*********************************************************
void DrawingTools::DrawRatio(const std::string& var, int nbins, double* xbins, const std::string& cut1, const std::string& cut2, const std::string& opt, const std::string& leg_name){
//*********************************************************

  // Ratio between two independent cuts

  std::string slevel = GetSameLevel(opt);
  if (slevel=="0" && leg_name!="")
    CreateLegend();

  TH1F* h1 = GetHisto(slevel,"cut_ratio1",var,nbins,xbins,cut1,opt,1);
  TH1F* h2 = GetHisto(slevel,"cut_ratio2",var,nbins,xbins,cut2,opt,1);

  h1->Sumw2();
  h2->Sumw2();

  // compute the ratio
  TH1F* ratio = new TH1F(*h1);
  ratio->SetName(("ratio_"+slevel).c_str());
  _saved_histos.push_back(ratio);

  ratio->Divide(h1,h2);

  ratio->GetXaxis()->SetTitle(_titleX.c_str());
  ratio->GetYaxis()->SetTitle(_titleY.c_str());
  ratio->SetTitle(leg_name.c_str());

  DrawHisto(ratio,_auto_colors[_same_level],1,1,0,opt,"LE1P","NOSTAT");

  if (opt.find("N") == std::string::npos && leg_name!="")
    _leg->Draw();

}


//**************************************************
void DrawingTools::Draw(DrawingTools& data, DrawingTools& mc, const std::string& var, int nx, double xmin, double xmax, 
			const std::string& categ, const std::string& cut, double norm, const std::string& opt){
//**************************************************

  double xbins[nx+1];
  Draw(data,mc,var,nx,GetVariableBins(nx,xmin,xmax,xbins),categ,cut,norm,opt);
}


//**************************************************
void DrawingTools::Draw(DrawingTools& data, DrawingTools& mc, const std::string& var, int nbins, double* xbins, 
			const std::string& categ, const std::string& cut, double norm, const std::string& opt, bool scale_errors){
//**************************************************

  // Draw data-MC comparison

  int statTemp = _stat_option;

  SetOptStat(0);
  data.Draw(var,nbins,xbins,"all", cut, opt+" N", scale_errors);             
  if (opt.find("ST")!=std::string::npos || opt.find("SYS")!=std::string::npos || opt.find("DIS")!=std::string::npos)
    mc.Draw(var,nbins,xbins,categ,  cut, "ST same",norm, scale_errors);           
  else
    mc.Draw(var,nbins,xbins,categ,  cut, " same",norm,scale_errors);           

  mc.Draw(var,nbins,xbins,"all", cut, opt+"same e2 N",norm,scale_errors);   
  SetOptStat(10);
  data.Draw(var,nbins,xbins, "all",cut, opt+" N same",scale_errors);             

  SetOptStat(statTemp);

}


//**************************************************
//void DrawingTools::Draw(DrawingTools& data, DrawingTools& mc, const std::string& var, int nx, double xmin, double xmax, 
//			const std::string& categ, const std::string& cut, double norm, const std::string& opt){
//**************************************************
/*
  double xbins[nx+1];
  Draw(data,mc,var,nx,GetVariableBins(nx,xmin,xmax,xbins),categ,cut,norm,opt);
}
*/
//**************************************************
//void DrawingTools::Draw(DrawingTools& data, DrawingTools& mc, const std::string& var, int nbins, double* xbins, 
//			const std::string& categ, const std::string& cut, double norm, const std::string& opt){
//**************************************************
/*
  // Draw data-MC comparison

  int statTemp = _stat_option;

  SetOptStat(0);
  data.Draw(var,nbins,xbins,"all", cut, opt+" N");             
  if (opt.find("ST")!=std::string::npos || opt.find("SYS")!=std::string::npos || opt.find("DIS")!=std::string::npos)
    mc.Draw(var,nbins,xbins,categ,  cut, "ST same",norm);           
  else
    mc.Draw(var,nbins,xbins,categ,  cut, " same",norm);           

  mc.Draw(var,nbins,xbins,"all", cut, opt+"same e2 N",norm);   
  SetOptStat(10);
  data.Draw(var,nbins,xbins, "all",cut, opt+" N same");             

  SetOptStat(statTemp);

}
*/


//**************************************************
void DrawingTools::DrawRatio(DrawingTools& data1, DrawingTools& data2, const std::string& var, int nx, double xmin, double xmax, 
			     const std::string& cut, double norm, const std::string& opt, const std::string& leg_name){
//**************************************************

  double xbins[nx+1];
  DrawRatio(data1,data2,var,nx,GetVariableBins(nx,xmin,xmax,xbins),cut,norm,opt,leg_name);
}


//**************************************************
void DrawingTools::DrawRatio(DrawingTools& data1, DrawingTools& data2, const std::string& var, int nx, double* xbins, 
		     const std::string& cut, double norm, const std::string& opt, const std::string& leg_name){
//**************************************************

  // Draw data/MC ratio
  
  std::string slevel = GetSameLevel(opt);
  if (slevel=="0" && leg_name!="")
    CreateLegend();
  
  // to reset all histograms of data1 and data2
  std::string slevel_dummy1 = data1.GetSameLevel(opt);
  std::string slevel_dummy2 = data2.GetSameLevel(opt);
  
  TH1F* h1;
  TH1F* h2;
  
  //TH1F* h1 = data1.GetHisto(slevel,"ratio1",var,nx,xbins,cut,"ST",1);
  //TH1F* h2 = data2.GetHisto(slevel,"ratio2",var,nx,xbins,cut,"ST",norm);
  
  if (opt.find("z") != std::string::npos) { // to plot all the analysis
    h1 = data1.GetHisto(slevel,"ratio1",var,nx,xbins,cut,"",1);
    h2 = data2.GetHisto(slevel,"ratio2",var,nx,xbins,cut,"",norm);
  }
  else { 
    h1 = data1.GetHisto(slevel,"ratio1",var,nx,xbins,cut,"ST",1);
    h2 = data2.GetHisto(slevel,"ratio2",var,nx,xbins,cut,"ST",norm);
  }
  
  h1->Sumw2();
  h2->Sumw2();
  
  //  ScaleHisto(h2,norm);
  
  std::string name2 = "ratio_"+slevel;
  
  // compute the ratio
  TH1F* ratio = new TH1F(*h1);
  ratio->SetName(name2.c_str());
  _saved_histos.push_back(ratio);
  
  ratio->Divide(h1,h2);
  
  ratio->GetXaxis()->SetTitle(h1->GetXaxis()->GetTitle());
  ratio->GetYaxis()->SetTitle("ratio");
  
  ratio->SetTitle("");
  
  TH1F* ratio2 = ratio;
  
  // Now get the systematic errors 
  if (GetNAnalyses()>1 && (opt.find("SYS") != std::string::npos || opt.find("DIS") != std::string::npos)){
    
    TH2F* hsyst2D = GetSystematic2DHistoRatio(data1, data2, var, nx, xbins, cut, norm, name2);
    TH1F* hsyst;
    if (opt.find("SYS") != std::string::npos)
      hsyst = GetHistoWithSystErrors(ratio, hsyst2D, true);
    else
      hsyst = GetHistoWithSystErrors(ratio, hsyst2D, false);
    
    if (opt.find("ST") != std::string::npos)
      ratio2 = AddErrorsInQuadrature(ratio,hsyst,name2);
    else
      ratio2 = hsyst;
  }
  
  ratio2->GetXaxis()->SetTitle(_titleX.c_str());
  ratio2->GetYaxis()->SetTitle(_titleY.c_str());
  
  ratio2->SetTitle(leg_name.c_str());
  
  DrawHisto(ratio2,_auto_colors[_same_level],1,1,0,opt,"LE1P","NOSTAT");
  
  if (opt.find("N") == std::string::npos && leg_name!="")
    _leg->Draw();
  
}


//**************************************************
void DrawingTools::DrawDeltaRatio(DrawingTools& data1, DrawingTools& data2, const std::string& var, int nx, double xmin, double xmax, 
				  const std::string& cut, const std::string& wocut, double norm, const std::string& opt, const std::string& leg_name){
//**************************************************

  double xbins[nx+1];
  DrawDeltaRatio(data1,data2,var,nx,GetVariableBins(nx,xmin,xmax,xbins),cut,wocut,norm,opt,leg_name);
}


//**************************************************
void DrawingTools::DrawDeltaRatio(DrawingTools& data1, DrawingTools& data2, const std::string& var, int nx, double* xbins, 
				  const std::string& cut, const std::string& wocut, double norm, const std::string& opt, const std::string& leg_name){
//**************************************************

  // Draw data/MC ratio

  std::string slevel = GetSameLevel(opt);
  if (slevel=="0" && leg_name!="")
    CreateLegend();

  // to reset all histograms of data1 and data2
  std::string slevel_dummy1 = data1.GetSameLevel(opt);
  std::string slevel_dummy2 = data2.GetSameLevel(opt);

  TH1F* h1_cut = data1.GetHisto(slevel,"h1_cut",var,nx,xbins,cut,"",1);
  TH1F* h2_cut = data2.GetHisto(slevel,"h2_cut",var,nx,xbins,cut,"",1);

  TH1F* h1_wocut = data1.GetHisto(slevel,"h1_wocut",var,nx,xbins,wocut,"",1);
  TH1F* h2_wocut = data2.GetHisto(slevel,"h2_wocut",var,nx,xbins,wocut,"",1);
  
  // data1 for events that do not pass the cut
  TH1F* h1_nocut = new TH1F(*h1_cut);
  h1_nocut->SetName(("h1_nocut_"+slevel).c_str());
  _saved_histos.push_back(h1_nocut);
  h1_nocut->Add(h1_wocut,h1_cut,1,-1);

  // data2 for events that do not pass the cut
  TH1F* h2_nocut = new TH1F(*h2_cut);
  h2_nocut->SetName(("h2_nocut_"+slevel).c_str());
  _saved_histos.push_back(h2_nocut);
  h2_nocut->Add(h2_wocut,h2_cut,1,-1);

  ScaleHisto(h2_cut,norm);
  ScaleHisto(h2_wocut,norm);
  ScaleHisto(h2_nocut,norm);

  // this lines need to be here, after computing the difference, since the error on the difference should be 
  // just the sqrt of the number of events
  h1_cut->Sumw2();
  h2_cut->Sumw2();
  h1_wocut->Sumw2();
  h2_wocut->Sumw2();

  // compute the data1/data2 ratio with cut
  std::string name_cut = "ratio_cut_"+slevel;
  TH1F* ratio_cut = new TH1F(*h1_cut);
  ratio_cut->SetName(name_cut.c_str());
  _saved_histos.push_back(ratio_cut);
  ratio_cut->Divide(h1_cut,h2_cut);

  // compute the data1/data2 ratio without cut
  TH1F* ratio_wocut = new TH1F(*h1_wocut);
  ratio_wocut->SetName(("ratio_wocut_"+slevel).c_str());
  _saved_histos.push_back(ratio_wocut);
  ratio_wocut->Divide(h1_wocut,h2_wocut);

  // compute the no_cut/cut ratio for data1
  TH1F* ratio1_nocut_cut = new TH1F(*h1_cut);
  ratio1_nocut_cut->SetName(("ratio1_nocut_cut_"+slevel).c_str());
  _saved_histos.push_back(ratio1_nocut_cut);
  ratio1_nocut_cut->Divide(h1_nocut,h1_cut);

  // compute the no_cut/cut ratio for data2
  TH1F* ratio2_nocut_cut = new TH1F(*h2_cut);
  ratio2_nocut_cut->SetName(("ratio2_nocut_cut_"+slevel).c_str());
  _saved_histos.push_back(ratio2_nocut_cut);
  ratio2_nocut_cut->Divide(h2_nocut,h2_cut);

  // compute the difference between data1/data2 ratios
  TH1F* delta_ratio = new TH1F(*ratio_wocut);
  delta_ratio->SetName(("delta_ratio_"+slevel).c_str());
  _saved_histos.push_back(delta_ratio);
  delta_ratio->Add(ratio_wocut,ratio_cut,1,-1);

  for (int i=0;i<delta_ratio->GetNbinsX();i++){
    double r_c = ratio_cut->GetBinContent(i+1);
    double sr_c = ratio_cut->GetBinError(i+1);
    double r1_nc_c = ratio1_nocut_cut->GetBinContent(i+1);
    double r2_nc_c = ratio2_nocut_cut->GetBinContent(i+1);
    double sr1_nc_c = ratio1_nocut_cut->GetBinError(i+1);
    double sr2_nc_c = ratio2_nocut_cut->GetBinError(i+1);

    double alpha = (r1_nc_c - r2_nc_c)/(1+r2_nc_c);
    double err2 = pow(r_c/(1+r2_nc_c),2)*(pow(sr1_nc_c,2)+pow(sr2_nc_c,2)*(1+alpha)) + pow(alpha*sr_c,2);

    delta_ratio->SetBinError(i+1,sqrt(err2));

    cout << "deltaR = " << delta_ratio->GetBinContent(i+1) << endl;
    cout << "err2 = " << err2 << "    err = " << sqrt(err2) << endl;

    /*
    std::cout << h1_nocut->GetBinContent(i+1) << " +- " << h1_nocut->GetBinError(i+1) << std::endl;
    std::cout << h2_nocut->GetBinContent(i+1) << " +- " << h2_nocut->GetBinError(i+1) << std::endl;
    std::cout << h1_cut->GetBinContent(i+1) << " +- " << h1_cut->GetBinError(i+1) << std::endl;
    std::cout << h2_cut->GetBinContent(i+1) << " +- " << h2_cut->GetBinError(i+1) << std::endl;
    std::cout << h1_wocut->GetBinContent(i+1) << " +- " << h1_wocut->GetBinError(i+1) << std::endl;
    std::cout << h2_wocut->GetBinContent(i+1) << " +- " << h2_wocut->GetBinError(i+1) << std::endl;
    std::cout << ratio_cut->GetBinContent(i+1) << " +- " << ratio_cut->GetBinError(i+1) << std::endl;
    std::cout << ratio_wocut->GetBinContent(i+1) << " +- " << ratio_wocut->GetBinError(i+1) << std::endl;
    std::cout << ratio1_nocut_cut->GetBinContent(i+1) << " +- " << ratio1_nocut_cut->GetBinError(i+1) << std::endl;
    std::cout << ratio2_nocut_cut->GetBinContent(i+1) << " +- " << ratio2_nocut_cut->GetBinError(i+1) << std::endl;
    std::cout << alpha << std::endl;
    */

  }
  
  delta_ratio->GetXaxis()->SetTitle(_titleX.c_str());
  delta_ratio->GetYaxis()->SetTitle(_titleY.c_str());
  delta_ratio->SetTitle(leg_name.c_str());

  DrawHisto(delta_ratio,_auto_colors[_same_level],1,1,0,opt,"LE1P","NOSTAT NOMIN");

  if (opt.find("N") == std::string::npos && leg_name!="")
    _leg->Draw();

}


//**************************************************
void DrawingTools::DrawHisto(TH1F* h, int lc, int lw, int fc, int fs, const std::string& opt,const std::string& leg_opt,const std::string& draw_opt, int mode){
//**************************************************

  // Basic function to draw 1D histo

  if (draw_opt.find("NOSTAT")!=std::string::npos)
    gStyle->SetOptStat(0);
  else
    gStyle->SetOptStat(_stat_option);

  gStyle->SetStatX(0.9);
  gStyle->SetStatY(0.95);

  h->SetLineColor(lc);
  h->SetMarkerColor(lc);

  h->SetLineWidth(lw);
  h->SetFillColor(fc);
  h->SetFillStyle(fs);

  int entries = h->GetEntries();
  
  //----- This is to properly normalized bins with different width -------
  if (mode>0){
    float wmin=1e6;
    for (int i=0;i<h->GetNbinsX();i++){
      float w = h->GetBinWidth(i+1);
      if (w<wmin) wmin=w;
    }    
    for (int i=0;i<h->GetNbinsX();i++){
      float w = h->GetBinWidth(i+1);
      float y = h->GetBinContent(i+1);
      if (y==0) continue;
      float yp = y*(wmin/w);
      float e = h->GetBinError(i+1);
      
      h->SetBinContent(i+1,yp);

      if (mode==2){
	float ep = e*(wmin/w);
	h->SetBinError(i+1,ep);

      }
    }
  }

  h->SetEntries(entries);

  if (draw_opt.find("NOMIN")==std::string::npos)
    h->SetMinimum(_minY);

  // get the title for the legend
  std::string title = h->GetTitle();

  h->GetXaxis()->SetTitle(_titleX.c_str());
  h->GetYaxis()->SetTitle(_titleY.c_str());

  h->SetTitle(_title.c_str());
  h->Draw(opt.c_str());

  //Add entry to legend
  if (_leg &&  opt.find("N") == std::string::npos && title!="")
    _leg->AddEntry( h, title.c_str(),leg_opt.c_str());

}


//**************************************************
void DrawingTools::DrawHisto(TH2F* h, int lc, int lw, int fc, int fs, const std::string& opt){
//**************************************************

  // Basic function to draw 2D histo

  gStyle->SetOptStat(_stat_option);
  gStyle->SetStatX(0.9);
  gStyle->SetStatY(0.95);

  h->SetTitle("");

  h->SetLineColor(lc);
  h->SetLineWidth(lw);
  h->SetFillColor(fc);
  h->SetFillStyle(fs);

  h->SetMarkerColor(lc);

  h->Draw(opt.c_str()); 

}


//**************************************************
void DrawingTools::DrawHisto(TH3F* h, int lc, int lw, int fc, int fs, const std::string& opt){
//**************************************************

  // Basic function to draw 3D histo

  gStyle->SetOptStat(_stat_option);
  gStyle->SetStatX(0.9);
  gStyle->SetStatY(0.95);

  h->SetTitle("");

  h->SetLineColor(lc);
  h->SetLineWidth(lw);
  h->SetFillColor(fc);
  h->SetFillStyle(fs);

  h->SetMarkerColor(lc);

  h->Draw(opt.c_str()); 

}


//**************************************************
void DrawingTools::ScaleHisto(TH1* h, double scale, bool scale_errors){
//**************************************************

  // Scale an histogram
  // if scale_errors=true, errors are also scaled
  // This is needed to draw data-MC comparisons or ratios, for which data and MC have different statistics

  if (scale==0 || scale ==1) return;

  if (scale_errors)
    h->Sumw2();
  h->Scale(scale);

}


//*********************************************************
double* DrawingTools::GetVariableBins(int nx, double xmin, double xmax, double* xbins){
//*********************************************************

  for (int i=0;i<nx+1;i++)
    xbins[i]=  xmin+i*(xmax-xmin)/nx;

  return xbins;

}


//*********************************************************
std::string DrawingTools::GetSameLevel(const std::string& opt ){
//*********************************************************

  if (opt.find("same") == std::string::npos){
    Reset();
    _same_level=0;
  }
  else{
    _same_level++;
  }

  std::stringstream slevel;
  slevel << _same_level;

  return slevel.str();
}


//*********************************************************
void DrawingTools::CreateLegend(){
//*********************************************************

  if (_leg) delete _leg;
  _leg = new TLegend(_legendParam[0],_legendParam[1],_legendParam[2],_legendParam[3]);
  _leg->SetShadowColor(0);
  
}


//*********************************************************
bool DrawingTools::CheckSystComputed(const std::string& var, int nx, double* xbins, const std::string& cut, bool rms){
//*********************************************************

  if (_syst_computed && _var_syst==var && _nbins_syst==nx && _cut_syst==cut && _rms_syst == rms){
    bool diff=false;
    for (int i=0;i<nx;i++){
      if (xbins[i] != _xbins_syst[i]) diff=true;
    }
    if (!diff)
      return true;
  }

  _var_syst=var;
  _nbins_syst=nx;
  _xbins_syst=xbins;
  _cut_syst=cut;
  _rms_syst=rms;
  _syst_computed=true;

  return false;
}


//*********************************************************
TH1F* DrawingTools::GetHisto(const std::string& slevel,const std::string& name, const std::string& var, int nx, double* xbins, 
			     const std::string& cut, const std::string& opt, double scale, bool scale_errors){
//*********************************************************

  TH1F* hall;

  std::stringstream sana_ref;
  sana_ref << GetRefAnalysisIndex();

  std::string name2 = name+"_"+slevel;
  
  // Create an histo and project the tree (only statistical errors)
  TH1F* hstat = new TH1F(name2.c_str(),"all",nx,xbins);
  _saved_histos.push_back(hstat);
  
  // if specified (option "SYS") add systematics errors in quadrature
  // option ST  = statistics only
  // option SYS = systematics only
  // option SYS ST = systematics + statistical errors

  if (opt.find("ST")== std::string::npos && opt.find("SYS")== std::string::npos && opt.find("DIS")== std::string::npos)
    _tree_map[GetCurrentConfiguration()]->Project(name2.c_str(),var.c_str(),cut.c_str());
  else{
    // Project the reference histogram
    _tree_map[GetCurrentConfiguration()]->Project(name2.c_str(),var.c_str(),(cut+" && ana=="+sana_ref.str()).c_str());
  }

  hall = hstat;

  // Now get the systematic errors 
  if (GetNAnalyses()>1 && (opt.find("SYS") != std::string::npos || opt.find("DIS") != std::string::npos)){

    TH2F* hsyst2D = GetSystematic2DHisto(var, nx, xbins, cut, name2);
    TH1F* hsyst;

    if (opt.find("SYS") != std::string::npos)
      hsyst = GetHistoWithSystErrors(hstat, hsyst2D, true);
    else
      hsyst = GetHistoWithSystErrors(hstat, hsyst2D, false);

    if (opt.find("ST") != std::string::npos)
      hall = AddErrorsInQuadrature(hstat,hsyst,name2);
    else
      hall = hsyst;
  }

  ScaleHisto(hall,scale,scale_errors);

  return hall;

}


//**************************************************
void DrawingTools::DrawDeltaRatioCut(DrawingTools& data1, DrawingTools& data2, const std::string& var, int nx, double xmin, double xmax, 
				  const std::string& cut, const std::string& wocut, double norm, const std::string& opt, const std::string& leg_name){
//**************************************************

  double xbins[nx+1];
  DrawDeltaRatioCut(data1,data2,var,nx,GetVariableBins(nx,xmin,xmax,xbins),cut,wocut,norm,opt,leg_name);
}


//**************************************************
void DrawingTools::DrawDeltaRatioCut(DrawingTools& data1, DrawingTools& data2, const std::string& var, int nx, double* xbins, 
     const std::string& cut0, const std::string& wocut, double norm, const std::string& opt, const std::string& leg_name){
//**************************************************

  std::string slevel = GetSameLevel(opt);
  if (slevel=="0" && leg_name!="")
    CreateLegend();

  // to reset all histograms of data1 and data2
  std::string slevel_dummy1 = data1.GetSameLevel(opt);
  std::string slevel_dummy2 = data2.GetSameLevel(opt);

  std::string cut = "(" + wocut + " && " + cut0 +")";

  TH1F* h1_cut = data1.GetHisto(slevel,"h1_cut",var,nx,xbins,cut,"",1);
  TH1F* h2_cut = data2.GetHisto(slevel,"h2_cut",var,nx,xbins,cut,"",1);

  TH1F* h1_wocut = data1.GetHisto(slevel,"h1_wocut",var,nx,xbins,wocut,"",1);
  TH1F* h2_wocut = data2.GetHisto(slevel,"h2_wocut",var,nx,xbins,wocut,"",1);

  //  ScaleHisto(h2_cut,norm);
  ScaleHisto(h2_wocut,norm);
  ScaleHisto(h2_cut,norm);

  // this lines need to be here, after computing the difference, since the error on the difference should be 
  // just the sqrt of the number of events
  //h1_nocut->Sumw2();
  //h2_nocut->Sumw2();
  //h1_wocut->Sumw2();
  //h2_wocut->Sumw2();

  // compute the fraction of events that do not pass the cut for data1
  TH1F* ratio_cut1 = new TH1F(*h1_cut);
  ratio_cut1->SetName(("ratio_cut1_"+slevel).c_str());
  _saved_histos.push_back(ratio_cut1);
  ratio_cut1->Divide(h1_cut,h1_wocut);

  // compute the fraction of events that do not pass the cut for data2
  TH1F* ratio_cut2 = new TH1F(*h1_cut);
  ratio_cut2->SetName(("ratio_cut2_"+slevel).c_str());
  _saved_histos.push_back(ratio_cut2);
  ratio_cut2->Divide(h2_cut,h2_wocut);

  // compute the data1-data2 difference of ratios 
  TH1F* delta_ratio = new TH1F(*ratio_cut1);
  delta_ratio->SetName(("delta_ratio_"+slevel).c_str());
  _saved_histos.push_back(delta_ratio);
  delta_ratio->Add(ratio_cut1,ratio_cut2,1,-1);

  for (int i=0;i<delta_ratio->GetNbinsX();i++){
    double p1 = ratio_cut1->GetBinContent(i+1);
    double p2 = ratio_cut2->GetBinContent(i+1);
    double n1 = h1_wocut->GetBinContent(i+1);
    double n2 = h2_wocut->GetBinContent(i+1);

    double err2 = p1*(1-p1)/n1 + p2*(1-p2)/n2;
    //double var = err2 + pow(p1-p2,2);
    double var1 = err2 + pow(p1-p2,2);
    //double rms = sqrt(var);
    double rms = sqrt(var1);

    delta_ratio->SetBinError(i+1,sqrt(err2));

    cout << "  ------- bin " << i+1 << "-------" << endl;
    cout << "p data = " << p1 << endl;
    cout << "p mc   = " << p2 << endl;
    //cout << "p1 = " << p1 << endl;
    //cout << "p2 = " << p2 << endl;
    //cout << "1-p1 = " << 1-p1 << endl;
    //cout << "1-p2 = " << 1-p2 << endl;
    //cout << "n1 = " << n1 << endl;
    //cout << "n2 = " << n2 << endl;
    //cout << " p1*(1-p1)/n1 = " << p1*(1-p1)/n1 << endl;
    //cout << " p2*(1-p2)/n2 = " << p2*(1-p2)/n2 << endl;
    cout << "deltaP = " << delta_ratio->GetBinContent(i+1) << "   err = " << sqrt(err2) << endl;
    //cout << "err2 = " << err2 << " err = " << sqrt(err2) << endl;
    cout << "rms = " << rms << endl;
  }
  
  delta_ratio->GetXaxis()->SetTitle(_titleX.c_str());
  delta_ratio->GetYaxis()->SetTitle(_titleY.c_str());
  delta_ratio->SetTitle(leg_name.c_str());

  DrawHisto(delta_ratio,_auto_colors[_same_level],1,1,0,opt,"LE1P","NOSTAT NOMIN");

  if (opt.find("N") == std::string::npos && leg_name!="")
    _leg->Draw();

}


//**************************************************
void DrawingTools::DrawEffRatio(DrawingTools& data1, DrawingTools& data2, const std::string& var, int nx, double xmin, double xmax, 
				  const std::string& cut, const std::string& wocut, double norm, const std::string& opt, const std::string& leg_name){
//**************************************************

  double xbins[nx+1];
  DrawEffRatio(data1,data2,var,nx,GetVariableBins(nx,xmin,xmax,xbins),cut,wocut,norm,opt,leg_name);
}


//**************************************************
void DrawingTools::DrawEffRatio(DrawingTools& data1, DrawingTools& data2, const std::string& var, int nx, double* xbins, 
     const std::string& cut0, const std::string& wocut, double norm, const std::string& opt, const std::string& leg_name){
//**************************************************

  std::string slevel = GetSameLevel(opt);
  if (slevel=="0" && leg_name!="")
    CreateLegend();

  // to reset all histograms of data1 and data2
  std::string slevel_dummy1 = data1.GetSameLevel(opt);
  std::string slevel_dummy2 = data2.GetSameLevel(opt);

  std::string cut = "(" + wocut + " && " + cut0 +")";

  TH1F* h1_cut = data1.GetHisto(slevel,"h1_cut",var,nx,xbins,cut,"",1);
  TH1F* h2_cut = data2.GetHisto(slevel,"h2_cut",var,nx,xbins,cut,"",1);

  TH1F* h1_wocut = data1.GetHisto(slevel,"h1_wocut",var,nx,xbins,wocut,"",1);
  TH1F* h2_wocut = data2.GetHisto(slevel,"h2_wocut",var,nx,xbins,wocut,"",1);

  //  ScaleHisto(h2_cut,norm);
  ScaleHisto(h2_wocut,norm);
  ScaleHisto(h2_cut,norm);

  // compute the fraction of events that do not pass/pass the cut for data1
  TH1F* ratio_cut1 = new TH1F(*h1_cut);
  ratio_cut1->SetName(("ratio_cut1_"+slevel).c_str());
  _saved_histos.push_back(ratio_cut1);
  ratio_cut1->Divide(h1_cut,h1_wocut);

  // compute the fraction of events that do not pass/pass the cut for data2
  TH1F* ratio_cut2 = new TH1F(*h1_cut);
  ratio_cut2->SetName(("ratio_cut2_"+slevel).c_str());
  _saved_histos.push_back(ratio_cut2);
  ratio_cut2->Divide(h2_cut,h2_wocut);

  // compute the data1-data2 ratio of ratios 
  TH1F* eff_ratio = new TH1F(*ratio_cut1);
  eff_ratio->SetName(("eff_ratio_"+slevel).c_str());
  _saved_histos.push_back(eff_ratio);
  eff_ratio->Divide(ratio_cut1,ratio_cut2);

  for (int i=0;i<eff_ratio->GetNbinsX();i++){

    double n = eff_ratio->GetBinContent(i+1);
    eff_ratio->SetBinContent(i+1,1-n);
    
    double p1 = ratio_cut1->GetBinContent(i+1);
    double p2 = ratio_cut2->GetBinContent(i+1);
    double n1 = h1_wocut->GetBinContent(i+1);
    double n2 = h2_wocut->GetBinContent(i+1);

    double err2 = ( (1/pow(p2,2))*p1*(1-p1)/n1 ) + ( (pow(p1,2)/pow(p2,3))*(1-p2)/n2 );
    eff_ratio->SetBinError(i+1,sqrt(err2));

    cout << "  ------- bin " << i+1 << "-------" << endl;
    cout << "p data = " << p1 << endl;
    cout << "p mc   = " << p2 << endl;
    cout << "ratio = " << n << endl;
    cout << "1-ratio = " << eff_ratio->GetBinContent(i+1) << endl;
    cout << "err2 = " << err2 << "   err = " << sqrt(err2) << "  " << eff_ratio->GetBinError(i+1) << endl;
  }
  
  eff_ratio->GetXaxis()->SetTitle(_titleX.c_str());
  eff_ratio->GetYaxis()->SetTitle(_titleY.c_str());
  eff_ratio->SetTitle(leg_name.c_str());

  DrawHisto(eff_ratio,_auto_colors[_same_level],1,1,0,opt,"LE1P","NOSTAT NOMIN");

  if (opt.find("N") == std::string::npos && leg_name!="")
    _leg->Draw();

}


