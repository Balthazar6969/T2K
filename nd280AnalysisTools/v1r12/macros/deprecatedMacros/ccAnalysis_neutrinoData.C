#include <TMath.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iomanip>
#include "ccAnalysis_neutrinoData.hh"
#include <TH1F.h>
#include <TGraphAsymmErrors.h>
#include <TMatrixD.h> 
#include <TFile.h>
#include <TMatrixD.h>
#include <TRandom.h>
using namespace std;

double sqr(double x) { return x*x ; }

double eneuFermi(double Pmu, double Thetamu) {
//*******************************************************************
//
// Compute neutrino energy for CCQE events taking into
// account Fermi motion
//
//*******************************************************************
  // Put here the formula needed for CC-QE 
  const double Mn(0.93956536); //(MeV) neutron mass
  const double Mp(0.93827203); //(MeV) proton mass
  const double Mmu(0.10566);//(MeV) muon mass
  double Emu=pow(pow(Mmu,2)+pow(Pmu,2),0.5);
  double eps=0.027;//binding energy for Oxygen (27 MeV)
  return ((Mn-eps)*Emu-pow(Mmu,2)/2+(pow(Mp,2)-pow(Mn,2))/2-(pow(eps,2)/2-Mn*eps))/(Mn-eps-Emu+Pmu*Thetamu);
}

double eneu(double Pmu, double Thetamu) {
//*******************************************************************
//
// Compute neutrino energy for CCQE events WITHOUT taking into
// account Fermi motion
//
//*******************************************************************
  // Put here the formula needed for CC-QE 
  const double Mn(0.93956536); //(MeV) neutron mass
  const double Mp(0.93827203); //(MeV) proton mass
  const double Mmu(0.10566);//(MeV) muon mass
  double Emu=pow(pow(Mmu,2)+pow(Pmu,2),0.5);
  return (Mn*Emu-pow(Mmu,2)/2+(pow(Mp,2)-pow(Mn,2))/2)/(Mn-Emu+Pmu*Thetamu);
}

double 
MyBin::pmu() const 
{
  return pmuon;
}

double 
MyBin::thetamu() const 
{
  return thetamuon;

}

double 
MyBin::energy() const 
{
  return energynu;

}

double 
MyBin::pbf() const 
{
  return signal;

}

double 
MyBin::oldpbf() const 
{
  return oldsignal;

}

double 
MyBin::pbfstat() const 
{
  return statsignal;
}

double 
MyBin::pbfsysttot(bool plus) const
{
  //  return pbf() * (plus ? syst_plus : syst_plus )/100. ;
  return pbf() * 0.10;
}

void
MyBin::printpbf() const
{
  std::cout << setw(10) << pbf() << " +/- " 
	    << setw(10) << pbfstat() << "   +/-  "
	    << setw(10) << pbfsysttot(true) << " / " << pbfsysttot(false) << std::endl ;
}

void printmatrix(const TMatrixDBase& m, bool roundfuf=false){
//*******************************************************************
//
// Print input matrix
//
//*******************************************************************
  std::cout << "Print Matrix"<<endl; ;
  std::cout << std::setprecision(5) ;
  int nrow = m.GetNrows() ;
  int ncol = m.GetNcols() ;
  for(int irow=0; irow<nrow; ++irow) {
    for(int icol=0; icol<ncol; ++icol) {
      double x =  m(irow,icol) ;
      if( roundfuf && fabs(x) < 1e-10 ) x =0 ;
      std::cout << setw(14) << x  ;
    }
    std::cout << std::endl ;
  }
}

TMatrixDSym converttocorrelation( const TMatrixDSym& m){
//*******************************************************************
//
// Convert covariance matrix to correlation matrix
//
//*******************************************************************
  int nrow = m.GetNrows() ;
  TMatrixDSym rc(nrow) ;
  for( int row=0; row<nrow; ++row) 
    for( int col=0; col<nrow; ++col) 
      rc[row][col] = m[row][col] / sqrt( m[row][row] * m[col][col] ) ;
  return rc ;
}

