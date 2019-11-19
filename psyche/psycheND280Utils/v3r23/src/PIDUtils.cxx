#include "PIDUtils.hxx"
#include "CutUtils.hxx"
#include "VersioningUtils.hxx"

const unsigned int NMAXPARTICLESWITHTPC = NMAXPARTICLES;

//********************************************************************
void anaUtils::RecomputeTPCPulls(AnaTPCParticleB &track) {
//********************************************************************

    // TODO
    (void)track;

    /*
       track.Pullmu  = ComputeTPCPull(track,"muon");
       track.Pullp   = ComputeTPCPull(track,"proton");
       track.Pullele = ComputeTPCPull(track,"electron");
       track.Pullpi  = ComputeTPCPull(track,"pion");
       track.Pullk   = ComputeTPCPull(track,"kaon");
       */
}

//********************************************************************
void anaUtils::ComputeTPCPull(const AnaTPCParticleB &track, Float_t* pulls) {
//********************************************************************

    pulls[0] = ((track.dEdxMeas - track.dEdxexpMuon) / track.dEdxSigmaMuon);
    pulls[1] = ((track.dEdxMeas - track.dEdxexpEle) / track.dEdxSigmaEle);
    pulls[2] = ((track.dEdxMeas - track.dEdxexpProton) / track.dEdxSigmaProton);
    pulls[3] = ((track.dEdxMeas - track.dEdxexpPion) / track.dEdxSigmaPion);
    
	
}

//********************************************************************
Float_t anaUtils::ExpectedTPCdEdx(const AnaTPCParticleB &track, const std::string& particle) {
//********************************************************************

    // for production 5
    Float_t ExpecteddEP0 = 149.4;
    Float_t ExpecteddEP1 = 2.765;
    Float_t ExpecteddEP2 = 0.103;
    Float_t ExpecteddEP3 = 2.052;
    Float_t ExpecteddEP4 = 0.5104;

    if (versionUtils::prod6_corrections){
      // for production 6
      ExpecteddEP0 = 53.87;
      ExpecteddEP1 = 5.551;
      ExpecteddEP2 = 0.001913;
      ExpecteddEP3 = 2.283;
      ExpecteddEP4 = 1.249;
    }

    Float_t mass = -1;

    if (particle == "electron") {
        mass = 0.511;
    } else if (particle == "pion") {
        mass = 139.57;
    } else if (particle == "muon") {
        mass = 105.66;
    } else if (particle == "proton") {
        mass = 938.27;
    } else if (particle == "kaon") {
        mass = 493.67;
    } else {
        std::cout << "Tried to compute dEdx for invalid particle hypothesis" << std::endl;
        return -10000;
    }

    Float_t bg = track.Momentum / mass;
    Float_t beta = bg / sqrt(1. + bg * bg);
    Float_t func = ExpecteddEP1 - pow(beta, ExpecteddEP3) - log(ExpecteddEP2 + 1. / pow(bg, ExpecteddEP4));
    func = func * ExpecteddEP0 / pow(beta, ExpecteddEP3);

    return func;
}

