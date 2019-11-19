#include "NumuDataMcCompTools.h"

//**************************************************
NumuDataMcCompTools::NumuDataMcCompTools(string option, int canvas_nbr){
//**************************************************
    
    if(option!=""){ 
        _outfile = new TFile("data_mc_comp.root",option.c_str());
        _write_in_rootfile=true;
    }
    else _write_in_rootfile=false;

    
    _fdata= new TFile(_data.dataFile.c_str(),"r");

    for(int i=0;i<NMCMAX;i++){
        _fmc0[i]= new TFile(_data.mcFile[i].c_str(),"r");
    }
    if (_positive_ana)  
        _cuts = new NumuCutsDefinition(1);
    else
        _cuts = new NumuCutsDefinition(-1);
    
       
    _plotpath="../GlbAna10a/plots/";
    _comp_genie = false;
    _positive_ana = false;
    _draw_histo = true;
    _draw_MC = true;
    _draw_data = true;
    _draw_leg = false;
    _normalize_area = false;
    _save_histos = false;
    _logY = false;
    _plot_name = "";
    _ncanvas=0;
  
//_part_types = false;
    _part_types_tr1 = false;
    _part_types_tr2 = false;
    _out_fv = false;
    _out_fv_int = false;
    _out_fv_part_tr1 = false;
    _commonVtx_bg = false;
    _bg = false;
    _run_3132 = false;
    _run_3334 = false;
    _nc_bkg = false;
    _bg_out_fv = false;
    _bg_fv_cc = false;
    _bg_fv_ccqe = false;
    _cc = false;
    _ccqe = false;
    _batch =false;

  
    for (int i=0;i<5;i++)
        _h[i]=NULL;

     for (int i=0;i<5;i++)
         _h2[i]=NULL;

  

    _nmc=NMCMAX;
    _nmc0=NMC0MAX;

}
                          
