#include "TDisplayedTrackerModule.hxx"
#include "TChain.h"
#include "TEveManager.h"
#include "TEveVSDStructs.h"
#include "TOstreamOverloads.hxx"
#include "TVector3.h"

TDisplayedTrackerModule::TDisplayedTrackerModule() :
TDisplayedAnalysisModuleBase(TString("ReconDir/Tracker"),
  TString( "ND::TTrackerReconModule::TTrackerResult"),
  TString( "ND::TTrackerReconModule::TTrackerVertex"),
  TString("NONE"),
  TString("NONE")) {
  if(!fKchainOK){ return; }
  if(fChain->SetBranchAddress("NTracks", &fNTracks) < 0){ fKchainOK = false; }
  if(fChain->SetBranchAddress("Tracks", &fTracks) < 0){ fKchainOK = false; }
  if(fChain->SetBranchAddress("NVertices", &fNVertices) < 0){
    fKchainOK = false;
  }
  if(fChain->SetBranchAddress("Vertices", &fVertices) < 0){
    fKchainOK = false;
  }
}

template <class objectType>
TND280TrackList* TDisplayedTrackerModule::DrawArray(TClonesArray* Constituents,
  Int_t NConstituents) {
  if(NConstituents < 1){ return NULL; }
  TND280TrackList* List = new TND280TrackList("");
  for(int i = 0; i < NConstituents; i++){
    objectType Cons = static_cast<objectType>(Constituents->At(i));
    if(i == 0){
      TString name=Cons->ClassName();
      name.Remove(0,26);
      if(i == 0){ List->SetName(name+TString(" constituents")); }
    }
    List->AddElement(Draw(Cons, i));
  }
  return List;
}

template <>
TND280TrackList* TDisplayedTrackerModule::DrawArray<
  ND::TTrackerReconModule::TTrackerConstituent*>(TClonesArray* Constituents,
  Int_t NConstituents) {
  if(NConstituents < 1){ return NULL; }
  TND280TrackList* List = new TND280TrackList("");
  for(int i = 0; i < NConstituents; i++){
    ND::TTrackerReconModule::TTrackerConstituent* Cons =
      static_cast<ND::TTrackerReconModule::TTrackerConstituent*>(
        Constituents->At(i));
    if(i == 0){
      TString name = Cons->ClassName();
      name.Remove(0,26);
      if(i == 0){ List->SetName(name+TString(" constituents")); }
    }
    List->AddElement(Draw(Cons, i, Constituents));
  }
  return List;
}

void TDisplayedTrackerModule::Draw(int entry, int bunch){

  if(!fKchainOK){ return; }
  TND280TrackList* gTrackList = PrepareToDraw(entry,"Tracker Tracks");
  if(gTrackList == NULL){ return; }
  gTrackList->SetRnrState(kFALSE);

  fTrkProp = gTrackList->GetPropagator();
  for(int i = 0; i < fNTracks; i++){

    ND::TTrackerReconModule::TTrackerResult* result =
      static_cast<ND::TTrackerReconModule::TTrackerResult*>(fTracks->At(i));
    if(!InSelectedBunch(result->Position.T(),bunch)){ continue; }
    TND280Track* track = MakeTrack( fTrkProp,i,result);
    gTrackList->AddElement(track);

    /*
    look for constituents , can either be composite, or FGD or TPC only!
    */
    TND280TrackList* e =
      DrawArray<ND::TTrackerReconModule::TTrackerConstituent*>(
        result->Constituents, result->NConstituents);
    if(e != NULL){ track->AddElement(e); }
    /*
    Look at TPC only contributions
    */
    TND280TrackList* e2 = DrawArray<ND::TTrackerReconModule::TTPCTrack*>(
      result->TPC,result->NTPCs);
    if(e != NULL){ track->AddElement(e2); }
    /*
    Look at FGD only contributions
    */
    TND280TrackList* e3 =
      DrawArray<ND::TTrackerReconModule::TFGDTrack*>(result->FGD,result->NFGDs);
    if(e != NULL){ track->AddElement(e3); }
    }// loop over Tracks
  if(gTrackList->HasChildren()){ gEve->AddElement(gTrackList); }
}

