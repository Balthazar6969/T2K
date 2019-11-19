#include "DrawingToolsNueCC.hxx"
#include <iomanip>
#include <iostream>
#include <fstream>

#include "TF1.h"
#include "TMultiGraph.h"
#include "TGraphErrors.h"
#include "TSystem.h"

//********************************************************************
DrawingToolsNueCC::DrawingToolsNueCC(const string& file, bool useT2Kstyle):DrawingTools(file,useT2Kstyle){
//********************************************************************

  _treeForSystErrors = NULL;
}

//********************************************************************
DrawingToolsNueCC::DrawingToolsNueCC(Experiment& exp, bool useT2Kstyle):DrawingTools(exp.GetFilePath(),useT2Kstyle){
//********************************************************************

  _treeForSystErrors = NULL;
}

// ************************************************************************************************************
// Useful tools.
// ************************************************************************************************************

// Calculate the uncertainty based on Binomial statistics.
double GetBinomialUncertainty(double numer, double denom)
{
	if(denom == 0) return 0;

	double frac = numer/denom;
	return sqrt(frac*(1 - frac)/denom);
}

// Calculate the uncertainty based on Poisson statistics.
double GetPoissonUncertainty(double x, double dx, double y, double dy, double z = 0, double dz = 0)
{
	if(dx == 0 || dy == 0) return 0;
	if(dz == 0) return sqrt(((dx*dx)/(x*x)) + ((dy*dy)/(y*y)));
	return sqrt(((dx*dx)/(x*x)) + ((dy*dy)/(y*y)) + ((dz*dz)/(z*z)));
}

// Fitting functions. Only one so far!
double Line(Double_t *v, Double_t *par)
{
	Double_t fitval = par[0] + v[0]*par[1];
	return fitval;
}

// ************************************************************************************************************
// Getting and drawing methods.
// ************************************************************************************************************

void DrawingToolsNueCC::ProcessPlot(TH1 *hist, const string opt, const string leg)
{

	bool same = (drawUtils::ToUpper(opt).find("SAME") != string::npos);

	hist->GetXaxis()->SetTitle(_titleX.c_str());
        hist->GetYaxis()->SetTitle(_titleY.c_str());
        hist->SetTitle(_title.c_str());

	if(same) _same_level++;
	else _same_level = 0;

	hist->SetLineColor(_auto_colors[_same_level]);
	hist->SetFillColor(_auto_colors[_same_level]);
	hist->SetMarkerColor(_auto_colors[_same_level]);
	hist->SetMarkerStyle(_auto_markers[_same_level]);

	hist->Draw(opt.c_str());

	if(leg != "")
        {
		if(!same) CreateLegend();
                _legends.back()->AddEntry(hist, leg.c_str(), "LE1P");
                _legends.back()->Draw();
        }
}

void DrawingToolsNueCC::ProcessPlot(TH2 *hist, const string opt, const string leg)
{

	bool same = (drawUtils::ToUpper(opt).find("SAME") != string::npos);

        hist->GetXaxis()->SetTitle(_titleX.c_str());
        hist->GetYaxis()->SetTitle(_titleY.c_str());
        hist->GetZaxis()->SetTitle(_titleZ.c_str());
        hist->SetTitle(_title.c_str());
	hist->SetStats(0);

        if(same) _same_level++;
        else _same_level = 0;

	if(drawUtils::ToUpper(opt).find("TEXT") != string::npos)
	{
		hist->SetMarkerSize(2);
		gStyle->SetPaintTextFormat("1.2f");
		if(drawUtils::ToUpper(opt).find("COLZ") != string::npos)
		{
			hist->SetMarkerColor(kWhite);
			hist->SetMarkerSize(2.5);
		}
	}

        hist->Draw(opt.c_str());

        if(leg != "")
       	{
                if(!same) CreateLegend();
                _legends.back()->AddEntry(hist, leg.c_str(), "LE1P");
                _legends.back()->Draw();
        }
}

void DrawingToolsNueCC::ProcessPlot(TGraph *graph, const string opt, const string leg)
{

	bool same = (drawUtils::ToUpper(opt).find("C") != string::npos);
	if(drawUtils::ToUpper(opt).find("SAME") != string::npos)
	{
		std::cout << "Option \"same\" set for TGraph. \"C\" should be used instead. Exiting!" << std::endl;
		return;
	}

        graph->GetXaxis()->SetTitle(_titleX.c_str());
        graph->GetYaxis()->SetTitle(_titleY.c_str());
        graph->SetTitle(_title.c_str());

        if(same) _same_level++;
        else _same_level = 0;

        
	graph->SetLineColor(_auto_colors[_same_level]);
	graph->SetFillColor(_auto_colors[_same_level]);
	graph->SetMarkerColor(_auto_colors[_same_level]);
	graph->SetMarkerStyle(_auto_markers[_same_level]);

        graph->Draw(opt.c_str());

        if(leg != "")
       	{
                if(!same) CreateLegend();
                _legends.back()->AddEntry(graph, leg.c_str(), "LE1P");
                _legends.back()->Draw();
        }
}

