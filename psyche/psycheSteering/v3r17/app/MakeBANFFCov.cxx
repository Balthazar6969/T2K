#include "AnalysisManager.hxx"
//#include <gperftools/profiler.h>
#include <sys/time.h>
#include <iostream>
#include <unistd.h>
#include "sys/types.h"
//#include "sys/sysinfo.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "Parameters.hxx"
#include "ToyMakerExample.hxx"
#include "TRandom3.h"
#include "TH2F.h"
#include "TH1F.h"
#include "AnalysisUtils.hxx"
#include "MultiThread.hxx"

#ifdef MULTITHREAD
#include "omp.h"
#endif 

int parseLine(char* line){
    int i = strlen(line);
    while (*line < '0' || *line > '9') line++;
    line[i-3] = '\0';
    i = atoi(line);
    return i;
}


int getValue(){ //Note: this value is in KB!
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];
    if (!file) return 0;

    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmSize:", 7) == 0){
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result;
}
void  DivideMatrix(const int nbins,TH2F *hratio,TH2F *hnum, TH2F *hden){
  for(int i=0;i<nbins;i++){
    for(int j=0;j<nbins;j++){
      double num=hnum->GetBinContent(i+1,j+1);
      double den=hden->GetBinContent(i+1,j+1);
      double ratio=0;
      if(den!=0)
	ratio=num/den;
      hratio->SetBinContent(i+1,j+1,ratio);
    }
  }
}
void  MakeErrorMatrix(const int nbins,TH2F *hcov,TH2F *herr){
  for(int i=0;i<nbins;i++){
    for(int j=0;j<nbins;j++){
      double cov=hcov->GetBinContent(i+1,j+1);
      double sign=0;
      if(cov!=0)
	sign=cov/fabs(cov);
      herr->SetBinContent(i+1,j+1,sqrt(fabs(cov))*sign);
    }
  }
}
void  MakeCovarianceBANFFStyle(const int nbins,int ntoys,TH2F *hcov,TH2F *hcov_st,TH2F *hcov_w,TH1F *prior, TH2F *hn_va_norm, TH1F *hn_w0,TH1F *hn_Nom){
  float NA_av=0;
  float na_av[500]={0};
  if(nbins>500){ std::cout<<" too small size definition"<<std::endl; return;}
  for(int i=0;i<nbins;i++){
    na_av[i]=0;
    for(int ia=0;ia<ntoys;ia++){
      float nai=hn_va_norm->GetBinContent(ia+1,i+1);
      na_av[i]+=nai;
      NA_av+=nai;
    }
    na_av[i]=na_av[i]/(float)ntoys;
    float nomi=hn_Nom->GetBinContent(i+1);
    prior->SetBinContent(i+1,na_av[i]/nomi);
  }
  float cov_st[500][500]={{0}};
  float cov_ww[500][500]={{0}};
  float cov[500][500]={{0}};
  std::cout<<" BANFF Style\n********************** "<<std::endl;
  for(int i=0;i<nbins;i++){
    for(int j=0;j<nbins;j++){
      cov_st[i][j]=0;
      float niw,njw,nomi,nomj;
      niw=hn_w0->GetBinContent(i+1);
      njw=hn_w0->GetBinContent(j+1);
      nomi=hn_Nom->GetBinContent(i+1);
      nomj=hn_Nom->GetBinContent(j+1);
      if(nomj!=0 && nomi!=0){
	if(nomi!=nomi || nomj!=nomj || niw!=niw || njw!=njw){
	  if(i==j)
	    std::cout<<" nomi "<<nomi<<" niw "<<niw<<std::endl;
	  cov_ww[i][j]=0;
	}else
	  cov_ww[i][j]=(nomi    -niw)*(nomj    -njw)/(nomi*nomj);
	
      }
      // float nnomi=hn_nom->GetBinContent(i+1);
      for(int ia=0;ia<ntoys;ia++){
	float nai,naj;
	nai=hn_va_norm->GetBinContent(ia+1,i+1);
	naj=hn_va_norm->GetBinContent(ia+1,j+1);
	if(nomj!=0 && nomi!=0){
	  cov_st[i][j]+=(na_av[i]-nai)*(na_av[j]-naj)/(nomi*nomj*ntoys);
	}
	if(nai!=nai || naj!=naj || na_av[i]!=na_av[i] || na_av[j]!=na_av[j])
	  std::cout<<" NAN !!!! ni"<<nai<<" or av:"<<na_av[i]<<std::endl;
      }
      //cov[i][j]=cov_ww[i][j]+cov_st[i][j];
      double ccc=cov_ww[i][j]+cov_st[i][j];
      if(i==j)
	std::cout<<i<<" total "<<sqrt(ccc)<<" st "<<sqrt(cov_st[i][j])<<" w "<<sqrt(cov_ww[i][j])<<std::endl;
      hcov->SetBinContent(i+1,j+1,ccc);
      hcov_st->SetBinContent(i+1,j+1,cov_st[i][j]);
      hcov_w->SetBinContent(i+1,j+1,cov_ww[i][j]);
    }
  }

}