template <class objectType>
void TDisplayedTrackerModule::DescribeArray(TClonesArray* Constituents,
  Int_t NConstituents){

  doIndent();
  std::cout << "DescribeArray called for array containing " << NConstituents
    << " object" << ((NConstituents>1)?"s.":".") << std::endl;
  indent++;
  divider();
  for(int i = 0; i < NConstituents; i++) {

    objectType Cons = static_cast<objectType>(Constituents->At(i));
    doIndent();
    std::cout << "Describe constituent number " << (i+1) <<" of type "
      << Cons->ClassName() << std::endl;
    Describe(Cons);
    divider();
  }
  indent--;
}

template <>
void TDisplayedTrackerModule::DescribeArray<
  ND::TTrackerReconModule::TTrackerConstituent*>(TClonesArray* Constituents,
  Int_t NConstituents){

  doIndent();
  std::cout << "DescribeArray called for array containing " << NConstituents
    << " object" << ((NConstituents>1)?"s.":".") << std::endl;
  indent++;
  divider();
  for(int i = 0; i < NConstituents; i++) {

    ND::TTrackerReconModule::TTrackerConstituent* Cons =
      static_cast<ND::TTrackerReconModule::TTrackerConstituent*>(
        Constituents->At(i));
    doIndent();
    std::cout << "Describe constituent number " << (i+1) <<" of type "
      << Cons->ClassName() << std::endl;
    Describe(Cons, Constituents);
    divider();
  }
  indent--;
}


void TDisplayedTrackerModule::Describe(ND::TTrackerReconModule::TFGDTrack* RO){
  doIndent();
  std::cout << "FGD Track: " << std::endl;
  Output(RO->Chi2,"Chi2","Chi2 of the FGD track fit.");
  Output(RO->Detector,"Detector","FGD number 1 or 2.");
  Output(RO->Direction,"Direction","the FGD track direction vector (dx,dy,dz)");
  Output(RO->EDeposit,"EDeposit","the FGD track energy deposit (total number of"
    " pe's)");
  Output(RO->Ndof,"Ndof","Number of degrees of freedom in FGD track fit.");
  Output(RO->Position,"Position","the FGD track starting position vector coordi"
    "nates in order x,y,z,t where x,y,z are in mm, t is in ns");
}
TND280Track* TDisplayedTrackerModule::Draw(
  ND::TTrackerReconModule::TTPCTrack* RO, int index){
  return MakeTrack(fTrkProp,index,RO);
}
TND280Track* TDisplayedTrackerModule::Draw(
  ND::TTrackerReconModule::TFGDTrack* RO, int index){
  return MakeTrack(fTrkProp,index,RO);
}

