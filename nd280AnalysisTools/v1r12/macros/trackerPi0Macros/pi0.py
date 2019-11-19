import sys
import optparse
import os
import math # cos, log, pi etc
import logging # print with logging.debug/info/warning/error/critical('message')
from array import array
import re

import ROOT
from ROOT import TMVA
# import the userAnalysisBase class from userAnalysisBase.py
from userAnalysisBase import userAnalysisBase 


DeltaFGD1 = { 31:-18.58, 32: 14.33, 33:  9.10, 34: 11.09, 36:  7.04, 37:30.71, 38:33.96, 1:11.44, 2:11.44 }
DeltaFGD2 = { 31:-16.18, 32: 17.37, 33: 12.49, 34: 14.26, 36: 11.87, 37:35.68, 38:39.50, 1:12.84, 2:12.84 }

DeltaTPC1 = { 31:-22.82, 32: 11.24, 33:  3.89, 34:  6.40, 36:-1.44, 37:22.21, 38:29.17, 1:9.96, 2:9.96 }
DeltaTPC2 = { 31:-16.97, 32: 16.41, 33: 11.08, 34: 12.67, 36:9.99, 37:32.27, 38:35.02, 1:13.62, 2:13.62 }
DeltaTPC3 = { 31:-12.41, 32: 16.39, 33: 12.35, 34: 14.10, 36:12.46, 37:32.22, 38:36.27, 1:12.34, 2:12.34 }

DeltaP0D  = { 31:-38.86, 32:-23.12, 33:-23.90, 34:-23.12, 36:-30.56, 37:-32.54, 38:-33.25, 1:-13.16, 2:-13.16 }
DeltaSMRD = { 31:-29.15, 32:-11.62, 33:-08.14, 34:-11.17, 36:-21.48, 37:-21.44, 38:-19.19, 1:-5.23, 2:-5.23 }

DeltaPECAL = { 36: -93.10, 37:-21.94, 38:-18.71, 1: -3.26, 2: -3.26 }
DeltaLECAL = { 36:-111.57, 37: -8.14, 38:-11.43, 1: -4.21, 2: -4.21 }
DeltaRECAL = { 36:-140.88, 37:-12.20, 38:-10.08, 1: -4.41, 2: -4.41}
DeltaTECAL = { 36:-112.78, 37: -4.41, 38: -1.97, 1: -4.05, 2: -4.05}
DeltaBECAL = { 36:-111.98, 37:-12.75, 38:-14.06, 1: -2.94, 2: -2.94}

FirstECalBunch = { 31:2848.12, 32:2830.25, 33:2822.74, 34:2832.76, 36:2847.20, 37:2982.97, 38:2988.75, 1: 2744.46, 2: 2744.46 }
SigmaECalBunch = { 31:10.93, 32:11.47, 33:13.01, 34:12.86, 36:20.43, 37:21.82, 38:18.56, 1: 9.96, 2: 9.96 }

MCTruthRecoOffset = FirstECalBunch[1] - 78.88;

FGD1MZ = 100.
FGD1PZ = 500.
FGD2MZ = 1440
FGD2PZ = 1840.
FGDX = 1000.
FGDY = 1000.

# GeomId of DsECal
DsECalModule = 134217728

DeltaPi0TracksTime = 150.;

MCRunMatcher = re.compile('_9[01]([0-9])([01][0-9][0-9][0-9][0-9])-([0-9][0-9][0-9][0-9])_')

BranchType = { 'f':'F', 'i':'I', 'b':'O' }

def GetBunchSigma(t, run, bunch):
    bunchmean = FirstECalBunch[run] + (bunch-1)*582.
    bunchsigma = SigmaECalBunch[run]
    return (t-bunchmean)/bunchsigma

def FindAdjustedDetector(pid, run):
    if pid.NECALs > 0:
      return 1
    if pid.NP0Ds > 0:
      return 2
    if pid.NSMRDs > 0:
      return 3
    if pid.NFGDs > 0:
      adjtime = 0.
      for f in xrange(pid.NFGDs):
        fgd = pid.FGD[f]
        if fgd.Detector == 1:
          adjtime = 4
        if fgd.Detector == 2:
          adjtime = 5
      return adjtime
    if pid.NTPCs > 0:
      adjtime = 0.
      for t in xrange(pid.NTPCs):
        tpc = pid.TPC[t]
        if tpc.Detector == 1:
          adjtime = 6
        if tpc.Detector == 2:
          adjtime = 7
        if tpc.Detector == 3:
          adjtime = 8
      return adjtime
      
    return 0;


