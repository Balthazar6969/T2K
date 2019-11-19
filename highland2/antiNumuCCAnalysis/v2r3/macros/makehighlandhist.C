
void makehighlandhist(TString stFGD,TString infile,TString outfile,TString update="UPDATE",int _nbins=50)
{
  int nbins=_nbins;
  int low=0;
  int high=5000;
  TString mcfile    =infile;
  TString name      =outfile;
  TFile *outf=new TFile(name.Data(),update);
  DrawingTools *draw=new DrawingTools(mcfile.Data(),false);
  DataSample   *mc  =new DataSample(mcfile.Data()); 
  TString path="antiNumuCC"+stFGD;
  outf->mkdir(path.Data(),path.Data());

  TString treename="default";
  TH1F *hh[3];
  TString scuth2[3]={"(accum_level[][0]>6)*weight[0]*weight[1]","(accum_level[][0]>7)**weight[0]*weight[1]","(accum_level[][1]>7)**weight[0]*weight[1]"};
  TString nn[3]={"CC","CC1trk","CCNtrk"};  
 
  
  if(!mc->GetTree(treename.Data()) ){
    std::cout<<" tree "<<treename<<" does not exist! "<<std::endl;
    continue;
  }
  
  for(int ih=0;ih<3;ih++){
    std::cout<<" ih "<<ih<<" nn "<<nn[ih]<<std::endl;
    hh[ih]=new TH1F(Form("hh%s",nn[ih].Data()),"",nbins,low,high);
    TString scut=scuth2[ih];
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
    TString path3=path+"/"+nn[ih];
    outf->mkdir(path3.Data(),path3.Data());
    outf->cd(path3.Data());
    hh[ih]->SetName(Form("mom_h2_%s",nn[ih].Data()));
    hh[ih]->Write();
  }
  
  outf->Close();

}