void NumuDataMcCompTools::DefineMcComp(string *mc_cut, string *mc_cat_name,int *mc_cat_color,int itrkCut){

      for(int imc=0;imc<NMC0MAX;imc++){
          mc_cut[imc]=" ";
          mc_cat_name[imc]=" ";
          mc_cat_color[imc]=0;
      }
    bool _fpart_types[2]={_part_types_tr1,_part_types_tr2};
    bool _fout_fv_part[2]={_out_fv_part_tr1,_out_fv_part_tr2};    
    stringstream out_itrkCut;out_itrkCut<<itrkCut;
    string s_itrkCut = out_itrkCut.str();
    for(int itrk=0;itrk<2;itrk++){
        stringstream out_itrk;out_itrk<<itrk;
        string s_itrk = out_itrk.str();
      
        if (_fpart_types[itrk]){
            mc_cut[6]= " && (trTruePDG["+s_itrk+"] == 13)"; //muon
            mc_cut[5]= " && (trTruePDG["+s_itrk+"] == 11)"; //electron
            mc_cut[4]= " && (trTruePDG["+s_itrk+"] == -211)"; //pi-
            mc_cut[3]= " && (trTruePDG["+s_itrk+"] == -13)"; //antimu
            mc_cut[2]= " && (trTruePDG["+s_itrk+"] == -11)"; //positron
            mc_cut[1]= " && (trTruePDG["+s_itrk+"] == 211)"; //pi+
            mc_cut[0]= " && (trTruePDG["+s_itrk+"] == 2212)"; //proton
        }
        if (_fout_fv_part[itrk]){
            mc_cut[6]= " && trTrueVertexIndex["+s_itrk+"]>-1 && !"+_cuts->fgdFV_cutMC[itrkCut]+" && (trTruePDG["+s_itrk+"] == 13)"; //muon
            mc_cut[5]= " && trTrueVertexIndex["+s_itrk+"]>-1 && !"+_cuts->fgdFV_cutMC[itrkCut]+" && (trTruePDG["+s_itrk+"] == 11)"; //electron
            mc_cut[4]= " && trTrueVertexIndex["+s_itrk+"]>-1 && !"+_cuts->fgdFV_cutMC[itrkCut]+" && (trTruePDG["+s_itrk+"] == -211)"; //pi-
            mc_cut[3]= " && trTrueVertexIndex["+s_itrk+"]>-1 && !"+_cuts->fgdFV_cutMC[itrkCut]+" && (trTruePDG["+s_itrk+"] == -13)"; //antimu
            mc_cut[2]= " && trTrueVertexIndex["+s_itrk+"]>-1 && !"+_cuts->fgdFV_cutMC[itrkCut]+" && (trTruePDG["+s_itrk+"] == -11)"; //positron
            mc_cut[1]= " && trTrueVertexIndex["+s_itrk+"]>-1 && !"+_cuts->fgdFV_cutMC[itrkCut]+" && (trTruePDG["+s_itrk+"] == 211)"; //pi+
            mc_cut[0]= " && trTrueVertexIndex["+s_itrk+"]>-1 && !"+_cuts->fgdFV_cutMC[itrkCut]+" && (trTruePDG["+s_itrk+"] == 2212)"; //proton
        }
    }
    
    if (_out_fv){
        mc_cut[0]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && !"+_cuts->fgdFV_cutMC[itrkCut];
        mc_cut[1]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && !"+_cuts->fgdFV_cutMC[itrkCut]+" && (sTrueVertexDet[trTrueVertexIndex["+s_itrkCut+"]]==4 || sTrueVertexDet[trTrueVertexIndex["+s_itrkCut+"]]==5)";
        mc_cut[2]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && !"+_cuts->fgdFV_cutMC[itrkCut]+" && (sTrueVertexDet[trTrueVertexIndex["+s_itrkCut+"]]==6)";
        mc_cut[3]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && !"+_cuts->fgdFV_cutMC[itrkCut]+" && (sTrueVertexDet[trTrueVertexIndex["+s_itrkCut+"]]==7)";
        mc_cut[4]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && !"+_cuts->fgdFV_cutMC[itrkCut]+" && (sTrueVertexDet[trTrueVertexIndex["+s_itrkCut+"]]==8)";
        mc_cut[5]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && !"+_cuts->fgdFV_cutMC[itrkCut]+" && (sTrueVertexDet[trTrueVertexIndex["+s_itrkCut+"]]==9)";
        mc_cut[6]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && !"+_cuts->fgdFV_cutMC[itrkCut]+" && (sTrueVertexDet[trTrueVertexIndex["+s_itrkCut+"]]>9 || sTrueVertexDet[trTrueVertexIndex["+s_itrkCut+"]]==0)";
    }
    else if (_out_fv_int){
        mc_cut[0]= " && 1==2";
        mc_cut[1]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==-14)   && !"+_cuts->fgdFV_cutMC[itrkCut];
        mc_cut[2]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (ReacCode[trTrueVertexIndex["+s_itrkCut+"]]>30 && sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==14)  && !"+_cuts->fgdFV_cutMC[itrkCut];
        mc_cut[3]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (ReacCode[trTrueVertexIndex["+s_itrkCut+"]]==16 && sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==14) && !"+_cuts->fgdFV_cutMC[itrkCut];
        mc_cut[4]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (ReacCode[trTrueVertexIndex["+s_itrkCut+"]]>16   && ReacCode[trTrueVertexIndex["+s_itrkCut+"]]<30 && sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==14) && !"+_cuts->fgdFV_cutMC[itrkCut];
        mc_cut[5]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (ReacCode[trTrueVertexIndex["+s_itrkCut+"]]>=11  && ReacCode[trTrueVertexIndex["+s_itrkCut+"]]<=13 && sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==14) && !"+_cuts->fgdFV_cutMC[itrkCut];
        mc_cut[6]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (ReacCode[trTrueVertexIndex["+s_itrkCut+"]]==1 && sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==14)  && !"+_cuts->fgdFV_cutMC[itrkCut];
    }
    else if (_nc_bkg) {	
        // NC out of FV
        mc_cut[0]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]>30 && sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==14)  && !"+_cuts->fgdFV_cutMC[itrkCut];
        // all out of FV
        mc_cut[1]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && !(sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]>30 && sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==14)  && !"+_cuts->fgdFV_cutMC[itrkCut];
        // NC in FV
        mc_cut[2]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]>30 && sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==14)  && "+_cuts->fgdFV_cutMC[itrkCut];
    }
    
    else if (_commonVtx_bg)
        mc_cut[0]= " && (trTrueVertexIndex["+s_itrkCut+"] != trTrueVertexIndex[1])";
    else if (_bg) {	
        // NC out of FV
        mc_cut[0]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]>30 && sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==14)  && !"+_cuts->fgdFV_cutMC[itrkCut];
        // anti numu out of FV
        mc_cut[1]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==-14)   && !"+_cuts->fgdFV_cutMC[itrkCut];
        // nue out of FV
        mc_cut[2]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==12)   && !"+_cuts->fgdFV_cutMC[itrkCut];
        // anti nue out of FV
        mc_cut[3]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==-12)   && !"+_cuts->fgdFV_cutMC[itrkCut];
        // NC in FV
        mc_cut[4]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]>30 && sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==14)  && "+_cuts->fgdFV_cutMC[itrkCut];
        // anti numu in FV      
        mc_cut[5]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==-14)   && "+_cuts->fgdFV_cutMC[itrkCut];
        // nue in FV
        mc_cut[6]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==12)   && "+_cuts->fgdFV_cutMC[itrkCut];
        // anti nue in FV
        mc_cut[7]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==-12)   && "+_cuts->fgdFV_cutMC[itrkCut];
        //CC out of FV
        mc_cut[8]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]==16 && sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==14) && !"+_cuts->fgdFV_cutMC[itrkCut];
        mc_cut[9]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]>16   && sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]<30 && sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==14) && !"+_cuts->fgdFV_cutMC[itrkCut];
        mc_cut[10]= " && trTrueVertexIndex[0]>-1 && (sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]>=11  && sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]<=13 && sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==14) && !"+_cuts->fgdFV_cutMC[itrkCut];
        mc_cut[11]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]==1 && sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==14)  && !"+_cuts->fgdFV_cutMC[itrkCut];
        //CC in FV
        mc_cut[12]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]==16 && sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==14) && "+_cuts->fgdFV_cutMC[itrkCut];
        mc_cut[13]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]>16   && sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]<30 && sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==14) && "+_cuts->fgdFV_cutMC[itrkCut];
        mc_cut[14]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]>=11  && sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]<=13 && sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==14) && "+_cuts->fgdFV_cutMC[itrkCut];
        mc_cut[15]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]==1 && sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==14)  && "+_cuts->fgdFV_cutMC[itrkCut];
    }
  
    else if (_bg_out_fv) {	
        // NC out of FV
        mc_cut[0]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]>30 && sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==14)  && !"+_cuts->fgdFV_cutMC[itrkCut];
        // anti numu out of FV
        mc_cut[1]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==-14)   && !"+_cuts->fgdFV_cutMC[itrkCut];
        // nue out of FV
        mc_cut[2]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==12)   && !"+_cuts->fgdFV_cutMC[itrkCut];
        // anti nue out of FV
        mc_cut[3]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==-12)   && !"+_cuts->fgdFV_cutMC[itrkCut];
        //CC out of FV
        mc_cut[4]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]==16 && sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==14) && !"+_cuts->fgdFV_cutMC[itrkCut];
        mc_cut[5]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]>16   && sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]<30 && sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==14) && !"+_cuts->fgdFV_cutMC[itrkCut];
        mc_cut[6]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]>=11  && sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]<=13 && sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==14) && !"+_cuts->fgdFV_cutMC[itrkCut];
        mc_cut[7]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]==1 && sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==14)  && !"+_cuts->fgdFV_cutMC[itrkCut];
    }
      
    else if (_bg_fv_cc) {	
        // NC in FV
        mc_cut[0]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]>30 && sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==14)  && "+_cuts->fgdFV_cutMC[itrkCut];
        // anti numu in FV      
        mc_cut[1]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==-14)   && "+_cuts->fgdFV_cutMC[itrkCut];
        // nue in FV
        mc_cut[2]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==12)   && "+_cuts->fgdFV_cutMC[itrkCut];
        // anti nue in FV
        mc_cut[3]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==-12)   && "+_cuts->fgdFV_cutMC[itrkCut];
    }

    else if (_bg_fv_ccqe) {	
        // NC in FV
        mc_cut[0]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]>30 && sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==14)  && "+_cuts->fgdFV_cutMC[itrkCut];
        // anti numu in FV
        mc_cut[1]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==-14)   && "+_cuts->fgdFV_cutMC[itrkCut];
        // nue out in FV
        mc_cut[2]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==12)   && "+_cuts->fgdFV_cutMC[itrkCut];
        // anti nue in FV
        mc_cut[3]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==-12)   && "+_cuts->fgdFV_cutMC[itrkCut];
        //COH in FV
        mc_cut[4]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]==16 && sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==14) && "+_cuts->fgdFV_cutMC[itrkCut];
        //DIS in FV
        mc_cut[5]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]>16   && sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]<30 && sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==14) && "+_cuts->fgdFV_cutMC[itrkCut];
        //RES in FV
        mc_cut[6]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]>=11  && sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]<=13 && sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==14) && "+_cuts->fgdFV_cutMC[itrkCut];
    }
    else{
        mc_cut[0]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && !"+_cuts->fgdFV_cutMC[itrkCut];
        mc_cut[1]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==-14)   && "+_cuts->fgdFV_cutMC[itrkCut];
        mc_cut[2]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]>30 && sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==14)  && "+_cuts->fgdFV_cutMC[itrkCut];
        mc_cut[3]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]==16 && sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==14) && "+_cuts->fgdFV_cutMC[itrkCut];
        mc_cut[4]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]>16   && sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]<30 && sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==14) && "+_cuts->fgdFV_cutMC[itrkCut];
        mc_cut[5]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]>=11  && sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]<=13 && sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==14) && "+_cuts->fgdFV_cutMC[itrkCut];
        mc_cut[6]= " && trTrueVertexIndex["+s_itrkCut+"]>-1 && (sTrueReacCode[trTrueVertexIndex["+s_itrkCut+"]]==1 && sTrueNuPDG[trTrueVertexIndex["+s_itrkCut+"]]==14)  && "+_cuts->fgdFV_cutMC[itrkCut];
    }
    
    std::string mc_cat_name0[7]={"nofid","anti","NC","COH","DIS","RES","QE"};
    int mc_cat_color0[7]       ={ 1,      31,    6,   5,    4,   3,    2   };
    
    std::string mc_cat_name1[1] = {"noComVtx"};
    int mc_cat_color1[1]        = { 4        };
    
    std::string mc_cat_name2[16] = {"NC nofid","anti numu nofid","nue nofid","anti nue nofid","NC","anti nunu","nue", "anti nue", "COH nofid","DIS nofid","RES nofid","QE nofid","COH","DIS","RES","QE"};

    int mc_cat_color2cc[16] = { 1,  2,  3,  4, 17,  6,  7,  8, 9, 9, 9, 9, 4, 5, 6, 7}; // cc bg
    int mc_cat_color2ccqe[16] = { 1, 30, 10, 29, 17, 41, 46, 38, 6, 7, 8, 9, 4, 3, 2, 7}; // ccqe bg
    
    std::string    mc_cat_name3[8] = {"NC","anti","nue","antinue","COH","DIS","RES","QE"};

    int mc_cat_color3cc[8] = { 6,      31,    7,       28,    5,    5,    5,   5};// cc bg out FV
    int mc_cat_color3ccqe[8] = { 6,      31,    7,       28,    5,    4,    3,   2};// ccqe bg out FV      
    
    std::string mc_cat_name4[4]={"NC","anti","nue","antinue"};
    int mc_cat_color4[4]       ={ 6,    31,    7,     28    }; //cc bg in FV
    
    std::string mc_cat_name5[7]={"NC","anti","nue","antinue","COH","DIS","RES"};
    int mc_cat_color5[7]       ={ 6,    31,    7,     28,      5,    4,    3  }; //ccqe bg in FV
    
    for(int imc=0;imc<_nmc0;imc++){
        if(_commonVtx_bg){
            mc_cat_name[imc]=mc_cat_name1[imc];
            mc_cat_color[imc]=mc_cat_color1[imc];
        }
        else if(_bg){
            mc_cat_name[imc]=mc_cat_name2[imc];
            if(_cc) mc_cat_color[imc]=mc_cat_color2cc[imc];
            if(_ccqe) mc_cat_color[imc]=mc_cat_color2ccqe[imc];
        }
        else if(_bg_out_fv){
            mc_cat_name[imc]=mc_cat_name3[imc];
            if(_cc) mc_cat_color[imc]=mc_cat_color3cc[imc];
            if(_ccqe) mc_cat_color[imc]=mc_cat_color3ccqe[imc];
        }
        else if(_bg_fv_cc){
            mc_cat_name[imc]=mc_cat_name4[imc];
            mc_cat_color[imc]=mc_cat_color4[imc];
        }
        else if(_bg_fv_ccqe){
            mc_cat_name[imc]=mc_cat_name5[imc];
            mc_cat_color[imc]=mc_cat_color5[imc];
        }
        else{
            mc_cat_name[imc]=mc_cat_name0[imc];
            mc_cat_color[imc]=mc_cat_color0[imc];
        }
    }            
}
//**************************************************
std::string NumuDataMcCompTools::GetCut(int icut,string acut,int itrkCut){
//**************************************************
    stringstream out_itrkCut;out_itrkCut<<itrkCut;
    string s_itrkCut = out_itrkCut.str();
    std::string scut3;
    if (icut == -1){
        scut3 = " 1==1 &&"+acut;
    }
    else if (icut == -2){
        scut3 = acut+ "&& trTrueVertexIndex["+s_itrkCut+"]>-1 && "+ _cuts->fgdFV_cutMC[itrkCut];
    }
    else {
        scut3 =acut+" && "+ _cuts->scut[itrkCut][icut];
    }

    
    return scut3;
}

