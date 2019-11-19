#ifndef TDISPLAYEDAnalysisModuleBase_HXX
#define TDISPLAYEDAnalysisModuleBase_HXX

#include "TChain.h"
#include "TND280Track.hxx"
#include "TND280TrackList.hxx"
#include "TND280TrackPropagator.hxx"
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TGlobalAnalysisUtils.hxx"
#include "TTruthUtils.hxx"
#endif
/// Base class for use in event display when displaying an analysis module
///\author Alex Finch (A.Finch@lancaster.ac.uk)
class TDisplayedAnalysisModuleBase {
 public:
  /// Add a file to the chain.
  void AddFile(const char *inputfilename) {
    fChain->AddFile(inputfilename);
    if (fVertexChain != NULL) fVertexChain->AddFile(inputfilename);
  };
  /// Get number of entries in the chain.
  int GetEntries() { return fChain->GetEntries(); };
  /// Centre the view on the FrontPosition of this object.
  void Centre(ND::TSubBaseObject *RO);
  /// Centre view at mean position of f and b.
  void Centre(TLorentzVector f, TLorentzVector b);
  /// Centre the view at c.
  void Centre(TLorentzVector c);
  /// Centre the view at c.
  void Centre(TVector3 c);
  /// True if location c is inside ND280.
  bool inND280(TVector3 c);
  /// True if location c is inside ND280.
  bool inND280(TLorentzVector c);
  static unsigned int indent;

 protected:
  bool fKchainOK;
  const static int NDETSEXTRAP = 23;
  TChain *fChain;
  TChain *fVertexChain;
  Int_t fNVertices;
  TClonesArray *fVertices;
  Int_t fRunID;
  Int_t fNParticles;
  Int_t fNTracks;
  Int_t fNShowers;
  TClonesArray *fTracks;
  TClonesArray *fShowers;
  TClonesArray *fParticles;

  /// Indent string by amount determined by class variable indent.
  void doIndent(TString x = "|") {
    for (unsigned int i = 0; i < (2 * indent); i++) cout << " ";
    if (indent > 0) cout << x << " ";
  };
  /// Write a divider on standard output for pretty output.
  void divider() {
    doIndent();
    for (unsigned int i = (2 * indent); i < 132; i++) cout << "=";
    cout << endl;
  };
  /// Constructor
  TDisplayedAnalysisModuleBase(TString ChainName, TString TrackName);
  /// Constructor
  TDisplayedAnalysisModuleBase(TString ChainName, TString TrackName,
                               TString VertexName, TString ShowerName,
                               TString ParticleName);
  /// Constructor
  TDisplayedAnalysisModuleBase(TString ChainName, TString VertexChainName,
                               TString TrackName, TString VertexName);