void TDisplayedTrackerModule::Describe(ND::TTrackerReconModule::TTPCTrack* RO){
  doIndent();
  std::cout << "TPC Track: " << std::endl;
  Output(RO->Ccorr,"Ccorr","Corrected truncated mean charge deposit in PID.");
  Output(RO->Charge,"Charge","Charge from the TPC pid (+1, or -1).");
  Output(RO->Chi2,"Chi2","TPC chi2 calculated after likelihood fit.");
  Output(RO->dEdxexpEle,"dEdxexpEle",
    "Estimated dE/dx for electron hypothesis.");
  Output(RO->dEdxexpKaon,"dEdxexpKaon","Estimated dE/dx for kaon hypothesis.");
  Output(RO->dEdxexpMuon,"dEdxexpMuon","Estimated dE/dx for muon hypothesis.");
  Output(RO->dEdxexpPion,"dEdxexpPion","Estimated dE/dx for pion hypothesis.");
  Output(RO->dEdxexpProton,"dEdxexpProton",
    "Estimated dE/dx for proton hypothesis.");
  Output(RO->Detector,"Detector","TPC number 1, 2 or 3.");
  Output(RO->Direction,"Direction","TPC pid direction vector in mm.");
  Output(RO->DirectionVariance,"DirectionVariance",
    "TPC pid variance in vector direction in mm^2.");
  Output(RO->EnteringFGD,"EnteringFGD","not sure");
  Output(RO->ExtrapolatedVertexXX,"ExtrapolatedVertexXX",
    "for xbar vertex, this is x coordinate in mm");
  Output(RO->ExtrapolatedVertexYY,"ExtrapolatedVertexYY",
    "for ybar vertex, this is y coordinate in mm");
  Output(RO->ExtrapolatedVertexZX,"ExtrapolatedVertexZX",
    "for xbar vertex, this is z coordinate in mm");
  Output(RO->ExtrapolatedVertexZY,"ExtrapolatedVertexZY",
    "for ybar vertex, this is z coordinate in mm");
  Output(RO->HasExtrapolation,"HasExtrapolation",
    "extrapolation method of vertex is calculated or not");
  Output(RO->MeanDrift,"MeanDrift",
    "TPC track mean drift value used in diffusion model.");
  Output(RO->Momentum,"Momentum","Momentum of the TPC pid in MeV/c.");
  Output(RO->MomentumError,"MomentumError",
    "Uncertainty in the Momentum in MeV/c from the TPC pid.");
  Output(RO->NConstituents,"NConstituents",
    "Number of constituents (probably 1, for track!).");
  Output(RO->Ndof,"Ndof","Number of degrees of freedom in TPC fit.");
  Output(RO->NHits,"NHits","number of clusters used in TPC fit");
  Output(RO->NTrun,"NTrun","70% of the number of clusters");
  Output(RO->Position,"Position",
    "Position at which kinematics are reported in mm, ns.");
  Output(RO->PositionVariance,"PositionVariance",
    "Variance in Position in mm^2, ns^2.");
  Output(RO->PullEle,"PullEle","Pull for TPC pid electron hypothesis.");
  Output(RO->PullKaon,"PullKaon","Pull for TPC pid kaon hypothesis.");
  Output(RO->PullMuon,"PullMuon","Pull for TPC pid muon hypothesis.");
  Output(RO->PullPion,"PullPion","Pull for TPC pid pion hypothesis.");
  Output(RO->PullProton,"PullProton","Pull for TPC pid proton hypothesis.");
  Output(RO->Sigma0,"Sigma0",
    "TPC track diffusion parameters depend on diffusion model used Original"
    " TPC diffusion model was: sigma^2 = sigma0 + sigma1*drift + sigma*drift^2"
    " Updated TPC diffusion model is: sigma^2 = sigma0*(drift-mean_drift) + "
    "sigma1*mean_drift + sigma2*drift^2 units are mm, to convert to "
    "microns/sqrt(cm) take for example: D = sqrt( sigma/10 )*10000.");
  Output(RO->Sigma1,"Sigma1","TPC track diffusion sigma1 parameter.");
  Output(RO->Sigma2,"Sigma2","TPC track diffusion sigma2 parameter.");
  Output(RO->SigmaEle,"SigmaEle",
    "Sigma estimated width of TPC pid electron hypothesis.");
  Output(RO->SigmaKaon,"SigmaKaon",
    "Sigma estimated width of TPC pid kaon hypothesis.");
  Output(RO->SigmaMuon,"SigmaMuon",
    "Sigma estimated width of TPC pid muon hypothesis.");
  Output(RO->SigmaPion,"SigmaPion",
    "Sigma estimated width of TPC pid pion hypothesis.");
  Output(RO->SigmaProton,"SigmaProton",
    "Sigma estimated width of TPC pid proton hypothesis.");
  Output(RO->TrCurvature,"TrCurvature","track curvature, units are 1/mm");
  Output(RO->TrCurvatureVar,"TrCurvatureVar",
    "variance in track direction vector, units are (1/mm)^2");
  Output(RO->TrDirection,"TrDirection","track direction vector");
  Output(RO->TrDirectionVar,"TrDirectionVar",
    "variance in track direction vector ");
}

