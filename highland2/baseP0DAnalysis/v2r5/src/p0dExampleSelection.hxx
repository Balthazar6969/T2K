#ifndef p0dExampleSelection_h
#define p0dExampleSelection_h

#include "SelectionBase.hxx"
#include "Parameters.hxx"
#include "P0DDataClasses.hxx"
#include "ToyBoxND280.hxx"

//---- Define an specific box for this selection -------
class ToyBoxP0D: public ToyBoxND280{
  
public:
  ToyBoxP0D(){
    Reset();
  }

  virtual void Reset(){

    if (!_ResetCheckDone){
      if( typeid(*this) !=  typeid(ToyBoxP0D)){
        std::cerr << "ERROR in ToyBoxP0D::Reset(). Either this mandatory method is not implemented " 
                  << "by the derived class '" << typeid(*this).name() << "' "
                  << "or ToyBoxP0D::Reset() is called from the Reset method of the derived class. "
                  << "Please correct any of these bugs. " << std::endl;
        
        exit(1);
      }
      _ResetCheckDone=true;
    }
  }

  /// Reset this base class
  virtual void ResetBase(){
    nShowers = 0;
    Shower1=Shower2=NULL;
    nTracks = 0;
    ToyBoxND280::ResetBase();
  }  


  
  virtual ~ToyBoxP0D(){}
  
  Short_t nShowers;
  AnaP0DParticle* Shower1;
  AnaP0DParticle* Shower2;
  
  Short_t nTracks;
};





class p0dExampleSelection: public SelectionBase{

public:
  p0dExampleSelection(bool forcebreak=true);
  virtual ~p0dExampleSelection(){}

  //************************Mandatory Functions ****************

  void DefineSteps();
  void DefineDetectorFV();

  ToyBoxB* MakeToyBox() {return new ToyBoxP0D();}

  void InitializeEvent(AnaEventC& event);

  //************************************************************

};

class CountTracksAndShowersAction : public StepBase {  
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new CountTracksAndShowersAction();}
};


class TwoShowersCut : public StepBase {
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new TwoShowersCut();}
};

class NoTracksCut : public StepBase {
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new NoTracksCut();}
};

class NoMichelElectronsCut : public StepBase {
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new NoMichelElectronsCut();}
};


class FindTrueVertexAction : public StepBase {  
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FindTrueVertexAction();}
};


#endif
