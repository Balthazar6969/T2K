import ROOT
import math
import re
from userAnalysisBase import userAnalysisBase
from grtf.tools import grtf_tools
from grtf.tools import timing_cuts
from grtf.tools import volume_cuts

class node_ordering(userAnalysisBase):
    """
    GRTF module for testing whether nodes are badly ordered in global objects.

    This module looks at all nodes in a global object and looks for kinks. A straight
    line is drawn between pairs of adjacent nodes, and the angle between these lines
    is recorded. Any nodes that have cos(angle) ~< 0.9 indicate badly-ordered nodes
    (the track is doubling back on itself). Any nodes that have cos(angle) ~> 0.9 are
    fine. Any nodes with -0.9 ~< cos(angle) ~< 0.9 may indicate a subtler problem
    such as the kinks from the PCA->Kalman Filter fitting.

    This module can only be run on oaAnalysis files produced with the 
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
        """Setup the high-level objects for storing results."""

        self.listOfHistosAndOtherObjects = []

        # All histograms are stored in this multi-level dictionary. The structure of the dictionary defines a 
        # tree-like structure that will be written out as a tree of TDirectories. Each directory eventually 
        # contains a single histogram. 
        self.histos = {}
        self.histos["node_ordering"] = {}

        # The number of events we look at. We need this so we can scale plots when doing Data/MC comparisons.
        self.numEvents = {"All" : 0}

        # Record the percentage of tracks that fail the "really bad kink" test [cos(angle) < -0.9].
        self.percentages = {}
        self.percentages["ByPos"] = {}

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

                path    = obj.SubdetectorString
                new     = False
                prev    = False
                preprev = False
                summary = grtf_tools.ConvertPathToSummary(path)

                if path not in self.percentages["ByPos"]:
                    self.percentages["ByPos"][path] = {"Total": 0, "Fail": 0}

                self.percentages["ByPos"][path]["Total"] += 1
                failedByPos = False

                # Loop over the nodes and check for any that show bad kinks.
                for node in obj.GlobalNodes:
                    new = node.NodeState

                    if not new.SetOK:
                        break

                    if preprev:
                        ok = self.FillByPosPlot(summary, preprev, prev, new, obj)
                        if not ok:
                            failedByPos = True

                    preprev = prev
                    prev = new

                # Print the details of this track if it contains a bad track.
                if failedByPos:
                    self.percentages["ByPos"][path]["Fail"] += 1

                    if grtf_tools.ContainsTracker(obj.SubdetectorString):
                        grtf_tools.PrintEvent(self, "TRACKER-Failures.txt", path)

        return

    def FillByPosPlot(self, path, preprev, prev, new, obj):
        """
        Keyword arguments:
        path    - The summarised version of the object's detector string.
        preprev - The node 2 before this one.
        prev    - The node before this one.
        new     - The current node.
        obj     - The global object we're looking at.
        """

        # Generate the histograms if needed.
        if path not in self.histos["node_ordering"]:
            self.histos["node_ordering"][path] = {}

        if "ByPos" not in self.histos["node_ordering"][path]:
            self.histos["node_ordering"][path]["ByPos"] = ROOT.TH1F("hAngleBetweenDirByPos" + path, "Based on Node Position", 52, -1.02, 1.02)
            self.histos["node_ordering"][path]["ByPos"].GetXaxis().SetTitle("cos(Angular difference of adjacent node directions, based on position)")

        # Work out the direction between the adjacent pairs of nodes, and fill the histograms.
        first  = preprev.Position.Vect() - prev.Position.Vect()
        second = prev.Position.Vect() - new.Position.Vect()
        ang = math.cos(first.Angle(second))
        self.histos["node_ordering"][path]["ByPos"].Fill(ang)
        ok = True

        # Write to the "really bad kinks" file.
        if ang < -0.9:
            grtf_tools.PrintEvent(self, "_node_ordering.angle.lt.-0.9_ByPos.txt", obj.SubdetectorString + " : " + str(ang))

            if grtf_tools.ContainsTracker(obj.SubdetectorString):
                grtf_tools.PrintEvent(self, "_node_ordering.angle.lt.-0.9_ByPos_TRACKER.txt", obj.SubdetectorString + " : " + str(ang))

            ok = False

        # Write to the "quite bad kinks" file.
        if ang < 0:
            grtf_tools.PrintEvent(self, "_node_ordering.angle.lt.0_ByPos.txt", obj.SubdetectorString + " : " + str(ang))

        return ok

    def WritePercentages(self, input, inputstr):
        """
        Calculate and write out the percentage of objects that failed the "really bad kink" test for each topology.

        Keyword arguments:
        input    - The dict containing the number of failed and total objects.
        inputstr - The string explaining exactly waht we're looking at.
        """

        for path, pathInfo in input.iteritems():
            pathInfo["Percentage"] = pathInfo["Fail"] * 100. / pathInfo["Total"]

        file = open(self.rootOutput.GetName() + "-FailurePercentages" + inputstr + ".txt", 'w')

        for path in sorted(input, key=lambda info: input[info]["Percentage"], reverse=True):
            pathInfo = input[path]
            string = "%s : %.2f%% failed (%d/%d)" % (path, pathInfo["Percentage"], pathInfo["Fail"], pathInfo["Total"])

            print string
            file.write(string + "\n")

    def WriteResultsToOutputFile(self):
        """
        Write percentage failures to screen, and write histograms and numebr of events to file.
        """

        if self.rootOutput:
            self.rootOutput.cd()

            self.WritePercentages(self.percentages["ByPos"], "ByPos")

            tree = grtf_tools.ConvertDictToTTree(self.numEvents, "NumEvents")
            tree.Write()

            for histo in self.listOfHistosAndOtherObjects:
                histo.Write()

            grtf_tools.RecurseOutput(self.histos, self.rootOutput)

            self.rootOutput.Close()
        else:
            print "Unable to find output file. Tree not saved!"
