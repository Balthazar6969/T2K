#include "EventPlotter.hxx"

#include <TList.h>

const butils::PrjType plot::PrjUsed[] = {
  butils::kXZ,
  butils::kYZ,
  butils::kXY,
  butils::kXZ,
  butils::kYZ,
  butils::kXY
};

const std::string plot::NmsUsed[] = {
  "TruthXZ",
  "TruthYZ",
  "TruthXY",
  "RecoXZ",
  "RecoYZ",
  "RecoXY"
};


//********************************************************************
bool plot::EventPlotter::Initialize(){
  //********************************************************************

  // Check that main canvas is there
  if (!_canvasMain) return false; 

  // Check that the detector info provider is available
  if (!_detForager) return false;
  
  // Initialize the forager
  if (!_detForager->FillDetectorInfo())
    return false;



  // Fill info for all possible canvas considered
  // three projections for true and reco
  
  std::map<Int_t,  det::DetBBox>::const_iterator it;
  
  for (int i = 0; i < butils::kNCnvTypes; i++){
    
    _padDataInfo[i].Proj = PrjUsed[i];
    
    det::DetBBox bndBox;
    bndBox.first.SetXYZ(1e6, 1e6, 1e6);
    bndBox.second = - bndBox.first;  
    
    for (it = _detForager->GetDetBBoxes().begin(); it != _detForager->GetDetBBoxes().end(); it++){
      det::DetectorForager::Bound(bndBox, (*it).second.first, (*it).second.second);
      if (!CheckAddForProjection(PrjUsed[i], (*it).first)) continue;
      _padDataInfo[i].BBoxes.push_back(BoundingBox(PrjUsed[i], (*it).second, GetLineAttributesBox((*it).first))); 
    }
    
    _padDataInfo[i].Histo = BuildBoundHistogram(PrjUsed[i], NmsUsed[i], bndBox); 
  }
  

  return true;
   
}


//********************************************************************
void plot::EventPlotter::CleanScreen(){
  //********************************************************************
   
  if (!_canvasMain) return;

  // Clean the object scheduled for delete
  for (std::vector<TObject*>::iterator it = _cleanupVector.begin(); it != _cleanupVector.end(); it++) {
    delete *it;
  }
  _cleanupVector.clear();
  
  if (!UpdateCanvasDrawToExpected())
    UpdateMainCanvas();
  
  // Draw the detectors 
  size_t npads = ((TList*)(_canvasMain->GetListOfPrimitives()))->GetSize();
 
  int count = butils::kXZTrue;
  for (size_t i = 0; i < npads; i++){
    _canvasMain->cd(i+1);
    gPad->Clear();
    // Found the first set bit
    for (int j = count; j < butils::kNCnvTypes; j++){
      if (GetDrawCanvas(static_cast<butils::CnvType>(j))){
        count = j+1;
        _padDataInfo[j].Draw();
        break;
      } 
    }
  }

  _canvasMain->cd(npads+1);
  DrawDefinitions();
   
  _canvasMain->Update();
}

//********************************************************************
void plot::EventPlotter::UpdateMainCanvas(){
  //********************************************************************
  if (!_canvasMain) return;
  _canvasMain->Clear();
  
  unsigned int npads = butils::BitUtils::NumberOfSetBits(_canvasDrawField);
   
  if (npads == 1)
    _canvasMain->Divide(2);
  else if (npads > 1)
    _canvasMain->Divide(2, 2);
 
}

//********************************************************************
void plot::EventPlotter::DrawEvent(const AnaEventB* event){
  //********************************************************************

  if (!_canvasMain) 
    return;

  if (!event)
    return;
  
  // Clean the canvas
  CleanScreen();

  ResetZoom();
  
  // Display event info
  event->Print();
  
  // Draw the info 
  size_t npads = ((TList*)(_canvasMain->GetListOfPrimitives()))->GetSize();
 
  int count = butils::kXZTrue;
  for (size_t i = 0; i < npads; i++){
    _canvasMain->cd(i+1);
    
    // Found the first set bit
    for (int j = count; j < butils::kNCnvTypes; j++){
      if (GetDrawCanvas(static_cast<butils::CnvType>(j))){
        count = j+1;
        DrawEvent(*event, _padDataInfo[j].Proj);
        break;
      } 
    }
  }
  _canvasMain->Update();
}

//********************************************************************
void plot::EventPlotter::DrawEvent(const AnaEventB& event, butils::PrjType prj){
  //********************************************************************  
  // Draw true info if set
  if (GetDrawObject(butils::kTrueParticles)){
    DrawTrueParticles(event, prj);
    gPad->Update();
  }
  // Draw reco info if set
  if (GetDrawObject(butils::kRecoParticles)){
    DrawParticles(event, prj);
    gPad->Update();
  }
}


