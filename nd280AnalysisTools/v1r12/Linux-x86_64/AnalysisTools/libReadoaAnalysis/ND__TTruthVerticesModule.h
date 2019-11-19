//////////////////////////////////////////////////////////
//   This class has been generated by TFile::MakeProject
//     (Tue Mar 20 17:07:53 2018 by ROOT version 5.34/34)
//      from the StreamerInfo in file /mnt/data/backup/production006/B/mcp/neut/2010-11-air/magnet/run4/anal/oa_nt_beam_90400000-0000_jrvqqzb53okw_anal_000_prod6amagnet201011airc-bsdv01_2.root
//////////////////////////////////////////////////////////


#ifndef ND__TTruthVerticesModule_h
#define ND__TTruthVerticesModule_h
namespace ND {
class TTruthVerticesModule;
} // end of namespace.

#include "ND__TAnalysisTruthModuleBase.h"
#include "TClonesArray.h"
#include "TObject.h"
#include "TLorentzVector.h"
#include "Riostream.h"
#include <string>
#include <vector>

namespace ND {
class TTruthVerticesModule : public ND::TAnalysisTruthModuleBase {

public:
// Nested classes forward declaration.
class TTruthVertex;

public:
// Nested classes declaration.
class TTruthVertex : public TObject {

public:
// Nested classes declaration.

public:
// Data Members.
   Int_t       ID;          ///<  A number which uniquely identifies this vertex within the event.
   TLorentzVector Position;    ///< Position and time of the vertex
   string         Generator;    ///< The generator that created the vertex. eg: "genie:mean@free-spill"
   string         ReactionCode;    ///< The Reaction code according to the generator
   Int_t          Subdetector;     ///< Subdetector which the vertex occurs in.
   Double_t       Fiducial;        ///< The distance inside the local fiducial volume [mm].
   Int_t          NPrimaryTraj;    ///< The number of primary trajectories
   vector<Int_t>  PrimaryTrajIDs;    ///< ID numbers which uniquely identify the trajectories of the  primary particles of the vertex within the event.
   Int_t          NeutrinoPDG;       ///< The PDG number of the incoming neutrino. Set to 0 in the absence of a neutrino.
   TLorentzVector NeutrinoMomentum;    ///< The four-momentum of the incoming neutrino. Set to (-999999.9, -999999.9, -999999.9, -999999.9) in the absence of a neutrino.
   Int_t          TargetPDG;           ///< The (extended for nuclei) PDG number of the target. Set to 0 in the absence of a target.
   TLorentzVector TargetMomentum;      ///< The four-momentum of the target.

   TTruthVertex();
   TTruthVertex(const TTruthVertex & );
   virtual ~TTruthVertex();

   ClassDef(TTruthVertex,2); // Generated by MakeProject.
};

public:
// Data Members.
   UInt_t      fMaxNVertices;    //
   Int_t       fNVtx;            ///< The total number of vertices recorded
   Int_t       fNVtxFGD1;        ///< The number of vertices recorded in FGD 1
   Int_t       fNVtxFGD2;        ///< The number of vertices recorded in FGD 2
   Int_t       fNVtxP0D;         ///< The number of vertices recorded in the P0D
   Int_t       fNVtxDsECal;      ///< The number of vertices recorded in the Downstream ECal
   Int_t       fNVtxBrlECal;     ///< The number of vertices recorded in the Barrel ECal
   Int_t       fNVtxP0DECal;     ///< The number of vertices recorded in the P0D ECal
   Int_t       fNVtxTPC1;        ///< The number of vertices recorded in TPC 1
   Int_t       fNVtxTPC2;        ///< The number of vertices recorded in TPC 2
   Int_t       fNVtxTPC3;        ///< The number of vertices recorded in TPC 3
   Int_t       fNVtxSMRD;        ///< The number of vertices recorded in the SMRD
   Int_t       fNVtxRestOfOffAxis;    ///< The number of vertices recorded in the rest of the off-axis detector
   Int_t       fNVtxINGRID;           ///< The number of vertices recorded in INGRID
   Int_t       fNVtxOther;            ///< The number of vertices recorded anywhere which does not fall  into the other available categories
   TClonesArray* fVertices;             ///< The TClonesArray storing the TTruthVertex objects holding the information relating to each vertex.

   TTruthVerticesModule();
   TTruthVerticesModule(const TTruthVerticesModule & );
   virtual ~TTruthVerticesModule();

   ClassDef(TTruthVerticesModule,2); // Generated by MakeProject.
};
} // namespace
#endif