TND280Track* TDisplayedTrackerModule::MakeTrack(TND280TrackPropagator* trkProp,
  Int_t index, ND::TTrackerReconModule::TTrackerResult* result){

  // Helper function creating TND280Track from T2K Tracker recon track.
  //
  TEveRecTrack* rt = new TEveRecTrack();;
  rt->fIndex = index;
  rt->fV.Set(result->Position.Vect());
  double Q = result->Charge;
  rt->fSign = (Q > 0) ? 1 : -1;
  double momentum = result->Momentum;
  TVector3 direction = result->Direction;
  TVector3 momentum3 = momentum*direction;
  rt->fP.Set(momentum3);

  TND280Track* track = new TND280Track(rt, trkProp);
  // Loop over all nodes in the track and add a path mark at each point
  //
  TClonesArray* Nodes = result->Nodes;
  int NNodes = result->NNodes;
  TEvePathMarkT<double>::EType_e mType = TEvePathMarkD::kReference;
  for(int node = 0; node < NNodes; node++){
    ND::TTrackerReconModule::TTrackerNode * Node =
      static_cast<ND::TTrackerReconModule::TTrackerNode*>(Nodes->At(node));
    if(node == (NNodes-1) ){ mType = TEvePathMarkD::kDecay; }
    track->AddPathMark(TEvePathMarkD(mType,
      LorentzToEve(Node->Position),
      Vector3ToEve((Node->Momentum)*(Node->Direction)),
      Node->Position.T()));
  }

  track->SetName(Form("Tracker track %d", rt->fIndex));
  track->SetStdTitle();
  track->SetSourceObject(result);
  track->SetMainColor(kYellow);
  track->SetLineWidth(4);
  track->MakeTrack();

  return track;
}

TND280Track* TDisplayedTrackerModule::MakeTrack(TND280TrackPropagator* trkProp,
  Int_t index, ND::TTrackerReconModule::TTrackerConstituent* constituent){
  // Helper function creating TND280Track from T2K Tracker recon constituent.
  //
  TEveRecTrack* rt = new TEveRecTrack();;
  rt->fIndex = index;
  rt->fV.Set(constituent->Position.Vect());
  double Q = constituent->Charge;
  rt->fSign = (Q > 0) ? 1 : -1;
  double momentum = constituent->Momentum;
  TVector3 direction = constituent->Direction;
  TVector3 momentum3 = momentum*direction;
  rt->fP.Set(momentum3);
  TND280Track* track = new TND280Track(rt, trkProp);
    // mark end of track
  track->AddPathMark(TEvePathMarkD(TEvePathMarkD::kDecay,
    LorentzToEve(constituent->BackPosition),
    Vector3ToEve((constituent->BackMomentum)*(constituent->BackDirection)),
    constituent->BackPosition.T()));

  track->SetName(Form("Tracker constituent %d", rt->fIndex));
  track->SetStdTitle();
  track->SetSourceObject(constituent);
  track->SetMainColor(kBlue);
  track->SetLineWidth(4);
  track->MakeTrack();
  return track;
}

TND280Track* TDisplayedTrackerModule::MakeTrack(TND280TrackPropagator* trkProp,
  Int_t index, ND::TTrackerReconModule::TTPCTrack* track){
  // Helper function creating TND280Track from T2K Tracker TPC track.
  //
  TEveRecTrack* rt = new TEveRecTrack();;
  rt->fIndex = index;
  rt->fV.Set(track->Position.Vect());
  double Q = track->Charge;
  rt->fSign = (Q > 0) ? 1 : -1;
  double momentum = track->Momentum;
  TVector3 direction=track->Direction;
  TVector3 momentum3 = momentum*direction;
  rt->fP.Set(momentum3);
  TND280Track* eveTrack = new TND280Track(rt, trkProp);
  // mark end of track ? how - there is no length or end position ???
  // choose an arbitary length
  eveTrack->AddPathMark(TEvePathMarkD(TEvePathMarkD::kDecay,
    Vector3ToEve(track->Position.Vect()+(500.0*track->Direction)),
    Vector3ToEve((track->Momentum)*(track->Direction)),
    track->Position.T()));
  eveTrack->SetName(Form("TPC Track  %d", rt->fIndex));
  eveTrack->SetStdTitle();
  eveTrack->SetSourceObject(track);
  eveTrack->SetMainColor(kGreen);
  eveTrack->SetLineWidth(4);
  eveTrack->MakeTrack();
  return eveTrack;
}

