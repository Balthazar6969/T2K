import ROOT
import math
import re
from userAnalysisBase import userAnalysisBase
from grtf.tools import grtf_tools
from grtf.tools import timing_cuts
from grtf.tools import volume_cuts

class general(userAnalysisBase):
    """
    GRTF module for testing general global reconstruction performance.

    This analysis tests most general quantites related to each global object and its
    constituents. It can only be run on oaAnalysis files produced with the 
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

        # Make TChains of the oaAnalysis trees to be read, and append to 
        # oaAnalysisModuleTChains
        self.ReconPerfEval = ROOT.TChain("ReconDir/ReconPerfEval")
        self.oaAnalysisModuleTChains.append(self.ReconPerfEval)
        self.DAQ = ROOT.TChain("HeaderDir/BasicDataQuality")
        self.oaAnalysisModuleTChains.append(self.DAQ)
        self.BasicHeader = ROOT.TChain("HeaderDir/BasicHeader")
        self.oaAnalysisModuleTChains.append(self.BasicHeader)

        # Deal with the useroption specifying which FV cut to apply.
        self.cuttype = False
        self.trigger = False
        if options.useroption:
            print options.useroption

            for opt in options.useroption:
                [key, val] = opt.split("=", 1)
                if key == "cuttype":
                    self.cuttype = val
                if key == "trigger":
                    self.trigger = val

            if self.cuttype and self.cuttype != "P0D" and self.cuttype != "FGD1" and self.cuttype != "FGD2":
                raise ValueError("If '--useroption  cuttype=<option>' is specified, option must be either P0D, FGD1 or FGD2")
        else:
            print "No useroption"

    # _________________________________________________________________________
    def BookHistogramsAndAnalysisSteps(self):
        """
        Set up most of the histograms that will be filled.

        Some histograms are generated dynamically, but most are created here.
        We don't use the standard self.listOfHistosAndOtherObjects list to
        store the histograms in as we want a heirarchical structure to be 
        written out as TDirectories. Use multi-level dicts in self.histos.
        """

        self.listOfHistosAndOtherObjects = []

        # All histograms are stored in this multi-level dictionary. The structure of the dictionary defines a 
        # tree-like structure that will be written out as a tree of TDirectories. Each directory eventually 
        # contains a single histogram. The histograms for each module are generated on the fly by 
        # GenerateModuleHistograms().
        self.histos = {}

        # Logical groupings of histograms.
        self.groupStrings = [ "Offsets", "Positions", "Directions", "Quality", "NHits", "Truth-matching", "Efficiency of refitting", "Strings" ]

        # Some histograms are split according to whether global recon successfully matched the object to another or not.
        self.matchedStrings = [ "Matched objects", "Unmatched objects" ]

        # The status strings we support in the relevant histograms.
        self.statusKeys = { "()"                       : 0, 
                            "(kalman)"                 : 1, 
                            "(likelihood)"             : 2, 
                            "(success)"                : 3, 
                            "(ran)"                    : 4, 
                            "(success:kalman)"         : 5, 
                            "(success:likelihood)"     : 6, 
                            "(ran:success)"            : 7, 
                            "(ran:success:kalman)"     : 8, 
                            "(ran:success:likelihood)" : 9, 
                            "Other"                    : 10 }

        # The PID strings we support in PID-related histograms.
        self.pidKeys = {    "Not Set"          : 0,
                            "Other"            : 1,
                            "Shower"           : 2,
                            "EM"               : 3,
                            "Electron"         : 4,
                            "Gamma"            : 5,
                            "Hadronic"         : 6,
                            "Pi0"              : 7,
                            "Light track"      : 8,
                            "Muon"             : 9,
                            "Pion"             : 10,
                            "Heavy track"      : 11,
                            "Proton"           : 12,
                            "Kaon"             : 13,
                            "NOT_A_TRECONPID"  : 14,
                            "SOMETHING_ELSE"   : 15 }

        # Information related to the event as a whole is stored in another top-level directory.
        self.histos["Event"] = {}

        # Number of global objects in event
        self.histos["Event"]["NGlobalReconObjects"] = {}
        self.histos["Event"]["NGlobalReconObjects"]["All"] = ROOT.TH1F("hNGlobalReconObjectsAll", "# Global Objects in event", 16, -0.5, 15.5)
        self.histos["Event"]["NGlobalReconObjects"]["Success"] = ROOT.TH1F("hNGlobalReconObjectsSuccess", "# kSuccess Global Objects in event", 16, -0.5, 15.5)

        # Number of global objects with a constituent in a particular module. Histograms are generated on the fly by GenerateNGlobalContModuleHisto()
        self.histos["Event"]["NGlobalReconObjects"]["Containing Module"] = {}

        # Number of hits in each detector. Histograms are generated on the fly by GenerateNumHitsHisto()
        self.histos["Event"]["Hit Selection Size"] = {}

        # Information related to the global track is stored in another top-level directory.
        self.histos["Global"] = {}
        self.histos["Global"]["Global object"] = {}

        # Number of constituents for the global track, and subdivided by module. Histograms are generated on the fly by GenerateModuleConstituentsHisto()
        self.histos["Global"]["Global object"]["Constituents"] = {}

        # Properties of the global track
        self.histos["Global"]["Global object"]["Time"] = ROOT.TH1F("hTime", "Time of global track", 300, 0, 9000)
        self.histos["Global"]["Global object"]["Kinematics"] = {}
        self.histos["Global"]["Global object"]["Kinematics"]["Momentum"] = ROOT.TH1F("hMomentum", "Momentum of global track", 50, 0, 20000)
        self.histos["Global"]["Global object"]["Kinematics"]["Charge"] = ROOT.TH1F("hCharge", "Charge of global track", 3, -1.5, 1.5)
        self.histos["Global"]["Global object"]["Kinematics"]["Direction"] = {}
        self.histos["Global"]["Global object"]["Kinematics"]["Direction"]["CosTheta"] = ROOT.TH1F("hCosTheta", "CosTheta of global track", 50, -1.001, 1.001)
        self.histos["Global"]["Global object"]["Kinematics"]["Direction"]["Phi"] = ROOT.TH1F("hPhi", "Phi of global track", 50, -3.2, 3.2)
        self.histos["Global"]["Global object"]["Quality"] = {}
        self.histos["Global"]["Global object"]["Quality"]["Quality"] = ROOT.TH1F("hQuality", "Quality", 50, 0, 250) 
        self.histos["Global"]["Global object"]["Quality"]["NDOF"] = ROOT.TH1F("hNDOF", "NDOF", 50, 0, 250) 
        self.histos["Global"]["Global object"]["Quality"]["QualityPerNDOF"] = ROOT.TH1F("hQualityPerNDOF", "Quality/NDOF", 50, 0, 50) 
        self.histos["Global"]["Global object"]["Quality"]["QualityvsAngle"] = ROOT.TH2F("hQualityvsAngle", "Quality vs Angle", 50, -1.001, 1.001, 50, 0, 250) 
        self.histos["Global"]["Global object"]["Quality"]["NDOFvsAngle"] = ROOT.TH2F("hNDOFvsAngle", "NDOF vs Angle", 50, -1.001, 1.001, 52, -10, 250) 
        self.histos["Global"]["Global object"]["Quality"]["QualityPerNDOFvsAngle"] = ROOT.TH2F("hQualityPerNDOFvsAngle", "Quality/NDOF vs Angle", 50, -1.001, 1.001, 50, 0, 50) 
        self.histos["Global"]["Global object"]["Strings"] = {}
        self.histos["Global"]["Global object"]["Strings"]["Status"] = ROOT.TH1F("hStatusStringsGlobal", "Status", 11, 0, 11)
        self.histos["Global"]["Global object"]["PID"] = {}
        self.histos["Global"]["Global object"]["PID"]["PID"] = ROOT.TH1F("hPIDPID", "PID", 16, 0, 16)
        self.histos["Global"]["Global object"]["PID"]["Weight"] = ROOT.TH1F("hPIDWeight", "Weight", 50, 0, 1.001)
        self.histos["Global"]["Global object"]["PID"]["PID vs Weight"] = ROOT.TH2F("hPIDPIDvsWeight", "PID vs Weight", 50, 0, 1.001, 16, 0, 16)
        self.histos["Global"]["Global object"]["PID"]["WithTPC"] = {}
        self.histos["Global"]["Global object"]["PID"]["WithTPC"]["PID"] = ROOT.TH1F("hPIDWithTPCPID", "PID", 16, 0, 16)
        self.histos["Global"]["Global object"]["PID"]["WithTPC"]["Weight"] = ROOT.TH1F("hPIDWithTPCWeight", "Weight", 50, 0, 1.001)
        self.histos["Global"]["Global object"]["PID"]["WithTPC"]["PID vs Weight"] = ROOT.TH2F("hPIDWithTPCPIDvsWeight", "PID vs Weight", 50, 0, 1.001, 16, 0, 16)

        parts = ["Electron", "Muon", "Proton", "Pion"]

        for part in parts:
            self.histos["Global"]["Global object"]["PID"]["WithTPC"][part] = {}
            self.histos["Global"]["Global object"]["PID"]["WithTPC"][part]["Momentum"] = ROOT.TH1F("hMomentum"+part, "Momentum of global track", 50, 0, 5000)

        self.histos["Global"]["Global object"]["Offsets"] = {}

        for offset in ["First node state - first node object", "Last node state - last node object"]:
            self.histos["Global"]["Global object"]["Offsets"][offset] = {}

            for var in [ "XZ Measurements", "YZ Measurements", "XY Measurements", "XYZ Measurements" ]:
                self.histos["Global"]["Global object"]["Offsets"][offset][var] = {}
                for proj in var[0:3]:
                    if proj != " ":
                        self.histos["Global"]["Global object"]["Offsets"][offset][var][proj] = ROOT.TH1F("hOffset" + offset + var + proj, proj, 50, -250, 250)

        for key, val in self.statusKeys.iteritems():
            self.histos["Global"]["Global object"]["Strings"]["Status"].GetXaxis().SetBinLabel(val + 1, key)
        for key, val in self.pidKeys.iteritems():
            self.histos["Global"]["Global object"]["PID"]["PID"].GetXaxis().SetBinLabel(val + 1, key)
            self.histos["Global"]["Global object"]["PID"]["WithTPC"]["PID"].GetXaxis().SetBinLabel(val + 1, key)
            self.histos["Global"]["Global object"]["PID"]["PID vs Weight"].GetYaxis().SetBinLabel(val + 1, key)
            self.histos["Global"]["Global object"]["PID"]["WithTPC"]["PID vs Weight"].GetYaxis().SetBinLabel(val + 1, key)


        # Generate all the module-specific histograms.
        for det in grtf_tools.detStrings:
            self.GenerateModuleHistograms(det)

    def GenerateModuleHistograms(self, det):
        """
        Generate histograms related to constituents in each module.

        Some histograms are generated dynamically, but most are created here.
        """

        # Initialise the next two levels of the dictionary.
        self.histos[det] = {}

        for group in self.groupStrings:
            self.histos[det][group] = {}

        # Various offsets of the constituent.
        offsets = [ "Projected closest global state - Subdet object used by global (first node state)" ] 
        offsets.append("Subdet object used by global (object state) - Subdet object used by global (first node state)")

        objectoffsets = ["Subdet object used by global (first node state) - Subdet object used by global (first node object)"]
        objectoffsets.append("Subdet object used by global (last node state) - Subdet object used by global (last node object)")

        if grtf_tools.DetGetsRefit(det):
            # Setup histograms for the offset between a refitted object and its original object. Only valid for ECAL, P0DECAL and POD constituents.
            offsets.append("Original subdet object (first node state) - Subdet object used by global (first node state)")
            offsets.append("Original subdet object (object state) - Subdet object used by global (object state)")
            offsets.append("Original subdet object (object state) - Original subdet object (first node state)")

        for offset in offsets: 
            self.histos[det]["Offsets"][offset] = {}
        
            for matched in self.matchedStrings: 
                self.histos[det]["Offsets"][offset][matched] = {}

                for var in [ "X", "Y", "Z" ]:
                    self.histos[det]["Offsets"][offset][matched][var] = ROOT.TH1F("hOffset" + det + offset + matched + var, var, 50, -500, 500)
                self.histos[det]["Offsets"][offset][matched]["Mag"] = ROOT.TH1F("hOffset" + det + offset + matched + "Mag", "Mag", 50, 0, 500)
                self.histos[det]["Offsets"][offset][matched]["CosAngle"] = ROOT.TH1F("hOffset" + det + offset + matched + "CosAngle", "CosAngle", 50, -1.001, 1.001)

                if grtf_tools.DetHasGoodMom(det):
                    self.histos[det]["Offsets"][offset][matched]["Mom"] = ROOT.TH1F("hOffset" + det + offset + matched + "Mom", "Mom", 50, -1000, 1000)

        # State-Measurement offsets. These plots are irrelevant for the TPC as the node object is not filled.
        if det.find("TPC") == -1:
            for offset in objectoffsets:
                self.histos[det]["Offsets"][offset] = {}

                for matched in self.matchedStrings: 
                    self.histos[det]["Offsets"][offset][matched] = {}

                    for var in [ "XZ Measurements", "YZ Measurements", "XY Measurements", "XYZ Measurements" ]:
                        self.histos[det]["Offsets"][offset][matched][var] = {}

                        for proj in var[0:3]:
                            if proj != " ":
                                limit = 250
                                if det.find("SMRD") != -1:
                                    limit = 450
                                self.histos[det]["Offsets"][offset][matched][var][proj] = ROOT.TH1F("hOffset" + det + offset + matched + var + proj, proj, 50, -1 * limit, limit)
                    

        # Setup histograms for the recorded position and direction of each subdetector's objects. Original object is only valid for ECAL, P0DECAL and P0D constituents.
        states = [ "Subdet object used by global (first node state)", 
                   "Subdet object used by global (last node state)", 
                   "Subdet object used by global (object state)",
                   "Projected closest global state" ]

        if det.find("TPC") == -1:
            states.append("Subdet object used by global (first node object)") 
            states.append("Subdet object used by global (last node object)")

        if grtf_tools.DetGetsRefit(det):
            states.append("Original subdet object (first node state)") 
            states.append("Original subdet object (last node state)") 
            states.append("Original subdet object (object state)") 

        for obj in states:
            self.histos[det]["Positions"][obj] = {}

            for var in [ "ZX", "ZY", "XY" ]:
                self.histos[det]["Positions"][obj][var] = ROOT.TH2F("hPositions" + det + obj + var, var, 100, -4000, 4000, 100, -4000, 4000)

            self.histos[det]["Positions"][obj]["ZX"].GetXaxis().SetTitle("Z")
            self.histos[det]["Positions"][obj]["ZX"].GetYaxis().SetTitle("X")
            self.histos[det]["Positions"][obj]["ZY"].GetXaxis().SetTitle("Z")
            self.histos[det]["Positions"][obj]["ZY"].GetYaxis().SetTitle("Y")
            self.histos[det]["Positions"][obj]["XY"].GetXaxis().SetTitle("X")
            self.histos[det]["Positions"][obj]["XY"].GetYaxis().SetTitle("Y")

            if obj.find("node object") == -1:
                self.histos[det]["Directions"][obj] = {}
                self.histos[det]["Directions"][obj]["CosTheta"] = ROOT.TH1F("hDirections" + det + obj + "CosTheta", "CosTheta", 50, -1.001, 1.001)
                self.histos[det]["Directions"][obj]["Phi"] = ROOT.TH1F("hDirections" + det + obj + "Phi", "Phi", 50, -3.2, 3.2)

        # Quality and NDOF histograms. Different subdetectors have very different values, so set
        # different maxima for the histograms. Order of maxima/minima is Quality, NDOF, Quality/NDOF
        maxima = [250, 50, 50]
        minima = [0, -10, 0]
        if det.find("TPC") != -1:
            maxima = [50, 500, 20]
            minima = [0, -100, 0]
        if det.find("P0D") != -1:
            maxima = [50, 200, 20]
            minima = [0, -40, 0]

        self.histos[det]["Quality"]["Subdet object used by global" ] = {}
        self.histos[det]["Quality"]["Subdet object used by global" ]["Quality"] = ROOT.TH1F("hQuality" + det + "Quality", "Quality", 60, minima[0], maxima[0]) 
        self.histos[det]["Quality"]["Subdet object used by global" ]["NDOF"] = ROOT.TH1F("hQuality" + det + "NDOF", "NDOF", 60, minima[1], maxima[1]) 
        self.histos[det]["Quality"]["Subdet object used by global" ]["QualityPerNDOF"] = ROOT.TH1F("hQuality" + det + "QualityPerNDOF", "Quality/NDOF", 60, minima[2], maxima[2]) 
        self.histos[det]["Quality"]["Subdet object used by global" ]["NDOFIsShower"] = ROOT.TH2F("hQuality" + det + "NDOFIsShower", "NDOF vs IsShower", 60, minima[1], maxima[1], 2, 0, 2)
        self.histos[det]["Quality"]["Subdet object used by global" ]["NDOFIsShower"].GetXaxis().SetTitle("NDOF") 
        self.histos[det]["Quality"]["Subdet object used by global" ]["NDOFIsShower"].GetYaxis().SetTitle("IsShower") 
        self.histos[det]["Quality"]["Subdet object used by global" ]["QualityvsAngle"] = ROOT.TH2F("hQuality" + det + "QualityvsAngle", "Quality vs Angle", 50, -1.001, 1.001, 60, minima[0], maxima[0]) 
        self.histos[det]["Quality"]["Subdet object used by global" ]["NDOFvsAngle"] = ROOT.TH2F("hQuality" + det + "NDOFvsAngle", "NDOF vs Angle", 50, -1.001, 1.001, 60, minima[1], maxima[1]) 
        self.histos[det]["Quality"]["Subdet object used by global" ]["QualityPerNDOFvsAngle"] = ROOT.TH2F("hQuality" + det + "QualityPerNDOFvsAngle", "Quality/NDOF vs Angle", 50, -1.001, 1.001, 60, minima[2], maxima[2]) 

        # Miscellaneous quantities
        self.histos[det]["Misc"] = {}
        self.histos[det]["Misc"]["NHits"] = ROOT.TH1F("hNHits" + det, "NHits", 100, 0, 200)
        self.histos[det]["Misc"]["Time"] = ROOT.TH1F("hTime" + det, "Time", 300, 0, 9000) 
        self.histos[det]["Misc"]["IsShower"] = ROOT.TH1F("hIsShower" + det, "IsShower", 2, 0, 2) 

        maximum = 5
        if det.find("TPC") != -1 or det.find("P0D") != -1:
            maximum = 1000
        self.histos[det]["Misc"]["EDeposit"] = ROOT.TH1F("hEDeposit" + det, "EDeposit", 100, 0, maximum) 

        # Setup histograms for the truth-matching
        self.histos[det]["Truth-matching"]["Efficiency"] = ROOT.TH1F("hTruthEff" + det, "Efficiency", 10, -0.0001, 1.0001)
        self.histos[det]["Truth-matching"]["Purity"] = ROOT.TH1F("hTruthPur" + det, "Purity", 10, -0.0001, 1.0001)

        # Status of object, with axis labelled appropriately.
        self.histos[det]["Strings"]["Status"] = ROOT.TH1F("hStatusStrings" + det, "Status", 11, 0, 11)

        for key, val in self.statusKeys.iteritems():
            self.histos[det]["Strings"]["Status"].GetXaxis().SetBinLabel(val + 1, key)

        # PID and PID weight, with axes labelled appropriately.
        self.histos[det]["PID"] = {}
        self.histos[det]["PID"]["PID"] = ROOT.TH1F("hPIDPID" + det, "PID", 16, 0, 16)
        self.histos[det]["PID"]["Weight"] = ROOT.TH1F("hPIDWeight" + det, "Weight", 50, 0, 1.001)
        self.histos[det]["PID"]["PID vs Weight"] = ROOT.TH2F("hPIDPIDvsWeight" + det, "PID vs Weight", 50, 0, 1.001, 16, 0, 16)

        for key, val in self.pidKeys.iteritems():
            self.histos[det]["PID"]["PID"].GetXaxis().SetBinLabel(val + 1, key)
            self.histos[det]["PID"]["PID vs Weight"].GetYaxis().SetBinLabel(val + 1, key)

        # Efficiency of refitting, as a function of various variables. Only applicable to  
        # subdetectors that get refit by RECPACK before being considered.
        if grtf_tools.DetGetsRefit(det):
            varDetails = {
                          "Angle"          : {"Bins" : 50,  "Min" : -1.001, "Max" : 1.001, "Axis" : "CosTheta of original subdetector object"} ,   
                          "NumHits"        : {"Bins" : 150, "Min" : 0,      "Max" : 150,   "Axis" : "Number of hits in constituent"} ,   
                         }

            for var, dets in  varDetails.iteritems():
                self.histos[det]["Efficiency of refitting"][var] = {}

                for hist in ["Total", "Refitted", "Eff"]:
                    self.histos[det]["Efficiency of refitting"][var][hist] = ROOT.TH1F("hRefitEff" + det + var + hist, hist, dets["Bins"], dets["Min"], dets["Max"])
                    self.histos[det]["Efficiency of refitting"][var][hist].GetXaxis().SetTitle(dets["Axis"])

        return

    # _________________________________________________________________________
    def EventAnalysis(self):
        """
        The main event analysis function, called for each event.
        """

        # Require a good data quality flag
        if self.DAQ.ND280OffFlag > 0:
            return

        if self.trigger and self.trigger == "FGD":
            if (not self.BasicHeader.FGDCosmicEvent) or self.BasicHeader.TripTCosmicEvent:
                return

        if self.ReconPerfEval.NGlobalReconObject > 25:
            grtf_tools.PrintEvent(self, "HighMultiplicity.txt", str(self.ReconPerfEval.NGlobalReconObject) + " objects")
            return

        # Counters for the number of object in this event
        nGlobalReconObjectsSuccess = 0
        nGlobalReconObjectsAll = 0
        nGlobalReconObjectsContModule = {}
        for det in grtf_tools.detStrings:
            nGlobalReconObjectsContModule[det] = 0

        # Event-level quantity - number of hits in each module's THitSelection
        for name, value in self.ReconPerfEval.HitInfo:
            self.FillNumHitsHisto(name, value)

        for obj in self.ReconPerfEval.GlobalReconObject:
            # For these plots, require the global object to have been successfully fitted.
            if obj.SetOK and obj.StatusString.find("success") != -1:
                # FV and timing cuts, if appropriate
                if self.cuttype:
                    # Python fails to read the boolean BasicHeader.IsMC flag, so use this hack for 
                    # deciding whether we're looking at an MC file or not.
                    isMC = (self.BasicHeader.RunID > 100000)
                    if not timing_cuts.PassesCut(isMC, self.BasicHeader.RunID, obj.Position.T()):
                        continue
                    if not volume_cuts.IsInVolume(obj.Position.Vect(), self.cuttype):
                        continue

                nGlobalReconObjectsSuccess += 1

                # Fill plots related to the global track
                for det in grtf_tools.detStrings:
                    string = "(" + det + ")"
                    if string in obj.NModuleConstituents:
                        value = obj.NModuleConstituents[string]
                        self.FillModuleConstituentsHistos(det, value, obj.NConstituents)

                        if value > 0:
                            nGlobalReconObjectsContModule[det] += 1

                self.histos["Global"]["Global object"]["Time"].Fill(obj.Position.T())
                self.histos["Global"]["Global object"]["Kinematics"]["Momentum"].Fill(obj.Momentum)
                self.histos["Global"]["Global object"]["Kinematics"]["Charge"].Fill(obj.Charge)
                self.histos["Global"]["Global object"]["Kinematics"]["Direction"]["CosTheta"].Fill(obj.Direction.CosTheta())
                self.histos["Global"]["Global object"]["Kinematics"]["Direction"]["Phi"].Fill(obj.Direction.Phi())
                self.FillGlobalQualityHists(obj.Quality, obj.NDOF, obj.Direction)

                nodesSaved = obj.NGlobalNodesSaved
                if nodesSaved > 0:
                    firstNode = obj.GlobalNodes.At(0)
                    lastNode = obj.GlobalNodes.At(nodesSaved - 1)
                    self.FillGlobalOffsetHists("First node state - first node object", firstNode) 
                    self.FillGlobalOffsetHists("Last node state - last node object", lastNode)

                # Fill plots related to each constituent.
                for con in obj.Constituents:
                    # Set global variables that help define which histogram to fill.
                    self.matched = self.matchedStrings[(len(obj.Constituents) == 1)]
                    self.det     = grtf_tools.GetDet(con.DetectorName)

                    refittedFirstState  = con.GlobalReconConsFirstNodeState
                    refittedLastState   = con.GlobalReconConsLastNodeState
                    refittedFirstObject = con.GlobalReconConsFirstNodeObject
                    refittedLastObject  = con.GlobalReconConsLastNodeObject
                    refittedObject = con.GlobalReconConsState
                    closestGlobal  = con.ClosestGlobalNodeState
                    originalFirst  = con.OriginalObjectFirstNodeState
                    originalLast   = con.OriginalObjectLastNodeState
                    originalObject = con.OriginalObjectState
                    truth          = con.ConsTruth

                    self.FillPositionHists("Subdet object used by global (first node state)", refittedFirstState)
                    self.FillPositionHists("Subdet object used by global (last node state)", refittedLastState)
                    self.FillPositionHists("Subdet object used by global (object state)", refittedObject)

                    if self.det.find("TPC") == -1:
                        self.FillPositionHists("Subdet object used by global (first node object)", refittedFirstObject, False)
                        self.FillPositionHists("Subdet object used by global (last node object)", refittedLastObject, False)

                    self.FillTimeHist(refittedObject)

                    self.FillPositionHists("Projected closest global state", closestGlobal)

                    self.FillOffsetHists("Projected closest global state - Subdet object used by global (first node state)", closestGlobal, refittedFirstState)
                    self.FillOffsetHists("Subdet object used by global (object state) - Subdet object used by global (first node state)", refittedObject, refittedFirstState)

                    if self.det.find("TPC") == -1:
                        self.FillObjectOffsetHists("Subdet object used by global (first node state) - Subdet object used by global (first node object)", refittedFirstState, refittedFirstObject)
                        self.FillObjectOffsetHists("Subdet object used by global (last node state) - Subdet object used by global (last node object)", refittedLastState, refittedLastObject)

                    self.FillQualityHists(con.Quality, con.NDOF, (0,1)[con.IsShower], refittedObject.Direction)

                    if con.NDOF < 0 and not con.IsShower:
                        grtf_tools.PrintEvent(self, "_NDOF.lt.0.txt", obj.SubdetectorString + "->" + con.DetectorName + " NDOF = " + str(con.NDOF) + "[IsShower=" + str(con.IsShower) + "]")

                    self.FillNHitsHist(con.NumHits)
                    self.FillEDepositHist(con.EDeposit)
                    self.FillIsShowerHist((0,1)[con.IsShower])

                    if grtf_tools.DetGetsRefit(self.det):
                        self.FillPositionHists("Original subdet object (first node state)", originalFirst)
                        self.FillPositionHists("Original subdet object (last node state)", originalLast)
                        self.FillPositionHists("Original subdet object (object state)", originalObject)

                        self.FillOffsetHists("Original subdet object (first node state) - Subdet object used by global (first node state)", originalFirst, refittedFirstState)
                        self.FillOffsetHists("Original subdet object (object state) - Subdet object used by global (object state)", originalObject, refittedObject)
                        self.FillOffsetHists("Original subdet object (object state) - Original subdet object (first node state)", originalObject, originalFirst)

                    if truth.SetOK:
                        self.histos[self.det]["Truth-matching"]["Efficiency"].Fill(truth.Efficiency)
                        self.histos[self.det]["Truth-matching"]["Purity"].Fill(truth.Purity)

            # Fill plots that are related to the efficiency of refitting / fitting. By definition, these plots don't require
            # the kSuccess bit to be set.
            if obj.SetOK:
                # FV and timing cuts, if necessary.
                if self.cuttype:
                    isMC = (self.BasicHeader.RunID > 100000)
                    if not timing_cuts.PassesCut(isMC, self.BasicHeader.RunID, obj.Position.T()):
                        continue
                    if not volume_cuts.IsInVolume(obj.Position.Vect(), self.cuttype):
                        continue

                nGlobalReconObjectsAll += 1
                self.FillGlobalStatusString(obj.StatusString)

                hasTPC = (obj.SubdetectorString.find("TPC") != -1)
                self.FillGlobalPIDInfo(obj.ParticleID, obj.PIDWeight, obj.Momentum, hasTPC)

                for con in obj.Constituents:
                    refittedObject = con.GlobalReconConsFirstNodeState
                    originalObject = con.OriginalObjectFirstNodeState
                    truth          = con.ConsTruth

                    self.det = grtf_tools.GetDet(con.DetectorName)
                    self.FillStatusString(con.StatusString)
                    self.FillPIDInfo(con.ParticleID, con.PIDWeight)

                    if grtf_tools.DetGetsRefit(self.det):
                        if refittedObject.SetOK and originalObject.SetOK:
                            self.FillRefitEff("Angle", con.StatusString, originalObject.Direction.CosTheta())
                            self.FillRefitEff("NumHits", con.StatusString, con.NumHits)

        # Number of objects we have in this event
        self.histos["Event"]["NGlobalReconObjects"]["Success"].Fill(nGlobalReconObjectsSuccess)
        self.histos["Event"]["NGlobalReconObjects"]["All"].Fill(nGlobalReconObjectsAll)

        for module, value in nGlobalReconObjectsContModule.iteritems():
            self.FillNGlobalContModuleHisto(module, value)

        return

    # _________________________________________________________________________
    def GenerateNumHitsHisto(self, name):
        """
        Create a histogram for storing the number of hits in a given hit selection.

        Keyword arguments:
        name - The name of hit selection.
        """

        nbins = 100
        max = 10000
        if name.find("ecal") != -1 or name.find("barrel") != -1:
            max = 1000
        if name.find("p0d") != -1:
            max = 10000
        if name.find("tpc") != -1:
            max = 1000
        if name.find("fgd") != -1:
            max = 500
        self.histos["Event"]["Hit Selection Size"][name] = ROOT.TH1F("hNumHits" + name, "Number of hits in " + name, nbins, 0, max)

        return

    # _________________________________________________________________________
    def GenerateModuleConstituentsHisto(self, counter):
        """
        Create histograms for storing the number of constituents a global PID has in a given subdetector.

        Keyword arguments:
        counter - The name of the counting variable (e.g. "NP0D")
        """

        self.histos["Global"]["Global object"]["Constituents"][counter] = {} 
        self.histos["Global"]["Global object"]["Constituents"][counter][counter] = ROOT.TH1F("hCount" + counter, counter, 16, -0.5, 15.5)
        self.histos["Global"]["Global object"]["Constituents"][counter][counter + " vs NConstituents"] = ROOT.TH2F("hCount" + counter + "NConstituents", counter + " vs NConstituents", 16, -0.5, 15.5, 16, -0.5, 15.5)
        self.histos["Global"]["Global object"]["Constituents"][counter][counter + " vs NConstituents"].GetXaxis().SetTitle("NConstituents")
        self.histos["Global"]["Global object"]["Constituents"][counter][counter + " vs NConstituents"].GetYaxis().SetTitle(counter)

        return

    # _________________________________________________________________________
    def GenerateNGlobalContModuleHisto(self, module):
        """
        Create a histogram for storing the number of global PIDs that pass through the module.

        Keyword arguments:
        module - The name of the subdetector to create a histogram for.
        """

        self.histos["Event"]["NGlobalReconObjects"]["Containing Module"][module] = ROOT.TH1F("hGlobalCont" + module, module, 16, -0.5, 15.5)

        return

    # _________________________________________________________________________
    def FillNumHitsHisto(self, name, value):
        """
        Record the number of hits in a given hit selection, creating the histogram if necessary.

        Keyword arguments:
        name  - The name of the hit selection.
        value - The number of hits in the hit selection.
        """

        if name not in self.histos["Event"]["Hit Selection Size"]:
            self.GenerateNumHitsHisto(name)

        self.histos["Event"]["Hit Selection Size"][name].Fill(value)

        return

    # _________________________________________________________________________
    def FillModuleConstituentsHistos(self, module, nCons, nConsTotal):
        """
        Record the number of constituents a global object has in a given module.

        The appropriate histograms are created if necessary.

        Keyword arguments:
        module     - The name of the subdetector of we're looking at.
        nCons      - The number of constituents of in this module.
        nConsTotal - The total number of constituents the global object has.
        """

        counter = "N" + module
        if counter not in self.histos["Global"]["Global object"]["Constituents"].keys():
            self.GenerateModuleConstituentsHisto(counter)

        self.histos["Global"]["Global object"]["Constituents"][counter][counter].Fill(nCons)
        self.histos["Global"]["Global object"]["Constituents"][counter][counter + " vs NConstituents"].Fill(nConsTotal, nCons)

        return

    # _________________________________________________________________________
    def FillNGlobalContModuleHisto(self, module, value):
        """
        Record the number of global objects in an event that had constituents in a given module.

        The appropriate histograms are created if necessary.

        Keyword arguments:
        module - The name of the subdetector we're looking at.
        value  - The number of global objects that had constituents in this module.
        """

        if module not in self.histos["Event"]["NGlobalReconObjects"]["Containing Module"]:
            self.GenerateNGlobalContModuleHisto(module)

        self.histos["Event"]["NGlobalReconObjects"]["Containing Module"][module].Fill(value)

        return

    # _________________________________________________________________________
    def FillGlobalStatusString(self, key):
        """
        Record the global object's status strings.

        If the status is not mapped to a given bin in the histogram, a message is
        printed to screen, and the status is recorded as "Other".

        Keyword arguments:
        key - The status of the object.
        """

        if key not in self.statusKeys:
            print "Unmapped status key is ", key
            key = "Other"
        self.histos["Global"]["Global object"]["Strings"]["Status"].Fill(self.statusKeys[key])

        return

    # _________________________________________________________________________
    def FillStatusString(self, key):
        """
        Record a constituent's status string.

        If the status is not mapped to a given bin in the histogram, a message is
        printed to screen, and the status is recorded as "Other".

        Keyword arguments:
        key - The status of the object.
        """

        if key not in self.statusKeys:
            print "Unmapped status key is ", key
            key = "Other"
        self.histos[self.det]["Strings"]["Status"].Fill(self.statusKeys[key])

        return

    # _________________________________________________________________________
    def FillGlobalPIDInfo(self, pid, weight, mom, hasTPC):
        """
        Record information relating to a global object's PID.

        If the pid is not mapped to a bin in the relevant histogram, a message
        is printed to screen, and the PID is recorded as "SOMETHING_ELSE"

        Keyword arguments:
        pid    - The ParticleID of the object.
        weight - The PID weight of the object.
        mom    - The global momentum of the object.
        hasTPC - Whether the object uses TPC information.
        """

        if pid not in self.pidKeys:
            print "Unmapped pid key is ", pid
            pid = "SOMETHING_ELSE"

        self.histos["Global"]["Global object"]["PID"]["PID"].Fill(self.pidKeys[pid])
        self.histos["Global"]["Global object"]["PID"]["Weight"].Fill(weight)
        self.histos["Global"]["Global object"]["PID"]["PID vs Weight"].Fill(weight, self.pidKeys[pid])

        if hasTPC:
            self.histos["Global"]["Global object"]["PID"]["WithTPC"]["PID"].Fill(self.pidKeys[pid])
            self.histos["Global"]["Global object"]["PID"]["WithTPC"]["Weight"].Fill(weight)
            self.histos["Global"]["Global object"]["PID"]["WithTPC"]["PID vs Weight"].Fill(weight, self.pidKeys[pid])

            if pid in self.histos["Global"]["Global object"]["PID"]["WithTPC"]:
                self.histos["Global"]["Global object"]["PID"]["WithTPC"][pid]["Momentum"].Fill(mom)

        return

    # _________________________________________________________________________
    def FillPIDInfo(self, pid, weight):
        """
        Record information relating to a constituent's PID.

        If the pid is not mapped to a bin in the relevant histogram, a message
        is printed to screen, and the PID is recorded as "SOMETHING_ELSE"

        Keyword arguments:
        pid    - The ParticleID of the object.
        weight - The PID weight of the object.
        """

        if pid not in self.pidKeys:
            print "Unmapped pid key is ", pid
            pid = "SOMETHING_ELSE"
        self.histos[self.det]["PID"]["PID"].Fill(self.pidKeys[pid])
        self.histos[self.det]["PID"]["Weight"].Fill(weight)
        self.histos[self.det]["PID"]["PID vs Weight"].Fill(weight, self.pidKeys[pid])

        return

    # _________________________________________________________________________
    def FillRefitEff(self, key, status, val):
        """
        Fill the appropriate refitting efficiency plots.

        This is only relevant for subdetectors that get refit by RECPACK before
        being considered for merging.

        Keyword arguments:
        key    - The name of the variable that refitting efficiency is being plotted against.
        status - The status string of the object. Classed as being successfully refit if it contains "success".
        val    - The value of the variable that refitting efficiency is being plotted against.
        """

        self.histos[self.det]["Efficiency of refitting"][key]["Total"].Fill(val)
        if status.find("success") != -1:
            self.histos[self.det]["Efficiency of refitting"][key]["Refitted"].Fill(val)

        return

    # _________________________________________________________________________
    def FillQualityHists(self, quality, ndof, isShower, direction):
        """
        Record the quality of a constituent.

        If ndof = 0, QualityPerDOF is set to +1e6 (-1e6) if the quality is positive (negative).

        Keyword arguments:
        quality   - The constituent's quality.
        ndof      - The constituent's NDOF.
        isShower  - Set to 1 (0) if the constituent is a shower (track).
        direction - The constituents direction, as a TVector3.
        """

        qualityPerDOF = 100000

        if ndof != 0:
            qualityPerDOF = quality / ndof
        elif quality < 0:
            qualityPerDOF *= -1.

        ang = direction.CosTheta()

        self.histos[self.det]["Quality"]["Subdet object used by global" ]["Quality"].Fill(quality)
        self.histos[self.det]["Quality"]["Subdet object used by global" ]["NDOF"].Fill(ndof)
        self.histos[self.det]["Quality"]["Subdet object used by global" ]["QualityPerNDOF"].Fill(qualityPerDOF)
        self.histos[self.det]["Quality"]["Subdet object used by global" ]["NDOFIsShower"].Fill(ndof, isShower)
        self.histos[self.det]["Quality"]["Subdet object used by global" ]["QualityvsAngle"].Fill(ang, quality)
        self.histos[self.det]["Quality"]["Subdet object used by global" ]["NDOFvsAngle"].Fill(ang, ndof)
        self.histos[self.det]["Quality"]["Subdet object used by global" ]["QualityPerNDOFvsAngle"].Fill(ang, qualityPerDOF)

        return

    # _________________________________________________________________________
    def FillGlobalQualityHists(self, quality, ndof, direction):
        """
        Record the quality of a constituent.

        If ndof = 0, QualityPerDOF is set to +1e6 (-1e6) if the quality is positive (negative).

        Keyword arguments:
        quality   - The constituent's quality.
        ndof      - The constituent's NDOF.
        direction - The constituents direction, as a TVector3.
        """

        qualityPerDOF = 100000

        if ndof != 0:
            qualityPerDOF = quality / ndof
        elif quality < 0:
            qualityPerDOF *= -1.

        ang = direction.CosTheta()

        self.histos["Global"]["Global object"]["Quality"]["Quality"].Fill(quality)
        self.histos["Global"]["Global object"]["Quality"]["NDOF"].Fill(ndof)
        self.histos["Global"]["Global object"]["Quality"]["QualityPerNDOF"].Fill(qualityPerDOF)
        self.histos["Global"]["Global object"]["Quality"]["QualityvsAngle"].Fill(ang, quality)
        self.histos["Global"]["Global object"]["Quality"]["NDOFvsAngle"].Fill(ang, ndof)
        self.histos["Global"]["Global object"]["Quality"]["QualityPerNDOFvsAngle"].Fill(ang, qualityPerDOF)

        return

    # _________________________________________________________________________
    def FillNHitsHist(self, nhits):
        """
        Record the number of hits in this constituent.

        Keyword arguments:
        nhits - The number of hits used for this constituent.
        """

        self.histos[self.det]["Misc"]["NHits"].Fill(nhits)

        return

    # _________________________________________________________________________
    def FillEDepositHist(self, dep):
        """
        Record the EDeposit of this constituent.

        Keyword arguments:
        dep - The EDeposit.
        """

        self.histos[self.det]["Misc"]["EDeposit"].Fill(dep)

        return

    # _________________________________________________________________________
    def FillIsShowerHist(self, isShower):
        """
        Record whether the constituent is a shower.

        Keyword arguments:
        isShower - 1 (0) if the constituent is a shower (track), or a PID that immediately
                   inherits from a shower (track) in the object heirarchy.
        """

        self.histos[self.det]["Misc"]["IsShower"].Fill(isShower)

        return

    # _________________________________________________________________________
    def FillTimeHist(self, object):
        """
        Record the time of a constituent.

        Keyword arguments:
        object - The constituent to get the time of.
        """

        if object.SetOK:
            time = object.Position.T()
            self.histos[self.det]["Misc"]["Time"].Fill(time)

        return

    # _________________________________________________________________________
    def FillOffsetHists(self, key, obj1, obj2):
        """
        Record the positional and directional offsets between two objects related to a constituent.

        Offsets are filled as (obj1 - obj2). If the constituent gives a good momentum estimate
        then momentum offsets are also created.

        Keyword arguments:
        key  - The string describing the relationship between the two objects. 
        obj1 - The first object.
        obj2 - The second object. 
        """

        if obj1.SetOK and obj2.SetOK:
            offset = (obj1.Position - obj2.Position).Vect()
            dir = math.cos(obj1.Direction.Angle(obj2.Direction))
            mom = (obj1.Momentum - obj2.Momentum)

            self.histos[self.det]["Offsets"][key][self.matched]["X"].Fill(offset.X())
            self.histos[self.det]["Offsets"][key][self.matched]["Y"].Fill(offset.Y())
            self.histos[self.det]["Offsets"][key][self.matched]["Z"].Fill(offset.Z())
            self.histos[self.det]["Offsets"][key][self.matched]["Mag"].Fill(offset.Mag())
            self.histos[self.det]["Offsets"][key][self.matched]["CosAngle"].Fill(dir)

            if grtf_tools.DetHasGoodMom(self.det):
                self.histos[self.det]["Offsets"][key][self.matched]["Mom"].Fill(mom)

        return

    # _________________________________________________________________________
    def FillObjectOffsetHists(self, key, obj1, obj2):
        """
        Record the positional offsets between a constituent node's state and the node's object.

        The node object relates to the measurement, and the node state to the fitted position.
        This offset allows for safety residual cut tuning. Offsets are recorded as (obj1 - obj2).

        XY, XZ, YZ and XYZ measurements are recorded in separate histograms.

        Keyword arguments:
        key  - The string describing the relationship between obj1 and obj2.
        obj1 - The node state (the fitted position).
        obj2 - The node object (the measurement position).
        """

        if obj1.SetOK and obj2.SetOK:
            var = off1 = off2 = off3 = False

            # Free coordinates have a variance of ~1e38. See which one (if any) is free.
            if obj2.PositionVariance.X() > 100000:
                var = "YZ Measurements"
                off1 = obj1.Position.Y() - obj2.Position.Y()
                off2 = obj1.Position.Z() - obj2.Position.Z()
            elif obj2.PositionVariance.Y() > 100000:
                var = "XZ Measurements"
                off1 = obj1.Position.X() - obj2.Position.X()
                off2 = obj1.Position.Z() - obj2.Position.Z()
            elif obj2.PositionVariance.Z() > 100000:
                var = "XY Measurements"
                off1 = obj1.Position.X() - obj2.Position.X()
                off2 = obj1.Position.Y() - obj2.Position.Y()
            else:
                var = "XYZ Measurements"
                off1 = obj1.Position.X() - obj2.Position.X()
                off2 = obj1.Position.Y() - obj2.Position.Y()
                off3 = obj1.Position.Z() - obj2.Position.Z()

            self.histos[self.det]["Offsets"][key][self.matched][var][var[0]].Fill(off1)
            self.histos[self.det]["Offsets"][key][self.matched][var][var[1]].Fill(off2)

            if var.find("XYZ") != -1:
                self.histos[self.det]["Offsets"][key][self.matched][var][var[2]].Fill(off3)

        return

    # _________________________________________________________________________
    def FillGlobalOffsetHists(self, key, node):
        """
        Record the positional offsets between a global node's state and the node's object.

        The node object relates to the measurement, and the node state to the fitted position.
        This offset allows for safety residual cut tuning. Offsets are recorded as (obj1 - obj2).

        XY, XZ, YZ and XYZ measurements are recorded in separate histograms.

        Keyword arguments:
        key  - The string describing the relationship between obj1 and obj2.
        obj1 - The node state (the fitted position).
        obj2 - The node object (the measurement position).
        """

        obj1 = node.NodeState
        obj2 = node.ObjectState

        if obj1.SetOK and obj2.SetOK:
            var = off1 = off2 = off3 = False

            if obj2.PositionVariance.X() > 100000:
                var = "YZ Measurements"
                off1 = obj1.Position.Y() - obj2.Position.Y()
                off2 = obj1.Position.Z() - obj2.Position.Z()
            elif obj2.PositionVariance.Y() > 100000:
                var = "XZ Measurements"
                off1 = obj1.Position.X() - obj2.Position.X()
                off2 = obj1.Position.Z() - obj2.Position.Z()
            elif obj2.PositionVariance.Z() > 100000:
                var = "XY Measurements"
                off1 = obj1.Position.X() - obj2.Position.X()
                off2 = obj1.Position.Y() - obj2.Position.Y()
            else:
                var = "XYZ Measurements"
                off1 = obj1.Position.X() - obj2.Position.X()
                off2 = obj1.Position.Y() - obj2.Position.Y()
                off3 = obj1.Position.Z() - obj2.Position.Z()

            self.histos["Global"]["Global object"]["Offsets"][key][var][var[0]].Fill(off1)
            self.histos["Global"]["Global object"]["Offsets"][key][var][var[1]].Fill(off2)

            if var.find("XYZ") != -1:
                self.histos["Global"]["Global object"]["Offsets"][key][var][var[2]].Fill(off3)

        return


    # _________________________________________________________________________
    def FillPositionHists(self, key, object, dodir = True):
        """
        Fill the 2D position histograms and 1D direction histograms. 

        Keyword arguments:
        key    - Description of what this position relates to.
        object - The consituent to get the position from.
        dodir  - True (False) if we (do not) want to record the direction as well as the position.
        """

        if object.SetOK:
            position  = object.Position
            direction = object.Direction

            self.histos[self.det]["Positions"][key]["ZX"].Fill(position.Z(), position.X())
            self.histos[self.det]["Positions"][key]["ZY"].Fill(position.Z(), position.Y())
            self.histos[self.det]["Positions"][key]["XY"].Fill(position.X(), position.Y())

            if dodir:
                self.histos[self.det]["Directions"][key]["CosTheta"].Fill(direction.CosTheta())
                self.histos[self.det]["Directions"][key]["Phi"].Fill(direction.Phi())

            try:
                # Record objects that are clearly reconstructed in bad places.
                if self.det.find("TPC") != -1: 
                    grtf_tools.PrintEvent(self, "_TPC_location.txt", key + " (" + str(position.X()) + ", " +str(position.Y()) + ", " +str(position.Z()) + ")")

                    if math.fabs(direction.CosTheta()) < 0.001:
                        grtf_tools.PrintEvent(self, "_TPC_dir0.txt", key + " (" + str(direction.X()) + ", " +str(direction.Y()) + ", " +str(direction.Z()) + ")")

                if self.det.find("TPC") != -1 and math.fabs(position.Y()) > 1500 and key.find("used by") != -1:
                    grtf_tools.PrintEvent(self, "_TPC_absY.gt.1500.txt", key + " (" + str(position.X()) + ", " +str(position.Y()) + ", " +str(position.Z()) + ")")

                if self.det.find("TPC") != -1 and math.fabs(position.X()) > 1500 and key.find("used by") != -1:
                    grtf_tools.PrintEvent(self, "_TPC_absX.gt.1500.txt", key + " (" + str(position.X()) + ", " +str(position.Y()) + ", " +str(position.Z()) + ")")

                if self.det.find("P0D") != -1 and (math.fabs(position.X()) > 1500 or math.fabs(position.Y()) > 1500) and key.find("used by") != -1:
                    grtf_tools.PrintEvent(self, "_P0D_absXorY.gt.1500.txt", "P0D refitted position (" + str(position.X()) + ", " +str(position.Y()) + ", " +str(position.Z()) + ")")

                if self.det.find("BSMRD") != -1 and position.Y() > -1500 and key.find("used by") != -1:
                    grtf_tools.PrintEvent(self, "_BSMRD_Y.gt.-1500.txt", "BSMRD position (" + str(position.X()) + ", " +str(position.Y()) + ", " +str(position.Z()) + ")")
            except OverflowError:
                # The position may be so large that math.fabs() can't deal with it. There are plenty of cases
                # that are bad but not *that* bad, so just catch the error and carry on.
                pass

        return

    # _________________________________________________________________________
    def GenerateEfficiencyPlots(self):
        """
        Fill the histograms related the efficiency of refitting by RECPACK. 

        The "Total" and "Refitted" histograms should already be filled and have
        the same X axis range, so a simple bin-by-bin filling is possible.
        """

        for det in self.histos.iterkeys():
            if det == "Global" or det == "Event":
                continue

            for dir in self.histos[det]["Efficiency of refitting"].itervalues():
                for bin in xrange(dir["Total"].GetXaxis().GetNbins() + 1):
                    total = dir["Total"].GetBinContent(bin)
                    refitted = dir["Refitted"].GetBinContent(bin)
                    if total > 0:
                        eff = refitted / total
                        error = math.sqrt((eff * (1 - eff)) / total)
                        dir["Eff"].SetBinContent(bin, eff)
                        dir["Eff"].SetBinError(bin, error)

        return

    # _________________________________________________________________________
    def WriteResultsToOutputFile(self):
        """
        Called when all events have been processed. 
        
        Makes efficiency plots for the refitting efficiency, then writes out all
        the histograms to an output file.
        """

        if self.rootOutput:
            self.rootOutput.cd()

            for histo in self.listOfHistosAndOtherObjects:
                histo.Write()

            self.GenerateEfficiencyPlots()
            grtf_tools.RecurseOutput(self.histos, self.rootOutput)

            self.rootOutput.Close()
        else:
            print "Unable to find output file. Tree not saved!"

        return
