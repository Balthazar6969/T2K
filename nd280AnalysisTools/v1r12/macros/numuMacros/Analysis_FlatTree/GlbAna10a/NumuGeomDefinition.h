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
       cut = " (std_TrueVertexPos[][0]<"+s_maxX+" && std_TrueVertexPos[][0]>="+s_minX+" && std_TrueVertexPos[][1]<"+s_maxY+" && std_TrueVertexPos[][1]>="+s_minY+" && std_TrueVertexPos[][2]<"+s_maxZ+" && std_TrueVertexPos[][2]>="+s_minZ+")";
       
     }  
     else if(option == 101){
       cut = "(std_TrueVertexPos[stdB_trTrueVertexIndex["+s_itrk+"]][0]<"+s_maxX+" && std_TrueVertexPos[stdB_trTrueVertexIndex["+s_itrk+"]][0]>="+s_minX+" && std_TrueVertexPos[stdB_trTrueVertexIndex["+s_itrk+"]][1]<"+s_maxY+" && std_TrueVertexPos[stdB_trTrueVertexIndex["+s_itrk+"]][1]>="+s_minY+" && std_TrueVertexPos[stdB_trTrueVertexIndex["+s_itrk+"]][2]<"+s_maxZ+" && std_TrueVertexPos[stdB_trTrueVertexIndex["+s_itrk+"]][2]>="+s_minZ+")";
       
     }
     else if(option == 102){
         cut = "(std_TrueVertexPos["+s_ivtx+"][0]<"+s_maxX+" && std_TrueVertexPos["+s_ivtx+"][0]>="+s_minX+" && std_TrueVertexPos["+s_ivtx+"][1]<"+s_maxY+" && std_TrueVertexPos["+s_ivtx+"][1]>="+s_minY+" && std_TrueVertexPos["+s_ivtx+"][2]<"+s_maxZ+" && std_TrueVertexPos["+s_ivtx+"][2]>="+s_minZ+")";
     }
     
     else if(option == 0){
          cut = "(stdB_Vertex[0][0]<"+s_maxX+" && stdB_Vertex[0][0]>="+s_minX+" && stdB_Vertex[0][1]<"+s_maxY+" && stdB_Vertex[0][1]>="+s_minY+" && stdB_Vertex[0][2]<"+s_maxZ+" && stdB_Vertex[0][2]>="+s_minZ+")";
     }
     else if(option == 1){
          cut = "(stdB_Vertex[1][0]<"+s_maxX+" && stdB_Vertex[1][0]>="+s_minX+" && stdB_Vertex[1][1]<"+s_maxY+" && stdB_Vertex[1][1]>="+s_minY+" && stdB_Vertex[1][2]<"+s_maxZ+" && stdB_Vertex[1][2]>="+s_minZ+")";
     }
     else if(option == 3){
       cut="(stdB_trPositionStart["+s_itrk+"][0]<"+s_maxX+" && stdB_trPositionStart["+s_itrk+"][0]>="+s_minX+" && stdB_trPositionStart["+s_itrk+"][1]<"+s_maxY+" && stdB_trPositionStart["+s_itrk+"][1]>="+s_minY+" && stdB_trPositionStart["+s_itrk+"][2]<"+s_maxZ+" && stdB_trPositionStart["+s_itrk+"][2]>="+s_minZ+")";
     }
     else if(option == 4){
       cut = "(stdB_trPositionStartEnd["+s_itrk+"][0]<"+s_maxX+" && stdB_trPositionStartEnd["+s_itrk+"][0]>="+s_minX+" && stdB_trPositionStartEnd["+s_itrk+"][1]<"+s_maxY+" && stdB_trPositionStartEnd["+s_itrk+"][1]>="+s_minY+" && stdB_trPositionStartEnd["+s_itrk+"][2]<"+s_maxZ+" && stdB_trPositionStartEnd["+s_itrk+"][2]>="+s_minZ+")";
     }
     
     return cut;  
}


#endif