//********************************************************************
void plot::EventPlotter::DrawTrueParticles(const AnaEventB& event, butils::PrjType prj){
//********************************************************************

  // Get the true particles
  AnaTrueParticleB** TrueParticles = event.TrueParticles;
  int nTrueParticles = event.nTrueParticles;

  if (nTrueParticles == 0)
    return;

  for (int i = 0; i < nTrueParticles; i++) {
    AnaTrueParticleB* particle = TrueParticles[i];

    if (!particle) continue;
    
    if (!_drawNeutral && particle->Charge == 0)
      continue;

    DrawTrueParticle(*particle, prj);  
  }
 
}

//********************************************************************
void plot::EventPlotter::DrawParticles(const AnaEventB& event, butils::PrjType prj){
//********************************************************************

  // Get the reco particles
  AnaParticleB** Particles = event.Particles;
  int nParticles = event.nParticles;

  if (nParticles == 0)
    return;

  for (int i = 0; i < nParticles; i++) {
    AnaParticleB* particle = Particles[i];

    if (!particle) continue;

    DrawParticle(*particle, prj);  
  }
}


//********************************************************************
void plot::EventPlotter::DrawTrueParticle(const AnaTrueParticleB& particle, butils::PrjType prj){
  //********************************************************************  
 
  std::vector<TVector3> points;
 
  points.push_back(particle.Position);
  points.push_back(particle.PositionEnd);
  datt::HLAttLine att = GetLineAttributes(particle);
  att.Style = 3;
  DrawLineObject(prj, points, att);
  

}
 
//********************************************************************
void plot::EventPlotter::DrawParticle(const AnaParticleB& particle , butils::PrjType prj){
  //********************************************************************  

  
  std::vector<TVector3> points;
 
  points.push_back(particle.PositionStart);
  points.push_back(particle.PositionEnd);
  DrawLineObject(prj, points, GetLineAttributes(particle));

}

//********************************************************************
void plot::EventPlotter::DrawLineObject(butils::PrjType proj, const std::vector<TVector3>& points, const datt::HLAttLine& att){
  //********************************************************************

  if (points.size() == 0) return;

  // A line
  TPolyLine* traj = new TPolyLine;

  _cleanupVector.push_back(traj);

  std::vector<TVector3>::const_iterator it = points.begin();
  for (; it != points.end(); it++){
    switch (proj){
      case butils::kXZ:{
                 traj->SetNextPoint((*it).Z(), (*it).X());
                 break;
               }
      case butils::kYZ:{
                 traj->SetNextPoint((*it).Z(), (*it).Y());
                 break;
               }          
      case butils::kXY:{
                 traj->SetNextPoint((*it).X(), (*it).Y());
                 break;
               }
      default:
               continue;
               break;
    }
  }

  traj->SetLineColor(att.Color);
  traj->SetLineWidth(att.Width);
  traj->SetLineStyle(att.Style);
  traj->Draw();
}

//********************************************************************
TBox* plot::EventPlotter::BoundingBox(butils::PrjType proj, 
    const det::DetBBox& bnd, 
    const datt::HLAttLine& att) const{
  //********************************************************************
  TBox* box = NULL;

  switch (proj){
    case butils::kXZ:{
               box = new TBox(bnd.first.Z(), 
                   bnd.first.X(),
                   bnd.second.Z(),
                   bnd.second.X());
               break;
             }
    case butils::kYZ:{
               box = new TBox(bnd.first.Z(), 
                   bnd.first.Y(),
                   bnd.second.Z(),
                   bnd.second.Y());
               break;
             }          
    case butils::kXY:{
               box = new TBox(bnd.first.X(), 
                   bnd.first.Y(),
                   bnd.second.X(),
                   bnd.second.Y());
               break;
             }
    default:
             break;
  }
  if (!box)
    return box;

  box->SetLineStyle(att.Style);
  box->SetLineColor(att.Color);
  box->SetLineWidth(att.Width);
  
  // No filling
  box->SetFillStyle(0);
  return box;
}

//********************************************************************
TH2* plot::EventPlotter::BuildBoundHistogram(butils::PrjType type, const std::string& name, const det::DetBBox& box) const{
      std::pair<unsigned int, unsigned int> tmp(0, 0);
//********************************************************************
      // assert(type == butils::kNPrjTypes);
      switch (type){
        case butils::kXZ:
          tmp.first = 0;
          tmp.second = 2;
          break;
        case butils::kYZ:
          tmp.first = 1;
          tmp.second = 2;
          break;
        case butils::kXY:
          tmp.first = 1;
          tmp.second = 0;
          break;
        default:
          break;
      }
       
      TH2* htmp = new TH2F(name.c_str(), name.c_str(),
          100, box.first[tmp.second] - 5. * units::mm,
          box.second[tmp.second] + 5. * units::mm,
          100, box.first[tmp.first] - 5. * units::mm,
          box.second[tmp.first] + 5. * units::mm); 

      htmp->SetStats(false); 
   
      return htmp; 
    }