ccAnalysis_neutrinoData::ccAnalysis_neutrinoData()
  : _systcov(nbins), _weightMatrix(nbins),_weightCorrMatrix(nbins)
{
//*******************************************************************
//
// Upload the data events.
//
// Of course, now the data events are the same than MC events, but 
// the events in each bin are randomized according to their statistical error
// There are some systematic errors implemented, but other implementations
// are under way so commented out.
//
// Francesca Di Lodovico (October 2009)
//
//*******************************************************************

  gRandom->SetSeed(65539);
  //  gRandom->SetSeed(0);

  // define dimensions
  //  const int sizecorr = nsystcorr ;
  const int size = nsyst ;

  /*
  // give all matrices the proper size
  _systcov.ResizeTo(sizecorr,sizecorr) ;
  
  ifstream tableinput("syst_table.txt") ;
  
  for( int row=0; row<sizecorr; ++row)
    for(int col=0; col<sizecorr; ++col) 
      _systcov(row,col) = 0 ;
  
  for( int row=0; row<sizecorr; ++row) {
    // the systematic errors are correlated for each row
    for( int col=0; col<sizecorr; ++col) {
      tableinput >> _systcov[row][col];
      _systcov[row][col] /= 100.;
    }
  }

  //Total systematics is given by the product of the systematic factors
  double totalsys(1); 
  //  for(int i=0; i<size; i++) totalsys *= (1+randomfac[i]);
  cout<<"totalsys "<<totalsys<<endl;
  */  

  double total(0);
  ifstream datainput("output/Enu_20_tree.txt") ;
  const Float_t myenu[] = {0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50, 0.55, 0.60, 0.65, 0.70, 0.75, 0.80, 0.85, 0.90, 0.95, 1.00, 1.10, 1.20, 1.30, 1.40, 1.50, 2.00}; 
  
  double testDataBin[nbinsEnu]; for(int i = 0 ; i<nbinsEnu; i++)testDataBin[i]=0;
  double testMCBin[nbinsEnu]; for(int i = 0 ; i<nbinsEnu; i++)testMCBin[i]=0;

  for(int row=0; row<nbins; ++row){
    gRandom->SetSeed(0);
    MyBin abin ;
    datainput >> abin.pmuon;
    datainput >> abin.thetamuon;
    double mysignal(0);
    datainput >> mysignal;
    double statfluctuaction(0);
    //    double normal(2.);
    double rdm = gRandom->Gaus(mysignal*normal(),pow(mysignal*normal(),.5));
    if(mysignal>0) statfluctuaction = 1+(mysignal*normal()-rdm)/(mysignal*normal());

    //    if(statfluctuaction*totalsys>0){
    //      abin.signal = mysignal*statfluctuaction*totalsys;
    if(statfluctuaction>0){
      abin.signal = mysignal*normal()*statfluctuaction;
    }else{
      abin.signal = mysignal*normal();
    }
    
    abin.oldsignal = mysignal*normal();
    abin.energynu= eneuFermi(abin.pmuon/1000.,abin.thetamuon);
    //    abin.energynu= eneu(abin.pmuon/1000.,abin.thetamuon);
    for(int k=0; k<nbinsEnu; ++k){
      if(abin.energynu < myenu[k+1] && abin.energynu>= myenu[k]){
	  testDataBin[k] += abin.signal; testMCBin[k] += mysignal; }}
    total += abin.signal;
    double dummy(0);
    abin.statsignal=sqrt( abin.signal);
    abin.syst_plus = 0.;
    abin.syst_minus = 0.;
    //    datainput >> dummy; abin.statsignal=sqrt( abin.signal);
    //    datainput >> abin.syst_plus;
    //    datainput >> abin.syst_minus;
    abin.istotalbin = row==nbins ;
    _bins[row] = abin  ;
    //    abin.printpbf();
  }
  datainput.close() ;
//  cout<<"final versus original bins"<<endl;for(int i = 0 ; i<nbinsEnu; i++) cout<<i<<" "<<testDataBin[i]<<" "<<testMCBin[i]<<" "<<endl;
  updateCache() ;

}


void ccAnalysis_neutrinoData::updateCache(){
//*******************************************************************
//
// This routine is meant to compute the correlation matrix
// correlated systematic errors
//
//*******************************************************************

/*
  // give all matrices the proper size
  const int size = nsystcorr ;

  // weight matrix (assuming everything is symmetric)
  TMatrixDSym totalcov = _systcov ;
  printmatrix(totalcov);
  _weightMatrix.ResizeTo(size,size) ;
  _weightMatrix = totalcov ;
  _weightMatrix.Invert(0) ;
  _weightCorrMatrix.ResizeTo(size,size) ;
  //  _weightCorrMatrix = converttocorrelation( totalcov ) ;
  //  printmatrix(_weightCorrMatrix);
  _weightCorrMatrix = totalcov ;
  _weightCorrMatrix.Invert(0) ;
  printmatrix(_weightCorrMatrix);

  // some more caching
  for( int row=0; row<size; ++row)
    _pbferr[row]  = _systcov(row,row) ;

*/

  // cache the pbfs
  for( int row=0; row<nbins; ++row){ 
    _pbf[row] = _bins[row].pbf() ;
    _oldpbf[row] = _bins[row].oldpbf() ;
    _energy[row] = _bins[row].energy() ;
  }

}

