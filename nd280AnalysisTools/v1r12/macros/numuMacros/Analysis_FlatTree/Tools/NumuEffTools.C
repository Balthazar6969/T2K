#include "NumuEffTools.h"


NumuEffTools::NumuEffTools(){
    _cutSTART=0;
    _fileSTART=0;
    _NMCFILES=NMCFILES;
    _fillarrays=true;
    _fillgraphs=true;
    _takestepdef=false;
    _trueParam=true;
    _useDir=false;
    _useDir2=false;
    _write=false;
        
    _c1=new TCanvas("c1"," ",1);_c1->SetFillColor(0);
    _c2=new TCanvas("c2"," ",1);_c2->SetFillColor(0);
    _c3=new TCanvas("c3"," ",1);_c3->SetFillColor(0);

    if(_write)
        _myfile = new TFile("effpur.root","update");
    _ffiles[0]=new TFile(_data.mcFile[0].c_str(),"read");
    _ffiles[1]=new TFile(_data.mcFile[1].c_str(),"read");
    _ffiles[2]=new TFile(_data.dataFile.c_str(),"read");
  
}

    
void NumuEffTools::setstep(int dim,double *idstep){
    for(int i=0;i<dim;i++) _dstep[i]=idstep[i];
   
}

double NumuEffTools::getstep(int icut){
    return _dstep[icut];
}



