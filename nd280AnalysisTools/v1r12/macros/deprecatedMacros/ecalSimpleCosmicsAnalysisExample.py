import sys
import optparse
import os
import math # cos, log, pi etc
import logging # print with logging.debug/info/warning/error/critical('message')

import ROOT
# import the userAnalysisBase class from userAnalysisBase.py
from userAnalysisBase import userAnalysisBase 

# class name must be the same as the filename without the .py
class ecalSimpleCosmicsAnalysisExample(userAnalysisBase): # inherits from userAnalysisBase
    """\
ecalSimpleCosmicsAnalysisExample.py
This is a simple ECAL cosmics distribution analysis macro.
Derives from userAnalysisBase, and is called from runAnalyses.py

runAnalyses.py -a ecalSimpleCosmicsAnalysisExample inputoaAnalysisFile.root
    """

    ### __init__ gets called automatically upon instantiation
    def __init__(self, options):
        """\
Specifies further trees to be added to oaAnalysisModuleTChains, which is
set up in the base class userAnalysisBase.__init__
Needs to be passed the command line options from optionParser.
        """
        ### Call __init__ in base class
        userAnalysisBase.__init__(self, options)
        ###############################################################
        ### Specify additional oaAnalysis modules to read, and append to oaAnalysisModuleTChains
        # Truth information about all the vertices
        # self.truthVertices = ROOT.TChain("TruthDir/Vertices"); self.oaAnalysisModuleTChains.append(self.truthVertices)
        # Truth information about the trajectories
        # self.truthTrajectories = ROOT.TChain("TruthDir/Trajectories"); self.oaAnalysisModuleTChains.append(self.truthTrajectories)
        # TrackerECal Reconstruction
        self.reconTrackerECal = ROOT.TChain("ReconDir/TrackerECal"); self.oaAnalysisModuleTChains.append(self.reconTrackerECal)

    def BookHistogramsAndAnalysisSteps(self):
        ####################################################################
        # Define analysis steps
        #self.listOfAnalysisSteps.extend( # "No Cuts" is already defined
                #["Vertex", "Negative TPC Track", "FGD Pion"])
        
        ####################################################################
        # Define some histograms
        self.hCosmicXYPosition = ROOT.TH2F("xyposition","Reconstructed Track Position", 300, -1500, 1500, 300, -1500, 1500)
        self.hCosmicXYPosition.SetXTitle("Track X Postion [mm]")
        self.hCosmicXYPosition.SetYTitle("Track Y Postion [mm]")
        self.hCosmicXYDirection = ROOT.TH2F("xydirection","Reconstructed Track Direction", 100, -1, 1, 100, -1, 1)
        self.hCosmicXYDirection.SetXTitle("Track Direction X");
        self.hCosmicXYDirection.SetYTitle("Track Direction Y");
        self.hCosmicAMR = ROOT.TH1F("amr","Reconstructed Track AMR", 100, 0, 100);
        self.hCosmicAMR.SetXTitle("Track Length / Width ratio"); 
        self.hCosmicNumHits = ROOT.TH1F("numhits","Reconstructed Track NHits", 100, 0, 100);
        self.hCosmicNumHits.SetXTitle("Numbers of hits in Reconstructed Tracks"); 
        self.hCosmicTrackLength = ROOT.TH1F("length","Reconstructed Track Length", 100, 0, 1000);
        self.hCosmicTrackLength.SetXTitle("Length of Reconstructed Tracks [mm]"); 
        self.hCosmicTrShval = ROOT.TH1F("trshval","Reconstructed Track NN Track-Shower Output", 100, -10, 10);
        self.hCosmicTrShval.SetXTitle("Track-Shower PID NN Output"); 

        self.listOfHistosAndOtherObjects = [self.hCosmicXYPosition, 
            self.hCosmicXYDirection,
            self.hCosmicAMR, self.hCosmicNumHits, 
            self.hCosmicTrackLength,
            self.hCosmicTrShval]

        # Declare cut counters, which are accessed using their names
        #self.RegisterCutCounter("True CCQE Events in Fiducial")


        # Define other quantities
        #self.trackerAnalysisUtil = ROOT.TTrackerAnalysisUtils()
        # defined fiducial within active FGD volume:

    def EventAnalysis(self):
        ################################################################
        # abbreviations for use within this function
        reconTrackerECal = self.reconTrackerECal

        ################################################################
        # analysis utilities
        #trackerAnalysisUtil = self.trackerAnalysisUtil

        ################################################################
        # Initialise event-level quantities
        #self.ResetCutCounter(
        hasPassedCut = {}
        # for step in self.listOfAnalysisSteps:
            # hasPassedCut[step] = False
        # hasPassedCut["No Cuts"] = True

        for itrack in xrange(reconTrackerECal.NReconTracks):
          self.hCosmicXYPosition.Fill(reconTrackerECal.ReconTracks[itrack].Position.X(),
                                 reconTrackerECal.ReconTracks[itrack].Position.Y())
          self.hCosmicXYDirection.Fill(reconTrackerECal.ReconTracks[itrack].Direction.X(),
                                 reconTrackerECal.ReconTracks[itrack].Direction.Y())
          self.hCosmicAMR.Fill(reconTrackerECal.ReconTracks[itrack].AMR)
          self.hCosmicNumHits.Fill(reconTrackerECal.ReconTracks[itrack].NumHits);
          self.hCosmicTrackLength.Fill(reconTrackerECal.ReconTracks[itrack].Track_Length);
          self.hCosmicTrShval.Fill(reconTrackerECal.ReconTracks[itrack].TrShval);


    def WriteResultsToOutputFile(self):
        ####################################################################
        # Write out analysis results
        if self.rootOutput:
            self.rootOutput.cd()
            for histo in self.listOfHistosAndOtherObjects:
                histo.Write()
                
            summary = ROOT.TObjString(self.GetAnalysisCutResults())
            summary.Write("AnalysisSummary")
            print summary
            self.rootOutput.Close()
            print "Output saved in " + self.options.output

    def GetAnalysisCutResults(self):
        """\
Prints out a summary of results, using the analysis steps and cut counters
"""

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

        result += """\
=======================================================================
"""
        result += "-------- Purity: \n"
        for step in self.listOfAnalysisSteps:
            if self.dictOfCutCounters["Signal & Background"][step]:
                result += ("%-20s:  %10d/%-10d  %4.1d%%\n" % \
                           (step,  self.dictOfCutCounters["True CCQE Events in Fiducial"][step], \
                            self.dictOfCutCounters["Signal & Background"][step], \
                            100.0 * float(self.dictOfCutCounters["True CCQE Events in Fiducial"][step]) \
                            /float(self.dictOfCutCounters["Signal & Background"][step])))        
        logging.info(result)

        return result
