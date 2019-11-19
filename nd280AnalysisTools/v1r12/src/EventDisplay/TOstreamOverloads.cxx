#include "TOstreamOverloads.hxx"
//
// convenient overloads for printing things out
//
ostream& operator<< ( ostream& theStream, TLorentzVector& V )
{
    
  
   TString X;
    X+=V.X();
    X+=" ";
    X+=V.Y();
    X+=" ";
    X+=V.Z();
    X+=" ";
    X+=V.T();   
    theStream<<X;
    return theStream;
}

ostream& operator<< ( ostream& theStream, TVector3& V )
{

    TString X;
    X+=V.X();
    X+=" ";
    X+=V.Y();
    X+=" ";
    X+=V.Z();
    
    theStream<<X;
    return theStream;
}

ostream& operator<< ( ostream& theStream,const  TVector3& V )
{ 
    TString X;
    X+=V.X();
    X+=" ";
    X+=V.Y();
    X+=" ";
    X+=V.Z();
    
    theStream<<X;
    return theStream;
}

ostream& operator<< ( ostream& theStream, TGLVertex3& V )
{
   TString X;
    X+=V.X();
    X+=" ";
    X+=V.Y();
    X+=" ";
    X+=V.Z();
    return theStream;
}

ostream& operator<< ( ostream& theStream, TGLVector3& V )
{
TString X;
    X+=V.X();
    X+=" ";
    X+=V.Y();
    X+=" ";
    X+=V.Z();
    return theStream;
}
ostream& operator<< ( ostream& theStream, const TGLVector3& V )
{
TString X;
    X+=V.X();
    X+=" ";
    X+=V.Y();
    X+=" ";
    X+=V.Z();
    return theStream;
}

