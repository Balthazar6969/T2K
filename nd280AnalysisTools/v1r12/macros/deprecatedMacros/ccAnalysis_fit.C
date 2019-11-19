//*CMZ :  0.90/09 03/12/96  17.45.55  by  Rene Brun
//*-- Author :    Rene Brun   22/08/95
// ---------------------------------- minexam.C

// Copyright (C) 1994 CodeCERN. All rights reserved.

//______________________________________________________________________________
//*-*-*-*-*-*-*-*-*-*-*-*The Minuit standard test program-*-*-*-*-*-*-*-*-*
//*-*                    ========================                         *
//*-*                                                                     *
//*-*    This program is the translation to C++ of the minexam program    *
//*-*    distributed with the Minuit/Fortran source file.                 *
//*-*         original author Fred James                                  *
//*-*                                                                     *
//*-*                                                                     * 
//*-*                                                                     *
//*-*    Initial fit to extract the Enu distribution from the npls        *
//*-*    produced from the CCnumu analysis in ccAnalysis.C                *
//*-*    This code is evolving.                                           *
//*-*                                                                     *
//*-*    Francesca Di Lodovico (October 2009)                             *
//*-*                                                                     *
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

#include "TROOT.h"
#include "TH1.h"
#include "TH1F.h"
#include "TMinuit.h"
#include "TMath.h"
#include "TGraph.h"
#include "TStopwatch.h"
#include "TCanvas.h"
#include "TMarker.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TGraphAsymmErrors.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <string.h>
#include <fstream>
#include <strstream>
#include "TMatrixD.h"
#include "ccAnalysis_neutrinoData.hh"
#include <math.h>
#include "ccAnalysis_fit.hh"
using namespace std;

#ifdef __SC__
long G__globalvarpointer; // To make the Symantec linker happy
#endif

LookupTable::LookupTable(int model)
  : _model(model),_verbose(false){
//*******************************************************************
//
// Read the MC data information
//
//*******************************************************************

  // initialize the lookuptables

  for(int j=0; j<nbins2dim; j++){
    brCCnQE[j]=0; pmu[j]=0; thetamu[j]=0;
    for(int k=0; k<nbinsMA; k++){
      brCCQE[j][k]=0;	 }}
  
  for(int k=0; k<nbinsMA; k++){ 
    ostrstream base; base<<"output/EnuCCQE_"<<k<<"_tree.txt"<<ends; string name; name=base.str();
    ifstream infile(name.c_str()); 
    for(int j=0; j<nbins2dim; j++)  infile>>pmu[j]>>thetamu[j]>>brCCQE[j][k];  
    infile.close();}
  
  ostrstream basenqe; basenqe<<"output/EnuCCnQE_20_tree.txt"<<ends; string namenqe; namenqe=basenqe.str(); 
  ifstream infilenqe(namenqe.c_str()); 
  for(int j=0; j<nbins2dim; j++)    infilenqe>>pmu[j]>>thetamu[j]>>brCCnQE[j]; // printf("%3i %3i %3i %3.3f \n%",i,j,k,brCCnQE[j]);	 	 
  infilenqe.close();
  
  for(int j=0; j<nbins2dim; j++) {brCCnQE[j] *= normal(); for(int k=0; k<nbinsMA; k++) {brCCQE[j][k] *= normal();}}
  
}

void
LookupTable::interpol(double MA) const {
//*******************************************************************
//
// Interpolate among the MA values
//
//*******************************************************************


  int ima(-1000);
  for(int k=0; k<nbinsMA; ++k){
    double MAmin = 0.1 + float(k)*0.05;
    double MAmax = 0.15 + float(k)*0.05;
    if(MAmin>MA && MA < MAmax){ima=k; break;}
    if(k==0&&MA==MAmin){ima=k; break;}
    if(MA==2.5){ima=(nbinsMA-1); break;}
  }

  double MAxial[nbinsMA];
  for(int k=0; k < nbinsMA; k++) MAxial[k]=0.10+float(k)*0.05;

  for(int j=0; j<nbins2dim; j++){
    brCCQEMA[j] = 0;
    if(ima==(nbinsMA-1)){brCCQEMA[j] = brCCQE[j][ima];}else{
      brCCQEMA[j] = brCCQE[j][ima]+(MA-MAxial[ima])*(brCCQE[j][ima+1]- brCCQE[j][ima])/(MAxial[ima+1]-MAxial[ima]);
    }
  }

}

double eneufit(double Pmu, double Thetamu) {
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
  return ((Mn-eps)*Emu-pow(Mmu,2)/2+(pow(Mp,2)-pow(Mn,2))/2-(pow(eps,2)/2-Mn*eps))/(Mn-eps-Emu+Pmu*Thetamu);//mine

}

const Double_t*
LookupTable::calcspectrum(double* weights, double ratio, double MA, double* syst) const {
//*******************************************************************
//
// Compute neutrino (Pmu, costhetamu) spectrum according to 
// fitted coefficients
//
//*******************************************************************

  interpol(MA);
  
  const Float_t myenu[] = {0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50, 0.55, 0.60, 0.65, 0.70, 0.75, 0.80, 0.85, 0.90, 0.95, 1.00, 1.10, 1.20, 1.30, 1.40, 1.50, 2.00}; 

  //move from brCCQEMA total to brMA energy dependent
  //move from brCCnQE total to brnQE energy dependent
  for(int k=0; k<nbinsene; ++k){for(int j=0; j<nbins2dim; ++j) {brMA[k][j] = 0; brnQE[k][j] = 0;}}
  for(int j=0; j<nbins2dim; ++j) {
    Double_t enucal = eneufit(pmu[j]*syst[0]/1000., thetamu[j]);
    Double_t enucalnormal = eneufit(pmu[j]/1000., thetamu[j]);
    for(int k=0; k<nbinsene; ++k){
      if(enucal < myenu[k+1] && enucal>= myenu[k]) {brMA[k][j] = brCCQEMA[j]; brnQE[k][j] = brCCnQE[j];}}}

  for(int j=0; j<nbins2dim; ++j) _thisbr[j] = 0;
  for(int j=0; j<nbins2dim; ++j) {
    for(int i=0; i<nbinsene; ++i) {
      _thisbr[j] += weights[i]*(brMA[i][j]+ratio*brnQE[i][j]);
    }
  }

  return _thisbr ;
}

