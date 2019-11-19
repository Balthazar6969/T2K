import re
import ROOT

# Names of all the detectors.
detStrings = [ "TPC1", "TPC2", "TPC3", 
               "FGD1", "FGD2", 
               "P0D", 
               "DSECal", 
               "RTECal", "LTECal", "TTECal", "BTECal", 
               "RPECal", "LPECal", "TPECal", "BPECal", 
               "RSMRD", "LSMRD", "TSMRD", "BSMRD" 
             ]

# The subdetectors that provide a good momentum estimate
detStringsGoodMom = [ "TPC", "FGD" ]

def DetGetsRefit(det):
    """
    Whether this detector has its objects refitted by RECPACK.
    
    Currently, this is true for P0D objects.

    Keyword arguments:
    det - A string containing the subdetector's name
    """
    return (det.find("P0D") != -1)

def DetHasGoodMom(det):
    """
    Whether this detector returns a good momentum estimate.

    Keyword arguments:
    det - A string containing the subdetector's name.
    """
    global detStringsGoodMom

    for testDet in detStringsGoodMom:
        if det.find(testDet) != -1:
            return True

    return False

def GetDet(string):
    """
    Get the "bare" version of a subdetector's name, without parentheses. 

    Keyword arguments:
    string - A string containing the subdetector's name.
    """

    return string.replace("(","").replace(")","")

# List of currently open files for use in PrintEvent().
__EVENTFILES__ = {}

def PrintEvent(object, file, comment):
    """
    Print the details of an event to the specified filename. 
    
    This is useful for helping produce skimmed files of events
    with certain characteristics.

    The output format is:
    <run>,<subrun>,<event>,<comment>

    Keyword arguments:
    object  - The analysis currently being run. 
    file    - The string to append to the current output file name (e.g. "Failures.txt").
    comment - Details to be printed after the run, subrun and event numbers.
    """
    global __EVENTFILES__
    if file not in __EVENTFILES__:
        __EVENTFILES__[file] = open(object.rootOutput.GetName() + "-" + file, 'w')

    run = -1
    subrun = -1
    event = -1

    try:
        run = object.ReconPerfEval.RunID 
        subrun = object.ReconPerfEval.SubrunID
        event = object.ReconPerfEval.EventID
    except:
        try:
            run = object.NoiseModule.RunID 
            subrun = object.NoiseModule.SubrunID
            event = object.NoiseModule.EventID
        except:
            run = object.PerfModule.RunID 
            subrun = object.PerfModule.SubrunID
            event = object.PerfModule.EventID

    __EVENTFILES__[file].write(str(run) + "," + str(subrun) + "," + str(event) + "," + comment + "\n")

    return

# Number of structs we've created in ConvertDictToTTree.
__NUMSTRUCTS__ = 0

def ConvertDictToTTree(dict, name):
    """
    Convert a python dict to a ROOT TTree so it can be written to a file.

    The following type conversions are currently supported for values:
    int   ==> Int_t
    float ==> Double_t
    str   ==> TString

    All keys in the dict are treated as str.

    This function works by dynamically generating a C++ struct corresponding
    to the dict. The TTree has its branches set to the addresses of the
    relevant entry in the struct. The struct is filled, then the TTree written.

    Keyword arguments:
    dict - The dictionary to convert to a TTree.
    name - The name to give the TTree.

    Returns:
    The ROOT TTree version of the dict.
    """

    # Create a C++ struct corresponding to the input dict. The struct needs
    # a unique name, so use the __NUMSTRUCTS__ counter as part of it.
    global __NUMSTRUCTS__
    structName = "TreeStruct" + str(__NUMSTRUCTS__)
    string = "struct " + structName + " {"

    for key, value in dict.iteritems():
        key  = str(key)
        type = "Int_t"
        if isinstance(value, int):
            type = "Int_t"
        elif isinstance(value, float):
            type = "Double_t"
        elif isinstance(value, str):
            type = "TString"
        string += " " + type + " " + key + ";"
    string += "};"

    flatTree = ROOT.TTree(name, name)

    # Make ROOT aware of our struct, then import it from the ROOT namespace
    ROOT.gROOT.ProcessLine(string)
    __import__("ROOT", globals(), locals(), [structName])

    # Create a new object based on our struct, and setup the branches of the tree to read from it
    flatTreeStruct = eval("ROOT." + structName + "()")

    for key, value in dict.iteritems():
        key = str(key)
        if isinstance(value, int):
            flatTree.Branch(key, ROOT.AddressOf(flatTreeStruct, key), key + "/I")
        elif isinstance(value, float):
            flatTree.Branch(key, ROOT.AddressOf(flatTreeStruct, key), key + "/D")
        elif isinstance(value, str):
            flatTree.Branch(key, ROOT.AddressOf(flatTreeStruct, key), 16000, 0)

    # Fill the struct...
    for key, value in dict.iteritems():  
        key = str(key)
        if isinstance(value, str):
            value = ROOT.TString(value)
        setattr(flatTreeStruct, key, value)

    # ...and fill the Tree from the struct
    flatTree.Fill()
    __NUMSTRUCTS__ += 1

    return flatTree

