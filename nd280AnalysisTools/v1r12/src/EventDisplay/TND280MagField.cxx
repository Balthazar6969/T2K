#include "TND280MagField.hxx"
TND280MagField::TND280MagField()
: TEveMagField() 
{
    fYokeOuterWidth = 5870 ;
    fYokeOuterHeight = 6086 ;
    fYokeOuterLength = 7489 ;
    fYokeInnerWidth = 3824 ;
    fYokeInnerHeight = 4040 ;
    fYokeInnerLength = 7489 ;
    fCoilOuterWidth = 3824 ;
    fCoilOuterHeight = 4020 ;
    fCoilOuterLength = 7489 ;
    fCoilInnerWidth = 3824 ;
    fCoilInnerHeight = 3540 ;
    fCoilInnerLength = 7009 ;
    fMCFieldStrengthX = 0.2*-100; // 0.2 Tesla * conversion factor for Eve conventions
    fFieldConstant = kFALSE;
}
//********************************************************
TEveVectorD TND280MagField::GetMCFieldValue(Double_t x, Double_t y, Double_t z)  const {
    //********************************************************
    
    TEveVectorD pos(x,y,z);
    TEveVectorD BField(0,0,0);
    
    if (fabs(pos.fX) > fYokeOuterWidth/2.) return BField;
    if (fabs(pos.fY) > fYokeOuterHeight/2.) return BField;
    if (fabs(pos.fZ) > fYokeOuterLength/2.) return BField;
    
    // This is the field strength we use for MC.
    // As backup we use the value from parameters file...
    // scaled to match Eve conventions
    
    if (fabs(pos.fX) < fCoilInnerWidth/2.
    	&& fabs(pos.fY) < fCoilInnerHeight/2. 
    && fabs(pos.fZ) < fCoilInnerLength/2.) {
    // Inside the inner volume of the magnet.  
    // This will eventually come from a measured field map.
    BField.fX= (fMCFieldStrengthX);
    } else if (fabs(pos.fX) < fCoilOuterWidth/2.
    	&& fabs(pos.fY) < fCoilOuterHeight/2. 
    && fabs(pos.fZ) < fCoilOuterLength/2.) {
    // Inside the coil volume of the magnet.
    double Y = (fabs(pos.fY) - fCoilInnerHeight/2.)
    / (fCoilOuterHeight/2. - fCoilInnerHeight/2.);
    double Z = (fabs(pos.fZ) - fCoilInnerLength/2.)
    / (fCoilOuterLength/2. - fCoilInnerLength/2.);
    double depth = std::max(Y,Z);
    if (depth<0.0) depth = 0.0;
    if (depth>1.0) depth = 1.0;
    BField.fX= (fMCFieldStrengthX * (1.0 - depth));
    }
    return BField;
}