//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void NumuEffTools::FillSelTrueEventsArray( std::string *true_cut,  std::string *seltrue_cut, std::string *sel_cut,int NCUTS2, double xmin,double xmax){

  
    double step=0;
    for (int i=_cutSTART;i<NCUTS2;i++){
        if(_takestepdef)step=xmin+getstep(i);
        else step=xmin+(xmax-xmin)/(double)NCUTS2*i;
             
        std::string scut[3];
        //seltrue if false all sel + selcut i
        scut[0] = seltrue_cut[i];
        // selected if false all sel + selcut i
        scut[1] = sel_cut[i];
        // all true if false all sel
        scut[2] = true_cut[i];

                    
        for (int f=_fileSTART;f<_NMCFILES;f++){
            _ffiles[f]->cd();
            TTree *numutree=(TTree*) _ffiles[f]->Get("numutree");

            for (int j=0;j<3;j++){
                TH1F *h1 = new TH1F("h1","h",100,0,100);
                numutree->Project("h1","std_TrueSpillInfo",scut[j].c_str());
                _seltrueEvents[f][i][j] =(int) h1->GetEntries();
                delete h1;
            }
                
            std::cout <<" cut "<< i << " pos "<<step<< " file " << f
                      <<" seltrue " << _seltrueEvents[f][i][0]
                      <<" true "<<_seltrueEvents[f][i][2]
                      <<" sel " <<_seltrueEvents[f][i][1] ;
            if((double)_seltrueEvents[f][i][2]!=0)
                cout<<" eff " << (double)_seltrueEvents[f][i][0]/(double)_seltrueEvents[f][i][2];
            if((double)_seltrueEvents[f][i][1]!=0)
                cout<< " pur " << (double)_seltrueEvents[f][i][0]/(double)_seltrueEvents[f][i][1]<<endl;
                

        }
      
    }

        
}
    
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void NumuEffTools::FillEffPurGraphs(int NCUTS2,double xmin,double xmax){
    double _posmax=xmax+5;
    double _posmin=xmin;
    double step=0;
       
    for (int f=_fileSTART;f<_NMCFILES;f++){
        cout<<_posmax<<endl;
        TH1F *hsel = new TH1F("hsel","eff",NCUTS2*10+1,_posmin,(double)_posmax);
        TH1F *htrue = new TH1F("htrue","eff",NCUTS2*10+1,_posmin,(double)_posmax);
        TH1F *hseltrue = new TH1F("hseltrue","eff",NCUTS2*10+1,_posmin,(double)_posmax);
          
        for (int i=_cutSTART;i<NCUTS2;i++){
            if(_takestepdef) step=xmin+getstep(i);
            else step=xmin+(xmax-xmin)/(double)NCUTS2*i;
            hseltrue->Fill(step,(double)_seltrueEvents[f][i][0]);
            htrue->Fill(step,(double)_seltrueEvents[f][i][2]);
            hsel->Fill(step,(double)_seltrueEvents[f][i][1]);
        }
            
        _geff[f] = new TGraphAsymmErrors(NCUTS2*10+1);
        _geff[f]->BayesDivide(hseltrue,htrue);
        _gpur[f] = new TGraphAsymmErrors(NCUTS2*10+1);
        _gpur[f]->BayesDivide(hseltrue,hsel);

             

        delete hsel;
        delete htrue;
        delete hseltrue;
            
            

        if(f==0){
            _geff[f]->SetLineColor(1);
            _geff[f]->SetLineStyle(1);
            _geff[f]->SetMarkerColor(1);
            _geff[f]->SetMarkerStyle(8);

            _gpur[f]->SetLineColor(2);
            _gpur[f]->SetMarkerColor(2);
            _gpur[f]->SetMarkerStyle(8);
        }
        else if(f==1){
            _geff[f]->SetLineColor(1);
            _geff[f]->SetLineStyle(2);
            _geff[f]->SetFillStyle(0);
            _geff[f]->SetMarkerColor(1);
            _geff[f]->SetMarkerStyle(8);

            _gpur[f]->SetLineColor(2);
            _gpur[f]->SetLineStyle(2);
            _gpur[f]->SetFillStyle(0);
            _gpur[f]->SetMarkerColor(2);
            _gpur[f]->SetMarkerStyle(8);
        }
    }
}
    
   
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void NumuEffTools::FillContGraphs(int NCUTS2,double xmin,double xmax,int color){
    double _posmax=xmax+5;
    double _posmin=xmin;
    double step=0;
     
    for (int f=_fileSTART;f<_NMCFILES;f++){
        TH1F *hsel = new TH1F("hsel","eff",NCUTS2*10+1,_posmin,(double)_posmax);
        TH1F *htrue = new TH1F("htrue","eff",NCUTS2*10+1,_posmin,(double)_posmax);
        TH1F *hseltrue = new TH1F("hseltrue","eff",NCUTS2*10+1,_posmin,(double)_posmax);
          
        for (int i=_cutSTART;i<NCUTS2;i++){
            if(_takestepdef) step=xmin+getstep(i);
            else step=xmin+(xmax-xmin)/(double)NCUTS2*i;
            hseltrue->Fill(step,(double)_seltrueEvents[f][i][0]);
            htrue->Fill(step,(double)_seltrueEvents[f][i][2]);
            hsel->Fill(step,(double)_seltrueEvents[f][i][1]);
        }

        _gcont[f] = new TGraphAsymmErrors(NCUTS2*10+1);
        _gcont[f]->BayesDivide(hseltrue,hsel);
            

        delete hsel;
        delete htrue;
        delete hseltrue;
            
            

        if(f==0){
            _gcont[f]->SetLineColor(color);
            _gcont[f]->SetMarkerColor(color);
            _gcont[f]->SetMarkerStyle(8);
        }
        else if(f==1){
            _gcont[f]->SetLineColor(color);
            _gcont[f]->SetLineStyle(2);
            _gcont[f]->SetFillStyle(0);
            _gcont[f]->SetMarkerColor(color);
            _gcont[f]->SetMarkerStyle(8);
        }
    }
}
    
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void NumuEffTools::DrawEffPur( std::string *true_cut,  std::string *seltrue_cut, std::string *sel_cut,int NCUTS2,std::string name,string title,double xmin,double xmax,double ymin,double ymax,string xAxisLabel, int legPos){ 

      
    if(_fillarrays)
        FillSelTrueEventsArray(true_cut,seltrue_cut,sel_cut,NCUTS2,xmin,xmax);
   
    if(_fillgraphs)
        FillEffPurGraphs(NCUTS2,xmin,xmax);
    //---------- Draw Plot ----------
    gStyle->SetOptStat(0);
    _c1->cd();
        
    if(ymax!=0)
        _geff[0]->GetYaxis()->SetRangeUser(ymin,ymax);
        
    if(xmax!=0)
        _geff[0]->GetXaxis()->SetRangeUser(xmin-1,xmax+1);

    _geff[0]->SetTitle(title.c_str());
    _geff[0]->GetXaxis()->SetTitle(xAxisLabel.c_str());
    _c1->cd();
        
    _geff[0]->Draw("A l p e1");
    _gpur[0]->Draw("l p e1");
    if(_NMCFILES==2){
        _geff[1]->Draw("l p e1");
        _gpur[1]->Draw("l p e1");
    }
        
    gStyle->SetOptStat(0);
    gStyle->SetErrorX(0);
    _c1->SetGridy();
       
    TLegend *leg;
    if (legPos==1) leg = new TLegend(0.55,0.73,0.75,0.87);  //nuE CC
    else if (legPos==2) leg = new TLegend(0.15,0.13,0.35,0.27);  //Q2 CC
    else if (legPos==3) leg = new TLegend(0.35,0.18,0.55,0.33);  //
    else leg = new TLegend(0.25,0.73,0.45,0.87);
        
    leg->SetFillColor(10);  
    leg->SetLineColor(0);
    leg->AddEntry(_geff[0],"efficiency/NEUT","PEL");
    if(_NMCFILES==2)
        leg->AddEntry(_geff[1],"efficiency/GENIE","P");
    leg->AddEntry(_gpur[0],"purity/NEUT","PEL");
    if(_NMCFILES==2)
        leg->AddEntry(_gpur[1],"purity/GENIE","P");
    leg->Draw("l");
        
    string epsname;
    epsname="plots/"+name+".eps";
    _c1->Print(epsname.c_str());
    if(_write){
        _myfile->cd();
        _c1->Write();
    }
        
}
    
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void NumuEffTools::DrawEff( std::string *true_cut,  std::string *seltrue_cut, std::string *sel_cut,int NCUTS2,std::string name,string title,double xmin,double xmax,double ymin,double ymax,string xAxisLabel, int legPos,string drawOption){ 

      
    if(_fillarrays)
        FillSelTrueEventsArray(true_cut,seltrue_cut,sel_cut,NCUTS2,xmin,xmax);
    if(_fillgraphs)
        FillEffPurGraphs(NCUTS2,xmin,xmax);
    //---------- Draw Plot ----------
    gStyle->SetOptStat(0);
    _c1->cd();
        
    if(ymax!=0)
        _geff[0]->GetYaxis()->SetRangeUser(ymin,ymax);
    if(xmax!=0)
        _geff[0]->GetXaxis()->SetRangeUser(xmin-0.5,xmax+0.5);
    _geff[0]->SetTitle(title.c_str());
    _geff[0]->GetXaxis()->SetTitle(xAxisLabel.c_str());
    _c1->cd();

    if(drawOption=="same")
        _geff[0]->Draw("l p e1");
    else
        _geff[0]->Draw("A l p e1");
    if(_NMCFILES>1)
        _geff[1]->Draw("l p e2");
                
    gStyle->SetOptStat(0);
    gStyle->SetErrorX(0);
    _c1->SetGridy();
       
    TLegend *leg;
    if (legPos==1) leg = new TLegend(0.55,0.73,0.75,0.87);  //nuE CC
    else if (legPos==2) leg = new TLegend(0.15,0.13,0.35,0.27);  //Q2 CC
    else if (legPos==3) leg = new TLegend(0.35,0.18,0.55,0.33);  //
    else leg = new TLegend(0.25,0.73,0.45,0.87);
        
    leg->SetFillColor(10);  
    leg->SetLineColor(0);
    leg->AddEntry(_geff[0],"efficiency/NEUT","PEL");
    if(_NMCFILES>1)
        leg->AddEntry(_geff[1],"efficiency/GENIE","P");
    leg->Draw("l");
        
    string epsname;
    epsname="plots/"+name+".eps";
    _c1->Print(epsname.c_str());
    if(_write){
        _myfile->cd();
        _c1->Write();
    }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void NumuEffTools::DrawPur( std::string *true_cut,  std::string *seltrue_cut, std::string *sel_cut,int NCUTS2,std::string name,string title,double xmin,double xmax,double ymin,double ymax,string xAxisLabel, int legPos,string drawOption){ 

      
    if(_fillarrays)
        FillSelTrueEventsArray(true_cut,seltrue_cut,sel_cut,NCUTS2,xmin,xmax);
    if(_fillgraphs)
        FillEffPurGraphs(NCUTS2,xmin,xmax);
    //---------- Draw Plot ----------
    gStyle->SetOptStat(0);
    _c1->cd();
        
    if(ymax!=0)
        _gpur[0]->GetYaxis()->SetRangeUser(ymin,ymax);
    if(xmax!=0)
        _gpur[0]->GetXaxis()->SetRangeUser(xmin-0.5,xmax+0.5);
    _gpur[0]->GetXaxis()->SetTitle(xAxisLabel.c_str());
    _gpur[0]->SetTitle(title.c_str());
    _c1->cd();

    if(drawOption=="same")
        _gpur[0]->Draw("l p e1");
    else
        _gpur[0]->Draw("A l p e1");
    if(_NMCFILES>1)
        _gpur[1]->Draw("l p e2");
        
        
    gStyle->SetOptStat(0);
    gStyle->SetErrorX(0);
    _c1->SetGridy();
       
    TLegend *leg;
    if (legPos==1) leg = new TLegend(0.55,0.73,0.75,0.87);  //nuE CC
    else if (legPos==2) leg = new TLegend(0.15,0.13,0.35,0.27);  //Q2 CC
    else if (legPos==3) leg = new TLegend(0.35,0.18,0.55,0.33);  //
    else leg = new TLegend(0.25,0.73,0.45,0.87);
        
    leg->SetFillColor(10);  
    leg->SetLineColor(0);
    leg->AddEntry(_gpur[0],"purity/NEUT","PEL");
    if(_NMCFILES>1)
        leg->AddEntry(_gpur[1],"purity/GENIE","P");
    leg->Draw("l");
        
    string epsname;
    epsname="plots/"+name+".eps";
    _c1->Print(epsname.c_str());
    if(_write){
        _myfile->cd();
        _c1->Write();
    }
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void  NumuEffTools::DrawCont( std::string *true_cut,  std::string *seltrue_cut, std::string *sel_cut,int NCUTS2,std::string name,string title,double xmin,double xmax,double ymin,double ymax,string xAxisLabel, int legPos,string legname,int color,string drawOption){ 


      
    if(_fillarrays)
        FillSelTrueEventsArray(true_cut,seltrue_cut,sel_cut,NCUTS2,xmin,xmax);
    if(_fillgraphs)
        FillContGraphs(NCUTS2,xmin,xmax,color);
    //---------- Draw Plot ----------
    gStyle->SetOptStat(0);
    _c1->cd();
        
    if(ymax!=0)
        _gcont[0]->GetYaxis()->SetRangeUser(ymin,ymax);
    if(xmax!=0)
        _gcont[0]->GetXaxis()->SetRangeUser(xmin-0.5,xmax+0.5);
    _gcont[0]->GetXaxis()->SetTitle(xAxisLabel.c_str());
    _gcont[0]->SetTitle(title.c_str());
    _c1->cd();

    if(drawOption=="same")
        _gcont[0]->Draw("l p e1");
    else
        _gcont[0]->Draw("A l p e1");
    if(_NMCFILES>1)
        _gcont[1]->Draw("l p e2");
        
        
    gStyle->SetOptStat(0);
    gStyle->SetErrorX(0);
    _c1->SetGridy();
       
    TLegend *leg;
    if (legPos==1) leg = new TLegend(0.55,0.73,0.75,0.87);  //nuE CC
    else if (legPos==2) leg = new TLegend(0.15,0.13,0.35,0.27);  //Q2 CC
    else if (legPos==3) leg = new TLegend(0.35,0.18,0.55,0.33);  //
    else leg = new TLegend(0.25,0.73,0.45,0.87);
        
    leg->SetFillColor(10);  
    leg->SetLineColor(0);
    string _legname_nt=legname+"/NEUT";
    string _legname_gn=legname+"/GENIE";
    leg->AddEntry(_gcont[0],_legname_nt.c_str(),"PEL");
    if(_NMCFILES>1)
        leg->AddEntry(_gcont[1],_legname_gn.c_str(),"P");
    leg->Draw("l");

    string epsname;
    epsname="plots/"+name+".eps";
    _c1->Print(epsname.c_str());
    if(_write){
        _myfile->cd();
        _c1->Write();
    }
    
        
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void  NumuEffTools::FillFracEventsArray( std::string *num_cut,std::string *denom_cut,int NCUTS2, double xmin,double xmax,int NFILES2){
      
    double step=0;
    for (int i=_cutSTART;i<NCUTS2;i++){
        if(_takestepdef)step=xmin+getstep(i);
        else step=xmin+(xmax-xmin)/(double)NCUTS2*i;
               
        std::string scut[2];
        scut[0] = num_cut[i] ;
        scut[1] = denom_cut[i] ;
         
        for (int f=_fileSTART;f<NFILES2;f++){
            _ffiles[f]->cd();
            TTree *numutree=(TTree*) _ffiles[f]->Get("numutree");
            

            for (int j=0;j<2;j++){
                TH1F *h1 = new TH1F("h1","h",100,0,100);
                numutree->Project("h1","std_TrueSpillInfo",scut[j].c_str());
                _fracEvents[f][i][j] = (int) h1->GetEntries();
                delete h1;
            }
                
            std::cout << i <<" pos "<<step<< " " << f;
            if(f==2)
                std::cout<< " num " << _fracEvents[f][i][0] << " denom " << _fracEvents[f][i][1];
            else
                std::cout<< " num " << _fracEvents[f][i][0]*_data.mcNorm[f] << " denom " << _fracEvents[f][i][1]*_data.mcNorm[f];
            if(_fracEvents[f][i][1]!=0)
                std::cout<<" frac  "<< (double)_fracEvents[f][i][0]/(double)_fracEvents[f][i][1]; 
            std::cout<<" "<< std::endl;
        }
    }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void NumuEffTools::FillFracGraphs(int NCUTS2, double xmin,double xmax,int NFILES2,int color){
    FillFracGraphs(_gfrac,NCUTS2,xmin,xmax,NFILES2,color);
}
    
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void NumuEffTools::FillFracGraphs(TGraphAsymmErrors **gfrac,int NCUTS2, double xmin,double xmax,int NFILES2,int color){
        
    double _posmax=xmax;
    double _posmin=xmin;
    double step=0;
    if(_takestepdef){
        _posmax=xmin+getstep(NCUTS2-1)+5;
        _posmin=xmin;
    }
     
    for (int f=_fileSTART;f<NFILES2;f++){
        TH1F *hnum = new TH1F("hnum"," frac ",NCUTS2*10,_posmin,(double)_posmax);
        TH1F *hdenom = new TH1F("hdenom"," frac ",NCUTS2*10,_posmin,(double)_posmax);
        for (int i=_cutSTART;i<NCUTS2;i++){
            if(_takestepdef) step=xmin+getstep(i);
            else step=xmin+(xmax-xmin)/NCUTS2*i;
            hnum->Fill(step,(double)_fracEvents[f][i][0]);
            hdenom->Fill(step,(double)_fracEvents[f][i][1]);
        }
        gfrac[f] = new TGraphAsymmErrors(NCUTS2*10);
        gfrac[f]->BayesDivide(hnum,hdenom);

        delete hnum;
        delete hdenom;
    
        gfrac[f]->SetLineStyle(f+1);      
        gfrac[f]->SetLineColor(color);
        gfrac[f]->SetMarkerColor(color);
        gfrac[f]->SetMarkerStyle(8);
            
    }
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void NumuEffTools::DrawFrac( std::string *num_cut, std::string *denom_cut, int NCUTS2,std::string name,string title,double xmin,double xmax,double ymin,double ymax,int NFILES2,string xAxisLabel, int legPos,string legname,int color ,string drawOption){ 

      
    if(_fillarrays)
        FillFracEventsArray(num_cut,denom_cut,NCUTS2,xmin,xmax,NFILES2);
    if(_fillgraphs)
        FillFracGraphs(NCUTS2,xmin,xmax,NFILES2,color);
    //---------- Draw Plot ----------
    gStyle->SetOptStat(0);
    _c1->cd();

    _gfrac[0]->SetTitle(title.c_str()); 
    if(ymax!=0)
        _gfrac[0]->GetYaxis()->SetRangeUser(ymin,ymax);
    if(xmax!=0)
        _gfrac[0]->GetXaxis()->SetRangeUser(xmin-0.5,xmax+0.5);
    _gfrac[0]->GetXaxis()->SetTitle(xAxisLabel.c_str());
    _c1->cd();


    if(drawOption=="same")
        _gfrac[0]->Draw("l p e1");
    else
        _gfrac[0]->Draw("A l p e1");
    if(_NMCFILES>1)
        _gfrac[1]->Draw("l p e2");
        
        
    gStyle->SetOptStat(0);
    gStyle->SetErrorX(0);
    _c1->SetGridy();
       
    TLegend *leg;
    if (legPos==1) leg = new TLegend(0.55,0.73,0.75,0.87);  
    else if (legPos==2) leg = new TLegend(0.15,0.13,0.35,0.27); 
    else if (legPos==3) leg = new TLegend(0.35,0.18,0.55,0.33); 
    else leg = new TLegend(0.25,0.73,0.45,0.87);
        
    leg->SetFillColor(10);  
    leg->SetLineColor(0);
    string _legname_nt=legname+"/NEUT";
    string _legname_gn=legname+"/GENIE";
    leg->AddEntry(_gfrac[0],_legname_nt.c_str(),"PEL");
    leg->AddEntry(_gfrac[1],_legname_gn.c_str(),"P");
    leg->Draw("l");
        
    string epsname;
    epsname="plots/"+name+".eps";
    _c1->Print(epsname.c_str());
    if(_write){
        _myfile->cd();
        _c1->Write();
    }
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void NumuEffTools::FillEventsArray(std::string *sel_cut,int NCUTS2, double xmin,double xmax){
      
    double step=0;
    for (int i=_cutSTART;i<NCUTS2;i++){
        if(_takestepdef)step=xmin+getstep(i);
        else step=xmin+(xmax-xmin)/(double)NCUTS2*i;
               
        std::string scut;
        scut = sel_cut[i];
         
        for (int f=_fileSTART;f<NFILES;f++){
            _ffiles[f]->cd();
            TTree *numutree=(TTree*) _ffiles[f]->Get("numutree");
                         
            TH1F *h1 = new TH1F("h1","h",100,0,100);
            numutree->Project("h1","std_TrueSpillInfo",scut.c_str());
            _Events[f][i] =(int) h1->GetEntries();
            delete h1;
              
                
            std::cout << i <<" pos "<<step<< " " << f;
            if(f==2)
                std::cout<< " seli " << _Events[f][i];
            else
                std::cout<< " seli " << _Events[f][i]*_data.mcNorm[f] ;
            std::cout<<" "<< std::endl;
        }

    }
       
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void NumuEffTools::FillEventsGraphs(int NCUTS2, double xmin,double xmax){

    const int NCUTS=NCUTS2;
    double step=0;
    for (int f=_fileSTART;f<NFILES;f++){
        _gevt[f]=new TGraphErrors(NCUTS2);
        for (int i=_cutSTART;i<NCUTS;i++){
            if(_takestepdef) step=xmin+getstep(i);
            else step=xmin+(xmax-xmin)/(double)NCUTS2*i;//xmin+dstep*i;

               
            if(f==2){
                _gevt[f]->SetPoint(i,step,_Events[f][i]);
                _gevt[f]->SetPointError(i,0,sqrt(_Events[f][i]));
            }
            else{
                double evt=_Events[f][i]*_data.mcNorm[f];
                double evt_error=sqrt(_Events[f][i])*_data.mcNorm[f];
                    
                _gevt[f]->SetPoint(i,step,evt);
                _gevt[f]->SetPointError(i,0,evt_error);
            }
        }
        _gevt[f]->SetLineStyle(1);
        _gevt[f]->SetLineColor(f+1);      
        _gevt[f]->SetMarkerColor(f+1);
        _gevt[f]->SetMarkerStyle(f+6);
    }
    _gratio=new TGraphErrors(NCUTS);
    _gratio_neut=new TGraphErrors(NCUTS);
    _gratio_genie=new TGraphErrors(NCUTS);

    for(int icut=0;icut<NCUTS;icut++){
        //f=A/B
        //error=f * sqrt((dA/A)^2 + (dB/B)^2)
        double ixa,iya,ixb,iyb;
        _gevt[0]->GetPoint(icut,ixa,iya);//neut
        _gevt[1]->GetPoint(icut,ixb,iyb);//genie
        double error=iya/iyb*sqrt(pow(_gevt[0]->GetErrorY(icut)/iya,2)+pow(_gevt[1]->GetErrorY(icut)/iyb,2));
        double val=iya/iyb;
                         
        _gevt[2]->GetPoint(icut,ixa,iya);//data
        _gevt[0]->GetPoint(icut,ixb,iyb);//neut
        double error_neut=iya/iyb*sqrt(pow(_gevt[0]->GetErrorY(icut)/iya,2)+pow(_gevt[2]->GetErrorY(icut)/iyb,2));
        double val_neut=iya/iyb;
        _gevt[1]->GetPoint(icut,ixb,iyb);//genie
        double error_genie=iya/iyb*sqrt(pow(_gevt[0]->GetErrorY(icut)/iya,2)+pow(_gevt[2]->GetErrorY(icut)/iyb,2));
        double val_genie=iya/iyb;

        _gratio->SetPointError(icut,0,error);
        _gratio->SetPoint(icut,ixa,val);
            
        _gratio_neut->SetPointError(icut,0,error_neut);
        _gratio_neut->SetPoint(icut,ixa,val_neut);
        _gratio_genie->SetPointError(icut,0,error_genie);
        _gratio_genie->SetPoint(icut,ixa,val_genie);
        cout <<" ix "<<ixa
             <<"\n val a/b: NEUT/GENIE "<<val<<"\t Data/NEUT "<<val_neut<<"\t Data/Genie "<<val_genie
             <<" \n errorY: NEUT/GENIE "<<error<<"\t Data/NEUT "<<error_neut<<"\t Data/Genie "<<error_genie
             <<endl;
    }

    _gratio->SetLineColor(4);
    _gratio->SetMarkerColor(4);
    _gratio->SetMarkerStyle(8);
    _gratio_neut->SetLineStyle(1);      
    _gratio_neut->SetMarkerStyle(8);
    _gratio_genie->SetLineStyle(2);      
    _gratio_genie->SetMarkerStyle(25);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void NumuEffTools::DrawEv( std::string *sel_cut,int NCUTS2,std::string nameEv,string titleEv, double xmin,double xmax,double ymin,double ymax,string xAxisLabel, int legPos){ 
        

   
    if(_fillarrays)
        FillEventsArray(sel_cut,NCUTS2,xmin,xmax);
    if(_fillgraphs)
        FillEventsGraphs(NCUTS2,xmin,xmax);
    //----------- Draw Plot ----------
    gStyle->SetOptStat(0);
    _c1->cd();
    double ixa,iya;
    _gevt[2]->GetPoint(0,ixa,iya);//neut
       
    if(ymax!=0)
        _gevt[2]->GetYaxis()->SetRangeUser(ymin,ymax);
    if(xmax!=0)
        _gevt[2]->GetXaxis()->SetRangeUser(xmin-0.5,xmax+0.5);

    _gevt[2]->SetTitle(titleEv.c_str());
    _gevt[2]->GetXaxis()->SetTitle(xAxisLabel.c_str());
    _gevt[2]->Draw("A  p e1");
    _gevt[1]->Draw(" p e1");
    _gevt[0]->Draw(" p e1");

    gStyle->SetOptStat(0);
    gStyle->SetErrorX(0);
    _c1->SetGridy();
    TLegend *leg;
    if (legPos==1) leg = new TLegend(0.55,0.73,0.75,0.87);  //nuE CC
    else if (legPos==2) leg = new TLegend(0.15,0.13,0.35,0.27);  //Q2 CC
    else if (legPos==3) leg = new TLegend(0.35,0.18,0.55,0.33);  //
    else leg = new TLegend(0.25,0.73,0.45,0.87);

    leg->SetFillColor(10);  
    leg->SetLineColor(0);
    leg->AddEntry(_gevt[0],"NEUT","l");
    leg->AddEntry(_gevt[1],"GENIE","l");
    leg->AddEntry(_gevt[2],"Data","l");
    leg->Draw("l");
        
    string epsname;
    epsname="plots/"+nameEv+".eps";
    _c1->Print(epsname.c_str());
    if(_write){
        _myfile->cd();
        _c1->Write();
    }
     
        
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void  NumuEffTools::DrawEvRatio(std::string *sel_cut,int NCUTS2,string nameEvRatio, string titleEvRatio, double xmin,double xmax,double ymin,double ymax,string xAxisLabel, int legPos,bool dofit){ 
    
    
    if(_fillarrays)
        FillEventsArray(sel_cut,NCUTS2,xmin,xmax);
    if(_fillgraphs)
        FillEventsGraphs(NCUTS2,xmin,xmax);
       
    gStyle->SetOptStat(0);
           
    //--------- Fill Ev Ratio plots //
      
     
    _c2->cd();
    if(dofit){
        double min=0; double max=0; double iya=0;
        _gevt[0]->GetPoint(2,min,iya);
        _gevt[0]->GetPoint(NCUTS2-1,max,iya);
        TF1 *func_0=new TF1("f0","pol0",min,max);
        TF1 *func_1=new TF1("f1","pol0",min,max);
        TF1 *func=new TF1("f","pol0",min,max);
        // gStyle->SetOptFit(1);
        // gStyle->SetStatY(0.3);
        // gStyle->SetStatX(0.5);
        gStyle->SetStatBorderSize(0);
        gStyle->SetStatColor(0);
        func_0->SetLineWidth(1);
        cout<<" for Data/NEUT "<<endl;
        _gratio_neut->Fit(func_0,"QRV+");
        func_1->SetLineStyle(2);
        func_1->SetLineWidth(1);
        cout<<" for Data/GENIE "<<endl;
        _gratio_genie->Fit(func_1,"QRV+");
        func->SetLineColor(4);
        func->SetLineWidth(1);
        cout<<" for NEUT/GENIE "<<endl;
        _gratio->Fit(func,"QRV+");
    }

    _gratio->SetTitle(titleEvRatio.c_str());
    if(ymax!=0)
        _gratio->GetYaxis()->SetRangeUser(ymin,ymax);
    if(xmax!=0)
        _gratio->GetXaxis()->SetRangeUser(xmin-5,(double)xmax+5);
    _gratio->GetXaxis()->SetTitle(xAxisLabel.c_str());
    _gratio->Draw("A  p e1");
    _gratio_neut->Draw("  p e1");
    _gratio_genie->Draw(" p e1");

    TLegend *leg;
    if (legPos==1) leg = new TLegend(0.55,0.73,0.75,0.87);  //nuE CC
    else if (legPos==2) leg = new TLegend(0.15,0.13,0.35,0.27);  //Q2 CC
    else if (legPos==3) leg = new TLegend(0.35,0.18,0.55,0.33);  //
    else leg = new TLegend(0.25,0.73,0.45,0.87);

    leg->SetFillColor(10);  
    leg->SetLineColor(0);
    leg->AddEntry(_gratio_neut,"Data/NEUT","l");
    leg->AddEntry(_gratio_genie,"Data/GENIE","l");
    leg->AddEntry(_gratio,"NEUT/GENIE","l");
    leg->Draw("l");

        
    gStyle->SetErrorX(0);
    gStyle->SetOptStat(0);
        
    _c2->SetGridy();
    string epsname="plots/"+nameEvRatio+".eps";
    _c2->Print(epsname.c_str());
    if(_write){
        _myfile->cd();
        _c2->Write();
    }
        
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void NumuEffTools::FillDoubleRatioGraphs( TGraphAsymmErrors **gfrac1, TGraphAsymmErrors **gfrac2,int NCUTS2, double xmin,double xmax,int NFILES2,int color){

    double _posmax=xmax;
    double _posmin=xmin;
    if(_takestepdef){
        _posmax=xmin+getstep(NCUTS2-1)+5;
        _posmin=xmin;
    }
    for(int f=_fileSTART;f<NFILES2;f++){
        _gdoubleratio[f]=new TGraphErrors(NCUTS2);
        for(int icut=_cutSTART;icut<NCUTS2;icut++){
            double ixa,iya,ixb,iyb;
            gfrac1[f]->GetPoint(icut,ixa,iya);
            gfrac2[f]->GetPoint(icut,ixb,iyb);
                    
            double error=iya/iyb*sqrt(pow(gfrac1[f]->GetErrorY(icut)/iya,2)+pow(gfrac2[f]->GetErrorY(icut)/iyb,2));
            _gdoubleratio[f]->SetPoint(icut,ixa,iya/iyb);
            _gdoubleratio[f]->SetPointError(icut,0,error);
            cout<<"file "<<f<<" val a/b "<<iya/iyb
                <<" ix "<<ixa
                <<" errorY a "<<gfrac1[f]->GetErrorY(icut)<<" yA "<<iya
                <<" errorY b "<<gfrac2[f]->GetErrorY(icut)<<" yB "<<iyb
                <<endl;
            if(f==0){
                _gdoubleratio[f]->SetLineColor(color);
                _gdoubleratio[f]->SetMarkerColor(color);
                _gdoubleratio[f]->SetMarkerStyle(8);
            }
            else{
                _gdoubleratio[f]->SetLineColor(f+color);
                _gdoubleratio[f]->SetMarkerColor(f+color);
                _gdoubleratio[f]->SetMarkerStyle(8);
                                
            }
        }
    }
        
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void  NumuEffTools::DrawDoubleRatio( std::string *num_cut1, std::string *denom_cut1,std::string *num_cut2, std::string *denom_cut2, int NCUTS2,std::string name,string title,double xmin,double xmax,double ymin,double ymax,int NFILES2,string xAxisLabel, int legPos,string legname,int color,bool dofit){ 

    const int NCUTS=NCUTS2;
    TGraphAsymmErrors* gfrac1[NFILES];
    TGraphAsymmErrors* gfrac2[NFILES];
      
    FillFracEventsArray(num_cut1,denom_cut1,NCUTS2,xmin,xmax,NFILES2);
    FillFracGraphs(gfrac1,NCUTS2,xmin,xmax,NFILES2,color);
        
    FillFracEventsArray(num_cut2,denom_cut2,NCUTS2,xmin,xmax,NFILES2);
    FillFracGraphs(gfrac2,NCUTS2,xmin,xmax,NFILES2,color);

    FillDoubleRatioGraphs( gfrac1, gfrac2, NCUTS2,  xmin, xmax, NFILES2, color);
    if(dofit){
        double min; double max; double iya;
        for(int f=0;f<NFILES2;f++){
            _gdoubleratio[f]->GetPoint(2,min,iya);
            _gdoubleratio[f]->GetPoint(NCUTS-1,max,iya);
            TF1 *func=new TF1("f","pol0",min,max);
            // gStyle->SetOptFit(1);
            // gStyle->SetStatY(0.3);
            // gStyle->SetStatX(0.5);
            gStyle->SetStatBorderSize(0);
            gStyle->SetStatColor(0);
            func->SetLineWidth(1);
            func->SetLineStyle(f+1);
            _gdoubleratio[f]->Fit(func,"QRV+");
        }
    }
    if(ymax!=ymin)
        _gdoubleratio[0]->GetYaxis()->SetRangeUser(ymin,ymax);
    _gdoubleratio[0]->GetXaxis()->SetTitle(xAxisLabel.c_str());
    _gdoubleratio[0]->SetTitle(title.c_str());
    _gdoubleratio[0]->Draw("A  p e1");
    _gdoubleratio[1]->Draw("p e1");
    if(NFILES2>2)
        _gdoubleratio[2]->Draw("p e1");

    TLegend *leg;
    if (legPos==1) leg = new TLegend(0.55,0.73,0.75,0.87);  //nuE CC
    else if (legPos==2) leg = new TLegend(0.15,0.13,0.35,0.27);  //Q2 CC
    else if (legPos==3) leg = new TLegend(0.35,0.18,0.55,0.33);  //
    else leg = new TLegend(0.25,0.73,0.45,0.87);

    leg->SetFillColor(10);  
    leg->SetLineColor(0);
    string _legname_nt=legname+"NEUT";
    string _legname_gn=legname+"GENIE";
    string _legname_dt=legname+"DATA";
    leg->AddEntry(_gdoubleratio[0],_legname_nt.c_str(),"l");
    leg->AddEntry(_gdoubleratio[1],_legname_gn.c_str(),"l");
    if(NFILES2>2)
        leg->AddEntry(_gdoubleratio[2],_legname_dt.c_str(),"l");
    leg->Draw("l");

    gStyle->SetErrorX(0);
    gStyle->SetOptStat(0);
    _c3->SetGridy();
    string epsname="plots/"+name+".eps";
    _c3->Print(epsname.c_str());
    if(_write){
        _myfile->cd();
        _c3->Write();
    }
}

//**************************************************
void NumuEffTools::FillEffPurvsVar(std::string var,std::string true_cut,  std::string seltrue_cut, std::string sel_cut,double nbins,double xmin,double xmax){ 

    string scut[3];
    // selected true cc with true vtx in fv
    scut[0] = seltrue_cut;
    // all true cc with true vtx in fv
    scut[1] = true_cut;
    // selected cc    
    scut[2] = sel_cut;
 
    std::string var1[3];
    for(int j=0;j<3;j++)var1[j]=var;
    if(_trueParam){
        var1[0] = var+"[stdB_trTrueVertexIndex[0]]";
        var1[1] = var+"[]";
        var1[2] = var+"[stdB_trTrueVertexIndex[0]]";
        if(_useDir){
            var1[0] = var+"[stdB_trTrueVertexIndex[0]][2]";
            var1[1] = var+"[][2]";
            var1[2] = var+"[stdB_trTrueVertexIndex[0]][2]";
        }
    }

       
    string purmcname[NMCFILES];
    string effmcname[NMCFILES];
    purmcname[0]="pur_neut";
    effmcname[0]="eff_neut";
    if(_NMCFILES>1){
        purmcname[1]="pur_genie";
        effmcname[1]="eff_genie";
    }
    for(int imc=_fileSTART;imc<_NMCFILES;imc++){
           
        _geffvsvar[imc] = new TGraphAsymmErrors((int)nbins);
        _gpurvsvar[imc] = new TGraphAsymmErrors((int)nbins);
         
        _heffvsvar[imc] = new TH1F(effmcname[imc].c_str(),"",(int)nbins,xmin,xmax);
        _hpurvsvar[imc] = new TH1F(purmcname[imc].c_str(),"",(int)nbins,xmin,xmax);
    }
    TH1F *h0[3];//j=0 (seltrue), j=1 (true), j=2 (sel)
    TH1F *h1[3];
    for (int j=0; j<3; j++){
        char hname[50];
        sprintf(hname,"h0%d",j);
        _ffiles[0]->cd();
        TTree *numutree=(TTree*) _ffiles[0]->Get("numutree");
        h0[j] = new TH1F(hname,"neut",(int)nbins,xmin,xmax);
        numutree->Project(hname,var1[j].c_str(),scut[j].c_str());
        std::cout << hname << " " << h0[j]->GetEntries() << std::endl; 

        if(_NMCFILES>1){
            _ffiles[1]->cd();
            numutree=(TTree*) _ffiles[1]->Get("numutree");
            sprintf(hname,"h1%d",j);
            h1[j] = new TH1F(hname,"genie",(int)nbins,xmin,xmax);
            numutree->Project(hname,var1[j].c_str(),scut[j].c_str());
            std::cout << hname << " " << h1[j]->GetEntries() << std::endl; 
        }
    }
  
    _geffvsvar[0]->BayesDivide(h0[0],h0[1]);
    _gpurvsvar[0]->BayesDivide(h0[0],h0[2]);
    _heffvsvar[0]->Clear();
    _hpurvsvar[0]->Clear();
    _heffvsvar[0]->Divide(h0[0],h0[1]);
    _hpurvsvar[0]->Divide(h0[0],h0[2]);
    
    if(_NMCFILES>1){
        _geffvsvar[1]->BayesDivide(h1[0],h1[1]);
        _gpurvsvar[1]->BayesDivide(h1[0],h1[2]);
        _heffvsvar[1]->Clear();
        _hpurvsvar[1]->Clear();
        _heffvsvar[1]->Divide(h1[0],h1[1]);
        _hpurvsvar[1]->Divide(h1[0],h1[2]);
    }
    
    for (int j=0; j<3; j++){
        delete h0[j];
        if(_NMCFILES>1)
            delete h1[j];
    }
}

//**************************************************
void NumuEffTools::DrawEffvsVar(std::string var,std::string true_cut,  std::string seltrue_cut, std::string sel_cut,std::string name,string title,double nbins,double xmin,double xmax,string xAxisLabel, int legPos){
    if(_fillarrays)
        FillEffPurvsVar(var,true_cut,seltrue_cut,sel_cut,nbins,xmin,xmax);

    _geffvsvar[0]->SetLineColor(1);
    _geffvsvar[0]->SetMarkerColor(1);
    _geffvsvar[0]->SetMarkerStyle(8);
    _geffvsvar[0]->GetYaxis()->SetRangeUser(0.,1.1);
    _geffvsvar[0]->GetXaxis()->SetTitle(xAxisLabel.c_str());
    _geffvsvar[0]->SetTitle(title.c_str());
    _geffvsvar[0]->Draw("A p e1");
    if(_NMCFILES>1){
        _geffvsvar[1]->SetLineColor(4);
        _geffvsvar[1]->SetLineStyle(2);
        _geffvsvar[1]->SetFillStyle(0);
        _geffvsvar[1]->SetMarkerColor(4);
        _geffvsvar[1]->SetMarkerStyle(8);
        _geffvsvar[1]->Draw("p e2");
    }
    TLegend *leg;
    if (legPos==1) leg = new TLegend(0.55,0.73,0.75,0.87);  //nuE CC
    else if (legPos==2) leg = new TLegend(0.15,0.13,0.35,0.27);  //Q2 CC
    else if (legPos==3) leg = new TLegend(0.35,0.18,0.55,0.33);  //
    else leg = new TLegend(0.25,0.73,0.45,0.87);
        
    leg->SetFillColor(10);  
    leg->SetLineColor(0);
    leg->AddEntry(_geffvsvar[0],"NEUT","PEL");
    if(_NMCFILES>1)
        leg->AddEntry(_geffvsvar[1],"GENIE","P");
    leg->Draw("l");
    gStyle->SetErrorX(0);
    if (name!=""){
        std::string fname2 = "plots/"+name+".eps";
        _c1->SetFillColor(0);
        //_c1->SetGridy();
        _c1->Print(fname2.c_str());
    }
    gStyle->SetOptStat(0);
        
    if(_write){
        _myfile->cd();
        _c1->Write();
    }
        
}

//**************************************************
void  NumuEffTools::DrawPurvsVar(std::string var,std::string true_cut,  std::string seltrue_cut, std::string sel_cut,std::string name,string title,double nbins,double xmin,double xmax,string xAxisLabel, int legPos){
    if(_fillarrays)
        FillEffPurvsVar(var,true_cut,seltrue_cut,sel_cut,nbins,xmin,xmax);
    _gpurvsvar[0]->SetLineColor(1);
    _gpurvsvar[0]->SetMarkerColor(1);
    _gpurvsvar[0]->SetMarkerStyle(8);
    _gpurvsvar[0]->GetYaxis()->SetRangeUser(0.,1.1);
    _gpurvsvar[0]->GetXaxis()->SetTitle(xAxisLabel.c_str());
    _gpurvsvar[0]->SetTitle(title.c_str());
    _gpurvsvar[0]->Draw("A p e1");

    if(_NMCFILES>1){
        _gpurvsvar[1]->SetLineColor(4);
        _gpurvsvar[1]->SetLineStyle(2);
        _gpurvsvar[1]->SetFillStyle(0);
        _gpurvsvar[1]->SetMarkerColor(4);
        _gpurvsvar[1]->SetMarkerStyle(8);
        _gpurvsvar[1]->Draw("p e2");
    }
    TLegend *leg;
    if (legPos==1) leg = new TLegend(0.55,0.73,0.75,0.87);  //nuE CC
    else if (legPos==2) leg = new TLegend(0.15,0.13,0.35,0.27);  //Q2 CC
    else if (legPos==3) leg = new TLegend(0.35,0.18,0.55,0.33);  //
    else leg = new TLegend(0.25,0.73,0.45,0.87);
        
    leg->SetFillColor(10);  
    leg->SetLineColor(0);
    leg->AddEntry(_gpurvsvar[0],"NEUT","PEL");
    if(_NMCFILES>1)
        leg->AddEntry(_gpurvsvar[1],"GENIE","P");
    leg->Draw("l");
    gStyle->SetErrorX(0);
    if (name!=""){
        std::string fname2 = "plots/"+name+".eps";
        _c1->SetFillColor(0);
        //_c1->SetGridy();
        _c1->Print(fname2.c_str());
    }
    gStyle->SetOptStat(0);
    if(_write){
        _myfile->cd();
        _c1->Write();
    }
        
}
  
//**************************************************
void NumuEffTools::FillEffPurvsVar2d(std::string varx,std::string vary,std::string true_cut,  std::string seltrue_cut, std::string sel_cut,double nxbins,double xmin,double xmax,double nybins,double ymin,double ymax){ 

    string scut[3];
    // selected true cc with true vtx in fv
    scut[0] = seltrue_cut;
    // all true cc with true vtx in fv
    scut[1] = true_cut;
    // selected cc    
    scut[2] = sel_cut;
 
    std::string var1[3];
    std::string var2[3];
    for(int j=0;j<3;j++)var1[j]=varx;
    for(int j=0;j<3;j++)var2[j]=vary;
    if(_trueParam){
        for(int j=0;j<3;j++){
            if(j==1){
                var1[1] = varx+"[]";
                var2[1] = vary+"[]";
                if(_useDir)
                    var1[1] = varx+"[][2]";
                if(_useDir2)
                    var2[1] = vary+"[][2]";
                
            }
            else{
                var1[j] = varx+"[stdB_trTrueVertexIndex[0]]";
                var2[j] = vary+"[stdB_trTrueVertexIndex[0]]";
                if(_useDir)
                    var1[j] = varx+"[stdB_trTrueVertexIndex[0]][2]";
                if(_useDir2)
                    var2[j] = vary+"[stdB_trTrueVertexIndex[0]][2]";
                
            }
        }
    }

       
    string purmcname[NMCFILES];
    string effmcname[NMCFILES];
    purmcname[0]="pur_neut";
    effmcname[0]="eff_neut";
    if(_NMCFILES>1){
        effmcname[1]="eff_genie";
        purmcname[1]="pur_genie";
    }
    for(int imc=_fileSTART;imc<_NMCFILES;imc++){
        _heffvsvar2d[imc] = new TH2F(effmcname[imc].c_str(),"",(int)nxbins,xmin,xmax,(int)nybins,ymin,ymax);
        _hpurvsvar2d[imc] = new TH2F(purmcname[imc].c_str(),"",(int)nxbins,xmin,xmax,(int)nybins,ymin,ymax);
    }
    TH2F *h0[3];//j=0 (seltrue), j=1 (true), j=2 (sel)
    TH2F *h1[3];
       
    for (int j=0; j<3; j++){
        string var2d=var2[j]+":"+var1[j];
        char hname[50];
        sprintf(hname,"h0%d_2d",j);
        _ffiles[0]->cd();
        TTree* numutree=(TTree*) _ffiles[0]->Get("numutree");
        h0[j] = new TH2F(hname,"neut",(int)nxbins,xmin,xmax,(int)nybins,ymin,ymax);
        numutree->Project(hname,var2d.c_str(),scut[j].c_str());
        std::cout << hname << " " << h0[j]->GetEntries() << std::endl; 
        if(_NMCFILES>1){
            _ffiles[1]->cd();
            numutree=(TTree*) _ffiles[1]->Get("numutree");
            sprintf(hname,"h1%d_2d",j);
            h1[j] = new TH2F(hname,"genie",(int)nxbins,xmin,xmax,(int)nybins,ymin,ymax);
            numutree->Project(hname,var2d.c_str(),scut[j].c_str());
            std::cout << hname << " " << h1[j]->GetEntries() << std::endl; 
        }
    }

    _heffvsvar2d[0]->Clear();
    _hpurvsvar2d[0]->Clear();
    _heffvsvar2d[0]->Divide(h0[0],h0[1]);
    _hpurvsvar2d[0]->Divide(h0[0],h0[2]);
    if(_NMCFILES>1){
        _heffvsvar2d[1]->Clear();
        _hpurvsvar2d[1]->Clear();
        _heffvsvar2d[1]->Divide(h1[0],h1[1]);
        _hpurvsvar2d[1]->Divide(h1[0],h1[2]);
    }
    for (int j=0; j<3; j++){
        delete h0[j];
        if(_NMCFILES>1)
            delete h1[j];
    }
}

//**************************************************
void NumuEffTools::DrawEffvsVar2d(std::string varx,std::string vary,std::string true_cut,  std::string seltrue_cut, std::string sel_cut,std::string name,string title,double nxbins,double xmin,double xmax,double nybins, double ymin, double ymax,string xAxisLabel,string yAxisLabel ){
    if(_fillarrays)
        FillEffPurvsVar2d(varx,vary,true_cut,seltrue_cut,sel_cut,nxbins,xmin,xmax,nybins,ymin,ymax);

    _heffvsvar2d[0]->SetTitle(title.c_str());
    
    _heffvsvar2d[0]->GetXaxis()->SetTitle(xAxisLabel.c_str());
    _heffvsvar2d[0]->GetYaxis()->SetTitle(yAxisLabel.c_str());
    if(_NMCFILES>1){
        _heffvsvar2d[1]->GetXaxis()->SetTitle(xAxisLabel.c_str());
        _heffvsvar2d[1]->GetYaxis()->SetTitle(yAxisLabel.c_str());
    }
    if(_NMCFILES>1)
        _c1->Divide(2,1);
    _c1->cd(1);
    _heffvsvar2d[0]->Draw("colz");
    if(_NMCFILES>1){
        _c1->cd(2);
        _heffvsvar2d[1]->Draw("colz");
    }
    if (name!=""){
        std::string fname2 = "plots/"+name+".eps";
        _c1->SetFillColor(0);
        //_c1->SetGridy();
        _c1->Print(fname2.c_str());
    }
    gStyle->SetOptStat(0);
        
    if(_write){
        _myfile->cd();
        _c1->Write();
    }
        
}

//**************************************************
void NumuEffTools::DrawPurvsVar2d(std::string varx,std::string vary,std::string true_cut,  std::string seltrue_cut, std::string sel_cut,std::string name,string title,double nxbins,double xmin,double xmax,double nybins, double ymin, double ymax,string xAxisLabel,string yAxisLabel ){

    if(_fillarrays)
        FillEffPurvsVar2d(varx,vary,true_cut,seltrue_cut,sel_cut,nxbins,xmin,xmax,nybins,ymin,ymax);

    _hpurvsvar2d[0]->SetTitle(title.c_str());        
    _hpurvsvar2d[0]->GetXaxis()->SetTitle(xAxisLabel.c_str());
    _hpurvsvar2d[0]->GetYaxis()->SetTitle(yAxisLabel.c_str());
    if(_NMCFILES>1){
        _hpurvsvar2d[1]->GetXaxis()->SetTitle(xAxisLabel.c_str());
        _hpurvsvar2d[1]->GetYaxis()->SetTitle(yAxisLabel.c_str());
    }
    if(_NMCFILES>1)
        _c1->Divide(2,1);
    _c1->cd(1);
    _hpurvsvar2d[0]->Draw("colz");
    if(_NMCFILES>1){
        _c1->cd(2);
        _hpurvsvar2d[1]->Draw("colz");
    }
    if (name!=""){
        std::string fname2 = "plots/"+name+".eps";
        _c1->SetFillColor(0);
        //_c1->SetGridy();
        _c1->Print(fname2.c_str());
    }
    gStyle->SetOptStat(0);
        
    if(_write){
        _myfile->cd();
        _c1->Write();
    }
        
}
  
  


  




