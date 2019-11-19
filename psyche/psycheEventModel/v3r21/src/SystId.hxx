#ifndef SystId_hxx_seen
#define SystId_hxx_seen


namespace SystId{

  enum SystEnum {
    // Momentum related systematics
    kBFieldDist=0,
    kMomScale,      
    kMomResol,
    kMomRangeResol,
    kMomBiasFromVertexMigration, 
    // Charge and PID identification 
    kTpcPid,        
    kFgdPid,        
    kChargeIDEff,    
    // Efficiencies for TPC tracking
    kTpcClusterEff,     
    kTpcTrackEff,
    // TPC matching
    kTpcFgdMatchEff,
    kTpcECalMatchEff, 
    kTpcP0dMatchEff, 
    // FGD reconstruction efficiencies
    kFgdTrackEff,       
    kFgdHybridTrackEff, 
    kMichelEleEff,          
    // Pile-up, tracker
    kPileUp,
    // Detector mass, normalization
    kFgdMass,
    // OOFV
    kOOFV, 
    // GEANT 4 model
    kSIPion,
    kSIProton,
    // Effects from sand-muons, tracker
    kSandMu,
    // ECal
    kECalEMResol,
    kECalEMScale,
    kECalTrackEff,
    kECalPID, 
    // High-angle matching
    kFgdECalMatchEff, 
    kFgdECalSmrdMatchEff,
    // ToF
    kToFResol, 
    // Flux
    kFluxWeight,      
    // NuE analysis specific
    kECalEmHipPID,
    kFGD2Shower,
    kNuETPCPileUp,
    kNuEP0DPileUp,
    kNuEECalPileUp,
    kNuEOOFV,
    // P0D+Tracker specific
    kP0DELossScale,
    kP0DELossResol,
    kP0DVeto,
    SystEnumLast_SystId
  };
}

#endif