void  MakeCovarianceNUMUStyle(const int nbins,int ntoys,TH2F *hcov, TH2F *hn_va,  TH2F *hn_a,TH1F *hn_Nom){
  if(nbins>500){ std::cout<<" too small size definition"<<std::endl; return;}
  float NA_av=0;
  float na_av[500]={0};
  for(int i=0;i<nbins;i++){
    na_av[i]=0;
    for(int ia=0;ia<ntoys;ia++){
      float nai=hn_va->GetBinContent(ia+1,i+1);
      na_av[i]+=nai;
      NA_av+=nai;
    }
    na_av[i]=na_av[i]/(float)ntoys;
  }
  std::cout<<" numu Style\n********************** "<<std::endl;
  float cov[500][500]={{0}};
  for(int i=0;i<nbins;i++){
    for(int j=0;j<nbins;j++){
      cov[i][j]=0;
      // float nnomi=hn_nom->GetBinContent(i+1);
      for(int ia=0;ia<ntoys;ia++){
	float nai,naj,nomi,nomj;
	nai=hn_a->GetBinContent(ia+1,i+1);
	naj=hn_a->GetBinContent(ia+1,j+1);
	nomi=hn_Nom->GetBinContent(i+1);
	nomj=hn_Nom->GetBinContent(j+1);
	// if(na_av[i]!=0 && na_av[j]!=0)
	//   cov[i][j]+=(na_av[i]-nai)*(na_av[j]-naj)/(na_av[i]*na_av[j]*ntoys);
	if(nomj!=0 && nomi!=0)
	  cov[i][j]+=(na_av[i]-nai)*(na_av[j]-naj)/(nomi*nomj*ntoys);

      if(nai!=nai || naj!=naj || na_av[i]!=na_av[i] || na_av[j]!=na_av[j])
	std::cout<<" NAN !!!! ni"<<nai<<" or av:"<<na_av[i]<<std::endl;
      
      }
      if(i==j)
	std::cout<<i<<" total "<<sqrt(cov[i][j])<<std::endl;
  
      hcov->SetBinContent(i+1,j+1,cov[i][j]);
    }
  }
  
}




