#ifndef TrackCategory_h
#define TrackCategory_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <map>
#include <vector>
#include <TH1F.h>
#include <stdio.h>
#include <iostream>

class TrackType {
public :

  std::string _name;
  int _color;
  int _order;
  int _code;
  bool _enabled;

};


class TrackCategory {
public :
  TrackCategory()
    {
      _code_map["reaction"]=0;
      _code_map["particle"]=0;
      _code_map["parent"]=0;
      _code_map["grandparent"]=0;
      _code_map["vertex"]=0;
    }

  TrackCategory(int reac, int part, int parent, int grandparent){
    _code_map["reaction"]=reac;
    _code_map["particle"]=part;
    _code_map["parent"]=parent;
    _code_map["grandparent"]=grandparent;}

  void SetCode(const std::string& comp, int code) { _code_map[comp]=code;}

  void SetCode(const std::string& comp, const std::string& type, bool ok) { _comp_map[comp][type]=ok;}

  int GetCode(const std::string& comp) {
    if (_code_map.find(comp)==_code_map.end()){
      std::cout << "track type '" << comp << "' does not exist in this category" << std::endl;
      return -1;
    }
    return _code_map[comp];
  }

  bool CheckTrackType(const std::string& comp, const std::string& type, int code){
    if (_code_map.find(comp)!=_code_map.end()){
      //      std::cout << "component type '" << comp << "' does not exist" << std::endl;
      //      return false;
      return (_code_map[comp] == code);
    }
    else if (_comp_map.find(comp)!=_comp_map.end())
      return _comp_map[comp][type];
    else return false;
  }


  std::map<std::string, int> _code_map;
  std::map<std::string, std::map<std::string, bool> > _comp_map;

};


#endif


