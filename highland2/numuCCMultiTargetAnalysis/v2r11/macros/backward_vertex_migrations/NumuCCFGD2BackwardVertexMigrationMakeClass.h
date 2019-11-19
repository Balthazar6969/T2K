//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Nov 25 10:25:32 2015 by ROOT version 5.34/09
// from TTree default/7
// found on file: /data/t2k/short/t2k/highland2output/highland2output_neut_water_magnet_run2_v1r11highland2_ALL.root
//////////////////////////////////////////////////////////

#ifndef NumuCCFGD2BackwardVertexMigrationMakeClass_h
#define NumuCCFGD2BackwardVertexMigrationMakeClass_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class NumuCCFGD2BackwardVertexMigrationMakeClass {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           NTOYS;
   Int_t           toy_index[1];   //[NTOYS]
   Float_t         toy_weight[1];   //[NTOYS]
   Int_t           entry;
   Int_t           toy_ref;
   Int_t           detector;
   Int_t           fgd2detector;
   Int_t           fgd2gparent;
   Int_t           fgd2mectopology;
   Int_t           fgd2moduletype;
   Int_t           fgd2moduletypeCC;
   Int_t           fgd2nutype;
   Int_t           fgd2parent;
   Int_t           fgd2particle;
   Int_t           fgd2primary;
   Int_t           fgd2reaction;
   Int_t           fgd2reactionCC;
   Int_t           fgd2reactionnofv;
   Int_t           fgd2reactionsand;
   Int_t           fgd2reactionsandCC;
   Int_t           fgd2target;
   Int_t           fgd2targetCC;
   Int_t           fgd2targetCCQE;
   Int_t           fgd2topology;
   Int_t           fgd2topology_ccpizero;
   Int_t           fgd2topology_no1pi;
   Int_t           fgd2topology_withpi0;
   Int_t           gparent;
   Int_t           mectopology;
   Int_t           nutype;
   Int_t           parent;
   Int_t           particle;
   Int_t           primary;
   Int_t           reaction;
   Int_t           reactionCC;
   Int_t           reactionnofv;
   Int_t           reactionsand;
   Int_t           reactionsandCC;
   Int_t           target;
   Int_t           topology;
   Int_t           topology_ccpizero;
   Int_t           topology_no1pi;
   Int_t           topology_withpi0;
   Int_t           redo[1];   //[NTOYS]
   Int_t           accum_level[1][4];   //[NTOYS]
   Int_t           cut0[1][4];   //[NTOYS]
   Int_t           cut1[1][4];   //[NTOYS]
   Int_t           cut2[1][4];   //[NTOYS]
   Int_t           cut3[1][4];   //[NTOYS]
   Int_t           cut4[1][4];   //[NTOYS]
   Int_t           cut5[1][4];   //[NTOYS]
   Int_t           cut6[1][4];   //[NTOYS]
   Int_t           run;
   Int_t           subrun;
   Int_t           evt;
   Int_t           bunch;
   Int_t           NWEIGHTS;
   Float_t         weight[2];   //[NWEIGHTS]
   Int_t           TruthVertexID[1];   //[NTOYS]
   Int_t           RooVtxIndex[1];   //[NTOYS]
   Int_t           RooVtxEntry[1];   //[NTOYS]
   Int_t           nu_pdg;
   Float_t         nu_trueE;
   Int_t           nu_truereac;
   Float_t         nu_truedir[3];
   Int_t           selvtx_det;
   Float_t         selvtx_pos[4];
   Float_t         selvtx_truepos[4];
   Int_t           ntpctracks;
   Int_t           ntpcposQualityFV;
   Int_t           ntpcnegQualityFV;
   Int_t           nfgdtracks;
   Int_t           nfgdonlytracks;
   Float_t         selmu_truemom;
   Float_t         selmu_truepos[4];
   Float_t         selmu_trueendpos[4];
   Float_t         selmu_truedir[3];
   Float_t         truemu_mom;
   Float_t         truemu_costheta;
   Int_t           truemu_fgd;
   Float_t         selmu_dir[3];
   Float_t         selmu_pos[4];
   Float_t         selmu_endpos[4];
   Float_t         selmu_costheta;
   Int_t           selmu_closest_tpc;
   Int_t           selmu_detectors;
   Int_t           selmu_fgd;
   Float_t         selmu_mom[1];   //[NTOYS]
   Float_t         selmu_theta[1];   //[NTOYS]
   Float_t         selmu_amom;
   Float_t         selmu_likemu;
   Float_t         selmu_likemip;
   Float_t         shmn_mom;
   Int_t           selmu_ntpcs;
   Int_t           selmu_tpc_det[2];   //[selmu_ntpcs]
   Int_t           selmu_tpc_nhits[2];   //[selmu_ntpcs]
   Int_t           selmu_tpc_nnodes[2];   //[selmu_ntpcs]
   Float_t         selmu_tpc_charge[2];   //[selmu_ntpcs]
   Float_t         selmu_tpc_mom[2];   //[selmu_ntpcs]
   Float_t         selmu_tpc_bfield_mom[2];   //[selmu_ntpcs]
   Float_t         selmu_tpc_efield_mom[2];   //[selmu_ntpcs]
   Float_t         selmu_tpc_emom[2];   //[selmu_ntpcs]
   Float_t         selmu_tpc_truemom[2];   //[selmu_ntpcs]
   Float_t         selmu_tpc_dedx_raw[2];   //[selmu_ntpcs]
   Float_t         selmu_tpc_dedx_expmu[2];   //[selmu_ntpcs]
   Float_t         selmu_tpc_dedx_exppi[2];   //[selmu_ntpcs]
   Float_t         selmu_tpc_dedx_expele[2];   //[selmu_ntpcs]
   Float_t         selmu_tpc_dedx_expp[2];   //[selmu_ntpcs]
   Float_t         selmu_tpc_pullmu[2];   //[selmu_ntpcs]
   Float_t         selmu_tpc_pullpi[2];   //[selmu_ntpcs]
   Float_t         selmu_tpc_pullele[2];   //[selmu_ntpcs]
   Float_t         selmu_tpc_pullp[2];   //[selmu_ntpcs]
   Float_t         selmu_tpc_dedx[2];   //[selmu_ntpcs]
   Int_t           selmu_nfgds;
   Int_t           selmu_fgd_det[2];   //[selmu_nfgds]
   Float_t         selmu_fgd_x[2];   //[selmu_nfgds]
   Float_t         selmu_fgd_V11[2];   //[selmu_nfgds]
   Float_t         selmu_fgd_V33[2];   //[selmu_nfgds]
   Float_t         selmu_fgd_V55[2];   //[selmu_nfgds]
   Float_t         selmu_fgd_V77[2];   //[selmu_nfgds]
   Float_t         selmu_fgd_VLayer[2];   //[selmu_nfgds]
   Float_t         selmu_fgd_pullmu[2];   //[selmu_nfgds]
   Int_t           selmu_necals;
   Int_t           selmu_ecal_det[2];   //[selmu_necals]
   Int_t           selmu_ecal_nhits[2];   //[selmu_necals]
   Int_t           selmu_ecal_nnodes[2];   //[selmu_necals]
   Float_t         selmu_ecal_length[2];   //[selmu_necals]
   Float_t         selmu_ecal_showerstartpos[2][4];   //[selmu_necals]
   Float_t         selmu_ecal_showerendpos[2][4];   //[selmu_necals]
   Float_t         selmu_ecal_showerstartdir[2][3];   //[selmu_necals]
   Float_t         selmu_ecal_showerenddir[2][3];   //[selmu_necals]
   Float_t         selmu_ecal_EMenergy[2];   //[selmu_necals]
   Float_t         selmu_ecal_edeposit[2];   //[selmu_necals]
   Int_t           selmu_ecal_IsShower[2];   //[selmu_necals]
   Float_t         selmu_ecal_mipem[2];   //[selmu_necals]
   Float_t         selmu_ecal_mippion[2];   //[selmu_necals]
   Float_t         selmu_ecal_emhip[2];   //[selmu_necals]
   Float_t         selmu_ecal_containment[2];   //[selmu_necals]
   Float_t         selmu_ecal_trshval[2];   //[selmu_necals]
   Int_t           selmu_ecal_mostupstreamlayerhit[2];   //[selmu_necals]
   Int_t           ntracks;
   Int_t           selmu_fgdmoduletype;
   Int_t           selmu_fgdlayer;
   Float_t         selmu_hit1_pos[3];
   Float_t         selmu_hit2_pos[3];
   Float_t         selmu_hit1_charge;
   Float_t         selmu_hit2_charge;
   Float_t         selmu_recEccqe;
   Int_t           truemu_fgdmoduletype;
   Int_t           truemu_fgdlayer;
   Int_t           truemu_targetZ;
   Int_t           truemu_targetPDG;
   Float_t         distance_track_hit1;
   Float_t         distance_track_hit2;
   Float_t         selmu_fittrack_hit1_pos[3];
   Float_t         selmu_fittrack_hit2_pos[3];
   Float_t         selmu_deltachi2_hit1;
   Float_t         selmu_deltachi2_hit2;

   // List of branches
   TBranch        *b_NTOYS;   //!
   TBranch        *b_toy_index;   //!
   TBranch        *b_toy_weight;   //!
   TBranch        *b_entry;   //!
   TBranch        *b_toy_ref;   //!
   TBranch        *b_detector;   //!
   TBranch        *b_fgd2detector;   //!
   TBranch        *b_fgd2gparent;   //!
   TBranch        *b_fgd2mectopology;   //!
   TBranch        *b_fgd2moduletype;   //!
   TBranch        *b_fgd2moduletypeCC;   //!
   TBranch        *b_fgd2nutype;   //!
   TBranch        *b_fgd2parent;   //!
   TBranch        *b_fgd2particle;   //!
   TBranch        *b_fgd2primary;   //!
   TBranch        *b_fgd2reaction;   //!
   TBranch        *b_fgd2reactionCC;   //!
   TBranch        *b_fgd2reactionnofv;   //!
   TBranch        *b_fgd2reactionsand;   //!
   TBranch        *b_fgd2reactionsandCC;   //!
   TBranch        *b_fgd2target;   //!
   TBranch        *b_fgd2targetCC;   //!
   TBranch        *b_fgd2targetCCQE;   //!
   TBranch        *b_fgd2topology;   //!
   TBranch        *b_fgd2topology_ccpizero;   //!
   TBranch        *b_fgd2topology_no1pi;   //!
   TBranch        *b_fgd2topology_withpi0;   //!
   TBranch        *b_gparent;   //!
   TBranch        *b_mectopology;   //!
   TBranch        *b_nutype;   //!
   TBranch        *b_parent;   //!
   TBranch        *b_particle;   //!
   TBranch        *b_primary;   //!
   TBranch        *b_reaction;   //!
   TBranch        *b_reactionCC;   //!
   TBranch        *b_reactionnofv;   //!
   TBranch        *b_reactionsand;   //!
   TBranch        *b_reactionsandCC;   //!
   TBranch        *b_target;   //!
   TBranch        *b_topology;   //!
   TBranch        *b_topology_ccpizero;   //!
   TBranch        *b_topology_no1pi;   //!
   TBranch        *b_topology_withpi0;   //!
   TBranch        *b_redo;   //!
   TBranch        *b_accum_level;   //!
   TBranch        *b_cut0;   //!
   TBranch        *b_cut1;   //!
   TBranch        *b_cut2;   //!
   TBranch        *b_cut3;   //!
   TBranch        *b_cut4;   //!
   TBranch        *b_cut5;   //!
   TBranch        *b_cut6;   //!
   TBranch        *b_run;   //!
   TBranch        *b_subrun;   //!
   TBranch        *b_evt;   //!
   TBranch        *b_bunch;   //!
   TBranch        *b_NWEIGHTS;   //!
   TBranch        *b_weight;   //!
   TBranch        *b_TruthVertexID;   //!
   TBranch        *b_RooVtxIndex;   //!
   TBranch        *b_RooVtxEntry;   //!
   TBranch        *b_nu_pdg;   //!
   TBranch        *b_nu_trueE;   //!
   TBranch        *b_nu_truereac;   //!
   TBranch        *b_nu_truedir;   //!
   TBranch        *b_selvtx_det;   //!
   TBranch        *b_selvtx_pos;   //!
   TBranch        *b_selvtx_truepos;   //!
   TBranch        *b_ntpctracks;   //!
   TBranch        *b_ntpcposQualityFV;   //!
   TBranch        *b_ntpcnegQualityFV;   //!
   TBranch        *b_nfgdtracks;   //!
   TBranch        *b_nfgdonlytracks;   //!
   TBranch        *b_selmu_truemom;   //!
   TBranch        *b_selmu_truepos;   //!
   TBranch        *b_selmu_trueendpos;   //!
   TBranch        *b_selmu_truedir;   //!
   TBranch        *b_truemu_mom;   //!
   TBranch        *b_truemu_costheta;   //!
   TBranch        *b_truemu_fgd;   //!
   TBranch        *b_selmu_dir;   //!
   TBranch        *b_selmu_pos;   //!
   TBranch        *b_selmu_endpos;   //!
   TBranch        *b_selmu_costheta;   //!
   TBranch        *b_selmu_closest_tpc;   //!
   TBranch        *b_selmu_detectors;   //!
   TBranch        *b_selmu_fgd;   //!
   TBranch        *b_selmu_mom;   //!
   TBranch        *b_selmu_theta;   //!
   TBranch        *b_selmu_amom;   //!
   TBranch        *b_selmu_likemu;   //!
   TBranch        *b_selmu_likemip;   //!
   TBranch        *b_shmn_mom;   //!
   TBranch        *b_selmu_ntpcs;   //!
   TBranch        *b_selmu_tpc_det;   //!
   TBranch        *b_selmu_tpc_nhits;   //!
   TBranch        *b_selmu_tpc_nnodes;   //!
   TBranch        *b_selmu_tpc_charge;   //!
   TBranch        *b_selmu_tpc_mom;   //!
   TBranch        *b_selmu_tpc_bfield_mom;   //!
   TBranch        *b_selmu_tpc_efield_mom;   //!
   TBranch        *b_selmu_tpc_emom;   //!
   TBranch        *b_selmu_tpc_truemom;   //!
   TBranch        *b_selmu_tpc_dedx_raw;   //!
   TBranch        *b_selmu_tpc_dedx_expmu;   //!
   TBranch        *b_selmu_tpc_dedx_exppi;   //!
   TBranch        *b_selmu_tpc_dedx_expele;   //!
   TBranch        *b_selmu_tpc_dedx_expp;   //!
   TBranch        *b_selmu_tpc_pullmu;   //!
   TBranch        *b_selmu_tpc_pullpi;   //!
   TBranch        *b_selmu_tpc_pullele;   //!
   TBranch        *b_selmu_tpc_pullp;   //!
   TBranch        *b_selmu_tpc_dedx;   //!
   TBranch        *b_selmu_nfgds;   //!
   TBranch        *b_selmu_fgd_det;   //!
   TBranch        *b_selmu_fgd_x;   //!
   TBranch        *b_selmu_fgd_V11;   //!
   TBranch        *b_selmu_fgd_V33;   //!
   TBranch        *b_selmu_fgd_V55;   //!
   TBranch        *b_selmu_fgd_V77;   //!
   TBranch        *b_selmu_fgd_VLayer;   //!
   TBranch        *b_selmu_fgd_pullmu;   //!
   TBranch        *b_selmu_necals;   //!
   TBranch        *b_selmu_ecal_det;   //!
   TBranch        *b_selmu_ecal_nhits;   //!
   TBranch        *b_selmu_ecal_nnodes;   //!
   TBranch        *b_selmu_ecal_length;   //!
   TBranch        *b_selmu_ecal_showerstartpos;   //!
   TBranch        *b_selmu_ecal_showerendpos;   //!
   TBranch        *b_selmu_ecal_showerstartdir;   //!
   TBranch        *b_selmu_ecal_showerenddir;   //!
   TBranch        *b_selmu_ecal_EMenergy;   //!
   TBranch        *b_selmu_ecal_edeposit;   //!
   TBranch        *b_selmu_ecal_IsShower;   //!
   TBranch        *b_selmu_ecal_mipem;   //!
   TBranch        *b_selmu_ecal_mippion;   //!
   TBranch        *b_selmu_ecal_emhip;   //!
   TBranch        *b_selmu_ecal_containment;   //!
   TBranch        *b_selmu_ecal_trshval;   //!
   TBranch        *b_selmu_ecal_mostupstreamlayerhit;   //!
   TBranch        *b_ntracks;   //!
   TBranch        *b_selmu_fgdmoduletype;   //!
   TBranch        *b_selmu_fgdlayer;   //!
   TBranch        *b_selmu_hit1_pos;   //!
   TBranch        *b_selmu_hit2_pos;   //!
   TBranch        *b_selmu_hit1_charge;   //!
   TBranch        *b_selmu_hit2_charge;   //!
   TBranch        *b_selmu_recEccqe;   //!
   TBranch        *b_truemu_fgdmoduletype;   //!
   TBranch        *b_truemu_fgdlayer;   //!
   TBranch        *b_truemu_targetZ;   //!
   TBranch        *b_truemu_targetPDG;   //!
   TBranch        *b_distance_track_hit1;   //!
   TBranch        *b_distance_track_hit2;   //!
   TBranch        *b_selmu_fittrack_hit1_pos;   //!
   TBranch        *b_selmu_fittrack_hit2_pos;   //!
   TBranch        *b_selmu_deltachi2_hit1;   //!
   TBranch        *b_selmu_deltachi2_hit2;   //!

   NumuCCFGD2BackwardVertexMigrationMakeClass(TTree *tree=0);
   virtual ~NumuCCFGD2BackwardVertexMigrationMakeClass();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef NumuCCFGD2BackwardVertexMigrationMakeClass_cxx
