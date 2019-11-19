#ifndef CreateFlatTree_h
#define CreateFlatTree_h

#include "SimpleLoopBase.hxx"
#include "DataClassesIO.hxx"
#include "InputManager.hxx"
#include "RooTrackerVtxManager.hxx"
#include <set>

class CreateFlatTree: public SimpleLoopBase {
 public:

  CreateFlatTree(int argc, char *argv[]);
  virtual ~CreateFlatTree(){}

protected:

  //---- These are mandatory functions
  virtual bool Initialize     ();
  virtual bool InitializeSpill(){return _rooVtxManager.InitializeEntry();}

  virtual void DefineOutputTree();

  virtual void FinalizeSpill(){}
  virtual void Finalize     ();

  virtual bool Process();
  //--------------------

  virtual void FillSpillVariables();

  virtual bool CheckTruthFillFlatTree(const AnaSpill& spill);
  virtual bool CheckReconFillFlatTree(const AnaBunch& bunch);
  virtual bool CheckReconFillFlatTreeOutOfBunch(const AnaBunch& bunch);

  virtual bool CheckTrueVertexReaction(const AnaTrueVertex& vtx);
  virtual bool CheckTrueVertexDetector(unsigned long det);
  virtual void CreateListOfTrueParticlesToSave();
  virtual AnaTrueParticleB* GetTrueParticleByID(int ID);

  virtual void FillFlatTree();
  virtual void FillRooTrackerVtxTree();
  virtual void FilterRooTrackerVtxTree();
  
  virtual void DefineSpill               (AnaSpillIO*                spill      ){spill      ->Define(*this,"s"                                     );}
  virtual void DefineTrueVertex          (AnaTrueVertexIO*           trueVertex ){trueVertex ->Define(*this,"sNTrueVertices",      "sTrueVertex"    );}
  virtual void DefineTrueParticle        (AnaTrueParticleIO*         trueParticle){trueParticle  ->Define(*this,"sNTrueTracks",        "sTrueTrack"     );}
  virtual void DefineFgdTimeBin          (AnaFgdTimeBinIO*           fgdTimeBin ){fgdTimeBin ->Define(*this,"sNFgdTimeBins",       "sFgdTimeBin"    );}
  virtual void DefineBunch               (AnaBunchIO*                bunch      ){bunch      ->Define(*this,""                                      );}
  virtual void DefineTrack               (AnaTrackIO*                track      ){track      ->Define(*this,"NTracks",              "tr"            );}
  virtual void DefineVertex              (AnaVertexIO*               vertex     ){vertex     ->Define(*this,"NVertices",            "v"             );}
  virtual void DefineTPCParticle         (AnaTPCParticleIO*          tpc        ){tpc        ->Define(*this,"NTracks",              "trTpc"         );}
  virtual void DefineFGDParticle         (AnaFGDParticleIO*          fgd        ){fgd        ->Define(*this,"NTracks",              "trFgd"         );}
  virtual void DefineECALParticle        (AnaECALParticleIO*         ecal       ){ecal       ->Define(*this,"NTracks",              "trECAL"        );}
  virtual void DefineSMRDParticle        (AnaSMRDParticleIO*         smrd       ){smrd       ->Define(*this,"NTracks",              "trSMRD"        );}
  virtual void DefineP0DParticle         (AnaP0DParticleIO*          p0d        ){p0d        ->Define(*this,"NTracks",              "trP0D"         );}
  virtual void DefineTrackerTrack        (AnaTrackerTrackIO*         tracker    ){tracker    ->Define(*this,"NTracks",              "trTRACKER"     );}
  virtual void DefineTECALReconObject    (AnaTECALReconObjectIO*      tecal      ){tecal      ->Define(*this,"tecalReconCounter",    "tecalRecon"    );}
  virtual void DefineTECALUnmatchedObject(AnaTECALUnmatchedObjectIO*  tecal      ){tecal      ->Define(*this,"tecalUnmatchedCounter","tecalUnmatched");}
  virtual void DefineP0DReconVertex      (AnaP0DReconVertexIO*        p0dVertex  ){p0dVertex  ->Define(*this,"p0dVertexCounter",     "p0dVertex"     );}
  virtual void DefineP0DReconParticle    (AnaP0DReconParticleIO*      p0dParticle){p0dParticle->Define(*this,"p0dParticleCounter",   "p0dParticle"   );} 