TH2_h* DrawingToolsNueCC::Get2DHisto(DataSample& data, const string var, const string var2, const string cut, int nx, double xmin, double xmax, int ny, double ymin, double ymax)
{
	double xbins[NMAXBINS];
	double ybins[NMAXBINS];

	return Get2DHisto(data, var, var2, cut, nx, DrawingToolsBase::GetVariableBins(nx,xmin,xmax,xbins), ny, DrawingToolsBase::GetVariableBins(ny,ymin,ymax,ybins));
}

TH2_h* DrawingToolsNueCC::Get2DHisto(DataSample& data, const string var, const string var2, const string cut, int nx, double* xbins, int ny, double *ybins)
{
	string name = DrawingToolsBase::GetUniqueName("name");

	TH2_h* hist = new TH2_h(name.c_str(), "hist", nx, xbins, ny, ybins);
	data.GetTree()->Project(name.c_str(), (var2 + ":" + var).c_str(), cut.c_str());

	return hist;
}

void DrawingToolsNueCC::Draw(DataSample& data, const string var, const string var2, const string cut, int nx, double xmin, double xmax, int ny, double ymin, double ymax, const string opt)
{
	double xbins[NMAXBINS];
	double ybins[NMAXBINS];

	Draw(data, var, var2, cut, nx, DrawingToolsBase::GetVariableBins(nx,xmin,xmax,xbins), ny, DrawingToolsBase::GetVariableBins(ny,ymin,ymax,ybins), opt);
}

void DrawingToolsNueCC::Draw(DataSample& data, const string var, const string var2, const string cut, int nx, double *xbins, int ny, double *ybins, const string opt)
{
	TH2_h* hist = Get2DHisto(data, var, var2, cut, nx, xbins, ny, ybins);
	ProcessPlot(hist, opt, "");
}


// ************************************************************************************************************
// Weighting methods.
// ************************************************************************************************************

double DrawingToolsNueCC::GetPurity(DataSample& mc, const string signal, const string cut, double *error)
{

	string sel = cut + " && " + signal;

	double selec = mc.GetTree()->GetEntries(sel.c_str());
	double total = mc.GetTree()->GetEntries(cut.c_str());

	// Uncertainty in purity.
	if(error) *error = GetBinomialUncertainty(selec, total);

	if(total != 0) return selec/total;
	else return 0;
}

double DrawingToolsNueCC::GetEfficiency(DataSample& mc, const string signal, const string cut, string tree, double *error)
{

	string sel = cut + " && " + signal;

	double selec = mc.GetTree()->GetEntries(sel.c_str());
	double total = mc.GetTree(tree)->GetEntries(signal.c_str());

	// Uncertainty in purity.
	if(error) *error = GetBinomialUncertainty(selec, total);

	if(total != 0) return selec/total;
	else return 0;
}

// The pointer to the vector is used as an arguements instead of just the vector so that it can be initialised in the header
// This allows it to be an optional argument.
// Vectors are used instead of arrays for greater stability and simplicity in the more complicated functions.
vector<double> DrawingToolsNueCC::GetPurityVsBin(DataSample& data, const string var, const string& signal, const string& cut, int nx, double xmin, double xmax, vector<double> *purerrors, bool AddSyst)
{
	double xbins[NMAXBINS];

	return GetPurityVsBin(data, var, signal, cut, nx, DrawingToolsBase::GetVariableBins(nx,xmin,xmax,xbins), purerrors, AddSyst);
}

vector<double> DrawingToolsNueCC::GetPurityVsBin(DataSample& data, const string var, const string& signal, const string& cut, int nx, double *xbins, vector<double> *purerrors, bool AddSyst)
{

        if(purerrors && (signed) purerrors->size() != nx)
        {
                cout << "Passed error vector has incorrect size! Resizing..." << endl;
                purerrors->resize(nx);
        }

	vector<double> purities(nx);
	double temp = 0;
	string sel = cut + " && " + signal;

	TH1_h* selec = DrawingToolsBase::GetHisto(data.GetTree(), "selec", var, nx, xbins, sel, "", "", 1);
	TH1_h* total = DrawingToolsBase::GetHisto(data.GetTree(), "total", var, nx, xbins, cut, "", "", 1);

	for(int i = 1; i < nx + 1; i++)
	{
		if(total->GetBinContent(i) != 0) purities.at(i-1) = (selec->GetBinContent(i)) / (total->GetBinContent(i));

                // Sort out errors.
		if(!purerrors) continue;
                temp = GetBinomialUncertainty(selec->GetBinContent(i), total->GetBinContent(i)); // Uncertainty in fraction

                if(AddSyst) purerrors->at(i-1) = sqrt(temp*temp + (purerrors->at(i-1))*(purerrors->at(i-1))); // Stat + Syst
                else purerrors->at(i-1) = temp; // Statistical errors only

                if(purerrors->at(i-1) != purerrors->at(i-1)) purerrors->at(i-1) = 0;
	}

	return purities;
}

