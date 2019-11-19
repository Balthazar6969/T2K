
{

  bool isMC = true;

  TCanvas c1;

  c1.Print("numutree_validation.pdf[");        // No actual print, just open file.ps
        
  TH1F h1("h1","h1",10, 0,1000000000000);
  numutree.Project("h1","std_Run","std_Run>0"); 
  if (h1.GetEntries() > 0) isMC = false;

  numutree.Draw("std_TrueSpillInfo","std_BunchNbr==0"); 		 c1.Print("numutree_validation.pdf");
  numutree.Draw("std_Evt","std_BunchNbr==0 & std_Evt>=0"); 		 c1.Print("numutree_validation.pdf");        
  numutree.Draw("std_Run","std_BunchNbr==0"); 		 c1.Print("numutree_validation.pdf");        
  numutree.Draw("std_Subrun","std_BunchNbr==0"); 		 c1.Print("numutree_validation.pdf");        
  numutree.Draw("std_FileRun","std_BunchNbr==0"); 		 c1.Print("numutree_validation.pdf");    
  numutree.Draw("std_FileSubrun","std_BunchNbr==0"); 		 c1.Print("numutree_validation.pdf"); 
  numutree.Draw("std_Spill",     "std_BunchNbr==0 && std_Spill>-1"); 		 c1.Print("numutree_validation.pdf");        
  numutree.Draw("std_SpillPOT",  "std_BunchNbr==0 && std_SpillPOT>0"); 		 c1.Print("numutree_validation.pdf");        
  numutree.Draw("std_SpillPOT:std_Spill","std_BunchNbr==0 && std_SpillPOT>0 && std_Spill>0","prof");  c1.Print("numutree_validation.pdf");        
  numutree.Draw("std_Entry","std_BunchNbr==0"); 		 c1.Print("numutree_validation.pdf");      
  c1.SetLogy(1);
  numutree.Draw("std_NBunches","std_BunchNbr==0"); 		 c1.Print("numutree_validation.pdf");   
  numutree.Draw("std_NValidBunches","std_BunchNbr==0"); 		 c1.Print("numutree_validation.pdf"); 
  c1.SetLogy(0);
  numutree.Draw("std_BunchNbr"); 		 c1.Print("numutree_validation.pdf");   
  numutree.Draw("std_SpillWithTPC1Track","std_BunchNbr==0"); 		 c1.Print("numutree_validation.pdf");

  numutree.Draw("std_NECALTracks","std_BunchNbr==0"); 		 c1.Print("numutree_validation.pdf");     
  numutree.Draw("std_NP0DTracks","std_BunchNbr==0"); 		 c1.Print("numutree_validation.pdf");      
  numutree.Draw("std_NSMRDTracks","std_BunchNbr==0"); 		 c1.Print("numutree_validation.pdf");    
  numutree.Draw("std_NTPCTracks","std_BunchNbr==0"); 		 c1.Print("numutree_validation.pdf");     



  // ------ MC truth variables ----------

  if (isMC){
    numutree.Draw("std_NTrueVertices","std_BunchNbr==0"); 		 c1.Print("numutree_validation.pdf");    
    numutree.Draw("std_TrueReacCode","std_BunchNbr==0"); 		 c1.Print("numutree_validation.pdf");           
    numutree.Draw("std_TrueVertexPos[][0]","std_BunchNbr==0"); 		 c1.Print("numutree_validation.pdf");  
    numutree.Draw("std_TrueVertexPos[][1]","std_BunchNbr==0"); 		 c1.Print("numutree_validation.pdf");  
    numutree.Draw("std_TrueVertexPos[][2]","std_BunchNbr==0"); 		 c1.Print("numutree_validation.pdf");  
    numutree.Draw("std_TrueVertexPos[][3]","std_BunchNbr==0"); 		 c1.Print("numutree_validation.pdf");  
    numutree.Draw("std_TrueNuEnergy","std_BunchNbr==0"); 		 c1.Print("numutree_validation.pdf");   
    numutree.Draw("std_TrueNuDir"); 		 c1.Print("numutree_validation.pdf");      
    numutree.Draw("std_TrueNuPDG"); 		 c1.Print("numutree_validation.pdf");      
    numutree.Draw("std_TrueEnuWeight"); 		 c1.Print("numutree_validation.pdf");  
    numutree.Draw("std_TrueVertexBunch"); 		 c1.Print("numutree_validation.pdf"); 
    numutree.Draw("std_TrueLeptonMom"); 		 c1.Print("numutree_validation.pdf");   
    numutree.Draw("std_TrueProtonMom"); 		 c1.Print("numutree_validation.pdf");   
    numutree.Draw("std_TrueTargetMom"); 		 c1.Print("numutree_validation.pdf");   
    numutree.Draw("std_TrueLeptonDir[][0]"); 		 c1.Print("numutree_validation.pdf");   
    numutree.Draw("std_TrueLeptonDir[][1]"); 		 c1.Print("numutree_validation.pdf");   
    numutree.Draw("std_TrueLeptonDir[][2]"); 		 c1.Print("numutree_validation.pdf");   
    numutree.Draw("std_TrueProtonDir[][0]"); 		 c1.Print("numutree_validation.pdf");   
    numutree.Draw("std_TrueProtonDir[][1]"); 		 c1.Print("numutree_validation.pdf");   
    numutree.Draw("std_TrueProtonDir[][2]"); 		 c1.Print("numutree_validation.pdf");   
    numutree.Draw("std_TrueTargetDir[][0]"); 		 c1.Print("numutree_validation.pdf");   
    numutree.Draw("std_TrueTargetDir[][1]"); 		 c1.Print("numutree_validation.pdf");   
    numutree.Draw("std_TrueTargetDir[][2]"); 		 c1.Print("numutree_validation.pdf");   
    numutree.Draw("std_TrueTargetPDG"); 		 c1.Print("numutree_validation.pdf");   
    numutree.Draw("std_TrueBaryonSize"); 		 c1.Print("numutree_validation.pdf");  
    numutree.Draw("std_TrueQ2"); 		 c1.Print("numutree_validation.pdf");          
    numutree.Draw("std_TrueVertexDet"); 		 c1.Print("numutree_validation.pdf");   
  }
  
  //----------- filled for each bunch (syntax _stdB_) -----------
  numutree.Draw("stdB_BunchWithMaxMomInSpill"); 		 c1.Print("numutree_validation.pdf"); 
  numutree.Draw("stdB_TBunch"); 		 c1.Print("numutree_validation.pdf");   
  numutree.Draw("stdB_Bunch"); 		 c1.Print("numutree_validation.pdf");    
  numutree.Draw("stdB_Vertex[0][0]","abs(stdB_Vertex[0][0])<3000 && stdB_Vertex[0][0]!=0"); 		 c1.Print("numutree_validation.pdf");     
  numutree.Draw("stdB_Vertex[0][1]","abs(stdB_Vertex[0][1])<3000 && stdB_Vertex[0][1]!=0"); 		 c1.Print("numutree_validation.pdf");     
  numutree.Draw("stdB_Vertex[0][2]","abs(stdB_Vertex[0][2])<5000 && stdB_Vertex[0][2]!=0"); 		 c1.Print("numutree_validation.pdf");     
  numutree.Draw("stdB_Vertex[0][3]","abs(stdB_Vertex[0][3])<8000 && stdB_Vertex[0][3]!=0"); 		 c1.Print("numutree_validation.pdf");     
  if (isMC)
    numutree.Draw("stdB_nTrueVerticesPerBunch"); 		 c1.Print("numutree_validation.pdf"); 

  numutree.Draw("stdB_NTracksGlobal"); 		 c1.Print("numutree_validation.pdf"); 
  numutree.Draw("stdB_NTracksWithTPC","stdB_NTracksGlobal!=0"); 		 c1.Print("numutree_validation.pdf");  
  numutree.Draw("stdB_NPosTracksWithTPC","stdB_NTracksGlobal!=0"); 		 c1.Print("numutree_validation.pdf");
  numutree.Draw("stdB_NNegTracksWithTPC","stdB_NTracksGlobal!=0"); 		 c1.Print("numutree_validation.pdf");
  numutree.Draw("stdB_NTpc1Tracks","stdB_NTracksGlobal!=0"); 		 c1.Print("numutree_validation.pdf");  
  numutree.Draw("stdB_NTpc2Tracks","stdB_NTracksGlobal!=0"); 		 c1.Print("numutree_validation.pdf");  
  numutree.Draw("stdB_NTpc3Tracks","stdB_NTracksGlobal!=0"); 		 c1.Print("numutree_validation.pdf");  

  
  c1.SetLogx(1);
  numutree.Draw("stdB_trDetectors"); 		 c1.Print("numutree_validation.pdf");  
  c1.SetLogx(0);


  TH1F h("h","stdB_trDetUsed",19,1,20);
  numutree.Project("h","(stdB_trDetUsed[][1]==1)*2","stdB_NTracksWithTPC>0");
  h.Draw();
  numutree.Draw("(stdB_trDetUsed[][18]==1)*19","stdB_NTracksWithTPC>0","same");
  numutree.Draw("(stdB_trDetUsed[][17]==1)*18","stdB_NTracksWithTPC>0","same");
  numutree.Draw("(stdB_trDetUsed[][16]==1)*17","stdB_NTracksWithTPC>0","same");
  numutree.Draw("(stdB_trDetUsed[][15]==1)*16","stdB_NTracksWithTPC>0","same");
  numutree.Draw("(stdB_trDetUsed[][14]==1)*15","stdB_NTracksWithTPC>0","same");
  numutree.Draw("(stdB_trDetUsed[][13]==1)*14","stdB_NTracksWithTPC>0","same");
  numutree.Draw("(stdB_trDetUsed[][12]==1)*13","stdB_NTracksWithTPC>0","same");
  numutree.Draw("(stdB_trDetUsed[][11]==1)*12","stdB_NTracksWithTPC>0","same");
  numutree.Draw("(stdB_trDetUsed[][10]==1)*11","stdB_NTracksWithTPC>0","same");
  numutree.Draw("(stdB_trDetUsed[][9]==1)*10","stdB_NTracksWithTPC>0","same");
  numutree.Draw("(stdB_trDetUsed[][8]==1)*9","stdB_NTracksWithTPC>0","same");
  numutree.Draw("(stdB_trDetUsed[][7]==1)*8","stdB_NTracksWithTPC>0","same");
  numutree.Draw("(stdB_trDetUsed[][6]==1)*7","stdB_NTracksWithTPC>0","same");
  numutree.Draw("(stdB_trDetUsed[][5]==1)*6","stdB_NTracksWithTPC>0","same");
  numutree.Draw("(stdB_trDetUsed[][4]==1)*5","stdB_NTracksWithTPC>0","same");
  numutree.Draw("(stdB_trDetUsed[][3]==1)*4","stdB_NTracksWithTPC>0","same");
  numutree.Draw("(stdB_trDetUsed[][2]==1)*3","stdB_NTracksWithTPC>0","same");
  numutree.Draw("(stdB_trDetUsed[][0]==1)*1","stdB_NTracksWithTPC>0","same");
  c1.Print("numutree_validation.pdf");      
  

  numutree.Draw("stdB_trNHits"); 		 c1.Print("numutree_validation.pdf");      
  c1.SetLogy(1);
  numutree.Draw("stdB_trChi2","abs(stdB_trChi2)<100000");  c1.Print("numutree_validation.pdf"); 
  c1.SetLogy(0);
  numutree.Draw("stdB_trChi2","stdB_trChi2<100 && stdB_trChi2>0");  c1.Print("numutree_validation.pdf"); 
  numutree.Draw("stdB_trPositionStart[][0]","abs(stdB_trPositionStart[][0])<3000"); 		 c1.Print("numutree_validation.pdf");  
  numutree.Draw("stdB_trPositionStart[][1]","abs(stdB_trPositionStart[][1])<3000"); 		 c1.Print("numutree_validation.pdf");  
  numutree.Draw("stdB_trPositionStart[][2]","abs(stdB_trPositionStart[][2])<5000"); 		 c1.Print("numutree_validation.pdf");  
  numutree.Draw("stdB_trPositionStart[][3]","abs(stdB_trPositionStart[][3])<8000"); 		 c1.Print("numutree_validation.pdf");  

  numutree.Draw("stdB_trPositionEnd[][0]","abs(stdB_trPositionEnd[][0])<3000"); 		 c1.Print("numutree_validation.pdf");  
  numutree.Draw("stdB_trPositionEnd[][1]","abs(stdB_trPositionEnd[][1])<3000"); 		 c1.Print("numutree_validation.pdf");  
  numutree.Draw("stdB_trPositionEnd[][2]","abs(stdB_trPositionEnd[][2])<5000"); 		 c1.Print("numutree_validation.pdf");  
  numutree.Draw("stdB_trPositionEnd[][3]","abs(stdB_trPositionEnd[][3])<8000"); 		 c1.Print("numutree_validation.pdf");  

  numutree.Draw("stdB_trDir[][0]"); 		 c1.Print("numutree_validation.pdf"); 
  numutree.Draw("stdB_trDir[][1]"); 		 c1.Print("numutree_validation.pdf"); 
  numutree.Draw("stdB_trDir[][2]"); 		 c1.Print("numutree_validation.pdf"); 
  numutree.Draw("stdB_trMom","stdB_trMom<11000"); 		 c1.Print("numutree_validation.pdf");  
  c1.SetLogy(1);
  numutree.Draw("stdB_trMomError","stdB_trMomError<10000"); 		 c1.Print("numutree_validation.pdf");
  c1.SetLogy(0);
  numutree.Draw("stdB_trMomRelErr","stdB_trMomRelErr<1"); 		 c1.Print("numutree_validation.pdf"); 
  numutree.Draw("stdB_trMom","abs(stdB_trCharge)>1"); 		 c1.Print("numutree_validation.pdf");   
  numutree.Draw("stdB_trCharge","abs(stdB_trCharge)<2"); 		 c1.Print("numutree_validation.pdf");   
  numutree.Draw("stdB_trCosTheta"); 		 c1.Print("numutree_validation.pdf");  
  numutree.Draw("stdB_trECALTrShVal"); 		 c1.Print("numutree_validation.pdf"); 
  numutree.Draw("stdB_trEnuWeight"); 		 c1.Print("numutree_validation.pdf"); 
  numutree.Draw("stdB_trNFgds"); 		 c1.Print("numutree_validation.pdf");     

  // ---- TPC variables -------

  numutree.Draw("stdB_trTpcDet",     "stdB_trTpcDet>0"); 		 c1.Print("numutree_validation.pdf");      
  numutree.Draw("stdB_trTpcCharge",   "stdB_trTpcDet>0"); 		 c1.Print("numutree_validation.pdf");    
  numutree.Draw("stdB_trTpcMom",      "stdB_trTpcMom<10000    && stdB_trTpcDet>0"); 		 c1.Print("numutree_validation.pdf");       
  numutree.Draw("stdB_trTpcMomRelErr","stdB_trTpcMomRelErr<2  && stdB_trTpcDet>0"); 		 c1.Print("numutree_validation.pdf");       
  numutree.Draw("stdB_trTpcNHits",    "stdB_trTpcDet>0"); 		 c1.Print("numutree_validation.pdf");            


  // ---- TPC PID variables -------

  numutree.Draw("stdB_trCT"); 		 c1.Print("numutree_validation.pdf");      
  numutree.Draw("stdB_trCT",     "stdB_trCT>0 && stdB_trCT<1000"); 		 c1.Print("numutree_validation.pdf");      
  numutree.Draw("stdB_trPullmu", "abs(stdB_trPullmu)<10  && stdB_trPullmu!=0"); 		 c1.Print("numutree_validation.pdf");  
  numutree.Draw("stdB_trPullele","abs(stdB_trPullele)<10 && stdB_trPullele!=0"); 		 c1.Print("numutree_validation.pdf");  
  numutree.Draw("stdB_trPullp",  "abs(stdB_trPullp)<10   && stdB_trPullp!=0"); 		 c1.Print("numutree_validation.pdf");  
  numutree.Draw("stdB_trPullpi", "abs(stdB_trPullpi)<10  && stdB_trPullpi!=0"); 		 c1.Print("numutree_validation.pdf");
  numutree.Draw("stdB_trPullk",  "abs(stdB_trPullk)<10   && stdB_trPullk!=0"); 		 c1.Print("numutree_validation.pdf");  


  numutree.Draw("stdB_trCT:stdB_trTpcMom",      "stdB_trCT>200 && stdB_trCT<1000                 && stdB_trTpcMom<2000 && stdB_trTpcDet==1");
  numutree.Draw("stdB_trCTexpmu:stdB_trTpcMom", "abs(stdB_trCTexpmu)<1000  && stdB_trCTexpmu!=0  && stdB_trTpcMom<2000 && stdB_trTpcDet==1","same");
  gStyle->SetMarkerColor(2);
  numutree.Draw("stdB_trCTexpele:stdB_trTpcMom","abs(stdB_trCTexpele)<1000 && stdB_trCTexpele!=0 && stdB_trTpcMom<2000 && stdB_trTpcDet==1","same");
  gStyle->SetMarkerColor(3);
  numutree.Draw("stdB_trCTexpp:stdB_trTpcMom",  "abs(stdB_trCTexpp)<1000   && stdB_trCTexpp!=0   && stdB_trTpcMom<2000 && stdB_trTpcDet==1","same");
  gStyle->SetMarkerColor(5);
  c1.Print("numutree_validation.pdf");      


  /*
    numutree.Draw("stdB_trCTexpmu"); 		 c1.Print("numutree_validation.pdf"); 
    numutree.Draw("stdB_trCTexpele"); 		 c1.Print("numutree_validation.pdf");
    numutree.Draw("stdB_trCTexpp"); 		 c1.Print("numutree_validation.pdf");  
    numutree.Draw("stdB_trCTexppi"); 		 c1.Print("numutree_validation.pdf"); 
    numutree.Draw("stdB_trCTexpk"); 		 c1.Print("numutree_validation.pdf");  
  */
  
  numutree.Draw("stdB_trCTerrmu", "stdB_trCTerrmu<200  && stdB_trTpcDet!=0");		 c1.Print("numutree_validation.pdf"); 
  numutree.Draw("stdB_trCTerrele","stdB_trCTerrele<200 && stdB_trTpcDet!=0");		 c1.Print("numutree_validation.pdf"); 
  numutree.Draw("stdB_trCTerrp",  "stdB_trCTerrp<200   && stdB_trTpcDet!=0");		 c1.Print("numutree_validation.pdf"); 
  numutree.Draw("stdB_trCTerrpi", "stdB_trCTerrpi<200  && stdB_trTpcDet!=0");		 c1.Print("numutree_validation.pdf"); 
  numutree.Draw("stdB_trCTerrk",  "stdB_trCTerrk<200   && stdB_trTpcDet!=0");		 c1.Print("numutree_validation.pdf"); 

  
  // ---- Truth-Rec link variables -------

  if (isMC){    
    numutree.Draw("stdB_trTruePDG"); 		 c1.Print("numutree_validation.pdf");      
    numutree.Draw("stdB_trTrueParentPDG"); 		 c1.Print("numutree_validation.pdf");
    numutree.Draw("stdB_trTrueGParentPDG"); 		 c1.Print("numutree_validation.pdf");
    numutree.Draw("stdB_trTruePur"); 		 c1.Print("numutree_validation.pdf");       
    numutree.Draw("stdB_trTruePosition[][0]"); 		 c1.Print("numutree_validation.pdf");  
    numutree.Draw("stdB_trTruePosition[][1]"); 		 c1.Print("numutree_validation.pdf");  
    numutree.Draw("stdB_trTruePosition[][2]"); 		 c1.Print("numutree_validation.pdf");  
    numutree.Draw("stdB_trTruePosition[][3]"); 		 c1.Print("numutree_validation.pdf");  
    numutree.Draw("stdB_trTrueDirection[][0]"); 		 c1.Print("numutree_validation.pdf"); 
    numutree.Draw("stdB_trTrueDirection[][1]"); 		 c1.Print("numutree_validation.pdf"); 
    numutree.Draw("stdB_trTrueDirection[][2]"); 		 c1.Print("numutree_validation.pdf"); 
    numutree.Draw("stdB_trTrueMomentum"); 		 c1.Print("numutree_validation.pdf");  
    numutree.Draw("stdB_trTrueCharge"); 		 c1.Print("numutree_validation.pdf");    
    numutree.Draw("stdB_trTrueVertexIndex"); 		 c1.Print("numutree_validation.pdf");
  }  


  // ---- FGD variables -------
  
  numutree.Draw("stdB_NTracksWithFGD"); 		 c1.Print("numutree_validation.pdf");  
  numutree.Draw("stdB_trFgdDet"); 		 c1.Print("numutree_validation.pdf");  
  numutree.Draw("stdB_trFgdNHits"); 		 c1.Print("numutree_validation.pdf");
  numutree.Draw("stdB_trFgdChi2","stdB_trFgdChi2<100"); 		 c1.Print("numutree_validation.pdf"); 
  numutree.Draw("stdB_trFgdPositionStart[][0]"); 		 c1.Print("numutree_validation.pdf");
  numutree.Draw("stdB_trFgdPositionStart[][1]"); 		 c1.Print("numutree_validation.pdf");
  numutree.Draw("stdB_trFgdPositionStart[][2]"); 		 c1.Print("numutree_validation.pdf");
  numutree.Draw("stdB_trFgdPositionStart[][3]"); 		 c1.Print("numutree_validation.pdf");
  numutree.Draw("stdB_trFgdPositionEnd[][0]"); 		 c1.Print("numutree_validation.pdf");  
  numutree.Draw("stdB_trFgdPositionEnd[][1]"); 		 c1.Print("numutree_validation.pdf");  
  numutree.Draw("stdB_trFgdPositionEnd[][2]"); 		 c1.Print("numutree_validation.pdf");  
  numutree.Draw("stdB_trFgdPositionEnd[][3]"); 		 c1.Print("numutree_validation.pdf");  
  numutree.Draw("stdB_trFgdPositionEnd[][0]-stdB_trFgdPositionStart[][0]"); 		 c1.Print("numutree_validation.pdf");
  numutree.Draw("stdB_trFgdPositionEnd[][1]-stdB_trFgdPositionStart[][1]"); 		 c1.Print("numutree_validation.pdf");
  numutree.Draw("stdB_trFgdPositionEnd[][2]-stdB_trFgdPositionStart[][2]"); 		 c1.Print("numutree_validation.pdf");
  numutree.Draw("stdB_trFgdPositionEnd[][3]-stdB_trFgdPositionStart[][3]"); 		 c1.Print("numutree_validation.pdf");


  numutree.Draw("stdB_trFgdDir[0]"); 		 c1.Print("numutree_validation.pdf"); 
  numutree.Draw("stdB_trFgdDir[1]"); 		 c1.Print("numutree_validation.pdf"); 
  numutree.Draw("stdB_trFgdDir[2]"); 		 c1.Print("numutree_validation.pdf"); 
  
  // ----  Kinematics variables (for 2010a analysis see specific module) --------

  numutree.Draw("stdB_trENuRec_1Muon","stdB_trENuRec_1Muon>0 && stdB_trENuRec_1Muon<10000");
  numutree.Draw("stdB_trEMuRec_1Muon","stdB_trEMuRec_1Muon>0 && stdB_trEMuRec_1Muon<10000");
  numutree.Draw("stdB_trQ2_1Muon",    "stdB_trENuRec_1Muon<10000 && stdB_trENuRec_1Muon>0");		 c1.Print("numutree_validation.pdf");
  numutree.Draw("stdB_ENuRec_CCQE",   "stdB_ENuRec_CCQE<10000 && stdB_ENuRec_CCQE>0"); 		 c1.Print("numutree_validation.pdf");
  numutree.Draw("stdB_Q2_CCQE",       "stdB_Q2_CCQE>0 && stdB_Q2_CCQE<1"); 		 c1.Print("numutree_validation.pdf"); 
  numutree.Draw("stdB_PQ2_CCQE",      "stdB_PQ2_CCQE>0 && stdB_PQ2_CCQE<1"); 		 c1.Print("numutree_validation.pdf"); 
  numutree.Draw("stdB_NuInvMass_CCQE","stdB_NuInvMass_CCQE!=0 && abs(stdB_NuInvMass_CCQE)<100"); 		 c1.Print("numutree_validation.pdf"); 
  numutree.Draw("stdB_EProton_CCQE",  "stdB_EProton_CCQE<10000 && stdB_EProton_CCQE!=0");		 c1.Print("numutree_validation.pdf");       
  numutree.Draw("stdB_FinalCosTheta_CCQE","stdB_FinalCosTheta_CCQE>-2"); 		 c1.Print("numutree_validation.pdf"); 
  numutree.Draw("stdB_FinalMass_CCQE",    "stdB_FinalMass_CCQE>0 && stdB_FinalMass_CCQE<5000"); 		 c1.Print("numutree_validation.pdf");     
  numutree.Draw("stdB_FinalEnergy_CCQE",  "stdB_FinalEnergy_CCQE>0 && stdB_FinalEnergy_CCQE<5000"); 		 c1.Print("numutree_validation.pdf");   
  numutree.Draw("stdB_FinalMom_CCQE",     "stdB_FinalMom_CCQE>0 && stdB_FinalMom_CCQE<5000"); 		 c1.Print("numutree_validation.pdf");      
  
  numutree.Draw("stdB_ENuRec_CCQE-stdB_trENuRec_1Muon","stdB_ENuRec_CCQE<10000 && stdB_ENuRec_CCQE>0 && abs(stdB_trENuRec_1Muon)<10000");


  // ----  Delayed clusters -----

  numutree.Draw("stdB_EarliestTrackTime","stdB_EarliestTrackTime>80"); 		 c1.Print("numutree_validation.pdf"); 
  c1.SetLogy(1);
  numutree.Draw("stdB_NClusters"); 		 c1.Print("numutree_validation.pdf");         
  numutree.Draw("stdB_NClusterscut[0][0]"); 	 	
  numutree.Draw("stdB_NClusterscut[1][0]","","same"); 		 c1.Print("numutree_validation.pdf");      
  numutree.Draw("stdB_NClusterscut[0][1]"); 	 	
  numutree.Draw("stdB_NClusterscut[1][1]","","same"); 		 c1.Print("numutree_validation.pdf");      
  numutree.Draw("stdB_NClusterscut[0][2]"); 	 	
  numutree.Draw("stdB_NClusterscut[1][2]","","same"); 		 c1.Print("numutree_validation.pdf");      
  numutree.Draw("stdB_NClusterscut[0][3]"); 	 	
  numutree.Draw("stdB_NClusterscut[1][3]","","same"); 		 c1.Print("numutree_validation.pdf");      
  numutree.Draw("stdB_NClusterscut[0][4]"); 	 	
  numutree.Draw("stdB_NClusterscut[1][4]","","same"); 		 c1.Print("numutree_validation.pdf");      
  c1.SetLogy(0);

  numutree.Draw("stdB_ClusterPos[][0]","stdB_NClusters>0"); 		 c1.Print("numutree_validation.pdf"); 
  numutree.Draw("stdB_ClusterPos[][0]","stdB_NClusters>0 && abs(stdB_ClusterPos[][0])>2");  c1.Print("numutree_validation.pdf"); 
  numutree.Draw("stdB_ClusterPos[][1]","stdB_NClusters>0"); 		 c1.Print("numutree_validation.pdf"); 
  numutree.Draw("stdB_ClusterPos[][1]","stdB_NClusters>0 && abs(stdB_ClusterPos[][1]-50)>3");  c1.Print("numutree_validation.pdf"); 
  numutree.Draw("stdB_ClusterPos[][2]","stdB_NClusters>0"); 		 c1.Print("numutree_validation.pdf"); 
  numutree.Draw("stdB_ClusterPos[][3]","stdB_NClusters>0"); 		 c1.Print("numutree_validation.pdf"); 
  numutree.Draw("stdB_ClusterNHits",   "stdB_NClusters>0"); 		 c1.Print("numutree_validation.pdf"); 
  numutree.Draw("stdB_ClusterRMS[][0]",     "stdB_NClusters>0"); 		 c1.Print("numutree_validation.pdf");   
  numutree.Draw("stdB_ClusterRMS[][0]",     "stdB_NClusters>0  && abs(stdB_ClusterPos[][0])>2"); 		 c1.Print("numutree_validation.pdf");   
  numutree.Draw("stdB_ClusterRMS[][1]",     "stdB_NClusters>0"); 		 c1.Print("numutree_validation.pdf");   
  numutree.Draw("stdB_ClusterRMS[][1]",     "stdB_NClusters>0  && abs(stdB_ClusterPos[][1]-50)>3"); 		 c1.Print("numutree_validation.pdf");   
  numutree.Draw("stdB_ClusterRMS[][2]",     "stdB_NClusters>0"); 		 c1.Print("numutree_validation.pdf");    
  numutree.Draw("stdB_ClusterRMS[][3]",     "stdB_NClusters>0 && stdB_ClusterRMS[][3]<20");		 c1.Print("numutree_validation.pdf");   
  numutree.Draw("stdB_ClusterRMS[][3]",     "stdB_NClusters>0 && stdB_ClusterRMS[][3]>20");		 c1.Print("numutree_validation.pdf");   
  numutree.Draw("stdB_ClusterCharge",  "stdB_NClusters>0"); 		 c1.Print("numutree_validation.pdf"); 
  numutree.Draw("stdB_ClusterDistVertex[][0]",    "stdB_NClusters>0 && stdB_ClusterDistVertex[][0]<100000"); 		
  numutree.Draw("stdB_ClusterDistVertex[][1]",    "stdB_NClusters>0 && stdB_ClusterDistVertex[][1]<100000","same"); 		 c1.Print("numutree_validation.pdf"); 
  numutree.Draw("stdB_ClusterTimeDistVertex[][0]","stdB_NClusters>0"); 		
  numutree.Draw("stdB_ClusterTimeDistVertex[][1]","stdB_NClusters>0","same"); 		 c1.Print("numutree_validation.pdf");         




  numutree.Draw("g10aB_ctc","stdB_trTpcDet>=0");
  numutree.Draw("g10aB_trPullcmu","stdB_trTpcDet>=0");
  numutree.Draw("g10aB_trPullcele","stdB_trTpcDet>=0");
  numutree.Draw("g10aB_trPullcp","stdB_trTpcDet>=0");
  numutree.Draw("g10aB_trPullcpi","stdB_trTpcDet>=0");
  numutree.Draw("g10aB_trPullck","stdB_trTpcDet>=0");
  numutree.Draw("g10a_trProbmu");
  numutree.Draw("g10a_trProbele");
  numutree.Draw("g10a_trProbp");
  numutree.Draw("g10a_trProbpi");
  numutree.Draw("g10a_trProbk");
  numutree.Draw("g10aB_ctexpCorrmu","stdB_trTpcDet>=0");
  numutree.Draw("g10aB_ctexpCorrele","stdB_trTpcDet>=0");
  numutree.Draw("g10aB_ctexpCorrp","stdB_trTpcDet>=0");
  numutree.Draw("g10aB_ctexpCorrpi","stdB_trTpcDet>=0");
  numutree.Draw("g10aB_ctexpCorrk","stdB_trTpcDet>=0");
  
  numutree.Draw("g10aB_NTPCTracksCut");
  numutree.Draw("g10aB_NTPCTracksPosCut");
  numutree.Draw("g10aB_NTPCTracksNegCut");



  c1.Print("numutree_validation.pdf]");        // close the file



}
