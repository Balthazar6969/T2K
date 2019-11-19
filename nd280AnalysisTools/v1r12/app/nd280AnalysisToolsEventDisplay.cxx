#if !defined(__CINT__) || defined(__MAKECINT__)
#include <AnalysisTools/ToaAnalysisEventDisplayUtils.hxx>
#endif 

void nd280AnalysisToolsEventDisplay(std::string infile, std::string geomfile){

  ToaAnalysisEventDisplayUtils disp;
  disp.runEvents(infile, geomfile);

}

//**************************************************************
int main(int argc, char **argv) {
//**************************************************************

  std::string infile_name, geom_file; 

  // Process the options.
  for (;;) {
    int c = getopt(argc, argv, "aG:v");
    if (c<0) break;
    switch (c) {
    case 'd':
      {
	// increase the debugging level.
	//	++debugLevel;
	break;
      }
    case 'G':
      {
	// Force a particular geometry file.
	geom_file = optarg;
	break;
      }

    default:
      break;
    }
  }



  std::cout << argc << " " << argv[0] << " " << argv[1] << std::endl;
  infile_name = argv[argc-1];

  nd280AnalysisToolsEventDisplay(infile_name, geom_file);

}
  

