#include "TSimpleGeometry.hxx"
#include <iostream>
#include "TROOT.h"
#include "TEveManager.h"
#include "TEveBox.h"
#include "TFile.h"
#include "TGLScenePad.h"



using std::cout;
using std::endl;

TSimpleGeometry::TSimpleGeometry(int hard_coded_build){
    
    // Add in useless code to remove compiler warnings
    if(hard_coded_build){
        std::cout << "";
    }
    
    // gGeoManager->PushPath();
    fBoxGeometry=new TEveElementList("ND280","ND280");
//!    TEveRGBAPalette* pal = new TEveRGBAPalette(0, 100);
//!    fBoxGeometry->SetPalette(pal);
    //  fBoxGeometry->SetRenderMode(TEveDigitSet::kRM_Line ); 
    // DEFINE TYPE OF BOX, AND VALUE IS COLOUR, 512 IS CHUNKSIZE
//!    fBoxGeometry->Reset(TEveElementList::kBT_FreeBox  ,kTRUE,1024);
    fBoxGeometry->SetPickable(kFALSE);
//!    fBoxGeometry->SetAntiFlick(kFALSE);
    
    int color;
    
    Float_t box[24];
    TEveElementList* SubLevel=new TEveElementList("Basket","Basket");
    fBoxGeometry->AddElement(SubLevel);
   
    // path : /t2k_1/OA_0/Magnet_0/Basket_0/P0D_1
    color=2;
    box[0]=-1160;
    box[1]=-1177;
    box[2]=-3325;
    box[3]=-1160;
    box[4]=1234.8;
    box[5]=-3325;
    box[6]=1113.8;
    box[7]=1234.8;
    box[8]=-3325;
    box[9]=1113.8;
    box[10]=-1177;
    box[11]=-3325;
    box[12]=-1160;
    box[13]=-1177;
    box[14]=-914.17;
    box[15]=-1160;
    box[16]=1234.8;
    box[17]=-914.17;
    box[18]=1113.8;
    box[19]=1234.8;
    box[20]=-914.17;
    box[21]=1113.8;
    box[22]=-1177;
    box[23]=-914.17;
    AddBoxToElementList(SubLevel,box,color,"P0D");
    // path : /t2k_1/OA_0/Magnet_0/Basket_0/Tracker_0/FGD2_0
    color=6;
    box[0]=-1150;
    box[1]=-1189.2;
    box[2]=1478.54;
    box[3]=-1150;
    box[4]=1210.8;
    box[5]=1478.54;
    box[6]=1150;
    box[7]=1210.8;
    box[8]=1478.54;
    box[9]=1150;
    box[10]=-1189.2;
    box[11]=1478.54;
    box[12]=-1150;
    box[13]=-1189.2;
    box[14]=1843.54;
    box[15]=-1150;
    box[16]=1210.8;
    box[17]=1843.54;
    box[18]=1150;
    box[19]=1210.8;
    box[20]=1843.54;
    box[21]=1150;
    box[22]=-1189.2;
    box[23]=1843.54;
    AddBoxToElementList(SubLevel,box,color,"FGD2");
    // path : /t2k_1/OA_0/Magnet_0/Basket_0/Tracker_0/FGD1_0
    box[0]=-1150;
    box[1]=-1189.2;
    box[2]=119.54;
    box[3]=-1150;
    box[4]=1210.8;
    box[5]=119.54;
    box[6]=1150;
    box[7]=1210.8;
    box[8]=119.54;
    box[9]=1150;
    box[10]=-1189.2;
    box[11]=119.54;
    box[12]=-1150;
    box[13]=-1189.2;
    box[14]=484.54;
    box[15]=-1150;
    box[16]=1210.8;
    box[17]=484.54;
    box[18]=1150;
    box[19]=1210.8;
    box[20]=484.54;
    box[21]=1150;
    box[22]=-1189.2;
    box[23]=484.54;
    AddBoxToElementList(SubLevel,box,color,"FGD1");
    // path : /t2k_1/OA_0/Magnet_0/Basket_0/Tracker_0/TPC1_0
     color=7;
    box[0]=-1150;
    box[1]=-1189.2;
    box[2]=-864.46;
    box[3]=-1150;
    box[4]=1210.8;
    box[5]=-864.46;
    box[6]=1150;
    box[7]=1210.8;
    box[8]=-864.46;
    box[9]=1150;
    box[10]=-1189.2;
    box[11]=-864.46;
    box[12]=-1150;
    box[13]=-1189.2;
    box[14]=109.54;
    box[15]=-1150;
    box[16]=1210.8;
    box[17]=109.54;
    box[18]=1150;
    box[19]=1210.8;
    box[20]=109.54;
    box[21]=1150;
    box[22]=-1189.2;
    box[23]=109.54;
    AddBoxToElementList(SubLevel,box,color,"TPC1");
    // path : /t2k_1/OA_0/Magnet_0/Basket_0/Tracker_0/TPC2_0
    box[0]=-1150;
    box[1]=-1189.2;
    box[2]=494.54;
    box[3]=-1150;
    box[4]=1210.8;
    box[5]=494.54;
    box[6]=1150;
    box[7]=1210.8;
    box[8]=494.54;
    box[9]=1150;
    box[10]=-1189.2;
    box[11]=494.54;
    box[12]=-1150;
    box[13]=-1189.2;
    box[14]=1468.54;
    box[15]=-1150;
    box[16]=1210.8;
    box[17]=1468.54;
    box[18]=1150;
    box[19]=1210.8;
    box[20]=1468.54;
    box[21]=1150;
    box[22]=-1189.2;
    box[23]=1468.54;
    AddBoxToElementList(SubLevel,box,color,"TPC2");
    // path : /t2k_1/OA_0/Magnet_0/Basket_0/Tracker_0/TPC3_0
    box[0]=-1150;
    box[1]=-1189.2;
    box[2]=1853.54;
    box[3]=-1150;
    box[4]=1210.8;
    box[5]=1853.54;
    box[6]=1150;
    box[7]=1210.8;
    box[8]=1853.54;
    box[9]=1150;
    box[10]=-1189.2;
    box[11]=1853.54;
    box[12]=-1150;
    box[13]=-1189.2;
    box[14]=2827.54;
    box[15]=-1150;
    box[16]=1210.8;
    box[17]=2827.54;
    box[18]=1150;
    box[19]=1210.8;
    box[20]=2827.54;
    box[21]=1150;
    box[22]=-1189.2;
    box[23]=2827.54;
    AddBoxToElementList(SubLevel,box,color,"TPC3");
    // path : /t2k_1/OA_0/Magnet_0/Basket_0/DsECal_0
    box[0]=-1000;
    box[1]=-1000;
    box[2]=2887.25;
    box[3]=-1000;
    box[4]=1000;
    box[5]=2887.25;
    box[6]=1000;
    box[7]=1000;
    box[8]=2887.25;
    box[9]=1000;
    box[10]=-1000;
    box[11]=2887.25;
    box[12]=-1000;
    box[13]=-1000;
    box[14]=3315;
    box[15]=-1000;
    box[16]=1000;
    box[17]=3315;
    box[18]=1000;
    box[19]=1000;
    box[20]=3315;
    box[21]=1000;
    box[22]=-1000;
    box[23]=3315;
    AddBoxToElementList(SubLevel,box,color,"DsECal");
    SubLevel=new TEveElementList("P0DEcal","P0DEcal");
    fBoxGeometry->AddElement(SubLevel);
     color=9;
    // path : /t2k_1/OA_0/Magnet_0/RightClam_0/P0DECal_0/Bottom_0
    box[0]=-1500;
    box[1]=-1410;
    box[2]=-3404.5;
    box[3]=-1500;
    box[4]=-1410;
    box[5]=-1074.5;
    box[6]=-100;
    box[7]=-1410;
    box[8]=-1074.5;
    box[9]=-100;
    box[10]=-1410;
    box[11]=-3404.5;
    box[12]=-1500;
    box[13]=-1520;
    box[14]=-3404.5;
    box[15]=-1500;
    box[16]=-1520;
    box[17]=-1074.5;
    box[18]=-100;
    box[19]=-1520;
    box[20]=-1074.5;
    box[21]=-100;
    box[22]=-1520;
    box[23]=-3404.5;
    AddBoxToElementList(SubLevel,box,color,"P0DECal/Bottom Right");
    
    // path : /t2k_1/OA_0/Magnet_0/RightClam_0/P0DECal_0/Side_0
    box[0]=-1422;
    box[1]=-1300;
    box[2]=-1074.5;
    box[3]=-1422;
    box[4]=-1300;
    box[5]=-3404.5;
    box[6]=-1422;
    box[7]=1300;
    box[8]=-3404.5;
    box[9]=-1422;
    box[10]=1300;
    box[11]=-1074.5;
    box[12]=-1532;
    box[13]=-1300;
    box[14]=-1074.5;
    box[15]=-1532;
    box[16]=-1300;
    box[17]=-3404.5;
    box[18]=-1532;
    box[19]=1300;
    box[20]=-3404.5;
    box[21]=-1532;
    box[22]=1300;
    box[23]=-1074.5;
    AddBoxToElementList(SubLevel,box,color,"P0DECal/Right Side");
   
    // path : /t2k_1/OA_0/Magnet_0/RightClam_0/P0DECal_0/Top_0
    box[0]=-1450;
    box[1]=1410;
    box[2]=-1074.5;
    box[3]=-1450;
    box[4]=1410;
    box[5]=-3404.5;
    box[6]=-50;
    box[7]=1410;
    box[8]=-3404.5;
    box[9]=-50;
    box[10]=1410;
    box[11]=-1074.5;
    box[12]=-1450;
    box[13]=1520;
    box[14]=-1074.5;
    box[15]=-1450;
    box[16]=1520;
    box[17]=-3404.5;
    box[18]=-50;
    box[19]=1520;
    box[20]=-3404.5;
    box[21]=-50;
    box[22]=1520;
    box[23]=-1074.5;
    AddBoxToElementList(SubLevel,box,color,"P0DECal/Top Right");
 
    // path : /t2k_1/OA_0/Magnet_0/LeftClam_0/P0DECal_0/Bottom_0
    box[0]=100;
    box[1]=-1410;
    box[2]=-3404.5;
    box[3]=100;
    box[4]=-1410;
    box[5]=-1074.5;
    box[6]=1500;
    box[7]=-1410;
    box[8]=-1074.5;
    box[9]=1500;
    box[10]=-1410;
    box[11]=-3404.5;
    box[12]=100;
    box[13]=-1520;
    box[14]=-3404.5;
    box[15]=100;
    box[16]=-1520;
    box[17]=-1074.5;
    box[18]=1500;
    box[19]=-1520;
    box[20]=-1074.5;
    box[21]=1500;
    box[22]=-1520;
    box[23]=-3404.5;
    AddBoxToElementList(SubLevel,box,color,"P0DECal/Bottom Left");
   
    // path : /t2k_1/OA_0/Magnet_0/LeftClam_0/P0DECal_0/Side_0
    box[0]=1532;
    box[1]=1300;
    box[2]=-3404.5;
    box[3]=1532;
    box[4]=1300;
    box[5]=-1074.5;
    box[6]=1532;
    box[7]=-1300;
    box[8]=-1074.5;
    box[9]=1532;
    box[10]=-1300;
    box[11]=-3404.5;
    box[12]=1422;
    box[13]=1300;
    box[14]=-3404.5;
    box[15]=1422;
    box[16]=1300;
    box[17]=-1074.5;
    box[18]=1422;
    box[19]=-1300;
    box[20]=-1074.5;
    box[21]=1422;
    box[22]=-1300;
    box[23]=-3404.5;
    AddBoxToElementList(SubLevel,box,color,"P0DECal/Left Side");
    
    // path : /t2k_1/OA_0/Magnet_0/LeftClam_0/P0DECal_0/Top_0
    box[0]=50;
    box[1]=1410;
    box[2]=-1074.5;
    box[3]=50;
    box[4]=1410;
    box[5]=-3404.5;
    box[6]=1450;
    box[7]=1410;
    box[8]=-3404.5;
    box[9]=1450;
    box[10]=1410;
    box[11]=-1074.5;
    box[12]=50;
    box[13]=1520;
    box[14]=-1074.5;
    box[15]=50;
    box[16]=1520;
    box[17]=-3404.5;
    box[18]=1450;
    box[19]=1520;
    box[20]=-3404.5;
    box[21]=1450;
    box[22]=1520;
    box[23]=-1074.5;
    AddBoxToElementList(SubLevel,box,color,"P0DECal/Top Left");
    SubLevel=new TEveElementList("BrlECal","BrlECal");
    fBoxGeometry->AddElement(SubLevel);
     color=8;
   
    
    // path : /t2k_1/OA_0/Magnet_0/LeftClam_0/BrlECal_0/Side_0
    box[0]=1362.75;
    box[1]=-1150;
    box[2]=3404.5;
    box[3]=1362.75;
    box[4]=1150;
    box[5]=3404.5;
    box[6]=1362.75;
    box[7]=1150;
    box[8]=-795.5;
    box[9]=1362.75;
    box[10]=-1150;
    box[11]=-795.5;
    box[12]=1832;
    box[13]=-1150;
    box[14]=3404.5;
    box[15]=1832;
    box[16]=1150;
    box[17]=3404.5;
    box[18]=1832;
    box[19]=1150;
    box[20]=-795.5;
    box[21]=1832;
    box[22]=-1150;
    box[23]=-795.5;
    AddBoxToElementList(SubLevel,box,color,"BrlECal/Left Side");
     // path : /t2k_1/OA_0/Magnet_0/LeftClam_0/BrlECal_0/Bottom_0
    box[0]=100;
    box[1]=-1320.75;
    box[2]=-795.5;
    box[3]=100;
    box[4]=-1320.75;
    box[5]=3404.5;
    box[6]=1600;
    box[7]=-1320.75;
    box[8]=3404.5;
    box[9]=1600;
    box[10]=-1320.75;
    box[11]=-795.5;
    box[12]=100;
    box[13]=-1770;
    box[14]=-795.5;
    box[15]=100;
    box[16]=-1770;
    box[17]=3404.5;
    box[18]=1600;
    box[19]=-1770;
    box[20]=3404.5;
    box[21]=1600;
    box[22]=-1770;
    box[23]=-795.5;
    AddBoxToElementList(SubLevel,box,color,"BrlECal/Bottom Left");
    // path : /t2k_1/OA_0/Magnet_0/RightClam_0/BrlECal_0/Top_0
    box[0]=-1550;
    box[1]=1320.75;
    box[2]=3404.5;
    box[3]=-1550;
    box[4]=1320.75;
    box[5]=-795.5;
    box[6]=-50;
    box[7]=1320.75;
    box[8]=-795.5;
    box[9]=-50;
    box[10]=1320.75;
    box[11]=3404.5;
    box[12]=-1550;
    box[13]=1770;
    box[14]=3404.5;
    box[15]=-1550;
    box[16]=1770;
    box[17]=-795.5;
    box[18]=-50;
    box[19]=1770;
    box[20]=-795.5;
    box[21]=-50;
    box[22]=1770;
    box[23]=3404.5;
    AddBoxToElementList(SubLevel,box,color,"BrlECal/Top Right");
    // path : /t2k_1/OA_0/Magnet_0/RightClam_0/BrlECal_0/Side_0
    box[0]=-1362.75;
    box[1]=-1150;
    box[2]=-795.5;
    box[3]=-1362.75;
    box[4]=1150;
    box[5]=-795.5;
    box[6]=-1362.75;
    box[7]=1150;
    box[8]=3404.5;
    box[9]=-1362.75;
    box[10]=-1150;
    box[11]=3404.5;
    box[12]=-1832;
    box[13]=-1150;
    box[14]=-795.5;
    box[15]=-1832;
    box[16]=1150;
    box[17]=-795.5;
    box[18]=-1832;
    box[19]=1150;
    box[20]=3404.5;
    box[21]=-1832;
    box[22]=-1150;
    box[23]=3404.5;
    AddBoxToElementList(SubLevel,box,color,"BrlECal/Right Side");
// path : /t2k_1/OA_0/Magnet_0/RightClam_0/BrlECal_0/Bottom_0
    box[0]=-1600;
    box[1]=-1320.75;
    box[2]=-795.5;
    box[3]=-1600;
    box[4]=-1320.75;
    box[5]=3404.5;
    box[6]=-100;
    box[7]=-1320.75;
    box[8]=3404.5;
    box[9]=-100;
    box[10]=-1320.75;
    box[11]=-795.5;
    box[12]=-1600;
    box[13]=-1770;
    box[14]=-795.5;
    box[15]=-1600;
    box[16]=-1770;
    box[17]=3404.5;
    box[18]=-100;
    box[19]=-1770;
    box[20]=3404.5;
    box[21]=-100;
    box[22]=-1770;
    box[23]=-795.5;
    AddBoxToElementList(SubLevel,box,color,"BrlEcal/Bottom Right");  
    // path : /t2k_1/OA_0/Magnet_0/LeftClam_0/BrlECal_0/Top_0
    box[0]=50;
    box[1]=1320.75;
    box[2]=3404.5;
    box[3]=50;
    box[4]=1320.75;
    box[5]=-795.5;
    box[6]=1550;
    box[7]=1320.75;
    box[8]=-795.5;
    box[9]=1550;
    box[10]=1320.75;
    box[11]=3404.5;
    box[12]=50;
    box[13]=1770;
    box[14]=3404.5;
    box[15]=50;
    box[16]=1770;
    box[17]=-795.5;
    box[18]=1550;
    box[19]=1770;
    box[20]=-795.5;
    box[21]=1550;
    box[22]=1770;
    box[23]=3404.5;
    AddBoxToElementList(SubLevel,box,color,"BrlECal/Top Left");
    SubLevel = new TEveElementList("Magnet","Magnet");
    fBoxGeometry->AddElement(SubLevel);
    box [0]=0;
    box [1]=-3075;
    box [2]=-3861.5;
    
    box [3]=0;
    box [4]=3075;
    box [5]=-3861.5;
    
    box [6]=2855;
    box [7]=3075;
    box [8]=-3861.5;
    
    box [9]=2855;
    box [10]=-3075;
    box [11]=-3861.5;
    
    box [12]=0;
    box [13]=-3075;
    box [14]=3861.5;
    
    box [15]=0;
    box [16]=3075;
    box [17]=3861.5;
    
    box [18]=2855;
    box [19]=3075;
    box [20]=3861.5;
    
    box [21]=2855;
    box [22]=-3075;
    box [23]=3861.5;
    
    color=4;
    
    AddBoxToElementList(SubLevel,box,color,"Magnet/Left Side");
    
    box [0]=-2855;
    box [1]=-3075;
    box [2]=-3861.5;
    
    box [3]=-2855;
    box [4]=3075;
    box [5]=-3861.5;
    
    box [6]=0;
    box [7]=3075;
    box [8]=-3861.5;
    
    box [9]=0;
    box [10]=-3075;
    box [11]=-3861.5;
    
    box [12]=-2855;
    box [13]=-3075;
    box [14]=3861.5;
    
    box [15]=-2855;
    box [16]=3075;
    box [17]=3861.5;
    
    box [18]=0;
    box [19]=3075;
    box [20]=3861.5;
    
    box [21]=0;
    box [22]=-3075;
    box [23]=3861.5;
    
    AddBoxToElementList(SubLevel,box,color,"Magnet/Right Side");
    
    fSimpleGeometryScene = gEve->SpawnNewScene("Simple Geomery");
    fSimpleGeometryScene->GetGLScene()->SetSelectable(kFALSE);
    
    fSimpleGeometryScene->AddElement(fBoxGeometry);
}

