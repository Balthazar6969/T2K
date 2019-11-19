//////////////////////////////////////////////////////////
//   This class has been generated by TFile::MakeProject
//     (Tue Mar 20 17:07:52 2018 by ROOT version 5.34/34)
//      from the StreamerInfo in file /mnt/data/backup/production006/B/mcp/neut/2010-11-air/magnet/run4/anal/oa_nt_beam_90400000-0000_jrvqqzb53okw_anal_000_prod6amagnet201011airc-bsdv01_2.root
//////////////////////////////////////////////////////////


#ifndef ND__TSubBaseShowerObject_h
#define ND__TSubBaseShowerObject_h
namespace ND {
class TSubBaseShowerObject;
} // end of namespace.

#include "ND__TSubBaseObject.h"
#include "TVector3.h"

namespace ND {
class TSubBaseShowerObject : public ND::TSubBaseObject {

public:
// Nested classes declaration.

public:
// Data Members.
   TVector3    Cone;        ///< The opening angles of the cone (only for showers)

   TSubBaseShowerObject();
   TSubBaseShowerObject(const TSubBaseShowerObject & );
   virtual ~TSubBaseShowerObject();

   ClassDef(TSubBaseShowerObject,2); // Generated by MakeProject.
};
} // namespace
#endif
