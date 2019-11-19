#include "NumuCutsDefinition.h"
#include "../Tools/NumuDataMcCompTools.C"
#include "../Tools/NumuEffTools.C"
#include "NumuDataFiles.h"


//**************************************************
void fgdhits_study(int mode=-1){

    NumuDataMcCompTools comp;
    NumuCutsDefinition cuts;
    NumuEffTools effTools;
    NumuDataFiles data;
    
    

  
    string s_acut_null[NMAXTRACKS];
    string s_acut[NMAXTRACKS];
    string s_acut_a[NMAXTRACKS];
    string s_acut_b[NMAXTRACKS];
    string s_acut_c[NMAXTRACKS];
    string s_acut_d[NMAXTRACKS];
    string s_acut_e[NMAXTRACKS];
    string s_acut_f[NMAXTRACKS];
    string s_var[NMAXTRACKS];
    string s_var_a[NMAXTRACKS];
    string s_var_b[NMAXTRACKS];
    string s_var_c[NMAXTRACKS];
    string s_var_d[NMAXTRACKS];
    string s_var_e[NMAXTRACKS];
    string s_var_null[NMAXTRACKS];

    string datafileslist=data.dataFile_txt;
    double datapot= comp.GetDataPOT(datafileslist);
    string mcfileslist0=data.mcFile_txt[0];
    double mcpot0=comp.GetMcPOT(mcfileslist0);
    string mcfileslist1=data.mcFile_txt[1];
    double mcpot1=comp.GetMcPOT(mcfileslist1);
    comp.SetNorm(datapot,mcpot0,mcpot1);
    string basic=" 1==1 ";
    int NCUTS;
    if(mode == -2 || mode == 1){
        effTools._cutSTART=cuts.icutCCQE;//16
        effTools._fileSTART=0;
        effTools._NMCFILES=1;
        int ncuts=cuts.icutCCQE+1;
        std::string true_cut[NMAXCUTS];
        std::string sel_cut[NMAXCUTS];
        std::string seltrue_cut[NMAXCUTS];
        // MC: CCQE CUTS
        true_cut[0]="std_TrueSpillInfo==1 && ("+cuts.fgdFV_cutMC2[0]+") && std_ReacCode==1 &&  std_TrueNuPDG==14 ";
        seltrue_cut[0]="stdB_trTrueVertexIndex[0]>-1 && "+cuts.fgdFV_cutMC[0]+" && std_ReacCode[stdB_trTrueVertexIndex[0]]==1 &&  std_TrueNuPDG[stdB_trTrueVertexIndex[0]]==14";

        //=================================================================
        //do for different cut with respect to default= without cutin 10
        //1) wo
        cuts.reset();
        cuts.scutin[0][10]="1==1";
        cuts.scutin[1][10]="1==1";
        cuts.DoCumulativeCut();
        for(int icut=0;icut<ncuts;icut++){
            sel_cut[icut]="("+cuts.scut[0][icut]+")";
            true_cut[icut]=true_cut[0];
            seltrue_cut[icut]=seltrue_cut[0]+" && ("+cuts.scut[0][icut]+")";//+" && "+true_cut[0];
        }
        effTools.DrawEffPur(true_cut,seltrue_cut,sel_cut,ncuts,"effpurvscut","Efficiency and purity for each cut",0,ncuts,0,1.1,"cut # ",0);
        //=================================================================
        //do for different cut with respect to default= without cutin 10
        //2) use unused cut
        cuts.reset();
        cuts.scutin[0][10]="( ("+ cuts.fgd1FV_cut[0]+" && std_NFGDUnusedHits[0]<15 ) || ("+cuts.fgd2FV_cut[0]+" && std_NFGDUnusedHits[1]<8))";
        cuts.scutin[1][10]="( ("+ cuts.fgd1FV_cut[0]+" && std_NFGDUnusedHits[0]<15 ) || ("+cuts.fgd2FV_cut[0]+" && std_NFGDUnusedHits[1]<8))";
        cuts.DoCumulativeCut();
        for(int icut=0;icut<ncuts;icut++){
            sel_cut[icut]="("+cuts.scut[0][icut]+")";
            true_cut[icut]=true_cut[0];
            seltrue_cut[icut]=seltrue_cut[0]+" && ("+cuts.scut[0][icut]+")";//+" && "+true_cut[0];
        }
        effTools.DrawEffPur(true_cut,seltrue_cut,sel_cut,ncuts,"effpurvscut","Efficiency and purity for each cut",0,ncuts,0,1.1,"cut # ",0);
        //=================================================================
        //do for different cut with respect to default= without cutin 10
        //2) use unused cut + NFGDtracks<2
        cuts.reset();
        cuts.scutin[0][10]="( ("+ cuts.fgd1FV_cut[0]+" && std_NFGDUnusedHits[0]<15 ) || ("+cuts.fgd2FV_cut[0]+" && std_NFGDUnusedHits[1]<8)) && stdB_NFgdTracksBunch<2";
        cuts.scutin[1][10]="( ("+ cuts.fgd1FV_cut[0]+" && std_NFGDUnusedHits[0]<15 ) || ("+cuts.fgd2FV_cut[0]+" && std_NFGDUnusedHits[1]<8)) && stdB_NFgdTracksBunch<2";
        cuts.DoCumulativeCut();
        for(int icut=0;icut<ncuts;icut++){
            sel_cut[icut]="("+cuts.scut[0][icut]+")";
            true_cut[icut]=true_cut[0];
            seltrue_cut[icut]=seltrue_cut[0]+" && ("+cuts.scut[0][icut]+")";//+" && "+true_cut[0];
        }
        effTools.DrawEffPur(true_cut,seltrue_cut,sel_cut,ncuts,"effpurvscut","Efficiency and purity for each cut",0,ncuts,0,1.1,"cut # ",0);
         //=================================================================
        //do for different cut with respect to default= without cutin 10
        //2) use unused cut + NFGDtracks==0
        cuts.reset();
        cuts.scutin[0][10]="( ("+ cuts.fgd1FV_cut[0]+" && std_NFGDUnusedHits[0]<15 ) || ("+cuts.fgd2FV_cut[0]+" && std_NFGDUnusedHits[1]<8)) && stdB_NFgdTracksBunch==0";
        cuts.scutin[1][10]="( ("+ cuts.fgd1FV_cut[0]+" && std_NFGDUnusedHits[0]<15 ) || ("+cuts.fgd2FV_cut[0]+" && std_NFGDUnusedHits[1]<8)) && stdB_NFgdTracksBunch==0";
        cuts.DoCumulativeCut();
        for(int icut=0;icut<ncuts;icut++){
            sel_cut[icut]="("+cuts.scut[0][icut]+")";
            true_cut[icut]=true_cut[0];
            seltrue_cut[icut]=seltrue_cut[0]+" && ("+cuts.scut[0][icut]+")";//+" && "+true_cut[0];
        }
        effTools.DrawEffPur(true_cut,seltrue_cut,sel_cut,ncuts,"effpurvscut","Efficiency and purity for each cut",0,ncuts,0,1.1,"cut # ",0);
         //=================================================================
        //do for different cut with respect to default= without cutin 10
        //2) use unused cut + sumused hits<25
        cuts.reset();
        cuts.scutin[0][10]="( ("+ cuts.fgd1FV_cut[0]+" && std_NFGDUnusedHits[0]<15 ) || ("+cuts.fgd2FV_cut[0]+" && std_NFGDUnusedHits[1]<8)) && Alt$(stdB_fgdtr_NHits[0],0)+Alt$(stdB_fgdtr_NHits[1],0)+Alt$(stdB_fgdtr_NHits[2],0)+Alt$(stdB_fgdtr_NHits[3],0)<25";
        cuts.scutin[1][10]="( ("+ cuts.fgd1FV_cut[0]+" && std_NFGDUnusedHits[0]<15 ) || ("+cuts.fgd2FV_cut[0]+" && std_NFGDUnusedHits[1]<8)) &&Alt$(stdB_fgdtr_NHits[0],0)+Alt$(stdB_fgdtr_NHits[1],0)+Alt$(stdB_fgdtr_NHits[2],0)+Alt$(stdB_fgdtr_NHits[3],0)<25";
        cuts.DoCumulativeCut();
        for(int icut=0;icut<ncuts;icut++){
            sel_cut[icut]="("+cuts.scut[0][icut]+")";
            true_cut[icut]=true_cut[0];
            seltrue_cut[icut]=seltrue_cut[0]+" && ("+cuts.scut[0][icut]+")";//+" && "+true_cut[0];
        }
        effTools.DrawEffPur(true_cut,seltrue_cut,sel_cut,ncuts,"effpurvscut","Efficiency and purity for each cut",0,ncuts,0,1.1,"cut # ",0);
        
    }
    
    if(mode == -2 || mode == 2){
        //do not compare with data
        comp._draw_data=false;
        comp._draw_MC=true;
        comp._nmc=1;
        comp._nmc0=7;
        comp._draw_leg=true;
        comp._draw_histo=true;
        comp._batch=true;
        
        //do not put first the cut on unused fgd hits
        comp._cuts->reset();
        comp._cuts->scutin[0][10]="1==1";
        comp._cuts->scutin[1][10]="1==1";
        comp._cuts->DoCumulativeCut();
        
        NCUTS=cuts.icutCCQE+1;
        
        s_acut_a[0]=basic+" && "+cuts.fgd1FV_cut[0];
        s_acut_b[0]=basic+" && "+cuts.fgd1FV_cut[0]+" && Alt$(stdB_fgdtr_Det[0],0)==4 ";
        s_acut_c[0]=basic+" && "+cuts.fgd1FV_cut[0]+" && Alt$(stdB_fgdtr_Det[1],0)==4 ";
        s_acut_d[0]=basic+" && "+cuts.fgd1FV_cut[0]+" && Alt$(stdB_fgdtr_Det,0)==4 ";
        
        s_var_a[0]="std_NFGDUnusedHits[0]";
        s_var_b[0]="stdB_fgdtr_NHits[0]";
        s_var_c[0]="stdB_fgdtr_NHits[1]";
        s_var_d[0]="Alt$(stdB_fgdtr_NHits[0],0)+Alt$(stdB_fgdtr_NHits[1],0)+Alt$(stdB_fgdtr_NHits[2],0)+Alt$(stdB_fgdtr_NHits[3],0)";
        s_var_e[0]="stdB_NFgdTracksBunch";
        
        comp._plot_name="nFgd1Tr_15";
        comp.draw_var(NCUTS-1,s_var_e," Nb of fgd1 iso tracks ",10, 0,10,-1,s_acut_a,1);
        comp._plot_name="unusedFgdHits_vtxFGD1";
        comp.draw_var(NCUTS-1,s_var_a," N unused FGD hits ",30, 0,30,-1,s_acut_a,1);
        comp._plot_name="fgdtr1FgdHits_vtxFGD1";
  //       comp.draw_var(NCUTS-1,s_var_b," N hits of fgd1 trk1 when rec vtx is in FGD1 ",100, 0,100,-1,s_acut_b,1);
//         comp._plot_name="fgdtr2FgdHits_vtxFGD1";
//         comp.draw_var(NCUTS-1,s_var_c," N hits of fgd1 trk2 when rec vtx is in FGD1 ",100, 0,100,-1,s_acut_c,1);
        comp._plot_name="FgdHits_vtxFGD1";
        comp.draw_var(NCUTS-1,s_var_d,"Total nb of used hits when rec vtx is in FGD1 ",30, 0,30,-1,s_acut_a,1);
        comp._plot_name="FgdHits_vtxFGD1_zoom";
        comp.draw_var(NCUTS-1,s_var_d,"Total nb of used hits when rec vtx is in FGD1 ",30, 0,30,10,s_acut_a,1);

        //for fgd2
        s_acut_a[0]=basic+" && "+cuts.fgd2FV_cut[0];
        s_acut_b[0]=basic+" && "+cuts.fgd2FV_cut[0]+" && Alt$(stdB_fgdtr_Det[0],0)==5 ";
        s_acut_c[0]=basic+" && "+cuts.fgd2FV_cut[0]+" && Alt$(stdB_fgdtr_Det[1],0)==5 ";
        s_acut_d[0]=basic+" && "+cuts.fgd2FV_cut[0]+" && stdB_fgdtr_Det==5";
    
        s_var_a[0]="std_NFGDUnusedHits[1]";
        
        comp._plot_name="nFgd2Tr_15";
        comp.draw_var(NCUTS-1,s_var_e," Nb of fgd2 iso tracks ",10, 0,10,-1,s_acut_a,1);
        comp._plot_name="unusedFgdHits_vtxFGD2";
        comp.draw_var(NCUTS-1,s_var_a," N unused FGD2 hits when rec vtx is in FGD2 ",30, 0,30,-1,s_acut_a,1);
    //     comp._plot_name="fgdtr1FgdHits_vtxFGD2";
//         comp.draw_var(NCUTS-1,s_var_b," N hits of fgd2 trk1 when rec vtx is in FGD2 ",100, 0,100,-1,s_acut_b,1);
//         comp._plot_name="fgdtr2FgdHits_vtxFGD2";
//        comp.draw_var(NCUTS-1,s_var_c," N hits of fgd2 trk2 when rec vtx is in FGD2 ",100, 0,100,-1,s_acut_c,1);
        comp._plot_name="FgdHits_vtxFGD2";
        comp.draw_var(NCUTS-1,s_var_d,"Total nb of used hits when rec vtx is in FGD2 ",30, 0,30,-1,s_acut_a,1);
        comp._plot_name="FgdHits_vtxFGD2_zoom";
        comp.draw_var(NCUTS-1,s_var_d,"Total nb of used hits when rec vtx is in FGD2 ",30, 0,30,10,s_acut_a,1);
              
    }
    //
    if(mode==-2 || mode==3){
      //do not compare with data
        comp._draw_data=true;
        comp._draw_MC=true;
        comp._nmc=1;
        comp._nmc0=7;
        comp._draw_leg=true;
        comp._draw_histo=true;
        comp._batch=true;
        
        s_acut_a[0]=basic;
        s_var_a[0]="stdB_ENuRec[0]";//"stdB_trENuRec_1Muon";

        comp._cuts->reset();
        comp._cuts->scutin[0][10]="( ("+ cuts.fgd1FV_cut[0]+" && std_NFGDUnusedHits[0]<15 ) || ("+cuts.fgd2FV_cut[0]+" && std_NFGDUnusedHits[1]<8))";
        comp._cuts->scutin[1][10]="( ("+ cuts.fgd1FV_cut[0]+" && std_NFGDUnusedHits[0]<15 ) || ("+cuts.fgd2FV_cut[0]+" && std_NFGDUnusedHits[1]<8))";
        comp._cuts->DoCumulativeCut();
        comp._plot_name="RecEnu_cutCCQE_unusedFgdhitsCut";
        comp.draw_var(cuts.icutCCQE,s_var_a,"Reconstructed neutrino energy for last CCQE cut",30, 0,3000,-1,s_acut_a,1);
        comp._plot_name="RecEnu_cutCC_unusedFgdhitsCut";
        comp.draw_var(cuts.icutCC,s_var_a,"Reconstructed neutrino energy for CC cut",30, 0,3000,-1,s_acut_a,1);
        comp._plot_name="RecEnu_cutCCQE1track_unusedFgdhits";
        comp.draw_var(cuts.icutCCQE-6,s_var_a,"Reconstructed neutrino energy for before 2tracks cut",30, 0,3000,-1,s_acut_a,1);

     
          comp._cuts->reset();
        comp._cuts->scutin[0][10]="1==1";
        comp._cuts->scutin[1][10]="1==1";
        comp._cuts->DoCumulativeCut();
        comp._plot_name="RecEnu_cutCCQE";
        comp.draw_var(cuts.icutCCQE,s_var_a,"Reconstructed neutrino energy for last CCQE cut",30, 0,3000,-1,s_acut_a,1);
        comp._plot_name="RecEnu_cutCC";
        comp.draw_var(cuts.icutCC,s_var_a,"Reconstructed neutrino energy for CC cut",30, 0,3000,-1,s_acut_a,1);
        comp._plot_name="RecEnu_cutCCQE1track";
        comp.draw_var(cuts.icutCCQE-6,s_var_a,"Reconstructed neutrino energy for before 2tracks cut",30, 0,3000,-1,s_acut_a,1);

        
    }
  
 
}