//**************************************************
void NumuDataMcCompTools::draw_histo(TH1F* h, int lc, int lw, int fc, int fs, char* opt){
//**************************************************

    h->SetLineColor(lc);
    h->SetLineWidth(lw);
    h->SetFillColor(fc);
    h->SetFillStyle(fs);
    h->Draw(opt);
    std::cout << h->GetName() << ": " << h->GetEntries() << ", " << h->GetEntries()*_data.mcNorm[0] << std::endl; 


}

//**************************************************
void NumuDataMcCompTools::draw_graph(TGraphAsymmErrors* h, int lc, int lw, char* opt){
//**************************************************

    h->SetLineColor(lc);
    h->SetFillColor(lc);
    h->SetMarkerColor(lc);
    h->SetLineWidth(lw);
    h->SetMarkerStyle(8);
    h->Draw(opt);

}
//**************************************************
void  NumuDataMcCompTools::draw_var( TH1F *mup1,int icut,string *var,string *acut,string add_cut,  int dim, int min,int itrkCut,bool isMC){ 
//**************************************************
    int _dim=dim;
    if(dim==-1)_dim=1;
    //when dim=-1 we want to consider all the true vertices making a visible track ??????
    //when dim>=0 we consider all the true vertices that corresponds to the visible track from ntracks=min to dim ?????
   
    int nbins = mup1->GetNbinsX();
    double xmin = mup1->GetXaxis()->GetXmin();
    double xmax = mup1->GetXaxis()->GetXmax();
    TH1F *mup = new TH1F("mup"," ",nbins,xmin,xmax);

    std::stringstream potWeight0;
    //	potWeight0 << _data.mcNorm[0];
    potWeight0 << 1;
    std::string potWeight=potWeight0.str();

    for(int it=min;it<_dim;it++){
        string cut;
        string tmpcut=GetCut(icut,acut[it],itrkCut);
        string tmpcut2=add_cut;
        if(isMC){
            cut="(  "+tmpcut+tmpcut2+")*sTrueEnuWeight[0]*"+potWeight;
        }
        else
            cut="(  "+tmpcut+add_cut+")";
        _numutree->Project("mup",var[it].c_str(),cut.c_str());
        mup1->Add(mup);
    }
    std::cout << mup1->GetName() << std::endl;
  

    delete mup;
  
  
}


