import ROOT
import math
import re
from userAnalysisBase import userAnalysisBase
from grtf.tools import grtf_tools
from grtf.tools import timing_cuts
from grtf.tools import volume_cuts

class mom_by_range(userAnalysisBase):
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
        self.histos["mom_vs_rangemom"] = {}
        self.histos["mom_vs_rangemom_summary"] = {}

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

                if not volume_cuts.IsObjectContained(obj):
                  continue

                forwards = (obj.Direction.Z() > 0)
                det = obj.SubdetectorString
                mom = obj.Momentum
                rangemom = obj.MomentumByRange

                # Always fill a non-summary plot.
                self.Fillmom_by_rangePlots(det, forwards, mom, rangemom, False)

                # For certain topologies, we record in a summary plot too. This makes it easier to
                # see the general state of things without diving in to a several hundred page PDF.
                if grtf_tools.IsOnlyTracker(det) and det.find("TPC") != -1:
                    self.Fillmom_by_rangePlots("(TRACKER inc TPC)", forwards, mom, rangemom, True)

                if not grtf_tools.ContainsTracker(det) and det.find("TECal") == -1 and det.find("P0D") != -1 and det.find("DSECal") == -1 and det.find("PECal") != -1 and det.find("SMRD") != -1:
                    self.Fillmom_by_rangePlots("(P0D + xPECal + xSMRD)", forwards, mom, rangemom, True)

                if not grtf_tools.ContainsTracker(det) and det.find("TECal") == -1 and det.find("P0D") != -1 and det.find("DSECal") == -1 and det.find("PECal") != -1 and det.find("SMRD") == -1:
                    self.Fillmom_by_rangePlots("(P0D + xPECal)", forwards, mom, rangemom, True)

                if det.find("TPC") != -1 and det.find("TECal") == -1 and det.find("P0D") != -1 and det.find("DSECal") != -1 and det.find("PECal") == -1 and det.find("SMRD") == -1:
                    self.Fillmom_by_rangePlots("(TRACKER inc TPC + P0D + DSECal)", forwards, mom, rangemom, True)

                if det.find("TPC") != -1 and det.find("TECal") != -1 and det.find("P0D") == -1 and det.find("DSECal") == -1 and det.find("PECal") == -1 and det.find("SMRD") != -1:
                    self.Fillmom_by_rangePlots("(TRACKER inc TPC + xTECal + xSMRD)", forwards, mom, rangemom, True)

                if det.find("TPC") != -1 and det.find("TECal") == -1 and det.find("P0D") == -1 and det.find("DSECal") == -1 and det.find("PECal") == -1 and det.find("SMRD") != -1:
                    self.Fillmom_by_rangePlots("(TRACKER inc TPC + xSMRD)", forwards, mom, rangemom, True)

                if det.find("TPC") != -1 and det.find("TECal") == -1 and det.find("P0D") == -1 and det.find("DSECal") != -1 and det.find("PECal") == -1 and det.find("SMRD") == -1:
                    self.Fillmom_by_rangePlots("(TRACKER inc TPC + DSECal)", forwards, mom, rangemom, True)

                if det.find("TPC") != -1 and det.find("TECal") != -1 and det.find("P0D") == -1 and det.find("DSECal") == -1 and det.find("PECal") == -1 and det.find("SMRD") == -1:
                    self.Fillmom_by_rangePlots("(TRACKER inc TPC + xTECal)", forwards, mom, rangemom, True)

        return

    # _________________________________________________________________________
    def Createmom_by_rangePlots(self, det, forwards, topLevel):
        """
        Create histograms for a topology setup if they don't already exist.

        Keyword arguments:
        det      - The detector string of the object.
        forwards - String defining whether the object was forwards-going.
        topLevel - Whether this is a summary plot or not.
        """

        if det not in self.histos[topLevel]:
            self.histos[topLevel][det] = {}

        if forwards not in self.histos[topLevel][det]:
            # Plots for all momenta.
            self.histos[topLevel][det][forwards] = {}
            self.histos[topLevel][det][forwards]["1D"] = ROOT.TH1F("hMom1D" + topLevel + det + forwards, "All momenta", 100, -1000, 1000)
            self.histos[topLevel][det][forwards]["1D"].GetXaxis().SetTitle("Momentum difference")
            self.histos[topLevel][det][forwards]["2D"] = ROOT.TH2F("hMom2D" + topLevel + det + forwards, "All momenta", 100, 0, 25000, 100, 0, 25000)
            self.histos[topLevel][det][forwards]["2D"].GetXaxis().SetTitle("Momentum")
            self.histos[topLevel][det][forwards]["2D"].GetYaxis().SetTitle("Momentum by range")

            self.histos[topLevel][det][forwards]["Fractional"] = ROOT.TH1F("hMomFractional" + topLevel + det + forwards, "All momenta (fractional)", 100, -5, 5)
            self.histos[topLevel][det][forwards]["Fractional"].GetXaxis().SetTitle("Fractional momentum difference (A-B)/A")

            # Split into momentum ranges.
            for mom in ["0-200", "200-500", "500-1000", "1000-1500"]:
                self.histos[topLevel][det][forwards][mom] = ROOT.TH1F("hMom1D" + topLevel + det + forwards + mom, mom + " MeV", 100, -1000, 1000)
                self.histos[topLevel][det][forwards][mom].GetXaxis().SetTitle("Momentum difference")

            return

    # _________________________________________________________________________
    def Fillmom_by_rangePlots(self, det, forwards, mom, rangemom, isSummary):
        """
        Record the change in momentum, creating relevant histograms if needed.

        Keyword arguments:
        det       - The detectors used by this object
        forwards  - Whether the object was reconstructed as forwards going
        mom       - The momentum found using the default algorithm
        rangemom  - The momentum found using the momentum-by-range algorithm
        isSummary - Whether this is a summary plot or not.
        """

        topLevel = ("mom_vs_rangemom", "mom_vs_rangemom_summary")[isSummary]
        forwards = ("backwards", "forwards")[isSummary]
        self.Createmom_by_rangePlots(det, forwards, topLevel)

        self.histos[topLevel][det][forwards]["1D"].Fill(mom - rangemom)
        self.histos[topLevel][det][forwards]["2D"].Fill(mom, rangemom)
        frac = -10000
        if abs(mom) > 0.00001:
            frac = (mom - rangemom) / mom
        self.histos[topLevel][det][forwards]["Fractional"].Fill(frac)

        if 0 < mom <= 200:
            self.histos[topLevel][det][forwards]["0-200"].Fill(mom - rangemom)
        if 200 < mom <= 500:
            self.histos[topLevel][det][forwards]["200-500"].Fill(mom - rangemom)
        if 500 < mom <= 1000:
            self.histos[topLevel][det][forwards]["500-1000"].Fill(mom - rangemom)
        if 1000 < mom <= 1500:
            self.histos[topLevel][det][forwards]["1000-1500"].Fill(mom - rangemom)

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
