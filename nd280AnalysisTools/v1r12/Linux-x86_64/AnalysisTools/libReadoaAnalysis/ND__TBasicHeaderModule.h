//////////////////////////////////////////////////////////
//   This class has been generated by TFile::MakeProject
//     (Tue Mar 20 17:07:53 2018 by ROOT version 5.34/34)
//      from the StreamerInfo in file /mnt/data/backup/production006/B/mcp/neut/2010-11-air/magnet/run4/anal/oa_nt_beam_90400000-0000_jrvqqzb53okw_anal_000_prod6amagnet201011airc-bsdv01_2.root
//////////////////////////////////////////////////////////


#ifndef ND__TBasicHeaderModule_h
#define ND__TBasicHeaderModule_h
namespace ND {
class TBasicHeaderModule;
} // end of namespace.

#include "ND__TAnalysisHeaderModuleBase.h"

namespace ND {
class TBasicHeaderModule : public ND::TAnalysisHeaderModuleBase {

public:
// Nested classes declaration.

public:
// Data Members.
   Char_t      fSoftwareVersion[50];    //
   bool        fSoftware;               //

   TBasicHeaderModule();
   TBasicHeaderModule(const TBasicHeaderModule & );
   virtual ~TBasicHeaderModule();

   ClassDef(TBasicHeaderModule,2); // Generated by MakeProject.
};
} // namespace
#endif