//**************************************************
void NumuDataMcCompTools::draw_var( int icut, string *var, char* title, int nbins, double xmin, double xmax, double ymax, string *acut,int dim , int min, int itrkCut){
//**************************************************

    _c1 = new TCanvas("c1","",1);
    _c1->SetFillColor(0);
    _c1->cd();
  
    double yfactor=1.;

    for (int i=0;i<5;i++)
        if (_h[i]) delete _h[i]; 

    double ndata=0;
    TH1F * mup[5];
    TH1F * mup0[NMC0MAX];
    for (int i=0;i<5;i++)mup[i]=NULL;
    for (int i=0;i<NMC0MAX;i++)mup0[i]=NULL;
      
    //=================================================
    //---- Init + Fill + Draw data ----
    //=================================================
    if (_draw_data){
        _fdata->cd();
        _numutree=(TTree*) _fdata->Get("numutree");
        mup[4] = new TH1F("mup4",title,nbins,xmin,xmax);
        draw_var(mup[4],icut,var,acut," ",dim,min,itrkCut,0);
        ndata = mup[4]->GetEntries();
        
    }


    //=================================================
    //---- Init + Fill + Draw MC ----
    //=================================================
    TFile* fff[3];
    char _name[256];
    std::string mc_cut[NMC0MAX];
    std::string mc_cat_name[NMC0MAX];
    int mc_cat_color[NMC0MAX];
    if (_draw_MC){
        //=============================================
        //---- Initialize + fill MC histos
        //=============================================
        for (int i=0;i<_nmc;i++){
            sprintf(_name,"mup%d",i);
            fff[i]=new TFile(_data.mcFile[i].c_str(),"read");
            fff[i]->cd();
            mup[i] = new TH1F(_name,title,nbins,xmin,xmax);
            _numutree=(TTree*) fff[i]->Get("numutree");
            draw_var(mup[i],icut,var,acut,"",dim,min,itrkCut);
        }
        //=============================================
        //---- Draw MC (components) for NEUT----
        //=============================================
        _fmc0[0]->cd();
        
        if (_nmc0>0){
            DefineMcComp(mc_cut,mc_cat_name,mc_cat_color,itrkCut );
            for (int imc=0;imc<_nmc0;imc++){
                mup0[imc] = new TH1F(mc_cat_name[imc].c_str(),title,nbins,xmin,xmax);
                draw_var(mup0[imc],icut,var,acut,mc_cut[imc],dim,min,itrkCut);
                if (imc>0)
                    mup0[imc]->Add(mup0[imc],mup0[imc-1]);
            }
        }
        cout<<" ending mccomp "<<endl;
        //=============================================
        //---- Normalization ----------
        //=============================================
        for (int i=0;i<_nmc;i++){
            //fill error with sum weight do it only for genie
            // if put this option it always plot with the error bars
            if(i==1)MakeNorm(mup[i],i,ndata,1);
            else MakeNorm(mup[i],i,ndata);
        }
        for (int i=0;i<_nmc0;i++){
            MakeNorm(mup0[i],0,ndata);
        }
        cout<<" ending norm setting "<<endl; 
    }
    //=============================================     
    if (_draw_histo){
        //===================================================
        // drawing histograms: set scale 
        //
        //====================================================
        double ymaxhisto=0;
        if (ymax == -1) {
            for(int i=0;i<5;i++){
                if(mup[i]==NULL)continue;
                int _maxbin= mup[i]->GetMaximumBin();
                double _maxhisto=mup[i]->GetBinContent(_maxbin);
                double _ymaxhisto=_maxhisto+0.5*_maxhisto;
                if(_ymaxhisto>ymaxhisto)
                    ymaxhisto=_ymaxhisto;
            }
            
            for(int i=0;i<5;i++){
                if(mup[i]==NULL)continue;
                if (_logY)
                    mup[i]->GetYaxis()->SetRangeUser(0.1,ymaxhisto);
                else
                    mup[i]->GetYaxis()->SetRangeUser(0,ymaxhisto);
            }
        }
        else{
            for(int i=0;i<5;i++){
                if(mup[i]==NULL)continue;
                if (_logY)
                    mup[i]->GetYaxis()->SetRangeUser(0.1,ymax*yfactor);
                else
                    mup[i]->GetYaxis()->SetRangeUser(0,ymax*yfactor);
            }
        }
        
        //===================================================
        // drawing histograms...
        //====================================================
        gStyle->Reset();
        gStyle->SetOptStat(1000110);
        if (_draw_data){
            mup[4]->SetMarkerColor(1);
            mup[4]->SetMarkerStyle(8);
            mup[4]->SetLineColor(1);
            mup[4]->GetXaxis()->SetTitle(title);
            mup[4]->SetTitle("");
            mup[4]->Draw("p e1");
            draw_histo(mup[0],       1,1,5,1001,"same");
            std::cout << mup[4]->GetName() << ": " << mup[4]->GetEntries() << std::endl;       
        }
        else draw_histo(mup[0],       1,1,5,1001,"");
        if (_nmc0>0){
            for (int imc=_nmc0-1;imc>=0;imc--){
                draw_histo(mup0[imc], mc_cat_color[imc],1,mc_cat_color[imc],1001,"same"); 
            }    
        }    
        if (_nmc>1)
            draw_histo(mup[1],       1,3,4,0,   "same e2");
        if (_nmc>2){
            draw_histo(mup[2],       4,3,4,0,   "same e2");
            draw_histo(mup[3],       1,3,1,1,   "same");
        }
        if (_draw_data)
            mup[4]->Draw("p e1 same");
        
        if(_draw_leg){
            TLegend *leg0 = new TLegend(0.6,0.5,0.8,0.8);
            leg0->SetFillColor(10);  leg0->SetLineColor(0);
            if (_draw_data)
                leg0->AddEntry(mup[4],"data","pl");
            if(_nmc>1)
                leg0->AddEntry(mup[1],"genie","f");
            for(int i=0;i<_nmc0;i++)
                leg0->AddEntry(mup0[i], mc_cat_name[i].c_str(),"l");
            leg0->Draw("same");
        }
       
        _c1->cd();
        if (_logY)
            _c1->SetLogy();
        else
            _c1->SetLogy(false);
        _c1->Draw();
     
        //====================================================================================
        // print histos...
        //====================================================================================
        if (_plot_name!=""){
            _c1->SetName(_plot_name.c_str());
            std::string fname2 = _plotpath+_plot_name+".eps";
            _c1->Print(fname2.c_str());
     
        }
        else{
            char plname[256];
            sprintf(plname,"canv_%d",_ncanvas);
            _c1->SetName(plname);
            _ncanvas++;
        }
     
      //   if ( !_batch)
//             getchar();
        
    }
     
    //====================================================================================
    // save histos to global histos
    //====================================================================================
    if (_save_histos){
        for (int i=0;i<_nmc;i++){
            _h[i] =new TH1F(*(mup[i]));
        }
        if (_draw_data){
            _h[4] =new TH1F(*(mup[4]));
        }
    }
  
    //====================================================================================
    // write histos
    //====================================================================================

    if(_write_in_rootfile)
    {
       //  TString pname(_plot_name);
//         _c1->SetName(pname.Data());

        _outfile->cd();
        _c1->Write();
        mup[0]->Write();
        if(_draw_data)
            mup[4]->Write();
        for(int i=0;i<_nmc0;i++)
            mup0[i]->Write();
           
    }
   
    //====================================================================================
    // reset and delete...
    //====================================================================================
    _plot_name="";
    _save_histos=false;    

    if (_draw_data)
        delete mup[4];

    if (_draw_MC){
        for (int i=0;i<_nmc;i++){    
            delete mup[i];
        }
        for (int i=0;i<_nmc0;i++){    
            delete mup0[i];
        }
    }
    _draw_histo=true;

  
}


