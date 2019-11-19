#ifndef CreateMiniTree_h
#define CreateMiniTree_h

#include "SimpleLoopBase.hxx"
#include "DataClasses.hxx"
#include "InputManager.hxx"
#include "RooTrackerVtxManager.hxx"
#include <set>

class CreateMiniTree: public SimpleLoopBase {
 public:

  CreateMiniTree(int argc, char *argv[]);
  virtual ~CreateMiniTree(){}

protected:

  //---- These are mandatory functions
  virtual bool Initialize     ();
  virtual bool InitializeSpill(){return _rooVtxManager.InitializeEntry();}

  virtual void DefineOutputTree();

  virtual void FinalizeSpill(){}
  virtual void Finalize     ();

  virtual bool Process();
  //--------------------

  virtual bool CheckTruthFillMiniTree(const AnaSpill& spill);
  virtual bool CheckReconFillMiniTree(const AnaBunchB& bunch);
  virtual bool CheckReconFillMiniTreeOutOfBunch(const AnaBunchB& bunch);

  virtual bool CheckTrueVertexReaction(const AnaTrueVertex& vtx);
  virtual bool CheckTrueVertexDetector(unsigned long det);

  virtual void FillMiniTree();
  virtual void FillRooTrackerVtxTree();
  virtual void FilterRooTrackerVtxTree();

  virtual void WriteGeometry();

  void DeleteUninterestingBunches();

  virtual AnaSpillB* MakeSpill(){return new AnaSpill();}
  virtual AnaBeamB*  MakeBeam() {return new AnaBeam();}

  enum miniTreeIndex{
    minitree = OutputManager::enumSpecialTreesLast+1
  };


protected:

  RooTrackerVtxManager _rooVtxManager;

  bool _saveGeometry;
  bool _saveRoo;
  bool _filterRoo;

  bool _forceSaveAll;

  bool _saveTPCInfo;
  bool _saveFGDInfo;
  bool _saveECALInfo;
  bool _saveP0DInfo;
  bool _saveSMRDInfo;
  bool _saveTrackerInfo;

  // read the parameters file to decide if information from the local reconstruction should
  // be saved to the mini tree.
  bool _saveReconDirTECALInfo;
  bool _saveReconDirPECALInfo;
  bool _saveReconDirP0DInfo;
  bool _saveReconDirFGDOnlyInfo;

  bool _useTPC1;
  bool _useTPC2;
  bool _useTPC3;
  bool _useFGD1;
  bool _useFGD2;
  bool _useP0D;
  bool _useDsECal;
  bool _useTrECal;
  bool _useP0DECal;
  bool _useSMRD;

  bool _useTPC1outOfBunch;
  bool _useTPC2outOfBunch;
  bool _useTPC3outOfBunch;
  bool _useFGD1outOfBunch;
  bool _useFGD2outOfBunch;
  bool _useP0DoutOfBunch;
  bool _useDsECaloutOfBunch;
  bool _useTrECaloutOfBunch;
  bool _useP0DECaloutOfBunch;
  bool _useSMRDoutOfBunch;

  bool _saveTrueNuNC;
  bool _saveTrueAntiNuNC;
  bool _saveTrueNumuCC;
  bool _saveTrueAntiNumuCC;
  bool _saveTrueNueCC;
  bool _saveTrueAntiNueCC;

  Double_t _POTSincePreviousSavedSpill;
  Int_t _SpillsSincePreviousSavedSpill;

  bool _lastSpillSaved;

  std::vector<SubDetId::SubDetEnum> _saveTrueVertexInDet;

  AnaSpill* _spill;

  UInt_t _currentGeomID;

  Int_t NVtx;
  TClonesArray* Vtx;
  Int_t RunID;
  Int_t SubrunID;
  Int_t EventID;
  bool  Preselected;
};

#endif
