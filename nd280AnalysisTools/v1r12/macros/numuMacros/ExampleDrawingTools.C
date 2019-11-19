{


  std::string dir = "/hep/T2K/repo/T2K/analysis/prod4C/";

  // compile the code, or load if it already compiled

  gSystem->CompileMacro("TrackCategory.h","k"); 
  gSystem->CompileMacro("DrawingTools.C","k"); 
  gSystem->CompileMacro("TreeToolsBase.h","k"); 
  gSystem->CompileMacro("oaAnalysisTreeDefinition.C","k"); 
  gSystem->CompileMacro("oaAnalysisTreeTools.C","k"); 
  gSystem->CompileMacro("NumuFlatTreeDefinition.C","k"); 
  gSystem->CompileMacro("NumuFlatTreeTools.C","k"); 
  gSystem->CompileMacro("SystematicsAnalysis.C","k"); 
  gSystem->CompileMacro("AnalysisBase.C","k"); 
  gSystem->CompileMacro("MyAnalysis.C","k"); 


  // create instance of MyAnalysis for data and MC
  MyAnalysis mc2((dir+"NeutMCRun2.root").c_str()); 
  MyAnalysis data2((dir+"SpillRun2.root").c_str());   

  // select the configurations we want to run
  mc2.DisableAllConfigurations();
  data2.DisableAllConfigurations();
  
  mc2.EnableConfiguration("default");
  mc2.EnableConfiguration("mom_scale_syst");

  data2.EnableConfiguration("default");
  data2.EnableConfiguration("mom_scale_syst");

  // loop over events (50000 is the number of events to run. By default it runs all)
  data2.Loop(50000);
  mc2.Loop(50000);

  // compute the normalisation factor
  double npot_data_Run2 = 4.479e+19; 
  double npot_neut_Run2 = 3.25e+20; // 650 files (5e17 each)
  double norm2 = npot_data_Run2/npot_neut_Run2; // 0.4721;
  cout << "Run_2 normalisation = " << norm2 << std::endl;


  // use the default configuration (a single analysis)
  mc2.SetCurrentConfiguration("default");
  data2.SetCurrentConfiguration("default");

  // This will be used for all plots util the options are changed
  mc2.SetTitleX("muon momentum candidate (GeV/c)");
  mc2.SetTitleY("entries");
  data2.SetTitleX("muon momentum candidate (GeV/c)");
  data2.SetTitleY("entries");

  //------------ Single variable plots ------------------

  // Plot muon candidate momentum for MC only. Color codes for particle types. Uniform binning
  mc2.Draw("mu_mom",50,0,5000,"particle","accum_level>6");
  gStyle->SetOptStat(0);getchar();

  // Plot muon candidate momentum for MC only. Color codes for particle types. variable binning
  double pbins[11]={0,200,300,400,500,600,800,1000,2000,3000,5000};
  mc2.Draw("mu_mom",10,pbins,"particle","accum_level>6");
  gStyle->SetOptStat(0);getchar();


  // Plot muon candidate momentum for data only
  data2.Draw("mu_mom",10,pbins,"all","accum_level>6");
  gStyle->SetOptStat(0);getchar();

  // Plot muon candidate momentum for data and MC . Color codes for particle types
  mc2.Draw(data2,mc2,"mu_mom",10,pbins,"particle","accum_level>6",norm2,"e1");
  gStyle->SetOptStat(0);getchar();

  //------------ Data/MC ratio ------------------

  mc2.SetTitleY("data/MC ratio");

  // Plot data/MC ratio for muon candidate momentum 
  mc2.DrawRatio(data2,mc2,"mu_mom",10,pbins,"accum_level>6",norm2,"e1");
  gStyle->SetOptStat(0);getchar();


  //------------ Efficiencies, purities, ratios and significance  ------------------

  // Plot MC efficiency for the PID and veto cuts as a function of muon candidate momentum 
  mc2.SetTitleY("efficiency");
  mc2.DrawEff("mu_mom",10,pbins,"accum_level>6","accum_level>5","e1","PID cut");
  mc2.DrawEff("mu_mom",10,pbins,"accum_level>6","accum_level>4","e1 same","veto cut");
  gStyle->SetOptStat(0);getchar();

  // Plot MC CC purity for the PID and veto cuts as a function of muon candidate momentum 
  mc2.SetTitleY("numu CC purity");
  mc2.DrawEff("mu_mom",10,pbins,"reaction==0","accum_level>5","e1","PID cut");
  mc2.DrawEff("mu_mom",10,pbins,"reaction==0","accum_level>4","e1 same","veto cut");
  gStyle->SetOptStat(0);getchar();

  // Plot significance for CC selection as a function of muon candidate momentum for several syst error on bg
  mc2.SetTitleY("numu CC significance");
  mc2.DrawSignificance("mu_mom",10,pbins,"reaction==0","accum_level>6",norm2,0.1,"e1","syst 10%");
  mc2.DrawSignificance("mu_mom",10,pbins,"reaction==0","accum_level>6",norm2,0.2,"e1 same","syst 20%");
  gStyle->SetOptStat(0);getchar();

  // Plot data ratio between FGD1 and FGD2
  data2.SetTitleY("FGD1/FGD2 ratio");
  data2.DrawRatio("mu_mom",10,pbins,"accum_level>6 && mu_vfgd==1","accum_level>6 && mu_vfgd==2","e1");
  gStyle->SetOptStat(0);getchar();

  //------------ Systematic errors ------------------

  // use the "mom_scale_syst" configuration (50 analyses with different momentum scales). Only for data
  data2.SetCurrentConfiguration("mom_scale_syst");

  // Draw the distribution of the number of selected events in the bin 500-1000 for the 50 analyses
  data2.SetTitleX("number of events passing all cuts in the bin 500-1000 MeV/c");
  data2.SetTitleY("entries");
  data2.DrawEntries("accum_level>6 && mu_mom>500 && mu_mom<1000");
  gStyle->SetOptStat(0);getchar();


  // Plot number of events passing all cuts in the bin 500-1000 as a function of the analysis number
  data2.SetTitleX("analysis number");
  data2.SetTitleY("number of events passing all cuts in the bin 500-1000 MeV/c");
  data2.Draw("ana",50,0,50,"all","accum_level>6 && mu_mom>500 && mu_mom<1000");
  gStyle->SetOptStat(0);getchar();

  // Plot muon candidate momentum with Statistical errors only
  data2.SetTitleX("muon candidate momentum (GeV/c)");
  data2.SetTitleY("entries");
  data2.Draw("mu_mom",10,pbins,"all","accum_level>6","e1 ST");
  gStyle->SetOptStat(0);getchar();

  // Plot muon candidate momentum with Systematic errors only (computed as rms)
  data2.Draw("mu_mom",10,pbins,"all","accum_level>6","e1 SYS");
  gStyle->SetOptStat(0);getchar();

  // Plot muon candidate momentum with Systematic errors only (computed as dispersion max-min of the 50 analyses)
  data2.Draw("mu_mom",10,pbins,"all","accum_level>6","e1 DIS");
  gStyle->SetOptStat(0);getchar();

  // Plot muon candidate momentum with statistical and systematic errors (in quadrature)
  data2.Draw("mu_mom",10,pbins,"all","accum_level>6","e1 ST SYS");
  gStyle->SetOptStat(0);getchar();

  // Plot data/MC comparison of muon candidate momentum with statistical and systematic errors (only for data)
  // Colors for reaction types
  data2.Draw(data2,mc2,"mu_mom",10,pbins,"reaction","accum_level>6",norm2,"e1 ST SYS");
  gStyle->SetOptStat(0);getchar();

  // Plot data/MC ratio for muon candidate momentum with stat and syst errors 
  data2.SetTitleY("data/MC ratio");
  data2.DrawRatio(data2,mc2,"mu_mom",10,pbins,"accum_level>6",norm2,"e1 ST SYS");
  gStyle->SetOptStat(0);getchar();

  // Plot data/MC ratio for muon candidate momentum with stat and syst errors (separately and together)
  data2.SetTitleY("data/MC ratio");
  data2.DrawRatio(data2,mc2,"mu_mom",10,pbins,"accum_level>6",norm2,"e1 ST SYS",  "stat+syst");
  data2.DrawRatio(data2,mc2,"mu_mom",10,pbins,"accum_level>6",norm2,"e1 ST same", "stat only");
  data2.DrawRatio(data2,mc2,"mu_mom",10,pbins,"accum_level>6",norm2,"e1 SYS same","syst only");
  gStyle->SetOptStat(0);getchar();



}
