#! /usr/bin/env python
"""\
Python script to set up simple analyses.

If the readout library libReadoaAnalysis.so does not exist,
it  uses the first input  file specified to create  and compile the library.

If you have errors related to libReadoaAnalysis, it might be a good idea to
remove the libReadoaAnalysis directory and try again. The -c option is also
available, which purges all compiled files and rebuilds them.

Any time this is run, it will leave a file in the local directory called
oaAnalysisReadInitFile-$ND280ANALYSISTOOLSCONFIG.C which can be sourced by root
to help load in the approriate libraries.

The specifics of the analysis are contained in a separate file, which by
default is ccqeAnalysis.py. The -a option allows one to choose this name.

Sets up the eventChain, and loads headers as listed at the top of the
external analysis file.

The event loop is specified in doAnalysis() in the external file.

Python analyses with C++ utility scripts appear to run fast enough to be
used for large-scale analyses.
"""
import string
import shutil
import sys, traceback
import optparse
import os
import glob
import re
import math # cos, log, pi etc
import logging # print with logging.debug/info/warning/error/critical('message')
import subprocess

if not os.environ["ROOTSYS"]:
    print "Cannot find ROOTSYS environment variable! Run ... source thisroot.(c)sh ... first!"
    sys.exit(1)

import ROOT

# Define function to check if there is a symlink present in the nd280AnalysisTools directory path
def CheckForSymlink(nd280analysistoolsdir):
    dir_list = nd280analysistoolsdir.split("/")
    software_version_link = dir_list[-3]
    version_search = re.search('v[0-9]+r[0-9]+',software_version_link)
    dir_list.pop()
    dir_list.pop()
    link_path = '/'.join(dir_list)

    if not version_search and os.path.islink(link_path):
        software_version = os.readlink(link_path)
        nd280analysistoolsdir = nd280analysistoolsdir.replace(software_version_link,software_version)

    return nd280analysistoolsdir

def LoadPrerequisiteROOTLibs():
    ExtraROOTLibsToLoad = ["libGeom.so", "libPhysics.so", "libGui.so",
        "libCint.so", "libRooFitCore.so","libRooFit.so"]
    CINTLoadDirectives = ""
    for lib in ExtraROOTLibsToLoad:
        try:
            if ROOT.gSystem.Load(lib) == -1:
                raise
        except:
            print "[ERROR]: Failed to load library %s." % lib
            sys.exit(1)
        print "[INFO]: Loaded %s." % lib
        CINTLoadDirectives += "  gSystem->Load(\"%s\");\n" % lib
    sys.stdout.flush()
    return CINTLoadDirectives

RCO = False
def GetROOTCompileOpts():
    global RCO
    if RCO:
        return RCO
    command = ["root-config", "--cflags"]
    proc = subprocess.Popen(command,stdout=subprocess.PIPE)
    proc.wait()
    RCO = [x.strip() for x in proc.stdout.readline().split(" ")]
    return GetROOTCompileOpts()
RLO = False
def GetROOTLinkOpts():
    global RLO
    if RLO:
        return RLO
    command = ["root-config", "--libs"]
    proc = subprocess.Popen(command,stdout=subprocess.PIPE)
    proc.wait()
    RLO = [x.strip() for x in proc.stdout.readline().split(" ")]
    return GetROOTLinkOpts()

RCXX = False
def GetROOTCompiler():
    global RCXX
    if RCXX:
        return RCXX
    command = ["root-config", "--cxx"]
    proc = subprocess.Popen(command,stdout=subprocess.PIPE)
    proc.wait()
    RCXX = proc.stdout.readline().strip()
    return GetROOTCompiler()

CMTCPPFLAGS = False
def GetCMTcppFlags():
    global CMTCPPFLAGS
    if CMTCPPFLAGS:
        return CMTCPPFLAGS
    command = ["cmt", "show", "macro_value", "cppflags"]
    proc = subprocess.Popen(command,stdout=subprocess.PIPE)
    proc.wait()
    CMTCPPFLAGS = [x.strip() for x in proc.stdout.readline().split(" ")]
    CMTCPPFLAGS = filter(lambda x: len(x),CMTCPPFLAGS)
    print "[INFO]: CMT CPPFlags: %s" % GetCMTcppFlags()
    return GetCMTcppFlags()

