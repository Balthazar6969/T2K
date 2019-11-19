// To run:
// .L NumuCCFGD2BackwardVertexMigrationMakeClass.C
// .L FluxTuning.C
// .L fit_vertex_backward_migrations.C
// fit(options)

#include "FluxTuning.h"
#include "NumuCCFGD2BackwardVertexMigrationMakeClass.h"

// Histogram templates
TH1F* h1NonMig;
TH1F* h1Mig;

Double_t fit_function_1var(Double_t *x, Double_t *par){
  // This is the function that you are fitting:
  // F_nomig + beta*F_mig = 1
  // For every point in the 2D histogram 
  // par is beta -> root doesn't like it if it's not an array
  Double_t xx = x[0];
  Double_t normalisation = h1NonMig->Integral("") + par[0]*h1Mig->Integral("");
  Int_t binx = h1NonMig->GetXaxis()->FindBin(xx);
  Double_t noMig = h1NonMig->GetBinContent(binx);
  Double_t Mig = par[0]*h1Mig->GetBinContent(binx);

  return (noMig + Mig);

}

void fit(int fit = 1){

  const int n_bins_charge = 8;
  double bins_charge[n_bins_charge] = {0, 30, 60, 100, 150, 200, 300, 600};
  const int n_bins_dist = 10;
  double bins_dist[n_bins_dist] = {0, 1, 2, 3, 4, 5, 7, 9, 15, 20};


  run("selmu_hit1_charge",
      n_bins_charge, bins_charge,
      "Charge_{1st hit}", "charge1",
      fit); 

  run("distance_track_hit1",
      n_bins_dist, bins_dist,
      "line-line distance for hit1 (mm)", "distance1",
      fit); 

}


