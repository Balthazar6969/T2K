#ifndef P0DAnalysisUtils_h
#define P0DAnalysisUtils_h

#include "ND280AnalysisUtils.hxx"
#include "CutUtils.hxx"
#include "SubDetId.hxx"
#include "P0DDataClasses.hxx"
#include "CoreUtils.hxx"



/// This namespace contains useful functions for analyses. 
/// This is in addition to those defined in psycheUtils
namespace p0dUtils{

  /// Get the P0D particle from a AnaParticleB: either a segment in a global track or the cast of the P0D-only track it self
  AnaP0DParticle* GetP0DParticle(AnaParticleB* part);
     
  // Get all P0DParticles in the event
  Int_t GetAllP0DParticles(const AnaEventB& event, AnaP0DParticle* selParts[]);

  // Get all P0DVertices in the event
  Int_t GetAllP0DVertices(const AnaEventB& event, AnaP0DVertex* selVerts[]);

  /// Get the number of P0D michel electrons
  Int_t GetMichelElectrons(const AnaSpillB& spill, AnaDelayedClustersB**& delayedClusters, Int_t firstBunch=0);

  /// Get the number of P0D michel electrons
  Int_t GetNMichelElectrons(const AnaSpillB& spill, Int_t firstBunch=0);

  /// Get the number of P0D michel electrons from anaP0DVertex
  Int_t GetMichelElectrons(AnaP0DVertex* anaP0DVertex, AnaDelayedClustersB**& delayedClusters);

  /// Resize the array

  //********************************************************************************
  // Geometry Utils (Maybe it should be moved to a different namespace)
  //********************************************************************************

  int GetField(unsigned int geomid, int lsb, int msb);

  int GetSP0DuleFromGeomId(unsigned int geomid);

  int GetP0DuleFromGeomId(unsigned int geomid);

  int GetLayerFromGeomId(unsigned int geomid);

  int GetBarFromGeomId(unsigned int geomid);

}

namespace boxP0DUtils{
  void FillVerticesFinal(AnaEventB& event);
  void FillP0DParticles(AnaEventB& event);
}

#endif


