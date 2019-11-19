"""\
A basis for user analyses based on reading out oaAnalysis files. Works in 
conjunction with runAnalyses.py, and a derived user analysis class.
"""
import sys
import optparse
import os
import math # cos, log, pi etc
import logging # print with logging.debug/info/warning/error/critical('message')
import ROOT

processReportCounts = [10,32,100,316,1000]

class userAnalysisBase:
    """\
Base class for user analyses, which read out oaAnalysis root files,
and produce analysis output.
    """

    def __init__(self, options):
        """\
Sets up a list of TTrees (which correspond to oaAnalysis modules) in the input
files to read in. This list is called "oaAnalysisModuleTChains". The
"BasicHeader" TTree is treated a bit specially, and is always read in, and is
the sole element of "oaAnalysisModuleTChains" at this point.
Derived classes can populate this list with more TTrees.
I say TTree here, but I actually use TChains so that multiple .root files are
concatenated when being read in.
Needs to be passed the command line options from optionParser.

        """
        # class attributes
        self.options = options
        self.basicHeader = ROOT.TChain("HeaderDir/BasicHeader")
        self.eventChain = self.basicHeader # used as a basis for adding friend chains
        # more oaAnalysis modules to be read can be added in derived classes
        self.oaAnalysisModuleTChains = [self.basicHeader]

        # lists and dictionaries to be populated by derived classes
        self.listOfAnalysisSteps = ["No Cuts"] # to be appended to with "5 negative tracks" etc
        self.dictOfCutCounters = {} # "Signal & Background" will be added automatically.
                                    # To be appended  with "True CCQE-like in FGD1 Fiducial" etc
        self.dictOfEfficiencyGraphs = {}
        self.dictOfPurityGraphs = {}
        self.dictOfEfficiencyAndPurityCanvases = {}
        self.listOfHistosAndOtherObjects = []

        self.generatorUtils = None

        # Verbosity:
        # Set the logging level
        LEVELS = {'debug': logging.DEBUG, 'info': logging.INFO, 'warning': logging.WARNING,
                  'error': logging.ERROR, 'critical': logging.CRITICAL}
        logging.basicConfig(level=LEVELS.get(options.loglevel, logging.NOTSET))

    def OpenInputFiles(self, inputfiles):
        """\
Having been given a list of input files, does a TChain::AddFile on each one, so
that they can be read in.
Also guesses the generatorType depending on the first input filename. This
should be replaced by something that uses the file catalogue to determine this.
        """
        hasTree = [1]*len(self.oaAnalysisModuleTChains)
        for i in xrange(len(inputfiles)):
            inputfile = inputfiles[i]
            if os.access(inputfile, os.R_OK) == True: # can be read
                for j in xrange(len(self.oaAnalysisModuleTChains)):
                    # Check if TTree actually exists in this file first
                    moduleTChain = self.oaAnalysisModuleTChains[j]
                    f = ROOT.TFile.Open(inputfile)
                    directory,tree = moduleTChain.GetName().split('/')
                    d = f.GetDirectory(directory)
                    if d.GetListOfKeys().Contains(tree):

                        if hasTree[j] == 0:
                            logging.error("%s not contained in %s, TChains will have different lengths and analysis will not be correct" % (tree,inputfile))
                            sys.exit(1)
                        moduleTChain.AddFile(inputfile)

                    else:
                        if i != 0 and hasTree[j] == 1:
                            logging.error("%s not contained in %s, TChains will have different lengths and analysis will not be correct" % (tree,inputfile))
                            sys.exit(1)
                        else:
                            logging.warn("%s not contained in %s" % (tree,inputfile))
                            hasTree[j] = 0

                    f.Close()
            else:
                logging.error("OpenInputFiles" + inputfile + " can't be read")
                sys.exit(1)
        # Chains please say "Hi" to each other:
        for moduleTChain in self.oaAnalysisModuleTChains:
            # this makes the chains (of trees) read in events in unison
            self.eventChain.AddFriend(moduleTChain)

        ### Chooses (or guesses) the right neutrino generator utility
        if self.options.generatorType == "None" and inputfiles[0].find("_gn_") >= 0:
           self.options.generatorType = "GENIE"
        elif self.options.generatorType == "None" and inputfiles[0].find("_nt_") >= 0:
           self.options.generatorType = "NEUT"
        elif self.options.generatorType == "None" and inputfiles[0].find("_nc_") >= 0:
           self.options.generatorType = "NUANCE"

        if self.options.generatorType == "NEUT":
            self.generatorUtils = ROOT.TNEUTAnalysisUtils()
        elif self.options.generatorType == "GENIE":
            self.generatorUtils = ROOT.TGENIEAnalysisUtils()
        elif self.options.generatorType == "NUANCE":
            self.generatorUtils = ROOT.TNUANCEAnalysisUtils()
        else:
            logging.warn("Generator type not specified, or guessed, but trying to carry on....")

        ### Prints some info before starting analysis
        logging.info("Entries in MC tree:  " + str(self.eventChain.GetEntries()))

    def OpenOutputFile(self):
        """\
Opens an output file as specified in the options. This file is typically not
overridden
        """
        self.rootOutput = False
        if self.options.output != None:
            self.rootOutput = ROOT.TFile(self.options.output,"RECREATE")
            if not self.rootOutput.IsOpen():
                sys.exit(1)

    def BookHistogramsAndAnalysisSteps(self):
        """\
Override this function in a derived class to specify histograms and analysis
steps, and any other initialisations.
        """
        logging.warning("""
Calling BookHistogramsAndAnalysisSteps in userAnalysisBase. Override in derived
class to specify your analysis
        """)

    def RegisterCutCounter(self, name):
        """\
Creates a new counter to keep track of the number of events selected at each
analysis step. Counters are initialised to 0. 
        """
        newCutCounter = {}
        self.dictOfCutCounters[name] = newCutCounter
        for step in self.listOfAnalysisSteps:
            newCutCounter[step] = 0

    def IncrementCutCounter(self, name, analysisstep, increment = 1):
        """\
Increments the cut counter labelled by "name", for the "analysis step",
by the optional value "increment"
        """
        self.dictOfCutCounters[name][analysisstep] += increment


    def GetCutCounter(self, name, analysisstep):
        """\
Get the cut counter labelled by "name", for the "analysis step"
        """
        return self.dictOfCutCounters[name][analysisstep]
    

    def ResetCutCounters(self):
        """\
Resets all the cut counters that have been registered.
Not normally needed because they are reset when initially 
registered
        """
        for step in self.listOfAnalysisSteps:
            for counterLabel in self.dictOfCutCounters:
                self.dictOfCutCounters[counterLabel][step] = 0


    def InitialiseAnalysis(self):
        """\
Books histograms, and specifies the cuts to be processed. Probably does not
needed to be overridden in derived class.
        """
        self.OpenOutputFile()
        self.BookHistogramsAndAnalysisSteps()
        self.RegisterCutCounter("Signal & Background")
        logging.info("Analysis Steps:\n    " + "\n    ".join(self.listOfAnalysisSteps))
        logging.info("Cut Counters:\n    " + "\n    ".join(self.dictOfCutCounters))
        logging.info("Histograms: ")
        for histo in self.listOfHistosAndOtherObjects:
            logging.info("    " + histo.GetName())


    def EventAnalysis(self):
        """\
Override this function in a derived class to specify event-by-event analysis.
        """
        logging.error("""
Calling EventAnalysis in userAnalysisBase. Must be overridden in derived class
and the user's analysis specified there. See ccqeAnalysis.py.
        """)

    def ProcessEventLoop(self):
        logging.info("\n### " + self.options.analysisName)
        logging.info(self.__doc__)
        ####################################################################
        # The main event loop
        self.entries = min(self.options.nEvents, 
                self.eventChain.GetEntries() - self.options.nSkip)
        logging.info("Processing " + str(self.entries) + " entries, starting at "\
                + str(self.options.nSkip))
        for entry in xrange(self.options.nSkip, self.options.nSkip + self.entries):
            if self.eventChain.LoadTree(entry) < 0: break
            if self.eventChain.GetEntry(entry) <= 0: break
            
            # Say something every 10% of events, first make self.entries divisible by 10
            nTest = self.entries - self.entries%10
            if nTest < 10:
                nTest = 10
            for count in processReportCounts:
                if entry == count:
                    logging.info("Events Processed " + str(entry))
            if entry % (nTest/10) == 0:
                logging.info("Events Processed " + str(entry))

            ################################################################
            # Call EventAnalysis in the derived class for each event
            self.EventAnalysis()

    def GetAnalysisCutResults(self):
        """\
Prints out a summary of results, using the analysis steps and cut counters
registered by the user. Can be overridden by the user for more detailed
summaries."""

        result = ''
        for counter in self.dictOfCutCounters:
            nFirstStep = self.dictOfCutCounters[counter]["No Cuts"]
            result += """\
=======================================================================
"""
            result += "-------- " + counter + ": \n";
            if not nFirstStep: nFirstStep = 1
            for step in self.listOfAnalysisSteps:
                if self.dictOfCutCounters[counter][step]:
                    result += ("%-20s:  %10d/%-10d  %4.1d%%\n" % \
                            (step,  self.dictOfCutCounters[counter][step], nFirstStep, \
                            100.0 * float(self.dictOfCutCounters[counter][step])/nFirstStep))
            logging.info(result)
        return result

    def FinishAnalysis(self):
        """\
This version calls the function CreateEfficiencyAndPurityPlots and
WriteResultsToOutputFile. Can be overridden by the user for 
customised functionality."""
        self.CreateEfficiencyAndPurityPlots()
        self.WriteResultsToOutputFile()

    def CreateEfficiencyAndPurityPlots(self):
        """\
Draws efficiency and purity plots."""
        # This code was adapted from that provided by Christian Hansen
        for counter in self.dictOfCutCounters:
            if counter == "Signal & Background": continue
            i = 0
            logging.info("USING counter in self.dictOfCutCounters: ", counter)
            nTotal = self.dictOfCutCounters[counter]["No Cuts"]
            self.dictOfEfficiencyGraphs[counter] = ROOT.TGraph(len(self.listOfAnalysisSteps))
            self.dictOfEfficiencyGraphs[counter].SetNameTitle(counter + "efficiencyPlot", counter + " Efficiency Plot")
            self.dictOfPurityGraphs[counter] = ROOT.TGraph(len(self.listOfAnalysisSteps))
            self.dictOfPurityGraphs[counter].SetNameTitle(counter + "purity Plot", counter + " Purity Plot")
            self.dictOfEfficiencyAndPurityCanvases[counter] = ROOT.TCanvas(counter + " Efficiency And Purity")
            self.dictOfEfficiencyAndPurityCanvases[counter].SetFillColor(10)
            self.dictOfEfficiencyAndPurityCanvases[counter].SetBorderSize(0)
            self.dictOfEfficiencyAndPurityCanvases[counter].SetBorderMode(0)
            self.listOfHistosAndOtherObjects.append(self.dictOfEfficiencyAndPurityCanvases[counter])

            if nTotal > 0:
                # Fill efficiency graph
                for step in self.listOfAnalysisSteps:
                    #eff = number true events in fiducial that were selected / number true events in fiducial
                    self.dictOfEfficiencyGraphs[counter].SetPoint(i, i+1, \
                                               float(self.dictOfCutCounters[counter][step]) / \
                                               float(nTotal)*100.0)
                    logging.info(counter + " efficiency: ", i, float(self.dictOfCutCounters[counter][step]) / \
                                               float(nTotal)*100.0)
                    #pur = number events in this counter that were selected / number signal & background events that were selected
                    if self.dictOfCutCounters[counter][step] > 0:
                        self.dictOfPurityGraphs[counter].SetPoint(i, i+1, \
                                                   float(self.dictOfCutCounters[counter][step]) / \
                                                   float(self.dictOfCutCounters["Signal & Background"][step])*100.0)
                    else:
                        self.dictOfPurityGraphs[counter].SetPoint(i, i+1, 0)
                             
                    i += 1 
                    
            # For labeling of graph's x-axis with appropriate analysis steps
            for idx, step in enumerate(self.listOfAnalysisSteps):
                idx_bin = self.dictOfEfficiencyGraphs[counter].GetXaxis().FindBin(idx+1)
                self.dictOfEfficiencyGraphs[counter].GetXaxis().SetBinLabel(idx_bin, step)
            
            self.dictOfEfficiencyGraphs[counter].Draw("APL*")
            self.dictOfEfficiencyGraphs[counter].GetXaxis().LabelsOption('u')
            self.dictOfEfficiencyGraphs[counter].SetMarkerStyle(22)
            self.dictOfEfficiencyGraphs[counter].SetMarkerColor(4)
            self.dictOfEfficiencyGraphs[counter].SetMarkerSize(3)
            self.dictOfEfficiencyGraphs[counter].GetYaxis().SetTitle("Percentage")
            self.dictOfEfficiencyGraphs[counter].SetTitle("Efficiency and Purity - " + counter)
            self.dictOfEfficiencyGraphs[counter].SetMaximum(110)
            self.dictOfEfficiencyGraphs[counter].SetMinimum(0)

            self.dictOfPurityGraphs[counter].Draw("L*")
            self.dictOfPurityGraphs[counter].SetMarkerStyle(21)
            self.dictOfPurityGraphs[counter].SetMarkerColor(2)
            self.dictOfPurityGraphs[counter].SetMarkerSize(3)

            leg = ROOT.TLegend(0.6,0.68,0.89,0.85);
            leg.SetFillColor(10);
            leg.AddEntry(self.dictOfEfficiencyGraphs[counter],"Efficiency","p");
            leg.AddEntry(self.dictOfPurityGraphs[counter],"Purity","p");
            leg.Draw("SAME");
            ROOT.SetOwnership(leg, 0)

    def WriteResultsToOutputFile(self):
        ####################################################################
        # Write out analysis results
        if self.rootOutput:
            self.rootOutput.cd()
            for histo in self.listOfHistosAndOtherObjects:
                histo.Write()
            summary = ROOT.TObjString(self.GetAnalysisCutResults())
            summary.Write("AnalysisSummary")
            logging.info(summary)
            self.rootOutput.Close()
            logging.info("Output saved in " + self.options.output)

       
    def DrawPlotsOnScreen(self):