NumuCCFGD2BackwardVertexMigrationMakeClass::NumuCCFGD2BackwardVertexMigrationMakeClass(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/data/t2k/short/t2k/highland2output/highland2output_neut_water_magnet_run2_v1r11highland2_ALL.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/data/t2k/short/t2k/highland2output/highland2output_neut_water_magnet_run2_v1r11highland2_ALL.root");
      }
      f->GetObject("default",tree);

   }
   Init(tree);
}

NumuCCFGD2BackwardVertexMigrationMakeClass::~NumuCCFGD2BackwardVertexMigrationMakeClass()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t NumuCCFGD2BackwardVertexMigrationMakeClass::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t NumuCCFGD2BackwardVertexMigrationMakeClass::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void NumuCCFGD2BackwardVertexMigrationMakeClass::Init(TTree *tree)
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

   fChain->SetBranchAddress("NTOYS", &NTOYS, &b_NTOYS);
   fChain->SetBranchAddress("toy_index", toy_index, &b_toy_index);
   fChain->SetBranchAddress("toy_weight", toy_weight, &b_toy_weight);
   fChain->SetBranchAddress("entry", &entry, &b_entry);
   fChain->SetBranchAddress("toy_ref", &toy_ref, &b_toy_ref);
   fChain->SetBranchAddress("detector", &detector, &b_detector);
   fChain->SetBranchAddress("fgd2detector", &fgd2detector, &b_fgd2detector);
   fChain->SetBranchAddress("fgd2gparent", &fgd2gparent, &b_fgd2gparent);
   fChain->SetBranchAddress("fgd2mectopology", &fgd2mectopology, &b_fgd2mectopology);
   fChain->SetBranchAddress("fgd2moduletype", &fgd2moduletype, &b_fgd2moduletype);
   fChain->SetBranchAddress("fgd2moduletypeCC", &fgd2moduletypeCC, &b_fgd2moduletypeCC);
   fChain->SetBranchAddress("fgd2nutype", &fgd2nutype, &b_fgd2nutype);
   fChain->SetBranchAddress("fgd2parent", &fgd2parent, &b_fgd2parent);
   fChain->SetBranchAddress("fgd2particle", &fgd2particle, &b_fgd2particle);
   fChain->SetBranchAddress("fgd2primary", &fgd2primary, &b_fgd2primary);
   fChain->SetBranchAddress("fgd2reaction", &fgd2reaction, &b_fgd2reaction);
   fChain->SetBranchAddress("fgd2reactionCC", &fgd2reactionCC, &b_fgd2reactionCC);
   fChain->SetBranchAddress("fgd2reactionnofv", &fgd2reactionnofv, &b_fgd2reactionnofv);
   fChain->SetBranchAddress("fgd2reactionsand", &fgd2reactionsand, &b_fgd2reactionsand);
   fChain->SetBranchAddress("fgd2reactionsandCC", &fgd2reactionsandCC, &b_fgd2reactionsandCC);
   fChain->SetBranchAddress("fgd2target", &fgd2target, &b_fgd2target);
   fChain->SetBranchAddress("fgd2targetCC", &fgd2targetCC, &b_fgd2targetCC);
   fChain->SetBranchAddress("fgd2targetCCQE", &fgd2targetCCQE, &b_fgd2targetCCQE);
   fChain->SetBranchAddress("fgd2topology", &fgd2topology, &b_fgd2topology);
   fChain->SetBranchAddress("fgd2topology_ccpizero", &fgd2topology_ccpizero, &b_fgd2topology_ccpizero);
   fChain->SetBranchAddress("fgd2topology_no1pi", &fgd2topology_no1pi, &b_fgd2topology_no1pi);
   fChain->SetBranchAddress("fgd2topology_withpi0", &fgd2topology_withpi0, &b_fgd2topology_withpi0);
   fChain->SetBranchAddress("gparent", &gparent, &b_gparent);
   fChain->SetBranchAddress("mectopology", &mectopology, &b_mectopology);
   fChain->SetBranchAddress("nutype", &nutype, &b_nutype);
   fChain->SetBranchAddress("parent", &parent, &b_parent);
   fChain->SetBranchAddress("particle", &particle, &b_particle);
   fChain->SetBranchAddress("primary", &primary, &b_primary);
   fChain->SetBranchAddress("reaction", &reaction, &b_reaction);
   fChain->SetBranchAddress("reactionCC", &reactionCC, &b_reactionCC);
   fChain->SetBranchAddress("reactionnofv", &reactionnofv, &b_reactionnofv);
   fChain->SetBranchAddress("reactionsand", &reactionsand, &b_reactionsand);
   fChain->SetBranchAddress("reactionsandCC", &reactionsandCC, &b_reactionsandCC);
   fChain->SetBranchAddress("target", &target, &b_target);
   fChain->SetBranchAddress("topology", &topology, &b_topology);
   fChain->SetBranchAddress("topology_ccpizero", &topology_ccpizero, &b_topology_ccpizero);
   fChain->SetBranchAddress("topology_no1pi", &topology_no1pi, &b_topology_no1pi);
   fChain->SetBranchAddress("topology_withpi0", &topology_withpi0, &b_topology_withpi0);
   fChain->SetBranchAddress("redo", redo, &b_redo);
   fChain->SetBranchAddress("accum_level", accum_level, &b_accum_level);
   fChain->SetBranchAddress("cut0", cut0, &b_cut0);
   fChain->SetBranchAddress("cut1", cut1, &b_cut1);
   fChain->SetBranchAddress("cut2", cut2, &b_cut2);
   fChain->SetBranchAddress("cut3", cut3, &b_cut3);
   fChain->SetBranchAddress("cut4", cut4, &b_cut4);
   fChain->SetBranchAddress("cut5", cut5, &b_cut5);
   fChain->SetBranchAddress("cut6", cut6, &b_cut6);
   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("subrun", &subrun, &b_subrun);
   fChain->SetBranchAddress("evt", &evt, &b_evt);
   fChain->SetBranchAddress("bunch", &bunch, &b_bunch);
   fChain->SetBranchAddress("NWEIGHTS", &NWEIGHTS, &b_NWEIGHTS);
   fChain->SetBranchAddress("weight", weight, &b_weight);
   fChain->SetBranchAddress("TruthVertexID", TruthVertexID, &b_TruthVertexID);
   fChain->SetBranchAddress("RooVtxIndex", RooVtxIndex, &b_RooVtxIndex);
   fChain->SetBranchAddress("RooVtxEntry", RooVtxEntry, &b_RooVtxEntry);
   fChain->SetBranchAddress("nu_pdg", &nu_pdg, &b_nu_pdg);
   fChain->SetBranchAddress("nu_trueE", &nu_trueE, &b_nu_trueE);
   fChain->SetBranchAddress("nu_truereac", &nu_truereac, &b_nu_truereac);
   fChain->SetBranchAddress("nu_truedir", nu_truedir, &b_nu_truedir);
   fChain->SetBranchAddress("selvtx_det", &selvtx_det, &b_selvtx_det);
   fChain->SetBranchAddress("selvtx_pos", selvtx_pos, &b_selvtx_pos);
   fChain->SetBranchAddress("selvtx_truepos", selvtx_truepos, &b_selvtx_truepos);
   fChain->SetBranchAddress("ntpctracks", &ntpctracks, &b_ntpctracks);
   fChain->SetBranchAddress("ntpcposQualityFV", &ntpcposQualityFV, &b_ntpcposQualityFV);
   fChain->SetBranchAddress("ntpcnegQualityFV", &ntpcnegQualityFV, &b_ntpcnegQualityFV);
   fChain->SetBranchAddress("nfgdtracks", &nfgdtracks, &b_nfgdtracks);
   fChain->SetBranchAddress("nfgdonlytracks", &nfgdonlytracks, &b_nfgdonlytracks);
   fChain->SetBranchAddress("selmu_truemom", &selmu_truemom, &b_selmu_truemom);
   fChain->SetBranchAddress("selmu_truepos", selmu_truepos, &b_selmu_truepos);
   fChain->SetBranchAddress("selmu_trueendpos", selmu_trueendpos, &b_selmu_trueendpos);
   fChain->SetBranchAddress("selmu_truedir", selmu_truedir, &b_selmu_truedir);
   fChain->SetBranchAddress("truemu_mom", &truemu_mom, &b_truemu_mom);
   fChain->SetBranchAddress("truemu_costheta", &truemu_costheta, &b_truemu_costheta);
   fChain->SetBranchAddress("truemu_fgd", &truemu_fgd, &b_truemu_fgd);
   fChain->SetBranchAddress("selmu_dir", selmu_dir, &b_selmu_dir);
   fChain->SetBranchAddress("selmu_pos", selmu_pos, &b_selmu_pos);
   fChain->SetBranchAddress("selmu_endpos", selmu_endpos, &b_selmu_endpos);
   fChain->SetBranchAddress("selmu_costheta", &selmu_costheta, &b_selmu_costheta);
   fChain->SetBranchAddress("selmu_closest_tpc", &selmu_closest_tpc, &b_selmu_closest_tpc);
   fChain->SetBranchAddress("selmu_detectors", &selmu_detectors, &b_selmu_detectors);
   fChain->SetBranchAddress("selmu_fgd", &selmu_fgd, &b_selmu_fgd);
   fChain->SetBranchAddress("selmu_mom", selmu_mom, &b_selmu_mom);
   fChain->SetBranchAddress("selmu_theta", selmu_theta, &b_selmu_theta);
   fChain->SetBranchAddress("selmu_amom", &selmu_amom, &b_selmu_amom);
   fChain->SetBranchAddress("selmu_likemu", &selmu_likemu, &b_selmu_likemu);
   fChain->SetBranchAddress("selmu_likemip", &selmu_likemip, &b_selmu_likemip);
   fChain->SetBranchAddress("shmn_mom", &shmn_mom, &b_shmn_mom);
   fChain->SetBranchAddress("selmu_ntpcs", &selmu_ntpcs, &b_selmu_ntpcs);
   fChain->SetBranchAddress("selmu_tpc_det", selmu_tpc_det, &b_selmu_tpc_det);
   fChain->SetBranchAddress("selmu_tpc_nhits", selmu_tpc_nhits, &b_selmu_tpc_nhits);
   fChain->SetBranchAddress("selmu_tpc_nnodes", selmu_tpc_nnodes, &b_selmu_tpc_nnodes);
   fChain->SetBranchAddress("selmu_tpc_charge", selmu_tpc_charge, &b_selmu_tpc_charge);
   fChain->SetBranchAddress("selmu_tpc_mom", selmu_tpc_mom, &b_selmu_tpc_mom);
   fChain->SetBranchAddress("selmu_tpc_bfield_mom", selmu_tpc_bfield_mom, &b_selmu_tpc_bfield_mom);
   fChain->SetBranchAddress("selmu_tpc_efield_mom", selmu_tpc_efield_mom, &b_selmu_tpc_efield_mom);
   fChain->SetBranchAddress("selmu_tpc_emom", selmu_tpc_emom, &b_selmu_tpc_emom);
   fChain->SetBranchAddress("selmu_tpc_truemom", selmu_tpc_truemom, &b_selmu_tpc_truemom);
   fChain->SetBranchAddress("selmu_tpc_dedx_raw", selmu_tpc_dedx_raw, &b_selmu_tpc_dedx_raw);
   fChain->SetBranchAddress("selmu_tpc_dedx_expmu", selmu_tpc_dedx_expmu, &b_selmu_tpc_dedx_expmu);
   fChain->SetBranchAddress("selmu_tpc_dedx_exppi", selmu_tpc_dedx_exppi, &b_selmu_tpc_dedx_exppi);
   fChain->SetBranchAddress("selmu_tpc_dedx_expele", selmu_tpc_dedx_expele, &b_selmu_tpc_dedx_expele);
   fChain->SetBranchAddress("selmu_tpc_dedx_expp", selmu_tpc_dedx_expp, &b_selmu_tpc_dedx_expp);
   fChain->SetBranchAddress("selmu_tpc_pullmu", selmu_tpc_pullmu, &b_selmu_tpc_pullmu);
   fChain->SetBranchAddress("selmu_tpc_pullpi", selmu_tpc_pullpi, &b_selmu_tpc_pullpi);
   fChain->SetBranchAddress("selmu_tpc_pullele", selmu_tpc_pullele, &b_selmu_tpc_pullele);
   fChain->SetBranchAddress("selmu_tpc_pullp", selmu_tpc_pullp, &b_selmu_tpc_pullp);
   fChain->SetBranchAddress("selmu_tpc_dedx", selmu_tpc_dedx, &b_selmu_tpc_dedx);
   fChain->SetBranchAddress("selmu_nfgds", &selmu_nfgds, &b_selmu_nfgds);
   fChain->SetBranchAddress("selmu_fgd_det", selmu_fgd_det, &b_selmu_fgd_det);
   fChain->SetBranchAddress("selmu_fgd_x", selmu_fgd_x, &b_selmu_fgd_x);
   fChain->SetBranchAddress("selmu_fgd_V11", selmu_fgd_V11, &b_selmu_fgd_V11);
   fChain->SetBranchAddress("selmu_fgd_V33", selmu_fgd_V33, &b_selmu_fgd_V33);
   fChain->SetBranchAddress("selmu_fgd_V55", selmu_fgd_V55, &b_selmu_fgd_V55);
   fChain->SetBranchAddress("selmu_fgd_V77", selmu_fgd_V77, &b_selmu_fgd_V77);
   fChain->SetBranchAddress("selmu_fgd_VLayer", selmu_fgd_VLayer, &b_selmu_fgd_VLayer);
   fChain->SetBranchAddress("selmu_fgd_pullmu", selmu_fgd_pullmu, &b_selmu_fgd_pullmu);
   fChain->SetBranchAddress("selmu_necals", &selmu_necals, &b_selmu_necals);
   fChain->SetBranchAddress("selmu_ecal_det", selmu_ecal_det, &b_selmu_ecal_det);
   fChain->SetBranchAddress("selmu_ecal_nhits", selmu_ecal_nhits, &b_selmu_ecal_nhits);
   fChain->SetBranchAddress("selmu_ecal_nnodes", selmu_ecal_nnodes, &b_selmu_ecal_nnodes);
   fChain->SetBranchAddress("selmu_ecal_length", selmu_ecal_length, &b_selmu_ecal_length);
   fChain->SetBranchAddress("selmu_ecal_showerstartpos", selmu_ecal_showerstartpos, &b_selmu_ecal_showerstartpos);
   fChain->SetBranchAddress("selmu_ecal_showerendpos", selmu_ecal_showerendpos, &b_selmu_ecal_showerendpos);
   fChain->SetBranchAddress("selmu_ecal_showerstartdir", selmu_ecal_showerstartdir, &b_selmu_ecal_showerstartdir);
   fChain->SetBranchAddress("selmu_ecal_showerenddir", selmu_ecal_showerenddir, &b_selmu_ecal_showerenddir);
   fChain->SetBranchAddress("selmu_ecal_EMenergy", selmu_ecal_EMenergy, &b_selmu_ecal_EMenergy);
   fChain->SetBranchAddress("selmu_ecal_edeposit", selmu_ecal_edeposit, &b_selmu_ecal_edeposit);
   fChain->SetBranchAddress("selmu_ecal_IsShower", selmu_ecal_IsShower, &b_selmu_ecal_IsShower);
   fChain->SetBranchAddress("selmu_ecal_mipem", selmu_ecal_mipem, &b_selmu_ecal_mipem);
   fChain->SetBranchAddress("selmu_ecal_mippion", selmu_ecal_mippion, &b_selmu_ecal_mippion);
   fChain->SetBranchAddress("selmu_ecal_emhip", selmu_ecal_emhip, &b_selmu_ecal_emhip);
   fChain->SetBranchAddress("selmu_ecal_containment", selmu_ecal_containment, &b_selmu_ecal_containment);
   fChain->SetBranchAddress("selmu_ecal_trshval", selmu_ecal_trshval, &b_selmu_ecal_trshval);
   fChain->SetBranchAddress("selmu_ecal_mostupstreamlayerhit", selmu_ecal_mostupstreamlayerhit, &b_selmu_ecal_mostupstreamlayerhit);
   fChain->SetBranchAddress("ntracks", &ntracks, &b_ntracks);
   fChain->SetBranchAddress("selmu_fgdmoduletype", &selmu_fgdmoduletype, &b_selmu_fgdmoduletype);
   fChain->SetBranchAddress("selmu_fgdlayer", &selmu_fgdlayer, &b_selmu_fgdlayer);
   fChain->SetBranchAddress("selmu_hit1_pos", selmu_hit1_pos, &b_selmu_hit1_pos);
   fChain->SetBranchAddress("selmu_hit2_pos", selmu_hit2_pos, &b_selmu_hit2_pos);
   fChain->SetBranchAddress("selmu_hit1_charge", &selmu_hit1_charge, &b_selmu_hit1_charge);
   fChain->SetBranchAddress("selmu_hit2_charge", &selmu_hit2_charge, &b_selmu_hit2_charge);
   fChain->SetBranchAddress("selmu_recEccqe", &selmu_recEccqe, &b_selmu_recEccqe);
   fChain->SetBranchAddress("truemu_fgdmoduletype", &truemu_fgdmoduletype, &b_truemu_fgdmoduletype);
   fChain->SetBranchAddress("truemu_fgdlayer", &truemu_fgdlayer, &b_truemu_fgdlayer);
   fChain->SetBranchAddress("truemu_targetZ", &truemu_targetZ, &b_truemu_targetZ);
   fChain->SetBranchAddress("truemu_targetPDG", &truemu_targetPDG, &b_truemu_targetPDG);
   fChain->SetBranchAddress("distance_track_hit1", &distance_track_hit1, &b_distance_track_hit1);
   fChain->SetBranchAddress("distance_track_hit2", &distance_track_hit2, &b_distance_track_hit2);
   fChain->SetBranchAddress("selmu_fittrack_hit1_pos", selmu_fittrack_hit1_pos, &b_selmu_fittrack_hit1_pos);
   fChain->SetBranchAddress("selmu_fittrack_hit2_pos", selmu_fittrack_hit2_pos, &b_selmu_fittrack_hit2_pos);
   fChain->SetBranchAddress("selmu_deltachi2_hit1", &selmu_deltachi2_hit1, &b_selmu_deltachi2_hit1);
   fChain->SetBranchAddress("selmu_deltachi2_hit2", &selmu_deltachi2_hit2, &b_selmu_deltachi2_hit2);
   Notify();
}

Bool_t NumuCCFGD2BackwardVertexMigrationMakeClass::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void NumuCCFGD2BackwardVertexMigrationMakeClass::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t NumuCCFGD2BackwardVertexMigrationMakeClass::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef NumuCCFGD2BackwardVertexMigrationMakeClass_cxx