// NOTE!!! When using these functions you must use accum_level with the brackets afterwards. Like accum_level[][0]>2 or you will get the wrong results.
vector<double> DrawingToolsNueCC::GetEfficiencyVsBin(DataSample& data, const string var, const string& signal, const string& cut, int nx, double xmin, double xmax, string tree, vector<double> *efferrors, bool AddSyst)
{
	double xbins[NMAXBINS];

	return GetEfficiencyVsBin(data, var, signal, cut, nx, DrawingToolsBase::GetVariableBins(nx,xmin,xmax,xbins), tree, efferrors, AddSyst);
}

vector<double> DrawingToolsNueCC::GetEfficiencyVsBin(DataSample& data, const string var, const string& signal, const string& cut, int nx, double *xbins, string tree, vector<double> *efferrors, bool AddSyst)
{
	if(efferrors && (signed) efferrors->size() != nx)
	{
		cout << "Passed error array has incorrect size! Resizing..." << endl;
		efferrors->resize(nx);
	}

        vector<double> efficiencies(nx);
        double temp = 0;
        string sel = cut + " && " + signal;

        TH1_h* selec = DrawingToolsBase::GetHisto(data.GetTree(), "selec", var, nx, xbins, sel, "", "", 1);
        TH1_h* total = DrawingToolsBase::GetHisto(data.GetTree(tree), "total", var, nx, xbins, signal, "", "", 1);

        for(int i = 1; i < nx + 1; i++)
        {
                if(total->GetBinContent(i) != 0) efficiencies.at(i-1) = (selec->GetBinContent(i)) / (total->GetBinContent(i));

                // Sort out errors.
		if(!efferrors) continue;
                temp = GetBinomialUncertainty(selec->GetBinContent(i), total->GetBinContent(i)); // Uncertainty in fraction

                if(AddSyst) efferrors->at(i-1) = sqrt(temp*temp + efferrors->at(i-1)*efferrors->at(i-1)); // Stat + Syst
                else efferrors->at(i-1) = temp; // Statistical errors only

		if(efferrors->at(i-1) != efferrors->at(i-1)) efferrors->at(i-1) = 0;
        }

	return efficiencies;
}

vector<double> DrawingToolsNueCC::GetPurEffVsBin(DataSample& data, const string var, const string& signal, const string& cut, int nx, double xmin, double xmax, string tree, vector<double> *errors, bool AddSyst)
{
	double xbins[NMAXBINS];

	return GetPurEffVsBin(data, var, signal, cut, nx, DrawingToolsBase::GetVariableBins(nx,xmin,xmax,xbins), tree, errors, AddSyst);
}

vector<double> DrawingToolsNueCC::GetPurEffVsBin(DataSample& data, const string var, const string& signal, const string& cut, int nx, double *xbins, string tree, vector<double> *errors, bool AddSyst)
{

        if(errors && (signed) errors->size() != nx)
        {
                cout << "Passed error vector has incorrect size! Resizing..." << endl;
                errors->resize(nx);
        }

	double temp = 0;

	// These are filled in the Get... methods.
        vector<double> eff_errs(nx);
        vector<double> pur_errs(nx);

	// Get the efficiencies and purities and fill the statistical errors.
	// The systematic errors (if AddSyst == true) are applied to pur*eff
        vector<double> eff = GetEfficiencyVsBin(data, var, signal, cut, nx, xbins, tree, &eff_errs, false);
        vector<double> pur = GetPurityVsBin(data, var, signal, cut, nx, xbins, &pur_errs, false);

	vector<double> pureff(nx);

	for(int i = 0; i < nx; i++)
	{
		pureff.at(i) = eff.at(i)*pur.at(i);

        	temp = GetPoissonUncertainty(eff.at(i), eff_errs.at(i), pur.at(i), pur_errs.at(i)); // Statistical errors only

        	if(AddSyst) errors->at(i) = sqrt(temp*temp + errors->at(i)*errors->at(i)); // Stat + Syst
        	else errors->at(i) = temp; // Statistical errors only
	}

	return pureff;
}

