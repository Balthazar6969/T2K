
void makehighlandhist(TString stFGD,TString infile,TString outfile,int ishighland1,TString update="UPDATE",int _nbins=50)
{
  int nbins=_nbins;
  int low=0;
  int high=5000;
  TString mcfile    =infile;
  TString name      =outfile;
  TFile *outf=new TFile(name.Data(),update);
#if ishighland1
  DrawingTools    *draw =new DrawingTools(mcfile.Data(),false);
  MultiAnalysisTools *mc=new MultiAnalysisTools(mcfile.Data());
#else  
  DrawingTools *draw=new DrawingTools(mcfile.Data(),false);
  DataSample   *mc  =new DataSample(mcfile.Data()); 
#endif
  TString path="numuCC"+stFGD;
  outf->mkdir(path.Data(),path.Data());

  TString treename="default";
  TH1F *hh[4];
  TString scuth2[4]={"accum_level[][0]>5","accum_level[][0]>6","accum_level[][1]>6","accum_level[][2]>6"};
  TString scuth1[4]={"accum_level>5","accum_level>5 && cut6==1","accum_level>5 && cut7==1","accum_level>5 && cut8==1"};
  TString nn[4]={"CC","CC0pi","CC1pi","CCOth"};  
 
  
  if(!mc->GetTree(treename.Data()) ){
    std::cout<<" tree "<<treename<<" does not exist! "<<std::endl;
    continue;
  }
  
  for(int ih=0;ih<4;ih++){
    std::cout<<" ih "<<ih<<" nn "<<nn[ih]<<std::endl;
    hh[ih]=new TH1F(Form("hh%s",nn[ih].Data()),"",nbins,low,high);
#if ishighland1
    TString scut=scuth1[ih];
#else
    TString scut=scuth2[ih];
#endif
    std::cout<<treename.Data()<<std::endl;
    TTree *tree=(TTree*) mc->GetTree(treename.Data());
    //draw->Draw(tree,"selmu_mom",nbins,low,high,"reaction",scut.Data());
    // hh[ih]->Reset();
    //hh[ih] =(TH1F*) draw->GetLastHisto();

     tree->Project(hh[ih]->GetName(),"selmu_mom",scut.Data());
     //     tree->Scan("selmu_mom",scut.Data());
    hh[ih]->SetTitle(Form("%s ",nn[ih].Data()));
    hh[ih]->GetYaxis()->SetTitle("Entries");
    hh[ih]->GetXaxis()->SetTitle("Reconstructed momentum (MeV/c)");
    outf->cd();  
    TString nnn=nn[ih];
    if(_nbins==20){
      nnn+="_reg20";
    }

    TString path3=path+"/"+nnn;
    outf->mkdir(path3.Data(),path3.Data());
    outf->cd(path3.Data());
#if ishighland1
    hh[ih]->SetName(Form("mom_h1_%s",nn[ih].Data()));
    hh[ih]->Write();
#else
    hh[ih]->SetName(Form("mom_h2_%s",nn[ih].Data()));
    hh[ih]->Write();
#endif
  }
  
  outf->Close();

}