//**************************************************
void  NumuDataMcCompTools::draw_var_ratio(int icut1, int icut2,string* var, char* title, int nbins, double xmin, double xmax, double ymax, std::string *acut, int dim, int min,int itrkCut){
//**************************************************
    _c1=new TCanvas("c1","",1);;
    _c1->SetFillColor(0);
    _c1->cd();
    std::string plot_name = _plot_name;
    _plot_name = "";


    TH1F* h1[5];
    TH1F* h2[5];
    TGraphAsymmErrors* gratio[5];

    _save_histos=true;
    _draw_histo=false;
  
    draw_var(icut1,var,title,nbins, xmin,xmax,ymax,acut,dim,min,itrkCut);//cut1
    for (int i=0;i<2;i++)
        h1[i] = new TH1F(*(_h[i]));

    h1[4] = new TH1F(*(_h[4]));

    _draw_histo=false;
    _save_histos=true;
    draw_var(icut2,var,title,nbins, xmin,xmax,ymax,acut,dim,min,itrkCut);//cut2
    for (int i=0;i<2;i++)
        h2[i] = new TH1F(*(_h[i]));
    
    h2[4] = new TH1F(*(_h[4]));

    for (int i=0;i<2;i++){
        gratio[i] = new TGraphAsymmErrors(nbins);
        gratio[i]->BayesDivide(h2[i],h1[i]);
    }
    gratio[4] = new TGraphAsymmErrors(nbins);
    gratio[4]->BayesDivide(h2[4],h1[4]);

    gratio[4]->GetYaxis()->SetRangeUser(0,1.1);
    gratio[4]->SetTitle(title);
  
    draw_graph(gratio[4],       1,1,   "A p e1");
    // line color , line width , fill color, fill style, options    

    int color[2]={2,4};
    for (int i=0;i<_nmc;i++)
        draw_graph(gratio[i],       color[i],1,   "p e1");

    gStyle->Reset();
    gStyle->SetOptStat(1000111);

    _c1->SetFillColor(0);
    _c1->Draw();
  
    if (plot_name!=""){
        std::string fname2 = _plotpath+plot_name+".eps";
        _c1->Print(fname2.c_str());
    }
    _plot_name="";


    
    //getchar();


}
//**************************************************
void NumuDataMcCompTools::draw_var_DataMC_ratio(int icut,string* var, char* title,int nbins, double xmin, double xmax,  double ymax, string *acut,int dim, int min, int itrkCut,int legPos){
//**************************************************
    _c1=new TCanvas("c1","",1);;
    _c1->SetFillColor(0);
    _c1->cd();
    std::string plot_name = _plot_name;
    _plot_name = "";
  

    TH1F* h[3];
    TH1F* he[3];
    string hname[3]={"h0","h1","data"};
    TFile* fff[3];
    bool mcdata[3]={true,true,false};
    for(int i=0;i<NMCMAX+1;i++){
        if(i<NMCMAX)  fff[i]=new TFile(_data.mcFile[i].c_str(),"read");
        else  fff[i]=new TFile(_data.dataFile.c_str(),"read");
        fff[i]->cd();
        _numutree=(TTree*) fff[i]->Get("numutree");
        h[i]= new TH1F(hname[i].c_str(),title,nbins,xmin,xmax);
        draw_var(h[i],icut,var,acut,"",dim,min,itrkCut,mcdata[i]);
    }
   
    he[0]=new TH1F(*h[0]);
    he[1]=new TH1F(*h[1]);
    double ndata=h[2]->GetEntries();
    //==========================================
    //---- Normalization ----------
    //==========================================
    for (int i=0;i<_nmc;i++){
        MakeNorm(h[i],i,ndata);
        MakeNorm(he[i],i,ndata,1);
    }
    cout<<" ending norm setting "<<endl; 
    //==========================================
    //---- Initialize ratios ----------
    //==========================================
    TH1F* ratio[3];
    TH1F* ratioe[3];
    char _title[256];
    char _name[256];
    char _namee[256];
    for (int i=0;i<3;i++){
        if(i==0){
            sprintf(_title," %s (data/NEUT) ",title);
            sprintf(_name,"r_DataNeut");
            sprintf(_namee,"r_DataNeut_cp");
        }
        else if(i==1 && _nmc>1){
            sprintf(_title," %s (data/GENIE) ",title);
            sprintf(_name,"r_DataGenie");
            sprintf(_namee,"r_DataGenie_cp");
        }
        else if(i==2 && _nmc>1){
            sprintf(_title," %s (NEUT/GENIE) ",title);
            sprintf(_name,"r_NeutGenie");
            sprintf(_namee,"r_NeutGenie_cp");
        }
        ratio[i] = new TH1F(_name,_title,nbins,xmin,xmax);
        ratioe[i] = new TH1F(_namee,_title,nbins,xmin,xmax);
    }
    //==========================================
    //---- set error bars to ratios
    // error bars: f/g : d(f/g)= 1/g df - f/g2 dg = (f/g)*(df/f - dg/g)
    //==========================================
    for(int i=0;i<_nmc;i++){
        ratio[i]->Divide(h[2],h[i]);//data/MC
        ratioe[i]->Divide(h[2],h[i]);//data/MC
        for(int ib=0;ib<nbins;ib++){
            double totf=h[2]->GetBinContent(ib+1);
            double df=h[2]->GetBinError(ib+1);
            double totg=h[i]->GetBinContent(ib+1)*_data.mcNorm[i];
            double dg=sqrt(h[i]->GetBinContent(ib+1)*_data.mcNorm[i]);//=he[i]->GetBinError(ib+1);
            double totfg=ratio[i]->GetBinContent(ib+1);
            double dfg;
            if(totf==0 || totg==0) dfg=0;
            else dfg=totfg*sqrt(pow(df/totf-dg/totg,2));
                
            ratioe[i]->SetBinError(ib+1,dfg);
        }
    }
    if(_nmc>1){
        ratio[2]->Divide(h[0],h[1]);
        ratioe[2]->Divide(h[0],h[1]);
        for(int ib=0;ib<nbins;ib++){
            double totf=h[0]->GetBinContent(ib+1)*_data.mcNorm[0];
            double df=sqrt(h[0]->GetBinContent(ib+1)*_data.mcNorm[0]);//=he[i]->GetBinError(ib+1);
            double totg=h[1]->GetBinContent(ib+1)*_data.mcNorm[1];
            double dg=sqrt(h[1]->GetBinContent(ib+1)*_data.mcNorm[1]);//=he[i]->GetBinError(ib+1);
            double totfg=ratio[2]->GetBinContent(ib+1);
            double dfg;
            if(totf==0 || totg==0) dfg=0;
            else dfg=totfg*sqrt(pow(df/totf-dg/totg,2));

            ratioe[2]->SetBinError(ib+1,dfg);

        }
    }
    
   
    delete h[0];
    delete h[1];
    delete he[0];
    delete he[1];
    delete h[2];
    //================================================================
    // draw ratios
    //// line color , line width , fill color, fill style, options
    //================================================================
    _c1->cd();
    gStyle->Reset();
    gStyle->SetOptStat(1000111);
    if(_nmc>1){
        ratio[2]->GetYaxis()->SetRangeUser(0,5);
        ratio[2]->Draw();
        draw_histo(ratio[2],7,1,4,1001,"");
        draw_histo(ratioe[2],7,1,4,1001,"same e1");
        draw_histo(ratioe[0],2,1,0,0,"same e1");
        draw_histo(ratioe[1],1,3,0,0,"same e2 ");
     }
    else{
        draw_histo(ratioe[0],2,1,0,0,"e1");
    }
    if(_draw_leg){
        TLegend *leg0;
        if(legPos==0)leg0 = new TLegend(0.6,0.5,0.8,0.8);
        if(legPos==1)leg0 = new TLegend(0.4,0.5,0.6,0.8);
        if(legPos==2)leg0 = new TLegend(0.2,0.5,0.4,0.8);
           
        leg0->SetFillColor(10);  leg0->SetLineColor(0);
        if (_draw_data)
            leg0->AddEntry(ratioe[0],"data/Neut","l");
        if(_nmc>1){
            leg0->AddEntry(ratioe[1],"data/Genie","l");
            leg0->AddEntry(ratio[2],"Neut/Genie","f");
        }
        
        
        leg0->Draw("same");
    }

   

    
    if (plot_name!=""){
        std::string fname2 = _plotpath+plot_name+".eps";
        _c1->Print(fname2.c_str());
    }
    
    _plot_name="";
   
    for(int i=0;i<3;i++){
        delete ratio[i];
        delete ratioe[i];
    }


}
//**************************************************
void  NumuDataMcCompTools::draw_var_2d( TH2F *mup1_2d,int icut,string *var,string *acut,string add_cut,  int dim, int min, int itrkCut,bool isMC){ 
//**************************************************

    int _dim=dim;
    if(dim==-1)_dim=1;
    int nxbins = mup1_2d->GetNbinsX();
    double xmin = mup1_2d->GetXaxis()->GetXmin();
    double xmax = mup1_2d->GetXaxis()->GetXmax();
    int nybins = mup1_2d->GetNbinsY();
    double ymin = mup1_2d->GetYaxis()->GetXmin();
    double ymax = mup1_2d->GetYaxis()->GetXmax();
  
  
    TH2F *mup1a_2d = new TH2F("mup1a_2d","mup1",nxbins,xmin,xmax,nybins,ymin,ymax);
    std::stringstream potWeight0;
  
    potWeight0 << 1;
    std::string potWeight=potWeight0.str();
    for(int it=min;it<_dim;it++){
      string cut;

      string tmpcut=add_cut;
     
      string tmpcut2=GetCut(icut,acut[it],itrkCut);
    
      if(isMC)
          cut="(  "+tmpcut2+tmpcut+")*sTrueEnuWeight[0]*"+potWeight;
      else
          cut="(  "+tmpcut2+tmpcut+")";

   
      cout<<_numutree->Project("mup1a_2d",var[it].c_str(),cut.c_str())<<endl;
      mup1_2d->Add(mup1a_2d);
    }
    delete mup1a_2d; 
 
   
  
}


