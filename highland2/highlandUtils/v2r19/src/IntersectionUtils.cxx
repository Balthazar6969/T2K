#include "IntersectionUtils.hxx"
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <TVector3.h>
#include "ConstituentsUtils.hxx"

//********************************************************************
bool intersectionUtils::ExtrapolatesToDetSurf(const Float_t* pos, const Float_t* dir, SubDetId::SubDetEnum det, Float_t* tol, Float_t& length){
  //********************************************************************
  length = 99999.;

  // detector surface
  AnaDetectorSurface surf(det);
  if (!surf.IsValid()) return false;

  TVector3 norm = surf.GetNorm();
  TVector3 r0   = surf.GetPosition(); 

  TVector3 r(pos[0], pos[1], pos[2]); 
  TVector3 u(dir[0], dir[1], dir[2]);

  if (u.Mag() == 0 ) return false;

  u *= 1/u.Mag();

  Float_t eps = 0.5;


  Float_t costheta = u.Dot(norm);

  if (fabs(costheta) == 0) return false;

  Float_t distance = ((r0-r).Dot(norm)/costheta);

  length = distance;

  TVector3 final_pos = r + distance*u;

  Float_t fpos[3] = {final_pos.X(), final_pos.Y(), final_pos.Z()};


  switch (det){
    case SubDetId::kTopSMRD:
      {  
        Float_t tweak[3] = {-1.*tol[0], -1.*eps, -1.*tol[1]};
        return anaUtils::InFiducialVolume(SubDetId::kTopSMRD, fpos, tweak, tweak);
      }
    case SubDetId::kBottomSMRD:
      {
        Float_t tweak[3] = {-1.*tol[0], -1.*eps, -1.*tol[1]};
        return anaUtils::InFiducialVolume(SubDetId::kBottomSMRD, fpos, tweak, tweak);
      }
    case SubDetId::kLeftSMRD:
      {
        Float_t tweak[3] = {-1.*eps, -1.*tol[0], -1.*tol[1]};
        return anaUtils::InFiducialVolume(SubDetId::kLeftSMRD, fpos, tweak, tweak);
      }
    case SubDetId::kRightSMRD:
      {
        Float_t tweak[3] = {-1.*eps, -1.*tol[0], -1.*tol[1]};
        return anaUtils::InFiducialVolume(SubDetId::kRightSMRD, fpos, tweak, tweak);
      }
    case SubDetId::kTopTECAL:
      {
        Float_t tweak[3] = {-1.*tol[0], -1.*eps, -1.*tol[1]};
        return anaUtils::InFiducialVolume(SubDetId::kTopTECAL, fpos, tweak, tweak);
      }
    case SubDetId::kBottomTECAL:
      {
        Float_t tweak[3] = {-1.*tol[0], -1.*eps, -1.*tol[1]};
        return anaUtils::InFiducialVolume(SubDetId::kBottomTECAL, fpos, tweak, tweak);
      }
    case SubDetId::kLeftTECAL:
      {
        Float_t tweak[3] = {-1.*eps, -1.*tol[0], -1.*tol[1]};
        return anaUtils::InFiducialVolume(SubDetId::kLeftTECAL, fpos, tweak, tweak);
      }
    case SubDetId::kRightTECAL:
      {
        Float_t tweak[3] = {-1.*eps, -1.*tol[0], -1.*tol[1]};
        return anaUtils::InFiducialVolume(SubDetId::kRightTECAL, fpos, tweak, tweak);
      }


    default:
      return false;
  }

  return false;

}


//********************************************************************
bool intersectionUtils::ExtrapolatesToDetSurf(const AnaParticleB& track, SubDetId::SubDetEnum det, Float_t* tol, Float_t& length){
  //********************************************************************
  length = 99999.;

  Float_t spos[3]; 
  Float_t epos[3];
  anaUtils::CopyArray(track.PositionStart, spos, 3);
  anaUtils::CopyArray(track.PositionEnd,   epos, 3);


  Float_t* dir = intersectionUtils::GetSLineDir(spos,  epos);

  return ExtrapolatesToDetSurf(spos, dir, det, tol, length);
}

//********************************************************************
Float_t* intersectionUtils::GetSLineDir(Float_t* start, Float_t* end){
  //********************************************************************
  static Float_t uf[3] = {0, 0, 0};
  TVector3 u(end[0]-start[0], end[1]-start[1], end[2]-start[2]);
  if (u.Mag()!=0){
    u *= 1/(u.Mag());
    uf[0] = u.X();
    uf[1] = u.Y();
    uf[2] = u.Z();
  }
  return uf;

}