# doesnt' work yet....
        pass
#        ROOT.SetMemoryPolicy( ROOT.kMemoryStrict )
#        # Change default style for the statistics box
#        ROOT.gStyle.SetStatW(0.30)
#        ROOT.gStyle.SetStatH(0.20)
#        ROOT.gStyle.SetStatColor(42)
#        while True:
#            fourHistos = [self.listOfHistosAndOtherObjects[i] \
#                    for i in xrange(min(4, len(self.listOfHistosAndOtherObjects)))]
#            print repr(self.listOfHistosAndOtherObjects)
#            print "### Opening new canvas"
#            canvas = ROOT.TCanvas()
#            pad1 = ROOT.TPad('pad1','This is pad1',0.02,0.52,0.48,0.98,21)
#            pad2 = ROOT.TPad('pad2','This is pad2',0.52,0.52,0.98,0.98,21)
#            pad3 = ROOT.TPad('pad3','This is pad3',0.02,0.02,0.48,0.48,21)
#            pad4 = ROOT.TPad('pad4','This is pad4',0.52,0.02,0.98,0.48,1)
#            pads = [pad1, pad2, pad3, pad4]
#            for i in xrange(len(fourHistos)):
#                pads[i].Draw()
#                pads[i].cd()
#                # fourHistos[i].Draw()
#                # canvas.Update()
#            del self.listOfHistosAndOtherObjects[0:4]
#            if not self.listOfHistosAndOtherObjects:
#                break
#