//**************************************************
void  NumuDataMcCompTools::draw_histo_2d(TH2F* h,  char* opt){
//**************************************************

  h->Draw(opt);

}
//**************************************************
                   
void NumuDataMcCompTools::draw_var_2d( int icut, string *var, char* title, int nxbins, double xmin, double xmax,  int nybins, double ymin, double ymax, double zmax, string *acut,int dim, int min, int itrkCut){
//**************************************************
    _c1=new TCanvas("c1","",1);;
    _c1->SetFillColor(0);
    _c1->cd();
    double yfactor =1.;
    
   
    for (int i=0;i<5;i++)
        if (_h2[i]) delete _h2[i]; 

    char _title[256];
    char _name[256];
    double ndata=0;
    TH2F * mup[5];
    TH2F * mup0[NMC0MAX];
    for (int i=0;i<5;i++)mup[i]=NULL;
    for (int i=0;i<NMC0MAX;i++)mup0[i]=NULL;
  
    //=============================================
    // Initialize + fill data histograms
    //=============================================
    if (_draw_data){
        _fdata->cd();
        _numutree=(TTree*) _fdata->Get("numutree");
        sprintf(_title," %s (Data) ",title);
        mup[4] = new TH2F("mup4",_title,nxbins,xmin,xmax,nybins,ymin,ymax);
        draw_var_2d(mup[4],icut,var,acut,"",dim,min,itrkCut,0);
        ndata = mup[4]->GetEntries();
    }
    TFile* fff[3];
    if (_draw_MC){
        //=============================================
        //---- Initialize + fill MC histos
        //=============================================
        for (int i=0;i<_nmc;i++){
            sprintf(_name,"mup%d",i);
            if(i==0)  sprintf(_title," %s (NEUT) ",title);
            else if(i==1)  sprintf(_title," %s (GENIE) ",title);
            fff[i]=new TFile(_data.mcFile[i].c_str(),"read");
            fff[i]->cd();
            _numutree=(TTree*) fff[i]->Get("numutree");
            mup[i] = new TH2F(_name,_title,nxbins,xmin,xmax,nybins,ymin,ymax);
            draw_var_2d(mup[i],icut,var,acut," ",dim,min,itrkCut);
        }
        //=============================================
        //---- Define MC (components) for NEUT only
        //=============================================
        _fmc0[0]->cd();
        std::string mc_cut[NMC0MAX];
        std::string mc_cat_name[NMC0MAX];
        int mc_cat_color[NMC0MAX];
        if (_nmc0>0){
            DefineMcComp(mc_cut,mc_cat_name,mc_cat_color,itrkCut );
            for (int imc=0;imc<_nmc0;imc++){
                mup0[imc] = new TH2F(mc_cat_name[imc].c_str(),_title,nxbins,xmin,xmax,nybins,ymin,ymax);
                cout<<" imc "<<imc<<endl;
                draw_var_2d(mup0[imc],icut,var,acut,mc_cut[imc],dim,min,itrkCut);
                if (imc>0)
                    mup0[imc]->Add(mup0[imc],mup0[0]);
            }
        }
        cout<<" ending mccomp "<<endl;

       //=============================================
        //------- Normalization ---------
        //=============================================
        for (int i=0;i<_nmc;i++){
            MakeNorm_2d(mup[i],i,ndata);
        }
        for (int i=0;i<_nmc0;i++){
            MakeNorm_2d(mup0[i],0,ndata);
        }
        cout<<" ending normalisation ....  "<<endl;

    }
    //=================================================
    // drawing histograms: take the same scale for all
    //=================================================
    if (_draw_histo){
        if (zmax == -1) {
            double zmaxhisto=0;
            for (int i=0;i<5;i++){
                if(mup[i]==NULL) continue;
                int _maxbin= mup[i]->GetMaximumBin();
                double _maxhisto=mup[i]->GetBinContent(_maxbin);
                double _zmaxhisto=_maxhisto+0.5*_maxhisto;
                cout<<" i "<<i<<" "<<_zmaxhisto<<endl;
                if(_zmaxhisto>zmaxhisto)
                    zmaxhisto=_zmaxhisto;
            }
            
            for (int i=0;i<5;i++){
                if(mup[i]==NULL) continue;
                if (_logY)
                    mup[i]->GetZaxis()->SetRangeUser(0.1,zmaxhisto);
                else
                    mup[i]->GetZaxis()->SetRangeUser(0,zmaxhisto);
            }
        }
        else{
            for (int i=0;i<5;i++){
                if(mup[i]==NULL) continue;
                if (_logY)
                    mup[i]->GetZaxis()->SetRangeUser(0.1,zmax*yfactor);
                else
                    mup[i]->GetZaxis()->SetRangeUser(0,zmax*yfactor);
            }
        }

        //=================================================
        // drawing histograms:....
        //=================================================
        if(_draw_data && _draw_MC )  _c1->Divide(1,3);
        else if(!_draw_data && _draw_MC )  _c1->Divide(1,2);

        gStyle->Reset();
        gStyle->SetPalette(1);
        gStyle->SetStatX(0.7);
        gStyle->SetStatY(0.7);
        gStyle->SetStatW(0.1);
        gStyle->SetStatH(0.4);
        gStyle->SetOptStat(110);
        
        if(_draw_MC){
            for (int i=0;i<_nmc;i++){
                _c1->cd(i+1);
                draw_histo_2d(mup[i],"colz");
            }
        }
        if (_draw_data){
            if(!_draw_MC) _c1->cd(1);      
            else _c1->cd(_nmc+1);      
            draw_histo_2d(mup[4],"colz");
        }
        _c1->Draw();
        //===============================================================
        // print histograms
        //===============================================================
        if (_plot_name!=""){
            _c1->SetName(_plot_name.c_str());
            std::string fname2 = _plotpath+_plot_name+".eps";
            _c1->Print(fname2.c_str());
        }
        else{
            char plname[256];
            sprintf(plname,"canv_%d",_ncanvas);
            _c1->SetName(plname);
            _ncanvas++;
        }
    }

    //===============================================================
    // save local hist to public histograms
    //===============================================================
    if (_save_histos){
        for (int i=0;i<_nmc;i++)
            _h2[i] = new TH2F(*(mup[i]));
        if (_draw_data)
            _h2[4] = new TH2F(*(mup[4]));
    }
    
    //===============================================================
    // save all hist to a rootfile
    //===============================================================
    if(_write_in_rootfile)
    {
        _outfile->cd();
        _c1->Write();
        mup[0]->Write();
        if(_draw_data)
            mup[4]->Write();
        for(int i=0;i<_nmc0;i++)
            mup0[i]->Write();
    }
 

    //===============================================================
    // delete and reset global variables
    //===============================================================
    _plot_name="";
    _save_histos=false;
    if (_draw_data)
        delete mup[4];

    if (_draw_MC){
        for (int i=0;i<_nmc;i++){    
            delete mup[i];
        }
        for (int i=0;i<_nmc0;i++){    
            delete mup0[i];
        }
    }
}
//**************************************************
void NumuDataMcCompTools::draw_var_DataMC_ratio_2d(int icut1,string* var, char* title,int nxbins, double xmin, double xmax,  int nybins, double ymin, double ymax, double zmax, string *acut,int dim, int min, int itrkCut){
//**************************************************
    _c1=new TCanvas("c1","",1);;
    _c1->SetFillColor(0);
    _c1->cd();
    std::string plot_name = _plot_name;
    _plot_name = "";
  
    double yfactor=1;
    
    TH2F* ratio[3];
//     _save_histos=true;
//     _draw_histo=false;
//     draw_var_2d(icut1,var,title,nxbins,xmin,xmax,nybins,ymin,ymax,zmax,acut,dim,min,itrkCut);//cut1
    
    TH2F* h[3];
    string hname[3]={"h0","h1","data"};
    TFile* fff[3];
    bool mcdata[3]={true,true,false};
    for(int i=0;i<NMCMAX+1;i++){
        if(i<NMCMAX)  fff[i]=new TFile(_data.mcFile[i].c_str(),"read");
        else  fff[i]=new TFile(_data.dataFile.c_str(),"read");
        fff[i]->cd();
        _numutree=(TTree*) fff[i]->Get("numutree");
        h[i]= new TH2F(hname[i].c_str(),title,nxbins,xmin,xmax,nybins,ymin,ymax);
        draw_var_2d(h[i],icut1,var,acut,"",dim,min,itrkCut,mcdata[i]);
    }
    double ndata=h[2]->GetEntries();
    //---- Normalization ----------
    for (int i=0;i<_nmc;i++){
        MakeNorm_2d(h[i],i,ndata);
    }
    cout<<" ending norm setting "<<endl; 
    //==========================================

    char _title[256];
    char _name[256];
    for (int i=0;i<3;i++){
        if(i==0){
            sprintf(_title," %s (data/NEUT) ",title);
            sprintf(_name,"r_DataNeut");
        }
        else if(i==1 && _nmc>1){
            sprintf(_title," %s (data/GENIE) ",title);
            sprintf(_name,"r_DataGenie");
        }
        else if(i==2 && _nmc>1){
            sprintf(_title," %s (NEUT/GENIE) ",title);
            sprintf(_name,"r_NeutGenie");
        }
        ratio[i] = new TH2F(_name,_title,nxbins,xmin,xmax,nybins,ymin,ymax);
       
    }
 
    gStyle->SetPalette(1);
    gStyle->SetStatX(0.7);
    gStyle->SetStatY(0.7);
    gStyle->SetStatW(0.1);
    gStyle->SetStatH(0.4);
    gStyle->SetOptStat(110);
    for (int i=0;i<_nmc;i++)  ratio[i]->Divide(h[2],h[i]);//data/MC
    ratio[2]->Divide(h[0],h[1]);//neut/genie

    for (int i=0;i<_nmc+1;i++) ratio[i]->GetZaxis()->SetRangeUser(0,2);

    
    _c1=new TCanvas("c1","",1);
    _c1->SetFillColor(0);
    _c1->Divide(1,3);
    _c1->cd(1);
    ratio[0]->Draw("colz");
    _c1->cd(2);
    ratio[1]->Draw("colz");
  _c1->cd(3);
  ratio[NMCMAX]->Draw("colz");

  if (plot_name!=""){
    std::string fname2 = _plotpath+plot_name+".eps";
    _c1->Print(fname2.c_str());
  }

  _plot_name="";


  
  gStyle->SetOptStat(110);
  
  //getchar();
  delete ratio[0];
  delete ratio[1];
  delete ratio[2];

}
//**************************************************
void NumuDataMcCompTools::draw_var_ratio_2d(int icut1, int icut2,string* var, char* title,int nxbins, double xmin, double xmax,  int nybins, double ymin, double ymax, double zmax, string *acut,int dim, int min, int itrkCut){
//**************************************************

    _c1=new TCanvas("c1","",1);;
    _c1->SetFillColor(0);
    _c1->cd();
  std::string plot_name = _plot_name;
  _plot_name = "";

  double yfactor=1;

  TH2F* h1[5];
  TH2F* h2[5];
  TH2F* ratio[5];
  
  _save_histos=true;
  draw_var_2d(icut1,var,title,nxbins, xmin,xmax,nybins,ymin,ymax,zmax,acut,dim,min,itrkCut);//cut1
  for (int i=0;i<2;i++)
    h1[i] = new TH2F(*(_h2[i]));

    h1[4] = new TH2F(*(_h2[4]));
  
  _save_histos=true;
  draw_var_2d(icut2,var,title,nxbins, xmin,xmax,nybins,ymin,ymax,zmax,acut,dim,min,itrkCut);//cut1
  for (int i=0;i<2;i++)
    h2[i] = new TH2F(*(_h2[i]));
    
    h2[4] = new TH2F(*(_h2[4]));

  for (int i=0;i<2;i++){
    ratio[i] = new TH2F(*(h2[i])); 
    ratio[i]->Divide(h1[i],h2[i]); 
  }

  ratio[4] = new TH2F(*(h2[4])); 
  ratio[4]->Divide(h1[4],h2[4]); 
  
  _c1->cd(1);
  ratio[4]->Draw("colz");
  _c1->cd(2);
  ratio[0]->Draw("colz");
  
  if (plot_name!=""){
    std::string fname2 = _plotpath+plot_name+".eps";
    _c1->Print(fname2.c_str());
  }

  _plot_name="";


  
  gStyle->SetOptStat(110);
  
  //getchar();


}
double NumuDataMcCompTools::GetMcPOT(string inputname){
    TString fileName(inputname);
    ifstream inputFile(fileName.Data(), ios::in);
    if (!inputFile) {
        cerr << "Cannot open input file '" << fileName.Data() << "'. Exit!" << endl;
        return 0;
    }
    string inputString;
    double totfiles=0;
    double totpot=0;
    while (inputFile >> inputString) {
        totfiles++;
    }
    totpot=totfiles*5e17;
    return totpot;
}

