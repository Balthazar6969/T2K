#ifndef Stage2GasSelection_h_seen
#define Stage2GasSelection_h_seen

#include "TCanvas.h"
#include "TChain.h"
#include "TClonesArray.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TROOT.h"
#include "TRandom3.h"
#include "TTree.h"
#include "libReadoaAnalysis/libReadoaAnalysisProjectHeaders.h"
#include <fstream>
#include <iostream>
#include <list>
#include <set>
#include <sstream>

// To use AnalysisTools utils in ACLiC mode then need to add the relevant header
// here and inside the ifdef so it is not seen by CINT.
//#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TTruthUtils.hxx"
//#endif

// Utilities
//-----------------

typedef std::pair<ND::TTRExReconModule::TTPCAnaPath *,
                  ND::TTRExReconModule::TTPCAnaPattern *>
    path_pattern_pair;
typedef std::pair<ND::TTRExReconModule::TTPCAnaPath *,
                  std::vector<ND::TTRExReconModule::TTPCAnaJunction *> >
    path_junctions_pair;

// Struct for storing simple information about the end points of muon paths
struct PathEnd {
  TVector3 Position;
  ND::TTRExReconModule::TTPCAnaPath *Path;
  ND::TTRExReconModule::TTPCAnaJunction *Junction;
};

// Struct for storing the information that is saved to the output tree
struct VertexCandidate {
  int NTracks;
  int NFittedTracks;
  int NProtonTracks;
  TVector3 Position;
  double Time;

  int MatchedTruthCode;
  TVector3 MatchedPosition;
  int MatchedSubdetector;
  int MatchedTargetZ;
  int MatchedNProtons;
  double MatchedNeutrinoMomentum;
  double MatchedMuonMomentum;
  double MatchedTotalMomentum;

  double VertexCleanliness;
  double VertexCompleteness;

  TVector3 VertexExtent;
  double TotalRecoMomentum;
  double LowestRecoMomentum;
  double HMNTMomentum;
  bool HMNTIsMuon;

  double MuMomDiff;
  double LowestAngle;
  int NAssociatedPaths;
};

// Simple functions for sorting paths by momentum
bool MomentumGreaterThan_V1(std::pair<ND::TTRExReconModule::TTPCAnaPath *,
                                      ND::TTRExReconModule::TTPCAnaPattern *>
                                path_A,
                            std::pair<ND::TTRExReconModule::TTPCAnaPath *,
                                      ND::TTRExReconModule::TTPCAnaPattern *>
                                path_B);

bool MomentumGreaterThan_V2(
    std::pair<ND::TTRExReconModule::TTPCAnaPath *,
              std::vector<ND::TTRExReconModule::TTPCAnaJunction *> >
        path_A,
    std::pair<ND::TTRExReconModule::TTPCAnaPath *,
              std::vector<ND::TTRExReconModule::TTPCAnaJunction *> >
        path_B);

// Simple function for testing if a point is in the fiducial volume
bool isInFV(TVector3 max_coords, TVector3 min_coords, TVector3 testpos);

// Simple function for finding the momentum of a CC primary muon
TLorentzVector
primaryMuonMomentum(ND::TTruthVerticesModule::TTruthVertex *parent_vertex,
                    int nvtx, TClonesArray *vertices, bool is_it_neut);

// Simple function for counting the charged trajectories from a true vertex
int countChargedTraj(ND::TTruthVerticesModule::TTruthVertex *parent_vertex,
                     int pdg, Int_t N_RooTrackerVertices,
                     TClonesArray *RooTrackerVertices, bool is_it_neut);

// Simple function for assessing the total momentum from a true vertex
double findTotalMomentum(ND::TTruthVerticesModule::TTruthVertex *parent_vertex,
                         TTruthUtils trutil, int pdg);

// Truth matching functions
//------------------------
int trueVertexTruthCode(ND::TTruthVerticesModule::TTruthVertex *vert,
                        TVector3 max_coords, TVector3 min_coords, int tpc,
                        bool isGENIE);

