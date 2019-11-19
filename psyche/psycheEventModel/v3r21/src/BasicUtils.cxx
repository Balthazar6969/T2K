#include "BasicUtils.hxx"
#include <TVector3.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <typeinfo>


//********************************************************************
void anaUtils::CreateArray(SubDetId::SubDetEnum* &tgtArr, int nObj, SubDetId::SubDetEnum ini){
//********************************************************************

    tgtArr = new SubDetId::SubDetEnum[nObj];
    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = ini;
    }
}
