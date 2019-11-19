#include "P0dMassSystematics.hxx"
#include "GeometryManager.hxx"
#include "ND280AnalysisUtils.hxx"
#include "BasicUtils.hxx"
#include "DataClasses.hxx"
#include "ToyBoxTracker.hxx"
#include <iostream>
//********************************************************************
P0dMassSystematics::P0dMassSystematics():EventWeightBase(1),BinnedParams(){
  //********************************************************************

  BinnedParams::SetName("P0DMass");
  BinnedParams::SetType(k1D_SYMMETRIC);

  char dirname[256];
  sprintf(dirname,"%s/data",getenv("P0DNUMUCCANALYSISROOT"));
  BinnedParams::Read(dirname);

  SetNParameters(2);//1: water 0: everything else

  Float_t corr,err;
  GetParametersForBin(0,corr,err);
  leadMass_err = err;
  leadMass_corr = corr;
  GetParametersForBin(1,corr,err);
  brassMass_err = err;
  brassMass_corr = corr;
  GetParametersForBin(2,corr,err);
  waterMass_err = err;
  waterMass_corr = corr;
  GetParametersForBin(3,corr,err);
  otherMass_err = err;
  otherMass_corr = corr;
}

//********************************************************************
Weight_h P0dMassSystematics::ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& boxB){
//********************************************************************

  (void)event;

  // Cast the ToyBox to the appropriate type
  const ToyBoxTracker& box = *static_cast<const ToyBoxTracker*>(&boxB); 

  Weight_h eventWeight=1;

  // True vertex associated to the recon vertex should exist
  if (!box.Vertex->TrueVertex) return eventWeight;         
  


    if (anaUtils::InDetVolume(SubDetId::kP0D, box.Vertex->TrueVertex->Position)){  // TrueVertex in P0D FV


  // Get the geometry from the GeometryManager
      TGeoManager* geom = ND::hgman().GeoManager();

    //Only load in the geometry if it has not already been loaded by the InputConverter. Use in this case the default file
      if (!geom){
        ND::hgman().LoadGeometry();  // Use default file
        geom = ND::hgman().GeoManager();
      }
      AnaTrueVertexB* vert = box.Vertex->TrueVertex;
      TString material = geom->FindNode(vert->Position[0],vert->Position[1],vert->Position[2])->GetMedium()->GetName();
      Double_t err, corr;
      Double_t weight=1;
      if (material == "Water"){
        err = waterMass_err;
        corr = waterMass_corr;
        weight *= corr*(1+err * toy.GetToyVariations(_index)->Variations[1]);
      } else if (material == "Brass"){//Note: Where should "other material" category in TN-073 go? Here or in "other"?
        err = brassMass_err;
        corr = brassMass_corr;
        weight *= corr * (1+err * toy.GetToyVariations(_index)->Variations[0]);
      } else if (material == "Lead"){
        err = leadMass_err;
        corr = leadMass_corr;
        weight *= corr *(1+err* toy.GetToyVariations(_index)->Variations[0]);
      } else {//Carbon/epoxy/polystyrene/scintillator/ etc
        err = otherMass_err;
        corr = otherMass_corr;
        weight *= corr*(1+err * toy.GetToyVariations(_index)->Variations[0]);
      }


/* //Old Code based on target nucleus. Not much difference in neutrino selections (no hydrogen events) and should be much faster
      Int_t target = ((AnaTrueVertex*)box.Vertex->TrueVertex)->TargetPDG;
//      std::cout <<"P0D Target: "<<target<<std::endl;
      Float_t corr, err;
      if (target/10000 == 100082) {err = leadMass_err; corr = leadMass_corr;}
      else if (target/10000 == 100029 || target/10000 == 100030) {err = brassMass_err; corr = brassMass_corr;}
      else if (target/10000 == 100008) {err = waterMass_err;corr = waterMass_corr;}
      else {err = otherMass_err; corr = otherMass_corr;}//Mostly carbon
*/      
      eventWeight.Systematic *= (corr + err*toy.GetToyVariations(_index)->Variations[0]);
      eventWeight.Correction *= (corr);
    }
//  }
//  std::cout << "Event Weight: "<<eventWeight<<std::endl;

  return eventWeight;

}

