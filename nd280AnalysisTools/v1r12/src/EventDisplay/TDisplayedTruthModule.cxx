#include "TChain.h"
#include "TEveManager.h"
#include "TEveVSDStructs.h"

#include "oaAnalysisEnums.hxx"

#include "TOstreamOverloads.hxx"
#include "TDisplayedTruthModule.hxx"

TDisplayedTruthModule::TDisplayedTruthModule() :
  TDisplayedAnalysisModuleBase("TruthDir/Trajectories",
  "TruthDir/Vertices",
  "ND::TTruthTrajectoriesModule::TTruthTrajectory",
  "ND::TTruthVerticesModule::TTruthVertex"){

  fChain->SetBranchAddress("Trajectories", &fTracks);
  fChain->SetBranchAddress("NTraj", &fNTracks);
  fVertexChain->SetBranchAddress("Vertices", &fVertices);
  fVertexChain->SetBranchAddress("NVtx", &fNVertices);

  fChainGenieRooTracker = new TChain("TruthDir/GRooTrackerVtx");

  fChainNEUTRooTracker = new TChain("TruthDir/NRooTrackerVtx");
  GENIEUtils = new TGENIEAnalysisUtils(0,0);
  NEUTUtils = new TNEUTAnalysisUtils(0,0);

  fIsGENIE = 0;
  fIsNEUT = 0;
}

void TDisplayedTruthModule::AddFile(const char * inputfilename){
  fChain->AddFile(inputfilename);
  fVertexChain->AddFile(inputfilename);

  fChainGenieRooTracker->SetBranchAddress("NVtx", &fNVtxGenie);
  fVtxGenie = new TClonesArray("ND::GRooTrackerVtx", 50);
  fChainGenieRooTracker->SetBranchAddress("Vtx", &fVtxGenie);

  fChainNEUTRooTracker->SetBranchAddress("NVtx", &fNVtxNEUT);
  fVtxNEUT = new TClonesArray("ND::NRooTrackerVtx", 50);
  fChainNEUTRooTracker->SetBranchAddress("Vtx", &fVtxNEUT);

  fIsGENIE = fChainGenieRooTracker->AddFile(inputfilename);

  if(fIsGENIE){
    GENIEUtils->SetVerticesArray(fVtxGenie);
    GENIEUtils->SetVerticesNumber(&fNVtxGenie);
  }

  fIsNEUT = fChainNEUTRooTracker->AddFile(inputfilename);

  if(fIsNEUT){
    NEUTUtils->SetVerticesArray(fVtxNEUT);
    NEUTUtils->SetVerticesNumber(&fNVtxNEUT);
  }
}