const Double_t*
LookupTable::calcspectrumenu(double* weights, double ratio, double MA, double* syst) const {
//*******************************************************************
//
// Compute neutrino energy spectrum according to 
// fitted coefficients
//
//*******************************************************************

  interpol(MA);
  const Float_t myenu[] = {0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50, 0.55, 0.60, 0.65, 0.70, 0.75, 0.80, 0.85, 0.90, 0.95, 1.00, 1.10, 1.20, 1.30, 1.40, 1.50, 2.00}; 
 
  //move from brCCQEMA total to brMA energy dependent
  //move from brCCnQE total to brnQE energy dependent
  for(int k=0; k<nbinsene; ++k){for(int j=0; j<nbins2dim; ++j) {brMA[k][j] = 0; brnQE[k][j] = 0;}}
  for(int j=0; j<nbins2dim; ++j) {
    Double_t enucal = eneufit(pmu[j]*syst[0]/1000., thetamu[j]);
    Double_t enucalnormal = eneufit(pmu[j]/1000., thetamu[j]);
    for(int k=0; k<nbinsene; ++k){
      if(enucal <= myenu[k+1] && enucal> myenu[k]) {brMA[k][j] = brCCQEMA[j]; brnQE[k][j] = brCCnQE[j];}}}
    
  for(int k=0; k<nbinsene; ++k)_thisbrenu[k] = 0;
  for(int k=0; k<nbinsene; ++k){
    for(int j=0; j<nbins2dim; ++j) {if(brMA[k][j]!=0 || brnQE[k][j] !=0 ){
      _thisbrenu[k] += weights[k]*(brMA[k][j]+ratio*brnQE[k][j]);
      }
    }
  }
  
  double norm(0);
  for(int k=0; k<nbinsene; ++k){ printf("calcspectrumenu: %2i  %4.3f %4.3f %4.3f  %4.3f \n",k,_thisbrenu[k],weights[k]); norm += _thisbrenu[k]; }

  std::cout << setprecision(10) ;
  std::cout << setw(14) << "Normalization: "<<norm<<endl;

  return _thisbrenu ;
}


const Double_t*
LookupTable::calcspectrumenuraw(double MA) const {
//*******************************************************************
//
// Compute neutrino energy spectrum without Fermi motion according to 
// fitted coefficients
//
//*******************************************************************

  interpol(MA);
  
  //  const Float_t myenu[] = {0.10, 0.20, 0.30, 0.40, 0.50, 0.60, 0.70, 0.80, 0.90, 1.00, 1.10, 1.20, 1.30, 1.40, 1.50, 2.00}; 
  const Float_t myenu[] = {0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50, 0.55, 0.60, 0.65, 0.70, 0.75, 0.80, 0.85, 0.90, 0.95, 1.00, 1.10, 1.20, 1.30, 1.40, 1.50, 2.00}; 
  
  //move from brCCQEMA total to brMA energy dependent
  //move from brCCnQE total to brnQE energy dependent
  for(int k=0; k<nbinsene; ++k){for(int j=0; j<nbins2dim; ++j) {brMA[k][j] = 0; brnQE[k][j] = 0;}}
  for(int j=0; j<nbins2dim; ++j) {
    Double_t enucal = eneufit(pmu[j]/1000., thetamu[j]);
    for(int k=0; k<nbinsene; ++k){
      if(enucal <= myenu[k+1] && enucal> myenu[k]) {brMA[k][j] = brCCQEMA[j]; brnQE[k][j] = brCCnQE[j];}}}
    
  for(int i=0; i<nbinsene; ++i) {
    _thisbrenuMA[i] = 0;
    for(int j=0; j<nbins2dim; ++j) {
      _thisbrenuMA[i] += brMA[i][j];
    }
  }

  return _thisbrenuMA ;
}


void
LookupTable::draw(double* Weights, double Ratio, double MA, double* syst, int col) const {
//*******************************************************************
//
// Draw energy spectrum after the fit
//
//*******************************************************************

  const Float_t myenu[] = {0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50, 0.55, 0.60, 0.65, 0.70, 0.75, 0.80, 0.85, 0.90, 0.95, 1.00, 1.10, 1.20, 1.30, 1.40, 1.50, 2.00}; 

  int nplotbins(nbinsene) ;
  TH1D h1("lookuptable","",nplotbins,myenu) ;
  const Double_t* thespectrum = calcspectrumenu(Weights,Ratio, MA, syst);
  for(int i=0; i<nplotbins; ++i){ 
    h1.SetBinContent(i+1,thespectrum[i]) ;
    if(i>(nplotbins-7)&&i<(nplotbins-1)){
      h1.SetBinContent(i+1,thespectrum[i]/2.) ;
    }else if(i==(nplotbins-1)){
      h1.SetBinContent(i+1,thespectrum[i]/10.) ;
    }      
    double pippo = h1.GetBinContent(i+1) ;
  }
  h1.SetLineColor(col) ;
  h1.DrawCopy("same hist") ;
}

