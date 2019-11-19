import ROOT
import math
import re
from userAnalysisBase import userAnalysisBase
from grtf.tools import grtf_tools
from grtf.tools import volume_cuts
from grtf.tools import timing_cuts

class timing_offsets(userAnalysisBase): 
    """
    GRTF module for testing subdetector timing offsets from FGD1.

    This is a very simple analysis, and compares the time reported by
    different constituents of the same global track. TOF corrections
    are not included.

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
        """Create histograms for storing results in."""
        self.listOfHistosAndOtherObjects = []
        self.histos = {}
        self.histos["timing_offsets"] = {}
        self.histos["timing_offsets"]["HasFGD1"] = {}
        self.histos["timing_offsets"]["NoFGD1"] = {}

        for rawdet in grtf_tools.detStrings:
            det = "(" + rawdet + ")"
            self.histos["timing_offsets"]["HasFGD1"][det] = ROOT.TH1F("htiming_offsets" + det, det + " - (FGD1)", 100, -50, 50)
            self.histos["timing_offsets"]["HasFGD1"][det].GetXaxis().SetTitle("Timing offset (ns)")
            self.histos["timing_offsets"]["NoFGD1"][det] = ROOT.TH1F("htiming_offsets" + det, det + " - (FGD2)", 100, -50, 50)
            self.histos["timing_offsets"]["NoFGD1"][det].GetXaxis().SetTitle("Timing offset (ns)")

        # Record the number of events, so Data/MC plots can be scaled.
        self.numEvents = {"All" : 0}

    # _________________________________________________________________________
    def EventAnalysis(self):
        """Main event analysis function, called for each event."""

        # Require a good data quality flag.
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

                base = "ERR"
                fgd = "ERR"

                # Ensure we have either an FGD1 or FGD2 constituent to compare to.
                if obj.SubdetectorString.find("FGD1") != -1:
                    base = "HasFGD1"
                    fgd = "(FGD1)"
                elif obj.SubdetectorString.find("FGD2") != -1:
                    base = "NoFGD1"
                    fgd = "(FGD2)"
                else:
                    continue

                # Store the time of all constituents in this track.
                times = {}
                for rawdet in grtf_tools.detStrings:
                    det = "(" + rawdet + ")"
                    times[det] = []

                for con in obj.Constituents:
                    det  = con.DetectorName
                    time = con.GlobalReconConsState.Position.T()
                    if con.AverageHitTime != -10000:
                        time = con.AverageHitTime
                    times[det].append(time)

                if len(times[fgd]) == 0:
                    raise UserWarning("No " + fgd + " times, but obj has " + fgd + " constituent", times)

                # Choose the earliest FGD time, and compute the time difference for 
                # all other constituents.
                fgdtime = min(times[fgd])

                for det, timelist in times.iteritems():
                    for dettime in timelist:
                        self.histos["timing_offsets"][base][det].Fill(dettime - fgdtime)

        return

    # _________________________________________________________________________
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