void TDisplayedTruthModule::Draw(int entry,int bunch){

  TND280TrackList* gTrackList= PrepareToDraw(entry,"Truth Tracks");
  if(gTrackList == NULL){ return; }
  if(fIsGENIE){
    fChainGenieRooTracker->GetEntry(entry);
  }
  if(fIsNEUT){
   fChainNEUTRooTracker->GetEntry(entry);
  }

  // map from particle id to TND280Track object
  std::map<int,TND280Track*> EveList;
  // maps to tell whether a particle has been added to Eve
  std::map<int,bool> UsageList;
  TND280TrackPropagator* trkProp = gTrackList->GetPropagator();

    // First create Eve tracks and store them in map, indexed by ID
    for(int i = 0; i < fNTracks; i++){
      ND::TTruthTrajectoriesModule::TTruthTrajectory* trajectory =
        static_cast<ND::TTruthTrajectoriesModule::TTruthTrajectory*>(
          fTracks->At(i));
      if( (GetBunchMCTrue(trajectory->InitPosition.T()) != bunch) &&
          (bunch!=-2) ){
        continue;
      }

      TND280Track* track = MakeTrack(trkProp, i, trajectory);
      EveList[trajectory->ID] = track;
      UsageList[trajectory->ID] = false;
      track->MakeTrack(); // do this last!
    }

    for(int iv = 0; iv < fNVertices; iv++){

      ND::TTruthVerticesModule::TTruthVertex* Vertex =
        static_cast<ND::TTruthVerticesModule::TTruthVertex*>(fVertices->At(iv));
      if( (GetBunchMCTrue(Vertex->Position.T()) != bunch) && (bunch!=-2) ){
        continue;
      }

      TLorentzVector position = Vertex->Position;
      std::string generatorName = Vertex->Generator;

      TEveMCTrack*  rt = new TEveMCTrack();
      TLorentzVector p = Vertex->NeutrinoMomentum;
      rt->SetMomentum(p);
      int iStartVertex = Vertex->ID;

      TLorentzVector start = position;
      start.SetZ(-3900);
      if(fIsGENIE){
        ND::GRooTrackerVtx* GenieVtx = GENIEUtils->GetVertexById(iStartVertex);
        double* NuParentDecX4;
        NuParentDecX4=GenieVtx->NuParentDecX4;
      }
      if(fIsNEUT){
        ND::NRooTrackerVtx* NeutVtx = NEUTUtils->GetVertexById(iStartVertex);
        double* NuParentDecX4;
        NuParentDecX4=NeutVtx->NuParentDecX4;
      }

      rt->SetProductionVertex( start);

      rt->SetPdgCode(Vertex->NeutrinoPDG);
      TND280Track* Primary = new TND280Track(rt, trkProp);
      TEveVectorD EndV(position.X(),position.Y(),position.Z());
      double T = position.T();
      Primary->AddPathMark(TEvePathMarkD(TEvePathMarkD::kDecay,EndV,T));
      Primary->SortPathMarksByTime();
      Primary->SetIndex(iv);
      Primary->SetLabel(Vertex->ID);

      Primary->SetLineStyle(5);
      Primary->SetLineWidth(1);
      Primary->SetPdg(Vertex->NeutrinoPDG);
      Primary->SetMainColor(ColorBasedOnPDGValue(Vertex->NeutrinoPDG));
      TString Reaction=Vertex->ReactionCode;
      TString code(Reaction(0,2));
      if(Reaction.BeginsWith("-")){ code=Reaction(1,2); }
      if(code.IsDigit()){ NeutReaction(Reaction); }

      TString Name(Form("#%i : %s " ,iv, Reaction.Data()));
      Primary->SetElementName(Name);
      Primary->SetElementTitle(Name);

      Primary->SetSourceObject(Vertex);

      gTrackList->AddElement(Primary);
      Primary->MakeTrack();

      std::vector< Int_t > PrimaryTrajIDs = Vertex->PrimaryTrajIDs;
     for(int i = 0; i < Vertex->NPrimaryTraj; i++){
       int DaughterID=PrimaryTrajIDs[i];
       if(EveList.find(DaughterID) != EveList.end()){
        Primary->AddElement(EveList[DaughterID]);
        UsageList[DaughterID] = true;
      }
    }
  }

  for(int i = 0; i < fNTracks; i++){

    ND::TTruthTrajectoriesModule::TTruthTrajectory* trajectory =
      static_cast<ND::TTruthTrajectoriesModule::TTruthTrajectory*>(
        fTracks->At(i));

    if( (GetBunchMCTrue(trajectory->InitPosition.T()) != bunch) &&
        (bunch!=-2) ){
      continue;
    }

    if(!UsageList[trajectory->ID]){

      if( (trajectory->ParentID == 0) ||
          (EveList.find(trajectory->ParentID) == EveList.end()) ){

        gTrackList->AddElement(EveList[trajectory->ID]);
      } else {

        EveList[trajectory->ParentID]->AddElement(EveList[trajectory->ID]);
      }
    }
  }

  if(gTrackList->HasChildren()){ gEve->AddElement(gTrackList); }

}

TND280Track* TDisplayedTruthModule::MakeTrack(TND280TrackPropagator* trkProp,
  Int_t index, ND::TTruthTrajectoriesModule::TTruthTrajectory* trajectory){
  // Helper function creating TND280Track from T2K Truth recon track.
  //
  TEveMCTrack* rt=new TEveMCTrack();;
  TLorentzVector p = trajectory->InitMomentum;
  rt->SetMomentum(p);
  rt->SetProductionVertex(trajectory->InitPosition);

  rt->SetPdgCode(trajectory->PDG);
  TND280Track* track = new TND280Track(rt, trkProp);
  track->SetName(TString::Format("%d : %s",index,(trajectory->Name).c_str()));

  for(size_t step = 1; step < trajectory->TraceSubdetectors.size(); step++) {

    if(trajectory->TraceEntranceMomentum[step].Mag() > 1E-6){
      track->AddPathMark(TEvePathMarkD(TEvePathMarkD::kReference,
        LorentzToEve(trajectory->TraceEntrancePosition[step]),
        Vector3ToEve(trajectory->TraceEntranceMomentum[step]),
        trajectory->TraceEntrancePosition[step].T()));
    }

    float diff =
      (trajectory->TraceExitPosition[step].Vect() -
        trajectory->TraceEntrancePosition[step].Vect()).Mag();
    if( (diff>1E-4) && (step != size_t(trajectory->TraceSubdetectors.back())) ){

      track->AddPathMark(TEvePathMarkD(TEvePathMarkD::kReference,
        LorentzToEve(trajectory->TraceExitPosition[step]),
        Vector3ToEve(trajectory->TraceExitMomentum[step]),
        trajectory->TraceExitPosition[step].T()));
    }

  }

  TLorentzVector ev = trajectory->FinalPosition;
  TEveVectorD EndV(ev.X(),ev.Y(),ev.Z());
  double T = trajectory->FinalPosition.T();

  track->AddPathMark(TEvePathMarkD(TEvePathMarkD::kDecay,EndV,T));
  track->SortPathMarksByTime();
  track->SetIndex(index);
  track->SetLabel(trajectory->ID);
  track->SetMainColor(ColorBasedOnPDGValue(trajectory->PDG));
  track->SetLineWidth(2);
  track->SetSourceObject(trajectory);
  if(trajectory->Charge == 0){
    track->SetLineStyle(5);
    track->SetLineWidth(1);
  }
  track->SetPdg(trajectory->PDG);
  track->SetTitle(Form("True trajectory %d : ID %d : pdg code %d ( %s )",
    index, trajectory->ID, trajectory->PDG, trajectory->Name.c_str()));
  return track;
}

