#ifndef MiniGeomDefinition_h
#define MiniGeomDefinition_h

#include "string.h"

TVector3 FVdefminFGD1(100,100,7.4);
TVector3 FVdefmaxFGD1(100,100,0);
TVector3 FVdefminFGD2(100,100,7.4);
TVector3 FVdefmaxFGD2(100,100,0);

TVector3 fgd1min(-932.17,-877.17,115.95);
TVector3 fgd1max(932.17,987.17,447.05);
TVector3 fgd2min(-932.17,-877.17,1473.95);
TVector3 fgd2max(932.17,987.17,1807.05);

TVector3 p0dmin(-1092.79,-1107.39,-3296.48);
TVector3 p0dmax(1012.45,1130.99,-938.753);
TVector3 ecalmin(-1000,-1000,2887);
TVector3 ecalmax(1000,1000,3315);
TVector3 smrdLmin(-2848.99,-3098,-3898);
TVector3 smrdLmax(-1832.99,3098,3898);
TVector3 smrdRmin(1832.99,-3098,-3898);
TVector3 smrdRmax(2848.99,3098,3898);
TVector3 smrdDmin(-1832.99,-3098,-3898);
TVector3 smrdDmax(1832.99,-1770.99,3898);
TVector3 smrdUmin(-1832.99,1770.99,-3898);
TVector3 smrdUmax(1832.99,3098,3898);

TVector3 tpc1min(-1092.79,-1107.39,-938.753);
TVector3 tpc1max(1012.45,1130.99,116.045);
TVector3 tpc2min(-1092.79,-1107.39,446.955);
TVector3 tpc2max(1012.45,1130.99,1474.05);
TVector3 tpc3min(-1092.79,-1107.39,1806.96);
TVector3 tpc3max(1012.45,1130.99,2887);

string DefineVolume_Cut(TVector3 posmin, TVector3 posmax, int option, int itrk = 0, int ivtx = 0){
     stringstream out_minX;
     out_minX<<posmin.X();
     string s_minX = out_minX.str();
     stringstream out_minY;
     out_minY<<posmin.Y();
     string s_minY = out_minY.str();
     stringstream out_minZ;
     out_minZ<<posmin.Z();
     string s_minZ = out_minZ.str();
     stringstream out_maxX;
     out_maxX<<posmax.X();
     string s_maxX = out_maxX.str();
     stringstream out_maxY;
     out_maxY<<posmax.Y();
     string s_maxY = out_maxY.str();
     stringstream out_maxZ;
     out_maxZ<<posmax.Z();
     string s_maxZ = out_maxZ.str();
     stringstream out_itrk;
     out_itrk<<itrk;
     string s_itrk = out_itrk.str();
     stringstream out_ivtx;
     out_ivtx<<ivtx;
     string s_ivtx = out_ivtx.str();
     string cut;
       
     if(option == 101){
       if(itrk == 0)
         cut = "(trueVertexPos[trTrueVertexIndex[0]][0]<"+s_maxX+" && trueVertexPos[trTrueVertexIndex[0]][0]>="+s_minX+" && trueVertexPos[trTrueVertexIndex[0]][1]<"+s_maxY+" && trueVertexPos[trTrueVertexIndex[0]][1]>="+s_minY+" && trueVertexPos[trTrueVertexIndex[0]][2]<"+s_maxZ+" && trueVertexPos[trTrueVertexIndex[0]][2]>="+s_minZ+")";
       else if(itrk == 1)
         cut = "(trueVertexPos[trTrueVertexIndex[1]][0]<"+s_maxX+" && trueVertexPos[trTrueVertexIndex[1]][0]>="+s_minX+" && trueVertexPos[trTrueVertexIndex[1]][1]<"+s_maxY+" && trueVertexPos[trTrueVertexIndex[1]][1]>="+s_minY+" && trueVertexPos[trTrueVertexIndex[1]][2]<"+s_maxZ+" && trueVertexPos[trTrueVertexIndex[1]][2]>="+s_minZ+")";

     }
     else if(option == 102){
         cut = "(trueVertexPos["+s_ivtx+"][0]<"+s_maxX+" && trueVertexPos["+s_ivtx+"][0]>="+s_minX+" && trueVertexPos["+s_ivtx+"][1]<"+s_maxY+" && trueVertexPos["+s_ivtx+"][1]>="+s_minY+" && trueVertexPos["+s_ivtx+"][2]<"+s_maxZ+" && trueVertexPos["+s_ivtx+"][2]>="+s_minZ+")";
     }
     else if(option == 103){
         cut = "(trueVertexPos[0][0]<"+s_maxX+" && trueVertexPos[0][0]>="+s_minX+" && trueVertexPos[0][1]<"+s_maxY+" && trueVertexPos[0][1]>="+s_minY+" && trueVertexPos[0][2]<"+s_maxZ+" && trueVertexPos[0][2]>="+s_minZ+")";
     }
     else if(option == 0){
          cut = "(vertex[0][0]<"+s_maxX+" && vertex[0][0]>="+s_minX+" && vertex[0][1]<"+s_maxY+" && vertex[0][1]>="+s_minY+" && vertex[0][2]<"+s_maxZ+" && vertex[0][2]>="+s_minZ+")";
     }
     else if(option == 1){//cluster's algo
          cut = "(vertex[1][0]<"+s_maxX+" && vertex[1][0]>="+s_minX+" && vertex[1][1]<"+s_maxY+" && vertex[1][1]>="+s_minY+" && vertex[1][2]<"+s_maxZ+" && vertex[1][2]>="+s_minZ+")";
     }
     else if(option == 3){
       if(itrk == 0)
         cut="(trPos[0][0]<"+s_maxX+" && trPos[0][0]>="+s_minX+" && trPos[0][1]<"+s_maxY+" && trPos[0][1]>="+s_minY+" && trPos[0][2]<"+s_maxZ+" && trPos[0][2]>="+s_minZ+")";
       if(itrk == 1)
         cut="(trPos[1][0]<"+s_maxX+" && trPos[1][0]>="+s_minX+" && trPos[1][1]<"+s_maxY+" && trPos[1][1]>="+s_minY+" && trPos[1][2]<"+s_maxZ+" && trPos[1][2]>="+s_minZ+")";
     }
     else if(option == 4){
       if(itrk == 0)
         cut = "(trPosEnd[0][0]<"+s_maxX+" && trPosEnd[0][0]>="+s_minX+" && trPosEnd[0][1]<"+s_maxY+" && trPosEnd[0][1]>="+s_minY+" && trPosEnd[0][2]<"+s_maxZ+" && trPosEnd[0][2]>="+s_minZ+")";
       if(itrk == 1)
         cut = "(trPosEnd[1][0]<"+s_maxX+" && trPosEnd[1][0]>="+s_minX+" && trPosEnd[1][1]<"+s_maxY+" && trPosEnd[1][1]>="+s_minY+" && trPosEnd[1][2]<"+s_maxZ+" && trPosEnd[1][2]>="+s_minZ+")";
     }
     
     return cut;  
}


#endif
