#ifndef ProtonInteractionSystematic_h
#define ProtonInteractionSystematic_h

#include "SecondaryInteractionSystematic.hxx"
#include "Parameters.hxx"
 
namespace proton_si{
  
  class ProtonSIPropagator:  public si_syst::ParticleSIPropagator{
    
  public:
    ProtonSIPropagator(){
      // For the moment read the uncertainty from the param file
      _uncertainty = (Float_t)ND::params().GetParameterD("psycheND280Utils.ProtonSI.XSec.Uncertainty"); 
    }
    
    
    virtual Double_t GetCrossSection(const si_syst::InteractionType&, const Float_t&, const Int_t&, TGeoNode*) const;
   
    virtual Double_t GetCrossSectionError(const si_syst::InteractionType& type, const Float_t&, const Int_t&, TGeoNode*) const{
      if (type != si_syst::kInelastic) return 0.; 
      return _uncertainty;
    }
   
    /// Whether a track is in VOI given a position and detector
    virtual Bool_t InVOI(const TVector3&) const;
    
  protected:
    
     void SetParameters(){
       
      // Read from the param files
      _lengthStepSize = (Float_t)ND::params().GetParameterD("psycheND280Utils.SI.StepSize");
      _momStepSize    = (Float_t)ND::params().GetParameterD("psycheND280Utils.SI.StepMom");
      
      // Set interaction types here
      AddInteractionType(si_syst::kInelastic);  
     
      // Set relevant PDGs
      AddParticlePDG(ParticleId::kProtonPDG);
      
     }
    
  private:
    
    Double_t GetProtonInelasticCrossSection(const Double_t& momentum, const Double_t& Z, const Double_t& A) const;
    
    Float_t _uncertainty;
   
    static const Float_t _thEnergy; 
  };  
}

#endif
