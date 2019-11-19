#ifndef HighlandTreeConverterUtils_h
#define HighlandTreeConverterUtils_h

#include "ND280AnalysisUtils.hxx"
#include "SubDetId.hxx"
#include "DataClasses.hxx"
#include <TClonesArray.h>


/// This namespace contains useful functions for analyses. 
/// This is in addition to those defined in psycheUtils
namespace anaUtils{

  bool FillTrueVertexRooInfo(AnaTrueVertexB* vertexB, Int_t RunID, bool Neut, bool Genie, 
                             Int_t NNVtx, TClonesArray* NVtx, Int_t NGVtx, TClonesArray* GVtx, 
                             bool& foundCohOnH, bool& foundPauliBlocked );
}

#endif