void TDisplayedTruthModule::Describe(int index){

  std::cout << "This is Truth track number:" << index << endl;

  ND::TTruthTrajectoriesModule::TTruthTrajectory* T =
    static_cast<ND::TTruthTrajectoriesModule::TTruthTrajectory*>(
      fTracks->At(index));
  Describe(T);
}

void TDisplayedTruthModule::Describe(
  ND::TTruthTrajectoriesModule::TTruthTrajectory* T){

  std:: cout << " ID : " << T->ID << ", PDG code:  " << T->PDG << " name :"
    << T->Name << " parent id: " << T->ParentID
    << " ID of primary : " << T->PrimaryID<< std::endl
    << " Initial Momentum " << T->InitMomentum
    << " ( magnitude : " << T->InitMomentum.Vect().Mag() << " )" << std::endl
    << " Initial Position " << T->InitPosition<< std::endl
    << " Final Position " << T->FinalPosition<< std::endl;

  Centre(T->InitPosition, T->FinalPosition);
  bool contained = true;
  for(int i = 0; i < 10; i++){
    //Just waiting for us to redefine the ESubdetector enum to be iterable.
    oaAnalysisEnums::ESubdetector sdet =
      static_cast<oaAnalysisEnums::ESubdetector>(i);
    if(TTruthUtils::EnteredSubdetector(T,sdet) ||
      TTruthUtils::ExitedSubdetector(T,sdet) ){
      contained = false;
    }
  }
  if(contained){
   std::cout << "It started and ended in the same detector. " << std::endl;
  } else {
    std::cout<<" It entered or exited the following subdetectors: "<< std::endl;
    for(int i = 0; i < 10; i++){
      oaAnalysisEnums::ESubdetector sdet =
        static_cast<oaAnalysisEnums::ESubdetector>(i);
      if(TTruthUtils::EnteredSubdetector(T,sdet)){
        std::cout << " Entered " << DetectorName(i) << std::endl;
      }
      if(TTruthUtils::ExitedSubdetector(T,sdet)){
        std::cout << "Exited " << DetectorName(i) << std::endl;
      }
    }
  }
  if(T->ParentID != 0){
    bool found = false;
    for(int i = 0; i < fNTracks; i++){
      ND::TTruthTrajectoriesModule::TTruthTrajectory* trajectory =
        static_cast<ND::TTruthTrajectoriesModule::TTruthTrajectory*>(
          fTracks->At(i));
      if(T->ParentID == trajectory->ID){
        std::cout << std::endl << "Parent of this particle is... " << std::endl
        << std::endl;
        Describe(trajectory);
        found = true;
      }
    }
    if(!found){
      std::cout << std::endl << "Parent trajectory not found on file, may have"
      " been removed to save space." << std::endl;
    }

  } else {
    std::cout << std::endl << " This particle came from a neutrino interaction "
    "vertex " << std::endl;
  }
}

