#ifndef IntersectionUtils_h
#define IntersectionUtils_h

#include "DataClasses.hxx"
#include "SubDetId.hxx"
#include "DetectorDefinition.hxx"

/// Some utils that allows to deal with inersections of rays with 3D
// axis-alinged boxes (this is to use the functionality w/o the need of
// highlandRecPack(i.e. RECPACK))

namespace intersectionUtils{
  

  const Int_t kUnassigned = 0xDEAD;

  class AnaDetectorSurface{
  public:
    AnaDetectorSurface(){
      _norm     = TVector3(0, 0, 0);
      _r0       = TVector3(0, 0, 0);
      _isValid  = true;
    }
    AnaDetectorSurface(SubDetId::SubDetEnum det){
      _norm     = TVector3(0, 0, 0);
      _r0       = TVector3(0, 0, 0);
      _isValid  = true;
      switch (det){
        case SubDetId::kTopSMRD:
          _norm.SetY(1);
          _r0 = TVector3(0, DetDef::smrdTLmin[1], 0.); 
          break;
        case SubDetId::kBottomSMRD:
          _norm.SetY(-1);
          _r0 = TVector3(0, DetDef::smrdBLmax[1], 0.); 
          break;
        case SubDetId::kLeftSMRD:
          _norm.SetX(1);
          _r0 = TVector3(DetDef::smrd6Lmin[0], 0., 0.); 
          break;
        case SubDetId::kRightSMRD:
          _norm.SetX(-1);
          _r0 = TVector3(DetDef::smrd6Rmax[0], 0., 0.); 
          break;
        case SubDetId::kTopTECAL:
          _norm.SetY(1);
          _r0 = TVector3(0, DetDef::tecalTLmax[1], 0.); 
          break;
        case SubDetId::kBottomTECAL:
          _norm.SetY(-1);
          _r0 = TVector3(0, DetDef::tecalBLmin[1], 0.); 
          break;
        case SubDetId::kLeftTECAL:
          _norm.SetX(1);
          _r0 = TVector3(DetDef::tecalLmax[0], 0., 0.); 
          break;
        case SubDetId::kRightTECAL:
          _norm.SetX(-1);
          _r0 = TVector3(DetDef::tecalRmin[0], 0., 0.); 
          break; 
        default:
          _isValid = false;
      }
    }
    
    virtual ~AnaDetectorSurface(){};
    
    // Getters
    bool IsValid() const {
      return _isValid;
    } 
    
    const TVector3& GetPosition() const {
      return _r0;
    } 
    
    const TVector3& GetNorm() const {
      return _norm;
    } 
    
  private:
    bool      _isValid;
    TVector3  _r0;
    TVector3  _norm;
  };
  
  class AnaRay{
    public:
      /// A constructor given a position and direction
      AnaRay(const Float_t* pos, const Float_t* dir);

      /// A default destructor
      virtual ~AnaRay() {};

      const TVector3& GetPos()    const {return _pos;}
      const TVector3& GetDir()    const {return _dir;}
      const TVector3& GetInvDir() const {return _invDir;}

      TVector3 Propagate(Float_t distance) const {return _pos + distance*_dir;}

    
    private:
      TVector3 _pos;
      TVector3 _dir;
      TVector3 _invDir;
  };
 
  /// This is a simple box in 3D, for the moment axis aligned one (as is ND280 complex) 
  class AnaDetector3DBox_AAB{
    public:
      /// A constructor given a position of the center and half-sizes for each side
      AnaDetector3DBox_AAB(const Float_t* pos, Float_t Dx, Float_t Dy, Float_t Dz);
     
      /// a constructor given max and min edges
      AnaDetector3DBox_AAB(const Float_t* min, const Float_t* max);
      
      /// Default destructor
      virtual ~AnaDetector3DBox_AAB() {};

      const TVector3& GetPos() const {return _pos;}
     
      Float_t GetDx() const {return _Dx;}
      Float_t GetDy() const {return _Dy;}
      Float_t GetDz() const {return _Dz;}
      
      const TVector3& GetMin() const {return _min;}
      const TVector3& GetMax() const {return _max;}

      /// Wether a ray intersect this box
      bool Intersect(const intersectionUtils::AnaRay& ray, Float_t& tmin, Float_t& tmax) const;

    private:

      /// Position of the center
      TVector3 _pos;

      /// Half sizes
      Float_t _Dx;
      Float_t _Dy;
      Float_t _Dz;

      /// Edge coordinates of the box
      TVector3 _min;
      TVector3 _max;
  };

 
  const  AnaDetector3DBox_AAB Fgd1ActiveBox(DetDef::fgd1min, DetDef::fgd1max);
  const  AnaDetector3DBox_AAB Fgd2ActiveBox(DetDef::fgd2min, DetDef::fgd2max);
  
  // Check whether the particular ray intersect a box
  bool IntersectsBox(const Float_t* pos, const Float_t* dir,  const AnaDetector3DBox_AAB& box, Float_t& tmin, Float_t& tmax);

  bool ExtrapolatesToDetSurf(const AnaParticleB& track,               SubDetId::SubDetEnum det, Float_t* tol, Float_t& length);
  bool ExtrapolatesToDetSurf(const Float_t* pos, const Float_t* dir,  SubDetId::SubDetEnum det, Float_t* tol, Float_t& length);

  Float_t* GetSLineDir(Float_t* start, Float_t* end);
  Float_t GetDistanceToSurface(Float_t* point_pos, Float_t* surf_pos, Float_t* norm);
}

#endif
