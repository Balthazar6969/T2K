#include "p0dAnaUtils.hxx"
#include "BaseDataClasses.hxx"
#include "ND280AnalysisUtils.hxx"
#include "TMath.h"

TVector3
p0dAnaUtils::ProjectTransverse(TVector3& nudir, TVector3& thisvec){
  return thisvec - thisvec.Dot(nudir)/nudir.Mag2() * nudir;
}

Float_t
p0dAnaUtils::GetDPhi(const Float_t* nudir, const Float_t* mudir, const Float_t* pdir) {
  TVector3 nudirv = TVector3(nudir);
  TVector3 mudirv = TVector3(mudir);
  TVector3 pdirv = TVector3(pdir);
  return TMath::Pi()-ProjectTransverse(nudirv, mudirv).Angle(ProjectTransverse(nudirv, pdirv));
}

Float_t
p0dAnaUtils::GetTransverseMom(const Float_t* nudir, const Float_t* thisdir, Float_t thismom){
  TVector3 nudirv = TVector3(nudir);
  TVector3 thisdirv = TVector3(thisdir);
  TVector3 projectedv = p0dAnaUtils::ProjectTransverse(nudirv, thisdirv);
  return (thismom * projectedv).Mag();
}

Int_t
p0dAnaUtils::True1Mu1P(const AnaTrueVertex& trueVertex){
  // Check outgoing primaries
  Int_t Nmuon = trueVertex.NPrimaryParticles[ParticleId::kMuon];
  Int_t Nproton = trueVertex.NPrimaryParticles[ParticleId::kProton];
  Int_t Nbaryon = trueVertex.NPrimaryParticles[ParticleId::kBaryons];
  Int_t Nmeson = trueVertex.NPrimaryParticles[ParticleId::kMesons];
  Int_t Nlepton = trueVertex.NPrimaryParticles[ParticleId::kLeptons];
  Int_t Nphoton = trueVertex.NPrimaryParticles[ParticleId::kPhoton];
  Int_t Nother = trueVertex.NPrimaryParticles[ParticleId::kOthers];

  // std::cout <<"Nmuon" << Nmuon << "Nlepton" << Nlepton << 
  //   "Np" << Nproton << "Nbaryon" << Nbaryon << "Nmeson" << 
  //   Nmeson << "Nphoton" << Nphoton << "Nother" << Nother << std::endl;
  if (Nmuon == 1 && Nlepton == 1 &&
      Nproton == 1 && Nbaryon == 1 &&
      Nmeson == 0 && Nphoton == 0 && Nother ==0) return true;

  return false;
}


// ===>
// code inside this region is from xianguo. method for calculating the reconstructed neutrino direction
void p0dAnaUtils::SetNeutrinoParentDecPointRec(TVector3 &vec){ vec.SetXYZ(-0.1388, -1.729, 34.55); }

TVector3 p0dAnaUtils::CalcNuDir(TVector3 &nup0Global, TVector3 &nup1Local)
{
  //in m
  const TVector3 nd280Global(-3.221999, -8.14599, 280.10);

  //in m 
  const TVector3 nup0Local = nup0Global - nd280Global;

  TVector3 nuDirCalc = TVector3(nup1Local - nup0Local);
  nuDirCalc *= 1./nuDirCalc.Mag();

  return nuDirCalc;
}

TVector3 p0dAnaUtils::GetNuDirRec(const Float_t vertex_pos[])
{
  //-------- nup0 
  //in m, mean of the distribution, 6B neutrino flux
  TVector3 nup0Global;
  p0dAnaUtils::SetNeutrinoParentDecPointRec(nup0Global);
  
  //------ nup1
  //in m
  TVector3 nup1Local(vertex_pos);
  nup1Local *= 0.001; //default mm

  return p0dAnaUtils::CalcNuDir(nup0Global, nup1Local);
}
//<============
