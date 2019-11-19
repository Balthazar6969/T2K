
#ifndef TND280MAGFIELD_HXX
#define TND280MAGFIELD_HXX

#include "TEveVector.h"
#include "TEveTrackPropagator.h"


class TEveMagField;

//==============================================================================
// TND280MagField
//==============================================================================
/// Magnetic field class representing ND280.
class TND280MagField : public TEveMagField
{
protected:
    //Initiaze values for the field map used so far by the ND280MC
    double fYokeOuterWidth; 
    double fYokeOuterHeight; 
    double fYokeOuterLength;
    double fYokeInnerWidth; 
    double fYokeInnerHeight; 
    double fYokeInnerLength;
    double fCoilOuterWidth; 
    double fCoilOuterHeight; 
    double fCoilOuterLength;
    double fCoilInnerWidth; 
    double fCoilInnerHeight; 
    double fCoilInnerLength;
    double fMCFieldStrengthX;
     
    
    
public:
    TND280MagField();

    virtual ~TND280MagField() {}
    
    virtual TEveVectorD GetFieldD(Double_t x, Double_t y, Double_t z) const
    { return GetMCFieldValue(x,y,z); }
    TEveVectorD GetMCFieldValue(Double_t x, Double_t y, Double_t z) const;
   
  virtual Double_t GetMaxFieldMagD() const
  { return fMCFieldStrengthX;}
    
    ClassDef(TND280MagField, 0); // Interface to nd280  magnetic field
};


#endif
