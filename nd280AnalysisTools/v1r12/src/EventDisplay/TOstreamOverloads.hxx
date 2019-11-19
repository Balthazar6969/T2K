//
// convenient overloads for printing things out
//
#ifndef TEDOSTREAMOVERLOADS
#define TEDOSTREAMOVERLOADS

#include "TLorentzVector.h"
ostream& operator<< ( ostream& theStream, TLorentzVector& V );

#include "TVector3.h"

ostream& operator<< ( ostream& theStream, TVector3& V );

ostream& operator<< ( ostream& theStream,const  TVector3& V );

#include "TGLUtil.h"
 
ostream& operator<< ( ostream& theStream, TGLVertex3& V );

ostream& operator<< ( ostream& theStream, TGLVector3& V );

ostream& operator<< ( ostream& theStream, const TGLVector3& V );

#endif
