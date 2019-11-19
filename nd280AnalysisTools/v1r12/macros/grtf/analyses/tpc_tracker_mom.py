import ROOT
import math
import re
from userAnalysisBase import userAnalysisBase
from grtf.tools import grtf_tools
from grtf.tools import timing_cuts
from grtf.tools import volume_cuts

class tpc_tracker_mom(userAnalysisBase):
    """
    GRTF module for testing the momentum change when going from TPC-only to Tracker objects.
    Also tests the momentum difference between different TPCs.

    This analysis can only be run on oaAnalysis files produced with the 
    TReconPerformanceEvalModule enabled.
    
    The output file is populated with a heirarchy of TDirectories which logically
    structure the many histograms created.
    """

    # __init__ gets called automatically upon instantiation
    # _________________________________________________________________________
    def __init__(self, options):
        """
        Adds trees to oaAnalysisModuleTChains, and handles useroption.

        Keyword arguments:
        options -- this is passed in automatically by runAnalyses.py. 

        Recognised useroption values:
        cuttype=<val> -- which fiducial volume cut to apply. Allowed options are
                         P0D, FGD1, FGD2. Defaults to no FV cut.
        """

        # Call __init__ in base class
        userAnalysisBase.__init__(self, options)
        # Make TChains of the oaAnalysis trees to be read, and append to i
        # oaAnalysisModuleTChains
        self.ReconPerfEval = ROOT.TChain("ReconDir/ReconPerfEval")
        self.oaAnalysisModuleTChains.append(self.ReconPerfEval)
        self.GeometrySummary = ROOT.TChain("HeaderDir/GeometrySummary")
        self.oaAnalysisModuleTChains.append(self.GeometrySummary)
        self.DAQ = ROOT.TChain("HeaderDir/BasicDataQuality")
        self.oaAnalysisModuleTChains.append(self.DAQ)
        self.BasicHeader = ROOT.TChain("HeaderDir/BasicHeader")
        self.oaAnalysisModuleTChains.append(self.BasicHeader)

        self.particles = ["All", "Electron", "Muon", "Proton", "Pion"]

        self.cuttype = False
        self.trigger = False
        if options.useroption:
            for opt in options.useroption:
                [key, val] = opt.split("=", 1)
                if key == "cuttype":
                    self.cuttype = val
                if key == "trigger":
                    self.trigger = val

            if self.cuttype and self.cuttype != "P0D" and self.cuttype != "FGD1" and self.cuttype != "FGD2":
                raise ValueError("If '--useroption  cuttype=<option>' is specified, option must be either P0D, FGD1 or FGD2")

    # _________________________________________________________________________
    def BookHistogramsAndAnalysisSteps(self):
        """Create the main objects for storing results."""

        self.listOfHistosAndOtherObjects = []

        # All histograms are stored in this multi-level dictionary. The structure of the dictionary defines a 
        # tree-like structure that will be written out as a tree of TDirectories. Each directory eventually 
        # contains a single histogram. 
        self.histos = {}
        self.histos["tpc_vs_tracker_mom"] = {}
        self.histos["inter_tpc_mom"] = {}

        # Record the number of events we look at, so we can scale Data/MC histograms appropriately.
        self.numEvents = {"All" : 0}

    def EventAnalysis(self):
        """Main event analysis function, called for each event."""

        # Require a good data quality flag
        if self.DAQ.ND280OffFlag > 0:
            return

        if self.trigger and self.trigger == "FGD":
            if (not self.BasicHeader.FGDCosmicEvent) or self.BasicHeader.TripTCosmicEvent:
                return

        if self.ReconPerfEval.NGlobalReconObject > 25:
            return

        self.numEvents["All"] += 1

        for obj in self.ReconPerfEval.GlobalReconObject:
            if obj.SetOK and obj.StatusString.find("success") != -1:
                # FV and timing cuts, if requested.
                if self.cuttype:
                    isMC = (self.BasicHeader.RunID > 100000)
                    if not timing_cuts.PassesCut(isMC, self.BasicHeader.RunID, obj.Position.T()):
                        continue
                    if not volume_cuts.IsInVolume(obj.Position.Vect(), self.cuttype):
                        continue

                # Only look at forwards-going objects as the reversal changes the momentum greatly.
                if obj.NDownToTrackerConstituents > 0 and obj.Direction.Z() > 0:
                    # Get the tracker-only constituent, which is the last entry in DownToTrackerConstituents.
                    trackerCon = obj.DownToTrackerConstituents[-1]
                    tracker = { "Det" : trackerCon.DetectorName, "Mom" : trackerCon.Momentum }

                    # Get the TPC constituents, and plot the TPC-Tracker differences.
                    for i_con, con in enumerate(obj.Constituents):
                        if con.DetectorName.find("TPC") != -1:
                            tpc = { "Det" : con.DetectorName, "Mom" : con.GlobalReconConsFirstNodeState.Momentum }
                            self.Filltpc_tracker_momPlots(tracker["Det"], tracker["Mom"], tpc["Det"], tpc["Mom"], "tpc_vs_tracker_mom", obj.ParticleID)

                            # Also plot the difference between the respective TPC momenta
                            for i_con2, con2 in enumerate(obj.Constituents):
                                tpc1 = tpc
                                if con2.DetectorName.find("TPC") != -1 and i_con != i_con2:
                                    tpc2 = { "Det" : con2.DetectorName, "Mom" : con2.GlobalReconConsFirstNodeState.Momentum }
                                    if tpc["Det"] == "(TPC3)" or tpc2["Det"] == "(TPC1)":
                                        tpc1, tpc2 = tpc2, tpc1
                                    self.Filltpc_tracker_momPlots(tpc2["Det"], tpc2["Mom"], tpc1["Det"], tpc1["Mom"], "inter_tpc_mom", obj.ParticleID)

        return

    def Createtpc_tracker_momPlots(self, prevDet, newDet, topLevel):
        """
        Generate histograms for this topology, if required.

        Keyword arguments:
        prevDet  - The Track object subdetector string.
        newDet   - The TPC-only object subdetector string.
        """

        if newDet not in self.histos[topLevel]:
            self.histos[topLevel][newDet] = {}

        if prevDet not in self.histos[topLevel][newDet]:
            # Plots with no momentum cut
            self.histos[topLevel][newDet][prevDet] = {}

            for part in self.particles:
                self.histos[topLevel][newDet][prevDet][part] = {}
                self.histos[topLevel][newDet][prevDet][part]["1D"] = ROOT.TH1F("hMom1D" + topLevel + newDet + prevDet, "All momenta", 100, -1000, 1000)
                self.histos[topLevel][newDet][prevDet][part]["1D"].GetXaxis().SetTitle("Momentum difference")
                self.histos[topLevel][newDet][prevDet][part]["2D"] = ROOT.TH2F("hMom2D" + topLevel + newDet + prevDet, "All momenta", 100, 0, 25000, 100, 0, 25000)
                self.histos[topLevel][newDet][prevDet][part]["2D"].GetXaxis().SetTitle(newDet)
                self.histos[topLevel][newDet][prevDet][part]["2D"].GetYaxis().SetTitle(prevDet)

                self.histos[topLevel][newDet][prevDet][part]["Fractional"] = ROOT.TH1F("hMomFractional" + topLevel + newDet + prevDet, "All momenta (fractional)", 100, -5, 5)
                self.histos[topLevel][newDet][prevDet][part]["Fractional"].GetXaxis().SetTitle("Fractional momentum difference (A-B)/A")

            # Split into momentum bins
            for mom in ["0-200", "200-500", "500-1000", "1000-1500"]:
                self.histos[topLevel][newDet][prevDet][part][mom] = ROOT.TH1F("hMom1D" + topLevel + newDet + prevDet + mom, mom + " MeV", 100, -1000, 1000)
                self.histos[topLevel][newDet][prevDet][part][mom].GetXaxis().SetTitle("Momentum difference")

            return

    def Filltpc_tracker_momPlots(self, prevDet, prevMom, newDet, newMom, topLevel, particle):
        """
        Record the momentum change for this topology, creating the histograms if needed.

        Keyword arguments:
        prevDet  - The Tracker object subdetector string.
        prevMom  - The Tracker object momentum.
        newDet   - The TPC-only object subdetector string.
        newMom   - The TPC-only object momentum.
        topLevel - The top-level type of plot to fill.
        particle - The global PID of the object.
        """

        self.Createtpc_tracker_momPlots(prevDet, newDet, topLevel)

        parts = ["All"]

        if particle in self.particles:
            parts.append(particle)

        for part in parts:
            self.histos[topLevel][newDet][prevDet][part]["1D"].Fill(newMom - prevMom)
            self.histos[topLevel][newDet][prevDet][part]["2D"].Fill(newMom, prevMom)
            self.histos[topLevel][newDet][prevDet][part]["Fractional"].Fill((newMom - prevMom) / newMom)

            if 0 < newMom <= 200:
                self.histos[topLevel][newDet][prevDet][part]["0-200"].Fill(newMom - prevMom)
            if 200 < newMom <= 500:
                self.histos[topLevel][newDet][prevDet][part]["200-500"].Fill(newMom - prevMom)
            if 500 < newMom <= 1000:
                self.histos[topLevel][newDet][prevDet][part]["500-1000"].Fill(newMom - prevMom)
            if 1000 < newMom <= 1500:
                self.histos[topLevel][newDet][prevDet][part]["1000-1500"].Fill(newMom - prevMom)

        return

    def WriteResultsToOutputFile(self):
        """Write histograms and number of events to file."""
        
        if self.rootOutput:
            self.rootOutput.cd()

            tree = grtf_tools.ConvertDictToTTree(self.numEvents, "NumEvents")
            tree.Write()

            for histo in self.listOfHistosAndOtherObjects:
                histo.Write()

            grtf_tools.RecurseOutput(self.histos, self.rootOutput)

            self.rootOutput.Close()
        else:
            print "Unable to find output file. Tree not saved!"