vector<double> DrawingToolsNueCC::GetWeights(DataSample& data, const string var, const string& signal, const string& cut, int nx, double xmin, double xmax, string tree)
{
	double xbins[NMAXBINS];

	return GetWeights(data, var, signal, cut, nx, DrawingToolsBase::GetVariableBins(nx,xmin,xmax,xbins), tree);
}

vector<double> DrawingToolsNueCC::GetWeights(DataSample& data, const string var, const string& signal, const string& cut, int nx, double *xbins, string tree)
{
	vector<double> weights(nx);
	vector<double> purities = GetPurityVsBin(data, var, signal, cut, nx, xbins);
	vector<double> efficiencies = GetEfficiencyVsBin(data, var, signal, cut, nx, xbins, tree);

	for(int i = 0; i < nx; i++)
	{
		if(efficiencies.at(i) != 0) weights.at(i) = purities.at(i)/efficiencies.at(i);
		else weights.at(i) = 1;
	}

	return weights;
}

void DrawingToolsNueCC::ScaleHist(TH1_h *hist, vector<double> scale, bool inv)
{

	if((unsigned) hist->GetNbinsX() != scale.size())
        {
                cout << "WARNING!!!! Size of scale vector doesn't match number of histogram bins!" << endl;
                cout << "Not scaling!" << endl;
                return;
        }

	if(!inv)
	{
		for(int i=1; i<=hist->GetNbinsX(); i++)
	        {
        	        hist->SetBinContent(i,(hist->GetBinContent(i))*scale.at(i-1));
	        }
	}
	else
	{
                for(int i=1; i<=hist->GetNbinsX(); i++)
               	{
                        if(scale.at(i-1) != 0) hist->SetBinContent(i,(hist->GetBinContent(i))*(1/scale.at(i-1)));
			else cout << "Trying to divide by efficiency = 0 in bin " << i-1 << endl;
               	}
	}
}

void DrawingToolsNueCC::ScaleHist(TH1_h *hist, double *scale, bool inv)
{
	// This doesn't work...
	// Not sure it's possible to find length of an array if simply passed as a pointer.
//	if(hist->GetNbinsX() != sizeof(scale)/sizeof(*scale))
//	{
//		cout << "WARNING!!!! Length of scale array doesn't match number of histogram bins!" << endl;
//		cout << "Not scaling!" << endl;
//		return;
//	}

	if(!inv)
	{
		for(int i=1; i<=hist->GetNbinsX(); i++)
		{
			hist->SetBinContent(i,(hist->GetBinContent(i))*scale[i-1]);
		}
	}
	else
	{
                for(int i=1; i<=hist->GetNbinsX(); i++)
               	{
                        hist->SetBinContent(i,(hist->GetBinContent(i))*(1/scale[i-1]));
               	}
	}
}

void DrawingToolsNueCC::SetErrors(TH1_h *hist, vector<double> errs)
{
	if((unsigned) hist->GetNbinsX() != errs.size())
	{
		cout << "Size of errors vector does not match number of histogram bins. Returning!" << endl;
		return;
	}

	for(unsigned int i=0; i<errs.size(); i++)
	{
		hist->SetBinError(i, errs.at(i));
	}
}

void DrawingToolsNueCC::DrawPurity(DataSample& data, const string var, const string& signal, const string& cut, int nx, double xmin, double xmax, string opt, string leg, vector<double> *errors)
{
        double xbins[NMAXBINS];

        DrawPurity(data, var, signal, cut, nx, DrawingToolsBase::GetVariableBins(nx,xmin,xmax,xbins), opt, leg, errors);
}

void DrawingToolsNueCC::DrawPurity(DataSample& data, const string var, const string& signal, const string& cut, int nx, double *xbins, string opt, string leg, vector<double> *errors)
{
        vector<double> fillvar(nx);
        vector<double> staterrors(nx);

	fillvar = GetPurityVsBin(data, var, signal, cut, nx, xbins, &staterrors);

        TH1_h *hist = new TH1_h("", "", nx, xbins);

        for(int i=1; i<=nx; i++)
        {
                hist->SetBinContent(i, fillvar.at(i-1));
                // Add systematic errors to statistical errors.
                if(!isinf(staterrors.at(i-1))) hist->SetBinError(i, staterrors.at(i-1));
                else hist->SetBinError(i, 0);
                if(errors) hist->SetBinError(i, sqrt(hist->GetBinError(i)*hist->GetBinError(i) + errors->at(i)*errors->at(i-1)));
        }

	hist->SetAxisRange(0,1.1,"Y");

	ProcessPlot(hist, opt, leg);
        gPad->Update();
}