TND280Track* TDisplayedTrackerModule::MakeTrack(TND280TrackPropagator* trkProp,
  Int_t index, ND::TTrackerReconModule::TFGDTrack* track){
  // Helper function creating TND280Track from T2K Tracker TPC track.
  //
  TEveRecTrack* rt = new TEveRecTrack();;
  rt->fIndex = index;
  rt->fV.Set(track->Position.Vect());
  rt->fSign = 0;
  double momentum = 500.0;
  TVector3 direction = track->Direction;
  TVector3 momentum3 = momentum*direction;
  rt->fP.Set(momentum3);
  TND280Track* eveTrack = new TND280Track(rt, trkProp);
    // mark end of track ? how - there is no length or end position ???
    // choose an arbitary length
  eveTrack->AddPathMark(TEvePathMarkD(TEvePathMarkD::kDecay,
    Vector3ToEve(track->Position.Vect()+(500.0*track->Direction)),
    Vector3ToEve(momentum3),
    track->Position.T()));
  eveTrack->SetName(Form("FGD Track  %d", rt->fIndex));
  eveTrack->SetStdTitle();
  eveTrack->SetSourceObject(track);
  eveTrack->SetMainColor(kRed);
  eveTrack->SetLineWidth(4);
  eveTrack->MakeTrack();
  return eveTrack;
}

void TDisplayedTrackerModule::Describe(
  ND::TTrackerReconModule::TTrackerResult* RO){

  Output(RO->AlgorithmName,"AlgorithmName","The name of the algorithm that "
    "created this object.");
  Output(RO->Charge,"Charge","The Charge (+-1).");
  Output(RO->Chi2,"Chi2","The chi2 of the fit.");
  if(RO->NConstituents>0){
    Output(RO->ConstitIdx[0]," Index to first constituent. ","");
  }
  if(RO->NConstituents>1){
    Output(RO->ConstitIdx[1]," Index to second constituent. ","");
  }

  Output(RO->Detectors,"Detectors","Detectors used.");
  Output(RO->Direction,"Direction","track direction vector");
  Output(RO->DirectionVariance,"DirectionVariance","track direction variance");
  Output(RO->EDeposit,"EDeposit",
    "The deposited charge for the object (number of pe's).");
  Output(RO->isForward,"isForward","Sense of object.");
  Output(RO->Length,"Length","The total length of the object in mm.");
  Output(RO->Likelihoods,"Likelihoods",
    "the PID likelihoods for combined PID for now this is the product of the "
    "three TPC PID probabilities The first entry is the most likely hypothesis,"
    " followed by alternate hypotheses The Pid for the given likelihood is in "
    "the same index into the vectors.");
  Output(RO->Momentum,"Momentum","track momentum MeV/c");
  Output(RO->MomentumError,"MomentumError","track momentum MeV/c");
  Output(RO->NConstituents,"NConstituents","The number of constituents (tracks"
    " and pids) used to build this track.");
  Output(RO->NDOF,"NDOF","The number of degrees of freedom.");
  Output(RO->NFGDs,"NFGDs","Number of FGD Specific objects.");
  Output(RO->NHits,"NHits","The number of hits.");
  Output(RO->NNodes,"NNodes","The number of nodes (fgd hits + tpc tracks).");
  Output(RO->NTotalConstituents,"NTotalConstituents",
    "Number of all constituents, and constituents-constituents...");
  Output(RO->NTPCs,"NTPCs","Number of TPC tracks used to build this track.");
  Output(RO->Position,"Position","track position 4-vector (x,y,z,t) in mm, ns");
  Output(RO->Quality,"Quality",
    "The quality of the fit.(probability(chi2,ndof)).");
  Output(RO->Status,"Status","The status for the fit.");
  Output(RO->Variance,"Variance","track position variance 4-vector var(x),"
    "var(y),var(z),var(t) in mm^2, ns^2");

}

TND280Track* TDisplayedTrackerModule::Draw(
  ND::TTrackerReconModule::TTrackerConstituent* RO, int index,
  TClonesArray* Constituents){

  TND280Track* track = MakeTrack(fTrkProp,index,RO);

  if(RO->NConstituents > 0){
    ND::TTrackerReconModule::TTrackerConstituent* ConsIn =
    static_cast<ND::TTrackerReconModule::TTrackerConstituent*>(
      Constituents->At(RO->ConstitIdx[0]));
    track->AddElement(Draw(ConsIn, RO->ConstitIdx[0], Constituents));
    if(RO->NConstituents > 1){
      ND::TTrackerReconModule::TTrackerConstituent* ConsIn =
      static_cast<ND::TTrackerReconModule::TTrackerConstituent*>(
        Constituents->At(RO->ConstitIdx[1]));
      track->AddElement(Draw(ConsIn, RO->ConstitIdx[1], Constituents));
    }
  }
  return track;
}

