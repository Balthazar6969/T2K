#ifndef NumuGeomDefinition_h
#define NumuGeomDefinition_h

#include <string>
#include <iostream>
#include <sstream>
using namespace std;

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
     
     if(option == 100){
       cut = " (sTrueVertexPos[][0]<"+s_maxX+" && sTrueVertexPos[][0]>="+s_minX+" && sTrueVertexPos[][1]<"+s_maxY+" && sTrueVertexPos[][1]>="+s_minY+" && sTrueVertexPos[][2]<"+s_maxZ+" && sTrueVertexPos[][2]>="+s_minZ+")";
       
     }  
     else if(option == 101){
       cut = "(sTrueVertexPos[stdB_trTrueVertexIndex["+s_itrk+"]][0]<"+s_maxX+" && sTrueVertexPos[stdB_trTrueVertexIndex["+s_itrk+"]][0]>="+s_minX+" && sTrueVertexPos[stdB_trTrueVertexIndex["+s_itrk+"]][1]<"+s_maxY+" && sTrueVertexPos[stdB_trTrueVertexIndex["+s_itrk+"]][1]>="+s_minY+" && sTrueVertexPos[stdB_trTrueVertexIndex["+s_itrk+"]][2]<"+s_maxZ+" && sTrueVertexPos[stdB_trTrueVertexIndex["+s_itrk+"]][2]>="+s_minZ+")";
       
     }
     else if(option == 102){
         cut = "(sTrueVertexPos["+s_ivtx+"][0]<"+s_maxX+" && sTrueVertexPos["+s_ivtx+"][0]>="+s_minX+" && sTrueVertexPos["+s_ivtx+"][1]<"+s_maxY+" && sTrueVertexPos["+s_ivtx+"][1]>="+s_minY+" && sTrueVertexPos["+s_ivtx+"][2]<"+s_maxZ+" && sTrueVertexPos["+s_ivtx+"][2]>="+s_minZ+")";
     }
     
     else if(option == 0){// kalman filter algo (TGlobalVertex)
          cut = "(vtxPos[0]<"+s_maxX+" && vtxPos[0]>="+s_minX+" && vtxPos[1]<"+s_maxY+" && vtxPos[1]>="+s_minY+" && vtxPos[2]<"+s_maxZ+" && vtxPos[2]>="+s_minZ+")";
     }
     else if(option == 1){//HM
          cut = "(Vertex[0][0]<"+s_maxX+" && Vertex[0][0]>="+s_minX+" && Vertex[0][1]<"+s_maxY+" && Vertex[0][1]>="+s_minY+" && Vertex[0][2]<"+s_maxZ+" && Vertex[0][2]>="+s_minZ+")";
     }
     else if(option == 2){//HMN
          cut = "(Vertex[1][0]<"+s_maxX+" && Vertex[1][0]>="+s_minX+" && Vertex[1][1]<"+s_maxY+" && Vertex[1][1]>="+s_minY+" && Vertex[1][2]<"+s_maxZ+" && Vertex[1][2]>="+s_minZ+")";
     }
     else if(option == 3){
       cut="(trPosStart["+s_itrk+"][0]<"+s_maxX+" && trPosStart["+s_itrk+"][0]>="+s_minX+" && trPosStart["+s_itrk+"][1]<"+s_maxY+" && trPosStart["+s_itrk+"][1]>="+s_minY+" && trPosStart["+s_itrk+"][2]<"+s_maxZ+" && trPosStart["+s_itrk+"][2]>="+s_minZ+")";
     }
     else if(option == 4){
       cut = "(trPosStartEnd["+s_itrk+"][0]<"+s_maxX+" && trPosStartEnd["+s_itrk+"][0]>="+s_minX+" && trPosStartEnd["+s_itrk+"][1]<"+s_maxY+" && trPosStartEnd["+s_itrk+"][1]>="+s_minY+" && trPosStartEnd["+s_itrk+"][2]<"+s_maxZ+" && trPosStartEnd["+s_itrk+"][2]>="+s_minZ+")";
     }
     
     return cut;  
}


#endif
