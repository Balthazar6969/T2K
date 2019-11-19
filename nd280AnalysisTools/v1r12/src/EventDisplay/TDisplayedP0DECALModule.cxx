#include "TDisplayedP0DECALModule.hxx"
#include "TChain.h"
#include "TEveManager.h"
#include "TEveBoxSet.h"
#include "TEveArrow.h"
#include "TEveVSDStructs.h"
#include "TOstreamOverloads.hxx"
#include "TSorted4Vector.hxx"

const int NDETSUSED = 19;

TDisplayedP0DECALModule::TDisplayedP0DECALModule():
TDisplayedAnalysisModuleBase(TString("ReconDir/P0DECal"),	
    TString( "ND::TP0DECALReconModule::TP0DECALReconTrack"),
    TString("NONE"),
    TString( "ND::TP0DECALReconModule::TP0DECALReconShower"),
    TString("NONE")
    )
{
    if(!fKchainOK)return;
    if(fChain->SetBranchAddress("NReconTracks", &fNTracks)<0)fKchainOK=kFALSE;
    if(fChain->SetBranchAddress("ReconTracks", &fTracks)<0)fKchainOK=kFALSE;
    if(fChain->SetBranchAddress("NReconShowers", &fNShowers)<0){
    	cout<<" failed to set fNShowers in "<<
    	"TDisplayedP0DECALModule::TDisplayedP0DECALModule():"<<endl;
    	
    	fKchainOK=kFALSE;
    }
    if(fChain->SetBranchAddress("ReconShowers", &fShowers)<0){
    	cout<<" failed to set fShowers in "<<
    	"TDisplayedP0DECALModule::TDisplayedP0DECALModule():"<<endl;
    	
    	fKchainOK=kFALSE; 
    }
}
void TDisplayedP0DECALModule::Draw(int entry,int bunch)
{
    if(!fKchainOK)return;    
    TND280TrackList* gTrackList= PrepareToDraw(entry,"P0DECALRecon Tracks");
    if(gTrackList ==NULL)return;
    gTrackList->SetRnrState(kFALSE);
    TEveElementList* ReconstructedShowers = new TEveElementList("P0DECALRecon Shower");
    TString longName = "P0DECALRecon  showers";
    ReconstructedShowers->SetTitle(longName);
    ReconstructedShowers->SetRnrState(kFALSE);    
    TND280TrackPropagator* trkProp = gTrackList->GetPropagator();
    for(int i = 0; i<fNTracks;i++)
    {   	    	
    	ND::TP0DECALReconModule::TP0DECALReconTrack* reconTrack = 
    	(ND::TP0DECALReconModule::TP0DECALReconTrack *) fTracks->At(i);
    	if(reconTrack){
    	    if(!InSelectedBunch(reconTrack->Position.T(),bunch)
    	    )continue;
    	    TEveTrack* track = MakeTrack( trkProp,i,reconTrack);
    	    gTrackList->AddElement(track);
    	    track->MakeTrack();
    	    track->SetMainColor(kRed);     	    
            track->SetLineWidth(4);
    	}
    }
    for(int i = 0; i<fNShowers;i++)
    {
    	ND::TP0DECALReconModule::TP0DECALReconShower* Shower = 
    	(ND::TP0DECALReconModule::TP0DECALReconShower *) fShowers->At(i);  
    	if(Shower){
    	    if(!InSelectedBunch(Shower->Position.T(),bunch)
    	    )continue;
    	    TEveVector Direction ; 
    	    Direction.Set(Shower->Direction);
    	    // shower position
    	    TEveVector Position;
    	    Position.Set(Shower->Position.Vect());
    	    //float length=reconObject->ObjectLength;
    	    float length=500;
    	    //	if(length<10)length=500;
    	    //	if(length>10000)length=500;
    	    TVector3  angle3=Shower->ConeAngle;   	    
    	    const float aMin=0.05; // cone must have  a finite angle to be drawn!!
    	    float angle1=fmax(aMin,angle3.X());
    	    Direction*=length;
    	    float r1 = length*tan(angle1);
    	    // Draw an arrow 
    	    TEveArrow* arrow = new TEveArrow(-Direction[0],-Direction[1],-Direction[2],
    	    	Position[0]+Direction[0],
    	    	Position[1]+Direction[1],
    	    	Position[2]+Direction[2]);
    	    
    	    arrow->SetConeR(r1/length);
    	    arrow->SetConeL(1.0);
    	    arrow->SetDrawQuality(20);
    	    TString myName(Form("P0DECALRecon  shower (%2d)",i));	    	
    	    arrow->SetNameTitle(myName,myName);
    	    arrow->SetPickable(kTRUE);
    	    arrow->SetMainColor(kRed);
    	    ReconstructedShowers->AddElement(arrow);
    	    arrow->SetSourceObject(Shower);
    	}
    	else
    	    cout<<" failed to find p0d shower "<<i<<endl;
	
    }
    
    // loop over PIDs
    
    
    if(gTrackList->HasChildren() && ReconstructedShowers->HasChildren())
    {
    	TEveElementList* base=new TEveElementList("P0DECALRecon");
    	base->AddElement(gTrackList);
    	base->AddElement(ReconstructedShowers);
    	gEve->AddElement(base);
    	gTrackList->SetRnrState(kTRUE);
    	ReconstructedShowers->SetRnrState(kTRUE);
    	base->SetRnrState(kFALSE);
    	
    }
    else
    {
    	if(gTrackList->HasChildren()          ) gEve->AddElement(gTrackList);
    	if(ReconstructedShowers->HasChildren()) gEve->AddElement(ReconstructedShowers);
    } 
    
}

