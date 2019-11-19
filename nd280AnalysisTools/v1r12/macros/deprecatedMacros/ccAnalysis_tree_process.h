//******************************************************************* 
//
// File created with MakeClass() in ROOT which runs over the                                                                             // rootfile output of ccAnalysis.C
// The outfiles are ascii files for CCQE+CCnQE, CCQE and CCnQE events, seperately, that
// contains the number of events in each bin of the (Pmu, costhetamu) distribution
//
//******************************************************************

#ifndef ccAnalysis_tree_process_h
#define ccAnalysis_tree_process_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class ccAnalysis_tree_process {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           evid;
   Int_t           ntracks;
   Int_t           ntpctracks;
   Int_t           nfgdtracks;
   Int_t           evSel[5][10];
   Int_t           evTrue[5];
   Int_t           evG4Code;
   Float_t         RecVtx[3];
   Float_t         RecPmu;
   Float_t         RecEnu[5];
   Float_t         RecQ2[5];
   Float_t         RecCosThmu;
   Float_t         TrueVtx[3];
   Float_t         TruePmu;
   Float_t         TrueEnu;
   Float_t         TrueQ2;
   Float_t         TrueCosThmu;
   Int_t           trid[20];
   Int_t           trSel[20][7][7];
   Int_t           trTrue[20];
   Int_t           trG4Code[20];
   Float_t         RecMom[20];
   Float_t         TrueMom[20];
   Float_t         RecCosTh[20];
   Float_t         TrueCosTh[20];
   Int_t           RecCharge[20];
   Int_t           TrueCharge[20];
   Int_t           BackWards[20];
   Float_t         SigMom[20];
   Float_t         MomErr[20];
   Int_t           NNodes[20];
   Float_t         dEdxmeas[20];
   Float_t         Pull[20][5];
   Float_t         Sigma[20][5];
   Float_t         dEdxexp[20][5];
   Float_t         LikeLHD[20][5];
   Float_t         Prob[20][5];

   // List of branches
   TBranch        *b_evid;   //!
   TBranch        *b_ntracks;   //!
   TBranch        *b_ntpctracks;   //!
   TBranch        *b_nfgdtracks;   //!
   TBranch        *b_evSel;   //!
   TBranch        *b_evTrue;   //!
   TBranch        *b_evG4Code;   //!
   TBranch        *b_RecVtx;   //!
   TBranch        *b_RecPmu;   //!
   TBranch        *b_RecEnu;   //!
   TBranch        *b_RecQ2;   //!
   TBranch        *b_RecCosThmu;   //!
   TBranch        *b_TrueVtx;   //!
   TBranch        *b_TruePmu;   //!
   TBranch        *b_TrueEnu;   //!
   TBranch        *b_TrueQ2;   //!
   TBranch        *b_TrueCosThmu;   //!
   TBranch        *b_trid;   //!
   TBranch        *b_trSel;   //!
   TBranch        *b_trTrue;   //!
   TBranch        *b_trG4Code;   //!
   TBranch        *b_RecMom;   //!
   TBranch        *b_TrueMom;   //!
   TBranch        *b_RecCosTh;   //!
   TBranch        *b_TrueCosTh;   //!
   TBranch        *b_RecCharge;   //!
   TBranch        *b_TrueCharge;   //!
   TBranch        *b_BackWards;   //!
   TBranch        *b_SigMom;   //!
   TBranch        *b_MomErr;   //!
   TBranch        *b_NNodes;   //!
   TBranch        *b_dEdxmeas;   //!
   TBranch        *b_Pull;   //!
   TBranch        *b_Sigma;   //!
   TBranch        *b_dEdxexp;   //!
   TBranch        *b_LikeLHD;   //!
   TBranch        *b_Prob;   //!

   ccAnalysis_tree_process(TTree *tree=0);
   virtual ~ccAnalysis_tree_process();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(int which, TFile *out, ofstream *myfile, ofstream *myfileCCQE, ofstream *myfileCCnQE);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef ccAnalysis_tree_process_cxx