void
LookupTable::drawMA(double MA, int start, int col) const {
//*******************************************************************
//
// Draw MA spectrum
//
//*******************************************************************

  const Float_t myenu[] = {0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50, 0.55, 0.60, 0.65, 0.70, 0.75, 0.80, 0.85, 0.90, 0.95, 1.00, 1.10, 1.20, 1.30, 1.40, 1.50, 2.00}; 

  int nplotbins(nbinsene) ;
  TH1D h1("lookuptable","",nplotbins,myenu) ;
  h1.SetMaximum(maxhistoheight) ;
  const Double_t* thespectrum = calcspectrumenuraw(MA);
  for(int i=0; i<nplotbins; ++i){
    if(thespectrum[i]>maxhistoheight)cout<<"wrong spectrum?? MA: "<<MA<<endl;
    h1.SetBinContent(i+1,thespectrum[i]) ;
    if(i>(nplotbins-7)&&i<(nplotbins-1)){
      h1.SetBinContent(i+1,thespectrum[i]/2.) ;
    }else if(i==(nplotbins-1)){
      h1.SetBinContent(i+1,thespectrum[i]/10.) ;
    }      
  }
  h1.SetLineColor(col) ;
  if(start){ h1.DrawCopy() ;
  }else{h1.DrawCopy("same hist") ;}
}

ccAnalysis_neutrinoData thedata ;
LookupTable    thelookuptable ;

void chisquare(Int_t &npar, Double_t *gin, Double_t &f, Double_t *x, Int_t iflag) {
//*******************************************************************
//
// Input chi2square for TMinuit fit
//
//*******************************************************************

  Double_t weights[24];
  for(int i=0; i<24; i++){    weights[i]=0.; weights[i] = x[i];}
  Double_t MA = x[24] ; 
  Double_t Ratio = x[25] ; 
  Double_t syst[10];
  syst[0] = x[26] ; 
  syst[1] = x[27] ; 
  syst[2] = x[28] ; 
  syst[3] = x[29] ; 
  syst[4] = x[30] ; 
  syst[5] = x[31] ; 
  syst[6] = x[32] ; 
  syst[7] = x[33] ; 
  syst[8] = x[34] ; 
  syst[9] = x[35] ; 
  f= thedata.likelihood(thelookuptable.calcspectrum(weights,Ratio, MA, syst), syst) ;

}

void drawspectrum( const LookupTable& lt, TMinuit* aminuit, int col) {
//*******************************************************************
//
// Draw energy spectrum after fit
//
//*******************************************************************

  Double_t weights[24], err_weights[24];
  Double_t ma(0), err_ma(0);
  Double_t ratio(0), err_ratio(0);
  Double_t syst[10], err_syst[10];
  aminuit->GetParameter(0, weights[0], err_weights[0]); 
  aminuit->GetParameter(1, weights[1], err_weights[1]); 
  aminuit->GetParameter(2, weights[2], err_weights[2]); 
  aminuit->GetParameter(3, weights[3], err_weights[3]); 
  aminuit->GetParameter(4, weights[4], err_weights[4]); 
  aminuit->GetParameter(5, weights[5], err_weights[5]); 
  aminuit->GetParameter(6, weights[6], err_weights[6]); 
  aminuit->GetParameter(7, weights[7], err_weights[7]); 
  aminuit->GetParameter(8, weights[8], err_weights[8]); 
  aminuit->GetParameter(9, weights[9], err_weights[9]); 
  aminuit->GetParameter(10, weights[10], err_weights[10]); 
  aminuit->GetParameter(11, weights[11], err_weights[11]); 
  aminuit->GetParameter(12, weights[12], err_weights[12]); 
  aminuit->GetParameter(13, weights[13], err_weights[13]); 
  aminuit->GetParameter(14, weights[14], err_weights[14]); 
  aminuit->GetParameter(15, weights[15], err_weights[15]); 
  aminuit->GetParameter(16, weights[16], err_weights[16]); 
  aminuit->GetParameter(17, weights[17], err_weights[17]); 
  aminuit->GetParameter(18, weights[18], err_weights[18]); 
  aminuit->GetParameter(19, weights[19], err_weights[19]); 
  aminuit->GetParameter(20, weights[20], err_weights[20]); 
  aminuit->GetParameter(21, weights[21], err_weights[21]); 
  aminuit->GetParameter(22, weights[22], err_weights[22]); 
  aminuit->GetParameter(23, weights[23], err_weights[23]); 

  aminuit->GetParameter(24, ma, err_ma); 
  aminuit->GetParameter(25, ratio, err_ratio); 
  aminuit->GetParameter(26, syst[0], err_syst[0]); 
  aminuit->GetParameter(27, syst[1], err_syst[1]); 
  aminuit->GetParameter(28, syst[2], err_syst[2]); 
  aminuit->GetParameter(29, syst[3], err_syst[3]); 
  aminuit->GetParameter(30, syst[4], err_syst[4]); 
  aminuit->GetParameter(31, syst[5], err_syst[5]); 
  aminuit->GetParameter(32, syst[6], err_syst[6]); 
  aminuit->GetParameter(33, syst[7], err_syst[7]); 
  aminuit->GetParameter(34, syst[8], err_syst[8]); 
  aminuit->GetParameter(35, syst[9], err_syst[9]); 
  lt.draw(weights, ratio, ma, syst,col) ;
}

void drawMAspectra(const LookupTable& lt) {
//*******************************************************************
//
// Draw MA spectrum after fit
//
//*******************************************************************


  lt.drawMA(1.1,1,kBlue) ;

  int nbinsMA(47);
  for(int k=0; k<nbinsMA; k++){
    double mamin(0.10); double mastep(0.05);
    double ma = mamin+float(k)*mastep;
    if(ma!=1.1)lt.drawMA(ma,0, kRed) ;
  }
  lt.drawMA(1.1,0,kBlue) ;

}