  virtual void WriteTrueParticle(AnaTrueParticleIO *trueParticle, int trueVertexIndex                  ){trueParticle->Write(*this, trueVertexIndex,"sTrueTrack");}
  virtual void WriteVertex   (AnaVertexIO    *vertex, const std::vector<int> trueVertexIndices){vertex   ->Write(*this, trueVertexIndices,"v"       );}

  virtual void WriteTrueVertex          (AnaTrueVertexIO           *trueVertex      ){trueVertex  ->Write(*this, "sTrueVertex"       );}
  virtual void WriteFgdTimeBin          (AnaFgdTimeBinIO           *fgdTimeBin      ){fgdTimeBin  ->Write(*this, "sFgdTimeBin"       );}
  virtual void WriteBunch               (AnaBunchIO                *bunch           ){bunch       ->Write(*this, ""                  );}
  virtual void WriteTrack               (AnaTrackIO                *track           ){track       ->Write(*this, "tr"                );}
  virtual void WriteTPCParticle         (AnaTPCParticleIO          *tpc             ){tpc         ->Write(*this, "trTpc"             );}
  virtual void WriteFGDParticle         (AnaFGDParticleIO          *fgd             ){fgd         ->Write(*this, "trFgd"             );}
  virtual void WriteECALParticle        (AnaECALParticleIO         *ecal            ){ecal        ->Write(*this, "trECAL"            );}
  virtual void WriteSMRDParticle        (AnaSMRDParticleIO         *smrd            ){smrd        ->Write(*this, "trSMRD"            );}
  virtual void WriteP0DParticle         (AnaP0DParticleIO          *p0d             ){p0d         ->Write(*this, "trP0D"             );}
  virtual void WriteTrackerTrack        (AnaTrackerTrackIO         *tracker, int j  ){tracker     ->Write(*this, "trTRACKER", -1, j  );}
  virtual void WriteTECALReconObject    (AnaTECALReconObjectIO      *tecal           ){tecal       ->Write(*this, "tecalRecon"        );}
  virtual void WriteTECALUnmatchedObject(AnaTECALUnmatchedObjectIO  *tecal           ){tecal       ->Write(*this, "tecalUnmatched"    );}
  virtual void WriteP0DReconVertex      (AnaP0DReconVertexIO        *p0dVertex       ){p0dVertex   ->Write(*this, "p0dVertex"         );}
  virtual void WriteP0DReconParticle    (AnaP0DReconParticleIO      *p0dParticle     ){p0dParticle ->Write(*this, "p0dParticle"       );}

  virtual int FindTrueVertexIndex(AnaTrueVertexB* vertex);

  bool CheckWriteSegment(SubDetId::SubDetEnum det, const AnaParticleB& seg);
  void ResetCheckWriteSegment();


  enum flatTreeVars{
    sPOTSincePreviousSpill = TreeVars::TreeVarsLast+1
  };

  enum flatTreeIndex{
    flattree = OutputManager::enumSpecialTreesLast+1
  };


protected:

  RooTrackerVtxManager _rooVtxManager;
  
  bool _saveTruthWithRecon;
  bool _saveRoo;
  bool _filterRoo;
  
  bool _saveTPCInfo;
  bool _saveFGDInfo;
  bool _saveECALInfo;
  bool _saveP0DInfo;
  bool _saveSMRDInfo;
  bool _saveTrackerInfo;

  // read the parameters file to decide if information from the local reconstruction should
  // be saved to the flat tree.
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

  double _POTSincePreviousSpill;

  bool _spill_filled;

  std::set<int> _trueParticleIDs;

  std::vector<SubDetId::SubDetEnum> _saveTrueVertexInDet;

  std::vector<AnaTrueVertex*> _savedTrueVertices;

  Int_t _max_nodes[10];

  Int_t NVtx;
  TClonesArray* Vtx;
  Int_t RunID;
  Int_t SubrunID;
  Int_t EventID;
  bool  Preselected;


};

#endif
