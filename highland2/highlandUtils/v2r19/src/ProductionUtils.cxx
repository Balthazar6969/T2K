#include "ProductionUtils.hxx"

//********************************************************************
void anaUtils::AddStandardProductions(){
//********************************************************************

  // Add the different productions
  ND::versioning().AddProduction(ProdId::PROD5D,     "PROD5D",      "v10r11",    "v5r11");
  ND::versioning().AddProduction(ProdId::PROD5E,     "PROD5E",      "v10r11p17", "v5r15p1");
  ND::versioning().AddProduction(ProdId::PROD5F,     "PROD5F",      "v10r11p19", "v5r15p3");
  ND::versioning().AddProduction(ProdId::PROD5G,     "PROD5G",      "v10r11p27", "v5r15p3");
  ND::versioning().AddProduction(ProdId::PROD6PRE,   "PROD6PRE",    "v11r17",    "v11r28"); //TODO
  ND::versioning().AddProduction(ProdId::PROD6A,     "PROD6A",      "v11r29",    "v5r29p17");
  ND::versioning().AddProduction(ProdId::PROD6B,     "PROD6B",      "v11r31",    "v5r29p19");
  ND::versioning().AddProduction(ProdId::PROD6C,     "PROD6C",      "v11r31",    "v5r29p19");
  ND::versioning().AddProduction(ProdId::PROD6D,     "PROD6D",      "v11r31",    "v5r29p19");
  ND::versioning().AddProduction(ProdId::PROD6E,     "PROD6E",      "v11r31p5",  "v5r29p21");
  ND::versioning().AddProduction(ProdId::PROD6F,     "PROD6F",      "v11r31",    "v5r29p25");
  ND::versioning().AddProduction(ProdId::PROD6G,     "PROD6G",      "v11r31p7",  "v5r29p25");
  ND::versioning().AddProduction(ProdId::PROD6H,     "PROD6H",      "v11r31p9",  "v5r31p9");
  ND::versioning().AddProduction(ProdId::PROD6I,     "PROD6I",      "v11r37",    "v5r31p9");
  ND::versioning().AddProduction(ProdId::PROD6J,     "PROD6J",      "v11r31p11", "v5r29p19");
  ND::versioning().AddProduction(ProdId::PROD6K,     "PROD6K",      "v11r31p5",  "v5r29p21");
  ND::versioning().AddProduction(ProdId::PROD6L,     "PROD6L",      "v11r31p13", "v5r29p25");
  ND::versioning().AddProduction(ProdId::PROD6M,     "PROD6M",      "v11r31p5",  "v5r29p21");

  
  //May change when production evolves
  ND::versioning().AddProduction(ProdId::PROD7DEVEL, "PROD7DEVEL", "v12",        "v13");
}

//********************************************************************
ProdId_h anaUtils::GetProductionIdFromoaAnalysisReader(){
//********************************************************************

  ProdId_h prodId=UNKNOWN_PRODUCTION;

  // Add here only software versions having a reader in oaAnalysisReader package, which correspond to different versions of oaAnalysis

  // ---- Production 5 ----
#if   EQUALS_ANATOOLS_FILE(10,11,0)
  prodId=ProdId::PROD5D;  
#elif EQUALS_ANATOOLS_FILE(10,11,17)
  prodId=ProdId::PROD5E;
#elif EQUALS_ANATOOLS_FILE(10,11,19)
  prodId=ProdId::PROD5F;

  // ---- Production 6 ----
#elif (!BEFORE_ANATOOLS_FILE(11,17,0) && BEFORE_ANATOOLS_FILE(11,29,0))
  prodId=ProdId::PROD6PRE;  
#elif EQUALS_ANATOOLS_FILE(11,31,0)
  prodId=ProdId::PROD6B;  
#elif EQUALS_ANATOOLS_FILE(11,31,5)
  prodId=ProdId::PROD6E;
#elif EQUALS_ANATOOLS_FILE(11,31,7)
  prodId=ProdId::PROD6G;
#elif EQUALS_ANATOOLS_FILE(11,31,9)  // TREx version
  prodId=ProdId::PROD6H;

  // ---- Production 7 ----
#elif !BEFORE_ANATOOLS_FILE(12,0,0)
  prodId=ProdId::PROD7DEVEL;

#endif

  return prodId;

}
