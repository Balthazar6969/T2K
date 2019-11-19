#_________________________________________________________________________
#
# This is an example python analysis macro to demonstrate how to loop over an 
# oaAnalysis output file and access the TClonesArrays of analysis summary 
# objects stored in the various output trees. It also shows how to call
# an analysis utility from the AnalysisUtils directory. It has the same
# functionality as its .C equivalent except that in this macro a simple 
# output tree is saved as this is a bit tricky in python. 
#
# It is important to note that neither this script or its C equivalent
# link to any of the nd280 softwarei. This includes anything in the 
# oaAnalysis src directory (with the possible exception of oaAnalysisUtilsModule). 
# This is important as to run the macros you should not need to compile oaAnalysis 
# and should only require an input MC analysis file and the version of ROOT you 
# installed when installing the nd280 software (this is because you need a rooti
# for which the python module was enables).
#
# Always run the macros from the directory in which you ran runAnalysis.py 
# with the -c option (if you have not done this then please read the
# EXAMPLES_README and the analysis turorial). To run this macro do:
#
# runAnalyses.py [-n nevents] -a exampleAnalysisScript [-o outputfile.root] path/2/MC/anal/inputfile.root
#
#_________________________________________________________________________

# import array module so can give ROOT its pointers
from array import array
# import the ROOT module
import ROOT
# import the userAnalysisBase class from userAnalysisBase.py
from userAnalysisBase import userAnalysisBase 

# class name must be the same as the filename without the .py
class exampleAnalysisScript(userAnalysisBase): # inherits from userAnalysisBase
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
        self.Trajectories = ROOT.TChain("TruthDir/Trajectories") 
        self.oaAnalysisModuleTChains.append(self.Trajectories)
 
    # _________________________________________________________________________
    def BookHistogramsAndAnalysisSteps(self):
        # Make a TCheatingAnalysisUtils object
        self.cheatingUtils = ROOT.TCheatingAnalysisUtils()

        # Define variables for output tree. As pointer don't exist in python we 
        # have to declare an array and then pass this as the branch address as 
        # ROOT expects a pointer.
        self._nleptontracks = array("i", [0.0]) 
        # Create tree and set branch addresses.
        self.outputTree = ROOT.TTree("trajectory_tree","trajectory_tree");
        self.outputTree.Branch("ntpcleptontracks", self._nleptontracks,"nleptontracks/I");

    # _________________________________________________________________________
    # EventAnalysis gets called for each event
    def EventAnalysis(self):
        # Abbreviations for use within this function.
        cheatingUtils = self.cheatingUtils 
        Trajectories = self.Trajectories

        # First fill the truth info from the GRooTrackerVertices
        NTrajsLepton = Trajectories.NTrajLepton
        NTraj = Trajectories.NTraj

        print "\n======== New event =========="
        print "        -> the TClonesArray contains "+str(NTrajsLepton)+" lepton trajectories";

        # Note that we access the first element of the array.
        self._nleptontracks[0] = NTrajsLepton

        for i in xrange(NTraj):
            # Example of accessing a data member of a TruthTrajectory.
            curr_traj = Trajectories.Trajectories[i]
            print "              ---> trajectory "+str(i)+" is for particle with pdg = "+str(curr_traj.PDG)
            # Example of calling an AnalysisUtils method. 
            useful = cheatingUtils.IsAUsefulTPCTrack(curr_traj);
            print "                   is it a usefull TPC track bool = "+str(useful)     
         
        # Fill the tree.
        self.outputTree.Fill()         

    # _________________________________________________________________________
    def WriteResultsToOutputFile(self):
        # Write out analysis results
        if self.rootOutput:
            self.rootOutput.cd()
            self.outputTree.Write()
            print "Found output file. Tree has been saved!"
        else:
            print "Unable to find output file. Tree not saved!"
            
