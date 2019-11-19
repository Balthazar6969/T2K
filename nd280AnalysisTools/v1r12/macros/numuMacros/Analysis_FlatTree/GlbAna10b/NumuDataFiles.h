#ifndef NumuDataFiles_h
#define NumuDataFiles_h


#include "string.h"

class NumuDataFiles{

 public:
      
std::string dataFileList; 
  std::string dataFile; 
  std::string mcFile[4]; 
  std::string dataFile_txt; 
  std::string mcFile_txt[4]; 
  double mcNorm[4]; 
  double mcNorm_3132[4]; 
  double mcNorm_3334[4]; 
  double dataPOT;
  double dataPOT_3132;
  double dataPOT_3334;


  NumuDataFiles(){
    dataFileList = "datafiles_used.txt";
    dataFile ="/neutrino/data4/t2k/production004/4Brdp/numuflattree/numuflattree_datarun1.root";
    mcFile[0] = "/neutrino/data4/t2k/production004/4Bmcp/neut/2010-02-water/numuflattree/numuflattree_neut.root";
    mcFile[1] = "/neutrino/data4/t2k/production004/4Bmcp/neut/2010-02-water/numuflattree/numuflattree_neut.root";
    dataFile_txt ="/neutrino/data4/t2k/production004/4Brdp/numuflattree/numuflattree_4Brdp_usedfiles.txt";
    mcFile_txt[0] = "/neutrino/data4/t2k/production004/4Bmcp/neut/2010-02-water/numuflattree/numuflattree_4Bmcp_neut_usedfiles.txt";
    mcFile_txt[1] = "/neutrino/data4/t2k/production004/4Bmcp/genie/2010-02-water/numuflattree/numuflattree_4Bmcp_genie_usedfiles.txt";

    // If you runn with all data sets, the following is useful
    dataPOT = 2.7; //for 2010a analysis
    dataPOT_3132 = 0.8152; //beam runs 31-32 (run<4600) 
    dataPOT_3334 = 1.8891; //beam runs 33-34 (run>4600)

    mcNorm[0] = dataPOT/10; //for 200 Mc Files
    mcNorm[1] = dataPOT/10; //for 200 Mc Files

    mcNorm_3132[0] = dataPOT_3132/10;//for 200 Mc Files
    mcNorm_3132[1] = dataPOT_3132/10;//for 200 Mc Files

    mcNorm_3334[0] = dataPOT_3334/10;    //for 200 Mc Files
    mcNorm_3334[1] = dataPOT_3334/10;    //for 200 Mc Files


  }

};

#endif