void TDisplayedTruthModule::Describe(ND::TTruthVerticesModule::TTruthVertex* V){
  Centre(V->Position);
  std::cout << "Neutrino interaction vertex " << std::endl;
  Output(V->Fiducial,"Fiducial",
    "The distance inside the local fiducial volume [mm].");
  Output(V->Generator,"Generator",
    "The generator that created the vertex.");
  Output(V->ID,"ID",
    "A number which uniquely identifies this vertex within the event.");
  Output(V->NeutrinoMomentum,"NeutrinoMomentum",
    "The four-momentum of the incoming neutrino.");
  Output(V->NeutrinoPDG,"NeutrinoPDG",
    "The PDG number of the incoming neutrino.");
  Output(V->NPrimaryTraj,"NPrimaryTraj",
    "The number of primary trajectories (ie: the number of primary particles "
    "exiting the interaction vertex).");
  Output(V->Position,"Position","Position and time of the vertex.");
  Output(V->ReactionCode,"ReactionCode",
    "The Reaction code according to the generator For Genie this will be of the"
    " form: \"nu:14;tgt:1000260560;N:2112;pVc:Weak[CC],QES;\" For Neut it will "
    "be an integer, see definitions here: http://www.t2k.org/asg/xsec/niwgdocs"
    "/neut_xsecs/neutmodesC.h/view.");
  Output(V->Subdetector,"Subdetector",
    "Subdetector which the vertex occurs in.");
  Output(V->TargetMomentum,"TargetMomentum","The four-momentum of the target.");
  Output(V->TargetPDG,"TargetPDG",
    "The (extended for nuclei) PDG number of the target. ");
}