void DrawingToolsNueCC::DrawEfficiency(DataSample& data, const string var, const string& signal, const string& cut, int nx, double xmin, double xmax, string opt, string leg, string tree, vector<double> *errors)
{
	double xbins[NMAXBINS];

	DrawEfficiency(data, var, signal, cut, nx, DrawingToolsBase::GetVariableBins(nx,xmin,xmax,xbins), opt, leg, tree, errors);
}

void DrawingToolsNueCC::DrawEfficiency(DataSample& data, const string var, const string& signal, const string& cut, int nx, double *xbins, string opt, string leg, string tree, vector<double> *errors)
{
        vector<double> fillvar(nx);
        vector<double> staterrors(nx);

        fillvar = GetEfficiencyVsBin(data, var, signal, cut, nx, xbins, tree, &staterrors);

        TH1_h *hist = new TH1_h("", "", nx, xbins);

        for(int i = 1; i < nx + 1; i++)
        {
                hist->SetBinContent(i, fillvar.at(i-1));
                // Add systematic errors to statistical errors.
                if(!isinf(staterrors.at(i-1))) hist->SetBinError(i, staterrors.at(i-1));
                else hist->SetBinError(i, 0);
                if(errors) hist->SetBinError(i, hist->GetBinError(i) + errors->at(i-1));
        }

	hist->SetAxisRange(0,1.1,"Y");

	ProcessPlot(hist, opt, leg);
        gPad->Update();
}

void DrawingToolsNueCC::DrawPurEff(DataSample& data, const string var, const string& signal, const string& cut, int nx, double xmin, double xmax, string opt, string leg, string tree, vector<double> *errors)
{
        double xbins[NMAXBINS];

        DrawPurEff(data, var, signal, cut, nx, DrawingToolsBase::GetVariableBins(nx,xmin,xmax,xbins), opt, leg, tree, errors);
}

void DrawingToolsNueCC::DrawPurEff(DataSample& data, const string var, const string& signal, const string& cut, int nx, double *xbins, string opt, string leg, string tree, vector<double> *errors)
{
        vector<double> fillvar(nx);
        vector<double> staterrors(nx);

	fillvar = GetPurEffVsBin(data, var, signal, cut, nx, xbins, tree, &staterrors);

        TH1_h *hist = new TH1_h("", "", nx, xbins);

        for(int i=1; i<=nx; i++)
        {
                hist->SetBinContent(i, fillvar.at(i-1));
                // Add systematic errors to statistical errors.
                if(!isinf(staterrors.at(i-1))) hist->SetBinError(i, staterrors.at(i-1));
                else hist->SetBinError(i, 0);
                if(errors) hist->SetBinError(i, sqrt(hist->GetBinError(i)*hist->GetBinError(i) + errors->at(i)*errors->at(i-1)));
        }

	hist->SetAxisRange(0,1.1,"Y");

	ProcessPlot(hist, opt, leg);
        gPad->Update();
}

// ************************************************************************************************************
// Analysis methods.
// ************************************************************************************************************

void DrawingToolsNueCC::GetSpline(TString file_name, const char* graph_name, const string opt, Color_t col, double LineWidth, const string leg)
{

	TString path = "/home/t2k/il/T2K/highland/xsecRatioAnalysis/v0r1/cmt/";
	cout << "Looking for root file " << file_name << " in " << path << endl;

	TFile *file = TFile::Open(path + file_name);
	if(!file || !file->IsOpen())
	{
		cout << "Error! Cannot open " << file_name << endl;
		delete file;
		exit(1);
	}

	TGraph *graph = (TGraph*) file->GetObjectChecked(graph_name, "TGraph");
	if(!graph)
	{
		cout << "Error in TGraph " << graph_name << " in file " << file_name << endl;
		cout << "Check the graph exists in the root file!" << endl;
		delete graph;
		exit(1);
	}

	// Convert from GeV to MeV
	const int nBins = graph->GetN();
	double *xbins = new double[nBins];
	for(int i = 0; i < nBins; i++) xbins[i] = 1000*(graph->GetX())[i]; 

	// Copy the values of the graph to a new graph.
	// I can't get the graphs to plot correctly if read straight from file.
	// This method is messy, but it works.
	TGraph *out_graph = new TGraph(graph->GetN(), xbins, graph->GetY());
	out_graph->SetLineColor(col);
	out_graph->SetLineWidth(LineWidth);
	out_graph->SetTitle(_title.c_str());
	out_graph->GetXaxis()->SetTitle(_titleX.c_str());
	out_graph->GetYaxis()->SetTitle(_titleY.c_str());
	out_graph->Draw(opt.c_str());

        bool same = (drawUtils::ToUpper(opt).find("SAME") != string::npos);
	if (leg != "")
	{
		if(!same) CreateLegend();
		_legends.back()->AddEntry(out_graph, leg.c_str(), "l");
		_legends.back()->Draw();
	}

	gPad->Update();
}

