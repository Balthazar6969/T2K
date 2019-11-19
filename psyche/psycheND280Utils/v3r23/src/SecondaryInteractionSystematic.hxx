#ifndef SecondaryInteractionSystematic_h
#define SecondaryInteractionSystematic_h

#include "AnalysisUtils.hxx"

#include "HEPConstants.hxx"
#include "Units.hxx"

//For geometry stuff.
#include "GeometryManager.hxx"
#include <TGeoManager.h>
#include <TGeoNode.h>
#include <TGeoVolume.h>
#include <TGeoMaterial.h>
#include "ParticleId.hxx"
#include "CoreUtils.hxx"

#include <string>
#include <set>

/// A general class to do various business related to secondary interactions: retrieve particles of intereset
/// and




namespace si_syst{
  
  // **********
  /// Constants
   
  /// The magnetic field is 0.2 T in the x direction --> used for stepping --> keep it general
  const TVector3 field = TVector3(0.2, 0., 0.);
    
  /// Bethe-Bloch constants
  const Double_t K = 0.307075;
  
  // ********** 
  
  
  // ********** 
  /// Enums
  
  /// Interaction types,  keep a wide list here, one can add what needed
  enum InteractionType{
    kNoInteraction = 0, // particle does not interact in the volume of interest
    kElastic, 
    kInelastic 
  };
  
  
  /// Enumerate various materials of interest
  /// Will be moved to secondary interactions syst...
  enum MaterialEnum{
    kCarbon = 0,  //  probably better to use exact Z values, but want to be able to do simple loops etc      
    kOxygen,        
    kAluminium,     
    kIron,          
    kAlG10,         
    kWaterSystem,
    kG10,
    kFGDGlue,
    kG10Roha, 
    kCounter
  };
  // ********** 
  
  
  // ********** 
  /// General kinematics methods
  /// ZoverA, material properties retrieved from node
  Double_t GetZoverAMaterial(TGeoNode*);
  
  /// Calculates the effective mean excitation energy for
  /// Equation taken from Leo p.29.
  /// Material properties retrieved from node
  Double_t GetIMaterial(TGeoNode* node);

  /// Takes in an atomic number and returns the Mean Ionization Potential for
  /// that element, from http://physics.nist.gov/PhysRefData/XrayMassCoef/tab1.html
  /// Only filled for elements present in ND280UserDetectorConstruction.cc
  /// Uses units of eV.  Must be converted to MeV later.
  Double_t GetIElement(Int_t Z);
  
  
  /// Get the material enum given a material name
  MaterialEnum GetMaterialEnum(const std::string&);
  
  // ********** 
 
  // ********** 
  /// A simple structure to represent a state object: position, direction, momentum, charge
  struct ParticleState{
    
    ParticleState(){}
    
    ParticleState(const AnaTrueParticleB& track){
      Pos      = anaUtils::ArrayToTLorentzVector(track.Position).Vect();
      Dir      = anaUtils::ArrayToTVector3(track.Direction);
      Charge   = track.Charge;
      Momentum = track.Momentum;
      Mass     = units::pdgBase->GetParticle(track.PDG)->Mass() * units::GeV;
    }
    
    TVector3 Pos;
    TVector3 Dir;
    Float_t  Charge;
    Float_t  Momentum;
    Float_t  Mass;
    
    
  };
  // ********** 
  
  // ********** 
  /// Propagation
  /// Bethe-Bloch dEdX given particle state + TGeoNode (the particle is currently crossing) to specify material properties
  Double_t BetheBloch(const ParticleState&);
  
  /// Bethe-Bloch dEdX given particle state + TGeoNode (the particle is currently crossing) to specify material properties
  /// this is not to read the node if already avaiable
  Double_t BetheBloch(const ParticleState&, TGeoNode*);
 
  /// Propagate a state in a small step
  void TakeSmallStep(ParticleState&, const Double_t&);
  
  /// Propagate a state in a small step provided the node (material properties) and a step
  /// this is not to read the node if already avaiable
  void TakeSmallStep(ParticleState&, const Double_t&, TGeoNode*);
   
  // ********** 
    
  // **********  
  /// A simple structure to repesent a pair: xsection value and its uncertainty
  struct SIXSecData{
    
    SIXSecData(Double_t xsec, Double_t err){
      XSec    = xsec;
      XSecErr = err;
    }
    Float_t XSec;
    Float_t XSecErr;
 
  };
  // ********** 
  
 

    
  // ********** 
  /// A class to store all the info of a true particle relevant for SI systematics evaluation
  /// Includes also weights for possible MC re-weighting
  class ParticleHistory{
    
    public:
      /// Each step is charactarized with the corresponding n*sigma*step_length and uncertainty value (!)
      /// various channels can be involved --> hence a vector
      typedef std::vector<SIXSecData> StepInfo;
      
