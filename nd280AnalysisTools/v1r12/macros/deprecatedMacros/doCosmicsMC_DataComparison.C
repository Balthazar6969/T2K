#include "iostream"
#include "vector"




//______________________________________________________________________________________
// 
//
//  -----------------  Version 2 --------------------
//
//    To execute open in the macros folder of oaAnalysis, where the script is saved,
//    a root terminal and type: .x doCosmicsMC_DataComparison.C
//
//
//  A macro to read in a data file and an MC file, with their locations 
//  stored in a text file. The macro then produces a number of plots ( energy and 
//  direction) in order to compare Data-MC,
//  where MC here refers to the Corsika MC files that are available at 
//  http://pprc.qmul.ac.uk/~mageorge/T2Knd280CosmicFlux/.  Currently the macro
//  is designed for use with the DSECal cosmics data but will soon be edited so that
//  it can be used with any subdetector.
//
//  Eventually the script will do the full analysis including KS test, dE/dx and per
//  bar dE/dx.  As well as this the module will become faster and much more user friendly.
//  In light of this and all the work currently being done 
//  
//  PLEASE REFRAIN FROM USING THIS CODE UNTIL THIS MESSAGE IS REMOVED!!
//
//
// 
//  M. George, Queen Mary University - September 2009
//  G. Davies, Lancaster Univeristy - September 2009
//
void doCosmicsMC_DataComparison() {

// Load some important libraries.

  gSystem->AddIncludePath("-I$OAANALYSISROOT/src");
  gSystem->AddIncludePath("-I.");
  
  gSystem->Load("libPhysics");
  gSystem->Load("libGeom");
  gSystem->Load("libTree");
  gSystem->Load("libMatrix");
  gSystem->Load("liboaEvent.so");
  gSystem->Load("liboaAnalysis.so");
  
  gStyle->SetPalette(1);

  // -------------------------------------------------------------------------------------
  // Create histograms.

//Directional histograms
   TH1F *ReconTrackShowerTheta = new TH1F("ReconTrackShowerTheta","The Reconstucted TrackLike Shower Theta angle",500,0,100);
   TH1F *mcReconTrackShowerTheta = new TH1F("mcReconTrackShowerTheta","The Reconstucted MC TrackLike Shower Theta angle",500,0,100);
   ReconTrackShowerTheta->SetXTitle("Reconstructed 'Tracklike Shower' Theta (Degrees) for MC(red) and Data(black)");
   ReconTrackShowerTheta->SetYTitle("No of Entries for MC(red) and Data(black)");
   mcReconTrackShowerTheta->SetXTitle("Reconstructed 'Tracklike Shower' Theta (Degrees) for MC(red) and Data(black)");
   mcReconTrackShowerTheta->SetYTitle("No of Entries for MC(red) and Data(black)");
   mcReconTrackShowerTheta->SetTitle("Reconstructed 'Tracklike Shower' Theta (Degrees) for MC(red) and Data(black)");
   ReconTrackShowerTheta->SetTitle("Reconstructed 'Tracklike Shower' Theta (Degrees) for MC(red) and Data(black)");

//Energy histograms
   TH1F *ReconTrackEDeposit = new TH1F("ReconTrackEDeposit","The Reconstucted Track Energy Deposited",500,0,30);
   ReconTrackEDeposit->SetXTitle("Reconstructed Track Energy Deposited - MC(red) data(black)");
   ReconTrackEDeposit->SetYTitle("Number of Entries");
   TH1F *mcReconTrackEDeposit = new TH1F("mcReconTrackEDeposit","The Reconstucted MC Track Energy Deposited",500,0,30);
   mcReconTrackEDeposit->SetXTitle("Reconstructed Track Energy Deposited - MC(red) data(black)");
   mcReconTrackEDeposit->SetYTitle("Number of entries");
   mcReconTrackEDeposit->SetTitle("Reconstructed Track Energy Deposited For MC(red) and data(black)");
   ReconTrackEDeposit->SetTitle("Reconstructed Track Energy Deposited For MC(red) and data(black)");

//E vs Theta MC histograms
   TH2F *mcE_theta = new TH2F("mcE_theta","E vs Theta Using MC For The Detector as a Whole",500,0,100,500,0,30);
   mcE_theta->SetXTitle("Reconstructed 'Tracklike Shower' Theta (Degrees)");
   mcE_theta->SetYTitle("Reconstructed Track Energy Deposited");
   mcE_theta->SetTitle("E vs Theta Using MC For The Detector as a Whole");

//E vs Theta data histograms
   TH2F *dataE_theta = new TH2F("datadE_theta","E vs Theta Using Data For The Detector as a Whole",500,0,100,500,0,30);
   dataE_theta->SetXTitle("Reconstructed 'Tracklike Shower' Theta (Degrees)");
   dataE_theta->SetYTitle("Reconstructed Track Energy Deposited");
   dataE_theta->SetTitle("E vs theta Using Data For The Detector as a Whole");

//Hit Map (BarHit v LayerHit) for bars in X-orientation data histograms
   TH2F *dataHitMapX = new TH2F("dataHitMapX","",51,0,50,35,0,35);
   dataHitMapX->SetXTitle("Number Of Hits per Layer in Data");
   dataHitMapX->SetYTitle("Number Of Hits per Bar in Data");
   dataHitMapX->SetTitle("Hit Map of Layers Vs Bars in the X-Orientation for Data");

//Hit Map (BarHit v LayerHit) for bars in X-orientation MC histograms  
   TH2F *mcHitMapX = new TH2F("mcHitMapX","",51,0,50,35,0,35);
   mcHitMapX->SetXTitle("Number Of Hits per Layer in MC");
   mcHitMapX->SetYTitle("Number Of Hits per Bar in MC");
   mcHitMapX->SetTitle("Hit Map of Layers Vs Bars in the X-Orientation for MC");

//Hit Map (BarHit v LayerHit) for bars in Y-orientation data histograms
   TH2F *dataHitMapY = new TH2F("dataHitMapY","",35,0,35,51,0,50);
   dataHitMapY->SetXTitle("Number Of Hits per Layer");
   dataHitMapY->SetYTitle("Number Of Hits per Bar");
   dataHitMapY->SetTitle("Hit Map of Layers Vs Bars in the Y-Orientation for Data");

//Hit Map (BarHit v LayerHit) for bars in Y-orientation MC histograms  
   TH2F *mcHitMapY = new TH2F("mcHitMapY","",35,0,35,51,0,50);
   mcHitMapY->SetXTitle("Number Of Hits per Layer in MC");
   mcHitMapY->SetYTitle("Number Of Hits per Bar in MC");
   mcHitMapY->SetTitle("MC Hit Map of Layers Vs Bars in the Y-Orientation");


// histograms that are filled and may be used but are not currently.
   TH1F *CosTrackTheta = new TH1F("CosTrackTheta","Cosine of the track theta angle",100,0,1);
   CosTrackTheta->SetXTitle("Cosmine of the Track Theata Value"); 
   TH1F *mcCosTrackTheta = new TH1F("mcCosTrackTheta","Cosine of the MC track theta angle",100,0,1);
   mcCosTrackTheta->SetXTitle("Cosine of the MC Track Theta Value"); 
   TH1F *CosShowerTheta = new TH1F("CosShowerTheta","Cosine of the shower theta angle",100,0,1);
   CosShowerTheta->SetXTitle("Cosmine of the Shower Theta Value"); 
   TH1F *mcCosShowerTheta = new TH1F("CosShowerTheta","Cosine of the MC shower theta angle",100,0,1);
   mcCosShowerTheta->SetXTitle("Cosine of the MC Shower Theta Value");
   TH1F *mcCosTrackTheta = new TH1F("mcCosTrackTheta","Cosine of the MC track theta angle",100,0,1);
   mcCosTrackTheta->SetXTitle("Cosine of the MC Track Theata Value"); 
   TH1F *ReconShowerEDeposit = new TH1F("ReconShowerEDeposit","The Reconstucted Shower Energy Deposited",10000,0,10000);
   ReconShowerEDeposit->SetXTitle("Reconstructed Shower Energy Deposited");
   TH1F *ReconTrackTheta = new TH1F("ReconTrackTheta","The Reconstucted Track Theta angle",90,0,90);
   ReconTrackTheta->SetXTitle("Reconstructed Track Theta (Degrees)");
   TH1F *mcReconTrackTheta = new TH1F("mcReconTrackTheta","The Reconstucted MC Track Theta angle",90,0,90);
   mcReconTrackTheta->SetXTitle("Reconstructed MC Track Theta (Degrees)");
   TH1F *ReconTrackShowerTheta2 = new TH1F("ReconTrackShowerTheta","The Reconstucted TrackLike Shower Theta angle",90,0,90);



// Adding MC file(s)
	TChain mcReconChain = TChain("ReconDir/TrackerECal");

	TString mcfileName("input_MC_files.txt");
		if(mcfileName){
			bool MCfile = true;
		}
  	ifstream InputFileMC(mcfileName.Data(), ios::in);
  
  		if (!InputFileMC) {
   	 		cerr << "Cannot open input file '" << mcfileName.Data() << "'. Exit!" << endl;
   	 		return;
  		}
  
  	string InputStringMC;

  		while (InputFileMC >> InputStringMC) {
    			std::cout << "Adding file: " << InputStringMC << std::endl;
    			mcReconChain.AddFile(InputStringMC.c_str());
  	
  		}


 


// Adding data file(s)
	TChain dataReconChain = TChain("ReconDir/TrackerECal");

	TString datafileName("input_data_files.txt");
  	ifstream inputFileData(datafileName.Data(), ios::in);
  
  		if (!inputFileData) {
    			cerr << "Cannot open input Data file '" << datafileName.Data() << "'. Exit!" << endl;
    			return;
  		}
  
  	string inputStringData;

  		while (inputFileData >> inputStringData) {
    			std::cout << "Adding file: " << inputStringData << std::endl;
    			dataReconChain.AddFile(inputStringData.c_str());
  	
  		}


	 _____________________________________________________________________________________
  // Prepare to read trees.

	if(MCfile){
   // Set the branches for the tracker recon information.
  		Int_t fNReconObject;
  		TClonesArray *fReconObject = new TClonesArray("ND::TTrackerECALReconModule::TECALReconObject", 20);
  		mcReconChain.SetBranchAddress("NReconObject", &fNReconObject);
  		mcReconChain.SetBranchAddress("ReconObject", &fReconObject);
  		mcReconChain.Print("toponly");
	}
	else {
  		dataReconChain.SetBranchAddress("NReconObject", &fNReconObject);
  		dataReconChain.SetBranchAddress("ReconObject", &fReconObject);
  		dataReconChain.Print("toponly");
	}
  
  int MCtotalEvts = mcReconChain.GetEntries();
  int totalEvts = dataReconChain.GetEntries();

  double MCtrueThetaDeg = 0;
  double MCfirstTrueThetaDeg = 0;

  double MCtThetaDeg = 0;
  double MCtsThetaDeg = 0;

  double trueThetaDeg = 0;
  double firstTrueThetaDeg = 0;

  double tThetaDeg = 0;
  double tsThetaDeg = 0;


  for(int MCevtNum = 0; MCevtNum < MCtotalEvts; MCevtNum++){
    
    mcReconChain.GetEntry(MCevtNum);
    	if(MCevtNum%500 == 0){
      	std::cout <<  "Events Processed " << MCevtNum << std::endl;
    	}


    	for(int i =0; i < fNReconObject ;i++){    
      		ND::TTrackerECALReconModule::TECALReconObject *mcObject 
	= (ND::TTrackerECALReconModule::TECALReconObject*)(*fReconObject)[i];
      
     		if(mcObject->IsTrackLike){

        		TVector3 tDir = mcObject->Track.Direction;
        
        		double MCtEDep = mcObject->Track.EDeposit;
        		double MCtsEDep = mcObject->Shower.EDeposit;
   			
			double MCBarHit = mcObject->BarHit;
			double MCLayerHit = mcObject->LayerHit;

      			// Finding the track direction theta
      			double MCtS = sqrt( (tDir.X() * tDir.X()) +  (tDir.Y() * tDir.Y()) );
      			//double tS = sqrt( (tDir.Y() * tDir.Y()) +  (tDir.X() * tDir.X()) );
      			double tThetaRad;

      			mcReconTrackEDeposit->Fill(MCtEDep);

      			// Finding the tracklike shower direction theta
      			TVector3 MCtsDir = mcObject->Shower.Direction;
      			double MCtsS = sqrt( (MCtsDir.X() * MCtsDir.X()) +  (MCtsDir.Y() * MCtsDir.Y()) );
      			double MCtsThetaRad;
      				if(MCtsDir.Z()){
					MCtsThetaRad = atan(MCtsS/MCtsDir.Z());
      				}
      			MCtsThetaDeg = ( ( MCtsThetaRad / 3.14 ) * 180 );
      
      			mcReconTrackShowerTheta->Fill(MCtsThetaDeg);
                        mcE_theta->Fill(MCtsThetaDeg,MCtEDep);
      			mcCosShowerTheta->Fill((cos(MCtsThetaDeg)*cos(MCtsThetaDeg)));

			if(mcObject->IsXHit){
		        mcHitMapX->Fill(MCLayerHit,MCBarHit);
			}
			if(mcObject->IsYHit){
			mcHitMapY->Fill(MCLayerHit,MCBarHit);
			}
    		}
  	}

  }
 

  for(int evtNum = 0; evtNum < totalEvts; evtNum++){
    
    dataReconChain.GetEntry(evtNum);
    	if(evtNum%500 == 0){
      	std::cout <<  "Events Processed " << evtNum << std::endl;
    	}

    	
	for(int i =0; i < fNReconObject ;i++){    
      	ND::TTrackerECALReconModule::TECALReconObject *object 
	= (ND::TTrackerECALReconModule::TECALReconObject*)(*fReconObject)[i];
      
     		if(object->IsTrackLike){

			TVector3 tDir = object->Track.Direction;
        
        		double tEDep = object->Track.EDeposit;
        		double tsEDep = object->Shower.EDeposit;

			double dataBarHit = object->BarHit;
			double dataLayerHit = object->LayerHit;

      			// Finding the track direction theta
      			double tS = sqrt( (tDir.X() * tDir.X()) +  (tDir.Y() * tDir.Y()) );
      			//double tS = sqrt( (tDir.Y() * tDir.Y()) +  (tDir.X() * tDir.X()) );
      			double tThetaRad;
    
      			ReconTrackEDeposit->Fill(tEDep);
			//      ReconShowerEDeposit->Fill(tsEDep);

      			// Finding the tracklike shower direction theta
      			TVector3 tsDir = object->Shower.Direction;
      			double tsS = sqrt( (tsDir.X() * tsDir.X()) +  (tsDir.Y() * tsDir.Y()) );
      			double tsThetaRad;
      				if(tsDir.Z()){
				tsThetaRad = atan(tsS/tsDir.Z());
      				}
      			tsThetaDeg = ( ( tsThetaRad / 3.14 ) * 180 );
      
      			ReconTrackShowerTheta->Fill(tsThetaDeg);
			dataE_theta->Fill(tsThetaDeg,tEDep);
      			CosShowerTheta->Fill((cos(tsThetaDeg)*cos(tsThetaDeg)));
			
			if(object->IsXHit){
		        dataHitMapX->Fill(dataLayerHit,dataBarHit);
			}
			if(object->IsYHit){
			dataHitMapY->Fill(dataLayerHit,dataBarHit);
			}
    		}
    
	}
  }

// Normalising histograms

     double mcDirEntries = mcReconTrackShowerTheta->GetEntries();
     double dataDirEntries = ReconTrackShowerTheta->GetEntries();

     std::cout << mcDirEntries << " " << dataDirEntries << std::endl;
     
     double norm = mcDirEntries/dataDirEntries;
     std::cout << norm << std::endl;

  // -----------------------------------------------------------------------------------------
  // Plot histograms and graphs.

  TCanvas *MC_DataComparisonCanvas = (TCanvas *)gROOT->GetListOfCanvases()->FindObject("MC_DataComparisonCanvas");  
  if (!MC_DataComparisonCanvas)
     MC_DataComparisonCanvas = new TCanvas("MC_DataComparisonCanvas","MC_DataComparisonCanvas",800,1000);
   else
     MC_DataComparisonCanvas->Clear();

     MC_DataComparisonCanvas->cd();


  MC_DataComparisonCanvas->SetFillColor(10);
  MC_DataComparisonCanvas->Divide(2,4);


   //Comparison histograms of mc and data direction not currently using solid angle      
   MC_DataComparisonCanvas->cd(1);
   mcReconTrackShowerTheta->SetLineColor(2);
   mcReconTrackShowerTheta->Draw();
   ReconTrackShowerTheta->Scale(norm);
   //ReconTrackShowerTheta->SetMaximum(250000.0);
   ReconTrackShowerTheta->Draw("same");

   //Comparison histograms of mc and data energy    
   MC_DataComparisonCanvas->cd(2);
   mcReconTrackEDeposit->SetLineColor(2);
   mcReconTrackEDeposit->Draw();
   ReconTrackEDeposit->Scale(norm);
   ReconTrackEDeposit->SetMaximum(2000.0);
   ReconTrackEDeposit->Draw("same");

   //E vs theta histogram for mc
   MC_DataComparisonCanvas->cd(3);
   mcE_theta->Draw("colz");

   //E vs Theta histogram for data
   MC_DataComparisonCanvas->cd(4);
   dataE_theta->Draw("colz");

   //Hit Map of X Hits histogram for mc
   MC_DataComparisonCanvas->cd(5);
   mcHitMapX->Draw("colz");

   //Hit Map of X Hits histogram for data
   MC_DataComparisonCanvas->cd(6);
   dataHitMapX->Draw("colz");
 
   //Hit Map of Y Hits histogram for mc
   MC_DataComparisonCanvas->cd(7);
   mcHitMapY->Draw("colz");

   //Hit Map of Y Hits histogram for data
   MC_DataComparisonCanvas->cd(8);
   dataHitMapY->Draw("colz");


   // Currently the following is a work in progress and doesnt work, watch this space!


     TH1F *KSValues1 = new TH1F("KSValues1","KS values1",10,0,1.0);

     ReconTrackShowerTheta->Scale(norm);
     ReconTrackEDeposit->Scale(norm);
     double ks1 = mcReconTrackShowerTheta->KolmogorovTest(ReconTrackShowerTheta);
     double ks2 = mcReconTrackEDeposit->KolmogorovTest(ReconTrackEDeposit);

     std::cout <<  "KS Test Value - mc compares well to data if the value >0.05 " << ks1 << std::endl;
     std::cout <<  "KS Test Value - data compares well to mc if  >0.05 " << ks2 << std::endl;


   return; 
}