CMTLINKOPTS = False
def GetCMTLinkOpts():
    global CMTLINKOPTS
    if CMTLINKOPTS:
        return CMTLINKOPTS
    command = ["cmt", "show", "macro_value", "linkopts"]
    proc = subprocess.Popen(command,stdout=subprocess.PIPE)
    proc.wait()
    CMTLINKOPTS = proc.stdout.readline().strip()
    return GetCMTLinkOpts()

AnaToolsDir = False
def GetAnaToolsDir():
    global AnaToolsDir
    if AnaToolsDir:
        return AnaToolsDir
    if not os.environ["ND280ANALYSISTOOLSROOT"]:
        print "[ERROR]: Cannot find ND280ANALYSISTOOLSROOT environment variable! Run ... source setup.(c)sh ... first!"
        sys.exit(1)
    AnaToolsDir = CheckForSymlink(os.environ["ND280ANALYSISTOOLSROOT"])
    return GetAnaToolsDir()

AnaToolsConfig = False
def GetAnaToolsConfigDir():
    global AnaToolsConfig
    if AnaToolsConfig:
        return AnaToolsConfig
    if not os.environ["ND280ANALYSISTOOLSCONFIG"]:
        print "[ERROR]: Cannot find ND280ANALYSISTOOLSROOT environment variable! Run ... source setup.(c)sh ... first!"
        sys.exit(1)
    AnaToolsConfig = "%s/%s" % (GetAnaToolsDir(), os.environ["ND280ANALYSISTOOLSCONFIG"])
    return GetAnaToolsConfigDir()

LocalAnaToolsVersion = False
def SetLocalAnaToolsDir(direct):
    global AnaToolsDir, AnaToolsConfig, LocalAnaToolsVersion
    AnaToolsDir = direct
    AnaToolsConfig = direct
    LocalAnaToolsVersion = True
    os.environ["LD_LIBRARY_PATH"] = os.environ["LD_LIBRARY_PATH"] + ":" + direct + "/AnalysisTools:" + direct + "/AnalysisTools/libReadoaAnalysis"
    print "[INFO]: Running with local anatools: %s" % direct

def GetAnaToolsBuildDir():
    return "%s/%s" % (GetAnaToolsConfigDir(), "AnalysisTools")


def GetAnaToolsSrcDir():
    return GetAnaToolsDir() + "/AnalysisTools"

def CheckNeedROOTDict(ToolLocationStub):
    LinkDefName = "%s_LinkDef.h" % ToolLocationStub
    return os.path.exists(LinkDefName)

