//////////////////////////////////////////////////////////
//   This class has been generated by TFile::MakeProject
//     (Fri Nov  4 11:44:16 2016 by ROOT version 5.34/34)
//      from the StreamerInfo in file /hep/T2K/DataDir/valid/oa_nt_beam_90410000-0000_h252u2jujozw_anal_000_prod005magnet201011waterc-bsdv01.root
//////////////////////////////////////////////////////////


#ifndef ND__TExample2010aAnalysis1Module_h
#define ND__TExample2010aAnalysis1Module_h
namespace ND {
class TExample2010aAnalysis1Module;
} // end of namespace.

#include "ND__TAnalysisReconModuleBase.h"

namespace ND {
class TExample2010aAnalysis1Module : public ND::TAnalysisReconModuleBase {

public:
// Nested classes declaration.

public:
// Data Members.
   Double_t    fTimeOffset;    //

   TExample2010aAnalysis1Module();
   TExample2010aAnalysis1Module(const TExample2010aAnalysis1Module & );
   virtual ~TExample2010aAnalysis1Module();

   ClassDef(TExample2010aAnalysis1Module,2); // Generated by MakeProject.
};
} // namespace
#endif