TSimpleGeometry::TSimpleGeometry(){
    
    gGeoManager->PushPath();
    fBoxGeometry=new TEveElementList("fBoxGeometry","fBoxGeometry");
//!    TEveRGBAPalette* pal = new TEveRGBAPalette(0, 100);
//!    fBoxGeometry->SetPalette(pal);
    //  fBoxGeometry->SetRenderMode(TEveDigitSet::kRM_Line ); 
    // DEFINE TYPE OF BOX, AND VALUE IS COLOUR, 512 IS CHUNKSIZE
//!    fBoxGeometry->Reset(TEveElementList::kBT_FreeBox  ,kTRUE,1024);
    fBoxGeometry->SetPickable(kFALSE);
//!    fBoxGeometry->SetAntiFlick(kFALSE);
    
    TEveElementList* SubLevel=new TEveElementList("Basket","Basket");
    fBoxGeometry->AddElement(SubLevel);
    
    int  color=4;
    //  addElementToSimpleGeometry(fBoxGeometry,"/t2k_1/OA_0",color);
    
    //  addElementToSimpleGeometry(fBoxGeometry,"/t2k_1/OA_0/Magnet_0/Basket_0/P0D_1",color);
    /*
    //   if(fSubDetector[ND::TSimpleGeometry::FluxReturn]->IsOn()){
    //   	color=TColor::GetColor(fSubDetector[ND::TSimpleGeometry::FluxReturn]->GetColor());
    
    */
    
    /* bizarrely - these crash on exit from root!!! */
    
    
    
    
    // addElementToSimpleGeometry(fBoxGeometry,"/t2k_1/OA_0/Magnet_0/RightClam_0",color);  
    
    //   }
    
    //   if(fSubDetector[ND::TSimpleGeometry::P0D]->IsOn())
    //   {
    //   	color=TColor::GetColor(fSubDetector[ND::TSimpleGeometry::P0D]->GetColor());
    // This uses a namespace access as defined in oaEvent 
    
    
    color=3;
    
    
    addElementToSimpleGeometry(SubLevel,"/t2k_1/OA_0/Magnet_0/Basket_0/P0D_0",color);
    //   }
    
    
    
    
    //    if(fSubDetector[ND::TSimpleGeometry::FGD]->IsOn()){
    //    	color=TColor::GetColor(fSubDetector[ND::TSimpleGeometry::FGD]->GetColor());
    color=6;
    addElementToSimpleGeometry(SubLevel,"/t2k_1/OA_0/Magnet_0/Basket_0/Tracker_0/FGD2_0",color);
    addElementToSimpleGeometry(SubLevel,"/t2k_1/OA_0/Magnet_0/Basket_0/Tracker_0/FGD1_0",color);
    
    //    }
    //    if(fSubDetector[ND::TSimpleGeometry::TPC]->IsOn()){   	
    //    	color=TColor::GetColor(fSubDetector[ND::TSimpleGeometry::TPC]->GetColor());
    
    color=7;
    addElementToSimpleGeometry(SubLevel,"/t2k_1/OA_0/Magnet_0/Basket_0/Tracker_0/TPC1_0",color);
    addElementToSimpleGeometry(SubLevel,"/t2k_1/OA_0/Magnet_0/Basket_0/Tracker_0/TPC2_0",color);  
    addElementToSimpleGeometry(SubLevel,"/t2k_1/OA_0/Magnet_0/Basket_0/Tracker_0/TPC3_0",color);
    //    	addElementToSimpleGeometry(fBoxGeometry,ND::GeomId::TPC::TPC2(),color); 	
    //    	addElementToSimpleGeometry(fBoxGeometry,ND::GeomId::TPC::TPC3(),color);
    /*maybe too much?
    for (int tpc=1 ; tpc<4; tpc++) 
    for(int half=0;  half<2; half ++) 
    for(int mm=0 ; mm<12;mm++) 
    {
    TString Path(Form
    ("/t2k_1/OA_0/Magnet_0/Basket_0/Tracker_0/TPC%d_0/GasGap_0/Drift_0/Half_%d/MM_%d",tpc,half,mm));
    //  cout<<" path name .."<<Path<<endl;
    addElementToSimpleGeometry(fBoxGeometry,Path,color); 
    }
    
    */	
    //    if(fSubDetector[ND::TSimpleGeometry::DsECal]->IsOn()){
    //    	color=TColor::GetColor(fSubDetector[ND::TSimpleGeometry::DsECal]->GetColor());
    color=8;
    addElementToSimpleGeometry(SubLevel,"/t2k_1/OA_0/Magnet_0/Basket_0/DsECal_0",color);
    //    }
    //    for(int clam = 0;clam<2;clam++)
    //    	for(int module=0;module<3;module++)
    //   	{
    //    	    if(fSubDetector[ND::TSimpleGeometry::P0DECal]->IsOn()){
    //    	    	color=TColor::GetColor(fSubDetector[ND::TSimpleGeometry::P0DECal]->GetColor());
     SubLevel=new TEveElementList("P0DEcal");
    fBoxGeometry->AddElement(SubLevel);
   
    addElementToSimpleGeometry(SubLevel,"/t2k_1/OA_0/Magnet_0/RightClam_0/P0DECal_2/Bottom_0",color);
    addElementToSimpleGeometry(SubLevel,"/t2k_1/OA_0/Magnet_0/RightClam_0/P0DECal_1/Side_0",color);
    addElementToSimpleGeometry(SubLevel,"/t2k_1/OA_0/Magnet_0/LeftClam_0/P0DECal_0/Top_0",color);
    addElementToSimpleGeometry(SubLevel,"/t2k_1/OA_0/Magnet_0/RightClam_0/P0DECal_0/Top_0",color);
    addElementToSimpleGeometry(SubLevel,"/t2k_1/OA_0/Magnet_0/LeftClam_0/P0DECal_1/Side_0",color);
    addElementToSimpleGeometry(SubLevel,"/t2k_1/OA_0/Magnet_0/LeftClam_0/P0DECal_2/Bottom_0",color);

    color = 4;
    SubLevel=new TEveElementList("BrlECal");
    fBoxGeometry->AddElement(SubLevel);
   
    addElementToSimpleGeometry(SubLevel,"/t2k_1/OA_0/Magnet_0/RightClam_0/BrlECal_0/Top_0",color);
    addElementToSimpleGeometry(SubLevel,"/t2k_1/OA_0/Magnet_0/LeftClam_0/BrlECal_2/Bottom_0",color);
    addElementToSimpleGeometry(SubLevel,"/t2k_1/OA_0/Magnet_0/RightClam_0/BrlECal_1/Side_0",color);
    addElementToSimpleGeometry(SubLevel,"/t2k_1/OA_0/Magnet_0/LeftClam_0/BrlECal_1/Side_0",color);
    addElementToSimpleGeometry(SubLevel,"/t2k_1/OA_0/Magnet_0/LeftClam_0/BrlECal_0/Top_0",color);
    addElementToSimpleGeometry(SubLevel,"/t2k_1/OA_0/Magnet_0/RightClam_0/BrlECal_2/Bottom_0",color);
    
    //    	    if(fSubDetector[ND::TSimpleGeometry::BrlECal]->IsOn()){
    //    	    	color=TColor::GetColor(fSubDetector[ND::TSimpleGeometry::BrlECal]->GetColor());
    
    //    	    	addElementToSimpleGeometry(fBoxGeometry,ND::GeomId::TECal::Module(clam,module),color);
    //    	    }
    //    	}
    
    //  	addElementToSimpleGeometry(fBoxGeometry,"/t2k_1/OA_0/Magnet_0/RightClam_0",color);  
    
    color=4;
    //gGeoManager->cd("/t2k_1/OA_0/Magnet_0/LeftClam_0");
    
    Float_t box[24];
    
    /*
    box[0]=50.;
    box[1]=1320.75;
    box[2]=3404.5;
    box[3]=50.;
    box[4]=1320.75;
    box[5]=-795.5;
    box[6]=1550.;
    box[7]=1320.75;
    box[8]=-795.5;
    box[9]=1550.;
    
    
    box[10]=1320.75;
    box[11]=3404.5;
    box[12]=50.;
    box[13]=1770.;
    box[14]=3404.5;
    box[15]=50.;
    box[16]=1770.;
    box[17]=-795.5;
    box[18]=1550.;
    box[19]=1770.;
    box[20]=-795.5;
    
    box[21]=1550;
    box[22]=1770;
    box[23]=3404.5;
    */
    
    box [0]=0;
    box [1]=-3075;
    box [2]=-3861.5;
    
    box [3]=0;
    box [4]=3075;
    box [5]=-3861.5;
    
    box [6]=2855;
    box [7]=3075;
    box [8]=-3861.5;
    
    box [9]=2855;
    box [10]=-3075;
    box [11]=-3861.5;
    
    box [12]=0;
    box [13]=-3075;
    box [14]=3861.5;
    
    box [15]=0;
    box [16]=3075;
    box [17]=3861.5;
    
    box [18]=2855;
    box [19]=3075;
    box [20]=3861.5;
    
    box [21]=2855;
    box [22]=-3075;
    box [23]=3861.5;
    
    
    color=4;
    
    SubLevel=new TEveElementList("Magnet");
    fBoxGeometry->AddElement(SubLevel);
   
    AddBoxToElementList(SubLevel,box,color,"");
    
    
    
    //	fBoxGeometry->AddBox(box);
    //	fBoxGeometry->DigitColor(color);
    
    box [0]=-2855;
    box [1]=-3075;
    box [2]=-3861.5;
    
    box [3]=-2855;
    box [4]=3075;
    box [5]=-3861.5;
    
    box [6]=0;
    box [7]=3075;
    box [8]=-3861.5;
    
    box [9]=0;
    box [10]=-3075;
    box [11]=-3861.5;
    
    box [12]=-2855;
    box [13]=-3075;
    box [14]=3861.5;
    
    box [15]=-2855;
    box [16]=3075;
    box [17]=3861.5;
    
    box [18]=0;
    box [19]=3075;
    box [20]=3861.5;
    
    box [21]=0;
    box [22]=-3075;
    box [23]=3861.5;
    
    AddBoxToElementList(SubLevel,box,color,"");	
    // 	fBoxGeometry->AddBox(box);
    
    // 	fBoxGeometry->DigitColor(color);
    
    fSimpleGeometryScene = gEve->SpawnNewScene("Simple Geomery");
    fSimpleGeometryScene->GetGLScene()->SetSelectable(kFALSE);
    
    fSimpleGeometryScene->AddElement(fBoxGeometry);
    gGeoManager->PopPath();
    // force 3d view to be wireframe in this case...
    
    //if(f3Dchoice !=NULL) f3Dchoice->Select(TGLRnrCtx::kWireFrame );// make this the selected one in the list too
    //f3Dchoice->SetEnabled(kFALSE); // disallow changing the 3d style, when we switch back we can read back the 
    // correct value
}