void TDisplayedTruthModule::NeutReaction(TString& Reaction){
  int code = Reaction.Atoi();
  switch (code){
    case 1:{
      Reaction = Reaction + " : "
        "Charged Current Elastic; neutrino,n -> lepton-,p";
      break;
    }
    case 11:{
      Reaction = Reaction + " : "
        "Charged current ; neutrino,p -> lepton-,p,pi+";
      break;
    }
    case 12:{
      Reaction = Reaction + " : "
        "Charged  current ; neutrino,n -> lepton-,p,pi0";
      break;
    }
    case 13:{
      Reaction = Reaction + " : "
        "Charged  current ; neutrino,n -> lepton-,n,pi+";
      break;
    }
    case 16:{
      Reaction = Reaction + " : "
        "Charged  current ; neutrino,O(16) -> lepton-,O(16),pi+";
      break;
    }
    case 17:{
      Reaction = Reaction + " : "
        "Charged  current ; neutrino,n -> lepton-,p,gamma";
      break;
    }
    case 21:{
      Reaction = Reaction + " : "
        "Charged  current ; neutrino,(n or p) -> lepton-,(n or p),multi pi";
      break;
    }
    case 22:{
      Reaction = Reaction + " : "
        "Charged  current ; neutrino,n -> lepton-,p,eta0";
      break;
    }
    case 23:{
      Reaction = Reaction + " : "
        "Charged  current ; neutrino,n -> lepton-,lambda,K+";
      break;
    }
    case 26:{
      Reaction = Reaction + " : "
        "Charged  current ; neutrino,(n or p) -> lepton-,(n or p),mesons";
      break;
    }
    case 31:{
      Reaction = Reaction + " : "
        "Neutral current; neutrino,n -> neutrino,n,pi0";
      break;
    }
    case 32:{
      Reaction = Reaction + " : "
        "Neutral current; neutrino,p -> neutrino,p,pi0";
      break;
    }
    case 33:{
      Reaction = Reaction + " : "
        "Neutral current; neutrino,n -> neutrino,p,pi-";
      break;
    }
    case 34:{
      Reaction = Reaction + " : "
        "Neutral current; neutrino,p -> neutrino,n,pi+";
      break;
    }
    case 36:{
      Reaction = Reaction + " : "
        "Neutral current; neutrino,O(16) -> neutrino,O(16),pi0";
      break;
    }
    case 38:{
      Reaction = Reaction + " : "
        "Neutral current; neutrino,n -> neutrino,n,gamma";
      break;
    }
    case 39:{
      Reaction = Reaction + " : "
        "Neutral current; neutrino,p -> neutrino,p,gamma";
      break;
    }
    case 41:{
      Reaction = Reaction + " : "
        "Neutral current; neutrino,(n or p) -> neutrino,(n or p),multi pi";
      break;
    }
    case 42:{
      Reaction = Reaction + " : "
        "Neutral current; neutrino,n -> neutrino,n,eta0";
      break;
    }
    case 43:{
      Reaction = Reaction + " : "
        "Neutral current; neutrino,p -> neutrino,p,eta0";
      break;
    }
    case 44:{
      Reaction = Reaction + " : "
        "Neutral current; neutrino,n -> neutrino,lambda,K0";
      break;
    }
    case 45:{
      Reaction = Reaction + " : "
        "Neutral current; neutrino,p-> neutrino,lambda,K+";
      break;
    }
    case 46:{
      Reaction = Reaction + " : "
        "Neutral current; neutrino,(n or p) -> neutrino,(n or p),mesons";
      break;
    }
    case 51:{
      Reaction = Reaction + " : "
        "Neutral current; neutrino,p -> neutrino,p";
      break;
    }
    case 52:{
      Reaction = Reaction + " : "
        "Neutral current; neutrino,n -> neutrino,n";
      break;
    }

    case -1:{
      Reaction = Reaction + " : "
        "Charged Current Elastic; anti-neutrino,p -> lepton+,n";
      break;
    }
    case -11:{
      Reaction = Reaction + " : "
        "Charged current ; anti-neutrino,n -> lepton+,n,pi+";
      break;
    }
    case -12:{
      Reaction = Reaction + " : "
        "Charged  current ; anti-neutrino,p -> lepton+,n,pi0";
      break;
    }
    case -13:{
      Reaction = Reaction + " : "
        "Charged  current ; anti-neutrino,p -> lepton+,p,pi+";
      break;
    }
    case -16:{
      Reaction = Reaction + " : "
        "Charged  current ; anti-neutrino,O(16) -> lepton+,O(16),pi-";
      break;
    }
    case -17:{
      Reaction = Reaction + " : "
        "Charged  current ; anti-neutrino,p -> lepton+,n,gamma";
      break;
    }
    case -21:{
      Reaction = Reaction + " : "
        "Charged  current ; anti-neutrino,(n or p) -> lepton+,(n or p),"
        "multi pi";
      break;
    }
    case -22:{
      Reaction = Reaction + " : "
        "Charged  current ; anti-neutrino,p -> lepton+,n,eta0";
      break;
    }
    case -23:{
      Reaction = Reaction + " : "
        "Charged  current ; anti-neutrino,p -> lepton+,lambda,K0";
      break;
    }
    case -26:{
      Reaction = Reaction + " : "
        "Charged  current ; anti-neutrino,(n or p) -> lepton+,(n or p),mesons";
      break;
    }
    case -31:{
      Reaction = Reaction + " : "
        "Neutral current; anti-neutrino,n -> anti-neutrino,n,pi0";
      break;
    }
    case -32:{
      Reaction = Reaction + " : "
        "Neutral current; anti-neutrino,p -> anti-neutrino,p,pi0";
      break;
    }
    case -33:{
      Reaction = Reaction + " : "
        "Neutral current; anti-neutrino,n -> anti-neutrino,p,pi-";
      break;
    }
    case -34:{
      Reaction = Reaction + " : "
        "Neutral current; anti-neutrino,p -> anti-neutrino,n,pi+";
      break;
    }
    case -36:{
      Reaction = Reaction + " : "
        "Neutral current; anti-neutrino,O(16) -> anti-neutrino,O(16),pi0";
      break;
    }
    case -38:{
      Reaction = Reaction + " : "
        "Neutral current; anti-neutrino,n -> anti-neutrino,n,gamma";
      break;
    }
    case -39:{
      Reaction = Reaction + " : "
        "Neutral current; anti-neutrino,p -> anti-neutrino,p,gamma";
      break;
    }
    case -41:{
      Reaction = Reaction + " : "
        "Neutral current; anti-neutrino,(n or p) -> anti-neutrino,(n or p),"
        "multi pi";
      break;
    }
    case -42:{
      Reaction = Reaction + " : "
        "Neutral current; anti-neutrino,n -> anti-neutrino,n,eta0";
      break;
    }
    case -43:{
      Reaction = Reaction + " : "
        "Neutral current; anti-neutrino,p -> anti-neutrino,p,eta0";
      break;
    }
    case -44:{
      Reaction = Reaction + " : "
        "Neutral current; anti-neutrino,n -> anti-neutrino,lambda,K0";
      break;
    }
    case -45:{
      Reaction = Reaction + " : "
        "Neutral current; anti-neutrino,p-> anti-neutrino,lambda,K+";
      break;
    }
    case -46:{
      Reaction = Reaction + " : "
        "Neutral current; anti-neutrino,(n or p) -> anti-neutrino,(n or p),"
        "mesons";
      break;
    }
    case -51:{
      Reaction = Reaction + " : "
        "Neutral current; anti-neutrino,p -> anti-neutrino,p";
      break;
    }
    case -52:{
      Reaction=Reaction + " : "
        "Neutral current; anti-neutrino,n -> anti-neutrino,n";
      break;
    }
  }
}
