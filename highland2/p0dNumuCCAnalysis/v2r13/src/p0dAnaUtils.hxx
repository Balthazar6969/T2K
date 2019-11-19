#ifndef P0DANAUTILS_HXX
#define P0DANAUTILS_HXX

#include "TVector3.h"
#include "DataClasses.hxx"

class AnaEventB;
class AnaTrueParticleB;

namespace p0dAnaUtils
{
  /// Transverse plane calculations
  /// Returns the projection of thisvec onto the transverse nu plane
  TVector3 ProjectTransverse(TVector3& nudir, TVector3& thisvec);
  /// Returns pi-(the angle between muon and proton). Should be delta-fn in ideal back2back
  Float_t GetDPhi(const Float_t* nudir, const Float_t* mudir, const Float_t* pdir);
  /// Get the transverse momentum
  Float_t GetTransverseMom(const Float_t* nudir, const Float_t* thisdir, Float_t thismom);
  /// Check to see if outgoing particles consists of 1mu+1p only
  Int_t True1Mu1P(const AnaTrueVertex& trueVertex);

  // ===>
  // code inside this region is from xianguo. method for calculating the reconstructed neutrino direction
  void SetNeutrinoParentDecPointRec(TVector3 &vec);
  TVector3 CalcNuDir(TVector3 &nup0Global, TVector3 &nup1Local);
  TVector3 GetNuDirRec(const Float_t vertex_pos[]);
  //<============
}

#endif