void run(std::string variable, int nbins, double* bins,
	 std::string niceVar, std::string saveVar, int fittype=1){


  ofstream output;
  output.open("fit_results.txt", ios::app);

  // Determine max and min bounds of histogram
  int min = bins[0];
  int max = bins[nbins-1];
  std::stringstream sstream_min; sstream_min << min; std::string stringmin = sstream_min.str();
  std::stringstream sstream_max; sstream_max << max; std::string stringmax = sstream_max.str();
  
  gStyle->SetOptStat(0);
  

  // Get relevant files
  std::string path = "/data/t2k/short/t2k/highland2output/";
  std::string mc_filename = "";
  std::string data_filename = "";

  if(fittype == 0){
    mc_filename   = path + "highland2output_neut_water_magnet_run2_v1r11highland2_ALL.root";
    data_filename = path + "highland2output_neut_run2_data_v1r9highland2.root"; 
  }
  else if(fittype == 1){
    mc_filename   = path + "highland2output_neut_water_magnet_run2_v1r11highland2_ALL.root";
    data_filename = path + "highland2output_neut_water_magnet_run2_v1r11highland2_ALL.root";
  }
  else if(fittype == 2){
    mc_filename   = path + "highland2output_neut_water_magnet_run2_v1r11highland2_ALL.root";
    data_filename = path + "highland2output_neut_air_magnet_run2_v1r11highland2_ALL.root";
  }
  else if(fittype == 3){
    mc_filename   = path + "highland2output_neut_water_magnet_run2_v1r11highland2_ALL.root";
    data_filename = path + "highland2output_neut_air_magnet_run3_v1r11highland2_ALL.root";
  }
  else if(fittype == 4){
    mc_filename   = path + "highland2output_neut_water_magnet_run2_v1r11highland2_ALL.root";
    data_filename = path + "highland2output_neut_air_magnet_run4_v1r11highland2_ALL.root";
  }
  else if(fittype == 5){
    mc_filename   = path + "highland2output_neut_water_magnet_run2_v1r11highland2_ALL.root";
    data_filename = path + "highland2output_genie_air_magnet_run2_v1r11highland2_ALL.root";
  }
  else if(fittype == 6){
    mc_filename   = path + "highland2output_neut_water_magnet_run2_v1r11highland2_ALL.root";
    data_filename = path + "highland2output_genie_water_magnet_run2_v1r11highland2_ALL.root";
  }
  else if(fittype == 7){
    mc_filename   = path + "highland2output_antineut_water_magnet_run5_v1r11highland2_ALL.root";
    data_filename = path + "highland2output_antineut_water_magnet_run5_v1r11highland2_ALL.root";
  }
  else{std::cout<<"ERROR: must specify a valid fit type"; exit(0);}

  std::cout<<"\nUsing the following files:"<<std::endl;
  std::cout<<"MC:   "<<mc_filename<<std::endl;
  std::cout<<"DATA: "<<data_filename<<"\n"<<std::endl;
  output<<"\n************************************"<<std::endl;
  output<<"MC:       "<<mc_filename<<std::endl;
  output<<"DATA:     "<<data_filename<<"\n"<<std::endl;
  output<<"VARIABLE: " << niceVar << std::endl;

  // Make make class objects for the data and the MC
  TChain* chain_mc   = new TChain("default", "");
  TChain* chain_data = new TChain("default", "");
  chain_mc  ->AddFile(mc_filename.c_str());
  chain_data->AddFile(data_filename.c_str());
  NumuCCFGD2BackwardVertexMigrationMakeClass mc(chain_mc);
  NumuCCFGD2BackwardVertexMigrationMakeClass data(chain_data);


  // Get POT (make DrawingTools objects)
  // to scale MC so that it matches data
  DrawingTools draw(mc_filename);
  DataSample ds_mc(mc_filename);
  DataSample ds_data(data_filename);
  double pot_ratio = draw.GetPOTRatio(ds_data,ds_mc);
  ds_mc.DumpPOT();
  ds_data.DumpPOT();
  std::cout<<"pot_ratio = "<<pot_ratio<<std::endl;
  output<<"MC POT    = "<<draw.GetGoodPOT(ds_mc)<<std::endl;
  output<<"DATA POT  = "<<draw.GetGoodPOT(ds_data)<<std::endl;
  output<<"POT RATIO = "<<pot_ratio<<std::endl;
  

  // Use DrawingTools to make plots showing the fraction of events
  // being migrated, etc
  std::string dt_var = "overunderflow(" + variable + "," + stringmin + "," + stringmax + ")";

  const int ncat_new = 8;
  int leg_new[ncat_new] = {0,1,2,3,4,5,6,-1};
  int col_new[ncat_new] = {1,2,3,4,5,6,7,8};
  std::string name_new[ncat_new] = {"Water match","Water forward","Water backward","Scint match","Scint forward","Scint backward","OOFV","Other"};
  std::string var_new = "fgd2_new_category(selmu_fgdlayer, truemu_fgdlayer, truemu_fgdmoduletype, selmu_fgdmoduletype, selmu_pos[2], selmu_truepos[2])";
  draw.ChangeTrackCategory(var_new, ncat_new , name_new, leg_new, col_new);

  std::string dt_cut = "(accum_level[0][3]>5) && (selmu_hit1_charge != -999) && (selmu_hit2_charge != -999)";

  TCanvas* c1 = new TCanvas();
  draw.SetLegendPos("c");
  draw.Draw(ds_data, ds_mc, dt_var, nbins-1, bins, var_new, dt_cut);
  std::stringstream sstream; sstream << fittype; std::string stringfittype = sstream.str();
  std::string saveas = "plots/fitresults/1D_fittype" + stringfittype + "_" + saveVar + "_highland2plot.pdf";
  c1->SaveAs(saveas.c_str());

  
  // Print purity info:
  if(fittype != 7){draw.PrintPurities(ds_mc, var_new, "(accum_level[0][3]>5) && (selmu_hit1_charge != -999) && (selmu_hit2_charge != -999)");}
  else{
    var_new = "fgd2_new_category(selmu_fgdlayer, truemu_fgdlayer, truemu_fgdmoduletype, selmu_fgdmoduletype, selmu_pos[2], selmu_truepos[2])";
    draw.ChangeTrackCategory(var_new, ncat_new , name_new, leg_new, col_new);
    draw.PrintPurities(ds_mc, var_new, "(accum_level[0][0]>6) && (selmu_hit1_charge != -999) && (selmu_hit2_charge != -999)"); 
    return;
  }


  // Histogram templates
  h1NonMig          = new TH1F("h1NonMig",    "", nbins-1, bins);    h1NonMig->Sumw2();   
  h1Mig             = new TH1F("h1Mig",       "", nbins-1, bins);       h1Mig->Sumw2();      
  TH1F* h1Data      = new TH1F("h1Data",      "", nbins-1, bins);      h1Data->Sumw2();     
  TH1F* h1MigCheck  = new TH1F("h1MigCheck",  "", nbins-1, bins);  h1MigCheck->Sumw2(); 
  TH1F* h1MCNoScale = new TH1F("h1MCNoScale", "", nbins-1, bins); h1MCNoScale->Sumw2(); // to be used in plotting


  // Make a flux tuning object so can apply the flux tuning
  FluxTuning* flux_tuning = new FluxTuning("2",false);
  
  // Fill the MC histograms
  for(int i = 0; i < mc.fChain->GetEntries(); i++){

    mc.fChain->GetEntry(i);

    // Check event passes cut
    if(mc.accum_level[0][3] > 5 && (mc.selmu_hit1_charge != -999) && (mc.selmu_hit2_charge != -999)){}
    else{continue;}

    // Get the variable you're fitting
    double var = -9999;
    if     (variable == "selmu_hit1_charge")  {var = overunderflow(mc.selmu_hit1_charge, min, max);}
    else if(variable == "distance_track_hit1"){var = overunderflow(mc.distance_track_hit1, min, max);}
    else{std::cout<<"ERROR. Check variable fitting."<<std::endl; exit(0);}

    // Weights
    double weight = flux_tuning->GetFluxTuningWeight(mc.nu_pdg, mc.nu_trueE);
    weight *= pot_ratio;
    // Bin weight (to match what is done in highland2) - only relevant if comparing MC and real data
    // if(fittype == 0){
    //  weight *= GetBinWeight(nbins, bins, var);
    // }
    
    // Get category for event (i.e. backward mig, forward mig, match)
    int cat = fgd2_new_category(mc.selmu_fgdlayer, mc.truemu_fgdlayer, mc.truemu_fgdmoduletype, mc.selmu_fgdmoduletype, mc.selmu_pos[2], mc.selmu_truepos[2]);

    // Migration (backward)
    if(cat == 2 || cat == 5){
      h1Mig->Fill(var, weight);
    }
    
    // No migration
    if(cat != 2 && cat != 5){
      h1NonMig->Fill(var, weight);
    }

    // To be used later in plotting
    h1MCNoScale->Fill(var, weight);

  }


  // Fill the data histogram
  for(int i = 0; i < data.fChain->GetEntries(); i++){

    data.fChain->GetEntry(i);

    // Check event passes cut
    if(data.accum_level[0][3] > 5 && (data.selmu_hit1_charge != -999) && (data.selmu_hit2_charge != -999)){}
    else{continue;}

    // Get the variable you're fitting
    double var = -9999;
    if     (variable == "selmu_hit1_charge")  {var = overunderflow(data.selmu_hit1_charge, min, max);}
    else if(variable == "distance_track_hit1"){var = overunderflow(data.distance_track_hit1, min, max);}
    else{std::cout<<"ERROR. Check variable fitting."<<std::endl; exit(0);}

    // If this is fake data, apply the flux tuning
    double weight = 1.0;
    if(fittype != 0 && data.nu_pdg != -999 && data.nu_trueE != -999){
      weight = flux_tuning->GetFluxTuningWeight(data.nu_pdg, data.nu_trueE);
    }
    // Bin weight (to match what is done in highland2) - only relevant if comparing MC and real data
    // if(fittype == 0){
    //  weight *= GetBinWeight(nbins, bins, var);
    // }

    // Fill histogram
    h1Data->Fill(var, weight);

    // If the data is actually MC, see what the fraction of migrated events actually is
    if(fittype != 0){
       // Get category for event (i.e. backward mig, forward mig, match)
       int cat = fgd2_new_category(data.selmu_fgdlayer, data.truemu_fgdlayer, data.truemu_fgdmoduletype, data.selmu_fgdmoduletype, data.selmu_pos[2], data.selmu_truepos[2]);
       if(cat == 2 || cat == 5){
	 h1MigCheck->Fill(var, weight);
       }
    }

  }


  // Get integral of histograms so can normalise area
  double integralNonMig   = h1NonMig->Integral("");
  double integralMig      = h1Mig->Integral("");
  double integralTotal    = h1MCNoScale->Integral("");//integralNonMig + integralMig;
  double integralMigCheck = h1MigCheck->Integral("");
  double integralData     = h1Data->Integral("");



  // Now do the fitting.
  // Definition of function:
  //    no_mig + beta*backward_mig = 1
  TF1 *func = new TF1("func", fit_function_1var, min, max, 1);
  func->SetParameter(0, 1.0);
  h1Data->Fit("func", "E0");
  
  double par0    = func->GetParameter(0); //par0 is beta
  double par0err = func->GetParError(0);
  double chi2    = func->GetChisquare();
  double ndf     = func->GetNDF()*1.;
  
  // Propagate the errors and calculate the fraction of migrated events
  double nMig    = h1Mig->Integral("");
  double nNonMig = h1NonMig->Integral("");
  // Rename some things for ease:
  //   f = beta*A / (beta*A + B)
  //     where beta = par0, A = nMig, B = nNonMig
  // error_f = ... do the partial derivatives 
  //
  double beta = par0;
  double error_beta = par0err;
  double A = nMig;
  double error_A = sqrt(A);
  double B = nNonMig;
  double error_B = sqrt(B);

  double frac = beta*A / (beta*A + B);

  double temp_denom = (beta*A + B) * (beta*A + B);

  double error_1 = (A*B / temp_denom) * error_beta;
  double error_2 = (beta*B / temp_denom) * error_A;
  double error_3 = (-1*beta*A / temp_denom) * error_B;
  
  double error_frac = sqrt(error_1*error_1 + error_2*error_2 + error_3*error_3);



  // Print results to screen
  std::cout << std::setprecision(4);
  std::cout<<"\n************************************"<<std::endl;
  std::cout<< "Truth info for: " << niceVar << std::endl;
  std::cout<< "NEUT all back migration   " << integralMig/integralTotal << std::endl;
  if(fittype != 0){
    std::cout << "(Using fake data => true backward migration of data " << integralMigCheck/integralData << ")" <<std::endl;
  }

  std::cout << "\nFitting info:"<<std::endl;
  std::cout << "Beta : " <<  (par0) << " +- " << par0err << " ( " << par0err*100./(par0) << " % )" << std::endl;
  if(ndf != 0){
    std::cout << "Chi2 : " << chi2 << " NDF : " << ndf << " chi2/ndf = " << chi2/ndf << std::endl;
  }

  std::cout<<"\nCalculated info:"<<std::endl;
  std::cout<<"Frac migrated events = "<<frac<<" +- "<<error_frac<<" ("<<100.0*error_frac/frac<<"%)"<<std::endl;
  if(fittype != 0){
    std::cout<<" -> Diff between (fake data) true and fitted = "
	     <<100.0*fabs((integralMigCheck/integralData) - frac)/(integralMigCheck/integralData)<<"%"<<std::endl;
  }
  else{
    std::cout<<" -> Diff between (MC) true and fitted = "
	     <<100.0*fabs((integralMig/integralTotal) - frac)/(integralMig/integralTotal)<<"%"<<std::endl;
  }
  std::cout<<"************************************\n"<<std::endl;

  // Write results to file
  output<<"\nNEUT all back migration   " << integralMig/integralTotal << std::endl;
  if(fittype != 0){
    output<<"(Using fake data => true backward migration of data "<<integralMigCheck/integralData<<")"<<std::endl;
  }

  output<<"\nFitting info:"<<std::endl;
  output<< "Beta : " <<  (par0) << " +- " << par0err << " ( " << par0err*100./(par0) << " % )" << std::endl;
  if(ndf != 0){
    output<< "Chi2 : " << chi2 << " NDF : " << ndf << " chi2/ndf = " << chi2/ndf << std::endl;
  }

  output<<"\nCalculated info:"<<std::endl;
  output<<"Frac migrated events = "<<frac<<" +- "<<error_frac<<" ("<<100.0*error_frac/frac<<"%)"<<std::endl;
  if(fittype != 0){
    output<<" -> Diff between (fake data) true and fitted = "
	  <<100.0*fabs((integralMigCheck/integralData) - frac)/(integralMigCheck/integralData)<<"%"<<std::endl;
  }
  else{
    output<<" -> Diff between (MC) true and fitted = "
	  <<100.0*fabs((integralMig/integralTotal) - frac)/(integralMig/integralTotal)<<"%"<<std::endl;
  }
  output<<"************************************\n"<<std::endl;



  // Make plots to show distributions pre and post fit

  // Now, rescale the MC based on the fit results
  // First, calculate the difference between the true and the fitted fraction
  double scale_mig    = frac     / (integralMig/integralTotal);
  double scale_nonmig = (1-frac) / (integralNonMig/integralTotal);

  h1Mig->Scale(scale_mig);
  h1NonMig->Scale(scale_nonmig);

  // Add MC histograms together before plotting
  h1NonMig->Add(h1Mig);


  // Get errors
  for(int i = 1; i < nbins; i++){
    double err1 = h1NonMig->GetBinError(i);
    double err2 = h1Mig->GetBinContent(i)*error_frac / frac;
    h1NonMig->SetBinError(i, sqrt(err1*err1 + err2*err2));
  }

   
  // Make pretty
  h1Data->SetMarkerStyle(20);
  h1NonMig->SetFillColor(kRed);
  h1NonMig->SetFillStyle(3001);
  h1MCNoScale->SetFillColor(kBlue);
  h1MCNoScale->SetFillStyle(3001);

  //h1NonMig->GetYaxis()->SetTitle("Area normalised");
  h1NonMig->GetXaxis()->SetTitle(niceVar.c_str());
  
  double max_histo = h1Data->GetMaximum()*1.2;
  h1NonMig->SetMaximum(max_histo);
   
    
  // Legend
  TLegend *leg = new TLegend(0.2, 0.8, 0.7, 0.9);
  leg->SetFillColor(kWhite);
  leg->AddEntry(h1MCNoScale, "MC without fitted fraction scaling", "f");
  leg->AddEntry(h1NonMig, "MC scaled using fitted fraction of migrations", "f");
  leg->AddEntry(h1Data, "Data", "lp");
  
  // Draw
  h1NonMig->Draw("e2");
  h1MCNoScale->Draw("e2 same");
  h1Data->Draw("e1 same");
  leg->Draw();


  // Save plot
  saveas = "1D_fittype" + stringfittype + "_" + saveVar + "_fitresultsplot.pdf";
  c1->SaveAs(saveas.c_str());


  // Clean up
  delete chain_mc;
  delete chain_data;
  delete h1Data;
  delete h1MigCheck;
  delete h1MCNoScale;
  delete func;
  delete leg;
  delete c1;
          
}


