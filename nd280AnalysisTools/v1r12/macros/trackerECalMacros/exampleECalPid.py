#_________________________________________________________________________
#
# An example script showing how to use the tracker ecal pid tools.
# This example is based on "exampleAnalysisScript.py" in the parent directory.
#
# d.r.hadley@warwick.ac.uk 30/05/2012 
#_________________________________________________________________________

# import the ROOT module
import ROOT
import numpy
# import the userAnalysisBase class from userAnalysisBase.py
from userAnalysisBase import userAnalysisBase 

# class name must be the same as the filename without the .py
class exampleECalPid(userAnalysisBase): # inherits from userAnalysisBase
    """\
exampleAnalysisScript.py
Example analysis macro with the same functionality as the .C equivalent.
Derives from userAnalysisBase, and is called from runAnalyses.py
    """
    # _________________________________________________________________________
    # __init__ gets called automatically upon instantiation
    def __init__(self, options):
        """\
Specifies further trees to be added to oaAnalysisModuleTChains.
        """
        # Call __init__ in base class
        userAnalysisBase.__init__(self, options)
        # Make TChains of the oaAnalysis trees to be read, and append to i
        # oaAnalysisModuleTChains
        self.TrackerECal = ROOT.TChain("ReconDir/TrackerECal") 
        self.oaAnalysisModuleTChains.append(self.TrackerECal)
 
    # _________________________________________________________________________
    def BookHistogramsAndAnalysisSteps(self):
        # Define variables for output tree. As pointer don't exist in python we 
        # have to declare an array and then pass this as the branch address as 
        # ROOT expects a pointer.
        self.mipEm = numpy.array([0.0])
        self.mipPion = numpy.array([0.0])
        self.emHip = numpy.array([0.0])
        # Create tree and set branch addresses.
        self.outputTree = ROOT.TTree("TrackerECalPidOutput","TrackerECalPidOutput");
        self.outputTree.Branch("mipEm", self.mipEm,"mipEm/D");
        self.outputTree.Branch("mipPion", self.mipPion,"mipPion/D");
        self.outputTree.Branch("emHip", self.emHip,"emHip/D");
        
        #Setup the PID calculator
        self.pidCalculator = ROOT.ecalPid.TECALPidLikelihood()
        self.pidCalculator.SetupAlgorithms()
        

    # _________________________________________________________________________
    # EventAnalysis gets called for each event
    def EventAnalysis(self):
        pidCalculator = self.pidCalculator
        for ecalObject in self.TrackerECal.ReconObject:
            pidCalculator.CalculateLikelihoodValues(ecalObject)
            self.mipEm[0] = pidCalculator.GetLikelihoodValue("MipEm")
            self.mipPion[0] = pidCalculator.GetLikelihoodValue("MipPion")
            self.emHip[0] = pidCalculator.GetLikelihoodValue("EmHip")
            # Fill the tree.
            self.outputTree.Fill()         

    # _________________________________________________________________________
    def WriteResultsToOutputFile(self):
        # Write out analysis results
        if self.rootOutput:
            print self.rootOutput.GetName()
            self.rootOutput.cd()
            self.outputTree.Write()
            print "Found output file. Tree has been saved!"
        else:
            print "Unable to find output file. Tree not saved!"
            
