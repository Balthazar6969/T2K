#ifndef BasicUtils_h
#define BasicUtils_h

#include "BasicTypes.hxx"
#include "CoreUtils.hxx"
#include "BaseDataClasses.hxx"
#include "SubDetId.hxx"

/// This namespace contains useful functions for analyses
/// SHOULD CONSIDER TEMPLATING THE METHODS!
namespace anaUtils{
 

    /// Create varaible sized arrays of pointers
    void CreateArray(SubDetId::SubDetEnum*  &tgtArr,  int nObj, SubDetId::SubDetEnum ini=SubDetId::kInvalid);
 
}
#endif
