import ROOT
import math
import re
from userAnalysisBase import userAnalysisBase
from grtf.tools import timing_cuts
from grtf.tools import volume_cuts
from grtf.tools import grtf_tools

class tpc_ecal_matching(userAnalysisBase): 
    """
    GRTF module for testing TPC-ECal and TPC-P0D matching efficiencies.

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
        self.GeometrySummary = ROOT.TChain("HeaderDir/GeometrySummary")
        self.oaAnalysisModuleTChains.append(self.GeometrySummary)
        self.BasicHeader = ROOT.TChain("HeaderDir/BasicHeader")
        self.oaAnalysisModuleTChains.append(self.BasicHeader)

        # FV cuts if needed.
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
        """Create histograms for storing results, and define other useful quantities."""

        self.listOfHistosAndOtherObjects = []

        # These are the matches we look at.
        self.matches = [ {"det1" : "TPC1", "det2" : "P0D"},
                         {"det1" : "TPC2", "det2" : "LTECal"},
                         {"det1" : "TPC2", "det2" : "RTECal"},
                         {"det1" : "TPC2", "det2" : "TTECal"},
                         {"det1" : "TPC2", "det2" : "BTECal"},
                         {"det1" : "TPC3", "det2" : "DSECal"} ]

        # Number of events we look at, so we can scale Data/MC histograms.
        self.numEvents = {"All" : 0}

        for match in self.matches:
            self.numEvents[match["det1"] + match["det2"]] = 0

        # All histograms are stored in this multi-level dictionary. The structure of the dictionary defines a 
        # tree-like structure that will be written out as a tree of TDirectories. Each directory eventually 
        # contains a single histogram. The histograms for each module are generated on the fly by 
        # GenerateModuleHistograms().
        self.histos = {}

        for match in self.matches:
            submatch = match["det1"] + "-" + match["det2"]
            self.histos[submatch] = {}

            self.histos[submatch]["NumObjects"] = {}
            self.histos[submatch]["NumObjects"]["NMatched"] = ROOT.TH1F("hNMatched" + submatch, "NMatched", 15, 0, 15)

            for subdet in submatch.split("-"):
                self.histos[submatch]["NumObjects"]["NUnmatched" + subdet] = ROOT.TH1F("hNUnmatched" + subdet + submatch, "NUnmatched" + subdet, 15, 0, 15)

            self.histos[submatch]["Positions"] = {}

            for string in ["Matched", "Unmatched"]:
                self.histos[submatch]["Positions"][string] = {}

                for obj in submatch.split("-"):
                    self.histos[submatch]["Positions"][string][obj] = {}

                    for pos in ["XY", "ZX", "ZY"]:
                        self.histos[submatch]["Positions"][string][obj][pos] = ROOT.TH2F("hPos" + string + obj + pos + submatch, pos, 100, -4000, 4000, 100, -4000, 4000)

            self.histos[submatch]["Offsets"] = {}

            for string in ["Matched", "Unmatched"]:
                self.histos[submatch]["Offsets"][string] = {}

                for var in ["X", "Y", "Z"]:
                    self.histos[submatch]["Offsets"][string][var] = ROOT.TH1F("hOffset" + string + var + submatch, var, 100, -500, 500)
                self.histos[submatch]["Offsets"][string]["Mag"] = ROOT.TH1F("hOffset" + string + "Mag" + submatch, "Mag", 50, 0, 500)

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
        
        for match in self.matches:
            self.FillTPCMatching(match["det1"], match["det2"])

    def FillTPCMatching(self, det1, det2):
        """
        Test the matching efficiency for this pair of detectors.

        The testing works by looking for objects with constituents in the
        two detectors of interest. The TPC constituent must extrapolate to 
        the inner face of the other subdetector. If we find constituents 
        that are part of the same global object then they are matched,
        otherwise they are unmatched.

        Keyword arguments:
        det1 - The TPC to look for objects in.
        det2 - THe other module to look for objects in.
        """

        # Global objects containing matched/unmatched objects.
        matched = []
        tpcUnmatched = []
        otherUnmatched = []

        for obj in self.ReconPerfEval.GlobalReconObject:
            # For these plots, require the global object to have been successfully fitted.
            if obj.SetOK and obj.StatusString.find("success") != -1:
                # FV and timing cuts if requested.
                if self.cuttype:
                    isMC = (self.BasicHeader.RunID > 100000)
                    if not timing_cuts.PassesCut(isMC, self.BasicHeader.RunID, obj.Position.T()):
                        continue
                    if not volume_cuts.IsInVolume(obj.Position.Vect(), self.cuttype):
                        continue

                tpcCons = []
                otherCons = []

                # Search for constituents that are in the right subdetectors.
                for con in obj.Constituents:
                    if con.DetectorName.find(det1) != -1:
                        # TPC constituents must also extrapolate to the inner face of the otehr module.
                        if det2 == "DSECal" or det2 == "P0D":
                            if det2 == "DSECal":
                                [min, max] = grtf_tools.GetMinMax(self.GeometrySummary.DSECALExtent)
                                extrap = con.StateAtDSECal.Position
                            else:
                                [min, max] = grtf_tools.GetMinMax(self.GeometrySummary.P0DExtent)
                                extrap = con.StateAtP0D.Position

                            if (min.X() < extrap.X() < max.X()) and (min.Y() < extrap.Y() < max.Y()):
                                tpcCons.append(con)

                        elif det2 == "LTECal" or det2 == "RTECal":
                            if det2 == "RTECal":
                                [min, max] = grtf_tools.GetMinMax(self.GeometrySummary.TECAL2Extent)
                                extrap = con.StateAtRTECal.Position
                            else:
                                [min, max] = grtf_tools.GetMinMax(self.GeometrySummary.TECAL5Extent)
                                extrap = con.StateAtLTECal.Position

                            if (min.Y() < extrap.Y() < max.Y()) and (min.Z() < extrap.Z() < max.Z()):
                                tpcCons.append(con)

                        elif det2 == "BTECal" or det2 == "TTECal":
                            if det2 == "BTECal":
                                [minmod1, maxmod1] = grtf_tools.GetMinMax(self.GeometrySummary.TECAL1Extent)
                                [minmod2, maxmod2] = grtf_tools.GetMinMax(self.GeometrySummary.TECAL4Extent)
                                extrap = con.StateAtBTECal.Position
                            else:
                                [minmod1, maxmod1] = grtf_tools.GetMinMax(self.GeometrySummary.TECAL3Extent)
                                [minmod2, maxmod2] = grtf_tools.GetMinMax(self.GeometrySummary.TECAL6Extent)
                                extrap = con.StateAtTTECal.Position

                            if ((minmod1.X() < extrap.X() < maxmod1.X()) and (minmod1.Z() < extrap.Z() < maxmod1.Z())) or ((minmod2.X() < extrap.X() < maxmod2.X()) and (minmod2.Z() < extrap.Z() < maxmod2.Z())):
                                tpcCons.append(con)

                    if con.DetectorName.find(det2) != -1:
                        # Non-TPC constituents must start / end in the appropriate section of the module.
                        startPos = con.GlobalReconConsFirstNodeState.Position
                        endPos   = con.GlobalReconConsLastNodeState.Position
                        gap = 100

                        if det2 == "DSECal":
                            [min, max] = grtf_tools.GetMinMax(self.GeometrySummary.DSECALExtent)
                            if startPos.Z() < (min.Z() + gap) or endPos.Z() < (min.Z() + gap):
                                otherCons.append(con)
                        elif det2 == "P0D":
                            [min, max] = grtf_tools.GetMinMax(self.GeometrySummary.P0DActive)
                            if startPos.Z() > (max.Z() - gap) or endPos.Z() > (max.Z() - gap):
                                otherCons.append(con)
                        elif det2 == "RTECal":
                            [min, max] = grtf_tools.GetMinMax(self.GeometrySummary.TECAL2Extent)
                            if startPos.X() > (max.X() - gap) or endPos.X() > (max.X() - gap):
                                otherCons.append(con)
                        elif det2 == "LTECal":
                            [min, max] = grtf_tools.GetMinMax(self.GeometrySummary.TECAL5Extent)
                            if startPos.X() < (min.X() + gap) or endPos.X() < (min.X() + gap):
                                otherCons.append(con)
                        elif det2 == "BTECal":
                            [minmod1, maxmod1] = grtf_tools.GetMinMax(self.GeometrySummary.TECAL1Extent)
                            if startPos.Y() > (maxmod1.Y() - gap) or endPos.Y() > (maxmod1.Y() - gap):
                                otherCons.append(con)
                        elif det2 == "TTECal":
                            [minmod1, maxmod1] = grtf_tools.GetMinMax(self.GeometrySummary.TECAL3Extent)
                            if startPos.Y() < (minmod1.Y() + gap) or endPos.Y() < (minmod1.Y() + gap):
                                otherCons.append(con)

                # See if this object meets our (un)matched criteria.
                if len(tpcCons) > 0 and len(otherCons) > 0:
                    matched.append({"TPC" : tpcCons, "Other" : otherCons})
                elif len(tpcCons) > 0:
                    tpcUnmatched.append({"TPC" : tpcCons})
                elif len(otherCons) > 0:
                    otherUnmatched.append({"Other" : otherCons})

        # We're not interested in any of the global objects - break out.
        if len(matched) == 0 and len(otherUnmatched) == 0:
            return 

        # Fill the histograms.
        self.numEvents[det1 + det2] += 1
        self.histos[det1 + "-" + det2]["NumObjects"]["NMatched"].Fill(len(matched))
        self.histos[det1 + "-" + det2]["NumObjects"]["NUnmatched" + det1].Fill(len(tpcUnmatched))
        self.histos[det1 + "-" + det2]["NumObjects"]["NUnmatched" + det2].Fill(len(otherUnmatched))

        for obj in matched:
            for con in obj["TPC"]:
                self.FillPositions(getattr(con, "StateAt" + det2).Position, self.histos[det1 + "-" + det2]["Positions"]["Matched"][det1])

            for con in obj["Other"]:
                pos = self.GetPositionToPlot(con.GlobalReconConsFirstNodeState.Position, con.GlobalReconConsLastNodeState.Position, det2)
                self.FillPositions(pos, self.histos[det1 + "-" + det2]["Positions"]["Matched"][det2])

            for tpcCon in obj["TPC"]:
                state = getattr(tpcCon, "StateAt" + det2)
                tpcPos = state.Position

                for otherCon in obj["Other"]:
                    otherPos = self.GetPositionToPlot(con.GlobalReconConsFirstNodeState.Position, con.GlobalReconConsLastNodeState.Position, det2).Vect()
                    self.FillOffsets(tpcPos - otherPos, self.histos[det1 + "-" + det2]["Offsets"]["Matched"])

        for obj in tpcUnmatched:
            for con in obj["TPC"]:
                self.FillPositions(getattr(con, "StateAt" + det2).Position, self.histos[det1 + "-" + det2]["Positions"]["Unmatched"][det1])

        for obj in otherUnmatched:
            for con in obj["Other"]:
                pos = self.GetPositionToPlot(con.GlobalReconConsFirstNodeState.Position, con.GlobalReconConsLastNodeState.Position, det2)
                self.FillPositions(pos, self.histos[det1 + "-" + det2]["Positions"]["Unmatched"][det2])

        for obj in tpcUnmatched:
            for tpcCon in obj["TPC"]:
                tpcPos = getattr(tpcCon, "StateAt" + det2).Position

                for obj2 in otherUnmatched:
                    for otherCon in obj2["Other"]:
                        otherPos = self.GetPositionToPlot(otherCon.GlobalReconConsFirstNodeState.Position, otherCon.GlobalReconConsLastNodeState.Position, det2).Vect()
                        self.FillOffsets(tpcPos - otherPos, self.histos[det1 + "-" + det2]["Offsets"]["Unmatched"])

        return

    def GetPositionToPlot(self, pos1, pos2, det):
        """
        Get the end of the track that is nearest the Tracker.

        Keyword arguments:
        pos1 - One end of the track.
        pos2 - Other end of the track.
        det  - The subdetector the track is in.
        """

        if det == "P0D":
            if pos1.Z() > pos2.Z():
                return pos1
            else:
                return pos2
        if det == "DSECal":
            if pos1.Z() < pos2.Z():
                return pos1
            else:
                return pos2
        if det == "RTECal":
            if pos1.X() > pos2.X():
                return pos1
            else:
                return pos2
        if det == "LTECal":
            if pos1.X() < pos2.X():
                return pos1
            else:
                return pos2
        if det == "BTECal":
            if pos1.Y() > pos2.Y():
                return pos1
            else:
                return pos2
        if det == "TTECal":
            if pos1.Y() < pos2.Y():
                return pos1
            else:
                return pos2

    def FillOffsets(self, vect, obj):
        """
        Fill the X/Y/Z/Mag offset plots.

        Keyword arguments:
        vect - TVector3 of offset between two points.
        obj  - The dict within which to fill histograms.
        """

        obj["X"].Fill(vect.X())
        obj["Y"].Fill(vect.Y())
        obj["Z"].Fill(vect.Z())
        obj["Mag"].Fill(vect.Mag())

    def FillPositions(self, pos, obj):
        """
        Fill the XZ/YZ/XY position plots.

        Keyword arguments:
        pos - TVector3 of the objects position.
        obj - The dict within which to fill histograms.
        """

        obj["ZX"].Fill(pos.Z(), pos.X())
        obj["ZY"].Fill(pos.Z(), pos.Y())
        obj["XY"].Fill(pos.X(), pos.Y())

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
