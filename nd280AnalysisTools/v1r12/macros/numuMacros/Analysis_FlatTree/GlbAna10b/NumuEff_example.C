#include "NumuCutsDefinition.h"
#include "NumuDataFiles.h"

// 
// Example of how to use the NumuFlatTree
// to compute efficiency and purity
// F.Dufour (February 2011)
// Note: this code doesn't do anything real, this is just an example
// for synatx

//----------------------------------------
// Basic computer tools:
void DoEffPur(std::string base_cut,std::string test_cut){
  NumuCutsDefinition cuts;
  NumuDataFiles data;


  std::string true_cut;
  std::string sel_cut;
  std::string seltrue_cut;
  std::string here_cut[3];
  
  true_cut ="("+cuts.fgdFV_cutMC2[0]+" && (stdB_trTrueVertexIndex[0]>-1 &&  std_TrueNuPDG[stdB_trTrueVertexIndex[0]]==14 && std_ReacCode[stdB_trTrueVertexIndex[0]]>0 && std_ReacCode[stdB_trTrueVertexIndex[0]]<30))";
  sel_cut = base_cut+" && "+test_cut ; 
  seltrue_cut = base_cut+ " && "+test_cut+" && "+true_cut;
  
  
  here_cut[0]=seltrue_cut ; // Numerator, selected and true
  here_cut[1]=sel_cut ; // Denominator for purity: selected
  here_cut[2]=true_cut; // Denominator for efficiency: true
  
  TFile fmc0(data.mcFile[0].c_str()); // load neut to make check plot  

  int cut[3];
  
  for (int j=0;j<3;j++){
    fmc0.cd();
    TH1F *h1 = new TH1F("h1","h",10,0,10);
    numutree->Project("h1","stdB_Bunch",here_cut[j].c_str());
    cut[j] = h1->GetEntries();
    delete h1;
  }
  
  if ((double)cut[2]!=0 && (double)cut[1]!=0){
    std::cout<< "Eff:  " << (double)cut[0]/(double)cut[2] << "  Pur:  " << (double)cut[0]/(double)cut[1]
	     << std::endl;
  }
}


void NumuEff_example(int mode=-1){ 

  NumuCutsDefinition cuts;
  NumuDataFiles data;

  std::string base_cut = "";
  std::string test_cut;


//------------------------------
// Baseline analysis

if (mode ==1){ 
  
  cout << "Running mode ==1 " << endl;
  cout << " Gives eff and purity of baseline analysis" << endl;
  // all CC cuts based only on most energetic track
  base_cut="("+cuts.scut[0][cuts.icutCC]+")";


  test_cut="(stdB_Bunch==stdB_Bunch)";// I don't want to test anything new here     

  DoEffPur(base_cut,test_cut);

 }


}
