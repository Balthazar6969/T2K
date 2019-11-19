import sys
import optparse
import os
import math # cos, log, pi etc
import string
from array import array

import re

import ROOT
from ROOT import TObjString
# import the userAnalysisBase class from userAnalysisBase.py
from userAnalysisBase import userAnalysisBase 


# class name must be the same as the filename without the .py
class ecalBeam(userAnalysisBase): # inherits from userAnalysisBase
    """\
Derives from userAnalysisBase, and is called from runAnalyses.py

Produces two trees in the output file. These trees are read in
the root macro ecalBeam.C
>> ecalBeam("output.root");
which produces the plots made for Neutrino 2010

This has been run with nd280soft v7r19p1, on the runs/subruns
for neutrino run 31+32 that have been flagged by the Data Quality
group as good subruns

Author: Pawel Guzowski <p.guzowski07@imperial.ac.uk>

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
        self.ReconECal = ROOT.TChain("ReconDir/TrackerECal"); self.oaAnalysisModuleTChains.append(self.ReconECal)
        self.Basic = ROOT.TChain("HeaderDir/BasicHeader"); self.oaAnalysisModuleTChains.append(self.Basic)

    def BookHistogramsAndAnalysisSteps(self):
        ####################################################################
            
        
        self.RecoTime = array("f",[0.])
        self.RunNumber = array("i", [0])
        self.Bunch = array("i", [0])
        self.GoodSpill = array("i", [0])
        
        self.EventTime = array("i",[0])
        self.EventNumber = array("i",[0])
        self.CumulativeNClus = array("i", [0])
        self.CumulativePOTDAQd = array("f", [0.])
        self.daq = 0.
        self.InstNClus = array("i", [0,0,0,0,0,0,0,0])
        self.InstPOT = array("f", [0.,0.,0.,0.,0.,0.,0.])
        self.CumulativeSpill = array("i", [0])


        self.outputTree = ROOT.TTree("cluster_tree","Tree summarising cluster level analysis")
        self.outputTree.Branch("event_time", self.EventTime, "event_time/I")
        self.outputTree.Branch("time", self.RecoTime, "time/F")
        self.outputTree.Branch("bunch", self.Bunch, "bunch/I")
        self.outputTree.Branch("instpot", self.InstPOT, "instpot[7]/F") 
       
        
        self.outputTree5 = ROOT.TTree("event_tree", "Tree summarising event level analysis")
        self.outputTree5.Branch("time", self.EventTime, "time/I")
        self.outputTree5.Branch("cumclus", self.CumulativeNClus, "cumclus/I")
        self.outputTree5.Branch("cumpotdaqd", self.CumulativePOTDAQd, "cumpotdaqd/F") 
        self.outputTree5.Branch("instclus", self.InstNClus, "instclus[8]/I")
        self.outputTree5.Branch("instpot", self.InstPOT, "instpot[7]/F")
        self.outputTree5.Branch("run", self.RunNumber, "run/I")
        self.outputTree5.Branch("event", self.EventNumber, "event/I")
        self.outputTree5.Branch("gs", self.GoodSpill, "gs/I") 
        
        
        #############################################################################
        # Load info from beamline from
        # http://www.t2k.org/nd280/physics/beam/beam-quick-summary-data/2010/Beam%20Summary%20Data/Good%20Spill%20List%20in%20ND280
        #############################################################################
        self.updated_bsd = dict()
        regexp = re.compile(r'(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)')
	infiles = ["Run31_goodspill.txt", "Run32_goodspill.txt", "Run33_goodspill.txt", "Run34_goodspill.txt"]
        for fn in infiles:
	  infile = open(fn, "r")
          for line in infile:
            res = regexp.search(line)
            if res != None:
              t = int(res.group(5))
              p = float(res.group(6))
              self.updated_bsd[t] = p
          infile.close()
        
        
        
    def EventAnalysis(self):
        ################################################################
        # abbreviations for use within this function
        reco = self.ReconECal
        ################################################################

        # This takes out the "run start"/"run end" events
        if self.Basic.EventNumber < 0:
          return

        

        self.EventTime[0] = self.Basic.EventTime
        self.RunNumber[0] = self.Basic.RunID
        
        
        self.InstNClus[7] = 0
        self.InstNClus[0] = 0
        
        
        if self.Basic.EventTime in self.updated_bsd:
          pot = self.updated_bsd[self.Basic.EventTime]
          for i in xrange(1, 7):
            self.InstPOT[i] = 0
            self.InstNClus[i] = 0
          self.GoodSpill[0] = 1
        else:
          return
        
        
        if self.GoodSpill[0] > 0:
          self.daq += pot
        
        
        
        self.InstPOT[0] = pot
        self.CumulativePOTDAQd[0] = self.daq
        self.EventNumber[0] = self.Basic.EventNumber
        
        ngoodclus = 0
        
        for i in xrange(reco.NReconObjects):
          p = reco.ReconObject[i]
          t = p.AverageHitTime 
          self.RecoTime[0] = t
          self.Bunch[0] = self.WhichBunch(t)
          if self.Bunch[0] > 0:
            ngoodclus += 1
            self.InstNClus[self.Bunch[0]] += 1
          else:
            self.InstNClus[7] += 1
          
          self.outputTree.Fill()
          
        
        if self.GoodSpill[0] > 0:
          self.CumulativeNClus[0] += ngoodclus
        self.InstNClus[0] = ngoodclus
        self.outputTree5.Fill()
        
        
    def WhichBunch(self, t):
        # Determine if a cluster at time t is part of which beam bunch.
        # these numbers are taken by performing a gaussian fit to each beam bunch peak
        # a cut of +- 50 ns is them made around each peak
        beamTimes = [2837.,3419.,4003.,4585.5,5168.5,5751.]
        for i in xrange(len(beamTimes)):
          bt = beamTimes[i]
          if abs(t - bt) < 50.:
            return i + 1
        return 0
        

    def WriteResultsToOutputFile(self):
        ####################################################################
        # Write out analysis results
        print "Looking for output file:"
        if self.rootOutput:
            self.rootOutput.cd()
            self.outputTree.Write()
            self.outputTree5.Write()
            print "Found output file. Tree has been saved!"
        else:
            print "Unable to find output file. Tree not saved!"
            
