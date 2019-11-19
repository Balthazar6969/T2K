#ifndef DrawingToolsNueCC_h
#define DrawingToolsNueCC_h

#include "DrawingTools.hxx"

/// A series of functions that provide extra utilities compared to
/// the standard DrawingTools functions.
/// This class inherits from DrawingTools

using namespace std;

class DrawingToolsNueCC: public DrawingTools {
public :

  DrawingToolsNueCC(const string& file="", bool useT2Kstyle=true);
  DrawingToolsNueCC(Experiment& exp, bool useT2Kstyle=true);
  virtual ~DrawingToolsNueCC(){}

/// Process a TH1_h. E.g. draw axis, set colours, draw legends etc...
void ProcessPlot(TH1 *hist, const string opt, const string leg);

/// Process a TH2_h. E.g. draw axis, set colours, draw legends etc...
void ProcessPlot(TH2 *hist, const string opt, const string leg);

/// Process a TGraph. E.g. draw axis, set colours, draw legends etc...
void ProcessPlot(TGraph *graph, const string opt, const string leg);

/// Get 2D histogram
TH2_h* Get2DHisto(DataSample& data, const string var, const string var2, const string cut, int nx, double xmin, double xmax, int ny, double ymin, double ymax);

/// Get 2D histogram
TH2_h* Get2DHisto(DataSample& data, const string var, const string var2, const string cut, int nx, double* xbins, int ny, double *ybins);

/// Draw 2D histogram
void Draw(DataSample& data, const string var, const string var2, const string cut, int nx, double xmin, double xmax, int ny, double ymin, double ymax, const string opt = "COLZ");

/// Draw 2D histogram
void Draw(DataSample& data, const string var, const string var2, const string cut, int nx, double *xbins, int ny, double *ybins, const string opt = "COLZ");

/// Get the purity of a sample for a given cut. Pass error variable to get the uncertainty too.
/// This method is faster than using GetPurityVsBin(...).at(0);
double GetPurity(DataSample& mc, const string signal, const string cut, double *error = NULL);

/// Get the efficiency of a sample for a given cut. Pass error variable to get the uncertainty too.
/// This method is faster than using GetEfficiencyVsBin(...).at(0);
double GetEfficiency(DataSample& mc, const string signal, const string cut, string tree="truth", double *error = NULL);

/// Returns a vector where the i th element is the purity of the i+1 th histogram bin for variable binning.
/// (Histogram bins are counted from 1)
vector<double> GetPurityVsBin(DataSample& data, const string var, const string& signal, const string& cut, int nx, double *xbins, vector<double> *purerrors = 0, bool AddSyst = false);

/// Returns a vector where the i th element is the purity of the i+1 th histogram bin.
/// (Histogram bins are counted from 1)
vector<double> GetPurityVsBin(DataSample& data, const string var, const string& signal, const string& cut, int nx, double xmin, double xmax, vector<double> *purerrors = 0, bool AddSyst = false);

/// Returns a vector where the i th element is the efficiency of the i+1 th histogram bin for variable binning.
/// (Histogram bins are counted from 1)
/// If efferrors is passed as an arguement, then the values are filled as the errors in efficiency.
vector<double> GetEfficiencyVsBin(DataSample& data, const string var, const string& signal, const string& cut, int nx, double *xbins, string tree="truth", vector<double> *efferrors = 0, bool AddSyst = false);

/// Returns a vector where the i th element is the purity of the i+1 th histogram bin.
/// (Histogram bins are counted from 1)
/// If efferrors is passed as an arguement, then the values are filled as the errors in efficiency.
vector<double> GetEfficiencyVsBin(DataSample& data, const string var, const string& signal, const string& cut, int nx, double xmin, double xmax, string tree="truth", vector<double> *efferrors = 0, bool AddSyst = false);

/// Vector containing purity * efficiency. Normal binning.
/// Pass optional arguement "errors" and AddSyst = false and it will be filled with the statistical errors.
/// Pass with AddSyst = true then the elements of errors will be added in quadrature with the statistical errors
vector<double> GetPurEffVsBin(DataSample& data, const string var, const string& signal, const string& cut, int nx, double xmin, double xmax, string tree="truth", vector<double> *errors = 0, bool AddSyst = false);

/// Vector containing purity * efficiency. Variable binning.
/// Pass optional arguement "errors" and AddSyst = false and it will be filled with the statistical errors.
/// Pass with AddSyst = true then the elements of errors will be added in quadrature with the statistical errors
vector<double> GetPurEffVsBin(DataSample& data, const string var, const string& signal, const string& cut, int nx, double *xbins, string tree="truth", vector<double> *errors = 0, bool AddSyst = false);

/// Get the weights to scale a histogram by purity/efficiency for variable binning for a specified signal.
vector<double> GetWeights(DataSample& data, const string var, const string& signal, const string& cut, int nx, double xmin, double xmax, string tree="truth");

/// Get the weights to scale a histogram by purity/efficiency for a specified signal.
vector<double> GetWeights(DataSample& data, const string var, const string& signal, const string& cut, int nx, double *xbins, string tree="truth");

/// Scale the i th bin of a histogram by the i-1 th element of an array.
/// (Histogram bins are counted from 1)
void ScaleHist(TH1_h *hist, double *scale, bool inv = false);

/// Scale the i th bin of a histogram by the i-1 th element of a vector.
/// (Histogram bins are counted from 1)
void ScaleHist(TH1_h *hist, vector<double> scale, bool inv = false);

/// Set the errors on a histogram by the elements of a vector
void SetErrors(TH1_h *hist, vector<double> errs);

/// Draw 1D purity vs bin.
/// Errors are statistical only if errors variable isn't used. Added in quadrature with errors otherwise.
/// Calculates statistical uncertainties using binomial statistics.
void DrawPurity(DataSample& data, const string var, const string& signal, const string& cut, int nx, double xmin, double xmax, string opt = "", string leg = "", vector<double> *errors = 0);

/// Draw 1D purity vs bin.
/// Errors are statistical only if errors variable isn't used. Added in quadrature with errors otherwise.
/// Calculates statistical uncertainties using binomial statistics.
void DrawPurity(DataSample& data, const string var, const string& signal, const string& cut, int nx, double *xbins, string opt = "", string leg = "", vector<double> *errors = 0);

/// Draw 1D efficiency vs bin.
/// Errors are statistical only if errors variable isn't used. Added in quadrature with errors otherwise.
/// Calculates statistical uncertainties using binomial statistics.
void DrawEfficiency(DataSample& data, const string var, const string& signal, const string& cut, int nx, double *xbins, string opt = "", string leg = "", string tree="truth", vector<double> *errors = 0);

/// Draw 1D efficiency vs bin.
/// Errors are statistical only if errors variable isn't used. Added in quadrature with errors otherwise.
/// Calculates statistical uncertainties using binomial statistics.
void DrawEfficiency(DataSample& data, const string var, const string& signal, const string& cut, int nx, double xmin, double xmax, string opt = "", string leg = "", string tree="truth", vector<double> *errors = 0);

/// Draw purity * efficiency. Normal binning.
void DrawPurEff(DataSample& data, const string var, const string& signal, const string& cut, int nx, double xmin, double xmax, string opt = "", string leg = "", string tree="truth", vector<double> *errors = 0);

/// Draw purity * efficiency. Variable binning.
void DrawPurEff(DataSample& data, const string var, const string& signal, const string& cut, int nx, double *xbins, string opt = "", string leg = "", string tree="truth", vector<double> *errors = 0);

/// Get the predictions for the cross sections saved in a .root file <file_name>
/// Should work for importing any TGraph from a file.
/// The user will have to create the .root file and specify the path in the .cxx file.
void GetSpline(TString file_name, const char* graph_name, const string opt = "", Color_t col = kBlack, double LineWidth = 2, const string leg = "");

/// Fit a straight line to a histogram
void FitLine(TH1_h *hist);

/// Draw a plot showing how the purity, efficiency and pur*eff change with the VALUE of a cut.
/// Something like draw.PurEffVsCutVal(mc, "pullelec", ">", "particle==11", "accum_level[][0]>4", 20, -10, 10);
/// Will show the values of pur/eff/pur*eff for pullelec>-10, pullelec>-9 e.t.c
void PurEffVsCutVal(DataSample& mc, const string var, const string operat, const string signal, const string cut, int nx, double xmin, double xmax);

/// Draw a plot showing how the purity, efficiency and pur*eff change with the VALUE of a cut.
/// Something like draw.PurEffVsCutVal(mc, "pullelec", ">", "particle==11", "accum_level[][0]>4", nx, xbins);
/// Will show the values of pur/eff/pur*eff for pullelec>xbins[0], pullelec>-xbins[i] e.t.c
void PurEffVsCutVal(DataSample& mc, const string var, const string operat, const string signal, const string cut, int nx, double *xbins);

/// Prints out the optimum value of a cut in terms of purity, efficiency and pur*eff.
/// Cut values tested are determined by nx, xmin and xmax
/// Direction of cut is operat i.e. ">"
/// Can draw the distributions maximised by this function using PurEffVsCutVal(...).
void FindMaxPurEff(DataSample& mc, const string var, const string operat, const string signal, const string cut, int nx, double xmin, double xmax, double *result = NULL);

/// Prints out the optimum value of a cut in terms of purity, efficiency and pur*eff.
/// Cut values tested are determined by nx and xbins
/// Direction of cut is operat i.e. ">"
/// Can draw the distributions maximised by this function using PurEffVsCutVal(...).
void FindMaxPurEff(DataSample& mc, const string var, const string operat, const string signal, const string cut, int nx, double *xbins, double *result = NULL);

/// Prints out the optimum values of two cuts in terms of purity, efficiency and pur*eff.
/// Cut values tested are determined by nx, xmin and xmax, and ny, ymin and ymax.
/// Direction of cut is operat and operat2 i.e. ">" and "<".
/// Can draw the distributions maximised by this function using PurEffVsCutVal(...).
void FindMaxPurEff(DataSample& mc, const string var, const string operat, const string var2, const string operat2, const string signal, const string cut, int nx, double xmin, double xmax, int ny, double ymin, double ymax, double *res1 = NULL, double *res2 = NULL);

/// Prints out the optimum values of two cuts in terms of purity, efficiency and pur*eff.
/// Cut values tested are determined by nx, xbins, and ny, ybins.
/// Direction of cut is operat and operat2 i.e. ">" and "<".
/// Can draw the distributions maximised by this function using PurEffVsCutVal(...).
void FindMaxPurEff(DataSample& mc, const string var, const string operat, const string var2, const string operat2, const string signal, const string cut, int nx, double *xbins, int ny, double *ybins, double *res1 = NULL, double *res2 = NULL);

/// Draw the average of a variable for events that pass a certain cut in terms of another variable.
/// i.e draw.DrawAverage(mc, "tpcNNodes", "ElemomErr", ...) to see how "Elemom" changes with tpcNNodes.
/// Use min and max to exclude outlying values (so results aren't skewed by var = 9999999999)
/// Values to test determined by nx, xmin, xmax
void DrawAverage(DataSample& mc, const string var, const string var2, double min, double max, const string cut, int nx, double xmin, double xmax, const string opt = "AP", const string leg = "");

/// Draw the average of a variable for events that pass a certain cut in terms of another variable.
/// i.e draw.DrawAverage(mc, "tpcNNodes", "ElemomErr", ...) to see how "Elemom" changes with tpcNNodes.
/// Use min and max to exclude outlying values (so results aren't skewed by var = 9999999999)
/// Values to test determined by nx, xbins
void DrawAverage(DataSample& mc, const string var, const string var2, double min, double max, const string cut, int nx, double *xbins, const string opt = "AP", const string leg = "");

/// Get the selections defined by var, cut, nx, xmin, xmax
/// And calculate and draw (data-mc)/mc.
/// This can be fitted with a line is fitLine = true is passed.
void DataMCDiff(DataSample& data, DataSample& mc, const string var, const string cut, int nx, double xmin, double xmax, const string opt = "", const string leg = "", bool fitLine = false);

/// Get the selections defined by var, cut, nx, xbins.
/// And calculate and draw (data-mc)/mc.
/// This can be fitted with a line is fitLine = true is passed.
void DataMCDiff(DataSample& data, DataSample& mc, const string var, const string cut, int nx, double *xbins, const string opt = "", const string leg = "", bool fitLine = false);

/// Draws the histogram fetched from GetUnfolded(...)
/// Currently commented out because it requires the user to link extra ROOT libraries.
//void DrawUnfolded(DataSample& data, DataSample& mc, DataSample& ctrl, const string reco_var, int nreco, double *reco_bins, const string true_var, int ntrue, double *true_bins, int iterations, const string signal, const string cut, bool weight = false, const string opt = "", const string leg = "");

/// Currently commented out because it requires the user to link extra ROOT libraries.
/// Uses a control sample (ctrl) to unfold data.
/// Returns a histogram of "true_var" for data.
/// These are automatically weighted to efficiency and purity.
/// If you use weight = false, the scaling is done by the ctrl sample inside the unfolding methods.
/// Using weight = true, allows you to use an independent sample, mc, to do the weighting. This is probably better.
//TH1_h* GetUnfolded(DataSample& data, DataSample& mc, DataSample& ctrl, const string reco_var, int nreco, double *reco_bins, const string true_var, int ntrue, double *true_bins, int iterations, const string signal, const string cut, bool weight = false);


/// Set the title in Z
void SetTitleZ(const std::string& titleZ){_titleZ=titleZ;}

protected:
  std::string _titleZ;

};

#endif