double NumuDataMcCompTools::GetDataPOT(string inputname){
//    cout<<_data.dataFileList<<endl;

    FILE *fpot = fopen(_data.dataFileList.c_str(),"r");
    // FILE *fpot = fopen( "/neutrino/data2/t2k/minitree_geneva/datafiles_pot_valencia.txt","r");
    //long int potvalue[100000000];
    double potvalue[100000000];
//   double runvalue[100000000];
    if( fpot ){
        int run,srun; double lpot; int nscan;
        do {
            nscan = fscanf(fpot,"%d %d %lf ",&run,&srun,&lpot);
            int indx = run*10000+srun;
            potvalue[indx]=lpot;
            //cout<<run <<" sub "<<srun<<" pot "<<potvalue[indx]<<endl;
        }
        while( nscan == 3 );
    }
    else
        std::cout << " error opening file " << std::endl;
    
    double totpot = 0.;
 
    TString fileName(inputname);
    ifstream inputFile(fileName.Data(), ios::in);
    if (!inputFile) {
        cerr << "Cannot open input file '" << fileName.Data() << "'. Exit!" << endl;
        return 0;
    }

    string inputString;   
    while (inputFile >> inputString) {
        size_t found = inputString.find("numuflattree"); bool skipsubrun = false;
        if( found != string::npos) {
            char strrun[10]; char strsubrun[10];
            size_t foundrun = inputString.find("_",found);
            inputString.copy(strrun,4,foundrun+1);
            int run = atoi(strrun);
          
            size_t foundsubrun = inputString.find("-",foundrun);
            inputString.copy(strsubrun,4,foundsubrun+1);
            int subrun = atoi(strsubrun);
            int index = run*10000+subrun;
//            cout<<index<<" pot "<< potvalue[index]<<" tot "<<totpot<<endl;
            totpot += potvalue[index];
            //       cout<<totpot<<endl;
            if( potvalue[index] < 100 ) {
                skipsubrun = true;
            }
         
            potvalue[index] = -10.0;
        }
     }

    return totpot;
}