void TSimpleGeometry::addElementToSimpleGeometry(TEveElementList* nd280,TString path)
{
    if(gGeoManager->CheckPath(path))
    {
    	gGeoManager->cd(path);
    	//  CurrentNode =  gGeoManager->GetCurrentNode();
    	AddCurrentVolumeToSimpleGeometry(nd280);  	
    }
    else
    	cout<<" Failed to find path "<<path<<" when constructing simple geometry "<<endl;
}
void TSimpleGeometry::addElementToSimpleGeometry(TEveElementList* nd280,TString path,int color)
{
    if(gGeoManager->CheckPath(path))
    {
    	gGeoManager->cd(path);
    	//  CurrentNode =  gGeoManager->GetCurrentNode();
    	AddCurrentVolumeToSimpleGeometry(nd280,color);  	
    }
    else
    	cout<<" Failed to find path "<<path<<" when constructing simple geometry "<<endl;
}
void TSimpleGeometry::AddCurrentVolumeToSimpleGeometry(TEveElementList* nd280 ){
    int color = TColor::GetColor(255,0,0);
    // cout<<" color from getcolor = "<<color<<endl;
    AddCurrentVolumeToSimpleGeometry(nd280,color );
}
void TSimpleGeometry::AddCurrentVolumeToSimpleGeometry(TEveElementList* nd280, int color ){
    TGeoShape* shape = gGeoManager->GetCurrentVolume()->GetShape();
    if(shape == NULL)  return;
    const int npoints=8;
    const int ndimensions=3;
    Double_t* local=shape->GetBuffer3D(TBuffer3D::kAll,true).fPnts; // get 8 vertices in local coordinates
    
    Double_t  master[npoints*ndimensions];
    for (int i = 0;i<npoints*3;i+=3){
    	gGeoManager->LocalToMaster(&local[i],&master[i]);
    }
    Float_t fMaster[ndimensions*npoints];
    for(int i=0;i<ndimensions*npoints;i++){
    	fMaster[i]=(Float_t)master[i];
    	//	if(fMaster[i]==0)fMaster[i]=1;
    	
    }
    // cout<<" adding to box set the box... ";
    //  for(int i=0;i<ndimensions*npoints;i++)	cout<<fMaster[i]<<" ";
    //  cout<<endl;
    
    //   float charge=100;
    //    Color_t color2 =  TColor::GetColor(0,255,0);
    AddBoxToElementList(nd280,fMaster,color,"");
    
}
void TSimpleGeometry::AddBoxToElementList(TEveElementList* set,Float_t* box,int color,TString text)
{ 
/*    int nmax=5000;

    if( set->GetPlex()->Size()==nmax){
    	cout<<" AddBoxToElementList WARNING too much data to draw, giving up "<<endl;
    	cout<<" No more than "<<nmax<<" objects allowed in this box set"<<endl;
    	cout<< " Next object to draw would have been:"<<endl<<text<<endl;
    }
    if( set->GetPlex()->Size()>nmax)return;
    */
    /* cout<<" Adding Box "<<endl;
    int count=0;
    for(int i=0;i<8;i++)
    {
    cout<<endl;
    for(int j = 0 ; j<3 ; j++)
    {
    cout<<"box ["<<count<<"]="<<box[count]<<"; "<<endl;
    count++;
    }
    }
    cout<<endl;
    */
    TEveBox* B=new TEveBox(text,text);
    B->SetVertices(box);
    B->SetMainColor(color);
    set->AddElement(B);
//    set->AddBox(box);
    
//    set->DigitColor(color);
    
    //  set->DigitValue(color);
    //   set->DigitId(new TND280ObjectInEve(base,text));
}
