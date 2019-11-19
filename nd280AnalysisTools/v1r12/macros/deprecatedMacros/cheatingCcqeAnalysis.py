import sys
import optparse
import os
import math # cos, log, pi etc
import logging # print with logging.debug/info/warning/error/critical('message')

import ROOT
# import the userAnalysisBase class from userAnalysisBase.py
from userAnalysisBase import userAnalysisBase 

# class name must be the same as the filename without the .py
class cheatingCcqeAnalysis(userAnalysisBase): # inherits from userAnalysisBase
    """\
cheatingCcqeAnalysis.py
This is performing the same analysis as the ccqeAnalysis.py but instead
of using reconstructed tracks it uses only truth information with expected smearing.
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
        self.truthTrajectories = ROOT.TChain("TruthDir/Trajectories"); self.oaAnalysisModuleTChains.append(self.truthTrajectories)
        # Tracker Reconstruction
        # self.reconTracker = ROOT.TChain("ReconDir/Tracker"); self.oaAnalysisModuleTChains.append(self.reconTracker)
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
        self.cheatingAnalysisUtil = ROOT.TCheatingAnalysisUtils()
        #self.oaAnalysisUtil = ROOT.ToaAnalysisUtils()
        # defined fiducial within active FGD volume:
        self.fgdFiducialTrim = ROOT.TVector3(200.0,200.0,10.0) 

    def TrajectoriesOfType(self, typeNumber):
        ####################################################################
        # Returns trajectories of type number typeNumber
        trajectories = self.truthTrajectories.TrajsLepton          
        if typeNumber == 0:
            trajectories = self.truthTrajectories.TrajsLepton
        elif typeNumber == 1:
            trajectories = self.truthTrajectories.TrajsBaryon
        elif typeNumber == 2:
            trajectories = self.truthTrajectories.TrajsMeson
        elif typeNumber == 3:
            trajectories = self.truthTrajectories.TrajsPhoton
        elif typeNumber == 4:
            trajectories = self.truthTrajectories.TrajsOtherCharged
        elif typeNumber == 5:
            trajectories = self.truthTrajectories.TrajsOtherNeutral
        elif typeNumber == 6:
            trajectories = self.truthTrajectories.TrajsOther
        return trajectories

    def NumberTrajectoriesOfType(self, typeNumber):
        ####################################################################
        # Returns number trajectories of type number typeNumber
        nTrajs = 0          
        if typeNumber == 0:
            nTrajs = self.truthTrajectories.NTrajsLepton
        elif typeNumber == 1:
            nTrajs = self.truthTrajectories.NTrajsBaryon
        elif typeNumber == 2:
            nTrajs = self.truthTrajectories.NTrajsMeson
        elif typeNumber == 3:
            nTrajs = self.truthTrajectories.NTrajsPhoton
        elif typeNumber == 4:
            nTrajs = self.truthTrajectories.NTrajsOtherCharged
        elif typeNumber == 5:
            nTrajs = self.truthTrajectories.NTrajsOtherNeutral
        elif typeNumber == 6:
            nTrajs = self.truthTrajectories.NTrajsOther
        return nTrajs
    


    def EventAnalysis(self):
        ################################################################
        # abbreviations for use within this function
        truthVertices = self.truthVertices 
        truthTrajectories = self.truthTrajectories 
        fgdFiducialTrim = self.fgdFiducialTrim
        generatorUtils = self.generatorUtils

        ################################################################
        # analysis utilities
        trackerAnalysisUtil = self.trackerAnalysisUtil
        cheatingAnalysisUtil = self.cheatingAnalysisUtil

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
                    
                    
                # end if
            # end for
        # end if    

        ################################
        # Apply event selection criteria

        #Loop over all trajectories of the event, but only for this vertex, to see how many of them exist in a TPC
        usefulTPCTrajs = []
        #We want here to loop over AllTrajectories, but until the problem with TRefArray is solved
        nTrajs = 0
        trajectories = truthTrajectories.TrajsLepton          
        for trajType in xrange(7):
            trajectories = self.TrajectoriesOfType(trajType)
            nTrajs = self.NumberTrajectoriesOfType(trajType)
            #print "Trajectory type is ", trajType, " and will loop over ", nTrajs, " number trajectories"
            for j in xrange(nTrajs):
                trajectory = trajectories[j] # a TTruthTrajectoriesModule.TTruthTrajectory
                #Check that this trajectory comes from the current chosen vertex 
                if ((trajectory.TrajID >= true_fgd_vertex.MinTrackID) and (trajectory.TrajID <= true_fgd_vertex.MaxTrackID)):
                    #Check that cheating info is set
                    if (not trajectory.CheatingInfoIsSet ): print "ERROR, cheating info not set"

                    #Find out if this trajectory is long enough in one or many TPC(s)
                    if (cheatingAnalysisUtil.IsAUsefulTPCTrack(trajectory)):
                        usefulTPCTrajs.append(trajectory)
                    
                    #end if: trajectory in a tpc
                #end if: check if trajectory is from the current vertex    
            #end loop: ovar all trajectories of this type
        #end loop: over all trajectory types

        #Assume that we can reconstruct the vertex and that it is the
        #first point of the first useful TPC track
        vertexPos = ROOT.TLorentzVector(0,0,0,0);
        if len(usefulTPCTrajs) >= 1:
            vertexPos = usefulTPCTrajs[0].InitPos 
        
        # One track analysis
        if len(usefulTPCTrajs) == 1:
            # Step 1 . Check vertex is in fiducial - use the useful TPC trajectory
            # Assume that first point of this useful trajectory is reconstructed and is the initial vertex
            if trackerAnalysisUtil.IsInFiducialFGD1(vertexPos.Vect(), fgdFiducialTrim) \
            or trackerAnalysisUtil.IsInFiducialFGD2(vertexPos.Vect(), fgdFiducialTrim):
                hasPassedCut["Vertex"] = True
                # Step 2 . Check that we have exactly one negative track.
                # Since this trajectory is "useful" it was possible to detect its charge
                if usefulTPCTrajs[0].Charge == -3:   #unit charge in root
                    hasPassedCut["Negative TPC Track"] = True
        
        # Two track analysis
        if len(usefulTPCTrajs) == 2:
            # Need to be exactly one measured vertex - for cheating analysis check if
            # the two "useful TPC trajectories" starts in same point   --- would be good to use unit
            if (vertexPos - usefulTPCTrajs[1].InitPos).Mag() < 0.1:
               # check that vertex position is inside one of the two FGDs.
                if trackerAnalysisUtil.IsInFiducialFGD1(vertexPos.Vect(), fgdFiducialTrim) \
                or trackerAnalysisUtil.IsInFiducialFGD2(vertexPos.Vect(), fgdFiducialTrim):
                    hasPassedCut["Vertex"] = True
                    
                    # Step 2 . Check that we have exactly one negative track.
                    # Since these trajectories are "useful" it is possible to detect their charge
                    number_negative_tpc_tracks = 0
                    if usefulTPCTrajs[0].Charge  == -3:  #e- unit charge = -3 in root
                        number_negative_tpc_tracks += 1;
                    if usefulTPCTrajs[1].Charge == -3:   #e- unit charge = -3 in root
                        number_negative_tpc_tracks += 1;

                    if number_negative_tpc_tracks == 1:
                        hasPassedCut["Negative TPC Track"] = True
                            
            # End step 2, two track analysis.
        # End step 1, two track analysis.
        
        if hasPassedCut["Vertex"] and hasPassedCut["Negative TPC Track"]:
            # Check if there are any delayed  FGD clustered
            # near the vertex point.  If so, then assume from
            # pi+ . mu+ . e+ and hence reject
            foundMichelCluster = False

            #Loop over all trajectories of the event and of the vertex to see if they are Michel electron/positron candidates
            michEls = [];
            #We want here to loop over AllTrajectories, but until the problem with TRefArray is solved
            #we will loop over the trajectory types one by one
            nTrajs2 = 0
            trajectories2 = truthTrajectories.TrajsLepton          
            for trajType2 in xrange(7):
                trajectories2 = self.TrajectoriesOfType(trajType)
                nTrajs2 = self.NumberTrajectoriesOfType(trajType)
                for j in xrange(nTrajs2):
                    trajectory = trajectories2[j] # a TTruthTrajectoriesModule.TTruthTrajectory     
                    #Check that this trajectory comes from the current chosen vertex 
                    if ((trajectory.TrajID >= true_fgd_vertex.MinTrackID) and (trajectory.TrajID <= true_fgd_vertex.MaxTrackID)):
                        #Find out if this trajectory could be a Michel electron/positron candidate
                        if (cheatingAnalysisUtil.IsMichelElectronCandidate(trajectory, vertexPos)):
                            michEls.append(trajectory)

            #end for loop types

            if len(michEls) == 0:
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