void NumuDataMcCompTools::SetNorm(double datapot, double datamc0, double datamc1){

    _data.dataPOT=datapot;
    _data.mcNorm[0]=datapot/datamc0;
    _data.mcNorm[1]=datapot/datamc1;
}

void NumuDataMcCompTools::MakeNorm(TH1F *hist,int mcfile,double ndata,int option){
    double nmc= hist->GetEntries();  
    double norm=nmc;
    
    if(option==1) hist->Sumw2();

    if (_run_3132){
        norm = nmc*_data.mcNorm_3132[mcfile];
        if (_draw_data)norm = ndata;
        if (_normalize_area) hist->SetNormFactor(norm);
        else hist->Scale(_data.mcNorm_3132[mcfile]);
    }
    else if (_run_3334){
        norm = nmc*_data.mcNorm_3334[mcfile];
        if (_draw_data)norm = ndata;
        if (_normalize_area) hist->SetNormFactor(norm);
        else hist->Scale(_data.mcNorm_3334[mcfile]);
    }
    else{
        norm = nmc*_data.mcNorm[mcfile];
        if (_draw_data)norm = ndata;
        if (_normalize_area) hist->SetNormFactor(norm);
        else hist->Scale(_data.mcNorm[mcfile]);
    }
}

void NumuDataMcCompTools::MakeNorm_2d(TH2F *hist,int mcfile,double ndata,int option){

   //---- Normalization ----------
    double nmc=0;
    double norm=0;
    nmc = hist->GetEntries();

    if(option==1) hist->Sumw2();
    
    if (_run_3132){
//        norm = nmc*_data.mcNorm_3132[mcfile];
//        if (_draw_data)norm = ndata;
        //  hist->SetNormFactor(norm);
        hist->Scale(_data.mcNorm_3132[mcfile]);
    }
    else if (_run_3334){
//        norm = nmc*_data.mcNorm_3334[mcfile];
//        if (_draw_data)norm = ndata;
//        hist->SetNormFactor(norm);
        hist->Scale(_data.mcNorm_3334[mcfile]);
    }
    else{
      //   norm = nmc*_data.mcNorm[mcfile];
//         if (_draw_data)norm = ndata;
//        hist->SetNormFactor(norm);
        hist->Scale(_data.mcNorm[mcfile]);
    }
   
}