ccAnalysis_tree_process::ccAnalysis_tree_process(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("ccAnalysis_output_Curv3.00-Cut6-MA1.1.root");
      if (!f) {
         f = new TFile("ccAnalysis_output_Curv3.00-Cut6-MA1.1.root");
      }
      tree = (TTree*)gDirectory->Get("ccAnalysis_tree_process");

   }
   Init(tree);
}

ccAnalysis_tree_process::~ccAnalysis_tree_process()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t ccAnalysis_tree_process::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t ccAnalysis_tree_process::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void ccAnalysis_tree_process::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("evid", &evid, &b_evid);
   fChain->SetBranchAddress("ntracks", &ntracks, &b_ntracks);
   fChain->SetBranchAddress("ntpctracks", &ntpctracks, &b_ntpctracks);
   fChain->SetBranchAddress("nfgdtracks", &nfgdtracks, &b_nfgdtracks);
   fChain->SetBranchAddress("evSel", evSel, &b_evSel);
   fChain->SetBranchAddress("evTrue", evTrue, &b_evTrue);
   fChain->SetBranchAddress("evG4Code", &evG4Code, &b_evG4Code);
   fChain->SetBranchAddress("RecVtx", RecVtx, &b_RecVtx);
   fChain->SetBranchAddress("RecPmu", &RecPmu, &b_RecPmu);
   fChain->SetBranchAddress("RecEnu", RecEnu, &b_RecEnu);
   fChain->SetBranchAddress("RecQ2", RecQ2, &b_RecQ2);
   fChain->SetBranchAddress("RecCosThmu", &RecCosThmu, &b_RecCosThmu);
   fChain->SetBranchAddress("TrueVtx", TrueVtx, &b_TrueVtx);
   fChain->SetBranchAddress("TruePmu", &TruePmu, &b_TruePmu);
   fChain->SetBranchAddress("TrueEnu", &TrueEnu, &b_TrueEnu);
   fChain->SetBranchAddress("TrueQ2", &TrueQ2, &b_TrueQ2);
   fChain->SetBranchAddress("TrueCosThmu", &TrueCosThmu, &b_TrueCosThmu);
   fChain->SetBranchAddress("trid", trid, &b_trid);
   fChain->SetBranchAddress("trSel", trSel, &b_trSel);
   fChain->SetBranchAddress("trTrue", trTrue, &b_trTrue);
   fChain->SetBranchAddress("trG4Code", trG4Code, &b_trG4Code);
   fChain->SetBranchAddress("RecMom", RecMom, &b_RecMom);
   fChain->SetBranchAddress("TrueMom", TrueMom, &b_TrueMom);
   fChain->SetBranchAddress("RecCosTh", RecCosTh, &b_RecCosTh);
   fChain->SetBranchAddress("TrueCosTh", TrueCosTh, &b_TrueCosTh);
   fChain->SetBranchAddress("RecCharge", RecCharge, &b_RecCharge);
   fChain->SetBranchAddress("TrueCharge", TrueCharge, &b_TrueCharge);
   fChain->SetBranchAddress("BackWards", BackWards, &b_BackWards);
   fChain->SetBranchAddress("SigMom", SigMom, &b_SigMom);
   fChain->SetBranchAddress("MomErr", MomErr, &b_MomErr);
   fChain->SetBranchAddress("NNodes", NNodes, &b_NNodes);
   fChain->SetBranchAddress("dEdxmeas", dEdxmeas, &b_dEdxmeas);
   fChain->SetBranchAddress("Pull", Pull, &b_Pull);
   fChain->SetBranchAddress("Sigma", Sigma, &b_Sigma);
   fChain->SetBranchAddress("dEdxexp", dEdxexp, &b_dEdxexp);
   fChain->SetBranchAddress("LikeLHD", LikeLHD, &b_LikeLHD);
   fChain->SetBranchAddress("Prob", Prob, &b_Prob);
   Notify();
}

Bool_t ccAnalysis_tree_process::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void ccAnalysis_tree_process::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t ccAnalysis_tree_process::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef ccAnalysis_tree_process_cxx
