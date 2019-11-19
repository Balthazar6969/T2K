#include "NumuCutsDefinition.h"

#include "../Tools/NumuEffTools.C"
NumuDataFiles data;
TFile fmc0(data.mcFile[0].c_str()); // load neut to make check plot


void McEffPur(int sel=0){
   
    NumuEffTools effTools;
 
    NumuCutsDefinition cuts(-1,true);
    effTools._cutSTART=0;
    effTools._NMCFILES=1;
    int ncuts;
    if(sel==0)ncuts=cuts.icutCC+1;
    else ncuts=cuts.icutCCQE+1;
   
         
    std::string true_cut[NMAXCUTS];
    std::string sel_cut[NMAXCUTS];
    std::string seltrue_cut[NMAXCUTS];

 
    
    if(sel==0){
      
        // MC: CC CUTS                               
        true_cut[0]="std_BunchNbr==0 && ("+cuts.fgdFV_cutMC2[0]+") && std_ReacCode>0 && std_ReacCode<30 &&  std_TrueNuPDG==14 ";
        seltrue_cut[0]="stdB_trTrueVertexIndex[0]>-1 && "+cuts.fgdFV_cutMC[0]+" && std_ReacCode[stdB_trTrueVertexIndex[0]]>0 && std_ReacCode[stdB_trTrueVertexIndex[0]]<30 &&  std_TrueNuPDG[stdB_trTrueVertexIndex[0]]==14";
    }
    else{
        // MC: CCQE CUTS
        true_cut[0]="std_TrueSpillInfo==1 && ("+cuts.fgdFV_cutMC2[0]+") && std_ReacCode==1 &&  std_TrueNuPDG==14 ";
        seltrue_cut[0]="stdB_trTrueVertexIndex[0]>-1 && "+cuts.fgdFV_cutMC[0]+" && std_ReacCode[stdB_trTrueVertexIndex[0]]==1 &&  std_TrueNuPDG[stdB_trTrueVertexIndex[0]]==14";
    }

    fmc0.cd();
    for(int icut=0;icut<ncuts;icut++){
        sel_cut[icut]="("+cuts.scut[0][icut]+")";
        true_cut[icut]=true_cut[0];
        seltrue_cut[icut]=seltrue_cut[0]+" && ("+cuts.scut[0][icut]+")";//+" && "+true_cut[0];
    }
  
    effTools.DrawEffPur(true_cut,seltrue_cut,sel_cut,ncuts,"effpurvscut","Efficiency and purity for each cut",0,ncuts,0,1.1,"cut # ",0);

}
