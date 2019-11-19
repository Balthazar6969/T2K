import ROOT
import math
import re
from userAnalysisBase import userAnalysisBase
from grtf.tools import timing_cuts
from grtf.tools import volume_cuts
from grtf.tools import grtf_tools

class tpc_single_track_finding(userAnalysisBase): 
    """
    GRTF module for testing the TPC single track-finding efficiency.

    This analysis can only be run on oaAnalysis files produced with the 
    TReconPerformanceEvalModule enabled.
    
    The output file is populated with a hierarchy of TDirectories which logically
    structure the many histograms created.
    """

    # __init__ gets called automatically upon instantiation
    # _________________________________________________________________________
    def __init__(self, options):
        """
        Adds trees to oaAnalysisModuleTChains, and handles useroption.

        Keyword arguments:
        options -- this is passed in automatically by runAnalyses.py. 
        """

        # Call __init__ in base class
        userAnalysisBase.__init__(self, options)
        # Make TChains of the oaAnalysis trees to be read, and append to i
        # oaAnalysisModuleTChains
        self.ReconPerfEval = ROOT.TChain("ReconDir/ReconPerfEval")
        self.oaAnalysisModuleTChains.append(self.ReconPerfEval)
        self.DAQ = ROOT.TChain("HeaderDir/BasicDataQuality")
        self.oaAnalysisModuleTChains.append(self.DAQ)
        self.GeometrySummary = ROOT.TChain("HeaderDir/GeometrySummary")
        self.oaAnalysisModuleTChains.append(self.GeometrySummary)
        self.BasicHeader = ROOT.TChain("HeaderDir/BasicHeader")
        self.oaAnalysisModuleTChains.append(self.BasicHeader)

    # _________________________________________________________________________
    def MakeStandardPlots(self, name, title):
        stdhistos = {}
        stdhistos["TrkLength"]= ROOT.TH1F(TPC+name+"Trks"+"TrkLength",title+" track's length in "+TPC, 300, 0.0, 300.0);
        stdhistos["Momentum"] = ROOT.TH1F(TPC+name+"Trks"+"Momentum",	title+" track's momentum in "+TPC, 500, 0.0, 5000.0);
        stdhistos["CosTheta"] = ROOT.TH1F(TPC+name+"Trks"+"CosTheta",	title+" track's cos#theta in "+TPC, 100, 0.0, 1.01);
        stdhistos["MuPull"]   = ROOT.TH1F(TPC+name+"Trks"+"MuPull",   title+" track's muon pull in "+TPC, 400, -20.0, 20.0);
        return stdhistos


    # _________________________________________________________________________
    def BookHistogramsAndAnalysisSteps(self):
        """Create histograms for storing results, and define other useful quantities."""

        # These are the matches we look at.
        self.TPCs = [ "TPC1", "TPC2", "TPC3" ]

        # Number of events we look at, so we can scale Data/MC histograms.
        self.numEvents = {"All" : 0}

        for TPC in self.TPCs:
            self.numEvents[TPC] = 0

        # All histograms are stored in this multi-level dictionary. The structure of the dictionary defines a 
        # tree-like structure that will be written out as a tree of TDirectories. Each directory eventually 
        # contains a single histogram. The histograms for each module are generated on the fly by 
        # GenerateModuleHistograms().
        self.histos = {}

        for TPC in self.TPCs:
            self.histos[TPC] = {}

            self.histos[TPC]["RefTracks"] = MakeStandardPlots("Ref", "Reference")
            self.histos[TPC]["AllTracks"] = MakeStandardPlots("All", "All")
            self.histos[TPC]["SelTracks"] = MakeStandardPlots("Sel", "Selected")

            self.histos[TPC]["Efficiency"] = {}

            MomBin = array('d', (0.,)*6)
            MomBin[0] = 0.
            MomBin[1] = 400.
            MomBin[2] = 500.
            MomBin[3] = 700.
            MomBin[4] = 900.
            MomBin[5] = 5000.

            CosTBin = array('d', (0.,)*5)
            CosTBin[0] = 0.;
            CosTBin[1] = 0.84;
            CosTBin[2] = 0.90;
            CosTBin[3] = 0.94;
            CosTBin[4] = 1.;

            self.histos[TPC]["Efficiency"]["PassTrks_2011bins"] = ROOT.TH2F(TPC+"PassTrks_2011bins", "Tracks reconstructed (>17 nodes) in "+TPC, 5, MomBin, 4, CosTBin)
            self.histos[TPC]["Efficiency"]["TotalTrks_2011bins"]= ROOT.TH2F(TPC+"TotalTrks_2011bins", "Expected tracks in "+TPC, 5, MomBin, 4, CosTBin)

            self.histos[TPC]["Efficiency"]["PassTrks_AltBin1"] = ROOT.TH2F(TPC+"PassTrks_AltBin1", "Tracks reconstructed (>17 nodes) in "+TPC, 25, 0., 5000., 4, CosTBin)
            self.histos[TPC]["Efficiency"]["TotalTrks_AltBin1"]= ROOT.TH2F(TPC+"TotalTrks_AltBin1", "Expected tracks in "+TPC, 25, 0., 5000., 4, CosTBin)

    def EventAnalysis(self):
        """Main event analysis function, called for each event."""

        # Require a good data quality flag
        if self.DAQ.ND280OffFlag > 0:
            return

        if self.ReconPerfEval.NGlobalReconObject > 25:
            return

        self.numEvents["All"] += 1
        
        self.FillTPCTrkFindingEfficiency()

    def FillTPCTrkFindingEfficiency(self):
        """
        Test the TPC track finding efficiency for this TPC.

        The principle is to select an event with a clean track upstream and
        one downstream the tested TPC, then check if there is a track of at least
        18 nodes in this TPC.
        Use the propagation of the TPC tracks to verify that there will be a track
        in the tested TPC
        - TPC2: One clean track in TPC1 and TPC3, propagated to the downstream and
        upstream faces of TPC2 respectively
        - TPC1: One long clean track at the end of the P0D, and one clean in TPC2
        propagated to the end face of the P0D
        - TPC3: One clean track in DsECal, and one clean track in TPC2 propagated
        to the upstream face of the DsECal

        """


        #### First check the number of tracks in each TPC, a track at the
        #### end of the P0D, and a track at the upstream side of the DsECal.

        Tracks = {"TPC1" : [],
                  "TPC2" : [],
                  "TPC3" : [],
                  "P0D" : [],
                  "DSECal" : []}
        
        for obj in self.ReconPerfEval.GlobalReconObject:
            # Search for constituents that are in the right subdetectors.
            for con in obj.Constituents:
              for Det in Tracks.keys():
                if con.DetectorName.find(Det) != -1:
                  # Clean up. Don't save delta ray tracks for TPCs (less than 50MeV/c)
                  if con.DetectorName.find("TPC") and con.Momentum < 50:
                    continue
                  if con.DetectorName.find("P0D"):
                    [min, max] = grtf_tools.GetMinMax(self.GeometrySummary.P0DActive)
                    endPos   = con.GlobalReconConsLastNodeState.Position
                    # Don't count P0D tracks that are 20mm away from end face
                    if endPos.Z() < (max.Z() - 20):
                      continue
                  if con.DetectorName.find("DSECal"):
                    [min, max] = grtf_tools.GetMinMax(self.GeometrySummary.DSECALActive)
                    startPos = con.GlobalReconConsFirstNodeState.Position
                    # Don't count P0D tracks that are 20mm away from end face
                    if startPos.Z() > (min.Z() - 20):
                      continue
                  Tracks[Det].append(con)


        #### Do the three TPCs one after the other
        ####### Check TPC1
        NbTPC2Trk = 0
        RefTPC2Trk = None
        for TPC2trk in Tracks["TPC2"]:
          ### TODO: When muon pull is available from trackerRecon module using the Unique ID
          # if TPC2trk.NumHits > 17 and abs(PulldEdxMuon) < 2.5:
          if TPC2trk.NumHits > 17:
            NbTPC2Trk = NbTPC2Trk + 1
            RefTPC2Trk = TPC2trk

        NbP0DTrk = 0
        RefP0DTrk = None
        for P0Dtrk in Tracks["P0D"]:
          startPos = P0Dtrk.GlobalReconConsFirstNodeState.Position
          endPos   = P0Dtrk.GlobalReconConsLastNodeState.Position
          if (endPos.Z() - startPos.Z()):
            NbP0DTrk = NbP0DTrk + 1
            RefP0DTrk = P0Dtrk
        
        if NbTPC2Trk == 1 and NbP0DTrk == 1:
          TPC2PropPos = RefTPC2Trk.StateAtP0D.Position
          P0DendPos   = RefP0Dtrk.GlobalReconConsLastNodeState.Position
          [TPC1min, TPC1max] = grtf_tools.GetMinMax(self.GeometrySummary.P0DActive)
          #### check that the TPC2 propagated track is within the TPC1 boundaries
          InTPC1Bound = (TPC2PropPos.X() < TPC1max.X() and TPC2PropPos.X() > TPC1min.X() and TPC2PropPos.Y() < TPC1max.Y() and TPC2PropPos.Y() > TPC1min.Y() )
          #### check distance between TPC2 propagated and P0D tracks
          TPC2P0DDist = abs(TPC2PropPos.X()-P0DendPos.X()) < 200.0 and abs(TPC2PropPos.Y()-P0DendPos.Y()) < 200.0
          #### check that both ref tracks are on the same side of the cathode
          CathPosSide = P0DendPos.X() > 7.0 and RefTPC2Trk.GlobalReconConsFirstNodeState.Position.X() > 0.0
          CathNegSide = P0DendPos.X() < -7.0 and RefTPC2Trk.GlobalReconConsFirstNodeState.Position.X() < 0.0
          if ( InTPC1Bound and TPC2P0DDist and (CathPosSide or CathNegSide)):
            MaxTPC1Hits = 0
            TPC1TrkFnd = None
            for TPC1trk in Tracks["TPC1"]:
              if TPC1trk.NumHits > 17:
                if MaxTPC1Hits < TPC1trk.NumHits:
                  MaxTPC1Hits = TPC1trk.NumHits
                  TPC1TrkFnd = TPC1trk
            if TPC1TrkFnd == None:
              self.histos["TPC1"]["Efficiency"]["TotalTrks_2011bins"].Fill(RefTPC2Trk.Momentum, RefTPC2Trk.CosTheta)
              self.histos["TPC1"]["Efficiency"]["TotalTrks_AltBin1"].Fill(RefTPC2Trk.Momentum, RefTPC2Trk.CosTheta)
            else:
              self.histos["TPC1"]["Efficiency"]["TotalTrks_2011bins"].Fill(TPC1TrkFnd.Momentum, TPC1TrkFnd.CosTheta)
              self.histos["TPC1"]["Efficiency"]["TotalTrks_AltBin1"].Fill(TPC1TrkFnd.Momentum, TPC1TrkFnd.CosTheta)
              self.histos["TPC1"]["Efficiency"]["PassTrks_2011bins"].Fill(TPC1TrkFnd.Momentum, TPC1TrkFnd.CosTheta)
              self.histos["TPC1"]["Efficiency"]["PassTrks_AltBin1"].Fill(TPC1TrkFnd.Momentum, TPC1TrkFnd.CosTheta)




    def WriteResultsToOutputFile(self):
        """Write histograms and number of events to file."""

        if self.rootOutput:
            self.rootOutput.cd()

            tree = grtf_tools.ConvertDictToTTree(self.numEvents, "NumEvents")
            tree.Write()

            grtf_tools.RecurseOutput(self.histos, self.rootOutput)

            self.rootOutput.Close()
        else:
            print "Unable to find output file. Tree not saved!"
