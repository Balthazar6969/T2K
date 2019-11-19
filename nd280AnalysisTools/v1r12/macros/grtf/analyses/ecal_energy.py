import ROOT
import math
import re
from userAnalysisBase import userAnalysisBase
from grtf.tools import grtf_tools

class ecal_energy(userAnalysisBase):
    """
    GRTF module for testing ECal energy MIP scale.

    This analysis tests the energy deposition of MIP-like objects
    passing through the DsECal. We would ideally like to see a peak in the
    EDeposit spectrum around 1. Tracks are chosen such that they pass
    through at least FGD1-->DsECal, so should be pretty MIPPpy.
    
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
        None
        """

        # Call __init__ in base class
        userAnalysisBase.__init__(self, options)
        # Make TChains of the oaAnalysis trees to be read, and append to i
        # oaAnalysisModuleTChains
        self.ReconPerfEval = ROOT.TChain("ReconDir/ReconPerfEval")
        self.oaAnalysisModuleTChains.append(self.ReconPerfEval)
        self.DAQ = ROOT.TChain("HeaderDir/BasicDataQuality")
        self.oaAnalysisModuleTChains.append(self.DAQ)

    # _________________________________________________________________________
    def BookHistogramsAndAnalysisSteps(self):
        """Create the histograms we'll store the data in."""

        self.listOfHistosAndOtherObjects = []

        # This list contains the moduels we should generate histograms for. For example the analysis could
        # easily be extended to the FGDs by only chaning this line.
        self.modules = ["(DSECal)"]

        # All histograms are stored in this multi-level dictionary. The structure of the dictionary defines a 
        # tree-like structure that will be written out as a tree of TDirectories. Each directory eventually 
        # contains a single histogram. 
        self.histos = {}
        self.histos["ecal_energy"] = {}

        for ecal in self.modules:
            self.histos["ecal_energy"][ecal] = {}
            self.histos["ecal_energy"][ecal]["EDeposit"] = ROOT.TH1F("hEDeposit" + ecal, "EDeposit", 100, 0, 250)
            self.histos["ecal_energy"][ecal]["NumHits"] = ROOT.TH1F("hNumHits" + ecal, "NumHits", 50, 0, 50)
            self.histos["ecal_energy"][ecal]["EDepositPerHit"] = ROOT.TH1F("hEDepositPerHit" + ecal, "EDeposit/NumHits", 100, 0, 5)

        # Keep track of the total number of events we look at, as this is needed for scaling plots when making Data/MC comparisons.
        self.numEvents = {"All" : 0}

    def EventAnalysis(self):
        """
        Main event analysis function, called for each event.
        """

        # Require a good data quality flag
        if self.DAQ.ND280OffFlag > 0:
            return

        if self.ReconPerfEval.NGlobalReconObject > 25:
            return

        self.numEvents["All"] += 1

        for obj in self.ReconPerfEval.GlobalReconObject:
            # Only look at sucessful objects that go (at least) FGD1-->DsECal, ensuring they are MIPpy.
            if obj.SetOK and obj.StatusString.find("success") == -1:
                continue

            if obj.SubdetectorString.find("TPC2-TPC3-FGD1-FGD2-DSECal") != -1:
                for con in obj.Constituents:
                    for module in self.modules:
                        if con.DetectorName == module:
                            deposit = con.EDeposit
                            numhits = con.NumHits
                            self.histos["ecal_energy"][module]["EDeposit"].Fill(deposit)
                            self.histos["ecal_energy"][module]["NumHits"].Fill(numhits)
                            self.histos["ecal_energy"][module]["EDepositPerHit"].Fill(deposit/float(numhits))


        return

    def WriteResultsToOutputFile(self):
        """
        Write out the histograms and number of events to the output file.
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