void DrawingToolsNueCC::FitLine(TH1_h *hist)
{
	if(hist == NULL)
	{
		cout << "Pointer to histogram is NULL!!! Not fitting!" << endl;
		return;
	}

	TF1 *myfit = new TF1("line", Line, hist->GetMinimum(), hist->GetMaximum(), 2);

	myfit->SetParameter(0,1); // Intercept
  	myfit->SetParameter(1,0); // Gradient

	hist->Fit("line");

  	Double_t pars[2];
  	myfit->GetParameters(pars);

  	cout << " Intercept = " << pars[0] << endl;
 	cout << " Gradient = " << pars[1] << endl;
	cout << " The fit has chi^2 " << myfit->GetChisquare() << " with " << myfit->GetNDF() << " degrees of freedom and a probability of " << myfit->GetProb() << endl;

	gPad->Update();
}

void DrawingToolsNueCC::PurEffVsCutVal(DataSample& mc, const string var, const string operat, const string signal, const string cut, int nx, double xmin, double xmax)
{
        double xbins[NMAXBINS];

        PurEffVsCutVal(mc, var, operat, signal, cut, nx, DrawingToolsBase::GetVariableBins(nx,xmin,xmax,xbins));
}

void DrawingToolsNueCC::PurEffVsCutVal(DataSample& mc, const string var, const string operat, const string signal, const string cut, int nx, double *xbins)
{

	double *pur = new double[nx];
	double *eff = new double[nx];
	double *pur_err = new double[nx];
	double *eff_err = new double[nx];
	double *pureff = new double[nx];
	double *pureff_err = new double[nx];

	// This method is a little messy but it's simple and works.
	double temp;
	double *xerr = new double[nx];

	for (int i = 0; i < nx+1; i++)
	{
		stringstream ss;
		ss << cut << " && " << var << operat << xbins[i];

		// Purity and Efficiencies + errors
		pur[i] = GetPurity(mc, signal, ss.str(), &temp);
		pur_err[i] = temp;
		eff[i] = GetEfficiency(mc, signal, ss.str(), "default", &temp);
		eff_err[i] = temp;

		// Purity * efficiency + errors.
		pureff[i] = pur[i]*eff[i];
		pureff_err[i] = sqrt(pur_err[i]*pur_err[i] + eff_err[i]*eff_err[i]);

		xerr[i] = 0; // Zero x error
	}
	
	TGraphErrors *pgraph = new TGraphErrors(nx+1, xbins, pur, xerr, pur_err);
        TGraphErrors *egraph = new TGraphErrors(nx+1, xbins, eff, xerr, eff_err);
	TGraphErrors *pegraph = new TGraphErrors(nx+1, xbins, pureff, xerr, pureff_err);

	pgraph->SetMinimum(0);
	pgraph->SetMaximum(1.2);
	pgraph->SetLineWidth(2);
	egraph->SetLineWidth(2);
	pegraph->SetLineWidth(2);
	
	ProcessPlot(pgraph, "AL", "Purity");
	ProcessPlot(egraph, "C", "Efficiency");
	ProcessPlot(pegraph, "C", "pur x eff");
}

void DrawingToolsNueCC::FindMaxPurEff(DataSample& mc, const string var, const string operat, const string signal, const string cut, int nx, double xmin, double xmax, double *result)
{
        double xbins[NMAXBINS];

        FindMaxPurEff(mc, var, operat, signal, cut, nx, DrawingToolsBase::GetVariableBins(nx,xmin,xmax,xbins), result);
}

void DrawingToolsNueCC::FindMaxPurEff(DataSample& mc, const string var, const string operat, const string signal, const string cut, int nx, double *xbins, double *result)
{
        double pur, eff, pureff;
	double maxpur = -1, maxeff = -1, maxpureff = -1;
	double varpur = 0, vareff = 0, varpureff = 0;

        for (int i = 0; i < nx+1; i++)
        {
                stringstream ss;
                ss << cut << " && " << var << operat << xbins[i];

                pur = GetPurity(mc, signal, ss.str());
                eff = GetEfficiency(mc, signal, ss.str());
                pureff = pur*eff;

		if(pur > maxpur)
		{
			maxpur = pur;
			varpur = xbins[i];
		}
                if(eff > maxeff)
                {
                        maxeff = eff;
                        vareff = xbins[i];
                }
                if(pureff > maxpureff)
                {
                        maxpureff = pureff;
                        varpureff = xbins[i];
                }
	}

	*result = varpureff;

	cout << "Maximum purity found = " << maxpur << " for " << var << operat << varpur << endl;
	cout << "Maximum efficiency found = " << maxeff << " for " << var << operat << vareff << endl;
	cout << "Maximum purity x efficiency found = " << maxpureff << " for " << var << operat << varpureff << endl;
}

