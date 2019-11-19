import sys
import optparse
import os
import math # cos, log, pi etc
import logging # print with logging.debug/info/warning/error/critical('message')

import ROOT
# import the userAnalysisBase class from userAnalysisBase.py
from userAnalysisBase import userAnalysisBase 

# class name must be the same as the filename without the .py
class ccqeAnalysis(userAnalysisBase): # inherits from userAnalysisBase
    """\
ccqeAnalysis.py
This is a python version of Thomas Lindner et al's CCQE analysis written in 
ROOT C++, but has not been validated yet.
Derives from userAnalysisBase, and is called from runAnalyses.py
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
        self.truthVertices = ROOT.TChain("TruthDir/Vertices"); self.oaAnalysisModuleTChains.append(self.truthVertices)
        # Truth information about the trajectories
        # self.truthTrajectories = ROOT.TChain("TruthDir/Trajectories"); self.oaAnalysisModuleTChains.append(self.truthTrajectories)
        # Tracker Reconstruction
        self.reconTracker = ROOT.TChain("ReconDir/Tracker"); self.oaAnalysisModuleTChains.append(self.reconTracker)
        # TrackerECal Reconstruction
        # self.reconTrackerECal = ROOT.TChain("ReconDir/TrackerECal"); self.oaAnalysisModuleTChains.append(self.reconTrackerECal)
        # FGDIso Reconstruction
        # self.reconFGDIso = ROOT.TChain("ReconDir/FGDIso"); self.oaAnalysisModuleTChains.append(self.reconFGDIso)
        # P0D Reconstruction
        # self.reconP0D = ROOT.TChain("ReconDir/P0D"); self.oaAnalysisModuleTChains.append(self.reconP0D)

    def BookHistogramsAndAnalysisSteps(self):
        ####################################################################
        # Define analysis steps
        self.listOfAnalysisSteps.extend( # "No Cuts" is already defined
                ["Vertex", "Negative TPC Track", "FGD Pion"])
        
        ####################################################################
        # Define some histograms
        self.hNeutrinoEnergy = ROOT.TH1F("nu_energy","Neutrino Energy Spectrum",100,0,3000)
        self.hNeutrinoEnergy.SetXTitle("Energy (MeV)")
        self.hNeutrinoCCQEEnergy = ROOT.TH1F("nu_energy_ccqe","Neutrino CCQE Energy Spectrum",100,0,3000)
        self.hNeutrinoCCQEEnergy.SetXTitle("Energy (MeV)")
        
        self.hCCQEEffVNeutrinoEnergy = ROOT.TProfile("eff_vs_nu_energy","CCQE Efficiency vs Neutrino Energy",100,0,3000)
        self.hCCQEEffVNeutrinoEnergy.SetXTitle("Energy (MeV)")
        self.hCCQEEffVNeutrinoEnergy.SetYTitle("CCQE Efficiency")
        
        self.hMuonCCQEAngle = ROOT.TH1F("muon_ccqe_angle","Muon CCQE Angle",60,0,180)
        self.hMuonCCQEAngle.SetXTitle("Muon Angle (degrees)")
        
        self.hCCQEEffVsMuonAngle = ROOT.TProfile("eff_vs_muon_angle","CCQE Efficiency vs Muon Angle",60,0,180)
        self.hCCQEEffVsMuonAngle.SetXTitle("Muon Angle (degrees)")
        self.hCCQEEffVsMuonAngle.SetYTitle("CCQE Efficiency")

        self.listOfHistosAndOtherObjects = [self.hNeutrinoEnergy,
                self.hNeutrinoCCQEEnergy,
                self.hCCQEEffVNeutrinoEnergy,
                self.hMuonCCQEAngle,
                self.hCCQEEffVsMuonAngle]

        # Declare cut counters, which are accessed using their names
        self.RegisterCutCounter("True CCQE Events in Fiducial")


        # Define other quantities
        self.trackerAnalysisUtil = ROOT.TTrackerAnalysisUtils()
        # defined fiducial within active FGD volume:
        self.fgdFiducialTrim = ROOT.TVector3(200.0,200.0,10.0) 

    def EventAnalysis(self):
        ################################################################
        # abbreviations for use within this function
        truthVertices = self.truthVertices 
        reconTracker = self.reconTracker 
        fgdFiducialTrim = self.fgdFiducialTrim
        generatorUtils = self.generatorUtils

        ################################################################
        # analysis utilities
        trackerAnalysisUtil = self.trackerAnalysisUtil

        ################################################################
        # Initialise event-level quantities
        #self.ResetCutCounter(
        hasPassedCut = {}
        for step in self.listOfAnalysisSteps:
            hasPassedCut[step] = False
        hasPassedCut["No Cuts"] = True
        inFiducial = False
        ccqeInFiducial = False

        # Check vertex truth information
        if truthVertices.NVtxFGD1 >0:
            true_fgd_vertex = \
                    truthVertices.VtxFGD1[0] # a TTruthVerticesModule.TTruthVertex
            trueFiducialFGD1 = True
        elif truthVertices.NVtxFGD2 >0:
            true_fgd_vertex = \
                    truthVertices.VtxFGD2[0] # a TTruthVerticesModule.TTruthVertex
            trueFiducialFGD2 = True
        else:
            print "There should be at least one vertex in one FGD.  Skipping event."
            return False
        
        neutrinoMomentum = true_fgd_vertex.MomNu
        vtxPosition = ROOT.TVector3(true_fgd_vertex.Vertex.X(),
        true_fgd_vertex.Vertex.Y(),
        true_fgd_vertex.Vertex.Z())
        
        if trackerAnalysisUtil.IsInFiducialFGD1(vtxPosition, fgdFiducialTrim) \
            or trackerAnalysisUtil.IsInFiducialFGD2(vtxPosition, fgdFiducialTrim):
            inFiducial = True
            ccqeInFiducial = generatorUtils.IsCCQE(true_fgd_vertex)

        # Find the true muon track
        if ccqeInFiducial:
            ccqeInFiducial = False # will set to True again when true outgoing muon is found
            for ll in xrange(true_fgd_vertex.LeptonIDs.size()):
                if true_fgd_vertex.LeptonPDGs[ll] == 13:
                    muonMomentum = true_fgd_vertex.LeptonMoms[ll]
                    ccqeInFiducial = True
        
        # Apply event selection criteria
        total_tracks = reconTracker.NTracks
        total_tpc = 0
        for j in xrange(total_tracks):
            track = reconTracker.Tracks[j] # a TTrackerReconModule.TTrackerTrack
            hasTPC = False
            for tt in xrange(3):
                if track.tpc_charge[tt] != 0.0:
                    hasTPC = True
            if hasTPC:
                total_tpc += 1
        
        # One track analysis
        if total_tracks == 1 and total_tpc == 1:
            # Step 1 . Check vertex is in fiducial
            track = reconTracker.Tracks[0] # a TTrackerReconModule.TTrackerTrack
            # Assume that upstream end of track is initial vertex.
            recon_vertex_pos = track.FrontPosition
            trackFrontPos = ROOT.TVector3(recon_vertex_pos.X(),recon_vertex_pos.Y(),recon_vertex_pos.Z())
            
            if trackerAnalysisUtil.IsInFiducialFGD1(trackFrontPos, fgdFiducialTrim) \
            or trackerAnalysisUtil.IsInFiducialFGD2(trackFrontPos, fgdFiducialTrim):
                hasPassedCut["Vertex"] = True
            
            # Step 2 . Check that we have exactly one negative track.
            if trackerAnalysisUtil.GetTrackTpcCharge(track) == -1.0:
                hasPassedCut["Negative TPC Track"] = True
                muonTrack = track
        
        # Two track analysis
        if total_tracks == 2 and total_tpc > 0:
            # Need to be exactly one measured vertex.
            if reconTracker.NVertices == 1:
                vertex = reconTracker.Vertices[0] # a TTrackerReconModule.TTrackerVertex
                recon_vertex_pos  = vertex.Position
                tmp_position = ROOT.TVector3(recon_vertex_pos.X(),recon_vertex_pos.Y(),recon_vertex_pos.Z())
                
                # check that vertex position is inside one of the two FGDs.
                if trackerAnalysisUtil.IsInFiducialFGD1(tmp_position, fgdFiducialTrim) \
              or trackerAnalysisUtil.IsInFiducialFGD2(tmp_position, fgdFiducialTrim):
                    hasPassedCut["Vertex"] = True
                    
                    # Step 2 . Check that we have exactly one negative track.
                    
                    number_negative_tpc_tracks = 0
                    for j in xrange(reconTracker.NTracks):
                        track = reconTracker.Tracks[j] # TTrackerReconModule.TTrackerTrack
                        
                        # Get reconstructed charge (which assumes forward going track)
                        charge = trackerAnalysisUtil.GetTrackTpcCharge(track)
                        
                        # Check if track was actually backward-going.  If so, flip charge.
                        delta_z_front = recon_vertex_pos.Z() - track.FrontPosition.Z()
                        
                        if delta_z_front > 100.0:
                            charge = -1*charge
                        
                        # Now add up number of negative tracks.
                        if charge < 0:
                            number_negative_tpc_tracks += 1
                            muonTrack = track
                        elif  charge > 0:
                            protonTrack = track
                    if number_negative_tpc_tracks == 1:
                        hasPassedCut["Negative TPC Track"] = True
        # End step 2, two track analysis.
        # End step 1, two track analysis.
        
        if hasPassedCut["Vertex"] and hasPassedCut["Negative TPC Track"]:
            # check if there are any delayed  FGD clustered
            # near the vertex point.  If so, then assume from
            # pi+ . mu+ . e+ and hence reject
            foundMichelCluster = False
            for j in xrange(reconTracker.NDelayedClusters):
                cluster = reconTracker.DelayedClusters[j] # a TTrackerReconModule.TFgdCluster
                dp = cluster.Position - recon_vertex_pos
                # find distance between vertex position and hit position.
                dist = 9999.0
                if cluster.Variance.X() == 0.0:
                    dist = ROOT.TMath.Sqrt(dp.X()*dp.X() + dp.Z()*dp.Z())
                else:
                    dist = ROOT.TMath.Sqrt(dp.Y()*dp.Y() + dp.Z()*dp.Z())
                
                if dist < 500.0:
                    foundMichelCluster = True
            
            if not foundMichelCluster:
                hasPassedCut["FGD Pion"] = True
        
        # End step 3.
        
        # Update counters
        for step in self.listOfAnalysisSteps:
            if hasPassedCut[step]:
                self.IncrementCutCounter("Signal & Background", step)
                if ccqeInFiducial:
                    self.IncrementCutCounter("True CCQE Events in Fiducial", step)

        # Fill quantities as necessary
        if inFiducial:
            self.hNeutrinoEnergy.Fill(neutrinoMomentum.P())
        
        if ccqeInFiducial:
            self.hNeutrinoCCQEEnergy.Fill(neutrinoMomentum.P())
            dir = ROOT.TVector3(muonMomentum.X(), muonMomentum.Y(), muonMomentum.Z())
            self.hMuonCCQEAngle.Fill(dir.Theta()*180.0/math.pi)
            if hasPassedCut["FGD Pion"]:
                self.hCCQEEffVNeutrinoEnergy.Fill(neutrinoMomentum.P(), 1.0)
                self.hCCQEEffVsMuonAngle.Fill(dir.Theta()*180.0/math.pi, 1.0)
            else:
                self.hCCQEEffVNeutrinoEnergy.Fill(neutrinoMomentum.P(), 0.00001)
                self.hCCQEEffVsMuonAngle.Fill(dir.Theta()*180.0/math.pi, 0.0001)

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
