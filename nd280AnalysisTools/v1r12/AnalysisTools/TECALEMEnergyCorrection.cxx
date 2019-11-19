#include "TECALEMEnergyCorrection.hxx"

TECALEMEnergyCorrection::TECALEMEnergyCorrection(std::string production) {

  // determine which correction filename to open
  TString inputfilename;
  inputfilename.Form("production_%s_correction.xml", production.c_str());

  // load correction file
  Int_t loadresult = this->loadCorrectionFile(inputfilename.Data());

  if (loadresult == -1) {
    std::cout << "Problem loading energy correction XML files, cannot continue"
              << std::endl;
    exit(1);
  }

  // fill correction splines for each subdetector
  this->populateSplines();
}

TECALEMEnergyCorrection::~TECALEMEnergyCorrection() {
  // cleanup
  rootnode = NULL;
  currentworkingnode = NULL;
  xmlengine = NULL;
  xmldoc = NULL;
}

Double_t TECALEMEnergyCorrection::GetEnergyCorrection(Double_t inputenergy,
                                                      std::string detector) {

  // obtain correct spline based on stored ECAL detector name
  TSpline3 *thespline = parameter_map[detector];

  if (thespline) {
    // obtain spline operating limits
    fMinimumEne = thespline->GetXmin();
    fMaximumEne = thespline->GetXmax();

    // if input energy is within spline's limits, lookup correction coefficient
    // and return it
    // failing that, leave the energy unchanged
    if ((inputenergy > fMinimumEne) && (inputenergy < fMaximumEne)) {
      return thespline->Eval(inputenergy);
    } else {
      return 1.0;
    }
  }

  // if the spline lookup fails, return the energy unchanged

  else {
    return 1.0;
  }
}

void TECALEMEnergyCorrection::populateSplines() {

  // create spline for each ECAL module from data stored in XML file
  const char *detectors_char[] = {"BBL", "BBR", "BSR", "BTR",
                                  "BTL", "BSL", "DS"};

  std::vector<std::string> detectors(detectors_char, detectors_char + 7);

  for (std::vector<std::string>::iterator detector = detectors.begin();
       detector != detectors.end(); detector++) {

    this->createSpline(*detector);
  }

  // free XML pointers
  xmlengine->FreeDoc(xmldoc);
  delete xmlengine;
}

void TECALEMEnergyCorrection::createSpline(std::string detectorname) {

  // grab pointer to appropriate XML node
  TString pathtomodule;
  pathtomodule.Form("/EMEnergyCorrection/%s/", detectorname.c_str());
  this->traversetoNode(pathtomodule.Data());

  // determine information for the spline
  Int_t NumPoints = atoi(xmlengine->GetAttr(
      xmlengine->GetParent(currentworkingnode), "num_points"));
  std::string det_longname = xmlengine->GetAttr(
      xmlengine->GetParent(currentworkingnode), "detector_str");

  // create input Double_t arrays for input to spline ctor
  Double_t *evalues = new Double_t[NumPoints];
  Double_t *values = new Double_t[NumPoints];

  Double_t energy;
  Double_t value;

  int j = 0;

  // load data from XML nodes into Double_t arrays
  for (XMLNodePointer_t currentelement = currentworkingnode;
       currentelement != NULL;
       currentelement = xmlengine->GetNext(currentelement)) {
    energy = atof(xmlengine->GetAttr(currentelement, "recoenergy"));
    value = atof(xmlengine->GetAttr(currentelement, "correction"));

    evalues[j] = energy;
    values[j] = value;

    j++;
  }

  // set string->spline map with appropriate spline
  parameter_map[det_longname] =
      new TSpline3(det_longname.c_str(), evalues, values, NumPoints, "", 0, 0);

  // cleanup
  delete[] evalues;
  delete[] values;
}

Int_t TECALEMEnergyCorrection::loadCorrectionFile(std::string filename) {

  // obtain ND280ANALYSISROOT environment variable
  char *environment_path_cstr = getenv("ND280ANALYSISTOOLSROOT");

  // check it exists, return failed if absent
  if (!environment_path_cstr) {
    std::cerr << "ND280ANALYSISTOOLSROOT path environment variable is not set, "
                 "cannot load energy correction parameters."
              << std::endl;
    return -1;
  }

  // construct path and filename to be opened
  std::string pathtofile(environment_path_cstr);
  pathtofile += ("/AnalysisTools/" + filename);

  // create XML engine object and parse the file
  xmlengine = new TXMLEngine;
  xmldoc = xmlengine->ParseFile(pathtofile.c_str());

  if (!xmldoc) {
    std::cerr << "Failed to parse XML file." << std::endl;
    return -1;
  }

  rootnode = xmlengine->DocGetRootElement(xmldoc);

  return 1;
}

// splits a string given a delimiter and returns a std::vector of elements
std::vector<std::string>
TECALEMEnergyCorrection::splitString(std::string inputstring, char delimiter) {

  std::string element;
  std::stringstream inputstream(inputstring);
  std::vector<std::string> thelist;

  while (getline(inputstream, element, delimiter)) {
    if (element != "")
      thelist.push_back(element);
  }

  return thelist;
}

// method to set the current working node given a slash delimited path
void TECALEMEnergyCorrection::traversetoNode(std::string destination) {

  currentworkingnode = rootnode;

  std::vector<std::string> elementlist = this->splitString(destination, '/');

  XMLNodePointer_t result;

  for (std::vector<std::string>::iterator theelement = elementlist.begin();
       theelement != elementlist.end(); theelement++) {
    result = this->findNode(*theelement, true);

    if (!result) {
      break;
    }

    currentworkingnode = xmlengine->GetChild(currentworkingnode);
  }
}

// method to find a particular node in a set of subnodes given a name
XMLNodePointer_t TECALEMEnergyCorrection::findNode(std::string NodeName,
                                                   bool change_workingnode) {
  XMLNodePointer_t currentelement;

  for (currentelement = currentworkingnode; currentelement != NULL;
       currentelement = xmlengine->GetNext(currentelement)) {

    if (NodeName == xmlengine->GetNodeName(currentelement)) {
      if (change_workingnode) {
        currentworkingnode = currentelement;
      }

      break;
    }
  }

  return currentelement;
}