void ccAnalysis_fit::Loop(ofstream *outfileresults, ofstream *outfilecorrelation, const char *eps, const char *pdf) {
//*******************************************************************
//
// This function perform the actual to the Pmu, costhemu distribution 
// using the files produced by the process_tree.C class
//
// Legenda of the parameters
//
// weights: multiplicatibe factors for each energy
// ma: Axial Mass
// ratio: ratio between nCCQE and CCqe
// syst[0]: Pmu scal
// syst[1]: 
// syst[2]: 
// syst[3]: 
// syst[4]: 
// syst[5]: 
// syst[6]: 
// syst[7]: 
// syst[8]: 
// syst[9]: 
//
//
//*******************************************************************

  TStopwatch timer;
  
  Int_t ierflg = 0;
  Double_t arglist[35];
  arglist[0] = 5000;   //MaxIterations
  
  TMinuit *gMinuit = new TMinuit(35);  //initialize TMinuit with a maximum of 35 params
  timer.Start();
  gMinuit->SetFCN(chisquare);

  gMinuit->mnparm(0, "weights[0]",  1., 0.00005, 0., 3.5, ierflg);
  gMinuit->mnparm(1, "weights[1]",  1., 0.00005, 0., 3.5, ierflg);
  gMinuit->mnparm(2, "weights[2]",  1., 0.00005, 0., 3.5, ierflg);
  gMinuit->mnparm(3, "weights[3]",  1., 0.00005, 0., 3.5, ierflg);
  gMinuit->mnparm(4, "weights[4]",  1., 0.00005, 0., 3.5, ierflg);
  gMinuit->mnparm(5, "weights[5]",  1, 0.05, 0., 3.5, ierflg);
  gMinuit->mnparm(6, "weights[6]",  1, 0.05, 0., 3.5, ierflg);
  gMinuit->mnparm(7, "weights[7]",  1, 0.05, 0., 3.5, ierflg);
  gMinuit->mnparm(8, "weights[8]",  1, 0.05, 0., 3.5, ierflg);
  gMinuit->mnparm(9, "weights[9]",  1, 0.05, 0., 3.5, ierflg);
  gMinuit->mnparm(10, "weights[10]",  1, 0.05, 0., 3.5, ierflg);
  gMinuit->mnparm(11, "weights[11]",  1, 0.05, 0., 3.5, ierflg);
  gMinuit->mnparm(12, "weights[12]",  1, 0.05, 0., 3.5, ierflg);
  gMinuit->mnparm(13, "weights[13]",  1, 0.05, 0., 3.5, ierflg);
  gMinuit->mnparm(14, "weights[14]",  1, 0.05, 0., 3.5, ierflg);
  gMinuit->mnparm(15, "weights[15]",  1, 0.05, 0., 3.5, ierflg);
  gMinuit->mnparm(16, "weights[16]",  1, 0.05, 0., 3.5, ierflg);
  gMinuit->mnparm(17, "weights[17]",  1, 0.05, 0., 3.5, ierflg);
  gMinuit->mnparm(18, "weights[18]",  1, 0.05, 0., 3.5, ierflg);
  gMinuit->mnparm(19, "weights[19]",  1, 0.05, 0., 3.5, ierflg);
  gMinuit->mnparm(20, "weights[20]",  1, 0.05, 0., 3.5, ierflg);
  gMinuit->mnparm(21, "weights[21]",  1, 0.05, 0., 3.5, ierflg);
  gMinuit->mnparm(22, "weights[22]",  1, 0.05, 0., 3.5, ierflg);
  gMinuit->mnparm(23, "weights[23]",  1, 0.05, 0., 3.5, ierflg);
  gMinuit->mnparm(24, "MA",  1., 0.01, 0.1, 2.1, ierflg);
  gMinuit->mnparm(25, "ratio",  1, 0.05, 0., 3.5, ierflg);
  gMinuit->mnparm(26, "syst[0]",  1., 0.05, 0., 2.5, ierflg);
  gMinuit->mnparm(27, "syst[1]",  1, 0.05, 0., 2.5, ierflg);
  gMinuit->mnparm(28, "syst[2]",  0, 0.05, -2.5, 2.5, ierflg);
  gMinuit->mnparm(29, "syst[3]",  1, 0.05, 0., 2.5, ierflg);
  gMinuit->mnparm(30, "syst[4]",  1, 0.05, 0., 2.5, ierflg);
  gMinuit->mnparm(31, "syst[5]",  1, 0.05, 0., 2.5, ierflg);
  gMinuit->mnparm(32, "syst[6]",  1, 0.05, 0., 2.5, ierflg);
  gMinuit->mnparm(33, "syst[7]",  1, 0.05, 0., 2.5, ierflg);
  gMinuit->mnparm(34, "syst[8]",  1, 0.05, 0., 2.5, ierflg);
  gMinuit->mnparm(35, "syst[9]",  1, 0.05, 0., 2.5, ierflg);
  gMinuit->SetErrorDef(1);
    
  //*-*-       Request FCN to read in (or generate random) data (IFLAG=1)

  //    gMinuit->FixParameter(0);
  //    gMinuit->FixParameter(1);
  //    gMinuit->FixParameter(2);
  //    gMinuit->FixParameter(3);
  //    gMinuit->FixParameter(4);
  //    gMinuit->FixParameter(5);
  //    gMinuit->FixParameter(6);
  //    gMinuit->FixParameter(7);
  //    gMinuit->FixParameter(8);
  //    gMinuit->FixParameter(9);
  //    gMinuit->FixParameter(10);
  //    gMinuit->FixParameter(11);
  //    gMinuit->FixParameter(12);
  //    gMinuit->FixParameter(13);
  //    gMinuit->FixParameter(14);
  //  gMinuit->FixParameter(15);
  //  gMinuit->FixParameter(16);
  //  gMinuit->FixParameter(17);
  //  gMinuit->FixParameter(18);
  //  gMinuit->FixParameter(19);
  //  gMinuit->FixParameter(20);
  //  gMinuit->FixParameter(21);
  //  gMinuit->FixParameter(22);
  //  gMinuit->FixParameter(23);
  //  gMinuit->FixParameter(24);
  //    gMinuit->FixParameter(25);
  gMinuit->FixParameter(26);
  gMinuit->FixParameter(27);
  gMinuit->FixParameter(28);
  gMinuit->FixParameter(29);
  gMinuit->FixParameter(30);
  gMinuit->FixParameter(31);
  gMinuit->FixParameter(32);
  gMinuit->FixParameter(33);
  gMinuit->FixParameter(34);
  gMinuit->FixParameter(35);

  //  gMinuit->Release(0);
  gMinuit->mnexcm("HESSE", arglist ,1,ierflg);
  //  gMinuit->mnexcm("MIGRAD", arglist ,1,ierflg);
  //  gMinuit->mnexcm("MINOS", arglist ,1,ierflg);
  //  gMinuit->mnexcm("MIGRAD", arglist ,1,ierflg);
  gMinuit->mnexcm("MIGRAD", arglist ,1,ierflg);
  //  gMinuit->mnexcm("SET PRINT", arglist ,1,ierflg);
  gMinuit->mnexcm("MINOS", arglist ,1,ierflg);
  // Print results
  cout<<"start mnmatu"<<endl;
  gMinuit->mnmatu(1);
  cout<<"start mnstat"<<endl;
  Double_t amin,edm,errdef;
  Int_t nvpar,nparx,icstat;
  gMinuit->mnstat(amin,edm,errdef,nvpar,nparx,icstat);
  gMinuit->mnprin(4,amin);

  // ----------------------------------------------------------------------
  char line[200];
  Double_t vline[25];
  Double_t eline[25];
  const char *cnambf; 

  // ----------------------------------------------------------------------
  cout << "Fitted parameters: " << endl;
  cout << "       NO.     NAME       VALUE       ERROR " << endl;
  for (int n = 0; n < gMinuit->fNu; n++) {
    cnambf = gMinuit->fCpnam[n];
    double x1 = gMinuit->fWerr[n];
    sprintf(line, "     %4d %9s %12.5e %12.5e",n+1,(const char*)cnambf,gMinuit->fU[n],x1);
    cout << line << endl;
  }


  // ----------------------------------------------------------------------
  cout << "External Error Matrix: " << endl;
  double dxdi, dxdj; 
  for (int i = 1; i <= gMinuit->fNu; ++i) {
    int ix  = gMinuit->fNexofi[i-1];
    int   ndi = i*(i + 1) / 2;
    gMinuit->mndxdi(gMinuit->fX[i-1], i-1, dxdi);
    for (Int_t j = 1; j <= gMinuit->fNu; ++j) {
      gMinuit->mndxdi(gMinuit->fX[j-1], j-1, dxdj);
      int m    = TMath::Max(i,j);
      int n    = TMath::Min(i,j);
      int  ndex = m*(m-1) / 2 + n;
      int  ndj  = j*(j + 1) / 2;
      eline[j-1] = dxdi*gMinuit->fVhmat[ndex-1]*dxdj*gMinuit->fUp;
    }
    for (Int_t it = 1; it <= gMinuit->fNu; ++it) {
      sprintf(line, " %10.3e",eline[it-1]);
      cout << line; 
    }
    cout << endl;
  }
  
  // ----------------------------------------------------------------------
  cout << "Correlation Matrix: " << endl;
  //  cout << "       NO.  GLOBAL        ";
  //  for (Int_t id = 1; id <= gMinuit->fNu; ++id)  cout << gMinuit->fNexofi[id-1] << "       " ;
  //  cout << endl;

  const int nfixedparams = gMinuit->fNpfix;
  int ifix[nfixedparams];
  for (int i = 1; i <= nfixedparams; ++i) {
    ifix[i-1]=0; ifix[i-1] = gMinuit->fIpfix[i-1];  }

  for (int i = 1; i <= gMinuit->fNu; ++i) vline[i-1]=-100000;

  for (int i = 1; i <= gMinuit->fNu; ++i) {
    int itime1(0); for (int i2 = 0; i2 < nfixedparams; ++i2) {
      if(ifix[i2]==(i-1)){itime1=1; break;}}
    if(itime1==1) continue;
    int ix  = gMinuit->fNexofi[i-1];
    int   ndi = i*(i + 1) / 2;
    for (Int_t j = 1; j <= gMinuit->fNu; ++j) {
      int itime2(0); for (int j2 = 0; j2 < nfixedparams; ++j2) if(ifix[j2]==(j-1)){itime2=1; break;}
      if(itime2==1) continue;
      int m    = TMath::Max(i,j);
      int n    = TMath::Min(i,j);
      int  ndex = m*(m-1) / 2 + n;
      int  ndj  = j*(j + 1) / 2;
      vline[j-1] = gMinuit->fVhmat[ndex-1] / TMath::Sqrt(TMath::Abs(gMinuit->fVhmat[ndi-1]*gMinuit->fVhmat[ndj-1]));
    }
    //    sprintf(line, "      %3d   %7.4g ",ix,gMinuit->fGlobcc[i-1]);
    //    cout << line;
    for (Int_t it = 1; it <= gMinuit->fNu; ++it) {
      if(vline[it-1]>-100000){
	//	sprintf(line, " %7.2g",vline[it-1]); cout << line; }
	*outfilecorrelation<<vline[it-1]<<" ";}
    }
    //    cout << endl;
    *outfilecorrelation << endl;
  }

  // ----------------------------------------------------------------------

  Double_t weights[24], err_weights[24], errpos_weights[24], errneg_weights[24], errpar_weights[24], corr_weights[24];
  Double_t ma, err_ma, errpos_ma, errneg_ma, errpar_ma, corr_ma;
  Double_t ratio, err_ratio, errpos_ratio, errneg_ratio, errpar_ratio, corr_ratio;
  Double_t syst[10], err_syst[10], errpos_syst[10], errneg_syst[10], errpar_syst[10], corr_syst[10];

  gMinuit->GetParameter(0, weights[0], err_weights[0]); 
  gMinuit->GetParameter(1, weights[1], err_weights[1]); 
  gMinuit->GetParameter(2, weights[2], err_weights[2]); 
  gMinuit->GetParameter(3, weights[3], err_weights[3]); 
  gMinuit->GetParameter(4, weights[4], err_weights[4]); 
  gMinuit->GetParameter(5, weights[5], err_weights[5]); 
  gMinuit->GetParameter(6, weights[6], err_weights[6]); 
  gMinuit->GetParameter(7, weights[7], err_weights[7]); 
  gMinuit->GetParameter(8, weights[8], err_weights[8]); 
  gMinuit->GetParameter(9, weights[9], err_weights[9]); 
  gMinuit->GetParameter(10, weights[10], err_weights[10]); 
  gMinuit->GetParameter(11, weights[11], err_weights[11]); 
  gMinuit->GetParameter(12, weights[12], err_weights[12]); 
  gMinuit->GetParameter(13, weights[13], err_weights[13]); 
  gMinuit->GetParameter(14, weights[14], err_weights[14]); 
  gMinuit->GetParameter(15, weights[15], err_weights[15]); 
  gMinuit->GetParameter(16, weights[16], err_weights[16]); 
  gMinuit->GetParameter(17, weights[17], err_weights[17]); 
  gMinuit->GetParameter(18, weights[18], err_weights[18]); 
  gMinuit->GetParameter(19, weights[19], err_weights[19]); 
  gMinuit->GetParameter(20, weights[20], err_weights[20]); 
  gMinuit->GetParameter(21, weights[21], err_weights[21]); 
  gMinuit->GetParameter(22, weights[22], err_weights[22]); 
  gMinuit->GetParameter(23, weights[23], err_weights[23]); 
  gMinuit->GetParameter(24, ma, err_ma); 
  gMinuit->GetParameter(25, ratio, err_ratio); 
  gMinuit->GetParameter(26, syst[0], err_syst[0]); 
  gMinuit->GetParameter(27, syst[1], err_syst[1]); 
  gMinuit->GetParameter(28, syst[2], err_syst[2]); 
  gMinuit->GetParameter(29, syst[3], err_syst[3]); 
  gMinuit->GetParameter(30, syst[4], err_syst[4]); 
  gMinuit->GetParameter(31, syst[5], err_syst[5]); 
  gMinuit->GetParameter(32, syst[6], err_syst[6]); 
  gMinuit->GetParameter(33, syst[7], err_syst[7]); 
  gMinuit->GetParameter(34, syst[8], err_syst[8]); 
  gMinuit->GetParameter(35, syst[9], err_syst[9]); 

  gMinuit->mnerrs(0, errpos_weights[0], errneg_weights[0], errpar_weights[0], corr_weights[0]);
  gMinuit->mnerrs(1, errpos_weights[1], errneg_weights[1], errpar_weights[1], corr_weights[1]);
  gMinuit->mnerrs(2, errpos_weights[2], errneg_weights[2], errpar_weights[2], corr_weights[2]);
  gMinuit->mnerrs(3, errpos_weights[3], errneg_weights[3], errpar_weights[3], corr_weights[3]);
  gMinuit->mnerrs(4, errpos_weights[4], errneg_weights[4], errpar_weights[4], corr_weights[4]);
  gMinuit->mnerrs(5, errpos_weights[5], errneg_weights[5], errpar_weights[5], corr_weights[5]);
  gMinuit->mnerrs(6, errpos_weights[6], errneg_weights[6], errpar_weights[6], corr_weights[6]);
  gMinuit->mnerrs(7, errpos_weights[7], errneg_weights[7], errpar_weights[7], corr_weights[7]);
  gMinuit->mnerrs(8, errpos_weights[8], errneg_weights[8], errpar_weights[8], corr_weights[8]);
  gMinuit->mnerrs(9, errpos_weights[9], errneg_weights[9], errpar_weights[9], corr_weights[9]);
  gMinuit->mnerrs(10, errpos_weights[10], errneg_weights[10], errpar_weights[10], corr_weights[10]);
  gMinuit->mnerrs(11, errpos_weights[11], errneg_weights[11], errpar_weights[11], corr_weights[11]);
  gMinuit->mnerrs(12, errpos_weights[12], errneg_weights[12], errpar_weights[12], corr_weights[12]);
  gMinuit->mnerrs(13, errpos_weights[13], errneg_weights[13], errpar_weights[13], corr_weights[13]);
  gMinuit->mnerrs(14, errpos_weights[14], errneg_weights[14], errpar_weights[14], corr_weights[14]);
  gMinuit->mnerrs(15, errpos_weights[15], errneg_weights[15], errpar_weights[15], corr_weights[15]);
  gMinuit->mnerrs(16, errpos_weights[16], errneg_weights[16], errpar_weights[16], corr_weights[16]);
  gMinuit->mnerrs(17, errpos_weights[17], errneg_weights[17], errpar_weights[17], corr_weights[17]);
  gMinuit->mnerrs(18, errpos_weights[18], errneg_weights[18], errpar_weights[18], corr_weights[18]);
  gMinuit->mnerrs(19, errpos_weights[19], errneg_weights[19], errpar_weights[19], corr_weights[19]);
  gMinuit->mnerrs(20, errpos_weights[20], errneg_weights[20], errpar_weights[20], corr_weights[20]);
  gMinuit->mnerrs(21, errpos_weights[21], errneg_weights[21], errpar_weights[21], corr_weights[21]);
  gMinuit->mnerrs(22, errpos_weights[22], errneg_weights[22], errpar_weights[23], corr_weights[22]);
  gMinuit->mnerrs(23, errpos_weights[23], errneg_weights[23], errpar_weights[23], corr_weights[23]);
  gMinuit->mnerrs(24, errpos_ma, errneg_ma, errpar_ma, corr_ma);
  gMinuit->mnerrs(25, errpos_ratio, errneg_ratio, errpar_ratio, corr_ratio);
  gMinuit->mnerrs(26, errpos_syst[0], errneg_syst[0], errpar_syst[0], corr_syst[0]);
  gMinuit->mnerrs(27, errpos_syst[1], errneg_syst[1], errpar_syst[1], corr_syst[1]);
  gMinuit->mnerrs(28, errpos_syst[2], errneg_syst[2], errpar_syst[2], corr_syst[2]);
  gMinuit->mnerrs(29, errpos_syst[3], errneg_syst[3], errpar_syst[3], corr_syst[3]);
  gMinuit->mnerrs(30, errpos_syst[4], errneg_syst[4], errpar_syst[4], corr_syst[4]);
  gMinuit->mnerrs(31, errpos_syst[5], errneg_syst[5], errpar_syst[5], corr_syst[5]);
  gMinuit->mnerrs(32, errpos_syst[6], errneg_syst[6], errpar_syst[6], corr_syst[6]);
  gMinuit->mnerrs(33, errpos_syst[7], errneg_syst[7], errpar_syst[7], corr_syst[7]);
  gMinuit->mnerrs(34, errpos_syst[8], errneg_syst[8], errpar_syst[8], corr_syst[8]);
  gMinuit->mnerrs(35, errpos_syst[9], errneg_syst[9], errpar_syst[9], corr_syst[9]);
 
  
  std::cout << setprecision(3) ;
  std::cout << setw(14) << weights[0]<<" " << err_weights[0]<< std::endl;
  std::cout << setw(14) << weights[1]<<" " << err_weights[1]<< std::endl;
  std::cout << setw(14) << weights[2]<<" " << err_weights[2]<< std::endl;
  std::cout << setw(14) << weights[3]<<" " << err_weights[3]<< std::endl;
  std::cout << setw(14) << weights[4]<<" " << err_weights[4]<< std::endl;
  std::cout << setw(14) << weights[5]<<" " << err_weights[5]<< std::endl;
  std::cout << setw(14) << weights[6]<<" " << err_weights[6]<< std::endl;
  std::cout << setw(14) << weights[7]<<" " << err_weights[7]<< std::endl;
  std::cout << setw(14) << weights[8]<<" " << err_weights[8]<< std::endl;
  std::cout << setw(14) << weights[9]<<" " << err_weights[9]<< std::endl;
  std::cout << setw(14) << weights[10]<<" " << err_weights[10]<< std::endl;
  std::cout << setw(14) << weights[11]<<" " << err_weights[11]<< std::endl;
  std::cout << setw(14) << weights[12]<<" " << err_weights[12]<< std::endl;
  std::cout << setw(14) << weights[13]<<" " << err_weights[13]<< std::endl;
  std::cout << setw(14) << weights[14]<<" " << err_weights[14]<< std::endl;
  std::cout << setw(14) << weights[15]<<" " << err_weights[15]<< std::endl;
  std::cout << setw(14) << weights[16]<<" " << err_weights[16]<< std::endl;
  std::cout << setw(14) << weights[17]<<" " << err_weights[17]<< std::endl;
  std::cout << setw(14) << weights[18]<<" " << err_weights[18]<< std::endl;
  std::cout << setw(14) << weights[19]<<" " << err_weights[19]<< std::endl;
  std::cout << setw(14) << weights[20]<<" " << err_weights[20]<< std::endl;
  std::cout << setw(14) << weights[21]<<" " << err_weights[21]<< std::endl;
  std::cout << setw(14) << weights[22]<<" " << err_weights[22]<< std::endl;
  std::cout << setw(14) << weights[23]<<" " << err_weights[23]<< std::endl;
  std::cout << setw(14) << ma<<" " << err_ma<< std::endl;
  std::cout << setw(14) << ratio<<" " << err_ratio << std::endl ;
  std::cout << setw(14) << syst[0]<<" " << err_syst[0] << std::endl ;
  std::cout << setw(14) << syst[1]<<" " << err_syst[1] << std::endl ;
  std::cout << setw(14) << syst[2]<<" " << err_syst[2] << std::endl ;
  std::cout << setw(14) << syst[3]<<" " << err_syst[3] << std::endl ;
  std::cout << setw(14) << syst[4]<<" " << err_syst[4] << std::endl ;
  std::cout << setw(14) << syst[5]<<" " << err_syst[5] << std::endl ;
  std::cout << setw(14) << syst[6]<<" " << err_syst[6] << std::endl ;
  std::cout << setw(14) << syst[7]<<" " << err_syst[7] << std::endl ;
  std::cout << setw(14) << syst[8]<<" " << err_syst[8] << std::endl ;
  std::cout << setw(14) << syst[9]<<" " << err_syst[9] << std::endl ;

  
  //-- to comments as for fixed parameters
  *outfileresults<<setprecision(5)<<weights[0]<<" "<<errpos_weights[0]<<" "<<errneg_weights[0]<<endl;
  *outfileresults<<setprecision(5)<<weights[1]<<" "<<errpos_weights[1]<<" "<<errneg_weights[1]<<endl;
  *outfileresults<<setprecision(5)<<weights[2]<<" "<<errpos_weights[2]<<" "<<errneg_weights[2]<<endl;
  *outfileresults<<setprecision(5)<<weights[3]<<" "<<errpos_weights[3]<<" "<<errneg_weights[3]<<endl;
  *outfileresults<<setprecision(5)<<weights[4]<<" "<<errpos_weights[4]<<" "<<errneg_weights[4]<<endl;
  *outfileresults<<setprecision(5)<<weights[5]<<" "<<errpos_weights[5]<<" "<<errneg_weights[5]<<endl;
  *outfileresults<<setprecision(5)<<weights[6]<<" "<<errpos_weights[6]<<" "<<errneg_weights[6]<<endl;
  *outfileresults<<setprecision(5)<<weights[7]<<" "<<errpos_weights[7]<<" "<<errneg_weights[7]<<endl;
  *outfileresults<<setprecision(5)<<weights[8]<<" "<<errpos_weights[8]<<" "<<errneg_weights[8]<<endl;
  *outfileresults<<setprecision(5)<<weights[9]<<" "<<errpos_weights[9]<<" "<<errneg_weights[9]<<endl;
  *outfileresults<<setprecision(5)<<weights[10]<<" "<<errpos_weights[10]<<" "<<errneg_weights[10]<<endl;
  *outfileresults<<setprecision(5)<<weights[11]<<" "<<errpos_weights[11]<<" "<<errneg_weights[11]<<endl;
  *outfileresults<<setprecision(5)<<weights[12]<<" "<<errpos_weights[12]<<" "<<errneg_weights[12]<<endl;
  *outfileresults<<setprecision(5)<<weights[13]<<" "<<errpos_weights[13]<<" "<<errneg_weights[13]<<endl;
  *outfileresults<<setprecision(5)<<weights[14]<<" "<<errpos_weights[14]<<" "<<errneg_weights[14]<<endl;
  *outfileresults<<setprecision(5)<<weights[15]<<" "<<errpos_weights[15]<<" "<<errneg_weights[15]<<endl;
  *outfileresults<<setprecision(5)<<weights[16]<<" "<<errpos_weights[16]<<" "<<errneg_weights[16]<<endl;
  *outfileresults<<setprecision(5)<<weights[17]<<" "<<errpos_weights[17]<<" "<<errneg_weights[17]<<endl;
  *outfileresults<<setprecision(5)<<weights[18]<<" "<<errpos_weights[18]<<" "<<errneg_weights[18]<<endl;
  *outfileresults<<setprecision(5)<<weights[19]<<" "<<errpos_weights[19]<<" "<<errneg_weights[19]<<endl;
  *outfileresults<<setprecision(5)<<weights[20]<<" "<<errpos_weights[20]<<" "<<errneg_weights[20]<<endl;
  *outfileresults<<setprecision(5)<<weights[21]<<" "<<errpos_weights[21]<<" "<<errneg_weights[21]<<endl;
  *outfileresults<<setprecision(5)<<weights[22]<<" "<<errpos_weights[22]<<" "<<errneg_weights[22]<<endl;
  *outfileresults<<setprecision(5)<<weights[23]<<" "<<errpos_weights[23]<<" "<<errneg_weights[23]<<endl;
  *outfileresults<<setprecision(5)<<ma<<" "<<errpos_ma<<" "<<errneg_ma<<endl;
  *outfileresults<<setprecision(5)<<ratio<<" "<<errpos_ratio<<" "<<errneg_ratio<<endl;
  *outfileresults<<setprecision(5)<<syst[0]<<" "<<errpos_syst[0]<<" "<<errneg_syst[0]<<endl;
  //  *outfileresults<<setprecision(5)<<syst[1]<<" "<<errpos_syst[1]<<" "<<errneg_syst[1]<<endl;
  //  *outfileresults<<setprecision(5)<<syst[2]<<" "<<errpos_syst[2]<<" "<<errneg_syst[2]<<endl;
  //  *outfileresults<<setprecision(5)<<syst[3]<<" "<<errpos_syst[3]<<" "<<errneg_syst[3]<<endl;
  //  *outfileresults<<setprecision(5)<<syst[4]<<" "<<errpos_syst[4]<<" "<<errneg_syst[4]<<endl;
  //  *outfileresults<<setprecision(5)<<syst[5]<<" "<<errpos_syst[5]<<" "<<errneg_syst[5]<<endl;
  //  *outfileresults<<setprecision(5)<<syst[6]<<" "<<errpos_syst[6]<<" "<<errneg_syst[6]<<endl;
  //  *outfileresults<<setprecision(5)<<syst[7]<<" "<<errpos_syst[7]<<" "<<errneg_syst[7]<<endl;
  //  *outfileresults<<setprecision(5)<<syst[8]<<" "<<errpos_syst[8]<<" "<<errneg_syst[8]<<endl;
  //  *outfileresults<<setprecision(5)<<syst[9]<<" "<<errpos_syst[9]<<" "<<errneg_syst[9]<<endl;

  gMinuit->mnwerr();

  TSeqCollection* canvases = gROOT->GetListOfCanvases() ;

  //  setup();
  TCanvas* c1 = dynamic_cast<TCanvas*>(canvases->FindObject("c1")) ;
  if(!c1) c1 = new TCanvas("c1","",700,400) ;
  gStyle->SetOptStat(0) ;
  c1->Clear() ;
  
  //  c1->cd(1) ;
  thedata.drawEnuSpectrum() ;
  drawspectrum(thelookuptable,gMinuit,kRed) ;

  //  gMinuit->SetErrorDef(1);
  gPad->GetCanvas()->Update() ;
 
  c1->Print( eps) ;
  c1->Print( pdf) ;
  // 

  c1->Clear() ;
  thedata.drawEnuSpectrumFermi();
  c1->Print( "output/fermi.eps") ;
  c1->Print( "output/fermi.pdf") ;

  // 
  c1->Clear() ;
  drawMAspectra(thelookuptable) ;
  c1->Print( "output/MA.eps") ;
  c1->Print( "output/MA.pdf") ;
  //
    
  cout<<"Time at the end of job = "<<timer.CpuTime()<<" seconds"<<endl;
}