//********************************************************************
void anaUtils::GetPIDLikelihood(const AnaTrackB& track, Float_t* hypo, bool prod5Cut){
//********************************************************************

    UInt_t itrk = track.Index;

    if( itrk >= NMAXPARTICLESWITHTPC ) return; // Protection against values out of the vector. 

    Double_t prob[4]={1,1,1,1};
    Double_t tmp_prob[3][4];
    Double_t total_prob=0;
    bool found=false;

    AnaTPCParticleB* segmentsInTPC[3];
    for(int i = 0; i < 3; ++i){
        segmentsInTPC[i] = NULL;
        for (Int_t j=0;j<4;j++){
            hypo[j]=-1;
            tmp_prob[i][j] = 1;
        }
    }
    // Get the closest TPC. We should make sure that at least the segment in that TPC has the proper PID info
    SubDetId::SubDetEnum closesttpc = anaUtils::GetClosestTPC(track);

    // Loop over TPC segments
    for (int j = 0; j < track.nTPCSegments; ++j){      
        AnaTPCParticleB* TPCSegment = track.TPCSegments[j];
        if (!TPCSegment) continue;

        // Only segments passing the TPC track quality cut will contribute to the likelihood
        // Was not applied for production 5 BANFF analysis
        if(!prod5Cut) if (!cutUtils::TPCTrackQualityCut(*TPCSegment)) continue;

        // Require valid values for all quantities involved
        if( TPCSegment->dEdxexpMuon==-0xABCDEF || TPCSegment->dEdxexpEle==-0xABCDEF || TPCSegment->dEdxexpPion==-0xABCDEF || TPCSegment->dEdxexpProton==-0xABCDEF) continue;
        if( TPCSegment->dEdxMeas ==-0xABCDEF ) continue; 
        if( TPCSegment->dEdxexpMuon==-99999 || TPCSegment->dEdxexpEle==-99999 || TPCSegment->dEdxexpPion==-99999 || TPCSegment->dEdxexpProton==-99999) continue;
        if( TPCSegment->dEdxMeas ==-99999 ) continue; 

        Float_t pulls[4];
        // Pulls in order: Muon, Electron, Proton, Pion
        anaUtils::ComputeTPCPull(*TPCSegment,pulls);
        Float_t pullmu  = pulls[0];
        Float_t pullp   = pulls[2];
        Float_t pullele = pulls[1];
        Float_t pullpi  = pulls[3];
	
        if (!TMath::Finite(pullmu) || !TMath::Finite(pullele) || !TMath::Finite(pullp) || !TMath::Finite(pullpi)) continue;
        if (pullmu  != pullmu || pullele != pullele || pullp   != pullp || pullpi  != pullpi) continue;

        SubDetId::SubDetEnum det = SubDetId::GetSubdetectorEnum(TPCSegment->Detector);
        
        // To avoid mismatching between FlatTree and oaAnalysis we allow only one segment per TPC to be included in the likelihood, the one with more nodes
        if (segmentsInTPC[det-2]){
            if (TPCSegment->NNodes > segmentsInTPC[det-2]->NNodes){
                segmentsInTPC[det-2] = TPCSegment;
                tmp_prob[det-2][0] = exp(-(pullmu*pullmu)/2);
                tmp_prob[det-2][1] = exp(-(pullele*pullele)/2);
                tmp_prob[det-2][2] = exp(-(pullp*pullp)/2);
                tmp_prob[det-2][3] = exp(-(pullpi*pullpi)/2);
            }            
        }
        else{
            segmentsInTPC[det-2] = TPCSegment;      
            tmp_prob[det-2][0] = exp(-(pullmu*pullmu)/2);  
            tmp_prob[det-2][1] = exp(-(pullele*pullele)/2);
            tmp_prob[det-2][2] = exp(-(pullp*pullp)/2);    
            tmp_prob[det-2][3] = exp(-(pullpi*pullpi)/2);  
        }
    }

    // Loop over all segments contributing to the likelihood and compute it
    for (int tpc=0;tpc<3;tpc++){
        if (segmentsInTPC[tpc]){
            // The pull should be already corrected by all corrections (CT and CT expected)
            prob[0] *= tmp_prob[tpc][0]; 
            prob[1] *= tmp_prob[tpc][1];
            prob[2] *= tmp_prob[tpc][2];
            prob[3] *= tmp_prob[tpc][3];

            if (SubDetId::GetDetectorUsed(segmentsInTPC[tpc]->Detector, closesttpc)) found = true;
        }
    }

    // If at least the segment in the closest TPC has a  valid PID info
    if (found){
        for (int h=0;h<4;h++){
            total_prob += prob[h] ;
        }

        if (total_prob>0){
            for (int h=0;h<4;h++){
                hypo[h] = prob[h]/total_prob ;
            }
        }
    }
    return;
}

//********************************************************************
Float_t anaUtils::GetPIDLikelihood(const AnaTrackB& track, Int_t hypo, bool prod5Cut){
//********************************************************************

    if( hypo >= 4 ) return -1.e+6; 

    Float_t Likelihood[4];
    GetPIDLikelihood(track,Likelihood,prod5Cut);
    return Likelihood[hypo];
}

//********************************************************************
Float_t anaUtils::GetPIDLikelihoodMIP(const AnaTrackB &track) {
//********************************************************************

    Float_t Likelihood[4];
    GetPIDLikelihood(track,Likelihood);

    Float_t likemu = Likelihood[0];
    Float_t likepi = Likelihood[3];
    Float_t likep  = Likelihood[2];

    return (likemu+likepi)/(1-likep); 
}

//********************************************************************
Float_t anaUtils::GetPIDPrior(const AnaTrackB& track, Int_t hypo){
//********************************************************************

    // This function is not used yet

    Float_t xbins[18] = {0., 100., 200., 300., 400., 500., 600., 700, 800, 1000., 1200., 1400., 1700, 2000., 2500., 3000., 4000., 5000.};

    Float_t eprior[17] = {800., 250., 100,  40,    30,  25,   10,    5,    0,   0,     0,     0,     0,     0,     0,     0,     0}; 

    for (Int_t i=0;i<17;i++){
        eprior[i] /=400.;
    }

    Int_t pbin = 16;
    for (Int_t i=0;i<17;i++){
        pbin = i-1;
        if (track.Momentum>0 && track.Momentum < xbins[i]) break;
    }


    if (hypo==1)  return eprior[pbin];
    else return 1.;

}
