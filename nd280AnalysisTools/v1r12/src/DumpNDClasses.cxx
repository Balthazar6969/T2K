#include <algorithm>
#include <fstream>
#include <iostream>
#include "TCollection.h"
#include "TError.h"
#include "TFile.h"
#include "TStreamerInfo.h"
#include "TString.h"
#include "TTree.h"

int main(int argc, char const* argv[]) {
  // shhh ROOT it okay.
  gErrorIgnoreLevel = kError;

  if (argc != 4) {
    std::cout << "[ERROR](DumpClasses.C): Recieved " << (argc - 1)
              << " CLI opts." << std::endl;
    for (int i = 1; i < argc; ++i) {
      std::cout << "[" << i << "] \"" << argv[i] << "\"" << std::endl;
    }
    std::cout << "Usage: " << argv[0]
              << " <Input ROOT filename> "
                 "<Project source output directory> <ClassName Prefix To "
                 "Include in LinkDef>"
              << std::endl;
    return 1;
  }
  char const* fln = argv[1];
  char const* od = argv[2];
  char const* cl_pref = argv[3];
  TFile* fl = TFile::Open(fln);
  if (!fl || !fl->IsOpen()) {
    std::cerr << "[ERROR](DumpClasses.C): Failed to open root file: " << fl
              << std::endl;
    return 1;
  }
  std::cout << "[INFO]: Writing project files to " << od << std::endl;
  fl->MakeProject(od);

  TIter next(fl->GetStreamerInfoList());
  TStreamerInfo* inf = 0;
  std::vector<std::string> ClassesToLink;
  while ((inf = dynamic_cast<TStreamerInfo*>(next()))) {
    if (TString(inf->GetName()).BeginsWith(cl_pref)) {
      ClassesToLink.push_back(inf->GetName());
      std::cout << "Including streamer info for: " << inf->GetName()
        << " in generated LinkDef" << std::endl;
    } else {
      std::cout << "Found streamer info for: " << inf->GetName() << std::endl;
    }
  }

  std::string OutputDir(od);

  size_t pos = OutputDir.find_last_of('/');
  pos = (pos == std::string::npos) ? 0 : pos;

  std::string LinkDef_loc =
      OutputDir + "/" + OutputDir.substr(pos + 1) + "LinkDef.h";
  std::ofstream LinkDefos(LinkDef_loc);

  if (!LinkDefos.is_open()) {
    std::cerr << "[ERROR] Couldn't open " << LinkDef_loc << " for writing."
              << std::endl;
  }

  LinkDefos << "#ifdef __CINT__" << std::endl;
  for (auto cl : ClassesToLink) {
    LinkDefos << "#pragma link C++ class  " << cl << "+;" << std::endl;
  }
  LinkDefos << "#endif" << std::endl;
  LinkDefos.close();

  std::string Instances_loc =
      OutputDir + "/" + OutputDir.substr(pos + 1) + "ProjectInstances.h";
  std::ofstream Instancesos(Instances_loc);
  if (!Instancesos.is_open()) {
    std::cerr << "[ERROR] Couldn't open " << Instances_loc << " for writing."
              << std::endl;
  }
  Instancesos.close();

  std::string Headers_loc =
      OutputDir + "/" + OutputDir.substr(pos + 1) + "ProjectHeaders.h";
  std::ofstream Headersos(Headers_loc, std::ofstream::app);

  TTree* BasicHeaderTree = (TTree*)fl->Get("HeaderDir/BasicHeader");
  if (!BasicHeaderTree) {
    std::cerr << "*************************************************************"
                 "*****************"
              << std::endl;
    std::cerr << "* WARNING: input file doesn't contain HeadeDir/BasicHeader "
                 "tree!             *"
              << std::endl;
    std::cerr << "* Pre-processor variables cannot be set!                     "
                 "                *"
              << std::endl;
    std::cerr << "*************************************************************"
                 "*****************"
              << std::endl;
    fl->Close();
    return 1;
  }

  char version[50];
  BasicHeaderTree->SetBranchAddress("SoftwareVersion", &version);
  BasicHeaderTree->GetEntry(0);
  fl->Close();
  BasicHeaderTree = nullptr;
  fl = nullptr;

  if (!strcmp(version, "") || !strcmp(version, "NULL")) {
    std::cerr << "*************************************************************"
                 "*****************"
              << std::endl;
    std::cerr << "* WARNING: Your oaAnalysis file does not say which "
                 "SoftwareVersion was used! *"
              << std::endl;
    std::cerr << "* >>>>>>>>> IF YOU ARE USING HIGHLAND, THEN THIS MAY BE A "
                 "PROBLEM! <<<<<<<<< *"
              << std::endl;
    std::cerr << "* If you created the file yourself, please re-run oaAnalysis "
                 "with the        *"
              << std::endl;
    std::cerr << "* -O TBasicHeaderModule=software_version option enabled.     "
                 "                *"
              << std::endl;
    std::cerr << "*************************************************************"
                 "*****************"
              << std::endl;
    return 2;
  }

  std::string vStr = "";
  std::string rStr = "";
  std::string pStr = "";
  std::string versionStr(version);

  size_t vloc = versionStr.find("v");
  size_t rloc = versionStr.find("r");
  size_t ploc = versionStr.find("p");
  size_t size = versionStr.length();

  if (vloc == std::string::npos) {
    std::cerr << "WARNING: SoftwareVersion " << versionStr
              << " is not a valid format!" << std::endl;
    exit(3);
  }
  if (rloc == std::string::npos) {
    // vX
    vStr = versionStr.substr(vloc + 1, size - (vloc + 1));
    rStr = "0";
    pStr = "0";
  } else if (ploc == std::string::npos) {
    // vXrY
    vStr = versionStr.substr(vloc + 1, rloc - (vloc + 1));
    rStr = versionStr.substr(rloc + 1, size - (rloc + 1));
    pStr = "0";
  } else {
    // vXrYpZ
    vStr = versionStr.substr(vloc + 1, rloc - (vloc + 1));
    rStr = versionStr.substr(rloc + 1, ploc - (rloc + 1));
    pStr = versionStr.substr(ploc + 1, size - (ploc + 1));
  }

  Headersos << "/* Version information generated by DumpNDClassex.cxx */"
            << std::endl;
  Headersos << "#ifndef ANATOOLS_FILE_VERSION" << std::endl;
  Headersos << "#define ANATOOLS_FILE_VERSION \"" << versionStr << "\""
            << std::endl;
  Headersos << "#define ANATOOLS_FILE_MAJOR_VERSION " << vStr << std::endl;
  Headersos << "#define ANATOOLS_FILE_MINOR_VERSION " << rStr << std::endl;
  Headersos << "#define ANATOOLS_FILE_PATCH_VERSION " << pStr << std::endl;
  Headersos << "#define BEFORE_ANATOOLS_FILE(v,r,p) "
               "((ANATOOLS_FILE_MAJOR_VERSION==(v)&&ANATOOLS_FILE_MINOR_"
               "VERSION==(r)&&ANATOOLS_FILE_PATCH_VERSION<(p))||(ANATOOLS_FILE_"
               "MAJOR_VERSION==(v)&&ANATOOLS_FILE_MINOR_VERSION<(r))||("
               "ANATOOLS_FILE_MAJOR_VERSION<(v)))"
            << std::endl;
  Headersos << "#define EQUALS_ANATOOLS_FILE(v,r,p) "
               "(ANATOOLS_FILE_MAJOR_VERSION==(v)&&ANATOOLS_FILE_MINOR_VERSION="
               "=(r)&&ANATOOLS_FILE_PATCH_VERSION==(p))"
            << std::endl;
  Headersos << "#define ANATOOLS_FILE_PROD5 BEFORE_ANATOOLS_FILE(11,0,0)"
            << std::endl;
  Headersos << "#define ANATOOLS_FILE_AFTERPROD5 !BEFORE_ANATOOLS_FILE(11,0,0)"
            << std::endl;
  Headersos << "#endif" << std::endl;
  Headersos.close();

  std::cout << "[INFO]: Added version information, \"" << versionStr
            << "\", to " << Headers_loc << std::endl;

  std::cout << "[INFO]: Done." << std::endl;
  return 0;
}
