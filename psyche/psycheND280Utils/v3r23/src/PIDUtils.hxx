#ifndef PIDUtils_h
#define PIDUtils_h

#include "ConstituentsUtils.hxx"

/// This namespace contains useful functions for analyses. Functions include
/// those related to fiducial volumes, accessing tracks with specific
/// characteristics, and more.
namespace anaUtils{

    //----- Utility functions -------------

    /// Function to recompute the pull for a TPC track segment
    Float_t ComputeTPCPull(const AnaTPCParticleB &track, const std::string& particle);

    /// Function to recompute the pull for a TPC track segment for all hypotheses
    void ComputeTPCPull(const AnaTPCParticleB &track, Float_t* pulls);

    /// Function to recompute all the pull for a TPC track segment and save them into the segment
    void RecomputeTPCPulls(AnaTPCParticleB &track);

    /// Compute the expected TPC dEdx for a track, using a different
    /// parameterisation than the one in tpcRecon.
    Float_t ExpectedTPCdEdx(const AnaTPCParticleB &track, const std::string& particle);

    /// The likelihood of a track being a given particle hypothesis, based on the
    /// pull values of all TPC segments in the track.
    ///
    /// hypo is one of:
    /// * 0 - muon
    /// * 1 - electron
    /// * 2 - proton
    /// * 3 - pion
    Float_t GetPIDLikelihood(const AnaTrackB& track, Int_t hypo, bool prod5Cut = 0);

    /// Get all likelihoods
    void GetPIDLikelihood(const AnaTrackB&, Float_t* hypo, bool prod5Cut = 0);

    /// Get the likelihood for MIP: (like_mu+like_pi)/(1-like_p)
    Float_t GetPIDLikelihoodMIP(const AnaTrackB &track);

    /// A function that is not currently used, and will be documented when it is.
    Float_t GetPIDPrior(const AnaTrackB& track, Int_t hypo);
}
#endif
