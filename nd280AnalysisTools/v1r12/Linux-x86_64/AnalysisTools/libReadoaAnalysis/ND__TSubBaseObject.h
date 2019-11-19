//////////////////////////////////////////////////////////
//   This class has been generated by TFile::MakeProject
//     (Tue Mar 20 17:07:52 2018 by ROOT version 5.34/34)
//      from the StreamerInfo in file /mnt/data/backup/production006/B/mcp/neut/2010-11-air/magnet/run4/anal/oa_nt_beam_90400000-0000_jrvqqzb53okw_anal_000_prod6amagnet201011airc-bsdv01_2.root
//////////////////////////////////////////////////////////


#ifndef ND__TSubBaseObject_h
#define ND__TSubBaseObject_h
namespace ND {
class TSubBaseObject;
} // end of namespace.

#include "TObject.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "ND__TTrueParticle.h"

namespace ND {
class TSubBaseObject : public TObject {

public:
// Nested classes declaration.

public:
// Data Members.
   UInt_t      UniqueID;    ///< Unique identifier for sub-detector recon objects
   unsigned long Status;      ///< The status for the fit.
   int           Detector;    ///< Integer specifying which subdetector(s) the object belongs to. See TGlobalReconModule::GetDetectorNumber for the options. 
   int           NHits;       ///< The number of hits.
   int           NNodes;      ///< The number of nodes.
   int           NDOF;        ///< The number of degrees of freedom.
   double        Chi2;        ///< The chi2 of the fit.
   double        EDeposit;    ///< Deposited energy.
   int           NConstituents;    ///< The number of constituents.
   double        Length;           ///< The length of the track or shower (RMS)
   TLorentzVector FrontPosition;    ///< The position of the object.
   TLorentzVector BackPosition;     ///< The position of the object.
   TLorentzVector FrontPositionVar;    ///< The variance of the position variance;
   TLorentzVector BackPositionVar;     ///< The variance of the position variance;
   TVector3       FrontDirection;      ///< The direction of the object.
   TVector3       BackDirection;       ///< The direction of the object.
   TVector3       FrontDirectionVar;    ///< The variance of the direction of the object.
   TVector3       BackDirectionVar;     ///< The variance of thedirection of the object.
   double         FrontMomentum;        ///< the momentum of the object
   double         BackMomentum;         ///< the momentum of the object
   double         FrontMomentumError;    ///< the error on the momentum of the object
   double         BackMomentumError;     ///< the error on the momentum of the object
   ND::TTrueParticle TrueParticle;          ///< The true particle

   TSubBaseObject();
   TSubBaseObject(const TSubBaseObject & );
   virtual ~TSubBaseObject();

   ClassDef(TSubBaseObject,2); // Generated by MakeProject.
};
} // namespace
#endif