      /// A default ctor
      ParticleHistory(): 
        XSecData(0, 0), 
        XSecDataReference(0, 0){}
      
      virtual ~ParticleHistory(){}
      
      
      /// A ctor given the interaction type and true particle
      ParticleHistory(const  InteractionType& type, const AnaTrueParticleB& track):
        XSecData(0, 0), 
        XSecDataReference(0, 0){
          intType      = type;
          stateStart   = ParticleState(track);
          trueTrack    = &track; 
          Reset();
      }

      ///  Dump the history     
      void Print(){}
     
      /// Reset the history of the particle
      void Reset(){
        // Reset the current state
        stateCurrent = stateStart;
        
        // Clear the steps
        propSteps.clear();
        propSteps.reserve(200);
               
        // Clear the reference steps
        propStepsReference.clear();
        propStepsReference.reserve(200);
        
        // Reset the xsec data
        XSecData          = si_syst::SIXSecData(0., 0.);
        
        // Reset the reference xsec data
        XSecDataReference = si_syst::SIXSecData(0., 0.);
        
        Weight = 1.;

        
      }
       
      /*!
       * Interaction type
       */
      InteractionType intType;
     
      /// Initial state of the particle
      ParticleState stateStart;
      
      /// Current state of the particle
      ParticleState stateCurrent;
       
      /// A pointer to the original true track
      const AnaTrueParticleB* trueTrack;
            
      /// Vector of steps considered with the corresponding info
      std::vector<StepInfo> propSteps;
      
      /// Vector of steps considered with the corresponding info
      /// but with reference cross-section,  this is to calculate the weight
      std::vector<StepInfo> propStepsReference;
 
      /// Store both the cross-section and error
      si_syst::SIXSecData XSecData;
      
      /// Store data for reference cross-section
      si_syst::SIXSecData XSecDataReference;
      
      /// Weight for this particle, to be applied to correspond to a reference cross-section
      Float_t Weight; 
         
  };
 
  // ********** 
  
  
  // ********** 
  /// Basic structures to be able to associate a track with its "relatives"
  typedef std::vector<std::pair<AnaTrueParticleB*, std::vector<AnaTrueParticleB*> > > ParentsAndDaughters;
  typedef std::vector<std::pair<AnaTrueParticleB*, std::vector<AnaTrueParticleB*> > >::iterator ParentsAndDaughtersIt;
  // ********** 
  


  // ********** 
  /// A simple class to provide the data for the systematic itself: a collection of particles with the relevant history
  class SISystInput{

  public:

    int nParticles;

    ParticleHistory* Particles;

    SISystInput(){
      nParticles = 0;
      Particles  = NULL;
    }

    virtual ~SISystInput(){
      if (Particles)
        delete [] Particles;
      Particles   = NULL;
      nParticles  = 0;
    
    }

  };
  // ********** 

  
  // ********** 
  /// A class that does the actual propagation of a true track, taking into account and storing the xsec
  /// values and uncertainties
  class ParticleSIPropagator{
  public:
    
    ParticleSIPropagator(){
      _initialized = false;
      
      _det = SubDetId::kInvalid;
      
      // No external weights by default
      _computeReWeightInfo = false;   
    }
   
    virtual ~ParticleSIPropagator(){}
    
    
    /// Initialization
    void Initialize(){
      if (_initialized) return;
      SetParameters();
      _initialized = true;
    }

    
    /// Propagates a particle to a given pos filling all the history info
    virtual void Propagate(ParticleHistory&, const TVector3&) const;
    
    /// Whether the propagator was initialized
    bool IsInitialized(){return _initialized;}
    
    
    /// Is a point inside a volume of interest: the region where the propagation is relevant
    virtual Bool_t InVOI(const TVector3&) const = 0;
   
   
    
    
    /// The function that gives a x-section value given a channel, momentum value, and node 
    /// + particle type so to make it fully general
    virtual Double_t GetCrossSection(const si_syst::InteractionType&, const Float_t&, const Int_t&, TGeoNode*) const = 0;
    
    
    /// Same but given the state
    Double_t GetCrossSection(const si_syst::InteractionType& type, const ParticleState& state, const Int_t& PDG){
     
     TGeoNode *node = ND::hgman().GeoManager()->FindNode(state.Pos.X(), state.Pos.Y(), state.Pos.Z());
     
     return GetCrossSection(type, state.Momentum, PDG, node);

   }
   
    /// The function that gives a x-section value` uncertainty given a channel, momentum value and 
    /// + particle type so to make it fully general
    virtual Double_t GetCrossSectionError(const si_syst::InteractionType&, const Float_t&, const Int_t&, TGeoNode*) const = 0;
        
