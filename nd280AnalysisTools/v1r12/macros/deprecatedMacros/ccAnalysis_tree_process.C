#define ccAnalysis_tree_process_cxx
#include "ccAnalysis_tree_process.h"
#include <TH2.h>
#include <TF1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <AnalysisTools/TCCweightMA.hxx>
using namespace std;

#ifdef __SC__
long G__globalvarpointer; // To make the Symantec linker happy
#endif


//--
double eneu(double Pmu, double Thetamu) {
//*******************************************************************
//
// Energy computation for CCQE events, using Oxygen binding energy
//
//*******************************************************************                                                                                                       
  // Put here the formula needed for CC-QE 
  const double Mn(0.93956536); //(MeV) neutron mass
  const double Mp(0.93827203); //(MeV) proton mass
  const double Mmu(0.10566);//(MeV) muon mass
  double Emu=pow(pow(Mmu,2)+pow(Pmu,2),0.5);
  double eps=0.027;//binding energy for Oxygen (27 MeV)
  return (pow(Mp,2)/2+(Mn+eps)*Emu-pow(Mn+eps,2)/2)/(Mn+eps-Emu+Pmu*Thetamu);
}

double q2lept(double Enu, double Pmu, double Thetamu) {
//*******************************************************************
// q2 computation
//
//*******************************************************************                                                                                                       
  // Put here the formula needed for CC-QE 
  const double Mmu(0.10566);//(MeV) muon mass
  double Emu=pow(pow(Mmu,2)+pow(Pmu,2),0.5);
  return -pow(Mmu,2)+2*Enu*(Emu-Pmu*Thetamu);
}

