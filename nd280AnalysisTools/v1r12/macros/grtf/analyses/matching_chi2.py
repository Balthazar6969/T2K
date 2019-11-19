import ROOT
import math
import re
from userAnalysisBase import userAnalysisBase

from grtf.tools import grtf_tools
from grtf.tools import timing_cuts
from grtf.tools import volume_cuts

class matching_chi2(userAnalysisBase): 
    """
    GRTF module for testing Matching Chi2 cuts in trackerRecon and oaRecon.

    This analysis tests what Matching Chi2 values succesfully matched tracks
    had. It includes both trackerRecon and oaRecon matches. 

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
        """Create the high-level objects for storing results in."""

        self.listOfHistosAndOtherObjects = []

        # All histograms are stored in this multi-level dictionary. The structure of the dictionary defines a 
        # tree-like structure that will be written out as a tree of TDirectories. Each directory eventually 
        # contains a single histogram. 
        self.histos = {}
        self.histos["matching_chi2"] = {}
        self.histos["matching_chi2Summary"] = {}

        # Store the number of events we look at, as this is needed for scaling histograms when making Data/MC comparisons.
        self.numEvents = {"All" : 0}

        return

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

                # The matching_chi2Info object contains a TClonesArray of objects relating to 
                # the matching at various stages of combining consittuents to get to the global object.
                # Only the matches that have a Matching Chi2 value set are stored.
                for match in obj.MatchingChi2Info:
                    if match.SetOK:
                        # The subdetectors each constituent was in
                        det1 = match.Det1
                        det2 = match.Det2
                        # Whether the objects were showers
                        sh1 = match.IsShower1
                        sh2 = match.IsShower2
                        # The Matching Chi2 value when matching the two objects.
                        val  = match.MatchingChi2

                        if det2.find("TPC") != -1 or det2.find("FGD") != -1:
                            det1, det2 = det2, det1
                            sh1, sh2 = sh2, sh1

                        # Always add to the main document.
                        self.Fillmatching_chi2Plots(det1, det2, sh1, sh2, val, False)

                        # If the topology meets certain criteria, also add to the summary document.
                        if grtf_tools.IsOnlyTracker(det1) and det2 == "(DSECal)":
                            self.Fillmatching_chi2Plots("(TRACKER)", "(DSECal)", sh1, sh2, val, True)
                        if grtf_tools.IsOnlyTracker(det1) and (det2 == "(LTECal)" or det2 == "(TTECal)" or det2 == "(BTECal)" or det2 == "(RTECal)"):
                            self.Fillmatching_chi2Plots("(TRACKER)", "(xTECal)", sh1, sh2, val, True)
                        if grtf_tools.IsOnlyTracker(det1) and (det2 == "(LPECal)" or det2 == "(TPECal)" or det2 == "(BPECal)" or det2 == "(RPECal)"):
                            self.Fillmatching_chi2Plots("(TRACKER)", "(xPECal)", sh1, sh2, val, True)
                        if grtf_tools.IsOnlyTracker(det1) and (det2 == "(LSMRD)" or det2 == "(TSMRD)" or det2 == "(BSMRD)" or det2 == "(RSMRD)"):
                            self.Fillmatching_chi2Plots("(TRACKER)", "(xSMRD)", sh1, sh2, val, True)
                        if grtf_tools.IsOnlyTracker(det1) and det2 == "(P0D)":
                            self.Fillmatching_chi2Plots("(TRACKER)", "(P0D)", sh1, sh2, val, True)
                        if det1 == "(TPC3)" and det2 == "(DSECal)":
                            self.Fillmatching_chi2Plots("(TPC3)", "(DSECal)", sh1, sh2, val, True)
                        if det1 == "(TPC1)" and det2 == "(P0D)":
                            self.Fillmatching_chi2Plots("(TPC1)", "(P0D)", sh1, sh2, val, True)
                        if (det1 == "(TPC1)" or det1 == "(TPC2)" or det1 == "(TPC3)") and (det2 == "(LTECal)" or det2 == "(TTECal)" or det2 == "(BTECal)" or det2 == "(RTECal)"):
                            self.Fillmatching_chi2Plots("(TPCn)", "(xTECal)", sh1, sh2, val, True)

        return

    def Creatematching_chi2Plots(self, det1, det2, topLevel):
        """
        Generate the histograms for this topology.

        Keyword arguments:
        det1     - The first object's detector string.
        det2     - The second object's detector string.
        topLevel - Whether this is a summary plot or not.
        """

        if det1 not in self.histos[topLevel]:
            self.histos[topLevel][det1] = {}

        if det2 not in self.histos[topLevel][det1]:
            self.histos[topLevel][det1][det2] = {}
            self.histos[topLevel][det1][det2]["1D"] = ROOT.TH1F("hMom1D" + topLevel + det1 + det2, "Matching #chi^{2}", 75, 0, 150)

        return

    def Fillmatching_chi2Plots(self, det1, det2, sh1, sh2, val, isSummary):
        """
        Record the matching chi2 value at this stage of matching, creating the histograms if needed.

        Keyword arguments:
        det1      - The first object's detector string.
        det2      - The second object's detector string.
        sh1      - Whether the first object was a shower
        sh2      - Whether the second object was a shower
        val       - The Matching Chi2 value.
        isSummary - Whether this is a summary plot or not.
        """

        if sh1:
            det1 += "(Shower)"
        else:
            det1 += "(Track)"

        if sh2:
            det2 += "(Shower)"
        else:
            det2 += "(Track)"

        topLevel = ("matching_chi2", "matching_chi2Summary")[isSummary]
        self.Creatematching_chi2Plots(det1, det2, topLevel)

        self.histos[topLevel][det1][det2]["1D"].Fill(val)

        return

    def WriteResultsToOutputFile(self):
        """Write the histograms and number of events to the output file."""

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
