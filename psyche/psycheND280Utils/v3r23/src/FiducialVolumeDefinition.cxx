#include <FiducialVolumeDefinition.hxx>
#include <DetectorDefinition.hxx>
#include <iomanip>

//dumping of array
#define dump_array3(par) std::cout << std::fixed    << \
  std::setw(10) << std::left   << #par   << ": \t"  << \
  std::setw(10) << std::right  << par[0] << " "     << \
  std::setw(10) << std::right  << par[1] << " "     << \
  std::setw(10) << std::right  << par[2] << std::endl;

namespace FVDef {

  // FV definitions: edges to be removed from the detector definitions in DetectorDefinition.cxx
  // Can be overridden in yourAnalysis::Initialize()
  // with e.g. FVDef::FVdefminFGD1[0] = x;
  // (note that FVDef::FVdefminFGD1 = {x,y,z} works only with old compilers)

  // Fiducial volume for FGDs

  // skip first module (2 layers) for FGD1
  Float_t FVdefminFGD1z = DetDef::fgdXYModuleWidth + (DetDef::fgdXYAirWidth/2.);
  // skip half module (1 layer) for FGD2
  Float_t FVdefminFGD2z = DetDef::fgdXYModuleWidth/2.;

  Float_t FVdefminFGD1[3] = {57.66, 57.66, FVdefminFGD1z};
  Float_t FVdefmaxFGD1[3] = {57.66, 57.66, 0};
  Float_t FVdefminFGD2[3] = {72.17, 72.17, FVdefminFGD2z};
  Float_t FVdefmaxFGD2[3] = {72.17, 72.17, 0};

  // Fiducial volume for P0D. From TN-73
  Float_t FVdefminP0D[3] = {256.79, 236.39, 327.48};
  Float_t FVdefmaxP0D[3] = {248.45, 261.99, 325.247};

	//BarrelECal FV for stopping point cut 
	Float_t FVdefminTECALLeft[3]   = { 0.0  , 40.0 , 40.0 };
	Float_t FVdefmaxTECALLeft[3]   = { 10.0 , 40.0 , 40.0 };
	Float_t FVdefminTECALRight[3]  = { 10  , 40.0 , 40.0 };
	Float_t FVdefmaxTECALRight[3]  = { 0.0 , 40.0 , 40.0 };
	Float_t FVdefminTECALTop[3]    = { 40.0 , 0.0  , 40.0 };
	Float_t FVdefmaxTECALTop[3]    = { 40.0 , 10.0 , 40.0 };
	Float_t FVdefminTECALBottom[3] = { 40.0 , 10.0 , 40.0 };
	Float_t FVdefmaxTECALBottom[3] = { 40.0 , 0.0  , 40.0 };

	//SMRD FV for stopping point cut 
	Float_t FVdefminSMRDLeft[3]   = { 0.0  , 0.0 , 125.0 };
	Float_t FVdefmaxSMRDLeft[3]   = { 60.0 , 0.0 , 125.0 };
	Float_t FVdefminSMRDRight[3]  = { 60.0 , 0.0 , 125.0 };
	Float_t FVdefmaxSMRDRight[3]  = { 0.0  , 0.0 , 125.0 };
	Float_t FVdefminSMRDTop[3]    = { 0.0 , 0.0  , 125.0 };
	Float_t FVdefmaxSMRDTop[3]    = { 0.0 , 60.0 , 125.0 };
	Float_t FVdefminSMRDBottom[3] = { 0.0 , 60.0 , 125.0 };
	Float_t FVdefmaxSMRDBottom[3] = { 0.0 , 0.0  , 125.0 };

  //**********************************
  void DumpFV(){
  //**********************************
    std::streamsize ss = std::cout.precision();
    std::cout.precision(3);

    std::cout << "\n***** FiducialVolume: available volumes list: ***** \n" << std::endl;

    dump_array3(FVdefminFGD1);
    dump_array3(FVdefmaxFGD1);

    dump_array3(FVdefminFGD2);
    dump_array3(FVdefmaxFGD2);

    dump_array3(FVdefminP0D);
    dump_array3(FVdefmaxP0D);

    dump_array3(FVdefminTECALLeft);
    dump_array3(FVdefmaxTECALLeft);
    dump_array3(FVdefminTECALRight);
    dump_array3(FVdefmaxTECALRight);
    dump_array3(FVdefminTECALTop);
    dump_array3(FVdefmaxTECALTop);
    dump_array3(FVdefminTECALBottom);
    dump_array3(FVdefmaxTECALBottom);

    dump_array3(FVdefminSMRDLeft);
    dump_array3(FVdefmaxSMRDLeft);
    dump_array3(FVdefminSMRDRight);
    dump_array3(FVdefmaxSMRDRight);
    dump_array3(FVdefminSMRDTop);
    dump_array3(FVdefmaxSMRDTop);
    dump_array3(FVdefminSMRDBottom);
    dump_array3(FVdefmaxSMRDBottom);

    std::cout << "\n******************************************************* \n" << std::endl;

    std::cout.precision(ss);
  }

}