//********************************************************************
Float_t intersectionUtils::GetDistanceToSurface(Float_t* point_pos, Float_t* surf_pos, Float_t* norm){
  //******************************************************************** 
  TVector3 n(norm[0], norm[1], norm[2]);
  TVector3 r0(point_pos[0], point_pos[1], point_pos[2]);
  TVector3 r(surf_pos[0], surf_pos[1], surf_pos[2]);

  return (r-r0).Dot(n);

}

//********************************************************************
intersectionUtils::AnaRay::AnaRay(const Float_t* pos, const Float_t* dir){
  //********************************************************************
  _pos = anaUtils::ArrayToTVector3(pos);
  _dir = anaUtils::ArrayToTVector3(dir);

  _dir = _dir.Unit();

  //inverse direction
  Float_t x = _dir.X()!=0 ? 1/_dir.X() : DBL_MAX;
  Float_t y = _dir.Y()!=0 ? 1/_dir.Y() : DBL_MAX;
  Float_t z = _dir.Z()!=0 ? 1/_dir.Z() : DBL_MAX;

  _invDir.SetXYZ(x,y,z);
}

//******************************************************************** 
intersectionUtils::AnaDetector3DBox_AAB::AnaDetector3DBox_AAB(const Float_t* pos, Float_t Dx, Float_t Dy, Float_t Dz ){
  //******************************************************************** 
  _pos  = anaUtils::ArrayToTVector3(pos);
  _Dx   = fabs(Dx);
  _Dy   = fabs(Dy);
  _Dz   = fabs(Dz);

  // get the edges
  _min = TVector3(_pos - TVector3(_Dx, _Dy, _Dz));
  _max = TVector3(_pos + TVector3(_Dx, _Dy, _Dz));
}

//******************************************************************** 
intersectionUtils::AnaDetector3DBox_AAB::AnaDetector3DBox_AAB(const Float_t* min, const Float_t* max ){
  //********************************************************************  
  //  get the edges
  _min = anaUtils::ArrayToTVector3(min);
  _max = anaUtils::ArrayToTVector3(max);

  _Dx   = fabs( 0.5*(max[0]-min[0]) );
  _Dy   = fabs( 0.5*(max[1]-min[1]) );
  _Dz   = fabs( 0.5*(max[1]-min[2]) );


  _pos  = TVector3(0.5*(max[0]+min[0]), 0.5*(max[1]+min[1]), 0.5*(max[2]+min[2]));

}


//******************************************************************** 
bool intersectionUtils::AnaDetector3DBox_AAB::Intersect(const intersectionUtils::AnaRay& ray, Float_t& t_min, Float_t& t_max) const {
  //******************************************************************** 
  t_min = intersectionUtils::kUnassigned;
  t_max = intersectionUtils::kUnassigned;

  Float_t tmin, tmax, tymin, tymax, tzmin, tzmax;

  // X
  tmin = (_min.X() - ray.GetPos().X()) * ray.GetInvDir().X();
  tmax = (_max.X() - ray.GetPos().X()) * ray.GetInvDir().X();
  if (tmin > tmax) std::swap(tmin, tmax);

  // Y
  tymin = (_min.Y() - ray.GetPos().Y()) * ray.GetInvDir().Y();
  tymax = (_max.Y() - ray.GetPos().Y()) * ray.GetInvDir().Y();

  if (tymin > tymax) std::swap(tymin, tymax);
  if ((tmin > tymax) || (tymin > tmax))
    return false;

  if (tymin > tmin)
    tmin = tymin;
  if (tymax < tmax)
    tmax = tymax;

  // Z
  tzmin = (_min.Z() - ray.GetPos().Z()) * ray.GetInvDir().Z();
  tzmax = (_max.Z() - ray.GetPos().Z()) * ray.GetInvDir().Z();
  if (tzmin > tzmax) std::swap(tzmin, tzmax);

  if ((tmin > tzmax) || (tzmin > tmax))
    return false;

  if (tzmin > tmin)
    tmin = tzmin;
  if (tzmax < tmax)
    tmax = tzmax;

  // Set the output values
  t_min =  tmin;

  t_max = tmax;

  return true; 
}


//******************************************************************** 
bool intersectionUtils::IntersectsBox(const Float_t* pos, const Float_t* dir,  const intersectionUtils::AnaDetector3DBox_AAB& box, Float_t& tmin, Float_t& tmax){
//******************************************************************** 
  intersectionUtils::AnaRay ray(pos, dir);
  return box.Intersect(ray, tmin, tmax);
  
}

