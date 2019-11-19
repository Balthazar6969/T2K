void make_final_plots(TString run5c_data, TString MagnetMC, TString SandMC){

  DrawingTools *draw = new DrawingTools(MagnetMC.Data(),false);

  Experiment exp("nd280");

  //data samples
  DataSample* data_run5 = new DataSample(run5c_data.Data());

  //Magnet MC samples
  DataSample* magnet_mc_run5 = new DataSample(MagnetMC.Data());

  //Sand MC samples
  DataSample* sand_mc_run5 = new DataSample(SandMC.Data());

  SampleGroup run5("run5");
  run5.AddDataSample(data_run5);
  run5.AddMCSample("magnet",magnet_mc_run5);
  run5.AddMCSample("sand",sand_mc_run5);

  exp.AddSampleGroup("run5",run5);

  magnet_mc_run5->SetCurrentTree("all_syst");
  sand_mc_run5->SetCurrentTree("sandmu_syst");

  TCanvas *c1[3];
  //the additional condition "weight_syst[][7]<100" is needed to avoid abnormal weight from SI pion systematic (that is the weight systematic number 7 in this analysis)
  TString scuth2[3]={"(accum_level[][0]>6 && weight_syst[][7]<100)","(accum_level[][0]>7 && weight_syst[][7]<100)","(accum_level[][1]>7 && weight_syst[][7]<100)"};
  TString nn[3]={"CCInc","CC1trk","CCNtrk"};  
  TString mom_plot_TitleY[3]={"Events/(200 MeV/c)","Events/(200 MeV/c)","Events/(250 MeV/c)"};
  TString variable[2]={"selmu_mom","selmu_costheta"};
  TString var[2]={"mom","cos"};


  for(int ii=0; ii<2; ii++){
    for(int ih=0; ih<3; ih++){
      c1[ih]=new TCanvas(Form("c1_%s_%s",nn[ih].Data(),var[ii].Data()),Form("c1_%s_%s",nn[ih].Data(),var[ii].Data()),1);
      draw->SetOptStat("emri");
      if(ii==0){
	draw->SetTitleY(Form("%s",mom_plot_TitleY[ih].Data()));
	draw->SetTitleX("Muon momentum (MeV/c)");
      }
      else{
	draw->SetTitleY("Events");
	draw->SetTitleX("Muon cos(#theta)");
      }
      draw->SetAllMCStatLabel("MC stat. error");
      draw->SetAllMCLabel("MC stat. + det. syst. error");
      draw->SetMCErrorColor(kAzure);
      if(ii==0){
	if(ih==2)draw->Draw(exp,Form("%s",variable[ii].Data()),25,0,5000,"all",Form("%s",scuth2[ih].Data()),"","SYS E2");
	else draw->Draw(exp,Form("%s",variable[ii].Data()),20,0,5000,"all",Form("%s",scuth2[ih].Data()),"","SYS E2");
      }
      else draw->Draw(exp,Form("%s",variable[ii].Data()),15,0,1,"all",Form("%s",scuth2[ih].Data()),"","SYS E2");

      c1[ih]->Print(Form("final_%s_%s.png",nn[ih].Data(),var[ii].Data()));
    }
  }
}
