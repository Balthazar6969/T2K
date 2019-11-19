import ROOT
import math
import re
from userAnalysisBase import userAnalysisBase
from grtf.tools import grtf_tools
from grtf.tools import timing_cuts
from grtf.tools import volume_cuts

class tracker_global_mom(userAnalysisBase):
    """
    GRTF module for testing the effect non-tracker constituents have on the global momentum.
    
    This module compares the momentum change when adding non-tracker constituents
    to tracker-only objects. It can only be run on oaAnalysis files produced with 
    the TReconPerformanceEvalModule enabled.
    
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

        # Deal with the FV cut, if requested.
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
        """
        Set up the basic structure for storing our data.

        Histograms are created on the fly, depending on what topologies of event we
        encounter.
        """

        self.listOfHistosAndOtherObjects = []

        # All histograms are stored in this multi-level dictionary. The structure of the dictionary defines a 
        # tree-like structure that will be written out as a tree of TDirectories. Each directory eventually 
        # contains a single histogram. 
        self.histos = {}
        self.histos["tracker_global_mom"] = {}
        self.histos["tracker_global_momSummary"] = {}

        # Record the number of events we look at, as we need this to scale plots when plotting Data/MC comparisons.
        self.numEvents = {"All" : 0}

    # _________________________________________________________________________
    def EventAnalysis(self):
        """
        Main event analysis function, called for each event.
        """

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

                # Require tracks that are forwards-going and contain ECal or SMRD constituents. We explicitly require there
                # to NOT be a P0D constituent as that changes the momentum greatly. 
                if obj.Direction.Z() > 0 and (obj.SubdetectorString.find("ECal") != -1 or obj.SubdetectorString.find("SMRD") != -1):
                    if obj.SubdetectorString.find("P0D") == -1:
                        if obj.NDownToTrackerConstituents > 0 and grtf_tools.IsInFGDFV(self.GeometrySummary, obj.Position):
                            prev = {}
                            new  = {}

                            # The DownToTrackerConstituents object contains information on all 
                            # intermediate constituents between the global object and the 
                            # tracker-only object. As we iterate through it, we get closer to the 
                            # tracker-only object.
                            for ii, con in enumerate(obj.DownToTrackerConstituents):
                                new = { "Det" : con.DetectorName, "Mom" : con.Momentum }

                                # Note any objects that have odd topologies. The tracks are supposed to start
                                # in the FGD FV and be forwards going, so if they have a TPC1 constituent, for 
                                # example, things are quite possibly broken.
                                if new["Det"].find("SMRD") != -1 and new["Det"].find("DSECal") != -1:
                                    grtf_tools.PrintEvent(self, "_SMRD+DSECal_FGDFV_Forwards.txt", new["Det"])

                                if new["Det"].find("TPC1") != -1:
                                    grtf_tools.PrintEvent(self, "_TPC1_FGDFV_Forwards.txt", new["Det"])

                                if new["Det"].count("SMRD") == 2:
                                    grtf_tools.PrintEvent(self, "_TwoSMRDs_FGDFV_Forwards.txt", new["Det"])

                                if ii > 0:
                                    # Always fill a non-summary plot.
                                    self.Filltracker_global_momPlots(prev["Det"], prev["Mom"], new["Det"], new["Mom"], False)

                                    # For certain topologies, we record in a summary plot too. This makes it easier to
                                    # see the general state of things without diving in to a several hundred page PDF.
                                    if grtf_tools.IsOnlyTracker(new["Det"]) and prev["Det"].find("DSECal") != -1:
                                        self.Filltracker_global_momPlots("(TRACKER+DSECal)", prev["Mom"], "(TRACKER)", new["Mom"], True)

                                    if grtf_tools.IsOnlyTracker(new["Det"]) and prev["Det"].find("TECal") != -1:
                                        self.Filltracker_global_momPlots("(TRACKER+TECal)", prev["Mom"], "(TRACKER)", new["Mom"], True)

                                    if grtf_tools.IsOnlyTracker(new["Det"]) and prev["Det"].find("SMRD") != -1:
                                        self.Filltracker_global_momPlots("(TRACKER+SMRD)", prev["Mom"], "(TRACKER)", new["Mom"], True)

                                    if new["Det"].find("TECal") != -1 and new["Det"].find("SMRD") == -1 and prev["Det"].find("SMRD") != -1:
                                        self.Filltracker_global_momPlots("(TRACKER+TECal+SMRD)", prev["Mom"], "(TRACKER+TECal)", new["Mom"], True)

                                prev = new

        return

    # _________________________________________________________________________
    def Createtracker_global_momPlots(self, prevDet, newDet, topLevel):
        """
        Create histograms for a topology setup if they don't already exist.

        Keyword arguments:
        prevDet  - The detector string of the larger object.
        newDet   - The detector string of the smaller object.
        topLevel - Whether this is a summary plot or not.
        """

        if newDet not in self.histos[topLevel]:
            self.histos[topLevel][newDet] = {}

        if prevDet not in self.histos[topLevel][newDet]:
            # Plots for all momenta.
            self.histos[topLevel][newDet][prevDet] = {}
            self.histos[topLevel][newDet][prevDet]["1D"] = ROOT.TH1F("hMom1D" + topLevel + newDet + prevDet, "All momenta", 100, -1000, 1000)
            self.histos[topLevel][newDet][prevDet]["1D"].GetXaxis().SetTitle("Momentum difference")
            self.histos[topLevel][newDet][prevDet]["2D"] = ROOT.TH2F("hMom2D" + topLevel + newDet + prevDet, "All momenta", 100, 0, 25000, 100, 0, 25000)
            self.histos[topLevel][newDet][prevDet]["2D"].GetXaxis().SetTitle(newDet)
            self.histos[topLevel][newDet][prevDet]["2D"].GetYaxis().SetTitle(prevDet)

            self.histos[topLevel][newDet][prevDet]["Fractional"] = ROOT.TH1F("hMomFractional" + topLevel + newDet + prevDet, "All momenta (fractional)", 100, -5, 5)
            self.histos[topLevel][newDet][prevDet]["Fractional"].GetXaxis().SetTitle("Fractional momentum difference (A-B)/A")

            # Split into momentum ranges.
            for mom in ["0-200", "200-500", "500-1000", "1000-1500"]:
                self.histos[topLevel][newDet][prevDet][mom] = ROOT.TH1F("hMom1D" + topLevel + newDet + prevDet + mom, mom + " MeV", 100, -1000, 1000)
                self.histos[topLevel][newDet][prevDet][mom].GetXaxis().SetTitle("Momentum difference")

            return

    # _________________________________________________________________________
    def Filltracker_global_momPlots(self, prevDet, prevMom, newDet, newMom, isSummary):
        """
        Record the change in momentum, creating relevant histograms if needed.

        Keyword arguments:
        prevDet   - The detector string of the larger object.
        prevMom   - The momentum of the larger object.
        newDet    - The detector string of the smaller object.
        newMom    - The momentum of the smaller object.
        isSummary - Whether this is a summary plot or not.
        """

        topLevel = ("tracker_global_mom", "tracker_global_momSummary")[isSummary]
        self.Createtracker_global_momPlots(prevDet, newDet, topLevel)

        self.histos[topLevel][newDet][prevDet]["1D"].Fill(newMom - prevMom)
        self.histos[topLevel][newDet][prevDet]["2D"].Fill(newMom, prevMom)
        self.histos[topLevel][newDet][prevDet]["Fractional"].Fill((newMom - prevMom) / newMom)

        if 0 < newMom <= 200:
            self.histos[topLevel][newDet][prevDet]["0-200"].Fill(newMom - prevMom)
        if 200 < newMom <= 500:
            self.histos[topLevel][newDet][prevDet]["200-500"].Fill(newMom - prevMom)
        if 500 < newMom <= 1000:
            self.histos[topLevel][newDet][prevDet]["500-1000"].Fill(newMom - prevMom)
        if 1000 < newMom <= 1500:
            self.histos[topLevel][newDet][prevDet]["1000-1500"].Fill(newMom - prevMom)

        return

    # _________________________________________________________________________
    def WriteResultsToOutputFile(self):
        """
        Write out histograms and number of events to output file.
        """

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