  /// Get entry from track and vertex chains and set up default track list in
  /// Eve.
  /// \return Default EveTrackList from defaultTrackList(EveName)
  TND280TrackList *PrepareToDraw(int entry, TString EveName);
  /// Set up a default  TEveTrackList with default attributes,
  /// TEveTrackPropagator, and magnetic field.
  TND280TrackList *defaultTrackList(TString EveName);
  /// Convert a LorentzVector into an EveVector.
  /// \returns EveVector copy of LorentzVector
  TEveVectorD LorentzToEve(const TLorentzVector L) {
    return TEveVectorD(L.X(), L.Y(), L.Z());
  }
  /// Convert a TVector3 into an EveVector.
  /// \returns EveVector copy of TVector3.
  TEveVectorD Vector3ToEve(const TVector3 L) {
    return TEveVectorD(L.X(), L.Y(), L.Z());
  }
  /// Create a TEveVector with magnitude and Direction given in arguments.
  /// Direction is assumed
  /// to be a unit vector.
  TEveVectorD Dir3MagToEve(double magnitude, TVector3 Direction);
  /// Return a standard colour code for a given PDG particle type.
  Color_t ColorBasedOnPDGValue(int PDG);
  /// Return true if z component of second vector is greater than that of the
  /// first.
  static bool SortInZ(TVector3 h1, TVector3 h2) { return (h1.Z() < h2.Z()); }
  /// Returns Name of detector i. 0 = FGD1 to 12 = Cavern, otherwise unknown.
  TString DetectorName(int i);
  /// Returns Name of detecot i, where 0 = TPC1, and 18 = Right Tracker ECal
  /// (oarecon convention).
  TString DetectorUsedName(int i);
#define FORMATTED_OUTPUT1 \
  doIndent();cout<<TString::Format(
#define FORMATTED_OUTPUT3 ,shortS.Data(),v)<<endl;
#define CONDITIONAL_OUTPUT                                                   \
  if (longS.Length() > 0) {                                                  \
    doIndent();                                                              \
    cout << "                                         :  " << longS << endl; \
  }
  /// Pretty printing including indenting etc.
  void Output(int v, TString shortS, TString longS) {
    FORMATTED_OUTPUT1 "%-40s : %d " FORMATTED_OUTPUT3 CONDITIONAL_OUTPUT
  }
  /// Pretty printing including indenting etc.
  void Output(TString v, TString shortS, TString longS) {
        FORMATTED_OUTPUT1 "%-40s : %s " ,shortS.Data(),v.Data())<<endl;
        CONDITIONAL_OUTPUT
  }
  /// Pretty printing including indenting etc.
  void Output(long unsigned int v, TString shortS, TString longS) {
    FORMATTED_OUTPUT1 "%-40s : %lu " FORMATTED_OUTPUT3 CONDITIONAL_OUTPUT
  }
  /// Pretty printing including indenting etc.
  void Output(UInt_t i, TString shortS, TString longS) {
    Output((int)i, shortS, longS);
  }

  /// Pretty printing including indenting etc.
  void Output(double v, TString shortS, TString longS) {
    FORMATTED_OUTPUT1 "%-40s : %f " FORMATTED_OUTPUT3 CONDITIONAL_OUTPUT
  }
  /// Pretty printing including indenting etc.
  void Output(TVector3 v, TString shortS, TString longS) {
        FORMATTED_OUTPUT1"%-40s : %f, %f, %f ",shortS.Data(),v.X(),v.Y(),v.Z())<<endl;
        CONDITIONAL_OUTPUT
  }
  /// Pretty printing including indenting etc.
  void Output(TLorentzVector &v, TString shortS, TString longS) {
        FORMATTED_OUTPUT1"%-40s : %f, %f, %f, %f ",shortS.Data(),v.X(),v.Y(),v.Z(),v.T())<<endl;
        CONDITIONAL_OUTPUT
  }
  /// Pretty printing including indenting etc.
  template <class outputType>
  void Output(std::vector<outputType> list, TString shortS, TString longS) {
    if (list.size() > 0) {
      Output(TString(""), shortS, longS);
      for (unsigned int i = 0; i < list.size(); i++) {
        doIndent();
        for (int c = 0; c < 36; c++) cout << " ";
        cout << TString::Format(" %3d : ", i) << list[i] << endl;
      }
    } else {
      cout << " There are no items in: " << shortS << endl;
    }
  }

  /// Pretty printing including indenting etc.
  void Output(double covariance[7][7], TString shortS, TString longS) {
    cout << TString::Format("%-40s : %s \n", shortS.Data(), longS.Data());
    for (int i = 0; i < 7; i++) {
      doIndent();
      for (int j = 0; j < 7; j++) {
        cout << TString::Format("%#11.3g ", covariance[i][j]);
      }
      cout << endl;
    }
  }

  int GetBunchMCTrue(double tTime);
  int GetBunch(double tTrack, int run, bool isMC, double sigma = 15.,
               double NSigmas = 4.0);
  double GetTimeOnBunch(int ibunch, int run, bool isMC);
  bool InSelectedBunch(float time, int bunch);
// ---- For real data -----------
#define NBUNCH 8
  static const double time_mc[NBUNCH];

  static const double time_data_run1[NBUNCH];

  static const double time_data_run2[NBUNCH];

  static const double time_data_run3[NBUNCH];

  static const double time_data_run4[NBUNCH];

 public:
  static bool fIsMC;
};

#endif