void
ccAnalysis_neutrinoData::drawEnuSpectrum() const {
//*******************************************************************
//
// Draw energy spectrum (computed w/o Fermi motion)
//
//*******************************************************************

  const Float_t myenu[] = {0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50, 0.55, 0.60, 0.65, 0.70, 0.75, 0.80, 0.85, 0.90, 0.95, 1.00, 1.10, 1.20, 1.30, 1.40, 1.50, 2.00}; 

  TH1F h1("enudata","E_{#nu} spectrum",nbinsEnu,myenu) ;
  h1.SetXTitle("E_{#nu} (GeV)");
  h1.SetYTitle("Events/arbitrary units");
  
  TGraphAsymmErrors* grall  = new TGraphAsymmErrors(nbinsEnu) ;
  TGraphAsymmErrors* grstat = new TGraphAsymmErrors(nbinsEnu) ;
  Double_t bineneu[nbinsEnu], oldbineneu[nbinsEnu], bineneustat[nbinsEnu], bineneusyst[nbinsEnu]; 
  for(int j=0; j<nbinsEnu; ++j) {
    bineneu[j] =  0; 
    oldbineneu[j] =  0; 
    bineneustat[j] = 0; 
    bineneusyst[j] = 0; 
  }
  for(int j=0; j<nbins; ++j) {
    Double_t enucal = eneuFermi(_bins[j].pmu()/1000.,_bins[j].thetamu());
    for(int k=0; k<nbinsEnu; ++k){
      if(enucal <= myenu[k+1] && enucal> myenu[k]) {
	//	bineneu[k] += _bins[j].pbf();
	bineneu[k] += _pbf[j];
	oldbineneu[k] += _bins[j].oldpbf();
	bineneustat[k] += sqr(_bins[j].pbfstat());
	bineneusyst[k] += sqr(_bins[j].pbfsysttot(true)); 
	break;
      }
    }
  }  
    
  for(int i=0; i<nbinsEnu; ++i) {
    double fraction  = bineneu[i]/oldbineneu[i];
    printf("bineneu: %2i  %3.1f  %3.1f  %3.5f  \n",i,bineneu[i],oldbineneu[i],fraction);
    if(i>(nbinsEnu-7)&&i<(nbinsEnu-1)){
      bineneu[i] /=2.;
      bineneustat[i] /=2.;
      bineneusyst[i] /=2.;
    }else if(i==(nbinsEnu-1)){
      bineneu[i] /=10.;
      bineneustat[i] /=10.;
      bineneusyst[i] /=10.;
    }      
    h1.SetBinContent(i+1,bineneu[i]) ;
    grall->SetPoint(i,h1.GetBinCenter(i+1),bineneu[i]) ;
    grall->SetPointEYlow(i,sqrt( bineneustat[i] + bineneusyst[i])) ;
    grall->SetPointEYhigh(i,sqrt( bineneustat[i] + bineneusyst[i])) ;
    grstat->SetPoint(i,h1.GetBinCenter(i+1),bineneu[i]) ;
    grstat->SetPointEYlow(i,sqrt( bineneustat[i])) ;
    grstat->SetPointEYhigh(i,sqrt( bineneustat[i])) ;
  }

  h1.SetMinimum(0.) ;
  h1.SetMaximum(maxhistohight) ;
  h1.DrawCopy() ;
  grall->SetLineColor(kBlue) ;
  grstat->SetMarkerStyle(20) ;
  grstat->SetMarkerSize(0.5) ;
  grall->Draw("P") ;
  grstat->Draw("P") ;

}

