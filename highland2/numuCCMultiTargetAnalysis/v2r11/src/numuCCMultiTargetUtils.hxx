#ifndef numuCCMultiTargetUtils_h
#define numuCCMultiTargetUtils_h

#include "SelectionBase.hxx"
#include "DataClasses.hxx"


namespace numuccmultitarget_utils{

  void AddCategories(const std::string& prefix="");

  int GetFgdModuleTypeCC(bool IsMC, const AnaTrueVertex* vtx, SubDetId::SubDetEnum det, bool IsAntinu = false);

  int GetTargetCCQE(const AnaTrueVertex* trueVertex, SubDetId::SubDetEnum = SubDetId::kFGD1);
  int GetTargetCC(const AnaTrueVertex* trueVertex, SubDetId::SubDetEnum = SubDetId::kFGD1);

  int GetFgdLayer(bool IsMC, const Float_t* pos);

  TVector3 GetFittedPos(TVector3 A, TVector3 B, double z);
  float GetDeltaChi2(TVector3 hit, TVector3 fit);

  float CalculateLineLineDistance(
    TVector3 track_start_pos,
    TVector3 track_end_pos,
    TVector3 hit_pos
  );
}

#endif
