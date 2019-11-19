#ifndef TECALEMEnergyCorrection_hxx_seen
#define TECALEMEnergyCorrection_hxx_seen

#include "libReadoaAnalysis/libReadoaAnalysisProjectHeaders.h"

#include "TSpline.h"
#include "TXMLEngine.h"
#include <TObject.h>

#include <sstream>

/// Provides a correction coefficient for reconstructed EM shower energies in
/// the downstream/barrel ECAL
/// This is a MC derived correction and is a function of energy and detector.

class TECALEMEnergyCorrection{

public:
  // object constructor, specify what production one is using
  TECALEMEnergyCorrection(std::string production);

  // object destructor
  virtual ~TECALEMEnergyCorrection();

  /// Returns the energy correction coefficient for a given energy and detector.
  /// Use the Module variable from ECALRECON's output as the detector string
  Double_t GetEnergyCorrection(Double_t inputenergy, std::string detector);

private:
  // xml processing/spline generation methods
  void populateSplines();

  // create spine for a given detector abbreviation
  void createSpline(std::string detectorname);

  // alter present working node to that specificed in argument
  void traversetoNode(std::string nodename);

  // split a string given a delimiter
  std::vector<std::string> splitString(std::string inputstring, char delimiter);

  // load and parse XML correction file
  Int_t loadCorrectionFile(std::string filename);

  // find a node in an XML tree, with the option to set this node as the present
  // working element
  XMLNodePointer_t findNode(std::string NodeName, bool change_workingnode);

  // XML working node pointers
  XMLNodePointer_t rootnode;
  XMLNodePointer_t currentworkingnode;

  // XML Engine pointers
  TXMLEngine *xmlengine;
  XMLDocPointer_t xmldoc;

  // maximum/minimum ranges of splines
  Double_t fMinimumEne, fMaximumEne;

  // map of splines
  std::map<std::string, TSpline3 *> parameter_map;
};
#endif