void
ccAnalysis_neutrinoData::drawEnuSpectrumFermi() const {
//*******************************************************************
//
// Draw energy spectrum compute w/ Fermi motion
//
//*******************************************************************
  const Float_t myenu[] = {0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50, 0.55, 0.60, 0.65, 0.70, 0.75, 0.80, 0.85, 0.90, 0.95, 1.00, 1.10, 1.20, 1.30, 1.40, 1.50, 2.00}; 

  TH1F h1("enudata","E_{#nu} spectrum",nbinsEnu,myenu) ;
  h1.SetXTitle("E_{#nu} (GeV)");
  h1.SetYTitle("Events/arbitrary units");
  TH1F h11("enudatafermi","E_{#nu} noFermi spectrum",nbinsEnu,myenu) ;
  h11.SetXTitle("E_{#nu} (GeV)");
  h11.SetYTitle("Events/arbitrary units");
  
  TGraphAsymmErrors* grall  = new TGraphAsymmErrors(nbinsEnu) ;
  TGraphAsymmErrors* grstat = new TGraphAsymmErrors(nbinsEnu) ;
  Double_t bineneu[nbinsEnu], nofermibineneu[nbinsEnu];
  for(int j=0; j<nbinsEnu; ++j) {
    bineneu[j] =  0; 
    nofermibineneu[j] =  0; 
  }
  for(int j=0; j<nbins; ++j) {
    Double_t enucalFermi = eneuFermi(_bins[j].pmu()/1000.,_bins[j].thetamu());
    for(int k=0; k<nbinsEnu; ++k){
      if(enucalFermi <= myenu[k+1] && enucalFermi> myenu[k]) {
	bineneu[k] += _bins[j].pbf();
	break;
      }
    }
  }  
  for(int j=0; j<nbins; ++j) {
    Double_t enucal = eneu(_bins[j].pmu()/1000.,_bins[j].thetamu());
    for(int k=0; k<nbinsEnu; ++k){
      if(enucal <= myenu[k+1] && enucal> myenu[k]) {
	nofermibineneu[k] += _bins[j].pbf();
	break;
      }
    }
  }  
    
  for(int i=0; i<nbinsEnu; ++i) {
    if(i>(nbinsEnu-7)&&i<(nbinsEnu-1)){
      bineneu[i] /=2.;
      nofermibineneu[i] /=2.;
    }else if(i==(nbinsEnu-1)){
      bineneu[i] /=10.;
      nofermibineneu[i] /=10.;
    }      
    h1.SetBinContent(i+1,bineneu[i]) ;
    h11.SetBinContent(i+1,nofermibineneu[i]) ;
  }
  

  h1.SetMinimum(0.) ;
  h1.SetMaximum(maxhistohight) ;
  h1.SetLineColor(kBlue) ;
  h1.DrawCopy() ;
  h11.SetLineColor(kRed) ;
  h11.DrawCopy("same hist") ;

}

double
ccAnalysis_neutrinoData::likelihood(const double* thespectrum, const double* systparam) const {
//*******************************************************************
//
// Likelihood function
//
//*******************************************************************

  const Float_t myenu[] = {0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50, 0.55, 0.60, 0.65, 0.70, 0.75, 0.80, 0.85, 0.90, 0.95, 1.00, 1.10, 1.20, 1.30, 1.40, 1.50, 2.00}; 

  double testDataBin[nbinsEnu]; for(int i = 0 ; i<nbinsEnu; i++)testDataBin[i]=0;
  double testMCBin[nbinsEnu]; for(int i = 0 ; i<nbinsEnu; i++)testMCBin[i]=0;

  const int size = nbins ;
  //  const int sizecorr = nsystcorr ;

  // Poisson Contribution to the Likelihood.
  int ndof(0);
  double poissond(0);
  for(int i=0; i<size; ++i) {
    //    cout<<i<<" "<<thespectrum[i]<<" "<< _pbf[i]<<endl;
    if(thespectrum[i]<=0 || _pbf[i]<=0) continue;
    double residual = thespectrum[i] - _pbf[i] ;
    //    cout<<"residual "<<i<<" "<<residual<<" "<< thespectrum[i]<<" "<< _pbf[i]<<endl;
    poissond += residual + _pbf[i]*log(_pbf[i]/thespectrum[i]); 
    // Compute the total amount of event in each energy data bin in data and MC
    for(int k=0; k<nbinsEnu; ++k){
      if(_energy[i] <= myenu[k+1] && _energy[i] > myenu[k]){ ++ndof; testDataBin[k] += _pbf[i]; testMCBin[k] += thespectrum[i];}}
  }

  //Correlated systematic errors
  //  TMatrixD chi(1,sizecorr) ;
  //  for(int i=0; i<sizecorr; ++i) {
  //    double residual = 1 - systparam[i] ;
  //    chi(0,i) = residual/_pbferr[i] ;
  //  }

  //  TMatrixDSym w(_weightCorrMatrix) ;
  //  w.Similarity( chi ) ;

  //Likelihood
  double likelihood = poissond + //0.5*w(0,0) +

    0.5*sqr((1-systparam[0])/0.02)+
    //    0.5*sqr((1-systparam[0])/0.10)+
    0.5*sqr((1-systparam[1])/0.04)+
    //    sqr((1-systparam[2])/0.03)+
    0.5*sqr((1-systparam[3])/0.02)+
    0.5*sqr((1-systparam[4])/0.02)+
    0.5*sqr((1-systparam[5])/0.03)+
    0.5*sqr((1-systparam[6])/0.04)+
    0.5*sqr((1-systparam[7])/0.03)+
    0.5*sqr((1-systparam[8])/0.04)+
    0.5*sqr((1-systparam[9])/0.12);

  //  double likelihood = w(0,0);
  return likelihood;
}