    /// Same but given the state
    Double_t GetCrossSectionError(const si_syst::InteractionType& type, const ParticleState& state, const Int_t& PDG){
    
     TGeoNode *node = ND::hgman().GeoManager()->FindNode(state.Pos.X(), state.Pos.Y(), state.Pos.Z());
     
     return GetCrossSectionError(type, state.Momentum, PDG, node);
   }
    
    /// If one wants to re-weight to a given cross-section
    /// Default implementation returns zero
    virtual Double_t GetReferenceCrossSection(const si_syst::InteractionType&, const Float_t&, const Int_t&, TGeoNode*) const{
      std::cout << " si_syst::ParticleSIPropagator::GetReferenceCrossSection() - the function has to be properly implemented " <<
        " in the derived class if the re-weighting is needed " << std::endl;
      return 0.;
    }
    
    /// Same but given the state
    Double_t GetReferenceCrossSection(const si_syst::InteractionType& type, const si_syst::ParticleState& state, const Int_t& PDG){

      TGeoNode *node = ND::hgman().GeoManager()->FindNode(state.Pos.X(), state.Pos.Y(), state.Pos.Z());

      return GetReferenceCrossSection(type, state.Momentum, PDG, node);
    }
   
    /// Get interaction types
    const std::set<si_syst::InteractionType>& GetInteractionTypes() const{
      return _intTypes;
    }
    
    /// Get particle PDGs
    const std::set<ParticleId::PdgEnum>& GetParticlePDGs() const{
      return _particlePDGs;
    }
    
    /// Add an interaction type
    void AddInteractionType(si_syst::InteractionType type){
      _intTypes.insert(type);
    } 
    
    /// Add particle PDG
  void AddParticlePDG(ParticleId::PdgEnum pdg){
      _particlePDGs.insert(pdg);
    }
    
    /// Set detector of interest
    void SetDetector(const SubDetId::SubDetEnum& det){
      _det = det;
    } 
    
    /// Get detector of interest
    SubDetId::SubDetEnum GetDetector() const{
      return _det;
    } 
    
    
    /// Whether to calculate correction weight, w.r.t to reference cross-section 
    void SetComputeReWeightStatus(bool status){
      _computeReWeightInfo = status; 
    }   
   
    /// Whether to calculate correction weight, w.r.t to reference cross-section 
    bool GetComputeReWeightStatus() const{
      return _computeReWeightInfo;
    }   
    
    
  protected: 
      
    // Set the parameters for propagator: step length, momentum bins,  relevant particles, 
    // whatever needed,  is called in initialization
    virtual void SetParameters() = 0; 
     
    /// Density of scattering centers: n/cm^3
    /// keep virtual if need fine tuning material based
    virtual Double_t DScattCenters(TGeoNode*) const; 
    
    /// The step size in terms of length
    Double_t _lengthStepSize;
    
    /// The step size in terms of monetum
    Double_t _momStepSize; 
    
    /// Relevant interaction types to be considered while propagating
    std::set<si_syst::InteractionType> _intTypes;
     
    /// PDG of relevant particles,  keep here for the moment
    std::set<ParticleId::PdgEnum> _particlePDGs; 
    
    /// Keep the relevant sub-detector here for the moment
    SubDetId::SubDetEnum _det;
    
    
    /// Whether to caculate weight for re-weighting (using the reference cross-section)
    bool _computeReWeightInfo;
  
  private:
    bool _initialized;
  };
  
    
  /// Manager 
  class ParticleSIManager{
  
  public:
    
    /// ctor
    ParticleSIManager(){
      _propagator   = NULL;
    }
    
    /// dtor
    virtual ~ParticleSIManager(){
      if (_propagator)
        delete _propagator;
      _propagator = NULL;  
    }
    
    /// Set the propagator
    virtual void SetPropagator(ParticleSIPropagator* propagator){
      
      if (_propagator)
        delete _propagator;
      
      _propagator = propagator;
      _propagator->Initialize();
    }
    
    /// Get propagator
    si_syst::ParticleSIPropagator* GetPropagator() const{
      return _propagator;
    } 
     
    /// Calculates the information needed to compute an event weight
    si_syst::SISystInput* CollectParticleInteractions(AnaTrueParticleB**, int, const SubDetId::SubDetEnum&) const;
    
  protected:
   
    /// Get relevant particles
    /// Given array of input true tracks, its size
    /// The propagator provides the information on relevant tracks, interactions and VOI,  so one has to set them first
    /// Default method just splits between inelastic and no-interaction in VOI
    virtual si_syst::SISystInput* GetRelevantParticles(AnaTrueParticleB**, int) const;  
    
    ParticleSIPropagator* _propagator; 
    
  };
  
};

std::ostream& operator<<(std::ostream& os, const TVector3& vect);
std::ostream& operator<<(std::ostream& os, const TLorentzVector& vect);


#endif