int patternTrueVertexMatch(ND::TTRExReconModule::TTPCAnaPattern *patt,
                           TTruthUtils trutil);

int getTrueVertexFromJunction(
    std::vector<ND::TTRExReconModule::TTPCAnaPath *> vertex_paths,
    TTruthUtils trutil);

int evaluatePathTruth(
    ND::TTRExReconModule::TTPCAnaPath *path,
    std::vector<ND::TTRExReconModule::TTPCAnaPattern *> other_matched_patterns,
    TTruthUtils trutil, TVector3 fv_maxima, TVector3 fv_minima, int tpc,
    bool isGENIE);

int evaluateJunctionTruth(
    TVector3 junction_pos,
    std::vector<ND::TTRExReconModule::TTPCAnaPath *> vertex_paths,
    std::vector<ND::TTRExReconModule::TTPCAnaPattern *> other_matched_patterns,
    TTruthUtils trutil, TVector3 fv_maxima, TVector3 fv_minima, int tpc,
    bool isGENIE);

// Vertex finding tools
//--------------------

std::vector<PathEnd> getPathEnds(
    ND::TTRExReconModule::TTPCAnaPath *muon_path,
    std::vector<ND::TTRExReconModule::TTPCAnaJunction *> connected_junctions);

std::vector<std::pair<ND::TTRExReconModule::TTPCAnaPath *,
                      std::vector<ND::TTRExReconModule::TTPCAnaJunction *> > >
leapFrog(
    ND::TTRExReconModule::TTPCAnaPath *muon_path,
    std::vector<ND::TTRExReconModule::TTPCAnaJunction *> connected_junctions,
    ND::TTRExReconModule::TTPCAnaPattern *parent_pattern);

// PID/topology cuts
//-----------------
bool isInBunch(int run_number, double T0, bool isMC);

bool isMuon(double muon_pull, double pion_pull, double proton_pull,
            double electron_pull, double momentum);

bool isProton(double muon_pull, double pion_pull, double proton_pull,
              double electron_pull, double momentum);

bool isDelta(ND::TTRExReconModule::TTPCAnaJunction *junc,
             ND::TTRExReconModule::TTPCAnaPattern *parent_pattern);

bool isBroken(ND::TTRExReconModule::TTPCAnaJunction *junc,
              ND::TTRExReconModule::TTPCAnaPattern *parent_pattern,
              TClonesArray *Patterns, int NPatterns, TVector3 fv_maxima,
              TVector3 fv_minima);

int evaluateFinalCuts(PathEnd end,
                      ND::TTRExReconModule::TTPCAnaPattern *parent_pattern,
                      TClonesArray *Patterns, int NPatterns, TVector3 fv_maxima,
                      TVector3 fv_minima);

// Recon performance tests
//-----------------------
double evaluateVertexCompleteness(
    std::vector<ND::TTRExReconModule::TTPCAnaPath *> paths,
    ND::TTruthVerticesModule::TTruthVertex *parent_vertex, TTruthUtils trutil);

double evaluateVertexCleanliness(
    std::vector<ND::TTRExReconModule::TTPCAnaPath *> paths,
    ND::TTruthVerticesModule::TTruthVertex *parent_vertex, TTruthUtils trutil);

// Function to fill vertex candidate struct
void fillVertexInfo(
    VertexCandidate &vert_cand,
    ND::TTRExReconModule::TTPCAnaPattern *parent_pattern, PathEnd selected_end,
    std::map<int, std::vector<ND::TTRExReconModule::TTPCAnaPattern *> >
        pattern_truth,
    TTruthUtils trutil, TVector3 fv_maxima, TVector3 fv_minima, int tpc,
    bool isMC, Int_t N_RooTrackerVertices, TClonesArray *RooTrackerVertices,
    bool is_it_neut, bool hasTrackerVertices);

void Stage2GasSelection(char *inputfilename, char *outputfilename,
                        int max_entries = -1);

#endif
