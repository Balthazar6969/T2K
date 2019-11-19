#ifndef __ccAnalysis_fit_HH__
#define __ccAnalysis_fit_HH__
#endif

class LookupTable {
//*******************************************************************
//
// Header file for ccAnalysis_fit
//
// Francesca Di Lodovico (October 2009)
//
//*******************************************************************

public:
  int _model ;
  enum NumberOfBinsEnu { nbinsene = 24 } ;
  enum NumberOfBins2Dim { nbins2dim = 4000 } ;
  enum NumberOfBinsMA { nbinsMA = 49 } ;
  enum BinHeight { maxhistoheight=900};
  Double_t pmu[nbins2dim];
  Double_t thetamu[nbins2dim];
  Double_t brCCnQE[nbins2dim];
  Double_t brCCQE[nbins2dim][nbinsMA];
  mutable Double_t brCCQEMA[nbins2dim];
  mutable Double_t brMA[nbinsene][nbins2dim];
  mutable Double_t brnQE[nbinsene][nbins2dim];
  mutable Double_t _thisbr[nbins2dim]; 
  mutable Double_t _thisbrenu[nbinsene]  ;
  mutable Double_t _thisbrenuMA[nbinsene]  ;
  bool _verbose ;
public:
  LookupTable(int model=0) ;
  const Double_t* calcspectrum(double* weights, double ratio, double MA, double* syst) const ;
  const Double_t* calcspectrumenu(double* weights, double ratio, double MA, double* syst) const ;
  const Double_t* calcspectrumenuraw(double MA) const ;
  void interpol(double MA) const;
  void draw(double* weights, double ratio, double MA, double* syst, int col=kRed) const ;
  void drawMA(double MA, int start=0, int col=kRed) const ;
  //Normalization factor:
  const double normal() const { return  2. ; } 
} ;

class ccAnalysis_fit{
  public:
  void Loop(ofstream *outfileresults, ofstream *outfilecorrelation, const char *eps, const char *pdf);
};