void TDisplayedTrackerModule::Describe(
  ND::TTrackerReconModule::TTrackerConstituent* RO, TClonesArray* Constituents){

  Output(RO->AlgorithmName,"AlgorithmName",
    "algorithm that created this object.");
  Output(RO->BackDirection,"BackDirection",
    "The direction vector at the back of the track.");
  Output(RO->BackMomentum,"BackMomentum",
    "the momentum at the back of the track in MeV/c");
  Output(RO->BackPosition,"BackPosition",
    "The 4-vector position of the back of the track (x,y,z,t) in mm, ns.");
  Output(RO->BackVariance,"BackVariance",
    "The 4-vector position variance at the back of the track (var(x),var(y),"
    "var(z),var(t)) in mm^2, ns^2.");
  Output(RO->Charge,"Charge","The Charge of this constituent (+-1).");
  Output(RO->Chi2,"Chi2","The chi2 of the fit.");
  if(RO->NConstituents > 0){
    Output(RO->ConstitIdx[0]," Index to first constituent. ","");
  }
  if(RO->NConstituents > 1){
    Output(RO->ConstitIdx[1]," Index to second constituent. ","");
  }

  //Index into Constituents in TTrackerResult::Constituents of this
  //constituent's constituents.
  Output(RO->Detectors,"Detectors","Detectors used.");
  Output(RO->Direction,"Direction","direction vector");
  Output(RO->DirectionVariance,"DirectionVariance",
    "direction variance vector");
  Output(RO->EDeposit,"EDeposit",
    "The deposited charge for the constituent object (number of pe's).");
  Output(RO->FrontDirection,"FrontDirection",
    "The direction vector at the front of the track.");
  Output(RO->FrontMomentum,"FrontMomentum",
    "the momentum at the front of the track in MeV/c");
  Output(RO->FrontPosition,"FrontPosition",
    "The 4-vector position at the front of the track (x,y,z,t) in mm, ns.");
  Output(RO->FrontVariance,"FrontVariance",
    "The 4-vector position variance at the front of the track (var(x),var(y),"
    "var(z),var(t)) in mm^2, ns^2.");
  Output(RO->isForward,"isForward","Sense of track.");
  Output(RO->Momentum,"Momentum","momentum MeV/c");
  Output(RO->MomentumError,"MomentumError","uncertainty in momentum MeV/c");
  Output(RO->NConstituents,"NConstituents",
    "The number of constituents this constituent is made of.");
  Output(RO->NDOF,"NDOF","The number of degrees of freedom.");
  Output(RO->NHits,"NHits","The number of hits.");
  Output(RO->NNodes,"NNodes","The number of nodes.");
  Output(RO->Position,"Position","position 4-vector (x,y,z,t) in mm, ns");
  Output(RO->Quality,"Quality","The quality of the fit.(probability).");
  Output(RO->Status,"Status","The status for the fit.");
  Output(RO->Variance,"Variance",
    "position variance 4-vector (var(x),var(y),var(z),var(t)) in mm^2, ns^2 ");

  if( (RO->NConstituents > 0) && (Constituents != NULL) ){
    indent++;
    doIndent("+");
    std::cout << std::endl;
    std::cout << "First constituent at index " << RO->ConstitIdx[0]
      << std::endl;
    ND::TTrackerReconModule::TTrackerConstituent* ConsIn =
      static_cast<ND::TTrackerReconModule::TTrackerConstituent*>(
        Constituents->At(RO->ConstitIdx[0]));
    Describe(ConsIn,Constituents);
    divider();
    if(RO->NConstituents > 1){
      std::cout << "Second constituent at index " << RO->ConstitIdx[1]
        << std::endl;
      ND::TTrackerReconModule::TTrackerConstituent* ConsIn =
        static_cast<ND::TTrackerReconModule::TTrackerConstituent*>(
          Constituents->At(RO->ConstitIdx[1]));
      Describe(ConsIn,Constituents);
      divider();
    }
    doIndent("+");
    std::cout << std::endl;
    indent--;
  }
}