void ccAnalysis_tree_process::Loop(int which, TFile *out, ofstream *myfile, ofstream *myfileCCQE, ofstream *myfileCCnQE) {
//*******************************************************************
//
// File created with MakeClass() in ROOT which runs over the
// rootfile output of ccAnalysis.C
// The outfiles are ascii files for CCQE+CCnQE, CCQE and CCnQE events, seperately, that
// contain the number of events in each bin of the (Pmu, costhetamu) distribution 
// The currrent format of the output depends on the current needs of the fit
// and will be changed accordingly in the future.
//
// Francesca Di Lodovico (October 2009)
//
//*******************************************************************                                                                                                       

   if (fChain == 0) return;

   // -- Open Output File 
   out->cd();
   // -- histo  
   const int nsizeX = 40 ;
   const int nsizeY = 100 ;
   TH2D *PmuCostThmuCC = new TH2D("PmuCostThmuCC","",nsizeX, -1., 1., nsizeY, 0., 2500.);
   TH2D *PmuCostThmuCCQE = new TH2D("PmuCostThmuCCQE","",nsizeX, -1., 1., nsizeY, 0., 2500.);
   TH2D *PmuCostThmuCCnQE = new TH2D("PmuCostThmuCCnQE","",nsizeX, -1., 1., nsizeY, 0., 2500.);

   const int nbinsEnu=24;
   const Float_t myenu[] = {0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50, 0.55, 0.60, 0.65, 0.70, 0.75, 0.80, 0.85, 0.90, 0.95, 1.00, 1.10, 1.20, 1.30, 1.40, 1.50, 2.00}; 
   TH1D h1("enudata","E_{#nu} spectrum",nbinsEnu,myenu) ;
   h1.SetXTitle("E_{#nu} (GeV)");
   h1.SetYTitle("Events/arbitrary units");  
   TH1D h2("enudata","E_{#nu} spectrum",nbinsEnu,myenu) ;
   h2.SetXTitle("E_{#nu} (GeV)");
   h2.SetYTitle("Events/arbitrary units");  

   Double_t bineneuCCQE[nbinsEnu], bineneuCCnQE[nbinsEnu];
   for(int j=0; j<nbinsEnu; ++j){bineneuCCQE[j]=0;bineneuCCnQE[j]=0;}

   Long64_t nentries = fChain->GetEntriesFast();

   double manom(1.1);
   double mamin(0.10); double mastep(0.05);
   TCCweightMA myweight=TCCweightMA(manom);
   double ma = mamin+float(which)*mastep;

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      if(evSel[0][9]==0 && evTrue[0]==0){PmuCostThmuCC->Fill(RecCosThmu,RecPmu); }

      if(evSel[1][9]==1 && evTrue[1]==1){ 
	double enucal = eneu(RecPmu/1000., RecCosThmu);
	double q2 = q2lept(enucal, RecPmu/1000., RecCosThmu);
	int sel(0);
	double fact(1); fact = myweight.Weight(enucal, q2, ma, sel);if(fact == 0)fact =1; 	
	PmuCostThmuCCQE->Fill(RecCosThmu,RecPmu,fact); 
      }
      if(evSel[2][9]==2 && evTrue[2]==2){ PmuCostThmuCCnQE->Fill(RecCosThmu,RecPmu); 
      }
   }

   cout<<"starting filling the ascii files -- set up "<<endl;
   Int_t indexbin(0); const int nsize = nsizeX*nsizeY;
   Double_t dataContentCC[nsize];for(int i=0; i<nsize; ++i)dataContentCC[i]=0.;
   Double_t dataContentCCQE[nsize];for(int i=0; i<nsize; ++i)dataContentCCQE[i]=0.;
   Double_t dataContentCCnQE[nsize];for(int i=0; i<nsize; ++i)dataContentCCnQE[i]=0.;
   
   cout<<"Retrieving histo contents "<<endl;
   for(Int_t i=1; i<(nsizeX+1); i++){
     for(Int_t j=1; j<(nsizeY+1); j++){
       Double_t binContentCC = PmuCostThmuCC->GetBinContent(i,j); dataContentCC[indexbin] += binContentCC;
       Double_t binContentCCQE = PmuCostThmuCCQE->GetBinContent(i,j); dataContentCCQE[indexbin] += binContentCCQE; 
       Double_t binContentCCnQE = PmuCostThmuCCnQE->GetBinContent(i,j);  dataContentCCnQE[indexbin] += binContentCCnQE;
       indexbin++;
     }
   } 


   cout<<"starting filling the ascii files "<<endl;
   indexbin=0;
   for(Int_t i=1; i<(nsizeX+1); i++){
     for(Int_t j=1; j<(nsizeY+1); j++){
       double pmu = j*25.;
       double thetamu = -1.0 + i*0.05;
       *myfile<<pmu<<" "<<thetamu<<" "<<(dataContentCCQE[indexbin]+dataContentCCnQE[indexbin])<<endl;
       *myfileCCQE<<pmu<<" "<<thetamu<<" "<<dataContentCCQE[indexbin]<<endl;
       *myfileCCnQE<<pmu<<" "<<thetamu<<" "<<dataContentCCnQE[indexbin]<<endl;

       Double_t enucal = eneu(pmu/1000., thetamu);
       for(int k=0; k<nbinsEnu; ++k){
	 if(enucal <= myenu[k+1] && enucal> myenu[k] ) {
	   bineneuCCQE[k] += dataContentCCQE[indexbin];
	   bineneuCCnQE[k] += dataContentCCnQE[indexbin];
	 }
       }
       indexbin++;
     }
   } 
   for(int i=0; i<nbinsEnu; ++i) {
     if(i>(nbinsEnu-7)&&i<(nbinsEnu-1)){
       bineneuCCQE[i] /=2.;  bineneuCCnQE[i] /=2.; 
     }else if(i==(nbinsEnu-1)){
       bineneuCCQE[i] /=10.;  bineneuCCnQE[i] /=10.;
     }      
     h1.SetBinContent(i+1,bineneuCCQE[i]) ;
     h2.SetBinContent(i+1,bineneuCCnQE[i]) ;
   }

   TSeqCollection* canvases = gROOT->GetListOfCanvases() ;
   TCanvas* c1 = dynamic_cast<TCanvas*>(canvases->FindObject("c1")) ;
   c1 = new TCanvas("c1","gerrors2",140,10,700,500);

   h1.Draw();
   c1->Print("output/EnuCCQE.eps");
   h2.Draw("same");
   c1->Print("output/EnuAll.eps");
   h2.Draw();
   c1->Print("output/EnuCCnQE.eps");

   PmuCostThmuCC->SetXTitle("cos#theta#mu");   PmuCostThmuCC->SetYTitle("P#mu (GeV)");
   PmuCostThmuCC->Draw("box");
   c1->Print("output/PmuCostThmuCC.eps");
   
   PmuCostThmuCCQE->SetXTitle("cos#theta#mu");   PmuCostThmuCCQE->SetYTitle("P#mu (GeV)");
   PmuCostThmuCCQE->Draw("box");
   c1->Print("output/PmuCostThmuCCQE.eps");

   PmuCostThmuCCnQE->SetXTitle("cos#theta#mu");   PmuCostThmuCCnQE->SetYTitle("P#mu (GeV)");
   PmuCostThmuCCnQE->Draw("box");
   c1->Print("output/PmuCostThmuCCnQE.eps");
   
}
