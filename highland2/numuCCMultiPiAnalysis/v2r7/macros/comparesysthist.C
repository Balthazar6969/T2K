

//TString key[3]={"h2","h2","psy"};

TString key[3]={"h2","h2","h2"};
TString sleg[3]={"FGD1","FGD1","FGD2"};
TString slegopt[3]={"f","l p","f"};
TString inh1="hist_fth2_"+key[0]+"_500_6Brun2w_6Btables_patchHEAD_reg20.root";
TString inh2="hist_fth2_"+key[1]+"_500_6Brun2w_6Btables_patchHEAD_reg20.root";
TString inxx="hist_fth2_"+key[2]+"_500_6Brun2w_6Btables_patch2HEAD_reg20.root";
TString stFGD="FGD";

//TString key[3]={"h2","h2","h2"};
//TString sleg[3]={"","patch fgd2","head fgd2"};
// TString inh1="";//hist_fth2_"+key[0]+"_100_6Brun2w_6Btables_patch2_reg20.root";
// TString inh2="hist_fth2_"+key[1]+"_100_6Brun2w_6Btables_patch2bis_reg20.root";
// TString inxx="hist_fth2_"+key[2]+"_100_6Brun2w_6Btables_HEAD2bis_reg20.root";

//TString inh2="hist_fth2_"+key[1]+"_100_6Brun2w_6Btables_HEAD_reg20.root";
//TString inh1="hist_fth2_"+key[0]+"_100_6Brun2w_6Btables_charge_reg20.root";
//TString inxx="hist_fth2_"+key[2]+"_500_6Brun2w_5Ftables_reg20.root";
//TString inxx="";


void comparesysthist(){
  TFile *inf[3]={NULL};
  
  if(inh1!="")inf[0]=new TFile(inh1.Data(),"READ");
  if(inh2!="")inf[1]=new TFile(inh2.Data(),"READ");
  if(inxx!="")inf[2]=new TFile(inxx.Data(),"READ");
 
  TString systname[17]={"bfield","momresol","momscale","tpcpid","fgdpid","chargeideff","tpcclustereff","tpctrackeff","fgdtrackeff","fgdhybtrackeff","tpcfgdmatcheff","meeff","pileup","fgdmass","oofv","sipion","all"};
  TString systname_dir[17]={"bfield_syst","momresol_syst","momscale_syst","tpcpid_syst","fgdpid_syst","chargeideff_syst","tpcclustereff_syst","tpctrackeff_syst","fgdtrackeff_syst","fgdhybridtrackeff_syst","tpcfgdmatcheff_syst","michel_syst","pileup_syst","fgdmass_syst","oofv_syst","sipion_syst","all_syst"};
  TString systlatex[17]={"Field distortions","Momentum resolution","Momentum scale","TPC PID","FGD PID","Charge ID efficiency","TPC cluster efficiency","TPC track efficiency","FGD track efficiency","FGD Hybrid track efficiency","TPC-FGD matching efficiency","Michel electron","Pile-up","FGD mass","OOFV","Pion secondary interactions","all magnet"};
  
  TString nn[4]={"CC","CC0pi","CC1pi","CCOth"};
  
  TString path="numuCC"+stFGD;
  
  TCanvas *c1[4];
  for(int isyst=0;isyst<17;isyst++){
    TString path2=path+"/"+systname_dir[isyst];
    if((inf[0] && !inf[0]->cd(path2.Data())) || (inf[1] && !inf[1]->cd(path2.Data())) || (inf[2] && !inf[2]->cd(path2.Data())) )continue;
      
    TH1F *hh[4][3];
    for(int ih=0;ih<4;ih++){
      c1[ih]=new TCanvas(Form("c1%s%d",systname[isyst].Data(),ih),Form("c1%s%d",systname[isyst].Data(),ih),1);
      TString path3=path2+"/"+nn[ih];
      for(int iff=0;iff<3;iff++){
	if(inf[iff]){
	  inf[iff]->cd(path3.Data());
	  //inf[iff]->ls();
	  hh[ih][iff]= (TH1F*) inf[iff]->Get(Form("%s/relerr_%s_%s_%s",path3.Data(),key[iff].Data(),systname[isyst].Data(),nn[ih].Data()));
	  if(hh[ih][iff])std::cout<<" ih "<<ih<<" iff "<<iff<<" exists! "<<std::endl;
	}
      }
      if(hh[ih][0]){
	hh[ih][0]->SetTitle(Form("%s for %s",systlatex[isyst].Data(),nn[ih].Data()));
	hh[ih][0]->SetLineColor(kBlue);
      }
      if(hh[ih][1]){
	hh[ih][1]->SetTitle(Form("%s for %s",systlatex[isyst].Data(),nn[ih].Data()));
	hh[ih][1]->SetMarkerColor(2);
	hh[ih][1]->SetLineColor(kBlue);
	hh[ih][1]->SetMarkerStyle(21);
      }
      if(hh[ih][2]){
	hh[ih][2]->SetTitle(Form("%s for %s",systlatex[isyst].Data(),nn[ih].Data()));
	hh[ih][2]->SetLineWidth(2);
	hh[ih][2]->SetLineColor(7);
      }
      TLegend *leg;
      if(systname[isyst]=="all" || systname[isyst]=="fgdmass"){ 
	leg = new TLegend(0.7,0.15,0.88,0.35);//right bottom
      }
      else if(systname[isyst]=="tpcfgdmatcheff"  || systname[isyst]=="tpcpid"){
	leg = new TLegend(0.5,0.65,0.68,0.85);//middle top
      }
      else if(systname[isyst]=="oofv" || systname[isyst]=="pileup"){
	leg = new TLegend(0.7,0.5,0.88,0.7);//middle right
      }
      else{
	leg = new TLegend(0.15,0.68,0.33,0.88);//top left
      }

      leg->SetFillColor(0);
      leg->SetLineColor(0);

      //    if(hh[ih][0])
      // 	leg->AddEntry(hh[ih][0],sleg[0].Data(),slegopt[0].Data());
      if(hh[ih][1])
	leg->AddEntry(hh[ih][1],sleg[1].Data(),slegopt[1].Data());
      if(hh[ih][2])
	leg->AddEntry(hh[ih][2],sleg[2].Data(),slegopt[2].Data());
      double vmax1=1;

      if(hh[ih][1] && hh[ih][0])
	vmax1= TMath::Max(hh[ih][0]->GetMaximum(),hh[ih][1]->GetMaximum());
      if(hh[ih][2])
	vmax1  = TMath::Max(vmax1,hh[ih][2]->GetMaximum());

      if(hh[ih][0]){
	hh[ih][0]->GetYaxis()->SetRangeUser(0.,vmax1*1.1);
	hh[ih][0]->Draw();
      }
      if(hh[ih][1])
	hh[ih][1]->Draw("p same");    
      if(hh[ih][2])
	hh[ih][2]->Draw("same");
      leg->Draw("same");
      c1[ih]->Print(Form("numuValidation/fgd1fgd2%s_%s.eps",systname[isyst].Data(),nn[ih].Data()));
    }
  }
}