int main(int argc, char *argv[]){

    // Usage:  MakeBANFFEventHistograms.exe -n N (preloads N events) -d 1/0 (swith on debugging mode, default off) -p X (X = Data/MC POT ratio to apply, default to 1) -b 1/0 (normalise by bin area in histograms, default off) -o outputfilename /path/to/input/file

    std::string programName = argv[0];
    std::string paramFile = "";
    Int_t nmax = -1;
    UInt_t ntoys=1;
    std::string inputFileName = "";
    std::string inputFileType = "kHighlandTree";
    Int_t debug=0;
    double pot_ratio = 1;
    bool normalise = 0;
    const char* outfilename = NULL;

    for (;;) {
        int c = getopt(argc, argv, "n:t:o:d:p:b:");
        if (c < 0)
            break;
        switch (c) {
	case 'n': {
	  std::istringstream tmp(optarg);
	  tmp >> nmax;
	  break;
	}
	case 't': {
	  std::istringstream tmp(optarg);
	  tmp >> ntoys;
	  break;
	}
	case 'o': {
	  outfilename = optarg;
	  break;
	}
	case 'd': {
	  std::istringstream tmp(optarg);
	  tmp >> debug;
	  break;
	}
	case 'p': {
	  std::istringstream tmp(optarg);
	  tmp >> pot_ratio;
	  break;
	}
	case 'b': {
	  std::istringstream tmp(optarg);
	  tmp >> normalise;
	  break;
	}
	default: {
	}
        }
    }

    // Read the parameters files following the package hierarchy
    // first the top level package. Set the parameters as fixed
    ND::params().LoadParametersFiles(anaUtils::GetPackageHierarchy(), true);

    // Make sure no parameters have been accessed yet
    ND::params().SetReadParamOverrideFilePointPassed();

    
    int npbins[7]={10,10,10,5,5,5,5};
    int ncbins[7]={7,7,7,4,4,4,4};
    int nbins=0;
    for(int is=0;is<7;is++) nbins+=(ncbins[is]*npbins[is]);

    double pbins[7][11]={{0,300,500,600,700,900,1000,1500,3000,5000,30000}, //numu
			 {0,300,500,600,700,900,1000,1500,3000,5000,30000}, //numu
			 {0,300,500,600,700,900,1000,1500,3000,5000,30000}, //numu
			 {0,500,900,1200,2000,10000,10000,10000,10000,10000,10000}, //antinumu
			 {0,500,900,1200,2000,10000,10000,10000,10000,10000,10000}, //antinumu
			 {0,600,1000,1500,2200,10000,10000,10000,10000,10000,10000},//antinumu, numu bg
			 {0,600,1000,1500,2200,10000,10000,10000,10000,10000,10000}};//antinumu, numu bg
    double cbins[7][8]={{-1,0.6,0.8,0.85,0.9,0.94,0.98,1.0}, //numu
			{-1,0.6,0.8,0.85,0.9,0.94,0.98,1.0}, //numu
			{-1,0.6,0.8,0.85,0.9,0.94,0.98,1.0}, //numu
			{-1,0.8,0.92,0.98,1,1,1,1},//antinumu
			{-1,0.8,0.92,0.98,1,1,1,1},//antinumu
			{-1,0.8,0.9,0.97,1,1,1,1},//antinumu, numu bg
			{-1,0.8,0.9,0.97,1,1,1,1}};//antinumu, numu bg
    
  
    // get the input file name
    inputFileName = argv[optind++];

    double ccqe_weight[25];
    for(int i = 0; i < 25; ++i){
        ccqe_weight[i] = 1;
    }

// Uncomment this section if you want to apply weights from the BANFF input spline files
    TFile infile(inputFileName.c_str(),"READ");
    TTree* sample_sum;
    bool hasSampleSum=false;
    if(infile.Get("sample_sum")){
      sample_sum= (TTree*) infile.Get("sample_sum");
      sample_sum->SetBranchAddress("CCQETuningWeight",ccqe_weight);
      hasSampleSum=true;
    }
    if(outfilename==NULL) outfilename = "BANFFCovariance.root";
    TFile outfile(outfilename,"RECREATE");
    

    TH1F* hnNom=new TH1F("hnNom","hnNom",nbins,0,nbins);
    TH1F* hnZero=new TH1F("hnZero","hnZero",nbins,0,nbins);
    TH1F* hnOnlyWZero=new TH1F("hnOnlyW","hnonlyW",nbins,0,nbins);

    //content for each toy
    TH2F* hnvstoy=new TH2F("hnvstoy","hnvstoy",ntoys,0,ntoys,nbins,0,nbins);
    TH2F* hnOnlyVarvstoy=new TH2F("hnOnlyVarvstoy","hnOnlyVarvstoy",ntoys,0,ntoys,nbins,0,nbins);
    
    //"BANFF" way cov matrix /divided by wnom
    TH2F* hnOnlyVarvstoynorm=new TH2F("hnOnlyVarvstoynorm","hnOnlyVarvstoynorm",ntoys,0,ntoys,nbins,0,nbins);
   
      
    TTree *tree=new TTree("tree","tree");
    Float_t selmu_mom=-999;
    Float_t selmu_costheta=-999;
    Float_t niwgW=-999;
    Float_t detW=-999;
    Float_t fluxW=-999;
    Float_t potW=-999;
    Float_t pileupW=-999;
    Int_t   isFGD1NuMuCC=-1;
    Int_t   isFGD1NuMuCC0Pi=-1;
    Int_t   isFGD1NuMuCC1Pi=-1;
    Int_t   isFGD1NuMuCCOther=-1;
    Int_t   isFGD1AntiNuMuCC=-1;
    Int_t   isFGD1AntiNuMuCCQE=-1;
    Int_t   isFGD1AntiNuMuCCnQE=-1;
    Int_t   isFGD1NuMuBkgInAntiNuMuModeCC=-1;
    Int_t   isFGD1NuMuBkgInAntiNuMuModeCCQE=-1;
    Int_t   isFGD1NuMuBkgInAntiNuMuModeCCnQE=-1;

    tree->Branch("selmu_mom",&selmu_mom,"selmu_mom/F");
    tree->Branch("selmu_costheta",&selmu_costheta,"selmu_costheta/F");
    tree->Branch("niwgW",&niwgW,"niwgW/F");
    tree->Branch("detW",&detW,"detW/F");
    tree->Branch("fluxW",&fluxW,"fluxW/F");
    tree->Branch("potW",&potW,"potW/F");
    tree->Branch("pileupW",&pileupW,"pileupW/F");

    tree->Branch("isFGD1NuMuCC",     &isFGD1NuMuCC,     "isFGD1NuMuCC/I");
    tree->Branch("isFGD1NuMuCC0Pi",  &isFGD1NuMuCC0Pi,  "isFGD1NuMuCC0Pi/I");
    tree->Branch("isFGD1NuMuCC1Pi",  &isFGD1NuMuCC1Pi,  "isFGD1NuMuCC1Pi/I");
    tree->Branch("isFGD1NuMuCCOther",&isFGD1NuMuCCOther,"isFGD1NuMuCCOther/I");

    tree->Branch("isFGD1AntiNuMuCC",     &isFGD1AntiNuMuCC,     "isFGD1AntiNuMuCC/I");
    tree->Branch("isFGD1AntiNuMuCCQE",   &isFGD1AntiNuMuCCQE,   "isFGD1AntiNuMuCCQE/I");
    tree->Branch("isFGD1AntiNuMuCCnQE",  &isFGD1AntiNuMuCCnQE,  "isFGD1AntiNuMuCCnQE/I");

    tree->Branch("isFGD1NuMuBkgInAntiNuMuModeCC",     &isFGD1NuMuBkgInAntiNuMuModeCC,     "isFGD1NuMuBkgInAntiNuMuModeCC/I");
    tree->Branch("isFGD1NuMuBkgInAntiNuMuModeCCQE",   &isFGD1NuMuBkgInAntiNuMuModeCCQE,   "isFGD1NuMuBkgInAntiNuMuModeCCQE/I");
    tree->Branch("isFGD1NuMuBkgInAntiNuMuModeCCnQE",  &isFGD1NuMuBkgInAntiNuMuModeCCnQE,  "isFGD1NuMuBkgInAntiNuMuModeCCnQE/I");

  

    // This the analysis manager
    AnalysisManager _man;

    // Make and fill the EventSummary even when the selection is not passed.
    if(ND::params().GetParameterI("psycheSteering.Selections.ForceFillEventSummary")) _man.sel().SetForceFillEventSummary(true);
    // Parameters to control the systematics
    bool applyVariationSystematics = (bool)ND::params().GetParameterI("psycheSteering.Systematics.ApplyVariationSystematics");
    bool applyWeightSystematics    = (bool)ND::params().GetParameterI("psycheSteering.Systematics.ApplyWeightSystematics");
    bool applyFluxWeightSystematic = (bool)ND::params().GetParameterI("psycheSteering.Systematics.ApplyFluxWeightSystematic");

    // Get the initial amount of memory being used
    double initial_mem = getValue();

    // Print the steps for the numuCC selection
    _man.sel().GetSelection("kTrackerNumuCC")->DumpSteps();

    std::vector<SystematicBase*>  allSyst = _man.syst().GetSystematics();
    ToyExperiment toy = (*_man.syst().CreateToyExperiment()); 
    ToyExperiment toy0 = (*_man.syst().CreateToyExperiment()); 
    
    Int_t nsyst=toy.GetNSystematics();
    for(int i=0;i<nsyst;i++){
      if(!allSyst[i])continue;
      if(allSyst[i]->Name()=="kBFieldDist")
	allSyst[i]->SetPDF(SystematicBase::kUniform);
      else
	allSyst[i]->SetPDF(SystematicBase::kGaussian);
    }
    TRandom3 *rand= new TRandom3();
    UInt_t seed= (UInt_t)ND::params().GetParameterI("psycheSteering.Systematics.RandomSeed");
    rand->SetSeed(seed);  
    timeval tim;
    gettimeofday(&tim, NULL);
    double t0=tim.tv_sec+(tim.tv_usec/1000000.0);
    // Initialize clock
    gettimeofday(&tim, NULL);
    double t1=tim.tv_sec+(tim.tv_usec/1000000.0);
    std::vector<double> systpull[1000][20];
    /// Define the throws and generate them 
    for (UInt_t itoy= 0; itoy<ntoys;itoy++){  
      for(int isyst = 0; isyst < nsyst; isyst++){
	for (UInt_t ipar = 0;ipar<toy.GetNParameters(isyst);ipar++){
	  Float_t var;
	  if(allSyst[isyst]->PDF()== SystematicBase::kUniform)
	    var=rand->Uniform(0.,1.);
	  else
	    var=rand->Gaus(0.,1.);
	  systpull[itoy][isyst].push_back(var);
	}
      }
    }



    std::cout<<" nsyst     "<<toy.GetNSystematics()<<std::endl;
    std::cout<<" nsyst var "<<toy.GetNVariationSystematics()<<std::endl;
    std::cout<<" nsyst wei "<<toy.GetNWeightSystematics()<<std::endl;

    double POT_weight = pot_ratio;


    // Preload nmax events from the file
    if (!_man.ReadEvents(inputFileName, nmax)) return 0;

    if(nmax < 0) nmax = _man.GetEntries();

    _man.SetNEventsToProcess(nmax);

    // Now get the amnount of memory in use after loading the events
    double full_mem = getValue();

    std::cout << "The program uses " << full_mem-initial_mem << "Kb of memory to read in the flat tree" << std::endl;
    
    float* mom = new float[nmax*ntoys+1];
    float* weight = new float[nmax*ntoys+1];
    for(int j = 0; j<nmax; ++j){
        for(unsigned int k = 0; k < ntoys;  ++k){
            weight[j*ntoys + k] = -99999;
            mom[j*ntoys + k] = -1;
        }
    }


#ifdef MULTITHREAD
    int nCores = atoi(getenv("OMP_NUM_THREADS"));
#define EPT nmax/nCores
#pragma omp parallel for num_threads(nCores) schedule(dynamic,EPT)
#endif
    for(int j = 0; j < nmax; ++j){
        // 1. ----------------------------------------------------------
        // Get the next event in the Experiment
      if(hasSampleSum)
	sample_sum->GetEvent(j);

      //get nominal
      AnaEventB* event = _man.GetEvent(j);
      _man.ProcessEvent(*event);//Nominal
      Float_t pileupW=event->Weight;
      Float_t fluxWeight=1;
      if (_man.GetFluxWeighting() && event->Summary){
	if (static_cast<AnaEventSummaryB*>(event->Summary)->TrueVertex[static_cast<AnaEventSummaryB*>(event->Summary)->EventSample])
	  fluxWeight=_man.GetFluxWeighting()->GetWeight(static_cast<AnaEventSummaryB*>(event->Summary)->TrueVertex[static_cast<AnaEventSummaryB*>(event->Summary)->EventSample], anaUtils::GetRunPeriod(event->EventInfo.Run));
      }
      

      int isampleNom=-1;
      if(static_cast<AnaEventSummaryB*>(event->Summary)->EventSample == SampleId::kFGD1NuMuCC0Pi)        isampleNom=0;	
      else if(static_cast<AnaEventSummaryB*>(event->Summary)->EventSample == SampleId::kFGD1NuMuCC1Pi)   isampleNom=1;	
      else if(static_cast<AnaEventSummaryB*>(event->Summary)->EventSample == SampleId::kFGD1NuMuCCOther) isampleNom=2;	
      else if(static_cast<AnaEventSummaryB*>(event->Summary)->EventSample == SampleId::kFGD1AntiNuMuCCQE)    isampleNom=3;	
      else if(static_cast<AnaEventSummaryB*>(event->Summary)->EventSample == SampleId::kFGD1AntiNuMuCCnQE)   isampleNom=4;	
      else if(static_cast<AnaEventSummaryB*>(event->Summary)->EventSample == SampleId::kFGD1NuMuBkgInAntiNuModeCCQE) isampleNom=5;
      else if(static_cast<AnaEventSummaryB*>(event->Summary)->EventSample == SampleId::kFGD1NuMuBkgInAntiNuModeCCnQE)isampleNom=6;

      if(static_cast<AnaEventSummaryB*>(event->Summary)->EventSample != SampleId::kUnassigned){
	int shift=0;
	for(int is=0;is<7;is++){
	  if(is>0)shift+=npbins[is-1]*ncbins[is-1];
	  int bb=0;
	  if(is==isampleNom){
	    for(int ic=0;ic<ncbins[is];ic++){
	      for(int ip=0;ip<npbins[is];ip++){
		selmu_mom=static_cast<AnaEventSummaryB*>(event->Summary)->LeptonCandidate[static_cast<AnaEventSummaryB*>(event->Summary)->EventSample]->Momentum;
		selmu_costheta=static_cast<AnaEventSummaryB*>(event->Summary)->LeptonCandidate[static_cast<AnaEventSummaryB*>(event->Summary)->EventSample]->DirectionStart[2];
		if((selmu_mom>=pbins[is][ip] && selmu_mom<pbins[is][ip+1]) && (selmu_costheta>=cbins[is][ic] && selmu_costheta<cbins[is][ic+1])){
		  hnNom->Fill(bb+shift,fluxWeight*POT_weight*pileupW);

		}
		bb++;
	      }
	    }
	  }
	}
      }
      for (UInt_t jsyst = 0; jsyst<toy.GetNSystematics();jsyst++){
	for (UInt_t ipar = 0;ipar<toy.GetNParameters(jsyst);ipar++){
	  toy.SetVariation(jsyst,ipar,0);
	}
      }
      Float_t detectorWeight0;
      Float_t fluxWeight0;
      _man.ProcessEvent(toy,*event,detectorWeight0,fluxWeight0);//result with 0 variation
      int isampleZero=-1;
      if(static_cast<AnaEventSummaryB*>(event->Summary)->EventSample == SampleId::kFGD1NuMuCC0Pi)        isampleZero=0;	
      else if(static_cast<AnaEventSummaryB*>(event->Summary)->EventSample == SampleId::kFGD1NuMuCC1Pi)   isampleZero=1;	
      else if(static_cast<AnaEventSummaryB*>(event->Summary)->EventSample == SampleId::kFGD1NuMuCCOther) isampleZero=2;	
      else if(static_cast<AnaEventSummaryB*>(event->Summary)->EventSample == SampleId::kFGD1AntiNuMuCCQE)    isampleZero=3;	
      else if(static_cast<AnaEventSummaryB*>(event->Summary)->EventSample == SampleId::kFGD1AntiNuMuCCnQE)   isampleZero=4;	
      else if(static_cast<AnaEventSummaryB*>(event->Summary)->EventSample == SampleId::kFGD1NuMuBkgInAntiNuModeCCQE) isampleZero=5;
      else if(static_cast<AnaEventSummaryB*>(event->Summary)->EventSample == SampleId::kFGD1NuMuBkgInAntiNuModeCCnQE)isampleZero=6;

      if(static_cast<AnaEventSummaryB*>(event->Summary)->EventSample != SampleId::kUnassigned){
	int shift=0;
	for(int is=0;is<7;is++){
	  if(is>0)shift+=npbins[is-1]*ncbins[is-1];
	  int bb=0;	 
	  for(int ic=0;ic<ncbins[is];ic++){
	    for(int ip=0;ip<npbins[is];ip++){
	      double selmu_mom0=static_cast<AnaEventSummaryB*>(event->Summary)->LeptonCandidate[static_cast<AnaEventSummaryB*>(event->Summary)->EventSample]->Momentum;
	      double selmu_costheta0=static_cast<AnaEventSummaryB*>(event->Summary)->LeptonCandidate[static_cast<AnaEventSummaryB*>(event->Summary)->EventSample]->DirectionStart[2];
	    
	      if(is==isampleZero && detectorWeight0<100){	      
		if((selmu_mom0>=pbins[is][ip] && selmu_mom0<pbins[is][ip+1]) && (selmu_costheta0>=cbins[is][ic] && selmu_costheta0<cbins[is][ic+1])){
		  hnZero->Fill(bb+shift,fluxWeight0*POT_weight*pileupW*detectorWeight0);
		}
	      }
	      
	      if(is==isampleNom && detectorWeight0<100){
		if((selmu_mom>=pbins[is][ip] && selmu_mom<pbins[is][ip+1]) && (selmu_costheta>=cbins[is][ic] && selmu_costheta<cbins[is][ic+1])){
		  hnOnlyWZero->Fill(bb+shift,fluxWeight0*POT_weight*pileupW*detectorWeight0);
		}
	      }
	      bb++;
	    }
	  }
	}
      }
      /// Loop over toy experiments
      for (UInt_t itoy= 0; itoy<ntoys;itoy++){
	// 2. ====================================================================
	// Configure the toy experiment. variation for each of the systematics. Just one example of 1sigma for everybody
	for (UInt_t jsyst = 0; jsyst<toy.GetNSystematics();jsyst++){
	  for (UInt_t ipar = 0;ipar<toy.GetNParameters(jsyst);ipar++){
	    toy.SetVariation(jsyst,ipar,systpull[itoy][jsyst][ipar]);
	  }
	}
	Float_t detectorWeight;
	Float_t fluxWeight;
	_man.ProcessEvent(toy,*event,detectorWeight,fluxWeight);//result with 0 variation
	int isample=-1;
	if(static_cast<AnaEventSummaryB*>(event->Summary)->EventSample == SampleId::kFGD1NuMuCC0Pi)        isample=0;	
	else if(static_cast<AnaEventSummaryB*>(event->Summary)->EventSample == SampleId::kFGD1NuMuCC1Pi)   isample=1;	
	else if(static_cast<AnaEventSummaryB*>(event->Summary)->EventSample == SampleId::kFGD1NuMuCCOther) isample=2;	
	else if(static_cast<AnaEventSummaryB*>(event->Summary)->EventSample == SampleId::kFGD1AntiNuMuCCQE)    isample=3;	
	else if(static_cast<AnaEventSummaryB*>(event->Summary)->EventSample == SampleId::kFGD1AntiNuMuCCnQE)   isample=4;	
	else if(static_cast<AnaEventSummaryB*>(event->Summary)->EventSample == SampleId::kFGD1NuMuBkgInAntiNuModeCCQE) isample=5;
	else if(static_cast<AnaEventSummaryB*>(event->Summary)->EventSample == SampleId::kFGD1NuMuBkgInAntiNuModeCCnQE)isample=6;

	if(static_cast<AnaEventSummaryB*>(event->Summary)->EventSample != SampleId::kUnassigned){
	  int shift=0;
	  for(int is=0;is<7;is++){
	    if(is>0)shift+=npbins[is-1]*ncbins[is-1];
	    int bb=0;
	    for(int ic=0;ic<ncbins[is];ic++){
	      for(int ip=0;ip<npbins[is];ip++){
		selmu_mom=static_cast<AnaEventSummaryB*>(event->Summary)->LeptonCandidate[static_cast<AnaEventSummaryB*>(event->Summary)->EventSample]->Momentum;
		selmu_costheta=static_cast<AnaEventSummaryB*>(event->Summary)->LeptonCandidate[static_cast<AnaEventSummaryB*>(event->Summary)->EventSample]->DirectionStart[2];
		if((selmu_mom>=pbins[is][ip] && selmu_mom<pbins[is][ip+1]) && (selmu_costheta>=cbins[is][ic] && selmu_costheta<cbins[is][ic+1])){
		  if(is==isample && detectorWeight<100){
		    hnvstoy->Fill(itoy,bb+shift,fluxWeight*POT_weight*pileupW*detectorWeight);
		    hnOnlyVarvstoy->Fill(itoy,bb+shift,fluxWeight*POT_weight*pileupW);
		    hnOnlyVarvstoynorm->Fill(itoy,bb+shift,fluxWeight*POT_weight*pileupW*detectorWeight/detectorWeight0);
		  }
		 
		  
		}
		bb++;
	      }
	    }
	  }
	}
      }
      tree->Fill();
    }

    outfile.cd();
    tree->Write(); 
    TH2F *hcov_BANFFstyle=new TH2F("hcov_BANFFstyle","BANFF",nbins,0,nbins,nbins,0,nbins);
    TH2F *hcov_st=new TH2F("hcov_st","BANFF",nbins,0,nbins,nbins,0,nbins);
    TH2F *herr_st=new TH2F("herr_st","BANFF",nbins,0,nbins,nbins,0,nbins);
    TH2F *hcov_w=new TH2F("hcov_w","BANFF",nbins,0,nbins,nbins,0,nbins);
    TH2F *herr_w=new TH2F("herr_w","BANFF",nbins,0,nbins,nbins,0,nbins);
    TH2F *herr_BANFFstyle=new TH2F("herr_BANFFstyle","BANFF",nbins,0,nbins,nbins,0,nbins);
    TH1F *prior_BANFFstyle=new TH1F("prior_BANFFstyle","BANFF",nbins,0,nbins);
    MakeCovarianceBANFFStyle((const int)nbins,ntoys,hcov_BANFFstyle,hcov_st,hcov_w,prior_BANFFstyle, hnOnlyVarvstoynorm, hnOnlyWZero,hnNom);
    MakeErrorMatrix((const int) nbins,hcov_BANFFstyle,herr_BANFFstyle);
    MakeErrorMatrix((const int) nbins,hcov_st,herr_st);
    MakeErrorMatrix((const int) nbins,hcov_w,herr_w);
    hcov_BANFFstyle->Write();
    hcov_st->Write();
    hcov_w->Write();
    herr_st->Write();
    herr_w->Write();
    herr_BANFFstyle->Write();
    prior_BANFFstyle->Write();
   
    TH2F *hcov_NUMUstyle=new TH2F("hcov_NUMUstyle","NUMU",nbins,0,nbins,nbins,0,nbins);
    TH2F *hcov_NUMUstyleNew=new TH2F("hcov_NUMUstyleNew","NUMU",nbins,0,nbins,nbins,0,nbins);
    TH2F *herr_NUMUstyle=new TH2F("herr_NUMUstyle","NUMU",nbins,0,nbins,nbins,0,nbins);
    TH2F *herr_NUMUstyleNew=new TH2F("herr_NUMUstyleNew","NUMU",nbins,0,nbins,nbins,0,nbins);
   
    MakeCovarianceNUMUStyle((const int) nbins,ntoys,hcov_NUMUstyle,hnOnlyVarvstoy,hnvstoy,hnNom);
    MakeCovarianceNUMUStyle((const int) nbins,ntoys,hcov_NUMUstyleNew,hnOnlyVarvstoynorm,hnvstoy,hnNom);
    MakeErrorMatrix((const int) nbins,hcov_NUMUstyle,herr_NUMUstyle);
    MakeErrorMatrix((const int) nbins,hcov_NUMUstyleNew,herr_NUMUstyleNew);
    TH2F *hcov_ratio=new TH2F("hcov_ratio","BANFF",nbins,0,nbins,nbins,0,nbins);
    TH2F *hcov_ratioNew=new TH2F("hcov_ratioNew","BANFF",nbins,0,nbins,nbins,0,nbins);
    TH2F *hcov_ratioNUMU=new TH2F("hcov_ratioNUMU","NUMU",nbins,0,nbins,nbins,0,nbins);
    DivideMatrix((const int) nbins,hcov_ratio,hcov_BANFFstyle,hcov_NUMUstyle);
    DivideMatrix((const int) nbins,hcov_ratioNUMU,hcov_NUMUstyleNew,hcov_NUMUstyle);
    DivideMatrix((const int) nbins,hcov_ratioNew,hcov_BANFFstyle,hcov_NUMUstyleNew);
    hcov_ratio->Write();
    hcov_ratioNew->Write();
    hcov_ratioNUMU->Write();
    herr_NUMUstyle->Write();
    herr_NUMUstyleNew->Write();
    hcov_NUMUstyle->Write();
    hcov_NUMUstyleNew->Write();
    hnOnlyVarvstoy->Write();
    hnvstoy->Write();
    hnNom->Write();
    hnZero->Write();
    hnOnlyVarvstoynorm->Write();
    hnOnlyWZero->Write();
    outfile.Close();

    double final_mem = getValue();

    gettimeofday(&tim, NULL);
    double t2=tim.tv_sec+(tim.tv_usec/1000000.0);

    std::cout << "Time to preload events = " << t1-t0 << std::endl;
    std::cout << "Time to process events = " << t2-t1 << std::endl;
    std::cout << "The program used " << (final_mem-initial_mem)/1024 << "Mb of memory in total" << std::endl;

    //  ProfilerStop();
    _man.sel().PrintStatistics();
}
