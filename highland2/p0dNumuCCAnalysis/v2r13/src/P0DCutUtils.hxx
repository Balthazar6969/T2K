#ifndef P0DCUTUTILS_HH
#define P0DCUTUTILS_HH

class AnaEventC;
class AnaTrackB;
class AnaTrueTrackB;
class ToyBoxB;

namespace cutUtils{

  bool IsStoppingP0DSandCandidate(AnaTrackB* track);
  bool IsP0DSandCandidate(AnaTrackB* track);
  int CountTracksInP0DVetoCorridor(const AnaEventC& event);
  int CountP0DTracks(const AnaEventC& event);
  bool ChooseP0DVetoTracks(const AnaEventC& event, ToyBoxB& box);

//  int CountP0DMichelClusters(const AnaEventC& event, const AnaTrackB& track);
//  int CountTrueP0DMichelClusters(const AnaEventC& event, const AnaTrueTrackB& track);


}


#endif
