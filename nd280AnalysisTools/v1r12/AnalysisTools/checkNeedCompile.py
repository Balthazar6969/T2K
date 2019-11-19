import string
import os
import sys
import re
haveWorkToDo = 0

if (len(sys.argv) != 3):
  print "Bad options " + str(len(sys.argv))
  sys.exit(1)

localversion = sys.argv[1]
nd280analysistoolsdir = sys.argv[2]

analysisToolsFile=open(os.getcwd() + "/analysisToolsOrder.list", 'r')
analysisToolsFileContents = analysisToolsFile.readlines()
analysisToolsFile.close()

# We must have the libReadoaAnalysis.so
if os.access(os.getcwd() + "/libReadoaAnalysis.so", os.R_OK) == False:
    print "libReadoaAnalysis.so doesn't exist: we must re-compile"
    haveWorkToDo = 1

# The input-file.list mustn't have changed
stamp = nd280analysistoolsdir + "/" + os.getenv("ND280ANALYSISTOOLSCONFIG") + "/nd280AnalysisTools.stamp"
if not os.path.exists(stamp) or os.path.getmtime("input-file.list") > os.path.getmtime(stamp):
    print "input-file.list has changed: we must re-compile"
    haveWorkToDo = 1

# We must have an up-to-date .so for all the tools the user wants
for line in analysisToolsFileContents:
    line = string.strip(line)

    if (line.find("cxx") == -1):
        continue

    fileRoot = re.sub(".cxx$", "", line)
    fileName = re.sub(".cxx$", "", line)

    if localversion:
        fileRoot = os.getenv("PWD") + "/AnalysisTools/" + fileRoot
    else :
        fileRoot = nd280analysistoolsdir + "/AnalysisTools/" + fileRoot

    if not os.path.exists(fileRoot + ".cxx"):
        continue

    if not os.path.exists(fileRoot + "_cxx.so") or not os.path.exists(fileRoot + "_cxx.d"):
        print "Libraries are missing: we must re-compile"
        haveWorkToDo = 1
        break

    if os.path.getmtime(fileRoot + ".cxx") > os.path.getmtime(fileRoot + "_cxx.so") or os.path.getmtime(fileRoot + ".hxx") > os.path.getmtime(fileRoot + "_cxx.so"):
        print "Libraries are out-of-date: we must re-compile"
        haveWorkToDo = 1
        break

sys.exit(haveWorkToDo)