# class name must be the same as the filename without the .py
class pi0(userAnalysisBase): # inherits from userAnalysisBase

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
        self.truthVertices = ROOT.TChain("TruthDir/Vertices"); self.oaAnalysisModuleTChains.append(self.truthVertices)
        self.truthTrajectories = ROOT.TChain("TruthDir/Trajectories"); self.oaAnalysisModuleTChains.append(self.truthTrajectories)
        self.Global = ROOT.TChain("ReconDir/Global"); self.oaAnalysisModuleTChains.append(self.Global)
        self.Pi0 = ROOT.TChain("ReconDir/TrackerECalPi0"); self.oaAnalysisModuleTChains.append(self.Pi0)
        self.BSD = ROOT.TChain("HeaderDir/BeamSummaryData"); self.oaAnalysisModuleTChains.append(self.BSD)
        self.DQ = ROOT.TChain("HeaderDir/BasicDataQuality"); self.oaAnalysisModuleTChains.append(self.DQ)
        self.ECal = ROOT.TChain("ReconDir/TrackerECal"); self.oaAnalysisModuleTChains.append(self.ECal)
        self.FGD = ROOT.TChain("ReconDir/FGDOnly"); self.oaAnalysisModuleTChains.append(self.FGD)

        self.RooTracker = None
        if options.generatorType == "NEUT":
          self.RooTracker = ROOT.TChain("TruthDir/NRooTrackerVtx"); self.oaAnalysisModuleTChains.append(self.RooTracker)
        elif options.generatorType == "GENIE":
          self.RooTracker = ROOT.TChain("TruthDir/GRooTrackerVtx"); self.oaAnalysisModuleTChains.append(self.RooTracker)
        
        self.LastNumInteractions = 0
        self.UsedInt = dict()
        self.FilePOT = dict()

        self.rootTrees = {}

        ROOT.gROOT.ProcessLine(".L pi0_mva_Likelihood.class.C+")


        v = ROOT.std.vector("string")()
        for s in [ "hPid", "lPid", "hQskew", "lQskew", "hEMlik", "lEMlik", "hPoint", "hFL", "lFL", "hMT", #"lMT", 
            "hAngle", "lAngle", "hConeA", "lConeA", #"hShowW", "lShowW", 
            "hMeanP", "lMeanP", "hNH", "lNH", "hFid", "lFid", "hThrust", "lThrust", "Asymm", "CosTh", "ClusM", "DeltaD" ]:
          v.push_back(s)


        self.TMVALikelihood = ROOT.ReadLikelihood(v)


    def BookTree(self, name, title=None):

        if not title:
          title = name

        tree = ROOT.TTree(name, title)


        tree.Branch("Run",     self.GlobalRunN,    "Run/I")
        tree.Branch("Event",   self.GlobalEventN,  "Event/I")
        tree.Branch("BeamRun", self.GlobalBeamRun, "BeamRun/I")
        tree.Branch("Bunch",   self.GlobalBunchN,  "Bunch/I")

        self.listOfHistosAndOtherObjects.append(tree)

        self.rootTrees[name] = tree

        return tree

    def BookBranch(self, tree, var_name, int_or_float, array_size=1):

        tree_name = tree.GetName()

        full_var_name = tree_name + var_name

        array_str = ""
        if array_size > 1:
          array_str = "[" + str(array_size) + "]"

        if full_var_name in self.__dict__:
          raise NameError(var_name + " already in tree")

        self.__dict__[full_var_name] = array(int_or_float, [0] * array_size)

        tree.Branch(var_name, self.__dict__[full_var_name], var_name + array_str + "/" + BranchType[int_or_float])

    def BookHistogramsAndAnalysisSteps(self):
        ####################################################################
        # Define analysis steps
        self.listOfAnalysisSteps.extend( # "No Cuts" is already defined
                ["Good Spill", "Data Quality", "Preselected", "Pi0 Recon"])
        
        ####################################################################
        # Define some histograms

        # Global quantites

        self.GlobalRunN    = array("i", [0])
        self.GlobalEventN  = array("i", [0])
        self.GlobalBeamRun = array("i", [0])
        self.GlobalBunchN  = array("i", [0])

        ##################
        #                #
        #    Pi0 Tree    #
        #                #
        ##################

        tree = self.BookTree("Pi0Tree", "Pi0 Recon & Truth Properties")

        # Reco Quantites

        self.BookBranch(tree, "RecoMass", "f")
        self.BookBranch(tree, "RecoP", "f")
        self.BookBranch(tree, "RecoX", "f")
        self.BookBranch(tree, "RecoY", "f")
        self.BookBranch(tree, "RecoZ", "f")
        self.BookBranch(tree, "RecoT", "f")
        self.BookBranch(tree, "RecoDFGD", "f")
        self.BookBranch(tree, "RecoAltMass", "f")
        self.BookBranch(tree, "RecoAltP", "f")
        self.BookBranch(tree, "RecoAltX", "f")
        self.BookBranch(tree, "RecoAltY", "f")
        self.BookBranch(tree, "RecoAltZ", "f")
        self.BookBranch(tree, "RecoAltDFGD", "f")
        self.BookBranch(tree, "RecoPhotonDeltaT", "f")
        self.BookBranch(tree, "RecoPhotonDeltaX", "f")
        self.BookBranch(tree, "RecoPhotonDeltaY", "f")
        self.BookBranch(tree, "RecoPhotonDeltaZ", "f")
        self.BookBranch(tree, "RecoPhotonDeltaDist", "f")
        self.BookBranch(tree, "RecoVertexResolutionAlongHigh", "f")
        self.BookBranch(tree, "RecoVertexResolutionAlongLow", "f")
        self.BookBranch(tree, "RecoVertexResolutionAlongThird", "f")
        self.BookBranch(tree, "RecoPhotonInvariantMass", "f")

        self.BookBranch(tree, "TMVALikelihood", "f")


        for photon in ["High", "Low"]:
          self.BookBranch(tree, "Reco" + photon + "UncorrectedEnergy", "f")
          self.BookBranch(tree, "Reco" + photon + "Energy", "f")
          self.BookBranch(tree, "Reco" + photon + "Thrust", "f")
          self.BookBranch(tree, "Reco" + photon + "PosX", "f")
          self.BookBranch(tree, "Reco" + photon + "PosY", "f")
          self.BookBranch(tree, "Reco" + photon + "PosZ", "f")
          self.BookBranch(tree, "Reco" + photon + "Time", "f")
          self.BookBranch(tree, "Reco" + photon + "DirectionX", "f")
          self.BookBranch(tree, "Reco" + photon + "DirectionY", "f")
          self.BookBranch(tree, "Reco" + photon + "DirectionZ", "f")
          self.BookBranch(tree, "Reco" + photon + "IncidenceAngle", "f")
          self.BookBranch(tree, "Reco" + photon + "Pid", "f")
          self.BookBranch(tree, "Reco" + photon + "Had", "f")
          self.BookBranch(tree, "Reco" + photon + "AngularResolution", "f")
          self.BookBranch(tree, "Reco" + photon + "ConstrainedResolution", "f")
          self.BookBranch(tree, "Reco" + photon + "SkewResolution", "f")
          self.BookBranch(tree, "Reco" + photon + "FGDResolution", "f")
          self.BookBranch(tree, "Reco" + photon + "TrackerResolution", "f")
          self.BookBranch(tree, "Reco" + photon + "GlobalResolution", "f")
          self.BookBranch(tree, "Reco" + photon + "Resolution", "f")
          self.BookBranch(tree, "Reco" + photon + "AltResolution", "f")
          self.BookBranch(tree, "Reco" + photon + "NumHits", "i")
          self.BookBranch(tree, "Reco" + photon + "Module", "i")
          self.BookBranch(tree, "Reco" + photon + "FirstLayer", "i")
          self.BookBranch(tree, "Reco" + photon + "LastLayer", "i")
          self.BookBranch(tree, "Reco" + photon + "FirstXBar", "i")
          self.BookBranch(tree, "Reco" + photon + "LastXBar", "i")
          self.BookBranch(tree, "Reco" + photon + "FirstYBar", "i")
          self.BookBranch(tree, "Reco" + photon + "LastYBar", "i")
          self.BookBranch(tree, "Reco" + photon + "BarsToEdge", "i")
          self.BookBranch(tree, "Reco" + photon + "DeltaLayer", "i")
          self.BookBranch(tree, "Reco" + photon + "NHitLayer", "i")
          self.BookBranch(tree, "Reco" + photon + "MichelTags", "i")
          self.BookBranch(tree, "Reco" + photon + "ConeAngle", "f")
          self.BookBranch(tree, "Reco" + photon + "AMR", "f")
          self.BookBranch(tree, "Reco" + photon + "Asymmetry", "f")
          self.BookBranch(tree, "Reco" + photon + "MeanPosition", "f")
          self.BookBranch(tree, "Reco" + photon + "QSkew", "f")
          self.BookBranch(tree, "Reco" + photon + "ShowerWidth", "f")
          self.BookBranch(tree, "Reco" + photon + "EMLikelihood", "f")
          self.BookBranch(tree, "Reco" + photon + "BunchSigma", "f")
          self.BookBranch(tree, "Reco" + photon + "MatchingLikelihood", "f")
          self.BookBranch(tree, "Reco" + photon + "PointingX", "f")
          self.BookBranch(tree, "Reco" + photon + "PointingY", "f")
          self.BookBranch(tree, "Reco" + photon + "PointingZ", "f")
          self.BookBranch(tree, "Reco" + photon + "XDistanceFromFGD1", "f")
          self.BookBranch(tree, "Reco" + photon + "YDistanceFromFGD1", "f")
          self.BookBranch(tree, "Reco" + photon + "XDistanceFromFGD2", "f")
          self.BookBranch(tree, "Reco" + photon + "YDistanceFromFGD2", "f")
          self.BookBranch(tree, "Reco" + photon + "ThrustDotPCA", "f")

        for photon in ["Earlier", "Later"]:
          self.BookBranch(tree, "Reco" + photon + "Energy", "f")
          self.BookBranch(tree, "Reco" + photon + "PosX", "f")
          self.BookBranch(tree, "Reco" + photon + "PosY", "f")
          self.BookBranch(tree, "Reco" + photon + "PosZ", "f")
          self.BookBranch(tree, "Reco" + photon + "Time", "f")
          self.BookBranch(tree, "Reco" + photon + "DirectionX", "f")
          self.BookBranch(tree, "Reco" + photon + "DirectionY", "f")
          self.BookBranch(tree, "Reco" + photon + "DirectionZ", "f")
          self.BookBranch(tree, "Reco" + photon + "IncidenceAngle", "f")
          self.BookBranch(tree, "Reco" + photon + "Pid", "f")
          self.BookBranch(tree, "Reco" + photon + "Had", "f")
          self.BookBranch(tree, "Reco" + photon + "NumHits", "i")
          self.BookBranch(tree, "Reco" + photon + "FirstLayer", "i")
          self.BookBranch(tree, "Reco" + photon + "LastLayer", "i")
          self.BookBranch(tree, "Reco" + photon + "FirstXBar", "i")
          self.BookBranch(tree, "Reco" + photon + "LastXBar", "i")
          self.BookBranch(tree, "Reco" + photon + "FirstYBar", "i")
          self.BookBranch(tree, "Reco" + photon + "LastYBar", "i")
          self.BookBranch(tree, "Reco" + photon + "BarsToEdge", "i")
          self.BookBranch(tree, "Reco" + photon + "DeltaLayer", "i")
          self.BookBranch(tree, "Reco" + photon + "NHitLayer", "i")
          self.BookBranch(tree, "Reco" + photon + "MichelTags", "i")
          self.BookBranch(tree, "Reco" + photon + "ConeAngle", "f")
          self.BookBranch(tree, "Reco" + photon + "AMR", "f")
          self.BookBranch(tree, "Reco" + photon + "Asymmetry", "f")
          self.BookBranch(tree, "Reco" + photon + "MeanPosition", "f")
          self.BookBranch(tree, "Reco" + photon + "QSkew", "f")
          self.BookBranch(tree, "Reco" + photon + "ShowerWidth", "f")
          self.BookBranch(tree, "Reco" + photon + "EMLikelihood", "f")
          self.BookBranch(tree, "Reco" + photon + "BunchSigma", "f")
          self.BookBranch(tree, "Reco" + photon + "MatchingLikelihood", "f")
          self.BookBranch(tree, "Reco" + photon + "PointingX", "f")
          self.BookBranch(tree, "Reco" + photon + "PointingY", "f")
          self.BookBranch(tree, "Reco" + photon + "PointingZ", "f")
          self.BookBranch(tree, "Reco" + photon + "ThrustDotPCA", "f")



        self.BookBranch(tree, "ConstrainedVertexX", "f")
        self.BookBranch(tree, "ConstrainedVertexY", "f")
        self.BookBranch(tree, "ConstrainedVertexZ", "f")
        self.BookBranch(tree, "ConstrainedMass", "f")
        self.BookBranch(tree, "ConstrainedP", "f")
        self.BookBranch(tree, "ConstrainedDir", "f")
        self.BookBranch(tree, "ConstrainedDeltaDist", "f")

        self.BookBranch(tree, "SkewVertexX", "f")
        self.BookBranch(tree, "SkewVertexY", "f")
        self.BookBranch(tree, "SkewVertexZ", "f")
        self.BookBranch(tree, "SkewMass", "f")
        self.BookBranch(tree, "SkewP", "f")
        self.BookBranch(tree, "SkewDistance", "f")



        # Global Reco Quantites

        self.BookBranch(tree, "TrackerNumTracks", "i")
        self.BookBranch(tree, "TrackerNumVertices", "i")
        self.BookBranch(tree, "NumNonTrackerVertices", "i")
        self.BookBranch(tree, "TrackerNumIsolatedDsECalClusters", "i")
        self.BookBranch(tree, "TrackerNumIsolatedBrlECalClusters", "i")
        self.BookBranch(tree, "TrackerNumTPC1", "i")
        self.BookBranch(tree, "TrackerNumTPC2", "i")
        self.BookBranch(tree, "TrackerNumTPC3", "i")
        self.BookBranch(tree, "TrackerNumFGD1", "i")
        self.BookBranch(tree, "TrackerNumFGD2", "i")
        self.BookBranch(tree, "TrackerNumP0Ds", "i")
        self.BookBranch(tree, "TrackerNumSMRD", "i")
        #self.BookBranch(tree, "TrackerIStart", "i")
        #self.BookBranch(tree, "TrackerIEnd", "i")
        self.BookBranch(tree, "TrackerMass", "f")
        self.BookBranch(tree, "TrackerP", "f")

        self.BookBranch(tree, "GlobalVertexX", "f")
        self.BookBranch(tree, "GlobalVertexY", "f")
        self.BookBranch(tree, "GlobalVertexZ", "f")
        self.BookBranch(tree, "GlobalMass", "f")
        self.BookBranch(tree, "GlobalP", "f")
        self.BookBranch(tree, "GlobalDir", "f")

        # Highest Global Reco Track Properties

        self.BookBranch(tree, "HighestMomTrackMomentum", "f")
        self.BookBranch(tree, "HighestMomTrackCharge", "i")
        self.BookBranch(tree, "HighestMomTrackPosX", "f")
        self.BookBranch(tree, "HighestMomTrackPosY", "f")
        self.BookBranch(tree, "HighestMomTrackPosZ", "f")
        self.BookBranch(tree, "HighestMomTrackTime", "f")
        self.BookBranch(tree, "HighestMomTrackBunchSigma", "f")
        self.BookBranch(tree, "HighestMomTrackHasE", "i")
        self.BookBranch(tree, "HighestMomTrackPidE", "f")
        self.BookBranch(tree, "HighestMomTrackHadE", "f")
        self.BookBranch(tree, "HighestMomTrackHasS", "i")

        for tpc in ["1", "2", "3"]:
          self.BookBranch(tree, "HighestMomTrackHas" + tpc, "i")
          self.BookBranch(tree, "HighestMomTrackBest" + tpc, "i")
          for particle in ["Muon", "Electron", "Proton", "Pion", "Kaon"]:
            self.BookBranch(tree, "HighestMomTrackPull" + particle + tpc, "f")

        for fgd in ["1", "2"]:
          self.BookBranch(tree, "HighestMomTrackHasFGD" + fgd, "i")
          self.BookBranch(tree, "HighestMomTrackBestFGD" + fgd, "i")
          for particle in ["Muon", "Proton", "Pion", "NotSet"]:
            self.BookBranch(tree, "HighestMomTrackFGDPull" + particle + fgd, "f")
          self.BookBranch(tree, "HighestMomTrackFGDNHits" + fgd, "i")

        # FGD-only quantities

        self.BookBranch(tree, "FGDNumTracks", "i");
        self.BookBranch(tree, "FGDTrackX", "f");
        self.BookBranch(tree, "FGDTrackY", "f");
        self.BookBranch(tree, "FGDTrackZ", "f");
        self.BookBranch(tree, "FGDNum2DXTracks", "i");
        self.BookBranch(tree, "FGD2DXTrackX", "f");
        self.BookBranch(tree, "FGD2DXTrackZ", "f");
        self.BookBranch(tree, "FGDNum2DYTracks", "i");
        self.BookBranch(tree, "FGD2DYTrackY", "f");
        self.BookBranch(tree, "FGD2DYTrackZ", "f");

        self.BookBranch(tree, "FGD1Unused", "i")
        self.BookBranch(tree, "FGD1UnusedMinX", "f")
        self.BookBranch(tree, "FGD1UnusedMinY", "f")
        self.BookBranch(tree, "FGD1UnusedMinZ", "f")
        self.BookBranch(tree, "FGD2Unused", "i")
        self.BookBranch(tree, "FGD2UnusedMinX", "f")
        self.BookBranch(tree, "FGD2UnusedMinY", "f")
        self.BookBranch(tree, "FGD2UnusedMinZ", "f")

        self.BookBranch(tree, "FGDMass", "f")
        self.BookBranch(tree, "FGDP", "f")
        self.BookBranch(tree, "FGDDir", "f")
        self.BookBranch(tree, "FGDVertexX", "f")
        self.BookBranch(tree, "FGDVertexY", "f")
        self.BookBranch(tree, "FGDVertexZ", "f")

        # True Quantities

        self.BookBranch(tree, "TrueInteractionUID", "i")
        self.BookBranch(tree, "TrueNInteractions", "i")
        self.BookBranch(tree, "TruePi0Code", "i")
        self.BookBranch(tree, "TrueCurrentType", "i")
        self.BookBranch(tree, "TrueNPi0", "i")
        self.BookBranch(tree, "TrueNPions", "i")
        self.BookBranch(tree, "TrueNPi0PhotECal", "i")
        self.BookBranch(tree, "TrueVertexX", "f")
        self.BookBranch(tree, "TrueVertexY", "f")
        self.BookBranch(tree, "TrueVertexZ", "f")
        self.BookBranch(tree, "TrueVertexT", "f")
        self.BookBranch(tree, "TruePi0Mom", "f")
        self.BookBranch(tree, "TruePi0ID", "i")
        self.BookBranch(tree, "TrueLowEnergy", "f")
        self.BookBranch(tree, "TrueHighEnergy", "f")
        self.BookBranch(tree, "TrueNumOverlaps", "i")

        # True photon (cluster) Quantites

        for photon in ["High", "Low"]:
          self.BookBranch(tree, "True" + photon + "MainContributorWeight", "f")
          self.BookBranch(tree, "True" + photon + "Pi0PhotonWeight", "f")
          self.BookBranch(tree, "True" + photon + "ClusterEnergy", "f")
          self.BookBranch(tree, "True" + photon + "MainContributorPDGId", "i")
          self.BookBranch(tree, "True" + photon + "MainContributorTrajId", "i")
          self.BookBranch(tree, "True" + photon + "MainContributorParentPDGId", "i")
          self.BookBranch(tree, "True" + photon + "MainContributorParentTrajId", "i")
          self.BookBranch(tree, "True" + photon + "MainContributorParentPrimary", "i")
          self.BookBranch(tree, "True" + photon + "NumContributors", "f")
          self.BookBranch(tree, "True" + photon + "NumPhotonTrajectories", "f")
          self.BookBranch(tree, "True" + photon + "TrajectoryID", "f")
          self.BookBranch(tree, "True" + photon + "NumInteractions", "i")
          self.BookBranch(tree, "True" + photon + "NumPhotonContributors", "f")
          self.BookBranch(tree, "True" + photon + "PhotonWeight", "f")
          self.BookBranch(tree, "True" + photon + "PosX", "f")
          self.BookBranch(tree, "True" + photon + "PosY", "f")
          self.BookBranch(tree, "True" + photon + "PosZ", "f")
          self.BookBranch(tree, "True" + photon + "Time", "f")
          self.BookBranch(tree, "True" + photon + "IncidenceAngle", "f")
          self.BookBranch(tree, "True" + photon + "OverlapFromOther", "f")
          self.BookBranch(tree, "True" + photon + "InteractionUID", "i")
          self.BookBranch(tree, "True" + photon + "InteractionCCNC", "i")
          self.BookBranch(tree, "True" + photon + "InteractionPosX", "f")
          self.BookBranch(tree, "True" + photon + "InteractionPosY", "f")
          self.BookBranch(tree, "True" + photon + "InteractionPosZ", "f")
          self.BookBranch(tree, "True" + photon + "InteractionTime", "f")
          self.BookBranch(tree, "True" + photon + "InteractionDetector", "i")
          self.BookBranch(tree, "True" + photon + "InteractionNumLeptons", "i")
          self.BookBranch(tree, "True" + photon + "InteractionNumPi0s", "i")
          self.BookBranch(tree, "True" + photon + "InteractionNumPions", "i")
          self.BookBranch(tree, "True" + photon + "InteractionNumProtons", "i")
          self.BookBranch(tree, "True" + photon + "InteractionNumNeutrons", "i")
          self.BookBranch(tree, "True" + photon + "InteractionNumPhotons", "i")
          self.BookBranch(tree, "True" + photon + "InteractionNumOther", "i")
          self.BookBranch(tree, "True" + photon + "InteractionPi0Mom", "f")
          self.BookBranch(tree, "True" + photon + "InteractionPi0OpeningAngle", "f")
          self.BookBranch(tree, "True" + photon + "InteractionPi0DirZ", "f")

        self.BookBranch(tree, "TrueTrackerMass", "f")
        self.BookBranch(tree, "TrueTrackerP", "f")

        self.BookBranch(tree, "TrueGlobalMass", "f")
        self.BookBranch(tree, "TrueGlobalP", "f")

        self.BookBranch(tree, "TrueFGDMass", "f")
        self.BookBranch(tree, "TrueFGDP", "f")

        self.BookBranch(tree, "TruePhotonsSignalType", "i")

        # True-Reco comparison

        self.BookBranch(tree, "CompareDeltaTime", "f")
        self.BookBranch(tree, "CompareDeltaPos", "f")
        self.BookBranch(tree, "CompareTrackerDeltaPos", "f")
        self.BookBranch(tree, "CompareGlobalDeltaPos", "f")
        self.BookBranch(tree, "CompareFGDDeltaPos", "f")

        ######################
        #                    #
        #    Tracker Tree    #
        #                    #
        ######################
        '''
        tree = self.BookTree("TrackerTree", "Properties for each global track")

        self.BookBranch(tree, "pullE", "f", 3)
        self.BookBranch(tree, "pullMu", "f", 3)
        self.BookBranch(tree, "pullP", "f", 3)
        self.BookBranch(tree, "pullPi", "f", 3)
        self.BookBranch(tree, "pullK", "f", 3)
        self.BookBranch(tree, "ccorr", "f", 3)
        self.BookBranch(tree, "globmom", "f", 3)
        self.BookBranch(tree, "globcharge", "f", 3)
        self.BookBranch(tree, "ntpcs", "i")
        self.BookBranch(tree, "det", "i", 3)
        self.BookBranch(tree, "globpos", "f", 3)
        self.BookBranch(tree, "globtime", "f")
        self.BookBranch(tree, "ecalInfo", "i")
        self.BookBranch(tree, "fgdInfo", "i")
        self.BookBranch(tree, "ecalE", "f")
        self.BookBranch(tree, "TrackerI", "i")
        self.BookBranch(tree, "TrackX", "f")
        self.BookBranch(tree, "TrackY", "f")
        self.BookBranch(tree, "TrackZ", "f")
        self.BookBranch(tree, "DeltaT", "f")
        '''
        ###########################
        #                         #
        #    Preselection Tree    #
        #                         #
        ###########################

        tree = self.BookTree("PreselectionTree", "Preselection properties")

        self.PreselectionTreeNStandaloneClus          = array("i",[0])
        self.PreselectionTreeNTrackerMatchedClus      = array("i",[0])
        self.PreselectionTreeNTrackerStandaloneTracks = array("i",[0])
        self.PreselectionTreeNOtherMatchedClus        = array("i",[0])
        self.PreselectionTreeNTrackerOtherTracks      = array("i",[0])
        self.PreselectionTreeNOthers                  = array("i",[0])
        self.PreselectionTreeNCand                    = array("i",[0])

        ###########################
        #                         #
        #    Minimum Bias Tree    #
        #                         #
        ###########################

        tree = self.BookTree("MinBiasTree", "Minimum bias properties, before preselection")
        
        self.BookBranch(tree, "Time", "f")
        self.BookBranch(tree, "T0", "f")

        #################
        #               #
        #    MC Tree    #
        #               #
        #################

        tree = self.BookTree("MCTree", "Tree of MC information")

        self.BookBranch(tree, "IsUsed", "i")
        self.BookBranch(tree, "UID", "i")
        self.BookBranch(tree, "PosX", "f")
        self.BookBranch(tree, "PosY", "f")
        self.BookBranch(tree, "PosZ", "f")
        self.BookBranch(tree, "Time", "f")
        self.BookBranch(tree, "Detector", "i")
        self.BookBranch(tree, "NumLeptons", "i")
        self.BookBranch(tree, "NumPi0s", "i")
        self.BookBranch(tree, "NumPions", "i")
        self.BookBranch(tree, "NumProtons", "i")
        self.BookBranch(tree, "NumNeutrons", "i")
        self.BookBranch(tree, "NumPhotons", "i")
        self.BookBranch(tree, "NumOther", "i")
        self.BookBranch(tree, "Pi0Mom", "f")
        self.BookBranch(tree, "Pi0OpeningAngle", "f")
        self.BookBranch(tree, "Pi0DirZ", "f")
        self.BookBranch(tree, "NumPi0DecayGammas", "i")

        for photon in ["High", "Low"]:
          self.BookBranch(tree, photon + "PhotonFinalDet", "i")
          self.BookBranch(tree, photon + "Module", "i")
          self.BookBranch(tree, photon + "PhotonEnergy", "f")
          self.BookBranch(tree, photon + "PosX", "f")
          self.BookBranch(tree, photon + "PosY", "f")
          self.BookBranch(tree, photon + "PosZ", "f")
          self.BookBranch(tree, photon + "Time", "f")
          self.BookBranch(tree, photon + "IncidenceAngle", "f")
        

        self.potTree = ROOT.TTree("POT","POT")
        self.potSum = array("f",[0.])
        self.potTree.Branch("POT",self.potSum,"POT/F")
        self.listOfHistosAndOtherObjects.append(self.potTree)

        tree = self.BookTree("DQTree","Data Quality Tree")

        self.BookBranch(tree, "EventId", "i")
        self.BookBranch(tree, "ND280", "i")
        self.BookBranch(tree, "TPC1", "i")
        self.BookBranch(tree, "TPC2", "i")
        self.BookBranch(tree, "TPC3", "i")
        self.BookBranch(tree, "FGD1", "i")
        self.BookBranch(tree, "FGD2", "i")
        self.BookBranch(tree, "DsECal", "i")
        self.BookBranch(tree, "BrlECal", "i")
        self.BookBranch(tree, "P0DECal", "i")
        self.BookBranch(tree, "P0D", "i")
        self.BookBranch(tree, "SMRD", "i")
        self.BookBranch(tree, "Magnet", "i")
        self.BookBranch(tree, "TPCAll", "i")
        self.BookBranch(tree, "FGDAll", "i")
        self.BookBranch(tree, "ECalAll", "i")


        # Declare cut counters, which are accessed using their names
        self.RegisterCutCounter("True NCPi0 Events in FGD1")
        self.RegisterCutCounter("True NCPi0 Events in FGD2")
        self.RegisterCutCounter("True NCPi0 Events in TPC1")
        self.RegisterCutCounter("True NCPi0 Events in TPC2")
        self.RegisterCutCounter("True NCPi0 Events in TPC3")
        
    def FindFailedGlobalTime(self, pid, pidid, det):
      for i in xrange(self.Pi0.NFailedGlobalObject):
        o = self.Pi0.FailedGlobalObject[i]
        if o.PidId == pidid and o.Detector == det and o.Subdetector == pid.Detector:
          return o.Time
      return None
        
    def FindAdjustedECalTime(self, pid, pidid, run):
      #DsECal
      for i in xrange(pid.NECALs):
        d = pid.ECAL[i]
        if d.Detector == 9:
          t = d.FrontPosition.T()
          if d.FrontPosition.Vect().Mag() > 0.:
            return t
          t = self.FindFailedGlobalTime(d, pidid, 7)
          if t != None:
            return t

      #TECal
      for i in xrange(pid.NECALs):
        d = pid.ECAL[i]
        if d.Detector > 4 and d.Detector < 9:
          t = d.FrontPosition.T()
          if d.FrontPosition.Vect().Mag() > 0.:
            if d.Detector == 5:
              t -= DeltaLECAL[run]
            elif d.Detector == 6:
              t -= DeltaRECAL[run]
            elif d.Detector == 7:
              t -= DeltaTECAL[run]
            elif d.Detector == 8:
              t -= DeltaBECAL[run]
            return t
          t = self.FindFailedGlobalTime(d, pidid, 9)
          if t != None:
            if d.Detector == 5:
              t -= DeltaLECAL[run]
            elif d.Detector == 6:
              t -= DeltaRECAL[run]
            elif d.Detector == 7:
              t -= DeltaTECAL[run]
            elif d.Detector == 8:
              t -= DeltaBECAL[run]
            return t

      #PECal
      for i in xrange(pid.NECALs):
        d = pid.ECAL[i]
        if d.Detector < 5:
          t = d.FrontPosition.T()
          if d.FrontPosition.Vect().Mag() > 0.:
            t -= DeltaPECAL[run]
            return t
          t = self.FindFailedGlobalTime(d, pidid, 9)
          if t != None:
            t -= DeltaPECAL[run]
            return t

      #P0D
      for i in xrange(pid.NP0Ds):
        d = pid.P0D[i]
        t = d.FrontPosition.T()
        if d.FrontPosition.Vect().Mag() > 0.:
          t -= DeltaP0D[run]
          return t
        t = self.FindFailedGlobalTime(d, pidid, 6)
        if t != None:
          t -= DeltaP0D[run]
          return t

      #SMRD
      for i in xrange(pid.NSMRDs):
        d = pid.SMRD[i]
        t = d.FrontPosition.T()
        if d.FrontPosition.Vect().Mag() > 0.:
          t -= DeltaSMRD[run]
          return t
        t = self.FindFailedGlobalTime(d, pidid, 8)
        if t != None:
          t -= DeltaSMRD[run]
          return t

      #FGD
      for i in xrange(pid.NFGDs):
        d = pid.FGD[i]
        t = d.FrontPosition.T()
        if d.FrontPosition.Vect().Mag() > 0.:
          if d.Detector == 1:
            t -= DeltaFGD1[run]
          elif d.Detector == 2:
            t -= DeltaFGD2[run]
          return t
        t = self.FindFailedGlobalTime(d, pidid, d.Detector + 3)
        if t != None:
          if d.Detector == 1:
            t -= DeltaFGD1[run]
          elif d.Detector == 2:
            t -= DeltaFGD2[run]
          return t

      #TPC
      for i in xrange(pid.NTPCs):
        d = pid.TPC[i]
        t = d.FrontPosition.T()
        if d.FrontPosition.Vect().Mag() > 0.:
          if d.Detector == 1:
            t -= DeltaTPC1[run]
          elif d.Detector == 2:
            t -= DeltaTPC2[run]
          elif d.Detector == 3:
            t -= DeltaTPC3[run]
          return t
        t = self.FindFailedGlobalTime(d, pidid, d.Detector)
        if t != None:
          if d.Detector == 1:
            t -= DeltaTPC1[run]
          elif d.Detector == 2:
            t -= DeltaTPC2[run]
          elif d.Detector == 3:
            t -= DeltaTPC3[run]
          return t
        
      if pid.Detectors == 0:
        t = pid.FrontPosition.T()
        if pid.FrontPosition.Vect().Mag() > 0.:
          return t
      
      print "Failed to find failed global time for F:",self.basicHeader.GetCurrentFile().GetName(),"E:",self.basicHeader.EventID,"PID:",pidid,"Dets:",pid.Detectors
      return 0.;
    
        
    def VertexOrdering(self, pos=None):
      if pos == None:
        return 10
      if pos == 0:
        return [self.truthVertices.VtxFGD1, self.truthVertices.NVtxFGD1, "FGD1"]
      if pos == 1:
        return [self.truthVertices.VtxFGD2, self.truthVertices.NVtxFGD2, "FGD2"]
      if pos == 2:
        return [self.truthVertices.VtxTPC, self.truthVertices.NVtxTPC, "TPC"]
      if pos == 3:
        return [self.truthVertices.VtxDsECal, self.truthVertices.NVtxDsECal, "DsECal"]
      if pos == 4:
        return [self.truthVertices.VtxBrlECal, self.truthVertices.NVtxBrlECal, "BrlECal"]
      if pos == 5:
        return [self.truthVertices.VtxP0DECal, self.truthVertices.NVtxP0DECal, "P0DECal"]
      if pos == 6:
        return [self.truthVertices.VtxP0D, self.truthVertices.NVtxP0D, "P0D"]
      if pos == 7:
        return [self.truthVertices.VtxMRD, self.truthVertices.NVtxMRD, "SMRD"]
      if pos == 8:
        return [self.truthVertices.VtxOther, self.truthVertices.NVtxOther, "Other"]
      if pos == 9:
        return [self.truthVertices.VtxRestOfOffAxis, self.truthVertices.NVtxRestOfOffAxis, "ROOA"]

    def TrajectoryOrdering(self, pos=None):
      if pos==None:
        return 7
      if pos == 0:
        return [self.truthTrajectories.TrajsLepton, self.truthTrajectories.NTrajsLepton, "Lepton"]
      if pos == 1:
        return [self.truthTrajectories.TrajsBaryon, self.truthTrajectories.NTrajsBaryon, "Baryon"]
      if pos == 2:
        return [self.truthTrajectories.TrajsMeson, self.truthTrajectories.NTrajsMeson, "Meson"]
      if pos == 3:
        return [self.truthTrajectories.TrajsPhoton, self.truthTrajectories.NTrajsPhoton, "Photon"]
      if pos == 4:
        return [self.truthTrajectories.TrajsOtherCharged, self.truthTrajectories.NTrajsOtherCharged, "OtherCharged"]
      if pos == 5:
        return [self.truthTrajectories.TrajsOtherNeutral, self.truthTrajectories.NTrajsOtherNeutral, "OtherNeutral"]
      if pos == 6:
        return [self.truthTrajectories.TrajsOther, self.truthTrajectories.NTrajsOther, "Other"]

    def ParticleNaming(self, pdg):
      if pdg == -3:
        return [-3, "noise"]
      if pdg == -2:
        return [-2, "magnet"]
      if pdg == -1:
        return [-1, "nu"]
      if pdg == 11:
        return [1, "e"]
      if pdg == 13:
        return [2, "mu-"]
      if pdg == 15:
        return [3, "tau-"]
      if pdg == -11:
        return [4, "e+"]
      if pdg == -13:
        return [5, "mu+"]
      if pdg == -15:
        return [6, "tau+"]
      if pdg == 22:
        return [7, "gamma"]
      if pdg == 2212:
        return [8, "p"]
      if pdg == 2112:
        return [9, "n"]
      if pdg == 211:
        return [10, "pi+"]
      if pdg == -211:
        return [11, "pi-"]
      if pdg == 111:
        return [12, "pi0"]
      if pdg == 130 or pdg == 310 or pdg == 311 or pdg == -311:
        return [13, "K0"]
      if pdg == 321:
        return [14, "K+"]
      if pdg == -321:
        return [15, "K-"]
      if pdg == -2212:
        return [16, "p-"]
      if pdg == -2112:
        return [17, "~n"]
      if pdg == 3122:
        return [18, "Lam0"]
      if pdg == 3222:
        return [18, "Sig+"]
      if pdg == 221:
        return [19, "Eta"]
      print "Unknown pdg code: ", pdg
      return [0, "other"]


    def GetVertexTrajectories(self, vertex):
      
      listOfTrajectories = []

      for trajId in vertex.LeptonIDs:
        for i in xrange(self.truthTrajectories.NTrajsLepton):
          t = self.truthTrajectories.TrajsLepton[i]
          if t.TrajID == trajId:
            listOfTrajectories.append(t)
      for trajId in vertex.BaryonIDs:
        for i in xrange(self.truthTrajectories.NTrajsBaryon):
          t = self.truthTrajectories.TrajsBaryon[i]
          if t.TrajID == trajId:
            listOfTrajectories.append(t)
      for trajId in vertex.MesonIDs:
        for i in xrange(self.truthTrajectories.NTrajsMeson):
          t = self.truthTrajectories.TrajsMeson[i]
          if t.TrajID == trajId:
            listOfTrajectories.append(t)
      for trajId in vertex.PhotonIDs:
        for i in xrange(self.truthTrajectories.NTrajsPhoton):
          t = self.truthTrajectories.TrajsPhoton[i]
          if t.TrajID == trajId:
            listOfTrajectories.append(t)
      for trajId in vertex.OtherChargedIDs:
        for i in xrange(self.truthTrajectories.NTrajsOtherCharged):
          t = self.truthTrajectories.TrajsOtherCharged[i]
          if t.TrajID == trajId:
            listOfTrajectories.append(t)
      for trajId in vertex.OtherNeutralIDs:
        for i in xrange(self.truthTrajectories.NTrajsOtherNeutral):
          t = self.truthTrajectories.TrajsOtherNeutral[i]
          if t.TrajID == trajId:
            listOfTrajectories.append(t)
      for trajId in vertex.OtherIDs:
        for i in xrange(self.truthTrajectories.NTrajsOther):
          t = self.truthTrajectories.TrajsOther[i]
          if t.TrajID == trajId:
            listOfTrajectories.append(t)

      return listOfTrajectories

    def GetWeights(self, tracks, module, pos):
        Pi0DecayPhotonWeight = 0.
        NumPi0DecayPhotons = 0
        Pi0DecayPhotonEnergy = 0.
        Pi0DecayPhotonTrajId = 0
        MaxEDep = 0.
        SumEDep = 0.
        MaxTrajPDG = 0
        MaxTrajID = -1
        MaxTrajPi0Parent = 0
        InteractionUId = -1
        MaxInternalEDep = 0.
        InternalUId = -1
        NumContrib = 0
        NumInternal = 0
        listOfInteractions = []

        ParentTrajID = -1
        ParentTrajPDG = -1
        ParentTrajPrimary = -1

        recoModule = 0
        recoDet = 0
        if module == DsECalModule:
          recoDet = 3
          recoModule = self.WhichModule(3, pos)
        else:
          recoDet = 4
          recoModule = self.WhichModule(4, pos)
            
        for c in xrange(tracks.size()):
          clarrid = tracks[c]
          trajid = self.Pi0.TrueTraj[clarrid].Id
          uid = self.FindInteractionUID(trajid)
          v = self.GetVertex(uid)
          if uid not in self.UsedInt:
            self.UsedInt[uid] = True
          det = self.WhichDetector(v)
          mod = self.WhichModule(det, v.Vertex)
          edep = self.Pi0.TrueTraj[clarrid].EDep
          SumEDep += edep
          if v == None or det != recoDet or mod != recoModule : #or self.FindPDG(trajid) != 0:
            NumContrib += 1
            if self.FindPDG(trajid) == 22 and self.FindPi0Parent(trajid) != 0:
              Pi0DecayPhotonWeight += edep
              NumPi0DecayPhotons += 1
              Pi0DecayPhotonEnergy = self.FindE(trajid)
              Pi0DecayPhotonTrajId = trajid
            if edep > MaxEDep:
              MaxEDep = edep
              MaxTrajPDG = self.FindPDG(trajid)
              MaxTrajID = trajid
              ParentTrajID = self.FindParentTrajID(trajid)
              if ParentTrajID == 0:
                ParentTrajPDG = -1
              else:
                ParentTrajPDG = self.FindPDG(ParentTrajID)
              if self.FindParentTrajID(ParentTrajID) == 0:
                ParentTrajPrimary = 1
              elif self.FindParentTrajID(ParentTrajID) > 0:
                ParentTrajPrimary = 0
              else:
                ParentTrajPrimary = -1
                ParentTrajPDG = -2
              InteractionUId = uid
          else:
            found = False
            for i in listOfInteractions:
              if i[0] == uid:
                i[1] += edep
                found = True
            if not found:
              listOfInteractions.append([uid, edep])

        for i in listOfInteractions:
          NumContrib += 1
          if i[1] > MaxEDep:
            MaxEDep = i[1]
            MaxTrajPDG = -1
            MaxTrajID = -1
            MaxTrajPi0Parent = 0
            InteractionUId = i[0]


        if MaxTrajPDG == 0:
          if tracks.size() == 0:
            MaxTrajPDG = -3
          elif ParentTrajID == -1:
            MaxTrajPDG = -2


          
        MaxWeight = MaxEDep / (SumEDep + 1e-100)
        Pi0DecayPhotonWeight /= (SumEDep + 1e-100)
          
        return [Pi0DecayPhotonWeight, NumPi0DecayPhotons, MaxTrajPDG, MaxWeight, Pi0DecayPhotonEnergy, Pi0DecayPhotonTrajId, NumContrib, InteractionUId, MaxTrajID, ParentTrajPDG, ParentTrajID, ParentTrajPrimary]

    def GetOverlaps(self, tracks1, tracks2):
      listOfTracks = []
      listOfInteractions = []
      listOfFoundInteractions = []
      SumEDep = 0.
      OverlapEDep = 0.
      NumOverlap = 0
      for c in xrange(tracks1.size()):
        clarrid = tracks1[c]
        trajid = self.Pi0.TrueTraj[clarrid].Id
        edep = self.Pi0.TrueTraj[clarrid].EDep
        uid = self.FindInteractionUID(trajid)
        v = self.GetVertex(uid)
        det = self.WhichDetector(v)
        if v == None or det < 3 or det > 5:
          listOfTracks.append([trajid, edep])
        else:
          intuid = uid
          found = False
          for i in listOfInteractions:
            if i[0] == intuid:
              i[1] += edep
              found = True
          if not found:
            listOfInteractions.append([intuid, edep])
        SumEDep += edep
      for c2 in xrange(tracks2.size()):
        clarrid = tracks2[c2]
        trajid = self.Pi0.TrueTraj[clarrid].Id
        for t in listOfTracks:
          if trajid == t[0]:
            NumOverlap += 1
            OverlapEDep += t[1]
        for i in listOfInteractions:
          t = self.GetTrajectory(trajid)
          if not (t == None or t < 3 or t > 5):
            intuid = self.FindInteractionUID(t.TrajID)
            if intuid == i[0] and intuid not in listOfFoundInteractions:
              NumOverlap += 1
              OverlapEDep += i[1]
              listOfFoundInteractions.append(intuid)

      OverlapWeight = OverlapEDep / (SumEDep + 1e-100)

      return [NumOverlap, OverlapWeight]


    def FindInteractionUID(self, trajId):
      if trajId == -1:
        return -1
      vuid = -1
      uid = -1
      for i in xrange(self.VertexOrdering()):
        retv = self.VertexOrdering(i)
        for j in xrange(retv[1]):
          vuid += 1
          v = retv[0][j]
          for t in self.GetVertexTrajectories(v):
            if t.TrajID == trajId:
              uid = vuid + self.LastNumInteractions

      if uid == -1:
        parentId = self.FindParentTrajID(trajId)
        return self.FindInteractionUID(parentId)

      return uid

    def PhotonMCInfo(self, trajId, mod):

      pos = ROOT.TLorentzVector()
      mom = ROOT.TLorentzVector()
      incang = -2.
      ene = -1.
      
      for i in xrange(self.truthTrajectories.NTrajsPhoton):
        t = self.truthTrajectories.TrajsPhoton[i]
        if t.TrajID == trajId:
          ene = t.InitMom.E()
          if mod == 1:
            pos = t.EntrancePositions[3]
            mom = t.EntranceMomentum[3]
            mom2 = t.InitMom
            incang = mom.Vect().Z() / (mom.Vect().Mag() + 1e-100)
          else:
            pos = t.EntrancePositions[4]
            mom = t.EntranceMomentum[4]
            if mod == 3 or mod == 6:
              incang = mom.Vect().X() / (mom.Vect().Mag() + 1e-100)
            else:
              incang = mom.Vect().Y() / (mom.Vect().Mag() + 1e-100)

      return [pos.X(), pos.Y(), pos.Z(), pos.T(), incang, ene]




        
    def AreBothPhotonsPurelyFromSamePi0(self, tracksH, tracksL):
        
        ntraj = 0
        nphH = 0
        nphL = 0
        pi0id = 0
        photonHid = 0
        photonLid = 0
        
        esumH = 0.
        ephsumH = 0.
        esumL = 0.
        ephsumL = 0.
        
        for c in xrange(tracksH.size()):
          clarrid = tracksH[c]
          trajid = self.Pi0.TrueTraj[clarrid].Id
          uid = self.FindInteractionUID(trajid)
          v = self.GetVertex(uid)
          det = self.WhichDetector(v)
          ntraj+= 1
          edep = self.Pi0.TrueTraj[clarrid].EDep
          esumH += edep
          if (v == None or det < 3 or det > 5) and self.FindPDG(trajid) == 22:
            pi0id = self.FindPi0Parent(trajid)
            photonHid = trajid
            ephsumH += edep
            nphH += 1
        
        if ntraj != 1 and photonHid > 0:
          pi0id = 0
        
        ntraj = 0
        for c in xrange(tracksL.size()):
          clarrid = tracksL[c]
          trajid = self.Pi0.TrueTraj[clarrid].Id
          ntraj += 1
          esumL += self.Pi0.TrueTraj[clarrid].EDep
          if self.FindPDG(trajid) == 22:
            ephsumL += self.Pi0.TrueTraj[clarrid].EDep
            nphL += 1
            if self.FindPi0Parent(trajid) == pi0id:
              photonLid = trajid
            
            
        if ntraj != 1 and photonLid > 0:
          pi0id = 0
          
        photonHweight = ephsumH / (esumH + 1e-100)
        photonLweight = ephsumL / (esumL + 1e-100)
          
        return [pi0id, photonHid, photonLid, nphH, nphL, photonHweight, photonLweight]
        
    def FindPi0Parent(self, trajid):
    
        #print "Finding parent of "+str(trajid)
    
        for i in xrange(self.truthTrajectories.NTrajsPhoton):
          t = self.truthTrajectories.TrajsPhoton[i]
          if t.TrajID == trajid:
            parentPDG = self.FindPDG(t.ParentID)
            #print "Found parent " + str(parentPDG)
            if parentPDG == 111:
              return t.ParentID
            return 0
              
        return 0

    def FindDaughters(self, trajId):
      listOfDaughters = []
      for i in xrange(self.TrajectoryOrdering()):
        ret = self.TrajectoryOrdering(i)
        for j in xrange(ret[1]):
          t = ret[0][j]
          if t.ParentID == trajId:
            listOfDaughters.append(t.TrajID)
      return listOfDaughters

    def FindParentTrajID(self, trajid):
      for i in xrange(self.TrajectoryOrdering()):
        ret = self.TrajectoryOrdering(i)
        for j in xrange(ret[1]):
          t = ret[0][j]
          if t.TrajID == trajid:
            return t.ParentID
      return -1
        
        
    def FindE(self, pid):
        for i in xrange(self.TrajectoryOrdering()):
          ret = self.TrajectoryOrdering(i)
          for j in xrange(ret[1]):
            t = ret[0][j]
            if t.TrajID == pid:
              return t.InitMom.E()
        return -1.

    def WhichDetector(self, vertex):
      for i in xrange(self.VertexOrdering()):
        ret = self.VertexOrdering(i)
        for j in xrange(ret[1]):
          if vertex == ret[0][j]:
            return i
      return -1

    def WhichModule(self, detector, pos):
      if detector == 3:
        return 1
      if detector == 4:
        if pos.X() > 0.:
          # Left side
          if pos.Y() > 1000:
            return 7
          if pos.Y() < -1000:
            return 5
          return 6
        else:
          # Right side
          if pos.Y() > 1000:
            return 4
          if pos.Y() < -1000:
            return 2
          return 3
      return 0
        
    def FindPDG(self, pid):
        for i in xrange(self.TrajectoryOrdering()):
          ret = self.TrajectoryOrdering(i)
          for j in xrange(ret[1]):
            t = ret[0][j]
            if t.TrajID == pid:
              return t.TrajPDG
        return 0

    def Pi0MassMom(self, vert, high, low, eh, el, dh = ROOT.TVector3(0.,0.,0.), dl = ROOT.TVector3(0.,0.,0.)):
        mom = ROOT.TLorentzVector(0.,0.,0.,0.)
        vert2 = ROOT.TVector3(vert.X(), vert.Y(), vert.Z())
        high2 = ROOT.TVector3(high.X(), high.Y(), high.Z())
        low2 = ROOT.TVector3(low.X(), low.Y(), low.Z())

        ph = ROOT.TVector3(high.X()-vert.X(), high.Y()-vert.Y(), high.Z()-vert.Z())
        pl = ROOT.TVector3(low.X()-vert.X(), low.Y()-vert.Y(), low.Z()-vert.Z())
        if not ph.Mag() > 0.:
          ph = ROOT.TVector3(dh.X(), dh.Y(), dh.Z())
        if not pl.Mag() > 0.:
          pl = ROOT.TVector3(dl.X(), dl.Y(), dl.Z())
        if eh > 0 and el > 0:
          ph.SetMag(eh)
          pl.SetMag(el)
          mom = ROOT.TLorentzVector(ph.X() + pl.X(), ph.Y() + pl.Y(), ph.Z()+pl.Z(), eh + el)
        ph.SetMag(1.)
        pl.SetMag(1.)
        return [mom.M(), mom.Vect(), ph, pl]

    def GetTrajectory(self, trajid):

      for i in xrange(self.TrajectoryOrdering()):
        ret = self.TrajectoryOrdering(i)
        for j in xrange(ret[1]):
          t = ret[0][j]
          if t.TrajID == trajid:
            return t

      print "GetTrajectory: TrajId ", trajid, " could not be found!"
      return None

    def GetVertex(self, vertexuid):
      uid = self.LastNumInteractions
      for i in xrange(self.VertexOrdering()):
        ret = self.VertexOrdering(i)
        for j in xrange(ret[1]):
          if uid == vertexuid:
            return ret[0][j]
          uid += 1

      #print "GetVertex: VertexUID ", vertexuid, " not found!"
      return None

    def PerformMCTasks(self, bunch):

      retparams = self.BunchTimes(bunch)
      EarliestTime = retparams[0] - MCTruthRecoOffset
      LatestTime = retparams[1] - MCTruthRecoOffset
      
      nFGD1 = 0
      nFGD2 = 0
      nTPC1 = 0
      nTPC2 = 0
      nTPC3 = 0
      nOther = 0

      self.Pi0TreeTrueNInteractions[0] = 0 
      self.Pi0TreeTrueCurrentType[0] = 0

      self.Pi0TreeTrueVertexX[0] = 0.
      self.Pi0TreeTrueVertexY[0] = 0.
      self.Pi0TreeTrueVertexZ[0] = 0.
      self.Pi0TreeTrueVertexT[0] = 0.

      self.Pi0TreeTruePi0Mom[0] = -1.

      listOfVtx = []
      uid = -1

      for i in xrange(self.VertexOrdering()):
        ret = self.VertexOrdering(i)
        for j in xrange(ret[1]):
          uid += 1
          v = ret[0][j]
          t = v.Vertex.T()
          if bunch == 0:
            listOfVtx.append(v)
          if t > EarliestTime and t < LatestTime:
            self.Pi0TreeTrueNInteractions[0] += 1
            for p, pdg in zip(v.OtherNeutralMoms, v.OtherNeutralPDGs):
              if pdg == 111:
                self.Pi0TreeTrueInteractionUID[0] = uid + self.LastNumInteractions
                position = v.Vertex
                self.Pi0TreeTrueVertexX[0] = position.X()
                self.Pi0TreeTrueVertexY[0] = position.Y()
                self.Pi0TreeTrueVertexZ[0] = position.Z()
                self.Pi0TreeTrueVertexT[0] = position.T()
                self.Pi0TreeTruePi0Mom[0] = p.Vect().Mag()
                try:
                  if self.generatorUtils.IsNC(v):
                    self.Pi0TreeTrueCurrentType[0] = 1
                  elif self.generatorUtils.IsCC(v):
                    self.Pi0TreeTrueCurrentType[0] = 2
                except:
                  if int(v.ReactionCode) >= 31:
                    self.Pi0TreeTrueCurrentType[0] = 1
                  else:
                    self.Pi0TreeTrueCurrentType[0] = 2
                if ret[2] == "FGD1":
                  if nFGD1 < 9:
                    nFGD1 += 1
                elif ret[2] == "FGD2":
                  if nFGD2 < 9:
                    nFGD2 += 1
                elif ret[2] == "TPC":
                  if position.Z() < 300. and nTPC1 < 9:
                    nTPC1 += 1
                  elif position.Z() > 300. and position.Z() < 1700. and nTPC2 < 9:
                    nTPC2 += 1
                  elif position.Z() > 1700. and nTPC3 < 9:
                    nTPC3 += 1
                else:
                  nOther += 1

      self.Pi0TreeTruePi0Code[0] = nFGD1 + 10*nFGD2 + 100*nTPC1 + 1000*nTPC2 + 10000*nTPC3 + 100000*nOther

      if bunch == 0:
        self.FillMCTree(listOfVtx)

      
      return self.Pi0TreeTruePi0Code[0]

    def ResetMCTree(self):
      self.MCTreeIsUsed[0] = 0
      self.MCTreeUID[0] = uid
      self.MCTreePosX[0] = 0.
      self.MCTreePosY[0] = 0.
      self.MCTreePosZ[0] = 0.
      self.MCTreeTime[0] = 0.
      self.MCTreeDetector[0] = -1
      self.MCTreePi0OpeningAngle[0] = -2.
      self.MCTreePi0DirZ[0] = -2.
      self.MCTreePi0Mom[0] = -1.
      self.MCTreeHighPhotonFinalDet[0] = -1
      self.MCTreeHighModule[0] = -1
      self.MCTreeHighPhotonEnergy[0] = -1.
      self.MCTreeHighPosX[0] = 0.
      self.MCTreeHighPosY[0] = 0.
      self.MCTreeHighPosZ[0] = 0.
      self.MCTreeHighTime[0] = 0.
      self.MCTreeHighIncidenceAngle[0] = -2.
      self.MCTreeHighPhotonFinalDet[0] = -1
      self.MCTreeLowModule[0] = -1
      self.MCTreeLowPhotonEnergy[0] = -1.
      self.MCTreeLowPosX[0] = 0.
      self.MCTreeLowPosY[0] = 0.
      self.MCTreeLowPosZ[0] = 0.
      self.MCTreeLowTime[0] = 0.
      self.MCTreeLowIncidenceAngle[0] = -2.
      self.MCTreePi0OpeningAngle[0] = -2.
      self.MCTreePi0DirZ[0] = -2.
      self.MCTreePi0Mom[0] = -1.
      self.MCTreeNumPi0DecayGammas[0] = -1
      self.MCTreeNumLeptons[0] = 0
      self.MCTreeNumPhotons[0] = 0
      self.MCTreeNumPi0s[0] = 0
      self.MCTreeNumPions[0] = 0
      self.MCTreeNumProtons[0] = 0
      self.MCTreeNumNeutrons[0] = 0
      self.MCTreeNumOther[0] = 0

    def GetMCInteractionInfo(self, uid):
      posx = 0.
      posy = 0.
      posz = 0.
      post = 0.
      det = 0
      ccnc = 0
      nlep = 0
      npi0 = 0
      npi = 0
      np = 0
      nn = 0
      ngam = 0
      noth = 0
      pi0mom = -1.
      pi0ang = 0
      pi0z = 0


      v = self.GetVertex(uid)

      if v != None:
        posx = v.Vertex.X()
        posy = v.Vertex.Y()
        posz = v.Vertex.Z()
        post = v.Vertex.T()
        det = self.WhichDetector(v)
        try:
          if self.generatorUtils.IsNC(v):
            ccnc = 1
          elif self.generatorUtils.IsCC(v):
            ccnc = 2
        except:
          if int(v.ReactionCode) >= 31:
            ccnc = 1
          else:
            ccnc = 2
        trajectories = self.GetVertexTrajectories(v)
        for t in trajectories:
          if t.TrajPDG == 11 or t.TrajPDG == 13 or t.TrajPDG == 15 or t.TrajPDG == -11 or t.TrajPDG == -13 or t.TrajPDG == -15:
            nlep += 1
          elif t.TrajPDG == 22:
            ngam += 1
          elif t.TrajPDG == 111:
            npi0 += 1
            gammas = []
            daughters = self.FindDaughters(t.TrajID)
            for d in daughters:
              if self.FindPDG(d) == 22:
                gammas.append(self.GetTrajectory(d))
            if len(gammas) == 2:
              if gammas[0].InitMom.E() < gammas[1].InitMom.E():
                gammas.reverse()
              # opening angle
              hdir = gammas[0].InitMom.Vect()
              hdir.SetMag(1.)
              ldir = gammas[1].InitMom.Vect()
              ldir.SetMag(1.)
              pi0ang = hdir.Dot(ldir)
            if t.InitMom.Vect().Mag() > 0.:
              pi0z = t.InitMom.Vect().Z() / t.InitMom.Vect().Mag()
            pi0mom = t.InitMom.Vect().Mag()
          elif t.TrajPDG == 211 or t.TrajPDG == -211:
            npi += 1
          elif t.TrajPDG == 2212:
            np += 1
          elif t.TrajPDG == 2112:
            nn += 1
          else:
            noth += 1

      return [posx, posy, posz, post, det, ccnc, nlep, npi0, npi, np, nn, ngam, noth, pi0mom, pi0ang, pi0z]

    def FillMCTree(self, listOfVtx):

      uid = self.LastNumInteractions

      for v in listOfVtx:
        if uid in self.UsedInt:
          self.MCTreeIsUsed[0] = 1
        else:
          self.MCTreeIsUsed[0] = 0
        self.MCTreeUID[0] = uid
        self.MCTreePosX[0] = v.Vertex.X()
        self.MCTreePosY[0] = v.Vertex.Y()
        self.MCTreePosZ[0] = v.Vertex.Z()
        self.MCTreeTime[0] = v.Vertex.T()
        self.MCTreeDetector[0] = self.WhichDetector(v)
        time = v.Vertex.T()
        for b in [-1] + range(1,10):
          retparams = self.BunchTimes(b)
          EarliestTime = retparams[0] - MCTruthRecoOffset
          LatestTime = retparams[1] - MCTruthRecoOffset
          if time > EarliestTime and time < LatestTime:
            self.GlobalBunchN[0] = b

        nlep = 0
        nphot = 0
        npi0 = 0
        npion = 0
        nproton = 0
        nneutron = 0
        nother = 0
        
        self.MCTreePi0OpeningAngle[0] = -2.
        self.MCTreePi0DirZ[0] = -2.
        self.MCTreePi0Mom[0] = -1.

        trajectories = self.GetVertexTrajectories(v)
        gammas = []
        for t in trajectories:
          if t.TrajPDG == 11 or t.TrajPDG == 13 or t.TrajPDG == 15 or t.TrajPDG == -11 or t.TrajPDG == -13 or t.TrajPDG == -15:
            nlep += 1
          elif t.TrajPDG == 22:
            nphot += 1
          elif t.TrajPDG == 111:
            npi0 += 1
            ngamm = 0
            gammas = []
            daughters = self.FindDaughters(t.TrajID)
            for d in daughters:
              if self.FindPDG(d) == 22:
                ngamm += 1
                gammas.append(self.GetTrajectory(d))
            if ngamm == 2:
              if gammas[0].InitMom.E() < gammas[1].InitMom.E():
                gammas.reverse()
              # high
              self.MCTreeHighPhotonFinalDet[0] = gammas[0].FinalDet
              mod = self.WhichModule(gammas[0].FinalDet, gammas[0].FinalPos)
              self.MCTreeHighModule[0] = mod
              ret = self.PhotonMCInfo(gammas[0].TrajID, mod)
              self.MCTreeHighPhotonEnergy[0] = ret[5]
              self.MCTreeHighPosX[0] = ret[0]
              self.MCTreeHighPosY[0] = ret[1]
              self.MCTreeHighPosZ[0] = ret[2]
              self.MCTreeHighTime[0] = ret[3]
              self.MCTreeHighIncidenceAngle[0] = ret[4]
              # low
              self.MCTreeLowPhotonFinalDet[0] = gammas[1].FinalDet
              mod = self.WhichModule(gammas[1].FinalDet, gammas[1].FinalPos)
              self.MCTreeLowModule[0] = mod
              ret = self.PhotonMCInfo(gammas[1].TrajID, 1)
              self.MCTreeLowPhotonEnergy[0] = ret[5]
              self.MCTreeLowPosX[0] = ret[0]
              self.MCTreeLowPosY[0] = ret[1]
              self.MCTreeLowPosZ[0] = ret[2]
              self.MCTreeLowTime[0] = ret[3]
              self.MCTreeLowIncidenceAngle[0] = ret[4]
              # opening angle
              hdir = gammas[0].InitMom.Vect()
              hdir.SetMag(1.)
              ldir = gammas[1].InitMom.Vect()
              ldir.SetMag(1.)
              self.MCTreePi0OpeningAngle[0] = hdir.Dot(ldir)
            if t.InitMom.Vect().Mag() > 0.:
              self.MCTreePi0DirZ[0] = t.InitMom.Vect().Z() / t.InitMom.Vect().Mag()
            self.MCTreePi0Mom[0] = t.InitMom.Vect().Mag()
            self.MCTreeNumPi0DecayGammas[0] = ngamm
          elif t.TrajPDG == 211 or t.TrajPDG == -211:
            npion += 1
          elif t.TrajPDG == 2212:
            nproton += 1
          elif t.TrajPDG == 2112:
            nneutron += 1
          else:
            nother += 1

        self.MCTreeNumLeptons[0] = nlep
        self.MCTreeNumPhotons[0] = nphot
        self.MCTreeNumPi0s[0] = npi0
        self.MCTreeNumPions[0] = npion
        self.MCTreeNumProtons[0] = nproton
        self.MCTreeNumNeutrons[0] = nneutron
        self.MCTreeNumOther[0] = nother

        self.rootTrees["MCTree"].Fill()
        uid += 1

      self.LastNumInteractions = uid
      self.UsedInt.clear()

    def BunchTimes(self, bunch):

      if bunch == 0:
        return [-1e10, 1e10]

      run = self.GlobalBeamRun[0]

      if bunch == -1:
        return [-1e10, FirstECalBunch[run] - 250.]
      if (run > 30 and run < 35) or run == 1:
        if bunch == 9:
          return [FirstECalBunch[run] + 580. * 5 + 250., 1e100]
        elif bunch > 6:
          return [0., -1.]
      if ((run > 35 and run < 39) or run == 2) and bunch > 8:
        return [FirstECalBunch[run] + 580. * 7 + 250., 1e100]

      mid = FirstECalBunch[run] + 580. * (bunch - 1)

      return [mid - 250., mid + 250.]

    def FillMinBiasTree(self):
        
        npids = self.Global.NPIDs
        
        # first look for 2 standalone clusters
        for i in xrange(npids):
          pid = self.Global.PIDs[i]
          
          t1 = self.FindAdjustedECalTime(pid, i, self.GlobalBeamRun[0])
            
          if pid.NECALs > 0 and pid.NTPCs == 0 and pid.NFGDs == 0 and pid.NP0Ds == 0 and pid.NSMRDs == 0:
            isStandaloneClus = True
          else:
            continue

          for j in xrange(i+1, npids):
            pid2 = self.Global.PIDs[j]
            t2 = self.FindAdjustedECalTime(pid2, j, self.GlobalBeamRun[0])
            if pid2.NECALs > 0 and pid2.NTPCs == 0 and pid2.NFGDs == 0 and pid2.NP0Ds == 0 and pid2.NSMRDs == 0:
              isStandaloneClus = True
            else:
              continue

            self.MinBiasTreeTime[0] = abs(t1-t2)
            self.MinBiasTreeT0[0] = min(t1,t2)
            self.rootTrees["MinBiasTree"].Fill()

    def ResetHighestTrackParams(self):

      self.Pi0TreeHighestMomTrackMomentum[0] = -1 
      self.Pi0TreeHighestMomTrackCharge[0] = 0
      self.Pi0TreeHighestMomTrackPosX[0] = 0.
      self.Pi0TreeHighestMomTrackPosY[0] = 0.
      self.Pi0TreeHighestMomTrackPosZ[0] = 0.
      self.Pi0TreeHighestMomTrackTime[0] = 0.
      self.Pi0TreeHighestMomTrackBunchSigma[0] = 0.
      self.Pi0TreeCompareTrackerDeltaPos[0] = -1.
      self.Pi0TreeCompareGlobalDeltaPos[0] = -1.
      self.Pi0TreeCompareFGDDeltaPos[0] = -1.

      self.Pi0TreeHighestMomTrackHas1[0] = 0
      self.Pi0TreeHighestMomTrackHas2[0] = 0
      self.Pi0TreeHighestMomTrackHas3[0] = 0
      self.Pi0TreeHighestMomTrackHasE[0] = 0
      self.Pi0TreeHighestMomTrackPidE[0] = -1.
      self.Pi0TreeHighestMomTrackHadE[0] = -1.
      self.Pi0TreeHighestMomTrackHasS[0] = 0

      self.Pi0TreeHighestMomTrackPullMuon1[0] = 0.
      self.Pi0TreeHighestMomTrackPullElectron1[0]  = 0.
      self.Pi0TreeHighestMomTrackPullProton1[0]  = 0.
      self.Pi0TreeHighestMomTrackPullPion1[0] = 0.
      self.Pi0TreeHighestMomTrackPullKaon1[0]  = 0.
      self.Pi0TreeHighestMomTrackBest1[0] = 0

      self.Pi0TreeHighestMomTrackPullMuon2[0] = 0.
      self.Pi0TreeHighestMomTrackPullElectron2[0]  = 0.
      self.Pi0TreeHighestMomTrackPullProton2[0]  = 0.
      self.Pi0TreeHighestMomTrackPullPion2[0] = 0.
      self.Pi0TreeHighestMomTrackPullKaon2[0]  = 0.
      self.Pi0TreeHighestMomTrackBest2[0] = 0

      self.Pi0TreeHighestMomTrackPullMuon3[0] = 0.
      self.Pi0TreeHighestMomTrackPullElectron3[0]  = 0.
      self.Pi0TreeHighestMomTrackPullProton3[0]  = 0.
      self.Pi0TreeHighestMomTrackPullPion3[0] = 0.
      self.Pi0TreeHighestMomTrackPullKaon3[0]  = 0.
      self.Pi0TreeHighestMomTrackBest3[0] = 0

      self.Pi0TreeHighestMomTrackFGDNHits1[0] = 0
      self.Pi0TreeHighestMomTrackFGDPullMuon1[0] = 0.
      self.Pi0TreeHighestMomTrackFGDPullProton1[0]  = 0.
      self.Pi0TreeHighestMomTrackFGDPullPion1[0] = 0.
      self.Pi0TreeHighestMomTrackFGDPullNotSet1[0]  = 0.
      self.Pi0TreeHighestMomTrackBestFGD1[0] = 0
      self.Pi0TreeHighestMomTrackHasFGD1[0] = 0

      self.Pi0TreeHighestMomTrackFGDNHits2[0] = 0
      self.Pi0TreeHighestMomTrackFGDPullMuon2[0] = 0.
      self.Pi0TreeHighestMomTrackFGDPullProton2[0]  = 0.
      self.Pi0TreeHighestMomTrackFGDPullPion2[0] = 0.
      self.Pi0TreeHighestMomTrackFGDPullNotSet2[0]  = 0.
      self.Pi0TreeHighestMomTrackBestFGD2[0] = 0
      self.Pi0TreeHighestMomTrackHasFGD2[0] = 0
      
    def FinishAnalysis(self):

        if self.options.generatorType != "None":
          self.potSum[0] = sum(self.FilePOT.values())
          if self.options.userOptions:
            for o in self.options.userOptions:
              if type(o) == tuple and o[0].upper() == 'POT':
                self.potSum[0] *= float(o[1])

        elif self.options.userOptions:
          for o in self.options.userOptions:
            if type(o) == tuple and o[0].upper() == 'POT':
              self.potSum[0] = float(o[1])

        print "POT processed: " + str(self.potSum[0])
        self.potTree.Fill()

        userAnalysisBase.FinishAnalysis(self)

    def IsNCPi0InFGDWithPhotonsInECal(self):

      
      nFGD1 = 0
      nFGD2 = 0
      nOther = 0

      pi0id = -1

      for i in xrange(self.VertexOrdering()):
        ret = self.VertexOrdering(i)
        for j in xrange(ret[1]):
          v = ret[0][j]
          for p, pdg in zip(v.OtherNeutralIDs, v.OtherNeutralPDGs):
              if pdg == 111:
                pi0id = p
                if ret[2] == "FGD1":
                    nFGD1 += 1
                elif ret[2] == "FGD2":
                    nFGD2 += 1
                else:
                  nOther += 1;

      isNC = False
      try:
        if self.generatorUtils.IsNC(v):
          isNC = True
      except:
        if int(v.ReactionCode) >= 31:
          isNC = True
      if nFGD1 + nFGD2 == 1 and nOther == 0 and isNC:
        nGam = 0
        for i in xrange(self.truthTrajectories.NTrajsPhoton):
          t = self.truthTrajectories.TrajsPhoton[i]
          if t.ParentID == pi0id and t.FinalDet == 3:
            nGam += 1

        if nGam == 2:
          return True

      return False

    def IsInFGD(self, pos):
      x = pos.X()
      y = pos.Y()
      z = pos.Z()
      if abs(x) < 1000 and abs(y) < 1000:
        if z > 100 and z < 500:
          return True
        if z > 1440 and z < 1840:
          return True
      return False
      
    def GetNumBunches(self, mrrun):
      if mrrun >= 31 and mrrun <= 34:
        return 6
      if mrrun >= 36:
        return 8
      if mrrun == 2:
        return 8
      return 6

    def GetIntersectWithFGD(self, v, d, fm, fp):
      np = 0

      if abs(d.Z()) > 1e-3:
        alphapz = v.Z() - fp
        vertex = ROOT.TVector3(v.X(), v.Y(), v.Z())
        dir = ROOT.TVector3(d.X(), d.Y(), d.Z())
        dir *= (alphapz/d.Z())
        vpz = vertex + dir
        if abs(vpz.X()) < FGDX and abs(vpz.Y()) < FGDY:
          p1 = vpz
          np = 1
        alphamz = v.Z() - fm
        vertex = ROOT.TVector3(v.X(), v.Y(), v.Z())
        dir = ROOT.TVector3(d.X(), d.Y(), d.Z())
        dir *= (alphamz/d.Z())
        vmz = vertex + dir
        if abs(vmz.X()) < FGDX and abs(vmz.Y()) < FGDY:
          if np == 0:
            p1 = vmz
            np = 1
          elif np == 1:
            p2 = vmz
            np = 2
      
      if abs(d.X()) > 1e-3:
        alphapx = v.X() - FGDX
        vertex = ROOT.TVector3(v.X(), v.Y(), v.Z())
        dir = ROOT.TVector3(d.X(), d.Y(), d.Z())
        dir *= (alphapx/d.X())
        vpx = vertex  + dir
        if vpx.Z() > fm and vpx.Z() < fp and abs(vpx.Y()) < FGDY:
          if np == 0:
            p1 = vpx
            np = 1
          elif np == 1:
            p2 = vpx
            np = 2
        alphamx = v.X() + FGDX 
        vertex = ROOT.TVector3(v.X(), v.Y(), v.Z())
        dir = ROOT.TVector3(d.X(), d.Y(), d.Z())
        dir *= (alphamx/d.X())
        vmx = vertex  + dir
        if vmx.Z() > fm and vmx.Z() < fp and abs(vmx.Y()) < FGDY:
          if np == 0:
            p1 = vmx
            np = 1
          elif np == 1:
            p2 = vmx
            np = 2

      if abs(d.Y()) > 1e-3:
        alphapy = v.Y() - FGDY
        vertex = ROOT.TVector3(v.X(), v.Y(), v.Z())
        dir = ROOT.TVector3(d.X(), d.Y(), d.Z())
        dir *= (alphapy/d.Y())
        vpy = vertex  + dir
        if vpy.Z() > fm and vpy.Z() < fp and abs(vpy.X()) < FGDX:
          if np == 0:
            p1 = vpy
            np = 1
          elif np == 1:
            p2 = vpy
            np = 2
        alphamy = v.Y() + FGDY 
        vertex = ROOT.TVector3(v.X(), v.Y(), v.Z())
        dir = ROOT.TVector3(d.X(), d.Y(), d.Z())
        dir *= (alphamy/d.Y())
        vmy = vertex  + dir
        if vmy.Z() > fm and vmy.Z() < fp and abs(vmy.X()) < FGDX:
          if np == 0:
            p1 = vmy
            np = 1
          elif np == 1:
            p2 = vmy
            np = 2
      
      if np == 2 :
        p3 = p1 + p2
        p3 *= 0.5
        return p3
      
      return None
      #if abs(vp.X()) < FGDX and abs(vp.Y()) < FGDY:

    def GetConstrainedVertex(self, phpos, plpos, phdir, pldir):
      return ROOT.TLorentzVector(0.5*(phpos.X()+plpos.X()), 0.5*(phpos.Y()+plpos.Y()), 0.5*(FGD2MZ+FGD2PZ), 0.)

    def GetSkew(self, oh, ah, ol, al):
      r = ROOT.TMiscAnalysisUtils.GetSkew(oh, ah, ol, al)
      return [ r.Position, r.Distance, r.Angle_High, r.Angle_Low ]

    def GetDistanceFromFGD(self, p, d, mz, pz):
      alpha = p.z() - pz
      dd = ROOT.TVector3(-d.X(), -d.Y(), -d.Z())
      dd *= alpha
      x1 = abs(p.X() + dd.X())
      y1 = abs(p.Y() + dd.Y())

      alpha = p.z() - mz
      dd = ROOT.TVector3(-d.X(), -d.Y(), -d.Z())
      dd *= alpha
      x2 = abs(p.X() + dd.X())
      y2 = abs(p.Y() + dd.Y())

      return [min(x1, x2), min(y1, y2)]

    def GetDistanceToFGD(self, p):
      d = ROOT.TVector3(0.,0.,0.)

      if abs(p.X()) > FGDX:
        d.SetX(abs(p.X())-FGDX)

      if abs(p.Y()) > FGDY:
        d.SetY(abs(p.Y())-FGDY)
      
      if p.Z() < FGD1MZ:
        d.SetZ(FGD1MZ-p.Y());
      elif p.Z() > FGD1PZ and p.Z() < FGD2MZ:
        d.SetZ(min(FGD2MZ-p.Z(), p.Z()-FGD1PZ))
      elif p.Z() > FGD2PZ:
        d.SetZ(p.Z()-FGD2PZ)

      return d.Mag()

    def GetIntersections(self, oh, ah, ol, al):

      dh = ROOT.TVector3(ah.X(), 0., ah.Z())
      dl = ROOT.TVector3(al.X(), 0., al.Z())
      do = ROOT.TVector3(ol.X()-oh.X(), 0., ol.Z()-oh.Z())
      
      lhs = dh.Cross(dl)
      rhs = do.Cross(dl)
      dot = lhs.Dot(rhs)
      if dot < 0.:
        dot = -1.
      else:
        dot = 1.
      if lhs.Mag() > 0.:
        s = dot * rhs.Mag()/lhs.Mag()
        p1 = ROOT.TVector3(oh.X() + s*ah.X(), oh.Y() + s*ah.Y(), oh.Z() + s*ah.Z())
        d1 = self.GetDistanceToFGD(p1)
      else:
        p1 = ROOT.TVector3(0.,0.,0.)
        d1 = -2.

      dh = ROOT.TVector3(0., ah.Y(), ah.Z())
      dl = ROOT.TVector3(0., al.Y(), al.Z())
      do = ROOT.TVector3(0., ol.Y()-oh.Y(), ol.Z()-oh.Z())
      
      lhs = dh.Cross(dl)
      rhs = do.Cross(dl)
      dot = lhs.Dot(rhs)
      if dot < 0.:
        dot = -1.
      else:
        dot = 1.
      if lhs.Mag() > 0.:
        s = dot * rhs.Mag()/lhs.Mag()
        p2 = ROOT.TVector3(oh.X() + s*ah.X(), oh.Y() + s*ah.Y(), oh.Z() + s*ah.Z())
        d2 = self.GetDistanceToFGD(p2)
      else:
        p2 = ROOT.TVector3(0.,0.,0.)
        d2 = -2.

      #print p1.Z(), p2.Z(), d1, d2

      if d1 < d2 and d1 > -1.:
        return [[p1, d1], [p2, d2]]
      else:
        return [[p2, d2], [p1, d1]]






    def GetECalReconObject(self, pid):

      pids = []
      for i in xrange(self.ECal.NReconObject):
        e = self.ECal.ReconObject[i]
        pids.append(e.PID_TrShval)
        if e.PID_TrShval == pid:
          return e

      print "search for ", pid, "found",pids
      return None



    def EventAnalysis(self):

        if self.basicHeader.EventID < 0:
          return

        if self.options.userOptions and self.options.userOptions.count("pi0") > 0:
          if not self.IsNCPi0InFGDWithPhotonsInECal():
            self.GlobalBeamRun[0] = 2
            self.PerformMCTasks(0)
            if self.RooTracker:
              fn = self.basicHeader.GetCurrentFile().GetName()
              if fn not in self.FilePOT:
                filepot = self.RooTracker.Vtx[0].OrigTreePOT
                self.FilePOT[fn] = filepot
            return

        isMC = self.options.generatorType != "None"
          
        self.GlobalRunN[0] = self.basicHeader.RunID
        self.GlobalEventN[0] = self.basicHeader.EventID

        
        
          
        if not isMC and self.BSD.BeamSummaryDataStatus > 0:
          self.GlobalBeamRun[0] = self.BSD.BeamSummaryData[0].OtherData.MRRunNumber
          
        if isMC:
          self.GlobalRunN[0] = int(MCRunMatcher.search(self.basicHeader.GetCurrentFile().GetName()).group(2))*10000 + int(MCRunMatcher.search(self.basicHeader.GetCurrentFile().GetName()).group(3))
          self.GlobalBeamRun[0] = int(MCRunMatcher.search(self.basicHeader.GetCurrentFile().GetName()).group(1))

        deltatMC = FirstECalBunch[self.GlobalBeamRun[0]] - FirstECalBunch[1]

        self.FillMinBiasTree()

        hasPassedCut = {}
        for step in self.listOfAnalysisSteps:
            hasPassedCut[step] = False
        hasPassedCut["No Cuts"] = True
        pi0InFGD1 = False
        pi0InFGD2 = False
        pi0InTPC1 = False
        pi0InTPC2 = False
        pi0InTPC3 = False

        EMEnCorrection = 1.

        if isMC:
          hasPassedCut["Data Quality"] = True
          hasPassedCut["Good Spill"] = True

          if self.RooTracker:
            fn = self.basicHeader.GetCurrentFile().GetName()
            if fn not in self.FilePOT:
              filepot = self.RooTracker.Vtx[0].OrigTreePOT
              self.FilePOT[fn] = filepot

        else: # !isMC

          EMEnCorrection = 1./1.083

          if self.BSD.BeamSummaryDataStatus > 0 and self.BSD.BeamSummaryData[0].GoodSpillFlag == 1:
            hasPassedCut["Good Spill"] = True


            self.DQTreeND280[0]   = 5 + 5*(self.DQ.ND280OffFlag != 0)
            self.DQTreeTPC1[0]    = 5 + 5*(self.DQ.TPC1Flag != 0)
            self.DQTreeTPC2[0]    = 5 + 5*(self.DQ.TPC2Flag != 0)
            self.DQTreeTPC3[0]    = 5 + 5*(self.DQ.TPC3Flag != 0)
            self.DQTreeFGD1[0]    = 5 + 5*(self.DQ.FGD1Flag != 0)
            self.DQTreeFGD2[0]    = 5 + 5*(self.DQ.FGD2Flag != 0)
            self.DQTreeDsECal[0]  = 5 + 5*(self.DQ.DSECALFlag != 0)
            self.DQTreeBrlECal[0] = 5 + 5*(self.DQ.BarECALFlag != 0)
            self.DQTreeP0DECal[0] = 5 + 5*(self.DQ.P0DECALFlag != 0)
            self.DQTreeP0D[0]     = 5 + 5*(self.DQ.P0DFlag != 0)
            self.DQTreeSMRD[0]    = 5 + 5*(self.DQ.SMRDFlag != 0)
            self.DQTreeMagnet[0]  = 5 + 5*(self.DQ.MAGNETFlag != 0)
            self.DQTreeTPCAll[0]  = 5 + 5*(self.DQ.TPCFlag != 0)
            self.DQTreeFGDAll[0]  = 5 + 5*(self.DQ.FGDFlag != 0)
            self.DQTreeECalAll[0]  = 5 + 5*(self.DQ.ECALFlag != 0)

            self.rootTrees["DQTree"].Fill()
            self.DQTreeEventId[0] += 1

            if self.DQ.ND280OffFlag == 0:
              hasPassedCut["Data Quality"] = True
              self.potSum[0] += self.BSD.BeamSummaryData[0].CT5ProtonsPerSpill
              
        
        #numBunches = self.GetNumBunches(self.GlobalBeamRun[0])

        for bunch in [-1] + range(1, 10): #numBunches+2):

          self.GlobalBunchN[0] = bunch

          whichVol = self.PerformMCTasks(bunch)



          # Apply event preselection criteria
          self.PreselectionTreeNCand[0] = 0
          
          retparam = self.BunchTimes(bunch)
          EarliestTime = retparam[0]
          LatestTime = retparam[1]
          
          npids = 0
          if hasPassedCut["Good Spill"] and hasPassedCut["Data Quality"]:
            npids = self.Global.NPIDs
          
          # first look for 2 standalone clusters
          isPreselected = False
          for i in xrange(npids):
            pid = self.Global.PIDs[i]
            
            t1 = self.FindAdjustedECalTime(pid, i, self.GlobalBeamRun[0])
            
            if t1 < EarliestTime or t1 > LatestTime:
              continue
              
            isStandaloneClus = False
            if pid.NECALs == 1 and pid.NTPCs == 0 and pid.NFGDs == 0 and pid.NP0Ds == 0 and pid.NSMRDs == 0:
              if pid.ECAL[0].Detector == 9:
                isStandaloneClus = True

            if not isStandaloneClus:
              continue

            for j in xrange(i+1, npids):
              pid2 = self.Global.PIDs[j]
              t2 = self.FindAdjustedECalTime(pid2, j, self.GlobalBeamRun[0])
              if t2 < EarliestTime or t2 > LatestTime:
                continue
              if pid2.NECALs == 1 and pid2.NTPCs == 0 and pid2.NFGDs == 0 and pid2.NP0Ds == 0 and pid2.NSMRDs == 0:
                if pid2.ECAL[0].Detector == 9:
                  isStandaloneClus = True


              if isStandaloneClus:
                hasPassedCut["Preselected"] = True
                isPreselected = True
                



          if isPreselected:
            self.PreselectionTreeNCand[0] += 1
            self.PreselectionTreeNStandaloneClus[0] = 0
            self.PreselectionTreeNTrackerMatchedClus[0] = 0
            self.PreselectionTreeNTrackerStandaloneTracks[0] = 0
            self.PreselectionTreeNOtherMatchedClus[0] = 0
            self.PreselectionTreeNTrackerOtherTracks[0] = 0
            self.PreselectionTreeNOthers[0] = 0
            for i in xrange(self.Global.NPIDs):
              pid = self.Global.PIDs[i]
              
              t1 = self.FindAdjustedECalTime(pid, i, self.GlobalBeamRun[0])
              
              if t1 < EarliestTime or t1 > LatestTime:
                continue
              
              if pid.NECALs > 0 and pid.NTPCs == 0 and pid.NFGDs == 0 and pid.NP0Ds == 0 and pid.NSMRDs == 0:
                self.PreselectionTreeNStandaloneClus[0] += 1
              elif pid.NECALs > 0 and pid.NTPCs + pid.NFGDs > 0 and pid.NP0Ds == 0 and pid.NSMRDs == 0:
                self.PreselectionTreeNTrackerMatchedClus[0] += 1
              elif pid.NTPCs + pid.NFGDs > 0 and pid.NP0Ds == 0 and pid.NSMRDs == 0:
                self.PreselectionTreeNTrackerStandaloneTracks[0] += 1
              elif pid.NECALs > 0:
                self.PreselectionTreeNOtherMatchedClus[0] += 1
              elif pid.NTPCs + pid.NFGDs > 0:
                self.PreselectionTreeNTrackerOtherTracks[0] += 1
              else:
                self.PreselectionTreeNOthers[0] += 1
              
            
            self.rootTrees["PreselectionTree"].Fill()
            hasPassedCut["Preselected"] = True
          
            # One track analysis
            for i in xrange(self.Pi0.NPi0):
              pi0reco = self.Pi0.Pi0[i]
              if not (pi0reco.PhotonHigh.Module == DsECalModule and pi0reco.PhotonLow.Module == DsECalModule):
                continue

              #if pi0reco.DeltaT > 400. or pi0reco.Position.T() < EarliestTime or pi0reco.Position.T() > LatestTime:
              #  continue


              pi0recot = pi0reco.Position.T()
              if pi0recot < EarliestTime or pi0recot > LatestTime:
                continue

              plow = pi0reco.PhotonLow
              phigh = pi0reco.PhotonHigh

              pearlier = plow
              plater = phigh

              if pearlier.Position.T() > plater.Position.T():
                pearlier = phigh
                plater = plow



              ecalobj = self.GetECalReconObject(phigh.PID)
              if ecalobj:
                  self.Pi0TreeRecoHighMichelTags[0] = int(ecalobj.MElectronTag_NDelayedCluster)
                  self.Pi0TreeRecoHighNumHits[0] = ecalobj.NHits
                  self.Pi0TreeRecoHighHad[0] = ecalobj.PID_EMHadVal
                  self.Pi0TreeRecoHighNHitLayer[0] = ecalobj.NLayersHit
                  self.Pi0TreeRecoHighConeAngle[0] = ecalobj.Shower.ConeAngle.X()
                  self.Pi0TreeRecoHighAMR[0] = ecalobj.PID_AMR
                  self.Pi0TreeRecoHighAsymmetry[0] = ecalobj.PID_Asymmetry
                  self.Pi0TreeRecoHighMeanPosition[0] = ecalobj.PID_MeanPosition
                  self.Pi0TreeRecoHighQSkew[0] = ecalobj.PID_Qskew
                  self.Pi0TreeRecoHighShowerWidth[0] = ecalobj.PID_ShowerWidth
                  self.Pi0TreeRecoHighEMLikelihood[0] = ecalobj.PID_EMLikelihood
                  self.Pi0TreeRecoHighMatchingLikelihood[0] = ecalobj.MatchingLikelihood
                  self.Pi0TreeRecoHighThrustDotPCA[0] = abs(ecalobj.Shower.Direction.Dot(phigh.ThrustAxis))
              ecalobj = self.GetECalReconObject(plow.PID)
              if ecalobj:
                  self.Pi0TreeRecoLowMichelTags[0] = int(ecalobj.MElectronTag_NDelayedCluster)
                  self.Pi0TreeRecoLowNumHits[0] = ecalobj.NHits
                  self.Pi0TreeRecoLowHad[0] = ecalobj.PID_EMHadVal
                  self.Pi0TreeRecoLowNHitLayer[0] = ecalobj.NLayersHit
                  self.Pi0TreeRecoLowConeAngle[0] = ecalobj.Shower.ConeAngle.X()
                  self.Pi0TreeRecoLowAMR[0] = ecalobj.PID_AMR
                  self.Pi0TreeRecoLowAsymmetry[0] = ecalobj.PID_Asymmetry
                  self.Pi0TreeRecoLowMeanPosition[0] = ecalobj.PID_MeanPosition
                  self.Pi0TreeRecoLowQSkew[0] = ecalobj.PID_Qskew
                  self.Pi0TreeRecoLowShowerWidth[0] = ecalobj.PID_ShowerWidth
                  self.Pi0TreeRecoLowEMLikelihood[0] = ecalobj.PID_EMLikelihood
                  self.Pi0TreeRecoLowMatchingLikelihood[0] = ecalobj.MatchingLikelihood
                  self.Pi0TreeRecoLowThrustDotPCA[0] = abs(ecalobj.Shower.Direction.Dot(plow.ThrustAxis))

              self.Pi0TreeTrackerNumTracks[0] = 0
              self.Pi0TreeTrackerNumVertices[0] = 0
              self.Pi0TreeNumNonTrackerVertices[0] = 0
              self.Pi0TreeTrackerNumIsolatedDsECalClusters[0] = 0
              self.Pi0TreeTrackerNumIsolatedBrlECalClusters[0] = 0
              #self.Pi0TreeTrackerIStart[0] = self.Pi0TreeTrackerIEnd[0]
              self.Pi0TreeTrackerNumTPC1[0] = 0
              self.Pi0TreeTrackerNumTPC2[0] = 0
              self.Pi0TreeTrackerNumTPC3[0] = 0
              self.Pi0TreeTrackerNumFGD1[0] = 0
              self.Pi0TreeTrackerNumFGD2[0] = 0
              self.Pi0TreeTrackerNumP0Ds[0] = 0
              self.Pi0TreeTrackerNumSMRD[0] = 0
              self.Pi0TreeGlobalVertexX[0] = 0.
              self.Pi0TreeGlobalVertexY[0] = 0.
              self.Pi0TreeGlobalVertexZ[0] = 0.

              maxk = -1;
              maxmom = -1;

              #hasTrackerV = False
              usedVertexPIDs = []
              vertexHighestMoms = dict()

              for k in xrange(self.Global.NVertices):
                vertex = self.Global.Vertices[k]
                vertext = vertex.Position.T()
                if abs(pi0recot - vertext) < DeltaPi0TracksTime and vertex.PrimaryIndex == -1: 
                  if vertex.Position.Z() > 0.:
                    self.Pi0TreeTrackerNumVertices[0] += 1

                    vmaxmom = -1

                    for p in xrange(vertex.NConstituents):
                      pid = self.Global.PIDs[vertex.ConstituentPID[p]]
                      mom = pid.Momentum
                      if mom > vmaxmom:
                        vmaxmom = mom
                        vertexHighestMoms[k] = (vertex.Position.Z(), vertex.ConstituentPID[p])

                  else: # negative z
                    self.Pi0TreeNumNonTrackerVertices[0] += 1

                for p in xrange(vertex.NConstituents):
                  usedVertexPIDs.append(vertex.ConstituentPID[p])
                for kk in xrange(self.Global.NVertices):
                  secver = self.Global.Vertices[kk]
                  if secver.PrimaryIndex == k:
                    for pp in xrange(secver.NConstituents):
                      usedVertexPIDs.append(secver.ConstituentPID[pp])

              maxz = -1.

              for k,(z,m) in vertexHighestMoms.items():
                if z > maxz:
                  maxz = z
                  maxk = m
                  vertex = self.Global.Vertices[k]
                  self.Pi0TreeGlobalVertexX[0] = vertex.Position.X()
                  self.Pi0TreeGlobalVertexY[0] = vertex.Position.Y()
                  self.Pi0TreeGlobalVertexZ[0] = vertex.Position.Z()



              for k in xrange(self.Global.NPIDs):
                pid = self.Global.PIDs[k]
                trackrecot = self.FindAdjustedECalTime(pid, k, self.GlobalBeamRun[0])
                if abs(pi0recot - trackrecot) < DeltaPi0TracksTime:
                  hasTPC1 = False
                  for j in xrange(pid.NTPCs):
                    tpc = pid.TPC[j]
                    if tpc.Detector == 1:
                      self.Pi0TreeTrackerNumTPC1[0] += 1
                      hasTPC1 = True
                    if tpc.Detector == 2:
                      self.Pi0TreeTrackerNumTPC2[0] += 1
                    if tpc.Detector == 3:
                      self.Pi0TreeTrackerNumTPC3[0] += 1
                  for j in xrange(pid.NFGDs):
                    fgd = pid.FGD[j]
                    if fgd.Detector == 1:
                      self.Pi0TreeTrackerNumFGD1[0] += 1
                    if fgd.Detector == 2:
                      self.Pi0TreeTrackerNumFGD2[0] += 1
                  for j in xrange(pid.NP0Ds):
                    self.Pi0TreeTrackerNumP0Ds[0] += 1
                  for j in xrange(pid.NSMRDs):
                    self.Pi0TreeTrackerNumSMRD[0] += 1
                  if pid.NTPCs + pid.NECALs + pid.NFGDs == 0:
                    continue
                  if pid.NP0Ds > 0:
                    continue
                  if hasTPC1:
                    continue
                  if pid.NECALs == 1 and pid.NTPCs == 0 and pid.NFGDs == 0 and pid.NSMRDs == 0:
                    ecal = pid.ECAL[0]
                    if ecal.Detector == 9:
                      self.Pi0TreeTrackerNumIsolatedDsECalClusters[0] += 1
                    elif ecal.Detector > 4:
                      self.Pi0TreeTrackerNumIsolatedBrlECalClusters[0] += 1
                  if pid.NTPCs + pid.NFGDs > 0 and maxz == -1. and k not in usedVertexPIDs:
                    self.Pi0TreeTrackerNumTracks[0] += 1
                    if maxmom < pid.Momentum:
                      maxmom = pid.Momentum
                      maxk = k
                  
                    #self.TrackerTreeDeltaT[0] = abs(pi0recot - trackrecot)
                    #self.TrackerTreeTrackerI[0] = self.Pi0TreeTrackerIEnd[0]

                    #self.TrackerTreentpcs[0] = pid.NTPCs
                    #self.TrackerTreeTrackX[0] = pid.FrontPosition.X()
                    #self.TrackerTreeTrackY[0] = pid.FrontPosition.Y()
                    #self.TrackerTreeTrackZ[0] = pid.FrontPosition.Z()
                    #self.TrackerTreeglobtime[0] = pid.FrontPosition.T()

                    #for j in xrange(pid.NTPCs):
                      #tpc = pid.TPC[j]

                      #self.TrackerTreeccorr[j] = tpc.Ccorr
                      #self.TrackerTreepullE[j] = tpc.PullEle
                      #self.TrackerTreepullMu[j] = tpc.PullMuon
                      #self.TrackerTreepullP[j] = tpc.PullProton
                      #self.TrackerTreepullPi[j] = tpc.PullPion
                      #self.TrackerTreepullK[j] = tpc.PullKaon
                      #self.TrackerTreeglobmom[j] = tpc.FrontMomentum
                      #self.TrackerTreeglobcharge[j] = tpc.Charge
                      #self.TrackerTreedet[j] = tpc.Detector



                    #self.TrackerTreeecalInfo[0] = pid.NECALs
                    #self.TrackerTreefgdInfo[0] = pid.NFGDs
                    #if pid.NECALs > 0:
                    #  self.TrackerTreeecalE[0] = pid.ECAL[0].TrShVal
   
                    #self.rootTrees["TrackerTree"].Fill()
                    #self.Pi0TreeTrackerIEnd[0] += 1
              self.ResetHighestTrackParams()
              if maxk > -1:
                pid = self.Global.PIDs[maxk]
                self.Pi0TreeHighestMomTrackMomentum[0] = pid.Momentum
                self.Pi0TreeHighestMomTrackCharge[0] = int(pid.Charge)
                self.Pi0TreeHighestMomTrackPosX[0] = pid.FrontPosition.X()
                self.Pi0TreeHighestMomTrackPosY[0] = pid.FrontPosition.Y()
                self.Pi0TreeHighestMomTrackPosZ[0] = pid.FrontPosition.Z()
                adjtime = self.FindAdjustedECalTime(pid, maxk, self.GlobalBeamRun[0])
                self.Pi0TreeHighestMomTrackTime[0] = adjtime -deltatMC -(self.GlobalBunchN[0]-1) * 582.
                self.Pi0TreeHighestMomTrackBunchSigma[0] = GetBunchSigma(adjtime, self.GlobalBeamRun[0], self.GlobalBunchN[0])

                dxv = ROOT.TVector3(self.Pi0TreeTrueVertexX[0], self.Pi0TreeTrueVertexY[0], self.Pi0TreeTrueVertexZ[0])
                dxv -= pid.FrontPosition.Vect()
                self.Pi0TreeCompareTrackerDeltaPos[0] = dxv.Mag()

                if pid.NECALs > 0:
                  self.Pi0TreeHighestMomTrackHasE[0] = 1
                  self.Pi0TreeHighestMomTrackPidE[0] = pid.ECAL[0].TrShVal
                  self.Pi0TreeHighestMomTrackHadE[0] = pid.ECAL[0].EMHadVal

                if pid.NSMRDs > 0:
                  self.Pi0TreeHighestMomTrackHasS[0] = 1

                for j in xrange(pid.NTPCs):
                  tpc = pid.TPC[j]
                  bestpull = 990e3
                  bestdet = 0
                  if bestpull > abs(tpc.PullMuon):
                    bestdet = 1
                    bestpull = abs(tpc.PullMuon)
                  if bestpull > abs(tpc.PullEle):
                    bestdet = 2
                    bestpull = abs(tpc.PullEle)
                  if bestpull > abs(tpc.PullProton):
                    bestdet = 3
                    bestpull = abs(tpc.PullProton)
                  if bestpull > abs(tpc.PullPion):
                    bestdet = 4
                    bestpull = abs(tpc.PullPion)
                  if bestpull > abs(tpc.PullKaon):
                    bestdet = 5
                    bestpull = abs(tpc.PullKaon)
                  if tpc.Detector == 1:
                    self.Pi0TreeHighestMomTrackHas1[0] = 1
                    self.Pi0TreeHighestMomTrackPullMuon1[0] = tpc.PullMuon
                    self.Pi0TreeHighestMomTrackPullElectron1[0]  = tpc.PullEle
                    self.Pi0TreeHighestMomTrackPullProton1[0]  = tpc.PullProton
                    self.Pi0TreeHighestMomTrackPullPion1[0] = tpc.PullPion
                    self.Pi0TreeHighestMomTrackPullKaon1[0]  = tpc.PullKaon
                    self.Pi0TreeHighestMomTrackBest1[0] = bestdet
                  if tpc.Detector == 2:
                    self.Pi0TreeHighestMomTrackHas2[0] = 1
                    self.Pi0TreeHighestMomTrackPullMuon2[0] = tpc.PullMuon
                    self.Pi0TreeHighestMomTrackPullElectron2[0]  = tpc.PullEle
                    self.Pi0TreeHighestMomTrackPullProton2[0]  = tpc.PullProton
                    self.Pi0TreeHighestMomTrackPullPion2[0] = tpc.PullPion
                    self.Pi0TreeHighestMomTrackPullKaon2[0]  = tpc.PullKaon
                    self.Pi0TreeHighestMomTrackBest2[0] = bestdet
                  if tpc.Detector == 3:
                    self.Pi0TreeHighestMomTrackHas3[0] = 1
                    self.Pi0TreeHighestMomTrackPullMuon3[0] = tpc.PullMuon
                    self.Pi0TreeHighestMomTrackPullElectron3[0]  = tpc.PullEle
                    self.Pi0TreeHighestMomTrackPullProton3[0]  = tpc.PullProton
                    self.Pi0TreeHighestMomTrackPullPion3[0] = tpc.PullPion
                    self.Pi0TreeHighestMomTrackPullKaon3[0]  = tpc.PullKaon
                    self.Pi0TreeHighestMomTrackBest3[0] = bestdet

                for j in xrange(pid.NFGDs):
                  fgd = pid.FGD[j]
                  bestpull = 9999.
                  bestdet = 0
                  if bestpull > abs(fgd.PullMuon):
                    bestpull = abs(fgd.PullMuon)
                    bestdet = 1
                  if bestpull > abs(fgd.PullPion):
                    bestpull = abs(fgd.PullPion)
                    bestdet = 4
                  if bestpull > abs(fgd.PullProton):
                    bestpull = abs(fgd.PullProton)
                    bestdet = 3
                  if bestpull > abs(fgd.PullNotSet):
                    bestpull = abs(fgd.PullNotSet)
                    bestdet = -1
                  if fgd.Detector == 1:
                    self.Pi0TreeHighestMomTrackHasFGD1[0] = 1
                    self.Pi0TreeHighestMomTrackBestFGD1[0] = bestdet
                    self.Pi0TreeHighestMomTrackFGDNHits1[0] = fgd.NHits
                    self.Pi0TreeHighestMomTrackFGDPullMuon1[0] = fgd.PullMuon
                    self.Pi0TreeHighestMomTrackFGDPullPion1[0] = fgd.PullPion
                    self.Pi0TreeHighestMomTrackFGDPullProton1[0]  = fgd.PullProton
                    self.Pi0TreeHighestMomTrackFGDPullNotSet1[0]  = fgd.PullNotSet
                  if fgd.Detector == 2:
                    self.Pi0TreeHighestMomTrackHasFGD2[0] = 1
                    self.Pi0TreeHighestMomTrackBestFGD2[0] = bestdet
                    self.Pi0TreeHighestMomTrackFGDNHits2[0] = fgd.NHits
                    self.Pi0TreeHighestMomTrackFGDPullMuon2[0] = fgd.PullMuon
                    self.Pi0TreeHighestMomTrackFGDPullPion2[0] = fgd.PullPion
                    self.Pi0TreeHighestMomTrackFGDPullProton2[0]  = fgd.PullProton
                    self.Pi0TreeHighestMomTrackFGDPullNotSet2[0]  = fgd.PullNotSet


              self.Pi0TreeFGDNumTracks[0] = 0
              self.Pi0TreeFGDNum2DXTracks[0] = 0
              self.Pi0TreeFGDNum2DYTracks[0] = 0
              if self.Pi0TreeTrackerNumVertices[0] == 0 and self.Pi0TreeTrackerNumTracks[0] == 0:
                for k in xrange(self.FGD.NXYZTracks):
                  fgdTrack = self.FGD.XYZTracks[k]
                  t = fgdTrack.Origin.T()
                  if abs(pi0recot - t) < DeltaPi0TracksTime:
                    self.Pi0TreeFGDNumTracks[0] += 1
                    self.Pi0TreeFGDTrackX[0] = fgdTrack.Origin.X()
                    self.Pi0TreeFGDTrackY[0] = fgdTrack.Origin.Y()
                    self.Pi0TreeFGDTrackZ[0] = fgdTrack.Origin.Z()
                for k in xrange(self.FGD.NXZTracks):
                  fgdTrack = self.FGD.XZTracks[k]
                  t = fgdTrack.Origin.T()
                  if abs(pi0recot - t) < DeltaPi0TracksTime:
                    self.Pi0TreeFGDNum2DXTracks[0] += 1
                    self.Pi0TreeFGD2DXTrackX[0] = fgdTrack.Origin.X()
                    self.Pi0TreeFGD2DXTrackZ[0] = fgdTrack.Origin.Z()
                for k in xrange(self.FGD.NYZTracks):
                  fgdTrack = self.FGD.YZTracks[k]
                  t = fgdTrack.Origin.T()
                  if abs(pi0recot - t) < DeltaPi0TracksTime:
                    self.Pi0TreeFGDNum2DYTracks[0] += 1
                    self.Pi0TreeFGD2DYTrackY[0] = fgdTrack.Origin.Y()
                    self.Pi0TreeFGD2DYTrackZ[0] = fgdTrack.Origin.Z()


              self.Pi0TreeFGD1Unused[0] = 0
              self.Pi0TreeFGD2Unused[0] = 0
              if self.Pi0TreeTrackerNumVertices[0] == 0 and self.Pi0TreeTrackerNumTracks[0] == 0:
                fgd1minzx = 10000.
                fgd1minzy = 10000.
                fgd1minz = 10000.
                fgd2minzx = 10000.
                fgd2minzy = 10000.
                fgd2minz = 10000.
                fgd1minx = 0.
                fgd1nx = 1.
                fgd1miny = 0.
                fgd1ny = 1. 
                fgd2minx = 0.
                fgd2nx = 1.
                fgd2miny = 0.
                fgd2ny = 1.
                for k in xrange(self.Global.NFgdTimeBins):
                  fgdbin = self.Global.FgdTimeBins[k]
                  for l in xrange(fgdbin.NFGD1Unused):
                    fgdunused = fgdbin.FGD1Unused[l]
                    if abs(pi0recot - fgdunused.Time) < DeltaPi0TracksTime:
                      self.Pi0TreeFGD1Unused[0] += 1
                      if fgdunused.Type == 101: # xz hit
                        if fgdunused.Position.Z() < fgd1minzx:
                          fgd1minzx = fgdunused.Position.Z()
                          fgd1minx = 0.
                          fgd1nx = 0.
                          if fgd1minzx < fgd1minzy:
                            fgd1minz = fgd1minzx
                        if not fgdunused.Position.Z() > fgd1minzx+.1:
                          fgd1nx += 1.
                          fgd1minx += fgdunused.Position.X()

                      elif fgdunused.Type == 110: # yz hit
                        if fgdunused.Position.Z() < fgd1minzy:
                          fgd1minzy = fgdunused.Position.Z()
                          fgd1miny = 0.
                          fgd1ny = 0.
                          if fgd1minzy < fgd1minzx:
                            fgd1minz = fgd1minzy
                        if not fgdunused.Position.Z() > fgd1minzy+.1:
                          fgd1ny += 1.
                          fgd1miny += fgdunused.Position.Y()

                  for l in xrange(fgdbin.NFGD2Unused):
                    fgdunused = fgdbin.FGD2Unused[l]
                    if abs(pi0recot - fgdunused.Time) < DeltaPi0TracksTime:
                      self.Pi0TreeFGD2Unused[0] += 1
                      if fgdunused.Type == 101: # xz hit
                        if fgdunused.Position.Z() < fgd2minzx:
                          fgd2minzx = fgdunused.Position.Z()
                          fgd2minx = 0.
                          fgd2nx = 0.
                          if fgd2minzx < fgd2minzy:
                            fgd2minz = fgd2minzx
                        if not fgdunused.Position.Z() > fgd2minzx+.1:
                          fgd2nx += 1.
                          fgd2minx += fgdunused.Position.X()

                      elif fgdunused.Type == 110: # yz hit
                        if fgdunused.Position.Z() < fgd2minzy:
                          fgd2minzy = fgdunused.Position.Z()
                          fgd2miny = 0.
                          fgd2ny = 0.
                          if fgd2minzy < fgd2minzx:
                            fgd2minz = fgd2minzy
                        if not fgdunused.Position.Z() > fgd2minzy+.1:
                          fgd2ny += 1.
                          fgd2miny += fgdunused.Position.Y()
                        #
                self.Pi0TreeFGD1UnusedMinX[0] = fgd1minx/fgd1nx
                self.Pi0TreeFGD1UnusedMinY[0] = fgd1miny/fgd1ny
                self.Pi0TreeFGD1UnusedMinZ[0] = fgd1minz
                self.Pi0TreeFGD2UnusedMinX[0] = fgd2minx/fgd2nx
                self.Pi0TreeFGD2UnusedMinY[0] = fgd2miny/fgd2ny
                self.Pi0TreeFGD2UnusedMinZ[0] = fgd2minz





              retparam = self.GetWeights(phigh.TrueParticles, phigh.Module, phigh.ThrustOrigin)
              self.Pi0TreeTrueHighPi0PhotonWeight[0] = retparam[0]
              self.Pi0TreeTrueHighNumPhotonTrajectories[0] = retparam[1]
              if isMC:
                self.Pi0TreeTrueHighMainContributorPDGId[0] = self.ParticleNaming(retparam[2])[0]
              self.Pi0TreeTrueHighMainContributorWeight[0] = retparam[3]
              self.Pi0TreeTrueHighClusterEnergy[0] = retparam[4]
              self.Pi0TreeTrueHighTrajectoryID[0] = retparam[5]
              self.Pi0TreeTrueHighNumContributors[0] = retparam[6]
              self.Pi0TreeTrueHighInteractionUID[0] = retparam[7]
              self.Pi0TreeTrueHighMainContributorTrajId[0] = retparam[8]
              if isMC:
                self.Pi0TreeTrueHighMainContributorParentPDGId[0] = self.ParticleNaming(retparam[9])[0]
              self.Pi0TreeTrueHighMainContributorParentTrajId[0] = retparam[10]
              self.Pi0TreeTrueHighMainContributorParentPrimary[0] = retparam[11]
              if isMC:
                retparam = self.GetMCInteractionInfo(self.Pi0TreeTrueHighInteractionUID[0])
                self.Pi0TreeTrueHighInteractionPosX[0] = retparam[0]
                self.Pi0TreeTrueHighInteractionPosY[0] = retparam[1]
                self.Pi0TreeTrueHighInteractionPosZ[0] = retparam[2]
                self.Pi0TreeTrueHighInteractionTime[0] = retparam[3]
                self.Pi0TreeTrueHighInteractionDetector[0] = retparam[4]
                self.Pi0TreeTrueHighInteractionCCNC[0] = retparam[5]
                self.Pi0TreeTrueHighInteractionNumLeptons[0] = retparam[6]
                self.Pi0TreeTrueHighInteractionNumPi0s[0] = retparam[7]
                self.Pi0TreeTrueHighInteractionNumPions[0] = retparam[8]
                self.Pi0TreeTrueHighInteractionNumProtons[0] = retparam[9]
                self.Pi0TreeTrueHighInteractionNumNeutrons[0] = retparam[10]
                self.Pi0TreeTrueHighInteractionNumPhotons[0] = retparam[11]
                self.Pi0TreeTrueHighInteractionNumOther[0] = retparam[12]
                self.Pi0TreeTrueHighInteractionPi0Mom[0] = retparam[13]
                self.Pi0TreeTrueHighInteractionPi0OpeningAngle[0] = retparam[14]
                self.Pi0TreeTrueHighInteractionPi0DirZ[0] = retparam[15]


              retparam = self.GetWeights(plow.TrueParticles, plow.Module, plow.ThrustOrigin)
              self.Pi0TreeTrueLowPi0PhotonWeight[0] = retparam[0]
              self.Pi0TreeTrueLowNumPhotonTrajectories[0] = retparam[1]
              if isMC:
                self.Pi0TreeTrueLowMainContributorPDGId[0] = self.ParticleNaming(retparam[2])[0]
              self.Pi0TreeTrueLowMainContributorWeight[0] = retparam[3]
              self.Pi0TreeTrueLowClusterEnergy[0] = retparam[4]
              self.Pi0TreeTrueLowTrajectoryID[0] = retparam[5]
              self.Pi0TreeTrueLowNumContributors[0] = retparam[6]
              self.Pi0TreeTrueLowInteractionUID[0] = retparam[7]
              self.Pi0TreeTrueLowMainContributorTrajId[0] = retparam[8]
              #self.Pi0TreeTrueLowMainContributorPi0ParentId[0] = retparam[9]
              if isMC:
                self.Pi0TreeTrueLowMainContributorParentPDGId[0] = self.ParticleNaming(retparam[9])[0]
              self.Pi0TreeTrueLowMainContributorParentTrajId[0] = retparam[10]
              self.Pi0TreeTrueLowMainContributorParentPrimary[0] = retparam[11]
              if isMC:
                retparam = self.GetMCInteractionInfo(self.Pi0TreeTrueLowInteractionUID[0])
                self.Pi0TreeTrueLowInteractionPosX[0] = retparam[0]
                self.Pi0TreeTrueLowInteractionPosY[0] = retparam[1]
                self.Pi0TreeTrueLowInteractionPosZ[0] = retparam[2]
                self.Pi0TreeTrueLowInteractionTime[0] = retparam[3]
                self.Pi0TreeTrueLowInteractionDetector[0] = retparam[4]
                self.Pi0TreeTrueLowInteractionCCNC[0] = retparam[5]
                self.Pi0TreeTrueLowInteractionNumLeptons[0] = retparam[6]
                self.Pi0TreeTrueLowInteractionNumPi0s[0] = retparam[7]
                self.Pi0TreeTrueLowInteractionNumPions[0] = retparam[8]
                self.Pi0TreeTrueLowInteractionNumProtons[0] = retparam[9]
                self.Pi0TreeTrueLowInteractionNumNeutrons[0] = retparam[10]
                self.Pi0TreeTrueLowInteractionNumPhotons[0] = retparam[11]
                self.Pi0TreeTrueLowInteractionNumOther[0] = retparam[12]
                self.Pi0TreeTrueLowInteractionPi0Mom[0] = retparam[13]
                self.Pi0TreeTrueLowInteractionPi0OpeningAngle[0] = retparam[14]
                self.Pi0TreeTrueLowInteractionPi0DirZ[0] = retparam[15]

              retparam = self.PhotonMCInfo(self.Pi0TreeTrueHighTrajectoryID[0], phigh.Module)
              self.Pi0TreeTrueHighPosX[0] = retparam[0]
              self.Pi0TreeTrueHighPosY[0] = retparam[1]
              self.Pi0TreeTrueHighPosZ[0] = retparam[2]
              self.Pi0TreeTrueHighTime[0] = retparam[3]
              self.Pi0TreeTrueHighIncidenceAngle[0] = retparam[4]

              retparam = self.PhotonMCInfo(self.Pi0TreeTrueLowTrajectoryID[0], plow.Module)
              self.Pi0TreeTrueLowPosX[0] = retparam[0]
              self.Pi0TreeTrueLowPosY[0] = retparam[1]
              self.Pi0TreeTrueLowPosZ[0] = retparam[2]
              self.Pi0TreeTrueLowTime[0] = retparam[3]
              self.Pi0TreeTrueLowIncidenceAngle[0] = retparam[4]

              retparam = self.GetOverlaps(phigh.TrueParticles, plow.TrueParticles)
              self.Pi0TreeTrueNumOverlaps[0] = retparam[0]
              self.Pi0TreeTrueHighOverlapFromOther[0] = retparam[1]

              retparam = self.GetOverlaps(plow.TrueParticles, phigh.TrueParticles)
              self.Pi0TreeTrueLowOverlapFromOther[0] = retparam[1]
              
              #retparam = self.AreBothPhotonsPurelyFromSamePi0(phigh.TrueParticles, plow.TrueParticles)
              #pi0id = retparam[0]
              #photHid = retparam[1]
              #photLid = retparam[2]
              #self.Pi0TreeTrueHighNumPhotonContributors[0] = retparam[3]
              #self.Pi0TreeTrueLowNumPhotonContributors[0] = retparam[4]
              #self.Pi0TreeTrueHighPhotonWeight[0] = retparam[5]
              #self.Pi0TreeTrueLowPhotonWeight[0] = retparam[6]

              # Categorise the signal type
              # 1 = two seperate photons, both perfectly reconstructed, from pi0 (golden signal)
              # 2 = one photon, two clusters (fragmentation), from pi0
              # 3 = one of the photons is perfectly reconstructed, the other is something else
              # 0 = anything else
              
              #if pi0id > 0 and photHid > 0 and photLid > 0 and photHid != photLid:
              #  self.Pi0TreeTruePhotonsSignalType[0] = 1
              #elif photHid > 0 and photHid == photLid:
              #  self.Pi0TreeTruePhotonsSignalType[0] = 2
              #elif photHid >0 or photLid > 0:
              #  self.Pi0TreeTruePhotonsSignalType[0] = 3
              #else:
              #  self.Pi0TreeTruePhotonsSignalType[0] = 0
              #self.Pi0TreeTruePi0ID[0] = pi0id
                
              

              #self.Pi0TreeRecoMass[0] = pi0reco.Mass
              #self.Pi0TreeRecoX[0] = pi0reco.Position.X()
              #self.Pi0TreeRecoY[0] = pi0reco.Position.Y()
              #self.Pi0TreeRecoZ[0] = pi0reco.Position.Z()
              self.Pi0TreeRecoT[0] = pi0reco.Position.T() - deltatMC - (self.GlobalBunchN[0]-1)*582.
              #self.Pi0TreeRecoP[0] = pi0reco.Momentum.Mag()
              self.Pi0TreeRecoLowUncorrectedEnergy[0] = plow.Energy
              self.Pi0TreeRecoHighUncorrectedEnergy[0] = phigh.Energy
              self.Pi0TreeRecoLowEnergy[0] = EMEnCorrection * plow.Energy
              self.Pi0TreeRecoHighEnergy[0] = EMEnCorrection * phigh.Energy
              self.Pi0TreeRecoPhotonInvariantMass[0] = EMEnCorrection * math.sqrt(2 * plow.Energy * phigh.Energy * (1 - plow.ThrustAxis.Dot(phigh.ThrustAxis)))
              retparam = self.GetIntersections(phigh.ThrustOrigin, phigh.ThrustAxis, plow.ThrustOrigin, plow.ThrustAxis)
              pos = retparam[0][0]
              altpos = retparam[1][0] 
              self.Pi0TreeRecoX[0] = pos.X()
              self.Pi0TreeRecoY[0] = pos.Y()
              self.Pi0TreeRecoZ[0] = pos.Z()
              self.Pi0TreeRecoDFGD[0] = retparam[0][1]
              self.Pi0TreeRecoAltDFGD[0] = retparam[1][1]
              retparam = self.Pi0MassMom(pos, phigh.ThrustOrigin, plow.ThrustOrigin, self.Pi0TreeRecoHighEnergy[0], self.Pi0TreeRecoLowEnergy[0], phigh.ThrustAxis, plow.ThrustAxis)
              self.Pi0TreeRecoMass[0] = retparam[0]
              self.Pi0TreeRecoP[0] = retparam[1].Mag()
              self.Pi0TreeRecoLowResolution[0] = retparam[3].Dot(plow.ThrustAxis)
              self.Pi0TreeRecoAltX[0] = altpos.X()
              self.Pi0TreeRecoAltY[0] = altpos.Y()
              self.Pi0TreeRecoAltZ[0] = altpos.Z()
              retparam = self.Pi0MassMom(altpos, phigh.ThrustOrigin, plow.ThrustOrigin, self.Pi0TreeRecoHighEnergy[0], self.Pi0TreeRecoLowEnergy[0], phigh.ThrustAxis, plow.ThrustAxis)
              self.Pi0TreeRecoAltMass[0] = retparam[0]
              self.Pi0TreeRecoAltP[0] = retparam[1].Mag()
              self.Pi0TreeRecoLowAltResolution[0] = retparam[3].Dot(plow.ThrustAxis)
              self.Pi0TreeRecoHighPid[0] = phigh.PID
              self.Pi0TreeRecoLowPid[0] = plow.PID
              self.Pi0TreeRecoPhotonDeltaT[0] = pi0reco.DeltaT
              self.Pi0TreeTrackerMass[0] = self.Pi0TreeTrackerP[0] = self.Pi0TreeTrueTrackerMass[0] = self.Pi0TreeTrueTrackerP[0] = 0.
              self.Pi0TreeGlobalMass[0] = self.Pi0TreeGlobalP[0] = self.Pi0TreeTrueGlobalMass[0] = self.Pi0TreeTrueGlobalP[0] = 0.
              self.Pi0TreeFGDMass[0] = self.Pi0TreeFGDP[0] = self.Pi0TreeTrueFGDMass[0] = self.Pi0TreeTrueFGDP[0] = 0.
              tv = ROOT.TLorentzVector(self.Pi0TreeTrueVertexX[0], self.Pi0TreeTrueVertexY[0], self.Pi0TreeTrueVertexZ[0], self.Pi0TreeTrueVertexT[0])
              rv = pi0reco.Position
              qv = -tv
              qv += rv
              self.Pi0TreeCompareDeltaPos[0] = qv.Vect().Mag()
              self.Pi0TreeCompareDeltaTime[0] = qv.T() - MCTruthRecoOffset
              self.Pi0TreeRecoHighFirstLayer[0] = phigh.UpstreamLayer
              self.Pi0TreeRecoLowFirstLayer[0] = plow.UpstreamLayer
              self.Pi0TreeRecoHighLastLayer[0] = phigh.DownstreamLayer
              self.Pi0TreeRecoLowLastLayer[0] = plow.DownstreamLayer
              self.Pi0TreeRecoHighFirstXBar[0] = phigh.ExtremeMinusXHit
              self.Pi0TreeRecoHighLastXBar[0] = phigh.ExtremePlusXHit
              self.Pi0TreeRecoHighFirstYBar[0] = phigh.ExtremeMinusYHit
              self.Pi0TreeRecoHighLastYBar[0] = phigh.ExtremePlusYHit
              self.Pi0TreeRecoHighBarsToEdge[0] = min(min(phigh.ExtremeMinusXHit, 49-phigh.ExtremePlusXHit), min(phigh.ExtremeMinusYHit, 49-phigh.ExtremePlusYHit))
              self.Pi0TreeRecoLowLastYBar[0] = plow.ExtremePlusYHit
              self.Pi0TreeRecoLowFirstXBar[0] = plow.ExtremeMinusXHit
              self.Pi0TreeRecoLowLastXBar[0] = plow.ExtremePlusXHit
              self.Pi0TreeRecoLowFirstYBar[0] = plow.ExtremeMinusYHit
              self.Pi0TreeRecoLowBarsToEdge[0] = min(min(plow.ExtremeMinusXHit, 49-plow.ExtremePlusXHit), min(plow.ExtremeMinusYHit, 49-plow.ExtremePlusYHit))
              self.Pi0TreeRecoHighDeltaLayer[0] = phigh.DownstreamLayer - phigh.UpstreamLayer
              self.Pi0TreeRecoLowDeltaLayer[0] = plow.DownstreamLayer - plow.UpstreamLayer
              self.Pi0TreeRecoHighModule[0] = phigh.Module
              self.Pi0TreeRecoLowModule[0] = plow.Module
              self.Pi0TreeRecoHighAngularResolution[0] = phigh.VertexResolution
              self.Pi0TreeRecoLowAngularResolution[0] = plow.VertexResolution
              self.Pi0TreeRecoVertexResolutionAlongHigh[0] = pi0reco.ResAlongHigh
              self.Pi0TreeRecoVertexResolutionAlongLow[0] = pi0reco.ResAlongLow
              self.Pi0TreeRecoVertexResolutionAlongThird[0] = pi0reco.ResThird
              if(maxk > -1):
                trackerpos = ROOT.TLorentzVector(self.Pi0TreeHighestMomTrackPosX[0], self.Pi0TreeHighestMomTrackPosY[0], self.Pi0TreeHighestMomTrackPosZ[0], 0.)
                retparam = self.Pi0MassMom(trackerpos, phigh.ThrustOrigin, plow.ThrustOrigin, EMEnCorrection * phigh.Energy, EMEnCorrection * plow.Energy)
                self.Pi0TreeTrackerMass[0] = retparam[0]
                self.Pi0TreeFGDVertexX[0] = self.Pi0TreeFGD1UnusedMinX[0]
                self.Pi0TreeFGDVertexY[0] = self.Pi0TreeFGD1UnusedMinY[0]
                self.Pi0TreeFGDVertexZ[0] = self.Pi0TreeFGD1UnusedMinZ[0]
                self.Pi0TreeTrackerP[0] = retparam[1].Mag()
                self.Pi0TreeRecoHighTrackerResolution[0] = retparam[2].Dot(phigh.ThrustAxis)
                self.Pi0TreeRecoLowTrackerResolution[0] = retparam[3].Dot(plow.ThrustAxis)
                #retparam = self.Pi0MassMom(trackerpos, phigh.ThrustOrigin, plow.ThrustOrigin, self.Pi0TreeTrueHighClusterEnergy[0], self.Pi0TreeTrueLowClusterEnergy[0])
                #self.Pi0TreeTrueTrackerMass[0] = retparam[0]
                #self.Pi0TreeTrueTrackerP[0] = retparam[1]
              if self.Pi0TreeFGD2Unused[0] > 0:
                fgdpos = ROOT.TLorentzVector(self.Pi0TreeFGD2UnusedMinX[0], self.Pi0TreeFGD2UnusedMinY[0], self.Pi0TreeFGD2UnusedMinZ[0], 0.)
                retparam = self.Pi0MassMom(fgdpos, phigh.ThrustOrigin, plow.ThrustOrigin, EMEnCorrection * phigh.Energy, EMEnCorrection * plow.Energy)
                self.Pi0TreeFGDMass[0] = retparam[0]
                self.Pi0TreeFGDVertexX[0] = self.Pi0TreeFGD2UnusedMinX[0]
                self.Pi0TreeFGDVertexY[0] = self.Pi0TreeFGD2UnusedMinY[0]
                self.Pi0TreeFGDVertexZ[0] = self.Pi0TreeFGD2UnusedMinZ[0]
                self.Pi0TreeFGDP[0] = retparam[1].Mag()
                if retparam[1].Mag() > 0.:
                  self.Pi0TreeFGDDir[0] = retparam[1].Z()/retparam[1].Mag()
                else:
                  self.Pi0TreeFGDDir[0] = -2.
                self.Pi0TreeRecoHighFGDResolution[0] = retparam[2].Dot(phigh.ThrustAxis)
                self.Pi0TreeRecoLowFGDResolution[0] = retparam[3].Dot(plow.ThrustAxis)
                dxv = ROOT.TVector3(self.Pi0TreeTrueVertexX[0], self.Pi0TreeTrueVertexY[0], self.Pi0TreeTrueVertexZ[0])
                dxv -= fgdpos.Vect()
                self.Pi0TreeCompareFGDDeltaPos[0] = dxv.Mag()
                #retparam = self.Pi0MassMom(fgdpos, phigh.ThrustOrigin, plow.ThrustOrigin, self.Pi0TreeTrueHighClusterEnergy[0], self.Pi0TreeTrueLowClusterEnergy[0])
                #self.Pi0TreeTrueFGDMass[0] = retparam[0]
                #self.Pi0TreeTrueFGDP[0] = retparam[1]
              elif self.Pi0TreeFGD1Unused[0] > 0:
                fgdpos = ROOT.TLorentzVector(self.Pi0TreeFGD1UnusedMinX[0], self.Pi0TreeFGD1UnusedMinY[0], self.Pi0TreeFGD1UnusedMinZ[0], 0.)
                retparam = self.Pi0MassMom(fgdpos, phigh.ThrustOrigin, plow.ThrustOrigin, EMEnCorrection * phigh.Energy, EMEnCorrection * plow.Energy)
                self.Pi0TreeFGDMass[0] = retparam[0]
                self.Pi0TreeFGDVertexX[0] = self.Pi0TreeFGD1UnusedMinX[0]
                self.Pi0TreeFGDVertexY[0] = self.Pi0TreeFGD1UnusedMinY[0]
                self.Pi0TreeFGDVertexZ[0] = self.Pi0TreeFGD1UnusedMinZ[0]
                self.Pi0TreeFGDP[0] = retparam[1].Mag()
                if retparam[1].Mag() > 0.:
                  self.Pi0TreeFGDDir[0] = retparam[1].Z()/retparam[1].Mag()
                else:
                  self.Pi0TreeFGDDir[0] = -2.
                self.Pi0TreeRecoHighFGDResolution[0] = retparam[2].Dot(phigh.ThrustAxis)
                self.Pi0TreeRecoLowFGDResolution[0] = retparam[3].Dot(plow.ThrustAxis)
                dxv = ROOT.TVector3(self.Pi0TreeTrueVertexX[0], self.Pi0TreeTrueVertexY[0], self.Pi0TreeTrueVertexZ[0])
                dxv -= fgdpos.Vect()
                self.Pi0TreeCompareFGDDeltaPos[0] = dxv.Mag()
                #retparam = self.Pi0MassMom(fgdpos, phigh.ThrustOrigin, plow.ThrustOrigin, self.Pi0TreeTrueHighClusterEnergy[0], self.Pi0TreeTrueLowClusterEnergy[0])
                #self.Pi0TreeTrueFGDMass[0] = retparam[0]
                #self.Pi0TreeTrueFGDP[0] = retparam[1]
              if self.Pi0TreeTrackerNumVertices[0] > 0:
                trackerpos = ROOT.TLorentzVector(self.Pi0TreeGlobalVertexX[0], self.Pi0TreeGlobalVertexY[0], self.Pi0TreeGlobalVertexZ[0], 0.)
                retparam = self.Pi0MassMom(trackerpos, phigh.ThrustOrigin, plow.ThrustOrigin, EMEnCorrection * phigh.Energy, EMEnCorrection * plow.Energy)
                self.Pi0TreeGlobalMass[0] = retparam[0]
                self.Pi0TreeGlobalP[0] = retparam[1].Mag()
                self.Pi0TreeGlobalDir[0] = retparam[1].Z()/retparam[1].Mag()
                self.Pi0TreeRecoHighGlobalResolution[0] = retparam[2].Dot(phigh.ThrustAxis)
                self.Pi0TreeRecoLowGlobalResolution[0] = retparam[3].Dot(plow.ThrustAxis)
                dxv = ROOT.TVector3(self.Pi0TreeTrueVertexX[0], self.Pi0TreeTrueVertexY[0], self.Pi0TreeTrueVertexZ[0])
                dxv -= trackerpos.Vect()
                self.Pi0TreeCompareGlobalDeltaPos[0] = dxv.Mag()
                #retparam = self.Pi0MassMom(trackerpos, phigh.ThrustOrigin, plow.ThrustOrigin, self.Pi0TreeTrueHighClusterEnergy[0], self.Pi0TreeTrueLowClusterEnergy[0])
                #self.Pi0TreeTrueTrackerVMass[0] = retparam[0]
                #self.Pi0TreeTrueTrackerVP[0] = retparam[1]
              self.Pi0TreeRecoHighTime[0] = phigh.Position.T() - deltatMC - (self.GlobalBunchN[0]-1)*582.
              self.Pi0TreeRecoHighBunchSigma[0] = GetBunchSigma(phigh.Position.T(), self.GlobalBeamRun[0], self.GlobalBunchN[0])
              self.Pi0TreeRecoLowTime[0] = plow.Position.T() - deltatMC - (self.GlobalBunchN[0]-1)*582.
              self.Pi0TreeRecoLowBunchSigma[0] = GetBunchSigma(plow.Position.T(), self.GlobalBeamRun[0], self.GlobalBunchN[0])
   
              self.Pi0TreeRecoHighPosX[0] = phigh.ThrustOrigin.X()
              self.Pi0TreeRecoHighPosY[0] = phigh.ThrustOrigin.Y()
              self.Pi0TreeRecoHighPosZ[0] = phigh.ThrustOrigin.Z()
              self.Pi0TreeRecoLowPosX[0] = plow.ThrustOrigin.X()
              self.Pi0TreeRecoLowPosY[0] = plow.ThrustOrigin.Y()
              self.Pi0TreeRecoLowPosZ[0] = plow.ThrustOrigin.Z()

              constrainedpos = self.GetConstrainedVertex(phigh.ThrustOrigin, plow.ThrustOrigin, phigh.ThrustAxis, plow.ThrustAxis)
              self.Pi0TreeConstrainedVertexX[0] = constrainedpos.X()
              self.Pi0TreeConstrainedVertexY[0] = constrainedpos.Y()
              self.Pi0TreeConstrainedVertexZ[0] = constrainedpos.Z()
              dx = constrainedpos.Vect()
              dx -= pi0reco.Position.Vect()
              self.Pi0TreeConstrainedDeltaDist[0] = dx.Mag()
              retparam = self.Pi0MassMom(constrainedpos, phigh.ThrustOrigin, plow.ThrustOrigin, EMEnCorrection * phigh.Energy, EMEnCorrection * plow.Energy)
              self.Pi0TreeConstrainedMass[0] = retparam[0]
              self.Pi0TreeConstrainedP[0] = retparam[1].Mag()
              if retparam[1].Mag() > 0.:
                self.Pi0TreeConstrainedDir[0] = retparam[1].Z()/retparam[1].Mag()
              else:
                self.Pi0TreeConstrainedDir[0] = -2.
              self.Pi0TreeRecoHighConstrainedResolution[0] = retparam[2].Dot(phigh.ThrustAxis)
              self.Pi0TreeRecoLowConstrainedResolution[0] = retparam[3].Dot(plow.ThrustAxis)

              retparam = self.GetSkew(phigh.ThrustOrigin, phigh.ThrustAxis, plow.ThrustOrigin, plow.ThrustAxis)
              self.Pi0TreeSkewVertexX[0] = retparam[0].X()
              self.Pi0TreeSkewVertexY[0] = retparam[0].Y()
              self.Pi0TreeSkewVertexZ[0] = retparam[0].Z()
              self.Pi0TreeSkewDistance[0] = retparam[1]
              retparam = self.Pi0MassMom(retparam[0], phigh.ThrustOrigin, plow.ThrustOrigin, EMEnCorrection * phigh.Energy, EMEnCorrection * plow.Energy, phigh.ThrustAxis, plow.ThrustAxis)
              self.Pi0TreeSkewMass[0] = retparam[0]
              self.Pi0TreeSkewP[0] = retparam[1].Mag()
              self.Pi0TreeRecoHighSkewResolution[0] = retparam[2].Dot(phigh.ThrustAxis)
              self.Pi0TreeRecoLowSkewResolution[0] = retparam[3].Dot(plow.ThrustAxis)
    


              self.Pi0TreeRecoHighThrust[0] = phigh.Thrust
              self.Pi0TreeRecoLowThrust[0] = plow.Thrust
              
              self.Pi0TreeRecoHighIncidenceAngle[0] = math.acos(phigh.ThrustAxis.Z())
              self.Pi0TreeRecoLowIncidenceAngle[0] = math.acos(plow.ThrustAxis.Z())
              
              
              self.Pi0TreeRecoHighDirectionX[0] = phigh.ThrustAxis.X()
              self.Pi0TreeRecoHighDirectionY[0] = phigh.ThrustAxis.Y()
              self.Pi0TreeRecoHighDirectionZ[0] = phigh.ThrustAxis.Z()
              self.Pi0TreeRecoLowDirectionX[0] = plow.ThrustAxis.X()
              self.Pi0TreeRecoLowDirectionY[0] = plow.ThrustAxis.Y()
              self.Pi0TreeRecoLowDirectionZ[0] = plow.ThrustAxis.Z()

              self.Pi0TreeRecoHighPointingX[0] = phigh.Pointing.X()
              self.Pi0TreeRecoHighPointingY[0] = phigh.Pointing.Y()
              self.Pi0TreeRecoHighPointingZ[0] = phigh.Pointing.Z()
              self.Pi0TreeRecoLowPointingX[0] = plow.Pointing.X()
              self.Pi0TreeRecoLowPointingY[0] = plow.Pointing.Y()
              self.Pi0TreeRecoLowPointingZ[0] = plow.Pointing.Z()

              ret = self.GetDistanceFromFGD(phigh.ThrustOrigin, phigh.ThrustAxis, FGD1MZ, FGD1PZ)
              self.Pi0TreeRecoHighXDistanceFromFGD1[0] = ret[0]
              self.Pi0TreeRecoHighYDistanceFromFGD1[0] = ret[1]
              ret = self.GetDistanceFromFGD(phigh.ThrustOrigin, phigh.ThrustAxis, FGD2MZ, FGD2PZ)
              self.Pi0TreeRecoHighXDistanceFromFGD2[0] = ret[0]
              self.Pi0TreeRecoHighYDistanceFromFGD2[0] = ret[1]
              ret = self.GetDistanceFromFGD(plow.ThrustOrigin, plow.ThrustAxis, FGD1MZ, FGD1PZ)
              self.Pi0TreeRecoLowXDistanceFromFGD1[0] = ret[0]
              self.Pi0TreeRecoLowYDistanceFromFGD1[0] = ret[1]
              ret = self.GetDistanceFromFGD(plow.ThrustOrigin, plow.ThrustAxis, FGD2MZ, FGD2PZ)
              self.Pi0TreeRecoLowXDistanceFromFGD2[0] = ret[0]
              self.Pi0TreeRecoLowYDistanceFromFGD2[0] = ret[1]

              dist = phigh.ThrustOrigin - plow.ThrustOrigin
              self.Pi0TreeRecoPhotonDeltaX[0] = abs(dist.X())
              self.Pi0TreeRecoPhotonDeltaY[0] = abs(dist.Y())
              self.Pi0TreeRecoPhotonDeltaZ[0] = abs(dist.Z())
              self.Pi0TreeRecoPhotonDeltaDist[0] = dist.Mag()

              eh = self.Pi0TreeRecoHighEnergy[0]
              el = self.Pi0TreeRecoLowEnergy[0]

              dxh = self.Pi0TreeRecoHighDirectionX[0]
              dxl = self.Pi0TreeRecoLowDirectionX[0]
              dyh = self.Pi0TreeRecoHighDirectionY[0]
              dyl = self.Pi0TreeRecoLowDirectionY[0]
              dzh = self.Pi0TreeRecoHighDirectionZ[0]
              dzl = self.Pi0TreeRecoLowDirectionZ[0]

              asym = (eh - el)/(eh + el)
              costh = dxh*dxl + dyh*dyl + dzh*dzl
              if costh > 1.:
                print "costh > 1.: ", costh
                costh = 1.;


              # "hPid", "lPid", "hQskew", "lQskew", "hEMlik", "lEMlik", "hPoint", "lPoint", "hFL", "lFL", "hMT", "lMT", "hHad", "lHad", "hAngle", "lAngle", "hConeA", "lConeA", "hAMR", "lAMR", "hShowW", "lShowW", "hMeanP", "lMeanP", "hNH", "lNH", "hFid", "lFid", "hThrust", "lThrust", "Asymm", "CosTh", "ClusM", "DeltaD", "RecoX", "RecoY", "RecoZ"
              inputValues = [ self.Pi0TreeRecoHighPid[0], self.Pi0TreeRecoLowPid[0], self.Pi0TreeRecoHighQSkew[0], 
                  self.Pi0TreeRecoLowQSkew[0], self.Pi0TreeRecoHighEMLikelihood[0], self.Pi0TreeRecoLowEMLikelihood[0],
                  self.Pi0TreeRecoHighPointingZ[0], #'''self.Pi0TreeRecoLowPointingZ[0],'''
                  self.Pi0TreeRecoHighFirstLayer[0],
                  self.Pi0TreeRecoLowFirstLayer[0], self.Pi0TreeRecoHighMichelTags[0], # self.Pi0TreeRecoLowMichelTags[0],
                  self.Pi0TreeRecoHighIncidenceAngle[0], self.Pi0TreeRecoLowIncidenceAngle[0], self.Pi0TreeRecoHighConeAngle[0],
                  self.Pi0TreeRecoLowConeAngle[0], #'''self.Pi0TreeRecoHighAMR[0], self.Pi0TreeRecoLowAMR[0],'''
                  #self.Pi0TreeRecoHighShowerWidth[0], self.Pi0TreeRecoLowShowerWidth[0], 
                  self.Pi0TreeRecoHighMeanPosition[0],
                  self.Pi0TreeRecoLowMeanPosition[0], self.Pi0TreeRecoHighNumHits[0], self.Pi0TreeRecoLowNumHits[0],
                  self.Pi0TreeRecoHighBarsToEdge[0], self.Pi0TreeRecoLowBarsToEdge[0], self.Pi0TreeRecoHighThrust[0], self.Pi0TreeRecoLowThrust[0],
                  asym, costh, self.Pi0TreeRecoPhotonInvariantMass[0], self.Pi0TreeRecoPhotonDeltaDist[0] #''', pos.X(), pos.Y(), pos.Z()''' ]
                  ]


              v = ROOT.std.vector('double')()
              for f in inputValues:
                v.push_back(1.*f)

              self.Pi0TreeTMVALikelihood[0] = self.TMVALikelihood.GetMvaValue(v)

              if phigh == plater:
                for var in ["Energy", "PosX", "PosY", "PosZ", "Time", "DirectionX", "DirectionY", "DirectionZ", "IncidenceAngle", "Pid", "Had", "NumHits", "FirstLayer", "LastLayer", "FirstXBar", "LastXBar", "FirstYBar", "LastYBar", "BarsToEdge", "DeltaLayer", "NHitLayer", "MichelTags", "ConeAngle", "AMR", "Asymmetry", "MeanPosition", "QSkew", "ShowerWidth", "EMLikelihood", "BunchSigma", "MatchingLikelihood", "PointingX", "PointingY", "PointingZ", "ThrustDotPCA"]:
                  self.__dict__["Pi0TreeRecoLater" + var][0] = self.__dict__["Pi0TreeRecoHigh" + var][0]
                  self.__dict__["Pi0TreeRecoEarlier" + var][0] = self.__dict__["Pi0TreeRecoLow" + var][0]
              else:
                for var in ["Energy", "PosX", "PosY", "PosZ", "Time", "DirectionX", "DirectionY", "DirectionZ", "IncidenceAngle", "Pid", "Had", "NumHits", "FirstLayer", "LastLayer", "FirstXBar", "LastXBar", "FirstYBar", "LastYBar", "BarsToEdge", "DeltaLayer", "NHitLayer", "MichelTags", "ConeAngle", "AMR", "Asymmetry", "MeanPosition", "QSkew", "ShowerWidth", "EMLikelihood", "BunchSigma", "MatchingLikelihood", "PointingX", "PointingY", "PointingZ", "ThrustDotPCA"]:
                  self.__dict__["Pi0TreeRecoLater" + var][0] = self.__dict__["Pi0TreeRecoLow" + var][0]
                  self.__dict__["Pi0TreeRecoEarlier" + var][0] = self.__dict__["Pi0TreeRecoHigh" + var][0]
                


              self.rootTrees["Pi0Tree"].Fill()

              hasPassedCut["Pi0 Recon"] = True
        
        if isMC:
          whichVol = self.PerformMCTasks(0)
          if whichVol == 1:
            pi0InFGD1 = True
          elif whichVol == 10:
            pi0InFGD2 = True
          elif whichVol == 100:
            pi0InTPC1 = True
          elif whichVol == 1000:
            pi0InTPC2 = True
          elif whichVol == 10000:
            pi0InTPC3 = True
        
        # Update counters
        for step in self.listOfAnalysisSteps:
            if hasPassedCut[step]:
                self.IncrementCutCounter("Signal & Background", step)
                if pi0InFGD1:
                    self.IncrementCutCounter("True NCPi0 Events in FGD1", step)
                if pi0InFGD2:
                    self.IncrementCutCounter("True NCPi0 Events in FGD2", step)
                if pi0InTPC1:
                    self.IncrementCutCounter("True NCPi0 Events in TPC1", step)
                if pi0InTPC2:
                    self.IncrementCutCounter("True NCPi0 Events in TPC2", step)
                if pi0InTPC3:
                    self.IncrementCutCounter("True NCPi0 Events in TPC3", step)