double GetBinWeight(const int nbins, double* bins, double value){
 
  // Determine the bin width and weight
  std::vector<double> weights;
  for(int i = 0; i < nbins-1; i++){
    double diff = bins[i+1] - bins[i];
    weights.push_back((double) 1.0 / diff);
  }

  // Determine which bin value is in
  int bin_of_interest = 0;
  for(int i = 0; i < nbins-1; i++){
    double content0 = bins[i];
    double content1 = bins[i+1];
    if(value >= content0 && value <= content1){bin_of_interest = i; break;}
  }

  double weight = weights[bin_of_interest];
  return weight;

}

double overunderflow(double var, double min, double max){

  if (min==0 && var<0) return 0.000000001;
  if (var < min) return min*0.99;
  else if (var>max) return max*0.99;

  return var;

}

int fgd2_new_category(int reco_FGDbar, int true_FGDbar, int true_FGDModuletype, int reco_FGDModuletype, double reco_pos, double true_pos){

  int value = -1;

  // Definitions:
  //  * Water and 1st half of XY module = water enhanced (0,2)
  //  * 2nd half of XY module = carbon enhanced (1)

  if(true_FGDModuletype == 7){value = 6;} // OOFV
  else {

    // 1st half of XY module
    if(true_FGDModuletype == 0){

      if     (reco_FGDModuletype == 0){value = 0;}
      else if(reco_FGDModuletype == 2){value = 0;}
      else if(reco_FGDModuletype == 1){ // Migration
        if     (reco_FGDbar > true_FGDbar){value = 1;} // Forward migration
        else if(reco_FGDbar < true_FGDbar){value = 2;} // Backward migration

      }
    }
    // 2nd half of XY module
    else if(true_FGDModuletype == 1){
      if(reco_FGDModuletype == 1){value = 3;}
      else if(reco_FGDModuletype == 0){ // Migration
	if     (reco_FGDbar > true_FGDbar){value = 4;} // Forward migration
	else if(reco_FGDbar < true_FGDbar){value = 5;} // Backward migration
      } 
      else if(reco_FGDModuletype == 2){ // Migration to water
	if     (reco_pos > true_pos){value = 4;} // Forward migration
	else if(reco_pos < true_pos){value = 5;} // Backward migration
      } 
    }
    // water
    else if(true_FGDModuletype == 2){
      if     (reco_FGDModuletype == 0){value = 0;}
      else if(reco_FGDModuletype == 2){value = 0;}
      else if(reco_FGDModuletype == 1){ // Migration
	if     (reco_pos > true_pos){value = 1;} // Forward migration
	else if(reco_pos < true_pos){value = 2;} // Backward migration
      } 
    }
  }

  return value;
}