void DrawingToolsNueCC::FindMaxPurEff(DataSample& mc, const string var, const string operat, const string var2, const string operat2, const string signal, const string cut, int nx, double xmin, double xmax, int ny, double ymin, double ymax, double *res1, double *res2)
{
        double xbins[NMAXBINS];
	double ybins[NMAXBINS];

        FindMaxPurEff(mc, var, operat, var2, operat2, signal, cut, nx, DrawingToolsBase::GetVariableBins(nx,xmin,xmax,xbins), ny, DrawingTools::GetVariableBins(ny,ymin,ymax,ybins), res1, res2);
}

void DrawingToolsNueCC::FindMaxPurEff(DataSample& mc, const string var, const string operat, const string var2, const string operat2, const string signal, const string cut, int nx, double *xbins, int ny, double *ybins, double *res1, double *res2)
{
        double pur, eff, pureff;
        double maxpur = -1, maxeff = -1, maxpureff = -1;
        double varpur1 = 0, vareff1 = 0, varpureff1 = 0, varpur2 = 0, vareff2 = 0, varpureff2 = 0;

        for (int i = 0; i < nx+1; i++)
       	{
		for(int j = 0; j < ny+1; j++)
		{
        	        stringstream ss;
	                ss << cut << " && " << var << operat << xbins[i] << " && " << var2 << operat2 << ybins[j];

        	        pur = GetPurity(mc, signal, ss.str());
	                eff = GetEfficiency(mc, signal, ss.str());
        	        pureff = pur*eff;

        	        if(pur > maxpur)
	                {
        	                maxpur = pur;
	                        varpur1 = xbins[i];
                        	varpur2 = ybins[j];
                	}
        	        if(eff > maxeff)
	                {
                	        maxeff = eff;
        	                vareff1 = xbins[i];
	                        vareff2 = ybins[j];
                	}
        	       	if(pureff > maxpureff)
	                {
                        	maxpureff = pureff;
                	        varpureff1 = xbins[i];
        	                varpureff2 = ybins[j];
	                }
		}
        }

	*res1 = varpureff1;
	*res2 = varpureff2;

        cout << "Maximum purity found = " << maxpur << " for " << var << operat << varpur1 << ", " << var2 << operat2 << varpur2 << endl;
        cout << "Maximum efficiency found = " << maxeff << " for " << var << operat << vareff1 << ", " << var2 << operat2 << vareff2 << endl;
        cout << "Maximum purity x efficiency found = " << maxpureff << " for " << var << operat << varpureff1 << ", " << var2 << operat2 << varpureff2 << endl;
}

void DrawingToolsNueCC::DrawAverage(DataSample& file, const string var, const string var2, double min, double max, const string cut, int nx, double xmin, double xmax, const string opt, const string leg)
{
        double xbins[NMAXBINS];

        DrawAverage(file, var, var2, min, max, cut, nx, DrawingToolsBase::GetVariableBins(nx,xmin,xmax,xbins), opt, leg);
}

void DrawingToolsNueCC::DrawAverage(DataSample& file, const string var, const string var2, double min, double max, const string cut, int nx, double *xbins, const string opt, const string leg)
{

	TH1_h* hist;
	double *fillvar = new double[nx];
	double temp[NMAXBINS];

	for(int i = 0; i < nx+1; i++)
	{
		stringstream ss;
		ss << cut << "&&" << var << "==" << xbins[i] << "&&" << var2 << "<" << max << "&&" << var2 << ">" << min;

        	hist = DrawingToolsBase::GetHisto(file.GetTree(), "", var2, nx, GetVariableBins(nx,min,max,temp), ss.str(), "", "", 1);
		fillvar[i] = hist->GetMean();
	}

	TGraph *graph = new TGraph(nx+1, xbins, fillvar);
	graph->SetLineWidth(2);
	ProcessPlot(graph, opt, leg);

	gPad->Update();
}

void DrawingToolsNueCC::DataMCDiff(DataSample& data, DataSample& mc, const string var, const string cut, int nx, double xmin, double xmax, const string opt, const string leg, bool fitLine)
{
	double xbins[NMAXBINS];

	DataMCDiff(data, mc, var, cut, nx, DrawingToolsBase::GetVariableBins(nx,xmin,xmax,xbins), opt, leg, fitLine);
}

