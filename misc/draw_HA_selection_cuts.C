{
  // Style sheet
  gROOT.ProcessLine(".x /home/rberner/nd280work/.rootlogin.C");
  gStyle->SetPadRightMargin(0.05); 
  gStyle->SetPadLeftMargin(0.18); //0.18
  TGaxis::SetMaxDigits(5); //4
  gStyle->SetTitleOffset(1.05,"Y");

  double pi = 3.141592653589793238463;

  // Create experiment
  Experiment exp("t2k");


  // Run testfile or not
  bool testfile = false;


  if (testfile) {
    DataSample* test_01 = new DataSample("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/test.root");
    /*DataSample* test_02 = new DataSample("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/test2.root"); // SAMPLE2
    DataSample* test_03 = new DataSample("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/test3.root"); // SAMPLE3
    DataSample* test_04 = new DataSample("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/test4.root"); // SAMPLE4
    DataSample* test_05 = new DataSample("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/test5.root"); // SAMPLE5
    DataSample* test_06 = new DataSample("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/test6.root"); // SAMPLE6
    DataSample* test_07 = new DataSample("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/test7.root"); // SAMPLE7
    DataSample* test_08 = new DataSample("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/test8.root"); // SAMPLE8*/
    test_01->SetCurrentTree("default");
    /*test_02->SetCurrentTree("default"); // SAMPLE2
    test_03->SetCurrentTree("default"); // SAMPLE3
    test_04->SetCurrentTree("default"); // SAMPLE4
    test_05->SetCurrentTree("default"); // SAMPLE5
    test_06->SetCurrentTree("default"); // SAMPLE6
    test_07->SetCurrentTree("default"); // SAMPLE7
    test_08->SetCurrentTree("default"); // SAMPLE8*/
    SampleGroup test("test");
    test.AddMCSample("test_01",test_01);
    /*test.AddMCSample("test_02",test_02); // SAMPLE2
    test.AddMCSample("test_03",test_03); // SAMPLE3
    test.AddMCSample("test_04",test_04); // SAMPLE4
    test.AddMCSample("test_05",test_05); // SAMPLE5
    test.AddMCSample("test_06",test_06); // SAMPLE6
    test.AddMCSample("test_07",test_07); // SAMPLE7
    test.AddMCSample("test_08",test_08); // SAMPLE8*/
    DrawingTools draw("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/test.root");
    exp.AddSampleGroup("test", test);
  }


  // Load root files
  // =========================
  if (!testfile) {
    // Want to put GENIE files to real data. Because only one RD file is allowed to be added to each sample group, I have to generate a sample group for each file:
    // mc_magnet_run5_01 and so on instead of only mc_magnet_run5.
    DataSample* mc_magnet_run5_01   = new DataSample("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/NEUT_Run3_20180117/water_01.root");
    DataSample* mc_magnet_run5_02   = new DataSample("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/NEUT_Run3_20180117/water_02.root");
    DataSample* mc_magnet_run6_01   = new DataSample("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/NEUT_Run3_20180117/air_01.root");
    DataSample* mc_magnet_run6_02   = new DataSample("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/NEUT_Run3_20180117/air_02.root");
    DataSample* mc_magnet_run6_03   = new DataSample("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/NEUT_Run3_20180117/air_03.root");
    DataSample* mc_magnet_run6_04   = new DataSample("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/NEUT_Run3_20180117/air_04.root");
    DataSample* mc_sand_01          = new DataSample("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/NEUT_Run3_20180117/sand_01.root");
    DataSample* mc_sand_02          = new DataSample("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/NEUT_Run3_20180117/sand_02.root");

    DataSample* mc_magnet_run5_01_GENIE   = new DataSample("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/GENIE_Run3_20180117/water_GENIE_01.root");
    DataSample* mc_magnet_run5_02_GENIE   = new DataSample("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/GENIE_Run3_20180117/water_GENIE_02.root");
    DataSample* mc_magnet_run6_01_GENIE   = new DataSample("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/GENIE_Run3_20180117/air_GENIE_01.root");
    DataSample* mc_magnet_run6_02_GENIE   = new DataSample("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/GENIE_Run3_20180117/air_GENIE_02.root");
    DataSample* mc_magnet_run6_03_GENIE   = new DataSample("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/GENIE_Run3_20180117/air_GENIE_03.root");
    DataSample* mc_magnet_run6_04_GENIE   = new DataSample("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/GENIE_Run3_20180117/air_GENIE_04.root");

    mc_magnet_run5_01->SetCurrentTree("default");
    mc_magnet_run5_02->SetCurrentTree("default");
    mc_magnet_run6_01->SetCurrentTree("default");
    mc_magnet_run6_02->SetCurrentTree("default");
    mc_magnet_run6_03->SetCurrentTree("default");
    mc_magnet_run6_04->SetCurrentTree("default");
    mc_sand_01->SetCurrentTree("default");
    mc_sand_02->SetCurrentTree("default");

    mc_magnet_run5_01_GENIE->SetCurrentTree("default");
    mc_magnet_run5_02_GENIE->SetCurrentTree("default");
    mc_magnet_run6_01_GENIE->SetCurrentTree("default");
    mc_magnet_run6_02_GENIE->SetCurrentTree("default");
    mc_magnet_run6_03_GENIE->SetCurrentTree("default");
    mc_magnet_run6_04_GENIE->SetCurrentTree("default");

    SampleGroup magnet_run5_01("magnet_run5_01");
    SampleGroup magnet_run5_02("magnet_run5_02");
    SampleGroup magnet_run6_01("magnet_run6_01");
    SampleGroup magnet_run6_02("magnet_run6_02");
    SampleGroup magnet_run6_03("magnet_run6_03");
    SampleGroup magnet_run6_04("magnet_run6_04");
    SampleGroup sand_01("sand_01");
    SampleGroup sand_02("sand_02");

    SampleGroup magnet_run5_01_GENIE("magnet_run5_01_GENIE");
    SampleGroup magnet_run5_02_GENIE("magnet_run5_02_GENIE");
    SampleGroup magnet_run6_01_GENIE("magnet_run6_01_GENIE");
    SampleGroup magnet_run6_02_GENIE("magnet_run6_02_GENIE");
    SampleGroup magnet_run6_03_GENIE("magnet_run6_03_GENIE");
    SampleGroup magnet_run6_04_GENIE("magnet_run6_04_GENIE");

    magnet_run5_01.AddMCSample("mc_magnet_run5_01",mc_magnet_run5_01);
    magnet_run5_02.AddMCSample("mc_magnet_run5_02",mc_magnet_run5_02);
    magnet_run6_01.AddMCSample("mc_magnet_run6_01",mc_magnet_run6_01);
    magnet_run6_02.AddMCSample("mc_magnet_run6_02",mc_magnet_run6_02);
    magnet_run6_03.AddMCSample("mc_magnet_run6_03",mc_magnet_run6_03);
    magnet_run6_04.AddMCSample("mc_magnet_run6_04",mc_magnet_run6_04);
    sand_01.AddMCSample("mc_sand_01",mc_sand_01);
    sand_01.AddMCSample("mc_sand_02",mc_sand_02);

    magnet_run5_01.AddDataSample(mc_magnet_run5_01_GENIE);
    magnet_run5_02.AddDataSample(mc_magnet_run5_02_GENIE);
    magnet_run6_01.AddDataSample(mc_magnet_run6_01_GENIE);
    magnet_run6_02.AddDataSample(mc_magnet_run6_02_GENIE);
    magnet_run6_03.AddDataSample(mc_magnet_run6_03_GENIE);
    magnet_run6_04.AddDataSample(mc_magnet_run6_04_GENIE);
}


  // Open file and access tree
  // =========================
  //TChain * RD_magnet = new TChain("default");
  //RD_magnet->Add("../RD_water_medium.root");
  //RD_magnet->Add("../RD_water_medium.root");
  //RD_magnet->Branch("default");

  // Experiment
  if (!testfile) {
    exp.AddSampleGroup("magnet_run5_01", magnet_run5_01);
    exp.AddSampleGroup("magnet_run5_02", magnet_run5_02);
    exp.AddSampleGroup("magnet_run6_01", magnet_run6_01);
    exp.AddSampleGroup("magnet_run6_02", magnet_run6_02);
    exp.AddSampleGroup("magnet_run6_03", magnet_run6_03);
    exp.AddSampleGroup("magnet_run6_04", magnet_run6_04);
    exp.AddSampleGroup("sand_01", sand_01);
    exp.AddSampleGroup("sand_02", sand_02);
  }

  // Summary Plots
  // =========================
  // Create an instance of the DrawingTools and load the configuration specified in the input file
  if (!testfile) DrawingTools draw("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/NEUT_Run3_20180117/air_01.root");

  // Plot canvas size
  int npix_x = 1000;
  int npix_y = 800;

  // -------------------------------------------------
  // HA: selmu_tpc_nnodes
  // -------------------------------------------------
  TCanvas *c0 = new TCanvas("c0","Canvas",npix_x,npix_y);
    gStyle->SetOptStat(0);
    draw.SetMinY(1);
    draw.SetLogY(1);
    draw.SetMaxY(100000);
    draw.SetLegendParam(0.21,0.90,0.39,0.91);
    draw.SetTitleX("#mu candidate TPC nodes [-]");
    draw.SetTitleY("#events");
    draw.Draw(exp,"selmu_tpc_nnodes",101,-20.5,80.5,"particle","accum_level[][2]>2","E1","POTNORM",-1,true); //"","NODATA",-1 //E1 is the argument for root: Plotting error bars with small lines at the end
    draw.DrawCutLineVertical(18.5,true,"l");
    c0->SaveAs("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/Plots/CutValues/PNGs/HA/HA_TPC_nodes.png");

  // -------------------------------------------------
  // HA: selmu_fgdlayer
  // -------------------------------------------------
  TCanvas *c1 = new TCanvas("c1","Canvas",npix_x,npix_y);
    gStyle->SetOptStat(0);
    draw.SetMinY(0);
    draw.SetLogY(0);
    draw.SetMaxY(1500);
    draw.SetLegendParam(0.58,0.90,0.76,0.91); //0.27,0.90,0.45,0.91
    draw.SetTitleX("#mu candidate FGD1 start layer [-]");
    draw.SetTitleY("#events");
    draw.Draw(exp,"selmu_fgdlayer",34,-1.5,32.5,"reactionCC","accum_level[][2]>3","E1","POTNORM",-1,true);
    //draw.DrawCutLineVertical(1.5,true,"r");
    draw.DrawCutLineVertical(27.5,true,"l");
    c1->SaveAs("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/Plots/CutValues/PNGs/HA/HA_FGD1_start_layer.png");

  // -------------------------------------------------
  // HA: ToF from FGD1 to: earliest ECal
  // -------------------------------------------------
  TCanvas *c2 = new TCanvas("c2","Canvas",npix_x,npix_y);
    gStyle->SetOptStat(0);
    draw.SetMinY(0);
    draw.SetLogY(0);
    draw.SetMaxY();
    draw.SetLegendParam(0.22,0.90,0.40,0.91);
    draw.SetTitleX("ToF FGD1-earliestECal [ns]");
    draw.SetTitleY("#events");
    draw.Draw(exp,"selmu_ToF_ecal_fgd1_other",80,-40,40,"reactionCC","accum_level[][2]>4","E1","POTNORM",-1,true);
    draw.DrawCutLineVertical(2.2,true,"r");
    c2->SaveAs("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/Plots/CutValues/PNGs/HA/HA_ToF_FGD1_to_earliestECal.png");

  // -------------------------------------------------
  // HA: ToF from FGD1 to: TECal
  // -------------------------------------------------
  TCanvas *c3 = new TCanvas("c3","Canvas",npix_x,npix_y);
    gStyle->SetOptStat(0);
    draw.SetMinY(0);
    draw.SetLogY(0);
    draw.SetLegendParam(0.22,0.90,0.40,0.91);
    draw.SetTitleX("ToF FGD1-TECal [ns]");
    draw.SetTitleY("#events");
    draw.Draw(exp,"selmu_ToF_tecal_fgd1",80,-40,40,"reactionCC","accum_level[][2]>6","E1","POTNORM",-1,true);
    draw.DrawCutLineVertical(0.4,true,"r");
    c3->SaveAs("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/Plots/CutValues/PNGs/HA/HA_ToF_FGD1_to_TECal.png");

  // -------------------------------------------------
  // HA: selmu_mom of ECal contained tracks
  // -------------------------------------------------
  TCanvas *HA_selmu_mom_ecal_contained = new TCanvas("HA_selmu_mom_ecal_contained","Canvas",npix_x,npix_y);
    gStyle->SetOptStat(0);
    draw.SetLegendParam(0.72,0.90,0.90,0.91);
    draw.SetTitleX("reconstructed momentum [MeV/c]");
    draw.SetTitleY("#events");
    draw.SetMaxY();
    draw.Draw(exp,"selmu_mom",100,0,7000,"particle","accum_level[0][2]>6 && selmu_ecal_containment==1","E1","POTNORM",-1,true);
    HA_selmu_mom_ecal_contained->SaveAs("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/Plots/CutValues/PNGs/HA/HA_selmu_mom_ECal_contained.png");

  // -------------------------------------------------
  // HA: selmu_mom of ECal NOT contained tracks
  // -------------------------------------------------
  TCanvas *HA_selmu_mom_ecal_not_contained = new TCanvas("HA_selmu_mom_ecal_not_contained","Canvas",npix_x,npix_y);
    gStyle->SetOptStat(0);
    draw.SetLegendParam(0.72,0.90,0.90,0.91);
    draw.SetTitleX("reconstructed momentum [MeV/c]");
    draw.SetTitleY("#events");
    draw.SetMaxY();
    draw.Draw(exp,"selmu_mom",100,0,7000,"particle","accum_level[0][2]>6 && selmu_ecal_containment==0","E1","POTNORM",-1,true);
    HA_selmu_mom_ecal_not_contained->SaveAs("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/Plots/CutValues/PNGs/HA/HA_selmu_mom_ECal_not_contained.png");

  // -------------------------------------------------
  // HA: selmu_truemom of ECal contained tracks
  // -------------------------------------------------
  TCanvas *HA_selmu_truemom_ecal_contained = new TCanvas("HA_selmu_truemom_ecal_contained","Canvas",npix_x,npix_y);
    gStyle->SetOptStat(0);
    draw.SetLegendParam(0.72,0.90,0.90,0.91);
    draw.SetTitleX("true momentum [MeV/c]");
    draw.SetTitleY("#events");
    draw.SetMaxY();
    draw.Draw(exp,"selmu_truemom",100,0,7000,"particle","accum_level[0][2]>6 && selmu_ecal_containment==1","E1","POTNORM",-1,true);
    HA_selmu_truemom_ecal_contained->SaveAs("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/Plots/CutValues/PNGs/HA/HA_selmu_truemom_ECal_contained.png");

  // -------------------------------------------------
  // HA: selmu_truemom of ECal NOT contained tracks
  // -------------------------------------------------
  TCanvas *HA_selmu_truemom_ecal_not_contained = new TCanvas("HA_selmu_truemom_ecal_not_contained","Canvas",npix_x,npix_y);
    gStyle->SetOptStat(0);
    draw.SetLegendParam(0.72,0.90,0.90,0.91);
    draw.SetTitleX("true momentum [MeV/c]");
    draw.SetTitleY("#events");
    draw.SetMaxY();
    draw.Draw(exp,"selmu_truemom",100,0,7000,"particle","accum_level[0][2]>6 && selmu_ecal_containment==0","E1","POTNORM",-1,true);
    HA_selmu_truemom_ecal_not_contained->SaveAs("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/Plots/CutValues/PNGs/HA/HA_selmu_truemom_ECal_not_contained.png");



  // -------------------------------------------------
  // HA: selmu_ecal_mippion for ECal contained tracks
  // -------------------------------------------------
  TCanvas *HA_selmu_ecal_mippion_contained = new TCanvas("HA_selmu_ecal_mippion_contained","Canvas",npix_x,npix_y);
    gStyle->SetOptStat(0);
    draw.SetLegendParam(0.22,0.90,0.40,0.91);
    draw.SetTitleX("#mu candidate ECal MIPPion [-]");
    draw.SetTitleY("#events");
    draw.SetMaxY();
    draw.Draw(exp,"selmu_ecal_mippion",60,-60,60,"particle","accum_level[0][2]>6 && selmu_ecal_containment==1","E1","POTNORM",-1,true);
    //draw.DrawCutLineVertical(1.20,true,"r");
    HA_selmu_ecal_mippion_contained->SaveAs("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/Plots/CutValues/PNGs/HA/HA_selmu_ecal_mippion_contained.png");

  // -------------------------------------------------
  // HA: selmu_ecal_mippion for ECal NOT contained tracks
  // -------------------------------------------------
  TCanvas *HA_selmu_ecal_mippion_not_contained = new TCanvas("HA_selmu_ecal_mippion_not_contained","Canvas",npix_x,npix_y);
    gStyle->SetOptStat(0);
    draw.SetLegendParam(0.22,0.90,0.40,0.91);
    draw.SetTitleX("#mu candidate ECal MIPPion [-]");
    draw.SetTitleY("#events");
    draw.SetMaxY();
    draw.Draw(exp,"selmu_ecal_mippion",60,-60,60,"particle","accum_level[0][2]>6 && selmu_ecal_containment==0","E1","POTNORM",-1,true);
    //draw.DrawCutLineVertical(1.20,true,"r");
    HA_selmu_ecal_mippion_not_contained->SaveAs("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/Plots/CutValues/PNGs/HA/HA_selmu_ecal_mippion_not_contained.png");

















/*
  TCanvas *c1 = new TCanvas("c1","Canvas",npix_x,npix_y);
    gStyle->SetOptStat(0);
    draw.SetMinY(0);
    draw.SetLogY(0);
    draw.SetLegendParam(0.25,0.90,0.43,0.91);
    draw.SetTitleX("ToF FGD1-TECal [ns]");
    draw.SetTitleY("#events");
    draw.Draw(exp,"selmu_ToF_tecal_fgd1",50,-25,25,"reaction","accum_level[][1]>4","E1","POTNORM",-1,true);
    draw.DrawCutLineVertical(0,true,"r");
    draw.DrawCutLineVertical(13,true,"l");
    //c1->SaveAs("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/Plots/PNGs/Selection/Bwd_ToF/TECal-FGD1.png");

  TCanvas *c2 = new TCanvas("c2","Canvas",npix_x,npix_y);
    gStyle->SetOptStat(0);
    draw.SetMinY(0);
    draw.SetLogY(0);
    draw.SetLegendParam(0.25,0.90,0.43,0.91);
    draw.SetTitleX("ToF FGD1-earliestECal [ns]");
    draw.SetTitleY("#events");
    draw.Draw(exp,"selmu_ToF_ecal_fgd1_other",100,-50,50,"reaction","accum_level[][1]>4","E1","POTNORM",-1,true);
    draw.DrawCutLineVertical(0,true,"r");
    draw.DrawCutLineVertical(25,true,"l");
    //c2->SaveAs("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/Plots/PNGs/Selection/Bwd_ToF/earliestECal-FGD1.png");

  // Number of nodes in ECal, FGD1, P0D and TPC
  // ------------------------------------------------
  TCanvas *c3 = new TCanvas("c3","Canvas",npix_x,npix_y);
    gStyle->SetOptStat(0);
    draw.SetMinY(0);
    draw.SetLogY(0);
    draw.SetLegendParam(0.72,0.90,0.90,0.91);
    draw.SetTitleX("ECal nodes");
    draw.SetTitleY("#events");
    draw.Draw(exp,"selmu_ecal_nnodes",35,0,35,"reaction","accum_level[][1]>4","E1","POTNORM",-1,true);
    //c3->SaveAs("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/Plots/PNGs/Selection/Bwd_nnodes/ECal_nodes.png");

  TCanvas *c4 = new TCanvas("c4","Canvas",npix_x,npix_y);
    gStyle->SetOptStat(0);
    draw.SetMinY(0);
    draw.SetLogY(0);
    draw.SetLegendParam(0.72,0.90,0.90,0.91);
    draw.SetTitleX("FGD1 nodes");
    draw.SetTitleY("#events");
    draw.Draw(exp,"selmu_fgd_nnodes",100,0,100,"reaction","accum_level[][1]>4","E1","POTNORM",-1,true);
    //c4->SaveAs("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/Plots/PNGs/Selection/Bwd_nnodes/FGD1_nodes.png");

  TCanvas *c5 = new TCanvas("c5","Canvas",npix_x,npix_y);
    gStyle->SetOptStat(0);
    draw.SetMinY(0);
    draw.SetLogY(0);
    draw.SetLegendParam(0.72,0.90,0.90,0.91);
    draw.SetTitleX("P0D nodes");
    draw.SetTitleY("#events");
    draw.Draw(exp,"selmu_pod_nnodes",85,0,85,"reaction","accum_level[][1]>4","E1","POTNORM",-1,true);
    draw.DrawCutLineVertical(40,true,"l");
    //c5->SaveAs("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/Plots/PNGs/Selection/Bwd_nnodes/P0D_nodes.png");

  TCanvas *c6 = new TCanvas("c6","Canvas",npix_x,npix_y);
    gStyle->SetOptStat(0);
    draw.SetMinY(0);
    draw.SetLogY(0);
    draw.SetLegendParam(0.72,0.90,0.90,0.91);
    draw.SetTitleX("TPC nodes");
    draw.SetTitleY("#events");
    draw.Draw(exp,"selmu_tpc_nnodes",50,0,200,"reaction","accum_level[][1]>4","E1","POTNORM",-1,true);
    //c6->SaveAs("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/Plots/PNGs/Selection/Bwd_nnodes/TPC_nodes.png");

  // FGD layer of reconstructed track start position
  // ------------------------------------------------
  TCanvas *c7 = new TCanvas("c7","Canvas",npix_x,npix_y);
    gStyle->SetOptStat(0);
    draw.SetMinY(0);
    draw.SetLogY(0);
    draw.SetLegendParam(0.72,0.90,0.90,0.91);
    draw.SetTitleX("Start position [FGD1 layer]");
    draw.SetTitleY("#events");
    draw.Draw(exp,"selmu_fgdlayer",50,0,50,"reaction","accum_level[][1]>4","E1","POTNORM",-1,true);
    //c7->SaveAs("/home/rberner/nd280work/antiNumuCCInc4PiAnalysis/Plots/PNGs/Selection/Bwd_FGD1_layer/FGD1_layer.png");
*/




  // Fiducial volume definition:
  // ----------------------------
  //selmu_truepos[0]>-874.51 && selmu_truepos[0]<874.51 && selmu_truepos[1]>-819.51 && selmu_truepos[1]<929.51 && selmu_truepos[2]>116.045 && selmu_truepos[2]<446.955








  /*draw.PrintEventNumbers(*mc_magnet_run5_01,"accum_level[][1]>4 && reaction==7","events_01.list"); // && reaction==0
  draw.PrintEventNumbers(*mc_magnet_run5_02,"accum_level[][1]>4 && reaction==7","events_02.list");
  draw.PrintEventNumbers(*mc_magnet_run6_01,"accum_level[][1]>4 && reaction==7","events_03.list");
  draw.PrintEventNumbers(*mc_magnet_run6_02,"accum_level[][1]>4 && reaction==7","events_04.list");
  draw.PrintEventNumbers(*mc_magnet_run6_03,"accum_level[][1]>4 && reaction==7","events_05.list");
  draw.PrintEventNumbers(*mc_magnet_run6_04,"accum_level[][1]>4 && reaction==7","events_06.list");
  draw.PrintEventNumbers(*mc_sand_01,"accum_level[][1]>4 && reaction==7","events_07.list");
  draw.PrintEventNumbers(*mc_sand_02,"accum_level[][1]>4 && reaction==7","events_08.list");*/

 /* draw.PrintEventNumbers(*mc_magnet_run5_02,"cut2[][1]==1 && cut1[][1]==1 && cut0[][1]==1 && (reaction==0 || reaction==1 || reaction==2 || reaction==3 || reaction==9)","events_02.list");
  draw.PrintEventNumbers(*mc_magnet_run6_01,"cut2[][1]==1 && cut1[][1]==1 && cut0[][1]==1 && (reaction==0 || reaction==1 || reaction==2 || reaction==3 || reaction==9)","events_03.list");
  draw.PrintEventNumbers(*mc_magnet_run6_02,"cut2[][1]==1 && cut1[][1]==1 && cut0[][1]==1 && (reaction==0 || reaction==1 || reaction==2 || reaction==3 || reaction==9)","events_04.list");
  draw.PrintEventNumbers(*mc_magnet_run6_03,"cut2[][1]==1 && cut1[][1]==1 && cut0[][1]==1 && (reaction==0 || reaction==1 || reaction==2 || reaction==3 || reaction==9)","events_05.list");
  draw.PrintEventNumbers(*mc_magnet_run6_04,"cut2[][1]==1 && cut1[][1]==1 && cut0[][1]==1 && (reaction==0 || reaction==1 || reaction==2 || reaction==3 || reaction==9)","events_06.list");
  draw.PrintEventNumbers(*mc_sand_01,"cut2[][1]==1 && cut1[][1]==1 && cut0[][1]==1 && (reaction==0 || reaction==1 || reaction==2 || reaction==3 || reaction==9)","events_07.list");
  draw.PrintEventNumbers(*mc_sand_02,"cut2[][1]==1 && cut1[][1]==1 && cut0[][1]==1 && (reaction==0 || reaction==1 || reaction==2 || reaction==3 || reaction==9)","events_08.list");*/
  //draw.PrintEventNumbers(*test_02,"accum_level[][1]>3 && accum_level[][1]>3","events_02.list"); // && !cut2
  //draw.PrintEventNumbers(*test_03,"accum_level[][1]>3 && accum_level[][1]>3","events_03.list");
  //draw.PrintEventNumbers(*test_04,"accum_level[][1]>3 && accum_level[][1]>3","events_04.list");
  //draw.PrintEventNumbers(*test_05,"accum_level[][1]>3 && accum_level[][1]>3","events_05.list");
  //draw.PrintEventNumbers(*test_06,"accum_level[][1]>3 && accum_level[][1]>3","events_06.list");
  //draw.PrintEventNumbers(*mc_magnet_run6_06_of_06,"accum_level[][1]>3","events.list");
  //draw.PrintEventNumbers(*mc_sand,"accum_level[][1]>2","events.list");

}