def RecurseOutput(object, dir):
    """
    Recursively write out histogram dict to output file. 
    
    If the current object is an iterable (i.e. a dict), a new TDirectory is created.
    If the object is not an iterable, it is written to the current directory.

    Keyword arguments:
    object - The current object to be written.
    dir    - The current directory to write to.
    """

    if getattr(object, '__iter__', False):
        for subdirname, subobject in object.iteritems():
            subdir = dir.mkdir(subdirname)
            subdir.cd()
            RecurseOutput(subobject, subdir)
            dir.cd()
    else:
        dir.cd()
        object.Write()

    return

def GetMinMax(box):
    """ 
    Redefine the minimum and maximum edges of a box to be the minimum and maximum corners in the global coordinates.
    
    Keyword arguments:
    box - The geometry object read in from the geometry summary module.
    """

    boxmin = box.Minimum
    boxmax = box.Maximum

    actualmin = ROOT.TVector3(min([boxmin.X(), boxmax.X()]), min([boxmin.Y(), boxmax.Y()]), min([boxmin.Z(), boxmax.Z()]))
    actualmax = ROOT.TVector3(max([boxmin.X(), boxmax.X()]), max([boxmin.Y(), boxmax.Y()]), max([boxmin.Z(), boxmax.Z()]))

    return [actualmin, actualmax]

def IsInFGDFV(geometry, position):
    """
    DEPRECATED - Use volume_cuts instead.

    Whether an object is in the FGD FV or not.

    Keyword arguments:
    geometry - The geometry summary module.
    position - The TVector3 to test if it's in the FGD FV.
    """

    [fgd1min, fgd1max] = GetMinMax(geometry.FGD1Active)
    [fgd2min, fgd2max] = GetMinMax(geometry.FGD2Active)

    inFGD = False
    if (fgd1min.X() < position.X() < fgd1max.X()) and (fgd1min.Y() < position.Y() < fgd1max.Y()) and (fgd1min.Z() < position.Z() < fgd1max.Z()):
        inFGD = 1
    if (fgd2min.X() < position.X() < fgd2max.X()) and (fgd2min.Y() < position.Y() < fgd2max.Y()) and (fgd2min.Z() < position.Z() < fgd2max.Z()):
        inFGD = 2

    return inFGD

def ContainsTracker(str):
    """
    Whether a given subdetector string contains tracker constituents.

    Keyword arguments:
    str - The subdetector string to test.
    """

    if str.find("TPC") != -1 or str.find("FGD") != -1:
        return True

    return False

def IsOnlyTracker(str):
    """
    Whether a given subdetector string contains only tracker constituents.

    Keyword arguments:
    str - The subdetector string to test.
    """

    if (not ContainsTracker(str)) or str.find("ECal") != -1 or str.find("P0D") != -1 or str.find("SMRD") != -1:
        return False

    return True

def ConvertPathToSummary(path):
    """
    Create a summarised version of the the subdetector string.

    This is useful for combining several topologies into summarised versions.

    Keyword arguments:
    path - The path to convert.
    """

    # Convert to module type rather than specific module.
    subs = {"BSMRD" : "xSMRD",
            "TSMRD" : "xSMRD",
            "RSMRD" : "xSMRD",
            "LSMRD" : "xSMRD",
            "DSECal" : "DSECal",
            "LTECal" : "xTECal",
            "RTECal" : "xTECal",
            "TTECal" : "xTECal",
            "BTECal" : "xTECal",
            "BPECal" : "xPECal",
            "TPECal" : "xPECal",
            "RPECal" : "xPECal",
            "LPECal" : "xPECal",
            "FGD1" : "FGDn",
            "FGD2" : "FGDn",
            "TPC1" : "TPCn",
            "TPC2" : "TPCn",
            "TPC3" : "TPCn",
            "P0D" : "P0D"}

    for old, new in subs.iteritems():
        path = re.sub(old, new, path)

    # Now group all the tracker topologies together.
    subs2 ={"TPCn-FGDn" : "TRACKER",
            "TPCn-TPCn" : "TRACKER",
            "TPCn-TPCn-FGDn" : "TRACKER",
            "TPCn-TPCn-TPCn-FGDn" : "TRACKER",
            "TPCn-FGDn-FGDn" : "TRACKER",
            "TPCn-TPCn-FGDn-FGDn" : "TRACKER",
            "TPCn-TPCn-TPCn-FGDn-FGDn" : "TRACKER",
            }

    for old in sorted(subs2, key=lambda o: len(o), reverse=True):
        new = subs2[old]
        path = re.sub(old, new, path)

    return path

