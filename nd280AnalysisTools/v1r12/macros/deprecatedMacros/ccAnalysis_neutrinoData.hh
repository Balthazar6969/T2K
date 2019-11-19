#ifndef __ccAnalysis_neutrinoData_HH__
#define __ccAnalysis_neutrinoData_HH__

#include <TMatrixDSym.h>
//*******************************************************************
//
// Header file associated to ccAnalysis_neutrinoData.C
//
// Francesca Di Lodovico (October 2009)
//
//*******************************************************************

class MyBin
{
public:
  double pmuon ;
  double thetamuon ;
  double energynu ;
  double signal ;
  double oldsignal ;
  double statsignal ;
  double syst_plus ;   // systematic error in %
  double syst_minus ;
  double syst_plus_table[71] ;
  double syst_minus_table[71] ;
  bool   istotalbin ;
  //  static double eneu( double emu, double thetamu ) ;

  //  double binwidthPer100MeV() const { return (Enumax-Enumin)/0.5 ; }
  double pmu() const ;
  double thetamu() const ;
  double energy() const ;
  double pbf() const ;
  double oldpbf() const ;
  double pbfstat() const ;
  double pbfsysttot(bool plus) const ;
  void printpbf() const ;
} ;

class ccAnalysis_neutrinoData 
{
private:
  enum NumberOfBins { nbins=4000 } ;
  enum NumberOfBinsEnu { nbinsEnu=24 } ;
  enum NumberOfSystErr { nsystcorr=2 } ;
  enum NumberOfCorrSystErr { nsyst=9 } ;
  enum BinHeight { maxhistohight=900 } ;
  MyBin _bins[nbins+1] ;
  // cached for speed
  TMatrixDSym _statcov ;
  TMatrixDSym _systcov ;
  TMatrixDSym _systcholesky ;
  TMatrixDSym _weightMatrix ;
  TMatrixDSym _weightCorrMatrix ; // inverted correlation matrix
  double _energy[nbins+1] ;
  double _pbf[nbins+1] ;
  double _oldpbf[nbins+1] ;
  double _pbferr[nsyst] ;
  static double _enu[nbins+2] ;
private:
public:
  ccAnalysis_neutrinoData() ;
  void drawEnuSpectrum() const ;
  void drawEnuSpectrumFermi() const ;
  double likelihood(const double* thespectrum, const double* systparam) const ;
  const MyBin& bin(int i) const { return  _bins[i] ; }
  void updateCache()  ;
  //normalization:
  const double normal() const { return  2. ; } 

} ;


#endif