void DrawingToolsNueCC::DataMCDiff(DataSample& data, DataSample& mc, const string var, const string cut, int nx, double *xbins, const string opt, const string leg, bool fitLine)
{

	double scale = GetPOTRatio(data, mc);
	TH1_h *data_hist = DrawingToolsBase::GetHisto(data.GetTree(), "", var, nx, xbins, cut, "", "", 1); 
	TH1_h *mc_hist = DrawingToolsBase::GetHisto(mc.GetTree(), "", var, nx, xbins, cut, "", "", scale);
	TH1_h *diff_hist = new TH1_h("", "", nx, xbins);

	for(int i = 0; i < nx+1; i++)
	{
		if(mc_hist->GetBinContent(i) == 0)
		{
			diff_hist->SetBinContent(i, 0);
			continue;
		}
		diff_hist->SetBinContent(i, (data_hist->GetBinContent(i) - mc_hist->GetBinContent(i))/(mc_hist->GetBinContent(i)));
		diff_hist->SetBinError(i, sqrt(fabs(data_hist->GetBinContent(i) - mc_hist->GetBinContent(i))/(mc_hist->GetBinContent(i))));
	}

	if(fitLine) FitLine(diff_hist);

	ProcessPlot(diff_hist, opt, leg);
}

// ************************************************************************************************************
// Unfolding methods.
// ************************************************************************************************************
/*
void DrawingToolsNueCC::DrawUnfolded(DataSample& data, DataSample& mc, DataSample& ctrl, const string reco_var, int nreco, double *reco_bins, const string true_var, int ntrue, double *true_bins, int iterations, const string signal, const string cut, bool weight, const string opt, const string leg)
{
	TH1_h* hist_reco = GetUnfolded(data, mc, ctrl, reco_var, nreco, reco_bins, true_var, ntrue, true_bins, iterations, signal, cut, weight);
        ProcessPlot(hist_reco, opt, leg);
}

// Set up from already-filled histograms.
// "response" gives the response matrix, measured X truth.
// "measured" and "truth" give the projections of "response" onto the X-axis and Y-axis respectively,
// but with additional entries in "measured" for measurements with no corresponding truth (fakes/background) and
// in "truth" for unmeasured events (inefficiency).
// "measured" and/or "truth" can be specified as 0 (1D case only) or an empty histograms (no entries) as a shortcut
// to indicate, respectively, no fakes and/or no inefficiency.
TH1_h* DrawingToolsNueCC::GetUnfolded(DataSample& data, DataSample& mc, DataSample& ctrl, const string reco_var, int nreco, double *reco_bins, const string true_var, int ntrue, double *true_bins, int iterations, const string signal, const string cut, bool weight)
{

        // Purities in terms of pre-unfolding variable.
        vector<double> purities = GetPurityVsBin(mc, reco_var, signal, cut, nreco, reco_bins);
        // Efficiencies in terms of post-unfolding variable.
        vector<double> efficiencies = GetEfficiencyVsBin(mc, true_var, signal, cut, ntrue, true_bins);

	// Get the true and reconstructed histograms
        TH1_h *hReco = new TH1_h("", "", nreco, reco_bins);
        TH1_h *hTrue = new TH1_h("", "", ntrue, true_bins);

	// If we use the filled histograms, the weighting is taken care of in the unfolding procedure.
	// This would mean using the same sample to unfold and weight.
	// Setting "weight = true" lets you use a different sample.
	if(!weight)
	{
        	hReco = DrawingToolsBase::GetHisto(ctrl.GetTree(), "", reco_var, nreco, reco_bins, cut, "", "", 1);
	        hTrue = DrawingToolsBase::GetHisto(ctrl.GetTree(), "", true_var, ntrue, true_bins, signal, "", "", 1);
	}
        TH1_h *hData = DrawingToolsBase::GetHisto(data.GetTree(), "", reco_var, nreco, reco_bins, cut, "", "", 1);

        // Scale data to mc purities.
        if(weight) ScaleHist(hData, purities);

        // Create a 2D histogram that contains the response information.
        // No neat way to do this in highland so we must do it ourselves.
        string name = GetUniqueName("name");
        TH2_h* hResp = new TH2_h(name.c_str(), "", nreco, reco_bins, ntrue, true_bins);

	// Fill the 2D histogram.
	string var2 = true_var + ":" + reco_var;
	string cut2 = cut + " && " + signal;
	ctrl.GetTree()->Project(name.c_str(), var2.c_str(), cut2.c_str());

        // Create the response and unfolding objects based on our histograms.
        RooUnfoldResponse response(hReco, hTrue, hResp);
        RooUnfoldBayes unfold(&response, hData, iterations);

        // Fetch the unfolded histogram.
        TH1_h* hist_reco = (TH1_h*) unfold.Hreco();

        // Scale data to mc efficiencies. true means we scale by the inverse of each element.
        if(weight) ScaleHist(hist_reco, efficiencies, true);

        return hist_reco;
}
*/