def GenROOTDict(ToolLocationStub):
    ImplFile = "%s.cxx" % ToolLocationStub
    HeaderName = "%s.hxx" % ToolLocationStub
    DictName = "%sDict.cxx" % ToolLocationStub.split("/")[-1]
    DictLoc = GetAnaToolsConfigDir() + "/" + DictName
    LinkDefName = "%s_LinkDef.h" % ImplFile[:-4]
    command = ["rootcint", "-f", DictLoc, "-c", "-p",
        "-I%s" % GetAnaToolsSrcDir(),
        "-I%s" % GetAnaToolsConfigDir()]
    command.extend(GetCMTcppFlags())
    command.extend(GetROOTCompileOpts())
    command.extend([HeaderName,LinkDefName])
    print "[INFO]: Generating ROOT dictionary: %s " % " ".join(command)
    proc = subprocess.Popen(command,stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    proc.wait()
    print proc.stdout.read()
    if proc.returncode:
        print "[ERROR]: Failed to generate ROOT dictionary."
        sys.exit(1)
    return DictLoc

def GenDeps(ToolLocationStub):
    #Can only use GCC for this
    if not GetROOTCompiler() == "g++":
        return

    ImplFile = "%s.cxx" % ToolLocationStub
    command = [GetROOTCompiler(), "-MM", ImplFile]
    command.extend([
        "-I%s" % GetAnaToolsSrcDir(),
        "-I%s" % GetAnaToolsBuildDir()])
    command.extend(GetCMTcppFlags())
    command.extend(GetROOTCompileOpts())
    command.extend(GetROOTLinkOpts())

    DepsName = "%s_cxx.so.d" % ToolLocationStub.split("/")[-1]
    DepsLoc = GetAnaToolsConfigDir() + "/" + DepsName
    depsF = open(DepsLoc, "w")
    depsP = subprocess.Popen(command,stdout=depsF,stderr=subprocess.PIPE)
    depsP.wait()
    if depsP.returncode:
        print depsP.stderr.read()
        print "[ERROR]: Failed to generate dependencies for %s: %s" % (ImplFile, " ".join(command))
        sys.exit(1)
    print "[INFO]: Wrote %s dependencies to %s" % (ImplFile,DepsLoc)

def CheckDeps(ToolLocationStub):
    #Can only use GCC for this
    if not GetROOTCompiler() == "g++":
        print "[INFO]: Need recompile because not using gcc dependency generation to check whether we need to compile"
        return False

    DepsName = "%s_cxx.so.d" % ToolLocationStub.split("/")[-1]
    DepsLoc = GetAnaToolsConfigDir() + "/" + DepsName

    if not os.path.exists(DepsLoc):
        print "[INFO]: Need recompile because %s doesn't exist." % DepsLoc
        return False

    depsF = open(DepsLoc, "r")
    Lines = depsF.readlines()
    depsF.close()
    for l in Lines:
        if l.find(".o:") != -1:
            continue
        dep = l.strip()
        if dep[-1] == '\\':
            dep = dep[:-1].strip()
        if (os.path.getmtime(dep) > os.path.getmtime(DepsLoc)):
            print "[INFO]: Out of date because %s is newer than %s." % (dep, DepsLoc)
            return False
    return True

def CompileFile(ToolLocationStub, opts, Recompile):
    FileName = ToolLocationStub.split('/')[-1]
    ImplFile = "%s.cxx" % ToolLocationStub
    LibraryName = "%s_cxx.so" % FileName
    LibraryLocation = "%s/%s"  % (GetAnaToolsBuildDir(),LibraryName)

    if opts.UsesSystemCompiler:
        if not os.path.exists(LibraryLocation) or not CheckDeps(ToolLocationStub):
            command = [GetROOTCompiler(), ImplFile]
            if CheckNeedROOTDict(ToolLocationStub):
                DictLoc = GenROOTDict(ToolLocationStub)
                command.extend([DictLoc])
            command.extend(["-o", LibraryLocation, "-shared", "-fPIC", "-Wall"])
            command.extend([
                "-I%s" % GetAnaToolsSrcDir(),
                "-I%s" % GetAnaToolsBuildDir()])
            command.extend(GetCMTcppFlags())
            command.extend(GetROOTCompileOpts())
            command.extend(GetROOTLinkOpts())
            print "[INFO]: %s" % " ".join(command)
            proc = subprocess.Popen(command,stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            outs, errs = proc.communicate()
            print outs
            if proc.returncode:
                print "[ERROR]: Compilation of %s failed with command: %s" % (ImplFile," ".join(command))
                print errs
                sys.exit(1)
            sys.stdout.flush()
            GenDeps(ToolLocationStub)
    else:
        if os.path.exists(LibraryLocation):
            if (os.path.getmtime(ImplFile) > os.path.getmtime(LibraryLocation)) or Recompile:
                os.remove(LibraryLocation)
        try:
            if not ROOT.gSystem.CompileMacro(ImplFile,"kv"):
                raise
        except:
            print "[ERROR]: Failed to compile %s with:" % ImplFile
            print "[ACLIC]\tLibraries To Link: %s" % (ROOT.gSystem.GetLinkedLibs())
            print "[ACLIC]\tBuild Dir: %s" % (ROOT.gSystem.GetBuildDir())
            print "[ACLIC]\tMake Directive: %s" % (ROOT.gSystem.GetMakeSharedLib())
            sys.exit(1)

    print "[INFO]: Loading compiled library: %s " % LibraryLocation
    sys.stdout.flush()
    try:
        if ROOT.gSystem.Load(LibraryLocation) == -1:
            raise
    except:
        print "[ERROR] Wasn't able to load shared library " + LibraryLocation
        sys.exit(1)
    print "[INFO] Loaded precompiled shared library " + LibraryName

    return LibraryLocation
#######################################################################
# Parse the command line
optionParser = optparse.OptionParser(usage ="""\
usage: %prog [options] files
       When successful, always writes a file
       \"oaAnalysisReadInitFile-$ND280ANALYSISTOOLSCONFIG.C\", which
       can be executed in ROOT to manually initialise it for browsing oaAnalysis
       files (but not when just showing the help message).""")
optionParser.add_option("-n",type="int",dest="nEvents", default=1000000000,
        help="Set the number of events to process. Default is 1000000000.")
optionParser.add_option("-s",type="int",dest="nSkip", default=0,
        help="Set the number of events to skip. Default is 0.")
optionParser.add_option("-a",type="string",dest="analysisName",
        default="ccqeAnalysis", help="""\
Set the analysis script to read (analysisName.py) and the
default output name (analysisName.root). The latter can be
overridden with the -o option. The default analysisName is
'ccqeAnalysis'.""")
optionParser.add_option("--neut", action="store_const",dest="generatorType",
        const="NEUT", default="None",
        help="Force code to assume NEUT as the interaction generator.")
optionParser.add_option("--genie", action="store_const",dest="generatorType",
        const="GENIE", default="None",
        help="""\
Force code to assume GENIE as the interaction generator.
By default the generator names use the file catalogue, or are guessed.""")
optionParser.add_option("--nuance", action="store_const",dest="generatorType",
        const="NUANCE", default="None",
        help="""\
Force code to assume NUANCE as the interaction generator.
By default the generator names use the file catalogue, or are guessed.""")
optionParser.add_option("-o",type="string",dest="output", default=None,
        help="Set output file, overriding analysisName.root.")
optionParser.add_option("-f",action="store_true",dest="fullcompile", default=False,
        help="""\
Purge the existing compiled libraries and re-compile the C++ macros for use by ROOT
macros. An input oaAnalysis file needs to be specified. Not needed when using the
python-based analyses because compilation is automatic.""");
optionParser.add_option("-c",action="store_true",dest="compile", default=False,
        help="""\
Compile the C++ macros for use by ROOT macros. An input oaAnalysis file needs
to be specified. Not needed when using the python-based analyses because
compilation is automatic.  This will only compile a tool if it has changed
since the last compilation""");
optionParser.add_option("--system-compiler",action="store_true",dest="UsesSystemCompiler", default=False,
        help="""\
Use system compiler directly to compile analysis tools instead of TSystem.""");
optionParser.add_option("--ignore-version-info",action="store_false",dest="CareAboutVersionInfo", default=True,
        help="""\
If set compilation can continue even if input oaAnalysis file does not contain version info""");
optionParser.add_option("--roofit",action="store_true",dest="compileROOFit", default=False,
                        help="""\
Use this option if you also want to compile the ROOFit libraries.""");
optionParser.add_option("--noisyROOT", action="store_const",dest="noisyROOT",
        const=True, default=False,
        help="""\
Show all ROOT standard output, including those that are unnecessary and
often misleading in normal usage.""")
optionParser.add_option("--batch",action="store_false",dest="BatchMode",default=True,help="""\
Run in batch mode, no graphics etc.""")
optionParser.add_option("--loglevel", type="string", dest="loglevel",
        default='info',
        help="""\
Set the log level for the macro: debug, info, warning, error, critical.
in code print with logging.debug/info/warning/error/critical('message')""")
optionParser.add_option("-O", type="string", dest="useroption",
        action='append',
        help="""\
User option that may be passed to the analysis module. Multiple options may be specified.""")

# The remaining arguments are expected to be the input filenames
print "[ARGS]: %s" % sys.argv
(options, args) = optionParser.parse_args()
if len(args) < 1:
    optparse.OptionParser.print_help(optionParser)
    logging.critical("Need at least one input file")
    sys.exit(1);
else:
    print "[RUN ARGS]: %s" % args

firstInputFileName = args[0]

sys.stdout.flush()

RunDir = os.getcwd()

# Output filename is analysisName.root, unless specified on command line
if options.output == None:
    options.output = options.analysisName + ".root"

if options.BatchMode:
    ROOT.gROOT.SetBatch(True)

#######################################################################
# If cannot find in current working dir, copy AnalysisTools from macros
# directory in $ND280ANALYSISTOOLSROOT (previously used directory in which
# this python script was running).

if options.fullcompile:
    os.chdir(GetAnaToolsSrcDir())
    os.environ["PWD"] = os.getcwd()

if not os.path.samefile(os.path.abspath(GetAnaToolsSrcDir()),os.path.abspath(os.getcwd())):
    # We are not in the AnalysisTools directory, so create a copy of it then cd into it.
    if os.access("./AnalysisTools", os.R_OK):
        print """\
Using local version of "AnalysisTools". If there are problems, it may help
to remove this directory or copy any changes from the nd280AnalysisTools/vXrY/AnalysisTools
directory.
Output from diff --brief """ + GetAnaToolsSrcDir() + ":"
        os.system("diff --brief --exclude *.so --exclude *.d --exclude CVS " + GetAnaToolsSrcDir() + " ./AnalysisTools")
    else:
        print """\
Copying "AnalysisTools/" from the directory """ + GetAnaToolsSrcDir() + """
to use locally."""
        # Copy across everything except CVS directory
        os.system("rsync -av " + GetAnaToolsSrcDir() + "/*.*xx " + GetAnaToolsSrcDir() + "/*.so " + GetAnaToolsSrcDir() + "/*.h " + GetAnaToolsSrcDir() + "/*.d " + GetAnaToolsSrcDir() + "/*.list " + GetAnaToolsSrcDir() + "/*.list " + GetAnaToolsSrcDir() + "/*.py " + GetAnaToolsSrcDir() + "/*.C " + GetAnaToolsSrcDir() + "/*.dat ./AnalysisTools")
        if not os.access("./AnalysisTools", os.R_OK):
            print """\
Was unable to copy "AnalysisTools/" from the directory """ + GetAnaToolsSrcDir() + """
to use locally. Exiting."""
            sys.exit(1)
    SetLocalAnaToolsDir(os.getcwd())
    os.chdir("./AnalysisTools")
    os.environ["PWD"] = os.getcwd()

analysisToolsOrderFileName = GetAnaToolsSrcDir() + "/analysisToolsOrder.list"
analysisToolsFile = open(analysisToolsOrderFileName, 'r')
analysisToolsFileContents = analysisToolsFile.readlines()
analysisToolsFile.close()

# If we're just compiling the tools, see whether we really need to do any work.
# If we're up-to-date, we can save a lot of time by just bailing out now.
haveWorkToDo = True
libReadStamp = GetAnaToolsBuildDir() + "/libReadoaAnalysis/libReadoaAnalysis.stamp"
libReadBuildLocation = GetAnaToolsBuildDir() + "/libReadoaAnalysis/libReadoaAnalysis.so"
if not options.fullcompile:
    haveWorkToDo = False

    # We must have the libReadoaAnalysis.so
    if os.access(libReadBuildLocation, os.R_OK) == False:
        print "[INFO]: libReadoaAnalysis.so doesn't exist: we must re-compile"
        haveWorkToDo = True

    # The input-file.list mustn't have changed
    if not os.path.exists(libReadStamp):
        print "[INFO]: Stamp file: %s doesxn't exist. Rebuilding." % libReadStamp
        haveWorkToDo = True

    if not haveWorkToDo and (os.path.getmtime(firstInputFileName) > os.path.getmtime(libReadStamp)):
        print "File to compile(%s) has changed (%s > %s): we must re-compile" % \
            (firstInputFileName, os.path.getmtime(firstInputFileName), os.path.getmtime(libReadStamp))
        haveWorkToDo = True

    # We must have an up-to-date .so for all the tools the user wants
    if not haveWorkToDo:
        for line in analysisToolsFileContents:
            line = string.strip(line)

            if (not len(line)) or (line[0] == "#") or (line.find("cxx") == -1):
                continue

            fileRoot = re.sub(".cxx$", "", line)
            fileName = re.sub(".cxx$", "", line)
            SrcLoc = GetAnaToolsSrcDir() + "/" + fileRoot + ".cxx"
            LibLoc = GetAnaToolsBuildDir() + "/" + fileRoot + "_cxx.so"

            if not os.path.exists(SrcLoc):
                print "[ERROR]: File: %s does not exist but is included in %s"\
                    % (SrcLoc,analysisToolsOrderFileName)
                sys.exit(1)

            if not os.path.exists(LibLoc):
                print "[INFO]: Library is missing: we must re-compile (can't find " + LibLoc + ")"
                haveWorkToDo = True
                continue

            if not CheckDeps(fileRoot):
                print "[INFO]: Libraries are out-of-date: we must re-compile"
                haveWorkToDo = True
                continue
            print "[INFO]: %s up to date!" % (fileRoot + "_cxx.so")

if options.compile and not haveWorkToDo:
    print "Nothing to do!"
    sys.exit(0)

# Print a message out early on to let user know we are doing something
if options.fullcompile:
    print("[INFO]: Purging libraries in AnalysisTools/ and recompiling.")
else:
    print "[INFO]: Loading libraries. (Try runAnalyses.py -f oaAnalysisFile.root if this fails.)"

sys.stdout.flush()

#######################################################################
# Set up ROOT, by reading in the standard libraries
if os.access("rootlogon.C", os.R_OK):
    print "[INFO]: Reading rootlogon.C..."
    ROOT.gROOT.ProcessLine(".x rootlogon.C")
    print "[INFO]: done."

ExtraROOTLibDirectives = LoadPrerequisiteROOTLibs()

# Shut ROOT up for now unless verbose output specified.
if not options.noisyROOT:
    ROOT.gErrorIgnoreLevel = ROOT.kError

#######################################################################
# if "-f" option chosen, purge existing libraries
if options.fullcompile:
    print "Purging any compiled analysis tools and libReadoaAnalysis from directory", GetAnaToolsBuildDir()
    shutil.rmtree(GetAnaToolsBuildDir() + "/libReadoaAnalysis", ignore_errors=True)
    for compilationFile in glob.glob(GetAnaToolsBuildDir() + "/*.so") + glob.glob(GetAnaToolsBuildDir() + "/*.d"):
        os.remove(compilationFile)

#######################################################################
# Try to load the dictionary for the oaAnalysis input files. These are
# auto-generated in ./libReadoaAnalysis the first time round

if not os.path.exists(GetAnaToolsBuildDir()):
    os.makedirs(GetAnaToolsBuildDir())

RebuiltlibReadoaAnalysis = False
if not os.access(libReadBuildLocation, os.R_OK) or ROOT.gSystem.Load(libReadBuildLocation) == -1:
    print "[INFO]: Rebuilding %s from %s." % (libReadBuildLocation, firstInputFileName)
    sys.stdout.flush()
    # Use the first input file to generate a class library
    firstInputFile = ROOT.TFile(firstInputFileName, "READ")

    # Will now generated a compiled library for the input classes
    # Need to ensure '.' is in the path for this (which is annoying)
    ROOT.gSystem.Setenv("PATH", ROOT.gSystem.Getenv("PATH") + ":.")

    if not firstInputFile.IsOpen():
        print "[ERROR]: Cannot open first input file. Exiting!"
        sys.exit(1)

    status = firstInputFile.MakeProject("/".join(libReadBuildLocation.split("/")[:-1]), "*", "recreate++")
    firstInputFile.Close()
    RebuiltlibReadoaAnalysis = True

    libReadHeader = "/".join(libReadBuildLocation.split("/")[:-1]) + "/libReadoaAnalysisProjectHeaders.h"
    command = ['root','-l','-b',"exportVersion.C(\"" + libReadHeader + "\")"]
    if subprocess.call(command):
      if options.CareAboutVersionInfo:
        print "[ERROR]: Failed to add versioning info."
        sys.exit(1)
      else:
        print "[WARN]: Failed to add versioning info."
    else:
      print "[INFO]: Added versioning info to %s" % libReadHeader

    subprocess.call(["touch", libReadStamp])
    print "[INFO]: Created libReadoaAnalysis.so"

if ROOT.gSystem.Load(libReadBuildLocation) == -1:
    print "[ERROR]: Couldn't Load %s." % libReadBuildLocation
    sys.exit(1)

print "[INFO]: Loaded %s" % libReadBuildLocation
sys.stdout.flush()

# Start building ROOT init file to write out as "./oaAnalysisReadInitFile-$ND280ANALYSISTOOLSCONFIG.C"
oaAnalysisReadInitContents  = "{\n"
oaAnalysisReadInitContents += "  gInterpreter->AddIncludePath(\"" + GetAnaToolsSrcDir() + "\");\n"
oaAnalysisReadInitContents += "  gSystem->SetBuildDir(\"" + GetAnaToolsBuildDir()  + "\",1);\n"
oaAnalysisReadInitContents += ExtraROOTLibDirectives
oaAnalysisReadInitContents += "  gSystem->Load(\"" + libReadBuildLocation + "\");\n"

#######################################################################
# Try to load all the utility classes that can be found in ./AnalysisTools/,
# recompiling them if they have changed

# Lower message level to kWarning if previously suppressed to kError.
if not options.noisyROOT:
    ROOT.gErrorIgnoreLevel = ROOT.kWarning

readInitFile = os.getcwd() + "/oaAnalysisReadInitFile-"\
    + os.environ["ND280ANALYSISTOOLSCONFIG"] + ".C"

if not options.UsesSystemCompiler:
    #Set the build dir
    ROOT.gSystem.AddIncludePath("-I"+GetAnaToolsSrcDir())
    ROOT.gSystem.SetBuildDir(GetAnaToolsBuildDir(),1)

for line in analysisToolsFileContents:
    line = string.strip(line)

    if not len(line):
        continue

    if line[0] == "#":
        print "[INFO]: Skipping commented AnalysisTool %s" % line[1:-1]
        continue

    fileRoot = re.sub(".cxx$", "", line)
    fileRoot = GetAnaToolsSrcDir() + "/" + fileRoot
    print "[BUILD]: %s " % fileRoot
    LibLoc = CompileFile(fileRoot, options, RebuiltlibReadoaAnalysis)
    oaAnalysisReadInitContents += "  gSystem->Load(\"%s\");\n" % LibLoc

######################################################################
# Write out a root macro file oaAnalysisReadInitFile-$ND280ANALYSISTOOLSCONFIG.C, which loads all the
# libraries when executed. Its contents are built up above in
# oaAnalysisReadInitContents

oaAnalysisReadInitContents += "  gSystem->SetBuildDir(gSystem->WorkingDirectory(),1);\n"

oaAnalysisReadInitContents += "}\n"
oaAnalysisReadInitFile = open(readInitFile, 'w')
oaAnalysisReadInitFile.write(oaAnalysisReadInitContents)
oaAnalysisReadInitFile.close()

# Choose if you want to keep the ROOFit libraries
if options.compileROOFit:
    print "Write file oaAnalysisReadInitFile-$ND280ANALYSISTOOLSCONFIG.C, which can be executed to initialiase ROOT"
else:
    file('oaAnalysisReadInitialFile.C',"w").writelines([l for l in file(readInitFile).readlines() if 'Weighting' not in l])
    os.remove(readInitFile)
    shutil.move('oaAnalysisReadInitialFile.C',readInitFile)
    print "Write file oaAnalysisReadInitFile-$ND280ANALYSISTOOLSCONFIG.C, which can be executed to initialiase ROOT"

# if "-c" or "-f" option chosen, state job done and exit
if options.compile or options.fullcompile:
    print "runAnalyses.py: finished compilation job"
    sys.exit(0)

#Move back to the directory we ran the script in
if LocalAnaToolsVersion:
    os.chdir(RunDir)
    os.environ["PWD"] = os.getcwd()

# Set the logging level
LEVELS = {'debug': logging.DEBUG, 'info': logging.INFO, 'warning': logging.WARNING,
          'error': logging.ERROR, 'critical': logging.CRITICAL}
logging.basicConfig(level=LEVELS.get(options.loglevel, logging.NOTSET))

# If doing an analysis, load the oaAnalysisReadInitFile-$ND280ANALYSISTOOLSCONFIG.C
ROOT.gROOT.ProcessLine(".x " + readInitFile)

#######################################################################
# Download PDF files for ECAL PID Tool

ecalPdfDir = os.environ["ND280ANALYSISTOOLSROOT"]+"/ecalPidPdfs"
if not os.path.exists(ecalPdfDir):
    #run the script to download the likelihood pdf files
    os.system("bash " + os.environ["ND280ANALYSISTOOLSROOT"] + "/nd280AnalysisTools-get-likelihood-pdfs.sh")

#######################################################################

AnalysisFile = ""
if options.analysisName[-3:] == ".py":
    AnalysisFile = options.analysisName
else:
    AnalysisFile = options.analysisName + ".py"

AnalysisName = AnalysisFile.split("/")[-1][:-3]

print ("[INFO]: Looking for the selected analysis class " + AnalysisName +
        " from " + AnalysisFile)

copyAnalysisClass = 0
copyBaseClass = 0

if not os.access("__init__.py", os.R_OK):
    subprocess.call(["touch", "__init__.py"])

if not os.access(AnalysisName + ".py", os.R_OK):
    if os.access(AnalysisFile, os.R_OK):
        shutil.copy(AnalysisFile, os.getcwd())
    elif os.access(os.environ["ND280ANALYSISTOOLSROOT"] + "/macros/" + AnalysisName + ".py", os.R_OK):
        shutil.copy(os.environ["ND280ANALYSISTOOLSROOT"] + "/macros/" + AnalysisName + ".py", os.getcwd())
    else:
        print "[ERROR]: Couldn't find Analysis implementation for %s." % options.analysisName
        print "[INFO]: Tried: %s, %s, and %s." % (AnalysisName + ".py",AnalysisFile,os.environ["ND280ANALYSISTOOLSROOT"] + "/macros/" + AnalysisName + ".py")
        sys.exit(1)
    copyAnalysisClass = 1

if not os.access("userAnalysisBase.py", os.R_OK):
    shutil.copy(os.environ["ND280ANALYSISTOOLSROOT"] + "/macros/userAnalysisBase.py", os.getcwd())
    copyBaseClass = 1

print "[INFO]: Importing \"%s\"..." % AnalysisName
AnalysisModule = __import__(AnalysisName)
# (dynamic specification of class analysisName in file analysisName.py)
print "[INFO]: Getting class %s from module" % AnalysisName
analysisClass = getattr(AnalysisModule, AnalysisName)

# after this, analysis represents an instance of the user analysis class
# derived from userAnalysisBase
analysis = analysisClass(options)

#######################################################################
# Perform the analysis
try:
    analysis.OpenInputFiles(args)
    analysis.InitialiseAnalysis()
    analysis.ProcessEventLoop()
    analysis.FinishAnalysis()
except:
    traceback.print_exc()


if copyAnalysisClass :
    print "Removing copied analysis code"
    os.remove(AnalysisName + ".py")
    os.remove(AnalysisName + ".pyc")

if copyBaseClass :
    print "Removing copied userAnalysisBase.py class"
    os.remove("userAnalysisBase.py")
    os.remove("userAnalysisBase.pyc")