TND280Track* TDisplayedP0DECALModule::MakeTrack(TND280TrackPropagator*   trkProp,
    Int_t                  index,
    ND::TP0DECALReconModule::TP0DECALReconTrack* reconObject)
{
    // Helper function creating TEveTrack from T2K global recon track.
    //
    TEveRecTrack*  rt=new TEveRecTrack();;    
    rt->fIndex  = index;    
    rt->fSign   = ( reconObject->Curvature> 0) ? 1 : -1;
    rt->fV.Set(reconObject->Position.Vect());
    // work out momentum from curvature!
    //rt->fP.Set( ( reconObject->FrontMomentum)*reconObject->FrontDirection);    
    TND280Track* track = new TND280Track(rt, trkProp);
    float arbitaryLength=300;
    TVector3 	Direction=arbitaryLength*(reconObject->Direction);
    TVector3    Start=reconObject->Position.Vect();
    TVector3    End=Start+Direction;
    // as these tracks apparently have no length, make one up!
    
    track->AddPathMark(TEvePathMarkD(TEvePathMarkD::kDecay,
    	Vector3ToEve(End),
    	reconObject->Position.T()));
    
    // track->SetName(Form("ECAL ReconTrack %d2", index)); 
    // track->SetTitle(Form("ECAL recon Track %d2 ",index));
    TString myName(Form("P0DECALRecon  track (%2d)",index));
    track->SetName(myName);
    track->SetTitle(myName);
    track->SetSourceObject(reconObject);
    return track;
}



void TDisplayedP0DECALModule::Describe(ND::TP0DECALReconModule::TP0DECALReconShower* RO)
{
    Centre(RO->Position);
    Output(RO->AMR,"AMR","The Pid Variables - AMR/F ChargeStandardDeviation/F MaxWidth/F MeanCharge/F NumHits/I Track_Length/I.");
    Output(RO->BenCWPosition,"BenCWPosition","Ben CW position.");
    Output(RO->ConeAngle,"ConeAngle","The shower opening angle.");
    Output(RO->CWTrackWidth,"CWTrackWidth","");
    Output(RO->Direction,"Direction","The shower direction.");
    Output(RO->EDeposit,"EDeposit","the energy deposited in the shower.");
    Output(RO->Max_Ratio,"Max_Ratio","");
    Output(RO->NDOF,"NDOF","the number of degrees of freedom in the reconstruction.");
    Output(RO->NHits,"NHits","");
    Output(RO->NormChargeSD,"NormChargeSD","");
    Output(RO->PathChargeRatio,"PathChargeRatio","");
    Output(RO->Position,"Position","the shower starting position.");
    Output(RO->Quality,"Quality","the goodness of fit for the reconstruction.");
    Output(RO->StackNo,"StackNo","Stack of interaction.");
    Output(RO->TrShval,"TrShval","");
    Output(RO->UniqueID,"UniqueID","The object UniqueID which can be used to match this object to the Global Recon object.");
}
void TDisplayedP0DECALModule::Describe( ND::TP0DECALReconModule::TP0DECALReconTrack* RO)
{
    Centre(RO->Position);
    Output(RO->AMR,"AMR","The Pid Variables.");
    Output(RO->BenCWPosition,"BenCWPosition","Ben CW position.");
    Output(RO->Curvature,"Curvature","The track curvature aRO->nd the X axis.");
    Output(RO->CWTrackWidth,"CWTrackWidth","");
    Output(RO->Direction,"Direction","The track direction.");
    Output(RO->EDeposit,"EDeposit","the energy deposited in the track.");
    Output(RO->Max_Ratio,"Max_Ratio","");
    Output(RO->NDOF,"NDOF","the number of degrees of freedom in the reconstruction.");
    Output(RO->NHits,"NHits","");
    Output(RO->NormChargeSD,"NormChargeSD","");
    Output(RO->PathChargeRatio,"PathChargeRatio","");
    Output(RO->Position,"Position","the track starting position.");
    Output(RO->Quality,"Quality","the goodness of fit for the reconstruction.");
    Output(RO->StackNo,"StackNo","Stack of interaction.");
    Output(RO->TrShval,"TrShval","");
    Output(RO->UniqueID,"UniqueID","The object UniqueID which can be used to match this object to the Global Recon object.");
    Output(RO->Width,"Width","the width of the track. ");
    
}
